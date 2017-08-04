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

     Name:    SGW Lower Interface 

     Type:    C source file

     Desc:    Transport Connection Manager lower interface primitives

     File:    qo_li.c

     Sid:      qo_li.c@@/main/1 - Thu Jul  8 21:22:20 2010

     Prg:     mm 
*********************************************************************21*/

/* header include files (.h) */
#include <stdarg.h>        /* environment options */
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
#include "eg_dbs11.h"
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

/***************************************************************/

/***************************************************************/

/***************************************************************/
/*
*
*       Fun:   EuLiEgtBndCfm
*
*       Desc:  Call handler for Bind Confirmation
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
#else
PUBLIC S16 EuLiEgtBndCfm(pst, suId, status)
Pst            *pst;
SuId           suId;
U8             status;
#endif
{
   QoUstaDgn dgn;
   QO_SGW_TRC2(EuLiEgtBndCfm);

   cmMemset((U8 *)&dgn, 0, sizeof(QoUstaDgn));
   if(qoSgwCb.init.cfgDone != TRUE)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("SGW is not configured \n"));
      RETVALUE(RFAILED);
   }
   if(CM_BND_OK == status)
   {
#ifdef QO_TODO_SECOND_PHASE
      /* When all saps interact with Trillium GTP.
       * Add a check for the SapID.
       * Right now it is only eNB-User SAP. */
#endif
      if(suId == qoSgwCb.egUSap->suId)
      {
         QO_SGW_DBG_INFO(("The Lower SAP at EGTPU is now bound"));
         qoSgwCb.egUSap->state = LQO_SAP_ISBOUND;
      }
      else if(suId == qoSgwCb.pgwCSap->suId)
      {
         QO_SGW_DBG_INFO(("The Lower SAP at EGTPC for S5S8 is now bound"));
         qoSgwCb.pgwCSap->state = LQO_SAP_ISBOUND;
      }
      else if(suId == qoSgwCb.mmeCSap->suId)
      {
         QO_SGW_DBG_INFO(("The Lower SAP at EGTPC for S11 is now bound"));
         qoSgwCb.mmeCSap->state = LQO_SAP_ISBOUND;
      }
      else if(suId == qoSgwCb.echoCSap->suId)
      {
         QO_SGW_DBG_INFO(("The Lower SAP at EGTPC for echo is now bound"));
         qoSgwCb.echoCSap->state = LQO_SAP_ISBOUND;
      }
      else if(suId == qoSgwCb.sgsnCSap->suId)
      {
         QO_SGW_DBG_INFO(("The Lower SAP at EGTPC for S4 is now bound"));
         qoSgwCb.sgsnCSap->state = LQO_SAP_ISBOUND;
      }
      else
      {
         QO_SGW_DBG_ERR(("Invalid suId received.\n"));
         RETVALUE(RFAILED);
      }
      dgn.sapId = suId;

      /* Generate an Alarm to stack manager */
      qoSendLmAlarm(LCM_CATEGORY_INTERFACE, LQO_EVENT_SGW_EGTP_BNDCFM, 
            LCM_CAUSE_UNKNOWN,  
            &dgn);
   }
   else
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("Bind Failure with EGTP \n"));
   }

   RETVALUE(ROK);
}
/***************************************************************/

/**
 * Handle incoming GTP-U Data Packet
 * */
#ifdef ANSI
PUBLIC S16 EuLiEgtEguDatInd
     (
      Pst   *pst,        /* post structure */
      SuId         suId,        /* eGTP-U service user SAP ID */
      EgtUEvnt     *datInd
      )
#else
PUBLIC S16 EuLiEgtEguDatInd(pst,suId,datInd)
      Pst          *pst;
      SuId         suId;
      EgtUEvnt     *datInd;
#endif

{
   S16 ret = ROK;
   QO_SGW_TRC2(EuLiEgtEguDatInd)

   if(qoSgwCb.init.cfgDone != TRUE)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("SGW is not configured \n"));
      RETVALUE(RFAILED);
   }
   ret = qoSgwInUserData(suId, datInd);
   RETVALUE(ret);
}

