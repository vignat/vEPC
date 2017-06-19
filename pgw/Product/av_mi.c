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
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:     C source code for LM Module
  
     File:     jg_mi.c
  
     Sid:      av_mi.c@@/main/1 - Tue Aug 17 18:54:33 2010
  
     Prg:      ap
  
*********************************************************************21*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */

#ifndef AV_PERF
#include <pcap.h>
#endif

#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */

#include "leg.h"
#include "egt.h"

#include "lav.h"
#include "av.h"
#include "avsm.h"
#include "av_err.h"

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */

#include "leg.x"
#include "egt.x"

#include "lav.x"
#include "av.x"
#include "avsm.x"


SSTskId       avTskId;

/* PGW Global structure. */
PUBLIC AvPgwCb avPgwCb;


/**********************************************************************/
/************* Prototypes for MI functions ****************************/
/**********************************************************************/


PRIVATE S16 avPgwGenCfg ARGS((AvPgwGenCfg *genCfg,
                              CmStatus    *status));

PRIVATE S16 avPgwCfgEgtEchoSap ARGS((AvPgwLSapCfg *lSap,
                                        CmStatus      *status));

PRIVATE S16 avPgwCfgEgtcUSap ARGS((AvPgwLSapCfg *lSap,
                                        CmStatus      *status));

PRIVATE S16 avPgwCfgEgtuUSap ARGS((AvPgwLSapCfg *lSap,
                                        CmStatus     *status));

PRIVATE S16 avPgwPeerCfg ARGS((AvPgwPeerCfg *peerCfg,
                               CmStatus     *status));

PRIVATE S16 avPgwGenCntrl ARGS (( AvCntrl *cntrl,
         CmStatus   *status        ));

PRIVATE S16 avPgwLSapCntrl ARGS (( AvCntrl *cntrl,
         CmStatus   *status        ));

PRIVATE S16 avPgwSndArpReq ARGS((AvPgwCntrl *cntrl,
                                 CmStatus   *status));

PRIVATE S16 avPgwCreateRcvTsk ARGS((Void));


PRIVATE S16 avPgwRcvActvInit ARGS((Ent entity,
                                  Inst inst,    
                                  Region region, 
                                  Reason reason));

PRIVATE S16 avSendLmCfm ARGS((Pst    *pst,
                              Header *hdr,
                              U8      type,
                              AvMngmt *cfm));

/******************************************************************************/
/*                   Management Primitives                                    */
/******************************************************************************/

/******************************************************************************/
/*                   Configuration Request Primitive                          */
/******************************************************************************/
/*
 *
 *       Fun:   AvMiLavCfgReq - Configuration Request
 *
 *       Desc:  This function is used by the layer manager to configure
 *              the LTE CNE layer. The layer responds with a Configuration
 *              Confirm "AvMiLavCfgCfm" to the layer manager after
 *              completing the configuration.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: Configuration must be performed in the following
 *              sequence:
 *              1) MME General configuration (STAVGEN)
 *              2) Lower sap configuration (STAVSZTSAP)
 *              3) SGW General configuration (STAVGEN)
 *              4) SGW lower sap configurations( STAVENBUSAP, STAVMMECSAP
 *                 STAVPGWUSAP, STAVPGWCSAP)
 *              
 *       File:  av_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 AvMiLavCfgReq
(
Pst     *pst,
AvMngmt *cfg            /* Config structure */
)
#else
PUBLIC S16 AvMiLavCfgReq(pst, cfg)
Pst     *pst;
AvMngmt *cfg;           /* Config structure */
#endif
{
   S16 ret = ROK;       /* Return Value */
   AvMngmt cfgCfm;      /* Config confirm */

   AV_PGW_SND_TRC2(AvMiLavCfgReq);

   cmMemset ((U8 *)&cfgCfm, 0, sizeof (AvMngmt));

   AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf,
            "AvMiLavCfgReq (pst, elmId (%d))\n", cfg->hdr.elmId.elmnt));

   switch(cfg->hdr.elmId.elmnt)
   {
      case STAVGEN:
         {
            ret = avPgwGenCfg(&cfg->u.cfg.s.pgwGenCfg, &cfgCfm.cfm);
            break;
         }
      case STAVPEER:
         {
            ret = avPgwPeerCfg(&cfg->u.cfg.s.peerCfg, &cfgCfm.cfm);
            break;
         }
      case STAVECHOSAP:
         {
            /* Configure the Lower SAPs for EGTP- echo */
            ret = avPgwCfgEgtEchoSap(&cfg->u.cfg.s.pgwLSapCfg, &cfgCfm.cfm);
            if (ROK != ret)
            {
               AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                        "AvMiLavCfgReq: PDN-GW' to EGTP- echo SAP Configuration failed\n"));

               cmHashListDeinit(&avPgwCb.ueCbHlst);
               AV_PGW_SND_RETVALUE(ret);
            }

            break;
         }
      case STAVEGTPCSAP:
         {
            /* Configure the Lower SAPs for EGTP-C */
            ret = avPgwCfgEgtcUSap(&cfg->u.cfg.s.pgwLSapCfg, &cfgCfm.cfm);
            if (ROK != ret)
            {
               AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                        "AvMiLavCfgReq: PDN-GW' to EGTP-C SAP Configuration failed\n"));

               cmHashListDeinit(&avPgwCb.ueCbHlst);
               AV_PGW_SND_RETVALUE(ret);
            }

            break;
         }
      case STAVEGTPUSAP:
         {

            /* Configure the Lower SAPs for EGTP-U */
            ret = avPgwCfgEgtuUSap(&cfg->u.cfg.s.pgwLSapCfg, &cfgCfm.cfm);
            if (ROK != ret)
            {
               AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                        "AvMiLavCfgReq: PDN-GW' to EGTP-U SAP Configuration failed\n"));

               /* Since this is dummy EGTP and in future with actual EGTP we
                  should expect LSAP Config Request from stack manager, in which
                  this code would be separate;In which we need not be deleting UE Cb List
                  in failure case.
                */
               cmHashListDeinit(&avPgwCb.ueCbHlst);
               AV_PGW_SND_RETVALUE(ret);
            }

            break;
         }
      default:
         break;
   }/* End of case */

   avSendLmCfm(pst, &cfg->hdr, TCFG, &cfgCfm);
   /*Freeing the Mgmt Structure*/
   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(AvMngmt));
   RETVALUE(ret);

} /* End of AvMiLavCfgReq */

