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
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:     C source code for LM Module
  
     File:     qo_mi.c
  
     Sid:      qo_mi.c@@/main/1 - Thu Jul  8 21:22:20 2010
  
     Prg:      ap
  
*********************************************************************21*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "lqo.h"
#include "qo.h"
#include "qosm.h"
#include "qo_err.h"
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

#include "lqo.x"
#include "qo.x"
#include "qosm.x"


SSTskId       qoTskId;


#ifdef QO_SGW
/* SGW Global structure. */
PUBLIC QoSgwCb qoSgwCb;
#endif /* QO_SGW */


#ifdef QO_SGW

PRIVATE S16 qoSgwGenCfg ARGS((QoGenCfg *genCfg,
                              CmStatus *status));

PRIVATE S16 qoSgwEgUSapCfg ARGS((QoLSapCfg *lSap,
                                  CmStatus *status));

PRIVATE S16 qoSgwPgwCSapCfg ARGS((QoLSapCfg *lSap,
                                  CmStatus *status));

PRIVATE S16 qoSgwMmeCSapCfg ARGS((QoLSapCfg *lSap,
                                  CmStatus *status));

PRIVATE S16 qoSgwSgsnCSapCfg ARGS((QoLSapCfg *lSap,
                                  CmStatus *status));

PRIVATE S16 qoSgwEchoCSapCfg ARGS((QoLSapCfg *lSap,
                                  CmStatus *status));

PRIVATE S16 qoSgwGenCntrl ARGS((QoCntrl *cntrl, 
                                CmStatus *status));

PRIVATE S16 qoSgwValidateEgUSapCntrl ARGS((QoCntrl  *cntrl));

PRIVATE S16 qoSgwLSapCntrl ARGS((QoCntrl *cntrl,
                                 CmStatus *status));

PRIVATE S16 qoSgwEnaUSap ARGS((QoCntrl *cntrl,
         CmStatus   *status 
         ));

PRIVATE S16 qoSgwUbndEgUSap ARGS((Void));

PRIVATE S16 qoSgwDelEgUSap ARGS((Void));

#endif


PRIVATE S16 qoSendLmCfm ARGS((Pst    *pst,
                              Header *hdr,
                              U8      type,
                              QoMngmt *cfm));

/******************************************************************************/
/*                   Management Primitives                                    */
/******************************************************************************/

/******************************************************************************/
/*                   Configuration Request Primitive                          */
/******************************************************************************/
/*
 *
 *       Fun:   QoMiLqoCfgReq - Configuration Request
 *
 *       Desc:  This function is used by the layer manager to configure
 *              the LTE CNE layer. The layer responds with a Configuration
 *              Confirm "QoMiLqoCfgCfm" to the layer manager after
 *              completing the configuration.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: Configuration must be performed in the following
 *              sequence:
 *              1) SGW General configuration (STQOGEN)
 *              2) Lower sap configuration (STQOSZTSAP)
 *              3) SGW General configuration (STQOGEN)
 *              4) SGW lower sap configurations( STQOEGUSAP, STQOMMECSAP
 *                 STQOPGWCSAP)
 *              
 *       File:  qo_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 QoMiLqoCfgReq
(
Pst     *pst,
QoMngmt *cfg            /* Config structure */
)
#else
PUBLIC S16 QoMiLqoCfgReq(pst, cfg)
Pst     *pst;
QoMngmt *cfg;           /* Config structure */
#endif
{
   S16 ret = ROK;       /* Return Value */
   QoMngmt cfgCfm;      /* Config confirm */

   TRC3(QoMiLqoCfgReq);

   cmMemset ((U8 *)&cfgCfm, 0, sizeof (QoMngmt));

   QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
      "QoMiLqoCfgReq (pst, elmId (%d))\n", cfg->hdr.elmId.elmnt));

      
#ifdef QO_SGW
   switch(cfg->hdr.elmId.elmnt)
   {
      case STQOGEN:
         {
            ret = qoSgwGenCfg(&cfg->u.cfg.s.sgwGenCfg, &cfgCfm.cfm);
            break;
         }
      case STQOEGUSAP:
         {
            ret = qoSgwEgUSapCfg(&cfg->u.cfg.s.sgwLSapCfg, &cfgCfm.cfm);
            break;
         }
      case STQOPGWCSAP:
         {
            ret = qoSgwPgwCSapCfg(&cfg->u.cfg.s.sgwLSapCfg, &cfgCfm.cfm);
            break;
         }
      case STQOMMECSAP:
         {
            ret = qoSgwMmeCSapCfg(&cfg->u.cfg.s.sgwLSapCfg, &cfgCfm.cfm);
            break;
         }
      case STQOSGSNCSAP:
         {
            ret = qoSgwSgsnCSapCfg(&cfg->u.cfg.s.sgwLSapCfg, &cfgCfm.cfm);
            break;
         }
      case STQOECHOCSAP:
         {
            ret = qoSgwEchoCSapCfg(&cfg->u.cfg.s.sgwLSapCfg, &cfgCfm.cfm);
            break;
         }
      default:
         break;
   }/* End of switch */
#endif /* QO_SGW */


   qoSendLmCfm(pst, &cfg->hdr, TCFG, &cfgCfm);
   SPutSBuf(pst->region, pst->pool, (Data *)cfg, sizeof(QoMngmt));

   RETVALUE(ret);

} /* End of QoMiLqoCfgReq */

