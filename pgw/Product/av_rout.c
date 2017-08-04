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

     Name:    Trillium LTE CNE - PDN GW's Router Module

     Type:    C Source file

     Desc:    This file contains the PDN-GW's routing controller source code

     File:    jg_rout.c

     Sid:      av_rout.c@@/main/1 - Tue Aug 17 18:54:34 2010

     Prg:     sv 
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

#include "lav.h" 
#include "av.h" 

#ifndef AV_PERF
#include <pcap.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

#include "lav.x"
#include "av.x"


/* NOTE: Following values are reverse of actual values for direct
   memcpy and comparision purpose as this software is written for 
   x86 platform (little endian), actual values are in comments */
/* Let us see how best we can make it platform independent */
typedef enum avPgwEthType
{
   AV_PGW_ETH_TYPE_IP  = 0x0800,  /* 0x0008 */
   AV_PGW_ETH_TYPE_ARP = 0x0806  /* 0x0608 */ 
}AvPgwEthType;

typedef enum avPgwARPType
{
   AV_PGW_ARP_REQ     =  0x0001, /* 0x0001 */
   AV_PGW_ARP_RSP     =  0x0002 /* 0x0002 */
}AvPgwARPType;

typedef enum avPgwIPProtType
{
   AV_PGW_IP_ICMP =   0x01, /* 1 */
   AV_PGW_IP_TCP  =   0x06, /* 6 */
   AV_PGW_IP_UDP  =   0x11  /* 17 */
}AvPgwIPProtType;

typedef enum avPgwICMPType
{
   AV_PGW_ICMP_REDIR   =  0x05
}AvPgwICMPType;


#ifndef AV_PERF
#ifndef TEST_SAMPLE_DATA
PRIVATE U16 arp_hardware_type_ethernet =  0x0001; /* 0x0100 */
PRIVATE U8  arp_hardware_size          =  0x06;
PRIVATE U8  arp_protocol_size          =  0x04;
#endif 
#endif 
/* Set of Protocol Numbers 
Registry Name: Assigned Internet Protocol Types Values
Reference: [RFC5237]
Registration Procedures: IESG Approval or Standards Action
Note: 
In the Internet Protocol version 4 (IPv4) [RFC791] there is a field
called "Protocol" to identify the next level protocol. This is an 8 bit field.

In Internet Protocol version 6 (IPv6) [RFC1883], this field is called the
"Next Header" field.

Registry:
Decimal  Keyword          Protocol                                 
-------  ---------------  ---------------------------------------  
0        HOPOPT           IPv6 Hop-by-Hop Option                   
1        ICMP             Internet Control Message                 
2        IGMP             Internet Group Management                
3        GGP              Gateway-to-Gateway                       
4        IP               IP in IP (encapsulation)                 
5        ST               Stream                                   
6        TCP              Transmission Control                     
7        CBT              CBT                                      
8        EGP              Exterior Gateway Protocol                
9        IGP              any private interior gateway             
                          (used by Cisco for their IGRP)  
10       BBN-RCC-MON      BBN RCC Monitoring                       
11       NVP-II           Network Voice Protocol                   
12       PUP              PUP                                      
13       ARGUS            ARGUS                                    
14       EMCON            EMCON                                    
15       XNET             Cross Net Debugger                       
16       CHAOS            Chaos                                    
17       UDP              User Datagram                            
18       MUX              Multiplexing                             
19       DCN-MEAS         DCN Measurement Subsystems               
20       HMP              Host Monitoring                          
21       PRM              Packet Radio Measurement                 
22       XNS-IDP          XEROX NS IDP                             
23       TRUNK-1          Trunk-1                                  
24       TRUNK-2          Trunk-2                                  
25       LEAF-1           Leaf-1                                   
26       LEAF-2           Leaf-2                                   
27       RDP              Reliable Data Protocol                   
28       IRTP             Internet Reliable Transaction            
29       ISO-TP4          ISO Transport Protocol Class 4           
30       NETBLT           Bulk Data Transfer Protocol              
31       MFE-NSP          MFE Network Services Protocol            
32       MERIT-INP        MERIT Internodal Protocol                
33       DCCP             Datagram Congestion Control Protocol     
34       3PC              Third Party Connect Protocol             
35       IDPR             Inter-Domain Policy Routing Protocol     
36       XTP              XTP                                      
37       DDP              Datagram Delivery Protocol               
38       IDPR-CMTP        IDPR Control Message Transport Proto     
39       TP++             TP++ Transport Protocol                  
40       IL               IL Transport Protocol                    
41       IPv6             Ipv6                                     
42       SDRP             Source Demand Routing Protocol           
43       IPv6-Route       Routing Header for IPv6                  
44       IPv6-Frag        Fragment Header for IPv6                 
45       IDRP             Inter-Domain Routing Protocol            
46       RSVP             Reservation Protocol                     
47       GRE              General Routing Encapsulation            
48       DSR              Dynamic Source Routing Protocol          
49       BNA              BNA                                      
50       ESP              Encap Security Payload                   
51       AH               Authentication Header                    
52       I-NLSP           Integrated Net Layer Security  TUBA      
53       SWIPE            IP with Encryption                       
54       NARP             NBMA Address Resolution Protocol         
55       MOBILE           IP Mobility                              
56       TLSP             Transport Layer Security Protocol        
                          using Kryptonet key management
57       SKIP             SKIP                                     
58       IPv6-ICMP        ICMP for IPv6                            
59       IPv6-NoNxt       No Next Header for IPv6                  
60       IPv6-Opts        Destination Options for IPv6             
61                        any host internal protocol               
62       CFTP             CFTP                                     
63                        any local network                        
64       SAT-EXPAK        SATNET and Backroom EXPAK                
65       KRYPTOLAN        Kryptolan                                
66       RVD              MIT Remote Virtual Disk Protocol         
67       IPPC             Internet Pluribus Packet Core            
68                        any distributed file system              
69       SAT-MON          SATNET Monitoring                        
70       VISA             VISA Protocol                            
71       IPCV             Internet Packet Core Utility             
72       CPNX             Computer Protocol Network Executive      
73       CPHB             Computer Protocol Heart Beat             
74       WSN              Wang Span Network                        
75       PVP              Packet Video Protocol                    
76       BR-SAT-MON       Backroom SATNET Monitoring               
77       SUN-ND           SUN ND PROTOCOL-Temporary                
78       WB-MON           WIDEBAND Monitoring                      
79       WB-EXPAK         WIDEBAND EXPAK                           
80       ISO-IP           ISO Internet Protocol                    
81       VMTP             VMTP                                     
82       SECURE-VMTP      SECURE-VMTP                              
83       VINES            VINES                                    
84       TTP              TTP                                      
85       NSFNET-IGP       NSFNET-IGP                               
86       DGP              Dissimilar Gateway Protocol              
87       TCF              TCF                                      
88       EIGRP            EIGRP                                    
89       OSPFIGP          OSPFIGP                                  
90       Sprite-RPC       Sprite RPC Protocol                      
91       LARP             Locus Address Resolution Protocol        
92       MTP              Multicast Transport Protocol             
93       AX.25            AX.25 Frames                             
94       IPIP             IP-within-IP Encapsulation Protocol      
95       MICP             Mobile Internetworking Control Pro.      
96       SCC-SP           Semaphore Communications Sec. Pro.       
97       ETHERIP          Ethernet-within-IP Encapsulation         
98       ENCAP            Encapsulation Header                     
99                        any private encryption scheme            
100      GMTP             GMTP                                     
101      IFMP             Ipsilon Flow Management Protocol         
102      PNNI             PNNI over IP                             
103      PIM              Protocol Independent Multicast           
104      ARIS             ARIS                                     
105      SCPS             SCPS                                     
106      QNX              QNX                                      
107      A/N              Active Networks                          
108      IPComp           IP Payload Compression Protocol          
109      SNP              Sitara Networks Protocol                 
110      Compaq-Peer      Compaq Peer Protocol                     
111      IPX-in-IP        IPX in IP                                
112      VRRP             Virtual Router Redundancy Protocol       
113      PGM              PGM Reliable Transport Protocol          
114                       any 0-hop protocol                       
115      L2TP             Layer Two Tunneling Protocol             
116      DDX              D-II Data Exchange (DDX)                 
117      IATP             Interactive Agent Transfer Protocol      
118      STP              Schedule Transfer Protocol               
119      SRP              SpectraLink Radio Protocol               
120      UTI              UTI                                      
121      SMP              Simple Message Protocol                  
122      SM               SM                                       
123      PTP              Performance Transparency Protocol        
124      ISIS over IPv4                                            
125      FIRE                                                      
126      CRTP             Combat Radio Transport Protocol          
127      CRUDP            Combat Radio User Datagram               
128      SSCOPMCE                                                  
129      IPLT                                                      
130      SPS              Secure Packet Shield                     
131      PIPE             Private IP Encapsulation within IP       
132      SCTP             Stream Control Transmission Protocol     
133      FC               Fibre Channel                            
134      RSVP-E2E-IGNORE                                           
135      Mobility Header                                           
136      UDPLite                                                   
137      MPLS-in-IP                                                
138      manet            MANET Protocols                          
139      HIP              Host Identity Protocol                   
140      Shim6            Shim6 Protocol                           
141-252                   Unassigned                               
253                       Use for experimentation and testing      
254                       Use for experimentation and testing      
255      Reserved                                                  




The port numbers are divided into three ranges: the Well Known Ports,
the Registered Ports, and the Dynamic and/or Private Ports.

The Well Known Ports are those from 0 through 1023.

DCCP Well Known ports SHOULD NOT be used without IANA registration.
The registration procedure is defined in [RFC4340], Section 19.9.

The Registered Ports are those from 1024 through 49151

http://www.iana.org/assignments/port-numbers

*/

PRIVATE  S16 avPgwAddPktFilter ARGS((
AvPgwIpCb         *pIpCb,
AvPgwUeTunnelCb   *pDedTun,
AvPgwEgPktFilter  *pkFilter
));


PRIVATE Void avPgwInitRoutPst ARGS((Void));
PRIVATE S16 avPgwGetGatewayAddr ARGS((Void));

PRIVATE AvPgwUeRouteCb* avPgwFndUeRouteCb ARGS((
CmLListCp      *routeCbLst,
CmInetIpAddr   ipAddr 
));

PRIVATE S16 avPgwGetNwParam ARGS((U32 selfIp));
#ifndef AV_PERF

/* Declaration of pcap_sendpacket() which is not present in pcap.h */
EXTERN int pcap_sendpacket(pcap_t *hdl, CONSTANT U8* buf, int len);

PRIVATE S16 avPgwInitAdaptor ARGS((Void));
PRIVATE S16 avPgwInitEthPkt ARGS((Void));
PRIVATE S16 avPgwInitArpPkt ARGS((Void));
PRIVATE Void avPgwSendGratArpReqPkt ARGS((U8  *ipAddr));

PRIVATE Void avPgwSendEthPkt ARGS((U8  *ethPkt,
                                   U32 len));


PRIVATE Void avPgwSendArpRspPkt ARGS((CONSTANT U8  *arpReqPkt,
                                      U32          len));

#ifdef TODO_TFT
PRIVATE S16 avPgwFndDstBearerOnAddrPort ARGS((
U32               ipAddr,
U16               port,
AvPgwUeRouteCb    *ueRouteCb,
AvPgwUeTunnelCb   **ueTunlCb));

PRIVATE S16 avPgwFndDstBearerOnAddrProtType ARGS((
CONSTANT U8        protoType,
AvPgwUeRouteCb     *ueRouteCb,
AvPgwUeTunnelCb  **ueTunlCb));
#endif

PRIVATE Void avPgwRoutToSgw ARGS((CONSTANT U8  *ipPkt,
                                  U32           len,
                                  CONSTANT U8   protoType));

PRIVATE U8 * avPgwFindMACAddress ARGS((U32 ipAddr));

PRIVATE Void avPgwBuildEthPkt ARGS((U8   *ipPkt, 
                                    U32   len));


PRIVATE U16 avPgwCalcIPChecksum ARGS((U8  *ipPkt, 
                                      U32 ipHdrLen));

PRIVATE Void avPgwFragmentIPPkt ARGS((U8  *ipPkt, 
                                      U32  len));

PRIVATE Void avPgwHndlICMPPkt ARGS((CONSTANT U8  *ipPkt,
                                    U32          bytesRcvd));

PRIVATE Void avPgwRcvdPktHndlr ARGS((u_char                   *args,
                                     const struct pcap_pkthdr *hdr,
                                     const u_char             *pktData));

PRIVATE Void avPgwHndlARPPkt ARGS((CONSTANT U8  *arpPkt,
                                   U32          bytesRcvd));

PRIVATE Void avPgwHndlARPRequest ARGS((CONSTANT U8  *arpPkt,
                                       U32          bytesRcvd));

PRIVATE Void avPgwHndlARPResponse ARGS((CONSTANT U8  *arpPkt,
                                        U32          bytesRcvd));

PRIVATE S16 avPgwIsIpAssigned ARGS((U32      ipAddr));


PRIVATE Void avPgwHndlIPPkt ARGS((CONSTANT U8  *ipPkt,
                                  U32          bytesRcvd));
#endif

PRIVATE S8 AvPgwGetFreePktFilterIndex ARGS((
AvPgwPktFilterCb     *ppPktFilterArr
));

PRIVATE S16 AvPgwGetDstTeidOnTft ARGS((
AvPgwIpCb         *pIpCb,
CONSTANT U8       *pIpPkt,
AvPgwTunInfo      *tunInfo
));

PRIVATE S16 avPgwDelTft ARGS((
AvPgwIpCb    *pIpCb
));



