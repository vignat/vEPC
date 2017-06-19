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

     Desc:    This file contains security module source code

     File:    vb_alg_intf.c

     Sid:     vb_alg_intf.c@@/main/1 - Mon Jun 28 19:35:38 2010

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
#include "vb_aes_mode.h"   /* CMAC and CTR defines            */
#include "vb_hss_auth.h"   /* CMAC and CTR defines            */

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
#include "vb_aes_mode.x"   /* CMAC and CTR declarations       */

/*All external security library include */ 
#include "ext_common.h"
#include "auth_alg.h"
#include "hmacsha256.h"
#include "snow3g_f8f9.h"

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */
extern u8 OP[16];

/**********************************************************************/
/*                 Encryption Alg Interface Fnc's                     */
/**********************************************************************/

#ifdef ANSI
Void vbMmeEncSnow3GAlg
(
U8    *key, 
U32    count, 
U8     dir, 
U8     bearer, 
U8    *msg, 
U16    len
)
#else
Void vbMmeEncSnow3GAlg(key, count, dir, bearer, msg, len)
(
U8    *key;
U32    count; 
U8     dir;
U8     bearer; 
U8    *msg;
U16    len;
)
#endif
{
   /* Calling Snow-3G encode function */
   f8Snow3g(key, count, bearer, dir, msg, len*8);

   RETVOID;
}

#ifdef ANSI
Void vbMmeEncNullAlg
(
U8    *key, 
U32    count, 
U8     dir, 
U8     bearer, 
U8    *msg, 
U16    len
)
#else
Void vbMmeEncNullAlg(key, count, dir, bearer, msg, len)
(
U8     *key; 
U32     count; 
U8      dir; 
U8      bearer; 
U8     *msg; 
U16     len;
)
#endif
{
   UNUSED(key);
   UNUSED(count);
   UNUSED(dir);
   UNUSED(bearer);
   UNUSED(msg);
   UNUSED(len);
   /*No encoding of message */
   RETVOID;
}

#ifdef ANSI
Void vbMmeEncAESAlg
(
U8    *key,
U32    count,
U8     dir,
U8     bearer,
U8    *msg,
U16    len
)
#else
Void vbMmeEncAESAlg(key, count, dir, bearer, msg, len)
(
U8     *key;
U32     count;
U8      dir;
U8      bearer;
U8     *msg;
U16     len;
)
#endif
{
   U8 T1[16] = {0};
   /* prepare the input */
   T1[0] = (count >> 24) & 0xff; 
   T1[1] = (count >> 16) & 0xff; 
   T1[2] = (count >> 8) & 0xff; 
   T1[3] = (count) & 0xff; 
   T1[4] = (bearer << 3) & 0xf8;
   T1[4] = T1[4] | ((dir << 2 ) & 0x04);
   /* Call CTR algorithm */
   vbAESCTR(key, 128, T1, 128, msg, len*8);
   RETVOID;
}


/**********************************************************************/
/*                 Integrity Alg Interface Fnc's                      */
/**********************************************************************/
/* Integrity Alg */

#ifdef ANSI
U32  vbMmeIntSnow3GAlg
(
U8     *key, 
U32     count, 
U8      dir, 
U8      bearer, 
U8     *msg, 
U16     len
)
#else
U32  vbMmeIntSnow3GAlg(key, count, dir, bearer, msg, len)
U8     *key; 
U32     count; 
U8      dir;
U8      bearer; 
U8     *msg; 
U16     len;
#endif
{
   U32 mac;
   /* Calling Snow-3G integrity function */
   mac = f9Snow3g(key, count, bearer, dir, msg, len*8);
   RETVALUE(mac);
}

#ifdef ANSI
U32  vbMmeIntNullAlg
(
U8     *key, 
U32     count, 
U8      dir, 
U8      bearer, 
U8     *msg, 
U16     len
)
#else
U32  vbMmeIntNullAlg(key, count, dir, bearer, msg, len)
U8     *key; 
U32     count; 
U8      dir;
U8      bearer; 
U8     *msg; 
U16     len;
#endif
{
   UNUSED(key);
   UNUSED(count);
   UNUSED(dir);
   UNUSED(bearer);
   UNUSED(msg);
   UNUSED(len);
   /* mac is zero */
   RETVALUE(0);
}

#ifdef ANSI
U32  vbMmeIntAESAlg
(
U8     *key,
U32     count,
U8      dir,
U8      bearer,
U8     *msg,
U16     len
)
#else
U32  vbMmeIntAESAlg(key, count, dir, bearer, msg, len)
U8     *key;
U32     count;
U8      dir;
U8      bearer;
U8     *msg;
U16     len;
#endif
{
   U8 M[VB_AES_MAX_MSG_SIZE]    = {0};
   U8 mac[4]                    = {0};

   /* Preparation */
   M[0] = (count >> 24) & 0xff; 
   M[1] = (count >> 16) & 0xff; 
   M[2] = (count >> 8) & 0xff; 
   M[3] = (count) & 0xff; 
   M[4] = (bearer << 3) & 0xf8;
   M[4] = M[4] | ((dir << 2 ) & 0x04);
   cmMemcpy(&M[8], msg, len);

   /* Call CMAC algorithm */
   vbAESCMAC(key,128, M, (len+8)*8, mac, 4);

   RETVALUE( mac[0] << 24 |
             mac[1] << 16 |
             mac[2] << 8  |
             mac[3]);
   
}

