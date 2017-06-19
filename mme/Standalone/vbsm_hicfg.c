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

     Name:    Trillium LTE CNE - MME Controller Module

     Type:    C Include file

     Desc:    This file contains the vb application source code

     File:    vb_mme.c

     Sid:      vbsm_hicfg.c@@/main/1 - Mon Jun 28 19:35:42 2010

     Prg:     sv 
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

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
/* #include "sz_err.h"*/        /* S1AP Error */
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

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "sct.x"           /* SCT interface defines           */

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */
/* vb007.101 s1ap include files order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "vbsm.x"          /*    */


#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

/* #define VB_SM_HI_CONFIGURED  (STGEN | STTSAP) */

#define VBSM_SBTSAP_CFG 0x2
#define VBSM_EGTSAP_CFG 0x4



#define VB_SM_HI_CONFIGURED (STGEN | VBSM_SBTSAP_CFG | VBSM_EGTSAP_CFG)

U8      vbSmHiCfg = 0;

/* Function prototypes */
PRIVATE Void vbMmeLhiHiGenCfg ARGS ((Void));

PRIVATE Void vbMmeLhiHiSAPCfg ARGS ((SpId spId));



/*
 *
 *       Fun:    vbMmeHiCfg - configure HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_hicfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeHiCfg
(
Void
)
#else
PUBLIC Void vbMmeHiCfg()
#endif /* ANSI */
{
   SM_TRC2(vbMmeHiCfg);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sending MME HiCfg...\n"));
   vbSmCb.hiPst.event = EVTLHICFGREQ;
   vbMmeLhiHiGenCfg();
   /* vb006.101: Introduced new flag VB_SB_SCTP for Trillium
    * SCTP and added support for Kernel SCTP. */
#ifdef VB_SB_SCTP
   vbMmeLhiHiSAPCfg(VBSM_SB_HI_SPID); /* From SCTP to TUCL in MME */
#else
   vbMmeLhiHiSAPCfg(VBSM_SZ_SPID); /* From S1AP to TUCL in MME */
#endif
   vbMmeLhiHiSAPCfg(VBSM_EG_HI_SPID); /* From EGTP to TUCL in S-GW */

   RETVOID;
}
/*
 *
 *       Fun:    vbMmeLhiHiGenCfg - fill in default genCfg for HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_hicfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLhiHiGenCfg
(
Void
)
#else
PRIVATE Void vbMmeLhiHiGenCfg()
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiGenCfg    *cfg;
#ifdef HI_TLS
   Random      rnd;
#endif /* HI_TLS */

   SM_TRC2(vbMmeLhiDefHiGenCfg);

   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   vbSmDefHdr(&hiMgt.hdr, ENTHI, STGEN, VBSM_HISMSEL);

   cfg = &hiMgt.t.cfg.s.hiGen;

   cfg->numSaps = VBSM_HI_NUMBSAPS;
   cfg->numCons = VBSM_HI_NMBCONS;
   cfg->numFdsPerSet = VBSM_HI_FDS;
   cfg->numFdBins = VBSM_HI_FDBINS;

   cfg->selTimeout = VBSM_HI_SELTIMEOUT;
   cfg->numRawMsgsToRead = 80;

   cfg->permTsk = VBSM_HI_PERM_TSK_FLAG;
   cfg->schdTmrVal = VBSM_HI_SCHDTMR_VAL;
   cfg->poolStrtThr = VBSM_HI_STRT_THRESH;
   cfg->poolDropThr = VBSM_HI_DRP_THRESH;
   cfg->poolStopThr = VBSM_HI_STP_THRESH;
   cfg->timeRes = VBSM_HI_TIME_RES;

   /* vb006.101: HI_LKSCTP configuration*/
#ifdef HI_LKSCTP
   cfg->numAssocBins   =  5;
   cfg->maxInitReTx    =  5;
   cfg->maxAssocReTx   =  8;
   cfg->maxPathReTx    =  4;
#endif /* HI_LKSCTP */

