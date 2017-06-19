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

     Sid:      vbsm_szcfg.c@@/main/1 - Mon Jun 28 19:35:42 2010

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

#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "vbsm.x"          /*    */


PRIVATE S16 vbMmeLszGenCfgReq ARGS((Void));


PRIVATE S16 vbMmeLszLSapCfgReq ARGS((SuId        suId, 
                                     SpId        spId));


PRIVATE S16 vbMmeLszUSapCfgReq ARGS((SpId        spId));

PRIVATE S16 vbMmeLszProtCfgReq ARGS((Void ));

PRIVATE S16 vbMmeLszPeerCfgReq ARGS((Void));


#define VBSM_SZTSAP_CFG 0x4
#define VBSM_SCTSAP_CFG 0x8
#define VBSM_SZPEER_CFG 0x10

#define VBSM_SZ_CONFIGURED  (STSZGEN | STSZPROT | VBSM_SZTSAP_CFG | \
                             VBSM_SCTSAP_CFG | VBSM_SZPEER_CFG)
U8      vbSmSzCfg = 0;

/*
 *
 *       Fun:    vbMmeSzCfg - configure SB 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_szcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeSzCfg
(
Void
)
#else
PUBLIC Void vbMmeSzCfg()
#endif /* ANSI */
{
   SM_TRC2(vbMmeSzCfg);

   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sending MME S1AP configuration...\n"));
   vbSmSzCfg = 0;
   vbSmCb.szPst.event = EVTLSZCFGREQ;
   vbMmeLszGenCfgReq();
   vbMmeLszProtCfgReq();
   vbMmeLszLSapCfgReq (VBSM_SB_SPID, VBSM_SZ_SUID);
   vbMmeLszUSapCfgReq(VBSM_SZ_SPID);
   vbMmeLszPeerCfgReq();
   

   RETVOID;
}

/*
*
*       Fun:   vbMmeHdlGenCfgReq
*
*       Desc:  fill in genCfg for SZ
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_szcfg.c 
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLszGenCfgReq
(
Void
)
#else
PRIVATE S16 vbMmeLszGenCfgReq()
#endif
{
   SzMngmt  szMgt;
   SzGenCfg    *cfg;

   SM_TRC2(vbMmeLszGenCfgReq);

   cmMemset((U8 *)&szMgt, 0, sizeof(SzMngmt));
   vbSmDefHdr(&szMgt.hdr, ENTSZ, STSZGEN, VBSM_SZSMSEL);

   cfg   = &(szMgt.u.cfg.u.gen);

   cfg->nmbSztSaps = (S16)VBSM_LSZ_MAX_SZTSAP;
   cfg->nmbSctSaps = (S16)VBSM_LSZ_MAX_SCTSAP;
   cfg->nmbPeer = LVB_MAX_ENBS;
   cfg->nmbConn = LVB_MAX_ENBS * vbSmCb.cfgCb.ueNum;
   cfg->timeRes = (Ticks)VBSM_LSZ_TIMERES;
#if (defined(SZ_MME) && defined(SZ_DYNAMIC_PEER))
   cfg->dynamPeerFlag = TRUE;
#endif

   cfg->lmPst.srcProcId = SFndProcId();
   cfg->lmPst.dstProcId = SFndProcId();
   cfg->lmPst.srcEnt = (Ent)ENTSZ;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;

   cfg->lmPst.prior = (Prior)VBSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = (Region)vbSmCb.init.region;
   cfg->lmPst.pool = (Pool)vbSmCb.init.pool;
   cfg->lmPst.selector = (Selector)VBSM_SZSMSEL;

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&vbSmCb.szPst, &szMgt);
    VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP Gen cfg....\n"));
   RETVALUE(ROK);
}/* vbMmeLszGenCfgReq */

