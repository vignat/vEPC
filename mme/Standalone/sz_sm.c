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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for SM

     File:     sz_sm.c

     Sid:      sq_sm.c@@/main/nodal_main/3 - Fri Nov 22 17:55:49 2013

     Prg:      sn

*********************************************************************21*/

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
#include "cm_pasn.h"
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
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"
#ifdef YT
#include "lyt.h"           /* S1AP-PSF LM Interface */
#include "yt.h"            /* S1AP-PSF Layer */
#endif /* YT */
#include "sz.h"            /* S1AP Layer */
#include "sz_err.h"
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
#include "cm_pasn.x"
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_pftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif /* SZ_FTHA */

#include "szt_asn.x"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "sct.x"           /* SCTP Layer */
#include "szt.x"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP LM Interface */
#ifdef YT
#include "lyt.x"           /* S1AP-PSF LM Interface */
#include "yt.x"
#endif /* YT */
#include "sz.x"            /* S1AP Layer */

/*
*
*       Fun:  szSmSndConnStMsg
*
*       Desc:  Called to send messages in connecting state
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smuimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmSndConnStMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmSndConnStMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   S16 ret = ROK;

#ifdef SZ_MME
   TknU8 *mmeS1apId = NULLP;
#endif /*SZ_MME */
   U8    evnt;

   TRC2(szSmSndConnStMsg)

   szUtilsGetMsgIdx(&evnt,pdu);


#ifdef SZ_MME    
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      switch(evnt)
      {
         case SZT_MSG_ID_CXT_SETUP_REQ:
         case SZT_MSG_ID_PATH_SWTCH_REQ_ACK:
              szGetIE(&pdu->pdu, Sztid_MME_UE_S1AP_ID, &mmeS1apId);
              ((SztMME_UE_S1AP_ID *)mmeS1apId)->val = conCb->lclConRefNo; 
         break;
         default:
         break;
      }
   }
#endif /*SZ_MME */

   /* Send this message send/receive interface */
   ret = szLiSndToLower(peerCb,conCb, pdu);
   if(ret != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, ret, 0);
      }
      else
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);
      }

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ146, (ErrVal) conCb->lclConRefNo,
            "szSmSndConnStMsg: Lower Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(ret);
   }

   /* Change the state to Connected */
   conCb->state = SZ_SMSZ_ESTABLISHED;

   /* Send the runtime update message to peer for state updation */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_CONN_CB, conCb);
#endif /* YT */

   RETVALUE(ROK);
} /* end of szSmSndConnStMsg */

/*
*
*       Fun:  szSmSndEstStMsg
*
*       Desc:  Called to send messages in established state
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smuimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmSndEstStMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmSndEstStMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
#ifdef SZ_MME
   TknU8  *eNbId = NULLP;
#endif /*SZ_MME */
   S16    ret = RFAILED;

   TRC2(szSmSndEstStMsg)


#ifdef SZ_MME
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      szGetIE(&(pdu->pdu), Sztid_eNB_UE_S1AP_ID, &eNbId);
      if((eNbId != NULLP) && ((SztENB_UE_S1AP_ID *)eNbId)->val
            != conCb->rmtConRefNo)
      {
         /* Error Indication to the user */
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.spConnId, 
                             peerCb->sztSapCb, SZT_CAUSE_INV_CONID,
                             ((SztENB_UE_S1AP_ID *)eNbId)->val);
         SZ_FREE_SDU(pdu);
         RETVALUE(ret);
      }
   }
#endif /*SZ_MME */

   /* Send this message send/receive interface */
   ret = szLiSndToLower(peerCb,conCb, pdu);
   if(ret != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, ret, 0);
      }
      else
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);
      }

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ147, (ErrVal) conCb->lclConRefNo,
            "szSmSndEstStMsg: Lower Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   RETVALUE(ret);
} /* end of szSmSndEstStMsg */

