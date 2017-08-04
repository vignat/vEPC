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

     Name:    Trillium LTE CNE - PGW Egt Utility Module

     Type:    C Include file

     Desc:    This file contains the PDN-GW Application source code for
              Encoding/Decoding Gtp-c messages

     File:    av_egt_utl.c

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
#include "avsm.h"


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
#include "avsm.x"

/* av002.101: Modified function name as this function name is already present */
/* av001.101: Added utility for adding bearer TFT */
PRIVATE S16 avPgwEgtUtlAddBCtxIe ARGS((
EgMsg          *egMsg,        /* gtp-c msg */
U32            ieType,        /* ie type */
U8             ieInst,        /* ie instance value */
EgDataTypes    dataType,      /* IE Data type */
EgIe           *egIe          /* pointer to IE details */
));


PRIVATE S16 avPgwUtilEgtAddBearerTft ARGS ((
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         *buf,
U32        len
));

PRIVATE S16 avPgwEgtUtlAppendIe ARGS((
EgMsg          *egMsg,        /* gtp-c msg */
U32            ieType,        /* ie type */
U8             ieInst,        /* ie instance value */
EgDataTypes    dataType,      /* IE Data type */
EgIe           *egIe          /* pointer to IE details */
));

PRIVATE S16  avPgwEgtUtlGetBearerCtxCnt ARGS((
EgMsg          *egMsg,
U8             inst,
S16            *bCnt
));

PRIVATE S16  avPgwEgtUtlGetIeCnt ARGS((
EgMsg          *egMsg,
U32            ieType,
U8             inst,
S16            *bCnt
));

 /* av004.101 : CR Field is not present in eGTP Rel9 */
#ifndef EG_REL_930
PRIVATE S16 avPgwUtlEgtBuildEgIeHeader ARGS((EgIe *egIe, 
                                             U8    ieType, 
                                             U8    ieInst,
                                             U8    ieCr, 
                                             U8    dataType));
#else
PRIVATE S16 avPgwUtlEgtBuildEgIeHeader ARGS((EgIe *egIe, 
                                             U8    ieType, 
                                             U8    ieInst,
                                             U8    dataType));
#endif /* EG_REL_930 */


PRIVATE S16 avPgwEgtUtlGetIeInBCtx ARGS((
EgMsg          *egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst,        /* Instance of ie in bearer ctx */
EgIe           **egIe         /* ie */
));

PRIVATE S16  avPgwUtlEgtAddBearerCxt ARGS((EgMsg           *egMsg, 
                                           EgIeLevelInfo   *egLvlInfo, 
                                           TknU8           *bearerId, 
                                           TknU8           *cause,
                                           U8               chrgId, 
                                           EgIe             egIe[], 
                                           U16              ieCnt));

/*
 *
 *    Fun:    avPgwEgtValCSReq
 *
 *    Desc:   This function Validates n stores Create SessioN Request IEs
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtValCSReq
(
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
)
#else
PUBLIC S16 avPgwEgtValCSReq(ueCb, pdnCb, egMsg, cause)
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
#endif
{
   S16               ret = ROK;
   AvPgwUeTunnelCb   *cTun = NULLP;
   EgIe              *egIe = NULLP;
   AvPgwBearerCb     *bearer = NULLP;
   AvPgwUeTunnelCb   *uTun = NULLP;
   U32               bearerInd = 0;

   AV_PGW_SND_TRC2(avPgwEgtValCSReq)

   /* SGW s5/s8 C F-TEID */
   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 0);
   if (ROK != ret || NULLP == egIe)
   {
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                           "Decoding of SGW S5/S8 C F-TEID, FAILED"));
      ret = RFAILED;
   }
   else
   {
      AV_PDNGW_SENDTSK_ALLOC(cTun, sizeof(AvPgwUeTunnelCb)); 
      if(NULLP == cTun)
      {
         *cause = EGT_GTPC_CAUSE_NO_MEM_AVAIL;
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                         "Memory allocation for control tunnel, FAILED"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }
      else
      {
         cTun->remoteTeId = egIe->t.fTeid.teid;
         cTun->remAddr.type = CM_NETADDR_IPV4;
         avPgwUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,\
               &(cTun->remAddr.u.ipv4TptAddr.address));
         cTun->ueCb = ueCb;
         cTun->pdnCb = pdnCb;

         pdnCb->s5s8CTun = cTun;
      }
   }

   /* PAA */
   egIe = NULLP;
   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_PAA_IETYPE, 0);
   if (ROK != ret || NULLP == egIe)
   {
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "PAA IE Not repsent"));
   }
   else
   {
      if (egIe->t.paa.pdnType == 1) /* IPv4 */
      {
         pdnCb->ipAddr.type = CM_NETADDR_IPV4;
         avPgwUtlIpv4ArrToInet(egIe->t.paa.u.pdnIpv4Addr,
                              &(pdnCb->ipAddr.u.ipv4NetAddr));
      }
      else
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IPv6 Not supported of UE"));
         ret = RFAILED;
      }
   }

   /* Bearer Context - EBI */
   if (ROK == ret)
   {
      ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
      if (ROK != ret || NULLP == egIe)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                           "Getting EBI IE from Bearer Context"));
      }
      else
      {
         bearerInd = AV_PGW_BEARER_IND(egIe->t.valUnSgnd8);
         if ((bearerInd < 0) || (bearerInd >= AV_PGW_MAX_BEARERS))
	 {
            *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
            ret = RFAILED;
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                 "Bearer ID is out of range"));
            AV_PGW_SND_RETVALUE(ret);
	 }	 
         if ((ueCb->bearerTable[bearerInd] != NULLP)) 
         {
            *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
            ret = RFAILED;
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                 "Bearer ID Already Allocated"));
         }
         else
         {
            AV_PDNGW_SENDTSK_ALLOC(bearer, sizeof(AvPgwBearerCb)); 
            if (NULLP == bearer)
            {
               ret = RFAILED;
               *cause = EGT_GTPC_CAUSE_NO_MEM_AVAIL;
               AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                 "Allocating memory for Bearer CB, FAILED"));
            }
            else
            {
               bearer->ebi = egIe->t.valUnSgnd8;
               bearer->pdnCb = pdnCb;
               bearer->isDefBearer = TRUE;

               ueCb->bearerTable[bearerInd] = bearer;
               ueCb->numBearers += 1;

               /* Set the PDN's bearer mapping */
               AV_PGW_SET_BEARER_MAP(pdnCb->bearerMap, bearer->ebi);
               pdnCb->s5s8CTun->bearerId = bearer->ebi;
            }
         }
      }
   }

   /* Beaerer Context - SGW U F-TEID  */
   if (ROK == ret)
   {
      ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 2, &egIe);
      if (ROK != ret || NULLP == egIe)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                         "Getting F-TEID IE from Bearer Context"));
      }
      else
      {
         AV_PDNGW_SENDTSK_ALLOC(uTun, sizeof(AvPgwUeTunnelCb)); 
         if(NULLP == uTun)
         {
            *cause = EGT_GTPC_CAUSE_NO_MEM_AVAIL;
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Memory allocation for User Plane tunnel, FAILED"));
            ret = RFAILED;
         }
         else
         {
            uTun->pdnCb = pdnCb;
            uTun->ueCb = ueCb;
            uTun->remoteTeId = egIe->t.fTeid.teid;
            uTun->bearerId = bearer->ebi;
            uTun->remAddr.type = CM_NETADDR_IPV4;
            avPgwUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr, 
                                 &(uTun->remAddr.u.ipv4TptAddr.address));

            bearer->s5s8UTun = uTun;
         }
      }
   }

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwEgtValCSReq */

/*
 *
 *    Fun:    avPgwEgtUtlGetIeInBCtx
 *
 *    Desc:   Gets an IE in Bearer Context Ie in gtpc-msg
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwEgtUtlGetIeInBCtx
(
EgMsg          *egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst,        /* Instance of ie in bearer ctx */
EgIe           **egIe          /* ie */
)
#else
PRIVATE S16 avPgwEgtUtlGetIeInBCtx(egMsg, bearerInst, bearerCnt, ieType,
ieInst, egIe)
EgMsg          *egMsg;        /* gtp-c msg */
U8             bearerInst;    /* Instance of Bearer Ctx */
U8             bearerCnt;     /* occr count of bearer */
U32            ieType;        /* type of Ie in bearer ctx */
U8             ieInst;        /* Instance of ie in bearer ctx */
EgIe           **egIe;        /* ie */
#endif
{
   EgIeLevelInfo     ieLvlInfo = {0};
   U16               iePos = 0;
   S16               ret = ROK;

   AV_PGW_SND_TRC2(avPgwEgtUtlGetIeInBCtx)
   
   ieLvlInfo.level = 1;
   ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst = bearerInst;
   ieLvlInfo.ieInfo[0].occrPres = TRUE;
   ieLvlInfo.ieInfo[0].occr = bearerCnt;

   ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[1].ieType = ieType;
   ieLvlInfo.ieInfo[1].ieInst = ieInst;

   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, egIe, &iePos);
   
   AV_PGW_SND_RETVALUE(ret);
}


/*
*
*       Fun:   avPgwEgtUtlGetIE
*
*       Desc:  Thie function gets an IE from the EgMsg
*
*       Ret:  
*
*       Notes: None
*
*       File:  av.c
*
*/
 
#ifdef ANSI
PUBLIC S16 avPgwEgtUtlGetIE
(
EgMsg   *egMsg,
EgIe    **egIe,
U8      ieType,
U8      ieInst
)
#else
PUBLIC S16 avPgwEgtUtlGetIE(egMsg, egIe, ieType, ieInst)
EgMsg  *egMsg;
EgIe   **egIe;
U8     ieType;
U8     ieInst;
#endif /* ANSI */
{  
   EgIeLevelInfo ieLevelInfo;
   U16           iePos = 0;
   S16           ret = ROK;
   
   AV_PGW_SND_TRC2(avPgwEgtUtlGetIE)

   *egIe = NULLP;
   cmMemset((U8 *)&ieLevelInfo, 0, sizeof(EgIeLevelInfo));

   ieLevelInfo.level = 0;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieTypePres = TRUE;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieType = ieType;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieInst = ieInst;

   /* Retrieve IE */
   ret = EgUtilGmGetIe(egMsg, &ieLevelInfo, egIe, &iePos);

   AV_PGW_SND_RETVALUE(ret);
}



/*
 *
 *    Fun:    avPgwEgtSndFailureRsp
 *
 *    Desc:   Sends GTP-C Failure response to SGW
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtSndFailureRsp
(
AvPgwPdnCb     *pdnCb,  /* UE control block (IN) */
U32            locTeid, /* PGW C-Teid (IN) */
CmTptAddr      *locAddr, /* Local Addr */
CmTptAddr      *remAddr, /* Remote Addr */
EgtMsgType     msgType, /* Type of Response message (IN) */
TknU32         transId,   /* seq number (IN) */
U32            cause    /* Cause (IN) */
)
#else
PUBLIC S16 avPgwEgtSndFailureRsp(pdnCb, teid, msgType, cause)
AvPgwPdnCb     *pdnCb;  /* UE control block (IN) */
U32            locTeid; /* PGW C-Teid (IN) */
CmTptAddr      *locAddr; /* Local Addr */
CmTptAddr      *remAddr; /* Remote Addr */
EgtMsgType     msgType; /* Type of Response message (IN) */
U32            cause;   /* Cause (IN) */
#endif
{
   TknU8          causeIe;
   TknU8          bearerId;
   S16            ret = ROK;
   TknU32         localTeId;
   EgIeLevelInfo  ieLvlInfo;
   EgMsg          *rspMsg = NULLP;
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   EgIe           egIe;
   U32            remTeid = 0;

   AV_PGW_SND_TRC2(avPgwEgtSndFailureRsp)

   cmMemset((U8 *)&localTeId, 0, sizeof(TknU32));
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;

   if (egUtlAllocEgMsg(&rspMsg, &memInfo) != ROK)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                        "Allocating memory for the GTP Message failed\n"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   if (NULLP != pdnCb && NULLP != pdnCb->s5s8CTun)
   {
      remTeid = pdnCb->s5s8CTun->remoteTeId;
   }
   else
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
               "PDN Control Block is NULL\n"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

     

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&(rspMsg->msgHdr), remTeid, msgType, transId.val);

   /* Cause IE */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[0].ieTypePres = TRUE;
   egLvlInfo.ieInfo[0].ieType = EG_CAUSE_IETYPE;
   egLvlInfo.ieInfo[0].ieInst = 0;
   egUtlEgtAddCause(rspMsg, &egLvlInfo, cause, 0);

   /* Junk because of egtp layer */
   if (EGT_GTPC_MSG_CS_RSP == msgType ||
       EGT_GTPC_MSG_DB_FAIL_IN == msgType)
   {
      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres = TRUE;
      egLvlInfo.ieInfo[0].ieType     = EG_BEARER_CNTX_IETYPE;
      egLvlInfo.ieInfo[0].ieInst     = 0;

      bearerId.pres = TRUE;
      bearerId.val  = 0;
      causeIe.pres    = NOTPRSNT;
      causeIe.val     = cause;

      avPgwUtlEgtAddBearerCxt(rspMsg, &egLvlInfo, &bearerId, &causeIe, 0,
                              NULLP, 0);
   }
   else if (EGT_GTPC_MSG_BR_FAIL_IND == msgType)
   {
      /* LBI */
      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.t.valUnSgnd8 = pdnCb->s5s8CTun->bearerId;
      avPgwEgtUtlAppendIe(rspMsg, EG_EBI_IETYPE, 0, EG_UNSIGNED_8, &egIe); 

      /* PTI */
      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.t.valUnSgnd8 = pdnCb->pti;
      avPgwEgtUtlAppendIe(rspMsg, EG_PROCEDURE_TID_IETYPE, 0, EG_UNSIGNED_8,
                        &egIe); 

   }

   localTeId.pres = TRUE;
   localTeId.val = locTeid;

   EuLiEgtSigRsp(&(avPgwCb.egtCSapCb->pst), (avPgwCb.egtCSapCb->spId),
               localTeId, rspMsg);

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwEgtSndFailureRsp */


/*
 *
 *       Fun:   avPgwEgtBldCBReq
 *
 *       Desc:  This function builds the Create Session Response of EGTP
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtBldCBReq
(
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg,
U8                causeVal
)
#else
PUBLIC S16 avPgwEgtBldCBReq(pdnCb, egMsg, causeVal)
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg,
U8                causeVal
#endif
{
   CmLList           *tmp = NULLP;
   Mem               memInfo;
   TknU8             bearerId;
   TknU8             cause;   
   EgIeLevelInfo     egLvlInfo;
   EgIe              egIe[5];
   U16               ipaddr1;
   U16               ipaddr2;
   U8                ieCnt = 0;
   U8                cId = AV_PGW_CHRGNG_ID;
   AvPgwUeTunnelCb   *uTun = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtBldCBReq)

   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;

   if (egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Allocating memory for the GTP Message failed\n"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   /* av004.101 use the same sequence number received */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), pdnCb->s5s8CTun->remoteTeId,
                     EGT_GTPC_MSG_CB_REQ, pdnCb->seqNum);

   /* Add Sender PTI */
   cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
   avPgwUtilEgtAddPTidIe(*egMsg, pdnCb->pti, &egLvlInfo, 0);

   /* Add Linked EPS Bearer ID */
   cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
   egUtlEgtAddEpsBId(*egMsg, &egLvlInfo, pdnCb->s5s8CTun->bearerId, 0);

   /* Add one for number of bearers to include default */
   /* Currently hard coding this value*/
   CM_LLIST_FIRST_NODE(&(pdnCb->pendingTunLst), tmp);
   while (tmp)
   {
      uTun = ((AvPgwPendingTun *)tmp)->uTun;
      ieCnt = 0;

      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
      egLvlInfo.ieInfo[egLvlInfo.level].ieType     = EG_BEARER_CNTX_IETYPE;
      egLvlInfo.ieInfo[egLvlInfo.level].ieInst     = 0;

      bearerId.pres = TRUE;
      bearerId.val  = 0;
      cause.pres    = NOTPRSNT;
      cause.val     = causeVal;

      /* Add Fteid  information for S5/S8 Address user plane */
      egIe[ieCnt].ieType = EG_FTEID_IETYPE;
      egIe[ieCnt].ieInst = 1; /* S5/S8 F-TEID Instance */
      egIe[ieCnt].t.fTeid.teid = uTun->localTeIdKey;
      egIe[ieCnt].t.fTeid.intfType = 5; /* S5/S8 Interface */
      egIe[ieCnt].t.fTeid.isIPv4Pres = TRUE;
      ipaddr1 = (U16) GetHiWord(uTun->lclAddr.u.ipv4TptAddr.address);
      ipaddr2 = (U16) GetLoWord(uTun->lclAddr.u.ipv4TptAddr.address);
      egIe[ieCnt].t.fTeid.ip4Addr[0] = (U8) GetHiByte(ipaddr1);
      egIe[ieCnt].t.fTeid.ip4Addr[1] = (U8) GetLoByte(ipaddr1);
      egIe[ieCnt].t.fTeid.ip4Addr[2] = (U8) GetHiByte(ipaddr2);
      egIe[ieCnt].t.fTeid.ip4Addr[3] = (U8) GetLoByte(ipaddr2);
      ieCnt++;

      /* Add the TFT in the Bearer Context*/
      egIe[ieCnt].ieType = EG_BEARER_TFT_IETYPE;
      egIe[ieCnt].ieInst = 0; /* TFT Instance */
      egIe[ieCnt].t.valStr.length = uTun->tftLen; 
      cmMemcpy(egIe[ieCnt].t.valStr.val,
               uTun->tftBuf, egIe[ieCnt].t.valStr.length);            
      ieCnt++;
      /* Add the EPS QOS in the Bearer QOS */
      egIe[ieCnt].ieType = EG_BEARER_QOS_IETYPE;
      egIe[ieCnt].ieInst = 0; /* TFT Instance */
      /* av001.101: warning removal */
      cmMemcpy((U8*)&egIe[ieCnt].t.bQos, (U8*)&uTun->epsQos,
               sizeof(EgBearQos));            
      ieCnt++;

      avPgwUtlEgtAddBearerCxt(*egMsg, &egLvlInfo, &bearerId, &cause, cId,
                              egIe, ieCnt);


      CM_LLIST_NEXT_NODE(&(pdnCb->pendingTunLst), tmp);
   }

   AV_PGW_SND_RETVALUE(ROK);
}


/* av002.101: Modified function name as this function name is already present */
/* av001.101: Added utility to add PTI IE */
/*
*
*       Fun:   avPgwUtilEgtAddPTidIe
*
*       Desc:  This function is used to Build PTI Ie 
*
*       Ret:   None
*
*       Notes: None
*
*       File: smeg_ieutl.c
*
*/
#ifdef ANSI
PUBLIC S16  avPgwUtilEgtAddPTidIe
(
EgMsg      *egMsg,
U8         pTidValue,
EgIeLevelInfo *egLvlInfo,
U8               inst
)
#else
PUBLIC S16 avPgwUtilEgtAddPTidIe(egMsg, pTidValue, egLvlInfo, inst)
EgMsg      *egMsg;
U8         pTidValue;
EgIeLevelInfo *egLvlInfo;
U8               inst;
#endif /* ANSI */
{
   EgIe   egIe;

   TRC2(avPgwUtilEgtAddPTidIe)

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe,EG_PROCEDURE_TID_IETYPE,inst, 0,EG_UNSIGNED_8);
#else
   egUtlEgtBuildEgIeHeader(&egIe,EG_PROCEDURE_TID_IETYPE,inst,EG_UNSIGNED_8);
#endif /* EG_REL_930 */
   egIe.t.valUnSgnd8 = pTidValue;
   if(EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe) != ROK)
   {
       RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* avPgwUtilEgtAddPTidIe */


/*
 *
 *    Fun:    avPgwEgtValCBRsp
 *
 *    Desc:   This function Validates n stores Create bearer Response
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtValCBRsp
(
AvPgwUeCb      *ueCb,      /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,     /* PDN Control Block (INOUT) */
EgMsg          *egMsg,     /* Create Session Request message (IN) */
Bool           *sndDBReq   /* whether to send DB req to SGW or not (OUT) */
)
#else
PUBLIC S16 avPgwEgtValCBRsp(ueCb, pdnCb, egMsg, sndDbReq)
AvPgwUeCb      *ueCb;   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb;  /* PDN Control Block (INOUT) */
EgMsg          *egMsg;  /* Create Session Request message (IN) */
Bool           *sndDBReq;  /* whether to send DB req to SGW or not (OUT) */
#endif
{
   U8                ebi = 0;
   S16               ret = ROK;
   EgIe              *egIe = NULLP;
   AvPgwBearerCb     *bearer = NULLP;
   AvPgwUeTunnelCb   *uTun = NULLP;
   U32               bearerInd = 0;

   AV_PGW_SND_TRC2(avPgwEgtValCBRsp)
   
   /* If Create Bearer Response has any malformed IEs, the PGW
    * shall delete S1-U tunnels and send send Delete Bearer Request
    * to SGW to delete the dedicated bearers.
    * If Create Bearer Response has a failure cause dont send the
    * Delete bearer Request
    */
   *sndDBReq = FALSE;

   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ret != ROK || egIe == NULLP)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                           "Decoding of Cause IE, FAILED"));
      *sndDBReq = TRUE;
      AV_PGW_SND_RETVALUE(RFAILED);
   }
   else
   {
      if (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ)
      {
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                           "Failure Response recevied from SGW"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }

   /* Currently UE initiated create bearer is supported.
    * so only one bearer shall be present in Create bearer Request/Response */
   ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
   if (ROK != ret || NULLP == egIe)
   {
      ret = RFAILED;
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                      "Getting EBI IE from Bearer Context"));
   }
   else
   {
      ebi = egIe->t.valUnSgnd8;
   }

   /* pgw s5/s8 u f-teid in bctxt */
   if (ROK == ret)
   {
      egIe = NULLP;
      ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 3, &egIe);
      if (ROK != ret || NULLP == egIe)
      {
         ret = RFAILED;
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                         "Getting F-TEID PGW S5-U IE from Bearer Context"));
      }
      else
      {
         uTun = avPgwUtlFindPendingTun(&(pdnCb->pendingTunLst),
                                       egIe->t.fTeid.teid);
         if (uTun == NULLP)
         {
            ret = RFAILED;
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                 "mismatch in CBReq & CBRsp U - TEID"));
         }
         else
         {
            bearerInd = AV_PGW_BEARER_IND(ebi);
            AV_PDNGW_SENDTSK_ALLOC(bearer, sizeof(AvPgwBearerCb)); 
            if (NULLP == bearer)
            {
               ret = RFAILED;
               AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                    "Allocating memory for Bearer CB"));
            }
            else
            {
               uTun->bearerId = ebi;
               bearer->ebi = ebi;
               bearer->pdnCb = pdnCb;
               bearer->s5s8UTun = uTun;

               if((bearerInd >= 0) && (bearerInd < AV_PGW_MAX_BEARERS))
               {   
                  ueCb->bearerTable[bearerInd] = bearer;
                  ueCb->numBearers += 1;
                  /* Set the PDN's bearer mapping */
                  AV_PGW_SET_BEARER_MAP(pdnCb->bearerMap, bearer->ebi);
               }
               else
               {
                  ret = RFAILED;
               }   
            }
         }
      }
   }

   /* fill the sgw s5/s8 u teid */
   if (ROK == ret)
   {
      ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 2, &egIe);
      if (ROK != ret || NULLP == egIe)
      {
         ret = RFAILED;
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                         "Getting SGW U F-TEID from Bearer Context"));
      }
      else
      {
         uTun->remoteTeId = egIe->t.fTeid.teid;
         uTun->remAddr.type = CM_NETADDR_IPV4;
         avPgwUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr, 
                              &(uTun->remAddr.u.ipv4TptAddr.address));
      }
   }

   /* IE parsing or some other mem failure, so delete the user plan
    * bearers and temporary data created during create bearer request
    * and send Delete Bearer Request to all the pending tunnels */
   if (ret != ROK)
   {
      *sndDBReq = TRUE;
   }

   AV_PGW_SND_RETVALUE(ret);
}