/******************************************************************************/
/*                   Control Request Primitive                                */
/******************************************************************************/
/*
*
*       Fun:   AvMiLavCntrlReq - Control Request
*
*       Desc:  This primitive is used to control the specified element.
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  av_mi.c
*/
#ifdef ANSI
PUBLIC S16 AvMiLavCntrlReq
(
Pst     *pst,
AvMngmt *cntrl              /* Control structure */
)
#else
PUBLIC S16 AvMiLavCntrlReq(pst, cntrl)
Pst     *pst;
AvMngmt *cntrl;             /* Control structure */
#endif
{
   S16 ret = ROK;           /* Return value */
   AvMngmt cntrlCfm;        /* Control confirm */

   TRC3(AvMiLavCntrlReq);

   cmMemset ((U8 *)&cntrlCfm, 0, sizeof (AvMngmt));

   AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf,
            "AvMiLavCntrlReq (pst, elmId (%d))\n", cntrl->hdr.elmId.elmnt));

   /* Check if General Configuration is done */
   if (!avPgwCb.init.cfgDone)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV022, 0,
            "AvMiLavCntrlReq: PDN-GW General configuration not done ");

      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      avSendLmCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

      RETVALUE(ROK);
   }

   switch(cntrl->hdr.elmId.elmnt)
   {
      case STAVGEN:
         {
            /* general (applicable layer-wide) control */
            ret = avPgwGenCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
            break;
         }
      case STAVEGTPCSAP:  /* eGTP-C towards SGW */
      case STAVECHOSAP:   /* eGTP- echo sap towards SGW */
      case STAVEGTPUSAP:  /* eGTP-U towards SGW */
         {
            ret = avPgwLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
            break;
         }
      case STAVTRGARP:
         {
            /* ARP request to discover EX-GW */
            ret = avPgwSndArpReq(&cntrl->u.cntrl.u.pgwCntrl, &cntrlCfm.cfm);
            break;
         }
      default:
         {
            /* invalid element */
            AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV023, 
                  (ErrVal) cntrl->hdr.elmId.elmnt,
                  "AvMiLavCntrlReq: Invalid Element in control request");
            cntrlCfm.cfm.status = LCM_PRIM_NOK;
            cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
            break;
         }
   } /* end switch */

   avSendLmCfm(pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(AvMngmt));

   RETVALUE(ret);

} /* AvMiLavCntrlReq */

/******************************************************************************/
/*                   Statistics Request Primitive                             */
/******************************************************************************/
/*
*
*       Fun:   AvMiLavStsReq - Statistics Request
*
*       Desc:  This primitive is used by the Layer Manager to solicit 
*              statistics information. The statistics are returned
*              in the  AvMiLavStsCfm primitive.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  av_mi.c
*
*/

#ifdef ANSI
PUBLIC S16 AvMiLavStsReq
(
Pst     *pst,
Action   action,
AvMngmt *sts                 /* Statistics structure */
)
#else
PUBLIC S16 AvMiLavStsReq(pst, action, sts)
Pst     *pst;
Action   action;
AvMngmt *sts;                /* Statistics structure */
#endif
{
   S16 ret = ROK;            /* Return value */

   RETVALUE(ret);

} /* AvMiLavStsReq */


/******************************************************************************/
/*                   Status  Request Primitive                                */
/******************************************************************************/
/*
*
*       Fun:   Status Request
*
*       Desc:  This primitive is used by the Layer Manager to solicit
*              status information. The information is returned via the
*              AvMiLavStaCfm primitive.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  av_mi.c
*
*/
#ifdef ANSI
PUBLIC S16 AvMiLavStaReq
(
Pst     *pst,
AvMngmt *sta                 /* Status structure */
)
#else
PUBLIC S16 AvMiLavStaReq(pst, sta)
Pst     *pst;
AvMngmt *sta;                /* Status structure */
#endif
{
   S16 ret = ROK;            /* Return value */

   TRC3(AvMiLavStaReq)

   RETVALUE(ret);

} /* AvMiLavStaReq */


/******************************************************************************/
/*                  Indication Generation Functions                           */
/******************************************************************************/
/*
 *
 *      Fun:   avSendLmAlarm
 *
 *      Desc:  This function is used to send an Alarm / Unsolicited Status
 *             Indication to the Layer Manager
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PUBLIC Void avSendLmAlarm
(
U16 category,           /* category */
U16 event,              /* event generated */
U16 cause              /* cause */
)
#else
PUBLIC Void avSendLmAlarm(category, event, cause)
U16 category;           /* category */
U16 event;              /* event generated */
U16 cause;              /* cause */
#endif
{
   AvMngmt usta;
   Pst     pst;

   TRC2(avSendLmAlarm);

   /* initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(usta));

   usta.hdr.elmId.elmnt = STAVGEN;
   /* Check if general configuration is done */
   if (avPgwCb.init.cfgDone == FALSE)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV024, (ErrVal)0,
            "Status Indication requested; general configuration not done!");
      RETVOID;
   }

   if (avPgwCb.init.usta == FALSE)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV025, (ErrVal)0,
            "usta is not initialized!");
      RETVOID;
   }
   usta.hdr.entId.ent  = avPgwCb.init.ent;
   usta.hdr.entId.inst = avPgwCb.init.inst;
   cmMemcpy((U8 *)&pst, (U8 *)&(avPgwCb.init.lmPst), sizeof(Pst));

   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event = event;
   usta.u.usta.alarm.cause = cause;

   /* update the date and time */
   (Void) SGetDateTime(&usta.u.usta.alarm.dt);

   AvMiLavStaInd(&pst, &usta);

   RETVOID;

} /* end of avSendLmAlarm */



