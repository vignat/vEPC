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
 
     Name:     Trillium LTE CNE - PDN-GW 
  
     Type:     C include file
  
     Desc:     C structures for CNE node PGW
  
     File:     av_pgw.x 
  
     Sid:      av.x@@/main/1 - Tue Aug 17 18:54:32 2010
  
     Prg:      sv
  
*********************************************************************21*/
#ifndef __AV_PGW_X__
#define __AV_PGW_X__


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************/
/*                PDN-GW                               */
/*******************************************************/

/* Basic types and forward references */
typedef U32 TeId;

struct avPgwUeCb;
struct avPgwUeRouteCb;
struct _AvPgwPdnCb;


/* EGTP Tunnel types */
typedef enum avPgwTunnelType
{
   AV_PGW_CTRL_TUNNEL      = 0,
   AV_PGW_DATA_TUNNEL_DFLT,
   AV_PGW_DATA_TUNNEL_DED,
   AV_PGW_DATA_TUNNEL = (AV_PGW_DATA_TUNNEL_DFLT | AV_PGW_DATA_TUNNEL_DED)

}AvPgwTunnelType;

/* EGTPC Tunnel management states */
typedef enum avPgwEgtCState
{
   AV_PGW_EGTPC_TUNNEL_DOWN= 0,    /* Tunnel Down */
   AV_PGW_EGTPC_TUNNEL_PENDING,    /* Tunnel Pending */
   AV_PGW_EGTPC_TUNNEL_UP,         /* Tunnel Up */
   AV_PGW_EGTPC_TUNNEL_PROC        /* message processing ongoing */
}AvPgwEgtCState;

/* EGTPU Tunnel management states */
typedef enum avPgwEgtUState
{
   AV_PGW_EGTPU_TUNNEL_DOWN= 0,    /* Tunnel Down */
   AV_PGW_EGTPU_TUNNEL_PENDING,    /* Tunnel Pending */
   AV_PGW_EGTPU_TUNNEL_UP          /* Tunnel Up */
}AvPgwEgtUState;

/* EGTPC Tunnel management events */
typedef enum avPgwEgtCEvnt
{
   AV_PGW_EGTPC_MSG_CS_REQ_ACC = 1,   /* Create Session Req Success */
   AV_PGW_EGTPC_MSG_CS_REQ_REJ,       /* Create Session Req No Success */
   AV_PGW_EGTPC_MSG_MB_REQ_ACC,       /* Modify Bearer Req Success */
   AV_PGW_EGTPC_MSG_MB_REQ_REJ,       /* Modify Bearer Req No Success */
   AV_PGW_EGTPC_MSG_DS_REQ_ACC,       /* Delete Session Req Success */
   AV_PGW_EGTPC_MSG_DS_REQ_REJ,       /* Delete Session Req No Success */ 
   AV_PGW_EGTPC_MSG_BR_RES_ACC,       /* Bearer Res Allocation Succces */ 
   AV_PGW_EGTPC_MSG_DB_CMD_ACC,       /* Delete Bearer Rsp, Success */
   AV_PGW_EGTPC_LOC_TUN_DEL,          /* Local Deletion of Tunnels due to 
                                       * some internal error   */
   AV_PGW_EGTPC_MSG_UNKNOWN
}AvPgwEgtCEvnt;



#if 1
/*
Packet filter component type identifier
Bits
8 7 6 5 4 3 2 1 
0 0 0 1 0 0 0 0 IPv4 remote address type
0 0 1 0 0 0 0 0 IPv6 remote address type
0 0 1 1 0 0 0 0 Protocol identifier/Next header type
0 1 0 0 0 0 0 0 Single local port type
0 1 0 0 0 0 0 1 Local port range type
0 1 0 1 0 0 0 0 Single remote port type 
0 1 0 1 0 0 0 1 Remote port range type
0 1 1 0 0 0 0 0 Security parameter index type
0 1 1 1 0 0 0 0 Type of service/Traffic class type
1 0 0 0 0 0 0 0 Flow label type

All other values are reserved.


For "IPv4 remote address type", the packet filter component value field shall be
encoded as a sequence of a four octet IPv4 address field and a four octet IPv4
address mask field. The IPv4 address field shall be transmitted first. */

/* av001.101: Added TFT Related Data Structures */
/* Add all the variabled required for TFT encoding/decoding */
/*Ipv4 Remote Addr type */
typedef U8 AvPgwPres;               /* presence indicator */

typedef struct avPgwTftPfIpv4
{
   AvPgwPres   pres;
   U8          ip4[AV_PGW_IPV4_SIZE];
}AvPgwTftPfIpv4;

