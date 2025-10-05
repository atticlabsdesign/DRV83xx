/**
 * @file main.c
 * @author joe
 * @date 2025-09-27
 * @brief Main function
 */
// #include "mcc_generated_files/system/system.h"
#include "config_1/mcc/mcc_generated_files/system/pins.h"
#include "config_1/mcc/mcc_generated_files/system/system.h"

#include <avr/builtins.h>
#include <stdint.h>
#include <string.h>

#include "include/drv83xx/drv8305.h"

#define MAX_COMMAND_LEN         (8U)
#define LINEFEED_CHAR           ((uint8_t)'\n')
#define CARRIAGERETURN_CHAR     ((uint8_t)'\r')

static char     command[MAX_COMMAND_LEN];
static uint8_t  index = 0;
static uint8_t  readMessage;

void UART_ExecuteCommand(char *command);
void UART_ProcessCommand(void);


void UART_ExecuteCommand(char *command)
{
    if(strcmp(command, "ON") == 0)
    {
        uint16_t buff = 0;
        // (void)printf("sending via SPI: 0x%x\r\n", buff);
        DRV_CS_SetLow();
        drv8305RegRead(HSGATECTRL, &buff);
        DRV_CS_SetHigh();
        (void)printf("recieved via SPI: 0x%x\r\n", buff);

        LED_SetHigh();
        (void)printf("OK, LED ON.\r\n");
    }
    else if (strcmp(command, "OFF") == 0)
    {
        LED_SetLow();
        (void)printf("OK, LED OFF.\r\n");
    }
    else
    {
        (void)printf("Incorrect command.\r\n");
    }
}

void UART_ProcessCommand(void)
{
    if(UART.IsRxReady())
    {
        readMessage = UART.Read();
        if ( (readMessage != LINEFEED_CHAR) && (readMessage != CARRIAGERETURN_CHAR) ) 
        {
            command[index++] = readMessage;
            if (index > MAX_COMMAND_LEN) 
            {
                (index) = 0;
            }
        }
    
           if (readMessage == CARRIAGERETURN_CHAR) 
           {
                command[index] = '\0';
                index = 0;
                UART_ExecuteCommand(command);
            }
    }
}

int main(void)
{
    SYSTEM_Initialize();
    DRV_CS_SetHigh();
    SPI.Open(0);
    
    (void)printf("In the terminal, send 'ON' to turn the LED on, and 'OFF' to turn it off.\r\n");
    (void)printf("Note: commands 'ON' and 'OFF' are case sensitive.\r\n");
    
    while(1)
    {
        UART_ProcessCommand();

    }
}
