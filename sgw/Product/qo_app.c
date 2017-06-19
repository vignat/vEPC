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

     Name:    Trillium LTE SGW Application

     Type:    C file

     Desc:    This file contains SGW application source code

     File:    qo_app.c

     Sid:      qo_app.c@@/main/1 - Thu Jul  8 21:22:18 2010

     Prg:     mm 
*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "egt.h"           /* */
/* qo003.101 Changes */
#ifndef EG_REL_930
#include "eg_dbs11.h"
#else
#include "eg_dbutl.h"     /* eGTP Message Dictionary related macros */
#endif /* EG_REL_930 */
#include "eg_util.h"
#include "qo.h"
#include "qo_err.h"

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "egt.x"           /* Event structure file */
#include "qo.x"
#include "eg_util.x"
#include "egt_util.x"
#include "qo_egt_utl.x"

/***************************************************************/
/***************************************************************/
/* SGW Global structure. */
PUBLIC QoSgwCb qoSgwCb;

PRIVATE S16 qoSgwEgtHndlSndCSReqFailure
(
QoSgwUeCb      *pUeCb,
QoSgwTunnelCb  *pS5S8Tun
);

PRIVATE S16 qoSgwHndls5s8DefTunMgmtCfmDel ARGS((
EgtUEvnt             *mgmtCfm,
QoSgwTunnelCb        *tunnel));

PRIVATE S16 qoSgwHndls5s8DefTunMgmtCfmAdd ARGS((
EgtUEvnt             *mgmtCfm,
QoSgwTunnelCb        *tunnel));

PRIVATE S16 qoSgwHndls5s8DedTunMgmtCfmAdd ARGS((
EgtUEvnt             *mgmtCfm,
QoSgwTunnelCb        *tunnel));

PRIVATE S16 qoSgwHndls5s8DedTunMgmtCfmDel ARGS((
EgtUEvnt             *mgmtCfm,
QoSgwTunnelCb        *tunnel));

PRIVATE S16 qoSgwHndls4DefTunMgmtCfmDel ARGS((
EgtUEvnt             *mgmtCfm,
QoSgwTunnelCb        *tunnel));


PRIVATE S16 qoSgwHndls4TunMgmtCfmMod ARGS((
EgtUEvnt             *mgmtCfm, 
QoSgwTunnelCb        *tunnel));

PRIVATE S16 qoSgwInMBRspStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwInDDNAckStateHndlr ARGS((
QoSgwUeCb         *ueCb,      /* UE Control block (INOUT) */
EgMsg             *egMsg      /* gtp-c message (IN)   */
));

PRIVATE S16 qoSgwProcRABReq ARGS((
QoSgwUeCb         *ueCb,
EgMsg             *egMsg
));

PRIVATE S16 qoSgwHdls11EgtErrInd ARGS((
QoSgwTunnelCb     *s11Tun
));


PRIVATE S16 qoSgwHdls5s8EgtErrInd ARGS((
QoSgwTunnelCb     *s11Tun
));

PRIVATE S16 qoSgwOutUBRsp ARGS ((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

#if 0
PRIVATE S16 qoSgwSendGtpUErrInd ARGS((EgtUEvnt *data, QoSgwTunnelCb *dataTun,
      QoSgwEgtSapCb *sap, U8 cause));
#endif

PRIVATE S16 qoSgwSendFailureResponse ARGS((EgMsg *msg, QoSgwEgtSapCb *sap,
      U32 teid, QoSgwTunnelCb *tunnel, U8 cause, U8 respType));

PRIVATE S16 qoSgwCreateNSendDeleteSessReq ARGS((
         QoSgwTunnelCb *tunnel, U8 cause));

PRIVATE S16 qoSgwOutCSReq ARGS((
QoSgwPdnCb    *s5s8Tun,
EgMsg          *egMsg));

PRIVATE S16 qoSgwHandleCSReqForHandover ARGS((
QoSgwPdnCb     *pPdnCb,
EgMsg          *pCsReq
));

PRIVATE S16 qoSgwHandleCSReqForAttach ARGS((
QoSgwPdnCb     *pPdnCb,
EgMsg          *pCsReq
));

/* qo001.101: Handler for DL Data Notification */
PRIVATE S16 qoSgwOutDDN ARGS((EgMsg *msg, QoSgwTunnelCb *s11Tun));

PRIVATE Void qoSgwOutDSReq ARGS((QoSgwPdnCb *pdnCb, EgMsg *egMsg));

PRIVATE S16 qoSgwOutMBReq ARGS((
QoSgwPdnCb     *pPdnCb,
EgMsg          *pMbReq
));

PRIVATE Void qoSgwOutDSRsp ARGS((QoSgwPdnCb *pdnCb, EgMsg *egMsg));

PRIVATE S16 qoSgwOutCSRsp ARGS((QoSgwPdnCb *pdnCb, EgMsg *egMsg));

PRIVATE Void qoSgwOutBRCmd ARGS((QoSgwPdnCb *pdnCb, EgMsg *egMsg));

PRIVATE Void qoSgwOutDBCmd ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE Void qoSgwOutMBCmd ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwOutUBReq ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwOutCBReq ARGS((QoSgwPdnCb *s11Tun, EgMsg *msg));

PRIVATE S16 qoSgwInCSReqStateHndlr ARGS((
                                                QoSgwTunnelCb     *s11Tun,
                                                QoSgwPdnCb        **pdnCb,
                                                EgMsg             *egMsg));


PRIVATE S16 qoSgwCSRspStateHndlr ARGS((
                                                QoSgwPdnCb        *pdnCb,
                                                EgMsg             *egMsg));

PRIVATE S16 qoSgwMBReqStateHndlr ARGS((
U32               ratType,
QoSgwUeCb         *ueCb,
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg,      /* gtp-c message (IN)   */
CmTptAddr         *remAddr   /* remote address */
));

PRIVATE S16 qoSgwInDSReqStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg));

PRIVATE S16 qoSgwInCBReqStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg));

PRIVATE S16 qoSgwInCBRspStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg));

PRIVATE S16 qoSgwOutMBRsp ARGS((
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg      /* gtp-c msg */
));
PRIVATE S16 qoSgwOutMBRspToSGSN ARGS((
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwOutCBRsp ARGS((
                                          QoSgwPdnCb    *pdnCb,
                                          EgMsg          *egMsg));

PRIVATE S16 qoSgwInUBRspStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwInDSRspStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwInDBReqStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwInDBRspStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwOutDBRsp ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

PRIVATE S16 qoSgwInUBReqStateHndlr ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
));

/*********************************************************************
 *
 *       Fun:   qoSgwProcS5S8DefTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S5/S8 Default bearer
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcS5S8DefTnlMgmtCfm
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwProcS5S8DefTnlMgmtCfm(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwProcS5S8DefTnlMgmtCfm)

   switch (mgmtCfm->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
      {
         ret = qoSgwHndls5s8DefTunMgmtCfmAdd(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Add, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_DEL:
      {
         ret = qoSgwHndls5s8DefTunMgmtCfmDel(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                           "FAILED"));
         }

         break;
      }

      default:
      {
         QO_SGW_LOGERR(ERRCLS_DEBUG, 
                     ("Unknown Tunnel Management action \n"));
         QO_SGW_RETVALUE(RFAILED);
         break;
      }
   }


   QO_SGW_RETVALUE(ret);
}   

/*********************************************************************
 *
 *       Fun:   qoSgwProcessS1UTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1-U Default bearer
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcessS1UTnlMgmtCfm
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwProcessS1UTnlMgmtCfm(mgmtCfm,tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwProcessS1UTnlMgmtCfm)

   switch (mgmtCfm->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
      {
         ret = qoSgwHndls1UTunMgmtCfmAdd(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Add, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_DEL:
      {
         ret = qoSgwHndls1UTunMgmtCfmDel(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_MOD:
      {
         ret = qoSgwHndls1UTunMgmtCfmMod(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                           "FAILED"));
         }

         break;
      }

      default:
      {
         QO_SGW_LOGERR(ERRCLS_DEBUG, 
                     ("Unknown Tunnel Management action \n"));
         QO_SGW_RETVALUE(RFAILED);
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwProcS4DefTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S4 Default bearer
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcS4DefTnlMgmtCfm
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwProcS4DefTnlMgmtCfm(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwProcS4DefTnlMgmtCfm)

   switch (mgmtCfm->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
      {
         ret = qoSgwHndls4TunMgmtCfmAdd(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Add, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_DEL:
      {
         ret = qoSgwHndls4DefTunMgmtCfmDel(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_MOD:
      {
         ret = qoSgwHndls4TunMgmtCfmMod(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                     "FAILED"));
         }
      }
      break;

      default:
      {
         QO_SGW_LOGERR(ERRCLS_DEBUG, 
                     ("Unknown Tunnel Management action \n"));
         QO_SGW_RETVALUE(RFAILED);
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls4TunMgmtCfmAdd
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S4 Default bearer Adding
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwHndls4TunMgmtCfmAdd
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwHndls4TunMgmtCfmAdd(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwTunnelCb  *s4Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;
   TknU32         teidTkn = {0};

   QO_SGW_TRC2(qoSgwHndls4TunMgmtCfmAdd);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   s4Tun = tunnel->ueCb->s4Tun;

   if (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_NOK)
   {
      /* status is Request failure, send failure Response */
      /* Send Delete Session Request to PGW */
      ret = qoSgwCreateNSendDeleteSessReq(tunnel,
            EGT_GTPC_CAUSE_SYS_FAIL);

      qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.sgsnCSap, 
            s4Tun->locTeid, s4Tun,
            EGT_GTPC_CAUSE_SYS_FAIL,
            tunnel->msg->msgHdr.msgType);

      s4Tun->state = QO_SGW_TUN_IDLE;
      pdnCb->state = QO_SGW_PDN_IDLE;
      QO_SGW_DBG_ERR(("In GTP-U Tunnel Management Request Failed"));
   }
   else
   {
      if (tunnel->msg)
      {
         QO_SGW_DBG_INFO (("Sending Create Session Response to SGSN for tunnel:"\
               "%ld\n", teidTkn.val)); 

         /* Send Pending Response, update eg header */
         tunnel->msg->msgHdr.teidPres = PRSNT_NODEF;
         tunnel->msg->msgHdr.teid = s4Tun->remTeid;
         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val = s4Tun->locTeid;

         ret = QoSgwLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId,
                              teidTkn, tunnel->msg);
         if (ret == ROK)
         {
            /* Sending create session response, hence S4 is now
             * connected */
            QO_SGW_DBG_INFO(("Create Session Response Sent, SUCCESS"));
         }
         else
         {
            qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.sgsnCSap, 
                  s4Tun->locTeid, s4Tun,
                  EGT_GTPC_CAUSE_SYS_FAIL,
                  tunnel->msg->msgHdr.msgType);

            s4Tun->state = QO_SGW_TUN_IDLE;
            pdnCb->state = QO_SGW_PDN_IDLE;
            EgUtilDeAllocGmMsg(&(tunnel->msg));
            QO_SGW_DBG_ERR(("Failed to send the pending EGTP-C Response\n"));
         }

         tunnel->msg = NULLP;
      }
      tunnel->state = QO_SGW_TUN_CONNECTED;
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls1UTunMgmtCfmAdd
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1U Default bearer Adding
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwHndls1UTunMgmtCfmAdd
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwHndls1UTunMgmtCfmAdd(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   TknU32         teidTkn = {0};
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwHndls1UTunMgmtCfmAdd);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   s11Tun = tunnel->ueCb->s11Tun;

   if (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_NOK)
   {
      /* status is Request failure, send failure Response */
      /* Send Delete Session Request to PGW */
      ret = qoSgwCreateNSendDeleteSessReq(tunnel,
                                          EGT_GTPC_CAUSE_SYS_FAIL);

      qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.mmeCSap, 
                              s11Tun->locTeid, s11Tun,
                              EGT_GTPC_CAUSE_SYS_FAIL,
                              tunnel->msg->msgHdr.msgType);

      s11Tun->state = QO_SGW_TUN_IDLE;
      pdnCb->state = QO_SGW_PDN_IDLE;
      QO_SGW_DBG_ERR(("In GTP-U Tunnel Management Request Failed"));
   }
   else if (tunnel->msg)
   {
      /* Send Pending Response, update eg header */
      tunnel->msg->msgHdr.teidPres = PRSNT_DEF;
      tunnel->msg->msgHdr.teid = s11Tun->remTeid;
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = s11Tun->locTeid;

      QO_SGW_DBG_INFO(("Sending Modify Bearer Response to MME"));
      QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                     teidTkn, tunnel->msg);

      tunnel->state = QO_SGW_TUN_CONNECTED;
      s11Tun->state = QO_SGW_TUN_CONNECTED;

      pdnCb->state = QO_SGW_PDN_CONNECTED;
      tunnel->ueCb->state = QO_SGW_UE_CONNECTED;
      tunnel->msg = NULLP;
      tunnel->ueCb->bitmask = 0;
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls1UTunMgmtCfmDel
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1U Default bearer Adding
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwHndls1UTunMgmtCfmDel
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwHndls1UTunMgmtCfmDel(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   QoSgwUeCb      *ueCb = NULLP;
   S16            ret = ROK;
   TknU32         teidTkn = {0};
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;
   CmLListCp      *lnkLst = NULLP;
   CmLList        *tmp = NULLP;
   QoEgtUEvntInfo *egtEvntInfo;

   QO_SGW_TRC2(qoSgwHndls1UTunMgmtCfmDel);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   ueCb = tunnel->ueCb;
   s11Tun = ueCb->s11Tun;

   if ((tunnel->state == QO_SGW_TUN_LCLDEL) &&
       (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK))
   {
      /* qo001.101: send any pending response */
      if(tunnel->msg)
      {
         /* Send Pending Response */
         /* update eg header */
         tunnel->msg->msgHdr.teidPres = PRSNT_NODEF;
         tunnel->msg->msgHdr.teid = s11Tun->remTeid;
         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val = s11Tun->locTeid;
         QO_SGW_DBG_INFO(("Sending Modify Bearer Response to MME for tunnel "
                        "%ld\n", teidTkn.val)); 

         ret = QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), 
                              qoSgwCb.mmeCSap->spId, teidTkn, tunnel->msg);
         if (ret == ROK)
         {
            QO_SGW_DBG_INFO(("Pending ETGPC Response sent."));
            tunnel->state = QO_SGW_TUN_DISCONNECTED;
            pdnCb->state = QO_SGW_PDN_IDLE;
         }
         else
         {
            EgUtilDeAllocGmMsg(&(tunnel->msg));
            QO_SGW_DBG_ERR(("Failed to send the pending EGTP-C Response\n"));
         }

         /* Message will be freed once sent. */
         tunnel->msg = NULLP;
      }

      /* flush out buffered data */
      if((tunnel->intf == QO_IF_S1U_DEF) || (tunnel->intf == QO_IF_S1U_DED))
      {
         lnkLst = &(ueCb->pendingOnS1);
         CM_LLIST_FIRST_NODE(lnkLst, tmp);
         while(tmp)
         {
            egtEvntInfo = (QoEgtUEvntInfo *)tmp->node;
            EgUUtilDeAllocGmMsg(&(egtEvntInfo->egtUEvnt->u.egMsg));
            SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
                    (Data *)egtEvntInfo->egtUEvnt, (Size)sizeof(EgtUEvnt));
            /* Delete the buffered packet from the Linked List */
            cmLListDelFrm(&(tunnel->ueCb->pendingOnS1), tmp);
            (Void) SPutSBuf(DFLT_REGION, DFLT_POOL,
                  (U8*)egtEvntInfo, sizeof(QoEgtUEvntInfo));

            CM_LLIST_NEXT_NODE(lnkLst, tmp);
         } /* end of while loop */
      }

      qoSgwDeleteUTunnel(tunnel);
      bearer->s1UTun = NULLP;

      /* if s5/s8 too is deleted then free the bearer */
      if (bearer->s5s8UTun == NULLP)
      {
         QO_SGW_FREE(bearer, sizeof(QoSgwBearerCb));
         ueCb->bearerTable[bearerInd] = NULLP;
         ueCb->bearerCnt -= 1;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwHndls1UTunMgmtCfmMod
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1U Default bearer Modifying
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwHndls1UTunMgmtCfmMod
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwHndls1UTunMgmtCfmMod(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   TknU32         teidTkn = {0};
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;
   CmLListCp      *lnkLst = NULLP;
   CmLList        *tmp = NULLP;
   QoEgtUEvntInfo *egtEvntInfo;


   QO_SGW_TRC2(qoSgwHndls1UTunMgmtCfmMod);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   s11Tun = tunnel->ueCb->s11Tun;

   if (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_NOK)
   {
      QO_SGW_DBG_ERR(("Modifying Tunnel Teid (%ld)", tunnel->locTeid));
      QO_SGW_RETVALUE(RFAILED);
   }

   tunnel->state = QO_SGW_TUN_CONNECTED;

   if (tunnel->msg)
   {
      /* Send Pending Response, update eg header */
      tunnel->msg->msgHdr.teidPres = PRSNT_DEF;
      tunnel->msg->msgHdr.teid = s11Tun->remTeid;
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = s11Tun->locTeid;

      QO_SGW_DBG_INFO(("Sending Modify Bearer Response to MME"));
      QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                     teidTkn, tunnel->msg);

      tunnel->state = QO_SGW_TUN_CONNECTED;
      s11Tun->state = QO_SGW_TUN_CONNECTED;
      pdnCb->state = QO_SGW_PDN_CONNECTED;
      tunnel->ueCb->state = QO_SGW_UE_CONNECTED;
      tunnel->msg = NULLP;
      tunnel->ueCb->bitmask = 0;
   }

   /* Send buffered data to eNB if any */
   if((tunnel->intf == QO_IF_S1U_DEF) || (tunnel->intf == QO_IF_S1U_DED))  
   {
      lnkLst = &(tunnel->ueCb->pendingOnS1);
      CM_LLIST_FIRST_NODE(lnkLst, tmp);
      while(tmp)
      {
         egtEvntInfo = (QoEgtUEvntInfo *)tmp->node;
         ret = qoSgwOutUserData(egtEvntInfo->egtUEvnt, tunnel,
               qoSgwCb.egUSap); 
         if(ret != ROK)
         {
            EgUUtilDeAllocGmMsg(&(egtEvntInfo->egtUEvnt->u.egMsg));
            SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
                  (Data *)egtEvntInfo->egtUEvnt, (Size)sizeof(EgtUEvnt));
         }

         /* Delete the buffered packet from the Linked List */
         cmLListDelFrm(&(tunnel->ueCb->pendingOnS1), tmp);
         (Void) SPutSBuf(DFLT_REGION, DFLT_POOL,
               (U8*)egtEvntInfo, sizeof(QoEgtUEvntInfo));

         CM_LLIST_NEXT_NODE(lnkLst, tmp); 
      } /* end of while loop */
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwHndls5s8DefTunMgmtCfmAdd
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S5/S8 Default bearer Adding
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHndls5s8DefTunMgmtCfmAdd
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PRIVATE S16 qoSgwHndls5s8DefTunMgmtCfmAdd(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   TknU32         teidTkn = {0};
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwTunnelCb  *s4Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwHndls5s8DefTunMgmtCfmAdd);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   s11Tun = tunnel->ueCb->s11Tun;
   s4Tun = tunnel->ueCb->s4Tun;

   if (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_NOK)
   {
      /* status is Request failure, send failure Response */
      /* Send Delete Session Request to PGW */
      ret = qoSgwCreateNSendDeleteSessReq(tunnel,
            EGT_GTPC_CAUSE_SYS_FAIL);

      if(tunnel->ueCb->intf == QO_IF_S11_C )
      {
         qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.mmeCSap, 
               s11Tun->locTeid, s11Tun,
               EGT_GTPC_CAUSE_SYS_FAIL,
               tunnel->msg->msgHdr.msgType);

         s11Tun->state = QO_SGW_TUN_IDLE;
      }
      else if(tunnel->ueCb->intf == QO_IF_S4_C)
      {
         qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.sgsnCSap, 
               s4Tun->locTeid, s11Tun,
               EGT_GTPC_CAUSE_SYS_FAIL,
               tunnel->msg->msgHdr.msgType);

         s4Tun->state = QO_SGW_TUN_IDLE;
      }
      pdnCb->state = QO_SGW_PDN_IDLE;
      QO_SGW_DBG_ERR(("In GTP-U Tunnel Management request failed.\n"));
   }
   else
   {
      if (tunnel->msg)
      {
         QO_SGW_DBG_INFO (("Sending Create Session Response to MME/SGSN for tunnel:"\
                  "%ld\n", teidTkn.val)); 

         /* Send Pending Response, update eg header */
         tunnel->msg->msgHdr.teidPres = PRSNT_NODEF;
         if(tunnel->ueCb->intf == QO_IF_S11_C )
         {
            tunnel->msg->msgHdr.teid = s11Tun->remTeid;
            teidTkn.pres = PRSNT_NODEF;
            teidTkn.val = s11Tun->locTeid;

            ret = QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                  teidTkn, tunnel->msg);
         }
         else if(tunnel->ueCb->intf == QO_IF_S4_C)
         { 
            tunnel->msg->msgHdr.teid = s4Tun->remTeid;
            teidTkn.pres = PRSNT_NODEF;
            teidTkn.val = s4Tun->locTeid;

            ret = QoSgwLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId,
                  teidTkn, tunnel->msg);
         } 
         if (ret == ROK)
         {
            /* Sending create session response, hence S11 is now
             * connected */
            QO_SGW_DBG_INFO(("Create Session Response Sent, SUCCESS"));
         }
         else
         {
            EgUtilDeAllocGmMsg(&(tunnel->msg));
            QO_SGW_DBG_ERR(("Failed to send the pending EGTP-C Response\n"));
         }

         tunnel->msg = NULLP;
      }

      tunnel->state = QO_SGW_TUN_CONNECTED;
      if(tunnel->ueCb->intf == QO_IF_S11_C )
      {
         s11Tun->state = QO_SGW_TUN_CONNECTED;
      }
      else if(tunnel->ueCb->intf == QO_IF_S4_C)
      {
         s4Tun->state = QO_SGW_TUN_CONNECTED;
         pdnCb->state = QO_SGW_PDN_CONNECTED;
         tunnel->ueCb->state = QO_SGW_UE_CONNECTED;
         tunnel->ueCb->bitmask = 0;
      }
  }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls5s8DefTunMgmtCfmDel
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S5/S8 Default bearer Deleting
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHndls5s8DefTunMgmtCfmDel
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PRIVATE S16 qoSgwHndls5s8DefTunMgmtCfmDel(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   QoSgwBearerCb  *bearer = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   U8             bearerInd = 0;
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwHndls5s8DefTunMgmtCfmDel);

   ueCb = tunnel->ueCb;
   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = ueCb->bearerTable[bearerInd];

   if ((tunnel->state == QO_SGW_TUN_LCLDEL) &&
       (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK))
   {
      qoSgwDeleteTunnel(qoSgwCb.egUSap, &tunnel);
      bearer->s5s8UTun = NULLP;

     if(bearer->pdnCb->ueCb->intf == QO_IF_S11_C)
     {
        /* if S1-u too is deleted then free the bearer */
        if (bearer->s1UTun == NULLP)
        {
           QO_SGW_FREE(bearer, sizeof(QoSgwBearerCb));
           ueCb->bearerTable[bearerInd] = NULLP;
           ueCb->bearerCnt -= 1;
        }
      }
     else if(bearer->pdnCb->ueCb->intf == QO_IF_S4_C)
     {
        /* if S1-u too is deleted then free the bearer */
        if (bearer->s4UTun == NULLP)
        {
           QO_SGW_FREE(bearer, sizeof(QoSgwBearerCb));
           ueCb->bearerTable[bearerInd] = NULLP;
           ueCb->bearerCnt -= 1;
        }
      }
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls4DefTunMgmtCfmDel
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S4 Default bearer Deleting
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHndls4DefTunMgmtCfmDel
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PRIVATE S16 qoSgwHndls4DefTunMgmtCfmDel(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   QoSgwBearerCb  *bearer = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   U8             bearerInd = 0;
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwHndls4DefTunMgmtCfmDel);

   ueCb = tunnel->ueCb;
   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = ueCb->bearerTable[bearerInd];

   if ((tunnel->state == QO_SGW_TUN_LCLDEL) &&
       (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK))
   {
      qoSgwDeleteTunnel(qoSgwCb.egUSap, &tunnel);
      bearer->s4UTun = NULLP;

      /* if s5/s8 too is deleted then free the bearer */
      if (bearer->s5s8UTun == NULLP)
      {
         QO_SGW_FREE(bearer, sizeof(QoSgwBearerCb));
         ueCb->bearerTable[bearerInd] = NULLP;
         ueCb->bearerCnt -= 1;
      }
   }
   else
   {
      QO_SGW_DBG_ERR(("Failed to delete S4 def tunnel\n"));
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls4TunMgmtCfmMod 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S4 Default bearer modify 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHndls4TunMgmtCfmMod
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)
#else
PRIVATE S16 qoSgwHndls4TunMgmtCfmMod (mgmtCfm, tunnel)
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{

   U8             bearerInd = 0;
   QoSgwBearerCb  *bearer = NULLP;
   TknU32         teidTkn = {0};
   EgMsg          *mbRsp = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   S16            ret = RFAILED;

   QO_SGW_TRC2(qoSgwHndls4DefTunMgmtCfmMod);

   ueCb = tunnel->ueCb;
   if(mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK)
   {
      if(tunnel->pdnCb->state == QO_SGW_PDN_MODIFY)
      {
         tunnel->pdnCb->state = QO_SGW_PDN_CONNECTED;
         bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
         bearer = ueCb->bearerTable[bearerInd];
         bearer->s4UTun->remTeid = bearer->s4UTun->newRemTeid;
         bearer->s4UTun->remAddr = bearer->s4UTun->newRemAddr;
         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val = ueCb->s4Tun->locTeid;
         mbRsp = tunnel->msg;
         if(mbRsp != NULL)
         {
            QO_SGW_DBG_INFO(("sending modify bearer response to SGSN\n"));
            ret = QoSgwLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId,
                  teidTkn, mbRsp);
         }
         else
         {
            QO_SGW_DBG_ERR(("sending modify bearer request Failed: egMsg is NULL \n"));
            QO_SGW_RETVALUE(RFAILED);
         }
      }
   }
   QO_SGW_RETVALUE(ret);
}/* qoSgwHndls4TunMgmtCfmMod */