/*
*
*       Fun:   vbMmeLszLSapCfgReq
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_szcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLszLSapCfgReq
(
SuId        suId,             /* S1AP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PRIVATE S16 vbMmeLszLSapCfgReq(suId, spId)
SuId        suId;             /* S1AP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   SzMngmt  szMgt;
   SzSctSapCfg  *cfg;

   SM_TRC2(vbMmeLszLSapCfgReq);

   cmMemset((U8 *)&szMgt, 0, sizeof(SzMngmt));
   vbSmDefHdr(&szMgt.hdr, ENTSZ, STSZSCTSAP, VBSM_SZSMSEL);

   cfg   = &(szMgt.u.cfg.u.sctSap);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = VBSM_SZSBSEL;
   cfg->mem.region   = vbSmCb.init.region;
   cfg->mem.pool     = vbSmCb.init.pool;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SFndProcId();
   cfg->dstEntId     = ENTSB;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = VBSM_LSZ_VAL_10;
   cfg->maxBndRetry = VBSM_LSZ_VAL_3;

#ifdef LSZV1
   /* vb005.101: Fill source address info */
   cfg->srcAddr.sctPort = vbSmCb.cfgCb.mmeSctPort;
   cfg->srcAddr.ipAddr.nmb = 1;
   cfg->srcAddr.ipAddr.nAddr[0].type = CM_IPV4ADDR_TYPE;
   cfg->srcAddr.ipAddr.nAddr[0].u.ipv4NetAddr = vbSmCb.cfgCb.mmeIpAddr;
#endif /* LSZV1 */

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&vbSmCb.szPst, &szMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP LSap Cfg...\n"));
   RETVALUE(ROK);
}/* vbMmeLszLSapCfgReq */

/*
*
*       Fun:   vbMmeLszUSapCfgReq
*
*       Desc:  Call handler for SSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_szcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLszUSapCfgReq
(
SpId        spId              /* Service Provider Sap ID */
)
#else
PRIVATE S16 vbMmeLszUSapCfgReq(spId)
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   SzMngmt  szMgt;
   SzSztSapCfg    *cfg;

   SM_TRC2(vbMmeLszUSapCfgReq);

   cmMemset((U8 *)&szMgt, 0, sizeof(SzMngmt));
   vbSmDefHdr(&szMgt.hdr, ENTSZ, STSZSZTSAP, VBSM_SZSMSEL);

   cfg = &(szMgt.u.cfg.u.sztSap);

   cfg->spId   = spId;
   cfg->selector     = VBSM_SZVBSEL;
   cfg->mem.region   = vbSmCb.init.region;
   cfg->mem.pool     = vbSmCb.init.pool;
   cfg->priority     = PRIOR0;
   cfg->route        = RTESPEC;

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&vbSmCb.szPst, &szMgt);
    VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP USap cfg ...\n"));
   RETVALUE(ROK);
}/* vbMmeLszUSapCfgReq */

/*
*
*       Fun:   vbMmeLszProtCfgReq
*
*       Desc:  Call handler for Protocol Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_szcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLszProtCfgReq
(
Void
)
#else
PRIVATE S16 vbMmeLszProtCfgReq()
#endif
{
   SzMngmt  szMgt;
   SzProtCfg     *protCfg;

   SM_TRC2(vbMmeLszProtCfgReq);

   cmMemset((U8 *)&szMgt, 0, sizeof(SzMngmt));
   vbSmDefHdr(&szMgt.hdr, ENTSZ, STSZPROT, VBSM_SZSMSEL);

   protCfg = &(szMgt.u.cfg.u.prot);

   /* vb005.101: Fill source address info */
#ifndef LSZV1
   protCfg->srcAddr.ipAddr.nmb  =   1;
   protCfg->srcAddr.ipAddr.nAddr[0].type  =   CM_IPV4ADDR_TYPE;
   protCfg->srcAddr.ipAddr.nAddr[0].u.ipv4NetAddr   =  vbSmCb.cfgCb.mmeIpAddr;
   protCfg->srcAddr.sctPort =   vbSmCb.cfgCb.mmeSctPort;
