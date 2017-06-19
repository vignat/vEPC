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
  
     File:     vb_mi.c
  
     Sid:      vb_mi.c@@/main/1 - Mon Jun 28 19:35:38 2010
  
     Prg:      ap
  
*********************************************************************21*/

/************************************************************************
 
     Note: 
 
************************************************************************/


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
#include "cm_pasn.h"       /* Common pasn define              */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */
#include "lvb.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"
#include "cm_emm.h"
#include "cm_esm.h"
#include "vbsm.h"
#include "vb_err.h"

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
#include "cm_pasn.x"       /* Common pasn library             */
#include "sct.x"           /* S1AP Upper Interface */
/* vb007.101 : s1ap include file order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */
#include "egt.x"           /* EGTP Upper Interface */

#include "lvb.x"
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */
#include "vbsm.x"

U32 Last_M_Tmsi_Used; /* MANO M_TMSI */
SSTskId       vbTskId;

/* MME Global structure. */
PUBLIC VbMmeCb vbMmeCb;

/**********************************************************************/
/************* Prototypes for MI functions ****************************/
/**********************************************************************/

PRIVATE S16 vbMmeGenCfg ARGS((VbMmeGenCfg *genCfg,
                              CmStatus *status));

PRIVATE S16 vbMmeSztSapCfg ARGS((VbMmeLSapCfg *lSap,
                                 CmStatus *status));

PRIVATE S16 vbMmeEgtSapCfg ARGS((VbMmeLSapCfg *lSap,
                                 CmStatus *status));

PRIVATE S16 vbMmeEgtSapCfgVal  ARGS((VbMmeEgtSapCb **lSapCb,
                                     VbMmeLSapCfg  *lSap,  
                                     CmStatus      *status));


PRIVATE S16 vbMmeGenCntrl ARGS((VbMmeCntrl *cntrl, 
                                CmStatus   *status));

PRIVATE S16 vbMmeLSapCntrl ARGS((VbMmeCntrl *cntrl,  
                                 Elmnt  elem,
                                 CmStatus   *status));

PRIVATE S16 vbMmeValidateGenCfg ARGS((VbMmeGenCfg *gen));

PRIVATE S16 vbMmeValidateLSapCfg ARGS((VbMmeLSapCfg *lSap));

PRIVATE S16 vbMmeValidateLSapCntrl ARGS((VbMmeCntrl  *cntrl));

PRIVATE S16 vbMmeEnaLSap ARGS((Elmnt  elem, SpId spId));

PRIVATE S16 vbMmeDelLSap ARGS((Elmnt  elem));

PRIVATE S16 vbMmeShutDown ARGS((Void));

PRIVATE S16 vbMmeUbndSztSap ARGS((Void));

PRIVATE S16 vbMmeUbndEgtSap ARGS((SpId spId));



PRIVATE S16 vbSendLmCfm ARGS((Pst    *pst,
                              Header *hdr,
                              U8      type,
                              VbMngmt *cfm));

/******************************************************************************/
/*                   Management Primitives                                    */
/******************************************************************************/

/******************************************************************************/
/*                   Configuration Request Primitive                          */
/******************************************************************************/
/*
 *
 *       Fun:   VbMiLvbCfgReq - Configuration Request
 *
 *       Desc:  This function is used by the layer manager to configure
 *              the LTE CNE layer. The layer responds with a Configuration
 *              Confirm "VbMiLvbCfgCfm" to the layer manager after
 *              completing the configuration.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: Configuration must be performed in the following
 *              sequence:
 *              1) MME General configuration (STVBGEN)
 *              2) Lower sap configuration (STVBSZTSAP)
 *              2) Lower sap configuration (STVBEGTSAP)
 *              
 *       File:  vb_mi.c
 *
 */
#ifdef ANSI
PUBLIC S16 VbMiLvbCfgReq
(
Pst     *pst,
VbMngmt *cfg            /* Config structure */
)
#else
PUBLIC S16 VbMiLvbCfgReq(pst, cfg)
Pst     *pst;
VbMngmt *cfg;           /* Config structure */
#endif
{
   S16 ret = ROK;       /* Return Value */
   VbMngmt cfgCfm;      /* Config confirm */

   TRC3(VbMiLvbCfgReq);

   cmMemset ((U8 *)&cfgCfm, 0, sizeof (VbMngmt));

   VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
      "VbMiLvbCfgReq (pst, elmId (%d))\n", cfg->hdr.elmId.elmnt));

   switch (cfg->hdr.elmId.elmnt)
   {
      case STVBGEN:       /* general config */
      {
         ret = vbMmeGenCfg (&cfg->u.cfg.s.mmeGenCfg, &cfgCfm.cfm);
         break;
      }
      case STVBSZTSAP:    /* SZT sap -  config */
      {
         ret = vbMmeSztSapCfg (&cfg->u.cfg.s.mmeLSapCfg, &cfgCfm.cfm);
         break;
      }
      case STVBEGTSAP:    /* EGT sap -  config */
      {
         ret = vbMmeEgtSapCfg (&cfg->u.cfg.s.mmeLSapCfg, &cfgCfm.cfm);
         break;
      }
      default:
      {
         VBLOGERROR(ERRCLS_DEBUG, EVB004, (ErrVal) cfg->hdr.elmId.elmnt,
                    "VbMiLvbCfgReq: invalid elmnt");
         cfgCfm.cfm.status = LCM_PRIM_NOK;
         cfgCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   }/* End of switch */

   vbSendLmCfm(pst, &cfg->hdr, TCFG, &cfgCfm);

   RETVALUE(ret);

} /* End of VbMiLvbCfgReq */

