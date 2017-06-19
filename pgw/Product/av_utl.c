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

     Name:    Trillium LTE PGW Application

     Type:    C Include file

     Desc:    This file contains utility functions for CNE-LTE

     File:    jg_utl.c

     Sid:      av_utl.c@@/main/1 - Tue Aug 17 18:54:35 2010

     Prg:       sp
*********************************************************************21*/

/* header include files -- defines (.h) */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */

#ifndef AV_PERF
#include <pcap.h>
#endif

#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "eg_util.h"
/* av004.101 changes */
#ifndef EG_REL_930
#include "eg_dbs11.h"
#else
#include "eg_dbutl.h"      /* eGTP Message Dictionary related macros */
#endif /* EG_REL_930 */

#include "leg.h"
#include "egt.h"

#include "lav.h"
#include "av.h"

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
#include "leg.x"
#include "egt.x"
#include "egt_util.x"
#include "eg_util.x"

#include "lav.x"
#include "av.x"            /* CNE Application structures     */

PRIVATE U32 gAvPgwTransId = 1;

/************************ Prototypes **********************************/

PUBLIC S16 avUtlEgtBuildEgIeHeader ARGS((EgIe *egIe, U8 ieType, U8 ieInst,
      U8 ieCr, U8 dataType));

PRIVATE Void avPgwDelBearerPktFilters ARGS((
AvPgwUeRouteCb    *pUeRouteCb,
U8                ebi
));

/**********************************************************************/

/* av001.101: Removed unused functions */
#ifdef ANSI
PUBLIC S16 avUtlEgtAddApnRstr
(
EgMsg            *egMsg,
EgIeLevelInfo    *egLvlInfo,
U8               *apnRstr,
U8               inst
)
#else
PUBLIC S16 avUtlEgtAddApnRstr(egMsg, egLvlInfo, apn, inst)
EgMsg            *egMsg;
EgIeLevelInfo    *egLvlInfo;
U8               *apnRstr;
U8               inst;
#endif
{
   EgIe   egIe;

   AV_PGW_SND_TRC2(avUtlEgtAddApnRstr)

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe,EG_APN_RESTRICT_IETYPE, inst, 0, EG_UNSIGNED_8);
#else
   egUtlEgtBuildEgIeHeader(&egIe,EG_APN_RESTRICT_IETYPE, inst, EG_UNSIGNED_8);
#endif /* EG_REL_930 */

   egIe.t.valUnSgnd8 = *apnRstr;

   if(EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* End of function */


/* av001.101: Removed unused functions */


#ifdef ANSI
PUBLIC S16  avUtlEgtAddChargIdIe
(
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         chrgngId
)
#else
PUBLIC S16 avUtlEgtAddChargIdIe(egMsg, egLvlInfo, inst, chrgngId)
EgMsg      *egMsg;
EgIeLevelInfo *egLvlInfo;
U8         inst;
U8         chrgngId;
#endif /* ANSI */
{
   EgIe   egIe;

   AV_PGW_SND_TRC2(avUtlEgtAddChargIdIe)

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe, EG_CHARGING_ID_IETYPE, inst, 0, EG_UNSIGNED_32);
#else
   egUtlEgtBuildEgIeHeader(&egIe, EG_CHARGING_ID_IETYPE, inst, EG_UNSIGNED_32);
#endif /* EG_REL_930 */

   egIe.t.valUnSgnd16 = chrgngId;

   if(EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe) != ROK)
   {
      RETVALUE(RFAILED);
   }
    /*Added KlockWorks Comment: Adding RETVALE statement */
    RETVALUE(ROK);

}  /* avUtlEgtAddChargIdIe  */




/*
 *
 *       Fun:   UeCb Finding in hash list
 *
 *       Desc:  Invoked by the application 
 *             
 *
 *       Ret:   ROK   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwFndUeCb 
(
AvPgwUeCb     **ueCb,
AvImsi        *imsi
)
#else
PUBLIC S16 avPgwFndUeCb(ueCb, imsi)
AvPgwUeCb     **ueCb;
AvImsi        *imsi;
#endif
{
   S16  ret;

   AV_PGW_SND_TRC2(avPgwFndUeCb)

   ret = cmHashListFind(&avPgwCb.ueCbHlst, (U8 *)(imsi->buf), (U16)imsi->len,
                        (U16)0, (PTR *)ueCb);
   if(ROK != ret)
   {
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Failed to get ueCb"));
      ueCb = NULLP;
   }

   AV_PGW_SND_RETVALUE(ret);

} /* end of avPgwFndUeCb */


/*
 *
 *       Fun:   UeCb Insertion into hash list
 *
 *       Desc:  Invoked by the application 
 *             
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwAddUeCb
(
AvImsi         *imsi,
AvPgwUeCb      **ueCb
)
#else
PUBLIC S16 avPgwAddUeCb(imsi, ueCb)
AvImsi         *imsi;
AvPgwUeCb      **ueCb;
#endif
{
   S16  ret;

   AV_PGW_SND_TRC2(avPgwAddUeCb)
      
   /* Allocate a new ueCb */
   AV_PDNGW_SENDTSK_ALLOC((*ueCb),sizeof(AvPgwUeCb)); 
   if(NULLP == *ueCb)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Memory Allocation for "
                           "avPgwAddUeCb"));
      AV_PGW_SND_RETVALUE(RFAILED);   
   }

   (*ueCb)->imsi = *imsi;
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "ADDING UE: %x%x%x%x%x%x%x%x",
                     imsi->buf[0], imsi->buf[1], imsi->buf[2], imsi->buf[3],
                     imsi->buf[4], imsi->buf[5], imsi->buf[6], imsi->buf[7]));

   ret = cmHashListInsert(&avPgwCb.ueCbHlst, (PTR)(*ueCb), (U8 *)imsi->buf,
                        (U16)imsi->len);
   if(ROK!= ret)
   { 
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                 "Inserting UE Cb in Hash Table"));
      AV_PDNGW_SENDTSK_FREE(*ueCb, sizeof(AvPgwUeCb)); 
      AV_PGW_SND_RETVALUE(RFAILED);   
   }

   AV_PGW_SND_RETVALUE(ret);

} /* end of avPgwAddUeCb */

