#include "../drv8305.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// extern void SPI_BufferExchange(void * bufferData, size_t bufferSize); //could probably replace this with a function pointer in the struct which the programmer defines with the settings, the functions would all then take the struct and use the function


/********************
---------------------
|  Private Functions |
---------------------
********************/
uint8_t drv8305StateMachine(bool);


/********************
---------------------
|Function Definitions|
---------------------
********************/



/**
 * @brief Reads a single SPI register of a DRV8305 Motor Controller
 * 
 * @param addr Register address to be read
 * @param data Data read from register
 * @return drvError_t 
 */
drvError_t drv8305RegRead(drv8305Comms_t *spi, drv8305Addr_t addr, uint16_t *data){
    bool timeout = false;
    
    pinId_t test = {.port = &(PORTB), .pin = 1};
    
    setPin(test, LOW);
    printf("%x\n\r",addr);

    uint16_t buffer = (1<<15) | (addr << 11);

    #if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        buffer = (buffer << 8 ) | (buffer >> 8); 
    #endif

    SPI.BufferExchange(&buffer, sizeof(buffer));

    #if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        buffer = (buffer << 8 ) | (buffer >> 8); 
    #endif
    
    *data = (buffer & 0x3ff);
    printf("data done: 0X%X \n\r", *data);

    setPin(test, HIGH);
    
    if (timeout) {
        return DRV_NO_RESP;
    } else {
        return DRV_OK;
    }
    
}

/**
 * @brief Writes to a single SPI register of a DRV8305 Motor Controller
 * 
 * @param addr Register address to be written to
 * @param data Data to be written to register, contains the overwritten value of the register after writing
 * @return drvError_t 
 */
drvError_t drv8305RegWrite(const drv8305Comms_t *spi, drv8305Addr_t addr, uint16_t *data){
    bool timeout = false;
    setPin(*spi->nCS, LOW);
    
    uint16_t buffer = (1<<15) | (addr << 11) | (*data & 0x3ff);

    #if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        buffer = (buffer << 8 ) | (buffer >> 8); 
    #endif

    spi->spiInterface->BufferExchange(&buffer, sizeof(buffer));
    *data = (buffer & 0x3ff); //data now cointains the overwritten values, is this even useful?

    setPin(*spi->nCS, HIGH);

    if (timeout) {
        return DRV_NO_RESP;
    } else {
        return DRV_OK;
    }
    
}


drvError_t drv8305GetSettings(drv8305Settings_t *settings){
    drvError_t error = DRV_OK;
    
    error = drv8305RegRead(settings->comms, HSGATECTRL, &settings->highGateCtrl.bits);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(settings->comms, LSGATECTRL, &settings->lowGateCtrl.bits);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(settings->comms, GATECTRL, &settings->gateCtrl.bits);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(settings->comms, ICCTRL, &settings->icCtrl.bits);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(settings->comms, SHNTCTRL, &settings->shntCtrl.bits);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(settings->comms, VREGCTRL, &settings->vregCtrl.bits);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(settings->comms, VDSCNTRL, &settings->vdsCtrl.bits);

    return error;
}

