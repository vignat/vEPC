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

/*******************************************************************20*
 *   Name:    EGTP - Transport Module
 *
 *   Type:    C source file
 *
 *   Desc:    This file implements Transport Module.
 *
 *   File:    eg_tpt.c
 *
 *   Sid:      pg_tpt.c@@/main/3 - Fri Feb  4 18:18:16 2011
 *
 *   Prg:     ad 
 *******************************************************************21*/

/********************************************************************
 *       H E A D E R  I N C L U D E  F I L E S  (.h)                *
 ********************************************************************/
#include "envopt.h"                          /* Environment options */
#include "envdep.h"                /* Environment dependent options */
#include "envind.h"              /* Environment independent options */
#include "gen.h"                                   /* General layer */
#include "ssi.h"                        /* System service interface */
#include "cm5.h"                            /* Common timer library */
#include "cm_llist.h"                 /* Common linked list library */
#include "cm_hash.h"                         /* Common hash library */
#include "cm_tpt.h"                     /* Common transport library */
#include "cm_tkns.h"                               /* Common tokens */
#include "cm_mblk.h"                    /* Common memory allocation */
#include "cm_dns.h"                           /* Common DNS library */
#include "cm_inet.h"                       /* Common egcket library */
#include "hit.h" 
/* eg006.201 : moved inclusion of  eg.h */
#include "eg.h"            /* eGTP Layer */

/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#include "hw.h"
#include "sht.h"
#endif
/* HIT interface defines */

#include "egt.h"                    /* EGTP Upper Interface Defines */
#include "leg.h"                       /* EGTP MI Interface Defines */
#include "eg_tpt.h"                              /* EGTP TPT Module */
#include "eg_edm.h"                              /* EGTP TPT Module */

/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "lhw.h"
#endif
/********************************************************************
 *      H E A D E R/E X T E R N  I N C L U D E  F I L E S (.x)      *
 ********************************************************************/
#include "gen.x"                                   /* General layer */
#include "ssi.x"                       /* System services interface */
#include "cm5.x"                             /* Common timer module */
#include "cm_lib.x"                      /* Common linrary function */
#include "cm_llist.x"                   /* Common link list library */
#include "cm_hash.x"                    /* Common hash list library */
#include "cm_tkns.x"                               /* Common tokens */
#include "cm_tpt.x"                     /* Common transport library */
#include "cm_mblk.x"                    /* Common memory allocation */

#include "cm_dns.x"                           /* Common DNS library */
#include "cm_inet.x"                       /* Common egcket library */
#include "hit.x"                           /* HIT interface defines */
/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif

#include "egt.x"                   /* EGTP Upper Interface Defines  */
#include "leg.x"                      /* EGTP MI Interface Defines  */
#include "eg_edm.x"             /* EDM Module structures            */
#include "eg.x"                                    /* EGTP Defines  */
#include "eg_tpt.x"                              /* EGTP TPT Module */

/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC PSF                       */
#endif


/********************************************************************
  *         L O C A L  F U N C T I O N  D E F I N E S               *
  *******************************************************************/
#ifdef __cplusplus
EXTERN "C" {
#endif
#ifdef EGTP_C
PRIVATE S16 egTptPartialReadMsg ARGS (( 
                           Buffer          *mBuf, 
                           EgPartialMsg    *partialMsg,
                           Bool          pbVal));

PRIVATE S16 egTptValIncMsg ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           EgPartialMsg    *partialMsg));

PRIVATE S16 egTptValIncMsgWthMsb ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           EgPartialMsg    *partialMsg));


PRIVATE S16 egTptQueueMsg ARGS (( 
                           EgEvnt          *egEvnt,
                           EgTptSrvCb      *serverCb, 
                           EgPartialMsg    *partialMsg,
                           Buffer          *mBuf));

/* eg004.201 Warnings fixed for eGTP-C*/
#ifndef EGTP_C_PTH_MGT
PUBLIC S16 egTptQueueMsgRmv ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           U32             seqNo,
                           Bool            isSendInd));
#else
PUBLIC S16 egTptQueueMsgRmv ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           U32             seqNo,
                           Bool            isSendInd,
                           CmTptAddr       *destAddr));
#endif


PUBLIC S16 egTptQueueMsgWthMsb ARGS (( 
                           EgEvnt          *egEvnt,
                           EgTptSrvCb      *dfltSrvCb, 
                           EgPartialMsg    *partialMsg,
                           Buffer          *mBuf));

PUBLIC S16 egTptQueueMsgRmvWthMsb ARGS (( 
                           EgTptSrvCb      *dfltSrvCb, 
                           U32             teid,
                           U32             seqNo,
                           Bool            isSendInd));

/* eg012.201:Added egtpMsgType, fix for ccpu00125389 */
PRIVATE S16 egTptCheckQReqrd  ARGS ((
                           Bool         isMsbSet,
                           EgtMsgCat    egtpMsgCat,
                           EgtMsgType   egtpMsgType));


PRIVATE S16 egTptRmvTunnFrmLstWthMSB ARGS (( 
                           EgTptSrvCb      *dfltSrvCb, 
                           U32             teid));

PRIVATE S16 egTptRmvTunnFrmLst ARGS (( 
                           EgTptSrvCb      *serverCb, 
                           U32             teid));

PRIVATE S16 egTptValidatePBMsg ARGS ((
                           Buffer        *mBuf,
                           EgPartialMsg  *partialMsg));

#endif


#ifdef __cplusplus
}
#endif
#ifdef EGTP_U
/**********************************************************************
*       Fun:   egUTptPartialReadGTPuMsg
*
*       Desc:  This function Read the Message Partially to get:
*              (a) Sequence No.
*              (b) Message Type
*              (c) Message Category
*              (d) Tunnel Id.
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUTptPartialReadGTPuMsg
(
Buffer        *mBuf,                             /* Received message */
EgPartialMsg  *partialMsg                  /* Ptr to Partial Message */
)
#else
PRIVATE S16 egUTptPartialReadGTPuMsg (mBuf, partialMsg)
Buffer        *mBuf;                             /* Received message */
EgPartialMsg  *partialMsg;                 /* Ptr to Partial Message */
#endif
{
   S16         retVal;                    /* Holds the return value */
   MsgLen      bufLen;             /* Holds the total buffer length */
   U8          tmpByte;     /* Holds the one byte of data after Dec */
   U16         count;                   /* To count number of bytes */
   U8          msgType;                                 /* Msg Type */
   U32         teid;                                   /* Tunnel Id */

   EG_TRC2 (egUTptPartialReadGTPuMsg);

   retVal = ROK;
   bufLen = 0;
   tmpByte = 0;
   count = 0;

   /**************************
    * Get the message Length *
    **************************/
   SFndLenMsg(mBuf, &bufLen);

   /********************************************************
    * The minimum length should be 8 bytes.                *
    ********************************************************/
    if(bufLen <  EGU_MIN_HDR_LEN)
    {
       EG_DBG_ERR(0, 0, (egp,"\n Invalid Message Length"));
       retVal = EGT_ERR_MSG_INV_LEN;
       EG_RETVALUE(retVal);

    }

   /***********************************************
    * Get EGTP protocol version from the message. *
    * if Unable to get then log error and return  *
    ***********************************************/
/*     if (SExamMsg(&tmpByte, mBuf, count) != ROK)
    {
       EG_DBG_ERR(0, 0, (egp,"\n Failed to get the Version "));
       EG_RETVALUE(RFAILED);
    }

    version = tmpByte >> EG_SHIFT_5; */


   /*******************************
    * Now Decode the message Type *
    *******************************/
    count++;
    SExamMsg(&msgType, mBuf, count);


   /******************************************************
    * Skip reading the Message Length and decode the TEID* 
    ******************************************************/
    count = count + 3;

    EG_EXAM_U32(teid, mBuf, count);
#ifdef ALIGN_64BIT
    EG_DBG_INFO(0, 0, (egp,"\n  TEID(%d) of Message", teid));
#else
    EG_DBG_INFO(0, 0, (egp,"\n  TEID(%ld) of Message", teid));

#endif
   /*************************************************
    * Update the message parameter as Return values *
    *************************************************/
    partialMsg->msgType = (EgtMsgType)msgType;
    partialMsg->teid = teid;

    EG_RETVALUE (retVal);

}
/**********************************************************************
 *      Fun:   egUTptGetMsg
 *
 *      Desc:  This function handles message received from peer node.
 *             
 *      Ret:   ROK on success.
 *             RFAILED on error.
 *
 *      Note:  
 *             
 *      File:  eg_tpt.c
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egUTptGetMsg
(
EgTptSrvCb    *serverCb,  /* Server connection that received message */
CmTptAddr     *srcAddr,                         /* Source IP address */
Buffer        *mBuf,                              /* Received message */
Pst           *pst
)
#else
PUBLIC S16 egUTptGetMsg (serverCb, srcAddr, mBuf, pst)
EgTptSrvCb    *serverCb;  /* Server connection that received message */
CmTptAddr     *srcAddr;                         /* Source IP address */
Buffer        *mBuf;                             /* Received message */
Pst           *pst;
#endif
{
   S16               ret;          /* Used for function return value */
   EgUEvnt            evntMsg;           /* Actual Event Message ptr */
   EgPartialMsg      readMsgVals;        /* To store the Partial Msg */

   EG_TRC2 (egUTptGetMsg);

   EG_DBG_INFO(0,0,(egp,"\n Process the Message from Peer "));

   ret = RFAILED;

#ifdef EGTP_U_MULTI_THREADED
#ifndef SS_MT_CB
   /*************************************************************
    *   In case of multithreaded, drop the message if some control
    *   request has been posted to any of the threads.
    *************************************************************/
   if(serverCb->pendOp)
   {
           EG_DBG_ERR(0, 0,
                 (egp,"\n Control Operation in progress. Drop the message"));
           /*****************************
            * Delete The Message Buffer *
            *****************************/
           EG_FREEMBUF (mBuf);
           EG_RETVALUE (ROK);
   }
#endif /* SS_MT_CB */
#endif
   /*************************************************************
    *         STEP 1: Read the Message Partially                *
    *************************************************************/
   ret = egUTptPartialReadGTPuMsg(
         mBuf,              /* Encoded Message */
         &readMsgVals); /* Partial Read Values */

   /************************************************************
    * If failed to read partial Message, log error and return  *
    ************************************************************/
   if (ret != ROK)
   {
      EG_DBG_ERR(0, 0, (egp,"\n Reading Failed"));
      /*****************************
       * Delete The Message Buffer *
       *****************************/
      EG_FREEMBUF (mBuf);

      /*-- eg006.201: Return the correct value */
      EG_RETVALUE (ret);
   }

#ifdef ALIGN_64BIT
   EG_DBG_PARAM(0,0,
         (egp, "\n Received Message: MessageType (%d) Local Teid(%d)",
          readMsgVals.msgType, readMsgVals.teid));
#else
   EG_DBG_PARAM(0,0,
         (egp, "\n Received Message: MessageType (%d) Local Teid(%ld)",
          readMsgVals.msgType, readMsgVals.teid));

#endif

   /*****************************************************************
    * Only Echo Response is allowed on non-default server           *
    *****************************************************************/
   if(!(serverCb->isdftlTptSrv) && ((readMsgVals.msgType != EGT_GTPU_MSG_ECHO_RSP) && (readMsgVals.msgType != EGT_GTPU_MSG_ECHO_REQ)))
   {
      EG_DBG_ERR(0, 0,
            (egp,"\n Message type %d is not allowed on non-default server",
             readMsgVals.msgType));
      /*****************************
       * Delete The Message Buffer *
       *****************************/
      EG_FREEMBUF (mBuf);
      EG_RETVALUE (ROK);
   }
  
   /**********************************************************
    ********** Initialisze the Event Stucture                *
    ***********************************************************/
   cmMemset( (U8* )&evntMsg, 0, sizeof(EgUEvnt));

   /*****************************************************
    * Fill the event structure with                     *
    *  a) Received Message                              *
    *  b) Tpt Server on which Message is  Received      *
    *  c) Source Address from where Message is Received *
    *****************************************************/
   evntMsg.mBuf = mBuf;
   evntMsg.srvCb = serverCb;
   egCpyCmTptAddr(&(evntMsg.remAddr), srcAddr);

   egCpyCmTptAddr(&(evntMsg.localAddr), &serverCb->cfg.tptAddr);

   /******************************************
    * Fill the Event message with Local TEID *
    ******************************************/
   evntMsg.teid = readMsgVals.teid;

   /**********************************************************************
    * Fill the default region & pool Id . This will be overwritten 
    * with the region and pool of the repective thread for multithreading*
    **********************************************************************/
   evntMsg.region = serverCb->uSapCb->pst.region;
   evntMsg.pool = serverCb->uSapCb->pst.pool;

   /*******************
    * Fill the msgType*
    *******************/
   evntMsg.msgType = readMsgVals.msgType;


   /*-- eg003.201 : changes from eg009.102 : Verify that *
   *                Echo Req/Rsp have valid Destination--*/
   if ((evntMsg.msgType == EGT_GTPU_MSG_ECHO_REQ ) ||
         (evntMsg.msgType == EGT_GTPU_MSG_ECHO_RSP ))
   {
      EgUSapCb      *uSapCb  = NULLP;    /* pointer to current SAP */
      EgIpCb        *ipCb    = NULLP;
      EgDestAddrCb  *dstCb   = NULLP;
      SpId           spId    = 0;        /* Variable to store SAP Id --*/
      U8             uSapCnt = 0;        /* pointer to current SAP */
      CmTptAddr      keyAddr;

      for (spId=0;spId < egCb.genCfg.nmbUSaps ; spId++)
      {
         uSapCb = egCb.egtSapLst[spId];

         if(uSapCb != NULLP && (uSapCb->cfg.flag == LEG_EGTP_U))
         {

            cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
            EGU_SET_IP(keyAddr,evntMsg.localAddr );

            /**************************************
             * Find the source IP control block * 
             ************************************/
            cmHashListFind(&(uSapCb->ipCbHashList),
                  (U8 *) &(keyAddr),
                  sizeof(CmTptAddr), 0, (PTR *)&ipCb );

            if (ipCb == NULL)
            {
               EG_DBG_ERR(0, 0,
                     (egp,"\n NULL ipCb received for address %lx",
                      keyAddr.u.ipv4TptAddr.address));
               EG_RETVALUE(RFAILED);
            }

            cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
            EGU_SET_IP(keyAddr,evntMsg.remAddr ); 

            cmHashListFind(&(ipCb->destAddrCbLst),
                  (U8 *) &(keyAddr),
                  sizeof(CmTptAddr), 0, (PTR *)&dstCb);

            /*-- Multi threaded changes for performance --*/
#ifdef SS_MT_CB
            EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /*-- SS_MT_CB --*/

            /*---- Verify valid destination, but skip the loopback address --*/
            if((dstCb == NULLP)&&
                  (evntMsg.localAddr.u.ipv4TptAddr.address!= evntMsg.remAddr.u.ipv4TptAddr.address))
            {
               EgUsta     sta;/* unsolicited status diagnostics structure --*/
               /*--**********************************
                * Send Alarm to the layer manager *
                ***********************************/
               cmMemset( (U8* )&sta, 0, sizeof(EgUsta));
               sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
               egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), srcAddr);

               /*---- Send the alarm --*/
               egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
                     LEG_CAUSE_DST_IP_NOT_CONFIG, &sta.dgn); 

               EG_RETVALUE (RFAILED);

            }
         } /*---- uSapCb != NULLP --*/
         else
         {
            if( ++uSapCnt == egCb.genCfg.nmbUSaps)
            {
               /*---- No usap is configured, it shouldn't happen --*/
               EG_DBG_ERR(0, 0, (egp,"\n uSapCb not configured"));
               EG_RETVALUE(RFAILED);
            }
            continue ;
         } /*----  else --*/
      } /*---- for --*/
     }

   /***************************************************************
    * Fill the eventType(received from LI). It is required by CPM.* 
    ***************************************************************/
 /*  evntMsg.eventType = EGU_EVT_HIT; */

   /*********************
    * Update Statistics * 
    *********************/
   serverCb->stats.totalMsgsRx++;
   serverCb->ipCbPtr->ipStats.totalMsgsRx++;

   /**********************************************
    * eg007.201: Provide uSap for stats updation*
    **********************************************/
   egUpdateRxStatistics(readMsgVals.msgType, &(serverCb->uSapCb->sts));

   /************
    * Call CPM * 
    ************/
   pst->srcInst = pst->dstInst;
   egUCpmHdlEvt(&evntMsg, pst);


   EG_RETVALUE (ROK);
}
#endif
#ifdef EGTP_C
/**********************************************************************
 *      Fun:   egTptGetMsg
 *
 *      Desc:  This function handles message received from peer node.
 *             
 *      Ret:   ROK on success.
 *             RFAILED on error.
 *
 *      Note:  This function is also responsible for deallocating m-
 *             essage buffer.
 *      File:  eg_tpt.c
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptGetMsg
(
EgTptSrvCb    *serverCb,  /* Server connection that received message */
CmTptAddr     *srcAddr,                         /* Source IP address */
Buffer        *mBuf,                              /* Received message */
Bool          pbVal                              /* pBit validation  */
)
#else
PUBLIC S16 egTptGetMsg (serverCb, srcAddr, mBuf, pbVal)
EgTptSrvCb    *serverCb;  /* Server connection that received message */
CmTptAddr     *srcAddr;                         /* Source IP address */
Buffer        *mBuf;                             /* Received message */
Bool          pbVal;                             /* pBit validation  */
#endif
{
   S16               ret;          /* Used for function return value */
   U32               seqNo;        /* Sequence Number of the Message */
   EgtMsgType        egtpMessageType;  /* EGTP Message Type received */
   EgtMsgCat         egtpMsgCat;          /* Category of the message */
   EgEvnt            *evntMsg;           /* Actual Event Message ptr */
   Pst               pst;        /* PST structure for Multithreading */
   EgTptSrvCb        *dfltSrvCb; /* Default Tpt Server Control Block */
   U32               teid;             /* Stores Tunnel End Point Id */
   /*eg004.201 Intialize EgPartialMsg object for consistency*/
   EgPartialMsg      readMsgVals = {0};        /* tO store the Partial Msg */
   Bool              isMsbSet;      /* To check if MSB is set or not */
   Buffer            *pbmBuf;       /* Piggy backed Received message */

   EG_TRC2 (egTptGetMsg);

   EG_DBG_INFO(0,0,(egp,"\n Process the Message from Peer "));

   ret = RFAILED;
   evntMsg = NULLP;
   egtpMessageType = EGT_GTP_MSG_INVALID;
   egtpMsgCat = EG_GTPC_MSGCAT_OTHER;
   dfltSrvCb = NULLP;
   teid = EG_INVALID_TEID;
   isMsbSet = FALSE;

   /*************************************************************
    *         STEP 1: Read the Message Partially                *
    *************************************************************/
   ret = egTptPartialReadMsg(mBuf,&readMsgVals,pbVal);

   /************************************************************
    * If failed to read partial Message, log error and return  *
    ************************************************************/
   if (ret != ROK)
   {
      /**********************************************************
       * If the length validation of a piggy backed message has *
       * then send an error indication to user                  *
       **********************************************************/
      if(ret == EGT_ERR_MSG_INVOVR_LEN_PBMSG)
      {
         EG_ALLOCEVNT (&evntMsg);

         /**************************************************************
          * If unable to allocate event structure log error and return *
          **************************************************************/
         if (evntMsg == NULLP)
         {
            EG_DBG_ERR (0, 0, (egp, "\n Allocation of event failed "));
         }
         else
         {
           /*********************************************
            * Send Error indication to User application *
            *********************************************/
            EG_DBG_INFO(0, 0, (egp,"\n Sending Error Indication to user for invalid PBMsg"));
            evntMsg->msgType = readMsgVals.msgType;
            evntMsg->seqNumber = readMsgVals.seqNo;
            evntMsg->resultType = EGT_ERR_MSG_INVOVR_LEN_PBMSG;
            (void) egSendErrInd(evntMsg, serverCb->uSapCb);
         }
      }
      else
        EG_DBG_ERR(0, 0, (egp,"\n Reading Failed"));
     /*****************************
      * Delete The Message Buffer *
      *****************************/
      EG_FREEMBUF (mBuf);
      EG_RETVALUE (ROK);
   }

   /******************************************************************* 
    * If Piggy Backed message present, split the msg using the Length *
    * field values as both needs to be processed independently        *
    *******************************************************************/
   if(pbVal && readMsgVals.pbMsgPres)
   {
     ret = SSegMsg(mBuf, (readMsgVals.iniMsgLen + EG_LEN_FOUR), &pbmBuf);
   }

   /**************************************
    * Get Seq No , Message Type and TEID *
    **************************************/
   seqNo = readMsgVals.seqNo;
   egtpMessageType = readMsgVals.msgType; 
   egtpMsgCat = readMsgVals.msgCat; 
   teid = readMsgVals.teid;

#ifdef ALIGN_64BIT
   EG_DBG_PARAM(0,0,
         (egp, "\n Received Message: SeqNo(%d) MessageType (%d) Local Teid(%d)", 
          seqNo, egtpMessageType, teid));
#else
   EG_DBG_PARAM(0,0,
         (egp, "\n Received Message: SeqNo(%ld) MessageType (%d) Local Teid(%ld)", 
          seqNo, egtpMessageType, teid));

#endif


   /***********************************************************
    *         STEP 2: Check if MSB is set                     *
    ***********************************************************/
    EG_IS_MSB_SET_IN_SEQ_NO(seqNo, isMsbSet);

   /**********************************************************
    * if MSB is SET in EGTP message Sequence Number then we  *
    * need to validate Message based on the message category *
    **********************************************************/
    if (TRUE == isMsbSet)
    {
       /************************************************
        * Since MSB is set in Sequence Number, we need *
        * to use the defalut TPT server Control block  *
        ************************************************/
        EG_DBG_INFO(0,0, (egp, "\n MSB is set in Sequence No. "));
#ifdef EG_MULTI_IP
        /**********************************************************
         * eg007.201: derive Default SrvCb from TSapCb            *
         **********************************************************/
        dfltSrvCb = serverCb->tSapCb->dftlTptSrv;
#else
        dfltSrvCb = egCb.tSapCb->dftlTptSrv;
#endif
 
       /*******************************************
        * if defalut TPT server Control block is  *
        * invalid, log error and return from here *
        *******************************************/
        if (NULLP == dfltSrvCb)   
        {
           EG_DBG_ERR(0, 0,
                    (egp,"\n Default TPT Server is not present"));

          /*****************************
           * Delete The Message Buffer *
           *****************************/
           EG_FREEMBUF (mBuf);
           EG_RETVALUE (ROK);
        }
       
       /***********************************************
        * We got valid defalut serverCB, So validate  *
        * the Incoming Message based on Category      *
        ***********************************************/
        switch (egtpMsgCat)
        {
           case EG_GTPC_MSGCAT_RESPONSE: 
           case EG_GTPC_MSGCAT_REQUEST:
           case EG_GTPC_MSGCAT_FAILIND:
           case EG_GTPC_MSGCAT_NOTFN:
              {  /******************************************
                  * Validate EGTP Incoming Req/Rsp Message *
                  ******************************************/
                  ret = egTptValIncMsgWthMsb (dfltSrvCb, &readMsgVals);

                  /*************************************************
                   * Fail to validate message,log error and return *
                   *************************************************/
                  if (ret != ROK)
                  {
                     EG_DBG_ERR(0, 0, 
                      (egp,"\n Incoming Req/Rsp Validation Failed"));
                    
                    /*****************************
                     * Delete The Message Buffer *
                     *****************************/
                     EG_FREEMBUF (mBuf);
                     EG_RETVALUE (ROK);
                  }
               }
               break;
           case EG_GTPC_MSGCAT_COMMAND: 
           case EG_GTPC_MSGCAT_ACKN:
              {  /*************************************************
                  * No validation is required for the CMD message *
                  *************************************************/
                  EG_DBG_INFO(0, 0, 
                  (egp,"\n No Validation for Incoming CMD Message "));
              }
              break;
           default :
              {  /*************************************************** 
                  * For other messages MSB should not be set, So no *
                  * need to continue from here,log error and return *
                  ***************************************************/       
                 EG_DBG_ERR(0, 0, 
                       (egp,"\n Validation Failed: MSB shouldn't be Set"));
                
                /*****************************
                 * Delete The Message Buffer *
                 *****************************/
                 EG_FREEMBUF (mBuf);
                 EG_RETVALUE (ROK);
  
              }
        }
     }
     else
     {
       /**********************************************************
        * MSB is UNSET in EGTP message Sequence Number so we can *
        * consider this as response message. Now validate this   *
        * EGTP Rsp Message  based on the EGTP message category   *
        **********************************************************/
        EG_DBG_INFO(0,0, (egp, "\n MSB is not set in Sequence No."));

        switch (egtpMsgCat)
        {
           /* ccpu00138943: Freeing Queued Message */
           case EG_GTPC_MSGCAT_RESPONSE: 
           case EG_GTPC_MSGCAT_FAILIND:
           case EG_GTPC_MSGCAT_RSP_ACK:
           case EG_GTPC_MSGCAT_ACKN:
              {   /*************************************************
                   * Validate Incoming Response or Acknowledgement *
                   *************************************************/
                   ret = egTptValIncMsg (serverCb, &readMsgVals);

                   /************************************************
                    * If error in validation, Log error and return *  
                    ************************************************/
                   if (ret != ROK)
                   {
                      EG_DBG_ERR(0, 0,
                            (egp,"Reponse/Ackn Validation Failed"));
                   
                     /*****************************
                      * Delete The Message Buffer *
                      *****************************/
                      EG_FREEMBUF (mBuf);
                      EG_RETVALUE (ROK);
                   }
              }
              break;
           case EG_GTPC_MSGCAT_REQUEST:
           case EG_GTPC_MSGCAT_IND:
           case EG_GTPC_MSGCAT_NOTFN:
              {   /***************************************************
                   * For Request or Notification or Indication       *
                   * messages no validation is required, so continue *
                   ***************************************************/
                   EG_DBG_INFO(0, 0, (egp,
                   "\n No Validation for Incoming Req/Notfy/Ind Msg"));
              }
              break;
           default:
              {  /*************************************************** 
                  * For other messages MSB should be set, So no     *
                  * need to continue from here,log error and return *
                  ***************************************************/       
                    EG_DBG_ERR(0, 0, 
                      (egp,"\n Validation Failed: MSB should be Set"));
                   
                   /*****************************
                    * Delete The Message Buffer *
                    *****************************/
                    EG_FREEMBUF (mBuf);
                    EG_RETVALUE (ROK);
              }
        }
     } 
    /*************************************************************
     *       STEP 3: Allocate the Event Structure                *
     *************************************************************/
     EG_ALLOCEVNT (&evntMsg);
  
    /**************************************************************
     * If unable to allocate event structure log error and return *
     **************************************************************/
     if (evntMsg == NULLP)
     {
        EG_DBG_ERR (0, 0, (egp, "\n Allocation of event failed "));
 
       /*****************************
        * Delete The Message Buffer *
        *****************************/
        EG_FREEMBUF (mBuf);
        /*-- eg006.201 : Return Error for Alloc failure */
        EG_RETVALUE (RFAILED);
     }

    /**********************************************************
     ********** Initialisze the Event Stucture                *
    ***********************************************************/
     cmMemset( (U8* )evntMsg, 0, sizeof(EgEvnt));

    /*****************************************************
     * Fill the event structure with                     *
     *  a) Received Message                              *
     *  b) Tpt Server on which Message is  Received      *
     *  c) Source Address from where Message is Received *
     *****************************************************/
     evntMsg->u.mBuf = mBuf;
     evntMsg->srvCb = serverCb;
     egCpyCmTptAddr(&(evntMsg->remAddr), srcAddr);

     egCpyCmTptAddr(&(evntMsg->localAddr), &serverCb->cfg.tptAddr);

     /******************************************
      * Fill the Event message with Local TEID *
      ******************************************/
     evntMsg->lclTeid.pres = PRSNT_NODEF;
     evntMsg->lclTeid.val = teid;

     evntMsg->msgType = egtpMessageType;
     evntMsg->seqNumber = seqNo;

     /* eg010:201: variable name changed */
     evntMsg->isitPbMsg = readMsgVals.isitPbmsg;


    /**********************************************************
     * eg006.201: Destination validation for Echo Req/Rsp  
     **********************************************************/
#ifdef EGTP_C_PTH_MGT
     /* ccpu00134942  */
     if ((egtpMessageType == EGT_GTPC_MSG_ECHO_REQ)|| 
         (egtpMessageType == EGT_GTPC_MSG_ECHO_RSP))
     {
        evntMsg->intfType = EGT_GTP_INT_UNKNWN;
     }
#endif  /* EGTP_C_PTH_MGT */

    /*************************************************************
     *    STEP 4: Pass the Message to Encoder/Decoder Module     *
     *************************************************************
     * Now it is required to Decode the message based on whether *
     * received EGTP message is Path message or Tunnel Message   *
     *************************************************************/
     if ((egtpMessageType == EGT_GTPC_MSG_ECHO_REQ)|| 
         (egtpMessageType == EGT_GTPC_MSG_ECHO_RSP) ||
         (egtpMessageType == EGT_GTPC_MSG_VER_N_SUPP_IND))
     {
        /*********************************************
         * We are confirm that this is Path Message, *
         * So call EDM module premitive for decoding *
         *********************************************/
         /* ccpu00136942 */
         ret = egDecPthReq(evntMsg);
     }
     else
     {
        /*********************************************
         * We are sure that it is Tunnel Message,    *
         * So call EDM module premitive for decoding *
         *********************************************/
#ifdef EG_MT
        /*************************************
         * If Multithreaded then PST is must *
         *************************************/
        ret = egDecMsgReq(&pst, evntMsg);
#else   
        ret = egDecMsgReq(evntMsg);
#endif /* EG_MT */
     }

    /*************************************************
     * Continue processing even if the encode decode *
     * module unable to decode the message. Now the  *
     * processing is based on EDM erorr category     *
     *************************************************/
    if (ret != ROK)
    {
        EG_DBG_ERR(0, 0, (egp,"\n Message Processing Failed"));

        /*************************************************************
          * Here we will be processing error from ED module          *
          * only, if there is any failure in Tunnel or other modules *
          * then it should be handled by Tunnel module and it should *
          * return ROK.
          ***********************************************************/
        /*-- eg010.102: Error Ind sent for EGT_ERR_MSG_UNABLE_TO_COMPLY --*/ 
        if (EGT_ERR_MSG_INV_VERSION == ret)
        {
           /**************************************************
            * Send back the response for Invalid Version Req *
            **************************************************/
            EG_DBG_INFO(0, 0,
            (egp,"\n Sending Version Not Supported Msg to peer"));
            (void) egTptSendMsg(serverCb, srcAddr, evntMsg->u.mBuf, &pst);

            EG_FREEMBUF (evntMsg->u.mBuf);
            egCmFreeEvent(evntMsg);
            EG_RETVALUE (ROK);
        }
        else
        {
 
           /*********************************************
            * Send Error indication to User application *
            *********************************************/
            EG_DBG_INFO(0, 0, (egp,"\n Sending Error Indication to user"));
            (void) egSendErrInd(evntMsg, serverCb->uSapCb);

            EG_RETVALUE (ROK);
        }

        /******************************
         *  Delete The Message Buffer *
         ******************************/
        EG_FREEMBUF (mBuf);

        EG_RETVALUE (ROK);
    }

    EG_DBG_INFO(0, 0, (egp,"\n Message Processing Successful"));
  
   /*****************************************************
    * eg007.201: Update EGTP protocol received  message * 
    * Statistics with tSapCb obtained from serverCb     *
    *****************************************************/
#ifdef EG_MULTI_IP
    egUpdateRxStatistics(egtpMessageType, &(serverCb->tSapCb->sts));
#else
    egUpdateRxStatistics(egtpMessageType, &(egCb.tSapCb->sts));
#endif

   /************************************************************ 
    * If Piggy Backed message was present, then process it now *
    ************************************************************/
   if(pbVal && readMsgVals.pbMsgPres)
   {
     (Void) egTptGetMsg(serverCb, srcAddr, pbmBuf, FALSE);
   }

    EG_RETVALUE (ROK);
}

