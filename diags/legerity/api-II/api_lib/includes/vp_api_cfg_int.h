/** \file vp_api_cfg_int.h
 * vp_api_cfg_int.h
 *
 * This file contains internal configuration and compile time settings for
 * building appropriate VP-API library modules needed for any application.

 * Copyright (c) 2011, Microsemi
 *
 * $Revision: 1.1.1.1 $
 * $LastChangedDate: 2010-02-12 16:40:10 -0600 (Fri, 12 Feb 2010) $
 */

#ifndef VP_API_CFG_INT_H
#define VP_API_CFG_INT_H

/**< STRICT_SIGNED_ENUM_VALUES
 * Define this to avoid using enumerations with negative values (e.g., 0x8000). It should not be
 * needed in compilers compliant with ANSII C89 or later. Setting it to #define also removes strict
 * type checking in the API which is generally not preferred.
 */
#undef STRICT_SIGNED_ENUM_VALUES

/******************************************************************************
 * Device Context Size                                                        *
 *****************************************************************************/
/*
 * Define the maximum number of lines per device in your system. Note that a
 * system (i.e., all devices controlled by one instance of VP-API) could have
 * more than one type of device, each of those devices may support different
 * number of lines. For example in a system, device A might support 2 lines
 * and device B might support 4 lines, in such a scenario, the following macro
 * should be defined as 4. */
#if defined(VP_CC_790_SERIES) || defined(VP_CC_580_SERIES)
    #define VP_MAX_LINES_PER_DEVICE     (4)
#else /* VP_CC_880_SERIES + VP_CC_890_SERIES */
    /*
     * Note that for VE890 this value must be >=2 even when using the single
     * FXO device. That's because the line context pointer in the device
     * context is assigned to the "channelId" element of the line context
     * pointer array. The FXO channelId = 1 so the array must be size >= 2.
     */
    #define VP_MAX_LINES_PER_DEVICE     (2)
#endif

/**< Special Case for Kernel Wrapper: DO NOT TOUCH THESE VALUES */
#if defined(VP_CC_KWRAP)
  #undef VP_MAX_LINES_PER_DEVICE
  #define VP_MAX_LINES_PER_DEVICE (128)
#endif

/**< CSLAC_GAIN_RELATIVE
 * VE880 and VE890 applications only. This value affects whether or not the VE880/VE890 API will
 * support VpSetRelGain() function. Setting to #undef reduces the API library size for each device
 * by ~1K. Applications may choose to compile out VpSetRelGain() functionality and instead use
 * the VP_OPTION_ID_ABS_GAIN feature (see CSLAC_GAIN_ABS).
 *
 * NOTE: The API has not been tested with this string set to #undef.
 */
#define CSLAC_GAIN_RELATIVE

/**< CSLAC_GAIN_ABS
 * VE880 and VE890 applications only. This value affects whether or not the VE880/VE890 API will
 * support the VP_OPTION_ID_ABS_GAIN feature of the API. Setting to #undef reduces the API library
 * size by ~1K independent of whether compiling only for VE880 or VE890, or compiling for both. The
 * majority of the code used to support VP_OPTION_ID_ABS_GAIN is common for these devices.
 * Applications are not likely to use both VP_OPTION_ID_ABS_GAIN and VpSetRelGain() features of the
 * API (see CSLAC_GAIN_RELATIVE for VpSetRelGain() compile option).
 *
 * NOTE: The API has not been tested with this string set to #undef.
 */
#define CSLAC_GAIN_ABS

/**< VP_ENABLE_PROD_TEST
 * This value exists for test purposes only. DO NOT change to #define in production code.
 * Unexpected or damaging behavior may occur.
 */
#undef  VP_ENABLE_PROD_TEST

/**< VP_CID_POLLING_ENABLED
 * This value should only be defined for applications with <= 4 lines on the same MPI BUS and
 * with 8.33ms tickrate. With VP_CID_POLLING_ENABLED set to #define, the API will consume an
 * additional >= 500us per line for each tick (additional 1ms per tick for a 2-FXS device).
 *
 * For systems with tickrates = 8.33ms and using FSK CID with standard baud rate = 1200, for a
 * per-byte line update rate of 8.33ms (same as the tickrate) it is possible on the VE880 and
 * VE890 silicon that without this set to #define the CID message data on the line may
 * corrupt. The error occurs when the API writes to the silicon at "exactly" the same time as
 * the CID message on the line is changing from one byte to the next ("exact" in this context
 * specifically refers to a window of 80ns). Repeated bulk call testing of 100,000 calls
 * indicates that this error can occur in the range of [< 0.001% (>99.999% success) to 4%
 * (96% success)]. Note however these numbers are application and system specific. The problem
 * will occur 100% of the time IF the API write occurs within the 80ns window where the
 * device/line CID message data byte is being updated, and will occur 0% of the time IF this
 * precise timing is avoided.
 *
 * When this is set to #define the exact tick consumption to avoid this error is defined by
 * VP_CID_NORMAL_MPI_CNT described below.
 */