#ifndef EG_REL_930
#ifdef ANSI
PRIVATE S16 avPgwUtlEgtBuildEgIeHeader
(
EgIe         *egIe,
U8           ieType,
U8           ieInst,
U8           ieCr ,
U8           dataType
)
#else
PRIVATE S16 avPgwUtlEgtBuildEgIeHeader(egIe, ieType, ieInst, ieCr, dataType)
EgIe         *egIe;
U8           ieType;
U8           ieInst;
U8           ieCr ;
U8           dataType;
#endif
#else
#ifdef ANSI
PRIVATE S16 avPgwUtlEgtBuildEgIeHeader
(
EgIe         *egIe,
U8           ieType,
U8           ieInst,
U8           dataType
)
#else
PRIVATE S16 avPgwUtlEgtBuildEgIeHeader(egIe, ieType, ieInst, dataType)
EgIe         *egIe;
U8           ieType;
U8           ieInst;
U8           dataType;
#endif
#endif /* EG_REL_930 */
{
   AV_PGW_SND_TRC2(avPgwUtlEgtBuildEgIeHeader)

   egIe->ieType = ieType;
   egIe->ieInst = ieInst;
#ifndef EG_REL_930
   egIe->ieCr = ieCr;
#endif /* EG_REL_930 */
   egIe->dataType = dataType;

   AV_PGW_SND_RETVALUE(ROK);
}/* End of function */


/*
*
*       Fun:   avPgwUtlEgtAddBearerCxt
*
*       Desc:  This function adds the bearer context IE to required EGTP-C Message
*              tunnel.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  av.c
*
*/

