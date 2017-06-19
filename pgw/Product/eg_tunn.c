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
  
     Name:     e-GTP layer
  
     Type:     C include file
  
     Desc:     Tunnel management module.
  
     File:     eg_tunn.c
  
     Sid:      pg_tunn.c@@/main/3 - Fri Feb  4 18:18:17 2011
     
     Prg:      
  
*********************************************************************21*/
/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment dependent */
  
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common header 1 */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common header 3 */
#include "cm_err.h"        /* common error */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tpt.h"        /* common transport */
#include "cm_dns.h"        /* common transport */
#include "leg.h"           /* eGTP Layer */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "hw.h"
#include "sht.h"
#include "lhw.h"
#endif

#include "eg.h"            /* eGTP Layer */
#include "eg_err.h"        /* eGTP Error Codes */
#include "eg_edm.h"        /* EDM Module structures            */
#include "eg_dbutl.h"       /* eGTP layer */
#include "eg_tunn.h"       /* eGTP layer */
/* eg009.201 - Added for S2B */
#ifdef EG_REL_AC0
#include "eg_dbs2b.h"       /* eGTP layer */
#endif
/* eg009.201 - Added for S2A */
#ifdef EG_S2A_SUPP
#include "eg_dbs2a.h"       /* eGTP layer */
#endif
/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common header 3 */
#include "cm_inet.x"       /* common tokens  */
#include "cm_tpt.x"        /* common transport */
#include "cm_dns.x"        /* common transport */
#include "egt.x"           /* eGTP layer */
#include "leg.x"           /* eGTP layer */
#include "eg_edm.x"        /* EDM Module structures            */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "eg.x"            /* eGTP layer */
#include "eg_tpt.x"            /* EGTP TPT module defines */
#include "eg_tunn.x"       /* eGTP layer */
#include "eg_util.x"       /* eGTP layer */
#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC PSF*/
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif


/******************************************************************************
 *                      Global Static variables                               *
 ******************************************************************************/
#ifdef EGTP_U
PRIVATE S16 egUTunHdlMsgTypeEchoReq ARGS
((
EgUSapCb *usap,           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb             /*IP CB*/
));
#endif /* EGTP_U */

#ifdef EGTP_C
/**********************************************************************
 Global Information about the categories of the messages 
**********************************************************************/
EgtMsgCat EgMsgCatDb[EGT_GTP_MSG_MAX+1] =
{
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST,      /*  = 1, */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST,      /*    = 32, */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER, /* 50 */
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_COMMAND,       /*   = 64, */
   EG_GTPC_MSGCAT_FAILIND,
   EG_GTPC_MSGCAT_COMMAND,
   EG_GTPC_MSGCAT_FAILIND,
   EG_GTPC_MSGCAT_COMMAND,
   EG_GTPC_MSGCAT_FAILIND,
   EG_GTPC_MSGCAT_FAILIND,
/* eg009.201 - Added for S2B and S2A */
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST,     /*     = 95, */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   /* ccpu00137353 */
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST,     /*   = 128, */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST,     /*   = 130, */
   EG_GTPC_MSGCAT_RSP_ACK,     /*   = 131, */
   EG_GTPC_MSGCAT_ACKN,        /*   = 132, */
   EG_GTPC_MSGCAT_REQUEST,     /*   = 133, */
   EG_GTPC_MSGCAT_RESPONSE,    /*   = 134, */
   EG_GTPC_MSGCAT_NOTFN,       /*   = 135, */
   EG_GTPC_MSGCAT_ACKN,        /*   = 136, */
   EG_GTPC_MSGCAT_NOTFN,       /*   = 137, */
   EG_GTPC_MSGCAT_ACKN,        /*   = 138, */
   EG_GTPC_MSGCAT_REQUEST,     /*   = 139, */
   EG_GTPC_MSGCAT_RESPONSE,    /*   = 140, */
   EG_GTPC_MSGCAT_REQUEST,    /*    = 141, */
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_NOTFN,  /*      = 149, */
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_IND,
   EG_GTPC_MSGCAT_IND, /* = 152, */
   EG_GTPC_MSGCAT_NOTFN,
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_NOTFN,
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST, /*    = 160, */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_NOTFN,
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_NOTFN,
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   /* eg007.201: Update msg category of DIDF req/resp */
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   /* eg010.201 : lpr_fix_ccpu00124570 */
   EG_GTPC_MSGCAT_REQUEST,
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_NOTFN,    /*  = 176,  */
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_NOTFN, /* =179, */
   EG_GTPC_MSGCAT_ACKN,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER, /* 195 */
   EG_GTPC_MSGCAT_OTHER, 
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST, /* = 200 */
   EG_GTPC_MSGCAT_RESPONSE,
   /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_REQUEST, /* = 211 */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER,
   EG_GTPC_MSGCAT_OTHER, /* = 230 */
   EG_GTPC_MSGCAT_REQUEST, /* = 231 */
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST, 
   EG_GTPC_MSGCAT_RESPONSE,
   EG_GTPC_MSGCAT_REQUEST, /* = 235 */
   EG_GTPC_MSGCAT_RESPONSE,
#endif /* EG_REL_930 */
   EG_GTPC_MSGCAT_OTHER
};

/*****************************************************************************
*                       FSM initialization.                                  *
******************************************************************************/

PRIVATE EgTunnFsmDef egTunnFsm[EGT_TUN_STATE_MAX][EG_TUN_EVENT_MAX] =
{
   /* EGT_TUN_STATE_IDLE */
   {
      egTunnFsmIdlISesReq, /* EG_TUN_EVENT_INCSESSREQ */
      egTunnFsmIdlOSesReq, /* EG_TUN_EVENT_OUTSESSREQ */
      egTunnFsmInvNetEvnt, /* EG_TUN_EVENT_INCSESSRSP */
      egTunnFsmInvUsrEvnt, /* EG_TUN_EVENT_OUTSESSRSP */
      NULLP,               /* EG_TUN_EVENT_ERR_IND
                              cannot happen in idle state */
      egTunnFsmInvNetEvnt, /* EG_TUN_EVENT_INC_MSG */
      egTunnFsmInvUsrEvnt  /* EG_TUN_EVENT_OUT_MSG */
   },
   /* EGT_TUN_STATE_INCSESSREQ */
   {
      egTunnFsmInvNetEvnt, /* EG_TUN_EVENT_INCSESSREQ */
      egTunnFsmInvUsrEvnt, /* EG_TUN_EVENT_OUTSESSREQ */
      egTunnFsmInvNetEvnt, /* EG_TUN_EVENT_INCSESSRSP */
      egTunnFsmIncOutRsp,  /* EG_TUN_EVENT_OUTSESSRSP */
#ifdef PHASE2
      egTunnFsmIncErr,     /* EG_TUN_EVENT_ERR_IND */
#else
      NULLP,               /* EG_TUN_EVENT_ERR_IND
                              There are no error indication for this state.
                              In case multi threaded Enc/Dec. Failure of
                              outgoing response may be posted. */
#endif
      egTunnFsmInvNetEvnt, /* EG_TUN_EVENT_INC_MSG */
      egTunnFsmIncOutRsp   /* EG_TUN_EVENT_OUT_MSG */
   },
   /* EGT_TUN_STATE_OUTSESSREQ */
   {
      egTunnFsmInvNetEvnt, /* EG_TUN_EVENT_INCSESSREQ */
      egTunnFsmInvUsrEvnt, /* EG_TUN_EVENT_OUTSESSREQ */
      egTunnFsmOutIncRsp,  /* EG_TUN_EVENT_INCSESSRSP */
      egTunnFsmInvUsrEvnt, /* EG_TUN_EVENT_OUTSESSRSP */
      egTunnFsmOutErr,     /* EG_TUN_EVENT_ERR_IND */
      egTunnFsmOutIncRsp,  /* EG_TUN_EVENT_INC_MSG */
      egTunnFsmInvUsrEvnt  /* EG_TUN_EVENT_OUT_MSG */
   },
   /* EG_TUN_STATE CONNECTED */
   {
      egTunnFsmConIncMsg, /* EG_TUN_EVENT_INCSESSREQ */
      egTunnFsmConOutMsg, /* EG_TUN_EVENT_OUTSESSREQ */
      egTunnFsmConIncMsg, /* EG_TUN_EVENT_INCSESSRSP */
      egTunnFsmConOutMsg, /* EG_TUN_EVENT_OUTSESSRSP */
      egTunnFsmConErr,    /* EG_TUN_EVENT_ERR_IND */
      egTunnFsmConIncMsg, /* EG_TUN_EVENT_INC_MSG */
      egTunnFsmConOutMsg  /* EG_TUN_EVENT_OUT_MSG */
   }
};

#endif
/******************************************************************************
 *             Public function definitions for Tunnel Management              *
 ******************************************************************************/

#ifdef EGTP_C
/*************************************************************************
 *      Fun:   egChkFailInd
 *
 *      Desc:  The function egChkFailInd matches a GTP command message type to
 *             to the GTP command error indication message type.
 *
 *      Ret:
 *              - ROK - If the procedure was successful
 *              - ROUTRES - If the procedure failed due to memory failure.
 *              - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egChkFailInd
( 
U8   msgType,
U8   pairMsgType
)
#else
PUBLIC S16 egChkFailInd(msgType, pairMsgType)
U8   msgType;
U8   pairMsgType;
#endif
{
   S16 ret;

   EG_TRC2(egChkFailInd );

   ret = ROK;

   /***********************************************
   * if the Pair Message type is not equal to    * 
   * EGTP Message type then log error and return *
   ***********************************************/
   switch(pairMsgType)
   {
      case EGT_GTPC_MSG_DWN_DATA_NOTFN:
      {
         if (msgType != EGT_GTPC_MSG_DD_NOTFN_FAIL_IND && 
                  msgType != EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK)
         {
            ret = RFAILED;
         }
         break;
      }
      case EGT_GTPC_MSG_MB_CMND:
      {
         if (msgType != EGT_GTPC_MSG_MBFAIL_IND && 
                   msgType != EGT_GTPC_MSG_UB_REQ)
         {
            ret = RFAILED;
         }
         break;
      } 
      case EGT_GTPC_MSG_BR_CMND:
      {
         if (msgType != EGT_GTPC_MSG_BR_FAIL_IND && 
                        msgType != EGT_GTPC_MSG_UB_REQ && 
                        msgType != EGT_GTPC_MSG_DB_REQ && 
                        msgType != EGT_GTPC_MSG_CB_REQ)
         {
            ret = RFAILED;
         }
         break;
      } 
      case EGT_GTPC_MSG_DB_CMND:
      {
         if (msgType != EGT_GTPC_MSG_DB_FAIL_IN && 
               msgType != EGT_GTPC_MSG_DB_REQ)
         {
            ret = RFAILED;
         }
         break;
      }
      default:
      {
         ret = RFAILED;
         break;
      }
   }
   EG_RETVALUE(ret);
} /* end of egChkFailInd () */


/*************************************************************************
 *      Fun:   egSaveMsgRemDetails
 *
 *      Desc:  The function egSaveMsgRemDetails save the transport details
 *             of the incoming GTP message in a hash list in tunnel Cb.
 *             The key to the hash list is the sequence number of the 
 *             incoming message.
 *
 *      Ret:
 *              - ROK - If the procedure was successful
 *              - ROUTRES - If the procedure failed due to memory failure.
 *              - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PRIVATE S16 egSaveMsgRemDetails
(
EgUSapCb          *usap,
CmHashListCp      *incReqCp,
EgIncReq          *incReqNode,
EgEvnt            *event
)
#else
PRIVATE S16 egSaveMsgRemDetails (usap, incReqCp, incReqNode, event)
EgUSapCb          *usap;
CmHashListCp      *incReqCp;
EgIncReq          *incReqNode;
EgEvnt            *event;
#endif
{ 
   S16 ret;

   EG_TRC2( egSaveMsgRemDetails );

   ret = ROK;

   EG_ALLOC(&incReqNode, sizeof(EgIncReq)) 
   if (!incReqNode) 
   { 
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,
               "Failed to allocate memory for new tunnel endpoint Cb.")); 
      /* delete egMsg and event */
      EG_FREE_GM_MSG(event->u.egMsg);
      egCmFreeEvent(event); 
      EG_RETVALUE(ROUTRES); 
   } 
 
   incReqNode->seqNum = event->seqNumber; 
   incReqNode->reqType = event->u.egMsg->msgHdr.msgType; 
/*eg004.201 Added for eGTPC PSF Upgrade*/
#ifdef HW
	incReqNode->localTeid = event->lclTeid.val;
#endif
   egCpyCmTptAddr(&(incReqNode->remAddr), &(event->remAddr)); 
   egCpyCmTptAddr(&(incReqNode->locAddr), &(event->localAddr)); 
 
   ret = cmHashListInsert(incReqCp, (PTR)incReqNode,  
         (U8 *)&(incReqNode->seqNum), sizeof(U32) ); 
   /* eg012.201 :Removed extra check ROKDUP for return,fix for ccpu00129790 */ 
   if(ret != ROK) 
   { 
      /* Duplicate requests fail here and will be dropped. */
      EG_FREE(incReqNode, sizeof(EgIncReq)); 
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp, 
               "Failed to insert into hash list for incoming request.")); 
      /* delete egMsg and event */
      EG_FREE_GM_MSG(event->u.egMsg);
      egCmFreeEvent(event); 
      SEND_HASH_FAIL_ALARM(); 
   }/* end of if */ 
/*eg004.201 Add mapping for a new IncReqCb and update incReqCb on STANDBY*/
#ifdef HW
   /* ccpu00138513, ccpu00138370 */
   else
   {
   HW_ADD_MAPPING(HW_EG_INCREQCB, incReqNode);
   HW_DBG_INFO(0,0, (hwp, "\n PSF Addmapping called for CbType=[%d],"
                 " TeidValue=[%ld], Sequence Value=[%ld]",
                 HW_EG_INCREQCB,incReqNode->localTeid,incReqNode->seqNum));

   HW_RTUPD_ADD(HW_EG_INCREQCB, incReqNode);
   HW_DBG_INFO(0,0, (hwp, "\n Runtime Update for CbType=[%d],"
                 " TeidValue=[%ld], Sequence Value=[%ld]",
                 HW_EG_INCREQCB,incReqNode->localTeid,incReqNode->seqNum));
   }
#endif
   EG_RETVALUE( ret); 
} /* end of egSaveMsgRemDetails() */

/*************************************************************************
 *      Fun:   egGetMsgRemDetails
 *
 *      Desc:  The function egGetMsgRemDetails gets the transport details
 *             of the incoming GTP message in a hash list in tunnel Cb.
 *             The key to the hash list is the sequence number of the 
 *             incoming message.
 *
 *      Ret:
 *              - ROK - If the procedure was successful
 *              - ROUTRES - If the procedure failed due to memory failure.
 *              - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PRIVATE S16 egGetMsgRemDetails
(
EgUSapCb *usap,
CmHashListCp *incReqCp,
EgIncReq *incReqNode,
EgEvnt *event,
EgtMsgCat msgCat
)
#else
PRIVATE S16 egGetMsgRemDetails (usap, incReqCp, incReqNode,
                                        event, msgCat)
EgUSapCb *usap;
CmHashListCp *incReqCp;
EgIncReq *incReqNode;
EgEvnt *event;
EgtMsgCat msgCat;
#endif
{ 
   S16 ret;

   EG_TRC2(egGetMsgRemDetails );


   ret = ROK;

   ret = cmHashListFind(incReqCp, (U8 *)&event->seqNumber, 
         sizeof(U32), 0,(PTR *)&incReqNode); 

   if(ret != ROK) 
   { 
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp, 
               "The Message %d does not match with the sequence number of any\
               message in inc req list.", event->u.egMsg->msgHdr.msgType)); 
         event->resultType = EGT_ERR_MSG_NO_MATCH; 
      egSendErrInd(event, usap); 
      EG_RETVALUE( RFAILED); 
   }/* end of if */ 

   /* validate if response type is matching */ 
   if(event->u.egMsg->msgHdr.msgType != \
              egGetAssocMsg((EgtMsgType)incReqNode->reqType)) 
   { 
      /* For command the egGetAssocMsg matches to a request.
         Hence we need special handling for failure indication
         for command. */
      ret = egChkFailInd(event->u.egMsg->msgHdr.msgType, incReqNode->reqType);
      if ( ret != ROK)
      { 
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp, 
                  "The Message %d does not match with the message %d.", 
                  event->u.egMsg->msgHdr.msgType, incReqNode->reqType)); 
         event->resultType = EGT_ERR_MSG_NO_MATCH; 
         egSendErrInd(event, usap); 
         EG_RETVALUE( RFAILED); 
      }

   }/* end of if req != rsp*/ 

   if(msgCat != EG_GTPC_MSGCAT_REQUEST)
   {
      /*set remote details in event from the hash list*/ 

      egCpyCmTptAddr(&(event->remAddr), &(incReqNode->remAddr)); 
      egCpyCmTptAddr(&(event->localAddr), &(incReqNode->locAddr)); 
      event->seqNumber = incReqNode->seqNum; 
   }

   cmHashListDelete(incReqCp, (PTR)incReqNode); 
/*eg004.201 Delete mapping for IncReqCb and update incReqCb on STANDBY when an incReqCb
 * is deleted on ACTIVE node on receving the associated signalling response from peer
 */
#ifdef HW
   HW_RTUPD_DEL(HW_EG_INCREQCB, incReqNode);

   HW_DBG_INFO(0,0, (hwp, "\n PSF Runtime Update called with action Delete CbType=[%d],"
                 " TeidValue=[%ld], Sequence Value=[%ld]",
                 HW_EG_INCREQCB,incReqNode->localTeid,incReqNode->seqNum));

   HW_DEL_MAPPING(HW_EG_INCREQCB, incReqNode);
   HW_DBG_INFO(0,0, (hwp, "\n PSF Delete Mapping for CbType=[%d],"
                 " TeidValue=[%ld], Sequence Value=[%ld]",
                 HW_EG_INCREQCB,incReqNode->localTeid,incReqNode->seqNum));
#endif
   EG_FREE(incReqNode, sizeof(EgIncReq)); 
   EG_RETVALUE (ret); 
} /* end of egGetMsgRemDetails */

/*************************************************************************
 *      Fun:   egTunnInit
 *
 *      Desc:  The function egTunnInit initializes the global array of teids.
 *
 *      Ret:
 *              - ROK - If the procedure was successful
 *              - ROUTRES - If the procedure failed due to memory failure.
 *              - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunnInit
(
Void
)
#else
PUBLIC S16 egTunnInit (Void)
#endif
{
   S16 ret;

   EG_TRC2(egTunnInit );

   ret = ROK;

   EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,"Initializing tunnel management module"));
   /*
    *  Allocate memory for the global list of messages update the details. *
    *  Used to validate incoming and outgoing messages.                    *
    */
   ret = egInitTeidArr();
   EG_RETVALUE( ret);
} /* End of egTunnInit() */

/*************************************************************************
 *      Func:  egInitEchoUsap
 *
 *      Desc:  The function egInitEchoUsap initializes the hash list in the usap
 *         meant for Echo mechanism.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egInitEchoUsap
(
EgUSapCb *usap     /*Pointer to the usap control block meant for Echo*/
)
#else
PUBLIC S16 egInitEchoUsap (usap)
EgUSapCb *usap;   /*Pointer to the usap control block meant for Echo*/
#endif
{
   S16 ret;
   U16 hashBin = 32;

   EG_TRC2(egInitEchoUsap);

   ret = ROK;
   /*
    * initialize inc req hash list for incoming echo requests *
    */
   /***********************************************************
    * eg007.201: Since incReqCp won't be storing a large set of data
    * the bin size is reduced from 1024 to leser value 
    * ************************************************************/
   ret = cmHashListInit(&usap->incReqCp, hashBin, EG_HLOFFSET,
         FALSE, CM_HASH_KEYTYPE_CONID, egCb.init.region,
         egCb.init.pool);
   if(ret != ROK)
   {
      SEND_HASH_FAIL_ALARM();
   }/* end of if */

   usap->cfg.echo = TRUE;
   EG_RETVALUE(ret);

} /* End of egInitEchoUsap() */

/*************************************************************************
 *      Func:  egTunnDeInit
 *
 *      Desc:  The function egTunnDeInit un-initializes the global array of teids.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunnDeInit
(
Void
)
#else
PUBLIC S16 egTunnDeInit (Void)
#endif
{
   U16 idx; 

   EG_TRC2(egTunnDeInit);

   EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
            "De Initializing tunnel management module"));
   /*
    * Deallocate all the memory maintained for TEID generation and  *
    * Tunnel Cb maintainance.                                       *
    */
   egDeInitTeidArr();
   /* uninitialize the hash list in echo usap */
   for (idx = 0; idx < egCb.genCfg.nmbUSaps; idx++)
   {
      if(egCb.egtSapLst[idx] != NULLP)
      {
         if(egCb.egtSapLst[idx]->cfg.echo == TRUE)
        {
            egDeInitEchoUsap(egCb.egtSapLst[idx]);
            /* eg007.201: Multiple Usap are possible when Multi-IP is active */
#ifndef EG_MULTI_IP
            /* There can be only on echo usap */
            break;
#endif
        }
      }
   }

   EG_RETVALUE( ROK);

} /* End of egTunnDeInit() */

/*************************************************************************
 *      Func:  egDeInitEchoUsap
 *
 *      Desc:  The function egDeInitEchoUsap un-initializes the hash 
 *              list in the usap meant for Echo mechanism.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egDeInitEchoUsap
(
EgUSapCb *usap      /* pointer to the usap control block meant for Echo */
)
#else
PUBLIC S16 egDeInitEchoUsap (usap)
EgUSapCb *usap;     /* the pointer to the usap control block meant for Echo */
#endif
{
   S16 ret;
   EgIncReq *incReqNode;

   EG_TRC2(egDeInitEchoUsap);

   ret = ROK;
   incReqNode = NULLP;

   while (cmHashListGetNext (&usap->incReqCp, (PTR) NULLP,
            (PTR *) &incReqNode) == ROK)
   {   
      /*****************************************
       * Delete the incReqNode from the hash list * 
       *****************************************/
      cmHashListDelete (&usap->incReqCp, (PTR) incReqNode);

      /*******************************
       * Free the memory for incReqNode *
       *******************************/
      EG_FREE(incReqNode, sizeof(EgIncReq));
   }
   cmHashListDeinit(&(usap->incReqCp));

   EG_RETVALUE(ret);

} /* End of egDeInitEchoUsap() */

/*************************************************************************
 *      Fun:   egTunReceive
 *
 *      Desc:  The function  egTunReceive is called by transport when ever a
 *               GTPC message is received.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTunReceive
(
EgEvnt *event           /* the event representing the incoming message */
)
#else
PUBLIC S16 egTunReceive (event)
EgEvnt *event;           /* the event representing the incoming message */
#endif
{
   S16 ret;
   EgTECCb *teCCb;
   EgUSapCb *usap;
   EgUSapCb *echoSap  = NULLP;
   TknU32 transId;
#ifdef EGTP_C_PTH_MGT
   /*-- eg003.201 : merged from eg006.102 & destCb init to NULL --*/
   EgDestCb *destCb=NULLP;
#endif

   EG_TRC2(egTunReceive);

   teCCb =NULLP;
   usap = NULLP;
   ret = ROK;

   cmMemset((U8 *)&transId, 0 , sizeof(TknU32));

   /* This function always returns ROK.
      In case of error, all the neccessary cleanup in done in
      internal functions. The caller, tpt has no cleanup needed for now.
     */

   /*------------------- Get Usap ----------------*/
   if(event->srvCb->cfg.isDfltSrv == TRUE)
   {
      /* Get usap through received interface */
         usap = egGetUsapFrmIntf((EgtIntf)event->intfType);
   }
   else
   {
      usap = event->srvCb->uSapCb;
   }
      
   if(!usap)
   {
      /* eg006.201: Merge from eg010.102 --*/
      EG_RETVALUE(RFAILED);
   }

#ifdef EG_MULTI_IP
   /* eg007.201: obtain EchoSap by referencing default srv in TSap */
   echoSap = usap->tSapCb->dftlTptSrv->uSapCb;
#else
   echoSap = egCb.echoSap;
#endif

   /* Handle echo first */
   switch(event->u.egMsg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_ECHO_REQ:
         {
#ifndef EGTP_C_PTH_MGT
            egHandleIncEchoReq(echoSap, event);
#else
            if(egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb)
            {
               egSendPathRsp(event);
               /*eg012.201:memory leak fix for ccpu00128780*/
               EG_FREE_GM_MSG(event->u.egMsg);
               egCmFreeEvent(event);

            }
            else if(echoSap)
            {
               egHandleIncEchoReq(echoSap, event);
            }
#endif
            EG_RETVALUE(ret);
         }
         break;

      case EGT_GTPC_MSG_ECHO_RSP:
         {
#ifndef EGTP_C_PTH_MGT
            if(usap == NULLP)
            {
               usap = echoSap;
            }
            egHandleIncEchoRsp(usap, event);
#else
            if(egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb)
            {
               CmTptAddr  destIpAddr;
#ifdef EG_MULTI_IP
               /*-- eg007.201: Obtain tSapCb associated from SrvCb in USap --*/
               EgTSapCb     *tSapCb = usap->tSapCb;
#endif

               /* eg006.201: Init destIpAddr to zero  */
               cmMemset((U8 *)&destIpAddr, 0 ,sizeof(CmTptAddr)); 
               egCpyCmTptAddr(&destIpAddr, &event->remAddr);
               EG_SET_PORT(destIpAddr, EG_ZERO);
#ifdef EG_MULTI_IP
               /* eg007.201: Perform Hash search on dstCbList in tSapCb */
               ret = cmHashListFind(&tSapCb->dstCbList, (U8 *)&(destIpAddr),\
                                sizeof(CmTptAddr),0,(PTR *)&destCb);
#else
               ret = cmHashListFind(&egCb.dstCbList, (U8 *)&(destIpAddr),\
                                sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif
               if(destCb != NULLP)
               {
                  /***************************************************************
                   * eg006.201: Since we received the Response from a destination
                   * which was previously unreachable, Send out Status Indication to
                   * Application &  Alarm indicating that Path recovery layer 
                   **************************************************************/
                  if(!destCb->destReachable)
                  {
                     EgUsta         sta;  /* unsolicited status diagnostics structure */
                     EgPathStatus  *status;

                     EG_ALLOC(&status, sizeof(EgPathStatus));
                     if (NULLP == status)
                     {
                        /* eg012.201: added check for failure */
                        EG_DBG_ERR (0, 0, (egp, "\n Memory Allocation Failed"));
                        EG_RETVALUE(RFAILED);
                     }
                     status->pathInfo.staType = EGT_STATYPE_PATH;
                     status->pathInfo.s.pathState.pres = PRSNT_NODEF;
                     status->pathInfo.s.pathState.val  = EGT_STA_PATH_EST_SUCCESS;
                     egCpyCmTptAddr(&(status->pathInfo.u.localAddr), &destIpAddr);
                     status->cause        = EGT_CAUSE_N3_REQ_EXCEEDED;
                     status->seqNum.pres  = PRSNT_NODEF;
                     status->message.pres = PRSNT_NODEF;
                     status->message.val  = EGT_GTPC_MSG_ECHO_RSP;
                     status->dgn.pres     = NOTPRSNT;
                     EgUiEgtStaInd(&(usap->pst),usap->suId, \
                                        EGT_STA_PATH_EST_SUCCESS, status);

                     EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,"Destination IP reachable"));

                     cmMemset( (U8* )&sta, 0, sizeof(EgUsta));
                     sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
                     egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr),&destIpAddr);
                     egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
                           LEG_CAUSE_DST_IP_REACHABLE, &sta.dgn);
                  } 

                  destCb->destReachable = TRUE;
                  /* eg007.201: Free memory allocated for egMsg & event*/
                  EG_FREE_GM_MSG(event->u.egMsg);
                  egCmFreeEvent(event);
               }
            }
            else
            {
               /* eg009.201: Klockwork fix-UnReachable code */
               /* eg007.201: send Echo Response to the Usap */
               egHandleIncEchoRsp(usap, event);
            }
#endif
            EG_RETVALUE(ret);
         }
         break;
         /*ccpu00137353*/
      case  EGT_GTPC_MSG_DPDN_CON_SET_REQ:
         /* fix for ccpu00139013 */
      case  EGT_GTPC_MSG_ID_REQ:
         /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
      case  EGT_GTPC_MSG_PGW_RST_NOTFN:
      case  EGT_GTPC_MSG_DWN_DATA_NOTFN:
      case  EGT_GTPC_MSG_SSPND_NOTFN:
#endif
         {
            if(event->u.egMsg->msgHdr.teid == 0)
         {
            EgUiEgtSigInd(&(usap->pst), usap->suId,
                           &(event->localAddr), &(event->remAddr),event->lclTeid, event->u.egMsg);
            egCmFreeEvent(event);
            EG_RETVALUE( ROK);
         }
         }
         break;   
      case  EGT_GTPC_MSG_DPDN_CON_SET_RSP:
         /* fix for ccpu00139013 */
      case  EGT_GTPC_MSG_ID_RSP:
#ifdef EG_REL_AC0
      case  EGT_GTPC_MSG_PGW_RST_NOTFN_ACK:
      case  EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK:
      case  EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
      case  EGT_GTPC_MSG_SSPND_ACK:
#endif
         {
            if(event->u.egMsg->msgHdr.teid == 0)
         {
            EgUiEgtSigCfm(&(usap->pst), usap->suId,
                           &(event->localAddr), &(event->remAddr),transId,event->lclTeid, event->u.egMsg);
            egCmFreeEvent(event);
            EG_RETVALUE( ROK);
         }
         }
         break;   
      default:
         break;
   }

   /* the message is not received on a tunnel */
   if((event->lclTeid.pres != PRSNT_NODEF) || (event->lclTeid.val == 0))
   {
      /* try to create a tunnel and then indicate to the user */
      egTunCreateReceive(usap, event);
      EG_RETVALUE( ret );
   }
   else
   {
      /* invoke FSM or appropriate function */
      switch(event->u.egMsg->msgHdr.msgType)
      {

         case EGT_GTPC_MSG_CS_RSP:
            /* get the tunnel context */
            GET_TUNN_FRM_EVENT(event, (&teCCb));
            egTunnFsm[teCCb->state][EG_TUN_EVENT_INCSESSRSP](teCCb,
                     event);
            break;

         default:
            /* get the tunnel context */
            GET_TUNN_FRM_EVENT(event, &teCCb);
            egTunnFsm[teCCb->state][EG_TUN_EVENT_INC_MSG](teCCb,
                     event);
            break;
      }

      EG_RETVALUE( ret );
   }/* end of if */
} /* End of egTunReceive() */