drvError_t drv8305SetSettings(drv8305Settings_t *settings){ //TODO: try to figure out a way to avoid unneccessarily writing to registers
    drvError_t error = DRV_OK;          

    uint16_t buffer; //so we dont overwrite the values in the struct

    buffer = settings->highGateCtrl.bits;
    error = drv8305RegWrite(settings->comms, HSGATECTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->lowGateCtrl.bits; 
    error = drv8305RegWrite(settings->comms, LSGATECTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->gateCtrl.bits;
    error = drv8305RegWrite(settings->comms, GATECTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->icCtrl.bits; 
    error = drv8305RegWrite(settings->comms, ICCTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->shntCtrl.bits;
    error = drv8305RegWrite(settings->comms, SHNTCTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->vregCtrl.bits; 
    error = drv8305RegWrite(settings->comms, VREGCTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->vdsCtrl.bits; 
    error = drv8305RegWrite(settings->comms, VDSCNTRL, &buffer);

    return error;

}

drvError_t drv8305DevInit(drv8305Dev_t *dev, const drv8305Comms_t spi){
    drvError_t error = DRV_OK;

    // dev->settings->comms->nCS = csPort;

    // printf("CS pin Set \n\r");

    // dev->settings->comms->spiInterface = spi;

    // printf("SPI Set \n\r");

    // error = drv8305GetSettings(dev->settings);

    return error;

}

drvError_t drv8305CW(drv8305Dev_t *dev){ //this needs to be called at the desired motor speed in Single PWM mode and should not be called if the motor is in the stop or align state
    drvError_t error = DRV_OK;
    uint8_t state = 0;

    switch (dev->settings->gateCtrl.PWM_MODE) {

        case INPUT_1:
            state = drv8305StateMachine(false);

            setPin(dev->pinCtrl->singlePwm.inla, (state & 1));
            state >>= 1;
            setPin(dev->pinCtrl->singlePwm.inlb, (state & 1));
            state >>= 1;
            setPin(dev->pinCtrl->singlePwm.inhb, (state & 1));
            state >>= 1;
            setPin(dev->pinCtrl->singlePwm.inla, (state & 1));

        break;

        case  INPUT_3:
            
        break;

        case  INPUT_6:
            
        break;
        
        default:
        error = DRV_UNKNOWN_SETTING;
    
    }

    return error;
}

drvError_t drv8305CCW(drv8305Dev_t *dev){ //this needs to be called at the desired motor speed in Single PWM mode and should not be called if the motor is in the stop or align state
    drvError_t error = DRV_OK;
    uint8_t state = 0;

    switch (dev->settings->gateCtrl.PWM_MODE) {

        case INPUT_1:
            state = drv8305StateMachine(true);

            setPin(dev->pinCtrl->singlePwm.dwell, (state & 1));
            state >>= 1;
            setPin(dev->pinCtrl->singlePwm.inlb, (state & 1));
            state >>= 1;
            setPin(dev->pinCtrl->singlePwm.inhb, (state & 1));
            state >>= 1;
            setPin(dev->pinCtrl->singlePwm.inla, (state & 1));

        break;

        case  INPUT_3:
            
        break;

        case  INPUT_6:
            
        break;
        
        default:
        error = DRV_UNKNOWN_SETTING;
    
    }

    return error;
}


drvError_t drv8305Brake(drv8305Dev_t *dev){
    drvError_t error = DRV_OK;
    switch (dev->settings->gateCtrl.PWM_MODE) {

        case INPUT_1:
            // all gpio low
            setPin(dev->pinCtrl->singlePwm.inla, false);
            setPin(dev->pinCtrl->singlePwm.inhb, false);
            setPin(dev->pinCtrl->singlePwm.inlb, false);
            setPin(dev->pinCtrl->singlePwm.dwell, false);
            
        break;

        case  INPUT_3:
            //pull all 3 channels low
            pwm8SetDutyCycle(dev->pinCtrl->triplePWM.pwm1,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->triplePWM.pwm1,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->triplePWM.pwm1,0, true);
        break;

        case  INPUT_6: //this is super scuffed
            pwm8SetDutyCycle(dev->pinCtrl->sixPWM.pwm1,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->sixPWM.pwm2,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->sixPWM.pwm3,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->sixPWM.pwm4,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->sixPWM.pwm5,0, true);
            pwm8SetDutyCycle(dev->pinCtrl->sixPWM.pwm6,0, true);
        break;
        
        default:
        error = DRV_UNKNOWN_SETTING;
    
    }

    return error;
}

drvError_t drv8305Align(drv8305Dev_t *dev){
    drvError_t error = DRV_OK;
    switch (dev->settings->gateCtrl.PWM_MODE) {

        case INPUT_1:
            // 1110
            setPin(dev->pinCtrl->singlePwm.inla, false);
            setPin(dev->pinCtrl->singlePwm.inhb, false);
            setPin(dev->pinCtrl->singlePwm.inlb, false);
            setPin(dev->pinCtrl->singlePwm.dwell, false);
        break;

        case  INPUT_3:
            //pwm first channel, low on other 2
        break;

        case  INPUT_6:
            //two pwm, 0101 on the rest
        break;
        
        default:
        error = DRV_UNKNOWN_SETTING;
    
    }

    return error;
}

uint8_t drv8305StateMachine(bool CCW){
    static uint8_t currentState = 0;

    uint8_t STATE_LUT[12] = {0x6, 0x5, 0x4, 0xD, 0xC, 0x9, 0x8, 0xB, 0xA, 0x3, 0x2, 0x7};
    
    if (currentState == 0 && !CCW) {
        currentState = 11;
    } else if (currentState == 11 && CCW) {
        currentState = 0;
    } else {
        currentState += CCW; 
    }

    return STATE_LUT[currentState];
}

#ifdef DRV8305_DEBUG_MENUS

#include "../../dbgcli/dbgcli.h"

char drv8305DbgReadOptions[2] = {'h','a'};

const dbgCliCommand_t drv8305DbgRead = {
    .keyword = {"read", "r"},
    .help = "read(aka: r) <addr> to read registers -a to read all",
    .options = drv8305DbgReadOptions,
};

char drv8305DbgWriteOptions[2] = {'h'};

const dbgCliCommand_t drv8305DbgWrite = {
    .keyword = {"write", "w"},
    .help = "read(aka: r) <addr> to read registers -a to read all",
    .options = drv8305DbgWriteOptions,
};

const dbgCliCommand_t drv8305DbgHelp = {
    .keyword = {"read", "r"},
    .help = "read(aka: r) <addr> to read registers -a to read all",
    .options = drv8305DbgWriteOptions,
};

dbgCliCommand_t drv8305DbgCommands[2] = {drv8305DbgRead,drv8305DbgWrite};

const submenu_t drv8305DbgMenu = {
    .description = "This is the DRV8305 motor controller driver Debug Menu type\n Help to see a list of commands or a command followed by -h\n for more info on that command",
    .commands = drv8305DbgCommands,
    .cmdNum = sizeof(drv8305DbgCommands),
};

// ccw <duration> to spin the motor counter clockwise

// cw <duration> to spin the motor counter clockwise

#endif