/*
 *
 *       Fun:   avPgwInitRoutPst
 *
 *       Desc:  This function initializes post structure for posting
 *              messages to avActvTsk
 *
 *       Ret:   none
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwInitRoutPst
(
Void
)
#else
PRIVATE Void avPgwInitRoutPst (Void)
#endif
{
   Pst *pst = &avPgwCb.routeCb.pst;

   AV_PGW_SND_TRC2(avPgwInitRoutPst)
  
   pst->srcProcId = SFndProcId();
   pst->srcEnt    = ENTEU; /* Source PDN-GW */
   pst->srcInst   = AV_PGW_SECOND_INSTANCE; /* PDN-GW instance 1 */
   pst->dstProcId = SFndProcId();
   pst->dstEnt    = ENTEG; /* Destination EGTP */
   pst->dstInst   = 0; /* Destination EGTP instance 0 */
   pst->selector  = 2; /* Tight coupled */
   pst->route     = RTESPEC;
   pst->prior     = PRIOR0;
   pst->event     = EVTEGTUDATREQ;
   pst->region    = avPgwCb.routeCb.init.region;
   pst->pool      = avPgwCb.routeCb.init.pool;

   AV_PGW_SND_RETVOID;

}/* End of avPgwInitRoutPst */



/*
 *
 *       Fun:   avPgwGetGatewayAddr
 *
 *       Desc:  This function gets the Gateway IP address
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwGetGatewayAddr
(
Void
)
#else
PRIVATE S16 avPgwGetGatewayAddr (Void)
#endif
{
   struct in_addr addr;
   S8             line[255];
   S8             *ptr = NULLP;
   FILE           *fp = NULLP;
   U8             idx;
   S32            ret;
   S8             ethIntf[8];

   AV_PGW_RCV_TRC2(avPgwGetGatewayAddr)
   
   /* Copy the ethernet interface name upto ":"(in case of virtual interface)
      to ethIntf */
   strncpy(ethIntf, avPgwCb.routeCb.ethIntf, sizeof(ethIntf));
   for (idx = 0; idx < sizeof(ethIntf); idx++)
   {
      if (ethIntf[idx] == ':')
      {
         ethIntf[idx] = '\0';
      }
   }
   
   /* Execute 'netstat -rn' command and redirect output 
      to 'gateway.txt' file temporarily. This file will be deleted after
      extracting the Gateway IP address */
   ret = system(AV_PGW_CMD1);
   if (ret != 0)
   {
#ifdef ALIGN_64BIT
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Failed to execute command:%s, error(%d)\n",
                            AV_PGW_CMD1, ret));
#else
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Failed to execute command:%s, error(%ld)\n",
                            AV_PGW_CMD1, ret));
#endif

      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* Open 'gateway.txt' file in read mode */
   fp = fopen(AV_PGW_FILENAME, "rb");
   if (!fp)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Failed to open file:%s\n", AV_PGW_FILENAME));
      perror("fopen");
      system(AV_PGW_CMD2);
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /*
    Dirty parsing of 'netstat -rn' output, as there is no ioctl command
    to get the Gateway IP address. The following code parses the output
    of following format to get the Gateway IP address. 

Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
172.25.0.0      0.0.0.0         255.255.0.0     U         0 0          0 eth0
169.254.0.0     0.0.0.0         255.255.0.0     U         0 0          0 eth0
0.0.0.0         172.25.0.254    0.0.0.0         UG        0 0          0 eth0
0.0.0.0         172.26.0.254    0.0.0.0         UG        0 0          0 eth1

    NOTE: If there are multiple default gateways for same interface
          first one shall be used.
    */
   while (fgets(line, 255, fp))
   {
      if ((ptr = strstr(line, AV_PGW_DEFAULT)))
      {
         /* See sample output above, the default Gateway entry 
            contains falgs 'UG' and interface name */
         if (strstr(line, "UG") && strstr(line, ethIntf))
            break;
      }
   }

   if (!ptr)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Failed to get Gateway address\n"));
      fclose(fp);
      system(AV_PGW_CMD2);
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* Skip default address */
   ptr = ptr + strlen(AV_PGW_DEFAULT);
   
   /* Skip tabs and(or) spaces */
   idx = 0;
   while (!isdigit(ptr[idx]))
   {
      idx++;
   }
   ptr = ptr + idx;
   
   /* Get the length of the Gateway IP address string */
   idx = 0;
   while (isdigit(ptr[idx]))
   {
      idx++;
      if (ptr[idx] == '.')
         idx++;
   }

   /* Make sure to add null character at the end of 
      Gateway IP address string */
   ptr[idx] = '\0';

   /* Convert the Gateway IP address string to U32 format */
   inet_aton(ptr, &addr);
   avPgwCb.peerCfg.exGwCfg.u.ipv4NetAddr = htonl(addr.s_addr);

   /* Close 'gateway.txt' file pointer and delete the file */
   fclose(fp);
   system(AV_PGW_CMD2);

   AV_PGW_RCV_RETVALUE(ROK);
}/* End of avPgwGetGatewayAddr */


/*
 *
 *       Fun:   avPgwGetNwParam
 *
 *       Desc:  This function gets the Network paramters Ethernet Interface,
 *              MAC address and gateway IP address
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwGetNwParam
(
U32 selfIp
)
#else
PRIVATE S16 avPgwGetNwParam (selfIp)
U32 selfIp;
#endif
{
   struct ifreq       *ifr;
   struct ifconf      ifc;
   struct in_addr     *addr;
   struct sockaddr_in sa;
   S32                sockfd;
   S32                ifclen;
   S16                ret;
   U8                 found = FALSE;

   AV_PGW_RCV_TRC2(avPgwGetNwParam)

   /* Open a socket for ioctl */
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Failed to open socket\n"));
      perror("socket");
      close(sockfd);
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* Get the length of configuration info of all interfaces */
   cmMemset((U8 *)&ifc, 0, sizeof(struct ifconf));
   if (ioctl(sockfd, SIOCGIFCONF, &ifc))
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, 
                           "Failed to get length of config info of all \
                            interfaces\n"));
      perror("ioctl SIOCGIFCONF");
      close(sockfd);
      AV_PGW_RCV_RETVALUE(RFAILED);
   }
   ifclen = ifc.ifc_len;

   /* Get the configuration info of all interfaces */
   cmMemset((U8 *)&ifc, 0, sizeof(struct ifconf));

   AV_PDNGW_RECVTSK_ALLOC(ifc.ifc_req, ifclen);

   if (NULLP == ifc.ifc_req)
   {
#ifdef ALIGN_64BIT
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Failed to allocate memory of size(%d) for ifc.ifc_req\n",
                 ifclen));
#else
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Failed to allocate memory of size(%ld) for ifc.ifc_req\n",
                 ifclen));
#endif
      /*Added Klockworks Check : Adding close(sockfd)*/
      close(sockfd);
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   ifc.ifc_len = ifclen;
   if (ioctl(sockfd, SIOCGIFCONF, &ifc))
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                           "Failed to get config info of all interfaces\n"));

      perror("ioctl SIOCGIFCONF");
      AV_PDNGW_RECVTSK_FREE(ifc.ifc_req, ifclen);

      close(sockfd);
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* Get the ethernet interface name and address matching the selfIp from
      all configured interfaces */
   for (ifr = ifc.ifc_req; 
        (ifr && ((char *)ifr < (char *) ifc.ifc_req + ifc.ifc_len)); ++ifr)
   {
      addr = ((struct in_addr *) &ifr->ifr_addr.sa_data[sizeof(sa.sin_port)]);

      if (htonl(addr->s_addr) == selfIp)
      {
         /* Found the interface matching selfIp, get other details */
         strcpy(avPgwCb.routeCb.ethIntf, ifr->ifr_name);

         /* Get Ethernet (Hardware) Address */
         if (ioctl(sockfd, SIOCGIFHWADDR, ifr)) 
         {
            AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                                  "Failed to get Ethernet address\n"));
            perror("ioctl SIOCGIFHWADDR");

            AV_PDNGW_RECVTSK_FREE(ifc.ifc_req, ifclen);
            close(sockfd);
            AV_PGW_RCV_RETVALUE(RFAILED);
         }
         cmMemcpy(avPgwCb.routeCb.localMACAddr, (U8 *)ifr->ifr_addr.sa_data, 
                  AV_PGW_ROUT_MAC_ADDR_LEN);
         found = TRUE;
         break;
      }
   }

   AV_PDNGW_RECVTSK_FREE(ifc.ifc_req, ifclen);
   close(sockfd);

   if (FALSE == found)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Failed to get network parameteres\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }
   /* We now configure external network node's address */
   /* Check the primary external network node's address*/
   if((CM_INET_IPV4ADDR_TYPE == avPgwCb.peerCfg.exGwCfg.type) &&
      (0 == avPgwCb.peerCfg.exGwCfg.u.ipv4NetAddr))
   { 
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                     "External network details not configured, trying default \
                      Gateway...\n"));
   /* The following is optional to retrieve the default gateway's IP 
      Address */
      ret = avPgwGetGatewayAddr();

      if (ret != ROK)
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                               "Failed to get Gateway IP address\n"));

         AV_PGW_RCV_RETVALUE(RFAILED);
      }
   }
   
   AV_PGW_RCV_RETVALUE(ROK);
}/* End of avPgwGetNwParam */



/*
 *
 *       Fun:   avPgwRouteInit
 *
 *       Desc:  This function initializes the routing related params
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwRouteInit
(
Void
)
#else
PUBLIC S16 avPgwRouteInit (Void)
#endif
{
   AvPgwRouteCb      *routeCb = NULLP;
   AvPgwUeRouteCb    *ueRouteCb;
   CmInetIpAddr      ueIPAddr, selfIP;
   U8                idx, routeIdx = 0, ipIdx = 0;
   U8                primaryMac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   U8                *addr = NULLP;
   S16               ret = ROK;
   AvPgwIpCb         **ipCbsList = NULLP;

   AV_PGW_RCV_TRC2(avPgwRouteInit)

   /* This function is invoked by the PDN-GW sender task as Receive
      task would have been just created */
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
            "Initialising PDN-GW Routing Parameters\n"));

   routeCb = &avPgwCb.routeCb; 

   /* Initialise post structure for posting messages to avSgwActvTsk */
   avPgwInitRoutPst();

   /* Initialize all UE routing control blocks */
   routeCb->maxIpsAvail = avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb;

   /* Start Address */
   ueIPAddr = avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.startAddr.u.ipv4NetAddr;

   ipCbsList = &(avPgwCb.ipCbsList);
   if(ROK != SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)ipCbsList,\
                     sizeof(AvPgwIpCb) * routeCb->maxIpsAvail))
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
               "Failed to allocate  IPCbs List \n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* av004.101 ipCbsList contains the data tunnel Cb assigned */
   cmMemset((U8 *)*ipCbsList, 0, (sizeof(AvPgwIpCb) * routeCb->maxIpsAvail));

   cmLListInit(&(routeCb->ueRouteCbLst));
   for (routeIdx = 0; routeIdx < routeCb->maxIpsAvail; routeIdx++)
   {
      if (ROK != SGetSBuf(DFLT_REGION, DFLT_POOL, (Data **)&(ueRouteCb),\
                        sizeof(AvPgwUeRouteCb)))
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Allocating memory for"
                              " AvPgwUeRouteCb"));
         AV_PGW_RCV_RETVALUE(RFAILED);
      }

      cmMemset((U8 *)ueRouteCb, 0, (sizeof(AvPgwUeRouteCb)));

      ueRouteCb->nwUeAddr.addr.type = CM_INET_IPV4ADDR_TYPE;
      ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr = ueIPAddr;
      ueRouteCb->lnk.node = (PTR)ueRouteCb;

      cmLListAdd2Tail(&(routeCb->ueRouteCbLst), &(ueRouteCb->lnk));
      
      /* IP CB Initialization */
      AV_PGW_IPCB_INDX(ueIPAddr, ipIdx);
      (*ipCbsList)[ipIdx].nwUeAddr.assigned = FALSE;
      (*ipCbsList)[ipIdx].pktFilterCnt = 0;

      for (idx = 0; idx < AV_PGW_MAX_PKT_FILTERS; idx++)
      {
         (*ipCbsList)[ipIdx].uePktFilterCbArr[idx].empty = TRUE;
      }
      
      ueIPAddr++;
   }

   /* Get Network Parameters using self assigned IP Address */
   ret = avPgwGetNwParam(avPgwCb.pgwCfg.pgwAddrCfg.selfAddr.u.\
                        ipv4TptAddr.address);
   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
               "Failed to get PDN-GW network parameters\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   selfIP = avPgwCb.pgwCfg.pgwAddrCfg.selfAddr.u.ipv4TptAddr.address;

   /* Save the U32 local s IP address in byte array form
      to save time in future */
   for (idx = 0; idx < AV_PGW_ROUT_IP_ADDR_LEN; idx++)
   {
      routeCb->localIPAddr[idx] = selfIP >> (8 * (3 - idx)); 
   }

   /* We have one address of external network node
      configured. This will change if redirected during
      run time. */

   routeCb->mapCnt = 1; 

   /* Initialize IP to MAC Mapping Table */
   cmMemset((U8 *)&routeCb->ip2Mac, 0, 
         sizeof(AvPgwRoutIP2MacMap) * (AV_PGW_MAX_NW_NODES));

   /* Save the U32 external node's IP addresses in byte array form
      to save time in future */
   for(routeIdx = 0; routeIdx < routeCb->mapCnt; routeIdx++)
   {
      routeCb->ip2Mac[routeIdx].ipAddr = avPgwCb.peerCfg.exGwCfg.u.ipv4NetAddr;
      for (idx = 0; idx < AV_PGW_ROUT_IP_ADDR_LEN; idx++)
      {
         routeCb->ip2Mac[routeIdx].ipAddrStr[idx] = 
            avPgwCb.peerCfg.exGwCfg.u.ipv4NetAddr >> (8 * (3 - idx));
      }
      addr = routeCb->ip2Mac[routeIdx].ipAddrStr;

      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, 
               "Gateway IP address : %d.%d.%d.%d\n", 
               addr[0], addr[1], addr[2], addr[3]));

#ifdef ALIGN_64BIT
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
               "Gateway IP address in hex : %x\n", \
               routeCb->ip2Mac[routeIdx].ipAddr));
#else
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
               "Gateway IP address in hex : %lx\n", \
               routeCb->ip2Mac[routeIdx].ipAddr));