/*
 *
 *       Fun:   Deleting Beaere CB
 *
 *       Desc:  Invoked by the application 
 *             
 *
 *       Ret:   ROK  
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwDelBearerCb
(
AvPgwBearerCb     *bearer
)
#else
PUBLIC S16 avPgwDelBearerCb(bearer)
AvPgwBearerCb     *bearer;
#endif
{
   S16            ret = ROK;
   AvPgwUeCb      *ueCb = NULLP;

   AV_PGW_SND_TRC2(avPgwDelBearerCb)

   ueCb = bearer->pdnCb->ueCb;

   if (bearer->isDefBearer == TRUE)
   {
      bearer->pdnCb->s5s8CTun->state = AV_PGW_EGTPC_TUNNEL_PENDING;
   }

   ret = avPgwEgtUTunnelMachine(bearer->s5s8UTun,
                              bearer->s5s8UTun->localTeIdKey,
                              EGT_TMGMT_REQTYPE_DEL, NULLP);
   if (ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Deletion of Bearer"));
   }
   else
   {
      /* Memory associated with this bearer will be deleted when tunnel
       * deletion confirmation is received */
      ueCb->numBearers -= 1;
   }

   AV_PGW_SND_RETVALUE(ret);
}


/*
 *
 *       Fun:   Deleting PDN CB
 *
 *       Desc:  Invoked by the application 
 *             
 *
 *       Ret:   ROK  
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwDelPdnCb
(
AvPgwPdnCb     *pdnCb
)
#else
PUBLIC S16 avPgwDelPdnCb(pdnCb)
AvPgwPdnCb     *pdnCb;
#endif
{
   U32            bCnt = 0;
   S16            ret = ROK;
   AvPgwBearerCb  *bearer = NULLP;
   AvPgwBearerCb  *defBearer = NULLP;
   AvPgwUeCb      *ueCb = NULLP;

   AV_PGW_SND_TRC2(avPgwDelPdnCb)

   ueCb = pdnCb->ueCb;
   avPgwUtlDelPendingTunLst(&(pdnCb->pendingTunLst));

   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      bearer = ueCb->bearerTable[bCnt];
      if (bearer != NULLP && bearer->pdnCb == pdnCb)
      {
         if (bearer->isDefBearer == FALSE)
         {
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                              "Deleting Dedicated Bearer"));
            avPgwDelBearerCb(bearer);
         }
         else
         {
            defBearer = bearer;
         }
      }
   }

   if (defBearer != NULLP)
   {
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Deleting Default Bearer"));
      avPgwDelBearerCb(defBearer);
   }

   AV_PGW_SND_RETVALUE(ret);
}


/*
 *
 *       Fun:   Deleting UeCb in hash list
 *
 *       Desc:  Invoked by the application 
 *             
 *
 *       Ret:   ROK  
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwDelUeCb
(
AvPgwUeCb    **ueCb
)
#else
PUBLIC S16 avPgwDelUeCb(ueCb)
AvPgwUeCb    **ueCb;
#endif
{
   U32            bCnt = 0;
   S16            ret = ROK;
   AvPgwBearerCb  *bearer = NULLP;

   AV_PGW_SND_TRC2(avPgwDelUeCb)

   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      bearer = (*ueCb)->bearerTable[bCnt];
      if (bearer != NULLP)
      {
         if (bearer->isDefBearer == TRUE && bearer->pdnCb != NULLP)
         {
            avPgwDelPdnCb(bearer->pdnCb);
         }
      }
   }
   
   ret = cmHashListDelete(&avPgwCb.ueCbHlst, (PTR)*ueCb);
   if(ROK != ret)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Deletion of UE CB"));
   }
   else
   {
      AV_PDNGW_SENDTSK_FREE(*ueCb, sizeof(AvPgwUeCb)); 
   }

   AV_PGW_SND_RETVALUE(ret);
} /* end of avPgwDelUeCb */



/*
 *
 *       Fun:   Adding a new control tunnelCb into hash list
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
PUBLIC S16 avPgwAddUeCtrlTunnelCb
(
AvPgwUeTunnelCb   **tunnelCb, /* Tunnel Control block (OUT) */
AvPgwUeCb         *ueCb,      /* UE CB (INT) */
U32               locTeid,    /* Local C-TEID (IN) */
CmTptAddr         *locAddr,   /* Local Address */
CmTptAddr         *remAddr,   /* Remote Address */
EgMsg             *egMsg      /* gtp-c msg (INT) */
)
#else
PUBLIC S16 avPgwAddUeCtrlTunnelCb(tunnelCb, ueCb, teid, locAddr, remAddr, egMsg)
AvPgwUeTunnelCb   **tunnelCb; /* Tunnel Control block (OUT) */
AvPgwUeCb         *ueCb;      /* UE CB (INT) */
U32               locTeid;    /* Local C-TEID (IN) */
CmTptAddr         *locAddr;   /* Local Address */
CmTptAddr         *remAddr;   /* Remote Address */
EgMsg             *egMsg;     /* gtp-c msg (INT) */
#endif
{
   S16               ret = ROK;

   AV_PGW_SND_TRC2(avPgwAddUeCtrlTunnelCb)

   /* av001.101: use the default port numbers */
   (*tunnelCb)->localTeIdKey = locTeid;
   (*tunnelCb)->tunnelType = AV_PGW_CTRL_TUNNEL;
   (*tunnelCb)->state = AV_PGW_EGTPC_TUNNEL_DOWN;
   (*tunnelCb)->ueCb = ueCb;
   (*tunnelCb)->ueRouteCb = NULLP;
   (*tunnelCb)->lclAddr = *locAddr;
   (*tunnelCb)->lclAddr.u.ipv4TptAddr.port = AV_PGW_LOC_PORT;
   (*tunnelCb)->remAddr = *remAddr;
   (*tunnelCb)->seqNum = egMsg->msgHdr.seqNumber;

   ret = cmHashListInsert(&(avPgwCb.egtCSapCb->ueTunnelCbHlCp), 
                          (PTR)(*tunnelCb), (U8 *)&((*tunnelCb)->localTeIdKey),
                          (U16)sizeof((*tunnelCb)->localTeIdKey));
   if (ROK != ret)
   { 
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "avPgwAddUeCtrl(*tunnelCb) "
                           "Insert control (*tunnelCb) to hash list, FAILED"));
   }

   AV_PGW_SND_RETVALUE(ret);

} /* end of avPgwAddUeCtrlTunnelCb */


