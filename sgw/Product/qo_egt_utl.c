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

     Name:    Trillium LTE SGW Application

     Type:    C file

     Desc:    This file utility function for creating, validating gtp-c
              messages

     File:    qo_egt_utl.c

     Sid:      qo_app.c@@/main/1 - Thu Jul  8 21:22:18 2010

     Prg:     mm 
*********************************************************************21*/

/***************************************************************/

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

PUBLIC S16 qoSgwEgtUtlAppendIe ARGS((
EgMsg          *egMsg,        /* gtp-c msg */
U32            ieType,        /* ie type */
U8             ieInst,        /* ie instance value */
EgDataTypes    dataType,      /* IE Data type */
EgIe           *egIe          /* pointer to IE details */
));

PRIVATE S16  qoSgwUtilEgtAddBearerTft ARGS((
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         *buf,
U32        len
));

PRIVATE S16  qoSgwEgtUtlGetIeCnt ARGS((
EgMsg          *egMsg,
U32            ieType,
U8             inst,
S16            *bCnt
));

/*
 *
 *       Fun:   qoSgwValCSReq
 *
 *       Desc:  Validates and Stores Create Session Request IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwValCSReq
(
EgMsg           *egMsg,    /* GTP-C message (IN) */
 QoSgwTunnelCb   *sTun,   /* Tunnel Control Block (INOUT) */
QoSgwPdnCb      **pdnCb,   /* PDN Control Block (OUT) */
U32             *cause     /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValCSReq(msg, sTun, pdnCb, cause)
EgMsg           *egMsg,    /* GTP-C message (IN) */
   QoSgwTunnelCb   *sTun    /* Tunnel Control Block (INOUT) */
QoSgwPdnCb      **pdnCb,   /* PDN Control Block (OUT) */
U32             *cause     /* cause of failure (OUT) */
#endif
{
   S16                     ret = ROK;
   QoSgwPeerCb             *peer = NULLP;
   QoSgwPeerCb             *sgsnuPeer = NULLP;
   QoSgwTunnelCb           *s5s8CTun=NULLP;
   CmTptAddr               remAddr = {0};
   EgIe                    *egIe = NULLP;
   S16                     ieOcr = 0;
   U32                     transId = 0;
   QoSgwUeCb               *ueCb = NULLP;
   QoSgwBearerCb           *bearer = NULLP;
   U32                     bearerInd = 0;
   U32                     pgwRemTeid = 0;

   QO_SGW_TRC2(qoSgwValCSReq)

   QO_SGW_DBG_INFO(("Validating Create Session Request"));

   /* Update sequence number */
   sTun->seqNum = egMsg->msgHdr.seqNumber;
   ueCb = sTun->ueCb;

   /* Find Sender's(MME/SGSN)  TEID */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 0);
   if (ret != ROK)
   {
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
      QO_SGW_DBG_ERR(("Senders(MME/SGSN) FTEID IE Could Not Be Retrieved"));
   }
   else
   {
      sTun->remTeid = egIe->t.fTeid.teid;
   }

   if (ROK == ret)
   {
      /* find PDN gateway address */
      QO_SGW_DBG_INFO(("Retrieving PDN Gateway F-TEID IE"));
      egIe = NULLP;
      ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 1);
      if(ret != ROK || egIe == NULLP)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         QO_SGW_DBG_ERR(("PDN Gateway F-TEID IE Retrieval, FAILED"));
      }
      else
      {
         if (egIe->t.fTeid.teid == 0)
         {
            QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_INITIAL_ATTACH);
            QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_PDN_CONN_REQUEST);
         }
         else 
         {
            pgwRemTeid = egIe->t.fTeid.teid;
            QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC);
         }

         remAddr.type = CM_IPV4ADDR_TYPE;
         qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,\
               &(remAddr.u.ipv4TptAddr.address));
         remAddr.u.ipv4TptAddr.port = qoSgwCb.echoCSap->localAddr.u.\
               ipv4TptAddr.port;
         ret = qoSgwFindNCreatePeer(&remAddr, &peer);
         if(ret != ROK)
         {
            *cause = EGT_GTPC_CAUSE_SYS_FAIL;
            QO_SGW_DBG_ERR(("Creation Of PDN Gateway Peer Entry, FAILED"));
         }
         else
         {
            peer->nodeType = QO_ENT_PGW;
         }
      }
   }

   /* Bearer Contexts to be created */
   if (ROK == ret)
   {
      /* Get the number of Bearer ID IEs */
      ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
      if(ret != ROK)
      {
         *cause = EGT_GTPC_CAUSE_SYS_FAIL;
         QO_SGW_DBG_ERR(("Retreving Bearer Occurence, FAILED"));
      }
      else if((ieOcr != 1) && 
             (QO_SGW_CHK_MASK(ueCb->bitmask,
             (QO_SGW_INITIAL_ATTACH | QO_SGW_PDN_CONN_REQUEST))))
      {
         /* Initial Attach/PDN Connectivity request can have only on Bearer */
         *cause = EGT_GTPC_CAUSE_SEMANTIC_ERR_TADOP;
         QO_SGW_DBG_ERR(("Inital Attach/PDN Connectivity Request With "
                        "Multiple Bearer Contexts"));
      }
      else
      {
         /* Get the EBI Id */
         egIe = NULLP;
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
         if (ret != ROK)
         {
            *cause = EGT_GTPC_CAUSE_SYS_FAIL;
            QO_SGW_DBG_ERR(("EBI IE Could Not Be Retrieved"));
         }
         else
         {
            QO_SGW_ALLOC((&bearer), sizeof(QoSgwBearerCb));
            if (NULL != bearer)
            {
               bearer->ebi = egIe->t.valUnSgnd8;
               bearer->isDefBearer = TRUE;
               bearerInd = QO_SGW_BEARER_IND(egIe->t.valUnSgnd8);
               if((bearerInd < 0) || (bearerInd >= QO_SGW_MAX_BEARERS))
               {
                  QO_SGW_DBG_ERR(("Bearer Id In Create Session Request"
                           " Is Not Valid"));
                  *cause = EGT_GTPC_CAUSE_SYS_FAIL;
                  ret = RFAILED;
                  QO_SGW_RETVALUE(ret);
               } 
               ueCb->bearerTable[bearerInd] = bearer;
               ueCb->bearerCnt += 1;
               if (ueCb->intf == QO_IF_S11_C)
               {
               ueCb->s11Tun->ebi = bearer->ebi;
               }
               else if(ueCb->intf == QO_IF_S4_C)
               {
                  ueCb->s4Tun->ebi = bearer->ebi;
               }
               else
               {
                  QO_SGW_DBG_ERR(("Invalid interface type"));
                  *cause = EGT_CAUSE_INVALID;
               }
               (*pdnCb)->ueCb = ueCb;
               (*pdnCb)->transId = transId;

               /*Store pdn in UE Cb */
               ueCb->bearerTable[bearerInd]->pdnCb = *pdnCb;

               QO_SGW_SET_BEARER_MAP((*pdnCb)->bearerMap, bearer->ebi);
            }
            else
            {
               QO_SGW_DBG_ERR(("Memory Allocation for Bearer CB"));
               *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
            }
         }

         /* In case of HO with SGW relocation, PGW S5/S8 should be present */
         if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC))
         {
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 3,\
                  &egIe);
            if (ROK == ret)
            {
               if(NULLP == bearer)
               {
                  QO_SGW_DBG_ERR(("Bearer control block is NULL"));
                  QO_SGW_RETVALUE(RFAILED);
               }   
               QO_SGW_DBG_INFO(("Creation Of S5/S8-U Default Tunnel"));
               ret = qoSgwCreateUTunnel(*pdnCb, &(bearer->s5s8UTun),\
                     QO_IF_S5_S8_DEF);
               if (ROK == ret)
               {
                  bearer->s5s8UTun->remTeid = egIe->t.fTeid.teid;
                  bearer->s5s8UTun->remAddr = &(peer->addr);
                  bearer->s5s8UTun->ebi = bearer->ebi;
               }
               else
               {
                  QO_SGW_DBG_ERR(("Creation Of S5/S8-U Default Tunnel"));
                  QO_SGW_RETVALUE(RFAILED);
               }
            }
         }
         /* Get the SGSN U FTEID */
         if(QO_IF_S4_C == ueCb->intf)
         {
           egIe = NULLP;
           ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 1, &egIe);
           if (ret != ROK)
           {
              *cause = EGT_GTPC_CAUSE_SYS_FAIL;
              QO_SGW_DBG_ERR(("Getting S4 U FTEID "));
           }
           else
           {
              QO_SGW_DBG_INFO(("Creation Of S4-U Default Tunnel"));
              ret = qoSgwCreateUTunnel(*pdnCb, &(bearer->s4UTun),\
                  QO_IF_S4_DEF);
              if (ROK == ret)
              {
                 bearer->s4UTun->remTeid = egIe->t.fTeid.teid;
                 qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,\
                 &(remAddr.u.ipv4TptAddr.address));
                 remAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.u.\
                                            ipv4TptAddr.port;
                 /* Adding SGSN peer information */
                 ret = qoSgwFindNCreatePeer(&remAddr, &sgsnuPeer);
                 if(ret == ROK)
                 {
                   sgsnuPeer->nodeType = QO_ENT_SGSN;
                   bearer->s4UTun->remAddr = &(sgsnuPeer->addr);
                 } 
                 bearer->s4UTun->ebi = bearer->ebi;
                 QO_SGW_DBG_INFO(("Sending Tunnel Add Request for S4 U to EGT Layer"));
                 ret = qoSgwEguTunnelAddReq(bearer, NULLP,\
                     EGT_INT_S4_SGW_GTP_U);
                 if(ret != ROK)
                 {
                    *cause = EGT_GTPC_CAUSE_SYS_FAIL;
                    QO_SGW_DBG_ERR(("Sending Tunnel Add Request for S4 U to EGT Layer"));
                 }
              }
              else
              {
                 QO_SGW_DBG_ERR(("Creation Of S5/S8-U Default Tunnel"));
                 QO_SGW_RETVALUE(RFAILED);
              }
           }
         }
      } /* if (ieOcr > 1 .. */
   } /* if (ret == OK) */

   /* Create the S5/S8 C-Plane tunnel and store the tunnel details */
   if (ROK == ret)
   {
      qoSgwGenerateCTransId(&transId);

      ret = qoSgwCreateTunnelTrans(qoSgwCb.pgwCSap, transId, &s5s8CTun);
      if (ret != ROK)
      {
         *cause = EGT_GTPC_CAUSE_SYS_FAIL;
         QO_SGW_DBG_ERR(("Creation Of S5/S8 C-Plane Tunnel, FAILED"));
      }
      else
      {
         s5s8CTun->plane    = QO_CONTROL;
         s5s8CTun->intf     = QO_IF_S5_S8_C;
         s5s8CTun->seqNum   = egMsg->msgHdr.seqNumber; 
         s5s8CTun->ueCb     = ueCb;
         s5s8CTun->pdnCb    = *pdnCb;
         if(NULLP != bearer)
         {   
            s5s8CTun->ebi      = bearer->ebi;
         }
         s5s8CTun->remAddr  = &(peer->addr);
         s5s8CTun->state    = QO_SGW_TUN_PROGRESS;
         s5s8CTun->remTeid  = pgwRemTeid;
         (*pdnCb)->s5s8CTun = s5s8CTun;
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwBldCSReq
 *
 *       Desc:  Creates Create Session Request message towards PGW
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldCSReq
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Create Session Request received from MME */
)
#else
PUBLIC S16 qoSgwBldCSReq(msg, s5s8Tun)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Create Session Request received from MME */
#endif
{
   U16            ret = ROK;
   EgIeLevelInfo  ieLvlInfo = {0};
   EgIe           egIe;
   QoSgwTunnelCb  *s5s8CTun = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwBearerCb  *bearer = NULLP;

   QO_SGW_TRC2(qoSgwBldCSReq);

   s5s8CTun = pdnCb->s5s8CTun;
   ueCb = s5s8CTun->ueCb;

   /* GTP Header */
   egMsg->msgHdr.version = 2;
   egMsg->msgHdr.teid = 0;
   egMsg->msgHdr.teidPres = PRSNT_NODEF;
   egMsg->msgHdr.msgType = EGT_GTPC_MSG_CS_REQ;

   /* Add IMSI */
   cmMemset((U8 *)&ieLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddImsi(egMsg, &ieLvlInfo, ueCb->ueId, ueCb->ueIdLen, 0);
   
   /* Add MSISDN */
   cmMemset((U8 *)&ieLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddMsisdn(egMsg,&ieLvlInfo, ueCb->msisdn, ueCb->msisdnLen, 0);

   /* SGW C F-TEID */
   egIe.t.fTeid.isIPv4Pres = TRUE;
   qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                     &(s5s8CTun->locAddr->u.ipv4TptAddr.address));
   egIe.t.fTeid.teid = 0; /* Note: filled by Gtp-C layer */
   egIe.t.fTeid.intfType = EGT_INT_S5_S8_SGW_GTP_C;
   ret = qoSgwEgtUtlAppendIe(egMsg, EG_FTEID_IETYPE, 0, EG_FTEID, &egIe);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Adding SGW F-TEID to GTP-C msg"));
   }

   /* S5/S8 SGW U F-TEID */
   if (ROK == ret)
   {
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
      bearer = ueCb->bearerTable[QO_SGW_BEARER_IND(s5s8CTun->ebi)];

      egIe.t.fTeid.intfType = EGT_INT_S5_S8_SGW_GTP_U;
      egIe.t.fTeid.teid = bearer->s5s8UTun->locTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                        &(bearer->s5s8UTun->locAddr->u.ipv4TptAddr.address));

      qoUtlEgtBuildEgIeHeader(&egIe, EG_FTEID_IETYPE, 2, EG_FTEID);

      /* sgw u f-teid lvl info */
      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 1, EG_FTEID_IETYPE, 2);
      ret = EgUtilGmAppendIe(egMsg, &ieLvlInfo, &egIe);
      if(ret != ROK)
      {
         QO_SGW_DBG_ERR(("Adding SGW  F-TEID to GTP-C msg, FAILED"));
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwRemCSReqIes
 *
 *       Desc:  Removes IEs specific to MME from Create Session Request
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwRemCSReqIes
(
EgMsg          *egMsg,     /* Create Session Request received from MME */
QoSgwTunnelCb  *s5s8Tun    /* Tunnel on which Create Sess Req to be sent */
)
#else
PUBLIC S16 qoSgwRemCSReqIes(egMsg, s5s8Tun)
EgMsg          *egMsg,     /* Create Session Request received from MME */
QoSgwTunnelCb  *s5s8Tun    /* Tunnel on which Create Sess Req to be sent */
#endif
{
   QoSgwUeCb            *ueCb = NULLP;
   EgIeLevelInfo        ieLvlInfo = {0};

   QO_SGW_TRC2(RemCreateSessionReqIes)
   ueCb = s5s8Tun->ueCb;

   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_IMSI_IETYPE, 0);
   EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);
   
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_MSISDN_IETYPE, 0);
   EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);

   /* MME F-TEID */
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_FTEID_IETYPE, 0);
   EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);

   /* PGW F-TEID */
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_FTEID_IETYPE, 1);
   EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);

   /* SGSN U F-TEID 
      This is removed as this information is not required to be passed to PGW */
   if(QO_IF_S4_C == ueCb->intf)
   {
     ieLvlInfo.level = 1;
     ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
     ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
     ieLvlInfo.ieInfo[0].ieInst = 0;
     ieLvlInfo.ieInfo[0].occrPres = TRUE;
     ieLvlInfo.ieInfo[0].occr = 1;

     ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
     ieLvlInfo.ieInfo[1].ieType = EG_FTEID_IETYPE;
     ieLvlInfo.ieInfo[1].ieInst = 1;
     EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);
   }
   /* LBI */
   if (QO_SGW_CHK_MASK(ueCb->bitmask,
      (QO_SGW_TAU_WITH_SGW_CHANGE | QO_SGW_S1_HO_WITH_SGW_RELOC)))
   {
      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_EBI_IETYPE, 0);
      EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);
   }

   QO_SGW_RETVALUE(ROK);
}