/***************************************************************/
/**
 * Handle incoming GTP-C request
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtSigInd
    (
      Pst *pst,
      SuId suId,
      CmTptAddr *localAddr,
      CmTptAddr *remAddr,
      TknU32 lclTeid,
      EgMsg *msg
      )
#else
PUBLIC S16 EuLiEgtSigInd(pst,suId,localAddr,remAddr,lclTeid,msg)
      Pst *pst;
      SuId suId;
      CmTptAddr *localAddr;
      CmTptAddr *remAddr;
      TknU32 lclTeid;
      EgMsg *msg;
#endif

{
   S16 ret = ROK;
   QO_SGW_TRC2(EuLiEgtSigInd)

   if(qoSgwCb.init.cfgDone != TRUE)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("SGW is not configured \n"));
      RETVALUE(RFAILED);
   }

   switch(msg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_CS_REQ:
         ret = qoSgwInCSReq(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_MB_REQ:
         ret = qoSgwInMBReq(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_DS_REQ:
         ret = qoSgwInDSReq(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case  EGT_GTPC_MSG_BR_CMND:
         ret = qoSgwInBRCmd(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_CB_REQ:
         ret = qoSgwInCBReq(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_DB_CMND:
         ret = qoSgwInDBCmd(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_MB_CMND:
         ret = qoSgwInMBCmd(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_DB_REQ:
         ret = qoSgwInDBReq(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_UB_REQ:
         ret = qoSgwInUBReq(suId, localAddr, remAddr, lclTeid, msg);
         break;
      case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
         ret = qoSgwInDDNFInd(suId,localAddr,remAddr,lclTeid,msg);
         break;
      case EGT_GTPC_MSG_RAB_REQ:
         ret = qoSgwInRABReq(suId,localAddr,remAddr,lclTeid,msg);
         break;
      default:
         QO_SGW_LOGERR(ERRCLS_DEBUG, ("Unknown Message Type \n"));
         ret = RFAILED;
         break;
   }

   RETVALUE(ret);
}

/***************************************************************/
/**
 * Handle incoming GTP-C Response
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtSigCfm
     (
      Pst         *pst,           /* post structure */
      SuId         suId,          /* Use values from EgwNodeSapId */
      CmTptAddr   *localAddr,     /* Local node address */
      CmTptAddr   *remAddr,       /* Remote node address */
      TknU32       transId,       /* */
      TknU32       teid,          /* */
      EgMsg       *msg          /* structure for carrying eGTP MSG info */
      )
#else
PUBLIC S16 EuLiEgtSigCfm(pst,suId,localAddr,remAddr,transId,teid,msg)
      Pst         *pst;
      SuId         suId;
      CmTptAddr   *localAddr;
      CmTptAddr   *remAddr;
      TknU32       transId;
      TknU32       teid;
      EgMsg       *msg;
#endif

