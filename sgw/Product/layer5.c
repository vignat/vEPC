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
  
     Name:     TCP UDP Convergence Layer (TUCL)
  
     Type:     C source file
  
     Desc:     Sample Code for service user
              
     File:     layer5.c
  
     Sid:      layer5.c@@/main/6 - Mon Mar  3 20:09:33 2008

     Prg:      asa
  
*********************************************************************21*/

/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

/* external headers */
#ifdef HI_TLS
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport defines */
#include "lhi.h"           /* layer management, TUCL  */
#include "hit.h"           /* upper interface */
#ifdef HI_LKSCTP
#include "sct.h"           /* SCT interface */
#endif
#ifdef FTHA
#include "sht.h"
#endif
#include "hi_acc.h"        /* Acceptance Test */
  
/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* common sockets */
#include "cm_lib.x"        /* common library functions */
#include "cm_tpt.x"        /* common transport typedefs */
#include "lhi.x"           /* layer management TUCL */
#ifdef HI_LKSCTP
#include "sct.h"           /* SCT interface */
#include "sct.x"           /* SCT interface */
#endif
#include "hit.x"           /* upper interface */
#ifdef FTHA
#include "sht.x"
#endif
#include "hi_acc.x"        /* Acceptance Test */

#ifdef LOCAL_INTF
#include <net/if.h>        /* for Local Interface info */
#endif /* LOCAL_INTF */

/* local defines, if any */

/* local typedefs, if any */

/* local externs, if any */
/* hi029.201: Fix for compilation warning*/
/* hi031.201: Fix for compilation warning*/
Txt l5prntBuf[HIACC_PRNTBUF_SIZE];


/* private variable declarations */

/*
*
*       Fun:   Connect Indication
*
*       Desc:  This function indicates the service user of a new
*              TCP client connection.
*              
*              "peerAddr" - has the address of the remote entity
*                           which has initiated the client connection.
*
*       Ret:   ROK   - ok
*
*       Notes: Service user will be issued a Connect Indication only 
*              when it has initiated a TCP server using 
*              HiUiHitServOpenReq() primitive.
*
*       File:  layer5.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitConInd
(
Pst *pst,                 /* post stucture */
SpId suId,                /* service user Id */
UConnId servConId,        /* server's connection id */
UConnId spConId,          /* service provider's connection Id */
CmTptAddr *peerAddr       /* peer address */
)
#else
PUBLIC S16 HcLiHitConInd(pst, suId, servConId, spConId, peerAddr)
Pst *pst;                 /* post stucture */
SpId suId;                /* service user Id */
UConnId servConId;        /* server's connection id */
UConnId spConId;          /* service provider's connection Id */
CmTptAddr *peerAddr;      /* peer address */
#endif
{

   TRC3(HcLiHitConInd)

   /* Store the spConId for the new socket */
   hiAccCb.curTst.newSpConId = spConId;

  
   /* Issue a Connect Response to the HI Layer */
   HcLiHitConRsp(&hiAccCb.uiPst, HI_ACC_SAP_0, HIACC_NEW_SUCON_ID, 
                 spConId);

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
} /* end of HcLiHitConInd */


/*
*
*       Fun:   Connect Confirmation
*
*       Desc:  This primitive is used by the TUCL  to indicate 
*              to the service user  that the socket (as requested
*              by HiUiHitServOpenReq or by HiUiHitConReq) has been
*              opened, and the UDP data transfer may take place or 
*              new TCP client connections may be accepted.
*              
*       Ret:   ROK   - ok
*
*       Notes: None
*
*       File:  layer5.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitConCfm
(
Pst  *pst,                /* post structure */
SuId  suId,               /* Service User Id */
UConnId  suConId,         /* service user's connection Id */
UConnId  spConId,         /* service provider's connection Id */
CmTptAddr *localTptAddr
)
#else
PUBLIC S16 HcLiHitConCfm(pst, suId, suConId, spConId, localTptAddr)
Pst  *pst;                /* post structure */
SuId  suId;               /* Service User Id */
UConnId  suConId;         /* service user's connection Id */
UConnId  spConId;         /* service provider's connection Id */
CmTptAddr *localTptAddr;
#endif
{

   TRC3(HcLiHitConCfm)

/*
   hiAccCb.curTst.conId = spConId;
 */

   if((suConId == HIACC_RAW_SUCON_ID) || (suConId == HIACC_SERV_SUCON_ID))
      hiAccCb.curTst.servSpConId = spConId;
   else
      hiAccCb.curTst.cltSpConId =  spConId;

   hiAccCb.curTst.state++;
   RETVALUE(ROK);
} /* end of HcLiHitConCfm */


/*
*
*       Fun:   Bind Confirmation
*
*       Desc:  This function indicates to the service user whether the 
*              bind operation has been successful or not.
*              
*       Ret:   ROK
*
*       Notes: None
*
*       File:  layer5.c
*
*/
  
#ifdef ANSI
PUBLIC S16 HcLiHitBndCfm
(
Pst         *pst,         /* post structure */
SuId        suId,         /* Service User Id */
U8          status        /* status */
)
#else
PUBLIC S16 HcLiHitBndCfm(pst, suId, status)
Pst         *pst;         /* post structure */
SuId        suId;         /* Service User Id */
U8          status;       /* status */ 
#endif
{
   TRC3(HcLiHitBndCfm)


   sprintf(l5prntBuf,"\tReceived Bind Confirm from TUCL \n");
   SPrint(l5prntBuf);
   if(status == CM_BND_OK)
   {
        sprintf(l5prntBuf,"\tBind Successful suId(%d)\n",suId);
        SPrint(l5prntBuf);
   }
   else
   {
        sprintf(l5prntBuf,"\tOooops Bind Failed suId(%d)\n",suId);
        SPrint(l5prntBuf);
   }

   /* Initialization complete
    * proceed with test cases 
    */
   hiAccCb.cfgDone = TRUE;  
   RETVALUE(ROK);
} /* end of HcLiHitBndCfm */


