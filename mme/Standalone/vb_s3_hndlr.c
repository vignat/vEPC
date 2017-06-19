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

     Name:    vb_s10_hndlr.c

     Desc:    Trillium LTE CNE - S3 Interface Handler functions

******************************************************************************/

#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent           */
#include "envind.h"        /* Environment independent         */

#include "gen.h"           /* General                         */
#include "ssi.h"           /* System services                 */
#include "ss_queue.h"      /* System services interface       */
#include "ss_mem.h"        /* System services interface       */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library            */
#include "eg_dbutl.h"           /* EG Upper Interface              */
#include "egt.h"           /* EG Upper Interface              */
#include "leg.h"           /* EG LM Interface                 */
#include "szt.h"           /* S1AP Upper Interface            */
#include "szt_asn.h"       /* S1AP ASN                        */
#include "lvb.h"           /* CNE Layer management            */
#include "vb_hss.h"        /* CNE Application defines         */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines         */
#include "cm_emm.h"        /* CNE Application defines         */
#include "cm_esm.h"        /* CNE Application defines         */
#include "cm_emm_esm.h"    /* EMM/ESM EDM                     */
#include <stdlib.h>        /* For atoi()                      */
#ifdef VB_MME_AUTH
#include "vb_hss_autn.h"
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */

/* header/extern include files (.x)                           */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "ss_queue.x"      /* System services interface       */
#include "ss_task.x"       /* System services interface       */
#include "ss_msg.x"        /* System services interface       */
#include "ss_mem.x"        /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library            */
#include "egt.x"           /* EG Upper Interface              */
#include "eg_util.x"           /* EG Upper Interface              */
#include "leg.x"           /* EG LM Interface                 */
#include "szt_asn.x"       /* S1AP ASN                        */
#include "szt.x"           /* S1AP Upper Interface            */
#include "lvb.x"           /* CNE Layer management           */
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */
#include "vb_hss.x"
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */
#include "vb_gtp_utl.x"
#include "vb_s3_hndlr.x"
#include "vb_s3_utl.x"

#ifdef __cplusplus
EXTERN "C"
{
#endif

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
 *    SUCCESS if Forward Relocation Request is send to SGSN successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndS3FRReq
(
VbMmeUeCb      *pUeCb,
CmTptAddr      *pTgtSgsnIpAddr
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TmrCfg      t3Tmr   = {0};
   TknU32      teidTkn = {0};
   TknU8       create  = {0};

   VB_TRC2(vbMmeSndS3FRReq);

   /* Pass the local Tunnel ID as 0, It will be allocated by EGTP layer
    * and returned when Fwd Relocation Response message is received
    */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Creating of S3 C-Tunnel"));
   ret = vbMmeUtlCreateS3Tun(pUeCb, pTgtSgsnIpAddr, vbMmeUtlGetS3TransId());
   if (ROK == ret)
   {
      ret = vbMmeUtlBldS3FRReq(pUeCb, &pEgMsg);
      if (ROK == ret)
      {
         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val  = pUeCb->pUeS3TunCb->localTeIdKey;
         create.pres  = PRSNT_NODEF;
         create.val   = TRUE;
         t3Tmr.enb    = TRUE;
         t3Tmr.val    = VB_MME_EGT_T3_RESP_TMR;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Request to SGSN"));

         VbLiEgtSigReq(&(vbMmeCb.egtS3SapCb->pst),\
               (vbMmeCb.egtS3SapCb->spId), &(pUeCb->pUeS3TunCb->locAddr),\
               &(pUeCb->pUeS3TunCb->remAddr), teidTkn,\
               (pUeCb->pUeS3TunCb->transId), create, t3Tmr, pEgMsg);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Relocation "\
               "Request"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Creation of S3 C-Tunnel, FAILURE"));
   }

   RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Request received from SGSN over S3 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S3 tunnel
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
PUBLIC S16 vbMmeIncS3FRReq
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
)
{
   EgIe              *pEgIe = NULLP;
   Teid              remTeid = 0;
   S16               ret = ROK;
   VbMmeUeCb         *pUeCb = NULLP;
   VbMmeUeTunnelCb   *pS3Tun = NULLP;
   VbMmeUeCtxt       ueCtxt = {0};
   VbMmeGlbEnbId     gEnbId;
   VbMmeEnbCb        *pEnbCb = NULLP;

   VB_TRC2(vbMmeIncS3FRReq);

   ret = vbMmeUtlFndS3Tun(locTeid.val, pEgMsg->msgHdr.seqNumber, &pS3Tun);
   if (ROK == ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "S3 tunnel with TEID [%d] Already "\
            "Present", locTeid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "S3 tunnel with TEID [%ld] Already "\
            "Present", locTeid.val));
