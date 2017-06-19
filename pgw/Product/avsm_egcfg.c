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

     Desc:    This file contains the AV application source code

     File:    jgsm_egcfg.c

     Sid:      avsm_egcfg.c@@/main/1 - Tue Aug 17 18:54:36 2010

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

#ifdef AV

PRIVATE S16 avPgwLegLSapCfg ARGS((SuId        suId,
                                  SpId        spId));

PRIVATE S16 avPgwLegUSapCfg ARGS((SpId spId,
                                  S16 type,
                                  U8 intfType));


PRIVATE S16 avPgwLegGenCfgReq ARGS((Void));

PRIVATE S16 avPgwLegIpAddrCfg ARGS((Void ));

PRIVATE S16 avPgwLegTSrvCfg ARGS((S16 type, S16 tptSrvId, S16 uSapId ));


PRIVATE S16 avPgwEgEnaUStaReq ARGS((Void));

#define AV_EGUSAP_CFG 0x4
#define AV_IPADDR_CFG 0x8
#define AV_TPTSRV_CFG 0x10

#define AV_SM_EG_C_CONFIGURED  (STEGGEN | AV_EGUSAP_CFG | \
                                STEGTSAP | AV_TPTSRV_CFG)
#define AV_SM_EG_ECHO_CONFIGURED  (AV_EGUSAP_CFG | AV_TPTSRV_CFG)
#define AV_SM_EG_U_CONFIGURED  (AV_EGUSAP_CFG | AV_IPADDR_CFG | AV_TPTSRV_CFG)

#define AV_SM_EG_C_TPTSRV_OPEN 0x1
#define AV_SM_EG_U_TPTSRV_OPEN 0x2
#define AV_SM_EG_ECHO_TPTSRV_OPEN 0x4

#define AV_SM_EG_TPTSRV_OPEN (AV_SM_EG_C_TPTSRV_OPEN | \
                              AV_SM_EG_U_TPTSRV_OPEN | \
                              AV_SM_EG_ECHO_TPTSRV_OPEN)

U8      avPgwEgCfgCb = 0;
U8      avPgwEgTptSrvOpen = 0;


/*
 *
 *       Fun:    avPgwEgCCfg - configure EG
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_egcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void avPgwEgCCfg
(
Void
)
#else
PUBLIC Void avPgwEgCCfg()
#endif /* ANSI */
{
   SM_TRC2(avPgwEgCCfg);

   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sending Pgw Egtpc configuration...\n"));
   avSmCb.egPst.event = EVTLEGCFGREQ;
   avPgwEgCfgCb = 0;
   avPgwLegGenCfgReq();

   avPgwLegUSapCfg(AVSM_SGW_EG_C_SPID, LEG_EGTP_C, EGT_GTP_INT_S5_S8);

   avPgwLegLSapCfg(AVSM_EG_HI_SUID, AVSM_EG_HI_SPID);

   avPgwLegTSrvCfg(LEG_EGTP_C, AVSM_TPT_SRVR_EGTPC, AVSM_SGW_EG_C_SPID);

   RETVOID;
}

/*
 *
 *       Fun:    avPgwEgECfg - configure EGTP ECHO 
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_egcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void avPgwEgECfg
(
Void
)
#else
PUBLIC Void avPgwEgECfg()
#endif /* ANSI */
{
   SM_TRC2(avPgwEgECfg);

   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sending Pgw Egtp echo configuration...\n"));
   avSmCb.egPst.event = EVTLEGCFGREQ;
   avPgwEgCfgCb = 0;
   avPgwLegUSapCfg(AVSM_SGW_ECHO_SPID, LEG_EGTP_C, EGT_GTP_INT_UNKNWN);

   avPgwLegTSrvCfg(LEG_EGTP_C, AVSM_TPT_SRVR_ECHO, AVSM_SGW_ECHO_SPID);

   RETVOID;
}