#endif

      cmMemcpy((U8 *)routeCb->ip2Mac[routeIdx].macAddr, primaryMac, 
            AV_PGW_ROUT_MAC_ADDR_LEN);

   }

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
            "PDN-GW Network Configuration:\n"));

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, 
            "---------------------------\n"));
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
            "Ethernet Interface : %s\n", routeCb->ethIntf));

   addr = routeCb->localMACAddr;
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
            "Ethernet Address : %x:%x:%x:%x:%x:%x\n", addr[0], addr[1],
            addr[2], addr[3], addr[4], addr[5]));

   addr = routeCb->localIPAddr;
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, 
            "Local IP address : %d.%d.%d.%d\n", 
            addr[0], addr[1], addr[2], addr[3]));

#ifdef ALIGN_64BIT
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
            "Local IP address in hex : %x\n", selfIP));
#else
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
            "Local IP address in hex : %lx\n", selfIP));
#endif

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "---------------------------\n"));

   /* Initialize Ethernet packet */
   ret = avPgwInitEthPkt();
   if (ret != ROK)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
               "Failed to initialise Ethernet Packet\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* Initialize ARP packet */
   ret = avPgwInitArpPkt();
   if (ret != ROK)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, 
               "Failed to initialise ARP Packet\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   /* Open the adaptor for capturing packets - LibPcap */
   ret = avPgwInitAdaptor();
   if (ret != ROK)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
               "avPgwInitAdaptor failed\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   AV_PGW_RCV_RETVALUE(ROK);
}/* End of avPgwRouteInit */

/*
 *
 *       Fun:  avPgwAddUeRouteCb 
 *
 *       Desc:  This function adds a new UE route control block
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwAddUeRouteCb
(
AvPgwUeTunnelCb *pDefUTun,
AvPgwUeRouteCb  **newUeRouteCb
)
#else
PUBLIC S16 avPgwAddUeRouteCb(pDefUTun, newUeRouteCb)
AvPgwUeTunnelCb *pDefUTun;
AvPgwUeRouteCb  **newUeRouteCb;
#endif
{
   AvPgwIpCb         *pIpCb                        = NULLP;
   CmLList           *lnk                          = NULLP;
   AvPgwRouteCb      *routeCb                      = NULLP;
   S32               ipIdx                         = 0;
   S16               ret                           = RFAILED;
   U8                addr[AV_PGW_ROUT_IP_ADDR_LEN] = {'\0'};

   AV_PGW_SND_TRC2(avPgwAddUeRouteCb);

   routeCb = &avPgwCb.routeCb;
   *newUeRouteCb = NULLP;

   if (routeCb->ueRouteCbLst.count == 0)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
            "Failed to add new UE route control block, reached max IPs "\
            " configured count %d\n", routeCb->maxIpsAvail));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* Finding ue route cb for static ip allocation */
   if (pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr != 0)
   {
      AV_PGW_IPCB_INDX(pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr, ipIdx);

      if (ipIdx < 0 || ipIdx >= avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb)
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP Address out of allowed"                  " Range"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }

      *newUeRouteCb = avPgwFndUeRouteCb(&(routeCb->ueRouteCbLst),
                                       pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr);
      if (NULLP == *newUeRouteCb)
      {
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Route CB cannot be found"));
         AV_PGW_SND_RETVALUE(RFAILED);
      }
      else
      {
         if (TRUE == (*newUeRouteCb)->nwUeAddr.assigned)
         {
            AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP Address already "
                                 "allocated"));
            AV_PGW_SND_RETVALUE(RFAILED);
         }
         else
         {
            AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, "Static IP Address "
                                 "allocated"));
         }
      }
   }

   /* get ue route cb for dynamic allocation */
   if (NULLP == *newUeRouteCb)
   {
      *newUeRouteCb = (AvPgwUeRouteCb *)CM_LLIST_FIRST_NODE(\
                                       &(routeCb->ueRouteCbLst), lnk);
      if (NULLP == *newUeRouteCb)
      {
#ifdef ALIGN_64BIT
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Route CB Allocation (%d)",
               routeCb->ueRouteCbLst.count));
#else
         AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Route CB Allocation (%ld)",
               routeCb->ueRouteCbLst.count));
#endif
         AV_PGW_SND_RETVALUE(RFAILED);
      }
   }

   (*newUeRouteCb)->nwUeAddr.assigned = TRUE;

   pDefUTun->ueRouteCb = *newUeRouteCb;
   pDefUTun->pdnCb->ipAddr.type = (*newUeRouteCb)->nwUeAddr.addr.type;
   pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr = (*newUeRouteCb)->nwUeAddr.\
         addr.u.ipv4NetAddr;

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
         "Added new UE info:IP addr(%d), selfTeid(%d), remTeid(%d)\n",
         pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr,
         pDefUTun->localTeIdKey, pDefUTun->remoteTeId));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
         "Added new UE info:IP addr(%ld), selfTeid(%ld), remTeid(%ld)\n",
         pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr,
         pDefUTun->localTeIdKey, pDefUTun->remoteTeId));
#endif

   AV_PGW_GET_IPCB(((*newUeRouteCb)->nwUeAddr.addr.u.ipv4NetAddr), pIpCb);
   if (NULLP != pIpCb)
   {
      ret = ROK;

      pIpCb->nwUeAddr = (*newUeRouteCb)->nwUeAddr;
      pIpCb->defTunInfo.remTeid = pDefUTun->remoteTeId;
	   pIpCb->defTunInfo.remAddr.port = pDefUTun->remAddr.u.ipv4TptAddr.port;
	   pIpCb->defTunInfo.remAddr.address = pDefUTun->remAddr.u.\
            ipv4TptAddr.address;
	   pIpCb->defTunInfo.locAddr.port = pDefUTun->lclAddr.u.ipv4TptAddr.port;
	   pIpCb->defTunInfo.locAddr.address = pDefUTun->lclAddr.u.\
            ipv4TptAddr.address;
      pIpCb->defTunInfo.state = AV_PGW_EGTPU_TUNNEL_DOWN;

      AvPgwUtlInetIpAddrToArr((pIpCb->nwUeAddr.addr.u.ipv4NetAddr), addr);
      avPgwSendGratArpReqPkt(addr);

      /* Delete the entry from route cb list */
      cmLListDelFrm(&(routeCb->ueRouteCbLst), &((*newUeRouteCb)->lnk));
   }
   else
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP CB List allocation for "\
            "IP Address [%d]", pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP CB List allocation for "\
            "IP Address [%ld]", pDefUTun->pdnCb->ipAddr.u.ipv4NetAddr));
#endif
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   AV_PGW_SND_RETVALUE(ret);
}

/*
 *
 *       Fun:   avPgwDelUeRouteCb
 *
 *       Desc:  This function deletes an existing UE route control block
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwDelUeRouteCb
(
AvPgwUeTunnelCb *dataTunnel
)
#else
PUBLIC S16 avPgwDelUeRouteCb(dataTunnel)
AvPgwUeTunnelCb *dataTunnel;
#endif
{
   AvPgwRouteCb         *routeCb = &avPgwCb.routeCb;
   AvPgwUeRouteCb       *ueRouteCb = NULLP;
   S32                  ipIdx = 0;

   AV_PGW_SND_TRC2(avPgwDelUeRouteCb);

   if (dataTunnel->ueRouteCb == NULLP)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Delete UE Route CB"));
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   ueRouteCb  = dataTunnel->ueRouteCb;
   AV_PGW_IPCB_INDX(ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr, ipIdx);
   if (ipIdx < 0 || ipIdx >= avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Invalid IP (%d) in "
                        "UE Route CB", ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Invalid IP (%ld) in "
                        "UE Route CB", ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr));
#endif
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   /* Free the TFT */
   avPgwDelTft(&(avPgwCb.ipCbsList[ipIdx]));

   /* in ipCbs List set the routeCb pointer to NULL */
   ueRouteCb->nwUeAddr.assigned = FALSE;
   avPgwCb.ipCbsList[ipIdx].nwUeAddr.assigned = FALSE;

   ueRouteCb->lnk.node = (PTR)ueRouteCb;
   cmLListAdd2Tail(&(routeCb->ueRouteCbLst), &(ueRouteCb->lnk));

   AV_PGW_SND_RETVALUE(ROK);
}

#ifdef TODO_TFT
#ifdef ANSI
PRIVATE S16 avPgwFndDstBearerOnAddrPort
(
U32               ipAddr,
U16               port,
AvPgwUeRouteCb    *ueRouteCb,
AvPgwUeTunnelCb   **ueTunlCb
)
#else
PRIVATE S16 avPgwFndDstBearerOnAddrPort(ipAddr, port, ueRouteCb, ueTunlCb)
U32               ipAddr;
U16               port;
AvPgwUeRouteCb    *ueRouteCb;
AvPgwUeTunnelCb   **ueTunlCb;
#endif
{
   U16               portRangeLow = 0;
   U16               portRangeHigh = 0;
   S16               ret = RFAILED;
   AvPgwPktFilterCb  *pktFilterCb = NULLP;
   CmLList           *lnk = NULLP;

   AV_PGW_RCV_TRC2(avPgwFndDstBearerOnAddrPort)

   /* Next match on packet filter context */
   /* Destination/remote port + Port Range which we serve to UEs */
   /* Remote Port and Remote Port range is a packet component within
    * a packet filter context */

   /* Loop upto existing packet filters assigned to this UE */
   AV_PGW_FOR_EACH_LST_NODE(AvPgwPktFilterCb, &(ueRouteCb->uePktFilterCbLst),
                           pktFilterCb, lnk)
   {
      /* av004.101 : ccpu00118425 need to correct the TFT */
      /* Check on the remote port range component match */
      portRangeLow = GetLoWord(pktFilterCb->pFilterPort);
      portRangeHigh = GetHiWord(pktFilterCb->pFilterPort);

      if(port >= portRangeLow && port<= portRangeHigh)
      {
         /* Port is in our service for this destination IP Address */
         /* Right retrieve the mapped bearer/tunnel */
         *ueTunlCb = pktFilterCb->ueTunnel;
         if(pktFilterCb->ueTunnel != NULLP)
         {
            ret = ROK;
            break;
         }
      }

      /* Match on lowest evaluation precedence index is must too */
      /* Match on rest of the packet filter components */

      /* This phase we do not consider any other components as a matching
         criterion and evaluation precedence index too */ 
   } /* End of Packet Filter Cb Loop */

   AV_PGW_RCV_RETVALUE(ret);

}/* End of avPgwFndDstBearerOnAddrPort */
#endif

#ifdef TODO_TFT
#ifdef ANSI
PRIVATE S16 avPgwFndDstBearerOnAddrProtType
(
CONSTANT U8        protoType,
AvPgwUeRouteCb     *ueRouteCb,
AvPgwUeTunnelCb    **ueTunlCb
)
#else
PRIVATE S16 avPgwFndDstBearerOnAddrProtType(protoType, ueRouteCb, ueTunlCb)
CONSTANT U8        protoType;
AvPgwUeRouteCb     *ueRouteCb;
AvPgwUeTunnelCb    **ueTunlCb;
#endif
{
   S16                  ret = RFAILED;
   AvPgwPktFilterCb     *pfCb = NULLP;
   CmLList              *lnkPf = NULLP;

   AV_PGW_RCV_TRC2(avPgwFndDstBearerOnAddrProtType)

   /* Loop upto existing packet filters assigned to this UE */
   AV_PGW_FOR_EACH_LST_NODE(AvPgwPktFilterCb,
                           &(ueRouteCb->uePktFilterCbLst), pfCb, lnkPf)
   {
      /* Match on lowest evaluation precedence index is must too */
      /* But in this phase we consider other factor components 
       * which can help find the bearer */
      /* Match on rest of the packet filter components */
      /* Try matching based on Protocol Type for IPV4 Next Header Type */
      if ((GTP_TFT_PROTO_ID_PRESENT(pfCb->pfCompPres)) && 
          (protoType == pfCb->pfComp.proto_id)) 
      {      
         /* Protocol is in our service for this destination IP Address */
         /* Right retrieve the mapped bearer/tunnel */
         *ueTunlCb = pfCb->ueTunnel;
         ret = ROK;
         break;
      }
      
   } /* End of Packet Filter Cb Loop */

   AV_PGW_RCV_RETVALUE(ret);

}/* End of avPgwFndDstBearerOnAddrProtType */
#endif



/*
 *
 *       Fun:   avPgwIsIpAssigned
 *
 *       Desc:  This function finds an UE associated data bearer based on IP Address
 *              assigned to UE
 *
 *       Ret:   
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */

#ifdef ANSI
PRIVATE S16 avPgwIsIpAssigned
(
U32              ipAddr
)
#else
PRIVATE S16 avPgwIsIpAssigned(ipAddr)
U32              ipAddr;
#endif
{
   S32            ipIdx = 0;
   AvPgwIpCb      *ipCbsList = NULLP;

   AV_PGW_RCV_TRC2(avPgwIsIpAssigned)

   ipCbsList = avPgwCb.ipCbsList;
   /* av004.101 find the route cb from ip cb */
   AV_PGW_IPCB_INDX(ipAddr, ipIdx);
   if(( ipIdx < 0 || ipIdx  >= avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb
            || FALSE == ipCbsList[ipIdx].nwUeAddr.assigned))
   {
      AV_PGW_RCV_RETVALUE(RFAILED);
   }

   AV_PGW_RCV_RETVALUE(ROK);

}