/*************************************************************************
 *      Fun:   egTunSend
 *
 *      Desc:  The function egTunSend sends a GTP-C message through the tunnel
 *               FSM to the transport. For a fresh Create Session Request it 
 *               creates a tunnel. Echo Request and Responses are also handled
 *               independent of tunnel FSM.
 *
 *      Ret: 
 *                - ROK     - If the procedure was successful.
 *                - ROUTRES - If the procedure failed due to memory failure.
 *                - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunSend
(
EgUSapCb *usap,         /* the pointer to the usap control block representing */
EgEvnt *event           /* event representing the GTP-C message  */
)
#else
PUBLIC S16 egTunSend (usap, event)
EgUSapCb *usap;         /* the pointer to the usap control block representing */
EgEvnt *event;           /* the event representing the GTP-C message . */
#endif
{
   S16 ret = ROK;
   EgTECCb **teCCb;

   EG_TRC2(egTunSend);

   teCCb = NULLP;
   ret = ROK;
   
   /************************************************** 
    * eg006.201: verify that SeqNumber does not exceeds 24BIT
    ****************************************************/
   if ( event->seqNumber > EG_MAX_SEQ_NUM )
   {
      event->resultType = EGT_ERR_INV_SEQ_NUM;
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Message Sequence Number  %ld is greater than 24Bit",
               event->seqNumber ));
      egSendErrInd(event, usap);
      EG_RETVALUE(RFAILED);
   }

   /* handle echo first */
   switch(event->u.egMsg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_ECHO_REQ:
#ifndef EGTP_C_PTH_MGT
         ret = egHandleOutEchoReq(usap, event);
#else  
         if(egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb)
         {
            EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Echo Request will only be sent by the layer"));
         }
         else
         {
            ret = egHandleOutEchoReq(usap, event);
         }
#endif
         EG_RETVALUE(ret);
         break;

      case EGT_GTPC_MSG_ECHO_RSP:
#ifndef EGTP_C_PTH_MGT
         ret = egHandleOutEchoRsp(usap, event);
#else
         if(egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb)
         {
            EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Echo Response will only be sent by the layer"));
         }
         else
         {
            ret = egHandleOutEchoRsp(usap, event);
         }
#endif
         EG_RETVALUE(ret);
         break;
      /* ccpu00137353 */     
      case EGT_GTPC_MSG_DPDN_CON_SET_REQ:
         ret = egTransConnSigReq (usap, event);
         EG_RETVALUE(ret);
         break;
      /* ccpu00137353 */     
      case EGT_GTPC_MSG_DPDN_CON_SET_RSP:
         ret = egTransConnSigRsp (usap, event);
         EG_RETVALUE(ret);
         break;

      default: 
         /*************************************************************************
          * eg006.201: Send Error to application in case Message (other than Echo Req/Resp)
          * arrives with Seq num reserved for Echo Req/Rsp 
          *************************************************************************/
#ifdef EGTP_C_PTH_MGT
		  /* eg012.201 : msgType check for path mgmnt,fix for ccpu00124404 */
          if ( ( ( EgMsgCatDb[ event->u.egMsg->msgHdr.msgType ] == EG_GTPC_MSGCAT_REQUEST ) ||
                ( EgMsgCatDb[ event->u.egMsg->msgHdr.msgType ] == EG_GTPC_MSGCAT_NOTFN) ) &&
            ((event->seqNumber >= egCb.genCfg.echoSeqNumStart) &&
               (event->seqNumber <= egCb.genCfg.echoSeqNumEnd )) )
         {
            event->resultType = EGT_ERR_INV_SEQ_NUM;
            EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Message sent with incorrect Sequence Number %ld",
                     event->seqNumber ));
            egSendErrInd(event, usap);
            EG_RETVALUE(RFAILED);
         }
#endif
         break;
   }

   /* check if the user intends to create a new tunnel using the current
      request */
   /*-- eg006.201: create tunnel data when crtFlag is DUMMY --*/ 
#ifdef EGTP_C_PTH_MGT
   if(event->crtFlag.pres == PRSNT_NODEF &&
         (event->crtFlag.val == TRUE ||event->crtFlag.val == EG_DUMMY_TUNN))
#else
   if(event->crtFlag.pres == PRSNT_NODEF && event->crtFlag.val == TRUE)
#endif
   {
      /* create a new tunnel and then send the message */
      EG_RETVALUE( egTunCreateSend(usap, event));
   }
   else
   {
      /* get the tunnel context */
      if((event->lclTeid.pres != PRSNT_NODEF) || (event->lclTeid.val == 0))
      {
         /* The user is not creating the tunnel
            and the TEID is not provided */
         teCCb = NULLP;
         event->resultType = EGT_ERR_UNKNOWN_TEID_MSG;

         /* eg006.201 : put remote Teid to indicate error --*/ 
         event->lclTeid.val = event->u.egMsg->msgHdr.teid;

         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Message sent on unknown teid"));
         egSendErrInd(event, usap);
         EG_RETVALUE(RFAILED);
      }
      else
      {
         teCCb = egGetTunnelCb(event->lclTeid.val);
         if(teCCb == NULLP)
         {
            /* The TEID provided in out of range */
            event->resultType = EGT_ERR_UNKNOWN_TEID_MSG;

            /* eg006.201 : put remote Teid to indicate error --*/ 
            event->lclTeid.val = event->u.egMsg->msgHdr.teid;

            EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Message sent on unknown teid"));
            egSendErrInd(event, usap);
            EG_RETVALUE(RFAILED);
         }
      }/* end of if */

      if(*teCCb != NULLP)
      {
#ifdef EG_PIGGYBACK_SUPP
         /* eg010.201:Call Piggyback control function*/
         if((event->isitPbMsg == TRUE) || ((*teCCb)->piggyBackStatus == EG_PIGGYBACK_QUEUE))
         {
              /* eg012.201: check if the piggibacked message is a initial message with piggibacked bit set*/
            if((event->isitPbMsg == TRUE) && 
			   ((EgMsgCatDb[event->u.egMsg->msgHdr.msgType] == EG_GTPC_MSGCAT_REQUEST)))
            {
               ret=RFAILED;
            }
            else
            {
               ret = egTptHdlOutPBMsg(teCCb,event);
            }   
            /* Send Error Indication to application*/
            if(ret == RFAILED)
            {
               /* Send Error Indication to User for piggyback message failed */
               event->resultType = EGT_ERR_INV_PIGGY_MSG;
               event->lclTeid.val = event->u.egMsg->msgHdr.teid;

               EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Invalid Piggyback message"));
               egSendErrInd(event, usap);
               if ((*teCCb)->piggyBackStatus == EG_PIGGYBACK_QUEUE)
               {
                  EG_FREEMBUF ((*teCCb)->tempPiggyEvnt->u.mBuf);
                  egCmFreeEvent((*teCCb)->tempPiggyEvnt);
                  (*teCCb)->piggyBackStatus = EG_PIGGYBACK_NONE;
               }
               EG_RETVALUE(RFAILED);
            }
         }
#endif
         /* invoke FSM or appropriate function */
         switch(event->u.egMsg->msgHdr.msgType)
         {
            case EGT_GTPC_MSG_CS_RSP:
               EG_RETVALUE( egTunnFsm[(*teCCb)->state][EG_TUN_EVENT_OUTSESSRSP]
                     ((*teCCb), event));
               break;

            default:
               EG_RETVALUE( egTunnFsm[(*teCCb)->state][EG_TUN_EVENT_OUT_MSG](
                        (*teCCb), event));
               break;
         } /* ebd of switch (msgType) */
      }
      else
      {
         /* Tunnel Context is null */
         event->resultType = EGT_ERR_UNKNOWN_TEID_MSG;

         /* eg006.201 : put remote Teid to indicate error */ 
         event->lclTeid.val = event->u.egMsg->msgHdr.teid;

         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Message sent on unknown teid"));
         egSendErrInd(event, usap);
         EG_RETVALUE( RFAILED);

      }/* end of if teCCb != NULL*/
   }
} /* End of egTunSend() */

/*************************************************************************
 *      Fun:   egTunDelete
 *
 *      Desc:  The function egTunDelete is called by the user whenever the User wants
 *             destroy a tunnel context locally.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunDelete
(
EgUSapCb *usap,         /* USAP control block */
U32 localTeid,          /* the local TEID of the tunnel to be deleted. */
U32 transId             /* transaction ID given by the the user */
)
#else
PUBLIC S16 egTunDelete (usap, localTeid, transId)
EgUSapCb *usap;         /* USAP control block */
U32 localTeid;          /* the local TEID of the tunnel to be deleted. */
U32 transId;             /* the transaction ID given by the the user */
#endif
{
   S16 ret;
   EgTECCb *teCCb;
   EgtTunnState state;
   Bool found;

   EG_TRC2(egTunDelete);

   ret = ROK;
   teCCb = NULLP;
   state = EGT_TUN_STATE_IDLE;
   found = FALSE;

   /* If TEID is not available use transID to find the Cb.
    * This is performance intensive due to the search,
    * but the probability of this scenario is very very low. */

   if(localTeid == 0)
   {
      U32 idx;
      /* search through all tunnel endpoint Cbs for transId */
      for(idx = 0; idx < egCb.tnlCb.freeArrIdx; idx++)
      {
         /* search in array of arrays of tunnel endpoint Cbs */
         if(egCb.tnlCb.teCCbArr[idx] != NULLP)
         {
            U32 cnt;
            for(cnt = 0; cnt < egCb.genCfg.maxTunlArrSz; cnt++)
            {
               if(egCb.tnlCb.teCCbArr[idx][cnt] != NULLP)
               {
                  /* get tunnel endpoint Cb */
                  teCCb = egCb.tnlCb.teCCbArr[idx][cnt];
                  /* check for transId */
                  if(teCCb->transId == transId)
                  {
                     localTeid = teCCb->locTeid;
                     state = teCCb->state;
                     egTunnDelTECCb(teCCb);
                     found = TRUE;
#ifndef ALIGN_64BIT
                     EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                              "Tunnel with transId %ld, Teid %ld deleted.",
                              transId, localTeid));
#else
                     EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                              "Tunnel with transId %d, Teid %d deleted.",
                              transId, localTeid));
#endif
                     break;
                  } /* End of if transId */
               } /* End of if teCCbArr[cnt] */
            } /* end of for maxTunlArrSz */
         }/* end of if tnlCbLst[idx] */
         if(found == TRUE)
                break;
      } /* end of for freeArrIdx */
      if(found == FALSE)
         ret= RFAILED;
   } /* end of if localTeid == 0 */
   else
   {
      EgTECCb **teCCbIdx;
      teCCbIdx = NULLP;
      teCCbIdx = egGetTunnelCb(localTeid);
      if(teCCbIdx == NULLP)
      {
         ret = RFAILED;
      }
      else
      {
         teCCb = *teCCbIdx;
         if(teCCb != NULLP)
         {
            state = teCCb->state;
#ifndef ALIGN_64BIT
            EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                     "Tunnel with transId %ld, Teid %ld deleted.",
                     transId, localTeid));
#else
            EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                     "Tunnel with transId %d, Teid %d deleted.",
                     transId, localTeid));
#endif
            egTunnDelTECCb(teCCb);
         }
         else
         {
            ret = RFAILED;
         }
      }
   }
   if (ret == ROK)
   {
      ret = egReleaseTeid(localTeid);
      (void) egTptProcessLclDel (usap->tptSrvCb, localTeid);
      if(state == EGT_TUN_STATE_CONNECTED)
      {
         egCb.genSts.numActvTnls--;
#ifndef ALIGN_64BIT
         EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                  "Active Teid %ld deleted. Total act tunnels %ld.",
                  localTeid, egCb.genSts.numActvTnls));
#else
         EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                  "Active Teid %d deleted. Total act tunnels %d.",
                  localTeid, egCb.genSts.numActvTnls));
#endif
      }/* end of if */
   }

   EG_RETVALUE( ret);
} /* End of egTunDelete() */


/*************************************************************************
 *      Fun:   egTunModify
 *
 *      Desc:  The function egTunModify is called by the user whenever the User wants
 *             to modify a tunnel context locally.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunModify
(
EgUSapCb    *usap,         /* USAP control block */
U32         localTeid,     /* the local TEID of the tunnel to be deleted. */
U32         transId,       /* transaction ID given by the the user */
CmTptAddr   oldDstIpAddr,
CmTptAddr   newDstIpAddr
)
#else
PUBLIC S16 egTunModify (usap, localTeid, transId, oldDstIpAddr, newDstIpAddr)
EgUSapCb    *usap;         /* USAP control block */
U32         localTeid;     /* the local TEID of the tunnel to be deleted. */
U32         transId;       /* the transaction ID given by the the user */
CmTptAddr   oldDstIpAddr;
CmTptAddr   newDstIpAddr;
#endif
{
   S16 ret;
   EgTECCb *teCCb;
   EgtTunnState state;
   Bool found;
#ifdef EGTP_C_PTH_MGT
   EgDestCb *destCb=NULLP;
#endif
   EG_TRC2(egTunModify);

   ret = ROK;
   teCCb = NULLP;
   state = EGT_TUN_STATE_IDLE;
   found = FALSE;

   /* If TEID is not available use transID to find the Cb.
    * This is performance intensive due to the search,
    * but the probability of this scenario is very very low. */

   if(localTeid == 0)
   {
      U32 idx;
      /* search through all tunnel endpoint Cbs for transId */
      for(idx = 0; idx < egCb.tnlCb.freeArrIdx; idx++)
      {
         /* search in array of arrays of tunnel endpoint Cbs */
         if(egCb.tnlCb.teCCbArr[idx] != NULLP)
         {
            U32 cnt;
            for(cnt = 0; cnt < egCb.genCfg.maxTunlArrSz; cnt++)
            {
               if(egCb.tnlCb.teCCbArr[idx][cnt] != NULLP)
               {
                  /* get tunnel endpoint Cb */
                  teCCb = egCb.tnlCb.teCCbArr[idx][cnt];
                  /* check for transId */
                  if(teCCb->transId == transId)
                  {
                     localTeid = teCCb->locTeid;
                     state = teCCb->state;
                     found = TRUE;
#ifndef ALIGN_64BIT
                     EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                              "Tunnel with transId %ld, Teid %ld deleted.",
                              transId, localTeid));
#else
                     EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                              "Tunnel with transId %d, Teid %d deleted.",
                              transId, localTeid));
#endif
                     egCpyCmTptAddr(&teCCb->destIpAddr, &newDstIpAddr);
                     break;
                  } /* End of if transId */
               } /* End of if teCCbArr[cnt] */
            } /* end of for maxTunlArrSz */
         }/* end of if tnlCbLst[idx] */
         if(found == TRUE)
                break;
      } /* end of for freeArrIdx */
      if(found == FALSE)
      {
         ret = RFAILED;
      }
   } /* end of if localTeid == 0 */
   else
   {
      EgTECCb **teCCbIdx;
      teCCbIdx = NULLP;
      teCCbIdx = egGetTunnelCb(localTeid);
      if(teCCbIdx == NULLP)
      {
         ret = RFAILED;
      }
      else
      {
         teCCb = *teCCbIdx;
         if(teCCb != NULLP)
         {
            state = teCCb->state;
#ifndef ALIGN_64BIT
            EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                     "Tunnel with transId %ld, Teid %ld deleted.",
                     transId, localTeid));
#else
            EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                     "Tunnel with transId %d, Teid %d deleted.",
                     transId, localTeid));
#endif
            egCpyCmTptAddr(&teCCb->destIpAddr, &newDstIpAddr);
         }
         else
         {
            ret = RFAILED;
         }
      }
   }

#ifdef EGTP_C_PTH_MGT
   if(ret == ROK)
   {
#ifdef EG_MULTI_IP
         /*eg007.201: get tSap from SrvCb associated with Usap */
         EgTSapCb *tSapCb = usap->tSapCb;
#endif
         EG_SET_PORT(oldDstIpAddr, EG_ZERO);
         EG_SET_PORT(newDstIpAddr, EG_ZERO);

#ifdef EG_MULTI_IP
         /* eg007.201: use dstCbList in TSAP cb to search for destination */
         ret = cmHashListFind(&tSapCb->dstCbList, (U8 *)&(oldDstIpAddr),\
                                        sizeof(CmTptAddr),0,(PTR *)&destCb);
#else 
         ret = cmHashListFind(&egCb.dstCbList, (U8 *)&(oldDstIpAddr),\
                                        sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif/*EG_MULTI_IP*/     
         if(destCb != NULLP)
         {
            destCb->numTunn --;
            /* eg006.201: Decrement the number of connected tunnels */
            destCb->connTunnCnt--;

            if (destCb->numTunn == 0)
            {
               EG_FREE(destCb, sizeof(EgDestCb));
#ifdef EG_MULTI_IP
               /* eg007.201: delete destCb from dstCbList in TSAP cb  */
               cmHashListDelete(&tSapCb->dstCbList, (PTR)destCb);
#else 
               cmHashListDelete(&egCb.dstCbList, (PTR)destCb);
#endif           
            }
         }
         else
         {
            EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,\
                     "Tunnel with transId %ld, Teid %ld modify failed.",\
                     transId, localTeid));
            RETVALUE(RFAILED);
         }
         destCb = NULLP;

#ifdef EG_MULTI_IP
         /* eg007.201: use dstCbList in TSAP cb to search for destination */
         ret = cmHashListFind(&(tSapCb->dstCbList), (U8 *)&(newDstIpAddr),\
                                        sizeof(CmTptAddr),0,(PTR *)&destCb);
#else
         ret = cmHashListFind(&egCb.dstCbList, (U8 *)&(newDstIpAddr),\
                                        sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif
         /* eg006.201: Corrected NULL check, destCb allocation should be done if its NULL */
         if(destCb == NULLP)
         {
            EG_ALLOC(&destCb, sizeof(EgDestCb));
            if(destCb == NULLP)
            {
               EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                     "Tunnel with transId %ld, Teid %ld modify failed.",
                     transId, localTeid));
               EG_RETVALUE(ROUTRES);
            }
            destCb->numTunn       = 1;
            destCb->echoReqSent   = 0;
            /*-- eg006.201: Init destReachable with TRUE */
            destCb->destReachable = TRUE;

            egCpyCmTptAddr(&destCb->destIpAddr, &newDstIpAddr);
#ifdef EG_MULTI_IP
            /*eg007.201: insert destCb in dstCbList at TSAP cb  */
            ret = cmHashListInsert(&(tSapCb->dstCbList), (PTR)destCb, \
                                   (U8*)&(destCb->destIpAddr), sizeof(CmTptAddr));
#else
            ret = cmHashListInsert(&(egCb.dstCbList), (PTR)destCb, \
                                   (U8*)&(destCb->destIpAddr), sizeof(CmTptAddr));
#endif
            if (ret != ROK)
            {
               EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,
                     "Tunnel with transId %ld, Teid %ld modify failed.",
                     transId, localTeid));
               EG_FREE(destCb, sizeof(EgDestCb));
               EG_RETVALUE(RFAILED);
            }
            if(!egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb)
            {
               ret = egSchedTmr(NULLP, EG_TMR_PATH_GLOBAL, TMR_START, \
                          egCb.genCfg.pathTmr.val);
               if (ret != ROK)
               {
                  egCb.echoTmrStarted = FALSE;
                  EG_DBG_ERR(0, 0, (egp,"\n  Starting of Echo Timer Failed"));
               }
               else
               {
                  egCb.echoTmrStarted = TRUE;
               }
            }
         }
         else
         {
            destCb->numTunn ++;
         }
   }
#endif


   EG_RETVALUE( ret);
} /* End of egTunModify() */

/*************************************************************************
 *      Fun:   egTunIndicate
 *
 *      Desc:  The function egTunIndicate is called by the transport or 
 *               encoder/decoder modules whenever the PCM has to be indicated with any
 *               failure events.
 *
 *      Ret: 
 *        - ROK     - If the procedure was successful.
 *        - ROUTRES - If the procedure failed due to memory failure.
 *        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunIndicate
(
EgUSapCb *usap,         /* the control block of the SAP representing the */
U32 localTeid,          /* the tunnel ID for which the indication is sent */
EgEvnt *event           /* contains the details about the failure */
)
#else
PUBLIC S16 egTunIndicate (usap, localTeid, event)
EgUSapCb *usap;         /* the control block of the SAP representing the */
U32 localTeid;          /* the tunnel ID for which the indication is sent */
EgEvnt *event;           /* contains the details about the failure */
#endif
{
   S16 ret;
   EgTECCb **teCCb;

   EG_TRC2(egTunIndicate);

   ret = RFAILED;
   teCCb = NULLP;

   if(localTeid == 0)
   {
#ifdef EG_MULTI_IP
        /*-- eg007.201: Obtain tSapCb associated from SrvCb in USap --*/
        EgTSapCb     *tSapCb = usap->tSapCb;
        egSendErrInd(event, tSapCb->dftlTptSrv->uSapCb);
#else
        egSendErrInd(event, egCb.tSapCb->dftlTptSrv->uSapCb);
#endif
   }
   teCCb = egGetTunnelCb(localTeid);
   if(teCCb != NULLP)
   {
      if(*teCCb != NULLP)
              ret = egTunnFsm[(*teCCb)->state][EG_TUN_EVENT_ERR_IND](
                    *teCCb, event);
   }/* end of if */

   EG_RETVALUE( ROK );
} /* End of egTunIndicate() */

/************************ Public Utilities ****************************/

/*************************************************************************
 *      Fun:   egGetMsgCategory
 *
 *      Desc:  The function egGetMsgCategory is an utility to get the category
 *         of a GTP-C message, namely, request, response, command,
 *         notification, acknowledge and indication.
 *
 *      Ret: The function returns the enumerated value of the message category.
 *   - EG_GTPC_MSGCAT_REQUEST,
 *   - EG_GTPC_MSGCAT_RESPONSE,
 *   - EG_GTPC_MSGCAT_NOTFN,
 *   - EG_GTPC_MSGCAT_ACKN,
 *   - EG_GTPC_MSGCAT_COMMAND,
 *   - EG_GTPC_MSGCAT_IND,
 *   - EG_GTPC_MSGCAT_OTHER
 *
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC EgtMsgCat egGetMsgCategory
(
EgtMsgType msgType              /* GTP-C message type. */
)
#else
PUBLIC EgtMsgCat egGetMsgCategory (msgType)
EgtMsgType msgType;             /* GTP-C message type. */
#endif
{
   EgtMsgCat ret;

   EG_TRC2(egGetMsgCategory);

   ret = EG_GTPC_MSGCAT_OTHER;

   if((msgType > EGT_GTP_MSG_INVALID) && (msgType < EGT_GTP_MSG_MAX))
        ret = EgMsgCatDb[msgType];

   EG_RETVALUE(ret);
} /* End of egGetMsgCategory() */

/*************************************************************************
 *      Fun:   egGetAssocMsg
 *
 *      Desc:  The function egGetAssocMsg is an utility to get the
 *             corresponsding GTP-C message type for one type.
 *
 *      Ret: The enumerated message type.
 *        - ZERO                  - If the input is invalid message
 *        - EGT_GTPC_MSG_ECHO_REQ - if input is EGT_GTPC_MSG_ECHO_RSP.
 *        - EGT_GTPC_MSG_MB_CMND  - if input is EGT_GTPC_MSG_UB_REQ.
 *        - so on...
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC EgtMsgType egGetAssocMsg
(
EgtMsgType msgType      /* type of the message whose associated msg to be returned */
)
#else
PUBLIC EgtMsgType egGetAssocMsg (msgType)
EgtMsgType msgType;    /* type of the message whose associated msg has to be returned */
#endif
{
   EG_TRC2(egGetAssocMsg);

   switch(msgType)
   {
      case EGT_GTPC_MSG_ECHO_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_ECHO_RSP);
         break;
      case EGT_GTPC_MSG_CS_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_CS_RSP);
         break;
      case EGT_GTPC_MSG_MB_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_MB_RSP);
         break;
#ifdef EG_REL_AC0
      case EGT_GTPC_MSG_MOD_ACC_BRE_REQ:
        EG_RETVALUE(EGT_GTPC_MSG_MOD_ACC_BRE_RSP);
         break;
      case EGT_GTPC_MSG_PGW_RST_NOTFN:
        EG_RETVALUE(EGT_GTPC_MSG_PGW_RST_NOTFN_ACK);
         break;
#endif
         /* eg010.201 : lpr_fix_ccpu00124570 */
      case EGT_GTPC_MSG_RAB_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_RAB_RSP);
         break;
      case EGT_GTPC_MSG_DS_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_DS_RSP);
         break;
      case EGT_GTPC_MSG_CB_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_CB_RSP);
         break;
      case EGT_GTPC_MSG_UB_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_UB_RSP);
         break;
      case EGT_GTPC_MSG_DB_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_DB_RSP);
         break;
      case EGT_GTPC_MSG_CN_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_CN_RSP);
         break;
      case EGT_GTPC_MSG_ID_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_ID_RSP);
         break;
      case EGT_GTPC_MSG_CNTXT_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_CNTXT_RSP);
         break;
      case EGT_GTPC_MSG_CNTXT_RSP:
         EG_RETVALUE( EGT_GTPC_MSG_CNTXT_ACK);
         break;
      case EGT_GTPC_MSG_FR_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_FR_RSP);
         break;
      case EGT_GTPC_MSG_FR_COMP_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_FR_COMP_ACK);
         break;
      case EGT_GTPC_MSG_ALRT_MME_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_ALRT_MME_ACK);
         break;
      case EGT_GTPC_MSG_UE_ACT_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_UE_ACT_ACK);
         break;
      case EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK);
         break;
      case EGT_GTPC_MSG_RC_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_RC_RSP);
         break;
      case EGT_GTPC_MSG_DTCH_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_DTCH_ACK);
         break;
      case EGT_GTPC_MSG_CF_TUNN_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_CF_TUNN_RSP);
         break;
      case EGT_GTPC_MSG_SSPND_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_SSPND_ACK);
         break;
      case EGT_GTPC_MSG_RSME_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_RSME_ACK);
         break;
      case EGT_GTPC_MSG_CIDF_TUNN_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_CIDF_TUNN_RSP);
         break;
         /*-- eg007.201: Added Associated msg for DIDF req msg --*/
      case EGT_GTPC_MSG_DIDF_TUNN_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_DIDF_TUNN_RSP);
         break;
      case EGT_GTPC_MSG_DWN_DATA_NOTFN:
         EG_RETVALUE( EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK);
         break;
      case EGT_GTPC_MSG_MB_CMND:
         EG_RETVALUE( EGT_GTPC_MSG_UB_REQ);
         break;
      case EGT_GTPC_MSG_DB_CMND:
         EG_RETVALUE( EGT_GTPC_MSG_DB_REQ);
         break;
      case EGT_GTPC_MSG_BR_CMND:
         EG_RETVALUE( EGT_GTPC_MSG_CB_REQ);
         break;
#if defined(EG_REL_930) || defined(EG_REL_AC0)
      case EGT_GTPC_MSG_MBMS_SESS_START_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_MBMS_SESS_START_RSP);
         break;
      case EGT_GTPC_MSG_MBMS_SESS_UPD_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_MBMS_SESS_UPD_RSP);
         break;
      case EGT_GTPC_MSG_MBMS_SESS_STOP_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_MBMS_SESS_STOP_RSP);
         break;
#endif /* EG_REL_930 */
         /*************************************
          * eg009.201 - Added for S2B and S2A *
          *************************************/
      case EGT_GTPC_MSG_DPDN_CON_SET_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_DPDN_CON_SET_RSP);
         break;

      case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ:
         EG_RETVALUE( EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP);
         break;

      default:
         EG_RETVALUE((EgtMsgType)ZERO);
         break;
   }
} /* End of egGetAssocMsg() */

/*************************************************************************
 *      Fun:   egValidateMsgInt
 *
 *      Desc:  The function egValidateMsgInt is a utility to evaluate if a 
 *        message type is valid on a given interface, originating entity
 *        and terminating entity.
 *
 *      Ret: -
 *        - TRUE  - If the message is valid.
 *        - FALSE - If the message is valid.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC Bool egValidateMsgInt
(
EgtMsgType msgType,             /* GTP-C message Type. */
LegEntity origEnt,              /* originating entity type. */
LegEntity termEnt,              /* terminating entity type. */
EgtIntf intf                    /* E-UTRAN interface type */
)
#else
PUBLIC Bool egValidateMsgInt (msgType, origEnt,
                                 termEnt, intf)