/******************************************************************************/
/*                   Control Request Primitive                                */
/******************************************************************************/
/*
*
*       Fun:   VbMiLvbCntrlReq - Control Request
*
*       Desc:  This primitive is used to control the specified element.
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  vb_mi.c
*/
#ifdef ANSI
PUBLIC S16 VbMiLvbCntrlReq
(
Pst     *pst,
VbMngmt *cntrl              /* Control structure */
)
#else
PUBLIC S16 VbMiLvbCntrlReq(pst, cntrl)
Pst     *pst;
VbMngmt *cntrl;             /* Control structure */
#endif
{
   S16 ret = ROK;           /* Return value */
   VbMngmt cntrlCfm;        /* Control confirm */

   TRC3(VbMiLvbCntrlReq);

   cmMemset ((U8 *)&cntrlCfm, 0, sizeof (VbMngmt));

   VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
          "VbMiLvbCntrlReq (pst, elmId (%d))\n", cntrl->hdr.elmId.elmnt));


   /* Check if General Configuration is done */
   if (!vbMmeCb.init.cfgDone) 
   {
      VBLOGERROR(ERRCLS_DEBUG,EVB005,0,
                 "VbMiLvbCntrlReq: MME General configuration not done ");
      cntrlCfm.cfm.status = LCM_PRIM_NOK;
      cntrlCfm.cfm.reason = LCM_REASON_GENCFG_NOT_DONE;
      vbSendLmCfm (pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

      RETVALUE(ROK);
   }
   switch(cntrl->hdr.elmId.elmnt)
   {
      case STVBGEN:
      {
         /* general (applicable layer-wide) control */
         ret = vbMmeGenCntrl(&cntrl->u.cntrl.mmeCntrl, &cntrlCfm.cfm);
         cmMemcpy((U8 *)&cntrlCfm.u.cntrl, (U8 *)&cntrl->u.cntrl,\
               sizeof(VbCntrl));
         break;
      }
      case STVBSZTSAP:
      {
         /* MME 's lower SAP with S1AP for eNodeB interface */
         ret = vbMmeLSapCntrl(&cntrl->u.cntrl.mmeCntrl, STVBSZTSAP,
                              &cntrlCfm.cfm);
         break;
      }
      case STVBEGTSAP:
      {
         /* MME 's lower SAP with EGTP for S-GW interface */
         ret = vbMmeLSapCntrl(&cntrl->u.cntrl.mmeCntrl, STVBEGTSAP,
                              &cntrlCfm.cfm);
         break;
      }
      default:
      {
         /* Invalid element */
         VBLOGERROR(ERRCLS_DEBUG, EVB006, (ErrVal) cntrl->hdr.elmId.elmnt,
                    "VbMiLvbCntrlReq: Invalid Element in control request");
         cntrlCfm.cfm.status = LCM_PRIM_NOK;
         cntrlCfm.cfm.reason = LCM_REASON_INVALID_ELMNT;
         break;
      }
   } /* end switch */

   vbSendLmCfm(pst, &cntrl->hdr, TCNTRL, &cntrlCfm);

   /* Valgrind fixes */
   SPutSBuf(pst->region, pst->pool, (Data *)cntrl, sizeof(VbMngmt));

   RETVALUE(ret);

} /* VbMiLvbCntrlReq */

/******************************************************************************/
/*                   Statistics Request Primitive                             */
/******************************************************************************/
/*
*
*       Fun:   VbMiLvbStsReq - Statistics Request
*
*       Desc:  This primitive is used by the Layer Manager to solicit 
*              statistics information. The statistics are returned
*              in the  VbMiLvbStsCfm primitive.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  vb_mi.c
*
*/

#ifdef ANSI
PUBLIC S16 VbMiLvbStsReq
(
Pst     *pst,
Action   action,
VbMngmt *sts                 /* Statistics structure */
)
#else
PUBLIC S16 VbMiLvbStsReq(pst, action, sts)
Pst     *pst;
Action   action;
VbMngmt *sts;                /* Statistics structure */
#endif
{
   S16 ret = ROK;            /* Return value */

   RETVALUE(ret);

} /* VbMiLvbStsReq */


/******************************************************************************/
/*                   Status  Request Primitive                                */
/******************************************************************************/
/*
*
*       Fun:   Status Request
*
*       Desc:  This primitive is used by the Layer Manager to solicit
*              status information. The information is returned via the
*              VbMiLvbStaCfm primitive.
*
*       Ret:   ROK     - ok
*              RFAILED - failure
*
*       Notes: None
*
*       File:  vb_mi.c
*
*/
#ifdef ANSI
PUBLIC S16 VbMiLvbStaReq
(
Pst     *pst,
VbMngmt *sta                 /* Status structure */
)
#else
PUBLIC S16 VbMiLvbStaReq(pst, sta)
Pst     *pst;
VbMngmt *sta;                /* Status structure */
#endif
{
   S16 ret = ROK;            /* Return value */

   TRC3(VbMiLvbStaReq)

   RETVALUE(ret);

} /* VbMiLvbStaReq */


/******************************************************************************/
/*                  Indication Generation Functions                           */
/******************************************************************************/
/*
 *
 *      Fun:   vbSendLmAlarm
 *
 *      Desc:  This function is used to send an Alarm / Unsolicited Status
 *             Indication to the Layer Manager
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PUBLIC Void vbSendLmAlarm
(
U16 category,           /* category */
U16 event,              /* event generated */
U16 cause              /* cause */
)
#else
PUBLIC Void vbSendLmAlarm(category, event, cause)
U16 category;           /* category */
U16 event;              /* event generated */
U16 cause;              /* cause */
#endif
{
   VbMngmt usta;
   Pst     pst;

   TRC2(vbSendLmAlarm);

   /* initialize the management structure */
   (Void)cmMemset((U8 *) &usta, '\0', (PTR)sizeof(usta));

   usta.hdr.elmId.elmnt = STVBGEN;
   /* Check if general configuration is done */
   if (vbMmeCb.init.cfgDone == FALSE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      VBLOGERROR(ERRCLS_DEBUG, EVB007, (ErrVal)0,
            "Status Indication requested; general configuration not done!");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVOID;
   }

   if (vbMmeCb.init.usta == FALSE)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      VBLOGERROR(ERRCLS_DEBUG, EVB008, (ErrVal)0,
            "usta is not initialized!");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVOID;
   }
   usta.hdr.entId.ent   = vbMmeCb.init.ent;
   usta.hdr.entId.inst  = vbMmeCb.init.inst;
   /* copy the pst structure */
   cmMemcpy((U8 *)&pst, (U8 *)&(vbMmeCb.init.lmPst), sizeof(Pst));

   usta.u.usta.alarm.category = category;
   usta.u.usta.alarm.event = event;
   usta.u.usta.alarm.cause = cause;

   /* update the date and time */
   (Void) SGetDateTime(&usta.u.usta.alarm.dt);

   VbMiLvbStaInd(&pst, &usta);

   RETVOID;

} /* end of vbSendLmAlarm */