/**********************************************************************
*       Fun:   egTptPartialReadMsg
*
*       Desc:  This function Read the Message Partially to get:
*              (a) Sequence No.
*              (b) Message Type
*              (c) Message Category
*              (d) Tunnel Id.
*              (e) Piggybacked Message presence
*              (f) Length of the initial Message in a piggybacked message
*              (g) Length of the piggybacked message 
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTptPartialReadMsg
(
Buffer        *mBuf,                             /* Received message */
EgPartialMsg  *partialMsg,                 /* Ptr to Partial Message */ 
Bool          pbVal                               /* pBit validation */ 
)
#else
PRIVATE S16 egTptPartialReadMsg (mBuf, partialMsg, pbVal)
Buffer        *mBuf;                             /* Received message */
EgPartialMsg  *partialMsg;                 /* Ptr to Partial Message */ 
Bool          pbVal;                              /* pBit validation */ 
#endif
{
   S16         retVal;                    /* Holds the return value */
   MsgLen      bufLen;             /* Holds the total buffer length */
   U8          tmpByte;     /* Holds the one byte of data after Dec */
   U16         count;                   /* To count number of bytes */
   U8          version;                              /* GTP version */
   U8          msgType;                                 /* Msg Type */
   U32         teid;                                   /* Tunnel Id */
   U32         seqNo;                            /* Sequence Number */
   U8          pBit;                                     /* sathish */

   EG_TRC2 (egTptPartialReadMsg);

   retVal = ROK;
   bufLen = 0;
   tmpByte = 0;
   count = 0;
   seqNo = 0;
   teid = EG_INVALID_TEID;
   
   /**************************
    * Get the message Length *
    **************************/
   SFndLenMsg(mBuf, &bufLen);

   /********************************************************
    * Msg header is 8 bytes in case of ECHO REQ/RSP , and  *
    * 12 bytes in case of all control messages. Atleast    *
    * the EGTP Message should be of this much length       *
    * The minimum length should be 8 bytes.                *
    ********************************************************/
    if(bufLen <  8)
    {
       EG_DBG_ERR(0, 0, (egp,"\n Invalid Message Length"));
       retVal = EGT_ERR_MSG_INV_LEN;
       EG_RETVALUE(retVal);
 
    }   
   
   /***********************************************
    * Get EGTP protocol version from the message. *
    * if Unable to get then log error and return  *
    ***********************************************/
    if (SExamMsg(&tmpByte, mBuf, count) != ROK)
    {
       EG_DBG_ERR(0, 0, (egp,"\n Failed to get the Version "));
       EG_RETVALUE(RFAILED);
    }
    
    count++;
    version = tmpByte >> EG_SHIFT_5;

   /*************************************************
    * Check if the version is 2, if not report      *
    * Invalid Version and continue with processing  *
    *************************************************/
    if(version != EG_MSG_VERSION)
    {
      /********************************
       * No need to return from here. *
       * EDM will take care of this.  *
       ********************************/
       EG_DBG_INFO(0,0, (egp,
       "\n Invalid Msg Version(%d), resultType(%d)", version, retVal));
   }

   /*******************************
    * Now Decode the message Type *
    *******************************/
    SExamMsg(&msgType, mBuf, count);

    count++;

   /************************************************* 
    * Check the P bit in the header to see if piggy *
    * backed message is present                     *
    *************************************************/
    pBit = (tmpByte >> EG_SHIFT_4) & 0x01;

   /************************************************************************* 
    * There can be error instance where the piggy back bit in the header is *
    * set for piggy backed initial message also. This check is done below.  *
    *************************************************************************/
    if(pBit && !(pbVal))
    {
       EG_DBG_ERR(0, 0, (egp,"\n pBit set for PgyBck IniMsg(%d) also",msgType));
       retVal = EGT_ERR_MSG_INV_HDR;
       EG_RETVALUE(retVal);
    }

    if(!pBit && !pbVal)
    {
       partialMsg->isitPbmsg = TRUE;
    }
    else
    {
       partialMsg->isitPbmsg = FALSE;
    }
    

   /***************************************************************************
    * Length validation has to be done initially if pBit is set so that the   *
    * message can be split based on the respective lengths. This validation   *
    * should be skipped once the message is split i.e, for the second message *
    ***************************************************************************/
    if(pbVal && pBit)
    { 
         EG_DBG_INFO(0, 0, (egp,"\n pBit is set"));
    
        /*******************************
         * Validate the message length *
         *******************************/ 
         retVal = egTptValidatePBMsg(mBuf, partialMsg);

         if(retVal != ROK)
         {
            /*********************************************************
             * Length validation failed, so send an error indication *
             * to user with appropriate error                        *
             *********************************************************/ 
             EG_DBG_ERR(0, 0, (egp,"\n PiggyBacked Message Len Validation Failed"));
             /* Send Error Indication to User with retVal cause */
             EG_RETVALUE(retVal);
         }
        /*************************************************************
         * Set the piggybacked message presence flag to TRUE as this *
         * flag is used to schedule the processing of second message *
         *************************************************************/ 
         partialMsg->pbMsgPres = TRUE;
    }

   /***********************************
    * Skip reading the Message Length * 
    ***********************************/ 
    count = count + 2;
   
    if ((msgType == EGT_GTPC_MSG_ECHO_REQ) || 
         (msgType == EGT_GTPC_MSG_ECHO_RSP) || 
         (msgType == EGT_GTPC_MSG_VER_N_SUPP_IND))
    {
      /**************************************************
       * TEID not needed for these messages, do nothing *
       **************************************************/
       EG_DBG_INFO(0,0, (egp,
               "\n EGTP Msg(%d) received", msgType));
    }
    else
    {
      /***********************************************
       *  For messages other than ECHO Req/Rsp and   *
       * Version Support Lenght should be atleast 12 *
       ***********************************************/
       if(bufLen <  12)
       {
          EG_DBG_ERR(0, 0, (egp,"\n Invalid Message Length"));
          retVal = EGT_ERR_MSG_INV_LEN;
          EG_RETVALUE(retVal);
       }   
 
      /*******************************************************
       * Read TEID and Increment counter by 4 bytes for TEID *
       *******************************************************/
       if (tmpByte & EG_MASK_BIT4)
       {
          EG_EXAM_U32(teid, mBuf, count);
#ifdef ALIGN_64BIT
          EG_DBG_INFO(0, 0, (egp,"\n  TEID(%d) of Message", teid));
#else
          EG_DBG_INFO(0, 0, (egp,"\n  TEID(%ld) of Message", teid));

#endif

          count = count + 4; 
       }
    }
   
   /******************************************
    * Read Sequence Number from the message. *
    ******************************************/
    EG_EXAM_U32(seqNo, mBuf, count);
    seqNo >>= EG_SHIFT_8;
    seqNo &= 0x00FFFFFF;
    EG_DBG_INFO(0, 0, (egp,"\n Seq No.(%ld) of Message", seqNo));


   /*************************************************
    * Update the message parameter as Return values *
    *************************************************/
    partialMsg->seqNo = seqNo;
    partialMsg->msgType = (EgtMsgType)msgType;
    partialMsg->msgCat = egGetMsgCategory((EgtMsgType)msgType);
    partialMsg->teid = teid;
    
    EG_RETVALUE (retVal);

}

/**********************************************************************
*       Fun:   egTptValIncMsg
*
*       Desc:  This function Validate whether incoming Response/Ack
*            has corresponding Request/Notification in Sequence Queue.
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTptValIncMsg
(
EgTptSrvCb   *serverCb,   /* Server connection that received message */
EgPartialMsg *partialMsg                  /* Partial Decoded Message */
)
#else
PRIVATE S16 egTptValIncMsg (serverCb, partialMsg)
EgTptSrvCb   *serverCb;   /* Server connection that received message */
EgPartialMsg *partialMsg;                 /* Partial Decoded Message */
#endif
{
   S16               ret;          /* return value of function call */
   U32               seqNo;       /* Sequence Number in the Message */
   EgSeqNoCb         *seqNoCb;      /* Control block of Seq  Number */
   EgtMsgType        pairMsgType;    /* Pair Msg type of stored Msg */
   EgtMsgType        msgType;                  /* EGTP Message Type */
   U32               teid;

   EG_TRC2 (egTptValIncMsg);

   EG_DBG_INFO(0,0, (egp, "\n Validating the Incoming Message "));

   ret = RFAILED;
   seqNoCb = NULLP;
   pairMsgType = EGT_GTP_MSG_INVALID;

   seqNo = partialMsg->seqNo;
   msgType = partialMsg->msgType;
   teid = partialMsg->teid;

   /***************************************************
    * eg007.201 : Removed restriction from Dflt Servr *
    * to allow processing of the response msgs 
   ***************************************************/
   /* ccpu00135369 : Default server can expect response messages */

   /*************************************************
    * Get the Seq. Num CB from from the hash list.  *
    * if unable to get the CB, log error and return *
    *************************************************/
    ret = cmHashListFind(&serverCb->msgHlCp, 
          (U8 *) &seqNo, sizeof(U32), 
          0, (PTR *)&seqNoCb); 
    if ((ret != ROK) || (seqNoCb == NULLP))
    {
       EG_DBG_ERR(0, 0,
        (egp,"\n Failed to get the Seq No(%ld). CB ", seqNo));
       EG_RETVALUE(RFAILED);
    }
   
   /****************************************************
    * Validate if whether we got the correct Local Teid*
    * if not then log error and return from here.      *
    ****************************************************/
    if (seqNoCb->teid != teid)
    {
       /************************************************
       *  In case of the Echo Req/Rsp/Ver Ind, teid is *
       *  not present. So check whether it is path msg *
       *  If yes dont return error.                    *
       ************************************************/
       if (!((msgType == EGT_GTPC_MSG_ECHO_REQ)|| 
           (msgType == EGT_GTPC_MSG_ECHO_RSP) ||
           (msgType == EGT_GTPC_MSG_VER_N_SUPP_IND)))
       {
#ifdef ALIGN_64BIT
          EG_DBG_ERR(0, 0,
            (egp,"\n Local Teid(%d) doesn't Match with Stored Teid(%d)", 
                     teid, seqNoCb->teid));
#else
          EG_DBG_ERR(0, 0,
            (egp,"\n Local Teid(%ld) doesn't Match with Stored Teid(%ld)", 
                     teid, seqNoCb->teid));

#endif

          EG_RETVALUE(RFAILED);
       }
    }

   /*******************************************
    * Get the Pair Message for stored Message * 
    *******************************************/
    pairMsgType = egGetAssocMsg(seqNoCb->msgType);

   /***********************************************
    * if the Pair Message type is not equal to    * 
    * EGTP Message type then log error and return *
    ***********************************************/
    if (msgType != pairMsgType)
    {
       ret = egChkFailInd((U8)msgType, (U8)seqNoCb->msgType);
       if ( ret != ROK)
       { 
    
          EG_DBG_ERR(0, 0,
          (egp,"\n Expected Message(%d) however got Message(%d)", 
                pairMsgType, msgType));
          EG_RETVALUE(RFAILED);
       }
    }

   /***********************************************************
    * Got appropriate Message, do the clean up                *
    ***********************************************************/

   /*******************************
    * Removed Queued EGTP Message *
    *******************************/

#ifndef EGTP_C_PTH_MGT
    (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE);
#else
    (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE,NULLP);
#endif

  EG_RETVALUE (ROK);
}

