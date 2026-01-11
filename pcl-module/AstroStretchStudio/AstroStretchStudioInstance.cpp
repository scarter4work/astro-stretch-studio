// ----------------------------------------------------------------------------
// AstroStretchStudio Instance Implementation
// ----------------------------------------------------------------------------

#include "AstroStretchStudioInstance.h"
#include "AstroStretchStudioProcess.h"
#include "AstroStretchStudioParameters.h"

#include <pcl/AutoViewLock.h>
#include <pcl/Console.h>
#include <pcl/StdStatus.h>
#include <pcl/View.h>
#include <pcl/MuteStatus.h>
#include <pcl/SeparableConvolution.h>
#include <pcl/GaussianFilter.h>
#include <pcl/Histogram.h>

namespace pcl
{

// ----------------------------------------------------------------------------

AstroStretchStudioInstance::AstroStretchStudioInstance( const MetaProcess* m )
   : ProcessImplementation( m )
{
   SetDefaultParameters();
}

// ----------------------------------------------------------------------------

AstroStretchStudioInstance::AstroStretchStudioInstance( const AstroStretchStudioInstance& x )
   : ProcessImplementation( x )
{
   Assign( x );
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::SetDefaultParameters()
{
   p_algorithm = ASSAlgorithm::Default;

   // OTS defaults
   p_otsObjectType = ASSOTSObjectType::Default;
   p_otsBackgroundTarget = TheASSOTSBackgroundTargetParameter->DefaultValue();
   p_otsStretchIntensity = TheASSOTSStretchIntensityParameter->DefaultValue();
   p_otsProtectHighlights = TheASSOTSProtectHighlightsParameter->DefaultValue();
   p_otsPreserveColor = TheASSOTSPreserveColorParameter->DefaultValue();

   // SAS defaults
   p_sasNumScales = int32( TheASSSASNumScalesParameter->DefaultValue() );
   p_sasBackgroundTarget = TheASSSASBackgroundTargetParameter->DefaultValue();
   p_sasFineScaleGain = TheASSSASFineScaleGainParameter->DefaultValue();
   p_sasMidScaleGain = TheASSSASMidScaleGainParameter->DefaultValue();
   p_sasCoarseScaleGain = TheASSSASCoarseScaleGainParameter->DefaultValue();
   p_sasCompressionAlpha = TheASSSASCompressionAlphaParameter->DefaultValue();
   p_sasHighlightProtection = TheASSSASHighlightProtectionParameter->DefaultValue();
   p_sasNoiseThreshold = TheASSSASNoiseThresholdParameter->DefaultValue();
   p_sasFlattenBackground = TheASSSASFlattenBackgroundParameter->DefaultValue();
   p_sasPreserveColor = TheASSSASPreserveColorParameter->DefaultValue();
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::Assign( const ProcessImplementation& p )
{
   const AstroStretchStudioInstance* x = dynamic_cast<const AstroStretchStudioInstance*>( &p );
   if ( x != nullptr )
   {
      p_algorithm = x->p_algorithm;

      p_otsObjectType = x->p_otsObjectType;
      p_otsBackgroundTarget = x->p_otsBackgroundTarget;
      p_otsStretchIntensity = x->p_otsStretchIntensity;
      p_otsProtectHighlights = x->p_otsProtectHighlights;
      p_otsPreserveColor = x->p_otsPreserveColor;

      p_sasNumScales = x->p_sasNumScales;
      p_sasBackgroundTarget = x->p_sasBackgroundTarget;
      p_sasFineScaleGain = x->p_sasFineScaleGain;
      p_sasMidScaleGain = x->p_sasMidScaleGain;
      p_sasCoarseScaleGain = x->p_sasCoarseScaleGain;
      p_sasCompressionAlpha = x->p_sasCompressionAlpha;
      p_sasHighlightProtection = x->p_sasHighlightProtection;
      p_sasNoiseThreshold = x->p_sasNoiseThreshold;
      p_sasFlattenBackground = x->p_sasFlattenBackground;
      p_sasPreserveColor = x->p_sasPreserveColor;
   }
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInstance::IsHistoryUpdater( const View& ) const
{
   return true;
}

// ----------------------------------------------------------------------------

UndoFlags AstroStretchStudioInstance::UndoMode( const View& ) const
{
   return UndoFlag::PixelData;
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInstance::CanExecuteOn( const View& view, String& whyNot ) const
{
   if ( view.Image().IsComplexSample() )
   {
      whyNot = "AstroStretchStudio cannot be executed on complex images.";
      return false;
   }
   return true;
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInstance::ExecuteOn( View& view )
{
   AutoViewLock lock( view );

   ImageVariant image = view.Image();

   StandardStatus status;
   image.SetStatusCallback( &status );

   Console console;
   console.EnableAbort();

   if ( p_algorithm == ASSAlgorithm::OTS )
   {
      console.WriteLn( "<end><cbr>Applying Optimal Transport Stretch..." );
      image.SetStatusCallback( &status );

      if ( image.IsFloatSample() )
         switch ( image.BitsPerSample() )
         {
         case 32: ApplyOTS( static_cast<Image&>( *image ) ); break;
         case 64: ApplyOTS( static_cast<DImage&>( *image ) ); break;
         }
      else
         switch ( image.BitsPerSample() )
         {
         case  8: ApplyOTS( static_cast<UInt8Image&>( *image ) ); break;
         case 16: ApplyOTS( static_cast<UInt16Image&>( *image ) ); break;
         case 32: ApplyOTS( static_cast<UInt32Image&>( *image ) ); break;
         }
   }
   else // SAS
   {
      console.WriteLn( "<end><cbr>Applying Starlet Arctan Stretch..." );

      if ( image.IsFloatSample() )
         switch ( image.BitsPerSample() )
         {
         case 32: ApplySAS( static_cast<Image&>( *image ) ); break;
         case 64: ApplySAS( static_cast<DImage&>( *image ) ); break;
         }
      else
         switch ( image.BitsPerSample() )
         {
         case  8: ApplySAS( static_cast<UInt8Image&>( *image ) ); break;
         case 16: ApplySAS( static_cast<UInt16Image&>( *image ) ); break;
         case 32: ApplySAS( static_cast<UInt32Image&>( *image ) ); break;
         }
   }

   return true;
}

// ----------------------------------------------------------------------------

void* AstroStretchStudioInstance::LockParameter( const MetaParameter* p, size_type /*tableRow*/ )
{
   if ( p == TheASSAlgorithmParameter )            return &p_algorithm;
   if ( p == TheASSOTSObjectTypeParameter )        return &p_otsObjectType;
   if ( p == TheASSOTSBackgroundTargetParameter )  return &p_otsBackgroundTarget;
   if ( p == TheASSOTSStretchIntensityParameter )  return &p_otsStretchIntensity;
   if ( p == TheASSOTSProtectHighlightsParameter ) return &p_otsProtectHighlights;
   if ( p == TheASSOTSPreserveColorParameter )     return &p_otsPreserveColor;
   if ( p == TheASSSASNumScalesParameter )         return &p_sasNumScales;
   if ( p == TheASSSASBackgroundTargetParameter )  return &p_sasBackgroundTarget;
   if ( p == TheASSSASFineScaleGainParameter )     return &p_sasFineScaleGain;
   if ( p == TheASSSASMidScaleGainParameter )      return &p_sasMidScaleGain;
   if ( p == TheASSSASCoarseScaleGainParameter )   return &p_sasCoarseScaleGain;
   if ( p == TheASSSASCompressionAlphaParameter )  return &p_sasCompressionAlpha;
   if ( p == TheASSSASHighlightProtectionParameter ) return &p_sasHighlightProtection;
   if ( p == TheASSSASNoiseThresholdParameter )    return &p_sasNoiseThreshold;
   if ( p == TheASSSASFlattenBackgroundParameter ) return &p_sasFlattenBackground;
   if ( p == TheASSSASPreserveColorParameter )     return &p_sasPreserveColor;
   return nullptr;
}

// ----------------------------------------------------------------------------

bool AstroStretchStudioInstance::AllocateParameter( size_type sizeOrLength,
                                                     const MetaParameter* p,
                                                     size_type tableRow )
{
   // No variable-length parameters
   return false;
}

// ----------------------------------------------------------------------------

size_type AstroStretchStudioInstance::ParameterLength( const MetaParameter* p,
                                                        size_type tableRow ) const
{
   return 0;
}

// ----------------------------------------------------------------------------
// OTS Implementation
// ----------------------------------------------------------------------------

template <class P>
void AstroStretchStudioInstance::ApplyOTS( GenericImage<P>& image ) const
{
   const int resolution = 65536;

   // Extract or compute luminance
   Image L;
   bool isColor = image.NumberOfChannels() >= 3;

   if ( isColor && p_otsPreserveColor )
   {
      // Extract CIE luminance
      L.AllocateData( image.Width(), image.Height() );
      for ( int y = 0; y < image.Height(); ++y )
         for ( int x = 0; x < image.Width(); ++x )
         {
            double r = image( x, y, 0 );
            double g = image( x, y, 1 );
            double b = image( x, y, 2 );
            L( x, y ) = 0.2126 * r + 0.7152 * g + 0.0722 * b;
         }
   }
   else
   {
      // Use first channel or grayscale
      L.AllocateData( image.Width(), image.Height() );
      for ( int y = 0; y < image.Height(); ++y )
         for ( int x = 0; x < image.Width(); ++x )
            L( x, y ) = image( x, y, 0 );
   }

   // Store original luminance for color reconstruction
   Image L_orig( L );

   // Compute source histogram and CDF
   FVector srcCDF( resolution );
   ComputeHistogramCDF( L, srcCDF );

   // Generate target CDF based on object type
   FVector tgtCDF( resolution );
   GenerateTargetCDF( tgtCDF, p_otsObjectType, p_otsBackgroundTarget );

   // Compute optimal transport map
   FVector transportMap( resolution );
   ComputeTransportMap( transportMap, srcCDF, tgtCDF );

   // Apply highlight protection
   if ( p_otsProtectHighlights > 0 )
   {
      for ( int i = 0; i < resolution; ++i )
      {
         double x = double( i ) / ( resolution - 1 );
         double t = ( x - 0.7 ) / 0.25;
         t = Max( 0.0, Min( 1.0, t ) );
         double blend = t * t * ( 3 - 2 * t ) * p_otsProtectHighlights;
         transportMap[i] = ( 1 - blend ) * transportMap[i] + blend * x;
      }
   }

   // Apply stretch intensity blend
   for ( int i = 0; i < resolution; ++i )
   {
      double identity = double( i ) / ( resolution - 1 );
      transportMap[i] = ( 1 - p_otsStretchIntensity ) * identity +
                         p_otsStretchIntensity * transportMap[i];
   }

   // Apply transport map to luminance
   for ( int y = 0; y < L.Height(); ++y )
      for ( int x = 0; x < L.Width(); ++x )
      {
         int bin = Min( resolution - 1, Max( 0, RoundInt( L( x, y ) * ( resolution - 1 ) ) ) );
         L( x, y ) = transportMap[bin];
      }

   // Reconstruct color or apply to image
   if ( isColor && p_otsPreserveColor )
   {
      for ( int y = 0; y < image.Height(); ++y )
         for ( int x = 0; x < image.Width(); ++x )
         {
            double origLum = L_orig( x, y );
            double newLum = L( x, y );
            if ( origLum > 1e-10 )
            {
               double scale = newLum / origLum;
               for ( int c = 0; c < image.NumberOfChannels(); ++c )
                  image( x, y, c ) = Range( image( x, y, c ) * scale, 0.0, 1.0 );
            }
         }
   }
   else
   {
      for ( int c = 0; c < image.NumberOfChannels(); ++c )
         for ( int y = 0; y < image.Height(); ++y )
            for ( int x = 0; x < image.Width(); ++x )
            {
               int bin = Min( resolution - 1, Max( 0, RoundInt( image( x, y, c ) * ( resolution - 1 ) ) ) );
               image( x, y, c ) = transportMap[bin];
            }
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::GenerateTargetCDF( FVector& cdf, int objectType, double bgTarget ) const
{
   const int n = cdf.Length();
   FVector pdf( n );

   for ( int i = 0; i < n; ++i )
   {
      double x = double( i ) / ( n - 1 );

      switch ( objectType )
      {
      case ASSOTSObjectType::Nebula:
         // Background peak
         pdf[i] = 0.3 * Exp( -0.5 * Pow( ( x - bgTarget ) / 0.03, 2 ) );
         // Nebula body
         if ( x >= bgTarget && x <= 0.7 )
            pdf[i] += 0.5 * Pow( x - bgTarget, 1.0 ) * Pow( 0.7 - x, 2.0 );
         // Highlights
         if ( x >= 0.6 && x <= 0.95 )
            pdf[i] += 0.2 * Pow( x - 0.6, 0.5 ) * Pow( 0.95 - x, 3.0 );
         break;

      case ASSOTSObjectType::Galaxy:
         pdf[i] = 0.25 * Exp( -0.5 * Pow( ( x - bgTarget ) / 0.025, 2 ) );
         if ( x >= bgTarget && x <= 0.5 )
            pdf[i] += 0.35 * Pow( x - bgTarget, 1.5 ) * Pow( 0.5 - x, 1.5 );
         if ( x >= 0.4 && x <= 0.75 )
            pdf[i] += 0.25 * Pow( x - 0.4, 2.0 ) * Pow( 0.75 - x, 1.0 );
         if ( x >= 0.7 && x <= 0.9 )
            pdf[i] += 0.15;
         break;

      case ASSOTSObjectType::StarCluster:
         pdf[i] = 0.20 * Exp( -0.5 * Pow( ( x - bgTarget * 0.8 ) / 0.02, 2 ) );
         if ( x >= 0.15 && x <= 0.70 )
            pdf[i] += 0.50 * Pow( x - 0.15, 0.5 ) * Pow( 0.70 - x, 1.0 );
         if ( x >= 0.60 && x <= 0.95 )
            pdf[i] += 0.30 * Pow( x - 0.60, 1.0 ) * Pow( 0.95 - x, 4.0 );
         break;

      case ASSOTSObjectType::DarkNebula:
         pdf[i] = 0.15 * Exp( -0.5 * Pow( ( x - bgTarget * 1.3 ) / 0.04, 2 ) );
         if ( x >= 0.05 && x <= bgTarget )
            pdf[i] += 0.40 * Pow( x - 0.05, 2.0 ) * Pow( bgTarget - x, 1.0 );
         if ( x >= bgTarget && x <= 0.55 )
            pdf[i] += 0.30 * Pow( x - bgTarget, 1.0 ) * Pow( 0.55 - x, 1.5 );
         if ( x >= 0.5 && x <= 0.85 )
            pdf[i] += 0.15;
         break;

      default:
         pdf[i] = 1.0;
      }
   }

   // Normalize PDF and compute CDF
   double sum = 0;
   for ( int i = 0; i < n; ++i )
      sum += pdf[i];
   if ( sum > 0 )
      for ( int i = 0; i < n; ++i )
         pdf[i] /= sum;

   cdf[0] = pdf[0];
   for ( int i = 1; i < n; ++i )
      cdf[i] = cdf[i-1] + pdf[i];

   // Ensure ends are 0 and 1
   if ( cdf[n-1] > 0 )
      for ( int i = 0; i < n; ++i )
         cdf[i] /= cdf[n-1];
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::ComputeHistogramCDF( const Image& image, FVector& cdf ) const
{
   const int n = cdf.Length();
   FVector hist( n );
   hist.Fill( 0 );

   for ( Image::const_sample_iterator i( image ); i; ++i )
   {
      int bin = Min( n - 1, Max( 0, RoundInt( *i * ( n - 1 ) ) ) );
      hist[bin]++;
   }

   double sum = 0;
   for ( int i = 0; i < n; ++i )
      sum += hist[i];

   cdf[0] = hist[0] / sum;
   for ( int i = 1; i < n; ++i )
      cdf[i] = cdf[i-1] + hist[i] / sum;
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::ComputeTransportMap( FVector& tmap,
                                                       const FVector& srcCDF,
                                                       const FVector& tgtCDF ) const
{
   const int n = tmap.Length();

   for ( int i = 0; i < n; ++i )
   {
      double quantile = srcCDF[i];

      // Binary search for inverse CDF
      int lo = 0, hi = n - 1;
      while ( lo < hi )
      {
         int mid = ( lo + hi ) / 2;
         if ( tgtCDF[mid] < quantile )
            lo = mid + 1;
         else
            hi = mid;
      }

      tmap[i] = double( lo ) / ( n - 1 );
   }
}

// ----------------------------------------------------------------------------
// SAS Implementation
// ----------------------------------------------------------------------------

template <class P>
void AstroStretchStudioInstance::ApplySAS( GenericImage<P>& image ) const
{
   bool isColor = image.NumberOfChannels() >= 3;

   // Extract luminance
   Image L( image.Width(), image.Height() );

   if ( isColor && p_sasPreserveColor )
   {
      for ( int y = 0; y < image.Height(); ++y )
         for ( int x = 0; x < image.Width(); ++x )
         {
            double r = image( x, y, 0 );
            double g = image( x, y, 1 );
            double b = image( x, y, 2 );
            L( x, y ) = 0.2126 * r + 0.7152 * g + 0.0722 * b;
         }
   }
   else
   {
      for ( int y = 0; y < image.Height(); ++y )
         for ( int x = 0; x < image.Width(); ++x )
            L( x, y ) = image( x, y, 0 );
   }

   Image L_orig( L );

   // Starlet decomposition
   Array<Image> scales;
   StarletDecompose( L, scales, p_sasNumScales );

   // Estimate noise from finest scale
   double sigma_noise = EstimateNoise( scales[0] );

   // Process each scale
   for ( int j = 0; j < p_sasNumScales; ++j )
   {
      double gain = ComputeScaleGain( j );

      // Noise thresholding for fine scales
      if ( j <= 1 )
      {
         double threshold = p_sasNoiseThreshold * sigma_noise * 5;
         for ( Image::sample_iterator i( scales[j] ); i; ++i )
         {
            double w = *i;
            if ( Abs( w ) <= threshold )
               *i = 0;
            else
               *i = ( w > 0 ) ? ( w - threshold ) : ( w + threshold );
         }
      }

      // Apply gain with highlight protection
      if ( p_sasHighlightProtection > 0 )
      {
         // Compute smoothed luminance for modulation
         double sigma = Pow2( j + 1 );
         Image Lsmooth( L_orig );
         GaussianFilter G( Min( sigma, 16.0 ) );
         G >> Lsmooth;

         for ( int y = 0; y < scales[j].Height(); ++y )
            for ( int x = 0; x < scales[j].Width(); ++x )
            {
               double intensity = Lsmooth( x, y );
               double sigmoid = 1.0 / ( 1.0 + Exp( -8.0 * ( intensity - 0.5 ) ) );
               double mod = Max( 1.0 - p_sasHighlightProtection * sigmoid, 0.2 );
               scales[j]( x, y ) *= gain * mod;
            }
      }
      else
      {
         scales[j] *= gain;
      }
   }

   // Process coarsest scale
   if ( p_sasFlattenBackground )
   {
      double coarseTarget = p_sasBackgroundTarget * 0.5;
      for ( Image::sample_iterator i( scales[p_sasNumScales] ); i; ++i )
         *i = 0.2 * *i + 0.8 * coarseTarget;
   }

   // Reconstruct
   StarletReconstruct( L, scales );

   // Arctangent compression
   const double twoOverPi = 2.0 / Pi();
   for ( Image::sample_iterator i( L ); i; ++i )
   {
      double x = *i;
      if ( x > p_sasBackgroundTarget )
      {
         double normalized = ( x - p_sasBackgroundTarget ) / ( 1.0 - p_sasBackgroundTarget );
         double compressed = twoOverPi * ArcTan( p_sasCompressionAlpha * normalized );
         *i = p_sasBackgroundTarget + compressed * ( 1.0 - p_sasBackgroundTarget );
      }
   }

   // Normalize background
   Array<double> samples;
   for ( Image::const_sample_iterator i( L ); i; ++i )
      samples.Add( *i );
   Sort( samples.Begin(), samples.End() );
   double currentBg = samples[samples.Length() / 20];

   if ( currentBg > 0 && currentBg != p_sasBackgroundTarget )
   {
      double scale = p_sasBackgroundTarget / currentBg;
      for ( Image::sample_iterator i( L ); i; ++i )
      {
         double v = *i;
         if ( v <= currentBg )
            *i = v * scale;
         else
            *i = p_sasBackgroundTarget + ( v - currentBg ) / ( 1.0 - currentBg ) * ( 1.0 - p_sasBackgroundTarget );
      }
   }

   L.Truncate( 0, 1 );

   // Reconstruct color
   if ( isColor && p_sasPreserveColor )
   {
      for ( int y = 0; y < image.Height(); ++y )
         for ( int x = 0; x < image.Width(); ++x )
         {
            double origLum = L_orig( x, y );
            double newLum = L( x, y );
            if ( origLum > 1e-10 )
            {
               double s = newLum / origLum;
               for ( int c = 0; c < image.NumberOfChannels(); ++c )
                  image( x, y, c ) = Range( image( x, y, c ) * s, 0.0, 1.0 );
            }
         }
   }
   else
   {
      for ( int c = 0; c < image.NumberOfChannels(); ++c )
         for ( int y = 0; y < image.Height(); ++y )
            for ( int x = 0; x < image.Width(); ++x )
               image( x, y, c ) = L( x, y );
   }
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::StarletDecompose( const Image& image,
                                                    Array<Image>& scales,
                                                    int numScales ) const
{
   scales.Clear();
   Image current( image );

   // B3-spline kernel [1,4,6,4,1]/16
   static const float b3[] = { 1.0f/16, 4.0f/16, 6.0f/16, 4.0f/16, 1.0f/16 };

   for ( int j = 0; j < numScales; ++j )
   {
      Image smooth( current.Width(), current.Height() );
      int spacing = 1 << j;

      // Separable convolution with spacing (à trous)
      Image temp( current.Width(), current.Height() );

      // Horizontal
      for ( int y = 0; y < current.Height(); ++y )
         for ( int x = 0; x < current.Width(); ++x )
         {
            double sum = 0;
            for ( int k = -2; k <= 2; ++k )
            {
               int xx = x + k * spacing;
               xx = Max( 0, Min( current.Width() - 1, xx ) );
               sum += b3[k+2] * current( xx, y );
            }
            temp( x, y ) = sum;
         }

      // Vertical
      for ( int y = 0; y < current.Height(); ++y )
         for ( int x = 0; x < current.Width(); ++x )
         {
            double sum = 0;
            for ( int k = -2; k <= 2; ++k )
            {
               int yy = y + k * spacing;
               yy = Max( 0, Min( current.Height() - 1, yy ) );
               sum += b3[k+2] * temp( x, yy );
            }
            smooth( x, y ) = sum;
         }

      // Wavelet = difference
      Image wavelet( current.Width(), current.Height() );
      for ( int y = 0; y < current.Height(); ++y )
         for ( int x = 0; x < current.Width(); ++x )
            wavelet( x, y ) = current( x, y ) - smooth( x, y );

      scales.Add( wavelet );
      current = smooth;
   }

   scales.Add( current ); // Residual
}

// ----------------------------------------------------------------------------

void AstroStretchStudioInstance::StarletReconstruct( Image& output,
                                                      const Array<Image>& scales ) const
{
   output.Zero();
   for ( const Image& scale : scales )
      output += scale;
}

// ----------------------------------------------------------------------------

double AstroStretchStudioInstance::EstimateNoise( const Image& fineScale ) const
{
   Array<double> absValues;
   for ( Image::const_sample_iterator i( fineScale ); i; ++i )
      absValues.Add( Abs( *i ) );

   Sort( absValues.Begin(), absValues.End() );
   double median = absValues[absValues.Length() / 2];

   Array<double> absDeviations;
   for ( double v : absValues )
      absDeviations.Add( Abs( v - median ) );

   Sort( absDeviations.Begin(), absDeviations.End() );
   double mad = absDeviations[absDeviations.Length() / 2];

   return mad * 1.4826;
}

// ----------------------------------------------------------------------------

double AstroStretchStudioInstance::ComputeScaleGain( int j ) const
{
   if ( j <= 1 )
      return p_sasFineScaleGain;
   else if ( j <= 3 )
   {
      double t = ( j - 1.5 ) / 2.0;
      return ( 1 - t ) * p_sasFineScaleGain + t * p_sasMidScaleGain;
   }
   else if ( j <= 5 )
   {
      double t = ( j - 3.5 ) / 2.0;
      return ( 1 - t ) * p_sasMidScaleGain + t * p_sasCoarseScaleGain;
   }
   return p_sasCoarseScaleGain;
}

// ----------------------------------------------------------------------------

} // namespace pcl

// ----------------------------------------------------------------------------