/*Ipv6 Remote Addr type */
typedef struct avPgwTftPfIpv6
{
   AvPgwPres   pres;
   U8          ip6[AV_PGW_IPV6_SIZE];
}AvPgwTftPfIpv6;

typedef struct avPgwTftProtIden
{
   AvPgwPres    pres;
   U8           protType;  
}AvPgwTftProtIden;


typedef struct avPgwTftPort
{
   AvPgwPres    pres;
   U16          port;  
}AvPgwTftPort;

typedef struct avPgwTftPortRange
{
   AvPgwPres    pres;
   U16          rangeLow;  
   U16          rangeHigh;  
}AvPgwTftPortRange;

typedef struct avPgwTftSecParam
{
   AvPgwPres   pres;
   U8          params[AV_PGW_IP_SEC_SIZE];  
}AvPgwTftSecParam;

typedef struct avPgwTftTos
{
   AvPgwPres   pres;
   U8          tos;  
   U8          mask;  
}AvPgwTftTos;

typedef struct avPgwTftIpv6FlowLbl
{
   AvPgwPres   pres;
   U8          buf[AV_PGW_IPV6_FLOW_LABEL_SIZE];  
}AvPgwTftIpv6FlowLbl;

/* TFT Parameter Type */
typedef enum avPgwTftParamType
{
   AV_PGW_TFT_PARAM_AUTH_TOKEN= 0x1,
   AV_PGW_TFT_PARAM_FLOW_ID,
   AV_PGW_TFT_PARAM_FILTER_ID
}AvPgwTftParamType;


typedef struct avPgwTftPf
{
   AvPgwPres            pres;         /* present or Not*/ 
   U8                   id;           /* Packet Filter idenntifier*/   
   U8                   dir;          /* Direction */ 
   U8                   preced;       /* Precedence */
   U8                   len;          /* length of Filter Content */
   AvPgwTftPfIpv4       ipv4;         /* Ipv4 Address */
   AvPgwTftPfIpv6       ipv6;         /* Ipv6 Address */
   AvPgwTftProtIden     protId;       /* Protocol Identifier */
   AvPgwTftPort         localPort;    /* Local  Port Idemtifier */
   AvPgwTftPort         remotePort;   /* Remote Port Idemtifier */
   AvPgwTftPortRange    locPortRange; /* Port Range */
   AvPgwTftPortRange    remPortRange; /* Port Range */
   AvPgwTftSecParam     secParam;     /* Security  Param */
   AvPgwTftTos          tos;          /* Type of Service  Param */
   AvPgwTftIpv6FlowLbl  flowLabel;    /* Flow Label   */
}AvPgwTftPf;

typedef struct avPgwTftParam
{
   AvPgwTftParamType      paramType;
   U8                     len;
   U8                     buf [AV_PGW_TFT_MAX_PARAM_BUF]; 
}AvPgwTftParam;

/*TFT Operation Code */
typedef enum avPgwTftOpCode
{
   AV_PGW_TFT_OPCODE_SPARE = 0,
   AV_PGW_TFT_OPCODE_NEWTFT ,
   AV_PGW_TFT_OPCODE_DELTFT ,
   AV_PGW_TFT_OPCODE_ADD_FILTER,
   AV_PGW_TFT_OPCODE_REP_FILTER,
   AV_PGW_TFT_OPCODE_DEL_FILTER,
   AV_PGW_TFT_OPCODE_NOTFTOP ,
   AV_PGW_TFT_OPCODE_RESERVED 
} AvPgwTftOpCode;

typedef struct avPgwTft
{
   AvPgwPres         pres;                            /* Present or not */  
   U32               len;  
   AvPgwTftOpCode    opCode;                          /* TFT Operation Code*/
   U8                eBit ;                           /* param List presnt */
   U8                noOfPfs;                         /* No Of packet Filters */
   AvPgwTftPf        pfList[AV_PGW_MAX_PKT_FILTERS];  /* Pf List */
   U8                noOfParams;                      /* No Of Params */
   AvPgwTftParam     params[AV_PGW_MAX_PARAMS];       /* Tft Params */  
}AvPgwTft;
#endif



typedef struct {
    /*  Type of Service and Traffic Class Type = Octect 1 */
    /*  Type of Service and Traffic Class Type mask  = Octect 2 */
    U8                              tos_tc;     
    U8                              tos_tc_mask;
} AvPgwSrvClassComp;

typedef struct
{
   CmIpv4TptAddr     ipv4_addr; /* IP transport address */
   CmIpv4TptAddr     ipv4_addr_mask; /* IP transport address */
} AvPgwTftIpv4Comp;

typedef struct
{
   CmIpv6TptAddr     ipv6_addr;
   CmIpv6TptAddr     ipv6_addr_mask;
} AvPgwTftIpv6Comp;