/*
 *
 *       Fun:   avPgwModUeRouteCb
 *
 *       Desc:  This function initializes a new UE route control block
 *              This invoked only for dedicated user plan tunnels
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC  S16 avPgwModUeRouteCb
(
AvPgwUeTunnelCb *pUTun
)
#else
PUBLIC S16 avPgwModUeRouteCb(pUTun, ueRouteCb)
AvPgwUeTunnelCb *pUTun;
#endif
{
   S16                  ret = ROK;
   AvPgwIpCb            *pIpCb = NULLP;

   AV_PGW_SND_TRC2(avPgwModUeRouteCb)

   AV_PGW_GET_IPCB(pUTun->pdnCb->ipAddr.u.ipv4NetAddr, pIpCb);
   if (NULLP == pIpCb)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP CB cannot be found for "
            "IP (%d) ", pUTun->pdnCb->ipAddr.u.ipv4NetAddr));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP CB cannot be found for "
            "IP (%ld) ", pUTun->pdnCb->ipAddr.u.ipv4NetAddr));
#endif
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   pUTun->remoteTeId = pUTun->newRemTeid;
   pUTun->remAddr = pUTun->newRemAddr;

   pIpCb->defTunInfo.remTeid = pUTun->remoteTeId;
	pIpCb->defTunInfo.remAddr.port = pUTun->remAddr.u.ipv4TptAddr.port;
	pIpCb->defTunInfo.remAddr.address = pUTun->remAddr.u.ipv4TptAddr.address;
      
   AV_PGW_SND_RETVALUE(ret);
}


/*
 *
 *       Fun:   avPgwInitUeRouteCb
 *
 *       Desc:  This function initializes a new UE route control block
 *              This invoked only for dedicated user plan tunnels
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC  S16 avPgwInitUeRouteCb
(
AvPgwUeTunnelCb *pUTun,
AvPgwUeRouteCb  *ueRouteCb
)
#else
PUBLIC S16 avPgwInitUeRouteCb(pUTun, ueRouteCb)
AvPgwUeTunnelCb *pUTun;
AvPgwUeRouteCb  *ueRouteCb;
#endif
{
   U16                  pfIndx = 0;
   S16                  ret = ROK;
   AvPgwIpCb            *pIpCb = NULLP;

   AV_PGW_SND_TRC2(avPgwInitUeRouteCb)

   AV_PGW_GET_IPCB(pUTun->pdnCb->ipAddr.u.ipv4NetAddr, pIpCb);
   if (NULLP == pIpCb)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP CB cannot be found for "
            "IP (%d) ", pUTun->pdnCb->ipAddr.u.ipv4NetAddr));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "IP CB cannot be found for "
            "IP (%ld) ", pUTun->pdnCb->ipAddr.u.ipv4NetAddr));
#endif
      AV_PGW_SND_RETVALUE(RFAILED);
   }

   ueRouteCb->nwUeAddr = pIpCb->nwUeAddr;

   /* Packet Filters to this UE */
   /* av001.101: only in case of default tunnel */
   if (pUTun->tunnelType == AV_PGW_DATA_TUNNEL_DED)
   {
      /* Currently we are taking the values as port range. But Ideally we need 
       * to consider all type of possibilities such as single port, ipv6 and
       * other option */
      for (pfIndx = 0; pfIndx < pUTun->tft.pktFilterCnt; pfIndx++)
      {
         switch (pUTun->tft.opCode)
         {
            case AV_PGW_TFT_OPCODE_NEWTFT:
            case AV_PGW_TFT_OPCODE_ADD_FILTER:
            {
               avPgwAddPktFilter(pIpCb, pUTun,\
                     pUTun->tft.pktFilters[pfIndx]);
               break;
            }

            case AV_PGW_TFT_OPCODE_DEL_FILTER:
            {
               avPgwDelPktFilter(pIpCb, pUTun->tft.pktFilters[pfIndx]->pfId);
               break;
            }

            case AV_PGW_TFT_OPCODE_DELTFT:
            {
               avPgwDelTft(pIpCb);
               break;
            }

            case AV_PGW_TFT_OPCODE_NOTFTOP:
            {
               break;
            }

            default:
            {
               ret = RFAILED;
               break;
            }
         }

         if (RFAILED == ret)
         {
            break;
         }
      } 
   }
      
   AV_PGW_SND_RETVALUE(ret);
}



/*
 *
 *       Fun:   avPgwAddPktFilter
 *
 *       Desc:  Adds packet filter into ueRouteCb
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE  S16 avPgwAddPktFilter
(
AvPgwIpCb         *pIpCb,
AvPgwUeTunnelCb   *pDedTun,
AvPgwEgPktFilter  *pkFilter
)
#else
PRIVATE S16 avPgwAddPktFilter(pIpCb, pDedTun, pkFilter)
AvPgwIpCb         *pIpCb;
AvPgwUeTunnelCb   *pDedTun;
AvPgwEgPktFilter  *pkFilter;
#endif
{
   S16               ret = ROK;
   AvPgwPktFilterCb  pfCb = {0};
   U8                ind = 0;
   AvPgwPktFilterCb  *pPfCbPrev = NULLP;
   AvPgwPktFilterCb  *pPfCbCurr = NULLP;

   AV_PGW_SND_TRC2(avPgwAddPktFilter)

   if (pIpCb->pktFilterCnt == AV_PGW_MAX_PKT_FILTERS)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Maximum number of packet"
            "Filters allocated"));
      ret = RFAILED;
   }

   pfCb.pNext = NULLP;
   pfCb.pFId = AV_PGW_PKT_FILTER_ID(pDedTun->bearerId, pIpCb->pktFilterCnt);
   pfCb.pFDirection = pkFilter->pktFilterDir; 
   pfCb.evalPrecedIndex = pkFilter->precedence;
   pfCb.pfCompPres = pkFilter->presence_mask;
   cmMemcpy((U8 *)&(pfCb.pfComp), (U8 *)&(pkFilter->component),
            sizeof(AvPgwPktFilterComp));
   pfCb.tunInfo.remTeid = pDedTun->remoteTeId;
	pfCb.tunInfo.remAddr.port = pDedTun->remAddr.u.ipv4TptAddr.port;
	pfCb.tunInfo.remAddr.address = pDedTun->remAddr.u.\
            ipv4TptAddr.address;
	pfCb.tunInfo.locAddr.port = pDedTun->lclAddr.u.ipv4TptAddr.port;
	pfCb.tunInfo.locAddr.address = pDedTun->lclAddr.u.\
            ipv4TptAddr.address;
   /*pfCb.tunInfo.state = AV_PGW_EGTPU_TUNNEL_UP;*/ /*TODO Need to update the state*/


   if (pIpCb->pktFilterCnt == 0)
   {
      pIpCb->pHead = pIpCb->uePktFilterCbArr; 
      pIpCb->uePktFilterCbArr[0] = pfCb;
      pIpCb->pktFilterCnt += 1;
      avPgwSetRouteCbTunState(pDedTun, AV_PGW_EGTPU_TUNNEL_PENDING);

      AV_PGW_SND_RETVALUE(ROK);
   }

   /* Find the position where Packet Filter CB needs to be insered */
   pPfCbCurr = pIpCb->pHead;
   while ((NULLP != pPfCbCurr) && \
         (pfCb.evalPrecedIndex <= pPfCbCurr->evalPrecedIndex))
   {
      pPfCbPrev = pPfCbCurr;
      pPfCbCurr = pPfCbCurr->pNext;
   }

   ind = AvPgwGetFreePktFilterIndex(pIpCb->uePktFilterCbArr);

   if((ind < 0) || (ind >= AV_PGW_MAX_PKT_FILTERS ))
   {
      AV_PGW_SND_RETVALUE(RFAILED);
   }   

   /* copy the entire packet filter CB */
   pIpCb->uePktFilterCbArr[ind] = pfCb;
   pIpCb->uePktFilterCbArr[ind].empty = FALSE;

   /* Rearrange the List pointers
    * Prev ptr can be null if the new filter CB has he highest precedence
    * Current ptr can be null if we are adding into the end of list
    */
   if (NULLP != pPfCbPrev)
   {
      pPfCbPrev->pNext = pIpCb->uePktFilterCbArr + ind;
   }
   else
   {
      /* The new packet filter has the highest precedence. So make it head */
      pIpCb->pHead = pIpCb->uePktFilterCbArr + ind;
   }

   if (NULLP != pPfCbCurr)
   {
      pIpCb->uePktFilterCbArr[ind].pNext = pPfCbCurr;   
   }

   pIpCb->pktFilterCnt += 1;

   avPgwSetRouteCbTunState(pDedTun, AV_PGW_EGTPU_TUNNEL_PENDING);
   AV_PGW_SND_RETVALUE(ret);
}



/**************************************************************************//**
 * @brief:
 *    Fetches a free index in Packet filter CB array where a packet
 *    filter can be inserted
 *
 * @param   ppPktFilterArr
 *    Pointer to packet filter cb array
 *
 * @return
 *    Inddex in packter filter Cb array
 *
 *****************************************************************************/
PRIVATE S8 AvPgwGetFreePktFilterIndex
(
AvPgwPktFilterCb     *ppPktFilterArr
)
{
   S16      ind = 0;

   AV_PGW_SND_TRC2(AvPgwGetFreePktFilterIndex);

   for (ind = 0; ind < AV_PGW_MAX_PKT_FILTERS; ind++)
   {
      if (TRUE == ppPktFilterArr[ind].empty)
         break;
   }

   AV_PGW_SND_RETVALUE(ind);
}



/**************************************************************************//**
 * @brief
 *    Deletes packet filter from pIpCb with a specific Packet filter ID
 *
 * @param[in]  pIpCb
 *    IP Cb pointer
 * @param[in]  pfId
 *    Packet filter ID
 *
 * @return
 *    Returns the pointer to next packet filter after deletion
 *    Returns NULL if the deleted node is the last one
 *
 *****************************************************************************/
PUBLIC AvPgwPktFilterCb* avPgwDelPktFilter
(
AvPgwIpCb      *pIpCb,
U8             pfId 
)
{
   AvPgwPktFilterCb     *pPfCbCurr = NULLP;
   AvPgwPktFilterCb     *pPfCbPrev = NULLP;
   AvPgwPktFilterCb     *retPtr = NULLP;

   AV_PGW_SND_TRC2(avPgwDelPktFilter)

   pPfCbCurr = pIpCb->pHead;
   while (NULLP != pPfCbCurr && pPfCbCurr->pFId != pfId)
   {
      pPfCbPrev = pPfCbCurr;
      pPfCbCurr = pPfCbCurr->pNext;
   }

   if (NULLP == pPfCbCurr)
   {
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, "Finding Packet filter with"\
            " ID [%d]", pfId));
   }
   else
   {
      if (NULLP == pPfCbPrev)
      {
         pIpCb->pHead = pPfCbCurr->pNext;
      }
      else
      {
         pPfCbPrev->pNext = pPfCbCurr->pNext;

      }

      retPtr = pPfCbCurr->pNext;
      pPfCbCurr->empty = TRUE;
      pIpCb->pktFilterCnt -= 1;
   }

   AV_PGW_SND_RETVALUE(retPtr);
}



/*
 *
 *       Fun:   avPgwDelTft
 *
 *       Desc:  Delets TFT
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwDelTft
(
AvPgwIpCb    *pIpCb
)
#else
PRIVATE S16 avPgwDelTft(pIpCb)
AvPgwIpCb    *pIpCb;
#endif
{
   S16                  ret = ROK;
   AvPgwPktFilterCb     *pPfCbCurr = NULLP;
   AvPgwPktFilterCb     *pPfCbNext = NULLP;


   AV_PGW_SND_TRC2(avPgwDelTft)

   pPfCbCurr = pIpCb->pHead;
   while (NULLP != pPfCbCurr) 
   {
      pPfCbNext = pPfCbCurr->pNext;
      
      pPfCbCurr->empty = TRUE;
      pPfCbCurr->pNext = NULLP;

      pPfCbCurr = pPfCbNext;
   }

   pIpCb->pHead = NULLP;
   pIpCb->pktFilterCnt = 0;

   AV_PGW_SND_RETVALUE(ret);
}

#ifndef AV_PERF

/*
 *
 *       Fun:   avPgwInitEthPkt
 *
 *       Desc:  This function initializes the Ethernet Packet
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwInitEthPkt
(
Void
)
#else
PRIVATE S16 avPgwInitEthPkt (Void)
#endif
{
   /* av003.101: Converting in to network bit order. */
   U16 ethType = htons(AV_PGW_ETH_TYPE_IP);
   AvPgwRouteCb *routeCb = &avPgwCb.routeCb;

   AV_PGW_SND_TRC2(avPgwInitEthPkt)

   /* Set the destination MAC address to broadcast address (all 0xff) */
   cmMemset(routeCb->ethPkt, 0xff, AV_PGW_ROUT_MAC_ADDR_LEN);

   /* Set the source MAC address to local MAC address */
   cmMemcpy(routeCb->ethPkt + AV_PGW_ROUT_MAC_ADDR_LEN, 
            routeCb->localMACAddr, AV_PGW_ROUT_MAC_ADDR_LEN);

   /* Set the type of Ethernet packet to IP packet (0x0800) */
   cmMemcpy(routeCb->ethPkt + (AV_PGW_ROUT_MAC_ADDR_LEN * 2), 
            (U8 *)&ethType, 2);

   AV_PGW_SND_RETVALUE(ROK);

}/* End of avPgwInitEthPkt */

/*
 *
 *       Fun:   avPgwInitArpPkt
 *
 *       Desc:  This function initializes the Ethernet Packet
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwInitArpPkt
(
Void
)
#else
PRIVATE S16 avPgwInitArpPkt (Void)
#endif
{
   /* av003.101:Converting in to network bit order.  */
   U16 ethType = htons(AV_PGW_ETH_TYPE_ARP);
   U16 arpHwType = htons(arp_hardware_type_ethernet);
   U16 ethProtocolType = htons(AV_PGW_ETH_TYPE_IP);
   U16 arpType = htons(AV_PGW_ARP_REQ);
   AvPgwRouteCb *routeCb = &avPgwCb.routeCb;
   U8 idx;

   AV_PGW_SND_TRC2(avPgwInitArpPkt)

   /* First Fill Ethenet Header */
   /* Set the destination MAC address to broadcast address and 
      set the source MAC address to local MAC address */
   cmMemcpy(routeCb->arpPkt, routeCb->ethPkt, AV_PGW_ROUT_MAC_ADDR_LEN * 2);
   idx = AV_PGW_ROUT_MAC_ADDR_LEN * 2;

   /* Set the type of Ethernet packet to ARP (0x0806) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&ethType, 2);

   /* Fill ARP packet (Ethernet payload) */
   idx += 2;
   /* Set the hardware type to Ethernet (0x0001) */
   /* av003.101: Converting in to network bit order.  */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&arpHwType, 2);
   idx += 2;

   /* Set the ethernet protocol type to IP (0x0800) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&ethProtocolType, 2);
   idx += 2;

   /* Set hardware size to 6 for Ethernet */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&arp_hardware_size, 1);
   idx += 1;

   /* Set protocol size to 4 for IP */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&arp_protocol_size, 1);
   idx += 1;

   /* Set operation to ARP request (0x0001) */
   cmMemcpy(routeCb->arpPkt + idx, (U8 *)&arpType, 2);
   idx += 2;

   /* Set the Sender Hardware Address to local MAC address */
   cmMemcpy(routeCb->arpPkt + idx, routeCb->localMACAddr,
            AV_PGW_ROUT_MAC_ADDR_LEN);

   idx += AV_PGW_ROUT_MAC_ADDR_LEN;

   /* Set the Sender Protocol Address to local IP address */
   cmMemcpy(routeCb->arpPkt + idx, routeCb->localIPAddr, 
            AV_PGW_ROUT_IP_ADDR_LEN);

   idx += AV_PGW_ROUT_IP_ADDR_LEN;

   /* Initialise the Target Hardware Address and 
      Target Protocol Adress to 0xff */
   cmMemset(routeCb->arpPkt + idx, 0xff, 
            AV_PGW_ROUT_MAC_ADDR_LEN + AV_PGW_ROUT_IP_ADDR_LEN);

   AV_PGW_SND_RETVALUE(ROK);

}/* End of avPgwInitArpPkt */

