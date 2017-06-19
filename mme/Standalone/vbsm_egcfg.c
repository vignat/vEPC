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

     Name:    Trillium LTE CNE - MME Config and Control 

     Type:    C Source file

     Desc:    This file contains the stack manager configuration-control
              for EGTP.

     File:    vbsm_egcfg.c

     Sid:      vbsm_egcfg.c@@/main/1 - Mon Jun 28 19:35:41 2010

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
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
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

#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "vbsm.x"          /*    */

PRIVATE S16 vbMmeLegGenCfgReq ARGS((Void ));

PRIVATE S16 vbMmeLegLSapCfg ARGS((SuId   suId,
                                  SpId   spId));

PRIVATE S16 vbMmeLegUSapCfg ARGS((SpId spId, Bool echoFlag, EgtIntf ifType));


PRIVATE S16 vbMmeLegTSrvCfg ARGS((U32  tptSrvrId, SpId spId, U16 port, 
                                  Bool dfltSrvFlag));

PRIVATE S16 vbMmeEgEnaUStaReq ARGS((Void));


#define VB_SM_EG_CONFIGURED  (STEGGEN + STEGTSAP + \
                              STEGUSAP + STEGUSAP + STEGUSAP + STEGUSAP + \
                              STEGSRVR + STEGSRVR + STEGSRVR + STEGSRVR)

#define VB_SM_EG_TPTSRVRS_OPENED (VBSM_EG_ECHO_TPTSRVID + VBSM_EG_TPTSRVID + \
                                 VBSM_EG_S10_TPTSRVID + VBSM_EG_S3_TPTSRVID)

U8      vbMmeEgCfgCb = 0;
U8      vbMmeEgTptCb = 0;

/*
 *
 *       Fun:    vbMmeEgCfg - configure EG
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
 *      File:  vbsm_egcfg.c
 *
 */

#ifdef ANSI
PUBLIC Void vbMmeEgCfg
(
Void
)
#else
PUBLIC Void vbMmeEgCfg()
#endif /* ANSI */
{
   SM_TRC2(vbMmeEgCfg);

   VBSM_DBG_INFO((VBSM_PRNTBUF,"Initializing MME's EGTPC configuration...\n"));
   vbSmCb.egPst.event = EVTLEGCNTRLREQ;
   vbMmeEgCfgCb = 0;
   vbMmeLegGenCfgReq();

   /* Configure upper SAP non default port other than 2123 for S11 interface */ 
   vbMmeLegUSapCfg(VBSM_EG_SPID, FALSE, EGT_GTP_INT_S11);

   /* Configuring the Upper SAP for S10 interface */
   vbMmeLegUSapCfg(VBSM_EG_S10_SPID, FALSE, EGT_GTP_INT_S10);

   /* Configure upper SAP for echo SAP for standard port 2123 */ 
   vbMmeLegUSapCfg(VBSM_EG_ECHO_SPID, TRUE, EGT_GTP_INT_S11);

   /* Configuring the Upper SAP for S3 interface */
   vbMmeLegUSapCfg(VBSM_EG_S3_SPID, FALSE, EGT_GTP_INT_S3);

   /* Configure lower SAP with TUCL */
   vbMmeLegLSapCfg(VBSM_EG_HI_SUID, VBSM_EG_HI_SPID);

   /* Configure Transport Server for echo SAP, default/standard port 2123 */ 
   vbMmeLegTSrvCfg(VBSM_EG_ECHO_TPTSRVID, VBSM_EG_ECHO_SPID, 
                   vbSmCb.cfgCb.egDfltPort, TRUE);

   /* Configure Transport Server for non default port other than 2123 */ 
   vbMmeLegTSrvCfg(VBSM_EG_TPTSRVID, VBSM_EG_SPID, 
                   vbSmCb.cfgCb.egNonDfltPort, FALSE);

   /* Configure Transport Server for non default port other than 2123 for S10 i/f */ 
   vbMmeLegTSrvCfg(VBSM_EG_S10_TPTSRVID, VBSM_EG_S10_SPID,\
         vbSmCb.cfgCb.egS10NonDfltPort, FALSE);

   /* Configure Transport Server for non default port other than 2123 for S3 i/f */ 
   vbMmeLegTSrvCfg(VBSM_EG_S3_TPTSRVID, VBSM_EG_S3_SPID, \
         vbSmCb.cfgCb.egS3NonDfltPort, FALSE);

   RETVOID;
} /* vbMmeEgCfg */