typedef struct
{
   union
   {
      AvPgwTftIpv4Comp  remIpv4Comp;
      AvPgwTftIpv6Comp  remIpv6Comp;
   } uRemIpAddr;

   U16                  locPort;     
   U16                  locPortRangeLow;     
   U16                  locPortRangeHigh;     
   U16                  remPort;     
   U16                  remPortRangeLow;     
   U16                  remPortRangeHigh;     
   U8                   proto_id;       
   U8                   ipsecParamInd; 
   AvPgwSrvClassComp    srvClass; 
   U32                  flowLabelType; 
} AvPgwPktFilterComp;


typedef struct {
    /* The packet filter identifier : least significant 4 bits       */
    /* The packet filter direction : bits 5 and 6 (7,8 bit spare)    */
    /* Packet filter evaluation precedence                           */
    U8                              pfId;  
    U8                              pktFilterDir; 
    U8                              precedence;

    U16                             presence_mask; /* component presence mask */
    AvPgwPktFilterComp              component;

} AvPgwEgPktFilter;


typedef struct {
    /* TFT Operation Code */
    U8                              opCode; 

    U8                              paramLstPres;     /*  E bit 0 or 1    */
    U8                              pktFilterCnt;     /*  4 bits : 15     */

    AvPgwEgPktFilter                *pktFilters[GTP_MAX_PKT_FILTERS_PER_TFT];
} AvPgwEgTft;

typedef AvPgwEgTft   AvPgwEgTad;



/* The following structure represents a tunnel for an
   UE. An UE can consist of multiple tunnels
   represented by a bearer */
typedef struct avPgwUeTunnelCb
{
   CmHashListEnt ueTunHashEnt; /* Hashlist manipulator */
   TeId          localTeIdKey; /* Local Tunnel ID for this tunnel */
   TeId          remoteTeId;   /* Remote Tunnel ID mapped to this tunnel */

   AvPgwTunnelType  tunnelType; /* Control/Data Default/ Data Dedicated Tunnel*/
   U8            state;        /* State of the tunnel */
   CmTptAddr     lclAddr;      /* Local Tunnel Transport Address details -
                                  Reference to Transport Address Cb */
   CmTptAddr     remAddr;      /* Remote Tunnel Transport Address details -
                                  Reference to Transport Address Cb */

   U8            bearerId;     /* EPS Bearer ID */
   struct avPgwUeCb  *ueCb;    /* Reference to UE Cb to which this tunnel
                                  belongs to. */
   struct avPgwUeRouteCb *ueRouteCb;  /* Reference to UE RouteCb to which this
                                  tunnel belongs to. */
   struct _AvPgwPdnCb    *pdnCb;
   TknU32        transID;     /* Transaction ID pertaining to the TunnelCb. */

#ifndef EG_REL_930
   U16           seqNum;      /* Current sequence number for this tunnel */  
#else
   U32           seqNum;      /* Current sequence number for this tunnel */  
#endif /* EG_REL_930 */

   EgBearQos     epsQos;      /* EpsQos to be passed in the Bearer */
   U32           tftLen;      /* TFT Buffer Length */    
   U8            tftBuf[AV_PGW_MAX_TFT];   
   AvPgwEgTft    tft;         /* TFT Strcuture */ 

   EgMsg          *pEgMsg;    /* Stores the Eg message, which is sent to 
                               * SGW once tunnel mgmt/add/del procedures
                               * confirmation is received from EGT
                               */

   TeId           newRemTeid; /* New SGW teid, recieved during S1-HO
                               * with SGW change
                               */

   CmTptAddr      newRemAddr; /* SGW address, used during Tunnel Mgmt 
                               * for changing the remote teid, and ip address
                               * during S1 Handover with SGW change
                               */
} AvPgwUeTunnelCb;


typedef enum _AvPgwBearerType
{
   AV_PGW_INV_BEARER,      /* Invalid Bearer Entry */
   AV_PGW_DEF_BEARER,      /* Default Bearer */
   AV_PGW_DED_BEARER       /* Dedicated Bearer */
} AvPgwBearerType;

/* A UE can have multiple Default Bearers because of PDN-Connection Request
 * Every Default Beaerer will have a Control tunnel & Data Tunnel.
 */
typedef struct _AvPgwBearerCb
{
   Bool                    isDefBearer;   /* TRUE if its default bearer */
   U32                     ebi;           /* EPS Bearer ID */
   struct _AvPgwPdnCb      *pdnCb;        /* PDN Connection details */ 
   AvPgwUeTunnelCb         *s5s8UTun;     /* S5/S8 User Plane tunnel */
   U8                      op_mask;       /* Bearer operation bit mask */

} AvPgwBearerCb;

