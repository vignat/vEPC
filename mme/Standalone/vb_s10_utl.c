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

     Name:    vb_s10_utl.c

     Desc:    Trillium LTE CNE - S10 Interface Utility functions

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
#include "vb_s10_utl.x"
#include "vb_gtp_utl.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */


/**************************************************************************//**
 *
 * @brief
 *    Returns new transaction ID on S10 interface
 *
 * @return
 *    New transaction ID
 *
 *****************************************************************************/
PUBLIC U32 vbMmeUtlGetS10TransId()
{
   return ++vbMmeCb.s10TransIdCntr;
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
PUBLIC S16 vbMmeUtlBldFRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppFwdLocReq
)
{
   S16               ret        = ROK;
   Mem               memInfo    = {0, 0};
   EgMsg             *pEgMsg    = NULLP;
   EgIeLevelInfo     egLvlInfo  = {0};
   VbMmeUeTunnelCb   *pS10Tun   = NULLP;
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
   pS10Tun        = pUeCb->pUeS10TunCb;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&pEgMsg->msgHdr, pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_REQ, pS10Tun->transId.val);

   /* imsi */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding IMSI"));
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   /*KWORKS Fix*/
   if(pUeCb->ueCtxt.ueImsiLen <= VB_HSS_IMSI_LEN)
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
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Faild to Add IMSI"));
      cmFreeMem((Ptr)ppFwdLocReq);
      RETVALUE(RFAILED);
   }

   /* sender f-teid, c-plane */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Sender C-Plane F-TEID"));
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      vbUtlIpv4InetToArr(ip4Addr, &(pS10Tun->locAddr.u.ipv4TptAddr.address));
      ret = egUtlEgtAddFteid(pEgMsg, &egLvlInfo, pS10Tun->localTeIdKey,\
            ip4Addr, EGT_INT_S10_MME_GTP_C, 0);
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
   /* UE MM Context */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE MM Context"));
      ret = vbMmeEgtUtlAddUeMmCtx(pEgMsg, pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding UE MM Context"));
      }
   }
#endif /* VB_MME_AUTH */

   /* E-UTRAN Transparent Container */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding E-UTRAN Transparent "\
            "Container"));
      ret = vbMmeEgtUtlAddEUTransCont(pEgMsg, &pUeCb->pHoData->srcToTgtTCont);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding E-UTRAN Transparent "\
               "Container"));
      }
   }

   /* Target (target eNB id) */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Target ID"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.t.targetId.targetType = 3; /* Home eNB ID type */
      egIe.t.targetId.targetId.length = 8;
      cmMemcpy(egIe.t.targetId.targetId.val, pUeCb->pHoData->hoTgt.gEnbId.\
            enbId.val.homeENB_ID.val, egIe.t.targetId.targetId.length);
      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_TARGET_ID_IETYPE, 0,\
            EG_TRGTID, &egIe);
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
PUBLIC S16 vbMmeUtlBldFRRsp
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
   VbMmeUeTunnelCb   *pS10Tun   = NULLP;
   EgIe              egIe;

   VB_TRC2(vbMmeUtlBldFRRsp);

   pS10Tun        = pUeCb->pUeS10TunCb;
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
   egUtlBuildEgHeader(&pEgMsg->msgHdr, pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_RSP, pUeCb->pUeS10TunCb->seqNum);

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
      egIe.t.fTeid.intfType = EGT_INT_S10_MME_GTP_C;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.teid = pS10Tun->localTeIdKey;
      vbUtlIpv4InetToArr(egIe.t.fTeid.ip4Addr,\
            &(pS10Tun->locAddr.u.ipv4TptAddr.address));
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
      ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_BEARER_CNTX_IETYPE, 0,\
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
         ret = vbMmeEgtUtlAddBCtxIe(pEgMsg, EG_EBI_IETYPE, 0,\
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
         ret = vbMmeEgtUtlAddBCtxIe(pEgMsg, EG_FTEID_IETYPE, 0,\
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
         ret = vbMmeEgtUtlAddBCtxIe(pEgMsg, EG_FTEID_IETYPE, 1,\
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
PUBLIC S16 vbMmeUtlBldFRCmplNotif
(
VbMmeUeCb      *pUeCb,
EgMsg          **ppEgMsg
)
{
   S16         ret     = ROK;
   Mem         memInfo = {0, 0};
   EgIe        egIe;
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
   seqNum = ++pUeCb->pUeS10TunCb->seqNum;
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_COMP_NOTFN, seqNum);

   /* Indication */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Indication IE"));
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   ret = vbMmeEgtUtlAppendIe(*ppEgMsg, EG_INDICATION_IETYPE, 0,\
         EG_INDICATION, &egIe);
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Indication IE"));
   }

   RETVALUE(ret);
}

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
)
{
   S16                  ret = ROK;
   VbMmeUeTunnelCb      *pCTun = NULLP;

   VB_MME_TRC2(vbMmeUtlCreateS10Tun);

   VB_MME_ALLOC(&pCTun, sizeof(VbMmeUeTunnelCb));
   if (pCTun == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation VbMmeUeTunnelCb"));
      RETVALUE(RFAILED);
   }

   pCTun->localTeIdKey = 0;
   pCTun->transId.pres = PRSNT_NODEF;
   pCTun->transId.val = transId;
   pCTun->locAddr = vbMmeCb.mmeCfg.s10selfAddr;
   pCTun->remAddr = *pTgtMmeIpAddr;

   ret = cmHashListInsert(&(vbMmeCb.egtS10SapCb->tunnPendCbHlCp),\
         (PTR)pCTun, (U8*)&(pCTun->transId.val), sizeof(pCTun->transId.val));
   if (ret != ROK)
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding S10 C-Tunnel to Hash Table"));
      VB_MME_FREE(pCTun, sizeof(VbMmeUeTunnelCb));
   }
   else
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Added S10 C-Tunnel to Hash Table "\
            "Transaction ID [%d]", pCTun->transId.val));