/*
*
*       Fun:   Data Indication
*
*       Desc:  This function provides for indicating incoming 
*              TCP data on the socket.
*
*       Ret:   ROK   - ok
*
*       Notes: HI layer strips off the TPKT header and delievers
*              the data in a message buffer.
*
*       File:  layer5.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitDatInd
(
Pst         *pst,         /* post structure */
SuId        suId,         /* service User Id */
UConnId      suConId,     /* service provider's connection id */
Buffer      *mBuf         /* message buffer */
)
#else
PUBLIC S16 HcLiHitDatInd(pst, suId, suConId, mBuf)
Pst         *pst;         /* post structure */
SuId        suId;         /* service User Id */
UConnId      suConId;     /* service provider's connection id */ 
Buffer      *mBuf;        /* message buffer */
#endif
{

   UConnId  spConId;
   S16      ret;

   static int numDatIndRecvd = 0;

   TRC3(HcLiHitDatInd)
   
   if(hiAccCb.curTst.prntFlag)
   {
      sprintf(l5prntBuf,"received DatInd from TUCL \n");
      SPrint(l5prntBuf);
   }

   if(hiAccCb.curTst.id == HI_TSTID_5)
   {
      if(suConId == HIACC_NEW_SUCON_ID)
      {
         numDatIndRecvd++;
         /* Message received back on the accepted connection */
         /* Check if message was corrupted */
         ret = hiAccChkMsg(hiAccCb.bigMsg.msg, mBuf);
         if(ret != ROK)
         {
            sprintf(l5prntBuf,"received corrupt echo \n");
            SPrint(l5prntBuf);
            hiAccCb.curTst.state  = HI_ACC_ST_END;
            hiAccCb.curTst.result = HI_TST_FAILED;
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
         /* Close connections when all messages are received */
         if(numDatIndRecvd == HI_ACC_NUM_TST_MSGS)
         {
            /* reset the received counter */
            numDatIndRecvd = 0;

            hiAccCb.curTst.state += 1;
            SPutMsg(mBuf);
            RETVALUE(ROK);
         }
         SPutMsg(mBuf);   
      }
      else /* Client received data */
      {
         /* Client loops back data */
         HcLiHitDatReq(&hiAccCb.uiPst, HI_ACC_SAP_0, 
                       hiAccCb.curTst.cltSpConId, 
                       mBuf);
      }

      RETVALUE(ROK);
   }    
#ifdef IPV6_SUPPORTED
   else if (hiAccCb.curTst.id == HI_TSTID_11)
   {
      if(suConId == HIACC_NEW_SUCON_ID)
      {
         numDatIndRecvd++;
         /* Message received back on the accepted connection */
         /* Check if message was corrupted */
         ret = hiAccChkMsg(hiAccCb.bigMsg.msg, mBuf);
         if(ret != ROK)
         {
            sprintf(l5prntBuf,"received corrupt echo \n");
            SPrint(l5prntBuf);
            hiAccCb.curTst.state  = HI_ACC_ST_END;
            hiAccCb.curTst.result = HI_TST_FAILED;
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
         /* Close connections when all messages are received */
         if(numDatIndRecvd == HI_ACC_NUM_TST_MSGS)
         {
            /* reset the received counter */
            numDatIndRecvd = 0;

            hiAccCb.curTst.state += 1;
            SPutMsg(mBuf);
            RETVALUE(ROK);
         }
         SPutMsg(mBuf);   
      }
      else /* Client received data */
      {
         /* Client loops back data */
         HcLiHitDatReq(&hiAccCb.uiPst, HI_ACC_SAP_0, 
                       hiAccCb.curTst.cltSpConId, 
                       mBuf);
      }
      
      RETVALUE(ROK);
   }
#endif /* IPV6_SUPPORTED */
#ifdef HI_TLS
   else if (hiAccCb.curTst.id == HI_TSTID_18
            ||  hiAccCb.curTst.id == HI_TSTID_19)
   {
      /* we can get multiple DatInds; don't let state run away */
      if (hiAccCb.curTst.state == 6)
         hiAccCb.curTst.state--;
   }
#endif

   /* Get the spConId of the new server socket from hiAccCb */
   spConId = hiAccCb.curTst.newSpConId;

   if(hiAccCb.curTst.id == HI_TSTID_1
#ifdef HI_TLS
         ||  hiAccCb.curTst.id == HI_TSTID_19
#endif
         )
      spConId = hiAccCb.curTst.cltSpConId;
   
   hiAccCb.curTst.state++;

#ifdef HI_TLS
   if (hiAccCb.curTst.id == HI_TSTID_18
         ||  hiAccCb.curTst.id == HI_TSTID_19)
   {
      MsgLen mLen;

      SFndLenMsg(mBuf, &mLen);
      hiAccCb.curTst.loopLen += mLen;
   }
#endif

   /* Loop back the same data received */
   HcLiHitDatReq(&hiAccCb.uiPst, HI_ACC_SAP_0, spConId, mBuf);

   RETVALUE(ROK);
} /* end of HcLiHitDatInd */


/*
*
*       Fun:   Unit Data Indication
*
*       Desc:  This function provides for indicating incoming 
*              UDP datagrams to the service user.
*
*              "srcAddr" - indicates the address from where the 
*                          data was received.
*       Ret:   ROK
*
*       Notes: None
*
*       File:  layer5.c
*
*/
#ifdef LOCAL_INTF  
#ifdef ANSI
PUBLIC S16 HcLiHitUDatInd
(
Pst *pst,                 /* post stucture */
SuId suId,                /* service user Id */
UConnId suConId,          /* service user's connection Id */
CmTptAddr *srcAddr,       /* source transport address */
CmTptAddr *remAddr,       /* Remote Address */
CmIpHdrParm *ipHdrParm,   /* IP header Parameters */
CmTptLocalInf *localIf,   /* local intf on which pkt arrived */
Buffer *mBuf              /* message buffer */
)
#else
PUBLIC S16 HcLiHitUDatInd(pst, suId, suConId, srcAddr , remAddr, 
                          ipHdrParm, localIf, mBuf)
Pst *pst;                 /* post stucture */
SuId suId;                /* service user Id */
UConnId suConId;          /* service user's connection Id */
CmTptAddr *srcAddr;       /* source transport address */
CmTptAddr *remAddr;       /* Remote Address */
CmIpHdrParm *ipHdrParm;   /* IP header Parameters */
CmTptLocalInf *localIf;   /* local intf on which pkt arrived */
Buffer *mBuf;             /* message buffer */
#endif /* ANSI */
#else
#ifdef ANSI
PUBLIC S16 HcLiHitUDatInd
(
Pst *pst,                 /* post stucture */
SuId suId,                /* service user Id */
UConnId suConId,          /* service user's connection Id */
CmTptAddr *srcAddr,       /* source transport address */
CmTptAddr *remAddr,       /* Remote Address */
CmIpHdrParm *ipHdrParm,   /* IP header Parameters */
Buffer *mBuf              /* message buffer */
)
#else
PUBLIC S16 HcLiHitUDatInd(pst, suId, suConId, srcAddr , remAddr, ipHdrParm, mBuf)
Pst *pst;                 /* post stucture */
SuId suId;                /* service user Id */
UConnId suConId;          /* service user's connection Id */
CmTptAddr *srcAddr;       /* source transport address */
CmTptAddr *remAddr;       /* Remote Address */
CmIpHdrParm *ipHdrParm;   /* IP header Parameters */
Buffer *mBuf;             /* message buffer */
#endif /* ANSI */
#endif
{

   static int numDatIndRecvd = 0;
   
#if (defined(IPV4_OPTS_SUPPORTED) && defined(HI_RSVP_SUPPORT))
   static int rsvpMsgNum = 0;/* count of rsvp msg */
#endif /* IPV4_OPTS_SUPPORTED */
/* hi001.105 - Removed compiler warnings for IPV6 support */
#if (defined(IPV6_OPTS_SUPPORTED) && defined(HI_RSVP_SUPPORT))     
   U8 i;                  /* loop counter */ 
   U8 j;                  /* loop counter */
   CmInetIpv6HdrParm *ipv6HdrParm;
#endif /* IPV6_OPTS_SUPPORTED */
   
   S16 ret;

   TRC3(HcLiHitUDatInd)

   if(hiAccCb.curTst.prntFlag)
   {
      sprintf(l5prntBuf,"received UDatInd from TUCL remport(%d)\n", 
              srcAddr->u.ipv4TptAddr.port);
      SPrint(l5prntBuf);
   }

   if(hiAccCb.curTst.id == HI_TSTID_2)
   {
      ret = hiAccChkMsg(hiAccCb.tstMsgs[suConId].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received corrupt echo \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.state  = 5;
         hiAccCb.curTst.result = HI_TST_FAILED;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      numDatIndRecvd++;
      if(numDatIndRecvd == HIACC_UDP_MAX_SERV)
      {
         if(hiAccCb.curTst.prntFlag)
         {
            sprintf(l5prntBuf,"received all messages \n");
            SPrint(l5prntBuf);
         }
         numDatIndRecvd = 0;
         hiAccCb.curTst.state  = 5;
         hiAccCb.curTst.result = HI_TST_OK;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
#ifdef LOCAL_INTF      
      /* print local interface address if it is there */
      if (localIf->intfPrsnt == TRUE)
      {   
         /* if come here, then we are getting local intf for UDP IPv4 socket */     
         sprintf(l5prntBuf,"Received Pkt: Intf Index: %ld  Intf Addr: %lx ",
                         localIf->localIf, localIf->localIfAddr.u.ipv4NetAddr);
         SPrint(l5prntBuf);

         sprintf(l5prntBuf,"\n");
         SPrint(l5prntBuf);
      }
#endif /* LOCAL_INTF */      
   }

   if (hiAccCb.curTst.id == HI_TSTID_7)
   {
      if((ipHdrParm->type == CM_HDRPARM_IPV4) && 
         (ipHdrParm->u.hdrParmIpv4.proto.val == CM_PROTOCOL_ICMP))
      {

         sprintf(l5prntBuf,"An ICMP Message received \n");
         SPrint(l5prntBuf);
         sprintf(l5prntBuf,"Protocol :  %d \n",
                            ipHdrParm->u.hdrParmIpv4.proto.val);
         SPrint(l5prntBuf);
         /*  hiAccCb.curTst.state++; */
      }
      else 
      {
         static int msgIdx = 0;

         ret = hiAccChkMsg(hiAccCb.tstMsgs[msgIdx].msg, mBuf);
         if(ret != ROK)
         {
            sprintf(l5prntBuf,"received corrupt echo \n");
            SPrint(l5prntBuf);
            hiAccCb.curTst.result = HI_TST_FAILED;
            hiAccCb.curTst.errFlag = TRUE;
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
       
         numDatIndRecvd++;
         if(numDatIndRecvd == 2)
         {
            if(hiAccCb.curTst.prntFlag)
            {
               sprintf(l5prntBuf,"received all messages \n");
               SPrint(l5prntBuf);
            }
            numDatIndRecvd = 0;
            hiAccCb.curTst.state  = 4;
            if(++msgIdx == 2)
            {
               msgIdx = 0;
               numDatIndRecvd = 0;
               hiAccCb.curTst.state  = 6;
               hiAccCb.curTst.result = HI_TST_OK;
               SPutMsg(mBuf);
               RETVALUE(ROK);
            }
         }
         
         if(ipHdrParm->type == CM_HDRPARM_IPV4)
         {
            
            sprintf(l5prntBuf,"Protocol :  %d \n",
            ipHdrParm->u.hdrParmIpv4.proto.val);
            SPrint(l5prntBuf);
            
            if (ipHdrParm->u.hdrParmIpv4.dfBit.pres)
            {
               sprintf(l5prntBuf,"DFBit :  %d \n",
               ipHdrParm->u.hdrParmIpv4.dfBit.val);
               SPrint(l5prntBuf);
            }
            else
            {
               sprintf(l5prntBuf,"DFBit : 0 \n");
               SPrint(l5prntBuf);
            }
            
            sprintf(l5prntBuf,"TTL   : %d \n",
            ipHdrParm->u.hdrParmIpv4.ttl.val);
            SPrint(l5prntBuf);
            
            sprintf(l5prntBuf,"Tos   : %d \n",
            ipHdrParm->u.hdrParmIpv4.tos.val);
            SPrint(l5prntBuf);
            /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
            sprintf(l5prntBuf, "Remote IP Address   : %lx \n",
                  remAddr->u.ipv4TptAddr.address);
#else
            sprintf(l5prntBuf, "Remote IP Address   : %x \n",
                  remAddr->u.ipv4TptAddr.address);
#endif
            SPrint(l5prntBuf);
         }

         hiAccCb.curTst.state++;
      }
   }
   
#if (defined(IPV4_OPTS_SUPPORTED) && defined(HI_RSVP_SUPPORT))
   if (hiAccCb.curTst.id == HI_TSTID_8)
   {
      static int msgIdx = 0;
      ret = hiAccChkMsg(hiAccCb.tstMsgs[msgIdx].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received RSVP msg is corrupted!!! \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.result = HI_TST_FAILED;
         hiAccCb.curTst.errFlag = TRUE;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      
      rsvpMsgNum ++;
      if(rsvpMsgNum == 1)
      { 
         /* First RSVP msg will have Router Alert option set to TRUE */
         /* Chk the incoming IPv4 HdrParam to see if router alert opt is set */
         /* if the router alert option is not set then this is an error */
         if(ipHdrParm->u.hdrParmIpv4.ipv4HdrOpt.pres != TRUE)
         { 
            sprintf(l5prntBuf,"First Rsvp Msg has not Router Alert opt SET \n");
            SPrint(l5prntBuf);
            hiAccCb.curTst.result = HI_TST_FAILED;
            hiAccCb.curTst.errFlag = TRUE;
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         } 
      }  
      else
      {
         /* Router Alert option is not present in 2nd Rsvp msg */ 
         /* Check to make sure that router alert option is not set */
         /* if the router alert option is set then this is an error */

         if(ipHdrParm->u.hdrParmIpv4.ipv4HdrOpt.pres != FALSE)
         {
            sprintf(l5prntBuf,
                   "Second Rsvp Msg should not have Router Alert opt SET \n");
            SPrint(l5prntBuf);
            hiAccCb.curTst.result = HI_TST_FAILED;
            hiAccCb.curTst.errFlag = TRUE;
            SPutMsg(mBuf);
            RETVALUE(RFAILED);
         }
         rsvpMsgNum = 0;
      }

#ifdef LOCAL_INTF
      /* print local interface address if it is there */
      if (localIf->intfPrsnt == TRUE)
      {   
         sprintf(l5prntBuf,"Received Pkt %d: Intf Index: %ld  Intf Addr: %lx ",
             rsvpMsgNum, localIf->localIf, localIf->localIfAddr.u.ipv4NetAddr);
         SPrint(l5prntBuf);

         sprintf(l5prntBuf,"\n");
         SPrint(l5prntBuf);
      }
#endif /* LOCAL_INTF */ 
      
      hiAccCb.curTst.state++;
   }
#endif /* IPV4_OPTS_SUPPORTED */
   
#ifdef IPV6_SUPPORTED
   if (hiAccCb.curTst.id == HI_TSTID_9)
   {
      ret = hiAccChkMsg(hiAccCb.tstMsgs[0].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received corrupt echo \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.state  = HI_ACC_ST_END;
         hiAccCb.curTst.result = HI_TST_FAILED;
         hiAccCb.curTst.errFlag = TRUE;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }

#ifdef LOCAL_INTF      
      /* print local interface address if it is there */
      if (localIf->intfPrsnt == TRUE)
      {   
         U8 l;
         /* print intf index */   
         sprintf(l5prntBuf, "  Recvd IPv6 PKTINFO: Intf Index: %ld Intf Addr:",
                 localIf->localIf);
         SPrint(l5prntBuf);
   
         /* print intf address */
         for(l=0; l < 16; l++)
         {   
            sprintf(l5prntBuf, "%x",
                    localIf->localIfAddr.u.ipv6NetAddr[l]);
            SPrint(l5prntBuf);
         }
   
         /* print intf name */
         sprintf(l5prntBuf,"\n");
         SPrint(l5prntBuf);
      } 
#endif /* LOCAL_INTF */
      
      if(ipHdrParm->u.hdrParmIpv6.ttl.pres == TRUE)
      {
         sprintf(l5prntBuf, "  HopLimit Received: %d\n", 
         ipHdrParm->u.hdrParmIpv6.ttl.val);
         SPrint(l5prntBuf);
      }

      if((srcAddr != NULLP) && (srcAddr->type == CM_TPTADDR_IPV6))
      {  
         U8 k;
         sprintf(l5prntBuf, "  Source addr of the pkt: "); 
         SPrint(l5prntBuf);
         for(k = 0; k < 16; k++)
         { 
            sprintf(l5prntBuf, "%x",
                       (srcAddr->u.ipv6TptAddr.ipv6NetAddr[k]));
            SPrint(l5prntBuf);
         }
         sprintf(l5prntBuf, "\n");            
         SPrint(l5prntBuf);
      }

      hiAccCb.curTst.state++;
   }
  
   if (hiAccCb.curTst.id == HI_TSTID_10)
   {
      ret = hiAccChkMsg(hiAccCb.tstMsgs[0].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received corrupt echo \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.state  = HI_ACC_ST_END;
         hiAccCb.curTst.result = HI_TST_FAILED;
         hiAccCb.curTst.errFlag = TRUE;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }

      hiAccCb.curTst.state++;
   }
   
#if (defined(IPV6_OPTS_SUPPORTED) && defined(HI_RSVP_SUPPORT))     
   if (hiAccCb.curTst.id == HI_TSTID_12)
   {
      static int msgIdx = 0;
      ret = hiAccChkMsg(hiAccCb.tstMsgs[msgIdx].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received RSVP msg is corrupted!!! \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.result = HI_TST_FAILED;
         hiAccCb.curTst.errFlag = TRUE;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      
      /* chk if HBH is really present in received ipHdrParm */
      if(ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhHdrPrsnt == TRUE)
      {
         sprintf(l5prntBuf, "  HopByHop Option Header Received\n");
         SPrint(l5prntBuf);
         /* print all the HBH options received */
         for (i=0; 
              i < ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.numHBHOpts; 
              i++)
         {
            sprintf(l5prntBuf, "   HBH Option #%d :: ",i);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Type: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.hbhOpts[i].
                                                                          type);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Length: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.hbhOpts[i].
                                                                        length);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Value: %d%d\n",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.
                                                            hbhOpts[i].value[1],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.
                                                           hbhOpts[i].value[0]);
            SPrint(l5prntBuf);
         }
      }
      
      if(ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsPrsnt == TRUE)
      {
         sprintf(l5prntBuf, "  Dest Option Header Received\n");
         SPrint(l5prntBuf);
         /* print all the Dest Opt received */
         for (i=0; 
              i < ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.numDestOpts; 
              i++)
         {
            sprintf(l5prntBuf, "   Dest Option #%d :: ",i);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Type: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                              destOpts[i].type);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Length: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                            destOpts[i].length);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Value: %d%d%d%d\n",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                           destOpts[i].value[3],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                           destOpts[i].value[2],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                           destOpts[i].value[1],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                          destOpts[i].value[0]);
            SPrint(l5prntBuf);
         }
      }

      if(ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOptsPrsnt == TRUE)
      {
         sprintf(l5prntBuf, "  Route Hdr Received\n");
         SPrint(l5prntBuf);
         sprintf(l5prntBuf, "   Total # of IPv6 addrs in the route is %d\n",
                 ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.numAddrs);  
         SPrint(l5prntBuf);
         
         sprintf(l5prntBuf, "   strict/loose bit map: 0x%lx\n",
                 *(&ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.slMap));
         SPrint(l5prntBuf);
         
         /* print all IPv6 Adresses in the Route Header */
         for (i=0; 
              i < ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.numAddrs; 
              i++)
         {
            sprintf(l5prntBuf, "   IPv6 address %x : ", i+1);            
            SPrint(l5prntBuf);
         
            for(j=0; j < 16; j++)
            {   
               sprintf(l5prntBuf, "%x",
                       (ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.
                                                ipv6Addrs[i][j]));
               SPrint(l5prntBuf);
            }
            sprintf(l5prntBuf, "\n");            
            SPrint(l5prntBuf);
         }   
      }

#ifdef LOCAL_INTF      
      /* print local interface address if it is there */
      if (localIf->intfPrsnt == TRUE)
      {  
         U8 l;
         /* print intf index */   
         sprintf(l5prntBuf, "  Recvd IPv6 PKTINFO: Intf Index: %ld Intf Addr:",
                 localIf->localIf);
         SPrint(l5prntBuf);
   
         /* print intf address */
         for(l=0; l < 16; l++)
         {   
            sprintf(l5prntBuf, "%x",
                    localIf->localIfAddr.u.ipv6NetAddr[l]);
            SPrint(l5prntBuf);
         }
   
         /* print intf name */
         sprintf(l5prntBuf,"\n");
         SPrint(l5prntBuf);
      }
#endif /* LOCAL_INTF */
      
      if(ipHdrParm->u.hdrParmIpv6.ttl.pres == TRUE)
      {
         sprintf(l5prntBuf, "  HopLimit Received: %d\n", 
         ipHdrParm->u.hdrParmIpv6.ttl.val);
         SPrint(l5prntBuf);
      }
     
      if((srcAddr != NULLP) && (srcAddr->type == CM_TPTADDR_IPV6))
      {  
         sprintf(l5prntBuf, "  Source addr of the pkt: "); 
         SPrint(l5prntBuf);
         for(j = 0; j < 16; j++)
         { 
            sprintf(l5prntBuf, "%x",
                       (srcAddr->u.ipv6TptAddr.ipv6NetAddr[j]));
            SPrint(l5prntBuf);
         }
         sprintf(l5prntBuf, "\n");            
         SPrint(l5prntBuf);
      }
      /* Deallocate all memory in the ipHdrParams structure */
      ipv6HdrParm = (CmInetIpv6HdrParm *)&ipHdrParm->u.hdrParmIpv6;
      if(ipHdrParm->type == CM_HDRPARM_IPV6)
         CM_INET_FREE_IPV6_HDRPARM(hiAccCb.init.region, hiAccCb.init.pool, 
                                   ipv6HdrParm);
      hiAccCb.curTst.state++;
   }
   
   if (hiAccCb.curTst.id == HI_TSTID_13)
   {
      static int msgIdx = 0;
      ret = hiAccChkMsg(hiAccCb.tstMsgs[msgIdx].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received RSVP msg is corrupted!!! \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.result = HI_TST_FAILED;
         hiAccCb.curTst.errFlag = TRUE;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      
      /* chk if HBH is really present in received ipHdrParm */
      if(ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhHdrPrsnt == TRUE)
      {
         sprintf(l5prntBuf, "HopByHop Option Header Received\n");
         SPrint(l5prntBuf);
         /* print all the HBH options received */
         for (i=0; 
              i < ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.numHBHOpts; 
              i++)
         {
            sprintf(l5prntBuf, "  HBH Option #%d :: ",i);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Type: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.hbhOpts[i].
                                                                          type);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Length: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.hbhOpts[i].
                                                                        length);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Value: %d%d\n",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.
                                                            hbhOpts[i].value[1],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.hbhOptsArr.
                                                           hbhOpts[i].value[0]);
            SPrint(l5prntBuf);
         }
      }   

      /* chk if Dest Option is really present in received ipHdrParm */
      if(ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsPrsnt == TRUE)
      {
         sprintf(l5prntBuf, "Dest Option Header Received\n");
         SPrint(l5prntBuf);
         /* print all the Destination options received */
         for (i=0; 
              i < ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.numDestOpts; 
              i++)
         {
            sprintf(l5prntBuf, "  Dest Option #%d :: ",i);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Type: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                              destOpts[i].type);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Length: %d  ",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                            destOpts[i].length);
            SPrint(l5prntBuf);
            sprintf(l5prntBuf, "Value: %d%d%d%d\n",
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                           destOpts[i].value[3],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                           destOpts[i].value[2],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                           destOpts[i].value[1],
                    ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.destOptsArr.
                                                          destOpts[i].value[0]);
            SPrint(l5prntBuf);
         }
      }

      if(ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOptsPrsnt == TRUE)
      {
         sprintf(l5prntBuf, "Route Hdr Received\n");
         SPrint(l5prntBuf);
         sprintf(l5prntBuf, "  Total # of IPv6 addrs in the route is %d\n",
                 ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.numAddrs);  
         SPrint(l5prntBuf);
         
         sprintf(l5prntBuf, "  strict/loose bit map: 0x%lx\n",
                 *(&ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.slMap));
         SPrint(l5prntBuf);
         
         /* print all IPv6 Adresses in the Route Header */
         for (i=0; 
              i < ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.numAddrs; 
              i++)
         {
            sprintf(l5prntBuf, "  IPv6 address %d : ", i+1);            
            SPrint(l5prntBuf);
         
            for(j=0; j < 16; j++)
            {   
               sprintf(l5prntBuf, "%x",
                       (ipHdrParm->u.hdrParmIpv6.ipv6ExtHdr.rtOpts.
                                                              ipv6Addrs[i][j]));
               SPrint(l5prntBuf);
            }
            sprintf(l5prntBuf, "\n");            
            SPrint(l5prntBuf);
         }   
      }

#ifdef LOCAL_INTF      
      /* print local interface address if it is there */
      if (localIf->intfPrsnt == TRUE)
      {   
         U8 l;
         /* print intf index */   
         sprintf(l5prntBuf, "Recvd IPv6 PKTINFO: Intf Index: %ld Intf Addr:",
                 localIf->localIf);
         SPrint(l5prntBuf);
   
         /* print intf address */
         for(l=0; l < 16; l++)
         {   
            sprintf(l5prntBuf, "%x",
                    localIf->localIfAddr.u.ipv6NetAddr[l]);
            SPrint(l5prntBuf);
         }
   
         /* print intf name */
         sprintf(l5prntBuf,"\n");
         SPrint(l5prntBuf);
      }
#endif /* LOCAL_INTF */      

      if(ipHdrParm->u.hdrParmIpv6.ttl.pres == TRUE)
      {
         sprintf(l5prntBuf, "HopLimit Received: %d\n", 
         ipHdrParm->u.hdrParmIpv6.ttl.val);
         SPrint(l5prntBuf);
      }
      
      /* Deallocate all memory in the ipHdrParams structure */
      ipv6HdrParm = (CmInetIpv6HdrParm *)&ipHdrParm->u.hdrParmIpv6; 
      if(ipHdrParm->type == CM_HDRPARM_IPV6)
         CM_INET_FREE_IPV6_HDRPARM(hiAccCb.init.region, hiAccCb.init.pool, 
                                   ipv6HdrParm);
      hiAccCb.curTst.state++;
   }
