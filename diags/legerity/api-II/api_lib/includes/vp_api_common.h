/** \file vp_api_common.h
 * vp_api_common.h
 *
 * Header file for the VP-API-II c files.
 *
 * This file contains all of the VP-API-II declarations that are Voice
 * Termination Device (VTD) family independent.
 *
 * $Revision: 1.1.1.1 $
 * $LastChangedDate: 2011-09-22 14:13:03 -0500 (Thu, 22 Sep 2011) $
 */

#ifndef VP_API_COMMON_H
#define VP_API_COMMON_H

#include "../../arch/uvb/vp_api_types.h"
#include "vp_api_dev_term.h"
#include "vp_api_event.h"
#include "vp_api_option.h"
#include "vp_api_test.h"


/******************************************************************************
 *                                DEFINES                                     *
 ******************************************************************************/

/*
 * Profile Table Indexes
 */
#define VP_PTABLE_NULL      (VpProfilePtrType)0
#define VP_PTABLE_INDEX1    (VpProfilePtrType)1
#define VP_PTABLE_INDEX2    (VpProfilePtrType)2
#define VP_PTABLE_INDEX3    (VpProfilePtrType)3
#define VP_PTABLE_INDEX4    (VpProfilePtrType)4
#define VP_PTABLE_INDEX5    (VpProfilePtrType)5
#define VP_PTABLE_INDEX6    (VpProfilePtrType)6
#define VP_PTABLE_INDEX7    (VpProfilePtrType)7
#define VP_PTABLE_INDEX8    (VpProfilePtrType)8
#define VP_PTABLE_INDEX9    (VpProfilePtrType)9
#define VP_PTABLE_INDEX10   (VpProfilePtrType)10
#define VP_PTABLE_INDEX11   (VpProfilePtrType)11
#define VP_PTABLE_INDEX12   (VpProfilePtrType)12
#define VP_PTABLE_INDEX13   (VpProfilePtrType)13
#define VP_PTABLE_INDEX14   (VpProfilePtrType)14
#define VP_PTABLE_INDEX15   (VpProfilePtrType)15
#define VP_PTABLE_NO_INDEX   (VpProfilePtrType)99
#define VP_PTABLE_MAX_INDEX VP_PTABLE_INDEX15
#define VP_PTABLE_INDEX(n)  (VpProfilePtrType)(n)


/*
 *Utility macros:
 */
#ifndef ABS
  #define ABS(arg) ((arg) < 0 ? -(arg) : (arg))
#endif

#ifndef MAX
  #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
  #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX4
  #define MAX4(a, b, c, d) MAX(MAX(a, b), MAX(c, d))
#endif

#ifndef MAX12
  #define MAX12(a, b, c, d, e, f, g, h, i, j, k, l) \
    MAX(MAX(MAX4(a, b, c, d), MAX4(e, f, g, h)), MAX4(i, j, k, l))
#endif


/******************************************************************************
 *                    ENUMERATIONS AND NEW DATA TYPES                         *
 ******************************************************************************/
/** Standard return value for most API library functions.  See the VP-API-II
    Reference Guide for descriptions. */
