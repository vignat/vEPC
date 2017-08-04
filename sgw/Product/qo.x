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

     Name:     Trillium LTE SGW Application  

     Type:      C include file

     Desc:     Structures, variables and typedefs required by SGW 

     File:     qo.x

     Sid:      qo.x@@/main/1 - Thu Jul  8 21:22:18 2010

     Prg:      mm

*********************************************************************21*/

#ifndef __SGWX__
#define __SGWX__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_mblk.h"       /* Memory Link List */
#include "egt.h"           /* Event structure file */
#include "lqo.h"           /* */

/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* Memory Link List typedefs */
#include "cm_lib.x"        /* common ss7 */
#include "egt.x"           /* EG Event structure file */
#include "lqo.x"           /* */

/* IDLE -> ATTACH -> MODIFY -> CONNECTED -> DETACH */
typedef enum _qoSgwPdnState
{
  QO_SGW_PDN_IDLE = 0,      /**< UE context is created */

  QO_SGW_PDN_CREATE,        /**< UE has initiated ATTACH, create session
                             request is received from MME and forwarded to PGW
                             1 S11-C tunnel in PROGRESS state
                             1 S5S8-C tunnel in PROGRESS state
                             1 S1-U Def tunnel in PROGRESS state
                             1 S5S8-U Def tunnel in PROGRESS state
                             */

  QO_SGW_PDN_MODIFY,        /**< UE received Modify bearer request.
                             This state is only applicable in ATTACH state

                             1 S11-C tunnel in CONNECTED state
                             1 S5S8-C tunnel in CONNECTED state
                             1 S1-U Def tunnel in PROGRESS state
                             1 S5S8-U Def tunnel in CONNECTED state
                             */

  QO_SGW_PDN_CONNECTED,     /**< UE is now connected. Create Session Procedure
                             is done. Now SGW has all tunnels CONNECTED
                             1 S11-C tunnel, 1 S5S8-C tunnel
                             1 S1-U default tunnel, 1 S5S8-U default tunnel

                             IMP: Receiving a Modify Bearer Request in this
                             state will not move the state back to MODIFY
                             */

  QO_SGW_PDN_DETACH,        /**< UE received Delete Session Request */

  QO_SGW_PDN_ERROR          /**< Erroneous state */

} QoSgwPdnState;

typedef enum _qoSgwTunState
{
   QO_SGW_TUN_IDLE = 0,     /**< tunnel is down */
   QO_SGW_TUN_PROGRESS,     /**< tunnel is in the process of creation */
   QO_SGW_TUN_CONNECTED,    /**< tunnel is connected */
   QO_SGW_TUN_PENDING,      /**< tunnel is in a process */ 
   QO_SGW_TUN_DISCONNECTED, /**< tunnel is being disconnected on network */
   QO_SGW_TUN_LCLDEL        /**< tunnel is being local destroyed */
}QoSgwTunState;

/* Used when a PDN or UE is being deleted, to decide whether to do
 * signaling on s5/s8 and s11 interface before deleting the PDN or UE Context
 */
typedef enum _qoSgwSigType
{
    QO_SGW_SIGNAL_NO,       /* No signalling required */
    QO_SGW_SIGNAL_PGW,      /* Send Delete Session Req to PGW */
    QO_SGW_SIGNAL_MME,      /* Send Delete Bearer Req to MME */
    QO_SGW_SIGNAL_MME_PGW,  /* Send DSReq to PGW and DBReq to MME */
    QO_SGW_SIGNAL_SGSN      /* Send Delete Session Req to MME */
} QoSgwSigType;

typedef enum _qoSgwUeState
{
   QO_SGW_UE_IDLE = 0,
   QO_SGW_UE_PROGRESS,
   QO_SGW_UE_CONNECTED,
   QO_SGW_UE_DISCONNECTED
} QoSgwUeState;

typedef enum _qoSgwEgtEvent
{
   QO_SGW_EGT_EVT_INV,
   QO_SGW_EGT_EVT_CS_REQ_SENT,
   QO_SGW_EGT_EVT_MB_REQ_SENT,
   QO_SGW_EGT_EVT_DS_REQ_SENT,
   QO_SGW_EGT_EVT_BR_CMD_SENT,
   QO_SGW_EGT_EVT_DB_CMD_SENT,
   QO_SGW_EGT_EVT_MB_CMD_SENT

} QoSgwEgtEvent;

typedef enum _qoNodeType
{
   QO_ENT_MME,
   QO_ENT_SGW,
   QO_ENT_PGW,
   QO_ENT_ENB,
   QO_ENT_SGSN
}QoNodeType;

typedef enum _qoInterfaceType
{
   QO_IF_INV,
   QO_IF_S11_C,
   QO_IF_S5_S8_C,
   QO_IF_S4_C,
   QO_IF_S5_S8_DEF,     /* S5/S8 Default     */
   QO_IF_S1U_DEF,       /* S1-U Default      */
   QO_IF_S5_S8_DED,     /* S5/S8-U Dedicated */
   QO_IF_S1U_DED,       /* S1-U Dedicated    */
   QO_IF_S4_DEF,        /* S4 Default        */
   QO_IF_S4_DED         /* S4 Dedicated      */
}QoInterfaceType;

typedef enum _qoPlane
{
   QO_USER,
   QO_CONTROL
}QoPlane;