/*
 *
 *       Fun:    avPgwEgUCfg - configure EG
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  avsm_egcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void avPgwEgUCfg
(
Void
)
#else
PUBLIC Void avPgwEgUCfg()
#endif /* ANSI */
{
   SM_TRC2(avPgwEgUCfg);

   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sending PGW EGTP U configuration...\n"));
   avSmCb.egPst.event = EVTLEGCFGREQ;
   avPgwEgCfgCb = 0;

   avPgwLegUSapCfg(AVSM_SGW_EG_U_SPID, LEG_EGTP_U, EGT_GTP_INT_UNKNWN);

   avPgwLegIpAddrCfg();

   avPgwLegTSrvCfg(LEG_EGTP_U, AVSM_TPT_SRVR_EGTPU, AVSM_SGW_EG_U_SPID);

   RETVOID;
}

/*
*
*       Fun:   avPgwLegGenCfgReq
*
*       Desc:  fill in genCfg for EG
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  avsm_egcfg.c 
*
*/
#ifdef ANSI
PRIVATE S16 avPgwLegGenCfgReq
(
)
#else
PRIVATE S16 avPgwLegGenCfgReq()
#endif
{
   EgMngmt  egMgt;
   EgGenCfg    *cfg;

   SM_TRC2(avPgwLegGenCfgReq);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   avSmDefHdr(&egMgt.hdr, ENTEG, STEGGEN, AVSM_EGSMSEL);

   cfg   = &(egMgt.u.cfg.s.gen);
#ifdef EGTP_C
   cfg->nodeType     = LEG_GTP_ENT_PGW;
   cfg->nmbIntfs     = 1;
   cfg->nmbMaxTnls   = avSmCb.cfgCb.pgwUeNum * 16;

   /* To keep the tunnel search faster tune the tunnel array size
     to 30-40% of total number of tunnels. Keeping it power of 2
     to make it effective.  */
   if((cfg->nmbMaxTnls * 32) > 100)
   {
      cfg->maxTunlArrSz = (cfg->nmbMaxTnls * 32) / 100;
   }
   else
   {
      cfg->maxTunlArrSz =  cfg->nmbMaxTnls/2;
   }
   cfg->n3Requests   = 2;
#endif /* EGTP_C */

   cfg->nmbUSaps     = 3;
   cfg->maxNmbTSaps  = 1;
   cfg->nmbSrvs      = 3;
   cfg->timerRes     = 100;
   cfg->resThreshUpper= 9;
   cfg->resThreshLower= 1;
   cfg->lmPst.dstProcId  = SFndProcId();
   cfg->lmPst.srcProcId  = SFndProcId();
   cfg->lmPst.dstEnt     = ENTSM;
   cfg->lmPst.srcEnt     = ENTEG;
   cfg->lmPst.dstInst    = 0;
   cfg->lmPst.srcInst    = 0;
   cfg->lmPst.prior      = 0;
   cfg->lmPst.route      = 0;
   cfg->lmPst.event      = 0;
   cfg->lmPst.region     = avSmCb.init.region;
   cfg->lmPst.pool       = avSmCb.init.pool;
   cfg->lmPst.selector   = AVSM_EGSMSEL;

#ifdef EGTP_U
   cfg->eguT3Timer.enb           = FALSE;
   cfg->eguT3Timer.val           = 15;
   cfg->nmbRetries               = 3;
   cfg->echoTmr.enb              = FALSE;
   cfg->echoTmr.val              = 60;
   cfg->reOrderTmr.enb           = TRUE;
   cfg->reOrderTmr.val           = 20;
   cfg->pvtExtId                 = 10;
   cfg->pduWindSize              = 20;
   cfg->maxNumOfIp               = 1;
   cfg->nodeArch                 = 1;
#endif /* EGTP_U */

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&avSmCb.egPst, &egMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP Gen cfg....\n"));
   RETVALUE(ROK);
}/* avPgwLegGenCfgReq */