/******************************************************************************/
/*                   Management Interface Support Functions                   */
/******************************************************************************/

/******************************************************************************/
/*                   Layer Manager Confirm  Function                          */
/******************************************************************************/
/*
 *
 *      Fun:   avSendLmCfm
 *
 *      Desc:  Generate the Confirm to the Layer Manager
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avSendLmCfm
(
Pst     *pst,                           
Header  *hdr,         /* Pointer to the Header structure */
U8       type,        /* Confirmation Type */
AvMngmt *cfm         /* Management Structure */
)
#else
PRIVATE S16 avSendLmCfm(pst, hdr, type, cfm)
Pst     *pst;
Header  *hdr;         /* Pointer to the Header structure */
U8       type;       /* Confirmation Type */
AvMngmt *cfm;        /* Management Structure */
#endif
{

   Pst cfmPst;        /* Confirmation Post Structure */

   TRC2(avSendLmCfm);

   /* Fill up the header in the confirm structure */
   cmMemset( (U8 *)&cfm->hdr, '\0', sizeof(cfm->hdr));       
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.transId = hdr->transId;

   /* Fill up the post struct for confirm */
   cfm->hdr.entId.ent = avPgwCb.init.ent;
   cfm->hdr.entId.inst = avPgwCb.init.inst;
   cfmPst.srcEnt  = avPgwCb.init.ent;
   cfmPst.srcInst = avPgwCb.init.inst;

   cfmPst.srcProcId = SFndProcId();
   cfmPst.dstEnt = pst->srcEnt;
   cfmPst.dstInst = pst->srcInst;
   cfmPst.dstProcId = pst->srcProcId;
   cfmPst.selector = hdr->response.selector;
   cfmPst.prior = hdr->response.prior;
   cfmPst.route = hdr->response.route;

   cfmPst.region = hdr->response.mem.region;
   cfmPst.pool=  hdr->response.mem.pool;

   /* Send the corresponding confirm */
   switch (type)
   {
      case TCFG:
         {
            /* Send Configuration Confirm */
            AvMiLavCfgCfm (&cfmPst, cfm);

            AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf,
                     "avSendLmCfm: sent AvMiLavCfgCfm (cfmType: %d) \n", type));
            break;
         }
      case TCNTRL:
         { 
            /* Send Control Confirm */
            AvMiLavCntrlCfm (&cfmPst, cfm);

            AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf,
                     "avSendLmCfm: sent AvMiLavCntrlCfm (cfmType: %d) \n", type));
            break;
         }
      case TSTS:
         {
            /* Send Statistics Confirm */
            AvMiLavStsCfm (&cfmPst, cfm);

            AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf,
                     "avSendLmCfm: sent AvMiLavStsCfm (cfmType: %d) \n", type));
            break;
         }
      case TSSTA:
         {
            /* Send Status Confirm */
            AvMiLavStaCfm (&cfmPst, cfm);

            AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf,
                     "avSendLmCfm: sent AvMiLavStaCfm (cfmType: %d) \n", type));
            break;
         }
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         AVDBGP(DBGMASK_MI, (avPgwCb.init.prntBuf, "avSendLmCfm : Invalid Type \n"));
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         break;
   }

   RETVALUE(ROK); 

} /* avSendLmCfm */


/******************************************************************************/
/*                   Configuration Request Support Functions                  */
/******************************************************************************/

/******************************************************************************/
/*                   PGW Peer Configuration Support Functions              */
/******************************************************************************/

/*
 *
 *      Fun:   avPgwPeerCfg
 *
 *      Desc:  Stores the general LTE CNE configuration for PGW.
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwPeerCfg
(
AvPgwPeerCfg *peerCfg,       /* Peer configuration */
CmStatus     *status         /* Return status */
)
#else
PRIVATE S16 avPgwPeerCfg(peerCfg, status)
AvPgwPeerCfg *peerCfg;       /* Peer configuration */
CmStatus     *status;        /* Return status */
#endif
{
   AV_PGW_SND_TRC2(avPgwPeerCfg)

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Store the PeerCfg in global control block */
   cmMemset((U8 *)&avPgwCb.peerCfg, 0, sizeof(AvPgwPeerCfg));
   cmMemcpy((U8 *)&avPgwCb.peerCfg, (U8 *)peerCfg, sizeof(AvPgwPeerCfg));

   AV_PGW_SND_RETVALUE(ROK);

} /* avPgwPeerCfg */

/******************************************************************************/
/*                   PGW General Configuration Support Functions              */
/******************************************************************************/
/*
 *
 *      Fun:   avPgwValidateGenCfg
 *
 *      Desc:  Validates the LTE CNE general configuration. 
 *
 *      Ret:   LCM_REASON_NOT_APPL - ok
 *             LCM_REASON_INVALID_PAR_VAL - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwValidateGenCfg
(
AvPgwGenCfg *gen        /* General configuration */
)
#else
PRIVATE S16 avPgwValidateGenCfg(gen)
AvPgwGenCfg *gen;       /* General configuration */
#endif
{
   AV_PGW_SND_TRC2(avPgwValidateGenCfg)

   if (gen == NULLP)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV026, ERRZERO,
          "avPgwValidateGenCfg,  NULL cfg ");
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   if (gen->maxNmbBearers <= 0)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG,EAV027, (ErrVal) gen->maxNmbBearers, 
                  "avPgwValidateGenCfg: Invalid Num of bearers");
      AV_PGW_SND_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gen->maxNmbEgtSaps <= 0)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG,EAV028, (ErrVal) gen->maxNmbEgtSaps, 
                  "avPgwValidateGenCfg: Invalid Num of egt saps");
      AV_PGW_SND_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gen->maxNmbTunls <= 0)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG,EAV029, (ErrVal) gen->maxNmbTunls, 
                  "avPgwValidateGenCfg: Invalid Num of tunnels");
      AV_PGW_SND_RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   AV_PGW_SND_RETVALUE(LCM_REASON_NOT_APPL);

} /* avPgwValidateGenCfg */

