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

     Name:    Trillium LTE CNE - PGW Controller Module

     Type:    C Include file

     Desc:    This file contains the PDN-GW application source code

     File:    jg.c

     Sid:      av.c@@/main/1 - Tue Aug 17 18:54:31 2010

     Prg:     sv 
*********************************************************************21*/

#include "envopt.h"        /* environment options */

/* header include files (.h) */
#include "envdep.h"        /* environment dependent */

#ifndef AV_PERF
#include <pcap.h>
#endif

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
#include "cm_dns.h"        /* common DNS library defines */

#include "leg.h"            /* defines and macros for EGTP */
#include "egt.h"            /* defines and macros for EGTP */
#include "eg_util.h"            /* defines and macros for EGTP */
#include "eg_dbs16.h"
#ifdef EG_REL_930
#include "eg_dbutl.h"
#endif /* EG_REL_930 */
#include "lav.h"
#include "av.h"


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
#include "cm_dns.x"        /* common DNS library defines */

#include "leg.x"           /* layer management typedefs for EGTP */
#include "egt.x"           /* typedefs for EGTP */
#include "egt_util.x"      /* EGTP Upper Interface utilities */
#include "eg_util.x"       /* EGTP utilities */
#include "lav.x"
#include "av.x"
#include "av_egt_utl.x"

PUBLIC AvPgwCb avPgwCb;
/* av001.101: T3 timer */
TmrCfg t3;

PRIVATE S16 avPgwProcPendingEvnts ARGS((Void));
PRIVATE S16 avPgwDelPendingEvnts ARGS((Void));

PRIVATE S16 avPgwEgtInMBReq ARGS((
AvPgwUeTunnelCb   *tunnelCb,
CmTptAddr         *locAddr,
EgMsg             *egMsg
));

PRIVATE S16 avPgwEgtCTunnelProc ARGS((
AvPgwUeTunnelCb *tunnelCb,
U8              evnt,
EgMsg           *egMsg,
Bool            *msgQueued
));


PRIVATE S16 avPgwEgtCHdlLocalTunnelDel ARGS((
AvPgwUeTunnelCb  *tunnelCb
));

PRIVATE S16 avPgwEgtDBCmdAcc ARGS((
AvPgwUeTunnelCb  *tunnelCb
));

PRIVATE S16 avPgwEgtInDBCmd ARGS((
AvPgwUeTunnelCb   *tunnelCb,
CmTptAddr         *locAddr,
EgMsg             *egMsg
));

PRIVATE S16 avPgwEgtInDBRsp ARGS((
AvPgwUeTunnelCb   *tunnelCb,  /* Local C-Tunnel CB */
EgMsg             *egMsg      /* gtp-c message  */
));

PRIVATE S16 avPgwEgtCTunnelDown ARGS((AvPgwUeTunnelCb *tunnelCb,
                                      CmTptAddr       *lclAddr,
                                      CmTptAddr       *remAddr,
                                      TeId            teId,
                                      U8              evnt,
                                      EgMsg           *egMsg));

PRIVATE S16 avPgwEgtCTunnelPending ARGS((AvPgwUeTunnelCb *tunnelCb,
                                         CmTptAddr       *lclAddr,
                                         CmTptAddr       *remAddr,
                                         TeId            teId,
                                         AvPgwEgtCEvnt   evnt,
                                         EgMsg           *egMsg));

PRIVATE S16 avPgwEgtCTunnelUp  ARGS((AvPgwUeTunnelCb *tunnelCb,
                                     CmTptAddr       *lclAddr,
                                     CmTptAddr       *remAddr,
                                     TeId             teId,
                                     U8               evnt,
                                     EgMsg           *egMsg));

PRIVATE S16 avPgwEgtInCSReq ARGS((CmTptAddr     *lclAddr,
                                                CmTptAddr     *remAddr,
                                                TeId           teId,
                                                EgMsg         *egMsg));


PRIVATE S16 avPgwEgtCSReqAcc ARGS((AvPgwUeTunnelCb *tunnelCb));


PRIVATE S16 avPgwEgtCSReqRej ARGS((AvPgwUeTunnelCb *tunnelCb));


PRIVATE S16 avPgwEgtDSReqAcc ARGS((AvPgwUeTunnelCb *tunnelCb));

PRIVATE S16 avPgwEgtMBReqRej ARGS((
AvPgwUeTunnelCb   *pCTun));

PRIVATE S16 avPgwEgtMBReqAcc ARGS((
AvPgwUeTunnelCb   *pCTun,
EgMsg             *pEgMsg));

PRIVATE S16 avPgwEgtDSReqRej ARGS((AvPgwUeTunnelCb *tunnelCb));


PRIVATE S16 avPgwEgtInDSReq ARGS((AvPgwUeTunnelCb *tunnelCb,
                                             EgMsg           *egMsg));



PRIVATE S16 avPgwSndGtpuTnlMgmtReq ARGS((AvPgwUeTunnelCb *tunnelCb,
                                         U8               action));


PRIVATE S16 avPgwEgtUTunnelDown ARGS((AvPgwUeTunnelCb *tunnelCb,
                                        TeId             teId,
                                        U8               action,
                                        EgtUEvnt        *eguEvnt));

PRIVATE S16 avPgwEgtUTunnelPending ARGS((AvPgwUeTunnelCb *tunnelCb,
                                        TeId             teId,
                                        U8               action,
                                        EgtUEvnt        *eguEvnt));

PRIVATE S16 avPgwEgtUTunnelUp ARGS((AvPgwUeTunnelCb *tunnelCb,
                                    TeId             teId,
                                    U8               action,
                                    EgtUEvnt         *eguEvnt));

PRIVATE S16 avPgwEgtUHdlAddTunnelReqCfm ARGS((AvPgwUeTunnelCb *tunnelCb,
                                              EgtUEvnt        *eguEvnt));


PRIVATE S16 avPgwEgtUHdlDelTunnelReqCfm ARGS((AvPgwUeTunnelCb *tunnelCb,
                                              EgtUEvnt        *eguEvnt));

PRIVATE S16 avPgwEgtUHdlModTunnelReqCfm ARGS((
AvPgwUeTunnelCb   *pUTun,
EgtUEvnt          *pEguEvnt
));


PRIVATE S16 avPgwUtlBldEgtTnlMgmtReq ARGS((AvPgwUeTunnelCb *tunnel,
                                           EgtUEvnt       **eguEvt,
                                           U8               action));


/* av001.101: added handler for incoming create bearer response */
PRIVATE S16 avPgwEgtInCBRsp ARGS(
(
AvPgwUeTunnelCb *tunnelCb,
EgMsg          *egMsg
));


/*
*
*    Fun:    avPgwEgtCTunnelMachine
*
*    Desc:   EGTPC Control Tunnel State Machine,handling important states :
*            AV_PGW_EGTPC_TUNNEL_DOWN
*            AV_PGW_EGTPC_TUNNEL_PENDING,
*            AV_PGW_EGTPC_TUNNEL_UP
*    Note:   
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:   av.c
*
*/
#ifdef ANSI
PUBLIC S16 avPgwEgtCTunnelMachine
(
AvPgwUeTunnelCb *tunnelCb,
CmTptAddr       *lclAddr,
CmTptAddr       *remAddr,
TeId             teId,
AvPgwEgtCEvnt    evnt,
EgMsg           *egMsg
)
#else
PUBLIC S16 avPgwEgtCTunnelMachine(tunnelCb, lclAddr, remAddr, teId, evnt,
                                  egMsg)
AvPgwUeTunnelCb *tunnelCb;
CmTptAddr       *lclAddr;
CmTptAddr       *remAddr;
TeId             teId;
AvPgwEgtCEvnt    evnt;
EgMsg           *egMsg;
#endif
{
   Bool     msgQueued = FALSE;
   S16      ret = ROK;
   U8       state;

   AV_PGW_SND_TRC2(avPgwEgtCTunnelMachine)

   if(NULLP == tunnelCb)
   {
      /* Right first find the UE Control tunnel on which this message has
         arrived from SAP */
      ret = avPgwFndUeTunnelCb(&tunnelCb, teId, AV_PGW_CTRL_TUNNEL);
   }

   if(NULLP != tunnelCb)
   {
      state = tunnelCb->state;
   }
   else
   {
      state = AV_PGW_EGTPC_TUNNEL_DOWN;
   }

   switch(state)
   {
      case AV_PGW_EGTPC_TUNNEL_DOWN:
      {
         ret = avPgwEgtCTunnelDown(tunnelCb, lclAddr, remAddr, teId,
                                   egMsg->msgHdr.msgType,
                                   egMsg);
         break;
      }   

      case AV_PGW_EGTPC_TUNNEL_PENDING:
      {
         ret = avPgwEgtCTunnelPending(tunnelCb, lclAddr, remAddr, teId,
                                    evnt, egMsg);
         break;
      }

      case AV_PGW_EGTPC_TUNNEL_UP:
      {
         if (evnt == AV_PGW_EGTPC_MSG_BR_RES_ACC)
         {
            tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                     "PDN-GW: DEDICATED TUNNEL ADDED SUCCESSFULLY\n"));

         }
         else
         {
         ret = avPgwEgtCTunnelUp(tunnelCb, lclAddr, remAddr, teId,
                                 egMsg->msgHdr.msgType, egMsg);
         }
         break;
      }

      case AV_PGW_EGTPC_TUNNEL_PROC:
      {
         /* av001.101: handle bearer resource command */
         if (evnt == AV_PGW_EGTPC_MSG_BR_RES_ACC)
         {
            tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                     "PDN-GW: DEDICATED TUNNEL ADDED SUCCESSFULLY\n"));

            ret = avPgwProcPendingEvnts();
         }
         else
         {
            ret = avPgwEgtCTunnelProc(tunnelCb, evnt, egMsg, &msgQueued);
         }

         break;
      }

      default:
      {
         ret = RFAILED;
      }
   }

   /* Free the EGTP Message now */
   if (NULLP != egMsg && msgQueued == FALSE)
   {
      EgUtilDeAllocGmMsg(&egMsg);
   }

   AV_PGW_SND_RETVALUE(ret);
}


/*
*
*    Fun:    avPgwEgtCTunnelDown
*
*    Desc:   This function handles requests for EGTP Tunnel in down State.
*
*    Ret:    ROK  -ok
*
*    Notes:  None
*
*    File:  av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwEgtCTunnelDown
(
AvPgwUeTunnelCb *tunnelCb,
CmTptAddr       *lclAddr,
CmTptAddr       *remAddr,
TeId             teId,
U8               evnt,
EgMsg           *egMsg
)
#else
PRIVATE S16 avPgwEgtCTunnelDown(tunnelCb, lclAddr, remAddr, teId, evnt, egMsg)
AvPgwUeTunnelCb *tunnelCb;
CmTptAddr       *lclAddr;
CmTptAddr       *remAddr;
TeId             teId;
U8               evnt;
EgMsg           *egMsg;
#endif
{
   AV_PGW_SND_TRC2(avPgwEgtCTunnelDown)

   switch(evnt)
   {
      case EGT_GTPC_MSG_CS_REQ:
      {
         /*Added a Klockworks Check : Replacing %d with %ld*/
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Create "
                           "Session Request from S-GW:tunnel Id(%d)\n",
                           (U32)teId)); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Create "
                           "Session Request from S-GW:tunnel Id(%ld)\n",
                           (U32)teId)); 
#endif
         AV_PGW_SND_RETVALUE(avPgwEgtInCSReq(lclAddr, remAddr,
                                                teId, egMsg));
      } 
      default:
      {
        /* Should we check for sending reject, we cannot recognize the event*/
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                         "avPgwEgtCTunnelDown: Unhandled event"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }  
   }
}


