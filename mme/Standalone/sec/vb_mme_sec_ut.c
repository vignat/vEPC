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

     Name:    Trillium LTE CNE - MME Security Module

     Type:    C source file

     Desc:    This file contains security module source code

     File:    vb_mme_sec_ut.c

     Sid:     vb_mme_sec_ut.c@@/main/1 - Mon Jun 28 19:35:38 2010

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
#include "vb_mme_sec.h"    /* Defines of security module      */
#include "cm_emm.h"        /* Defines of EMM moduels          */
#include "vb_hss_auth.h"   /* Defines of Auth module          */
#include "ext_common.h"    /* Common external defines         */
#include "snow3g_f8f9.h"   /* F8 and F9 alg of snow 3g        */
#include "hmacsha256.h"    /* Defines of HMAC-SHA-256         */

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
#include "vb_alg_intf.x"   /* External algorithm interface    */
#include "vb_hss_auth.x"   /* Auth algorithm interface        */
#include "vb_aes_mode.x"   /* AES alogorithm interface        */


/* Forward function declaration */
S16 testCase1();
S16 testCase2();
S16 testCase3();
S16 testCase4();
S16 testCase5();
S16 testCase6();
S16 testCase7();
S16 testCase8();

int main()
{
   U32  input;
   while(1)
   {
      printf("\n================================\n");
      printf("            Test Cases");
      printf("\n================================\n");
      printf("1. Snow 3G Encryption\n");
      printf("2. Snow 3G Integrity\n");
      printf("3. Hmac-Sha-256 Key Generation\n");
      printf("4. Hmac-Sha-256 Key Generation\n");
      printf("5. Auth Vector Generation\n");
      printf("6. AES-128 Encryption\n");
      printf("7. AES-128 Integrity(Test Case 1)\n");
      printf("8. AES-128 Integrity(Test Case 2)\n");
      printf("9. Exit\n");
      printf("Enter your selection:");
      scanf("%d", (int*)&input);
 
      switch(input)
      {
         case 1:
         {
            printf("1. Snow 3G Encryption Selected\n");
            testCase1();
            break;
         }
         case 2:
         {
            printf("2. Snow 3G Integrity Selected\n");
            testCase2();
            break;
         }
         case 3:
         {
            printf("3. Hmac-Sha-256 Key Generation Selected\n");
            testCase3();
            break;
         }
         case 4:
         {
            printf("4. Hmac-Sha-256 Key Generation Selected\n");
            testCase4();
            break;
         }
         case 5:
         {
            printf("5. Auth Vector Generation Selected\n");
            testCase5();
            break;
         }
         case 6:
         {
            printf("6. AES-128 Encryption\n");
            testCase6();
            break;
         }
         case 7:
         {
            printf("7. AES-128 Integrity(TestCase 1)\n");
            testCase7(); 
            break;
         }
         case 8:
         {
            printf("8. AES-128 Integrity(TestCase 2)\n");
            testCase8(); 
            break;
         }
         case 9:
         {
            printf("9. Exit...\n");
            exit(0);
            break;
         }
      }
   }
   RETVALUE(ROK);
}
   