/*
 *
 *      Fun:   avPgwCfgEgtEchoSap
 *
 *      Desc:  Stores the LTE CNE (EGTC)SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: This initializes the EGTP- echo SAP.
 *
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwCfgEgtEchoSap
(
AvPgwLSapCfg *lSap,     /* Lower SAP configuration */
CmStatus      *status    /* Return status */
)
#else
PRIVATE S16 avPgwCfgEgtEchoSap(lSap, status)
AvPgwLSapCfg *lSap;     /* Lower SAP configuration */
CmStatus      *status;   /* Return status */
#endif
{
   AvPgwEgtSapCb *egtcSapCb = NULLP;
   S16            ret = ROK;

   AV_PGW_SND_TRC2(avPgwCfgEgtEchoSap)

   egtcSapCb = NULLP;

   /* Allocate SAP Control Block */
   AV_PDNGW_SENDTSK_ALLOC(egtcSapCb, sizeof(AvPgwEgtSapCb));

   if (egtcSapCb == NULLP)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV030, ERRZERO,
          "avPgwCfgEgtEchoSap, Memory Allcation failed for Sap");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   egtcSapCb->pst.selector   = lSap->selector;

/* If multiple regions support 
   Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   egtcSapCb->pst.region   = avPgwCb.init.region;
   egtcSapCb->pst.pool     = avPgwCb.init.pool;
#else
   egtcSapCb->pst.region   = lSap->mem.region;
   egtcSapCb->pst.pool     = lSap->mem.pool;
#endif

   egtcSapCb->pst.prior      = lSap->prior;
   egtcSapCb->pst.route      = lSap->route;
   egtcSapCb->pst.srcProcId  = SFndProcId();
   egtcSapCb->pst.srcEnt     = avPgwCb.init.ent;
   egtcSapCb->pst.srcInst    = avPgwCb.init.inst;

   egtcSapCb->pst.dstProcId   = lSap->dstProcId;
   egtcSapCb->pst.dstEnt     = lSap->dstEntId;
   egtcSapCb->pst.dstInst    = lSap->dstInstId;
   egtcSapCb->suId           = lSap->suId;
   egtcSapCb->spId           = lSap->spId;
   egtcSapCb->nmbConn        = lSap->nmbConn;
   egtcSapCb->state          = LAV_SAP_CFG;

   ret = cmHashListInit(&(egtcSapCb->ueTunnelCbHlCp), /* hash list Cp */
          AV_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          avPgwCb.init.region,                /* Mem region for HL */
          avPgwCb.init.pool);

    if (ROK != ret)
    {
       AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV031, ERRZERO,
                  "avPgwCfgEgtEchoSap: Hash List Initialization failed\n");

       AV_PDNGW_SENDTSK_FREE(egtcSapCb, sizeof(AvPgwEgtSapCb));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       AV_PGW_SND_RETVALUE(ret);
    }

   avPgwCb.egtEchoSapCb = egtcSapCb;
   AV_PGW_SND_RETVALUE(ROK);
}

/*
 *
 *      Fun:   avPgwCfgEgtuUSap
 *
 *      Desc:  Stores the LTE CNE (EGTU)SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: This initializes the dummy EGTP-U SAP. To be replaced
 *             by actual EGTP-U SAP using the SAP parameter.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwCfgEgtuUSap
(
AvPgwLSapCfg *lSap,     /* Lower SAP configuration */
CmStatus     *status     /* Return status */
)
#else
PRIVATE S16 avPgwCfgEgtuUSap(lSap, status)
AvPgwLSapCfg *lSap;       /* Lower SAP configuration */
CmStatus      *status;     /* Return status */
#endif
{
   AvPgwEgtSapCb *egtuSapCb = NULLP;
   S16 ret = ROK;

   AV_PGW_SND_TRC2(avPgwCfgEgtuUSap)

   /* Allocate SAP Control Block */
   AV_PDNGW_SENDTSK_ALLOC(egtuSapCb, sizeof(AvPgwEgtSapCb));

   if (egtuSapCb == NULLP)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV032, ERRZERO,
          "avPgwCfgEgtuUSap, Memory Allcation failed for Sap");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   egtuSapCb->pst.selector   = lSap->selector;

  /* If multiple regions support 
     Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   egtuSapCb->pst.region   = avPgwCb.init.region;
   egtuSapCb->pst.pool     = avPgwCb.init.pool;
#else
   egtuSapCb->pst.region   = lSap->mem.region;
   egtuSapCb->pst.pool     = lSap->mem.pool;
#endif

   egtuSapCb->pst.prior      = 0;
   egtuSapCb->pst.route      = 0;
   egtuSapCb->pst.srcProcId  = SFndProcId();

   egtuSapCb->pst.srcEnt     = ENTEU;
   egtuSapCb->pst.srcInst    = AV_PGW_PRIM_INSTANCE;

   egtuSapCb->pst.dstProcId  = lSap->dstProcId;
   egtuSapCb->pst.dstEnt     = lSap->dstEntId;
   egtuSapCb->pst.dstInst    = lSap->dstInstId;

   egtuSapCb->suId           = lSap->suId;
   egtuSapCb->spId           = lSap->spId;
   egtuSapCb->nmbConn        = lSap->nmbConn;
   egtuSapCb->state          = LAV_SAP_CFG;

   ret = cmHashListInit(&(egtuSapCb->ueTunnelCbHlCp), /* hash list Cp */
          AV_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          avPgwCb.init.region,                /* Mem region for HL */
          avPgwCb.init.pool);

   if (ROK != ret)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV033, ERRZERO,
                "avPgwCfgEgtuUSap: Hash List Initialization failed\n");

      AV_PDNGW_SENDTSK_FREE(egtuSapCb, sizeof(AvPgwEgtSapCb));
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      AV_PGW_SND_RETVALUE(ret);
   }

   avPgwCb.egtUSapCb = egtuSapCb;
   AV_PGW_SND_RETVALUE(ROK);
}