/*
*
*    Fun:    avPgwEgtInCSReq
*
*    Desc:   This function handles new incoming Create Session Request from EGTP
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwEgtInCSReq
(
CmTptAddr      *lclAddr,
CmTptAddr      *remAddr,
TeId            teId,
EgMsg          *egMsg
)
#else
PRIVATE S16 avPgwEgtInCSReq(lclAddr, remAddr, teId, egMsg)
CmTptAddr      *lclAddr;
CmTptAddr      *remAddr;
TeId            teId;
EgMsg          *egMsg;
#endif
{
   AvPgwUeTunnelCb   *s5s8CTun = NULLP;
   AvPgwUeTunnelCb   *s5s8UTun = NULLP;
   AvPgwUeRouteCb    *newUeRouteCb = NULLP;
   S16               ret;
   AvPgwUeCb         *ueCb = NULLP;
   AvPgwPdnCb        *pdnCb = NULLP;
   U32               cause = 0;
   AvPgwBearerCb     *defBearer = NULLP;
   EgIe              *egIe = NULLP;
   AvImsi            imsi = {0, {'\0'}};
   U16               idx = 0;

   AV_PGW_SND_TRC2(avPgwEgtInCSReq)

   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_IMSI_IETYPE, 0);
   if(ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Retrieving IMSI IE from "
                           "GTP-C message"));
      AV_PGW_SND_RETVALUE(ret);

   }
   else
   {
      cmMemset((U8 *)&imsi, 0, sizeof(AvImsi));
      imsi.len = egIe->t.valStr32.length;
#if 0
      cmMemcpy(imsi.buf, (U8 *)&(egIe->t.valStr8.val), AV_PGW_IMSI_LEN);
#else
      for(idx = 0; idx < imsi.len; idx ++)
      {
         imsi.buf[idx] = egIe->t.valStr32.val[idx] - '0';
      }
#endif
   }

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Create Session Request Recieved "
                     "with IMSI: %x%x%x%x%x%x%x%x",
                     imsi.buf[0], imsi.buf[1], imsi.buf[2], imsi.buf[3],
                     imsi.buf[4], imsi.buf[5], imsi.buf[6], imsi.buf[7]));

   ret = avPgwFndUeCb(&ueCb, &imsi);
   if (ROK != ret)
   {
      /* Attach Request */
      ret = avPgwAddUeCb(&imsi, &ueCb);
      if (ROK != ret)
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Creation of UE CB"));
         AV_PGW_SND_RETVALUE(ret);
      }
   }
   else
   {
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Found UE CB"));

      /* If Create Session Request is received on existing tunnel
       * then delete the entire UE CB and tunnels send a failure response
       */
      if (avPgwIsCTunExist(ueCb, egMsg))
      {
         /* Overlapping Request: happens when SGW sends a Create Session
          * Request without doing a Delete previously */
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Creation Session Request "\
               " received on existing tunnel"));
         AV_PGW_SND_RETVALUE(ret);
      }
      else
      {
         /* UE Requested PDN Connection Request */
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Received UE Requested "
                           "PDN Connection Request"));
      }
   }

   AV_PDNGW_SENDTSK_ALLOC(pdnCb, sizeof(AvPgwPdnCb));
   if (pdnCb == NULLP)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                      "Memory Allocation for AvPgwPdnCb"));
      AV_PGW_SND_RETVALUE(ret);
   }

   pdnCb->ueCb = ueCb;
   pdnCb->transId.pres = PRSNT_NODEF;
   pdnCb->transId.val = egMsg->msgHdr.seqNumber;

   ret = avPgwEgtValCSReq(ueCb, pdnCb, egMsg, &cause);
   if (ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                        "Validation of Create Session Request, FAILED"));
      avPgwEgtSndFailureRsp(pdnCb, teId, lclAddr, remAddr,
                           EGT_GTPC_MSG_CS_RSP, pdnCb->transId, cause);
      AV_PGW_SND_RETVALUE(ret);
   }
   
   s5s8CTun = pdnCb->s5s8CTun;
   if(NULLP == s5s8CTun)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                           "S5/S8 Tunnel is not valid"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
      ret = RFAILED;
      AV_PGW_SND_RETVALUE(ret);
   }

   ret = avPgwAddUeCtrlTunnelCb(&s5s8CTun, ueCb, teId, lclAddr,
                              remAddr, egMsg);
   if(ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                           "Adding UE Control Tunnel to Hash List"));
      cause = EGT_GTPC_CAUSE_SYS_FAIL;
   }
   else
   {
      defBearer = avPgwFndDefBearer(pdnCb);
      if (defBearer != NULLP)
      {
         s5s8UTun = defBearer->s5s8UTun;
         if(NULLP != s5s8UTun)
         {  
            ret = avPgwAddUeDataTunnelCb(&s5s8UTun, ueCb, lclAddr,
                  AV_PGW_DATA_TUNNEL_DFLT);
         }
      }
      else
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Default Bearer not found"));
         cause = EGT_GTPC_CAUSE_SYS_FAIL;
      }
   }

   /* Create a new route control block for this UE with a new IP Address
    *  assigned for this PDN Connection */
   if (ROK == ret)
   {
      ret = avPgwAddUeRouteCb(s5s8UTun, &newUeRouteCb);
      if (ROK != ret || newUeRouteCb == NULLP)
      {
         ret = RFAILED;
         cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Adding Route Cb"));
      }
   }

   /* Trigger on data tunnel state machine to add default tunnel through 
    * GTP-U Stack Interface primitive */
   if (ret == ROK)
   {
      /* Associate route control block with this newly created control
       * tunnel */
      s5s8CTun->ueRouteCb = newUeRouteCb;
      s5s8CTun->state = AV_PGW_EGTPC_TUNNEL_PENDING;

      ret = avPgwEgtUTunnelMachine(s5s8UTun, s5s8UTun->localTeIdKey,
                                EGT_TMGMT_REQTYPE_ADD, NULLP);
   }
   else
   {
      cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
      avPgwEgtSndFailureRsp(pdnCb, teId, lclAddr, remAddr,
                           EGT_GTPC_MSG_CS_RSP, pdnCb->transId, cause);

      /* This will delete even the def u-tunnel and UE CB */
      avPgwDelUeTunnelCb(ueCb, &s5s8CTun, AV_PGW_CTRL_TUNNEL);
   }

   AV_PGW_SND_RETVALUE(ret);

} /*avPgwEgtInCSReq */


/*
*
*    Fun:    avPgwEgtCTunnelPending
*
*    Desc:   eGTPC Tunnel Pending State Handler.
*
*    Ret:    
*
*    Notes:  
*
*    Notes:  None
*
*    File:  av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtCTunnelPending
(
AvPgwUeTunnelCb *tunnelCb,
CmTptAddr       *lclAddr,
CmTptAddr       *remAddr,
TeId            teId,
AvPgwEgtCEvnt   evnt,
EgMsg           *egMsg
)
#else
PRIVATE S16 avPgwEgtCTunnelPending(tunnelCb, lclAddr, remAddr, teId, evnt, 
                                   egMsg)
AvPgwUeTunnelCb *tunnelCb;
CmTptAddr       *lclAddr;
CmTptAddr       *remAddr;
TeId            teId;
AvPgwEgtCEvnt   evnt;
EgMsg           *egMsg;
#endif
{
   S16      ret;

   AV_PGW_SND_TRC2(avPgwEgtCTunnelPending)

   switch(evnt)
   {
      /* We have received Create Session Request
         whose transaction is pending */

      case AV_PGW_EGTPC_MSG_CS_REQ_ACC: /* Successful event */
      { 
#ifdef ALIGN_64BIT
         /*Added Klockworks Check : Replacing %d with %ld*/
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Local creation of "
                           "tunnel(s) successful: tunnel Id(%d)\n", teId)); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Local creation of "
                           "tunnel(s) successful: tunnel Id(%ld)\n", teId)); 
#endif
         ret = avPgwEgtCSReqAcc(tunnelCb);
         break;
      }
      case AV_PGW_EGTPC_MSG_CS_REQ_REJ: /* Unsuccessful event */
      {
         /*Added Klockworks Check : Replacing %d with %ld*/
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "PDN-GW: Local creation of "
                     "tunnel(s) unsuccessful: tunnel Id(%d)\n", teId)); 
#else
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "PDN-GW: Local creation of "
                     "tunnel(s) unsuccessful: tunnel Id(%ld)\n", teId)); 
#endif

         /* Build Create Session Response with unsuccessful cause*/
         ret = avPgwEgtCSReqRej(tunnelCb);
         break;
      }

      /* We have received Delete Session Request
         whose transaction is pending */
      case AV_PGW_EGTPC_MSG_DS_REQ_ACC: /* Successful event */
      {
          /*Added Klockworks Check : Replacing %d with %ld*/
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                           "tunnel(s) session successful: tunnel Id(%d)\n ",
                           teId));
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                           "tunnel(s) session successful: tunnel Id(%ld)\n ",
                           teId));
#endif
         ret = avPgwEgtDSReqAcc(tunnelCb);
         break;
      }
      case AV_PGW_EGTPC_MSG_DS_REQ_REJ: /* Unsuccessful event */
      {  
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                              "tunnel(s) session unsuccessful: tunnel "
                              "Id(%d)\n",teId));
#else
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                              "tunnel(s) session unsuccessful: tunnel "
                              "Id(%ld)\n",teId));
#endif
         ret = avPgwEgtDSReqRej(tunnelCb);
         break;
      }

      case AV_PGW_EGTPC_MSG_DB_CMD_ACC:
      {
         tunnelCb->pdnCb->cEvent = AV_PGW_EGTPC_MSG_UNKNOWN;
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                              "tunnel(s) session successful: tunnel "
                              "Id(%d)\n",teId));
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                              "tunnel(s) session successful: tunnel "
                              "Id(%ld)\n",teId));
#endif
         ret = avPgwEgtDBCmdAcc(tunnelCb);
         break;
      }

      case AV_PGW_EGTPC_LOC_TUN_DEL:
      {
         tunnelCb->pdnCb->cEvent = AV_PGW_EGTPC_MSG_UNKNOWN;
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                              "tunnel(s) session successful: tunnel "
                              "Id(%d)\n",teId));
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Local deletion of "
                              "tunnel(s) session successful: tunnel "
                              "Id(%ld)\n",teId));
#endif
         ret = avPgwEgtCHdlLocalTunnelDel(tunnelCb);
      }

      default:
      {
        /* Should we check for sending reject, we cannot recognize the event
           itself ? :) */
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Unhandled event"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }  
   }

   AV_PGW_SND_RETVALUE(ret);
}



/*
*
*    Fun:    avPgwEgtCSReqRej
*
*    Desc:   This function handles successful creation of session with EGTP-C 
*            tunnel confirmation status from EGTP.
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwEgtCSReqAcc
(
AvPgwUeTunnelCb  *tunnelCb
)
#else
PRIVATE S16 avPgwEgtCSReqAcc(tunnelCb)
AvPgwUeTunnelCb  *tunnelCb;
#endif
{

   EgMsg           *egCSRspMsg = NULLP;
   AvPgwUeTunnelCb *dfltTunnelCb;
   TknU32          localTeId;
   S16             ret;

   AV_PGW_SND_TRC2(avPgwEgtCSReqAcc)
   /*Added Klockworks Check : Initialising localTeId*/
   /* Fill the ie properties */
   cmMemset((U8 *)&localTeId, 0 , sizeof(TknU32)); 


   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
         
   /* Build Create Session Response with successful cause */
   ret = avPgwEgtUtlBldCSRsp(tunnelCb, &egCSRspMsg, 
                                         EGT_GTPC_CAUSE_ACC_REQ);

   if (ROK != ret)
   {  
      dfltTunnelCb = avPgwFndDefUTun(tunnelCb->pdnCb);

      if(NULLP == dfltTunnelCb)
      {
         AV_PGW_SND_RETVALUE(RFAILED);
      }   

      /* Delete the Route Control Block for this UE */
      avPgwDelUeRouteCb(dfltTunnelCb);

      /* Delete the EGTPC locally */
      EuLiEgtLclDelReq(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                          0, tunnelCb->localTeIdKey);

      /* Delete the Default EGTPU local tunnel */
      avPgwEgtUTunnelMachine(dfltTunnelCb, dfltTunnelCb->localTeIdKey,
                             EGT_TMGMT_REQTYPE_DEL, NULLP);

      AV_PGW_SND_RETVALUE(ret);
   }

   localTeId.pres = TRUE;
   localTeId.val = tunnelCb->localTeIdKey;

   ret = EuLiEgtSigRsp(&(avPgwCb.egtCSapCb->pst), (avPgwCb.egtCSapCb->spId),
                     localTeId, egCSRspMsg);

   AV_PGW_SND_RETVALUE(ret);
}