#endif /* IPV6_OPTS_SUPPORTED */    
#endif /* IPV6_SUPPORTED */
 
   if((hiAccCb.curTst.id == HI_TSTID_3) || (hiAccCb.curTst.id == HI_TSTID_6))
   {
      ret = hiAccChkMsg(hiAccCb.tstMsgs[hiAccCb.curTst.msgIdx].msg, mBuf);
      if(ret != ROK)
      {
         sprintf(l5prntBuf,"received corrupt echo \n");
         SPrint(l5prntBuf);
         hiAccCb.curTst.result = HI_TST_FAILED;
         hiAccCb.curTst.errFlag = TRUE;
         SPutMsg(mBuf);
         RETVALUE(RFAILED);
      }
      hiAccCb.curTst.state++;
   }
/*hi005.201 Added timer based resource checking in case of conjestion */
#ifdef HI_CONG_CHECK_TMR
   if((hiAccCb.curTst.id == HI_TSTID_45))
	{
	   if ( hiAccCb.conjBuffer == NULLP ) 
		{
			hiAccCb.conjBuffer = mBuf;
			RETVALUE(ROK);
		}
		SCatMsg((hiAccCb.conjBuffer), mBuf, M1M2);
      SPutMsg(mBuf);

		RETVALUE(ROK);
	}