/******************************************************************************/
/*                   Control Request Primitive                                */
/******************************************************************************/
/*
*
*       Fun:   QoMiLqoCntrlReq - Control Request
*
*       Desc:  This primitive is used to control the specified element.
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  qo_mi.c
*/
#ifdef ANSI
PUBLIC S16 QoMiLqoCntrlReq
(
Pst     *pst,
QoMngmt *cntrl              /* Control structure */
)
#else
PUBLIC S16 QoMiLqoCntrlReq(pst, cntrl)
Pst     *pst;
QoMngmt *cntrl;             /* Control structure */
#endif
{
   S16 ret = ROK;           /* Return value */
   QoMngmt cntrlCfm;        /* Control confirm */

   TRC3(QoMiLqoCntrlReq);

   cmMemset ((U8 *)&cntrlCfm, 0, sizeof (QoMngmt));

   QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
      "QoMiLqoCntrlReq (pst, elmId (%d))\n", cntrl->hdr.elmId.elmnt));

   /* check element */
#ifdef QO_SGW
   /* Check if General Configuration is done */
   if (!qoSgwCb.init.cfgDone) 
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG,EQO002,0,
            "QoMiLqoCntrlReq: S-GW General configuration not done ");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      qoSendLmCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

      RETVALUE(ROK);
   }

   switch(cntrl->hdr.elmId.elmnt)
   {
      case STQOGEN:
         {
            /* general (applicable layer-wide) control */
            ret = qoSgwGenCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
            break;
         }
      case STQOEGUSAP:
         {
            /* S-GW 's lower SAP with EGTP for eNodeB */
            ret = qoSgwLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
            break;
         }
      case STQOPGWCSAP:
         {
            /* S-GW 's lower SAP with EGTP for eNodeB */
            ret = qoSgwLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
            break;
         }
      case STQOMMECSAP:
      case STQOSGSNCSAP:
      case STQOECHOCSAP:
         {
            /* S-GW 's lower SAP with EGTP for eNodeB */
            ret = qoSgwLSapCntrl(&cntrl->u.cntrl, &cntrlCfm.cfm);
            break;
         }
      default:
         {
            /* invalid element */
            QOLOGERROR(ERRCLS_DEBUG, EQO003, (ErrVal) cntrl->hdr.elmId.elmnt,
                  "QoMiLqoCntrlReq: Invalid Element in control request");
            cntrlCfm.cfm.status = LCM_PRIM_NOK;
            cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
            break;
         }
   } /* end switch */
#endif /* QO_SGW */

   qoSendLmCfm(pst, &cntrl->hdr, TCNTRL, &cntrlCfm);
   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(QoMngmt));

   RETVALUE(ret);

} /* QoMiLqoCntrlReq */

/******************************************************************************/
/*                   Statistics Request Primitive                             */
/******************************************************************************/
/*
*
*       Fun:   QoMiLqoStsReq - Statistics Request
*
*       Desc:  This primitive is used by the Layer Manager to solicit 
*              statistics information. The statistics are returned
*              in the  QoMiLqoStsCfm primitive.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  qo_mi.c
*
*/

#ifdef ANSI
PUBLIC S16 QoMiLqoStsReq
(
Pst     *pst,
Action   action,
QoMngmt *sts                 /* Statistics structure */
)
#else
PUBLIC S16 QoMiLqoStsReq(pst, action, sts)
Pst     *pst;
Action   action;
QoMngmt *sts;                /* Statistics structure */
#endif
{
   S16 ret = ROK;            /* Return value */

   RETVALUE(ret);

} /* QoMiLqoStsReq */


/******************************************************************************/
/*                   Status  Request Primitive                                */
/******************************************************************************/
/*
*
*       Fun:   Status Request
*
*       Desc:  This primitive is used by the Layer Manager to solicit
*              status information. The information is returned via the
*              QoMiLqoStaCfm primitive.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  qo_mi.c
*
*/
#ifdef ANSI
PUBLIC S16 QoMiLqoStaReq
(
Pst     *pst,
QoMngmt *sta                 /* Status structure */
)
#else
PUBLIC S16 QoMiLqoStaReq(pst, sta)
Pst     *pst;
QoMngmt *sta;                /* Status structure */
#endif
{
   S16 ret = ROK;            /* Return value */

   TRC3(QoMiLqoStaReq)

   RETVALUE(ret);

} /* QoMiLqoStaReq */


/******************************************************************************/
/*                  Indication Generation Functions                           */
/******************************************************************************/
/*
 *
 *      Fun:   qoSendLmAlarm
 *
 *      Desc:  This function is used to send an Alarm / Unsolicited Status
 *             Indication to the Layer Manager
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PUBLIC Void qoSendLmAlarm
(
U16 category,           /* category */
U16 event,              /* event generated */
U16 cause,              /* cause */
QoUstaDgn *dgn
)
#else
PUBLIC Void qoSendLmAlarm(category, event, cause, dgn)
U16 category;           /* category */
U16 event;              /* event generated */
U16 cause;              /* cause */
QoUstaDgn *dgn;
#endif
{
   QoMngmt usta;
   Pst     pst;

   TRC2(qoSendLmAlarm);

   /* initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(usta));
   (Void)cmMemcpy((U8 *)&usta.u.usta, (U8 *)dgn, sizeof(QoUstaDgn));

   usta.hdr.elmId.elmnt = STQOGEN;
#ifdef QO_SGW
   /* Check if general configuration is done */
   if (qoSgwCb.init.cfgDone == FALSE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO004, (ErrVal)0,
            "Status Indication requested; general configuration not done!");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVOID;
   }

   if (qoSgwCb.init.usta == FALSE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO005, (ErrVal)0,
            "usta is not initialized!");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVOID;
   }
   usta.hdr.entId.ent  = qoSgwCb.init.ent;
   usta.hdr.entId.inst = qoSgwCb.init.inst;
   cmMemcpy((U8 *)&pst, (U8 *)&(qoSgwCb.init.lmPst), sizeof(Pst));