/*
 *
 *      Fun:   avPgwCfgEgtcUSap
 *
 *      Desc:  Stores the LTE CNE (EGTC)SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: This initializes the dummy EGTP-C SAP. To be replaced
 *             by actual EGTP-U SAP using the SAP parameter.
 *
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwCfgEgtcUSap
(
AvPgwLSapCfg *lSap,     /* Lower SAP configuration */
CmStatus      *status    /* Return status */
)
#else
PRIVATE S16 avPgwCfgEgtcUSap(lSap, status)
AvPgwLSapCfg *lSap;     /* Lower SAP configuration */
CmStatus      *status;   /* Return status */
#endif
{
   AvPgwEgtSapCb *egtcSapCb = NULLP;
   S16            ret = ROK;

   AV_PGW_SND_TRC2(avPgwCfgEgtcUSap)

   egtcSapCb = NULLP;
   
   /* Allocate SAP Control Block */
   AV_PDNGW_SENDTSK_ALLOC(egtcSapCb, sizeof(AvPgwEgtSapCb));

   if (egtcSapCb == NULLP)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV034, ERRZERO,
          "avPgwCfgEgtcUSap, Memory Allcation failed for Sap");
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      AV_PGW_SND_RETVALUE(RFAILED);
   }
  
   egtcSapCb->pst.selector   = lSap->selector;

/* If multiple regions support 
   Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   egtcSapCb->pst.region   = avPgwCb.init.region;
   egtcSapCb->pst.pool     = avPgwCb.init.pool;
#else
   egtcSapCb->pst.region   = lSap->mem.region;
   egtcSapCb->pst.pool     = lSap->mem.pool;
#endif

   egtcSapCb->pst.prior      = 0;
   egtcSapCb->pst.route      = 0;
   egtcSapCb->pst.srcProcId  = SFndProcId();
   egtcSapCb->pst.srcEnt     = avPgwCb.init.ent;
   egtcSapCb->pst.srcInst    = avPgwCb.init.inst;

   egtcSapCb->pst.dstProcId   = lSap->dstProcId;
   egtcSapCb->pst.dstEnt     = lSap->dstEntId;
   egtcSapCb->pst.dstInst    = lSap->dstInstId;

   egtcSapCb->suId           = lSap->suId;
   egtcSapCb->spId           = lSap->spId;
   egtcSapCb->nmbConn        = lSap->nmbConn;
   egtcSapCb->state          = LAV_SAP_CFG;

   ret = cmHashListInit(&(egtcSapCb->ueTunnelCbHlCp), /* hash list Cp */
          AV_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          avPgwCb.init.region,                /* Mem region for HL */
          avPgwCb.init.pool);

    if (ROK != ret)
    {
       AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV035, ERRZERO,
                  "avPgwCfgEgtcUSap: Hash List Initialization failed\n");

       AV_PDNGW_SENDTSK_FREE(egtcSapCb, sizeof(AvPgwEgtSapCb));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       AV_PGW_SND_RETVALUE(ret);
    }

   avPgwCb.egtCSapCb = egtcSapCb;
   AV_PGW_SND_RETVALUE(ROK);
}


/*
 *
 *      Fun:   avPgwGenCfg
 *
 *      Desc:  Stores the general LTE CNE configuration for PGW.
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwGenCfg
(
AvPgwGenCfg *genCfg,    /* General configuration */
CmStatus    *status     /* Return status */
)
#else
PRIVATE S16 avPgwGenCfg(genCfg, status)
AvPgwGenCfg *genCfg;       /* General configuration */
CmStatus    *status;    /* Return status */
#endif
{
   S16  ret;

   AV_PGW_SND_TRC2(avPgwGenCfg)

   /****************************************************
    * Validate the general configuration elements      *
    ****************************************************/
   ret = avPgwValidateGenCfg(genCfg);   

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      AV_PGW_SND_RETVALUE(ret);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (avPgwCb.init.cfgDone == TRUE) /* Reconfiguration */
   {
      (Void) cmMemcpy((U8 *)&avPgwCb.init.lmPst, (U8 *) &genCfg->lmPst,
                      (PTR)sizeof(Pst));
      avPgwCb.init.lmPst.srcProcId = avPgwCb.init.procId;
      avPgwCb.init.lmPst.srcEnt    = avPgwCb.init.ent;
      avPgwCb.init.lmPst.srcInst   = avPgwCb.init.inst;
      avPgwCb.init.lmPst.event     = EVTNONE;

      AV_PGW_SND_RETVALUE(ROK);
   } /* end of reconfiguration */

   cmMemset((U8 *)&avPgwCb.pgwCfg,0,sizeof(AvPgwGenCfg));
   cmMemcpy ((U8 *)&avPgwCb.pgwCfg, (U8 *)genCfg, sizeof (AvPgwGenCfg));

   /***********************************
    * Initialize the elements of AvPgwCb *
    ***********************************/ 
  
   (Void) cmMemcpy((U8 *)&avPgwCb.init.lmPst, (U8 *) &genCfg->lmPst,
                   (PTR)sizeof(Pst));
    avPgwCb.init.lmPst.srcProcId = avPgwCb.init.procId;
    avPgwCb.init.lmPst.srcEnt    = avPgwCb.init.ent;
    avPgwCb.init.lmPst.srcInst   = avPgwCb.init.inst;
    avPgwCb.init.lmPst.event     = EVTNONE;


   ret = cmHashListInit(&(avPgwCb.ueCbHlst),  /* hash list Cp */
          AV_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          avPgwCb.init.region,                /* Mem region for HL */
          avPgwCb.init.pool);

   if (ROK != ret)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV036, ERRZERO,
                  "avPgwGenCfg: Hash List Initialization failed\n");
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      AV_PGW_SND_RETVALUE(ret);
   }

   avPgwCb.gtpUTeIdCntr = 0;

   avPgwCb.init.cfgDone = TRUE;

   AV_PGW_SND_RETVALUE(ROK);

} /* avPgwGenCfg */