#endif /* HI_CONG_CHECK_TMR */

   SPutMsg(mBuf);
   
   RETVALUE(ROK);
} /* end of HcLiHitUDatInd */

/*
*
*       Fun:   Disconnect Indication
*
*       Desc:  This primitive is used by the TCP UDP Convergence
*              Layer to inform the service user that the socket 
*              is being closed by TCP UDP Convergence Layer and 
*              the associated connection information is being released. 
*              This may be issued in error cases  (e.g. socket related 
*              errors  like error in opening, binding, connecting to a 
*              socket, or miscellaneous errors like errors encountered 
*              in hash list operations or error in allocating static 
*              memory.) . 
*
*              "reason" - indicates the reason for closure
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  layer5.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitDiscInd
(
Pst *pst,                 /* post stucture */
SuId suId,                /* service user Id */
U8  choice,               /* choice parameter */
UConnId conId,            /* connection Id */
Reason reason             /* reason */
)
#else
PUBLIC S16 HcLiHitDiscInd(pst, suId, choice, conId, reason)
Pst *pst;                 /* post stucture */
SuId suId;                /* service user Id */
U8  choice;               /* choice parameter */
UConnId conId;            /* connection Id */
Reason reason;            /* reason */
#endif
{

   TRC3(HcLiHitDiscInd)

   if(hiAccCb.curTst.prntFlag)
   {
     sprintf(l5prntBuf, "received HcLiHitDiscInd from TUCL \n");
     SPrint(l5prntBuf);
            /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
     sprintf(l5prntBuf, "suId(%d), conId(%ld), reason(%d)\n", suId, conId, 
             reason);
#else
     sprintf(l5prntBuf, "suId(%d), conId(%d), reason(%d)\n", suId, conId, 
             reason);
#endif
     SPrint(l5prntBuf);
   }

   if ((hiAccCb.curTst.id == HI_TSTID_9) ||
       (hiAccCb.curTst.id == HI_TSTID_11))
      hiAccCb.curTst.state ++;

   if(hiAccCb.curTst.id == HI_TSTID_5)
      hiAccCb.curTst.state ++;

   RETVALUE(ROK);
} /* end of HcLiHitDiscInd */


