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

     Name:    HSS Authentication Module

     Type:    C Source file

     Desc:    This file contains HSS authentication
              source code

     File:    vb_hss_auth.c

     Sid:     vb_hss_auth.c@@/main/1 - Thu Nov 17 19:35:38 2011

     Prg:     sk
*********************************************************************21*/

/* Header include files (.h)                                  */
#include <stdlib.h>
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
#include "vb_hss_auth.h"

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
#include "vb_alg_intf.x"
#include "vb_hss_auth.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */
/********************************************************************
 *
 *       Fun:   vbHssAuthcHexDump
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
PUBLIC Void vbHssAuthHexDump
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
PUBLIC Void vbHssAuthHexDump(mod, name, file, line, fun, buf, len)
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
  printf(" [%s] [%s:%d:%s] [HEX_DUMP]: %s[Len(%d bytes)]:\n", mod, file, line, fun, name, len);
  printf(" [");
  for(cnt = 0; cnt < len; cnt++)
  {
     if((cnt != 0) &&(0 == cnt%16))
     {
        printf(" ]\n [");
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
 *       Fun:   vbHssUpdSeqNmb 
 *       
 *       Desc:  This function updates sequence number based 
 *              on received sequence number
 *       
 *       Ret:   None
 *       
 *       Notes: Table based sequnce number maintainence is pending 
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbHssUpdSeqNmb
(
VbHssUeAuthInfo *ueInfo, 
U8              *seq
)
#else
PRIVATE Void vbHssUpdSeqNmb(ueInfo, seq)
VbHssUeAuthInfo *ueInfo; 
U8              *seq;
#endif
{
   VB_HSS_AUTH_TRC2(vbHssUpdSeqNmb)
   /*TODO:Table Based Implementation*/
   cmMemcpy(ueInfo->seqNmb, seq, VB_HSS_SEQ_NMB_SIZE);
   RETVOID;
}


