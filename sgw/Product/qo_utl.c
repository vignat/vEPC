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
 
     Name:     Trillium LTE SGW Application    
  
     Type:     C include  file
  
     Desc:    This file contains utility functions for SGW 
  
     File:     qo_utl.c
  
     Sid:      qo_utl.c@@/main/1 - Thu Jul  8 21:22:21 2010
  
     Prg:      sp
  
*********************************************************************21*/

/* header include files -- defines (.h) */
#include <stdarg.h>        /* environment options */
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
#include "eg_util.h"
/* qo003.101 changes */
#ifndef EG_REL_930
#include "eg_dbs11.h"
#else
#include "eg_dbutl.h"
#endif /* EG_REL_930 */
#include "lqo.h"
#include "qo.h"
#include "qo_err.h"

/*
#include <stdio.h>
#include <pcap-bpf.h>
#include <pcap.h>
#include <stdlib.h>
*/

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
#include "egt.x"
#include "eg_util.x"
#include "egt_util.x"
#include "lqo.x"
#include "qo.x"

/************************ Prototypes **********************************/

#ifdef QO_SGW


PRIVATE S16 qoSgwSendUTunDel ARGS((
QoSgwTunnelCb     *tun        /* Tunnel to be deleted */
));


/*
 *
 *       Fun:   qoSgwUtlBldEgtDelSsnReq
 *
 *       Desc:  Invoked by the  application 
 *             
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwUtlBldEgtDelSsnReq
(
QoSgwTunnelCb    *sgwTunlCb,
EgMsg            **egMsg,
U8               cause
)
#else
PUBLIC S16 qoSgwUtlBldEgtDelSsnReq (sgwTunlCb, egMsg, cause)
QoSgwTunnelCb    *sgwTunlCb;
EgMsg            **egMsg;
U8               cause;
#endif
{
   EgIeLevelInfo         egLvlInfo;
   Mem memInfo;

   QO_SGW_TRC2(qoSgwUtlBldEgtDelSsnReq)
   QO_SGW_DBG_INFO((QO_SGW_PRNTBUF, "qoSgwUtlBldEgtDelSsnReq: "
               "Building Egt Delete Session Req "));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   if( qoUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG,
            "Allocating memory for the GTP Message failed\n");
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   qoUtlBuildEgHeader(&((*egMsg)->msgHdr), sgwTunlCb->remTeid,
                      EGT_GTPC_MSG_DS_REQ, sgwTunlCb->seqNum);

   /* Add cause */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   qoUtlEgtAddCause(*egMsg, &egLvlInfo, cause, 0);

   /* Linked EPS Bearer ID */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   qoUtlEgtAddEpsBId(*egMsg, &egLvlInfo, sgwTunlCb->ebi, 0);

   QO_SGW_RETVALUE(ROK);
}/* End of function qoSgwUtlBldEgtDelSsnReq */






/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoUtlIpv4InetToArr
(
U8           *addrArr,        /* ascii address representation */
CmInetIpAddr *address         /* 4 byte interent address */
)
#else
PUBLIC S16 qoUtlIpv4InetToArr(addrArr, address)
U8           *addrArr;        /* ascii address representation */
CmInetIpAddr *address;        /* 4 byte interent address */
#endif
{
   addrArr[3] = (U8)(*address & 0xff);
   addrArr[2] = (U8)((*address >> 8) & 0xff);
   addrArr[1] = (U8)((*address >> 16) & 0xff);
   addrArr[0] = (U8)((*address >> 24) & 0xff);

   RETVALUE(ROK);
}

/***********************************************************************
 * *
 * *    Fun:    qoUtlAllocEgMsg
 * *
 * *    Desc:   Allocate memory for the GTP message and initialize
 * *
 * *    Ret:    ROK  - if memory allocation and initialization is success
 * *
 * *    Notes:  None
 * *
 * *    File:   qo_utl.c
 * *
 * **********************************************************************/
#ifdef ANSI
PUBLIC S16 qoUtlAllocEgMsg
(
 EgMsg   **egMsg,    /* GTP Message */
 Mem     *memInfo    /* memory information */
 )
#else
PUBLIC S16 qoUtlAllocEgMsg(egMsg, memInfo)
   EgMsg   **egMsg;    /* GTP Message */
   Mem     *memInfo;   /* memory information */
#endif
{
   /*****************************************************
    ** Allocate memory for the gtp message of size EgMsg *
    ******************************************************/
   if (cmAllocEvnt(sizeof(EgMsg), EG_UTIL_MEM_SIZE, memInfo,
            (Ptr* )egMsg) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /******************
    ** return success *
    *******************/
   RETVALUE(ROK);
}


/***************************************************************/
PUBLIC S16 qoUtlIpv4ArrToInet(
   U8           *addrArr,        /* Byte Array address representation */
   CmInetIpAddr *address          /* 4 byte interent address */
   )
{
   *address = ((U32)addrArr[0] << 24)|  ((U32)addrArr[1] << 16)   | 
      ((U32)addrArr[2] << 8)  |  (U32)(addrArr[3]);
   RETVALUE(ROK);
}

/***************************************************************/
/**                 Utilities                                  */
/***************************************************************/
#ifdef QO_TODO_SECOND_PHASE
/*
 * Now thw Peers are added on the fly
 * It has to be configured from stack manager.
 * The peer address shd be always checked before accepting any
 * message from one. */
#endif
#ifdef ANSI
PUBLIC S16 qoSgwFindNCreatePeer
(
CmTptAddr *addr,
QoSgwPeerCb **peer
)
#else
PUBLIC S16 qoSgwFindNCreatePeer(addr,peer)
      CmTptAddr *addr;
      QoSgwPeerCb **peer;
#endif
{
   S16            ret = ROK;
   QoSgwPeerEnt   *peerEnt = NULLP;

   QO_SGW_TRC2(FindNCreatePeer)

   *peer = NULLP;
   ret = cmHashListFind(&(qoSgwCb.peerCbHlCp), (U8 *)addr, sizeof(CmTptAddr), 0,
         (PTR *)(&peerEnt));
   if(ret != ROK)
   {
      QO_SGW_DBG_INFO(("Peer not found. Now creating the peer."));
      ret = qoSgwCreatePeer(addr, peer);
   }
   else
   {
      *peer = &(peerEnt->peerCb);
      QO_SGW_DBG_INFO(("Peer Found."));
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwCreatePeer
    (
      CmTptAddr *addr, 
      QoSgwPeerCb **peer
     )
#else
PUBLIC S16 qoSgwCreatePeer(addr,peer)
      CmTptAddr *addr;
      QoSgwPeerCb **peer;
#endif
{
   S16 ret = ROK;
   QoSgwPeerEnt *peerEnt = NULLP;
   QO_SGW_TRC2(CreatePeer)

#ifdef QO_TODO_SECOND_PHASE
      /* Delete Peer is to be implemented
       * currently required only for shutdown */
#endif

   QO_SGW_ALLOC(&peerEnt, sizeof(QoSgwPeerEnt));
   if(peerEnt == NULLP)
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES,
            ("Could not create a peer Cb entry CB.\n"));
   }

   *peer = &peerEnt->peerCb;
   cmMemcpy((U8 *)&((*peer)->addr), (U8 *)addr, sizeof(CmTptAddr));
   /* Add into global hash list */
   ret = cmHashListInsert(&(qoSgwCb.peerCbHlCp), (PTR)peerEnt,
         (U8 *)&((*peer)->addr), sizeof((*peer)->addr));
   if(ret != ROK)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG,
            ("Failed to insert in hash list.\n"));
      QO_SGW_FREE(peerEnt, sizeof(QoSgwPeerEnt));
   }
   else
   {
      QO_SGW_DBG_INFO(("Successfully created a peer CB."));
   }
   /* Node type filled by the caller */
   QO_SGW_RETVALUE(ret);
}