#endif
   protCfg->nodeType =   LSZ_NODE_MME;
   protCfg->u.mmeId.mmeCode[0]     =   VBSM_LSZ_MME_PRC1;

   /* Send the request to the LM */
   (Void) SmMiLszCfgReq(&vbSmCb.szPst, &szMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP Prot Cfg...\n"));
   RETVALUE(ROK);
}/* vbMmeLszProtCfgReq */

/*
*
*       Fun:   vbMmeLszPeerCfgReq
*
*       Desc:  Call handler for Peer Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_szcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLszPeerCfgReq
(
Void
)
#else
PRIVATE S16 vbMmeLszPeerCfgReq()
#endif
{
   SzMngmt        szMgt;
   SzPeerCfg      *cfg = NULLP;

   SM_TRC2(vbMmeLszPeerCfgReq);

   cmMemset((U8 *)&szMgt, 0, sizeof(SzMngmt));
   vbSmDefHdr(&szMgt.hdr, ENTSZ, STSZPEER, VBSM_SZSMSEL);

   cfg = &(szMgt.u.cfg.u.peer);
   cfg->nmbPeers = LVB_MAX_ENBS;

   VBSM_ALLOC(&(cfg->peerInfo), (sizeof(SzPeerInfo)* cfg->nmbPeers));
   if(cfg->peerInfo == NULLP)
   {
      RETVALUE(RFAILED);
   }

   cfg->peerInfo[0].type        = LSZ_NODE_ENB;
   cfg->peerInfo[0].peerId.pres = TRUE;
   cfg->peerInfo[0].peerId.val  = LVB_ENB_PEER_ID_0;
   cfg->peerInfo[0].uSapId      = VBSM_SZ_SPID;
   cfg->peerInfo[0].sapId       = VBSM_SZ_SUID;
   cfg->peerInfo[0].assocCfg.nmbRetry          = VBSM_LSZ_VAL_3;
   cfg->peerInfo[0].assocCfg.tmrAssocRecov.enb = TRUE;
   cfg->peerInfo[0].assocCfg.tmrAssocRecov.val = VBSM_LSZ_VAL_50;
   cfg->peerInfo[0].assocCfg.locOutStrms       = VBSM_LSZ_OUT_STRMS;
   cfg->peerInfo[0].assocCfg.priDstAddr.type   = CM_IPV4ADDR_TYPE;
   cfg->peerInfo[0].assocCfg.dstPort = vbSmCb.cfgCb.eNbPort;
   cfg->peerInfo[0].assocCfg.priDstAddr.u.ipv4NetAddr = \
         vbSmCb.cfgCb.eNbIpAddr;
   cfg->peerInfo[0].assocCfg.dstAddrLst.nmb =  1;
   cfg->peerInfo[0].assocCfg.dstAddrLst.nAddr[0].type = CM_IPV4ADDR_TYPE;
   cfg->peerInfo[0].assocCfg.dstAddrLst.nAddr[0].u.ipv4NetAddr  = \
         vbSmCb.cfgCb.eNbIpAddr;

   cfg->peerInfo[1].type        = LSZ_NODE_ENB;
   cfg->peerInfo[1].peerId.pres = TRUE;
   cfg->peerInfo[1].peerId.val  = LVB_ENB_PEER_ID_1;
   cfg->peerInfo[1].uSapId      = VBSM_SZ_SPID;
   cfg->peerInfo[1].sapId       = VBSM_SZ_SUID;
   cfg->peerInfo[1].assocCfg.nmbRetry          = VBSM_LSZ_VAL_3;
   cfg->peerInfo[1].assocCfg.tmrAssocRecov.enb = TRUE;
   cfg->peerInfo[1].assocCfg.tmrAssocRecov.val = VBSM_LSZ_VAL_50;
   cfg->peerInfo[1].assocCfg.locOutStrms       = VBSM_LSZ_OUT_STRMS;
   cfg->peerInfo[1].assocCfg.priDstAddr.type   = CM_IPV4ADDR_TYPE;
   cfg->peerInfo[1].assocCfg.dstPort = vbSmCb.cfgCb.eNbPort_2;
   cfg->peerInfo[1].assocCfg.priDstAddr.u.ipv4NetAddr = \
         vbSmCb.cfgCb.eNbIpAddr_2;
   cfg->peerInfo[1].assocCfg.dstAddrLst.nmb =  1;
   cfg->peerInfo[1].assocCfg.dstAddrLst.nAddr[0].type = CM_IPV4ADDR_TYPE;
   cfg->peerInfo[1].assocCfg.dstAddrLst.nAddr[0].u.ipv4NetAddr  = \
         vbSmCb.cfgCb.eNbIpAddr_2;

   (Void) SmMiLszCfgReq(&vbSmCb.szPst, &szMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP Peer Cfg...\n"));
   RETVALUE(ROK);
}/* vbMmeLszPeerCfgReq */