typedef struct _qoSgwUeCb QoSgwUeCb;
typedef struct _QoSgwPdnCb QoSgwPdnCb;

/**
 * Tunnel structure.
 * */
typedef struct _qoSgwTunnelCb
{
   U8                ebi;        /* EPS Bearer ID */
   U32               locTeid;    /* TEID */
   U32               remTeid;    /* TEID */
   TknU32            transId;    /* Transaction ID */
   U8                state;      /* Tunnel State */
   QoPlane           plane;      /* Control tunnel / User Tunnel */
   QoInterfaceType   intf;       /* S11/S5/S8 */

   /* qo003.101 Changes in eGTP Rel9 Sequence no is 24bit wide */
#ifndef EG_REL_930
   U16               seqNum;  /* Sequence number to be used for this tunnel */
#else
   U32               seqNum;  /* Sequence number to be used for this tunnel */
#endif /* EG_REL_930 */

   CmTptAddr         *locAddr;  /* reference into IP Cb in selfaddr->config
                                 * field->global CB*/
   CmTptAddr         *remAddr;  /* reference into IP Cb in peecb->global CB*/
   U32               newRemTeid; /* New remote TEID in MBReq (U-Plane) */
   CmTptAddr         *newRemAddr; /* New remote Address in MBReq (U-Plane) */
   EgMsg             *msg;

   /* qo001.101: Flag to indicate tunnel release state */
   Bool              tRel;          /* Tunnel released so far or not */
   QoSgwUeCb         *ueCb;         /* Pointer to UE control block   */
   QoSgwPdnCb        *pdnCb;        /* NOT applicable for S11Tun */
   QoSgwEgtEvent     currEvent;     /* Current event on the C- Tunnel */
} QoSgwTunnelCb;

typedef struct _qoSgwTunnEnt
{
   CmHashListEnt tunnEnt;    /**< Hashlist manipulator with TEID */
   CmHashListEnt transEnt;    /**< Hashlist manipulator with TransID */
   QoSgwTunnelCb tunnelCb;
}QoSgwTunnEnt;

/* qo001.101: Dedicated GTPU tunnel */
typedef struct _qoSgwGtpUDedTunnel
{
      U8            ebi;    /** Dedicated Bearer ID */
      U32           pti;
      QoSgwTunnelCb *s1UTun;
      QoSgwTunnelCb *s5s8UTun;
}QoSgwGtpUDedTunnel;

/* Temperory data of Bearer Contexts in GTP-C messages during a transaction
 * This data will be udpated to actual Bearer CB if transaction
 * is successfull */
typedef struct _QoSgwBearerNode
{
   CmLList           lnk;        /* Link of Linked List  */
   U32               ebi;        /* Bearer ID            */
   QoSgwTunnelCb     *s1UTun;    /* S1-U tunnel          */
   QoSgwTunnelCb     *s5s8UTun;  /* S5/s8-U tunnel       */
   QoSgwPdnCb        *pdnCb;     /* PDN of the bearer    */
} QoSgwBearerNode;

typedef struct _QoSgwBearerDelNode
{
   CmLList           lnk;        /* Link of Linked List  */
   U32               ebi;        /* ebi */
} QoSgwBearerDelNode;

/* Temperory data of PDN Details in GTP-C messages during a transaction
 * This data will be udpated to actual Bearer CB if transaction
 * is successfull */
typedef struct _QoSgwPdnTmpData
{
   U32            lbi;           /* default bearer id */
   CmLListCp      bearers;       /* Bearers to be added or modified  
                                  * QoSgwTmpBearerNode type          
                                  */
   CmLListCp      bearersDel;    /* Bearers to be deleted 
                                  * QoSgwTmpBearerNode type          
                                  */
} QoSgwPdnTmpData;

typedef struct _qoSgwGtpUMap
{
   U8             ebi;    /** Linked Bearer ID */
   U8             nmbDedTunnels; /* qo001.101: Number of ded tunls present*/
   QoSgwTunnelCb *s1UDefTun;
   QoSgwTunnelCb *s5s8DefTun;
   QoSgwGtpUDedTunnel  dedTunnel[QO_MAX_TUNNEL]; /* qo001.101: changed to array */
}QoSgwGtpUMap;
/*
 * SGW Bearer Table Structure
 * */
typedef struct _qoSgwBearerMap
{
   struct
   {
      QoSgwTunnelCb *s11Tun;
      QoSgwTunnelCb *s5s8Tun;
      QoSgwTunnelCb *s4Tun;
   }gtpc;
   QoSgwGtpUMap gtpu;
}QoSgwBearerMap;

/* PDN Connection details */
struct _QoSgwPdnCb
{
   QoSgwPdnState     state;      /* State of the UE */
   U32               transId;    /* Transaction ID for rsp messages        */
   U32               bearerMap;  /* check the mask for bearer ID           
                                  * e.g if Bearer Id is 5 belongs          
                                  * to this PDN                            
                                  * then 5th bit will be set               
                                  */

   QoSgwTunnelCb     *s5s8CTun;  /* C-Plane tunnel information towards PGW */
   QoSgwUeCb         *ueCb;      /* UE Control block                       */
   U32               pti;        /* Procedure Transaction ID               */
   QoSgwPdnTmpData   *tmpPdnData;  /* temporary data of gtp-c transaction  */
   Bool              delPdn;     /* TRUE pdn has to be deleted */
   Bool              fwdDSReq;   /* set to TRUE if SGW has to forward the 
                                  * the Delete Session Request received
                                  * from MME
                                  */
};