S16 testCase1()
{
   U16 len         = 0;
   U32 count_1     = 0x72A4F20F;
   U32  bearer_1   = 0x0C;
   U32  dir_1      = 0x01;
   U8  key_1[]     = {
                      0x2B,0xD6,0x45,0x9F, 0x82,0xC5,0xB3,0x00, 
                      0x95,0x2C,0x49,0x10, 0x48,0x81,0xFF,0x48
                     };
   U32 length_1    = 798 ; /*Length in bits */
   U8 msg_1[]      = {
                      0x7E,0xC6,0x12,0x72, 0x74,0x3B,0xF1,0x61, 
                      0x47,0x26,0x44,0x6A, 0x6C,0x38,0xCE,0xD1,
                      0x66,0xF6,0xCA,0x76, 0xEB,0x54,0x30,0x04, 
                      0x42,0x86,0x34,0x6C, 0xEF,0x13,0x0F,0x92,
                      0x92,0x2B,0x03,0x45, 0x0D,0x3A,0x99,0x75, 
                      0xE5,0xBD,0x2E,0xA0, 0xEB,0x55,0xAD,0x8E,
                      0x1B,0x19,0x9E,0x3E, 0xC4,0x31,0x60,0x20, 
                      0xE9,0xA1,0xB2,0x85, 0xE7,0x62,0x79,0x53,
                      0x59,0xB7,0xBD,0xFD, 0x39,0xBE,0xF4,0xB2, 
                      0x48,0x45,0x83,0xD5, 0xAF,0xE0,0x82,0xAE,
                      0xE6,0x38,0xBF,0x5F, 0xD5,0xA6,0x06,0x19, 
                      0x39,0x01,0xA0,0x8F, 0x4A,0xB4,0x1A,0xAB,
                      0x9B,0x13,0x48,0x80
                     };

   U8 msg_expted[] = {
                      0x8C,0xEB,0xA6,0x29 ,0x43,0xDC,0xED,0x3A,
                      0x09,0x90,0xB0,0x6E ,0xA1,0xB0,0xA2,0xC4,
                      0xFB,0x3C,0xED,0xC7 ,0x1B,0x36,0x9F,0x42,
                      0xBA,0x64,0xC1,0xEB ,0x66,0x65,0xE7,0x2A,
                      0xA1,0xC9,0xBB,0x0D ,0xEA,0xA2,0x0F,0xE8,
                      0x60,0x58,0xB8,0xBA ,0xEE,0x2C,0x2E,0x7F,
                      0x0B,0xEC,0xCE,0x48 ,0xB5,0x29,0x32,0xA5,
                      0x3C,0x9D,0x5F,0x93 ,0x1A,0x3A,0x7C,0x53,
                      0x22,0x59,0xAF,0x43 ,0x25,0xE2,0xA6,0x5E,
                      0x30,0x84,0xAD,0x5F ,0x6A,0x51,0x3B,0x7B,
                      0xDD,0xC1,0xB6,0x5F ,0x0A,0xA0,0xD9,0x7A,
                      0x05,0x3D,0xB5,0x5A ,0x88,0xC4,0xC4,0xF9,
                      0x60,0x5E,0x41,0x40
                     };

   VB_MME_SEC_TRC1(testCase1)
   len = length_1/8+ (length_1%8?1:0);

   VB_MME_SEC_PRNT_INFO(("Test Case 1: Snow-3G Encryption Verification"))
   VB_MME_SEC_PRNT_HEX("Input Buffer", msg_1, len);
   f8Snow3g(key_1, count_1, bearer_1, dir_1, msg_1, length_1);
   VB_MME_SEC_PRNT_HEX("Expected Buffer", msg_expted, len);
   VB_MME_SEC_PRNT_HEX("Output Buffer", msg_1, len);
   
   RETVALUE(ROK);
}

S16 testCase2()
{
   U16 len         = 0;
   U32 mac          = 0;
   U8  calcMac[4]      = {0};
   U8  exptedMac[] = {
                       0x2B,0xCE,0x18,0x20
                      };
   U32  count_1     = 0x38A6F056;
   U32  bearer_1    = 0x05D2EC49;
   U32  dir_1       = 0x0;
   U8   key_1[]     = {
                      0x2B,0xD6,0x45,0x9F,0x82,0xC5,0xB3,0x00,
                      0x95,0x2C,0x49,0x10,0x48,0x81,0xFF,0x48
                     };
   U32  length_1    = 189 ;
   U8   msg_1[]     = {
                      0x6B,0x22,0x77,0x37, 0x29,0x6F,0x39,0x3C, 0x80,0x79,0x35,0x3E, 0xDC,0x87,0xE2,0xE8, 
                      0x05,0xD2,0xEC,0x49, 0xA4,0xF2,0xD8,0xE0 
                     };
   VB_MME_SEC_TRC1(testCase2)
   len = length_1/8+ (length_1%8?1:0);
   VB_MME_SEC_PRNT_INFO(("Test Case 2: Snow-3G Integrity Verification"))
   VB_MME_SEC_PRNT_HEX("Input Buffer", msg_1, len);
   VB_MME_SEC_PRNT_HEX("Expected MAC", exptedMac, 4);

   mac = f9Snow3g(key_1, count_1, bearer_1, dir_1, msg_1, length_1);
   calcMac[0] = mac>>24 & 0xFF;
   calcMac[1] = mac>>16 & 0xFF;
   calcMac[2] = mac>>8 & 0xFF;
   calcMac[3] = mac & 0xFF;
   VB_MME_SEC_PRNT_HEX("Calc MAC", calcMac, 4);

   RETVALUE(ROK);
}