#else
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Added S10 C-Tunnel to Hash Table "\
            "Transaction ID [%ld]", pCTun->transId.val));
#endif

      pCTun->ueCb = pUeCb;
      pUeCb->pUeS10TunCb = pCTun;
   }

   RETVALUE(ret);
}


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
)
{
   S16                  ret = ROK;
   VbMmeUeTunnelCb      *pS10Tun = NULLP;

   VB_MME_TRC2(vbMmeUtlDelS10Tun);

   if (NULL != pUeCb->pUeS10TunCb)
   {
      pS10Tun = pUeCb->pUeS10TunCb;
      VbLiEgtLclDelReq(&(vbMmeCb.egtS10SapCb->pst), vbMmeCb.egtS10SapCb->spId,\
            0, pS10Tun->localTeIdKey);

      cmHashListDelete(&(vbMmeCb.egtS10SapCb->ueTunnelCbHlCp), (PTR)pS10Tun);
      VB_MME_FREE(pS10Tun, sizeof(VbMmeUeTunnelCb));

      pUeCb->pUeS10TunCb = NULLP;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Parameter, "\
            "S10 Tunnel CB is NULL"));
      ret = RFAILED;
   }

   RETVALUE(ret);
}


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
)
{
   S16        ret = ROK;

   VB_TRC2(vbMmeUtlCreateUeCb);

   VB_MME_ALLOC(ppUeCb, sizeof(VbMmeUeCb));
   if (NULLP == *ppUeCb)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Allocating memory: VbMmeUeCb"));
      RETVALUE(RFAILED);
   }

   (*ppUeCb)->emmCb.ueCb = *ppUeCb;

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
PUBLIC S16 vbMmeUtlValNStoreFRReq
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFRReq
)
{
   S16               ret      = ROK;
   EgIe              *pEgIe   = NULLP;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;
   VbMmeEsmCb        *pEsmCb  = NULLP;
   VbMmePdnCb        *pPdnCb  = NULLP;
   EgESecCxtQdQt     *pEpsSec = NULLP;

   VB_TRC2(vbMmeUtlValNStoreFRReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Forward "\
         "Relocation Request"));

   VB_MME_ALLOC(&pUeCb->pHoData, sizeof(VbMmeHoData));
   if (NULLP == pUeCb->pHoData)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation: VbMmeHoData"));
      RETVALUE(RFAILED);
   }

   pS10Tun = pUeCb->pUeS10TunCb;
   pS10Tun->seqNum = pFRReq->msgHdr.seqNumber;

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
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Sender F-TEID (S10)"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_FTEID_IETYPE, 0);
      if (ROK == ret)
      {
         pS10Tun->remoteTeId = pEgIe->t.fTeid.teid;
         pS10Tun->remAddr.type = CM_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pS10Tun->remAddr.u.ipv4TptAddr.address));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Sender F-TEID (S10)"));
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
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving IP Address within "\
               "PDN Connection IE"));
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
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving Bearer QoS IE "\
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

   /* UE MM Context */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving UE MM Context"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_EPS_SEC_QUAD_QUIN_IETYPE, 0);
      if (ROK == ret)
      {
#ifdef VB_MME_AUTH
         pEpsSec                 = &pEgIe->t.eSecCxtQdQt;

         pUeCb->ueCtxt.nasKsi.id = pEpsSec->ksi;
         pUeCb->ueCtxt.secCtxt.ksi = pEpsSec->ksi;
         pUeCb->ueCtxt.secCtxt.intAlg = (VbMmeIntAlg)pEpsSec->nasProtAlgo;
         pUeCb->ueCtxt.secCtxt.encAlg = (VbMmeEncAlg)pEpsSec->usedNasCipher;
         pUeCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb = pEpsSec->nasDlCount;
         pUeCb->ueCtxt.secCtxt.ulSeqNmb.seqNmb = pEpsSec->nasUlCount;
         pUeCb->ueCtxt.secCtxt.status = VB_MME_SEC_CTXT_CRNT;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "UL Nas Count [%d]",\
               pUeCb->ueCtxt.secCtxt.ulSeqNmb.seqNmb));
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "DL Nas Count [%d]",\
               pUeCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb));

         cmMemcpy(pUeCb->ueCtxt.authVec.kasme, pEpsSec->kasme.val,\
               VB_KASME_SIZE);
         cmMemcpy(pUeCb->ueCtxt.secCtxt.kasme, pEpsSec->kasme.val,\
               VB_KASME_SIZE);
         VB_MME_SEC_PRNT_HEX("Kasme: ", pUeCb->ueCtxt.authVec.kasme,\
               VB_KASME_SIZE);

         /* Auth Quadruplet */
         cmMemcpy(pUeCb->ueCtxt.authVec.rndNmb, pEpsSec->authQuadru[0].\
               rand.val, pEpsSec->authQuadru[0].rand.length);
         cmMemcpy(pUeCb->ueCtxt.authVec.xres, pEpsSec->authQuadru[0].xres.val,\
               pEpsSec->authQuadru[0].xres.length);
         cmMemcpy(pUeCb->ueCtxt.authVec.autn, pEpsSec->authQuadru[0].autn.val,\
               pEpsSec->authQuadru[0].autn.length);

         /* Auth Quintuplet */
         cmMemcpy(pUeCb->ueCtxt.authVec.autn, pEpsSec->authQuin[0].autn.val,\
               pEpsSec->authQuin[0].autn.length);
         cmMemcpy(pUeCb->ueCtxt.ueHssCtxt.ueSecInfo.ck,\
               pEpsSec->authQuin[0].ck.val, pEpsSec->authQuin[0].ck.length);
         cmMemcpy(pUeCb->ueCtxt.ueHssCtxt.ueSecInfo.ik,\
               pEpsSec->authQuin[0].ck.val, pEpsSec->authQuin[0].ik.length);
         cmMemcpy(pUeCb->ueCtxt.authVec.rndNmb, pEpsSec->authQuin[0].rand.val,\
               pEpsSec->authQuin[0].rand.length);
         cmMemcpy(pUeCb->ueCtxt.authVec.xres, pEpsSec->authQuin[0].xres.val,\
               pEpsSec->authQuin[0].xres.length);

         /* Next Hop and NCC */
         cmMemcpy(pUeCb->ueCtxt.secCtxt.nxtHop, pEpsSec->nxtHop,\
               VB_NEXT_HOP_SIZE);
         VB_MME_SEC_PRNT_HEX("Next Hop: ", pUeCb->ueCtxt.secCtxt.nxtHop,\
               VB_NEXT_HOP_SIZE);

         pUeCb->ueCtxt.secCtxt.nxtHopChainCnt = pEpsSec->ncc;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "NCC [%d]",\
               pUeCb->ueCtxt.secCtxt.nxtHopChainCnt));