{
   S16 ret = ROK;
   QO_SGW_TRC2(EuLiEgtSigCfm)
   if(qoSgwCb.init.cfgDone != TRUE)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("SGW is not configured \n"));
      RETVALUE(RFAILED);
   }
   switch(msg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_CS_RSP:
         qoSgwInCSRsp(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_DS_RSP:
         qoSgwInDSRsp(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_MB_RSP:
         qoSgwInMBRsp(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_CB_RSP:
         ret = qoSgwInCBRsp(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_DB_RSP:
         ret = qoSgwInDBRsp(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_UB_RSP:
         ret = qoSgwInUBRsp(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_BR_FAIL_IND:
         ret = qoSgwInBRFInd(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_DB_FAIL_IN:
         ret = qoSgwInDBFInd(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_MBFAIL_IND:
         ret = qoSgwInMBFInd(suId, localAddr, remAddr, teid, transId, msg);
         break;
      case EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK:
         ret = qoSgwInDDNAck(suId, localAddr, remAddr, teid, transId, msg);
         break;
      default:
         QO_SGW_LOGERR(ERRCLS_DEBUG, ("Unknown Message Type \n"));
         ret = RFAILED;
         break;
   }
   RETVALUE(ret);
}

/*
*
*       Fun:   EuLiEgtStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtStaInd
(
Pst            *pst,
SuId           suId,
U8           eventType,    /* Status event type */
EgPathStatus   *pathStatus
)
#else
PUBLIC S16 EuLiEgtStaInd(pst, suId, eventType, pathStatus)
Pst            *pst;
SuId           suId;
U8           eventType;    /* Status event type */
EgPathStatus   *pathStatus;
#endif /* ANSI */
{
   QO_SGW_TRC2(EuLiEgtStaInd);

   QO_SGW_DBG_INFO(("Status indication received from EGTP."));
   RETVALUE(ROK);
} /* End of EuLiEgtStaInd */

/*
*
*       Fun:   EuLiEgtStaCfm
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtStaCfm
(
Pst            *pst,
SuId           suId,
U8           eventType,    /* Status event type */
EgPathInfo   *pathInfo,
CmStatus     status
)
#else
PUBLIC S16 EuLiEgtStaCfm(pst, suId, eventType, pathInfo, status)
Pst            *pst;
SuId           suId;
U8           eventType;    /* Status event type */
EgPathInfo   *pathInfo;
CmStatus     status;
#endif /* ANSI */
{
   QO_SGW_TRC2(EuLiEgtStaCfm);

   QO_SGW_DBG_INFO(("Status Confirmation received from EGTP."));
   RETVALUE(ROK);
} /* End of EuLiEgtStaCfm */



/*
*
*       Fun:   EuLiEgtErrInd
*
*       Desc:  Call handler for Error Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtErrInd
(
Pst            *pst,
SuId           suId,
EgErrEvnt      *errEvnt
)
#else
PUBLIC S16 EuLiEgtErrInd(pst, suId, errEvnt)
Pst            *pst;
SuId           suId;
EgErrEvnt      *errEvnt;
#endif /* ANSI */
{
   QO_SGW_TRC2(EuLiEgtErrInd);

   QO_SGW_DBG_INFO(("Error Indication received from EGTP Trans ID (%ld)",\
         errEvnt->seqNo));

   if (qoSgwCb.mmeCSap->suId == suId)
   {
      qoSgwProcessEgtErrInd(suId, errEvnt->seqNo);
   }
   else if (qoSgwCb.pgwCSap->suId == suId)
   {
      qoSgwProcessEgtErrInd(suId, errEvnt->seqNo);
   }
   else
   {
      QO_SGW_DBG_INFO(("Error Indication for suId [%ld] Unhandled", suId));
   }

   RETVALUE(ROK);
} /* End of EuLiEgtErrInd */

/***************************************************************/
/**
 * handle the confirmation of local delete request for a tunnel end point
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtLclDelCfm 
       (
         Pst          *pst,      /* post structure */
         SuId         suId,      /* eGTP service user SAP ID */
         U32          transId,    /* Transaction ID */
         U32          teid,      /* self TEID */
         CmStatus     status  /* Status of request primitive */
        )
#else
PUBLIC S16 EuLiEgtLclDelCfm (pst,suId,transId,teid,status)
         Pst          *pst;
         SuId         suId;
         U32          transId;
         U32          teid;
         CmStatus     status;
#endif
{
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwUeEnt     *ueEnt = NULLP;
   S16            ret = ROK;
   QoSgwTunnelCb  *tunnel = NULLP;

   QO_SGW_TRC2(EuLiEgtLclDelCfm )

   if(qoSgwCb.init.cfgDone != TRUE)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("SGW is not configured \n"));
      RETVALUE(RFAILED);
   }
   if(status.status == LCM_PRIM_NOK)
   {
#ifdef QO_TODO_SECOND_PHASE
      /* status is Request failure , rollback */
#endif
      QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
   }
   else
   {
      if(qoSgwCb.mmeCSap->suId == suId)
      {
         tunnel = qoSgwFindTunnel(qoSgwCb.mmeCSap, teid);
         if(NULLP != tunnel)
         {
            if(tunnel->state == QO_SGW_TUN_LCLDEL)
            {
               if(tunnel == tunnel->ueCb->s11Tun)
               {
                  tunnel->ueCb->s11Tun = NULLP;
               }

               ueCb = tunnel->ueCb;
               qoSgwDeleteTunnel(qoSgwCb.mmeCSap, &tunnel);

               /* s11 c-plane tunnel is delete, now free the UE CB */
               ret = cmHashListFind(&(qoSgwCb.ueCbHlCp), (U8 *)ueCb->ueId,
                                    ueCb->ueIdLen, 0, (PTR *)&ueEnt);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("UE not found to delete."));
               }
               else
               {
                  cmHashListDelete(&(qoSgwCb.ueCbHlCp), (PTR)ueEnt);
                  QO_SGW_FREE(ueEnt, sizeof(QoSgwUeEnt));
                  QO_SGW_DBG_INFO(("UE deleted."));
               }
            }
            else
            {
               ret = RFAILED;
               QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
            }
         }
         else
         {
            ret = RFAILED;
            QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
         }
      }
      else if(qoSgwCb.sgsnCSap->suId == suId)
      {
         tunnel = qoSgwFindTunnel(qoSgwCb.sgsnCSap, teid);
         if(NULLP != tunnel)
         {
            if(tunnel->state == QO_SGW_TUN_LCLDEL)
            {
               if(tunnel == tunnel->ueCb->s4Tun)
               {
                  tunnel->ueCb->s4Tun = NULLP;
               }

               ueCb = tunnel->ueCb;
               qoSgwDeleteTunnel(qoSgwCb.sgsnCSap, &tunnel);

               /* s4 c-plane tunnel is delete, now free the UE CB */
               ret = cmHashListFind(&(qoSgwCb.ueCbHlCp), (U8 *)ueCb->ueId,
                                    ueCb->ueIdLen, 0, (PTR *)&ueEnt);
               if(ret != ROK)
               {
                  QO_SGW_DBG_ERR(("UE not found to delete."));
               }
               else
               {
                  cmHashListDelete(&(qoSgwCb.ueCbHlCp), (PTR)ueEnt);
                  QO_SGW_FREE(ueEnt, sizeof(QoSgwUeEnt));
                  QO_SGW_DBG_INFO(("UE deleted."));
               }
            }
            else
            {
               ret = RFAILED;
               QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
            }
         }
         else
         {
            ret = RFAILED;
            QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
         }
      }
      else if(qoSgwCb.pgwCSap->suId == suId)
      {
         tunnel = qoSgwFindTunnel(qoSgwCb.pgwCSap, teid);
         if(NULLP != tunnel)
         {
            if(tunnel->state == QO_SGW_TUN_LCLDEL)
            {
               qoSgwDeleteTunnel(qoSgwCb.pgwCSap, &tunnel);
            }
            else
            {
               ret = RFAILED;
               QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
            }
         }
         else
         {
            ret = RFAILED;
            QO_SGW_DBG_ERR(("In GTP-C Delete Tunnel request failed.\n"));
         }
      }
      else
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Invalid SAP.\n"));
      }
   }
   RETVALUE(ROK);
}