/***************************************************************
 *
 *       Fun:   qoSgwValCSRsp
 *
 *       Desc:  Validates and Stores Create Session Response IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValCSRsp
(
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause     /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValCSRsp(pdnCb, egMsg, cause)
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause     /* cause of failure (OUT) */
#endif
{
   S16                     ret = ROK;
   QoSgwTunnelCb           *s5s8CTun = NULLP;
   EgIe                    *egIe = NULLP;
   S16                     ieOcr = 0;
   QoSgwUeCb               *ueCb = NULLP;
   U32                     bearerInd = 0;
   QoSgwTunnelCb           *s5s8UTun = NULLP;

   QO_SGW_TRC2(qoSgwValCSRsp)

   ueCb = pdnCb->ueCb;
   s5s8CTun = pdnCb->s5s8CTun;

   /* cause */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Retrieval Of Cause IE, FAILED"));
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
   }
   else
   {
      if (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ)
      {
         *cause = egIe->t.valUnSgnd8;
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   /* PGW F-TEID */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 1);
   if (ret != ROK)
   {
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
      QO_SGW_DBG_ERR(("Retrieval Of PGW FTEID IE"));
   }
   else
   {
      s5s8CTun->remAddr->type = CM_IPV4ADDR_TYPE;
      s5s8CTun->remTeid = egIe->t.fTeid.teid;
   }

   /* Bearer Contexts to be created */
   if (ROK == ret)
   {
      /* Get the number of Bearer Contexts IEs */
      ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
      if ((ieOcr != 1) && 
          (QO_SGW_CHK_MASK(ueCb->bitmask,
          (QO_SGW_INITIAL_ATTACH | QO_SGW_INITIAL_ATTACH))))
      {
         /* Initial Attach/PDN Connectivity request can have only on Bearer */
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
         QO_SGW_DBG_ERR(("Inital Attach/PDN Connectivity Request With "
                        "Multiple Bearer Contexts"));
      }
      else
      {
         /*Only one bearer is expected*/
         /* Get the EBI Id */
         egIe = NULLP;
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
         if (ret != ROK)
         {
            QO_SGW_DBG_ERR(("EBI IE Could Not Be Retrieved"));
            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            ret = RFAILED;
         }
         else
         {
            bearerInd = QO_SGW_BEARER_IND(egIe->t.valUnSgnd8);
            if((bearerInd < 0) || (bearerInd >= QO_SGW_MAX_BEARERS))
            {
               QO_SGW_DBG_ERR(("Bearer Id In Create Session Request"
                              " Is Not Valid"));
               ret = RFAILED;
               QO_SGW_RETVALUE(ret);
            }   
            if (ueCb->bearerTable[bearerInd] == NULLP) 
            {
               *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
               QO_SGW_DBG_ERR(("Bearer Contexts In Create Session Request"
                              " Is Not Matching With Create Session Rsp"));
               ret = RFAILED;
            }
            else
            {
               ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 2,
                                          &egIe);
               if(ROK != ret)
               {
                  ret = RFAILED;
                  QO_SGW_DBG_ERR(("F-TEID IE Could Not Be Retrieved"));
                  *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
               }
               else
               {
                  s5s8UTun = ueCb->bearerTable[bearerInd]->s5s8UTun;
                  s5s8UTun->remTeid = egIe->t.fTeid.teid;
                  s5s8UTun->remAddr = s5s8CTun->remAddr;
               }
            }
         }
      } /* if (ieOcr > 1 .. */
   } /* if (ret == OK) */

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwValMBRsp
 *
 *       Desc:  Validates and Stores Modify Bearer Response IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValMBRsp
(
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *pMbRsp,   /* GTP-C message (IN) */
U32             *cause,    /* cause of failure (OUT) */
Bool            *pFwdRsp   /* Forward the response to remove Node */
)
#else
PUBLIC S16 qoSgwValMBRsp(pdnCb, pMbRsp, cause, pFwdRsp)
QoSgwPdnCb      *pdnCb;    /* PDN Control Block (OUT) */
EgMsg           *pMbRsp;    /* GTP-C message (IN) */
U32             *cause;     /* cause of failure (OUT) */
Bool            *pFwdRsp;  /* Forward the response to remove Node */
#endif
{
   S16                     ret = ROK;
   EgIe                    *egIe = NULLP;
   S16                     ieOcr = 0;
   QoSgwBearerCb           *pBearer = NULLP;

   QO_SGW_TRC2(qoSgwValMBRsp)

   QO_SGW_DBG_INFO(("Validating Modify Bearer Response"));

   *pFwdRsp = FALSE;

   /* cause */
   QO_SGW_DBG_INFO(("Retrieving Cause IE"));
   ret = qoSgwEgtUtlGetIE(pMbRsp, &egIe, EG_CAUSE_IETYPE, 0);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Retrieving Cause IE"));
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
   }
   else
   {
      if (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ)
      {
         *pFwdRsp = TRUE;
         *cause = egIe->t.valUnSgnd8;
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   if (QO_SGW_CHK_MASK(pdnCb->ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC))
   {
      /* If SGW is relocated, S5/S8 U-TEID has changed. Bearer Contexts
       * to be modified IE is sent fo PGW, so corresponding Bearer
       * Contexts Modified IE is expected from PGW
       * Handling only for Default Bearer
       */
      QO_SGW_DBG_INFO(("Retrieving Bearer Context Modified IE count"));
      ret = qoSgwEgtUtlGetIeCnt(pMbRsp, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
      if (ROK == ret && ieOcr != 0)
      {
         /* EBI */
         QO_SGW_DBG_INFO(("Retrieving EBI in Bearer Context Modified IE"));
         ret = qoSgwEgtUtlGetIeInBCtx(pMbRsp, 0, 1, EG_EBI_IETYPE, 0, &egIe);
         if (ROK == ret && egIe != NULLP)
         {
            QO_SGW_GET_BEARER(pdnCb->ueCb->bearerTable, egIe->t.valUnSgnd8,\
                  pBearer);
            if (NULLP == pBearer)
            {
               ret = RFAILED;
               QO_SGW_DBG_ERR(("Bearer Context CB not found for EBI: [%d]",\
                     egIe->t.valUnSgnd8));
            }
         }
         else
         {
            QO_SGW_DBG_ERR(("Retrieving EBI in Bearer Context Modified IE"));
            ret = RFAILED;
         }
      }
      else
      {
         QO_SGW_DBG_ERR(("Retrieving Bearer Context Modified IE"));
         ret = RFAILED;
      }
   }

   QO_SGW_RETVALUE(ROK);
}


/**************************************************************************//**
 *
 * @brief 
 *    Builds Create Session Response. Allocates memory for GTP message
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[out]  pCsRsp
 *    Pointer to GTP message structure containing Create Session Response
 * 
 * @return
 *    SUCCESS if Create Session Response created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 qoSgwBldHOCSRsp
(
QoSgwPdnCb     *pPdnCb,
EgMsg          **pCsRsp
)
{
   U16            ret      = ROK;
   Mem            memInfo  = {0, 0};
   QoSgwTunnelCb  *pS11Tun = NULLP;
   EgIe           egIe;
   QoSgwBearerCb  *pDefBearer = NULLP;

   QO_SGW_TRC2(qoSgwBldHOCSRsp);
   QO_SGW_DBG_INFO(("Building Create Session Response for S1 HO with SGW"\
         "Relocation"));

   pS11Tun = pPdnCb->ueCb->s11Tun;
   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   if (qoUtlAllocEgMsg(pCsRsp, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Memory Allocation: EgMsg"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* GTP Header */
   egUtlBuildEgHeader(&((*pCsRsp)->msgHdr), pS11Tun->remTeid,\
         EGT_GTPC_MSG_CS_RSP, pS11Tun->seqNum); 

   /* Cause */
   QO_SGW_DBG_ERR(("Adding Cause IE"));
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   egIe.t.cause.causeVal = EGT_GTPC_CAUSE_ACC_REQ;
   ret = qoSgwEgtUtlAppendIe(*pCsRsp, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);
   if (ret != ROK)
   {
      QO_SGW_DBG_ERR(("Adding Cause IE"));
   }

   if (ROK == ret)
   {
      /* SGW F-TEID C-Plane */
      QO_SGW_DBG_INFO(("Adding SGW-C S11 C-FTEID IE"));
      egIe.ieInst = 0;
      egIe.t.fTeid.teid = pS11Tun->locTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S11_S4_SGW_GTP_C;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
            &(pS11Tun->locAddr->u.ipv4TptAddr.address));
      ret = qoSgwEgtUtlAppendIe(*pCsRsp, EG_FTEID_IETYPE, 0, EG_FTEID, &egIe);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("Adding SGW-C S11 C-FTEID IE"));
      }
   }

   QO_SGW_GET_BEARER(pPdnCb->ueCb->bearerTable, pS11Tun->ebi, pDefBearer);

   /* Bearer Contexts Created */
   if (ROK == ret && NULLP != pDefBearer)
   {
      /* Only default bearer is handled */
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      QO_SGW_DBG_INFO(("Adding Bearer Context Created IE"));
      ret = qoSgwEgtUtlAppendIe(*pCsRsp, EG_BEARER_CNTX_IETYPE, 0,\
            EG_GROUPED, &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding Bearer Context Created IE"));
      }

      /* Cause IE */
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding Cause IE into Bearer Context Created IE"));
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_ACC_REQ;
         ret = qoSgwEgtUtlAddBCtxIe(*pCsRsp, EG_CAUSE_IETYPE, 0,\
               EG_CAUSE, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding Cause IE into Bearer Context Created IE"));
         }
      }

      /* Add EBI ie into Bearer Context */
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding EBI IE into Bearer Context Created IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.valUnSgnd8 = pS11Tun->ebi;
         ret = qoSgwEgtUtlAddBCtxIe(*pCsRsp, EG_EBI_IETYPE, 0,\
               EG_UNSIGNED_8, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding EBI IE into Bearer Context Created IE"));
         }
      }

      /* Add S5/S8 SGW U F-TEID */
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding SGW S1-U FTEID IE into Bearer Context "\
               "Created IE"));
         egIe.ieInst = 0;
         egIe.t.fTeid.teid = pDefBearer->s1UTun->locTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_S1_U_SGW_GTP_U;
         qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(pS11Tun->locAddr->u.ipv4TptAddr.address));
         ret = qoSgwEgtUtlAddBCtxIe(*pCsRsp, EG_FTEID_IETYPE, 0,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding SGW S1-U FTEID IE into Bearer Context "\
                  "Created IE"));
         }
      }
   }
   else
   {
      QO_SGW_DBG_ERR(("Default Bearer Context details Not found"));
   }

   /* Recovery IE */
   if (ROK == ret)
   {
      QO_SGW_DBG_INFO(("Adding Recovery IE"));
      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.t.valUnSgnd32 = 0;
      qoSgwEgtUtlAppendIe(*pCsRsp, EG_RECOVERY_IETYPE, 0, EG_UNSIGNED_32,\
            &egIe);
      if (ret != ROK)
      {
         QO_SGW_DBG_ERR(("Adding Recovery IE"));
      }
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(pCsRsp);
   }

   QO_SGW_RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief: 
 *    Builds Create Session Response message towards MME using
 *    Using Create Session Response received from PGW as a part of
 *    Initial Attach Procedure
 *
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which Initial Attach is being handled
 * @param[inout] egMsg
 *    Pointer to GTP message structure containing Create Session Response
 *    received from MME
 *
 * @return
 *    SUCCESS if Create Session Response created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldIACSRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Create Session Response received from MME */
)
#else
PUBLIC S16 qoSgwBldIACSRsp(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg;     /* Create Session Response received from MME */
#endif
{
   U16            ret = ROK;
   EgIeLevelInfo  ieLvlInfo = {0};
   EgIe           egIe;
   QoSgwTunnelCb  *s11Tun = NULLP;
   QoSgwTunnelCb  *s4Tun = NULLP;
   QoSgwTunnelCb  *s5s8CTun = NULLP;
   QoSgwUeCb      *ueCb = NULLP;
   QoSgwBearerCb  *bearer = NULLP;

   QO_SGW_TRC2(qoSgwBldIACSRsp);
   QO_SGW_DBG_INFO(("Buildin Create Session Response"));

   ueCb = pdnCb->ueCb;
   s5s8CTun = pdnCb->s5s8CTun;
   s11Tun = ueCb->s11Tun;
   s4Tun = ueCb->s4Tun;

   /* GTP Header */
   egMsg->msgHdr.version = 2;
   if(QO_IF_S11_C == ueCb->intf)
   {
   egMsg->msgHdr.teid = s11Tun->remTeid;
   }
   else if(QO_IF_S4_C == ueCb->intf)
   {
      egMsg->msgHdr.teid = s4Tun->remTeid;
   }
   egMsg->msgHdr.teidPres = PRSNT_DEF;
   egMsg->msgHdr.msgType = EGT_GTPC_MSG_CS_RSP;

   /* SGW C F-TEID */
   egIe.t.fTeid.isIPv4Pres = TRUE;
   if(QO_IF_S11_C == ueCb->intf)
   {
   qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                     &(s11Tun->locAddr->u.ipv4TptAddr.address));
   egIe.t.fTeid.teid = pdnCb->ueCb->s11Tun->locTeid;
   egIe.t.fTeid.intfType = EGT_INT_S11_S4_SGW_GTP_C;
   }
   else if(QO_IF_S4_C == ueCb->intf)
   {
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
            &(s4Tun->locAddr->u.ipv4TptAddr.address));
      egIe.t.fTeid.teid = pdnCb->ueCb->s4Tun->locTeid;
      egIe.t.fTeid.intfType = EGT_INT_S11_S4_SGW_GTP_C;
   }
   qoUtlEgtBuildEgIeHeader(&egIe, EG_FTEID_IETYPE, 0, EG_FTEID);
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_FTEID_IETYPE, 0);

   ret = EgUtilGmAppendIe(egMsg, &ieLvlInfo, &egIe);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Adding SGW F-TEID to GTP-C msg, FAILED"));
   }

   if (ROK == ret)
   {
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
      bearer = ueCb->bearerTable[QO_SGW_BEARER_IND(s5s8CTun->ebi)];

   /* S1 SGW U F-TEID */
      if(QO_IF_S11_C == ueCb->intf)
      {
      egIe.t.fTeid.intfType = EGT_INT_S1_U_SGW_GTP_U;
      egIe.t.fTeid.teid = bearer->s1UTun->locTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                        &(bearer->s1UTun->locAddr->u.ipv4TptAddr.address));
         qoUtlEgtBuildEgIeHeader(&egIe, EG_FTEID_IETYPE, 0, EG_FTEID);
      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 1, EG_FTEID_IETYPE, 0);
      }
      /* S4 SGW U F-TEID */
      else if(QO_IF_S4_C == ueCb->intf)
      {
         egIe.t.fTeid.intfType = EGT_INT_S4_SGW_GTP_U;
         egIe.t.fTeid.teid = bearer->s4UTun->locTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
               &(bearer->s4UTun->locAddr->u.ipv4TptAddr.address));
         qoUtlEgtBuildEgIeHeader(&egIe, EG_FTEID_IETYPE, 1, EG_FTEID);
         /* sgw u f-teid lvl info */
         QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
         QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 1, EG_FTEID_IETYPE, 1);
      }

      ret = EgUtilGmAppendIe(egMsg, &ieLvlInfo, &egIe);
      if(ret != ROK)
      {
         QO_SGW_DBG_ERR(("Adding SGW  F-TEID to GTP-C msg, FAILED"));
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwValMBReq
 *
 *       Desc:  Validates and Stores Modify bearer Request IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValMBReq
(
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause     /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValMBReq(ueCb, pdnCb, egMsg, cause)
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause     /* cause of failure (OUT) */
#endif
{
   S16                     ret = ROK;
   QoSgwBearerCb           *bearer = NULLP;
   EgIe                    *egIe = NULLP;
   S16                     ieOcr = 0;
   QoSgwPeerCb             *peer = NULLP;
   CmTptAddr               eNbAddr = {0};
   QoSgwUeCb               *ueCb = NULLP;
   U32                     bCnt = 0;

   QO_SGW_TRC2(qoSgwValMBReq);

   /* required for response messages on this tunnel */
   ueCb = pdnCb->ueCb;
   
   if( ueCb->s11Tun )
   { 
   ueCb->s11Tun->transId.val = egMsg->msgHdr.seqNumber;
   }
   else
   {
      ueCb->s4Tun->transId.val = egMsg->msgHdr.seqNumber;
   }
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_ULI_IETYPE, 0);
   if (ret == ROK && NULLP != egIe)
   {
      QO_SGW_DBG_INFO(("Storing ULI for the UE"));

      if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_ULI_PRESENT))
      {
         if (cmMemcmp((U8 *)&egIe->t.uli, (U8 *)&ueCb->uli,\
            sizeof(EgUli)) != 0)
         {
            /* This bitmask will be reset to QO_SGW_ULI_PRESENT
             * once SGW sends a Modify Bearer Request to PGW
             */
            QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED)
            ueCb->uli = egIe->t.uli;
         }
      }
      else
      {
         QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED)
         ueCb->uli = egIe->t.uli;
      }
   }
   else
   {
      ret = ROK;
      QO_SGW_DBG_INFO(("ULI Not Present"));
   }
   
   /* Just update the eNB S1-U F-Teid */
   /* Get the number of Bearer Context IEs */
   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
   if (ieOcr == 0)
   {
      *cause = EGT_GTPC_CAUSE_SEMANTIC_ERR_TADOP;
      QO_SGW_DBG_ERR(("Bearer Context IE Not present"));
      QO_SGW_RETVALUE(RFAILED);
   }

   for (bCnt = 0; bCnt < ieOcr; bCnt++)
   {
      /* Get the EBI Id */
      ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
      if (ret != ROK || egIe == NULLP)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         QO_SGW_DBG_ERR(("EBI IE Could Not Be Retrieved"));
         break;
      }
      else
      {
         QO_SGW_GET_BEARER(ueCb->bearerTable, egIe->t.valUnSgnd8, bearer);
         if (NULLP != bearer)
         {
            /* Now get the eNB S1-U F-TEID */
            egIe = NULLP;
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 0,\
                  &egIe);
            if (ret != ROK || egIe == NULLP)
            {
               QO_SGW_DBG_ERR(("Retrieval Of S1-U F-TEID From Bearer Context"));
               *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
               ret = RFAILED;
               break;
            }
            else
            {
               /* in case of TAU set the state of tunnel as pending 
                * for chaning the remote teid */
               if (pdnCb->state == QO_SGW_PDN_CREATE)
               {
                  bearer->s1UTun->remTeid = egIe->t.fTeid.teid;
                  eNbAddr.type = CM_TPTADDR_IPV4;
                  qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                        &(eNbAddr.u.ipv4TptAddr.address));
                  eNbAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.u.\
                                               ipv4TptAddr.port;
                  ret = qoSgwFindNCreatePeer(&eNbAddr, &peer);
                  peer->nodeType = QO_ENT_ENB;
                  bearer->s1UTun->remAddr = &(peer->addr);
                  QO_SGW_SET_MASK(bearer->op_mask, QO_SGW_CRT_BEARER);
               }
               else if (pdnCb->state == QO_SGW_PDN_CONNECTED)
               {
                  QO_SGW_SET_MASK(bearer->op_mask, QO_SGW_MOD_BEARER);

                  eNbAddr.type = CM_INET_IPV4ADDR_TYPE;
                  eNbAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.u.\
                        ipv4TptAddr.port;
                  qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                        &(eNbAddr.u.ipv4TptAddr.address));

                  qoSgwFindNCreatePeer(&eNbAddr, &peer);
                  bearer->s1UTun->newRemAddr = &(peer->addr);
                  bearer->s1UTun->newRemTeid = egIe->t.fTeid.teid;
               }
            }
         }
         else
         {
            ret = RFAILED;
            *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
            QO_SGW_DBG_ERR(("Invalid EBI Value Received"));
            break;
         }
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwValMBReqfrmSgsnIrat 
 *
 *       Desc:  Validates and Stores Modify bearer Request IEs into a
 *              a temperory data store.
 *              qoSgwValMBReqfrmSgsnIrat this function get called when SGW 
 *              receives the modify  bearer request from the SGSN.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValMBReqfrmSgsnIrat 
(
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause,     /* cause of failure (OUT) */
CmTptAddr      *remAddr   /* remote address */
)
#else
PUBLIC S16 qoSgwValMBReqfrmSgsnIrat(pdnCb, egMsg, cause, remAddr)
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause,     /* cause of failure (OUT) */
CmTptAddr      *remAddr,   /* remote address */
#endif
{
   S16                     ret = ROK;
   QoSgwBearerCb           *bearer = NULLP;
   EgIe                    *egIe = NULLP;
   S16                     ieOcr = 0;
   QoSgwPeerCb             *peer = NULLP;
   CmTptAddr               SgsnAddr = {0};
   QoSgwUeCb               *ueCb = NULLP;
   U32                     bCnt = 0;
   QoSgwTunnelCb           *s4Tun = NULLP;
   U32                     lclTeid;

   QO_SGW_TRC2(qoSgwValMBReqfrmSgsnIrat);


   /* required for response messages on this tunnel */
   ueCb = pdnCb->ueCb;
   lclTeid = ueCb->s11Tun->locTeid;

   /* creating new tunnel towards the SGSN C-Plane */
   ret =  qoSgwFindNCreateTunnel(qoSgwCb.sgsnCSap, lclTeid, &s4Tun);
   if (ret != ROK)
   {
      QO_SGW_DBG_ERR(("GTP-C Tunnel Control Block Creation."));
      *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
   }
   if (ret == ROK)
   {
      s4Tun->ueCb = ueCb;
      s4Tun->plane = QO_CONTROL;
      s4Tun->state = QO_SGW_TUN_PROGRESS; 
      s4Tun->intf = QO_IF_S4_C;
      s4Tun->pdnCb = pdnCb;
      s4Tun->seqNum = egMsg->msgHdr.seqNumber;
      ueCb->s4Tun = s4Tun;
      ueCb->intf = QO_IF_S4_C;
   }

   /* Getting S4 SGSN C-plane TEID */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 0);
   if (ROK != ret || NULLP == egIe)
   {
      /* Cannot send a failure response with 0 remote teid. So drop
       * the message
       */
      QO_SGW_DBG_ERR(("Invalid modify request from the SGSN droping "\
               "the message"));

      if((ret = EgUtilDeAllocGmMsg(&egMsg)) == EG_UTIL_ERR_INV_PARAM)
      {  
         /* 
          * If the egMsg pointer is freed already and we are passing NULL pointer
          * to free, this code leg should hit.
          */
         QO_SGW_DBG_INFO(("egMsg is pointer is NULL"));
      }

      QO_SGW_RETVALUE(ROK);
   }
   else
   {
      ueCb->s4Tun->newRemTeid = egIe->t.fTeid.teid;
      ueCb->s4Tun->newRemAddr = remAddr;
   }

   pdnCb->state = QO_SGW_PDN_MODIFY;

   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_ULI_IETYPE, 0);
   if (ret == ROK && NULLP != egIe)
   {
      QO_SGW_DBG_INFO(("Storing ULI for the UE"));

      if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_ULI_PRESENT))
      {
         if (cmMemcmp((U8 *)&egIe->t.uli, (U8 *)&ueCb->uli,\
                  sizeof(EgUli)) != 0)
         {
            /* This bitmask will be reset to QO_SGW_ULI_PRESENT
             * once SGW sends a Modify Bearer Request to PGW
             */
            QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED)
               ueCb->uli = egIe->t.uli;
         }
      }
      else
      {
         QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED)
            ueCb->uli = egIe->t.uli;
      }
   }
   else
   {
      ret = ROK;
      QO_SGW_DBG_INFO(("ULI Not Present"));
   }

   /* Update the U-Plane F-Teid */
   /* Get the number of Bearer Context IEs */
   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
   if (ieOcr == 0)
   {
      *cause = EGT_GTPC_CAUSE_SEMANTIC_ERR_TADOP;
      QO_SGW_DBG_ERR(("Bearer Context IE Not present"));
      QO_SGW_RETVALUE(RFAILED);
   }

   for (bCnt = 0; bCnt < ieOcr; bCnt++)
   {
      /* Get the EBI Id */
      ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
      if (ret != ROK || egIe == NULLP)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         QO_SGW_DBG_ERR(("EBI IE Could Not Be Retrieved"));
         break;
      }
      else
      {
         QO_SGW_GET_BEARER(ueCb->bearerTable, egIe->t.valUnSgnd8, bearer);
         if (NULLP != bearer)
         {
            /* Now get the S4-Uplane F-TEID */
            egIe = NULLP;
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 3,\
                  &egIe);
            if (ret != ROK || egIe == NULLP)
            {
               QO_SGW_DBG_ERR(("Retrieval Of S4-U F-TEID From Bearer Context FAILED"));
               *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
               ret = RFAILED;
               break;
            }
            else
            {
               /* Copying the existing S1U-Tunnel structure to S4U-Tunnel */
               if (pdnCb->state == QO_SGW_PDN_MODIFY)
               {
                  QO_SGW_SET_MASK(bearer->op_mask, QO_SGW_MOD_BEARER);
                  bearer->s4UTun = bearer->s1UTun;
                  bearer->s1UTun = NULLP;
                  bearer->s4UTun->intf = QO_IF_S4_DEF;
              /*  bearer->s4UTun->ebi = bearer->s1UTun->ebi;
                  bearer->s4UTun->plane = QO_USER;
                  bearer->s4UTun->locTeid = bearer->s1UTun->locTeid;
                  bearer->s4UTun->locAddr = bearer->s1UTun->locAddr;
               */
                  SgsnAddr.type = CM_TPTADDR_IPV4;
                  qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                        &(SgsnAddr.u.ipv4TptAddr.address));
                  SgsnAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.u.\
                                            ipv4TptAddr.port;
                  ret = qoSgwFindNCreatePeer(&SgsnAddr, &peer);
                  peer->nodeType = QO_ENT_SGSN;
                  bearer->s4UTun->newRemTeid = egIe->t.fTeid.teid;
                  /*
                  bearer->s4UTun->remTeid = bearer->s1UTun->remTeid;
                  bearer->s4UTun->remAddr = bearer->s1UTun->remAddr;
                  */
                  bearer->s4UTun->newRemAddr = &(peer->addr);
               }
            }
         }
         else
         {
            ret = RFAILED;
            *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
            QO_SGW_DBG_ERR(("Invalid EBI Value Received"));
            break;
         }
      }
   }
   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwValMBReqfrmMmeIrat 
 *
 *       Desc:  Validates and Stores Modify bearer Request IEs into a
 *              a temperory data store.
 *              qoSgwValMBReqfrmSgsnIrat this function get called when SGW 
 *              receives the modify  bearer request from the SGSN.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValMBReqfrmMmeIrat 
(
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause,     /* cause of failure (OUT) */
CmTptAddr      *remAddr   /* remote address */
)
#else
PUBLIC S16 qoSgwValMBReqfrmMmeIrat(pdnCb, egMsg, cause, remAddr)
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause,     /* cause of failure (OUT) */
CmTptAddr      *remAddr,   /* remote address */
#endif
{
   S16                     ret = ROK;
   QoSgwBearerCb           *bearer = NULLP;
   EgIe                    *egIe = NULLP;
   S16                     ieOcr = 0;
   QoSgwPeerCb             *peer = NULLP;
   CmTptAddr               enbAddr = {0};
   QoSgwUeCb               *ueCb = NULLP;
   U32                     bCnt = 0;
   QoSgwTunnelCb           *s11Tun = NULLP;
   U32                     lclTeid;

   QO_SGW_TRC2(qoSgwValMBReqfrmMmeIrat);


   /* required for response messages on this tunnel */
   ueCb = pdnCb->ueCb;
   lclTeid = ueCb->s4Tun->locTeid;

   /* creating new tunnel towards the SGSN C-Plane */
   ret =  qoSgwFindNCreateTunnel(qoSgwCb.mmeCSap, lclTeid, &s11Tun);
   if (ret != ROK)
   {
      QO_SGW_DBG_ERR(("GTP-C Tunnel Control Block Creation."));
      *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
   }
   if (ret == ROK)
   {
      s11Tun->ueCb = ueCb;
      s11Tun->plane = QO_CONTROL;
      s11Tun->state = QO_SGW_TUN_PROGRESS; 
      s11Tun->intf = QO_IF_S11_C;
      s11Tun->pdnCb = pdnCb;
      s11Tun->seqNum = egMsg->msgHdr.seqNumber;
      s11Tun->transId.pres = PRSNT_NODEF;
      s11Tun->transId.val = egMsg->msgHdr.seqNumber;
      ueCb->s11Tun = s11Tun;
      ueCb->intf = QO_IF_S11_C;
   }

   /* Getting S11 MME C-plane TEID */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_FTEID_IETYPE, 0);
   if (ROK != ret || NULLP == egIe)
   {
      /* Cannot send a failure response with 0 remote teid. So drop
       * the message
       */
      QO_SGW_DBG_ERR(("Invalid modify request from the MME droping "\
               "the message"));

      if((ret = EgUtilDeAllocGmMsg(&egMsg)) == EG_UTIL_ERR_INV_PARAM)
      {  
         /* 
          * If the egMsg pointer is freed already and we are passing NULL pointer
          * to free, this code leg should hit.
          */
         QO_SGW_DBG_INFO(("egMsg is pointer is NULL"));
      }

      QO_SGW_RETVALUE(ROK);
   }
   else
   {
      ueCb->s11Tun->newRemTeid = egIe->t.fTeid.teid;
      ueCb->s11Tun->newRemAddr = remAddr;
   }

   pdnCb->state = QO_SGW_PDN_MODIFY;

   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_ULI_IETYPE, 0);
   if (ret == ROK && NULLP != egIe)
   {
      QO_SGW_DBG_INFO(("Storing ULI for the UE"));

      if (QO_SGW_CHK_MASK(ueCb->bitmask, QO_SGW_ULI_PRESENT))
      {
         if (cmMemcmp((U8 *)&egIe->t.uli, (U8 *)&ueCb->uli,\
                  sizeof(EgUli)) != 0)
         {
            /* This bitmask will be reset to QO_SGW_ULI_PRESENT
             * once SGW sends a Modify Bearer Request to PGW
             */
            QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED)
               ueCb->uli = egIe->t.uli;
         }
      }
      else
      {
         QO_SGW_SET_MASK(ueCb->bitmask, QO_SGW_ULI_CHANGED)
            ueCb->uli = egIe->t.uli;
      }
   }
   else
   {
      ret = ROK;
      QO_SGW_DBG_INFO(("ULI Not Present"));
   }

   /* Update the U-Plane F-Teid */
   /* Get the number of Bearer Context IEs */
   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
   if (ieOcr == 0)
   {
      *cause = EGT_GTPC_CAUSE_SEMANTIC_ERR_TADOP;
      QO_SGW_DBG_ERR(("Bearer Context IE Not present"));
      QO_SGW_RETVALUE(RFAILED);
   }

   for (bCnt = 0; bCnt < ieOcr; bCnt++)
   {
      /* Get the EBI Id */
      ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_EBI_IETYPE, 0, &egIe);
      if (ret != ROK || egIe == NULLP)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
         QO_SGW_DBG_ERR(("EBI IE Could Not Be Retrieved"));
         break;
      }
      else
      {
         QO_SGW_GET_BEARER(ueCb->bearerTable, egIe->t.valUnSgnd8, bearer);
         if (NULLP != bearer)
         {
            /* Now get the S11-Uplane F-TEID */
            egIe = NULLP;
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, 1, EG_FTEID_IETYPE, 0,\
                  &egIe);
            if (ret != ROK || egIe == NULLP)
            {
               QO_SGW_DBG_ERR(("Retrieval Of S11-U F-TEID From "\
                  "Bearer Context FAILED"));
               *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
               ret = RFAILED;
               break;
            }
            else
            {
               /* Copying the existing S1U-Tunnel structure to S4U-Tunnel */
               if (pdnCb->state == QO_SGW_PDN_MODIFY)
               {
                  QO_SGW_SET_MASK(bearer->op_mask, QO_SGW_MOD_BEARER);
                  bearer->s1UTun = bearer->s4UTun;
                  bearer->s4UTun = NULLP;
                  bearer->s1UTun->intf = QO_IF_S1U_DEF;
               /*  bearer->s4UTun->ebi = bearer->s1UTun->ebi;
                              bearer->s4UTun->plane = QO_USER;
                              bearer->s4UTun->locTeid = bearer->s1UTun->locTeid;
                              bearer->s4UTun->locAddr = bearer->s1UTun->locAddr;
                        */
                  enbAddr.type = CM_TPTADDR_IPV4;
                  qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                        &(enbAddr.u.ipv4TptAddr.address));
                  enbAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.u.\
                                            ipv4TptAddr.port;
                  ret = qoSgwFindNCreatePeer(&enbAddr, &peer);
                  peer->nodeType = QO_ENT_ENB;
                  bearer->s1UTun->newRemTeid = egIe->t.fTeid.teid;
                  /*
                              bearer->s4UTun->remTeid = bearer->s1UTun->remTeid;
                              bearer->s4UTun->remAddr = bearer->s1UTun->remAddr;
                             */
                  bearer->s1UTun->newRemAddr = &(peer->addr);
               }
            }
         }
         else
         {
            ret = RFAILED;
            *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
            QO_SGW_DBG_ERR(("Invalid EBI Value Received"));
            break;
         }
      }
   }
   QO_SGW_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Builds Modify Bearer Request.
 *    The IEs encoded are with respect to S1 Handover with SGW relocation.
 *    Only default bearer is handled
 *    Of S1 HO without SGW change, and ULI change
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which S1 HO with SGW relocation is being
 *    handled
 * @param[out]  ppMbReq
 *    Pointer to GTP message structure containing Modify Bearer Request
 *    received from MME
 * 
 * @return
 *    SUCCESS if Modify Bearer Request created successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 qoSgwBldMBReq
