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

     Name:    MME NAS Security Module

     Type:    C Source file

     Desc:    This file contains vb application message routing
              source code

     File:    vb_mme_sec.c

     Sid:     vb_mme_sec.c@@/main/1 - Thu Nov 17 19:35:38 2011

     Prg:     sk
*********************************************************************21*/


/* Header include files (.h)                                  */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent           */
#include "envind.h"        /* Environment independent         */

#include "gen.h"           /* General                         */
#include "ssi.h"           /* System services */
#include "ss_queue.h"      /* System services interface       */
#include "ss_mem.h"        /* System services interface       */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library            */
#include "vb_mme_sec.h"
#include "cm_emm.h"
#include <stdlib.h>        /* rand() */

/* header/extern include files (.x)                           */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "ss_queue.x"      /* System services interface       */
#include "ss_task.x"       /* System services interface       */
#include "ss_msg.x"        /* System services interface       */
#include "ss_mem.x"        /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "vb_mme_sec.x"
#include "vb_alg_intf.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/*Private NAS Seq Handling Functions*/
PRIVATE U16 vbMmeEstimatedSeqNmb ARGS((VbMmeNasSeqNmb *currSeqNmb,
                                       U8             rcvdNmb));

PRIVATE U16 vbMmeGetSeqNmb ARGS((VbMmeNasSeqNmb *currSeqNmb));

PRIVATE S16 vbMmeIsVldSeqNmb ARGS((VbMmeNasSeqNmb *currSeqNmb,
                                   U8             rcvdNmb));

PRIVATE S16 vbMmeIncSeqNmb ARGS((VbMmeNasSeqNmb *currSeqNmb));

PRIVATE S16 vbMmeSetSeqNmb ARGS((VbMmeNasSeqNmb *currSeqNmb,
                                 U8             rcvdNmb));


/*Private Utility Functions*/
PRIVATE S16 vbMmeGetSecHdr ARGS((VbMmeMsg     *srcMsg,
                                 VbMmeSecHdr  *secHdr));

PRIVATE S16 vbMmeGetEncBuf ARGS((VbMmeMsg *srcMsg,
                                 U8       hdrType,
                                 U8       **msg,
                                 U16      *msgLen));

PRIVATE S16 vbMmeGetIntBuf ARGS((VbMmeMsg *srcMsg,
                                 U8       hdrType,
                                 U8       **msg,
                                 U16      *msgLen));

PRIVATE S16 vbMmeVldUplnkSrvReqSec ARGS((VbMmeSecCtxt *secCtx,
                                         VbMmeMsg       *srcMsg,
                                         VbMmeMsg       *dstMsg));

/*Raw MAC Calculation and Ciphering Interface*/
PRIVATE S16 vbMmeCalcMac ARGS((VbMmeSecCtxt *secCtx,
                              U32            seqNmb,
                              U8             dir,
                              U8             bearer,
                              U8             *msg,
                              U16            msgLen,
                              U32            *mac));
PRIVATE S16 vbMmeEncMsg ARGS((VbMmeSecCtxt *secCtx,
                             U32            seqNmb,
                             U8             dir,
                             U8             bearer,
                             U8             *msg,
                             U16            msgLen));


/*Function Pointer Declarations*/
typedef Void (*VbMmeEncFnP)(U8 *key, U32 count, U8 dir, U8 bearer,
                            U8 *msg, U16 len);

typedef U32 (*VbMmeIntFnP)(U8 *key, U32 count, U8 dir, U8 bearer,
                           U8 *msg, U16 len);

typedef Void (*VbMmeKeyFnP)(U8 *key, U16 keyLen, U8 *s, U16 sLen,
                            U8 *output, U16 outputLen);


/*Global Function Pointers Initialization*/
VbMmeEncFnP vbMmeEncFnP[VB_MME_ENC_MAX_ALG] =
                          {&vbMmeEncNullAlg, 
                           &vbMmeEncSnow3GAlg, 
                           &vbMmeEncAESAlg, 
                           NULLP,
                           NULLP, 
                           NULLP, 
                           NULLP};

VbMmeIntFnP vbMmeIntFnP[VB_MME_INT_MAX_ALG] =
                          {&vbMmeIntNullAlg, 
                           &vbMmeIntSnow3GAlg, 
                           &vbMmeIntAESAlg, 
                           NULLP,
                           NULLP, 
                           NULLP, 
                           NULLP};

VbMmeKeyFnP vbMmeKeyFnP = &vbMmeKeyGenAlg;

/********************************************************************
 *
 *       Fun:   vbMmeSecHexDump
 *       
 *       Desc:  Prints hex valuse of buffer 
 *       
 *       Ret:  None 
 *       
 *       Notes: 
 *              
 *       File:  
 ********************************************************************/  
