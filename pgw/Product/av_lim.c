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

     Name:    Trillium LTE CNE - PDN-GW Lower Interface Module

     Type:    C Include file

     Desc:    This file contains the PDN-GW lower interface module source code

     File:    jg_lim.c

     Sid:      av_lim.c@@/main/1 - Tue Aug 17 18:54:33 2010

     Prg:     sp 
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
#include "cm_dns.h"        /* common DNS library defines */

#include "leg.h"            /* defines and macros for EGTP */
#include "egt.h"            /* defines and macros for EGTP */

#include "eg_util.h"

#include "lav.h"
#include "av.h"
#include "av_err.h"
#ifndef AV_PERF
#include <pcap.h>
#endif

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

#include "eg_util.x"

#include "lav.x"
#include "av.x"

#ifdef AV_PERF
PRIVATE unsigned long int totalSize = 0;
PRIVATE unsigned long int mbps = 0;
PRIVATE U32 megaByte = (1024 * 1024);
PRIVATE struct timespec entryRx;
PRIVATE struct timespec exitRx;
PRIVATE MsgLen msgLen;
#endif


/**************************************************************************/
/* eGTP-C Primitives - Handling tunnel management primitives              */
/**************************************************************************/

/*
 *
 *       Fun:   New Signalling Request Indication handler
 *
 *       Desc:  Invoked from EGTP layer to user through EGTP Upper Interface
 *              primitive.
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_lim.c
 *
 */

#ifdef ANSI
PUBLIC S16 EuLiEgtSigInd
(
Pst       *pst,
SuId       suId,
CmTptAddr *lclAddr,
CmTptAddr *remAddr,
TknU32     lclTeid,
EgMsg     *egMsg
)
#else
PUBLIC S16 EuLiEgtSigInd(pst, suId, lclAddr, remAddr, lclTeid, egMsg)
Pst       *pst;
SuId       suId;
CmTptAddr *lclAddr;
CmTptAddr *remAddr;
TknU32     lclTeid;
EgMsg     *egMsg;
#endif
{
   S16 ret;

   AV_PGW_SND_TRC3(EuLiEgtSigInd)

   if(NULLP == egMsg)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV008, (ErrVal) suId,\
                      "Null EGTPC Message from EGTP sap");
      AV_PGW_SND_RETVALUE(RFAILED);
   }
   if(suId != avPgwCb.egtCSapCb->suId) /* Invalid SAP Identification */
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV009, (ErrVal) suId,\
                      "Invalid SAP identification from EGTP sap");

      EgUtilDeAllocGmMsg(&egMsg);
      AV_PGW_SND_RETVALUE(RFAILED);
   }
#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
          "EuLiEgtSigInd: Received Signaling Indication on suId(%d),\
          teId(%d)\n", (S32)suId, (U32)lclTeid.val));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
          "EuLiEgtSigInd: Received Signaling Indication on suId(%ld),\
          teId(%ld)\n", (S32)suId, (U32)lclTeid.val));
#endif


   /* Let us invoke EGTPC Control tunnel machine */
   ret = avPgwEgtCTunnelMachine(NULLP, lclAddr, remAddr, lclTeid.val, 
                                AV_PGW_EGTPC_MSG_UNKNOWN, egMsg);

   AV_PGW_SND_RETVALUE(ret);
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
      EgMsg       *egMsg        /* structure for carrying eGTP MSG info */
      )
#else
PUBLIC S16 EuLiEgtSigCfm(pst,suId,localAddr,remAddr,transId,teid,egMsg)
      Pst         *pst;
      SuId         suId;
      CmTptAddr   *localAddr;
      CmTptAddr   *remAddr;
      TknU32       transId;
      TknU32       teid;
      EgMsg       *egMsg;
#endif

{
   S16 ret = ROK;
   AV_PGW_SND_TRC3(EuLiEgtSigCfm)

   if(avPgwCb.init.cfgDone != TRUE)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV010, (ErrVal) suId,\
            ("PGW is not configured \n"));
      RETVALUE(RFAILED);
   }
   /* av001.101: added null check */
   if(NULLP == egMsg)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV008, (ErrVal) suId,\
                      "Null EGTPC Message from EGTP sap");
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* av001.101: added SAP id validation */
   if(suId != avPgwCb.egtCSapCb->suId) /* Invalid SAP Identification */
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV009, (ErrVal) suId,\
                      "Invalid SAP identification from EGTP sap");

      EgUtilDeAllocGmMsg(&egMsg);
      AV_PGW_SND_RETVALUE(RFAILED);
   }

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
          "EuLiEgtSigCfm: Received Signaling  on suId(%d),\
          teId(%d)\n", suId, teid.val));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Received Signaling  on suId(%d),\
                     teId(%ld)\n", suId, teid.val));