(
QoSgwPdnCb     *pPdnCb,
EgMsg          **ppMbReq
)
{
   EgIe           egIe;
   S16            ret         = ROK;
   QoSgwTunnelCb  *pS5S8CTun  = NULLP;
   EgMsg          *pMbReq     = NULLP;
   Mem            memInfo     = {0};
   QoSgwBearerCb  *defBearer  = NULLP;

   QO_SGW_TRC2(qoSgwBldMBReq);

   QO_SGW_DBG_INFO(("Building Modify Bearer Request message"));

   QO_SGW_GET_BEARER(pPdnCb->ueCb->bearerTable, pPdnCb->s5s8CTun->ebi,\
         defBearer);
   if (NULLP == defBearer)
   {
      QO_SGW_DBG_ERR(("Default Bearer CB Not found"));
      QO_SGW_RETVALUE(RFAILED);
   }

   pS5S8CTun = pPdnCb->s5s8CTun;
   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;
   if (qoUtlAllocEgMsg(ppMbReq, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Memory Allocation for GTP Message"));
      QO_SGW_RETVALUE(RFAILED);
   }

   pMbReq = *ppMbReq;
   egUtlBuildEgHeader(&pMbReq->msgHdr, pS5S8CTun->remTeid,\
         EGT_GTPC_MSG_MB_REQ, pS5S8CTun->transId.val); 

   /* ULI */
   if (QO_SGW_CHK_MASK(pPdnCb->ueCb->bitmask, QO_SGW_ULI_CHANGED))
   {
      QO_SGW_DBG_INFO(("Adding ULI IE"));

      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.ieInst = 0;
      egIe.t.uli.isTAIPres = TRUE;
      egIe.t.uli.tai = pPdnCb->ueCb->uli.tai;
      egIe.t.uli.isECGIPres = TRUE;
      egIe.t.uli.ecgi = pPdnCb->ueCb->uli.ecgi;

      ret = qoSgwEgtUtlAppendIe(pMbReq, EG_ULI_IETYPE, 0, EG_ULI_DATA, &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding ULI IE"));
      }
      
      QO_SGW_UNSET_MASK(pPdnCb->ueCb->bitmask, QO_SGW_ULI_CHANGED);
   }

   /* Send S5/S8 SGW C-TEID only if SGW is relocated during HO */
   if (ROK == ret && \
      (QO_SGW_CHK_MASK(pPdnCb->ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC)))
   {
      /* SGW F-TEID IE */
      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.ieInst = 0;
      egIe.t.fTeid.teid = 0;  /* This TEID will be given by EGT layer
                               * when Modify Bearer Response is received
                               */
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S5_S8_SGW_GTP_C;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
            &(pS5S8CTun->locAddr->u.ipv4TptAddr.address));

      QO_SGW_DBG_INFO(("Adding SGW C-FTEID IE"));
      ret = qoSgwEgtUtlAppendIe(pMbReq, EG_FTEID_IETYPE, 0, EG_FTEID, &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding SGW C-FTEID IE"));
      }
   }

   /* Bearer Contexts to be Modified IE (Only Default Bearer Handled) 
    * Send this IE of SGW is relocated during HO
    */
   if (ROK == ret && \
      (QO_SGW_CHK_MASK(pPdnCb->ueCb->bitmask, QO_SGW_S1_HO_WITH_SGW_RELOC)))
   {
      QO_SGW_DBG_INFO(("Adding Bearer Context to be Modified IE"));

      ret = qoSgwEgtUtlAppendIe(pMbReq, EG_BEARER_CNTX_IETYPE, 0, EG_GROUPED,\
            &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding Bearer Context to be Modified IE"));
      }

      /* Add EBI */
      QO_SGW_DBG_INFO(("Adding EBI IE into Bearer Context to be Modified"));
      egIe.t.valUnSgnd8 = pPdnCb->s5s8CTun->ebi;
      ret = qoSgwEgtUtlAddBCtxIe(pMbReq, EG_EBI_IETYPE, 0, EG_UNSIGNED_8,\
            &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding EBI IE into Bearer Context to be Modified"));
      }

      /* Add S5/S8 SGW U F-TEID */
      if (ROK == ret)
      {
         cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
         egIe.ieInst = 1;
         egIe.t.fTeid.teid = defBearer->s5s8UTun->locTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_S5_S8_SGW_GTP_U;
         qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(defBearer->s5s8UTun->locAddr->u.ipv4TptAddr.address));

         QO_SGW_DBG_INFO(("Adding SGW S5/S8 U-FTEID into Bearer Context "\
               "to be Modified IE"));
         ret = qoSgwEgtUtlAddBCtxIe(pMbReq, EG_FTEID_IETYPE, 1,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding SGW S5/S8 U-FTEID IE into Bearer Context "\
                  "to be Modified IE"));
         }
      }
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(&pMbReq);
   }

   QO_SGW_RETVALUE(ret);
} /* qoSgwBldMBReq */

/***************************************************************
 *
 *       Fun:   qoSgwBldDSReq
 *
 *       Desc:  Builds Delete Session Request
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldDSReq
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg     /* MBRsp out pointer */
)
#else
PUBLIC S16 qoSgwBldDSReq(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (OUT) */
EgMsg          **egMsg;    /* MBRsp out pointer */
#endif
{
   QoSgwTunnelCb     *s5CTun = NULLP;
   EgIe              egIe;
   Mem               memInfo;

   QO_SGW_TRC2(qoSgwBldDSReq);

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;
   s5CTun = pdnCb->s5s8CTun;

   if (qoUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Memory Allocation for GTP Message"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), s5CTun->remTeid,
                     EGT_GTPC_MSG_DS_REQ, s5CTun->transId.val); 

   /* LBI */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   egIe.t.valUnSgnd8 = s5CTun->ebi;
   qoSgwEgtUtlAppendIe(*egMsg, EG_EBI_IETYPE, 0, EG_UNSIGNED_8, &egIe);

   QO_SGW_RETVALUE(ROK);
} /* End of function qoSgwBldDSReq */


/**************************************************************************//**
 *
 * @brief 
 *    Copies IE from source GTP message to destination GTP message
 *             
 * @param[inout]  pDstMsg
 *    Pointer to GTP message where the IE shall be copied (destination)
 * @param[in]  pCsReq
 *    Pointer to GTP message where the IE shall be copied from (source)
 * @param[in]  ieType
 *    Type of the IE to be copied
 * @param[in]  ieInst
 *    Instance onf GTP IE to be copied
 * @param[in]  ieDataType
 *    Type of IE Data
 * 
 * @return
 *    SUCCESS if IE is copied successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 qoSgwEgtUtlCopyIe
(
EgMsg          *pDstMsg,
EgMsg          *pSrcMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType
)
{
   S16            ret = ROK;
   EgIe           *pEgIe = NULLP;
   EgIe           *pDstIe = NULLP;

   QO_SGW_TRC2(qoSgwEgtUtlCopyIe);

   ret = qoSgwEgtUtlGetIE(pSrcMsg, &pEgIe, EG_CAUSE_IETYPE, 0);
   if (ROK == ret)
   {
      ret = egUtilDupIe(&pDstMsg->memCp, pEgIe, &pDstIe);
      if (ROK == ret)
      {
         ret = qoSgwEgtUtlAppendIe(pDstMsg, ieType, ieInst, ieDataType, pDstIe);
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwBldMBRsp
 *
 *       Desc:  Builds Modify Bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldMBRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldMBRsp(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (OUT) */
EgMsg          **egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   EgIe              egIe;
   U16               ret = ROK;
   EgIeLevelInfo     egLvlInfo = {0};
   Mem               memInfo;
   QoSgwTunnelCb     *s11Tun = NULLP;
   QoSgwUeCb         *ueCb = NULLP;
   QoSgwBearerCb     *pDefBearer = NULLP;
   Bool              isBCntxtRecd = NULLP;

   QO_SGW_TRC2(qoSgwBldMBRsp)

   QO_SGW_DBG_INFO(("Building Modify Bearer Response message"));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   ueCb = pdnCb->ueCb;
   s11Tun = ueCb->s11Tun;

   if (qoUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Memory Allocation for GTP Message"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), s11Tun->remTeid,  
                     EGT_GTPC_MSG_MB_RSP, s11Tun->transId.val); 

   /* Add cause */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   qoUtlEgtAddCause(*egMsg, &egLvlInfo, EGT_GTPC_CAUSE_ACC_REQ, 0);

   QO_SGW_GET_BEARER(pdnCb->ueCb->bearerTable, pdnCb->s5s8CTun->ebi,\
         pDefBearer);

   /* Check is bearer contex details received in modify bearer request */
   if (NULLP != pDefBearer)
   {
      isBCntxtRecd = (QO_SGW_CHK_MASK(pDefBearer->op_mask, QO_SGW_CRT_BEARER) \
            || QO_SGW_CHK_MASK(pDefBearer->op_mask, QO_SGW_MOD_BEARER));
   }
   else
   {
      ret = RFAILED;
      QO_SGW_DBG_ERR(("Unable to find Default Bearer Context Cb"));
   }

   /* If bearer context details is received in modify bearer request 
    * send the corresponding detials in the response message */
   if (NULLP != pDefBearer && isBCntxtRecd)
   {
      QO_SGW_DBG_INFO(("Addling Bearer Context Modified IE"));
      ret = qoSgwEgtUtlAppendIe(*egMsg, EG_BEARER_CNTX_IETYPE, 0, EG_GROUPED,\
            &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Addling Bearer Context Modified IE"));
      }

      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding EBI IE into Bearer Context Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.valUnSgnd8 = pDefBearer->ebi;
         ret = qoSgwEgtUtlAddBCtxIe(*egMsg, EG_EBI_IETYPE, 0,\
               EG_UNSIGNED_8, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding EBI IE into Bearer Context Modified IE"));
         }
      }

      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding SGW S1 U-FTEID IE into Bearer Context "\
               "Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.ieInst = 0;
         egIe.t.fTeid.teid = pDefBearer->s1UTun->locTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_S1_U_SGW_GTP_U;
         qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(pdnCb->ueCb->s11Tun->locAddr->u.ipv4TptAddr.address));
         ret = qoSgwEgtUtlAddBCtxIe(*egMsg, EG_FTEID_IETYPE, 0,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding SGW S1 U-FTEID IE into Bearer Context "\
                  "Modified IE"));
         }
      }
   }

   QO_SGW_RETVALUE(ROK);
} /* qoSgwBldMBRsp */

/***************************************************************
 *
 *       Fun:   qoSgwBldMBRspToSGSN
 *
 *       Desc:  Builds Modify Bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldMBRspToSGSN
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldMBRspToSGSN(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (OUT) */
EgMsg          **egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   EgIe              egIe;
   U16               ret = ROK;
   EgIeLevelInfo     egLvlInfo = {0};
   Mem               memInfo;
   QoSgwTunnelCb     *s4Tun = NULLP;
   QoSgwUeCb         *ueCb = NULLP;
   QoSgwBearerCb     *pDefBearer = NULLP;
   Bool              isBCntxtRecd = NULLP;

   QO_SGW_TRC2(qoSgwBldMBRspToSGSN)

   QO_SGW_DBG_INFO(("Building Modify Bearer Response message"));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   ueCb = pdnCb->ueCb;
   s4Tun = ueCb->s4Tun;

   if (qoUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Memory Allocation for GTP Message"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), s4Tun->newRemTeid,  
                     EGT_GTPC_MSG_MB_RSP, s4Tun->seqNum); 

   /* Add cause */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   qoUtlEgtAddCause(*egMsg, &egLvlInfo, EGT_GTPC_CAUSE_ACC_REQ, 0);

   QO_SGW_GET_BEARER(pdnCb->ueCb->bearerTable, pdnCb->s5s8CTun->ebi,\
         pDefBearer);

   /* Check is bearer contex details received in modify bearer request */
   if (NULLP != pDefBearer)
   {
      isBCntxtRecd = (QO_SGW_CHK_MASK(pDefBearer->op_mask, QO_SGW_CRT_BEARER) \
            || QO_SGW_CHK_MASK(pDefBearer->op_mask, QO_SGW_MOD_BEARER));
   }
   else
   {
      ret = RFAILED;
      QO_SGW_DBG_ERR(("Unable to find Default Bearer Context Cb"));
   }

   /* If bearer context details is received in modify bearer request 
    * send the corresponding detials in the response message */
   if (NULLP != pDefBearer && isBCntxtRecd)
   {
      QO_SGW_DBG_INFO(("Adding Bearer Context Modified IE"));
      ret = qoSgwEgtUtlAppendIe(*egMsg, EG_BEARER_CNTX_IETYPE, 0, EG_GROUPED,\
            &egIe);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding Bearer Context Modified IE"));
      }

      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding EBI IE into Bearer Context Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.valUnSgnd8 = pDefBearer->ebi;
         ret = qoSgwEgtUtlAddBCtxIe(*egMsg, EG_EBI_IETYPE, 0,\
               EG_UNSIGNED_8, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding EBI IE into Bearer Context Modified IE"));
         }
      }

      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding Cause IE into Bearer Context Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.cause.causeVal = EGT_GTPC_CAUSE_ACC_REQ;
         egIe.t.cause.ieLength = 0;
         ret = qoSgwEgtUtlAddBCtxIe(*egMsg, EG_CAUSE_IETYPE, 0,\
               EG_CAUSE, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding Cause IE into Bearer Context Modified IE"));
         }
      }

      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Adding SGW S4 U-FTEID IE into Bearer Context "\
               "Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.ieInst = 0;
         egIe.t.fTeid.teid = pDefBearer->s4UTun->locTeid;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_S4_SGW_GTP_U;
         qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(pdnCb->ueCb->s4Tun->locAddr->u.ipv4TptAddr.address));
         ret = qoSgwEgtUtlAddBCtxIe(*egMsg, EG_FTEID_IETYPE, 2,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            QO_SGW_DBG_ERR(("Adding SGW S4 U-FTEID IE into Bearer Context "\
                  "Modified IE"));
         }
      }
   }

   QO_SGW_RETVALUE(ROK);
} /* qoSgwBldMBRspToSGSN */

