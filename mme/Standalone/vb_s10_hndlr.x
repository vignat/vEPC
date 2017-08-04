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

     Name:    vb_mme_s10_hndlr.x

     Desc:    Trillium LTE CNE - S10 Interface Handler functions

******************************************************************************/

#ifndef _VB_MME_S10_HNDLR_X_
#define _VB_MME_S10_HNDLR_X_

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/**************************************************************************//**
 *
 * @brief
 *    Handles Handover Preparation Message received from eNodeB
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  tgtMmeIpAddr
 *    Pointer to Target MME's IP Address
 *
 * @return
 *    SUCCESS if Forward Relocation Request is send to MME successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndFRReq
(
VbMmeUeCb      *pUeCb,
CmTptAddr      *pTgtMmeIpAddr
);


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Request received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Request
 *
 * @return
 *    SUCCESS if Forward Relocation Request is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRReq
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);



/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Response received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Response
 *
 * @return
 *    SUCCESS if Forward Relocation Response is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRRsp
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);


/**************************************************************************//**
 *
 * @brief
 *    Builds and Sends a forward relocation response to MME
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which S1 Handover is being handled
 * @param[in]  cause
 *    GTP Cause Value to be sent to MME
 *
 * @return
 *    SUCCESS if Forward Relocation Response is send to MME successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndFRRsp
(
VbMmeUeCb      *pUeCb,
U32            cause
);


/**************************************************************************//**
 *
 * @brief
 *    Sends Forward Relocation Complete notification message to MME
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Notification is sen3 to MME
 *    successfully otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndFRCmplNotif
(
VbMmeUeCb      *pUeCb
);


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Complete Notification
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Complete
 *    Notification
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Notification is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRCmplNotif
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Builds and Sends a forward relocation Compelete Ack to MME
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which S1 Handover is being handled
 * @param[in]  cause
 *    GTP Cause Value to be sent to MME
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Ack is send to MME successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndFRCmplAck
(
VbMmeUeCb      *pUeCb,
U32            cause
);


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Complete Acknowledge
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Complete
 *    Acknowledge
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Acknowledge is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRCmplAck
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Response received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Response
 *
 * @return
 *    SUCCESS if Forward Relocation Response is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRRsp
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Complete Acknowledge
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Complete
 *    Acknowledge
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Acknowledge is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRCmplAck
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Request received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Request
 *
 * @return
 *    SUCCESS if Forward Relocation Request is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRReq
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Complete Notification
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Relocation Complete
 *    Notification
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Notification is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFRCmplNotif
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Sends Foward Access context notification
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which eNB Status Tft received
 * @param[in]  tgtMmeIpAddr
 *    Pointer to Target MME's IP Address
 *
 * @return
 *    SUCCESS if Forward Access Notification is send to MME successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndFACtxNotif
(
VbMmeUeCb      *pUeCb,
CmTptAddr      *pTgtMmeIpAddr
);

/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Access Context Acknowledge
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Access Context
 *    Acknowledge
 *
 * @return
 *    SUCCESS if Forward Acceess Context Acknowledge is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFACtxAck
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Access Context Notification
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Forward Access Context
 *    Notification
 *
 * @return
 *    SUCCESS if Forward Access Context Notification is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncFACtxNotif
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Builds and Sends a forward Access Context Ack to MME
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which S1 Handover is being handled
 * @param[in]  cause
 *    GTP Cause Value to be sent to MME
 *
 * @return
 *    SUCCESS if Forward Access Context Ack is send to MME successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndFACtxAck
(
VbMmeUeCb      *pUeCb,
U32            cause
);

/**************************************************************************//**
 *
 * @brief
 *    Sends Relocation Cancel Request message to MME
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover Cancel is received
 *
 * @return
 *    SUCCESS if Relocation Cancel Request is sent to MME
 *    successfully otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndRCReq
(
VbMmeUeCb      *pUeCb
);

/**************************************************************************//**
 *
 * @brief
 *    Handles Relocation Cancel Request
 *    received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Relocation Release Request
 *
 * @return
 *    SUCCESS if Relocation Release Request is
 *    handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncRCReq
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Sends Relocation Cancel Response message to MME
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover Cancel is received
 * @param[in]  cause
 *    Cause value to be sent in Relocation Cancel Response
 *
 * @return
 *    SUCCESS if Relocation Cancel Response is sent to MME
 *    successfully otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndRCRsp
(
VbMmeUeCb      *pUeCb,
U32            cause
);

/**************************************************************************//**
 *
 * @brief
 *    Handles Relocation Cancel Response received from MME over S10 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S10 tunnel
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  pEgMsg
 *    Pointer to EgMsg structure containing Relocation Cancel Response
 *
 * @return
 *    SUCCESS if Relocation Cancel Response is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeIncRCRsp
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
);

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
#endif