/* Bearer Details */
typedef struct _QoSgwBearerCb
{
   U8                ebi;           /* EBI of the bearer             */
   Bool              isDefBearer;   /* TRUE: if a default bearer     */  
   QoSgwTunnelCb     *s5s8UTun;     /* S5/S8 u-plane tunnel details  */
   QoSgwTunnelCb     *s1UTun;       /* S1-AP u-plane tunnel details  */
   QoSgwTunnelCb     *s4UTun;       /* S4 u-plane tunnel details  */
   QoSgwPdnCb        *pdnCb;        /* pointer to PDN Details and    *
                                     * PGW C-Plane tunnel            */
   U8                op_mask;       /* Bearer operation bitmask      */
} QoSgwBearerCb;

/* qo001.101: EGTP-U Event info structure */
typedef struct qoEgtUEvntInfo
{
   CmLList      lnk;         /* Link of Linked List */
   EgtUEvnt     *egtUEvnt;   /* Egt Event */

}QoEgtUEvntInfo;

typedef struct _QoImsi
{
   U8          len;                    /* number of bytes   */
   U8          val[QO_SGW_IMSI_LEN];   /* imsi value        */
} QoImsi;

typedef enum
{
   QO_SGW_NO_BUFFERING,          /* initail state */
   QO_SGW_START_BUFFERING        /* for staring dl data buffering */
} QoSgwBufState;

struct _qoSgwUeCb
{
   U8             ueId[QO_SGW_IMSI_LEN];          /* IMSI, Hash Key  */
   U8             ueIdLen;       
   QoSgwUeState   state;
   QoSgwBearerCb  *bearerTable[QO_SGW_MAX_BEARERS];   /* Bearers of the UE */

   QoSgwTunnelCb  *s11Tun;       /* S11 C-Plane tunnel */
   QoSgwTunnelCb  *s4Tun;        /* S4 C-Plane tunnel */
   CmLListCp      pendingOnS1;   /* qo001.101: Linked list of EgtUEvntInfo */
   QoSgwBufState  bufferingState;/* qo001.101: flag to indicate buffering  */

   U32            bitmask;       /* GTP-C Procedure information
                                  * eg: Initial Attach, TAU, HO etc
                                  * Presense of IEs etc..
                                  */

   U32            bearerCnt;     /* Number of Bearers in UE                */
   U32            pdnCnt;        /* Number of PDNs this UE is connected to */
   EgUli          uli;           /* E-UTRAN ULI */
   QoInterfaceType   intf;       /* S11/S5/S8 */
   U8             msisdn[QO_SGW_MSISDN_LEN];
   U8             msisdnLen;
};

typedef struct _qoSgwUeEnt
{
   CmHashListEnt   ueEnt;
   QoSgwUeCb       ueCb;
}QoSgwUeEnt;

typedef struct _qoSgwPeerCb
{
   CmTptAddr     addr;
   QoNodeType    nodeType; /* MME/PGW/ENB */
}QoSgwPeerCb;

/** Structure to store peer addresses. */
typedef struct _qoSgwPeerEnt
{
   CmHashListEnt peerEnt;
   QoSgwPeerCb   peerCb;
}QoSgwPeerEnt;

typedef struct _qoSgwGtpU
{
   CmHashListEnt hashEnt;
   U8            ebi; /** Dedicated bearer ID */
   QoSgwTunnelCb *s1UTun;
   QoSgwTunnelCb *s5s8Tun;
}QoSgwGtpU;
/*
 * SGW Lower SAP Control Block.
 * */
typedef struct _qoSgwEgtSapCb                   
{
   Pst           pst;               /** Service user post structure */
   State         state;             /** state of this SAP */
   SuId          suId;              /** service user SAP id */
   SpId          spId;              /** Service Provider Id */
   QoInterfaceType intf;      /* S11/S5/S8/S1-U */
   QoPlane       plane;             /** control Sap / User Sap */
   CmTptAddr     localAddr;
   U32           nmbTunn;           /** Number of Bearers through this SAP */
   CmHashListCp  tunnCbHlCp;        /** Hash list of tunnels handles through
                                     this SAP. key is 
                                     local TEID */
   CmHashListCp  tunnPendCbHlCp;   /** Hash list of tunnels handles through
                                      this SAP which are incomplete. Key:
                                      transID */
}QoSgwEgtSapCb;


/*
 * SGW Control Block.
 * */
typedef struct _qoSgwCb
{
   TskInit         init;             /* Task initialization structure */
   QoGenCfg        cfg;
   QoSgwEgtSapCb  *mmeCSap; 
   QoSgwEgtSapCb  *pgwCSap; 
   QoSgwEgtSapCb  *echoCSap; 
   QoSgwEgtSapCb  *sgsnCSap; 
   QoSgwEgtSapCb  *egUSap; 
   CmHashListCp    ueCbHlCp;        /** Hash List of UEs. Key IMSI */
   CmHashListCp    peerCbHlCp;      /** All the remote addresses are stored
                                          here. The addresses are referenced 
                                          from tunnel CBs. Stores structure of
                                          QoSgwPeerCb. */
   U32             gtpuTeidCntr;
   U32             TransIdCntr;
   TmrCfg          t3;              /* Request timer expiry */
#ifdef QO_TODO_SECOND_PHASE
   /* t3 to be configured based on procedure types */
   /* Peer list in peerCbHlCp is added dynamically. Configure the peers before
   hand.*/
#endif
}QoSgwCb;