/*
*
*       Fun:  szSmLclRlsReq
*
*       Desc:  Called to release logical S1 connection
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smuimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmLclRlsReq
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmLclRlsReq(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{  
   S16 ret = RFAILED;

   TRC2(szSmLclRlsReq)

   /* Deallocate connection control block */
   ret = szNdbDeallocConCb(conCb->lclConRefNo, peerCb, SZ_CONN_REF_LCL);

   if(ret == RFAILED)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
                          peerCb->sztSapCb, SZT_CAUSE_INV_CONID,
                          conCb->lclConRefNo);
   
      SZ_FREE_SDU(pdu);

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ148, (ErrVal) conCb->lclConRefNo,
           "szSmLclRlsReq: Couldn't find the connection CB");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   SZ_FREE_SDU(pdu);

   RETVALUE(ret);
} /* end of szSmLclRlsReq */

/*
*
*       Fun:   szSmLclErrMsg  
*
*       Desc:  Called to send error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smuimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmLclErrMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmLclErrMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   U8       evnt;

   TRC2(szSmLclErrMsg)

   peerCb->peerSts.cfm.nmbUnexpEvt++;

   szUtilsGetMsgIdx(&evnt,pdu);

   SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
                       peerCb->sztSapCb, SZT_CAUSE_UNEXP_MSG, evnt);

   SZ_FREE_SDU(pdu);

   RETVALUE(ROK);
} /* end of szSmLclErrMsg */

/*
*
*       Fun:   szSmLclErrState
*
*       Desc:  Called to send error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smuimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmLclErrState
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmLclErrState(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{

   TRC2(szSmLclErrState)

   peerCb->peerSts.cfm.nmbUnexpEvt++;

   SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
                       peerCb->sztSapCb, SZT_CAUSE_INV_STATE, conCb->state);
   SZ_FREE_SDU(pdu);

   RETVALUE(ROK);
} /* end of szSmLclErrState */

/*
*
*       Fun:   szSmSndIdleStMsg
*
*       Desc:  Called to send Idle state message
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_sm.c
*
*/
#ifdef ANSI
PUBLIC S16 szSmSndIdleStMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmSndIdleStMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{ 
   S16 ret = RFAILED;

#ifdef SZ_MME
   TknU8 *mmeS1apId = NULLP;
#endif /*SZ_MME */
   U8    evnt;

   TRC2(szSmSndIdleStMsg)


      conCb->cd.spConnId = conCb->lclConRefNo;

   szUtilsGetMsgIdx(&evnt,pdu);


#ifdef SZ_MME    
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      switch(evnt)
      {
         case SZT_MSG_ID_DL_NAS_TPT:
         case SZT_MSG_ID_HO_REQ:
            szGetIE(&pdu->pdu, Sztid_MME_UE_S1AP_ID, &mmeS1apId);
            ((SztMME_UE_S1AP_ID *)mmeS1apId)->val = conCb->lclConRefNo; 
            break;
         default:
            break;
      }
   }
#endif /*SZ_MME */

   /* Change the state to Connection in progress */
   conCb->state = SZ_SMSZ_CONNECTING;

   ret = szLiSndToLower(peerCb,conCb, pdu);
   if(ret != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, ret, 0);
      }
      else
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);
      }

      /* Deleate the connection control block */
      szNdbDeallocConCb(conCb->lclConRefNo,peerCb, SZ_CONN_REF_LCL);

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ149, (ErrVal) conCb->lclConRefNo,
            "szSmSndDlNasTpt: Lower Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   RETVALUE(ret);
} /* end of szSmSndIdleStMsg */