#undef  VP_CID_POLLING_ENABLED

/**< VP_CID_NORMAL_INIT_CNT
 * This count value is used at the start of sending FSK data in a CID sequence.
 *
 * The VE880 and VE890 silicon CID message buffer is 2+ bytes deep. That means the API can at a
 * given time provide a little less than 3 bytes to the silicon. When CID FSK Data is initialized,
 * the API writes one byte to the silicon. It takes the silicon 18us to update it's CID buffer
 * management such that it will then accept another 2 bytes before indicating "Full". When "Full"
 * the silicon will tolerate the maximum amount of VpApiTick() jitter without underrun of the CID
 * message buffer.
 *
 * The exact value of VP_CID_NORMAL_INIT_CNT depends on the DCLK rate and CS-Off time assumed to be
 * 8.192MHz and 2.5us respectively. The delay is implemented in the API by reading the CID State
 * machine buffer where each call to VpMpiCmd() requires 4 bytes of MPI traffic. Therefore, one
 * read will take a minimum of (4 * (2.5us + 8/8.192MHz)) = 13.90625us. In order to meet the minimum
 * time of 18us then requires at least 2 reads. In systems with much longer CS Off times or slower
 * DCLK rates, this count value could be reduced.
 *
 * NOTE: This value is used by the API independant of VP_CID_POLLING_ENABLED setting.
 */
#define VP_CID_NORMAL_INIT_CNT  (2)

/** VP_CID_NORMAL_MPI_CNT
 * This count value is used by the API during normal CID FSK Data transmission (all FSK conditions
 * except for the very first 3 bytes of initialization controlled by VP_CID_NORMAL_INIT_CNT). This
 * value is set to provide a minimum 500us window where the API will monitor the silicon for a
 * Caller ID Buffer update.
 *
 * When running a 8.33ms tickrate and 1200baud FSK, the silicon buffer should update very nearly
 * when VpApiTick() is called. As mentioned above, if the API updates the silicon at almost exactly
 * the same time as the silicon buffer update occurs, Caller ID on the line will be corrupted. The
 * API avoids this by checking for the silicon buffer update before writing to the silicon. With
 * a tickrate of 8.33ms + jitter, the siliocn update should occur either just before VpApiTick() is
 * called or very nearly when VpApiTick() starts.
 *
 * The precise value (36) assumes DCLK = 8.192MHz and CS Off-Time of 2.5us. Each read of the CID
 * state machine requires 4 bytes for total duration of (4 * (2.5us + 8/8.192MHz)) = 13.90625us.
 * To meet the 500us minimum time requires (500 / 13.90625) ~ 36 reads. In systems with much longer
 * CS Off times or slower DCLK rates, this count value could be reduced.
 *
 * NOTE: This value is used by the API ONLY if VP_CID_POLLING_ENABLED is set to #define.
 */
#define VP_CID_NORMAL_MPI_CNT   (36)

/**< VE8XX_LPM_LEAKY_LINE_CNT
 * This count is for the numner of times during a Low Power Mode leaky line test the VP-API-II
 * will retry using LPM state before "giving up".
 */
#define VE8XX_LPM_LEAKY_LINE_CNT    (3)

#ifdef VP_CC_880_SERIES

/*
 * Modifications to previous settings to correct for potential inconsistencies.
 * DO NOT CHANGE anything below this line. All corrections should be made above
 */
#if !defined (VP880_TRACKER_SUPPORT) && defined (VP880_LP_SUPPORT)
    #error "Invalid 880 Compiler Combination: Low Power Mode Requires Tracker defined"
#endif

#if defined (VP880_FXS_SUPPORT) && (!defined (VP880_ABS_SUPPORT) && !defined (VP880_TRACKER_SUPPORT))
    #error "Invalid 880 Compiler Combination: FXS requires either ABS or Tracker defined"
#endif

#ifndef VP880_FXS_SUPPORT
    #undef VP880_ABS_SUPPORT
    #undef VP880_INCLUDE_TESTLINE_CODE
    #undef VP880_LP_SUPPORT
#endif

#if !defined (VP880_FXS_SUPPORT) && !defined (VP880_FXO_SUPPORT)
    #error "Invalid 880 Compiler Combination: Either FXS or FXO must be defined if using VE880 device"