typedef enum _qbRatType
{
   QO_RAT_RESERVED      = 0,  /* reserved */
   QO_RAT_UTRAN,              /* UTRAN */               
   QO_RAT_GERAN,              /* GERAN */               
   QO_RAT_WLAN,               /* WLAN */               
   QO_RAT_GAN,                /* GAN */               
   QO_RAT_HSPAE,              /* HSPA Evolution  */               
   QO_RAT_EUTRAN,             /* EUTRAN  */               
   QO_RAN_INVALID
} QoRatType;


/* SGW Global structure. */
EXTERN QoSgwCb qoSgwCb;

/**
 * Validates the incoming message.
 * Maps to outgoig TEID.
 * Updates few IEs.
 * Sends to the other entity. 
 */
EXTERN S16 QoSgwLiEgtEguDatInd ARGS((
         Pst          *pst,        /* post structure */
         SuId         suId,        /* eGTP-U service user SAP ID */
         EgtUEvnt     *datInd
      ));



EXTERN S16 QoSgwLiEgtEguErrInd ARGS((
         Pst          *pst,        /* post structure */
         SuId         suId,        /* eGTP-U service user SAP ID */
         EgtUEvnt     *errInd));   /* error event */



EXTERN S16 QoSgwLiEgtSigInd ARGS((
         Pst *pst,
         SuId suId,
         CmTptAddr *localAddr,
         CmTptAddr *remAddr,
         TknU32 lclTeid,
         EgMsg *egMsg
         ));



EXTERN S16 QoSgwLiEgtSigCfm ARGS((
      Pst         *pst,           /* post structure */
      SuId         suId,          /* Use values from EgwNodeSapId */
      CmTptAddr   *localAddr,     /* Local node address */
      CmTptAddr   *remAddr,       /* Remote node address */
      TknU32       teid,          /* */
      TknU32       transId,       /* */
      EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
      ));



EXTERN S16 QoSgwLiEgtLclDelCfm ARGS ((
         Pst          *pst,      /* post structure */
         SuId         suId,      /* eGTP service user SAP ID */
         U32          transId,    /* Transaction ID */
         U32          teid,      /* self TEID */
         CmStatus     status));  /* Status of request primitive */



EXTERN S16 QoSgwLiEgtEguStaCfm ARGS ((
         Pst *pst, SuId suId, EgtUEvnt *egtUEvnt
         ));



EXTERN S16 QoSgwLiEgtEguLclTnlMgmtCfm ARGS ((
         Pst          *pst,        /* post structure */
         SuId         suId,        /* eGTP-U service user SAP ID */
         EgtUEvnt     *mgmtCfm));  /* Tunnel Management Confirm */

EXTERN S16 QoSgwLiEgtEguTnlMgmtReq ARGS((
         Pst *pst,
         SpId spId,
         EgtUEvnt *egtUEvnt
         ));

EXTERN S16 QoSgwLiEgtEgcTnlModreq ARGS((
 Pst *pst,
 SpId spId,
 U32 transId,
 U32 teid,
 CmTptAddr oldDstIpAddr,
 CmTptAddr newDstIpAddr
 ));

EXTERN S16 EuLiEgtEgcTnlModreq ARGS((
 Pst *pst,
 SpId spId,
 U32 transId,
 U32 teid,
 CmTptAddr oldDstIpAddr,
 CmTptAddr newDstIpAddr
 ));


EXTERN S16 QoSgwLiEgtSigRsp ARGS((
         Pst *pst,
         SpId spId,
         TknU32 lclTeid,
         EgMsg *msg
         ));

EXTERN S16 QoSgwLiEgtEguDatReq ARGS((
         Pst *pst,
         SpId spId,
         EgtUEvnt *egtUEvnt
         ));

EXTERN S16 QoSgwLiEgtEguLclTnlMgmtReq ARGS((
         Pst *pst,
         SpId spId,
         EgtUEvnt *egtUEvnt
         ));

EXTERN S16 QoSgwLiEgtSigReq ARGS((
         Pst *pst,
         SpId spId,
         CmTptAddr *localAddr,
         CmTptAddr *remAddr,
         TknU32 lclTeid,
         TknU32 transId,
         TknU8 crtFlag,
         TmrCfg t3,
         EgMsg *msg
         ));


EXTERN S16 qoSgwActvInit  ARGS((
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
));

PUBLIC S16 qoSgwBuildCreateBearerReq ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));


EXTERN S16 qoSgwInitExt ARGS ((Void));

EXTERN S16 qoSgwUtlBldEgtMfyBearerRsp ARGS(( QoSgwTunnelCb  *tunlCb,
                                             EgMsg   **egMsg,
                                             EgMsg *egReqMsg,
                                             U8   cause ));

EXTERN S16 qoSgwUtlBldEgtDelSsnReq ARGS(( QoSgwTunnelCb  *tunlCb,
                                          EgMsg   **egMsg, 
                                          U8   cause ));

