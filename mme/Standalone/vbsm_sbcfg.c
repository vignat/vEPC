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

     Sid:      vbsm_sbcfg.c@@/main/1 - Mon Jun 28 19:35:42 2010

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
#include <unistd.h>         /* vb005.101: ccpu00117528  root checking */
#include <stdlib.h>         
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
/*  vb007.101 : s1ap include file order has changed */ 
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "vbsm.x"          /*    */

#define VB_SM_SB_CONFIGURED  (STSBGEN | STSBTSAP | VBSM_SBSCTSAP_CFG) 

U8      vbSmSbCfg = 0;

/* Function prototypes */
PRIVATE Void vbMmeLsbSbGenCfg ARGS ((Void));

PRIVATE Void vbMmeLsbSbTSAPCfg ARGS ((SpId   spId,
                                      SuId   suId));

PRIVATE Void vbMmeLsbSbSctSapCfg ARGS ((SpId spId));

/*
 *
 *       Fun:    vbMmeSbCfg - configure SB 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeSbCfg
(
Void
)
#else
PUBLIC Void vbMmeSbCfg()
#endif /* ANSI */
{
   SM_TRC2(vbMmeSbCfg);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sending MME Sb configuration...\n"));
   vbSmSbCfg = 0;
   vbSmCb.sbPst.event = LSB_EVTCFGREQ;
   vbMmeLsbSbGenCfg();
   vbMmeLsbSbTSAPCfg (VBSM_SB_HI_SPID, VBSM_SB_SUID);
   vbMmeLsbSbSctSapCfg(VBSM_SB_SPID);

   RETVOID;
}