/******************************************************************************/
/*                   Management Interface Support Functions                   */
/******************************************************************************/

/******************************************************************************/
/*                   Layer Manager Confirm  Function                          */
/******************************************************************************/
/*
 *
 *      Fun:   vbSendLmCfm
 *
 *      Desc:  Generate the Confirm to the Layer Manager
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbSendLmCfm
(
Pst     *pst,                           
Header  *hdr,         /* Pointer to the Header structure */
U8       type,        /* Confirmation Type */
VbMngmt *cfm         /* Management Structure */
)
#else
PRIVATE S16 vbSendLmCfm(pst, hdr, type, cfm)
Pst     *pst;
Header  *hdr;         /* Pointer to the Header structure */
U8       type;       /* Confirmation Type */
VbMngmt *cfm;        /* Management Structure */
#endif
{
   Pst cfmPst;        /* Confirmation Post Structure */

   TRC2(vbSendLmCfm);

   /* Fill up the header in the confirm structure */
   cmMemset( (U8 *)&cfm->hdr, '\0', sizeof(cfm->hdr));       
   cfm->hdr.elmId.elmnt = hdr->elmId.elmnt;
   cfm->hdr.transId = hdr->transId;

   cfm->hdr.entId.ent = vbMmeCb.init.ent;
   cfm->hdr.entId.inst = vbMmeCb.init.inst;
   cfmPst.srcEnt  = vbMmeCb.init.ent;
   cfmPst.srcInst = vbMmeCb.init.inst;

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
         VbMiLvbCfgCfm (&cfmPst, cfm);
        
         VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
               "vbSendLmCfm: sent VbMiLvbCfgCfm (cfmType: %d) \n", type));
         break;
      }
      case TCNTRL:
      { 
         /* Send Control Confirm */
         VbMiLvbCntrlCfm (&cfmPst, cfm);

         VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
               "vbSendLmCfm: sent VbMiLvbCntrlCfm (cfmType: %d) \n", type));
         break;
      }
      case TSTS:
      {
         /* Send Statistics Confirm */
         VbMiLvbStsCfm (&cfmPst, cfm);

         VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
               "vbSendLmCfm: sent VbMiLvbStsCfm (cfmType: %d) \n", type));
         break;
      }
      case TSSTA:
      {
         /* Send Status Confirm */
         VbMiLvbStaCfm (&cfmPst, cfm);

         VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
               "vbSendLmCfm: sent VbMiLvbStaCfm (cfmType: %d) \n", type));
         break;
      }
      default:
#if (ERRCLASS & ERRCLS_DEBUG)
         VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf, "vbSendLmCfm:Invalid Type"));
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
         break;
   }

   RETVALUE(ROK); 

} /* vbSendLmCfm */


/******************************************************************************/
/*                   Configuration Request Support Functions                  */
/******************************************************************************/

