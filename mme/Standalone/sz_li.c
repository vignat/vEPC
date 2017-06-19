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




/***********************************************************************
^L
************************************************************************
 
     Name: S1AP Layer
          
     Type: C Soruce code File
                         
     Desc: Primitives for lower interface module of S1AP
                                            
     File: sz_li.c
               
     Sid:      sq_li.c@@/main/nodal_main/akaranth_feat_lpdev_branch/1 - Tue Sep 29 21:39:58 2015
   
     Prg: ms
                                                                                                                             
***********************************************************************/

/* Header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#ifdef SZ_FTHA
#include "cm_psfft.h"
#include "cm_ftha.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#endif /* SZ_FTHA */

#include "sct.h"           /* SCTP Layer */
#include "szt.h"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "sht.h"           /* System Agent */
#include "lyt.h"
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP LM Interface */
#include "sz.h"            /* S1AP Layer */
#include "sz_err.h"        /* S1AP Error */
#include "szt_asn.h"       /* S1AP ASN */
#include "cm_pasn.h"
#ifdef YT
#include "yt.h"
#endif /* YT */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* Header include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_pftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif /* SZ_FTHA */

#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCTP Layer */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "sht.x"           /* System Agent */
#include "lyt.x"
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP LM Interface */
#include "cm_pasn.x"
#include "sz.x"            /* S1AP Layer */
 
#ifdef SS_DIAG
#include "ss_diag.h"        /* Include for the log file */
#endif
#ifdef YT
#include "yt.x"
#endif /* YT */

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

EXTERN U8 rspMsg[];

PRIVATE S16 szLiHdlDecFail ARGS((CmPAsnErr asnErr,S1apPdu *pdu,
                                 SzPeerCb *peerCb));


#ifdef SZ_MME
PFSZLIMSGHANDLER szLiRcvMme[SZ_MAX_MSG_ID] =
{
   szLiCOMsgHandler,     /* E-RAB setup request */
   szLiCOMsgHandler,     /* E-RAB setup response */
   szLiCOMsgHandler,     /* E-RAB modify request */
   szLiCOMsgHandler,     /* E-RAB modify response */
   szLiCOMsgHandler,     /* E-RAB release command */
   szLiCOMsgHandler,     /* E-RAB release complete */
   szLiCOMsgHandler,     /* E-RAB release request */
   szLiCOMsgHandler,     /* Initial Context setup request */
   szLiCOMsgHandler,     /* Initial Context setup response */
   szLiCOMsgHandler,     /* Initial Context setup failure */
   szLiCOMsgHandler,     /* UE Context release request */
   szLiCOMsgHandler,     /* UE Context release command */
   szLiCOMsgHandler,     /* UE Context release complete */
   szLiCOMsgHandler,     /* UE Context modification request */
   szLiCOMsgHandler,     /* UE Context modification response */
   szLiCOMsgHandler,     /* UE Context modification failure */
   szLiCOMsgHandler,     /* Initial UE message */
   szLiCOMsgHandler,     /* Downlink NAS transport */
   szLiCOMsgHandler,     /* Uplink NAS transport */
   szLiCOMsgHandler,     /* NAS non delivery indication */
   szLiCOMsgHandler,     /* Error indication */
   szLiCOMsgHandler,     /* No Data */
   szMgmtLiRst,          /* Reset */
   szMgmtLiRstAck,       /* Reset Acknowledge */
   szMgmtLiSetupReq,     /* Setup Request */
   szLiPeerErrHandler,   /* Setup Response */
   szLiPeerErrHandler,   /* Setup Fail */
   szLiPeerErrHandler,   /* Paging */
   szLiCOMsgHandler,     /* Handover Required */
   szLiCOMsgHandler,     /* Handover Command */
   szLiCOMsgHandler,     /* Handover Preperation Failure */
   szLiCOMsgHandler,     /* Handover Request */
   szLiCOMsgHandler,     /* Handover Request Acknowledge */
   szLiCOMsgHandler,     /* Handover Request Failure */
   szLiCOMsgHandler,     /* Handover Notify */
   szLiCOMsgHandler,     /* Path Switch Request */
   szLiCOMsgHandler,     /* Path Switch Request Acknowledge */
   szLiCOMsgHandler,     /* Path Switch Request Failure */
   szLiCOMsgHandler,     /* Handover Cancel */
   szLiCOMsgHandler,     /* Handover Cancel Acknowledge */
   szLiCOMsgHandler,     /* ENB Status Transfer */
   szLiCOMsgHandler,     /* MME Status Transfer */
   szLiCOMsgHandler,     /* Deactivate Trace */
   szLiCOMsgHandler,     /* Trace Start */
   szLiCOMsgHandler,     /* Trace Failure Indication */
   szLiCOMsgHandler,     /* Location Reporting Control */
   szLiCOMsgHandler,     /* Location Report Failure Indication */
   szLiCOMsgHandler,     /* Location Report */
   szMgmtLiMsgHandler,   /* ENB Configuration Update */
   szLiPeerErrHandler,   /* ENB Configuration Update Acknowledge */
   szLiPeerErrHandler,   /* ENB Configuration Update Failure */
   szLiPeerErrHandler,   /* MME Configuration Update */
   szMgmtLiMsgHandler,   /* MME Configuration Update Acknowledge */
   szMgmtLiMsgHandler,   /* MME Configuration Update Failure */
   szLiCOMsgHandler,     /* Uplink CDMA 2000 Tunneling */
   szLiCOMsgHandler,     /* Downlink CDMA 2000 Tunneling */
   szLiCOMsgHandler,     /* UE Capability Info Indication */
   szLiPeerErrHandler,   /* Overload Start */
   szLiPeerErrHandler,   /* Overload Stop */
   szLiPeerErrHandler,   /* Write Replace Warning Request */
   szMgmtLiMsgHandler,   /* Write Replace Warning Response */
   szMgmtLiMsgHandler,   /* ENB Information Direct Transfer */
   szLiPeerErrHandler,   /* MME Information Direct Transfer */
   szLiPeerErrHandler,   /* Private Message */
   szMgmtLiMsgHandler,   /* ENB Configuration Transfer */
   szLiPeerErrHandler,   /* MME Configuration Transfer */
   szLiCOMsgHandler     /* Cell Traffic Trace */
#ifndef S1AP_REL8A0   /* Messages are valid only for release 9.2.2. */
   ,
   szLiPeerErrHandler,   /* Kill Request */
   szMgmtLiMsgHandler,   /* Kill Response */
   szLiCOMsgHandler,     /* Downlink UE Associated LPPa Transport */
   szLiCOMsgHandler,     /* Uplink UE Associated LPPa Transport */
   szLiPeerErrHandler,   /* Downlink non-UE Associated LPPa Transport */
   szMgmtLiMsgHandler    /* Uplink non-UE Associated LPPa Transport */
#endif /* S1AP_REL8A0 */
};
#endif /* SZ_ENB */

/*
*
*       Fun:   szLiSndToLower
*
*       Desc:  This function is used to send the S1AP PDU to lower interface
*              i.e SCTP 
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiSndToLower 
(
SzPeerCb   *peerCb,     /* Peer control block */
SzConCb    *conCb,      /* Connection control block */
S1apPdu    *pdu         /* Pdu to be sent to lower layer */
)
#else /* ANSI */
PUBLIC S16 szLiSndToLower (peerCb, conCb, pdu) 
SzPeerCb   *peerCb;     /* Peer control block */
SzConCb    *conCb;      /* Connection control block */
S1apPdu    *pdu;        /* Pdu to be sent to lower layer */
#endif /* ANSI */
{
   S16           retVal = ROK;
   Mem           mem;              /* Memory block */
   Buffer       *mBuf = NULLP;    /* Encoded Buffer */
   SzSctSapCb   *sctSap   = NULLP;
   SzTrc         trcParam;

   TRC2(szLiSndToLower);

   /* Retrieve the sctSap control block */
   sctSap = peerCb->sctSapCb;

   /* If endpoint is not in open state  or assoc state isnot connected
    * free the pdu and return error 
    */
   if ((sctSap->state != LSZ_SAP_ISBOUND) || 
       (sctSap->endpCb.epState != SZ_ENDP_OPEN) || 
       (peerCb->assocCb->assocState != SZ_ASSOC_UP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ006, (ErrVal) ERRZERO,
                 "szLiSndToLower: Invalid sap state or endp is not open \
                  or association is not UP \n");
#endif /* ERRCLS */   

      SZ_FREE_SDU(pdu);

      RETVALUE(RFAILED);
   } 

   mem.region = sctSap->pst.region;
   mem.pool   = sctSap->pst.pool;
   
   /* Encode the pdu */
   if ((retVal = szEncode(&mem, &pdu, &mBuf)) != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ007, (ErrVal) ERRZERO,
            "szLiSndToLower: Encoding of PDU fails. Dropping the Pdu\n");
#endif /* if ERRCLASS */

      /* Update the global statistics */
      szCb.genSts.nmbEncErr++;

      SZ_FREE_SDU(pdu);
      SZ_FREE_BUF(mBuf);

      RETVALUE(retVal);
   }
#ifdef SS_DIAG
	/* sz008.301 : Change for Diagnostics */
	SZ_DIAG_MSG(mBuf, "Sending S1AP Message : ");
#endif
   /* If trace flag is enabled send the trace indication */
   {
      S32   tLen = 0;

      if (sctSap->trcLen != 0)
         tLen = sctSap->trcLen;
      else if (peerCb->trcLen != 0)
         tLen = peerCb->trcLen;

      if (tLen != 0)
      {
         cmMemset((U8*)&trcParam, 0, sizeof(SzTrc));

         /* Populate the trace params */
         trcParam.event = SCT_EVTDATREQ;

         cmMemcpy((U8*)&trcParam.srcAddr.ipAddr, 
                  (U8*)&sctSap->endpCb.srcAddrLst, 
                  sizeof(SctNetAddrLst));

         trcParam.srcAddr.sctPort = sctSap->endpCb.port; 

         cmMemcpy((U8*)&trcParam.dstAddr.ipAddr, 
                  (U8*)&peerCb->assocCb->assocCfg->dstAddrLst, 
                  sizeof(SctNetAddrLst));

         trcParam.sapId = sctSap->spId;

         trcParam.dstAddr.sctPort = peerCb->assocCb->assocCfg->dstPort;

         if(conCb != NULLP)
         {
            trcParam.suConnId = conCb->cd.suConnId;
         }

         szSendLmTrc(tLen, &trcParam, mBuf);
      }
   }
   
   /* Call SctDatReq primitive to send the pdu to lower layer
    * If pdu is for common management procedure, send the
    * pdu over stream allocated for common procedure 
    */
   if (conCb == NULLP)
   {
      SzLiSctDatReq(&sctSap->pst, sctSap->spId,
                    peerCb->assocCb->spAssocId,
                    &peerCb->assocCb->assocCfg->dstAddrLst.nAddr[0],
                    peerCb->assocCb->assocCfg->comProcStrm.val,
                    FALSE, FALSE, 0, SCT_PROTID_S1AP, mBuf);
   }  
   else
   { 
      SzLiSctDatReq(&sctSap->pst, sctSap->spId,
                    peerCb->assocCb->spAssocId,
                    &peerCb->assocCb->assocCfg->dstAddrLst.nAddr[0],
                    conCb->cd.streamNo, FALSE, FALSE, 0, SCT_PROTID_S1AP,
                    mBuf);
   }

   /* Update the transmiting end statistics */
   SZ_STS_INC_DATA_TX_SCT(peerCb, pdu, conCb);

   SZ_FREE_SDU(pdu);

   RETVALUE(ROK);

} /* end of szLiSndToLower */