typedef enum VpStatusType {
    VP_STATUS_SUCCESS              = 0,
    VP_STATUS_FAILURE              = 1,
    VP_STATUS_FUNC_NOT_SUPPORTED   = 2,
    VP_STATUS_INVALID_ARG          = 3,
    VP_STATUS_MAILBOX_BUSY         = 4,
    VP_STATUS_ERR_VTD_CODE         = 5,
    VP_STATUS_OPTION_NOT_SUPPORTED = 6,
    VP_STATUS_ERR_VERIFY           = 7,
    VP_STATUS_DEVICE_BUSY          = 8,
    VP_STATUS_MAILBOX_EMPTY        = 9,
    VP_STATUS_ERR_MAILBOX_DATA     = 10,
    VP_STATUS_ERR_HBI              = 11,
    VP_STATUS_ERR_IMAGE            = 12,
    VP_STATUS_IN_CRTCL_SECTN       = 13,
    VP_STATUS_DEV_NOT_INITIALIZED  = 14,
    VP_STATUS_ERR_PROFILE          = 15,
    VP_STATUS_CUSTOM_TERM_NOT_CFG  = 17,
    VP_STATUS_DEDICATED_PINS       = 18,
    VP_STATUS_INVALID_LINE         = 19,
    VP_STATUS_LINE_NOT_CONFIG      = 20,
    VP_STATUS_INPUT_PARAM_OOR,  /* This enum value does not yet require to be a set value */
    VP_STATUS_NUM_TYPES,
    VP_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpStatusType;

/* Enumeration for Boot load Function */
typedef enum VpBootStateType {
    VP_BOOT_STATE_FIRST,        /* First block to download */
    VP_BOOT_STATE_CONTINUE,     /* Additional block to download */
    VP_BOOT_STATE_LAST,         /* Last block to download */
    VP_BOOT_STATE_FIRSTLAST,    /* First and only block to download */
    VP_NUM_BOOT_STATES,
    VP_BOOT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBootStateType;

/* Enumeration for Boot load Function */
typedef enum VpBootModeType {
    VP_BOOT_MODE_NO_VERIFY,     /* No write verification is performed */
    VP_BOOT_MODE_VERIFY,        /* Verify Load Image Checksum */
    VP_NUM_BOOT_MODES,
    VP_BOOT_MODE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBootModeType;

/* Version and checksum information for VCP */
typedef struct VpVersionInfoType {
    uint16 vtdRevCode;          /* Silicon Revision for VTD */
    uint8 swProductId;          /* VTD Firmware ID */
    uint8 swVerMajor;           /* Major Revision for VTD Firmware */
    uint8 swVerMinor;           /* Minor Revision for VTD Firmware */
} VpVersionInfoType;

typedef struct VpChkSumType {
    uint32 loadChecksum;        /* Calculated Checksum for Code Image */
    VpVersionInfoType vInfo;    /* Version Information for VTD HW/SW */
} VpChkSumType;

typedef enum VpProfileType {
    VP_PROFILE_DEVICE      = 0, /* Device profile */
    VP_PROFILE_AC          = 1, /* AC profile */
    VP_PROFILE_DC          = 2, /* DC profile */
    VP_PROFILE_RING        = 3, /* Ring profile */
    VP_PROFILE_RINGCAD     = 4, /* Ringing cadence profile */
    VP_PROFILE_TONE        = 5, /* Tone profile */
    VP_PROFILE_METER       = 6, /* Metering profile */
    VP_PROFILE_CID         = 7, /* Caller ID profile */
    VP_PROFILE_TONECAD     = 8, /* Tone cadence profile */
    VP_PROFILE_FXO_CONFIG,      /* FXO configuration profile */
    VP_PROFILE_CUSTOM_TERM,     /* Custom Termination Type profile */
    VP_PROFILE_CAL,             /* Used for VpCal when passing calibration coefficients */
    VP_NUM_PROFILE_TYPES,
    VP_PROFILE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpProfileType;

/**< typedef for VpCalCodec() function */
typedef enum VpDeviceCalType {
    VP_DEV_CAL_NOW   = 0, /* Calibrate immediately */
    VP_DEV_CAL_NBUSY = 1, /* Calibrate if all lines are "on-hook" */
    VP_NUM_DEV_CAL_TYPES,
    VP_DEV_CAL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceCalType;

/* Valid line states for VpSetLineState() */
/* Defined else where */

/**< The following types are for VpSetLineTone() function */
typedef struct VpDtmfToneGenType {
    VpDigitType toneId;  /* Identifies the DTMF tone to generated */
    VpDirectionType dir; /* Direction in which DTMF tone needs to be
                          * generated */
} VpDtmfToneGenType;

typedef enum VpDigitGenerationType {
  VP_DIGIT_GENERATION_DTMF,             /* Generate DTMF digit */
  VP_DIGIT_GENERATION_DIAL_PULSE,       /* Generate pulse digit */
  VP_DIGIT_GENERATION_DIAL_HOOK_FLASH,   /* Generate hook flash */
  VP_DIGIT_GEN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDigitGenerationType;

typedef enum VpBFilterModeType {
  VP_BFILT_DIS = 0,
  VP_BFILT_EN  = 1,
  VP_BFILT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBFilterModeType;

typedef enum VpBatteryModeType {
  VP_BATT_MODE_DIS = 0,
  VP_BATT_MODE_EN  = 1,
  VP_BATT_MODE_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBatteryModeType;

typedef struct VpBatteryValuesType {
    int16 batt1;
    int16 batt2;
    int16 batt3;
} VpBatteryValuesType;

/**< The following types are for VpSetRelayState() and VpGetRelayState() functions */
typedef enum VpRelayControlType {
    VP_RELAY_NORMAL         = 0,  /* No test access, ringing controlled
                                   * by line state */
    VP_RELAY_RESET          = 1,  /* LCAS all-off state, test-out released */
    VP_RELAY_TESTOUT        = 2,  /* LCAS all-off state, test-out active */
    VP_RELAY_TALK           = 3,  /* LCAS talk state, test-out released */
    VP_RELAY_RINGING        = 4,  /* LCAS ringing state, test-out released */
    VP_RELAY_TEST           = 5,  /* LCAS test state, test-out released */
    VP_RELAY_BRIDGED_TEST   = 6,  /* LCAS test/monitor state,
                                   * test-out released */
    VP_RELAY_SPLIT_TEST     = 7,  /* LCAS test/monitor state,
                                   * test-out active */
    VP_RELAY_DISCONNECT     = 8,  /* LCAS talk state, test-out active */
    VP_RELAY_RINGING_NOLOAD = 9,  /* LCAS ringing state, test-out active */
    VP_RELAY_RINGING_TEST   = 10, /* LCAS test ringing state,
                                   * test-out active */
    VP_RELAY_LAMP_ON        = 11, /* VCP2-790 FXS_TL_MW only */
    VP_RELAY_IDLE           = 12, /* All switches open, test switch off */
    VP_NUM_RELAY_STATES,
    VP_RELAY_ENUM_RSVD  = FORCE_SIGNED_ENUM,
    VP_RELAY_ENUM_SIZE  = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpRelayControlType;

/**< The following types are for VpSetCalRelayState() function */
typedef enum VpCalRelayControlType {
    VP_CAL_RELAY_OPEN,                /* Open circuit                      */
    VP_CAL_RELAY_R_DIFF_LOW_TIP_RING, /* Res Tip-to-Ring: R_DIFF_LOW Ohms  */
    VP_CAL_RELAY_R_CM_REF_TIP_GND,    /* Res Tip-to-Gnd:  R_CM_REF Ohms    */
    VP_CAL_RELAY_V_CAL_REF_TIP_GND,   /* Volt Tip:        V_CAL_REF Volts  */
    VP_CAL_RELAY_SHORT_TIP_RING,      /* Res Tip-to-Ring: short (0 Ohms)   */
    VP_CAL_RELAY_R_DIFF_HIGH_TIP_RING,/* Res Tip-to-Ring: R_DIFF_HIGH Ohms */
    VP_CAL_RELAY_R_CM_REF_RING_GND,   /* Res Ring-to-Gnd: R_CM_REF Ohms    */
    VP_CAL_RELAY_V_CAL_REF_RING_GND,  /* Volt Ring:       V_CAL_REF Volts  */
    VP_CAL_RELAY_NUM_STATES,
    VP_CAL_RELAY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req.*/
} VpCalRelayControlType;

/**< The following types are for VpSetRelGain() function */
typedef enum VpGainResultType {
    VP_GAIN_SUCCESS  = 0, /* Gain setting adjusted successfully */
    VP_GAIN_GR_OOR   = 1, /* GR Gain setting overflowed (reset to default) */
    VP_GAIN_GX_OOR   = 2, /* GX Gain setting overflowed (reset to default) */
    VP_GAIN_BOTH_OOR = 3, /* Both GR & GX overflowed (and reset to default) */
    VP_GAIN_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpGainResultType;

typedef struct VpRelGainResultsType {
    VpGainResultType gResult; /* Success / Failure status return */
    uint16 gxValue;           /* new GX register abs value */
    uint16 grValue;           /* new GR register abs value */
} VpRelGainResultsType;

/* The number of 16 bits words used to define whether seal current
 * should be applied to lines 0-127 */
#define VP_SEAL_CURRENT_ARRAY_SIZE 8

typedef struct VpSealCurType {
    uint16 sealApplyTime;
    uint16 sealCycleTime;
    uint16 maxCurrent;
    uint16 minCurrent;
    uint16 sealMaskArray[VP_SEAL_CURRENT_ARRAY_SIZE];
} VpSealCurType;

/**< The following types are for the VpQuery() function */
typedef enum VpQueryIdType {
    VP_QUERY_ID_TEMPERATURE = 0,
    VP_QUERY_ID_METER_COUNT = 1,
    VP_QUERY_ID_LOOP_RES    = 2,
    VP_QUERY_ID_SEAL_CUR    = 3,
    VP_NUM_QUERY_IDS,
    VP_QUERY_ID_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpQueryIdType;

typedef union VpQueryResultsType {
    int16 temp;
    uint16 meterCount;
    uint16 rloop;
    VpSealCurType sealCur;
} VpQueryResultsType;

/**< The following types are for VpLineIoAccess() function */
typedef enum VpIoDirectionType {
    VP_IO_WRITE = 0,
    VP_IO_READ  = 1,
    VP_IO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpIoDirectionType;

typedef struct VpLineIoBitsType {
    uint8 mask;
    uint8 data;
} VpLineIoBitsType;

typedef struct VpLineIoAccessType {
    VpIoDirectionType direction;
    VpLineIoBitsType ioBits;
} VpLineIoAccessType;

/**< The following types are for VpDeviceIoAccess() function */
typedef enum VpDeviceIoAccessType {
    VP_DEVICE_IO_WRITE = 0, /* Perform device specific IO write access */
    VP_DEVICE_IO_READ  = 1, /* Perform device specific IO read access */
    VP_DEVICE_IO_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceIoAccessType;

typedef enum VpDeviceIoAccessMask {
    VP_DEVICE_IO_IGNORE = 0, /* Ignore I/O access */
    VP_DEVICE_IO_ACCESS = 1, /* Perform I/O access */
    VP_DEVUCE_IOA_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDeviceIoAccessMask;

typedef struct VpDeviceIoAccessDataType {
    VpDeviceIoAccessType accessType;/* Device I/O access type */
    uint32 accessMask_31_0;         /* I/O access mask (Pins 0 - 31) */
    uint32 accessMask_63_32;        /* I/O access mask (Pins 32 - 63) */
    uint32 deviceIOData_31_0;   /* Output pin data (Pins 0 - 31) */
    uint32 deviceIOData_63_32;  /* Output pin data (Pins 32 - 63) */
} VpDeviceIoAccessDataType;

/**< The following types are for VpDeviceIoAccessExt() function */

typedef struct VpDeviceIoAccessExtType {
    VpIoDirectionType direction;
    VpLineIoBitsType lineIoBits[VP_MAX_LINES_PER_DEVICE];
} VpDeviceIoAccessExtType;

/**< The following types are for VpGetLineStatus() function */
typedef enum VpInputType {
    VP_INPUT_HOOK      = 0, /* Hook Status (ignoring pulse & flash) */
    VP_INPUT_RAW_HOOK  = 1, /* Hook Status (include pulse & flash) */
    VP_INPUT_GKEY      = 2, /* Ground-Key/Fault Status */
    VP_INPUT_THERM_FLT = 3, /* Thermal Fault Status */
    VP_INPUT_CLK_FLT   = 4, /* Clock Fault Status */
    VP_INPUT_AC_FLT    = 5, /* AC Fault Status */
    VP_INPUT_DC_FLT    = 6, /* DC Fault Status */
    VP_INPUT_BAT1_FLT  = 7, /* Battery 1 Fault Status */
    VP_INPUT_BAT2_FLT  = 8, /* Battery 2 Fault Status */
    VP_INPUT_BAT3_FLT  = 9, /* Battery 3 Fault Status */

    VP_INPUT_RINGING,    /* Ringing Status */
    VP_INPUT_LIU,        /* Line In Use Status */
    VP_INPUT_FEED_DIS,   /* Feed Disable Status */
    VP_INPUT_FEED_EN,    /* Feed Enable Status */
    VP_INPUT_DISCONNECT, /* Feed Disconnect Status */
    VP_INPUT_CONNECT,    /* Feed Connect Status */
    VP_INPUT_POLREV,     /* Reverse Polarity if TRUE, Normal if FALSE */
    VP_INPUT_RES_LEAK,   /* Resistive Leakage on line if TRUE */

    VP_NUM_INPUT_TYPES,
    VP_INPUT_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpInputType;

/**< The following type is for the VpGetDeviceStatusExt() function: */
typedef struct VpDeviceStatusType {
    VpInputType input;
    uint8 status[VP_LINE_FLAG_BYTES];
} VpDeviceStatusType;

/**< The following types are for VpGetLoopCond() function (return via
 * VpGetResults())
 */
typedef enum VpBatteryType {
    VP_BATTERY_UNDEFINED = 0, /* Battery being used is not known or feature
                               * not supported */
    VP_BATTERY_1         = 1, /* Battery 1 */
    VP_BATTERY_2         = 2, /* Battery 2 */
    VP_BATTERY_3         = 3, /* Battery 3 */
    VP_NUM_BATTERY_TYPES,
    VP_BATTERY_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpBatteryType;

typedef enum VpDcFeedRegionType {
    VP_DF_UNDEFINED    = 0, /* DC feed region not known or feature not
                             * supported */
    VP_DF_ANTI_SAT_REG = 1, /* DC feed is in anti saturation region */
    VP_DF_CNST_CUR_REG = 2, /* DC feed is in constant current region */
    VP_DF_RES_FEED_REG = 3, /* DC feed is in resistive feed region */
    VP_NUM_DC_FEED_TYPES,
    VP_DC_FEED_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpDcFeedRegionType;

typedef struct VpLoopCondResultsType {
    int16 rloop;                    /**< The loop resistance */
    int16 ilg;                      /**< The longitudinal loop current */
    int16 imt;                      /**< The metallic loop current */
    int16 vsab;                     /**< The tip/ring voltage */
    int16 vbat1;                    /**< The Battery 1 voltage */
    int16 vbat2;                    /**< The Battery 2 voltage */
    int16 vbat3;                    /**< The Battery 3 voltage */
    int16 mspl;                     /**< Metering signal peak level */
    VpBatteryType selectedBat;      /**< Battery that is presently used for
                                     *   the DC feed */
    VpDcFeedRegionType dcFeedReg;   /**< DC feed region presently selected */
} VpLoopCondResultsType;

/**< The following types are used for the function VpSendSignal() */
typedef enum VpSendSignalType {
    VP_SENDSIG_MSG_WAIT_PULSE      = 0,
    VP_SENDSIG_DTMF_DIGIT          = 1,
    VP_SENDSIG_PULSE_DIGIT         = 2,
    VP_SENDSIG_HOOK_FLASH          = 3,
    VP_SENDSIG_FWD_DISCONNECT      = 4,
    VP_SENDSIG_POLREV_PULSE        = 5,
    VP_SENDSIG_MOMENTARY_LOOP_OPEN = 6,
    VP_SENDSIG_TIP_OPEN_PULSE      = 7,
    VP_SENDSIG_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpSendSignalType;

typedef struct VpSendMsgWaitType {
    int8 voltage;   /**< Voltage in Volts to apply to the line. A negative
                     * value means Tip is more Negative than Ring, a positive
                     * value means Ring is more Negative than Tip.
                     */
    uint16 onTime;  /**< Duration of pulse on-time in mS */
    uint16 offTime; /**< Duration of pulse off-tim in mS. If the off-time is
                     * set to 0, the voltage is applied to the line continuously
                     */
    uint8 cycles;   /**< Number of pulses to send on the line. If set to 0, will
                     * repeat forever
                     */
} VpSendMsgWaitType;

/**< The following type is used for the function VpGenTimerCtrl() */
typedef enum VpGenTimerCtrlType {
    VP_GEN_TIMER_START  = 0,
    VP_GEN_TIMER_CANCEL = 1,
    VP_GEN_TIMER_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpGenTimerCtrlType;

/**< The enum is used by the system service layer pcmCollect() function */
typedef enum VpPcmOperationBitType {
    VP_PCM_OPERATION_AVERAGE      = 0x0001, /* average PCM samples */
    VP_PCM_OPERATION_RANGE        = 0x0002, /* find pk to pk pcm sample data. */
    VP_PCM_OPERATION_RMS          = 0x0004, /* find rms value of pcm samples */
    VP_PCM_OPERATION_MIN          = 0x0008, /* find min pcm value of pcm samples */
    VP_PCM_OPERATION_MAX          = 0x0010, /* find max pcm value of pcm samples */
    VP_PCM_OPERATION_APP_SPECIFIC = 0x0020, /* generic operation */
    VP_PCM_OPERATION_FREQ         = 0x0040, /* Measure frequency */
    VP_PCM_OPERATION_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpPcmOperationBitType;

typedef uint16 VpPcmOperationMaskType;

typedef struct VpPcmOperationResultsType {
    int16 average;  /* Average value of all collected PCM samples */
    int16 range;    /* Maximum - minimum PCM value during operation time */
    int16 rms;      /* RMS result of all collected pcm samples */
    int16 min;      /* Maximum pcm value found */
    int16 max;      /* Minimum pcm value found */
    void *pApplicationInfo; /* Any results that the implementation chooses to
                             * pass. This data is not interpreted neither
                             * by the VP-API nor the LT-API. LT-API passes
                             * this pointer back to the application as part
                             * of the test result
                             */
    int32 freq;     /* Measured average frequenny */

    bool error;     /* indication of PCM operation process (1 = failure) */
} VpPcmOperationResultsType;

/**< The enum is used by the cal function */
typedef enum VpCalType {
    VP_CAL_BFILTER              = 1,
    VP_CAL_APPLY_BFILTER        = 2,
    VP_CAL_MEASURE_BFILTER      = 3,
    VP_CAL_GET_SYSTEM_COEFF     = 4,
    VP_CAL_APPLY_SYSTEM_COEFF   = 5,
    VP_CAL_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCalType;

typedef struct VpCalBFilterType {
    uint16 vRms;    /**< Customer Target Value in approximate vRms */
    VpProfilePtrType pAcProfile;
} VpCalBFilterType;

typedef struct VpCalApplyBFilterType {
    VpProfilePtrType pAcProfile;
    uint16 index;
} VpCalApplyBFilterType;

typedef enum VpCalStatusType {
    VP_CAL_SUCCESS,
    VP_CAL_FAILURE,
    VP_CAL_STATUS_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpCalStatusType;

typedef enum VpLowLevelCmdType {
    VP_LOWLEV_PAGE_WR = 0,
    VP_LOWLEV_PAGE_RD = 1,
    VP_LOWLEV_MBOX_WR = 2,
    VP_LOWLEV_MBOX_RD = 3,
    VP_LOWLEV_ENUM_SIZE = FORCE_STANDARD_C_ENUM_SIZE /* Portability Req. */
} VpLowLevelCmdType;

typedef enum VpProfileFieldIdType {

    /* Device parameters */

    /* DC parameters */
    VP_PROF_FIELD_ILA

} VpProfileFieldIdType;

typedef struct VpProfileFieldType {
    VpProfileFieldIdType fieldId;

    union data {
        uint16 ila;
    } data;

} VpProfileFieldType;

/* The following struct can be passed to VpGetResults() if the event ID is not
   known at compile time, to ensure that the buffer is large enough regardless
   of the result type. */
typedef union VpResultsType {
    VpChkSumType codeChecksum;                  /* VP_DEV_EVID_BOOT_CMP or
                                                   VP_DEV_EVID_CHKSUM          */
    VpRelGainResultsType setRelGain;            /* VP_LINE_EVID_GAIN_CMP       */
    uint8 lowLevelCmd[256];                     /* VP_LINE_EVID_LLCMD_RX_CMP   */
    VpLoopCondResultsType getLoopCond;          /* VP_LINE_EVID_RD_LOOP        */
    VpOptionValueType getOption;                /* VP_LINE_EVID_RD_OPTION      */
    VpTestResultType testLine;                  /* VP_LINE_EVID_TEST_CMP       */
    VpDeviceIoAccessDataType deviceIoAccess;    /* VP_DEV_EVID_IO_ACCESS_CMP   */
    VpDeviceIoAccessExtType deviceIoAccessExt;  /* VP_DEV_EVID_IO_ACCESS_CMP   */
    VpLineIoAccessType lineIoAccess;            /* VP_LINE_EVID_LINE_IO_RD_CMP */
    VpQueryResultsType query;                   /* VP_LINE_EVID_QUERY_CMP      */
} VpResultsType;

typedef enum VpFreeRunModeType {
    VP_FREE_RUN_START,
    VP_FREE_RUN_STOP
} VpFreeRunModeType;

/******************************************************************************
 *                 DEVICE/LINE CONTEXT (SUPPORT) DEFINITION                   *
 ******************************************************************************/
/* The following function pointers are required to get around problem of
 * what comes first? chicken or the egg? when dealing with declaration of
 * device context, function pointer table and hence the following declarations.
 */
struct VpDevCtxType;    /**< forward declaration */
struct VpLineCtxType;   /**< forward declaration */
struct VpEventType;     /**< forward declaration */

/*
 * System Configuration functions
 */
typedef VpStatusType
(*VpMakeLineObjectFuncPtrType) (
    VpTermType termType,
    uint8 channelId,
    struct VpLineCtxType *pLineCtx,
    void *pLineObj,
    struct VpDevCtxType *pDevCtx);

/*
 * Initialization functions
 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpBootLoadFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation);

typedef VpStatusType
(*VpBootSlacFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize);
#endif  /* !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * | defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

typedef VpStatusType
(*VpInitDeviceFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile,
    VpProfilePtrType pFxoAcProfile,
    VpProfilePtrType pFxoCfgProfile);

typedef VpStatusType
(*VpFreeRunFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpFreeRunModeType freeRunMode);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpInitSlacFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

typedef VpStatusType
(*VpInitLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

typedef VpStatusType
(*VpConfigLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
typedef VpStatusType
(*VpSetBFilterFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpBFilterModeType bFiltMode,
    VpProfilePtrType pAcProfile);

typedef VpStatusType
(*VpSetBatteriesFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpBatteryModeType battMode,
    VpBatteryValuesType *pBatt);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
         */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES) \
                                || (defined(VP_CC_880_SERIES) && defined(VP_ENABLE_PROD_TEST))
typedef VpStatusType
(*VpCalCodecFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpDeviceCalType mode);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES) \
         * || (defined(VP_CC_880_SERIES) && defined(VP_ENABLE_PROD_TEST))
         */

typedef VpStatusType
(*VpCalLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx);

typedef VpStatusType
(*VpCalFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpCalType calType,
    void *inputArgs);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CSLAC_SEQ_EN)
typedef VpStatusType
(*VpInitRingFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pCadProfile,
    VpProfilePtrType pCidProfile);

typedef VpStatusType
(*VpInitCidFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

typedef VpStatusType
(*VpInitMeterFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CSLAC_SEQ_EN)
         */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
typedef VpStatusType
(*VpInitCustomTermTypeFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pCustomTermProfile);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
         */

typedef VpStatusType
(*VpInitProfileFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpSoftResetFuncPtrType) (
    struct VpDevCtxType *pDevCtx);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

/*
 * Control functions
 */
typedef VpStatusType
(*VpSetLineStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpLineStateType state);

typedef VpStatusType
(*VpSetLineToneFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

typedef VpStatusType
(*VpSetRelayStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpRelayControlType rState);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
typedef VpStatusType
(*VpSetCalRelayStateFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpCalRelayControlType rState);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
         */

typedef VpStatusType
(*VpSetRelGainFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 txLevel,
    uint16 rxLevel,
    uint16 handle);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)  || defined(VP_CSLAC_SEQ_EN)
typedef VpStatusType
(*VpSendSignalFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpSendSignalType signalType,
    void *pSignalData);

typedef VpStatusType
(*VpSendCidFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 length,
    VpProfilePtrType pCidProfile,
    uint8p pCidData);

typedef VpStatusType
(*VpContinueCidFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

typedef VpStatusType
(*VpStartMeterFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP)  || defined(VP_CSLAC_SEQ_EN)
         */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpStartMeter32QFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint32 minDelay,
    uint32 onTime,
    uint32 offTime,
    uint16 numMeters,
    uint16 eventRate);

typedef VpStatusType
(*VpGenTimerCtrlFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpGenTimerCtrlType timerCtrl,
    uint32 duration,
    uint16 handle);

typedef VpStatusType
(*VpAssocDslLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    bool connect,
    uint8 line);

typedef VpStatusType
(*VpSetSealCurFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    uint16 sealApplyTime,
    uint16 sealCycleTime,
    uint16 maxCurrent,
    uint16 minCurrent,
    uint16 *pSealArray,
    VpProfilePtrType pDcProfile);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

typedef VpStatusType
(*VpSetOptionFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    struct VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *pValue);

typedef VpStatusType
(*VpDeviceIoAccessFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpDeviceIoAccessExtFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpDeviceIoAccessExtType *pDeviceIoAccess);

typedef VpStatusType
(*VpLineIoAccessFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpLineIoAccessType *pLineIoAccess,
    uint16 handle);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

typedef VpStatusType
(*VpVirtualISRFuncPtrType) (
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpApiTickFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    bool *pEventStatus);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(ZARLINK_CFG_INTERNAL)
typedef VpStatusType
(*VpLowLevelCmdFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle);
#endif

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpLowLevelCmd16FuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpLowLevelCmdType cmdType,
    uint16 *writeWords,
    uint8 numWriteWords,
    uint8 numReadWords,
    uint16 handle);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

/*
 * Status and query functions
 */

typedef bool
(*VpGetEventFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    struct VpEventType *pEvent);

typedef VpStatusType
(*VpGetLineStatusFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus);

typedef VpStatusType
(*VpGetDeviceStatusFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpGetDeviceStatusExtFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    VpDeviceStatusType *pDeviceStatus);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
typedef VpStatusType
(*VpGetLoopCondFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 handle);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
         */

typedef VpStatusType
(*VpGetOptionFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    struct VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle);

typedef VpStatusType
(*VpGetLineStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpLineStateType *pCurrentState);

typedef VpStatusType
(*VpFlushEventsFuncPtrType) (
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpGetResultsFuncPtrType) (
    struct VpEventType *pEvent,
    void *pResults);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpClearResultsFuncPtrType) (
    struct VpDevCtxType *pDevCtx);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
                                || (defined(VP880_INCLUDE_TESTLINE_CODE) && defined(VP_CC_880_SERIES)) \
                                || (defined(VP890_INCLUDE_TESTLINE_CODE) && defined(VP_CC_890_SERIES))
typedef VpStatusType
(*VpGetRelayStateFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpRelayControlType *pRstate);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
         * || (defined(VP880_INCLUDE_TESTLINE_CODE) && defined(VP_CC_880_SERIES)) \
         * || (defined(VP890_INCLUDE_TESTLINE_CODE) && defined(VP_CC_890_SERIES))
         */

typedef VpStatusType
(*VpObjectDumpFuncPtrType)(
    struct VpLineCtxType *pLineCtx,
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpRegisterDumpFuncPtrType)(
    struct VpDevCtxType *pDevCtx);

typedef VpStatusType
(*VpDtmfDigitDetectedFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)

typedef VpStatusType
(*VpQueryFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpQueryIdType queryId,
    uint16 handle);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

/*
 * Test Functions
 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
                                || (defined(VP_CC_880_SERIES) && defined (VP880_INCLUDE_TESTLINE_CODE)) \
                                || (defined(VP_CC_890_SERIES) && defined (VP890_INCLUDE_TESTLINE_CODE))
typedef VpStatusType
(*VpTestLineFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle);

typedef VpStatusType
(*VpTestLineIntFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle,
    bool callback);

typedef VpStatusType
(*VpTestLineCallbackType) (
    struct VpLineCtxType *pLineCtx,
    VpPcmOperationResultsType *pResults);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
typedef VpStatusType
(*VpCodeCheckSumFuncPtrType) (
    struct VpDevCtxType *pDevCtx,
    uint16 handle);

typedef VpStatusType
(*VpSelfTestFuncPtrType) (
    struct VpLineCtxType *pLineCtx);

typedef VpStatusType
(*VpFillTestBufFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData);

typedef VpStatusType
(*VpReadTestBufFuncPtrType) (
    struct VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData);
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
         * || (defined(VP_CC_880_SERIES) && defined (VP880_INCLUDE_TESTLINE_CODE)) \
         * || (defined(VP_CC_890_SERIES) && defined (VP890_INCLUDE_TESTLINE_CODE))
         */

/** ApiFunctionPointer Table to be used by Device Context */
typedef struct ApiFunctions {
    /* System Configuration Functions */
    VpMakeLineObjectFuncPtrType MakeLineObject;

    /* Initialization Functions */
    /*   -- Init Functions common to all devices and VP-API-II configuration modes */
    VpInitDeviceFuncPtrType InitDevice;
    VpFreeRunFuncPtrType FreeRun;

    VpInitLineFuncPtrType InitLine;
    VpConfigLineFuncPtrType ConfigLine;

    VpCalLineFuncPtrType CalLine;
    VpCalFuncPtrType Cal;
    VpInitProfileFuncPtrType InitProfile;

    /*   -- Init Functions NOT provided in CSLAC VP-API-II */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
    VpBootLoadFuncPtrType BootLoad;
    VpBootSlacFuncPtrType BootSlac;
    VpInitSlacFuncPtrType InitSlac;
    VpSoftResetFuncPtrType SoftReset;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

    /*   -- Init Functions NOT provided in CSLAC VP-API-II except VE790 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
    VpSetBFilterFuncPtrType SetBFilter;
    VpSetBatteriesFuncPtrType SetBatteries;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
         */


#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES) \
                                || (defined(VP_CC_880_SERIES) && defined(VP_ENABLE_PROD_TEST))
    VpCalCodecFuncPtrType CalCodec;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
         */

    /*   -- Init Functions NOT provided in CSLAC VP-API-II except VE580 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
    VpInitCustomTermTypeFuncPtrType InitCustomTerm;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CC_580_SERIES)
         */

    /*   -- Init Functions NOT provided in CSLAC VP-API-II Lite Version */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(VP_CSLAC_SEQ_EN)
    VpInitRingFuncPtrType InitRing;
    VpInitCidFuncPtrType InitCid;
    VpInitMeterFuncPtrType InitMeter;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP) || defined(VP_CSLAC_SEQ_EN)
         */

    /* Control Functions */
    /*   -- Control Functions common to all devices and VP-API-II configuration modes */
    VpSetLineStateFuncPtrType SetLineState;
    VpSetLineToneFuncPtrType SetLineTone;
    VpSetRelayStateFuncPtrType SetRelayState;
    VpSetRelGainFuncPtrType SetRelGain;

    VpSetOptionFuncPtrType SetOption;
    VpDeviceIoAccessFuncPtrType DeviceIoAccess;
    VpVirtualISRFuncPtrType VirtualISR;
    VpApiTickFuncPtrType ApiTick;

    /*   -- Control Functions NOT provided in CSLAC VP-API-II */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
    VpSetCalRelayStateFuncPtrType SetCalRelayState;

    VpStartMeter32QFuncPtrType StartMeter32Q;
    VpGenTimerCtrlFuncPtrType GenTimerCtrl;
    VpAssocDslLineFuncPtrType AssocDslLine;
    VpSetSealCurFuncPtrType SetSealCur;

    VpDeviceIoAccessExtFuncPtrType DeviceIoAccessExt;
    VpLineIoAccessFuncPtrType LineIoAccess;

    VpLowLevelCmd16FuncPtrType LowLevelCmd16;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP)
         */

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP) || defined(ZARLINK_CFG_INTERNAL)
    VpLowLevelCmdFuncPtrType LowLevelCmd;
#endif

    /*   -- Control Functions NOT provided in CSLAC VP-API-II Lite Version */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)  || defined(VP_CSLAC_SEQ_EN)
    VpSendSignalFuncPtrType SendSignal;
    VpSendCidFuncPtrType SendCid;
    VpContinueCidFuncPtrType ContinueCid;
    VpStartMeterFuncPtrType StartMeter;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
         * || defined(VP_CC_KWRAP)  || defined(VP_CSLAC_SEQ_EN)
         */

    /* Status and Query Functions */
    /*   -- Status and Query Functions common to all devices and VP-API-II configuration modes */
    VpGetEventFuncPtrType GetEvent;
    VpGetLineStatusFuncPtrType GetLineStatus;
    VpGetDeviceStatusFuncPtrType GetDeviceStatus;

    VpGetOptionFuncPtrType GetOption;
    VpGetLineStateFuncPtrType GetLineState;
    VpFlushEventsFuncPtrType FlushEvents;
    VpGetResultsFuncPtrType GetResults;

    VpObjectDumpFuncPtrType ObjectDump;
    VpRegisterDumpFuncPtrType RegisterDump;
    VpDtmfDigitDetectedFuncPtrType DtmfDigitDetected;

    /*   -- Status and Query Functions used only for CSLAC Line Testing */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
                                || (defined(VP880_INCLUDE_TESTLINE_CODE) && defined(VP_CC_880_SERIES)) \
                                || (defined(VP890_INCLUDE_TESTLINE_CODE) && defined(VP_CC_890_SERIES))
    VpGetRelayStateFuncPtrType GetRelayState;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
         * || (defined(VP880_INCLUDE_TESTLINE_CODE) && defined(VP_CC_880_SERIES)) \
         * || (defined(VP890_INCLUDE_TESTLINE_CODE) && defined(VP_CC_890_SERIES))
         */

    /*   -- Status and Query Functions NOT provided in CSLAC VP-API-II */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
    VpGetDeviceStatusExtFuncPtrType GetDeviceStatusExt;
    VpClearResultsFuncPtrType ClearResults;
    VpQueryFuncPtrType Query;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES)  || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

    /*   -- Status and Query Functions NOT provided in CSLAC VP-API-II except VE790 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
    VpGetLoopCondFuncPtrType GetLoopCond;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
         */

    /* Test Functions */
    /*   -- All test functions removed for CSLAC VP-API-II unless Line Test is included */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
                                || (defined(VP_CC_880_SERIES) && defined (VP880_INCLUDE_TESTLINE_CODE)) \
                                || (defined(VP_CC_890_SERIES) && defined (VP890_INCLUDE_TESTLINE_CODE))
    /*   -- Test Functions common to all devices and VP-API-II configuration modes */
    VpTestLineFuncPtrType TestLine;
    VpTestLineIntFuncPtrType TestLineInt;
    VpTestLineCallbackType TestLineCallback;

    /*   -- Test Functions NOT provided in CSLAC VP-API-II */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
                                 || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
    VpCodeCheckSumFuncPtrType CodeCheckSum;
    VpFillTestBufFuncPtrType FillTestBuf;
    VpReadTestBufFuncPtrType ReadTestBuf;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP)
         */

    /*   -- Test Functions NOT provided in CSLAC VP-API-II except VE790 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
            || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
    VpSelfTestFuncPtrType SelfTest;
#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         *  || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) || defined(VP_CC_790_SERIES)
         */

#endif  /*
         * !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES) \
         * || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) || defined(VP_CC_KWRAP) \
         * || (defined(VP_CC_880_SERIES) && defined (VP880_INCLUDE_TESTLINE_CODE)) \
         * || (defined(VP_CC_890_SERIES) && defined (VP890_INCLUDE_TESTLINE_CODE))
         */
} ApiFunctions;

/******************************************************************************
 *                       DEVICE/LINE CONTEXT DEFINITION                       *
 ******************************************************************************/
/** Voice Path Line Context type */
typedef struct VpLineCtxType {
    struct VpDevCtxType *pDevCtx;   /**< Pointer back Device Context */

    void *pLineObj;                 /**< Pointer (forward) to Line Object */
} VpLineCtxType;

/** Voice Path Device Context type */
typedef struct VpDevCtxType {
    VpDeviceType deviceType;    /**< What type is the device context (enum) */
    void *pDevObj;              /**< Pointer to device object */

    ApiFunctions funPtrsToApiFuncs; /**< Pointers to API functions */

    /**< Pointers to each line context associated with this device.  Set size
     * to max allowable per device in system
     */
    VpLineCtxType *pLineCtx[VP_MAX_LINES_PER_DEVICE];
} VpDevCtxType;

/******************************************************************************
 *                        VP-API-II Definitions that use context              *
 ******************************************************************************/
/** Event struct: Type reported by VpGetEvent(). */
typedef struct VpEventType {
    VpStatusType status;        /**< Function return status */

    uint8 channelId;            /**< Channel that caused the event */

    VpLineCtxType *pLineCtx;    /**< Pointer to the line context (corresponding
                                 *   to the channel that caused the event) */

    VpDeviceIdType deviceId;    /**< device chip select ID corresponding to the
                                 *   device that caused the event */

    VpDevCtxType *pDevCtx;      /**< Pointer to the device context
                                 *   (corresponding to the device that caused
                                 *   the event) */

    VpEventCategoryType eventCategory; /**< Event category.  The event catagory
                                        *   is necessary because there are more
                                        *   events than can be specified by the
                                        *   size of "eventId  */

    uint16 eventId;     /**< The event that occurred.  Requires that the event
                         *   catagory be known to interpret */

    uint16 parmHandle;  /**< Event�s Parameter or Host Handle.  This value is
                         *   specified by the application only, not used by the
                         *   API */

    uint16 eventData;   /**< Data associated with the event. Event Id specific*/

    bool hasResults;    /**< If TRUE indicates this event has results associated
                         *   with it */

    VpLineIdType lineId;    /**< Application provide line Id to ease mapping of
                             * lines to specific line contexts.
                             */
} VpEventType;

/**< The following types are for VpGetDeviceInfo() and VpGetLineInfo()
 * functions */

typedef enum VpFeatureType {
    VP_UNKNOWN,
    VP_AVAILABLE,
    VP_NOT_AVAILABLE
} VpFeatureType;

typedef struct VpFeatureListType {
    VpFeatureType testLoadSwitch;
    VpFeatureType internalTestTermination;
} VpFeatureListType;

typedef struct VpDeviceInfoType {
    VpLineCtxType *pLineCtx;            /* Pointer to Line Context */
    VpDeviceIdType deviceId;            /* Device identity */
    VpDevCtxType *pDevCtx;              /* Pointer to device Context */
    VpDeviceType deviceType;            /* Device Type */
    VpFeatureListType featureList;      /* Store the device features */
    uint8 numLines;                     /* Number of lines */
    uint8 revCode;                      /* Revision Code Number */
    uint8 slacId;                       /* Unique ID associated with Device identity */
    uint16 productCode;                 /* Number indicating features of the device.*/
} VpDeviceInfoType;

typedef struct VpLineInfoType {
    VpDevCtxType *pDevCtx;       /* Pointer to device Context */
    uint8 channelId;             /* Channel identity */
    VpLineCtxType *pLineCtx;     /* Pointer to Line Context */
    VpTermType termType;         /* Termination Type */
    VpLineIdType lineId;         /* Application system wide line identifier */
} VpLineInfoType;

/******************************************************************************
 *                        VP-API-II FUNCTION PROTOTYPES                       *
 ******************************************************************************/
/*
 * System configuration functions
 */
EXTERN VpStatusType
VpMakeDeviceObject(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    VpDevCtxType *pDevCtx,
    void *pDevObj);

EXTERN VpStatusType
VpMakeDeviceCtx(
    VpDeviceType deviceType,
    VpDevCtxType *pDevCtx,
    void *pDevObj);

EXTERN VpStatusType
VpMakeLineObject(
    VpTermType termType,
    uint8 channelId,
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpMakeLineCtx(
    VpLineCtxType *pLineCtx,
    void *pLineObj,
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpFreeLineCtx(
    VpLineCtxType *pLineCtx);

EXTERN VpStatusType
VpGetDeviceInfo(
    VpDeviceInfoType *pDeviceInfo);

EXTERN VpStatusType
VpGetLineInfo(
    VpLineInfoType *pLineInfo);

/*
 * Initialization functions
 */
EXTERN VpStatusType
VpBootLoad(
    VpDevCtxType *pDevCtx,
    VpBootStateType state,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize,
    VpScratchMemType *pScratchMem,
    VpBootModeType validation);

EXTERN VpStatusType
VpBootSlac(
    VpLineCtxType *pLineCtx,
    VpImagePtrType pImageBuffer,
    uint32 bufferSize);

EXTERN VpStatusType
VpInitDevice(
    VpDevCtxType *pDevCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile,
    VpProfilePtrType pFxoAcProfile,
    VpProfilePtrType pFxoCfgProfile);

EXTERN VpStatusType
VpFreeRun(
    VpDevCtxType *pDevCtx,
    VpFreeRunModeType freeRunMode);

EXTERN VpStatusType
VpInitSlac(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pDevProfile,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcProfile,
    VpProfilePtrType pRingProfile);

EXTERN VpStatusType
VpInitLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

EXTERN VpStatusType
VpConfigLine(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pAcProfile,
    VpProfilePtrType pDcFeedOrFxoCfgProfile,
    VpProfilePtrType pRingProfile);

EXTERN VpStatusType
VpCalCodec(
    VpLineCtxType *pLineCtx,
    VpDeviceCalType mode);

EXTERN VpStatusType
VpCalLine(
    VpLineCtxType *pLineCtx);

EXTERN VpStatusType
VpCal(
    VpLineCtxType *pLineCtx,
    VpCalType calType,
    void *inputArgs);

EXTERN VpStatusType
VpInitRing(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCadProfile,
    VpProfilePtrType pCidProfile);

EXTERN VpStatusType
VpInitCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

EXTERN VpStatusType
VpInitMeter(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pMeterProfile);

EXTERN VpStatusType
VpInitCustomTermType (
    VpDevCtxType *pDevCtx,
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pCustomTermProfile);

EXTERN VpStatusType
VpInitProfile(
    VpDevCtxType *pDevCtx,
    VpProfileType type,
    VpProfilePtrType pProfileIndex,
    VpProfilePtrType pProfile);

EXTERN VpStatusType
VpSoftReset(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpSetBatteries(
    VpLineCtxType *pLineCtx,
    VpBatteryModeType battMode,
    VpBatteryValuesType *pBatt);

EXTERN VpStatusType
VpSetBFilter(
    VpLineCtxType *pLineCtx,
    VpBFilterModeType bFiltMode,
    VpProfilePtrType pAcProfile);

/*
 * Control functions
 */
EXTERN VpStatusType
VpSetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType state);

EXTERN VpStatusType
VpSetLineTone(
    VpLineCtxType *pLineCtx,
    VpProfilePtrType pToneProfile,
    VpProfilePtrType pCadProfile,
    VpDtmfToneGenType *pDtmfControl);

EXTERN VpStatusType
VpSetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType rState);

EXTERN VpStatusType
VpSetCalRelayState(
    VpDevCtxType *pDevCtx,
    VpCalRelayControlType rState);

EXTERN VpStatusType
VpSetRelGain(
    VpLineCtxType *pLineCtx,
    uint16 txLevel,
    uint16 rxLevel,
    uint16 handle);

EXTERN VpStatusType
VpSendSignal(
    VpLineCtxType *pLineCtx,
    VpSendSignalType signalType,
    void *pSignalData);

EXTERN VpStatusType
VpSendCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    VpProfilePtrType pCidProfile,
    uint8p pCidData);

EXTERN VpStatusType
VpContinueCid(
    VpLineCtxType *pLineCtx,
    uint8 length,
    uint8p pCidData);

EXTERN VpStatusType
VpStartMeter(
    VpLineCtxType *pLineCtx,
    uint16 onTime,
    uint16 offTime,
    uint16 numMeters);

EXTERN VpStatusType
VpStartMeter32Q(
    VpLineCtxType *pLineCtx,
    uint32 minDelay,
    uint32 onTime,
    uint32 offTime,
    uint16 numMeters,
    uint16 eventRate);

EXTERN VpStatusType
VpAssocDslLine(
    VpLineCtxType *pLineCtx,
    bool connect,
    uint8 line);

EXTERN VpStatusType
VpSetSealCur(
    VpDevCtxType *pDevCtx,
    uint16 sealApplyTime,
    uint16 sealCycleTime,
    uint16 maxCurrent,
    uint16 minCurrent,
    uint16 *pSealArray,
    VpProfilePtrType pDcProfile);


#define VpStartMeter32(pLineCtx, minDelay, onTime, offTime, numMeters) \
    VpStartMeter32Q(pLineCtx, minDelay, onTime, offTime, numMeters, 1)

EXTERN VpStatusType
VpSetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    void *pValue);

EXTERN VpStatusType
VpDeviceIoAccess(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessDataType *pDeviceIoData);

VpStatusType
VpDeviceIoAccessExt(
    VpDevCtxType *pDevCtx,
    VpDeviceIoAccessExtType *pDeviceIoAccess);

EXTERN VpStatusType
VpLineIoAccess(
    VpLineCtxType *pLineCtx,
    VpLineIoAccessType *pLineIoAccess,
    uint16 handle);

EXTERN VpStatusType
VpVirtualISR(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpApiTick(
    VpDevCtxType *pDevCtx,
    bool *pEventStatus);

EXTERN VpStatusType
VpLowLevelCmd(
    VpLineCtxType *pLineCtx,
    uint8 *pCmdData,
    uint8 len,
    uint16 handle);

EXTERN VpStatusType
VpLowLevelCmd16(
    VpLineCtxType *pLineCtx,
    VpLowLevelCmdType cmdType,
    uint16 *writeWords,
    uint8 numWriteWords,
    uint8 numReadWords,
    uint16 handle);

EXTERN VpStatusType
VpGenTimerCtrl(
    VpLineCtxType *pLineCtx,
    VpGenTimerCtrlType timerCtrl,
    uint32 duration,
    uint16 handle);

/*
 * Status and query functions
 */
EXTERN bool
VpGetEvent(
    VpDevCtxType *pDevCtx,
    VpEventType *pEvent);

EXTERN VpStatusType
VpGetLineStatus(
    VpLineCtxType *pLineCtx,
    VpInputType input,
    bool *pStatus);

EXTERN VpStatusType
VpGetDeviceStatus(
    VpDevCtxType *pDevCtx,
    VpInputType input,
    uint32 *pDeviceStatus);

EXTERN VpStatusType
VpGetDeviceStatusExt(
    VpDevCtxType *pDevCtx,
    VpDeviceStatusType *pDeviceStatus);

EXTERN VpStatusType
VpGetLoopCond(
    VpLineCtxType *pLineCtx,
    uint16 handle);

EXTERN VpStatusType
VpGetOption(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx,
    VpOptionIdType option,
    uint16 handle);

EXTERN VpStatusType
VpGetLineState(
    VpLineCtxType *pLineCtx,
    VpLineStateType *pCurrentState);

EXTERN VpStatusType
VpFlushEvents(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpGetResults(
    VpEventType *pEvent,
    void *pResults);

EXTERN VpStatusType
VpClearResults(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpObjectDump(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpRegisterDump(
    VpDevCtxType *pDevCtx);

EXTERN VpStatusType
VpDtmfDigitDetected(
    VpLineCtxType *pLineCtx,
    VpDigitType digit,
    VpDigitSenseType sense);

EXTERN VpStatusType
VpQuery(
    VpLineCtxType *pLineCtx,
    VpQueryIdType queryId,
    uint16 handle);

EXTERN VpStatusType
VpGetRelayState(
    VpLineCtxType *pLineCtx,
    VpRelayControlType *pRstate);

/*
 * Test Functions
 */
EXTERN VpStatusType
VpTestLine(
    VpLineCtxType *pLineCtx,
    VpTestIdType test,
    const void *pArgs,
    uint16 handle);

EXTERN VpStatusType
VpTestLineCallback(
    VpLineCtxType *pLineCtx,
    VpPcmOperationResultsType *pResults);

EXTERN VpStatusType
VpCodeCheckSum(
    VpDevCtxType *pDevCtx,
    uint16 handle);

EXTERN VpStatusType
VpSelfTest(
    VpLineCtxType *pLineCtx);

EXTERN VpStatusType
VpFillTestBuf(
    VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData);

EXTERN VpStatusType
VpReadTestBuf(
    VpLineCtxType *pLineCtx,
    uint16 length,
    VpVectorPtrType pData);


EXTERN VpStatusType
VpMapLineId(
    VpLineCtxType *pLineCtx,
    VpLineIdType lineId);

#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
EXTERN VpStatusType
VpMapSlacId(
    VpDevCtxType *pDevCtx,
    uint8 slacId);
#endif

EXTERN bool
VpMemCpyCheck(
    uint8 *dest,
    uint8 *src,
    uint16 count);

EXTERN void *
VpMemCpy(
    void *dest,
    const void *src,
    uint16 count);

EXTERN void *
VpMemSet(
    void * s,
    int c,
    uint16 count);


/*
 * Undocumented external functions:
 */
#if !defined(VP_REDUCED_API_IF) || defined(VP_CC_792_SERIES) || defined(VP_CC_VCP_SERIES)   \
                                || defined(VP_CC_VCP2_SERIES) || defined(VP_CC_MELT_SERIES) \
                                || defined(VP_CC_KWRAP)
EXTERN void
VpSetCalFlag(
    VpLineCtxType *pLineCtx,
    bool value);

EXTERN bool
VpReadCalFlag(
    VpLineCtxType *pLineCtx);

EXTERN VpStatusType
Vcp2HbiCheck(
    VpDevCtxType *pDevCtx);
#endif

EXTERN void
VpMpiCmdWrapper(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 mpiCmd,
    uint8 mpiCmdLen,
    uint8 *dataBuffer);

EXTERN uint8
VpCSLACBuildMpiBuffer(
    uint8 index,
    uint8 *mpiBuffer,
    uint8 mpiCmd,
    uint8 mpiCmdLen,
    uint8 *mpiData);
#endif /* VP_API_COMMON_H */
