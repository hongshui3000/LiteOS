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

#if defined(WITH_AT_FRAMEWORK) && defined(USE_NB_NEUL95)
#include "los_nb_api.h"
#include "at_api_interface.h"
#include "atiny_socket.h"

int32_t nb_data_ioctl(void* arg,int8_t * buf, int32_t len)
{
    if (NULL == buf || len <= 0)
    {
        AT_LOG("param invailed!");
        return -1;
    }
    AT_LOG("cmd in:%s",buf);
	return 0;
}

int los_nb_init(const int8_t* host, const int8_t* port, sec_param_s* psk)
{
    int ret;
    int timecnt = 0;
    at.init();

    nb_reboot();
    LOS_TaskDelay(1000);
    if(psk != NULL)//encryption v1.9
    {
        nb_send_psk(psk->pskid, psk->psk);
    }

    while(1)
    {
        ret = nb_hw_detect();
        if(ret != AT_FAILED)
            break;
        LOS_TaskDelay(1000);
    }
    //nb_get_auto_connect();
    //nb_connect(NULL, NULL, NULL);

	while(timecnt < 120)
	{
		ret = nb_get_netstat();
		//nb_check_csq();
		if(ret != AT_FAILED)
		{
			ret = nb_query_ip();
			break;
		}
		LOS_TaskDelay(1000);
		timecnt++;
	}
	if(ret != AT_FAILED)
	{
		ret = nb_query_ip();
	}
	nb_set_cdpserver((char *)host, (char *)port);
    return ret;
}

int los_nb_report(const char* buf, int len)
{
    return nb_send_payload(buf, len);
}

int los_nb_notify(oob_callback callback)
{
    return at.oob_register(AT_CMD_PREFIX,strlen(AT_CMD_PREFIX), callback);
}

int los_nb_deinit(void)
{
    return nb_reboot();;
}

#endif