/*
*
*       Fun:   Disconnect Confirmation
*
*       Desc:  This primitive is used by the TCP UDP COnvergence  
*              Layer to indicate to the service user that 
*              Disconnect Request issued by  the service user 
*              for the action was successful.
*
*       Ret:   ROK
*
*       Notes: Please note that DiscCfm does not necessarily imply 
*              that the connection has  been closed or the connection 
*              information has been released by the HI layer ( as the 
*              socket may have been put into a shutdown state )
* 
*       File:  layer5.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitDiscCfm
(
Pst *pst,                 /* post stucture */
SuId suId,                /* service user Id */
U8  choice,               /* choice parameter */
UConnId conId,            /* connection Id */
Action action             /* action type */
)
#else
PUBLIC S16 HcLiHitDiscCfm(pst, suId, choice, conId, action)       
Pst *pst;                 /* post stucture */
SuId suId;                /* service user Id */
U8  choice;               /* choice parameter */
UConnId conId;            /* connection Id */
Action action;            /* action type */
#endif
{
   static int numDiscCfmRcvd = 0;

   TRC3(HcLiHitDiscCfm)

   if(hiAccCb.curTst.prntFlag)
   {
     sprintf(l5prntBuf, "received HcLiHitDiscCfm from TUCL \n");
     SPrint(l5prntBuf);
   }

   if((hiAccCb.curTst.id == HI_TSTID_0) ||
      (hiAccCb.curTst.id == HI_TSTID_3) ||
      (hiAccCb.curTst.id == HI_TSTID_4) ||
      (hiAccCb.curTst.id == HI_TSTID_5) ||
      (hiAccCb.curTst.id == HI_TSTID_6) ||
      (hiAccCb.curTst.id == HI_TSTID_11) ||
      (hiAccCb.curTst.id == HI_TSTID_18) ||
      (hiAccCb.curTst.id == HI_TSTID_19) ||
      (hiAccCb.curTst.id == HI_TSTID_45)) 
   {
      hiAccCb.curTst.state ++;
   }
      
   if(hiAccCb.curTst.id == HI_TSTID_1)
   {
      if (hiAccCb.curTst.state == 8)
         hiAccCb.curTst.state ++;
   }

   if(hiAccCb.curTst.id == HI_TSTID_2)
   {
      numDiscCfmRcvd += 1;

      if (numDiscCfmRcvd == HIACC_UDP_MAX_SERV)
      {
         numDiscCfmRcvd = 0;
         hiAccCb.curTst.state ++;
      }
   }

   if ((hiAccCb.curTst.id == HI_TSTID_7) ||
       (hiAccCb.curTst.id == HI_TSTID_8)) 
       
      hiAccCb.curTst.state ++;

#ifdef IPV6_SUPPORTED 
   if ((hiAccCb.curTst.id == HI_TSTID_9) || (hiAccCb.curTst.id == HI_TSTID_10))
      hiAccCb.curTst.state ++;
   
#if (defined(IPV6_OPTS_SUPPORTED) && defined(HI_RSVP_SUPPORT))     
   if ((hiAccCb.curTst.id == HI_TSTID_12) || 
       (hiAccCb.curTst.id == HI_TSTID_13))
      hiAccCb.curTst.state ++;
#endif /* IPV6_OPTS_SUPPORTED */   
   
#endif /* IPV6_SUPPORTED */

   RETVALUE(ROK);
} /* end of HcLiHitDiscCfm */