/*
 *
 *       Fun:   avPgwSendEthPkt
 *
 *       Desc:  This function sends out Ethernet packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwSendEthPkt
(
U8    *ethPkt,
U32    len
)
#else
PRIVATE Void avPgwSendEthPkt (ethPkt, len)
U8    *ethPkt;
U32    len;
#endif
{
   S32 ret;

   AV_PGW_SND_TRC2(avPgwSendEthPkt)

   /* Send out Ethernet packet using PCAP library function */
   ret = pcap_sendpacket(avPgwCb.routeCb.pcapHndl, ethPkt, (S32)len);
   if (0 != ret)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, 
                      "Failed to send Ethernet packet through the \
                       interface - %d\n", ret));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF, 
                      "Failed to send Ethernet packet through the \
                       interface - %ld\n", ret));
#endif
   }

   AV_PGW_SND_RETVOID;
}/* End of avPgwSendEthPkt */

/*
 *
 *       Fun:   avPgwSendArpReqPkt
 *
 *       Desc:  This function sends the ARP request packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwSendArpReqPkt
(
U8  *ipAddr
)
#else
PUBLIC S16 avPgwSendArpReqPkt (ipAddr)
U8  *ipAddr;
#endif
{
   U16 idx = 38; /* Index of ARP packet at which destination IP address
                    has to be added */

   AV_PGW_SND_TRC2(avPgwSendArpReqPkt)

   /* Take the default ARP packet and add the destination IP address */
   cmMemcpy(avPgwCb.routeCb.arpPkt + 38, ipAddr, AV_PGW_ROUT_IP_ADDR_LEN);

   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, 
              "Sending ARP request packet for IP address:%d.%d.%d.%d\n",
              ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]));

   /* Send out ARP request packet */
   avPgwSendEthPkt(avPgwCb.routeCb.arpPkt, idx + 4); 

   AV_PGW_SND_RETVALUE(ROK);

}/* End of avPgwSendArpReqPkt */

/*
 *
 *       Fun:   avPgwSendGratArpReqPkt
 *
 *       Desc:  This function sends the Gratuitous ARP request packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwSendGratArpReqPkt
(
U8  *ipAddr
)
#else
PRIVATE Void avPgwSendGratArpReqPkt (ipAddr)
U8  *ipAddr;
#endif
{
   U8  gratArpPkt[AV_PGW_ROUT_MAX_ARP_PKT_LEN]; /* Gratuitous ARP */
   U16 idx = 38; /* Index of ARP packet at which destination IP address
                    has to be added */

   AV_PGW_RCV_TRC2(avPgwSendGratArpReqPkt)

   /* Copy ARP request packet to gratuitous ARP request packet */
   cmMemcpy(gratArpPkt, avPgwCb.routeCb.arpPkt, AV_PGW_ROUT_MAX_ARP_PKT_LEN);

   /* Gratuitous ARP request packet should have same source and destination
      protocol address. Also destination MAC address should be broadcast
      address */
   cmMemcpy(gratArpPkt + 28, ipAddr, AV_PGW_ROUT_IP_ADDR_LEN);
   cmMemcpy(gratArpPkt + 38, ipAddr, AV_PGW_ROUT_IP_ADDR_LEN);

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, 
              "Sending Gratuitous ARP request for IP address:%d.%d.%d.%d\n",
               ipAddr[0], ipAddr[1], ipAddr[2], ipAddr[3]));

   /* Send out ARP request packet */
   avPgwSendEthPkt(gratArpPkt, idx + 4); 

   AV_PGW_RCV_RETVOID;
}/* End of avPgwSendGratArpReqPkt */

/*
 *
 *       Fun:   avPgwSendArpRspPkt
 *
 *       Desc:  This function sends the ARP response packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwSendArpRspPkt
(
CONSTANT U8  *arpReqPkt,
U32          len
)
#else
PRIVATE Void avPgwSendArpRspPkt (arpReqPkt, len)
CONSTANT U8  *arpReqPkt;
U32          len;
#endif
{
   U8 idx = 0;
   U8 arpRspPkt[AV_PGW_ROUT_MIN_ETH_PKT_SIZE]; /* ARP Response packet */
   U16 ethType = htons(AV_PGW_ETH_TYPE_ARP);
   U16 arpType = htons(AV_PGW_ARP_RSP);

   AV_PGW_RCV_TRC2(avPgwSendArpRspPkt)

   /* Fill Ethernet header */
   /* Collect sender MAC address from ARP request packet and 
      add it as destination MAC address in the Ethernet header */
   cmMemcpy(arpRspPkt, arpReqPkt + 8, AV_PGW_ROUT_MAC_ADDR_LEN);
   idx += AV_PGW_ROUT_MAC_ADDR_LEN;

   /* Add local MAC address as source MAC address */
   cmMemcpy(arpRspPkt + idx, avPgwCb.routeCb.localMACAddr, 
            AV_PGW_ROUT_MAC_ADDR_LEN);

   idx += AV_PGW_ROUT_MAC_ADDR_LEN;

   /* Set type of Ethernet packet to ARP packet (0x0806) */
   cmMemcpy(arpRspPkt + idx, (U8 *)&ethType, 2);
   idx += 2;

   /* Construct ARP response packet (Ethernet payload) */
   /* First 6 bytes are same for ARP request and response, hence
      copy from ARP request packet */
   cmMemcpy(arpRspPkt + idx, arpReqPkt, 6);
   idx += 6;

   /* Fill the ARP response code */
   cmMemcpy(arpRspPkt + idx, (U8 *)&arpType, 2);
   idx += 2;

   /* Fill local MAC address as the sender MAC address */
   cmMemcpy(arpRspPkt + idx, avPgwCb.routeCb.localMACAddr,
            AV_PGW_ROUT_MAC_ADDR_LEN);

   idx += AV_PGW_ROUT_MAC_ADDR_LEN;

   /* Collect target IP address from ARP request packet and 
      add it as sender IP address */
   /* NOTE: We should not add local IP address as sender IP address because
      we are only responding to an ARP request directed at an IP address
      allocated to the UE */
   cmMemcpy(arpRspPkt + idx, arpReqPkt + 24, AV_PGW_ROUT_IP_ADDR_LEN);
   idx += AV_PGW_ROUT_IP_ADDR_LEN;

   /* Collect sender IP address and MAC address from ARP request packet 
      and add them as target IP and MAC addresses */
   cmMemcpy(arpRspPkt + idx, arpReqPkt + 8, 10);
   idx += 10;

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, 
              "Sending ARP response packet to IP:%d.%d.%d.%d\n",
              arpRspPkt[38], arpRspPkt[39], arpRspPkt[40], arpRspPkt[41]));

   /* Send out ARP response packet */
   avPgwSendEthPkt(arpRspPkt, idx);

   AV_PGW_RCV_RETVOID;
}/* End of avPgwSendArpRspPkt */

/*
 *
 *       Fun:   avPgwRoutToSgw
 *
 *       Desc:  This function sends the IP packet to S-GW through GTP-U
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwRoutToSgw
(
CONSTANT U8  *ipPkt,
U32           len,
CONSTANT U8   protoType         
)
#else
PRIVATE Void avPgwRoutToSgw (ipPkt, len, protoType)
CONSTANT U8  *ipPkt;
U32           len;
CONSTANT U8   protoType; 
#endif
{
   Buffer            *mBuf = NULLP;
   EgtUEvnt          *datEvnt = NULLP;
   U16               dstPort = 0; 
   S16               ret = ROK;
   U32               dstIPAddr;
   S32               ipIdx = 0;
	AvPgwTunInfo		tunInfo = {0};

   AV_PGW_RCV_TRC2(avPgwRoutToSgw)

   /* Find GTP-U tunnel end point details based on the destination IP address */
   dstIPAddr = (ipPkt[16] << 24) + (ipPkt[17] << 16) +
               (ipPkt[18] << 8 ) + ipPkt[19];
   
   AV_PGW_IPCB_INDX(dstIPAddr, ipIdx);
   if (ipIdx < 0 || ipIdx >= avPgwCb.pgwCfg.pgwAddrCfg.pgwNetAddr.nmb)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Can not find destination UE tunnel mapping to the \
                 IP address:%d.%d.%d.%d\n",
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      /* Failed to find the destination UE tunnel mapping to the dstIPAddr */
      AV_PGW_RCV_RETVOID;
   }
   
   if (FALSE == avPgwCb.ipCbsList[ipIdx].nwUeAddr.assigned)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Can not find destination UE tunnel mapping to the \
                 IP address:%d.%d.%d.%d\n",
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      AV_PGW_RCV_RETVOID;
   }

   ret = AvPgwGetDstTeidOnTft(&(avPgwCb.ipCbsList[ipIdx]), ipPkt, \
         &tunInfo);
   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                "Can not find destination UE tunnel mapping to the \
                 IP address:%d.%d.%d.%d\n",
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      AV_PGW_RCV_RETVOID;
   }

   if (tunInfo.state != AV_PGW_EGTPU_TUNNEL_UP)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Tunnel state is not UP"));
      AV_PGW_RCV_RETVOID;
   }

   /* Packet is good, construct EgtUEvnt to tunnel through GTP */
   /* Allocate Buffer for posting to S-GW through dummy EGTP-U */
   ret = SGetMsg(avPgwCb.routeCb.init.region, avPgwCb.routeCb.init.pool, &mBuf);
   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, 
                            "Failed to allocate mBuf - %d\n", ret));
      AV_PGW_RCV_RETVOID;
   }

   /* Add the received IP Pkt buffer to to mBuf */
   SAddPstMsgMult((Data *)ipPkt, (MsgLen)len, mBuf);

   /* Now build EGTP-U Data Event Structure */
   ret = avPgwUtlBldEgtDatReq(&tunInfo, &datEvnt); 
   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, 
                "Cannot build EGTP-U Event structure for received data from\
                 IP address:%d.%d.%d.%d\n",
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      /* Failed to construct event structure to the dstIPAddr */
      /* Just drop the packet */
      SPutMsg(mBuf); 
      AV_PGW_RCV_RETVOID;
   }

   /* Assign the received buffer to EGTP-U map */  
   datEvnt->u.egMsg->u.mBuf = mBuf;
   
   /* We should be adding debug print here for success path. However for data 
      path should avoid debug prints. Required only for debugging. */
#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
         "PDN-GW: Sending packet to UE IP address through S-GW :%d.%d.%d.%d, \
         Port(%d) on Tunnel(%d)\n",
		   ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19],
         dstPort, tunInfo.remTeid));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
         "PDN-GW: Sending packet to UE IP address through S-GW :%d.%d.%d.%d, \
         Port(%d) on Tunnel(%ld)\n",
		   ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19],
         dstPort, tunInfo.remTeid));
#endif

   /* Post the message to send to S-GW through GTP-U */
   ret = EuLiEgtEguDatReq(&(avPgwCb.routeCb.pst), avPgwCb.egtUSapCb->spId,
                        datEvnt);

   AV_PGW_RCV_RETVOID;
}/* End of avPgwRoutToSgw */



/*
 *
 *       Fun:   AvPgwGetDstTeidOnTft
 *
 *       Desc:  Finds destination Teid based on TFT
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
PRIVATE S16 AvPgwGetDstTeidOnTft
(
AvPgwIpCb         *pIpCb,
CONSTANT U8       *pIpPkt,
AvPgwTunInfo      *tunInfo
)
{
   AvPgwPktFilterCb  *filterCb= NULLP;
   U32               pfCnt = 0;
   U8                protoType = pIpPkt[9];
   U16               srcPort = 0;
   U16               dstPort = 0;

   if((protoType == 0x06 /*TCP*/) || (protoType == 0x11 /*UDP*/))
   {
       srcPort = (pIpPkt[20] << 8) + pIpPkt[21];
       dstPort = (pIpPkt[22] << 8) + pIpPkt[23];
   }


   AV_PGW_RCV_TRC2(AvPgwGetDstTeidOnTft);

   for(filterCb = pIpCb->pHead; filterCb != NULLP; filterCb = filterCb->pNext)
   {
      if(((filterCb->pfCompPres & GTP_LOC_PORT_RNG_TY_PKT_FLTR_MASK) == GTP_LOC_PORT_RNG_TY_PKT_FLTR_MASK) && 
         ((filterCb->pfCompPres & GTP_REM_PORT_RNG_TY_PKT_FLTR_MASK) == GTP_REM_PORT_RNG_TY_PKT_FLTR_MASK))
      {
          if(filterCb->pfComp.locPortRangeLow <= srcPort && filterCb->pfComp.locPortRangeHigh >= srcPort &&
             filterCb->pfComp.remPortRangeLow <= dstPort && filterCb->pfComp.remPortRangeHigh >= dstPort)
          {
             *tunInfo = filterCb->tunInfo;
             AV_PGW_RCV_RETVALUE(ROK);
          }
      }
   }


   /* Only default bearer supported as of now */
	*tunInfo = pIpCb->defTunInfo;

   AV_PGW_RCV_RETVALUE(ROK);
} /* End of AvPgwGetDstTeidOnTft */