#ifdef ANSI
PRIVATE S16 avPgwUtlEgtAddBearerCxt
(
EgMsg            *egMsg,
EgIeLevelInfo    *egLvlInfo,
TknU8            *bearerId,
TknU8            *cause,
U8               chrgId,
EgIe             egIe[], /* array of FTEIDs with instance as index */
U16              ieCnt
)
#else
PRIVATE S16 avPgwUtlEgtAddBearerCxt(egMsg,egLvlInfo,bearerId,cause,\
chrgId,egIe,ieCnt)
EgMsg            *egMsg;
EgIeLevelInfo    *egLvlInfo;
TknU8            *bearerId;
TknU8            *cause;
U8               chrgId;
EgIe             egIe[];  /* array of FTEIDs with instance as index */
U16              ieCnt;
#endif
{
   EgIe   locEgIe;
   U16    cnt = 0;

   AV_PGW_SND_TRC2(avPgwUtlEgtAddBearerCxt)

   /* Fill the ie properties */
   cmMemset((U8 *)&locEgIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   avPgwUtlEgtBuildEgIeHeader(&locEgIe, EG_BEARER_CNTX_IETYPE, 0, 0,EG_GROUPED);
#else
   avPgwUtlEgtBuildEgIeHeader(&locEgIe, EG_BEARER_CNTX_IETYPE, 0, EG_GROUPED);
#endif /* EG_REL_930 */
   if(EgUtilGmAppendIe(egMsg, egLvlInfo, &locEgIe) != ROK)
   {
      RETVALUE(RFAILED);
   }

   egLvlInfo->level = egLvlInfo->level + 1;
   if(bearerId->pres == TRUE)
   {
      egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
      egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_EBI_IETYPE;
      egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = 0;
      egUtlEgtAddEpsBId(egMsg, egLvlInfo, bearerId->val, 0);
   }

   if(cause != NULLP)
   {
      if(cause->pres == PRSNT_NODEF)
      {
         egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_CAUSE_IETYPE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = 0;
         egUtlEgtAddCause(egMsg, egLvlInfo, cause->val, 0);
      }
   }

   for(cnt = 0; cnt < ieCnt; cnt++)
   {
      if(egIe[cnt].ieType == EG_FTEID_IETYPE)
      {
         egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_FTEID_IETYPE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = egIe[cnt].ieInst;
#ifndef EG_REL_930
         egUtlEgtAddFteid(egMsg, egLvlInfo, egIe[cnt].t.fTeid.teid,
               egIe[cnt].t.fTeid.teid, egIe[cnt].t.fTeid.u.ip4Addr,
               egIe[cnt].t.fTeid.intfType, egIe[cnt].ieInst);
#else
         egUtlEgtAddFteid(egMsg, egLvlInfo, egIe[cnt].t.fTeid.teid,
               egIe[cnt].t.fTeid.ip4Addr,egIe[cnt].t.fTeid.intfType,
                egIe[cnt].ieInst);
#endif /* EG_REL_930 */
      }
      /* av001.101: Added support for TFT IE */
      else if(egIe[cnt].ieType == EG_BEARER_TFT_IETYPE)
      {
         /* This has to be filled in advance */
         egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_BEARER_TFT_IETYPE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = egIe[cnt].ieInst;
         /* av002.101: Modified function name */
         avPgwUtilEgtAddBearerTft(egMsg, egLvlInfo, egIe[cnt].ieInst,\
               egIe[cnt].t.valStr.val, egIe[cnt].t.valStr.length );
      }
      /* av001.101: Added support for QOS IE */
      else if(egIe[cnt].ieType ==  EG_BEARER_QOS_IETYPE)
      {
         /* This to fill the QOS in the Create Beaerer Req*/
         egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_BEARER_QOS_IETYPE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = egIe[cnt].ieInst;
         egUtlEgtAddBearerQos(egMsg, egLvlInfo, &egIe[cnt].t.bQos,\
               egIe[cnt].ieInst);
      }
   }
   /* Add charging Id IE */
   if (chrgId)
   {
      egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
      egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_CHARGING_ID_IETYPE;
      egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = 0;
      avUtlEgtAddChargIdIe(egMsg, egLvlInfo, 0, chrgId);
   }

   AV_PGW_SND_RETVALUE(ROK);

}/* End of function */


/*
 *
 *       Fun:   avPgwEgtUtlBldCSRsp
 *
 *       Desc:  This function builds the Create Session Response of EGTP
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtUtlBldCSRsp
(
AvPgwUeTunnelCb  *ueTunlCb, 
EgMsg            **egMsg,
U8               causeVal
)
#else
PUBLIC S16 avPgwEgtUtlBldCSRsp(ueTunlCb, egMsg, causeVal)
AvPgwUeTunnelCb  *ueTunlCb; 
EgMsg            **egMsg;
U8               causeVal;
#endif
{
   Mem            memInfo;
   TknU8          bearerId;
   TknU8          cause;   
   EgIeLevelInfo  egLvlInfo;
   EgIe           egIe[1];
   U16 ipaddr1;
   U16 ipaddr2;
   U8  addrInfo[4];
   U8 apn;
   U32           bId;
   AvPgwUeTunnelCb *s5s8UTun = NULLP;
   U8  pco[27] = {0x80, 0x80, 0x21, 0x10, 0x02, 0x00, 0x00, 0x10, 0x81, 0x06,
                  0xac, 0x18, 0x64, 0x32, 0x83, 0x06, 0xac, 0x18, 0x64, 0x33,
                  0x00, 0x0c, 0x04, 0xac, 0x18, 0x64, 0x34};

   AV_PGW_SND_TRC2(avPgwEgtUtlBldCSRsp)

   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;

   if(egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Allocating memory for the GTP Message failed\n"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), ueTunlCb->remoteTeId,
                      EGT_GTPC_MSG_CS_RSP, ueTunlCb->seqNum);

   /* Add cause */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType = EG_CAUSE_IETYPE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieInst = 0;
   egUtlEgtAddCause(*egMsg, &egLvlInfo, causeVal, 0);

   /* As per spec - 29.274 EGTPC, Sender F-TEIDIE shall be sent on the S11/S4
      interface.
      For the S5/S8 interface it is not needed beacuse its contect would be
      identical to the IE PDW S5/S8 address for the control plane or PMIP. */
   ipaddr1 = (U16) GetHiWord(ueTunlCb->lclAddr.u.ipv4TptAddr.address);
   ipaddr2 = (U16) GetLoWord(ueTunlCb->lclAddr.u.ipv4TptAddr.address);
   addrInfo[0] = (U8) GetHiByte(ipaddr1);
   addrInfo[1] = (U8) GetLoByte(ipaddr1);
   addrInfo[2] = (U8) GetHiByte(ipaddr2);
   addrInfo[3] = (U8) GetLoByte(ipaddr2);

   /* Add Fteid  S5/S8 Address for control plane */
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType = EG_FTEID_IETYPE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieInst = 1;
   egUtlEgtAddFteid(*egMsg, &egLvlInfo, ueTunlCb->localTeIdKey,
                     addrInfo, EGT_INT_S5_S8_PGW_GTP_C, 1);

   /* Add PDN Address allocation */
   ipaddr1 = (U16) GetHiWord(ueTunlCb->ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr);
   ipaddr2 = (U16) GetLoWord(ueTunlCb->ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr);
   addrInfo[0] = (U8) GetHiByte(ipaddr1);
   addrInfo[1] = (U8) GetLoByte(ipaddr1);
   addrInfo[2] = (U8) GetHiByte(ipaddr2);
   addrInfo[3] = (U8) GetLoByte(ipaddr2);

   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType = EG_PAA_IETYPE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieInst = 0;
   egUtlEgtAddPAA(*egMsg, &egLvlInfo, addrInfo, 0);

   /* Add APN restrition */
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType = EG_APN_RESTRICT_IETYPE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieInst = 0;
   apn = 0;
   avUtlEgtAddApnRstr(*egMsg, &egLvlInfo, &apn, 0);

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType     = EG_BEARER_CNTX_IETYPE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieInst     = 0;

   bearerId.pres = TRUE;
   bearerId.val  = ueTunlCb->bearerId;

   cause.pres    = TRUE;
   cause.val     = causeVal;
   bId = AV_PGW_BEARER_IND(ueTunlCb->bearerId);
   if((bId >= 0) && (bId < AV_PGW_MAX_BEARERS))
   {  
      if(ueTunlCb->ueCb && ueTunlCb->ueCb->bearerTable[bId] 
                        && ueTunlCb->ueCb->bearerTable[bId]->s5s8UTun)
      { 
        s5s8UTun = ueTunlCb->ueCb->bearerTable[bId]->s5s8UTun;
      }
      else
      {
          AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                       "Could not find the s5s8U tunnel for bearerId: %d\n", ueTunlCb->bearerId));
          EgUtilDeAllocGmMsg(egMsg);
          AV_PGW_SND_RETVALUE(RFAILED);
      }
   }

   /* Add Fteid  information for S5/S8 Address user plane */
   egIe[0].ieType = EG_FTEID_IETYPE;
   egIe[0].ieInst = 2; /* F-TEID Instance */
   egIe[0].t.fTeid.teid = s5s8UTun->localTeIdKey;
   egIe[0].t.fTeid.intfType = EGT_INT_S5_S8_PGW_GTP_U;
   egIe[0].t.fTeid.isIPv4Pres = TRUE;

   ipaddr1 = (U16) GetHiWord(ueTunlCb->lclAddr.u.ipv4TptAddr.address);
   ipaddr2 = (U16) GetLoWord(ueTunlCb->lclAddr.u.ipv4TptAddr.address);
   egIe[0].t.fTeid.ip4Addr[0] = (U8) GetHiByte(ipaddr1);
   egIe[0].t.fTeid.ip4Addr[1] = (U8) GetLoByte(ipaddr1);
   egIe[0].t.fTeid.ip4Addr[2] = (U8) GetHiByte(ipaddr2);
   egIe[0].t.fTeid.ip4Addr[3] = (U8) GetLoByte(ipaddr2);
   avPgwUtlEgtAddBearerCxt(*egMsg, 
                 &egLvlInfo, &bearerId, &cause, 0, egIe, 1);
  
   /*Add PCO*/
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType = EG_PCO_IETYPE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieInst = 0;
   egIe[0].t.valStr.length = 27;
   ipaddr1 = (U16) GetHiWord(avSmCb.cfgCb.pcscfAddr);
   ipaddr2 = (U16) GetLoWord(avSmCb.cfgCb.pcscfAddr);
   pco[23] = (U8) GetHiByte(ipaddr1);
   pco[24] = (U8) GetLoByte(ipaddr1);
   pco[25] = (U8) GetHiByte(ipaddr2);
   pco[26] = (U8) GetLoByte(ipaddr2);
   egUtlEgtAddPCO(*egMsg, &egLvlInfo, pco,
                       egIe[0].t.valStr.length, 0);

   

   AV_PGW_SND_RETVALUE(ROK);
}/* End of avPgwEgtUtlBldCSRsp */




/* av002.101: Modified function name as this function name is already present */
/* av001.101: Added utility to added bearer TFT IE */
/*
*
*       Fun:   avPgwUtilEgtAddBearerTft
*
*       Desc:  This function is used to Build Tad Ie 
*
*       Ret:   None
*
*       Notes: None
*
*       File: smeg_ieutl.c
*
*/
#ifdef ANSI
PRIVATE S16  avPgwUtilEgtAddBearerTft
(
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         *buf,
U32        len
)
#else
PRIVATE S16 avPgwUtilEgtAddBearerTft(egMsg,egLvlInfo,inst,buf, len)
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         *buf,
U32        len
#endif /* ANSI */
{
   EgIe   egIe;

   TRC2(avPgwUtilEgtAddBearerTft)

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe,EG_BEARER_TFT_IETYPE,inst, 0,EG_STRING_MAX);
#else
   egUtlEgtBuildEgIeHeader(&egIe,EG_BEARER_TFT_IETYPE,inst, EG_STRING_MAX);
#endif /* EG_REL_930 */

   egIe.t.valStr16.length = len;
   cmMemcpy( (U8 *)egIe.t.valStr.val, (U8 *)buf ,egIe.t.valStr.length);

   if(EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}  /* avPgwUtilEgtAddBearerTft  */