/*********************************************************************
 *
 *       Fun:   qoSgwProcessS5s8DedTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S5/s8 Dedicated tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcessS5s8DedTnlMgmtCfm
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwProcessS5s8DedTnlMgmtCfm(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwProcessS5s8DedTnlMgmtCfm)

   switch (mgmtCfm->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
      {
         ret = qoSgwHndls5s8DedTunMgmtCfmAdd(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Add, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_DEL:
      {
         ret = qoSgwHndls5s8DedTunMgmtCfmDel(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                           "FAILED"));
         }

         break;
      }

      default:
      {
         QO_SGW_LOGERR(ERRCLS_DEBUG, 
                     ("Unknown Tunnel Management action \n"));
         QO_SGW_RETVALUE(RFAILED);
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwProcessS4cTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-C for
 *             S4-C tunnel.  
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcessS4cTnlMgmtCfm
(
QoSgwTunnelCb        *tunnel,           /* Tunnel Cb to process */
CmStatus             status            /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
)
#else
PUBLIC S16 qoSgwProcessS4cTnlMgmtCfm(QoSgwTunnelCb  *tunnel, CmStatus status)
QoSgwTunnelCb        *tunnel;           /* Tunnel Cb to process */
CmStatus             status;            /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
#endif
{

   QoSgwUeCb            *ueCb = NULLP;
   S16                   ret = RFAILED;

   QO_SGW_TRC2(qoSgwProcessS4cTnlMgmtCfm)


      if(tunnel == NULLP)
      {
         QO_SGW_DBG_INFO(("TunnelCB is null \n"));
         RETVALUE(RFAILED);
      }

   ueCb = tunnel->ueCb;
   if(ueCb == NULLP)
   {
      QO_SGW_DBG_ERR(("UeCb is NULL\n"));
      RETVALUE(ret);
   }

   if(status.status == LCM_PRIM_OK)
   {
      /* If EGTP updated succesfully, update the s4Tunn(c-lane) teid , remaddr, and state */
      ueCb->s4Tun->state = QO_SGW_TUN_CONNECTED;
      ueCb->s4Tun->remTeid = ueCb->s4Tun->newRemTeid;
      ueCb->s4Tun->remAddr = ueCb->s4Tun->newRemAddr;
      ueCb->s4Tun->newRemAddr = NULLP;

      /* deleting the S11tun */
      ret = qoSgwDeleteTunnel(qoSgwCb.mmeCSap, &(ueCb->s11Tun));
      if(ret == ROK)
      {
         QO_SGW_DBG_INFO(("teid(%d) deleted from the qoSgwCb->mmeCSap \n", tunnel->locTeid));
      }
      else
      {
         QO_SGW_DBG_ERR(("deleting of tunnel from qoSgwCb->mmeCSap failed, teid(%d)\n", tunnel->locTeid));
      }
   }
   else
   {
      /*
       * TODO: if status in LCM_PRIM_NOK we have to send a failure response to SGSN
       */
      QO_SGW_DBG_ERR(("Received status as LCM_PRIM_NOK: status(%d), reason(%d) \n",
               status.status, status.reason));
      RETVALUE(ret);
   }
   RETVALUE(ret);

}

/*********************************************************************
 *
 *       Fun:   qoSgwProcessS11cTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-C for
 *             S4-C tunnel.  
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcessS11cTnlMgmtCfm
(
QoSgwTunnelCb        *tunnel,           /* Tunnel Cb to process */
CmStatus             status            /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
)
#else
PUBLIC S16 qoSgwProcessS11cTnlMgmtCfm(QoSgwTunnelCb  *tunnel, CmStatus status)
QoSgwTunnelCb        *tunnel;           /* Tunnel Cb to process */
CmStatus             status;            /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
#endif
{

   QoSgwUeCb            *ueCb = NULLP;
   S16                   ret = RFAILED;

   QO_SGW_TRC2(qoSgwProcessS11cTnlMgmtCfm)


      if(tunnel == NULLP)
      {
         QO_SGW_DBG_INFO(("TunnelCB is null \n"));
         RETVALUE(RFAILED);
      }

   ueCb = tunnel->ueCb;
   if(ueCb == NULLP)
   {
      QO_SGW_DBG_ERR(("UeCb is NULL\n"));
      RETVALUE(ret);
   }

   if(status.status == LCM_PRIM_OK)
   {
      /* If EGTP updated succesfully, update the s4Tunn(c-lane) teid , remaddr, and state */
      ueCb->s11Tun->state = QO_SGW_TUN_CONNECTED;
      ueCb->s11Tun->remTeid = ueCb->s11Tun->newRemTeid;
      ueCb->s11Tun->remAddr = ueCb->s11Tun->newRemAddr;
      ueCb->s11Tun->newRemAddr = NULLP;
      ueCb->s11Tun->newRemTeid = 0;

      /* deleting the S4tun */
      ret = qoSgwDeleteTunnel(qoSgwCb.sgsnCSap, &(ueCb->s4Tun));
      if(ret == ROK)
      {
         QO_SGW_DBG_INFO(("teid(%d) deleted from the qoSgwCb->sgsnCSap \n", tunnel->locTeid));
      }
      else
      {
         QO_SGW_DBG_ERR(("deleting of tunnel from qoSgwCb->sgsnCSap failed, teid(%d)\n", tunnel->locTeid));
      }
   }
   else
   {
      /*
       * TODO: if status in LCM_PRIM_NOK we have to send a failure response to 
       MME
       */
      QO_SGW_DBG_ERR(("Received status as LCM_PRIM_NOK: status(%d), reason(%d) \n",
               status.status, status.reason));
      RETVALUE(ret);
   }
   RETVALUE(ret);

}



/*********************************************************************
 *
 *       Fun:   qoSgwHndls5s8DedTunMgmtCfmAdd
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S5/S8 Dedicated bearer adding
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHndls5s8DedTunMgmtCfmAdd
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PRIVATE S16 qoSgwHndls5s8DedTunMgmtCfmAdd(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   TknU32         teidTkn = {0};
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwPdnCb     *pdnCb = NULLP;
   QoSgwTunnelCb  *s5s8CTun = NULLP;
   EgIe           egIe;
   EgIeLevelInfo  egLvlInfo = {0};

   QO_SGW_TRC2(qoSgwHndls5s8DedTunMgmtCfmAdd);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   s5s8CTun = pdnCb->s5s8CTun;

   if (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_NOK)
   {
      qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.mmeCSap, 
                              s5s8CTun->locTeid, pdnCb->s5s8CTun,
                              EGT_GTPC_CAUSE_SYS_FAIL,
                              tunnel->msg->msgHdr.msgType);

      bearer->s5s8UTun->state = QO_SGW_TUN_IDLE;
      QO_SGW_DBG_ERR(("In GTP-U Tunnel Management request failed.\n"));
   }
   else if (tunnel->msg)
   {
      QO_SGW_DBG_INFO (("Sending Create Bearer Response to MME for tunnel "
                        "%ld\n", teidTkn.val)); 

      /* Send Pending Response, update eg header */
      tunnel->msg->msgHdr.teidPres = PRSNT_NODEF;
      tunnel->msg->msgHdr.teid = s5s8CTun->remTeid;
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = s5s8CTun->locTeid;

      /* qo003.101
       * Add Recovery IE to the egMsg as its 
       * mandatory IE in the S5 Interface dictionary
       */
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      qoUtlEgtBuildEgIeHeader(&egIe, EG_RECOVERY_IETYPE, 0, EG_UNSIGNED_8);

      egIe.t.valUnSgnd8 = 1; /* some random value */

      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres= TRUE;
      egLvlInfo.ieInfo[0].ieType= EG_RECOVERY_IETYPE;
      egLvlInfo.ieInfo[0].ieInst= 0;

      ret = EgUtilGmAppendIe(tunnel->msg, &egLvlInfo, &egIe);
      if(ret != ROK)
      {
         QO_SGW_LOGERR(ERRCLS_ADD_RES,"Appending Recovery IE, FAILED");
         QO_SGW_RETVALUE(RFAILED);
      }

      ret = QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.pgwCSap->spId,
                           teidTkn, tunnel->msg);
      if (ret == ROK)
      {
         /* Sending create session response, hence S11 is now
          * connected */
         QO_SGW_DBG_INFO(("Create Session Response Sent, SUCCESS"));
         tunnel->state = QO_SGW_TUN_CONNECTED;
         tunnel->ueCb->s11Tun->state = QO_SGW_TUN_CONNECTED;
      }
      else
      {
         EgUtilDeAllocGmMsg(&(tunnel->msg));
         QO_SGW_DBG_ERR(("Failed to send the pending EGTP-C Response\n"));
      }

      tunnel->msg = NULLP;
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwHndls5s8DedTunMgmtCfmDel
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S5/S8 Deicated tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHndls5s8DedTunMgmtCfmDel
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PRIVATE S16 qoSgwHndls5s8DedTunMgmtCfmDel(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwUeCb      *ueCb = NULL;

   QO_SGW_TRC2(qoSgwHndls5s8DedTunMgmtCfmDel);

   ueCb = tunnel->ueCb;
   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = ueCb->bearerTable[bearerInd];

   if ((tunnel->state == QO_SGW_TUN_LCLDEL) &&
       (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK))
   {
      qoSgwDeleteUTunnel(tunnel);
      bearer->s5s8UTun = NULLP;

      /* if S1-u too is deleted then free the bearer */
      if (bearer->s1UTun == NULLP)
      {
         QO_SGW_FREE(bearer, sizeof(QoSgwBearerCb));
         ueCb->bearerTable[bearerInd] = NULLP;
         ueCb->bearerCnt -= 1;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwProcessS1DedTnlMgmtCfm 
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1 Dedicated tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwProcessS1DedTnlMgmtCfm
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwProcessS1DedTnlMgmtCfm(mgmtCfm, ue, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwProcessS1DedTnlMgmtCfm)

   switch (mgmtCfm->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
      {
         ret = qoSgwHndls1DedTunMgmtCfmAdd(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Add, "
                           "FAILED"));
         }

         break;
      }

      case EGT_TMGMT_REQTYPE_DEL:
      {
         ret = qoSgwHndls1DedTunMgmtCfmDel(mgmtCfm, tunnel);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Handling of Tunnel Mgmt Confirmation for Del, "
                           "FAILED"));
         }

         break;
      }

      default:
      {
         QO_SGW_LOGERR(ERRCLS_DEBUG, 
                     ("Unknown Tunnel Management action \n"));
         QO_SGW_RETVALUE(RFAILED);
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwHndls1DedTunMgmtCfmAdd
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1 Dedicated bearer adding
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwHndls1DedTunMgmtCfmAdd
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwHndls1DedTunMgmtCfmAdd(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   TknU32         teidTkn = {0};
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwPdnCb     *pdnCb = NULLP;
   QoSgwTunnelCb  *s5s8CTun = NULLP;
   EgIe           egIe;
   EgIeLevelInfo  egLvlInfo = {0};

   QO_SGW_TRC2(qoSgwHndls1DedTunMgmtCfmAdd);

   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = tunnel->ueCb->bearerTable[bearerInd];
   pdnCb = bearer->pdnCb;
   s5s8CTun = pdnCb->s5s8CTun;

   if (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_NOK)
   {
      qoSgwSendFailureResponse(tunnel->msg, qoSgwCb.mmeCSap, 
                              s5s8CTun->locTeid, pdnCb->s5s8CTun,
                              EGT_GTPC_CAUSE_SYS_FAIL,
                              tunnel->msg->msgHdr.msgType);

      bearer->s5s8UTun->state = QO_SGW_TUN_IDLE;
      QO_SGW_DBG_ERR(("In GTP-U Tunnel Management request failed.\n"));
   }
   else if (tunnel->msg)
   {
      QO_SGW_DBG_INFO (("Sending Create Bearer Response to MME for tunnel "
                        "%ld\n", teidTkn.val)); 

      /* Send Pending Response, update eg header */
      tunnel->msg->msgHdr.teidPres = PRSNT_NODEF;
      tunnel->msg->msgHdr.teid = s5s8CTun->remTeid;
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = s5s8CTun->locTeid;

#ifdef EG_REL_930
      /* qo003.101
       * Add Recovery IE to the egMsg as its 
       * mandatory IE in the S5 Interface dictionary
       */
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      qoUtlEgtBuildEgIeHeader(&egIe, EG_RECOVERY_IETYPE, 0, EG_UNSIGNED_8);

      egIe.t.valUnSgnd8 = 1; /* some random value */

      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres= TRUE;
      egLvlInfo.ieInfo[0].ieType= EG_RECOVERY_IETYPE;
      egLvlInfo.ieInfo[0].ieInst= 0;

      ret = EgUtilGmAppendIe(tunnel->msg, &egLvlInfo, &egIe);
      if(ret != ROK)
      {
         QO_SGW_LOGERR(ERRCLS_ADD_RES,"Appending Recovery IE, FAILED");
         QO_SGW_RETVALUE(RFAILED);
      }
#endif /* EG_REL_930 */

      ret = QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.pgwCSap->spId,
                           teidTkn, tunnel->msg);
      if (ret == ROK)
      {
         /* Sending create session response, hence S11 is now
          * connected */
         QO_SGW_DBG_INFO(("Create Bearer Response Sent, SUCCESS"));
         tunnel->state = QO_SGW_TUN_CONNECTED;
         tunnel->ueCb->s11Tun->state = QO_SGW_TUN_CONNECTED;
      }
      else
      {
         EgUtilDeAllocGmMsg(&(tunnel->msg));
         QO_SGW_DBG_ERR(("Failed to send the pending EGTP-C Response\n"));
      }

      tunnel->msg = NULLP;
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwHndls1DedTunMgmtCfmDel
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U for
 *              S1 Deicated tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwHndls1DedTunMgmtCfmDel
(
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
)   
#else
PUBLIC S16 qoSgwHndls1DedTunMgmtCfmDel(mgmtCfm, tunnel)   
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
#endif
{
   S16            ret = ROK;
   QoSgwBearerCb  *bearer = NULLP;
   U8             bearerInd = 0;
   QoSgwUeCb      *ueCb = NULL;

   QO_SGW_TRC2(qoSgwHndls1DedTunMgmtCfmDel);

   ueCb = tunnel->ueCb;
   bearerInd = QO_SGW_BEARER_IND(tunnel->ebi);
   bearer = ueCb->bearerTable[bearerInd];

   if ((tunnel->state == QO_SGW_TUN_LCLDEL) &&
       (mgmtCfm->u.tnlMgmt.cfmStatus.status == LCM_PRIM_OK))
   {
      qoSgwDeleteUTunnel(tunnel);
      bearer->s1UTun = NULLP;

      /* if s5/s8 too is deleted then free the bearer */
      if (bearer->s5s8UTun == NULLP)
      {
         QO_SGW_FREE(bearer, sizeof(QoSgwBearerCb));
         ueCb->bearerTable[bearerInd] = NULLP;
         ueCb->bearerCnt -= 1;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************/
/***************************************************************/
/**              GTP-U Message Processors                      */
/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInUserData
(
SuId     suId, 
EgtUEvnt *data
)
#else
PUBLIC S16 qoSgwInUserData(suId,data)
SuId     suId;
EgtUEvnt *data;
#endif
{
   S16            ret            = ROK;
   QoSgwTunnelCb  *uInTun        = NULLP;
   QoSgwTunnelCb  *uOutTunn      = NULLP;
   QoSgwBearerCb  *bearer        = NULLP;
   EgMsg          *egMsg         = NULLP;
   QoEgtUEvntInfo *egtUEvntInfo  = NULLP;

   QO_SGW_TRC2(qoSgwInUserData);

   uInTun = qoSgwFindTunnel(qoSgwCb.egUSap, data->u.egMsg->msgHdr.teId);
   if(uInTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel not found for GTPU data.\n"));
      EgUUtilDeAllocGmMsg(&(data->u.egMsg));
      SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
            (Data *)data, (Size)sizeof(EgtUEvnt));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* If incoming tunnel is not in connected state, so packets will
    * be dropped */
   if(uInTun->state != QO_SGW_TUN_CONNECTED)
   {
      QO_SGW_DBG_ERR(("Data received on Disconnected Tunnel\n"));
      EgUUtilDeAllocGmMsg(&(data->u.egMsg));
      SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool, 
              (Data *)data, (Size)sizeof(EgtUEvnt));
      QO_SGW_RETVALUE(RFAILED);
   }

   bearer = uInTun->ueCb->bearerTable[QO_SGW_BEARER_IND(uInTun->ebi)];

   switch (uInTun->intf)
   {
      case QO_IF_S5_S8_DEF:
      case QO_IF_S5_S8_DED:
      {
         if(bearer->pdnCb->ueCb->intf == QO_IF_S11_C)
           uOutTunn = bearer->s1UTun;
         else if(bearer->pdnCb->ueCb->intf == QO_IF_S4_C)
           uOutTunn = bearer->s4UTun;
         QO_SGW_DBG_INFO(("Incoming GTP-U packet on S5/S8"));
         break;
      }

      case QO_IF_S1U_DEF:
      case QO_IF_S1U_DED:
      {
         uOutTunn = bearer->s5s8UTun;
         QO_SGW_DBG_INFO(("Incoming GTP-U packet on S1-U"));
         break;
      }
      case QO_IF_S4_DEF:
      case QO_IF_S4_DED:
      {
         uOutTunn = bearer->s5s8UTun;
         QO_SGW_DBG_INFO(("Incoming GTP-U packet on S4"));
         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Invalid Interface\n"));
         EgUUtilDeAllocGmMsg(&(data->u.egMsg));
         SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
                  (Data *)data, (Size)sizeof(EgtUEvnt));

         QO_SGW_RETVALUE(RFAILED);
         break;
      }
   }

   /* If out going tunnel is null, then send Error Ind to 
    * sender of packet */
   if (uOutTunn == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnels not in connected state to forward GTPU data"));
#if 0
      /* Send Error indication to peer */
      qoSgwSendGtpUErrInd(data, uInTun, qoSgwCb.egUSap, 0);
#endif
      EgUUtilDeAllocGmMsg(&(data->u.egMsg));
      SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
              (Data *)data, (Size)sizeof(EgtUEvnt));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* If outgoing tunnel is in disconnected state, the pkt need to buffered or
    * dropped depending on incoming tunnel */
   if (uOutTunn->state == QO_SGW_TUN_DISCONNECTED)
   {
      /* Removing the support for Buffering */
      /* Data coming from PGW so buffer it */
      if((QO_IF_S5_S8_DEF == uInTun->intf || QO_IF_S5_S8_DED == uInTun->intf)) 
      {
         if (uInTun->ueCb->bufferingState == QO_SGW_NO_BUFFERING)
         {
            uInTun->ueCb->bufferingState = QO_SGW_START_BUFFERING;
            ret = qoSgwOutDDN(egMsg, uInTun->ueCb->s11Tun);
            if (ret != ROK)
            {
               EgUUtilDeAllocGmMsg(&(data->u.egMsg));
               SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
                     (Data *)data, (Size)sizeof(EgtUEvnt));
               QO_SGW_RETVALUE(RFAILED);
            }
         }
         /* Drop packet if maximum packet buffering is reached */
         if (uInTun->ueCb->pendingOnS1.count >= QO_SGW_MAX_EGU_PKT_BUF_COUNT)
         {
            EgUUtilDeAllocGmMsg(&(data->u.egMsg));
            SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
                    (Data *)data, (Size)sizeof(EgtUEvnt));

            QO_SGW_RETVALUE(ROK);
         }

         /* Allocate memory for the link list node */
         ret = SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&egtUEvntInfo,
                        sizeof(QoEgtUEvntInfo));
         if(ret != ROK)
         {
            EgUUtilDeAllocGmMsg(&(data->u.egMsg));
            SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
                    (Data *)data, (Size)sizeof(EgtUEvnt));
            QO_SGW_RETVALUE(RFAILED);
         }

         /* Store data in linked list */
         egtUEvntInfo->egtUEvnt = data;

         cmLListAdd2Tail(&(uInTun->ueCb->pendingOnS1), &(egtUEvntInfo->lnk));
         egtUEvntInfo->lnk.node = (PTR)egtUEvntInfo;
    
         QO_SGW_RETVALUE(ROK);
      }
      else
      {
         /* Data coming from eNB, so drop a data pkt */
         EgUUtilDeAllocGmMsg(&(data->u.egMsg));
         SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
               (Data *)data, (Size)sizeof(EgtUEvnt));
         QO_SGW_RETVALUE(RFAILED);
      }
   } /* end of if statement */

   ret = qoSgwOutUserData(data, uOutTunn, qoSgwCb.egUSap);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Sending User Data, FAILURE"));
