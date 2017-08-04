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

/********************************************************************20**

     Name:    Trillium LTE CNE - PGW Controller Module

     Type:    C Include file

     Desc:    This file contains the av application source code

     File:     jgsm_hicfg.c

     Sid:      avsm_hicfg.c@@/main/1 - Tue Aug 17 18:54:37 2010

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

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */


#include "lav.h"           /* CNE Layer management           */
#include "avsm.h"          /* */

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

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "lav.x"           /* CNE Layer management           */
#include "avsm.x"          /*    */

/* #define AV_SM_HI_CONFIGURED  (STGEN | STTSAP) */

#define AVSM_EGTSAP_CFG 0x4

#define AV_SM_HI_CONFIGURED (STGEN | STTSAP | AVSM_EGTSAP_CFG )

U8      avSmHiCfg = 0;

/* Function prototypes */
PRIVATE Void avPgwLhiHiGenCfg ARGS ((Void));

PRIVATE Void avPgwLhiHiSAPCfg ARGS ((SpId spId));

/*
 *
 *       Fun:    avPgwHiCfg - configure HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_hicfg.c
 *
 */

#ifdef ANSI
PUBLIC Void avPgwHiCfg
(
Void
)
#else
PUBLIC Void avPgwHiCfg()
#endif /* ANSI */
{
   SM_TRC2(avPgwHiCfg);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sending PGW HiCfg...\n"));
   avSmCb.hiPst.event = EVTLHICFGREQ;
   avPgwLhiHiGenCfg();
   avPgwLhiHiSAPCfg(AVSM_EG_HI_SPID); /* From EGTP to TUCL */

   RETVOID;
}
/*
 *
 *       Fun:    avPgwLhiHiGenCfg - fill in default genCfg for HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_hicfg.c
 *
 */

#ifdef ANSI
PRIVATE Void avPgwLhiHiGenCfg
(
Void
)
#else
PRIVATE Void avPgwLhiHiGenCfg()
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiGenCfg    *cfg;
#ifdef HI_TLS
   Random      rnd;
#endif /* HI_TLS */

   SM_TRC2(avPgwLhiDefHiGenCfg);

   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   avSmDefHdr(&hiMgt.hdr, ENTHI, STGEN, AVSM_HISMSEL);

   cfg = &hiMgt.t.cfg.s.hiGen;

   cfg->numSaps = AVSM_HI_NUMBSAPS;
   cfg->numCons = AVSM_HI_NMBCONS;
   cfg->numFdsPerSet = AVSM_HI_FDS;
   cfg->numFdBins = AVSM_HI_FDBINS;

   cfg->selTimeout = AVSM_HI_SELTIMEOUT;
   cfg->numRawMsgsToRead = 80;

   cfg->permTsk = AVSM_HI_PERM_TSK_FLAG;
   cfg->schdTmrVal = AVSM_HI_SCHDTMR_VAL;
   cfg->poolStrtThr = AVSM_HI_STRT_THRESH;
   cfg->poolDropThr = AVSM_HI_DRP_THRESH;
   cfg->poolStopThr = AVSM_HI_STP_THRESH;
   cfg->timeRes = AVSM_HI_TIME_RES;

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

   cfg->lmPst.prior = (Prior)AVSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = (Region)avSmCb.init.region;
   cfg->lmPst.pool = (Pool)avSmCb.init.pool;
   cfg->lmPst.selector = (Selector)AVSM_HISMSEL;

   (Void)SmMiLhiCfgReq(&avSmCb.hiPst, &hiMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW hiGenCfg...\n"));
   RETVOID;
} /* end of avPgwLhiHiGenCfg() */

/*
 *
 *       Fun:    avPgwLhiHiSAPCfg - fill in default HI SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_hicfg.c
 *
 */

