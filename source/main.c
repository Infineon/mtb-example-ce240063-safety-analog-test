/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for Class-B safety Test for Analog
*              IP code example for ModusToolbox.
*
* Related Document: See README.md
*
*
*
*******************************************************************************
* Copyright 2024, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/    

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "self_test.h"


/*******************************************************************************
* Macros
*******************************************************************************/

/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function. It performs Class-B safety test for Analog block.
* SelfTest is performed for ADC, LPCOMP and Comparator based on the user 
* command.
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint8_t cmd;

#if defined (CY_DEVICE_SECURE)
    cyhal_wdt_t wdt_obj;

    /* Clear watchdog timer so that it doesn't trigger a reset */
    result = cyhal_wdt_init(&wdt_obj, cyhal_wdt_get_max_timeout_ms());
    CY_ASSERT(CY_RSLT_SUCCESS == result);
    cyhal_wdt_free(&wdt_obj);
#endif /* #if defined (CY_DEVICE_SECURE) */

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Enable global interrupts */
    __enable_irq();

    /* Initialize retarget-io to use the debug UART port */
    result = cy_retarget_io_init_fc(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX,
            CYBSP_DEBUG_UART_CTS,CYBSP_DEBUG_UART_RTS,CY_RETARGET_IO_BAUDRATE);

    /* retarget-io init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
#if COMPONENT_CAT1A
    /* Init AREF */
    result = Cy_SysAnalog_Init(&self_test_aref_0_config);
    if (CY_SAR_SUCCESS != result)
    {
        CY_ASSERT(0);
    }
    Cy_SysAnalog_Enable();

    /* Initialize SAR ADC */
    result = Cy_SAR_Init(CYBSP_DUT_SAR_ADC_HW, &CYBSP_DUT_SAR_ADC_config);
    if (CY_SAR_SUCCESS != result)
    {
        CY_ASSERT(0);
    }
    Cy_SAR_Enable(CYBSP_DUT_SAR_ADC_HW);

#elif COMPONENT_CAT1C
    /* Initialize the SAR2 module */
    Cy_SAR2_Init(CYBSP_DUT_SAR_ADC_HW, &CYBSP_DUT_SAR_ADC_config);
    /* Set ePASS MMIO reference buffer mode for bangap voltage */
    Cy_SAR2_SetReferenceBufferMode(PASS0_EPASS_MMIO, CY_SAR2_REF_BUF_MODE_ON);
#endif

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");

    printf("****************** "
           "Class-B: Analog IP SAFETY TEST "
           "****************** \r\n\n");

    /* Display available commands */
    printf("Available commands \r\n");
    printf("1 : Run SelfTest for ADC\r\n");
#if COMPONENT_CAT1A
    printf("2 : Run SelfTest for Comparator\r\n");
#endif
#if (defined(CY_DEVICE_PSOC6A256K) || defined(CY_DEVICE_PSOC6ABLE2))
    printf("3 : Run SelfTest for OP-AMP\r\n\n");
#endif
    for (;;)
    {
        /* Check for commands entered */
        result = cyhal_uart_getc(&cy_retarget_io_uart_obj, &cmd, UART_TIMEOUT_MS);
        if (result != CY_RSLT_ERR_CSP_UART_GETC_TIMEOUT)
        {
            if (SELFTEST_CMD_ADC == cmd)
            {
                printf("\r\n[Command] : Run SelfTest for ADC\r\n");
                adc_test();

            }
#if COMPONENT_CAT1A
            else if (SELFTEST_COMPARATOR == cmd)
            {
                printf("\r\n[Command] : Run SelfTest for Comparator\r\n");
                comparator_test();
            }
#endif
#if (defined(CY_DEVICE_PSOC6A256K) || defined(CY_DEVICE_PSOC6ABLE2))
            else if (SELFTEST_CMD_OPAMP == cmd)
            {
                printf("\r\n1[Command] : Run SelfTest for OP-AMP\r\n");
                opamp_test();

            }
#endif
        }
    }
}


/* [] END OF FILE */
