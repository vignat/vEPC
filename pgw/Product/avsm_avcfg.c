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

     Name:    Trillium LTE CNE - MME Stack Module

     Type:    C Include file

     Desc:    This file contains the stack manager source code

     File:    jgsm_avcfg.c

     Sid:      avsm_avcfg.c@@/main/1 - Tue Aug 17 18:54:36 2010

     Prg:     sp     
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
#include "cm_dns.h"         /* common DNS libraru defines */

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
#include "cm_dns.x"         /* common DNS libraru defines */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "lav.x"           /* CNE Layer management           */
#include "avsm.x"



/* AV_TODO_SECOND_PHASE - Expect a separate config request to configure
   the EGT SAP at MME. Hence should add another macro to track the
   confirmation of the same SAP similar to SZT SAP. */

#define AV_PGW_CONFIGURED  (STAVGEN | STAVPEER | STAVEGTPUSAP \
                           | STAVEGTPCSAP | STAVECHOSAP)

PRIVATE U8      avPgwCfg = 0;

PRIVATE Void avPgwLavGenCfg ARGS ((Void));

PRIVATE Void avPgwLavPeerCfg ARGS ((Void));

PRIVATE Void avPgwLavLSAPCfg ARGS ((SuId        suId,
                                    SpId        spId,
                                    Elmnt       elem));

PRIVATE S16 avPgwEnaUstaReq ARGS((Void));