/*
 *
 *       Fun:   avPgwFndBearer
 *
 *       Desc:  Finds and returns the Bearer Pointer pointer for the pdn
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_utl.c
 *
 */
#ifdef ANSI
PUBLIC AvPgwBearerCb* avPgwFndBearer
(
AvPgwUeCb         *ueCb,      /* UE CB (INT) */
U32               locTeid     /* Local C-TEID (IN) */
)
#else
PUBLIC AvPgwBearerCb* avPgwFndBearer(ueCb, teid)
AvPgwUeCb         *ueCb;      /* UE CB (INT) */
U32               teid;       /* Local C-TEID (IN) */
#endif
{
   AvPgwBearerCb       *bearer = NULLP;
   U32                  bCnt = 0;

   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      if ((ueCb->bearerTable[bCnt] != NULLP) &&
          (ueCb->bearerTable[bCnt]->s5s8UTun != NULLP))
      {
         if (ueCb->bearerTable[bCnt]->s5s8UTun->localTeIdKey == locTeid)
         {
            bearer = ueCb->bearerTable[bCnt];
            break;
         }
      }
   }

   return bearer;

} /* end of avPgwFndBearer */


/*
 *
 *       Fun:   avPgwFndDefUTun
 *
 *       Desc:  Finds and returns the Default userplane tunnel
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_utl.c
 *
 */
#ifdef ANSI
PUBLIC AvPgwUeTunnelCb* avPgwFndDefUTun
(
AvPgwPdnCb        *pdnCb     /* UE CB (INT) */
)
#else
PUBLIC AvPgwBearerCb* avPgwFndBearer(pdnCb)
AvPgwPdnCb        *pdnCb     /* UE CB (INT) */
#endif
{
   U32                  bCnt = 0;
   AvPgwUeTunnelCb      *uTun = NULLP;
   AvPgwBearerCb        *bearer = NULLP;

   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      if (pdnCb->ueCb->bearerTable[bCnt] != NULLP)
      {
         bearer = pdnCb->ueCb->bearerTable[bCnt];

         if (bearer->s5s8UTun->tunnelType == AV_PGW_DATA_TUNNEL_DFLT &&
            AV_PGW_CHK_BEARER_MAP(pdnCb->bearerMap, bearer->ebi))
         {
            uTun = bearer->s5s8UTun;
            break;
         }
      }
   }

   return uTun;

} /* end of avPgwFndBearer */


/*
 *
 *       Fun:   avPgwFndDefBearer
 *
 *       Desc:  Finds and returns the Default Bearer
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_utl.c
 *
 */
#ifdef ANSI
PUBLIC AvPgwBearerCb* avPgwFndDefBearer
(
AvPgwPdnCb        *pdnCb     /* UE CB (INT) */
)
#else
PUBLIC AvPgwBearerCb* avPgwFndDefBearer(pdnCb)
AvPgwPdnCb        *pdnCb     /* UE CB (INT) */
#endif
{
   U32                  bCnt = 0;
   AvPgwBearerCb        *defBearer = NULLP;
   AvPgwBearerCb        **bearerTable = NULLP;

   AV_PGW_SND_TRC2(avPgwFndDefBearer)

   bearerTable = pdnCb->ueCb->bearerTable;

   for (bCnt = 0; bCnt < AV_PGW_MAX_BEARERS; bCnt++)
   {
      if ((bearerTable[bCnt] != NULLP) && (bearerTable[bCnt]->isDefBearer) &&
          (bearerTable[bCnt]->pdnCb == pdnCb))
      {
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Found Default Bearer"));
         defBearer = bearerTable[bCnt];
         break;
      }
   }

   return defBearer;

} /* end of avPgwFndBearer */



/*
 *
 *       Fun:   avPgwAddUeDataTunnelCb
 *
 *       Desc:  Adding a new data tunnelCb into hash list, Invoked by the 
 *                application 
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
/* av001.101: added more parameters */
#ifdef ANSI
PUBLIC S16 avPgwAddUeDataTunnelCb
(
AvPgwUeTunnelCb   **tunnelCb,
AvPgwUeCb         *ueCb,
CmTptAddr         *locAddr,
AvPgwTunnelType   tunnelType
)
#else
PUBLIC S16 avPgwAddUeDataTunnelCb(tunnelCb, ueCb, locAddr, tunnelType)
AvPgwUeTunnelCb   **tunnelCb;
AvPgwUeCb         *ueCb;
CmInetIpAddr      *locAddr;
AvPgwTunnelType   tunnelType;
#endif
{
   S16         ret = ROK;

   AV_PGW_SND_TRC2(avPgwAddUeDataTunnelCb)
      
   /* Fill Local Address */
   cmMemcpy((U8 *)&(*tunnelCb)->lclAddr,
            (U8 *)&avPgwCb.pgwCfg.pgwAddrCfg.selfAddr,
            sizeof(CmTptAddr));

   (*tunnelCb)->tunnelType = tunnelType;
   (*tunnelCb)->state = AV_PGW_EGTPU_TUNNEL_DOWN;
   locAddr->u.ipv4TptAddr.port = AV_PGW_ROUT_REMOTE_PORT; 

   /* Generate and store Local U TEID */
   avPgwUtlGenerateGtpUTeId(&((*tunnelCb)->localTeIdKey));

   ret = cmHashListInsert(&avPgwCb.egtUSapCb->ueTunnelCbHlCp,
                          (PTR)(*tunnelCb),
                          (U8 *)&((*tunnelCb)->localTeIdKey),
                          (U16)sizeof((*tunnelCb)->localTeIdKey));
   if (ROK!= ret)
   { 
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                           "avPgwAddUeDataTunnelCb: Adding U-Plane tunnel to"
                           "Hash table"));
      AV_PDNGW_SENDTSK_FREE(*tunnelCb, sizeof(AvPgwUeTunnelCb));
      *tunnelCb = NULLP;
   }

   AV_PGW_SND_RETVALUE(ret);

} /* end of avPgwAddUeDataTunnelCb */


