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

     Name:    vb_s3_utl.c

     Desc:    Trillium LTE CNE - S3 Interface Utility functions

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
#include "egt.h"           /* EG Upper Interface              */
#include "leg.h"           /* EG LM Interface                 */
#include "eg_dbutl.h"           /* EG LM Interface                 */
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
#include "leg.x"           /* EG LM Interface                 */
#include "eg_util.x"       /* EG LM Interface                 */
#include "egt_util.x"      /* EGT Utils                       */
#include "szt_asn.x"       /* S1AP ASN                        */
#include "szt.x"           /* S1AP Upper Interface            */
#include "lvb.x"           /* CNE Layer management            */
#include "cm_esm.x"        /* CNE Application structures      */
#include "cm_emm.x"        /* CNE Application structures      */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures      */
#include "vb.x"            /* CNE Application structures      */
#include "vb_hss.x"
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */
#include "vb_s3_utl.x"
#include "vb_gtp_utl.x"

#ifdef __cplusplus
EXTERN "C"
{
#endif
/**************************************************************************//**
 *
 * @brief
 *    Returns new transaction ID on S3 interface
 *
 * @return
 *    New transaction ID
 *
 *****************************************************************************/
PUBLIC U32 vbMmeUtlGetS3TransId()
{
   return ++vbMmeCb.s3TransIdCntr;
}


/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Relocation Request message.
 *    Allocates memory for the GTP message
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocReq
 *    Pointer to Egtp message structure
 *
 * @return
 *    SUCCESS if Forward Relocation Request is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldS3FRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppFwdLocReq
)
{
   S16               ret        = ROK;
   Mem               memInfo    = {0, 0};
   EgMsg             *pEgMsg    = NULLP;
   EgIeLevelInfo     egLvlInfo  = {0};
   VbMmeUeTunnelCb   *pS3Tun   = NULLP;
   U8                ip4Addr[4] = {'\0'};
   EgIe              egIe;

   VB_TRC2(vbMmeUtlBldFRReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Relocation Request"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   if (egUtlAllocEgMsg(ppFwdLocReq, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   pEgMsg         = *ppFwdLocReq;
   pS3Tun        = pUeCb->pUeS3TunCb;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&pEgMsg->msgHdr, pUeCb->pUeS3TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_REQ, pS3Tun->transId.val);

   /* imsi */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding IMSI"));
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   if(pUeCb->ueCtxt.ueImsiLen <= 15)
   {
   ret = egUtlEgtAddImsi(pEgMsg, &egLvlInfo, pUeCb->ueCtxt.ueImsi,\
         pUeCb->ueCtxt.ueImsiLen, 0);
   }
   else
   {
      ret = RFAILED;
   }
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding IMSI"));
   }

   /* sender f-teid, c-plane */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Sender C-Plane F-TEID"));
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      vbUtlIpv4InetToArr(ip4Addr, &(pS3Tun->locAddr.u.ipv4TptAddr.address));
      ret = egUtlEgtAddFteid(pEgMsg, &egLvlInfo, pS3Tun->localTeIdKey,\
            ip4Addr, EGT_INT_S3_MME_GTP_C, 0);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Sender C-Plane F-TEID"));
      }
   }

   /* PDN Connection */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding EPC MME PDN Connections"));
      ret = vbMmeEgtUtlAddEpsPdn(pEgMsg, pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding EPC MME PDN Connections"));
      }
   }

   /* SGW f-teid, c-plane */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding SGW C-Plane F-TEID"));
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      vbUtlIpv4InetToArr(ip4Addr, &(pUeCb->ueTunnelCb->locAddr.u.\
            ipv4TptAddr.address));
      ret = egUtlEgtAddFteid(pEgMsg, &egLvlInfo, pUeCb->ueTunnelCb->\
            localTeIdKey, ip4Addr, EGT_INT_S11_S4_SGW_GTP_C, 1);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding SGW C-Plane F-TEID"));
      }
   }

#ifdef VB_MME_AUTH
   /*To do : UE MM Context */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE MM Context"));
      ret = vbMmeEgtUtlAddUeS3MmCtx(pEgMsg, pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding UE MM Context"));
      }
   }