/***************************************************************/
#ifdef ANSI
PUBLIC QoSgwTunnelCb * qoSgwFindTunnel
    (
      QoSgwEgtSapCb *sapCb, 
      U32 teid
    )
#else
PUBLIC QoSgwTunnelCb * qoSgwFindTunnel(sapCb,teid)
      QoSgwEgtSapCb *sapCb;
      U32 teid;
#endif
{
   QoSgwTunnEnt *tunEnt = NULLP;
   QoSgwTunnelCb *tunnel=NULLP;
   S16 ret = ROK;
   QO_SGW_TRC2(qoSgwFindTunnel)

   ret = cmHashListFind(&(sapCb->tunnCbHlCp), (U8 *)&teid, sizeof(U32), 0,
         (PTR* )&tunEnt);
   if(ret == ROK)
   {
      tunnel = &(tunEnt->tunnelCb);
      QO_SGW_DBG_INFO(("Tunnel Found. Teid = %d.", teid));
   }
   QO_SGW_RETVALUE(tunnel);
}
/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwFindNCreateTunnel
     (
      QoSgwEgtSapCb *sapCb, 
      U32 teid,
      QoSgwTunnelCb **tunnel
     )
#else
PUBLIC S16 qoSgwFindNCreateTunnel(sapCb,teid,tunnel)
      QoSgwEgtSapCb *sapCb;
      U32 teid;
      QoSgwTunnelCb **tunnel;
#endif
{
   S16 ret = ROK;
   QO_SGW_TRC2(FindNCreateTunnel)

   *tunnel = qoSgwFindTunnel(sapCb, teid);
   if(*tunnel == NULLP)
   {
      QO_SGW_DBG_INFO(("Tunnel not found. Create it."));
      /* Tunnel does not exist already
       * create one */
      ret = qoSgwCreateTunnel(sapCb, teid, tunnel);
   }
   QO_SGW_RETVALUE(ret);
}

/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwCreateTunnel
(
QoSgwEgtSapCb *sapCb, 
U32           teid,
QoSgwTunnelCb **tunnel
)
#else
PUBLIC S16 qoSgwCreateTunnel(sapCb, teid, tunnel)
QoSgwEgtSapCb *sapCb;
U32           teid;
QoSgwTunnelCb **tunnel;
#endif
{
   S16 ret = ROK;
   QoSgwTunnEnt *tunEnt = NULLP;

   QO_SGW_TRC2(CreateTunnel)

   QO_SGW_ALLOC(&tunEnt, sizeof(QoSgwTunnEnt));
   if(tunEnt == NULLP)
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES,
            ("Tunnel Entry CB could not be created.\n"));
      QO_SGW_RETVALUE(RFAILED);
   }

   *tunnel = &(tunEnt->tunnelCb);
   (*tunnel)->locTeid = teid;
   (*tunnel)->transId.pres = NOTPRSNT;
   (*tunnel)->locAddr = &(sapCb->localAddr);
   (*tunnel)->state = QO_SGW_TUN_IDLE;
   (*tunnel)->tRel = FALSE;

   /* Initialize tunnel add into the hash list rest of the initialization
    * by the caller */
   ret = cmHashListInsert(&(sapCb->tunnCbHlCp), (PTR)tunEnt,
         (U8 *)&(*tunnel)->locTeid, sizeof((*tunnel)->locTeid));
   if(ret != ROK)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, ("Failed to insert in hash list.\n"));
      QO_SGW_FREE(tunEnt, sizeof(QoSgwTunnEnt));
   }
   else
   {
      QO_SGW_DBG_INFO(("Tunnel created for Teid = %d.", teid));
   }

   QO_SGW_RETVALUE(ret);
}
/***************************************************************/
#ifdef ANSI
PUBLIC QoSgwTunnelCb * qoSgwFindTunnelTrans
     (
      QoSgwEgtSapCb *sap, 
      U32 transId
     )
#else
PUBLIC QoSgwTunnelCb * qoSgwFindTunnelTrans(sap,transId)
      QoSgwEgtSapCb *sap;
      U32 transId;
#endif
{
   S16 ret = ROK;
   QoSgwTunnEnt *tunEnt = NULLP;
   QO_SGW_TRC2(qoSgwFindTunnelTrans)

   ret = cmHashListFind(&(sap->tunnPendCbHlCp), (U8 *)&transId, sizeof(transId),
         0, (PTR *)&tunEnt);
   if(ret != ROK)
   {
      QO_SGW_DBG_ERR(("Tunnel CB for transId = %d not found.", transId));
      QO_SGW_RETVALUE(NULLP);
   }
   else
   {
      QO_SGW_DBG_INFO(("Tunnel CB for transId = %d found.", transId));
      QO_SGW_RETVALUE(&(tunEnt->tunnelCb));
   }
}
/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwCreateTunnelTrans
(
QoSgwEgtSapCb *sap, 
U32 transId,
QoSgwTunnelCb **tunnel
)
#else
PUBLIC S16 qoSgwCreateTunnelTrans(sap,transId,tunnel)
      QoSgwEgtSapCb *sap;
      U32 transId;
      QoSgwTunnelCb **tunnel;
#endif
{
   S16 ret = ROK;
   QoSgwTunnEnt *tunEnt = NULLP;

   QO_SGW_TRC2(qoSgwCreateTunnelTrans);

   QO_SGW_ALLOC(&tunEnt, sizeof(QoSgwTunnEnt));
   /* klocworks fix : when allocation failed return */      
   if(tunEnt == NULLP)
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES,
            ("Tunnel Entry CB could not be created.\n"));
      QO_SGW_RETVALUE(RFAILED);
   }
   *tunnel = &(tunEnt->tunnelCb);
   (*tunnel)->transId.pres = PRSNT_NODEF;
   (*tunnel)->transId.val = transId;
   (*tunnel)->locAddr = &(sap->localAddr);

   /*Initialize tunnel 
    * add into the hash list 
    * rest of the initialization by the caller */
   ret = cmHashListInsert(&(sap->tunnPendCbHlCp), (PTR)tunEnt,
         (U8 *)&(*tunnel)->transId.val, sizeof((*tunnel)->transId.val));
   if(ret != ROK)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG,
            ("Failed to insert in hash list.\n"));
      QO_SGW_FREE(tunEnt, sizeof(QoSgwTunnEnt));
   }
   else
   {
      QO_SGW_DBG_INFO(("Tunnel CB created with transId = %d.", transId));
   }
   QO_SGW_RETVALUE(ret);
}


