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

     Name:    Trillium LTE CNE - SGW Controller Module

     Type:    C Include file

     Desc:    This file contains the qo application source code

     File:    qosm_egcfg.c

     Sid:      qosm_egcfg.c@@/main/1 - Thu Jul  8 21:22:22 2010

     Prg:     mm 
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
#include "qo.h"          /* */

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
#include "qo.x"          /*    */

#ifdef QO_SGW

PRIVATE S16 qoSgwLegLSapCfg ARGS((SuId        suId,
                                  SpId        spId));

PRIVATE S16 qoSgwLegUSapCfg ARGS((SpId        spId, U8 intfType));
PRIVATE S16 qoSgwLegCSapCfg ARGS((SpId        spId, U8 intfType));
PRIVATE S16 qoSgwLegESapCfg ARGS((SpId        spId, U8 intfType));


PRIVATE S16 qoSgwLegGenCfgReq ARGS((Void ));

PRIVATE S16 qoSgwLegIpAddrCfg ARGS((Void ));

PRIVATE S16 qoSgwLegTSrvUCfg ARGS((Void ));
PRIVATE S16 qoSgwLegTSrvCCfg ARGS((U16 id, U16 port, U16 uSapId ));


PRIVATE S16 qoSgwEgEnaUStaReq ARGS((Void));

#define QO_EGUSAP_ECHO_CFG 0x4
#define QO_EGUSAP_S1U_CFG 0x8
#define QO_EGUSAP_S5S8U_CFG 0x10
#define QO_EGUSAP_S5S8C_CFG 0x20
#define QO_EGUSAP_S11_CFG 0x40
#define QO_EGTSAP_CFG 0x80
#define QO_IPADDR_CFG 0x100
#define QO_TPTSRV_C_DEF_CFG 0x200
#define QO_TPTSRV_C_S11_CFG 0x400
#define QO_TPTSRV_C_S5S8_CFG 0x800
#define QO_TPTSRV_U_CFG 0x1000
#define QO_EGUSAP_S4_CFG 0x2000
#define QO_TPTSRV_C_S4_CFG 0x4000

#define QO_TPTSRV_U_DEF  0x1
#define QO_TPTSRV_C_DEF  0x2
#define QO_TPTSRV_C_S11  0x4
#define QO_TPTSRV_C_S5S8 0x8
#define QO_TPTSRV_C_S4   0x10

#define QO_SM_EG_CONFIGURED  (STEGGEN | QO_EGUSAP_ECHO_CFG | QO_EGUSAP_S1U_CFG | \
      QO_EGUSAP_S5S8C_CFG | QO_EGUSAP_S4_CFG | QO_EGUSAP_S11_CFG | \
      QO_EGTSAP_CFG | QO_IPADDR_CFG | QO_TPTSRV_C_DEF_CFG | \
      QO_TPTSRV_C_S11_CFG | QO_TPTSRV_C_S5S8_CFG | QO_TPTSRV_C_S4_CFG | QO_TPTSRV_U_CFG)

#define QO_SM_EG_SRV_OPEN  (QO_TPTSRV_U_DEF | QO_TPTSRV_C_DEF | QO_TPTSRV_C_S11 \
      | QO_TPTSRV_C_S4 | QO_TPTSRV_C_S5S8)

U16     qoSgwEgCfgCb = 0;
U16     qoSgwEgSrvOpen = 0;

