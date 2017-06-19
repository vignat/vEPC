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

     Name:    Trillium LTE CNE - MME Stack Module

     Type:    C Include file

     Desc:    This file contains the stack manager source code

     File:    vbsm_sbcfg.c

     Sid:      vbsm_vbcfg.c@@/main/1 - Mon Jun 28 19:35:43 2010

     Prg:      
*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library           */
#include "cm_dns.h"         /* common DNS libraru defines */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#ifdef    SB
#include "sct.h"           /* SCT interface defines           */
#include "lsb.h"
#endif    /* SB */


#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
/* #include "sz_err.h" */       /* S1AP Error */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#include "vb_hss.h"        /* CNE Application defines        */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "vbsm.h"          /* */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "cm_dns.x"         /* common DNS libraru defines */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#ifdef    SB
#include "sct.x"           /* SCT interface defines           */
#include "lsb.x"
#endif    /* SB */

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */
/* vb007.101 : s1ap include file order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "vbsm.x"          

#define VB_MME_CONFIGURED  (STVBGEN + STVBSZTSAP + \
                           STVBEGTSAP + STVBEGTSAP + STVBEGTSAP)
/* there shall be 4 saps from which bind confirm should be received - echo , s3 , s10 and s11 */
#define VB_MME_EG_BOUND    (STVBEGTSAP + STVBEGTSAP + STVBEGTSAP + STVBEGTSAP)


PRIVATE U8   vbMmeCfg = 0;
PRIVATE U8   vbMmeBound = 0;

/* Function prototypes */

PRIVATE Void vbMmeLvbGenCfg ARGS ((Void));

PRIVATE Void vbMmeLvbSztSapCfg ARGS ((SuId        suId,
                                      SpId        spId));

PRIVATE Void vbMmeLvbEgtSapCfg ARGS ((SuId        suId,
                                      SpId        spId, U8 sapType));

/*
 *
 *       Fun:    vbMmeLvbCfg 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_vbcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeLvbCfg
(
Void
)
#else
PUBLIC Void vbMmeLvbCfg()
#endif /* ANSI */
{
   SM_TRC2(vbMmeLvbCfg);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sending MME Vb configuration...\n"));

   vbMmeLvbGenCfg();

   vbMmeLvbSztSapCfg(VBSM_SZ_SPID, VBSM_SZ_SUID);/* Configure SAP with S1AP */

   vbMmeLvbEgtSapCfg(VBSM_EG_SPID, VBSM_EG_SUID, LVB_EGT_SAP);

   vbMmeLvbEgtSapCfg(VBSM_EG_S10_SPID, VBSM_EG_S10_SUID, LVB_EGT_S10_SAP);

   vbMmeLvbEgtSapCfg(VBSM_EG_S3_SPID, VBSM_EG_S3_SUID, LVB_EGT_S3_SAP);

  /* Configure ECHO SAP with EGTP */
   vbMmeLvbEgtSapCfg(VBSM_EG_ECHO_SPID, VBSM_EG_ECHO_SUID, 
                     LVB_EGT_DFLT_ECHO_SAP);

   RETVOID;
} /* vbMmeLvbCfg */