/*
 *
 *       Fun:   avPgwAddPendingTunnelCb
 *
 *       Desc:  Adding a new data tunnelCb into pending Tunnel Cb
 *              used during create bearer request/rsp
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwAddPendingTunnelCb
(
AvPgwPdnCb        *pdnCb,
AvPgwUeTunnelCb   **tunnelCb,
CmTptAddr         *locAddr,
AvPgwTunnelType   tunnelType
)
#else
PUBLIC S16 avPgwAddPendingTunnelCb(pdnCb, tunnelCb, locAddr, tunnelType)
AvPgwPdnCb        *pdnCb;
AvPgwUeTunnelCb   **tunnelCb;
CmTptAddr         *locAddr;
AvPgwTunnelType   tunnelType;
#endif
{
   S16                  ret = ROK;
   AvPgwPendingTun      *pendingTun = NULLP;

   AV_PGW_SND_TRC2(avPgwAddPendingTunnelCb)

   AV_PDNGW_SENDTSK_ALLOC(pendingTun, sizeof(AvPgwPendingTun)); 
   if(NULLP == pendingTun)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "avPgwAddPendingTunnelCb: "
                           "Memory Allocation for AvPgwPendingTun"));
      AV_PGW_SND_RETVALUE(RFAILED);   
   }

   /* Fill Local Address */
   cmMemcpy((U8 *)&(*tunnelCb)->lclAddr,
            (U8 *)&avPgwCb.pgwCfg.pgwAddrCfg.selfAddr,
            sizeof(CmTptAddr));

   pendingTun->uTun = *tunnelCb;
   (*tunnelCb)->tunnelType = tunnelType;
   (*tunnelCb)->state = AV_PGW_EGTPU_TUNNEL_DOWN;
   (*tunnelCb)->lclAddr.u.ipv4TptAddr.port = AV_PGW_ROUT_REMOTE_PORT; 
   avPgwUtlGenerateGtpUTeId(&((*tunnelCb)->localTeIdKey));

   cmLListAdd2Tail(&(pdnCb->pendingTunLst) ,&(pendingTun->lnk));

   AV_PGW_SND_RETVALUE(ret);
} /* avPgwAddPendingTunnelCb */


/*
 *
 *       Fun:   avPgwUtlIpv4ArrToInet
 *
 *       Desc:  Converts array from array format to inet U32
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwUtlIpv4ArrToInet
(
U8             *addrArr,        /* Byte Array address representation */
U32            *address         /* 4 byte interent address */
)
#else
PUBLIC S16 avPgwUtlIpv4ArrToInet(addrArr, address)
U8           *addrArr;        /* Byte Array address representation */
U32          *address;        /* 4 byte interent address */
#endif
{
   *address = ((U32)addrArr[0] << 24)|  ((U32)addrArr[1] << 16)   | 
      ((U32)addrArr[2] << 8)  |  (U32)(addrArr[3]);
   RETVALUE(ROK);
}