typedef struct _AvImsi
{
   U32            len;                    /* length of the buffer */
   U8             buf[AV_PGW_IMSI_LEN];   /* imsi buffer */
} AvImsi;

/* E-UTRAN ULI */
typedef struct avPgwUli
{
   EgTAI       tai;                    /**<  Tracking Area Identity */
   EgECGI      ecgi;                   /**<  E-UTRAN Cell Global Identifier */
} AvPgwUli;

/*  This structure represents UE Control block in PDN-GW's
    scope on S5/S8 Interface.*/
typedef struct avPgwUeCb
{
   CmHashListEnt  ueHashEnt;  /* Hashlist manipulator */
   AvImsi         imsi;       /* UE CB key - IMSI */
   U32            numBearers; /* number of bearers present in the UE */
   AvPgwUli       uli;        /* UE ULI */
   AvPgwBearerCb  *bearerTable[AV_PGW_MAX_BEARERS];  /* Bearer Map for 
                                                      * this UE */
} AvPgwUeCb;

typedef enum
{
   AV_PGW_EGTPC_MSG_INV,
   AV_PGW_EGTPC_MSG_CB_RSP
} AvPgwExpMsg;

typedef struct _avPgwPendingTun
{
   CmLList           lnk;     /* Link for the linked list */
   AvPgwUeTunnelCb   *uTun;   /* Tunnel Cb */

} AvPgwPendingTun;

typedef struct
{
   CmLList           lnk;        /* Link for the linked list */
   U32               locTeid;    /* tunnel on which event it pending */
   U32               eventType;  /* event type */
   EgMsg             *egMsg;     /* gtp-c message pending */

} AvPgwPendingEvnt;


typedef struct _AvPgwPdnCb
{
   AvPgwUeCb         *ueCb;            /* UE Control Block */
   AvPgwUeTunnelCb   *s5s8CTun;        /* S5/S8 Control plane tunnel */
   CmInetNetAddr     ipAddr;           /* IP Address allocated to PDN */
   U32               bearerMap;        /* Bitmap for holding the EBI's
                                        * belonging to this PDN Connection
                                        */
   U32               numUTun;          /* Number of U-Tunnels in thisPDN */

   CmLListCp         pendingTunLst;    /* Used for Create Bearer Request/Rsp
                                        * type of AvPgwPendingTun */

   TknU32            transId;          /* Procedure TID of the Tunnel */

   /* av004.101 Changes for eGTP Rel9 upgrade seqnumber is 24bit wide now */   
#ifndef EG_REL_930
   U16               seqNum;           /* Current sequence number PDN */  
#else
   U32               seqNum;           /* Current Sequestn Number of PDN */  
#endif /* EG_REL_930 */
   
   AvPgwEgtCEvnt     cEvent;           /* control plane event */

   EgtMsgType        trigReq;          /* Request to be triggered after
                                        * Bearer Resource Command
                                        */
   U8                pti;              /* procedure transaction id iE */

   EgtMsgType        expectedMsg;      /* expeceted message from sgw */
} AvPgwPdnCb;



/* PDN-GW Lower SAP Control Block with EGTP */
/* Lower SAP with 
   1. For EGTPC - Control Plane towards S-GW on S5/S8 interface.
   2. For GTPU - Data Plane towards S-GW on S5/S8 interface.
*/
typedef struct avPgwEgtSapCb
{
   Pst           pst;            /* Service user post structure */
   State         state;          /* State of this SAP */
   SuId          suId;           /* Service user SAP id */
   SpId          spId;           /* Service Provider Id */
   U32           nmbConn;        /* Number of Bearers through this SAP */
   CmHashListCp  ueTunnelCbHlCp;  /* Hash List of Tunnel Context control
                                    blocks for the UE involved */
} AvPgwEgtSapCb;

/* The following structure represents the packet filter control block
   applied for a downlink data packet arrived at PDN-GW from external network.
   This filter shall be applied for the DL data packet when forwarding 
   to S-GW on S5/S8 interface based on lowest evealuation precedance index
   and a matching packet filter component type.

   This refers to an exclusive IP Address allocated to a known
   UE to this PDN-GW fetching the required service on default or dedicated
   bearer through a mapped unique tunnel/bearer to it.
   
   This will be used exclusively by recieve task to check if destination
   tunnel data packet to UE is intended one. 
*/
typedef struct _AvPgwTunInfo
{
	TeId				remTeid;
   CmIpv4TptAddr  remAddr;
	CmIpv4TptAddr	locAddr;
   AvPgwEgtUState state;

} AvPgwTunInfo;

