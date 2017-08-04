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

     Name:    Trillium - AES Mode Module

     Type:    C source file

     Desc:    This file contains c source code for AES mode module 
              Reference for CTR mode is: 
              - NIST Special Publication 800-38A (2001): 
                "Recommendation for Block Cipher Modes of Operation".
              Reference for CMAC mode is: 
              - NIST Special Publication 800-38B (2001): 
                "Recommendation for Block Cipher Modes of Operation: The CMAC Mode for Authentication".

     File:    vb_aes_mode.c

     Sid:     vb_aes_mode.c@@/main/1 - Mon Jun 01 19:35:38 2012

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
#include "vb_aes_mode.h"   /* AES mode defines                */
#include "aes.h"           /* External AES include            */

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
#include "vb_aes_mode.x"   /* AES mode declarations           */


/* Global variable declaration */
CONSTANT  PRIVATE U8 VB_RB[VB_AES_BLOCK_SIZE] = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87
  };


/* Private function declarations */
PRIVATE Void vbAES128 ARGS(( U8   *key,
                             U8   *input,
                             U8   *output
                          ));

PRIVATE Void vbPadding ARGS(( U8   *data,
                              U8   paddingBits
                           ));

PRIVATE Void vbLeftShiftByOneBit ARGS(( U8   *in,
                                        U8   *out
                                     ));

PRIVATE Void vbXOR128 ARGS(( CONSTANT U8  *in1,
                             CONSTANT U8  *in2,
                             U8           *out
                          ));

PRIVATE Void vbGenSubKey ARGS(( U8   *key,
                                U8   *K1,
                                U8   *K2
                             ));

/********************************************************************
 *
 *       Fun:   vbAES128
 *       
 *       Desc:  Makes the call to external AES library 
 *
 *       Input: 
 *              key:   in with 16 bytes
 *              input: in with 16 bytes  
 *              output: out with 16 bytes
 *
 *       Ret:  None 
 *       
 *       Notes: 
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbAES128
(
U8   *key,
U8   *input, 
U8   *output
)
#else
PRIVATE Void vbAES128(key, input, output)
U8   *key;
U8   *input; 
U8   *output;
#endif
{
   aesParam param;
   aesSetup(&param, key, 128, ENCRYPT);
   aesEncrypt(&param, (uint32_t*) output, (const uint32_t*) input);
}


/********************************************************************
 *
 *       Fun:   vbPadding
 *       
 *       Desc:  Pads  10* to data of length paddingBits 
 *
 *       Input: 
 *              data:   in/out with 16 bytes
 *              paddingBits: number of bits to pad  
 *
 *       Ret:  None 
 *       
 *       Notes: initialization of data with zero's assumed
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbPadding
(
U8   *data,
U8   paddingBits
)
#else
PRIVATE Void vbPadding(data, paddingBits)
U8   *data;
U8   paddingBits;
#endif
{
   U8  fromByte;
   U8  fromBit;

   fromByte = VB_AES_BLOCK_SIZE - ((paddingBits+7)/8);
   fromBit = (8 - (paddingBits % 8)) % 8;
  
   data[fromByte] = data[fromByte] |(1 <<(7-fromBit));

   RETVOID;
}


/********************************************************************
 *
 *       Fun:   vbLeftShiftByOneBit
 *       
 *       Desc:  Shift the input data by 1 bit to left
 *
 *       Input: 
 *              in:   in with 16 bytes
 *              out: out with 16 bytes 
 *
 *       Ret:  None 
 *       
 *       Notes: 
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbLeftShiftByOneBit
(
U8   *in, 
U8   *out
)
#else
PRIVATE Void vbLeftShiftByOneBit(in, out)
U8   *in; 
U8   *out;
#endif
{
   U8   indx;
   U8   overflow = 0;

   for(indx = VB_AES_BLOCK_SIZE; indx > 0; indx--) 
   {
       out[indx-1] = in[indx-1] << 1;
       out[indx-1] |= overflow;
       overflow = (in[indx-1] & 0x80)?1:0;
   }
   
   RETVOID;
}

/********************************************************************
 *
 *       Fun:   vbXOR128
 *       
 *       Desc:  Performs XOR operation on given input
 *
 *       Input: 
 *              in1, in2 : in with 16 bytes
 *              out: out with 16 bytes 
 *
 *       Ret:  None 
 *       
 *       Notes: 
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbXOR128
(
CONSTANT U8  *in1, 
CONSTANT U8  *in2, 
U8  *out
)
#else
PRIVATE Void vbXOR128(in1, in2, out)
U8  *in1; 
U8  *in2; 
U8  *out;
#endif
{
   U8  indx;

   for(indx = 0; indx < VB_AES_BLOCK_SIZE; indx++)
   {
       out[indx] = in1[indx] ^ in2[indx];
   }

   RETVOID;
}

/********************************************************************
 *
 *       Fun:   vbGenSubKey
 *       
 *       Desc:  Generate subkeys from CMAC algorithm
 *  
 *       Input: 
 *              key : in with 16 bytes
 *              K1, K2: out with 16 bytes 
 *
 *       Ret:  None 
 *       
 *       Notes: 
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/
#ifdef ANSI
PRIVATE Void vbGenSubKey
(
U8   *key, 
U8   *K1, 
U8   *K2
)
#else
PRIVATE Void vbGenSubKey(key, K1, K2)
U8   *key; 
U8   *K1;
U8   *K2;
#endif
{
   U8   L[VB_AES_BLOCK_SIZE] = {0};
   U8   Z[VB_AES_BLOCK_SIZE] = {0};
   U8   tmp[VB_AES_BLOCK_SIZE] = {0};

   /* Step 1: L = CIPHK(0b) */
   vbAES128(key,Z,L);

   /* Step 2: If MSB1(L) = 0, then K1 = L << 1; 
              Else K1 = (L << 1) ⊕ Rb; see Sec. 5.3 for the definition of Rb */

   if((L[0] & 0x80) == 0)            /* If MSB(L) = 0, then */ 
   {
      vbLeftShiftByOneBit(L, K1);    /* K1 = L << 1 */
   } 
   else                              /* Else */
   {    
      vbLeftShiftByOneBit(L,tmp);    /* tmp = L << 1 */
      vbXOR128(tmp, VB_RB, K1);       /* K1 = ( L << 1 ) (+) Rb */
   }

   /* Step 3: If MSB1(K1) = 0, then K2 = K1 << 1; 
              Else K2 = (K1 << 1) ⊕ Rb */
   if ( (K1[0] & 0x80) == 0 )        /* If MSB1(K1) = 0, then */ 
   {
      vbLeftShiftByOneBit(K1,K2);    /* K2 = K1 << 1 */
   } 
   else 
   {
      vbLeftShiftByOneBit(K1, tmp);  /* tmp = K1 << 1 */
      vbXOR128(tmp, VB_RB, K2);      /* K2 = (K1 << 1) ⊕ Rb */
   }

   /* Step 4: Return K1, K2 */
   RETVOID;
}

