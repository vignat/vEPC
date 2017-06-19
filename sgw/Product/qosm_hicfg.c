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

     Name:    Trillium LTE CNE - SGW Controller Module

     Type:    C Include file

     Desc:    This file contains the SGW application source code

     File:     qosm_hicfg.c   

     Sid:      qosm_hicfg.c@@/main/1 - Thu Jul  8 21:22:22 2010

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

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */


#include "lqo.h"           /* CNE Layer management           */
#include "qosm.h"          /* */

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

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "lqo.x"           /* CNE Layer management           */
#include "qosm.x"          /*    */

#define QO_SM_HI_CONFIGURED  (STGEN | STTSAP)

#define QOSM_EGTSAP_CFG 0x4

/*#define QO_SM_HI_CONFIGURED (STGEN |  STTSAP | QOSM_EGTSAP_CFG )*/

U8      qoSmHiCfg = 0;

/* Function prototypes */
PRIVATE Void qoSgwLhiHiGenCfg ARGS ((Void));

PRIVATE Void qoSgwLhiHiSAPCfg ARGS ((SpId spId));



/*
 *
 *       Fun:    qoSgwHiCfg - configure HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_hicfg.c
 *
 */

#ifdef ANSI
PUBLIC Void qoSgwHiCfg
(
Void
)
#else
PUBLIC Void qoSgwHiCfg()
#endif /* ANSI */
{
   SM_TRC2(qoSgwHiCfg);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sending Sgw HiCfg...\n"));
   qoSmCb.hiPst.event = EVTLHICFGREQ;
   qoSgwLhiHiGenCfg();
   qoSgwLhiHiSAPCfg(QOSM_EG_HI_SPID); /* From EGTP to TUCL in S-GW */

   RETVOID;
}
/*
 *
 *       Fun:    qoSgwLhiHiGenCfg - fill in default genCfg for HI
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_hicfg.c
 *
 */

#ifdef ANSI
PRIVATE Void qoSgwLhiHiGenCfg
(
Void
)
#else
PRIVATE Void qoSgwLhiHiGenCfg()
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiGenCfg    *cfg;
#ifdef HI_TLS
   Random      rnd;
#endif /* HI_TLS */

   SM_TRC2(qoSgwLhiDefHiGenCfg);

   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   qoSmDefHdr(&hiMgt.hdr, ENTHI, STGEN, QOSM_HISMSEL);

   cfg = &hiMgt.t.cfg.s.hiGen;

   cfg->numSaps = QOSM_HI_NUMBSAPS;
   cfg->numCons = QOSM_HI_NMBCONS;
   cfg->numFdsPerSet = QOSM_HI_FDS;
   cfg->numFdBins = QOSM_HI_FDBINS;

   cfg->selTimeout = QOSM_HI_SELTIMEOUT;
   cfg->numRawMsgsToRead = 80;

   cfg->permTsk = QOSM_HI_PERM_TSK_FLAG;
   cfg->schdTmrVal = QOSM_HI_SCHDTMR_VAL;
   cfg->poolStrtThr = QOSM_HI_STRT_THRESH;
   cfg->poolDropThr = QOSM_HI_DRP_THRESH;
   cfg->poolStopThr = QOSM_HI_STP_THRESH;
   cfg->timeRes = QOSM_HI_TIME_RES;

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

   cfg->lmPst.prior = (Prior)QOSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = (Region)qoSmCb.init.region;
   cfg->lmPst.pool = (Pool)qoSmCb.init.pool;
   cfg->lmPst.selector = (Selector)QOSM_HISMSEL;

   (Void)SmMiLhiCfgReq(&qoSmCb.hiPst, &hiMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw hiGenCfg...\n"));
   RETVOID;
} /* end of qoSgwLhiHiGenCfg() */

/*
 *
 *       Fun:    qoSgwLhiHiSAPCfg - fill in default HI SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_hicfg.c
 *
 */