/*
*
*       Fun:   Flow Control Indication 
*
*       Desc:  This primitive is used to indicate a shortage of
*              kernel buffers to the HI layer users. The service users
*              are expected not to give any Data Requests to the HI
*              layer while flow control in ON.
*
*       Ret:   ROK
*
*       Notes: None 
*
*       File:  layer5.c
*
*/
#ifdef ANSI
PUBLIC S16 HcLiHitFlcInd 
(
Pst         *pst,         /* post structure */
SuId        suId,         /* service user Id */
UConnId     suConId,      /* service user connection Id */
Reason      reason        /* reason for flow control */
)
#else
PUBLIC S16 HcLiHitFlcInd(pst, suId, suConId, reason)
Pst         *pst;         /* post structure */
SuId        suId;         /* service user Id */
UConnId     suConId;      /* service user connection Id */
Reason      reason;       /* reason for flow control */
#endif /* ANSI */
{

   TRC3(HcLiHitFlcInd)
   if(hiAccCb.curTst.prntFlag)
   {
     sprintf(l5prntBuf, "received HcLiHitFlcInd from TUCL \n");
     SPrint(l5prntBuf);
   }

   HCLOGERROR(ERRCLS_DEBUG, EHC034, (ErrVal)0,
              "layer5.c: Received HitFlcInd from TUCL");
   RETVALUE(ROK);
} /* end of HcLiHitFlcInd */


#ifdef HI_LKSCTP
/*
*
*       Fun:   SCT interface Bind Confirmation
*
*       Desc:  This function indicates to the service user whether the 
*              bind operation has been successful or not.
*              
*       Ret:   ROK
*
*       Notes: None
*
*       File:  layer5.c
*
*/
  
#ifdef ANSI
PUBLIC S16 HcLiSctBndCfm
(
Pst         *pst,         /* post structure */
SuId        suId,         /* Service User Id */
U16         status
)
#else
PUBLIC S16 HcLiSctBndCfm(pst, suId, status)
Pst         *pst;         /* post structure */
SuId        suId;         /* Service User Id */
U16         status;
#endif
{
   TRC3(HcLiSctBndCfm)

   sprintf(l5prntBuf,"\tReceived Bind Confirm from TUCL \n");
   SPrint(l5prntBuf);
   if(status == CM_BND_OK)
   {
        sprintf(l5prntBuf,"\tBind Successful suId(%d)\n",suId);
        SPrint(l5prntBuf);
   }
   else
   {
        sprintf(l5prntBuf,"\tOooops Bind Failed suId(%d)\n",suId);
        SPrint(l5prntBuf);
   }

   /* Initialization complete
    * proceed with test cases 
    */
   hiAccCb.cfgDone = TRUE;  

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
} /* end of HcLiSctBndCfm */

/*
*
*       Fun:   End point open confirmation 
*
*       Desc:  This function indicates to the service user whether the 
*              end point open operation has been successful or not.
*              
*       Ret:   ROK/RFAILED
*
*       Notes: None
*
*       File:  layer5.c
*
*/
  
#ifdef ANSI
PUBLIC S16 HcLiSctEndpOpenCfm
(
Pst                  *pst,
SuId                 suId,
UConnId              suEndpId,
UConnId              spEndpId,
SctResult            result,
SctCause             cause
)
#else
PUBLIC S16 HcLiSctEndpOpenCfm(pst, suId, suEndpId, spEndpId, result, cause)
Pst                  *pst;
SuId                 suId;
UConnId              suEndpId;
UConnId              spEndpId;
SctResult            result;
SctCause             cause;
#endif /* ANSI */
{

   sprintf(l5prntBuf,"\tReceived Endp Open Confirm from TUCL \n");
   SPrint(l5prntBuf);
   if(result == SCT_OK)
   {
        hiAccCb.curTst.spEndpId = spEndpId;
         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
        sprintf(l5prntBuf,"\tEndp Open Successful suEndpId(%ld) spEndpId(%ld)\n", suEndpId, spEndpId);
#else
        sprintf(l5prntBuf,"\tEndp Open Successful suEndpId(%d) spEndpId(%d)\n", suEndpId, spEndpId);
#endif
        SPrint(l5prntBuf);
   }
   else
   {
         /* hi029.201:Fix for compilation warning*/
#ifndef BIT_64
        sprintf(l5prntBuf,"\tEndp Open Failed suEndpId(%ld) spEndpId(%ld)\n", suEndpId, spEndpId);
#else
        sprintf(l5prntBuf,"\tEndp Open Failed suEndpId(%d) spEndpId(%d)\n", suEndpId, spEndpId);
#endif
        SPrint(l5prntBuf);
   }

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
} /* end of HcLiSctEndpOpenCfm */


/*
*
*       Fun:    End point close confirmation 
*
*       Desc:   prints the status confirm Endpoint close
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctEndpCloseCfm
(
Pst                  *pst,
SuId                 suId,
UConnId              suEndpId,
SctResult            result,
SctCause             cause
)
#else
PUBLIC S16 HcLiSctEndpCloseCfm(pst, suId, suEndpId, result, cause)
Pst                  *pst;
SuId                 suId;
UConnId              suEndpId;
SctResult            result;
SctCause             cause;
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived Endp Close Confirm from TUCL \n");
   SPrint(l5prntBuf);
   if(result == SCT_OK)
   {
#ifndef BIT_64
        sprintf(l5prntBuf,"\tEndp Close Successful suEndpId(%ld)\n", suEndpId);
#else
        sprintf(l5prntBuf,"\tEndp Close Successful suEndpId(%d)\n", suEndpId);
#endif
        SPrint(l5prntBuf);
   }
   else
   {
         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
        sprintf(l5prntBuf,"\tEndp Close Failed suEndpId(%ld)\n", suEndpId);
#else
        sprintf(l5prntBuf,"\tEndp Close Failed suEndpId(%d)\n", suEndpId);
#endif
        SPrint(l5prntBuf);
   }

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Associatoin establishment indication 
*
*       Desc:   prints the association establish parameters
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctAssocInd
(
Pst                *pst,
SuId               suId,
UConnId            suEndpId,
SctAssocIndParams  *assocParams,
Buffer             *vsInfo           /* VsInfo - not used any more */
)
#else
PUBLIC S16 HcLiSctAssocInd(pst, suId, suEndpId, assocParams, vsInfo)
Pst                *pst;
SuId               suId;
UConnId            suEndpId;
SctAssocIndParams *assocParams;
Buffer             *vsInfo;          /* VsInfo - not used any more */
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived Assoc Indication from TUCL \n");
   SPrint(l5prntBuf);

   hiAccCb.curTst.spAssocId = assocParams->t.cookieParams.spAssocId;
   hiAccCb.curTst.assocIndParams = *assocParams;

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tAssoc indication suEndpId(%ld) spAssocId(%ld)\n", suEndpId, assocParams->t.cookieParams.spAssocId);
#else
   sprintf(l5prntBuf,"\tAssoc indication suEndpId(%d) spAssocId(%d)\n", suEndpId, assocParams->t.cookieParams.spAssocId);
#endif
   SPrint(l5prntBuf);
   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Associatoin establishment confirmation 
*
*       Desc:   prints the association establishment confirmation
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef SCT2
#ifdef ANSI
PUBLIC S16 HcLiSctAssocCfm
(
Pst           *pst,
SuId          suId,
UConnId       suAssocId,
UConnId       spAssocId,
SctNetAddrLst *dstNAddrLst,
SctStrmId     inStrms,
SctPort       dstPort,
SctStrmId     outStrms,
Buffer        *vsInfo           /* VsInfo - not used any more */
)
#else
PUBLIC S16 HcLiSctAssocCfm(pst, suId, suAssocId, spAssocId, dstNAddrLst,inStrms, dstPort, 
                           outStrms, vsInfo)
Pst           *pst;
SuId          suId;
UConnId       suAssocId;
UConnId       spAssocId;
SctNetAddrLst *dstNAddrLst;
SctStrmId     inStrms;
SctPort       dstPort;
SctStrmId     outStrms;
Buffer        *vsInfo;          /* VsInfo - not used any more */
#endif /* ANSI */
#else
#ifdef ANSI
PUBLIC S16 HcLiSctAssocCfm
(
Pst           *pst,
SuId          suId,
UConnId       suAssocId,
UConnId       spAssocId,
SctNetAddrLst *dstNAddrLst,
SctPort       dstPort,
SctStrmId     outStrms,
Buffer        *vsInfo           /* VsInfo - not used any more */
)
#else
PUBLIC S16 HcLiSctAssocCfm(pst, suId, suAssocId, spAssocId, dstNAddrLst, dstPort, 
           outStrms, vsInfo)