#if 0
      /* Send Error indication to sending peer */
      qoSgwSendGtpUErrInd(data, uInTun, qoSgwCb.egUSap, 0);
#endif
      EgUUtilDeAllocGmMsg(&(data->u.egMsg));
      SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
            (Data *)data, (Size)sizeof(EgtUEvnt));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwOutUserData
    (
      EgtUEvnt *data, 
      QoSgwTunnelCb *dataTun,
      QoSgwEgtSapCb *sap
    )
#else
PUBLIC S16 qoSgwOutUserData(data,dataTun,sap)
      EgtUEvnt *data;
      QoSgwTunnelCb *dataTun;
      QoSgwEgtSapCb *sap;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(OutUserData)

   /* qo001.101: need not check for detach */
   if(dataTun->state != QO_SGW_TUN_CONNECTED) 
   {
      /* Send Error indication to other peer in calling function */
      QO_SGW_DBG_ERR(("Tunnels not in connected state to forward GTPU \
               data.\n"));

      QO_SGW_RETVALUE(RFAILED);
   }

   /*  update headers */
   data->u.egMsg->msgHdr.teId = dataTun->remTeid;
   cmMemcpy ((U8 *)&(data->u.egMsg->srcAddr), (U8 *)(dataTun->locAddr),
         sizeof(CmTptAddr));
   cmMemcpy ((U8 *)&(data->u.egMsg->remAddr), (U8 *)(dataTun->remAddr),
         sizeof(CmTptAddr));

   QO_SGW_DBG_INFO(("Sending User Data."));
   ret = QoSgwLiEgtEguDatReq(&(sap->pst), sap->spId, data);
   if(ret != ROK)
   {
      /* Send Error indication to other peer in calling function */
      QO_SGW_DBG_ERR(("Failed to send the data packet.\n"));
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************/
/**              GTP-C Message Processors                      */
/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInCSReq
(
SuId        suId, 
CmTptAddr   *localAddr, 
CmTptAddr   *remAddr, 
TknU32      lclTeid, 
EgMsg       *egMsg
)
#else
PUBLIC S16 qoSgwInCSReq(suId, localAddr, remAddr, lclTeid, egMsg)
SuId        suId;
CmTptAddr   *localAddr;
CmTptAddr   *remAddr;
TknU32      lclTeid;
EgMsg       *egMsg;
#endif
{
   U32            remTeid = 0;
   S16            ret     = ROK;
   QoSgwUeCb      *ueCb   = NULLP;
   QoSgwPeerCb    *peer   = NULLP;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwTunnelCb  *s4Tun = NULLP;
   QoImsi         imsi    = {0, "\0"};
   U32            cause   = EGT_CAUSE_INVALID;
   QoSgwPdnCb     *pdnCb  = NULLP;
   EgIe           *pEgIe  = NULLP;
   U16             idx    = 0;

   QO_SGW_TRC2(qoSgwInCSReq);

   /* Getting S11 MME/S4 SGSN TEID */
   ret = qoSgwEgtUtlGetIE(egMsg, &pEgIe, EG_FTEID_IETYPE, 0);
   if (ROK != ret || NULLP == pEgIe)
   {
      /* Cannot send a failure response with 0 remote teid. So drop 
       * the message
       */
      QO_SGW_DBG_ERR(("Invalid Create Session Request received, Dropping "\
            "the message"));
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_RETVALUE(ROK);
   }
   else
   {
      remTeid = pEgIe->t.fTeid.teid;
   }

   /* Get IMSI from received message */
   cmMemset((U8 *)&imsi, 0, sizeof(QoImsi));
   ret = qoSgwFindUeCbUsingImsi(egMsg, &imsi, &cause, &ueCb);
   if (ROK == ret && NULLP != ueCb)
   {
      if (egMsg->msgHdr.teid != 0)
      {
         /* Multile PDN connection request         *
          * Create new PDN and process the request */
         ret = qoSgwCreatePdn(&pdnCb);
         if (ROK != ret)
         {
            cause = EGT_GTPC_CAUSE_NO_MEM_AVAIL;
            QO_SGW_DBG_ERR(("Creating PDN Cb"));
         }
         else
         {
            ueCb->pdnCnt++;
         }
      }
      else
      {
         /* New Attach request before a detach. Remove the older sessions
          * without doing any signalling */
         if (QO_SGW_UE_CONNECTED == ueCb->state || \
             QO_SGW_UE_DISCONNECTED == ueCb->state)
         {
            qoSgwDeleteUeCb(ueCb, QO_SGW_SIGNAL_PGW);
            ueCb = NULLP;
         }
         else
         {
            QO_SGW_DBG_INFO(("Create Session in progress, Deleting the "\
                  "Create Session Request Msg received"));
            EgUtilDeAllocGmMsg(&egMsg);
            QO_SGW_RETVALUE(ROK);
         }
      }
   }

   /* No UE Cb present, create a new ue Cb and pdn cb for this ue */
   if (NULLP == ueCb)
   {
      ret = qoSgwCreateUeCbUsingImsi(egMsg, &imsi, &cause, &ueCb);
      if (ROK == ret)
      {
         /* Create the PDN */
         ret = qoSgwCreatePdn(&pdnCb);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Creating PDN Cb"));
            qoSgwDeleteUeCb(ueCb, QO_SGW_SIGNAL_NO);
            cause = EGT_GTPC_CAUSE_NO_MEM_AVAIL;

            QO_SGW_DBG_INFO(("Sending Create Session Response To MME With "
                     "Cause(%d)", cause));

            if(suId == qoSgwCb.mmeCSap->suId) 
            {
               qoSgwEgtUtlSndFailureRspToMme(lclTeid.val, remTeid,\
                     EGT_GTPC_MSG_CS_RSP, egMsg->msgHdr.seqNumber, cause);
            }

            else if(suId == qoSgwCb.sgsnCSap->suId) 
            {
               qoSgwEgtUtlSndFailureRspToSgsn(lclTeid.val, remTeid,\
                     EGT_GTPC_MSG_CS_RSP, egMsg->msgHdr.seqNumber, cause);
            }
            EgUtilDeAllocGmMsg(&egMsg);
            QO_SGW_RETVALUE(ROK);
         }
         else
         {
            if(suId == qoSgwCb.mmeCSap->suId) 
            {
               ueCb->intf = QO_IF_S11_C;
            } 
            else if(suId == qoSgwCb.sgsnCSap->suId) 
            {
               ueCb->intf = QO_IF_S4_C;
            }
            ueCb->pdnCnt++;
         }
      }
   }
   pEgIe = NULLP;
   ret = qoSgwEgtUtlGetIE(egMsg, &pEgIe, EG_MSISDN_IETYPE, 0);
   if (ROK != ret || pEgIe == NULLP)
   {
      QO_SGW_DBG_ERR(("MSISDN Missing/Parsing-Failure In Gtpc Msg"));
   }
   else
   {
      ueCb->msisdnLen = pEgIe->t.valStr32.length;
      for(idx = 0; idx < ueCb->msisdnLen; idx ++)
      {
         ueCb->msisdn[idx] = pEgIe->t.valStr32.val[idx] - '0';
      }
   }
   ret = qoSgwFindNCreatePeer(remAddr, &peer);
   if (ret != ROK)
   {
      cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
      QO_SGW_DBG_ERR(("New Entry creation in Peer Table"));
   }
   else if(QO_IF_S11_C == ueCb->intf) 
   {
      peer->nodeType = QO_ENT_MME;
      ret = qoSgwFindNCreateTunnel(qoSgwCb.mmeCSap, lclTeid.val, &s11Tun);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("GTP-C Tunnel Control Block Creation."));
         cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
      }
   }
   else if(QO_IF_S4_C == ueCb->intf) 
   {
      peer->nodeType = QO_ENT_SGSN;
      ret = qoSgwFindNCreateTunnel(qoSgwCb.sgsnCSap, lclTeid.val, &s4Tun);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("GTP-C Tunnel Control Block Creation."));
         cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
      }
   }

   if (ROK == ret)
   {
      if(QO_IF_S11_C == ueCb->intf) 
      {
         s11Tun->ueCb = ueCb;
         s11Tun->plane = QO_CONTROL;
         s11Tun->remAddr =  &(peer->addr);
         s11Tun->state = QO_SGW_TUN_PROGRESS;
         s11Tun->intf = QO_IF_S11_C;
         ueCb->s11Tun = s11Tun;

         ret = qoSgwInCSReqStateHndlr(s11Tun, &pdnCb, egMsg);
      }
      else if(QO_IF_S4_C == ueCb->intf) 
      {
         s4Tun->ueCb = ueCb;
         s4Tun->plane = QO_CONTROL;
         s4Tun->remAddr =  &(peer->addr);
         s4Tun->state = QO_SGW_TUN_PROGRESS;
         s4Tun->intf = QO_IF_S4_C;
         ueCb->s4Tun = s4Tun;

         ret = qoSgwInCSReqStateHndlr(s4Tun, &pdnCb, egMsg);
      }
      if (ROK != ret)
      {
         cause = EGT_GTPC_CAUSE_SYS_FAIL;
         QO_SGW_DBG_ERR(("Processing Of Create Session Request"));
      }
      else
      {
         QO_SGW_DBG_INFO(("Processing Of Create Session Request, SUCCESS"));
      }
   }
   
   if (ret != ROK)
   {
      if(QO_IF_S11_C == ueCb->intf) 
      {
         /*Send Create session response with failure indication*/
         qoSgwEgtUtlSndFailureRspToMme(lclTeid.val, remTeid, EGT_GTPC_MSG_CS_RSP,\
               egMsg->msgHdr.seqNumber, cause);
      }
      else if(QO_IF_S4_C == ueCb->intf) 
      {
         /*Send Create session response with failure indication*/
         qoSgwEgtUtlSndFailureRspToSgsn(lclTeid.val, remTeid, EGT_GTPC_MSG_CS_RSP,\
               egMsg->msgHdr.seqNumber, cause);
      }

      /* Release the PDN. If this is the only PDN, it will release the UE
       * CB as well */
      qoSgwDeletePdn(pdnCb, QO_SGW_SIGNAL_NO);
      EgUtilDeAllocGmMsg(&egMsg);
   }

   QO_SGW_RETVALUE(ret);
}

