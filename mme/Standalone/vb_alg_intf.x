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

     Name:    Trillium LTE CNE - MME Security Module

     Type:    C source file

     Desc:    This file contains security module c structure

     File:    vb_alg_intf.x

     Sid:     vb_alg_intf.x@@/main/1 - Mon Jun 28 19:35:38 2010

     Prg:     sk 
*********************************************************************21*/
#ifndef __VB_ALG_INTF_X__
#define __VB_ALG_INTF_X__ 1

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */


/*Algorithm Interface*/
EXTERN Void vbMmeEncSnow3GAlg ARGS((U8    *key, 
                                    U32    count, 
                                    U8     dir, 
                                    U8     bearer, 
                                    U8    *msg, 
                                    U16    len));

EXTERN Void vbMmeEncNullAlg  ARGS((U8    *key, 
                                   U32    count, 
                                   U8     dir, 
                                   U8     bearer, 
                                   U8    *msg, 
                                   U16    len));

EXTERN Void vbMmeEncAESAlg  ARGS((U8    *key,
                                   U32    count,
                                   U8     dir,
                                   U8     bearer,
                                   U8    *msg,
                                   U16    len));

EXTERN U32  vbMmeIntSnow3GAlg ARGS((U8     *key, 
                                    U32     count, 
                                    U8      dir, 
                                    U8      bearer, 
                                    U8     *msg, 
                                    U16     len));

EXTERN U32  vbMmeIntNullAlg ARGS((U8     *key, 
                                  U32     count, 
                                  U8      dir, 
                                  U8      bearer, 
                                  U8     *msg, 
                                  U16     len));

EXTERN U32  vbMmeIntAESAlg ARGS((U8     *key,
                                  U32     count,
                                  U8      dir,
                                  U8      bearer,
                                  U8     *msg,
                                  U16     len));

EXTERN Void vbMmeKeyGenAlg ARGS((U8    *key,
                                 U16   keyLen,
                                 U8    *s,
                                 U16   sLen,
                                 U8    *output,
                                 U16   outputLen));


EXTERN Void vbHssAuthF2345 ARGS((U8    *k, 
                                 U8    *op, 
                                 U8    *rand, 
                                 U8    *res, 
                                 U8    *ck, 
                                 U8    *ik, 
                                 U8    *ak));

EXTERN Void vbHssAuthF1 ARGS((U8    *k, 
                              U8    *op, 
                              U8    *rand, 
                              U8    *sqn, 
                              U8    *amf, 
                              U8    *mac));

EXTERN Void vbHssAuthF1Star ARGS((U8    *k, 
                                  U8    *op, 
                                  U8    *rand, 
                                  U8    *sqn, 
                                  U8    *amf, 
                                  U8    *mac));

EXTERN Void vbHssAuthF5Star ARGS((U8    *k, 
                                  U8    *op, 
                                  U8    *rand, 
                                  U8    *ak));

PUBLIC Void vbHssXorAuthVectors
(
U8    *k, 
U8    *rnd, 
U8    *res, 
U8    *ck, 
U8    *ik, 
U8    *ak
);

PUBLIC Void vbHssXorAuthMac
(
U8    *k, 
U8    *rnd, 
U8    *sqn, 
U8    *amf, 
U8    *mac
);

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
#endif /*__VB_ALG_INTF_X__*/