Pst           *pst;
SuId          suId;
UConnId       suAssocId;
UConnId       spAssocId;
SctNetAddrLst *dstNAddrLst;
SctPort       dstPort;
SctStrmId     outStrms;
Buffer        *vsInfo;          /* VsInfo - not used any more */
#endif /* ANSI */
#endif /* SCT2 */
{
   sprintf(l5prntBuf,"\tReceived Assoc Confirm from TUCL \n");
   SPrint(l5prntBuf);

   hiAccCb.curTst.spAssocId = spAssocId;

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tAssoc established suAssocId(%ld) spAssocId(%ld)\n", suAssocId, spAssocId);
#else
   sprintf(l5prntBuf,"\tAssoc established suAssocId(%d) spAssocId(%d)\n", suAssocId, spAssocId);
#endif
   SPrint(l5prntBuf);

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Association Termination indication
*
*       Desc:   print the Termation indication cause/status
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctTermInd
(
Pst                  *pst,
SuId                 suId,
UConnId              assocId,
U8                   assocIdType,
SctStatus            status,
SctCause             cause,
SctRtrvInfo          *rtrvInfo
)
#else
PUBLIC S16 HcLiSctTermInd(pst, suId, assocId, assocIdType, status, cause, rtrvInfo)
Pst                  *pst;
SuId                 suId;
UConnId              assocId;
U8                   assocIdType;
SctStatus            status;
SctCause             cause;
SctRtrvInfo          *rtrvInfo;
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived Term indication from TUCL \n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tTerm indication assocId(%ld) assocIdType(%d), status(%d), cause(%d)\n", assocId, assocIdType, status, cause);
#else
   sprintf(l5prntBuf,"\tTerm indication assocId(%d) assocIdType(%d), status(%d), cause(%d)\n", assocId, assocIdType, status, cause);
#endif
   SPrint(l5prntBuf);

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Association Termination confirmation
*
*       Desc:   print the Termation confirmation cause/status
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctTermCfm
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
SctResult            result,
SctCause             cause
)
#else
PUBLIC S16 HcLiSctTermCfm(pst, suId, suAssocId, result, cause )
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
SctResult            result;
SctCause             cause;
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived Term cfm from TUCL \n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tTerm cfm suAssocId(%ld), result(%d), cause(%d)\n", suAssocId, result, cause);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tTerm cfm suAssocId(%d), result(%d), cause(%d)\n", suAssocId, result, cause);
   SPrint(l5prntBuf);
#endif

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Set Primary Address confirmation
*
*       Desc:   print the Set Primary Address result/cause
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctSetPriCfm
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
SctResult            result,
SctCause             cause
)
#else
PUBLIC S16 HcLiSctSetPriCfm(pst, suId, suAssocId, result, cause )
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
SctResult            result;
SctCause             cause;
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived setpri cfm from TUCL \n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tset primary addr cfm suAssocId(%ld) result(%d), cause(%d)\n", suAssocId, result, cause);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tset primary addr cfm suAssocId(%d) result(%d), cause(%d)\n", suAssocId, result, cause);
   SPrint(l5prntBuf);
#endif

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Heartbeat enable/disable confirmation
*
*       Desc:   print the Termation confirmation cause/status/result
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctHBeatCfm
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
CmNetAddr            *dstNAddr,
SctStatus            status,
SctResult            result,
SctCause             cause
)
#else
PUBLIC S16 HcLiSctHBeatCfm(pst, suId, suAssocId, dstNAddr, status, result, cause )
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
CmNetAddr            *dstNAddr;
SctStatus            status;
SctResult            result;
SctCause             cause;
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived Hbeat Confirm from TUCL \n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tHbeat confirm suAssocId(%ld) status(%d) result(%d)\n", suAssocId, status, result);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tHbeat confirm suAssocId(%d) status(%d) result(%d)\n", suAssocId, status, result);
   SPrint(l5prntBuf);
#endif

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Data indication
*
*       Desc:   print the data buffer
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctDatInd
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
SctStrmId            strmId,
SctDatIndType        *indType,
U32                  protId,
Buffer               *mBuf
)
#else
PUBLIC S16 HcLiSctDatInd(pst, suId, suAssocId, strmId, indType, protId, mBuf )
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
SctStrmId            strmId;
SctDatIndType        *indType;
U32                  protId;
Buffer               *mBuf;
#endif /* ANSI */
{
   Data      *databuf;
   MsgLen     len, cnt;
   U32        idx, ret;

#define BUFLEN   32

   sprintf(l5prntBuf,"\tReceived Data indication from TUCL\n");
   SPrint(l5prntBuf);

   /* allocate flat receive buffer */
   ret = SGetSBuf(hiAccCb.info.region, hiAccCb.info.pool, &databuf, BUFLEN);
   if (ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   SFndLenMsg(mBuf, &len);

   SCpyMsgFix(mBuf, 0, len, databuf, &cnt);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tData indication suAssocId(%ld) strmId(%d) datIndType(%d) protId(%ld) len(%d) cnt(%d)\n", suAssocId, strmId, indType->type, protId, len, cnt);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tData indication suAssocId(%d) strmId(%d) datIndType(%d) protId(%d) len(%d) cnt(%d)\n", suAssocId, strmId, indType->type, protId, len, cnt);
   SPrint(l5prntBuf);
#endif

   printf("\nDataBuf = ");
   for (idx = 0; idx < (U32)cnt; idx++)
      printf("%c", *(databuf + idx));

   printf("\n");

   hiAccCb.curTst.state++;

#undef BUFLEN

   RETVALUE(ROK);
}