EXTERN S16 qoUtlAllocEgMsg ARGS((
         EgMsg   **egMsg,    /* GTP Message */
         Mem     *memInfo    /* memory information */
         ));

/* qo003.101 Changes ieCr is not present in rel9 ie header */
#ifndef EG_REL_930
EXTERN S16 qoUtlEgtBuildEgIeHeader ARGS((EgIe *egIe, U8 ieType, U8 ieInst,
                                         U8 ieCr, U8 dataType));
#else
EXTERN S16 qoUtlEgtBuildEgIeHeader ARGS((EgIe *egIe, U8 ieType, U8 ieInst,
                                         U8 dataType));
#endif /* EG_REL_930 */

EXTERN S16 qoUtlIpv4InetToArr ARGS((
   U8           *addrArr,        /* ascii address representation */
   CmInetIpAddr *address          /* 4 byte interent address */
   ));

EXTERN S16 qoUtlIpv4ArrToInet ARGS((
   U8           *addrArr,        /* ascii address representation */
   CmInetIpAddr *address          /* 4 byte interent address */
   ));


EXTERN Txt *qoSprintf ARGS((Txt *P_fmt, ...));

EXTERN S16 qoSgwInUserData ARGS((SuId suId, EgtUEvnt *data));

EXTERN S16 qoSgwInCSReq ARGS((
      SuId suId, CmTptAddr *localAddr, CmTptAddr *remAddr, TknU32 lclTeid, 
      EgMsg *msg));


EXTERN S16 qoSgwInDSReq ARGS((SuId suId, CmTptAddr *localAddr,
      CmTptAddr *remAddr, TknU32 lclTeid, EgMsg *msg));

EXTERN S16 qoSgwInCSRsp ARGS((SuId suId, CmTptAddr *localAddr,
      CmTptAddr *remAddr, TknU32 lclTeid, TknU32 transID, EgMsg *msg));

EXTERN S16 qoSgwInDSRsp ARGS((SuId suId, CmTptAddr *localAddr, 
      CmTptAddr *remAddr, TknU32 lclTeid, TknU32 transID, EgMsg *msg));

EXTERN QoSgwTunnelCb * qoSgwFindTunnel ARGS((QoSgwEgtSapCb *sapCb, U32 teid));

EXTERN S16 qoSgwOutUserData ARGS(( EgtUEvnt *data, QoSgwTunnelCb *dataTun,
      QoSgwEgtSapCb *sap));

/* qo001.101: Utility to build DL data notification */
EXTERN S16 qoSgwBldDDN ARGS((
QoSgwTunnelCb    *sgwTunlCb,
EgMsg            **egMsg
));

/* qo001.101: Handler for Bearer resource command */
EXTERN S16 qoSgwInBearResCmnd ARGS((
      SuId suId, 
      CmTptAddr *localAddr, 
      CmTptAddr *remAddr, 
      TknU32 lclTeid, 
      EgMsg *msg
     ));
/* qo001.101: Handler for Create Bearer Request */
EXTERN S16 qoSgwInCreateBearReq ARGS((
      SuId suId, 
      CmTptAddr *localAddr, 
      CmTptAddr *remAddr, 
      TknU32 lclTeid, 
      EgMsg *msg
     ));

/* qo001.101: Handler for Create Bearer Response */
EXTERN S16 qoSgwInCreateBearRsp ARGS((
      SuId suId, 
      CmTptAddr *localAddr, 
      CmTptAddr *remAddr, 
      TknU32 lclTeid, 
      EgMsg *msg
     ));

EXTERN S16 qoSgwInBRCmd ARGS((
                                       SuId           suId,
                                       CmTptAddr      *localAddr,
                                       CmTptAddr      *remAddr,
                                       TknU32         lclTeid,
                                       EgMsg          *msg));



EXTERN S16 qoSgwTransToTeidHashList ARGS((QoSgwEgtSapCb *sap, QoSgwTunnelCb
         *tunnel));
EXTERN S16 qoSgwGenerateCTransId ARGS((U32 *transId));

EXTERN S16 qoSgwCreateTunnelTrans ARGS((QoSgwEgtSapCb *sap, U32 transId, 
                                    QoSgwTunnelCb **tunnel));

EXTERN S16 qoSgwFindNCreatePeer ARGS((CmTptAddr *addr, QoSgwPeerCb **peer));

EXTERN S16 qoSgwCreatePeer ARGS((CmTptAddr *addr, QoSgwPeerCb **peer));

EXTERN S16 qoSgwCreateTunnel ARGS((QoSgwEgtSapCb *sapCb, U32 teid,
                                   QoSgwTunnelCb **tunnel));

EXTERN QoSgwTunnelCb * qoSgwFindTunnelTrans ARGS((QoSgwEgtSapCb *sap, 
                                                  U32          transId));

EXTERN S16 qoSgwFindNCreateTunnel ARGS((QoSgwEgtSapCb *sapCb, U32 teid,
                                        QoSgwTunnelCb **tunnel));

EXTERN S16 qoSgwFindNCreateUeCb ARGS((U8 *ueId, U8 ueIdLen, QoSgwUeCb **ue));

EXTERN S16 qoSgwGenerateGtpUTeid ARGS((U32 *teid));

EXTERN S16 qoSgwDeleteUeCb ARGS((
QoSgwUeCb       *ue,
QoSgwSigType    sigType
));