#ifdef SZ_MME
/*
*
*       Fun:   szSmSndConnStPathSwReqFail
*
*       Desc:  Called to send Path Switch Reqeust Failure in connecting state
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smuienb.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmSndConnStPathSwReqFail
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmSndConnStPathSwReqFail(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   S16 ret = RFAILED;

   TRC2(szSmSndConnStPathSwReqFail)

   /* Send decoded data to peer */
   ret = szLiSndToLower(peerCb, conCb, pdu);
   if(ret != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, ret, 0);
      }
      else
      {
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
               peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);
      }

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ152, (ErrVal) conCb->lclConRefNo,
            "szSmSndConnStPathSwReqFail: Lower Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   /* Change the state to idle */
   conCb->state = SZ_SMSZ_IDLE;

   /* Deallocate connection control block */
   ret = szNdbDeallocConCb(conCb->lclConRefNo, peerCb, SZ_CONN_REF_LCL);
#if (ERRCLASS & ERRCLS_DEBUG)
   if(ret == RFAILED)
   {
      SZLOGERROR(ERRCLS_DEBUG, ESZ153, (ErrVal) conCb->lclConRefNo,
            "szSmSndConnStPathSwReqFail: Couldn't retreive connection CB");
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
   RETVALUE(ret);
} /* end of szSmSndConnStPathSwReqFail */
#endif /*SZ_MME */


/*
*
*       Fun:  szSmRcvIdleStMsg
*
*       Desc:  Called to receive Idle state message
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_sm.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmRcvIdleStMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmRcvIdleStMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   SztConInd conInd;
   S16 ret = RFAILED;

#ifdef SZ_MME
   TknU8 *eNbId = NULLP;
   SzConCb  *tmpConCb;
#endif /*SZ_MME */
   U8    evnt;
   Bool  insToRmtLst = TRUE;

   TRC2(szSmRcvIdleStMsg)

   ret = szNdbAllocConCb(&conCb, peerCb);

   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   SZ_ZERO(&conInd, sizeof(SztConInd));

   /* This function will always return ROK */
   szNdbGetStream(conCb->cd.peerCb, &conCb->cd.streamNo);

   szUtilsGetMsgIdx(&evnt,pdu);
   conCb->conType   = SZ_INCMG_CONN;


#ifdef SZ_MME    
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      switch(evnt)
      {
         case SZT_MSG_ID_INIT_UE_MSG:
         case SZT_MSG_ID_PATH_SWTCH_REQ:
         {
              szGetIE(&pdu->pdu, Sztid_eNB_UE_S1AP_ID, &eNbId);
              conCb->rmtConRefNo = (((SztENB_UE_S1AP_ID *)eNbId)->val); 
              /* Check fot the presence of ConCb in the remote hash list */
              ret = cmHashListFind(&peerCb->conRefLstRem, (U8 *)&conCb->rmtConRefNo, 
                    sizeof (SzConRefNo), 0, (PTR *)&tmpConCb);
              /* If the ConCb is already present in the hash list deallocate the old
                  conCb and set the flag insToRmtLst as FALSE */
              if (ret == ROK)
              {
                 szNdbDeallocConCb(conCb->lclConRefNo, conCb->cd.peerCb, SZ_CONN_REF_LCL);
                 conCb = tmpConCb;
                 insToRmtLst = FALSE;
              }
         }
         break;
         default:
         break;
      }
   }
