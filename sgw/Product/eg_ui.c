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

     Name:     eGTP Protocol - upper interface

     Type:     C source file

     Desc:     C Source code for the eGTP Protocol upper interface primitives.

     File:     eg_ui.c

     Sid:      pg_ui.c@@/main/3 - Fri Feb  4 18:18:17 2011

     Prg:      kc

*********************************************************************21*/

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "cm_dns.h"        /* common transport */
#include "hit.h"           /* TUCL Layer */
/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "hw.h"
#include "sht.h"
#endif
#include "egt.h"           /* GTP Upper Interface */
#include "eg_util.h"       /* GTP Utility library */
#include "eg.h"            /* GTP Layer */
#include "eg_err.h"        /* Error codes */
#include "eg_edm.h"        /* GTP EDM */
#ifdef TIME_MES
#include <time.h>
#endif

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/

#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_dns.x"        /* common transport */
#include "hit.x"           /* TUCL Layer */
/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "egt.x"           /* GTP Upper Interface */
#include "eg_util.x"       /* GTP Utility library */
#include "leg.x"           /* GTP LM Interface */
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* GTP Layer */
#include "eg_tpt.x"        /* EGTP TPT module defines */

#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

/********************************************************************
 * L O C A L     D E F I N E S                                      *
 *******************************************************************/

/********************************************************************
 * L O C A L     E X T E R N S                                      *
 *******************************************************************/

/********************************************************************
 * L O C A L     V A R I A B L E    D E C L A R A T I O N S         *
 *******************************************************************/

/********************************************************************
 * L O C A L     F U N C T I O N    D E C L A R A T I O N S        *
 *******************************************************************/

#ifdef EGTP_C
/*******************************************
 Function to fill Evt Structure for SigRsp *
 *******************************************/
PRIVATE S16 egUiFillEvntStrSigRsp ARGS
((
EgEvnt      *egEvnt,       /* Event Structure */ 
TknU32      lclTeid,       /* Local Tunnel Identifier */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
));

#endif /* EGTP_C*/

/*************************************************************************************
 *
 *       Fun:   EgUiEgtBndReq
 *
 *       Desc:  This function binds a User to the eGTP Protocol upper interface.
 *              eGTP Protocol layer software registers this new User and allocates a
 *              Service Access Point for this bind and records the identity
 *              of the service user.
 *              It also issues a EgUiEgtBndCfm to the service user after
 *              completing a successful bind.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtBndReq
(
Pst *pst,             /* Post structure */
SuId suId,            /* service user id */
SpId spId             /* service provider id */
)
#else
PUBLIC S16 EgUiEgtBndReq(pst, suId, spId)
Pst *pst;              /* Post Structure */
SuId suId;             /* service user id */
SpId spId;             /* service provider id */
#endif
{
   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* pointer to current SAP */
/* eg004.201  New variables added for eGTP-C PSF RUG to find the valid interface version */
#ifdef EG_RUG
   Bool      found;
   S16       i;
#endif

   EG_TRC3(EgUiEgtBndReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } 
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG332, (ErrVal)0,
                (Txt *) "EgUiEgtBndReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   } 

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

   /****************
    * sanity check *
    ***************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP))
   {

      EGLOGERROR(ERRCLS_DEBUG,EEG333, ERRZERO, "EgUiEgtBndReq: invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   }

   /********************************************
    * Check if the SAP is already bound or not *
    * If so, then return bind confirm          *
    ********************************************/
   if (uSapCb->state == LEG_USAP_BND)
   {
      /*********************************
       * send bind confirm to the user *
       *********************************/
      EgUiEgtBndCfm(&uSapCb->pst, suId, CM_BND_OK);

#ifdef LEG_FILE_LOG
      EG_TRC_FILE(<< EgUiEgtBndReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

      EG_RETVALUE(ROK);
   } 
/* eg004.201  New variables added for eGTP-C PSF RUG to find the valid interface version */
#ifdef EG_RUG
   found = FALSE;
   if (uSapCb->remIntfValid == FALSE)
   {
     for(i=0; i < egCb.numIntfInfo && found == FALSE;i++)
     {
         if(egCb.egVerInfoCb[i].intfInfo.intf.intfId == EGTIF)
         {
            switch(egCb.egVerInfoCb[i].intfInfo.grpType)
            {
               case SHT_GRPTYPE_ALL:
                  if(egCb.egVerInfoCb[i].intfInfo.dstProcId == pst->srcProcId &&
                       egCb.egVerInfoCb[i].intfInfo.dstEnt.ent ==pst->srcEnt &&
                       egCb.egVerInfoCb[i].intfInfo.dstEnt.inst == pst->srcInst)
                     found = TRUE;
                  break;
               case SHT_GRPTYPE_ENT:
                  if (egCb.egVerInfoCb[i].intfInfo.dstEnt.ent == pst->srcEnt &&
                       egCb.egVerInfoCb[i].intfInfo.dstEnt.inst == pst->srcInst)
                     found = TRUE;
                  break;
          default:
        break;
            }
         }  /* end if EGTIF */
      } /* end for */
      if (found == TRUE)
      {
         uSapCb->pst.intfVer = egCb.egVerInfoCb[i - 1].intfInfo.intf.intfVer;
         uSapCb->remIntfValid = TRUE;
      }
      else
      {
         EGLOGERROR(ERRCLS_DEBUG, EEGXXX, 0,
               "EgUiEgtBndReq() failed,cannot derive egCb");
         RETVALUE(RFAILED);
      }
   }
#endif /* EG_RUG */
   /*****************************************
    * change the state and copy the SAP ids *
    *****************************************/
   uSapCb->suId = suId;
   uSapCb->pst.dstProcId = pst->srcProcId;
   uSapCb->pst.dstEnt = pst->srcEnt;
   uSapCb->pst.dstInst = pst->srcInst;
   uSapCb->state = LEG_USAP_BND;
/* eg004.201 Update stable state of USAPCB on STANDBY when USAP is binded with the service user */
#ifdef HW
   HW_RTUPD_MOD(HW_EG_USAPCB,uSapCb)
#endif

   sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
   sta.dgn.dgnVal[0].t.sapId = spId;

   /***********************************
    * Send Alarm to the layer manager *
    ***********************************/
   egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LEG_CAUSE_SAP_BND, &sta.dgn);

   /*********************************
    * send bind confirm to the user *
    *********************************/
   EgUiEgtBndCfm(&uSapCb->pst, suId, CM_BND_OK);

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtBndReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */
/* eg004.201 Dispatch runtime update for USAPCB when binded with the service user*/
#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
}