/******************************************************************************/
/*                   MME General Configuration Support Functions              */
/******************************************************************************/
/*
 *
 *      Fun:   vbMmeGenCfg
 *
 *      Desc:  Stores the general LTE CNE configuration. It calculates the 
 *             memory requirement and reserves it. It also initializes the
 *             control blocks, the hash lists and registers the timers. In
 *             case of error it rolls back and reports error in the confirm.
 *             If the general configuration is already done it is treated as
 *             reconfiguration. Not all parameters are reconfigurable. 
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeGenCfg
(
VbMmeGenCfg *genCfg,      /* General configuration */
CmStatus    *status       /* Return status */
)
#else
PRIVATE S16 vbMmeGenCfg(genCfg, status)
VbMmeGenCfg *genCfg;    /* General configuration */
CmStatus    *status;    /* Return status */
#endif
{
   VbMmeEnbCb    *enbCb = NULLP;
   S16  ret;
   U8   enbIdx;

   VB_MME_TRC2(vbMmeGenCfg);

   /****************************************************
    * Validate the general configuration elements      *
    ****************************************************/
   ret = vbMmeValidateGenCfg(genCfg);   
   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   if (vbMmeCb.init.cfgDone == TRUE) /* Reconfiguration */
   {
      (Void) cmMemcpy((U8 *)&vbMmeCb.init.lmPst, (U8 *) &genCfg->lmPst,
                      (PTR)sizeof(Pst));
      vbMmeCb.init.lmPst.srcProcId = vbMmeCb.init.procId;
      vbMmeCb.init.lmPst.srcEnt    = vbMmeCb.init.ent;
      vbMmeCb.init.lmPst.srcInst   = vbMmeCb.init.inst;
      vbMmeCb.init.lmPst.event     = EVTNONE;

      RETVALUE(ROK);
   } /* end of reconfiguration */

   cmMemset((U8 *)&vbMmeCb.mmeCfg,0,sizeof(VbMmeGenCfg));
   cmMemcpy ((U8 *)&vbMmeCb.mmeCfg, (U8 *)genCfg, sizeof (VbMmeGenCfg));

   /**************************
    * Register the timers    *
    **************************/
   ret = vbMmeRegInitTmr();

   if (ret != ROK)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_REGTMR_FAIL;
      RETVALUE (ret);
   }
   
   /***********************************
    * Initialize the elements of MME  *
    ***********************************/ 
  
   (Void) cmMemcpy((U8 *)&vbMmeCb.init.lmPst, (U8 *) &genCfg->lmPst,
                   (PTR)sizeof(Pst));

   vbMmeCb.init.lmPst.srcProcId = vbMmeCb.init.procId;
   vbMmeCb.init.lmPst.srcEnt    = vbMmeCb.init.ent;
   vbMmeCb.init.lmPst.srcInst   = vbMmeCb.init.inst;
   vbMmeCb.init.lmPst.event     = EVTNONE;

   vbMmeCb.gutiCntr = Last_M_Tmsi_Used; /* MANO M_TMSI */
   vbMmeCb.transIdCntr = 0;

   /* Initialze peer eNodeBs now */
   ret = vbMmeUtlInitENodeBs(VB_MAX_ENODEB_BIN, 0, FALSE, CM_HASH_KEYTYPE_ANY);
   if(ret != ROK)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_HASHING_FAILED;
      VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
               "vbMmeGenCfg: Hash List Initialization failed\n"));
      RETVALUE(ret);
   }

   /* Loop through all the peer eNodeBs which needs to be configured */
   for (enbIdx = 0; enbIdx < (genCfg->maxNmbEnbs);enbIdx++)
   {
      /* Allocate Control block one by one */
      enbCb = NULLP;
      
      ret = vbMmeUtlAddEnbCb(genCfg->eNbCfg[enbIdx].peerId, &enbCb);
      if(ROK != ret)
      {
         cmHashListDeinit(&(vbMmeCb.eNbLst));

         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_MEM_NOAVAIL;
         RETVALUE(RFAILED);
      }
   }

   /* Initialization for MME Application */
   ret = vbMmeAppInit();
   if(ret != ROK)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
               "vbMmeGenCfg: MME Application initialization failed\n"));
      RETVALUE(ret);
   }
   
   vbMmeCb.init.cfgDone = TRUE;

   RETVALUE(ROK);

} /* vbMmeGenCfg */

