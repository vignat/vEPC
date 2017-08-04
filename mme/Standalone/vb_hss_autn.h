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
 
    Name:   HSS
 
    Type:   C source file
 
    Desc:   Macros required by the HSS authrntication
 
    File:   vb_hss_autn.h
 
    Sid:      vb_hss_autn.h@@/main/1 - Mon Jun 28 19:35:37 2010
 
    Prg:    ad
 
*********************************************************************21*/
 

#ifndef __HSSAUTNH__
#define __HSSAUTNH__


/* VB_MME_AUTH Authentication feature added 
 * New file added */
/******************************************************************************
 * Macros for HSS
 *****************************************************************************/


#define VB_HSS_AUTN_AMF   32783

#define VB_HSS_AUTN_GET_XDOUT(_rand, _ak, _xdout)                    \
{                                                                    \
   U8    _index = 0;                                                 \
   for (_index = 0; _index < VB_HSS_SHARED_KEY_SIZE; _index++)       \
   {                                                                 \
      _xdout[_index] = _ak[_index] ^ _rand[_index];                  \
   }                                                                 \
}

/*AK[bits 0,1,…47] = XDOUT[bits 24,25,…,71]*/
#define VB_HSS_AUTN_GET_AK(_ueInfo, _rand, _ak) \
{ \
   U16 _indKey = 0;\
   U16 _indAk = 0;\
   for(_indKey = 3; _indKey <=8; _indKey++) \
   {\
      _ak[_indAk] = _ueInfo->ueSecInfo.authKey[_indKey] ^ _rand[_indKey]; \
      _indAk++; \
   }\
}

/*CK[bits 0,1,…127] = XDOUT[bits 8,9,….,126,127,0,1,…,6,7]*/
#define VB_HSS_AUTN_GET_CK(_ueInfo, _rand, _ck) \
{ \
   U16 _indKey = 0;\
   U16 _indCk = 0;\
   for(_indKey = 1; _indKey <=15; _indKey++) \
   {\
      _ck[_indCk] = _ueInfo->ueSecInfo.authKey[_indKey] ^ _rand[_indKey]; \
      _indCk++; \
   }\
   _ck[_indCk] = _ueInfo->ueSecInfo.authKey[0] ^ _rand[0]; \
}


/*RES[bits 0,1,…127] = XDOUT[bits 0,1,….127]*/
/* Optimizing by calculating the CK and RES together */
#define VB_HSS_AUTN_GET_XRES(_ueInfo, _rand, _res) \
{ \
   U16 _indKey = 0;\
   for(_indKey = 0; _indKey <= 15; _indKey++) \
   {\
      _res[_indKey] = _ueInfo->ueSecInfo.authKey[_indKey] ^ _rand[_indKey]; \
   }\
}

/*CK[bits 0,1,…127] = XDOUT[bits 8,9,….,126,127,0,1,…,6,7]*/
/*RES[bits 0,1,…127] = XDOUT[bits 0,1,….127]*/
/* Optimizing by calculating the CK and RES together */
#define VB_HSS_AUTN_GET_CK_N_XRES(_ueInfo, _rand, _ck, _res) \
{ \
   U16 _indKey = 0;\
   U16 _indCk = 0;\
   for(_indKey = 1; _indKey <=15; _indKey++) \
   {\
      _ck[_indCk] = _ueInfo->ueSecInfo.authKey[_indKey] ^ _rand[_indKey]; \
      _res[_indKey] = _ck[_indCk];\
      _indCk++; \
   }\
   _ck[_indCk] = _ueInfo->ueSecInfo.authKey[0] ^ _rand[0]; \
   _res[0] = _ck[_indCk];\
}

/*IK[bits 0,1,…127] = XDOUT[bits 16,17,….126,127,0,1,….14,15]*/
#define VB_HSS_AUTN_GET_IK(_ueInfo, _rand, _ik) \
{ \
   U16 _indKey = 0;\
   U16 _indIk = 0;\
   for(_indKey = 2; _indKey <=15; _indKey++) \
   {\
      _ik[_indIk] = _ueInfo->ueSecInfo.authKey[_indKey] ^ _rand[_indKey]; \
      _indIk++; \
   }\
   _ik[_indIk] = ueInfo->ueSecInfo.authKey[0] ^ _rand[0]; \
   _indIk ++; \
   _ik[_indIk] = _ueInfo->ueSecInfo.authKey[1] ^ _rand[1]; \
}

/*	CDOUT[bits 0,1,…62,63] = SQN[bits 0,1,…46,47] || AMF[bits 0,1,…14,15]*/
#define VB_HSS_AUTN_GET_CDOUT(_sqn, _amf, _cdOut) \
{ \
   U16 _indKey = 0;\
   for(_indKey = 0; _indKey <=5; _indKey++) \
   {\
      _cdOut[_indKey] = _sqn[_indKey]; \
   }\
   _cdOut[6] = _amf[0]; \
   _cdOut[7] = _amf[1]; \
}

/*MAC[bits 0,1,…62,63] = XDOUT[bits 0,1,…62,63] xor CDOUT[bits 0,1,…63]*/
#define VB_HSS_AUTN_GET_MAC(_ueInfo, _rand, _cdOut, _mac) \
{ \
   U16 _indKey = 0;\
   for(_indKey = 0; _indKey <=7; _indKey++) \
   {\
      _mac[_indKey] = _ueInfo->ueSecInfo.authKey[_indKey] ^ _rand[_indKey]; \
   }\
   for(_indKey = 0; _indKey <=7; _indKey++) \
   {\
      _mac[_indKey] = _mac[_indKey] ^ _cdOut[_indKey]; \
   }\
}