EXTERN S16 qoSgwCreateUeCb ARGS((QoImsi *imsi, QoSgwUeCb **ue));

EXTERN S16 qoSgwProcS5S8DefTnlMgmtCfm ARGS((EgtUEvnt *mgmtCfm, QoSgwTunnelCb *tunnel));
/* qo001.101: Handler for Dedicated tunnel management cfm */
EXTERN S16 qoSgwProcessS5s8DedTnlMgmtCfm ARGS((EgtUEvnt *mgmtCfm, QoSgwTunnelCb *tunnel));
EXTERN S16 qoSgwProcessS1DedTnlMgmtCfm ARGS((EgtUEvnt *mgmtCfm, QoSgwTunnelCb *tunnel));
EXTERN S16 qoSgwProcessS4cTnlMgmtCfm ARGS((QoSgwTunnelCb *tunnel,CmStatus status));

EXTERN S16 qoSgwProcessS11cTnlMgmtCfm ARGS((QoSgwTunnelCb *tunnel,CmStatus status));

EXTERN S16 qoSgwProcessS1UTnlMgmtCfm ARGS((EgtUEvnt *mgmtCfm, QoSgwTunnelCb *tunnel));

EXTERN S16 qoSgwProcS4DefTnlMgmtCfm ARGS((EgtUEvnt *mgmtCfm, QoSgwTunnelCb *tunnel));

EXTERN S16 qoSgwDeleteTunnel ARGS((QoSgwEgtSapCb *sap, QoSgwTunnelCb
         **tunnel));

PUBLIC S16 qoSgwHndls1UTunMgmtCfmAdd ARGS((
                                          EgtUEvnt             *mgmtCfm,
                                          QoSgwTunnelCb        *tunnel));

PUBLIC S16 qoSgwHndls4TunMgmtCfmAdd ARGS((
                                          EgtUEvnt             *mgmtCfm,
                                          QoSgwTunnelCb        *tunnel));

PUBLIC S16 qoSgwHndls1UTunMgmtCfmDel ARGS((
                                          EgtUEvnt             *mgmtCfm,
                                          QoSgwTunnelCb        *tunnel));

PUBLIC S16 qoSgwGetFreeBearerEntry ARGS((
                                       QoSgwBearerCb        **bearerTable,
                                       QoSgwBearerCb        *bearer,
                                       U32                  *bearerInd));

PUBLIC S16 qoSgwValCBReq ARGS((
                                             QoSgwPdnCb        *pdnCb,
                                             EgMsg             *egMsg,
                                             U32               *cause));

PUBLIC S16 qoSgwRemCreateBearerReqIes ARGS((
                                          QoSgwPdnCb     *pdnCb,
                                          EgMsg          *egMsg));

PUBLIC S16 qoSgwHndls1DedTunMgmtCfmAdd ARGS((
                                             EgtUEvnt             *mgmtCfm,
                                             QoSgwTunnelCb        *tunnel));

PUBLIC S16 qoSgwHndls1DedTunMgmtCfmDel ARGS((
                                             EgtUEvnt             *mgmtCfm,
                                             QoSgwTunnelCb        *tunnel));


PUBLIC S16 qoSgwValCSReq ARGS((
                                             EgMsg           *msg,
                                             QoSgwTunnelCb   *s11Tun,
                                             QoSgwPdnCb      **pdnCb,
                                             U32             *cause));

PUBLIC S16 qoSgwFindUeCbUsingImsi ARGS((
                                       EgMsg             *egMsg,
                                       QoImsi            *imsi,
                                       U32               *cause,
                                       QoSgwUeCb         **ueCb));
PUBLIC S16 qoSgwCreateUeCbUsingImsi ARGS((
                                       EgMsg             *egMsg,
                                       QoImsi            *imsi,
                                       U32               *cause,
                                       QoSgwUeCb         **ueCb));

PUBLIC S16 qoSgwCreateUTunnel ARGS((
                                 QoSgwPdnCb              *pdnCb,
                                 QoSgwTunnelCb           **uTun,
                                 QoInterfaceType         ifType));
   
   
PUBLIC S16 qoSgwDeleteUTunnel ARGS((QoSgwTunnelCb           *uTun));

PUBLIC S16 qoSgwValCSRsp ARGS((
                                                QoSgwPdnCb      *pdnCb,
                                                EgMsg           *egMsg,
                                                U32             *cause));

PUBLIC S16 qoSgwRemCSRspIes ARGS((EgMsg              *egMsg));


PUBLIC S16 qoSgwBldIACSRsp ARGS((
                                          QoSgwPdnCb     *pdnCb,
                                          EgMsg          *egMsg));

PUBLIC S16 qoSgwEguTunnelAddReq ARGS((
                                    QoSgwBearerCb           *bearer,
                                    EgMsg                   *egMsg,
                                    EgtIntfEnt              ifType));


PUBLIC S16 qoSgwRemCSReqIes ARGS((
                                          EgMsg          *egMsg,
                                          QoSgwTunnelCb  *s5s8Tun));

PUBLIC S16 qoSgwBldCSReq ARGS((
                                          QoSgwPdnCb     *pdnCb,
                                          EgMsg          *egMsg));

PUBLIC S16 qoSgwValMBReq ARGS((
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,
U32             *cause));