/*
*
*       Fun:   avPgwLegLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  avsm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwLegLSapCfg
(
SuId        suId,             /* EGTP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PRIVATE S16 avPgwLegLSapCfg(suId, spId)
SuId        suId;             /* EGTP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   EgMngmt    egMgt;
   EgTSapCfg  *cfg;

   SM_TRC2(avPgwLegLSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   avSmDefHdr(&egMgt.hdr, ENTEG, STEGTSAP, AVSM_EGSMSEL);

   cfg   = &(egMgt.u.cfg.s.tSap);

   cfg->tptType      = LEG_TSAP_TYPE_TUCL;

   cfg->tSapId       = suId;
   cfg->spId         = spId;
   cfg->tptType      = 1;
   cfg->selector     = AVSM_EGHISEL;
   cfg->mem.region   = avSmCb.init.region;
   cfg->mem.pool     = avSmCb.init.pool;
   cfg->priority     = 0;;
   cfg->route        = 0;
   cfg->dstProcId    = SFndProcId();
   cfg->dstEnt       = ENTHI;
   cfg->dstInst      = 0;
   cfg->tInitTmr.enb = TRUE;
   cfg->tInitTmr.val = 10;

   cfg->tptParam.type = CM_TPTPARAM_SOCK;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->tptParam.u.sockParam.numOpts     = 0;

   /* Send the request to EGTP */
   (Void) SmMiLegCfgReq(&avSmCb.egPst, &egMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent Pgw EGTP LSapCFg...\n"));
   RETVALUE(ROK);
}/* avPgwLegLSapCfg */