/*
 *
 *       Fun:   avPgwUtlGenerateGtpUTeId
 *
 *       Desc:  This function generates a new local tunnel id value for EGTPU
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
PUBLIC S16 avPgwUtlGenerateGtpUTeId
(
TeId  *teId
)
#else
PUBLIC S16 avPgwUtlGenerateGtpUTeId(teId)
TeId  *teId;
#endif
{
   AV_PGW_SND_TRC2(avPgwUtlGenerateGtpUTeId)

   *teId = ++avPgwCb.gtpUTeIdCntr;

   AV_PGW_SND_RETVALUE(ROK);
}


/*
 *
 *       Fun:   avPgwUtlFindPendingTun
 *
 *       Desc:  Matches the TEID of tunnels and returns the tunnel ptr
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC AvPgwUeTunnelCb* avPgwUtlFindPendingTun
(
CmLListCp            *tunList,
U32                  teid
)
#else
PUBLIC AvPgwUeTunnelCb* avPgwUtlFindPendingTun(teId)
CmLListCp            *tunList;
U32                  teid;
#endif
{
   AvPgwUeTunnelCb         *uTun = NULLP;
   CmLList                 *tmp = NULLP;

   AV_PGW_SND_TRC2(avPgwUtlFindPendingTun)

   CM_LLIST_FIRST_NODE(tunList, tmp);
   while (tmp)
   {
      uTun = ((AvPgwPendingTun *)tmp)->uTun;
      if (teid == uTun->localTeIdKey)
      {
         break;
      }

      CM_LLIST_NEXT_NODE(tunList, tmp);
   }

   return uTun;
}

/*
 *
 *       Fun:   avPgwUtlDelPendingTunLst
 *
 *       Desc:  Deletes Pending tunnels Cbs from PDN
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwUtlDelPendingTunLst
(
CmLListCp            *tunLst
)
#else
PUBLIC S16 avPgwUtlDelPendingTunLst(tunLst)
CmLListCp            *tunLst;
#endif
{
   CmLList                 *tmp = NULLP;
   U32                     pfIndex = 0;
   AvPgwUeTunnelCb         *uTun = NULLP;

   AV_PGW_SND_TRC2(avPgwUtlDelPendingTunLst)

   CM_LLIST_FIRST_NODE(tunLst, tmp);
   while (tmp)
   {
      uTun = ((AvPgwPendingTun *)tmp)->uTun;
      for (pfIndex = 0; pfIndex < uTun->tft.pktFilterCnt; pfIndex++)
      {
         AV_PDNGW_SENDTSK_FREE(uTun->tft.pktFilters[pfIndex],
                              sizeof(AvPgwEgPktFilter));
         uTun->tft.pktFilters[pfIndex] = NULLP;
      }

      uTun->tft.pktFilterCnt = 0;

      cmLListDelFrm(tunLst, tmp);
      AV_PDNGW_SENDTSK_FREE(tmp, sizeof(AvPgwPendingTun));
      tmp = NULLP;

      CM_LLIST_NEXT_NODE(tunLst, tmp);
   }

   AV_PGW_SND_RETVALUE(ROK);
}



/*
 *
 *       Fun:   avPgwUtlGetTransId
 *
 *       Desc:  Gets the transaction ID for PGW initiated transactions
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwUtlGetTransId(void)
#else
PUBLIC S16 avPgwUtlGetTransId()
#endif
{
   return (++gAvPgwTransId);
}


/*
 *
 *       Fun:   avPgwEgtQMsg
 *
 *       Desc:  Queues the message which will be processed later
 *             
 *
 *       Ret:   ROK  
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwEgtQMsg
(
AvPgwUeTunnelCb   *cTun,
U8                eventType,
EgMsg             *egMsg
)
#else
PUBLIC S16 avPgwEgtQMsg(cTun, eventType, egMsg)
AvPgwUeTunnelCb   *cTun;
U8                eventType,
EgMsg             *egMsg;
#endif
{
   AvPgwPendingEvnt  *evnt = NULLP;

   AV_PGW_SND_TRC2(avPgwEgtQMsg)

   AV_PDNGW_SENDTSK_ALLOC(evnt, sizeof(AvPgwPendingEvnt)); 
   if (NULLP == evnt)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Memory allocation for"
                           " AvPgwPendingEvnt"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   evnt->locTeid = cTun->localTeIdKey;
   evnt->egMsg = egMsg;
   evnt->eventType = eventType;

   evnt->lnk.node = (PTR)evnt;
   cmLListAdd2Tail(&(avPgwCb.pendingEvntLst) , &(evnt->lnk));
    
   AV_PGW_SND_RETVALUE(ROK);

} /* end of avPgwEgtQMsg */



/*
 *
 *       Fun:   avPgwDelUeTunnelCb
 *
 *       Desc:  This function deletes the tunnel control block in hash list.
 *             
 *
 *       Ret:   ROK  
 *
 *       Notes: None
 *
 *       File:  av.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwDelUeTunnelCb
(
AvPgwUeCb         *ueCb,
AvPgwUeTunnelCb   **tunnel,
AvPgwTunnelType   tunnelType
)
#else
PUBLIC S16 avPgwDelUeTunnelCb(ueCb, tunnel, tunnelType)
AvPgwUeCb         *ueCb;
AvPgwUeTunnelCb   **tunnel;
AvPgwTunnelType   tunnelType;
#endif
{
   S16               ret = ROK;
   AvPgwUeTunnelCb   *tempTunnel = *tunnel;
   AvPgwPdnCb        *pdnCb = NULLP;
   AvPgwBearerCb     *bearer = NULLP;
   U32               bIndex = 0;

   AV_PGW_SND_TRC2(avPgwDelUeTunnelCb)

   pdnCb = tempTunnel->pdnCb;
   ueCb = pdnCb->ueCb;
   bIndex = AV_PGW_BEARER_IND(tempTunnel->bearerId);
   bearer = ueCb->bearerTable[bIndex];

   switch(tunnelType)
   {
      case AV_PGW_CTRL_TUNNEL:
      {
         ret = cmHashListDelete(&(avPgwCb.egtCSapCb->ueTunnelCbHlCp),
                                (PTR)*tunnel);
         if (ROK != ret)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Failed to delete "
                                 "control plane tunnel Cb"));
            AV_PGW_SND_RETVALUE(ret);
         }

         if (tempTunnel == pdnCb->s5s8CTun)
         {
			   /* av004.101 Release the tunnel cb */
            AV_PDNGW_SENDTSK_FREE(*tunnel, sizeof(AvPgwUeTunnelCb));
            pdnCb->s5s8CTun = NULLP; 

            /* Deleting the bearer associated with this tunnel */
            AV_PDNGW_SENDTSK_FREE(bearer, sizeof(AvPgwBearerCb));
            ueCb->bearerTable[bIndex] = NULLP;

            /* Freeing the memory for PDN */
            AV_PDNGW_SENDTSK_FREE(pdnCb, sizeof(AvPgwPdnCb));
         }

         break;
      }

      case AV_PGW_DATA_TUNNEL_DFLT:
      case AV_PGW_DATA_TUNNEL_DED:
      {
         ret = cmHashListDelete(&(avPgwCb.egtUSapCb->ueTunnelCbHlCp),
                                (PTR)*tunnel);
         if (ROK != ret)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Deleting U-Tun Cb"));
            AV_PGW_SND_RETVALUE(ret);
         }

         if ((*tunnel)->tunnelType == AV_PGW_DATA_TUNNEL_DFLT)
         {
            avPgwDelUeRouteCb(*tunnel);
            AV_PDNGW_SENDTSK_FREE(*tunnel, sizeof(AvPgwUeTunnelCb));

            /* Bearer associated with default u-tun will be deleted when 
             * the c-tun is deleted
             */
         }
         else
         {
            avPgwDelBearerPktFilters((*tunnel)->ueRouteCb, (*tunnel)->bearerId);
            AV_PDNGW_SENDTSK_FREE((*tunnel)->ueRouteCb, sizeof(AvPgwUeRouteCb));
            AV_PDNGW_SENDTSK_FREE(*tunnel, sizeof(AvPgwUeTunnelCb));

            /* Deleting the bearer associated with this tunnel */
            AV_PDNGW_SENDTSK_FREE(bearer, sizeof(AvPgwBearerCb));
            ueCb->bearerTable[bIndex] = NULLP;
         }

         pdnCb->numUTun -= 1;
         break;
      }
      
      default:
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Unknown tunnel type"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }

   AV_PGW_SND_RETVALUE(ROK);

} /* end of avPgwDelUeTunnelCb */