/*
 *
 *       Fun:    vbMmeLsbSbGenCfg - fill in default genCfg for SB
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
         File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLsbSbGenCfg
(
Void
)
#else
PRIVATE Void vbMmeLsbSbGenCfg()
#endif /* ANSI */
{
   SbMgmt      sbMgt;
   SbGenCfg    *cfg;

   SM_TRC2(vbMmeLsbSbGenCfg);

   cmMemset((U8 *)&sbMgt, 0, sizeof(SbMgmt));
   vbSmDefHdr(&sbMgt.hdr, ENTSB, STSBGEN, VBSM_SBSMSEL);

   cfg = &sbMgt.t.cfg.s.genCfg;
   /* vb005.101 : ccpu00117528 service type configured */
   if(vbSmCb.cfgCb.sctpUdpServiceType == 0)
   {
      /* vb005.101: SCTP service type is changed to
       * RAW_SCTP_PRIOR & root checking */
      cfg->serviceType   = HI_SRVC_RAW_SCTP_PRIOR;
      if (getuid())
      {
         VBLOGERROR(ERRCLS_DEBUG,0,ERRZERO,
               "You must be root to open SCTP Association!\n ");

         exit(0);
      }

   }
   else
   {
      cfg->serviceType   = HI_SRVC_UDP;
   }


#ifdef LSB5
   cfg->srcUdpPort = 9898;
   cfg->dstUdpPort = 9898;
#endif /*LSB5*/

   cfg->maxNmbSctSaps     = 1;
   cfg->maxNmbTSaps       = 5;
   /* vb006.101: MULTIPLE_ENB Support */
   cfg->maxNmbAssoc       = 5;
   cfg->maxNmbEndp        = 2;
   cfg->maxNmbDstAddr     = (2 * cfg->maxNmbAssoc);
   cfg->maxNmbSrcAddr     = 10;
   cfg->maxNmbTxChunks    = 95000;
   cfg->maxNmbRxChunks    = 95000;
   cfg->maxNmbInStrms     = 10;
   cfg->maxNmbOutStrms    = 10;
   cfg->initARwnd         = 16000;
   cfg->mtuInitial        = 1500;
   cfg->mtuMinInitial     = 1500;
   cfg->mtuMaxInitial     = 1500;
   cfg->performMtu        = FALSE;
   cfg->timeRes           = 1;
   cfg->reConfig.maxInitReTx   = 8;
   cfg->reConfig.maxAssocReTx  = 20;
   /* vb006.101: Changed the value from 4 to 1 */
   cfg->reConfig.maxPathReTx   = 20;
   cfg->reConfig.altAcceptFlg  = FALSE;
   cfg->reConfig.keyTm         = 600;
   cfg->reConfig.alpha         = 12;
   cfg->reConfig.beta          = 25;

   cfg->smPst.srcProcId = SFndProcId();
   cfg->smPst.dstProcId = SFndProcId();
   cfg->smPst.srcEnt = (Ent)ENTSB;
   cfg->smPst.dstEnt = (Ent)ENTSM;
   cfg->smPst.srcInst = (Inst)0;
   cfg->smPst.dstInst = (Inst)0;

   cfg->smPst.prior = (Prior)VBSM_MSGPRIOR;
   cfg->smPst.route = (Route)RTESPEC;
   cfg->smPst.event = (Event)EVTNONE;
   cfg->smPst.region = (Region)vbSmCb.init.region;
   cfg->smPst.pool = (Pool)vbSmCb.init.pool;
   cfg->smPst.selector = (Selector)VBSM_SBSMSEL;

   (Void)SmMiLsbCfgReq(&vbSmCb.sbPst, &sbMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME sbGenCfg...\n"));
   RETVOID;
} /* end of vbMmeLsbSbGenCfg() */

/*
 *
 *       Fun:    vbMmeLsbSbTSAPCfg - fill in default SCTP TSAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLsbSbTSAPCfg
(
SpId        spId,
SuId        suId
)
#else
PRIVATE Void vbMmeLsbSbTSAPCfg(spId, suId)
SpId        spId;
SuId        suId;
#endif /* ANSI */
{
   SbMgmt      sbMgt;
   SbTSapCfg   *cfg;

   SM_TRC2(vbMmeLsbSbTSAPCfg);

   cmMemset((U8 *)&sbMgt, 0, sizeof(SbMgmt));
   vbSmDefHdr(&sbMgt.hdr, ENTSB, STSBTSAP, VBSM_SBSMSEL);

   cfg = &(sbMgt.t.cfg.s.tSapCfg);
   cfg->swtch        = LSB_SW_RFC_REL0;
   cfg->sel          = VBSM_SBHISEL;      /* Loosely coupled upper layer */
   cfg->ent          = ENTHI;              /* Loosely coupled upper layer */
   cfg->inst         = 0;                  /* Loosely coupled upper layer */
   cfg->procId       = SFndProcId();       /* Loosely coupled upper layer */
   cfg->memId.region = vbSmCb.init.region;
   cfg->memId.pool   = vbSmCb.init.pool;
   cfg->prior = PRIOR0;
   cfg->route = RTESPEC;
   cfg->suId  = suId;
   cfg->srcNAddrLst.nmb = 1;
   cfg->srcNAddrLst.nAddr[0].type = CM_NETADDR_IPV4;
   cfg->srcNAddrLst.nAddr[0].u.ipv4NetAddr = vbSmCb.cfgCb.mmeIpAddr;
   cfg->reConfig.spId         = spId;
   cfg->reConfig.tIntTmr      = 200;
   cfg->reConfig.maxBndRetry  = 2;

   (Void)SmMiLsbCfgReq(&vbSmCb.sbPst, &sbMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME sbTSap Configuration...\n"));
   RETVOID;
}

/*
 *
 *       Fun:    vbMmeLsbSbSctSapCfg - fill in default SCTP SAP
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void vbMmeLsbSbSctSapCfg
(
SpId          spId
)
#else
PRIVATE Void vbMmeLsbSbSctSapCfg(spId)
SpId          spId;
#endif /* ANSI */
{
   SbMgmt        sbMgt;
   SbSctSapCfg   *cfg;

   SM_TRC2(vbMmeLsbSbSctSapCfg);

   cmMemset((U8 *)&sbMgt, 0, sizeof(SbMgmt));
   vbSmDefHdr(&sbMgt.hdr, ENTSB, STSBSCTSAP, VBSM_SBSMSEL);

   cfg = &(sbMgt.t.cfg.s.sctSapCfg);
   cfg->swtch        = LSB_SW_RFC_REL0;
   cfg->sel          = VBSM_SBSZSEL;     /* Loosely coupled upper layer */
   cfg->memId.region = vbSmCb.init.region;
   cfg->memId.pool   = vbSmCb.init.pool;
   cfg->prior = PRIOR0;
   cfg->route = RTESPEC;
   cfg->spId  = spId;
   cfg->reConfig.maxAckDelayTm = 10;  /* increased from 2;
                                        Equivalent of 200 ms, TBD*/
   cfg->reConfig.maxAckDelayDg = 1;
   cfg->reConfig.rtoInitial = 20;
   cfg->reConfig.rtoMin     = 10;
   cfg->reConfig.rtoMax     = 50;
   cfg->reConfig.cookieLife = 50000;   /* increased from 500 */
   /* vb006.101: Changed the value from FALSE to TRUE */
   cfg->reConfig.hBeatEnable= TRUE;
   /* vb006.101: Changed the value from 500 to 1 */
   cfg->reConfig.intervalTm = 100;
   cfg->reConfig.freezeTm   = 15;
   cfg->reConfig.flcUpThr   = 80000;    /* number of packets */
   cfg->reConfig.flcLowThr  = 30000;
   cfg->reConfig.negAbrtFlg = TRUE;
   cfg->reConfig.handleInitFlg = TRUE; 
#ifdef LSB4
   cfg->reConfig.bundleTm = 1;
#endif /* LSB4 */
#ifdef LSB8
   cfg->reConfig.maxBurst = 4;
   cfg->reConfig.maxHbBurst = 1;
   cfg->reConfig.t5SdownGrdTm = 5 * cfg->reConfig.rtoMax;
#endif /* LSB8 */

   (Void)SmMiLsbCfgReq(&vbSmCb.sbPst, &sbMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME sctSap configuration...\n"));
   RETVOID;
}

/*
 *
 *       Fun:    vbMmeLsbSbTSapCntrlReq
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeLsbSbTSapCntrlReq
(
SpId        spId
)
#else
PUBLIC Void vbMmeLsbSbTSapCntrlReq(spId)
SpId        spId;
#endif /* ANSI */
{
   SbMgmt               sbMgt;

   SM_TRC2(vbMmeLsbSbTSapCntrlReq);

   cmMemset((U8 *)&sbMgt, 0, sizeof(SbMgmt));
   vbSmDefHdr(&sbMgt.hdr, ENTSB, STSBTSAP, VBSM_SBSMSEL);
   sbMgt.hdr.msgType     = TCNTRL;
   sbMgt.t.cntrl.action  = ABND_ENA;
   sbMgt.t.cntrl.subAction  = SAELMNT;
   sbMgt.t.cntrl.sapId    = spId;

   vbSmCb.sbPst.event = LSB_EVTCNTRLREQ;
   (Void)SmMiLsbCntrlReq(&vbSmCb.sbPst, &sbMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent ControlReq to enable MME sbTSap...\n"));

   RETVOID;
}

/*
 *      FUN:   vbMmeSbShutDwn
 *
 *      Desc:  Brings SCTP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_sbcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSbShutDwn
(
Void
)
#else
PUBLIC S16 vbMmeSbShutDwn()
#endif /* ANSI */
{
   SbMgmt               sbMgt;
   S16                  ret = ROK;
   SM_TRC3(vbMmeSbShutDwn);
   cmMemset((U8 *)&sbMgt, 0, sizeof(SbMgmt));
   vbSmDefHdr(&sbMgt.hdr, ENTSB, STSBGEN, VBSM_SBSMSEL);

   sbMgt.t.cntrl.action = ASHUTDOWN;

   vbSmCb.sbPst.event = LSB_EVTCNTRLREQ;
   (Void)SmMiLsbCntrlReq(&vbSmCb.sbPst, &sbMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Control request to shutdowm MME SCTP...\n"));

   RETVALUE(ret);
}

/*
 *
 *       Fun:    SmMiLsbCfgCfm - configuration confirm
 *
 *       Desc:   prints the config confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbCfgCfm
(
Pst         *pst,
SbMgmt     *cfg
)
#else
PUBLIC S16 SmMiLsbCfgCfm(pst, cfg)
Pst         *pst;
SbMgmt     *cfg;
#endif /* ANSI */
{

   SM_TRC3(SmMiLsbCfgCfm);

   VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME SCTP cfgCfm with elmnt(%d) - status(%d)...\n",cfg->hdr.elmId.elmnt,cfg->cfm.status));
   if (cfg->cfm.status == LCM_PRIM_OK)
   {
      switch (cfg->hdr.elmId.elmnt)
      {
         case STSBGEN:
         case STSBTSAP:
         {
            vbSmSbCfg |=  cfg->hdr.elmId.elmnt;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME SCTP gen/tSap CfgCfm...\n"));
            break;
         }
         case STSBSCTSAP:
         {
            vbSmSbCfg |=  VBSM_SBSCTSAP_CFG;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME SCTP sctSap CfgCfm...\n"));
            break;
         }
         default:
           VBSM_DBG_ERROR((VBSM_PRNTBUF,"Invalid elemt(%d)...\n",cfg->hdr.elmId.elmnt));
            break;
      }

      if (vbSmSbCfg == VB_SM_SB_CONFIGURED)
      {
         vbMmeSendMsg(EVTVBSCTPCFGDONE);
      }
      else
      {
          VBSM_DBG_INFO((VBSM_PRNTBUF,"MME SCTP CfgCfm Pending...\n"));
      }
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME SCTP CfgCFm NOT OK...\n"));
   }

   RETVALUE(ROK);
} /* end of SmMiLsbCfgCfm() */

/*
 *
 *       Fun:    SmMiLsbStsCfm - statistics confirm
 *
 *       Desc:   prints the statistics confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbStsCfm
(
Pst                  *pst,
SbMgmt               *sts
)
#else
PUBLIC S16 SmMiLsbStsCfm(pst, sts)
Pst                  *pst;
SbMgmt               *sts;
#endif /* ANSI */
{
   SM_TRC3(SmMiLsbStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLsbStsCfm() */

/*
 *
 *       Fun:    SmMiLsbStaCfm - status confirm
 *
 *       Desc:   prints the status confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbStaCfm
(
Pst                  *pst,
SbMgmt               *ssta
)
#else
PUBLIC S16 SmMiLsbStaCfm(pst, ssta)
Pst                  *pst;
SbMgmt               *ssta;
#endif /* ANSI */
{
   SM_TRC3(SmMiLsbStaCfm);

   UNUSED(pst);
   UNUSED(ssta);

   RETVALUE(ROK);
} /* end of SmMiLsbStaCfm() */

/*
 *
 *       Fun:    SmMiLsbCntrlCfm - control confirm
 *
 *       Desc:   prints the control confirm status
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbCntrlCfm
(
Pst         *pst,
SbMgmt     *cntrl
)
#else
PUBLIC S16 SmMiLsbCntrlCfm(pst, cntrl)
Pst         *pst;
SbMgmt     *cntrl;
#endif /* ANSI */
{
   SM_TRC3(SmMiLsbCntrlCfm);

   if (cntrl->cfm.status == LCM_PRIM_OK)
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"\nReceived control confirmation ok from SCTP \n"));
      vbMmeSendMsg(EVTVBSCTPBINDCFM);
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME SCTP Control Cfm NOT OK...\n"));
   }
 
   RETVALUE(ROK);
} /* end of SmMiLsbCntrlCfm() */

/*
 *
 *       Fun:    SmMiLsbStaInd - status indication
 *
 *       Desc:   prints the status indication (alarm)
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbStaInd
(
Pst         *pst,
SbMgmt     *usta
)
#else
PUBLIC S16 SmMiLsbStaInd(pst, usta)
Pst         *pst;
SbMgmt     *usta;
#endif /* ANSI */
{
   TRC3(SmMiLsbStaInd);

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);
} /* end of SmMiLsbStaInd() */

/*
 *
 *       Fun:    SmMiLsbTrcInd - trace indication
 *
 *       Desc:   prints the trace indication
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
 *       File:   vbsm_sbcfg.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLsbTrcInd
(
Pst                  *pst,
SbMgmt               *trc
)
#else
PUBLIC S16 SmMiLsbTrcInd(pst, trc)
Pst                  *pst;
SbMgmt               *trc;
#endif /* ANSI */
{
   TRC3(SmMiLsbTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLsbTrcInd() */

/********************************************************************30**
 
         End of file:     vbsm_sbcfg.c@@/main/1 - Mon Jun 28 19:35:42 2010

*********************************************************************31*/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ad         1. Initial version for LTE-CNE Phase 1.
/main/1   vb004.101  ve         1. SCTP service type is changed to RAW_SCTP_PRIOR.
/main/1   vb005.101  ms         1. service type configured and root checking.
/main/1   vb006.101 ragrawal    1. MULTIPLE_ENB Support.
                                2. Changed the values of some macro. 
/main/1   vb007.101 akaranth    1. s1ap include file order has changed. 
*********************************************************************91*/
