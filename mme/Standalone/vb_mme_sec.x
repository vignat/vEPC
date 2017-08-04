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
  
     Name:     MME NAS Security Module
  
     Type:     C include file
  
     Desc:     C structure for security module.
   
     File:     vb_mme_sec.x
  
     Sid:      vb_mme_sec.x@@/main/1156 - Mon Nov 21 11:09:25 2011
   
     Prg:      sk
  
*********************************************************************21*/

#ifndef __VB_MME_SEC_X__
#define __VB_MME_SEC_X__ 1

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

PUBLIC Void  vbMmeSecHexDump ARGS((S8    *mod,
                                   S8    *name,
                                   S8    *file,
                                   U16   line,
                                   S8    *fun,
                                   U8    *buf,
                                   U16   len));

#define VB_MME_SEC_MOD_NAME "LTE-CNE-SEC" 

#define VB_MME_SEC_PRNT_INFO(args) \
  printf(" [%s] [%s:%d:%s] [INFO]: ", VB_MME_SEC_MOD_NAME, __FILE__, __LINE__, __tapa_fun);\
  printf args;\
  printf("\n");

#define VB_MME_SEC_PRNT_ERR(args) \
  printf(" [%s] [%s:%d:%s] [ERROR]: ", VB_MME_SEC_MOD_NAME, __FILE__, __LINE__, __tapa_fun); \
  printf args;\
  printf("\n");

#ifdef DEBUGP
#define VB_MME_SEC_PRNT_HEX(name, buf, len) \
  vbMmeSecHexDump(VB_MME_SEC_MOD_NAME, name, __FILE__, __LINE__, __tapa_fun, buf, len);
#else
#define VB_MME_SEC_PRNT_HEX(name, buf, len)
#endif

#define VB_MME_SEC_TRC1(arg)\
  S8 __tapa_fun[100];\
  sprintf(__tapa_fun, #arg); \
  printf(" [%s] [%s:%d:%s] [TRC1]: Entering ", VB_MME_SEC_MOD_NAME, __FILE__, __LINE__, __tapa_fun); \
  printf(#arg); \
  printf("\n");
 
#define VB_MME_SEC_TRC2(arg) \
  S8 __tapa_fun[100];\
  sprintf(__tapa_fun, #arg); \
  printf(" [%s] [%s:%d:%s] [TRC2]: Entering ", VB_MME_SEC_MOD_NAME, __FILE__, __LINE__, __tapa_fun); \
  printf(#arg); \
  printf("\n");

/*Encryption Algoritm Identifier */
typedef enum vbMmeEncAlg
{
   VB_MME_EEA0 = 0,
   VB_MME_EEA1_128,
   VB_MME_EEA2_128,

   /* should be last member */
   VB_MME_EEA_MAX = 7
}VbMmeEncAlg;

/*Integrity Algoritm Identifier */
typedef enum vbMmeIntAlg
{
   VB_MME_EIA0 = 0,
   VB_MME_EIA1_128,
   VB_MME_EIA2_128,

   /* should be last member */
   VB_MME_EIA_MAX = 7
}VbMmeIntAlg;

typedef struct vbMmeMsg
{
   U8     *val;
   U16    len;
}VbMmeMsg;

/*NAS Security Hdr*/
typedef struct vbMmeSecHdr
{
   U8  pd;
   U8  hdrType;
   U32 mac;
   U8  seqNmb;
}VbMmeSecHdr;

/*NAS Sequence Number*/
typedef struct vbMmeNasSeqNmb
{
   U8 seqNmb;
   U16 ovrflwCnt;
}VbMmeNasSeqNmb;

/*Security Context In MME*/
typedef struct _VbMmeSecCtxt
{
   /*Inputs*/
   U8              secMode;
   U8              ck[CM_EMM_MAX_CK];
   U8              ik[CM_EMM_MAX_IK];
   U8              kasme[VB_MME_SZ_KASME_KEY];
   U8              tsc;    /*Native or Non-Native */
   U8              status;  /*Current or Non Current*/
   U8              ksi;
   VbMmeEncAlg     encAlg;
   VbMmeIntAlg     intAlg;

   /*Outputs*/
   U8              encKey[VB_MME_SZ_NAS_ENC_KEY];
   U8              intKey[VB_MME_SZ_NAS_INT_KEY];
   U8              enbKey[VB_MME_SZ_ENB_KEY];
   U32             lstUsdCnt;

   /*Local Info*/
   VbMmeNasSeqNmb  dlSeqNmb;
   VbMmeNasSeqNmb  ulSeqNmb;

   /* Parameters for S1 Handover */
   U8              nxtHop[VB_MME_SZ_NEXT_HOP_KEY]; /* Next Hop */
   U8              nxtHopChainCnt; /* next hop chaining count */

   U32             nonce;
} VbMmeSecCtxt;


/*MME NAS Security Interface */
EXTERN S16 vbMmeCompDLSec ARGS((VbMmeSecCtxt *secCtx, 
                                   U8             hdrType, 
                                   VbMmeMsg       *srcMsg, 
                                   VbMmeMsg       *dstMsg));

EXTERN S16 vbMmeVldULSec ARGS((VbMmeSecCtxt *secCtx, 
                                  VbMmeMsg       *srcMsg, 
                                  VbMmeMsg       *dstMsg));

/*Key Generation Interface in MME*/
EXTERN S16 vbMmeGenerateNasKey ARGS((VbMmeSecCtxt *secCtx));

EXTERN S16 vbMmeGenerateEnbKey ARGS((VbMmeSecCtxt *secCtx,
                                     U32            count));

EXTERN S16 vbMmeGenerateMappedCKIK ARGS((VbMmeSecCtxt *secCtx,
                                      VbMmeSecCtxt *mappedSecCtx));


EXTERN S16 vbMmeGenerateMappedKasme ARGS((VbMmeSecCtxt *secCtx));

EXTERN S16 vbMmeGenerateMappedSecKeys ARGS((VbMmeSecCtxt *secCtx));



EXTERN S16 vbMmeGenerateNextHop ARGS
((
VbMmeSecCtxt   *secCtx,
U8             *syncInput
));

#ifdef __cplusplus
}
#endif /* end of __cplusplus */
#endif /*__VB_MME_SEC_X__*/