/**********************************************************************
*       Fun:   egTptProcessMsg
*
*       Desc:  This function handles message received from
*              Encoder/Decoder Module
*
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef EG_MT
#ifdef ANSI
PUBLIC S16 egTptProcessMsg
(
Pst         *pst,                                  /* Post structure */
EgEvnt      *egEvnt                    /* Encoded Message to be sent */
)
#else
PUBLIC S16 egTptProcessMsg (pst, egEvnt)
Pst         *pst;                                  /* Post structure */
EgEvnt      *egEvnt;                   /* Encoded Message to be sent */
#endif
#else
#ifdef ANSI
PUBLIC S16 egTptProcessMsg
(
EgEvnt      *egEvnt                    /* Encoded Message to be sent */
)
#else
PUBLIC S16 egTptProcessMsg (egEvnt)
EgEvnt      *egEvnt;                    /* Encoded Message to be sent*/
#endif
#endif /* EG_MT */
{
   S16               ret = RFAILED;    /* Retrune value to validate */
   S16               qRequired = FALSE;     /* Queing Required flag */
   EgTptSrvCb        *serverCb = NULLP; /* Tpt Server Control Block */
   U32               seqNo = 0;      /* Seq. Number of EGTP Message */
   EgtMsgCat         egtpMsgCat = EG_GTPC_MSGCAT_OTHER; /* EGTP Message Category */
   EgtMsgType        egtpMessageType = EGT_GTP_MSG_INVALID; /* EGTP Message Type */  
   CmTptAddr         destAddr;            /* Dest IP-address/port */
   Bool              isMsbSet =  FALSE;   /* MSB of Message Seq Num */
   EgTptSrvCb        *dfltSrvCb = NULLP;   /* Default Tpt Server CB */
   U32               teid = EG_INVALID_TEID; /* To Store the Tunnel ID */
   EgPartialMsg      readMsgVals;       /* To store the Partial Msg */
   Pst               tmpPst;
   /*eg010.210 : Added for piggyback support */
#ifdef EG_PIGGYBACK_SUPP
   EgTECCb     **teCCb;
#endif
   Bool        isTransMsg = FALSE;
   
   EG_TRC2 (egTptProcessMsg);

   EG_DBG_INFO(0,0,(egp,"\n Process the Message from User "));

   (Void)cmMemset ((U8 *)&destAddr, 0, sizeof (CmTptAddr));

   /********************************************************
    * If event Message is not present log error and return *
    ********************************************************/
   if (egEvnt == NULLP)
   {
      EG_DBG_ERR(0, 0, (egp,"\n Event Message is NULL"));
      EG_RETVALUE(RFAILED);
   }

   /**********************************************
    * Get Remote Address                         *
    **********************************************/
    egCpyCmTptAddr(&destAddr, &(egEvnt->remAddr));

   /****************************************
    * Get the transport Server from Event, *
    * this should have been filled by PCM  *
    ****************************************/
    serverCb = egEvnt->srvCb;

   /*********************************************
    * If Transport Server Control block is not  *
    * present then Message cann't be processed  *
    *********************************************/
    if (NULLP == serverCb)
    {
       EG_DBG_ERR(0, 0, (egp,"\n Transport Server is Not Provided"));
       
      /******************************************
       * Delete the Message Buffer              *
       ******************************************/
       EG_FREEMBUF (egEvnt->u.mBuf);
       EG_RETVALUE (RFAILED);
     }

   /*************************************************************
    ************STEP 1: Read the Partial Message*****************
    *************************************************************/
   /****************************************
    * Get Sequence Number and Message Type,*
    * Message Category and TEID            *
    ****************************************/
    seqNo = egEvnt->seqNumber;
    egtpMessageType = (EgtMsgType)egEvnt->msgType;
    egtpMsgCat = egGetMsgCategory(egtpMessageType);
    if (NOTPRSNT != egEvnt->lclTeid.pres)
    {
       teid = egEvnt->lclTeid.val;
    }
    else
    { 
       teid = egEvnt->u.egMsg->msgHdr.teid;
       isTransMsg = TRUE;
    }

    /********************************************************* 
     *** Fill the Partial Read Message structure             *
    **********************************************************/
    readMsgVals.seqNo = seqNo;
    readMsgVals.msgType = egtpMessageType;
    readMsgVals.msgCat = egtpMsgCat;
    readMsgVals.teid = teid;

#ifdef ALIGN_64BIT
    EG_DBG_PARAM(0,0,
    (egp, "\n Process Message: SeqNo(%d) MessageType (%d) Local Teid(%d)",
                 seqNo, egtpMessageType, teid));
#else
    EG_DBG_PARAM(0,0,
    (egp, "\n Process Message: SeqNo(%ld) MessageType (%d) Local Teid(%ld)",
                 seqNo, egtpMessageType, teid));

#endif


   /*************************************************
    * TEID should be present for all messages       *
    * other than ECHO Request & ECHO Response       *
    *************************************************/
    /* ccpu00137353, cppu00137318 ,ccpu00139013 */
    if (((EGT_GTPC_MSG_ECHO_REQ != egtpMessageType) &&
        (EGT_GTPC_MSG_ECHO_RSP != egtpMessageType) &&
        (EGT_GTPC_MSG_DPDN_CON_SET_RSP != egtpMessageType) && 
        (EGT_GTPC_MSG_DPDN_CON_SET_REQ != egtpMessageType) &&  
        (EGT_GTPC_MSG_ID_REQ != egtpMessageType) &&
        (EGT_GTPC_MSG_ID_RSP != egtpMessageType) &&
        (EGT_GTPC_MSG_CNTXT_REQ != egtpMessageType) &&
        /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
        (EGT_GTPC_MSG_PGW_RST_NOTFN != egtpMessageType) &&
        (EGT_GTPC_MSG_PGW_RST_NOTFN_ACK != egtpMessageType) &&
        (EGT_GTPC_MSG_DWN_DATA_NOTFN != egtpMessageType) &&
        (EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK != egtpMessageType) &&
        (EGT_GTPC_MSG_DD_NOTFN_FAIL_IND != egtpMessageType) &&
   (EGT_GTPC_MSG_SSPND_NOTFN != egtpMessageType) &&
   (EGT_GTPC_MSG_SSPND_ACK != egtpMessageType) &&
#endif
        (NOTPRSNT != egEvnt->lclTeid.pres)))
    {
       /************************************************
        * if TEID is invalid then log error and return *
        ************************************************/
       if (EG_INVALID_TEID == teid)
       {
          EG_DBG_ERR(0, 0, (egp,"\n Local TEID is not Present in Event"));
          
          egEvnt->resultType = EGT_ERR_INV_TEID;

         /******************************************
          * Delete the Message Buffer and timer CB *
          ******************************************/
          EG_FREEMBUF (egEvnt->u.mBuf);
          EG_RETVALUE (RFAILED);
       }
    }
#ifdef EG_PIGGYBACK_SUPP
    else
    {
       isTransMsg = TRUE;
    }
#endif
 
   /*************************************************************
    ***************STEP 2: Check if MSB is set*******************
    *************************************************************/
    EG_IS_MSB_SET_IN_SEQ_NO(seqNo, isMsbSet);

   /*****************************************
    * if MSB is set in the Sequence Number  *
    * then use default Server Control block *
    *****************************************/
    if (TRUE == isMsbSet)
    {
       EG_DBG_INFO(0,0,(egp,"\n  MSB is set in Sequence No."));

#ifdef EG_MULTI_IP
       /***********************************************
        * eg007.201:  Get the default Tpt Server, from*
        * TSapCb provided by ServrCb, if not found    *
        * log error and return                        *
        ***********************************************/
        dfltSrvCb = serverCb->tSapCb->dftlTptSrv;
#else
        dfltSrvCb = egCb.tSapCb->dftlTptSrv;
#endif
        if (NULLP == dfltSrvCb)
        {
           EG_DBG_ERR(0, 0, (egp,"\n Default TPT Server is not present"));
          
          egEvnt->resultType = EGT_ERR_DFTL_SRV_NOT_PRSNT;
          /******************************************
           * Delete the Message Buffer and timer CB *
           ******************************************/
           EG_FREEMBUF (egEvnt->u.mBuf);
           EG_RETVALUE (RFAILED);
        }
    }
    else
    {
       /*************************************
        * MSB is not Set in Sequence Number *
        *************************************/
       EG_DBG_INFO(0,0, (egp, "\n MSB is not set in Sequence No. "));
    }

    /*************************************************
     * Check whether queing is required for message. *
     * If failed to get the Qeing required for the   *
     * message then  log error and return.           *
     *************************************************/
     /* eg012.201: messages from 64 to 70 need not be queued ,fix for ccpu00125389 */
     qRequired = egTptCheckQReqrd (isMsbSet, egtpMsgCat,egtpMessageType); 
     if(FAIL == qRequired)
     {
        egEvnt->resultType = EGT_ERR_REQ_FAIL;
       /******************************************
        * Delete the Message Buffer and timer CB *
        ******************************************/
        EG_FREEMBUF (egEvnt->u.mBuf);
        EG_RETVALUE (RFAILED);
     }

     /* Queing is required for Context Rsp */
     if(egtpMessageType == EGT_GTPC_MSG_CNTXT_RSP)
     {
        /********************************************************
         * For Context Rsp, Context Ack is expected as response *
         * So, this message needs to be queed                   *
         ********************************************************/
         egEvnt->t3.enb=1;
         egEvnt->t3.val=10;
         qRequired = TRUE;
     }

    /**************************************************************
     * If Queing is required for a message then Queue the message *
     * based on the MSB set in the Sequence Number. If MSB is set *
     * use the Default Server CB Other wise use normal Server CB  *
     **************************************************************/
     if (TRUE == qRequired )
     {
         if (TRUE == isMsbSet)
         {
             ret = egTptQueueMsgWthMsb (egEvnt, dfltSrvCb, 
                   &readMsgVals, egEvnt->u.mBuf);
         }
         else
         {
             ret = egTptQueueMsg (egEvnt, serverCb, &readMsgVals, 
                         egEvnt->u.mBuf);
         }
        /*********************************************************
         * Fail to put message in the Queue log error and return *
         *********************************************************/
         if (ret != ROK)
         {
            EG_DBG_ERR(0, 0, (egp,"\n Queueing of Message Failed"));
           
            egEvnt->resultType = EGT_ERR_REQ_FAIL;
           /******************************************
            * Delete the Message Buffer              *
            ******************************************/
            EG_FREEMBUF (egEvnt->u.mBuf);
            EG_RETVALUE (RFAILED);
         }
     }/* if (TRUE == qRequired ) */

#ifdef EG_PIGGYBACK_SUPP
     if (isTransMsg != TRUE)
     {
         teCCb = egGetTunnelCb(teid);   
         /* eg012.201: Added NULL check for teCCb */
         if(teCCb == NULLP)
         {
            /* The TEID provided in out of range */
            egEvnt->resultType = EGT_ERR_UNKNOWN_TEID_MSG;
            /******************************************
            * Delete the Message Buffer              *
            ******************************************/
            EG_FREEMBUF (egEvnt->u.mBuf);
            EG_RETVALUE (RFAILED);
         }
         if ((*teCCb)->piggyBackStatus == EG_PIGGYBACK_SEND)
#endif 
#ifdef EG_MULTI_IP
     /**********************************************************
      * eg007.201: derive TSapCb from ServrCb                  *
      **********************************************************/
#ifdef EG_PIGGYBACK_SUPP
         {
           /***************************************************
            *  Get Remote Address of piggyback response event *
            ***************************************************/
           egCpyCmTptAddr(&destAddr, &((*teCCb)->tempPiggyEvnt->remAddr));
           tmpPst.srcInst = (*teCCb)->tempPiggyEvnt->srvCb->tSapCb->pst.srcInst;
         }
         else
#endif
         {
            tmpPst.srcInst = serverCb->tSapCb->pst.srcInst;
         }
#else
#ifndef SS_MULTIPLE_PROCS 
#ifdef EG_PIGGYBACK_SUPP
         {
           /***************************************************
            *  Get Remote Address of piggyback response event *
            ***************************************************/
           egCpyCmTptAddr(&destAddr, &((*teCCb)->tempPiggyEvnt->remAddr));
           tmpPst.srcInst = (*teCCb)->tempPiggyEvnt->srvCb->tSapCb->pst.srcInst;
         }
         else
#endif
         {
            tmpPst.srcInst = egCb.tSapCb->pst.srcInst;
         }
#else
#ifdef EG_PIGGYBACK_SUPP
         {
           /***************************************************
            *  Get Remote Address of piggyback response event *
            ***************************************************/
           egCpyCmTptAddr(&destAddr, &((*teCCb)->tempPiggyEvnt->remAddr));
           tmpPst.srcInst = (*teCCb)->tempPiggyEvnt->srvCb->tSapCb->pst.srcInst;
         }
         else
#endif
         {
            tmpPst.srcInst = egCbPtr->tSapCb->pst.srcInst;
         }
#endif /* SS_MULTIPLE_PROCS */
#endif /* EG_MULTI_IP */
     /* eg010.201:Based on piggyBack status,send/buffer piggyback messag */
#ifdef EG_PIGGYBACK_SUPP
        if((*teCCb)->piggyBackStatus == EG_PIGGYBACK_QUEUE)
        {
           (*teCCb)->tempPiggyEvnt = egEvnt;
           ret = ROK;
        }
        else if ((*teCCb)->piggyBackStatus == EG_PIGGYBACK_SEND)
        {
           SCatMsg(((*teCCb)->tempPiggyEvnt)->u.mBuf,egEvnt->u.mBuf,M1M2);
           SPutMsg(egEvnt->u.mBuf);
           egEvnt->u.mBuf = ((*teCCb)->tempPiggyEvnt)->u.mBuf; 
           ((*teCCb)->tempPiggyEvnt)->u.mBuf = NULLP;
        }
        if ((*teCCb)->piggyBackStatus != EG_PIGGYBACK_QUEUE)
        {
           ret = egTptSendMsg(serverCb, &destAddr, egEvnt->u.mBuf, &tmpPst);
        }
     }/* if isitPbMsg ends here */
     else
     {
        /* ccpu00134942 : for testing. need to put proper fix here */
        tmpPst.srcInst = serverCb->tSapCb->pst.srcInst;
        egCpyCmTptAddr(&destAddr, &(egEvnt->remAddr));
        ret = egTptSendMsg(serverCb, &destAddr, egEvnt->u.mBuf, &tmpPst);
     }
#else
     /* ccpu00134942 : for testing. need to put proper fix here */
     tmpPst.srcInst = serverCb->tSapCb->pst.srcInst;
     egCpyCmTptAddr(&destAddr, &(egEvnt->remAddr));
     ret = egTptSendMsg(serverCb, &destAddr, egEvnt->u.mBuf, &tmpPst);
#endif

    /**********************************************************
     * If we fail to send the message to peer then we need to *
     * remove queued EGTP Message and stop the retransmission *
     * timer for this EGTP message.                           *
     **********************************************************/
     if (ret != ROK)
     {
        egEvnt->resultType = EGT_ERR_MSG_SNDING_FAIL;
        EG_DBG_ERR(0, 0, (egp,"\n Sending of Message Failed"));

        if (TRUE == isMsbSet)
        {
          /*************************************************
           * Remove Message from Queue only if the message *
           * is of REQUEST Type or of COMMAND Type         *
           *************************************************/
           if (EG_GTPC_MSGCAT_REQUEST == egtpMsgCat ||
                 (EG_GTPC_MSGCAT_COMMAND == egtpMsgCat))
           {
              (void) egTptQueueMsgRmvWthMsb (dfltSrvCb, teid, seqNo, FALSE);
           }

        }
        else
        {
          /*************************************************
           * Remove Message from Queue only if the message *
           * is of REQUEST Type or of NOTIFICATION Type    *
           *************************************************/
           if (EG_GTPC_MSGCAT_REQUEST == egtpMsgCat ||
                 (EG_GTPC_MSGCAT_NOTFN == egtpMsgCat) || 
                 (egtpMessageType == EGT_GTPC_MSG_CNTXT_RSP))
           {
#ifndef EGTP_C_PTH_MGT
              (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE);
#else
              (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE,NULLP);
#endif
           }
        }

        /******************************************
         * Delete the Message Buffer               *
         ******************************************/
#ifdef EG_PIGGYBACK_SUPPORT
       if (TRUE != isTransMsg)
       {
          if (NULL != (*teCCb)->tempPiggyEvnt)
          {
            egCmFreeEvent((*teCCb)->tempPiggyEvnt);
            (*teCCb)->tempPiggyEvnt = NULL;
          }
          (*teCCb)->piggyBackStatus = EG_PIGGYBACK_NONE;
       }
#endif
        EG_FREEMBUF (egEvnt->u.mBuf);

     }/* Sending of Msg failed */
     else
     {
        /**********************************************
         *  Message is successfully send to Peer      *
         **********************************************/

        /**********************************************
         * Update the Message Transmission Statistics *
         **********************************************/
#ifdef EG_MULTI_IP
        /**********************************************************
         * eg007.201: derive TSapCb from ServrCb                  *
         **********************************************************/
        egUpdateTxStatistics(egtpMessageType, &(serverCb->tSapCb->sts));

        /*****************************************************
         * eg009.201 -Updating the Statistics with upper sap *
         *****************************************************/
        egUpdateTxStatistics(egtpMessageType, &(serverCb->uSapCb->sts));
#else
        egUpdateTxStatistics(egtpMessageType, &(egCb.tSapCb->sts));
#endif

        /******************************************
         * Delete the Message Buffer               *
         ******************************************/
#ifdef EG_PIGGYBACK_SUPP
        if(TRUE == isTransMsg)
        {
           EG_FREEMBUF (egEvnt->u.mBuf);
           /**************************************************************            
            * Free the Event, only in case of success, for failure cases *
            * it will be freed by tunnel module                          *
            * ************************************************************/
           egCmFreeEvent(egEvnt);
        }
        else
        {
           if ((*teCCb)->piggyBackStatus != EG_PIGGYBACK_QUEUE )
           {
              EG_FREEMBUF (egEvnt->u.mBuf);
              /**************************************************************            
               * Free the Event, only in case of success, for failure cases *
               * it will be freed by tunnel module                          *
               * ************************************************************/
              egCmFreeEvent(egEvnt);
               /* check if piggybacked, free piggybacked response Evnt */
               if (NULL != (*teCCb)->tempPiggyEvnt)
              {
                  egCmFreeEvent((*teCCb)->tempPiggyEvnt);
                  (*teCCb)->tempPiggyEvnt = NULL;
              }
              (*teCCb)->piggyBackStatus = EG_PIGGYBACK_NONE;
           }
        }
#else
           EG_FREEMBUF (egEvnt->u.mBuf);
           /**************************************************************
           * Free the Event, only in case of success, for failure cases *
           * it will be freed by tunnel module                          *
           *************************************************************/
           egCmFreeEvent(egEvnt);
#endif
     }
     EG_RETVALUE (ret);
}
#endif
/**********************************************************************
*       Fun:   egTptSendMsg
*
*       Desc:  This function is used to send a EGTP Message.
*
*       Ret:   ROK   on success
*              RFAILED on error
*
*       Notes:
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptSendMsg
(
EgTptSrvCb     *serverCb,   /* Server Connection Control Block (SCB) */
CmTptAddr      *destAddr,   /* Destination IP-address/port           */
Buffer         *mBuf,        /* Data buffer to be sent                */
Pst            *pst
)
#else
PUBLIC S16 egTptSendMsg (serverCb, destAddr, mBuf, pst)
EgTptSrvCb     *serverCb;   /* Server Connection Control Block (SCB) */
CmTptAddr      *destAddr;   /* Destination IP-address/port           */
Buffer         *mBuf;       /* Data buffer to be sent                */
Pst            *pst;
#endif
{
   S16           ret = RFAILED;
   Buffer        *sendBuf = NULLP;

   EG_TRC2(egTptSendMsg);

   /******************************************************
    * Make a copy (i.e, add reference) of message buffer *
    ******************************************************/
#ifdef EG_MULTI_IP
   /**********************************************************
    * eg007.201: derive TSapCb from ServrCb                  *
    **********************************************************/
    ret = SAddMsgRef (mBuf, serverCb->tSapCb->pst.region,
                      serverCb->tSapCb->pst.pool, &sendBuf);
#else
    ret = SAddMsgRef (mBuf, egCb.tSapCb->pst.region,
                      egCb.tSapCb->pst.pool, &sendBuf);
#endif

   /****************************************************
    * fial to create Message copy log error and return *
    ****************************************************/
    if (ret != ROK)
    {
       EG_DBG_FATAL(0,0,(egp,"SendMsg AddMsgRef Failed "));
       EG_RETVALUE (RFAILED);
    }
 
   /***********************************************
    * Send the Data request to the remove element *
    ***********************************************/
    ret = egTptUdpServerDatReq (serverCb, destAddr, sendBuf, pst);

   /*************************************
    * Fail to send Message then delete  *
    * message references and clean up   *
    *************************************/
    if (ret != ROK)
    {
       EG_DBG_ERR(0,0,(egp,"DatReq Failed"));
       EG_FREEMBUF(sendBuf); 
    }
  EG_RETVALUE (ret);
}