/*
 *
 *       Fun:    vbMmeLvbGenCfg - fill in default genCfg for VB
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_vbcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLvbGenCfg
(
Void
)
#else
PRIVATE Void vbMmeLvbGenCfg()
#endif /* ANSI */
{
   VbMngmt  vbMmeMgt;
   VbMmeGenCfg  *cfg;
   U8 gummeiIdx, plmnIdx, taiIdx;
   U16    indx;
   VbGUMMEI *gummeiCfg;

   SM_TRC2(vbMmeLvbGenCfgReq);

   cmMemset((U8 *)&vbMmeMgt, 0, sizeof(VbMngmt));
   vbSmDefHdr(&vbMmeMgt.hdr, ENTUZ, STVBGEN, VBSM_VBSMSEL);

   cfg   = &(vbMmeMgt.u.cfg.s.mmeGenCfg);

   cfg->lmPst.srcProcId = SFndProcId();
   cfg->lmPst.dstProcId = SFndProcId();
   cfg->lmPst.srcEnt    = (Ent)ENTUZ;
   cfg->lmPst.dstEnt    = (Ent)ENTSM;
   cfg->lmPst.srcInst   = (Inst)0;
   cfg->lmPst.dstInst   = (Inst)0;
   cfg->lmPst.prior     = (Prior)VBSM_MSGPRIOR;
   cfg->lmPst.route     = (Route)RTESPEC;
   cfg->lmPst.event     = (Event)EVTNONE;
   cfg->lmPst.region    = (Region)vbSmCb.init.region;
   cfg->lmPst.pool      = (Pool)vbSmCb.init.pool;
   cfg->lmPst.selector  = (Selector)VBSM_VBSMSEL;
   cfg->mem.region      = (Region)vbSmCb.init.region;
   cfg->mem.pool        = (Pool)vbSmCb.init.pool;
   cfg->maxNmbSztSaps   = LVB_SZT_SAPS;
   cfg->maxNmbEgtSaps   = LVB_EGT_SAPS;
   cfg->maxNmbEnbs      = LVB_MAX_ENBS;
   cfg->maxNmbUes       = LVB_MAX_ENBS * vbSmCb.cfgCb.ueNum;
   cfg->tmrRes          = LVB_MME_TMR_RES;

   cfg->srvGLst.noOfGummeis = LVB_GUMMEIS;
   for(gummeiIdx = 0; gummeiIdx < (cfg->srvGLst.noOfGummeis); gummeiIdx++)
   {
      gummeiCfg = &cfg->srvGLst.gummeiLst[gummeiIdx];
      
      gummeiCfg->nwId.mcc[0] = vbSmCb.cfgCb.mccDig1;
      gummeiCfg->nwId.mcc[1] = vbSmCb.cfgCb.mccDig2;
      gummeiCfg->nwId.mcc[2] = vbSmCb.cfgCb.mccDig3;

      gummeiCfg->nwId.mnc[0] = vbSmCb.cfgCb.mncDig1;
      gummeiCfg->nwId.mnc[1] = vbSmCb.cfgCb.mncDig2;
      gummeiCfg->nwId.mnc[2] = vbSmCb.cfgCb.mncDig3;

      gummeiCfg->pLst.noOfPlmns = LVB_PLMNS;
      for(plmnIdx = 0; plmnIdx < gummeiCfg->pLst.noOfPlmns; plmnIdx++)
      {
         gummeiCfg->pLst.plmnLst[plmnIdx].plmnId[0] = \
               vbSmCb.cfgCb.mccDig2 << 4 | vbSmCb.cfgCb.mccDig1;
         gummeiCfg->pLst.plmnLst[plmnIdx].plmnId[1] = \
               vbSmCb.cfgCb.mncDig3 << 4 | vbSmCb.cfgCb.mccDig3;
         gummeiCfg->pLst.plmnLst[plmnIdx].plmnId[2] = \
               vbSmCb.cfgCb.mncDig2 << 4 | vbSmCb.cfgCb.mncDig1;
      }

      gummeiCfg->grpIdLst.noOfGrpIds = LVB_GROUPIDS;
      for(indx = 0; indx < gummeiCfg->grpIdLst.noOfGrpIds; indx++)
      {
         gummeiCfg->grpIdLst.grpId[indx] = LVB_MME_GROUP_ID;
      }

      gummeiCfg->codeLst.noOfCodes = LVB_MMECODES;
      for(indx = 0; indx < gummeiCfg->codeLst.noOfCodes; indx++)
      {
         gummeiCfg->codeLst.mmeCode[indx] = LVB_MME_CODE;
      }
   }

   cfg->mmeName.length = cmStrlen((CONSTANT U8 *)LVB_MME_NAME);
   cmMemcpy((U8 *)cfg->mmeName.name, (CONSTANT U8 *) LVB_MME_NAME,
            (cmStrlen((CONSTANT U8 *)LVB_MME_NAME)));

   cfg->tLst.noOfTais = LVB_TAIS;
   for(taiIdx = 0; taiIdx < cfg->tLst.noOfTais; taiIdx++)
   {
      cfg->tLst.taiLst[taiIdx].tac = LVB_TA_CODE;
      cfg->tLst.taiLst[taiIdx].nwId.mcc[0] = vbSmCb.cfgCb.mccDig1;
      cfg->tLst.taiLst[taiIdx].nwId.mcc[1] = vbSmCb.cfgCb.mccDig2;
      cfg->tLst.taiLst[taiIdx].nwId.mcc[2] = vbSmCb.cfgCb.mccDig3;

      cfg->tLst.taiLst[taiIdx].nwId.mnc[0] = vbSmCb.cfgCb.mccDig1;
      cfg->tLst.taiLst[taiIdx].nwId.mnc[1] = vbSmCb.cfgCb.mccDig2;
      cfg->tLst.taiLst[taiIdx].nwId.mnc[2] = vbSmCb.cfgCb.mccDig3;
   }

   cfg->selfAddr.type                  = CM_TPTADDR_IPV4;
   cfg->selfAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.mmeIpAddr;
   cfg->selfAddr.u.ipv4TptAddr.port    = vbSmCb.cfgCb.egNonDfltPort;

   cfg->sgwAddr.type                   = CM_TPTADDR_IPV4;
   cfg->sgwAddr.u.ipv4TptAddr.port     = vbSmCb.cfgCb.egDfltPort;
   cfg->sgwAddr.u.ipv4TptAddr.address  = vbSmCb.cfgCb.sgwIpAddr;

   cfg->pgwAddr.type                   = CM_TPTADDR_IPV4;
   cfg->pgwAddr.u.ipv4TptAddr.port     = vbSmCb.cfgCb.egDfltPort;
   cfg->pgwAddr.u.ipv4TptAddr.address  = vbSmCb.cfgCb.pgwIpAddr;

   cfg->s10selfAddr.type                  = CM_TPTADDR_IPV4;
   cfg->s10selfAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.mmeIpAddr;
   cfg->s10selfAddr.u.ipv4TptAddr.port    = vbSmCb.cfgCb.egS10NonDfltPort;

   cfg->s3selfAddr.type                  = CM_TPTADDR_IPV4;
   cfg->s3selfAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.mmeIpAddr;
   cfg->s3selfAddr.u.ipv4TptAddr.port    = vbSmCb.cfgCb.egS3NonDfltPort;

   cfg->sgsnAddr.type                  = CM_TPTADDR_IPV4;
   cfg->sgsnAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.sgsnIpAddr;
   cfg->sgsnAddr.u.ipv4TptAddr.port    = vbSmCb.cfgCb.egS3NonDfltPort;

   /* Configure the first eNB */
   cfg->eNbCfg[0].peerId   = LVB_ENB_PEER_ID_0;
   cfg->eNbCfg[0].lac      = LVB_LAC;
   cfg->eNbCfg[0].rac      = LVB_RAC;
   cfg->eNbCfg[0].t3412Tmr = vbSmCb.cfgCb.t3412;
   cfg->eNbCfg[0].t3413Tmr = LVB_T3413;
   cfg->eNbCfg[0].t3402Tmr = LVB_T3402;
   cfg->eNbCfg[0].t3422Tmr = LVB_T3422;
   cfg->eNbCfg[0].t3450Tmr = LVB_T3450;
   cfg->eNbCfg[0].t3460Tmr = LVB_T3460;
   cfg->eNbCfg[0].t3470Tmr = LVB_T3470;
   cfg->eNbCfg[0].t3485Tmr = LVB_T3485;
   cfg->eNbCfg[0].t3486Tmr = LVB_T3486;
   cfg->eNbCfg[0].t3495Tmr = LVB_T3495;
   cfg->eNbCfg[0].maxUeNum = cfg->maxNmbUes;
   cfg->eNbCfg[0].eNbAddr.type                  = CM_TPTADDR_IPV4;
   cfg->eNbCfg[0].eNbAddr.u.ipv4TptAddr.port    = vbSmCb.cfgCb.eNbPort;
   cfg->eNbCfg[0].eNbAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.eNbIpAddr;

   /* Configure the second eNB */
   cfg->eNbCfg[1].peerId   = LVB_ENB_PEER_ID_1;
   cfg->eNbCfg[1].lac      = LVB_LAC;
   cfg->eNbCfg[1].rac      = LVB_RAC;
   cfg->eNbCfg[1].t3412Tmr = vbSmCb.cfgCb.t3412;
   cfg->eNbCfg[1].t3413Tmr = LVB_T3413;
   cfg->eNbCfg[1].t3402Tmr = LVB_T3402;
   cfg->eNbCfg[1].t3422Tmr = LVB_T3422;
   cfg->eNbCfg[1].t3450Tmr = LVB_T3450;
   cfg->eNbCfg[1].t3460Tmr = LVB_T3460;
   cfg->eNbCfg[1].t3470Tmr = LVB_T3470;
   cfg->eNbCfg[1].t3485Tmr = LVB_T3485;
   cfg->eNbCfg[1].t3486Tmr = LVB_T3486;
   cfg->eNbCfg[1].t3495Tmr = LVB_T3495;
   cfg->eNbCfg[1].maxUeNum = cfg->maxNmbUes;
   cfg->eNbCfg[1].eNbAddr.type                  = CM_TPTADDR_IPV4;
   cfg->eNbCfg[1].eNbAddr.u.ipv4TptAddr.port    = vbSmCb.cfgCb.eNbPort_2;
   cfg->eNbCfg[1].eNbAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.eNbIpAddr_2;

   cmMemcpy(cfg->tgtMmePlmnId, vbSmCb.cfgCb.tgtMmePlmnId, LVB_MAX_PLMN_ID_LEN);

   cfg->tgtMmeIpAddr.type                   = CM_TPTADDR_IPV4;
   cfg->tgtMmeIpAddr.u.ipv4TptAddr.port     = vbSmCb.cfgCb.egDfltPort;
   cfg->tgtMmeIpAddr.u.ipv4TptAddr.address  = vbSmCb.cfgCb.tgtMmeIpAddr;

   cmMemcpy(cfg->sgsnPlmnId, vbSmCb.cfgCb.sgsnPlmnId, LVB_MAX_PLMN_ID_LEN);

   cfg->sgsnIpAddr.type                   = CM_TPTADDR_IPV4;
   cfg->sgsnIpAddr.u.ipv4TptAddr.port     = VBSM_EG_DFLT_PORT;
   cfg->sgsnIpAddr.u.ipv4TptAddr.address  = vbSmCb.cfgCb.sgsnIpAddr;
   
   
   (Void)SmMiLvbCfgReq(&vbSmCb.vbMmePst, &vbMmeMgt);

   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME Vb GenCFg...\n"));
   RETVOID;
} /* end of vbMmeLvbGenCfg() */