typedef struct _AvPgwPktFilterCb
{
   Bool                 empty;         /* Is this packet filter is empty */
	AvPgwTunInfo			tunInfo;
   U32                  pFilterPort;    /* Packet filter Key - Remote Port 
                                         * range component */
   U8                   pFId;           /* Packet filter identifier */
   U8                   pFDirection;    /* Pkt filter direction - UL/DL/Both*/
   U8                   evalPrecedIndex; /* Pkt filter evaluation precedance */

   U8                   pfCompPres; /* PF component presense mask */
   AvPgwPktFilterComp   pfComp;     /* packet filter components */
   struct _AvPgwPktFilterCb   *pNext;
} AvPgwPktFilterCb;


/* IP address to MAC address mapping for all the external network 
   details */
typedef struct avPgwRoutIP2MacMap
{
   U8            ipAddrStr[AV_PGW_ROUT_IP_ADDR_LEN]; /* External network node IP 
                                                  Address string */
   U32           ipAddr;                       /*  External network node IP 
                                                  Address */
   U8            macAddr[AV_PGW_ROUT_MAC_ADDR_LEN]; /* External network node
                                                  MAC Address */
}AvPgwRoutIP2MacMap;


/* IP Address allocation control block by PDN-GW */
typedef struct avPgwNwAddrCfg
{
   CmInetNetAddr addr;      /* IP Address details */

  /* Need to check if the following can be avoided; As we will
   insert in to routCb once the next address is taken.
   The tracking can be based on global counter 
   if the insertion is failed, due to maximum entries, and the counter 
   gets reset, then we are exhausted to serve the UEs */

   Bool          assigned;  /* To track if the address is in use or not */
}AvPgwNwAddrCfg;

/* IP Address allocation set to UEs by PDN-GW */
/* Can add port too if required. */
typedef struct avPgwUeRouteCb
{
   CmLList              lnk;        /* Link for the linked list      */

   AvPgwNwAddrCfg       nwUeAddr;   /* Network Address configured    */

} AvPgwUeRouteCb;

/* av004.101 Added IPCb which contains UE Data tunnel info */

typedef struct avPgwIpCb
{
   AvPgwNwAddrCfg       nwUeAddr;      /* Network Address configured    */
	AvPgwTunInfo			defTunInfo;
   U8                   pktFilterCnt;  /* number of packet filters      */

   AvPgwPktFilterCb     *pHead;        /* Points to Highest precendence 
                                        * Pakcket Filter in 
                                        * uePktFilterCbArr
                                        */

   AvPgwPktFilterCb     uePktFilterCbArr[AV_PGW_MAX_PKT_FILTERS];
   AvPgwPdnCb           *pdnCb;

} AvPgwIpCb;

/* Main route control block for communicating with external network */
/* SV- Need to check for
1. MAC Address range
2. Ethernet Packet range
3. ARP Packet range 
*/
typedef struct avPgwRouteCb
{
   TskInit    init;           /* Task initialization structure */
   Pst        pst;            /* Pst info to route packet back to
                                 S-GW */
#ifndef AV_PERF
   pcap_t     *pcapHndl;      /* Libpcap Handler reference */
#endif

   U8         ethPkt[AV_PGW_ROUT_MAX_ETH_PKT_LEN]; /* Total Ethernet Packet - used 
                                            everytime to send the data out
                                            to external network. Only the
                                            payload changes everytime and
                                            destination */

   U8         arpPkt[AV_PGW_ROUT_MAX_ARP_PKT_LEN];     /* ARP Packet */

   S8         ethIntf[AV_PGW_ROUT_PCAP_MAX_DEV_LEN];   /* Ethernet Interface */

   U8         localIPAddr[AV_PGW_ROUT_IP_ADDR_LEN];  /* Self IP Address of PDN-GW */
   U8         localMACAddr[AV_PGW_ROUT_MAC_ADDR_LEN];  /* Local MAC Address of PDN-GW */


   U32        mapCnt;     /* Number of external network
                             nodes */
   AvPgwRoutIP2MacMap ip2Mac[AV_PGW_MAX_NW_NODES];  /* External network nodes(s)' 
                                                       IP - MAC
                                                       address mapping */

   U16        nmbUeAddr;      /* Number of UE network addresses configured */
   U16        nmbUeNwAddrCnt; /* Number of existing UEs in service
                                 to which IP Address has been allocated*/

	/* av004.101 ip address allocation related changes */
   U16            maxIpsAvail;      /* Maximum number of IP addresses
                                     * available at PDN */

   CmLListCp      ueRouteCbLst;      /* ue route cb */
}AvPgwRouteCb;