#endif

/*
 * Current Limiting option in the VP880 library limits the peak switcher current
 * such that a 3REN load can be supported at 90V peak. Disable this option if
 * 5REN is required.
 */
#undef  VP880_CURRENT_LIMIT

#endif /* VP_CC_880_SERIES */

#if (defined (VP_CC_880_SERIES) && defined (VP880_INCLUDE_TESTLINE_CODE)) || defined(VP_CC_KWRAP)
    /*
     * #defines used in unit conversion function
     * All units are based on the following calculations
     *      Internal voltage unit   = 480.0  / 2^31 Volts
     *      Internal current unit   = 240e-3 / 2^31 Amps
     *      Internal freq uint      = 12e3   / 2^31 Hz
     */
    #define VP880_UNIT_MV                          4474L    /* mV unit */
    #define VP880_UNIT_MVRMS                       6327L    /* mVrms unit */
    #define VP880_UNIT_ADC_VAB                     -32768L  /* vab step from ADC */
    #define VP880_UNIT_ADC_VAB_RMS                 46341L   /* vab step from ADC rms*/
    #define VP880_UNIT_ADC_AC_VAB_RMS                664L   /* vab step from ADC Metallic Vrms*/
    #define VP880_UNIT_DAC_RING                    21076L   /* step due to DAC in ringing*/
    #define VP880_UNIT_TRACK_SWZ                23091612L   /* step used for tracking switching regulator Z */
    #define VP880_UNIT_UA                          8948L    /* uA unit */
    #define VP880_UNIT_ADC_IMET_NOR                16384L   /* met I from ADC in normal VC */
    #define VP880_UNIT_ADC_IMET_NOR_JA             15756L   /* met I from ADC in normal JA*/
    #define VP880_UNIT_ADC_IMET_RING               32768L   /* met I from ADC in ringing*/
    #define VP880_UNIT_ADC_ILG                     11469L   /* long current from ADC*/
    #define VP880_UNIT_DECHZ                       17896L   /* Deci Hz*/
    #define VP880_UNIT_MHZ                         179L     /* mHz */
    #define VP880_UNIT_FREQ                        65536L   /* siggen freq step */
    #define VP880_UNIT_RAMP_TIME                   1365L    /* Ramp time (-1/3) in ms*/

    #define VP880_UNIT_CONVERT(DATA, I_UNIT, O_UNIT)    (int32)(((int32)DATA * I_UNIT) / O_UNIT)

    /* converts requested slope into SigGen Freq */
    #define VP880_SLOPE_TO_FREQ(AMP,SLOPE)  (int16)(((int32)SLOPE * (int32)VP880_UNIT_RAMP_TIME) / (int32)AMP)

    #define VP880_AC_RLOOP_MAX_BAT_LVL              75000L /* mV */
    #define VP880_AC_RLOOP_MAX_TEST_LVL         ((VP880_AC_RLOOP_MAX_BAT_LVL - 5000) / 2)

#endif /*VP880_INCLUDE_TESTLINE_CODE*/

#ifdef VP_CC_890_SERIES

#define VP890_REL_GAIN_GX_LOW_LIMIT     (0x500)
#define VP890_REL_GAIN_GR_LOW_LIMIT     (0x1000)

#ifndef VP890_FXS_SUPPORT
    /*
     * Tests (using testline) is supported only for FXS termination types. If
     * line tests are required, make sure FXS support is enabled.
     */
    #undef VP890_INCLUDE_TESTLINE_CODE

    /* LPM is an FXS termination type. FXS must be supported to support LPM. */
    #undef  VP890_LP_SUPPORT
#endif

/* Define this to attempt to reduce the level of the noise signal generated for
 * adaptive balance B-Filter calibration.  Reducing the level will increase the
 * negative effect of outside noise on the line and can reduce performance */
#undef VP890_REDUCE_BFILTER_CAL_SIGNAL_LEVEL

#endif /* VP_CC_890_SERIES*/