/*
 * Handler for C-plane modification confirm from the egtp.
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtTunnModCfm
(
Pst      *pst,             /* Pst structure for the sap */
SuId      suId,            /* Service user ID of the SAP */ 
U32       transId,         /* Transaction ID */
U32       teid,            /* teid of for which cfm is received */
CmTptAddr oldDstIpAddr,    /* Old dst Ip associated with tunnel */
CmTptAddr newDstIpAddr,    /* New IP or new peer address */
CmStatus  status           /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
)
#else
PUBLIC S16 EuLiEgtTunnModCfm(pst, suId, transId, teid, oldDstIpAddr, newDstIpAddr, status)
Pst      *pst;             /* Pst structure for the sap */
SuId      suId;            /* Service user ID of the SAP */ 
U32       transId;         /* Transaction ID */
U32       teid;            /* teid of for which cfm is received */
CmTptAddr oldDstIpAddr;    /* Old dst Ip associated with tunnel */
CmTptAddr newDstIpAddr;    /* New IP or new peer address */
CmStatus  status;          /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
#endif
{
   QoSgwTunnelCb        *tunnel = NULLP;
   S16                   ret = RFAILED;

   QO_SGW_TRC2(EuLiEgtTunnModCfm)


      if(qoSgwCb.init.cfgDone != TRUE)
      {
         QO_SGW_LOGERR(ERRCLS_DEBUG,
               ("SGW is not configured \n"));
         RETVALUE(ret);
      }

   if(suId == qoSgwCb.mmeCSap->suId)
   { 
      tunnel = qoSgwFindTunnel(qoSgwCb.mmeCSap, teid);
   }
   else if(suId == qoSgwCb.sgsnCSap->suId)
   {
      tunnel = qoSgwFindTunnel(qoSgwCb.sgsnCSap, teid);
   }
   else
   {
      QO_SGW_DBG_ERR(("Tunnel not found. Invalid TEID.\n"));
   }

   if(tunnel == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel not found. Invalid TEID.\n"));
      RETVALUE(ret);
   }
   switch(tunnel->intf)
   {
      case QO_IF_S11_C:
         {
            ret = qoSgwProcessS11cTnlMgmtCfm(tunnel, status);
         }
         break;

      case QO_IF_S5_S8_C:
         {
            QO_SGW_DBG_INFO(("No handler for QO_IF_S5_S8_C interface \n"));
         }
         break;
      case QO_IF_S4_C: /* control plane towards SGSN */
         {
            ret = qoSgwProcessS4cTnlMgmtCfm(tunnel, status);
         }
         break; 
      default:
         {
            QO_SGW_DBG_ERR(("Unknown interface %d \n", tunnel->intf));
            RETVALUE(ret);
         }

   }
   RETVALUE(ret);
}