#endif


   /* av001.101: added message handling */
   /* Let us invoke EGTPC Control tunnel machine */
   ret = avPgwEgtCTunnelMachine(NULLP, localAddr, remAddr, teid.val, 
                                AV_PGW_EGTPC_MSG_UNKNOWN, egMsg);

   AV_PGW_SND_RETVALUE(ret);


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
*       File:  av_lim.c   
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
   AV_PGW_SND_TRC3(EuLiEgtStaInd);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "Status indication received from EGTP."));
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
*       File:  av_lim.c 
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
   AV_PGW_SND_TRC3(EuLiEgtStaCfm);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "Status Confirmation received from EGTP."));
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
*       File:  av_lim.c  
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

   AV_PGW_SND_TRC3(EuLiEgtErrInd);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "Error Indication received from EGTP."));
   RETVALUE(ROK);
} /* End of EuLiEgtErrInd */

/*
 *
 *       Fun:   Confirmation of Local Deletion of Control tunnel
 *
 *       Desc:  Invoked from EGTP layer to user through EGTP Upper Interface
 *              primitive.
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_lim.c
 *
 */

#ifdef ANSI
PUBLIC S16 EuLiEgtLclDelCfm
(
Pst          *pst,  
SuId         suId,   
U32          transId, 
U32          teid,  
CmStatus     status 
)
#else
PUBLIC S16 EuLiEgtLclDelCfm(pst, suId, transId, teid, status)
Pst          *pst;
SuId         suId;   
U32          transId;
U32          teid;
CmStatus     status;
#endif
{

   AvPgwUeTunnelCb   *tunnel = NULLP;
   S16               ret = ROK;

   AV_PGW_SND_TRC3(EuLiEgtLclDelCfm);

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
          "EuLiEgtLclDelCfm: Received Local Tunnel Deletion \
           confirmation on suId(%d), teId(%d), transId (%d), status(%d)\n",
           suId, teid, transId, status.status));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
          "EuLiEgtLclDelCfm: Received Local Tunnel Deletion \
           confirmation on suId(%d), teId(%ld), transId (%ld), status(%d)\n",
           suId, teid, transId, status.status));
#endif

   if(suId == avPgwCb.egtCSapCb->suId)
   {
      ret = avPgwFndUeTunnelCb(&tunnel, teid, AV_PGW_CTRL_TUNNEL);
      if(ret == ROK)
      {
         if(tunnel->state == AV_PGW_EGTPC_TUNNEL_DOWN)
         {
            avPgwDelUeTunnelCb(tunnel->ueCb, &tunnel, AV_PGW_CTRL_TUNNEL);
         }
         else
         {
            ret = RFAILED;
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                     "In GTP-C Delete Tunnel request failed.\n"));
         }
      }
      else
      {
         ret = RFAILED;
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                     "In GTP-C Delete Tunnel request failed.\n"));
      }
   }
   else if(suId == avPgwCb.egtUSapCb->suId)
   {
      ret = avPgwFndUeTunnelCb(&tunnel, teid, AV_PGW_DATA_TUNNEL);
      if(ret == ROK)
      {
         if(tunnel->state == AV_PGW_EGTPU_TUNNEL_PENDING)
         {
            avPgwDelUeTunnelCb(tunnel->ueCb, &tunnel, AV_PGW_DATA_TUNNEL);
         }
         else
         {
            ret = RFAILED;
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                     "In GTP-U Delete Tunnel request failed.\n"));
         }
      }
      else
      {
         ret = RFAILED;
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                     "In GTP-U Delete Tunnel request failed.\n"));
      }
   }
   else /* Invalid SAP Identification */
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV011, (ErrVal) suId,\
                      "Invalid SAP identification from EGTP sap");
      AV_PGW_SND_RETVALUE(RFAILED);

   }

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Deleting PDN Successfull"));
   AV_PGW_SND_RETVALUE(ROK);
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
   AV_PGW_SND_TRC3(EuLiEgtTunnModCfm)

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Tunnel Mod Cfm Received From EGTP\n"));
   RETVALUE(ROK);

}


/**************************************************************************/
/* GTP-U Primitives - Handling tunnel management and data path primitives */
/**************************************************************************/

/*
 *
 *       Fun:   Confirmation of Local Tunnel Management Request
 *
 *       Desc:  Addition / modification / deletion of the EGTP-U tunnel status
 *              is triggered from here. Invoked from EGTP layer to user through
 *              EGTP Upper Interface primitive.
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_lim.c
 *
 */