/*
*
*       Fun:   szLiRcvFromLower
*
*       Desc:  This function is used to process the PDU recieved from the lower
*              interface i.e SCTP 
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiRcvFromLower 
(
Buffer    *mBuf,       /* Pdu recevied from lower layer */
SzPeerCb  *peerCb      /* Peer control block */
)
#else /* ANSI */
PUBLIC S16 szLiRcvFromLower(mBuf,peerCb)
Buffer    *mBuf;        /* Pdu recevied from lower layer */
SzPeerCb  *peerCb;      /* Peer control block */
#endif /* ANSI */
{
   S16      retVal = ROK;
   U8       evnt = 0;
   S1apPdu  *pdu = NULLP;        /* Decoded S1AP Pdu */
   Mem      mem;      /* Memory block */
   SzTrc    trcParam;
   CmPAsnErr asnErr;
   
   TRC2(szLiRcvFromLower);

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   if (mBuf == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
   SZLOGERROR(ERRCLS_INT_PAR, ESZ008, (ErrVal) ERRZERO,
         "szLiRcvFromLower: NULL mBuf received.Dropping the mBuf\n");
#endif /* ERRCLS */  
      RETVALUE(ROK);
   }
  
   cmMemset((U8*)&asnErr, 0, sizeof(CmPAsnErr));

   /* If trace flag is enabled send the trace indication */
   {
      S32   tLen = 0;

      if (peerCb->sctSapCb->trcLen != 0)
         tLen = peerCb->sctSapCb->trcLen;
      else if (peerCb->trcLen != 0)
         tLen = peerCb->trcLen;

      if (tLen != 0)
      {
         cmMemset((U8*)&trcParam, 0, sizeof(SzTrc));

         /* Populate the trace params */
         trcParam.event = SCT_EVTDATIND;

         cmMemcpy((U8*)&trcParam.srcAddr.ipAddr, 
                  (U8*)&peerCb->sctSapCb->endpCb.srcAddrLst, 
                  sizeof(SctNetAddrLst));

         trcParam.srcAddr.sctPort = peerCb->sctSapCb->endpCb.port; 

         cmMemcpy((U8*)&trcParam.dstAddr.ipAddr, 
                  (U8*)&peerCb->assocCb->assocCfg->dstAddrLst, 
                  sizeof(SctNetAddrLst));

         trcParam.sapId = peerCb->sctSapCb->spId;

         trcParam.dstAddr.sctPort = peerCb->assocCb->assocCfg->dstPort;

         szSendLmTrc(tLen, &trcParam, mBuf);
      }
   }
#ifdef SS_DIAG
   /* sz008.301 : Change for Diagnostics
    * Sending the ASN buffer for dump */
   SZ_DIAG_MSG(mBuf, "Recieved S1AP Message :");
#endif

   /* Decode the message */
   if ((retVal = szDecode(&mem, &asnErr, &pdu, &mBuf)) != ROK)
   {   
#if (ERRCLASS & ERRCLS_INT_PAR)
   SZLOGERROR(ERRCLS_INT_PAR, ESZ009, (ErrVal) ERRZERO,
         "szLiRcvFromLower: Decoding of pdu fails.Dropping the mBuf\n");
#endif /* ERRCLS */    

      szLiHdlDecFail(asnErr, pdu, peerCb);
      RETVALUE(RFAILED);
   } /* decoding of msg fails */

   /* Free mBuf */
   SZ_FREE_BUF(mBuf);

   szUtilsGetMsgIdx(&evnt, pdu);
   
   /* If reset procedure is going on, free the pdu and * return failure */
   if ((peerCb->state == SZT_ND_ST_RESET) && (evnt != SZT_MSG_ID_RESET_ACK) && (evnt != SZT_MSG_ID_RESET))
   {
      SZDBGP(DBGMASK_LI, (szCb.init.prntBuf, 
             "szLiRcvFromLower : Peer is under reset. Ignoring the \
              Dat indication \n"));

      SZ_FREE_SDU(pdu);

      RETVALUE(RFAILED);
   }

   SZ_STS_INC_DATA_RX_PDU(peerCb, evnt);
 
   /* If the S1AP message is common management procedure,
    * call appropriate handler else invoke state machine
    */

#ifdef SZ_MME    
   if (peerCb->nodeType == LSZ_NODE_ENB)
   {
      retVal = szLiRcvMme[evnt](peerCb, pdu);

      if (retVal != ROK)
      {
         SZ_FREE_SDU(pdu);
      }
   } /* end of if */
#endif /* SZ_MME */

   RETVALUE(ROK);
} /* end of szLiRcvFromLower */

/*
*
*       Fun:   szLiCOMsgHandler  
*
*       Desc:  Called to send semantic error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: Incase if the bypassing of UE state machine is enabled
*              the decoded message is passed to user by UDatInd.
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiCOMsgHandler 
(
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szLiCOMsgHandler(peerCb, pdu)
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   SzConCb         *conCb = NULLP;
   SztUE_S1AP_IDs  *ueS1apId = NULLP;
   /* sz008.301 : Added UE_S1AP_IDS_MME_UE_S1AP_ID to decode,
    * in the Ue Context Release Command message. */
   TknU32           *mmeIdTkn = NULLP; 
   U8    evnt = 0;

   TRC2(szLiCOMsgHandler)

   SZDBGP(DBGMASK_LI, (szCb.init.prntBuf, 
     "szLiCOMsgHandler : Received dedicated procedure\n"));

   /* The UE state machine will be bypassed in case of skip UE sm flag is set
    * else the message is handled as usual */
#ifdef LSZV1
   if(peerCb->skipUeSm)
   {
      SztUDatEvnt uDatEvnt;
      S16         ret;

      if (peerCb->state != SZT_ND_ST_NORMAL)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR(ERRCLS_INT_PAR, ESZ010, (ErrVal)peerCb->peerId,
               "szLiCOMsgHandler: Node is not in valid state \
               to receive message");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
         RETVALUE(RFAILED);
      }

      SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));

      uDatEvnt.transId.pres = NOTPRSNT;
      uDatEvnt.peerId.pres = PRSNT_NODEF;
      uDatEvnt.peerId.val   = peerCb->peerId;
      uDatEvnt.pdu          = pdu;

      ret = szUiSndUDatInd(peerCb, &uDatEvnt);
      if (ret != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SZLOGERROR(ERRCLS_DEBUG, ESZ011, (ErrVal)peerCb->peerId,
               "szLiCOMsgHandler: Failed to send data to the user");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         RETVALUE(RFAILED);
      }
   }
   else
   {
#endif /* LSZV1 */

      szUtilsGetMsgIdx(&evnt, pdu);


#ifdef SZ_MME    
      if(peerCb->nodeType == LSZ_NODE_ENB)
      {
         /* Get mmeId from S1AP Msg, if it exsits */
         szGetIE(&pdu->pdu, Sztid_MME_UE_S1AP_ID, (TknU8 **)&mmeIdTkn);
         if (mmeIdTkn != NULLP)
         {
            cmHashListFind(&peerCb->conRefLst, 
                  (U8*) &(((SztMME_UE_S1AP_ID *)mmeIdTkn)->val),
                  sizeof(U32), 0, (PTR*)&conCb);
         }
      } /* end of if */
#endif /* SZ_MME */

      /* Invoke state machine */
      szSmExcMt(conCb, peerCb, pdu, evnt, SZ_LI);
#ifdef LSZV1
   }
#endif /* LSZV1 */

    RETVALUE(ROK);
} /* end of szLiCOMsgHandler */

/*
*
*       Fun:   szLiPeerErrHandler  
*
*       Desc:  Called to send semantic error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiPeerErrHandler 
(
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szLiPeerErrHandler(peerCb, pdu)
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   S16      ret = RFAILED;       /* return value */
   S1apPdu  *errPdu;             /* Response PDU */
   Mem      mem;                 /* Memory block */

   TRC2(szLiPeerErrHandler)

   errPdu = NULLP;

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   /* Call function to generate errPdu */
   ret = szLiErrHndlr(pdu, CAUSE_PROTOCOL,
                      SztCauseProtsemantic_errorEnum, &errPdu, mem);

   peerCb->peerSts.cfm.nmbUnexpEvt++;

#ifdef LSZV1
   /* Increment the error statistics */
   szIncErrSts(peerCb, CAUSE_PROTOCOL, SztCauseProtsemantic_errorEnum);
#endif /* LSZV1 */
            
   /* Send decoded data to its User */
   ret = szLiSndToLower(peerCb, NULLP, errPdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ret != ROK)
   {
      SZLOGERROR(ERRCLS_INT_PAR, ESZ012, (ErrVal) peerCb->peerId,
           "szLiPeerErrHandler: Lower Interface send failed");
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   SZ_FREE_SDU(pdu);

   RETVALUE(ret);
} /* end of szLiPeerErrHandler */

/*
*
*       Fun:   SzLiSctBndCfm
*
*       Desc:  This function is used to process the bind confirm
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctBndCfm 
(
Pst *pst,               /* Post structure */
SuId suId,              /* Service user Id */
SctResult result        /* Status of bind request */
)
#else /* ANSI */
PUBLIC S16 SzLiSctBndCfm (pst, suId, result)
Pst *pst;               /* Post structure */ 
SuId suId;              /* Service user Id */
SctResult result;       /* Status of bind request */
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   SzSctSapCb *sctSapCb = NULLP;
#ifdef SS_MULTIPLE_PROCS
   S16         ret = RFAILED;
#endif /* end of SS_MULTIPLE_PROCS */

   TRC3(SzLiSctBndCfm);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ013,0,
                "SzLiSctBndCfm() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctBndCfm-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif   /* end of DEBUGP */                        


   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ014, (ErrVal) suId,
                 "SzLiSctBndCfm: Invalid suId \n");
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SPID);
#endif /* if ERRCLS */
      RETVALUE(RFAILED);
   }

   /* If SapCb is NULLP then return invalid sap */
   if (!szCb.sctSapLst || ( (sctSapCb = szCb.sctSapLst[suId]) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ015, (ErrVal) suId,
                 "SzLiSctBndCfm: Invalid sap \n");
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_BND_CFM, LCM_CAUSE_INV_SAP);
#endif /* if ERRCLS */
      RETVALUE(RFAILED);
   }

   /* Check if sap stats is already in bound state */
   if (sctSapCb->state == LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ016, (ErrVal) LSZ_SAP_ISBOUND,
                 "SzLiSctBndCfm: SCT SAP is already bound"); 