/**********************************************************************
*       Fun:   egTptUdpServerDatReq
*
*       Desc:  This function sends data using UDP server  connection
*              to remote element.
*
*       Ret:   ROK   on success
*              RFAILED on error
*
*       Notes:
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptUdpServerDatReq
(
EgTptSrvCb     *serverCb,   /* Server Connection Control Block (SCB) */
CmTptAddr      *destAddr,             /* Destination IP-address/port */
Buffer         *mBuf,                       /* Data buffer to be sent */
Pst      *pst
)
#else
PUBLIC S16 egTptUdpServerDatReq (serverCb, destAddr, mBuf, pst)
EgTptSrvCb     *serverCb;   /* Server Connection Control Block (SCB) */
CmTptAddr      *destAddr;             /* Destination IP-address/port */
Buffer         *mBuf;                      /* Data buffer to be sent */
Pst      *pst;
#endif
{
   EgTSapCb     *tSapCb;            /* Transport SAP Control block */
   CmIpHdrParm  ipHdrParm;           /* IP Header Parameter Holder */
   CmTptParam   tPar;               /* Transport Parametere holder */

   /* Store the pst SrcInst and use it while sending data 
    * to below layer */
   Inst         tempInst;

   EG_TRC2(egTptUdpServerDatReq);

   (Void)cmMemset ((U8 *)&ipHdrParm, 0, sizeof (CmIpHdrParm));

#ifdef EG_MULTI_IP
  /**********************************************************
   * eg007.201: Get Transport SAP Control block from ServrCb*
   **********************************************************/
   tSapCb = serverCb->tSapCb;
#else
   tSapCb = egCb.tSapCb;
#endif

  /***********************************************
   * Check TSAP State (it should be avalilable ) *
   ***********************************************/
   if ((tSapCb == NULLP) || (tSapCb->state != LEG_TSAP_BNDENA))
   {
      EG_DBG_ERR(0,0,(egp,"\n TSap State not bind"));
      EG_RETVALUE (RFAILED);
   }

  /*******************************************************
   * If Server connection NOT active, log error and fail *
   *******************************************************/
   if (serverCb->state != LEG_TPTSRV_ENA)
   {
      EG_DBG_ERR(0,0,(egp,"\n Server State not Enabled"));
      EG_RETVALUE (RFAILED);
   }

   /*************************************************
    * Server connection is active, send the message *
    * Update the TUCL Release Specific parameters   *
    *************************************************
    * Now start with TUCL REL 1.4 Header parameter  *
    *************************************************/
    tPar.type = CM_TPTPARAM_NOTPRSNT;

   /****************************************
    * Update TUCL REL 1.3 Header parameter *
    ****************************************/
   ipHdrParm.type = CM_HDRPARM_NOTPRSNT;

   /*************************************************
   *  Check whether the trc is enabled for incoming *
   *  Messages                                      * 
   *************************************************/
   if ( tSapCb->trcMask & LEG_TRC_EVENT_TX)
   {
      /*************************************************
       * eg009.201: added TSapCb to function call      *
       * ***********************************************/
      egGenTrcInd(tSapCb,STTSAP, &serverCb->cfg.tptAddr, destAddr, LEG_TRC_EVENT_TX, mBuf);
   }

   /****************************
    * Now Send message to TUCL *
    ****************************/
   /*  Store the pst SrcInst and use it while sending data 
    * to below layer */
    tempInst = pst->srcInst;
    cmMemcpy((U8*)pst, (U8*)&tSapCb->pst, sizeof(Pst));
    pst->srcEnt = ENTEG;
    pst->srcInst = tempInst;
    EgLiHitUDatReq ( pst,
                     tSapCb->cfg.spId,
                     serverCb->spConnId,
                     destAddr,
                     &serverCb->cfg.tptAddr,
                     &ipHdrParm,
                     &tPar,
                     mBuf);

  EG_RETVALUE (ROK);
} 
#ifdef EGTP_C
/**********************************************************************
*       Fun:   egTptQueueMsg
*
*       Desc:  This function Queue the Outgoing message based on 
*              Sequence Number of messages.
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
**********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptQueueMsg
(
EgEvnt      *egEvnt,      /* Encoded Message to be sent*/
EgTptSrvCb    *serverCb,  /* Server connection that received message */
EgPartialMsg  *partialMsg,/* Partial Decoded Message                 */
Buffer        *mBuf       /* Received message                        */
)
#else
PUBLIC S16 egTptQueueMsg (egEvnt, serverCb, partialMsg, mBuf)
EgEvnt      *egEvnt;      /* Encoded Message to be sent*/
EgTptSrvCb    *serverCb;  /* Server connection that received message */
EgPartialMsg  *partialMsg;/* Partial Decoded Message                 */
Buffer        *mBuf;      /* Received message                        */
#endif
{
   S16            ret = RFAILED;        /* Stores Function Return Values */
   EgSeqNoCb     *seqNoCb = NULLP;  /* Seq. Number Control block */
   EgTptReTmrCb  *tptReTmrCb = NULLP; /* Retransmition Timer CB */

   EG_TRC2(egTptQueueMsg);

    EG_DBG_INFO(0,0,
        (egp, "\n Queueing the Message with Seq No(%ld).", partialMsg->seqNo));
 
   /********************************************
    * Allocate a seq. Number Control block. If *
    * Unable to allocate log error and return  *
    ********************************************/
    EG_ALLOC(&seqNoCb, sizeof(EgSeqNoCb));
    if (seqNoCb == NULLP)
    {
       EG_DBG_ERR(0, 0, 
        (egp,"Failed to Allocate the memory for seqNoCb"));
       EG_RETVALUE(EGT_GTPC_CAUSE_NO_RES_AVAIL);
    }
 
   /***********************************************
    * Initialize the allocated Sequence CB with   *
    * Seq. Num, Message Type and Message Category *
    ***********************************************/
    seqNoCb->seqNo = partialMsg->seqNo;
    seqNoCb->msgType = partialMsg->msgType;
    seqNoCb->msgCat = partialMsg->msgCat;
    seqNoCb->teid = partialMsg->teid;
#ifndef EG_MULTI_IP
   seqNoCb->intfType = (EgtIntf)egEvnt->intfType; /* Needs to be checked */
#endif

   /**************************************************
    * eg006.201: trasaction id into Seq Cb   
   **************************************************/
   if(egEvnt->transId.pres == PRSNT_NODEF)
   {
      seqNoCb->transId = egEvnt->transId.val;
   }
   else
   {
      seqNoCb->transId = 0;
   }

/* eg004.201 suConnId updated for updating ReTx CB in seqNoCb for PSF*/
#ifdef HW
    seqNoCb->suConnId = serverCb->suConnId;
#endif
    ret = SCpyMsgMsg(mBuf, egCb.init.region, egCb.init.pool,
                        &seqNoCb->mBuf);
    if ( ret != ROK)
    {
        EG_DBG_ERR(0, 0,
             (egp,"\n Failed to copy the mBuf to seqNoCb"));
       EG_RETVALUE(ret);
    }
 
    /***************************************************
     * Initialize the Transport Re-transmission CB     *
     ***************************************************/
    tptReTmrCb = &seqNoCb->tptReTmrCb;

    tptReTmrCb->tptSrvCb = serverCb;
    /***************************************************
     * eg006.201: Msgtype to identify the retransmit msg 
     ***************************************************/
    /* ccpu00135389 : g++ compilation fix */
    tptReTmrCb->msgType = (EgtMsgType)egEvnt->msgType;

    tptReTmrCb->seqNo = seqNoCb->seqNo;
    tptReTmrCb->teid = seqNoCb->teid;
    egCpyCmTptAddr(&tptReTmrCb->destAddr, &(egEvnt->remAddr));
    tptReTmrCb->msgRetxRetryCnt = egCb.genCfg.n3Requests;
    tptReTmrCb->t3.enb = egEvnt->t3.enb;
    tptReTmrCb->t3.val = egEvnt->t3.val;


    /************************
     * Initialize the Timer *
     ************************/
    egCmInitTimer (&tptReTmrCb->reTransTmrNode);

    /*****************************************************
    * Insert Sequence No. Control Block into hash list. *
    * Fail to insert should log error and return.       *
    *****************************************************/
    ret = cmHashListInsert(&serverCb->msgHlCp, 
          (PTR) seqNoCb,
          (U8 *) &seqNoCb->seqNo,
          sizeof(U32));
    if (ret != ROK)
    {
       EG_DBG_ERR(0, 0, (egp,"\n Failed to insert the Seq No. CB "));
       EG_FREEMBUF (seqNoCb->mBuf);
       EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
       EG_RETVALUE(RFAILED);
    }
 
    /********************************************************
     * Message is Queued now start the Retransmission timer *
     ********************************************************/
    if (egEvnt->t3.enb && egEvnt->t3.val)
    {
    ret = egSchedTmr(tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_START,
          egEvnt->t3.val );

    /*******************************************************
     * If Failed to start the Retransmission timer then we *
     * we need to remove Queued EGTP messages and return.  *
     *******************************************************/
    if (ret != ROK)
    {
       EG_DBG_ERR(0, 0, (egp,"\n  Starting of Re-Trans Timer Failed"));
#ifndef EGTP_C_PTH_MGT
       (void) egTptQueueMsgRmv (serverCb, seqNoCb->seqNo, FALSE);
#else
       (void) egTptQueueMsgRmv (serverCb, seqNoCb->seqNo, FALSE, NULLP);
#endif

       egEvnt->resultType = EGT_ERR_TIMER_FAILED;
       EG_RETVALUE (RFAILED);
    }
    }
/*eg004.201 Send run time update for seqNoCb and updating ReTx CB*/
#ifdef HW
   HW_ADD_MAPPING(HW_EG_SEQCB, seqNoCb);
   HW_DBG_INFO(0,0, (hwp, "\n PSF Addmapping called for CbType=[%d],"
                 " TeidValue=[%lu], Sequence Value=[%lu]",
                 HW_EG_TEIDCB,seqNoCb->teid,seqNoCb->seqNo));

   HW_RTUPD_ADD(HW_EG_SEQCB, seqNoCb);
   HW_UPD_PEER();

   HW_DBG_INFO(0,0, (hwp, "\n PSF RuntimeUpdate with Action ADD called for "
                    "CbType=[%d], TeidValue=[%lu], Sequence Value=[%lu]",
                    HW_EG_TEIDCB,seqNoCb->teid,seqNoCb->seqNo));

   /* Send mbuf associated with SeqCb to support ReTx@STANDBY node*/
   if(hwCb.cfg.s.genCfg.retransSuppt)
   {
      /*
       * ReTx message update to be sent to STANDBY when tunnel
       * in CONNECTED state
       */
      EgtTunnState tunnState;
      ret = egTunnState(tptReTmrCb->teid,&tunnState);
      if(tunnState == EGT_TUN_STATE_CONNECTED)
      {
         ret = hwSendReTxBuff(tptReTmrCb, serverCb, mBuf);
      }/* end of if((tunnState == EGT_TUN_STATE_CONNECTED)) */
   }/* end of if(hwCb.cfg.s.genCfg.retransSuppt)*/
#endif /* end of HW */

    EG_RETVALUE(ROK);
}/* end of egTptQueueMsg() */

/**********************************************************************
*       Fun:   egTptQueueMsgRmv
*
*       Desc:  This function removes the stored Message based on 
*              Seq. No.
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef EGTP_C_PTH_MGT
#ifdef ANSI
PUBLIC S16 egTptQueueMsgRmv
(
EgTptSrvCb     *serverCb, /* Server connection that received message */
U32             seqNo,                /* Seq No for Received message */
Bool            isSendInd,    /* To send the Error Indication to PCM */
CmTptAddr      *destAddr
)
#else
PUBLIC S16 egTptQueueMsgRmv (serverCb, seqNo, isSendInd,destAddr)
EgTptSrvCb     *serverCb; /* Server connection that received message */
U32             seqNo;                /* Seq No. of Received message */
Bool            isSendInd;    /* To send the Error Indication to PCM */
CmTptAddr      *destAddr;
#endif
#else
#ifdef ANSI
PUBLIC S16 egTptQueueMsgRmv
(
EgTptSrvCb     *serverCb, /* Server connection that received message */
U32             seqNo,                /* Seq No for Received message */
Bool            isSendInd     /* To send the Error Indication to PCM */
)
#else
PUBLIC S16 egTptQueueMsgRmv (serverCb, seqNo, isSendInd)
EgTptSrvCb     *serverCb; /* Server connection that received message */
U32             seqNo;                /* Seq No. of Received message */
Bool            isSendInd;    /* To send the Error Indication to PCM */
#endif
#endif
{

   S16            ret=RFAILED;          /* return value of function */
   EgSeqNoCb     *seqNoCb = NULLP;     /* Pointer to Seq. Number CB */
   EgEvnt        *event = NULLP;
   /* ccpu00135369 */
#ifdef EGTP_C_PTH_MGT
   EgUSapCb      *uSapCb;
   EgDestCb      *destCb = NULLP;
   EgPathStatus  *status;
#endif

   EG_TRC2(egTptQueueMsgRmv);

   /********************************************************
    * Retrieve Sequence Number CB for the received Sequence Number  *
    ********************************************************/
    ret = cmHashListFind(&serverCb->msgHlCp, 
                         (U8 *) &seqNo, sizeof(U32), 
                          0, (PTR *)&seqNoCb); 

   /********************************************************
    * If unable to retrive hash list, log error and return *
    ********************************************************/
    if ((ret != ROK) || (seqNoCb == NULLP))
    {
       EG_DBG_INFO(0, 0, (egp,"\n Failed to get the Seq No(%ld). CB ", seqNo));
       EG_RETVALUE(ROK);
    }

   /* Send the Error Indication to Tunnel Module */
   if (isSendInd == TRUE)
   {
#ifdef EGTP_C_PTH_MGT
      if(( seqNoCb->msgType == EGT_GTPC_MSG_ECHO_REQ) && (destAddr != NULLP))
      {
         /*-- eg006.201: Sending alarm to Layer manager */
         EgUsta         sta;  /* unsolicited status diagnostics structure */ 

         /* eg006.201: Use macro to set value of port to zero */
         EG_SET_PORT((*destAddr), EG_ZERO);

#ifdef EG_MULTI_IP
         /**********************************************************
          * eg007.201: Get Transport SAP Control block from ServrCb*
          **********************************************************/
         ret = cmHashListFind(&serverCb->tSapCb->dstCbList, (U8 *)(destAddr),\
               sizeof(CmTptAddr),0,(PTR *)&destCb);
#else
         ret = cmHashListFind(&egCb.dstCbList, (U8 *)(destAddr),\
               sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif
 
         /***********************************************************************
          * eg009.201 : Destination is not present in destCbList, indicates that*
          * destination is deleted from the list, present an error & return     * 
          * *********************************************************************/
          if(destCb == NULLP)
          {
             EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Destination IP not avaialble"));

             cmMemset( (U8* )&sta, 0, sizeof(EgUsta));
             sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
             egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), destAddr);
             egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
                   LEG_CAUSE_DST_IP_DEL, &sta.dgn);

             EG_RETVALUE (RFAILED);
          }
 

          /*****************************************************
            * eg009.201 : Send Indication & alarm only if the destination is marked 
            * Reachable. For a destination marked unreachable, the indications would 
            * already have reached the application                                  
           * ***********************************************/
           if (!destCb->destReachable)
           {
              EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Destination IP not reachable"));
           }
           else
           {
              destCb->destReachable = FALSE;
              EG_ALLOC(&status, sizeof(EgPathStatus));
              /* eg012.201: added memory alloc failure */
              if (NULLP == status)
              {
                  EG_RETVALUE(RFAILED);
              }
              /*-- eg006.201: Corrected Failure type to PATH */
              status->pathInfo.staType = EGT_STATYPE_PATH;
              status->pathInfo.s.pathState.pres = PRSNT_NODEF;
              status->pathInfo.s.pathState.val  = EGT_STA_PATH_FAILURE;
              egCpyCmTptAddr(&(status->pathInfo.u.localAddr), destAddr);
              status->cause        = EGT_CAUSE_N3_REQ_EXCEEDED;
              status->seqNum.pres  = PRSNT_NODEF;
              status->seqNum.val   = seqNoCb->seqNo;
              status->message.pres = PRSNT_NODEF;
              status->message.val  = EGT_GTPC_MSG_ECHO_REQ;
              status->dgn.pres     = NOTPRSNT;
              /* ccpu00135369 : mavenir*/
              uSapCb = egGetUsapFrmIntf (seqNoCb->intfType);

         if (NULLP == uSapCb)
              {
                  EG_RETVALUE(RFAILED);
              }

              EgUiEgtStaInd(&(uSapCb->pst),uSapCb->suId, \
                    EGT_STA_PATH_FAILURE, status);

              EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Destination IP not reachable"));

              /*-- eg006.201: Send alarm to Layer indicating the path failure */
              cmMemset( (U8* )&sta, 0, sizeof(EgUsta));
              sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
              egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), destAddr);
              egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
                    LEG_CAUSE_DST_IP_NOT_REACHABLE, &sta.dgn);
           }

      }
      else
      {
         EG_ALLOCEVNT (&event);
         if (event != NULLP)
         {
            cmMemset( (U8* )event, 0, sizeof(EgEvnt));
            event->seqNumber = seqNoCb->seqNo;
            event->msgType = seqNoCb->msgType;


            /**********************************************
             * eg009.201: fill in transId & Teid from SeqCb 
             * *********************************************/
            if(seqNoCb->transId != 0 )
               event->transId.pres = PRSNT_NODEF;
            event->transId.val     = seqNoCb->transId;

            event->lclTeid.val     = seqNoCb->teid;

            /*-- eg006.201: update resultType with Timer failure*/
            event->resultType = EGT_ERR_MAX_RETRY_EXHAUSTED; 

            /* Event will be freed by Tunnel Module */
            (Void) egTunIndicate (serverCb->uSapCb, seqNoCb->teid, event);
         }
         else
         {
            EG_DBG_ERR (0, 0, (egp, "\n Allocation of event failed,\
                       Couldn't indicate to Tunnel"));
            /*-- eg006.201 : Return Error for Alloc failure */
            EG_RETVALUE (RFAILED);
         }
      }
#else
      EG_ALLOCEVNT (&event);
 
      if (event != NULLP)
      {

         cmMemset( (U8* )event, 0, sizeof(EgEvnt));

         event->seqNumber = seqNoCb->seqNo;
         event->msgType = seqNoCb->msgType;
         
         /*-- eg006.201: fill in transId from SeqCb */
         if(seqNoCb->transId != 0 )
            event->transId.pres = PRSNT_NODEF;
         event->transId.val     = seqNoCb->transId;

         /***********************************************
          *eg009.201: fill in transId & Teid from SeqCb * 
          * *********************************************/
         event->lclTeid.val     = seqNoCb->teid;

         /*-- eg007.201: update resultType with Timer failure*/
         event->resultType = EGT_ERR_MAX_RETRY_EXHAUSTED; 

         /* Event will be freed by Tunnel Module */
         (Void) egTunIndicate (serverCb->uSapCb, seqNoCb->teid, event);
      }
      else
      {
         EG_DBG_ERR (0, 0, 
         (egp, 
         "\n Allocation of event failed, Couldn't indicate to Tunnel"));

         /*-- eg006.201 : Return Error for Alloc failure */
         EG_RETVALUE (RFAILED);
      }
#endif
   }

   EG_DBG_INFO (0, 0, 
         (egp, "\n Continue the Remaining Processing "));

   /********************************************
    * Delete this Server CB from the Hash List *
    ********************************************/
    cmHashListDelete(&serverCb->msgHlCp, (PTR)seqNoCb);

    /**********************************
     * Stop the Re-transmission Timer *
     **********************************/
    egSchedTmr(&seqNoCb->tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_STOP, NOTUSED);
/*eg004.201 Send run time update for seqNoCb when ReTx message removed from ReTx queue*/
#ifdef HW
   HW_RTUPD_DEL(HW_EG_SEQCB, seqNoCb);
   HW_DBG_INFO(0,0, (hwp, "\n PSF RuntimeUpdate with Action Delete "
                    "CbType=[%d], TeidValue=[%lu], Sequence Value=[%lu]",
                    HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));

   HW_DEL_MAPPING(HW_EG_SEQCB, seqNoCb);

   HW_DBG_INFO(0,0, (hwp, "\n PSF Delete Mapping CbType=[%d],"
               " TeidValue=[%lu], Sequence Value=[%lu]",
               HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));

#endif


   /*****************************
    * Free the Server CB memory *
    *****************************/
    if (NULLP != seqNoCb)
    {
       EG_DBG_INFO(0, 0, 
        (egp,"\n Freeing the memory allocated for SeqNoCb "));
       EG_FREEMBUF (seqNoCb->mBuf);
       EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
    }

  EG_RETVALUE(ROK);
}

