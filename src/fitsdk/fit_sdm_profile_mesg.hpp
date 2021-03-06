////////////////////////////////////////////////////////////////////////////////
// The following FIT Protocol software provided may be used with FIT protocol
// devices only and remains the copyrighted property of Dynastream Innovations Inc.
// The software is being provided on an "as-is" basis and as an accommodation,
// and therefore all warranties, representations, or guarantees of any kind
// (whether express, implied or statutory) including, without limitation,
// warranties of merchantability, non-infringement, or fitness for a particular
// purpose, are specifically disclaimed.
//
// Copyright 2014 Dynastream Innovations Inc.
////////////////////////////////////////////////////////////////////////////////
// ****WARNING****  This file is auto-generated!  Do NOT edit this file.
// Profile Version = 12.10Release
// Tag = $Name$
////////////////////////////////////////////////////////////////////////////////


#if !defined(FIT_SDM_PROFILE_MESG_HPP)
#define FIT_SDM_PROFILE_MESG_HPP

#include "fit_mesg.hpp"

namespace fit
{

class SdmProfileMesg : public Mesg
{
   public:
      SdmProfileMesg(void) : Mesg(Profile::MESG_SDM_PROFILE)
      {
      }

      SdmProfileMesg(const Mesg &mesg) : Mesg(mesg)
      {
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns message_index field
      ///////////////////////////////////////////////////////////////////////
      FIT_MESSAGE_INDEX GetMessageIndex(void) const
      {
         return GetFieldUINT16Value(254, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set message_index field
      ///////////////////////////////////////////////////////////////////////
      void SetMessageIndex(FIT_MESSAGE_INDEX messageIndex)
      {
         SetFieldUINT16Value(254, messageIndex, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns enabled field
      ///////////////////////////////////////////////////////////////////////
      FIT_BOOL GetEnabled(void) const
      {
         return GetFieldENUMValue(0, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set enabled field
      ///////////////////////////////////////////////////////////////////////
      void SetEnabled(FIT_BOOL enabled)
      {
         SetFieldENUMValue(0, enabled, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns sdm_ant_id field
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT16Z GetSdmAntId(void) const
      {
         return GetFieldUINT16ZValue(1, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set sdm_ant_id field
      ///////////////////////////////////////////////////////////////////////
      void SetSdmAntId(FIT_UINT16Z sdmAntId)
      {
         SetFieldUINT16ZValue(1, sdmAntId, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns sdm_cal_factor field
      // Units: %
      ///////////////////////////////////////////////////////////////////////
      FIT_FLOAT32 GetSdmCalFactor(void) const
      {
         return GetFieldFLOAT32Value(2, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set sdm_cal_factor field
      // Units: %
      ///////////////////////////////////////////////////////////////////////
      void SetSdmCalFactor(FIT_FLOAT32 sdmCalFactor)
      {
         SetFieldFLOAT32Value(2, sdmCalFactor, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns odometer field
      // Units: m
      ///////////////////////////////////////////////////////////////////////
      FIT_FLOAT32 GetOdometer(void) const
      {
         return GetFieldFLOAT32Value(3, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set odometer field
      // Units: m
      ///////////////////////////////////////////////////////////////////////
      void SetOdometer(FIT_FLOAT32 odometer)
      {
         SetFieldFLOAT32Value(3, odometer, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns speed_source field
      // Comment: Use footpod for speed source instead of GPS
      ///////////////////////////////////////////////////////////////////////
      FIT_BOOL GetSpeedSource(void) const
      {
         return GetFieldENUMValue(4, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set speed_source field
      // Comment: Use footpod for speed source instead of GPS
      ///////////////////////////////////////////////////////////////////////
      void SetSpeedSource(FIT_BOOL speedSource)
      {
         SetFieldENUMValue(4, speedSource, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns sdm_ant_id_trans_type field
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT8Z GetSdmAntIdTransType(void) const
      {
         return GetFieldUINT8ZValue(5, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set sdm_ant_id_trans_type field
      ///////////////////////////////////////////////////////////////////////
      void SetSdmAntIdTransType(FIT_UINT8Z sdmAntIdTransType)
      {
         SetFieldUINT8ZValue(5, sdmAntIdTransType, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Returns odometer_rollover field
      // Comment: Rollover counter that can be used to extend the odometer
      ///////////////////////////////////////////////////////////////////////
      FIT_UINT8 GetOdometerRollover(void) const
      {
         return GetFieldUINT8Value(7, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

      ///////////////////////////////////////////////////////////////////////
      // Set odometer_rollover field
      // Comment: Rollover counter that can be used to extend the odometer
      ///////////////////////////////////////////////////////////////////////
      void SetOdometerRollover(FIT_UINT8 odometerRollover)
      {
         SetFieldUINT8Value(7, odometerRollover, 0, FIT_SUBFIELD_INDEX_MAIN_FIELD);
      }

};

} // namespace fit

#endif // !defined(FIT_SDM_PROFILE_MESG_HPP)