EgtMsgType msgType;             /* GTP-C message Type. */
LegEntity origEnt;              /* originating entity type. */
LegEntity termEnt;              /* terminating entity type. */
EgtIntf intf;                   /*E-UTRAN interface type  */
#endif
{
   EG_TRC2(egValidateMsgInt);

   switch(msgType)
   {
      case EGT_GTPC_MSG_ECHO_REQ:
         EG_RETVALUE( TRUE);
         break;

      case EGT_GTPC_MSG_ECHO_RSP:
         EG_RETVALUE( TRUE);
         break;

      case EGT_GTPC_MSG_VER_N_SUPP_IND:
         EG_RETVALUE( TRUE);
         break;

      default:
         break;
   }/* end of switch(msgType)*/

   switch(intf)
   {
      case EGT_GTP_INT_S11:
         {
            if(((origEnt == LEG_GTP_ENT_MME) || 
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt ==LEG_GTP_ENT_SGW) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_CS_REQ:
                  case EGT_GTPC_MSG_CB_RSP:
                  case EGT_GTPC_MSG_BR_CMND:
                  case EGT_GTPC_MSG_MB_REQ:
                  case EGT_GTPC_MSG_DS_REQ:
                  case EGT_GTPC_MSG_DB_RSP:
                  case EGT_GTPC_MSG_CN_REQ:
#ifdef EG_REL_AC0
                  case EGT_GTPC_MSG_MOD_ACC_BRE_REQ:
                  /*case EGT_GTPC_MSG_MOD_ACC_BRE_RSP:
                  case EGT_GTPC_MSG_PGW_RST_NOTFN: */
                  case EGT_GTPC_MSG_PGW_RST_NOTFN_ACK:
#endif
                  case EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK:
                  /* eg012.201: added exception messages */
                  case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
                  case EGT_GTPC_MSG_BR_FAIL_IND:
                  case EGT_GTPC_MSG_DB_FAIL_IN:
                  case EGT_GTPC_MSG_MB_CMND:
                  case EGT_GTPC_MSG_UB_RSP:
                  case EGT_GTPC_MSG_DB_CMND:
                  case EGT_GTPC_MSG_CIDF_TUNN_REQ:
                  /*-- eg007.201: Added interface validation for DIDF req msg --*/
                  case EGT_GTPC_MSG_DIDF_TUNN_REQ:
                  case EGT_GTPC_MSG_SSPND_NOTFN:
                  case EGT_GTPC_MSG_RSME_NOTFN:
                  case EGT_GTPC_MSG_CF_TUNN_REQ:
                  /* eg012.201 :RAB_REQ added here,fix for ccpu00124570*/
                     case EGT_GTPC_MSG_RAB_REQ:
                  /* ccpu00137353 */
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_DPDN_CON_SET_RSP:   
                  /* case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP:    */
                  case EGT_GTPC_MSG_TRC_SESS_ACTVN:
                  case EGT_GTPC_MSG_TRC_SESS_DEACTVN:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = MME and term = SGW */
            if(((origEnt == LEG_GTP_ENT_SGW) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_MME) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_CS_RSP:
                  /* eg010.201 : lpr_fix_ccpu00124570 */
                  case EGT_GTPC_MSG_CB_REQ: 
                  case EGT_GTPC_MSG_MB_REQ: 
                  case EGT_GTPC_MSG_BR_FAIL_IND:
                  case EGT_GTPC_MSG_MB_RSP:
                  case EGT_GTPC_MSG_DB_REQ:
                  case EGT_GTPC_MSG_DS_RSP:
                  case EGT_GTPC_MSG_CN_RSP:
                  case EGT_GTPC_MSG_STOP_PAGING_IND:
#ifdef EG_REL_AC0
                  /* case EGT_GTPC_MSG_MOD_ACC_BRE_REQ: */
                  case EGT_GTPC_MSG_MOD_ACC_BRE_RSP:
                  case EGT_GTPC_MSG_PGW_RST_NOTFN:
                  /* case EGT_GTPC_MSG_PGW_RST_NOTFN_ACK: */
#endif
                  case EGT_GTPC_MSG_DWN_DATA_NOTFN:
                  /* eg012.201: added exception case */
                  case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
                  case EGT_GTPC_MSG_MBFAIL_IND:
                  case EGT_GTPC_MSG_UB_REQ:
                  case EGT_GTPC_MSG_DB_FAIL_IN:
                  case EGT_GTPC_MSG_CIDF_TUNN_RSP:
                  /*-- eg007.201: Added interface validation for DIDF rsp msg --*/
                  case EGT_GTPC_MSG_DIDF_TUNN_RSP:
                  case EGT_GTPC_MSG_SSPND_ACK:
                  case EGT_GTPC_MSG_RSME_ACK:
                  case EGT_GTPC_MSG_CF_TUNN_RSP:
                  /* eg012.201 :RAB_RSP added here,fix for ccpu00124570 */
                  case EGT_GTPC_MSG_RAB_RSP:
                     EG_RETVALUE( TRUE);
                     break;
                  /* ccpu00137353 */
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_DPDN_CON_SET_RSP:   
                  /* case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP:   */
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGW term = MME */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S11 */
      case EGT_GTP_INT_S5_S8:
         {
            if(((origEnt == LEG_GTP_ENT_SGW) || 
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt ==LEG_GTP_ENT_PGW) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_CS_REQ:
                  case EGT_GTPC_MSG_CB_RSP:
                  case EGT_GTPC_MSG_BR_CMND:
                  case EGT_GTPC_MSG_MB_REQ:
                  case EGT_GTPC_MSG_DS_REQ:
                  case EGT_GTPC_MSG_DB_RSP:
                  case EGT_GTPC_MSG_MB_CMND:
                  case EGT_GTPC_MSG_UB_RSP:
                  case EGT_GTPC_MSG_DB_CMND:
                  case EGT_GTPC_MSG_CN_REQ:
                  /* ccpu00135337 : added msg for S5 interface */
                  case EGT_GTPC_MSG_SSPND_NOTFN:
                  case EGT_GTPC_MSG_RSME_NOTFN:
                  case EGT_GTPC_MSG_TRC_SESS_ACTVN:
                  case EGT_GTPC_MSG_TRC_SESS_DEACTVN:
                  /*ccpu00137353  */
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_DPDN_CON_SET_RSP:   
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP:   
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGW and term PGW*/
            if(((origEnt == LEG_GTP_ENT_PGW) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_SGW) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_CS_REQ:
                  case EGT_GTPC_MSG_CS_RSP:
                  case EGT_GTPC_MSG_CB_REQ:
                  case EGT_GTPC_MSG_BR_FAIL_IND:
                  case EGT_GTPC_MSG_MB_RSP:
                  case EGT_GTPC_MSG_DB_REQ:
                  case EGT_GTPC_MSG_DS_RSP:
                  case EGT_GTPC_MSG_MBFAIL_IND:
                  case EGT_GTPC_MSG_UB_REQ:
                  case EGT_GTPC_MSG_DB_FAIL_IN:
                  case EGT_GTPC_MSG_CN_RSP:
                  /* ccpu00135337 : added msg for S5 interface */
                  case EGT_GTPC_MSG_SSPND_ACK:
                  case EGT_GTPC_MSG_RSME_ACK:
                     EG_RETVALUE( TRUE);
                     break;
                  /* ccpu00137353 */
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_DPDN_CON_SET_RSP:   
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ:   
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP:   
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = PGW and term = SGW */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S5_S8 */

      case EGT_GTP_INT_S3:
         {
            if(((origEnt == LEG_GTP_ENT_SGSN) || 
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt ==LEG_GTP_ENT_MME) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_ID_REQ:
                  case EGT_GTPC_MSG_ID_RSP:
                  case EGT_GTPC_MSG_CNTXT_REQ:
                  case EGT_GTPC_MSG_CNTXT_RSP:
                  case EGT_GTPC_MSG_CNTXT_ACK:
                  case EGT_GTPC_MSG_SSPND_NOTFN:
                  case EGT_GTPC_MSG_FR_REQ:
                  /* CR 135348 fix */
                  case EGT_GTPC_MSG_FR_RSP:
                  case EGT_GTPC_MSG_FR_COMP_NOTFN:
                  case EGT_GTPC_MSG_FR_COMP_ACK:
                  case EGT_GTPC_MSG_RC_REQ:
                  case EGT_GTPC_MSG_RC_RSP:
                  case EGT_GTPC_MSG_DTCH_NOTFN:
                  case EGT_GTPC_MSG_DTCH_ACK:
                  case EGT_GTPC_MSG_ALRT_MME_ACK:
                  case EGT_GTPC_MSG_UE_ACT_NOTFN:
             case EGT_GTPC_MSG_SSPND_ACK:

                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = MME and term = SGW */
            if(((origEnt == LEG_GTP_ENT_MME) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_SGSN) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {

                  case EGT_GTPC_MSG_ID_REQ:
                  case EGT_GTPC_MSG_ID_RSP:
                  case EGT_GTPC_MSG_CNTXT_REQ:
                  case EGT_GTPC_MSG_CNTXT_RSP:
                  case EGT_GTPC_MSG_CNTXT_ACK:
                  case EGT_GTPC_MSG_FR_REQ:
                  case EGT_GTPC_MSG_FR_RSP:
                  case EGT_GTPC_MSG_CS_PGNG_IND:
                  case EGT_GTPC_MSG_SSPND_ACK:
                  case EGT_GTPC_MSG_RAN_INFO_RELAY:
                  case EGT_GTPC_MSG_FR_COMP_NOTFN:
                  case EGT_GTPC_MSG_FR_COMP_ACK:
                  case EGT_GTPC_MSG_ALRT_MME_NOTFN:
                  case EGT_GTPC_MSG_RC_REQ:
                  case EGT_GTPC_MSG_RC_RSP:
                  case EGT_GTPC_MSG_DTCH_NOTFN:
                  case EGT_GTPC_MSG_DTCH_ACK:
                  case EGT_GTPC_MSG_UE_ACT_ACK:
                  case EGT_GTPC_MSG_SSPND_NOTFN:

                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGW term = MME */

            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S3*/
      case EGT_GTP_INT_S4:
         {
            if(((origEnt == LEG_GTP_ENT_SGW) || 
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt ==LEG_GTP_ENT_SGSN) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                     case EGT_GTPC_MSG_CS_RSP:
                     /* eg010.201 : lpr_fix_ccpu00124570 */
                     case EGT_GTPC_MSG_RAB_RSP:
                     case EGT_GTPC_MSG_MB_RSP:
                     case EGT_GTPC_MSG_DS_RSP:
                     case EGT_GTPC_MSG_CN_RSP:
                     case EGT_GTPC_MSG_BR_FAIL_IND:
                     case EGT_GTPC_MSG_CB_REQ:
                     case EGT_GTPC_MSG_UB_REQ:
                     case EGT_GTPC_MSG_DB_REQ:
                     case EGT_GTPC_MSG_DWN_DATA_NOTFN :
           case EGT_GTPC_MSG_MBFAIL_IND:
                case EGT_GTPC_MSG_SSPND_ACK:
           case EGT_GTPC_MSG_RSME_ACK:
                case EGT_GTPC_MSG_CIDF_TUNN_RSP:
                case EGT_GTPC_MSG_DIDF_TUNN_RSP:
                case EGT_GTPC_MSG_DB_FAIL_IN:
                case EGT_GTPC_MSG_STOP_PAGING_IND:
#ifdef EG_REL_AC0
                     case EGT_GTPC_MSG_PGW_RST_NOTFN:
#endif /* EG_REL_AC0 */
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = MME and term = SGW */
            if(((origEnt == LEG_GTP_ENT_SGSN) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_SGW) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                     case EGT_GTPC_MSG_CS_REQ:
                     /* eg010.201 : lpr_fix_ccpu00124570 */
                     case EGT_GTPC_MSG_RAB_REQ: 
                     case EGT_GTPC_MSG_MB_REQ :
                     case EGT_GTPC_MSG_DS_REQ:
                     case EGT_GTPC_MSG_CN_REQ:
                     case EGT_GTPC_MSG_BR_CMND:
                     case EGT_GTPC_MSG_CB_RSP:
                     case EGT_GTPC_MSG_UB_RSP:
                     case EGT_GTPC_MSG_DB_RSP:
                     case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
                     case EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK:
                     case EGT_GTPC_MSG_TRC_SESS_ACTVN:
                     case EGT_GTPC_MSG_TRC_SESS_DEACTVN:
           case EGT_GTPC_MSG_MB_CMND:
           case EGT_GTPC_MSG_SSPND_NOTFN:
           case EGT_GTPC_MSG_RSME_NOTFN:
                case EGT_GTPC_MSG_CIDF_TUNN_REQ:
                case EGT_GTPC_MSG_DIDF_TUNN_REQ:
                case EGT_GTPC_MSG_DB_CMND:
#ifdef EG_REL_AC0
                     case EGT_GTPC_MSG_PGW_RST_NOTFN_ACK:
#endif /* EG_REL_AC0 */
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGW term = MME */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S4 */
      case EGT_GTP_INT_S10:
         {
            if(((origEnt == LEG_GTP_ENT_MME) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_MME) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {

                  case EGT_GTPC_MSG_ID_REQ:
                  case EGT_GTPC_MSG_ID_RSP:
                  case EGT_GTPC_MSG_CNTXT_REQ:
                  case EGT_GTPC_MSG_CNTXT_RSP:
                  case EGT_GTPC_MSG_CFG_TRAN_TUNN:
                  case EGT_GTPC_MSG_CNTXT_ACK:
                  case EGT_GTPC_MSG_FR_REQ:
                  case EGT_GTPC_MSG_FR_RSP:
                  case EGT_GTPC_MSG_FR_COMP_NOTFN:
                  case EGT_GTPC_MSG_FR_COMP_ACK:
                  case EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN:
                  case EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK:
                  case EGT_GTPC_MSG_RC_REQ:
                  case EGT_GTPC_MSG_RC_RSP:

                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGW term = MME */

            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S10 */
      case EGT_GTP_INT_S16:
         {
            if(((origEnt == LEG_GTP_ENT_SGSN) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_SGSN) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {

                  case EGT_GTPC_MSG_ID_REQ:
                  case EGT_GTPC_MSG_ID_RSP:
                  case EGT_GTPC_MSG_CNTXT_REQ:
                  case EGT_GTPC_MSG_CNTXT_RSP:
                  case EGT_GTPC_MSG_CNTXT_ACK:
                  case EGT_GTPC_MSG_FR_REQ:
                  case EGT_GTPC_MSG_FR_RSP:
                  case EGT_GTPC_MSG_FR_COMP_NOTFN:
                  case EGT_GTPC_MSG_FR_COMP_ACK:
                  case EGT_GTPC_MSG_RAN_INFO_RELAY:
                  case EGT_GTPC_MSG_RC_REQ:
                  case EGT_GTPC_MSG_RC_RSP:
                  case EGT_GTPC_MSG_SSPND_NOTFN:
                  case EGT_GTPC_MSG_SSPND_ACK:
                  case EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN:
                  case EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK:

                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGW term = MME */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S16 */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         case EGT_GTP_INT_SM:
         { 
            if(((origEnt == LEG_GTP_ENT_MBMSGW ) ||
                   (origEnt == LEG_GTP_ENT_UNKNWN))&& 
                  ((termEnt == LEG_GTP_ENT_MME) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_MBMS_SESS_START_REQ:
                  case EGT_GTPC_MSG_MBMS_SESS_UPD_REQ:
                  case EGT_GTPC_MSG_MBMS_SESS_STOP_REQ:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = MBMSGW term = MME */
            if(((origEnt == LEG_GTP_ENT_MME ) ||
                   (origEnt == LEG_GTP_ENT_UNKNWN))&& 
                  ((termEnt == LEG_GTP_ENT_MBMSGW ) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_MBMS_SESS_START_RSP:
                  case EGT_GTPC_MSG_MBMS_SESS_UPD_RSP:
                  case EGT_GTPC_MSG_MBMS_SESS_STOP_RSP:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = MME term = MBMSGW */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_SM */
         case EGT_GTP_INT_SN:
         { 
            if(((origEnt == LEG_GTP_ENT_MBMSGW ) ||
                   (origEnt == LEG_GTP_ENT_UNKNWN))&& 
                  ((termEnt == LEG_GTP_ENT_SGSN) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_MBMS_SESS_START_REQ:
                  case EGT_GTPC_MSG_MBMS_SESS_UPD_REQ:
                  case EGT_GTPC_MSG_MBMS_SESS_STOP_REQ:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = MBMSGW term = SGSN */
            if(((origEnt == LEG_GTP_ENT_SGSN ) ||
                   (origEnt == LEG_GTP_ENT_UNKNWN))&& 
                  ((termEnt == LEG_GTP_ENT_MBMSGW ) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_MBMS_SESS_START_RSP:
                  case EGT_GTPC_MSG_MBMS_SESS_UPD_RSP:
                  case EGT_GTPC_MSG_MBMS_SESS_STOP_RSP:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = SGSN term = MBMSGW */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_SN */
#endif /* EG_REL_930 */

         /*****************************
          * eg009.201 - Added for S2B *
          *****************************/
#ifdef  EG_REL_AC0
      case EGT_GTP_INT_S2B:
         {
            if(((origEnt == LEG_GTP_ENT_EPDG) || 
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt ==LEG_GTP_ENT_PGW) ||
                   (termEnt ==LEG_GTP_ENT_SGW) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {

                  case EGT_GTPC_MSG_CS_REQ:
                  case EGT_GTPC_MSG_DS_REQ:
                  case EGT_GTPC_MSG_CB_RSP:
                  case EGT_GTPC_MSG_MB_CMND:
                  case EGT_GTPC_MSG_UB_RSP:
                  case EGT_GTPC_MSG_DB_RSP:
                  case EGT_GTPC_MSG_TRC_SESS_ACTVN:
                  case EGT_GTPC_MSG_TRC_SESS_DEACTVN:
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:
                     /* Deleted as this needs to be sent only on s5/s8 interface
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ: */
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = PGW and term = ePDG */
            if(((origEnt == LEG_GTP_ENT_PGW) ||
                     (termEnt == LEG_GTP_ENT_SGW) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_EPDG) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_CS_RSP:
                  case EGT_GTPC_MSG_DS_RSP:
                  case EGT_GTPC_MSG_CB_REQ:
                  case EGT_GTPC_MSG_UB_REQ:
                  case EGT_GTPC_MSG_DB_REQ:
                  case EGT_GTPC_MSG_MBFAIL_IND:
                  case EGT_GTPC_MSG_DPDN_CON_SET_RSP:
                     /* Deleted as this needs to be sent only on s5/s8 interface
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP: */
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = PGW term = ePDG */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S2B */
#endif 
         /*****************************
          * eg009.201 - Added for S2A *
          *****************************/
#ifdef  EG_S2A_SUPP
      case EGT_GTP_INT_S2A:
         {
            if(((origEnt == LEG_GTP_ENT_TWAN) || 
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt ==LEG_GTP_ENT_PGW) ||
                   (termEnt ==LEG_GTP_ENT_SGW) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {

                  case EGT_GTPC_MSG_CS_REQ:
                  case EGT_GTPC_MSG_DS_REQ:
                  case EGT_GTPC_MSG_CB_RSP:
                  case EGT_GTPC_MSG_MB_CMND:
                  case EGT_GTPC_MSG_UB_RSP:
                  case EGT_GTPC_MSG_DB_RSP:
                  case EGT_GTPC_MSG_TRC_SESS_ACTVN:
                  case EGT_GTPC_MSG_TRC_SESS_DEACTVN:
                  case EGT_GTPC_MSG_DPDN_CON_SET_REQ:
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = PGW and term = TWAN */
            if(((origEnt == LEG_GTP_ENT_PGW) ||
                     (termEnt == LEG_GTP_ENT_SGW) ||
                     (origEnt == LEG_GTP_ENT_UNKNWN)) && 
                  ((termEnt == LEG_GTP_ENT_TWAN) ||
                   (termEnt == LEG_GTP_ENT_UNKNWN)))
            {
               switch(msgType)
               {
                  case EGT_GTPC_MSG_CS_RSP:
                  case EGT_GTPC_MSG_DS_RSP:
                  case EGT_GTPC_MSG_CB_REQ:
                  case EGT_GTPC_MSG_UB_REQ:
                  case EGT_GTPC_MSG_DB_REQ:
                  case EGT_GTPC_MSG_MBFAIL_IND:
                  case EGT_GTPC_MSG_DPDN_CON_SET_RSP:
                  case EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP:
                     EG_RETVALUE( TRUE);
                     break;
                  default:
                     break;
               }/* end of switch (msgType)*/
            }/* end of if orig = PGW term = ePDG */
            EG_RETVALUE( FALSE);
            break;
         }/* case EGT_GTP_INT_S2A */
#endif 
      default:
         EG_RETVALUE( FALSE);
         break;
   }/* end of switch(interface)*/
} /* End of egValidateMsgInt() */

/*************************************************************************
 *      Fun:   egGetIntfFrmTeid
 *
 *      Desc:  The function egGetIntfFrmTeid is a utility to get the interface
 *        type of a tunnel given its TEID.
 *
 *      Ret: -
 *       - EGT_GTP_INT_UNKNWN
 *       - EGT_GTP_INT_S5_S8
 *       - EGT_GTP_INT_S11
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC EgtIntf egGetIntfFrmTeid
(
U32 teid,
Bool crtFlag
)
#else
PUBLIC EgtIntf egGetIntfFrmTeid (teid)
U32 teid;
Bool crtFlag;
#endif
{
   EgTECCb **teCCb;
   EgtIntf ret;
   U32      tunnArr;
   U32      arrIdx;

   EG_TRC2(egGetIntfFrmTeid);

   ret = EGT_GTP_INT_UNKNWN;
   teCCb = NULLP;

   tunnArr = (teid/egCb.genCfg.maxTunlArrSz);
   arrIdx = teid % egCb.genCfg.maxTunlArrSz;

   if((egCb.tnlCb.arrNum > tunnArr) && egCb.tnlCb.teCCbArr[tunnArr])
   {
      if (!((crtFlag==0) && !egCb.tnlCb.teCCbArr[tunnArr][arrIdx]))
      {
      ret = egCb.tnlCb.teCCbArr[tunnArr][arrIdx]->intfType;
      }
   }/* end of if */

   EG_RETVALUE (ret);

} /* End of egGetIntfFrmTeid() */

/*************************************************************************
 *      Fun:   egTunnState
 *
 *      Desc:  The function egTunnState returns the state of a tunnel given
               the tunnel ID.
 *
 *      Ret: 
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed for internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC S16 egTunnState
(
U32 teid,               /* tunnel whose state is queried. */
EgtTunnState *state             /* state of the tunnel returned. */
)
#else
PUBLIC S16 egTunnState (teid, state)
U32 teid;               /* tunnel whose state is queried. */
EgtTunnState *state;            /* state of the tunnel returned. */
#endif
{
   EgTECCb **teCCb;
   S16 ret;

   EG_TRC2(egTunnState);

   ret = RFAILED;
   teCCb = NULLP;
   *state = EGT_TUN_STATE_IDLE;

   teCCb = egGetTunnelCb(teid);
   if(teCCb != NULLP)
   {
      if(*teCCb != NULLP)
      {
         *state = (*teCCb)->state;
         ret = ROK;
      }
   }/* end of if */
   EG_RETVALUE (ret);
} /* End of egTunnState() */

/*************************************************************************
 *      Fun:   egGetUsapFrmIntf
 *
 *      Desc:  The function egGetUsapFrmIntf returns a pointer to the usap
 *             control block given its EUTRAN interface type.
 *
 *      Ret: Pointer to the usap control block.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PUBLIC EgUSapCb *egGetUsapFrmIntf
(
EgtIntf intfType                /* GTP-C interface type. */
)
#else
PUBLIC EgUSapCb *egGetUsapFrmIntf (intfType)
EgtIntf intfType;               /* GTP-C interface type. */
#endif
{
   U16 idx;

   EG_TRC2(egGetUsapFrmIntf);

   for (idx = 0; idx < egCb.genCfg.nmbUSaps; idx++)
   {
      if(egCb.egtSapLst[idx] != NULLP)
      {
         if(egCb.egtSapLst[idx]->cfg.intfType == intfType)
            EG_RETVALUE( egCb.egtSapLst[idx]);
      }
   }
   EG_RETVALUE( NULLP);
} /* End of egGetUsapFrmIntf() */

/*************************************************************************
 *      Fun:   egSendErrInd
 *
 *      Desc:  The function egSendErrInd is an utility to send an error 
               indication to the user.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egSendErrInd
(
EgEvnt *event,     /* pointer to the event for which the error has occured */
EgUSapCb *usap          /* pointer to the SAp on which the error has occured*/
)
#else
PUBLIC Void egSendErrInd (event, usap)
EgEvnt *event;    /* pointer to the event for which the error has occured */
EgUSapCb *usap;         /* pointer to the SAp on which the error has occured*/
#endif
{
   EgErrEvnt errEvnt;
   EgtMsgCat msgCat; /*fix for ccpu00135658*/
   EG_TRC2(egSendErrInd);

   cmMemset( (U8* )&errEvnt, 0, sizeof(EgErrEvnt));

   errEvnt.localTeid = event->lclTeid.val;
   if(event->transId.pres == PRSNT_NODEF)
   {
      errEvnt.transId = event->transId.val;
   }
   else
   {
      errEvnt.transId = 0;
   }
   errEvnt.seqNo = event->seqNumber;
   errEvnt.errCause = (EgtErrType)event->resultType; 
   /* eg006.201 : send proper info into Err msg */
   errEvnt.failedIeInst =  event->failedIeInst;
   errEvnt.failedIeType =  event->failedIeType;

   errEvnt.failedMsgType = event->msgType;
   egCpyCmTptAddr(&(errEvnt.remAddr), &(event->remAddr));
   errEvnt.intfType = usap->cfg.intfType;
   /*start fix for ccpu00135658*/
   msgCat = egGetMsgCategory ((EgtMsgType)event->msgType);      
   if((msgCat == EG_GTPC_MSGCAT_REQUEST)&&(event->u.egMsg))
   {   
      if((event->lclTeid.pres != PRSNT_NODEF) || (event->lclTeid.val == 0))
      {
             errEvnt.remTeid=event->u.egMsg->msgHdr.teid;
      }
   }   
/*end fix for ccpu00135658*/
   EgUiEgtErrInd(&(usap->pst), usap->suId, &errEvnt);

   /* delete egMsg and event */
   EG_FREE_GM_MSG(event->u.egMsg);
   egCmFreeEvent(event);

   EG_RETVOID;
} /* End of egSendErrInd () */

/*eg010.201:Definition of outgoing Piggyback Handler*/
#ifdef EG_PIGGYBACK_SUPP
/************************************************************************
 *       Fun:   egTptHdlOutPBMsg                                        *
 *                                                                      *
 *       Desc: This function is used to check if the current message    *
 *             is a piggyback message or the piggyback message carrier  *
 *             If a PIGGYBACK carrier the message is buffered in Tunnel *
 *             Cb till the piggyback message arrives.                   *
 *                                                                      *
 *       Ret:   ROK   on success                                        *
 *               on error                                               *
 *                                                                      *
 *       Notes: --                                                      *
 *                                                                      *
 *       File:  eg_tpt.c                                                *
 * **********************************************************************/
#ifdef ANSI
PUBLIC S16 egTptHdlOutPBMsg
(
 EgTECCb    **teCCb,                         /* Tunnel Control Block */
 EgEvnt      *egCurrEvnt                    /* current event structure */ 
 )
#else
PRIVATE S16 egTptHdlOutPBMsg(teCCb,egEvnt)
 EgTECCb     **teCCb;                        /* Tunnel Control Block */
 EgEvnt       *egCurrEvnt;                   /* current event structure */ 
#endif
{
   S16         retVal;         /* Holds the return value */
   U8          msgType;

   EG_TRC2(egTptHdlOutPBMsg);

   retVal = RFAILED;
   msgType = egCurrEvnt->u.egMsg->msgHdr.msgType;

   if((*teCCb)->tempPiggyEvnt == NULL)
   {
      if((msgType == EGT_GTPC_MSG_CS_RSP) || (msgType == EGT_GTPC_MSG_CB_RSP))
      {
         (*teCCb)->piggyBackStatus = EG_PIGGYBACK_QUEUE;
         /* eg012.201: Enhanced the piggy backing feature by adding the Timer *
          * to wait for the piggybacked SigReq Message */
         /* The timer will be disabled in case the value is zero */
#ifdef LEGV5
	 (*teCCb)->timeToWaitPbTmr = egCb.genCfg.timeToWaitPbTmr.val;
         if((*teCCb)->timeToWaitPbTmr != 0)
         {
            egSchedTmr(*teCCb,EG_TMR_PIGGY_RSP,TMR_START,
			           (*teCCb)->timeToWaitPbTmr); 
         }
#else
         (*teCCb)->timeToWaitPbTmr = 0;
#endif
         retVal = ROK;
      }
   }
   else 
   {
       if (((msgType != EGT_GTPC_MSG_CB_REQ) && ((*teCCb)->tempPiggyEvnt->msgType == EGT_GTPC_MSG_CS_RSP)) || 
           ((msgType != EGT_GTPC_MSG_MB_REQ) && ((*teCCb)->tempPiggyEvnt->msgType == EGT_GTPC_MSG_CB_RSP)))
       {
          EG_RETVALUE(retVal);
       }
       else
       {    
          /*eg012.201: transaction id check removed*/
          /* eg012.201: Stopping the timer for piggybacked SigReq */
          if((*teCCb)->timeToWaitPbTmr != 0)
          {
             egSchedTmr(*teCCb,EG_TMR_PIGGY_RSP,TMR_STOP,NOTUSED);
          }
          (*teCCb)->timeToWaitPbTmr = 0; 
          (*teCCb)->piggyBackStatus = EG_PIGGYBACK_SEND;
          retVal = ROK;
       }
    }
   EG_RETVALUE(retVal);
}/*end of egTptHdlOutPBMsg()*/

#endif /* EG_PIGGYBACK_SUPP */
#endif

/*************************************************************************
 *      Fun:   egUpdateTxStatistics
 *
 *      Desc:  The function egUpdateTxStatistics is an utility to update the
 *             SAP statistics for transmitted message.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egUpdateTxStatistics
(
EgtMsgType msgType,             /* type of the sent messasge. */
EgSapSts *sts           /* Sap statistics on which the message was sent */
)
#else
PUBLIC Void egUpdateTxStatistics (msgType, sts)
EgtMsgType msgType;             /* type of the sent messasge. */
EgSapSts *sts;          /* Sap statistics on which the message was sent */
#endif
{
   EG_TRC2(egUpdateTxStatistics);

   if((msgType > EGT_GTP_MSG_INVALID) && (msgType < EGT_GTP_MSG_MAX))
   {
      sts->msgType[msgType].Tx++;
      sts->totalMsgsTx++;
   }

   EG_RETVOID;
} /* End of egUpdateTxStatistics () */

/*************************************************************************
 *      Fun:   egUpdateRxStatistics
 *
 *      Desc:  The function egUpdateRxStatistics is an utility to update the
 *             SAP statistics for received message.
                 *
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC Void egUpdateRxStatistics
(
EgtMsgType msgType,             /* type of the received messasge. */
EgSapSts *sts            /* pointer to the Sap statistics on which the message was received. */
)
#else
PUBLIC Void egUpdateRxStatistics (msgType, sts)
EgtMsgType msgType;             /* type of the received messasge. */
EgSapSts *sts;             /* pointer to the Sap statistics on which the message was received.  */
#endif
{
   EG_TRC2(egUpdateRxStatistics);

   if((msgType > EGT_GTP_MSG_INVALID) && (msgType < EGT_GTP_MSG_MAX))
   {
      sts->msgType[msgType].Rx++;
      sts->totalMsgsRx++;
   }

   EG_RETVOID;
} /* End of egUpdateRxStatistics () */

/******************************************************************************
 *                       Private function definitions                         *
 ******************************************************************************/
#ifdef EGTP_C
/*******************************ECHO HANDLERS*******************************/

/*************************************************************************
 *      Fun:   egHandleIncEchoRsp
 *
 *      Desc:  The function egHandleIncEchoRsp handles the incoming Echo Response.
 *
 *      Ret: -
        - ROK     - If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egHandleIncEchoRsp
(
EgUSapCb *usap,         /* usap cb which the message belongs to. */
EgEvnt *event           /* incoming Echo response */
)
#else
PRIVATE S16 egHandleIncEchoRsp (usap, event)
EgUSapCb *usap;         /* which the message belongs to. */
EgEvnt *event;          /* incoming Echo response */
#endif
{
   TknU32 teid;   
   TknU32 transId;   

   EG_TRC2(egHandleIncEchoRsp);

   cmMemset((U8 *)&teid, 0 , sizeof(TknU32));
   cmMemset((U8 *)&transId, 0 , sizeof(TknU32));

   /* get usap for echo
      from SrvCb get Usap details */
   /* indicate to the user */

   teid.pres = NOTPRSNT;
   transId.pres = NOTPRSNT;

   /*eg007.201: update statistics, prior to sending Sig Confirm */
   /* eg012.201 : Removed updating the statistics twice,fix for ccpu00124118 */

   EgUiEgtSigCfm(&(usap->pst), usap->suId,
         &(event->localAddr), &(event->remAddr), transId, teid,event->u.egMsg);

   egCmFreeEvent(event);

   EG_RETVALUE( ROK);
} /* End of egHandleIncEchoRsp() */

/*************************************************************************
 *      Fun:   egHandleIncEchoReq
 *
 *      Desc:  The function egHandleIncEchoReq handles the incoming Echo Request.
 *
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egHandleIncEchoReq
(
EgUSapCb *usap,         /* usap cb which the message belongs to. */
EgEvnt *event           /* incoming Echo request. */
)
#else
PRIVATE S16 egHandleIncEchoReq (usap, event)
EgUSapCb *usap;         /* usap cb which the message belongs to. */
EgEvnt *event;          /* incoming Echo request. */
#endif
{
   EgIncReq *incReqNode;
   S16 ret;
   TknU32 teid;

   EG_TRC2(egHandleIncEchoReq);

   cmMemset((U8 *)&teid, 0 , sizeof(TknU32));
   ret = ROK;
   incReqNode = NULLP;

   /* This has to belong to the generic usap for echo
      since the incoming request comes on 2123 without any
      information about the tunnel or interface. This should be
      found in the serverCb.
      save req source details in a usap */
   if(event->srvCb->cfg.isDfltSrv == FALSE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Echo Request received on non - default\
               port. "));
      event->resultType = EGT_ERR_INV_TUNN_NET_EVNT;
      egSendErrInd(event, usap);
      EG_RETVALUE(RFAILED);
   }
/*eg004.201 Added for eGTP-C PSF Upgrade*/
#ifdef HW
	event->lclTeid.pres = 0;
	event->lclTeid.val = 0xFFFFFFFF;
#endif

   /* save remote details of incomin message */
   ret =  egSaveMsgRemDetails(usap, &(usap->incReqCp), incReqNode, event);

   /* Error logs and indications handled inside the function */
   if(ret != ROK)
        return ret;

   teid.pres = NOTPRSNT;

   /*eg007.201: update statistics before seding Sig Indication*/
   /* eg012.201 :Removed updating the statistics twice,fix for ccpu00124118 */

   /* indicate to the user */
   EgUiEgtSigInd(&(usap->pst), usap->suId,
         &(event->localAddr), &(event->remAddr), teid, event->u.egMsg);

   egCmFreeEvent(event);

   EG_RETVALUE( ROK);
} /* End of egHandleIncEchoReq () */

/*************************************************************************
 *      Fun:   egHandleOutEchoReq
 *
 *      Desc:  The function egHandleOutEchoReq handles the outgoing Echo Request.
 *
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egHandleOutEchoReq 
(
EgUSapCb *usap,         /* usap cb which the message belongs to. */
EgEvnt *event          /* outgoing Echo request. */
)
#else
PRIVATE S16 egHandleOutEchoReq ( usap, event)
EgUSapCb *usap;         /* usap cb which the message belongs to. */
EgEvnt *event;          /* outgoing Echo request. */
#endif
{
   S16 ret;

   EG_TRC2(egHandleOutEchoReq);

   ret = ROK;

   /* send to edm */
   event->srvCb = egCb.egtSapLst[usap->cfg.sapId]->tptSrvCb;
   /* override the remote port to 2123 */
   egSetDefPortAddr(event);

   ret = egEncPthReq(event);
   if(ret != ROK)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
      /* result type filled by EDM */
      egSendErrInd(event, usap);
      /***********************************************
      *  Sending the error indication is successful, *
      *  So Return from here with ROK                *
      ***********************************************/
      ret = ROK;
   }

   EG_RETVALUE( ret );
} /* End of egHandleOutEchoReq () */

/*************************************************************************
 *      Fun:   egHandleOutEchoRsp
 *
 *      Desc:  The function egHandleOutEchoRsp handles the outgoing Echo Response.
 *
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PRIVATE S16 egHandleOutEchoRsp 
(
EgUSapCb *usap,         /* usap Cb which the message belongs to. */
EgEvnt *event           /* outgoing Echo response. */
)
#else
PRIVATE S16 egHandleOutEchoRsp (usap, event)
EgUSapCb *usap;         /* usap Cb which the message belongs to. */
EgEvnt *event;          /* outgoing Echo response. */
#endif
{
   S16 ret;
   EgIncReq * incReqNode;

   EG_TRC2(egHandleOutEchoRsp);

   ret = ROK;
   incReqNode = NULLP;

   /* This has to belong to the generic usap for echo
      since the incoming request comes on 2123 without any
      information about the tunnel or interface.
      get the remote details from the inc req list in usap */

   /* ccpu00134942 : echo messages wil not be verified at stack which is sent from application */
   /* Remote details are filled from the application */

   /* Error logs and indications handled inside the function */

  /* ccpu00134942 : fix has to be added for MULTI_IP */
#ifdef EG_MULTI_IP
   event->srvCb = egCb.egtSapLst[usap->cfg.sapId]->tptSrvCb;
#else
   event->srvCb = egCb.tSapCb->dftlTptSrv;
   EG_SET_PORT(event->remAddr, LEG_DEF_PORT_MME);
#endif

   /* send this to edm */
   ret = egEncPthReq(event);
   if(ret != ROK)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
      /* result type filled by EDM */
      egSendErrInd(event, usap);
      /***********************************************
      *  Sending the error indication is successful, *
      *  So Return from here with ROK                *
      ***********************************************/
      ret = ROK;
   }

   EG_RETVALUE( ret );
} /* End of egHandleOutEchoRsp () */ 

/****************** FSM handlers *********************************************/

/*************************************************************************
 *      Fun:   egTunnFsmIdlISesReq
 *
 *      Desc:  The function is the FSM handler for event - receive session request
 *             in the state - IDLE(new tunnel).
 *
 *      Ret: 
                - ROK      - If the procedure was successful.
                - ROUTRES  - If the procedure failed due to resource failure.
                - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmIdlISesReq 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* incoming event */
)
#else
PRIVATE S16 egTunnFsmIdlISesReq (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;          /* incoming event */
#endif
{
   S16 ret;
   Bool validMsg;
   TknU32 teid;
   EgIncReq *incReqNode;

   EG_TRC2(egTunnFsmIdlISesReq);

   ret = ROK;
   validMsg = TRUE;
   incReqNode = NULLP;
   cmMemset((U8 *)&teid, 0 , sizeof(TknU32));

   /* Validate */
   validMsg = egValidateMsgInt((EgtMsgType)event->u.egMsg->msgHdr.msgType,
                               (LegEntity) LEG_GTP_ENT_UNKNWN,
                               (LegEntity) egCb.genCfg.nodeType,
                               (EgtIntf) event->intfType);
   if( validMsg != TRUE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "The Message %d is invalid on the interface %d.",
         event->u.egMsg->msgHdr.msgType, teCCb->intfType));
      event->resultType = EGT_ERR_INV_TUNN_NET_EVNT;
      egSendErrInd(event, teCCb->usap);
      egTunnDelTECCb(teCCb);
      EG_RETVALUE(RFAILED);
   }/* end of if */

   event->lclTeid.pres = PRSNT_NODEF;
   event->lclTeid.val = teCCb->locTeid; 
   teCCb->intfType = (EgtIntf)event->intfType;

/*eg004.201 Added for eGTP-C PSF Upgrade:Obtain the local teid for an incoming message*/
#ifdef HW
	event->lclTeid.pres = 1;
	event->lclTeid.val = teCCb->locTeid;
#endif

   /* save request in req list */
   ret =  egSaveMsgRemDetails(teCCb->usap, &(teCCb->incReqCp), incReqNode, event);

   /* Error logs and indications handled inside the function */
   if(ret != ROK)
        return ret;

   teCCb->state = EGT_TUN_STATE_INCSESSREQ;

   /* Indication to the user */
   teid.pres = PRSNT_NODEF;
   teid.val = teCCb->locTeid;

   /*eg007.201: update statistics before seding Sig Indication*/
   /* eg012.201 :Removed updating the statistics twice,fix for ccpu00124118 */

   EgUiEgtSigInd(&(teCCb->usap->pst), teCCb->usap->suId, &(event->localAddr),
         &(event->remAddr), teid,event->u.egMsg);

   egCmFreeEvent(event);

   EG_RETVALUE(ret);
} /* End of egTunnFsmIdlISesReq () */

/*************************************************************************
 *      Fun:   egTunnFsmIdlOSesReq
 *
 *      Desc:  The function is the FSM handler for event - send Session Request,
               in the state - IDLE(new tunnel).
 *
 *      Ret: 
                - ROK      - If the procedure was successful.
                - ROUTRES  - If the procedure failed due to resource failure.
                - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmIdlOSesReq 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmIdlOSesReq (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;          /* outgoing event */
#endif
{
   S16 ret;
   Bool validMsg;
   Bool isMsbSet;
   EgIncReq *incReqNode;

   EG_TRC2(egTunnFsmIdlOSesReq);

   ret = ROK;
   validMsg = TRUE;
   incReqNode = NULLP;

   /* Validate */
   validMsg = egValidateMsgInt((EgtMsgType)event->u.egMsg->msgHdr.msgType,
                               (LegEntity)egCb.genCfg.nodeType,
                               (LegEntity)LEG_GTP_ENT_UNKNWN,
                               (EgtIntf) event->intfType);
   if( validMsg != TRUE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "The Message %d is invalid on the interface %d.",
         event->u.egMsg->msgHdr.msgType, teCCb->intfType));
      event->resultType = EGT_ERR_MSG_INV_INT;
      egSendErrInd(event, teCCb->usap);
      egTunnDelTECCb(teCCb);

      EG_RETVALUE(RFAILED);
   }/* end of if */

   /* Save transaction ID */
   teCCb->transId = event->transId.val;
   event->lclTeid.pres = PRSNT_NODEF;
   event->lclTeid.val = teCCb->locTeid; 

   /* Save timer details */
   teCCb->intfType = (EgtIntf)event->intfType;

   /* Get SrvCb from USAP */
   event->srvCb = egCb.egtSapLst[teCCb->usap->cfg.sapId]->tptSrvCb;

   /* If this is a command triggered request delete the command node */

   isMsbSet = FALSE;

   /* If the event is a request triggered by a command
      remove the corresponding incoming node. */
   EG_IS_MSB_SET_IN_SEQ_NO(event->seqNumber, isMsbSet);

   if(isMsbSet == TRUE)
   {
      ret = egGetMsgRemDetails(teCCb->usap, &(teCCb->incReqCp), 
            incReqNode, event, EG_GTPC_MSGCAT_REQUEST);

      /* Error logs and indications handled inside the function */
      if(ret != ROK)
         return ret;
   }


   /* Override the remote port to 2123 */
   egSetDefPortAddr(event);

   ret = egEncMsgReq(event);
   /* Set State to TUN_STATE_OUTSESSREQ */
   if(ret == ROK)
   {
      teCCb->state = EGT_TUN_STATE_OUTSESSREQ;
   }
   else
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
      /* result type filled by EDM */
      egSendErrInd(event, teCCb->usap);
      egTunnDelTECCb(teCCb);
      ret = ROK;
   }
   EG_RETVALUE(ret);
} /* End of egTunnFsmIdlOSesReq () */

/*************************************************************************
 *      Fun:   egTunnFsmIncOutRsp
 *
 *      Desc:  The function is the FSM handler for event - send Session Response,
               in the state - INCSESSREQ.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmIncOutRsp 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmIncOutRsp (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;
   EgIncReq *incReqNode;

   EG_TRC2(egTunnFsmIncOutRsp);

   ret = ROK;
   incReqNode = NULLP;

   /* NOTE: Here the FTEID is not updated with the local teid.
      The user knows about the teid after SigInd for incoming CSR.
      Hence the user can update the FTEID himself. */

   /* Get SrvCb from USAP */
   event->srvCb = egCb.egtSapLst[teCCb->usap->cfg.sapId]->tptSrvCb;

   ret = egGetMsgRemDetails(teCCb->usap, &(teCCb->incReqCp), incReqNode,
                         event, EG_GTPC_MSGCAT_RESPONSE);

   /* Error logs and indications handled inside the function */
   if(ret != ROK)
        EG_RETVALUE(ret);
   
   /* Call Transport Send with msg, SrvCb, retransmission timer, RemAddr */
   ret = egEncMsgReq(event);
   if(ret == ROK)
   {
      teCCb->state = EGT_TUN_STATE_CONNECTED;
      egCb.genSts.numActvTnls++;
#ifndef ALIGN_64BIT
      EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,\
                "Teid %ld now active. Total act tunnels %ld.",
                 teCCb->locTeid, egCb.genSts.numActvTnls));
#else
      EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,\
                "Teid %d now active. Total act tunnels %d.",
                 teCCb->locTeid, egCb.genSts.numActvTnls));
#endif
/*eg004.201  Send RT Update when tunnel state changes to connected */
      /* If PSF functionality is enabled, Generate a RUN TIME update
      and update peer as tunnel state has been changed to CONNECTED.
      AddMapping for this control block was done when tunnels was 
      allocated i.e. in either of these functions. 
      egTunCreateSend
      egTunCreateReceive
      */
#ifdef HW
      HW_RTUPD_ADD(HW_EG_TEIDCB,(Void *)teCCb);
      HW_DBG_INFO(0,0, (hwp, "\n PSF RunTimeUpdate & UpdatePeer called for CbType=[%d],"
              " TeidValue=[%ld]",HW_EG_TEIDCB,teCCb->locTeid));
#endif
   }/* end of if */
   else
   {
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
         /* result type filled by EDM */
         egSendErrInd(event, teCCb->usap);
         ret = ROK;
   }  
   EG_RETVALUE(ret);
} /* End of egTunnFsmIncOutRsp () */

#ifdef PHASE2
/*************************************************************************
 *      Fun:   egTunnFsmIncErr
 *
 *      Desc:  The function is the FSM handler for event - error, in the state 
              - INCSESSREQ.
 *
 *      Ret: 
                - ROK      - If the procedure was successful.
                - ROUTRES  - If the procedure failed due to resource failure.
                - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmIncErr 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmIncErr (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;

   EG_TRC2(egTunnFsmIncErr);

   ret = RFAILED;

#ifndef ALIGN_64BIT
   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "Error %d occured on Teid %ld, in state %d.",
         event->resultType, teCCb->locTeid, teCCb->state));
#else
   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "Error %d occured on Teid %d, in state %d.",
         event->resultType, teCCb->locTeid, teCCb->state));
#endif

   egSendErrInd(event, teCCb->usap);
   egTunnDelTECCb(teCCb);

   EG_RETVALUE(ret);
} /* End of egTunnFsmIncErr () */
#endif /* PHASE2 */

/*************************************************************************
 *      Fun:   egTunnFsmOutIncRsp
 *
 *      Desc:  The function is the FSM handler for event - receive Session Response
              in the state - OUTSESSREQ.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmOutIncRsp 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmOutIncRsp (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;
   TknU32 teid;
   TknU32 transId;
#ifdef EGTP_C_PTH_MGT 
   CmTptAddr  destIpAddr;
   EgDestCb   *destCb = NULLP;
#ifdef EG_MULTI_IP
   /*-- eg007.201: Obtain tSapCb associated from SrvCb in USap --*/
   EgTSapCb     *tSapCb = teCCb->usap->tSapCb;
#endif
#endif

   EG_TRC2(egTunnFsmOutIncRsp);

   ret = ROK;
   cmMemset((U8 *)&teid, 0 , sizeof(TknU32));
   cmMemset((U8 *)&transId, 0 , sizeof(TknU32));

   /* Validate: transport layer would have already done this */


   /* validation, if response type is matching will be done in Transport
    * also the validate for message on the interface has already been done for
    * the request */

   teCCb->state = EGT_TUN_STATE_CONNECTED;

   egCb.genSts.numActvTnls++;

   teid.pres = PRSNT_NODEF;
   teid.val = teCCb->locTeid;

   transId.pres = PRSNT_NODEF;
   transId.val = teCCb->transId;

   event->transId.pres = PRSNT_NODEF;
   event->transId.val = teCCb->transId;

#ifndef ALIGN_64BIT
   EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,\
        "Teid %ld now active. Total act tunnels %ld.",
         teCCb->locTeid, egCb.genSts.numActvTnls));
#else
   EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,\
        "Teid %d now active. Total act tunnels %d.",
         teCCb->locTeid, egCb.genSts.numActvTnls));
#endif

   /* eg007.201:update statistics, before sending Sig confirm */
   /* eg012.201 :Removed updating the statistics twice,fix for ccpu00124118 */

   /* Indication to the user */
   EgUiEgtSigCfm(&(teCCb->usap->pst), teCCb->usap->suId, &(event->localAddr),
                 &(event->remAddr), transId, teid, event->u.egMsg);

#ifdef EGTP_C_PTH_MGT
   /************************************************************************* 
    * eg006.201:Tunnel is in connected state. Start Echo timer for the Destination
    *  if its new, else increase only connected tunnel count 
    **************************************************************************/
   cmMemset((U8 *)&(destIpAddr),EG_ZERO,sizeof(CmTptAddr)); 
   egCpyCmTptAddr(&destIpAddr, &event->remAddr);
   EG_SET_PORT(destIpAddr, EG_ZERO);
#ifdef EG_MULTI_IP
   /* eg007.201: Search dstCblist in TSAP Cb for destination IP */
   ret = cmHashListFind(&(tSapCb->dstCbList), (U8 *)&(destIpAddr),\
         sizeof(CmTptAddr),0,(PTR *)&destCb);
#else
   ret = cmHashListFind(&egCb.dstCbList, (U8 *)&(destIpAddr),\
         sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif/*EG_MULTI_IP*/

   if(destCb != NULLP)
   { 
      /*-- connTunnCnt will be Zero for first tunnel addition --*/
      if(EG_ZERO == destCb->connTunnCnt)
      {
         /*Increment the connected tunnel count*/
         destCb->connTunnCnt++;
         destCb->destReachable = TRUE;
         if(!egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb) 
         {
            ret = egSchedTmr(NULLP, EG_TMR_PATH_GLOBAL, TMR_START, \
                  egCb.genCfg.pathTmr.val);
            if (ret != ROK)
            {
               egCb.echoTmrStarted = FALSE;
               EG_DBG_ERR(0, 0, (egp,"\n  Starting of Echo Timer Failed"));
            }
            else
            {
               egCb.echoTmrStarted = TRUE;
            }
         }
      }
      else
      {
         /*Timers already started just Increment connected tunnel count*/
         destCb->connTunnCnt++;
         destCb->destReachable = TRUE;
      }
   }/*end of (destCb != NULLP)*/ 

#endif

   egCmFreeEvent(event);

/*eg004.201  Send RT Update STANDBY for tunnel Cb when tunnel state changes to conncetd on receiving
 *a CS Response from the peer eGTP-C node
 */
#ifdef HW
   /* 
    *If PSF functionality is enabled, Generate a RUN TIME update
    *and update peer as tunnel state has been changed to CONNECTED.
    *AddMapping for this control block was done when tunnels was 
    *allocated i.e. in either of these functions. 
    *egTunCreateSend
    *egTunCreateReceive
    */
   HW_RTUPD_ADD(HW_EG_TEIDCB,(Void *)teCCb);
   HW_DBG_INFO(0,0, (hwp, "\n PSF RunTimeUpdate & UpdatePeer called for CbType=[%d],"
            " TeidValue=[%ld]",HW_EG_TEIDCB,teCCb->locTeid));
#endif

   EG_RETVALUE(ret);
} /* End of egTunnFsmOutIncRsp () */

/*************************************************************************
 *      Fun:   egTunnFsmOutErr
 *
 *      Desc:  The function is the FSM handler for event - error in the state 
              - OUTSESSREQ.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmOutErr 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmOutErr (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;

   EG_TRC2(egTunnFsmOutErr);

   ret = RFAILED;

   /* Indication to the user */
#ifndef ALIGN_64BIT
   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "Error %d occured on Teid %ld, in state %d.",
         event->resultType, teCCb->locTeid, teCCb->state));
#else
   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "Error %d occured on Teid %d, in state %d.",
         event->resultType, teCCb->locTeid, teCCb->state));
#endif

   egSendErrInd(event, teCCb->usap);
   egTunnDelTECCb(teCCb);

   EG_RETVALUE(ret);
} /* End of egTunnFsmOutErr () */

/*************************************************************************
 *      Fun:   egTunnFsmConIncMsg
 *
 *      Desc:  The function is the FSM handler for event - receive any GTP message
               in the state - CONNECTED.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmConIncMsg 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmConIncMsg (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;
   Bool validMsg;
   TknU32 teid;
   EgtMsgCat msgCat;
   Bool isMsbSet =  FALSE;

   EG_TRC2(egTunnFsmConIncMsg);

   ret = ROK;
   validMsg = TRUE;
   cmMemset((U8 *)&teid, 0 , sizeof(TknU32));
   msgCat = EG_GTPC_MSGCAT_OTHER;

   /* validate */
   validMsg = egValidateMsgInt((EgtMsgType)event->u.egMsg->msgHdr.msgType,
                               (LegEntity)LEG_GTP_ENT_UNKNWN,
                                (LegEntity)egCb.genCfg.nodeType,
                                (EgtIntf)event->intfType);
   if( validMsg != TRUE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp, \
        "The Message %d is invalid on the interface %d.",
         event->u.egMsg->msgHdr.msgType, teCCb->intfType));
      event->resultType = EGT_ERR_INV_TUNN_NET_EVNT;
      egSendErrInd(event, teCCb->usap);
      EG_RETVALUE(RFAILED);
   }/* end of if */

   msgCat = egGetMsgCategory ((EgtMsgType)event->u.egMsg->msgHdr.msgType);

   /* If the message is request or notification*/
   /**********************************************************
    * For Context Rsp, Context Ack is expected as response.  *
    * So, this message needs to be saved                     *
    **********************************************************/

   if((msgCat == EG_GTPC_MSGCAT_REQUEST) || (msgCat == EG_GTPC_MSGCAT_NOTFN)
       || (msgCat == EG_GTPC_MSGCAT_COMMAND) || 
       (event->u.egMsg->msgHdr.msgType == EGT_GTPC_MSG_CNTXT_RSP))
   {
      EgIncReq *incReqNode = NULLP;
      EG_IS_MSB_SET_IN_SEQ_NO(event->u.egMsg->msgHdr.seqNumber, isMsbSet);

/*eg004.201 Added for eGTP-C PSF Upgrade:Obtain the local teid for an incoming message*/
#ifdef HW
	event->lclTeid.pres = 1;
	event->lclTeid.val = teCCb->locTeid;
#endif
      /* eg012.201:not required to save remote details for messages 64 to 70 ,fix for ccpu00125389 */
      switch(event->u.egMsg->msgHdr.msgType)
      {
         /* ccpu00135855 : This needs to be handled better in GPR */
         case EGT_GTPC_MSG_MBFAIL_IND:
         case EGT_GTPC_MSG_DB_FAIL_IN:
         case EGT_GTPC_MSG_BR_FAIL_IND:
         case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
            /* Not required to save remote details */
            break;
         default:
            /* eg010:201 : PbMsg variable name changed */
            if(isMsbSet != TRUE && event->srvCb->cfg.isDfltSrv == FALSE 
               && event->u.egMsg->msgHdr.msgType != EGT_GTPC_MSG_CNTXT_RSP 
               && !event->isitPbMsg)
            {
               EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Request received on non - default port"));
               event->resultType = EGT_ERR_INV_TUNN_NET_EVNT;
               egSendErrInd(event, teCCb->usap);
               EG_RETVALUE(RFAILED);
            }
            /* add into list */
            ret =  egSaveMsgRemDetails(teCCb->usap, &(teCCb->incReqCp), incReqNode, event);
            /* Error logs and indications handled inside the function */
            if(ret != ROK)
               EG_RETVALUE(ret);
      }
   }/* end of if msgCat is Request or Notification*/

   teid.pres = PRSNT_NODEF;
   teid.val = teCCb->locTeid;

   /* ccpu00137378 : FAIL Ind sent as sig indication messages */
   if((msgCat == EG_GTPC_MSGCAT_RESPONSE) || (msgCat == EG_GTPC_MSGCAT_ACKN)
       || (msgCat == EG_GTPC_MSGCAT_FAILIND) || (msgCat == EG_GTPC_MSGCAT_RSP_ACK))
   {
      TknU32 transId;

      cmMemset((U8 *)&transId, 0 , sizeof(TknU32));
      transId.pres = PRSNT_NODEF;
      transId.val = teCCb->transId;

      /* eg007.201:- update statistics, before sending Sig Confirm */
      /* eg012.201 : Removed updating the statistics twice,fix for ccpu00124118 */

      /* Indication to the user */
      EgUiEgtSigCfm(&(teCCb->usap->pst), teCCb->usap->suId, &(event->localAddr),
            &(event->remAddr), transId, teid, event->u.egMsg);

      egCmFreeEvent(event);
   }
   else
   {
      /* eg007.201:- update statistics before sending Sig Indication */
      /* eg012.201 : Removed updating the statistics twice,fix for ccpu00124118 */

      /* Indication to the user */
      EgUiEgtSigInd(&(teCCb->usap->pst), teCCb->usap->suId, &(event->localAddr),
            &(event->remAddr), teid,event->u.egMsg);

      egCmFreeEvent(event);
   }

   EG_RETVALUE( ret);
} /* End of egTunnFsmConIncMsg () */

/*************************************************************************
 *      Fun:   egTunnFsmConOutMsg
 *
 *      Desc:  The function is the FSM handler for event - send a GTP message 
              in the state - CONNECTED.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmConOutMsg 
(
EgTECCb *teCCb,       /* tunnel endpoint on which an event has occured */
EgEvnt *event         /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmConOutMsg (teCCb, event)
EgTECCb *teCCb;       /* tunnel endpoint on which an event has occured */
EgEvnt *event;        /* outgoing event */
#endif
{
   S16 ret;
   Bool validMsg;
   EgIncReq *incReqNode;
   EgtMsgCat msgCat;

   EG_TRC2(egTunnFsmConOutMsg);

   ret = ROK;
   validMsg = TRUE;
   incReqNode = NULLP;
   msgCat = EG_GTPC_MSGCAT_OTHER;

   /* Validate */
   validMsg = egValidateMsgInt((EgtMsgType)event->u.egMsg->msgHdr.msgType, 
                               (LegEntity)egCb.genCfg.nodeType,
                                 LEG_GTP_ENT_UNKNWN, (EgtIntf)event->intfType);
   if( validMsg != TRUE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,
        "The Message %d is invalid on the interface %d.",
         event->u.egMsg->msgHdr.msgType, teCCb->intfType));
      event->resultType = EGT_ERR_MSG_INV_INT;
      egSendErrInd(event, teCCb->usap);
      EG_RETVALUE( RFAILED);
   }/* end of if */

   event->srvCb = egCb.egtSapLst[teCCb->usap->cfg.sapId]->tptSrvCb;
   /*eg012.201:moving transaction id updation from here*/

   /* Check if the message type is a request */
   msgCat = egGetMsgCategory ((EgtMsgType)event->u.egMsg->msgHdr.msgType);

   /* If the message is response or aknowledgement */
   /* ccpu00138526, ccpu00137585 : Notn & FailInd Msgs will not be queued */
   if((msgCat == EG_GTPC_MSGCAT_RESPONSE) || (msgCat == EG_GTPC_MSGCAT_RSP_ACK)
      || (msgCat == EG_GTPC_MSGCAT_ACKN) || (msgCat == EG_GTPC_MSGCAT_FAILIND))
   {
      ret = egGetMsgRemDetails(teCCb->usap, &(teCCb->incReqCp), incReqNode,
                         event, msgCat);

      /* Error logs and indications handled inside the function */
      if(ret != ROK)
         return ret;
   }
   /*eg012.201:transaction id updation fix for ccpu00129025*/
   else if((msgCat == EG_GTPC_MSGCAT_REQUEST)||(msgCat == EG_GTPC_MSGCAT_NOTFN))
   {
      Bool isMsbSet;
      teCCb->transId = event->transId.val; 
      isMsbSet = FALSE;

      /* If the event is a request triggered by a command
         remove the corresponding incoming node. */
      EG_IS_MSB_SET_IN_SEQ_NO(event->seqNumber, isMsbSet);

      if(isMsbSet == TRUE)
      {
         /* eg012.201:for message 64 to 70 need not to get remote details,fix for ccpu00125389 */
         switch (event->u.egMsg->msgHdr.msgType)
         {
            /* ccpu00135855 : This needs to be handled better in GPR */
            case EGT_GTPC_MSG_DD_NOTFN_FAIL_IND:
               /* message 64 to 70 need not get remote details */
               break;
            default:
               ret = egGetMsgRemDetails(teCCb->usap, &(teCCb->incReqCp), 
                                        incReqNode, event, EG_GTPC_MSGCAT_REQUEST);
         }
         /* Error logs and indications handled inside the function */
         if(ret != ROK)
            EG_RETVALUE( ret);
      }

      /* Override the default port */
      egSetDefPortAddr(event);
   }
   /* eg012.201:store the transaction id for command request message,fix for ccpu00130148 */
   else if ( ( msgCat == EG_GTPC_MSGCAT_COMMAND ) ) 
   { 
      teCCb->transId = event->transId.val; 
   }
 
   ret = egEncMsgReq(event);
   if(ret != ROK)
   {
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
         /* result type filled by EDM */
         egSendErrInd(event, teCCb->usap);
         ret = ROK;
   } 
   EG_RETVALUE( ret);
} /* End of egTunnFsmConOutMsg () */