/**********************************************************************
*       Fun:   egTptQueueMsgWthMsb
*
*       Desc:  This function Queue the Outgoing message whose MSB is
*              set in Sequence Number. 
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
/*eg004.201 Warnings fixed for eGTP-C*/
#ifdef ANSI
PUBLIC S16 egTptQueueMsgWthMsb
(
EgEvnt        *egEvnt,
EgTptSrvCb    *dfltSrvCb,  /* Server connection of received message */
EgPartialMsg  *partialMsg,               /* Partial Decoded Message */
Buffer        *mBuf                      /* Received message buffer */
)
#else
PUBLIC S16 egTptQueueMsgWthMsb (egEvnt, dfltSrvCb, partialMsg, mBuf)
EgEvnt        *egEvnt;
EgTptSrvCb    *dfltSrvCb;  /* Server connection of received message */
EgPartialMsg  *partialMsg;               /* Partial Decoded Message */
Buffer        *mBuf;                     /* Received message buffer */
#endif
{
   S16            ret;             /* return value of the function */
   EgHashCb       *egTptTeidCb = NULLP;   /* Seq. Number Control Block list */
   U16            numHlBins;           /* Number of hash list bins */
   EgSeqNoCb      *seqNoCb = NULLP;  /* Seq. Number Congtrol Block */
   U32            teid;                       /* Tunnel Id of EGTP */
   U32            seqNo;             /* Seq. Numer of EGTP Message */
   EgtMsgCat      msgCat;                 /* EGTP Message Category */
   EgtMsgType     msgType;                    /* EGTP Message Type */
   EgTptReTmrCb  *tptReTmrCb = NULLP; /* Retransmition Timer CB */

    EG_TRC2 (egTptQueueMsgWthMsb);

   /************************************************
    * Get Tunnel Id, Sequence Number, Message Type *
    * and Message Category from Partial message    *
    ************************************************/
    teid = partialMsg->teid;
    seqNo = partialMsg->seqNo;
    msgType = partialMsg->msgType;
    msgCat = partialMsg->msgCat;

    EG_DBG_INFO(0,0, 
    (egp,"\n Queueing Message with MSB set in Seq No(%ld).", seqNo));

   /************************************************
    * Initialize with number of Hash Bins. Now try *
    * to locate the Tunnel Id in the hash List     *
    ************************************************/
    numHlBins = EG_MAX_NUM_HASHBIN_SEQNO;
    ret = cmHashListFind(&dfltSrvCb->teidMsgHlCp, 
                         (U8 *) &teid, sizeof(U32), 
                          0, (PTR *)&egTptTeidCb); 

   /****************************************************************  
    * If Sequence Number CB list is NULL then we consider this as  * 
    * First Command Message for TEID. Create the Seq No Hash List. *
    * if Sequence Number CB list is not NULL, it means we have the *
    * Sequecne Control CB so initilize the Control Block  with     *
    * a) EGTP Message Seq. Num,                                    *
    * b) EGTP Message Type                                         *
    * c) EGTP Message Category                                     *
    * d) Tunnel End Point Id                                       *
    * e) Message Buffer                                            *
    ****************************************************************/
    if (NULLP == egTptTeidCb)
    {
       EG_DBG_INFO(0,0, 
             (egp, "\n First Msg for TEID, creating Seq No. hash list"));

       /********************************************************
        * Allocate egTptTeidCb, for handling Sequence Num Queue *
        * If unable to allocate then log error and return      *
        ********************************************************/
       EG_ALLOC(&egTptTeidCb, sizeof(EgHashCb));
       if (egTptTeidCb == NULLP)
       {
          EG_DBG_ERR(0, 0, 
                (egp,"\n Failed to Allocate the memory for egTptTeidCb"));
          EG_RETVALUE(EGT_GTPC_CAUSE_NO_RES_AVAIL);
       }

       /****************************
        * Initialize the Hash List *
        ****************************/
       ret = cmHashListInit(&egTptTeidCb->seqNoCbLst,
             numHlBins,
             (U16) 0, 
             FALSE,
             CM_HASH_KEYTYPE_CONID,
             egCb.init.region, egCb.init.pool);

       /*****************************************
        * If failed to Initialize the Hash List *
        * then log error and return from here   *
        *****************************************/
       if (ret != ROK)
       {
          EG_DBG_ERR(0, 0,
                (egp,"\n Failed to Init the Seq No. hash List"));
          EG_FREE(egTptTeidCb, sizeof(EgHashCb));
          EG_RETVALUE(RFAILED);
       }

       /*****************************************
        * Now Allocate a seq. Num Control block *
        *****************************************/
       EG_ALLOC(&seqNoCb, sizeof(EgSeqNoCb));

       /*******************************************
        * If failed to Allocate the Control Block *
        * then log error and return from here     *
        *******************************************/
       if (seqNoCb == NULLP)
       {
          EG_DBG_ERR(0, 0, (egp,"\n Failed to Allocate Memory"));
          cmHashListDeinit (&egTptTeidCb->seqNoCbLst);
          EG_FREE(egTptTeidCb, sizeof(EgHashCb));
          EG_RETVALUE(EGT_GTPC_CAUSE_NO_RES_AVAIL);
       }

       /*******************************************
        * Allocation of Seq. Num CB is successful *
        * so initilize the Control Block.         *
        *******************************************/
       seqNoCb->seqNo = seqNo;
       seqNoCb->msgType = msgType;
       seqNoCb->msgCat = msgCat;
       seqNoCb->teid = teid;

       /*-- eg006.201: trasaction id into Seq Cb --*/
       if(egEvnt->transId.pres == PRSNT_NODEF)
       {
          seqNoCb->transId = egEvnt->transId.val;
       }
       else
       {
          seqNoCb->transId = 0;
       }

       ret = SCpyMsgMsg(mBuf, egCb.init.region, egCb.init.pool,
             &seqNoCb->mBuf);
       if (ret != ROK)
       {
          EG_DBG_ERR(0, 0,
                (egp,"\n Failed to copy the mBuf to seqNoCb"));
          EG_RETVALUE(ret);
       }


       /***************************************************
        * Initialize the Transport Re-transmission CB     *
        ***************************************************/
       tptReTmrCb = &seqNoCb->tptReTmrCb;
       /*-- eg006.201: Msgtype to identify the retransmit msg */
       /* ccpu00135389 : g++ compilation fix */
       tptReTmrCb->msgType = (EgtMsgType)egEvnt->msgType;

       tptReTmrCb->tptSrvCb = dfltSrvCb;
       tptReTmrCb->seqNo = seqNoCb->seqNo;
       tptReTmrCb->teid = seqNoCb->teid;
       egCpyCmTptAddr(&tptReTmrCb->destAddr, &(egEvnt->remAddr));
       tptReTmrCb->msgRetxRetryCnt = egCb.genCfg.n3Requests;
       tptReTmrCb->t3.enb = egEvnt->t3.enb;
       tptReTmrCb->t3.val = egEvnt->t3.val;

       /************************
        * Initialize the Timer *
        ************************/
       egCmInitTimer (&tptReTmrCb->reTransTmrNode);

       /*******************************************
        * Store the Outgoing message in hash list * 
        *******************************************/
       ret = cmHashListInsert(&egTptTeidCb->seqNoCbLst, 
             (PTR) seqNoCb,
             (U8 *) &seqNoCb->seqNo,
             sizeof(U32));

       /*********************************************
        * If unable to store in the Hash List then  *
        * De Init Hash List, Log error and Return   *
        *********************************************/
       if (ret != ROK)
       {
          EG_DBG_ERR(0, 0, (egp,"\n Failed to insert the Seq No. CB "));
          cmHashListDeinit (&egTptTeidCb->seqNoCbLst);
          EG_FREE(egTptTeidCb, sizeof(EgHashCb));
          EG_FREEMBUF (seqNoCb->mBuf);
          EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
          EG_RETVALUE(RFAILED);
       }

       egTptTeidCb->teid = teid;
       ret = cmHashListInsert(&dfltSrvCb->teidMsgHlCp, 
             (PTR)egTptTeidCb,(U8 *)&egTptTeidCb->teid, 
             sizeof(U32));

       /*********************************************
        * If unable to store in the Hash List then  *
        * De Init Hash List, Log error and Return   *
        *********************************************/
       if (ret != ROK)
       {
          EG_DBG_ERR(0, 0, (egp,"\n Failed to insert the Teid CB "));
          cmHashListDeinit (&egTptTeidCb->seqNoCbLst);
          EG_FREE(egTptTeidCb, sizeof(EgHashCb));
          EG_FREEMBUF (seqNoCb->mBuf);
          EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
          EG_RETVALUE(RFAILED);
       }
       

       /********************************************************
        * Message is Queued now start the Retransmission timer *
        ********************************************************/
       ret = egSchedTmr(tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_START,
             egEvnt->t3.val );

       /*******************************************************
        * If Failed to start the Retransmission timer then we *
        * we need to remove Queued EGTP messages and return.  *
        *******************************************************/
       if (ret != ROK)
       {
          EG_DBG_ERR(0, 0, (egp,"\n  Starting of Re-Trans Timer Failed"));
          (void) egTptQueueMsgRmvWthMsb (dfltSrvCb, seqNoCb->teid, 
                                         seqNoCb->seqNo, FALSE);

          egEvnt->resultType = EGT_ERR_TIMER_FAILED;
          EG_RETVALUE (RFAILED);
       }

    }
   else
   {
      /*******************************************
       * Seq Number Hash list is present, simply *
       * add the new Message in to the Hash List *
       *******************************************/
      EG_DBG_INFO(0,0, (egp, "\n Seq No. hash list present for TEID"));

      /******************************************
       *  Allocate Memory for new  Seq. Num CB. *
       *  If Failed then log error and return.  *
       ******************************************/
      EG_ALLOC(&seqNoCb, sizeof(EgSeqNoCb));
      if (seqNoCb == NULLP)
      {
         EG_RETVALUE(EGT_GTPC_CAUSE_NO_RES_AVAIL);
      }

      /*********************************************
       * Initialize Seq. Num CB with Msg Parmeters *
       *********************************************/
      seqNoCb->seqNo = seqNo;
      seqNoCb->msgType = msgType;
      seqNoCb->msgCat = msgCat;
      seqNoCb->teid = teid;
      ret = SCpyMsgMsg(mBuf, egCb.init.region, egCb.init.pool,
            &seqNoCb->mBuf);
      if ( ret != ROK)
      {
         EG_DBG_ERR(0, 0,
               (egp,"\n Failed to copy the mBuf to seqNoCb"));
         EG_RETVALUE(ret);
      }

      /***************************************************
       * Initialize the Transport Re-transmission CB     *
       ***************************************************/
      tptReTmrCb = &seqNoCb->tptReTmrCb;

      tptReTmrCb->tptSrvCb = dfltSrvCb;
      tptReTmrCb->seqNo = seqNoCb->seqNo;
      tptReTmrCb->teid = seqNoCb->teid;
      egCpyCmTptAddr(&tptReTmrCb->destAddr, &(egEvnt->remAddr));
      tptReTmrCb->msgRetxRetryCnt = egCb.genCfg.n3Requests;
      tptReTmrCb->t3.enb = egEvnt->t3.enb;
      tptReTmrCb->t3.val = egEvnt->t3.val;

      /************************
       * Initialize the Timer *
       ************************/
      egCmInitTimer (&tptReTmrCb->reTransTmrNode);

      /*******************************************
       * Store the Outgoing message in hash list *
       *******************************************/
      ret = cmHashListInsert(&egTptTeidCb->seqNoCbLst, 
            (PTR) seqNoCb,
            (U8 *) &seqNoCb->seqNo,
            sizeof(U32));
      if (ret != ROK)
      {
         EG_DBG_ERR(0, 0,
               (egp,"\n Failed to insert the Seq No. CB "));
         EG_FREEMBUF (seqNoCb->mBuf);
         EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
         EG_RETVALUE(RFAILED);
      }

      /********************************************************
       * Message is Queued now start the Retransmission timer *
       ********************************************************/
      if (egEvnt->t3.enb && egEvnt->t3.val)
      {
      ret = egSchedTmr(tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_START,
            egEvnt->t3.val );

         /******************************************************
       * If Failed to start the Retransmission timer then we *
       * we need to remove Queued EGTP messages and return.  *
         ******************************************************/
      if (ret != ROK)
      {
         EG_DBG_ERR(0, 0, (egp,"\n  Starting of Re-Trans Timer Failed"));
         (void) egTptQueueMsgRmvWthMsb (dfltSrvCb, seqNoCb->teid, 
                                        seqNoCb->seqNo, FALSE);
         egEvnt->resultType = EGT_ERR_TIMER_FAILED;
         EG_RETVALUE (RFAILED);
      }
      }
   } 
/* eg004.201 Send run time update for seqNoCb and updating ReTx CB to STANDBY
 * when a new message comes for a tunnel from the upper layer 
 */
#ifdef HW
   HW_ADD_MAPPING(HW_EG_SEQCB, seqNoCb);

   HW_DBG_INFO(0,0, (hwp, "\n PSF Addmapping CbType=[%d],"
                 " TeidValue=[%lu], Sequence Value=[%lu]",
                 HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));


   HW_RTUPD_ADD(HW_EG_SEQCB, seqNoCb);
   HW_UPD_PEER();

   HW_DBG_INFO(0,0, (hwp, "\n PSF RuntimeUpdate with Action ADD "
                    "CbType=[%d], TeidValue=[%lu], Sequence Value=[%lu]",
                    HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));

   /* Send mbuf associated with SeqCb to support ReTx@STANDBY node*/
   if(hwCb.cfg.s.genCfg.retransSuppt)
   {
      EgtTunnState tunnState;
      ret = egTunnState(tptReTmrCb->teid,&tunnState); 
      if(tunnState == EGT_TUN_STATE_CONNECTED)
      {
         ret = hwSendReTxBuff(tptReTmrCb, dfltSrvCb, mBuf);
      }/* end of if((tunnState == EGT_TUN_STATE_CONNECTED)) */
   }/* end of if(hwCb.cfg.s.genCfg.retransSuppt)*/
#endif
 EG_RETVALUE (ROK);
}/* end of egTptQueueMsgWthMsb()*/
/**********************************************************************
*       Fun:   egTptQueueMsgRmvWthMsb
*
*       Desc:  This function removes the stored Message based on 
*              Sequence number with MSB set.
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
/*eg004.201 Warnings fixed for eGTP-C by modifying the function PROTOTYPE
 *from PRIVATE to PUBlIC in the qualified
 */
#ifdef ANSI
PUBLIC S16 egTptQueueMsgRmvWthMsb
(
EgTptSrvCb     *dfltSrvCb,  /* Server connection of received message */
U32            teid,              /* Local TEID for Received message */
U32            seqNo,                /* Seq. Num of Received message */
Bool           isSendInd      /* To send the Error Indication to PCM */
)
#else
PUBLIC S16 egTptQueueMsgRmvWthMsb (dfltSrvCb, teid, seqNo,isSendInd)
EgTptSrvCb     *dfltSrvCb;  /* Server connection of received message */
U32            teid;              /* Local TEID for Received message */
U32            seqNo;                /* Seq. Num of Received message */
Bool           isSendInd;    /* To send the Error Indication to PCM */
#endif
{
    
   S16            ret;           /* Holds return value of functions */
   EgSeqNoCb      *seqNoCb = NULLP; /* Ptr to Seq.Num Control block */
   EgHashCb       *egTptTeidCb = NULLP;    /* Ptr to Seq.Num CB List */
   EgEvnt         *event=NULLP;

   EG_TRC2(egTptQueueMsgRmvWthMsb);

  /*****************************************
   * Locate the Tunnel Id in the Hash List *
   *****************************************/
   ret = cmHashListFind(&dfltSrvCb->teidMsgHlCp, 
                       (U8 *) &teid, sizeof(U32), 
                        0, (PTR *)&egTptTeidCb); 

  /*********************************************
   * Fail to locate? Then log error and return *   
   *********************************************/
   if ((ret != ROK) || (NULLP == egTptTeidCb)) 
   {
      EG_DBG_ERR(0, 0,
       (egp,"\n Failed to Locate TEID in hash List"));
      EG_RETVALUE(RFAILED);
   }

  /*******************************************
   * Locate the Seq. Number Cb in the Hash List *
   *******************************************/
   ret = cmHashListFind(&egTptTeidCb->seqNoCbLst, 
                        (U8 *) &seqNo, sizeof(U32), 
                         0, (PTR *)&seqNoCb); 

  /*********************************************
   * Fail to locate? Then log error and return *   
   *********************************************/
   if ((ret != ROK) || (seqNoCb == NULLP))
   {
      EG_DBG_INFO(0, 0,
       (egp,"\n Failed to Locate Seq. No in hash List"));
      EG_RETVALUE(ROK);
   }
   
   /* Send the Error Indication to Tunnel Module */
   if (isSendInd == TRUE)
   {
      EG_ALLOCEVNT (&event);
 
      if (event != NULLP)
      {
         cmMemset( (U8* )event, 0, sizeof(EgEvnt));

         event->seqNumber = seqNoCb->seqNo;
         event->msgType = seqNoCb->msgType;

         /************************************************
          * eg009.201: Fill in transId & Teid from SeqCb *
          * into Event structure                         *
          * **********************************************/
         if(seqNoCb->transId != 0 )
            event->transId.pres = PRSNT_NODEF;
         event->transId.val     = seqNoCb->transId;

         event->lclTeid.val     = seqNoCb->teid;

         /* eg013.201 : Added result type */
         event->resultType = EGT_ERR_MAX_RETRY_EXHAUSTED;

         /* Event will be freed by Tunnel Module */
         (Void) egTunIndicate (dfltSrvCb->uSapCb, teid, event);
      }
      else
      {
         EG_DBG_ERR (0, 0, 
         (egp, "\n Allocation of event failed, Couldn't indicate to Tunnel"));
      }
   }

   EG_DBG_INFO (0, 0, 
         (egp, "\n Continue the Remaining Processing "));

   /************************************************
    * Delete it from Hash List and free the memory *
    ************************************************/
    cmHashListDelete(&egTptTeidCb->seqNoCbLst, (PTR)seqNoCb);

    /**********************************
     * Stop the Re-transmission Timer *
     **********************************/
    egSchedTmr(&seqNoCb->tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_STOP, NOTUSED);

/*
 *eg004.201 Send run-time update for deleting seqNoCb when message with 
 *MSB removed from ReTx queue
 */
#ifdef HW
   HW_RTUPD_DEL(HW_EG_SEQCB, seqNoCb);
   HW_DBG_INFO(0,0, (hwp, "\n PSF RuntimeUpdate with Action Delete "
                    "CbType=[%d], TeidValue=[%lu], Sequence Value=[%lu]",
                    HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo)); 

   HW_DEL_MAPPING(HW_EG_SEQCB, seqNoCb);

   HW_DBG_INFO(0,0, (hwp, "\n PSF Delete Mapping CbType=[%d],"
                    " TeidValue=[%lu], Sequence Value=[%lu]",
                    HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));
#endif

    if (NULLP != seqNoCb)
    {
       EG_DBG_INFO(0, 0, 
       (egp,"\n Freeing the memory allocated for SeqNoCb "));
       EG_FREEMBUF (seqNoCb->mBuf);
       EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
    }

   /********************************************************
    * Check if any more messages are present in hash list  * 
    * If not then delete the Server CB List from the Hash  *
    * De-Initialize Seq. Num CB from Hash, Free the memory * 
    ********************************************************/
    if (egTptTeidCb->seqNoCbLst.nmbEnt == 0)
    {
       cmHashListDelete(&dfltSrvCb->teidMsgHlCp, (PTR)egTptTeidCb);
       cmHashListDeinit (&egTptTeidCb->seqNoCbLst);
       EG_FREE(egTptTeidCb, sizeof(EgHashCb));
    }

   EG_RETVALUE(ROK);
}

/**********************************************************************
*       Fun:   egTptValIncMsgWthMsb
*
*       Desc:  This function Validate whether incoming Request/Response
*            has corresponding Command/Request in Sequence Queue.
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptValIncMsgWthMsb
(
EgTptSrvCb     *dfltSrvCb,  /* Server connection of received message */
EgPartialMsg   *partialMsg                /* Partial Decoded Message */
)
#else
PUBLIC S16 egTptValIncMsgWthMsb (dfltSrvCb, partialMsg)
EgTptSrvCb     *dfltSrvCb;  /* Server connection of received message */
EgPartialMsg   *partialMsg;               /* Partial Decoded Message */
#endif
{
   S16           ret;                /* Holds Function return value */
   EgSeqNoCb     *seqNoCb;             /* Seq. Number Control Block */
   EgHashCb      *egTptTeidCb;            /* Seq. Number CB list ptr */
   U32           teid;                        /* Tunel End Point Id */      
   U32           seqNo;                      /* Message Seq. Number */
   EgtMsgType    msgType;                      /* EGTP Message Type */
   EgtMsgType    pairMsgType;             /* EGTP Message Pair Type */

   EG_TRC2 (egTptValIncMsgWthMsb);

   EG_DBG_INFO(0,0, 
   (egp, "\n Validating Incoming Message with MSB set in Seq No."));

  /********************************************************
   * Get Seq. Num, Msg Type and TEID from Partial message *
   ********************************************************/
   seqNo = partialMsg->seqNo;
   msgType = partialMsg->msgType;
   teid = partialMsg->teid;

  /*****************************************************
   * Locate the TEID in the Hash List. Fail to locate  *
   * then log the error and return from there          *
   *****************************************************/
   ret = cmHashListFind(&dfltSrvCb->teidMsgHlCp, 
         (U8 *) &teid, sizeof(U32), 
         0, (PTR *)&egTptTeidCb); 
   if ((ret != ROK) || (NULLP == egTptTeidCb)) 
   {
      EG_DBG_ERR(0, 0,
       (egp,"\n Failed to Locate TEID in hash List"));
      EG_RETVALUE(RFAILED);
   }
   
  /*****************************************************
   * Locate the Seq.Num in  Hash List. Fail to locate  *
   * then log the error and return from there          *
   *****************************************************/
   ret = cmHashListFind(&egTptTeidCb->seqNoCbLst, 
                        (U8 *) &seqNo, sizeof(U32), 
                         0, (PTR *)&seqNoCb); 
   if ((ret != ROK) || (seqNoCb == NULLP))
   {
      EG_DBG_ERR(0, 0,
       (egp,"\n Failed to Locate Seq. No in hash List"));
      EG_RETVALUE(RFAILED);
   }
   
  /************************************************
   * Get the Pair Message Type for stored Message *
   ************************************************/
   pairMsgType = egGetAssocMsg(seqNoCb->msgType);
   
   if (msgType != pairMsgType)
   {
      /************************************************
      * Since for the Fail ind the assocition is not  *
      * there, check whether the assocaition exists   *
      ************************************************/
      if ((ret = egChkFailInd((U8)msgType, (U8)seqNoCb->msgType)) != ROK)
      {
         EG_DBG_ERR(0, 0,
           (egp,"\n Expected Message(%d) however got Message(%d)", 
                    pairMsgType, msgType));
         EG_RETVALUE(RFAILED);
      }
   }
      
   /*******************************
    * Removed Queued EGTP Message *
    *******************************/
   (void) egTptQueueMsgRmvWthMsb (dfltSrvCb, teid, seqNo, FALSE);

   EG_RETVALUE (ROK);
}
#endif
/**********************************************************************
*       Fun:   egTptProcessTsapServers
*
*       Desc:  Processes all transport servers on specified TSAP. 
*             This function is used to open/close all transport-
*             servers on the given TSAP.
*
*       Ret:   If success, number of servers processed.
*              If failure, RFAILED.
*
*       File:  eg_tpt.c
***********************************************************************/

