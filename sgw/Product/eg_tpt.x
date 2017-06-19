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
/********************************************************************20**

     Name:    EGTP - Transport Module

     Type:    C source file

     Desc:    This file contains all functions prototypes, structures etc
              used in Transport Module.

     File:    eg_tpt.x

     Sid:      eg_tpt.x@@/main/3 - Fri Feb  4 18:18:12 2011

     Prg:     ad 

*********************************************************************21*/

#ifndef __EG_TPT_X__
#define __EG_TPT_X__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @file eg_tpt.x
    @brief Transport Module for EGTP
*/

/**
  @ingroup TPTMOD
  @struct egHashCb
    @brief Hash CB for maintaining TEIDs
 */
#ifdef EGTP_C
typedef struct egHashCb
{
   CmHashListEnt egHlEnt;     /**< Hash List Entity */
   U32           teid;        /**< Local Teid */
   CmHashListCp  seqNoCbLst;  /**< Sequence No hash list CB */
} EgHashCb;


/** 
  @ingroup TPTMOD
  @struct egTptReTmrCb
  @brief TPT Timer Re-transmission CB
 */
/*eg006.201: added param, msgType,to retransmit Cb */ 
typedef struct egTptReTmrCb
{
   U32           seqNo;          /**< Sequence Number of Msg */
   EgTptSrvCb    *tptSrvCb;      /**< Transport Server CB */
   U32           teid;           /**< Local TeID  */
   CmTptAddr     destAddr;       /**< Destination IP-address/port*/
   S16           msgRetxRetryCnt;/**< Current Number of Retx Retry Count */      
   TmrCfg        t3;             /**< Request timer */
   EgTimer        reTransTmrNode;/**< Re-Trans Timer control block*/
   EgtMsgType    msgType;        /*--*< Message Type of Msg --*/

}EgTptReTmrCb;

/* eg014.201 - ccpu00139510 Egtpc release 10 upgrade */
/**
    @ingroup TPTMOD
    @struct egSeqNoCb
    @brief Sequence Number CB 
 */
/*eg006.201: added param, transId, to Sequence Cb */
typedef struct egSeqNoCb
{
   CmHashListEnt seqNoHlEnt;  /**< Seq No Hash List Entry */
   U32           seqNo;       /**< Sequence Number of Msg */
   EgtMsgType    msgType;     /**< Message Type of Msg */
   EgtMsgCat     msgCat;      /**< Message Category of Msg */
   U32           teid;        /**< Local TeId of Msg */
   Buffer        *mBuf;       /**< Encoded Msg */
   EgTptReTmrCb  tptReTmrCb;  /**< Retx-Msg timer */
   U32           transId;        /*--*< Transaction Id  --*/
   EgtIntf       intfType;    /* ccpu00135369 : single server support */
/*eg004.201 New variables added for eGTP-C PSF Upgrade
 *suConnId - To fetch the assocaited server control block
 *localTeid- To fetch the teid associated with msg
 */
#ifdef HW
   UConnId        suConnId;   /**< Server connection id */
   U32        localTeid;      /**< Associtated Teid */
   CmPFthaRsetMap  egHwCb;
#endif
}EgSeqNoCb;
#endif /* EGTP_C */

/**
    @ingroup TPTMOD
    @struct egPartialMsg
    @brief Partial Message Control Block  
 */
typedef struct egPartialMsg
{
   U32           seqNo;         /**< Sequence Number of Msg */
   EgtMsgType    msgType;       /**< Message type of Msg */
   EgtMsgCat     msgCat;        /**< Message Category of Msg */
   U32           teid;          /**< Local TEID of Msg */ 
   Bool          pbMsgPres;     /**< Piggy backed Message Presence Flag */ 
   Bool          isitPbmsg;    /**< Is it piggy backed message? */
   MsgLen        iniMsgLen;     /**< Initial Message Length in Piggy backed Message */ 
   MsgLen        pbMsgLen;      /**< Piggy backed Message Length */ 
}EgPartialMsg;

  /**********************************************************
   ************************Function Prototypes *************
   *********************************************************/
   /**
    * @ingroup TPTMOD
    * @brief This function issue a Disconnect Req towards Lower Layer
    * @details This function issue a Disconnect Req towards Lower Layer
    * 
    *
    * @param[in] tsapCb Pointer to the Transport SAP Control Block. 
    *
    * @param[in] choice Whether the connection Id is allocated by 
    *                   User or Provider.
    *
    * @param[in] connId Connection Id to which Disc Req to sent

    * @return  Void
    */

EXTERN Void  egTptIssueDiscReq ARGS (( 
                             EgTSapCb    *tsapCb, 
                             U8           choice,
                             ConnId       connId));

 /**
    * @ingroup TPTMOD
    * @brief This function deletes all context from Transport Server
    * @details This function deletes all context from Transport Server 
    * 
    *
    * @param[in] serverCb Pointer to the Transport Server Control Block. 
    *
    * @param[in] informUser Whether to inform to User ot Not.
    *
    * @return  Void
    */

EXTERN Void  egTptDelAllServerContext ARGS (( 
                             EgTptSrvCb    *serverCb, 
                             Bool          informUser));

 /**
    * @ingroup TPTMOD
    * @brief This function handles the Re-transmission timer expiry
    * @details This function handles the Re-transmission timer expiry
    *
    * @param[in] tptReTmrCb Pointer to the Transport Server Control Block. 
    *
    * @return  Void
    */

#ifdef EGTP_C
EXTERN Void egTptMsgRetxTmrExpiry ARGS((EgTptReTmrCb       *tptReTmrCb));

#endif /* EGTP_C */
 /**********************************************************
  ************************Utilities Functions *************
  *********************************************************/

 /**
    * @ingroup TPTMOD
    * @brief This function deletes the Token string of variable length
    * @details This function deletes the Token string of variable length
    *
    * @param[in] delTknStrOSXL Pointer to string which needs to be deleted
    *
    * @return  Void
    */

EXTERN S16  egUtlDelTknStrOSXL ARGS (( 
                             TknStrOSXL    *delTknStrOSXL ));
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __EG_TPT_X__ */



/********************************************************************30**

         End of file:     eg_tpt.x@@/main/3 - Fri Feb  4 18:18:12 2011

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      rsharon           1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg004.201 magnihotri       1. New variables added for eGTP-C PSF Upgrade
                                          suConnId - To fetch the assocaited server control block
                                          localTeid- To fetch the teid associated with msg
/main/3     eg006.201 psingh           1. Added msgType to Retransmit Cb 
                                       2. Added transId to Sequence Cb 
/main/3      eg014.201    rss     1. Egtpc release 10 upgrade
*********************************************************************91*/