/*************************************************************************
 *      Fun:   egTunnFsmConErr
 *
 *      Desc:  The function is the FSM handler for event - error, in the state 
               - CONNECTED.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmConErr 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmConErr (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;

   EG_TRC2(egTunnFsmConErr);

   ret = RFAILED;

#ifndef ALIGN_64BIT
   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,
        "Error %d occured on Teid %ld, in state %d.",
         event->resultType, teCCb->locTeid, teCCb->state));
#else
   EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,
        "Error %d occured on Teid %d, in state %d.",
         event->resultType, teCCb->locTeid, teCCb->state));
#endif

   egSendErrInd(event, teCCb->usap);
   EG_RETVALUE( ret);
} /* End of egTunnFsmConErr () */

/*************************************************************************
 *      Fun:   egTunnFsmInvNetEvnt
 *
 *            Desc:  The function is the FSM handler for event - in the state -.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmInvNetEvnt 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmInvNetEvnt (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   /* drop the message */
   /* indicate to the user */

   S16 ret;

   EG_TRC2(egTunnFsmInvNetEvnt);

   ret = RFAILED;

#ifndef ALIGN_64BIT
   EG_DBG_ERR(EG_DBG_TUNN, teCCb, (egp,\
        "Invalid Message %d received on teid %ld in state %d.",
        event->u.egMsg->msgHdr.msgType, teCCb->locTeid, teCCb->state));
#else
   EG_DBG_ERR(EG_DBG_TUNN, teCCb, (egp,\
        "Invalid Message %d received on teid %d in state %d.",
        event->u.egMsg->msgHdr.msgType, teCCb->locTeid, teCCb->state));
#endif
   ret = RFAILED;
   event->resultType = EGT_ERR_INV_TUNN_NET_EVNT;

   egSendErrInd(event, teCCb->usap);

   EG_RETVALUE(ret);
} /* End of egTunnFsmInvNetEvnt () */