/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwDeleteTunnel
     (
      QoSgwEgtSapCb *sap, 
      QoSgwTunnelCb **tunnel
     )
#else
PUBLIC S16 qoSgwDeleteTunnel(sap,tunnel)
      QoSgwEgtSapCb *sap;
      QoSgwTunnelCb **tunnel;
#endif
{
   S16 ret = ROK;
   QoSgwTunnEnt *tunEnt = NULLP;

   QO_SGW_TRC2(qoSgwDeleteTunnel)

   /* Delete from the hash list */

   /* pointer to tunnel Hash Entry is retreived by pointer multiplication.
    * Refer to the structure. */
   tunEnt = (QoSgwTunnEnt *)((U8*)*tunnel - (2 * (sizeof(CmHashListEnt))));
   if((*tunnel)->transId.pres == PRSNT_NODEF)
   {
      cmHashListDelete(&(sap->tunnPendCbHlCp), (PTR)tunEnt);
      QO_SGW_DBG_INFO(("Tunnel Deleted from Tunnel Pending list "
                     "Interface(%d) teId(%d)",
                     (*tunnel)->intf,(*tunnel)->locTeid));
   }
   else
   {
      cmHashListDelete(&(sap->tunnCbHlCp), (PTR)tunEnt);
      QO_SGW_DBG_INFO(("Tunnel Deleted from Tunnel list.:Interface(%d) "
                     "teId(%d)", (*tunnel)->intf, (*tunnel)->locTeid));
   }
   /* local address and peer address is reference to peerCb
    * hence no need to free */

   if((*tunnel)->msg)
   {
      EgUtilDeAllocGmMsg(&((*tunnel)->msg));
   }

   QO_SGW_FREE(tunEnt, sizeof(QoSgwTunnEnt));
   *tunnel = NULLP;
   QO_SGW_RETVALUE(ret);
}
/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwTransToTeidHashList
    (
     QoSgwEgtSapCb *sap, 
     QoSgwTunnelCb *tunnel
    )
#else
PUBLIC S16 qoSgwTransToTeidHashList(sap,tunnel)
      QoSgwEgtSapCb *sap;
      QoSgwTunnelCb *tunnel;
#endif
{
   S16 ret              = ROK;
   QoSgwTunnEnt *tunEnt = NULLP;

   QO_SGW_TRC2(qoSgwTransToTeidHashList);

   /* pointer to tunnel hash Entry is retreived by pointer multiplication.
    * Refer to the structure. */
   tunEnt = (QoSgwTunnEnt *)((U8*)tunnel - (2 * (sizeof(CmHashListEnt))));
   cmHashListDelete(&(sap->tunnPendCbHlCp), (PTR)tunEnt);
   tunnel->transId.pres = NOTPRSNT;

   QO_SGW_DBG_INFO(("Tunnel CB tranferred from transient list to \
               permanent list."));
   ret = cmHashListInsert(&(sap->tunnCbHlCp), (PTR)tunEnt,
         (U8 *)&tunnel->locTeid, sizeof(tunnel->locTeid));
   if(ret != ROK)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG,
            ("Failed to tunnel insert in hash list.\n"));
   }
   else
   {
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwCreateUeCb
(
QoImsi      *imsi,
QoSgwUeCb   **ueCb
)
#else
PUBLIC S16 qoSgwCreateUeCb(imsi, ueCb)
QoImsi      *imsi;
QoSgwUeCb   **ueCb;
#endif
{
   S16         ret = ROK;
   QoSgwUeEnt  *ueEnt = NULLP;

   QO_SGW_TRC2(qoSgwCreateUeCb)

   QO_SGW_ALLOC(&ueEnt, sizeof(QoSgwUeEnt));
   if (NULLP == ueEnt)
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Failed to allocated memory QoSgwUeEnt"));
      QO_SGW_RETVALUE(RFAILED);
   }

   *ueCb = &(ueEnt->ueCb);
   (*ueCb)->state = QO_SGW_UE_IDLE;
   (*ueCb)->ueIdLen = imsi->len;
   cmMemcpy((U8 *)(*ueCb)->ueId, (U8 *)(imsi->val), imsi->len);

   /* Add into global hash list */
   ret = cmHashListInsert(&(qoSgwCb.ueCbHlCp), (PTR)ueEnt,
                        (U8 *)(*ueCb)->ueId, (*ueCb)->ueIdLen);
   if(ret != ROK)
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, ("Failed to insert in hash list.\n"));
      QO_SGW_FREE(ueEnt, sizeof(QoSgwUeEnt));
      QO_SGW_RETVALUE(ret);
   }
   else
   {
      QO_SGW_DBG_INFO(("UE Cb created."));
   }

   /* qo001.101: Initialize the list of pending GTP events on S1 */
   cmLListInit(&(*ueCb)->pendingOnS1);
   QO_SGW_RETVALUE(ret);
}


/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwGenerateGtpUTeid
     (
      U32 *teid
     )
#else
PUBLIC S16 qoSgwGenerateGtpUTeid(teid)
      U32 *teid;
#endif
{
   QO_SGW_TRC2(qoSgwGenerateGtpUTeid)
#ifdef QO_TODO_SECOND_PHASE
      /* implement rollover of counter */
#endif
   *teid = ++qoSgwCb.gtpuTeidCntr;
   QO_SGW_DBG_INFO(("ETGPU Teid generated = %d.", *teid));
   QO_SGW_RETVALUE(ROK);
}
/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwGenerateCTransId
     (
      U32 *transId
     )
#else
PUBLIC S16 qoSgwGenerateCTransId(transId)
      U32 *transId;
#endif
{
   QO_SGW_TRC2(qoSgwGenerateCTransId)
#ifdef QO_TODO_SECOND_PHASE
      /* implement rollover of counter */
#endif
   *transId = ++qoSgwCb.TransIdCntr;
   QO_SGW_DBG_INFO(("ETGPC TransId generated = %d.", *transId));
   QO_SGW_RETVALUE(ROK);
}