/****************************************************************************************
 *
 *       Fun:   EgUiEgtUbndReq
 *
 *       Desc:  This function unbinds a User to the eGTP Protocol upper interface.
 *              eGTP Protocol layer software unregisters this User and deallocates the
 *              Service Access Point for this bind and removes the record of the identity
 *              of this service user.
 *              It also issues a EgUiEgtUbndCfm to the service user after
 *              completing a successful bind.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtUbndReq
(
Pst      *pst,             /* Post structure */
SpId     spId,             /* service provider id */
Reason   reason            /* reason for unbinding */
)
#else
PUBLIC S16 EgUiEgtUbndReq(pst, spId, reason)
Pst      *pst;             /* Post Structure */
SpId     spId;             /* service user id */
Reason   reason;           /* reason for unbinding */
#endif
{
   EgUsta     sta;                     /* unsolicited status diagnostics structure */ 
   EgUSapCb   *uSapCb = NULLP;         /* pointer to current SAP */
#ifdef EGTP_U
   S16        retValue = ROK;
#ifdef EGTP_U_MULTI_THREADED
   U8         eventType = 0;
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */

   EG_TRC3(EgUiEgtUbndReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } 
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
          (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
       (Txt *)__FILE__, __LINE__, ERRCLS_DEBUG ,EEG334, (ErrVal)0,
       (Txt *)"EgUiEgtUbndReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   } 

   EG_DBG_INFO(0, 0, (egp,
       "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
       pst->dstProcId, pst->dstEnt, egCb.init.inst));

#else
   UNUSED(pst);
#endif /*SS_MULTIPLE_PROCS */

   UNUSED(reason);

   EGDBGP(DBGMASK_MI, (egCb.init.prntBuf,
       "EgUiEgtUbndReq: Unbind request from suId(%d), reason(%d)", spId, reason));

   cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
    (!egCb.egtSapLst) ||
    ( (uSapCb = egCb.egtSapLst[spId]) == NULLP))
   {
      EGLOGERROR(ERRCLS_DEBUG,EEG335, ERRZERO, "EgUiEgtUbndReq: invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   } 

   /*************************************************
    * check whether the SAP is in configured state, *
    * or unbind state if so, return success         *
    *************************************************/
   if (uSapCb->state == LEG_USAP_UBND || uSapCb->state == LEG_USAP_CFG)
   {
#ifdef LEG_FILE_LOG
      EG_TRC_FILE(<<EgUiEgtUbndReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

      uSapCb->state = LEG_USAP_UBND;
      EG_RETVALUE(ROK);
   } 

#ifdef EGTP_U
   if(uSapCb->cfg.flag == LEG_EGTP_U)
   {
      /**************************************************
       *  Remove the echo Cbs which are belongs to uSap *
       **************************************************/
      if ( (retValue = egMiRmvEchoCb(uSapCb, NULLP, NULLP, NULLP, EGU_USAP)) != ROK)
      {
         EGLOGERROR(ERRCLS_DEBUG,EEG336, ERRZERO, "EgUiEgtUbndReq: Failed to process the IpCb");
         sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
         sta.dgn.dgnVal[0].t.sapId = spId;

         /***********************************
          * Send Alarm to the layer manager *
          ***********************************/
         egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);
      }
#ifdef EGTP_U_MULTI_THREADED
      egCb.egUCb.pendOp = TRUE;
      uSapCb->pendOp = TRUE;
      eventType = EGTEVTUSAPUBNDREQ;

      if((retValue = egMiGetIpCb(uSapCb, NULLP, eventType, &uSapCb->nmbReqToWrkthr)) != ROK)
      {

         EGLOGERROR(ERRCLS_DEBUG,EEG337, ERRZERO, "EgUiEgtUbndReq: Failed to process the IpCb");
         sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
         sta.dgn.dgnVal[0].t.sapId = spId;

         /***********************************
          * Send Alarm to the layer manager *
          ***********************************/
         egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

         if(uSapCb->nmbReqToWrkthr == 0)
         {
            egCb.egUCb.pendOp = FALSE;
            uSapCb->notCmpltOp = FALSE;
            uSapCb->pendOp = FALSE;
         }
         else
         {
            uSapCb->notCmpltOp = TRUE;
         }
         EG_RETVALUE(RFAILED);
      }/* End of if egMiGetIpCb*/

      if(uSapCb->nmbReqToWrkthr == 0)
      {
         uSapCb->state = LEG_USAP_UBND;
         sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
         sta.dgn.dgnVal[0].t.sapId = spId;

         /***********************************************************************
          * Send Alarm to the layer manager that unbind operation is successful *
          ***********************************************************************/
         egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UBND_OK, LEG_CAUSE_SAP_UBND, &sta.dgn);
      }
      EG_RETVALUE(ROK);
#else
      if((retValue = egMiIpCbDeInit(&uSapCb->ipCbHashList, FALSE)) != ROK)
      {

         EGLOGERROR(ERRCLS_DEBUG,EEG338, ERRZERO, "EgUiEgtUbndReq: Failed to process the IpCb");
         sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
         sta.dgn.dgnVal[0].t.sapId = spId;

         /***********************************
          * Send Alarm to the layer manager *
          ***********************************/
         egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);
      }/* End of if */
#endif /* EGTP_U_MULTI_THREADED */
   }
#endif /* EGTP_U */
#ifndef EGTP_U_MULTI_THREADED
   /*************************************************
    * change the state of SAP to unbind state again *
    *************************************************/
   uSapCb->state = LEG_USAP_UBND;
/* eg004.201 Update stable state of USAPCB on STANDBY when USAP is UNBINDED with service user*/
#ifdef HW
   /*Update state to Peer*/
   HW_RTUPD_MOD(HW_EG_USAPCB, uSapCb);
#endif


   sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
   sta.dgn.dgnVal[0].t.sapId = spId;

   /***********************************************************************
    * Send Alarm to the layer manager that unbind operation is successful *
    ***********************************************************************/
   egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UBND_OK, LEG_CAUSE_SAP_UBND, &sta.dgn);

#else

   if(uSapCb->cfg.flag == LEG_EGTP_C)
   {
      /*************************************************
       * change the state of SAP to unbind state again *
       *************************************************/
      uSapCb->state = LEG_USAP_UBND;

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************************************************
       * Send Alarm to the layer manager that unbind operation is successful *
       ***********************************************************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UBND_OK, LEG_CAUSE_SAP_UBND, &sta.dgn);
   }
#endif /* EGTP_U_MULTI_THREADED */

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtUbndReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

 /* eg004.201 Dispatch runtime update for USAPCB when USAP is UNBINDED with service user*/
#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
} 

#ifdef EGTP_C
/*************************************************************************************
 *
 *       Fun:   EgUiEgtSigReq
 *
 *       Desc:  This function is used by the service user to send the
 *              signaling request message to the eGTP Protocol.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtSigReq
(
Pst         *pst,          /* Post structure */
SpId        spId,          /* service provider id */
CmTptAddr   *localAddr,    /* Local node address */
CmTptAddr   *remAddr,      /* Remote node address */
TknU32      lclTeid,       /* Local Tunnel Identifier */
TknU32      transId,       /* Transaction Identifier */
TknU8       crtFlag,       /* Create tunnel or not */
TmrCfg      t3,            /* Request timer expiry */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EgUiEgtSigReq(pst, spId, localAddr, remAddr, lclTeid, 
                         transId, crtFlag, t3, egMsg)