/*
 *      FUN:   vbMmeSzLSapBndReq
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_szcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSzLSapBndReq
(
Void
)
#else
PUBLIC S16 vbMmeSzLSapBndReq()
#endif /* ANSI */
{
   SzMngmt  cntrl;
   S16                  ret = ROK;
   SM_TRC2(vbMmeSzLSapBndReq);
   cmMemset((U8 *)&cntrl, 0, sizeof(SzMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTSZ, STSZSCTSAP, VBSM_SZSMSEL);

   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;
   cntrl.u.cntrl.u.sap.id       = VBSM_SZ_SUID; 

   vbSmCb.szPst.event = EVTLSZCNTRLREQ;
   ret = SmMiLszCntrlReq(&vbSmCb.szPst, &cntrl);
   if(ret != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"ERROR: Sending MME S1AP Control Req to bind LSap."));
   }
   else
   {
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP Control Req to bind LSap...\n"));
   }
   RETVALUE(ret);
}

/*
 *      FUN:   vbMmeSzShutDwn
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_szcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSzShutDwn
(
Void
)
#else
PUBLIC S16 vbMmeSzShutDwn()
#endif /* ANSI */
{
   SzMngmt  cntrl;
   S16                  ret = ROK;
   SM_TRC2(vbMmeSzShutDwn);
   cmMemset((U8 *)&cntrl, 0, sizeof(SzMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTSZ, STSZGEN, VBSM_SZSMSEL);

   cntrl.u.cntrl.action        = ASHUTDOWN;
   cntrl.u.cntrl.subAction     = SAELMNT;

   vbSmCb.szPst.event = EVTLSZCNTRLREQ;
   ret = SmMiLszCntrlReq(&vbSmCb.sbPst, &cntrl);
   if(ret != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"ERROR: Sending MME S1AP Control Req ShutDown S1AP layer...."));
   }
   else
   {
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent MME S1AP control Req to ShutDown S1AP layer...\n"));
   }
   RETVALUE(ret);
}