/******************************************************************************/
/*                   Control Request Support Functions                        */
/******************************************************************************/
/*
 *
 *      Fun:   avPgwGenCntrl
 *
 *      Desc:  Process the general control request
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwGenCntrl
(
AvCntrl *cntrl,         /* Control structure */
CmStatus   *status        /* Return status */
)
#else
PRIVATE S16 avPgwGenCntrl(cntrl, status)
AvCntrl *cntrl;         /* Control structure */
CmStatus   *status;        /* Return status */
#endif
{
   TRC2(avPgwGenCntrl);

   status->status = LCM_PRIM_NOK;

   switch (cntrl->u.pgwCntrl.action)
   {
      case AENA:
      {
         switch (cntrl->u.pgwCntrl.subAction)
         {
#ifdef DEBUGP
            case SADBG:
            {
               avPgwCb.init.dbgMask |= (cntrl->u.pgwCntrl.u.dbg.dbgMask);
               break;
            }

#endif /* DEBUGP */
            case SAUSTA: 
            {
               avPgwCb.init.usta = TRUE;
               break;
            }
            default:
            {
               status->reason = LCM_REASON_INVALID_SUBACTION;
               RETVALUE(LCM_REASON_INVALID_SUBACTION);
            }
         }
         break;
      }
      case ADISIMM:
      {
         switch (cntrl->u.pgwCntrl.subAction)
         {
#ifdef DEBUGP
            case SADBG:
            {
               avPgwCb.init.dbgMask &= ~(cntrl->u.pgwCntrl.u.dbg.dbgMask);
               break;
            }
#endif /* DEBUGP */
            case SAUSTA:
            {
               avPgwCb.init.usta = FALSE;
               break;
            }
            default:
            {
               status->reason = LCM_REASON_INVALID_SUBACTION;
               RETVALUE(LCM_REASON_INVALID_SUBACTION);
            }
         }
         break;
      }
      case ASHUTDOWN:
      {
         break;
      }
      default:
      {
         status->reason = LCM_REASON_INVALID_ACTION; 
         RETVALUE(LCM_REASON_INVALID_ACTION);
      }
   } 

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   RETVALUE(ROK);

} /* avPgwGenCntrl */
  
/*
 *
 *      Fun:   avPgwValidateEgUSapCntrl
 *
 *      Desc:  Validates the LTE CNE's EGT SAP control parameters. 
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwValidateEgUSapCntrl
(
AvCntrl  *cntrl         /* general control structure */
)
#else
PRIVATE S16 avPgwValidateEgUSapCntrl(cntrl)
AvCntrl  *cntrl;        /* general control structure */
#endif
{
   S16        ret = LCM_REASON_NOT_APPL; /* Return value */

   TRC2(avPgwValidateEgUSapCntrl);

   switch (cntrl->u.pgwCntrl.action)
   {
      case ADEL:
      case ABND:
      case AUBND:
         break;
      default:
      {
         ret = LCM_REASON_INVALID_ACTION;
         RETVALUE(ret);
      }
   }

   RETVALUE(ret);

} /* avPgwValidateLSapCntrl */

/*
 *
 *      Fun:   avPgwEnaUSap
 *
 *      Desc:  Enable the lower SAP 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwEnaUSap
(
AvPgwCntrl *cntrl,        /* Control structure */
CmStatus   *status       /* Return status */
)
#else
PRIVATE S16 avPgwEnaUSap(cntrl, status)
AvPgwCntrl *cntrl;        /* Control structure */
CmStatus   *status;      /* Return status */
#endif
{
   AvPgwEgtSapCb *lSapCb = NULLP;

   TRC2(avPgwEnaUSap);

   if(cntrl->u.sap.id == avPgwCb.egtCSapCb->spId)
      lSapCb = avPgwCb.egtCSapCb;
   else if(cntrl->u.sap.id == avPgwCb.egtUSapCb->spId)
      lSapCb = avPgwCb.egtUSapCb;
   else if(cntrl->u.sap.id == avPgwCb.egtEchoSapCb->spId)
      lSapCb = avPgwCb.egtEchoSapCb;

   /* Verify if SapCb exists */
   if (NULLP == lSapCb)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV037, (ErrVal) 0,
            "avPgwEnaUSap: Sap Cb not configured");
      RETVALUE(RFAILED);
   }
   EuLiEgtBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);

   RETVALUE(ROK);

} /* avPgwEnaUSap */