Pst         *pst;          /* Post structure */
SpId        spId;          /* service provider id */
CmTptAddr   *localAddr;    /* Local node address */
CmTptAddr   *remAddr;      /* Remote node address */
TknU32      lclTeid;       /* Local Tunnel Identifier */
TknU32      transId;       /* Transaction Identifier */
TknU8       crtFlag;       /* Create tunnel or not */
TmrCfg      t3;            /* Request timer expiry */
EgMsg       *egMsg;        /* structure for carrying eGTP MSG info */
#endif
{

   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */
   EgEvnt     *egEvnt = NULLP;         /* EgEvnt pointer for holding the all information
                                          required for processing of EgMsg received */ 
   S16        retValue = 0;            /* return value */
   Bool       invAddr = FALSE;         /* flag to indicate invalid Address */
   EgTptSrvCb *tSrvCb = NULLP;         /* TPT Server Control block */

   EG_TRC3(EgUiEgtSigReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst || !egMsg)
   {
      /*************************************************
       * if the message is valid, then de-allocate the *
       * memory to avoid memory leak                   *
       *************************************************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(EGT_ERR_INV_PARAM);
   }
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
                (Txt *)__FILE__, __LINE__, ERRCLS_DEBUG ,EEG339, (ErrVal)0,
                 (Txt *)"EgUiEgtSigReq: failed, cannot derive egCb");
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG340, ERRZERO,
                "EgUiEgtSigReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }
  
   /********************* 
    * Validate T3 timer *
    *********************/
   if((t3.enb == TRUE) && (t3.val == 0))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG341, ERRZERO,
                "EgUiEgtSigReq: Invalid T3 timer Value");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_PAR_VAL, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }


   /********************************************************
    * eg007.201: To enable send/recv on all port including *
    * 2123(default) the chk on echo validation is removed  *
    ********************************************************/
#ifndef EG_MULTI_IP
   /* ccpu00134942 : USAP validation removed as echo resp sent through this SAP */
#endif

   /************************
    * Validate source port *
    ************************/
   /* ccpu00134942  : dftSrv selected here for ECHO messages */
   if ((egMsg->msgHdr.msgType != EGT_GTPC_MSG_ECHO_RSP) &&
       (egMsg->msgHdr.msgType != EGT_GTPC_MSG_ECHO_REQ))
   {
      tSrvCb = uSapCb->tptSrvCb;
   }
   else
   {
#ifndef EG_MULTI_IP
      tSrvCb = egCb.tSapCb->dftlTptSrv;
#else
      tSrvCb = uSapCb->tptSrvCb;
#endif
   }

   if(tSrvCb == NULLP)
   {
       invAddr = TRUE;
   }
   else if(localAddr->type == CM_TPTADDR_IPV4)
   {
     if((localAddr->u.ipv4TptAddr.port != tSrvCb->cfg.tptAddr.u.ipv4TptAddr.port) ||
        (localAddr->u.ipv4TptAddr.address != tSrvCb->cfg.tptAddr.u.ipv4TptAddr.address))
        invAddr = TRUE;
   }
#ifdef IPV6_SUPPORTED
   else if(localAddr->type == CM_TPTADDR_IPV6)
   {
     if (cmMemcmp((U8 *)&tSrvCb->cfg.tptAddr.u.ipv6TptAddr,
                  (U8 *)&localAddr->u.ipv6TptAddr, sizeof(CmIpv6TptAddr)) != 0)
         invAddr = TRUE;
   } 
#endif

   if(invAddr == TRUE)
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG343, ERRZERO,
                "EgUiEgtSigReq: Invalid Local Address");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_PAR_VAL, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }


   /***************************************
    * Allocate memory for event structure * 
    ***************************************/
   EG_ALLOCEVNT (&egEvnt);
    
   if( egEvnt == NULLP )
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG344, ERRZERO,
                   "EgUiEgtSigReq: Memory allocation for egEvnt failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }
      
    /****************************   
     * Fill the event structure *
     ****************************/
   if ( (retValue = egUiFillEvntStrSigReq(egEvnt, localAddr, remAddr, lclTeid, 
                         transId, crtFlag, t3, egMsg)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG345, ERRZERO,
                   "EgUiEgtSigReq: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */

      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }

   /******************************
    * Print event structure      *
    ******************************/
   EG_DBG_MSG(EG_DBG_EVNT, egEvnt);

   /*********************************
   *  Get the intf Type the sap     *
   *  is supporting from the usapCb *
   *********************************/
   egEvnt->intfType = uSapCb->cfg.intfType;

   /* ccpu00134942 */
   if ((egMsg->msgHdr.msgType == EGT_GTPC_MSG_ECHO_RSP) ||
       (egMsg->msgHdr.msgType == EGT_GTPC_MSG_ECHO_REQ))
   {
#ifndef EG_MULTI_IP
      egEvnt->srvCb = egCb.tSapCb->dftlTptSrv;
#endif
      egEvnt->intfType = EGT_GTP_INT_UNKNWN;
   }
   /******************************
    * Send the Msg to CPM module *
    ******************************/
   if ( (retValue = egCpmSigHdl(uSapCb, egEvnt)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG346, ERRZERO,
                   "EgUiEgtSigReq: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */

      EG_RETVALUE(retValue);
   }

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtSigReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */
 /* eg004.201 Dispatch runtime update for USAPCB when signalling messages arrives on an USAP*/   
#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
}

/* ccpu00137922 */
/* eg014.201 - ccpu00139510 Egtpc release 10 upgrade */
/*************************************************************************************
 *
 *       Fun:   EgUiEgtTransSigReq
 *
 *       Desc:  This function is used by the service user to send the
 *              signaling request message for a transparent tunnel to the eGTP Protocol.
 *              ex: teid=0 or for messages whose tunnel context does not exist
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtTransSigReq
(
Pst         *pst,          /* Post structure */
SpId        spId,          /* service provider id */
CmTptAddr   *localAddr,    /* Local node address */
CmTptAddr   *remAddr,      /* Remote node address */
TknU32      lclTeid,       /* Local Tunnel Identifier */
TknU32      transId,       /* Transaction Identifier */
TmrCfg      t3,            /* Request timer expiry */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EgUiEgtTransSigReq(pst, spId, localAddr, remAddr, lclTeid, 
                         transId, crtFlag, t3, egMsg)
Pst         *pst;          /* Post structure */
SpId        spId;          /* service provider id */
CmTptAddr   *localAddr;    /* Local node address */
CmTptAddr   *remAddr;      /* Remote node address */
TknU32      lclTeid;       /* Local Tunnel Identifier */
TknU32      transId;       /* Transaction Identifier */
TmrCfg      t3;            /* Request timer expiry */
EgMsg       *egMsg;        /* structure for carrying eGTP MSG info */
#endif
{

   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */
   EgEvnt     *egEvnt = NULLP;         /* EgEvnt pointer for holding the all information
                                          required for processing of EgMsg received */ 
   S16        retValue = 0;            /* return value */
   Bool       invAddr = FALSE;         /* flag to indicate invalid Address */
   EgTptSrvCb *tSrvCb = NULLP;         /* TPT Server Control block */
   TknU8      crtFlag;

   EG_TRC3(EgUiEgtTransSigReq);

   cmMemset((U8 *)&crtFlag,0, sizeof(TknU8));

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst || !egMsg)
   {
      /*************************************************
       * if the message is valid, then de-allocate the *
       * memory to avoid memory leak                   *
       *************************************************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(EGT_ERR_INV_PARAM);
   }
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
                (Txt *)__FILE__, __LINE__, ERRCLS_DEBUG ,EEG339, (ErrVal)0,
                 (Txt *)"EgUiEgtTransSigReq: failed, cannot derive egCb");
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG340, ERRZERO,
                "EgUiEgtTransSigReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }
  
   /********************* 
    * Validate T3 timer *
    *********************/
   if((t3.enb == TRUE) && (t3.val == 0))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG341, ERRZERO,
                "EgUiEgtTransSigReq: Invalid T3 timer Value");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_PAR_VAL, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }


   /********************************************************
    * eg007.201: To enable send/recv on all port including *
    * 2123(default) the chk on echo validation is removed  *
    ********************************************************/
