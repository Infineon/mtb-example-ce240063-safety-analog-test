/******************************************************************************
* File Name:   self_test.c
*
* Description:This file provides the helper functions for executing self tests
*             for  PSoC6 MCU and XMC7000 MCU.
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

#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "self_test.h"


/*******************************************************************************
* Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Name: adc_test
********************************************************************************
* Summary:
* This function performs self test on the ADC block by  measuring the voltage on
* a specific channel and comparing it against the expected result within a defined
* accuracy range, this test validates the accuracy and functionality of the ADC in
* converting analog signals to digital values. The test is performed on two reference
* voltages VDD/3) and (2VDD/3), which is configured using the macro ADC_REF_VOLTAGE2.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void adc_test(void)
{
#if ADC_REF_VOLTAGE2
    printf("Ensure that a (2VDDA / 3) signal is connected to ADC channel 0.\r\n");
    if (OK_STATUS != SelfTests_ADC(CYBSP_DUT_SAR_ADC_HW, 0x00u, ANALOG_ADC_SAR_RESULT2,
            ANALOG_ADC_ACURACCY, VBG_CHANNEL, 1))
    {
        /* Process error */
        printf("Error: ADC SelfTest failed for 2VDD/3 signal.\r\n");
    }
    else
    {
        printf("SUCCESS: ADC SelfTest passed for 2VDD/3 signal.\r\n");
    }

#else
    printf("Ensure that a (VDDA / 3) signal is connected to ADC channel 0.\r\n");

    if (OK_STATUS != SelfTests_ADC(CYBSP_DUT_SAR_ADC_HW, 0x00u, ANALOG_ADC_SAR_RESULT1,
            ANALOG_ADC_ACURACCY, VBG_CHANNEL, 1))
    {
        /* Process error */
        printf("Error: ADC SelfTest failed for VDD/3 signal.\r\n");
    }
    else
    {
        printf("SUCCESS: ADC SelfTest passed for VDD/3 signal.\r\n");
    }
#endif
}

#if COMPONENT_CAT1A
/*******************************************************************************
* Function Name: comparator_test
********************************************************************************
* Summary:
* This function performs self test on the comparator block by verifying if the
* comparator output aligns with the expected result. The comparator is connected
* to GPIO pins, thus allowing selection of two voltage references on AMUXBUS A
* and AMUXBUS B.
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void comparator_test(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    /*Initialize the LPCOMP with device configurator generated structure*/
    Cy_LPComp_Init(CYBSP_DUT_LPCOMP_HW, CYBSP_DUT_LPCOMP_CHANNEL ,
            &CYBSP_DUT_LPCOMP_config);
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }
    Cy_LPComp_Enable(CYBSP_DUT_LPCOMP_HW, CYBSP_DUT_LPCOMP_CHANNEL);

    /* Apply lower voltage to positive input */
    printf("Apply lower voltage to positive input (CYBSP_DUT_LPCOMP_VPLUS_PIN).\r\n");

    Cy_GPIO_Pin_FastInit(CYBSP_DUT_LPCOMP_VPLUS_PORT, CYBSP_DUT_LPCOMP_VPLUS_PIN,
            CY_GPIO_DM_ANALOG, 0u, HSIOM_SEL_AMUXB);
    Cy_GPIO_Pin_FastInit(CYBSP_DUT_LPCOMP_VMINUS_PORT, CYBSP_DUT_LPCOMP_VMINUS_PIN,
            CY_GPIO_DM_ANALOG, 0u, HSIOM_SEL_AMUXA);

    if(OK_STATUS != SelfTests_Comparator(CYBSP_DUT_LPCOMP_HW, CYBSP_DUT_LPCOMP_CHANNEL,
            ANALOG_COMP_RESULT2))
    {
        /* Process error */
        printf("Error: LPCOMP lower voltage test fail\r\n");
    }
    else
    {
        printf("SUCCESS: LPCOMP lower voltage test\r\n");
    }


    /* Apply higher voltage to positive input */
    Cy_GPIO_Pin_FastInit(CYBSP_DUT_LPCOMP_VPLUS_PORT, CYBSP_DUT_LPCOMP_VPLUS_PIN,
            CY_GPIO_DM_ANALOG, 0u, HSIOM_SEL_AMUXA);
    Cy_GPIO_Pin_FastInit(CYBSP_DUT_LPCOMP_VMINUS_PORT, CYBSP_DUT_LPCOMP_VMINUS_PIN,
            CY_GPIO_DM_ANALOG, 0u, HSIOM_SEL_AMUXB);

    if(OK_STATUS != SelfTests_Comparator(CYBSP_DUT_LPCOMP_HW, CYBSP_DUT_LPCOMP_CHANNEL,
            ANALOG_COMP_RESULT1))
    {
        /* Process error */
        printf("Error: LPCOMP higher voltage test fail\r\n");
    }
    else
    {
        printf("SUCCESS: LPCOMP higher voltage test\r\n");
    }

}
#endif