/**************************************************************************//**
 * @brief:
 *    Deletes all packet filters associated with the Bearer
 *
 * @param[in]  ueRouteCb
 *    Route CB associated with the UE
 *
 * @param[in]  bearerId
 *    EPS Bearer ID
 *
 * @return
 *    No return values
 *
 *****************************************************************************/
PRIVATE Void avPgwDelBearerPktFilters
(
AvPgwUeRouteCb    *pUeRouteCb,
U8                ebi
)
{
   U8                   ipIdx = 0;
   AvPgwIpCb            *pIpCb = NULLP;
   AvPgwPktFilterCb     *pPfCb = NULLP;

   AV_PGW_SND_TRC2(avPgwDelBearerPktFilters);

   AV_PGW_IPCB_INDX(pUeRouteCb->nwUeAddr.addr.u.ipv4NetAddr, ipIdx);
   pIpCb = &(avPgwCb.ipCbsList[ipIdx]);

   pPfCb = pIpCb->pHead;
   while (pPfCb)
   {
      if (ebi == AV_PGW_GET_EBI_FROM_PFID(pPfCb->pFId))
      {
         /* return valid next pf cb element */
         pPfCb = avPgwDelPktFilter(pIpCb, pPfCb->pFId);
      }
      else
      {
         pPfCb = pPfCb->pNext;
      }
   }

   AV_PGW_RETVOID;
}


PUBLIC S16 avPgwGetTempTftParams(AvPgwTft *tft, U32 ueSrcPort)
{
   U32       pfCnt = 0;
   
   AV_PGW_SND_TRC2(avPgwGetTempTftParams);

   tft->pres = PRSNT_NODEF;
   tft->opCode = AV_PGW_TFT_OPCODE_NEWTFT;
   tft->eBit   = 0;   
   tft->noOfPfs = 2;    
   tft->noOfParams = 0; 


      tft->pfList[pfCnt].pres                 =  PRSNT_NODEF;
      tft->pfList[pfCnt].id                   =  1;
      tft->pfList[pfCnt].dir                  =  GTP_DOWNLINK_TFT_FILTER; 
      tft->pfList[pfCnt].preced               =  1; /* highest precedence: Higher the value lower the precedence. */
      tft->pfList[pfCnt].ipv4.pres            =  NOTPRSNT;
      tft->pfList[pfCnt].ipv6.pres            =  NOTPRSNT;
      tft->pfList[pfCnt].protId.pres          =  NOTPRSNT;
      tft->pfList[pfCnt].protId.protType      =  1;   /*ICMP*/
      /* Hard coding the local port to 30000. Idealy we are not using this
       * port because data will be received from the gtp port 
       * and pgw application will take care of looping back
       * to the sgw. port no 30000 will be used to just to pass the 
       * TUCL sap configuration. */
      tft->pfList[pfCnt].localPort.pres       =  PRSNT_NODEF;
      tft->pfList[pfCnt].localPort.port       =  30000;
      tft->pfList[pfCnt].remotePort.pres      =  PRSNT_NODEF;
      tft->pfList[pfCnt].remotePort.port      =  ueSrcPort;
      tft->pfList[pfCnt].locPortRange.pres    =  NOTPRSNT;
#if 0
      tft->pfList[pfCnt].locPortRange.rangeLow =  10000;
      tft->pfList[pfCnt].locPortRange.rangeHigh =  60000;
#endif
      tft->pfList[pfCnt].remPortRange.pres    =  NOTPRSNT;
#if 0
      tft->pfList[pfCnt].remPortRange.rangeLow  =  10000;
      tft->pfList[pfCnt].remPortRange.rangeHigh    =  60000;
#endif
      tft->pfList[pfCnt].secParam.pres        =  NOTPRSNT;
      tft->pfList[pfCnt].tos.pres             =  NOTPRSNT;
      tft->pfList[pfCnt].flowLabel.pres       =  NOTPRSNT;

      pfCnt++;
      /* second Packet filter  */
      tft->pfList[pfCnt].pres                 =  PRSNT_NODEF;
      tft->pfList[pfCnt].id                   =  2;
      tft->pfList[pfCnt].dir                  =  GTP_UPLINK_TFT_FILTER; 
      tft->pfList[pfCnt].preced               =  2; /* highest precedence: Higher the value lower the precedence. */
      tft->pfList[pfCnt].ipv4.pres            =  NOTPRSNT;
      tft->pfList[pfCnt].ipv6.pres            =  NOTPRSNT;
      tft->pfList[pfCnt].protId.pres          =  NOTPRSNT;
      tft->pfList[pfCnt].protId.protType      =  1;   /*ICMP*/
      /* Hard coding the local port to 30000. Idealy we are not using this
       * port because data will be received from the gtp port 
       * and pgw application will take care of looping back
       * to the sgw. port no 30000 will be used to just to pass the 
       * TUCL sap configuration. */
      tft->pfList[pfCnt].localPort.pres       =  PRSNT_NODEF;
      tft->pfList[pfCnt].localPort.port       =  ueSrcPort ;
      tft->pfList[pfCnt].remotePort.pres      =  PRSNT_NODEF;
      tft->pfList[pfCnt].remotePort.port      =  30000;
      tft->pfList[pfCnt].locPortRange.pres    =  NOTPRSNT;
      tft->pfList[pfCnt].remPortRange.pres    =  NOTPRSNT;
      tft->pfList[pfCnt].secParam.pres        =  NOTPRSNT;
      tft->pfList[pfCnt].tos.pres             =  NOTPRSNT;
      tft->pfList[pfCnt].flowLabel.pres       =  NOTPRSNT;

   AV_PGW_RETVALUE(ROK);
}

