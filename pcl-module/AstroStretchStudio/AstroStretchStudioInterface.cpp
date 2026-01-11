// ----------------------------------------------------------------------------
// AstroStretchStudio Interface Implementation
// ----------------------------------------------------------------------------

#include "AstroStretchStudioInterface.h"
#include "AstroStretchStudioProcess.h"
#include "AstroStretchStudioParameters.h"
#include "WebViewContent.h"  // Generated file with embedded HTML

#include <pcl/Console.h>
#include <pcl/ErrorHandler.h>
#include <pcl/View.h>
#include <pcl/ImageWindow.h>
#include <pcl/JSON.h>
#include <pcl/Base64.h>

namespace pcl
{

// ----------------------------------------------------------------------------

AstroStretchStudioInterface* TheAstroStretchStudioInterface = nullptr;

// ----------------------------------------------------------------------------

AstroStretchStudioInterface::AstroStretchStudioInterface()
   : m_instance( TheAstroStretchStudioProcess )
{
   TheAstroStretchStudioInterface = this;
}

// ----------------------------------------------------------------------------

AstroStretchStudioInterface::~AstroStretchStudioInterface()
{
   if ( GUI != nullptr )
      delete GUI, GUI = nullptr;
}

// ----------------------------------------------------------------------------

IsoString AstroStretchStudioInterface::Id() const
{
   return "AstroStretchStudio";
}

// ----------------------------------------------------------------------------

MetaProcess* AstroStretchStudioInterface::Process() const
{
   return TheAstroStretchStudioProcess;
}

// ----------------------------------------------------------------------------

String AstroStretchStudioInterface::IconImageSVG() const
{
   return TheAstroStretchStudioProcess->IconImageSVG();
}

// ----------------------------------------------------------------------------

InterfaceFeatures AstroStretchStudioInterface::Features() const
{
   return InterfaceFeature::Default | InterfaceFeature::RealTimeButton;
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::ApplyInstance() const
{
   m_instance.LaunchOnCurrentView();
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::ResetInstance()
{
   m_instance.SetDefaultParameters();
   SendParametersToWebView();
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInterface::Launch( const MetaProcess& P,
                                           const ProcessImplementation* p,
                                           bool& dynamic,
                                           unsigned& /*flags*/ )
{
   if ( GUI == nullptr )
   {
      GUI = new GUIData( *this );
      SetWindowTitle( "AstroStretch Studio" );
      InitializeWebView();
   }

   if ( p != nullptr )
      ImportProcess( *p );

   dynamic = false;
   return &P == TheAstroStretchStudioProcess;
}

// ----------------------------------------------------------------------------

ProcessImplementation* AstroStretchStudioInterface::NewProcess() const
{
   return new AstroStretchStudioInstance( m_instance );
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInterface::ValidateProcess( const ProcessImplementation& p,
                                                    String& whyNot ) const
{
   if ( dynamic_cast<const AstroStretchStudioInstance*>( &p ) != nullptr )
      return true;
   whyNot = "Not an AstroStretchStudio instance.";
   return false;
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInterface::RequiresInstanceValidation() const
{
   return true;
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInterface::ImportProcess( const ProcessImplementation& p )
{
   m_instance.Assign( p );
   SendParametersToWebView();
   return true;
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInterface::WantsImageNotifications() const
{
   return true;
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::ImageUpdated( const View& view )
{
   if ( GUI != nullptr && IsVisible() )
   {
      if ( view == m_currentView )
         SendImageToWebView( view );
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::ImageFocused( const View& view )
{
   if ( GUI != nullptr && IsVisible() )
   {
      m_currentView = view;
      SendImageToWebView( view );
   }
}

// ----------------------------------------------------------------------------
// WebView Integration
// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::InitializeWebView()
{
   // Load embedded HTML content
   GUI->WebView_Control.SetContent( String( WEBVIEW_HTML_CONTENT ) );

   // Set up message handler
   GUI->WebView_Control.OnScriptMessageAvailable(
      [this]( WebView& sender, const Variant& message )
      {
         if ( message.IsString() )
            OnWebViewMessage( sender, message.ToString() );
      }
   );

   // Send initial parameters
   SendParametersToWebView();

   // Send current image if available
   ImageWindow w = ImageWindow::ActiveWindow();
   if ( !w.IsNull() )
   {
      m_currentView = w.CurrentView();
      SendImageToWebView( m_currentView );
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::SendParametersToWebView()
{
   if ( GUI == nullptr )
      return;

   // Build JSON with current parameters
   String json = String().Format(
      "{"
      "\"type\":\"setParameters\","
      "\"algorithm\":\"%s\","
      "\"ots\":{"
         "\"objectType\":\"%s\","
         "\"backgroundTarget\":%.4f,"
         "\"stretchIntensity\":%.3f,"
         "\"protectHighlights\":%.3f,"
         "\"preserveColor\":%s"
      "},"
      "\"sas\":{"
         "\"numScales\":%d,"
         "\"backgroundTarget\":%.4f,"
         "\"fineScaleGain\":%.2f,"
         "\"midScaleGain\":%.2f,"
         "\"coarseScaleGain\":%.2f,"
         "\"compressionAlpha\":%.1f,"
         "\"highlightProtection\":%.3f,"
         "\"noiseThreshold\":%.5f,"
         "\"flattenBackground\":%s,"
         "\"preserveColor\":%s"
      "}"
      "}",
      ( m_instance.p_algorithm == ASSAlgorithm::OTS ) ? "ots" : "sas",
      TheASSOTSObjectTypeParameter->ElementId( m_instance.p_otsObjectType ).c_str(),
      m_instance.p_otsBackgroundTarget,
      m_instance.p_otsStretchIntensity,
      m_instance.p_otsProtectHighlights,
      m_instance.p_otsPreserveColor ? "true" : "false",
      m_instance.p_sasNumScales,
      m_instance.p_sasBackgroundTarget,
      m_instance.p_sasFineScaleGain,
      m_instance.p_sasMidScaleGain,
      m_instance.p_sasCoarseScaleGain,
      m_instance.p_sasCompressionAlpha,
      m_instance.p_sasHighlightProtection,
      m_instance.p_sasNoiseThreshold,
      m_instance.p_sasFlattenBackground ? "true" : "false",
      m_instance.p_sasPreserveColor ? "true" : "false"
   );

   GUI->WebView_Control.EvaluateScript(
      String().Format( "window.postMessage(%s, '*')", json.c_str() )
   );
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::SendImageToWebView( const View& view )
{
   if ( GUI == nullptr || view.IsNull() )
      return;

   try
   {
      ImageVariant image = view.Image();
      if ( image.IsComplexSample() )
         return;

      int w = image.Width();
      int h = image.Height();

      // Create RGBA data for WebView
      ByteArray rgba( w * h * 4 );

      if ( image.IsColor() )
      {
         for ( int y = 0; y < h; ++y )
            for ( int x = 0; x < w; ++x )
            {
               size_type idx = ( y * w + x ) * 4;
               rgba[idx + 0] = uint8( Range( image( x, y, 0 ), 0.0, 1.0 ) * 255 );
               rgba[idx + 1] = uint8( Range( image( x, y, 1 ), 0.0, 1.0 ) * 255 );
               rgba[idx + 2] = uint8( Range( image( x, y, 2 ), 0.0, 1.0 ) * 255 );
               rgba[idx + 3] = 255;
            }
      }
      else
      {
         for ( int y = 0; y < h; ++y )
            for ( int x = 0; x < w; ++x )
            {
               size_type idx = ( y * w + x ) * 4;
               uint8 v = uint8( Range( image( x, y, 0 ), 0.0, 1.0 ) * 255 );
               rgba[idx + 0] = v;
               rgba[idx + 1] = v;
               rgba[idx + 2] = v;
               rgba[idx + 3] = 255;
            }
      }

      // Encode as base64
      IsoString base64 = IsoString::ToBase64( rgba );

      // Send to WebView
      String json = String().Format(
         "{\"type\":\"setImage\",\"width\":%d,\"height\":%d,\"data\":\"%s\"}",
         w, h, base64.c_str()
      );

      GUI->WebView_Control.EvaluateScript(
         String().Format( "window.postMessage(%s, '*')", json.c_str() )
      );
   }
   catch ( ... )
   {
      // Silently ignore errors
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::OnWebViewMessage( WebView& sender, const String& message )
{
   try
   {
      // Parse JSON message
      JSONValue json = JSON::Parse( message.ToUTF8() );
      if ( !json.IsObject() )
         return;

      String type = json["type"].ToString();

      if ( type == "parametersChanged" )
      {
         // Update instance from WebView parameters
         String algo = json["algorithm"].ToString();
         m_instance.p_algorithm = ( algo == "ots" ) ? ASSAlgorithm::OTS : ASSAlgorithm::SAS;

         if ( json.HasMember( "ots" ) )
         {
            JSONValue ots = json["ots"];

            String objType = ots["objectType"].ToString();
            if ( objType == "nebula" )       m_instance.p_otsObjectType = ASSOTSObjectType::Nebula;
            else if ( objType == "galaxy" )  m_instance.p_otsObjectType = ASSOTSObjectType::Galaxy;
            else if ( objType == "starCluster" ) m_instance.p_otsObjectType = ASSOTSObjectType::StarCluster;
            else if ( objType == "darkNebula" )  m_instance.p_otsObjectType = ASSOTSObjectType::DarkNebula;

            m_instance.p_otsBackgroundTarget = ots["backgroundTarget"].ToDouble();
            m_instance.p_otsStretchIntensity = ots["stretchIntensity"].ToDouble();
            m_instance.p_otsProtectHighlights = ots["protectHighlights"].ToDouble();
            m_instance.p_otsPreserveColor = ots["preserveColor"].ToBool();
         }

         if ( json.HasMember( "sas" ) )
         {
            JSONValue sas = json["sas"];

            m_instance.p_sasNumScales = sas["numScales"].ToInt();
            m_instance.p_sasBackgroundTarget = sas["backgroundTarget"].ToDouble();
            m_instance.p_sasFineScaleGain = sas["fineScaleGain"].ToDouble();
            m_instance.p_sasMidScaleGain = sas["midScaleGain"].ToDouble();
            m_instance.p_sasCoarseScaleGain = sas["coarseScaleGain"].ToDouble();
            m_instance.p_sasCompressionAlpha = sas["compressionAlpha"].ToDouble();
            m_instance.p_sasHighlightProtection = sas["highlightProtection"].ToDouble();
            m_instance.p_sasNoiseThreshold = sas["noiseThreshold"].ToDouble();
            m_instance.p_sasFlattenBackground = sas["flattenBackground"].ToBool();
            m_instance.p_sasPreserveColor = sas["preserveColor"].ToBool();
         }
      }
      else if ( type == "apply" )
      {
         ApplyInstance();
      }
      else if ( type == "reset" )
      {
         ResetInstance();
      }
      else if ( type == "requestImage" )
      {
         ImageWindow w = ImageWindow::ActiveWindow();
         if ( !w.IsNull() )
         {
            m_currentView = w.CurrentView();
            SendImageToWebView( m_currentView );
         }
      }
   }
   catch ( ... )
   {
      // Ignore parse errors
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::e_Click( Button& sender, bool checked )
{
   if ( sender == GUI->Apply_Button )
   {
      ApplyInstance();
   }
   else if ( sender == GUI->Reset_Button )
   {
      ResetInstance();
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInterface::e_Timer( Timer& sender )
{
   // Debounced update handling if needed
}

// ----------------------------------------------------------------------------
// GUI Construction
// ----------------------------------------------------------------------------

AstroStretchStudioInterface::GUIData::GUIData( AstroStretchStudioInterface& w )
{
   // WebView takes most of the space
   WebView_Control.SetMinSize( 900, 700 );

   // Bottom buttons
   Apply_Button.SetText( "Apply" );
   Apply_Button.SetIcon( w.ScaledResource( ":/icons/execute.png" ) );
   Apply_Button.OnClick( (Button::click_event_handler)&AstroStretchStudioInterface::e_Click, w );

   Reset_Button.SetText( "Reset" );
   Reset_Button.SetIcon( w.ScaledResource( ":/icons/reload.png" ) );
   Reset_Button.OnClick( (Button::click_event_handler)&AstroStretchStudioInterface::e_Click, w );

   Buttons_Sizer.SetSpacing( 8 );
   Buttons_Sizer.AddStretch();
   Buttons_Sizer.Add( Reset_Button );
   Buttons_Sizer.Add( Apply_Button );

   Global_Sizer.SetMargin( 8 );
   Global_Sizer.SetSpacing( 8 );
   Global_Sizer.Add( WebView_Control, 100 );
   Global_Sizer.Add( Buttons_Sizer );

   w.SetSizer( Global_Sizer );

   w.EnsureLayoutUpdated();
   w.AdjustToContents();
}

// ----------------------------------------------------------------------------

} // namespace pcl

// ----------------------------------------------------------------------------