#endif /*SZ_MME */

   conCb->cd.spConnId = conCb->lclConRefNo;

   /* Change the state to Connection in progress */
   conCb->state = SZ_SMSZ_CONNECTING;

   conInd.peerId.pres = TRUE;
   conInd.peerId.val = conCb->cd.peerCb->peerId;
   conInd.u.spConnId = conCb->cd.spConnId;
   conInd.pdu = pdu;

   /* If the flag insToRmtLst is set to true then insert the conCb to the hash list
        if not just send a connection indication with same spConnId */
   if (insToRmtLst == TRUE)
   {
      /* Add connection CB into peerCb remote connection hash list */
      ret = cmHashListInsert (&peerCb->conRefLstRem, (PTR)conCb,
            (U8 *)&conCb->rmtConRefNo, sizeof (SzConRefNo));

      /* Handle the failure procedure in case not able to insert into
       * hash list */
      if(ret == RFAILED)
      {
         /* Deallocate the connection control block */
         szNdbDeallocConCb(conCb->lclConRefNo,conCb->cd.peerCb, SZ_CONN_REF_LCL);
         SZ_FREE_SDU(pdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR(ERRCLS_INT_PAR, ESZ154, (ErrVal) conCb->lclConRefNo,
               "szSmRcvInitUeMsg: Hashlist insertion failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         RETVALUE(ret);
      }
   }
   /* Send decoded data to its User */
   ret = szUiSndConInd(conCb, &conInd);

   if(ret == RFAILED)
   {
      /* Deallocate the connection control block */
      szNdbDeallocConCb(conCb->lclConRefNo,conCb->cd.peerCb, SZ_CONN_REF_LCL);
      SZ_FREE_SDU(pdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ155, (ErrVal) conCb->lclConRefNo,
                 "szSmRcvInitUeMsg: Upper Interface send failed");
#endif
   }

   RETVALUE(ret);
} /* end of szSmRcvIdleStMsg */

/*
*
*       Fun:  szSmRcvConnStMsg
*
*       Desc:  Called to handle receive of connecting state messages
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlienb.c
*
*/
#ifdef ANSI
PUBLIC S16 szSmRcvConnStMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else   /* ANSI */
PUBLIC S16 szSmRcvConnStMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif  /* ANSI */
{  
   S16 ret = RFAILED;

   SztConCfm conCfm;

#ifdef SZ_MME
   TknU8 *eNbId = NULLP;
#endif /*SZ_MME */
   U8    evnt;

   TRC2(szSmRcvConnStMsg)

   SZ_ZERO(&conCfm, sizeof(SztConCfm));

   szUtilsGetMsgIdx(&evnt,pdu);


#ifdef SZ_MME    
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      switch(evnt)
      {
         case SZT_MSG_ID_UL_NAS_TPT:
         case SZT_MSG_ID_HO_REQ_ACK:
              szGetIE(&pdu->pdu, Sztid_eNB_UE_S1AP_ID, &eNbId);
              conCb->rmtConRefNo = (((SztENB_UE_S1AP_ID *)eNbId)->val); 
         break;
         default:
            break;
      }
   }
#endif /*SZ_MME */

   conCb->cd.spConnId = conCb->lclConRefNo;

   conCfm.suConnId = conCb->cd.suConnId;
   conCfm.spConnId = conCb->cd.spConnId;
   conCfm.pdu = pdu;

   /* Change the state to connected */
   conCb->state = SZ_SMSZ_ESTABLISHED;

   /* Send the runtime update message to peer for state updation */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_CONN_CB, conCb);
#endif /* YT */

   /* Add connection CB in peerCb remote connection hash list */
   ret = cmHashListInsert (&peerCb->conRefLstRem, (PTR)conCb,
         (U8 *)&conCb->rmtConRefNo, sizeof (SzConRefNo));

   /* Handle the failure procedure in case not able to insert into
    * hash list */

	/* sz008.301: Fix to drop msg with duplicate s1ap id */
   if((ret == RFAILED) || (ROKDUP == ret))
   {
      /* Deallocate the connection control block */
      szNdbDeallocConCb(conCb->lclConRefNo, conCb->cd.peerCb, SZ_CONN_REF_LCL);
      SZ_FREE_SDU(pdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ156, (ErrVal) conCb->lclConRefNo,
                 "szSmRcvConnStMsg: Hashlist insertion failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(ret);
   }

   /* Send decoded data to its User */
   ret = szUiSndConCfm(conCb, &conCfm);

   RETVALUE(ret);
} /* end of szSmRcvConnStMsg */