/***************************************************************
 *
 *       Fun:   qoSgwDeletePdn
 *
 *       Desc:  Deletes PDN CB
 *              Delete All bearers CB belonging to PDN
 *              Deletes UE CB if its last PDN of the UE
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwDeletePdn
(
QoSgwPdnCb      *pdnCb,
QoSgwSigType    sigType
)
#else
PUBLIC S16 qoSgwDeletePdn(pdnCb)
QoSgwPdnCb      *pdnCb;
#endif
{
   QoSgwTunnelCb     *s5s8CTun = NULLP;
   EgMsg             *dsReq = NULLP;
   S16               ret = ROK;
   U8                bCnt = 0;
   QoSgwBearerCb     *bearer = NULLP;
   QoSgwUeCb         *ueCb = NULLP;
   U32               ebi = 0;
   CmLListCp         *bearerLst = NULLP;
   CmLList           *tmp = NULLP;
   TknU32            locTeid = {0};
   TknU8             create = {0};

   QO_SGW_TRC2(qoSgwDeletePdn)

   ueCb = pdnCb->ueCb;

   if (NULLP != pdnCb->tmpPdnData)
   {
      bearerLst = &(pdnCb->tmpPdnData->bearers);
      CM_LLIST_FIRST_NODE(bearerLst, tmp);

      while (tmp)
      {
         cmLListDelFrm(bearerLst, tmp);
         QO_SGW_FREE(tmp, sizeof(QoSgwBearerNode));

         CM_LLIST_NEXT_NODE(bearerLst, tmp);
      }

      QO_SGW_FREE(pdnCb->tmpPdnData, sizeof(QoSgwPdnTmpData));
      pdnCb->tmpPdnData = NULLP;
   }

   if (sigType == QO_SGW_SIGNAL_PGW && pdnCb->s5s8CTun->locTeid != 0)
   {
      ret = qoSgwBldDSReq(pdnCb, &dsReq);
      if (ROK == ret)
      {
         s5s8CTun = pdnCb->s5s8CTun;

         QO_SGW_DBG_INFO(("Sending Delete Session Request to PGW (%d)",
                        pdnCb->s5s8CTun->locTeid));
         /* qoSgwSndMsgToPgw(pdnCb, dsReq); */
         locTeid.pres = TRUE;
         locTeid.val = pdnCb->s5s8CTun->locTeid;
         create.pres = NOTPRSNT;
         create.val = FALSE;

         QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                        s5s8CTun->locAddr, s5s8CTun->remAddr, locTeid,
                        s5s8CTun->transId, create, qoSgwCb.t3, dsReq);
      }
   }

   /* Delete control and user plane tunnels */
   for (bCnt = 0; bCnt < QO_SGW_MAX_BEARERS; bCnt++)
   {
      /* check whether this bearer belongs to the PDN */
      bearer = ueCb->bearerTable[bCnt];

      if (bearer != NULLP && bearer->pdnCb == pdnCb &&
         (QO_SGW_CHK_BEARER_MAP(pdnCb->bearerMap, bearer->ebi)))
      {
         ebi = bearer->ebi;
         ret = qoSgwDeleteBearer(bearer);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Deletion of bearer EBI (%d)", ebi));
         }
         else
         {
            QO_SGW_UNSET_BEARER_MAP(pdnCb->bearerMap, ebi);
         }
      }
   }

   if (NULLP != pdnCb->s5s8CTun)
   {
      if ((QO_SGW_TUN_CONNECTED == pdnCb->s5s8CTun->state) && \
          (0 != pdnCb->s5s8CTun->locTeid))
      {
         pdnCb->s5s8CTun->state = QO_SGW_TUN_LCLDEL;
         QO_SGW_DBG_INFO(("Deleting s5/s8 C-Tunnel (%d)",\
               pdnCb->s5s8CTun->locTeid));

         EuLiEgtLclDelReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId, 0,
                        pdnCb->s5s8CTun->locTeid);
      }
      else if (pdnCb->s5s8CTun->transId.pres == PRSNT_NODEF)
      {
         qoSgwDeleteTunnel(qoSgwCb.pgwCSap, &pdnCb->s5s8CTun);
      }
   }
   
   QO_SGW_FREE(pdnCb, sizeof(QoSgwPdnCb));
   ueCb->pdnCnt -= 1;

   if (ueCb->pdnCnt == 0)
   {
      ret = qoSgwDeleteUeCb(ueCb, sigType);
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwDeleteBearer
 *
 *       Desc:  Deletes the Bearer
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_app.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwDeleteBearer
(
QoSgwBearerCb *bearer
)
#else
PUBLIC S16 qoSgwDeleteBearer(bearer)
QoSgwBearerCb *bearer;
#endif
{
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwDeleteBearer)

   if (bearer->s1UTun != NULLP)
   {
      ret = qoSgwSendUTunDel(bearer->s1UTun);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Deletion of S1-U Tunnel with Teid (%d)",
                        bearer->s1UTun->locTeid));
      }
   }

   /* Delete s5/s8 U Tunnel */
   if (bearer->s5s8UTun != NULLP)
   {
      ret = qoSgwSendUTunDel(bearer->s5s8UTun);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Deletion of S5-U Tunnel with Teid (%d)",
                        bearer->s5s8UTun->locTeid));
      }
   }

   if (bearer->s4UTun != NULLP)
   {
      ret = qoSgwSendUTunDel(bearer->s4UTun);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Deletion of S4-U Tunnel with Teid (%d)",
                        bearer->s4UTun->locTeid));
      }
   }


   QO_SGW_RETVALUE(ret);
}


/***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwDeleteUeCb
(
QoSgwUeCb       *ueCb,
QoSgwSigType    sigType
)
#else
PUBLIC S16 qoSgwDeleteUeCb(ueCb)
QoSgwUeCb       *ueCb;
#endif
{
   S16               ret = ROK;
   U8                bCnt = 0;
   U32               s11CTeid;
   U32               s4CTeid;
   QoSgwTunnelCb     *s11Tun = NULLP;
   QoSgwTunnelCb     *s4Tun = NULLP;
   QoSgwBearerCb     *bearer = NULLP;
   U32               pdnCnt = ueCb->pdnCnt;
   QoSgwBearerCb     **bearerTable = ueCb->bearerTable;

   QO_SGW_TRC2(qoSgwDeleteUeCb)

   /* Delete control and user plane tunnels */
   for (bCnt = 0; bCnt < QO_SGW_MAX_BEARERS && pdnCnt > 0; bCnt++)
   {
      bearer = bearerTable[bCnt];
      if (bearer != NULLP && bearer->isDefBearer == TRUE)
      {
         ret = qoSgwDeletePdn(bearer->pdnCb, sigType);
         if (ret != ROK)
         {
            QO_SGW_DBG_ERR(("Deletion of PDN"));
         }
         else
         {
            pdnCnt--;
         }
      }
   }

   if(ueCb->intf == QO_IF_S11_C )
   {
	/* S11 Tunnel */
	/* This is required because, s11 Tun & UE Cb may have got deleted
	 * as a result of previous PDN Delete events */
      if( ueCb->s11Tun )
      {
         s11CTeid = ueCb->s11Tun->locTeid;
         s11Tun = qoSgwFindTunnel(qoSgwCb.mmeCSap, s11CTeid);
         if(s11Tun != NULLP)
         {
            QO_SGW_DBG_INFO(("Deleting the S11 C-Plane Tunnel"));
            s11Tun->state = QO_SGW_TUN_LCLDEL;
            EuLiEgtLclDelReq(&(qoSgwCb.mmeCSap->pst), qoSgwCb.mmeCSap->spId, 0,
                  s11Tun->locTeid);
         }
      }
   }
   else if(ueCb->intf == QO_IF_S4_C )
   {
      /* S4 Tunnel */
      /* This is required because, s4 Tun & UE Cb may have got deleted
       * as a result of previous PDN Delete events */
       if( ueCb->s4Tun )
       {
           s4CTeid = ueCb->s4Tun->locTeid;
           s4Tun = qoSgwFindTunnel(qoSgwCb.sgsnCSap, s4CTeid);
           if(s4Tun != NULLP)
           {
               QO_SGW_DBG_INFO(("Deleting the S4 C-Plane Tunnel"));
               s4Tun->state = QO_SGW_TUN_LCLDEL;
               EuLiEgtLclDelReq(&(qoSgwCb.sgsnCSap->pst), qoSgwCb.sgsnCSap->spId, 0,
                       s4Tun->locTeid);
           }
       }
   }
   
   QO_SGW_RETVALUE(ret);
}


