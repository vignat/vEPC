/*
 * Copyright 2017-present Open Networking Foundation
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

     Name:    vb_mme_s3_utl.x

     Desc:    Trillium LTE CNE - S3 Interface Utility functions

******************************************************************************/

#ifndef _VB_S3_UTL_X_
#define _VB_S3_UTL_X_

#ifdef __cplusplus
EXTERN "C"
{
#endif
/**************************************************************************//**
 *
 * @brief
 *    Creates S3 Tunnel CB, Adds it into the hash table
 *    The local Tunnel ID will be given the egt layer if Forward Relocation
 *    Request is received from other MME
 *    If MME is sending the Forward Relocation Request the local teid
 *    will be given by EGT when Forward Relocation Response is received
 *
 * @param[in]  pUeCb
 *    Pointer to UE Cb
 * @param[in]  tgtMmeIpAddr
 *    Pointer to Target MME's IP Address
 * @param[in]  transId
 *    Transaction ID
 *
 * @return
 *    SUCCESS if S3 C-Tunnel is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlCreateS3Tun
(
VbMmeUeCb      *pUeCb,
CmTptAddr      *pTgtMmeIpAddr,
U32            transId
);


/**************************************************************************//**
 *
 * @brief
 *    Creates S3 Tunnel CB, Adds it into the hash table
 *    and the assigns the Local Teid.
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if S3 C-Tunnel is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldS3FRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppFwdLocReq
);


/**************************************************************************//**
 *
 * @brief
 *    Returns new transaction ID on S3 interface
 *
 * @return
 *    New transaction ID
 *
 *****************************************************************************/
PUBLIC U32 vbMmeUtlGetS3TransId(Void);


/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Relocation Complete Notification message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  cause
 *    GTP Cause Value to be sent to MME
 * @param[out] ppEgMsg
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Notification is
 *    Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldS3FRCmplNotif
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppEgMsg
);


/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Request message
 *    Creates and Stores ESM CBs (bearers) and UE releated information
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure with Forward Relocation Request
 *
 * @return
 *    SUCCESS if S3 C-Tunnel is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreS3FRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFwdRelocReq
);

/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Relocation Response message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  cause
 *    GTP Cause Value to be sent to MME
 * @param[out] ppFwdLocRsp
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Relocation Response is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldS3FRRsp
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppFwdLocRsp
);


/**************************************************************************//**
 *
 * @brief
 *    Deletes S3 Tunnel CB, Removes from Hash List
 *
 * @param[in]  pUeCb
 *    Pointer to UE CB
 *
 * @return
 *    SUCCESS if S3 C-Tunnel Deleted successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlDelS3Tun
(
VbMmeUeCb      *pUeCb
);


/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Response message
 *    and stores relavant parameters received in the message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure with Forward Relocation Response
 *
 * @return
 *    SUCCESS Forward Relocation Response is validated successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreS3FRRsp
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFwdRelocRsp
);


/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Complete Notification message
 *    and stores relavant parameters received in the message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure with Forward Relocation
 *    Complete Notification
 *
 * @return
 *    SUCCESS Forward Relocation Complete Notification is validated
 *    successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreS3FwdRelocCmplNotif
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFwdRelocCmplNotif
);


/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Relocation Complete Ack message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Ack is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldS3FRCmplAck
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppFwdLocReq
);

/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Complete Acknowledge message
 *    and stores relavant parameters received in the message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure with Forward Relocation
 *    Complete Acknowledge
 *
 * @return
 *    SUCCESS Forward Relocation Complete Acknowledge is validated
 *    successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreS3FRCmplAck
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFwdRelocCmplNotif
);

/**************************************************************************//**
 *
 * @brief
 *    Finds s3 tunnel cb, in Pending tunnel hash and Final Tunnel hash
 *
 * @param[in]  Teid
 *    S3 Tunnel ID
 * @param[in]  transId
 *    Transaction ID
 * @param[out] ppS3Tun
 *    Pointer to s3 tunnel
 *
 * @return
 *    SUCCESS if tunnel found in hash table otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlFndS3Tun
(
Teid              teid,
U32               transId,
VbMmeUeTunnelCb   **ppS3Tun
);


/**************************************************************************//**
 *
 * @brief
 *    Get Target eNB ID from the GTP Message
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message structure
 * @param[out] pEnbId
 *    Pointer to Global enode ID
 *
 * @return
 *    SUCCESS if Global enb id is retrieved successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlGetS3TgtEnbId
(
EgMsg          *pEgMsg,
VbMmeGlbEnbId  *pEnbId
);


#ifdef VB_MME_AUTH
/**************************************************************************//**
 *
 * @brief
 *    Adds and UE MM Context IE into GTP Message
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to Ue CB
 *
 * @return
 *    SUCCESS if IE if IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlAddUeS3MmCtx
(
EgMsg          *pEgMsg,
VbMmeUeCb      *pUeCb
);
#endif


#ifdef __cplusplus
}
#endif
#endif

/******************************************************************************

                                 END OF FILE

******************************************************************************/