#endif /* VB_MME_AUTH */

   /* UTRAN Transparent Container */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UTRAN Transparent "\
            "Container"));
      ret = vbMmeEgtUtlAddUtranTransCont(pEgMsg, &pUeCb->pHoData->srcToTgtTCont);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding UTRAN Transparent "\
               "Container"));
      }
   }

   /* Target (target RNC id) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Target ID"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.t.targetId.targetType = EGT_TGT_RNC_ID; /* RNC ID type */
      egIe.t.targetId.targetId.length = EGT_TGT_RNC_ID_LEN;

      /* Copying PLMN ID from LAI to MCC and MNC of Target Id (3bytes)*/
      egIe.t.targetId.targetId.val[0] =
         pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val[0];
      egIe.t.targetId.targetId.val[1] =
         pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val[1];
      egIe.t.targetId.targetId.val[2] =
         pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val[2];

      /*copying LAC from LAI to Target Id (2bytes)*/
      egIe.t.targetId.targetId.val[3] =
         pUeCb->pHoData->hoTgt.tgtRncId.laiId.lAC.val[0];
      egIe.t.targetId.targetId.val[4] =
         pUeCb->pHoData->hoTgt.tgtRncId.laiId.lAC.val[1];

      /*copying RNCID  to Target Id (2bytes) */
      egIe.t.targetId.targetId.val[5] =
         (U8)((pUeCb->pHoData->hoTgt.tgtRncId.rncId.val >> 8)& 0xff);
      egIe.t.targetId.targetId.val[6] =
         (U8)(pUeCb->pHoData->hoTgt.tgtRncId.rncId.val & 0xff);


      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_TARGET_ID_IETYPE, 0,\
            EG_TRGTID, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Target ID"));
      }
   }

   /* RAN Cause */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "RAN Cause"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));

      egIe.t.fCause.causeType = pUeCb->pHoData->hoCause.causeType;
      egIe.t.fCause.cause = (U16)pUeCb->pHoData->hoCause.cause;

      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_F_CAUSE_IETYPE, 0,\
            EG_F_CAUSE, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding RAN Cause"));
      }
   }

   /* Selected PLMN ID */

   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Selected PLMN ID"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));

      egIe.t.valStr8.length = 3; /* PLMN ID Length*/
      cmMemcpy(egIe.t.valStr8.val,
         pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val,3);

      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_SEL_PLMN_ID_IETYPE, 0,\
            EG_STRING_8, &egIe);

      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Target ID"));
      }
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(&pEgMsg);
   }

   RETVALUE(ret);
}


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
 * @param[out] ppFRRsp
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
EgMsg          **ppFRRsp
)
{
   VbMmeEsmCb        *pEsmCb    = NULLP;
   S16               ret        = ROK;
   Mem               memInfo    = {0, 0};
   EgMsg             *pEgMsg    = NULLP;
   VbMmeUeTunnelCb   *pS3Tun   = NULLP;
   EgIe              egIe;

   VB_TRC2(vbMmeUtlBldFRRsp);

   pS3Tun        = pUeCb->pUeS3TunCb;
   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Relocation Response"));
   if (egUtlAllocEgMsg(ppFRRsp, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   pEgMsg = *ppFRRsp;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&pEgMsg->msgHdr, pUeCb->pUeS3TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_RSP, pUeCb->pUeS3TunCb->seqNum);

   /* Cause IE */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Cause IE"));
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Cause IE"));
   }

   /* If cause is a failure cause, dont encode the rest of the IEs */
   if (EGT_GTPC_CAUSE_ACC_REQ != cause)
   {
      RETVALUE(ROK);
   }

   /* Sender F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Sender FTEID IE"));
      egIe.t.fTeid.intfType = EGT_INT_S3_MME_GTP_C;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.teid = pS3Tun->localTeIdKey;
      vbUtlIpv4InetToArr(egIe.t.fTeid.ip4Addr,\
            &(pS3Tun->locAddr.u.ipv4TptAddr.address));
      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_FTEID_IETYPE, 0, EG_FTEID, &egIe);
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Sender FTEID IE"));
      }
   }

   /* Indication */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Indication IE"));
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.t.indication.isSGWCIPres = TRUE;
      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_INDICATION_IETYPE, 0,\
            EG_INDICATION, &egIe);
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Indication IE"));
      }
   }

   /* List of setup Bearers */
   if (ROK == ret)
   {
      pEsmCb = pUeCb->esmList[0];
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding List of Set-up Bearers IE"));
      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_BEARER_CNTX_IETYPE, 1,\
            EG_GROUPED, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding List of Set-up Bearers IE"));
      }

      /* Add EBI ie into Bearer Context */
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding EBI IE into Bearer "\
               "Context to be Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.valUnSgnd8 = pEsmCb->bId;
         ret = vbMmeEgtUtlAddRABBCtxIe(pEgMsg, EG_EBI_IETYPE, 0,\
               EG_UNSIGNED_8, &egIe);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding EBI IE into Bearer "\
                  "Context to be Modified IE"));
         }
      }

      /* Add eNB DL GTP TEID and Address */
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding eNB DL FTEID IE into "\
               "Bearer Context Created IE"));

         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.ieInst = 0;
         egIe.t.fTeid.teid = pUeCb->pHoData->dlGtpTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_X2_DL_ENB_GTP_U;
         vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(pUeCb->pHoData->dlTransportLyrAddr.u.ipv4NetAddr));
         ret = vbMmeEgtUtlAddRABBCtxIe(pEgMsg, EG_FTEID_IETYPE, 0,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding eNB DL FTEID IE into "\
                  "Bearer Context Created IE"));
         }
      }

      /* Add eNB UL GTP TEID and Address */
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding eNB UL FTEID IE into "\
               "Bearer Context Created IE"));

         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.ieInst = 0;
         egIe.t.fTeid.teid = pUeCb->pHoData->ulGtpTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_X2_UL_ENB_GTP_U;
         vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(pUeCb->pHoData->ulTransportLyrAddr.u.ipv4NetAddr));
         ret = vbMmeEgtUtlAddRABBCtxIe(pEgMsg, EG_FTEID_IETYPE, 1,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding eNB UL FTEID IE into "\
                  "Bearer Context Created IE"));
         }
      }
   }

   /* E-UTRAN transparent container */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding E-UTRAN Transparent "\
            "Container"));
      ret = vbMmeEgtUtlAddEUTransCont(pEgMsg, &pUeCb->pHoData->tgtToSrcTCont);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding E-UTRAN Transparent "\
               "Container"));
      }
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(&pEgMsg);
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Relocation Complete Notification message.
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
PUBLIC S16 vbMmeUtlBldS3FRCmplNotif
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppEgMsg
)
{
   S16         ret     = ROK;
   Mem         memInfo = {0, 0};
#if 0
   EgIe        egIe;
#endif
   U32         seqNum  = 0;

   VB_TRC2(vbMmeUtlBldFRCmplNotif);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if (egUtlAllocEgMsg(ppEgMsg, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   seqNum = ++pUeCb->pUeS3TunCb->seqNum;
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pUeCb->pUeS3TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_COMP_NOTFN, seqNum);
   /* In IRAT HO, Indication flag should be added to msg if ISR bit is set in
      the  IE. Otherwise need not to add the indication IE to msg. */
#if 0
      /* Indication */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Indication IE"));
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      ret = vbMmeEgtUtlAppendIe(*ppEgMsg, EG_INDICATION_IETYPE, 0,\
            EG_INDICATION, &egIe);
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Indication IE"));
      }
#endif

   RETVALUE(ret);
}

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
CmTptAddr      *pRemSgsnIpAddr,
U32            transId
)
{
   S16                  ret = ROK;
   VbMmeUeTunnelCb      *pCTun = NULLP;

   VB_MME_TRC2(vbMmeUtlCreateS3Tun);

   VB_MME_ALLOC(&pCTun, sizeof(VbMmeUeTunnelCb));
   if (pCTun == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation VbMmeUeTunnelCb"));
      RETVALUE(RFAILED);
   }

   pCTun->localTeIdKey = 0;
   pCTun->transId.pres = PRSNT_NODEF;
   pCTun->transId.val = transId;
   pCTun->locAddr = vbMmeCb.mmeCfg.s3selfAddr;
   pCTun->remAddr = *pRemSgsnIpAddr;

   ret = cmHashListInsert(&(vbMmeCb.egtS3SapCb->tunnPendCbHlCp),\
         (PTR)pCTun, (U8*)&(pCTun->transId.val), sizeof(pCTun->transId.val));
   if (ret != ROK)
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding S3 C-Tunnel to Hash Table"));
      VB_MME_FREE(pCTun, sizeof(VbMmeUeTunnelCb));
   }
   else
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Added S3 C-Tunnel to Hash Table "\
            "Transaction ID [%d]", pCTun->transId.val));