#ifdef ANSI
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm
(
Pst       *pst,
SuId       suId,
EgtUEvnt  *mgmtCfm
)
#else
PUBLIC S16 EuLiEgtEguLclTnlMgmtCfm(pst, suId, mgmtCfm)
Pst       *pst;
SuId       suId;
EgtUEvnt  *mgmtCfm;
#endif
{
   S16 ret;

   AV_PGW_SND_TRC3(EuLiEgtEguLclTnlMgmtCfm);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
          "EuLiEgtEguLclTnlMgmtCfm: Received Local Tunnel Management \
           confirmation on suId(%d), status(%d)\n",
           suId, (mgmtCfm->u.tnlMgmt.cfmStatus.status)));

   if(NULLP == mgmtCfm)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV012, (ErrVal) suId,\
                      "Null EGTPU Message from EGTP sap");
      SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   if(suId == avPgwCb.egtUSapCb->suId)
   {
      /* Let us invoke EGTPU Data tunnel machine */
      ret = avPgwEgtUTunnelMachine(NULLP, (mgmtCfm->u.tnlMgmt.teId),
                               (mgmtCfm->u.tnlMgmt.action), mgmtCfm);

      if (ret != ROK)
      {
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                  "In GTP-U Tunnel Mgmt request failed.\n"));
      }
   }
   else /* Invalid SAP Identification */
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV013, (ErrVal) suId,\
                      "Invalid SAP identification from EGTP sap");
      SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));
      AV_PGW_SND_RETVALUE(RFAILED);

   }
#ifdef SS_MEM_LEAK_STS
   U8 action = mgmtCfm->u.tnlMgmt.action;
#endif 
   SPutSBuf(pst->region, pst->pool, (Data *)mgmtCfm, sizeof(EgtUEvnt));

#ifdef SS_MEM_LEAK_STS
   if(action == EGT_TMGMT_REQTYPE_DEL)
   { 
      SLogLkInfo(); 
   }
#endif 
   AV_PGW_SND_RETVALUE(ROK);
}


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
*       File:  av_lim.c
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
   AV_PGW_SND_TRC3(EuLiEgtEguStaCfm);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "Status Confirmation received from EGTPU"));
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
*       File:  av_lim.c  
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
   AV_PGW_SND_TRC3(EuLiEgtEguUStaInd);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "Status Indication received from EGTPU"));
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
         EgtUEvnt     *errInd       /* error event */
        )     
#else
PUBLIC S16 EuLiEgtEguErrInd(pst,suId,errInd)
         Pst          *pst;
         SuId         suId;
         EgtUEvnt     *errInd;
#endif
{
   S16 ret = ROK;
   AV_PGW_SND_TRC3(EuLiEgtEguErrInd)

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "In GTP-U. Cause = %d, \
               local TEID = %d remote TEID = %d Diagnostic Information = \
               %d\n",
               errInd->u.errInd.errCause, errInd->u.errInd.localTeid,
               errInd->u.errInd.remTeid, errInd->u.errInd.diagInfo));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
            "In GTP-U. Cause = %d, \
               local TEID = %ld remote TEID = %ld Diagnostic Information = \
               %ld\n",
               errInd->u.errInd.errCause, errInd->u.errInd.localTeid,
               errInd->u.errInd.remTeid, errInd->u.errInd.diagInfo));
#endif
   SPutSBuf(pst->region, pst->pool, (Data* )errInd, (sizeof(EgtUEvnt)));

   RETVALUE(ret);
}


#ifdef AV_PERF
/*
 *
 *       Fun:   eGTP-U Data Indication from layer to user. 
 *
 *       Desc:  Invoked through EGTP Upper Interface primitive.
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_lim.c
 *
 */