/*
 *
 *       Fun:    avPgwLavCfg - configure PGW
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_avcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void avPgwLavCfg
(
Void
)
#else
PUBLIC Void avPgwLavCfg()
#endif /* ANSI */
{
   SM_TRC2(avPgwLavCfg);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sending PGW Av configuration...\n"));
   avPgwLavGenCfg();
   avPgwLavPeerCfg();
   avPgwLavLSAPCfg(AVSM_S5S8_C_SPID, AVSM_S5S8_C_SUID, STAVEGTPCSAP);
   avPgwLavLSAPCfg(AVSM_ECHO_SPID, AVSM_ECHO_SUID, STAVECHOSAP);
   avPgwLavLSAPCfg(AVSM_S5S8_U_SPID, AVSM_S5S8_U_SUID, STAVEGTPUSAP);

   RETVOID;
}
/*
 *
 *       Fun:    avPgwLavGenCfg - fill in default genCfg for AV
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_avcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void avPgwLavGenCfg
(
Void
)
#else
PRIVATE Void avPgwLavGenCfg()
#endif /* ANSI */
{
   AvMngmt  avPgwMgt;
   AvPgwGenCfg  *cfg;

   SM_TRC2(avPgwLavGenCfg);

   cmMemset((U8 *)&avPgwMgt, 0, sizeof(AvMngmt));
   avSmDefHdr(&avPgwMgt.hdr, ENTEU, STAVGEN, AVSM_AVSMSEL);

   cfg   = &(avPgwMgt.u.cfg.s.pgwGenCfg);

   cfg->lmPst.srcProcId = SFndProcId();
   cfg->lmPst.dstProcId = SFndProcId();
   cfg->lmPst.srcEnt = (Ent)ENTEU;
   cfg->lmPst.dstEnt = (Ent)ENTSM;
   cfg->lmPst.srcInst = (Inst)0;
   cfg->lmPst.dstInst = (Inst)0;

   cfg->lmPst.prior = (Prior)AVSM_MSGPRIOR;
   cfg->lmPst.route = (Route)RTESPEC;
   cfg->lmPst.event = (Event)EVTNONE;
   cfg->lmPst.region = (Region)avSmCb.init.region;
   cfg->lmPst.pool = (Pool)avSmCb.init.pool;
   cfg->lmPst.selector = (Selector)AVSM_AVSMSEL;
   cfg->mem.region = (Region)avSmCb.init.region;
   cfg->mem.pool   = (Pool)avSmCb.init.pool;
   cfg->maxNmbEgtSaps = 3;
   cfg->maxNmbBearers = 10;
   cfg->maxNmbTunls   = avSmCb.cfgCb.pgwUeNum;
   cfg->tmrRes        = LAV_PGW_TMR_RES;
   cfg->pgwAddrCfg.selfAddr.type   = CM_TPTADDR_IPV4;
   cmInetAddr (avSmCb.cfgCb.pgwDfltIpAddr,
         &(cfg->pgwAddrCfg.selfAddr.u.ipv4TptAddr.address));
   cfg->pgwAddrCfg.selfAddr.u.ipv4TptAddr.address =
         CM_INET_NTOH_U32(cfg->pgwAddrCfg.selfAddr.u.ipv4TptAddr.address);
   cfg->pgwAddrCfg.selfAddr.u.ipv4TptAddr.port    = avSmCb.cfgCb.egUDfltPort;
   /* av004.101 Copying number of IPs configured at UeSim to PGW Gen Cfg */
   cfg->pgwAddrCfg.pgwNetAddr.nmb   = avSmCb.cfgCb.pgwIpNum; 
   cfg->pgwAddrCfg.pgwNetAddr.startAddr.type   = CM_TPTADDR_IPV4;
   cfg->pgwAddrCfg.pgwNetAddr.startAddr.u.ipv4NetAddr = avSmCb.cfgCb.pgwUeStartAddr;

   (Void)SmMiLavCfgReq(&avSmCb.avPgwPst, &avPgwMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW Av GenCfg...\n"));

   RETVOID;
} /* end of avPgwLavGenCfg() */

/*
 *
 *       Fun:    avPgwLavPeerCfg - fill in default peerCfg for AV
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_avcfg.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwLavPeerCfg
(
Void
)
#else
PRIVATE Void avPgwLavPeerCfg()
#endif /* ANSI */
{
   AvMngmt  avPgwMgt;
   AvPgwPeerCfg  *cfg;

   SM_TRC2(avPgwLavPeerCfg);

   cmMemset((U8 *)&avPgwMgt, 0, sizeof(AvMngmt));
   avSmDefHdr(&avPgwMgt.hdr, ENTEU, STAVPEER, AVSM_AVSMSEL);

   cfg   = &(avPgwMgt.u.cfg.s.peerCfg);

   /* Configure the external gatway address */
   cfg->exGwCfg.type = CM_INET_IPV4ADDR_TYPE;

   cmInetAddr ((S8 *)avSmCb.cfgCb.pgwDfltExgwIpAddr, &(cfg->exGwCfg.u.ipv4NetAddr));
   cfg->exGwCfg.u.ipv4NetAddr = CM_INET_NTOH_U32(cfg->exGwCfg.u.ipv4NetAddr);


   /* Configure peer (SGW details )*/
   cfg->sgwCfg.tptAddr.type = CM_TPTADDR_IPV4;
   cfg->sgwCfg.tptAddr.u.ipv4TptAddr.port = avSmCb.cfgCb.sgwPort;
   cfg->sgwCfg.tptAddr.u.ipv4TptAddr.address = avSmCb.cfgCb.sgwIpAddr;

   (Void)SmMiLavCfgReq(&avSmCb.avPgwPst, &avPgwMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW Av PeerCfg...\n"));

   RETVOID;
} /* end of avPgwLavPeerCfg() */

/*
 *
 *       Fun:    avPgwLavLSAPCfg - fill in default EGT Sap
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_avcfg.c
 *
 */

#ifdef ANSI
PRIVATE Void avPgwLavLSAPCfg
(
SuId        suId,
SpId        spId,
Elmnt       elem
)
#else
PRIVATE Void avPgwLavLSAPCfg(suId, spId, elem)
SuId        suId;
SpId        spId;
Elmnt       elem;
#endif /* ANSI */
{
   AvMngmt      avPgwMgt;
   AvPgwLSapCfg  *cfg;

   SM_TRC2(avPgwLavLSapCfg);

   cmMemset((U8 *)&avPgwMgt, 0, sizeof(AvMngmt));
   avSmDefHdr(&avPgwMgt.hdr, ENTEU, elem, AVSM_AVSMSEL);

   cfg   = &(avPgwMgt.u.cfg.s.pgwLSapCfg);

   cfg->suId   = suId;
   cfg->spId   = spId;

   cfg->selector     = AVSM_AVEGSEL;
   cfg->mem.region   = avSmCb.init.region;
   cfg->mem.pool     = avSmCb.init.pool;
   cfg->prior        = PRIOR0;
   cfg->route        = RTESPEC;
   cfg->dstProcId    = SFndProcId();
   cfg->dstEntId     = ENTEG;
   cfg->dstInstId    = (Inst)0;

   cfg->tBndTmr.enb = TRUE;
   cfg->tBndTmr.val = AVSM_LAV_VAL_10;
   cfg->maxBndRetry = AVSM_LAV_VAL_3;
   cfg->nmbConn   = avSmCb.cfgCb.pgwUeNum;

   (Void)SmMiLavCfgReq(&avSmCb.avPgwPst, &avPgwMgt);

   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW Av LSapCFg...\n"));
   RETVOID;
} /* end of avPgwLavLSapCfg() */



/*      FUN:  avPgwDbgMaskCntrlReq
 *
 *      Desc:  Enables debug with control request
 *
 *      Ret:   
 *
 *      Notes: None
 *
 *      File:  jgsm_avcfg.c
 *
 *
*/
#ifdef ANSI
PUBLIC S16 avPgwDbgMaskCntrlReq
(
Void
)
#else
PUBLIC S16  avPgwDbgMaskCntrlReq()
#endif /* ANSI */
{
/* av001.101: warning removal */
#ifdef DEBUGP
   AvMngmt  cntrl;
#endif

   SM_TRC2(avPgwDbgMaskCntrlReq);

   /* Enable alarm for PGW */
   avPgwEnaUstaReq();

#ifdef DEBUGP
   cmMemset((U8 *)&cntrl, 0, sizeof(AvMngmt));
   avSmDefHdr(&cntrl.hdr, ENTEU, STAVGEN, AVSM_AVSMSEL);

   cntrl.u.cntrl.u.pgwCntrl.action      = AENA;
   cntrl.u.cntrl.u.pgwCntrl.subAction   = SADBG;
   cntrl.u.cntrl.u.pgwCntrl.u.dbg.dbgMask = avSmCb.cfgCb.dbgMask;

   avSmCb.avPgwPst.event = EVTLAVCNTRLREQ;
   (Void)SmMiLavCntrlReq(&avSmCb.avPgwPst, &cntrl);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW control Req for debug...\n"));
#endif

   RETVALUE(ROK);
}


/*
 *      FUN:   avPgwEnaUstaReq
 *
 *      Desc:  Enables unsolicited status
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  avsm_avcfg.c
 *
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwEnaUstaReq
(
Void
)
#else
PRIVATE S16 avPgwEnaUstaReq()
#endif /* ANSI */
{
   AvMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(avPgwEnaUstaReq);

   cmMemset((U8 *)&cntrl, 0, sizeof(AvMngmt));
   avSmDefHdr(&cntrl.hdr, ENTEU, STAVGEN, AVSM_AVSMSEL);

   cntrl.u.cntrl.u.pgwCntrl.action       = AENA;
   cntrl.u.cntrl.u.pgwCntrl.subAction    = SAUSTA;

   avSmCb.avPgwPst.event = EVTLAVCNTRLREQ;
   (Void)SmMiLavCntrlReq(&avSmCb.avPgwPst, &cntrl);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW control Req to Unsolicited Sts...\n"));
   RETVALUE(ret);
}

/*
 *      FUN:   avPgwEgtSapBndReq
 *
 *      Desc:  Binds PGW with EGTP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  avsm_avcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtSapBndReq
(
S16 element
)
#else
PUBLIC S16 avPgwEgtSapBndReq(element)
S16 element;
#endif /* ANSI */
{
   AvMngmt  cntrl;
   SM_TRC2(avPgwEgtSapBndReq);
   cmMemset((U8 *)&cntrl, 0, sizeof(AvMngmt));

   avSmDefHdr(&cntrl.hdr, ENTEU, element, AVSM_AVSMSEL);

   cntrl.u.cntrl.u.pgwCntrl.action    = ABND;
   cntrl.u.cntrl.u.pgwCntrl.subAction = 0;

   switch(element)
   {
      case STAVEGTPCSAP:
         cntrl.u.cntrl.u.pgwCntrl.u.sap.id = AVSM_S5S8_C_SPID;
         break;
      case STAVEGTPUSAP:
         cntrl.u.cntrl.u.pgwCntrl.u.sap.id = AVSM_S5S8_U_SPID;
         break;
      case STAVECHOSAP:
         cntrl.u.cntrl.u.pgwCntrl.u.sap.id = AVSM_ECHO_SPID;
         break;
   }

   avSmCb.avPgwPst.event = EVTLAVCNTRLREQ;

   (Void)SmMiLavCntrlReq(&avSmCb.avPgwPst, &cntrl);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EgtSapBndREq...\n"));
   RETVALUE(ROK);
}

/*
 *      FUN:   avPgwSendArpReq
 *
 *      Desc:  Sends the control request to trigger ARP Request
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  avsm_avcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwSendArpReq
(
Void
)
#else
PUBLIC S16 avPgwSendArpReq(Void )
#endif /* ANSI */
{
   AvMngmt  cntrl;
   S16      ret = ROK;

   cmMemset((U8 *)&cntrl, 0, sizeof(AvMngmt));
   avSmDefHdr(&cntrl.hdr, ENTEU, STAVTRGARP, AVSM_AVSMSEL);

   (Void)SmMiLavCntrlReq(&avSmCb.avPgwPst, &cntrl);

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
*       File:  avsm_avcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavCfgCfm
(
Pst     *pst,          /* post structure */
AvMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLavCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
AvMngmt *cfm;          /* configuration */
#endif
{
   S16 ret = ROK;
   SM_TRC2(SmMiLavCfgCfm);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW Av CfgCfm with - status(%d)...\n",
            cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      avPgwCfg |=  cfm->hdr.elmId.elmnt;
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW Configuration Cfm...\n"));    

      /* If PGW Configuration is complete*/
      if(avPgwCfg == AV_PGW_CONFIGURED)
      {
          avPgwSendMsg(EVTAVPGWCFGDONE);
      }
      else
      {
         AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW Configuration PENDING...\n"));
         /* Debug print here that config is still pending */
      }
   }
   else
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Cinfiguration FAILED for node...\n"));
      /* Debug print here that confirmation is not OK */
   }

   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
   RETVALUE(ret);
} /* end of SmMiLavCfgCfm */

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
*       File:  avsm_avcfg.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLavCntrlCfm
(
Pst     *pst,          /* post structure */
AvMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLavCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
AvMngmt *cfm;          /* control */
#endif
{
   SM_TRC2(SmMiLavCntrlCfm);

   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW Control Cfm...\n"));
      /* avPgwSendMsg(EVTAVGTPUBINDCFM); */
   }
   else
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received Control Cfm. NOT OK for node..\n"));
      /* Debug print here that confirmation is not OK */
   }

   SPutSBuf(pst->region, pst->pool, (Data *)cfm, sizeof(AvMngmt));
   RETVALUE(ROK);
} /* end of SmMiLavCntrlCfm */

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
*       File:  avsm_avcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavStaInd
(
Pst     *pst,           /* post structure */
AvMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLavStaInd(pst, usta)
Pst     *pst;           /* post structure */
AvMngmt *usta;          /* unsolicited status */
#endif
{
   SM_TRC2(SmMiLavStaInd);

   /* Debug print here - print all the information from usta's Alarm structure
      AvUstaDgn */
   if (usta->u.usta.alarm.cause == LCM_CAUSE_UNKNOWN)
   {
      if (usta->u.usta.alarm.event == LAV_EVENT_SGW_EGTPC_BNDCFM)
      {
         avPgwSendMsg(EVTAVGTPCBINDCFM);
      }
      if (usta->u.usta.alarm.event == LAV_EVENT_SGW_EGTPU_BNDCFM)
      {
         avPgwSendMsg(EVTAVGTPUBINDCFM);
      }
      if (usta->u.usta.alarm.event == LAV_EVENT_SGW_ECHO_BNDCFM)
      {
         avPgwSendMsg(EVTAVECHOBINDCFM);
      }
      if (usta->u.usta.alarm.event == LAV_EVENT_PGW_ROUTECFM)
      {
         avPgwSendMsg(EVTAVPGWROUTECFM);
      }
   }
   /*Freeing the Mgmt Structure*/
   SPutSBuf(pst->region, pst->pool, (Data *)usta, sizeof(AvMngmt));
   RETVALUE(ROK);
} /* end of SmMiLavStaInd */


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
*       File:  avsm_avcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavTrcInd
(
Pst *pst,               /* post structure */
AvMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLavTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
AvMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   SM_TRC2(SmMiLavTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLavTrcInd */

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
*       File:  avsm_avcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavStsCfm
(
Pst       *pst,         /* post structure */
AvMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLavStsCfm(pst, sts)
Pst       *pst;         /* post structure */
AvMngmt   *sts;         /* confirmed statistics */
#endif
{
   SM_TRC2(SmMiLavStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLavStsCfm */

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
*       File:  avsm_avcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavStaCfm
(
Pst     *pst,           /* post structure */
AvMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLavStaCfm(pst, sta)
Pst     *pst;           /* post structure */
AvMngmt *sta;            /* confirmed status */
#endif
{
   SM_TRC2(SmMiLavStaCfm);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLavStaCfm */

/********************************************************************30**
 
         End of file:     avsm_avcfg.c@@/main/1 - Tue Aug 17 18:54:36 2010

*********************************************************************31*/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp         1.LTE-CNE:PGW Initial Release
/main/1      av001.101 rp        1.For removing warnings.
/main/1      av004.101 pmacharla  1.Multi UE Related changes
*********************************************************************91*/