/********************************************************************
 *
 *       Fun:   vbAESCMAC
 *       
 *       Desc:  Generates MAC using AES CMAC algorithm
 *   
 *       Input: 
 *              key : in with bitKeyLen bits
 *              data : in with bitDataLen bits
 *              mac: out with byteMacLen bytes 
 *
 *       Ret:  ROK - Success
 *             RFAILED - Failure
 *       
 *       Notes: 
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/
#ifdef ANSI
PUBLIC U16 vbAESCMAC
(
U8    *key,
U16   bitKeyLen, /* Bits Len */
U8    *data,
U32   bitDataLen, /* Bits Len */
U8    *mac,
U8    byteMacLen
)
#else
PUBLIC U16 vbAESCMAC(key, bitKeyLen, data, bitDataLen, mac, byteMacLen)
U8    *key;
U16   bitKeyLen; /* Bits Len */
U8    *data;
U32   bitDataLen; /* Bits Len */
U8    *mac;
U8    byteMacLen;
#endif
{
   U8    mnStar[VB_AES_BLOCK_SIZE] = {0};
   U8    mn[VB_AES_BLOCK_SIZE] = {0};
   U8    c[VB_AES_BLOCK_SIZE] = {0};
   U8    tmp[VB_AES_BLOCK_SIZE] = {0};
   U8    mnStarFlag = 0;
   U8    K1[VB_AES_BLOCK_SIZE];
   U8    K2[VB_AES_BLOCK_SIZE];
   U8    mnStarPaddingBitLen = 0;
   U16   n = 0;
   U16   indx;
   U16   byteDataLen = 0;

   /* Input validations */
   if((128 != bitKeyLen) || (byteMacLen > VB_AES_BLOCK_SIZE))
   {
      RETVALUE(RFAILED);
   }

   if((NULLP == key) || (NULLP == data) || (NULLP == mac))
   {
      RETVALUE(RFAILED);
   }

   /* Preparation stage */

   /* Convert bit length into byte length */
   byteDataLen = (bitDataLen+7)/8; /* Ceiling */   
   
 
   /* Begin algorithm processing */

   /* Step 1: Generate K1, K2 */
   vbGenSubKey(key, K1, K2);

   /* Step 2: Calculate n */
   if(0 == byteDataLen)
   {
      n = 1;
   }
   else
   {
      n = (byteDataLen + VB_AES_BLOCK_SIZE - 1)/ VB_AES_BLOCK_SIZE; /* Ceiling */
   }

   /* Step 3: Find mnStar 
              - Based on bit length, not on byte length */
   if(0 == bitDataLen)
   {
      mnStarFlag = 0;    /* Block is not complete */
      mnStarPaddingBitLen = VB_AES_BLOCK_SIZE * 8;
   }
   else if(0 != bitDataLen % (8 * VB_AES_BLOCK_SIZE))
   {
      mnStarFlag = 0;    /* Block is not complete */
      /* Last block remaining bits*/
      cmMemcpy(mnStar, &data[VB_AES_BLOCK_SIZE * (n-1)], 
                       (byteDataLen - ((n-1) * VB_AES_BLOCK_SIZE)));  
      /* Number of bits to pad to last block */
      mnStarPaddingBitLen = (8 * VB_AES_BLOCK_SIZE) - 
                            (bitDataLen % (8 * VB_AES_BLOCK_SIZE));
   }
   else
   { 
      mnStarFlag = 1;    /* Block is complete */
      cmMemcpy(mnStar, &data[VB_AES_BLOCK_SIZE * (n-1)], VB_AES_BLOCK_SIZE);
   }

   /* Step 4: Calculate mn */
   if(1 == mnStarFlag) 
   {
      /* Mn  = K1 ⊕ Mn* */
      vbXOR128(K1, mnStar, mn);
   }
   else
   {
      /* Mn  = K2 ⊕ (Mn *||10j) */
      vbPadding(mnStar, mnStarPaddingBitLen);
      vbXOR128(K2, mnStar, mn);
   }

   /* Step 5: Let C0 = 0b */
   /* c[VB_AES_BLOCK_SIZE] = {0} is initialized to zeros */

   /* Step 6: For i = 1 to n, let Ci = CIPHK(Ci-1 ⊕ Mi) */
   for (indx = 1; indx < n; indx++) 
   {
      vbXOR128(&data[16 * (indx-1)], c, tmp); /* Y := Mi (+) X  */
      vbAES128(key,tmp,c);      /* X := AES-128(KEY, Y); */
   }

   /* Last round */
   vbXOR128(mn, c, tmp);
   vbAES128(key, tmp, c);

   /* Step 7: Let T = MSBTlen(Cn). */
   for(indx = 0; indx < byteMacLen; indx++ ) 
   {
      mac[indx] = c[indx];
   }
   
   /* Step 8: Return mac */
   RETVALUE(ROK);
}