/*
*
*       Fun:   avPgwLegUSapCfg
*
*       Desc:  Call handler for EGUP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  avsm_egcfg.c
*
*/
#ifdef ANSI
PUBLIC S16 avPgwLegUSapCfg
(
SpId        spId,              /* Service Provider Sap ID */
S16         type,               /*-- EGTP C or U --*/
U8          intfType
)
#else
PUBLIC S16 avPgwLegUSapCfg(spId, type, intfType)
SpId        spId;             /* Service Provider Sap ID */
S16 type;                     /*-- EGTP C or U --*/
U8          intfType;
#endif
{
   EgMngmt  egMgt;
   EgUSapCfg  *cfg;

   SM_TRC2(avPgwLegUSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   avSmDefHdr(&egMgt.hdr, ENTEG, STEGUSAP, AVSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.uSap);

   cfg->sapId      = spId;
   cfg->selector   = AVSM_EGAVSEL;
   cfg->mem.region = avSmCb.init.region;
   cfg->mem.pool   = avSmCb.init.pool;
   cfg->priority   = 0;
   cfg->route      = 0;;
   cfg->dstProcId  = SFndProcId();
   cfg->dstEnt     = ENTEU;
   cfg->dstInst    = 0;
   cfg->flag       = type;
   cfg->intfType   = intfType;
   if (AVSM_SGW_ECHO_SPID == spId)
   {
      cfg->echo = TRUE;
   }

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&avSmCb.egPst, &egMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP-%c USapCfg...\n", (type == LEG_EGTP_C)?'C':'U'));
   RETVALUE(ROK);
}/* avPgwLegUSapCfg */

/*
*
*       Fun:   avPgwLegIpAddrCfg
*
*       Desc:  Call handler for IP Address Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  avsm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwLegIpAddrCfg
(
Void
)
#else
PRIVATE S16 avPgwLegIpAddrCfg()
#endif
{
   EgMngmt  egMgt;
   EgIpAddrCfg *cfg;

   SM_TRC2(avPgwLegIpAddrCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   avSmDefHdr(&egMgt.hdr, ENTEG, STEGIPADDR, AVSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.ipAddr);

   cfg->nmbIpAddr = 1;

   if ( (SGetSBuf(avSmCb.init.region, avSmCb.init.pool, (Data** )&cfg->ipInfo,
         (sizeof(EgIpInfo))) != ROK))
   {
      RETVALUE(RFAILED);
   }


   cmMemset((U8 *)cfg->ipInfo, 0, (sizeof(EgIpInfo)));

   cfg->ipInfo[0].srcIpAddr.type                     = CM_TPTADDR_IPV4;
   cmInetAddr (avSmCb.cfgCb.pgwDfltIpAddr,
         &(cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address));
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.port        = 0;
   cfg->ipInfo[0].spId                                = AVSM_SGW_EG_U_SPID;
   cfg->ipInfo[0].nodeType                            = TRUE;
   cfg->ipInfo[0].cfmStatus                           = FALSE;
   cfg->ipInfo[0].echoReqPvtExtVal.length             = 1;
   cfg->ipInfo[0].echoReqPvtExtVal.buff[0]            = 0x01;
   cfg->ipInfo[0].echoRspPvtExtVal.length             = 1;
   cfg->ipInfo[0].echoRspPvtExtVal.buff[0]            = 0x07;
   cfg->ipInfo[0].errIndPvtExtVal.length              = 1;
   cfg->ipInfo[0].errIndPvtExtVal.buff[0]             = 0x05;
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address =
      CM_INET_NTOH_U32(cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address);
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&avSmCb.egPst, &egMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP IpAddressCfg...\n"));
   RETVALUE(ROK);
}/* avPgwLegIpAddrCfg */

/*
*
*       Fun:   avPgwLegTSrvCfg
*
*       Desc:  Call handler for Transport server configuration 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  avsm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwLegTSrvCfg
(
S16 type,
S16 tptSrvId,
S16 uSapId
)
#else
PRIVATE S16 avPgwLegTSrvCfg (type, tptSrvId, uSapId)
S16 type;
S16 tptSrvId;
S16 uSapId;
#endif
{
   EgMngmt      egMgt;
   EgTptSrvCfg  *cfg = NULLP;

   SM_TRC2(avPgwLegTSrvCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   avSmDefHdr(&egMgt.hdr, ENTEG, STEGSRVR, AVSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.tptSrv);

   cfg->tptSrvId                         = tptSrvId;
   cfg->tSapId                           = 0;
   cfg->uSapId                           = uSapId;
   cfg->tptParam.u.sockParam.numOpts     = 0;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->opnSrvRetryCnt                   = 3;
   cfg->opnSrvTmr.enb                    = TRUE;
   cfg->opnSrvTmr.val                    = 10;
   cfg->hostName.pres                    = FALSE;
   cfg->hostName.len                     = (PTR) cmStrlen((CONSTANT U8 *)
                                            avSmCb.cfgCb.egDfltHstNm);
   cmMemcpy((U8 *)&cfg->hostName.val, (CONSTANT U8 *) avSmCb.cfgCb.egDfltHstNm,
            (cfg->hostName.len));

   cfg->gtp_C_Or_U                       = type;

   cfg->tptAddr.type                     = CM_TPTADDR_IPV4;
   if (tptSrvId == AVSM_TPT_SRVR_ECHO)
   {
      cfg->tptAddr.u.ipv4TptAddr.port       = avSmCb.cfgCb.egCDfltPort;
      cfg->isDfltSrv = TRUE;
   }
   else if(tptSrvId == AVSM_TPT_SRVR_EGTPU)
   {
      cfg->tptAddr.u.ipv4TptAddr.port       = avSmCb.cfgCb.egUDfltPort;
      cfg->isDfltSrv = FALSE; /*TRUE*/
   }
   else if(tptSrvId == AVSM_TPT_SRVR_EGTPC)
   {
      cfg->tptAddr.u.ipv4TptAddr.port       = avSmCb.cfgCb.egCNonDfltPort;
      cfg->isDfltSrv = FALSE;
   }
   cmInetAddr (avSmCb.cfgCb.egDfltAddr, &(cfg->tptAddr.u.ipv4TptAddr.address));
   cfg->tptAddr.u.ipv4TptAddr.address = CM_INET_NTOH_U32(cfg->tptAddr.u.
                                        ipv4TptAddr.address);
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&avSmCb.egPst, &egMgt);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP TransportServCfg...\n"));
   RETVALUE(ROK);
}/* avPgwLegTSrvCfg */