/***************************************************************
 *
 *       Fun:   qoSgwValDSReq
 *
 *       Desc:  Validates Delete Session Request IEs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValDSReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause      /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValDSReq(pdnCb, egMsg, cause)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block (OUT) */
EgMsg             *egMsg;     /* GTP-C message (IN) */
U32               *cause;     /* cause of failure (OUT) */
#endif
{
   S16      ret    = ROK;
   EgIe     *pEgIe = NULLP;

   QO_SGW_TRC2(qoSgwValDSReq)

   QO_SGW_DBG_INFO(("Retrieving Indication IE"));
   ret = qoSgwEgtUtlGetIE(egMsg, &pEgIe, EG_INDICATION_IETYPE, 0);
   if (ROK == ret && NULLP != pEgIe)
   {
      if (pEgIe->t.indication.isOIPres == TRUE)
      {
         QO_SGW_DBG_INFO(("Operation Indication is Enabled"));
         pdnCb->fwdDSReq = TRUE;
      }
      else
      {
         QO_SGW_DBG_INFO(("Operation Indication is Disabled"));
      }
   }
   else
   {
      QO_SGW_DBG_ERR(("Retrieving Indication IE"));
      ret = RFAILED;
   }

   RETVALUE(ROK);
}


/***************************************************************
 *
 *       Fun:   qoSgwValCBReq
 *
 *       Desc:  Validates and Stores Craete Bearer Request IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValCBReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause      /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValCBReq(pdnCb, egMsg, cause)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block (OUT) */
EgMsg             *egMsg;     /* GTP-C message (IN) */
U32               *cause;     /* cause of failure (OUT) */
#endif
{
   U32                  bCnt = 0;
   S16                  ret = ROK;
   EgIe                 *egIe = NULLP;
   QoSgwTunnelCb        *s5s8UTun = NULLP;
   QoSgwBearerNode      *tmpBearer = NULLP;
   S16                  ieOcr = 0;
   QoSgwPeerCb          *peer = NULLP;
   CmTptAddr            pgwAddr = {0};
   QoSgwPdnTmpData      *tmpPdnData = NULLP;

   QO_SGW_TRC2(qoSgwValCBReq);

   tmpPdnData = pdnCb->tmpPdnData;

   /* Get the number of Bearer Contexts */
   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
   if (ROK != ret || ieOcr <= 0)
   {
      QO_SGW_DBG_ERR(("Bearer Contexts Retrieval"));
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
      QO_SGW_RETVALUE(RFAILED);
   }

   QO_SGW_DBG_INFO(("Create Bearer Request Has (%d) Bearer Contexts", ieOcr));
   if ((pdnCb->ueCb->bearerCnt + ieOcr) > QO_SGW_MAX_BEARERS)
   {
      QO_SGW_DBG_INFO(("Exceeded Maximum Number Of Bearers For UE"));
      *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
      QO_SGW_RETVALUE(RFAILED);
   }

   for (bCnt = 0; bCnt < ieOcr; bCnt++)
   {
      ret = qoSgwCreateUTunnel(pdnCb, &s5s8UTun, QO_IF_S5_S8_DED);
      if (ROK == ret)
      {
         QO_SGW_DBG_INFO(("Creation Of S5/S8 Dedicated Tunnel CB, SUCCESS"));

         /* Receive the PGW FTEID and create the dedTunnel tunnel for the
          * S5/S8*/
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1, EG_FTEID_IETYPE, 1,
                                    &egIe);
         if (ROK == ret)
         {
            QO_SGW_DBG_INFO(("PGW S5/S8 U F-TEID Storing, SUCCESS"));

            /* Common to both default and dedicated bearers */
            pgwAddr.type = CM_TPTADDR_IPV4;
            qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                              &(pgwAddr.u.ipv4TptAddr.address));
            pgwAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.\
                                          u.ipv4TptAddr.port;
            ret = qoSgwFindNCreatePeer(&pgwAddr, &peer);
 
            s5s8UTun->remAddr = &(peer->addr);
            s5s8UTun->remTeid = egIe->t.fTeid.teid;

            QO_SGW_ALLOC((&tmpBearer), sizeof(QoSgwBearerNode))
            if (tmpBearer == NULLP)
            {
               ret = RFAILED;
               *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
               QO_SGW_DBG_ERR(("Retrieval Of PGW S5/S8 U F-TEID, FAILED"));
               break;
            }
            else
            {
               tmpBearer->pdnCb = pdnCb;
               tmpBearer->s5s8UTun = s5s8UTun;
               tmpBearer->ebi = 0;

               cmLListAdd2Tail(&(tmpPdnData->bearers), &(tmpBearer->lnk));
            }
         }
         else
         {
            *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
            QO_SGW_DBG_ERR(("Retrieval Of PGW S5/S8 U F-TEID, FAILED"));
            break;
         }
      }
      else
      {
         *cause = EGT_GTPC_CAUSE_SYS_FAIL;
         QO_SGW_DBG_ERR(("Creation Of S5/S8 Dedicated Tunnel CB, FAILED"));
         break;
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwValUBReq
 *
 *       Desc:  Validates and Stores Craete Bearer Request IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValUBReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause      /* Cause of validation Failure (OUT) */
)
#else
PUBLIC S16 qoSgwValUBReq(pdnCb, egMsg, cause)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block (OUT) */
EgMsg             *egMsg;     /* GTP-C message (IN) */
U32               *cause;     /* cause of failure (OUT) */
#endif
{
   QoSgwPdnTmpData   *tmpPdnData = NULLP;
   U32               i = 0;
   S16               bCnt = 0;
   S16               ret = ROK;
   EgIe              *egIe = NULLP;
   U32               ebi = 0;
   QoSgwBearerNode   *tmpBearer = NULLP;
   QoSgwBearerCb     *bearer = NULLP;

   QO_SGW_TRC2(qoSgwValUBReq);
   tmpPdnData = pdnCb->tmpPdnData;
   cmLListInit(&(tmpPdnData->bearers));

   /* Bearer Context list */
   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &bCnt);
   if (ret != ROK)
   {
      ret = RFAILED;
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
      QO_SGW_DBG_ERR(("No Bearer Contexts IEs Present"));
   }
   else
   {
      for (i = 0; i < bCnt; i++)
      {
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, i + 1, EG_EBI_IETYPE, 0,
                                    &egIe);
         if (ROK == ret && egIe != NULLP)
         {
            ebi = egIe->t.valUnSgnd8;
            if (QO_SGW_VAL_EBI(ebi))
            {
               QO_SGW_GET_BEARER(pdnCb->ueCb->bearerTable, ebi, bearer)
            }
            else
            {
               *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
               ret = RFAILED;
               QO_SGW_DBG_ERR(("Invalid EBI value received EBI (%ld)", ebi));
               break;
            }
         }
         else
         {
            QO_SGW_DBG_ERR(("EBI IE could not be retrieved"));
         }

         if (bearer != NULLP && bearer->pdnCb == pdnCb)
         {
            tmpBearer = NULLP;
            QO_SGW_ALLOC((&tmpBearer), sizeof(QoSgwBearerNode))
            if (tmpBearer == NULLP)
            {
               ret = RFAILED;
               *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
               QO_SGW_DBG_ERR(("Allocation of QoSgwBearerNode"));
               break;
            }
            else
            {
               tmpBearer->ebi = ebi;
               tmpBearer->pdnCb = pdnCb;
               cmLListAdd2Tail(&(tmpPdnData->bearers), &(tmpBearer->lnk));
            }
         }
      }
   }

   if (tmpPdnData->bearers.count == 0)
   {
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
      ret = RFAILED;
      QO_SGW_DBG_ERR(("No Valid Bearer Context IEs present"));
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwValDBReq
 *
 *       Desc:  Validates and Stores Craete Bearer Request IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValDBReq
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause      /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValDBReq(pdnCb, egMsg, cause)
QoSgwPdnCb        *pdnCb;     /* PDN Control Block (OUT) */
EgMsg             *egMsg;     /* GTP-C message (IN) */
U32               *cause;     /* cause of failure (OUT) */
#endif
{
   U32               bIndex = 0;
   U16               iePos = 0;
   EgIeLevelInfo     ieLvlInfo = {0};
   QoSgwPdnTmpData   *tmpPdnData = NULLP;
   U32               i = 0;
   S16               ebiCnt = 0;
   S16               ret = ROK;
   EgIe              *egIe = NULLP;
   U32               ebi = 0;
   QoSgwBearerNode   *tmpBearer = NULLP;
   QoSgwBearerCb     *bearer = NULLP;

   QO_SGW_TRC2(qoSgwValDBReq);
   tmpPdnData = pdnCb->tmpPdnData;
   cmLListInit(&(tmpPdnData->bearers));

   /* LBI */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_EBI_IETYPE, 0);
   if (ret == ROK && egIe != NULLP)
   {
      ebi = egIe->t.valUnSgnd8;
      if (QO_SGW_VAL_EBI(ebi) == FALSE)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
         QO_SGW_DBG_ERR(("Invalid EBI value received EBI (%ld)", ebi));
         QO_SGW_RETVALUE(ret);
      }

      bearer = pdnCb->ueCb->bearerTable[QO_SGW_BEARER_IND(ebi)];
      if (bearer != NULLP && bearer->pdnCb == pdnCb && 
         bearer->isDefBearer == TRUE)
      {
         QO_SGW_ALLOC((&tmpBearer), sizeof(QoSgwBearerNode))
         if (tmpBearer == NULLP)
         {
            ret = RFAILED;
            *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
            QO_SGW_DBG_ERR(("Allocation of QoSgwBearerNode"));
         }
         else
         {
            QO_SGW_DBG_INFO(("Delfault Bearer ID received in Delete"
                           "Bearer Request"));

            tmpBearer->ebi = ebi;
            tmpBearer->pdnCb = pdnCb;
            cmLListAdd2Tail(&(tmpPdnData->bearers), &(tmpBearer->lnk));
         }
      }
   }

   /* if default bearer is not present, then parse for EBI list */
   if (NULLP == bearer)
   {
      ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_EBI_IETYPE, 1, &ebiCnt);
      if (ret != ROK)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
         QO_SGW_DBG_ERR(("No EBI List Present"));
      }
      else
      {
         QO_SGW_DBG_INFO(("Dedicated Bearer IDs received in Delete"
                        "Bearer Request"));

         for (i = 0; i < ebiCnt; i++)
         {
            QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_EBI_IETYPE, 1);
            ieLvlInfo.ieInfo[0].occr = i + 1;
            ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);

            if (ROK == ret && egIe != NULLP)
            {
               ebi = egIe->t.valUnSgnd8;
               if (QO_SGW_VAL_EBI(ebi))
               {
                  bIndex = QO_SGW_BEARER_IND(ebi);
                  bearer = pdnCb->ueCb->bearerTable[bIndex];
               }
               else
               {
                  *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
                  ret = RFAILED;
                  QO_SGW_DBG_ERR(("Invalid EBI value received EBI (%ld)", ebi));
                  break;
               }
            }
            else
            {
               QO_SGW_DBG_ERR(("EBI IE could not be retrieved"));
            }

            if (bearer != NULLP && bearer->isDefBearer == FALSE &&
               bearer->pdnCb == pdnCb)
            {
               tmpBearer = NULLP;
               QO_SGW_ALLOC((&tmpBearer), sizeof(QoSgwBearerNode))
               if (tmpBearer == NULLP)
               {
                  ret = RFAILED;
                  *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
                  QO_SGW_DBG_ERR(("Allocation of QoSgwBearerNode"));
                  break;
               }
               else
               {
                  tmpBearer->ebi = ebi;
                  tmpBearer->pdnCb = pdnCb;
                  cmLListAdd2Tail(&(tmpPdnData->bearers), &(tmpBearer->lnk));
               }
            }
         }
      }
   }

   if (tmpPdnData->bearers.count == 0)
   {
      *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_MISSING;
      ret = RFAILED;
      QO_SGW_DBG_ERR(("EBI of LBI Detaisl not present"));
   }

   QO_SGW_RETVALUE(ret);
}