/*
 *
 *      Fun:   avPgwUbndEgUSap
 *
 *      Desc:  Unbind and disable the lower SAP 
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwUbndEgUSap
(
void
)
#else
PRIVATE S16 avPgwUbndEgUSap(Void )
#endif
{
   AvPgwEgtSapCb *lSapCb; 
   S16 ret = ROK;

   AV_PGW_SND_TRC2(avPgwUbndEgUSap);

   lSapCb = avPgwCb.egtUSapCb;


   if (lSapCb->state == LAV_SAP_UBND ||
       lSapCb->state == LAV_SAP_CFG)
   {
     AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                "avPgwUbndEgUSap: SAP already unbound \n"));
      RETVALUE(ROK);
   }
   EuLiEgtUbndReq(&(avPgwCb.egtUSapCb->pst), avPgwCb.egtUSapCb->spId, 0);
   lSapCb->state = LAV_SAP_UBND;

   RETVALUE(ret);

} /* avPgwUbndEgUSap */

/*
 *
 *      Fun:   avPgwDelEgUSap
 *
 *      Desc:  Delete the lower SAP 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwDelEgUSap
(
Void
)
#else
PRIVATE S16 avPgwDelEgUSap(Void)
#endif
{

   AvPgwEgtSapCb *lSapCb = NULLP; 
   AV_PGW_SND_TRC2(avPgwDelEgUSap);

   lSapCb = avPgwCb.egtUSapCb;
   if(avPgwCb.egtUSapCb->state == LAV_SAP_ISBOUND)
   {
      avPgwUbndEgUSap();
   }
   RETVALUE(ROK);

} /* avPgwDelEgUSap */
/*
 *
 *      Fun:   avPgwLSapCntrl
 *
 *      Desc:  Process the EGT SAP control request
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwLSapCntrl
(
AvCntrl *cntrl,        /* Control structure */
CmStatus   *status        /* Return status */
)
#else
PRIVATE S16 avPgwLSapCntrl(cntrl, status)
AvCntrl *cntrl;        /* Control structure */
CmStatus   *status;       /* Return status */
#endif
{
   S16 ret;

   TRC2(avPgwLSapCntrl);

   ret = avPgwValidateEgUSapCntrl (cntrl);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (cntrl->u.pgwCntrl.action)
   {
      case ABND:
      {
         if ((ret = avPgwEnaUSap(&cntrl->u.pgwCntrl, status)) != ROK)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_SAP;

#if (ERRCLASS & ERRCLS_ADD_RES)
            AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV038, ERRZERO,
                  "avPgwLSapCntrl, Failed to bind and enable the EGT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
            RETVALUE(ret);
         }
         break; 
      }
      case AUBND:
      {
         if ((ret = avPgwUbndEgUSap ()) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV039, ERRZERO,
               "avPgwLSapCntrl, Failed to Unbind and disable the EGT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
         }
         break;
      }
      case ADEL:
      {
         if ((ret = avPgwDelEgUSap ()) != ROK)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = ret;

#if (ERRCLASS & ERRCLS_ADD_RES)
            AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV040, ERRZERO,
                       "avPgwLSapCntrl, Couldn't delete the EGT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
            RETVALUE(ret);
         }
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
            AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV041, ERRZERO,
                       "avPgwLSapCntrl, Unrecognized action ");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);

} /* avPgwLSapCntrl */

/******************************************************************************/
/*                   PGW Control Support Functions                     */
/******************************************************************************/
/*
 *
 *      Fun:   avPgwCreateRcvTsk
 *
 *      Desc:  Function creates the Receiving thread for PDN-GW
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwCreateRcvTsk
(
)
#else
PRIVATE S16 avPgwCreateRcvTsk()
#endif
{
   Buffer       *sampleBuf;
   Pst           selfPst;
   S16           ret = ROK;

   AV_PGW_SND_TRC2(avPgwCreateRcvTsk)
      
   /* Create system task for PDNGW Application [Exclusive for Receiving data] */
   if (SCreateSTsk ((SSTskPrior)PRIOR0, &avTskId) != ROK)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV042, (ErrVal) 0,
                     "SCreateSTsk failed \n");
      RETVALUE(RFAILED);
   }
   
   /* Register PGW 2nd instance */
   if (SRegTTsk(
            (Ent)ENTEU, (Inst)AV_PGW_SECOND_INSTANCE, (Ttype)TTNORM, PRIOR0,
            avPgwRcvActvInit, avPgwRcvActvTsk ) != ROK)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV043, (ErrVal) 0,
                     "SRegTTsk failed \n");
      SDestroySTsk(avTskId);
      RETVALUE(RFAILED);
   }
   
   /* PGW Recv */
   if (SAttachTTsk (
            ENTEU, (Inst)AV_PGW_SECOND_INSTANCE, avTskId) != ROK)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV044, (ErrVal) 0,
                     "SAttachTTsk failed \n");

      SDeregTTsk((Ent)ENTEU, (Inst)AV_PGW_SECOND_INSTANCE);
      SDestroySTsk(avTskId);
      RETVALUE(RFAILED);
   }
   
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, 
                        "Created PDN-GW Receiver Thread for capturing packets\n"));


   /* Make this newly created thread wait to capture packets from outside */

   /* Allocate Buffer for posting to avActvTsk */
   ret = SGetMsg(avPgwCb.init.region, avPgwCb.init.pool, 
                 &sampleBuf);
   if (ROK != ret)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV045, (ErrVal)ret,
                          "Trigger PDN-GW Receiver Thread - allocate mBuf failed \n");

      SDetachTTsk(avTskId, AV_PGW_SECOND_INSTANCE);
      SDeregTTsk(avTskId, AV_PGW_SECOND_INSTANCE);
      SDestroySTsk(avTskId);
      RETVALUE(RFAILED);
   }

   selfPst.srcProcId = SFndProcId();
   selfPst.srcEnt    = ENTEU; /* Source PDN-GW */
   selfPst.srcInst   = AV_PGW_PRIM_INSTANCE; /* PDN-GW instance 0 */
   selfPst.dstProcId = SFndProcId();
   selfPst.dstEnt    = ENTEU; /* Destination PDN-GW */
   selfPst.dstInst   = AV_PGW_SECOND_INSTANCE; /* PDN-GW instance 1 */
   selfPst.selector  = 0; /* Loose coupled */
   selfPst.route     = RTESPEC;
   selfPst.prior     = PRIOR0;
   selfPst.event     = EVTAVRCVARPREQ;
   selfPst.region    = avPgwCb.routeCb.init.region;
   selfPst.pool      = avPgwCb.routeCb.init.pool;
   
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, 
                        "Trigger PDN-GW Receiver Thread - Start\n"));
   SPstTsk(&selfPst, sampleBuf); 

   RETVALUE(ret);
}
/*
 *
 *      Fun:   avPgwSndArpReq
 *
 *      Desc:  Process the Arp request
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 avPgwSndArpReq
(
AvPgwCntrl *cntrl,         /* Control structure */
CmStatus   *status           /* Return status */
)
#else
PRIVATE S16 avPgwSndArpReq(cntrl, status)
AvPgwCntrl *cntrl;         /* Control structure */
CmStatus   *status;          /* Return status */
#endif
{
   S16  ret;

   AV_PGW_SND_TRC2(avPgwSndArpReq)

   /* Create the receive task for PGW - This should initialize all route
      context control block fields */
   ret = avPgwCreateRcvTsk();

   if (ROK != ret)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV046, (ErrVal) 0,
                          "avPgwCreateRcvTsk failed \n");
      status->status = LCM_PRIM_NOK;
      RETVALUE (ret);
   }