#ifndef EG_MULTI_IP
   /* ccpu00134942 : USAP validation removed as echo resp sent through this SAP */
#endif

   tSrvCb = uSapCb->tptSrvCb;

   if(tSrvCb == NULLP)
   {
       invAddr = TRUE;
   }
   else if(localAddr->type == CM_TPTADDR_IPV4)
   {
     if((localAddr->u.ipv4TptAddr.port != tSrvCb->cfg.tptAddr.u.ipv4TptAddr.port) ||
        (localAddr->u.ipv4TptAddr.address != tSrvCb->cfg.tptAddr.u.ipv4TptAddr.address))
        invAddr = TRUE;
   }
#ifdef IPV6_SUPPORTED
   else if(localAddr->type == CM_TPTADDR_IPV6)
   {
     if (cmMemcmp((U8 *)&tSrvCb->cfg.tptAddr.u.ipv6TptAddr,
                  (U8 *)&localAddr->u.ipv6TptAddr, sizeof(CmIpv6TptAddr)) != 0)
         invAddr = TRUE;
   } 
#endif

   if(invAddr == TRUE)
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG343, ERRZERO,
                "EgUiEgtTransSigReq: Invalid Local Address");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_PAR_VAL, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }


   /***************************************
    * Allocate memory for event structure * 
    ***************************************/
   EG_ALLOCEVNT (&egEvnt);
    
   if( egEvnt == NULLP )
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG344, ERRZERO,
                   "EgUiEgtTransSigReq: Memory allocation for egEvnt failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }
      
    /****************************   
     * Fill the event structure *
     ****************************/
   if ( (retValue = egUiFillEvntStrSigReq(egEvnt, localAddr, remAddr, lclTeid, 
                         transId, crtFlag , t3, egMsg)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG345, ERRZERO,
                   "EgUiEgtTransSigReq: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */

      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }
   egEvnt->srvCb = tSrvCb;
   /******************************
    * Print event structure      *
    ******************************/
   EG_DBG_MSG(EG_DBG_EVNT, egEvnt);

   /*********************************
   *  Get the intf Type the sap     *
   *  is supporting from the usapCb *
   *********************************/
   egEvnt->intfType = uSapCb->cfg.intfType;

   /*******************************************************
    * Send the Transparent Msg directly to Encoder Module *
    *******************************************************/
   retValue = egEncMsgReq(egEvnt);
   if(retValue != ROK)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
      /* result type filled by EDM */
      egSendErrInd(egEvnt, uSapCb);
      retValue = ROK;
   }

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtTransSigReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
}



/* eg014.201 - ccpu00139510 Egtpc release 10 upgrade */
/*************************************************************************************
 *
 *       Fun:   EgUiEgtTransSigRsp
 *
 *       Desc:  This function is used by the service user to send the
 *              signaling response message for a transparent tunnel to the eGTP Protocol.
 *              ex: teid=0 or for messages whose tunnel context does not exist
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtTransSigRsp
(
Pst         *pst,          /* Post structure */
SpId        spId,          /* service provider id */
CmTptAddr   *localAddr,    /* Local node address */
CmTptAddr   *remAddr,      /* Remote node address */
TknU32      lclTeid,       /* Local Tunnel Identifier */
TknU32      transId,       /* Transaction Identifier */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EgUiEgtTransSigRsp(pst, spId, localAddr, remAddr, lclTeid, 
                         transId, crtFlag, t3, egMsg)
Pst         *pst;          /* Post structure */
SpId        spId;          /* service provider id */
CmTptAddr   *localAddr;    /* Local node address */
CmTptAddr   *remAddr;      /* Remote node address */
TknU32      lclTeid;       /* Local Tunnel Identifier */
TknU32      transId;       /* Transaction Identifier */
EgMsg       *egMsg;        /* structure for carrying eGTP MSG info */
#endif
{

   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */
   EgEvnt     *egEvnt = NULLP;         /* EgEvnt pointer for holding the all information
                                          required for processing of EgMsg received */ 
   S16        retValue = 0;            /* return value */
   Bool       invAddr = FALSE;         /* flag to indicate invalid Address */
   EgTptSrvCb *tSrvCb = NULLP;         /* TPT Server Control block */

   EG_TRC3(EgUiEgtTransSigRsp);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst || !egMsg)
   {
      /*************************************************
       * if the message is valid, then de-allocate the *
       * memory to avoid memory leak                   *
       *************************************************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(EGT_ERR_INV_PARAM);
   }
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
                (Txt *)__FILE__, __LINE__, ERRCLS_DEBUG ,EEG339, (ErrVal)0,
                 (Txt *)"EgUiEgtTransSigRsp: failed, cannot derive egCb");
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG340, ERRZERO,
                "EgUiEgtTransSigRsp: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }
  

   /********************************************************
    * eg007.201: To enable send/recv on all port including *
    * 2123(default) the chk on echo validation is removed  *
    ********************************************************/
#ifndef EG_MULTI_IP
   /* ccpu00134942 : USAP validation removed as echo resp sent through this SAP */
#endif

   tSrvCb = uSapCb->tptSrvCb;

   if(tSrvCb == NULLP)
   {
       invAddr = TRUE;
   }
   else if(localAddr->type == CM_TPTADDR_IPV4)
   {
     if((localAddr->u.ipv4TptAddr.port != tSrvCb->cfg.tptAddr.u.ipv4TptAddr.port) ||
        (localAddr->u.ipv4TptAddr.address != tSrvCb->cfg.tptAddr.u.ipv4TptAddr.address))
        invAddr = TRUE;
   }
#ifdef IPV6_SUPPORTED
   else if(localAddr->type == CM_TPTADDR_IPV6)
   {
     if (cmMemcmp((U8 *)&tSrvCb->cfg.tptAddr.u.ipv6TptAddr,
                  (U8 *)&localAddr->u.ipv6TptAddr, sizeof(CmIpv6TptAddr)) != 0)
         invAddr = TRUE;
   } 
#endif

   if(invAddr == TRUE)
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG343, ERRZERO,
                "EgUiEgtTransSigRsp : Invalid Local Address");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_PAR_VAL, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }


   /***************************************
    * Allocate memory for event structure * 
    ***************************************/
   EG_ALLOCEVNT (&egEvnt);
    
   if( egEvnt == NULLP )
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG344, ERRZERO,
                   "EgUiEgtTransSigRsp : Memory allocation for egEvnt failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }

   /****************************   
    * Fill the event structure *
    ****************************/
   egEvnt->u.egMsg         = egMsg;
   egEvnt->msgType         = egMsg->msgHdr.msgType;
   egEvnt->seqNumber       = egMsg->msgHdr.seqNumber;
   egCpyCmTptAddr(&(egEvnt->localAddr), localAddr);
   egCpyCmTptAddr(&(egEvnt->remAddr), remAddr);
   egEvnt->lclTeid.pres    = lclTeid.pres; 
   egEvnt->lclTeid.val     = lclTeid.val; 
   egEvnt->transId.pres    = transId.pres;
   egEvnt->transId.val     = transId.val;
   egEvnt->srvCb = tSrvCb;
   /******************************
    * Print event structure      *
    ******************************/
   EG_DBG_MSG(EG_DBG_EVNT, egEvnt);

   /*********************************
   *  Get the intf Type the sap     *
   *  is supporting from the usapCb *
   *********************************/
   egEvnt->intfType = uSapCb->cfg.intfType;

   /*******************************************************
    * Send the Transparent Msg directly to Encoder Module *
    *******************************************************/
   retValue = egEncMsgReq(egEvnt);
   if(retValue != ROK)
   {
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
         /* result type filled by EDM */
         egSendErrInd(egEvnt, uSapCb);
         retValue = ROK;
   }

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtTransSigRsp, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
}