/*
 *
 *       Fun:   avPgwBldDeleteBearerRequest
 *
 *       Desc:  This function builds the Delete bearer request
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwBldDeleteBearerRequest
(
AvPgwPdnCb       *pdnCb,
EgMsg            **egMsg
)
#else
PUBLIC S16 avPgwBldDeleteBearerRequest(pdnCb, egMsg)
AvPgwPdnCb       *pdnCb;
EgMsg            **egMsg;
#endif
{
   AvPgwUeTunnelCb   *uTun = NULLP;
   EgIeLevelInfo     ieLvlInfo;
   CmLList           *tmp = NULLP;
   Mem               memInfo;
   S16               ret = ROK;

   AV_PGW_SND_TRC2(avPgwBldDeleteBearerRequest)

   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;

   if(egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "avPgwBldDeleteBearerRequest"
                           "Allocating memory for the GTP Message\n"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), pdnCb->s5s8CTun->remoteTeId,
                      EGT_GTPC_MSG_DB_REQ, 0);

   /* Add the EBI list in to the Message */
   CM_LLIST_FIRST_NODE(&(pdnCb->pendingTunLst), tmp);
   while (tmp)
   {
      uTun = ((AvPgwPendingTun *)tmp)->uTun;

      cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
      ieLvlInfo.ieInfo[0].ieInst = 1;

      egUtlEgtAddEpsBId(*egMsg, &ieLvlInfo, uTun->bearerId, 0);

      CM_LLIST_NEXT_NODE(&(pdnCb->pendingTunLst), tmp);
   }

   AV_PGW_SND_RETVALUE(ret);

}/* End of avPgwBldDeleteBearerRequest */


/*
 *
 *       Fun:   avPgwUtlBldEgtDelSessionRsp
 *
 *       Desc:  This function builds the Delete Session Response message of EGTP
 *             
 *       Ret:  
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwUtlBldEgtDelSessionRsp
(
AvPgwUeTunnelCb  *ueTunlCb, 
EgMsg            **egMsg,
U8               causeVal
)
#else
PUBLIC S16 avPgwUtlBldEgtDelSessionRsp(ueTunlCb, egMsg, causeVal)
AvPgwUeTunnelCb  *ueTunlCb; 
EgMsg            **egMsg;
U8               causeVal;
#endif
{
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;

   AV_PGW_SND_TRC2(avPgwUtlBldEgtDelSessionRsp)

   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;

   if(egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                    "Allocating memory for the GTP Message failed\n"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }
   
   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), ueTunlCb->remoteTeId, 
                      EGT_GTPC_MSG_DS_RSP, ueTunlCb->pdnCb->transId.val);

   /* Add cause */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[egLvlInfo.level].ieTypePres = TRUE;
   egLvlInfo.ieInfo[egLvlInfo.level].ieType = EG_CAUSE_IETYPE;
   egUtlEgtAddCause(*egMsg, &egLvlInfo, causeVal, 0);

   AV_PGW_SND_RETVALUE(ROK);
}/* End of function avPgwUtlBldEgtDelSessionRsp */





/*
 *
 *    Fun:    avPgwValDBCmd
 *
 *    Desc:   This function Validates n stores Delete Bearer Command IEs
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwValDBCmd
(
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
)
#else
PUBLIC S16 avPgwValDBCmd(ueCb, pdnCb, egMsg, cause)
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
#endif
{
   S16               ret = ROK;
   EgIe              *egIe = NULLP;
   S16               bCnt = 0;
   S32               bInd = 0;
   AvPgwPendingTun   *pendingTun = NULLP;

   AV_PGW_SND_TRC2(avPgwValDBCmd)

   /* Only one Bearer Context shall be present */
   ret = avPgwEgtUtlGetBearerCtxCnt(egMsg, 0, &bCnt);
   if (ret == ROK && bCnt == 1)
   {
      ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
      if (ret != ROK || egIe == NULLP)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
      }
      else
      {
         if (AV_PGW_VALIDATE_EBI(egIe->t.valUnSgnd8))
         {
            bInd = AV_PGW_BEARER_IND(egIe->t.valUnSgnd8);

            AV_PDNGW_SENDTSK_ALLOC(pendingTun, sizeof(AvPgwPendingTun)); 
            if(NULLP == pendingTun || NULLP == ueCb->bearerTable[bInd])
            {
               *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
               AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                                    "Memory Allocation for AvPgwPendingTun"));
            }
            else
            {
               pendingTun->uTun = ueCb->bearerTable[bInd]->s5s8UTun;

               cmLListInit(&(pdnCb->pendingTunLst));
               cmLListAdd2Tail(&(pdnCb->pendingTunLst), &(pendingTun->lnk));
            }
         }
      }
   }
   else
   {
      ret = RFAILED;
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Delete bearer Command has "
                           "more than 1 Bearer Context"));
   }

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwValDBCmd */

/*
 *
 *    Fun:    avPgwEgtBldDeleteBearerReq
 *
 *    Desc:   This function builds Delete Bearer Request message
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwBldDBReq
(
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg
)
#else
PUBLIC S16 avPgwBldDBReq(pdnCb, egMsg)
AvPgwPdnCb        *pdnCb; 
EgMsg             **egMsg;
#endif
{
   S16               ret = ROK;
   CmLList           *tmp = NULLP;
   Mem               memInfo;
   EgIeLevelInfo     egLvlInfo;
   EgIe              egIe;
   AvPgwUeTunnelCb   *uTun = NULLP;

   AV_PGW_SND_TRC2(avPgwBldDBReq)

   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;

   if (egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Allocating memory for the GTP Message"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   /* av004.101 use the same sequence number received */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), pdnCb->s5s8CTun->remoteTeId,
                     EGT_GTPC_MSG_DB_REQ, pdnCb->seqNum);

   /* Add Sender PTI */
   if (pdnCb->transId.val != 0)
   {
      cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
      avPgwUtilEgtAddPTidIe(*egMsg, pdnCb->transId.val, &egLvlInfo ,0);
   }

   /* Add one for number of bearers to include default */
   /* Currently hard coding this value*/
   CM_LLIST_FIRST_NODE(&(pdnCb->pendingTunLst), tmp);
   while (tmp)
   {
      uTun = ((AvPgwPendingTun *)tmp)->uTun;
      if (uTun->tunnelType == AV_PGW_DATA_TUNNEL_DFLT)
      {
         /* ebi with instance 0 */
         egIe.t.valUnSgnd8 = uTun->bearerId;
         ret = avPgwEgtUtlAppendIe(*egMsg, EG_EBI_IETYPE, 0, EG_UNSIGNED_8,
                                 &egIe); 
         if (ret != ROK)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Encoding of EBI IE"));
         }

         /* encode on the default bearer id */
         break;
      }
      else
      {
         /* list of ebi with instance 1 */
         egIe.t.valUnSgnd8 = uTun->bearerId;
         ret = avPgwEgtUtlAppendIe(*egMsg, EG_EBI_IETYPE, 1, EG_UNSIGNED_8,
                                 &egIe); 
         if (ret != ROK)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Encoding of EBI IE"));
            break;
         }
      }

      CM_LLIST_NEXT_NODE(&(pdnCb->pendingTunLst), tmp);
   }

   AV_PGW_SND_RETVALUE(ROK);
}




/*
 *
 *    Fun:    avPgwEgtValDBRsp
 *
 *    Desc:   This function Validates n stores Delete Bearer Response IEs
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtValDBRsp
(
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg   /* Create Session Request message (IN) */
)
#else
PUBLIC S16 avPgwEgtValDBRsp(ueCb, pdnCb, egMsg, cause)
AvPgwUeCb      *ueCb;   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb;  /* PDN Control Block (INOUT) */
EgMsg          *egMsg;  /* Create Session Request message (IN) */
#endif
{
   S16               ret = ROK;
   EgIe              *egIe = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtValDBRsp)

   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ret != ROK || egIe == NULLP)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Decoding Cause IE"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }
   else
   {
      if (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ)
      {
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                           "Failure Response recevied from SGW"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwEgtValDBRsp */



/*
 *
 *    Fun:    avPgwEgtUtlAddBCtxIe
 *
 *    Desc:   Appends an IE into gtpc-msg
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwEgtUtlAddBCtxIe
(
EgMsg          *egMsg,        /* gtp-c msg */
U32            ieType,        /* ie type */
U8             ieInst,        /* ie instance value */
EgDataTypes    dataType,      /* IE Data type */
EgIe           *egIe          /* pointer to IE details */
)
#else
PRIVATE S16 avPgwEgtUtlAddBCtxIe(egMsg, ieType, ieInst, dataType, egIe)
EgMsg          *egMsg;        /* gtp-c msg */
U32            ieType;        /* ie type */
U8             ieInst;        /* ie instance value */
EgDataTypes    dataType;      /* IE Data type */
EgIe           *egIe;         /* pointer to IE details */
#endif
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret = ROK;

   AV_PGW_RCV_TRC2(avPgwEgtUtlAddBCtxIe)

   AV_PGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
   AV_PGW_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   egUtlEgtBuildEgIeHeader(egIe, ieType, ieInst, dataType);
   
   ret = EgUtilGmAppendIe(egMsg, &ieLvlInfo, egIe);

   AV_PGW_RCV_RETVALUE(ret);
}