#if (defined (VP_CC_890_SERIES) && defined (VP890_INCLUDE_TESTLINE_CODE)) || defined(VP_CC_KWRAP)
    /*
     * #defines used in unit conversion function
     * All units are based on the following calculations
     *      Internal voltage unit   = 480.0  / 2^31 Volts
     *      Internal current unit   = 240e-3 / 2^31 Amps
     *      Internal freq uint      = 12e3   / 2^31 Hz
     */
    #define VP890_UNIT_MV                          4474L    /* mV unit */
    #define VP890_UNIT_MVRMS                       6327L    /* mVrms unit */
    #define VP890_UNIT_ADC_VAB                     -32768L  /* vab step from ADC */
    #define VP890_UNIT_ADC_VAB_RMS                 46341L   /* vab step from ADC rms*/
    #define VP890_UNIT_ADC_AC_VAB_RMS              664L     /* vab step from ADC Metallic Vrms*/
    #define VP890_UNIT_DAC_RING                    21076L   /* step due to DAC in ringing*/
    #define VP890_UNIT_TRACK_SWZ                23091612L   /* step used for tracking switching regulator Z */
    #define VP890_UNIT_UA                          8948L    /* uA unit */
    #define VP890_UNIT_ADC_IMET_NOR                15756L   /* met I from ADC in normal*/
    #define VP890_UNIT_ADC_IMET_RING               32768L   /* met I from ADC in ringing*/
    #define VP890_UNIT_ADC_ILG                     11469L   /* long current from ADC*/
    #define VP890_UNIT_DECHZ                       17896L   /* Deci Hz*/
    #define VP890_UNIT_MHZ                         179L     /* mHz */
    #define VP890_UNIT_FREQ                        65536L   /* siggen freq step */
    #define VP890_UNIT_RAMP_TIME                   1365L    /* Ramp time (-1/3) in ms*/

    #define VP890_UNIT_CONVERT(DATA, I_UNIT, O_UNIT) (int32)( ((int32)DATA * I_UNIT) / O_UNIT)

    /* converts requested slope into SigGen Freq */
    #define VP890_SLOPE_TO_FREQ(AMP,SLOPE) (int16) ( ((int32)SLOPE * (int32)VP890_UNIT_RAMP_TIME) / (int32)AMP)

    #define VP890_AC_RLOOP_MAX_BAT_LVL              75000L /* mV */
    #define VP890_AC_RLOOP_MAX_TEST_LVL             ((VP890_AC_RLOOP_MAX_BAT_LVL - 5000) / 2)

#endif /* VP890_INCLUDE_TESTLINE_CODE */

/******************************************************************************
 * COMPILE-TIME OPTIONS:: Top level VP-API Default Device and Line Options    *
 *****************************************************************************/
/*
 *  The following defines specify the default settings of the device and line
 * options used in the API. The VP-API-II should generally take on the default
 * settings set here at VpInitDevice()/VpInitLine() but is not guaranteed (i.e.,
 * the combinations are not tested - VpInitDevice()/InitLine() could fail as a
 * result of setting to unexpected values).
 *
 * Please consult Field or Applications support before changing any of
 * these settings.
 */
#include "vp_api_option.h"
#include "vp_api_event.h"

/* Default Dial Pulse Options specified in units of 125micro sec  */
#define VP_OPTION_DEFAULT_DP_BREAK_MIN          (33 * 8)    /* == 33 ms */
#define VP_OPTION_DEFAULT_DP_BREAK_MAX          (100 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MIN           (17 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MAX           (75 * 8)
#define VP_OPTION_DEFAULT_DP_INTER_DIG_MIN      (250 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MIN          (250 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MAX          (1300 * 8)
#define VP_OPTION_DEFAULT_DP_ON_HOOK_MIN        VP_OPTION_DEFAULT_DP_FLASH_MAX
#define VP_OPTION_DEFAULT_DP_OFF_HOOK_MIN       0

/*
 * Specification 2 is usefull if you need to detect between two non-overlapping
 * dial pulse specifications.
 */
#define VP_OPTION_DEFAULT_DP_BREAK_MIN2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_BREAK_MAX2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MIN2          (0 * 8)
#define VP_OPTION_DEFAULT_DP_MAKE_MAX2          (0 * 8)
#define VP_OPTION_DEFAULT_DP_INTER_DIG_MIN2     (0 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MIN2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_FLASH_MAX2         (0 * 8)
#define VP_OPTION_DEFAULT_DP_ON_HOOK_MIN2       (0 * 8)

/* Default setting for Critical fault option */
#define VP_OPTION_DEFAULT_CF_AC_DIS_EN          TRUE
#define VP_OPTION_DEFAULT_CF_DC_DIS_EN          TRUE
#define VP_OPTION_DEFAULT_CF_THERMAL_DIS_EN     TRUE
/* CSLAC-880 and VCP-880 do not support the detection of AC, DC critical faults.
 * So, The VP-API uses the following default values for those devices.
 * Note that changing the following values will result in VpInitDevice()
 * failure. */
#define VP_OPTION_DEFAULT_CF_AC_DIS_EN_880      FALSE
#define VP_OPTION_DEFAULT_CF_DC_DIS_EN_880      FALSE