#ifdef ANSI
PUBLIC S16 egTptProcessTsapServers
(
EgTSapCb       *tsapCb,                /* TSAP control block        */
EgTptSrvAct    srvAction               /* Type of action to perform */
)
#else
PUBLIC S16 egTptProcessTsapServers (tsapCb, srvAction)
EgTSapCb       *tsapCb;                /* TSAP control block        */
EgTptSrvAct    srvAction;              /* Type of action to perform */
#endif
{
   U16            numSrv;              /* Number of servers         */
   EgTptSrvCb     *serverCb;           /* Server Control block Ptr  */
   U16            processed = 0;          
#ifdef EGTP_U
   EgTptSrvCb     *tempSrvCb;           /* Server Control block Ptr  */
   S16            retVal;
#endif /* EGTP_U */

   EG_TRC2(egTptProcessTsapServers);


   /********************************************
    * Check if any Tpt Server is present       *
    ********************************************/
   if (!tsapCb->tptSrvCbLst)
   {
      EG_DBG_INFO(EG_DBG_TSAP, tsapCb->tptSrvCbLst,
            (egp,"\n tptSrvCbLst is NULLP"));
      EG_RETVALUE(ROK);
   }

   /********************************************
    * Process each configured transport server *
    ********************************************/
    for (numSrv = 0; numSrv < egCb.genCfg.nmbSrvs; numSrv++)
    {
       if ((serverCb = tsapCb->tptSrvCbLst[numSrv]) == NULLP)
          continue;
 
       /**************************************
        * Perform required action on  server *
        * based on the  type of the action   *
        **************************************/
        switch (srvAction)
        {
#ifdef EG_PHASE2
           case EG_TPT_TPTSRV_CLOSE:
              {
                 if (serverCb->state != LEG_TPTSRV_DIS)
                 {
                    /************************************
                     *  CLOSE this server and inform LM *
                     ************************************/
                     (Void) egTptCloseServer (serverCb);
                     egMiTptSrvCloseInd (serverCb, TRUE);
/*eg004.201 Dispatch run-time update for TSAP cb when tptsrvrs closed*/
#ifdef HW
                     HW_RTUPD_MOD(HW_EG_TPTSERVERCB, serverCb);
#endif 
                      processed++;
                 }
              }
              break;
#ifdef EGTP_C
              /*-- eg003.201 : changes from eg008.102: EGTP_C flag to avoid
               *              compile error, when EG_PHASE2 flag is TRUE --*/
           case EG_TPT_TPTSRV_DISABLE:
              { 
                 /********************************************
                  * Just flag server as disabled and do not  *
                  * issue discReq/termReq.  This  option  is *
                  * used only if lower layer has crashed     *
                  ********************************************/
                  (Void) egTptDelAllServerContext (serverCb, TRUE);
                  serverCb->state = LEG_TPTSRV_DIS;
  
                 /************************************
                  * Inidicate transport server being *
                  * closed by layer manager.         *
                  ************************************/
                  egMiTptSrvCloseInd (serverCb, TRUE);
/*eg004.201 Dispatch run-time update for TSAP cb when tptsrvrs closed*/
#ifdef HW
                  HW_RTUPD_MOD(HW_EG_TPTSERVERCB, serverCb);
#endif 
                  processed++;
              }
              break;
#endif /* EGTP_C */
#endif /* EG_PHASE2 */
          case EG_TPT_TPTSRV_OPEN:
             { 
                /********************
                 * OPEN this server *
                 ********************/
#ifdef HW
                 serverCb->state = LEG_TPTSRV_DIS;
#endif
                 if (egTptOpenServer (serverCb, NULLP) != ROK)
                    EG_RETVALUE (RFAILED);
 
                  processed++;
             }
             break;
          case EG_TPT_TPTSRV_FREE:
             {  
#ifdef EGTP_U
                   if(serverCb->cfg.gtp_C_Or_U == LEG_EGTP_U)
                 {

                      /*********************************************************************
                       * Before deleting from the TSap first delete it from ipCb hash list *
                       *********************************************************************/
                       if((ROK != (retVal = cmHashListFind(&(serverCb->ipCbPtr->tptServCbLst), (U8 *)&(serverCb->portNum),
                                                   sizeof(U16),0,(PTR *)&tempSrvCb))) && tempSrvCb != serverCb)
                      {   
                           EG_DBG_ERR(0, 0,
                                   (egp,"egTServerCntrl(): Failed to find the tpt server from IpCb hashList"));

                      }/* end of if */
                       else
                      {
                        if ( (retVal = cmHashListDelete(&(serverCb->ipCbPtr->tptServCbLst), (PTR)tempSrvCb)) != ROK)
                        {
                           EG_DBG_ERR(0, 0,
                                   (egp,"egTServerCntrl(): Failed to delete tpt server Control block from hash list"));
                        } /* end of if deleting the peer fails */
                     } /* end of else */
                  }/* End of if serverCb->cfg.gtp_C_Or_U */
#endif /* EGTP_U */

                /**************************************
                 * Close transport server if open and *
                 * free the control block and memory  *
                 **************************************/
                 if (serverCb->state != LEG_TPTSRV_DIS)
                 {
                    /***********************************
                     * CLOSE this server and Inform LM *
                     ***********************************/
                     (Void) egTptCloseServer (serverCb);
  
                     egMiTptSrvCloseInd (serverCb, TRUE);
/*eg004.201 Dispatch run-time update for TSAP cb when tptsrvrs closed*/
#ifdef HW
                    HW_RTUPD_MOD(HW_EG_TPTSERVERCB, serverCb);
#endif
 
                     processed++;
                }
/*eg004.201 Dispatch run-time update for TSAP cb when tptsrvrs closed*/
#ifdef HW
                 HW_DEL_MAPPING(HW_EG_TPTSERVERCB, serverCb);
#endif
                egTptFreeServer(serverCb);
                tsapCb->tptSrvCbLst[numSrv] = NULLP;
 
             }
             break;
          default:
             {
                EG_DBG_ERR(EG_DBG_TSAP, tsapCb,
                      (egp,"\n Invalid action %d", srvAction));
                EG_RETVALUE (RFAILED);
             }
       }/* switch (srvAction) */
   }/* for (numSrv = 0; numSrv < egCb.genCfg.nmbSrvs; numSrv++) */
   
    EG_DBG_INFO(0, 0,
          (egp,"\n Number of Tpt Server Processed(%d)", processed));

    EG_RETVALUE (ROK);
} 

/**********************************************************************
*       Fun:   egTptOpenServer
*
*       Desc: This function is used to open transport server 
*              connection. 
*
*       Ret:   ROK   on success
*              RFAILED on error
*
*       Notes: This function  invokes  HitServOpenReq
*              to open transport server on HIT interface. 
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptOpenServer
(
EgTptSrvCb    *serverCb,          /* Server connection control block */
U16           *reason                               /* Return reason */
)
#else
PUBLIC S16 egTptOpenServer (serverCb,reason)
EgTptSrvCb    *serverCb;          /* Server connection control block */
U16           *reason;                              /* Return reason */
#endif
{
   CmIcmpFilter   nullFilter; 
   EgTSapCb       *tsapCb;                /* TSAP Control block Ptr */
   U8             hitTptProt;                 /* TUCL protocol type */
   S16            ret = 0;       /* Holds return value of functions */
   EgUsta         sta;  /* unsolicited status diagnostics structure */ 

   EG_TRC2(egTptOpenServer);

   EG_DBG_INFO(0,0, (egp, "\n Open the TPT Server "));

   cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

   /*****************************************************         
    *   Step 1: Perform basic error checkings           *
    *****************************************************/

   /***************************************************
    * First of all Server must not be disabled, if so *
    * log error and return from here with a reson     *
    ***************************************************/
    if (serverCb->state != LEG_TPTSRV_DIS)
    {
       EG_DBG_ERR(0, 0, 
       (egp,"\n serverCb is already opened / enabled state, state %d ", 
              serverCb->state));
 
       if( reason != NULLP )
       {
          *reason = LCM_REASON_INVALID_STATE;
       }
 
      EG_RETVALUE (RFAILED);
    }


   /**********************************************
    * TSAP used must be bound and enabled if not *
    * then generate alarm with proper reson      *
    **********************************************/
    tsapCb = serverCb->tSapCb;
    if (tsapCb->state != LEG_TSAP_BNDENA)
    {
       EG_DBG_ERR(EG_DBG_TSAP, tsapCb,
           (egp,"\n tsapCb not in enabled state"));
 
       sta.dgn.dgnVal[0].type    = LEG_PAR_TSAP;
       sta.dgn.dgnVal[0].t.sapId = tsapCb->cfg.tSapId;
 
       egSendLmAlarm(LCM_CATEGORY_INTERNAL, 
                     LEG_EVENT_TPTSRV_ENA, 
                     LEG_CAUSE_SAP_BNDDIS, 
                     &sta.dgn);
 
       if( reason != NULLP )
       {
          *reason = LCM_REASON_INVALID_SAP;
       }
 
      EG_RETVALUE (RFAILED);
    }

   /******************************************************
    *      Step 2: Decide the transport to be used       *
    ******************************************************/
    (Void)cmMemset((U8 *)&nullFilter, 0, sizeof (CmIcmpFilter));
    nullFilter.type = CM_ICMP_NO_FILTER;

   /************************************************
    * Since eGTP support UDP only, set accordingly *
    ************************************************/
    hitTptProt = HI_SRVC_UDP;

   /******************************************************
    *  Step 3: Change state and send Server Open request *
    ******************************************************/

   /*********************************************
    *  Guard opening of tpt server with a timer *
    *********************************************/
   if (serverCb->cfg.opnSrvTmr.enb == TRUE)
   {
      if (serverCb->opnSrvCurRetryCnt-- >= 0)
      {
          EG_DBG_INFO( EG_DBG_TSAP, tsapCb,
               (egp,"\n Decrementing opnSrvCurRetryCnt"));

         /*****************************************************
          * Start a timer before sending open server request. *
          * If fail to start then log error and return.       *
          *****************************************************/
          ret = egSchedTmr(serverCb, EG_TMR_TPT_OPEN_SRV, TMR_START,
                               serverCb->cfg.opnSrvTmr.val);
          if (ret != ROK)
          {
             EG_DBG_ERR(0, 0, (egp,"\n Failed to start the Timer"));
             EG_RETVALUE(ret);
          }
      }
      else
      {
        /*************************************************
         *  Unable to open server connection as maximum  *
         *  retries as reached. Inform this event to LM  *
         *  and reset the current retry count.           *
         *************************************************/
         EG_DBG_ERR(EG_DBG_TSAP, tsapCb, 
            (egp,"\n opnSrvCurRetryCnt exceeded"));

         egMiTptSrvCloseInd (serverCb, FALSE);

#ifdef EGTP_U
         if(serverCb->cfg.gtp_C_Or_U == LEG_EGTP_U)
         {
            serverCb->state = LEG_TPTSRV_DIS;
         }
#endif /* EGTP_U */

         serverCb->opnSrvCurRetryCnt =  serverCb->cfg.opnSrvRetryCnt;
 
         /*-- eg006.201: send alarm in case of max retries of Srv Open req fails */
         sta.dgn.dgnVal[0].type    = LEG_PAR_TSAP;
         sta.dgn.dgnVal[0].t.sapId = tsapCb->cfg.tSapId;

         egSendLmAlarm(LCM_CATEGORY_INTERNAL, 
               LEG_EVENT_TPTSRV_ENA, 
               LEG_CAUSE_MAX_SRV_RETRY, 
               &sta.dgn);


        EG_RETVALUE (ROK);
      }
   }

   /***********************************************
    * Set the state and send open request to TUCL *
    ***********************************************/
    serverCb->state = LEG_TPTSRV_WAIT_ENA;

    (Void) EgLiHitServOpenReq (&tsapCb->pst,
                               tsapCb->cfg.spId,
                               serverCb->suConnId,
                               &serverCb->cfg.tptAddr,
                               &serverCb->cfg.tptParam,
                               &nullFilter,
                               hitTptProt);

  EG_RETVALUE(ROK);
}

/**********************************************************************
*       Fun:   egTptCloseServer
*
*       Desc: This  function  is used  to close transport server
*              connection. This function  invokes  HitDiscReq  to
*              to close transport server on HIT interface.
*
*       Ret:   None
*
*       Notes: This function will NOT delete server control block.
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egTptCloseServer
(
EgTptSrvCb     *serverCb          /* Server connection control block */
)
#else
PUBLIC Void egTptCloseServer (serverCb)
EgTptSrvCb     *serverCb;         /* Server connection control block */
#endif
{
   U8           choice;  /* Choice of the  */
   ConnId       connId;  /* Connection id */
   EgTSapCb     *tsapCb; /* TSAP Control block ptr */

   EG_TRC2(egTptCloseServer);

   EG_DBG_INFO(0,0,(egp,"\n Close the TPT Server "));

   /***********************************************************
    *  If server is already disabled, do nothing, Just return * 
    ***********************************************************/
    if ((serverCb == NULLP ) || (serverCb->state == LEG_TPTSRV_DIS))
    {
       EG_DBGP(0, 0, (egp,"\n serverCb->state already disabled."));
       EG_RETVOID;
    }

   /************************************************************
    *  SAP state must be enabled. If not, log error and return * 
    ************************************************************/
    tsapCb = serverCb->tSapCb;

    if (tsapCb->state != LEG_TSAP_BNDENA)
    {
       EG_DBG_ERR(EG_DBG_TSAP, tsapCb, (egp,"\n tsapCb not enabled"));
       EG_RETVOID;
    }

   /************************************************
    *  Stop any open server timer which is running *
    ************************************************/
    if (serverCb->cfg.opnSrvTmr.enb == TRUE)
    {
       EG_DBG_INFO(0,0,(egp,"\n Stop the Open Server Timer if running "));
       (Void) egSchedTmr(serverCb, EG_TMR_TPT_OPEN_SRV, TMR_STOP, NOTUSED);
    }
 
   /************************************************
    * Choose between suConnId/spConnId for DiscReq *
    ************************************************/
    if (serverCb->state == LEG_TPTSRV_WAIT_ENA)
    {
       choice = HI_USER_CON_ID;
       connId = serverCb->suConnId;
    }
    else
    {
       choice = HI_PROVIDER_CON_ID;
       connId = serverCb->spConnId;
    }

   /************************************************************
    * Change server state to DISABLED, send Disconnect request *
    ************************************************************/
    serverCb->state = LEG_TPTSRV_DIS;


/*eg004.201 Issue connection termination request for a TSAP to TUCL only if node  is Active  */
#ifdef HW
    if(HW_IS_ACTV() == TRUE)  /* issue termReq to TUCL only if this is Active */
#endif
    egTptIssueDiscReq (tsapCb, choice, connId );

  EG_RETVOID;
}

/**********************************************************************
*       Fun:   egTptIssueDiscReq
*
*       Desc: This is an internal support function  that  issues an
*              HitDiscReq.
*
*       Ret:  EG_RETVOID
*
*       Notes: 
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egTptIssueDiscReq
(
EgTSapCb           *tsapCb,          /* transport SAP control block */
U8                 choice,         /* type of connection identifier */
ConnId             connId                  /* connection identifier */
)
#else
PUBLIC Void egTptIssueDiscReq (tsapCb, choice, connId )
EgTSapCb           *tsapCb;        /* transport SAP control block */
U8                 choice;       /* type of connection identifier */
ConnId             connId;               /* connection identifier */
#endif
{
   CmTptParam      tptParam;   /* Holds the transport parameter */

   EG_TRC2(egTptIssueDiscReq);

  /****************************************************
   * Set Transport Parameter type, Check if  TSAP is  *
   * enabled then send the Disconnect request to TUCL *
   ****************************************************/
   tptParam.type = CM_TPTPARAM_NOTPRSNT;

   if (tsapCb->state == LEG_TSAP_BNDENA)
   {
      EG_DBG_INFO(EG_DBG_TSAP, tsapCb,
           (egp,"\n Send TPT Server Disc Req "));

      (Void) EgLiHitDiscReq (&tsapCb->pst, tsapCb->cfg.spId,
                             choice, connId, HI_CLOSE, &tptParam);
     EG_RETVOID;
   }
  
  /****************************************************
   * If TSAP is not enabled then log error and return *
   ****************************************************/
   EG_DBG_ERR(EG_DBG_TSAP, tsapCb, (egp,"\n TSAP State not enabled"));

  EG_RETVOID;
}

/**********************************************************************
*       Fun:   egTptFreeServer
*
*       Desc:  Frees memory for transport server
*
*       Ret:   ROK   on success
*              RFAILED on error
*
*       Notes:
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egTptFreeServer
(
EgTptSrvCb      *serverCb               /* Transport server to free */
)
#else
PUBLIC Void egTptFreeServer (serverCb)
EgTptSrvCb      *serverCb;              /* Transport server to free */
#endif
{
   EG_TRC2(egTptFreeServer);

#ifdef EGTP_C
    /************************************************
     * If it is a EGTP_C tpt server then remove the *
     * tpt serverCb from the message and teid hash  *
     * list                                         *
     ************************************************/
   if(serverCb->cfg.gtp_C_Or_U == LEG_EGTP_C)
   {
   /***********************************************************
    * STEP 1: If any user is using server connection,         *
    *         inform  them  that connection is going down and *
    *         delete the context.
    ***********************************************************/
   (Void) egTptDelAllServerContext (serverCb, TRUE);

   /*******************************************************
    * STEP 2: Finally free server control block and  any  *
    *         associated memory.                          * 
    *******************************************************/
/*eg004.201 Delete mapping for TPTSERVER CB when tptServers is removed */
#ifdef HW 
         HW_DEL_MAPPING(HW_EG_TPTSERVERCB , serverCb);
#endif

   egUtlDelTknStrOSXL (&serverCb->hostName);
   } /* end of if */
#endif /* EGTP_C */

   /***************************************************************
    * eg009.201: Free memory allocated for storing Hostname value * 
    * *************************************************************/
   EG_FREE (serverCb->cfg.hostName.val, serverCb->cfg.hostName.len);

   EG_FREE (serverCb, sizeof (EgTptSrvCb));

  EG_RETVOID;
}

#ifdef EGTP_C

/**********************************************************************
*       Fun:   egTptDelAllServerContext
*
*       Desc:  Delete the Server Context
*
*       Ret:   ROK/RFAILED
*
*       Notes: This function is called when server connection is be-
*              ing removed. It deletes context of the user usi-
*              ng server connection and also informs that user.
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egTptDelAllServerContext
(
EgTptSrvCb      *serverCb,                      /* Transport server */
Bool            informUser        /* Inform user of this connection */
)
#else
PUBLIC Void egTptDelAllServerContext (serverCb, informUser)
EgTptSrvCb      *serverCb;                     /* Transport server */
Bool            informUser;      /* Inform user of this connection */
#endif
{
   EgSeqNoCb     *seqNoCb = NULLP;        /* Ptr to Seq. Number CB */
   EgHashCb      *egTptTeidCb = NULLP; /* Ptr to Seq.Number List CB */

   EG_TRC2(egTptDelAllServerContext);

   EG_DBG_INFO(0,0, (egp, "\n Delete the Server Context "));

  /********************************************************
   * Check if this Server Control block is default server *
   ********************************************************/
   if (TRUE == serverCb->cfg.isDfltSrv)
   {
      EG_DBG_INFO(0,0, (egp, "\n Default TPT Server "));

     /************************************************************
      * Loop through the complete Hash List for Seq.Numb List CB *
      ************************************************************/
      while (cmHashListGetNext (&serverCb->teidMsgHlCp, (PTR) NULLP,
               (PTR *) &egTptTeidCb) == ROK)
      {   
        /*********************************************************
         * Loop through the Seq.Num Hash List CB for a Seq.Numb  *
         *********************************************************/
         while (cmHashListGetNext (&egTptTeidCb->seqNoCbLst, (PTR) NULLP,
                  (PTR *) &seqNoCb) == ROK)
         {   
           /*****************************************
            * Delete the SeqNoCb from the hash list *
            *****************************************/
            cmHashListDelete (&egTptTeidCb->seqNoCbLst, (PTR) seqNoCb);

           /*******************************
            * Free the memory for SeqNoCb *
            *******************************/
            EG_FREEMBUF (seqNoCb->mBuf);
            EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
         }

        /********************************************
         * Delete the egTptTeidCb from the hash list *
         ********************************************/
         cmHashListDelete (&serverCb->teidMsgHlCp,
                                 (PTR) egTptTeidCb);
         
        /************************************
         * Deinit egTptTeidCb from hash list *
         ************************************/
         cmHashListDeinit (&egTptTeidCb->seqNoCbLst);
         
        /**********************************
         * Free the memory for egTptTeidCb *
         **********************************/
         EG_FREE(egTptTeidCb, sizeof(EgHashCb));
      }
/* eg004.201 Memory leak fix:On Shutdown serverCb->msgHlCp and seqNoCb->mBuf
 *memory were not being freed resulting in memory leak
 */
#ifdef EGTP_C_PTH_MGT
      EG_DBG_INFO(0,0, (egp, "\n Default TPT Server with path Management Enabled"));

      /***********************************************************
       * Handle Non default Server here. Loop through and delete *
       ***********************************************************/
      while (cmHashListGetNext (&serverCb->msgHlCp, (PTR) NULLP,
               (PTR *) &seqNoCb) == ROK)
      {
         /*****************************************
          * Delete the SeqNoCb from the hash list *
          *****************************************/
         cmHashListDelete (&serverCb->msgHlCp, (PTR) seqNoCb);

         /*******************************
          * Free the memory for SeqNoCb *
          *******************************/
         EG_FREEMBUF (seqNoCb->mBuf);
         EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
      }

      cmHashListDeinit (&serverCb->msgHlCp);
#endif



     /*****************************************
      * Now we can deInitialize the Hash List *
      *****************************************/
      cmHashListDeinit (&serverCb->teidMsgHlCp);
   }
   else
   {
      EG_DBG_INFO(0,0, (egp, "\n Non- Default TPT Server "));
     /***********************************************************
      * Handle Non default Server here. Loop through and delete * 
      ***********************************************************/
      while (cmHashListGetNext (&serverCb->msgHlCp, (PTR) NULLP,
               (PTR *) &seqNoCb) == ROK)
      {   
        /*****************************************
         * Delete the SeqNoCb from the hash list * 
         *****************************************/
         cmHashListDelete (&serverCb->msgHlCp, (PTR) seqNoCb);

        /*******************************
         * Free the memory for SeqNoCb *
         *******************************/
         EG_FREEMBUF (seqNoCb->mBuf);
         EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
      }

      cmHashListDeinit (&serverCb->msgHlCp);
   }

  /********************************************************
   * Tpt Server will be talking to only one USAP, this is *
   * beacuse of the fact that Seq. Num is unique per USAP *
   ********************************************************/
   if (TRUE == informUser)
   {
     /*****************************************************
      * Pass Message to CPM which in turn inform the user *
      *****************************************************/
   }

  EG_RETVOID;
} 