/*
*
*       Fun:   vbMmeLegGenCfgReq
*
*       Desc:  fill in genCfg for EG
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_egcfg.c 
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLegGenCfgReq
(
Void
)
#else
PRIVATE S16 vbMmeLegGenCfgReq()
#endif
{
   EgMngmt  egMgt;
   EgGenCfg    *cfg;

   SM_TRC2(vbMmeLegGenCfgReq);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   vbSmDefHdr(&egMgt.hdr, ENTEG, STEGGEN, VBSM_EGSMSEL);

   cfg             = &(egMgt.u.cfg.s.gen);
   cfg->nodeType   = LEG_GTP_ENT_MME;
   cfg->nmbIntfs   = 3;
   cfg->nmbMaxTnls = vbSmCb.cfgCb.ueNum + 1;
   cfg->n3Requests = 2;

   /* To keep the tunnel search faster tune the tunnel array size
    * to 30-40% of total number of tunnels. Keeping it power of 2
    * to make it effective
    */
   if((cfg->nmbMaxTnls * 32) > 100)
   {
      cfg->maxTunlArrSz = (cfg->nmbMaxTnls * 32) / 100;
   }
   else
   {
      cfg->maxTunlArrSz =  cfg->nmbMaxTnls/2;
   }

   /* To support two upper SAPS: 
    * 1. ECHO (With default port of EGTP-C 2123) SAP.
    * 2. Non default EGTP-C SAP
    */
   cfg->nmbUSaps    = 4;
   cfg->maxNmbTSaps = 1;

   /* To support two transport servers: 
    * 1. Default port of EGTP-C: 2123. 
    * 2. Non Default port
    */
   cfg->nmbSrvs         = 5;
   cfg->timerRes        = 2;
   cfg->resThreshUpper  = 9;
   cfg->resThreshLower  = 1;
   cfg->lmPst.dstProcId = SFndProcId();
   cfg->lmPst.srcProcId = SFndProcId();
   cfg->lmPst.dstEnt    = ENTSM;
   cfg->lmPst.srcEnt    = ENTEG;
   cfg->lmPst.dstInst   = 0;
   cfg->lmPst.srcInst   = 0;
   cfg->lmPst.prior     = 0;
   cfg->lmPst.route     = 0;
   cfg->lmPst.event     = 0;
   cfg->lmPst.region    = vbSmCb.init.region;
   cfg->lmPst.pool      = vbSmCb.init.pool;
   cfg->lmPst.selector  = VBSM_EGSMSEL;

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&vbSmCb.egPst, &egMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP Gen cfg....\n"));

   RETVALUE(ROK);
}/* vbMmeLegGenCfgReq */