/*
 *
 *       Fun:   qoSgwInCSReqStateHndlr
 *
 *       Desc:  Handles Create SessioN Request received from MME/SGSN based on 
 *              state 
 *             
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 */
#ifdef ANSI
PRIVATE S16 qoSgwInCSReqStateHndlr
(
QoSgwTunnelCb     *sTun,    /* S11 Tunnel Control block (INOUT) */
QoSgwPdnCb        **pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg      /* gtp-c message (IN)   */
)
#else
PRIVATE S16 qoSgwInCSReqStateHndlr(sTun, pdnCb, egMsg)
QoSgwTunnelCb     *sTun;    /* S11 Tunnel Control block (INOUT) */
QoSgwPdnCb        **pdnCb;     /* PDN Control Block (OUT) */
EgMsg             *egMsg;     /* gtp-c message (IN)   */
#endif
{
   U16            ret = RFAILED;
   QoSgwUeCb      *ueCb = NULLP;
   U32            cause = 0;

   QO_SGW_TRC2(qoSgwInCSReqStateHndlr)

   ueCb = sTun->ueCb;

   switch((*pdnCb)->state)
   {
      case QO_SGW_PDN_IDLE:
      {
         QO_SGW_DBG_INFO(("Create Session Request Received In IDLE State."));

         ret = qoSgwValCSReq(egMsg, sTun, pdnCb, &cause);
         if (ret == ROK)
         {
            /* S1 Handover with SGW change. Send Modify Bearer Request to PGW */
            if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC))
            {
               QO_SGW_DBG_INFO(("S1 Handover with SGW Relocation"));
               ret = qoSgwHandleCSReqForHandover(*pdnCb, egMsg);
               if (ROK == ret)
               {
                  ueCb->state = QO_SGW_UE_PROGRESS;
                  (*pdnCb)->state = QO_SGW_PDN_CREATE;
               }
               else
               {
                  QO_SGW_DBG_ERR(("Handling Create Session Request for S1 "\
                        "Handover with SGW change"));
               }
            }
            else
            {
               QO_SGW_DBG_INFO(("Initial Attach/PDN Connection Request"));
               ret = qoSgwHandleCSReqForAttach(*pdnCb, egMsg);
               if (ROK == ret)
               {
                  ueCb->state = QO_SGW_UE_PROGRESS;
                  (*pdnCb)->state = QO_SGW_PDN_CREATE;
               }
               else
               {
                  QO_SGW_DBG_ERR(("Handling of Create Session Request for"\
                        "Initial Attach/PDN Connection Request"));
               }
            }
         }
         else
         {
            ret = RFAILED;
            QO_SGW_DBG_ERR(("Validation Of Create Session Request"));
         }
        
         break;
      }

      case QO_SGW_PDN_CONNECTED:
      {
         QO_SGW_DBG_ERR(("Create Session Request Recd in CONNECTED State"));
         ret = RFAILED;
         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Create Session Request Recd in Invalid State"));
         ret = RFAILED;
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief 
 *    Builds and send Create Session Request to PGW
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[in]  pCsReq
 *    Pointer to GTP message structure containing Create Session Request
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 qoSgwOutCSReq
(
QoSgwPdnCb     *pPdnCb,
EgMsg          *pCsReq 
)
{
   TknU32            teidTkn = {0};
   TknU8             create = {0};
   S16               ret = ROK;
   QoSgwTunnelCb     *pS5S8CTun = NULLP;

   QO_SGW_TRC2(qoSgwOutCSReq)

   pS5S8CTun = pPdnCb->s5s8CTun;

   /* Re-use the Create Session request received from MME to prepare
    * the crease session request that needs to be sent to PGW
    * for this, remove the MME specific IEs.
    */
   qoSgwRemCSReqIes(pCsReq, pS5S8CTun);

   ret = qoSgwBldCSReq(pPdnCb, pCsReq);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Building Of Create Session Request To PDN-GW, FAILED"));
   }
   else
   {
      QO_SGW_DBG_INFO(("Sending Create Session Request To PDN-GW For "
                       "Transaction %ld", pS5S8CTun->transId.val)); 

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = pS5S8CTun->locTeid;
      create.pres = PRSNT_NODEF;
      create.val = TRUE;

      /* Send out on GTP-C */
      pS5S8CTun->currEvent = QO_SGW_EGT_EVT_CS_REQ_SENT;
      QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                     pS5S8CTun->locAddr, pS5S8CTun->remAddr, teidTkn, 
                     pS5S8CTun->transId, create, qoSgwCb.t3, pCsReq);
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwOutCSReq */



/**********************************************************************
 *
 *       Fun:   qoSgwInCSRsp
 *
 *       Desc:  Handles Create SessioN Resposne received from PGW 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInCSRsp
(
SuId           suId,       /* service user id */
CmTptAddr      *locAddr,   /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         locTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *msg        /* Create Session Response Msg */
)
#else
PUBLIC S16 qoSgwInCSRsp(suId, locAddr, remAddr, lolTeid, transId, msg)
SuId        suId, 
CmTptAddr   *locAddr,
CmTptAddr   *remAddr, 
TknU32      locTeid, 
TknU32      transId, 
EgMsg       *msg
#endif
{
   S16                     ret = ROK;
   QoSgwTunnelCb           *s11Tun = NULLP;
   QoSgwTunnelCb           *s4Tun = NULLP;
   QoSgwTunnelCb           *s5s8CTun = NULLP;
   U8                      bearerInd = 0;
   QoSgwPdnCb              *pdnCb = NULLP;
   U32                     cause = 0;
   EgIeLevelInfo           ieLvlInfo = {0};
   EgIe                    *egIe = NULLP;
   U16                     iePos = 0;

   QO_SGW_TRC2(qoSgwInCSRsp);

   s5s8CTun = qoSgwFindTunnelTrans(qoSgwCb.pgwCSap, transId.val);
   if(s5s8CTun == NULLP)
   {
      s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, locTeid.val);
      if(s5s8CTun == NULLP)
      {
         /* Context not found for response, discard the message */
         QO_SGW_DBG_ERR(("Tunnel Not Found For Create Session Response"));
         QO_SGW_RETVALUE(RFAILED);
      }
   }
   
   /*Bearer map index*/
   bearerInd = QO_SGW_BEARER_IND(s5s8CTun->ebi);
   pdnCb     = s5s8CTun->ueCb->bearerTable[bearerInd]->pdnCb;
   s11Tun    = pdnCb->ueCb->s11Tun;
   s4Tun     = pdnCb->ueCb->s4Tun;
   if (pdnCb->state == QO_SGW_PDN_CREATE)
   {
      QO_SGW_DBG_INFO(("Create Session Response Received In ATTACH State."));
   }
   else
   {
      QO_SGW_DBG_ERR(("Create Session Response Is Received In Invalid State.\
               Message Discarded"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /*Update local tunnel id*/
   s5s8CTun->locTeid = locTeid.val;
   s5s8CTun->transId.pres = NOTPRSNT;

   /* add in TEID hash list remove from transId hash list */
   ret = qoSgwTransToTeidHashList(qoSgwCb.pgwCSap, s5s8CTun);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Transferring Tunnel To Trans To Tied Hash"));

      /* Get cause and Teid from PGW */
      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_CAUSE_IETYPE, 0);
      ret = EgUtilGmGetIe(msg, &ieLvlInfo, &egIe, &iePos);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Retrieval Of Cause IE, FAILED"));
         cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
      }
      else
      {
         /* if cause is successful, then only send delete session request to PGW
            else send only create session with failure indication to MME */
         if (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ)
         {
            cause = egIe->t.valUnSgnd8;
         }
         else
         {
            cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
            QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_FTEID_IETYPE, 1);
            ret = EgUtilGmGetIe(msg, &ieLvlInfo, &egIe, &iePos);
            if (ret != ROK)
            {
               cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
               QO_SGW_DBG_ERR(("Retrieval of PGW FTEID IE, FAILED"));
            }
            else
            {
               s5s8CTun->remAddr->type = CM_IPV4ADDR_TYPE;
               s5s8CTun->remTeid = egIe->t.fTeid.teid;
               qoSgwCreateNSendDeleteSessReq(s5s8CTun, EGT_GTPC_CAUSE_SYS_FAIL);
            }
         }
      }
      if(QO_IF_S11_C == s5s8CTun->ueCb->intf)
      {
         qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,
               EGT_GTPC_MSG_CS_RSP, s11Tun->transId.val, cause);
      }
      if(QO_IF_S4_C == s5s8CTun->ueCb->intf)
      {
         qoSgwEgtUtlSndFailureRspToSgsn(s4Tun->locTeid, s4Tun->remTeid,
               EGT_GTPC_MSG_CS_RSP, s4Tun->transId.val, cause);
      }
      s5s8CTun->state = QO_SGW_TUN_IDLE;
      pdnCb->state = QO_SGW_PDN_IDLE;
      QO_SGW_RETVALUE(RFAILED);
   }

   ret = qoSgwCSRspStateHndlr(pdnCb, msg);
   if (ret != ROK)
   {
      QO_SGW_DBG_ERR(("Handling of Create Session Response from PGW"));
   }
   else
   {
      QO_SGW_DBG_INFO(("Processing Of Create Session Response From PGW,"
                     " SUCCESS"));
   }
   
   QO_SGW_RETVALUE(ret);
}


/*
 *
 *       Fun:   qoSgwCSRspStateHndlr
 *
 *       Desc:  Handles Create SessioN Request received from MME based on 
 *              state 
 *             
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 */
#ifdef ANSI
PRIVATE S16 qoSgwCSRspStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg     /* gtp-c message (IN)   */
)
#else
PRIVATE S16 qoSgwCSRspStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,    /* gtp-c message (IN)   */
#endif
{
   S16            ret = ROK;
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwBearerCb  *defBearer = NULLP;
   U32            cause = 0;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwTunnelCb  *s4Tun = NULLP;

   QO_SGW_TRC2(qoSgwCSRspStateHndlr)

   ueCb = pdnCb->ueCb;
   s11Tun = ueCb->s11Tun;
   s4Tun = ueCb->s4Tun;

   QO_SGW_GET_BEARER(ueCb->bearerTable, pdnCb->s5s8CTun->ebi, defBearer);
   if (NULLP == defBearer)
   {
      QO_SGW_DBG_ERR(("Default Bearer CB Not found"));
      QO_SGW_RETVALUE(RFAILED);
   }

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CREATE:
      {
         QO_SGW_DBG_INFO(("Create Session Response Received In ATTACH State"));
         ret = qoSgwValCSRsp(pdnCb, egMsg, &cause);
         if (ret == ROK)
         {
            QO_SGW_DBG_INFO(("Sending Tunnel Add Request to EGT Layer"));
            ret = qoSgwEguTunnelAddReq(defBearer, NULLP,\
                  EGT_INT_S5_S8_SGW_GTP_U);
            if(ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending Tunnel Add Request to EGT Layer"));
            }
            else
            {
               ret = qoSgwOutCSRsp(pdnCb, egMsg);
               if (ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending The Create Session Reponse"));
               }
               else
               {
                  pdnCb->s5s8CTun->state = QO_SGW_TUN_CONNECTED;
               }
            }
         }
         else
         {
            QO_SGW_DBG_ERR(("Validation of Create Session Response"));
            if(QO_IF_S11_C == ueCb->intf)
            {
               qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,\
                     EGT_GTPC_MSG_CS_RSP, egMsg->msgHdr.seqNumber,\
                     EGT_GTPC_CAUSE_INV_RPL_FRM_PEER);
            }
            else if(QO_IF_S4_C == ueCb->intf)
            {
               qoSgwEgtUtlSndFailureRspToSgsn(s4Tun->locTeid, s4Tun->remTeid,\
                     EGT_GTPC_MSG_CS_RSP, egMsg->msgHdr.seqNumber,\
                     EGT_GTPC_CAUSE_INV_RPL_FRM_PEER);
            }
            qoSgwDeletePdn(pdnCb, QO_SGW_SIGNAL_NO);
            ret = ROK;
         }

         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Create Session Response Received In Invalid State"));
         ret = RFAILED;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwOutCSRsp
 *
 *       Desc:  Handles Create Session Response received from PGW
 *              This function will not forward the message to MME.
 *              The forwarding is done when Tunnel Mgmt confirmation
 *              is received from EGT-U
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutCSRsp
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg     /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutCSRsp(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;    /* gtp-c msg */
#endif
{
   TknU32            teidTkn    = {0};
   S16               ret        = ROK;
   QoSgwTunnelCb     *s11Tun    = NULLP;
   QoSgwTunnelCb     *s4Tun    = NULLP;
   QoSgwBearerCb     *defBearer = NULLP;

   QO_SGW_TRC2(qoSgwOutCSRsp)

      s11Tun = pdnCb->ueCb->s11Tun;
   s4Tun = pdnCb->ueCb->s4Tun;

   QO_SGW_GET_BEARER(pdnCb->ueCb->bearerTable, pdnCb->s5s8CTun->ebi,\
         defBearer);
   if (NULLP == defBearer)
   {
      QO_SGW_DBG_ERR(("Default Bearer CB Not found"));
      QO_SGW_RETVALUE(RFAILED);
   }

   if(QO_IF_S11_C == pdnCb->ueCb->intf)
   {
      QO_SGW_DBG_INFO(("Creating SGW Apps's S1-U Tunnel CB"));
      ret = qoSgwCreateUTunnel(pdnCb, &(defBearer->s1UTun), QO_IF_S1U_DEF);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Creating SGW Apps's S1-U Tunnel CB"));
      }
      else
      {
         defBearer->s1UTun->ebi = pdnCb->s5s8CTun->ebi;
         ret = qoSgwBldIACSRsp(pdnCb, egMsg);
         if (ROK == ret)
         {
            QO_SGW_DBG_INFO(("Sending Create Session Response to EGT Layer"));

            teidTkn.pres = PRSNT_NODEF;
            teidTkn.val = s11Tun->locTeid;
            QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,\
                  teidTkn, egMsg);
         }
         else
         {
            QO_SGW_DBG_ERR(("Building of Create Session Response"));
         }
      }
   }
   else if(QO_IF_S4_C == pdnCb->ueCb->intf)
   {
      ret = qoSgwBldIACSRsp(pdnCb, egMsg);
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Sending Create Session Response to EGT Layer"));

         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val = s4Tun->locTeid;
         QoSgwLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId,\
               teidTkn, egMsg);
      }
      else
      {
         QO_SGW_DBG_ERR(("Building of Create Session Response"));
      }
   }

   QO_SGW_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Handles the create session request received as a part of 
 *    S1 Handover with SGW relocation
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[in]  pCsReq
 *    Pointer to GTP message structure containing Create Session Request
 * 
 * @return
 *    SUCCESS if Modify Bearer Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 qoSgwHandleCSReqForHandover