#ifdef HI_TLS
   cfg->prng.option =  LHI_OSSL_PRNG_SEED;
   for (i = 0;  i < LHI_OSSL_PRNG_SEED_LEN;  i++)
   {
      SRandom(&rnd);
      cfg->prng.u.seed[i] = (U8)rnd;
   }
#endif /* HI_TLS */

   cfg->lmPst.srcProcId = SFndProcId();
   cfg->lmPst.dstProcId = SFndProcId();
   cfg->lmPst.srcEnt = (Ent)ENTHI;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;

   cfg->lmPst.prior = (Prior)VBSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = (Region)vbSmCb.init.region;
   cfg->lmPst.pool = (Pool)vbSmCb.init.pool;
   cfg->lmPst.selector = (Selector)VBSM_HISMSEL;

   (Void)SmMiLhiCfgReq(&vbSmCb.hiPst, &hiMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME hiGenCfg...\n"));
   RETVOID;
} /* end of vbMmeLhiHiGenCfg() */

/*
 *
 *       Fun:    vbMmeLhiHiSAPCfg - fill in default HI SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_hicfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeLhiHiSAPCfg
(
SpId        spId
)
#else
PUBLIC Void vbMmeLhiHiSAPCfg(spId)
SpId        spId;
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiSapCfg    *cfg;

   SM_TRC2(vbMmeLhiHiSAPCfg);

   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   vbSmDefHdr(&hiMgt.hdr, ENTHI, STTSAP, VBSM_HISMSEL);

 /* Since we can have multiple SAPs for TUCL but only one element we should
   be able to distinguish the confirmation back */
   hiMgt.hdr.transId   = (TranId)spId; 

   cfg = &hiMgt.t.cfg.s.hiSap;

   cfg->spId = spId;
   cfg->uiSel = VBSM_HISBSEL;
   cfg->flcEnb = FALSE;
   cfg->txqCongStrtLim = VBSM_HI_CONG_STRT;
   cfg->txqCongDropLim  = VBSM_HI_CONG_DRP;
   cfg->txqCongStopLim = VBSM_HI_CONG_STP;
   cfg->numBins = VBSM_HI_NMB_HLBINS;

   cfg->uiMemId.region = vbSmCb.init.region;
   cfg->uiMemId.pool   = vbSmCb.init.pool;
   cfg->uiPrior        = PRIOR0;
   cfg->uiRoute        = RTESPEC;

   /* vb006.101: Added SAP type for HI_LKSCTP*/
#if (defined(HI_LKSCTP))
   cfg->type = HI_SCT_SAP;
#endif


   /* profile_1 : srvcType value = 3 (GTP header format) */
   cfg->hdrInf[0].hdrLen = 20;
   cfg->hdrInf[0].offLen = 2;
   cfg->hdrInf[0].lenLen = 2;
   cfg->hdrInf[0].flag   = 0;

   /* profile_2 : srvcType value = 4 (LDP header format) */
   cfg->hdrInf[1].hdrLen = 10;
   cfg->hdrInf[1].offLen = 2;
   cfg->hdrInf[1].lenLen = 2;
   cfg->hdrInf[1].flag   = 0;

   (Void)SmMiLhiCfgReq(&vbSmCb.hiPst, &hiMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME hi SapCfg...\n"));
   RETVOID;
} /* end of vbMmeLhiHiSAPCfg() */