/*Compute AUTN using the formula: (SQN xor AK) || AMF || MAC  */
#define VB_HSS_AUTN_GET_AUTN(_ueInfo, _rand, _ak, _sqn, _amf, _mac, _autn) \
{ \
   U16 _indKey = 0;\
   U16 _indAutn = 0;\
   for(_indKey = 0; _indKey <=5; _indKey++) \
   {\
      _autn[_indKey] = _sqn[_indKey] ^ _ak[_indKey]; \
   }\
   _autn[6] = _amf[0]; \
   _autn[7] = _amf[1]; \
   _indAutn = 8; \
   for(_indKey = 0; _indKey <=7; _indKey++) \
   {\
      _autn[_indAutn] = _mac[_indKey]; \
      _indAutn ++; \
   }\
}

#define VB_HSS_AUTN_GET_RAND(_rand) \
{ \
   U32 _randT = 0; \
   _randT = rand(); \
   _rand[0] = _randT; \
   _randT = _randT >> 8; \
   _rand[1] = _randT; \
   _randT = _randT >> 8; \
   _rand[2] = _randT; \
   _randT = _randT >> 8; \
   _rand[3] = _randT; \
   cmMemcpy((U8 *)(_rand + 4), (U8 *)_rand, 4); \
   cmMemcpy((U8 *)(_rand + 8), (U8 *)_rand, 4); \
   cmMemcpy((U8 *)(_rand + 12), (U8 *)_rand, 4); \
}

/* SQN = SEQHE(43bits, ) || IND(5 bits) */
/* MSB 43 bits of SQN = SEQ, i.e. at 0th to 43rd bit
 *   SQN[5]  SQN[4]   SQN[3]  SQN[2]    SQN[1]  SQN[0]
 * 00000 111 11111111 11111111 11111111 11111111 11111111
 * ----- ------------------------------------------------
 *   |                          |                     
 *  IND                        SEQ = 0x07FFFFFFFFFF
 *  */
#define VB_HSS_AUTN_GET_SQN(_ueInfo) \
{ \
   U8 _cnt = 0; \
   Bool _incr = FALSE; \
   if(_ueInfo->ueSecInfo.sqnHE[5] & 0x7)\
   {\
      /* reached the maximum value in bits, reset to 0 \
       * increment in next higher bits */\
      _ueInfo->ueSecInfo.sqnHE[5] = 0;\
   }\
   else\
   {\
      /* incrementing in LSB
       * no need to go to the higher bits*/\
      _ueInfo->ueSecInfo.sqnHE[5]++;\
      _incr = TRUE;\
   }\
   for(_cnt = 5; (_cnt != 0) && (_incr == FALSE); _cnt--) \
   {\
      if((_ueInfo->ueSecInfo.sqnHE[_cnt-1] != 0xFF) && (_incr == FALSE)) \
      { \
         /* incrementing in this LSB
          * no need to go to the higher bits*/\
         _ueInfo->ueSecInfo.sqnHE[_cnt-1]++; \
         _incr = TRUE; \
      } \
      if ((_ueInfo->ueSecInfo.sqnHE[_cnt-1] == 0xFF) && (_incr == FALSE))\
      { \
         /* reached the maximum value in bits, reset to 0 \
          * increment in next higher bits */\
         _ueInfo->ueSecInfo.sqnHE[_cnt-1] = 0; \
      } \
   }\
}

/* LSB 5 bits of SQN = IND,  are at 44th to 48th bit
 *   SQN[5]    SQN[4]   SQN[3]  SQN[2]    SQN[1]  SQN[0]
 * 11111 000 00000000 00000000 00000000 00000000 00000000
 * ----- ------------------------------------------------
 *   |                          |                     
 *  IND(0xF8)                   SEQ                       
 *  */
#define VB_HSS_AUTN_GET_IND(_ueInfo) \
{\
   if(_ueInfo->ueSecInfo.sqnHE[5] == 0xF8) \
   { \
         /* Rollback to 0 */ \
         _ueInfo->ueSecInfo.sqnHE[5] = (_ueInfo->ueSecInfo.sqnHE[5] & 0X07); \
   } \
   else \
   { \
      if((_ueInfo->ueSecInfo.sqnHE[5] & 0xF0) == 0xF0)\
      {\
         /* The next higher bit has to be 0 since, it s not 0xF8 \
          * hence control reached here \
          * so to increment just set the next higher bit*/\
         _ueInfo->ueSecInfo.sqnHE[5] = _ueInfo->ueSecInfo.sqnHE[5] | 0x08;\
      }\
      else\
      {\
         /* the lower 4 bits have not yet reached 0XF
          * so just increment */\
         _ueInfo->ueSecInfo.sqnHE[5]++; \
      }\
   } \
}

#define VB_HSS_AUTN_GET_SQN_MS(_ueInfo, _rand, _sqn, _auts, _ak) \
{ \
   U16 _byte= 0;\
   for(_byte= 0; _byte < 6; _byte++) \
   {\
      _sqn[_byte] = _auts[_byte] ^ _ak[_byte]; \
   }\
}

/*
 * FC = 0x10,
 * P0 = PLMN ID,
 * L0 = length of PLMN ID (i.e. 0x00 0x03),
 * P1 = SQN ¿ AK
 * L1 = length of SQN ¿ AK (i.e. 0x00 0x06) 
#define VB_HSS_AUTN_GET_K_ASME()\
{\
}
*/





#endif /* __HSSAUTNH__ */



/********************************************************************30**

         End of file:     vb_hss_autn.h@@/main/1 - Mon Jun 28 19:35:37 2010
*********************************************************************31*/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---     mm               1. Initial version for LTE_CNE Phase 1.
*********************************************************************91*/