/*
 *
 *      Fun:   vbMmeValidateGenCfg
 *
 *      Desc:  Validates the LTE CNE general configuration. 
 *
 *      Ret:   LCM_REASON_NOT_APPL - ok
 *             LCM_REASON_INVALID_PAR_VAL - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeValidateGenCfg
(
VbMmeGenCfg *gen        /* General configuration */
)
#else
PRIVATE S16 vbMmeValidateGenCfg(gen)
VbMmeGenCfg *gen;       /* General configuration */
#endif
{
   U16 idx = 0;
   VB_MME_TRC2(vbMmeValidateGenCfg);

   if (gen == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      VBLOGERROR(ERRCLS_DEBUG, EVB009, ERRZERO,
          "vbMmeValidateGenCfg,  NULL cfg ");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if ((gen->maxNmbSztSaps <= 0))
   {
      VBLOGERROR(ERRCLS_DEBUG,EVB010, (ErrVal) gen->maxNmbSztSaps, 
                  "vbMmeValidateGenCfg: Invalid Num of SZT Saps");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gen->maxNmbEnbs <= 0)
   {
      VBLOGERROR(ERRCLS_DEBUG,EVB011, (ErrVal) gen->maxNmbEnbs, 
                  "vbMmeValidateGenCfg: Invalid Num of eNodeB's");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if (gen->maxNmbUes <= 0)
   {
      VBLOGERROR(ERRCLS_DEBUG,EVB012, (ErrVal) gen->maxNmbUes,
                  "vbMmeValidateGenCfg: Invalid Num of UEs");
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   if(gen->tmrRes <= 0)
   {
      RETVALUE(LCM_REASON_INVALID_PAR_VAL);
   }

   for(idx = 0; idx < gen->maxNmbEnbs; idx++)
   {
      if(gen->eNbCfg[idx].t3402Tmr <= 0)
      {
          VBLOGERROR(ERRCLS_DEBUG,EVB013, (ErrVal)0,
                     "vbMmeValidateGenCfg: Invalid t3402Tmr");
          RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }

      if(gen->eNbCfg[idx].t3450Tmr <= 0)
      {
          VBLOGERROR(ERRCLS_DEBUG,EVB014, (ErrVal)0,
                     "vbMmeValidateGenCfg: Invalid t3450Tmr");
          RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }

   RETVALUE(LCM_REASON_NOT_APPL);

} /* vbMmeValidateGenCfg */

/******************************************************************************/
/*                   MME's SAP(s) Configuration Support Functions                 */
/******************************************************************************/

/*
 *
 *      Fun:   vbMmeSztSapCfg
 *
 *      Desc:  Stores the LTE CNE lower SAP configuration with S1AP. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeSztSapCfg
(
VbMmeLSapCfg *lSap,     /* Lower SAP configuration */
CmStatus     *status    /* Return status */
)
#else
PRIVATE S16 vbMmeSztSapCfg(lSap, status)
VbMmeLSapCfg *lSap;     /* Lower SAP configuration */
CmStatus     *status;   /* Return status */
#endif
{
   S16      ret;
   VbMmeSztSapCb *lSapCb;
   VbMmeEnbCb *eNbCb = NULLP;
   VbMmeEnbCb *prevEnbCb = NULLP;

   VB_MME_TRC2(vbMmeSztSapCfg);

   lSapCb = NULLP;

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   /* Validate the protocol parameters */
   ret = vbMmeValidateLSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   /* Get Sap control block */
   lSapCb = vbMmeCb.sztSapCb;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

     /* If multiple regions support 
       Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = vbMmeCb.init.region;
      lSapCb->pst.pool     = vbMmeCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      lSapCb->maxBndRetry  = lSap->maxBndRetry;
      lSapCb->tBndTmr.enb  = lSap->tBndTmr.enb;
      lSapCb->tBndTmr.val  = lSap->tBndTmr.val;

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   VB_MME_ALLOC(&lSapCb, sizeof(VbMmeSztSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      VBLOGERROR(ERRCLS_DEBUG, EVB015, ERRZERO,
          "vbMmeSztSapCfg, Memory Allcation failed for SAP with S1AP");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of VbMmeSztSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

  /* If multiple regions support 
     Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = vbMmeCb.init.region;
   lSapCb->pst.pool     = vbMmeCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = vbMmeCb.init.ent;
   lSapCb->pst.srcInst   = vbMmeCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;

   cmMemcpy ((U8 *)&lSapCb->tBndTmr, (U8 *)&lSap->tBndTmr, sizeof(TmrCfg));

   lSapCb->maxBndRetry  = lSap->maxBndRetry;
   lSapCb->bndRetryCnt = 0;
   
   cmInitTimers(lSapCb->timers, LVB_MAX_SZTSAP_TMRS);

   lSapCb->state = LVB_SAP_CFG;

   /* Place lSapCb in vbMmeCb's LSap list */
   vbMmeCb.sztSapCb = lSapCb;

   /* Save the sztSapCb in eNbCb as well */
   while (cmHashListGetNext (&(vbMmeCb.eNbLst), (PTR) prevEnbCb,
            (PTR *) &eNbCb) == ROK)
   {
      eNbCb->sztSapCb = lSapCb;
      prevEnbCb = eNbCb;
   }
   RETVALUE(ROK);

} /* vbMmeSztSapCfg */

/*
 *
 *      Fun:   vbMmeEgtSapCfg
 *
 *      Desc:  Configures the lower SAP with EGTP. 
 *
 *      Ret:   
 *
 *      Notes: None
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeEgtSapCfg
(
VbMmeLSapCfg *lSap,     /* Lower SAP configuration */
CmStatus     *status    /* Return status */
)
#else
PRIVATE S16 vbMmeEgtSapCfg(lSap, status)
VbMmeLSapCfg *lSap;     /* Lower SAP configuration */
CmStatus     *status;   /* Return status */
#endif
{
   VbMmeEgtSapCb *lSapCb;
   S16      ret;

   VB_MME_TRC2(vbMmeEgtSapCfg);

   lSapCb = NULLP;

   /* Validate the protocol parameters */
   ret = vbMmeValidateLSapCfg(lSap);

   if (ret != LCM_REASON_NOT_APPL)
   {
      status->status = LCM_PRIM_NOK;
      status->reason = ret;
      RETVALUE(ret);
   }

   status->status = LCM_PRIM_OK;
   status->reason = LCM_REASON_NOT_APPL;

   switch(lSap->type)
   {
      case LVB_EGT_DFLT_ECHO_SAP:
      {
         /* Get Sap control block */
         ret = vbMmeEgtSapCfgVal(&vbMmeCb.egtEchoSapCb, lSap, status);
         break; 
      }
      case LVB_EGT_SAP:
      {
         /* Get Sap control block */
         ret = vbMmeEgtSapCfgVal(&vbMmeCb.egtSapCb, lSap, status);
         break; 
      }
      case LVB_EGT_S10_SAP:
      {
         /* SAP Configuration for s10 interface */
         ret = vbMmeEgtSapCfgVal(&vbMmeCb.egtS10SapCb, lSap, status);
         break;
      }
      case LVB_EGT_S3_SAP:
      {
         /* SAP Configuration for s3 interface */
         ret = vbMmeEgtSapCfgVal(&vbMmeCb.egtS3SapCb, lSap, status);
         break;
      }
      default:
      {
         status->status = LCM_PRIM_NOK;
         status->reason = LCM_REASON_INVALID_PAR_VAL;
         RETVALUE(LCM_REASON_INVALID_PAR_VAL);
      }
   }
   RETVALUE(ret);

} /* vbMmeEgtSapCfg */


#ifdef ANSI
PRIVATE S16 vbMmeEgtSapCfgVal
(
VbMmeEgtSapCb **lSapCbPtr,
VbMmeLSapCfg  *lSap,     /* Lower SAP configuration */
CmStatus      *status    /* Return status */
)
#else
PRIVATE S16 vbMmeEgtSapCfgVal(lSapCbPtr, lSap, status)
VbMmeEgtSapCb **lSapCbPtr;
VbMmeLSapCfg  *lSap;     /* Lower SAP configuration */
CmStatus      *status;   /* Return status */
#endif
{
   VbMmeEgtSapCb *lSapCb;
   S16      ret;
   VB_MME_TRC2(vbMmeEgtSapCfg);

   lSapCb = *lSapCbPtr;

   if (lSapCb != NULLP)
   {
      /* Reconfiguration */
      lSapCb->pst.prior    = lSap->prior;

     /* If multiple regions support 
       Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
      lSapCb->pst.region   = vbMmeCb.init.region;
      lSapCb->pst.pool     = vbMmeCb.init.pool;
#else
      lSapCb->pst.region   = lSap->mem.region;
      lSapCb->pst.pool     = lSap->mem.pool;
#endif

      lSapCb->maxBndRetry  = lSap->maxBndRetry;
      lSapCb->tBndTmr.enb  = lSap->tBndTmr.enb;
      lSapCb->tBndTmr.val  = lSap->tBndTmr.val;

      RETVALUE(ROK);
   }

   /* Allocate SAP Control Block */
   VB_MME_ALLOC(&lSapCb, sizeof(VbMmeEgtSapCb));

   if (lSapCb == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      VBLOGERROR(ERRCLS_DEBUG, EVB016, ERRZERO,
          "vbMmeEgtSapCfg, Memory Allcation failed for SAP with EGTP");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(RFAILED);
   }

   /* Initialize the elements of VbMmeSztSapCb */
   lSapCb->suId = lSap->suId;
   lSapCb->spId = lSap->spId;

   lSapCb->pst.selector = lSap->selector;

  /* If multiple regions support 
     Overwrite the region and pool with owned memory details */
#ifdef SSI_MEM_CHNG
   lSapCb->pst.region   = vbMmeCb.init.region;
   lSapCb->pst.pool     = vbMmeCb.init.pool;
#else
   lSapCb->pst.region   = lSap->mem.region;
   lSapCb->pst.pool     = lSap->mem.pool;
#endif

   lSapCb->pst.prior    = lSap->prior;
   lSapCb->pst.route    = lSap->route;
   lSapCb->pst.srcProcId = SFndProcId();
   lSapCb->pst.srcEnt    = vbMmeCb.init.ent;
   lSapCb->pst.srcInst   = vbMmeCb.init.inst;

   lSapCb->pst.dstProcId = lSap->dstProcId;
   lSapCb->pst.dstEnt    = lSap->dstEntId;
   lSapCb->pst.dstInst   = lSap->dstInstId;

   cmMemcpy ((U8 *)&lSapCb->tBndTmr, (U8 *)&lSap->tBndTmr, sizeof(TmrCfg));

   lSapCb->maxBndRetry  = lSap->maxBndRetry;
   lSapCb->bndRetryCnt = 0;
   
   cmInitTimers(lSapCb->timers, LVB_MAX_EGTSAP_TMRS);

   lSapCb->nmbConn = 10;
   lSapCb->state = LVB_SAP_CFG;

   ret = vbMmeUtlInitUeTnlCbs(lSapCb, VB_MAX_UETUNL_BIN, 0, FALSE,
                              CM_HASH_KEYTYPE_ANY);
   if(ret != ROK)
   {
      VB_MME_FREE((vbMmeCb.egtSapCb), sizeof(VbMmeEgtSapCb));
      VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
                "vbMmeEgtSapCfg: Hash List Initialization failed\n"));
      status->status = LCM_PRIM_NOK;
      status->reason = LCM_REASON_MEM_NOAVAIL;
      RETVALUE(ret);
   }

   *lSapCbPtr = lSapCb;

   RETVALUE(ROK);

} /* vbMmeEgtSapCfg */


/*
 *
 *      Fun:   vbMmeValidateLSapCfg
 *
 *      Desc:  Validates the LTE CNE lower SZT SAP configuration. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeValidateLSapCfg
(
VbMmeLSapCfg *lSap      /* Lower SAP configuration */
)
#else
PRIVATE S16 vbMmeValidateLSapCfg(lSap)
VbMmeLSapCfg *lSap;     /* Lower SAP configuration */
#endif
{
   VB_MME_TRC2(vbMmeValidateLSapCfg);

   if (lSap == NULLP)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      VBLOGERROR(ERRCLS_DEBUG, EVB017, ERRZERO,
          "vbMmeValidateLSapCfg, lSap is NULL");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
      RETVALUE(RFAILED);
   }

   if (vbMmeCb.init.cfgDone == FALSE)
   {
       RETVALUE(LCM_REASON_GENCFG_NOT_DONE);
   }

   if ((lSap->suId > vbMmeCb.mmeCfg.maxNmbEgtSaps) || 
       (lSap->spId > vbMmeCb.mmeCfg.maxNmbEgtSaps))
        
   {
      RETVALUE(LCM_REASON_EXCEED_CONF_VAL);
   }
   RETVALUE(LCM_REASON_NOT_APPL);

} /* end of vbMmeValidateLSapCfg */