#ifdef ANSI
PUBLIC Void avPgwLhiHiSAPCfg
(
SpId        spId
)
#else
PUBLIC Void avPgwLhiHiSAPCfg(spId)
SpId        spId;
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiSapCfg    *cfg;

   SM_TRC2(avPgwLhiHiSAPCfg);

   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   avSmDefHdr(&hiMgt.hdr, ENTHI, STTSAP, AVSM_HISMSEL);

 /* Since we can have multiple SAPs for TUCL but only one element we should
   be able to distinguish the confirmation back */
   hiMgt.hdr.transId   = (TranId)spId; 

   cfg = &hiMgt.t.cfg.s.hiSap;

   cfg->spId = spId;
   cfg->uiSel = AVSM_HIEGSEL;
   cfg->flcEnb = FALSE;
   cfg->txqCongStrtLim = AVSM_HI_CONG_STRT;
   cfg->txqCongDropLim  = AVSM_HI_CONG_DRP;
   cfg->txqCongStopLim = AVSM_HI_CONG_STP;
   cfg->numBins = AVSM_HI_NMB_HLBINS;

   cfg->uiMemId.region = avSmCb.init.region;
   cfg->uiMemId.pool   = avSmCb.init.pool;
   cfg->uiPrior        = PRIOR0;
   cfg->uiRoute        = RTESPEC;

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

   (Void)SmMiLhiCfgReq(&avSmCb.hiPst, &hiMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent Pgw hi SapCfg...\n"));
   RETVOID;
} /* end of avPgwLhiHiSAPCfg() */

/*
 *      FUN:   avPgwHiShutDwn
 *
 *      Desc:  Brings the HI to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  avsm_hicfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwHiShutDwn
(
Void
)
#else
PUBLIC S16 avPgwHiShutDwn()
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   S16                  ret = ROK;
   SM_TRC2(avPgwHiShutDwn);
   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   avSmDefHdr(&hiMgt.hdr, ENTHI, STGEN, AVSM_HISMSEL);

   hiMgt.t.cntrl.action = ASHUTDOWN;
   hiMgt.t.cntrl.subAction = SAELMNT;

   avSmCb.hiPst.event = EVTLHICNTRLREQ;
   (Void)SmMiLhiCntrlReq(&avSmCb.hiPst, &hiMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent Pgw hi Shutdown CntrlReq...\n"));

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
*       File:  avsm_hicfg.c
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
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW hi CfgCfm with elmnt(%d) - status(%d)...\n",cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      avSmHiCfg |=  cfm->hdr.elmId.elmnt;
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
         {
            avSmHiCfg |=  cfm->hdr.elmId.elmnt;
            AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW hi Gen CfgCfm...\n"));
            break;
         }
         case STTSAP:
         {
            /* Make sure transId matches */
            if  (AVSM_EG_HI_SPID == cfm->hdr.transId)
            {
               avSmHiCfg |=  AVSM_EGTSAP_CFG;
            }
            AVSM_DBG_INFO((AVSM_PRNTBUF,"Received Pgw Sap CfgCfm...\n"));
            break;
         }
         default:
         AVSM_DBG_ERROR((AVSM_PRNTBUF,"Invalid elemt(%d)...\n",cfm->hdr.elmId.elmnt));

            break;
      }

      if (avSmHiCfg == AV_SM_HI_CONFIGURED)
      {
         avPgwSendMsg(EVTAVTUCLCFGDONE);
      }
      else
      {
         AVSM_DBG_INFO((AVSM_PRNTBUF,"Received Pgw CfgCfm Pending...\n"));
      }
   }
   else
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received Pgw CfgCfm .NOT OK..\n"));
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
*       File:  avsm_hicfg.c
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
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received control confirmation OK from TUCL...\n"));
   }
   else 
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received control confirmation NOT OK from TUCL...\n"));
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
*       File:  avsm_hicfg.c
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
*       File:  avsm_hicfg.c
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
*       File:  avsm_hicfg.c
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
*       File:  avsm_hicfg.c
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

/********************************************************************30**
 
         End of file:     avsm_hicfg.c@@/main/1 - Tue Aug 17 18:54:37 2010

*********************************************************************31*/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp         1.LTE-CNE:PGW Initial Release
*********************************************************************91*/