#endif /* QO_SGW */

   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event = event;
   usta.u.usta.alarm.cause = cause;

   /* update the date and time */
   (Void) SGetDateTime(&usta.u.usta.alarm.dt);

   QoMiLqoStaInd(&pst, &usta);

   RETVOID;

} /* end of qoSendLmAlarm */



/******************************************************************************/
/*                   Management Interface Support Functions                   */
/******************************************************************************/

/******************************************************************************/
/*                   Layer Manager Confirm  Function                          */
/******************************************************************************/
/*
 *
 *      Fun:   qoSendLmCfm
 *
 *      Desc:  Generate the Confirm to the Layer Manager
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSendLmCfm
(
Pst     *pst,                           
Header  *hdr,         /* Pointer to the Header structure */
U8       type,        /* Confirmation Type */
QoMngmt *cfm         /* Management Structure */
)
#else
PRIVATE S16 qoSendLmCfm(pst, hdr, type, cfm)
Pst     *pst;
Header  *hdr;         /* Pointer to the Header structure */
U8       type;       /* Confirmation Type */
QoMngmt *cfm;        /* Management Structure */
#endif
{

   Pst cfmPst;        /* Confirmation Post Structure */

   TRC2(qoSendLmCfm);

   /* Fill up the header in the confirm structure */
   cmMemset( (U8 *)&cfm->hdr, '\0', sizeof(cfm->hdr));       
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.transId = hdr->transId;

#ifdef QO_SGW
   cfm->hdr.entId.ent = qoSgwCb.init.ent;
   cfm->hdr.entId.inst = qoSgwCb.init.inst;
   cfmPst.srcEnt  = qoSgwCb.init.ent;
   cfmPst.srcInst = qoSgwCb.init.inst;
#endif /* QO_SGW */

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
            QoMiLqoCfgCfm (&cfmPst, cfm);

            QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
                     "qoSendLmCfm: sent QoMiLqoCfgCfm (cfmType: %d) \n", type));
            break;
         }
      case TCNTRL:
         { 
            /* Send Control Confirm */
            QoMiLqoCntrlCfm (&cfmPst, cfm);

            QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
                     "qoSendLmCfm: sent QoMiLqoCntrlCfm (cfmType: %d) \n", type));
            break;
         }
      case TSTS:
         {
            /* Send Statistics Confirm */
            QoMiLqoStsCfm (&cfmPst, cfm);

            QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
                     "qoSendLmCfm: sent QoMiLqoStsCfm (cfmType: %d) \n", type));
            break;
         }
      case TSSTA:
         {
            /* Send Status Confirm */
            QoMiLqoStaCfm (&cfmPst, cfm);

            QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
                     "qoSendLmCfm: sent QoMiLqoStaCfm (cfmType: %d) \n", type));
            break;
         }
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf, "qoSendLmCfm : Invalid Type \n"));
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         break;
   }

   RETVALUE(ROK); 

} /* qoSendLmCfm */


/******************************************************************************/
/*                   Configuration Request Support Functions                  */
/******************************************************************************/