/*
 *      FUN:   avPgwEgLSapBndReq
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  avsm_egcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgLSapBndReq
(
Void 
)
#else
PUBLIC S16 avPgwEgLSapBndReq( Void )
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(avPgwEgLSapBndReq);
   
  /* Enable Alarm for EGTP */ 
   avPgwEgEnaUStaReq();

   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   avSmDefHdr(&cntrl.hdr, ENTEG, STEGTSAP, AVSM_EGSMSEL);

   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */

   avSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&avSmCb.egPst, &cntrl);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP Control Req to bind LSap...\n"));
   RETVALUE(ret);
}

#ifdef ANSI
PRIVATE S16 avPgwEgEnaUStaReq
(
Void
)
#else
PRIVATE S16 avPgwEgEnaUStaReq()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(avPgwEgEnaUStaReq);

   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   avSmDefHdr(&cntrl.hdr, ENTEG, STEGGEN, AVSM_EGSMSEL);

   cntrl.u.cntrl.action       = AENA;
   cntrl.u.cntrl.subAction    = SAUSTA;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */

   avSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&avSmCb.egPst, &cntrl);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP control Req to Unsolicited Sts...\n"));
   RETVALUE(ret);
}


/*
 *      FUN:   avPgwEgShutDwn
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  avsm_egcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgShutDwn
(
Void
)
#else
PUBLIC S16 avPgwEgShutDwn()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(avPgwEgShutDwn);
   
   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   avSmDefHdr(&cntrl.hdr, ENTEG, STEGGEN, AVSM_EGSMSEL);

   cntrl.u.cntrl.action        = ASHUTDOWN;
   cntrl.u.cntrl.subAction    = SAELMNT;

   avSmCb.egPst.event = EVTLEGCFGREQ;
   (Void)SmMiLegCntrlReq(&avSmCb.egPst, &cntrl);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Sent PGW EGTP control Req to ShutDown EGTP layer...\n"));
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
*       File:  avsm_egcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegCfgCfm
(
Pst     *pst,          /* post structure */
EgMngmt *cfm           /* configuration */
)
#else
PUBLIC S16 SmMiLegCfgCfm(pst, cfm)
Pst     *pst;          /* post structure */
EgMngmt *cfm;          /* configuration */
#endif
{
   SM_TRC2(SmMiLegCfgCfm);
   AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP CfgCfm with elmnt(%d) - status(%d)...\n",cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STEGGEN:
            avPgwEgCfgCb |= STEGGEN;
            AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP Gen CfgCfm...\n"));
            break;
         case STEGUSAP:
               avPgwEgCfgCb |= AV_EGUSAP_CFG;
               AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP USap CfgCfm...\n"));
            break;
         case STEGTSAP:
            avPgwEgCfgCb |= STEGTSAP;
            AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP TSap CfgCfm...\n"));
            break;
         case STEGSRVR:
            avPgwEgCfgCb |= AV_TPTSRV_CFG;
            AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP TptsServ CfgCfm...\n"));
            break;
         case STEGIPADDR:
            avPgwEgCfgCb |= AV_IPADDR_CFG;
            AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP IpAddress CfgCfm...\n"));
            break;
         default:
            AVSM_DBG_ERROR((AVSM_PRNTBUF,"Received PGW EGTP CfgCfm. with invalid elmnt(%d)..\n",cfm->hdr.elmId.elmnt));
            break;
      }

      if (avPgwEgCfgCb == AV_SM_EG_C_CONFIGURED)
      {
         avPgwSendMsg(EVTAVEGTPCCFGDONE);
      }
      else if (avPgwEgCfgCb == AV_SM_EG_U_CONFIGURED)
      {
         avPgwSendMsg(EVTAVEGTPUCFGDONE);
      }
      else if (avPgwEgCfgCb == AV_SM_EG_ECHO_CONFIGURED)
      {
         avPgwSendMsg(EVTAVEGTPECHOCFGDONE);
      }
   }
   else
   {
     AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP Configuration NOT OK...\n")); 
   }

   RETVALUE(ROK);
} /* end of SmMiLegCfgCfm */

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
*       File:  avsm_egcfg.c
*
*/
#ifdef ANSI
PUBLIC S16 SmMiLegCntrlCfm
(
Pst     *pst,          /* post structure */
EgMngmt *cfm           /* control */
)
#else
PUBLIC S16 SmMiLegCntrlCfm(pst, cfm)
Pst     *pst;          /* post structure */
EgMngmt *cfm;          /* control */
#endif
{
   SM_TRC2(SmMiLegCntrlCfm);
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP Control Cfm OK...\n"));
   }
   else
   {
      AVSM_DBG_INFO((AVSM_PRNTBUF,"Received PGW EGTP Control Cfm NOT OK...\n"));
   }
 
   RETVALUE(ROK);
} /* end of SmMiLegCntrlCfm */

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
*       File:  avsm_egcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegStaInd
(
Pst     *pst,           /* post structure */
EgMngmt *usta           /* unsolicited status */
)
#else
PUBLIC S16 SmMiLegStaInd(pst, usta)
Pst     *pst;           /* post structure */
EgMngmt *usta;          /* unsolicited status */
#endif
{

   SM_TRC2(SmMiLegStaInd);

   if ((usta->u.usta.alarm.event == LEG_EVENT_TPTSRV_ENA) &&
       (usta->u.usta.alarm.cause  == LCM_CAUSE_UNKNOWN))
   {
      switch(usta->u.usta.dgn.dgnVal[0].t.connId)
      {
         case AVSM_TPT_SRVR_EGTPC:
         {
            avPgwEgTptSrvOpen |= AV_SM_EG_C_TPTSRV_OPEN;
            break;
         }
         case AVSM_TPT_SRVR_EGTPU:
         {
            avPgwEgTptSrvOpen |= AV_SM_EG_U_TPTSRV_OPEN;
            break;
         }
         case AVSM_TPT_SRVR_ECHO:
         {
            avPgwEgTptSrvOpen |= AV_SM_EG_ECHO_TPTSRV_OPEN;
            break;
         }
      }

      /*-- When we have 3 alarms we are done with tpt server opening --*/
      if (avPgwEgTptSrvOpen == AV_SM_EG_TPTSRV_OPEN)
      {
         avPgwSendMsg(EVTAVTPTSRVROPEN);
      }
   }
   RETVALUE(ROK);
} /* end of SmMiLegStaInd */

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
*       File:  avsm_egcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegTrcInd
(
Pst *pst,               /* post structure */
EgMngmt *trc,           /* trace */
Buffer *mBuf            /* message buffer */
)
#else
PUBLIC S16 SmMiLegTrcInd(pst, trc, mBuf)
Pst *pst;               /* post structure */
EgMngmt *trc;           /* trace */
Buffer *mBuf;           /* message buffer */
#endif
{
   SM_TRC2(SmMiLegTrcInd);

   UNUSED(pst);
   UNUSED(trc);

   RETVALUE(ROK);
} /* end of SmMiLegTrcInd */

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
*       File:  avsm_egcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegStsCfm
(
Pst       *pst,         /* post structure */
EgMngmt   *sts          /* confirmed statistics */
)
#else
PUBLIC S16 SmMiLegStsCfm(pst, sts)
Pst       *pst;         /* post structure */
EgMngmt   *sts;         /* confirmed statistics */
#endif
{
   SM_TRC2(SmMiLegStsCfm);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* end of SmMiLegStsCfm */

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
*       File:  avsm_egcfg.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegStaCfm
(
Pst     *pst,           /* post structure */
EgMngmt *sta             /* confirmed status */
)
#else
PUBLIC S16 SmMiLegStaCfm(pst, sta)
Pst     *pst;           /* post structure */
EgMngmt *sta;            /* confirmed status */
#endif
{
   SM_TRC2(SmMiLegStaCfm);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* end of SmMiLegStaCfm */
#endif /* AV */

/********************************************************************30**
 
         End of file:     avsm_egcfg.c@@/main/1 - Tue Aug 17 18:54:36 2010

*********************************************************************31*/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp         1.LTE-CNE:PGW Initial Release
/main/1     av004.101  pmacharla   1. Multi UE support added
*********************************************************************91*/