/*
*
*       Fun:   Configuration Confirm
*
*       Desc:  This function is used by to present configuration confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  szac_lszcb.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLszCfgCfm
(
Pst     *pst,          /* post structure */
SzMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLszCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
SzMngmt *cfm;          /* configuration */
#endif
{
   SM_TRC2(SmMiLszCfgCfm);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP CfgCfm with elmnt(%d) - status(%d)...\n",cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
       switch (cfm->hdr.elmId.elmnt)
       {
          case STSZGEN:
               vbSmSzCfg |= cfm->hdr.elmId.elmnt;
               VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP Gen CfgCfm...\n"));
               break;
          case STSZPROT:
               vbSmSzCfg |= cfm->hdr.elmId.elmnt;
               VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP Prot CfgCfm...\n"));
               break;
          case STSZSZTSAP:
               vbSmSzCfg |= VBSM_SZTSAP_CFG;
               VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP TSap CfgCfm...\n"));
               break;
          case STSZSCTSAP:
               vbSmSzCfg |= VBSM_SCTSAP_CFG;
               VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP Sct CfgCfm...\n"));
               break;
          case STSZPEER:
               vbSmSzCfg |= VBSM_SZPEER_CFG;
              VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP Peer CfgCfm...\n"));
               break;
          default:
              VBSM_DBG_ERROR((VBSM_PRNTBUF,"Received MME S1AP CfgCfm. with invalid elmnt(%d)..\n",cfm->hdr.elmId.elmnt));
               break;
       }

       if (vbSmSzCfg == VBSM_SZ_CONFIGURED)
       {
          vbMmeSendMsg(EVTVBS1APCFGDONE);
       }
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP Configuration confirmation NOT OK...\n"));
   }

   RETVALUE(ROK);
} /* end of SmMiLszCfgCfm */

/*
*
*       Fun:   Control Confirm
*
*       Desc:  This function is used by to present control confirm
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  szac_lszcb.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLszCntrlCfm
(
Pst     *pst,          /* post structure */
SzMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLszCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
SzMngmt *cfm;          /* control */
#endif
{
   SM_TRC2(SmMiLszCntrlCfm);
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP control confirmation ok from S1AP \n"));
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received MME S1AP control confirmation NOT OK  from S1AP \n"));
   }

   RETVALUE(ROK);
} /* end of SmMiLszCntrlCfm */

/*
*
*       Fun:   Status Indication
*
*       Desc:  This function is used by to present  unsolicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  szac_lszcb.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLszStaInd
(
Pst     *pst,           /* post structure */
SzMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLszStaInd(pst, usta)
Pst     *pst;           /* post structure */
SzMngmt *usta;          /* unsolicited status */
#endif
{
   SM_TRC2(SmMiLszStaInd);

   UNUSED(pst);
   if ((usta->u.usta.alarm.event == LSZ_EVENT_LI_ENDP_OPEN_CFM) &&
       (usta->u.usta.alarm.cause  == LSZ_CAUSE_SCT_ENPOPEN_SUCC))
   {
      vbMmeSendMsg(EVTVBS1APBINDCFM);
   }
   RETVALUE(ROK);
} /* end of SmMiLszStaInd */

/*
*
*       Fun:   Trace Indication
*
*       Desc:  This function is used to present trace
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  szac_lszcb.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLszTrcInd
(
Pst *pst,               /* post structure */
SzMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLszTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
SzMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   SM_TRC2(SmMiLszTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLszTrcInd */

/*
*
*       Fun:   Statistics Confirm
*
*       Desc:  This function is used to present solicited statistics 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  szac_lszcb.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLszStsCfm
(
Pst       *pst,         /* post structure */
SzMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLszStsCfm(pst, sts)
Pst       *pst;         /* post structure */
SzMngmt   *sts;         /* confirmed statistics */
#endif
{
   SM_TRC2(SmMiLszStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLszStsCfm */

/*
*
*       Fun:   Status Confirm
*
*       Desc:  This function is used to present solicited status 
*              information to Layer Management.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  szac_lszcb.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLszStaCfm
(
Pst     *pst,           /* post structure */
SzMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLszStaCfm(pst, sta)
Pst     *pst;           /* post structure */
SzMngmt *sta;            /* confirmed status */
#endif
{
   SM_TRC2(SmMiLszStaCfm);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLszStaCfm */


/********************************************************************30**
 
         End of file:     vbsm_szcfg.c@@/main/1 - Mon Jun 28 19:35:42 2010

*********************************************************************31*/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     ad         1. Initial version for LTE-CNE Phase 1.
/main/1  vb005.101   rk         1. S1AP 3.1 release support
*********************************************************************91*/