#endif /* if ERRCLS */
      RETVALUE(ROK);
   } /* sap is already bound */
#ifdef SS_DIAG   
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03070001, SZ_DIAG_NA, SS_DIAG_INV_ARG,
               "Received BndCfm from SCTP suId  = %d, spId  = %d, result  = %d",
                suId, sctSapCb->spId, result, 0)
#endif
   /* Check the status of bind request */
   if(result == CM_BND_NOK)
   {
      /* Change the state of lower Sap to config */
      sctSapCb->state = LSZ_SAP_CFG;

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ017, (ErrVal) 0,
                 "SzLiSctBndCfm: Bind fail \n");
#endif /*ERRCLASS & ERRCLS_INT_PAR */
      /* Send an alarm with LCM_EVENT_BIND_FAIL */
      SZ_SEND_SAPID_ALARM(suId, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);

      /* Stop the bind timer. */
      szStopTmr(&sctSapCb->timers[SZ_BND_TMR]);
      RETVALUE(RFAILED);
   } 
   else
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_LI, (szCb.init.prntBuf, 
             "S1AP:SzLiSctBndCfm - Binding with lsap (%d) successful\n", suId));
#endif /* DEBUGP */
      /* Change the state of the sap to bound */
      sctSapCb->state = LSZ_SAP_ISBOUND;

      /* Stop the bind timer */
      szStopTmr(&sctSapCb->timers[SZ_BND_TMR]);

      /* Raise an alarm with LCM_EVENT_BND_OK */
      SZ_SEND_SAPID_ALARM(suId, LCM_EVENT_BND_OK, LCM_CAUSE_UNKNOWN);
      
      /* Since there is going to be one end point per sct sap, we can have
       * suId as suEndpId, update the same in endpCb
       */
      sctSapCb->endpCb.locSuEndpId = sctSapCb->suId;

      /* Change the endpoint state before sending endp open request
       * as the endp cfm will change the state in case of tight coupling */
      /* Change the state of endpoint to OPEN_IN_PRG */
      sctSapCb->endpCb.epState = SZ_ENDP_OPEN_IN_PRG;

      /* Send end point open request and use suId as suendpId */
#ifdef SCT_ENDP_MULTI_IPADDR      
      SzLiSctEndpOpenReq(&sctSapCb->pst, sctSapCb->spId, sctSapCb->suId,
                         sctSapCb->endpCb.port,
                         &sctSapCb->endpCb.srcAddrLst);
#else
      SzLiSctEndpOpenReq(&sctSapCb->pst, sctSapCb->spId, sctSapCb->suId,
                         sctSapCb->endpCb.port,
                         &sctSapCb->endpCb.srcAddrLst.nAddr[0]);
#endif /* SCT_ENDP_MULTI_IPADDR */

   } /* end of if (result == OK) */

#ifdef YT
   YT_RTUPD_MOD(YT_UPD_SCT_SAP_CB, sctSapCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzLiSctBndCfm */




/*
*
*       Fun:   SzLiSctEndpOpenCfm
*
*       Desc:  This function is used to process the endp open confirm
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctEndpOpenCfm 
(
Pst        *pst,              /* Post structure */
SuId       suId,              /* Service user Id */
UConnId    suEndpId,          /* Service user endp Id */
UConnId    spEndpId,          /* Service provider endpId */
SctResult  result,            /* Result of endp open request */
SctCause   cause              /* Cause for endp open failure */
)
#else /* ANSI */
PUBLIC S16 SzLiSctEndpOpenCfm (pst, suId, suEndpId, spEndpId, result, cause)
Pst        *pst;              /* Post structure */
SuId       suId;              /* Service user Id */
UConnId    suEndpId;          /* Service user endp Id */
UConnId    spEndpId;          /* Service provider endpId */
SctResult  result;            /* Result of endp open request */
SctCause   cause;             /* Cause for endp open failure */
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   SzSctSapCb *sctSapCb = NULLP;
#ifdef SS_MULTIPLE_PROCS
   S16         ret = RFAILED;
#endif /* end of SS_MULTIPLE_PROCS */

   TRC3(SzLiSctEndpOpenCfm);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ018,0,
                "SzLiSctEndpOpenCfm() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctEndpOpenCfm-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif   /* end of DEBUGP */                        


   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ019, (ErrVal) suId,
                 "SzLiSctEndpOpenCfm: Invalid suId \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_OPEN_CFM,
            LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }
 
   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ( (sctSapCb = szCb.sctSapLst[suId]) == NULLP) ||
       (sctSapCb->state != LSZ_SAP_ISBOUND))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ020, (ErrVal) suId,
                 "SzLiSctEndpOpenCfm: Invalid sap \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_OPEN_CFM,
            LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }
  
   /* Check the result. If endpopen request fails, 
    * then return the error 
    */
   if (result != SCT_OK)
   {
      /* Change the state of end point from OPEN_IN_PRG to CFG */
      sctSapCb->endpCb.epState = SZ_ENDP_CFG;
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_DEBUG, ESZ021, (ErrVal) 0,
                 "SzLiSctEndpOpenCfm: Endp open request fails \n");
#endif /* if ERRCLS */   
      /* Raise an alarm with the cause received */
      switch(cause)
      {
         case SCT_CAUSE_INV_PAR_VAL:
         {
            cause = LCM_CAUSE_INV_PAR_VAL;    
            break;
         }     
              
         case SCT_CAUSE_ENDP_OPEN:
         {
            cause = LSZ_CAUSE_ENDP_OPEN;    
            break;
         }   
              
         case SCT_CAUSE_EXCEED_CONF_VAL:
         {
            cause = LSZ_CAUSE_EXCEED_CONF_VAL;    
             break;
         }     

         case SCT_CAUSE_OUTRES:
         {
            cause = LSZ_CAUSE_OUTRES;    
            break;
         }   

         default:
         break;
      } /* end of switch */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_OPEN_CFM, cause);
      RETVALUE(RFAILED);
   }
   else
   {
      /* Change the state of end point from OPEN_IN_PRG to OPEN */
      sctSapCb->endpCb.epState = SZ_ENDP_OPEN;
      /* Update the spEndpId in sctSapCb */
      sctSapCb->endpCb.locSpEndpId = spEndpId;
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_OPEN_CFM,
            LSZ_CAUSE_SCT_ENPOPEN_SUCC);

   } /* end of if result == OK */

   /* Send the runtime update message for endpoint */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_ENDP_CB, &sctSapCb->endpCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzLiSctEndpOpenCfm */



/*
*
*       Fun:   SzLiSctEndpCloseCfm
*
*       Desc:  This function is used to process the endp close confirm
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctEndpCloseCfm 
(
Pst        *pst,              /* Post structure */
SuId       suId,              /* Service user Id */
UConnId    suEndpId,          /* Service user endp Id */
SctResult  result,            /* Result of endp close request */
SctCause   cause              /* Cause for endp close failure */
)
#else /* ANSI */
PUBLIC S16 SzLiSctEndpCloseCfm (pst, suId, suEndpId, result, cause)
Pst        *pst;              /* Post structure */
SuId       suId;              /* Service user Id */
UConnId    suEndpId;          /* Service user endp Id */
SctResult  result;            /* Result of endp close request */
SctCause   cause;             /* Cause for endp close failure */
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   SzSctSapCb *sctSapCb = NULLP;
#ifdef SS_MULTIPLE_PROCS
   S16         ret = RFAILED;
#endif /* end of SS_MULTIPLE_PROCS */

   TRC3(SzLiSctEndpCloseCfm);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ022,0,
                "SzLiSctEndpCloseCfm() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
   
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctEndpCloseCfm-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif   /* end of DEBUGP */                        

   /* Need not proceed further if the layer has been shut down */ 
   if (szCb.shutdown)
      RETVALUE (ROK);

   /* Check if suID is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR) 
      SZLOGERROR(ERRCLS_INT_PAR, ESZ023, (ErrVal) suId,
                 "SzLiSctEndpCloseCfm: Invalid suId \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_CLOSE_CFM,
            LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }
 
   /* If Sap control block is not valid then return invalid sap */
   if (!szCb.sctSapLst || ( (sctSapCb = szCb.sctSapLst[suId]) == NULLP))
   {
#if (ERRCLASS & ERRCLS_INT_PAR) 
      SZLOGERROR(ERRCLS_INT_PAR, ESZ024, (ErrVal) suId,
                 "SzLiSctEndpCloseCfm: Invalid sap \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_CLOSE_CFM,
            LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }
  
   /*Check the result. If endpclose request fails, 
    * then return the error 
    */
   if (result != SCT_OK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_LI, (szCb.init.prntBuf, 
             "S1AP:SzLiSctEndpCloseCfm - EndpCloseReq fails \n"));
#endif /* DEBUGP */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_CLOSE_CFM,
            LSZ_CAUSE_INV_PAR_VAL);
      RETVALUE(RFAILED);
   }
   else
   {
      /* Change the state of end point cfg */
      sctSapCb->endpCb.epState = SZ_ENDP_CFG;
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ENDP_CLOSE_CFM,
            LSZ_CAUSE_EPCLOSE_OK);
   }

   /* Send the runtime update message for endpoint */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_ENDP_CB, &sctSapCb->endpCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzLiSctEndpCloseCfm */


/*
*
*       Fun:   SzLiSctAssocInd
*
*       Desc:  This function is used to process the association indication
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctAssocInd 
(
Pst               *pst,              /* Post structure */
SuId              suId,              /* Service user Id */
UConnId           suEndpId,          /* Service user endp Id */
SctAssocIndParams *assocParams,      /* Assoc params */
Buffer            *vsInfo            /* Vendor specific info */
)
#else /* ANSI */
PUBLIC S16 SzLiSctAssocInd (pst, suId, suEndpId, assocParams, vsInfo)
Pst               *pst;              /* Post structure */
SuId              suId;              /* Service user Id */
UConnId           suEndpId;          /* Service user endp Id */
SctAssocIndParams *assocParams;      /* Assoc params */
Buffer            *vsInfo;           /* Vendor specific info */
#endif /* ANSI */
{
   /* Variable to store the sct sap control block */
   SzSctSapCb *sctSapCb = NULLP;
   /* Variable to store the peer control block */
   SzPeerCb   *peerCb = NULLP;
   /* Variable to store the return value */
   S16 retVal = ROK;
#ifdef SZ_MME
   S16 ret;
#endif /* SZ_MME */
#ifdef SS_MULTIPLE_PROCS
   S16 ret1;
#endif /* end of SS_MULTIPLE_PROCS */
   
   TRC3(SzLiSctAssocInd);

#ifdef SS_MULTIPLE_PROCS
   ret1 = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret1 !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ025,0,
                "SzLiSctAssocInd() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */
   
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctAssocInd-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif   /* end of DEBUGP */                        


   /* Check for pst structure and assocParams */
   if(!(assocParams))
   {
#if (ERRCLASS & ERRCLS_INT_PAR) 
      SZLOGERROR(ERRCLS_INT_PAR, ESZ026, (ErrVal) 0,
                 "SzLiSctAssocInd: Invalid parameters \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ASSOC_IND, LCM_CAUSE_INV_PAR_VAL);
      RETVALUE(RFAILED);
   }

   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR) 
      SZLOGERROR(ERRCLS_INT_PAR, ESZ027, (ErrVal) suId,
                 "SzLiSctAssocInd: Invalid suId \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ASSOC_IND, LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }
 
   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ( (sctSapCb = szCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR) 
      SZLOGERROR(ERRCLS_INT_PAR, ESZ028, (ErrVal) suId,
                 "SzLiSctAssocInd: Invalid sap \n");
#endif /* if ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ASSOC_IND, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }

   /* Since vendor specific info is not used, if present 
    * free the memory
    */
   if(vsInfo)
   {
     SPutMsg(vsInfo);
     vsInfo = NULLP;
   } /* end of vsInfo */  
 
   /* Retrieve the peer control block based on the peer control address */
   if(assocParams->type == SCT_ASSOC_IND_INIT)
   {
      retVal = szNdbFindPeerAddrPortCb(&assocParams->t.initParams.peerAddrLst, 
                                    assocParams->t.initParams.peerPort, 
                                                                 &peerCb); 
   }
   else
   {
      retVal = szNdbFindPeerAddrPortCb(&assocParams->t.cookieParams.peerAddrLst, 
                                   assocParams->t.cookieParams.peerPort, 
                                   &peerCb); 
   }
   if(retVal != ROK)
   {
#ifdef SZ_MME
      /* do dynamic peer configuration */
      if (szCb.genCfg.dynamPeerFlag == FALSE)
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR(ERRCLS_INT_PAR, ESZ029, (ErrVal) 0,
         "SzLiSctAssocInd: flag for dynamic peer configuration is not enabled\n");
#endif /* if ERRCLASS */

#ifdef SCT3
         SzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId,
                         sctSapCb->endpCb.locSpEndpId,
                         assocParams, 0, SCT_NOK, vsInfo);
#else/* end of SCT3 */
         SzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId,
                         sctSapCb->endpCb.locSpEndpId,
                         assocParams, SCT_NOK, vsInfo);
#endif /* end of SCT3 */

         RETVALUE(RFAILED);
      }

      if ((ret = szLiDynamPeerConfig(suId, assocParams)) != ROK)
      {
#if (ERRCLASS & ERRCLS_INT_PAR) 
         SZLOGERROR(ERRCLS_INT_PAR, ESZ030, (ErrVal) 0,
               "SzLiSctAssocInd: Failed to do the dynamic peer configuration\n");
#endif /* if ERRCLASS */   
#ifdef SCT3
         SzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId,
                         sctSapCb->endpCb.locSpEndpId,
                         assocParams, 0, SCT_NOK, vsInfo);
#else /* end of SCT3 */
         SzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId,
                         sctSapCb->endpCb.locSpEndpId,
                         assocParams, SCT_NOK, vsInfo);
#endif /* end of SCT3 */

         RETVALUE(RFAILED);
      }
#ifdef YT
      YT_UPD_PEER();
#endif /* YT */

      RETVALUE(ROK);
#else /* SZ_MME */

#if (ERRCLASS & ERRCLS_INT_PAR) 
      SZLOGERROR(ERRCLS_INT_PAR, ESZ031, (ErrVal) 0,
                 "SzLiSctAssocInd: Failed to retrieve the peer control block \n");
#endif /* if ERRCLS */   
      RETVALUE(RFAILED);
#endif /* defined(SZ_MME) */

   }
    
   /* suAssocId is same as peerId, so that it would be easy
    * to retrieve the peer control block later. Update suAssocId
    * in assocCb.PeerId is the index in the peerLst (which is an 
    * array of pointers to peerCb 
    */
   peerCb->assocCb->suAssocId = peerCb->peerId;

   /* Update the peerCb with assoc info */
   if (assocParams->type == SCT_ASSOC_IND_INIT)
   {
      /* Update the assocCb */
      peerCb->assocCb->assocCfg->dstPort = assocParams->t.initParams.peerPort;
      cmMemcpy((U8 *)&peerCb->assocCb->assocCfg->dstAddrLst, 
               (U8 *)&assocParams->t.initParams.peerAddrLst,
                sizeof(SctNetAddrLst));
      peerCb->assocCb->assocCfg->locOutStrms = 
         (assocParams->t.initParams.inStrms < assocParams->t.initParams.outStrms)?
              assocParams->t.initParams.inStrms : assocParams->t.initParams.outStrms;


   } /* end of chunk type is INIT */
   else
   {
      /* Update the assocCb */
      cmMemcpy((U8 *)&peerCb->assocCb->assocCfg->dstAddrLst, 
               (U8 *)&assocParams->t.cookieParams.peerAddrLst,
               sizeof(SctNetAddrLst));
      peerCb->assocCb->assocCfg->dstPort= assocParams->t.cookieParams.peerPort;
      peerCb->assocCb->spAssocId = assocParams->t.cookieParams.spAssocId;
      assocParams->t.cookieParams.suAssocId = peerCb->assocCb->suAssocId;
      peerCb->assocCb->peer = peerCb;

#ifdef SCT6
      peerCb->assocCb->assocCfg->locOutStrms = 
         (assocParams->t.cookieParams.inStrms < assocParams->t.cookieParams.outStrms)?
              assocParams->t.cookieParams.inStrms : assocParams->t.cookieParams.outStrms;
#endif /* SCT6 */

      peerCb->assocCb->assocState = SZ_ASSOC_IN_PRG;

   } /* end of if chunk type is COOKIE_CHUNK */
   
   /* Send the association response */
#ifdef SCT3
   SzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId,
                   sctSapCb->endpCb.locSpEndpId,
                   assocParams, 0, SCT_OK, vsInfo);
#else /* end of SCT3 */
   SzLiSctAssocRsp(&sctSapCb->pst, sctSapCb->spId,
                   sctSapCb->endpCb.locSpEndpId,
                   assocParams, SCT_OK, vsInfo);
#endif /* end of SCT3 */

   RETVALUE(ROK);

} /* end of SzLiSctAssocInd */ 


/*
*
*       Fun:   SzLiSctAssocCfm
*
*       Desc:  This function is used to process the association confirmation
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef SCT2
#ifdef ANSI
PUBLIC S16 SzLiSctAssocCfm 
(
Pst               *pst,              /* Post structure */
SuId              suId,              /* Service user Id */
UConnId           suAssocId,         /* Service user assoc Id */
UConnId           spAssocId,         /* Service provider assoc Id */
SctNetAddrLst     *dstNAddrLst,      /* Dst Addres Lst */
SctPort           dstPort,           /* Destination port */
SctStrmId         inStrms,           /* No of in streams */
SctStrmId         outStrms,          /* No of out streams */
Buffer            *vsInfo            /* Vendor specific info */
)
#else /* ANSI */
PUBLIC S16 SzLiSctAssocCfm (pst, suId, suAssocId, spAssocId, dstNAddrLst, dstPort, inStrms, outStrms, vsInfo)
Pst               *pst;              /* Post structure */
SuId              suId;              /* Service user Id */
UConnId           suAssocId;         /* Service user assoc Id */
UConnId           spAssocId;         /* Service provider assoc Id */
SctNetAddrLst     *dstNAddrLst;      /* Dst Addres Lst */
SctPort           dstPort;           /* Destination port */
SctStrmId         inStrms;           /* No of in streams */
SctStrmId         outStrms;          /* No of out streams */
Buffer            *vsInfo;           /* Vendor specific info */
#endif /* ANSI */

#else /* SCT2 */