(
QoSgwPdnCb     *pPdnCb,
EgMsg          *pCsReq
)
{
   TknU32            teidTkn = {0};
   S16               ret = ROK;
   QoSgwBearerCb     *pDefBearer = NULLP;
   QoSgwUeCb         *pUeCb = NULLP;
   EgMsg             *pCsRsp = NULLP;

   QO_SGW_TRC2(qoSgwHandleCSReqForHandover);

   pUeCb = pPdnCb->ueCb;
   pDefBearer = qoSgwFndDefBearer(pPdnCb);

   if(NULLP == pDefBearer)
   {
      QO_SGW_DBG_ERR(("Creation Of S1-U Default Tunnel failed\n"));
      QO_SGW_RETVALUE(RFAILED);
   }   

   QO_SGW_DBG_INFO(("Creation Of S1-U Default Tunnel"));
   ret = qoSgwCreateUTunnel(pPdnCb, &(pDefBearer->s1UTun), QO_IF_S1U_DEF);
   if(ROK != ret)
   {
      QO_SGW_DBG_ERR(("Creation Of S1-U Default Tunnel"));
      QO_SGW_RETVALUE(RFAILED);
   }

   pDefBearer->s1UTun->ebi = pDefBearer->ebi;

   ret = qoSgwBldHOCSRsp(pPdnCb, &pCsRsp);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Sending Create Session Response to EGT Layer"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = pUeCb->s11Tun->locTeid;
      QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,\
            teidTkn, pCsRsp);
   }
   else
   {
      QO_SGW_DBG_ERR(("Building Create Session Response"));
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwHandleCSReqForHandover */

/**************************************************************************//**
 *
 * @brief 
 *    Handles the create session request received as a part of 
 *    Initial Attach Procedure
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[in]  pCsReq
 *    Pointer to GTP message structure containing Create Session Request
 * 
 * @return
 *    SUCCESS if Modify Bearer Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 qoSgwHandleCSReqForAttach
(
QoSgwPdnCb     *pPdnCb,
EgMsg          *pCsReq
)
{
   S16               ret = ROK;
   QoSgwTunnelCb     *pS5S8CTun = NULLP;
   QoSgwBearerCb     *pDefBearer = NULLP;
   QoSgwUeCb         *pUeCb = NULLP;

   QO_SGW_TRC2(qoSgwHandleCSReqForAttach)

   pUeCb = pPdnCb->ueCb;  
   pS5S8CTun = pPdnCb->s5s8CTun;
   pDefBearer = pUeCb->bearerTable[QO_SGW_BEARER_IND(pS5S8CTun->ebi)];

   ret = qoSgwCreateUTunnel(pPdnCb, &(pDefBearer->s5s8UTun), QO_IF_S5_S8_DEF);
   if(ROK != ret)
   {
      QO_SGW_DBG_ERR(("Creation Of U-Plane Tunnel for S5/S8 interface, FAILED"));
      QO_SGW_RETVALUE(RFAILED);
   }

   pDefBearer->s5s8UTun->ebi = pDefBearer->ebi;

   /* Send the create session request to PGW */
   ret = qoSgwOutCSReq(pPdnCb, pCsReq);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Sent Create Session Request to PGW successfully"));
   }
   else
   {
      QO_SGW_DBG_ERR(("Sending Create Session Request to PGW"));

      qoSgwDeleteUTunnel(pDefBearer->s5s8UTun);
      pDefBearer->s5s8UTun = NULLP;
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwHandleCSReqForAttach */


/**************************************************************************//**
 *
 * @brief 
 *    Sends Modify Bearer Request to PGW in case of S1 Handover with
 *    SGW change. Handling is done for only one (default) bearer
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[in]  pMbReq
 *    Pointer to GTP message structure containing Modify Bearer Request
 *    received from MME
 * 
 * @return
 *    SUCCESS if Modify Bearer Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 qoSgwOutMBReq
(
QoSgwPdnCb     *pPdnCb,
EgMsg          *pMbReq
)
{
   S16               ret         = ROK;
   TknU32            teidTkn     = {0};
   TknU8             create      = {PRSNT_NODEF, FALSE};
   QoSgwTunnelCb     *pS5S8CTun  = NULLP;
   EgMsg             *pPgwMbReq  = NULLP;

   QO_SGW_TRC2(qoSgwOutMBReq);

   pS5S8CTun = pPdnCb->s5s8CTun;

   ret = qoSgwBldMBReq(pPdnCb, &pPgwMbReq);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Sending Modify Bearer Request EGT Layer"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = pS5S8CTun->locTeid;

      if (0 == pS5S8CTun->locTeid)
      {
         create.val   = TRUE;
      }

      pS5S8CTun->currEvent = QO_SGW_EGT_EVT_MB_REQ_SENT;
      QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,\
            pS5S8CTun->locAddr, pS5S8CTun->remAddr, teidTkn,\
            pS5S8CTun->transId, create, qoSgwCb.t3, pPgwMbReq);
   }
   else
   {
      QO_SGW_DBG_ERR(("Building Modify Bearer Request"));
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwOutMBReq */


/***************************************************************
 *
 *
 *       Fun:   qoSgwInMBReq
 *
 *       Desc:  Handles Modify Bearer request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInMBReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInMBReq(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;       /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *sTun = NULLP;
   QoSgwUeCb         *ueCb = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;
   EgIe              *egIe = NULLP;
   S16               ratType = QO_RAN_INVALID;

   QO_SGW_TRC2(qoSgwInMBReq)

   sTun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if( sTun == NULLP )
   {
      sTun = qoSgwFindTunnel(qoSgwCb.sgsnCSap, lclTeid.val);
   }
   if (sTun == NULLP) 
   {
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_DBG_ERR(("Tunnel Not Found For Modify Bearer Request."));
      QO_SGW_RETVALUE(RFAILED);
   }

   ueCb = sTun->ueCb;
   ret = qoSgwGetPdnCbUsingEgMsg(ueCb, egMsg, &pdnCb);
   if (ROK != ret)
   {
      /* Modify Bearer Request received as a part of Release Access Bearer
       * So still process the request
       */
      QO_SGW_DBG_INFO(("Failed to find PDN for Modify Bearer Request"));
   }

   /* Getting RAT TYPE */ 
   /* This IE is Mandatory in case of I-RAT HO,so get the IE value and based on 
   the RAT Type process the MB Req*/
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_RAT_TYPE_IETYPE, 0);
   if (ROK == ret && NULLP != egIe)
   {            
      ratType = egIe->t.valUnSgnd8;
   }
   
   ret = qoSgwMBReqStateHndlr(ratType, ueCb, pdnCb, egMsg, remAddr);
   if (ret != ROK)
   {
      QO_SGW_DBG_ERR(("Processing Of Modify Bearer Request"));
   }
   else
   {
      QO_SGW_DBG_INFO(("Processing Of Modify Bearer Request, SUCCESS"));
   }

   if (ret != ROK)
   {
      
      if(ratType == QO_RAT_UTRAN )
      {
         if(ueCb->s4Tun != NULLP)
         {
            qoSgwEgtUtlSndFailureRspToSgsn(ueCb->s4Tun->locTeid, ueCb->s4Tun->remTeid,
                  EGT_GTPC_MSG_MB_RSP, egMsg->msgHdr.seqNumber,\
                  EGT_GTPC_CAUSE_SYS_FAIL);
         }
         else
         {
            /* Should not hit This case */
            QO_SGW_DBG_ERR(("Failed to send modify Bearer failure Rsp to SGSN \n"));
         }
      }
      else
      {
         qoSgwEgtUtlSndFailureRspToMme(sTun->locTeid, sTun->remTeid,
               EGT_GTPC_MSG_MB_RSP, egMsg->msgHdr.seqNumber,\
               EGT_GTPC_CAUSE_SYS_FAIL);
      }
   }

   if((ret = EgUtilDeAllocGmMsg(&egMsg)) == EG_UTIL_ERR_INV_PARAM)
   {  
      /* 
       * If the egMsg pointer is freed already and we are passing NULL pointer
       * to free, this code leg should hit.
       */
      QO_SGW_DBG_INFO(("egMsg is pointer is NULL"));
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwMBReqStateHndlr
 *
 *       Desc:  Handles Modify Bearer request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwMBReqStateHndlr
(
U32               ratType,
QoSgwUeCb         *ueCb,
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg,      /* gtp-c message (IN)   */
CmTptAddr         *remAddr   /* remote address */
)
#else
PRIVATE S16 qoSgwMBReqStateHndlr(ratType, ueCb, pdnCb, egMsg, remAddr)
U32               ratType;
QoSgwUeCb         *ueCb;
QoSgwPdnCb        *pdnCb;
EgMsg             *egMsg;     /* gtp-c message (IN)   */
CmTptAddr         *remAddr;   /* remote address */
#endif
{
   S16               ret = ROK;
   U32               cause = 0;

   QO_SGW_TRC2(qoSgwMBReqStateHndlr);

   if (NULLP == pdnCb)
   {
      QO_SGW_DBG_ERR(("PDN CB is NULL"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Received modify bearer request from the SGSN */
   if(ratType == QO_RAT_UTRAN)
   {
      ret = qoSgwValMBReqfrmSgsnIrat(pdnCb, egMsg, &cause, remAddr);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("validation of modify bearer request from SGSN"\
            "during IRAT failed"));
         RETVALUE(RFAILED);
      }
      else
      {
         /* calling QoSgwEgcTunnelModReq function, to update the 
                * C-plane teid and remaddr to EGTP */
         ret = QoSgwEgcTunnelModReq(ueCb, EGT_INT_S4_SGSN_GTP_C);
         if(ret != ROK)
         {
            QO_SGW_DBG_ERR(("C-plane modification Request failed"));
            RETVALUE(RFAILED);
         }
      }
      ueCb->intf = QO_IF_S4_C;
   }
   else if(ratType == QO_RAT_EUTRAN)
   {
      ret = qoSgwValMBReqfrmMmeIrat(pdnCb, egMsg, &cause, remAddr);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("validation of modify bearer request from MME"\
            "during IRAT failed"));
         RETVALUE(RFAILED);
      }
      else
      {
         /* calling QoSgwEgcTunnelModReq function, to update the 
                * C-plane teid and remaddr to EGTP */
         ret = QoSgwEgcTunnelModReq(ueCb, EGT_INT_S11_MME_GTP_C);
         if(ret != ROK)
         {
            QO_SGW_DBG_ERR(("C-plane modification Request failed"));
            RETVALUE(RFAILED);
         }
      }
      ueCb->intf = QO_IF_S11_C;
   }
   /* Received modify bearer request is received from the MME */
   else 
   {
      ret = qoSgwValMBReq(pdnCb, egMsg, &cause);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("validation of modify bearer request from MME failed"));
         RETVALUE(RFAILED);
      }
   }


   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Validation Of Modify Bearer Request, SUCCESS"));

      switch (pdnCb->state)
      {
         case QO_SGW_PDN_CREATE:
         {
            if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC) \
               || QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED))
            {
               /* S1 Handover with SGW relocation */
               ret = qoSgwOutMBReq(pdnCb, egMsg);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending Modify Bearer Response"));
               }
               else
               {
                  ueCb->state = QO_SGW_UE_CONNECTED;
               }
            }
            else
            {
               /* Initial LTE-Attach Case */
               ret = qoSgwOutMBRsp(pdnCb, egMsg);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending Modify Bearer Response"));
               }
               else
               {
                  ueCb->state = QO_SGW_UE_CONNECTED;
               }
            }

            break;
         }

         case QO_SGW_PDN_CONNECTED:
         {
            if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED))
            {
               /* S1 HO without SGW Relocation, and ULI Change */
               ret = qoSgwOutMBReq(pdnCb, egMsg);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending Modify Bearer Response"));
               }
               else
               {
                  ueCb->state = QO_SGW_UE_CONNECTED;
                  pdnCb->state = QO_SGW_PDN_MODIFY;
               }
            }
            else
            {
               ret = qoSgwOutMBRsp(pdnCb, egMsg);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending Modify Bearer Response"));
               }
               else
               {
                  ueCb->state = QO_SGW_UE_CONNECTED;
               }
            }

            break;
         }
         case QO_SGW_PDN_MODIFY:
         {  
            if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED))
            {
               ret = qoSgwOutMBReq(pdnCb, egMsg);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending Modify Bearer Response"));
               }
               else
               {
                  ueCb->state = QO_SGW_UE_CONNECTED;
                  pdnCb->state = QO_SGW_PDN_MODIFY;
               }
            }
            else
            {
               /* If modify bearer request is coming from the SGSN */
               if(ratType  == QO_RAT_UTRAN)
               {
                  ret = qoSgwOutMBRspToSGSN(pdnCb, egMsg);
               }
               /*Modify bearer request is from the MME */
               else
               {
                  ret = qoSgwOutMBRsp(pdnCb, egMsg);
               }
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Sending Modify Bearer Response"));
               }
               else
               {
                  ueCb->state = QO_SGW_UE_CONNECTED;
               }
            }
            break;
         }

         default:
         {
            QO_SGW_DBG_ERR(("Modify Bearer Request Received In Invalid State"));
            break;
         }
      }
   }
   else
   {

      if(ratType == QO_RAT_UTRAN)
      {
         QO_SGW_DBG_ERR(("Validation Of Modify Bearer Request from SGSN failed"));

         QO_SGW_DBG_INFO(("Sending Moidfy Bearer Response to SGSN"));
         qoSgwEgtUtlSndFailureRspToMme(pdnCb->ueCb->s4Tun->locTeid,\
               pdnCb->ueCb->s4Tun->remTeid, EGT_GTPC_MSG_MB_RSP,\
               egMsg->msgHdr.seqNumber, cause);
         ret = RFAILED;
      }
      else
      {
         QO_SGW_DBG_ERR(("Validation Of Modify Bearer Request from SGW failed"));

         QO_SGW_DBG_INFO(("Sending Moidfy Bearer Response to MME"));
         qoSgwEgtUtlSndFailureRspToMme(pdnCb->ueCb->s11Tun->locTeid,\
               pdnCb->ueCb->s11Tun->remTeid, EGT_GTPC_MSG_MB_RSP,\
               egMsg->msgHdr.seqNumber, cause);
         ret = RFAILED;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwProcRABReq
 *
 *       Desc:  Handles Modify Bearer request received as a part of
 *              Release Access Bearer Request message from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwProcRABReq
(
QoSgwUeCb         *ueCb,
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwProcRABReq(pdnCb, egMsg)
QoSgwUeCb         *ueCb;
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   S16         ret = ROK;
   U8          indx = 0;

   QO_SGW_TRC2(qoSgwProcRABReq)

   QO_SGW_DBG_INFO(("Changing state of ALL S1-U Tunnels to "
            "QO_SGW_TUN_DISCONNECTED"));

   for (indx = 0; indx < QO_SGW_MAX_BEARERS; indx++)
   {
      if ((NULLP != ueCb->bearerTable[indx]) &&
          (NULLP != ueCb->bearerTable[indx]->s1UTun))
      {
         ueCb->bearerTable[indx]->s1UTun->state = QO_SGW_TUN_DISCONNECTED;
      }
   }

   /* As part of service request procedure */
   ret = qoSgwEgtUtlSndSuccessRsp(ueCb->s11Tun, egMsg->msgHdr.seqNumber,\
         EGT_GTPC_MSG_RAB_RSP, QO_ENT_MME);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Sending Release Access Bearer Response to MME"));
      ueCb->state = QO_SGW_UE_DISCONNECTED;
   }
   else
   {
      QO_SGW_DBG_ERR(("Sending Release Access Bearer Response to MME"));
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwOutMBRsp
 *
 *       Desc:  Handles Modify Bearer request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutMBRsp
(
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutMBRsp(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   TknU32         teidTkn     = {0};
   S16            ret = ROK;
   U8             indx = 0;
   EgMsg          *mbRsp = NULLP;
   QoSgwBearerCb  **bearerTable = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwTunnelCb  *s1UTun = NULLP;

   QO_SGW_TRC2(qoSgwOutMBRsp)

   ueCb = pdnCb->ueCb;
   bearerTable = ueCb->bearerTable;

   ret = qoSgwBldMBRsp(pdnCb, &mbRsp);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Building Of Modify Bearer Response"));
      QO_SGW_RETVALUE(ret);
   }

   for (indx = 0; indx < QO_SGW_MAX_BEARERS; indx++)
   {
      if (NULLP != bearerTable[indx] && bearerTable[indx]->pdnCb == pdnCb)
      {
         if (bearerTable[indx]->op_mask & QO_SGW_DEL_BEARER)
         {
            if (bearerTable[indx]->isDefBearer == TRUE)
            {
               /* Send Pending Response, update eg header */
               mbRsp->msgHdr.teidPres = PRSNT_DEF;
               mbRsp->msgHdr.teid = ueCb->s11Tun->remTeid;
               mbRsp->msgHdr.seqNumber = egMsg->msgHdr.seqNumber;
               teidTkn.pres = PRSNT_NODEF;
               teidTkn.val = ueCb->s11Tun->locTeid;

               QO_SGW_DBG_INFO(("Sending Moidfy Bearer Response to MME"));
               QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                              teidTkn, mbRsp);

               qoSgwDeletePdn(bearerTable[indx]->pdnCb, QO_SGW_SIGNAL_NO);
               break;
            }
            else
            {
               qoSgwDeleteBearer(bearerTable[indx]);
            }
         }
         else if (bearerTable[indx]->op_mask & QO_SGW_MOD_BEARER)
         {
            ueCb->bufferingState = QO_SGW_NO_BUFFERING;
            QO_SGW_UNSET_MASK(bearerTable[indx]->op_mask, QO_SGW_MOD_BEARER);
            ret = qoSgwEguTunnelModReq(bearerTable[indx], mbRsp,\
                  EGT_INT_S1_U_SGW_GTP_U);
            if (ROK == ret)
            {
               s1UTun = bearerTable[indx]->s1UTun;
               s1UTun->remTeid = s1UTun->newRemTeid;
               s1UTun->remAddr = s1UTun->newRemAddr;
            }
            else
            {
               QO_SGW_DBG_ERR(("Tunnel Mod Management Request"));
            }
         }
         else if (bearerTable[indx]->op_mask & QO_SGW_CRT_BEARER)
         {
            QO_SGW_UNSET_MASK(bearerTable[indx]->op_mask, QO_SGW_CRT_BEARER);
            s1UTun = bearerTable[indx]->s1UTun;
            if (s1UTun->state == QO_SGW_TUN_PROGRESS)
            {
               /* Initial Attach */
               ret = qoSgwEguTunnelAddReq(bearerTable[indx], mbRsp,
                                          EGT_INT_S1_U_SGW_GTP_U);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("Tunnel Add Management Request"));
               }
            }
         }
      }
   }  

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwOutMBRspToSGSN 
 *
 *       Desc:  Handles Modify Bearer request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutMBRspToSGSN
(
QoSgwPdnCb        *pdnCb,
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutMBRspToSGSN(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   S16            ret = ROK;
   U8             indx = 0;
   EgMsg          *mbRsp = NULLP;
   QoSgwBearerCb  **bearerTable = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   EgIe           *egIe = NULLP;

   QO_SGW_TRC2(qoSgwOutMBRspToSGSN)

   ueCb = pdnCb->ueCb;
   bearerTable = ueCb->bearerTable;

   ret = qoSgwBldMBRspToSGSN(pdnCb, &mbRsp);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Building Of Modify Bearer Response"));
      QO_SGW_RETVALUE(ret);
   }

   for (indx = 0; indx < QO_SGW_MAX_BEARERS; indx++)
   {
      /* Get the EBI Id */
      ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
      if (ret != ROK || egIe == NULLP)
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("EBI IE Could Not Be Retrieved"));
         break;
      }

      if (NULLP != bearerTable[indx] && bearerTable[indx]->ebi == egIe->t.valUnSgnd8)
      {
         if (bearerTable[indx]->op_mask & QO_SGW_DEL_BEARER)
         {
            if (bearerTable[indx]->isDefBearer == TRUE)
            {
               /* Send Pending Response, update eg header */
               mbRsp->msgHdr.teidPres = PRSNT_DEF;
               mbRsp->msgHdr.teid = ueCb->s4Tun->remTeid;
               mbRsp->msgHdr.seqNumber = egMsg->msgHdr.seqNumber;
               teidTkn.pres = PRSNT_NODEF;
               teidTkn.val = ueCb->s4Tun->locTeid;

               QO_SGW_DBG_INFO(("Sending Moidfy Bearer Response to SGSN"));
               QoSgwLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId,
                              teidTkn, mbRsp);

               qoSgwDeletePdn(bearerTable[indx]->pdnCb, QO_SGW_SIGNAL_NO);
               break;
            }
            else
            {
               qoSgwDeleteBearer(bearerTable[indx]);
            }
         }
         else if (bearerTable[indx]->op_mask & QO_SGW_MOD_BEARER)
         {
            QO_SGW_UNSET_MASK(bearerTable[indx]->op_mask, QO_SGW_MOD_BEARER);

            ueCb->bufferingState = QO_SGW_NO_BUFFERING;
            ret = qoSgwEguTunnelModReq(bearerTable[indx], mbRsp,\
                  EGT_INT_S4_SGW_GTP_U);
            if(ret != ROK) 
            {
               QO_SGW_DBG_ERR(("Tunnel Mod Management Request from SGSN failed"));
            }
         }
      }
   }  

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwOutDDN
 *
 *       Desc:  Sends Downlink data notification toMME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutDDN
    (
      EgMsg *msg, 
      QoSgwTunnelCb *s11Tun
    )
#else
PRIVATE S16 qoSgwOutDDN(msg,s11Tun)
      EgMsg *msg;
      QoSgwTunnelCb *s11Tun;
#endif
{
   TknU32  teidTkn;
   TknU8  create;
   S16      ret = ROK;
   CmTptAddr  tmpAddr;

   QO_SGW_TRC2(qoSgwOutDDN)

   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val  = s11Tun->locTeid;
   create.pres=PRSNT_NODEF;
   create.val = FALSE;
   cmMemcpy((U8*)&tmpAddr, (U8*)s11Tun->remAddr, sizeof(CmTptAddr));
  
   tmpAddr.u.ipv4TptAddr.port =  2123; 

   /* Build Downlink Data Notification */
   ret = qoSgwBldDDN(s11Tun, &msg);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Building the Downlink Data Notification failed.\n"));
      QO_SGW_RETVALUE(RFAILED);
   }

   QoSgwLiEgtSigReq(&(qoSgwCb.mmeCSap->pst),
                  qoSgwCb.mmeCSap->spId, s11Tun->locAddr, &tmpAddr,
                  teidTkn, s11Tun->transId, create, qoSgwCb.t3, msg);
   QO_SGW_RETVALUE(ret);
}