/* AV PDN-GW Global Control Block */
typedef struct _avPgwCb
{
   TskInit         init;           /* Task initialization structure */
   AvPgwGenCfg     pgwCfg;         /* PDN-GW's general configuration */
   AvPgwPeerCfg    peerCfg;        /* All peer details for this PDN-GW */

   AvPgwRouteCb    routeCb;        /* PDN-GW's global route control block
                                      to interface with external network node */

   AvPgwEgtSapCb  *egtEchoSapCb;   /* Upper EGTPC SAP for PGW for echo */

   AvPgwEgtSapCb  *egtCSapCb;      /* Upper EGTPC SAP for this PGW on 
                                      S5/S8 interface */

   AvPgwEgtSapCb  *egtUSapCb;      /* Upper GTPU SAP for this PGW on 
                                      S5/S8 interface */

   CmHashListCp   ueCbHlst;        /* Hash list of UE control blocks */

   U32            gtpUTeIdCntr;    /* Global TE ID Allocator for GTP-U */

   AvPgwIpCb      *ipCbsList;      /* List of IPs Configured */
   
   CmLListCp      pendingEvntLst;  /* pending gtp-c event list */ 
}AvPgwCb;


EXTERN AvPgwCb avPgwCb;


/*******************************************************/
/* PDN-GW Function declarations */
/*******************************************************/

/* EGTPC Messages building Prototypes */
/* P-GW to S-GW */
EXTERN S16 avPgwUtlBldEgtDelSessionRsp ARGS ((AvPgwUeTunnelCb *ueTunlCb,
                                              EgMsg           **egMsg,
                                              U8              causeVal));

EXTERN S16 avPgwRouteInit ARGS((Void));


/* av004.101 to add routing entry for a bearer */
EXTERN S16 avPgwAddUeRouteCb ARGS((AvPgwUeTunnelCb *dataTunnel,
                                   AvPgwUeRouteCb  **newUeRouteCb));

EXTERN S16 avPgwInitUeRouteCb ARGS((AvPgwUeTunnelCb *dataTunnel,
                                     AvPgwUeRouteCb  *newUeRouteCb));

PUBLIC  S16 avPgwModUeRouteCb ARGS((AvPgwUeTunnelCb *pUTun));

EXTERN S16 avPgwDelUeRouteCb ARGS((AvPgwUeTunnelCb *dataTunnel));

EXTERN S16 avPgwEgtCTunnelMachine ARGS((AvPgwUeTunnelCb *tunnelCb,
                                        CmTptAddr       *lclAddr,
                                        CmTptAddr       *remAddr,
                                        TeId             teId,
                                        AvPgwEgtCEvnt    evnt,
                                        EgMsg           *egMsg));

EXTERN S16 avPgwEgtUTunnelMachine ARGS((AvPgwUeTunnelCb *tunnelCb,
                                        TeId             teId,
                                        U8               action,
                                        EgtUEvnt        *eguEvnt));

EXTERN S16 avPgwUtlBldEgtDatReq ARGS((AvPgwTunInfo      *tunInfo,
                                      EgtUEvnt          **eguEvt));


EXTERN  Void  avPgwUtlBuildEguHeader ARGS((TeId             remTeid,
                                           EgUMsgHdr         *egHdr,
                                           U8                msgType));

EXTERN  S16 avPgwUtlAllocEguMsg ARGS((EgUMsg **egMsg,
                                      Mem    *mem));

EXTERN  S16 avPgwUtlDeAllocEguMsg ARGS((EgUMsg **egMsg));

EXTERN  S16 avUtlAllocEgMsg ARGS((EgMsg **egMsg, 
                                  Mem   *memInfo));

/* av001.101: Utility to add APN */
EXTERN S16 avUtlEgtAddApnRstr ARGS((EgMsg         *egMsg,
                                    EgIeLevelInfo *egLvlInfo,
                                    U8            *apnRstr,
                                    U8            inst));

EXTERN S16 avUtlEgtAddFteid ARGS((EgMsg         *egMsg, 
                                  EgIeLevelInfo *egLvlInfo,
                                  U32            teId, 
                                  U8             ebi, 
                                  U8            *ipAddr,
                                  U8             intfType, 
                                  U8             inst));

EXTERN S16 avUtlEgtAddEpsBId ARGS((EgMsg         *egMsg, 
                                   EgIeLevelInfo *egLvlInfo,
                                   U8             ebiVal, 
                                   U8             inst));

EXTERN S16 avUtlEgtAddChargIdIe ARGS((EgMsg         *egMsg,
                                   EgIeLevelInfo *egLvlInfo,
                                   U8             inst,
                                   U8             chrgId));

/* av001.101: Utility to decode TFT */
EXTERN S16 avPgwUtlDecTft ARGS((U8       *buf,
                                U8       *indx,
                                AvPgwEgTft *tft));