#ifdef ANSI
PUBLIC S16 EuLiEgtEguDatInd 
(
Pst          *pst,        /* post structure */
SuId         suId,        /* eGTP-U service user SAP ID */ 
EgtUEvnt     *datInd      /* Data Indication */
)
#else
PUBLIC S16 EuLiEgtEguDatInd (pst, suId, datInd)
Pst          *pst;        /* post structure */
SuId         suId;        /* eGTP-U service user SAP ID */ 
EgtUEvnt     *datInd;     /* Data Indication */
#endif
{
   PRIVATE U32 ulDataCntr = 1;
   S16      ret;

   AV_PGW_SND_TRC3(EuLiEgtEguDatInd);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"Received data at PDN-GW for UE tunnel %d, total %d\n", 
           datInd->u.egMsg->lclTeid, ulDataCntr));


   /* On receipt of first UL data, inform the receiver task
      to start DL data load */
   if(1 == ulDataCntr)
   {
      Buffer  *sampleBuf = NULLP;
      Pst      selfPst;

      /* Track the receipt of first packet */
      clock_gettime(0, &entryRx);

      /* Allocate Buffer for posting to avActvTsk */
      ret = SGetMsg(avPgwCb.init.region, avPgwCb.init.pool, 
                    &sampleBuf);
      if (ROK != ret)
      {
         AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAVXXX, (ErrVal)ret,
                          "Trigger receiving packets - allocate mBuf failed \n");
         RETVALUE(RFAILED);
      }

      selfPst.srcProcId = SFndProcId();
      selfPst.srcEnt    = ENTEU; /* Source: PDN-GW */
      selfPst.srcInst   = AV_PGW_PRIM_INSTANCE; /* PDN-GW instance 0 */
      selfPst.dstProcId = SFndProcId();
      selfPst.dstEnt    = ENTEU; /* Destination: PDN-GW */
      selfPst.dstInst   = AV_PGW_SECOND_INSTANCE; /* PDN-GW instance 1 */
      selfPst.selector  = 0; /* Loose coupled */
      selfPst.route     = RTESPEC;
      selfPst.prior     = PRIOR0;
      selfPst.event     = EVTAVSENDDLDATREQ;
      selfPst.region    = avPgwCb.routeCb.init.region;
      selfPst.pool      = avPgwCb.routeCb.init.pool;
   
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, 
                        "Trigger DL Data to S-GW \n"));
      SPstTsk(&selfPst, sampleBuf); 
   }
   /*  Calculate the UL Data rate for all the UE tunnels */
   else
   {
      unsigned long int diff_time  = 0;
   
      SFndLenMsg(datInd->u.egMsg->u.mBuf,&msgLen);
      totalSize+=msgLen;
      if( ulDataCntr > 1000)
      {
         clock_gettime(0, &exitRx);
         if (exitRx.tv_sec - entryRx.tv_sec == 0)
         {
            diff_time = exitRx.tv_nsec - entryRx.tv_nsec;
         }
         else if (exitRx.tv_sec - entryRx.tv_sec == 1)
         {
            diff_time = (exitRx.tv_nsec) + (1000000000 - entryRx.tv_nsec);
         }
         else
         {
            diff_time = ((exitRx.tv_sec - entryRx.tv_sec )*1000000000) + 
                         (exitRx.tv_nsec - entryRx.tv_nsec);
         }
         mbps = (totalSize * 8 * 1000000000)/(megaByte * diff_time);

         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: UL Data rate at application (%ld) Mbps\n", mbps));
         totalSize = 0;
         ulDataCntr  = 1;

         /* Track the receipt of first packet */
         clock_gettime(0, &entryRx);
      }
   }

   ulDataCntr++;

   /* Release mBuf & associated event structures */
   avPgwUtlDeAllocEguMsg(&datInd->u.egMsg); 
   SPutSBuf(pst->region, pst->pool, (Data* )datInd, (sizeof(EgtUEvnt)));

   AV_PGW_SND_RETVALUE(ROK);

}

#else

/*
 *
 *       Fun:   eGTP-U Data Indication from layer to user. 
 *
 *       Desc:  Invoked through EGTP Upper Interface primitive.
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_lim.c
 *
 */

#ifdef ANSI
PUBLIC S16 EuLiEgtEguDatInd 
(
Pst          *pst,        /* post structure */
SuId         suId,        /* eGTP-U service user SAP ID */ 
EgtUEvnt     *datInd      /* Data Indication */
)
#else
PUBLIC S16 EuLiEgtEguDatInd (pst, suId, datInd)
Pst          *pst;        /* post structure */
SuId         suId;        /* eGTP-U service user SAP ID */ 
EgtUEvnt     *datInd;     /* Data Indication */
#endif
{
   Buffer *uBuf;
   AvPgwUeTunnelCb   *tunnel = NULLP;
   S16 ret = RFAILED;

   AV_PGW_SND_TRC3(EuLiEgtEguDatInd);

#ifndef NO_ERRCLS
  if(NULLP == datInd)
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV014, (ErrVal) suId,\
                      "Null EGTPU Message from EGTP sap");
      AV_PGW_SND_RETVALUE(RFAILED);

   }

   if(suId != avPgwCb.egtUSapCb->suId) /* Invalid SAP Identification */
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV015, (ErrVal) suId,\
                      "Invalid SAP identification from EGTP-U sap");
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
             "Received data from EGTP on suId(%d)\n", suId));