/******************************************************************************/
/*                   MME General Control Support Functions                     */
/******************************************************************************/
/*
 *
 *      Fun:   vbMmeGenCntrl
 *
 *      Desc:  Process the general control request
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeGenCntrl
(
VbMmeCntrl *cntrl,         /* Control structure */
CmStatus *status        /* Return status */
)
#else
PRIVATE S16 vbMmeGenCntrl(cntrl, status)
VbMmeCntrl *cntrl;          /* Control structure */
CmStatus *status;        /* Return status */
#endif
{
   S16   ret;

   TRC2(vbMmeGenCntrl)

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
               vbMmeCb.init.dbgMask |= cntrl->u.dbg.dbgMask;
               cmEdmInit.dbgMask |= cntrl->u.dbg.dbgMask;
               break;
            }

#endif /* DEBUGP */
            case SAUSTA: 
            {
               vbMmeCb.init.usta = TRUE;
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
               vbMmeCb.init.dbgMask &= ~(cntrl->u.dbg.dbgMask);
               break;
            }
#endif /* DEBUGP */
            case SAUSTA:
            {
               vbMmeCb.init.usta = FALSE;
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
         ret = vbMmeShutDown();
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

} /* vbMmeGenCntrl */

/*
 *
 *      Fun:   vbMmeShutDown
 *
 *      Desc:  Shutdown the LTE CNE layer
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PUBLIC S16 vbMmeShutDown
(
Void
)
#else
PUBLIC S16 vbMmeShutDown()
#endif
{

   TRC2(vbMmeShutDown);
   RETVALUE(ROK);

} /* vbMmeShutDown */