/*
 *
 *       Fun:   EuLiEgtEguLclTnlMgmtCfm
 *
 *       Desc:  Handles Tunnel management confirmation from EGT-U
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_li.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm
(
Pst          *pst,      /* post structure */
SuId         suId,      /* eGTP service user SAP ID */
EgtUEvnt     *mgmtCfm   /* Status of request primitive */
) 
#else
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm(pst, suId, mgmtCfm)
Pst          *pst;      /* post structure */
SuId         suId;      /* eGTP service user SAP ID */
EgtUEvnt     *mgmtCfm;  /* Status of request primitive */
#endif
{
   S16                  ret = ROK;
   QoSgwUeCb            *ueCb = NULLP;
   QoSgwTunnelCb        *tunnel = NULLP;

   QO_SGW_TRC2(EuLiEgtEguLclTnlMgmtCfm)

   if (qoSgwCb.init.cfgDone != TRUE)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("SGW is not configured \n"));
      SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));
      RETVALUE(RFAILED);
   }

   if(suId != qoSgwCb.egUSap->suId)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, 
            ("Invalid SAP \n"));
      SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));
      RETVALUE(RFAILED);
   }

   tunnel = qoSgwFindTunnel(qoSgwCb.egUSap, mgmtCfm->u.tnlMgmt.teId);
   if(tunnel == NULLP)
   {
      QO_SGW_DBG_ERR(("Tunnel not found. Invalid TEID.\n"));
      SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));
      RETVALUE(RFAILED);
   }

   ueCb = tunnel->ueCb;

   switch (tunnel->intf)
   {
      case QO_IF_S1U_DEF:
      {
         /* currently trillium layer is used for GTP-U connection
          * with ENobeB */
         ret = qoSgwProcessS1UTnlMgmtCfm(mgmtCfm, tunnel);
         break;
      }

      case QO_IF_S1U_DED:
      {
         if(mgmtCfm->u.tnlMgmt.action == EGT_TMGMT_REQTYPE_ADD)
         {
            /* Do nothing */
            QO_SGW_DBG_INFO(("S1-U Tunnel moved to COnnected State."));
            tunnel->state = QO_SGW_TUN_CONNECTED;
         }
         else if(mgmtCfm->u.tnlMgmt.action == EGT_TMGMT_REQTYPE_DEL)
         {
            QO_SGW_DBG_INFO(("S1-U Tunnel moved to Disconnected State."));
         }

         qoSgwProcessS1DedTnlMgmtCfm(mgmtCfm, tunnel);
         break;
      }

      case QO_IF_S5_S8_DEF:
      {
         ret = qoSgwProcS5S8DefTnlMgmtCfm(mgmtCfm, tunnel);
         break;
      }

      case QO_IF_S5_S8_DED:
      {
         ret = qoSgwProcessS5s8DedTnlMgmtCfm(mgmtCfm, tunnel);
         break;
      }

      case QO_IF_S4_DEF:
      {
         ret = qoSgwProcS4DefTnlMgmtCfm(mgmtCfm, tunnel);
         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Uknown Tunnel Type"));
         SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));
         RETVALUE(RFAILED);
         break;
      }
   }

   SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));

   /* qo001.101: This check is for destroying a UE context.
    * If tunnel confirm is for deletion of tunnel, all
    * tunnels are deleted remove the UE cotnext and there 
    * are no dedicated tunnels for the ue */
   if (ueCb != NULLP)
   {
      if((ueCb->intf == QO_IF_S11_C) && (ueCb->s11Tun == NULLP))
      {
         qoSgwDeleteUeCb(ueCb, QO_SGW_SIGNAL_MME);
      }
      else if((ueCb->intf == QO_IF_S4_C) && (ueCb->s4Tun == NULLP))
      {
         qoSgwDeleteUeCb(ueCb, QO_SGW_SIGNAL_SGSN);
      }
   }

   RETVALUE(ret);
}   


