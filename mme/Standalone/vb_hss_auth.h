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
  
     Name:     HSS Authentication Module
  
     Type:     C include file
  
     Desc:     C defines for authentication module.
   
     File:     vb_hss_auth.h
  
     Sid:      vb_hss_auth.h@@/main/1156 - Mon Nov 21 11:09:25 2011
   
     Prg:      sk
  
*********************************************************************21*/
#ifndef __VB_MME_HSS_H__
#define __VB_MME_HSS_H__ 1

/* HSS specific defines */
#define VB_HSS_SEQ_NMB_SIZE  6
#define VB_HSS_AK_SIZE       6
#define VB_HSS_MAC_SIZE      8
#define VB_HSS_CK_SIZE      16
#define VB_HSS_IK_SIZE      16
#define VB_HSS_SHARED_KEY_SIZE  16
#define VB_HSS_OP_KEY_SIZE  16

#define VB_MME_HSS_MAC_FAILURE  0x14
#define VB_MME_HSS_SYNC_FAILURE 0x15

/* Common to HSS and MME */
#define VB_RES_SIZE         16
#define VB_AUTS_SIZE        14
#define VB_AUTN_SIZE        16
#define VB_RAND_SIZE        16
#define VB_KASME_SIZE       32
#define VB_NEXT_HOP_SIZE    32

/*AK[bits 0,1,<85>47] = XDOUT[bits 24,25,<85>,71]*/
#define HSS_AUTN_GET_AK(_k, _rand, _ak) \
{ \
   U16 _indKey = 0;\
   U16 _indAk = 0;\
   for(_indKey = 3; _indKey <=8; _indKey++) \
   {\
      _ak[_indAk] = _k[_indKey] ^ _rand[_indKey]; \
      _indAk++; \
   }\
}

/*CK[bits 0,1,<85>127] = XDOUT[bits 8,9,<85>.,126,127,0,1,<85>,6,7]*/
/*RES[bits 0,1,<85>127] = XDOUT[bits 0,1,<85>.127]*/
/* Optimizing by calculating the CK and RES together */
#define HSS_AUTN_GET_CK_N_RES(_k, _rand, _ck, _res) \
{ \
   U16 _indKey = 0;\
   U16 _indCk = 0;\
   for(_indKey = 1; _indKey <=15; _indKey++) \
   {\
      _ck[_indCk] = _k[_indKey] ^ _rand[_indKey]; \
      _res[_indKey] = _ck[_indCk];\
      _indCk++; \
   }\
   _ck[_indCk] = _k[0] ^ _rand[0]; \
   _res[0] = _ck[_indCk];\
}

/*IK[bits 0,1,<85>127] = XDOUT[bits 16,17,<85>.126,127,0,1,<85>.14,15]*/
#define HSS_AUTN_GET_IK(_k, _rand, _ik) \
{ \
   U16 _indKey = 0;\
   U16 _indIk = 0;\
   for(_indKey = 2; _indKey <=15; _indKey++) \
   {\
      _ik[_indIk] = _k[_indKey] ^ _rand[_indKey]; \
      _indIk++; \
   }\
   _ik[_indIk] = _k[0] ^ _rand[0]; \
   _indIk++; \
   _ik[_indIk] = _k[1] ^ _rand[1]; \
}

/* CDOUT[bits 0,1,<85>62,63] = SQN[bits 0,1,<85>46,47] || AMF[bits 0,1,<85>14,15]*/
#define HSS_AUTN_GET_CDOUT(_sqn, _amf, _cdOut) \
{ \
   U16 _indKey = 0;\
   for(_indKey = 0; _indKey <=5; _indKey++) \
   {\
      _cdOut[_indKey] = _sqn[_indKey]; \
   }\
   _cdOut[6] = _amf[0]; \
   _cdOut[7] = _amf[1]; \
}

/*MAC[bits 0,1,<85>62,63] = XDOUT[bits 0,1,<85>62,63] xor CDOUT[bits 0,1,<85>63]*/
#define HSS_AUTN_GET_MAC(_k, _rand, _cdOut, _mac) \
{ \
   U16 _indKey = 0;\
   for(_indKey = 0; _indKey <=7; _indKey++) \
   {\
      _mac[_indKey] = _k[_indKey] ^ _rand[_indKey]; \
   }\
   for(_indKey = 0; _indKey <=7; _indKey++) \
   {\
      _mac[_indKey] = _mac[_indKey] ^ _cdOut[_indKey]; \
   }\
} 

#endif /*__VB_HSS_AUTH_H__*/