/*************************************************************************
 *      Fun:   egTunnFsmInvUsrEvnt
 *
 *      Desc:  The function is the FSM handler for event - in the state -.
 *
 *      Ret:
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunnFsmInvUsrEvnt 
(
EgTECCb *teCCb,         /* tunnel endpoint on which an event has occured */
EgEvnt *event           /* outgoing event */
)
#else
PRIVATE S16 egTunnFsmInvUsrEvnt (teCCb, event)
EgTECCb *teCCb;         /* tunnel endpoint on which an event has occured */
EgEvnt *event;           /* outgoing event */
#endif
{
   S16 ret;

   EG_TRC2(egTunnFsmInvUsrEvnt);

   ret = RFAILED;

#ifndef ALIGN_64BIT
   EG_DBG_ERR(EG_DBG_TUNN, teCCb, (egp,\
        "Invalid Message %d attempted to send on teid %ld in state %d.",
        event->u.egMsg->msgHdr.msgType, teCCb->locTeid, teCCb->state));
#else
   EG_DBG_ERR(EG_DBG_TUNN, teCCb, (egp,\
        "Invalid Message %d attempted to send on teid %d in state %d.",
        event->u.egMsg->msgHdr.msgType, teCCb->locTeid, teCCb->state));
#endif
   event->resultType = EGT_ERR_INV_TUNN_USR_EVNT;

   egSendErrInd(event, teCCb->usap);

   EG_RETVALUE(ret);
} /* End of egTunnFsmInvUsrEvnt () */

/**************************** TEID generators *********************************/

/*************************************************************************
 *      Fun:   egInitFreeTeidList
 *
 *      Desc:  The function egInitFreeTeidList initializes an array of teids. The array
 *             of free teids is initialized.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
/*eg004.201 Modified for eGTP-C PSF Upgrade*/
#ifdef ANSI
PUBLIC S16 egInitFreeTeidList 
(
U32      tunnIdx                /* The tunnel Cb whose teid array has to be initialize */
)
#else
PUBLIC S16 egInitFreeTeidList (tunnIdx)
U32      tunnIdx;               /* The tunnel Cb whose teid array has to be initialize */
#endif
{
   U32 idx;
   CmLList *node; 
   U32 teid;

   EG_TRC2(egInitFreeTeidList);

   teid = 0;
   node = NULLP;

   /* initialize array of tunnCb to NULL and free TEIDs */
   for (idx = 0; idx < egCb.genCfg.maxTunlArrSz; idx++)
   {
      teid = 0;
      node = NULLP;

      if ((idx == 0) && (tunnIdx == 0))
         continue;

      teid = (U32)(tunnIdx * egCb.genCfg.maxTunlArrSz) + idx;
      if(egCb.genCfg.nmbMaxTnls < teid )
      {
         /* max TEIDs reached */
         break;
      }

      node =  (CmLList *)egAlloc(sizeof(CmLList));
      if(node == NULLP)
      {
         EG_RETVALUE(ROUTRES);
      } 
      node->node = teid;
      cmLListAdd2Tail(&(egCb.tnlCb.freeTeidLst), node);
   }/* end of for loop */

   EG_RETVALUE (ROK);
} /* End of egInitFreeTeidList () */

/*************************************************************************
 *      Fun:   egInitTeidArr
 *
 *      Desc:  The function egInitTeidArr intializes the global array tunnel Cbs. It
        also initializes the array of teid Cbs in the first tunnel Cb.
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PRIVATE S16 egInitTeidArr 
(
Void
)
#else
PRIVATE S16 egInitTeidArr (Void)
#endif
{
   S16 ret;

   EG_TRC2(egInitTeidArr);

   ret = ROK;

   /* get the number of arrays required */
   egCb.tnlCb.arrNum = egCb.genCfg.nmbMaxTnls / egCb.genCfg.maxTunlArrSz; 
   if( (egCb.genCfg.nmbMaxTnls % egCb.genCfg.maxTunlArrSz) > 0)
      egCb.tnlCb.arrNum = egCb.tnlCb.arrNum + 1; 

   /* allocate memory in the global for top level array */
   egCb.tnlCb.teCCbArr = (EgTECCb***) egAlloc (sizeof(PTR) * egCb.tnlCb.arrNum );

   if(egCb.tnlCb.teCCbArr == NULLP)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
                "failed to allocate memory for array of array of tunnel Cbs"));
      EG_RETVALUE(ROUTRES);
   }/* end of if teCCbArr == NULLP  */

   /* allocate for the first array of teids*/
   egCb.tnlCb.teCCbArr[0] = (EgTECCb **) egAlloc 
                             (sizeof(PTR) * egCb.genCfg.maxTunlArrSz );

   if(egCb.tnlCb.teCCbArr[0] == NULLP)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "failed to allocate memory for first tunnel Cb with a range of teids"));
      egFree((Data *)egCb.tnlCb.teCCbArr, (sizeof(PTR) * egCb.tnlCb.arrNum ));
      EG_RETVALUE(ROUTRES);
   }/* end of if tnlCbLst[0] == NULLP */

   /* initialize the free teid list */
   cmLListInit(&(egCb.tnlCb.freeTeidLst));

   /* allocate for the array of free list of teids */
   egCb.tnlCb.freeArrIdx = 1;
   /* check return value */
   ret = egInitFreeTeidList(0);

   EG_RETVALUE(ret);
} /* End of egInitTeidArr () */

/*************************************************************************
 *      Fun:   egDeInitTeidArr
 *
 *      Desc:  The function egDeInitTeidArr un-intializes the global array tunnel Cbs.
        It also un-initializes the array of teid Cbs in the array of tunnel Cb.
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PRIVATE S16 egDeInitTeidArr 
(
Void
)
#else
PRIVATE S16 egDeInitTeidArr (Void)
#endif
{
   U32 cnt, idx, teid;
   CmLList *node; 

   EG_TRC2(egDeInitTeidArr);

   node = NULLP;
   teid = 0;

   /* for every array of array of TEIDs in use */
   for(cnt=0; cnt < egCb.tnlCb.freeArrIdx; cnt++)
   {
      /* if there is an array for a range of TEIDs */
      if( egCb.tnlCb.teCCbArr[cnt] != NULLP )
      {
         for(idx=0; idx < egCb.genCfg.maxTunlArrSz; idx++)
         {
                if(egCb.tnlCb.teCCbArr[cnt][idx] != NULLP)
                {
                   (void) egTptProcessLclDel ((egCb.tnlCb.
                                               teCCbArr[cnt][idx])->usap->tptSrvCb, teid);
                   /* Free the teCCb internally */
                   egTunnDelTECCb(egCb.tnlCb.teCCbArr[cnt][idx]);
                }
                teid ++;
         }
         /* free the array */
         egFree((Data *)egCb.tnlCb.teCCbArr[cnt], (sizeof(PTR) * 
                  egCb.genCfg.maxTunlArrSz ));
      }/* end of if */
   } /* end of for loop, cnt*/

  /* Free the free TEID list*/
   (U32)CM_LLIST_FIRST_NODE(&(egCb.tnlCb.freeTeidLst), node);

   while(node != NULLP)
   {

      cmLListDelFrm(&(egCb.tnlCb.freeTeidLst), node);
      egFree((Data *)node, sizeof(CmLList));
      (U32)CM_LLIST_FIRST_NODE(&(egCb.tnlCb.freeTeidLst), node);

   }

   egFree((Data *)egCb.tnlCb.teCCbArr, (sizeof(PTR) * egCb.tnlCb.arrNum ));


   EG_RETVALUE(ROK);

} /* End of egDeInitTeidArr () */

/*************************************************************************
 *      Fun:   egObtainFreeTeid
 *
 *      Desc:  The function egObtainFreeTeid returns a free TEID.
 *
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
************************************************************************/
#ifdef ANSI
PRIVATE S16 egObtainFreeTeid 
(
U32 *teid               /* The teid holds the TEID that this function returns */
)
#else
PRIVATE S16 egObtainFreeTeid (teid)
U32 *teid;              /* The teid holds the TEID that this function returns */
#endif
{
   S16 ret;
   CmLList *node; 

   EG_TRC2(egObtainFreeTeid);

   ret = ROK;
   node = NULLP;

   /* get next free TEID from free TEID list */
   *teid = (U32)CM_LLIST_FIRST_NODE(&(egCb.tnlCb.freeTeidLst), node);
   if(node != NULLP)
   {
      *teid = (U32) node->node;
      cmLListDelFrm(&(egCb.tnlCb.freeTeidLst), node);
      egFree((Data *)node, sizeof(CmLList));
   }
   else
   {
      /* existing teids are exhausted, allocate another TEID array */
      if(egCb.tnlCb.freeArrIdx == egCb.tnlCb.arrNum)
      {
         /* all TEIDs are exhausted */
         *teid = 0;
         ret = RFAILED;
      }
      else
      {
         U32 tunnArrIdx;

         tunnArrIdx = egCb.tnlCb.freeArrIdx;

         if(egCb.tnlCb.teCCbArr[tunnArrIdx] == NULLP)
         {
            egCb.tnlCb.teCCbArr[tunnArrIdx] = (EgTECCb **) egAlloc 
               (sizeof(PTR) * egCb.genCfg.maxTunlArrSz );

            /* create free teids out of new array */
            ret = egInitFreeTeidList(tunnArrIdx);

         }
         egCb.tnlCb.freeArrIdx ++;
         /* Now get TEID out of new free TEID list */
         /*eg009.201:Klocwork fix - Do not capture return*/
         CM_LLIST_FIRST_NODE(&(egCb.tnlCb.freeTeidLst), node);
         if(node != NULLP)
         {
            *teid = (U32)node->node;
            cmLListDelFrm(&(egCb.tnlCb.freeTeidLst), node);
            egFree((Data *)node, sizeof(CmLList));
         }
         else
         {
            /* all TEIDs are exhausted */
            *teid = 0;
            ret = RFAILED;
         }
      }
   }
   if(egCb.genCfg.nmbMaxTnls < *teid )
   {
      /* all TEIDs are exhausted */
      *teid = 0;
      ret = RFAILED;
   }

   EG_RETVALUE(ret);

} /* End of egObtainFreeTeid () */

/*************************************************************************
 *      Fun:   egReleaseTeid
 *
 *      Desc:  egReleaseTeid is a local function to release a teid in use. 
 *
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
/*eg004.201 Modified for eGTP-C PSF Upgrade*/
#ifdef ANSI
PUBLIC S16 egReleaseTeid 
(
U32 teid                /* teid to be released. */
)
#else
PUBLIC S16 egReleaseTeid (teid)
U32 teid;               /* teid to be released. */
#endif
{
   CmLList *node; 

   EG_TRC2(egReleaseTeid);

   node = NULLP;

   node =  (CmLList *)egAlloc(sizeof(CmLList));
   if(node == NULLP)
   {
      EG_RETVALUE(ROUTRES);
   } 

   node->node = teid;
   cmLListAdd2Tail(&(egCb.tnlCb.freeTeidLst), node);

#ifndef ALIGN_64BIT
   EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,"Teid %ld released", teid));
#else
   EG_DBG_INFO(EG_DBG_TUNN, 0, (egp,"Teid %d released", teid));
#endif

   EG_RETVALUE(ROK);
} /* End of egReleaseTeid () */

/*************************************************************************
 *      Fun:   egGetTunnelCb
 *
 *      Desc:  egGetTunnelCb is function to get the tunnel Cb given the teid.
                 first get the array into which the teid falls into.
                 get the index ofthe teid in that array.
                 return the tunnel Cb.
 *
 *      Ret: This function returns the pointer to the tunnel endpoint
                 control block.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
/*eg004.201 Modified for eGTP-C PSF Upgrade*/
#ifdef ANSI
PUBLIC EgTECCb** egGetTunnelCb 
(
U32 teid                /* TEID of the tunnel. */
)
#else
PUBLIC EgTECCb** egGetTunnelCb (teid)
U32 teid;               /* TEID of the tunnel. */
#endif
{ 
   U32      tunnArr;
   U32      arrIdx;

   EG_TRC2(egGetTunnelCb);

   tunnArr = 0;
   arrIdx = 0;

   tunnArr = (teid/egCb.genCfg.maxTunlArrSz);
   arrIdx = teid % egCb.genCfg.maxTunlArrSz;

   if((egCb.tnlCb.arrNum > tunnArr) && egCb.tnlCb.teCCbArr[tunnArr])
   {
      EG_RETVALUE(&egCb.tnlCb.teCCbArr[tunnArr][arrIdx]);
   }/* end of if */

   EG_RETVALUE(NULLP);
}

/***************************** Utilities *************************************/
/*************************************************************************
 *      Fun:   egTunCreateSend
 *
 *      Desc:  The function egTunCreateSend creates a tunnel and sends the GTP-C
 *             request on that tunnel.
 *
 *      Ret: -
        - ROK -     If the procedure was successful.
        - ROUTRES - If the procedure failed due to memory failure.
        - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunCreateSend 
(
EgUSapCb *usap,         /* pointer to the USAP control block */
EgEvnt *event           /* the event representing the request message */
)
#else
PRIVATE S16 egTunCreateSend (usap, event)
EgUSapCb *usap;         /* pointer to the USAP control block */
EgEvnt *event;           /* the event representing the request message */
#endif
{
   S16          ret;
   U32          teid;
   EgTECCb      **teCCb;
   EgIeInfo     ieInfo;
   EgIe         *egIe;
   U16          iePos;
   EgtMsgType   msgType;
#ifdef EGTP_C_PTH_MGT
   EgDestCb     *destCb = (EgDestCb *)NULLP;
#ifdef EG_MULTI_IP
   /*-- eg007.201: Obtain tSapCb associated with USap --*/
   EgTSapCb     *tSapCb = usap->tSapCb;
#endif
#endif

   EG_TRC2(egTunCreateSend);

   ret = RFAILED;
   teid = 0;
   teCCb = NULLP;
   ieInfo.ieTypePres = TRUE;
   ieInfo.occrPres = FALSE;
   ieInfo.posPres = FALSE;
   ieInfo.refValue = FALSE;
   ieInfo.ieType   = EG_FTEID_IETYPE;
   ieInfo.ieInst   = 0;
   iePos = 0;
   egIe = NULLP;

   msgType = (EgtMsgType)event->u.egMsg->msgHdr.msgType;

   if ( (msgType != EGT_GTPC_MSG_CS_REQ) &&(msgType !=EGT_GTPC_MSG_MB_REQ)
        && (msgType != EGT_GTPC_MSG_FR_REQ) && (msgType != EGT_GTPC_MSG_CNTXT_REQ)
#if defined(EG_REL_930) || defined(EG_REL_AC0)
        && (msgType != EGT_GTPC_MSG_MBMS_SESS_START_REQ)
#endif
     )
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
      "Tunnel can be created only for Create Session Request."));
      event->resultType = EGT_ERR_MSG_INV_TYPE;
      egSendErrInd(event, usap);
      EG_RETVALUE(RFAILED);
   }
 
   /* Generate TEID */
   ret = egObtainFreeTeid(&teid);
   if( ret != ROK)
   {
      event->resultType = EGT_ERR_TEID_EXHAUSTED;
      egSendErrInd(event, usap);
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Teids exhausted"));
      EG_RETVALUE(ret);
   }/* end of if teid == EG_INVALID_TEID */

   /* Allocate tunnel context */

   teCCb = egGetTunnelCb(teid);
   if(teCCb == NULLP)
   {
      egReleaseTeid(teid);
      EG_RETVALUE(RFAILED);
   }

   EG_ALLOC(teCCb, sizeof(EgTECCb));
   if(*teCCb == NULLP)
   {
      egReleaseTeid(teid);
      /* delete egMsg and event */
      EG_FREE_GM_MSG(event->u.egMsg);
      egCmFreeEvent(event);
      EG_RETVALUE(ROUTRES);
   }/* end of if *teCCb == NULL*/

   /* eg010:210 : Initialize piggyback context */
#ifdef EG_PIGGYBACK_SUPP
   (*teCCb)->piggyBackStatus =  EG_PIGGYBACK_NONE;
   (*teCCb)->tempPiggyEvnt = NULLP;
#endif
   /* Update the FTEID in the event with generated TEID */

   /**********************************************************
    * eg009.201: in case of Dummy tunnel request, the IEs and*
    * the incReqCp won't be required
    * ********************************************************/
   if(event->crtFlag.val != EG_DUMMY_TUNN)
   {
      egUtilGetTrgtIe(&(event->u.egMsg->egIeList), &ieInfo, &egIe, &iePos);
      if(egIe == NULLP)
      {
         EG_FREE(*teCCb, sizeof(EgTECCb));
         egReleaseTeid(teid);

         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
                  "Failed to update TEID in FTEID. "));

         event->resultType = EGT_ERR_INV_IE_TYPE;
         egSendErrInd(event, usap);

         EG_RETVALUE(ret);
      } /* ifegIe == NULLP */

      egIe->t.fTeid.teid = teid;

      /* initialize inc req hash list */
      ret = cmHashListInit(&(*teCCb)->incReqCp, EG_MAX_HASH_SIZE_PER_TEID, 0,
            FALSE, /* do not allow dup keys*/
            CM_HASH_KEYTYPE_DEF, /* HL key type */
            egCb.init.region,
            egCb.init.pool);
      if(ret != ROK)
      {
         EG_FREE(*teCCb, sizeof(EgTECCb));
         egReleaseTeid(teid);
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
                  "Failed to initialize hash list for incoming requests."));
         /* delete egMsg and event */
         EG_FREE_GM_MSG(event->u.egMsg);
         egCmFreeEvent(event);
         SEND_HASH_FAIL_ALARM();
         EG_RETVALUE(ret);
      }/* end of if */
   }

   /* Save USap details (entity, interface, srvCb) */
   (*teCCb)->usap = usap;
   (*teCCb)->locTeid = teid;
   (*teCCb)->state = EGT_TUN_STATE_IDLE;
#ifdef EGTP_C_PTH_MGT
   cmMemset((U8 *)&((*teCCb)->destIpAddr),0,sizeof(CmTptAddr));
   egCpyCmTptAddr(&((*teCCb)->destIpAddr), &event->remAddr);
   EG_SET_PORT((*teCCb)->destIpAddr, EG_ZERO);
#ifdef EG_MULTI_IP
   /* eg007.201: Search dstCbList in TSAP Cb for destinaion IP */
   ret = cmHashListFind(&(tSapCb->dstCbList), (U8 *)&((*teCCb)->destIpAddr),\
                         sizeof(CmTptAddr),0,(PTR *)&destCb);
#else 
   ret = cmHashListFind(&egCb.dstCbList, (U8 *)&((*teCCb)->destIpAddr),\
                         sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif 
   if(destCb == NULLP)
   {
      EG_ALLOC(&destCb, sizeof(EgDestCb));
      if(destCb == NULLP)
      {
         egReleaseTeid(teid);
         EG_FREE_GM_MSG(event->u.egMsg);
         egCmFreeEvent(event);
         EG_RETVALUE(ROUTRES);
      }
      destCb->numTunn       = 1;
      destCb->echoReqSent   = 0;
      destCb->intfType      = event->intfType;
      /*-- eg006.201: init destReachable with TRUE --*/
      destCb->destReachable = TRUE;

      cmMemset((U8 *)&(destCb->destIpAddr),0,sizeof(CmTptAddr));
      egCpyCmTptAddr(&destCb->destIpAddr, &event->remAddr);
      EG_SET_PORT(destCb->destIpAddr, EG_ZERO);
#ifdef EG_MULTI_IP
      /* eg007.201: Search dstCbList in TSAP Cb for destinaion IP */
      ret = cmHashListInsert(&(tSapCb->dstCbList), (PTR)destCb, \
                     (U8*)&(destCb->destIpAddr), sizeof(CmTptAddr));
#else 
      ret = cmHashListInsert(&(egCb.dstCbList), (PTR)destCb, \
                     (U8*)&(destCb->destIpAddr), sizeof(CmTptAddr));
#endif 
      if (ret != ROK)
      {
         egReleaseTeid(teid);
         EG_FREE_GM_MSG(event->u.egMsg);
         egCmFreeEvent(event);
         EG_FREE(destCb, sizeof(EgDestCb));
         EG_RETVALUE(ROUTRES);
      } 
      /*********************************************************************** 
       *eg006.201: Do not start the timer for sending echo request here.Since the
       *session is not yet established.Only when a new tunnel is moved to 
       *connected state start sending echo request for it.
       *Echo time will be started only if DUMMY tunnel request is given
       ***********************************************************************/
      if((!egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb) &&
         (event->crtFlag.val == EG_DUMMY_TUNN))
      {
         ret = egSchedTmr(NULLP, EG_TMR_PATH_GLOBAL, TMR_START, \
                          egCb.genCfg.pathTmr.val);
         if (ret != ROK)
         {
            egCb.echoTmrStarted = FALSE;
            EG_DBG_ERR(0, 0, (egp,"\n  Starting of Echo Timer Failed"));
         }
         else
         {
            egCb.echoTmrStarted = TRUE;
         }
      }
   }
   else
   {
      /*********************************************************************** 
       * eg006.201: Increase associated number of Tunnels only if Destinaion *
       * is reachable, or create Flag is set to DUMMY. For other cases, Send *
       * Error to user when Tunnel is added to an unreachable destination and*
       * release the associated data                                         *
       ***********************************************************************/
      if((destCb->destReachable)||
         (event->crtFlag.val == EG_DUMMY_TUNN))
      {
         destCb->numTunn ++;
      }
      else
      {
         event->resultType = EGT_ERR_INV_TEID_RCVD;

         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
                  "TEID insertion attempted for Unreachable destination"));
         egReleaseTeid(teid);
         egSendErrInd(event, usap);

         EG_RETVALUE(EGT_ERR_INV_TEID_RCVD);
      } 

   }

   /*********************************************************************** 
    * eg006.201: For dummy tunnel request, move state to Connected and    *
    * send SigCfm to Application, update PSF-mapping & return without     *
    * invoking FSM, since no info is to be sent to peer for Dummy tunnel  *
    ***********************************************************************/
   if(event->crtFlag.val == EG_DUMMY_TUNN)
   {
      TknU32 transId;
      TknU32 teidVal;

      cmMemset((U8 *)&transId, 0 , sizeof(TknU32));
      cmMemset((U8 *)&teidVal, 0 , sizeof(TknU32));
      transId.pres = PRSNT_NODEF;
      transId.val = (*teCCb)->transId;

      teidVal.pres = PRSNT_NODEF;
      teidVal.val = (*teCCb)->locTeid;

   
      /* mark state as connected for DUMMY Tunnel */
      (*teCCb)->state=EGT_TUN_STATE_CONNECTED;

      /* increase the connected tunnedl count */
      destCb->connTunnCnt++;

      /* eg009.201 : Filled in proper response msg type into EgMsg for Dummy tunnel request */
      event->u.egMsg->msgHdr.msgType = egGetAssocMsg(msgType);

      /* send SigCfm to User indicating the TEID to application*/
      EgUiEgtSigCfm(&((*teCCb)->usap->pst), (*teCCb)->usap->suId, &(event->localAddr),
                                 &(event->remAddr), transId, teidVal, event->u.egMsg);

#ifdef HW
      (Void)HW_ADD_MAPPING(HW_EG_TEIDCB,(Void *)(*teCCb));
      HW_DBG_INFO(0,0, (hwp, "\n PSF Addmapping called for CbType=[%d],"
               " TeidValue=[%ld]",HW_EG_TEIDCB,(*teCCb)->locTeid));
#endif

      egCmFreeEvent(event);
      EG_RETVALUE(ret);
   }
#endif

      /* Invoke the FSM */
      ret = egTunnFsm[(*teCCb)->state][EG_TUN_EVENT_OUTSESSREQ]((*teCCb), event);
      if(ret != ROK)
      {
           egTunDelete(usap, teid, 0);
      }/* end of if */
      else
      {
         /* eg004.201 Update statistics when a new message is received on an existing binded USAP */
         
         /************************************************************
          * eg009.201 -Statistics is already update for upper sap in *
          * egTptProcessMsg hence here it is removed.                *
          *************************************************************/ 
         /*eg004.201 Add mapping for TeidCb when a new tunnel is created on ACTIVE node on receving a request from upper layer
          *with create flag enabled.
          */
#ifdef HW
         (Void)HW_ADD_MAPPING(HW_EG_TEIDCB,(Void *)(*teCCb));
	      HW_DBG_INFO(0,0, (hwp, "\n PSF Addmapping called for CbType=[%d],"
                       " TeidValue=[%ld]",HW_EG_TEIDCB,(*teCCb)->locTeid));
#endif
      }
      EG_RETVALUE(ret);

} /* End of egTunCreateSend () */

   /*************************************************************************
    *      Fun:   egTunCreateReceive
    *
    *      Desc:  The function egTunCreateReceive creates a tunnel and for a GTP-C
    *              Create Session Request that is received.
    *
    *      Ret: -
           - ROK -     If the procedure was successful.
           - ROUTRES - If the procedure failed due to memory failure.
           - RFAILED - If the procedure failed due to internal reasons.
    *
    *      Note:
    *
    *      File:   eg_tunn.c
   ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTunCreateReceive 
(
EgUSapCb *usap,         /* pointer to the USAP control block*/
EgEvnt *event           /* the event representing the request message */
)
#else
PRIVATE S16 egTunCreateReceive (usap, event)
EgUSapCb *usap;         /* the pointer to the SAP control block*/
EgEvnt *event;           /* the event representing the request message */
#endif
{
   S16          ret;
   U32          teid;
   EgtMsgType   msgType;
   EgTECCb      **teCCb;
#ifdef EGTP_C_PTH_MGT
      EgDestCb     *destCb = NULLP;
#ifdef EG_MULTI_IP
   /*-- eg007.201: Obtain tSapCb associated with USap --*/
   EgTSapCb     *tSapCb = usap->tSapCb;
#endif
#endif

   EG_TRC2(egTunCreateReceive);

   ret = RFAILED;
   teid = 0;
   teCCb = NULLP;
   msgType = (EgtMsgType)event->u.egMsg->msgHdr.msgType;

   /* Check if it is a Create Session Request 
      This is already done by EDM while decoding.
    */

   /* Check if the message has come on 2123 */

   if(event->srvCb->cfg.isDfltSrv == FALSE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Create Session Request received on \
               non - default port. "));
      event->resultType = EGT_ERR_INV_TUNN_NET_EVNT;
      egSendErrInd(event, usap);
      EG_RETVALUE(RFAILED);
   }

   /* Generate TEID */
   ret = egObtainFreeTeid(&teid);
   if( ret != ROK )
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"TEIDs exhausted."));
      event->resultType = EGT_ERR_TEID_EXHAUSTED;

      egSendErrInd(event, usap);

      EG_RETVALUE(ret);
   }/* end of if teid == EG_INVALID_TEID */

   /* Allocate tunnel context */

   teCCb = egGetTunnelCb(teid);
   if(teCCb == NULLP)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
               "Failed to find index for tunnel endpoint Cb."));
      egReleaseTeid(teid);

      /* eg009.201: free Memory for invalid Teid */
      EG_FREE_GM_MSG(event->u.egMsg);
      egCmFreeEvent(event);
      EG_RETVALUE(RFAILED);
   }

   EG_ALLOC(teCCb, sizeof(EgTECCb));
   if(*teCCb == NULLP)
   {
      egReleaseTeid(teid);
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
               "Failed to allocate memory for new tunnel endpoint Cb."));
      /* delete egMsg and event */
      EG_FREE_GM_MSG(event->u.egMsg);
      egCmFreeEvent(event);
      EG_RETVALUE(ROUTRES);
   }/* end of if */

   /* initialize inc req hash list */
   ret = cmHashListInit(&(*teCCb)->incReqCp, EG_MAX_HASH_SIZE_PER_TEID, EG_HLOFFSET,
         FALSE, /* do not allow dup keys*/
         CM_HASH_KEYTYPE_DEF, /* HL key type */
         egCb.init.region,
         egCb.init.pool);
   if(ret != ROK)
   {
      EG_FREE(*teCCb, sizeof(EgTECCb));
      egReleaseTeid(teid);
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
               "Failed to initialize hash list for incoming requests."));
      /* delete egMsg and event */
      EG_FREE_GM_MSG(event->u.egMsg);
      egCmFreeEvent(event);
      SEND_HASH_FAIL_ALARM();
      EG_RETVALUE(ret);
   }/* end of if */

   (*teCCb)->locTeid = teid;
   (*teCCb)->usap = usap;
   (*teCCb)->state = EGT_TUN_STATE_IDLE;

#ifdef EG_PIGGYBACK_SUPP
    /*eg012.201:Enhanced the piggy backing feature by adding the Timer*/
   egCmInitTimer(&((*teCCb)->tmrDisPbSigRsp));
#endif

#ifdef EGTP_C_PTH_MGT
   cmMemset((U8 *)&((*teCCb)->destIpAddr),0,sizeof(CmTptAddr));
   egCpyCmTptAddr(&((*teCCb)->destIpAddr), &event->remAddr);
   EG_SET_PORT((*teCCb)->destIpAddr, EG_ZERO);
#ifdef EG_MULTI_IP
   /* eg007.201: search dstCblist in TSAP Cb for destination IP */
   ret = cmHashListFind(&tSapCb->dstCbList, (U8 *)&((*teCCb)->destIpAddr),\
                         sizeof(CmTptAddr),0,(PTR *)&destCb);
#else 
   ret = cmHashListFind(&egCb.dstCbList, (U8 *)&((*teCCb)->destIpAddr),\
                         sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif 
   if(destCb == NULLP)
   {
      EG_ALLOC(&destCb, sizeof(EgDestCb));
      if(destCb == NULLP)
      {
         egReleaseTeid(teid);
         EG_FREE_GM_MSG(event->u.egMsg);
         egCmFreeEvent(event);
         EG_RETVALUE(ROUTRES);
      }
      destCb->numTunn       = 1;
      destCb->echoReqSent   = 0;
      destCb->intfType      = event->intfType;
      /*-- eg006.201: Init destReachable to TRUE --*/
      destCb->destReachable = TRUE;

      cmMemset((U8 *)&(destCb->destIpAddr),0,sizeof(CmTptAddr));
      egCpyCmTptAddr(&destCb->destIpAddr, &event->remAddr);
      EG_SET_PORT(destCb->destIpAddr, EG_ZERO);
#ifdef EG_MULTI_IP
      /* eg007.201: Insert destCb at dstCblist in TSAP Cb */
      ret = cmHashListInsert(&(tSapCb->dstCbList), (PTR)destCb, \
                     (U8*)&(destCb->destIpAddr), sizeof(CmTptAddr));
#else
      ret = cmHashListInsert(&(egCb.dstCbList), (PTR)destCb, \
                     (U8*)&(destCb->destIpAddr), sizeof(CmTptAddr));
#endif  
      if (ret != ROK)
      {
         egReleaseTeid(teid);
         EG_FREE_GM_MSG(event->u.egMsg);
         egCmFreeEvent(event);
         EG_FREE(destCb, sizeof(EgDestCb));
         EG_RETVALUE(ROUTRES);
      }
      if(!egCb.echoTmrStarted && egCb.genCfg.pathTmr.enb)
      {
         ret = egSchedTmr(NULLP, EG_TMR_PATH_GLOBAL, TMR_START, \
                          egCb.genCfg.pathTmr.val);
         if (ret != ROK)
         {
            egCb.echoTmrStarted = FALSE;
            EG_DBG_ERR(0, 0, (egp,"\n  Starting of Echo Timer Failed"));
         }
         else
         {
            egCb.echoTmrStarted = TRUE;
         }
      }
   }
   else
   {
      destCb->numTunn ++; 

      /*********************************************************************** 
       * eg006.201: Since we received the Response from a destination
       * which was previously unreachable, Send out Status Indication to
       * Application &  Alarm indicating that Path recovery layer
       ***********************************************************************/
      if(!destCb->destReachable)
      {
         /*-- eg006.201: Sending alarm to Layer manager */
         EgUsta         sta;  /* unsolicited status diagnostics structure */ 
         EgPathStatus  *status;

         destCb->destReachable = TRUE;

         EG_DBG_ERR (0, 0, (egp, "\n Destination IP reachable"));

         EG_ALLOC(&status, sizeof(EgPathStatus));
         /* eg012.201: added check for failure */
         if (NULLP == status)
         {
            EG_DBG_ERR (0, 0, (egp, "\n Memory Allocation Failed"));
            EG_RETVALUE(RFAILED);
         }
         status->pathInfo.staType = EGT_STATYPE_PATH;
         status->pathInfo.s.pathState.pres = PRSNT_NODEF;
         status->pathInfo.s.pathState.val  = EGT_STA_PATH_EST_SUCCESS;
         egCpyCmTptAddr(&(status->pathInfo.u.localAddr),&(destCb->destIpAddr));
         status->cause        = EGT_CAUSE_DST_REACHABLE;
         status->message.pres = PRSNT_NODEF;
         status->message.val  = msgType;
         status->dgn.pres     = NOTPRSNT;
                     EgUiEgtStaInd(&(usap->pst),usap->suId, \
                                        EGT_STA_PATH_EST_SUCCESS, status);


         cmMemset( (U8* )&sta, 0, sizeof(EgUsta));
         sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
         egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr),&(*teCCb)->destIpAddr);
         egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
               LEG_CAUSE_DST_IP_REACHABLE, &sta.dgn);
      }
   }