PUBLIC Txt *qoSprintf(Txt *P_fmt, ...)
{
   static Txt V_traceMsg[1024];
   va_list L_args;

   va_start(L_args, P_fmt);
   vsprintf(V_traceMsg, P_fmt, L_args);
   va_end(L_args);
   return V_traceMsg;
}


/***********************************************************************
 *
 *       Fun:   qoSgwSendUTunDel
 *
 *       Desc:  Sends event to GTP-U layer to delete the tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***********************************************************************/
#ifdef ANSI
PRIVATE S16 qoSgwSendUTunDel
(
QoSgwTunnelCb     *tun        /* Tunnel to be deleted */
)
#else
PRIVATE S16 qoSgwSendUTunDel(tun)
QoSgwTunnelCb     *tun;        /* Tunnel to be deleted */
#endif
{
   S16               ret = ROK;
   EgtUEvnt          *eguEvt = NULLP;
   CmTptAddr         *tptAddr = NULLP;

   QO_SGW_TRC2(qoSgwSendUTunDel)

   if ((tun->state != QO_SGW_TUN_CONNECTED) &&
       (tun->state != QO_SGW_TUN_DISCONNECTED))
   {
      qoSgwDeleteUTunnel(tun);
      QO_SGW_RETVALUE(ROK);
   }

   tun->state = QO_SGW_TUN_LCLDEL;

   /* Delete context at GTP-U if exists. */
   QO_SGW_ALLOC((&eguEvt), sizeof(EgtUEvnt));
   if (eguEvt == NULLP)
   {
      QO_SGW_DBG_ERR(("Memory allocation for EgtUEvnt, FAILED"));
      QO_SGW_RETVALUE(RFAILED);
   }

   eguEvt->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_DEL;
   eguEvt->u.tnlMgmt.teId = tun->locTeid;

   /* Fill Local Address */
   tptAddr = &(eguEvt->u.tnlMgmt.srcAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tun->locAddr->u.ipv4TptAddr.port;
   tptAddr->u.ipv4TptAddr.address = tun->locAddr->u.ipv4TptAddr.address;

   /* Fill Remote Address */
   tptAddr = &((eguEvt)->u.tnlMgmt.dstIpAddr);
   tptAddr->type = CM_NETADDR_IPV4;
   tptAddr->u.ipv4TptAddr.port = tun->remAddr->u.ipv4TptAddr.port;
   tptAddr->u.ipv4TptAddr.address = tun->remAddr->u.ipv4TptAddr.address;

   ret = QoSgwLiEgtEguTnlMgmtReq(&(qoSgwCb.egUSap->pst),
                                 qoSgwCb.egUSap->spId, eguEvt);

   QO_SGW_RETVALUE(ret);
}

/*
 *
 *       Fun:   qoSgwFindUeCbUsingImsi
 *
 *       Desc:  Invoked by the  application to get UE cb using IMSI
 *              received in Create SessioN Request
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
PUBLIC S16 qoSgwFindUeCbUsingImsi
(
EgMsg             *egMsg,    /* gtp-c message (IN)   */
QoImsi            *imsi,     /* imsi (OUT)           */
U32               *cause,    /* cause value (OUT)    */
QoSgwUeCb         **ueCb     /* UE CB(OUT)           */
)
#else
PUBLIC S16 qoSgwFindUeCbUsingImsi(egMsg, imsi, cause, ueCb)
EgMsg             *egMsg;    /* gtp-c message (IN)   */
QoImsi            *imsi;    /* imsi (OUT)           */
U32               *cause;     /* cause value (OUT)    */
QoSgwUeCb         **ueCb;     /* UE CB(OUT)           */
#endif
{
   QoSgwUeEnt     *ueEnt = NULLP;
   EgIe           *egIe = NULLP;
   U16            ret = ROK;
   U16            idx = 0;

   QO_SGW_TRC2(qoSgwFindUeCbUsingImsi)

   /* Get the IMSI */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_IMSI_IETYPE, 0);
   if (ROK != ret || egIe == NULLP)
   {
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
      QO_SGW_DBG_ERR(("IMSI Missing/Parsing-Failure In Gtpc Msg"));
   }
   else
   {
      imsi->len = egIe->t.valStr32.length;
#if 0
      cmMemcpy(imsi->val, (U8 *)(egIe->t.valStr32.val), imsi->len);
#else
      for(idx = 0; idx < imsi->len; idx ++)
      {
         imsi->val[idx] = egIe->t.valStr32.val[idx] - '0';
      }
#endif

      ret = cmHashListFind(&(qoSgwCb.ueCbHlCp), (U8 *)imsi->val, imsi->len, 0,
                          (PTR *)&ueEnt);
      if(ret != ROK)
      {
         QO_SGW_DBG_INFO(("UeCb Not Found."));
         *ueCb = NULLP;
      }
      else
      {
         QO_SGW_DBG_INFO(("UeCb Found."));
         *ueCb = &(ueEnt->ueCb);
         ret = ROK;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/*
 *
 *       Fun:   qoSgwCreateUeCbUsingImsi
 *
 *       Desc:  Invoked by the  application to get/create UE cb using IMSI
 *              received in Create SessioN Request
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
PUBLIC S16 qoSgwCreateUeCbUsingImsi 
(
EgMsg             *egMsg,    /* gtp-c message (IN)   */
QoImsi            *imsi,     /* imsi (IN)           */
U32               *cause,    /* cause value (OUT)    */
QoSgwUeCb         **ueCb     /* UE CB(OUT)           */
)
#else
PUBLIC S16 qoSgwCreateUeCbUsingImsi(egMsg, imsi, cause, ueCb)
EgMsg             *egMsg;    /* gtp-c message (IN)   */
QoImsi            *imsi;    /* imsi (IN)           */
U32               *cause;     /* cause value (OUT)    */
QoSgwUeCb         **ueCb;     /* UE CB(OUT)           */
#endif
{
   U16            ret = ROK;

   QO_SGW_TRC2(qoSgwCreateUeCbUsingImsi)

   ret = qoSgwCreateUeCb(imsi, ueCb);
   if(ret != ROK)
   {
      *cause = EGT_GTPC_CAUSE_SYS_FAIL;
      QO_SGW_DBG_ERR(("Unable To Create UE Control Block"));
   }
   else
   {
      (*ueCb)->state = QO_SGW_UE_IDLE;
   }

   QO_SGW_RETVALUE(ret);
}


/*
 *
 *       Fun:   qoSgwCreateUTunnel
 *
 *       Desc:  Creates User Plance tunnels
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwCreateUTunnel
(
QoSgwPdnCb              *pdnCb,     /* PDN Control block */
QoSgwTunnelCb           **uTun,     /* User Plane Block */
QoInterfaceType         ifType      /* interface type */
)
#else
PUBLIC S16 qoSgwCreateUTunnel(pdnCb, uTun, ifType)
QoSgwPdnCb              *pdnCb;     /* PDN Control block */
QoSgwTunnelCb           **uTun;     /* User Plane Block */
QoInterfaceType         ifType;     /* interface type */
#endif
{
   S16            ret = ROK;
   U32            teid = 0;
   
   QO_SGW_TRC2(qoSgwCreateUTunnel);

   qoSgwGenerateGtpUTeid(&teid);

   ret = qoSgwCreateTunnel(qoSgwCb.egUSap, teid, uTun);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Creating of U Plane Tunnel with TEID (%d), FAILED",
                     teid));
   }
   else
   {
      (*uTun)->plane = QO_USER;
      (*uTun)->ueCb = pdnCb->ueCb;
      (*uTun)->pdnCb = pdnCb;
      (*uTun)->state = QO_SGW_TUN_PROGRESS; 
      (*uTun)->intf = ifType;
   }

   QO_SGW_RETVALUE(ret);
}

