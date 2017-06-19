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
  
     Name:     MME NAS Security Module
  
     Type:     C include file
  
     Desc:     This file contains HSS authentication module C structure
   
     File:     vb_mme_auth.x
  
     Sid:      vb_mme_auth.x@@/main/1156 - Mon Mar 21 11:09:25 2011
   
     Prg:      sk
  
*********************************************************************21*/

#ifndef __VB_HSS_AUTH_X__
#define __VB_HSS_AUTH_X__ 1

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */
PUBLIC Void  vbHssAuthHexDump ARGS((S8    *mod,
                                       S8    *name,
                                       S8    *file,
                                       U16   line,
                                       S8    *fun,
                                       U8    *buf,
                                       U16   len));

#define VB_HSS_AUTH_MOD_NAME "LTE-CNE-AUTH" 

#define VB_HSS_AUTH_PRNT_INFO(args) \
  printf(" [%s] [%s:%d:%s] [INFO]: ", VB_HSS_AUTH_MOD_NAME, __FILE__, __LINE__, __tapa_fun_);\
  printf args;\
  printf("\n");

#define VB_HSS_AUTH_PRNT_ERR(args) \
  printf(" [%s] [%s:%d:%s] [ERROR]: ", VB_HSS_AUTH_MOD_NAME, __FILE__, __LINE__, __tapa_fun_); \
  printf args;\
  printf("\n");

#define VB_HSS_AUTH_PRNT_HEX(name, buf, len) \
  vbHssAuthHexDump(VB_HSS_AUTH_MOD_NAME, name, __FILE__, __LINE__, __tapa_fun_, buf, len);

#define VB_HSS_AUTH_TRC1(arg)\
  S8 __tapa_fun_[100];\
  sprintf(__tapa_fun_, #arg); \
  printf(" [%s] [%s:%d:%s] [TRC1]: Entering ", VB_HSS_AUTH_MOD_NAME, __FILE__, __LINE__, __tapa_fun_); \
  printf(#arg); \
  printf("\n");

#define VB_HSS_AUTH_TRC2(arg) \
  S8 __tapa_fun_[100];\
  sprintf(__tapa_fun_, #arg); \
  printf(" [%s] [%s:%d:%s] [TRC2]: Entering ", VB_HSS_AUTH_MOD_NAME, __FILE__, __LINE__, __tapa_fun_); \
  printf(#arg); \
  printf("\n");

/* MME Authentication Vector */
typedef struct vbMMEAuthVec
{
   U8 rndNmb[VB_RAND_SIZE];
   U8 autn[VB_AUTN_SIZE];
   U8 xres[VB_RES_SIZE];
   U8 kasme[VB_KASME_SIZE];
}VbMmeAuthVec;

/* HSS Authentication Vector */
typedef struct vbHssAuthVec
{
   U8             ck[VB_HSS_CK_SIZE];
   U8             ik[VB_HSS_IK_SIZE];
   U8             ak[VB_HSS_AK_SIZE];
   VbMmeAuthVec   vec;
}VbHssAuthVec;

typedef enum _VbMmeAuthAlg
{
   VB_MME_AUTH_XOR,
   VB_MME_AUTH_MILANAGE,
   VB_MME_AUTH_MAX
} VbMmeAuthAlg;

typedef struct vbHssUeAuthInfo
{
   U8                sharedKey[VB_HSS_SHARED_KEY_SIZE];
   U8                opKey[VB_HSS_OP_KEY_SIZE];
   U8                seqNmb[VB_HSS_SEQ_NMB_SIZE];
   VbMmeAuthAlg      authAlg;
} VbHssUeAuthInfo;


PUBLIC S16  vbHssGenAuthVec ARGS
((
VbHssUeAuthInfo     *ueInfo, 
VbHssAuthVec        *authVec, 
U8                  *plmn
));

PUBLIC S16  vbHssPrcsSyncFlr ARGS
((
VbHssUeAuthInfo   *ueInfo, 
U8                *auts, 
U8                *rnd, 
VbHssAuthVec      *authVec, 
U8                *plmn
));

EXTERN S16 vbHssGenAuthRsp ARGS((VbHssUeAuthInfo     *ueInfo,
                                 U8                  *autn,
                                 U8                  *rand,
                                 U8                  *plmn,
                                 U8                  *res,
                                 U8                  *kasme,
                                 U8                  *cause,
                                 U8                  *auts));

#endif /* __VB_HSS_AUTH_X__ */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