S16 testCase3()
{
   U8 key[] = { 
               0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
               0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
               0xAA, 0xAA, 0xAA, 0xAA
              };
   U8 keyLen = 20;
   U8 data[] = {
                0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
                0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
                0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
                0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
                0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
                0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD,
                0xDD, 0xDD
               };
   U8 exptdOutputLen = 32;
   U8 exptdOutput[] = { 
                       0x77, 0x3e, 0xa9, 0x1e, 0x36, 0x80, 0x0e, 0x46,
                       0x85, 0x4d, 0xb8, 0xeb, 0xd0, 0x91, 0x81, 0xa7,
                       0x29, 0x59, 0x09, 0x8b, 0x3e, 0xf8, 0xc1, 0x22,
                       0xd9, 0x63, 0x55, 0x14, 0xce, 0xd5, 0x65, 0xfe
                      };
   U8 dataLen   = 50;
   U8 hash[512] = {'\0'};
   U8 hashLen   = 32;
   hmacsha256Param sp1;

   VB_MME_SEC_TRC1(testCase3)

   VB_MME_SEC_PRNT_INFO(("Test Case 3: Hmac-Sha-256 Key Generation Verification"))

   VB_MME_SEC_PRNT_HEX("Key", key, keyLen);
   VB_MME_SEC_PRNT_HEX("Data", data, dataLen);
   VB_MME_SEC_PRNT_HEX("Expected output", exptdOutput, exptdOutputLen);

   /*Setup - Give key- In bits*/
   hmacsha256Setup (&sp1, (const byte*) key, (size_t) keyLen*8);
   /*Input data- In bytes*/
   hmacsha256Update(&sp1, (const byte*) data, (size_t) dataLen);
   /*Get MAC*/
   hmacsha256Digest(&sp1, (byte*) hash);

   VB_MME_SEC_PRNT_HEX("Output", hash, hashLen);
   RETVALUE(ROK);
}

S16 testCase4()
{
   U8 key[] = { 
               0x4a, 0x65, 0x66, 0x65
              };
   U8 keyLen = 4;
   U8 data[] = {
                0x77, 0x68, 0x61, 0x74, 0x20, 0x64, 0x6f, 0x20, 
                0x79, 0x61, 0x20, 0x77, 0x61, 0x6e, 0x74, 0x20, 
                0x66, 0x6f, 0x72, 0x20, 0x6e, 0x6f, 0x74, 0x68, 
                0x69, 0x6e, 0x67, 0x3f
               };
   U8 dataLen = 28;
   U8 exptdOutput[] = {
                       0x5b, 0xdc, 0xc1, 0x46, 0xbf, 0x60, 0x75, 0x4e,
                       0x6a, 0x04, 0x24, 0x26, 0x08, 0x95, 0x75, 0xc7,
                       0x5a, 0x00, 0x3f, 0x08, 0x9d, 0x27, 0x39, 0x83,
                       0x9d, 0xec, 0x58, 0xb9, 0x64, 0xec, 0x38, 0x43
                      };
   U8 exptdOutputLen = 32;
   U8 hash[512] = {'\0'};
   U8 hashLen = 32;
   hmacsha256Param sp1;

   VB_MME_SEC_TRC1(testCase4)
   VB_MME_SEC_PRNT_INFO(("Test Case 4: Hmac-Sha-256 Key Generation Verification"))

   VB_MME_SEC_PRNT_HEX("Key", key, keyLen);
   VB_MME_SEC_PRNT_HEX("Data", data, dataLen);
   VB_MME_SEC_PRNT_HEX("Expected output", exptdOutput, exptdOutputLen);
   /*Setup - Give key- In bits*/
   hmacsha256Setup (&sp1, (const byte*) key, (size_t) keyLen*8);
   /*Input data- In bytes*/
   hmacsha256Update(&sp1, (const byte*) data, (size_t) dataLen);
   /*Get MAC*/
   hmacsha256Digest(&sp1, (byte*) hash);

   VB_MME_SEC_PRNT_HEX("Output", hash, hashLen);
   RETVALUE(ROK);
}

S16 testCase5()
{
   VbHssUeAuthInfo     ueInfo;
   VbHssAuthVec        authVec;
   U8                  plmn[3];
   U32                 indx;
   VB_MME_SEC_TRC1(testCase5);
   VB_MME_SEC_PRNT_INFO(("Test Case 5: Authentication Vector Generation"));

   cmMemset((U8*)&ueInfo, 0, sizeof(ueInfo));
   cmMemset((U8*)&authVec, 0, sizeof(authVec));
   cmMemset((U8*)plmn, 0, 3);
   for(indx = 0; indx < 1; indx++)
   {
      vbHssGenAuthVec(&ueInfo, &authVec, plmn);
   }

   RETVALUE(ROK);
}