EXTERN S16 avUtlEgtAddCause ARGS((EgMsg         *egMsg, 
                                  EgIeLevelInfo *egLvlInfo,
                                  U8             cause,
                                  U8             inst));

EXTERN Void  avUtlBuildEgHeader ARGS((EgMsgHdr    *egHdr,
                                      U32          teId,
                                      U8           msgType));
EXTERN S16 avUtlEgtAddPAA ARGS((EgMsg            *egMsg,
                                EgIeLevelInfo    *egLvlInfo,
                                U8               *ipAddr,
                                U8               inst));

EXTERN S16 avPgwSendArpReqPkt ARGS((U8  *ipAddr));


EXTERN S16 avPgwFndUeTunnelCb ARGS(( AvPgwUeTunnelCb **tunnel,
                                     U32               teId,
                                     AvPgwTunnelType   tunnelType));

EXTERN Void avPgwRoutHndlSndPkt ARGS((Buffer *mBuf));

EXTERN Void *avPgwPktReceiver ARGS((Void *arg));

#ifdef AV_PERF
EXTERN Void avSendEgDLData ARGS((Void));
#endif
EXTERN S16 avPgwDelUeTunnelCb ARGS((AvPgwUeCb         *ueCb,
                                    AvPgwUeTunnelCb   **tunnel,
                                    AvPgwTunnelType   tunnelType));


PUBLIC S16 avPgwFndUeCb ARGS((
AvPgwUeCb     **ueCb,
AvImsi        *imsi
));


PUBLIC S16 avPgwAddUeCb ARGS((
AvImsi         *imsi,
AvPgwUeCb      **ueCb
));

PUBLIC S16 avPgwFndNCreateUeCb ARGS((
EgMsg          *egMsg,   /* gtp-c message with IMSI IE (IN) */
AvPgwUeCb      **ueCb    /* Ue CB (OUT) */ 
));


PUBLIC S16 avPgwEgtValCSReq ARGS((
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN control block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
));


PUBLIC AvPgwBearerCb* avPgwFndBearer ARGS((
AvPgwUeCb         *ueCb,      /* UE CB (INT) */
U32               locTeid    /* Local C-TEID (IN) */
));


PUBLIC S16 avPgwUtlIpv4ArrToInet ARGS((
U8           *addrArr,        /* Byte Array address representation */
U32          *address         /* 4 byte interent address */
));

PUBLIC S16 avPgwAddUeDataTunnelCb ARGS((
AvPgwUeTunnelCb   **tunnelCb,
AvPgwUeCb         *ueCb,
CmTptAddr         *locAddr,
AvPgwTunnelType   tunnelType
));

PUBLIC S16 avPgwEgtSndFailureRsp ARGS((
AvPgwPdnCb     *pdnCb,   /* UE control block (IN) */
U32            locTeid, /* PGW C-Teid (IN) */
CmTptAddr      *locAddr, /* Local Addr */
CmTptAddr      *remAddr, /* Remote Addr */
EgtMsgType     msgType, /* Type of Response message (IN) */
TknU32         seqNo,   /* seq number (IN) */
U32            cause    /* Cause (IN) */
));

PUBLIC S16 avPgwUtlGenerateGtpUTeId ARGS((TeId  *teId));


PUBLIC S16 avPgwEgtUtlGetIE ARGS((EgMsg   *egMsg,
                                   EgIe    **egIe,
                                   U8      ieType,
                                   U8      ieInst));


PUBLIC AvPgwUeTunnelCb* avPgwUtlFindPendingTun ARGS((
CmLListCp            *tunList,
U32                  teid
));

PUBLIC S16 avPgwAddUeCtrlTunnelCb ARGS((
AvPgwUeTunnelCb   **tunnelCb, /* Tunnel Control block (OUT) */
AvPgwUeCb         *ueCb,      /* UE CB (INT) */
U32               locTeid,    /* Local C-TEID (IN) */
CmTptAddr         *locAddr,   /* Local Address */
CmTptAddr         *remAddr,   /* Remote Address */
EgMsg             *egMsg      /* gtp-c msg (INT) */
));


PUBLIC AvPgwUeTunnelCb* avPgwFndDefUTun ARGS((AvPgwPdnCb        *pdnCb));

PUBLIC S16 avPgwEgtBldCBReq ARGS((
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg,
U8                causeVal
));


PUBLIC S16 avPgwValBRCmd ARGS((
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
));

PUBLIC S16  avPgwUtilEgtAddPTidIe ARGS((
EgMsg      *egMsg,
U8         pTidValue,
EgIeLevelInfo *egLvlInfo,
U8               inst
));