/*
*
*    Fun:    avPgwEgtCSReqRej
*
*    Desc:   This function handles unsuccessful creation of session with EGTP-C
*            tunnel confirmation status from EGTP.
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtCSReqRej
(
AvPgwUeTunnelCb *tunnelCb
)
#else
PRIVATE S16 avPgwEgtCSReqRej(tunnelCb)
AvPgwUeTunnelCb *tunnelCb;
#endif
{
   EgMsg           *egCSRspMsg = NULLP;
   S16              ret;
   TknU32          localTeId;
   AvPgwUeTunnelCb   *defUTun = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtCSReqRej)
   
   /* Added Klockworks Check */
   /* Fill the ie properties */
   cmMemset((U8 *)&localTeId, 0 , sizeof(TknU32));

   defUTun = avPgwFndDefUTun(tunnelCb->pdnCb);
   if(NULLP != defUTun)
   {   
      avPgwDelUeRouteCb(defUTun);
   }
   /* No Need to delete the EGTPU tunnel, since the same tunnel is
      creation failure caused this  */

   /* Build Create Session Response with unsuccessful cause*/
   ret = avPgwEgtUtlBldCSRsp(tunnelCb, &egCSRspMsg, 
                                         EGT_GTPC_CAUSE_SYS_FAIL);

   if(ROK != ret)
   {
      /* We are not able to build response itself cannot do more */
      /* Debug Print here */
      AV_PGW_SND_RETVALUE(ret);
   }
   localTeId.pres = TRUE;
   localTeId.val = tunnelCb->localTeIdKey;

   EuLiEgtSigRsp(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
               localTeId, egCSRspMsg);

   AV_PGW_SND_RETVALUE(ROK);
}

/**************************************************************************//**
 *
 * @brief 
 *    Sends Modify Bearer Response to SGW
 *             
 * @param[in]  pCTun
 *    Pointer to the control plane tunnel on whith Modify Bearer Response
 *    Has to be sent to SGW
 * @param[in]  pMbRsp
 *    Pointer to GTP message structure containing Modify Bearer Response
 * 
 * @return
 *    SUCCESS if handling is successful otherwise its FAILURE
 *
 *****************************************************************************/
PRIVATE S16 avPgwEgtMBReqAcc
(
AvPgwUeTunnelCb   *pCTun,
EgMsg             *pMbRsp
)
{
   TknU32         teid = {0};

   AV_PGW_SND_TRC2(avPgwEgtMBReqAcc)

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Sending Modify Bearer Response"\
         "to EGT Layer"));

   teid.pres = PRSNT_NODEF;
   teid.val = pCTun->localTeIdKey;
   EuLiEgtSigRsp(&(avPgwCb.egtCSapCb->pst), (avPgwCb.egtCSapCb->spId),
         teid, pMbRsp);

   AV_PGW_SND_RETVALUE(ROK);

} /* avPgwEgtMBReqAcc */


/**************************************************************************//**
 *
 * @brief 
 *    Sends Modify Bearer Response with failure cause to SGW
 *             
 * @param[in]  pCTun
 *    Pointer to the control plane tunnel on whith Modify Bearer Response
 *    Has to be sent to SGW
 * 
 * @return
 *    SUCCESS if handling is successful otherwise its FAILURE
 *
 *****************************************************************************/
PRIVATE S16 avPgwEgtMBReqRej
(
AvPgwUeTunnelCb   *pCTun
)
{
   AvPgwPdnCb     *pPdn = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtMBReqRej)

   pPdn = pCTun->pdnCb;
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Sending Modify Bearer Response"\
         "with failure to SGW"));

   avPgwEgtSndFailureRsp(pPdn, pCTun->localTeIdKey, &(pCTun->lclAddr),\
         &(pCTun->remAddr), EGT_GTPC_MSG_MB_RSP, pPdn->transId,\
         EGT_GTPC_CAUSE_SYS_FAIL);

   AV_PGW_SND_RETVALUE(ROK);
}


/*
*
*    Fun:    avPgwEgtDSReqAcc
*
*    Desc:   This function handles successful local deletion of EGTP-C tunnel
*            confrimation status from EGTP.
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtDSReqAcc
(
AvPgwUeTunnelCb  *tunnelCb
)
#else
PRIVATE S16 avPgwEgtDSReqAcc(tunnelCb)
AvPgwUeTunnelCb  *tunnelCb;
#endif
{
   EgMsg          *egDSRspMsg = NULLP;
   TeId            teId = {0};
   S16             ret = ROK;
   TknU32          localTeId = {0};

   AV_PGW_SND_TRC2(avPgwEgtDSReqAcc)

   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_DOWN;

   /* Build Delete Session Response with successful cause*/
   ret = avPgwUtlBldEgtDelSessionRsp(tunnelCb, &egDSRspMsg, 
                                     EGT_GTPC_CAUSE_ACC_REQ);
   if (ROK != ret)
   {  
      /* We are not able to build response itself cannot do more */
      /* Debug Print here */
      AV_PGW_SND_RETVALUE(ret);
   }

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Sending Delete Session "
                        "Response to S-GW on tunnel Id(%u)\n",
                        (tunnelCb->localTeIdKey))); 
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Sending Delete Session "
                        "Response to S-GW on tunnel Id(%lu)\n",
                        (tunnelCb->localTeIdKey))); 
#endif

   localTeId.pres = TRUE;
   localTeId.val = tunnelCb->localTeIdKey;

   /* Send the Delete Session Response */
   ret = EuLiEgtSigRsp(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                     localTeId, egDSRspMsg);

   /* Postpone the deletion of tunnel till Local Delete 
      confirmation */

   /* Debug Print here */
   /* Need to check for transId */
   teId = tunnelCb->localTeIdKey;
   ret = EuLiEgtLclDelReq(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                        0, teId);

   AV_PGW_SND_RETVALUE(ret);
}


/*
*
*    Fun:    avPgwEgtDSReqRej
*
*    Desc:   This function handles unsuccessful local deletion of EGTP-C tunnel
*            confrimation status from EGTP.
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwEgtDSReqRej
(
AvPgwUeTunnelCb *tunnelCb
)
#else
PRIVATE S16 avPgwEgtDSReqRej(tunnelCb)
AvPgwUeTunnelCb *tunnelCb;
#endif
{
   EgMsg   *egDSRspMsg = NULLP;
   S16     ret;
   TknU32  localTeId;

   AV_PGW_SND_TRC2(avPgwEgtDSReqRej)
   /* Added klockworks Check : Initialising localTeId*/  
   /* Fill the ie properties */
   cmMemset((U8 *)&localTeId, 0 , sizeof(TknU32));

   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_DOWN;
  /* Build Delete Session Response with unsuccessful cause */
   ret = avPgwUtlBldEgtDelSessionRsp(tunnelCb, &egDSRspMsg, 
                                 EGT_GTPC_CAUSE_SYS_FAIL);

   if(ROK != ret)
   {  
      /* We are not able to build response itself cannot do more */
      /* Debug Print here */
      AV_PGW_SND_RETVALUE(ret);
   }
   /* Debug Print here */

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,"PDN-GW: Rejecting Delete Session Request to S-GW on tunnel Id(%u)\n", 
          (tunnelCb->localTeIdKey))); 
#else
   AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,"PDN-GW: Rejecting Delete Session Request to S-GW on tunnel Id(%lu)\n", 
          (tunnelCb->localTeIdKey))); 
#endif

   localTeId.pres = TRUE;
   localTeId.val = tunnelCb->localTeIdKey;

   /* Send the Delete Session Response to EGTP */
   ret = EuLiEgtSigRsp(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                     localTeId, egDSRspMsg);

   /* Need to check for transId */
   AV_PGW_SND_RETVALUE(ret);
}



/*
 *
 *    Fun:    avPgwEgtCTunnelProc
 *
 *    Desc:   GTP Tunnel Established/Up State Handler.
 *
 *    Ret:    
 *
 *    Notes:  It handles deleting the tunnel,Ignores rest of the event.
 *
 *    File:   av.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwEgtCTunnelProc
(
AvPgwUeTunnelCb *tunnelCb,
U8              evnt,
EgMsg           *egMsg,
Bool            *msgQueued
)
#else
PRIVATE S16 avPgwEgtCTunnelProc(tunnelCb, evnt, egMsg, msgQueued)
AvPgwUeTunnelCb *tunnelCb;
U8              evnt;
EgMsg           *egMsg;
Bool            *msgQueued;
#endif
{
   S16 ret = RFAILED;

   AV_PGW_SND_TRC2(avPgwEgtCTunnelProc)

   *msgQueued = FALSE;

   switch(evnt)
   {
      case EGT_GTPC_MSG_DS_REQ: /* Delete Session Request */
      { 
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Delete "
                           "Session Request from S-GW on tunnel Id(%d)\n",
                           (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Delete "
                           "Session Request from S-GW on tunnel Id(%ld)\n",
                           (tunnelCb->localTeIdKey))); 
#endif

         /* in the case of delete session request, ignore any buffered
          * events and process this request */
         avPgwDelPendingEvnts();
         tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
         AV_PGW_SND_RETVALUE(avPgwEgtInDSReq(tunnelCb, egMsg));
         break;
      }

      case EGT_GTPC_MSG_CB_RSP:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Create Bearer Response from S-GW on "
                  "tunnel Id(%d)\n", (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Create Bearer Response from S-GW on "
                  "tunnel Id(%ld)\n", (tunnelCb->localTeIdKey))); 
#endif
         
         if (tunnelCb->pdnCb->expectedMsg == EGT_GTPC_MSG_CB_RSP)
         {
            avPgwEgtInCBRsp(tunnelCb, egMsg);
            ret = avPgwProcPendingEvnts();
         }
         else
         {
            avPgwEgtQMsg(tunnelCb, evnt, egMsg);
            *msgQueued = TRUE;
         }

         break;
      }

      case EGT_GTPC_MSG_DB_RSP:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Response from S-GW on "
                  "tunnel Id(%d)\n", (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Response from S-GW on "
                  "tunnel Id(%ld)\n", (tunnelCb->localTeIdKey))); 
#endif
         
         if (tunnelCb->pdnCb->expectedMsg == EGT_GTPC_MSG_DB_RSP)
         {
            avPgwEgtInDBRsp(tunnelCb, egMsg);
            ret = avPgwProcPendingEvnts();
         }
         else
         {
            avPgwEgtQMsg(tunnelCb, evnt, egMsg);
            *msgQueued = TRUE;
         }

         break;
      }

      /* handling of Delete Bearer Command from SGW */
      case EGT_GTPC_MSG_BR_CMND:
      case EGT_GTPC_MSG_MB_CMND:
      case EGT_GTPC_MSG_DB_CMND:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Command from S-GW on "
                  "tunnel Id(%d)\n", (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Command from S-GW on "
                  "tunnel Id(%ld)\n", (tunnelCb->localTeIdKey))); 
#endif

         avPgwEgtQMsg(tunnelCb, evnt, egMsg);
         *msgQueued = TRUE;
         break;
      }

      case AV_PGW_EGTPC_MSG_MB_REQ_ACC:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "GTPU Tunnel Modification "\
               "successful: Tunnel ID: [%d]", tunnelCb->localTeIdKey));
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "GTPU Tunnel Modification "\
               "successful: Tunnel ID: [%ld]", tunnelCb->localTeIdKey));
#endif
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Sending Modify Bearer "\
               "Response to SGW"));

#if 0
         /* Modify Bearer Response is sent soon after building it.
          * Not waiting for tunnel modify cfm from egt layer. so commented
          * this portion
          */
         ret = avPgwEgtMBReqAcc(tunnelCb, egMsg);
#endif
         ret = ROK;
         tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
         break;
      }

      default:
      {
         break;
      }
   }

   AV_PGW_SND_RETVALUE(ret);
}