/***************************************************************
 *
 *       Fun:   qoSgwBldCBReq
 *
 *       Desc:  Builds Create bearer Request
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldCBReq
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldCBReq(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   S16               ret = ROK;
   EgIeLevelInfo     ieLvlInfo = {0};
   QoSgwTunnelCb     *s1UTun = NULLP;
   CmLListCp         *bearerLst = NULLP;
   CmLList           *tmp = NULLP;
   U32               bCnt = 0;
   EgIe              egIe;

   QO_SGW_TRC2(qoSgwBldCBReq);
   QO_SGW_DBG_INFO(("Building Create Bearer Request"));

   /* Add SGW S-1U F-TEID into Bearer Context IE */
   bearerLst = &(pdnCb->tmpPdnData->bearers);
   CM_LLIST_FIRST_NODE(bearerLst, tmp);
   while (tmp)
   {
      s1UTun = ((QoSgwBearerNode *)tmp)->s1UTun;

      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
      ieLvlInfo.level = 1;
      ieLvlInfo.ieInfo[0].occrPres = TRUE;
      ieLvlInfo.ieInfo[0].occr = bCnt + 1;

      ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[1].ieType = EG_FTEID_IETYPE;
      ieLvlInfo.ieInfo[1].ieInst = 0;

      /* check if tunnel is already existing */
      egIe.ieInst = 0;
      egIe.t.fTeid.teid = s1UTun->locTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S1_U_SGW_GTP_U;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                        &(s1UTun->locAddr->u.ipv4TptAddr.address));

      ret = egUtlEgtAddFteid(egMsg, &ieLvlInfo, egIe.t.fTeid.teid,
                           egIe.t.fTeid.ip4Addr,
                           egIe.t.fTeid.intfType, egIe.ieInst);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding of SGW S1-U FTEID, FAILED"));
         break;
      }
    
      CM_LLIST_NEXT_NODE(bearerLst, tmp); 
   } /* while (bearer) */

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwBldDBReq
 *
 *       Desc:  Builds Delete bearer Request
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldDBReq
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldDBReq(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwBldDBReq);
   QO_SGW_DBG_INFO(("Building Delete Bearer Request"));

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwBldUBReq
 *
 *       Desc:  Builds Update bearer Request
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldUBReq
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldUBReq(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwBldUBReq);
   QO_SGW_DBG_INFO(("Building Update Bearer Request"));

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwValDSRsp
 *
 *       Desc:  Validates and Stores Create Bearer Response IEs into a
 *              a temperory data store.
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValDSRsp
(
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause     /* cause of failure (OUT) */
)
#else
PUBLIC S16 qoSgwValDSRsp(pdnCb, egMsg, cause)
QoSgwPdnCb      *pdnCb;    /* PDN Control Block (OUT) */
EgMsg           *egMsg;    /* GTP-C message (IN) */
U32             *cause;    /* cause of failure (OUT) */
#endif
{
   S16            ieOcr = 0;
   S16            ret = ROK;
   EgIe           *egIe = NULLP;
   EgIeLevelInfo  ieLvlInfo;
   U16            iePos = 0;
   U32            bCnt = 0;
   QoSgwBearerCb  **bearerTable = NULLP;
   U32            bearerInd = 0;
   QoSgwTunnelCb  s11Tun;

   QO_SGW_TRC2(qoSgwValDSRsp)

   /* cause */
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_CAUSE_IETYPE, 0);
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Retrieval of Cause IE, FAILED"));
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
   }
   else
   {
      if (egIe->t.valUnSgnd8 == EGT_GTPC_CAUSE_ACC_REQ)
      {
         *cause = egIe->t.valUnSgnd8;
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   /* Bearer Contexts */
   cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
   
   /* Get the number of Bearer ID IEs */
   ret = EgUtilGmGetIeOccrCnt(egMsg, &ieLvlInfo, &ieOcr);
   if (ROK != ret)
   {
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
      QO_SGW_RETVALUE(RFAILED);
   }

   /* EBI IE & eNB S1-U FTEID */
   bearerTable = pdnCb->ueCb->bearerTable;
   for (bCnt = 0; bCnt < ieOcr; bCnt++)
   {
      if ((bearerTable[bCnt] != NULLP) && 
          (bearerTable[bCnt]->s1UTun->state == QO_SGW_TUN_PROGRESS))
      {
         /* ebi */
         egIe = NULLP;
         cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
         QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 1, EG_BEARER_CNTX_IETYPE, 0);
         ieLvlInfo.ieInfo[0].occrPres = TRUE;
         ieLvlInfo.ieInfo[0].occr = bCnt + 1;
   
         /* EPS Bearer Id */
         ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[1].ieType = EG_EBI_IETYPE;
         ieLvlInfo.ieInfo[1].ieInst = 0;
   
         ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
         if (ret != ROK)
         {
            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            QO_SGW_DBG_ERR(("Error: EBI IE could not be retrieved"));
            break;
         }
         else
         {
            bearerInd = QO_SGW_BEARER_IND(egIe->t.valUnSgnd8);
            bearerTable[bearerInd]->ebi = egIe->t.valUnSgnd8;
         }

         /* eNB S1-U F-TEID */
         egIe = NULLP;
         ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[1].ieType = EG_FTEID_IETYPE;
         ieLvlInfo.ieInfo[1].ieInst = 0;

         ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
         if (ret != ROK)
         {
            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            QO_SGW_DBG_ERR(("eNB S1-U F-TEID retreival, FAILED"));
            break;
         }
         else
         {
            s11Tun.remAddr->type = CM_IPV4ADDR_TYPE;
            s11Tun.remTeid = egIe->t.fTeid.teid;
            qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr, &(s11Tun.remAddr->u.\
                              ipv4TptAddr.address));
         }
      }
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwRemCreateBearerReqIes
 *
 *       Desc:  Removes IEs from Create bearer Request which are not
 *              applicable on s11 intf
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwRemCreateBearerReqIes
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          *egMsg      /* Create Bearer Request received from PGW */
)
#else
PUBLIC S16 qoSgwRemCreateBearerReqIes(egMsg, s5s8Tun)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (OUT) */
EgMsg          *egMsg;     /* Create Bearer Request received from PGW */
#endif
{
   EgIeLevelInfo        ieLvlInfo = {0};

   QO_SGW_TRC2(qoSgwRemCreateBearerReqIes)

   /* PGW F-TEID */
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_FTEID_IETYPE, 0);
   EgUtilGmDelIe(egMsg, &ieLvlInfo, FALSE);

   RETVALUE(ROK);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwValCBRsp
 *
 *       Desc:  Validates Create Bearer Response IEs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValCBRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (IN) */
EgMsg          *egMsg,     /* Create Bearer Response received from MME */
U32            *cause,     /* Cause of validation Failure (OUT) */
Bool           *fwdRsp     /* forward the response failure message is recd */
)
#else
PUBLIC S16 qoSgwValCBRsp(pdnCb, egMsg, cause, fwdRsp)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (IN) */
EgMsg          *egMsg;     /* Create Bearer Response received from MME */
U32            *cause;     /* Cause of validation Failure (OUT) */
Bool           *fwdRsp;    /* forward the response failure message is recd */
#endif
{
   U16               ret = ROK;
   EgIe              *egIe = NULLP;
   CmLListCp         *bearerLst = NULLP;
   U32               bIndex = 0;
   CmLList           *tmp = NULLP;
   QoSgwTunnelCb     *s1UTun = NULLP;
   QoSgwTunnelCb     *s5UTun = NULLP;
   U32               bCnt = 0;
   S16               ieOcr = 0;
   QoSgwBearerNode   *tmpBearer = NULLP;
   CmTptAddr         eNbAddr = {0};
   QoSgwPeerCb       *peer = NULLP;
   QoSgwBearerCb     *bearerCb = NULLP;

   QO_SGW_TRC2(qoSgwValCBRsp)

   /* cause */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Retrieval of Cause IE, FAILED"));
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
      QO_SGW_RETVALUE(RFAILED);
   }
   else
   {
      if (egIe->t.valUnSgnd8 == EGT_GTPC_CAUSE_ACC_REQ)
      {
         *cause = egIe->t.valUnSgnd8;
      }
      else
      { 
         *fwdRsp = TRUE;
         *cause = egIe->t.valUnSgnd8;
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   /* Bearer Contexts */
   /* Get the number of Bearer Contexts */
   bearerLst = &(pdnCb->tmpPdnData->bearers);

   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
   if (ROK == ret && bearerLst->count == ieOcr)
   {
      /* Add SGW S-1U F-TEID into Bearer Context IE */
      CM_LLIST_FIRST_NODE(bearerLst, tmp);
      while (tmp)
      {
         tmpBearer = (QoSgwBearerNode *)tmp;
         s1UTun = tmpBearer->s1UTun; 
         s5UTun = tmpBearer->s5s8UTun; 

         /* EBI */
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1, EG_EBI_IETYPE, 0,
                                    &egIe);
         if (ROK == ret && egIe != NULLP)
         {
            tmpBearer->ebi = egIe->t.valUnSgnd8;
         }
         else
         {
            QO_SGW_DBG_ERR(("Retrieval of EBI in Bearer Context"));
            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            break;
         }

         /* eNB S1-U F-TEID */
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1, EG_FTEID_IETYPE, 0,
                                    &egIe);
         if (ROK == ret)
         {
            s1UTun->remTeid = egIe->t.fTeid.teid;

            eNbAddr.type = CM_TPTADDR_IPV4;
            qoUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,
                              &(eNbAddr.u.ipv4TptAddr.address));
            eNbAddr.u.ipv4TptAddr.port = qoSgwCb.egUSap->localAddr.u.\
                                          ipv4TptAddr.port;
            ret = qoSgwFindNCreatePeer(&eNbAddr, &peer);
            s1UTun->remAddr = &(peer->addr);
         }
         else
         {
            QO_SGW_DBG_ERR(("Retrieval of S1-U FTEID in Bearer Context"));
            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            break;
         }

         QO_SGW_ALLOC((&bearerCb), sizeof(QoSgwBearerCb));
         if (bearerCb != NULLP)
         {
            bearerCb->ebi = tmpBearer->ebi;
            s1UTun->ebi = tmpBearer->ebi;
            s5UTun->ebi = tmpBearer->ebi;
            bearerCb->s1UTun = s1UTun;
            bearerCb->pdnCb = pdnCb;  
            bearerCb->s5s8UTun = s5UTun;

            bIndex = QO_SGW_BEARER_IND(tmpBearer->ebi);
            pdnCb->ueCb->bearerTable[bIndex] = bearerCb;
            pdnCb->ueCb->bearerCnt += 1;

            QO_SGW_SET_BEARER_MAP(pdnCb->bearerMap, bearerCb->ebi);
         }
         else
         {
            QO_SGW_DBG_ERR(("Memory Allocation for Bearer CB"));
            *cause = EGT_GTPC_CAUSE_NO_RES_AVAIL;
            break;
         }
       
         CM_LLIST_NEXT_NODE(bearerLst, tmp); 
         bCnt++;
      } /* while (bearer) */
   } /* ROK == ret */
   else
   {
      QO_SGW_DBG_ERR(("Bearer Contexts In Create Bearer Respose Invalid"));
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwValUBRsp
 *
 *       Desc:  Validates Update Bearer Response IEs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValUBRsp
(
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (IN) */
EgMsg             *egMsg,     /* Update Bearer Response received from MME */
U32               *cause,    /* Cause of validation Failure (OUT) */
Bool              *fwdRsp    /* forward the response failure message is recd */
)
#else
PUBLIC S16 qoSgwValUBRsp(pdnCb, egMsg, cause, fwdRsp)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (IN) */
EgMsg          *egMsg;     /* Update Bearer Response received from MME */
U32            *cause;     /* Cause of validation Failure (OUT) */
Bool           *fwdRsp;    /* forward the response failure message is recd */
#endif
{
   QoSgwBearerNode   *tmpBearer = NULLP;
   CmLList           *tmp = NULLP;
   S16               ieOcr = 0;
   CmLListCp         *bearerLst = NULLP;
   U8                bCnt = 0;
   U16               ret = ROK;
   EgIe              *egIe = NULLP;

   QO_SGW_TRC2(qoSgwValUBRsp)

   bearerLst = &(pdnCb->tmpPdnData->bearers);

   /* cause */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Retrieval of Cause IE, FAILED"));
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
      QO_SGW_RETVALUE(RFAILED);
   }
   else
   {
      if (egIe->t.valUnSgnd8 == EGT_GTPC_CAUSE_ACC_REQ)
      {
         *cause = egIe->t.valUnSgnd8;
      }
      else
      { 
         *fwdRsp = TRUE;
         *cause = egIe->t.valUnSgnd8;
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   /* if Default bearer is not getting Updated, then Update
    * the individual bearers
    */
   ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 1, &ieOcr);
   if (ROK == ret && bearerLst->count == ieOcr)
   {
      /* Add SGW S-1U F-TEID into Bearer Context IE */
      CM_LLIST_FIRST_NODE(bearerLst, tmp);
      while (tmp)
      {
         tmpBearer = (QoSgwBearerNode *)tmp;

         /* EBI */
         ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1, EG_EBI_IETYPE, 0,
                                    &egIe);
         if (ROK == ret && egIe != NULLP)
         {
            tmpBearer->ebi = egIe->t.valUnSgnd8;
         }
         else
         {
            QO_SGW_DBG_ERR(("Retrieval of EBI in Bearer Context"));
            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            break;
         }

         CM_LLIST_NEXT_NODE(bearerLst, tmp); 
         bCnt++;
      } /* while (bearer) */
   } /* ROK == ret */
   else
   {
      ret = RFAILED;
      QO_SGW_DBG_ERR(("Update Bearere Response mismatch, "
                     "no bearer Context List"));
   }

   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *
 *       Fun:   qoSgwValDBRsp
 *
 *       Desc:  Validates Delete Bearer Response IEs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValDBRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (IN) */
EgMsg          *egMsg,     /* Delete Bearer Response received from MME */
U32            *cause,     /* Cause of validation Failure (OUT) */
Bool           *fwdRsp     /* forward the response failure message is recd */
)
#else
PUBLIC S16 qoSgwValDBRsp(pdnCb, egMsg, cause, fwdRsp)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (IN) */
EgMsg          *egMsg;     /* Delete Bearer Response received from MME */
U32            *cause;     /* Cause of validation Failure (OUT) */
Bool           *fwdRsp;    /* forward the response failure message is recd */
#endif
{
   QoSgwBearerNode   *tmpBearer = NULLP;
   CmLList           *tmp = NULLP;
   QoSgwBearerCb     *bearer = NULLP;
   U32               ebi = 0;
   S16               ieOcr = 0;
   CmLListCp         *bearerLst = NULLP;
   U8                bCnt = 0;
   U16               ret = ROK;
   EgIe              *egIe = NULLP;

   QO_SGW_TRC2(qoSgwValDBRsp)

   bearerLst = &(pdnCb->tmpPdnData->bearers);

   /* cause */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ROK != ret)
   {
      QO_SGW_DBG_ERR(("Retrieval of Cause IE, FAILED"));
      *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
      QO_SGW_RETVALUE(RFAILED);
   }
   else
   {
      if (egIe->t.valUnSgnd8 == EGT_GTPC_CAUSE_ACC_REQ)
      {
         *cause = egIe->t.valUnSgnd8;
      }
      else
      { 
         *fwdRsp = TRUE;
         *cause = egIe->t.valUnSgnd8;
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   /* LBI */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_EBI_IETYPE, 0);
   if (ret == ROK && egIe != NULLP)
   {
      ebi = egIe->t.valUnSgnd8;
      if (QO_SGW_VAL_EBI(ebi) == FALSE)
      {
         ret = RFAILED;
         *cause = EGT_GTPC_CAUSE_REJ_MAND_IE_INCORR;
         QO_SGW_DBG_ERR(("Invalid EBI value received EBI (%ld)", ebi));
         QO_SGW_RETVALUE(ret);
      }

      bearer = pdnCb->ueCb->bearerTable[QO_SGW_BEARER_IND(ebi)];
      if (bearer != NULLP && bearer->pdnCb == pdnCb && 
         bearer->isDefBearer == TRUE)
      {
         QO_SGW_DBG_INFO(("Delfault Bearer ID received in Delete"
                        "Bearer Response"));

         CM_LLIST_FIRST_NODE(bearerLst, tmp);
         tmpBearer = (QoSgwBearerNode *)tmp;
         if (bearerLst->count == 1 && tmpBearer->ebi == ebi)
         {
            pdnCb->delPdn = TRUE;
            ret = ROK;
            QO_SGW_DBG_INFO(("Create bearer Request & Response matches"));
         }
         else
         {
            QO_SGW_DBG_ERR(("Create bearer Request & Response Mismatch"));
         }
      }
   }

   /* if Default bearer is not getting deleted, then delete
    * the individual bearers
    */
   if (pdnCb->delPdn == FALSE)
   {
      ret = qoSgwEgtUtlGetIeCnt(egMsg, EG_BEARER_CNTX_IETYPE, 0, &ieOcr);
      if (ROK == ret && bearerLst->count == ieOcr)
      {
         /* Add SGW S-1U F-TEID into Bearer Context IE */
         CM_LLIST_FIRST_NODE(bearerLst, tmp);
         while (tmp)
         {
            tmpBearer = (QoSgwBearerNode *)tmp;

            /* EBI */
            ret = qoSgwEgtUtlGetIeInBCtx(egMsg, 0, bCnt + 1, EG_EBI_IETYPE, 0,
                                       &egIe);
            if (ROK == ret && egIe != NULLP &&
               tmpBearer->ebi == egIe->t.valUnSgnd8)
            {
               QO_SGW_DBG_INFO(("Matching EBI in Delete Bearer Response"));
            }
            else
            {
               QO_SGW_DBG_ERR(("Retrieval of EBI in Bearer Context"));
               *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
               break;
            }

            CM_LLIST_NEXT_NODE(bearerLst, tmp); 
            bCnt++;
         } /* while (bearer) */
      } /* ROK == ret */
      else
      {
         ret = RFAILED;
         QO_SGW_DBG_ERR(("Delete Bearere Response mismatch, no EBI List"));
      }
   }

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwBldCBRsp
 *
 *       Desc:  Builds Create bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldCBRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldCBReq(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   S16           ret = ROK;
   CmLListCp     *bearerLst = NULLP;
   CmLList       *tmp = NULLP;
   QoSgwTunnelCb *s5s8UTun = NULLP;
   EgIeLevelInfo ieLvlInfo = {0};
   EgIe          egIe;
   U8            bCnt = 0;

   QO_SGW_TRC2(qoSgwBldCBReq)
   /* Add SGW S5-U teid in Bearer Context */

   egUtlBuildEgHeader(&(egMsg->msgHdr), pdnCb->s5s8CTun->remTeid,
                     EGT_GTPC_MSG_CB_RSP, egMsg->msgHdr.seqNumber);

   bearerLst = &(pdnCb->tmpPdnData->bearers);
   CM_LLIST_FIRST_NODE(bearerLst, tmp);
   while (tmp)
   {
      s5s8UTun = ((QoSgwBearerNode *)tmp)->s5s8UTun;

      /* remove s1-u enode F-TEID */
      qoSgwEgtUtlDelIeInBCtx(&egMsg, 0, bCnt + 1, EG_FTEID_IETYPE, 0);

      /* remove s1-u sgw F-TEID */
      qoSgwEgtUtlDelIeInBCtx(&egMsg, 0, bCnt + 1, EG_FTEID_IETYPE, 1);

      QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
      ieLvlInfo.ieInfo[ieLvlInfo.level].occrPres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].occr = bCnt + 1;

      ieLvlInfo.level = 1;
      ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[1].ieType = EG_FTEID_IETYPE;
      ieLvlInfo.ieInfo[1].ieInst = 2;

      /* SGW s5/s8 u TEID */
      egIe.ieInst = 2;
      egIe.t.fTeid.teid = s5s8UTun->locTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S5_S8_SGW_GTP_U;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                         &(s5s8UTun->locAddr->u.ipv4TptAddr.address));

      ret = egUtlEgtAddFteid(egMsg, &ieLvlInfo, egIe.t.fTeid.teid,
                           egIe.t.fTeid.ip4Addr,
                           egIe.t.fTeid.intfType, egIe.ieInst);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding Of SGW S5S8-U FTEID"));
         break;
      }

      /* Adding PGW s5/s8 U TEID */
      ieLvlInfo.ieInfo[1].ieInst = 3;

      egIe.ieInst = 3;
      egIe.t.fTeid.teid = s5s8UTun->remTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S5_S8_PGW_GTP_U;
      qoUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),
                         &(s5s8UTun->remAddr->u.ipv4TptAddr.address));

      ret = egUtlEgtAddFteid(egMsg, &ieLvlInfo, egIe.t.fTeid.teid,
                           egIe.t.fTeid.ip4Addr,
                           egIe.t.fTeid.intfType, egIe.ieInst);
      if (ROK != ret)
      {
         QO_SGW_DBG_ERR(("Adding Of SGW S5S8-U FTEID"));
         break;
      }

      CM_LLIST_NEXT_NODE(bearerLst, tmp);
   } /* while (bearer) */


   QO_SGW_RETVALUE(ret);
}