EXTERN S16 qoSgwValMBReqfrmSgsnIrat ARGS((
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,
U32             *cause,
CmTptAddr      *remAddr));

EXTERN S16 qoSgwValMBReqfrmMmeIrat ARGS((
QoSgwPdnCb      *pdnCb,
EgMsg           *egMsg,
U32             *cause,
CmTptAddr      *remAddr));

PUBLIC S16 QoSgwEgcTunnelModReq ARGS((
QoSgwUeCb               *ueCb,
EgtIntfEnt              ifType));

PUBLIC S16 qoSgwBldMBRspToSGSN ARGS((
QoSgwPdnCb     *pdnCb,
EgMsg          **egMsg));

PUBLIC S16 qoSgwGetPdnCb ARGS((
                              QoSgwUeCb               *ueCb,
                              U32                     transId,
                              QoSgwPdnCb              **pdnCb));


PUBLIC S16 qoSgwGetPdnCbForInitAttach ARGS((
                                          QoSgwUeCb               *ueCb,
                                          QoSgwPdnCb              **pdnCb));

PUBLIC S16 qoSgwBldMBRsp ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg      /* Modify Bearer Response received from MME */
));


PUBLIC S16 qoSgwValDSReq ARGS((
                                                QoSgwPdnCb        *pdnCb,
                                                EgMsg             *egMsg,
                                                U32               *cause));

PUBLIC S16 qoSgwGetPdnCbUsingEgMsg ARGS((
                                       QoSgwUeCb               *ueCb,
                                       EgMsg                   *egMsg,
                                       QoSgwPdnCb              **pdnCb));

PUBLIC S16 qoSgwInCBReq ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *msg        /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwInCBRsp ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *msg        /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwValCBRsp ARGS ((
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause,    /* Cause of validation Failure (OUT) */
Bool            *fwdRsp    /* forward the response failure message is recd */
));

PUBLIC S16 qoSgwBldCBRsp ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));

PUBLIC S16 qoSgwValDSRsp ARGS((
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause     /* cause of failure (OUT) */
));

PUBLIC S16 qoSgwRemCBRspIes ARGS((
QoSgwPdnCb     *pdnCb,
EgMsg          *egMsg));

PUBLIC S16 qoSgwEgtUtlGetIE ARGS((
EgMsg   *egMsg,
EgIe    **egIe,
U8      ieType,
U8      ieInst
));

PUBLIC S16 qoSgwEgtUtlSndFailureRspToMme
(
U32            locTeid,
U32            remTeid,
EgtMsgType     msgType, /* Type of Response message (IN) */
U32            transId, /* seq number (IN) */
U32            cause   /* Cause (IN) */
);

PUBLIC S16 qoSgwEgtUtlSndFailureRspToSgsn
(
U32            locTeid,
U32            remTeid,
EgtMsgType     msgType, /* Type of Response message (IN) */
U32            transId, /* seq number (IN) */
U32            cause   /* Cause (IN) */
);


PUBLIC S16 qoSgwEgtUtlSndFailureRsp ARGS((
U32            locTeid, /* PGW C-Teid (IN) */
EgtMsgType     msgType, /* Type of Response message (IN) */
U32            transId, /* seq number (IN) */
U32            cause,   /* Cause (IN) */
QoNodeType     target   /* peer */
));

PUBLIC S16 qoSgwDeletePdn ARGS((
QoSgwPdnCb      *pdnCb,
QoSgwSigType    sigType
));


PUBLIC S16 qoSgwDeleteBearer ARGS((
QoSgwBearerCb *bearer
));

EXTERN S16 qoSgwInMBReq ARGS((SuId suId, CmTptAddr *localAddr, 
      CmTptAddr *remAddr, TknU32 lclTeid, EgMsg *msg));

PUBLIC S16 qoSgwEgtUtlGetIeInBCtx ARGS((
EgMsg          *egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst,        /* Instance of ie in bearer ctx */
EgIe           **egIe          /* ie */
));

PUBLIC S16 qoSgwEgtUtlSndSuccessRsp ARGS((
QoSgwTunnelCb  *cTun,   /* Tunnel CB */
U32            transId, /* seq number (IN) */
EgtMsgType     msgType, /* Type of Response message (IN) */
QoNodeType     target   /* peer */
));

PUBLIC S16 qoSgwEgtUtlDelIeInBCtx ARGS((
EgMsg          **egMsg,        /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst        /* Instance of ie in bearer ctx */
));

PUBLIC S16 qoSgwInDBCmd ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwInDBReq ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwValDBReq ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause      /* cause of failure (OUT) */
));

PUBLIC S16 qoSgwBldDBReq ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));

PUBLIC S16 qoSgwInDBRsp ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwValDBRsp ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (IN) */
EgMsg          *egMsg,     /* Delete Bearer Response received from MME */
U32            *cause,     /* Cause of validation Failure (OUT) */
Bool           *fwdRsp     /* forward the response failure message is recd */
));

PUBLIC S16 qoSgwBldDBRsp ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));

PUBLIC S16 qoSgwGetPdnCbUsingTransId ARGS((
QoSgwUeCb               *ueCb,      /* UE control block */
U32                     transId,    /* Transaction ID */
QoSgwPdnCb              **pdnCb     /* PDN Control block */
));

PUBLIC S16 qoSgwProcessEgtErrInd
(
U16               suId,
U32               locTeid
);