/***************************************************************/

/*
*
*       Fun:   EuLiEgtEguStaCfm
*
*       Desc:  Call handler for eGTP-U Status Confirm.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguStaCfm
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguStaCfm(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   QO_SGW_TRC2(EuLiEgtEguStaCfm);

   QO_SGW_DBG_INFO(("Status Confirmation received from EGTPU"));
   RETVALUE(ROK);
} /* End of EuLiEgtEguStaCfm */

/*
*
*       Fun:   EuLiEgtEguUStaInd
*
*       Desc:  Call handler for eGTP-U Unsolicited Status Indication.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  
*
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtEguUStaInd
(
Pst         *pst,
SuId        suId,
EgtUEvnt    *eguMsg
)
#else
PUBLIC S16 EuLiEgtEguUStaInd(pst, suId, eguMsg)
Pst         *pst;
SuId        suId;
EgtUEvnt    *eguMsg;
#endif /* ANSI */
{
   QO_SGW_TRC2(EuLiEgtEguUStaInd);

   QO_SGW_DBG_INFO(("Status Indication received from EGTPU"));
   RETVALUE(ROK);
} /* End of EuLiEgtEguUStaInd */

/***************************************************************/
/** 
 * Handle Error Indication from GTP-U
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtEguErrInd
        (
         Pst          *pst,        /* post structure */
         SuId         suId,        /* eGTP-U service user SAP ID */
         EgtUEvnt     *errInd     /* error event */
         )
#else
PUBLIC S16 EuLiEgtEguErrInd(pst,suId,errInd)
         Pst          *pst;
         SuId         suId;
         EgtUEvnt     *errInd;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(EuLiEgtEguErrInd)

   QO_SGW_DBG_ERR(("In GTP-U. Cause = %d, \
               local TEID = %ld remote TEID = %ld Diagnostic Information = \
               %ld\n",
               errInd->u.errInd.errCause, errInd->u.errInd.localTeid,
               errInd->u.errInd.remTeid, errInd->u.errInd.diagInfo));
   RETVALUE(ret);
}

/***************************************************************/
/** 
 * Handle Sending of GTP-U data packet through GTP-U
 */
#ifdef ANSI
PUBLIC S16 QoSgwLiEgtEguDatReq
(
Pst *pst, 
SpId spId, 
EgtUEvnt *egtUEvnt
)
#else
PUBLIC S16 QoSgwLiEgtEguDatReq(pst,spId,egtUEvnt)
      Pst *pst;
      SpId spId;
      EgtUEvnt *egtUEvnt;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(QoSgwLiEgtEguDatReq)

   ret = EuLiEgtEguDatReq (pst, spId, egtUEvnt);
   RETVALUE(ret);
}