#endif

   /* Invoke the FSM */
   ret = egTunnFsm[(*teCCb)->state][EG_TUN_EVENT_INCSESSREQ]((*teCCb), event);
   if(ret != ROK)
   {
      /* appropriate alarm will be given in the fsm handler itself */
      egTunDelete(usap, teid, 0);
   }/* end of if */

/*
 *eg004.201 Add mapping for TeidCb when a incoming session request message received from peer egGTP node
 *resulting in addition of new tunnel Cb
 */
#ifdef HW
   else
   {
      (Void)HW_ADD_MAPPING(HW_EG_TEIDCB,(Void *)(*teCCb));
      HW_DBG_INFO(0,0, (hwp, "\n PSF Addmapping called for CbType=[%d],"
                             " TeidValue=[%ld]",HW_EG_TEIDCB,(*teCCb)->locTeid));
   }
#endif

   EG_RETVALUE(ret);
} /* End of egTunCreateReceive () */

/*************************************************************************
 *      Fun:   egTunnDelTECCb
 *
 *      Desc:  The function egTunnDelTECCb is a utility to free the tunnel
 *             control block context.
 *
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
/*eg004.201 Modified for eGTP-C PSF Upgrade:Function Prototype modified from PRIVATE to PUBLIC*/
#ifdef ANSI
PUBLIC Void egTunnDelTECCb 
(
EgTECCb *teCCb          /* pointer to the control block that has to be freed. */
)
#else
PUBLIC Void egTunnDelTECCb (teCCb)
EgTECCb *teCCb;         /* pointer to the control block that has to be freed. */
#endif
{
   EgIncReq *incReqNode;
   U32       arrNum;
   U32       arrIdx;
#ifdef EGTP_C_PTH_MGT
   S16       ret = RFAILED;
   EgDestCb  *destCb = NULLP;
#ifdef EG_MULTI_IP
   /*-- eg007.201: Obtain tSapCb associated with USap of TeCCb --*/
   EgTSapCb     *tSapCb = teCCb->usap->tSapCb;
#endif
#endif

   EG_TRC2(egTunnDelTECCb);

   incReqNode = NULLP;

   if(teCCb != NULLP)
   {
      arrNum = teCCb->locTeid / egCb.genCfg.maxTunlArrSz;
      arrIdx = teCCb->locTeid % egCb.genCfg.maxTunlArrSz;

#ifdef EGTP_C_PTH_MGT
#ifdef EG_MULTI_IP
   /* eg007.201: Search dstCbList in TSAP Cb for destination IP */
   ret = cmHashListFind(&(tSapCb->dstCbList), (U8 *)&(teCCb->destIpAddr),\
                         sizeof(CmTptAddr),0,(PTR *)&destCb);
#else 
   ret = cmHashListFind(&(egCb.dstCbList), (U8 *)&(teCCb->destIpAddr),\
                         sizeof(CmTptAddr),0,(PTR *)&destCb);
#endif 
   if(destCb != NULLP)
   {
      destCb->numTunn --;
      /* eg006.201: Decrement the number of connected tunnels */
      destCb->connTunnCnt--;
      if (destCb->numTunn == 0)
      {
#ifdef EG_MULTI_IP 
         cmHashListDelete(&tSapCb->dstCbList, (PTR)destCb);
#else 
         cmHashListDelete(&egCb.dstCbList, (PTR)destCb);
#endif     
         EG_FREE(destCb, sizeof(EgDestCb));
      }
   }
#endif
      while (cmHashListGetNext (&teCCb->incReqCp, (PTR) NULLP,
               (PTR *) &incReqNode) == ROK)
      {   
        /*****************************************
         * Delete the incReqNode from the hash list * 
         *****************************************/
/*eg004.201 Delete Mapping for incReCb when the assocaited tunnel Cb is deleted*/
#ifdef HW
         HW_RTUPD_DEL(HW_EG_INCREQCB,incReqNode);
         HW_DEL_MAPPING(HW_EG_INCREQCB,incReqNode);
#endif
         cmHashListDelete (&teCCb->incReqCp, (PTR) incReqNode);

        /*******************************
         * Free the memory for incReqNode *
         *******************************/
         EG_FREE(incReqNode, sizeof(EgIncReq));
      }
      cmHashListDeinit(&(teCCb->incReqCp));

      /* eg010:201: Free piggybacked buffer if present */
#ifdef EG_PIGGYBACK_SUPP
      /*  Stopping the wait timer for piggybacked SigReq */
	   if (teCCb->timeToWaitPbTmr != 0)
	   {
	      egSchedTmr(teCCb,EG_TMR_PIGGY_RSP,TMR_STOP,NOTUSED);
         teCCb->timeToWaitPbTmr = 0;
	   }
      if (teCCb->piggyBackStatus == EG_PIGGYBACK_QUEUE)
      {
         EG_FREEMBUF (teCCb->tempPiggyEvnt->u.mBuf);
         egCmFreeEvent(teCCb->tempPiggyEvnt);
      }
#endif /* EG_PIGGYBACK_SUPP */
      /*eg004.201 Delete Mapping for teid Cb when a tunnel is deleted at ACTIVE node 
       *and send RTU for tunnel deletion to STANDBY
       */
      /*In order to delete mapping for tunnel at the peer following steps must be done 
      1) Update the peer abt the TeidCB being deleted/
      2) Remove mapping for the DLL

      There may be cases where this function is called without addmapping being done for a 
      given control block. In such cases, PSF control block appended to each CB would be blank
      and CmPsf would return silently.*/
#ifdef HW
      HW_RTUPD_DEL(HW_EG_TEIDCB,teCCb);
      HW_DEL_MAPPING(HW_EG_TEIDCB,teCCb);
#endif

      EG_FREE(teCCb, sizeof(EgTECCb));

      /* Set the array index to NULL */
      egCb.tnlCb.teCCbArr[arrNum][arrIdx] = NULLP;
   }
   EG_RETVOID;
} /* End of egTunnDelTECCb () */

/*************************************************************************
 *      Fun:   egSetDefPortAddr
 *
 *      Desc:  The function egSetDefPortAddr is an utility to set the remote
 *             port to the default port.      .
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE Void egSetDefPortAddr 
(
EgEvnt *event     /* pointer to the event whose remote address is to be overwritten */
)
#else
PRIVATE Void egSetDefPortAddr (event)
EgEvnt *event;     /* pointer to the event whose remote address is to be overwritten */
#endif
{
   U16 port = 0;

   EG_TRC2(egSetDefPortAddr);

   switch(egCb.genCfg.nodeType)
   {
      case LEG_GTP_ENT_MME:
         {
            if(event->intfType == EGT_GTP_INT_S11)
               port = LEG_DEF_PORT_SGW;
            else if (event->intfType == EGT_GTP_INT_S3)/* UMAPATHI S3 interface check is missing*/
                port = LEG_DEF_PORT_MME;
            else if(event->intfType == EGT_GTP_INT_S10)
            {
               if(event->localAddr.type == CM_INET_IPV4ADDR_TYPE)
               {
                  port = (event->localAddr.u.ipv4TptAddr.port == LEG_DEF_PORT_MME) ?
                         LEG_DEF_PORT_MME2 : LEG_DEF_PORT_MME; 
               }
               else if(event->localAddr.type == CM_INET_IPV6ADDR_TYPE)
               {
                  port = (event->localAddr.u.ipv6TptAddr.port == LEG_DEF_PORT_MME) ?
                         LEG_DEF_PORT_MME2 : LEG_DEF_PORT_MME; 
               }
            }
         }
         break;
      case LEG_GTP_ENT_SGSN:
        {
            if (event->intfType == EGT_GTP_INT_S3)
                port = LEG_DEF_PORT_MME;
            else if(event->intfType == EGT_GTP_INT_S16)
                port = LEG_DEF_PORT_SGSN;
            /* CR ccpu00135305 fix */
            else if(event->intfType == EGT_GTP_INT_S4)
                port = LEG_DEF_PORT_SGW;
        }
        break;
      case LEG_GTP_ENT_SGW:
        {
            if(event->intfType == EGT_GTP_INT_S11)
                port = LEG_DEF_PORT_MME;
            else if(event->intfType == EGT_GTP_INT_S5_S8)
                port = LEG_DEF_PORT_PGW;
            /* CR ccpu00135305 fix */
            else if(event->intfType == EGT_GTP_INT_S4)
                port = LEG_DEF_PORT_SGSN;
        }
        break;
      case LEG_GTP_ENT_PGW:
        {
            if(event->intfType == EGT_GTP_INT_S5_S8)
                port = LEG_DEF_PORT_SGW;

#ifdef EG_REL_AC0
            if(event->intfType == EGT_GTP_INT_S2B)
                port = LEG_DEF_PORT_EPDG;
#endif
        }
        break;
        /*****************************
         * eg009.201 - Added for S2B *
         *****************************/
#ifdef EG_REL_AC0
      case LEG_GTP_ENT_EPDG:
        {
            if(event->intfType == EGT_GTP_INT_S2B)
                port = LEG_DEF_PORT_PGW;
        }
        break;
#endif
        /*****************************
         * eg009.201 - Added for S2A *
         *****************************/
#ifdef EG_S2A_SUPP
      case LEG_GTP_ENT_TWAN:
        {
            if(event->intfType == EGT_GTP_INT_S2A)
                port = LEG_DEF_PORT_TWAN;
        }
        break;
#endif
      default:
        break;
   }

   /*
        In this function remote port is overridden with default port. 
        While testing each entity is given differnt default port.  Hence
        port is set based on entity and interface. But for echo it will not
        be possible to do this since it does not come with an interface in
        current design. Hence sample application is expected to send proper
        port. But in real scenario it will override with correct port.
    */

   if(event->intfType == EGT_GTP_INT_UNKNWN)
      port = LEG_DEF_PORT_GTP;

   if(port != 0)
   {
      if(event->remAddr.type == CM_INET_IPV4ADDR_TYPE)
      {
         event->remAddr.u.ipv4TptAddr.port = port; 
      }
      else if(event->remAddr.type == CM_INET_IPV6ADDR_TYPE)
      {
         event->remAddr.u.ipv6TptAddr.port = port; 
      }
   }
   EG_RETVOID;
} /* End of egSetDefPortAddr() */

/* ccpu00137353 */

/*************************************************************************
 *      Fun:   egTransConnSigReq
 *
 *      Desc:  The function egTransConnSigReq handles the outgoing Transparent Sig Req.
 *
 *      Ret: -
 *       - ROK -     If the procedure was successful.
 *       - ROUTRES - If the procedure failed due to memory failure.
 *       - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTransConnSigReq
(
EgUSapCb *usap,         /* usap cb which the message belongs to. */
EgEvnt *event          /* outgoing Echo request. */
)
#else
PRIVATE S16 egTransConnSigReq ( usap, event)
EgUSapCb *usap;         /* usap cb which the message belongs to. */
EgEvnt *event;          /* outgoing Echo request. */
#endif
{
   
   S16 ret=ROK;
   Bool validMsg=TRUE;

   EG_TRC2(egTransConnSigReq);

   /*validate the teid */
   if(!(event->u.egMsg->msgHdr.teidPres) || (event->u.egMsg->msgHdr.teid))
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "For the Message %d invalid teid  %d is not valid ",
        event->u.egMsg->msgHdr.msgType, (int)event->u.egMsg->msgHdr.teid));
        event->resultType = EGT_ERR_INV_TEID;
      egSendErrInd(event,usap);
      EG_RETVALUE(RFAILED);
   }

   /* Validate the interface */
   validMsg = egValidateMsgInt((EgtMsgType)event->u.egMsg->msgHdr.msgType,
                                   (LegEntity)egCb.genCfg.nodeType,
                               (LegEntity)LEG_GTP_ENT_UNKNWN,
                               (EgtIntf) event->intfType);
   if(validMsg != TRUE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "The Message %d is invalid on the interface %d.",
        event->u.egMsg->msgHdr.msgType, event->intfType));
        event->resultType = EGT_ERR_MSG_INV_INT;
      egSendErrInd(event,usap);
      EG_RETVALUE(RFAILED);
   }/* end of if */

   /* send to edm */
   event->srvCb = egCb.egtSapLst[usap->cfg.sapId]->tptSrvCb;
   /* override the remote port to 2123 */
   egSetDefPortAddr(event);

   ret = egEncMsgReq(event);
   if(ret != ROK)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
      /* result type filled by EDM */
      egSendErrInd(event, usap);
      /***********************************************
      * Sending the error indication is successful, *
      * So Return from here with ROK                *
      * ***********************************************/
      ret = ROK;
   }
     EG_RETVALUE( ret ); 

} /* End of egTransConnSigReq () */

/*************************************************************************
 *      Fun:   egTransConnSigRsp
 *
 *      Desc:  The function egTransConnSigRsp  handles the outgoing Delete PDN Connection Set Request.
 *
 *      Ret: -
 *       - ROK -     If the procedure was successful.
 *       - ROUTRES - If the procedure failed due to memory failure.
 *       - RFAILED - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 egTransConnSigRsp
(
EgUSapCb *usap,         /* usap cb which the message belongs to. */
EgEvnt *event          /* outgoing Echo request. */
)
#else
PRIVATE S16 egTransConnSigRsp ( usap, event)
EgUSapCb *usap;         /* usap cb which the message belongs to. */
EgEvnt *event;          /* outgoing Echo request. */
#endif
{
   
   S16 ret=ROK;
   Bool validMsg=TRUE;

   EG_TRC2(egDelPdnConnSetRsp);

   /*validate the teid */
   if(!(event->u.egMsg->msgHdr.teidPres) || (event->u.egMsg->msgHdr.teid))
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "For the Message %d invalid teid  %d is not valid ",
        event->u.egMsg->msgHdr.msgType, (int)event->u.egMsg->msgHdr.teid));
        event->resultType = EGT_ERR_INV_TEID;
      egSendErrInd(event,usap);
      EG_RETVALUE(RFAILED);
   }

   /* Validate the interface */
   validMsg = egValidateMsgInt((EgtMsgType)event->u.egMsg->msgHdr.msgType,
                                   (LegEntity)egCb.genCfg.nodeType,
                               (LegEntity)LEG_GTP_ENT_UNKNWN,
                               (EgtIntf) event->intfType);
   if( validMsg != TRUE)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,\
        "The Message %d is invalid on the interface %d.",
        event->u.egMsg->msgHdr.msgType, event->intfType));
        event->resultType = EGT_ERR_MSG_INV_INT;
      egSendErrInd(event,usap);
      EG_RETVALUE(RFAILED);
   }/* end of if */

   /* send to edm */
   event->srvCb = egCb.egtSapLst[usap->cfg.sapId]->tptSrvCb;
   /* override the remote port to 2123 */
   egSetDefPortAddr(event);

   ret = egEncMsgReq(event);
   if(ret != ROK)
   {
      EG_DBG_ERR(EG_DBG_TUNN, 0, (egp,"Encoding of the message failed. "));
      /* result type filled by EDM */
      egSendErrInd(event, usap);
      /***********************************************
      * Sending the error indication is successful, *
      * So Return from here with ROK                *
      * ***********************************************/
      ret = ROK;
   }
     EG_RETVALUE( ret ); 

} /* End of egTransConnSigReq () */


#endif
/*************************************************************************
 *      Fun:   egCpyCmTptAddr
 *
 *      Desc:  The function egCpyCmTptAddr is the utility to copy the CmTptAddr
 *             from source to destination.
 *
 *      Ret: 
             - ROK      - If the procedure was successful.
             - ROUTRES  - If the procedure failed due to resource failure.
             - RFAILED  - If the procedure failed due to internal reasons.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egCpyCmTptAddr 
(
CmTptAddr *dst,         /* pointer to the destination CmTptAddr. */
CmTptAddr *src          /* pointer to the source CmTptAddr. */
)
#else
PUBLIC S16 egCpyCmTptAddr (dst, src)
CmTptAddr *dst;         /* pointer to the destination CmTptAddr. */
CmTptAddr *src;         /* pointer to the source CmTptAddr. */
#endif
{
   EG_TRC2(egCpyCmTptAddr);

#ifdef ERRCHK
   if((src == NULLP) || (dst == NULLP))
        EG_RETVALUE(RFAILED);
#endif

   dst->type = src->type;
   if(src->type == CM_INET_IPV4ADDR_TYPE)
   {
      dst->u.ipv4TptAddr.port = src->u.ipv4TptAddr.port; 
      dst->u.ipv4TptAddr.address = src->u.ipv4TptAddr.address;
   }
#ifdef IPV6_SUPPORTED 
   else if(src->type == CM_INET_IPV6ADDR_TYPE)
   {
      dst->u.ipv6TptAddr.port = src->u.ipv6TptAddr.port; 
      cmMemcpy((U8*)dst->u.ipv6TptAddr.ipv6NetAddr, (U8*)src->u.ipv6TptAddr.ipv6NetAddr,
                 sizeof(CmInetIpAddr6));
   }
#endif
   EG_RETVALUE( ROK);
} /* End of egCpyCmTptAddr () */

#ifdef EGTP_U

/*************************************************************************
 *      Fun:   egUTunnTeidMod
 *
 *      Desc:  This function adds the destination IP if not already
 *             added. It adds a TEID to the mention destination. 
 *
 *      Ret:    EGT_GTPU_CAUSE_INV_DST_IP : Destination IP not configured.
 *              EGT_GTPU_CAUSE_INV_TEID : TEID not configured.
 *              EGT_GTPU_CAUSE_NO_RES_AVAIL : No resources available.
 *
 *      Note:   none
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUTunnTeidMod
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb              /* IP Control Block*/
)
#else
PUBLIC S16 egUTunnTeidMod (egtUEvnt, ipCb)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgIpCb   *ipCb;           /* IP Control Block*/
#endif
{
   EgDestAddrCb *dstCb;         /*Target Destination CB*/
   EgDestAddrCb *frmDstCb;      /*Source Destination CB*/
   EgTeIdCb     *teidCb;        /*TEID CB*/
   CmTptAddr    keyAddr;
   EgUsta sta;                  /*For Alarm generation*/
   S16 ret;                     /*Return value*/

   EG_TRC2(egUTunnTeidMod);

   dstCb = NULLP;
   frmDstCb = NULLP;
   teidCb = NULLP;
   ret = ROK;

   /**********************************************************
    * Retrieve  destination CB from which teid is to be moved* 
    **********************************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.frmDstIpAddr);

   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&frmDstCb);

   if (frmDstCb == NULLP)
   {
   /*****************************
    *Destination does not exist *
    *****************************/
      EG_DBG_ERR (0, 0, (egp, "\n Destination IP not configured"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_DST_IP);
   }

   /***********************
    *Retrieve the TEID CB *
    ***********************/
   ret = cmHashListFind(&(frmDstCb->teIdLst),
         (U8 *)&(egtUEvnt->u.tnlMgmt.teId),
         sizeof(U32), 0, (PTR *)&teidCb);

   if (teidCb == NULLP)
   {
   /************************
    * TEID already deleted * 
    ************************/
      EG_DBG_ERR (0, 0, (egp, "\n TEID not configured"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_TEID);
   }

#ifdef EGTP_U_MULTI_THREADED
  if (teidCb->tnlPendOp == TRUE)
   {
   /******************************************
    * TEID control operation is in progress  * 
    ******************************************/
      EG_DBG_ERR (0, 0, (egp, "\n TEID control operation is in progress"));
      EG_RETVALUE(EGT_EGTPU_CAUSE_PENDING_CNTRL_OP);
   }
#endif /* EGTP_U_MULTI_THREADED */
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
   EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

   /*******************************************************
    *Free the TEID from the hash list of this destination * 
    *******************************************************/
   cmHashListDelete(&(frmDstCb->teIdLst), (PTR)teidCb);
   if(frmDstCb->teIdLst.nmbEnt == 0)
   {
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(frmDstCb->destIpAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
            LEG_CAUSE_DST_IP_DEL, &sta.dgn);
      EGU_DELETE_DEST(ipCb, frmDstCb);
   }

   /************************************
    * Retrieve target destination CB   * 
    ************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);
   
   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if (dstCb == NULLP)
   {
      /******************************
       * Destination not configured * 
       ******************************/
      EG_ALLOC(&dstCb, sizeof(EgDestAddrCb));

      if(dstCb == NULLP)
      {
#ifdef SS_MT_CB
         /* Multi threaded changes for performance */
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
      }
      /********************************
       *Initialise the Teid Hash List *
       ********************************/
      ret = cmHashListInit(&dstCb->teIdLst,     /* messages */
            EG_MAX_TEID_HASH_SIZE,     /* HL bins for the msgs */
            EG_HLOFFSET,          /* Offset of HL Entry */
            FALSE,                /* Allow dup. keys ? */
            CM_HASH_KEYTYPE_U32MOD,  /* HL key type */
            egCb.init.region,     /* Mem region for HL */
            egCb.init.pool);      /* Mem pool for HL */

      if(ret != ROK)
      {

         /*********************************************************
          *Could not initialise hash list, delete the destination *
          *********************************************************/
         EG_FREE(dstCb, sizeof(EgDestAddrCb));
         EG_FREE(teidCb, sizeof(EgTeIdCb));
#ifdef SS_MT_CB
         /* Multi threaded changes for performance */
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
      }

      /**********************************************************
       *Copy destination IP Address, check if multicast address *
       *check interface type.                                   * 
       **********************************************************/
      cmMemset((U8 *)&(dstCb->destIpAddr), 0 ,sizeof(CmTptAddr));
      EGU_SET_IP(dstCb->destIpAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);
      EGU_CHECK_MULTICAST_IP(dstCb);
      dstCb->intfType = egtUEvnt->u.tnlMgmt.intfType;
      dstCb->maxTPduSize = egtUEvnt->u.tnlMgmt.maxTPduSize;
      dstCb->destReachable = TRUE;

      /*********************************************************
       * Insert the destination into Hash list of Source IP CB * 
       *********************************************************/
      ret = cmHashListInsert(&(ipCb->destAddrCbLst),
            (PTR)dstCb,
            (U8 *)&(dstCb->destIpAddr),
            sizeof(CmTptAddr));

      if(ret !=ROK)
      {
         EGU_DELETE_DEST2(dstCb);
         EG_FREE(teidCb, sizeof(EgTeIdCb));
#ifdef SS_MT_CB
         /*   Multi threaded changes for performance */
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
      }
   }
   /*********************************************************
    * Insert the TEID into Hash list of Destination IP CB * 
    *********************************************************/
   ret = cmHashListInsert(&(dstCb->teIdLst),
         (PTR)teidCb,
         (U8 *)&(teidCb->teId),
         sizeof(U32));

   if(ret !=ROK)
   {
      if(dstCb->teIdLst.nmbEnt == 0)
      {
         EGU_DELETE_DEST(ipCb, dstCb);
      }
      EG_FREE(teidCb, sizeof(EgTeIdCb));
#ifdef SS_MT_CB
         /*   Multi threaded changes for performance */
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
   }
   /*******************************
    * Increment the TEID counter  * 
    *******************************/
#ifdef SS_MT_CB
         /*   Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
   EG_RETVALUE(ROK);

}

/*************************************************************************
 *      Fun:   egUTunnTeidDel
 *
 *      Desc:  This function deletes a teid from the destination. 
 *
 *      Ret:    EGT_GTPU_CAUSE_INV_DST_IP : Destination IP not configured.
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUTunnTeidDel
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb,              /* IP Control Block*/
EgUSapCb *usap            /*Upper Sap CB*/
)
#else
PUBLIC S16 egUTunnTeidDel (egtUEvnt, ipCb, usap)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgIpCb *ipCb;              /* IP Control Block*/
EgUSapCb *usap;            /*Upper Sap CB*/
#endif
{
   EgDestAddrCb *dstCb = NULLP;
   EgTeIdCb     *teidCb = NULLP;
   EgUsta  sta;
   S16 ret = ROK;
   CmTptAddr  keyAddr;
#ifdef EGTP_U_MULTI_THREADED
   EgUEvnt    *eguEvnt = NULLP;
   Pst        wrkrPst;
   S16        cnt = 0;
#endif /* EGTP_U_MULTI_THREADED */

   EG_TRC2(egUTunnTeidDel);


#ifdef EGTP_U_MULTI_THREADED
   cmMemset((U8* )&wrkrPst, 0, sizeof(Pst));
   cmMemset((U8* )&sta, 0, sizeof(EgUsta));
#endif /* EGTP_U_MULTI_THREADED */

   /**************************************
    * Find the destination Control Block * 
    **************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);

   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if (dstCb == NULLP)
   {
      /*****************************
       *Destination does not exist *
       *****************************/
      EG_DBG_ERR(0, 0, (egp,
          "\n  Destination does not exist"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_DST_IP);
   }

   ret = cmHashListFind(&(dstCb->teIdLst),
         (U8 *)&(egtUEvnt->u.tnlMgmt.teId),
         sizeof(U32), 0, (PTR *)&teidCb);

   if (teidCb == NULLP)
   {
      /************************
       * TEID already deleted * 
       ************************/
#ifdef ALIGN_64BIT
      EG_DBG_ERR(0, 0, (egp,"\n TEID already deleted (%d)",
          egtUEvnt->u.tnlMgmt.teId));
#else
      EG_DBG_ERR(0, 0, (egp,"\n TEID already deleted (%ld)",
          egtUEvnt->u.tnlMgmt.teId));
#endif
     /*-- eg003.201 : updated proper Error code --*/
     EG_RETVALUE(EGT_GTPU_CAUSE_INV_TEID);      
   }

#ifdef EGTP_U_MULTI_THREADED
   if (teidCb->tnlPendOp == TRUE)
   {
      /************************
       * TEID already deleted * 
       ************************/
      EG_RETVALUE(EGT_EGTPU_CAUSE_PENDING_CNTRL_OP);
   }
   /*************************************************************
    *        Allocate the Event Structure                *
    *************************************************************/
   EGU_ALLOCEVNT (&eguEvnt);

   /**************************************************************
    * If unable to allocate event structure log error and return *
    **************************************************************/
   if (eguEvnt == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp, "\n Allocation of event failed "));

      EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
   }

/*   Multi threaded changes for performance */
#ifdef SS_MT_CB
   EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
   eguEvnt->teIdCb = teidCb;
   eguEvnt->destAddrCb = dstCb;
   eguEvnt->ipCb= ipCb;
   eguEvnt->usap = usap;
   eguEvnt->teIdCb->tnlPendOp = TRUE;
   egCpyCmTptAddr(&(eguEvnt->localAddr), &(egtUEvnt->u.tnlMgmt.srcAddr));
   egCpyCmTptAddr(&(eguEvnt->remAddr), &(egtUEvnt->u.tnlMgmt.dstIpAddr));


   if (teidCb->reOrdering)
   {

      /*****************************************************************
       * If re-ordering is TRUE and thread Cb is NULLP, no need to post*
       * to worker thread as there will be no messages in the tunnel   *
       * buffer. Continue with tunnel deletion and return.             *
       *****************************************************************/
      if(teidCb->egThreadCb == NULLP)
      {
    /**********************************************
     * Deallocate the re-order buffer and seqInfo *
     **********************************************/
    egFree((Data*)(teidCb->seqInfo->pduArray),(sizeof(EgtUEvnt *) * egCb.genCfg.pduWindSize));
    egFree((Data*)(teidCb->seqInfo),sizeof(SeqInfo));
    cmHashListDelete(&(dstCb->teIdLst), (PTR)teidCb);
    EG_FREE(teidCb, sizeof(EgTeIdCb));

    /*******************************
     * Decrement the TEID counter  * 
     *******************************/

    if(dstCb->teIdLst.nmbEnt == 0)
    {
       /***********************************
        * Send Alarm to the layer manager *
        ***********************************/
       cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

       sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
       egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(dstCb->destIpAddr));
       egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LEG_CAUSE_DST_IP_DEL, &sta.dgn);
       /*******************************************************************
        * No TEID for this destination, so the destination can be deleted * 
        *******************************************************************/
       EGU_DELETE_DEST(ipCb, dstCb);
    }
    EGU_FREEEVNT(eguEvnt);
#ifdef SS_MT_CB
    EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
    EG_RETVALUE(EGT_EGTPU_CAUSE_RETURN_OK);
      }
      egDeleteTun(teidCb);
      /************************************************************
       * Reordering enabled so post only to the respective thread * 
       ************************************************************/
       eguEvnt->eventType = EGTEVTTNLDELREQ;
      ret = egUPackSendMTMsg(eguEvnt, teidCb->egThreadCb->thrId, EGTEVTTNLDELREQ);

      if(ret != ROK)
      {
          EGU_FREEEVNT(eguEvnt);
#ifdef SS_MT_CB
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
          EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
      }
      eguEvnt->teIdCb->nmbReqToWrkThr++;
   } /* end of if reOredring present */
   else
   {

      /*************************************************
       * Reordering not enabled so post to all threads * 
       *************************************************/
       eguEvnt->eventType = EGTEVTTNLDELREQ;
      for(cnt = 0; cnt < egCb.genCfg.nmbWorkerThread; cnt++)
      {
         ret = egUPackSendMTMsg(eguEvnt, (cnt+1), EGTEVTTNLDELREQ);
         if(ret != ROK)
         {
            if(eguEvnt->teIdCb->nmbReqToWrkThr == 0)
            {
               eguEvnt->teIdCb->notCmpltOp = FALSE;
               eguEvnt->teIdCb->tnlPendOp = FALSE;
               EGU_FREEEVNT(eguEvnt);
#ifdef SS_MT_CB
               EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
               EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
            }
            else
            {
               eguEvnt->teIdCb->notCmpltOp = TRUE;
            }
         }
         eguEvnt->teIdCb->nmbReqToWrkThr++;
      }
   }