/*
*
*    Fun:    avPgwEgtCTunnelUp
*
*    Desc:   GTP Tunnel Established/Up State Handler.
*
*    Ret:    
*
*    Notes:  It handles deleting the tunnel,Ignores rest of the event.
*
*    File:   av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwEgtCTunnelUp
(
AvPgwUeTunnelCb *tunnelCb,
CmTptAddr       *lclAddr,
CmTptAddr       *remAddr,
TeId             teId,
U8              evnt,
EgMsg           *egMsg
)
#else
PRIVATE S16 avPgwEgtCTunnelUp(tunnelCb, lclAddr, remAddr, teId, evnt, egMsg)
AvPgwUeTunnelCb *tunnelCb;
CmTptAddr       *lclAddr;
CmTptAddr       *remAddr;
TeId            teId;
U8              evnt;
EgMsg           *egMsg;
#endif
{
   S16 ret = RFAILED;

   AV_PGW_SND_TRC2(avPgwEgtCTunnelUp)

   switch(evnt)
   {
      case EGT_GTPC_MSG_DS_REQ: /* Delete Session Request */
      { 
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Delete "
                           "Session Request from S-GW on tunnel Id(%d)\n",
                           (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Delete "
                           "Session Request from S-GW on tunnel Id(%ld)\n",
                           (tunnelCb->localTeIdKey))); 
#endif

         AV_PGW_SND_RETVALUE(avPgwEgtInDSReq(tunnelCb, egMsg));

         break;
      }

      case EGT_GTPC_MSG_MB_REQ:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Modify "
                           "Bearer Request from S-GW on tunnel Id(%d)\n",
                           (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Received Modify "
                           "Bearer Request from S-GW on tunnel Id(%ld)\n",
                           (tunnelCb->localTeIdKey))); 
#endif
         AV_PGW_SND_RETVALUE(avPgwEgtInMBReq(tunnelCb, lclAddr, egMsg));
         break;
      }

      /* av001.101: Add the BR Command handling */
      case EGT_GTPC_MSG_CB_RSP:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Create Bearer Response from S-GW on "
                  "tunnel Id(%d)\n", (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Create Bearer Response from S-GW on "
                  "tunnel Id(%ld)\n", (tunnelCb->localTeIdKey))); 
#endif

         AV_PGW_SND_RETVALUE(avPgwEgtInCBRsp(tunnelCb, egMsg));
         break;
      }

      /* handling of Delete Bearer Command from SGW */
      case EGT_GTPC_MSG_DB_CMND:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Command from S-GW on "
                  "tunnel Id(%d)\n", (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Command from S-GW on "
                  "tunnel Id(%ld)\n", (tunnelCb->localTeIdKey))); 
#endif

         AV_PGW_SND_RETVALUE(avPgwEgtInDBCmd(tunnelCb, lclAddr, egMsg));
         break;
      }

      /* handling of Delete Bearer Command from SGW */
      case EGT_GTPC_MSG_DB_RSP:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Response from S-GW on "
                  "tunnel Id(%d)\n", (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "PDN-GW: Received Delete Bearer Response from S-GW on "
                  "tunnel Id(%ld)\n", (tunnelCb->localTeIdKey))); 
#endif

         AV_PGW_SND_RETVALUE(avPgwEgtInDBRsp(tunnelCb, egMsg));
         break;
      }

      /* case EGT_GTPC_MSG_MOD_REQ: *//* We arent handling this */
      default:
      {
         break;
      }
   }
   AV_PGW_SND_RETVALUE(ret);
}


/* av001.101: Added handler for incoming create bearer response msg */
#ifdef ANSI
PRIVATE S16 avPgwEgtInCBRsp
(
AvPgwUeTunnelCb *tunnelCb,
EgMsg          *egMsg
)
#else
PRIVATE S16 avPgwEgtInCBRsp(tunnelCb, egMsg)
AvPgwUeTunnelCb *tunnelCb;
EgMsg          *egMsg;
#endif
{
   AvPgwUeTunnelCb  *dedTunnelCb = NULLP;
   AvPgwUeRouteCb  *newUeRouteCb = NULLP;
   U16            ret = ROK;
   AvPgwUeCb      *ueCb = NULLP;
   AvPgwPdnCb     *pdnCb = NULLP;
   CmLList        *tmp = NULLP;
   Bool           sndDBReq = TRUE;

   AV_PGW_SND_TRC2(avPgwEgtInCBRsp)

   ueCb = tunnelCb->ueCb;
   pdnCb = tunnelCb->pdnCb;

   ret = avPgwEgtValCBRsp(ueCb, pdnCb, egMsg, &sndDBReq);
   if (ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Validation of Create Bearer "
                           "Response Failed"));
      tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
      avPgwUtlDelPendingTunLst(&(pdnCb->pendingTunLst));
      RETVALUE(ret);
   }

   /* Add the Route */
   /* av004.101: we take the same UeRouteCb which we have allocated for the 
    * default bearer, we reuse the same
    * from dataTunnel we can get the ueCb, from there ueRouteCb
    */
   CM_LLIST_FIRST_NODE(&(pdnCb->pendingTunLst), tmp);
   while (tmp)
   {
      dedTunnelCb = ((AvPgwPendingTun *)tmp)->uTun;
      dedTunnelCb->ueCb = ueCb;
      cmHashListInsert(&avPgwCb.egtUSapCb->ueTunnelCbHlCp,
                     (PTR)(dedTunnelCb), (U8 *)&((dedTunnelCb)->localTeIdKey),
                     (U16)sizeof((dedTunnelCb)->localTeIdKey));

      ret = SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&(newUeRouteCb),
                  sizeof(AvPgwUeRouteCb));
      if(ROK != ret)
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Failed to allocate "
                              "Route Cb for a Dedicated Bearer \n"));
      }
      else
      {
         cmMemset((U8 *)newUeRouteCb, 0, (sizeof(AvPgwUeRouteCb)));
         dedTunnelCb->ueRouteCb = newUeRouteCb;
         ret = avPgwInitUeRouteCb(dedTunnelCb, newUeRouteCb);

         /* Now send the Add Tunnel Req to eGTP-u */
         /* Trigger on data tunnel state machine to add dedicated tunnel 
          * through GTP-U Stack Interface primitive */
         ret = avPgwEgtUTunnelMachine(dedTunnelCb, dedTunnelCb->localTeIdKey,
                                    EGT_TMGMT_REQTYPE_ADD, NULLP);
         if (ROK != ret)
         {
            AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "GTPU Tunnel machine"));
         }
      }

      CM_LLIST_NEXT_NODE(&(pdnCb->pendingTunLst), tmp);
   }

   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
   avPgwUtlDelPendingTunLst(&(pdnCb->pendingTunLst));
   AV_PGW_SND_RETVALUE(ret);
}



#ifdef ANSI
PRIVATE S16 avPgwEgtInDSReq
(
AvPgwUeTunnelCb *tunnelCb,
EgMsg          *egMsg
)
#else
PRIVATE S16 avPgwEgtInDSReq(tunnelCb, egMsg)
AvPgwUeTunnelCb *tunnelCb;
EgMsg          *egMsg;
#endif
{
   U32               bCnt = 0;
   AvPgwBearerCb     **bearerTable = NULLP;
   S16               ret = ROK;
   AvPgwPdnCb        *pdnCb = NULLP;
   AvPgwBearerCb     *bearer = NULLP;
   AvPgwBearerCb     *defBearer = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtInDSReq)

   pdnCb = tunnelCb->pdnCb;
   pdnCb->transId.val = egMsg->msgHdr.seqNumber;
   pdnCb->cEvent = AV_PGW_EGTPC_MSG_DS_REQ_ACC;
   pdnCb->transId.pres = PRSNT_NODEF;
   pdnCb->transId.val = egMsg->msgHdr.seqNumber;

   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_PENDING;
   bearerTable = tunnelCb->ueCb->bearerTable;

   /* Deleting the Dedicated u-plane tunnels */
   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      bearer = bearerTable[bCnt];
      if ((bearer != NULLP) && 
         (AV_PGW_CHK_BEARER_MAP(pdnCb->bearerMap, bearer->ebi)))
      {
         if (bearer->isDefBearer == TRUE)
         {
            defBearer = bearer;
         }
         else
         {
            ret = avPgwDelBearerCb(bearer);
            if (ROK != ret)
            {
               AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                                 "Deleting of Dedicated Data tunnel"));
            }
         }
      }
   }

   /* Initiate deleting the data default tunnel
    * Trigger on Data tunnel state machine to delete this GTP-U tunnel
    *  using GTP-U Stack interface */
   if (NULLP != defBearer)
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Deleting the PDN"));

      /* Since the default U tunnel tunnel is getting deleted, set 
       * the status of C-tunnel to pending */
      ret = avPgwDelPdnCb(pdnCb);
      if (ROK != ret)
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Deleting Pdn CB"));
      }
   }

   AV_PGW_SND_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Handling of Modify Bearer Request received from SGW. Only S1-Handover
 *    with SGW relocation is handled here. Sends out Modify Bearer Response
 *    to SGW.
 *             
 * @param pdnCb
 *    Pointer to PDN Session CB on which the MBReq received
 * @param egMsg
 *    Pointer to GTP message structure containing MBReq
 * 
 * @return
 *    SUCCESS if handling is successful otherwise its FAILURE
 *
 *****************************************************************************/
PRIVATE S16 avPgwEgtOutMBRsp
(
AvPgwPdnCb     *pPdnCb,
EgMsg          *pEgMsg
)
{
   TknU32            localTeId = {0};
   EgMsg             *pMbRsp   = NULLP;
   S16               ret       = ROK;
   CmLList           *pListEnt = NULLP;
   AvPgwUeTunnelCb   *pUTun    = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtOutMBRsp);

   pPdnCb->transId.val = pEgMsg->msgHdr.seqNumber;
   ret = avPgwBldMBRsp(pPdnCb, &pMbRsp);
   if (ROK == ret)
   {
      CM_LLIST_FIRST_NODE(&(pPdnCb->pendingTunLst), pListEnt);
      while (pListEnt)
      {
         pUTun = ((AvPgwPendingTun *)pListEnt)->uTun;
         if (NULLP != pUTun)
         {
            AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Sending Tunnel "\
                  "Modifying Request to EGT layer"));

            pPdnCb->s5s8CTun->state = AV_PGW_EGTPC_TUNNEL_PROC;
            ret = avPgwEguTunnelModReq(pUTun, NULLP);
            if (ROK != ret)
            {
               AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Sending Tunnel "\
                     "Modifying Request to EGT layer"));
            }
         }
      
         cmLListDelFrm(&(pPdnCb->pendingTunLst), pListEnt);
         AV_PDNGW_SENDTSK_FREE(pListEnt, sizeof(AvPgwPendingTun));

         CM_LLIST_NEXT_NODE(&(pPdnCb->pendingTunLst), pListEnt);
      }

      if (ROK == ret)
      {
         AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Sending Modify Bearer "\
               "Response message to EGT Layer"));

         localTeId.pres = TRUE;
         localTeId.val = pPdnCb->s5s8CTun->localTeIdKey;
         EuLiEgtSigRsp(&(avPgwCb.egtCSapCb->pst), avPgwCb.egtCSapCb->spId,
               localTeId, pMbRsp);
      }
   }
   else
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Building Modify Bearer "\
            "Request"));
   }

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwEgtOutMBRsp */


/**************************************************************************//**
 *
 * @brief 
 *    Handling of Modify Bearer Request received from SGW. Only S1-Handover
 *    with SGW relocation is handled here.
 *             
 * @param tunnelCb
 *    Pointer to control plane tunnel CB on which the MBReq received
 * @param locAddr
 *    Pointer to local address at which the request is received
 * @param egMsg
 *    Pointer to GTP message structure
 * 
 * @return
 *    SUCCESS if handling is successful otherwise its FAILURE
 *
 *****************************************************************************/