/*
 *
 *       Fun:   avPgwHndlICMPPkt
 *
 *       Desc:  This function handles the received ICMP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwHndlICMPPkt
(
CONSTANT U8  *ipPkt,
U32          bytesRcvd
)
#else
PRIVATE Void avPgwHndlICMPPkt (ipPkt, bytesRcvd)
CONSTANT U8  *ipPkt;
U32          bytesRcvd;
#endif
{
   U8  icmpType;
   U32 ipHdrLen;
   U8  ipAddr[AV_PGW_ROUT_IP_ADDR_LEN];
   U32 dstIPAddr;
   S16 ret;

   AV_PGW_RCV_TRC2(avPgwHndlICMPPkt)

   ipHdrLen = (ipPkt[0] & 0xF) * 4;

   /* ICMP header starts after IP header and ICMP type is the first octet
      of ICMP header */
   icmpType = ipPkt[ipHdrLen];

   /* Check if this is an ICMP redirect message. ICMP redirect message 
      is received because we have sent an IP packet to the gateway router,
      but we should have sent it directly. */
   if (icmpType == AV_PGW_ICMP_REDIR)
   {

      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                           "Received ICMP redirect, sending ARP request\n"));

      /* Check the destination IP address. If it is one of our IP addresses,
         process the packet. Otherwise ignore the packet. */
      /* NOTE: Here, our IP addresses refer to the IP addresses allocated by
         PDN-GW to UE through PDP context activation accept */
      dstIPAddr = (ipPkt[16] << 24) + (ipPkt[17] << 16) +
                  (ipPkt[18] << 8 ) + ipPkt[19];

      /* Search whether the destination address is in the list of 
         UE IP addresses list */
      ret = avPgwIsIpAssigned(dstIPAddr);
      if (ROK != ret)
      {
         /* This is not our IP address, just return */
         AV_PGW_RCV_RETVOID;
      }

      /* We should find the MAC address of redirect IP address by sending
         ARP request message */
      cmMemcpy(ipAddr, ipPkt + ipHdrLen + 4, AV_PGW_ROUT_IP_ADDR_LEN);

      /* Send ARP request to the redirect IP address */
      avPgwSendArpReqPkt(ipAddr);
   }
   else
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Sending IP packet to S-GW\n"));
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "PDN-GW: Received ICMP Packet "
               "from %d.%d.%d.%d to %d.%d.%d.%d \n",
               ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15],
               ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      /* Send the packet to S-GW */
      avPgwRoutToSgw(ipPkt, bytesRcvd, AV_PGW_IP_ICMP);
   }

   AV_PGW_RCV_RETVOID;
}/* End of avPgwHndlICMPPkt */


/*
 *
 *       Fun:   avPgwPktReceiver
 *
 *       Desc:  This function receives packets from network interface
 *
 *       Ret:   NULLP
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC Void *avPgwPktReceiver
(
Void *arg
)
#else
PUBLIC Void *avPgwPktReceiver (arg)
Void *arg;
#endif
{
   /* Read packets from Ethernet interface and invoke avPgwRcvdPktHndlr
      callback function for every packet read */
   pcap_loop(avPgwCb.routeCb.pcapHndl, -1, avPgwRcvdPktHndlr, NULLP);

   RETVALUE(NULLP);
}/* End of avPgwPktReceiver */



/*
 *
 *       Fun:   avPgwRcvdPktHndlr
 *
 *       Desc:  This is the callback function of pcap_loop
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwRcvdPktHndlr
(
u_char                   *args,
const struct pcap_pkthdr *hdr,
const u_char             *pktData
)
#else
PRIVATE Void avPgwRcvdPktHndlr (args, hdr, pktData)
u_char                   *args;
const struct pcap_pkthdr *hdr;
const u_char             *pktData;
#endif
{
   U16 pktType;
   U32 bytesRcvd;

   AV_PGW_RCV_TRC2(avPgwRcvdPktHndlr)

   bytesRcvd = hdr->caplen;

   /* Find out the type of Ethernet packet (bytes 12-13 in Ethernet header),
      handle IP and ARP packets */
   /* av003.101: Correction to the handling ethernet packet */
   pktType = (pktData[12] << 8) + pktData[13];
   if (pktType == AV_PGW_ETH_TYPE_IP)
   {
      /* Pass the IP packet, exclude Ethernet header 
         portion of first 14 bytes */
      avPgwHndlIPPkt(pktData + 14, bytesRcvd - 14);
   }
   else if (pktType == AV_PGW_ETH_TYPE_ARP)
   {
      /* Pass the ARP packet, exclude Ethernet header 
         portion of first 14 bytes */
      avPgwHndlARPPkt(pktData + 14, bytesRcvd - 14);
   }
   else
   {
      /* Unknown packet type received */
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Unknown packet type received - %d\n", pktType));
   }

   AV_PGW_RCV_RETVOID;
}/* End of avPgwRcvdPktHndlr */



/*
 *
 *       Fun:   avPgwHndlARPPkt
 *
 *       Desc:  This function handles the received ARP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwHndlARPPkt
(
CONSTANT U8  *arpPkt,
U32          bytesRcvd
)
#else
PRIVATE Void avPgwHndlARPPkt (arpPkt, bytesRcvd)
CONSTANT U8  *arpPkt;
U32          bytesRcvd;
#endif
{
   U16 opcode;

   AV_PGW_RCV_TRC2(avPgwHndlARPPkt)

   /* Operation fiels in the ARP packet indicates whether its an
      ARP request or ARP response, handle accordingly */
   /* av003.101: Correction to the handling ethernet packet */
   opcode = (arpPkt[6] << 8) + arpPkt[7];
   if (opcode == AV_PGW_ARP_REQ)
   {
      /* Handle ARP Request */
      avPgwHndlARPRequest(arpPkt, bytesRcvd);
   }
   else if (opcode == AV_PGW_ARP_RSP)
   {
      /* Handle ARP Response */
      avPgwHndlARPResponse(arpPkt, bytesRcvd);
   }
   else
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Unknown ARP Operation - %d\n", opcode));
   }

   AV_PGW_RCV_RETVOID;
}/* End of avPgwHndlARPPkt */


/*
 *
 *       Fun:   avPgwHndlARPRequest
 *
 *       Desc:  This function handles the ARP request packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwHndlARPRequest
(
CONSTANT U8  *arpPkt,
U32          bytesRcvd
)
#else
PRIVATE Void avPgwHndlARPRequest (arpPkt, bytesRcvd)
CONSTANT U8  *arpPkt;
U32          bytesRcvd;
#endif
{
   U32 targetIPAddr;
   S16 ret;

   AV_PGW_RCV_TRC2(avPgwHndlARPRequest)

   targetIPAddr = (arpPkt[24] << 24) + (arpPkt[25] << 16) +
                  (arpPkt[26] << 8) + arpPkt[27];

   /* Check the target IP address. If the target IP is one of our IP addresses
      in the avPgwRoutUeCb, send ARP response with local MAC address.
      Otherwise simply ignore the ARP request packet */
   ret = avPgwIsIpAssigned(targetIPAddr);
   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                     "Received ARP request for IP address:%d.%d.%d.%d Out of \
                      range\n", arpPkt[24], arpPkt[25], 
                      arpPkt[26], arpPkt[27]));
      /* Not our IP address, just return */
      AV_PGW_RCV_RETVOID;
   }

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
              "Received ARP request for IP address:%d.%d.%d.%d\n", 
              arpPkt[24], arpPkt[25], arpPkt[26], arpPkt[27]));

   /* We should further check if the ARP request has come from our own MAC
      address. We should respond only if it is NOT from our MAC address */
   ret = cmMemcmp(arpPkt + 8, avPgwCb.routeCb.localMACAddr,
                  AV_PGW_ROUT_MAC_ADDR_LEN);

   if (ROK != ret)
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                          "ARP request is for one of our IP addresses\n"));

      /* ARP request is not from our MAC address, send ARP response */
      avPgwSendArpRspPkt(arpPkt, bytesRcvd);
   }

   AV_PGW_RCV_RETVOID;
}/* End of avPgwHndlARPRequest */

/*
 *
 *       Fun:   avPgwHndlARPResponse
 *
 *       Desc:  This function handles the ARP response packet
 *
 *       Ret:   None
 *
 *       Notes: IP address conflict is not handled in this release
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwHndlARPResponse
(
CONSTANT U8  *arpPkt,
U32          bytesRcvd
)
#else
PRIVATE Void avPgwHndlARPResponse (arpPkt, bytesRcvd)
CONSTANT U8  *arpPkt;
U32          bytesRcvd;
#endif
{
   U32            updIndex = avPgwCb.routeCb.mapCnt;
   U32            i = 0;
   AvPgwRouteCb   *routeCb = &avPgwCb.routeCb;
   U32            dstIPAddr;
   U8             primaryMac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
   S16            ret = ROK;

   AV_PGW_RCV_TRC2(avPgwHndlARPResponse);

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
               "Received ARP response for:%d.%d.%d.%d\n", arpPkt[14],
               arpPkt[15], arpPkt[16], arpPkt[17]));

   /* If we receive ARP Response, more than we support, simply return */
   if (routeCb->mapCnt >= AV_PGW_MAX_NW_NODES)
   {
      AV_PGW_RCV_RETVOID;
   }

   dstIPAddr = (arpPkt[14] << 24) + (arpPkt[15] << 16) + 
               (arpPkt[16] << 8) + arpPkt[17];

   /* Add the received sender MAC address and IP address to IP->MAC map. If
      this is the MAC address of gateway then store it in routeCb */
   if (dstIPAddr != avPgwCb.peerCfg.exGwCfg.u.ipv4NetAddr)
   {
      /* start from index 1, 0 has primary gw ip to mac mapping */
      for (i = 1; i <= avPgwCb.routeCb.mapCnt; i++)
      {
         ret = cmMemcmp(routeCb->ip2Mac[i].macAddr, 
                        (arpPkt + 8), AV_PGW_ROUT_MAC_ADDR_LEN);
         if (ROK == ret)
         {
            updIndex = i;
            break;
         }
      }

      routeCb->ip2Mac[updIndex].ipAddr = dstIPAddr;
      cmMemcpy((U8 *)&(routeCb->ip2Mac[updIndex].macAddr),
                arpPkt + 8, AV_PGW_ROUT_MAC_ADDR_LEN);

      /* This MAC Map list is mainted to save time in ARP request/responses.*/ 
      if(updIndex == routeCb->mapCnt)         
      {
         (routeCb->mapCnt)++;
      }
   }
   else
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                          "Received ARP response for Gateway IP address\n"));
      ret = cmMemcmp(routeCb->ip2Mac[AV_PGW_PRIMARY_GW_IDX].macAddr, 
                     primaryMac, AV_PGW_ROUT_MAC_ADDR_LEN);
      if(ret == ROK) /* If No MAC is set */
      {
         /* Just update the MAC Address for primary destination 
            external network node */ 
         cmMemcpy(routeCb->ip2Mac[AV_PGW_PRIMARY_GW_IDX].macAddr, arpPkt + 8,
                     AV_PGW_ROUT_MAC_ADDR_LEN);
      }
   }

   /* Inform Layer manager back through Alarm */
   avSendLmAlarm(LCM_CATEGORY_PROTOCOL, LAV_EVENT_PGW_ROUTECFM,
                 LCM_CAUSE_UNKNOWN);

   AV_PGW_RCV_RETVOID;

}/* End of avPgwHndlARPResponse */

/*
 *
 *       Fun:   avPgwHndlIPPkt
 *
 *       Desc:  This function handles the received IP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwHndlIPPkt
(
CONSTANT U8  *ipPkt,
U32           bytesRcvd
)
#else
PRIVATE Void avPgwHndlIPPkt (ipPkt, bytesRcvd)
CONSTANT U8  *ipPkt;
U32           bytesRcvd;
#endif
{
   U8  protoType;

   AV_PGW_RCV_TRC2(avPgwHndlIPPkt)

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
               "Received IP packet for : %d.%d.%d.%d\n", ipPkt[16],
               ipPkt[17], ipPkt[18], ipPkt[19]));

   /* Get the protocol type from the IP header */
   protoType = ipPkt[9];
   if (protoType == AV_PGW_IP_ICMP)
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                           "Received IP packet is an ICMP packet\n"));

      /* Handle ICMP packet */
      avPgwHndlICMPPkt(ipPkt, bytesRcvd);
   }
   else
   {
      AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "Sending IP packet to S-GW\n"));

      /* Send the IP packet to S-GW */
      avPgwRoutToSgw(ipPkt, bytesRcvd, protoType);
   }

   AV_PGW_RCV_RETVOID;
}/* End of avPgwHndlIPPkt */

/*
 *
 *       Fun:   avPgwInitAdaptor
 *
 *       Desc:  This function opens Ethernet adaptor for capturing packets
 *              and creates a thread for capturing packets
 *
 *       Ret:   ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwInitAdaptor
(
Void
)
#else
PRIVATE S16 avPgwInitAdaptor (Void)
#endif
{
   S8   errBuf[PCAP_ERRBUF_SIZE];

   AV_PGW_RCV_TRC2(avPgwInitAdaptor)

   /* Open the Ethernet Adaptor with LibPCAP  */
   avPgwCb.routeCb.pcapHndl = pcap_open_live(avPgwCb.routeCb.ethIntf, 
                                        AV_PGW_ROUT_PCAP_BUF_SIZE,
                                        TRUE, AV_PGW_ROUT_PCAP_TIMEOUT, errBuf);
   if (NULLP == avPgwCb.routeCb.pcapHndl)
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                            "Error opening network adaptor\n"));
      AV_PGW_RCV_RETVALUE(RFAILED);
   }
   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF,
                    "Initialised Ethernet interface for capturing packets\n"));

   AV_PGW_RCV_RETVALUE(ROK);
}/* End of avPgwInitAdaptor */