/*****************************************************************
 *
 *       Fun:   qoSgwInDDNAck
 *
 *       Desc:  Handles Downlink Data Notification Act from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDDNAck
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDDNAck(suId, localAddr, remAddr, lclTeid, transId,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s11Tun = NULLP;

   QO_SGW_TRC2(qoSgwInDDNAck)

   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP) 
   {
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_DBG_ERR(("Tunnel not found for Downlink Data Notif ACK"));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO (("Received Downlink Data Notif ACK from MME for tunnel "
                     "%ld\n", s11Tun->locTeid)); 

   ret = qoSgwInDDNAckStateHndlr(s11Tun->ueCb, egMsg);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Downlink Data Notif Ack State Handling, SUCCESS"));
   }
   else
   {
      QO_SGW_DBG_ERR(("Downlink Data Notif Ack State Handling"));
   }

   EgUtilDeAllocGmMsg(&egMsg);
   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwInDDNAckStateHndlr
 *
 *       Desc:  Handles Downlink Data notif ack received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInDDNAckStateHndlr
(
QoSgwUeCb         *ueCb,      /* UE Control block (INOUT) */
EgMsg             *egMsg      /* gtp-c message (IN)   */
)
#else
PRIVATE S16 qoSgwInDDNAckStateHndlr(ueCb, egMsg)
QoSgwUeCb         *ueCb;      /* UE Control block (INOUT) */
EgMsg             *egMsg;     /* gtp-c message (IN)   */
#endif
{
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwInDDNAckStateHndlr);

   switch (ueCb->state)
   {
      case QO_SGW_UE_DISCONNECTED:
      {
         ret = qoSgwValDDNAck(ueCb, egMsg);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Validation of Downlink Data Notif Ack"));
            ret = ROK;
         }
         else
         {
            QO_SGW_DBG_INFO(("Validation of DL Data Notif Ack success"));
         }

         break;
      }

      default:
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Downlink data Notif Ack received in Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInDDNFInd
 *
 *       Desc:  Handles Downlink Data Notif Failure Ind received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDDNFInd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDDNFInd(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   CmLListCp      *lnkLst = NULLP;
   CmLList        *tmp = NULLP;
   QoEgtUEvntInfo *egtEvntInfo = NULLP;  
   EgtUEvnt       *egtUEvnt = NULLP;


   QO_SGW_TRC2(qoSgwInDDNFInd);
   QO_SGW_DBG_INFO(("Received DL Data Notif Failure Indication"));
   
   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find s11 Tunnel For DL Data Notif Failure Ind"));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received DL Data Notif Failure Ind From MME Tunnel %ld",
                  s11Tun->locTeid)); 
   
   /* Remove buffered data and set buffering state to NO Buffering, so
    * that when new pkt recvd, DDN will be sent to MME */
   lnkLst = &(s11Tun->ueCb->pendingOnS1);
   CM_LLIST_FIRST_NODE(lnkLst, tmp);
   while(tmp)
   {
      egtEvntInfo = (QoEgtUEvntInfo *)tmp->node;
      egtUEvnt = egtEvntInfo->egtUEvnt;

      EgUUtilDeAllocGmMsg(&(egtUEvnt->u.egMsg));
      SPutSBuf(qoSgwCb.egUSap->pst.region, qoSgwCb.egUSap->pst.pool,
            (Data *)egtUEvnt, (Size)sizeof(EgtUEvnt));

      /* Delete the buffered packet from the Linked List */
      cmLListDelFrm(&(s11Tun->ueCb->pendingOnS1), tmp);
      (Void) SPutSBuf(DFLT_REGION, DFLT_POOL,
                (U8*)egtEvntInfo, sizeof(QoEgtUEvntInfo));
    
     CM_LLIST_NEXT_NODE(lnkLst, tmp); 
   } /* end of while loop */

   s11Tun->ueCb->bufferingState = QO_SGW_NO_BUFFERING;
   
   EgUtilDeAllocGmMsg(&egMsg);

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwInDSReq
 *
 *       Desc:  Handle Delete Session Request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDSReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDSReq(suId, localAddr, remAddr, lclTeid, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16            ret = ROK;
   QoSgwTunnelCb  *sTun = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;
   U32            cause = 0;

   QO_SGW_TRC2(qoSgwInDSReq)

   sTun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   
   if(sTun != NULLP)
   {
      QO_SGW_DBG_INFO(("Received Delete Session Request from MME For "
               "Tunnel %ld", sTun->locTeid)); 
   }
   else
   {
      sTun = qoSgwFindTunnel(qoSgwCb.sgsnCSap, lclTeid.val);
      if(sTun != NULLP)
      {
         QO_SGW_DBG_INFO(("Received Delete Session Request from SGSN For "
                  "Tunnel %ld", sTun->locTeid)); 
      }
      else
      {
         QO_SGW_DBG_ERR(("Tunnel Not Found For Delete Session Request"));
         EgUtilDeAllocGmMsg(&egMsg);
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   ueCb = sTun->ueCb; 

   ret = qoSgwGetPdnCbUsingEgMsg(ueCb, egMsg, &pdnCb);
   if (ROK == ret)
   {
      ret = qoSgwInDSReqStateHndlr(pdnCb, egMsg);
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Processing of Delete Session Request, SUCCESS"));
      }
      else
      {
         cause = EGT_GTPC_CAUSE_SYS_FAIL;
         QO_SGW_DBG_ERR(("Processing of Delete Session Request"));
      }
   }
   else
   {
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }

   if (ROK != ret)
   {
      /* Sending failure response to MME */
      if(suId == qoSgwCb.mmeCSap->suId)
      {
         QO_SGW_DBG_INFO(("Sending Delete Session Response with Failure cause "
                     "(%d) to MME", cause));
         qoSgwEgtUtlSndFailureRspToMme(sTun->locTeid, sTun->remTeid,\
            EGT_GTPC_MSG_DS_RSP, egMsg->msgHdr.seqNumber, cause);
      }
      /* Sending failure response to SGSN */
      if(suId == qoSgwCb.sgsnCSap->suId)
      {
         QO_SGW_DBG_INFO(("Sending Delete Session Response with Failure cause "
                     "(%d) to SGSN", cause));
         qoSgwEgtUtlSndFailureRspToSgsn(sTun->locTeid, sTun->remTeid,\
            EGT_GTPC_MSG_DS_RSP, egMsg->msgHdr.seqNumber, cause);
      }  
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwInDSReqStateHndlr
 *
 *       Desc:  Handles Modify Bearer request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInDSReqStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control block (INOUT) */
EgMsg             *egMsg      /* gtp-c message (IN)   */
)
#else
PRIVATE S16 qoSgwInDSReqStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control block (INOUT) */
EgMsg             *egMsg;     /* gtp-c message (IN)   */
#endif
{
   S16               ret = ROK;
   U32               cause = 0;

   QO_SGW_TRC2(qoSgwInDSReqStateHndlr);

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CREATE:
      case QO_SGW_PDN_MODIFY:
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValDSReq(pdnCb, egMsg, &cause);
         if (ROK == ret)
         {
            if(pdnCb->ueCb->intf == QO_IF_S11_C )
              pdnCb->ueCb->s11Tun->transId.val = egMsg->msgHdr.seqNumber;
            else if(pdnCb->ueCb->intf == QO_IF_S4_C )
              pdnCb->ueCb->s4Tun->transId.val = egMsg->msgHdr.seqNumber;
            if (pdnCb->fwdDSReq == TRUE)
            {
               pdnCb->state = QO_SGW_PDN_DETACH;
               qoSgwOutDSReq(pdnCb, egMsg);
            }
            else
            {
               qoSgwOutDSRsp(pdnCb, egMsg);
               qoSgwDeletePdn(pdnCb, QO_SGW_SIGNAL_NO);
            }
         }
         else
         {
            if(pdnCb->ueCb->intf == QO_IF_S11_C )
            {
               qoSgwEgtUtlSndFailureRspToMme(pdnCb->ueCb->s11Tun->locTeid,\
                  pdnCb->ueCb->s11Tun->remTeid, EGT_GTPC_MSG_DS_RSP,\
                  egMsg->msgHdr.seqNumber, cause);
            }
            else if(pdnCb->ueCb->intf == QO_IF_S4_C )
            {
               qoSgwEgtUtlSndFailureRspToSgsn(pdnCb->ueCb->s4Tun->locTeid,\
                  pdnCb->ueCb->s4Tun->remTeid, EGT_GTPC_MSG_DS_RSP,\
                  egMsg->msgHdr.seqNumber, cause);
            }
            QO_SGW_DBG_ERR(("Validation of Delete session Request"));
            ret = ROK;
         }

         break;
      }

      default:
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Delete Session Request received in Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwOutDSReq
 *
 *       Desc:  Sends Delete Session Request to PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE Void qoSgwOutDSReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE Void qoSgwOutDSReq(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   TknU8          create = {0};
   QoSgwTunnelCb  *s5s8CTun = NULLP;
   TknU32         transId = {0};

   QO_SGW_TRC2(qoSgwOutDSReq)
   s5s8CTun = pdnCb->s5s8CTun;

   egMsg->msgHdr.teidPres = PRSNT_NODEF;
   egMsg->msgHdr.teid = s5s8CTun->remTeid;
   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = s5s8CTun->locTeid;
   create.pres = PRSNT_NODEF;
   create.val = FALSE;
   transId.val = s5s8CTun->locTeid;
   transId.pres = PRSNT_NODEF;

   s5s8CTun->currEvent = QO_SGW_EGT_EVT_DS_REQ_SENT;
   QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                  s5s8CTun->locAddr, s5s8CTun->remAddr, 
                  teidTkn, transId, create, qoSgwCb.t3, egMsg);

   QO_SGW_RETVOID;
}


/***************************************************************
 *
 *       Fun:   qoSgwOutBRCmd
 *
 *       Desc:  Handles Bearer Resource Command received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE Void qoSgwOutBRCmd
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE Void qoSgwOutBRCmd(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   TknU8          create = {PRSNT_NODEF, FALSE};
   QoSgwTunnelCb  *s5s8Tun = NULLP;

   QO_SGW_TRC2(qoSgwOutBRCmd)
   s5s8Tun = pdnCb->s5s8CTun;

   egMsg->msgHdr.teidPres = PRSNT_NODEF;
   egMsg->msgHdr.teid = s5s8Tun->remTeid;
   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = s5s8Tun->locTeid;

   QO_SGW_DBG_INFO(("Sending Of Bearer Resource Command To PGW"));
   QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                        s5s8Tun->locAddr, s5s8Tun->remAddr, 
                        teidTkn, s5s8Tun->transId, create, qoSgwCb.t3, egMsg);

   QO_SGW_RETVOID;
}

/***************************************************************
 *
 *       Fun:   qoSgwOutDBCmd
 *
 *       Desc:  Handles Bearer Resource Command received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE Void qoSgwOutDBCmd
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE Void qoSgwOutDBCmd(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   TknU8          create = {PRSNT_NODEF, FALSE};
   QoSgwTunnelCb  *s5s8Tun = NULLP;
   TknU32         transId = {0};

   QO_SGW_TRC2(qoSgwOutDBCmd)
   s5s8Tun = pdnCb->s5s8CTun;

   egMsg->msgHdr.teidPres = PRSNT_NODEF;
   egMsg->msgHdr.teid = s5s8Tun->remTeid;
   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = s5s8Tun->locTeid;
   transId.val = s5s8Tun->locTeid;
   transId.pres = PRSNT_NODEF;

   QO_SGW_DBG_INFO(("Sending Of Delete Bearer Command To PGW"));
   QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                        s5s8Tun->locAddr, s5s8Tun->remAddr, 
                        teidTkn, transId, create, qoSgwCb.t3, egMsg);

   s5s8Tun->currEvent = QO_SGW_EGT_EVT_DB_CMD_SENT;
   QO_SGW_RETVOID;
}


/***************************************************************
 *
 *       Fun:   qoSgwOutMBCmd
 *
 *       Desc:  Handles Bearer Resource Command received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE Void qoSgwOutMBCmd
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE Void qoSgwOutMBCmd(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   TknU8          create = {PRSNT_NODEF, FALSE};
   QoSgwTunnelCb  *s5s8Tun = NULLP;
   TknU32         transId = {0};

   QO_SGW_TRC2(qoSgwOutMBCmd)
   s5s8Tun = pdnCb->s5s8CTun;

   egMsg->msgHdr.teidPres = PRSNT_NODEF;
   egMsg->msgHdr.teid = s5s8Tun->remTeid;
   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = s5s8Tun->locTeid;
   transId.val = s5s8Tun->locTeid;
   transId.pres = PRSNT_NODEF;

   QO_SGW_DBG_INFO(("Sending Of Modify Bearer Command To PGW"));
   QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                        s5s8Tun->locAddr, s5s8Tun->remAddr, 
                        teidTkn, transId, create, qoSgwCb.t3, egMsg);

   s5s8Tun->currEvent = QO_SGW_EGT_EVT_MB_CMD_SENT;
   QO_SGW_RETVOID;
}

/***************************************************************
 *
 *       Fun:   qoSgwInMBRsp
 *
 *       Desc:  Handle Modify Bearer Response received from PGW 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInMBRsp
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInMBRsp(suId, localAddr, remAddr, lclTeid, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInMBRsp)

   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      s5s8CTun = qoSgwFindTunnelTrans(qoSgwCb.pgwCSap, transId.val);
      if(s5s8CTun == NULLP)
      {
         QO_SGW_DBG_ERR(("Tunnel Not Found For Modify Bearer Response"));
         QO_SGW_RETVALUE(RFAILED);
      }
      else
      {
         s5s8CTun->locTeid = lclTeid.val;

         /* add in TEID hash list remove from transId hash list */
         QO_SGW_DBG_INFO(("Removing tunnel CB from Transaction hash to "\
               "Teid Hash table"));
         ret = qoSgwTransToTeidHashList(qoSgwCb.pgwCSap, s5s8CTun);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Removing tunnel CB from Transaction hash to "\
                  "Teid Hash table"));
            RETVALUE(RFAILED);
         }

         s5s8CTun->transId.pres = NOTPRSNT;
      }
   }

   QO_SGW_DBG_INFO(("Received Modify Bearer Response From PDN-GW For "\
         "Tunnel %ld\n", s5s8CTun->locTeid)); 

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (pdnCb != NULLP)
   {
      ret = qoSgwInMBRspStateHndlr(pdnCb, egMsg);
      if (ROK == ret)
      {
         pdnCb->state = QO_SGW_PDN_CONNECTED;
         QO_SGW_DBG_INFO(("Processing of Modify Bearer Response, SUCCESS"));
      }
      else
      {
         QO_SGW_DBG_ERR(("Processing of Modify Bearer Response"));
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwInDSRsp
 *
 *       Desc:  Handle Delete Session Response received from PGW 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDSRsp
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDSRsp(suId, localAddr, remAddr, lclTeid, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInDSRsp)

   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel Not Found For Delete Session Response."));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Delete Session Response From PDN-GW For "
                  "Tunnel %ld\n", s5s8CTun->locTeid)); 

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (pdnCb != NULLP)
   {
      ret = qoSgwInDSRspStateHndlr(pdnCb, egMsg);
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Processing of Delete Session Response, SUCCESS"));
      }
      else
      {
         QO_SGW_DBG_ERR(("Processing of Delete Session Response"));
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwInMBRspStateHndlr
 *
 *       Desc:  Handle Modify Bearer Response received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInMBRspStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInMBRspStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   S16               ret = ROK;
   U32               cause = 0;
   Bool              fwd = FALSE;
   QoSgwBearerCb     *defBearer = NULLP;

   QO_SGW_TRC2(qoSgwInMBRspStateHndlr);

   QO_SGW_GET_BEARER(pdnCb->ueCb->bearerTable, pdnCb->s5s8CTun->ebi, defBearer);
   if (NULLP == defBearer)
   {
      QO_SGW_DBG_ERR(("Default Bearer CB Not found"));
      QO_SGW_RETVALUE(RFAILED);
   }

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_MODIFY:
      case QO_SGW_PDN_CREATE:
      {
         ret = qoSgwValMBRsp(pdnCb, egMsg, &cause, &fwd);
         if (ROK == ret)
         {
            ret = qoSgwEguTunnelAddReq(defBearer, NULLP,\
                  EGT_INT_S5_S8_SGW_GTP_U);
            if (ROK == ret)
            {
               QO_SGW_DBG_INFO(("Sending Modify Bearer Response to MME"));
               pdnCb->ueCb->state = QO_SGW_UE_CONNECTED;
               qoSgwOutMBRsp(pdnCb, egMsg);
            }
            else
            {
               QO_SGW_DBG_ERR(("Sending Tunnel Add Request to EGT Layer"));
            }
         }
         else
         {
            ret = ROK;
            if (TRUE == fwd)
            {
               qoSgwEgtUtlFwdFailureRsp(pdnCb->ueCb->s11Tun, QO_ENT_MME, egMsg);
            }
            else
            {
               qoSgwEgtUtlSndFailureRsp(pdnCb->ueCb->s11Tun->locTeid,\
                     EGT_GTPC_MSG_MB_RSP, egMsg->msgHdr.seqNumber, cause,\
                     QO_ENT_MME);
               QO_SGW_DBG_ERR(("Validation of Modify Bearer Response"));
            }

            ret = ROK;
         }

         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Modify Bearer Response Recieved In Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwInDSRspStateHndlr
 *
 *       Desc:  Handle Delete Session Response received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInDSRspStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInDSRspStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwInDSRspStateHndlr);

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_DETACH:
      {
         if(pdnCb->ueCb->intf == QO_IF_S11_C )
           QO_SGW_DBG_INFO(("Sending Delete Session Response to MME"));
         else if(pdnCb->ueCb->intf == QO_IF_S4_C )
           QO_SGW_DBG_INFO(("Sending Delete Session Response to SGSN"));
         qoSgwOutDSRsp(pdnCb, egMsg);

         qoSgwDeletePdn(pdnCb, QO_SGW_SIGNAL_NO);
         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Delete Session Response Recieved In Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwOutDSRsp
 *
 *       Desc:  Sends Delete Session Response towards MME 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE Void qoSgwOutDSRsp
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE Void qoSgwOutDSRsp(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   QoSgwTunnelCb  *sTun = NULLP;
   EgMsg          *pEgMsg = NULLP;
   S16            ret     = ROK;

   QO_SGW_TRC2(qoSgwOutDSRsp)

   if(pdnCb->ueCb->intf == QO_IF_S11_C )
   {
      sTun = pdnCb->ueCb->s11Tun;
      QO_SGW_DBG_INFO(("Sending Delete Session Response To MME For "
                     "Tunnel %ld\n", sTun->locTeid)); 
   }
   else if(pdnCb->ueCb->intf == QO_IF_S4_C )
   {
     sTun = pdnCb->ueCb->s4Tun;
     QO_SGW_DBG_INFO(("Sending Delete Session Response To SGSN For "
                     "Tunnel %ld\n", sTun->locTeid)); 
   }
   if(sTun != NULLP)
   {
     qoSgwBldDSRsp(pdnCb, &pEgMsg);
     if (ROK == ret)
     {
        pEgMsg->msgHdr.teidPres = PRSNT_NODEF;
        pEgMsg->msgHdr.teid = sTun->remTeid;
        teidTkn.pres = PRSNT_NODEF;
        teidTkn.val = sTun->locTeid;

        if(pdnCb->ueCb->intf == QO_IF_S11_C )
          QoSgwLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                     teidTkn, pEgMsg);
        else if(pdnCb->ueCb->intf == QO_IF_S4_C )
          QoSgwLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId,
                     teidTkn, pEgMsg);

      }
      else
      {
         if(pdnCb->ueCb->intf == QO_IF_S11_C )
         {     
            qoSgwEgtUtlSndFailureRspToMme(sTun->locTeid, sTun->remTeid,\
            EGT_GTPC_MSG_DS_RSP, egMsg->msgHdr.seqNumber,\
            EGT_GTPC_CAUSE_SYS_FAIL);
         }
         if(pdnCb->ueCb->intf == QO_IF_S4_C )
         {     
            qoSgwEgtUtlSndFailureRspToSgsn(sTun->locTeid, sTun->remTeid,\
            EGT_GTPC_MSG_DS_RSP, egMsg->msgHdr.seqNumber,\
            EGT_GTPC_CAUSE_SYS_FAIL);
         }
      }
    }

   QO_SGW_RETVOID;
}


#ifdef ANSI
PRIVATE S16 qoSgwSendFailureResponse
     (
      EgMsg *msg,
      QoSgwEgtSapCb *sap,
      U32 teid,
      QoSgwTunnelCb *tunnel,
      U8 cause,
      U8 respType
      )
#else
PRIVATE S16 qoSgwSendFailureResponse(msg,sap,teid,tunnel,cause,respType)
      EgMsg *msg;
      QoSgwEgtSapCb *sap;
      U32 teid;
      QoSgwTunnelCb *tunnel;
      U8 cause;
      U8 respType;
#endif
{
   S16 ret = ROK;
   TknU32  teidTkn;
   EgIeLevelInfo ieLvlInfo;
   EgIe *egIe = NULLP;
   EgIe locIE;
   U16 iePos = 0;
   EgMsg *rspMsg = NULLP;
   Mem            memInfo;

   QO_SGW_TRC2(qoSgwSendFailureResponse)
   /*Klocworks fix : initialising teidTkn */
   cmMemset((U8 *)&teidTkn,0,sizeof(TknU32));
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   teidTkn.pres = NOTPRSNT;
   cmMemset((U8 *)&locIE, 0, sizeof(EgIe));

   ieLvlInfo.level = 0;
   ieLvlInfo.ieInfo[0].ieTypePres= TRUE;
   ieLvlInfo.ieInfo[0].ieType= EG_CAUSE_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst= 0;

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   if( qoUtlAllocEgMsg(&rspMsg, &memInfo) != ROK )
   {
      QO_SGW_RETVALUE(RFAILED);
   }
   /* Get Cause, result in the response */
   ret = EgUtilGmGetIe(msg, &ieLvlInfo, &egIe, &iePos);
   if(ret != ROK)
   {
#ifndef EG_REL_930
      qoUtlEgtBuildEgIeHeader(&locIE, EG_CAUSE_IETYPE, 0, 0,
            EG_UNSIGNED_8);
#else
      qoUtlEgtBuildEgIeHeader(&locIE, EG_CAUSE_IETYPE, 0,EG_UNSIGNED_8);
#endif /* EG_REL_930 */
      locIE.t.valUnSgnd8 = cause;
      ret = EgUtilGmAppendIe(rspMsg, &ieLvlInfo, &locIE);
   }
   else
   {
      egIe->t.valUnSgnd8 = cause;
      ret = EgUtilGmAppendIe(rspMsg, &ieLvlInfo, egIe);
   }
   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = teid;
   if(tunnel != NULLP)
   {
      rspMsg->msgHdr.teidPres = PRSNT_NODEF;
      rspMsg->msgHdr.teid = tunnel->remTeid;
   }
   else
   {
      rspMsg->msgHdr.teidPres = NOTPRSNT;
   }
   rspMsg->msgHdr.msgType = respType;
   rspMsg->msgHdr.version = 2;

#ifdef QO_TODO_SECOND_PHASE
   /* If error in on S11 interface, send Delete Session Request on S5S8 */
#endif
   ret = QoSgwLiEgtSigRsp ( &(sap->pst), sap->spId, teidTkn, rspMsg);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Failed to send error Response = %d, with cause =%d.\n",
               respType, cause));
   }
   EgUtilDeAllocGmMsg(&rspMsg);
   QO_SGW_RETVALUE(ret);
}


#if 0
/***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwSendGtpUErrInd
    (
      EgtUEvnt *data, 
      QoSgwTunnelCb *dataTun,
      QoSgwEgtSapCb *sap,
      U8 cause
    )
#else
PRIVATE S16 qoSgwSendGtpUErrInd(data,dataTun,sap,cause)
      EgtUEvnt *data;
      QoSgwTunnelCb *dataTun;
      QoSgwEgtSapCb *sap;
      U8 cause;
#endif
{
   S16 ret = ROK;
#ifndef QO_TODO_SECOND_PHASE
   EgtUEvnt *eguErrInd;
   Mem   memInfo;
   EgUIe  eguIe;

   QO_SGW_TRC2(qoSgwSendGtpUErrInd)

   QO_SGW_ALLOC((&eguErrInd), sizeof(EgtUEvnt));
   /* klockworks fix : checking for NULLP for the allocated variable*/
   if(eguErrInd == NULLP)
   {
      QO_SGW_DBG_ERR(("Failed to get memory for EGTP-U Error Indication \
               .\n"));

      QO_SGW_RETVALUE(RFAILED);
   }   
   
   cmMemset((U8 *)&eguIe, 0 , sizeof(EgUIe));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool = qoSgwCb.init.pool;
   ret = EgUUtilAllocGmMsg(&(eguErrInd->u.egMsg), &(memInfo));
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Failed to create event for EGTP-U Error Indication \
               message.\n"));

      QO_SGW_RETVALUE(ret);
   }
   /* fill the message */
   eguErrInd->event = EGTU_EVENT_TYPE_MSG;
   /* klockworks fix :  removing & for pointer variable*/
   cmMemcpy((U8 *)&(eguErrInd->u.egMsg->srcAddr), (U8 *)(dataTun->locAddr),
         sizeof(CmTptAddr));

   /* klockworks fix :  removing & for pointer variable*/
   cmMemcpy((U8 *)&(eguErrInd->u.egMsg->remAddr), (U8 *)(dataTun->remAddr),
         sizeof(CmTptAddr));

   eguErrInd->u.egMsg->lclTeid = dataTun->locTeid;
   eguErrInd->u.egMsg->msgHdr.nPduNmb.pres = FALSE;
   eguErrInd->u.egMsg->msgHdr.seqNumber.pres =
      data->u.egMsg->msgHdr.seqNumber.pres;

   eguErrInd->u.egMsg->msgHdr.seqNumber.val =
      data->u.egMsg->msgHdr.seqNumber.val;

   eguErrInd->u.egMsg->msgHdr.extHdr.udpPort.pres = FALSE;
   eguErrInd->u.egMsg->msgHdr.extHdr.pdcpNmb.pres = FALSE;
   eguErrInd->u.egMsg->msgHdr.teId = dataTun->remTeid;
   eguErrInd->u.egMsg->msgHdr.msgType = EGT_GTPU_MSG_ERROR_IND;

   /* Add 2 manadatory IEs */
   eguIe.ieType = EGU_TEID_IETYPE;
   eguIe.dataType = EG_UNSIGNED_32;
   eguIe.t.valUnSgnd32 = data->u.egMsg->msgHdr.teId;
   ret = EgUUtilGmAppendIe(eguErrInd->u.egMsg, &eguIe);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Failed to add IE EGTP-U Error Indication message.\n"));
      EgUUtilDeAllocGmMsg(&(eguErrInd->u.egMsg));
      EgUUtilDeAllocGmMsg(&(data->u.egMsg));
      QO_SGW_RETVALUE(ret);
   }

   eguIe.ieType = EGU_GSN_ADDR_IETYPE;
   eguIe.dataType = EG_IP_ADDR;
   eguIe.t.ipAddr.addrType = CM_IPV4ADDR_TYPE;
   cmMemcpy((U8 *)&(eguIe.t.ipAddr.u.ipv4), (U8
            *)&(data->u.egMsg->remAddr.u.ipv4TptAddr),

         sizeof(CmIpAddr));
   ret = EgUUtilGmAppendIe(eguErrInd->u.egMsg, &eguIe);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Failed to add IE EGTP-U Error Indication message.\n"));
      EgUUtilDeAllocGmMsg(&(eguErrInd->u.egMsg));
      EgUUtilDeAllocGmMsg(&(data->u.egMsg));
      QO_SGW_RETVALUE(ret);
   }
   /* Send to Peer*/
   ret = QoSgwLiEgtEguDatReq(&(sap->pst), sap->spId, eguErrInd);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Failed to send EGTP-U Error Indication message.\n"));
   }
   /* Free Events */
  /* EgUUtilDeAllocGmMsg(&(eguErrInd.u.egMsg));*/
   /*EgUUtilDeAllocGmMsg(&(data->u.egMsg));*/
   QO_SGW_FREE(data, sizeof(EgtUEvnt));