PRIVATE S16 avPgwEgtInMBReq
(
AvPgwUeTunnelCb   *tunnelCb,
CmTptAddr         *locAddr,
EgMsg             *egMsg
)
{
   S16         ret       = ROK;
   AvPgwPdnCb  *pdnCb    = NULLP;
   U32         cause     = 0;

   AV_PGW_SND_TRC2(avPgwEgtInMBReq);

   pdnCb = tunnelCb->pdnCb;

   pdnCb->transId.pres = PRSNT_NODEF;
   pdnCb->transId.val = egMsg->msgHdr.seqNumber;

   ret = avPgwValMBReq(pdnCb, egMsg, &cause);
   if (ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Validation of Modify "\
            "Bearer Request"));
   }

   /* Send the Modify Bearer Response to the SGW */
   if (ROK == ret)
   {
      ret = avPgwEgtOutMBRsp(pdnCb, egMsg);
      if (ROK != ret)
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Sending of Modify Bearer "\
               "Request to SGW"));
      }
   }
   else
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Sending Modify Bearer "
            "Response with failure Cause to SGW"));
      avPgwEgtSndFailureRsp(pdnCb, tunnelCb->localTeIdKey, locAddr,\
            &(pdnCb->s5s8CTun->remAddr), EGT_GTPC_MSG_MB_RSP,\
            pdnCb->transId, cause);
   }
   
   AV_PGW_SND_RETVALUE(ret);
} /* avPgwEgtInMBReq */



/*
 *
 *       Fun:   Sending Local Tunnel Management request
 *
 *       Desc:  Invoked by the application 
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwSndGtpuTnlMgmtReq
(
AvPgwUeTunnelCb *tunnelCb,
U8               action           /* Action to be taken on tunnel */
)
#else
PRIVATE S16 avPgwSndGtpuTnlMgmtReq(tunnelCb, action)
AvPgwUeTunnelCb *tunnelCb;
U8               action;           /* Action to be taken on tunnel */
#endif
{
   EgtUEvnt    *tnlMgmtEvnt = NULLP;
   S16         ret = ROK;

   AV_PGW_SND_TRC2(avPgwSndGtpuTnlMgmtReq);

   ret = avPgwUtlBldEgtTnlMgmtReq(tunnelCb, &tnlMgmtEvnt, action);
   if (ROK != ret)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Cannot build EGTP-U Event structure for action(%d), \
                      tunnel(%lx)\n", action, (long int)tunnelCb));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Cannot build EGTP-U Event structure for action(%d), \
                      tunnel(%x)\n", action, (unsigned int)tunnelCb));
#endif
      AV_PGW_SND_RETVALUE(ret);
   }
   else
   {
      avPgwSetRouteCbTunState(tunnelCb, AV_PGW_EGTPU_TUNNEL_PENDING);
   }
    
   /* Send it to GTP-U */
   EuLiEgtEguLclTnlMgmtReq (&(avPgwCb.egtUSapCb->pst),\
         avPgwCb.egtUSapCb->spId, tnlMgmtEvnt);

   AV_PGW_SND_RETVALUE(ret);

}

/******************************************************************************/
/* GTP-U Tunnel management state machine                                      */
/******************************************************************************/
/*
*
*    Fun:    avPgwEgtUTunnelMachine
*
*    Desc:   EGTPU Tunnel State Machine handling important states:
*            AV_PGW_EGTPU_TUNNEL_DOWN
*            AV_PGW_EGTPU_TUNNEL_PENDING,
*            AV_PGW_EGTPU_TUNNEL_UP
*    Note:   
*
*    Ret:    
*
*    Notes:  None
*
*    File:   av.c
*
*/
#ifdef ANSI
PUBLIC S16 avPgwEgtUTunnelMachine
(
AvPgwUeTunnelCb *tunnelCb,
TeId             teId,
U8               action,
EgtUEvnt        *eguEvnt
)
#else
PUBLIC S16 avPgwEgtUTunnelMachine(tunnelCb, teId, action, eguEvnt)
AvPgwUeTunnelCb *tunnelCb;
TeId             teId;
U8               action;
EgtUEvnt        *eguEvnt;
#endif
{
   U8    state;
   S16   ret = ROK;

   AV_PGW_SND_TRC2(avPgwEgtUTunnelMachine)

   /* Right first find the UE Control tunnel on which this message has
      arrived from SAP */
   if(NULLP == tunnelCb)
   {
      ret = avPgwFndUeTunnelCb(&tunnelCb, teId, AV_PGW_DATA_TUNNEL);
   }

   if(NULLP != tunnelCb)
   {
      state = tunnelCb->state;
   }
   else
   {
      /* No tunnel is present yet; Asssuimng state is down, let the 
         handler act on it */
      state = AV_PGW_EGTPU_TUNNEL_DOWN;
   }

   switch(state)
   {
      case AV_PGW_EGTPU_TUNNEL_DOWN:
      {
         ret = avPgwEgtUTunnelDown(tunnelCb, teId, action, eguEvnt);
         break;
      }   
      case AV_PGW_EGTPU_TUNNEL_PENDING:
      {
         ret = avPgwEgtUTunnelPending(tunnelCb, teId, action, eguEvnt);
         break;
      }
      case AV_PGW_EGTPU_TUNNEL_UP:
      {
         ret = avPgwEgtUTunnelUp(tunnelCb, teId, action, eguEvnt);
         break;
      }
      default:
      {
         /* Debug print here - unrecognized event */
         ret = RFAILED;
         break;
      }
   }

   /* Not required to free the event structure here, as these are related
      EGTP-U tunnel management primitives only and not the data messages */

   AV_PGW_SND_RETVALUE(ret);

}


/*
*
*    Fun:    avPgwEgtUTunnelDown
*
*    Desc:   This function handles requests for EGTPU Tunnel in down state.
*
*    Ret:    
*
*    Notes:  None
*
*    File:  av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwEgtUTunnelDown
(
AvPgwUeTunnelCb *tunnelCb,
TeId             teId,
U8               action,
EgtUEvnt        *eguEvnt
)
#else
PRIVATE S16 avPgwEgtUTunnelDown(tunnelCb, teId, action, eguEvnt)
AvPgwUeTunnelCb *tunnelCb;
TeId             teId;
U8               action;
EgtUEvnt        *eguEvnt;
#endif
{
   S16   ret;

   AV_PGW_SND_TRC2(avPgwEgtUTunnelDown)

   switch(action)
   {
      case EGT_TMGMT_REQTYPE_ADD: /* Add Tunnel Request */
      {
         /* This shouldnt happen, as there is no explicit primitive 
            from EGTP-U which can trigger creating EGTP-U Tunnel. 
            As an application we should have created the tunnel before this
            However as a safe cover adding this code */
         if(NULLP == tunnelCb)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                            "Could not find the TunnelCb"));
            AV_PGW_SND_RETVALUE(RFAILED);
         }
         tunnelCb->state = AV_PGW_EGTPU_TUNNEL_PENDING;
         /*Added Klockworks Check : Replacing %d with %ld */
         /* Debug print here */
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Adding EGTP-U - Local tunnel: tunnel Id(%d)\n", 
                (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Adding EGTP-U - Local tunnel: tunnel Id(%ld)\n", 
                (tunnelCb->localTeIdKey))); 
#endif

         /* Right - Send a request to GTP-U to add a tunnel */
         ret = avPgwSndGtpuTnlMgmtReq(tunnelCb, EGT_TMGMT_REQTYPE_ADD);
         break; 
      } 
      default:
      {
        /* Should we check for sending reject, we cannot recognize the event*/
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                         "avPgwEgtUTunnelDown: Unhandled event"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }  
   }
   AV_PGW_SND_RETVALUE(ret);
}



/*
*
*    Fun:    avPgwEgtUTunnelPending
*
*    Desc:   EGTP-U Tunnel Pending State Handler.
*
*    Ret:   
*
*    Notes:  
*
*    Notes:  None
*
*    File:   av.c
*
*/


#ifdef ANSI
PRIVATE S16 avPgwEgtUTunnelPending
(
AvPgwUeTunnelCb *tunnelCb,
TeId             teId,
U8               action,
EgtUEvnt        *eguEvnt
)
#else
PRIVATE S16 avPgwEgtUTunnelPending(tunnelCb, teId, action, eguEvnt)
AvPgwUeTunnelCb *tunnelCb;
TeId             teId;
U8               action;
EgtUEvnt        *eguEvnt;
#endif
{
   AV_PGW_SND_TRC2(avPgwEgtUTunnelPending)

      if(NULLP == eguEvnt)
      {
         /* Should we check for sending reject, we cannot recognize the event*/
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                  "avPgwEgtUTunnelPending: event is NULL"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }  


   switch(action)
   {
      case EGT_TMGMT_REQTYPE_ADD: 
      {
         /* Add Tunnel Request -we received confirmation */

         /*Added Klockworks Check: Replacing %d with %ld*/
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Local creation of "
                           "EGTP-U tunnel successful: tunnel Id(%d)\n",
                           (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Local creation of "
                           "EGTP-U tunnel successful: tunnel Id(%ld)\n",
                           (tunnelCb->localTeIdKey))); 
#endif
         /* Debug print here */
         AV_PGW_SND_RETVALUE(avPgwEgtUHdlAddTunnelReqCfm(tunnelCb, eguEvnt));
      } 

      case EGT_TMGMT_REQTYPE_DEL:
      {
         /* Delete Tunnel Request - we received confirmation*/
         /*Added Klockworks Check : Replacing %d with %ld*/
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Local deletion of "
                           "EGTP-U tunnel successful: tunnel Id(%d)\n", 
                           (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Local deletion of "
                           "EGTP-U tunnel successful: tunnel Id(%ld)\n", 
                           (tunnelCb->localTeIdKey))); 
#endif
         /* Debug print here */
         AV_PGW_SND_RETVALUE(avPgwEgtUHdlDelTunnelReqCfm(tunnelCb, eguEvnt));
      }

      case EGT_TMGMT_REQTYPE_MOD:
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Tunnel "\
               "Modification confirmation for EGTP-U tunnel: Tunnel Id(%d)\n",\
               (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PDN-GW: Tunnel "\
               "Modification confirmation for EGTP-U tunnel: Tunnel Id(%ld)\n",\
               (tunnelCb->localTeIdKey))); 
#endif

         AV_PGW_SND_RETVALUE(avPgwEgtUHdlModTunnelReqCfm(tunnelCb, eguEvnt));
         break;
      }

      default:
      {
         /* Should we check for sending reject, we cannot recognize the event*/
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                            "avPgwEgtUTunnelPending: Unhandled event"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }  
   }
}


/*
*
*    Fun:    avPgwEgtUTunnelUp
*
*    Desc:   EGTP-U Tunnel UP State Handler.
*
*    Ret:    
*
*    Notes:  It handles deleting the tunnel, ignores rest of the event.
*
*    File:   av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtUTunnelUp
(
AvPgwUeTunnelCb *tunnelCb,
TeId             teId,
U8               action,
EgtUEvnt        *eguEvnt
)
#else
PRIVATE S16 avPgwEgtUTunnelUp(tunnelCb, teId, action, eguEvnt)
AvPgwUeTunnelCb *tunnelCb;
TeId             teId;
U8               action;
EgtUEvnt        *eguEvnt;
#endif
{
   S16 ret;

   AV_PGW_SND_TRC2(avPgwEgtUTunnelUp)

   switch(action)
   {
      case EGT_TMGMT_REQTYPE_DEL: /* Delete Tunnel Request */
      {
         tunnelCb->state = AV_PGW_EGTPU_TUNNEL_PENDING;

         /* Debug print here */
         /*Added Klockworks Check : Replacing %d with %ld*/
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Deleting EGTP-U - "
                           "Local tunnel: tunnel Id(%d)\n", 
                           (tunnelCb->localTeIdKey))); 
#else
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Deleting EGTP-U - "
                           "Local tunnel: tunnel Id(%ld)\n", 
                           (tunnelCb->localTeIdKey))); 
#endif

         /* Right - Send a request to GTP-U to delete this tunnel */
         ret = avPgwSndGtpuTnlMgmtReq(tunnelCb, EGT_TMGMT_REQTYPE_DEL);
         break; 
      } 
      default:
      {
        /* Should we check for sending reject, we cannot recognize the event*/
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                            "avPgwEgtUTunnelUp: Unhandled event"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }  
   }

   AV_PGW_SND_RETVALUE(ret);
}