/*
 *
 *       Fun:   qoSgwDeleteUTunnel
 *
 *       Desc:  Deletes User Plance tunnels
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwDeleteUTunnel
(
QoSgwTunnelCb           *uTun     /* User Plane Block */
)
#else
PUBLIC S16 qoSgwDeleteUTunnel(uTun)
QoSgwTunnelCb           *uTun;     /* User Plane Block */
#endif
{
   S16 ret = RFAILED;
   QO_SGW_TRC2(qoSgwDeleteUTunnel);
   ret = qoSgwDeleteTunnel(qoSgwCb.egUSap, &uTun);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Deleting Of U-Plane Tunnel, FAILED"));
   }
   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 *
 *       Fun:   qoSgwEguTunnelModReq
 *
 *       Desc:  Sends an event to egu for Modifying a U Tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwEguTunnelModReq
(
QoSgwBearerCb           *bearer,    /* Tunnel Control block */
EgMsg                   *egMsg,     /* gtp-c message (IN)   */
EgtIntfEnt              ifType      /* Interface type (IN)   */
)
#else
PUBLIC S16 qoSgwEguTunnelModReq(bearer, egMsg, ifType)
QoSgwBearerCb           *bearer;    /* Tunnel Control block */
EgMsg                   *egMsg;     /* gtp-c message (IN)   */
EgtIntfEnt              ifType;     /* Interface type (IN)   */
#endif
{
   EgtUEvnt             *eguEvt = NULLP;
   QoSgwTunnelCb        *uTun = NULLP;
   U16                  ret = ROK;

   QO_SGW_TRC2(qoSgwEguTunnelModReq)

   /* First create the local context for default s5s8 tunnel in EGTP layer
    * Send the Create Session Response after receiving the Confirm */
   QO_SGW_ALLOC((&eguEvt), sizeof(EgtUEvnt));
   if(eguEvt == NULLP)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For EgtUEvnt"));
      QO_SGW_RETVALUE(RFAILED);
   }

   if (ifType == EGT_INT_S5_S8_SGW_GTP_U)
   {
      uTun = bearer->s5s8UTun;
      QO_SGW_DBG_INFO(("Modify EGTP-U Tunnel For S5/S8-U Interface With "
                     "Tunnel %ld", uTun->locTeid));
   }
   else if (ifType == EGT_INT_S1_U_SGW_GTP_U)
   {
      uTun = bearer->s1UTun;
      QO_SGW_DBG_INFO(("Modify EGTP-U Tunnel For S1-U Interface With "
                     "Tunnel %ld", uTun->locTeid));
   }
   else if (ifType == EGT_INT_S4_SGW_GTP_U)
   {
      uTun = bearer->s4UTun;
      QO_SGW_DBG_INFO(("Modify EGTP-U Tunnel For S4-U Interface With "
                     "Tunnel %ld", uTun->locTeid));
   }


   if(NULLP != uTun)
   {   
      uTun->msg = egMsg;
      eguEvt->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_MOD;
      eguEvt->u.tnlMgmt.teId = uTun->locTeid;
      eguEvt->u.tnlMgmt.reOrderPres = FALSE;
      eguEvt->u.tnlMgmt.intfType = ifType;
      eguEvt->u.tnlMgmt.expSeqNmb = 1000;
      eguEvt->u.tnlMgmt.maxTPduSize = QO_SGW_DFLT_MAXTPDUSZ;
      eguEvt->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;

      cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.srcAddr),
            (U8 *)(uTun->locAddr), sizeof(CmTptAddr));
      cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.dstIpAddr),
            (U8 *)uTun->newRemAddr, sizeof(CmTptAddr));
      cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.frmDstIpAddr),
            (U8 *)uTun->remAddr, sizeof(CmTptAddr));

      QoSgwLiEgtEguTnlMgmtReq(&(qoSgwCb.egUSap->pst),
            qoSgwCb.egUSap->spId, eguEvt);
   }
   else
   {
      QO_SGW_DBG_ERR(("EGTP-U Tunnel is NULL"));
      QO_SGW_FREE((eguEvt), sizeof(EgtUEvnt));
      QO_SGW_RETVALUE(RFAILED);
   }   



   QO_SGW_RETVALUE(ret);
}

/*********************************************************************
 * 
 *        Fun:   qoSgwEgcTunnelModReq
 * 
 *        Desc:  sends a changed IP  address to the EGTP to update the peer. 
 * 
 *        Ret:   ROK/RFAILED
 * 
 *        Notes: None
 * 
 *        File:  qo_utl.c
 *
 **********************************************************************/
#ifdef ANSI
PUBLIC S16 QoSgwEgcTunnelModReq
(
QoSgwUeCb               *ueCb,    /* UE Control block */
EgtIntfEnt              ifType      /* Interface type (IN)   */
)
#else
PUBLIC S16 QoSgwEgcTunnelModReq(ueCb, ifType)
QoSgwUeCb               *ueCb,    /* UE Control block */
EgtIntfEnt              ifType;      /* Interface type (IN)   */
#endif
{
    Pst        *pst;
    SpId       spId;
    U32        transId;
    U32        teid;
    CmTptAddr  oldDstIpAddr;
    CmTptAddr  newDstIpAddr;
    U16        ret = ROK;

    QO_SGW_TRC2(QoSgwEgcTunnelModReq);


    if(ifType  == EGT_INT_S4_SGSN_GTP_C)
    {
       teid = ueCb->s4Tun->locTeid;
       oldDstIpAddr = *(ueCb->s11Tun->remAddr);
       newDstIpAddr = *(ueCb->s4Tun->newRemAddr); 
       spId = qoSgwCb.sgsnCSap->spId;
       pst = &qoSgwCb.sgsnCSap->pst;
       transId = ueCb->s4Tun->transId.val;
    }
    else if(ifType  == EGT_INT_S11_MME_GTP_C)
    {
       teid = ueCb->s11Tun->locTeid;
       oldDstIpAddr = *(ueCb->s4Tun->remAddr);
       newDstIpAddr = *(ueCb->s11Tun->newRemAddr); 
       spId = qoSgwCb.mmeCSap->spId;
       pst = &qoSgwCb.mmeCSap->pst;
       transId = ueCb->s11Tun->transId.val;
    }
    else
    {
       QO_SGW_DBG_INFO(("Interface type is wrong \n"));
       QO_SGW_RETVALUE(RFAILED)
    }


   /* calling egtp c tunnel modify request */
   ret = QoSgwLiEgtEgcTnlModreq(pst, spId, transId, teid, oldDstIpAddr, newDstIpAddr);
   QO_SGW_RETVALUE(ret)
}

