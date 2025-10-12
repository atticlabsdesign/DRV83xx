#include "../drv8305.h"
#include <stdbool.h>
#include <stdint.h>

extern void SPI_BufferExchange(void * bufferData, size_t bufferSize); //could probably replace this with a function pointer in the struct which the programmer defines with the settings, the functions would all then take the struct and use the function

/**
 * @brief Reads a single SPI register of a DRV8305 Motor Controller
 * 
 * @param addr Register address to be read
 * @param data Data read from register
 * @return drvError_t 
 */
drvError_t drv8305RegRead(drv8305Addr_t addr, uint16_t *data){
    bool timeout = false;
    
    uint16_t buffer = (1<<15) | (addr << 11);

    #if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        buffer = (buffer << 8 ) | (buffer >> 8); //swap bytes before sending this might be needed after too but i seem to recall the read value being fine... strange
    #endif

    SPI_BufferExchange(&buffer, sizeof(buffer));

    #if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        buffer = (buffer << 8 ) | (buffer >> 8); 
    #endif
    
    *data = (buffer & 0x3ff);
    
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
drvError_t drv8305RegWrite(drv8305Addr_t addr, uint16_t *data){
    bool timeout = false;
    
    uint16_t buffer = (1<<15) | (addr << 11) | (*data & 0x3ff);

    #if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        buffer = (buffer << 8 ) | (buffer >> 8); 
    #endif

    SPI_BufferExchange(&buffer, sizeof(buffer));
    *data = (buffer & 0x3ff); //data now cointains the overwritten values, is this even useful?

    if (timeout) {
        return DRV_NO_RESP;
    } else {
        return DRV_OK;
    }
    
}


drvError_t drv8305GetSettings(drv8305Settings_t *settings){
    drvError_t error = DRV_OK;
    
    error = drv8305RegRead(HSGATECTRL, &settings->highGateCtrl);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(LSGATECTRL, &settings->lowGateCtrl);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(GATECTRL, &settings->gateCtrl);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(ICCTRL, &settings->icCtrl);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(SHNTCTRL, &settings->shntCtrl);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(VREGCTRL, &settings->vregCtrl);
    if (error) {
        return error;
    }
    
    error = drv8305RegRead(VDSCNTRL, &settings->vdsCtrl);

    return error;
}

drvError_t drv8305SetSettings(drv8305Settings_t *settings){
    drvError_t error = DRV_OK;

    uint16_t buffer; //so we dont overwrite the values in the struct

    buffer = settings->highGateCtrl.bits;
    error = drv8305RegWrite(HSGATECTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->lowGateCtrl.bits; 
    error = drv8305RegWrite(LSGATECTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->gateCtrl.bits;
    error = drv8305RegWrite(GATECTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->icCtrl.bits; 
    error = drv8305RegWrite(ICCTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->shntCtrl.bits;
    error = drv8305RegWrite(SHNTCTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->vregCtrl.bits; 
    error = drv8305RegWrite(VREGCTRL, &buffer);
    if (error) {
        return error;
    }

    buffer = settings->vdsCtrl.bits; 
    error = drv8305RegWrite(VDSCNTRL, &buffer);

    return error;
}