/******************************************************************************/
/*                   SGW General Configuration Support Functions              */
/******************************************************************************/
#ifdef QO_SGW
/*
 *
 *      Fun:   qoValidateSgwGenCfg
 *
 *      Desc:  Validates the LTE CNE general configuration. 
 *
 *      Ret:   LCM_REASON_NOT_APPL - ok
 *             LCM_REASON_INVALID_PAR_VAL - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoValidateSgwGenCfg
(
QoGenCfg *gen        /* General configuration */
)
#else
PRIVATE S16 qoValidateSgwGenCfg(gen)
QoGenCfg *gen;       /* General configuration */
#endif
{
   TRC2(qoValidateSgwGenCfg);

   if (gen == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO006, ERRZERO,
          "qoValidateSgwGenCfg,  NULL cfg ");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if ((gen->maxNmbEgtSaps <= 0))
   {
      QOLOGERROR(ERRCLS_DEBUG,EQO007, (ErrVal) gen->maxNmbEgtSaps, 
                  "qoValidateSgwGenCfg: Invalid Num of EGT Saps");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gen->maxNmbBearers <= 0)
   {
      QOLOGERROR(ERRCLS_DEBUG,EQO008, (ErrVal) gen->maxNmbBearers, 
                  "qoValidateSgwGenCfg: Invalid Num of bearers");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gen->maxNmbTunls <= 0)
   {
      QOLOGERROR(ERRCLS_DEBUG,EQO009, (ErrVal) gen->maxNmbTunls,
                  "qoValidateSgwGenCfg: Invalid Num of tunnels");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(gen->tmrRes <= 0)
   {
      QOLOGERROR(ERRCLS_DEBUG,EQO010, (ErrVal) gen->tmrRes,
                  "qoValidateSgwGenCfg: Invalid timer resolution");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* qoValidateSgwGenCfg */

/*
 *
 *      Fun:   qoSgwGenCfg
 *
 *      Desc:  Stores the general LTE CNE configuration for SGW.
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwGenCfg
(
QoGenCfg *genCfg,       /* General configuration */
CmStatus *status     /* Return status */
)
#else
PRIVATE S16 qoSgwGenCfg(genCfg, status)
QoGenCfg *genCfg;       /* General configuration */
CmStatus *status;    /* Return status */
#endif
{
   S16  ret;

   TRC2(qoSgwGenCfg);

   /****************************************************
    * Validate the general configuration elements      *
    ****************************************************/
   ret = qoValidateSgwGenCfg(genCfg);   

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (qoSgwCb.init.cfgDone == TRUE) /* Reconfiguration */
   {
      (Void) cmMemcpy((U8 *)&qoSgwCb.init.lmPst, (U8 *) &genCfg->lmPst,
                      (PTR)sizeof(Pst));
      qoSgwCb.init.lmPst.srcProcId = qoSgwCb.init.procId;
      qoSgwCb.init.lmPst.srcEnt    = qoSgwCb.init.ent;
      qoSgwCb.init.lmPst.srcInst   = qoSgwCb.init.inst;
      qoSgwCb.init.lmPst.event     = EVTNONE;

      RETVALUE(ROK);
   } /* end of reconfiguration */

   cmMemset((U8 *)&qoSgwCb.cfg,0,sizeof(QoGenCfg));
   cmMemcpy ((U8 *)&qoSgwCb.cfg, (U8 *)genCfg, sizeof (QoGenCfg));

   /***********************************
    * Initialize the elements of qoSgwCb *
    ***********************************/ 
  
   (Void) cmMemcpy((U8 *)&qoSgwCb.init.lmPst, (U8 *) &genCfg->lmPst,
                   (PTR)sizeof(Pst));
    qoSgwCb.init.lmPst.srcProcId = qoSgwCb.init.procId;
    qoSgwCb.init.lmPst.srcEnt    = qoSgwCb.init.ent;
    qoSgwCb.init.lmPst.srcInst   = qoSgwCb.init.inst;
    qoSgwCb.init.lmPst.event     = EVTNONE;

   /***********************************
    * Initialize the Hash Lists *
    ***********************************/ 
    ret = cmHashListInit(&qoSgwCb.peerCbHlCp, /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                    /* HL key type */
          qoSgwCb.init.region,                   /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    ret = cmHashListInit(&qoSgwCb.ueCbHlCp, /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                    /* HL key type */
          qoSgwCb.init.region,                   /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       cmHashListDeinit(&qoSgwCb.peerCbHlCp);
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }
    qoSgwCb.gtpuTeidCntr = 0;
    qoSgwCb.TransIdCntr = 0;
    qoSgwCb.t3.val = QO_SGW_EGT_T3_RESP_TMR;
    qoSgwCb.t3.enb = TRUE;

    qoSgwCb.init.cfgDone = TRUE;

    RETVALUE(ROK);

} /* qoSgwGenCfg */

/******************************************************************************/
/*                   EGT SAP Configuration Support Functions                 */
/******************************************************************************/
/*
 *
 *      Fun:   qoSgwValidateMmeCSapCfg
 *
 *      Desc:  Validates the LTE CNE lower EGT SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwValidateMmeCSapCfg
(
QoLSapCfg *lSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 qoSgwValidateMmeCSapCfg(lSap)
QoLSapCfg *lSap;     /* Lower SAP configuration */
#endif
{
   TRC2(qoSgwValidateMmeCSapCfg);

   if (lSap == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO011, ERRZERO,
          "qoSgwValidateMmeCSapCfg, lSap is NULL");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if (qoSgwCb.init.cfgDone == FALSE)
   {
       RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of qoSgwValidateMmeCSapCfg */

/*
 *
 *      Fun:   qoSgwValidateSgsnCSapCfg
 *
 *      Desc:  Validates the LTE CNE lower EGT SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwValidateSgsnCSapCfg
(
QoLSapCfg *lSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 qoSgwValidateSgsnCSapCfg(lSap)
QoLSapCfg *lSap;     /* Lower SAP configuration */
#endif
{
   TRC2(qoSgwValidateSgsnCSapCfg);

   if (lSap == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO011, ERRZERO,
          "qoSgwValidateMmeCSapCfg, lSap is NULL");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if (qoSgwCb.init.cfgDone == FALSE)
   {
       RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of qoSgwValidateSgsnCSapCfg */

/*
 *
 *      Fun:   qoSgwValidateEchoCSapCfg
 *
 *      Desc:  Validates the LTE CNE lower EGT SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwValidateEchoCSapCfg
(
QoLSapCfg *lSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 qoSgwValidateEchoCSapCfg(lSap)
QoLSapCfg *lSap;     /* Lower SAP configuration */
#endif
{
   TRC2(qoSgwValidateEchoCSapCfg);

   if (lSap == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO012, ERRZERO,
          "qoSgwValidateEchoCSapCfg, lSap is NULL");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if (qoSgwCb.init.cfgDone == FALSE)
   {
       RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of qoSgwValidateEchoCSapCfg */

/*
 *
 *      Fun:   qoSgwValidatePgwCSapCfg
 *
 *      Desc:  Validates the LTE CNE lower EGT SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwValidatePgwCSapCfg
(
QoLSapCfg *lSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 qoSgwValidatePgwCSapCfg(lSap)
QoLSapCfg *lSap;     /* Lower SAP configuration */
#endif
{
   TRC2(qoSgwValidatePgwCSapCfg);

   if (lSap == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO013, ERRZERO,
          "qoSgwValidatePgwCSapCfg, lSap is NULL");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if (qoSgwCb.init.cfgDone == FALSE)
   {
       RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of qoSgwValidatePgwCSapCfg */

/*
 *
 *      Fun:   qoSgwValidateEgUSapCfg
 *
 *      Desc:  Validates the LTE CNE lower EGT SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwValidateEgUSapCfg
(
QoLSapCfg *lSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 qoSgwValidateEgUSapCfg(lSap)
QoLSapCfg *lSap;     /* Lower SAP configuration */
#endif
{
   TRC2(qoSgwValidateEgUSapCfg);

   if (lSap == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO014, ERRZERO,
          "qoSgwValidateEgUSapCfg, lSap is NULL");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if (qoSgwCb.init.cfgDone == FALSE)
   {
       RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }

   if (lSap->suId >= qoSgwCb.cfg.maxNmbEgtSaps)
   {
      RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of qoSgwValidateEgUSapCfg */

/*
 *
 *      Fun:   qoSgwPgwCSapCfg
 *
 *      Desc:  Stores the LTE CNE lower SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwPgwCSapCfg
(
QoLSapCfg *lSap,     /* lower SAP configuration */
CmStatus     *status    /* Return status */
)
#else
PRIVATE S16 qoSgwPgwCSapCfg(lSap, status)
QoLSapCfg *lSap;     /* lower SAP configuration */
CmStatus     *status;   /* Return status */
#endif
{
   S16      ret;
   QoSgwEgtSapCb *lSapCb;

   QO_SGW_TRC2(qoSgwPgwCSapCfg);

   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the protocol parameters */
   ret = qoSgwValidatePgwCSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   /* Get Sap control block */
   lSapCb = qoSgwCb.pgwCSap;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

      /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = qoSgwCb.init.region;
      lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   QO_SGW_ALLOC(&lSapCb, sizeof(QoSgwEgtSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO015, ERRZERO,
          "qoSgwEgtSapCfg, Memory Allcation failed for Sap");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of QoSgwEgtSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = qoSgwCb.init.region;
   lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = qoSgwCb.init.ent;
   lSapCb->pst.srcInst   = qoSgwCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;
   lSapCb->nmbTunn       = lSap->nmbTunn;
   lSapCb->state = LQO_SAP_CFG;

   /***********************************
    * Initialize the Hash Lists *
    ***********************************/ 
    ret = cmHashListInit(&(lSapCb->tunnCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                    /* HL key type */
          qoSgwCb.init.region,                   /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    ret = cmHashListInit(&(lSapCb->tunnPendCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          sizeof(CmHashListEnt),                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                    /* HL key type */
          qoSgwCb.init.region,                   /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       cmHashListDeinit(&(lSapCb->tunnCbHlCp));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }
    cmMemcpy((U8 *)&(lSapCb->localAddr),(U8 *) &(lSap->localAddr), sizeof(CmTptAddr));
   /* Place lSapCb in qoSgwCb's LSap list */
   qoSgwCb.pgwCSap = lSapCb;

   RETVALUE(ret);

} /* qoSgwPgwCSapCfg */

/*
 *
 *      Fun:   qoSgwEgUSapCfg
 *
 *      Desc:  Stores the LTE CNE lower SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwEgUSapCfg
(
QoLSapCfg *lSap,     /* lower SAP configuration */
CmStatus     *status    /* Return status */
)
#else
PRIVATE S16 qoSgwEgUSapCfg(lSap, status)
QoLSapCfg *lSap;     /* lower SAP configuration */
CmStatus     *status;   /* Return status */
#endif
{
   S16      ret;
   QoSgwEgtSapCb *lSapCb;

   QO_SGW_TRC2(qoSgwEgUSapCfg);

   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the protocol parameters */
   ret = qoSgwValidateEgUSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   /* Get Sap control block */
   lSapCb = qoSgwCb.egUSap;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

      /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = qoSgwCb.init.region;
      lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   QO_SGW_ALLOC(&lSapCb, sizeof(QoSgwEgtSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO016, ERRZERO,
          "qoSgwEgtSapCfg, Memory Allcation failed for Sap");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of QoSgwEgtSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = qoSgwCb.init.region;
   lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = qoSgwCb.init.ent;
   lSapCb->pst.srcInst   = qoSgwCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;
   lSapCb->nmbTunn       = lSap->nmbTunn;

   lSapCb->state = LQO_SAP_CFG;

   /***********************************
    * Initialize the Hash Lists *
    ***********************************/ 
    ret = cmHashListInit(&(lSapCb->tunnCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          qoSgwCb.init.region,                /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    cmMemcpy((U8 *)&(lSapCb->localAddr),(U8 *) &(lSap->localAddr), sizeof(CmTptAddr));
    /* Transaction hash list need not be used by GTP-U hence not initialized. */

   /* Place lSapCb in qoSgwCb's LSap list */
   qoSgwCb.egUSap = lSapCb;

   RETVALUE(ret);

} /* qoSgwEgUSapCfg */

/*
 *
 *      Fun:   qoSgwMmeCSapCfg
 *
 *      Desc:  Stores the LTE CNE lower SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwMmeCSapCfg
(
QoLSapCfg *lSap,     /* lower SAP configuration */
CmStatus     *status         /* Return status */
)
#else
PRIVATE S16 qoSgwMmeCSapCfg(lSap, status)
QoLSapCfg *lSap;     /* lower SAP configuration */
CmStatus     *status;        /* Return status */
#endif
{
   S16      ret;
   QoSgwEgtSapCb *lSapCb;

   QO_SGW_TRC2(qoSgwMmeCSapCfg);

   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the protocol parameters */
   ret = qoSgwValidateMmeCSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   /* Get Sap control block */
   lSapCb = qoSgwCb.mmeCSap;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = qoSgwCb.init.region;
      lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   QO_SGW_ALLOC(&lSapCb, sizeof(QoSgwEgtSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO017, ERRZERO,
          "qoSgwEgtSapCfg, Memory Allcation failed for Sap");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of QoSgwEgtSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = qoSgwCb.init.region;
   lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = qoSgwCb.init.ent;
   lSapCb->pst.srcInst   = qoSgwCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;
   lSapCb->nmbTunn       = lSap->nmbTunn;

   lSapCb->state = LQO_SAP_CFG;

   /***********************************
    * Initialize the Hash Lists *
    ***********************************/ 
    ret = cmHashListInit(&(lSapCb->tunnCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          qoSgwCb.init.region,                /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    ret = cmHashListInit(&(lSapCb->tunnPendCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          sizeof(CmHashListEnt),              /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          qoSgwCb.init.region,                /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       cmHashListDeinit(&(lSapCb->tunnCbHlCp));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    cmMemcpy((U8 *)&(lSapCb->localAddr), (U8 *)&(lSap->localAddr), sizeof(CmTptAddr));
   /* Place lSapCb in qoSgwCb's LSap list */
   qoSgwCb.mmeCSap = lSapCb;

   RETVALUE(ret);
} /* qoSgwMmeCSapCfg */

/*
 *
 *      Fun:   qoSgwSgsnCSapCfg
 *
 *      Desc:  Stores the LTE CNE lower SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwSgsnCSapCfg
(
QoLSapCfg *lSap,     /* lower SAP configuration */
CmStatus     *status         /* Return status */
)
#else
PRIVATE S16 qoSgwSgsnCSapCfg(lSap, status)
QoLSapCfg *lSap;     /* lower SAP configuration */
CmStatus     *status;        /* Return status */
#endif
{
   S16      ret;
   QoSgwEgtSapCb *lSapCb;

   QO_SGW_TRC2(qoSgwSgsnCSapCfg);

   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the protocol parameters */
   ret = qoSgwValidateSgsnCSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   /* Get Sap control block */
   lSapCb = qoSgwCb.sgsnCSap;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = qoSgwCb.init.region;
      lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   QO_SGW_ALLOC(&lSapCb, sizeof(QoSgwEgtSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO017, ERRZERO,
          "qoSgwEgtSapCfg, Memory Allcation failed for Sap");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of QoSgwEgtSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = qoSgwCb.init.region;
   lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = qoSgwCb.init.ent;
   lSapCb->pst.srcInst   = qoSgwCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;
   lSapCb->nmbTunn       = lSap->nmbTunn;

   lSapCb->state = LQO_SAP_CFG;

   /***********************************
    * Initialize the Hash Lists *
    ***********************************/ 
    ret = cmHashListInit(&(lSapCb->tunnCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          0,                                  /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          qoSgwCb.init.region,                /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    ret = cmHashListInit(&(lSapCb->tunnPendCbHlCp), /* hash list Cp */
          QO_HASH_LIST_SIZE,                  /* HL bins */
          sizeof(CmHashListEnt),              /* Offset of HL Entry */
          FALSE,                              /* Allow dup. keys ? */
          CM_HASH_KEYTYPE_ANY,                /* HL key type */
          qoSgwCb.init.region,                /* Mem region for HL */
          qoSgwCb.init.pool);
    if (ret != ROK)
    {
       QO_SGW_FREE(lSapCb, sizeof(QoSgwEgtSapCb));
       cmHashListDeinit(&(lSapCb->tunnCbHlCp));
       status->status = LCM_PRIM_NOK;
       status->reason = ret;
       RETVALUE(ret);
    }

    cmMemcpy((U8 *)&(lSapCb->localAddr), (U8 *)&(lSap->localAddr), sizeof(CmTptAddr));
   /* Place lSapCb in qoSgwCb's LSap list */
   qoSgwCb.sgsnCSap = lSapCb;

   RETVALUE(ret);
} /* qoSgwSgsnCSapCfg */

/*
 *
 *      Fun:   qoSgwEchoCSapCfg
 *
 *      Desc:  Stores the LTE CNE lower SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwEchoCSapCfg
(
QoLSapCfg *lSap,     /* lower SAP configuration */
CmStatus     *status         /* Return status */
)
#else
PRIVATE S16 qoSgwEchoCSapCfg(lSap, status)
QoLSapCfg *lSap;     /* lower SAP configuration */
CmStatus     *status;        /* Return status */
#endif
{
   S16      ret;
   QoSgwEgtSapCb *lSapCb;

   QO_SGW_TRC2(qoSgwEchoCSapCfg);

   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the protocol parameters */
   ret = qoSgwValidateEchoCSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   /* Get Sap control block */
   lSapCb = qoSgwCb.echoCSap;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = qoSgwCb.init.region;
      lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   QO_SGW_ALLOC(&lSapCb, sizeof(QoSgwEgtSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      QOLOGERROR(ERRCLS_DEBUG, EQO018, ERRZERO,
          "qoSgwEgtSapCfg, Memory Allcation failed for Sap");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of QoSgwEgtSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

   /* If multiple regions support 
      Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = qoSgwCb.init.region;
   lSapCb->pst.pool     = qoSgwCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = qoSgwCb.init.ent;
   lSapCb->pst.srcInst   = qoSgwCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;

   lSapCb->state = LQO_SAP_CFG;
    cmMemcpy((U8 *)&(lSapCb->localAddr),(U8 *) &(lSap->localAddr), sizeof(CmTptAddr));
   /* Place lSapCb in qoSgwCb's LSap list */
   qoSgwCb.echoCSap = lSapCb;

   RETVALUE(ret);
} /* qoSgwEchoCSapCfg */

#endif /* QO_SGW */

/******************************************************************************/
/*                   Control Request Support Functions                        */
/******************************************************************************/

/******************************************************************************/
/*                   SGW General Control Support Functions                     */
/******************************************************************************/
/*
 *
 *      Fun:   qoSgwGenCntrl
 *
 *      Desc:  Process the general control request
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwGenCntrl
(
QoCntrl *cntrl,         /* Control structure */
CmStatus   *status        /* Return status */
)
#else
PRIVATE S16 qoSgwGenCntrl(cntrl, status)
QoCntrl *cntrl;         /* Control structure */
CmStatus   *status;        /* Return status */
#endif
{

   TRC2(qoSgwGenCntrl);

   status->status = LCM_PRIM_NOK;

   switch (cntrl->action)
   {
      case AENA:
      {
         switch (cntrl->subAction)
         {
#ifdef DEBUGP
            case SADBG:
            {
               qoSgwCb.init.dbgMask |= cntrl->u.dbg.dbgMask;
               break;
            }

#endif /* DEBUGP */
            case SAUSTA: 
            {
               qoSgwCb.init.usta = TRUE;
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
         switch (cntrl->subAction)
         {
#ifdef DEBUGP
            case SADBG:
            {
               qoSgwCb.init.dbgMask &= ~(cntrl->u.dbg.dbgMask);
               break;
            }
#endif /* DEBUGP */
            case SAUSTA:
            {
               qoSgwCb.init.usta = FALSE;
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

} /* qoSgwGenCntrl */
  
/******************************************************************************/
/*                   EGT SAP  Control Support Functions                       */
/******************************************************************************/
/*
 *
 *      Fun:   qoSgwValidateEgUSapCntrl
 *
 *      Desc:  Validates the LTE CNE's EGT SAP control parameters. 
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwValidateEgUSapCntrl
(
QoCntrl  *cntrl         /* general control structure */
)
#else
PRIVATE S16 qoSgwValidateEgUSapCntrl(cntrl)
QoCntrl  *cntrl;        /* general control structure */
#endif
{
   S16        ret = LCM_REASON_NOT_APPL; /* Return value */

   TRC2(qoSgwValidateEgUSapCntrl);

   switch (cntrl->action)
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

} /* qoMmeValidateLSapCntrl */


/*
 *
 *      Fun:   qoSgwLSapCntrl
 *
 *      Desc:  Process the EGT SAP control request
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwLSapCntrl
(
QoCntrl *cntrl,        /* Control structure */
CmStatus   *status        /* Return status */
)
#else
PRIVATE S16 qoSgwLSapCntrl(cntrl, status)
QoCntrl *cntrl;        /* Control structure */
CmStatus   *status;       /* Return status */
#endif
{
   S16 ret;

   TRC2(qoSgwLSapCntrl);

   ret = qoSgwValidateEgUSapCntrl (cntrl);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(RFAILED);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch (cntrl->action)
   {
      case ABND:
      {
         if ((ret = qoSgwEnaUSap(cntrl, status)) != ROK)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_SAP;

#if (ERRCLASS & ERRCLS_ADD_RES)
            QOLOGERROR(ERRCLS_DEBUG, EQO019, ERRZERO,
                  "qoSgwLSapCntrl, Failed to bind and enable the EGT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
            RETVALUE(ret);
         }
         break; 
      }
      case AUBND:
      {
         if ((ret = qoSgwUbndEgUSap ()) != ROK)
         {
#if (ERRCLASS & ERRCLS_ADD_RES)
            QOLOGERROR(ERRCLS_DEBUG, EQO020, ERRZERO,
               "qoSgwLSapCntrl, Failed to Unbind and disable the EGT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
         }
         break;
      }
      case ADEL:
      {
         if ((ret = qoSgwDelEgUSap ()) != ROK)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = ret;

#if (ERRCLASS & ERRCLS_ADD_RES)
            QOLOGERROR(ERRCLS_DEBUG, EQO021, ERRZERO,
                       "qoSgwLSapCntrl, Couldn't delete the EGT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
            RETVALUE(ret);
         }
         break;
      }
      default:
      {
#if (ERRCLASS & ERRCLS_ADD_RES)
            QOLOGERROR(ERRCLS_DEBUG, EQO022, ERRZERO,
                       "qoSgwLSapCntrl, Unrecognized action ");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);

} /* qoSgwLSapCntrl */


#ifdef QO_SGW
/*
 *
 *      Fun:   qoSgwEnaUSap
 *
 *      Desc:  Enable the lower SAP 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwEnaUSap
(
QoCntrl *cntrl,        /* Control structure */
CmStatus   *status       /* Return status */
)
#else
PRIVATE S16 qoSgwEnaUSap(cntrl, status)
QoCntrl *cntrl;        /* Control structure */
CmStatus   *status;      /* Return status */
#endif
{
   /*klocworks fix : initialising pointer */
   QoSgwEgtSapCb *lSapCb = NULLP;

   TRC2(qoSgwEnaUSap);

   if(cntrl->u.sap.id == qoSgwCb.egUSap->spId)
      lSapCb = qoSgwCb.egUSap;
   else if(cntrl->u.sap.id == qoSgwCb.pgwCSap->spId)
      lSapCb = qoSgwCb.pgwCSap;
   else if(cntrl->u.sap.id == qoSgwCb.mmeCSap->spId)
      lSapCb = qoSgwCb.mmeCSap;
   else if(cntrl->u.sap.id == qoSgwCb.echoCSap->spId)
      lSapCb = qoSgwCb.echoCSap;
   else if(cntrl->u.sap.id == qoSgwCb.sgsnCSap->spId)
      lSapCb = qoSgwCb.sgsnCSap;

   /* Verify if SapCb exists */
   if (NULLP == lSapCb)
   {
      QOLOGERROR(ERRCLS_DEBUG, EQO023, (ErrVal) 0,
            "qoSgwEnaUSap: Sap Cb not configured");
      RETVALUE(RFAILED);
   }
   EuLiEgtBndReq(&lSapCb->pst, lSapCb->suId, lSapCb->spId);

   RETVALUE(ROK);

} /* qoSgwEnaUSap */

/*
 *
 *      Fun:   qoSgwUbndEgUSap
 *
 *      Desc:  Unbind and disable the lower SAP 
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwUbndEgUSap
(
Void 
)
#else
PRIVATE S16 qoSgwUbndEgUSap(Void)
#endif
{
   QoSgwEgtSapCb *lSapCb; 
   S16 ret = ROK;

   TRC2(qoSgwUbndEgUSap);

   lSapCb = qoSgwCb.egUSap;


   if (lSapCb->state == LQO_SAP_UBND ||
       lSapCb->state == LQO_SAP_CFG)
   {
     QODBGP(DBGMASK_MI, (qoSgwCb.init.prntBuf,
                "qoSgwUbndEgUSap: SAP already unbound \n"));
      RETVALUE(ROK);
   }
   EuLiEgtUbndReq(&(qoSgwCb.egUSap->pst), qoSgwCb.egUSap->spId, 0);
   lSapCb->state = LQO_SAP_UBND;

   RETVALUE(ret);

} /* qoSgwUbndEgUSap */

/*
 *
 *      Fun:   qoSgwDelEgUSap
 *
 *      Desc:  Delete the lower SAP 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  qo_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 qoSgwDelEgUSap
(
)
#else
PRIVATE S16 qoSgwDelEgUSap()
#endif
{

   QoSgwEgtSapCb *lSapCb = NULLP; 
   QoSgwTunnEnt  *tunEnt = NULLP;
   QO_SGW_TRC2(qoSgwDelEgUSap);

   lSapCb = qoSgwCb.egUSap;
   if(qoSgwCb.egUSap->state == LQO_SAP_ISBOUND)
   {
      qoSgwUbndEgUSap();
   }

   /* free all tunnel CBs */
   while (cmHashListGetNext (&(lSapCb->tunnCbHlCp), (PTR) NULLP,
            (PTR *) &tunEnt) == ROK)
   {
      cmHashListDelete (&lSapCb->tunnCbHlCp, (PTR)tunEnt);
      QO_SGW_FREE(tunEnt, sizeof(QoSgwTunnEnt));
   }

   while (cmHashListGetNext (&(lSapCb->tunnPendCbHlCp), (PTR) NULLP,
            (PTR *) &tunEnt) == ROK)
   {
      cmHashListDelete (&lSapCb->tunnPendCbHlCp, (PTR)tunEnt);
      QO_SGW_FREE(tunEnt, sizeof(QoSgwTunnEnt));
   }
   cmHashListDeinit(&(lSapCb->tunnCbHlCp));
   cmHashListDeinit(&(lSapCb->tunnPendCbHlCp));
   QO_SGW_FREE(qoSgwCb.egUSap, sizeof (QoSgwEgtSapCb));

   RETVALUE(ROK);

} /* qoSgwDelEgUSap */
#endif /* QO_SGW */

/**********************************************************************

         End of file:     qo_mi.c@@/main/1 - Thu Jul  8 21:22:20 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -       sp                 1.LTE-CNE:SGW Initial Release
*********************************************************************91*/