/*
 *
 *       Fun:    vbMmeLvbSztSapCfg 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_vbcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLvbSztSapCfg
(
SuId        suId,
SpId        spId
)
#else
PRIVATE Void vbMmeLvbSztSapCfg(suId, spId)
SuId        suId;
SpId        spId;
#endif /* ANSI */
{
   VbMngmt  vbMmeMgt;
   VbMmeLSapCfg  *cfg;

   SM_TRC2(vbMmeLvbSztSapCfg);

   cmMemset((U8 *)&vbMmeMgt, 0, sizeof(VbMngmt));
   vbSmDefHdr(&vbMmeMgt.hdr, ENTUZ, STVBSZTSAP, VBSM_VBSMSEL);

   cfg   = &(vbMmeMgt.u.cfg.s.mmeLSapCfg);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = VBSM_VBSZSEL;
   cfg->mem.region   = vbSmCb.init.region;
   cfg->mem.pool     = vbSmCb.init.pool;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SFndProcId();
   cfg->dstEntId     = ENTSZ;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = VBSM_LVB_VAL_10;
   cfg->maxBndRetry = VBSM_LVB_VAL_3;

   (Void)SmMiLvbCfgReq(&vbSmCb.vbMmePst, &vbMmeMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME Vb LSapCfg...\n"));
   RETVOID;
} /* end of vbMmeLvbLSapCfg() */

/*
 *      FUN:   vbMmeSztSapBndReq
 *
 *      Desc:  Binds MME with S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_vbcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSztSapBndReq
(
Void
)
#else
PUBLIC S16 vbMmeSztSapBndReq()
#endif /* ANSI */
{
   VbMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(vbMmeSztSapBndReq);
   cmMemset((U8 *)&cntrl, 0, sizeof(VbMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTUZ, STVBSZTSAP, VBSM_VBSMSEL);

   cntrl.u.cntrl.mmeCntrl.action    = ABND;
   cntrl.u.cntrl.mmeCntrl.subAction = 0;

   (Void)SmMiLvbCntrlReq(&vbSmCb.vbMmePst, &cntrl);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME SztBndReq...\n"));
   RETVALUE(ret);
}

/*
 *
 *       Fun:    vbMmeLvbEgtSapCfg 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_vbcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLvbEgtSapCfg
(
SuId        suId,
SpId        spId,
U8          sapType
)
#else
PRIVATE Void vbMmeLvbEgtSapCfg(suId, spId, sapType)
SuId        suId;
SpId        spId;
U8          sapType;
#endif /* ANSI */
{
   VbMngmt      vbMmeMgt;
   VbMmeLSapCfg  *cfg;

   SM_TRC2(vbMmeLvbEgtSapCfg);

   cmMemset((U8 *)&vbMmeMgt, 0, sizeof(VbMngmt));
   vbSmDefHdr(&vbMmeMgt.hdr, ENTEU, STVBEGTSAP, VBSM_VBSMSEL);

   cfg   = &(vbMmeMgt.u.cfg.s.mmeLSapCfg);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = VBSM_VBEGSEL;
   cfg->mem.region   = vbSmCb.init.region;
   cfg->mem.pool     = vbSmCb.init.pool;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SFndProcId();
   cfg->dstEntId     = ENTEG;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = VBSM_LVB_VAL_10;
   cfg->maxBndRetry = VBSM_LVB_VAL_3;

   cfg->type = sapType;

   (Void)SmMiLvbCfgReq(&vbSmCb.vbMmePst, &vbMmeMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Lower Sap Configuration for MME -> "\
         "EGTP...\n"));
   RETVOID;
} /* end of vbMmeLvbEgtSapCfg() */



/*
 *      FUN:   vbMmeEgtSapBndReq
 *
 *      Desc:  Binds MME with EGTP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_vbcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeEgtSapBndReq
(
Void
)
#else
PUBLIC S16 vbMmeEgtSapBndReq()
#endif /* ANSI */
{
   VbMngmt  cntrl;
   SM_TRC2(vbMmeEgtSapBndReq);
   cmMemset((U8 *)&cntrl, 0, sizeof(VbMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTEU, STVBEGTSAP, VBSM_VBSMSEL);

   cntrl.u.cntrl.mmeCntrl.action    = ABND;
   cntrl.u.cntrl.mmeCntrl.subAction = 0;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */
   vbSmCb.vbMmePst.event = EVTLVBCNTRLREQ;

   cntrl.u.cntrl.mmeCntrl.u.sap.id = VBSM_EG_ECHO_SPID;
   (Void)SmMiLvbCntrlReq(&vbSmCb.vbMmePst, &cntrl);

   cntrl.u.cntrl.mmeCntrl.u.sap.id = VBSM_EG_SPID;
   (Void)SmMiLvbCntrlReq(&vbSmCb.vbMmePst, &cntrl);

   cntrl.u.cntrl.mmeCntrl.u.sap.id = VBSM_EG_S10_SPID;
   (Void)SmMiLvbCntrlReq(&vbSmCb.vbMmePst, &cntrl);

   cntrl.u.cntrl.mmeCntrl.u.sap.id = VBSM_EG_S3_SPID;
   (Void)SmMiLvbCntrlReq(&vbSmCb.vbMmePst, &cntrl);

   VBSM_DBG_INFO((VBSM_PRNTBUF, "Sent Lower Sap Bind Request, MME -> EGTP\n"));
   RETVALUE(ROK);
} /* vbMmeEgtSapBndReq */


#ifdef DEBUGP
/*
 *      FUN:   vbMmeDbgMaskCntrlReq
 *
 *      Desc:  
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_vbcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeDbgMaskCntrlReq
(
Void
)
#else
PUBLIC S16 vbMmeDbgMaskCntrlReq()
#endif /* ANSI */
{
   VbMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(vbMmeDbgMaskCntrlReq);
   cmMemset((U8 *)&cntrl, 0, sizeof(VbMngmt));
   
   vbSmDefHdr(&cntrl.hdr, ENTUZ, STVBGEN, VBSM_VBSMSEL);

   cntrl.u.cntrl.mmeCntrl.action    = AENA;
   cntrl.u.cntrl.mmeCntrl.subAction = SADBG;

   cntrl.u.cntrl.mmeCntrl.u.dbg.dbgMask = vbSmCb.cfgCb.mmeDbgMask;

   vbSmCb.vbMmePst.event = EVTLVBCNTRLREQ;
   (Void)SmMiLvbCntrlReq(&vbSmCb.vbMmePst, &cntrl);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME LvbCntrlReq for Debug Print\n"));
   RETVALUE(ret);
} /* vbMmeDbgMaskCntrlReq */
#endif 

/*
 *      FUN:   vbMmeVbShutDwn
 *
 *      Desc:  Brings the HI to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  vbsm_vbcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVbShutDwn
(
Void
)
#else
PUBLIC S16 vbMmeVbShutDwn()
#endif /* ANSI */
{

   RETVALUE(ROK);
}

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by Layer to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm_vbcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbCfgCfm
(
Pst     *pst,          /* post structure */
VbMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLvbCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
VbMngmt *cfm;          /* configuration */
#endif
{
   SM_TRC2(SmMiLvbCfgCfm);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME Vb CfgCfm with - status(%d)..\n",\
         cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      vbMmeCfg +=  cfm->hdr.elmId.elmnt;
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME Configuration Cfm...\n"));

      /* If MME Configuration is complete*/
      if(VB_MME_CONFIGURED == vbMmeCfg)
      {
         vbMmeSendMsg(EVTVBMMECFGDONE);
      }
      else
      {
         VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME Configuration PENDING.\n"));
      }
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Configuration failed for element "\
            " type(%d)...\n", (cfm->hdr.elmId.elmnt)));
   }

   /* Valgrind Fixes */
   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
   RETVALUE(ROK);
} /* end of SmMiLvbCfgCfm */

/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by  Layer to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm_vbcfg.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLvbCntrlCfm
(
Pst     *pst,          /* post structure */
VbMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLvbCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
VbMngmt *cfm;          /* control */
#endif
{
   SM_TRC2(SmMiLvbCntrlCfm);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      /* Debug print here */
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME Control Cfm...\n"));

#ifdef DEBUGP
         /* Debug mask */
         if(cfm->u.cntrl.mmeCntrl.subAction == SADBG)
         {
            vbMmeSendMsg(EVTVBMMEDBGCNTRLCFM);
         }
#endif 

      /* We shouldnt send bind confirmation event
         now. MME Application shall generate an Alarm when actual bind
         confirmation arrives from S1AP */
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Control Cfm. Not Ok..\n"));
   }

   /* Valgrind fixes */
   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(VbMngmt));
   RETVALUE(ROK);
} /* end of SmMiLvbCntrlCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by Layer to present  unsolicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm_vbcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbStaInd
(
Pst     *pst,           /* post structure */
VbMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLvbStaInd(pst, usta)
Pst     *pst;           /* post structure */
VbMngmt *usta;          /* unsolicited status */
#endif
{
   SM_TRC2(SmMiLvbStaInd);

   /* Debug print here - print all the information from usta's Alarm structure
      VbUstaDgn */
   if ((usta->u.usta.alarm.event == LVB_EVENT_MME_S1AP_BNDCFM) &&
       (usta->u.usta.alarm.cause == LCM_CAUSE_UNKNOWN))
   {
      /* Check if this is triggered from somewhere else */
      vbMmeSendMsg(EVTVBAPPBINDCFM);
   }

   if ((usta->u.usta.alarm.event == LVB_EVENT_MME_EGTP_BNDCFM) &&
       (usta->u.usta.alarm.cause == LCM_CAUSE_UNKNOWN))
   {
      vbMmeBound += STVBEGTSAP;
      if(VB_MME_EG_BOUND == vbMmeBound)
      {
         vbMmeSendMsg(EVTVBS11APPBINDCFM);
      }
   }

   /* Valgrind fixes */
   SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(VbMngmt));
   RETVALUE(ROK);
} /* end of SmMiLvbStaInd */


/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used by  Layer to present trace
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm_vbcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbTrcInd
(
Pst *pst,               /* post structure */
VbMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLvbTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
VbMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   SM_TRC2(SmMiLvbTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLvbTrcInd */

/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used by Layer to present solicited statistics 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm_vbcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbStsCfm
(
Pst       *pst,         /* post structure */
VbMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLvbStsCfm(pst, sts)
Pst       *pst;         /* post structure */
VbMngmt   *sts;         /* confirmed statistics */
#endif
{
   SM_TRC2(SmMiLvbStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLvbStsCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used by Layer to present solicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  vbsm_vbcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbStaCfm
(
Pst     *pst,           /* post structure */
VbMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLvbStaCfm(pst, sta)
Pst     *pst;           /* post structure */
VbMngmt *sta;            /* confirmed status */
#endif
{
   SM_TRC2(SmMiLvbStaCfm);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLvbStaCfm */


/********************************************************************30**
 
         End of file:     vbsm_vbcfg.c@@/main/1 - Mon Jun 28 19:35:43 2010
 
*********************************************************************31*/
 
/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ad         1. Initial version for LTE-CNE Phase 1.
/main/1  vb001.101   rk         1. Added timer 3485
/main/1  vb005.101   rk         1. Added Network Intiation Detach Feature
/main/1  vb006.101  ragrawal    1. CRID:ccpu00117391 ERAB RELEASE support.
                                2. Support for MULTIPLE_ENB.
/main/1  vb007.101  akaranth    1. s1ap inlcude files order has changed.
*********************************************************************91*/