/*************************************************************************************
 *
 *       Fun:   EgUiEgtSigRsp
 *
 *       Desc:  This function is used by the service user to send the
 *              signaling response message to the eGTP Protocol.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtSigRsp
(
Pst         *pst,          /* Post structure */
SpId        spId,          /* service provider id */
TknU32      lclTeid,       /* Local Tunnel Identifier */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EgUiEgtSigRsp(pst, spId, lclTeid, egMsg)
Pst         *pst;          /* Post structure */
SpId        spId;          /* service provider id */
TknU32      lclTeid;       /* Local Tunnel Identifier */
EgMsg       *egMsg;        /* structure for carrying eGTP MSG info */
#endif
{
   S16        retValue = 0;            /* return value */
   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */
   EgEvnt     *egEvnt;                 /* EgEvnt pointer for holding the all information 
                                          required for processing of EgMsg received */

   EG_TRC3(EgUiEgtSigRsp)

#ifdef ERRCHK
   
   /****************
    * sanity check *
    ****************/
   if (!pst || !egMsg)
   {
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   }
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG347, (ErrVal)0,
                (Txt *) "EgUiEgtSigRsp: failed, cannot derive egCb");
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */


   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG348, ERRZERO, "EgUiEgtUbndRsp: invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      /********************
       * free the message *
       ********************/
      EG_FREE_GM_MSG(egMsg);

      EG_RETVALUE(RFAILED);
   }

   /***************************************
    * Allocate memory for event structure * 
    ***************************************/
   EG_ALLOCEVNT (&egEvnt);
    
   if( egEvnt == NULLP )
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG349, ERRZERO,
                   "EgUiEgtSigRsp: Memory allocation for egEvnt failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }
      
    /****************************
     * Fill the event structure *
     ****************************/
   if ( (retValue = egUiFillEvntStrSigRsp(egEvnt, lclTeid, egMsg)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG350, ERRZERO,
                   "EgUiEgtSigRes: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */

      EG_FREE_GM_MSG(egMsg);
      EG_RETVALUE(retValue);
   }

   /******************************
    * Print event structure      *
    ******************************/
   EG_DBG_MSG(EG_DBG_EVNT, egEvnt);

#ifdef NOT_USED
   egEvnt->intfType = 2;
#endif 
   /*********************************
   *  Get the intf Type the sap     *
   *  is supporting from the usapCb *
   *********************************/
   egEvnt->intfType = uSapCb->cfg.intfType;

   /********************************
    * Send received message to CPM *
    ********************************/
   if ( (retValue = egCpmSigHdl(uSapCb, egEvnt)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG351, ERRZERO,
                   "EgUiEgtSigRsp: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */

      EG_RETVALUE(retValue);
   }

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtSigRsp, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */
/* eg004.201 Dispatch runtime update for USAPCB when SIGNALLING RESPONSE received from service user
 * on a USAP
 */
#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
}

/*************************************************************************************
 *
 *       Fun:   EgUiEgtStaReq
 *
 *       Desc:  This function is used by the service user to send the
 *              status request message to the eGTP Protocol.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtStaReq
(
Pst        *pst,       /* Post structure */
SpId       spId,       /* service provider id */
U8         eventType,  /* Event type for which the status is required */
EgPathInfo *pathInfo     /* path information */
)
#else
PUBLIC S16 EgUiEgtStaReq(pst, spId, eventType, pathInfo)
Pst        *pst;       /* Post structure */
SpId       spId;       /* service provider id */
U8         eventType;  /* Event type for which the status is required */
EgPathInfo *pathInfo;  /* path information */
#endif
{
   S16        retValue = 0;            /* return value */
   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */

   EG_TRC3(EgUiEgtStaReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } 
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG352, (ErrVal)0,
                (Txt *) "EgUiEgtStaReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));
   
      EGLOGERROR(ERRCLS_DEBUG,EEG353, ERRZERO,
                "EgUiEgtStaReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   }

   switch(eventType)
   {
      case EGT_STATYPE_PATH:
      case EGT_STATYPE_TUNNEL:
      {
       /************************************
        * Send the received message to CPM *
        ************************************/
         if ( (retValue = egCpmStaReq(uSapCb, eventType, pathInfo)) != ROK)
         {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG354, ERRZERO,
                   "EgUiEgtStaReq: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
            EG_RETVALUE(retValue);
         }
      }
      break;
      default:
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         EGLOGERROR(ERRCLS_INT_PAR, EEG355, (ErrVal)uSapCb->cfg.intfType,
                     "EgUiEgtStaReq(): Invalid eventType ");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
 
         EG_RETVALUE(RFAILED);
      }
   }

   EG_RETVALUE(ROK);
}


