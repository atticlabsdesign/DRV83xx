#ifndef DRV8305_H
#define DRV8305_H

#include <stdint.h>
#include <stdbool.h>
#include "drv83xx_types.h"

// Masks for SPI registers


/********************
---------------------
|Warnings & Watchdog|
---------------------
********************/

#define DRV8305_WWR_OTW_MASK    0x001
#define DRV8305_WWR_TEMP_MASK   0x10E
#define DRV8305_WWR_VCHP_MASK   0x010
#define DRV8305_WWR_VDS_MASK    0x020
#define DRV8305_WWR_VDDO_MASK   0x040
#define DRV8305_WWR_VDDU_MASK   0x080
#define DRV8305_WWR_FAULT_MASK  0x400

/********************
---------------------
|   OV/VDS Faults   |
---------------------
********************/
#define DRV8305_VDSF_VDS        0x3F0
#define DRV8305_VDSF_SOCP       0x007

/********************
---------------------
|     IC Faults     |
---------------------
********************/
#define DRV8305_ICF_VCPH        0x007
#define DRV8305_IC_UVLO         0x070
#define DRV8305_IC_WD           0x200
#define DRV8305_IC_VDD_UVLO     0x200


typedef enum {
    WARNING     = 0x1,
    VDSFAULT    = 0x2,
    ICFAULT     = 0x3,
    VGSFAULT    = 0x4,
    HSGATECTRL  = 0x5,
    LSGATECTRL  = 0x6,
    GATECTRL    = 0x7,
    ICCTRL      = 0x9,
    SHNTCTRL    = 0xA,
    VREGCTRL    = 0xB,
    VDSCNTRL    = 0xC
} drv8305Addr_t;

/*________________________________________________*/

typedef enum {

    PKTSRC_220NS,
    PKTSRC_440NS,
    PKTSRC_880NS,
    PKTSRC_1780NS,

} drv8305TDrive_t;

typedef enum {
    PKISNK_20MA,
    PKISNK_30MA,
    PKISNK_40MA,
    PKISNK_50MA,
    PKISNK_60MA,
    PKISNK_70MA,
    PKISNK_80MA,
    PKISNK_250MA,
    PKISNK_500MA,
    PKISNK_750MA,
    PKISNK_1000MA,
    PKISNK_1250MA,
} drv8305IDriveN_t;

typedef enum {
    PKISRC_10MA,
    PKISRC_20MA,
    PKISRC_30MA,
    PKISRC_40MA,
    PKISRC_50MA,
    PKISRC_60MA,
    PKISRC_70MA,
    PKISRC_125MA,
    PKISRC_250MA,
    PKISRC_500MA,
    PKISRC_750MA,
    PKISRC_1000MA,
} drv8305IDrivep_t;


#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ //flip the struct order so that the bits member remains readable on the host device

typedef union {
    struct { // 0x5 and 0x6
        drv8305IDrivep_t   IDRIVEP : 4;
        drv8305IDriveN_t   IDRIVEN : 4;
        drv8305TDrive_t    TDRIVEN : 2;
        int                        : 6; //RSVD

    };

    uint16_t bits;

} drv8305_XS_GATE_DRIVE_CONTROL_t;

#else

typedef union { 
    struct { // 0x5 and 0x6
        int                        : 6; //RSVD
        drv8305TDrive_t    TDRIVEN : 2;
        drv8305IDriveN_t   IDRIVEN : 4;
        drv8305IDrivep_t   IDRIVEP : 4;

    };

    uint16_t bits;

} drv8305_XS_GATE_DRIVE_CONTROL_t;

#endif


/*________________________________________________*/

typedef enum {
    DIODE,
    ACTIVE
} drv8305CommOption_t;

typedef enum {
    INPUT_6,
    INPUT_3,
    INPUT_1
} drv8305PwmMode_t;

typedef enum {
    DEADTIME_35NS,
    DEADTIME_52NS,
    DEADTIME_88NS,
    DEADTIME_440NS,
    DEADTIME_880NS,
    DEADTIME_1760NS,
    DEADTIME_3520NS,
    DEADTIME_5280NS,
} drv8305DeadTime_t;


typedef enum {
    VDSBLNK_0US,
    VDSBLNK_1_75US,
    VDSBLNK_3_5US,
    VDSBLNK_7US,
} drv8305TBlank_t;