/*
*
*       Fun:  szSmRcvEstStMsg
*
*       Desc:  Called to receive messages in established state
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmRcvEstStMsg
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else  /* ANSI */
PUBLIC S16 szSmRcvEstStMsg(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif  /* ANSI */
{
   S16            ret = RFAILED;
   SztDatEvntInd  datEvnt;
   S1apPdu        *errPdu = NULLP;     /* Response Error S1AP PDU */
   Mem            mem;                 /* Memory block */
   U16            szErrCause = 0;
#ifdef SZ_MME
   TknU8          *eNbS1apId = NULLP;
#endif /*SZ_MME */
   U8             evnt;

   TRC2(szSmRcvEstStMsg)

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;


#ifdef SZ_MME    
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      szErrCause = SztCauseRadioNwunknown_enb_ue_s1ap_idEnum;

      /* Get eNBId from S1AP Msg, if it exsits */
      szGetIE(&(pdu->pdu), Sztid_eNB_UE_S1AP_ID, &eNbS1apId);

      /* removed NULLP check for eNB_UE_S1AP_ID as it is a
         mandatory parameter. Hence condition never becomes TRUE. */
      /* removed reading of Sztid_UE_S1AP_IDs as this comes only in 
         UE Context Realease Command (mandatory param Dir:MME->eNb)
         this function is never called for UE CTX Release Commnd */
      
      if ((eNbS1apId == NULLP) || (((SztENB_UE_S1AP_ID *)eNbS1apId)->val
               != conCb->rmtConRefNo))
      {
         szUtilsGetMsgIdx(&evnt, pdu);

         /* Donot trigger the error indication to peer if the message is
          * error indication */
         if(evnt != SZT_MSG_ID_ERR_IND)
         {
            /* The validation of Id failed, send Err Ind to Peer! */
            /* Call function to generate errPdu */
            szLiErrHndlr(pdu, CAUSE_RADIONW, szErrCause, &errPdu, mem);

#ifdef LSZV1
            /* Increment the error statistics */
            szIncErrSts(peerCb, CAUSE_RADIONW, szErrCause);
#endif /* LSZV1 */

            /* Send decoded data to its User */
            ret = szLiSndToLower(peerCb, conCb, errPdu);
            if(ret != ROK)
            {
               SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
                     peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);
            }

            SZ_FREE_SDU(pdu);

            RETVALUE(RFAILED);
         }
         /* Trigger local error indication */
         else
         {
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
                  peerCb->sztSapCb, SZT_CAUSE_INV_CONID, 0);
         }
      }
   }