#ifdef ANSI
PUBLIC Void vbMmeSecHexDump
(
S8    *mod, 
S8    *name, 
S8    *file, 
U16   line, 
S8    *fun,
U8    *buf, 
U16   len
)
#else
PUBLIC Void vbMmeSecHexDump(mod, name, file, line, fun, buf, len)
(
S8    *mod;
S8    *name;
S8    *file; 
U16   line;
S8    *fun;
U8    *buf; 
U16   len;
)
#endif
{
  U16 cnt = 0;
  printf(" [%s] [%s:%d:%s] [HEX_DUMP]: %s[Len(%d bytes)]\n", mod, file, line, fun, name, len);
  printf(" [%s] [%s:%d:%s] [HEX_DUMP] [", mod, file, line, fun);
  for(cnt = 0; cnt < len; cnt++) 
  { 
     if((cnt != 0) &&(0 == cnt%16))
     { 
        printf(" ]\n [%s] [%s:%d:%s] [HEX_DUMP] [", mod, file, line, fun);
     } 
     else if((cnt != 0) &&(0 == cnt%8))
     { 
        printf("  "); 
     } 
     else if((cnt != 0) &&(0 == cnt%4))
     { 
        printf(" "); 
     } 
     printf(" %02x", buf[cnt]); 
  } 
  printf(" ]\n");
}

/********************************************************************
 *
 *       Fun:   vbMmeEstimatedSeqNmb
 *       
 *       Desc:  This function estimates sequence number based 
 *              on received sequence number
 *       
 *       Ret:   Estimated Seq Number
 *       
 *       Notes: 
 *              
 *       File:  
 ********************************************************************/  
#ifdef ANSI
PRIVATE U16 vbMmeEstimatedSeqNmb
(
VbMmeNasSeqNmb *currSeqNmb, 
U8             rcvdNmb
)
#else
PRIVATE U16 vbMmeEstimatedSeqNmb(currSeqNmb, rcvdNmb)
VbMmeNasSeqNmb *currSeqNmb;
U8              rcvdNmb;
#endif

{
   VB_MME_SEC_TRC2(vbMmeEstimatedSeqNmb)

   if(rcvdNmb >= currSeqNmb->seqNmb)
   {
      RETVALUE((currSeqNmb->ovrflwCnt << 8) | rcvdNmb);
   }
     
   /* We are at the edge roll over */
   if(currSeqNmb->seqNmb >= 250 ) /* 16 Seq Num Margin */
   {
      if(rcvdNmb <= 6)
      {
         RETVALUE(((currSeqNmb->ovrflwCnt+1) << 8) | rcvdNmb);
      } 
   }

   RETVALUE((currSeqNmb->ovrflwCnt << 8) | rcvdNmb);
}

/********************************************************************
 *
 *       Fun:   vbMmeGetSeqNmb
 *       
 *       Desc:  This function gives 2-Bytes length sequence number
 *       
 *       Ret:   Seq Number
 *       
 *       Notes: None
 *              
 *       File:  
 *******************************************************************/
#ifdef ANSI
PRIVATE U16 vbMmeGetSeqNmb
(
VbMmeNasSeqNmb *currSeqNmb
)
#else
PRIVATE U16 vbMmeGetSeqNmb(currSeqNmb)
VbMmeNasSeqNmb *currSeqNmb;
#endif
{
   VB_MME_SEC_TRC2(vbMmeGetSeqNmb)
   RETVALUE(((currSeqNmb->ovrflwCnt) <<8) | currSeqNmb->seqNmb); 
}


/********************************************************************
 *
 *       Fun:   vbMmeIsVldSeqNmb
 *       
 *       Desc:  Validates the received seq number w.r.t to current
 *              sequence number
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/              
#ifdef ANSI
PRIVATE S16 vbMmeIsVldSeqNmb
(
VbMmeNasSeqNmb *currSeqNmb, 
U8             rcvdNmb
)
#else
S16 vbMmeIsVldSeqNmb(currSeqNmb, rcvdNmb)
(
VbMmeNasSeqNmb *currSeqNmb; 
U8             rcvdNmb;
)
#endif
{
   VB_MME_SEC_TRC2(vbMmeIsVldSeqNmb)

   if(vbMmeGetSeqNmb(currSeqNmb) > vbMmeEstimatedSeqNmb(currSeqNmb, rcvdNmb))
   {
      RETVALUE(RFAILED);
   }
   else
   {
      RETVALUE(ROK);
   }
}

/********************************************************************
 *
 *       Fun:   vbMmeIncSeqNmb
 *       
 *       Desc:  Increments the sequence number
 *       
 *       Ret:   ROK
 *       
 *       Notes: None
 *******************************************************************/              
#ifdef ANSI
PRIVATE S16 vbMmeIncSeqNmb
(
VbMmeNasSeqNmb *currSeqNmb
)
#else
PRIVATE S16 vbMmeIncSeqNmb(currSeqNmb)
VbMmeNasSeqNmb *currSeqNmb;
#endif
{
   VB_MME_SEC_TRC2(vbMmeIncSeqNmb)

   if(currSeqNmb->seqNmb == 255)
   { 
      currSeqNmb->ovrflwCnt++;
      currSeqNmb->seqNmb = 0;
   }
   else
   {
      currSeqNmb->seqNmb++;
   }
   RETVALUE(ROK);
}


