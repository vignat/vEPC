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
#include "eg.h"           /* EG Upper Interface              */
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
#include <stdio.h>
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
#include "egt_util.x"
#include "vb_s3_hndlr.x"
#include "vb_s10_hndlr.x"
#include "vb_acc_s3.x"


PRIVATE  U8  imsi[15] = {0x02,0x04,0x06,0x08,0x00,0x01,0x02,0x09,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
PRIVATE  U8  apn[50] = "airtelIndia.com";

PRIVATE S16 vbAccFillHoData(VbMmeUeCb *pUeCb)
{
   U32 plmnId = 0x123456;
   U32 lac    = 0x7890;
   U8  srcToTgtCont[20] = {0};
   U8  tgtToSrcCont[20] = {0};

   VB_TRC2(vbAccFillHoData);

   VB_MME_ALLOC(&pUeCb->pHoData, sizeof(VbMmeHoData));
   if( pUeCb->pHoData == NULL )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation failed"));
      RETVALUE(RFAILED);
   }
   pUeCb->pHoData->hoType = VB_EMM_HO_LTE_TO_UTRAN;
   pUeCb->pHoData->hoMmeType = VB_MME_HO_SRC_MME;

   /*LAI*/
   /* PLMN */
   VB_MME_ALLOC(&pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val, 3);
   if( pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val == NULL )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation failed"));
      VB_MME_FREE( pUeCb->pHoData, sizeof(VbMmeHoData));
      RETVALUE(RFAILED);
   }

   cmMemcpy(pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val,\
                  (U8*)&plmnId,3);
   /* LAC */
   cmMemcpy(pUeCb->pHoData->hoTgt.tgtRncId.laiId.lAC.val,(U8*)&lac,2);

   /* RNC ID  */
   pUeCb->pHoData->hoTgt.tgtRncId.rncId.val = 10;

   /* RAN Cause */
   pUeCb->pHoData->hoCause.causeType = CAUSE_RADIONW;
   pUeCb->pHoData->hoCause.cause = SztCauseRadioNwhandover_desirable_for_radio_reasonEnum;

   /* Source To Target Container */
   VB_MME_ALLOC(&pUeCb->pHoData->srcToTgtTCont.pVal, sizeof(srcToTgtCont));
   if( pUeCb->pHoData->srcToTgtTCont.pVal == NULL )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation failed"));
      VB_MME_FREE( pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val, 3);
      VB_MME_FREE( pUeCb->pHoData, sizeof(VbMmeHoData));
      RETVALUE(RFAILED);
   }

   pUeCb->pHoData->srcToTgtTCont.len = sizeof(srcToTgtCont);
   cmMemcpy(pUeCb->pHoData->srcToTgtTCont.pVal,
      srcToTgtCont, sizeof(srcToTgtCont));

   /* Target To Source Container */
   VB_MME_ALLOC(&pUeCb->pHoData->tgtToSrcTCont.pVal, sizeof(tgtToSrcCont));
   if( pUeCb->pHoData->tgtToSrcTCont.pVal == NULL )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation failed"));
      VB_MME_FREE( pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val, 3);
      VB_MME_FREE( pUeCb->pHoData->srcToTgtTCont.pVal, sizeof(srcToTgtCont));
      VB_MME_FREE( pUeCb->pHoData, sizeof(VbMmeHoData));
      RETVALUE(RFAILED);
   }

   pUeCb->pHoData->tgtToSrcTCont.len = sizeof(tgtToSrcCont);
   cmMemcpy(pUeCb->pHoData->tgtToSrcTCont.pVal,
      tgtToSrcCont, sizeof(tgtToSrcCont));

   VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);

   RETVALUE(ROK);
}