typedef enum {
    VDSGLTCH_0US,
    VDSGLTCH_1_75US,
    VDSGLTCH_3_5US,
    VDSGLTCH_7US,
} drv8305TVds_t;


#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union{
    struct { //0x7
        drv8305TVds_t       TVDS        : 2;
        drv8305TBlank_t     TBLANK      : 2;
        drv8305DeadTime_t   DEAD_TIME   : 3;
        drv8305PwmMode_t    PWM_MODE    : 2;
        drv8305CommOption_t COMM_OPTION : 1;
        int                             : 6; //RSVD
    };

    uint16_t bits;
} drv8305_GATE_DRIVE_CONTROL_t; 

#else

typedef union{
    struct { //0x7
        int                             : 6; //RSVD
        drv8305CommOption_t COMM_OPTION : 1;
        drv8305PwmMode_t    PWM_MODE    : 2;
        drv8305DeadTime_t   DEAD_TIME   : 3;
        drv8305TBlank_t     TBLANK      : 2;
        drv8305TVds_t       TVDS        : 2;
    };

    uint16_t bits;
} drv8305_GATE_DRIVE_CONTROL_t; 

#endif

/*________________________________________________*/

typedef enum {
    WD_10MS,
    WD_20MS,
    WD_50MS,
    WD_100MS
} drv8305WdDelay_t;

typedef enum {
    VCPH_UV_4V9,
    VCPH_UV_4V6
} drv8305VCPH_t;

#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union{
    struct {  //0x9
        drv8305VCPH_t       SET_VCPH_UV     : 1;
        bool                CLR_FLTS           ;
        bool                SLEEP              ;
        bool                WD_EN              ;
        bool                DIS_SNS_OCP        ;
        drv8305WdDelay_t    WD_DLY          : 2;        
        bool                EN_SNS_CLAMP       ;
        bool                DIS_GDRV_FAULT     ;
        bool                DIS_PVDD_UVLO2     ;
        bool                FLIP_OTSD          ;        
        int                                 : 5; //RSVD

    };

    uint16_t bits;
} drv8305_IC_OPERATION_t;

#else

typedef union{
    struct {  //0x9
        int                                 : 5; //RSVD
        bool                FLIP_OTSD          ;
        bool                DIS_PVDD_UVLO2     ;
        bool                DIS_GDRV_FAULT     ;
        bool                EN_SNS_CLAMP       ;
        drv8305WdDelay_t    WD_DLY          : 2;
        bool                DIS_SNS_OCP        ;  
        bool                WD_EN              ;
        bool                SLEEP              ;
        bool                CLR_FLTS           ;
        drv8305VCPH_t       SET_VCPH_UV     : 1;
    };

    uint16_t bits;
} drv8305_IC_OPERATION_t;

#endif
/*________________________________________________*/

typedef enum {
    CS_BLANK_0NS,
    CS_BLANK_500NS,
    CS_BLANK_2500NS,
    CS_BLANK_10000NS,
} drv8305CsBlank_t;

typedef enum {
    CSGAIN_10,
    CSGAIN_20,
    CSGAIN_40,
    CSGAIN_80,
} drv8305CsGain_t;

#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union {
    struct {  //0xA
        drv8305CsGain_t GAIN_CS1    : 2;
        drv8305CsGain_t GAIN_CS2    : 2;
        drv8305CsGain_t GAIN_CS3    : 2;
        drv8305CsBlank_t CS_BLANK   : 2;
        bool DC_CAL_CH1;
        bool DC_CAL_CH2;
        bool DC_CAL_CH3;
        int                         : 5; //RSVD
    };

    uint16_t bits;
} drv8305_SHUNT_AMP_CTRL_t;

#else

typedef union {
    struct {  //0xA
        int                         : 5; //RSVD
        bool DC_CAL_CH3;
        bool DC_CAL_CH2;
        bool DC_CAL_CH1;
        drv8305CsBlank_t CS_BLANK   : 2;
        drv8305CsGain_t GAIN_CS3    : 2;
        drv8305CsGain_t GAIN_CS2    : 2;
        drv8305CsGain_t GAIN_CS1    : 2;
    };

    uint16_t bits;
} drv8305_SHUNT_AMP_CTRL_t;

#endif
/*________________________________________________*/

typedef enum {
    SCALE_2 = 1,
    SCALE_4 = 2
} drv8305VrefScale_t;