/********************************************************************
 *
 *       Fun:   vbMmeSetSeqNmb
 *       
 *       Desc:  Set the current sequence number based on received 
 *              sequence number
 *       
 *       Ret:   ROK
 *       
 *       Notes: None
 ********************************************************************/
#ifdef ANSI
PRIVATE S16 vbMmeSetSeqNmb
(
VbMmeNasSeqNmb *currSeqNmb, 
U8              rcvdNmb
)
#else
PRIVATE S16 vbMmeSetSeqNmb(currSeqNmb, rcvdNmb)
VbMmeNasSeqNmb *currSeqNmb; 
U8              rcvdNmb;
#endif
{
   S16 count = 0;

   VB_MME_SEC_TRC2(vbMmeSetSeqNmb)

   count = vbMmeEstimatedSeqNmb(currSeqNmb, rcvdNmb);

   currSeqNmb->ovrflwCnt = count >> 8;
   currSeqNmb->seqNmb = count & 0xFF;
   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeCompDLSec
 *       
 *       Desc:  Cipher and Integrity protect outgoing 
 *              NAS message at MME
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeCompDLSec
(
VbMmeSecCtxt *secCtx, 
U8             hdrType, 
VbMmeMsg       *srcMsg, 
VbMmeMsg       *dstMsg
)
#else
PUBLIC S16 vbMmeCompDLSec(secCtx, hdrType, srcMsg, dstMsg)
VbMmeSecCtxt *secCtx;
U8             hdrType; 
VbMmeMsg       *srcMsg; 
VbMmeMsg       *dstMsg;
#endif
{
   PRIVATE U8   msg[VB_MAX_INTG_COMP_MSG_LEN]  = {'\0'};
   U16          msgLen     = 0;      
   U8           bId        = 0;
   U8           dir        = 1;
   U32          seqNmb     = 0;
   U32          mac        = 0;
   S16          ret        = ROK;

   VB_MME_SEC_TRC1(vbMmeCompDLSec)

   if(VB_MME_SEC_CTXT_CRNT != secCtx->status)
   {
      VB_MME_SEC_PRNT_ERR(("Not A Current Security Context"));
      RETVALUE(RFAILED);
   }

   if(VB_MME_SEC_HT_PLAIN_NAS_MSG == hdrType)
   {
      VB_MME_SEC_PRNT_ERR(("Plain NAS Message Is Received"));
      RETVALUE(RFAILED);
   }

   /* Make a copy of original plain msg buffer */
   cmMemcpy(&msg[6], srcMsg->val, srcMsg->len);
   msgLen = srcMsg->len;

   /*Use current downlink sequnce number*/
   seqNmb = vbMmeGetSeqNmb(&secCtx->dlSeqNmb);

   if((VB_MME_SEC_HT_INT_PRTD_ENC == hdrType) ||
          (VB_MME_SEC_HT_INT_PRTD_ENC_NEW_SEC_CTXT == hdrType))
   {
      /*KWORKS Fix*/
      if((msgLen > 0) && (msgLen < VB_MAX_INTG_MSG_LEN))
      {
      ret = vbMmeEncMsg(secCtx, seqNmb, dir, bId, &msg[6], msgLen);

      if(ROK != ret)
      {
         VB_MME_SEC_PRNT_ERR(("Message Encryption Failed"));
         RETVALUE(RFAILED);
      }
   }
      else
      {
         VB_MME_SEC_PRNT_ERR(("Message to be Encoded is more than VB_MAX_INTG_MSG_LEN bytes."));
         RETVALUE(RFAILED);
      }
   }
   
   /*Use sequence number for MAC calculation*/
   msg[5] = (U8)(seqNmb & 0xff);
   msgLen++;

   if( msgLen > (VB_MAX_INTG_MSG_LEN+1)) /*1000 - 5*/
   {
      msgLen = VB_MAX_INTG_MSG_LEN+1; /*Klockworks fix*/
   }

   /*Calculate mac for encrypted message and sequence number*/
   ret = vbMmeCalcMac(secCtx, seqNmb, dir, bId, &msg[5], msgLen, &mac);
   if(ROK != ret) 
   {
      VB_MME_SEC_PRNT_ERR(("Mac Calculation Failed"));
      RETVALUE(RFAILED);
   }
   
   msg[0] = ((hdrType & 0x0f) << 4) | CM_EMM_PD;
   msg[1] = (U8)((mac>>24) & 0xff);
   msg[2] = (U8)((mac>>16) & 0xff);
   msg[3] = (U8)((mac>>8) & 0xff);
   msg[4] = (U8)((mac) & 0xff);

   printf("\n########MAC VALUE: %lu, msg[1]: %d, msg[2]: %d, msg[3]: %d, msg[4]: %d \n",mac,
            msg[1], msg[2],msg[3], msg[4]);

   msgLen = msgLen + 5;

   /*Static Memory Assigned*/ 
   dstMsg->val = &msg[0];
   dstMsg->len = msgLen;

   vbMmeIncSeqNmb(&secCtx->dlSeqNmb);

   RETVALUE(ROK);
}


/********************************************************************
 *
 *       Fun:   vbMmeVldULSec
 *       
 *       Desc:  Validate(Integrity check and decipher) incoming 
 *              NAS message at MME
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeVldULSec
(
VbMmeSecCtxt *secCtx, 
VbMmeMsg       *srcMsg, 
VbMmeMsg       *dstMsg
)
#else
PUBLIC S16 vbMmeVldULSec(secCtx, srcMsg, dstMsg)
VbMmeSecCtxt *secCtx; 
VbMmeMsg       *srcMsg; 
VbMmeMsg       *dstMsg;
#endif
{
   VbMmeSecHdr secHdr;
   U8          *msg          = NULLP;
   U16         msgLen        = 0;      
   U8          bId           = 0;
   U8          dir           = 0;
   U32         seqNmb        = 0;
   U32         mac           = 0;
   S16         ret           = ROK;
   PRIVATE U8  dstBuf[VB_MAX_INTG_COMP_MSG_LEN]  = {'\0'};
  
   VB_MME_SEC_TRC1(vbMmeVldULSec);

   if (VB_MME_SEC_CTXT_CRNT != secCtx->status)
   {
      VB_MME_SEC_PRNT_ERR(("Not A Current Security Context"));
      RETVALUE(RFAILED);
   }

   /* Get nas security header */
   vbMmeGetSecHdr(srcMsg , &secHdr);
   if (VB_MME_SEC_HT_PLAIN_NAS_MSG == secHdr.hdrType)
   {
      VB_MME_SEC_PRNT_ERR(("Plain NAS Message Is Received"));
      RETVALUE(RFAILED);
   }

   /*Service Request Is Special Case*/
   if (VB_MME_SEC_HT_SER_REQ == secHdr.hdrType)
   {
      VB_MME_SEC_PRNT_INFO(("NAS Message With Security Request \
                           Header Type Received"));
      ret = vbMmeVldUplnkSrvReqSec(secCtx, srcMsg, dstMsg);
      RETVALUE(ret);
   }

   if (RFAILED == vbMmeIsVldSeqNmb(&secCtx->ulSeqNmb, secHdr.seqNmb))
   {
      VB_MME_SEC_PRNT_ERR(("Invalid Sequence Number"));
      RETVALUE(RFAILED);
   }

   /* Adjust pointer pointing to seq num*/
   vbMmeGetIntBuf(srcMsg, secHdr.hdrType, &msg, &msgLen);

   /* Get estimation of uplink seq num*/
   seqNmb = vbMmeEstimatedSeqNmb(&secCtx->ulSeqNmb, secHdr.seqNmb);

   /* Store last used count */
   secCtx->lstUsdCnt = seqNmb;

   /* Calculate mac for received message */
   ret = vbMmeCalcMac(secCtx, seqNmb, dir, bId, msg, msgLen, &mac);
   if((ROK != ret) || (mac != secHdr.mac))
   {
      VB_MME_SEC_PRNT_ERR(("Mac Mismatch"));
#ifdef ALIGN_64BIT   
      VB_MME_SEC_PRNT_INFO(("Mac Rcvd:%d", secHdr.mac));
      VB_MME_SEC_PRNT_INFO(("Mac Calc:%d", mac));
#else
      VB_MME_SEC_PRNT_INFO(("Mac Rcvd:%ld", secHdr.mac));
      VB_MME_SEC_PRNT_INFO(("Mac Calc:%ld", mac));
#endif
      RETVALUE(RFAILED);
   }

   if ((VB_MME_SEC_HT_INT_PRTD_ENC == secHdr.hdrType) || \
       (VB_MME_SEC_HT_INT_PRTD_ENC_NEW_SEC_CTXT == secHdr.hdrType))
   {
      vbMmeGetEncBuf(srcMsg, secHdr.hdrType, &msg, &msgLen);

      /* Make a copy of original encrypted plain msg buffer */
      dstMsg->val = &dstBuf[0]; /*Static memory pointer*/
      cmMemcpy(dstMsg->val, msg, msgLen);
      dstMsg->len = msgLen;

      /*KWORKS Fix*/
      if((dstMsg->len > 0) && (dstMsg->len < VB_MAX_INTG_COMP_MSG_LEN))
      {
      ret = vbMmeEncMsg(secCtx, seqNmb, dir, bId, dstMsg->val, dstMsg->len);
      if(ROK != ret)
      {
            VB_MME_SEC_PRNT_ERR(("Message Encryption Failed"));
            RETVALUE(RFAILED);
         }
      }
      else
      {
         VB_MME_SEC_PRNT_ERR(("Message to be Encoded is more than VB_MAX_INTG_COMP_MSG_LEN bytes."));
         RETVALUE(RFAILED);
      }
   }
    
   vbMmeSetSeqNmb(&secCtx->ulSeqNmb, secHdr.seqNmb);
   vbMmeIncSeqNmb(&secCtx->ulSeqNmb);

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeCmpUplnkSrvReqSec
 *       
 *       Desc:  Validates Integrity protection incoming 
 *              Service Request NAS message at MME
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PRIVATE S16 vbMmeVldUplnkSrvReqSec
(
VbMmeSecCtxt *secCtx,
VbMmeMsg       *srcMsg,
VbMmeMsg       *dstMsg
)
#else
PUBLIC S16 vbMmeVldUplnkSrvReqSec(secCtx, srcMsg, dstMsg)
VbMmeSecCtxt *secCtx;
VbMmeMsg       *srcMsg;
VbMmeMsg       *dstMsg;
#endif
{
   U8          *msg         = NULLP;
   U16         msgLen       = 0;
   U8          bId          = 0;
   U8          dir          = 0;
   U32         seqNmb       = 0;
   U32         mac          = 0;
   S16         ret          = ROK;
   PRIVATE U8  dstBuf[1000] = {'\0'};

   VB_MME_SEC_TRC2(vbMmeVldUplnkSrvReqSec)

   seqNmb = vbMmeGetSeqNmb(&secCtx->ulSeqNmb);

   /*Received Sequence Nmuber Estimate*/
   seqNmb = ((seqNmb & (0xE0))| (srcMsg->val[1] & 0x1F));

   if(RFAILED == vbMmeIsVldSeqNmb(&secCtx->ulSeqNmb, seqNmb))
   {
      VB_MME_SEC_PRNT_ERR(("Invalid Sequnce Number"));
      RETVALUE(RFAILED);
   }

   /* Get estimation of uplink seq num*/
   seqNmb = vbMmeEstimatedSeqNmb(&secCtx->ulSeqNmb, seqNmb);

   /* Store last used count */
   secCtx->lstUsdCnt = seqNmb;

   /*Get Message*/
   msg = srcMsg->val;
   /*Pd+Ht+seq+ksi*/
   msgLen = 2;

   /* Calculate mac for received message */
   ret = vbMmeCalcMac(secCtx, seqNmb, dir, bId, msg, msgLen, &mac);

   if((ROK != ret))
   {
      VB_MME_SEC_PRNT_ERR(("MAC Calculation Failed"));
      RETVALUE(RFAILED);
   }

   if((mac & 0xFFFF) != ((msg[2] << 8) | msg[3]))
   {
      VB_MME_SEC_PRNT_ERR(("MAC Not Matching"));
      RETVALUE(RFAILED);
   }

   dstMsg->val = &dstBuf[0];/*Static memory pointer*/
   /* Make a copy of original plain msg buffer */
   /*Copy including short MAC*/
   cmMemcpy(dstMsg->val, msg, msgLen + 2); 
   dstMsg->len = msgLen + 2;

   vbMmeSetSeqNmb(&secCtx->ulSeqNmb, seqNmb);
   vbMmeIncSeqNmb(&secCtx->ulSeqNmb);

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeCalcMac
 *       
 *       Desc:  Calculates MAC
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PRIVATE S16 vbMmeCalcMac
(
VbMmeSecCtxt *secCtx, 
U32            seqNmb, 
U8             dir, 
U8             bearer, 
U8             *msg, 
U16            msgLen, 
U32            *mac
)
#else
PRIVATE S16 vbMmeCalcMac(secCtx, seqNmb, dir, bearer, msg, msgLen, mac)
VbMmeSecCtxt *secCtx;
U32            seqNmb; 
U8             dir;
U8             bearer; 
U8             *msg;
U16            msgLen; 
U32            *mac;
#endif
{
   VB_MME_SEC_TRC2(vbMmeCalcMac)

   if((secCtx->intAlg >= VB_MME_EIA_MAX) || \
      (NULLP == vbMmeIntFnP[secCtx->intAlg]))  
   {
      RETVALUE(RFAILED);
   }

   VB_MME_SEC_PRNT_HEX("IntKey", secCtx->intKey, VB_MME_SZ_NAS_INT_KEY);
#ifdef ALIGN_64BIT
   VB_MME_SEC_PRNT_INFO(("Seq Nmb:%x", seqNmb));
   VB_MME_SEC_PRNT_INFO(("Dir:%d", dir));
   VB_MME_SEC_PRNT_INFO(("Bearer:%d", bearer));
   VB_MME_SEC_PRNT_INFO(("IntAlg:%d", secCtx->intAlg));
#else
   VB_MME_SEC_PRNT_INFO(("Seq Nmb:%lx", seqNmb));
   VB_MME_SEC_PRNT_INFO(("Dir:%d", dir));
   VB_MME_SEC_PRNT_INFO(("Bearer:%d", bearer));
   VB_MME_SEC_PRNT_INFO(("IntAlg:%d", secCtx->intAlg));
#endif

   VB_MME_SEC_PRNT_HEX("Message", msg, msgLen);

   *mac = vbMmeIntFnP[secCtx->intAlg](secCtx->intKey, seqNmb, dir, bearer,\
         msg, msgLen);

   VB_MME_SEC_PRNT_INFO(("Mac:%ld", *mac));

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeEncMsg
 *       
 *       Desc:  Encrypt the message 
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PRIVATE S16 vbMmeEncMsg
(
VbMmeSecCtxt *secCtx, 
U32            seqNmb, 
U8             dir, 
U8             bearer, 
U8             *msg, 
U16            msgLen
)
#else
PRIVATE S16 vbMmeEncMsg(secCtx, seqNmb, dir, bearer, msg, msgLen)
VbMmeSecCtxt *secCtx; 
U32            seqNmb;
U8             dir;
U8             bearer; 
U8             *msg;
U16            msgLen;
#endif
{
   VB_MME_SEC_TRC2(vbMmeEncMsg)

   if ((secCtx->encAlg >= VB_MME_EEA_MAX) || \
      (NULLP == vbMmeEncFnP[secCtx->encAlg]))
   {
      RETVALUE(RFAILED);
   }

   VB_MME_SEC_PRNT_HEX("EncKey", secCtx->encKey, VB_MME_SZ_NAS_ENC_KEY);
   VB_MME_SEC_PRNT_INFO(("Seq Nmb:%ld", seqNmb));
   VB_MME_SEC_PRNT_INFO(("Dir:%x", dir));
   VB_MME_SEC_PRNT_INFO(("Bearer:%x", bearer));
   VB_MME_SEC_PRNT_HEX("Message", msg, msgLen);

   vbMmeEncFnP[secCtx->encAlg](secCtx->encKey, seqNmb, dir, bearer,\
         msg, msgLen);

   VB_MME_SEC_PRNT_HEX("Enc Message", msg, msgLen);

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeGenerateNasKey
 *       
 *       Desc:  Generate NAS encryption and integrity keys 
 *              
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
S16 vbMmeGenerateNasKey
(
VbMmeSecCtxt *secCtx
)
#else
S16 vbMmeGenerateNasKey(secCtx)
VbMmeSecCtxt *secCtx;
#endif
{
   U8 s[7] = {'\0'};

   VB_MME_SEC_TRC2(vbMmeGenerateNasKey)

   if(NULLP == vbMmeKeyFnP)
   {
      RETVALUE(RFAILED);
   }

   /*NAS INT Key Gen*/ 
   s[0]= 0x15;
   s[1]= 0x02;
   s[2]= 0;
   s[3]= 1;
   s[4]= secCtx->intAlg;
   s[5]= 0;
   s[6]= 1;

   VB_MME_SEC_PRNT_HEX("Kasme Key", secCtx->kasme, VB_MME_SZ_KASME_KEY);
   VB_MME_SEC_PRNT_HEX("S Key", s, 7);

   vbMmeKeyFnP(secCtx->kasme, VB_MME_SZ_KASME_KEY, s, 7, 
               secCtx->intKey, VB_MME_SZ_NAS_INT_KEY);

   VB_MME_SEC_PRNT_HEX("Int Key", secCtx->intKey, VB_MME_SZ_NAS_INT_KEY);

   /*NAS ENC Key Gen*/
   s[0]= 0x15;
   s[1]= 0x01;
   s[2]= 0;
   s[3]= 1;
   s[4]= secCtx->encAlg;
   s[5]= 0;
   s[6]= 1;

   VB_MME_SEC_PRNT_HEX("Kasme Key", secCtx->kasme, VB_MME_SZ_KASME_KEY);
   VB_MME_SEC_PRNT_HEX("S Key", s, 7);

   vbMmeKeyFnP(secCtx->kasme, VB_MME_SZ_KASME_KEY, s, 7, 
               secCtx->encKey, VB_MME_SZ_NAS_ENC_KEY);

   VB_MME_SEC_PRNT_HEX("Enc Key", secCtx->encKey, VB_MME_SZ_NAS_ENC_KEY);

   RETVALUE(ROK);
}


/********************************************************************
 *
 *       Fun:   vbMmeGenerateEnbKey
 *       
 *       Desc:  Generates ENB Keys
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeGenerateEnbKey
(
VbMmeSecCtxt *secCtx,
U32            count
)
#else
PUBLIC S16 vbMmeGenerateEnbKey(secCtx, count)
VbMmeSecCtxt *secCtx;
U32            count;
#endif
{
   U8 s[7] = {'\0'};

   VB_MME_SEC_TRC2(vbMmeGenerateEnbKey)

   if (NULLP == vbMmeKeyFnP)
   {
      RETVALUE(RFAILED);
   }

   s[0] = 0x11; /* FC */
   s[1] = (count >> 24) & 0xFF;
   s[2] = (count >> 16) & 0xFF;
   s[3] = (count >> 8) & 0xFF;
   s[4] = count & 0xFF;
   s[5] = 0x00;
   s[6] = 0x04;

   VB_MME_SEC_PRNT_HEX("Kasme Key", secCtx->kasme, VB_MME_SZ_KASME_KEY);
   VB_MME_SEC_PRNT_HEX("S Key", s, 7);

   vbMmeKeyFnP(secCtx->kasme, VB_MME_SZ_KASME_KEY, s, 7,\
         secCtx->enbKey, VB_MME_SZ_ENB_KEY);

   VB_MME_SEC_PRNT_HEX("eNB Key", secCtx->enbKey, VB_MME_SZ_ENB_KEY);
   RETVALUE(ROK);
}


/********************************************************************
 *
 *       Fun:   vbMmeGenerateMappedKasme
 *       
 *       Desc:  This function generates Mapped KASME 
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeGenerateMappedKasme
(
VbMmeSecCtxt *secCtx
)
#else
PUBLIC S16 vbMmeGenerateMappedKasme(secCtx)
   VbMmeSecCtxt *secCtx;
#endif
{

   U8    *ck = NULL;
   U8    *ik = NULL;
   U32   nonce = 0;
   U8    *kasme = NULL; 
   U8    key[VB_MME_SZ_NAS_INT_KEY + VB_MME_SZ_NAS_ENC_KEY];
   U8    s[14]={0};

   VB_MME_SEC_TRC2(vbMmeGenerateMappedKasme);

   nonce = rand(); 
   ck = secCtx->ck;
   ik = secCtx->ik;
   kasme = secCtx->kasme;
   
   s[0] = 0x18; /*FC = 0x18*/
   s[1] = (nonce>>24) & 0xFF;
   s[2] = (nonce>>16) & 0xFF;
   s[3] = (nonce>>8) & 0xFF;
   s[4] = (nonce) & 0xFF;
   s[5] = 0x00;
   s[6] = 0x04;

   cmMemcpy(key, ck, VB_MME_SZ_NAS_ENC_KEY);
   cmMemcpy((key + VB_MME_SZ_NAS_ENC_KEY), ik, VB_MME_SZ_NAS_INT_KEY);

   VB_MME_SEC_PRNT_HEX("Key(ck||ik)", key, 
      VB_MME_SZ_NAS_INT_KEY + VB_MME_SZ_NAS_ENC_KEY);
   VB_MME_SEC_PRNT_HEX("S", s, 7);

   /*Generates Sequence Number*/   
   vbMmeKeyGenAlg(key, VB_MME_SZ_NAS_INT_KEY + VB_MME_SZ_NAS_ENC_KEY,
                  s, 7, kasme, VB_MME_SZ_KASME_KEY);

   VB_MME_SEC_PRNT_HEX("Kasme", kasme, VB_MME_SZ_KASME_KEY);

   secCtx->nonce = nonce;

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeGenerateMappedSecKeys
 *       
 *       Desc:  This function generates Mapped Keys during interRAT HO 
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeGenerateMappedSecKeys
(
VbMmeSecCtxt *secCtx
)
#else
PUBLIC S16 vbMmeGenerateMappedSecKeys(secCtx)
   VbMmeSecCtxt *secCtx;
#endif
{

   VB_MME_SEC_TRC2(vbMmeGenerateMappedSecKeys);

   /*  No key is available (UE to Network) */
   /*secCtx->ksi = VB_MME_NO_KEY_AVAILABLE;*/
   secCtx->ksi = secCtx->ksi;
   /* Mapped security context (for KSI SGSN )*/
   secCtx->tsc = VB_MME_MAPPED_KEY; 
   /* Generate K'asme  */
   vbMmeGenerateMappedKasme(secCtx);
   /* Generate NAS security keys*/
   vbMmeGenerateNasKey(secCtx);
   /* Generate KENB */
   vbMmeGenerateEnbKey(secCtx, secCtx->lstUsdCnt);

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbMmeGenerateMappedCKIK
 *       
 *       Desc:  This function generates Mapped CK and IK 
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeGenerateMappedCKIK
(
VbMmeSecCtxt *secCtx,
VbMmeSecCtxt *mappedSecCtx
)
#else
PUBLIC S16 vbMmeGenerateMappedCKIK(secCtx)
VbMmeSecCtxt *secCtx;
VbMmeSecCtxt *mappedSecCtx;
#endif
{
   U8      s[7] = {'\0'};
   U32     count = 0;
   U8      outputKey[VB_MME_SZ_NAS_INT_KEY + VB_MME_SZ_NAS_ENC_KEY] = {0};

   VB_MME_SEC_TRC2(vbMmeGenerateMappedCKIK)

   if (NULLP == vbMmeKeyFnP)
   {
      RETVALUE(RFAILED);
   }

   /*Use current downlink sequnce number*/
   count = vbMmeGetSeqNmb(&secCtx->dlSeqNmb);

   s[0] = 0x16; /* FC */
   s[1] = (count >> 24) & 0xFF;
   s[2] = (count >> 16) & 0xFF;
   s[3] = (count >> 8) & 0xFF;
   s[4] = count & 0xFF;
   s[5] = 0x00;
   s[6] = 0x04;

   VB_MME_SEC_PRNT_HEX("Kasme Key", secCtx->kasme, VB_MME_SZ_KASME_KEY);
   VB_MME_SEC_PRNT_HEX("S Key", s, 7);

   vbMmeKeyGenAlg(secCtx->kasme, VB_MME_SZ_KASME_KEY, s, 7,\
         outputKey, VB_MME_SZ_NAS_INT_KEY + VB_MME_SZ_NAS_ENC_KEY);

   VB_MME_SEC_PRNT_HEX("Key(CK'||IK')", outputKey, 
      VB_MME_SZ_NAS_INT_KEY + VB_MME_SZ_NAS_ENC_KEY);

   cmMemcpy(mappedSecCtx->ck, outputKey,VB_MME_SZ_NAS_ENC_KEY);
   cmMemcpy(mappedSecCtx->ik, (outputKey + VB_MME_SZ_NAS_ENC_KEY),\
               VB_MME_SZ_NAS_INT_KEY);
      

   /* Increment the DL NAS Count */
   vbMmeIncSeqNmb(&secCtx->dlSeqNmb);

   RETVALUE(ROK);

}



/********************************************************************
 *
 *       Fun:   vbMmeGenerateNextHop
 *       
 *       Desc:  Generates ENB Keys
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes: None
 *******************************************************************/
#ifdef ANSI
PUBLIC S16 vbMmeGenerateNextHop
(
VbMmeSecCtxt   *secCtx,
U8             *syncInput
)
#else
PUBLIC S16 vbMmeGenerateNextHop(secCtx, syncInput)
VbMmeSecCtxt   *secCtx;
U8             *syncInput;
#endif
{
   U8 s[35] = {'\0'};

   VB_MME_SEC_TRC2(vbMmeGenerateNextHop);

   if (NULLP == vbMmeKeyFnP)
   {
      RETVALUE(RFAILED);
   }

   s[0] = 0x12;   /* FC */
   cmMemcpy(s + 1, syncInput, VB_MME_SZ_NEXT_HOP_KEY); /* SYNC */
   s[33] = 0x00;  /* length of SYNC */
   s[34] = 0x20;  /* length of SYNC */

   VB_MME_SEC_PRNT_HEX("Kasme Key", secCtx->kasme, VB_MME_SZ_KASME_KEY);
   VB_MME_SEC_PRNT_HEX("S Key", s, 7);

   secCtx->nxtHopChainCnt += 1;
   /*NCC is only 3bit once it reach 7 we need to reset*/
   if(secCtx->nxtHopChainCnt >= 8)
   {
      secCtx->nxtHopChainCnt = 0;
   }
   vbMmeKeyFnP(secCtx->kasme, VB_MME_SZ_KASME_KEY, s, 35,\
         secCtx->nxtHop, VB_MME_SZ_NEXT_HOP_KEY);

   VB_MME_SEC_PRNT_HEX("eNB Key", secCtx->nxtHop, VB_MME_SZ_NEXT_HOP_KEY);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 vbMmeGetSecHdr
(
VbMmeMsg     *srcMsg,
VbMmeSecHdr  *secHdr
)
#else
PRIVATE S16 vbMmeGetSecHdr()
VbMmeMsg     *srcMsg;
VbMmeSecHdr  *secHdr;
#endif
{
   VB_MME_SEC_TRC2(vbMmeGetSecHdr)

   secHdr->pd = srcMsg->val[0] & 0x0f;
   secHdr->hdrType = (srcMsg->val[0] & 0xf0) >> 4;
   secHdr->mac = srcMsg->val[1];
   secHdr->mac = (secHdr->mac <<8) | srcMsg->val[2];
   secHdr->mac = (secHdr->mac <<8) | srcMsg->val[3];
   secHdr->mac = (secHdr->mac <<8) | srcMsg->val[4];
   secHdr->seqNmb = srcMsg->val[5]; 
   
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 vbMmeGetIntBuf
(
VbMmeMsg *srcMsg, 
U8       hdrType,
U8       **msg,
U16      *msgLen
)
#else
PRIVATE S16 vbMmeGetIntBuf(srcMsg, hdrType, msg, msgLen)
VbMmeMsg *srcMsg;
U8       hdrType;
U8       **msg;
U16      *msgLen;
#endif
{
   VB_MME_SEC_TRC2(vbMmeGetIntBuf)

   *msg = (srcMsg->val) +
              (1 /*(pd+hdrType)*/+
               4 /*mac*/);
   *msgLen = srcMsg->len -
              (1 /*(pd+hdrType)*/+
               4 /*mac*/);

   UNUSED(hdrType);
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 vbMmeGetEncBuf
(
VbMmeMsg *srcMsg, 
U8       hdrType, 
U8       **msg, 
U16      *msgLen
)
#else
PRIVATE S16 vbMmeGetEncBuf(srcMsg, hdrType, msg, msgLen)
VbMmeMsg *srcMsg; 
U8       hdrType; 
U8       **msg;
U16      *msgLen;
#endif
{
   VB_MME_SEC_TRC2(vbMmeGetEncBuf)

   *msg = (srcMsg->val) +
              (1 /*(pd+hdrType)*/+
               4 /*mac*/+
               1 /*seqNmb*/);
   *msgLen = srcMsg->len -
              (1 /*(pd+hdrType)*/+
               4 /*mac*/+
               1 /*seqNmb*/);

   UNUSED(hdrType);
   RETVALUE(ROK);
}



#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