/**************************************************************
 *      Fun    : QoSgwLiEgtEgcTnlModreq
 *      
 *      Desc   : Updating the C-Plane remaddr.
 *
 *
 * **********************************************************/
#ifdef ASNI
PUBLIC S16 QoSgwLiEgtEgcTnlModreq
(
Pst *pst,
SpId spId,
U32 transId,
U32 teid,
CmTptAddr oldDstIpAddr,
CmTptAddr newDstIpAddr
)
#else
PUBLIC S16 QoSgwLiEgtEgcTnlModreq(pst, spId, transId, teid, oldDstIpAddr, newDstIpAddr)
Pst *pst;
SpId spId;
U32 transId;
U32 teid;
CmTptAddr oldDstIpAddr;
CmTptAddr newDstIpAddr;
#endif
{
   S16 ret = ROK;

   QO_SGW_TRC2(QoSgwLiEgtEgcTnlModreq)


   ret = EuLiEgtEgcTnlModreq(pst, spId, transId, teid, oldDstIpAddr, newDstIpAddr);
   RETVALUE(ret);
}
/***************************************************************/
/** 
 * Handle Sending of tunnel Management Request
 */
#ifdef ANSI
PUBLIC S16 QoSgwLiEgtEguTnlMgmtReq
(
Pst *pst, 
SpId spId, 
EgtUEvnt *egtUEvnt
)
#else
PUBLIC S16 QoSgwLiEgtEguTnlMgmtReq(pst,spId,egtUEvnt)
 Pst *pst;
 SpId spId;
 EgtUEvnt *egtUEvnt;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(QoSgwLiEgtEguTnlMgmtReq)

   ret = EuLiEgtEguLclTnlMgmtReq (pst, spId, egtUEvnt);
   RETVALUE(ret);
}
/***************************************************************/
/** 
 * Handle Sending of GTP-C Request through GTP-C
 */
#ifdef ANSI
PUBLIC S16 QoSgwLiEgtSigReq
    ( 
      Pst *pst, 
      SpId spId, 
      CmTptAddr *localAddr,
      CmTptAddr *remAddr, 
      TknU32 lclTeid, 
      TknU32 transId,
      TknU8 crtFlag, 
      TmrCfg t3, 
      EgMsg *msg
     )
#else
PUBLIC S16 QoSgwLiEgtSigReq(
      pst,spId,localAddr,remAddr,lclTeid,transId,crtFlag,t3,msg)
      Pst *pst;
      SpId spId;
      CmTptAddr *localAddr;
      CmTptAddr *remAddr;
      TknU32 lclTeid;
      TknU32 transId;
      TknU8 crtFlag;
      TmrCfg t3;
      EgMsg *msg;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(QoSgwLiEgtSigReq)

   ret = EuLiEgtSigReq (pst, spId, localAddr, remAddr, lclTeid, transId, crtFlag,
            t3, msg);
   RETVALUE(ret);
}

/***************************************************************/
/** 
 * Handle Sending of GTP-C Response through GTP-C
 */
#ifdef ANSI
PUBLIC S16 QoSgwLiEgtSigRsp
     (
       Pst *pst, 
       SpId spId, 
       TknU32 lclTeid,
       EgMsg *msg
      )
#else
PUBLIC S16 QoSgwLiEgtSigRsp(pst,spId,lclTeid,msg)
      Pst *pst;
      SpId spId;
      TknU32 lclTeid;
      EgMsg *msg;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(QoSgwLiEgtSigRsp)

   ret = EuLiEgtSigRsp (pst, spId, lclTeid, msg);
   RETVALUE(ret);
}

/***************************************************************/
/***************************************************************/


/**********************************************************************
  
         End of file:     qo_li.c@@/main/1 - Thu Jul  8 21:22:20 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---         sp               1. LTE-CNE:SGW Initial Release.
/main/1      qo001.101   rp               1. Added support for dedicated tunnels
*********************************************************************91*/