PUBLIC S16 avPgwEgtValCBRsp ARGS((
AvPgwUeCb      *ueCb,      /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,     /* PDN Control Block (INOUT) */
EgMsg          *egMsg,     /* Create Session Request message (IN) */
Bool           *sndDBReq   /* whether to send DB req to SGW or not (OUT) */
));


PUBLIC S16 avPgwUtlDelPendingTuns ARGS ((
AvPgwPdnCb           *pdnCb,
Bool                 sndDBReq
));

PUBLIC S16 avPgwAddPendingTunnelCb ARGS((
AvPgwPdnCb        *pdnCb,
AvPgwUeTunnelCb   **tunnelCb,
CmTptAddr         *locAddr,
AvPgwTunnelType   tunnelType
));

PUBLIC S16 avPgwEgtUtlBldCSRsp ARGS((
AvPgwUeTunnelCb  *ueTunlCb, 
EgMsg            **egMsg,
U8               causeVal
));

PUBLIC AvPgwBearerCb* avPgwFndDefBearer ARGS((
AvPgwPdnCb        *pdnCb     /* UE CB (INT) */
));

PUBLIC S16 avPgwUtlGetTransId ARGS ((void));

PUBLIC S16 avPgwBldDeleteBearerRequest ARGS((
AvPgwPdnCb       *pdnCb,
EgMsg            **egMsg
));

PUBLIC S16 avPgwUtlDelPendingTunLst ARGS((
CmLListCp            *tunLst
));

PUBLIC S16 avPgwValDBCmd ARGS((
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
));

PUBLIC S16 avPgwBldDBReq ARGS((
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg
));

PUBLIC S16 avPgwEgtValDBRsp ARGS((
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg   /* Create Session Request message (IN) */
));

PUBLIC S16 avPgwDelUeCb ARGS((AvPgwUeCb    **ueCb));

PUBLIC S16 avPgwDelBearerCb ARGS((
AvPgwBearerCb     *bearer
));

PUBLIC S16 avPgwDelPdnCb ARGS((
AvPgwPdnCb     *pdnCb
));

PUBLIC S16 avPgwEgtValNStoreModifyBearerCmd ARGS((
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg,  /* Create Session Request message (IN) */
U32            *cause   /* Cause for decoding failure (OUT) */
));

PUBLIC S16 avPgwBldUBReq ARGS((
AvPgwPdnCb       *pdnCb,
EgMsg            **egMsg
));

PUBLIC S16 avPgwValUBRsp ARGS((
AvPgwUeCb      *ueCb,   /* UE control block (INOUT) */
AvPgwPdnCb     *pdnCb,  /* PDN Control Block (INOUT) */
EgMsg          *egMsg   /* Create Session Request message (IN) */
));

PUBLIC S16 avPgwIsCTunExist ARGS((
AvPgwUeCb         *ueCb,
EgMsg             *egMsg
));

PUBLIC S16 avPgwBldMBRsp ARGS((
AvPgwPdnCb        *pdnCb,
EgMsg             **egMsg
));

PUBLIC S16 avPgwEgtQMsg ARGS((
AvPgwUeTunnelCb   *cTun,
U8                eventType,
EgMsg             *egMsg
));

PUBLIC S16 avPgwBldMBRsp ARGS ((
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg
));
PUBLIC AvPgwPktFilterCb* avPgwDelPktFilter ARGS((
AvPgwIpCb        *pIpCb,
U8                pfId 
));

PUBLIC Void AvPgwUtlInetIpAddrToArr ARGS((
CmInetIpAddr   ipAddr,
U8             *pIpAddrArr
));


/**************************************************************************//**
 *
 * @brief
 *    Sets state of tunnel information in route CB
 *
 * @param[in] tunnelCb
 *    Pointer to UEs tunnel CB
 * @param[in]  state
 *    State to be set
 *
 * @return
 *    No return values
 *              
 *****************************************************************************/
PUBLIC Void avPgwSetRouteCbTunState
(
AvPgwUeTunnelCb   *tunnelCb,
AvPgwEgtUState    state
);

/*******************************************************/
/* End of PDN-GW Function declarations                 */
/*******************************************************/

/*******************************************************/
/*               End of PDN-GW                         */
/*******************************************************/

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __AV_PGW_X__ */

/**********************************************************************

         End of file:     av.x@@/main/1 - Tue Aug 17 18:54:32 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -       ad               1. LTE-CNE Initial Release.
/main/1      av001.101 rp             1. Added support for QOS and TFT
/main/1      av004.101 pmacharla      1. multi ue support added
                                      2. upgraded for eGTP Release 9(29.274 9.3.0)
*********************************************************************91*/