/***************************************************************
 *
 *       Fun:   qoSgwBldUBRsp
 *
 *       Desc:  Builds Update bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldUBRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldUBRsp(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   S16           ret = ROK;

   QO_SGW_TRC2(qoSgwBldUBRsp)

   egUtlBuildEgHeader(&(egMsg->msgHdr), pdnCb->s5s8CTun->remTeid,
                     EGT_GTPC_MSG_UB_RSP, egMsg->msgHdr.seqNumber);

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *       Fun:   qoSgwBldDBRsp
 *
 *       Desc:  Builds Delete bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldDBRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldDBRsp(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control block */
EgMsg          *egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   S16           ret = ROK;

   QO_SGW_TRC2(qoSgwBldDBRsp)

   egUtlBuildEgHeader(&(egMsg->msgHdr), pdnCb->s5s8CTun->remTeid,
                     EGT_GTPC_MSG_DB_RSP, egMsg->msgHdr.seqNumber);

   QO_SGW_RETVALUE(ret);
}


/***************************************************************
 *
 *
 *       Fun:   qoSgwRemCBRspIes
 *
 *       Desc:  Removes IEs from Create Bearer Response which are not
 *              applicable on s5/s8 intf
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwRemCBRspIes
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          *egMsg      /* Create Bearer Request received from PGW */
)     
#else    
PUBLIC S16 qoSgwRemCBRspIes(egMsg, s5s8Tun)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (OUT) */
EgMsg          *egMsg;     /* Create Bearer Request received from PGW */
#endif   
{  
   QO_SGW_TRC2(qoSgwRemCBRspIes)

   qoSgwEgtUtlDelIeInBCtx(&egMsg, 0, 1, EG_FTEID_IETYPE, 0);

   QO_SGW_RETVALUE(ROK);
}


/*
*
*       Fun:   qoSgwEgtUtlGetIE
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
PUBLIC S16 qoSgwEgtUtlGetIE
(
EgMsg   *egMsg,
EgIe    **egIe,
U8      ieType,
U8      ieInst
)
#else
PUBLIC S16 qoSgwEgtUtlGetIE(egMsg, egIe, ieType, ieInst)
EgMsg  *egMsg;
EgIe   **egIe;
U8     ieType;
U8     ieInst;
#endif /* ANSI */
{  
   EgIeLevelInfo ieLevelInfo;
   U16           iePos = 0;
   S16           ret = ROK;
   
   QO_SGW_TRC2(qoSgwEgtUtlGetIE)

   *egIe = NULLP;
   cmMemset((U8 *)&ieLevelInfo, 0, sizeof(EgIeLevelInfo));

   ieLevelInfo.level = 0;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieTypePres = TRUE;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieType = ieType;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieInst = ieInst;

   /* Retrieve IE */
   ret = EgUtilGmGetIe(egMsg, &ieLevelInfo, egIe, &iePos);

   QO_SGW_RETVALUE(ret);
}

/*
 *
 *    Fun:    qoSgwEgtUtlGetIeInBCtx
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
PUBLIC S16 qoSgwEgtUtlGetIeInBCtx
(
EgMsg          *egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst,        /* Instance of ie in bearer ctx */
EgIe           **egIe          /* ie */
)
#else
PUBLIC S16 qoSgwEgtUtlGetIeInBCtx(egMsg, bearerInst, bearerCnt, ieType,
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

   QO_SGW_TRC2(qoSgwEgtUtlGetIeInBCtx)
   
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
   
   QO_SGW_RETVALUE(ret);
}

/*
 *
 *    Fun:    qoSgwEgtUtlDelIeInBCtx
 *
 *    Desc:   Removes an IE in Bearer Context Ie in gtpc-msg
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgtUtlDelIeInBCtx
(
EgMsg          **egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst        /* Instance of ie in bearer ctx */
)
#else
PUBLIC S16 qoSgwEgtUtlDelIeInBCtx(egMsg, bearerInst, bearerCnt, ieType,
ieInst)
EgMsg          **egMsg;        /* gtp-c msg */
U8             bearerInst;    /* Instance of Bearer Ctx */
U8             bearerCnt;     /* occr count of bearer */
U32            ieType;        /* type of Ie in bearer ctx */
U8             ieInst;        /* Instance of ie in bearer ctx */
#endif
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwEgtUtlGetIeInBCtx)
   
   ieLvlInfo.level = 1;
   ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst = bearerInst;
   ieLvlInfo.ieInfo[0].occrPres = TRUE;
   ieLvlInfo.ieInfo[0].occr = bearerCnt;

   ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[1].ieType = ieType;
   ieLvlInfo.ieInfo[1].ieInst = ieInst;

   ret = EgUtilGmDelIe(*egMsg, &ieLvlInfo, FALSE);
   
   QO_SGW_RETVALUE(ret);
}


/*
 *
 *    Fun:    qoSgwEgtUtlAppendIe
 *
 *    Desc:   Removes an IE in Bearer Context Ie in gtpc-msg
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgtUtlAppendIe
(
EgMsg          *egMsg,        /* gtp-c msg */
U32            ieType,        /* ie type */
U8             ieInst,        /* ie instance value */
EgDataTypes    dataType,      /* IE Data type */
EgIe           *egIe          /* pointer to IE details */
)
#else
PRIVATE S16 qoSgwEgtUtlAppendIe(egMsg, ieType, ieInst, dataType, egIe)
EgMsg          *egMsg;        /* gtp-c msg */
U32            ieType;        /* ie type */
U8             ieInst;        /* ie instance value */
EgDataTypes    dataType;      /* IE Data type */
EgIe           *egIe;         /* pointer to IE details */
#endif
{
   EgIeLevelInfo     ieLvlInfo;
   S16               ret = ROK;

   QO_SGW_TRC2(qoSgwEgtUtlAppendIe)

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, ieType, ieInst);

   egUtlEgtBuildEgIeHeader(egIe, ieType, ieInst, dataType);
   ret = EgUtilGmAppendIe(egMsg, &ieLvlInfo, egIe);

   QO_SGW_RETVALUE(ret);
}



/*
*
*       Fun:   qoSgwUtlEgtAddBearerCxt
*
*       Desc:  This function adds the bearer context IE to required EGTP-C 
*              Message tunnel.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  av.c
*
*/
#ifdef ANSI
PRIVATE S16 qoSgwUtlEgtAddBearerCxt
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
PRIVATE S16 qoSgwUtlEgtAddBearerCxt(egMsg,egLvlInfo,bearerId,cause,chrgId,egIe,ieCnt)
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

   QO_SGW_TRC2(qoSgwUtlEgtAddBearerCxt)

   /* Fill the ie properties */
   cmMemset((U8 *)&locEgIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&locEgIe, EG_BEARER_CNTX_IETYPE, 0, 0, EG_GROUPED);
#else
   egUtlEgtBuildEgIeHeader(&locEgIe, EG_BEARER_CNTX_IETYPE, 0, EG_GROUPED);
#endif /* EG_REL_930 */
   if(EgUtilGmAppendIe(egMsg, egLvlInfo, &locEgIe) != ROK)
   {
      QO_SGW_RETVALUE(RFAILED);
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
         qoSgwUtilEgtAddBearerTft(egMsg, egLvlInfo, egIe[cnt].ieInst,
                                 egIe[cnt].t.valStr.val,
                                 egIe[cnt].t.valStr.length );
      }
      /* av001.101: Added support for QOS IE */
      else if(egIe[cnt].ieType ==  EG_BEARER_QOS_IETYPE)
      {
         /* This to fill the QOS in the Create Beaerer Req*/
         egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_BEARER_QOS_IETYPE;
         egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = egIe[cnt].ieInst;
         egUtlEgtAddBearerQos(egMsg, egLvlInfo, &egIe[cnt].t.bQos, egIe[cnt].ieInst);
      }
   }
   /* Add charging Id IE */
   if (chrgId)
   {
      egLvlInfo->ieInfo[egLvlInfo->level].ieTypePres = TRUE;
      egLvlInfo->ieInfo[egLvlInfo->level].ieType     = EG_CHARGING_ID_IETYPE;
      egLvlInfo->ieInfo[egLvlInfo->level].ieInst     = 0;
   }

   QO_SGW_RETVALUE(ROK);

}/* End of function */



/*
 *    Fun:    qoSgwEgtUtlFwdFailureRsp
 *
 *    Desc:   Forwards gtp failure response received from remote peer
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgtUtlFwdFailureRsp
(
QoSgwTunnelCb  *cTun,   /* ctrl tunnel */
QoNodeType     target,  /* peer */
EgMsg          *egMsg   /* message to be forwarded to peer */
)
#else
PUBLIC S16 qoSgwEgtUtlFwdFailureRsp(cTun, target, egMsg)
QoSgwTunnelCb  *cTun;   /* ctrl tunnel */
QoNodeType     target;  /* peer */
EgMsg          *egMsg;  /* message to be forwarded to peer */
#endif
{
   QoSgwEgtSapCb  *tarSap = NULLP; 
   S16            ret = ROK;
   TknU32         localTeId;

   QO_SGW_TRC2(qoSgwEgtUtlFwdFailureRsp)

   cmMemset((U8 *)&localTeId, 0, sizeof(TknU32));

   egMsg->msgHdr.teid = cTun->remTeid;
   localTeId.pres = TRUE;
   localTeId.val = cTun->locTeid;

   if (target == QO_ENT_MME)
   {
      tarSap = qoSgwCb.mmeCSap;
   }
   else
   {
      tarSap = qoSgwCb.pgwCSap;
   }

   EuLiEgtSigRsp(&(tarSap->pst), tarSap->spId, localTeId, egMsg);

   QO_SGW_RETVALUE(ret);
} /* qoSgwEgtUtlFwdFailureRsp */



/*
 *    Fun:    qoSgwEgtUtlSndFailureRspToMme
 *
 *    Desc:   Sends GTP-C Failure response to MME
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   qo_egt_utl.c
 *
 */
PUBLIC S16 qoSgwEgtUtlSndFailureRspToMme
(
U32            locTeid,
U32            remTeid,
EgtMsgType     msgType, /* Type of Response message (IN) */
U32            transId, /* seq number (IN) */
U32            cause    /* Cause (IN) */
)
{
   TknU8          causeIe;
   TknU8          bearerId;
   S16            ret = ROK;
   TknU32         tknLocTeid = {0};
   EgIeLevelInfo  ieLvlInfo;
   EgMsg          *rspMsg = NULLP;
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   EgIe           egIe;

   QO_SGW_TRC2(qoSgwEgtUtlSndFailureRspToMme);

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;
   if (egUtlAllocEgMsg(&rspMsg, &memInfo) != ROK )
   {
      QO_SGW_DBG_INFO(("Allocating memory for the GTP Message failed"));
      QO_SGW_RETVALUE(RFAILED);
   }

   tknLocTeid.pres = PRSNT_NODEF;
   tknLocTeid.val = locTeid;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&(rspMsg->msgHdr), remTeid, msgType, transId);

   /* Cause IE */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   qoSgwEgtUtlAppendIe(rspMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);

   /* Junk because of egtp layer */
   if (EGT_GTPC_MSG_CS_RSP == msgType)
   {
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres = TRUE;
      egLvlInfo.ieInfo[0].ieType     = EG_BEARER_CNTX_IETYPE;
      egLvlInfo.ieInfo[0].ieInst     = 0;

      bearerId.pres = TRUE;
      bearerId.val  = 0;
      causeIe.pres    = NOTPRSNT;
      causeIe.val     = cause;

      qoSgwUtlEgtAddBearerCxt(rspMsg, &egLvlInfo, &bearerId, &causeIe, 0,
                              NULLP, 0);
   }
   else if (EGT_GTPC_MSG_CB_RSP == msgType)
   {
      /* Recovery IE */
      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.t.valUnSgnd32 = 0;
      qoSgwEgtUtlAppendIe(rspMsg, EG_RECOVERY_IETYPE, 0, EG_UNSIGNED_32, &egIe);
   }

   EuLiEgtSigRsp(&(qoSgwCb.mmeCSap->pst), (qoSgwCb.mmeCSap)->spId,
               tknLocTeid, rspMsg);

   QO_SGW_RETVALUE(ret);
} /* qoSgwEgtUtlSndFailureRspToMme */


/*
 *    Fun:    qoSgwEgtUtlSndFailureRspToSgsn
 *
 *    Desc:   Sends GTP-C Failure response to Sgsn
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   qo_egt_utl.c
 *
 */
PUBLIC S16 qoSgwEgtUtlSndFailureRspToSgsn
(
U32            locTeid,
U32            remTeid,
EgtMsgType     msgType, /* Type of Response message (IN) */
U32            transId, /* seq number (IN) */
U32            cause    /* Cause (IN) */
)
{
   TknU8          causeIe;
   TknU8          bearerId;
   S16            ret = ROK;
   TknU32         tknLocTeid = {0};
   EgIeLevelInfo  ieLvlInfo;
   EgMsg          *rspMsg = NULLP;
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   EgIe           egIe;

   QO_SGW_TRC2(qoSgwEgtUtlSndFailureRspToSgsn);

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;
   if (egUtlAllocEgMsg(&rspMsg, &memInfo) != ROK )
   {
      QO_SGW_DBG_INFO(("Allocating memory for the GTP Message failed"));
      QO_SGW_RETVALUE(RFAILED);
   }

   tknLocTeid.pres = PRSNT_NODEF;
   tknLocTeid.val = locTeid;

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&(rspMsg->msgHdr), remTeid, msgType, transId);

   /* Cause IE */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   qoSgwEgtUtlAppendIe(rspMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);

   /* Junk because of egtp layer */
   if (EGT_GTPC_MSG_CS_RSP == msgType)
   {
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres = TRUE;
      egLvlInfo.ieInfo[0].ieType     = EG_BEARER_CNTX_IETYPE;
      egLvlInfo.ieInfo[0].ieInst     = 0;

      bearerId.pres = TRUE;
      bearerId.val  = 0;
      causeIe.pres    = NOTPRSNT;
      causeIe.val     = cause;

      qoSgwUtlEgtAddBearerCxt(rspMsg, &egLvlInfo, &bearerId, &causeIe, 0,
                              NULLP, 0);
   }

   EuLiEgtSigRsp(&(qoSgwCb.sgsnCSap->pst), (qoSgwCb.sgsnCSap)->spId,
               tknLocTeid, rspMsg);

   QO_SGW_RETVALUE(ret);
} /* qoSgwEgtUtlSndFailureRspToSgsn */



