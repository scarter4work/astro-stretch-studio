// ----------------------------------------------------------------------------
// AstroStretchStudio Parameters Header
// ----------------------------------------------------------------------------

#ifndef __AstroStretchStudioParameters_h
#define __AstroStretchStudioParameters_h

#include <pcl/MetaParameter.h>

namespace pcl
{

// ----------------------------------------------------------------------------

PCL_BEGIN_LOCAL

// ----------------------------------------------------------------------------
// Algorithm Selection
// ----------------------------------------------------------------------------

class ASSAlgorithm : public MetaEnumeration
{
public:
   enum { OTS,    // Optimal Transport Stretch
          SAS,    // Starlet Arctan Stretch
          NumberOfItems,
          Default = OTS };

   ASSAlgorithm( MetaProcess* );

   IsoString Id() const override;
   size_type NumberOfElements() const override;
   IsoString ElementId( size_type ) const override;
   int ElementValue( size_type ) const override;
   size_type DefaultValueIndex() const override;
};

extern ASSAlgorithm* TheASSAlgorithmParameter;

// ----------------------------------------------------------------------------
// OTS Parameters
// ----------------------------------------------------------------------------

class ASSOTSObjectType : public MetaEnumeration
{
public:
   enum { Nebula,
          Galaxy,
          StarCluster,
          DarkNebula,
          Custom,
          NumberOfItems,
          Default = Nebula };

   ASSOTSObjectType( MetaProcess* );

   IsoString Id() const override;
   size_type NumberOfElements() const override;
   IsoString ElementId( size_type ) const override;
   int ElementValue( size_type ) const override;
   size_type DefaultValueIndex() const override;
};

extern ASSOTSObjectType* TheASSOTSObjectTypeParameter;

// ----------------------------------------------------------------------------

class ASSOTSBackgroundTarget : public MetaFloat
{
public:
   ASSOTSBackgroundTarget( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSOTSBackgroundTarget* TheASSOTSBackgroundTargetParameter;

// ----------------------------------------------------------------------------

class ASSOTSStretchIntensity : public MetaFloat
{
public:
   ASSOTSStretchIntensity( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSOTSStretchIntensity* TheASSOTSStretchIntensityParameter;

// ----------------------------------------------------------------------------

class ASSOTSProtectHighlights : public MetaFloat
{
public:
   ASSOTSProtectHighlights( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSOTSProtectHighlights* TheASSOTSProtectHighlightsParameter;

// ----------------------------------------------------------------------------

class ASSOTSPreserveColor : public MetaBoolean
{
public:
   ASSOTSPreserveColor( MetaProcess* );

   IsoString Id() const override;
   bool DefaultValue() const override;
};

extern ASSOTSPreserveColor* TheASSOTSPreserveColorParameter;

// ----------------------------------------------------------------------------
// SAS Parameters
// ----------------------------------------------------------------------------

class ASSSASNumScales : public MetaInt32
{
public:
   ASSSASNumScales( MetaProcess* );

   IsoString Id() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASNumScales* TheASSSASNumScalesParameter;

// ----------------------------------------------------------------------------

class ASSSASBackgroundTarget : public MetaFloat
{
public:
   ASSSASBackgroundTarget( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASBackgroundTarget* TheASSSASBackgroundTargetParameter;

// ----------------------------------------------------------------------------

class ASSSASFineScaleGain : public MetaFloat
{
public:
   ASSSASFineScaleGain( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASFineScaleGain* TheASSSASFineScaleGainParameter;

// ----------------------------------------------------------------------------

class ASSSASMidScaleGain : public MetaFloat
{
public:
   ASSSASMidScaleGain( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASMidScaleGain* TheASSSASMidScaleGainParameter;

// ----------------------------------------------------------------------------

class ASSSASCoarseScaleGain : public MetaFloat
{
public:
   ASSSASCoarseScaleGain( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASCoarseScaleGain* TheASSSASCoarseScaleGainParameter;

// ----------------------------------------------------------------------------

class ASSSASCompressionAlpha : public MetaFloat
{
public:
   ASSSASCompressionAlpha( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASCompressionAlpha* TheASSSASCompressionAlphaParameter;

// ----------------------------------------------------------------------------

class ASSSASHighlightProtection : public MetaFloat
{
public:
   ASSSASHighlightProtection( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASHighlightProtection* TheASSSASHighlightProtectionParameter;

// ----------------------------------------------------------------------------

class ASSSASNoiseThreshold : public MetaFloat
{
public:
   ASSSASNoiseThreshold( MetaProcess* );

   IsoString Id() const override;
   int Precision() const override;
   double MinimumValue() const override;
   double MaximumValue() const override;
   double DefaultValue() const override;
};

extern ASSSASNoiseThreshold* TheASSSASNoiseThresholdParameter;

// ----------------------------------------------------------------------------

class ASSSASFlattenBackground : public MetaBoolean
{
public:
   ASSSASFlattenBackground( MetaProcess* );

   IsoString Id() const override;
   bool DefaultValue() const override;
};

extern ASSSASFlattenBackground* TheASSSASFlattenBackgroundParameter;

// ----------------------------------------------------------------------------

class ASSSASPreserveColor : public MetaBoolean
{
public:
   ASSSASPreserveColor( MetaProcess* );

   IsoString Id() const override;
   bool DefaultValue() const override;
};

extern ASSSASPreserveColor* TheASSSASPreserveColorParameter;

// ----------------------------------------------------------------------------

PCL_END_LOCAL

// ----------------------------------------------------------------------------

} // namespace pcl

#endif // __AstroStretchStudioParameters_h

// ----------------------------------------------------------------------------