/*
 *
 *       Fun:   avPgwEgtUHdlAddTunnelReqCfm
 *
 *       Desc:  This function handles GTPU Tunnel addition request confirmation
 *              from EGTP.
 *
 *       Ret:   ROK  / RFAILED 
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */


#ifdef ANSI
PRIVATE S16 avPgwEgtUHdlAddTunnelReqCfm
(
AvPgwUeTunnelCb *tunnelCb,
EgtUEvnt        *eguEvnt
)
#else
PRIVATE S16 avPgwEgtUHdlAddTunnelReqCfm(tunnelCb, eguEvnt)
AvPgwUeTunnelCb *tunnelCb;
EgtUEvnt        *eguEvnt;
#endif
{
   S16                  ret = ROK;
   AvPgwUeTunnelCb      *s5s8CTun = NULLP;
   U32                  bearerInd = 0;
   
   AV_PGW_SND_TRC2(avPgwEgtUHdlAddTunnelReqCfm);

   bearerInd = AV_PGW_BEARER_IND(tunnelCb->bearerId);

   if((bearerInd >= 0) && (bearerInd < AV_PGW_MAX_BEARERS))
   {   
      s5s8CTun = tunnelCb->ueCb->bearerTable[bearerInd]->pdnCb->s5s8CTun;
   }
   else
   {
      ret = RFAILED;
      AV_PGW_SND_RETVALUE(ret);
   }   
   if(NULLP != s5s8CTun)
   {
      if(LCM_PRIM_OK == eguEvnt->u.tnlMgmt.cfmStatus.status) /* Success */
      {
         tunnelCb->state = AV_PGW_EGTPU_TUNNEL_UP;
         tunnelCb->pdnCb->numUTun += 1;

         avPgwSetRouteCbTunState(tunnelCb, AV_PGW_EGTPU_TUNNEL_UP);
         avPgwUpdtPdnCbtoIpCb(tunnelCb->pdnCb);
         /* Let us invoke EGTPC Control tunnel machine successfully added 
            the data tunnel */
         /* av001.101: added support for dedicated tunnels */
         if(tunnelCb->tunnelType == AV_PGW_DATA_TUNNEL_DFLT)
         {
            ret = avPgwEgtCTunnelMachine(s5s8CTun,
                                      &(s5s8CTun->lclAddr),
                                      &(s5s8CTun->remAddr), 
                                      s5s8CTun->localTeIdKey,
                                      AV_PGW_EGTPC_MSG_CS_REQ_ACC,
                                      NULLP);
         }
         else
         {
            ret = avPgwEgtCTunnelMachine(s5s8CTun,
                                      &(s5s8CTun->lclAddr),
                                      &(s5s8CTun->remAddr), 
                                      s5s8CTun->localTeIdKey,
                                      AV_PGW_EGTPC_MSG_BR_RES_ACC,
                                      NULLP);

         }
      } 
      else
      {
         /* Let us invoke EGTPC Control tunnel machine - couldnt add 
            the data tunnel */
         ret = avPgwEgtCTunnelMachine(s5s8CTun, 
                                      &(s5s8CTun->lclAddr),
                                      &(s5s8CTun->remAddr), 
                                      s5s8CTun->localTeIdKey,
                                      AV_PGW_EGTPC_MSG_CS_REQ_REJ,
                                      NULLP);
      }
   }
   else 
   {
      /* No control tunnel at all - s
       * omehow we have landed to this condition */
      ret = RFAILED;
   }

   AV_PGW_SND_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Handling of tunnel modifcation request confirmation received from
 *    EGT layer. Sends out the Modify Bearer Response to SGW if the
 *    Message if present
 *             
 * @param[in]  pUTun
 *    Pointer to Data Plane tunnel Control block
 * @param[in]  pEguEvnt
 *    Pointer to EGU Event received from lower EGT layer
 * 
 * @return
 *    SUCCESS if handling is successful otherwise its FAILURE
 *
 *****************************************************************************/
PRIVATE S16 avPgwEgtUHdlModTunnelReqCfm
(
AvPgwUeTunnelCb   *pUTun,
EgtUEvnt          *pEguEvnt
)
{
   AvPgwUeTunnelCb   *pCTun = NULLP;
   S16               ret    = ROK;

   AV_PGW_SND_TRC2(avPgwEgtUHdlModTunnelReqCfm)

   pCTun = pUTun->pdnCb->s5s8CTun;
   if (NULLP != pCTun) 
   {
      if (LCM_PRIM_OK == pEguEvnt->u.tnlMgmt.cfmStatus.status)
      {
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Modifying UE Route CB"));
         avPgwModUeRouteCb(pUTun);
         avPgwSetRouteCbTunState(pUTun, AV_PGW_EGTPU_TUNNEL_UP);
         pUTun->state = AV_PGW_EGTPU_TUNNEL_UP;

         ret = avPgwEgtCTunnelMachine(pCTun, &pCTun->lclAddr, &pCTun->remAddr,\
               pCTun->localTeIdKey, pCTun->pdnCb->cEvent, pUTun->pEgMsg);
      }
      else
      {
         ret = avPgwEgtCTunnelMachine(pCTun, &pCTun->lclAddr, &pCTun->remAddr,\
               pCTun->localTeIdKey, AV_PGW_EGTPC_MSG_MB_REQ_REJ, NULLP);
      }
   }
   else 
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Control Plane tunnel NOT"\
            " Found"));
      ret = RFAILED;
   }

   AV_PGW_SND_RETVALUE(ret);
}


/*
 *
 *       Fun:   avPgwEgtUHdlDelTunnelReqCfm
 *
 *       Desc:  This function handles GTPU Tunnel deletion request confirmation
 *              from EGTP.
 *
 *       Ret:   ROK  / RFAILED 
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */

#ifdef ANSI
PRIVATE S16 avPgwEgtUHdlDelTunnelReqCfm
(
AvPgwUeTunnelCb *tunnelCb,
EgtUEvnt        *eguEvnt
)
#else
PRIVATE S16 avPgwEgtUHdlDelTunnelReqCfm(tunnelCb, eguEvnt)
AvPgwUeTunnelCb *tunnelCb;
EgtUEvnt        *eguEvnt;
#endif
{
   AvPgwUeTunnelCb   *ctrlTunnelCb = NULLP;
   S16               ret;

   AV_PGW_SND_TRC2(avPgwEgtUHdlDelTunnelReqCfm)

   /* Irrespective of the results from GTP-U interface primitive
    *  we should delete the tunnel - as we created it */
   ctrlTunnelCb = tunnelCb->pdnCb->s5s8CTun;

   if (ctrlTunnelCb == NULLP)
   {
      AV_PGW_SND_RETVALUE(RFAILED);
   }


   if (tunnelCb->tunnelType == AV_PGW_DATA_TUNNEL_DED ||
       tunnelCb->tunnelType == AV_PGW_DATA_TUNNEL_DFLT)
   {
      /* Locally delete this tunnel now */
      ret = avPgwDelUeTunnelCb(tunnelCb->ueCb, &tunnelCb, tunnelCb->tunnelType);
      if(ROK != ret)
      {
          AV_PGW_SND_RETVALUE(ret);
      }
   }

   /* av001.101: checking both */
   if (ctrlTunnelCb->pdnCb->numUTun > 0)
   {
      /* Still pending to get the confirmation of the associated 
       *  data tunnels */
      AV_PGW_SND_RETVALUE(ROK);
   }

   /* Inform the control state machine about this event based on
      success /failure */
   if (NULLP != ctrlTunnelCb) 
   {
      if(LCM_PRIM_OK == eguEvnt->u.tnlMgmt.cfmStatus.status)
      {
         /* Let us invoke EGTPC Control tunnel machine - deleted the data
            tunnel */
         ret = avPgwEgtCTunnelMachine(ctrlTunnelCb, 
                                      &ctrlTunnelCb->lclAddr,
                                      &ctrlTunnelCb->remAddr, 
                                      ctrlTunnelCb->localTeIdKey,
                                      ctrlTunnelCb->pdnCb->cEvent,
                                      NULLP);
      }
      else
      {
         /* Let us invoke EGTPC Control tunnel machine - couldnt delete the data 
            tunnel */
          ret = avPgwEgtCTunnelMachine(ctrlTunnelCb,
                                       &ctrlTunnelCb->lclAddr,
                                       &ctrlTunnelCb->remAddr, 
                                       ctrlTunnelCb->localTeIdKey,
                                       AV_PGW_EGTPC_MSG_DS_REQ_REJ,
                                       NULLP);
      }
   }
   else 
   {
      /* No control tunnel at all - somehow we have landed to this 
       * condition */
      ret = RFAILED;
   }

   AV_PGW_SND_RETVALUE(ret);
}


/*****************************************************/
/* All Helper Utilities                              */
/*****************************************************/

/*
 *
 *       Fun:   avPgwFndUeTunnelCb 
 *
 *       Desc:  This functions finds a tunnel control block in hash list.
 *
 *       Ret:   ROK   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwFndUeTunnelCb
(
AvPgwUeTunnelCb **tunnel,
U32               teId,
AvPgwTunnelType   tunnelType
)
#else
PUBLIC S16 avPgwFndUeTunnelCb(tunnel, teId, tunnelType)
AvPgwUeTunnelCb **tunnel;
U32               teId;
AvPgwTunnelType   tunnelType;
#endif
{
   S16  ret;

   AV_PGW_SND_TRC2(avPgwFndUeTunnelCb)

   switch(tunnelType)
   {
      case AV_PGW_CTRL_TUNNEL:
      {
         ret = cmHashListFind(&(avPgwCb.egtCSapCb->ueTunnelCbHlCp), 
                              (U8 *)&teId, (U16)sizeof(teId),
                               (U16)0, (PTR *)tunnel);

         if(ROK != ret)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                 "avPgwFndUeTunnelCb: Failed to get control tunnelCb"));
            *tunnel = NULLP;
         }
         break;
      }
      case AV_PGW_DATA_TUNNEL:
      {
         ret = cmHashListFind(&(avPgwCb.egtUSapCb->ueTunnelCbHlCp),
                              (U8 *)&teId, (U16)sizeof(teId),
                               (U16)0, (PTR *)tunnel);

         if(ROK != ret)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                 "avPgwFndUeTunnelCb: Failed to get data tunnelCb"));
            *tunnel = NULLP;
         }
         break;
      }
      default:
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                 "avPgwFndUeTunnelCb: Unrecognized tunnel type"));
         *tunnel = NULLP;
         ret = RFAILED;
      }
   }

   AV_PGW_SND_RETVALUE(ret);

} /* end of avPgwFndUeTunnelCb */




/*
 *
 *       Fun:   avPgwUtlBldEgtDatReq
 *
 *       Desc:  Build the EGTP-U Data Request Message
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: 
 *
 *       File: av.c
 *
 */