#endif /*SZ_MME */
   SZ_ZERO(&datEvnt, sizeof(SztDatEvntInd));

   datEvnt.suConnId = conCb->cd.suConnId;
   datEvnt.pdu = pdu;

   /* Send decoded data to its User */
   ret = szUiSndDatInd(conCb, &datEvnt);

   if(ret == RFAILED)
   {
      SZ_FREE_SDU(pdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ157, (ErrVal) conCb->lclConRefNo,
            "szSmRcvEstStMsg: Upper Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   RETVALUE(ret);
} /* end of szSmRcvEstStMsg */

/*
*
*       Fun:   szSmIdleStPeerErr 
*
*       Desc:  Called to send error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmIdleStPeerErr
(
SzConCb  *conCb,                      /* Connection control block */
SzPeerCb *peerCb,
S1apPdu  *pdu                         /* S1AP PDU */
)
#else  /* ANSI */
PUBLIC S16 szSmIdleStPeerErr(conCb, peerCb, pdu)
SzConCb  *conCb;                      /* Connection control block */
SzPeerCb *peerCb;
S1apPdu  *pdu;                        /* S1AP PDU */
#endif  /* ANSI */
{
   SztUE_S1AP_IDs  *ueS1apId = NULLP;
#ifdef SZ_MME
   TknU8           *mmeS1apId = NULLP;
#endif /*SZ_MME */
   S16             ret = RFAILED;
   S1apPdu         *errPdu = NULLP;     /* Response Error S1AP PDU */
   Mem             mem;                 /* Memory block */
   U16             szErrCause = 0;        /* Error cause */

   TRC2(szSmIdleStPeerErr)

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;


#ifdef SZ_MME
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      szErrCause = SztCauseRadioNwunknown_mme_ue_s1ap_idEnum;

      /* Get MME Id from S1AP Msg, if it exsits */
      szGetIE(&(pdu->pdu), Sztid_MME_UE_S1AP_ID, &mmeS1apId);

      if (mmeS1apId == NULLP)
      {
         szGetIE(&(pdu->pdu), Sztid_UE_S1AP_IDs, (TknU8 **)&ueS1apId);

         if (ueS1apId != NULLP)
         {
            switch (ueS1apId->choice.val)
            {
               case UE_S1AP_IDS_UE_S1AP_ID_PAIR:
               {
                  /* If s1ap id was extracted from pair then fill
                     invalid pair id error in errPdu */
                  szErrCause = SztCauseRadioNwunknown_pair_ue_s1ap_idEnum;
                  break;
               default:
                  break;
               }
            }
         }
      }
      /* Call function to generate errPdu */
      szLiErrHndlr(pdu, CAUSE_RADIONW, szErrCause, &errPdu, mem);

#ifdef LSZV1
      /* Increment the error statistics */
      szIncErrSts(peerCb, CAUSE_RADIONW, szErrCause);
#endif /* LSZV1 */

      /* Send decoded data to its User */
      ret = szLiSndToLower(peerCb, conCb, errPdu);
#if (ERRCLASS & ERRCLS_INT_PAR)
      if(ret != ROK)
      {
            SZLOGERROR(ERRCLS_INT_PAR, ESZ159, (ErrVal)ret,
                  "szSmIdleStPeerErr: Lower Interface send failed");
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }
#endif /*SZ_MME */

   SZ_FREE_SDU(pdu);

   RETVALUE(ret);
} /* end of szSmIdleStPeerErr */

/*
*
*       Fun:   szSmPeerErrHdlr
*
*       Desc:  Called to send error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmPeerErrHdlr
(
SzConCb  *conCb,                      /* Connection control block */
SzPeerCb *peerCb,
S1apPdu  *pdu                         /* S1AP PDU */
)
#else  /* ANSI */
PUBLIC S16 szSmPeerErrHdlr(conCb, peerCb, pdu)
SzConCb  *conCb;                      /* Connection control block */
SzPeerCb *peerCb;
S1apPdu  *pdu;                        /* S1AP PDU */
#endif  /* ANSI */
{
   S16      ret = RFAILED;           /* return value */
   S1apPdu  *errPdu;                 /* Response PDU */
   Mem      mem;                     /* Memory block */

   TRC2(szSmPeerErrHdlr)

   errPdu = NULLP;

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   peerCb->peerSts.cfm.nmbUnexpEvt++;

   /* Call function to generate errPdu */
   ret = szLiErrHndlr(pdu, CAUSE_PROTOCOL,
                      SztCauseProtmessage_not_compatible_with_receiver_stateEnum,
                                                                     &errPdu, mem);
   
#ifdef LSZV1
            /* Increment the error statistics */
            szIncErrSts(peerCb, CAUSE_PROTOCOL,
                   SztCauseProtmessage_not_compatible_with_receiver_stateEnum);
#endif /* LSZV1 */
   
   /* Send decoded data to its User */
   ret = szLiSndToLower(peerCb, conCb, errPdu);
   if(ret != ROK)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
            peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ160, (ErrVal) conCb->lclConRefNo,
            "szSmPeerErrHdlr: Lower Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   SZ_FREE_SDU(pdu);

   RETVALUE(ret);
} /* end of szSmPeerErrHdlr */