/*
 *
 *       Fun:    qoSgwEgCfg - configure EG
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  qosm_egcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void qoSgwEgCfg
(
Void
)
#else
PUBLIC Void qoSgwEgCfg()
#endif /* ANSI */
{
   SM_TRC2(qoSgwEgCfg);

   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sending SgwEg configuration...\n"));
   qoSmCb.egPst.event = EVTLEGCFGREQ;
   qoSgwEgCfgCb = 0;
   qoSgwLegGenCfgReq();
   qoSgwLegESapCfg(QOSM_SGW_ECHO_EG_SPID, EGT_GTP_INT_UNKNWN);
   qoSgwLegUSapCfg(QOSM_SGW_EG_U_SPID, EGT_GTP_INT_UNKNWN);
   qoSgwLegCSapCfg(QOSM_SGW_S11_C_SPID, EGT_GTP_INT_S11);
   qoSgwLegCSapCfg(QOSM_SGW_S5S8_C_SPID, EGT_GTP_INT_S5_S8);
   qoSgwLegCSapCfg(QOSM_SGW_S4_C_SPID, EGT_GTP_INT_S4);
   qoSgwLegLSapCfg(QOSM_EG_HI_SUID, QOSM_EG_HI_SPID);
   qoSgwLegIpAddrCfg();
   qoSgwLegTSrvUCfg();
   qoSgwLegTSrvCCfg(QO_EG_SRV_C_DEF_ID, qoSmCb.cfgCb.egDfltCPort,
         QOSM_SGW_ECHO_EG_SPID);
   qoSgwLegTSrvCCfg(QO_EG_SRV_C_S11_ID, qoSmCb.cfgCb.egS11CPort,
         QOSM_SGW_S11_C_SPID);
   qoSgwLegTSrvCCfg(QO_EG_SRV_C_S5S8_ID, qoSmCb.cfgCb.egS5S8CPort,
         QOSM_SGW_S5S8_C_SPID);
   qoSgwLegTSrvCCfg(QO_EG_SRV_C_S4_ID, qoSmCb.cfgCb.egS4CPort,
         QOSM_SGW_S4_C_SPID);

   RETVOID;
}

/*
*
*       Fun:   qoSgwLegGenCfgReq
*
*       Desc:  fill in genCfg for EG
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  qosm_egcfg.c 
*
*/
#ifdef ANSI
PRIVATE S16 qoSgwLegGenCfgReq
(
Void
)
#else
PRIVATE S16 qoSgwLegGenCfgReq()
#endif
{
   EgMngmt  egMgt;
   EgGenCfg    *cfg;

   SM_TRC2(qoSgwLegGenCfgReq);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGGEN, QOSM_EGSMSEL);

   cfg   = &(egMgt.u.cfg.s.gen);
#ifdef EGTP_C
   cfg->nodeType     = LEG_GTP_ENT_SGW;
   cfg->nmbIntfs     = 4;
   cfg->nmbMaxTnls   = qoSmCb.cfgCb.ueNum * cfg->nmbIntfs;
   /* To keep the tunnel search faster tune the tunnel array size
     to 30-40% of total number of tunnels. Keeping it power of 2
     to make it effective */
   if((cfg->nmbMaxTnls * 32) > 100)
   {
      cfg->maxTunlArrSz = (cfg->nmbMaxTnls * 32) / 100;
   }
   else
   {
      cfg->maxTunlArrSz =  (cfg->nmbMaxTnls/2);
   }
   cfg->n3Requests   = 2;
#endif /* EGTP_C */
   cfg->nmbUSaps     = 6;
   cfg->maxNmbTSaps  = 1;
   cfg->nmbSrvs      = 10; /* using numbers 1 and 2 */
   cfg->timerRes     = 20;
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
   cfg->lmPst.region     = qoSmCb.init.region;
   cfg->lmPst.pool       = qoSmCb.init.pool;
   cfg->lmPst.selector   = QOSM_EGSMSEL;

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
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP Gen cfg....\n"));
   RETVALUE(ROK);
}/* qoSgwLegGenCfgReq */

/*
*
*       Fun:   qoSgwLegLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  qosm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 qoSgwLegLSapCfg
(
SuId        suId,             /* EGTP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PRIVATE S16 qoSgwLegLSapCfg(suId, spId)
SuId        suId;             /* EGTP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   EgMngmt    egMgt;
   EgTSapCfg  *cfg;

   SM_TRC2(qoSgwLegLSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGTSAP, QOSM_EGSMSEL);

   cfg   = &(egMgt.u.cfg.s.tSap);

   cfg->tptType      = LEG_TSAP_TYPE_TUCL;

   cfg->tSapId       = suId;
   cfg->spId         = spId;
   cfg->tptType      = 1;
   cfg->selector     = QOSM_EGHISEL;
   cfg->mem.region   = qoSmCb.init.region;
   cfg->mem.pool     = qoSmCb.init.pool;
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
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP LSapCFg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegLSapCfg */

/*
*
*       Fun:   qoSgwLegUSapCfg
*
*       Desc:  Call handler for EGUP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  qosm_egcfg.c
*
*/
#ifdef ANSI
PUBLIC S16 qoSgwLegUSapCfg
(
SpId        spId,             /* Service Provider Sap ID */
U8 intfType
)
#else
PUBLIC S16 qoSgwLegUSapCfg(spId,intfType)
SpId        spId;             /* Service Provider Sap ID */
U8 intfType;
#endif
{
   EgMngmt  egMgt;
   EgUSapCfg  *cfg;

   SM_TRC2(qoSgwLegUSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGUSAP, QOSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.uSap);

   egMgt.hdr.transId    = spId;  /* To match response in CfgCfm*/
   cfg->sapId      = spId;
   cfg->selector   = QOSM_EGQOSEL;
   cfg->mem.region = qoSmCb.init.region;
   cfg->mem.pool   = qoSmCb.init.pool;
   cfg->priority   = 0;
   cfg->route      = 0;;
   cfg->dstProcId  = SFndProcId();
   cfg->dstEnt     = ENTEU;
   cfg->dstInst    = 0;
   cfg->flag       = LEG_EGTP_U;
   cfg->intfType = intfType;

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP USapCfg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegUSapCfg */
/*
 *
 *       Fun:   qoSgwLegCSapCfg
 *
 *       Desc:  Call handler for EGUP Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  qosm_egcfg.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwLegCSapCfg
(
 SpId        spId,             /* Service Provider Sap ID */
 U8 intfType
 )
#else
PUBLIC S16 qoSgwLegCSapCfg(spId,intfType)
   SpId        spId;             /* Service Provider Sap ID */
   U8 intfType;
#endif
{
   EgMngmt  egMgt;
   EgUSapCfg  *cfg;

   SM_TRC2(qoSgwLegCSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGUSAP, QOSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.uSap);

   egMgt.hdr.transId    = spId;  /* To match response in CfgCfm*/
   cfg->sapId      = spId;
   cfg->selector   = QOSM_EGQOSEL;
   cfg->mem.region = qoSmCb.init.region;
   cfg->mem.pool   = qoSmCb.init.pool;
   cfg->priority   = 0;
   cfg->route      = 0;;
   cfg->dstProcId  = SFndProcId();
   cfg->dstEnt     = ENTEU;
   cfg->dstInst    = 0;
   cfg->flag       = LEG_EGTP_C;
   cfg->intfType = intfType;
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP USapCfg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegCSapCfg */
/*
 *
 *       Fun:   qoSgwLegESapCfg
 *
 *       Desc:  Call handler for EGUP Configuration Request
 *
 *       Ret:   ROK
 *
 *       Notes: None
 *              
 *       File:  qosm_egcfg.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwLegESapCfg
(
 SpId        spId,             /* Service Provider Sap ID */
 U8 intfType
 )
#else
PUBLIC S16 qoSgwLegESapCfg(spId,intfType)
   SpId        spId;             /* Service Provider Sap ID */
   U8 intfType;