#endif
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Ignoring the Forward Relocation "\
            "Request received"));
      RETVALUE(ROK);
   }

  /* The remote teid is parsed here, so that while sending a Forward
    * Relocation Response with a failure cause, it can be sent to correct
    * tunnel
    */
   ret = vbMmeEgtUtlGetIe(pEgMsg, &pEgIe, EG_FTEID_IETYPE, 0);
   if (ROK == ret && NULLP != pEgIe)
   {
      remTeid = pEgIe->t.fTeid.teid;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Remote Teid Not present"));
      RETVALUE(RFAILED);
   }

   /* Target eNB ID */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target eNB ID from "\
         "Forward Relocation Request"));
   ret = vbMmeUtlGetS3TgtEnbId(pEgMsg, &gEnbId);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Finding eNodeB CB for UE"));
      ret = vbMmeUtlGetTgtEnbCb(&gEnbId, &pEnbCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Finding eNodeB CB for UE"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target eNB ID from "\
            "Forward Relocation Request"));
   }

   if (ROK == ret)
   {
      /* Allocating new suConnId for the UE and creating the UE CB */
      vbMmeUtlAllocConId(pEnbCb, &ueCtxt.suConnId);
      ret = vbMmeUtlAddUeCb(pEnbCb, &ueCtxt, &pUeCb);
      if (ROK == ret)
      {
         /* Forward relocation request is received during inter-RAT Handover */
         pUeCb->proc = VB_MME_HANDOVER_PROC;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Creating S3 Tunnel CB"));
         ret = vbMmeUtlCreateS3Tun(pUeCb, pRemAddr, pEgMsg->msgHdr.seqNumber);
         if (ROK == ret)
         {
            ret = vbMmeUtlValNStoreS3FRReq(pUeCb, pEgMsg);
            if (ROK == ret)
            {
#ifdef VB_MME_AUTH
               pUeCb->ueCtxt.mappedSecCtxt.lstUsdCnt = 0xFFFFFFFF;
               printf("\nSetting lastused Cnt is 0xFFFFFFFF\n");
               pUeCb->ueCtxt.mappedSecCtxt.intAlg = VB_MME_EIA1_128;
               pUeCb->ueCtxt.mappedSecCtxt.encAlg = VB_MME_EEA0;
               vbMmeGenerateMappedSecKeys(&pUeCb->ueCtxt.mappedSecCtxt);
               pUeCb->ueCtxt.mappedSecCtxt.lstUsdCnt = 0x0;
#endif
               pUeCb->pUeS3TunCb->localTeIdKey = locTeid.val;
               pUeCb->pHoData->hoMmeType = VB_MME_HO_TGT_MME;
               pUeCb->pHoData->hoType = VB_EMM_HO_UTRAN_TO_LTE;
               VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);
               VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_TAU_FOR_HO_REQD);
               pUeCb->emmCb.state = VB_EMM_UE_REGISTERED;
               pUeCb->ueCtxt.secCtxt = pUeCb->ueCtxt.mappedSecCtxt; /*RAJESH*/
               pUeCb->ueCtxt.secCtxt.intAlg =  VB_MME_EIA1_128;/*(VbMmeIntAlg)pEpsSec->nasProtAlgo;*/
               pUeCb->ueCtxt.secCtxt.encAlg = VB_MME_EEA0;/*(VbMmeEncAlg)pEpsSec->usedNasCipher*/;
               pUeCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb = 0/*pEpsSec->nasDlCount*/;
               pUeCb->ueCtxt.secCtxt.dlSeqNmb.ovrflwCnt = 1/*pEpsSec->nasDlCount*/;
               pUeCb->ueCtxt.secCtxt.ulSeqNmb.seqNmb = 0 /*pEpsSec->nasUlCount*/;
               pUeCb->ueCtxt.secCtxt.ulSeqNmb.ovrflwCnt = 1 /*pEpsSec->nasUlCount*/;
               pUeCb->ueCtxt.secCtxt.status = VB_MME_SEC_CTXT_CRNT;
               pUeCb->ueCtxt.secCtxt.status = VB_MME_SEC_CTXT_CRNT;
#ifdef VB_IRAT_ACC_TEST
               SGetSBuf(vbMmeCb.init.region, vbMmeCb.init.pool,
                     (Data **) &pUeCb->pHoData->tgtToSrcTCont.pVal, (Size) 200);
               if( pUeCb->pHoData->tgtToSrcTCont.pVal )
               {
                  pUeCb->pHoData->tgtToSrcTCont.len = 200;
                  ret = vbMmeSndS3FRRsp(pUeCb, EGT_GTPC_CAUSE_ACC_REQ);
                  if (ROK != ret)
                  {
                     VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward "
                              "Relocation Response to SGSN"));
                  }
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward "
                           "Relocation Response to SGSN"));
                  sleep(1);
                  vbMmeSndS3FRCmplNotif(pUeCb);
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward "
                           "Relocation Complete Notification to SGSN"));
               }