#if (defined(CY_DEVICE_PSOC6A256K) || defined(CY_DEVICE_PSOC6ABLE2))
/*******************************************************************************
* Function Name: opamp_test
********************************************************************************
* Summary:
* This function performs self test on the op-amp block by comparing the measured
* voltage against the anticipated outcome within a defined accuracy range, this
* test ensures that the opamp operates correctly and generates the expected output
* voltage. The opamp is connected to the ADC and utilizes GPIO pins as a
* multiplexer to choose various voltage references on AMUXBUS A and AMUXBUS B.
* The test is performed on two reference voltages VDD/3) and (2VDD/3), which is
* configured using the macro ADC_REF_VOLTAGE2.
* .
*
* Parameters:
*  none
*
* Return :
*  void
*
*******************************************************************************/
void opamp_test(void)
{
    cy_rslt_t result = CY_RSLT_SUCCESS;

    cy_stc_ctb_config_t CYBSP_DUT_CTB_config = {

        /* Opamp0 configuration */
        .oa0Power = CYBSP_DUT_OPAMP_config.oaPower,
        .oa0Mode  = CYBSP_DUT_OPAMP_config.oaMode,
        .oa0Pump = CYBSP_DUT_OPAMP_config.oaPump,
        .oa0CompEdge  = CYBSP_DUT_OPAMP_config.oaCompEdge,
        .oa0CompLevel = CYBSP_DUT_OPAMP_config.oaCompLevel,
        .oa0CompBypass  = CYBSP_DUT_OPAMP_config.oaCompBypass,
        .oa0CompHyst = CYBSP_DUT_OPAMP_config.oaCompHyst,
        .oa0CompIntrEn  = CYBSP_DUT_OPAMP_config.oaCompIntrEn,
    };

    /*Initialize the OPAMP0 with device configurator generated structure*/
     result = Cy_CTB_Init(CYBSP_DUT_OPAMP_HW, &CYBSP_DUT_CTB_config);
     if (result != CY_CTB_SUCCESS)
     {
         CY_ASSERT(0);
     }
      /*Enable Opamp0*/
     Cy_CTB_Enable(CYBSP_DUT_OPAMP_HW);

#if ADC_REF_VOLTAGE2
     /* Connect the (2VDDA / 3) signal to opamp channel.
      * Use the voltage divider to achieve the (2VDDA / 3) signal.
      */
     printf("Ensure that a (2VDDA / 3) signal is connected to ADC channel 0.\r\n");
     if(OK_STATUS != SelfTests_Opamp(CYBSP_DUT_SAR_ADC_HW, ANALOG_OPAMP_SAR_RESULT2,
             ANALOG_OPAMP_ACURACCY, 0x01, 1))
     {
         /* Process error */
         printf("Error: OPAMP test failed for 2VDD/3 signal.\r\n");
     }
     else
     {
         printf("SUCCESS: OPAMP test passed for 2VDD/3 signal.\r\n");
     }

#else
     /* Connect the (VDDA / 3) signal to OpAMP channel.
      * Use the voltage divider to achieve the (VDDA / 3) signal.
      */
     printf("Ensure that a (VDDA / 3) signal is connected to ADC channel 0.\r\n");
     if(OK_STATUS != SelfTests_Opamp(CYBSP_DUT_SAR_ADC_HW, ANALOG_OPAMP_SAR_RESULT1,
             ANALOG_OPAMP_ACURACCY, 0x01, 1))
     {
         /* Process error */
         printf("Error: OPAMP test failed for VDD/3 signal.\r\n");
     }
     else
     {
         printf("SUCCESS: OPAMP test passed for VDD/3 signal.\r\n");
     }

#endif


}
#endif

/* [] END OF FILE */