/*
*
*       Fun:    Status  confirmation
*
*       Desc:   print the status confirmation results.
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctStaCfm
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
CmNetAddr            *dstNAddr,
SctResult            result,
SctCause             cause,
SctStaInfo           *staInfo
)
#else
PUBLIC S16 HcLiSctStaCfm(pst, suId, suAssocId, dstNAddr, result, cause,
                         staInfo)
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
CmNetAddr            *dstNAddr;
SctResult            result;
SctCause             cause;
SctStaInfo           *staInfo;
#endif /* ANSI */
{
   U32   idx;

   sprintf(l5prntBuf,"\tReceived Sta confirm from TUCL\n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tSta confirm suAssocId(%ld) result(%d)\n", suAssocId, result);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tSta confirm suAssocId(%d) result(%d)\n", suAssocId, result);
   SPrint(l5prntBuf);
#endif


   if (hiAccCb.curTst.id != 40 && result != SCT_OK)
      RETVALUE(RFAILED);

   switch(staInfo->staType)
   {
      case SCT_GET_ASSOC_INFO:
         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
         sprintf(l5prntBuf,"\tSta confirm Assoc info rwnd(%ld), connSta(%d), dstPort(%d), priDstAddr(%ld) dstAddrLst.nmb(%d)\n", 
                 staInfo->u.assocInfo.rwnd, staInfo->u.assocInfo.connSta, staInfo->u.assocInfo.dstPort, staInfo->u.assocInfo.priDstAddr.u.ipv4NetAddr, staInfo->u.assocInfo.dstAddrLst.nmb);
         SPrint(l5prntBuf);
#else
         sprintf(l5prntBuf,"\tSta confirm Assoc info rwnd(%d), connSta(%d), dstPort(%d), priDstAddr(%d) dstAddrLst.nmb(%d)\n", 
                 staInfo->u.assocInfo.rwnd, staInfo->u.assocInfo.connSta, staInfo->u.assocInfo.dstPort, staInfo->u.assocInfo.priDstAddr.u.ipv4NetAddr, staInfo->u.assocInfo.dstAddrLst.nmb);
         SPrint(l5prntBuf);
#endif

         for (idx = 0; idx < staInfo->u.assocInfo.dstAddrLst.nmb; idx++)
         {
         /*  hi029.201: Fix for compilation warning*/
#ifndef BIT_64
            sprintf(l5prntBuf,"\tdstAddrLst[%ld] %ld\n", idx, staInfo->u.assocInfo.dstAddrLst.nAddr[idx].u.ipv4NetAddr);  
            SPrint(l5prntBuf);
#else
            sprintf(l5prntBuf,"\tdstAddrLst[%d] %d\n", idx, staInfo->u.assocInfo.dstAddrLst.nAddr[idx].u.ipv4NetAddr);  
            SPrint(l5prntBuf);
#endif
         }
         break;

      case SCT_GET_ADDR_INFO:
         /*  hi029.201: Fix for compilation warning*/
#ifndef BIT_64
         sprintf(l5prntBuf,"\tSta confirm Addr info rtt(%d), cwnd(%ld), rto(%ld), active(%d)\n", 
                 staInfo->u.addrInfo.rtt, staInfo->u.addrInfo.cwnd, staInfo->u.addrInfo.rto, staInfo->u.addrInfo.active);
         SPrint(l5prntBuf);
#else
         sprintf(l5prntBuf,"\tSta confirm Addr info rtt(%d), cwnd(%d), rto(%d), active(%d)\n", 
                 staInfo->u.addrInfo.rtt, staInfo->u.addrInfo.cwnd, staInfo->u.addrInfo.rto, staInfo->u.addrInfo.active);
         SPrint(l5prntBuf);
#endif
         break;

      default:
         break;
   }

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Status indication
*
*       Desc:   print the status indication results
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*
*       File:   layer5.c
*
*/
/* hi019.201 : Added protId parameter */
#ifdef SCT7
#ifdef ANSI
PUBLIC S16 HcLiSctStaInd
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
UConnId              spAssocId,
CmNetAddr            *dstNAddr,
SctStatus            status,
SctCause             cause,
U32                  protId,
Buffer               *mBuf
)
#else
PUBLIC S16 HcLiSctStaInd(pst, suId, suAssocId, spAssocId, dstNAddr, status, cause, protId, mBuf)
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
UConnId              spAssocId;
CmNetAddr            *dstNAddr;
SctStatus            status;
SctCause             cause;
U32                  protId;
Buffer               *mBuf;
#endif /* ANSI */
#else /* SCT7 */
#ifdef ANSI
PUBLIC S16 HcLiSctStaInd
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
UConnId              spAssocId,
CmNetAddr            *dstNAddr,
SctStatus            status,
SctCause             cause,
Buffer               *mBuf
)
#else
PUBLIC S16 HcLiSctStaInd(pst, suId, suAssocId, spAssocId, dstNAddr, status, cause, mBuf)
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
UConnId              spAssocId;
CmNetAddr            *dstNAddr;
SctStatus            status;
SctCause             cause;
Buffer               *mBuf;
#endif /* ANSI */
#endif /* SCT7 */
{
   sprintf(l5prntBuf,"\tReceived Status indication from TUCL \n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tStatus indication suAssocId(%ld) spAssocId(%ld) status(%d)\n", suAssocId, spAssocId, status);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tStatus indication suAssocId(%d) spAssocId(%d) status(%d)\n", suAssocId, spAssocId, status);
   SPrint(l5prntBuf);
#endif

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}

/*
*
*       Fun:    Flow control Indication
*
*       Desc:   print the flow control results
*
*       Ret:    ROK     - succeeded
*               RFAILED - failed
*
*       Notes:  None
*
*       File:   layer5.c
*
*/

#ifdef ANSI
PUBLIC S16 HcLiSctFlcInd
(
Pst                  *pst,
SuId                 suId,
UConnId              suAssocId,
Reason               reason
)
#else
PUBLIC S16 HcLiSctFlcInd(pst, suId, suAssocId, reason )
Pst                  *pst;
SuId                 suId;
UConnId              suAssocId;
Reason               reason;
#endif /* ANSI */
{
   sprintf(l5prntBuf,"\tReceived flc indication from TUCL\n");
   SPrint(l5prntBuf);

         /* hi029.201: Fix for compilation warning*/
#ifndef BIT_64
   sprintf(l5prntBuf,"\tflow control indication suAssocId(%ld) reason(%d)\n", suAssocId, reason);
   SPrint(l5prntBuf);
#else
   sprintf(l5prntBuf,"\tflow control indication suAssocId(%d) reason(%d)\n", suAssocId, reason);
   SPrint(l5prntBuf);
#endif

   hiAccCb.curTst.state++;

   RETVALUE(ROK);
}
#endif


/*
*
*       Fun:   hcActvInit
*
*       Desc:  This function
*
*       Ret:   ROK
*              RFAILED
*
*       Notes: None 
*
*       File:  layer5.c
*
*/
#ifdef SS_MULTIPLE_PROCS /* hi002.105 */
#ifdef ANSI
PUBLIC S16 hcActvInit
(
ProcId procId,         /* procId */
Ent      ent,                 /* entity */
Inst     inst,                /* instance */
Region   region,              /* region */
Reason   reason,               /* reason */
Void **xxCb           /* Protocol Control Block */
)
#else
PUBLIC S16 hcActvInit(procId, ent, inst, region, reason, xxCb)
ProcId procId;         /* procId */
Ent      ent;                 /* entity */
Inst     inst;                /* instance */
Region   region;              /* region */
Reason   reason;              /* reason */
Void **xxCb;           /* Protocol Control Block */
#endif
#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI
PUBLIC S16 hcActvInit
(
Ent      ent,                 /* entity */
Inst     inst,                /* instance */
Region   region,              /* region */
Reason   reason               /* reason */
)
#else
PUBLIC S16 hcActvInit(ent, inst, region, reason)
Ent      ent;                 /* entity */
Inst     inst;                /* instance */
Region   region;              /* region */
Reason   reason;              /* reason */
#endif
#endif /* SS_MULTIPLE_PROCS */
{

   TRC2(hcActvInit)

   /* save initialization parameters */
 
   hiAccCb.init.ent    = ent;
   hiAccCb.init.inst   = inst;
   hiAccCb.init.region = region;
   hiAccCb.init.reason = reason;
   hiAccCb.init.procId = SFndProcId();
   hiAccCb.init.usta   = TRUE;
   hiAccCb.init.trc    = FALSE;
 
   RETVALUE(ROK);
}/* end of hcActvInit()*/


/********************************************************************30**
 
         End of file:     layer5.c@@/main/6 - Mon Mar  3 20:09:33 2008

*********************************************************************31*/
 
/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/
/********************************************************************70**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat     init                  description
----------- --------- ---- ----------------------------------------------
1.1         ---       asa  1. initial release.
1.1+        hi002.11  asa  1. miscellaneous changes.
/main/2     ---       cvp  1. miscellaneous bug fixes.
                           2. changed copyright header.
            /main/4   sb   1. changes for Raw socket interface.
                           2. flag added for backward compatibility.
/main/4     ---       cvp  1. Changes for IPv6.
                           2. Changed the copyright header.
/main/4+    hi009.104 mmh  1. included header files sht.h and sht.x for Rolling
                              Upgrade support.
                           2. added new arg in function HcLiHitUDatInd
                           3. print local inerface info for test case 2,  
                              in HcLiHitUDatInd, 8, 12, 13
                           4. print recvd data for newly added test 
                              cases 8, 12, 13
                           5. renumber test case ID for IPV6 related test cases
                           6. added tab before printing some strings
/main/5      ---       kp   1. Updated for release 1.5.
/main/5+     hi001.105 sg   1. Resolved compilation issues with IPV6 support
/main/5+     hi002.105 ss  1. SS_MULTIPLE_PROC flag added.
/main/6      ---       hs   1. Updated for release of 2.1
/main/6+      hi005.201 rss 1. Added timer based resource checking in case of conjestion 
/main/6+      hi019.201 rcs 1. Added protId in SctStaInd primitive.
/main/6+   hi029.201  ragrawal 1. Fix for 64 bit compilation warnings.
/main/6+   hi031.201  ragrawal 1. Fix for g++ compilation warnings.
*********************************************************************91*/