#ifdef ANSI
PUBLIC S16 SzLiSctAssocCfm 
(
Pst               *pst,              /* Post structure */
SuId              suId,              /* Service user Id */
UConnId           suAssocId,         /* Service user assoc Id */
UConnId           spAssocId,         /* Service provider assoc Id */
SctNetAddrLst     *dstNAddrLst,      /* Dst Addres Lst */
SctPort           dstPort,           /* Destination port */
SctStrmId         outStrms,          /* No of out streams */
Buffer            *vsInfo            /* Vendor specific info */
)
#else /* ANSI */
PUBLIC S16 SzLiSctAssocCfm (pst, suId, suAssocId, spAssocId, dstNAddrLst, dstPort, outStrms, vsInfo)
Pst               *pst;              /* Post structure */
SuId              suId;              /* Service user Id */
UConnId           suAssocId;         /* Service user assoc Id */
UConnId           spAssocId;         /* Service provider assoc Id */
SctNetAddrLst     *dstNAddrLst;      /* Dst Addres Lst */
SctPort           dstPort;           /* Destination port */
SctStrmId         outStrms;          /* No of out streams */
Buffer            *vsInfo;           /* Vendor specific info */
#endif /* ANSI */
#endif /* SCT2 */
{
   TknU32 peerId;
   SzPeerCb   *peerCb = NULLP;
   SzSctSapCb *sctSapCb = NULLP;
#ifdef SS_MULTIPLE_PROCS
   S16         ret = RFAILED;
#endif /* end of SS_MULTIPLE_PROCS */
   
   TRC3(SzLiSctAssocCfm);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ032,0,
                "SzLiSctAssocCfm() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#endif /* end of SS_MULTIPLE_PROCS */
   
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctAssocCfm-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif   /* end of DEBUGP */                        

#ifdef SS_DIAG  
  /* sz008.301 : Change for Diagnostics */ 
  SZ_DIAG_LVL0(0x03070002, SZ_DIAG_NA, SS_DIAG_INV_ARG,
               "Received AssocCfm suAssocId = %ld, spAssocId = %ld",
                suAssocId, spAssocId, 0, 0)
#endif

   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ033, (ErrVal) suId, 
                 "SzLiSctAssocCfm: Invalid suId \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ASSOC_CFM, LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }
 
   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ((sctSapCb = szCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ034, (ErrVal) 0,
                 "SzLiSctAssocCfm: Invalid sap \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_ASSOC_CFM, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }

   /* Since vendor specific info is not used, if present 
    * free the memory
    */
   if(vsInfo)
   {
      SPutMsg(vsInfo);
      vsInfo = NULLP;
   } /* end of vsInfo is not NULL */   
           
   peerId.pres = TRUE;
   peerId.val = suAssocId;
   szNdbChkPeer(peerId, &peerCb);
   if (peerCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SZLOGERROR(ERRCLS_DEBUG, ESZ035, 0,
           "SzLiSctAssocCfm: Failed to retrieve the peer control block \n");
#endif /* if ERRCLS */
      RETVALUE(RFAILED);
   }
  
   /* Update the spAssocId in assocCb */
   peerCb->assocCb->spAssocId = spAssocId;

   /* Send the runtime update message for AssocCb */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_ASSOC_CB, peerCb->assocCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzLiSctAssocCfm */



/*
*
*       Fun:   SzLiSctTermInd
*
*       Desc:  This function is used to process the termination indication
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctTermInd 
(
Pst             *pst,            /* Post structure */
SuId            suId,            /* Service user Id */
UConnId         assocId,         /* Assoc Id */
U8              assocIdType,     /* AssocId type */
SctStatus       status,          /* Status of termination req */
SctCause        cause,           /* Cause if status ! OK */
SctRtrvInfo     *rtrvInfo        /* Retrieve info if any */
)
#else /* ANSI */
PUBLIC S16 SzLiSctTermInd(pst, suId, assocId, assocIdType, status, cause, rtrvInfo)
Pst             *pst;            /* Post structure */
SuId            suId;            /* Service user Id */
UConnId         assocId;         /* Assoc Id */
U8              assocIdType;     /* AssocId type */
SctStatus       status;          /* Status of termination req */
SctCause        cause;           /* Cause if status ! OK */
SctRtrvInfo     *rtrvInfo;       /* Retrieve info if any */
#endif /* ANSI */
{
   /* Variable to store the peer control block */
   S16 ret = RFAILED;
   SzPeerCb   *peerCb = NULLP;
   TknU32 peerId;
   /* Variable to store status indication */
   SztStaInd  sta;
   SzSztSapCb *sztSapCb = NULLP;
   SzSctSapCb *sctSapCb = NULLP;
   
   TRC3(SzLiSctTermInd);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ036,0,
                "SzLiSctTermInd() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#endif /* end of SS_MULTIPLE_PROCS */
   
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctTermInd-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* end of DEBUGP */          


   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ037, (ErrVal) suId,
                 "SzLiSctTermInd: Invalid suId \n");
#endif /* ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_TERM_IND, LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }
 
   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ( (sctSapCb = szCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ038, (ErrVal) 0,
                 "SzLiSctTermInd: Invalid sap \n");
#endif /* ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_TERM_IND, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }

   /* Retrieve the peerCb.Since suAssocId is same as peerId
    * we can directly retireve the peerCb with index suAssocId 
    * in peerLst
    */
   peerId.pres = TRUE;
   peerId.val = assocId;        
   ret = szNdbChkPeer(peerId,&peerCb); 
   if(peerCb == NULLP)
   {
#ifdef SZ_MME
      if (assocIdType == SCT_ASSOCID_SP)
      { 
         SzPeerAddrPortCb *crnt = NULLP;
         SzPeerAddrPortCb *next = NULLP; 
         SzPeerCb         *peer = NULLP;
         Bool             isFound = FALSE;

         while (cmHashListGetNext(&szCb.peerAddrPortLst,
                  (PTR)crnt,
                  (PTR *)&next) == ROK)
         {
            peer = next->peerCbPtr;
            if ((peer->isDynamCfgd) && (peer->assocParams != NULL) &&
                (peer->assocParams->type == SCT_ASSOC_IND_COOKIE) &&
                (peer->sctSapCb->suId == suId) &&
                (peer->assocParams->t.cookieParams.spAssocId == assocId))
            {
               isFound = TRUE;
               break;
            }

            crnt = next;
            next = NULLP;
         }
 
         if (isFound == TRUE)
         {
            szNdbRemPeerAddrPortCb(peer);
            szNdbDeallocDynamPeerCb(peer);
         }

         RETVALUE(ROK);
      }
#endif /* SZ_MME */

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_DEBUG, ESZ039, 0,
                 "SzLiSctTermInd: Failed to retrieve the peer control block \n");
#endif /* if ERRCLASS */
      RETVALUE(RFAILED);
   } /* end of if peerCb == NULLP */

#ifdef SS_DIAG
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03070003, SZ_DIAG_PEERID, peerId.val,
             "Received TermInd from SCTP AssocId = %d, status = %d, cause = %d",
                assocId, status, cause, 0)
#endif
   if (peerCb->state == SZT_ND_ST_SETUP_IN_PRG)
   {
   }
   
   peerCb->state = SZT_ND_ST_IDLE;

   /* Set the assoc state to DOWN */
   peerCb->assocCb->assocState = SZ_ASSOC_DOWN;

   /* Delete all associated connections */
   szNdbRelConns(peerCb);

   /* Populate the sta indication structure */
   cmMemset((U8*)&sta, 0, sizeof(SztStaInd));

   sta.peerId.pres = TRUE;
   sta.peerId.val = peerCb->peerId;
   sta.type = SZT_STA_TYPE_ASSOC;
   sta.status = SZT_STATUS_ASSOC_DOWN;
   sta.reason = SZT_REASON_RCVD_TERMIND;

   /* Retrieve the upper sap ids from SztSapIdLst and 
    * Send the status indication to all the users
    * that association is up
    */
   if (((sztSapCb = peerCb->sztSapCb) != NULLP) &&
       (sztSapCb->state == LSZ_SAP_ISBOUND))
   {
      /* Send status indication to user */
      SzUiSztStaInd (&sztSapCb->pst, sztSapCb->spId, &sta);
      /* Send an alarm indicating association is UP */
      SZ_SEND_PEER_ALARM(peerCb->peerId, LSZ_EVENT_LI_TERM_IND,
            LSZ_CAUSE_ASSOC_DOWN);
   } /* end of if sctSapCb != NULLP */      
      
   /* Send the runtime update message for PeerCb and AssocCb */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_PEER_CB, peerCb);
   YT_RTUPD_MOD(YT_UPD_ASSOC_CB, peerCb->assocCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);
} /* end of SzLiSctTermInd */



/*
*
*       Fun:   SzLiSctTermCfm
*
*       Desc:  This function is used to process the termination confirmation
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctTermCfm 
(
Pst             *pst,            /* Post structure */
SuId            suId,            /* Service user Id */
UConnId         suAssocId,       /* Service user Assoc Id */
SctStatus       status,          /* Status of termination req */
SctCause        cause            /* Cause if status ! OK */
)
#else /* ANSI */
PUBLIC S16 SzLiSctTermCfm(pst, suId, suAssocId, status, cause)
Pst             *pst;            /* Post structure */
SuId            suId;            /* Service user Id */
UConnId         suAssocId;       /* Service user Assoc Id */
SctStatus       status;          /* Status of termination req */
SctCause        cause;           /* Cause if status ! OK */
#endif /* ANSI */
{
   /* Variable to store the peer control block */
   S16         ret = RFAILED;
   TknU32      peerId;
   SzPeerCb    *peerCb = NULLP;
   SzSctSapCb  *sctSapCb = NULLP;
#ifdef SZTV2
   SztAbortS1  abortS1;
#endif /* SZTV2 */
   
   TRC3(SzLiSctTermCfm);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ040,0,
                "SzLiSctTermCfm() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctTermCfm-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */          

   /* Need not proceed further if the layer has been shut down */ 
   if (szCb.shutdown)
      RETVALUE (ROK);


   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ041, (ErrVal) suId,
                 "SzLiSctTermCfm: Invalid suId \n");
#endif /* ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_TERM_CFM, LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }

   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ((sctSapCb = szCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ042, (ErrVal) 0,
                 "SzLiSctTermCfm: Invalid sap \n");
#endif /* ERRCLS */   
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_TERM_CFM, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }

#ifdef SZTV2
   abortS1.abortS1Cfm.status = status;
   abortS1.abortS1Cfm.reason = SZT_CAUSE_NOT_APPL;
#endif /* SZTV2 */

   /* Retrieve the peerCb.Since suAssocId is same as peerId
    * we can directly retireve the peerCb with index suAssocId 
    * in peerLst
    */
   peerId.pres = TRUE;
   peerId.val  = suAssocId;
   ret = szNdbChkPeer(peerId,&peerCb); 
   if (peerCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_DEBUG, ESZ043, 0,
                 "SzLiSctTermCfm: Failed to retrieve the peer control block \n");
#endif /* if ERRCLASS */
      RETVALUE (RFAILED);
   }
   /* sz008.301: Filling the PeerId */
#ifdef SZTV2
   abortS1.peerId.pres = PRSNT_NODEF;
   abortS1.peerId.val  = peerId.val;
#endif

   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf, 
          "S1AP:SzLiSctTermCfm : Association is terminated \n"));

   if (peerCb->delPcb)
   {
      /* Send an alarm to LM indicating peer is deleted succsessfully */
      SZ_SEND_PEER_ALARM(peerCb->peerId, LSZ_EVENT_LI_TERM_CFM,
            LSZ_CAUSE_PEER_DELETED);

      /* Deallocate the peer */
      (Void)szNdbDeallocPeerCb(peerCb);
   }
   else 
   {
      /* Delete all associated connections */
      szNdbRelConns(peerCb);
#ifdef SZTV2
      if (peerCb->s1Abort == TRUE)
      {
         SzUiSztAbortS1Cfm(&peerCb->sztSapCb->pst, peerCb->sztSapCb->suId, &abortS1);
      }
#endif /* SZTV2 */
   }
          
   RETVALUE (ROK);

} /* end of SzLiSctTermCfm */


/*
*
*       Fun:   SzLiSctDatInd
*
*       Desc:  This function is used to process the data indication
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctDatInd
(
Pst             *pst,         /* Post structure */
SuId            suId,         /* Service user Id */
UConnId         suAssocId,    /* Su Assoc Id */
SctStrmId       strmId,       /* Stream Id */
SctDatIndType   *indType,     /* Indication type */
U32             protId,       /* Protocol Id */
Buffer          *mBuf         /* Buffer recieved */
)
#else /* ANSI */
PUBLIC S16 SzLiSctDatInd (pst, suId, suAssocId, strmId, indType, protId, mBuf)
Pst             *pst;         /* Post structure */ 
SuId            suId;         /* Service user Id */
UConnId         suAssocId;    /* Su Assoc Id */
SctStrmId       strmId;       /* Stream Id */
SctDatIndType   *indType;     /* Indication type */
U32             protId;       /* Protocol Id */
Buffer          *mBuf;        /* Buffer recieved */
#endif /* ANSI */
{
   /* Variable to store the peer control block */
   S16 ret = RFAILED;
   TknU32 peerId;
   SzPeerCb    *peerCb = NULLP;   
   SzSctSapCb  *sctSapCb = NULLP;
   
   TRC3(SzLiSctDatInd);

   SPrntS1ApMsg(mBuf, 0);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ044,0,
                "SzLiSctDatInd() failed, cannot derive szCb");
      SZ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */
   
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctDatInd-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */          

#ifdef SS_DIAG
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03070004, SZ_DIAG_SUASSOCID, suAssocId,
               "Received DatInd from SCTP suAssocId = %ld, stream = %d",
                suAssocId, strmId, 0, 0)