/*
 *
 *       Fun:   avPgwBuildEthPkt
 *
 *       Desc:  This function encapsulate IP packet in to Ethernet packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwBuildEthPkt
(
U8   *ipPkt, 
U32   len
)
#else
PRIVATE Void avPgwBuildEthPkt (ipPkt, len)
U8   *ipPkt; 
U32   len;
#endif
{
   U32  dstIPAddr;
   U8  *dstMACAddr;
   AvPgwRouteCb *routeCb = &avPgwCb.routeCb;

   AV_PGW_SND_TRC2(avPgwBuildEthPkt)
   
   /* Copy IP packet into Ethernet payload */
   cmMemcpy(routeCb->ethPkt + 14, ipPkt, len);

   /* Find out the destination MAC address using destination IP address */
   dstIPAddr = (routeCb->ethPkt[30] << 24) + (routeCb->ethPkt[31] << 16) +
               (routeCb->ethPkt[32] << 8) + routeCb->ethPkt[33];

   /* Search the MAC map for destination MAC address */
   dstMACAddr = avPgwFindMACAddress(dstIPAddr);

   /* Copy the destination MAC address */
   cmMemcpy(routeCb->ethPkt, dstMACAddr, AV_PGW_ROUT_MAC_ADDR_LEN);

   AV_PGW_SND_RETVOID;
}/* End of avPgwBuildEthPkt */

/*
 *
 *       Fun:   avPgwCalcIPChecksum
 *
 *       Desc:  This function calculates the 16 bit IP header checksum
 *
 *       Ret:   U16, checksum
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE U16 avPgwCalcIPChecksum
(
U8   *ipPkt, 
U32   ipHdrLen
)
#else
PRIVATE U16 avPgwCalcIPChecksum (ipPkt, ipHdrLen)
U8   *ipPkt; 
U32   ipHdrLen;
#endif
{
   U16 word;
   U16 idx;
   U32 chksum = 0;

   AV_PGW_SND_TRC2(avPgwCalcIPChecksum)

   /* Make 16 bit words out of every two adjacent 8 bit octets in
      the packet and add them up */
   for (idx = 0; idx < ipHdrLen; idx += 2)
   {
      word = (((U16)ipPkt[idx] << 8) & 0xFF00) + (ipPkt[idx + 1] & 0xFF);
      chksum += (U32)word;
   }

   /* Take only 16 bits out of the 32 bit checksum */
   while (chksum >> 16)
   {
      chksum = (chksum & 0xFFFF) + (chksum >> 16);
   }

   /* One's complement the result */
   chksum = ~chksum;

   /* Return the 16 bit checksum */
   AV_PGW_SND_RETVALUE((U16)chksum);
}/* End of avPgwCalcIPChecksum */

/*
 *
 *       Fun:   avPgwFragmentIPPkt
 *
 *       Desc:  This function fragments the IP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE Void avPgwFragmentIPPkt
(
U8  *ipPkt, 
U32  len
)
#else
PRIVATE Void avPgwFragmentIPPkt (ipPkt, len)
U8  *ipPkt; 
U32  len;
#endif
{
   U8  *ipHdr;
   U32 ipHdrLen;
   U32 bytesSent;
   U8  ipPacket[AV_PGW_ROUT_MAX_DATA_SIZE];
   U32 bytesRemaining;
   U32 maxIPDatSize;
   U32 fragOffset;
   U32 totLen;
   U32 dataSent;
   U16 ipChksum;

   AV_PGW_SND_TRC2(avPgwFragmentIPPkt)

#ifdef ALIGN_64BIT
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, 
                        "Fragmenting IP packet of length - %d\n", len));
#else
   AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF, 
                        "Fragmenting IP packet of length - %ld\n", len));
#endif

   /* Initialise variables */
   ipHdr = ipPkt; /* Copy IP header pointer */
   ipHdrLen = (ipPkt[0] & 0xF) * 4;
   bytesSent = ipHdrLen;
   bytesRemaining = len - bytesSent;
   maxIPDatSize = AV_PGW_ROUT_MAX_DATA_SIZE - ipHdrLen;
   fragOffset = 0;

   /* Loop for fragmenting the IP packets */
   while (bytesRemaining > 0)
   {
      /* Copy the original IP header to IP fragment and modify 
         the header to reflect fragmentation */
      cmMemcpy(ipPacket, ipHdr, ipHdrLen); 

      /* Set more flag in the IP header if this is not the last IP fragment */
      if (bytesRemaining > maxIPDatSize)
      {
         ipPacket[6] |= (0x1 << 5);
      }
      
      /* Set fragment offset */
      ipPacket[6] |= (U8)((fragOffset >> 8) & 0x1F);
      ipPacket[7] = (U8)(fragOffset & 0xFF);

      if (bytesRemaining > maxIPDatSize)
      {
         totLen = AV_PGW_ROUT_MAX_DATA_SIZE;
      }
      else
      {
         totLen = bytesRemaining + ipHdrLen;
      }

      dataSent = totLen - ipHdrLen;
      if (bytesRemaining > maxIPDatSize)
      {
         U32 bytesToSend;

         /* If there are more fragments, send multiple of 8 octets as data */
         bytesToSend = dataSent % 8;
         dataSent -= bytesToSend;
         totLen -= bytesToSend;
      }

      /* Change total length */
      ipPacket[2] = (U8)((totLen >> 8) & 0xFF);
      ipPacket[3] = (U8)(totLen & 0xFF);

      /* Calculate the IP header checksum */
      ipChksum = avPgwCalcIPChecksum(ipPacket, ipHdrLen);

      /* Copy new IP header checksum */
      ipPacket[10] = (ipChksum >> 8) & 0xFF;
      ipPacket[11] = ipChksum & 0xFF;

      /* Copy the data to IP packet */
      cmMemcpy(ipPacket + ipHdrLen, ipHdr + bytesSent, dataSent);

      /* Encapsulate IP packet in an Ethernet packet */
      avPgwBuildEthPkt(ipPacket, totLen);

#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                "Sending IP fragment from %d.%d.%d.%d to %d.%d.%d.%d \
                 of length = %d\n", ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15],
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19], totLen));
#else
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                "Sending IP fragment from %d.%d.%d.%d to %d.%d.%d.%d \
                 of length = %ld\n", ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15],
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19], totLen));
#endif

      /* Send out Ethernet packet */
      avPgwSendEthPkt(avPgwCb.routeCb.ethPkt, totLen + 14);
      
      /* Update variables */
      bytesSent += dataSent;
      bytesRemaining -= dataSent;
      fragOffset += ((dataSent + 7) / 8);
   }

   AV_PGW_SND_RETVOID;
}/* End of avPgwFragmentIPPkt */

/*
 *
 *       Fun:   avPgwRoutHndlSndPkt
 *
 *       Desc:  This function handles the IP packets received from EGTP-U
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC Void avPgwRoutHndlSndPkt
(
Buffer *mBuf
)
#else
PUBLIC Void avPgwRoutHndlSndPkt (mBuf)
Buffer *mBuf;
#endif
{
   U8     ipPkt[AV_PGW_ROUT_MAX_ETH_PKT_LEN]; 
   MsgLen len;
   MsgLen cnt;
#ifdef TODO_TFT
   U32    srcIPAddr;
   U16    srcPort = 0;
   U8     protoType;
#endif

   AV_PGW_SND_TRC2(avPgwRoutHndlSndPkt)

   /* Find the length of the received IP packet */
   SFndLenMsg(mBuf, &len);

   if(len > AV_PGW_ROUT_MAX_ETH_PKT_LEN)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Failed to send the Packet as it is exceeding,\
                     Maximum Ethernet Packet Size Size of Packet-%d\n",len));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Failed to send the Packet as it is exceeding,\
                     Maximum Ethernet Packet Size Size of Packet-%ld\n",len));
#endif
      AV_PGW_SND_RETVOID;
   }

   /* Convert the mBuf to U8 array */
   SCpyMsgFix(mBuf, 0, len, ipPkt, &cnt);

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "PGW: Received packet from UE IP "
                     "address:%d.%d.%d.%d through S-GW \n",
                     ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15]));

#ifdef CNE_UESIM_DEBUG
   SPrntMsg(mBuf, 0, 0);
#endif

#if TODO_TFT
   /* av003.101: Get the protocol type */
   protoType = ipPkt[9];

   /* Get the source port */
   /* av003.101: Update port only for UDP/TCP prototype */
   if(AV_PGW_IP_TCP == protoType || AV_PGW_IP_UDP == protoType)
   {
      srcPort = (ipPkt[20] << 8 ) | ipPkt[21];
   }

   /* Get the source address */
   srcIPAddr = (ipPkt[12] << 24) + (ipPkt[13] << 16) +
               (ipPkt[14] << 8) + ipPkt[15];

   /* Get the protocol type */
   protoType = ipPkt[9];

	/* lets just pass the packet to external gateway, 
    * with out any destination checks */
   ret = avPgwUpdUeRouteCbPort(srcIPAddr, srcPort, protoType);
   if(ROK != ret)
   {
    /* We didnt get the matching UE IP which was assigned earlier to forward */
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                      "Failed to get the intended IP Address match assigned,\
                      to this UE. Received IP from UE %d.%d.%d.%d, Port(%d)\n",
                      ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15], srcPort));
      AV_PGW_SND_RETVOID;
   }
   
#endif /* TODO_TFT */
   /* Check if this port falls in the range of the ports which was initially
       assigned as part of packet filter cb. */
   /* If within range just send the packet to external network, else
      update our packet filter control block */
   
   /* Fragment the IP packet if its length is greater than the 
      Maximum data size */
   if (cnt < AV_PGW_ROUT_MAX_DATA_SIZE)
   {
      /* Encapsulate the IP packet in an Ethernet packet */
      avPgwBuildEthPkt(ipPkt, cnt);
            
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,
                 "Sending IP packet from %d.%d.%d.%d to %d.%d.%d.%d\n",
                 ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15],
                 ipPkt[16], ipPkt[17], ipPkt[18], ipPkt[19]));

      /* Send out Ethernet packet */
      avPgwSendEthPkt(avPgwCb.routeCb.ethPkt, cnt + 14);
   }
   else
   {
      /* Fragment IP packet */
      avPgwFragmentIPPkt(ipPkt, cnt);
   }

   AV_PGW_SND_RETVOID;
}/* End of avPgwRoutHndlSndPkt */


#ifdef TODO_TFT
/*
 *
 *       Fun:  avPgwUpdUeRouteCbPort  
 *
 *       Desc: This function updates a routing control block 
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE S16 avPgwUpdUeRouteCbPort
(
U32      ipAddr,
U16      port,
U8       protoType
)
#else
PRIVATE S16 avPgwUpdUeRouteCbPort(ipAddr, port, protoType)
U32      ipAddr;
U16      port;
U8       protoType;
#endif
{
   AvPgwRouteCb *routeCb = &avPgwCb.routeCb;
   AvPgwUeRouteCb *ueRouteCb = NULLP;
   AvPgwPktFilterCb *pktFilterCb = NULLP;
   U16 routeIdx, pkFilterIndx, pFCompIndx;
   U16 portRangeHigh;

   AV_PGW_SND_TRC2(avPgwUpdUeRouteCbPort)

   /* Right first get the RouteCb based on 
      destination IP Address */
/* TODO
 * Multi UE Here we can calculate the index as
 * index = ipAddr - startadd 
 * see if the assigned flag is set then proceed
 */
   /* Loop upto latest counter PDN Nw Addresses assigned to set of UEs */
   for (routeIdx = 0; routeIdx < routeCb->nmbUeNwAddrCnt; routeIdx++)
   {
      if (ipAddr == routeCb->ueRouteCbArr[routeIdx].nwUeAddr.addr.u.ipv4NetAddr)
      {
         /* Right Match found */
         ueRouteCb = &routeCb->ueRouteCbArr[routeIdx];  

         /* Next match on packet filter context */
         /* Now the criterion is based on:
            Destination/remote port + Port Range which we serve to UEs */
         /* Remote Port and Remote Port range is a packet component within
            a packet filter context */
            
        /* Loop upto existing packet filters assigned to this UE */
         for (pkFilterIndx = 0;
              pkFilterIndx < ueRouteCb->uePktFilterCbLst.count;
              pkFilterIndx++)
         {
            /* We cannot check on specific port as we will filter based on
               UE port range we are serving */

            pktFilterCb = &ueRouteCb->uePktFilterCbLst[pkFilterIndx];

            for (pFCompIndx = 0; pFCompIndx < pktFilterCb->nmbPfComp; 
                 pFCompIndx++)
            {
               /* Try matching based on Protocol Type for IPV4 / 
                  Next Header Type for IPV6 */
               if((AV_PGW_PF_COMP_PROTOCOL == 
                        pktFilterCb->pfComp[pFCompIndx].type) &&
                     (protoType == pktFilterCb->pfComp[pFCompIndx].t.val)) 
               {      
                  /* Check on the remote port range component match */
                  portRangeHigh = GetHiWord(pktFilterCb->pFilterPort);

                  /* Update the highest port we are serving with this UE */ 
                  if(port > portRangeHigh)
                     /* && port < AV_PGW_MAX_WELL_KNOWN_IP_PORT)*/
                  {
                     pktFilterCb->pFilterPort = 
                        (pktFilterCb->pFilterPort & 0x0000ffff) | 
                        PutHiWord(pktFilterCb->pFilterPort,port);
                     break; /* We have updated the port on the right component
                               no need to further loop on next packet filter */ 
                  }
               }
            }
         } /* End of Packet Filter Cb Loop */

         AV_PGW_SND_RETVALUE(ROK);
      }/* End of UeRouteCb Match */
   } /* End of UeRoutCb Loop */

   AV_PGW_SND_RETVALUE(RFAILED);

}/* End of avPgwUpdUeRouteCbPort */
#endif

#if 1 /*def REMOVECODE*/