#ifdef SS_MT_CB
   EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
#else /* EGTP_U_MULTI_THREADED */

   if (teidCb->reOrdering)
   {
      /*****************************
       * Flush the re-order buffer * 
       *****************************/
      egFlushAllPackets(teidCb);
      /**********************************************
       * Deallocate the re-order buffer and seqInfo *
       **********************************************/
      egFree((Data*)*(teidCb->seqInfo->pduArray),(sizeof(EgtUEvnt *) * egCb.genCfg.pduWindSize));
      egFree((Data*)(teidCb->seqInfo),sizeof(SeqInfo));
   }
   /****************
    *Free the TEID * 
    ****************/
/* eg001.201 Decrementing respective Tunnel stats on tunnel deletion */
#ifdef EGTP_U_REL_9
   if(teidCb->tunnelType == EG_NORMAL_DATA_TUNN)
   {
       dstCb->normaldataTunnCnt--;
       egCb.genSts.numNrmlDataTunn--;
   }
   else if(teidCb->tunnelType == EG_FWD_DATA_TUNN)
   {
       dstCb->forwardingTunnCnt--;
       egCb.genSts.numFwdDataTunn--;
   }
#endif
   cmHashListDelete(&(dstCb->teIdLst), (PTR)teidCb);
   EG_FREE(teidCb, sizeof(EgTeIdCb));

   /*******************************
    * Decrement the TEID counter  * 
    *******************************/

   if(dstCb->teIdLst.nmbEnt == 0)
   {
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(dstCb->destIpAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LEG_CAUSE_DST_IP_DEL, &sta.dgn);
      /*******************************************************************
       * No TEID for this destination, so the destination can be deleted * 
       *******************************************************************/
      EGU_DELETE_DEST(ipCb, dstCb);
   }

   egCb.genSts.numActvTnls--;
#endif /* EGTP_U_MULTI_THREADED */
   EG_RETVALUE(ROK);

}

/*************************************************************************
 *      Fun:   egUTunnTeidReordDis
 *
 *      Desc:  This function diables the re-ordering functionality
 *             for a thread dynamically. 
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUTunnTeidReordDis
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb,              /* IP Control Block*/
EgUSapCb *usap           /*Upper Sap CB*/
)
#else
PUBLIC S16 egUTunnTeidReordDis (egtUEvnt, ipCb, usap)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgIpCb *ipCb;              /* IP Control Block*/
EgUSapCb *usap;           /*Upper Sap CB*/
#endif
{
   EgDestAddrCb *dstCb;
   EgTeIdCb     *teidCb;
   CmTptAddr keyAddr;
   S16 ret;
#ifdef EGTP_U_MULTI_THREADED
   EgUEvnt    *eguEvnt=NULLP;
   Pst        wrkrPst;
#endif /* EGTP_U_MULTI_THREADED */

   EG_TRC2(egUTunnTeidReordDis);

   dstCb = NULLP;
   teidCb = NULLP;
   ret = ROK;

#ifdef EGTP_U_MULTI_THREADED
   cmMemset((U8* )&wrkrPst, 0, sizeof(Pst));
#endif /* EGTP_U_MULTI_THREADED */


   /***********************************
    * Check if destination configured * 
    ***********************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);

   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if (dstCb == NULLP)
   {
      /*****************************
       *Destination does not exist *
       *****************************/
      EG_DBG_ERR (0, 0, (egp, "\n Invalid Destination IP"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_DST_IP);
   }
   ret = cmHashListFind(&(dstCb->teIdLst),
         (U8 *)&(egtUEvnt->u.tnlMgmt.teId),
         sizeof(U32), 0, (PTR *)&teidCb);

   if (teidCb == NULLP)
   {
      /************************
       * Invalid TEID. Return * 
       ************************/
      EG_DBG_ERR (0, 0, (egp, "\n Invalid Teid"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_TEID);
   }

   /******************************************
    * If re-ordering already disabled, return*
    ******************************************/
   if(!teidCb->reOrdering)
   {
      EG_RETVALUE(EGT_EGTPU_CAUSE_RETURN_OK);
   }
#ifdef EGTP_U_MULTI_THREADED

   /****************************************************
    *Thread not assigned so re-ordering queue is empty *
    *so return from here.                              *
    ****************************************************/
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
   EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

   if(teidCb->egThreadCb == NULLP)
   {
      /*********************************
       * Deallocate the re-prder buffer *
       *********************************/
      egFree((Data*)(teidCb->seqInfo->pduArray),(sizeof(EgtUEvnt *)*egCb.genCfg.pduWindSize));
      egFree((Data*)(teidCb->seqInfo),sizeof(SeqInfo));
      /***************************
       * set reOrdering to FALSE *
       **************************/
      teidCb->reOrdering = FALSE;
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_EGTPU_CAUSE_RETURN_OK);
   }

   if (teidCb->tnlPendOp == TRUE)
   {
      /******************************************
       * TEID control operation is in progress  * 
       ******************************************/
      EG_DBG_ERR (0, 0, (egp, "\n TEID control operation is in progress"));
#ifdef SS_MT_CB
      /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_EGTPU_CAUSE_PENDING_CNTRL_OP);
   }

 /*************************************************************
    *        Allocate the Event Structure                *
    *************************************************************/
   EGU_ALLOCEVNT (&eguEvnt);

   /**************************************************************
    * If unable to allocate event structure log error and return *
    **************************************************************/
   if (eguEvnt == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp, "\n Allocation of event failed "));
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
   }


   eguEvnt->teIdCb = teidCb;
   eguEvnt->destAddrCb = dstCb;
   eguEvnt->usap = usap;
   eguEvnt->teIdCb->tnlPendOp = TRUE;
   egCpyCmTptAddr(&(eguEvnt->localAddr), &(egtUEvnt->u.tnlMgmt.srcAddr));
   egCpyCmTptAddr(&(eguEvnt->remAddr), &(egtUEvnt->u.tnlMgmt.dstIpAddr));
   egDeleteTun(teidCb);
   ret = egUPackSendMTMsg(eguEvnt, teidCb->egThreadCb->thrId, EGTEVTTNLREORDDIS);

   if(ret != ROK)
   {
      EGU_FREEEVNT(eguEvnt);
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
   }

   eguEvnt->teIdCb->nmbReqToWrkThr++;
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
#else /* EGTP_U_MULTI_THREADED */
   /*****************************
    * Flush the re-order buffer * 
    *****************************/
   egFlushAllPackets(teidCb);
   /*********************************
    * Deallocate the re-prder buffer *
    *********************************/
   egFree((Data*)(teidCb->seqInfo->pduArray),(sizeof(EgtUEvnt *)*egCb.genCfg.pduWindSize));
   egFree((Data*)(teidCb->seqInfo),sizeof(SeqInfo));
   /***************************
    * set reOrdering to FALSE *
    **************************/
   teidCb->reOrdering = FALSE;

#endif

   EG_RETVALUE(ROK);
}
/*************************************************************************
 *      Fun:   egUTunnTeidReordEna
 *
 *      Desc:  This function enables the re-ordering functionality
 *             for a thread dynamically. 
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUTunnTeidReordEna
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb              /* IP Control Block*/
)
#else
PUBLIC S16 egUTunnTeidReordEna (egtUEvnt, ipCb)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgIpCb   *ipCb;           /* IP Control Block*/
#endif
{
   EgDestAddrCb *dstCb;
   EgTeIdCb     *teidCb;
   CmTptAddr    keyAddr;
   S16 ret;

   EG_TRC2(egUTunnTeidReordEna);

   dstCb = NULLP;
   teidCb = NULLP;
   ret = ROK;

   /*******************************************
    * Check if destination already configured * 
    *******************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);
   
   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if (dstCb == NULLP)
   {
      /*****************************
       *Destination does not exist *
       *****************************/
      EG_DBG_ERR (0, 0, (egp, "\n Invalid Destination"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_DST_IP);
   }
   ret = cmHashListFind(&(dstCb->teIdLst),
         (U8 *)&(egtUEvnt->u.tnlMgmt.teId),
         sizeof(U32), 0, (PTR *)&teidCb);

   if (teidCb == NULLP)
   {
      /********************
       * TEID not present * 
       ********************/
      EG_DBG_ERR (0, 0, (egp, "\n Invalid TEID"));
      EG_RETVALUE(EGT_GTPU_CAUSE_INV_TEID);
   }

#ifdef EGTP_U_MULTI_THREADED
  if (teidCb->tnlPendOp == TRUE)
   {
   /******************************************
    * TEID control operation is in progress  * 
    ******************************************/
      EG_DBG_ERR (0, 0, (egp, "\n TEID control operation is in progress"));
      EG_RETVALUE(EGT_EGTPU_CAUSE_PENDING_CNTRL_OP);
   }
#endif /* EGTP_U_MULTI_THREADED */

   /******************************************************
    *Re-ordering already enabled for this tunnel. Return *
    ******************************************************/
   if(teidCb->reOrdering)
   {
      EG_RETVALUE(ROK);
   }

#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
   EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

   teidCb->reordAlgo = egtUEvnt->u.tnlMgmt.reordAlgo;
   teidCb->reOrdering = TRUE;
    /*********************************
    * Initialise the reOrdering Timer *
    ***********************************/
    egCmInitTimer(&teidCb->reOrderTimer);
#ifdef EGTP_U_MULTI_THREADED 
    egCmInitTimer(&teidCb->tnlDelTimer);
#endif /* EGTP_U_MULTI_THREADED */
   /****************************************
    * Allocate seqInfo and re-order buffer *
    ****************************************/
   teidCb->seqInfo = (SeqInfo *)egAlloc(sizeof(SeqInfo));
   if(teidCb->seqInfo == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp,"\n Cannot allocate memory for SeqInfo\n"));
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(RFAILED);
   }
   teidCb->seqInfo->expSeqNmb = egtUEvnt->u.tnlMgmt.expSeqNmb;
   teidCb->seqInfo->pduArray = (EgtUEvnt **)egAlloc((sizeof(EgtUEvnt *))* (egCb.genCfg.pduWindSize));
   if(teidCb->seqInfo->pduArray == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp,"\n Cannot allocate memory for reordering bufer\n"));
#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(RFAILED);
   }

#ifdef EGTP_U_MULTI_THREADED
   teidCb->egThreadCb = NULLP;
   /* Thread assignment is differed until receive a packet for it
    *  egAssignThrd(teidCb);
    */

#ifdef SS_MT_CB
   /* Multi threaded changes for performance */
   EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
#endif
   EG_RETVALUE(ROK);
}
/*************************************************************************
 *      Fun:   egUTunnTeidAdd
 *
 *      Desc:  This function adds the destination IP if not already
 *             added. It adds a TEID to the mention destination. 
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUTunnTeidAdd
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb              /* IP Control Block*/
)
#else
PUBLIC S16 egUTunnTeidAdd (egtUEvnt, ipCb)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgIpCb   *ipCb;           /* IP Control Block*/
#endif
{
   EgDestAddrCb *dstCb;
   EgTeIdCb     *teidCb;
   CmTptAddr  keyAddr;
   S16 ret;

   EG_TRC2(egUTunnTeidAdd);

   dstCb = NULLP;
   teidCb = NULLP;
   ret = ROK;

   /******************************************
    * eg009.201 - Fix for TEID zero is valid *
    ******************************************/
   
   /*******************************************
    * Check if destination already configured * 
    *******************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);

   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);


   /* Multi threaded changes for performance */
#ifdef SS_MT_CB
   EG_WRLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */

   if (dstCb == NULLP)
   {
      /******************************
       * Destination not configured * 
       ******************************/
      EG_ALLOC(&dstCb, sizeof(EgDestAddrCb));

      if(dstCb == NULLP)
      {
         EG_DBG_ERR(0, 0, (egp,
                  "\n  Memory Allocation for destination failed"));
#ifdef SS_MT_CB
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
      }
      /********************************
       *Initialise the Teid Hash List *
       ********************************/
      ret = cmHashListInit(&dstCb->teIdLst,     /* messages */
            EG_MAX_TEID_HASH_SIZE,     /* HL bins for the msgs */
            EG_HLOFFSET,          /* Offset of HL Entry */
            FALSE,                /* Allow dup. keys ? */
            CM_HASH_KEYTYPE_U32MOD,  /* HL key type */
            egCb.init.region,     /* Mem region for HL */
            egCb.init.pool);      /* Mem pool for HL */

      if(ret != ROK)
      {

         /*********************************************************
          *Could not initialise hash list, delete the destination *
          *********************************************************/
         EG_DBG_ERR(0, 0, (egp,
                  "\n  TEID Hash List initialisation failed"));
         EG_FREE(dstCb, sizeof(EgDestAddrCb));
#ifdef SS_MT_CB
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
      }

      /**********************************************************
       *Copy destination IP Address, check if multicast address *
       *check interface type.                                   * 
       **********************************************************/
      cmMemset((U8 *)&(dstCb->destIpAddr), 0, sizeof(CmTptAddr));
      EGU_SET_IP(dstCb->destIpAddr, egtUEvnt->u.tnlMgmt.dstIpAddr);
      EGU_CHECK_MULTICAST_IP(dstCb);
      dstCb->intfType = egtUEvnt->u.tnlMgmt.intfType;
      dstCb->maxTPduSize = egtUEvnt->u.tnlMgmt.maxTPduSize;
      dstCb->destReachable = TRUE;
      /*********************************************************
       * Insert the destination into Hash list of Source IP CB * 
       *********************************************************/
      ret = cmHashListInsert(&(ipCb->destAddrCbLst),
            (PTR)dstCb,
            (U8 *)&(dstCb->destIpAddr),
            sizeof(CmTptAddr));

      if(ret !=ROK)
      {
         EG_DBG_ERR(0, 0, (egp,
                  "\n  Hash List insertion of destination \
                  CB into Source IP failed"));
         EGU_DELETE_DEST2(dstCb);
#ifdef SS_MT_CB
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
      }
      /**********************************************
       *Start the echo timer if not already started *
       **********************************************/
      if(!egCb.egUCb.echoTmrStarted && egCb.genCfg.echoTmr.enb)
      {
         ret = egSchedTmr(NULLP, EG_TMR_ECHO_GLOBAL, TMR_START,
               egCb.genCfg.echoTmr.val);
         if (ret != ROK)
         {
            egCb.egUCb.echoTmrStarted = FALSE;
            EG_DBG_ERR(0, 0, (egp,"\n  Starting of Echo Timer Failed"));
         }
    else
    {
       egCb.egUCb.echoTmrStarted = TRUE;
    }
      }
   }

   /*******************************
    * Allocate TEID Control Block * 
    *******************************/
   EG_ALLOC(&teidCb, sizeof(EgTeIdCb));

   if(teidCb == NULLP)
   {
      if(dstCb->teIdLst.nmbEnt == 0)
      {
         /*******************************************************************
          * No TEID for this destination, so the destination can be deleted * 
          *******************************************************************/
         EGU_DELETE_DEST(ipCb, dstCb);
      }
#ifdef SS_MT_CB
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_GTPU_CAUSE_NO_RES_AVAIL);
   }

   /*******************************
    *Copy TEID info into teid CB  * 
    *******************************/
   teidCb->teId = egtUEvnt->u.tnlMgmt.teId;
   /***************************************
    *Enable re-ordering only if UMTS node *
    ***************************************/
   if(egCb.genCfg.nodeArch == LEG_NODE_UMTS)
   {
      teidCb->reOrdering = egtUEvnt->u.tnlMgmt.reOrderPres;
   }
   else
   {
      teidCb->reOrdering =FALSE;
   }
   teidCb->reordAlgo = egtUEvnt->u.tnlMgmt.reordAlgo;
#ifdef EGTP_U_MULTI_THREADED
   teidCb->egThreadCb = NULLP;
#endif

   /*********************************************************
    * Insert the TEID into Hash list of Destination IP CB * 
    *********************************************************/
   ret = cmHashListInsert(&(dstCb->teIdLst),
         (PTR)teidCb,
         (U8 *)&(teidCb->teId),
         sizeof(U32));

   if(ret !=ROK)
   {
      if(dstCb->teIdLst.nmbEnt == 0)
      {
         /*******************************************************************
          * No TEID for this destination, so the destination can be deleted * 
          *******************************************************************/
         EGU_DELETE_DEST(ipCb, dstCb);
      }
      EG_FREE(teidCb, sizeof(EgTeIdCb));

#ifdef SS_MT_CB
      EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
      EG_RETVALUE(EGT_EGTPU_CAUSE_PROC_FAIL);
   }
   /*******************************
    * Assign a Thread to this TEID *
    * if reOrdering is requested   *
    *******************************/
   if(teidCb->reOrdering == TRUE)
   {
      /*************************
       * if not MULTI THREADED *
       ************************/
      egCmInitTimer(&teidCb->reOrderTimer);
#ifdef EGTP_U_MULTI_THREADED 
      egCmInitTimer(&teidCb->tnlDelTimer);
      teidCb->inMsgCnt = 0;
      teidCb->outMsgCnt = 0;
#endif /* EGTP_U_MULTI_THREADED */

      teidCb->seqInfo = (SeqInfo *)egAlloc(sizeof(SeqInfo));
      if(teidCb->seqInfo == NULLP)
      {
         EG_DBG_ERR (0, 0, (egp,"\n Cannot allocate memory for reordering bufer\n"));
         cmHashListDelete(&(dstCb->teIdLst), (PTR)teidCb);
         EG_FREE(teidCb, sizeof(EgTeIdCb));


         if(dstCb->teIdLst.nmbEnt == 0)
         {
            /*******************************************************************
             * No TEID for this destination, so the destination can be deleted * 
             *******************************************************************/
            EGU_DELETE_DEST(ipCb, dstCb);
         }
#ifdef SS_MT_CB
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(RFAILED);
      }

      teidCb->seqInfo->expSeqNmb = egtUEvnt->u.tnlMgmt.expSeqNmb;
      teidCb->seqInfo->pduArray = (EgtUEvnt **)egAlloc((sizeof(EgtUEvnt *))* (egCb.genCfg.pduWindSize));
      if(teidCb->seqInfo->pduArray == NULLP)
      {
         EG_DBG_ERR (0, 0, (egp,"\n Cannot allocate memory for reordering bufer\n"));
         cmHashListDelete(&(dstCb->teIdLst), (PTR)teidCb);
         EG_FREE(teidCb->seqInfo, sizeof(SeqInfo));
         EG_FREE(teidCb, sizeof(EgTeIdCb));


         if(dstCb->teIdLst.nmbEnt == 0)
         {
            /*******************************************************************
             * No TEID for this destination, so the destination can be deleted * 
             *******************************************************************/
            EGU_DELETE_DEST(ipCb, dstCb);
         }

#ifdef SS_MT_CB
         EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
         EG_RETVALUE(RFAILED);
      }
#ifdef EGTP_U_MULTI_THREADED
      teidCb->egThreadCb = NULLP;
      /* Thread assignment is differed until receive a packet for it
       * egAssignThrd(teidCb);
       */
#endif
   }

   /********************************************************************
   * eg001.201 Incrementing respective Tunnel stats on tunnel addition *
   *            & updating tunnel type for new tunnel added            *
   ********************************************************************/
#ifdef EGTP_U_REL_9
   if(egtUEvnt->u.tnlMgmt.tunnelType == EG_NORMAL_DATA_TUNN)
   {
      dstCb->normaldataTunnCnt++;
      teidCb->tunnelType = EG_NORMAL_DATA_TUNN;
      egCb.genSts.numNrmlDataTunn++;
   }
   else if(egtUEvnt->u.tnlMgmt.tunnelType == EG_FWD_DATA_TUNN)
   {
      dstCb->forwardingTunnCnt++;
      teidCb->tunnelType = EG_FWD_DATA_TUNN;
      egCb.genSts.numFwdDataTunn++;
   }
#endif
   egCb.genSts.numActvTnls++;
#ifdef SS_MT_CB
   EG_RDWR_UNLOCK(&egCb.egUCb.threadLock);
#endif /* SS_MT_CB */
   EG_RETVALUE(ROK);

}

/*************************************************************************
 *      Fun:   egTunProcEguTnlMgmtReq
 *
 *      Desc:  This function processes the Local Tunnel Management 
 *             Request from the UI.
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTunProcEguTnlMgmtReq
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgUSapCb *usap            /*Upper Sap CB*/
)
#else
PUBLIC S16 egTunProcEguTnlMgmtReq (egtUEvnt, usap)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgUSapCb *usap;           /*Upper Sap CB*/
#endif
{
   EgIpCb *ipCb;
   CmTptAddr keyAddr;
   EgUsta   sta;
   S16      ret = ROK;

   EG_TRC2(egTunProcEguTnlMgmtReq);

   ipCb=NULLP;

   /******************************************
    * Initialise the return status structure * 
    ******************************************/
   egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_OK;
   egtUEvnt->u.tnlMgmt.cfmStatus.reason = LCM_REASON_NOT_APPL;


   /***************************************************************
    * Copy the source IP Address into local variable and override
    * the port. *
    ***************************************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.tnlMgmt.srcAddr);

   /************************************
    * Find the source IP control block * 
    ************************************/
   cmHashListFind(&(usap->ipCbHashList),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&ipCb );

   if(ipCb == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp, "\n Source IP not configured"));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      sta.dgn.dgnVal[0].t.sapId = usap->cfg.sapId;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(egtUEvnt->u.tnlMgmt.srcAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

      /**************************************
       * Send Negative confirmation to user *
       **************************************/
      egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
      egtUEvnt->u.tnlMgmt.cfmStatus.reason = EGT_GTPU_CAUSE_INV_SRC_IP;
      EgUiEgtEguLclTnlMgmtCfm(&(usap->pst), usap->suId, egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

#ifdef EGTP_U_MULTI_THREADED
   if(ipCb->pendOp == TRUE)
   {
      EG_DBG_ERR (0, 0, (egp, "\n Ip Control Operation is pending"));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      sta.dgn.dgnVal[0].t.sapId = usap->cfg.sapId;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(egtUEvnt->u.tnlMgmt.srcAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, EGT_EGTPU_CAUSE_PENDING_CNTRL_OP, &sta.dgn);

      /**************************************
       * Send Negative confirmation to user *
       **************************************/
      egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
      egtUEvnt->u.tnlMgmt.cfmStatus.reason = EGT_EGTPU_CAUSE_PENDING_CNTRL_OP;
      EgUiEgtEguLclTnlMgmtCfm(&(usap->pst), usap->suId, egtUEvnt);
      EG_RETVALUE(RFAILED);
   }
#endif /* EGTP_U_MULTI_THREADED */

   switch (egtUEvnt->u.tnlMgmt.action)
   {
      case EGT_TMGMT_REQTYPE_ADD:
         /***************
          * Add Teid    * 
          ***************/
         ret = egUTunnTeidAdd(egtUEvnt, ipCb);
         break;
      case EGT_TMGMT_REQTYPE_DEL:
         /******************
          * Delete Teid    * 
          ******************/
         ret = egUTunnTeidDel(egtUEvnt, ipCb, usap);
         break;
      case EGT_TMGMT_REQTYPE_MOD:
         /******************
          * Modify Teid    * 
          ******************/
         ret = egUTunnTeidMod(egtUEvnt, ipCb);
         break;
      case EGT_TMGMT_REQTYPE_REORD_ENA:
         /*****************************************
          * Enable Re-ordering, only for UMTS node* 
          *****************************************/
         if(egCb.genCfg.nodeArch == LEG_NODE_UMTS)
         {
            ret = egUTunnTeidReordEna(egtUEvnt, ipCb);
         }
         break;
      case EGT_TMGMT_REQTYPE_REORD_DIS:
         /*************************************************
          * Disable Re-ordering, valid only for UMTS node * 
          *************************************************/
         if(egCb.genCfg.nodeArch == LEG_NODE_UMTS)
         {
            ret = egUTunnTeidReordDis(egtUEvnt, ipCb, usap);
         }
         break;
      default:
         egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
         egtUEvnt->u.tnlMgmt.cfmStatus.reason = EGT_GTPU_CAUSE_INV_ACTION;
   }
   /*******************************
    *Send the status to the user  * 
    *******************************/
   /**************************************************************
    * For action re-order disable and delete with return type    *
    * ret = EGT_GTPU_CAUSE_RETURN_OK, confirmation to the user   *
    * needs to be sent as ROK from here.                         *
    **************************************************************/
   if((egtUEvnt->u.tnlMgmt.action == EGT_TMGMT_REQTYPE_REORD_DIS) ||
         (egtUEvnt->u.tnlMgmt.action == EGT_TMGMT_REQTYPE_DEL))
   {
      if(ret == EGT_EGTPU_CAUSE_RETURN_OK)
      {
         EgUiEgtEguLclTnlMgmtCfm(&(usap->pst), usap->suId, egtUEvnt);
         EG_RETVALUE(ROK);
      }
   }
#ifdef EGTP_U_MULTI_THREADED
   /*************************************************************************
    * If multithreaded, then confirmation needs to be sent from here        *
    * in case of failure for all actions and in case of success for         *
    * actions other than delete and re-order disable as confirmation for    *
    * these actions  will be sent after getting reponse from worker threads *
    *************************************************************************/
   if(ret != ROK)
   {
      egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
      egtUEvnt->u.tnlMgmt.cfmStatus.reason = ret;
      EgUiEgtEguLclTnlMgmtCfm(&(usap->pst), usap->suId, egtUEvnt);
      EG_RETVALUE(ROK);
   }
   else if(!((egtUEvnt->u.tnlMgmt.action == EGT_TMGMT_REQTYPE_DEL)
         ||(egtUEvnt->u.tnlMgmt.action == EGT_TMGMT_REQTYPE_REORD_DIS)))
   {
      EgUiEgtEguLclTnlMgmtCfm(&(usap->pst), usap->suId, egtUEvnt);
      EG_RETVALUE(ROK);
   }
   /***********************************************
    *If come here, need to free the user event as *
    *msg posted to thread.                        *
    ***********************************************/
   EG_FREE(egtUEvnt, sizeof(EgtUEvnt));
#else
   if(ret != ROK)
   {
      egtUEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
      egtUEvnt->u.tnlMgmt.cfmStatus.reason = ret;
   }
   EgUiEgtEguLclTnlMgmtCfm(&(usap->pst), usap->suId, egtUEvnt);
#endif
   EG_RETVALUE(ROK);
}

/*************************************************************************
 *      Fun:   egTunProcEguStaReq
 *
 *      Desc:  This function processes the Status Request from UI. 
 *             It returns the status of the requested destinaton or port.
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTunProcEguStaReq
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgUSapCb *usap            /*Upper Sap CB*/
)
#else
PUBLIC S16 egTunProcEguStaReq (egtUEvnt, usap)
EgtUEvnt *egtUEvnt;       /* service provider id */
EgUSapCb *usap;           /*Upper Sap CB*/
#endif
{

   EgIpCb *ipCb;
   EgDestAddrCb *dstCb;
   CmTptAddr keyAddr;
   CmTptAddr dstkeyAddr;
   EgTptSrvCb     *srvCb;
   Bool portInfo = FALSE;
   U16 portNum = 0;

   EG_TRC2(egTunProcEguStaReq);

   ipCb = NULLP;
   srvCb = NULLP;
   dstCb=NULLP;
   /******************************************
    * Initialise the return status structure * 
    ******************************************/
   egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_OK;
   egtUEvnt->u.status.cfmStatus.reason = LCM_REASON_NOT_APPL;

   /***************************************************************************
    * Copy the source address into a local variable. If the port is not 0 then*
    * we need to set it to 0 to be able to retrieve the source IP CB. Also if *
    * port is not 0 then port info is needed so set the flag portInfo to TRUE * 
    ***************************************************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.status.srcAddr);

   if(egtUEvnt->u.status.srcAddr.type == CM_INET_IPV4ADDR_TYPE)
   {
      if (egtUEvnt->u.status.srcAddr.u.ipv4TptAddr.port != 0)
      {
         portInfo = TRUE;
         portNum = egtUEvnt->u.status.srcAddr.u.ipv4TptAddr.port;
      }
   }
#ifdef IPV6_SUPPORTED 
   else if(egtUEvnt->u.status.srcAddr.type == CM_INET_IPV6ADDR_TYPE)
   {
      if (egtUEvnt->u.status.srcAddr.u.ipv6TptAddr.port != 0)
      {
         portInfo = TRUE;
         portNum = egtUEvnt->u.status.srcAddr.u.ipv6TptAddr.port;
      }
   }
#endif


   /************************************
    * Find the source IP control block * 
    ************************************/

   cmHashListFind(&(usap->ipCbHashList),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&ipCb );

   if(ipCb == NULLP)
   {
      egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_NOK;
      egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_CAUSE_INV_SRC_IP;
      EgUiEgtEguStaCfm(&(usap->pst), usap->suId, egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

   if(portInfo == TRUE)
   {
      /**************************************************************
       * To find the status of the port retrieve the server CB from *
       * list in IP CB. * 
       *****************************************/

      cmHashListFind(&(ipCb->tptServCbLst),
            (U8 *) &(portNum),
            sizeof(U16), 0, (PTR *)&(srvCb));

      if(srvCb == NULLP)
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_NOK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_CAUSE_INV_PORT;
         EgUiEgtEguStaCfm(&(usap->pst), usap->suId, egtUEvnt);
         EG_RETVALUE(RFAILED);
      }
      /************************************************
       * Check the status of the port and send to user* 
       ************************************************/
      if(srvCb->portStatus)
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_OK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_PORT_ACTIVE;
      }
      else
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_OK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_PORT_INACTIVE;
      }
      EgUiEgtEguStaCfm(&(usap->pst), usap->suId, egtUEvnt);
   }
   else
   {
      /******************************************************
       * Port info is not reqd, retrieve the destination CB * 
       ******************************************************/
      cmMemset((U8 *)&dstkeyAddr, 0 ,sizeof(CmTptAddr));
      EGU_SET_IP(dstkeyAddr, egtUEvnt->u.status.dstAddr);

      cmHashListFind(&(ipCb->destAddrCbLst),
            (U8 *) &(dstkeyAddr),
            sizeof(CmTptAddr), 0, (PTR *)&dstCb);

      if(dstCb == NULLP)
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_NOK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_CAUSE_INV_DST_IP;
         EgUiEgtEguStaCfm(&(usap->pst), usap->suId, egtUEvnt);
         EG_RETVALUE(RFAILED);
      }

      if(dstCb->multicastAddr == TRUE)
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_NOK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_CAUSE_DST_MULT_IP;
         EgUiEgtEguStaCfm(&(usap->pst), usap->suId, egtUEvnt);
         EG_RETVALUE(RFAILED);
      }
      /********************************************************
       * Check if destination is reachable and send the status* 
       ********************************************************/
      if(dstCb->destReachable)
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_OK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_DEST_REACHABLE;
      }
      else
      {
         egtUEvnt->u.status.cfmStatus.status = LCM_PRIM_OK;
         egtUEvnt->u.status.cfmStatus.reason = EGT_GTPU_DEST_UNREACHABLE;
      }
      EgUiEgtEguStaCfm(&(usap->pst), usap->suId, egtUEvnt);
   }

   EG_RETVALUE(ROK);

}


