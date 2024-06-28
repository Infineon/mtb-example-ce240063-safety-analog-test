/******************************************************************************
* File Name:   self_test.h
*
* Description: This file is the public interface of self_test.c source file.
*
*
* Related Document: See README.md
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

/*******************************************************************************
* Include guard
*******************************************************************************/
#ifndef SELF_TEST_H_
#define SELF_TEST_H_

#include "SelfTest.h"

/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Macros
*******************************************************************************/
/* Available commands */
#define SELFTEST_CMD_ADC ('1')
#define SELFTEST_COMPARATOR ('2')
#define SELFTEST_CMD_OPAMP ('3')

/* in milliseconds */
#define ADC_REF_VOLTAGE2                   0

#define UART_TIMEOUT_MS (10u)      /* in milliseconds */

/* Channel no. where the VBG voltage is connected.*/
#if COMPONENT_CAT1A
    #define VBG_CHANNEL    (0u)
#elif COMPONENT_CAT1C
    #define VBG_CHANNEL    (1u)
#endif

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
void adc_test(void);

#if COMPONENT_CAT1A
void comparator_test(void);
#endif

#if (defined(CY_DEVICE_PSOC6A256K) || defined(CY_DEVICE_PSOC6ABLE2))
void opamp_test(void);
#endif

#endif /* SELF_TEST_H_ */

/* [] END OF FILE */