#else
               pUeCb->pHoData->pTgtEnbCb = pEnbCb;
               pUeCb->pHoData->pNewUeCb = pUeCb;

               ret = vbMmeSndHOReq(pUeCb, pUeCb->eNodebCb);
               if (ROK != ret)
               {
                  VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover "\
                        "Request to eNodeB."));
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward"
                     "Relocation Response with Failure cause to SGSN"));
                  ret = vbMmeSndS3FRRsp(pUeCb, EGT_GTPC_CAUSE_SYS_FAIL);
                  if (ROK != ret)
                  {
                     VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward "
                           "Relocation Response with Failure cause to SGSN"));
                  }
               }
#endif
            }
            else
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation and Storing of "\
                     "Forward Relocation Request"));
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Creating S3 Tunnel CB"));
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Creating UE CB"));
      }
   }

   if (ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation"\
            " Response with Failure cause to SGSN"));
      ret = vbMmeEgtUtlSndFailureRsp(locTeid.val, remTeid, EGT_GTPC_MSG_FR_RSP,\
            pEgMsg->msgHdr.seqNumber, EGT_GTPC_CAUSE_REQ_REJECT);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Relocation"\
               " Response with Failure cause to SGSN, FAILURE"));
      }

      if (NULLP != pUeCb)
      {
         vbMmeUtlDelUeCb(pUeCb);
      }
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Response received from SGSN over S3 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S3 tunnel
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
PUBLIC S16 vbMmeIncS3FRRsp
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS3Tun = NULLP;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeIncS3FRRsp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S3 Tunnel, "
         "Transaction ID: [%ld]", transId.val));
   ret = cmHashListFind(&vbMmeCb.egtS3SapCb->tunnPendCbHlCp,\
         (U8 *)&(transId.val), sizeof(transId.val), FALSE, (PTR *)&pS3Tun);
   if (ROK == ret)
   {
      pUeCb = pS3Tun->ueCb;
      pS3Tun->localTeIdKey = locTeid.val;
      pS3Tun->seqNum = pEgMsg->msgHdr.seqNumber;

      ret = vbMmeUtlValNStoreS3FRRsp(pUeCb, pEgMsg);
      if (ROK == ret)
      {
         /* removing from pending tunnel cb to ueTunnelCbHlCp */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting S3 Tunnel from Pending "\
               "Tunnel Hash Table"));
         cmHashListDelete(&(vbMmeCb.egtS3SapCb->tunnPendCbHlCp),\
               (PTR)pS3Tun);

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding S3 Tunnel to "\
               "Tunnel Hash Table, Tunnel ID [%ld]", pS3Tun->localTeIdKey));
         ret = cmHashListInsert(&(vbMmeCb.egtS3SapCb->ueTunnelCbHlCp),\
               (PTR)pS3Tun, (U8*)&(pS3Tun->localTeIdKey),\
               sizeof(pS3Tun->localTeIdKey));
         if (ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validated Forward Relocation "\
                  "Response message"));

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Command "\
                  "to eNodeB"));
#ifndef VB_IRAT_ACC_TEST
            ret = vbMmeSndHOCmd(pUeCb, pUeCb->eNodebCb);
#endif
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "failed to Send Handover Command "\
                     "to eNodeB"));
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding S3 Tunnel to "\
                  "Tunnel Hash Table"));
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validating Forward Relocation "\
               "Response message"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S3 Tunnel, Teid: [%ld]",\
            locTeid.val));
   }

   if (ROK != ret && NULLP != pUeCb)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Peparation "
            "Failure to eNodeB"));
      ret = vbMmeSndHOPrepFail(pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Peparation "
               "Failure to eNodeB"));
      }

   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Builds and Sends a forward relocation response to SGSN
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which S1 Handover is being handled
 * @param[in]  cause
 *    GTP Cause Value to be sent to SGSN
 *
 * @return
 *    SUCCESS if Forward Relocation Response is send to SGSN successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndS3FRRsp