/*
 *
 *    Fun:    avPgwEgtUtlAppendIe
 *
 *    Desc:   Appends an IE into gtpc-msg
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwEgtUtlAppendIe
(
EgMsg          *egMsg,        /* gtp-c msg */
U32            ieType,        /* ie type */
U8             ieInst,        /* ie instance value */
EgDataTypes    dataType,      /* IE Data type */
EgIe           *egIe          /* pointer to IE details */
)
#else
PRIVATE S16 avPgwEgtUtlAppendIe(egMsg, ieType, ieInst, dataType, egIe)
EgMsg          *egMsg;        /* gtp-c msg */
U32            ieType;        /* ie type */
U8             ieInst;        /* ie instance value */
EgDataTypes    dataType;      /* IE Data type */
EgIe           *egIe;         /* pointer to IE details */
#endif
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret = ROK;

   AV_PGW_RCV_TRC2(avPgwEgtUtlAppendIe)

   AV_PGW_SET_IE_LVL_INFO(ieLvlInfo, 0, ieType, ieInst);

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(egIe, ieType, ieInst, 0, dataType);
#else
   egUtlEgtBuildEgIeHeader(egIe, ieType, ieInst, dataType);
#endif /* EG_REL_930 */
   
   ret = EgUtilGmAppendIe(egMsg, &ieLvlInfo, egIe);

   AV_PGW_RCV_RETVALUE(ret);
}


/*
 *
 *       Fun:   avPgwEgtUtlGetBearerCtxCnt
 *
 *       Desc:  This function is get the no of bearer contexts in message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File: smeg_ieutl.c
 *
 */
#ifdef ANSI
PRIVATE S16  avPgwEgtUtlGetBearerCtxCnt
(
EgMsg          *egMsg,
U8             inst,
S16            *bCnt
)
#else
PRIVATE S16 avPgwEgtUtlGetBearerCtxCnt(egMsg, inst, bCnt)
EgMsg          *egMsg;
U8             inst;
S16            *bCnt;
#endif /* ANSI */
{
   EgIeLevelInfo           ieLvlInfo = {0};
   S16                     ret = ROK;

   AV_PGW_RCV_TRC2(avPgwEgtUtlGetBearerCtxCnt)

   cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   AV_PGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, inst);
   
   /* Get the number of Bearer ID IEs */
   ret = EgUtilGmGetIeOccrCnt(egMsg, &ieLvlInfo, bCnt);

   AV_PGW_RCV_RETVALUE(ret);
}




/*
 *
 *       Fun:   avPgwIsCTunExist
 *
 *       Desc:  Checks whether the C-Tunnel with same remote teid
 *              already exists or not
 *             
 *       Ret:   TRUE/FALSE
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwIsCTunExist
(
AvPgwUeCb         *ueCb,
EgMsg             *egMsg
)
#else
PUBLIC S16 avPgwDelUeTunnelCb(ueCb, egMsg)
AvPgwUeCb         *ueCb;
EgMsg             *egMsg;
#endif
{
   Bool     ret = FALSE;
   U32      bCnt = 0;
   EgIe     *egIe = NULLP;
   U32      remTeid = 0;

   AV_PGW_SND_TRC2(avPgwIsCTunExist);

   /* get SGW's teid */
   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 0);
   if (ret == ROK && egIe != NULLP)
   {
      remTeid = egIe->t.fTeid.teid;
   }
   else
   {
      AV_PGW_SND_RETVALUE(ret);
   }

   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      if (ueCb->bearerTable[bCnt] != NULLP && 
          ueCb->bearerTable[bCnt]->isDefBearer == TRUE)
      {
         if (ueCb->bearerTable[bCnt]->pdnCb->s5s8CTun->remoteTeId == remTeid)
         {
            ret = TRUE;
            break;
         }
      }
   }

   AV_PGW_SND_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Validates of Modify Bearer Request received from SGW.
 *    S1-Handover with SGW relocation related IEs are validated here OR
 *    S1-Handover without SGW relocation, and ULI change
 *             
 * @param[in] pdnCb 
 *    Pointer to PDN Session CB on which the MBReq received
 * @param[in] egMsg
 *    Pointer to GTP message structure
 * @param[out] cause 
 *    Pointer to cause if MBReq validation fails
 * 
 * @return
 *    SUCCESS if validation is successful otherwise its FAILURE
 *
 *****************************************************************************/
PUBLIC S16 avPgwValMBReq
(
AvPgwPdnCb        *pdnCb,
EgMsg             *egMsg,
U32               *cause
)
{
   S32                     bInd    = 0;
   S16                     ret     = ROK;
   AvPgwBearerCb           *bearer = NULLP;
   EgIe                    *egIe   = NULLP;
   S16                     ieOcr   = 0;
   U32                     bCnt    = 0;
   AvPgwUeCb               *ueCb   = NULLP;
   AvPgwPendingTun         *pPendTun = NULLP;

   AV_PGW_SND_TRC2(avPgwValMBReq)

   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_ULI_IETYPE, 0);
   if (ret == ROK && NULLP != egIe)
   {
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Storing ULI for the UE"));

      if (TRUE == egIe->t.uli.isTAIPres)
      {
         pdnCb->ueCb->uli.tai = egIe->t.uli.tai;
      }

      if (TRUE == egIe->t.uli.isTAIPres)
      {
         pdnCb->ueCb->uli.ecgi = egIe->t.uli.ecgi;
      }
   }
   else
   {
      ret = ROK;
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "ULI Not Present"));
   }

   /* SGW S5/S8 C FTEID */ 
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Getting SGW S5/S8 C FTEID IE"));
   ret = avPgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 0);
   if (ret == ROK && NULLP != egIe)
   {
      pdnCb->s5s8CTun->remoteTeId = egIe->t.fTeid.teid;
      pdnCb->s5s8CTun->remAddr.type = CM_INET_IPV4ADDR_TYPE;
      avPgwUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,\
            &(pdnCb->s5s8CTun->remAddr.u.ipv4TptAddr.address));
   }
   else
   {
      ret = ROK;
   }

   ret = avPgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
   if (ret != ROK)
   {
      /* This is the case SGW forwards MBReq to PGW when a HO happens
       * without SGW change and there is change in ULI
       */
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Bearer Contexts to be "
            "Modified not present"));
      ret = ROK;
   }
   else
   {
      ueCb = pdnCb->ueCb;

      for (bCnt = 0; bCnt < ieOcr; bCnt++)
      {
         /* Get the EBI Id */
         ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1, EG_EBI_IETYPE, 0,\
               &egIe);
         if (ret != ROK || egIe == NULLP)
         {
            ret = RFAILED;
            *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Getting EBI IE"));
            break;
         }
         else
         {
            AV_PGW_GET_BEARER(ueCb->bearerTable, egIe->t.valUnSgnd8, bearer);
            if (NULLP == bearer)
            {
               *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
               AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Bearer Contet Not"
                     "Found for EBI Id"));
               ret = RFAILED;
               break;
            }
            else
            {
               AV_PDNGW_SENDTSK_ALLOC(pPendTun, sizeof(AvPgwPendingTun)); 
               if(NULLP == pPendTun || NULLP == ueCb->bearerTable[bInd])
               {
                  *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
                  AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Memory "\
                        "Allocation for AvPgwPendingTun"));
               }
               else
               {
                  egIe = NULLP;
                  ret = avPgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1,\
                        EG_FTEID_IETYPE, 1, &egIe);
                  if (NULLP != egIe && ret == ROK)
                  {
                     bearer->s5s8UTun->newRemTeid = egIe->t.fTeid.teid;
                     bearer->s5s8UTun->newRemAddr.type = CM_INET_IPV4ADDR_TYPE;
                     avPgwUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                           &(bearer->s5s8UTun->newRemAddr.u.\
                           ipv4TptAddr.address));

                     pPendTun->uTun = bearer->s5s8UTun;
                     cmLListInit(&(pdnCb->pendingTunLst));
                     cmLListAdd2Tail(&(pdnCb->pendingTunLst), &(pPendTun->lnk));
                  }
                  else
                  {
                     AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Getting "\
                           "S5/S8 FTEID IE"));
                     break;
                  }
               }
            }
         }
      }
   }

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwValMBReq */


/*
 *
 *       Fun:   avPgwEgtUtlGetIeCnt
 *
 *       Desc:  This function is get the no of bearer contexts in message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File: smeg_ieutl.c
 *
 */
#ifdef ANSI
PRIVATE S16  avPgwEgtUtlGetIeCnt
(
EgMsg          *egMsg,
U32            ieType,
U8             inst,
S16            *bCnt
)
#else
PRIVATE S16 avPgwEgtUtlGetIeCnt(egMsg, ieType, inst, bCnt)
EgMsg          *egMsg;
U32            ieType;
U8             inst;
S16            *bCnt;
#endif /* ANSI */
{
   EgIeLevelInfo           ieLvlInfo = {0};
   S16                     ret = ROK;

   AV_PGW_SND_TRC2(avPgwEgtUtlGetIeCnt);

   cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   AV_PGW_SET_IE_LVL_INFO(ieLvlInfo, 0, ieType, inst);
   
   /* Get the number of Bearer ID IEs */
   ret = EgUtilGmGetIeOccrCnt(egMsg, &ieLvlInfo, bCnt);

   AV_PGW_SND_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Builds Modify Bearer Response to be sent to SGW
 *    Only S1 Handover with SGW changes related IEs are encoded apart
 *    from mandatory IEs
 *             
 * @param[in] pPdnCb
 *    Pointer to PDN Session  Control Block
 * @param[out] pEgMsg 
 *    Pointer to GTP message. This message will be sent to SGW
 * 
 * @return
 *    SUCCESS if validation is successful otherwise its FAILURE
 *
 *****************************************************************************/
PUBLIC S16 avPgwBldMBRsp
(
AvPgwPdnCb        *pPdnCb, 
EgMsg             **pEgMsg
)
{
   U32               bIndex    = 0;
   Mem               memInfo   = {0, 0};
   AvPgwUeCb         *ueCb     = NULLP;
   CmLList           *pListEnt = NULLP;
   AvPgwUeTunnelCb   *pUTun    = NULLP;
   EgIe              egIe;

   AV_PGW_SND_TRC2(avPgwBldMBRsp)

   memInfo.region = avPgwCb.init.region;
   memInfo.pool   = avPgwCb.init.pool;
   ueCb           = pPdnCb->ueCb;

   if (egUtlAllocEgMsg(pEgMsg, &memInfo) != ROK )
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Memory Allocation: EgMsg"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   egUtlBuildEgHeader(&((*pEgMsg)->msgHdr), pPdnCb->s5s8CTun->remoteTeId,
                     EGT_GTPC_MSG_MB_RSP, pPdnCb->transId.val);

   /* cause ie */
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   egIe.t.cause.causeVal = EGT_GTPC_CAUSE_ACC_REQ;
   avPgwEgtUtlAppendIe(*pEgMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);

   CM_LLIST_FIRST_NODE(&(pPdnCb->pendingTunLst), pListEnt);
   while (pListEnt)
   {
      pUTun = ((AvPgwPendingTun *)pListEnt)->uTun;
      if (NULLP != pUTun)
      {
         bIndex = AV_PGW_BEARER_IND(pUTun->bearerId);
         if((bIndex < 0) || (bIndex >= AV_PGW_MAX_BEARERS))
         {   
            AV_PGW_SND_RETVALUE(RFAILED);
         } 

         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         avPgwEgtUtlAppendIe(*pEgMsg, EG_BEARER_CNTX_IETYPE, 0, EG_GROUPED,\
               &egIe);

         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.valUnSgnd8 = ueCb->bearerTable[bIndex]->ebi;
         avPgwEgtUtlAddBCtxIe(*pEgMsg, EG_EBI_IETYPE, 0, EG_UNSIGNED_8, &egIe);

         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_ACC_REQ;
         avPgwEgtUtlAddBCtxIe(*pEgMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);
      }
   
      CM_LLIST_NEXT_NODE(&(pPdnCb->pendingTunLst), pListEnt);
   }


   AV_PGW_SND_RETVALUE(ROK);
} /* avPgwBldMBRsp */


/**************************************************************************//**
 *
 * @brief 
 *    Sends tunnel modification request to the EGT Layer
 *             
 * @param[in] pUTun
 *    Pointer to pending tunnel Control Block
 * @param[in] pEgMsg 
 *    Pointer to GTP message. This message will be sent to SGW
 * 
 * @return
 *    SUCCESS if validation is successful otherwise its FAILURE
 *
 *****************************************************************************/
PUBLIC S16 avPgwEguTunnelModReq
(
AvPgwUeTunnelCb         *pUTun,
EgMsg                   *pEgMsg
)
{
   U16                  ret     = ROK;
   EgtUEvnt             *eguEvt = NULLP;

   AV_PGW_SND_TRC2(avPgwEguTunnelModReq);

   /* First create the local context for default s5s8 tunnel in EGTP layer
    * Send the Create Session Response after receiving the Confirm */
   AV_PDNGW_SENDTSK_ALLOC(eguEvt, sizeof(EgtUEvnt));
   if (eguEvt == NULLP)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Memory Allocation: EgtUEvnt"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Modify EGTP-U Tunnel For S5/S8-U"\
         "Interface With Tunnel %ld", pUTun->localTeIdKey));

   pUTun->pEgMsg = pEgMsg;
   eguEvt->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_MOD;
   eguEvt->u.tnlMgmt.teId = pUTun->localTeIdKey;
   eguEvt->u.tnlMgmt.reOrderPres = FALSE;
   eguEvt->u.tnlMgmt.intfType = EGT_INT_S5_S8_PGW_GTP_U;
   eguEvt->u.tnlMgmt.expSeqNmb = 1000;
   eguEvt->u.tnlMgmt.maxTPduSize = AV_PGW_DFLT_MAXTPDUSZ;
   eguEvt->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;

   cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.srcAddr), (U8 *)(&pUTun->lclAddr),\
         sizeof(CmTptAddr));
   cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.dstIpAddr), (U8 *)(&pUTun->newRemAddr),\
         sizeof(CmTptAddr));
   cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.frmDstIpAddr),\
         (U8 *)&(pUTun->remAddr), sizeof(CmTptAddr));

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Sending Tunnel Management "\
         "Request to EGT Layer"));
   
   avPgwSetRouteCbTunState(pUTun, AV_PGW_EGTPU_TUNNEL_PENDING);
   pUTun->state = AV_PGW_EGTPU_TUNNEL_PENDING;
   pUTun->pdnCb->cEvent = AV_PGW_EGTPC_MSG_MB_REQ_ACC;
   EuLiEgtEguLclTnlMgmtReq(&(avPgwCb.egtUSapCb->pst), avPgwCb.egtUSapCb->spId,\
         eguEvt);

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwEguTunnelModReq */

/**********************************************************************
  
         End of file:     av.c@@/main/1 - Tue Aug 17 18:54:31 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/