#ifdef AV_PERF

   /* Inform Layer manager back through Alarm */
   avSendLmAlarm(LCM_CATEGORY_PROTOCOL, LAV_EVENT_PGW_ROUTECFM, 
                 LCM_CAUSE_UNKNOWN);

   RETVALUE(ROK);

#else

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                        "Sending ARP request to the External Network Node\
                         IP Address(%x)\n",
                        avPgwCb.routeCb.ip2Mac[0].ipAddr));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                        "Sending ARP request to the External Network Node\
                         IP Address(%lx)\n",
                        avPgwCb.routeCb.ip2Mac[0].ipAddr));
#endif

   /* Now send the ARP Request to primary external network node */

   ret = avPgwSendArpReqPkt(avPgwCb.routeCb.ip2Mac[0].ipAddrStr);
   if (ROK != ret)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV047, (ErrVal)0,\
                           "avPgwSendArpReqPkt failed \n");
      status->status = LCM_PRIM_NOK;
      RETVALUE (ret);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   RETVALUE(ROK);

#endif   /* AV_PERF */

} /* avPgwSndArpReq */



/*
 *
 *      Fun:   avPgwSndActvInit
 *
 *      Desc:  Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI 
PUBLIC S16 avPgwSndActvInit
(
Ent    entity,         /* entity */
Inst   inst,           /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 avPgwSndActvInit(entity, inst, region, reason)
Ent    entity;         /* entity */
Inst   inst;           /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   TRC3(avPgwSndActvInit);
   
   cmMemset ((U8 *)&avPgwCb, 0, sizeof(AvPgwCb));
   avPgwCb.init.procId = SFndProcId();

   avPgwCb.init.ent = entity;
   avPgwCb.init.inst = inst;
   avPgwCb.init.region = region;
   avPgwCb.init.reason = reason;
   avPgwCb.init.cfgDone = FALSE;
   avPgwCb.init.pool = 0;
   avPgwCb.init.acnt = FALSE;

   cmLListInit(&(avPgwCb.pendingEvntLst));

#ifdef AV_USTA
   avPgwCb.init.usta = TRUE;
#else
   avPgwCb.init.usta = FALSE;
#endif /* AV_USTA */

#ifdef DEBUGP
   avPgwCb.init.dbgMask = 0;
#endif /* DEBUGP */

   RETVALUE(ROK);

} /* avPgwSndActvInit */

/*
 *
 *      Fun:   avPgwRcvActvInit
 *
 *      Desc:  PDN-GW Receive Task initialization function
               Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  av_mi.c
 *
*/
#ifdef ANSI 
PUBLIC S16 avPgwRcvActvInit
(
Ent    entity,         /* entity */
Inst   inst,           /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 avPgwRcvActvInit(entity, inst, region, reason)
Ent    entity;         /* entity */
Inst   inst;           /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   AvPgwRouteCb *routCb;
   S16 ret;

   TRC3(avPgwRcvActvInit);

   cmMemset ((U8 *)&avPgwCb.routeCb, 0, sizeof(AvPgwRouteCb)); 

   routCb = &avPgwCb.routeCb;

   routCb->init.procId = SFndProcId();

   routCb->init.ent = entity;
   routCb->init.inst = inst;
   routCb->init.region = region;
   routCb->init.reason = reason;
   routCb->init.cfgDone = FALSE;
   routCb->init.pool = avPgwCb.init.pool;
   routCb->init.acnt = FALSE;

#ifdef AV_USTA
   routCb->init.usta = TRUE;
#else
   routCb->init.usta = FALSE;
#endif /* AV_USTA */

#ifdef DEBUGP
   routCb->init.dbgMask = 0x0;
#endif /* DEBUGP */

   /* Initialize all route parameters */
   ret = avPgwRouteInit();

   if(ROK != ret)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV048, (ErrVal) 0,
                    "avPgwRcvActvInit: PDN-GW routing initialization failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

} /* avPgwRcvActvInit */




/**********************************************************************

         End of file:     av_mi.c@@/main/1 - Tue Aug 17 18:54:33 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -      sp                1.LTE-CNE:PGW Initial Release
*********************************************************************91*/