/*
 *      FUN:   vbMmeHiShutDwn
 *
 *      Desc:  Brings the HI to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  vbsm_hicfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeHiShutDwn
(
Void
)
#else
PUBLIC S16 vbMmeHiShutDwn()
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   S16                  ret = ROK;
   SM_TRC2(vbMmeHiShutDwn);
   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   vbSmDefHdr(&hiMgt.hdr, ENTHI, STGEN, VBSM_HISMSEL);

   hiMgt.t.cntrl.action = ASHUTDOWN;
   hiMgt.t.cntrl.subAction = SAELMNT;

   vbSmCb.hiPst.event = EVTLHICNTRLREQ;
   (Void)SmMiLhiCntrlReq(&vbSmCb.hiPst, &hiMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME hi Shutdown CntrlReq...\n"));

   RETVALUE(ret);
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
*       File:  vbsm_hicfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiCfgCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLhiCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* configuration */
#endif
{
   SM_TRC2(SmMiLhiCfgCfm);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME hi CfgCfm with elmnt(%d) - status(%d)...\n",cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      vbSmHiCfg |=  cfm->hdr.elmId.elmnt;
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
         {
            vbSmHiCfg |=  cfm->hdr.elmId.elmnt;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME hi Gen CfgCfm...\n"));
            break;
         }
         case STTSAP:
         {
            /* Make sure transId matches */
            if(VBSM_SB_HI_SPID == cfm->hdr.transId)
            {
               vbSmHiCfg |=  VBSM_SBTSAP_CFG;
            }
            else if  (VBSM_EG_HI_SPID == cfm->hdr.transId)
            {
               vbSmHiCfg |=  VBSM_EGTSAP_CFG;
            }

            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME Sap CfgCfm...\n"));
            break;
         }
         default:
            VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid elemt(%d)...\n",cfm->hdr.elmId.elmnt));
        break;
      } 

      if (vbSmHiCfg == VB_SM_HI_CONFIGURED)
      {
         vbMmeSendMsg(EVTVBTUCLCFGDONE);
      }
      else
      {
           VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME CfgCfm Pending...\n"));
      }
   }
   else
   {
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME CfgCfm .NOT OK..\n"));
   }
   

   RETVALUE(ROK);
} /* end of SmMiLhiCfgCfm */



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
*       File:  vbsm_hicfg.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlCfm
(
Pst     *pst,          /* post structure */
HiMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLhiCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
HiMngmt *cfm;          /* control */
#endif
{
   SM_TRC2(SmMiLhiCntrlCfm)
   
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received control confirmation OK from TUCL...\n"));
   }
   else 
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received control confirmation NOT OK from TUCL...\n"));
   }
   RETVALUE(ROK);
} /* end of SmMiLhiCntrlCfm */

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
*       File:  vbsm_hicfg.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLhiStaInd
(
Pst     *pst,           /* post structure */
HiMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLhiStaInd(pst, usta)
Pst     *pst;           /* post structure */
HiMngmt *usta;          /* unsolicited status */
#endif
{
   SM_TRC2(SmMiLhiStaInd);

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);
} /* end of SmMiLhiStaInd */

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
*       File:  vbsm_hicfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiTrcInd
(
Pst *pst,               /* post structure */
HiMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLhiTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
HiMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   SM_TRC2(SmMiLhiTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLhiTrcInd */

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
*       File:  vbsm_hicfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStsCfm
(
Pst       *pst,         /* post structure */
HiMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLhiStsCfm(pst, sts)
Pst       *pst;         /* post structure */
HiMngmt   *sts;         /* confirmed statistics */
#endif
{
   SM_TRC2(SmMiLhiStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLhiStsCfm */

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
*       File:  vbsm_hicfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLhiStaCfm
(
Pst     *pst,           /* post structure */
HiMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLhiStaCfm(pst, sta)
Pst     *pst;           /* post structure */
HiMngmt *sta;            /* confirmed status */
#endif
{
   SM_TRC2(SmMiLhiStaCfm);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLhiStaCfm */

#ifdef __cplusplus
}
#endif /* __cplusplus */

/********************************************************************30**
 
         End of file:     vbsm_hicfg.c@@/main/1 - Mon Jun 28 19:35:42 2010

*********************************************************************31*/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ad         1. Initial version for LTE-CNE Phase 1.
/main/1   vb006.101 ragrawal    1. Introduced new flag for VB_SB_SCTP for Trillium 
                                   SCTP and added support for Kernel SCTP.
                                2. Added configuration and SAP type
                                   for HI_LKSCTP.
/main/1   vb007.101 akaranth    1. s1ap include files order has changed. 
***********************************************************************/
