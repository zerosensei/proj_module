/*
 * Copyright (c) 2022 zerosensei
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include "soc.h"
#include "config.h"
#include "HAL.h"
#include "debug/debug.h"

/*********************************************************************
 * GLOBAL TYPEDEFS
 */
__attribute__((aligned(4)))   u32 MEM_BUF[BLE_MEMHEAP_SIZE / 4];

const uint8_t MacAddr[6] = { 0x84, 0xC2, 0xE4, 0x13, 0x12, 0x42 };
extern int sys_clock_driver_init(void);
/*******************************************************************************
 * Function Name  : Main_Circulation
 * Description    : 主循环
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
__attribute__((section(".highcode")))
void Main_Circulation() 
{
    while (1) {
        TMOS_SystemProcess();
    }
}

int main()
{
#if (defined (DCDC_ENABLE)) && (DCDC_ENABLE == TRUE)
    PWR_DCDCCfg( ENABLE );
#endif
    SetSysClock(CLK_SOURCE_PLL_60MHz);

    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);
    GPIOB_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);

#ifdef DEBUG
    GPIOA_SetBits(bTXD1);
    GPIOA_ModeCfg(bTXD1, GPIO_ModeOut_PP_5mA);
    UART1_DefInit();
    DEBUG_Init();
#endif
    sys_clock_driver_init();



    CH58X_BLEInit( );
    HAL_Init();

    Main_Circulation();
}