/********************************************************************
 *
 *       Fun:   vbHssIncSeqNmb
 *       
 *       Desc:  This function increments sequence number by one 
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbHssIncSeqNmb
(
U8    *seqNmb
)
#else
PRIVATE Void vbHssIncSeqNmb(seqNmb)
U8    *seqNmb;
#endif
{
   U8 ind = 0;
   U8 bits = 0;
   Bool inc = FALSE;
   U8 indx = 0;
   VB_HSS_AUTH_TRC2(vbHssIncSeqNmb)
   
   ind  = seqNmb[VB_HSS_SEQ_NMB_SIZE - 1] & 0x1F;
   bits = (seqNmb[VB_HSS_SEQ_NMB_SIZE - 1] & 0xe0) >> 5;

   ind = (ind+1)%32;
   if(0x07 == bits)
   {
      bits = 0;
      inc = TRUE;
   }
   else
   {
      bits++;
   }

   seqNmb[VB_HSS_SEQ_NMB_SIZE - 1] = (bits << 5) | ind;

   for(indx = VB_HSS_SEQ_NMB_SIZE - 2; (indx >= 0) && (TRUE == inc); indx--)
   {
      if(0xFF == seqNmb[indx])
      {
         seqNmb[indx] = 0;
      }
      else
      {
         seqNmb[indx]++;
         inc = FALSE; 
      }
   }
   RETVOID;
}

/********************************************************************
 *
 *       Fun:   vbHssVldSeqNmb
 *       
 *       Desc:  This function validates received sequence vs 
 *              current sequence number 
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PRIVATE S16 vbHssVldSeqNmb
(
VbHssUeAuthInfo *ueInfo, 
U8              *seqRcvd
)
#else
PRIVATE S16 vbHssVldSeqNmb(ueInfo, seqRcvd)
VbHssUeAuthInfo *ueInfo; 
U8              *seqRcvd; 
#endif
{
   /*TODO:Table Based Implementation*/
   U8  seqExpected[VB_HSS_SEQ_NMB_SIZE];

   VB_HSS_AUTH_TRC2(vbHssVldSeqNmb)

   cmMemcpy(seqExpected, ueInfo->seqNmb, VB_HSS_SEQ_NMB_SIZE);
   vbHssIncSeqNmb(seqExpected);
   if(0 != cmMemcmp(seqRcvd, seqExpected, VB_HSS_SEQ_NMB_SIZE))
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbHssGenKasme
 *       
 *       Desc:  This function generates KASME 
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PRIVATE S16 vbHssGenKasme
(
U8           *ck,
U8           *ik,
U8           *ak,
U8           *seqNmb, 
U8           *plmn,
U8           *kasme
)
#else
PRIVATE S16 vbHssGenKasme(ck, ik, ak, seqNmb, plmn, kasme)
U8           *ck,
U8           *ik,
U8           *ak,
U8           *seqNmb, 
U8           *plmn,
U8           *kasme
#endif
{

   U8 key[VB_HSS_IK_SIZE + VB_HSS_CK_SIZE];
   U8 s[14];
   VB_HSS_AUTH_TRC2(vbHssGenKasme)

   s[0] = 0x10;
   s[1] = plmn[0];
   s[2] = plmn[1];
   s[3] = plmn[2];
   s[4] = 0x00;
   s[5] = 0x03;
   s[6] = seqNmb[0] ^ ak[0];
   s[7] = seqNmb[1] ^ ak[1];
   s[8] = seqNmb[2] ^ ak[2];
   s[9] = seqNmb[3] ^ ak[3];
   s[10] = seqNmb[4] ^ ak[4];
   s[11] = seqNmb[5] ^ ak[5];
   s[12] = 0x00;
   s[13] = 0x06;

   cmMemcpy(key, ck, VB_HSS_CK_SIZE);
   memcpy((key + VB_HSS_CK_SIZE), ik, VB_HSS_IK_SIZE);

   VB_HSS_AUTH_PRNT_HEX("Key(ck||ik)", key, VB_HSS_IK_SIZE + VB_HSS_CK_SIZE);
   VB_HSS_AUTH_PRNT_HEX("S", s, 14);

   /*Generates Sequence Number*/   
   vbMmeKeyGenAlg(key, VB_HSS_IK_SIZE + VB_HSS_CK_SIZE,
                  s, 14, kasme, VB_KASME_SIZE);

   VB_HSS_AUTH_PRNT_HEX("Kasme", kasme, VB_KASME_SIZE);

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbHssGenAuthVec
 *       
 *       Desc:  This function generates Authentication Vector At HSS
 *       
 *       Ret:   None
 *       
 *       Notes:  
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PUBLIC S16  vbHssGenAuthVec
(
VbHssUeAuthInfo     *ueInfo, 
VbHssAuthVec        *authVec, 
U8                  *plmn
)
#else
PUBLIC S16  vbHssGenAuthVec(ueInfo, authVec, plmn)
VbHssUeAuthInfo     *ueInfo;
VbHssAuthVec        *authVec; 
U8                  *plmn;
#endif
{
   U32 rndNmb = 0;
   U8  indx = 0;
   U8 seqXorAk[VB_HSS_SEQ_NMB_SIZE];
   U8 amf[2] = {0x80, 0x00};
   U8 mac[VB_HSS_MAC_SIZE];
   VB_HSS_AUTH_TRC1(vbHssGenAuthVec)

   /*Increment Sequence Number For Next Use*/
   vbHssIncSeqNmb(ueInfo->seqNmb);
#if 0
   /*Generate Random Number*/
   for(indx = 0; indx < VB_RAND_SIZE/4; indx = indx + 4)
   {
      rndNmb = rand();  /*stdlib defined call*/
      authVec->vec.rndNmb[indx + 0] = (rndNmb >> 24) & 0xff;
      authVec->vec.rndNmb[indx + 1] = (rndNmb >> 16) & 0xff;
      authVec->vec.rndNmb[indx + 2] = (rndNmb >> 8) & 0xff;
      authVec->vec.rndNmb[indx + 3] = rndNmb & 0xff;
   }
#endif

   /*Auth Algorithm Interface Functions*/
   /*Generates RES, CK, IK and AK*/

   VB_HSS_AUTH_PRNT_HEX("Shared Key", ueInfo->sharedKey, 
                            VB_HSS_SHARED_KEY_SIZE);
   VB_HSS_AUTH_PRNT_HEX("OP Key", ueInfo->opKey,
                            VB_HSS_OP_KEY_SIZE);
   VB_HSS_AUTH_PRNT_HEX("Rand", authVec->vec.rndNmb,
                            VB_RAND_SIZE);

   if (ueInfo->authAlg == VB_MME_AUTH_XOR)
   {
      vbHssXorAuthVectors(ueInfo->sharedKey, authVec->vec.rndNmb,\
            authVec->vec.xres, authVec->ck, authVec->ik, authVec->ak);
   }
   else
   {
      vbHssAuthF2345(ueInfo->sharedKey,  ueInfo->opKey, 
                     authVec->vec.rndNmb , authVec->vec.xres, 
                     authVec->ck, authVec->ik, authVec->ak);
   }


   VB_HSS_AUTH_PRNT_HEX("XRes", authVec->vec.xres, VB_RES_SIZE);
   VB_HSS_AUTH_PRNT_HEX("CK", authVec->ck, VB_HSS_CK_SIZE);
   VB_HSS_AUTH_PRNT_HEX("IK", authVec->ik, VB_HSS_IK_SIZE);
   VB_HSS_AUTH_PRNT_HEX("AK", authVec->ak, VB_HSS_AK_SIZE);

   /*Auth Algorithm Interface Functions*/
   /*Generates MAC*/
   if (ueInfo->authAlg == VB_MME_AUTH_XOR)
   {
      vbHssXorAuthMac(ueInfo->sharedKey, authVec->vec.rndNmb,\
            ueInfo->seqNmb, amf, mac);
   }
   else
   {
      vbHssAuthF1(ueInfo->sharedKey, ueInfo->opKey, 
               authVec->vec.rndNmb, ueInfo->seqNmb, amf, mac);
   }

   VB_HSS_AUTH_PRNT_HEX("Mac", mac,
                            VB_HSS_MAC_SIZE);
   /*Construct AUTN*/
   for(indx = 0; indx < VB_HSS_SEQ_NMB_SIZE; indx++)
   {
       seqXorAk[indx] = ueInfo->seqNmb[indx] ^ authVec->ak[indx];
       authVec->vec.autn[indx] = seqXorAk[indx];
   }
   authVec->vec.autn[VB_HSS_SEQ_NMB_SIZE] = amf[0];
   authVec->vec.autn[VB_HSS_SEQ_NMB_SIZE+1] = amf[1];
   for(indx = 0; indx < VB_HSS_MAC_SIZE; indx++)
   {
      authVec->vec.autn[indx + VB_HSS_SEQ_NMB_SIZE + 2] = mac[indx];
   }
   VB_HSS_AUTH_PRNT_HEX("Autn", authVec->vec.autn,
                            VB_AUTN_SIZE);

   /*Generate KASME*/
   vbHssGenKasme(authVec->ck, authVec->ik, authVec->ak, 
                 ueInfo->seqNmb, plmn, authVec->vec.kasme);
     
   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbHssPrcsSyncFlr
 *       
 *       Desc:  This function process the AUTS, validates the UE and generates 
 *              Authentiction Vector 
 *       
 *       Ret:   None
 *       
 *       Notes:     
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PUBLIC S16  vbHssPrcsSyncFlr
(
VbHssUeAuthInfo   *ueInfo, 
U8                *auts, 
U8                *rnd, 
VbHssAuthVec      *authVec, 
U8                *plmn
)
#else
PUBLIC S16 vbHssPrcsSyncFlr(ueInfo, auts, rnd, authVec, plmn)
VbHssUeAuthInfo   *ueInfo;
U8                *auts; 
U8                *rnd; 
VbHssAuthVec      *authVec; 
U8                *plmn;
#endif
{
   U8 indx = 0;
   U8 ak[VB_HSS_AK_SIZE];
   U8 sqnMS[VB_HSS_SEQ_NMB_SIZE];
   U8 macRcvd[VB_HSS_MAC_SIZE];
   U8 macCalc[VB_HSS_MAC_SIZE];
   U8 amf[2]={0, 0};

   VB_HSS_AUTH_TRC1(vbHssPrcsSyncFlr)

   /*Generates AK*/

   VB_HSS_AUTH_PRNT_HEX("Shared Key", ueInfo->sharedKey, 
                            VB_HSS_SHARED_KEY_SIZE);
   VB_HSS_AUTH_PRNT_HEX("OP Key", ueInfo->opKey,
                            VB_HSS_OP_KEY_SIZE);
   VB_HSS_AUTH_PRNT_HEX("Rand", rnd,
                            VB_RAND_SIZE);

   vbHssAuthF5Star(ueInfo->sharedKey,
                   ueInfo->opKey, rnd, ak) ; 

   VB_HSS_AUTH_PRNT_HEX("AK", ak,
                            VB_HSS_AK_SIZE);

   VB_HSS_AUTH_PRNT_HEX("Auts", auts,
                            VB_AUTS_SIZE);
   /*Construct UEs Seq Num*/
   for(indx = 0; indx < VB_HSS_SEQ_NMB_SIZE; indx++)
   {
      sqnMS[indx] = auts[indx] ^ ak[indx];
   }

   /*Extract MAC Received From UE*/
   for(indx = VB_HSS_SEQ_NMB_SIZE; indx < VB_AUTS_SIZE; indx++)
   {
      macRcvd[indx - VB_HSS_SEQ_NMB_SIZE] = auts[indx];
   }

   VB_HSS_AUTH_PRNT_HEX("Rcvd Mac", macRcvd,
                            VB_HSS_MAC_SIZE);

   VB_HSS_AUTH_PRNT_HEX("MS Seq Num", sqnMS,
                            VB_HSS_SEQ_NMB_SIZE);
   /*Generate MAC*/
   vbHssAuthF1Star(ueInfo->sharedKey, ueInfo->opKey,  
               rnd, sqnMS, amf, macCalc); 

   VB_HSS_AUTH_PRNT_HEX("Calc Mac", macCalc,
                            VB_HSS_MAC_SIZE);
    
   /*Received MAC And Calculated MAC Should Match*/
   if(0 == cmMemcmp(macCalc, macRcvd, VB_HSS_MAC_SIZE))
   {
      /*Update UE's Sequence Number*/
      vbHssUpdSeqNmb(ueInfo, sqnMS);
   }  
   else
   {
      /* MAC Failure: Illegal UE */
      RETVALUE(RFAILED);
   }
   
   /* Gen new set of authentication vector */ 
   vbHssGenAuthVec(ueInfo, authVec, plmn);

   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbHssGenAuthRsp
 *       
 *       Desc:  This function generates Auth RES and KASME or AUTS
 *       
 *       Ret:   ROK
 *              RFAILED
 *       
 *       Notes:     
 *              
 *       File:  
 ********************************************************************/
#ifdef ANSI
PUBLIC S16 vbHssGenAuthRsp
(
VbHssUeAuthInfo *ueInfo, 
U8              *autn, 
U8              *rnd, 
U8              *plmn,
U8              *res, 
U8              *kasme,
U8              *cause, 
U8              *auts
)
#else
PUBLIC S16 vbHssGenAuthRsp(ueInfo, autn, rnd, plmn, res, kasme, cause, auts)
VbHssUeAuthInfo *ueInfo;
U8              *autn; 
U8              *rnd; 
U8              *plmn;
U8              *res; 
U8              *kasme,
U8              *cause; 
U8              *auts;
#endif
{
   U8 ak[VB_HSS_AK_SIZE];
   U8 ck[VB_HSS_CK_SIZE];
   U8 ik[VB_HSS_IK_SIZE];
   U8 sqnRcvd[VB_HSS_SEQ_NMB_SIZE];
   U8 macRcvd[VB_HSS_MAC_SIZE];
   U8 macCalc[VB_HSS_MAC_SIZE];
   U8 amf[2];
   U8 indx = 0;
   VB_HSS_AUTH_TRC1(vbHssGenAuthRsp)

   /*Generates RES, CK, IK and AK*/
   vbHssAuthF2345(ueInfo->sharedKey, ueInfo->opKey, 
                  rnd, res, ck, ik, ak);
  
   /*Extract Info From AUTN*/
   for(indx = 0; indx < VB_HSS_SEQ_NMB_SIZE; indx++)
   {
      sqnRcvd[indx] = autn[indx] ^ ak[indx];  
   } 
   amf[0] = autn[VB_HSS_SEQ_NMB_SIZE];
   amf[1] = autn[VB_HSS_SEQ_NMB_SIZE + 1];
   for(indx = VB_HSS_SEQ_NMB_SIZE + 2; indx < VB_AUTN_SIZE; indx++)
   {
      macRcvd[(indx- VB_HSS_SEQ_NMB_SIZE - 2)] = autn[indx];  
   }

   /*Calculate MAC Based On Rcvd Info*/
   vbHssAuthF1(ueInfo->sharedKey, ueInfo->opKey, 
               rnd, sqnRcvd, amf, macCalc);
   if(0 != cmMemcmp(macRcvd, macCalc, VB_HSS_MAC_SIZE))
   {
      *cause = VB_MME_HSS_MAC_FAILURE;
      RETVALUE(RFAILED);
   }

   if(ROK != vbHssVldSeqNmb(ueInfo, sqnRcvd))
   {
      *cause = VB_MME_HSS_SYNC_FAILURE;

      /*Generates AK*/
      vbHssAuthF5Star(ueInfo->sharedKey,
               ueInfo->opKey, rnd, ak) ; 

      /*Generates MAC*/
      vbHssAuthF1Star(ueInfo->sharedKey, ueInfo->opKey,
                      rnd, ueInfo->seqNmb, amf, macCalc);
      
      /*Construct AUTS*/
      for(indx = 0; indx < VB_HSS_SEQ_NMB_SIZE; indx++) 
      {
         auts[indx] = ueInfo->seqNmb[indx] ^ ak[indx];
      }

      for(indx = 0; indx < VB_HSS_MAC_SIZE; indx++) 
      {
         auts[VB_HSS_SEQ_NMB_SIZE + indx] = macCalc[indx];
      }
    
      RETVALUE(RFAILED);
   }   
      
   /*Generate KASME*/
   vbHssGenKasme(ck, ik, ak, sqnRcvd, plmn, kasme);

   /*Update The UE's Sqn Number For Future Use*/
   vbHssUpdSeqNmb(ueInfo, sqnRcvd);
    
   RETVALUE(ROK);
}

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