#endif
   QO_SGW_RETVALUE(ret);
}
#endif

/***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwCreateNSendDeleteSessReq
(
QoSgwTunnelCb *tunnel, 
U8 cause
)
#else
PRIVATE S16 qoSgwCreateNSendDeleteSessReq(tunnel,cause)
QoSgwTunnelCb *tunnel;
U8 cause;
#endif
{
   S16 ret = ROK;
   TknU32 teidTkn;
   TknU8 create;
   EgMsg *egMsg = NULLP;

   QO_SGW_TRC2(qoSgwCreateNSendDeleteSessReq)

   cmMemset((U8 *)&create, 0, sizeof(TknU8));
   /*Klocworks fix : initialising teidTkn */
   cmMemset((U8 *)&teidTkn,0,sizeof(TknU32));
      /* Build Delete Session Request and send 
       * Use state DISCONNECT appropriately*/
   ret = qoSgwUtlBldEgtDelSsnReq(tunnel, &egMsg, cause);
   if(ret != ROK)
   {
      QO_SGW_RETVALUE(RFAILED);
   }
   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = tunnel->locTeid;
   create.pres=PRSNT_NODEF;
   create.val = FALSE;
   ret = QoSgwLiEgtSigReq ( &(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
         tunnel->locAddr, tunnel->remAddr, teidTkn, 
         tunnel->transId, create, qoSgwCb.t3, egMsg);
   if(ret == ROK)
   {
      tunnel->state = QO_SGW_TUN_DISCONNECTED;
   }
   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInBRCmd
 *
 *       Desc:  Handles Bearer Resource Command received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInBRCmd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInBRCmd(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInBRCmd);
   QO_SGW_DBG_INFO(("Received Bearer Resource Command"));
   
   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find s11 Tunnel For Bearer Resource Command"));
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Bearer Resource Command From MME Tunnel %ld",
                  s11Tun->locTeid)); 

   ret = qoSgwGetPdnCbUsingEgMsg(s11Tun->ueCb, egMsg, &pdnCb);
   if (ROK == ret)
   {
      qoSgwOutBRCmd(pdnCb, egMsg);
   }
   else
   {
      QO_SGW_DBG_ERR(("Finding PDN Control Block"));
   }

   if (ret != ROK)
   {
      QO_SGW_DBG_INFO(("Sending Bearer Resource Failure Ind To MME"));
      qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,\
            EGT_GTPC_MSG_BR_FAIL_IND, egMsg->msgHdr.seqNumber,
            EGT_GTPC_CAUSE_SYS_FAIL);
   }

   QO_SGW_RETVALUE(ret);
}

/*****************************************************************
 *
 *       Fun:   qoSgwInDBCmd
 *
 *       Desc:  Handles Bearer Resource Command received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDBCmd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDBCmd(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInDBCmd);
   QO_SGW_DBG_INFO(("Received Delete Bearer Command"));
   
   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find s11 Tunnel For Delete Bearer Command"));
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Delete Command From MME Tunnel %ld",
                  s11Tun->locTeid)); 

   ret = qoSgwGetPdnCbUsingEgMsg(s11Tun->ueCb, egMsg, &pdnCb);
   if (ROK == ret)
   {
      qoSgwOutDBCmd(pdnCb, egMsg);
   }
   else
   {
      QO_SGW_DBG_ERR(("Finding PDN Control Block"));
   }

   if (ret != ROK)
   {
      QO_SGW_DBG_INFO(("Sending Delete Bearer Failure Ind To MME"));
      qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,\
            EGT_GTPC_MSG_DB_FAIL_IN, egMsg->msgHdr.seqNumber,\
            EGT_GTPC_CAUSE_SYS_FAIL);
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInMBCmd
 *
 *       Desc:  Handles Modify Bearer Command received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInMBCmd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInMBCmd(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwPdnCb     *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInMBCmd);
   QO_SGW_DBG_INFO(("Received Modify Bearer Command"));
   
   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find s11 Tunnel For Modify Bearer Command"));
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Modify Command From MME Tunnel %ld",
                  s11Tun->locTeid)); 

   ret = qoSgwGetPdnCbUsingEgMsg(s11Tun->ueCb, egMsg, &pdnCb);
   if (ROK == ret)
   {
      qoSgwOutMBCmd(pdnCb, egMsg);
   }
   else
   {
      QO_SGW_DBG_ERR(("Finding PDN Control Block"));
   }

   if (ret != ROK)
   {
      QO_SGW_DBG_INFO(("Sending Modify Bearer Failure Ind To MME"));
      qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,\
            EGT_GTPC_MSG_MBFAIL_IND, egMsg->msgHdr.seqNumber,\
            EGT_GTPC_CAUSE_SYS_FAIL);
   }

   QO_SGW_RETVALUE(ret);
}

/*****************************************************************
 *
 *       Fun:   qoSgwInCBReq
 *
 *       Desc:  Handles Create Bearer Request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInCBReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInCBReq(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;
   U32               cause = 0;

   QO_SGW_TRC2(qoSgwInCBReq)
   QO_SGW_DBG_INFO(("Received Create Bearer Request"));
   
   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find Tunnel For Create Bearer Request, FAILED"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_CB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_REJ_CXT_NON_EXISTENT,
                              QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if(NULLP == pdnCb)
    {
      QO_SGW_DBG_ERR(("Memory Allocation For Temporary PDN Data"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_CB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_NO_RES_AVAIL, QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }
   QO_SGW_DBG_INFO(("Received Create Bearer Request From PGW Tunnel %ld",
                  s5s8CTun->locTeid)); 

   QO_SGW_ALLOC(&(pdnCb->tmpPdnData), sizeof(QoSgwPdnTmpData));
   if (NULLP == pdnCb->tmpPdnData)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For Temporary PDN Data"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_CB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_NO_RES_AVAIL, QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   cmLListInit(&(pdnCb->tmpPdnData)->bearers);
   ret = qoSgwInCBReqStateHndlr(pdnCb, egMsg);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Create Bearer Request State Handler, FAILED"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }
   else
   {
      QO_SGW_DBG_INFO(("Create Bearer Request State Handler, SUCCESS"));
   }

   /* sending failrue response to PGW */
   if (ret != ROK)
   {
      QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_CB_RSP,\
            egMsg->msgHdr.seqNumber, cause, QO_ENT_PGW);
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwInCBReqStateHndlr
 *
 *       Desc:  Handle Create bearer Request received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInCBReqStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInCBReqStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   S16      ret = RFAILED;
   U32      cause = 0;

   QO_SGW_TRC2(qoSgwInCBReqStateHndlr);

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValCBReq(pdnCb, egMsg, &cause);
         if (ROK == ret)
         {
            ret = qoSgwOutCBReq(pdnCb, egMsg);
            if (ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending The Create Bearer Request, FAILED"));
            }
         }
         else
         {
            QO_SGW_DBG_ERR(("Validating Create Session Request"));
            qoSgwEgtUtlSndFailureRsp(pdnCb->s5s8CTun->locTeid,
                                    EGT_GTPC_MSG_CB_RSP,
                                    egMsg->msgHdr.seqNumber, cause,
                                    QO_ENT_PGW);
            ret = ROK;
         }

         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Create Bearer Request Recieved In Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwOutCBReq
 *
 *       Desc:  Sends Create bearer Request to MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutCBReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutCBReq(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   TknU8          create = {0};
   U16            ret = ROK;
   QoSgwTunnelCb  *s1UTun = NULLP;
   QoSgwTunnelCb  *s11Tun = NULLP;
   CmLListCp      *bearerLst = NULLP;
   CmLList        *tmp = NULLP;
   TknU32         teidTkn;
   TknU32         transId;

   QO_SGW_TRC2(qoSgwOutCBReq)

   /* Create SGW S1-U dedicated tunnels */
   bearerLst = &(pdnCb->tmpPdnData->bearers);
   CM_LLIST_FIRST_NODE(bearerLst, tmp);
   while (tmp)
   {
      ret = qoSgwCreateUTunnel(pdnCb, &s1UTun, QO_IF_S1U_DED);
      if (ROK == ret && s1UTun != NULLP)
      {
         ((QoSgwBearerNode *)tmp)->s1UTun = s1UTun;
         QO_SGW_DBG_INFO(("Creation Of S1-U Tunnel, SUCCESS"));
      }
      else
      {
         ret = RFAILED;
         break;
      }
    
      CM_LLIST_NEXT_NODE(bearerLst, tmp); 
   } /* while */

   if (ROK == ret)
   {
      ret = qoSgwBldCBReq(pdnCb, egMsg);
      if (ROK == ret)
      {
         /* Send out on GTP-C */
         s11Tun = pdnCb->ueCb->s11Tun;
         egMsg->msgHdr.teidPres = PRSNT_NODEF;
         egMsg->msgHdr.teid = s11Tun->remTeid;
         teidTkn.pres = PRSNT_NODEF;
         teidTkn.val = s11Tun->locTeid;
         create.pres = PRSNT_NODEF;
         create.val = FALSE;

         qoSgwGenerateCTransId(&(pdnCb->transId));
         transId.pres = PRSNT_NODEF;
         transId.val = pdnCb->transId;
         s11Tun->transId.val = pdnCb->transId;

         QO_SGW_DBG_INFO(("Seding Of Create Bearer Request To MME"));
         QoSgwLiEgtSigReq(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                         s11Tun->locAddr, s11Tun->remAddr, teidTkn, 
                         transId, create, qoSgwCb.t3, egMsg);
      }
      else
      {
         QO_SGW_DBG_ERR(("Building Of Create Bearer Request"));
      }
   }
   else
   {
      QO_SGW_DBG_ERR(("S1-U Tunnel Creation"));
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwOutDBReq
 *
 *       Desc:  Sends Delete bearer Request to MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutDBReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutDBReq(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   TknU8          create = {0};
   U16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   TknU32         teidTkn;
   TknU32         transId;

   QO_SGW_TRC2(qoSgwOutDBReq)

   ret = qoSgwBldDBReq(pdnCb, egMsg);
   if (ROK == ret)
   {
      /* Send out on GTP-C */
      s11Tun = pdnCb->ueCb->s11Tun;
      egMsg->msgHdr.teidPres = PRSNT_NODEF;
      egMsg->msgHdr.teid = s11Tun->remTeid;
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = s11Tun->locTeid;
      create.pres = PRSNT_NODEF;
      create.val = FALSE;

      qoSgwGenerateCTransId(&(pdnCb->transId));
      transId.pres = PRSNT_DEF;
      transId.val = pdnCb->transId;
      s11Tun->transId.val = pdnCb->transId;

      QO_SGW_DBG_INFO(("Seding Of Delete Bearer Request To MME"));
      QoSgwLiEgtSigReq(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                      s11Tun->locAddr, s11Tun->remAddr, teidTkn, 
                      transId, create, qoSgwCb.t3, egMsg);
   }
   else
   {
      QO_SGW_DBG_ERR(("Building Of Delete Bearer Request"));
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwOutUBReq
 *
 *       Desc:  Sends Update bearer Request to MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutUBReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutUBReq(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   TknU8          create = {0};
   U16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   TknU32         teidTkn;
   TknU32         transId;

   QO_SGW_TRC2(qoSgwOutUBReq)

   ret = qoSgwBldUBReq(pdnCb, egMsg);
   if (ROK == ret)
   {
      /* Send out on GTP-C */
      s11Tun = pdnCb->ueCb->s11Tun;
      egMsg->msgHdr.teidPres = PRSNT_NODEF;
      egMsg->msgHdr.teid = s11Tun->remTeid;
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = s11Tun->locTeid;
      create.pres = PRSNT_NODEF;
      create.val = FALSE;

      qoSgwGenerateCTransId(&(pdnCb->transId));
      transId.pres = PRSNT_DEF;
      transId.val = pdnCb->transId;
      s11Tun->transId.val = pdnCb->transId;

      QO_SGW_DBG_INFO(("Seding Of Update Bearer Request To MME"));
      QoSgwLiEgtSigReq(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId,
                      s11Tun->locAddr, s11Tun->remAddr, teidTkn, 
                      transId, create, qoSgwCb.t3, egMsg);
   }
   else
   {
      QO_SGW_DBG_ERR(("Building Of Update Bearer Request"));
   }

   QO_SGW_RETVALUE(ret);
}

/*****************************************************************
 *
 *       Fun:   qoSgwInCBRsp
 *
 *       Desc:  Handles Create Bearer Response received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInCBRsp
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInCBRsp(suId, localAddr, remAddr, lclTeid, transId, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s11Tun = NULLP;
   QoSgwPdnCb        *pdnCb = NULL;
   U32               cause = 0;
   QoSgwTunnelCb     *s5s8CTun = NULLP;

   QO_SGW_TRC2(qoSgwInCBRsp)

   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP) 
   {
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_DBG_ERR(("Tunnel not found for Create Bearer Request.\n"));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO (("Received Create Bearer  Response from MME for tunnel "
                     "%ld\n", s11Tun->locTeid)); 

   ret = qoSgwGetPdnCbUsingTmpBearer(s11Tun->ueCb, egMsg, &pdnCb);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Find PDN Control block for transaction (%d), FAILED",
                     transId.val));
      QO_SGW_RETVALUE(ret);
   }

   ret = qoSgwInCBRspStateHndlr(pdnCb, egMsg);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Create Bearer Response State Handling, SUCCESS"));
   }
   else
   {
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
      QO_SGW_DBG_ERR(("Create Bearer Response State Handling"));
   }

   QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
   if (ret == RFAILED)
   {
      s5s8CTun = pdnCb->s5s8CTun;
      qoSgwEgtUtlSndFailureRsp(s5s8CTun->locTeid, EGT_GTPC_MSG_CB_RSP,
                              egMsg->msgHdr.seqNumber, cause,
                              QO_ENT_PGW);
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInDBRsp
 *
 *       Desc:  Handles Delete Bearer Response received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDBRsp
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDBRsp(suId, localAddr, remAddr, lclTeid, transId, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s11Tun = NULLP;
   QoSgwPdnCb        *pdnCb = NULL;
   U32               cause = 0;
   QoSgwTunnelCb     *s5s8CTun = NULLP;

   QO_SGW_TRC2(qoSgwInDBRsp)

   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP) 
   {
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_DBG_ERR(("Tunnel not found for Delete Bearer Request.\n"));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO (("Received Delete Bearer  Response from MME for tunnel "
                     "%ld\n", s11Tun->locTeid)); 

   ret = qoSgwGetPdnCbUsingEgMsg(s11Tun->ueCb, egMsg, &pdnCb);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Find PDN Control block for transaction (%d), FAILED",
                     transId.val));
      QO_SGW_RETVALUE(ret);
   }

   ret = qoSgwInDBRspStateHndlr(pdnCb, egMsg);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Delete Bearer Response State Handling, SUCCESS"));
   }
   else
   {
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
      QO_SGW_DBG_ERR(("Delete Bearer Response State Handling"));
   }

   if (ret == RFAILED)
   {
      s5s8CTun = pdnCb->s5s8CTun;
      qoSgwEgtUtlSndFailureRsp(s5s8CTun->locTeid, EGT_GTPC_MSG_DB_RSP,
                              egMsg->msgHdr.seqNumber, cause,
                              QO_ENT_PGW);
   }

   QO_SGW_RETVALUE(ret);
}

/*****************************************************************
 *
 *       Fun:   qoSgwInUBRsp
 *
 *       Desc:  Handles Update Bearer Response received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInUBRsp
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInUBRsp(suId, localAddr, remAddr, lclTeid, transId, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s11Tun = NULLP;
   QoSgwPdnCb        *pdnCb = NULL;
   U32               cause = 0;
   QoSgwTunnelCb     *s5s8CTun = NULLP;

   QO_SGW_TRC2(qoSgwInUBRsp)

   s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (s11Tun == NULLP) 
   {
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_DBG_ERR(("Tunnel not found for Update Bearer Request.\n"));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO (("Received Update Bearer  Response from MME for tunnel "
                     "%ld\n", s11Tun->locTeid)); 

   ret = qoSgwGetPdnCbUsingEgMsg(s11Tun->ueCb, egMsg, &pdnCb);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Find PDN Control block for transaction (%d), FAILED",
                     transId.val));
      QO_SGW_RETVALUE(ret);
   }

   ret = qoSgwInUBRspStateHndlr(pdnCb, egMsg);
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Update Bearer Response State Handling, SUCCESS"));
   }
   else
   {
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
      QO_SGW_DBG_ERR(("Update Bearer Response State Handling"));
   }

   if (ret == RFAILED)
   {
      s5s8CTun = pdnCb->s5s8CTun;
      qoSgwEgtUtlSndFailureRsp(s5s8CTun->locTeid, EGT_GTPC_MSG_UB_RSP,
                              egMsg->msgHdr.seqNumber, cause,
                              QO_ENT_PGW);
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwInCBRspStateHndlr
 *
 *       Desc:  Handle Create bearer Response received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInCBRspStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInCBRspStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   U32               cause = 0;
   S16               ret = ROK;
   Bool              fwd = FALSE;

   QO_SGW_TRC2(qoSgwInCBRspStateHndlr);
   
   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValCBRsp(pdnCb, egMsg, &cause, &fwd);
         if (ROK == ret)
         {
            ret = qoSgwOutCBRsp(pdnCb, egMsg);
            if (ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending the Create Bearer Response to PGW"));
            }
         }
         else
         {
            if (TRUE == fwd)
            {
               /* MME send Create Bearer Response with failure cause
                * forward as it is to PGW by setting CS bit */
               qoSgwEgtUtlFwdFailureRsp(pdnCb->s5s8CTun, QO_ENT_PGW,
                                       egMsg);
            }
            else
            {
               qoSgwEgtUtlSndFailureRsp(pdnCb->s5s8CTun->locTeid,
                                       EGT_GTPC_MSG_CB_RSP,
                                       egMsg->msgHdr.seqNumber, cause,
                                       QO_ENT_PGW);
               QO_SGW_DBG_ERR(("Validation of Create Bearer Response"));
            }

            ret = ROK;
         }

         break;
      }

      default:
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Create bearer Response recieved in invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwInUBRspStateHndlr
 *
 *       Desc:  Handle Update bearer Response received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInUBRspStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInUBRspStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   U32               cause = 0;
   S16               ret = ROK;
   Bool              fwd = FALSE;

   QO_SGW_TRC2(qoSgwInUBRspStateHndlr);
   
   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValUBRsp(pdnCb, egMsg, &cause, &fwd);
         if (ROK == ret)
         {
            ret = qoSgwOutUBRsp(pdnCb, egMsg);
            if (ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending the Update Bearer Response to PGW"));
            }
         }
         else
         {
            if (TRUE == fwd)
            {
               /* MME send Update Bearer Response with failure cause
                * forward as it is to PGW by setting CS bit */
               qoSgwEgtUtlFwdFailureRsp(pdnCb->s5s8CTun, QO_ENT_PGW,
                                       egMsg);
            }
            else
            {
               qoSgwEgtUtlSndFailureRsp(pdnCb->s5s8CTun->locTeid,
                                       EGT_GTPC_MSG_UB_RSP,
                                       egMsg->msgHdr.seqNumber, cause,
                                       QO_ENT_PGW);
               QO_SGW_DBG_ERR(("Validation of Update Bearer Response"));
            }

            ret = ROK;
         }

         break;
      }

      default:
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Update bearer Response recieved in invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwInDBRspStateHndlr
 *
 *       Desc:  Handle Delete bearer Response received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInDBRspStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInDBRspStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   U32               cause = 0;
   S16               ret = ROK;
   Bool              fwd = FALSE;

   QO_SGW_TRC2(qoSgwInDBRspStateHndlr);
   
   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValDBRsp(pdnCb, egMsg, &cause, &fwd);
         if (ROK == ret)
         {
            ret = qoSgwOutDBRsp(pdnCb, egMsg);
            if (ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending the Delete Bearer Response to PGW"));
            }
         }
         else
         {
            if (TRUE == fwd)
            {
               /* MME send Delete Bearer Response with failure cause
                * forward as it is to PGW by setting CS bit */
               qoSgwEgtUtlFwdFailureRsp(pdnCb->s5s8CTun, QO_ENT_PGW,
                                       egMsg);
            }
            else
            {
               qoSgwEgtUtlSndFailureRsp(pdnCb->s5s8CTun->locTeid,
                                       EGT_GTPC_MSG_DB_RSP,
                                       egMsg->msgHdr.seqNumber, cause,
                                       QO_ENT_PGW);
               QO_SGW_DBG_ERR(("Validation of Delete Bearer Response"));
            }

            ret = ROK;
         }

         break;
      }

      default:
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Delete bearer Response recieved in invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}