#endif
{
   EgMngmt  egMgt;
   EgUSapCfg  *cfg;

   SM_TRC2(qoSgwLegESapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGUSAP, QOSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.uSap);

   egMgt.hdr.transId    = spId;  /* To match response in CfgCfm*/
   cfg->sapId      = spId;
   cfg->selector   = QOSM_EGQOSEL;
   cfg->mem.region = qoSmCb.init.region;
   cfg->mem.pool   = qoSmCb.init.pool;
   cfg->priority   = 0;
   cfg->route      = 0;;
   cfg->dstProcId  = SFndProcId();
   cfg->dstEnt     = ENTEU;
   cfg->dstInst    = 0;
   cfg->flag       = LEG_EGTP_C;
   cfg->echo       = TRUE;
   cfg->intfType = intfType;
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP USapCfg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegESapCfg */

/*
*
*       Fun:   qoSgwLegIpAddrCfg
*
*       Desc:  Call handler for IP Address Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  qosm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 qoSgwLegIpAddrCfg
(
Void
)
#else
PRIVATE S16 qoSgwLegIpAddrCfg()
#endif
{
   EgMngmt  egMgt;
   EgIpAddrCfg *cfg;

   SM_TRC2(qoSgwLegIpAddrCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGIPADDR, QOSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.ipAddr);

   cfg->nmbIpAddr = 1;

   if ( (SGetSBuf(qoSmCb.init.region, qoSmCb.init.pool, (Data** )&cfg->ipInfo,
         (sizeof(EgIpInfo))) != ROK))
   {
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)cfg->ipInfo, 0, (sizeof(EgIpInfo)));

   cfg->ipInfo[0].srcIpAddr.type                  = CM_TPTADDR_IPV4;
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.address = qoSmCb.cfgCb.sgwIpAddr;
   cfg->ipInfo[0].srcIpAddr.u.ipv4TptAddr.port        = 0;
   cfg->ipInfo[0].spId                                = 3;
   cfg->ipInfo[0].nodeType                            = TRUE;
   cfg->ipInfo[0].cfmStatus                           = FALSE;
   cfg->ipInfo[0].echoReqPvtExtVal.length             = 1;
   cfg->ipInfo[0].echoReqPvtExtVal.buff[0]            = 0x01;
   cfg->ipInfo[0].echoRspPvtExtVal.length             = 1;
   cfg->ipInfo[0].echoRspPvtExtVal.buff[0]            = 0x07;
   cfg->ipInfo[0].errIndPvtExtVal.length              = 1;
   cfg->ipInfo[0].errIndPvtExtVal.buff[0]             = 0x05;

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP IpAddressCfg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegIpAddrCfg */

/*
*
*       Fun:   qoSgwLegTSrvUCfg
*
*       Desc:  Call handler for Transport server configuration 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  qosm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 qoSgwLegTSrvUCfg
(
void
)
#else
PRIVATE S16 qoSgwLegTSrvUCfg(Void )
#endif
{
   EgMngmt      egMgt;
   EgTptSrvCfg  *cfg = NULLP;

   SM_TRC2(qoSgwLegTSrvUCfg);

   /*--------------for EGTP-U----------------------------*/
   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGSRVR, QOSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.tptSrv);

   egMgt.hdr.transId    = QO_EG_SRV_U_DEF_ID;  /* To match response in CfgCfm*/
   cfg->tptSrvId                         = QO_EG_SRV_U_DEF_ID;
   cfg->tSapId                           = 0;
   cfg->uSapId                           = 3;
   cfg->tptParam.u.sockParam.numOpts     = 0;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->opnSrvRetryCnt                   = 3;
   cfg->opnSrvTmr.enb                    = TRUE;
   cfg->opnSrvTmr.val                    = 10;
   cfg->hostName.pres                    = FALSE;
   cfg->hostName.len                     = (PTR) cmStrlen((CONSTANT U8 *)
                                            qoSmCb.cfgCb.egDfltHstNm);
   cmMemcpy((U8 *)&cfg->hostName.val, (CONSTANT U8 *) qoSmCb.cfgCb.egDfltHstNm,
            (cfg->hostName.len));

   cfg->gtp_C_Or_U                       = LEG_EGTP_U;

   cfg->tptAddr.type                     = CM_TPTADDR_IPV4;
   cfg->tptAddr.u.ipv4TptAddr.port       = qoSmCb.cfgCb.egDfltUPort;
   cmInetAddr (qoSmCb.cfgCb.egDfltAddr, &(cfg->tptAddr.u.ipv4TptAddr.address));
   cfg->tptAddr.u.ipv4TptAddr.address = qoSmCb.cfgCb.sgwIpAddr;
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);

   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP-U TransportServCfg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegTSrvUCfg */

/*
*
*       Fun:   qoSgwLegTSrvCCfg
*
*       Desc:  Call handler for Transport server configuration 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  qosm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 qoSgwLegTSrvCCfg
(
U16 id,
U16 port,
U16 uSapId
)
#else
PRIVATE S16 qoSgwLegTSrvCCfg(id, port, uSapId)
U16 id;
U16 port;
U16 uSapId;
#endif
{
   EgMngmt      egMgt;
   EgTptSrvCfg  *cfg = NULLP;

   SM_TRC2(qoSgwLegTSrvCCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   qoSmDefHdr(&egMgt.hdr, ENTEG, STEGSRVR, QOSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.tptSrv);


   /*--------------for EGTP-C----------------------------*/
   egMgt.hdr.transId    = id;  /* To match response in CfgCfm*/
   cfg->tptSrvId                         = id;
   cfg->tSapId                           = 0;
   cfg->uSapId                           = uSapId;
   cfg->tptParam.u.sockParam.numOpts     = 0;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->opnSrvRetryCnt                   = 3;
   cfg->opnSrvTmr.enb                    = TRUE;
   cfg->opnSrvTmr.val                    = 10;
   cfg->hostName.pres                    = FALSE;
   cfg->hostName.len                     = (PTR) cmStrlen((CONSTANT U8 *)
                                            qoSmCb.cfgCb.egDfltHstNm);
   cmMemcpy((U8 *)&cfg->hostName.val, (CONSTANT U8 *) qoSmCb.cfgCb.egDfltHstNm,
            (cfg->hostName.len));

   if(port == LEG_DEF_PORT_SGW)
   {
      cfg->isDfltSrv                        = TRUE; /* Using Default port 2123
                                                       for server */
   }
   else
   {
      cfg->isDfltSrv                        = FALSE;
   }
   cfg->gtp_C_Or_U                       = LEG_EGTP_C;

   cfg->tptAddr.type                     = CM_TPTADDR_IPV4;
   cfg->tptAddr.u.ipv4TptAddr.port       = port;
   cmInetAddr (qoSmCb.cfgCb.egDfltAddr, &(cfg->tptAddr.u.ipv4TptAddr.address));
   cfg->tptAddr.u.ipv4TptAddr.address = qoSmCb.cfgCb.sgwIpAddr;
   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&qoSmCb.egPst, &egMgt);

   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP TransportServCfg...\n"));
   RETVALUE(ROK);
}/* qoSgwLegTSrvCCfg */

