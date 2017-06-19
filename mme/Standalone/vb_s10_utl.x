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

     Name:    vb_mme_s10_utl.x

     Desc:    Trillium LTE CNE - S10 Interface Utility functions

******************************************************************************/

#ifndef _VB_S10_UTL_X_
#define _VB_S10_UTL_X_

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/**************************************************************************//**
 *
 * @brief
 *    Creates S10 Tunnel CB, Adds it into the hash table
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
 *    SUCCESS if S10 C-Tunnel is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlCreateS10Tun
(
VbMmeUeCb      *pUeCb,
CmTptAddr      *pTgtMmeIpAddr,
U32            transId
);


/**************************************************************************//**
 *
 * @brief
 *    Creates S10 Tunnel CB, Adds it into the hash table
 *    and the assigns the Local Teid.
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if S10 C-Tunnel is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldFRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppFwdLocReq
);


/**************************************************************************//**
 *
 * @brief
 *    Returns new transaction ID on S10 interface
 *
 * @return
 *    New transaction ID
 *
 *****************************************************************************/
PUBLIC U32 vbMmeUtlGetS10TransId(Void);


/**************************************************************************//**
 *
 * @brief
 *    Allocates memory for UE Control block and adds it into Hash list
 *
 * @param[in]  ppUeCb
 *    Pointer to address of UE Control Block
 *    Pointer to EgMsg structure containing Forward Relocation Response
 *
 * @return
 *    SUCCESS if UE cb is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlCreateUeCb
(
VbMmeUeCb     **ppUeCb
);


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
PUBLIC S16 vbMmeUtlBldFRCmplNotif
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
 *    SUCCESS if S10 C-Tunnel is created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreFRReq
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
PUBLIC S16 vbMmeUtlBldFRRsp
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppFwdLocRsp
);


/**************************************************************************//**
 *
 * @brief
 *    Deletes S10 Tunnel CB, Removes from Hash List
 *
 * @param[in]  pUeCb
 *    Pointer to UE CB
 *
 * @return
 *    SUCCESS if S10 C-Tunnel Deleted successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlDelS10Tun
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
PUBLIC S16 vbMmeUtlValNStoreFRRsp
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
PUBLIC S16 vbMmeUtlValNStoreFwdRelocCmplNotif
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
PUBLIC S16 vbMmeUtlBldFRCmplAck
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
PUBLIC S16 vbMmeUtlValNStoreFRCmplAck
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFwdRelocCmplNotif
);

/**************************************************************************//**
 *
 * @brief
 *    Finds s10 tunnel cb, in Pending tunnel hash and Final Tunnel hash
 *
 * @param[in]  Teid
 *    S10 Tunnel ID
 * @param[in]  transId
 *    Transaction ID
 * @param[out] ppS10Tun
 *    Pointer to s10 tunnel
 *
 * @return
 *    SUCCESS if tunnel found in hash table otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlFndS10Tun
(
Teid              teid,
U32               transId,
VbMmeUeTunnelCb   **ppS10Tun
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
PUBLIC S16 vbMmeUtlGetTgtEnbId
(
EgMsg          *pEgMsg,
VbMmeGlbEnbId  *pEnbId
);


/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Access Context Notification message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppEgMsg
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Notification is
 *    Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldFACtxNotif
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Access COntext Ack message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  cause
 *    Cause value to be sent in GTP message
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Access COntext Ack is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldFACtxAck
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Access Context Notification message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure with Forward Access
 *    Context Notification
 *
 * @return
 *    SUCCESS Forward Acceess Context Notification is validated
 *    successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreFACtxNotif
(
VbMmeUeCb      *pUeCb,
EgMsg          *pEgMsg
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
EXTERN S16 vbMmeEgtUtlAddUeMmCtx
(
EgMsg          *pEgMsg,
VbMmeUeCb      *pUeCb
);
#endif

/**************************************************************************//**
 *
 * @brief
 *    Creates Relocation Cancel Request message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Relocation Cancel Request is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldRCReq
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppRelCanReq
);

/**************************************************************************//**
 *
 * @brief
 *    Build Relocation Cancel Response
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  cause
 *    GTP Cause Value to be sent to MME
 * @param[out] ppRCRsp
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Relocation Response is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldRCRsp
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppRCRsp
);

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
#endif

/******************************************************************************

                                 END OF FILE

******************************************************************************/