PRIVATE S16 vbAccAddUeCb( VbMmeUeCb **pUeCb)
{
   VB_TRC2(vbAccAddUeCb);
   VB_MME_ALLOC(pUeCb, sizeof(VbMmeUeCb));
   if( *pUeCb == NULL )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation failed"));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

VbMmeEsmCb        *g_esmCb = NULLP;

PRIVATE S16 vbAccFillUeCb( VbMmeUeCb *pUeCb)
{
   U32               count;
   VbMmeEsmCb        *esmCb = NULLP;
   U32               spConnId;
   U32               suConnId;

   VB_TRC2(vbAccFillUeCb);
   spConnId = 0;
   suConnId = 0;

   pUeCb->eNodebCb = NULL;

   VB_MME_GET_UEID(suConnId, pUeCb->ueCbKey);
   for(count=0; count<CM_ESM_MAX_BEARER_ID; count++)
   {
      pUeCb->esmList[count] = NULLP;
   }

   pUeCb->suConnId = suConnId;
   pUeCb->spConnId = spConnId;
   pUeCb->emmCb.ueCb = pUeCb;
   pUeCb->mMeUeS1apId = spConnId;

   cmInitTimers(pUeCb->emmCb.emmTimers, VB_EMM_MAX_TIMERS);

   /* Setting UE parameters */
   pUeCb->ueCtxt.ueImsiLen = 15;
   cmMemcpy(pUeCb->ueCtxt.ueImsi,imsi,15);

   /* Setting EMM States */
   pUeCb->ecmCb.state = VB_ECM_CONNECTED;
   pUeCb->emmCb.state = VB_EMM_UE_REGISTERED;

   VB_MME_ALLOC(&esmCb, sizeof(VbMmeEsmCb));
   if( esmCb == NULL )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation failed"));
      RETVALUE(RFAILED);
   }


   esmCb->tranId.pres = PRSNT_NODEF;
   esmCb->tranId.val  = ++(pUeCb->transCntr);


   esmCb->tId = 0;
   esmCb->ueCb = pUeCb;
   esmCb->state = VB_ESM_ST_BEARER_INACTIVE;
   esmCb->prevstate = VB_ESM_ST_BEARER_ACTIVE;

   esmCb->apn.pres = 1;
   esmCb->apn.len = strlen((S8*)apn);
   cmMemcpy((U8* )&esmCb->apn.apn, (U8* )apn,strlen((S8*)apn));
   esmCb->pdnType = CM_ESM_PDN_IPV4;
   esmCb->bearerType = CM_ESM_EPS_DEF_BEARER;
   esmCb->bId = 5;

   cmMemcpy((U8*) &esmCb->s1SgwAddr,
      (U8*)&vbMmeCb.mmeCfg.sgwAddr,sizeof(CmTptAddr));
   esmCb->s1SgwTeId = 101;

   esmCb->esmPiggyBacked = TRUE;

   pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.dl = 10000000;
   pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.ul = 20000000;
   strcpy(pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].apn,(S8*)apn);
   cmMemcpy((U8*)&(pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].pdnGwAddr.u.ipv4NetAddr),
      (U8*)&vbMmeCb.mmeCfg.pgwAddr.u.ipv4TptAddr.address,sizeof(CmInetIpAddr));

   g_esmCb = esmCb;

   RETVALUE(ROK);

}

PRIVATE S16 vbAcciRAT4Gto3G(VbMmeUeCb *pUeCb)
{

   pUeCb->proc = VB_MME_ATTACH_PROC;
   vbMmeSndCSReq(pUeCb, g_esmCb);

   RETVALUE(ROK);
}


PUBLIC S16 vbAccTriggeriRATHO(void)
{
   static VbMmeUeCb            *pUeCb;

   VB_TRC2(vbAccTriggeriRATHO);

   if( pUeCb == NULL )
   {
      vbAccAddUeCb(&pUeCb);
      /*KWORKS Fix*/
      if(pUeCb != NULLP)
      {
      vbAccFillUeCb(pUeCb);

      /*vbAcciRAT3Gto4G(pUeCb);*/
      /* Set UE CB with HO Required paramters */
      vbAccFillHoData(pUeCb);
   }
   }

   /*KWORKS Fix*/
   if(pUeCb != NULLP)
   {
   vbAcciRAT4Gto3G(pUeCb);
   }




   RETVALUE(ROK);
}

/******************************************************************************

                                 END OF FILE

******************************************************************************/
