/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

/** @file
 *
 * ex_sss_boot.h:  *The purpose and scope of this file*
 *
 * Project:  SecureIoTMW-Debug@appboot-top-eclipse_x86
 *
 * $Date: Mar 10, 2019 $
 * $Author: ing05193 $
 * $Revision$
 */

#ifndef SSS_EX_INC_EX_SSS_BOOT_H_
#define SSS_EX_INC_EX_SSS_BOOT_H_

/* *****************************************************************************************************************
 *   Includes
 * ***************************************************************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif

#include "ex_sss.h"
#include "fsl_sss_api.h"

#if SSS_HAVE_APPLET_SE05X_IOT
#include "fsl_sss_se05x_types.h"
#endif
#include "ex_sss_ports.h"
#include "nxScp03_Types.h"

/* *****************************************************************************************************************
 * MACROS/Defines
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 * Types/Structure Declarations
 * ***************************************************************************************************************** */
#if SSS_HAVE_APPLET || SSS_HAVE_APPLET_SE05X_IOT

typedef union ex_auth {
    struct
    {
        NXSCP03_StaticCtx_t ex_static; //!< .static keys data
        NXSCP03_DynCtx_t ex_dyn;       //!<  session keys data
    } scp03;
    struct
    {
        NXECKey03_StaticCtx_t ex_static; //!< .static keys data
        NXSCP03_DynCtx_t ex_dyn;         //!<  session keys data
    } eckey;
    struct
    {
        sss_object_t ex_id;
    } id;
} ex_SE05x_authCtx_t;
#endif

typedef struct
{
    sss_session_t session;
    sss_key_store_t ks;

#if SSS_HAVE_HOSTCRYPTO_ANY || SSS_HAVE_SSCP
    sss_session_t host_session;
#endif

#if SSS_HAVE_HOSTCRYPTO_ANY
    sss_key_store_t host_ks;
#endif

#if SSS_HAVE_APPLET_SE05X_IOT || SSS_HAVE_APPLET_LOOPBACK
    SE_Connect_Ctx_t se05x_open_ctx;
    sss_tunnel_t *pTunnel_ctx;
    ex_SE05x_authCtx_t ex_se05x_auth;
#endif

#if SSS_HAVE_SSCP
    sscp_context_t sscp_ctx;
#endif

} ex_sss_boot_ctx_t;

#if SSS_HAVE_APPLET_SE05X_IOT
typedef struct
{
    sss_session_t platf_session;
    SE_Connect_Ctx_t platf_open_ctx;
    sss_session_t *phost_session;
    sss_key_store_t *phost_ks;
#if 1 //SSS_HAVE_HOSTCRYPTO_ANY
    /* Keeping this to be consistant on binary sizes */
    ex_SE05x_authCtx_t ex_se05x_auth;
#endif // SSS_HAVE_HOSTCRYPTO_ANY
} ex_sss_platf_ctx_t;
#endif

/* *****************************************************************************************************************
 *   Extern Variables
 * ***************************************************************************************************************** */

/* *****************************************************************************************************************
 *   Function Prototypes
 * ***************************************************************************************************************** */

#if SSS_HAVE_APPLET_SE05X_IOT

sss_status_t ex_sss_se05x_prepare_host(sss_session_t *host_session,
    sss_key_store_t *host_ks,
    SE05x_Connect_Ctx_t *se05x_open_ctx,
    ex_SE05x_authCtx_t *ex_se05x_authctx,
    SE_AuthType_t auth_type);

sss_status_t ex_sss_se05x_prepare_host_with_key(sss_session_t *host_session,
    sss_key_store_t *host_ks,
    SE_Connect_Ctx_t *se05x_open_ctx,
    ex_SE05x_authCtx_t *se05x_auth_ctx,
    SE_AuthType_t auth_type,
    uint8_t *authKey,
    size_t authKeyLen);

/* Prepare host for multiple user sessions */
sss_status_t ex_sss_se05x_prepare_host_keys(sss_session_t *pHostSession,
    sss_key_store_t *pHostKs,
    SE_Connect_Ctx_t *pConnectCtx,
    ex_SE05x_authCtx_t *se05x_auth_ctx,
    uint32_t offset);
#endif

#if SSS_HAVE_APPLET
sss_status_t ex_sss_se_prepare_host(sss_session_t *host_session,
    sss_key_store_t *host_ks,
    SE_Connect_Ctx_t *se05x_open_ctx,
    ex_SE05x_authCtx_t *ex_se05x_authctx,
    SE_AuthType_t auth_type);
#endif



/** The case where we connect to the cyrptogrpahic system in-directly.
 *
 *
 * This function expects that the last argument in argv is the
 * expected/probable port name.
 *
 * @param argc count of parameters, as received by main
 * @param argv Array of argv, as received by main
 * @param[out] pPortName Possible port name
 * @return 0 if successful.
 */
sss_status_t ex_sss_boot_connectstring(int argc, const char *argv[], char **pPortName);

/** Is this a serail port */
bool ex_sss_boot_isSerialPortName(const char *portName);

/** Is this --help request */
bool ex_sss_boot_isHelp(const char *argname);

/** Is this a socket port */
bool ex_sss_boot_isSocketPortName(const char *portName);

/** Open an example session */
sss_status_t ex_sss_boot_open(ex_sss_boot_ctx_t *pCtx, const char *portName);

/** Open an example cc session */
sss_status_t ex_sss_boot_open_on_id(ex_sss_boot_ctx_t *pCtx, const char *portName, const int32_t authId);

/** Open an example session */
sss_status_t ex_sss_boot_factory_reset(ex_sss_boot_ctx_t *pCtx);

/** Close an example session */
void ex_sss_session_close(ex_sss_boot_ctx_t *pCtx);

/** Entry Point for each example */
sss_status_t ex_sss_entry(ex_sss_boot_ctx_t *pCtx);

#define ex_sss_kestore_and_object_init ex_sss_key_store_and_object_init

sss_status_t ex_sss_key_store_and_object_init(ex_sss_boot_ctx_t *pCtx);

#if SSS_HAVE_HOSTCRYPTO_ANY
sss_status_t ex_sss_boot_open_host_session(ex_sss_boot_ctx_t *pCtx);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* SSS_EX_INC_EX_SSS_BOOT_H_ */