/*********************************************************************
 *
 *       Fun:   qoSgwEguTunnelAddReq
 *
 *       Desc:  Sends an event to egu for creating a U Tunnel
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwEguTunnelAddReq
(
QoSgwBearerCb           *bearer,    /* Tunnel Control block */
EgMsg                   *egMsg,     /* gtp-c message (IN)   */
EgtIntfEnt              ifType      /* Interface type (IN)   */
)
#else
PUBLIC S16 qoSgwEguTunnelAddReq(bearer, egMsg, ifType)
QoSgwBearerCb           *bearer;    /* Tunnel Control block */
EgMsg                   *egMsg;     /* gtp-c message (IN)   */
EgtIntfEnt              ifType;     /* Interface type (IN)   */
#endif
{
   EgtUEvnt             *eguEvt = NULLP;
   QoSgwTunnelCb        *uTun = NULLP;
   U16                  ret = ROK;

   QO_SGW_TRC2(SgwEguTunnelAddReq)

   /* First create the local context for default s5s8 tunnel in EGTP layer
    * Send the Create Session Response after receiving the Confirm */
   QO_SGW_ALLOC((&eguEvt), sizeof(EgtUEvnt));
   if(eguEvt == NULLP)
   {
      QO_SGW_DBG_ERR(("Memory Allocation For EgtUEvnt, "
                     "FAILED"));
      QO_SGW_RETVALUE(RFAILED);
   }

   if (ifType == EGT_INT_S5_S8_SGW_GTP_U)
   {
      uTun = bearer->s5s8UTun;
   }
   else if (ifType == EGT_INT_S1_U_SGW_GTP_U)
   {
      uTun = bearer->s1UTun;
   }
   else if (ifType == EGT_INT_S4_SGW_GTP_U)
   {
      uTun = bearer->s4UTun;
   }
   else
   {
      QO_SGW_DBG_ERR(("unknown interface type"));
      QO_SGW_RETVALUE(RFAILED);
   }
   if(uTun != NULLP)
   {
     eguEvt->u.tnlMgmt.action = EGT_TMGMT_REQTYPE_ADD;
     eguEvt->u.tnlMgmt.reOrderPres = FALSE;
     eguEvt->u.tnlMgmt.teId = uTun->locTeid;
     eguEvt->u.tnlMgmt.intfType = ifType;
     eguEvt->u.tnlMgmt.expSeqNmb = 1000;
     eguEvt->u.tnlMgmt.maxTPduSize = QO_SGW_DFLT_MAXTPDUSZ;
     eguEvt->u.tnlMgmt.reordAlgo = EG_REORDER_TYPE_1;
     uTun->msg = egMsg;

     cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.srcAddr),
            (U8 *)(uTun->locAddr), sizeof(CmTptAddr));
     cmMemcpy((U8 *)&(eguEvt->u.tnlMgmt.dstIpAddr),
            (U8 *)(uTun->remAddr), sizeof(CmTptAddr));

     QO_SGW_DBG_INFO (("Adding A New EGTP-U Tunnel For S5/S8 Interface With "
                     "Tunnel %ld", eguEvt->u.tnlMgmt.teId));

     QoSgwLiEgtEguTnlMgmtReq(&(qoSgwCb.egUSap->pst),
                           qoSgwCb.egUSap->spId, eguEvt);
   }
   QO_SGW_RETVALUE(ret);

}