/*************************************************************************************
 *
 *       Fun:   EgUiEgtLclDelReq
 *
 *       Desc:  This function is used by the service user to send the
 *              request message to the eGTP Protocol for deleting a tuunel .
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtLclDelReq
(
Pst        *pst,       /* Post structure */
SpId       spId,       /* service provider id */
U32        transId,    /* Transaction ID */
U32        teid        /* Local TEID */
)
#else
PUBLIC S16 EgUiEgtLclDelReq(pst, spId, transId, teid)
Pst        *pst;       /* Post structure */
SpId       spId;       /* service provider id */
U32        transId;    /* Transaction ID */
U32        teid;        /* Local TEID */
#endif
{
   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */
   /*************************************************
    * this will hold the error event that has to be *
    * sent if any error occurred while processing   *
    * the request message                           *
    *************************************************/
   S16        retValue = 0;            /* return value */

   EG_TRC3(EgUiEgtLclDelReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } 
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG356, (ErrVal)0,
                (Txt *) "EgUiEgtLclDelReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG357, ERRZERO,
                "EgUiEgtLclDelReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   }

   /************************************
    * Send the received message to CPM *
    ************************************/
   if ( (retValue = egCpmLclDelReq(uSapCb, transId, teid)) != ROK)
      {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG358, ERRZERO,
                 "EgUiEgtLclDelReq: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
         EG_RETVALUE(retValue);
       }
/* eg004.201 Dispatch runtime update from ACTIVE node when a tunnel Cb is deleted on the ACTIVE node
 * triggered by a service user local delete request 
 */
#ifdef HW
   HW_UPD_PEER();
#endif

   EG_RETVALUE(ROK);
}
/*************************************************************************************
 *
 *       Fun:   EgUiEgtTunnModReq
 *
 *       Desc:  This function is used by the service user to send the
 *              request message to the eGTP Protocol for modifying a tuunel .
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 EgUiEgtTunnModReq
(
Pst        *pst,       /* Post structure */
SpId       spId,       /* service provider id */
U32        transId,    /* Transaction ID */
U32        teid,       /* Local TEID */
CmTptAddr  oldDstIpAddr, 
CmTptAddr  newDstIpAddr
)
#else
PUBLIC S16 EgUiEgtTunnModReq(pst, spId, transId, teid, oldDstIpAddr, newDstIpAddr)
Pst        *pst;       /* Post structure */
SpId       spId;       /* service provider id */
U32        transId;    /* Transaction ID */
U32        teid;        /* Local TEID */
CmTptAddr  oldDstIpAddr;
CmTptAddr  newDstIpAddr;
#endif
{
   EgUsta     sta;                     /* unsolicited status diagnostics structure */
   EgUSapCb   *uSapCb = NULLP;         /* this will hold the upper SAP control block */
   /*************************************************
    * this will hold the error event that has to be *
    * sent if any error occurred while processing   *
    * the request message                           *
    *************************************************/
   S16        retValue = 0;            /* return value */

   EG_TRC3(EgUiEgtTunnModReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } 
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS

   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG356, (ErrVal)0,
                (Txt *) "EgUiEgtTunnModReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   }

  EG_DBG_INFO(0, 0, (egp,
      "--------GTP-----(proc(%d),ent(%d),inst(%d))-------\n",
      pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ****************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG,EEG357, ERRZERO,
                "EgUiEgtTunnModReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   }

   /************************************
    * Send the received message to CPM *
    ************************************/
   if ( (retValue = egCpmLclModReq(uSapCb, transId, teid, oldDstIpAddr, newDstIpAddr)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      EGLOGERROR(ERRCLS_DEBUG,EEG358, ERRZERO,
                 "EgUiEgtLclModReq: processing the message failed.\n");
#endif /* end of (ERRCLASS & ERRCLS_DEBUG) */
         EG_RETVALUE(retValue);
   }

   EG_RETVALUE(ROK);
}

/*************************************************************************************
 *
 *       Fun:   egUiFillEvntStrSigReq
 *
 *       Desc:  This function is used to fill Evt Structure for SigReq.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 egUiFillEvntStrSigReq
(
EgEvnt      *egEvnt,       /* Event Structure */ 
CmTptAddr   *localAddr,    /* Local node address */
CmTptAddr   *remAddr,      /* Remote node address */
TknU32      lclTeid,       /* Local Tunnel Identifier */
TknU32      transId,       /* Transaction Identifier */
TknU8       crtFlag,       /* Create tunnel or not */
TmrCfg      t3,            /* Request timer expiry */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 egUiFillEvntStrSigReq(egEvnt, localAddr, remAddr, lclTeid, 
                         transId, crtFlag, t3, egMsg)
EgEvnt      *egEvnt;       /* Event Structure */
CmTptAddr   *localAddr;    /* Local node address */
CmTptAddr   *remAddr;      /* Remote node address */
TknU32      lclTeid;       /* Local Tunnel Identifier */
TknU32      transId;       /* Transaction Identifier */
TknU8       crtFlag;       /* Create tunnel or not */
TmrCfg      t3;            /* Request timer expiry */
EgMsg       *egMsg;          /* structure for carrying eGTP MSG info */
#endif
{
   EG_TRC3(egUiFillEvntStrSigReq);

   /* Filling EgEvnt Structure */
   egEvnt->u.egMsg         = egMsg;
   egEvnt->msgType         = egMsg->msgHdr.msgType;
   egEvnt->seqNumber       = egMsg->msgHdr.seqNumber;
   egCpyCmTptAddr(&(egEvnt->localAddr), localAddr);
   egCpyCmTptAddr(&(egEvnt->remAddr), remAddr);
   egEvnt->lclTeid.pres    = lclTeid.pres; 
   egEvnt->lclTeid.val     = lclTeid.val; 
   egEvnt->transId.pres    = transId.pres;
   egEvnt->transId.val     = transId.val;
   egEvnt->crtFlag         = crtFlag;    
   egEvnt->t3.enb          = t3.enb;
   egEvnt->t3.val          = t3.val;
   /*eg010.201: extract outgoing Piggyback flag*/
#ifdef EG_PIGGYBACK_SUPP
   egEvnt->isitPbMsg       = egMsg->msgHdr.pMsgPres;
#endif
   EG_RETVALUE(ROK);

}

/*************************************************************************************
 *
 *       Fun:   egUiFillEvntStrSigRsp
 *
 *       Desc:  This function is used to fill Evt Structure for SigRsp.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/

#ifdef ANSI
PUBLIC S16 egUiFillEvntStrSigRsp
(
EgEvnt      *egEvnt,       /* Event Structure */ 
TknU32      lclTeid,       /* Local Tunnel Identifier */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 egUiFillEvntStrSigRsp(egEvnt, lclTeid, egMsg)
EgEvnt      *egEvnt;       /* Event Structure */
TknU32      lclTeid;       /* Local Tunnel Identifier */
EgMsg       *egMsg;          /* structure for carrying eGTP MSG info */
#endif
{
   EG_TRC3(egUiFillEvntStrSigRsp);

   /* Filling EgEvnt Structure */
   egEvnt->u.egMsg         = egMsg;
   egEvnt->msgType         = egMsg->msgHdr.msgType;
   egEvnt->seqNumber       = egMsg->msgHdr.seqNumber;
   egEvnt->lclTeid.pres    = lclTeid.pres; 
   egEvnt->lclTeid.val     = lclTeid.val; 
   /*eg010.201: extract outgoing Piggyback flag*/
#ifdef EG_PIGGYBACK_SUPP
   egEvnt->isitPbMsg       = egMsg->msgHdr.pMsgPres;
#endif
   EG_RETVALUE(ROK);
 
}

#endif /* EGTP_C */
#ifdef EGTP_U

/*************************************************************************************
 *
 *       Fun:   EgUiEgtEguDatReq
 *
 *       Desc:  This function terminates the data request from the service user
 *              and sends the same to the peer mentioned in the event.
 *              It also issues a EgUiEgtEguErrInd to the service user if there is
 *              any error in the processing.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguDatReq
(
Pst      *pst,            /* Post structure */
SpId     spId,            /* service provider id */
EgtUEvnt *egtUEvnt        /* eGTP-U event */
)
#else
PUBLIC S16 EgUiEgtEguDatReq(pst, spId, egtUEvnt)
Pst      *pst;            /* Post Structure */
SpId     spId;            /* service provider id */
EgtUEvnt *egtUEvnt;       /* eGTP-U event */
#endif
{
   EgUSapCb   *uSapCb = NULLP;         /* pointer to current SAP */
   /* take temp Past and modify that */
   Pst        tempPst;
/*  Multithreaded performance changes gaurded under SS_MT_CB flag. */
#ifdef EGTP_U_MULTI_THREADED
#ifndef SS_MT_CB
   EgtUEvnt  *egtErrEvnt=NULLP;
#endif /* SS_MT_CB */
#ifdef TIME_MES
#ifdef SS_MT_CB
   if(entryTx[pst->dstInst-1].call < MAX_MSG)
   {
      clock_gettime(0, &entryTx[pst->dstInst-1].time[entryTx[pst->dstInst-1].call++]);
   }
#else
   PRIVATE U32 call;
   if(call < MAX_MSG)
   {
      clock_gettime(0, &entryTx1[call]);
      call ++;
   }
#endif
#endif
#endif 

   EG_TRC3(EgUiEgtEguDatReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst || !egtUEvnt)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } /* end of if */
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS
   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG359, (ErrVal)0,
               (Txt *) "EgUiEgtEguDatReq: failed, cannot derive egCb");

      /********************
       * free the message *
       ********************/
      egUFreeUiEvnt(egtUEvnt);

      EG_RETVALUE(RFAILED);
   } /* end of if */

   EG_DBG_INFO(0, 0, (egp, "--------GTP-U-----(proc(%d),ent(%d),inst(%d))-------\n",
                     pst->dstProcId, pst->dstEnt, egCb.init.inst));