/*
*
*       Fun:   szSmSemPeerErr
*
*       Desc:  Called to send semantic error indication to peer node
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmSemPeerErr
(
SzConCb  *conCb,                      /* Connection control block */
SzPeerCb *peerCb,
S1apPdu  *pdu                         /* S1AP PDU */
)
#else  /* ANSI */
PUBLIC S16 szSmSemPeerErr(conCb, peerCb, pdu)
SzConCb  *conCb;                      /* Connection control block */
SzPeerCb *peerCb;
S1apPdu  *pdu;                        /* S1AP PDU */
#endif  /* ANSI */
{
   S16      ret = RFAILED;           /* return value */
   S1apPdu  *errPdu;                 /* Response PDU */
   Mem      mem;                     /* Memory block */

   TRC2(szSmSemPeerErr)

   errPdu = NULLP;

   mem.region = peerCb->sctSapCb->pst.region;
   mem.pool   = peerCb->sctSapCb->pst.pool;

   peerCb->peerSts.cfm.nmbUnexpEvt++;

   /* Call function to generate errPdu */
   ret = szLiErrHndlr(pdu, CAUSE_PROTOCOL,
                      SztCauseProtsemantic_errorEnum, &errPdu, mem);
   
#ifdef LSZV1
   /* Increment the error statistics */
   szIncErrSts(peerCb, CAUSE_PROTOCOL,
                           SztCauseProtsemantic_errorEnum);
#endif /* LSZV1 */

   /* Send decoded data to its User */
   ret = szLiSndToLower(peerCb,conCb, errPdu);
   if(ret != ROK)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.suConnId,
            peerCb->sztSapCb, SZT_CAUSE_LOWER_LAYER_SND_FAILED, 0);

#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ161, (ErrVal) conCb->lclConRefNo,
            "szSmPeerErrHdlr: Lower Interface send failed");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   }

   SZ_FREE_SDU(pdu);

   RETVALUE(ret);
} /* end of szSmSemPeerErr */

/*
*
*       Fun:  szSmRcvIdleStErrInd
*
*       Desc:  Called to handle receive of idle state error indication
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlienb.c
*
*/
#ifdef ANSI
PUBLIC S16 szSmRcvIdleStErrInd 
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else  /* ANSI */
PUBLIC S16 szSmRcvIdleStErrInd(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif  /* ANSI */
{
   S16         ret = ROK;            /* Return value */
   SztUDatEvnt uDatEvnt;             /* Connectionless data */

   TRC2(szSmRcvIdleStErrInd);

   SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));
 
   uDatEvnt.transId.pres = NOTPRSNT;
   uDatEvnt.peerId.val = peerCb->peerId;
   uDatEvnt.pdu = pdu;

   /* Send data to the user */
   ret = szUiSndUDatInd(peerCb, &uDatEvnt);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SZLOGERROR(ERRCLS_DEBUG, ESZ162, (ErrVal)peerCb->peerId,
                 "szSmRcvIdleStErrInd: Failed to send error \
                 indication the user");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ret);

} /* szSmRcvIdleStErrInd */

/*
*
*       Fun:  szSmRcvConnStErrInd
*
*       Desc:  Called to handle receive of connecting state error indication
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sz_smlienb.c
*
*/
#ifdef ANSI
PUBLIC S16 szSmRcvConnStErrInd
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else  /* ANSI */
PUBLIC S16 szSmRcvConnStErrInd(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif  /* ANSI */
{  
   S16 ret = RFAILED;
#ifdef SZ_MME    
   TknU8 *eNbId = NULLP;
#endif /*SZ_MME */
   SztRelInd relInd;

   TRC2(szSmRcvConnStErrInd)


#ifdef SZ_MME    
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      szGetIE(&pdu->pdu, Sztid_eNB_UE_S1AP_ID, &eNbId);
      if(eNbId != NULLP)
      {
         conCb->rmtConRefNo = (((SztENB_UE_S1AP_ID *)eNbId)->val); 
      }
   }
#endif /*SZ_MME */

   conCb->cd.spConnId = conCb->lclConRefNo;

   SZ_ZERO(&relInd, sizeof(SztRelInd));

   relInd.suConnId.pres = PRSNT_NODEF;
   relInd.suConnId.val  = conCb->cd.suConnId;
   relInd.spConnId = conCb->cd.spConnId;
   relInd.pdu = pdu;

   /* Change the state to connected */
   conCb->state = SZ_SMSZ_RELEASING;

   /* Send decoded data to its User */
   ret = szUiSndRelInd(conCb, &relInd);

   RETVALUE(ret);
} /* end of szSmRcvConnStErrInd */