/*********************************************************************
 *
 *       Fun:   qoSgwGetPdnCb
 *
 *       Desc:  Gets PDN Control block using GTP-C message
 *              Parse the bearer to get the EBI, from the bearer details
 *              Get the PDN which the bearer belongs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwGetPdnCb
(
QoSgwUeCb               *ueCb,      /* UE control block */
U32                     transId,    /* transaction id */
QoSgwPdnCb              **pdnCb      /* PDN Control block */
)
#else
PUBLIC S16 qoSgwGetPdnCb(ueCb, transId, pdnCb)
QoSgwUeCb               *ueCb;      /* UE control block */
U32                     transId,    /* transaction id */
QoSgwPdnCb              **pdnCb;     /* PDN Control block */
#endif
{
   S16                  ret = RFAILED;
   U8                   bCnt = 0;

   QO_SGW_TRC2(qoSgwGetPdnCb);

   for (bCnt = 0; bCnt < QO_SGW_MAX_BEARERS; bCnt++)
   {
      if (ueCb->bearerTable[bCnt] != NULLP)
      {
         if (ueCb->bearerTable[bCnt]->pdnCb->transId == transId)
         {
            QO_SGW_DBG_INFO(("Finding of PDN Control Block, SUCCESS"));
            *pdnCb = ueCb->bearerTable[bCnt]->pdnCb;
            ret = ROK;
            break;
         }
      }
   }
   
   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwGetPdnCbUsingTransId
 *
 *       Desc:  Gets PDN Control block by iterating through the bearers.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwGetPdnCbUsingTransId
(
QoSgwUeCb               *ueCb,      /* UE control block */
U32                     transId,    /* Transaction ID */
QoSgwPdnCb              **pdnCb     /* PDN Control block */
)
#else
PUBLIC S16 qoSgwGetPdnCbUsingTransId(pdnCb)
QoSgwUeCb               *ueCb,      /* UE control block */
U32                     transId;    /* Transaction ID */
QoSgwPdnCb              **pdnCb;     /* PDN Control block */
#endif
{
   S16                  ret = RFAILED;
   U8                   bCnt = 0;

   QO_SGW_TRC2(qoSgwGetPdnCbUsingTransId)

   for (bCnt = 0; bCnt < QO_SGW_MAX_BEARERS; bCnt++)
   {
      if (ueCb->bearerTable[bCnt] != NULLP)
      {
         if (ueCb->bearerTable[bCnt]->pdnCb->transId == transId)
         {
            QO_SGW_DBG_INFO(("Finding of PDN Control Block, SUCCESS"));
            *pdnCb = ueCb->bearerTable[bCnt]->pdnCb;
            ret = ROK;
            break;
         }
      }
   }
   
   QO_SGW_RETVALUE(ret);
}



/*********************************************************************
 *
 *       Fun:   qoSgwGetPdnCbUsingTmpBearer
 *
 *       Desc:  Gets PDN Control block by iterating through tmp bearers.
 *              and matchin the tunnel IDs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwGetPdnCbUsingTmpBearer
(
QoSgwUeCb               *ueCb,      /* UE control block */
EgMsg                   *egMsg,     /* gtp-c msg */
QoSgwPdnCb              **pdnCb      /* PDN Control block */
)
#else
PUBLIC S16 qoSgwGetPdnCbUsingTmpBearer(pdnCb)
QoSgwUeCb               *ueCb;      /* UE control block */
EgMsg                   *egMsg;     /* gtp-c msg */
QoSgwPdnCb              **pdnCb;     /* PDN Control block */
#endif
{
   S16                  ret = RFAILED;
   U32                  bCnt = 0;
   EgIe                 *egIe = NULLP;
   QoSgwBearerCb        *bearer = NULLP;
   CmLList              *tmp = NULLP;

   QO_SGW_TRC2(qoSgwGetPdnCbUsingTmpBearer);

   *pdnCb = NULLP;

   /* get SGW S1-U F-TEID */ 
   ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 1, &egIe);
   if (ret != ROK || NULLP == egIe)
   {
      QO_SGW_RETVALUE(RFAILED);
   }

   ret = RFAILED;
   for (bCnt = 0; bCnt < QO_SGW_MAX_BEARERS; bCnt++)
   {
      bearer = ueCb->bearerTable[bCnt];
      if (NULL != bearer && NULLP != bearer->pdnCb->tmpPdnData &&
         bearer->pdnCb->tmpPdnData->bearers.count > 0)
      {
         CM_LLIST_FIRST_NODE(&(bearer->pdnCb->tmpPdnData->bearers), tmp);
         if ((((QoSgwBearerNode *)tmp)->s1UTun->locTeid == egIe->t.fTeid.teid))
         {
            *pdnCb = bearer->pdnCb;
            ret = ROK;
            break;
         }

         tmp = NULLP;
      }
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwGetPdnCbUsingTmpBearer */



/*********************************************************************
 *
 *       Fun:   qoSgwGetPdnCbUsingEgMsg
 *
 *       Desc:  Gets PDN Control block by iterating through the bearers.
 *              and matchin the EBI
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwGetPdnCbUsingEgMsg
(
QoSgwUeCb               *ueCb,      /* UE control block */
EgMsg                   *egMsg,     /* gtp-c msg */
QoSgwPdnCb              **pdnCb      /* PDN Control block */
)
#else
PUBLIC S16 qoSgwGetPdnCbUsingEgMsg(pdnCb)
QoSgwUeCb               *ueCb;      /* UE control block */
EgMsg                   *egMsg;     /* gtp-c msg */
QoSgwPdnCb              **pdnCb;     /* PDN Control block */
#endif
{
   S16                  ret = RFAILED;
   U32                  bInd = 0;
   EgIe                 *egIe = NULLP;

   QO_SGW_TRC2(qoSgwGetPdnCbUsingEgMsg);

   switch (egMsg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_DS_REQ:
      case EGT_GTPC_MSG_BR_CMND:
      {
         ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_EBI_IETYPE, 0);
         break;
      }

      case EGT_GTPC_MSG_MB_REQ:
      {
         /* bearer context to be modified */
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
         if (ret != ROK || NULLP == egIe)
         {
            /* bearer context to be deleted */
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 1, 1, EG_EBI_IETYPE, 0, &egIe);
         }

         break;
      }

      case EGT_GTPC_MSG_DB_RSP:
      {
         /* from LBI */
         ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_EBI_IETYPE, 0);
         if (ret != ROK || NULLP == egIe)
         {
            /* bearer contexts */
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
         }

         break;
      }

      case EGT_GTPC_MSG_UB_RSP:
      case EGT_GTPC_MSG_DB_CMND:
      case EGT_GTPC_MSG_MB_CMND:
      {
         /* bearer contexts */
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
         break;
      }

      default:
      {
         QO_SGW_DBG_ERR(("Invalid Message Type"));
         break;
      }
   }

   if (ret == ROK)
   {
      bInd = QO_SGW_BEARER_IND(egIe->t.valUnSgnd8);
     if ((bInd < QO_SGW_MAX_BEARERS) && (bInd >= 0))
     {		    
        if (ueCb->bearerTable[bInd] != NULLP)  
        {
           *pdnCb = ueCb->bearerTable[bInd]->pdnCb;
           ret = ROK;
        }
        else
        {
          ret = RFAILED;
        }
     }
     else
     {
         ret = RFAILED;
      
     }
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwCreatePdn
 *
 *       Desc:  Gets PDN Control block by iterating through the bearers.
 *              and matchin the EBI
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwCreatePdn
(
QoSgwPdnCb              **pdnCb      /* PDN Control block */
)
#else
PUBLIC S16 qoSgwCreatePdn(pdnCb)
QoSgwPdnCb              **pdnCb;     /* PDN Control block */
#endif
{
   S16                  ret = ROK;

   QO_SGW_TRC2(qoSgwCreatePdn);

   QO_SGW_ALLOC(pdnCb, sizeof(QoSgwPdnCb));
   if (NULLP == *pdnCb)
   {
      ret = RFAILED;
      QO_SGW_DBG_ERR(("Memory allocation failure for QoSgwPdnCb"));
   }
   else
   {
      (*pdnCb)->state = QO_SGW_PDN_IDLE;
   }

   QO_SGW_RETVALUE(ret);
}


/*********************************************************************
 *
 *       Fun:   qoSgwSndMsgToPgw
 *
 *       Desc:  Sends a gtp-c message to PGW via egt layer
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_utl.c
 *
 *********************************************************************/
#ifdef ANSI
PUBLIC Void qoSgwSndMsgToPgw
(
QoSgwPdnCb              *pdnCb,     /* PDN Control block */
EgMsg                   *egMsg
)
#else
PUBLIC Void qoSgwSndMsgToPgw(pdnCb, egMsg)
QoSgwPdnCb              *pdnCb;     /* PDN Control block */
EgMsg                   *egMsg;
#endif
{
   TknU32         locTeid = {0};
   TknU8          create = {0};
   QoSgwTunnelCb  *s5s8CTun = NULLP;

   QO_SGW_TRC2(qoSgwSndMsgToPgw);

   s5s8CTun = pdnCb->s5s8CTun;
   s5s8CTun->transId.val = egMsg->msgHdr.seqNumber;
   locTeid.pres = TRUE;
   locTeid.val = pdnCb->s5s8CTun->locTeid;
   create.pres = PRSNT_NODEF;
   create.val = FALSE;

   /* Send to EGT Layer */
   QoSgwLiEgtSigReq(&(qoSgwCb.pgwCSap->pst), qoSgwCb.pgwCSap->spId,
                  s5s8CTun->locAddr, s5s8CTun->remAddr, locTeid,
                  s5s8CTun->transId, create, qoSgwCb.t3, egMsg);

   QO_SGW_RETVOID;
}

#endif

/*******************************************************/


/********************************************************************30**

         End of file:     qo_utl.c@@/main/1 - Thu Jul  8 21:22:21 2010

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
/main/1      -          sp                  1.LTE-CNE:SGW Initial Release
/main/1      qo001.101  rp                  1.Added support for dedicated tunels
/main/1      qo003.101  pmacharla           1.eGTP 2.1 Release upgrade under EG_REL_930 flag
*********************************************************************91*/
