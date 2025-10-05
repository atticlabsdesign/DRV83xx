#ifndef DRV8305_H
#define DRV8305_H

#include <stdint.h>
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


typedef enum drv8305Addr{
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


typedef struct drv8305Settings{
    uint16_t highGateCtrl;      //should these have their own types/substructs?
    uint16_t lowGateCtrl;
    uint16_t gateCtrl;
    uint16_t icCtrl;
    uint16_t shntCtrl;
    uint16_t vregCtrl;
    uint16_t vdsCtrl;
    
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

#endif