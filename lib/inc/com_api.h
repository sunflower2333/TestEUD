/*************************************************************************
* 
* Copyright (c) 2025 Qualcomm Innovation Center, Inc. All rights reserved.
* SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
*
* File: 
*   com_api.h
*
* Description:                                                              
*   Header file for EUD COM Port public APIs declerations 
*
***************************************************************************/
#ifndef COM_API_H_
#define COM_API_H_

// #include "eud_api.h"
#include "com_eud.h"

#ifndef __cplusplus
typedef void ComEudDevice;
#endif

#define EUD_COM_EXEC_ENV_BASE 0x80

#define EUD_COM_EXEC_ENV_AOP      + EUD_COM_EXEC_ENV_BASE + 0x0
#define EUD_COM_EXEC_ENV_APPS     + EUD_COM_EXEC_ENV_BASE + 0x1
#define EUD_COM_EXEC_ENV_CDSP     + EUD_COM_EXEC_ENV_BASE + 0x2
#define EUD_COM_EXEC_ENV_RESERVED + EUD_COM_EXEC_ENV_BASE + 0x3
#define EUD_COM_EXEC_ENV_MPSS     + EUD_COM_EXEC_ENV_BASE + 0x4
#define EUD_COM_EXEC_ENV_ADSP     + EUD_COM_EXEC_ENV_BASE + 0x5
#define EUD_COM_EXEC_ENV_SLPI     + EUD_COM_EXEC_ENV_BASE + 0x6
#define EUD_COM_EXEC_ENV_SP       + EUD_COM_EXEC_ENV_BASE + 0x8
/**************************************//**
*   @brief Test routine
*
*******************************************/                        

EXPORT EUD_ERR_t test_function(uint32_t device_id, uint8_t exec_env_id);

#endif //COM_API_H_