PUBLIC S16 avPgwUpdtTftParams(AvPgwUeTunnelCb *ueTunnCb, AvPgwTft *tft)
{
    U32       pfCnt = 0;

    AV_PGW_SND_TRC2(avPgwUpdtTftParams);

    if(tft->pres != PRSNT_NODEF)
    {
        AV_PGW_RETVALUE(RFAILED);
    }
    ueTunnCb->tft.opCode       = tft->opCode;
    ueTunnCb->tft.paramLstPres = tft->noOfParams; 
    ueTunnCb->tft.pktFilterCnt = tft->noOfPfs;
    for (pfCnt = 0; pfCnt < tft->noOfPfs; pfCnt++) 
    {
        AV_PDNGW_SENDTSK_ALLOC(ueTunnCb->tft.pktFilters[pfCnt], sizeof(AvPgwEgPktFilter));
        ueTunnCb->tft.pktFilters[pfCnt]->pfId = tft->pfList[pfCnt].id;
        ueTunnCb->tft.pktFilters[pfCnt]->pktFilterDir = tft->pfList[pfCnt].dir;
        ueTunnCb->tft.pktFilters[pfCnt]->precedence = tft->pfList[pfCnt].preced;
        ueTunnCb->tft.pktFilters[pfCnt]->presence_mask = 0;
        if (tft->pfList[pfCnt].protId.pres) 
        {
            ueTunnCb->tft.pktFilters[pfCnt]->component.proto_id = tft->pfList[pfCnt].protId.protType;
            ueTunnCb->tft.pktFilters[pfCnt]->presence_mask |= GTP_PROTO_ID_PKT_FLTR_MASK;
        }
        if (tft->pfList[pfCnt].ipv4.pres) 
        {
            /*TODO: Need to update this*/
        }
        if (tft->pfList[pfCnt].ipv6.pres) 
        {
            /*TODO: Need to update this*/
        }
        if (tft->pfList[pfCnt].localPort.pres) 
        {
           ueTunnCb->tft.pktFilters[pfCnt]->component.locPort = tft->pfList[pfCnt].localPort.port;
        }
        if (tft->pfList[pfCnt].remotePort.pres) 
        {
           ueTunnCb->tft.pktFilters[pfCnt]->component.remPort = tft->pfList[pfCnt].remotePort.port;
        }
        if (tft->pfList[pfCnt].locPortRange.pres) 
        {
            ueTunnCb->tft.pktFilters[pfCnt]->component.locPortRangeLow = tft->pfList[pfCnt].locPortRange.rangeLow;
            ueTunnCb->tft.pktFilters[pfCnt]->component.locPortRangeHigh = tft->pfList[pfCnt].locPortRange.rangeHigh;
            ueTunnCb->tft.pktFilters[pfCnt]->presence_mask |= GTP_LOC_PORT_RNG_TY_PKT_FLTR_MASK;
        }
        if (tft->pfList[pfCnt].remPortRange.pres) 
        {
            ueTunnCb->tft.pktFilters[pfCnt]->component.remPortRangeLow = tft->pfList[pfCnt].remPortRange.rangeLow;
            ueTunnCb->tft.pktFilters[pfCnt]->component.remPortRangeHigh = tft->pfList[pfCnt].remPortRange.rangeHigh;
            ueTunnCb->tft.pktFilters[pfCnt]->presence_mask |= GTP_REM_PORT_RNG_TY_PKT_FLTR_MASK;
        }
        if (tft->pfList[pfCnt].secParam.pres) 
        {
            /*TODO: Need to update this*/
        }
        if (tft->pfList[pfCnt].tos.pres) 
        {
            /*TODO: Need to update this*/
        }
        if (tft->pfList[pfCnt].flowLabel.pres) 
        {
            /*TODO: Need to update this*/
        }

    }
    AV_PGW_RETVALUE(ROK);
}