#endif


   /* Map the tunnel Id recieved from S-GW */

   /* Take out the tunnel header */

   /* Send the remaining data payload */

   /* Assign the received buffer from EGTP-U map */  
   if (EGT_GTPU_MSG_GPDU == datInd->u.egMsg->msgHdr.msgType)
   {
      ret = cmHashListFind(&(avPgwCb.egtUSapCb->ueTunnelCbHlCp),
                      (U8 *)&datInd->u.egMsg->msgHdr.teId, (U16)sizeof(datInd->u.egMsg->msgHdr.teId),
                      (U16)0, (PTR *)&tunnel);
      if((ROK != ret) || tunnel == NULLP)
      {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                 "EuLiEgtEguDatInd: Failed to get data tunnelCb"));
      }

      uBuf = datInd->u.egMsg->u.mBuf;
      if(tunnel->tunnelType == AV_PGW_DATA_TUNNEL_DED)
      {
         avPgwLoopBackData(uBuf);
      }
      else
      {


      /* Send the received IP packet to the default Gateway */
      avPgwRoutHndlSndPkt(uBuf);
      }

      /* Release mBuf & associated event structures */
      SPutMsg(datInd->u.egMsg->u.mBuf);
      datInd->u.egMsg->u.mBuf = NULLP; /* mbuf is released now */

      avPgwUtlDeAllocEguMsg(&datInd->u.egMsg); 
      SPutSBuf(pst->region, pst->pool, (Data* )datInd, (sizeof(EgtUEvnt)));
   }

   AV_PGW_SND_RETVALUE(ROK);
}
#endif

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
*       File:  av_lim.c
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
   AV_PGW_SND_TRC3(EuLiEgtBndCfm);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
               "EuLiEgtBndCfm(pst, suId(%d), status(%d)\n", suId, status));

   if((suId != avPgwCb.egtCSapCb->suId) &&
      (suId != avPgwCb.egtUSapCb->suId) &&
      (suId != avPgwCb.egtEchoSapCb->suId))   /* Invalid SAP Identification */
   {
      AV_PGW_SND_LOGERROR(ERRCLS_INT_PAR, EAV018, (ErrVal) suId,\
                      "Invalid SAP identification from EGTP sap\n");
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   if(suId == avPgwCb.egtCSapCb->suId) /*EGTP-C SAP */
   {
      if(CM_BND_OK == status)
      {
         avPgwCb.egtCSapCb->state = LAV_SAP_ISBOUND;

         /*-- Send an alarm to SM --*/
         avSendLmAlarm(LCM_CATEGORY_INTERFACE, LAV_EVENT_SGW_EGTPC_BNDCFM,
            LCM_CAUSE_UNKNOWN);
      }
      else
      {
         avPgwCb.egtCSapCb->state = LAV_SAP_UBND;

         AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV019, (ErrVal) suId,
                      "Bind Failure with EGTPC for suId\n");

         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }
   else if(suId == avPgwCb.egtEchoSapCb->suId) /* EGTP- echo SAP */
   {
      if(CM_BND_OK == status)
      {
         avPgwCb.egtEchoSapCb->state = LAV_SAP_ISBOUND;

         /*-- Send an alarm to SM --*/
         avSendLmAlarm(LCM_CATEGORY_INTERFACE, LAV_EVENT_SGW_ECHO_BNDCFM,
            LCM_CAUSE_UNKNOWN);
      }
      else
      {
         avPgwCb.egtEchoSapCb->state = LAV_SAP_UBND;

         AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV020, (ErrVal) suId,
                      "Bind Failure with EGTPC for suId\n");

         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }
   else /* EGTP-U SAP */
   {
      if(CM_BND_OK == status)
      {
         avPgwCb.egtUSapCb->state = LAV_SAP_ISBOUND;

         /*-- Send an alarm to SM --*/
         avSendLmAlarm(LCM_CATEGORY_INTERFACE, LAV_EVENT_SGW_EGTPU_BNDCFM,
            LCM_CAUSE_UNKNOWN);

      }
      else
      {
         avPgwCb.egtUSapCb->state = LAV_SAP_UBND;
         AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV021, (ErrVal) suId,
                      "Bind Failure with EGTPU for suId\n");
         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }
 
   AV_PGW_SND_RETVALUE(ROK);
} /* End of EuLiEgtBndCfm */

/**********************************************************************
  
         End of file:     av_lim.c@@/main/1 - Tue Aug 17 18:54:33 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sp               1. LTE-CNE:PGW Initial Release.
/main/1      av001.101  rp               1. Handled SIG CFM message
*********************************************************************91*/