#ifdef ANSI
PUBLIC S16 avPgwUtlBldEgtDatReq
(
AvPgwTunInfo      *tunInfo,
EgtUEvnt          **eguEvt
)
#else
PUBLIC S16 avPgwUtlBldEgtDatReq(tunInfo, eguEvt)
AvPgwTunInfo      *tunInfo;
EgtUEvnt          **eguEvt;
#endif
{
   CmTptAddr *tptAddr;
   Mem      smem;
   S16      ret;

   AV_PGW_RCV_TRC2(avPgwUtlBldEgtDatReq)

   AV_PDNGW_RECVTSK_ALLOC((*eguEvt), sizeof(EgtUEvnt));
   if(NULLP == *eguEvt)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Memory allocation for \
                       EGTP-U Event failed \n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }
   smem.region  = avPgwCb.routeCb.init.region;
   smem.pool = avPgwCb.routeCb.init.pool;
   /**************************************************************
    * Allocate memory for the e-gtpu Data message of size EgUMsg *
    **************************************************************/
   if((ret = avPgwUtlAllocEguMsg(&((*eguEvt)->u.egMsg), &smem)) != ROK )
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                       "avPgwUtlBldEgtDatReq(): Memory allocation for \
                       EGTP-U message failed \n"));
      AV_PDNGW_RECVTSK_FREE((*eguEvt), sizeof(EgtUEvnt));
      AV_PGW_RCV_RETVALUE(ret);
   }

   /* Fill Local Address */
   tptAddr = &((*eguEvt)->u.egMsg->srcAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tunInfo->locAddr.port;
   tptAddr->u.ipv4TptAddr.address = tunInfo->locAddr.address;

   /* Fill Remote Address */
   tptAddr = &((*eguEvt)->u.egMsg->remAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tunInfo->remAddr.port;
   tptAddr->u.ipv4TptAddr.address = tunInfo->remAddr.address;

   avPgwUtlBuildEguHeader(tunInfo->remTeid, &(((*eguEvt)->u.egMsg)->msgHdr),
                          EGT_GTPU_MSG_GPDU);

   /* No need to construct the message, as we are using buffer from external
      network to assign to mBuf */
   AV_PGW_RCV_RETVALUE(ret);

} /* avPgwUtlBldEgtDatReq */

/*
 *
 *       Fun:   avPgwUtlBldEgtTnlMgmtReq
 *
 *       Desc:  Build the EGTP-U Tunnel Mgmt Request Message
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: 
 *
 *       File: av.c
 *
 */

#ifdef ANSI
PRIVATE S16 avPgwUtlBldEgtTnlMgmtReq
(
AvPgwUeTunnelCb *tunnel,
EgtUEvnt       **eguEvt,
U8               action           /* Action to be taken on tunnel */
)
#else
PRIVATE S16 avPgwUtlBldEgtTnlMgmtReq(tunnel, eguEvt, action)
AvPgwUeTunnelCb *tunnel;
EgtUEvnt       **eguEvt;
U8               action;           /* Action to be taken on tunnel */
#endif
{
   CmTptAddr  *tptAddr;
   S16        ret = ROK; 

   AV_PGW_SND_TRC2(avPgwUtlBldEgtTnlMgmtReq)

   AV_PDNGW_SENDTSK_ALLOC((*eguEvt), sizeof(EgtUEvnt));
   if(NULLP == *eguEvt)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Memory allocation \
                            failed for local tunnel mangement request"));

      AV_PGW_SND_RETVALUE(RFAILED);   
   }

   switch(action)
   {
      case EGT_TMGMT_REQTYPE_ADD: /* Add Tunnel */
      {
         /* Re-ordering Algorithm */
         (*eguEvt)->u.tnlMgmt.reOrderPres = AV_PGW_EGT_REORDER_TYPE_1;

         /* Expected sequence number */
         (*eguEvt)->u.tnlMgmt.expSeqNmb = AV_PGW_DFLT_EXPSEQNMB;

         /* MAX T-PDU size */
         (*eguEvt)->u.tnlMgmt.maxTPduSize = AV_PGW_DFLT_MAXTPDUSZ;
         
         break;
      }   
      case EGT_TMGMT_REQTYPE_DEL: /* Delete Tunnel */
      {
         break;
      }
      default: /* Right now we are not handling any other action */
      {
         AV_PGW_SND_RETVALUE(RFAILED); 
      }
   }

   /* Interface type */
   (*eguEvt)->u.tnlMgmt.intfType = EGT_GTP_INT_S5_S8;
   (*eguEvt)->u.tnlMgmt.action = action;  /* Create / Modify / Delete */
   (*eguEvt)->u.tnlMgmt.teId = tunnel->localTeIdKey;
   (*eguEvt)->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;

   /* Fill Local Address */
   tptAddr = &((*eguEvt)->u.tnlMgmt.srcAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tunnel->lclAddr.u.ipv4TptAddr.port;
   tptAddr->u.ipv4TptAddr.address = tunnel->lclAddr.u.ipv4TptAddr.address;

   /* Fill Remote Address */
   tptAddr = &((*eguEvt)->u.tnlMgmt.dstIpAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tunnel->remAddr.u.ipv4TptAddr.port;
   tptAddr->u.ipv4TptAddr.address = tunnel->remAddr.u.ipv4TptAddr.address;

   /* Fill From Remote Address */
   tptAddr = &((*eguEvt)->u.tnlMgmt.frmDstIpAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tunnel->remAddr.u.ipv4TptAddr.port;
   tptAddr->u.ipv4TptAddr.address = tunnel->remAddr.u.ipv4TptAddr.address;

   AV_PGW_SND_RETVALUE(ret);

} /* avPgwUtlBldEgtTnlMgmtReq */



/*****************************************************************************
 *
 *    Fun:    avPgwUtlAllocEguMsg
 *
 *    Desc:   Allocate memory for the GTP-U message and initialize
 *
 *    Ret:    ROK  - if memory allocation and initialization is success
 *            RFAILED - in case of memory allocation fails
 *
 *    Notes:  None
 *
 *    File:   av.c
 *
 ****************************************************************************/
#ifdef ANSI
PUBLIC S16 avPgwUtlAllocEguMsg
(
EgUMsg **egMsg,    /* GTP-U Message */
Mem    *mem       /* memory information */
)
#else
PUBLIC S16 avPgwUtlAllocEguMsg(egMsg, mem)
EgUMsg **egMsg;    /* GTP-U Message */
Mem    *mem;       /* memory information */
#endif
{

   AV_PGW_SND_TRC2(avPgwUtlAllocEguMsg)

   /******************************************************
    * Allocate memory for the GTP-U message of size EgUMsg *
    ******************************************************/
   if (cmAllocEvnt(sizeof(EgUMsg), EG_UTIL_MEM_SIZE, mem, (Ptr* )egMsg) != ROK)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
        "Allocating memory for the GTP-U Message failed.\n"));

      AV_PGW_SND_RETVALUE(RFAILED);
   }

   AV_PGW_SND_RETVALUE(ROK);
}

/****************************************************************************
 *
 *    Fun:    avPgwUtlDeAllocEguMsg
 *
 *    Desc:   De-Allocate memory for the GTP-U message and uninitialize
 *
 *    Ret:    ROK  - if memory de-allocation and initialization is success
 *            RFAILED - in case of memory de-allocation fails
 *
 *    Notes:  The GTP-U message will be destoryed in this method.  The
 *            memory allocated either for the message or for all the IEs
 *            inside the message (of any level) will be deallocated inside
 *            this method.
 *
 *    File:   av.c
 *
 ***************************************************************************/
#ifdef ANSI
PUBLIC S16 avPgwUtlDeAllocEguMsg
(
EgUMsg **egMsg     /* GTP-U Message */
)
#else
PUBLIC S16 avPgwUtlDeAllocEguMsg(egMsg)
EgUMsg **egMsg;    /* GTP-U Message */
#endif
{
   EgUIe   *ie = NULLP;    /* temporary ie to hold the element in the list */
   CmLList *node = NULLP;  /* temporary node to walk through the list */

   AV_PGW_SND_TRC2(avPgwUtlDeAllocEguMsg)

   if((*egMsg)->msgHdr.msgType != EGT_GTPU_MSG_GPDU)
   {
      /***************************************************************
       * In a loop iterate through the list and then delete the node * 
       ***************************************************************/
      for (ie = (EgUIe* )CM_LLIST_FIRST_NODE(&(*egMsg)->u.egIeList, node); 
            ie != NULLP;
            ie = (EgUIe* )CM_LLIST_NEXT_NODE(&(*egMsg)->u.egIeList, node))
      {
         /*********************************
          * delete the node from the list *
          *********************************/
         if (!cmLListDelFrm(&(*egMsg)->u.egIeList, node))
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                  "avPgwUtlDeAllocEguMsg: Deleting IE has failed....\
                  continuing with dropping the message.\n"));
            break;
         }
      }
   }
   else
   {
      if((*egMsg)->u.mBuf)
      {
         SPutMsg((*egMsg)->u.mBuf);
      }
   }

   /*****************************************
    * De-allocate memory of the gtp message *
    *****************************************/
   cmFreeMem(&(*egMsg)->memCp);

   /************************************
    * Initialize the pointer with zero *
    ************************************/
   *egMsg = NULLP;

   AV_PGW_SND_RETVALUE(ROK);
}



/*
*
*       Fun:   avPgwUtlBuildEguHeader
*
*       Desc:  This function is used to build EGTP U Header
*
*       Ret:   None
*
*       Notes: None
*
*       File:  av.c
*
*/

#ifdef ANSI
PUBLIC Void  avPgwUtlBuildEguHeader
(
TeId              remTeid,
EgUMsgHdr         *eguHdr,
U8                msgType
)
#else
PUBLIC Void avPgwUtlBuildEguHeader(tunnel, eguHdr, msgType)
TeId              remTeid;
EgUMsgHdr         *eguHdr;
U8                msgType;
#endif /* ANSI */
{
   AV_PGW_SND_TRC2(avPgwUtlBuildEguHeader)
#ifdef INCLUDE_EXT_HDR
   eguHdr->nPduNmb.pres = TRUE;
   eguHdr->nPduNmb.val = AV_PGW_EGU_DFLT_PDU_NMB;

   eguHdr->seqNumber.pres = TRUE;
   eguHdr->seqNumber.val = AV_PGW_EGU_DFLT_SEQ_NMB;

   /* Not filling extension headers */
   eguHdr->extHdr.udpPort.pres = FALSE;
   eguHdr->extHdr.pdcpNmb.pres = FALSE;
#endif

   eguHdr->teId = remTeid;
   eguHdr->msgType = msgType;

   AV_PGW_SND_RETVOID;
}




/*
*
*       Fun:   avPgwEgtInDBCmd
*
*       Desc:  This function is used to handle Delete bearer Command
*              Received from SGW
*
*       Ret:   None
*
*       Notes: None
*
*       File:  av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtInDBCmd
(
AvPgwUeTunnelCb   *tunnelCb,
CmTptAddr         *locAddr,
EgMsg             *egMsg
)
#else
PRIVATE S16 avPgwEgtInDBCmd(tunnelCb, locAddr, egMsg)
AvPgwUeTunnelCb   *tunnelCb;
CmTptAddr         *locAddr;
EgMsg             *egMsg;
#endif
{
   TknU32      localTeId = {0};
   S16         ret = ROK;
   AvPgwUeCb   *ueCb = NULLP;
   AvPgwPdnCb  *pdnCb = NULLP;
   EgMsg       *dbReq = NULLP;
   U32         cause = 0;
   TknU8       create;

   AV_PGW_SND_TRC2(avPgwEgtInDBCmd);

   ueCb = tunnelCb->ueCb;
   pdnCb = tunnelCb->pdnCb;
   pdnCb->seqNum = egMsg->msgHdr.seqNumber;
   pdnCb->transId.pres = PRSNT_NODEF;
   pdnCb->transId.val = egMsg->msgHdr.seqNumber;

   ret = avPgwValDBCmd(ueCb, pdnCb, egMsg, &cause);
   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                           "Validation of Delete Bearer Command"));
   }

   /* Send the Bearer Create Request to the SGW */
   localTeId.pres = TRUE;
   localTeId.val = tunnelCb->localTeIdKey;

   if (ROK == ret)
   {
      ret = avPgwBldDBReq(pdnCb, &dbReq);
      if (ROK == ret)
      {
         create.pres = PRSNT_NODEF;
         create.val = FALSE;
         t3.enb = TRUE;
         t3.val = AV_PGW_EGT_T3_RESP_TMR;

         EuLiEgtSigReq(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                        &(tunnelCb->lclAddr), &(tunnelCb->remAddr), localTeId,
                        pdnCb->transId, create, t3, dbReq);

         tunnelCb->state = AV_PGW_EGTPC_TUNNEL_PROC;
         tunnelCb->pdnCb->expectedMsg = EGT_GTPC_MSG_DB_RSP;
      }
      else
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                              "Building of Delete Bearer Request"));
      }
   }

   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Sending Delete Bearer "
                        "Failure Indication to SGW"));

      avPgwEgtSndFailureRsp(pdnCb, tunnelCb->localTeIdKey, locAddr,
                           &(pdnCb->s5s8CTun->remAddr),
                           EGT_GTPC_MSG_DB_FAIL_IN, pdnCb->transId, cause);
   }
   
   AV_PGW_SND_RETVALUE(ret);
}