/**********************************************************************/
/*                 Key Gen Alg Interface Fnc's                        */
/**********************************************************************/

#ifdef ANSI
Void vbMmeKeyGenAlg
(
U8    *key, 
U16   keyLen,
U8    *s,
U16   sLen, 
U8    *output, 
U16   outputLen
)
#else
Void vbMmeKeyGenAlg(key, keyLen, s, sLen, output, outPutLen)
U8    *key;
U16   keyLen;
U8    *s;
U16   sLen;
U8    *output;
U16   outputLen;
#endif
{
   U8 digest[512] ;

   /* start of hmacsha-256 interface dependent code */
   hmacsha256Param sp;
   hmacsha256Setup (&sp, (const byte*) key, (size_t) keyLen*8);
   hmacsha256Update(&sp, (const byte*) s, (size_t) sLen);
   hmacsha256Digest(&sp, (byte*) digest);
   /* end of hmacsha-256 interface dependent code */

   cmMemcpy(output, digest + (32 - outputLen), outputLen);
}

/**********************************************************************/
/*                 Authentication Alg Interface Fnc's                 */
/**********************************************************************/
#if ANSI
PUBLIC Void vbHssAuthF2345
(
U8    *k, 
U8    *op, 
U8    *rnd, 
U8    *res, 
U8    *ck, 
U8    *ik, 
U8    *ak
)
#else
PUBLIC Void vbHssAuthF2345(k, op, rnd, res, ck, ik, ak)
U8    *k; 
U8    *op; 
U8    *rnd; 
U8    *res; 
U8    *ck; 
U8    *ik; 
U8    *ak;
#endif
{
   cmMemcpy(OP, op, 16);
   f2345(k, rnd, res, ck, ik, ak);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void vbHssAuthF1
(
U8    *k, 
U8    *op, 
U8    *rnd, 
U8    *sqn, 
U8    *amf, 
U8    *mac
)
#else
PUBLIC Void vbHssAuthF1(k, op, rnd, sqn, amf, mac)
U8    *k; 
U8    *op; 
U8    *rnd; 
U8    *sqn; 
U8    *amf; 
U8    *mac;
#endif
{
   cmMemcpy(OP, op, 16);
   f1(k, rnd, sqn, amf, mac);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void vbHssAuthF1Star
(
U8    *k, 
U8    *op, 
U8    *rnd, 
U8    *sqn, 
U8    *amf, 
U8    *mac
)
#else
PUBLIC Void vbHssAuthF1Star(k, op, rnd, sqn, amf, mac)
U8    *k; 
U8    *op; 
U8    *rnd; 
U8    *sqn; 
U8    *amf; 
U8    *mac;
#endif
{
   cmMemcpy(OP, op, 16);
   f1star(k, rnd, sqn, amf, mac);
   RETVOID;
}

#ifdef ANSI
PUBLIC Void vbHssAuthF5Star
(
U8    *k, 
U8    *op, 
U8    *rnd, 
U8    *ak
)
#else
PUBLIC Void vbHssAuthF5Star(k, op, rnd, ak)
U8    *k;
U8    *op; 
U8    *rnd; 
U8    *ak;
#endif
{
   cmMemcpy(OP, op, 16);
   f5star(k, rnd, ak);
   RETVOID;
}

#if ANSI
PUBLIC Void vbHssXorAuthVectors
(
U8    *k, 
U8    *rnd, 
U8    *res, 
U8    *ck, 
U8    *ik, 
U8    *ak
)
#else
PUBLIC Void vbHssXorAuthVectors(k, rnd, res, ck, ik, ak)
U8    *k; 
U8    *rnd; 
U8    *res; 
U8    *ck; 
U8    *ik; 
U8    *ak;
#endif
{
   /* ak */
   HSS_AUTN_GET_AK(k, rnd, ak);
   /* ck and res */
   HSS_AUTN_GET_CK_N_RES(k, rnd, ck, res);
   /* ik */
   HSS_AUTN_GET_IK(k, rnd, ik);
}


#if ANSI
PUBLIC Void vbHssXorAuthMac
(
U8    *k, 
U8    *rnd, 
U8    *sqn, 
U8    *amf, 
U8    *mac
)
#else
PUBLIC Void vbHssXorAuthMac(k, rnd, sqn, amf, mac)
U8    *k; 
U8    *rnd; 
U8    *sqn; 
U8    *amf; 
U8    *mac;
#endif
{
   U8    cdOut[8];

   /* cdOut */
   HSS_AUTN_GET_CDOUT(sqn, amf, cdOut);
   /* mac */
   HSS_AUTN_GET_MAC(k, rnd, cdOut, mac);
}

#ifdef __cplusplus
}
#endif /* _cplusplus */