/******************************************************************************/
/*                   MME's Lower SAP(s)  Control Support Functions            */
/******************************************************************************/
/*
 *
 *      Fun:   vbMmeLSapCntrl
 *
 *      Desc:  Process the lower SAP control request for MME
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeLSapCntrl
(
VbMmeCntrl *cntrl,      /* Control structure */
Elmnt       elem,       /* Element type */
CmStatus   *status      /* Return status */
)
#else
PRIVATE S16 vbMmeLSapCntrl(cntrl, elem, status)
VbMmeCntrl *cntrl;      /* Control structure */
Elmnt       elem;       /* Element type */
CmStatus   *status;     /* Return status */
#endif
{
   S16 ret;

   TRC2(vbMmeLSapCntrl);

   ret = vbMmeValidateLSapCntrl (cntrl);

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
         if ((ret = vbMmeEnaLSap(elem, cntrl->u.sap.id)) != ROK)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = LCM_REASON_INVALID_SAP;

#if (ERRCLASS & ERRCLS_ADD_RES)
            VBLOGERROR(ERRCLS_DEBUG, EVB018, ERRZERO,
                       "vbMmeLSapCntrl, Failed to bind and enable the SZT SAP");
#endif /* (ERRCLASS & ERRCLS_ADD_RES) */
            RETVALUE(ret);
         }
         break; 
      }
      case AUBND:
      {
         switch(elem)
         {
            case STVBSZTSAP:
            {
               if ((ret = vbMmeUbndSztSap ()) != ROK)
               {
                  status->status = LCM_PRIM_NOK;
                  status->reason = LCM_REASON_INVALID_SAP;
                  VBLOGERROR(ERRCLS_DEBUG, EVB019, ERRZERO,
                       "Failed to Unbind and disable the SZT SAP");
                  RETVALUE(ret);
               }
               break;
            }
            case STVBEGTSAP:
            {
               if ((ret = vbMmeUbndEgtSap (cntrl->u.sap.id)) != ROK)
               {
                  status->status = LCM_PRIM_NOK;
                  status->reason = LCM_REASON_INVALID_SAP;
                  VBLOGERROR(ERRCLS_DEBUG, EVB020, ERRZERO,
                       "Failed to Unbind and disable the SZT SAP");
                  RETVALUE(ret);
               }
               break;
            }
         }
         break;
      }
      case ADEL:
      {
         if ((ret = vbMmeDelLSap (elem)) != ROK)
         {
            status->status = LCM_PRIM_NOK;
            status->reason = ret;
            VBLOGERROR(ERRCLS_DEBUG, EVB021, ERRZERO,
                       "vbMmeLSapCntrl, Couldn't delete the SZT SAP");
            RETVALUE(ret);
         }
         break;
      }
      default:
      {
         break;
      }
   }
   
   RETVALUE(ROK);

} /* vbMmeLSapCntrl */

/*
 *
 *      Fun:   vbMmeValidateLSapCntrl
 *
 *      Desc:  Validates the LTE CNE's SCT SAP control parameters. 
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeValidateLSapCntrl
(
VbMmeCntrl  *cntrl         /* general control structure */
)
#else
PRIVATE S16 vbMmeValidateLSapCntrl(cntrl)
VbMmeCntrl  *cntrl;        /* general control structure */
#endif
{
   S16        ret = LCM_REASON_NOT_APPL; /* Return value */
   SpId       sapId;                     /* Lower SAP Identifier */
   VbMmeSztSapCb *lSapCb;                 /* SCT Sap control block */

   lSapCb = NULLP;
   sapId  = 0;

   TRC2(vbMmeValidateLSapCntrl);

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

} /* vbMmeValidateLSapCntrl */

/*
 *
 *      Fun:   vbMmeEnaLSap
 *
 *      Desc:  Enable the lower SAP 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeEnaLSap
(
Elmnt  elem,
SpId   spId
)
#else
PRIVATE S16 vbMmeEnaLSap(elem, spId)
Elmnt  elem;
SpId   spId;
#endif
{
   TRC2(vbMmeEnaLSap);

   switch(elem)
   {
      case STVBSZTSAP:
      {
         /* Verify if SapCb exists */
         if (NULLP == vbMmeCb.sztSapCb)
         {
            VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
                "vbMmeEnaLSap: SAP Cb with S1AP not configured \n"));
            RETVALUE(RFAILED);
         }
         VbLiSztBndReq(&(vbMmeCb.sztSapCb->pst), (vbMmeCb.sztSapCb->suId),
                        (vbMmeCb.sztSapCb->spId));
         break;
      }
      case STVBEGTSAP:
      {
         /* Verify if SapCb exists */
         
         if (NULLP == vbMmeCb.egtSapCb || NULLP == vbMmeCb.egtEchoSapCb)
         {
            VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
                "vbMmeEnaLSap: SAP Cb with EGTP not configured \n"));
            RETVALUE(RFAILED);
         }

         if(spId == vbMmeCb.egtSapCb->spId)
         {
            VbLiEgtBndReq(&(vbMmeCb.egtSapCb->pst), 
                          (vbMmeCb.egtSapCb->suId), 
                          (vbMmeCb.egtSapCb->spId));
         }
         else if(spId == vbMmeCb.egtEchoSapCb->spId)
         {
            VbLiEgtBndReq(&(vbMmeCb.egtEchoSapCb->pst),
                           (vbMmeCb.egtEchoSapCb->suId), 
                           (vbMmeCb.egtEchoSapCb->spId));
         }
         else if(spId == vbMmeCb.egtS10SapCb->spId)
         {
            VbLiEgtBndReq(&(vbMmeCb.egtS10SapCb->pst),
                           (vbMmeCb.egtS10SapCb->suId), 
                           (vbMmeCb.egtS10SapCb->spId));
         }
         else if(spId == vbMmeCb.egtS3SapCb->spId)
         {
            VbLiEgtBndReq(&(vbMmeCb.egtS3SapCb->pst),
                           (vbMmeCb.egtS3SapCb->suId), 
                           (vbMmeCb.egtS3SapCb->spId));
         }
         break;
      }
      default:
      {
         VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
                "vbMmeEnaLSap: Invalid SAP element \n"));
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);

} /* vbMmeEnaLSap */


