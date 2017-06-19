/*
 * Copyright 2017-present Open Networking Laboratory
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/******************************************************************************

     Name:    Trillium LTE SGW Application

     Type:    C file

     Desc:    SGW Egt Utility functions

******************************************************************************/

#ifndef _QO_EGT_UTL_X_
#define _QO_EGT_UTL_X_

/**************************************************************************//**
 *
 * @brief 
 *    Copies IE from source GTP message to destination GTP message
 *             
 * @param[inout]  pDstMsg
 *    Pointer to GTP message where the IE shall be copied (destination)
 * @param[in]  pCsReq
 *    Pointer to GTP message where the IE shall be copied from (source)
 * @param[in]  ieType
 *    Type of the IE to be copied
 * @param[in]  ieInst
 *    Instance onf GTP IE to be copied
 * @param[in]  ieDataType
 *    Type of IE Data
 * 
 * @return
 *    SUCCESS if IE is copied successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 qoSgwEgtUtlCopyIe
(
EgMsg          *pDstMsg,
EgMsg          *pSrcMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType
);


/**************************************************************************//**
 *
 * @brief 
 *    Builds Modify Bearer Request.
 *    The IEs encoded are with respect to S1 Handover with SGW relocation.
 *    Only default bearer is handled
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[out]  ppMbReq
 *    Pointer to GTP message structure containing Modify Bearer Request
 *    received from MME
 * 
 * @return
 *    SUCCESS if Modify Bearer Request created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 qoSgwBldMBReq
(
QoSgwPdnCb     *pdnCb,
EgMsg          **ppMbReq
);


/**************************************************************************//**
 *
 * @brief 
 *    Adds an IE inside Bearer Context IE
 *             
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into Bearer Context IE
 * @param[in]  ieInst
 *    Instance of IE to be added into Bearer Context IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into Bearer Context IE
 * @param[in]  pEgIe 
 *    Pointer to ie details data structure
 * 
 * @return
 *    SUCCESS if IE is added into Bearer Context IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 qoSgwEgtUtlAddBCtxIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
);

/**************************************************************************//**
 *
 * @brief
 *    Finds Default Bearer belonging to a PDN session
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which default bearer has to be found
 *    handled
 * 
 * @return
 *    returns the Bearer CB of the default bearer
 *    If not found returns NULL
 *
 *****************************************************************************/
EXTERN QoSgwBearerCb* qoSgwFndDefBearer
(
QoSgwPdnCb        *pPdnCb
);


/**************************************************************************//**
 *
 * @brief 
 *    Builds Create Session Response. Allocates memory for GTP message
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[out]  pCsRsp
 *    Pointer to GTP message structure containing Create Session Response
 * 
 * @return
 *    SUCCESS if Create Session Response created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 qoSgwBldHOCSRsp
(
QoSgwPdnCb     *pPdnCb,
EgMsg          **pCsRsp
);

/***************************************************************
 *
 *       Fun:   qoSgwBldDSRsp
 *
 *       Desc:  Builds Modify Bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
PUBLIC S16 qoSgwBldDSRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg      /* Modify Bearer Response received from MME */
);

#endif