#else
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Added S3 C-Tunnel to Hash Table "\
            "Transaction ID [%ld]", pCTun->transId.val));
#endif

      pCTun->ueCb = pUeCb;
      pUeCb->pUeS3TunCb = pCTun;
   }

   RETVALUE(ret);
}


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
)
{
   S16                  ret = ROK;
   VbMmeUeTunnelCb      *pS3Tun = NULLP;

   VB_MME_TRC2(vbMmeUtlDelS3Tun);

   if (NULL != pUeCb->pUeS3TunCb)
   {
      pS3Tun = pUeCb->pUeS3TunCb;
      VbLiEgtLclDelReq(&(vbMmeCb.egtS3SapCb->pst), vbMmeCb.egtS3SapCb->spId,\
            0, pS3Tun->localTeIdKey);

      cmHashListDelete(&(vbMmeCb.egtS3SapCb->ueTunnelCbHlCp), (PTR)pS3Tun);
      VB_MME_FREE(pS3Tun, sizeof(VbMmeUeTunnelCb));

      pUeCb->pUeS3TunCb = NULLP;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Parameter, "\
            "S3 Tunnel CB is NULL"));
      ret = RFAILED;
   }

   RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Request message
 *    Creates and Stores ESM CBs (bearers) and UE releated information
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] pFRReq
 *    Pointer to Egtp message structure with Forward Relocation Request
 *
 * @return
 *    SUCCESS Forward Relocation Request is validated successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreS3FRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFRReq
)
{
   S16               ret      = ROK;
   EgIe              *pEgIe   = NULLP;
   VbMmeUeTunnelCb   *pS3Tun = NULLP;
   VbMmeEsmCb        *pEsmCb  = NULLP;
   VbMmePdnCb        *pPdnCb  = NULLP;
   EgUMTSKeyQt       *pUmtsSec = NULLP;
#ifdef VB_MME_AUTH
   U32               idx;
#endif


   VB_TRC2(vbMmeUtlValNStoreFRReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Forward "\
         "Relocation Request"));

   VB_MME_ALLOC(&pUeCb->pHoData, sizeof(VbMmeHoData));
   if (NULLP == pUeCb->pHoData)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation: VbMmeHoData"));
      RETVALUE(RFAILED);
   }

   pS3Tun = pUeCb->pUeS3TunCb;
   pS3Tun->seqNum = pFRReq->msgHdr.seqNumber;

   /* IMSI */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting IMSI"));
   ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_IMSI_IETYPE, 0);
   if (ROK == ret)
   {
      pUeCb->ueCtxt.ueImsiLen = vbMmeEgtUtlGetImsi(pEgIe->t.valStr32.val,\
            pEgIe->t.valStr32.length, pUeCb->ueCtxt.ueImsi);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting IMSI"));
   }

   /* Sender F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Sender F-TEID (S3)"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_FTEID_IETYPE, 0);
      if (ROK == ret)
      {
         pS3Tun->remoteTeId = pEgIe->t.fTeid.teid;
         pS3Tun->remAddr.type = CM_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pS3Tun->remAddr.u.ipv4TptAddr.address));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Sender F-TEID (S3)"));
      }
   }

   /* PDN Connection IE (only one PDN is considered) */
   if (ROK == ret)
   {
      /* Bearer Context */
      /* EPS bearer ID */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting EBI Ie in Bearer "\
            "Context IE within PDN Connection IE"));
      ret = vbMmeEgtUtlGetEpsPdnConnIe(pFRReq, EG_EBI_IETYPE, 0, &pEgIe);
      if (ROK == ret)
      {
         ret = vbMmeUtlAddEsmCb(pUeCb, &pEsmCb, pEgIe->t.valUnSgnd8, 0);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation: "\
                  "VbMmeEsmCb"));
         }
         else
         {
            pEsmCb->bearerType = CM_ESM_EPS_DEF_BEARER;
            pEsmCb->state = VB_ESM_ST_BEARER_INACTIVE;

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding PDN cb"));
            ret = vbMmeUtlAddPdnCb(pUeCb, pEsmCb, &pPdnCb);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding PDN cb"));
            }
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting EBI Ie in Bearer "\
               "Context IE within PDN Connection IE"));
      }
   }

   /* APN (PDN Connection IE) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting APN IE within "\
            "PDN Connection IE"));
      ret = vbMmeEgtUtlGetEpsPdnConnIe(pFRReq, EG_APN_IETYPE, 0, &pEgIe);
      if (ROK == ret)
      {
         pEsmCb->apn.pres = TRUE;
         pEsmCb->apn.len = pEgIe->t.valStr132.length;
         cmMemcpy(pEsmCb->apn.apn, pEgIe->t.valStr132.val,\
               pEsmCb->apn.len);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting APN IE within "\
               "PDN Connection IE"));
      }
   }

   /* UE IP Address (PDN Connection IE) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving IP Address within "\
            "PDN Connection IE"));
      ret = vbMmeEgtUtlGetEpsPdnConnIe(pFRReq, EG_IPADDR_IETYPE, 0, &pEgIe);
      if (ROK == ret)
      {
         pUeCb->pHoData->ueAddr.type = CM_IPV4ADDR_TYPE;
         pUeCb->pHoData->ueAddr.u.ipv4NetAddr = pEgIe->t.ipAddr.u.ipv4;
         /*pUeCb->ueTunnelCb->ipAddr.type = CM_IPV4ADDR_TYPE;
         pUeCb->ueTunnelCb->ipAddr.u.ipv4NetAddr = pEgIe->t.ipAddr.u.ipv4;*/
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving IP Address within "\
               "PDN Connection IE"));
      }
   }

   /* Linked EPS Bearer Id (PDN Connection IE) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving Linked EPS Bearer ID"));
      ret = vbMmeEgtUtlGetEpsPdnConnIe(pFRReq, EG_EBI_IETYPE, 0,\
            &pEgIe);
      if (ROK == ret)
      {
         pEsmCb->linkBearId = pEgIe->t.valUnSgnd8;
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving Linked EPS Bearer ID"));
      }
   }

   /* PGW F-TEID (PDN Connection IE) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving PGW F-TEID S5/S8"));
      ret = vbMmeEgtUtlGetEpsPdnConnIe(pFRReq, EG_FTEID_IETYPE, 0,\
            &pEgIe);
      if (ROK == ret)
      {
         pEsmCb->pdnCb->s5PgwTeId = pEgIe->t.fTeid.teid;
         pEsmCb->pdnCb->s5PgwAddr.type = CM_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pEsmCb->pdnCb->s5PgwAddr.u.ipv4NetAddr));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving PGW F-TEID S5/S8"));
      }
   }

   /* APN AMBR (PDN Connection IE) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting AMBR in PDN Connection"));
      ret = vbMmeEgtUtlGetEpsPdnConnIe(pFRReq, EG_AMBR_IETYPE, 0,\
            &pEgIe);
      if (ROK == ret)
      {
         pEsmCb->ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl = pEgIe->t.ambr.dnlAPNAMBR;
         pEsmCb->ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul = pEgIe->t.ambr.uplAPNAMBR;
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting AMBR in PDN Connection"));
      }
   }

    /* Bearer Context IEs (SGW S4 U FTEID) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving SGW U F-TEID IE "\
            "within Bearer Context within PDN Connection"));
      ret = vbMmeEgtUtlGetBCtxIeInPdnConn(pFRReq,\
            EG_FTEID_IETYPE, 0, &pEgIe);
      if (ROK == ret)
      {
         pEsmCb->s4SgwUTeid = pEgIe->t.fTeid.teid;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pEsmCb->s4SgwUAddr.u.ipv4NetAddr));
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving SGW U F-TEID IE "\
               "within Bearer Context within PDN Connection"));
      }

   }

    /* Bearer Context IEs (PGW S5/S8 U FTEID) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving PGW U F-TEID IE "\
            "within Bearer Context within PDN Connection"));
      ret = vbMmeEgtUtlGetBCtxIeInPdnConn(pFRReq,\
            EG_FTEID_IETYPE, 1, &pEgIe);
      if (ROK == ret)
      {
         pEsmCb->s5PgwUTeid = pEgIe->t.fTeid.teid;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pEsmCb->s5PgwUAddr.u.ipv4NetAddr));
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving PGW U F-TEID IE "\
               "within Bearer Context within PDN Connection"));
      }
   }

   /* Bearer Context IEs (QoS) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving Bearer QoS IE "\
            "within Bearer Context within PDN Connection"));
      ret = vbMmeEgtUtlGetBCtxIeInPdnConn(pFRReq,\
            EG_BEARER_QOS_IETYPE, 0, &pEgIe);
      if (ROK == ret)
      {
         /* only default bearer is handled so MBR/GBR is not applicable */
         pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.qci = pEgIe->t.bQos.qci;
         pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.priLevel = \
               pEgIe->t.bQos.prityLvl;
         pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.preCapbFlg = \
               (U32)pEgIe->t.bQos.isPCIPres;
         pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.preVlnbFlg = \
               (U32)pEgIe->t.bQos.isPVIPres;
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving Bearer QoS IE "\
               "within Bearer Context within PDN Connection"));
      }
   }

   /* SGW F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting SGW F-TEID (S4)"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_FTEID_IETYPE, 1);
      if (ROK == ret)
      {
         pUeCb->pHoData->sgwTeId = pEgIe->t.fTeid.teid;
         pUeCb->pHoData->sgwAddr.type = CM_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pUeCb->pHoData->sgwAddr.u.ipv4TptAddr.address));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting SGW F-TEID (S4)"));
      }
   }


   /* UE MM Context */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving UE MM Context"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_UMTS_KEY_QUIN_IETYPE, 0);
      if (ROK == ret)
      {
#ifdef VB_MME_AUTH

         pUmtsSec                = &pEgIe->t.umtsKeyQt;

         pUeCb->ueCtxt.mappedSecCtxt.secMode = pUmtsSec->secMode;

         pUeCb->ueCtxt.nasKsi.id = pUmtsSec->ksi;
         pUeCb->ueCtxt.mappedSecCtxt.ksi = pUmtsSec->ksi;

         pUeCb->ueCtxt.nasKsi.id = pUmtsSec->ksi;
         pUeCb->ueCtxt.secCtxt.ksi = pUmtsSec->ksi;
         pUeCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb = 0;
         pUeCb->ueCtxt.secCtxt.ulSeqNmb.seqNmb = 0;
         pUeCb->ueCtxt.secCtxt.status = VB_MME_SEC_CTXT_CRNT;



         /* UE Security Capabilities */
         vbMmeEgtUtlDecUeSecCap(pUmtsSec->ueNtwrkCapb.val,\
               pUmtsSec->ueNtwrkCapb.length, &pUeCb->ueCtxt.ueSecCap);

         /* NULL Ciphering is enabled */
         pUeCb->ueCtxt.ueSecCap.eea0 = 1;


         /*  Encryptioin Algorithm */
         pUeCb->ueCtxt.mappedSecCtxt.encAlg = (VbMmeEncAlg)pUeCb->ueCtxt.ueSecCap.eea0;

         /* Integrity Protection EIA1 is enabled */
         pUeCb->ueCtxt.ueSecCap.eia1_128 = 1;

         /*  Integrity Protection Algorithm */
         pUeCb->ueCtxt.mappedSecCtxt.intAlg = (VbMmeIntAlg)(pUeCb->ueCtxt.ueSecCap.eia1_128);



         /*CK & IK */
         cmMemcpy(pUeCb->ueCtxt.mappedSecCtxt.ck,\
               pUmtsSec->ck.val, pUmtsSec->ck.length);
         cmMemcpy(pUeCb->ueCtxt.mappedSecCtxt.ik,\
               pUmtsSec->ik.val, pUmtsSec->ik.length);


         /* Number of  Quintuplets */
         pUeCb->ueCtxt.numOfQuin = pUmtsSec->nmbQuin;

         /* Auth Quintuplet */
         for( idx = 0; idx < pUmtsSec->nmbQuin; idx++ )
         {
            cmMemcpy(pUeCb->ueCtxt.authVec.autn,\
                  pUmtsSec->authQuin[idx].autn.val,\
                  pUmtsSec->authQuin[idx].autn.length);
            cmMemcpy(pUeCb->ueCtxt.ueHssCtxt.ueSecInfo.ck,\
                  pUmtsSec->authQuin[idx].ck.val,\
                  pUmtsSec->authQuin[idx].ck.length);
            cmMemcpy(pUeCb->ueCtxt.ueHssCtxt.ueSecInfo.ik,\
                  pUmtsSec->authQuin[idx].ck.val,\
                  pUmtsSec->authQuin[idx].ik.length);
            cmMemcpy(pUeCb->ueCtxt.authVec.rndNmb,\
                  pUmtsSec->authQuin[idx].rand.val,\
                  pUmtsSec->authQuin[idx].rand.length);
            cmMemcpy(pUeCb->ueCtxt.authVec.xres,\
                  pUmtsSec->authQuin[idx].xres.val,\
                  pUmtsSec->authQuin[idx].xres.length);
         }
#endif /*VB_MME_AUTH*/

         /* subscribed UE AMBR */
         pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.dl =   pUmtsSec->dnSubsUEAMBR;
         pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.ul =  pUmtsSec->upSubsUEAMBR;

         /* UE network capability */
         vbMmeEgtUtlDecUeSecCap(pUmtsSec->ueNtwrkCapb.val,\
               pUmtsSec->ueNtwrkCapb.length, &pUeCb->ueCtxt.ueSecCap);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving UE MM Context"));
      }
   }

   /* E-UTRAN Transparent F-Container */
   if (ret == ROK)
   {
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_F_CONTAINER_IETYPE, 0);
      if (ROK == ret)
      {
         pUeCb->pHoData->srcToTgtTCont.len = pEgIe->t.fContainer.fContainer.\
               length;
         VB_MME_ALLOC(&pUeCb->pHoData->srcToTgtTCont.pVal, pUeCb->pHoData->\
               srcToTgtTCont.len);
         if (NULL != pUeCb->pHoData->srcToTgtTCont.pVal)
         {
            cmMemcpy(pUeCb->pHoData->srcToTgtTCont.pVal, pEgIe->t.fContainer.\
                  fContainer.val, pUeCb->pHoData->srcToTgtTCont.len);
         }
         else
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: UTRAN "\
                  "Transparent Container"));
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving E-UTRAN Transparent"
               " F-Container"));
      }
   }

   /* Target eNB ID */
   if (ret == ROK)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target eNB ID"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_TARGET_ID_IETYPE, 0);
      if (ROK == ret)
      {
         pUeCb->pHoData->hoTgt.gEnbId.enbId.val.homeENB_ID.len = \
               pEgIe->t.targetId.targetId.length;
         cmMemcpy(pUeCb->pHoData->hoTgt.gEnbId.enbId.val.homeENB_ID.val,\
               pEgIe->t.targetId.targetId.val,
               pEgIe->t.targetId.targetId.length);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target eNB ID"));
      }
   }

   /* RANAP CAUSE */
   if (ret == ROK)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "RANAP Cause"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_F_CAUSE_IETYPE, 1);
      if (ROK == ret)
      {
         if( pEgIe->t.fCause.causeType == CAUSE_RADIONW )
         {
            pUeCb->pHoData->hoCause.causeType = pEgIe->t.fCause.causeType;
         }
         if( pEgIe->t.fCause.cause == VB_RANAP_CAUSE_RELOC_DESR_RADIO_RSN )
         {
            pUeCb->pHoData->hoCause.cause =
               SztCauseRadioNwhandover_desirable_for_radio_reasonEnum;
         }
         /* even though S-SGSN gives different cause MME overwrites the casue
                    to HANDOVER desirabled for Radio resources */
         pUeCb->pHoData->hoCause.causeType = pEgIe->t.fCause.causeType;
         pUeCb->pHoData->hoCause.cause =
               SztCauseRadioNwhandover_desirable_for_radio_reasonEnum;
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting RANAP cause"));
      }
   }

   if (ROK != ret && NULLP != pEsmCb)
   {
      VB_MME_FREE(pEsmCb->pdnCb, sizeof(VbMmePdnCb));
      pEsmCb->pdnCb = NULLP;
      vbMmeUtlDelEsmCb(pEsmCb, pUeCb);
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Response message
 *    and stores relavant parameters received in the message.
 *    Currently the function validates only one EPSID in Bearer context.
 *
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] ppFwdLocRsp
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
)
{
   S16               ret    = ROK;
   EgIe              *pEgIe = NULLP;
   VbMmeUeTunnelCb   *pS3Tun = NULLP;
   VbMmeEsmCb        *pEsmCb  = NULLP;

   VB_TRC2(vbMmeUtlValNStoreS3FRRsp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Forward "\
         "Relocation Response"));
   ret = vbMmeEgtUtlGetIe(pFwdRelocRsp, &pEgIe, EG_CAUSE_IETYPE, 0);
   if (ROK == ret && pEgIe != NULLP)
   {
      if (pEgIe->t.cause.causeVal != EGT_GTPC_CAUSE_ACC_REQ)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Forward Relocation Response "\
               "with failure cause received"));
         ret = RFAILED;
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving Cause IE"));
      ret = RFAILED;
   }

   /* Remote S3 FTEID */
   if (ROK == ret)
   {
      pS3Tun = pUeCb->pUeS3TunCb;
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving FTEID IE"));
      ret = vbMmeEgtUtlGetIe(pFwdRelocRsp, &pEgIe, EG_FTEID_IETYPE, 0);
      if (ROK == ret && pEgIe != NULLP)
      {
         pS3Tun->remoteTeId = pEgIe->t.fTeid.teid;
         pS3Tun->remoteTeId = pEgIe->t.fTeid.teid;
         pS3Tun->remAddr.type = CM_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pS3Tun->remAddr.u.ipv4TptAddr.address));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving FTEID IE"));
         ret = RFAILED;
      }
   }

   /* EPS BearerID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving EPS Bearer ID IE "\
            "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(pFwdRelocRsp, 1, 1, EG_EBI_IETYPE, 0,\
            &pEgIe);
      if (ROK == ret && pEgIe != NULLP)
      {
         if(pUeCb->esmList[0]->bId == pEgIe->t.valUnSgnd8)
         {
            pEsmCb = pUeCb->esmList[0];
         }
         else
         {
            ret = RFAILED;
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Invalid EPS Bearer ID "\
               "in Bearer Context IE"));
         }
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving EPS Bearer ID IE "\
               "in Bearer Context IE"));
      }
   }


   /* RNC DL GTP F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving RNC DL FTEID IE "\
            "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(pFwdRelocRsp, 1, 1, EG_FTEID_IETYPE, 3,\
            &pEgIe);
      if (ROK == ret && pEgIe != NULLP)
      {
         pEsmCb->rncDlTeId = pEgIe->t.fTeid.teid;
         pEsmCb->rncIpAddr.type = CM_INET_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pEsmCb->rncIpAddr.u.ipv4NetAddr));
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving RNC DL FTEID IE "\
               "in Bearer Context IE"));
      }
   }

   /* Target to source transparent container */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving Target to Srouce "\
            "transparent container IE"));
      ret = vbMmeEgtUtlGetIe(pFwdRelocRsp, &pEgIe, EG_F_CONTAINER_IETYPE,\
                1);
      if (ROK == ret && pEgIe != NULLP)
      {
         pUeCb->pHoData->tgtToSrcTCont.len = pEgIe->t.fContainer.fContainer.\
               length;
         VB_MME_ALLOC(&pUeCb->pHoData->tgtToSrcTCont.pVal, pUeCb->pHoData->\
               tgtToSrcTCont.len);
         if (NULL != pUeCb->pHoData->tgtToSrcTCont.pVal)
         {
            cmMemcpy(pUeCb->pHoData->tgtToSrcTCont.pVal, pEgIe->t.fContainer.\
                  fContainer.val, pUeCb->pHoData->tgtToSrcTCont.len);
         }
         else
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: UTRAN "\
                  "Transparent Container"));
         }
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving UTRAN Transparent"
               " F-Container"));
      }
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Creates Forward Relocation Complete Ack message.
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
 *    SUCCESS if Forward Relocation Complete Ack is Created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldS3FRCmplAck
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppEgMsg
)
{
   S16         ret     = ROK;
   Mem         memInfo = {0, 0};
   EgIe        egIe;

   VB_TRC2(vbMmeUtlBldS3FRCmplAck);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Relocation "
         "Complete Acknowledge"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if (egUtlAllocEgMsg(ppEgMsg, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pUeCb->pUeS3TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_COMP_ACK, pUeCb->pUeS3TunCb->seqNum);

   /* Cause */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Cause IE"));
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   ret = vbMmeEgtUtlAppendIe(*ppEgMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Cause IE"));
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief
 *    Validates Forward Relocation Complete Acknowledge message
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
)
{
   S16               ret      = RFAILED;
   EgIe              *pEgIe   = NULLP;

   VB_TRC2(vbMmeUtlValNStoreFRCmplAck);

   ret = vbMmeEgtUtlGetIe(pFwdRelocCmplNotif, &pEgIe, EG_CAUSE_IETYPE, 0);
   if (ret == ROK && pEgIe != NULLP)
   {
      if (pEgIe->t.cause.causeVal == EGT_GTPC_CAUSE_ACC_REQ)
      {
         ret = ROK;
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Forware Relocation Complete"
               "Acknowledge received with Failure Cause"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving Cause IE"));
   }

   RETVALUE(ret);
}


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
)
{
   S16               ret      = RFAILED;

   VB_TRC2(vbMmeUtlFndS3Tun);

   ret = cmHashListFind(&(vbMmeCb.egtS3SapCb->ueTunnelCbHlCp),\
         (U8 *)&(teid), sizeof(Teid), 0, (PTR *)ppS3Tun);
   if (ROK != ret)
   {
      ret = cmHashListFind(&(vbMmeCb.egtS3SapCb->tunnPendCbHlCp),
            (U8 *)&(transId), sizeof(transId), 0, (PTR *)ppS3Tun);
   }

   RETVALUE(ret);
}

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
)
{
   S16         ret = ROK;
   EgIe        *pEgIe = NULLP;

   VB_TRC2(vbMmeUtlGetS3TgtEnbId);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target eNB ID"));
   ret = vbMmeEgtUtlGetIe(pEgMsg, &pEgIe, EG_TARGET_ID_IETYPE, 0);
   if (ROK == ret)
   {
      pEnbId->enbId.val.homeENB_ID.len = VB_HOME_ENB_ID_BITSTR_LEN;
      cmMemcpy(pEnbId->plmnId.plmnId,\
            (pEgIe->t.targetId.targetId.val),3);
      cmMemcpy(pEnbId->enbId.val.homeENB_ID.val,\
            (pEgIe->t.targetId.targetId.val+3),
            pEgIe->t.targetId.targetId.length);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target eNB ID"));
   }

   RETVALUE(ret);
} /* vbMmeUtlGetTgtEnbId */


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
PUBLIC S16 vbMmeEgtUtlAddUeS3MmCtx
(
EgMsg          *pEgMsg,
VbMmeUeCb      *pUeCb
)
{
   S16               ret = ROK;
   EgIe              egIe;
   EgUMTSKeyQt       *pUmtsSec = NULLP;
   EgIeLevelInfo     ieLvlInfo;

   VB_TRC2(vbMmeEgtUtlAddUeS3MmCtx);

   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_UMTS_KEY_QUIN_IETYPE, 0);
   egUtlEgtBuildEgIeHeader(&egIe, EG_UMTS_KEY_QUIN_IETYPE, 0,\
         EG_UMTS_KEY_QUIN);

   pUmtsSec                 = &egIe.t.umtsKeyQt;
   pUmtsSec->secMode        = EGT_SEC_MODE_UMTSKEY_QUINTUP; /* EPC Security */
   pUmtsSec->isDRXIPres     = FALSE;
   pUmtsSec->ksi            = pUeCb->ueCtxt.nasKsi.id;
   pUmtsSec->nmbQuin        = 0;
   pUmtsSec->isSAMBRIPres   = FALSE;
   pUmtsSec->isUAMBRIPres   = FALSE;

   /* To do: Generate CK'  ,IK' and KSI */
   vbMmeGenerateMappedCKIK(&pUeCb->ueCtxt.secCtxt,&pUeCb->ueCtxt.mappedSecCtxt);

   pUmtsSec->ck.length = CM_EMM_MAX_CK;
   cmMemcpy(pUmtsSec->ck.val, pUeCb->ueCtxt.mappedSecCtxt.ck, CM_EMM_MAX_CK);
   pUmtsSec->ik.length = CM_EMM_MAX_IK;
   cmMemcpy(pUmtsSec->ik.val, pUeCb->ueCtxt.mappedSecCtxt.ik, CM_EMM_MAX_IK);

   vbMmeEgtUtlEncUeSecCap(&pUeCb->ueCtxt.ueSecCap, \
         pUmtsSec->ueNtwrkCapb.val, &pUmtsSec->ueNtwrkCapb.length);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, &egIe);

   RETVALUE(ret);
} /* vbMmeEgtUtlAddUeMmCtx */
#endif /* VB_MME_AUTH */



#ifdef __cplusplus
}
#endif

/******************************************************************************

                                 END OF FILE

******************************************************************************/