#endif
   /* Check if suId is valid or not */
   if (suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ045, (ErrVal) suId,
                 "SzLiSctDatInd: Invalid suId \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_DAT_IND, LCM_CAUSE_INV_SPID);
      SZ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst )
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ046, (ErrVal) suId,
                 "SzLiSctDatInd: Invalid sctSapLst \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_DAT_IND, LCM_CAUSE_INV_SAP);
      SZ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   sctSapCb = szCb.sctSapLst[suId];
   /* If Sap is not bound then return invalid sap */
   if ((sctSapCb == NULLP) || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ047, (ErrVal) suId,
                 "SzLiSctDatInd: Invalid sap \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_DAT_IND, LCM_CAUSE_INV_SAP);
      SZ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   }

   /* Get the peer control block */
   peerId.pres = TRUE;
   peerId.val  = suAssocId;

   ret = szNdbChkPeer(peerId,&peerCb);
   if (peerCb == NULLP)
   { 
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_DEBUG, ESZ048, 0,
                 "SzLiSctDatInd: Failed to retrieve the peer control block \n");
#endif /* if ERRCLASS */
      SZ_FREE_BUF(mBuf);
      RETVALUE(RFAILED);
   } /* end of if peerCb == NULLP */
   
   /* Call szRcvFromLower which will decode the message and
    * send it to appropriate hander
    */
   szLiRcvFromLower(mBuf, peerCb);   

   /* Send the update message stored */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzLiSctDatInd */


/*
*
*       Fun:   SzLiSctFlcInd
*
*       Desc:  This function is used to process the flow control indication
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctFlcInd
(
Pst         *pst,         /* Post structure */
SuId        suId,         /* Service user Id */
UConnId     suAssocId,    /* Su Assoc Id */
Reason      reason        /* Reason */
)
#else /* ANSI */
PUBLIC S16 SzLiSctFlcInd (pst, suId, suAssocId, reason)
Pst         *pst;         /* Post structure */ 
SuId        suId;         /* Service user Id */
UConnId     suAssocId;    /* suAssocId */
Reason      reason;       /* Reason */
#endif /* ANSI */
{
   S16          ret = RFAILED;
   TknU32       peerId;
   SzPeerCb    *peerCb = NULLP;
   SzSctSapCb  *sctSapCb = NULLP;
   SztFlcInd    flcInd;

   TRC3(SzLiSctFlcInd);
   
#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ049,0,
                "SzLiSctFlcInd() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctFlcInd-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */          


   /* If suId is not valid, return error */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ050, (ErrVal) suId,
                 "SzLiSctFlcInd: Invalid suId \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_FLC_IND, LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }

   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ((sctSapCb = szCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ051, (ErrVal) 0,
                 "SzLiSctFlcInd: Invalid sap \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_FLC_IND, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }

   /* get the peer control block */
   peerId.pres = TRUE;
   peerId.val  = suAssocId;

   ret = szNdbChkPeer(peerId, &peerCb);
   if (peerCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_DEBUG, ESZ052, 0,
                 "SzLiSctFlcInd: Failed to retrieve the peer control block \n");
#endif /* if ERRCLASS */
      RETVALUE(RFAILED);
   }
#ifdef SS_DIAG  
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03070005, SZ_DIAG_PEERID, peerId.val,
               "Received FlcInd suAssocId = %ld, reason  = %d",
                suAssocId, reason, 0, 0)
#endif

   /* Populate the flow control indication structure */
   cmMemset((U8*)&flcInd, 0, sizeof(SztFlcInd));

   flcInd.peerId.pres = TRUE;
   flcInd.peerId.val  = suAssocId;
   switch(reason)
   {
      case SCT_FLC_START:
         flcInd.status = SZT_FLC_START;
         break;

      case SCT_FLC_DROP:
         flcInd.status = SZT_FLC_DROP;
         break;

      case SCT_FLC_STOP:
         flcInd.status = SZT_FLC_STOP;
         break;

      case SCT_FLC_ACTIVE:
         flcInd.status = SZT_FLC_ACTIVE;
         break;

      case SCT_FLC_INACTIVE:
         flcInd.status = SZT_FLC_INACTIVE;
         break;
   
      default:
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR(ERRCLS_DEBUG, ESZ053, 0,
                    "SzLiSctFlcInd: invalid congestion reason \
                    received in flow control indication \n");
#endif /* if ERRCLASS */
         RETVALUE(RFAILED);
   }

   /* Send the flow control indication to user */
   SzUiSztFlcInd(&peerCb->sztSapCb->pst, peerCb->sztSapCb->suId, &flcInd);

   /* Generate alarm to LM */
   SZ_SEND_PEER_ALARM(peerCb->peerId, LSZ_EVENT_LI_FLC_IND, flcInd.status);

   RETVALUE(ROK);
} /* end of SzLiSctFlcInd */


#ifdef SZ_PHASE2
/*
*
*       Fun:   SzLiSctHBeatCfm
*
*       Desc:  This function is used to process the heartbeat confirmation 
*              received from SCTP layer
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 SzLiSctHBeatCfm
(
Pst         *pst,         /* Post structure */
SuId        suId,         /* Service user Id */
UConnId     suAssocId,    /* Su Assoc Id */
CmNetAddr   *dstNAddr,    /* Destination Address */
SctStatus   status,       /* Status of HBeat request */
SctResult   result,       /* Result */
SctCause    cause         /* Cause */
)
#else /* ANSI */
PUBLIC S16 SzLiSctHBeatCfm (pst, suId, suAssocId, dstNAddr, status, result, cause)
Pst         *pst;         /* Post structure */ 
SuId        suId;         /* Service user Id */
UConnId     suAssocId;    /* Su Assoc Id */
CmNetAddr   *dstNAddr;    /* Destination Address */
SctStatus   status;       /* Status of HBeat request */
SctResult   result;       /* Result */
SctCause    cause;        /* Cause */
#endif /* ANSI */
{
   TRC3(SzLiSctHBeatCfm);
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctHBeatCfm-----(proc(%d),ent(%d),inst(%d))-------\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */          
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(suAssocId);
   UNUSED(dstNAddr);
   UNUSED(status);
   UNUSED(result);
   UNUSED(cause);

   RETVALUE(ROK);
} /* end of SzLiSctHBeatCfm */

#endif /* SZ_PHASE2 */          

#ifdef SZ_PHASE2            

/*
 *
 *       Fun:   SzLiSctStaCfm
 *
 *       Desc:  This function is used to process the status confirmation from
 *              the SCTP layer.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  sz_li.c
 *
 */

#ifdef ANSI
PUBLIC S16 SzLiSctStaCfm
(
Pst              *pst,              /* Post structure */
SuId             suId,              /* service userid */
UConnId          suAssocId,         /* service user association id */
CmNetAddr        *dstNAddr,         /* destination network address list */
SctResult        result,            /* result */
SctCause         cause,             /* cause */
SctStaInfo       *staInfo           /* status information */
)
#else /* ANSI */
PUBLIC S16 SzLiSctStaCfm(pst, suId, suAssocId, dstNAddr, result, cause, staInfo)
Pst              *pst;              /* Post structure */
SuId             suId;              /* service userid */
UConnId          suAssocId;         /* service user association id */
CmNetAddr        *dstNAddr;         /* destination network address list */
SctResult        result;            /* result */
SctCause         cause;             /* cause */
SctStaInfo       *staInfo;          /* status information */
#endif /* ANSI */
{
   TRC3(SzLiSctStaCfm);
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctStaCfm-----(proc(%d),ent(%d),inst(%d))----\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */          

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(suAssocId);
   UNUSED(dstNAddr);
   UNUSED(result);
   UNUSED(cause);
   UNUSED(staInfo);

   RETVALUE(ROK);

} /* end of SzLiSctStaCfm */
#endif


/*
 *
 *       Fun:   SzLiSctStaInd
 *
 *       Desc:  This function is used to process the status indication from
 *              the SCTP layer.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  sz_li.c
 *
 */

#ifdef SCT7
#ifdef ANSI
PUBLIC S16 SzLiSctStaInd
(
Pst              *pst,              /* Post structure */
SuId             suId,              /* service userid */
UConnId          suAssocId,         /* service user association id */
UConnId          spAssocId,         /* service provider association id */
CmNetAddr        *dstNAddr,         /* destination network address list */
SctStatus        status,            /* status */
SctCause         cause,             /* cause */
U32              protId,            /* Protocol ID */
Buffer           *mBuf              /* status information */
)
#else /* ANSI */
PUBLIC S16 SzLiSctStaInd(pst, suId, suAssocId, spAssocId, dstNAddr, status, cause, protId, mBuf)
Pst              *pst;              /* Post structure */
SuId             suId;              /* service userid */
UConnId          suAssocId;         /* service user association id */
UConnId          spAssocId;         /* service provider association id */
CmNetAddr        *dstNAddr;         /* destination network address list */
SctStatus        status;            /* status */
SctCause         cause;             /* cause */
U32              protId;            /* Protocol ID */
Buffer           *mBuf;             /* status information */
#endif /* ANSI */
#else /* SCT7 */
#ifdef ANSI
PUBLIC S16 SzLiSctStaInd
(
Pst              *pst,              /* Post structure */
SuId             suId,              /* service userid */
UConnId          suAssocId,         /* service user association id */
UConnId          spAssocId,         /* service provider association id */
CmNetAddr        *dstNAddr,         /* destination network address list */
SctStatus        status,            /* status */
SctCause         cause,             /* cause */
Buffer           *mBuf              /* status information */
)
#else /* ANSI */
PUBLIC S16 SzLiSctStaInd(pst, suId, suAssocId, spAssocId, dstNAddr, status, cause, mBuf)
Pst              *pst;              /* Post structure */
SuId             suId;              /* service userid */
UConnId          suAssocId;         /* service user association id */
UConnId          spAssocId;         /* service provider association id */
CmNetAddr        *dstNAddr;         /* destination network address list */
SctStatus        status;            /* status */
SctCause         cause;             /* cause */
Buffer           *mBuf;             /* status information */
#endif /* ANSI */
#endif /* SCT7 */
{
   S16 ret = RFAILED;
   TknU32 peerId;
   /* Variable to store the peer control block */
   SzPeerCb   *peerCb = NULLP;
   /* Variable to store the upper sap control block */
   SzSztSapCb *sztSapCb = NULLP;
   /* Variable to store the lower sap control block */
   SzSctSapCb *sctSapCb = NULLP;
   /* Variable to store the status indication structure */
   SztStaInd  sta;
   
   TRC3(SzLiSctStaInd);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId,__FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ054,0,
                "SzLiSctStaInd() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* if ERRCLASS */
#endif /* end of SS_MULTIPLE_PROCS */

#ifdef SCT7
   UNUSED(protId);
#endif /* SCT7 */
   
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "--------S1AP:SzLiSctStaInd-----(proc(%d),ent(%d),inst(%d))---\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif   /* end of DEBUGP */                        


   /* Check if suId is valid or not */
   if(suId < 0 || suId > szCb.genCfg.nmbSctSaps)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ055, (ErrVal) suId, 
                 "SzLiSctStaInd: Invalid suId \n");
#endif /* if ERRCLS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SPID);
      RETVALUE(RFAILED);
   }
 
   /* If Sap is not bound then return invalid sap */
   if (!szCb.sctSapLst || ((sctSapCb = szCb.sctSapLst[suId]) == NULLP)
       || sctSapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ056, (ErrVal) 0,
                 "SzLiSctStaInd: Invalid sap \n");