/*
 *
 *      Fun:   vbMmeUbndSztSap
 *
 *      Desc:  Unbind and disable the lower SAP with EGTP
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeUbndSztSap
(
Void
)
#else
PRIVATE S16 vbMmeUbndSztSap(Void)
#endif
{
   S16 ret = ROK;
   VbMmeSztSapCb *lSapCb; 
   
   TRC2(vbMmeUbndSztSap);

   lSapCb = vbMmeCb.sztSapCb;

   if (lSapCb->state == LVB_SAP_UBND ||
       lSapCb->state == LVB_SAP_CFG)
   {
      VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
                "vbMmeUbndSztSap: SAP already unbound \n"));
      RETVALUE(ROK);
   }

   lSapCb->state = LVB_SAP_CFG;

   VbLiSztUbndReq(&lSapCb->pst, lSapCb->spId, 0);

   RETVALUE(ret);

} /* vbMmeUbndSztSap */

/*
 *
 *      Fun:   vbMmeUbndEgtSap
 *
 *      Desc:  Unbind and disable the lower SAP wtih EGTP
 *
 *      Ret:   ROK     - ok
 *             RFAILED - failure
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeUbndEgtSap
(
SpId          spId
)
#else
PRIVATE S16 vbMmeUbndEgtSap(spId)
SpId          spId;
#endif
{
   S16 ret = ROK;
   VbMmeEgtSapCb *lSapCb = NULLP; 
   
   TRC2(vbMmeUbndEgtSap);

   if(spId == (vbMmeCb.egtSapCb->spId))
   {
      lSapCb = vbMmeCb.egtSapCb;
   }
   else if (spId == (vbMmeCb.egtEchoSapCb->spId))
   {
      lSapCb = vbMmeCb.egtEchoSapCb;
   }  
   else if (spId == (vbMmeCb.egtS10SapCb->spId))
   {
      lSapCb = vbMmeCb.egtS10SapCb;
   }  

   if ((lSapCb != NULLP) && (lSapCb->state == LVB_SAP_UBND ||
       lSapCb->state == LVB_SAP_CFG))
   {
      VBDBGP(DBGMASK_MI, (vbMmeCb.init.prntBuf,
                "vbMmeUbndEgtSap: SAP already unbound \n"));
      RETVALUE(ROK);
   }

	if (lSapCb != NULLP)
   {		 
		lSapCb->state = LVB_SAP_CFG;
		VbLiEgtUbndReq(&lSapCb->pst, lSapCb->spId, 0);
	}

   RETVALUE(ret);

} /* vbMmeUbndEgtSap */


/*
 *
 *      Fun:   vbDelLSap
 *
 *      Desc:  Delete the lower SAP 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI
PRIVATE S16 vbMmeDelLSap
(
Elmnt  elem
)
#else
PRIVATE S16 vbMmeDelLSap(elem)
Elmnt  elem;
#endif
{
   VB_MME_TRC2(vbMmeDelLSap);
   switch(elem)
   {
      case STVBSZTSAP:
      {
         if(vbMmeCb.sztSapCb->state == LVB_SAP_ISBOUND)
         {
            vbMmeUbndSztSap();
         }
         VB_MME_FREE(vbMmeCb.sztSapCb, sizeof (VbMmeSztSapCb));
         break;
      }
      case STVBEGTSAP:
      {
         if(vbMmeCb.egtSapCb->state == LVB_SAP_ISBOUND)
         {
            vbMmeUbndEgtSap(vbMmeCb.egtSapCb->spId);
         }
         if(vbMmeCb.egtEchoSapCb->state == LVB_SAP_ISBOUND)
         {
            vbMmeUbndEgtSap(vbMmeCb.egtEchoSapCb->spId);
         }
         if(vbMmeCb.egtS10SapCb->state == LVB_SAP_ISBOUND)
         {
            vbMmeUbndEgtSap(vbMmeCb.egtS10SapCb->spId);
         }
         VB_MME_FREE(vbMmeCb.egtSapCb, sizeof (VbMmeEgtSapCb));
         VB_MME_FREE(vbMmeCb.egtEchoSapCb, sizeof (VbMmeEgtSapCb));
         VB_MME_FREE(vbMmeCb.egtS10SapCb, sizeof (VbMmeEgtSapCb));
         break;
      }
   }
     
   RETVALUE(ROK);

} /* vbMmeDelLSap */

/***********************************************************************
                      System Service Interface Functions
 ***********************************************************************/
/*
 *
 *      Fun:   vbMmeActvInit
 *
 *      Desc:  MME initialization function. Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  vb_mi.c
 *
*/
#ifdef ANSI 
PUBLIC S16 vbMmeActvInit
(
Ent    entity,         /* entity */
Inst   inst,           /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 vbMmeActvInit(entity, inst, region, reason)
Ent    entity;         /* entity */
Inst   inst;           /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
{
   TRC3(vbMmeActvInit);
   cmMemset ((U8 *)&vbMmeCb, 0, sizeof(VbMmeCb));
   vbMmeCb.init.procId = SFndProcId();

   vbMmeCb.init.ent = entity;
   vbMmeCb.init.inst = inst;
   vbMmeCb.init.region = region;
   vbMmeCb.init.reason = reason;
   vbMmeCb.init.cfgDone = FALSE;
   vbMmeCb.init.pool = 0;
   vbMmeCb.init.acnt = FALSE;

#ifdef VB_USTA
   vbMmeCb.init.usta = TRUE;
#else
   vbMmeCb.init.usta = FALSE;
#endif /* VB_USTA */

#ifdef DEBUGP
   vbMmeCb.init.dbgMask = 0x00;
#endif

   RETVALUE(ROK);

} /* vbMmeActvInit */


/**********************************************************************

         End of file:     vb_mi.c@@/main/1 - Mon Jun 28 19:35:38 2010

**********************************************************************/
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -          sp              1. LTE-CNE Initial Release.
/main/1      vb006.101  ragrawal        1. ccpu00118283 Initializing of IMSI GUTI 
                                             Mappings in vbMmeGenCfg
/main/1      vb007.101  akaranth        1. s1ap include file order has changed.
*********************************************************************91*/