/*
*
*       Fun:   vbMmeLegLSapCfg
*
*       Desc:  Call handler for TSAP Configuration Request
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLegLSapCfg
(
SuId        suId,             /* EGTP Sap ID */
SpId        spId              /* Service Provider Sap ID */
)
#else
PRIVATE S16 vbMmeLegLSapCfg(suId, spId)
SuId        suId;             /* EGTP Sap ID */
SpId        spId;             /* Service Provider Sap ID */
#endif
{
   EgMngmt    egMgt;
   EgTSapCfg  *cfg;

   SM_TRC2(vbMmeLegLSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   vbSmDefHdr(&egMgt.hdr, ENTEG, STEGTSAP, VBSM_EGSMSEL);

   cfg   = &(egMgt.u.cfg.s.tSap);

   cfg->tptType                          = LEG_TSAP_TYPE_TUCL;
   cfg->tSapId                           = suId;
   cfg->spId                             = spId;
   cfg->tptType                          = 1;
   cfg->selector                         = VBSM_EGHISEL;
   cfg->mem.region                       = vbSmCb.init.region;
   cfg->mem.pool                         = vbSmCb.init.pool;
   cfg->priority                         = 0;;
   cfg->route                            = 0;
   cfg->dstProcId                        = SFndProcId();
   cfg->dstEnt                           = ENTHI;
   cfg->dstInst                          = 0;
   cfg->tInitTmr.enb                     = TRUE;
   cfg->tInitTmr.val                     = 10;
   cfg->tptParam.type                    = CM_TPTPARAM_SOCK;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->tptParam.u.sockParam.numOpts     = 0;

   /* Send the request to EGTP */
   (Void)SmMiLegCfgReq(&vbSmCb.egPst, &egMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP LSapCFg...\n"));

   RETVALUE(ROK);
}/* vbMmeLegLSapCfg */



/**************************************************************************//**
 *
 * @brief 
 *    EGT Upper SAP configuration. Sap between MME Application and EGT
 *             
 * @param[in]  spId
 *    Service Provider ID
 * @param[in]  echoFlag
 *    TRUE if SAP is for echo request/response
 * @param[in]  intfType
 *    GTP Interface type
 * 
 * @return
 *    SUCCESS if SAP is configured successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeLegUSapCfg
(
SpId        spId,
Bool        echoFlag,
EgtIntf     intfType
)
{
   EgMngmt  egMgt;
   EgUSapCfg  *cfg;

   SM_TRC2(vbMmeLegUSapCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   vbSmDefHdr(&egMgt.hdr, ENTEG, STEGUSAP, VBSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.uSap);

   cfg->sapId      = spId;
   cfg->echo       = echoFlag;
   cfg->intfType   = intfType;
   cfg->selector   = VBSM_EGVBSEL;
   cfg->mem.region = vbSmCb.init.region;
   cfg->mem.pool   = vbSmCb.init.pool;
   cfg->priority   = 0;
   cfg->route      = 0;
   cfg->dstProcId  = SFndProcId();
   cfg->dstEnt     = ENTEU;
   cfg->dstInst    = 0;
   cfg->flag       = LEG_EGTP_C;

   /* Send the request to the LM */
   (Void)SmMiLegCfgReq(&vbSmCb.egPst, &egMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP USapCfg...\n"));

   RETVALUE(ROK);
} /* vbMmeLegUSapCfg */ 

/*
*
*       Fun:   vbMmeLegTSrvCfg
*
*       Desc:  Call handler for Transport server configuration 
*
*       Ret:   ROK
*
*       Notes: None
*              
*       File:  vbsm_egcfg.c
*
*/
#ifdef ANSI
PRIVATE S16 vbMmeLegTSrvCfg
(
U32    tptSrvrId,
SpId   spId,
U16    port,
Bool   dfltSrvFlag
)
#else
PRIVATE S16 vbMmeLegTSrvCfg(tptSrvrId, spId, port, dfltSrvFlag)
U32    tptSrvrId;
SpId   spId;
U16    port;
Bool   dfltSrvFlag;
#endif
{
   EgMngmt      egMgt;
   EgTptSrvCfg  *cfg = NULLP;

   SM_TRC2(vbMmeLegTSrvCfg);

   cmMemset((U8 *)&egMgt, 0, sizeof(EgMngmt));
   vbSmDefHdr(&egMgt.hdr, ENTEG, STEGSRVR, VBSM_EGSMSEL);

   cfg = &(egMgt.u.cfg.s.tptSrv);

   cfg->tptSrvId                         = tptSrvrId;
   cfg->tSapId                           = 0;
   cfg->uSapId                           = spId;
   cfg->tptParam.u.sockParam.numOpts     = 0;
   cfg->tptParam.u.sockParam.listenQSize = 5;
   cfg->opnSrvRetryCnt                   = 3;
   cfg->opnSrvTmr.enb                    = TRUE;
   cfg->opnSrvTmr.val                    = 10;

   /* TRUE, if this is default server with port 2123, and there can be 
    * only one default server */
   cfg->isDfltSrv = dfltSrvFlag;  
   cfg->hostName.pres                    = FALSE;
   cfg->hostName.len = (PTR)cmStrlen((CONSTANT U8 *)\
         vbSmCb.cfgCb.egDfltHstNm);
   cmMemcpy((U8 *)&cfg->hostName.val, (CONSTANT U8 *) vbSmCb.cfgCb.egDfltHstNm,
            (cfg->hostName.len));

   cfg->gtp_C_Or_U                    = LEG_EGTP_C;
   cfg->tptAddr.type                  = CM_TPTADDR_IPV4;
   cfg->tptAddr.u.ipv4TptAddr.port    = port;
   cfg->tptAddr.u.ipv4TptAddr.address = vbSmCb.cfgCb.mmeIpAddr;

   /* Send the request to the LM */
   (Void) SmMiLegCfgReq(&vbSmCb.egPst, &egMgt);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP TransportServCfg...\n"));

   RETVALUE(ROK);
}/* vbMmeLegTSrvCfg */

/*
 *      FUN:   vbMmeEgLSapBndReq
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_egcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeEgLSapBndReq
(
Void
)
#else
PUBLIC S16 vbMmeEgLSapBndReq()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(vbMmeEgLSapBndReq);
   
  /* Enable Alarm for EGTP */ 
   vbMmeEgEnaUStaReq();

   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTEG, STEGTSAP, VBSM_EGSMSEL);

   cntrl.u.cntrl.action        = ABND_ENA;
   cntrl.u.cntrl.subAction    = 0;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */

   vbSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&vbSmCb.egPst, &cntrl);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP Control Req to bind LSap...\n"));
   RETVALUE(ret);
}