#endif /* if ERRCLASS */
      SZ_SEND_SAPID_ALARM(suId, LSZ_EVENT_LI_STA_IND, LCM_CAUSE_INV_SAP);
      RETVALUE(RFAILED);
   }

   /* Retrieve the peer control block.Since the peerId is
      * same as suAssocId, peer control block will have
      * the same index as suAssocId 
    */
   peerId.pres = TRUE;
   peerId.val = suAssocId;
   ret = szNdbChkPeer(peerId, &peerCb);
   if(peerCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
       SZLOGERROR(ERRCLS_DEBUG, ESZ057, 0,
                  "SzLiSctStaInd: Failed to retrieve the \
                  peer control block \n");
#endif /* if ERRCLASS */
       RETVALUE(RFAILED);
   }

#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */ 
  SZ_DIAG_LVL0(0x03070006, SZ_DIAG_PEERID, peerId.val,
            "Received StaInd from SCTP suAssocId = %d, status = %d, cause = %d",
                suAssocId, status, cause, 0)
#endif
    switch(status)
    {
        case SCT_STATUS_COMM_UP:
        {
           /* Update the assocState in assocCb */
           peerCb->assocCb->assocState = SZ_ASSOC_UP;

           /* Populate the sta indication structure */
           cmMemset((U8*)&sta, 0, sizeof(SztStaInd));

           sta.peerId.pres = TRUE;
           sta.peerId.val = peerCb->peerId;
           sta.type   = SZT_STA_TYPE_ASSOC;
           sta.status = SZT_STATUS_ASSOC_UP;
           sta.reason = SZT_REASON_RCVD_COMM_UP;

          /* Retrieve the upper sap ids from SztSapIdLst and 
            * send the status indication to all the users
            * that association is up
            */
          if (((sztSapCb = peerCb->sztSapCb) != NULLP) && 
              (sztSapCb->state == LSZ_SAP_ISBOUND))
          {
              /* Send status indication to user */
              SzUiSztStaInd (&sztSapCb->pst, sztSapCb->spId, &sta);
              /* Send an alarm indicating association is UP */
              SZ_SEND_PEER_ALARM(peerCb->peerId, LSZ_EVENT_LI_ASSOC_CFM,
                    LSZ_CAUSE_ASSOC_UP);
          } /* sztSap is not NULL */      

          /* Retrieve the S1Setup Request from peerCb if any.
           * and send it to the peer.Since it is a common management
           * procedure, conCb is not valid
           */
           break;
        } /* end of SCT_STATUS_COMM_UP */

        case SCT_STATUS_NET_UP:
           break;

        case SCT_STATUS_NET_DOWN:
           break;

        case SCT_STATUS_SND_FAIL:

           /* Send an alarm indicating sending failed */
           SZ_SEND_PEER_ALARM(peerCb->peerId, LSZ_EVENT_LI_SEND_FAIL, cause);
           break;

        case SCT_STATUS_INV:
           break;

        default:
#if (ERRCLASS & ERRCLS_INT_PAR)
            SZLOGERROR(ERRCLS_INT_PAR, ESZ058, (ErrVal) 0,
                       "SzLiSctStaInd: Invalid status received \
                       in Status Indication \n");
#endif /* if ERRCLASS */
            RETVALUE(RFAILED);

    } /* end of switch */

   /* Send the runtime update message for AssocCb */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_ASSOC_CB, peerCb->assocCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);
} /* end of SzLiSctStaInd */


#ifdef SZ_PHASE2

/*
 *
 *       Fun:   SzLiSctSetPriCfm
 *
 *       Desc:  This function is used to process the priority confirmation from
 *              the SCTP layer.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  sz_li.c
 *
 */

#ifdef ANSI
PUBLIC S16 SzLiSctSetPriCfm
(
Pst              *pst,              /* Post structure */
SuId             suId,              /* service userid */
UConnId          suAssocId,         /* service user association id */
SctResult        result,            /* result */
SctCause         cause              /* cause */
)
#else /* ANSI */
PUBLIC S16 SzLiSctSetPriCfm(pst, suId, suAssocId, result, cause)
Pst              *pst;              /* Post structure */
SuId             suId;              /* service userid */
UConnId          suAssocId;         /* service user association id */
SctResult        result;            /* result */
SctCause         cause;             /* cause */
#endif /* ANSI */
{
   TRC3(SzLiSctSetPriCfm);
#ifdef DEBUGP      
   SZDBGP(DBGMASK_LI,(szCb.init.prntBuf,
          "-----S1AP:SzLiSctSetPrimCfm----(proc(%d),ent(%d),inst(%d))----\n",
          pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */          

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(suAssocId);
   UNUSED(result);
   UNUSED(cause);

   RETVALUE(ROK);
} /* end of SzLiSctSetPriCfm */

#endif /* SZ_PHASE2 */          

/*
*
*       Fun:   szLiSndAssocReq   
*
*       Desc:  This function is used to send AssocReq to SCTP.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiSndAssocReq
(
SzAssocCb  *assocCb                /* Association control block */
)
#else /* ANSI */
PUBLIC S16 szLiSndAssocReq(assocCb)
SzAssocCb  *assocCb;               /* Association control block */
#endif /* ANSI */
{
   S16        ret = ROK;           /* return value */
   SzPeerCb   *peerCb = NULLP;     /* Peer control block */
   SzSctSapCb *sctSapCb = NULLP;   /* SCT SAP control block */
#ifdef SCT3
   SctTos     tos = 0;             /* Type of service */
#endif /* SCT3 */


   TRC2(szLiSndAssocReq)

   /* Check the assoc state to see if its already up */
   if (assocCb->assocState == SZ_ASSOC_UP)
   {
      SZDBGP(DBGMASK_LI, (szCb.init.prntBuf,
             "szLiSndAssocReq: Association is already up and running \n"));
      RETVALUE(RFAILED);
   }

   /* Update the Assoc state */
   assocCb->assocState = SZ_ASSOC_IN_PRG;

   peerCb = assocCb->peer;
   sctSapCb = peerCb->sctSapCb;
   assocCb->suAssocId = peerCb->peerId;


    /* Call SCT interface method to establish an association */ 
#ifdef SCT3
   SzLiSctAssocReq(&(sctSapCb->pst), sctSapCb->spId,
                   sctSapCb->endpCb.locSpEndpId,
                   assocCb->suAssocId, &(assocCb->assocCfg->priDstAddr), 
                   assocCb->assocCfg->dstPort, assocCb->assocCfg->locOutStrms,
                   &(assocCb->assocCfg->dstAddrLst),
                   &(sctSapCb->endpCb.srcAddrLst), 
                   tos, (Buffer *)NULLP);
#else /* SCT3 */
   SzLiSctAssocReq(&sctSapCb->pst, sctSapCb->spId,
         sctSapCb->endpCb.locSpEndpId,
                   assocCb->suAssocId, &(assocCb->assocCfg->priDstAddr), 
                   assocCb->assocCfg->dstPort, assocCb->assocCfg->locOutStrms,
                   &(assocCb->assocCfg->dstAddrLst),
                   &(sctSapCb->endpCb.srcAddrLst), 
                   (Buffer *)NULLP);
#endif /* SCT3 */
   
   RETVALUE(ret);

} /* szLiSndAssocReq */

/*
*
*       Fun:   szLiTermAssoc 
*
*       Desc:  This function is used to terminate an association with SCTP.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiTermAssoc 
(
SzPeerCb  **peerCb,     /* Association control block */
Bool      assocAbort
)
#else /* ANSI */
PUBLIC S16 szLiTermAssoc(peerCb,assocAbort)
SzPeerCb   **peerCb;    /* Association control block */
Bool       assocAbort;
#endif /* ANSI */
{
   SzAssocCb  *assocCb;    /* Association control block */
   S16        ret = ROK;
   U16         assocState;
   TRC2(szLiTermAssoc)

   assocCb = (*peerCb)->assocCb;
   if (assocCb->assocState == SZ_ASSOC_DOWN)
   {
     SZDBGP(DBGMASK_LI, (szCb.init.prntBuf,
             "szLiTermAssoc: Association is already set to down \n"));
         /* 
       * Delete the peer CB if flag is set to be deleted
       */
      if ((*peerCb)->delPcb)
      {
         /* Deallocate the peer */
         (Void)szNdbDeallocPeerCb(*peerCb);
         *peerCb = NULL;
      }
      RETVALUE(ROK);
   }

   assocState = assocCb->assocState;

   /* Updated the state of peer to IDLE instated of NORMAL as the
    * association is down */
   /* Update the Assoc state to DOWN */
   assocCb->assocState = SZ_ASSOC_DOWN;
   (*peerCb)->state = SZT_ND_ST_IDLE;

   /* Send the runtime update message for PeerCb and AssocCb */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_ASSOC_CB, assocCb);
   YT_RTUPD_MOD(YT_UPD_PEER_CB, *peerCb);
#endif /* YT */

   /* sz008.301 : Filling SpID insted of SuId 
    * send termenation request */
   if(assocState == SZ_ASSOC_IN_PRG)
   {
      /* Send termination request */
      ret = SzLiSctTermReq(&(assocCb->peer->sctSapCb->pst), 
            assocCb->peer->sctSapCb->spId,  
            assocCb->suAssocId, SCT_ASSOCID_SU, assocAbort);
   }
   else
   {
      /* Send termination request */
      ret = SzLiSctTermReq(&(assocCb->peer->sctSapCb->pst), 
                  assocCb->peer->sctSapCb->spId,  
                  assocCb->spAssocId, SCT_ASSOCID_SP, assocAbort);
   }

   if(ret != ROK)
   {
      RETVALUE(ret);
   }
   
   /* In case Association is aborted delete peer CB here */
   if ((*peerCb)->delPcb  && assocAbort)
   {
      /* Deallocate the peer */
      (Void) szNdbDeallocPeerCb(*peerCb);
      *peerCb = NULL;
   }
   /* sz008.301 : Moving the state to ROKPENDING insted of ROK */
   RETVALUE(ROKPENDING);

} /* szLiTermAssoc*/


/*
 *
 *       Fun:   szLiErrHndlr - Peer error handling
 *
 *       Desc:  This function handles Error scenarios invoked upon receiving the 
 *              messages from the peer. 
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_msgfn.c
 *
 */