/**********************************************************************
*       Fun:   egTptInit
*
*       Desc:  Initializse the TPT module
*
*       Ret:   
*
*       Notes: 
*
*       File:  eg_tpt.c
**********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptInit
(
)
#else
PUBLIC S16 egTptInit (void)
#endif
{
   S16 ret;

   EG_TRC2(egTptInit);

   /**************************************************
    * Nothing to be done at init time for TPT module *
    * Keeping this function for completeness purpose *
    **************************************************/   
   ret = ROK;

   EG_RETVALUE(ret);

}

#endif /* EGTP_C */
/***********************************************************************
*       Fun:   egTptDeInit
*
*       Desc:  Initializse the TPT module
*
*       Ret:   
*
*       Notes: 
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egTptDeInit
(
)
#else
PUBLIC Void egTptDeInit (Void)
#endif
{
#ifdef EG_MULTI_IP
   EgTSapCb  *tSapCb = NULLP;
   U16 idx;
#endif

   EG_TRC2(egTptDeInit);

#ifdef EG_MULTI_IP
  /**************************************************
   * eg007.201: Loop over range of TSapCb, Check for*
   * valid T-SAP and process the TSAP servers       *
   **************************************************/
   for (idx = 0;idx < egCb.genCfg.maxNmbTSaps;idx++)
   {
      tSapCb = egCb.tSapLst[idx];
      if (NULLP != tSapCb)
      {
         (Void) egTptProcessTsapServers(tSapCb, EG_TPT_TPTSRV_FREE);

      }
   }
#else
   if (NULLP != egCb.tSapCb)
   {
      (Void) egTptProcessTsapServers(egCb.tSapCb, EG_TPT_TPTSRV_FREE);

   }
#endif
   EG_RETVOID;
}

#ifdef EGTP_C
/**********************************************************************
*       Fun:   egTptState
*
*       Desc:  Get the TPT Server State
*
*       Ret:   
*
*       Notes: 
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptState
(
 CmTptAddr   tptSrvAddr,                /* Transport Server Address */
 TknU8       *state                          /* State of the server */
)
#else
PUBLIC S16 egTptState (tptSrvAddr, state)
 CmTptAddr   tptSrvAddr;                /* Transport Server Address */
 TknU8       *state;                         /* State of the server */
#endif
{
   S16         ret;                     /* Function Return values */
   EgTptSrvCb  *tptSrvCb;              /* Transport Server CB Ptr */
   U16         nmbSrv;            /* Number of Tpt Server in TSAP */
   CmTptAddr   *localAddrPtr = NULLP;     /* Ptr to Local Address */
   Bool        addressMatch = FALSE;   /* Flag,know address match */
   Bool        isMatchFound = FALSE; /* Flag,verify adderss match */
#ifdef EG_MULTI_IP
    EgTSapCb       *tSapCb  = NULLP;
#endif

   EG_TRC2(egTptState);

   ret = ROK;
   nmbSrv = 0;

#ifdef EG_MULTI_IP
  /**************************************************
   * eg007.201: Obtain TSAP for the IP addr provided*
   * Check if TSAP is Configured                    *
   **************************************************/
   egTptGetTsapFrmAddr (tptSrvAddr, &tSapCb );
   if (NULLP == tSapCb)
#else
   if (NULLP == egCb.tSapCb)
#endif
   {
      EG_DBG_ERR(0,0, (egp, "\n TSAP is not Configured "));
      ret = RFAILED;
      EG_RETVALUE(ret);
   }

  /********************************************
   * Loop through the Tpt servers of the TSAP *
   ********************************************/
#ifdef EG_MULTI_IP
   while (nmbSrv < tSapCb->numSrvr)
#else
   while (nmbSrv < egCb.tSapCb->numSrvr)
#endif
   {
#ifdef EG_MULTI_IP
      tptSrvCb = tSapCb->tptSrvCbLst[nmbSrv];
#else
      tptSrvCb = egCb.tSapCb->tptSrvCbLst[nmbSrv];
#endif

      if (NULLP == tptSrvCb)
      {
         /**************************************************
          * This server is not matching, So Check next one *
          **************************************************/
         nmbSrv++;
         continue;
      }

      EG_DBG_INFO(0,0, (egp, "\n Non- Default TPT Server "));

     /******************************************************
      * Check Whether Local Address and Tpt Server Address *
      * matches. If not matching check for the next one.   *
      * If matches then update the Tpt Server states       *
      ******************************************************/
      localAddrPtr= &tptSrvCb->cfg.tptAddr;

      ret = egCmpTptAddr(localAddrPtr, &tptSrvAddr, &addressMatch);
      if( (ret != ROK) || (addressMatch != TRUE))
      {
         nmbSrv++;
         continue;
      }

      /***********************************
       * TPT Server found, get the state *
       ***********************************/
      EG_DBG_INFO(0,0, (egp, "\n Match found for TPT Server "));
      isMatchFound = TRUE;
      state->pres = PRSNT_NODEF;
      state->val = tptSrvCb->state;
      break;
   }

  /****************************************
   * No Match found, log error and return * 
   ****************************************/
   if (TRUE != isMatchFound)
   {
      EG_DBG_INFO(0,0, (egp, "\n TPT Server not found"));
      ret = RFAILED;
   }

   EG_RETVALUE(ret);
}

/**********************************************************************
 *       Fun:   egTptMsgRetxTmrExpiry
 *
 *       Desc:  Process the EGTP Msg Retx timer expiry. 
 *
 *       Ret:   EG_RETVOID
 *
 *       Notes: None
 *
 *       File:  eg_tpt.c
 *
 **********************************************************************/
#ifdef ANSI
PUBLIC Void egTptMsgRetxTmrExpiry
(
EgTptReTmrCb       *tptReTmrCb            /* Control block pointer */
)
#else
PUBLIC Void egTptMsgRetxTmrExpiry(tptReTmrCb)
EgTptReTmrCb       *tptReTmrCb;            /* Control block pointer */
#endif
{
   S16            ret = 0;         /* Holds Fuction Return values */
   EgTptSrvCb     *serverCb;                     /* Tpt Server CB */
   Buffer         *mBuf;                /* Message Buffer Pointer */
   EgSeqNoCb      *seqNoCb;          /* Seq. Number Control block */
   CmTptAddr      *destAddr;       /* Destination IP-address/port */
   U32            seqNo;         /* Holds the Message Seq. Number */
   U32            teid;                    /* tunnel End Point Id */
   U16            tmrVal;                          /* Timer value */
   /* Added with EGTPU changes */
   Pst            tmpPst;
   Bool           isMsbSet =  FALSE;   /* MSB of Message Seq Num */

   EG_TRC2(egTptMsgRetxTmrExpiry);

  /**********************************
   * Initialize the Local variables *
   **********************************/
   serverCb = NULLP;
   mBuf = NULLP;
   seqNoCb = NULLP;

   EG_DBG_INFO(0, 0,(egp, "\n Message Re-transmission Timer Expired \n"));

  /*******************************************
   * Find the server Retransmission timer CB *
   * If not found log error & return         *
   *******************************************/
   if (NULLP == tptReTmrCb)
   {
      EG_DBG_ERR(0, 0,(egp, "\n Null Timer CB \n"));
      EG_RETVOID;
   }

  /*******************************************
   * Get all relevent parameters from the CB *
   *******************************************/
   serverCb = tptReTmrCb->tptSrvCb;
   seqNo = tptReTmrCb->seqNo;
   teid = tptReTmrCb->teid;
   destAddr = &tptReTmrCb->destAddr;
   tmrVal = tptReTmrCb->t3.val;

   if (NULLP == serverCb)
   {
      EG_DBG_ERR(0, 0,(egp, "\n Null Timer CB \n"));
      EG_RETVOID;
   }

  /***************************************************
   * eg009.201 : Obtained the value of MSB of seqNo  *
   * to process timeout msg accordingly              *
   ***************************************************/
    EG_IS_MSB_SET_IN_SEQ_NO(seqNo, isMsbSet);

  /*************************************************
   * eg007.201 :  Based on the MSB of seq Num retrive
   * the Server CB and get the Msg Buffer 
   *************************************************/
   
   if (TRUE == isMsbSet) 
   {
      EgHashCb  *egTptTeidCb;

      EG_DBG_INFO(0,0, (egp, "\n Default TPT Server "));
      egTptTeidCb = NULLP;
     /*************************************************
      * Retrive the default Server List CB from hash. *
      * Fail to retrive? then log error and return    *
      *************************************************/
      ret = cmHashListFind(&serverCb->teidMsgHlCp, 
            (U8 *) &teid, sizeof(U32), 
            0, (PTR *)&egTptTeidCb); 
      if ((ret != ROK) || (NULLP == egTptTeidCb)) 
      {
         EG_DBG_ERR(0, 0,
           (egp,"\n Failed to Locate TEID in hash List"));
         EG_RETVOID;
      }

     /***********************************************
      * Retrive the Sequence Number CB from hash.   *
      * Fail to retrive? then log error and return  *
      ***********************************************/
      ret = cmHashListFind(&egTptTeidCb->seqNoCbLst, 
            (U8 *) &seqNo, sizeof(U32), 
            0, (PTR *)&seqNoCb); 
      if ((ret != ROK) || (seqNoCb == NULLP))
      {
         EG_DBG_ERR(0, 0,
            (egp,"\n Failed to Locate Seq. No in hash List"));
         EG_RETVOID;
      }
   }
   else
   {
      EG_DBG_INFO(0,0, (egp, "\n Non- Default TPT Server "));

     /***********************************************
      * Retrive the Sequence Number CB from hash.   *
      * Fail to retrive? then log error and return  *
      ***********************************************/
      ret = cmHashListFind(&serverCb->msgHlCp, 
            (U8 *) &seqNo, sizeof(U32), 
            0, (PTR *)&seqNoCb); 
      if ((ret != ROK) || (seqNoCb == NULLP))
      {
         EG_DBG_ERR(0, 0, (egp,"\n Failed to get the Seq No(%ld). CB ", 
                    seqNo));
         EG_RETVOID;
      }
   }

  /*********************************
   * Get the Stored Message Buffer *
   *********************************/
   mBuf = seqNoCb->mBuf;

   /**********************************
    * Check the Number of retry left *
    **********************************/
   if (tptReTmrCb->msgRetxRetryCnt-- <= 0)
   {
      EG_DBG_INFO(0, 0,
      (egp,"\n No. of Retry exceeded, deleting the message"));

     /**********************************************************
      * eg007.201: Remove the Message from MSB of Seq Number   *
      **********************************************************/
      if (TRUE == isMsbSet)
      {
         (void) egTptQueueMsgRmvWthMsb (serverCb, teid, seqNo, TRUE);
      }
      else
      {
/* eg004.201: eGTPC PSF Modified to send error message to LM only from ACTIVE NODE*/
#ifdef HW
         if(HW_IS_ACTV())
         {
#ifndef EGTP_C_PTH_MGT
            (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE);
#else
            (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE, destAddr);
#endif
         }
         else
         {
#ifndef EGTP_C_PTH_MGT
            (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE);
#else
            (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE, destAddr);
#endif
         }
#else
#ifndef EGTP_C_PTH_MGT
         (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE);
#else
         (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE, destAddr);
#endif
#endif
      }
      EG_RETVOID;
   }

  /*******************************************
   * Start the Retransmission timer. If      *
   * fail to start then log error and return *
   *******************************************/
   ret = egSchedTmr(tptReTmrCb, 
                    EG_TMR_TPT_MSG_RETX, TMR_START,
                         tmrVal);
   if (ret != ROK)
   {
      EG_DBG_ERR(0, 0, (egp,"\n Starting of Timer Failed"));
     /**********************************************************
      * Remove the Message from the queue based on server type * 
      **********************************************************/
      if (TRUE == serverCb->cfg.isDfltSrv)
      {
         (void) egTptQueueMsgRmvWthMsb (serverCb, teid, seqNo, TRUE);
      }
      else
      {
/* eg004.201 Modified to send error message on failure to start timer to LM only from ACTIVE NODE*/
#ifdef HW
         if(HW_IS_ACTV())
         {
#ifndef EGTP_C_PTH_MGT
            (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE);
#else
            (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE, destAddr);
#endif
         }
         else
         {
#ifndef EGTP_C_PTH_MGT
            (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE);
#else
            (void) egTptQueueMsgRmv (serverCb, seqNo, FALSE, destAddr);
#endif
         }
#else
#ifndef EGTP_C_PTH_MGT
         (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE);
#else
         (void) egTptQueueMsgRmv (serverCb, seqNo, TRUE, destAddr);
#endif
#endif
      }
      EG_RETVOID;
   }

   /********************************
    * Now ReSend the Message again *
    ********************************/
/* eg004.201  Modified to resend message present in the ReTx queue to the PEER node  only from ACTIVE NODE*/
#ifdef HW
   if(HW_IS_ACTV())
   {
#endif
#ifdef EG_MULTI_IP
      /**********************************************************
       * eg007.201: Get TSAP Control block from ServrCb*
       **********************************************************/
   tmpPst.srcInst = serverCb->tSapCb->pst.srcInst;
#else
#ifndef SS_MULTIPLE_PROCS 
   tmpPst.srcInst = egCb.tSapCb->pst.srcInst;
#else
   tmpPst.srcInst = egCbPtr->tSapCb->pst.srcInst;
#endif /* SS_MULTIPLE_PROCS */
#endif /*EG_MULTI_IP*/
   ret = egTptSendMsg(serverCb, destAddr, mBuf, &tmpPst);
   if (ret != ROK)
   {
      EG_DBG_ERR(0, 0, (egp,"\n Sending of Message Failed"));
     /**********************************
      * Stop the Re-transmission Timer *
      **********************************/
      egSchedTmr(tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_STOP, NOTUSED);
   }
/* eg004.201  Modified to resend message present in the ReTx queue to the PEER node  only from ACTIVE NODE*/
#ifdef HW
   }
#endif

   EG_RETVOID;
}
/*************************************************************
 ********** U T I L I T Y   F U N C T I O N S  ***************
 *************************************************************/

/**********************************************************************
*       Fun:    egUtlDelTknStrOSXL
*
*       Desc:   Frees the internal mem for the TknStrOSXL struct
*
*       Ret:    ROK     - ok
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUtlDelTknStrOSXL
(
TknStrOSXL *delTknStrOSXL
)
#else
PUBLIC S16 egUtlDelTknStrOSXL(delTknStrOSXL)
TknStrOSXL *delTknStrOSXL;
#endif
{
   EG_TRC2(egUtlDelTknStrOSXL);

  /*************************************************
   * Check for the presence of the token and valid *
   * lenght and free it and reset the token flags  *
   *************************************************/
   if (delTknStrOSXL->pres == NOTPRSNT)
      EG_RETVALUE(ROK);

   if (delTknStrOSXL->len > 0)
      EG_FREE(delTknStrOSXL->val, sizeof(U8) * delTknStrOSXL->len);

   delTknStrOSXL->pres = NOTPRSNT;
   delTknStrOSXL->len  = 0;

   EG_RETVALUE(ROK);
} 
#endif

/**********************************************************************
*       Fun:   egCmpTptAddr 
*
*       Desc:  Compares two CmTptAddr
*
*       Ret:   TRUE = complete match, FALSE = no match or address match
*
*       Notes: Returns _address
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egCmpTptAddr
(
CmTptAddr  *one,                               /* Address to compare */
CmTptAddr  *two,                               /* Address to compare */
Bool       *addressMatch    /* Return true atleast _address_ matches */
)
#else
PUBLIC S16 egCmpTptAddr( one,  two, addressMatch)
CmTptAddr  *one;                               /* Address to compare */
CmTptAddr  *two;                               /* Address to compare */
Bool       *addressMatch;   /* Return true atleast _address_ matches */
#endif
{
#ifdef IPV6_SUPPORTED
   U16 ipV6AdrSz = 0;                         /* IPV6 Address size */
#endif 

   EG_TRC3(egCmpTptAddr);

   /***************************
    * Initialize the variable *
    ***************************/
   (*addressMatch) = FALSE;

  /********************************************************
   * If input address type is not unique then return fail *
   ********************************************************/
   if (one->type != two->type)
   {
      EG_RETVALUE(RFAILED);
   }

  /************************************************************
   * Process based on input Addres type.I,e IPV4 or IPV6 type *
   ************************************************************/
   if (one->type == CM_TPTADDR_IPV4)
   {
     /***************************************************
      * If Input address is not unique then return fail *
      ***************************************************/
      if (one->u.ipv4TptAddr.address != two->u.ipv4TptAddr.address)
      {
         EG_RETVALUE(RFAILED);
      }

      /********************************
       * Mark address matched as true *
       ********************************/
      (*addressMatch) = TRUE;

     /***************************************************
      * If Input address Port not unique then return fail *
      ***************************************************/
      if (one->u.ipv4TptAddr.port != two->u.ipv4TptAddr.port)
      {
         EG_RETVALUE(RFAILED);
      }

      EG_RETVALUE(ROK);
   }
#ifdef IPV6_SUPPORTED
   else if (one->type == CM_TPTADDR_IPV6)
   {
      /*********************************************
       * Loop though the size of IPV6 address size *
       * and compare the uniqueness of the address *
       * If it is not unique then return fail      *
       *********************************************/
      while (ipV6AdrSz < (CM_IPV6ADDR_SIZE - 1))
      {
         if (one->u.ipv6TptAddr.ipv6NetAddr[ipV6AdrSz] !=
             two->u.ipv6TptAddr.ipv6NetAddr[ipV6AdrSz])
         {
            EG_RETVALUE(RFAILED);
         }
         ipV6AdrSz++;
      }

      /********************************
       * Mark address matched as true *
       ********************************/
      (*addressMatch) = TRUE;

     /***************************************************
      * If Input address Port not unique then return fail *
      ***************************************************/
      if (one->u.ipv6TptAddr.port != two->u.ipv6TptAddr.port)
      {
         EG_RETVALUE(RFAILED);
      }

      EG_RETVALUE(ROK);
   }
#endif /* IPV6_SUPPORTED */

   /******************************************************
     * If we are reaching here it means there is some    *
     * error. eg type is having wrong value              *                        
     ****************************************************/
   EG_RETVALUE(RFAILED);

}
#ifdef EGTP_C
/**********************************************************************
*       Fun:   egTptCheckQReqrd
*
*       Desc:  This function uses the MSB bit of Message sequence
*              number and based on the message type idenifies
*              whether Queing is required for this message or not.
*
*       Ret:   TRUE or FALSE on success.
*              FAIL on error.
*
*       Note:
*
*       File:  eg_tpt.c
***********************************************************************/
/*eg012.201 : modified prototype,fix for ccpu00125389*/
#ifdef ANSI
PRIVATE S16 egTptCheckQReqrd
(
Bool           isMsbSet,      /* MSB of EGTP Message Sequence Number */
EgtMsgCat      egtpMsgCat,    /* EGTP Message Category */
EgtMsgType     egtpMsgType    /* EGTP Message Type */ 
)
#else
 /* eg012.201 : EGTP Message Type,fix for ccpu00125389*/