/*
 *
 *       Fun:   avPgwFindMACAddress
 *
 *       Desc:  This function finds the MAC address corresponding to an
 *              IP address from the IP->MAC map
 *
 *       Ret:   Pointer to a MAC address (U8 *)
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PRIVATE U8 * avPgwFindMACAddress
(
U32 ipAddr
)
#else
PRIVATE U8 * avPgwFindMACAddress (ipAddr)
U32 ipAddr;
#endif
{
 /*  AvPgwRoutIP2MacMap *ip2macTable = &avPgwCb.routeCb.ip2Mac[0];*/
   U8    idx;

   AV_PGW_SND_TRC2(avPgwFindMACAddress)

   /* search for MAC address mapped to the ipAddr */
   for (idx = 0; idx < avPgwCb.routeCb.mapCnt; idx++)
   {
      if (ipAddr == avPgwCb.routeCb.ip2Mac[idx].ipAddr)
      {
         AV_PGW_SND_RETVALUE(avPgwCb.routeCb.ip2Mac[idx].macAddr); 
      }
   }

   /* Retrun Gateway MAC address if no MAC address is found in the map */
   AV_PGW_SND_RETVALUE(avPgwCb.routeCb.ip2Mac[AV_PGW_PRIMARY_GW_IDX].macAddr);

}/* End of avPgwFindMACAddress */

#endif
#else

/*
 *
 *       Fun:   avSendEgDLData
 *
 *       Desc:  
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 */
#ifdef ANSI
PUBLIC Void avSendEgDLData
(
)
#else
PUBLIC Void avSendEgDLData()
#endif
{
   AvPgwUeTunnelCb *ueTunl, *prevUeTnl;
   EgtUEvnt *datEvnt = NULLP;
   Mem  smem = { avPgwCb.routeCb.init.region, avPgwCb.routeCb.init.pool };
   Buffer *mBuf = NULLP;
   CmTptAddr *tptAddr;
   S16    ret;

   AV_PGW_RCV_TRC2(avSendEgDLData)

   cmMemset(dataPkt, 0xFF, sizeof(dataPkt));

   /* Packet is good, construct EgtUEvnt to tunnel through GTP */

   /* Send data to all established tunnels at intervals of 20 seconds */
   do
   {
      prevUeTnl = ueTunl = NULLP;
      while (cmHashListGetNext (&(avPgwCb.egtUSapCb->ueTunnelCbHlCp),
                             (PTR) prevUeTnl,
                             (PTR *) &ueTunl) == ROK)
      {

      /* Allocate Buffer for posting to S-GW through EGTP-U */
      ret = SGetMsg(avPgwCb.routeCb.init.region, avPgwCb.routeCb.init.pool, &mBuf);
      if (ROK != ret)
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, 
                            "Failed to allocate mBuf - %d\n", ret));
         AV_PGW_RCV_RETVOID;
      }

      /* Add the received IP Pkt buffer to mBuf */
      SAddPstMsgMult((Data *)dataPkt, AV_PGW_ROUT_MAX_DATA_SIZE, mBuf);

      AV_PDNGW_RECVTSK_ALLOC((datEvnt), sizeof(EgtUEvnt));

      if(NULLP != datEvnt)
      {
         /**************************************************************
         * Allocate memory for the EGTP-U Data message of size EgUMsg *
         **************************************************************/
         if((ret = avPgwUtlAllocEguMsg(&((datEvnt)->u.egMsg), &smem)) != ROK )
         {
            AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                       "avPgwUtlBldEgtDatReq(): Memory allocation for \
                       EGTP-U message failed \n"));
            AV_PDNGW_RECVTSK_FREE((datEvnt), sizeof(EgtUEvnt));
            SPutMsg(mBuf);
            AV_PGW_RCV_RETVOID;
         }

         /* Message type */
         datEvnt->u.egMsg->msgHdr.msgType = EGT_GTPU_MSG_GPDU;

         /* Destination TEID */
         datEvnt->u.egMsg->msgHdr.teId = ueTunl->remoteTeId;

         datEvnt->u.egMsg->msgHdr.seqNumber.pres = TRUE;
         datEvnt->u.egMsg->msgHdr.seqNumber.val = AV_PGW_EGU_DFLT_SEQ_NMB;

         datEvnt->u.egMsg->lclTeid = ueTunl->localTeIdKey;

         /* Fill Local Address */
         tptAddr = &((datEvnt)->u.egMsg->srcAddr);
         tptAddr->type = CM_NETADDR_IPV4;
         tptAddr->u.ipv4TptAddr.port = ueTunl->lclAddr.u.ipv4TptAddr.port;
         tptAddr->u.ipv4TptAddr.address = ueTunl->lclAddr.u.ipv4TptAddr.address;

         /* Fill Remote Address */
         tptAddr = &((datEvnt)->u.egMsg->remAddr);
         tptAddr->type = CM_NETADDR_IPV4;
         tptAddr->u.ipv4TptAddr.port = ueTunl->remAddr.u.ipv4TptAddr.port;
         tptAddr->u.ipv4TptAddr.address = ueTunl->remAddr.u.ipv4TptAddr.address;

         /* Assign the received buffer to EGTP-U map */  
         datEvnt->u.egMsg->u.mBuf = mBuf;
         AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"Sending data for UE tunnel %d\n",  ueTunl->localTeIdKey));

         /* Post the message to send to S-GW through GTP-U */
         ret = EuLiEgtEguDatReq(&(avPgwCb.routeCb.pst), avPgwCb.egtUSapCb->spId, 
                                datEvnt);
      }
      else /* Memory allocation failure */
      {
         AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF,
                       "Memory allocation for EGTP-U event failed \n"));
         SPutMsg(mBuf);
         AV_PGW_RCV_RETVOID;
      }
      prevUeTnl = ueTunl;
      }
      AV_PGW_SND_DBG_INFO((AV_PGW_SND_PRNTBUF,"PDN-GW: Sent data for all the UEs, waiting.... \n"));
      usleep(20000);
   }while(1);

   AV_PGW_RCV_RETVOID;
}/* End of avSendEgDLDataStart */


#endif

/*************************************************************************
 *
 *       Fun:   avPgwFndUeRouteCb
 *
 *       Desc:  This function fragments the IP packet
 *
 *       Ret:   None
 *
 *       Notes: none
 *
 *       File:  av_rout.c
 *
 *************************************************************************/
#ifdef ANSI
PRIVATE AvPgwUeRouteCb* avPgwFndUeRouteCb
(
CmLListCp      *routeCbLst,
CmInetIpAddr   ipAddr 
)
#else
PRIVATE AvPgwUeRouteCb* avPgwFndUeRouteCb(routeCbLst, U32 ipAddr)
CmLListCp      *routeCbLst;
CmInetIpAddr   ipAddr;
#endif
{
   CmLList              *lnk = NULLP;
   AvPgwUeRouteCb       *ueRouteCb = NULLP;

   AV_PGW_SND_TRC2(avPgwFndUeRouteCb)

   ueRouteCb = (AvPgwUeRouteCb *)CM_LLIST_FIRST_NODE(routeCbLst, lnk);
   while (ueRouteCb)
   {
      if (ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr == ipAddr)
      {
         return ueRouteCb;
      }

      ueRouteCb = (AvPgwUeRouteCb *)CM_LLIST_NEXT_NODE(routeCbLst, lnk);
   }

   return (AvPgwUeRouteCb *)NULLP;
} /* End of avPgwFndUeRouteCb */


/**************************************************************************//**
 *
 * @brief
 *    Converts IP Address from INET U32 format to 4 Bytes array
 *
 * @param[in] ipAddr
 *    Inet IP Address
 * @param[inout]  ipAddrArr
 *    pointer to 4 bytes array 
 *
 * @return
 *    No return values
 *              
 *****************************************************************************/
PUBLIC Void AvPgwUtlInetIpAddrToArr
(
CmInetIpAddr   ipAddr,
U8             *pIpAddrArr
)
{
   U16      ipHiWord = 0;
   U16      ipLoWord = 0;

   AV_PGW_SND_TRC2(AvPgwUtlInetIpAddrToArr);

   ipHiWord = (U16)GetHiWord(ipAddr);
   ipLoWord = (U16)GetLoWord(ipAddr);

   pIpAddrArr[0] = (U8)GetHiByte(ipHiWord);
   pIpAddrArr[1] = (U8)GetLoByte(ipHiWord);
   pIpAddrArr[2] = (U8)GetHiByte(ipLoWord);
   pIpAddrArr[3] = (U8)GetLoByte(ipLoWord);

   AV_PGW_RETVOID;
} /* AvPgwUtlInetIpAddrToArr */


/**************************************************************************//**
 *
 * @brief
 *    Sets state of tunnel information in route CB
 *
 * @param[in] pUeRouteCb
 *    Pointer to UE's route CB
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
)
{
   AvPgwIpCb         *pIpCb = NULLP;
   AvPgwPktFilterCb  *pPfCb = NULLP;

   AV_PGW_RCV_TRC2(avPgwSetRouteCbTunState);

   AV_PGW_GET_IPCB((tunnelCb->ueRouteCb->nwUeAddr.addr.u.ipv4NetAddr), pIpCb);
   if (NULLP != pIpCb)
   {
      if(tunnelCb->tunnelType == AV_PGW_DATA_TUNNEL_DFLT)
      {
         pIpCb->defTunInfo.state = state;
      }
      else
      {
         pPfCb = pIpCb->pHead;
         while(pPfCb)
         {
            if((tunnelCb->bearerId == AV_PGW_GET_EBI_FROM_PFID(pPfCb->pFId)))
            {
               pPfCb->tunInfo.state = state;
               break;
            }
            pPfCb = pPfCb->pNext;
         }  
      }
   }
   else
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Could not find IP Cb"));
   }

   AV_PGW_RETVOID;
} /* AvPgwUtlInetIpAddrToArr */


/**************************************************************************//**
 *
 * @brief
 *
 * @param[in] 
 * @param[in]  state
 *
 * @return
 *              
 *****************************************************************************/
PUBLIC Void avPgwUpdtPdnCbtoIpCb
(
AvPgwPdnCb        *pdnCb
)
{
   AvPgwIpCb         *pIpCb = NULLP;

   AV_PGW_RCV_TRC2(avPgwUpdtPdnCbtoIpCb);

   AV_PGW_GET_IPCB(pdnCb->ipAddr.u.ipv4NetAddr, pIpCb);
   if (NULLP != pIpCb)
   {
      pIpCb->pdnCb = pdnCb;
   }
   else
   {
      AV_PGW_RCV_DBG_ERROR((AV_PGW_RCV_PRNTBUF, "Could not find IP Cb"));
   }

   AV_PGW_RETVOID;
} /* avPgwUpdtPdnCbtoIpCb */

PUBLIC S16 AvPgwGetPdnCbOnIp(AvPgwIpCb *pIpCb, AvPgwPdnCb **pdnCb)
{
   AV_PGW_RCV_TRC2(AvPgwGetPdnCbOnIp);

   *pdnCb = pIpCb->pdnCb;
    AV_PGW_RCV_RETVALUE(ROK);

   /*AV_PGW_RCV_RETVALUE(pIpCb->pdnCb);*/
} /* End of AvPgwGetPdnCbOnIp */

#ifdef ANSI
PUBLIC Void avPgwLoopBackData
(
Buffer *mBuf
)
#else
PUBLIC Void avPgwLoopBackData (mBuf)
Buffer *mBuf;
#endif
{
   U8     ipPkt[AV_PGW_ROUT_MAX_ETH_PKT_LEN]; 
   MsgLen len;
   MsgLen cnt;
   U32    srcIPAddr;
   U16    srcPort = 0;
   U32    dstIPAddr;
   U16    dstPort = 0;
   U8     protoType;
   U8     tempVal[4];

   int n;

   AV_PGW_SND_TRC2(avPgwLoopBackData)

   /* Find the length of the received IP packet */
   SFndLenMsg(mBuf, &len);

   if(len > AV_PGW_ROUT_MAX_ETH_PKT_LEN)
   {
#ifdef ALIGN_64BIT
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Failed to send the Packet as it is exceeding,\
                     Maximum Ethernet Packet Size Size of Packet-%d\n",len));
#else
      AV_PGW_SND_DBG_ERROR((AV_PGW_SND_PRNTBUF,
                     "Failed to send the Packet as it is exceeding,\
                     Maximum Ethernet Packet Size Size of Packet-%ld\n",len));
#endif
      AV_PGW_SND_RETVOID;
   }

   /* Convert the mBuf to U8 array */
   SCpyMsgFix(mBuf, 0, len, ipPkt, &cnt);

   AV_PGW_RCV_DBG_INFO((AV_PGW_RCV_PRNTBUF, "PGW: Received packet from UE IP "
                     "address:%d.%d.%d.%d through S-GW \n",
                     ipPkt[12], ipPkt[13], ipPkt[14], ipPkt[15]));

#ifdef CNE_UESIM_DEBUG
   SPrntMsg(mBuf, 0, 0);
#endif

   /* av003.101: Get the protocol type */
   protoType = ipPkt[9];

   /* Get the source port */
   /* av003.101: Update port only for UDP/TCP prototype */
   if(0x06 == protoType || 0x11 == protoType)
   {
      cmMemcpy(tempVal, &ipPkt[20], 2);
      cmMemcpy(&ipPkt[20], &ipPkt[22], 2);
      cmMemcpy(&ipPkt[22], tempVal, 2);
   }

   /* Get the source address */
   cmMemcpy(tempVal, &ipPkt[12], 4);
   cmMemcpy(&ipPkt[12],&ipPkt[16], 4);
   cmMemcpy(&ipPkt[16], tempVal, 4);

   avPgwRoutToSgw(ipPkt, len, protoType);

   AV_PGW_SND_RETVOID;
}/* End of avPgwRoutHndlSndPkt */


/**********************************************************************
  
         End of file:     av_rout.c@@/main/1 - Tue Aug 17 18:54:34 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      sp               1. LTE-CNE:PGW Initial Release.
/main/1      av001.101 rp              1. Added support for dedicated
                                          tunnels and TFT
/main/1      av003.101 ve              1. Converting in to network bit order.
/main/1      av004.101 ms              1. port checking removed.
                       pmacharla       1. Multi UE and eGTP 2.1 upgrade
*********************************************************************91*/