(
VbMmeUeCb      *pUeCb,
U32            cause
)
{
   S16               ret     = ROK;
   EgMsg             *pEgMsg = NULLP;
   TknU32            teidTkn = {0};
   VbMmeUeTunnelCb   *pS3Tun = NULLP;

   VB_TRC2(vbMmeSndFRRsp);

   pS3Tun = pUeCb->pUeS3TunCb;

   /* Remove the S3 tunnel from pending tunnel hash table to
    * ueTunnelCb hash table
    */
   cmHashListDelete(&(vbMmeCb.egtS3SapCb->tunnPendCbHlCp),\
         (PTR)pS3Tun);
   ret = cmHashListInsert(&(vbMmeCb.egtS3SapCb->ueTunnelCbHlCp),\
         (PTR)pS3Tun, (U8*)&(pS3Tun->localTeIdKey),\
         sizeof(pS3Tun->localTeIdKey));
   if (ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding S3 C-Tunnel to "\
            "Hash Table"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }

   ret = vbMmeUtlBldS3FRRsp(pUeCb, cause, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Response SGSN"));
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = pUeCb->pUeS3TunCb->localTeIdKey;

      VbLiEgtSigRsp(&(vbMmeCb.egtS3SapCb->pst),\
            (vbMmeCb.egtS3SapCb->spId), teidTkn, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Relocation "\
            "Response"));
   }

   /* Any other cause means the local tunnel has to be deleted */
   if (cause != EGT_GTPC_CAUSE_ACC_REQ)
   {
      vbMmeUtlDelS3Tun(pUeCb);
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Sends Forward Relocation Complete notification message to SGSN
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Notification is sen3 to SGSN
 *    successfully otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndS3FRCmplNotif
(
VbMmeUeCb      *pUeCb
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TmrCfg      t3Tmr   = {0, 0};
   TknU32      teidTkn = {0};
   TknU8       create  = {0};

   VB_TRC2(vbMmeSndS3FRCmplNotif);

   ret = vbMmeUtlBldS3FRCmplNotif(pUeCb, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Relocation "\
            "Complete Notification, SUCCESS"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = pUeCb->pUeS3TunCb->localTeIdKey;
      create.pres  = PRSNT_NODEF;
      create.val   = FALSE;
      t3Tmr.enb    = TRUE;
      t3Tmr.val    = VB_MME_EGT_T3_RESP_TMR;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Complete Notification to SGSN"));
      VbLiEgtSigReq(&(vbMmeCb.egtS3SapCb->pst),\
            (vbMmeCb.egtS3SapCb->spId), &(pUeCb->pUeS3TunCb->locAddr),\
            &(pUeCb->pUeS3TunCb->remAddr), teidTkn,\
            (pUeCb->pUeS3TunCb->transId), create, t3Tmr, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Relocation "\
            "Compete Notification"));
   }

   RETVALUE(ret);
}



/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Complete Notification
 *    received from SGSN over S3 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S3 tunnel
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
PUBLIC S16 vbMmeIncS3FRCmplNotif
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS3Tun = NULLP;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeIncS3FRCmplNotif);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Forward Relocation Complete "\
         "Notification on S3"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S3 Tunnel, Tunnel ID: [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS3SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS3Tun);
   if (ROK == ret)
   {
      pS3Tun->seqNum = pEgMsg->msgHdr.seqNumber;
      pUeCb = pS3Tun->ueCb;

#ifndef VB_IRAT_ACC_TEST
      /*Start a Timer to Send S1AP for 1 second : UE Context Release Command */

      vbMmeStartTmr((PTR)pUeCb, VB_TMR_EMM_IRAT_UE_CTXT_REL,
            VB_TMR_EMM_IRAT_CTXT_REL_CFG);
#endif
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Complete Acknowledge to SGSN"));

      ret = vbMmeSndS3FRCmplAck(pUeCb, EGT_GTPC_CAUSE_ACC_REQ);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Complete Acknowledge to SGSN"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S3 Tunnel, Tunnel ID: [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Builds and Sends a forward relocation Compelete Ack to SGSN
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which S1 Handover is being handled
 * @param[in]  cause
 *    GTP Cause Value to be sent to SGSN
 *
 * @return
 *    SUCCESS if Forward Relocation Complete Ack is send to SGSN successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndS3FRCmplAck
(
VbMmeUeCb      *pUeCb,
U32            cause
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TknU32      locTeid = {0};

   VB_TRC2(vbMmeSndS3FRCmplAck);

   ret = vbMmeUtlBldS3FRCmplAck(pUeCb, cause, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Complete Acknowledge to SGSN"));

      locTeid.pres = PRSNT_NODEF;
      locTeid.val = pUeCb->pUeS3TunCb->localTeIdKey;
      VbLiEgtSigRsp(&(vbMmeCb.egtS3SapCb->pst),\
            (vbMmeCb.egtS3SapCb->spId), locTeid, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Relocation "\
            "Compete Acknowledge"));
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Handles Fowrard Relocation Complete Acknowledge
 *    received from SGSN over S3 interface
 *
 * @param[in]  pLocAddr
 *    Pointer to local address at which the request is received
 * @param[in]  pRemAddr
 *    Pointer to remote address from where the request is received
 * @param[in]  locTeid
 *    Local Teid of S3 tunnel
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
PUBLIC S16 vbMmeIncS3FRCmplAck
(
CmTptAddr   *pLocAddr,
CmTptAddr   *pRemAddr,
TknU32      locTeid,
TknU32      transId,
EgMsg       *pEgMsg
)
{
   S16               ret = ROK;
   VbMmeEsmCb        *pEsmCb = NULLP;
   VbMmeUeTunnelCb   *pS3Tun = NULLP;

   VB_TRC2(vbMmeIncS3FRCmplAck);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Forward Relocation Complete "\
         "Ack"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S3 Tunnel, Tunnel ID [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS3SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS3Tun);
   if (ROK == ret)
   {
      ret = vbMmeUtlValNStoreS3FRCmplAck(pS3Tun->ueCb, pEgMsg);
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Modify Bearer Request "\
               "to SGW"));

         pEsmCb = pS3Tun->ueCb->esmList[0];
         ret = vbMmeSndMBReqiRAT(pS3Tun->ueCb, pEsmCb, FALSE);
         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Modify Bearer Request "\
                  "to SGW Failed"));
         }

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting the S3 Tunnel"));
         vbMmeUtlDelS3Tun(pS3Tun->ueCb);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation and Storing of "\
               "Forward Relocation Complete Acknowledge"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S3 Tunnel, Tunnel ID [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}


#ifdef __cplusplus
}
#endif

/******************************************************************************

                                 END OF FILE

******************************************************************************/
