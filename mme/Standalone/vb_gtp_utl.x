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

     Name:    vb_gtp_utl.x

     Desc:    Trillium LTE CNE - GTP Utility functions

******************************************************************************/

#ifndef _VB_GTP_UTL_X_
#define _VB_GTP_UTL_X_


/**************************************************************************//**
 *
 * @brief
 *    Retrieves the GTP IE of specific type and instance from EgMsg
 *
 * @param[in]  pEgMsg
 *    Pointer to gtp message containing the IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 * @param[in]  ieType
 *    Type of the GTP IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 *
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlGetIe
(
EgMsg   *pEgMsg,
EgIe    **ppEgIe,
U8      ieType,
U8      ieInst
);


/**************************************************************************//**
 *
 * @brief
 *    Converts 4 byte IP Address array into U32
 *
 * @param[in]  pAddrArr
 *    Pointer to address buffer array
 * @param[out] pAddr
 *    Pointer to u32 bit IP Address
 *
 * @return
 *    No return values
 *
 *****************************************************************************/
EXTERN Void vbMmeEgtUtlIpv4ArrToInet
(
U8             *pAddrArr,
U32            *pAddr
);


/**************************************************************************//**
 *
 * @brief
 *    Gets IE within PDN Connection IE
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 *
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlGetEpsPdnConnIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgIe           **ppEgIe
);


/**************************************************************************//**
 *
 * @brief
 *    Gets IE within Bearer Context ID within PDN Connection IE
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 *
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlGetBCtxIeInPdnConn
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgIe           **ppEgIe
);


/**************************************************************************//**
 *
 * @brief
 *    Retrieves the GTP IE of specific type and instance from EgMsg
 *
 * @param[in]  pEgMsg
 *    Pointer to gtp message containing the IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 * @param[in]  ieType
 *    Type of the GTP IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 *
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlGetIe
(
EgMsg   *pEgMsg,
EgIe    **ppEgIe,
U8      ieType,
U8      ieInst
);


/**************************************************************************//**
 *
 * @brief
 *    Adds and IE into GTP Message
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[in]  dataType
 *    Data type of the GTP IE
 * @param[out]  pEgIe
 *    Pointer to GTP IE to be added into GTP message
 *
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlAppendIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    dataType,
EgIe           *pEgIe
);



/**************************************************************************//**
 *
 * @brief
 *    Adds and EPC PDN Connection IE into GTP Message
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to Ue CB
 *
 * @return
 *    SUCCESS if PDN Connection IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlAddEpsPdn
(
EgMsg          *pEgMsg,
VbMmeUeCb      *pUeCb
);


/**************************************************************************//**
 *
 * @brief
 *    Adds and E-UTRAN Transparent Container IE into GTP Message
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to ETRAN Transparent container
 *
 * @return
 *    SUCCESS if E-UTRAN Transparent container IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/
EXTERN S16 vbMmeEgtUtlAddEUTransCont
(
EgMsg                *pEgMsg,
VbMmeEUTransCont     *pEuTransCont
);

/**************************************************************************//**
 *
 * @brief
 *    Adds and UTRAN Transparent Container IE into GTP Message
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to UTRAN Transparent container
 *
 * @return
 *    SUCCESS if UTRAN Transparent container IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/

EXTERN S16 vbMmeEgtUtlAddUtranTransCont
(
EgMsg             *pEgMsg,
VbMmeEUTransCont  *pEuTransCont
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
PUBLIC S16 vbMmeEgtUtlAddBCtxIe
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
 *    Adds an IE inside EPS PDN Connection
 *
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into EPS PDN Connection IE
 * @param[in]  ieInst
 *    Instance of IE to be added into EPS PDN Connection IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into EPS PDN Connection IE
 * @param[in]  pEgIe
 *    Pointer to ie details data structure
 *
 * @return
 *    SUCCESS if IE is added into EPS PDN Connection IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddEpsPdnConnIe
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
 *    Adds an IE inside Bearer Context IE in EPS PDN Connection
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
 *    SUCCESS if IE is added into EPS PDN Connection IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddEpsPdnConnBCtxIe
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
 *    Sends failure response to SGW
 *
 * @param[in]  locTeid
 *    Local S11 Teid, if available. if not available pass 0
 * @param[in]  remTeid
 *    Remove Tunnel Teid, if available. if not available pass 0
 * @param[in]  msgType
 *    Type Response message
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  cause
 *    Cause value to be sent in response
 *
 * @return
 *    SUCCESS if response message is sent successfully to SGW
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlSndFailureRsp
(
U32            locTeid,
U32            remTeid,
EgtMsgType     msgType,
U32            transId,
U32            cause
);

/**************************************************************************//**
 *
 * @brief
 *    Retrieves IMSI from GTP message to be store in 15 digit string
 *
 * @param[in]  imsi
 *    gtp imsi buffer
 * @param[in]  len
 *    gtp imsi buffer length
 * @param[inout]  ueImsi
 *    15 digit imsi buffer
 *
 * @return
 *    Length of imsi
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetImsi
(
U8             *imsi,
U32            len,
U8             *ueImsi
);

/**************************************************************************//**
 *
 * @brief
 *    Gets IE within Bearer Context IE
 *
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 *
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetBCtxIe
(
EgMsg          *egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst,        /* Instance of ie in bearer ctx */
EgIe           **egIe         /* ie */
);

/**************************************************************************//**
 *
 * @brief
 *    Encodes UE Security Capabilities into U8 buffer
 *
 * @param[in]  pUeSecCap
 *    Pointer to UE Security capability
 * @param[inout]  pBuf
 *    String buffer
 * @param[inout] pLen
 *    Length of pBuf after encoding
 *
 * @return
 *    No return values
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC Void vbMmeEgtUtlEncUeSecCap
(
CmEmmUeSecCap     *pUeSecCap,
U8                *pBuf,
U16               *pBufLen
);

/**************************************************************************//**
 *
 * @brief
 *    Decodes UE Security Capabilities into U8 buffer
 *
 * @param[in]  pBuf
 *    String buffer
 * @param[in] pLen
 *    Length of pBuf
 * @param[inout]  pUeSecCap
 *    Pointer to UE Security capability
 *
 * @return
 *    No return values
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC Void vbMmeEgtUtlDecUeSecCap
(
U8                *pBuf,
U16               bufLen,
CmEmmUeSecCap     *pUeSecCap
);

/**************************************************************************//**
 *
 * @brief
 *    Builds Release Access bearer Request
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 *
 * @return
 *    SUCCESS if Release Access Bearer Request is built successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldRABReq
(
VbMmeUeCb         *pUeCb,
EgMsg             **ppEgMsg
);

/**************************************************************************//**
 *
 * @brief
 *    Adds an IE inside RAB Bearer Context IE
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
EXTERN S16 vbMmeEgtUtlAddRABBCtxIe ARGS
((
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
));


#endif /* _VB_GTP_UTL_X_ */

/******************************************************************************

                                 END OF FILE

******************************************************************************/