#ifdef ANSI
PUBLIC S16 szLiErrHndlr 
(
S1apPdu    *pdu,                     /* S1AP PDU */
U8         group,                    /* Cause group */
U16        cause,                    /* Error cause */
S1apPdu    **rspPdu,                  /* Unsuccessful or Error response */
Mem        mem                       /* Memory for rspPdu */
)
#else /* ANSI */
PUBLIC S16 szLiErrHndlr(pdu, group, cause, rspPdu, mem)
S1apPdu    *pdu;                     /* S1AP PDU */
U8         group;                    /* Cause group */
U16        cause;                    /* Error cause */
S1apPdu    **rspPdu;                  /* Unsuccessful or Error response */
Mem        mem;                      /* Memory for rspPdu */
#endif /* ANSI */
{
   S16 ret = ROK;
   U8  msgIdx;
   U8  response;
   CmPAsnErr asnErr;

   TRC2(szLiErrHndlr);

   /* Get the message index */
   ret = szUtilsGetMsgIdx(&msgIdx, pdu);

   /* Get the response message for the index received */ 
   response = rspMsg[msgIdx];
#ifdef CM_PASN_CRIT_HANDL
   asnErr.ieCount = 0;
#endif /* CM_PASN_CRIT_HANDL */

   /* Build error message with the corresponding response */
   switch (response)
   {
      case SZT_MSG_ID_INIT_CXT_SETUP_FAIL:
      {
         ret = szDbFillInitCxtSetupFail(pdu, group, cause, rspPdu, mem,
               &asnErr);
         break;
      }
      case SZT_MSG_ID_UE_CXT_MOD_FAIL:
      {
         ret = szDbFillUeCxtModFail(pdu, group, cause, rspPdu, mem, &asnErr);
         break;
      }
      case SZT_MSG_ID_NO_MSG:
      {
         ret = szDbFillErrInd(pdu, group, cause, rspPdu, mem, &asnErr); 
         break;
      }
      default:
      {
         ret = RFAILED;
         break;
      }
   } 

   RETVALUE(ret);
      
} /* szLiErrHndlr */

#ifdef SZ_MME
/*
*
*       Fun:   szLiDynamPeerConfig
*
*       Desc:  This function is used to do the dynamic configuration
*              of the peer.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PUBLIC S16 szLiDynamPeerConfig
(
SuId              suId,              /* Service user Id */
SctAssocIndParams *assocParams       /* Assoc params */
)
#else /* ANSI */
PUBLIC S16 szLiDynamPeerConfig (suId, assocParams)
SuId              suId;              /* Service user Id */
SctAssocIndParams *assocParams;      /* Assoc params */
#endif /* ANSI */
{
   SzPeerCb            *peerCb = NULLP;
   SztPeerAssocInd      peerAssocInd;
   S16                  ret = ROK;

   TRC3(szLiDynamPeerConfig);

   if( szCb.genSta.nmbPeer >= szCb.genCfg.nmbPeer )
   {
#if (ERRCLASS & ERRCLS_ADD_RES)
      SZLOGERROR(ERRCLS_ADD_RES, ESZ059, (ErrVal) 0,
            "szLiDynamPeerConfig: Maximum value reached for number of peer");
#endif
      RETVALUE(RFAILED);
   }

   /* Allocate peer control block and assoc control block */
   if ((ret = szNdbAllocDynamPeerCb(suId, &peerCb)) != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ060, (ErrVal) 0,
       "szLiDynamPeerConfig: Failed to allocate control block for dyn peer\n");
#endif /* if ERRCLASS */

      RETVALUE(RFAILED);
   }

   /* Fill peer and association control blocks */
   if (assocParams->type == SCT_ASSOC_IND_INIT)
   {
      cmMemcpy((U8 *)&peerCb->assocCb->assocCfg->dstAddrLst, 
       (U8 *)&assocParams->t.initParams.peerAddrLst, sizeof(SctNetAddrLst));
      peerCb->assocCb->assocCfg->dstPort = assocParams->t.initParams.peerPort;

      peerCb->assocCb->assocCfg->locOutStrms = 
         (assocParams->t.initParams.inStrms < assocParams->t.initParams.outStrms)?
              assocParams->t.initParams.inStrms : assocParams->t.initParams.outStrms;
   }
   else
   {
      cmMemcpy((U8 *)&peerCb->assocCb->assocCfg->dstAddrLst, 
       (U8 *)&assocParams->t.cookieParams.peerAddrLst, sizeof(SctNetAddrLst));
      peerCb->assocCb->assocCfg->dstPort = assocParams->t.cookieParams.peerPort;

#ifdef SCT6
      peerCb->assocCb->assocCfg->locOutStrms = 
         (assocParams->t.cookieParams.inStrms < assocParams->t.cookieParams.outStrms)?
              assocParams->t.cookieParams.inStrms : assocParams->t.cookieParams.outStrms;
#endif /* SCT6 */

      peerCb->assocCb->spAssocId   = assocParams->t.cookieParams.spAssocId;
      peerCb->assocCb->assocState  = SZ_ASSOC_IN_PRG;
   }

   cmMemcpy((U8 *)peerCb->assocParams,(U8 *)assocParams,
                                  sizeof (SctAssocIndParams));
   ret = szNdbAddPeerAddrPortCb(&peerCb->assocCb->assocCfg->dstAddrLst,
                           peerCb->assocCb->assocCfg->dstPort,
                           peerCb);
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ061, (ErrVal) 0,
            "szLiDynamPeerConfig: Failed to insert in the \
            dynamic peerAssoc hash list\n");
#endif /* if ERRCLASS */

      szNdbRemPeerAddrPortCb(peerCb);
      szNdbDeallocDynamPeerCb(peerCb);

      RETVALUE(RFAILED);
   }

   /* fill peerAssocInd structure */
   peerAssocInd.dstPort = peerCb->assocCb->assocCfg->dstPort;
   cmMemcpy((U8 *)&peerAssocInd.dstAddrLst,
       (U8 *)&peerCb->assocCb->assocCfg->dstAddrLst, sizeof(SctNetAddrLst));

   /* Send dynamic peer indication to the user */
   szUiSndPeerAssocInd(peerCb, &peerAssocInd);

   /* Add the mapping for the peerCb as it is required to send in CS */
#ifdef YT
   YT_ADD_MAPPING(YT_UPD_PEER_CB, peerCb);
   YT_RTUPD_ADD(YT_UPD_DYNPEER_CB, peerCb);
#endif /* YT */

   RETVALUE(ROK);
}
#endif /* SZ_MME */

/*
*
*       Fun:   szLiHdlDecFail
*
*       Desc:  This function is used to handle decode message 
*              failure scenario
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: N/A
*
*       File:  sz_li.c
*
*/

#ifdef ANSI
PRIVATE S16 szLiHdlDecFail
(
CmPAsnErr asnErr, 
S1apPdu   *pdu,
SzPeerCb  *peerCb      /* Peer control block */
)
#else /* ANSI */
PRIVATE S16 szLiHdlDecFail (asnErr, pdu, peerCb)
CmPAsnErr asnErr;
S1apPdu   *pdu;
SzPeerCb  *peerCb;      /* Peer control block */
#endif /* ANSI */
{
   Mem mem;
   S16 retVal = ROK; /* Return value */
   U32 cause = 0;
   S1apPdu  *errPdu = NULLP;  /* Error Indication PDU */
   U8 msgType = 0;
   U32 procCode = 0;
   SzConCb  *conCb = NULLP;

   TRC2(szLiHdlDecFail)

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   /* Out of range is also needs to be treated as 
    * abstract syntax error */
   /* Escape function will fails, only in case of abstract syntax error */
   if((asnErr.errCode != CM_PASN_ESC_FUNC_FLD) &&
      (asnErr.errCode != CM_PASN_OUT_RANGE))
   {
      /* Build an error indication procedure message 
       * and send to the peer */
      cause = SztCauseProttransfer_syntax_errorEnum;

#ifdef CM_PASN_CRIT_HANDL
      asnErr.ieCount = 0;
#endif /* CM_PASN_CRIT_HANDL */

      szDbFillErrInd(NULLP, CAUSE_PROTOCOL, cause, &errPdu, mem, &asnErr);

#ifdef LSZV1
      /* Increment the error statistics */
      szIncErrSts(peerCb, CAUSE_PROTOCOL, cause);
#endif /* LSZV1 */

      /* Send decoded data to its User */
      retVal = szLiSndToLower(peerCb, NULLP, errPdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if (retVal != ROK)
      {
         SZLOGERROR(ERRCLS_INT_PAR, ESZ062, (ErrVal) peerCb->peerId,
               "szLiHdlDecFail: Lower Interface send failed");
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }
   else
   {
      msgType  = pdu->pdu.choice.val;
      /* If an initiating message then build an 
       * error indication procedure message and send to the peer */
      if(msgType == S1AP_PDU_INITIATINGMSG)
      {
         /* Build an response procedure message 
          * and send to the peer */

         /* Abstract syntax error cause set in user escape function */
#ifdef CM_PASN_CRIT_HANDL
         cause = asnErr.errCause;
#else /* CM_PASN_CRIT_HANDL */
         cause = SztCauseProtabstract_syntax_error_falsely_constructed_messageEnum;
#endif /* CM_PASN_CRIT_HANDL */

         szUtilsBldUnsucOutComeMsg(pdu, CAUSE_PROTOCOL, cause, &errPdu, mem,
               &asnErr);
#ifdef LSZV1
         /* Increment the error statistics */
         szIncErrSts(peerCb, CAUSE_PROTOCOL, cause);
#endif /* LSZV1 */

         /* Send decoded data to its User */
         retVal = szLiSndToLower(peerCb, NULLP, errPdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
         if (retVal != ROK)
         {
            SZLOGERROR(ERRCLS_INT_PAR, ESZ063, (ErrVal) peerCb->peerId,
                  "szLiHdlDecFail: Lower Interface send failed");
         }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      }
      else
      {
         /* If response message then assume inform the service user */
         switch(msgType)
         {
            case S1AP_PDU_SUCCESSFULOUTCOME:
               procCode = pdu->pdu.val.successfulOutcome.procedureCode.val;

               break;

            case S1AP_PDU_UNSUCCESSFULOUTCOME:
               procCode = pdu->pdu.val.unsuccessfulOutcome.procedureCode.val;
               break;

            default:
               break;
         }

         /* Get ConCb details */
         szUtilsGetConCb(peerCb, pdu, &conCb);
         if(conCb)
         {
            /* Send an local error indication with cause value set to 
             * the procedure code */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
                  peerCb->sztSapCb, SZT_CAUSE_RX_ERRONEOUS_RSP, procCode);
         }
      } /* if - type of message */
      /* Free the pdu here since its not done in the szDecode function */
      SZ_FREE_SDU(pdu);
   } /* if - type of error transfer/syntax */

#ifdef SS_DIAG
   /* sz008.301 : Change for Diagnostics */
   SZ_DIAG_LVL0(0x03070007, SZ_DIAG_PEERID, peerCb->peerId,
               "ASN Decoding failed, AsnErrorCode = %d, msgType = %d, procedureCode = %d ", asnErr.errCode, msgType, procCode, 0)
#endif
   /* Update the decode error sts */
   szCb.genSts.nmbDecErr++;
   RETVALUE(ROK); 
}

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */

/**********************************************************************

         End of file:     sq_li.c@@/main/nodal_main/akaranth_feat_lpdev_branch/1 - Tue Sep 29 21:39:58 2015

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat     init                  description
------------ -------- ------ ----------------------------------------------
/main/5      ---       va      1. Updated for S1AP release 4.1
*********************************************************************91*/
