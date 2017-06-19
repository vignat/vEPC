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

     Desc:    Trillium LTE CNE - S10 Interface Handler functions

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
#include "vb_s10_hndlr.x"
#include "vb_s10_utl.x"

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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TmrCfg      t3Tmr   = {0};
   TknU32      teidTkn = {0};
   TknU8       create  = {0};

   VB_TRC2(vbMmeSndFRReq);

   /* Pass the local Tunnel ID as 0, It will be allocated by EGTP layer
    * and returned when Fwd Relocation Response message is received
    */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Creating of S10 C-Tunnel"));
   ret = vbMmeUtlCreateS10Tun(pUeCb, pTgtMmeIpAddr, vbMmeUtlGetS10TransId());
   if (ROK == ret)
   {
      ret = vbMmeUtlBldFRReq(pUeCb, &pEgMsg);
      if (ROK == ret)
      {
         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val  = pUeCb->pUeS10TunCb->localTeIdKey;
         create.pres  = PRSNT_NODEF;
         create.val   = TRUE;
         t3Tmr.enb    = TRUE;
         t3Tmr.val    = VB_MME_EGT_T3_RESP_TMR;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Request to MME"));

         VbLiEgtSigReq(&(vbMmeCb.egtS10SapCb->pst),\
               (vbMmeCb.egtS10SapCb->spId), &(pUeCb->pUeS10TunCb->locAddr),\
               &(pUeCb->pUeS10TunCb->remAddr), teidTkn,\
               (pUeCb->pUeS10TunCb->transId), create, t3Tmr, pEgMsg);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Relocation "\
               "Request"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Creation of S10 C-Tunnel, FAILURE"));
   }

   RETVALUE(ret);
}


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
)
{
   EgIe              *pEgIe = NULLP;
   Teid              remTeid = 0;
   S16               ret = ROK;
   VbMmeUeCb         *pUeCb = NULLP;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;
   VbMmeUeCtxt       ueCtxt = {0};
   VbMmeGlbEnbId     gEnbId;
   VbMmeEnbCb        *pEnbCb = NULLP;

   VB_TRC2(vbMmeIncFRReq);

   ret = vbMmeUtlFndS10Tun(locTeid.val, pEgMsg->msgHdr.seqNumber, &pS10Tun);
   if (ROK == ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "S10 tunnel with TEID [%d] Already "\
            "Present", locTeid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "S10 tunnel with TEID [%ld] Already "\
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
   ret = vbMmeUtlGetTgtEnbId(pEgMsg, &gEnbId);
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
         /* Forward relocation request is received during S1 Handover */
         pUeCb->proc = VB_MME_HANDOVER_PROC;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Creating S10 Tunnel CB"));
         ret = vbMmeUtlCreateS10Tun(pUeCb, pRemAddr, pEgMsg->msgHdr.seqNumber);
         if (ROK == ret)
         {
            ret = vbMmeUtlValNStoreFRReq(pUeCb, pEgMsg);
            if (ROK == ret)
            {
#ifdef VB_MME_AUTH
               if ((pUeCb->ueCtxt.secCtxt.encAlg != VB_MME_EEA0) || \
                   (pUeCb->ueCtxt.secCtxt.intAlg != VB_MME_EIA0))
               {
                  vbMmeGenerateNasKey(&pUeCb->ueCtxt.secCtxt);
               }
#endif
               pUeCb->pUeS10TunCb->localTeIdKey = locTeid.val;
               pUeCb->pHoData->hoMmeType = VB_MME_HO_TGT_MME;
               pUeCb->pHoData->hoType = VB_EMM_HO_INTRA_LTE;
               VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);

               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Create Session "\
                     "Request to SGW"));
               ret = vbMmeSndCSReq(pUeCb, pUeCb->esmList[0]);
               if (ROK != ret)
               {
                  VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Create Session "\
                        "Request to SGW"));
               }
            }
            else
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation and Storing of "\
                     "Forward Relocation Request"));
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Creating S10 Tunnel CB"));
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
            " Response with Failure cause to MME"));
      ret = vbMmeEgtUtlSndFailureRsp(locTeid.val, remTeid, EGT_GTPC_MSG_FR_RSP,\
            pEgMsg->msgHdr.seqNumber, EGT_GTPC_CAUSE_REQ_REJECT);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Relocation"\
               " Response with Failure cause to MME, FAILURE"));
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
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeIncFRRsp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S10 Tunnel, "
         "Transaction ID: [%ld]", transId.val));
   ret = cmHashListFind(&vbMmeCb.egtS10SapCb->tunnPendCbHlCp,\
         (U8 *)&(transId.val), sizeof(transId.val), FALSE, (PTR *)&pS10Tun);
   if (ROK == ret)
   {
      pUeCb = pS10Tun->ueCb;
      pS10Tun->localTeIdKey = locTeid.val;
      pS10Tun->seqNum = pEgMsg->msgHdr.seqNumber;

      ret = vbMmeUtlValNStoreFRRsp(pUeCb, pEgMsg);
      if (ROK == ret)
      {
         /* removing from pending tunnel cb to ueTunnelCbHlCp */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting S10 Tunnel from Pending "\
               "Tunnel Hash Table"));
         cmHashListDelete(&(vbMmeCb.egtS10SapCb->tunnPendCbHlCp),\
               (PTR)pS10Tun);

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding S10 Tunnel to "\
               "Tunnel Hash Table, Tunnel ID [%ld]", pS10Tun->localTeIdKey));
         ret = cmHashListInsert(&(vbMmeCb.egtS10SapCb->ueTunnelCbHlCp),\
               (PTR)pS10Tun, (U8*)&(pS10Tun->localTeIdKey),\
               sizeof(pS10Tun->localTeIdKey));
         if (ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validated Forward Relocation "\
                  "Response message"));

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Command "\
                  "to eNodeB"));
            ret = vbMmeSndHOCmd(pUeCb, pUeCb->eNodebCb);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Command "\
                     "to eNodeB"));
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding S10 Tunnel to "\
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
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S10 Tunnel, Teid: [%ld]",\
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
)
{
   S16               ret     = ROK;
   EgMsg             *pEgMsg = NULLP;
   TknU32            teidTkn = {0};
   VbMmeUeTunnelCb   *pS10Tun = NULLP;

   VB_TRC2(vbMmeSndFRRsp);

   pS10Tun = pUeCb->pUeS10TunCb;

   /* Remove the S10 tunnel from pending tunnel hash table to
    * ueTunnelCb hash table
    */
   cmHashListDelete(&(vbMmeCb.egtS10SapCb->tunnPendCbHlCp),\
         (PTR)pS10Tun);
   ret = cmHashListInsert(&(vbMmeCb.egtS10SapCb->ueTunnelCbHlCp),\
         (PTR)pS10Tun, (U8*)&(pS10Tun->localTeIdKey),\
         sizeof(pS10Tun->localTeIdKey));
   if (ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding S10 C-Tunnel to "\
            "Hash Table"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }

   ret = vbMmeUtlBldFRRsp(pUeCb, cause, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Response MME"));
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = pUeCb->pUeS10TunCb->localTeIdKey;

      VbLiEgtSigRsp(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), teidTkn, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Relocation "\
            "Response"));
   }

   /* Any other cause means the local tunnel has to be deleted */
   if (cause != EGT_GTPC_CAUSE_ACC_REQ)
   {
      vbMmeUtlDelS10Tun(pUeCb);
   }

   RETVALUE(ret);
}


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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TmrCfg      t3Tmr   = {0, 0};
   TknU32      teidTkn = {0};
   TknU8       create  = {0};

   VB_TRC2(vbMmeSndFRCmplNotif);

   ret = vbMmeUtlBldFRCmplNotif(pUeCb, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Relocation "\
            "Complete Notification, SUCCESS"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = pUeCb->pUeS10TunCb->localTeIdKey;
      create.pres  = PRSNT_NODEF;
      create.val   = FALSE;
      t3Tmr.enb    = TRUE;
      t3Tmr.val    = VB_MME_EGT_T3_RESP_TMR;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Complete Notification to MME"));
      VbLiEgtSigReq(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), &(pUeCb->pUeS10TunCb->locAddr),\
            &(pUeCb->pUeS10TunCb->remAddr), teidTkn,\
            (pUeCb->pUeS10TunCb->transId), create, t3Tmr, pEgMsg);
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
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeIncFRCmplNotif);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Forward Relocation Complete "\
         "Notification"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS10SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS10Tun);
   if (ROK == ret)
   {
      pS10Tun->seqNum = pEgMsg->msgHdr.seqNumber;
      pUeCb = pS10Tun->ueCb;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Complete Acknowledge to MME"));
      ret = vbMmeSndFRCmplAck(pUeCb, EGT_GTPC_CAUSE_ACC_REQ);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Complete Acknowledge to MME"));
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting the S10 Tunnel"));
         vbMmeUtlDelS10Tun(pUeCb);

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting the PDN Connection "\
               " on SGW and PGW"));
         ret = vbMmeUtlDelPdnConn(pUeCb, FALSE);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Deleting the PDN Connection "\
                  " on SGW and PGW"));
         }
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}


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
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;

   VB_TRC2(vbMmeIncFACtxNotif);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Forward Access Context "\
         "Notification"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS10SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS10Tun);
   if (ROK == ret)
   {
      ret = vbMmeUtlValNStoreFACtxNotif(pS10Tun->ueCb, pEgMsg);
      if (ROK == ret)
      {
         pS10Tun->seqNum = pEgMsg->msgHdr.seqNumber;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Foward Access Context "\
                  "Notification to source MME"));
         ret = vbMmeSndFACtxAck(pS10Tun->ueCb, EGT_GTPC_CAUSE_ACC_REQ);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Foward Access Context "\
                     "Notification to source MME"));
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending eNB Status Transfer "));
            ret = vbMmeSndMmeStatusTfr(pS10Tun->ueCb, pS10Tun->ueCb->eNodebCb);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending eNB Status "\
                     "Transfer"));
            }
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation of Foward Access "\
               "Context Notification"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}

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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TknU32      locTeid = {0};

   VB_TRC2(vbMmeSndFRCmplAck);

   ret = vbMmeUtlBldFRCmplAck(pUeCb, cause, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Complete Acknowledge to MME"));

      locTeid.pres = PRSNT_NODEF;
      locTeid.val = pUeCb->pUeS10TunCb->localTeIdKey;
      VbLiEgtSigRsp(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), locTeid, pEgMsg);
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
)
{
   S16               ret = ROK;
   VbMmeEsmCb        *pEsmCb = NULLP;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;

   VB_TRC2(vbMmeIncFRCmplAck);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Forward Relocation Complete "\
         "Ack"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS10SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS10Tun);
   if (ROK == ret)
   {
      ret = vbMmeUtlValNStoreFRCmplAck(pS10Tun->ueCb, pEgMsg);
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Modify Bearer Request "\
               "to SGW"));

         pEsmCb = pS10Tun->ueCb->esmList[0];
         ret = vbMmeSndMBReq(pS10Tun->ueCb, pEsmCb, FALSE);
         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Modify Bearer Request "\
                  "to SGW Failed"));
         }

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting the S10 Tunnel"));
         vbMmeUtlDelS10Tun(pS10Tun->ueCb);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation and Storing of "\
               "Forward Relocation Complete Acknowledge"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}



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
)
{
   S16               ret = ROK;

   VB_TRC2(vbMmeIncFRCmplAck);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Forward Access Context "\
         "Ack"));

   RETVALUE(ret);
}
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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TmrCfg      t3Tmr   = {0};
   TknU32      teidTkn = {0};
   TknU8       create  = {0};

   VB_TRC2(vbMmeSndFACtxNotif);

   ret = vbMmeUtlBldFACtxNotif(pUeCb, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Forward Access "\
            "Context Notification, SUCCESS"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = pUeCb->pUeS10TunCb->localTeIdKey;
      create.pres  = PRSNT_NODEF;
      create.val   = FALSE;
      t3Tmr.enb    = TRUE;
      t3Tmr.val    = VB_MME_EGT_T3_RESP_TMR;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Access "\
            "Context Notification to MME"));
      VbLiEgtSigReq(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), &(pUeCb->pUeS10TunCb->locAddr),\
            &(pUeCb->pUeS10TunCb->remAddr), teidTkn,\
            (pUeCb->pUeS10TunCb->transId), create, t3Tmr, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Access "\
            "Context Notification"));
   }

   RETVALUE(ret);
} /* vbMmeSndFACtxNotif */


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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TknU32      locTeid = {0};

   VB_TRC2(vbMmeSndFACtxAck);

   ret = vbMmeUtlBldFACtxAck(pUeCb, cause, &pEgMsg);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Access "\
            "Context Acknowledge to MME"));

      locTeid.pres = PRSNT_NODEF;
      locTeid.val = pUeCb->pUeS10TunCb->localTeIdKey;
      VbLiEgtSigRsp(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), locTeid, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Forward Access "\
            "Context Acknowledge"));
   }

   RETVALUE(ret);
}



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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TmrCfg      t3Tmr   = {0, 0};
   TknU32      teidTkn = {0};
   TknU8       create  = {0};

   VB_TRC2(vbMmeSndRCReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Relocation Cancel "\
         "Request "));
   ret = vbMmeUtlBldRCReq(pUeCb, &pEgMsg);
   if (ROK == ret)
   {
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = pUeCb->pUeS10TunCb->localTeIdKey;
      create.pres  = PRSNT_NODEF;
      create.val   = FALSE;
      t3Tmr.enb    = TRUE;
      t3Tmr.val    = VB_MME_EGT_T3_RESP_TMR;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Relocation Cancel"\
            "Request to MME"));
      VbLiEgtSigReq(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), &(pUeCb->pUeS10TunCb->locAddr),\
            &(pUeCb->pUeS10TunCb->remAddr), teidTkn,\
            (pUeCb->pUeS10TunCb->transId), create, t3Tmr, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Relocation Cancel "\
            "Request "));
   }

   RETVALUE(ret);
}


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
)
{
   S16         ret     = ROK;
   EgMsg       *pEgMsg = NULLP;
   TknU32      teidTkn = {0};

   VB_TRC2(vbMmeSndRCRsp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Relocation Cancel "\
         "Response"));
   ret = vbMmeUtlBldRCRsp(pUeCb, cause, &pEgMsg);
   if (ROK == ret)
   {
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = pUeCb->pUeS10TunCb->localTeIdKey;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Relocation Cancel"\
            "Response to MME"));
      VbLiEgtSigRsp(&(vbMmeCb.egtS10SapCb->pst),\
            (vbMmeCb.egtS10SapCb->spId), teidTkn, pEgMsg);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Relocation Cancel "\
            "Response"));
   }

   RETVALUE(ret);
} /* vbMmeSndRCRsp */


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
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeIncRCReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Relocation Release "\
         "Request"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS10SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS10Tun);
   if (ROK == ret)
   {
      pS10Tun->seqNum = pEgMsg->msgHdr.seqNumber;
      pUeCb = pS10Tun->ueCb;
      VB_MME_SET_MASK(pUeCb->pHoData->bitmask, VB_MME_RELOC_CANCEL_RCVD);

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting the PDN Connection "\
            " on SGW"));
      ret = vbMmeUtlDelPdnConn(pUeCb, FALSE);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Deleting the PDN Connection "\
               " on SGW"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}


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
)
{
   S16               ret = ROK;
   VbMmeUeTunnelCb   *pS10Tun = NULLP;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeIncRCRsp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Relocation Cancel "\
         "Response"));

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
         locTeid.val));
   ret = cmHashListFind(&vbMmeCb.egtS10SapCb->ueTunnelCbHlCp,\
         (U8 *)&(locTeid.val), sizeof(locTeid.val), FALSE, (PTR *)&pS10Tun);
   if (ROK == ret)
   {
      pUeCb = pS10Tun->ueCb;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Send Handover Cancel Acknowledge "\
            "to eNodeB"));
      ret = vbMmeSndHOCancelAck(pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Send Handover Cancel Acknowledge "\
               "to eNodeB"));
      }

      vbMmeUtlDelS10Tun(pUeCb);
      vbMmeUtlDelHoData(pUeCb);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting S10 Tunnel, Tunnel ID: [%ld]",\
            locTeid.val));
   }

   RETVALUE(ret);
}

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */

/******************************************************************************

                                 END OF FILE

******************************************************************************/