#ifdef ANSI
PRIVATE S16 vbMmeEgEnaUStaReq
(
Void
)
#else
PRIVATE S16 vbMmeEgEnaUStaReq()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(vbMmeEgEnaUStaReq);

   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTEG, STEGGEN, VBSM_EGSMSEL);

   cntrl.u.cntrl.action       = AENA;
   cntrl.u.cntrl.subAction    = SAUSTA;

   /* There is no explicit SAP Id to send the control request for 
      This request triggers sending bind for the SAP distinguished
      based on element Id in header */

   vbSmCb.egPst.event = EVTLEGCNTRLREQ;
   (Void)SmMiLegCntrlReq(&vbSmCb.egPst, &cntrl);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP control Req to Unsolicited "\
         "Sts...\n"));
   RETVALUE(ret);
}


/*
 *      FUN:   vbMmeEgShutDwn
 *
 *      Desc:  Brings S1AP to the state before configuration
 *
 *      Ret:   Void
 *
 *      Notes: None
 *
 *      File:  vbsm_egcfg.c
 *
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeEgShutDwn
(
Void
)
#else
PUBLIC S16 vbMmeEgShutDwn()
#endif /* ANSI */
{
   EgMngmt  cntrl;
   S16      ret = ROK;
   SM_TRC2(vbMmeEgShutDwn);
   
   cmMemset((U8 *)&cntrl, 0, sizeof(EgMngmt));
   vbSmDefHdr(&cntrl.hdr, ENTEG, STEGGEN, VBSM_EGSMSEL);

   cntrl.u.cntrl.action        = ASHUTDOWN;
   cntrl.u.cntrl.subAction    = SAELMNT;

   vbSmCb.egPst.event = EVTLEGCFGREQ;
   (Void)SmMiLegCntrlReq(&vbSmCb.egPst, &cntrl);
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Sent Mme EGTP control Req to ShutDown EGTP  "\
         "layer...\n"));
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
*       File:  vbsm_egcfg.c
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
   VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP CfgCfm with elmnt(%d) - "\
         "status(%d)...\n", cfm->hdr.elmId.elmnt,cfm->cfm.status));
   if (cfm->cfm.status == LCM_PRIM_OK)
   {
      switch (cfm->hdr.elmId.elmnt)
      {
         case STEGGEN:
         {
            vbMmeEgCfgCb += cfm->hdr.elmId.elmnt;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP Gen CfgCfm...\n"));
           break;
         } 
         case STEGUSAP:
         { 
            vbMmeEgCfgCb += cfm->hdr.elmId.elmnt;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP USap CfgCfm...\n"));
           break;
         }
         case STEGTSAP:
         {
            vbMmeEgCfgCb += cfm->hdr.elmId.elmnt;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP TSap CfgCfm...\n"));
            break;
         }
         case STEGSRVR:
         {
            vbMmeEgCfgCb += cfm->hdr.elmId.elmnt;
            VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP TptsServ CfgCfm\n"));
            break;
         }
         default:
         {
            VBSM_DBG_ERROR((VBSM_PRNTBUF,"Received Mme EGTP CfgCfm. with "\
                  "invalid elmnt(%d)..\n", cfm->hdr.elmId.elmnt));
            break;
         }  
      }

      if(VB_SM_EG_CONFIGURED == vbMmeEgCfgCb)
      {
         vbMmeSendMsg(EVTVBEGTPCFGDONE);
      }
   }
   else
   {
     VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP Configuration NOT OK\n")); 
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
*       File:  vbsm_egcfg.c
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
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP Control Cfm OK...\n"));
   }
   else
   {
      VBSM_DBG_INFO((VBSM_PRNTBUF,"Received Mme EGTP Control Cfm NOT OK...\n"));
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
*       File:  vbsm_egcfg.c
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
      if(usta->u.usta.dgn.dgnVal[0].t.connId)
      {
         vbMmeEgTptCb += usta->u.usta.dgn.dgnVal[0].t.connId;
      }
      if(vbMmeEgTptCb == VB_SM_EG_TPTSRVRS_OPENED)
      {
         vbMmeSendMsg(EVTVBEGTPCSERVEROPEN);
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
*       File:  vbsm_egcfg.c
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
*       File:  vbsm_egcfg.c
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
*       File:  vbsm_egcfg.c
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


/***i*****************************************************************30**
 
         End of file:     vbsm_egcfg.c@@/main/1 - Mon Jun 28 19:35:41 2010
 
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
*********************************************************************91*/
