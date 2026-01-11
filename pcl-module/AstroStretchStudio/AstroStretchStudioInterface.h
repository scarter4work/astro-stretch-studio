// ----------------------------------------------------------------------------
// AstroStretchStudio Interface Header
// ----------------------------------------------------------------------------

#ifndef __AstroStretchStudioInterface_h
#define __AstroStretchStudioInterface_h

#include <pcl/ProcessInterface.h>
#include <pcl/Sizer.h>
#include <pcl/WebView.h>
#include <pcl/PushButton.h>
#include <pcl/Timer.h>

#include "AstroStretchStudioInstance.h"

namespace pcl
{

// ----------------------------------------------------------------------------

class AstroStretchStudioInterface : public ProcessInterface
{
public:

   AstroStretchStudioInterface();
   virtual ~AstroStretchStudioInterface();

   IsoString Id() const override;
   MetaProcess* Process() const override;
   String IconImageSVG() const override;
   InterfaceFeatures Features() const override;
   void ApplyInstance() const override;
   void ResetInstance() override;
   bool Launch( const MetaProcess&, const ProcessImplementation*, bool& dynamic, unsigned& flags ) override;
   ProcessImplementation* NewProcess() const override;
   bool ValidateProcess( const ProcessImplementation&, String& whyNot ) const override;
   bool RequiresInstanceValidation() const override;
   bool ImportProcess( const ProcessImplementation& ) override;
   bool WantsImageNotifications() const override;
   void ImageUpdated( const View& ) override;
   void ImageFocused( const View& ) override;

private:

   AstroStretchStudioInstance m_instance;

   // UI Elements
   struct GUIData
   {
      GUIData( AstroStretchStudioInterface& );

      VerticalSizer     Global_Sizer;
      WebView           WebView_Control;
      HorizontalSizer   Buttons_Sizer;
      PushButton        Apply_Button;
      PushButton        Reset_Button;
   };

   GUIData* GUI = nullptr;

   // WebView communication
   void InitializeWebView();
   void SendParametersToWebView();
   void SendImageToWebView( const View& view );
   void OnWebViewMessage( WebView& sender, const String& message );

   // Button handlers
   void e_Click( Button& sender, bool checked );

   // Timer for debounced updates
   Timer m_updateTimer;
   void e_Timer( Timer& sender );

   // Current view tracking
   View m_currentView;

   friend struct GUIData;
};

// ----------------------------------------------------------------------------

PCL_BEGIN_LOCAL
extern AstroStretchStudioInterface* TheAstroStretchStudioInterface;
PCL_END_LOCAL

// ----------------------------------------------------------------------------

} // namespace pcl

#endif // __AstroStretchStudioInterface_h

// ----------------------------------------------------------------------------