#ifdef ANSI
PUBLIC Void qoSgwLhiHiSAPCfg
(
SpId        spId
)
#else
PUBLIC Void qoSgwLhiHiSAPCfg(spId)
SpId        spId;
#endif /* ANSI */
{
   HiMngmt     hiMgt;
   HiSapCfg    *cfg;

   SM_TRC2(qoSgwLhiHiSAPCfg);

   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   qoSmDefHdr(&hiMgt.hdr, ENTHI, STTSAP, QOSM_HISMSEL);

 /* Since we can have multiple SAPs for TUCL but only one element we should
   be able to distinguish the confirmation back */
   hiMgt.hdr.transId   = (TranId)spId; 

   cfg = &hiMgt.t.cfg.s.hiSap;

   cfg->spId = spId;
   cfg->uiSel = QOSM_HIEGSEL;
   cfg->flcEnb = FALSE;
   cfg->txqCongStrtLim = QOSM_HI_CONG_STRT;
   cfg->txqCongDropLim  = QOSM_HI_CONG_DRP;
   cfg->txqCongStopLim = QOSM_HI_CONG_STP;
   cfg->numBins = QOSM_HI_NMB_HLBINS;

   cfg->uiMemId.region = qoSmCb.init.region;
   cfg->uiMemId.pool   = qoSmCb.init.pool;
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

   (Void)SmMiLhiCfgReq(&qoSmCb.hiPst, &hiMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw hi SapCfg...\n"));
   RETVOID;
} /* end of qoSgwLhiHiSAPCfg() */

/*
 *      FUN:   qoSgwHiShutDwn
 *
 *      Desc:  Brings the HI to the state before configuration
 *
 *      Ret:   void
 *
 *      Notes: None
 *
 *      File:  qosm_hicfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwHiShutDwn
(
Void
)
#else
PUBLIC S16 qoSgwHiShutDwn()
#endif /* ANSI */
{
   HiMngmt              hiMgt;
   S16                  ret = ROK;
   SM_TRC2(qoSgwHiShutDwn);
   cmMemset((U8 *)&hiMgt, 0, sizeof(HiMngmt));
   qoSmDefHdr(&hiMgt.hdr, ENTHI, STGEN, QOSM_HISMSEL);

   hiMgt.t.cntrl.action = ASHUTDOWN;
   hiMgt.t.cntrl.subAction = SAELMNT;

   qoSmCb.hiPst.event = EVTLHICNTRLREQ;
   (Void)SmMiLhiCntrlReq(&qoSmCb.hiPst, &hiMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw hi Shutdown CntrlReq...\n"));

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
*       File:  qosm_hicfg.c
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
   QOSM_DBG_INFO((QOSM_PRNTBUF,
            "Received Sgw hi CfgCfm with elmnt(%d) - status(%d)...\n",
            cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      qoSmHiCfg |=  cfm->hdr.elmId.elmnt;
      switch (cfm->hdr.elmId.elmnt)
      {
         case STGEN:
         {
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw hi Gen CfgCfm...\n"));
            break;
         }
         case STTSAP:
         {
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw Sap CfgCfm...\n"));
            break;
         }
         default:
         QOSM_DBG_ERROR((QOSM_PRNTBUF,"Invalid elemt(%d)...\n",cfm->hdr.elmId.elmnt));
                                 
            break;
      } 

      if (qoSmHiCfg == QO_SM_HI_CONFIGURED)
      {
         qoSgwSendMsg(EVTQOTUCLCFGDONE);
      }
      else
      {
          QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw CfgCfm Pending...\n"));
      }
   }
   else
   {
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw CfgCfm .NOT OK..\n"));
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
*       File:  qosm_hicfg.c
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
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received control confirmation OK from TUCL...\n"));
   }
   else 
   {
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received control confirmation NOT OK from TUCL...\n"));
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
*       File:  qosm_hicfg.c
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
*       File:  qosm_hicfg.c
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
*       File:  qosm_hicfg.c
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
*       File:  qosm_hicfg.c
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

/**********************************************************************

         End of file:     qosm_hicfg.c@@/main/1 - Thu Jul  8 21:22:22 2010

**********************************************************************/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ad         1. LTE-CNE:SGW Initial Release
*********************************************************************91*/