S16 testCase6()
{
   U8 key[16] = {
                  0xd3,0xc5,0xd5,0x92 ,0x32,0x7f,0xb1,0x1c,
                  0x40,0x35,0xc6,0x68 ,0x0a,0xf8,0xc6,0xd1
                };
   U8 nonce[16] = { 
                    0x39,0x8a,0x59,0xb4 ,0xac,0x00,0x00,0x00,
                    0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00
                  };
   U8 data[] = { 
                 0x98,0x1b,0xa6,0x82 ,0x4c,0x1b,0xfb,0x1a,
                 0xb4,0x85,0x47,0x20 ,0x29,0xb7,0x1d,0x80,
                 0x8c,0xe3,0x3e,0x2c ,0xc3,0xc0,0xb5,0xfc,
                 0x1f,0x3d,0xe8,0xa6 ,0xdc,0x66,0xb1,0xf0
               };
   U16 dataLen = 253;
   U8 exptdOutput[] = {
               0xe9,0xfe,0xd8,0xa6 ,0x3d,0x15,0x53,0x04,
               0xd7,0x1d,0xf2,0x0b ,0xf3,0xe8,0x22,0x14,
               0xb2,0x0e,0xd7,0xda ,0xd2,0xf2,0x33,0xdc,
               0x3c,0x22,0xd7,0xbd ,0xee,0xed,0x8e,0x78
             };

   VB_MME_SEC_TRC1(testCase6);

   VB_MME_SEC_PRNT_INFO(("Test Case 6: AES-128 CTR Mode Encryption"))

   VB_MME_SEC_PRNT_HEX("Key", key, 16);
   VB_MME_SEC_PRNT_HEX("Data", data, (dataLen + 7) / 8);
   VB_MME_SEC_PRNT_HEX("Expected output", exptdOutput, (dataLen + 7) / 8); 
   vbAESCTR(key, 128, nonce, 128, data, 253);
   VB_MME_SEC_PRNT_HEX("Output", data, (dataLen + 7) / 8); 
   vbAESCTR(key, 128, nonce, 128, data, 253);
   VB_MME_SEC_PRNT_HEX("Output", data, (dataLen + 7) / 8); 
   vbAESCTR(key, 128, nonce, 128, data, 253);
   VB_MME_SEC_PRNT_HEX("Output", data, (dataLen + 7) / 8); 

   RETVALUE(ROK);
}

S16 testCase7()
{
   U8 key[]  = {
                0x2b,0xd6,0x45,0x9f ,0x82,0xc5,0xb3,0x00,
                0x95,0x2c,0x49,0x10 ,0x48,0x81,0xff,0x48
               };
   U8 data[] = {
                0x38,0xa6,0xf0,0x56 ,0xc0,0x00,0x00,0x00,
                0x33,0x32,0x34,0x62 ,0x63,0x39,0x38,0x40
               };
   U8 dataLen = 122;
   U8 exptdOutput[] = {
                       0x11, 0x8c, 0x6e, 0xb8
                      };
   U8 mac[4];

   VB_MME_SEC_TRC1(testCase7);
   VB_MME_SEC_PRNT_INFO(("Test Case 7: AES-128 CMAC Mode Integrity(TestCase 1)"));

   VB_MME_SEC_PRNT_HEX("Key", key, 16);
   VB_MME_SEC_PRNT_HEX("Data", data, (dataLen + 7) / 8);
   VB_MME_SEC_PRNT_HEX("Expected MAC", exptdOutput, 4);
   vbAESCMAC(key,128, data, 122, mac, 4);
   VB_MME_SEC_PRNT_HEX("MAC", mac, 4);
   RETVALUE(ROK);
}

S16 testCase8()
{
   U32 Count_I    = 0x398a59b4;
   U8 Bearer      = 0x1a;
   U8 Direction   = 0x1;
   U8 IK[]        = {
                     0xd3,0xc5,0xd5,0x92 ,0x32,0x7f,0xb1,0x1c,
                     0x40,0x35,0xc6,0x68 ,0x0a,0xf8,0xc6,0xd1
                    };
   U8 Length      = 64; /* bits */
   U8 Message[]   = {
                     0x48,0x45,0x83,0xd5 ,0xaf,0xe0,0x82,0xae
                    }; 
   U32 Mac        = 0;
   U32 ExptdMac   = 0xb93787e6;

   VB_MME_SEC_TRC1(testCase8);
   VB_MME_SEC_PRNT_INFO(("Test Case 8: AES-128 CMAC Mode Integrity(TestCase 2)"));

   Mac = vbMmeIntAESAlg(IK, Count_I, Direction, Bearer, Message, Length/8);
   VB_MME_SEC_PRNT_HEX("IK", IK, 16);
   VB_MME_SEC_PRNT_HEX("Message", Message, Length/8);
   VB_MME_SEC_PRNT_HEX("Expected MAC", (U8*)&ExptdMac, 4);
   VB_MME_SEC_PRNT_HEX("MAC", (U8*)&Mac, 4);

   RETVALUE(ROK);
}