#endif
         /* subscribed UE AMBR */
         pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.dl =   pEpsSec->dnSubsUEAMBR;
         pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.ul =  pEpsSec->upSubsUEAMBR;

         /* UE network capability */
         vbMmeEgtUtlDecUeSecCap(pEpsSec->ueNtwrkCapb.val,\
               pEpsSec->ueNtwrkCapb.length, &pUeCb->ueCtxt.ueSecCap);
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
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: E-UTRAN "\
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

   /* RAN CAUSE */
   if (ret == ROK)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting RAN Cause for HO"));
      ret = vbMmeEgtUtlGetIe(pFRReq, &pEgIe, EG_F_CAUSE_IETYPE, 0);
      if (ROK == ret)
      {
         pUeCb->pHoData->hoCause.causeType = pEgIe->t.fCause.causeType;
         pUeCb->pHoData->hoCause.cause = pEgIe->t.fCause.cause;
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting RAN cause for HO"));
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
 *    and stores relavant parameters received in the message
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
PUBLIC S16 vbMmeUtlValNStoreFRRsp
(
VbMmeUeCb      *pUeCb,
EgMsg          *pFwdRelocRsp
)
{
   S16               ret    = ROK;
   EgIe              *pEgIe = NULLP;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;

   VB_TRC2(vbMmeUtlValNStoreFRRsp);

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

   /* Remote S10 FTEID */
   if (ROK == ret)
   {
      pS10Tun = pUeCb->pUeS10TunCb;
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving FTEID IE"));
      ret = vbMmeEgtUtlGetIe(pFwdRelocRsp, &pEgIe, EG_FTEID_IETYPE, 0);
      if (ROK == ret && pEgIe != NULLP)
      {
         pS10Tun->remoteTeId = pEgIe->t.fTeid.teid;
         pS10Tun->remoteTeId = pEgIe->t.fTeid.teid;
         pS10Tun->remAddr.type = CM_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pS10Tun->remAddr.u.ipv4TptAddr.address));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving FTEID IE"));
         ret = RFAILED;
      }
   }

   /* eNB DL GTP F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB DL FTEID IE "\
            "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(pFwdRelocRsp, 0, 1, EG_FTEID_IETYPE, 0,\
            &pEgIe);
      if (ROK == ret && pEgIe != NULLP)
      {
         pUeCb->pHoData->dlGtpTeid = pEgIe->t.fTeid.teid;
         pUeCb->pHoData->dlTransportLyrAddr.type = CM_INET_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pUeCb->pHoData->dlTransportLyrAddr.u.ipv4NetAddr));
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB DL FTEID IE "\
               "in Bearer Context IE"));
      }
   }

   /* eNB UL GTP F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB UL FTEID IE "\
            "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(pFwdRelocRsp, 0, 1, EG_FTEID_IETYPE, 1,\
            &pEgIe);
      if (ROK == ret && pEgIe != NULLP)
      {
         pUeCb->pHoData->ulGtpTeid = pEgIe->t.fTeid.teid;
         pUeCb->pHoData->ulTransportLyrAddr.type = CM_INET_IPV4ADDR_TYPE;
         vbMmeEgtUtlIpv4ArrToInet(pEgIe->t.fTeid.ip4Addr,
               &(pUeCb->pHoData->ulTransportLyrAddr.u.ipv4NetAddr));
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving FTEID IE"));
         ret = RFAILED;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB UL FTEID IE "\
               "in Bearer Context IE"));
      }
   }

   /* Target to source transparent container */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving Target to Srouce "\
            "transparent container IE"));
      ret = vbMmeEgtUtlGetIe(pFwdRelocRsp, &pEgIe, EG_F_CONTAINER_IETYPE,\
                0);
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
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: E-UTRAN "\
                  "Transparent Container"));
         }
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving E-UTRAN Transparent"
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
PUBLIC S16 vbMmeUtlBldFRCmplAck
(
VbMmeUeCb      *pUeCb,
U32            cause,
EgMsg          **ppEgMsg
)
{
   S16         ret     = ROK;
   Mem         memInfo = {0, 0};
   EgIe        egIe;

   VB_TRC2(vbMmeUtlBldFRCmplAck);
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
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_FR_COMP_ACK, pUeCb->pUeS10TunCb->seqNum);

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
)
{
   S16         ret     = ROK;
   Mem         memInfo = {0, 0};
   EgIe        egIe;

   VB_TRC2(vbMmeUtlBldFACtxAck);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Access "
         "Context Acknowledge"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if (egUtlAllocEgMsg(ppEgMsg, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK, pUeCb->pUeS10TunCb->seqNum);

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
PUBLIC S16 vbMmeUtlValNStoreFRCmplAck
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
)
{
   S16               ret      = RFAILED;
   EgIe              *pEgIe   = NULLP;

   VB_TRC2(vbMmeUtlValNStoreFACtxNotif);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving F Container IE"));
   ret = vbMmeEgtUtlGetIe(pEgMsg, &pEgIe, EG_F_CONTAINER_IETYPE, 0);
   if (ret == ROK && pEgIe != NULLP)
   {
      pUeCb->pHoData->staTfrTCont.len = pEgIe->t.fContainer.fContainer.length;
      VB_MME_ALLOC(&pUeCb->pHoData->staTfrTCont.pVal, pUeCb->pHoData->\
               staTfrTCont.len);
      if (NULLP != pUeCb->pHoData->staTfrTCont.pVal)
      {
         cmMemcpy(pUeCb->pHoData->staTfrTCont.pVal,\
               pEgIe->t.fContainer.fContainer.val,
               pUeCb->pHoData->staTfrTCont.len);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: "\
               "VbMmeEUTransCont"));
      }
   }
   else
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving F Container IE"));
   }

   RETVALUE(ret);
}

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
)
{
   S16               ret      = RFAILED;

   VB_TRC2(vbMmeUtlFndS10Tun);

   ret = cmHashListFind(&(vbMmeCb.egtS10SapCb->ueTunnelCbHlCp),\
         (U8 *)&(teid), sizeof(Teid), 0, (PTR *)ppS10Tun);
   if (ROK != ret)
   {
      ret = cmHashListFind(&(vbMmeCb.egtS10SapCb->tunnPendCbHlCp),
            (U8 *)&(transId), sizeof(transId), 0, (PTR *)ppS10Tun);
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
PUBLIC S16 vbMmeUtlGetTgtEnbId
(
EgMsg          *pEgMsg,
VbMmeGlbEnbId  *pEnbId
)
{
   S16         ret = ROK;
   EgIe        *pEgIe = NULLP;

   VB_TRC2(vbMmeUtlGetTgtEnbId);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target eNB ID"));
   ret = vbMmeEgtUtlGetIe(pEgMsg, &pEgIe, EG_TARGET_ID_IETYPE, 0);
   if (ROK == ret)
   {
      pEnbId->enbId.val.homeENB_ID.len = VB_HOME_ENB_ID_BITSTR_LEN;
      cmMemcpy(pEnbId->enbId.val.homeENB_ID.val,\
            pEgIe->t.targetId.targetId.val,
            pEgIe->t.targetId.targetId.length);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target eNB ID"));
   }

   RETVALUE(ret);
} /* vbMmeUtlGetTgtEnbId */


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
)
{
   S16         ret     = ROK;
   Mem         memInfo = {0, 0};

   VB_TRC2(vbMmeUtlBldFACtxNotif);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if (egUtlAllocEgMsg(ppEgMsg, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   pUeCb->pUeS10TunCb->seqNum += 1;
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN, pUeCb->pUeS10TunCb->seqNum);

   /* E-UTRAN Transparent Container */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding E-UTRAN Transparent "\
         "Container"));
   ret = vbMmeEgtUtlAddEUTransCont(*ppEgMsg, &pUeCb->pHoData->staTfrTCont);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding E-UTRAN Transparent "\
            "Container"));
   }

   RETVALUE(ret);
}


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
)
{
   S16               ret        = ROK;
   Mem               memInfo    = {0, 0};
   EgMsg             *pEgMsg    = NULLP;
   EgIeLevelInfo     egLvlInfo  = {0};
   VbMmeUeTunnelCb   *pS10Tun   = NULLP;

   VB_TRC2(vbMmeUtlBldRCReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Relocation Cancel Request"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   if (egUtlAllocEgMsg(ppRelCanReq, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   pEgMsg         = *ppRelCanReq;
   pS10Tun        = pUeCb->pUeS10TunCb;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&pEgMsg->msgHdr, pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_RC_REQ, pS10Tun->transId.val);

   /* imsi */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding IMSI"));
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   /*KWORKS Fix*/
   if(pUeCb->ueCtxt.ueImsiLen <= VB_HSS_IMSI_LEN)
   {
   ret = egUtlEgtAddImsi(pEgMsg, &egLvlInfo, pUeCb->ueCtxt.ueImsi,\
         pUeCb->ueCtxt.ueImsiLen, 0);
   }
   else
   {
      ret= RFAILED;
   }
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Faild to Add IMSI"));
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(&pEgMsg);
   }

   RETVALUE(ret);
}

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
PUBLIC S16 vbMmeEgtUtlAddUeMmCtx
(
EgMsg          *pEgMsg,
VbMmeUeCb      *pUeCb
)
{
   S16               ret = ROK;
   EgIe              egIe;
   EgESecCxtQdQt     *pEpsSec = NULLP;
   EgIeLevelInfo     ieLvlInfo;

   VB_TRC2(vbMmeEgtUtlAddUeMmCtx);

   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_EPS_SEC_QUAD_QUIN_IETYPE, 0);
   egUtlEgtBuildEgIeHeader(&egIe, EG_EPS_SEC_QUAD_QUIN_IETYPE, 0,\
         EG_EPS_SEC_QUAD_QUIN);

   pEpsSec                 = &egIe.t.eSecCxtQdQt;
   pEpsSec->secMode        = 4; /* EPC Security */
   pEpsSec->isNHIPres      = TRUE;
   pEpsSec->isDRXIPres     = FALSE;
   pEpsSec->ksi            = pUeCb->ueCtxt.nasKsi.id;
   pEpsSec->nmbQuin        = 1;
   pEpsSec->nmbQuadru      = 1;
   pEpsSec->isSAMBRIPres   = TRUE;
   pEpsSec->isOCSIPres     = FALSE;
   pEpsSec->isUAMBRIPres   = FALSE;
   pEpsSec->nasProtAlgo    = pUeCb->ueCtxt.secCtxt.intAlg;
   pEpsSec->usedNasCipher  = pUeCb->ueCtxt.secCtxt.encAlg;
   pEpsSec->nasDlCount     = pUeCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb;
   pEpsSec->nasUlCount     = pUeCb->ueCtxt.secCtxt.ulSeqNmb.seqNmb;
   pEpsSec->kasme.length   = VB_KASME_SIZE;
   cmMemcpy(pEpsSec->kasme.val, pUeCb->ueCtxt.authVec.kasme, VB_KASME_SIZE);

   /* Auth Quadruplet */
   pEpsSec->authQuadru[0].rand.length = VB_RAND_SIZE;
   cmMemcpy(pEpsSec->authQuadru[0].rand.val, pUeCb->ueCtxt.authVec.rndNmb,\
         VB_RAND_SIZE);
   pEpsSec->authQuadru[0].xresLen = VB_RES_SIZE;
   pEpsSec->authQuadru[0].xres.length = VB_RES_SIZE;
   cmMemcpy(pEpsSec->authQuadru[0].xres.val, pUeCb->ueCtxt.authVec.xres,\
         VB_RES_SIZE);
   pEpsSec->authQuadru[0].autnLen = VB_AUTN_SIZE;
   pEpsSec->authQuadru[0].autn.length = VB_AUTN_SIZE;
   cmMemcpy(pEpsSec->authQuadru[0].autn.val, pUeCb->ueCtxt.authVec.autn,\
         VB_AUTN_SIZE);
   pEpsSec->authQuadru[0].kasme.length = VB_KASME_SIZE;
   cmMemcpy(pEpsSec->authQuadru[0].kasme.val, pUeCb->ueCtxt.authVec.kasme,\
         VB_KASME_SIZE);

   /* Auth Quintuplet */
   pEpsSec->authQuin[0].autnLen = VB_AUTN_SIZE;
   pEpsSec->authQuin[0].autn.length = VB_AUTN_SIZE;
   cmMemcpy(pEpsSec->authQuin[0].autn.val, pUeCb->ueCtxt.authVec.autn,\
         VB_AUTN_SIZE);
   pEpsSec->authQuin[0].ck.length = CM_EMM_MAX_CK;
   cmMemcpy(pEpsSec->authQuin[0].ck.val, pUeCb->ueCtxt.ueHssCtxt.ueSecInfo.ck,\
         CM_EMM_MAX_CK);
   pEpsSec->authQuin[0].ik.length = CM_EMM_MAX_IK;
   cmMemcpy(pEpsSec->authQuin[0].ck.val, pUeCb->ueCtxt.ueHssCtxt.ueSecInfo.ik,\
         CM_EMM_MAX_IK);
   pEpsSec->authQuin[0].rand.length = VB_RAND_SIZE;
   cmMemcpy(pEpsSec->authQuin[0].rand.val, pUeCb->ueCtxt.authVec.rndNmb,\
         VB_RAND_SIZE);
   pEpsSec->authQuin[0].xresLen = VB_RES_SIZE;
   pEpsSec->authQuin[0].xres.length = VB_RES_SIZE;
   cmMemcpy(pEpsSec->authQuin[0].xres.val, pUeCb->ueCtxt.authVec.xres,\
         VB_RES_SIZE);

   cmMemcpy(pEpsSec->nxtHop, pUeCb->ueCtxt.secCtxt.nxtHop, VB_NEXT_HOP_SIZE);
   pEpsSec->ncc = pUeCb->ueCtxt.secCtxt.nxtHopChainCnt;

   pEpsSec->dnSubsUEAMBR   = pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.dl;
   pEpsSec->upSubsUEAMBR   = pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.ul;
   pEpsSec->dnUsedUEAMBR   = 0;
   pEpsSec->upUsedUEAMBR   = 0;

   vbMmeEgtUtlEncUeSecCap(&pUeCb->ueCtxt.ueSecCap, \
         pEpsSec->ueNtwrkCapb.val, &pEpsSec->ueNtwrkCapb.length);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, &egIe);

   RETVALUE(ret);
} /* vbMmeEgtUtlAddUeMmCtx */
#endif /* VB_MME_AUTH */


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
)
{
   S16               ret        = ROK;
   Mem               memInfo    = {0, 0};
   EgMsg             *pEgMsg    = NULLP;
   EgIe              egIe;

   VB_TRC2(vbMmeUtlBldRCRsp);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Relocation Cancel Response"));
   if (egUtlAllocEgMsg(ppRCRsp, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   pEgMsg = *ppRCRsp;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&pEgMsg->msgHdr, pUeCb->pUeS10TunCb->remoteTeId,\
         EGT_GTPC_MSG_RC_RSP, pUeCb->pUeS10TunCb->seqNum);

   /* Cause IE */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Cause IE"));
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   ret = vbMmeEgtUtlAppendIe(pEgMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Cause IE"));
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(&pEgMsg);
   }

   RETVALUE(ret);
}


#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
/******************************************************************************

                                 END OF FILE

******************************************************************************/