/********************************************************************
 *
 *       Fun:   vbAESCTR
 *       
 *       Desc:  Encrypt data using AES CTR algorithm
 *    
 *       Input:  
 *              key : in with bitKeyLen bits
 *              counterBlk : in with bitCounterBlkLen bits
 *              data: in/out with bitDataLen bits
 *    
 *       Ret:  ROK - Success
 *             RFAILED - Failure
 *       
 *       Notes: 
 *              
 *       File: vb_aes_mode.c
 ********************************************************************/


#ifdef ANSI
PUBLIC S16 vbAESCTR
(
U8    *key, 
U16   bitKeyLen,
U8    *counterBlk,
U16   bitCounterBlkLen,
U8    *data, 
U32   bitDataLen
)
#else
PUBLIC S16 vbAESCTR(key, bitKeyLen, counterBlk, bitCounterBlkLen, bitDataLen)
U8    *key; 
U16   bitKeyLen;
U8    *counterBlk,
U16   bitCounterBlkLen;
U8    *data;
U32   bitDataLen;
#endif
{
   S16         indx;
   aesParam    param;
   U8          keyStrmBlkI[VB_AES_BLOCK_SIZE];
   U8          cntrBlkI[VB_AES_BLOCK_SIZE];
   U32         leftLen = (bitDataLen+7) / 8; /* Ceiling */ 
   U16         blockLen;
   U32         dataPos = 0;

   if((NULLP == key) || (NULLP == counterBlk) || (NULLP == data))
   {
      RETVALUE(RFAILED);
   }
   if((128 != bitKeyLen) || (128 != bitCounterBlkLen))
   {
      RETVALUE(RFAILED);
   }
   
   /* Make copy of counterBlk */
   cmMemcpy(cntrBlkI, counterBlk, bitCounterBlkLen/8); 

   /* Setup key with AES */
   aesSetup(&param, key, bitKeyLen, ENCRYPT);

   while(leftLen)
   {
      /* Generate key stream block i */
      aesEncrypt(&param, (uint32_t*) keyStrmBlkI, (const uint32_t*) cntrBlkI);

      /* Make a block of data of length VB_AES_BLOCK_SIZE */ 
      blockLen = (leftLen > VB_AES_BLOCK_SIZE)? VB_AES_BLOCK_SIZE : leftLen;

      /* Encrypt block of data using key stream i*/
      for(indx = 0; indx < blockLen; indx++)
      {
         data[dataPos + indx] ^= keyStrmBlkI[indx];
      }

      /* Move to next block */
      dataPos += blockLen;
      leftLen -= blockLen;

      /* Increment the counter block using 2mod64*/
      for(indx = VB_AES_BLOCK_SIZE - 1; indx >= (VB_AES_BLOCK_SIZE/2); indx--)
      {
         cntrBlkI[indx]++;
         if (cntrBlkI[indx])
         {
            break;
         }
      }
   }

   RETVALUE(ROK);
}