/* Default setting for Zero cross option */
#define VP_OPTION_DEFAULT_ZERO_CROSS            VP_OPTION_ZC_M4B

/* Default setting for Ramp to standby option */
#define VP_OPTION_DEFAULT_RAMP2STBY             0

/* Default setting for Pulse mode option */
#define VP_OPTION_DEFAULT_PULSE_MODE            VP_OPTION_PULSE_DECODE_OFF

/* Default CODEC Mode */
#define VP_OPTION_DEFAULT_CODEC_MODE            VP_OPTION_ALAW

/* Default setting for PCM Highway option */
#define VP_OPTION_DEFAULT_PCM_HWY               VP_OPTION_HWY_A

/* Default setting for Loop back option */
#define VP_OPTION_DEFAULT_LOOP_BACK             VP_OPTION_LB_OFF

/* Default setting for Line state option */
#define VP_OPTION_DEFAULT_LS_BAT_REV            FALSE
#define VP_OPTION_DEFAULT_LS_BAT                VP_OPTION_BAT_AUTO

/*
 * Default Event Masks (VP_OPTION_ID_EVENT_MASK)
 * The user can arbitrarily set the mask bits here as needed.  A mask bit set to
 * '0' means the event is not masked.  Some events are non-maskable.
 */
#define VP_OPTION_DEFAULT_FAULT_EVENT_MASK      (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_FAULT_EVENTS)
#define VP_OPTION_DEFAULT_SIGNALING_EVENT_MASK  (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_SIGNALING_EVENTS)
#define VP_OPTION_DEFAULT_RESPONSE_EVENT_MASK   (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_RESPONSE_EVENTS)
#define VP_OPTION_DEFAULT_TEST_EVENT_MASK       (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_TEST_EVENTS)
#define VP_OPTION_DEFAULT_PROCESS_EVENT_MASK    (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_PROCESS_EVENTS)
#define VP_OPTION_DEFAULT_FXO_EVENT_MASK        (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_FXO_EVENTS)
#define VP_OPTION_DEFAULT_PACKET_EVENT_MASK     (VP_EVENT_MASK_ALL & ~VP_API_NONMASK_PACKET_EVENTS)

/* Default setting for Ring Control option */
#define VP_OPTION_DEFAULT_RC_ZERO_CROSS         VP_OPTION_ZC_M4B
#define VP_OPTION_DEFAULT_RC_RING_EXIT_DBNC_VAL (100 * 8) /* 100ms */
#define VP_OPTION_DEFAULT_RC_RING_EXIT_STATE    VP_LINE_TALK

/* Default setting for DTMF Mode option */
#define VP_OPTION_DEFAULT_DM_DTMF_CONTROL_MODE  VP_OPTION_DTMF_DECODE_OFF
#define VP_OPTION_DEFAULT_DM_DIRECTION          VP_DIRECTION_US

/* Default setting for PCM TX/RX path control */
#define VP_OPTION_DEFAULT_PCM_TXRX_CNTRL        VP_OPTION_PCM_BOTH

/* Default setting for DTMF Spec option */
#define VP_OPTION_DEFAULT_DTMF_SPEC             VP_OPTION_DTMF_SPEC_ATT

/* Default setting for Park Mode option */
#define VP_OPTION_DEFAULT_PARK_MODE_DISC        4   /* 2 seconds */
#define VP_OPTION_DEFAULT_PARK_MODE_STANDBY     3   /* 300 ms */

/* Default setting for DC Feed Slope option */
#define VP_OPTION_DEFAULT_DCFEED_SLOPE          109 /* 4 V/ms */

/* Default setting for hook detection mode option */
#define VP_OPTION_DEFAULT_HOOK_DETECT_MODE      VP_OPTION_HOOKDET_NORMAL

/* Default settings for PCM highway signaling and control */
#define VP_OPTION_DEFAULT_PCM_SIG_CTL_ENABLE    FALSE
#define VP_OPTION_DEFAULT_PCM_SIG_CTL_CTLTS     0
#define VP_OPTION_DEFAULT_PCM_SIG_CTL_SIGTS     0

/* Default setting for PCM highway lien state control mode */
#define VP_OPTION_DEFAULT_LINESTATE_CTL_MODE    VP_OPTION_LINESTATE_CTL_NORMAL

/* Default settings for Auto Loop Conditions option */
#define VP_OPTION_DEFAULT_AUTO_LOOP_COND_SELECT 0
#define VP_OPTION_DEFAULT_AUTO_LOOP_COND_DELAY  0

#define VP_SIZEOF_CID_MSG_BUFFER 16

#endif /* VP_API_CFG_INT_H */