PUBLIC S16 avPgwEncTftBuf(U8 *tftBuf, U32 *tftLen, AvPgwTft *tft)
{
    U32        pfCnt = 0;
    U32        paramCnt = 0;
    U8         pfLenIdx = 0;


    AV_PGW_SND_TRC2(avPgwEncTftBuf);

    /* Length Index to be filled Later */
    tftBuf[*tftLen] = tft->opCode << 5;
    tftBuf[*tftLen] |= (tft->noOfParams << 4);
    tftBuf[(*tftLen)++] |= tft->noOfPfs;
    for (pfCnt = 0; pfCnt < tft->noOfPfs; pfCnt++) 
    {
        tftBuf[(*tftLen)] = tft->pfList[pfCnt].id;
        if (tft->opCode == AV_PGW_TFT_OPCODE_DEL_FILTER) 
        {
            (*tftLen)++;
            continue;
	}
        tftBuf[(*tftLen)++] |= (tft->pfList[pfCnt].dir << 4);
        tftBuf[(*tftLen)++] = tft->pfList[pfCnt].preced;
        /*
         * Total Length of the Packet Filetr Content
         * will be written later on this particular Index
         * */
        pfLenIdx = (*tftLen)++;
        if (tft->pfList[pfCnt].protId.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_PROT_ID;
            tftBuf[(*tftLen)++] = tft->pfList[pfCnt].protId.protType;
        }
        if (tft->pfList[pfCnt].ipv4.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_IP4_ADDR;
            cmMemcpy(&tftBuf[(*tftLen)], tft->pfList[pfCnt].ipv4.ip4,AV_PGW_IPV4_SIZE);
            *tftLen += AV_PGW_IPV4_SIZE;
            /*cmMemcpy(&tftBuf[(*tftLen)], tft->pfList[pfCnt].ipv4.ip4Mask,AV_PGW_IPV4_SIZE);
            *tftLen += AV_PGW_IPV4_SIZE;*/
        }
        if (tft->pfList[pfCnt].ipv6.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_IP6_ADDR;
            cmMemcpy(&tftBuf[(*tftLen)], tft->pfList[pfCnt].ipv6.ip6,AV_PGW_IPV6_SIZE);
            *tftLen += AV_PGW_IPV6_SIZE;
        }
        if (tft->pfList[pfCnt].localPort.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_LOC_PORT;
            tftBuf[(*tftLen)++] =
                (tft->pfList[pfCnt].localPort.port >> 8) & 0xff;
            tftBuf[(*tftLen)++] = tft->pfList[pfCnt].localPort.port & 0xff;
        }

        if (tft->pfList[pfCnt].remotePort.pres) {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_REM_PORT;
            tftBuf[(*tftLen)++] =
                (tft->pfList[pfCnt].remotePort.port >> 8) & 0xff;
            tftBuf[(*tftLen)++] = tft->pfList[pfCnt].remotePort.port & 0xff;
        }
        if (tft->pfList[pfCnt].locPortRange.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_LOC_PORT_RANGE;
            tftBuf[(*tftLen)++] =
                (tft->pfList[pfCnt].locPortRange.rangeLow >> 8) & 0xff;
            tftBuf[(*tftLen)++] =
                tft->pfList[pfCnt].locPortRange.rangeLow & 0xff;
            tftBuf[(*tftLen)++] =
                (tft->pfList[pfCnt].locPortRange.rangeHigh >> 8) & 0xff;
            tftBuf[(*tftLen)++] =
                tft->pfList[pfCnt].locPortRange.rangeHigh & 0xff;
        }
        if (tft->pfList[pfCnt].remPortRange.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_REM_PORT_RANGE;
            tftBuf[(*tftLen)++] =
                (tft->pfList[pfCnt].remPortRange.rangeLow >> 8) & 0xff;
            tftBuf[(*tftLen)++] =
                tft->pfList[pfCnt].remPortRange.rangeLow & 0xff;
            tftBuf[(*tftLen)++] =
                (tft->pfList[pfCnt].remPortRange.rangeHigh >> 8) & 0xff;
            tftBuf[(*tftLen)++] =
                tft->pfList[pfCnt].remPortRange.rangeHigh & 0xff;
        }
        if (tft->pfList[pfCnt].secParam.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_SEC_PARAM;
            cmMemcpy(&tftBuf[(*tftLen)], tft->pfList[pfCnt].secParam.params,
                     AV_PGW_IP_SEC_SIZE);
            *tftLen += AV_PGW_IP_SEC_SIZE;
        }
        if (tft->pfList[pfCnt].tos.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_TOS;
            tftBuf[(*tftLen)++] = tft->pfList[pfCnt].tos.tos;
            tftBuf[(*tftLen)++] = tft->pfList[pfCnt].tos.mask;
        }
        if (tft->pfList[pfCnt].flowLabel.pres) 
        {
            tftBuf[(*tftLen)++] = AV_PGW_PF_CMPTYPE_FLOW_LABEL;
            cmMemcpy(&tftBuf[(*tftLen)], tft->pfList[pfCnt].flowLabel.buf,
                     AV_PGW_PF_CMPTYPE_FLOW_LABEL);
            *tftLen += AV_PGW_PF_CMPTYPE_FLOW_LABEL;
        }

        /*Total Length of the Packet Filter Content is filled here */
        tftBuf[pfLenIdx] = (*tftLen - pfLenIdx) - 1;
    }
    if (tft->noOfParams) 
    {
        for (paramCnt = 0; paramCnt < tft->noOfParams; paramCnt++) 
        {
            tftBuf[(*tftLen)++] = tft->params[paramCnt].paramType;
            tftBuf[(*tftLen)++] = tft->params[paramCnt].len;
            cmMemcpy(&tftBuf[(*tftLen)], tft->params[paramCnt].buf,
                     tft->params[paramCnt].len);
            *tftLen += tft->params[paramCnt].len;
        }
    }
    AV_PGW_RETVALUE(ROK);
}


PUBLIC S16 avPgwFillQos(AvPgwUeTunnelCb  *ueTunlCb)
{
    AV_PGW_SND_TRC2(avPgwFillQos);

    ueTunlCb->epsQos.isPVIPres = 1;
    ueTunlCb->epsQos.isPCIPres = 1;
    ueTunlCb->epsQos.prityLvl  = 0;
    ueTunlCb->epsQos.qci = 1;
    ueTunlCb->epsQos.upMBR.val[0] = 255;
    ueTunlCb->epsQos.upMBR.val[1] = 255;
    ueTunlCb->epsQos.upMBR.val[2] = 255;
    ueTunlCb->epsQos.upMBR.length = 3;
    ueTunlCb->epsQos.dnMBR.val[0] = 255;
    ueTunlCb->epsQos.dnMBR.val[1] = 255;
    ueTunlCb->epsQos.dnMBR.val[2] = 255;
    ueTunlCb->epsQos.dnMBR.length = 3;
    ueTunlCb->epsQos.upGBR.val[0] = 255;
    ueTunlCb->epsQos.upGBR.val[1] = 255;
    ueTunlCb->epsQos.upGBR.val[2] = 255;
    ueTunlCb->epsQos.upGBR.length = 3;
    ueTunlCb->epsQos.dnGBR.val[0] = 255;
    ueTunlCb->epsQos.dnGBR.val[1] = 255;
    ueTunlCb->epsQos.dnGBR.val[2] = 255;
    ueTunlCb->epsQos.dnGBR.length = 3;

    AV_PGW_RETVALUE(ROK);
}
/********************************************************************30**

         End of file:     av_utl.c@@/main/1 - Tue Aug 17 18:54:35 2010

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
/main/1      -      sp                 1.LTE-CNE:PGW Intial Release
/main/1      av001.101 rp              1.Added decoding utilty for TFT
                                       2.Removed unused functions
/main/1      av004.101 pmacharla       1.eGTP 2.1 upgrade under EG_REL_930 flag
*********************************************************************91*/