PUBLIC S16 qoSgwEguTunnelModReq ARGS((
QoSgwBearerCb           *bearer,    /* Tunnel Control block */
EgMsg                   *egMsg,     /* gtp-c message (IN)   */
EgtIntfEnt              ifType      /* Interface type (IN)   */
));

PUBLIC S16 qoSgwHndls1UTunMgmtCfmMod ARGS((
EgtUEvnt             *mgmtCfm,   /* Management confirmation */
QoSgwTunnelCb        *tunnel     /* Tunnel Control Block */
));

PUBLIC S16 EuLiEgtTunnModCfm ARGS((
Pst      *pst,                   /* Pst structure for the sap */
SuId      suId,                  /* Service user ID of the SAP */ 
U32       transId,               /* Transaction ID */
U32       teid,                  /* teid of for which cfm is received */
CmTptAddr oldDstIpAddr,          /* Old dst Ip associated with tunnel */
CmTptAddr newDstIpAddr,          /* New IP or new peer address */
CmStatus  status                 /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
));

PUBLIC S16 qoSgwInDDNAck ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwValDDNAck ARGS((
QoSgwUeCb       *ueCb,     /* UE Control Block (OUT) */
EgMsg           *egMsg     /* GTP-C message (IN) */
));

PUBLIC S16 qoSgwInDDNFInd ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwValUBReq ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause      /* Cause of validation Failure (OUT) */
));

PUBLIC S16 qoSgwValUBRsp ARGS((
QoSgwPdnCb        *pdnCb,     /* PDN Control Block (OUT) */
EgMsg             *egMsg,     /* GTP-C message (IN) */
U32               *cause,    /* Cause of validation Failure (OUT) */
Bool              *fwdRsp    /* forward the response failure message is recd */
));

PUBLIC S16 qoSgwBldUBReq ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));

PUBLIC S16 qoSgwValUBReq ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (IN) */
EgMsg          *egMsg,     /* Update Bearer Response received from MME */
U32            *cause      /* Cause of validation Failure (OUT) */
));

PUBLIC S16 qoSgwBldUBRsp ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));

PUBLIC S16 qoSgwInMBCmd ARGS ((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwInUBRsp ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwInUBReq ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwCreatePdn ARGS((
QoSgwPdnCb              **pdnCb      /* PDN Control block */
));

PUBLIC S16 qoSgwEgtUtlFwdFailureRsp ARGS((
QoSgwTunnelCb  *cTun,   /* ctrl tunnel */
QoNodeType     target,  /* peer */
EgMsg          *egMsg   /* message to be forwarded to peer */
));

PUBLIC S16 qoSgwInDBFInd ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwInMBFInd ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwInBRFInd ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC Void qoSgwSndMsgToPgw ARGS((
QoSgwPdnCb              *pdnCb,     /* PDN Control block */
EgMsg                   *egMsg      /* gtp-c message to be sent out */
));

PUBLIC S16 qoSgwInMBRsp ARGS((
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
TknU32         transId,    /* Transaction id */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
));

PUBLIC S16 qoSgwValMBRsp ARGS((
QoSgwPdnCb      *pdnCb,    /* PDN Control Block (OUT) */
EgMsg           *egMsg,    /* GTP-C message (IN) */
U32             *cause,    /* cause of failure (OUT) */
Bool            *pFwdRsp   /* Forward the response to remove Node */
));

PUBLIC S16 qoSgwBldCBReq ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control block */
EgMsg          *egMsg      /* Modify Bearer Response received from MME */
));

PUBLIC S16 qoSgwGetPdnCbUsingTmpBearer ARGS ((
QoSgwUeCb               *ueCb,      /* UE control block */
EgMsg                   *egMsg,     /* gtp-c msg */
QoSgwPdnCb              **pdnCb      /* PDN Control block */
));

PUBLIC S16 qoSgwBldDSReq ARGS((
QoSgwPdnCb     *pdnCb,     /* PDN Control Block (OUT) */
EgMsg          **egMsg     /* MBRsp out pointer */
));

PUBLIC S16 qoSgwInRABReq
(
SuId           suId,       /* service user id */
CmTptAddr      *localAddr, /* local address */
CmTptAddr      *remAddr,   /* remote address */
TknU32         lclTeid,    /* local teid */
EgMsg          *egMsg      /* Modify Bearer Request Msg */
);

#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /* __SGWX__ */

/********************************************************************30**

         End of file:     qo.x@@/main/1 - Thu Jul  8 21:22:18 2010

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
/main/1      -     mm                    1.  LTE-CNE:SGW Initial release.
/main/1      qo001.101 rp   1. Added new interface types for ded tnls.
                            2. Added flag to indicated tunnel released.
                            3. Added new type for dedicated tunnel structure,
                               and replaced the double pointer for dedicated
                               tunnels with array.
                            4. Added list of EGTU events to be buffered on
                               S1 and a flag to indicated whether to start
                               buffering.
                            5. Added prototypes: qoSgwBldDDN,
                               qoSgwInBearResCmnd, qoSgwInCreateBearReq,
                            qoSgwInCreateBearRsp, qoSgwProcessS5s8UTnlMgmtCfm
/main/1  qo003.101 pmacharla  1.eGTP 2.1 Release upgrade under EG_REL_930 flag
*********************************************************************91*/
