/*----------------------------------------------------------------------------
 * Copyright (c) <2016-2018>, <Huawei Technologies Co., Ltd>
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 * Notice of Export Control Law
 * ===============================================
 * Huawei LiteOS may be subject to applicable export control laws and regulations, which might
 * include those applicable to Huawei LiteOS of U.S. and the country in which you are located.
 * Import, export and usage of Huawei LiteOS in any manner by you shall be in compliance with such
 * applicable export control laws and regulations.
 *---------------------------------------------------------------------------*/
#include "main.h"
#include "sys_init.h"
#include "agent_tiny_demo.h"
#if defined WITH_AT_FRAMEWORK
#include "at_api_interface.h"
#if defined USE_NB_NEUL95
#include "los_nb_api.h"
#endif
#endif
UINT32 g_TskHandle;

VOID HardWare_Init(VOID)
{
    SystemClock_Config();
    Debug_USART1_UART_Init();
    hal_rng_config();
    dwt_delay_init(SystemCoreClock);
}

extern int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len);

VOID main_task(VOID)
{
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    hieth_hw_init();
    net_init();
#elif defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#if 0
    sec_param_s sec;
    sec.pskid = "868744031130978";
    sec.psk = "47019975ce03358b32ed25ef5c14b560";
#endif
#define lijingqian 1
#if lijingqian
    printf("\r\n=====================================================");
    printf("\r\nSTEP1: Report Data to Server( NB Init )");
    printf("\r\n=====================================================\r\n");
    los_nb_init((const int8_t*)"218.4.33.71",(const int8_t*)"5683",NULL);
    printf("\r\n=====================================================");
    printf("\r\nSTEP2: Report Data to Server( NB Notify )");
    printf("\r\n=====================================================\r\n");
    los_nb_notify("+NNMI:",strlen("+NNMI:"),nb_data_ioctl);
    osDelay(3000);
    printf("\r\n=====================================================");
    printf("\r\nSTEP3: Report Data to Server( NB Report )");
    printf("\r\n=====================================================\r\n");
    los_nb_report("22", 2);
    los_nb_report("23", 2);
#else
    extern int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len);
    los_nb_init((const int8_t*)"218.4.33.71",(const int8_t*)"5683",NULL);
    los_nb_notify("+NNMI:",strlen("+NNMI:"),nb_data_ioctl);
	osDelay(3000);
	ret = los_nb_report("22", 2);
	printf("send:%d\n",ret);
	ret = los_nb_report("33", 2);
	printf("send:%d\n",ret);
    ret = los_nb_report("44", 1);
	printf("send:%d\n",ret);
    ret = los_nb_report("55", 2);
	printf("send:%d\n",ret);
    //los_nb_deinit();
#endif
#elif defined(WITH_AT_FRAMEWORK) && (defined(USE_ESP8266) || defined(USE_SIM900A))
    extern at_adaptor_api at_interface;
    at_api_register(&at_interface);
    agent_tiny_entry();
#endif
#if defined(WITH_LINUX) || defined(WITH_LWIP)
    agent_tiny_entry();
#endif
}
UINT32 creat_main_task()
{
    UINT32 uwRet = LOS_OK;
    TSK_INIT_PARAM_S task_init_param;

    task_init_param.usTaskPrio = 0;
    task_init_param.pcName = "main_task";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)main_task;
    task_init_param.uwStackSize = 0x1000;

    uwRet = LOS_TaskCreate(&g_TskHandle, &task_init_param);
    if(LOS_OK != uwRet)
    {
        return uwRet;
    }
    return uwRet;
}

int main(void)
{
    UINT32 uwRet = LOS_OK;
    HardWare_Init();

    uwRet = LOS_KernelInit();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    uwRet = creat_main_task();
    if (uwRet != LOS_OK)
    {
        return LOS_NOK;
    }

    (void)LOS_Start();
    return 0;
}