PRIVATE S16 egTptCheckQReqrd (isMsbSet,egtpMsgCat,egtpMsgType) 
Bool         isMsbSet;        /* MSB of EGTP Message Sequence Number */
EgtMsgCat    egtpMsgCat;      /* EGTP Message Category */
EgtMsgType   egtpMsgType;     /* EGTP Message Type */ 
#endif
{
   S16 qRequired = FALSE;             /* The result of this function */

   EG_TRC3(egTptCheckQReqrd);

   /********************************************************
    * Based on the EFTP message Catagory and the MSB value *
    * of the Message Sequence Number we need to identify   *
    * whether Queing is required for this message.         *
    ********************************************************/
    switch (egtpMsgCat)
    {
       case EG_GTPC_MSGCAT_REQUEST:
           {
               /* eg012.201 : The messages from 64 to 70 need not be queued,fix for ccpu00125389 */
               switch(egtpMsgType)
               {
                  case EGT_GTPC_MSG_MBFAIL_IND:
                  case EGT_GTPC_MSG_DB_FAIL_IN:
                  case EGT_GTPC_MSG_BR_FAIL_IND:
                  case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:
                     /* The messages from 64 to 70 need not be queued */
                     break;
                  default:
                     /***********************************************
                     * Irrespective of the MSB set in the sequence *
                     * Number,this EGTP message need to be Queued  *
                     ***********************************************/
                     qRequired = TRUE;
               }
           }
           break;
       case EG_GTPC_MSGCAT_RSP_ACK:
           {  /***********************************************
               * Irrespective of the MSB set in the sequence *
               * Number,this EGTP message need to be Queued  *
               ***********************************************/
               qRequired = TRUE;
           }
           break;
       case EG_GTPC_MSGCAT_COMMAND:
           {  /************************************************
               * if the MSB is SET then Queing is required for *
               * this Message otherwise considered as failure  *
               *************************************************/
              if (FALSE == isMsbSet)
              {
               if ((egtpMsgType != EGT_GTPC_MSG_DB_CMND) && (egtpMsgType != EGT_GTPC_MSG_BR_CMND))
               {
                    /**************************************
                    * Not a valid Message to process here *
                    **************************************/
                   EG_DBG_ERR(0, 0,(egp,"\n Wrong Message received "));
                   qRequired = FAIL;
               }
              }
              else
                  {
                  qRequired = TRUE;
           }
           }
           break;
       case EG_GTPC_MSGCAT_NOTFN:
           {  /*************************************************
               * if MSB is UNSET then Queing is required for   *
               * this Message otherwise considered as failure  *
               *************************************************/
               if (TRUE == isMsbSet)
               {
                  /***************************************
                   * Not a valid Message to process here *
                   ***************************************/
                   EG_DBG_ERR(0, 0,
                         (egp,"\n Wrong Message received "));
                   qRequired = FAIL;
               }
              else
                  qRequired = TRUE;
           }
           break;
       case EG_GTPC_MSGCAT_RESPONSE:
       case EG_GTPC_MSGCAT_FAILIND:
           {  /**************************************************
               * No Queing is required for the Response Message *
               **************************************************/
               EG_DBG_INFO(0, 0,
                        (egp,"\n No queueing for Rsp messages "));
               qRequired = FALSE;
           }
           break;
       case EG_GTPC_MSGCAT_ACKN:
       case EG_GTPC_MSGCAT_IND:
           {  /***************************************************
               * For Acknowledgment and Indication EGTP Messages *
               * we need not consider Queing when MSB is UNSET.  *
               * If MSB is set then we consider it as failure    *
               ***************************************************/
               if (!(TRUE == isMsbSet))
               {
                  EG_DBG_INFO(0, 0,
                           (egp,"\n No queueing for Rsp/Ackn/Ind messages "));
                  qRequired = FALSE;
               }
               else
               {
                  /***************************************
                   * Not a valid Message to process here *
                   ***************************************/
                  EG_DBG_ERR(0, 0,
                           (egp,"\n Wrong Message received "));
                  qRequired = FAIL;
               }
           }
           break;
       default:
           {   /***************************************
                * Not a valid Message to process here *
                ***************************************/
                EG_DBG_ERR(0, 0,
                      (egp,"\n Wrong Message received "));
                qRequired = FAIL;
           }
     }/* switch (egtpMsgCat) */

   RETVALUE(qRequired);
}

/**********************************************************************
*       Fun:   egTptProcessLclDel
*
*       Desc:  This function handles Local Delete Request
*
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:
*
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptProcessLclDel
(
EgTptSrvCb        *serverCb, /* Tpt Server CB */
U32                teid
)
#else
PUBLIC S16 egTptProcessLclDel (serverCb, teid)
EgTptSrvCb        *serverCb; /* Tpt Server CB */
U32                teid;
#endif
{
   EgTSapCb          *tSapCb = NULLP;      /* Pointer to Transport SAP Control Block */

   EG_TRC2 (egTptProcessLclDel);

   EG_DBG_INFO(0,0,(egp,"\n Process the Local Delete Request "));

   if (serverCb == NULLP)
   {
      EG_RETVALUE(RFAILED);
   }

   if (TRUE == serverCb->cfg.isDfltSrv)
   {
      /* Ideally this request should not be called on Default Server */

      /* Find the Default Server and delete all messages on given TeID */
      (void) egTptRmvTunnFrmLstWthMSB (serverCb, teid);
      /*eg012.201:memory leak fix for ccpu00130148*/
      (void) egTptRmvTunnFrmLst(serverCb, teid);
   }
   else
   {
#ifdef EG_MULTI_IP
      /**********************************************************
       * eg007.201: Get TSAP Control block from ServrCb*
       **********************************************************/
      if ((tSapCb = serverCb->tSapCb) == NULLP)
#else
      if ((tSapCb = egCb.tSapCb) == NULLP)
#endif
      {
         EG_RETVALUE(RFAILED);
      }

      if (((tSapCb->isDfltTptSrvPsnt) != TRUE) || (tSapCb->dftlTptSrv == NULLP))
      {
         EG_RETVALUE(RFAILED);
      }

      /* Find the Default Server and delete all messages on given TeID */
      (void) egTptRmvTunnFrmLstWthMSB (tSapCb->dftlTptSrv, teid);

      /* Now Check if given TeID is present in the SeqNoCb list, *
         * if yes delete the message*/
      (void) egTptRmvTunnFrmLst(serverCb, teid);

   }

   EG_RETVALUE(ROK);

}

/**********************************************************************
*       Fun:   egTptRmvTunnFrmLstWthMSB
*
*       Desc:  This function removes 
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTptRmvTunnFrmLstWthMSB
(
EgTptSrvCb     *dfltSrvCb, /* Server connection that received message */
U32             teid                  /* Seq No. of Received message */
)
#else
PRIVATE S16 egTptRmvTunnFrmLstWthMSB (dfltSrvCb, teid)
EgTptSrvCb     *dfltSrvCb; /* Server connection that received message */
U32             teid;                  /* Seq No. of Received message */
#endif
{
    
   S16            ret;           /* Holds return value of functions */
   EgHashCb       *egTptTeidCb = NULLP;    /* Ptr to Seq.Num CB List */
   EgSeqNoCb      *seqNoCb = NULLP; /* Ptr to Seq.Num Control block */

   EG_TRC2(egTptRmvTunnFrmLstWthMSB);

  /*****************************************
   * Locate the Tunnel Id in the Hash List *
   *****************************************/
   ret = cmHashListFind(&dfltSrvCb->teidMsgHlCp, 
                       (U8 *) &teid, sizeof(U32), 
                        0, (PTR *)&egTptTeidCb); 

  /*********************************************
   * Fail to locate? Then log error and return *   
   *********************************************/
   if ((ret != ROK) || (NULLP == egTptTeidCb)) 
   {
      EG_DBG_ERR(0, 0,
       (egp,"\n Failed to Locate TEID in hash List"));
      EG_RETVALUE(RFAILED);
   }

   /*********************************************************
    * Loop through the Seq.Num Hash List CB for a Seq.Numb  *
    *********************************************************/
   while (cmHashListGetNext (&egTptTeidCb->seqNoCbLst, (PTR) NULLP,
            (PTR *) &seqNoCb) == ROK)
   {   
      /*****************************************
       * Delete the SeqNoCb from the hash list *
       *****************************************/
      cmHashListDelete (&egTptTeidCb->seqNoCbLst, (PTR) seqNoCb);
      /*eg004.201 Stop ReTx timer and delete mapping for seqNoCb when the associated message
       *response received from the peer node 
       */
      /**********************************
       * Stop the Re-transmission Timer *
       **********************************/
      egSchedTmr(&seqNoCb->tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_STOP, NOTUSED);
#ifdef HW
      HW_RTUPD_DEL(HW_EG_SEQCB, seqNoCb);
      HW_DBG_INFO(0,0, (hwp, "\n PSF RuntimeUpdate with Action Delete "
               "CbType=[%d], TeidValue=[%lu], Sequence Value=[%lu]",
               HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));

      HW_DEL_MAPPING(HW_EG_SEQCB, seqNoCb);
      HW_DBG_INFO(0,0, (hwp, "\n PSF Delete Mapping CbType=[%d],"
               " TeidValue=[%lu], Sequence Value=[%lu]",
               HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));
#endif

      EG_FREEMBUF (seqNoCb->mBuf);
      /*******************************
       * Free the memory for SeqNoCb *
       *******************************/
      EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
   }

   /********************************************
    * Delete the egTptTeidCb from the hash list *
    ********************************************/
   cmHashListDelete (&dfltSrvCb->teidMsgHlCp,
         (PTR) egTptTeidCb);

   /************************************
    * Deinit egTptTeidCb from hash list *
    ************************************/
   cmHashListDeinit (&egTptTeidCb->seqNoCbLst);

   /**********************************
    * Free the memory for egTptTeidCb *
    **********************************/
   EG_FREE(egTptTeidCb, sizeof(EgHashCb));

   EG_RETVALUE(ROK);
}


/**********************************************************************
*       Fun:   egTptRmvTunnFrmLst
*
*       Desc:  This function removes 
*              
*       Ret:   ROK on success.
*              RFAILED on error.
*
*       Note:  
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTptRmvTunnFrmLst
(
EgTptSrvCb     *serverCb, /* Server connection that received message */
U32             teid                 /* Seq No. of Received message */
)
#else
PRIVATE S16 egTptRmvTunnFrmLst (serverCb, teid)
EgTptSrvCb     *serverCb; /* Server connection that received message */
U32             teid;                 /* Seq No. of Received message */
#endif
{
   EgSeqNoCb     *seqNoCb = NULLP;        /* Ptr to Seq. Number CB */
   EgSeqNoCb     *prevSeqNoCb = NULLP;    /* Ptr to Seq. Number CB */

   EG_TRC2(egTptRmvTunnFrmLst);

#ifdef ALIGN_64BIT
   EG_DBG_INFO(0,0, (egp, "\n Delete the Tunnel Context of teid(%d)", teid));
#else
   EG_DBG_INFO(0,0, (egp, "\n Delete the Tunnel Context of teid(%ld)", teid));

#endif


   /***********************************************************
    *  Loop through and if teid matches then delete seqNoCb   * 
    ***********************************************************/
   prevSeqNoCb = NULLP;
   while (cmHashListGetNext (&serverCb->msgHlCp, (PTR) prevSeqNoCb,
            (PTR *) &seqNoCb) == ROK)
   {   
      if (seqNoCb->teid == teid)
      {
         /*****************************************
          * Delete the SeqNoCb from the hash list * 
          *****************************************/
         cmHashListDelete (&serverCb->msgHlCp, (PTR) seqNoCb);

         /**********************************
          * Stop the Re-transmission Timer *
          **********************************/
         /*eg004.201 eGTP-C PSF Stop ReTx timer and delete seqNoCb when MSB not set */
         egSchedTmr(&seqNoCb->tptReTmrCb, EG_TMR_TPT_MSG_RETX, TMR_STOP, NOTUSED);

#ifdef HW
         HW_RTUPD_DEL(HW_EG_SEQCB, seqNoCb);
         HW_DBG_INFO(0,0, (hwp, "\n PSF RuntimeUpdate with Action Delete "
                  "CbType=[%d], TeidValue=[%lu], Sequence Value=[%lu]",
                  HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));

         HW_DEL_MAPPING(HW_EG_SEQCB, seqNoCb);
         HW_DBG_INFO(0,0, (hwp, "\n PSF Delete Mapping CbType=[%d],"
                  " TeidValue=[%lu], Sequence Value=[%lu]",
                  HW_EG_SEQCB,seqNoCb->teid,seqNoCb->seqNo));
#endif

         EG_FREEMBUF (seqNoCb->mBuf);
         /*******************************
          * Free the memory for SeqNoCb *
          *******************************/
         EG_FREE(seqNoCb, sizeof(EgSeqNoCb));
      }
      else
      {
         /*********************
           * Get Next Node    *
           *******************/
         prevSeqNoCb = seqNoCb;
      }
   }

   EG_RETVALUE(ROK);
}

/**********************************************************************
*       Fun:   egTptValidatePBMsg
*
*       Desc: This function is used to validate the consistency in
*             Message length of a piggy backed message.
*
*       Ret:   ROK   on success
*              EGT_ERR_MSG_INVOVR_LEN_PBMSG on error
*
*       Notes: --
*              
*       File:  eg_tpt.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTptValidatePBMsg
(
Buffer        *mBuf,                             /* Received message */
EgPartialMsg  *partialMsg                  /* Ptr to Partial Message */ 
)
#else
PRIVATE S16 egTptValidatePBMsg(mBuf,partialMsg)
Buffer        *mBuf;                             /* Received message */
EgPartialMsg  *partialMsg;                 /* Ptr to Partial Message */ 
#endif
{
   MsgLen bufLen;                   /* Total mBuf Length */
   U8    nwByte[EG_LEN_TWO];        /* Temp Array */
   U16   tmpU16 = 0;                /* Temp Variable */

   EG_TRC2(egTptValidatePBMsg);

   /*****************************
    * Initialize the temp array *
    *****************************/
   nwByte[0] = 0; nwByte[1] = 0; 

   /*************************************************
    * Find the total length of the message received *
    *************************************************/
   SFndLenMsg(mBuf, &bufLen);

   /**************************************************************
    * Fetch the Length of the first message (triggered response) *
    * in the mBuf received                                       *
    **************************************************************/
   SExamMsg(&nwByte[0], mBuf, EG_LEN_TWO);
   SExamMsg(&nwByte[1], mBuf, EG_LEN_THREE);

   tmpU16 = PutHiByte(tmpU16, nwByte[0]); 
   tmpU16 = PutLoByte(tmpU16, nwByte[1]); 

   /**************************************************
    * store the length in the partial info structure *
    **************************************************/
   partialMsg->iniMsgLen = tmpU16; 

   /**********************************************************************
    * The total mBuf length should be greater than the (Initial message  *
    * length + 4 bytes Mandatory information of the Initial message) for *
    * it to be valid piggy backed message. If this validation fails then *
    * return with overall invalid length error.                          *
    **********************************************************************/
   if(bufLen > (partialMsg->iniMsgLen + EG_LEN_FOUR))
   {
     /********************************
      * Re-Initialize the temp array *
      ********************************/
     nwByte[0] = 0; nwByte[1] = 0;

     /***********************************************
      * Fetch the Length of the piggybacked message *
      * in the mBuf received                        *
      ***********************************************/
     SExamMsg(&nwByte[0], mBuf,(partialMsg->iniMsgLen + EG_LEN_FOUR + EG_LEN_TWO));
     SExamMsg(&nwByte[1], mBuf,(partialMsg->iniMsgLen + EG_LEN_FOUR + EG_LEN_THREE));

     tmpU16 = PutHiByte(tmpU16, nwByte[0]); 
     tmpU16 = PutLoByte(tmpU16, nwByte[1]); 

     /**************************************************
      * store the length in the partial info structure *
      **************************************************/
     partialMsg->pbMsgLen = tmpU16; 

     /***************************************************************************
      * The total mBuf length should be equal to the (Initial message length +  *
      * piggybacked Message length + 8 bytes Mandatory information of both the  * 
      * for it to be valid piggy backed message. If this validation fails then  *
      * return with overall invalid length error.                               *
      ***************************************************************************/
     if(bufLen != (partialMsg->iniMsgLen + partialMsg->pbMsgLen + EG_LEN_EIGHT))
     {
        /*-- eg003.201: use format specifier according to datatype --*/
#ifdef LONG_MSG
       EG_DBG_ERR(0, 0, (egp,"\n Invalid Length of PiggyBacked Msg (TotLen:%ld iniMLen:%ld pbMlen:%ld)",bufLen,
                               partialMsg->iniMsgLen,partialMsg->pbMsgLen));
#else
       EG_DBG_ERR(0, 0, (egp,"\n Invalid Length of PiggyBacked Msg (TotLen:%d iniMLen:%d pbMlen:%d)",bufLen,
                               partialMsg->iniMsgLen,partialMsg->pbMsgLen));
#endif
       EG_RETVALUE(EGT_ERR_MSG_INVOVR_LEN_PBMSG);
     }
   }
   else
   {
       /*********************************************
        * Length is inconsistent, return with error *
        *********************************************/
#ifdef LONG_MSG
       EG_DBG_ERR(0, 0, (egp,"\n Invalid Length of PiggyBacked Msg (TotLen:%ld iniMLen:%ld)",bufLen,partialMsg->iniMsgLen));
#else
       EG_DBG_ERR(0, 0, (egp,"\n Invalid Length of PiggyBacked Msg (TotLen:%d iniMLen:%d)",bufLen,partialMsg->iniMsgLen));
#endif
       EG_RETVALUE(EGT_ERR_MSG_INVOVR_LEN_PBMSG);
   }

   EG_RETVALUE(ROK);

} /* egTptValidatePBMsg */

#endif/* EGTP_C ends here */
/********************************************************************30**

         End of file:     pg_tpt.c@@/main/3 - Fri Feb  4 18:18:16 2011

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
/main/1      ---          ad                1. Created for Initial Release 1.1
/main/2      ---          rsharon           1. Initial for eGTP 1.2 Releas
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss               2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/2      eg002.102    rss           1.  Used local pst structure and modify it locally
/main/2      eg003.102    rss               1.  Removed cm_xtree from the include list.
             eg004.102    rrm               1.  Correcting the instance id 
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3      eg003.201    psingh            1. Merged code form eg008.102, Added EGTP_C flag to avoid error
                                            2. eg009.102:  Echo Req/Rsp have valid Destination
/main/3      eg004.201    magnihotri        1. Header files added for eGTP-C PSF
                                            2. Warnings fixed for eGTP-C by modifying the function PROTOTYPE
                                               from PRIVATE to PUBlIC in the qualified
                                            3. suConnId updated for updating ReTx CB in seqNoCb for PSF
                                            4. Send run time update for seqNoCb and updating ReTx CB to STANDBY
                                               when a new message comes for a tunnel from the upper layer
                                            5. Send run time update for seqNoCb when ReTx message removed from ReTx queue
                                            6. Send run time update for seqNoCb and updating ReTx CB  to STANDBY
                                               when a new message comes for a tunnel from the upper layer 
                                            7. Send run-time update for deleting seqNoCb when message with 
                                               MSB removed from ReTx queue 
                                            8. Dispatch run-time update for TSAP cb when tptsrvrs closed
                                            9. Dispatch run-time update for TSAP cb when tptsrvrs closed
                                            10. Issue connection termination request for a TSAP to TUCL only if node  is Active
                                            11. Delete mapping for TPTSERVER CB when tptServers is removed
                                            12. Memory leak fix:On Shutdown serverCb->msgHlCp and seqNoCb->mBuf
                                                memory were not being freed resulting in memory leak
                                            13. Modified to send error message on failure to start timer to 
                                                LM only from ACTIVE NODE
                                            14. Modified to resend message present in the ReTx queue to the PEER node  
                                                only from ACTIVE NODE
                                            15. Stop ReTx timer and delete mapping for seqNoCb when the associated message
                                                response received from the peer node
                                            16. Intialize EgPartialMsg object for consistency
/main/3      eg006.201    psingh            1. Merged code form eg010.102, which include Return Error on Alloc failure, Sending
                                                Error Ind for EGT_ERR_MSG_UNABLE_TO_COMPLY msg & Dest Address validation 
                                            2. Update to Expect Echo Req/Resp on Default server
                                            3. Update to provide Msgtype to identify the retransmit msg
                                            4. Raise alarm to layer manager when Path becomes unreachable
                                            5. Send Error indication to application when Path 
                                               becomes unreachable
                                            6. Check Retransmit msgtype when performing Retransmit queue 
                                               removal
/main/3      eg007.201    psingh            1. Updated references of tSapCb from egCb.tSapCb to the srvCb->tSapCb
                                            2. Used MSB of sequence number for queuing of message
/main/3      eg009.201    asaurabh          1. Alarm for a unknown peer is sent to application and return
                                            2. Added TSapCb to args of egGenTrcInd()
                                            3. In egTptProcessMsg, updating the Statistics with upper sap
                                            4. Destination is not present in destCbList, indicates that
                                               destination is deleted from the list, present an error & return 
                                            5. Send Indication & alarm only if the destination is marked Reachable.
                                               For a destination marked unreachable, only Error log will be thrown
                                            6. Fill in transId & Teid from SeqCb into Event structure
                                            7. Free memory allocated for storing Hostname value  
                                            8. Obtained the value of MSB of seqNo to process timeout msg accordingly
/main/3      eg010.201    asaurabh          1. Based on piggyBack status,send/buffer piggyback message
/main/3      eg012.201    shpandey          1. in function egTptProcessMsg:Added NULL check for teCCb 
                                            2. Added egtpMsgType, fix for ccpu00125389
                                            3. Messages from 64 to 70 need not be queued ,fix for ccpu00125389
                                            4. Modified prototype,fix for ccpu00125389
                                            5. EGTP Message Type,fix for ccpu00125389
                                            6. Memory leak fix for ccpu00130148
					    7. in function  egTptQueueMsgRmv:Added memory alloc failure
                                            8. Memory leak fix for ccpu00128780
/main/3      eg013.201    shpandey          1. Added result type for error message in egTptQueueMsgRmvWthMsb
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
********************************************************************91*/