/***************************************************************
 *
 *
 *       Fun:   qoSgwOutCBRsp
 *
 *       Desc:  Sends Create beaerer Response to PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutCBRsp
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutCBRsp(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   S16         ret = ROK;
   CmLListCp     *bearerLst = NULLP;
   CmLList       *tmp = NULLP;
   QoSgwTunnelCb *s5s8UTun = NULLP;
   QoSgwBearerCb *bearerCb;


   QO_SGW_TRC2(qoSgwOutCBRsp)

   /* Removing the Ies, which are not applicable for s5/s8 intf */
   qoSgwRemCBRspIes(pdnCb, egMsg);

   ret = qoSgwBldCBRsp(pdnCb, egMsg);
   if (ROK == ret)
   {
     bearerLst = &(pdnCb->tmpPdnData->bearers);
     CM_LLIST_FIRST_NODE(bearerLst, tmp);
     while (tmp)
     {
        s5s8UTun = ((QoSgwBearerNode *)tmp)->s5s8UTun;
        s5s8UTun->state = QO_SGW_TUN_CONNECTED;
        bearerCb = pdnCb->ueCb->bearerTable[QO_SGW_BEARER_IND(s5s8UTun->ebi)];

        ret = qoSgwEguTunnelAddReq(bearerCb, NULLP, EGT_INT_S1_U_SGW_GTP_U);
        if(ret != ROK)
        {
           QO_SGW_DBG_ERR(("S1-U Tunnel Management Request, FAILED"));
        }
        ret = qoSgwEguTunnelAddReq(bearerCb, egMsg, EGT_INT_S5_S8_SGW_GTP_U);
        if(ret != ROK)
        {
           QO_SGW_DBG_ERR(("SGW S5-S8 Tunnel Management Request, FAILED"));
        }
        cmLListDelFrm(bearerLst, tmp);
        QO_SGW_FREE(tmp, sizeof(QoSgwBearerNode));

        CM_LLIST_NEXT_NODE(bearerLst, tmp);
     }
     if(NULLP == pdnCb->tmpPdnData)
     {
        QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
        pdnCb->tmpPdnData = NULLP;
     }
   }
   else
   {
      QO_SGW_DBG_ERR(("Building of Create Bearer Response, FAILED"));
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwOutUBRsp
 *
 *       Desc:  Sends Update beaerer Response to PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutUBRsp
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutUBRsp(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   S16            ret = ROK;
   CmLListCp      *bearerLst = NULLP;
   CmLList        *tmp = NULLP;

   QO_SGW_TRC2(qoSgwOutUBRsp)

   ret = qoSgwBldUBRsp(pdnCb, egMsg);
   if (ROK == ret)
   {
      /* Send Pending Response, update eg header */
      QO_SGW_DBG_INFO(("Sending Update Bearer Response to MME"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = pdnCb->s5s8CTun->locTeid;
      QoSgwLiEgtSigRsp(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                     teidTkn, egMsg);

      bearerLst = &(pdnCb->tmpPdnData->bearers);
      CM_LLIST_FIRST_NODE(bearerLst, tmp);

      while (tmp)
      {
         cmLListDelFrm(bearerLst, tmp);
         QO_SGW_FREE(tmp, sizeof(QoSgwBearerNode));

         CM_LLIST_NEXT_NODE(bearerLst, tmp);
      }

      if(NULLP == pdnCb->tmpPdnData)
      {
         QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
         pdnCb->tmpPdnData = NULLP;
      }
   }
   else
   {
      QO_SGW_DBG_ERR(("Building of Delete Bearer Response"));
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwOutDBRsp
 *
 *       Desc:  Sends Delete beaerer Response to PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwOutDBRsp
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwOutDBRsp(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
#endif
{
   TknU32         teidTkn = {0};
   U8             ebi = 0;
   S16            ret = ROK;
   CmLListCp      *bearerLst = NULLP;
   CmLList        *tmp = NULLP;
   QoSgwBearerCb  *bearer = NULLP;

   QO_SGW_TRC2(qoSgwOutDBRsp)

   ret = qoSgwBldDBRsp(pdnCb, egMsg);
   if (ROK == ret)
   {
      /* Send Pending Response, update eg header */
      QO_SGW_DBG_INFO(("Sending Delete Bearer Response to MME"));

      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val = pdnCb->s5s8CTun->locTeid;
      QoSgwLiEgtSigRsp(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                     teidTkn, egMsg);

      if (pdnCb->delPdn == TRUE)
      {
         ret = qoSgwDeletePdn(pdnCb, QO_SGW_SIGNAL_NO);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Deletion of PDN"));
         }
         else
         {
            QO_SGW_DBG_INFO(("Deletion of PDN, SUCCESS"));
         }
      }
      else
      {
         bearerLst = &(pdnCb->tmpPdnData->bearers);
         CM_LLIST_FIRST_NODE(bearerLst, tmp);

         while (tmp)
         {
            ebi = ((QoSgwBearerNode *)tmp)->ebi;
            bearer = pdnCb->ueCb->bearerTable[QO_SGW_BEARER_IND(ebi)];

            ret = qoSgwDeleteBearer(bearer);
            if(ret != ROK)
            {
               QO_SGW_DBG_ERR(("Tunnel Management Request, FAILED"));
            }

            cmLListDelFrm(bearerLst, tmp);
            QO_SGW_FREE(tmp, sizeof(QoSgwBearerNode));

            CM_LLIST_NEXT_NODE(bearerLst, tmp);
         }

         if(NULLP == pdnCb->tmpPdnData)
         {
            QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
            pdnCb->tmpPdnData = NULLP;
         }
      }
   }
   else
   {
      QO_SGW_DBG_ERR(("Building of Delete Bearer Response"));
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInDBReq
 *
 *       Desc:  Handles Delete Bearer Request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDBReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDBReq(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;
   U32               cause = 0;

   QO_SGW_TRC2(qoSgwInDBReq)
   QO_SGW_DBG_INFO(("Received Delete Bearer Request"));
   
   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find Tunnel For Create Bearer Request, FAILED"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_CB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_REJ_CXT_NON_EXISTENT,
                              QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (NULLP == pdnCb)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For Temporary PDN Data"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_DB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_NO_RES_AVAIL, QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Delete Bearer Request From PGW Tunnel %ld",
                  s5s8CTun->locTeid)); 

   QO_SGW_ALLOC(&(pdnCb->tmpPdnData), sizeof(QoSgwPdnTmpData));
   if (NULLP == pdnCb->tmpPdnData)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For Temporary PDN Data"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_DB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_NO_RES_AVAIL, QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   cmLListInit(&(pdnCb->tmpPdnData)->bearers);
   ret = qoSgwInDBReqStateHndlr(pdnCb, egMsg);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Delete Bearer Request State Handler, FAILED"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }
   else
   {
      QO_SGW_DBG_INFO(("Delete Bearer Request State Handler, SUCCESS"));
   }

   /* sending failrue response to PGW */
   if (ret != ROK)
   {
      QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_DB_RSP,
            egMsg->msgHdr.seqNumber, cause, QO_ENT_PGW);
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInUBReq
 *
 *       Desc:  Handles Update Bearer Request received from MME
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInUBReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInUBReq(suId, localAddr, remAddr, lclTeid ,msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;
   U32               cause = 0;

   QO_SGW_TRC2(qoSgwInUBReq)
   QO_SGW_DBG_INFO(("Received Update Bearer Request"));
   
   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Find Tunnel For Create Bearer Request, FAILED"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_UB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_REJ_CXT_NON_EXISTENT,
                              QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (NULLP == pdnCb)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For Temporary PDN Data"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_UB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_NO_RES_AVAIL, QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }


   QO_SGW_DBG_INFO(("Received Update Bearer Request From PGW Tunnel %ld",
                  s5s8CTun->locTeid)); 

   QO_SGW_ALLOC(&(pdnCb->tmpPdnData), sizeof(QoSgwPdnTmpData));
   if (NULLP == pdnCb->tmpPdnData)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For Temporary PDN Data"));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_UB_RSP,
                              egMsg->msgHdr.seqNumber,
                              EGT_GTPC_CAUSE_NO_RES_AVAIL, QO_ENT_PGW);
      QO_SGW_RETVALUE(RFAILED);
   }

   cmLListInit(&(pdnCb->tmpPdnData)->bearers);
   ret = qoSgwInUBReqStateHndlr(pdnCb, egMsg);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Update Bearer Request State Handler"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }
   else
   {
      QO_SGW_DBG_INFO(("Update Bearer Request State Handler, SUCCESS"));
   }

   /* sending failrue response to PGW */
   if (ret != ROK)
   {
      QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
      qoSgwEgtUtlSndFailureRsp(lclTeid.val, EGT_GTPC_MSG_UB_RSP,
            egMsg->msgHdr.seqNumber, cause,  QO_ENT_PGW);
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwInUBReqStateHndlr
 *
 *       Desc:  Handle Update bearer Request received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInUBReqStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInUBReqStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   S16      ret = RFAILED;
   U32      cause = 0;

   QO_SGW_TRC2(qoSgwInUBReqStateHndlr);

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValUBReq(pdnCb, egMsg, &cause);
         if (ROK == ret)
         {
            ret = qoSgwOutUBReq(pdnCb, egMsg);
            if (ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending The Update Bearer Request, FAILED"));
            }
         }
         else
         {
            QO_SGW_DBG_ERR(("Validating Update Session Request"));
            qoSgwEgtUtlSndFailureRsp(pdnCb->s5s8CTun->locTeid,
                                    EGT_GTPC_MSG_UB_RSP,
                                    egMsg->msgHdr.seqNumber, cause,
                                    QO_ENT_PGW);
            ret = ROK;
         }

         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Update Bearer Request Recieved In Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwInDBReqStateHndlr
 *
 *       Desc:  Handle Delete bearer Request received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwInDBReqStateHndlr
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block */
EgMsg             *egMsg      /* gtp-c msg */
)
#else
PRIVATE S16 qoSgwInDBReqStateHndlr(pdnCb, egMsg)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block */
EgMsg             *egMsg;     /* gtp-c msg */
#endif
{
   S16      ret = RFAILED;
   U32      cause = 0;

   QO_SGW_TRC2(qoSgwInDBReqStateHndlr);

   switch (pdnCb->state)
   {
      case QO_SGW_PDN_CONNECTED:
      {
         ret = qoSgwValDBReq(pdnCb, egMsg, &cause);
         if (ROK == ret)
         {
            ret = qoSgwOutDBReq(pdnCb, egMsg);
            if (ret != ROK)
            {
               QO_SGW_DBG_ERR(("Sending The Delete Bearer Request, FAILED"));
            }
         }
         else
         {
            QO_SGW_DBG_ERR(("Validating Delete Session Request"));
            qoSgwEgtUtlSndFailureRsp(pdnCb->s5s8CTun->locTeid,
                                    EGT_GTPC_MSG_DB_RSP,
                                    egMsg->msgHdr.seqNumber, cause,
                                    QO_ENT_PGW);
            ret = ROK;
         }

         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Delete Bearer Request Recieved In Invalid State"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwProcessEgtErrInd
 *
 *       Desc:  Handle Error Indication from Lower layers
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
PUBLIC S16 qoSgwProcessEgtErrInd
(
U16               suId,
U32               transId 
)
{
   S16            ret = ROK;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwTunnelCb  *s5s8CTun = NULLP;

   QO_SGW_TRC2(qoSgwProcessEgtErrInd);

   if (qoSgwCb.mmeCSap->suId == suId)
   {
      s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, transId);
      if (NULLP != s11Tun)
      {
         ret = qoSgwHdls11EgtErrInd(s11Tun);
      }
      else
      {
         QO_SGW_DBG_ERR(("Unknown Transaction ID [%ld]", transId));
      }
   }
   else if (qoSgwCb.pgwCSap->suId == suId)
   {
      s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, transId);
      if (NULLP == s5s8CTun)
      {
         s5s8CTun = qoSgwFindTunnelTrans(qoSgwCb.pgwCSap, transId);
      }

      if (NULLP != s5s8CTun)
      {
         ret = qoSgwHdls5s8EgtErrInd(s5s8CTun);
      }
      else
      {
         QO_SGW_DBG_ERR(("Unknown Transaction ID [%ld]", transId));
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwHdls11EgtErrInd
 *
 *       Desc:  Handle Error Indication from egt layer on s11 interface 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHdls11EgtErrInd
(
QoSgwTunnelCb     *s11Tun
)
#else
PRIVATE S16 qoSgwHdls11EgtErrInd(s11Tun)
QoSgwTunnelCb     *s11Tun;
#endif
{
   S16            ret = ROK;

   QO_SGW_TRC2(qoSgwHdls11EgtErrInd);
   QO_SGW_DBG_INFO(("Handling Error Indication of s11 Interface "));

   switch (s11Tun->currEvent)
   {
      default:
      {
         QO_SGW_DBG_ERR(("Unknow Event"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwHdls11EgtErrInd
 *
 *       Desc:  Handle Error Indication from egt layer on s11 interface 
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwHdls5s8EgtErrInd
(
QoSgwTunnelCb     *s5s8CTun
)
#else
PRIVATE S16 qoSgwHdls5s8EgtErrInd(s5s8CTun)
QoSgwTunnelCb     *s5s8CTun;
#endif
{
   S16            ret = ROK;
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwTunnelCb  *s11Tun = NULLP;

   QO_SGW_TRC2(qoSgwHdls5s8EgtErrInd);
   QO_SGW_DBG_INFO(("Handling Error Indication of s5/s8 Interface "));

   ueCb = s5s8CTun->ueCb;
   s11Tun = ueCb->s11Tun;

   switch (s5s8CTun->currEvent)
   {
      case QO_SGW_EGT_EVT_CS_REQ_SENT:
      {
         ret = qoSgwEgtHndlSndCSReqFailure(ueCb, s5s8CTun);
         break;
      }

      case QO_SGW_EGT_EVT_BR_CMD_SENT:
      {
         qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,\
               EGT_GTPC_MSG_BR_FAIL_IND, s11Tun->seqNum,\
               EGT_GTPC_CAUSE_PEER_NOT_RESPOND);
      }

      case QO_SGW_EGT_EVT_DB_CMD_SENT:
      {
         qoSgwEgtUtlSndFailureRspToMme(s11Tun->locTeid, s11Tun->remTeid,\
               EGT_GTPC_MSG_DB_FAIL_IN, s11Tun->seqNum,\
               EGT_GTPC_CAUSE_PEER_NOT_RESPOND);
      }

      default:
      {
         QO_SGW_DBG_ERR(("Unhandled Event"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInBRFInd
 *
 *       Desc:  Handles Bearer Resource Failure Ind received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInBRFInd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInBRFInd(suId, localAddr, remAddr, lclTeid, transId, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInBRFInd);

   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel Not Found For Bearer Resource Failure Ind."));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Bearer Resource Failure Ind From PDN-GW For "
                  "Tunnel %ld\n", s5s8CTun->locTeid)); 

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (pdnCb != NULLP)
   {
      /* forward the failure indication message to MME */
      qoSgwEgtUtlFwdFailureRsp(pdnCb->ueCb->s11Tun, QO_ENT_MME, egMsg);
   }

   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInDBFInd
 *
 *       Desc:  Handles Delete bearer Failure Ind received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInDBFInd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInDBFInd(suId, localAddr, remAddr, lclTeid, transId, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInDBFInd);

   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel Not Found For Delete Bearer Failure Ind."));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Delete Bearer Failure Ind From PDN-GW For "
                  "Tunnel %ld\n", s5s8CTun->locTeid)); 

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (pdnCb != NULLP)
   {
      /* forward the failure indication message to MME */
      qoSgwEgtUtlFwdFailureRsp(pdnCb->ueCb->s11Tun, QO_ENT_MME, egMsg);
   }


   QO_SGW_RETVALUE(ret);
}


/*****************************************************************
 *
 *       Fun:   qoSgwInMBFInd
 *
 *       Desc:  Handles Modify Bearer Failure Ind received from PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ******************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwInMBFInd
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
#else
PUBLIC S16 qoSgwInMBFInd(suId, localAddr, remAddr, lclTeid, transId, msg)
SuId           suId;       /* service user id */
CmTptAddr      *localAddr; /* local address */
CmTptAddr      *remAddr;   /* remote address */
TknU32         lclTeid;    /* local teid */
TknU32         transId;    /* Transaction id */
EgMsg          *egMsg;     /* Modify Bearer Request Msg */
#endif
{
   S16               ret = ROK;
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   QoSgwPdnCb        *pdnCb = NULLP;

   QO_SGW_TRC2(qoSgwInMBFInd);

   s5s8CTun = qoSgwFindTunnel(qoSgwCb.pgwCSap, lclTeid.val);
   if (s5s8CTun == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel Not Found For Modify Bearer Failure Ind."));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Received Modify Bearer Failure Ind From PDN-GW For "
                  "Tunnel %ld\n", s5s8CTun->locTeid)); 

   QO_SGW_GET_PDNCB(s5s8CTun, pdnCb);
   if (pdnCb != NULLP)
   {
      /* forward the failure indication message to MME */
      qoSgwEgtUtlFwdFailureRsp(pdnCb->ueCb->s11Tun, QO_ENT_MME, egMsg);
   }

   QO_SGW_RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief 
 *    Handling of Error indication from EGT Layer while sending
 *    Create Session Request to PGW
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Cb
 *    handled
 * @param[in]  pS5S8Tun
 *    Pointer to S5/S8 Tun
 * 
 * @return
 *    SUCCESS if Error Indication is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 qoSgwEgtHndlSndCSReqFailure
(
QoSgwUeCb      *pUeCb,
QoSgwTunnelCb  *pS5S8CTun
)
{
   S16               ret      = ROK;
   QoSgwPdnCb        *pPdnCb  = NULLP;
   QoSgwTunnelCb     *pS11Tun = NULLP;

   QO_SGW_TRC2(qoSgwEgtHndlSndCSReqFailure);

   pS11Tun = pUeCb->s11Tun;
   pPdnCb = pS5S8CTun->pdnCb;

   QO_SGW_DBG_INFO(("Sending Create Session Response with Failure Cause"\
         " to MME"));
   qoSgwEgtUtlSndFailureRspToMme(pS11Tun->locTeid, pS11Tun->remTeid,\
         EGT_GTPC_MSG_CS_RSP, pS11Tun->seqNum, EGT_GTPC_CAUSE_PEER_NOT_RESPOND);

   QO_SGW_DBG_INFO(("Deleting PDN Cb"));
   ret = qoSgwDeletePdn(pPdnCb, QO_SGW_SIGNAL_NO);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Deleting PDN Cb"));
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwEgtHndlSndCSReqFailure */


/**************************************************************************//**
 *
 * @brief 
 *    Handling of Release Access Bearer Request
 *             
 * @return
 *    SUCCESS Release Access Bearer Request is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 qoSgwInRABReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
)
{
   S16               ret      = ROK;
   QoSgwTunnelCb     *pS11Tun = NULLP;

   QO_SGW_TRC2(qoSgwInRABReq);

   pS11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, lclTeid.val);
   if (pS11Tun == NULLP) 
   {
      EgUtilDeAllocGmMsg(&egMsg);
      QO_SGW_DBG_ERR(("Tunnel Not Found For Release Access Bearer Request."));
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Processing Release Access Bearer Request"));
   ret = qoSgwProcRABReq(pS11Tun->ueCb, egMsg);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Processing Release Access Bearer Request"));
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwInRABReq */


/**********************************************************************
  
         End of file:     qo_app.c@@/main/1 - Thu Jul  8 21:22:18 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -         mm               1. LTE-CNE:SGW Initial Release.
/main/1      qo001.101 rp               1. Added support for Paging
                                        2. Added support for Dedicated tunnels
/main/1      qo002.101 rk               1. Dedicated Bearer Paging support 
                                           changes
/main/1      qo003.101 pmacharla        1. eGTP 2.1 upgrade under EG_REL_930 flag
*********************************************************************91*/