/*
*
*       Fun:   avPgwEgtInDBRsp
*
*       Desc:  This function is used to handle Delete bearer Response 
*              Received from SGW
*
*       Ret:   None
*
*       Notes: None
*
*       File:  av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtInDBRsp
(
AvPgwUeTunnelCb   *tunnelCb,  /* Local C-Tunnel CB */
EgMsg             *egMsg      /* gtp-c message  */
)
#else
PRIVATE S16 avPgwEgtInDBRsp(tunnelCb, egMsg)
AvPgwUeTunnelCb   *tunnelCb;  /* Local C-Tunnel CB */
EgMsg             *egMsg;     /* gtp-c message  */
#endif
{
   AvPgwBearerCb     **bearerTable = NULLP;
   AvPgwUeTunnelCb   *uTun = NULLP;
   AvPgwUeTunnelCb   *defTun = NULLP;
   U16               ret = ROK;
   AvPgwUeCb         *ueCb = NULLP;
   AvPgwPdnCb        *pdnCb = NULLP;
   CmLList           *tmp = NULLP;
   U32               bIndex = 0;

   AV_PGW_SND_TRC2(avPgwEgtInDBRsp)

   ueCb = tunnelCb->ueCb;
   pdnCb = tunnelCb->pdnCb;
   bearerTable = ueCb->bearerTable;

   ret = avPgwEgtValDBRsp(ueCb, pdnCb, egMsg);
   if (ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Validation of Delete Bearer "
                           "Response Failed"));
      tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;
      avPgwUtlDelPendingTunLst(&(pdnCb->pendingTunLst));
      RETVALUE(ret);
   }

   CM_LLIST_FIRST_NODE(&(pdnCb->pendingTunLst), tmp);
   while (tmp)
   {
      uTun = ((AvPgwPendingTun *)tmp)->uTun;

      if (uTun->tunnelType == AV_PGW_DATA_TUNNEL_DFLT)
      {
         /* First delete all dedicated tunnels of this PDN and then delete
          * the default one
          */
         defTun = uTun;
         break;
      }
      else
      {
         bIndex = AV_PGW_BEARER_IND(uTun->bearerId);
         ret = avPgwDelBearerCb(bearerTable[bIndex]);
         if (ROK != ret)
         {
#ifdef ALIGN_64BIT
            AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                              "Deleting of Dedicated Data tunnel "
                              "(%u), FAILED", (uTun->localTeIdKey)));
#else
            AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                              "Deleting of Dedicated Data tunnel "
                              "(%lu), FAILED", (uTun->localTeIdKey)));
#endif
         }
      }

      CM_LLIST_NEXT_NODE(&(pdnCb->pendingTunLst), tmp);
   }

   /* dedicated tunnels are deleted. so change the state of C-Tun from
    * PROC to UP */
   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_UP;

   if (defTun != NULLP)
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Deleting the Default Data "
                           "tunnel"));

      /* Since the default U tunnel tunnel is getting deleted, set 
       * the status of C-tunnel to pending */
      pdnCb->cEvent = AV_PGW_EGTPC_MSG_DB_CMD_ACC;

      /* Delete the default U tun */
      ret = avPgwDelPdnCb(pdnCb);
      if (ROK != ret)
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Deleting of PDN"));
      }
   }

   AV_PGW_SND_RETVALUE(ret);
}

/*
*
*    Fun:    avPgwEgtDBCmdAcc
*
*    Desc:   This function handles successful local deletion of EGTP-C tunnel
*            confrimation status from EGTP.
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtDBCmdAcc
(
AvPgwUeTunnelCb  *tunnelCb
)
#else
PRIVATE S16 avPgwEgtDBCmdAcc(tunnelCb)
AvPgwUeTunnelCb  *tunnelCb;
#endif
{
   S16             ret = ROK;

   AV_PGW_SND_TRC2(avPgwEgtDBCmdAcc)

   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_DOWN;

   /* Need to check for transId */
   ret = EuLiEgtLclDelReq(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                        0, tunnelCb->localTeIdKey);

   AV_PGW_SND_RETVALUE(ret);
}

/*
*
*    Fun:    avPgwEgtCHdlLocalTunnelDel
*
*    Desc:   This function handles successful local deletion of EGTP-C tunnel
*            confrimation status from EGTP.
*
*    Ret:    
*
*    Notes:  
*
*    File:   av.c
*
*/
#ifdef ANSI
PRIVATE S16 avPgwEgtCHdlLocalTunnelDel
(
AvPgwUeTunnelCb  *tunnelCb
)
#else
PRIVATE S16 avPgwEgtCHdlLocalTunnelDel(tunnelCb)
AvPgwUeTunnelCb  *tunnelCb;
#endif
{
   S16             ret = ROK;

   AV_PGW_SND_TRC2(avPgwEgtCHdlLocalTunnelDel)

   tunnelCb->state = AV_PGW_EGTPC_TUNNEL_DOWN;

   /* Need to check for transId */
   ret = EuLiEgtLclDelReq(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
                        0, tunnelCb->localTeIdKey);

   AV_PGW_SND_RETVALUE(ret);
}

/*
 *
 *    Fun:    avPgwProcPendingEvnts
 *
 *    Desc:   Processes the egt message queued up in pending evnt list
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwProcPendingEvnts()
#else
PRIVATE S16 avPgwProcPendingEvnts()
#endif
{
   S16               ret = ROK;
   AvPgwPendingEvnt  *evnt = NULLP;
   AvPgwUeTunnelCb   *cTun = NULLP;
   CmLList           *lnk = NULLP;


   AV_PGW_SND_TRC2(avPgwProcPendingEvnts)

   evnt = (AvPgwPendingEvnt *)CM_LLIST_FIRST_NODE(&(avPgwCb.pendingEvntLst),
                                                lnk);
   if (evnt)
   {
      ret = avPgwFndUeTunnelCb(&cTun, evnt->locTeid, AV_PGW_CTRL_TUNNEL);
      if (ret == ROK)
      {
         /* tunnel has come to UP state, remove the event and process it */
         if (AV_PGW_EGTPC_TUNNEL_UP == cTun->state)
         {
            cmLListDelFrm(&(avPgwCb.pendingEvntLst), &(evnt->lnk));

            ret = avPgwEgtCTunnelMachine(cTun, &cTun->lclAddr, NULLP,
                                       evnt->locTeid, evnt->eventType,
                                       evnt->egMsg);
            if (ROK != ret)
            {
               AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                    "C-Tunnel Machine Processing"));
            }

            AV_PDNGW_SENDTSK_FREE(evnt, sizeof(AvPgwPendingEvnt));
         }
      }
      else
      {
         /* Tunnel is not present so delete the event */
         cmLListDelFrm(&(avPgwCb.pendingEvntLst), &(evnt->lnk));
         AV_PDNGW_SENDTSK_FREE(evnt, sizeof(AvPgwPendingEvnt));
      }
   }

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwProcPendingEvnts */


/*
 *
 *    Fun:    avPgwDelPendingEvnts
 *
 *    Desc:   Deletes the egt message queued up in pending evnt list
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwDelPendingEvnts()
#else
PRIVATE S16 avPgwDelPendingEvnts()
#endif
{
   S16               ret = ROK;
   AvPgwPendingEvnt  *evnt = NULLP;
   CmLList           *lnk = NULLP;


   AV_PGW_SND_TRC2(avPgwDelPendingEvnts)

   evnt = (AvPgwPendingEvnt *)CM_LLIST_FIRST_NODE(&(avPgwCb.pendingEvntLst),
                                                lnk);
   while (evnt)
   {
      /* Tunnel is not present so delete the event */
      cmLListDelFrm(&(avPgwCb.pendingEvntLst), &(evnt->lnk));
      AV_PDNGW_SENDTSK_FREE(evnt, sizeof(AvPgwPendingEvnt));
      
      evnt = (AvPgwPendingEvnt *)CM_LLIST_NEXT_NODE(&(avPgwCb.pendingEvntLst),
                                                   lnk);
   }

   AV_PGW_SND_RETVALUE(ret);
}

PUBLIC S16 avPgwTrigCBReq(U32 ipAddr, U32 ueSrcPort)
{
   AvPgwPdnCb        *pdnCb = NULLP;
   EgMsg             *egMsg = NULLP;
   AvPgwPendingTun   *pendingTun = NULLP;
   U32               ipIdx = 0;
   U32               bCnt = 0;
   TknU8             create;
   TknU32            localTeId = {0};
   AvPgwUeTunnelCb   *s5s8CTun = NULLP;
   AvPgwUeTunnelCb   *s5s8UDedTun = NULLP;
   AvPgwTft          temptft;
   
   AV_PGW_SND_TRC2(avPgwTrigCBReq)

   AV_PGW_IPCB_INDX(ipAddr, ipIdx);
   if (ipIdx < 0 || ipIdx >= avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Can not find destination UE tunnel mapping to the \
                 IP address:%x\n",
                 ipAddr));

      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   if (FALSE == avPgwCb.ipCbsList[ipIdx].nwUeAddr.assigned)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Can not find destination UE tunnel mapping to the \
                 IP address:%x\n",
                 ipAddr));

      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   AvPgwGetPdnCbOnIp(&(avPgwCb.ipCbsList[ipIdx]), &pdnCb);
   if(NULLP == pdnCb)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Can not find PDN CB mapping to the \
                 IP address:%x\n",
                 ipAddr));

      AV_PGW_RCV_RETVALUE(RFAILED);
   }


   AV_PDNGW_SENDTSK_ALLOC((s5s8UDedTun), sizeof(AvPgwUeTunnelCb));
   if(s5s8UDedTun == NULLP)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "s5s8UDedTun Memory allocation failed\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   avPgwGetTempTftParams(&temptft, ueSrcPort);
   avPgwUpdtTftParams(s5s8UDedTun, &temptft);
   avPgwEncTftBuf(s5s8UDedTun->tftBuf, &s5s8UDedTun->tftLen, &temptft);
   avPgwFillQos(s5s8UDedTun);
   pdnCb->seqNum = 100;  /*TODO*/
   pdnCb->pti    = 0;
   avPgwAddPendingTunnelCb(pdnCb, &s5s8UDedTun, &avPgwCb.pgwCfg.pgwAddrCfg.selfAddr,AV_PGW_DATA_TUNNEL_DED);
   s5s8UDedTun->pdnCb = pdnCb;
 
   avPgwEgtBldCBReq(pdnCb, &egMsg, EGT_GTPC_CAUSE_ACC_REQ); 
   
   s5s8CTun = pdnCb->s5s8CTun;
   if(NULLP == s5s8CTun)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "s5s8CTun for IP address:%x\n",
                 ipAddr));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   localTeId.pres = TRUE;
   localTeId.val = s5s8CTun->localTeIdKey;

   create.pres = PRSNT_NODEF;
   create.val = FALSE;
   t3.enb = TRUE;
   t3.val = AV_PGW_EGT_T3_RESP_TMR;

   EuLiEgtSigReq(&avPgwCb.egtCSapCb->pst, avPgwCb.egtCSapCb->spId,
		   &(s5s8CTun->lclAddr), &(s5s8CTun->remAddr), localTeId,
		   pdnCb->transId, create, t3, egMsg);

   RETVALUE(ROK);

}

/**********************************************************************
  
         End of file:     av.c@@/main/1 - Tue Aug 17 18:54:31 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sp               1. LTE-CNE:PGW Initial Release.
/main/1      av001.101  rp               1. Added support for dedicated tunnels
                                         2. Added support for TFT, QOS
/main/1      av002.101  rk               1. Modified function name for
                                            egUtilEgtAddBearerTft, and
                                            egUtilEgtAddPTidIe which are 
                                            already present in egt_utl.c
/main/1      av004.101  pmacharla        1. Multi UE support added and 
                                         2. eGTP 2.1 upgrade under EG_REL_930 flag
*********************************************************************91*/