/*
 *      FUN:   qoSgwEgLSapBndReq
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  qosm_egcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgLSapBndReq
(
Void
)
#else
PUBLIC S16 qoSgwEgLSapBndReq()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(qoSgwEgLSapBndReq);
   
  /* Enable Alarm for EGTP */ 
   qoSgwEgEnaUStaReq();

   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   qoSmDefHdr(&cntrl.hdr, ENTEG, STEGTSAP, QOSM_EGSMSEL);

   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */

   qoSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&qoSmCb.egPst, &cntrl);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP Control Req to bind LSap...\n"));
   RETVALUE(ret);
}

#ifdef ANSI
PRIVATE S16 qoSgwEgEnaUStaReq
(
Void
)
#else
PRIVATE S16 qoSgwEgEnaUStaReq()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(qoSgwEgEnaUStaReq);

   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   qoSmDefHdr(&cntrl.hdr, ENTEG, STEGGEN, QOSM_EGSMSEL);

   cntrl.u.cntrl.action       = AENA;
   cntrl.u.cntrl.subAction    = SAUSTA;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */

   qoSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&qoSmCb.egPst, &cntrl);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP control Req to Unsolicited Sts...\n"));
   RETVALUE(ret);
}


/*
 *      FUN:   qoSgwEgShutDwn
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  qosm_egcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgShutDwn
(
Void
)
#else
PUBLIC S16 qoSgwEgShutDwn()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(qoSgwEgShutDwn);
   
   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   qoSmDefHdr(&cntrl.hdr, ENTEG, STEGGEN, QOSM_EGSMSEL);

   cntrl.u.cntrl.action        = ASHUTDOWN;
   cntrl.u.cntrl.subAction    = SAELMNT;

   qoSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&qoSmCb.egPst, &cntrl);
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Sent Sgw EGTP control Req to ShutDown EGTP layer...\n"));
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
*       File:  qosm_egcfg.c
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
   QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP CfgCfm with elmnt(%d) - status(%d)...\n",cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STEGGEN:
            qoSgwEgCfgCb |= cfm->hdr.elmId.elmnt;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Gen CfgCfm...\n"));
            break;
         case STEGUSAP:
            {
               if(cfm->hdr.transId == QOSM_SGW_EG_U_SPID)
               {
                  qoSgwEgCfgCb |= QO_EGUSAP_S1U_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP USap CfgCfm for \
                           S1U interface\n"));
               }
               else if(cfm->hdr.transId == QOSM_SGW_S5S8_C_SPID)
               {
                  qoSgwEgCfgCb |= QO_EGUSAP_S5S8C_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP USap CfgCfm for \
                           S5S8-C interface\n"));
               }
               else if(cfm->hdr.transId == QOSM_SGW_S11_C_SPID)
               {
                  qoSgwEgCfgCb |= QO_EGUSAP_S11_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP USap CfgCfm for \
                           S11-C interface\n"));
               }
               else if(cfm->hdr.transId == QOSM_SGW_ECHO_EG_SPID)
               {
                  qoSgwEgCfgCb |= QO_EGUSAP_ECHO_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP USap CfgCfm for \
                           EGTP-C Echo\n"));
               }
               else if(cfm->hdr.transId == QOSM_SGW_S4_C_SPID)
               {
                  qoSgwEgCfgCb |= QO_EGUSAP_S4_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP USap CfgCfm for \
                           S4\n"));
               }
            break;
            }
         case STEGTSAP:
            qoSgwEgCfgCb |= QO_EGTSAP_CFG;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP TSap CfgCfm...\n"));
            break;
         case STEGSRVR:
               if(cfm->hdr.transId == QO_EG_SRV_U_DEF_ID)
               {
                  qoSgwEgCfgCb |= QO_TPTSRV_U_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP TptsServ CfgCfm for \
                           User Plane.\n"));
               }
               else if(cfm->hdr.transId == QO_EG_SRV_C_DEF_ID)
               {
                  qoSgwEgCfgCb |= QO_TPTSRV_C_DEF_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP Default TptsServ CfgCfm for \
                           Control Plane.\n"));
               }
               else if(cfm->hdr.transId == QO_EG_SRV_C_S11_ID)
               {
                  qoSgwEgCfgCb |= QO_TPTSRV_C_S11_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP TptsServ CfgCfm for \
                           S11 - Control Plane.\n"));
               }
               else if(cfm->hdr.transId == QO_EG_SRV_C_S5S8_ID)
               {
                  qoSgwEgCfgCb |= QO_TPTSRV_C_S5S8_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP TptsServ CfgCfm for \
                           S5S8 - Control Plane.\n"));
               }
               else if(cfm->hdr.transId == QO_EG_SRV_C_S4_ID)
               {
                  qoSgwEgCfgCb |= QO_TPTSRV_C_S4_CFG;
                  QOSM_DBG_INFO((QOSM_PRNTBUF,
                           "Received Sgw EGTP TptsServ CfgCfm for \
                           S4 - Control Plane.\n"));
               }
            break;
         case STEGIPADDR:
            qoSgwEgCfgCb |= QO_IPADDR_CFG;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP IpAddress CfgCfm...\n"));
            break;
         default:
       QOSM_DBG_ERROR((QOSM_PRNTBUF,"Received Sgw EGTP CfgCfm. with invalid elmnt(%d)..\n",cfm->hdr.elmId.elmnt));
            break;
      }

      if (qoSgwEgCfgCb == QO_SM_EG_CONFIGURED)
      {
         qoSgwSendMsg(EVTQOEGTPCFGDONE);
      }
   }
   else
   {
     QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Configuration NOT OK...\n")); 
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
*       File:  qosm_egcfg.c
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
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Control Cfm OK...\n"));
   }
   else
   {
      QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Control Cfm NOT OK...\n"));
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
*       File:  qosm_egcfg.c
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
         case QO_EG_SRV_U_DEF_ID:
            qoSgwEgSrvOpen |= QO_TPTSRV_U_DEF;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Alarm\
                     Server for EGTP-U default is open\n"));
            break;
         case QO_EG_SRV_C_DEF_ID:
            qoSgwEgSrvOpen |= QO_TPTSRV_C_DEF;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Alarm\
                     Server for EGTP-C default is open\n"));
            break;
         case QO_EG_SRV_C_S11_ID:
            qoSgwEgSrvOpen |= QO_TPTSRV_C_S11;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Alarm\
                     Server for EGTP-C S11 is open\n"));
            break;
         case QO_EG_SRV_C_S5S8_ID:
            qoSgwEgSrvOpen |= QO_TPTSRV_C_S5S8;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Alarm\
                     Server for EGTP-C S5S8 is open\n"));
            break;
         case QO_EG_SRV_C_S4_ID:
            qoSgwEgSrvOpen |= QO_TPTSRV_C_S4;
            QOSM_DBG_INFO((QOSM_PRNTBUF,"Received Sgw EGTP Alarm\
                     Server for EGTP-C S4 is open\n"));
            break;
      }
      if (qoSgwEgSrvOpen == QO_SM_EG_SRV_OPEN)
         qoSgwSendMsg(EVTQOEGTPSERVEROPEN);
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
*       File:  qosm_egcfg.c
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
*       File:  qosm_egcfg.c
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
*       File:  qosm_egcfg.c
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
#endif /* QO_SGW */

/**********************************************************************

         End of file:     qosm_egcfg.c@@/main/1 - Thu Jul  8 21:22:22 2010

**********************************************************************/

/********************************************************************60**
        Revision history:
*********************************************************************61*/


/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp         1. LTE-CNE:SGW Initial Release
*********************************************************************91*/