typedef enum {
    SLEEPDLY_0us,
    SLEEPDLY_10us,
    SLEEPDLY_50us,
    SLEEPDLY_1000us
} drv8305SleepDly_t;

typedef enum {
    VREGUV_9,
    VREGUV_8,
    VREGUV_7
} drv8305VregUv_t;


#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union {
    struct  {  //0xB
        drv8305VregUv_t     VREG_UV_LEVEL   : 2;
        bool                DIS_VREG_PWRGD     ; 
        drv8305SleepDly_t   SLEEP_DLY       : 2;
        int                                 : 3; //RSVD
        drv8305VrefScale_t  VREF_SCALE      : 2;
        int                                 : 6; //RSVD
    };

    uint16_t bits;
} drv8305_VOLAGE_REG_CTRL_t;

#else

typedef union {
    struct  {  //0xB
        int                                 : 6; //RSVD
        drv8305VrefScale_t  VREF_SCALE      : 2;
        int                                 : 3; //RSVD
        drv8305SleepDly_t   SLEEP_DLY       : 2;
        bool                DIS_VREG_PWRGD     ; 
        drv8305VregUv_t     VREG_UV_LEVEL   : 2;
    };

    uint16_t bits;
} drv8305_VOLAGE_REG_CTRL_t;

#endif


/*________________________________________________*/

typedef enum {
    VDSLVL_0V060,
    VDSLVL_0V068,
    VDSLVL_0V076,
    VDSLVL_0V086,
    VDSLVL_0V097,
    VDSLVL_0V109,
    VDSLVL_0V123,
    VDSLVL_0V138,
    VDSLVL_0V155,
    VDSLVL_0V175,
    VDSLVL_0V197,
    VDSLVL_0V222,
    VDSLVL_0V250,
    VDSLVL_0V282,
    VDSLVL_0V317,
    VDSLVL_0V358,
    VDSLVL_0V403,
    VDSLVL_0V454,
    VDSLVL_0V511,
    VDSLVL_0V576,
    VDSLVL_0V648,
    VDSLVL_0V730,
    VDSLVL_0V822,
    VDSLVL_0V926,
    VDSLVL_1V043,
    VDSLVL_1V175,
    VDSLVL_1V324,
    VDSLVL_1V491,
    VDSLVL_1V679,
    VDSLVL_1V892,
    VDSLVL_2V131    
} drv8305VdsLevel_t;

typedef enum {
    VDSPROTECTION_LATCH,
    VDSPROTECTION_REPORT,
    VDSPROTECTION_DISABLED,
} drv8305VdsMode_t;


#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union {
    struct  { //0xC
        drv8305VdsMode_t    VDS_MODE    : 3;
        drv8305VdsLevel_t   VDS_LEVEL   : 5;
        int                             : 8; //RSVD
    };

    uint16_t bits;
} drv8305_VDS_SENSE_CTRL_t;

#else

typedef union {
    struct  { //0xC
        int                             : 8; //RSVD
        drv8305VdsLevel_t   VDS_LEVEL   : 5;
        drv8305VdsMode_t    VDS_MODE    : 3;
    };

    uint16_t bits;
} drv8305_VDS_SENSE_CTRL_t;

#endif


/*________________________________________________*/

typedef struct {
    drv8305_XS_GATE_DRIVE_CONTROL_t highGateCtrl;      
    drv8305_XS_GATE_DRIVE_CONTROL_t lowGateCtrl;
    drv8305_GATE_DRIVE_CONTROL_t    gateCtrl;
    drv8305_IC_OPERATION_t          icCtrl;
    drv8305_SHUNT_AMP_CTRL_t        shntCtrl;
    drv8305_VOLAGE_REG_CTRL_t       vregCtrl;
    drv8305_VDS_SENSE_CTRL_t        vdsCtrl;
    
} drv8305Settings_t;

/********************
---------------------
|  Public Functions |
---------------------
********************/


drvError_t drv8305RegRead(drv8305Addr_t, uint16_t*);

drvError_t drv8305RegWrite(drv8305Addr_t, uint16_t*); // should this really be public?

drvError_t drv8305SetSettings(drv8305Settings_t *);

drvError_t drv8305GetSettings(drv8305Settings_t *);


//these functions are undercooked, give more time

drvError_t drv8305CW(drv8305Settings_t);

drvError_t drv8305CCW(drv8305Settings_t);

drvError_t drv8305Brake(drv8305Settings_t);

#endif