/*
 *    Fun:    qoSgwEgtUtlSndFailureRsp
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
PUBLIC S16 qoSgwEgtUtlSndFailureRsp
(
U32            locTeid, /* PGW C-Teid (IN) */
EgtMsgType     msgType, /* Type of Response message (IN) */
U32            transId, /* seq number (IN) */
U32            cause,   /* Cause (IN) */
QoNodeType     target   /* peer */
)
#else
PUBLIC S16 qoSgwEgtUtlSndFailureRsp(teid, msgType, transId, cause, target)
U32            locTeid; /* PGW C-Teid (IN) */
EgtMsgType     msgType; /* Type of Response message (IN) */
U32            transId; /* seq number (IN) */
U32            cause;   /* Cause (IN) */
QoNodeType     target;  /* peer */
#endif
{
   QoSgwEgtSapCb  *tarSap = NULLP; 
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

   QO_SGW_TRC2(qoSgwEgtUtlSndFailureRsp)

   cmMemset((U8 *)&localTeId, 0, sizeof(TknU32));
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   if (egUtlAllocEgMsg(&rspMsg, &memInfo) != ROK )
   {
      QO_SGW_DBG_INFO(("Allocating memory for the GTP Message failed"));
      QO_SGW_RETVALUE(RFAILED);
   }

   if (target == QO_ENT_MME)
   {
      tarSap = qoSgwCb.mmeCSap;
   }
   else
   {
      tarSap = qoSgwCb.pgwCSap;
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&(rspMsg->msgHdr), remTeid, msgType, transId);

   /* Cause IE */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   qoSgwEgtUtlAppendIe(rspMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);

   /* Junk because of egtp layer */
   if (EGT_GTPC_MSG_CS_RSP == msgType)
   {
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres = TRUE;
      egLvlInfo.ieInfo[0].ieType     = EG_BEARER_CNTX_IETYPE;
      egLvlInfo.ieInfo[0].ieInst     = 0;

      bearerId.pres = TRUE;
      bearerId.val  = 0;
      causeIe.pres    = NOTPRSNT;
      causeIe.val     = cause;

      qoSgwUtlEgtAddBearerCxt(rspMsg, &egLvlInfo, &bearerId, &causeIe, 0,
                              NULLP, 0);
   }
   else if (EGT_GTPC_MSG_CB_RSP == msgType)
   {
      /* Recovery IE */
      cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
      egIe.t.valUnSgnd32 = 0;
      qoSgwEgtUtlAppendIe(rspMsg, EG_RECOVERY_IETYPE, 0, EG_UNSIGNED_32, &egIe);
   }

   localTeId.pres = TRUE;
   localTeId.val = locTeid;

   EuLiEgtSigRsp(&(tarSap->pst), tarSap->spId, localTeId, rspMsg);

   QO_SGW_RETVALUE(ret);
} /* qoSgwEgtUtlSndFailureRsp */


/*
 *    Fun:    qoSgwEgtUtlSndSuccessRsp
 *
 *    Desc:   Sends GTP-C Success Response to MME or PGW
 *            with only cause value with SUCCESS
 *
 *    Ret:    
 *
 *    Notes:  
 *
 *    File:   av_egt_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 qoSgwEgtUtlSndSuccessRsp
(
QoSgwTunnelCb  *cTun,   /* Tunnel CB */
U32            transId, /* seq number (IN) */
EgtMsgType     msgType, /* Type of Response message (IN) */
QoNodeType     target   /* peer */
)
#else
PUBLIC S16 qoSgwEgtUtlSndSuccessRsp(teid, msgType, transId, target)
QoSgwTunnelCb  *cTun;   /* Tunnel CB */
U32            transId; /* seq number (IN) */
EgtMsgType     msgType; /* Type of Response message (IN) */
QoNodeType     target;  /* peer */
#endif
{
   QoSgwEgtSapCb  *tarSap = NULLP; 
   TknU8          causeIe;
   TknU8          bearerId;
   S16            ret = ROK;
   TknU32         localTeId;
   EgIeLevelInfo  ieLvlInfo;
   EgMsg          *rspMsg = NULLP;
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   EgIe           *egIe = NULLP;

   QO_SGW_TRC2(qoSgwEgtUtlSndSuccessRsp)

   cmMemset((U8 *)&localTeId, 0, sizeof(TknU32));
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   if (egUtlAllocEgMsg(&rspMsg, &memInfo) != ROK )
   {
      QO_SGW_DBG_INFO(("Allocating memory for the GTP Message failed"));
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&(rspMsg->msgHdr), cTun->remTeid, msgType, transId);

   /* Cause IE */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[0].ieTypePres = TRUE;
   egLvlInfo.ieInfo[0].ieType = EG_CAUSE_IETYPE;
   egLvlInfo.ieInfo[0].ieInst = 0;
   egUtlEgtAddCause(rspMsg, &egLvlInfo, EGT_GTPC_CAUSE_ACC_REQ, 0);

   /* Junk because of egtp layer */
   if (EGT_GTPC_MSG_CS_RSP == msgType)
   {
      egLvlInfo.level = 0;
      egLvlInfo.ieInfo[0].ieTypePres = TRUE;
      egLvlInfo.ieInfo[0].ieType     = EG_BEARER_CNTX_IETYPE;
      egLvlInfo.ieInfo[0].ieInst     = 0;

      bearerId.pres = TRUE;
      bearerId.val  = 0;
      causeIe.pres    = NOTPRSNT;
      causeIe.val     = EGT_GTPC_CAUSE_ACC_REQ;

      qoSgwUtlEgtAddBearerCxt(rspMsg, &egLvlInfo, &bearerId, &causeIe, 0,
                              egIe, 0);
   }

   localTeId.pres = TRUE;
   localTeId.val = cTun->locTeid;

   if (target == QO_ENT_MME)
   {
      tarSap = qoSgwCb.mmeCSap;
   }
   else
   {
      tarSap = qoSgwCb.pgwCSap;
   }

   QoSgwLiEgtSigRsp(&(tarSap->pst), tarSap->spId, localTeId, rspMsg);

   QO_SGW_RETVALUE(ret);
} /* qoSgwEgtUtlSndFailureRsp */


/*
 *
 *       Fun:   qoSgwUtilEgtAddBearerTft
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
PRIVATE S16  qoSgwUtilEgtAddBearerTft
(
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         *buf,
U32        len
)
#else
PRIVATE S16 qoSgwUtilEgtAddBearerTft(egMsg,egLvlInfo,inst,buf, len)
EgMsg      *egMsg,
EgIeLevelInfo *egLvlInfo,
U8         inst,
U8         *buf,
U32        len
#endif /* ANSI */
{
   EgIe     egIe;
   S16      ret = ROK;

   QO_SGW_TRC2(qoSgwUtilEgtAddBearerTft)

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe,EG_BEARER_TFT_IETYPE,inst, 0,EG_STRING_MAX);
#else
   egUtlEgtBuildEgIeHeader(&egIe,EG_BEARER_TFT_IETYPE,inst, EG_STRING_MAX);
#endif /* EG_REL_930 */

   egIe.t.valStr16.length = len;
   cmMemcpy( (U8 *)egIe.t.valStr.val, (U8 *)buf ,egIe.t.valStr.length);

   ret = EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe);

   QO_SGW_RETVALUE(ret);
}  /* qoSgwUtilEgtAddBearerTft  */


/*
 *
 *       Fun:   qoSgwEgtUtlGetIeCnt
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
PRIVATE S16  qoSgwEgtUtlGetIeCnt
(
EgMsg          *egMsg,
U32            ieType,
U8             inst,
S16            *bCnt
)
#else
PRIVATE S16 qoSgwEgtUtlGetIeCnt(egMsg, ieType, inst, bCnt)
EgMsg          *egMsg;
U32            ieType;
U8             inst;
S16            *bCnt;
#endif /* ANSI */
{
   EgIeLevelInfo           ieLvlInfo = {0};
   S16                     ret = ROK;

   QO_SGW_TRC2(qoSgwEgtUtlGetIeCnt)

   cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, ieType, inst);
   
   /* Get the number of Bearer ID IEs */
   ret = EgUtilGmGetIeOccrCnt(egMsg, &ieLvlInfo, bCnt);

   QO_SGW_RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Adds an IE inside Bearer Context IE
 *             
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into Bearer Context IE
 * @param[in]  ieInst
 *    Instance of IE to be added into Bearer Context IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into Bearer Context IE
 * @param[in]  pEgIe 
 *    Pointer to ie details data structure
 * 
 * @return
 *    SUCCESS if IE is added into Bearer Context IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 qoSgwEgtUtlAddBCtxIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret       = ROK;

   QO_SGW_TRC2(qoSgwEgtUtlAddBCtxIe);

   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
   QO_SGW_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   egUtlEgtBuildEgIeHeader(pEgIe, ieType, ieInst, ieDataType);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, pEgIe);

   QO_SGW_RETVALUE(ret);
}



/***************************************************************
 *
 *       Fun:   qoSgwValDDNAck
 *
 *       Desc:  Validates and Stores DL Data Notification IEs
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_sgw_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwValDDNAck
(
QoSgwUeCb       *ueCb,     /* UE Control Block (OUT) */
EgMsg           *egMsg     /* GTP-C message (IN) */
)
#else
PUBLIC S16 qoSgwValDDNAck(pdnCb, egMsg)
QoSgwUeCb       *ueCb;     /* UE Control Block (OUT) */
EgMsg           *egMsg;    /* GTP-C message (IN) */
#endif
{
   S16                     ret = ROK;
   EgIe                    *egIe = NULLP;

   QO_SGW_TRC2(qoSgwValCSRsp)

   /* cause */
   ret = qoSgwEgtUtlGetIE(egMsg, &egIe, EG_CAUSE_IETYPE, 0);
   if (ROK != ret || egIe == NULLP)
   {
      ret = RFAILED;
      QO_SGW_DBG_ERR(("Retrieval Of Cause IE"));
   }
   else
   {
      if (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ)
      {
         QO_SGW_RETVALUE(RFAILED);
      }
   }

   QO_SGW_RETVALUE(ROK);
}



/*
 *
 *       Fun:   qoSgwBldDDN
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
PUBLIC S16 qoSgwBldDDN
(
QoSgwTunnelCb    *sgwTunlCb,
EgMsg            **egMsg
)
#else
PUBLIC S16 qoSgwBldDDN(sgwTunlCb, egMsg)
QoSgwTunnelCb    *sgwTunlCb;
EgMsg            **egMsg;
#endif
{
   EgIeLevelInfo        egLvlInfo;
   Mem                  memInfo;
   U32                  transId = 0;

   QO_SGW_TRC2(qoSgwBldDDN)
   QO_SGW_DBG_INFO((QO_SGW_PRNTBUF, "Building Downlink Data Notification\n"));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   if( qoUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      QO_SGW_LOGERR(ERRCLS_DEBUG, "Allocating memory: GTP Message");
      QO_SGW_RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   qoSgwGenerateCTransId(&transId);
   qoUtlBuildEgHeader(&((*egMsg)->msgHdr), sgwTunlCb->remTeid,
                      EGT_GTPC_MSG_DWN_DATA_NOTFN, transId);

   /* Private Extension */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   qoUtlEgtAddPrivateExt(*egMsg, &egLvlInfo, 0, 0);

   QO_SGW_RETVALUE(ROK);
}/* End of function qoSgwBldDDN*/



/**************************************************************************//**
 *
 * @brief
 *    Finds Default Bearer belonging to a PDN session
 *             
 * @param[in]  pPdnCb
 *    Pointer to PDN session for which default bearer has to be found
 *    handled
 * 
 * @return
 *    returns the Bearer CB of the default bearer
 *    If not found returns NULL
 *
 *****************************************************************************/
PUBLIC QoSgwBearerCb* qoSgwFndDefBearer
(
QoSgwPdnCb        *pPdnCb
)
{
   U32                  bCnt = 0;
   QoSgwBearerCb        *pDefBearer = NULLP;
   QoSgwBearerCb        **ppBearerTable = NULLP;

   QO_SGW_TRC2(qoSgwFndDefBearer)

   ppBearerTable = pPdnCb->ueCb->bearerTable;

   for (bCnt = 0; bCnt < QO_SGW_MAX_BEARERS; bCnt++)
   {
      if ((ppBearerTable[bCnt] != NULLP) && \
          (ppBearerTable[bCnt]->isDefBearer) &&
          (ppBearerTable[bCnt]->pdnCb == pPdnCb))
      {
         pDefBearer = ppBearerTable[bCnt];
         break;
      }
   }

   return pDefBearer;
}

/***************************************************************
 *
 *       Fun:   qoSgwBldDSRsp
 *
 *       Desc:  Builds Modify Bearer Response
 *
 *       Ret:   ROK/RFAILED
 *
 *       Notes: None
 *
 *       File:  qo_egt_utl.c
 *
 ***************************************************************/
#ifdef ANSI
PUBLIC S16 qoSgwBldDSRsp
(
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg      /* Modify Bearer Response received from MME */
)
#else
PUBLIC S16 qoSgwBldDSRsp(pdnCb, egMsg)
QoSgwPdnCb     *pdnCb;     /* PDN Control Block (OUT) */
EgMsg          **egMsg;     /* Modify Bearer Response received from MME */
#endif
{
   EgIeLevelInfo     egLvlInfo = {0};
   Mem               memInfo;
   QoSgwTunnelCb     *sTun = NULLP;
   QoSgwUeCb         *ueCb = NULLP;

   QO_SGW_TRC2(qoSgwBldDSRsp)

   QO_SGW_DBG_INFO(("Building Delete Session Response message"));

   memInfo.region = qoSgwCb.init.region;
   memInfo.pool   = qoSgwCb.init.pool;

   ueCb = pdnCb->ueCb;
   if(pdnCb->ueCb->intf == QO_IF_S11_C )
   {
      sTun = ueCb->s11Tun;
   }
   else if(pdnCb->ueCb->intf == QO_IF_S4_C )
   {
      sTun = ueCb->s4Tun;
   }

   if(sTun != NULLP)
   {
      if (qoUtlAllocEgMsg(egMsg, &memInfo) != ROK )
      {
         QO_SGW_LOGERR(ERRCLS_ADD_RES, ("Memory Allocation for GTP Message"));
         QO_SGW_RETVALUE(RFAILED);
      }
      /* Build the Header  with Default values */
      egUtlBuildEgHeader(&((*egMsg)->msgHdr), sTun->remTeid,  
            EGT_GTPC_MSG_DS_RSP, sTun->transId.val); 

      /* Add cause */
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      qoUtlEgtAddCause(*egMsg, &egLvlInfo, EGT_GTPC_CAUSE_ACC_REQ, 0);
      QO_SGW_RETVALUE(ROK);
   }
   QO_SGW_RETVALUE(RFAILED);
} /* qoSgwBldDSRsp */


/**********************************************************************
  
         End of file:     qo_egt_utl.c@@/main/1 - Fri Oct  8 21:22:18 2011
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/

