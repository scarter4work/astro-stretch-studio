// ----------------------------------------------------------------------------
// AstroStretchStudio Parameters Implementation
// ----------------------------------------------------------------------------

#include "AstroStretchStudioParameters.h"

namespace pcl
{

// ----------------------------------------------------------------------------

ASSAlgorithm*              TheASSAlgorithmParameter = nullptr;
ASSOTSObjectType*          TheASSOTSObjectTypeParameter = nullptr;
ASSOTSBackgroundTarget*    TheASSOTSBackgroundTargetParameter = nullptr;
ASSOTSStretchIntensity*    TheASSOTSStretchIntensityParameter = nullptr;
ASSOTSProtectHighlights*   TheASSOTSProtectHighlightsParameter = nullptr;
ASSOTSPreserveColor*       TheASSOTSPreserveColorParameter = nullptr;
ASSSASNumScales*           TheASSSASNumScalesParameter = nullptr;
ASSSASBackgroundTarget*    TheASSSASBackgroundTargetParameter = nullptr;
ASSSASFineScaleGain*       TheASSSASFineScaleGainParameter = nullptr;
ASSSASMidScaleGain*        TheASSSASMidScaleGainParameter = nullptr;
ASSSASCoarseScaleGain*     TheASSSASCoarseScaleGainParameter = nullptr;
ASSSASCompressionAlpha*    TheASSSASCompressionAlphaParameter = nullptr;
ASSSASHighlightProtection* TheASSSASHighlightProtectionParameter = nullptr;
ASSSASNoiseThreshold*      TheASSSASNoiseThresholdParameter = nullptr;
ASSSASFlattenBackground*   TheASSSASFlattenBackgroundParameter = nullptr;
ASSSASPreserveColor*       TheASSSASPreserveColorParameter = nullptr;

// ----------------------------------------------------------------------------
// Algorithm Selection
// ----------------------------------------------------------------------------

ASSAlgorithm::ASSAlgorithm( MetaProcess* P ) : MetaEnumeration( P )
{
   TheASSAlgorithmParameter = this;
}

IsoString ASSAlgorithm::Id() const
{
   return "algorithm";
}

size_type ASSAlgorithm::NumberOfElements() const
{
   return NumberOfItems;
}

IsoString ASSAlgorithm::ElementId( size_type i ) const
{
   switch ( i )
   {
   case OTS: return "OTS";
   case SAS: return "SAS";
   default:  return IsoString();
   }
}

int ASSAlgorithm::ElementValue( size_type i ) const
{
   return int( i );
}

size_type ASSAlgorithm::DefaultValueIndex() const
{
   return size_type( Default );
}

// ----------------------------------------------------------------------------
// OTS Parameters
// ----------------------------------------------------------------------------

ASSOTSObjectType::ASSOTSObjectType( MetaProcess* P ) : MetaEnumeration( P )
{
   TheASSOTSObjectTypeParameter = this;
}

IsoString ASSOTSObjectType::Id() const
{
   return "otsObjectType";
}

size_type ASSOTSObjectType::NumberOfElements() const
{
   return NumberOfItems;
}

IsoString ASSOTSObjectType::ElementId( size_type i ) const
{
   switch ( i )
   {
   case Nebula:      return "Nebula";
   case Galaxy:      return "Galaxy";
   case StarCluster: return "StarCluster";
   case DarkNebula:  return "DarkNebula";
   case Custom:      return "Custom";
   default:          return IsoString();
   }
}

int ASSOTSObjectType::ElementValue( size_type i ) const
{
   return int( i );
}

size_type ASSOTSObjectType::DefaultValueIndex() const
{
   return size_type( Default );
}

// ----------------------------------------------------------------------------

ASSOTSBackgroundTarget::ASSOTSBackgroundTarget( MetaProcess* P ) : MetaFloat( P )
{
   TheASSOTSBackgroundTargetParameter = this;
}

IsoString ASSOTSBackgroundTarget::Id() const
{
   return "otsBackgroundTarget";
}

int ASSOTSBackgroundTarget::Precision() const
{
   return 3;
}

double ASSOTSBackgroundTarget::MinimumValue() const
{
   return 0.05;
}

double ASSOTSBackgroundTarget::MaximumValue() const
{
   return 0.30;
}

double ASSOTSBackgroundTarget::DefaultValue() const
{
   return 0.15;
}

// ----------------------------------------------------------------------------

ASSOTSStretchIntensity::ASSOTSStretchIntensity( MetaProcess* P ) : MetaFloat( P )
{
   TheASSOTSStretchIntensityParameter = this;
}

IsoString ASSOTSStretchIntensity::Id() const
{
   return "otsStretchIntensity";
}

int ASSOTSStretchIntensity::Precision() const
{
   return 2;
}

double ASSOTSStretchIntensity::MinimumValue() const
{
   return 0.0;
}

double ASSOTSStretchIntensity::MaximumValue() const
{
   return 1.0;
}

double ASSOTSStretchIntensity::DefaultValue() const
{
   return 0.75;
}

// ----------------------------------------------------------------------------

ASSOTSProtectHighlights::ASSOTSProtectHighlights( MetaProcess* P ) : MetaFloat( P )
{
   TheASSOTSProtectHighlightsParameter = this;
}

IsoString ASSOTSProtectHighlights::Id() const
{
   return "otsProtectHighlights";
}

int ASSOTSProtectHighlights::Precision() const
{
   return 2;
}

double ASSOTSProtectHighlights::MinimumValue() const
{
   return 0.0;
}

double ASSOTSProtectHighlights::MaximumValue() const
{
   return 1.0;
}

double ASSOTSProtectHighlights::DefaultValue() const
{
   return 0.3;
}

// ----------------------------------------------------------------------------

ASSOTSPreserveColor::ASSOTSPreserveColor( MetaProcess* P ) : MetaBoolean( P )
{
   TheASSOTSPreserveColorParameter = this;
}

IsoString ASSOTSPreserveColor::Id() const
{
   return "otsPreserveColor";
}

bool ASSOTSPreserveColor::DefaultValue() const
{
   return true;
}

// ----------------------------------------------------------------------------
// SAS Parameters
// ----------------------------------------------------------------------------

ASSSASNumScales::ASSSASNumScales( MetaProcess* P ) : MetaInt32( P )
{
   TheASSSASNumScalesParameter = this;
}

IsoString ASSSASNumScales::Id() const
{
   return "sasNumScales";
}

double ASSSASNumScales::MinimumValue() const
{
   return 4;
}

double ASSSASNumScales::MaximumValue() const
{
   return 8;
}

double ASSSASNumScales::DefaultValue() const
{
   return 6;
}

// ----------------------------------------------------------------------------

ASSSASBackgroundTarget::ASSSASBackgroundTarget( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASBackgroundTargetParameter = this;
}

IsoString ASSSASBackgroundTarget::Id() const
{
   return "sasBackgroundTarget";
}

int ASSSASBackgroundTarget::Precision() const
{
   return 3;
}

double ASSSASBackgroundTarget::MinimumValue() const
{
   return 0.05;
}

double ASSSASBackgroundTarget::MaximumValue() const
{
   return 0.25;
}

double ASSSASBackgroundTarget::DefaultValue() const
{
   return 0.12;
}

// ----------------------------------------------------------------------------

ASSSASFineScaleGain::ASSSASFineScaleGain( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASFineScaleGainParameter = this;
}

IsoString ASSSASFineScaleGain::Id() const
{
   return "sasFineScaleGain";
}

int ASSSASFineScaleGain::Precision() const
{
   return 2;
}

double ASSSASFineScaleGain::MinimumValue() const
{
   return 0.5;
}

double ASSSASFineScaleGain::MaximumValue() const
{
   return 2.0;
}

double ASSSASFineScaleGain::DefaultValue() const
{
   return 0.8;
}

// ----------------------------------------------------------------------------

ASSSASMidScaleGain::ASSSASMidScaleGain( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASMidScaleGainParameter = this;
}

IsoString ASSSASMidScaleGain::Id() const
{
   return "sasMidScaleGain";
}

int ASSSASMidScaleGain::Precision() const
{
   return 2;
}

double ASSSASMidScaleGain::MinimumValue() const
{
   return 1.0;
}

double ASSSASMidScaleGain::MaximumValue() const
{
   return 5.0;
}

double ASSSASMidScaleGain::DefaultValue() const
{
   return 2.5;
}

// ----------------------------------------------------------------------------

ASSSASCoarseScaleGain::ASSSASCoarseScaleGain( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASCoarseScaleGainParameter = this;
}

IsoString ASSSASCoarseScaleGain::Id() const
{
   return "sasCoarseScaleGain";
}

int ASSSASCoarseScaleGain::Precision() const
{
   return 2;
}

double ASSSASCoarseScaleGain::MinimumValue() const
{
   return 1.0;
}

double ASSSASCoarseScaleGain::MaximumValue() const
{
   return 8.0;
}

double ASSSASCoarseScaleGain::DefaultValue() const
{
   return 4.0;
}

// ----------------------------------------------------------------------------

ASSSASCompressionAlpha::ASSSASCompressionAlpha( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASCompressionAlphaParameter = this;
}

IsoString ASSSASCompressionAlpha::Id() const
{
   return "sasCompressionAlpha";
}

int ASSSASCompressionAlpha::Precision() const
{
   return 1;
}

double ASSSASCompressionAlpha::MinimumValue() const
{
   return 1.0;
}

double ASSSASCompressionAlpha::MaximumValue() const
{
   return 20.0;
}

double ASSSASCompressionAlpha::DefaultValue() const
{
   return 8.0;
}

// ----------------------------------------------------------------------------

ASSSASHighlightProtection::ASSSASHighlightProtection( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASHighlightProtectionParameter = this;
}

IsoString ASSSASHighlightProtection::Id() const
{
   return "sasHighlightProtection";
}

int ASSSASHighlightProtection::Precision() const
{
   return 2;
}

double ASSSASHighlightProtection::MinimumValue() const
{
   return 0.0;
}

double ASSSASHighlightProtection::MaximumValue() const
{
   return 1.0;
}

double ASSSASHighlightProtection::DefaultValue() const
{
   return 0.5;
}

// ----------------------------------------------------------------------------

ASSSASNoiseThreshold::ASSSASNoiseThreshold( MetaProcess* P ) : MetaFloat( P )
{
   TheASSSASNoiseThresholdParameter = this;
}

IsoString ASSSASNoiseThreshold::Id() const
{
   return "sasNoiseThreshold";
}

int ASSSASNoiseThreshold::Precision() const
{
   return 4;
}

double ASSSASNoiseThreshold::MinimumValue() const
{
   return 0.0;
}

double ASSSASNoiseThreshold::MaximumValue() const
{
   return 0.01;
}

double ASSSASNoiseThreshold::DefaultValue() const
{
   return 0.001;
}

// ----------------------------------------------------------------------------

ASSSASFlattenBackground::ASSSASFlattenBackground( MetaProcess* P ) : MetaBoolean( P )
{
   TheASSSASFlattenBackgroundParameter = this;
}

IsoString ASSSASFlattenBackground::Id() const
{
   return "sasFlattenBackground";
}

bool ASSSASFlattenBackground::DefaultValue() const
{
   return true;
}

// ----------------------------------------------------------------------------

ASSSASPreserveColor::ASSSASPreserveColor( MetaProcess* P ) : MetaBoolean( P )
{
   TheASSSASPreserveColorParameter = this;
}

IsoString ASSSASPreserveColor::Id() const
{
   return "sasPreserveColor";
}

bool ASSSASPreserveColor::DefaultValue() const
{
   return true;
}

// ----------------------------------------------------------------------------

} // namespace pcl

// ----------------------------------------------------------------------------