#endif /* SS_MULTIPLE_PROCS */

#ifdef SS_MT_CB
   EG_RDLOCK(&egCb.egUCb.threadLock)
#endif /* SS_MT_CB */
   /****************
    * sanity check *
    ***************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ( (uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      EgUsta     sta;                     /* unsolicited status diagnostics structure */

      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG, EEG360, ERRZERO, "EgUiEgtEguDatReq: Invalid spId");

#ifdef SS_MT_CB
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock)
#endif /* SS_MT_CB */
 
      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      /********************
       * free the message *
       ********************/
      egUFreeUiEvnt(egtUEvnt);

      EG_RETVALUE(RFAILED);
   } /* end of if SAP is not invalid */

#ifdef EGTP_U_MULTI_THREADED
   /* changes for Multi threaded performance */
#ifndef SS_MT_CB
   /*********************************************************************
    * If any sap Control operation is in pending then Indicate the user *
    *********************************************************************/
   if (uSapCb->pendOp == TRUE)
   {
      EgUsta     sta;                     /* unsolicited status diagnostics structure */

      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG, EEG361, ERRZERO, "EgUiEgtEguDatReq(): Sap Control operation is in progress");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, EGT_EGTPU_CAUSE_PENDING_CNTRL_OP, &sta.dgn);

      /****************************************************
       *generate Error Indication to User* 
       ****************************************************/
      EG_ALLOC(&egtErrEvnt, sizeof(EgtUEvnt));
      if (egtErrEvnt == NULLP)
      {
         EG_DBG_ERR(0, 0,
               (egp,"Failed to Allocate the memory for egtEvnt"));
         EG_RETVALUE(RFAILED);
      }
      /*********************************
       *Fill the error event structure *
       *********************************/
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, uSapCb, EGT_ERR_PENDING_CNTRL_OP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   } /* end of if SAP is not invalid */
#endif /* SS_MT_CB */
#endif /* EGTP_U_MULTI_THREADED */

   /*---- eg005.201 : corrected duplication of Tx statistics --*/ 

   /*************************************************
    * Call the CPM module for further processing *
    *************************************************/
   cmMemcpy((U8*)&tempPst, (U8*)pst, sizeof(Pst));
   egUCpmProcEguDatReq(egtUEvnt, uSapCb, &tempPst);

#ifdef SS_MT_CB
   EG_RDWR_UNLOCK(&egCb.egUCb.threadLock)
#endif /* SS_MT_CB */
#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtEguDatReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

   EG_RETVALUE(ROK);
} /* end of EgUiEgtEguDatReq */


/*************************************************************************************
 *
 *       Fun:   EgUiEgtEguTnlMgmtReq
 *
 *       Desc:  This function terminates the tunnel management request from the service user
 *              and takes corresponding action on the tunnels.  The possible actions
 *              are ADD, DELETE, MODIFY.
 *              It also issues a EgUiEgtEguErrInd to the service user if there is
 *              any error in the processing.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguLclTnlMgmtReq
(
Pst      *pst,            /* Post structure */
SpId     spId,            /* service provider id */
EgtUEvnt *egtUEvnt        /* eGTP-U event */
)
#else
PUBLIC S16 EgUiEgtEguLclTnlMgmtReq(pst, spId, egtUEvnt)
Pst      *pst;            /* Post Structure */
SpId     spId;            /* service provider id */
EgtUEvnt *egtUEvnt;       /* eGTP-U event */
#endif
{
   EgUSapCb   *uSapCb = NULLP;         /* pointer to current SAP */

   EG_TRC3(EgUiEgtEguTnlMgmtReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst || !egtUEvnt)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } /* end of if */
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS
   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG362, (ErrVal)0,
                (Txt *) "EgUiEgtEguTnlMgmtReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   } /* end of if */

   EG_DBG_INFO(0, 0, (egp, "--------GTP-U-----(proc(%d),ent(%d),inst(%d))-------\n",
                     pst->dstProcId, pst->dstEnt, egCb.init.inst));
#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ***************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ((uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      EgUsta     sta;                     /* unsolicited status diagnostics structure */

      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG, EEG363, ERRZERO, "EgUiEgtEguTnlMgmtReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      EG_FREE(egtUEvnt,sizeof(EgtUEvnt));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   } /* end of if SAP is not invalid */

   
#ifdef EGTP_U_MULTI_THREADED
   /*********************************************************************
    * If any sap Control operation is in pending then Indicate the user *
    *********************************************************************/
   if (uSapCb->pendOp == TRUE)
   {
      EgUsta     sta;                     /* unsolicited status diagnostics structure */

      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG, EEG364, ERRZERO, "EgUiEgtEguTnlMgmtReq: Sap Control operation is in progress");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
           EGT_EGTPU_CAUSE_PENDING_CNTRL_OP, &sta.dgn);

      egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
      egtUEvnt->u.tnlMgmt.cfmStatus.reason = EGT_EGTPU_CAUSE_PENDING_CNTRL_OP;
      EgUiEgtEguLclTnlMgmtCfm(&(uSapCb->pst), uSapCb->suId, egtUEvnt);
      EG_RETVALUE(RFAILED);
   } /* end of if SAP is not invalid */
#endif /* EGTP_U_MULTI_THREADED */

   /*************************************************
    * Call the CPM module for further processing *
    *************************************************/
   egUCpmProcEguTnlMgmtReq(egtUEvnt, uSapCb);

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtEguTnlMgmtReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

   EG_RETVALUE(ROK);
} /* end of EgUiEgtEguLclTnlMgmtReq */


/*************************************************************************************
 *
 *       Fun:   EgUiEgtEguStaReq
 *
 *       Desc:  This function terminates the tunnel status request from the service user
 *              and returns corresponding status of the tunnels.
 *              It issues a EgUiEgtEguStaCfm to the service user upon processing.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  eg_ui.c
 *
 *************************************************************************************/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguStaReq
(
Pst      *pst,            /* Post structure */
SpId     spId,            /* service provider id */
EgtUEvnt *egtUEvnt        /* eGTP-U event */
)
#else
PUBLIC S16 EgUiEgtEguStaReq(pst, spId, egtUEvnt)
Pst      *pst;            /* Post Structure */
SpId     spId;            /* service provider id */
EgtUEvnt *egtUEvnt;       /* eGTP-U event */
#endif
{
   EgUSapCb   *uSapCb = NULLP;         /* pointer to current SAP */

   EG_TRC3(EgUiEgtEguStaReq);

#ifdef ERRCHK
   /****************
    * sanity check *
    ****************/
   if (!pst || !egtUEvnt)
   {
      EG_RETVALUE(EGT_ERR_INV_PARAM);
   } /* end of if */
#endif /* end of ERRCHK */

#ifdef SS_MULTIPLE_PROCS
   if((SGetXxCb(pst->dstProcId, pst->dstEnt, egCb.init.inst,
      (Void **) &egCbPtr)) != ROK)
   {
      SLogError(pst->dstEnt, egCb.init.inst, pst->dstProcId,
               (Txt *) __FILE__, __LINE__, ERRCLS_DEBUG ,EEG365, (ErrVal)0,
               (Txt *) "EgUiEgtEguStaReq: failed, cannot derive egCb");
      EG_RETVALUE(RFAILED);
   } /* end of if */

   EG_DBG_INFO(0, 0, (egp, "--------GTP-U-----(proc(%d),ent(%d),inst(%d))-------\n",
                     pst->dstProcId, pst->dstEnt, egCb.init.inst));
#endif /* SS_MULTIPLE_PROCS */

   /****************
    * sanity check *
    ***************/
   if ( (spId > egCb.genCfg.nmbUSaps || spId < 0) ||
        (!egCb.egtSapLst) ||
        ((uSapCb = egCb.egtSapLst[spId]) == NULLP) ||
        (uSapCb->state != LEG_USAP_BND))
   {
      EgUsta     sta;                     /* unsolicited status diagnostics structure */

      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      EGLOGERROR(ERRCLS_DEBUG, EEG366, ERRZERO, "EgUiEgtEguStaReq: Invalid spId");

      sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
      sta.dgn.dgnVal[0].t.sapId = spId;

      EG_FREE(egtUEvnt,sizeof(EgtUEvnt));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
           LCM_CAUSE_INV_SPID, &sta.dgn);

      EG_RETVALUE(RFAILED);
   } /* end of if SAP is not invalid */

   /*************************************************
    * Call the CPM module for further processing *
    *************************************************/
   egUCpmProcEguStaReq(egtUEvnt, uSapCb);

#ifdef LEG_FILE_LOG
   EG_TRC_FILE(<< EgUiEgtEguStaReq, DBGMASK_UI)
#endif /* end of LEG_FILE_LOG */

   EG_RETVALUE(ROK);
} /* end of EgUiEgtEguStaReq */

#ifdef EGTP_U_MULTI_THREADED
/***********************************************************************************
 *
 *       Fun:   egUiProcUbndReq 
 *
 *       Desc:  
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  eg_mi.c
 *
 ***********************************************************************************/

#ifdef ANSI
PUBLIC S16 egUiProcUbndReq 
(
EgUEvnt    *eguEvnt
)
#else
PUBLIC S16 egUiProcUbndReq(eguEvnt)
EgUEvnt    *eguEvnt;
#endif
{
   S16            retValue = ROK;
   EgUsta     sta;                     /* unsolicited status diagnostics structure */ 
   /*-- eg003.201 : TRC changes from eg006.102--*/ 
   EG_TRC3(egUiProcUbndReq);

   if(++(eguEvnt->usap->nmbCfmFrmWrkthr) == (eguEvnt->usap->nmbReqToWrkthr))
   {
      if(eguEvnt->usap->notCmpltOp == FALSE)
      {
   /*************************************************
    * change the state of SAP to unbind state again *
    *************************************************/
   eguEvnt->usap->state = LEG_USAP_UBND;

         cmMemset((U8*)&sta, 0, sizeof(EgUsta));
   sta.dgn.dgnVal[0].type    = LEG_PAR_USAP;
   sta.dgn.dgnVal[0].t.sapId = eguEvnt->usap->suId; 

   /***********************************************************************
    * Send Alarm to the layer manager that unbind operation is successful *
    ***********************************************************************/
  egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UBND_OK, LEG_CAUSE_SAP_UBND, &sta.dgn);
      }
      else 
      {
         EGLOGERROR(ERRCLS_DEBUG, EEG367, ERRZERO,
             "egUiProcUbndReq() : Failed to process the unbind request");
         eguEvnt->usap->notCmpltOp = FALSE;
         eguEvnt->usap->nmbCfmFrmWrkthr = eguEvnt->usap->nmbReqToWrkthr = 0;
         eguEvnt->usap->pendOp = FALSE;
      }
     egCb.egUCb.pendOp = FALSE;
   } /* End of if */
   if(++eguEvnt->teIdCb->nmbCfmFrmWrkThr == eguEvnt->teIdCb->nmbReqToWrkThr)
   {
       eguEvnt->teIdCb->tnlPendOp = FALSE;
       EGU_FREEEVNT(eguEvnt);
   }

   EG_RETVALUE(retValue);
}/* egUTunnReordCfm() */

#endif /* EGTP_U_MULTI_THREADED */
#endif /* end of EGTP_U */


#ifdef __cplusplus
}
#endif /* end of __cplusplus */



/********************************************************************30**

         End of file:     pg_ui.c@@/main/3 - Fri Feb  4 18:18:17 2011

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      nkottapalli       1. Initial for eGTP 1.2 Release
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss               2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/2      eg002.102    rss           1.  Used local pst structure and modify it locally
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3      eg003.201    psingh        1. Merged code from eg006.102 to Fix
                                           TRACE5 macro related issue
/main/3      eg004.201   magnihotri     1. Header files added for eGTP-C PSF
                                        2. New variables added for eGTP-C PSF RUG to find the valid
                                           interface version.  
                                        3. Update stable state of USAPCB on STANDBY when USAP is binded 
                                           with the service user
                                        4. Dispatch runtime update for USAPCB when binded with the service user
                                        5. Update stable state of USAPCB on STANDBY when USAP is UNBINDED 
                                           with service user
                                        6. Dispatch runtime update for USAPCB when USAP is UNBINDED with service user
                                        7. Dispatch runtime update for USAPCB when signalling messages arrives on an USAP
                                        8. Dispatch runtime update for USAPCB when SIGNALLING RESPONSE received 
                                           from service user on a USAP
                                        9. Dispatch runtime update from ACTIVE node when a tunnel Cb is deleted 
                                           on the ACTIVE node triggered by a service user local delete request
/main/3      eg005.201    psingh        1. Corrected duplication of Tx statistics
/main/3      eg007.201    psingh        1. Updated EgUiEgtSigReq to allow sending of msg from Echo USap 
/main/3      eg010.201    asaurabh      1. Extract outgoing Piggyback flag
/main/3      eg014.201    rss           1. Egtpc release 10 upgrade
*********************************************************************91*/