#ifdef SZ_MME    
/*
*
*       Fun:  szSmRcvConnStHoFail
*
*       Desc:  Called to handle Handover Failure received in connecting state
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sq_sm.c
*
*/
#ifdef ANSI
PUBLIC S16 szSmRcvConnStHoFail
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else  /* ANSI */
PUBLIC S16 szSmRcvConnStHoFail(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif  /* ANSI */
{
   S16        ret = RFAILED;
   SztRelInd  relInd;

   TRC2(szSmRcvConnStHoFail)

   conCb->cd.spConnId = conCb->lclConRefNo;

   SZ_ZERO(&relInd, sizeof(SztRelInd));

   relInd.suConnId.pres = PRSNT_NODEF;
   relInd.suConnId.val  = conCb->cd.suConnId;
   relInd.spConnId      = conCb->cd.spConnId;
   relInd.pdu           = pdu;

   /* Change the state to idle */
   conCb->state = SZ_SMSZ_IDLE;

   /* Send decoded data to its User */
   ret = szUiSndRelInd(conCb, &relInd);

   /* Deallocate connection control block */
   ret = szNdbDeallocConCb(conCb->lclConRefNo, peerCb, SZ_CONN_REF_LCL);
#if (ERRCLASS & ERRCLS_DEBUG)
   if(ret == RFAILED)
   {
      SZLOGERROR(ERRCLS_DEBUG, ESZ163, (ErrVal) conCb->lclConRefNo,
           "szSmRcvConnStHoFail: Couldn't dealloc Connection CB");
   }
#endif  /* ERRCLASS & ERRCLS_DEBUG */
   RETVALUE(ret);
} /* end of szSmRcvConnStHoFail */
#endif /*SZ_MME */




/*
 *
 *       Fun:   szSmExcMt
 *
 *       Desc:  This function invokes state based event handler
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  sz_ui.c
 *
 */
 
#ifdef ANSI
PUBLIC S16 szSmExcMt
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu,
U8       evnt,
U8       dir
)
#else  /* ANSI */
PUBLIC S16 szSmExcMt(conCb, peerCb, pdu, evnt, dir)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
U8       evnt;
U8       dir;
#endif  /* ANSI */
{
   U8 state;
   S16 ret = RFAILED;

   TRC2(szSmExecMt)

   state = SZ_SMSZ_IDLE;
   
   if (conCb != NULLP) 
   {
      state = conCb->state;
   }

#ifdef SZ_MME
   if(peerCb->nodeType == LSZ_NODE_ENB)
   {
      if(dir == SZ_UI)
      {
         ret = szSmUiMme[evnt][state](conCb, peerCb, pdu);
      }
      else if(dir == SZ_LI)
      {
         ret = szSmLiMme[evnt][state](conCb, peerCb, pdu);
      }
   }
#endif /* SZ_MME */
   return(ret);
}

/**********************************************************************

         End of file:     sq_sm.c@@/main/nodal_main/3 - Fri Nov 22 17:55:49 2013

**********************************************************************/

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
------------ -------- ---- ----------------------------------------------
/main/5      ---      va          1. Updated for S1AP release 4.1
*********************************************************************91*/