/*************************************************************************
 *      Fun:   egUFreeUiEvnt
 *
 *      Desc:  This function de-allocates the UI event. 
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egUFreeUiEvnt
(
EgtUEvnt *egtUEvnt       /* Event from UI */
)
#else
PUBLIC S16 egUFreeUiEvnt (egtUEvnt)
EgtUEvnt *egtUEvnt;       /* Event from UI*/
#endif
{

   EG_TRC2(egUFreeUiEvnt);

   EgUUtilDeAllocGmMsg(&(egtUEvnt->u.egMsg));

   /**********************
    *Free the User Event *
    **********************/
   EG_FREE(egtUEvnt,sizeof(EgtUEvnt));
   EG_RETVALUE(ROK);
}

/*************************************************************************
 *      Fun:   egUTunHdlMsgTypeEchoReq
 *
 *      Desc: This function is called when application sends an Echo 
 *            Request.
 *            
 *
 *      Ret: void 
 *          
 *
 *      Note: none
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUTunHdlMsgTypeEchoReq
(
EgUSapCb *usap,           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb             /*IP CB*/
)
#else
PRIVATE S16 egUTunHdlMsgTypeEchoReq(usap, egtUEvnt, ipCb)
EgUSapCb *usap;           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt;       /* Event from UI*/
EgIpCb *ipCb;             /*IP CB*/
#endif
{
   CmTptAddr keyAddr;
   EgDestAddrCb *dstCb;
   EgtUEvnt *egtErrEvnt;
   EgtUEvnt *egtUStaEvnt;

   EG_TRC2(egUTunHdlMsgTypeEchoReq);

   dstCb = NULLP;

   /***************************************************************************
    * copy the source address into a local variable. if the port is not 0 then*
    * we need to set it to 0 to be able to retrieve the destination ip cb. 
    ***************************************************************************/
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.egMsg->remAddr);

   /*******************************
    * retrieve the destination cb * 
    *******************************/
   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8*) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if(dstCb == NULLP)
   {
      /****************************************************
       *generate error indication to user* 
       ****************************************************/
      EG_ALLOC(&egtErrEvnt, sizeof(EgtUEvnt));
      if (egtErrEvnt == NULLP)
      {
         EG_DBG_ERR(0, 0,
               (egp,"failed to allocate the memory for egtevnt"));
         EG_RETVALUE(RFAILED);
      }
      /*********************************
       *fill the error event structure *
       *********************************/
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_INV_DST_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

   if(dstCb->multicastAddr == TRUE)
   {
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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_DST_MULT_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }
   /*******************************************************
    *allocate memory for the unsolicited status indication* 
    *******************************************************/
   EG_ALLOC(&egtUStaEvnt, sizeof(EgtUEvnt));
   if (egtUStaEvnt == NULLP)
   {
      EG_DBG_ERR(0, 0,
            (egp,"Failed to Allocate the memory for egtEvnt"));
      EG_RETVALUE(RFAILED);
   }
   egCpyCmTptAddr(&(egtUStaEvnt->u.uStaInd.dstAddr),
         &(egtUEvnt->u.egMsg->remAddr));
   egCpyCmTptAddr(&(egtUStaEvnt->u.uStaInd.srcAddr),
         &(egtUEvnt->u.egMsg->srcAddr));

   if(dstCb->destReachable)
   {
      egtUStaEvnt->u.uStaInd.cfmStatus.status = LCM_PRIM_OK;
      egtUStaEvnt->u.uStaInd.cfmStatus.reason = EGT_GTPU_DEST_REACHABLE;
   }
   else
   {
      egtUStaEvnt->u.uStaInd.cfmStatus.status = LCM_PRIM_OK;
      egtUStaEvnt->u.uStaInd.cfmStatus.reason = EGT_GTPU_DEST_UNREACHABLE;
   }
   EgUiEgtEguUStaInd (&(usap->pst), usap->suId,
         egtUStaEvnt);
   egUFreeUiEvnt(egtUEvnt);
   EG_RETVALUE(ROK);
}



/*************************************************************************
 *      Fun:   egUTunHdlMsgTypeErrInd
 *
 *      Desc: This function is called when the application sends Error
 *            Indication message. 
 *           
 *      Ret: ROK    : If successfull.
 *           RFAILED: If not successfull.
 *
 *      Note: none
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUTunHdlMsgTypeErrInd
(
EgUSapCb *usap,           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb,             /*IP CB*/
U8 *intfType              /*Interface Type to be retrived*/
)
#else
PRIVATE S16 egUTunHdlMsgTypeErrInd (usap, egtUEvnt, ipCb, intfType)
EgUSapCb *usap;           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt;       /* Event from UI*/
EgIpCb *ipCb;             /*IP CB*/
U8 *intfType;             /*Interface Type to be retrived*/
#endif
{
   CmTptAddr keyAddr;
   EgDestAddrCb *dstCb;
   EgtUEvnt *egtErrEvnt;

   EG_TRC2(egUTunHdlMsgTypeErrInd);

   dstCb = NULLP;

   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.egMsg->remAddr);

   /*******************************
    * Retrieve the destination CB * 
    *******************************/
   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if(dstCb == NULLP)
   {
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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_INV_DST_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

   /***********************************************************
    *Error Indication should not be generated for Multicast IP* 
    ***********************************************************/
   if(dstCb->multicastAddr == TRUE)
   {
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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_DST_MULT_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

   /****************************************************************
    *Can proceed for encoding so fill interface type and return ROK*
    ****************************************************************/
   *intfType = dstCb->intfType;
   EG_RETVALUE(ROK);


}

/*************************************************************************
 *      Fun:   egUTunHdlMsgTypeEndMark
 *
 *      Desc: This function is called when application sends End Marker. 
 *            
 *
 *      Ret: ROK    : If need to continue processing.
 *           RFAILED: If need not process further.
 *
 *      Note: none
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUTunHdlMsgTypeEndMark
(
EgUSapCb *usap,           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb,             /*IP CB*/
U8 *intfType              /*Interface Type to be retrived*/
)
#else
PRIVATE S16 egUTunHdlMsgTypeEndMark (usap, egtUEvnt, ipCb, intfType)
EgUSapCb *usap;           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt;       /* Event from UI*/
EgIpCb *ipCb;             /*IP CB*/
U8 *intfType;             /*Interface Type to be retrived*/
#endif
{
   CmTptAddr keyAddr;
   EgDestAddrCb *dstCb;
   EgtUEvnt *egtErrEvnt;

   EG_TRC2(egUTunHdlMsgTypeEndMark);

   dstCb = NULLP;
   cmMemset((U8 *)&keyAddr, 0 ,sizeof(CmTptAddr));
   EGU_SET_IP(keyAddr, egtUEvnt->u.egMsg->remAddr);

   /******************************************************
    * retrieve the destination CB * 
    ******************************************************/
   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if(dstCb == NULLP)
   {
      /***********************************
       *generate Error Indication to User* 
       ***********************************/
      EG_ALLOC(&egtErrEvnt, sizeof(EgtUEvnt));
      if (egtErrEvnt == NULLP)
      {
         EG_DBG_ERR(0, 0,
               (egp,"Failed to Allocate the memory for egtEvnt"));
         EG_RETVALUE(RFAILED);
      }
      /****************************
       *Generate Error Indication *
       ****************************/
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_INV_DST_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }
   /**************************
    *Fill the interface type *
    **************************/
   *intfType = dstCb->intfType;
   EG_RETVALUE(ROK);
}

/*************************************************************************
 *      Fun:   egUTunHdlMsgType
 *
 *      Desc: This function checks the message Type. If Echo Request from 
 *            application then generate status Indication.If Echo Response 
 *            from application, generate Error Indication. If Error Indication  
 *            from application and IP is multicast address then should not
 *            process it further.
 *            
 *
 *      Ret: ROK    : If need to continue processing.
 *           RFAILED: If need not process further.
 *
 *      Note: none
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PRIVATE S16 egUTunHdlMsgType
(
EgUSapCb *usap,           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgIpCb *ipCb,             /*IP CB*/
U8 *intfType,              /*Interface Type to be retrived*/
U16 *maxTPduSize         /*Max TPDU size*/
)
#else
PRIVATE S16 egUTunHdlMsgType (usap, egtUEvnt, ipCb, intfType, maxTPduSize)
EgUSapCb *usap;           /*Upper Sap CB*/
EgtUEvnt *egtUEvnt;       /* Event from UI*/
EgIpCb *ipCb;             /*IP CB*/
U8 *intfType;             /*Interface Type to be retrived*/
U16 *maxTPduSize;         /*Max TPDU size*/
#endif
{
   EgDestAddrCb *dstCb;
   EgtUEvnt *egtErrEvnt;
   /* TO avoid Memset to zero, it is decalred as PRIVATE */
   PRIVATE CmTptAddr keyAddr;

   EG_TRC2(egUTunHdlMsgType);

   dstCb = NULLP;

   switch(egtUEvnt->u.egMsg->msgHdr.msgType)
   {
      case EGT_GTPU_MSG_GPDU:
         break;

      case EGT_GTPU_MSG_ECHO_REQ:
         {
            /**************************************************************
             * application echo reqest is not processed. instead a status
             * indication regarding the destination is returned to user.  
             **************************************************************/
            egUTunHdlMsgTypeEchoReq(usap, egtUEvnt, ipCb);
            EG_RETVALUE(RFAILED);

            break;
         }
      case EGT_GTPU_MSG_ECHO_RSP:
         {
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
            EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_MSG_NOT_ALLWD);
            egUFreeUiEvnt(egtUEvnt);
            EG_RETVALUE(RFAILED);
            break;
         } /*EGT_GTPU_MSG_ECHO_RSP*/

      case EGT_GTPU_MSG_ERROR_IND:
         {

            EG_RETVALUE(egUTunHdlMsgTypeErrInd(usap, egtUEvnt, ipCb, intfType));
            break;
         }/*EGT_GTPU_MSG_ERROR_IND*/
      case EGT_GTPU_MSG_END_MARKER:
         {
            /***********************************************
             *For End Marker, check the destination IP     *
             *configured and retrieve the  interface type. *
             ***********************************************/
            EG_RETVALUE(egUTunHdlMsgTypeEndMark(usap, egtUEvnt, ipCb, intfType));
            break;
         }
      default:
         {
         }
   }  /*switch*/

   /***********************************************************************
    *For G-PDU, check the destination IP configured and retrieve          *
    *the  interface type                                                  *
    ***********************************************************************/
   EGU_SET_IP(keyAddr, egtUEvnt->u.egMsg->remAddr);

   /******************************************************
    * Port info is not reqd, retrieve the destination CB * 
    ******************************************************/
   cmHashListFind(&(ipCb->destAddrCbLst),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&dstCb);

   if(dstCb == NULLP)
   {
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
      /****************************
       *Generate Error Indication *
       ****************************/
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_INV_DST_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

   *maxTPduSize = dstCb->maxTPduSize;

   EG_RETVALUE(ROK);
}




/*************************************************************************
 *      Fun:   egTunProcEguDatReq
 *
 *      Desc:  This function processes the Local Tunnel Management 
 *             Request from the UI.
 *
 *      Ret: 
 *
 *      Note:
 *
 *      File:   eg_tunn.c
***********************************************************************/
#ifdef ANSI
PUBLIC S16 egTunProcEguDatReq
(
EgtUEvnt *egtUEvnt,       /* Event from UI */
EgUSapCb *usap,            /*Upper Sap CB*/
Pst      *pst
)
#else
PUBLIC S16 egTunProcEguDatReq (egtUEvnt, usap, pst)
EgtUEvnt *egtUEvnt;       /* Event from UI*/
EgUSapCb *usap;           /*Upper Sap CB*/
Pst      *pst;
#endif
{
   EgIpCb *ipCb;
   EgTptSrvCb     *srvCb;
   EgtUEvnt  *egtErrEvnt;
   EgUsta sta;
   S16      ret = ROK;
   U16      portNum = 0;
   U8       intfType = 0;
   U16      maxTPduSize=0;         /*Max TPDU size*/
   /* TO avoid Memset to zero, it is decalred as PRIVATE */
   PRIVATE CmTptAddr keyAddr;

   EgUEvnt evntMsg;

   EG_TRC2(egTunProcEguDatReq);

   ipCb = NULLP;
   srvCb = NULLP;
   egtErrEvnt = NULLP;
   /**********************************************************
    ********** Initialisze the Event Stucture                *
    **********************************************************/
   cmMemset( (U8* )&evntMsg, 0, sizeof(EgUEvnt));

   /***************************************************************************
    * Copy the source port into a local variable. If the port is not 0 then*
    * we need to set it to 0 to be able to retrieve the source IP CB.         *
    ***************************************************************************/
   EGU_SET_IP(keyAddr, egtUEvnt->u.egMsg->srcAddr);

   EGU_GET_PORT(egtUEvnt->u.egMsg->srcAddr, portNum);


   /************************************
    * Find the source IP control block * 
    ************************************/

   cmHashListFind(&(usap->ipCbHashList),
         (U8 *) &(keyAddr),
         sizeof(CmTptAddr), 0, (PTR *)&ipCb );

   if(ipCb == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp, "\n Source IP not configured"));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      sta.dgn.dgnVal[0].t.sapId = usap->cfg.sapId;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(egtUEvnt->u.egMsg->srcAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LCM_CAUSE_INV_SPID, &sta.dgn);

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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_INV_SRC_IP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

#ifdef EGTP_U_MULTI_THREADED
#ifndef SS_MT_CB
   if(ipCb->pendOp == TRUE)
   {
      EG_DBG_ERR (0, 0, (egp, "\n egTunProcEguDatReq():Ip Control Operation is pending"));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      sta.dgn.dgnVal[0].t.sapId = usap->cfg.sapId;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(egtUEvnt->u.egMsg->srcAddr));
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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_PENDING_CNTRL_OP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }
#endif /* SS_MT_CB */
#endif /* EGTP_U_MULTI_THREADED */

   /**************************
    * Retrieve the server CB *
    **************************/

   cmHashListFind(&(ipCb->tptServCbLst),
         (U8 *) &(portNum),
         sizeof(U16), 0, (PTR *)&(srvCb));

   if(srvCb == NULLP)
   {
      EG_DBG_ERR (0, 0, (egp, "\n Server not configured"));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(egtUEvnt->u.egMsg->srcAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
           LCM_CAUSE_INV_SPID, &sta.dgn);

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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_INV_SRC_PORT);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }

#ifdef EGTP_U_MULTI_THREADED
#ifndef SS_MT_CB
   if(srvCb->pendOp == TRUE)
   {
      EG_DBG_ERR (0, 0, (egp, "\n egTunProcEguDatReq():Server Control Operation is pending"));
      /***********************************
       * Send Alarm to the layer manager *
       ***********************************/
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

      sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
      sta.dgn.dgnVal[0].t.sapId = usap->cfg.sapId;
      egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(egtUEvnt->u.egMsg->srcAddr));
      egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT,
            EGT_EGTPU_CAUSE_PENDING_CNTRL_OP, &sta.dgn);

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
      EGU_GEN_ERR_IND(egtErrEvnt, egtUEvnt, usap, EGT_ERR_PENDING_CNTRL_OP);
      egUFreeUiEvnt(egtUEvnt);
      EG_RETVALUE(RFAILED);
   }
#endif /* SS_MT_CB */
#endif /* EGTP_U_MULTI_THREADED */

   /**********************************************************************
    * If User has sent Echo Request, Echo Response message or Error      *
    * Indication for a multicast IP, we need to handle it here instead   *
    * of proceeding.                                                     * 
    **********************************************************************/
   if ((ret = egUTunHdlMsgType(usap, egtUEvnt, ipCb, &intfType, &maxTPduSize)) != ROK)
   {
      EG_RETVALUE(RFAILED);
   }

   egUpdateTxStatistics((EgtMsgType)egtUEvnt->u.egMsg->msgHdr.msgType, &(usap->sts));
   /*********************************
    * Override the destination port *
    *********************************/
   EGU_SET_PORT(egtUEvnt->u.egMsg->remAddr, EGTPU_PORT);


   /*****************************************************
    * Fill the event structure with                     *
    *  a) Received Message                              *
    *  b) Tpt Server on which Message is  to be sent    *
    *  c) Destination Address for the message           *
    *****************************************************/
   evntMsg.egMsg = egtUEvnt->u.egMsg;
   evntMsg.srvCb = srvCb;
   egCpyCmTptAddr(&(evntMsg.remAddr), &(egtUEvnt->u.egMsg->remAddr));

   egCpyCmTptAddr(&(evntMsg.localAddr), &(egtUEvnt->u.egMsg->srcAddr));


   /**********************************************************************
    * Fill the default region & pool Id . This will be overwritten 
    * with the region and pool of the repective thread for multithreading*
    **********************************************************************/
   evntMsg.region = srvCb->tSapCb->pst.region;
   evntMsg.pool = srvCb->tSapCb->pst.pool;

   /**********************************************************************
    * Fill the max T-PDU size and interface type as reqd by EDM 
    **********************************************************************/
   evntMsg.maxTPduSize  = maxTPduSize;
   evntMsg.intfType     = intfType;

   /************************************************************
    * Fill the event type to inform CPM that it is an UI event *  
    ************************************************************/

   evntMsg.eventType = EGU_EVT_EGT;

   /*******************
    * Free User Event * 
    *******************/
   EG_FREE(egtUEvnt, sizeof(EgtUEvnt));

   /********************
    * Update Statistics * 
    ********************/
   ipCb->ipStats.totalMsgsTx++;
   srvCb->stats.totalMsgsTx++;

   /*******************************
    *Call the statistics function * 
    *******************************/
   pst->srcInst = pst->dstInst;
   egUCpmHdlEvt(&evntMsg, pst);

   EG_RETVALUE(ROK);
}

#ifdef EGTP_U_MULTI_THREADED

/***********************************************************************************
 *
 *       Fun:   egUTunnDeleteTunnel
 *
 *       Desc:  
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  eg_tunn.c
 *
 ***********************************************************************************/

#ifdef ANSI
PUBLIC S16 egUTunnDeleteTunnel
(
EgUEvnt    *eguEvnt
)
#else
PUBLIC S16 egUTunnDeleteTunnel(eguEvnt)
EgUEvnt    *eguEvnt;
#endif
{
   S16            retValue = 0;
   EgDestAddrCb *destAddrCb = NULLP;
   EgtUEvnt *egtCfmEvnt;
   EgUsta sta;

   EG_TRC2(egUTunnDeleteTunnel);

   if(++(eguEvnt->teIdCb->nmbCfmFrmWrkThr) == eguEvnt->teIdCb->nmbReqToWrkThr) 
   {
      if(eguEvnt->teIdCb->notCmpltOp == FALSE)
      {
    destAddrCb = eguEvnt->destAddrCb;
    EG_ALLOC(&egtCfmEvnt, sizeof(EgtUEvnt));
    if (egtCfmEvnt == NULLP)
    {
       EG_DBG_ERR(0, 0,
        (egp,"Failed to Allocate the memory for egtEvnt"));
       EGU_FREEEVNT(eguEvnt);
       EG_RETVALUE(RFAILED);
    }
    egtCfmEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_OK;
    egtCfmEvnt->u.tnlMgmt.cfmStatus.reason = LCM_REASON_NOT_APPL;
    egtCfmEvnt->u.tnlMgmt.teId = eguEvnt->teIdCb->teId;
    egtCfmEvnt->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_DEL;
    egCpyCmTptAddr(&(egtCfmEvnt->u.tnlMgmt.srcAddr), &(eguEvnt->localAddr));
    egCpyCmTptAddr(&(egtCfmEvnt->u.tnlMgmt.dstIpAddr), &(eguEvnt->remAddr));

    cmHashListDelete(&destAddrCb->teIdLst, (PTR)eguEvnt->teIdCb);

    eguEvnt->teIdCb->egThreadCb = NULLP;
    if( eguEvnt->teIdCb->reOrdering)
    {
       egFree((Data*)eguEvnt->teIdCb->seqInfo->pduArray,(sizeof(EgtUEvnt *) * egCb.genCfg.pduWindSize));
       egFree((Data*)eguEvnt->teIdCb->seqInfo,(sizeof(SeqInfo)));
    }
    EG_FREE(eguEvnt->teIdCb, sizeof(EgTeIdCb));

    if(destAddrCb->teIdLst.nmbEnt == 0)
    {
       /***********************************
        * Send Alarm to the layer manager *
        ***********************************/
       cmMemset( (U8* )&sta, 0, sizeof(EgUsta));

       sta.dgn.dgnVal[0].type    = LEG_PAR_IP;
       egCpyCmTptAddr(&(sta.dgn.dgnVal[0].t.ipAddr), &(destAddrCb->destIpAddr));
       egSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, LEG_CAUSE_DST_IP_DEL, &sta.dgn);
       /*******************************************************************
        * No TEID for this destination, so the destination can be deleted * 
        *******************************************************************/
       EGU_DELETE_DEST(eguEvnt->ipCb, destAddrCb);
    }   
    egCb.genSts.numActvTnls--;
    EgUiEgtEguLclTnlMgmtCfm(&(eguEvnt->usap->pst), eguEvnt->usap->suId, egtCfmEvnt);
      }
      else
      {
         EG_ALLOC(&egtCfmEvnt, sizeof(EgtUEvnt));
         if (egtCfmEvnt == NULLP)
         {
            EG_DBG_ERR(0, 0,
                  (egp,"Failed to Allocate the memory for egtEvnt"));
            EGU_FREEEVNT(eguEvnt);
            EG_RETVALUE(RFAILED);
         }
         egtCfmEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
         egtCfmEvnt->u.tnlMgmt.cfmStatus.reason = LCM_REASON_NOT_APPL;
         EgUiEgtEguLclTnlMgmtCfm(&(eguEvnt->usap->pst), eguEvnt->usap->suId, egtCfmEvnt);
         eguEvnt->teIdCb->tnlPendOp = FALSE;
         eguEvnt->teIdCb->nmbReqToWrkThr = eguEvnt->teIdCb->nmbCfmFrmWrkThr = 0;
         eguEvnt->teIdCb->notCmpltOp = FALSE;
      }
      EGU_FREEEVNT(eguEvnt);
   }
   EG_RETVALUE(retValue);
}/* egUTunnDeleteTunnel() */

/***********************************************************************************
 *
 *       Fun:   egUTunnReordCfm
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
PUBLIC S16 egUTunnReordCfm
(
EgUEvnt    *eguEvnt
)
#else
PUBLIC S16 egUTunnReordCfm(eguEvnt)
EgUEvnt    *eguEvnt;
#endif
{
   S16            retValue = 0;
   EgtUEvnt *egtCfmEvnt = NULLP;

   EG_TRC2(egUTunnReordCfm);

   if(++(eguEvnt->teIdCb->nmbCfmFrmWrkThr) == eguEvnt->teIdCb->nmbReqToWrkThr)
   {
      if(eguEvnt->teIdCb->notCmpltOp == FALSE)
      {
         EG_ALLOC(&egtCfmEvnt, sizeof(EgtUEvnt));
         if (egtCfmEvnt == NULLP)
         {
            EG_DBG_ERR(0, 0,
                  (egp,"Failed to Allocate the memory for egtEvnt"));
            EGU_FREEEVNT(eguEvnt);
            EG_RETVALUE(RFAILED);
         }

         eguEvnt->teIdCb->reOrdering = FALSE;
         eguEvnt->teIdCb->egThreadCb = NULLP;
         egFree((Data*)eguEvnt->teIdCb->seqInfo->pduArray,(sizeof(EgtUEvnt *) * egCb.genCfg.pduWindSize));
         egFree((Data*)eguEvnt->teIdCb->seqInfo,(sizeof(SeqInfo)));
         egtCfmEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_OK;
         egtCfmEvnt->u.tnlMgmt.cfmStatus.reason = LCM_REASON_NOT_APPL;
         egtCfmEvnt->u.tnlMgmt.teId = eguEvnt->teIdCb->teId;
         egtCfmEvnt->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_REORD_DIS;
         egCpyCmTptAddr(&(egtCfmEvnt->u.tnlMgmt.srcAddr), &(eguEvnt->localAddr));
         egCpyCmTptAddr(&(egtCfmEvnt->u.tnlMgmt.dstIpAddr), &(eguEvnt->remAddr));
         EgUiEgtEguLclTnlMgmtCfm(&(eguEvnt->usap->pst), eguEvnt->usap->suId, egtCfmEvnt);
      }
      else
      {
         EG_ALLOC(&egtCfmEvnt, sizeof(EgtUEvnt));
         if (egtCfmEvnt == NULLP)
         {
            EG_DBG_ERR(0, 0,
                  (egp,"Failed to Allocate the memory for egtEvnt"));
            EGU_FREEEVNT(eguEvnt);
            EG_RETVALUE(RFAILED);
         }
         egtCfmEvnt->u.tnlMgmt.cfmStatus.status = LCM_PRIM_NOK;
         egtCfmEvnt->u.tnlMgmt.cfmStatus.reason = LCM_REASON_NOT_APPL;
         EgUiEgtEguLclTnlMgmtCfm(&(eguEvnt->usap->pst), eguEvnt->usap->suId, egtCfmEvnt);
      }
      eguEvnt->teIdCb->tnlPendOp = FALSE;
      eguEvnt->teIdCb->nmbReqToWrkThr = eguEvnt->teIdCb->nmbCfmFrmWrkThr = 0;
      eguEvnt->teIdCb->notCmpltOp = FALSE;
      EGU_FREEEVNT(eguEvnt);
   }

   EG_RETVALUE(retValue);
}/* egUTunnReordCfm() */
#endif /* EGTP_U_MULTI_THREADED */
#endif /* EGTP_U */

#ifdef __cplusplus
}
#endif

/********************************************************************30**
  
         End of file:     pg_tunn.c@@/main/3 - Fri Feb  4 18:18:17 2011
    
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
/main/1                mm       1. Created for Initial Release 1.1.
/main/2      ---      rsharon  1. Initial for eGTP 1.2 Release
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss                 2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3      eg001.201    asaurabh          1. eg001.201 Updating respective Tunnel stats on tunnel deletion
                                            2. Incrementing respective Tunnel stats on tunnel addition 
                                               & updating tunnel type for new tunnel added
/main/2      eg002.201    psingh            1.updating the statistics prior to indiation(ccpu00115886)
/main/3      eg003.201    psingh            1. Merged code from eg006.102 
                                            2. In EGTP_C_PTH_MGT destCb is initialized to NULLP
                                            3. Updated the proper error code in case of deletion of already
                                               deleted tunnel
/main/3      eg004.201    magnihotri        1. Header files added for eGTP-C PSF
                                            2. Add mapping for a new IncReqCb and update incReqCb on STANDBY
                                            3. Added for eGTP-C PSF Upgrade:Obtain the local teid for an incoming message
                                            4. Delete mapping for IncReqCb and update incReqCb on STANDBY when an incReqCb
                                               is deleted on ACTIVE node on receving the associated signalling response from peer
                                            5. Send RT Update STANDBY for tunnel Cb when tunnel state changes to conncetd on receiving
                                               a CS Response from the peer eGTP-C node
                                            6. Update statistics when a new message is received on an existing binded USAP
                                            7. Send RT Update when tunnel state changes to connected
                                            8. Modified for eGTP-C PSF Upgrade::Function Prototype modified from PRIVATE to PUBLIC
                                            9.  Add mapping for TeidCb when a incoming session request message received 
                                                from peer egGTP node resulting in addition of new tunnel Cb
                                            10. Delete Mapping for incReCb when the assocaited tunnel Cb is deleted
                                            11. Delete Mapping for teid Cb when a tunnel is deleted at ACTIVE node 
                                                and send RTU for tunnel deletion to STANDBY 
/main/3      eg006.201  psingh               1. Send Error to application in case Msg has Seq Num 
                                                assigned for Echo req/rsp 
                                             2. Inilize destReachable value to TRUE during Tunnel 
                                                creation
                                             3. Start Echo timer only after tunnel reaches connected 
                                                state
                                             4. Changes for Creating Dummy tunnel
                                             5. Start Echo timer immediately after Dummy tunnel creation
                                             6. Send Error indication to User when add tunnel is 
                                                attempted for an unreachable destination
                                             7. Raise an alarm when Echo resp is received for 
                                                unreachable destination indicating Path recovery
/main/3      eg007.201  psingh               1. Updated destCbList references to point to TSapCb instead of egCb
                                             2. References to egCb.echoSap replaced with Usap assosciated with Dflt srver
                                             3. Updated statistics before sending SigInd or SigCfm msg
                                             4. Changed Hash bin size for teCCb->incReqCp from 1024 to 32
/main/3      eg009.201  asaurabh             1. Added for S2B
                                             2. Added for S2A
                                             3. Added for S2B and S2A
                                             4. Klockwork fix-UnReachable code
                                             5. Klocwork fix- Do not capture return
                                             6. In case of Dummy tunnel request, the IEs and the incReqCp won't be required
                                             7. Filled in proper response msg type into EgMsg for Dummy tunnel request
                                             8. Statistics is already update for upper sap in egTptProcessMsg 
                                                hence here it is removed.
                                             9. free Memory for invalid Teid. 
                                             10.Fix for TEID zero is valid.
/main/3    eg010.201   asaurabh              1. Fix for handling RAB Release Req and RAB Release Response lpr_fix_ccpu00124570
                                             2. Free buffered piggybacked message while deleting the tunnel context
/main/3    eg012.201   shpandey              1. In egValidateMsgInt:Added exception message types and exception cases
                                             2. MsgType check for path mgmnt,fix for ccpu00124404 
                                             3. Added entry for RAB Release Req and RAB Release Response ,fix for ccpu00124570
                                             4. in function egSaveMsgRemDetailsBug :fix for condition check for inserting
                                                 dupilcate message to hashlist queue
                                             5. Memory leak fix for ccpu00128780 
                                             6. Transaction id updation fix for ccpu00129025
                                             7. In fuction egTptHdlOutPBMsg:Transaction id check removed for piggibaked response message
                                             8. in fuction egTunSend:added Check for piggibacked request message
                                             9. Added message type,fix for ccpu00125389
                                             10.Not required to save remote details for messages 64 to 70 ,fix for ccpu00125389
                                             11.For message 64 to 70 need not to get remote details,fix for ccpu00125389 
                                             12.Removed extra check ROKDUP for return,fix for ccpu00129790
                                             13.Enhanced the piggy backing feature by adding the Timer
					     14.In fuction egTunnFsmConOutMsg:Moved transaction id updation 
					     15.Removed updating the statistics twice,fix for ccpu00124118
					     16.Store the transaction id for command request message,fix for ccpu00130148
                                             17.In egTunReceive:added check for failure
/main/3     eg014.201    rss                  1. Egtpc release 10 upgrade
*********************************************************************91*/
