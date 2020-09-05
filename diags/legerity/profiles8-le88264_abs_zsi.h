/*
 * profiles8-le88264_abs_zsi.h --
 *
 * This header file exports the Profile data types
 *
 * Project Info --
 *   File:
 *   Type:   Line Module LE71HR1865 RevA PBX VBL Reg
 *   Date:   Monday, March 07, 2011 08:40:06
 *   Device: VoicePort Le88266
 *
 *   This file was generated with Profile Wizard Version: P2.0.0
 */

#ifndef PROFILES8_LE88264_ABS_ZSI_H
#define PROFILES8_LE88264_ABS_ZSI_H

#ifdef VP_API_TYPES_H
#include "api-II/arch/uvb/vp_api_types.h"
#else
typedef unsigned char VpProfileDataType;
#endif


/************** Device Parameters **************/
extern const VpProfileDataType DEV_PROFILE_LE88264_ABS_ZSI[];   /* Le88264 Power Optimized With Free Run */

/************** AC Filter Coefficients **************/
extern const VpProfileDataType AC_PROFILE_LE88264_ABS_ZSI[];    /* AC FXS RF14 600 Coefficients */

/************** DC Feed Parameters **************/
extern const VpProfileDataType DC_PROFILE_LE88264_ABS_ZSI[];    /* 25mA  current feed */

/************** Ring Signal Parameters **************/
extern const VpProfileDataType RING_PROFILE_LE88264_ABS_ZSI[];     /* 20Hz, Sinewave ringing, 70Vpk */

/************** Cadence_Definitions **************/
extern const VpProfileDataType RING_CAD_LE88264_ABS_ZSI[];/* Ringing Cadence with CallerID */

#endif /* PROFILES8_LE88264_ABS_ZSI_H */

