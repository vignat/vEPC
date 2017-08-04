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
  
     Desc:     C defines for security module.
   
     File:     vb_mme_sec.h
  
     Sid:      vb_mme_sec.h@@/main/1156 - Mon Nov 21 11:09:25 2011
   
     Prg:      sk
  
*********************************************************************21*/

#ifndef __VB_MME_SEC_H__
#define __VB_MME_SEC_H__


#define VB_MME_SEC_HT_PLAIN_NAS_MSG               0x00
#define VB_MME_SEC_HT_INT_PRTD                    0x01
#define VB_MME_SEC_HT_INT_PRTD_ENC                0x02
#define VB_MME_SEC_HT_INT_PRTD_NEW_SEC_CTXT       0x03
#define VB_MME_SEC_HT_INT_PRTD_ENC_NEW_SEC_CTXT   0x04
#define VB_MME_SEC_HT_SER_REQ                     0x0C

/* For indicating to MME controller from lower layer */
#define VB_MME_SEC_SUCCESS                        0xFF  /*Security Validation Successful*/
#define VB_MME_SEC_FAILURE                        0xFE  /*Security Validation Failed*/
#define VB_MME_SEC_NOTPRTD                        0xFC  /*Security Not Protected*/
#define VB_MME_SEC_INTPRTD                        0xFB  /*Integrity Protected*/
#define VB_MME_SEC_SERVREQ                        0xFA  /*Serv Request*/

#define VB_MME_SZ_KASME_KEY                       32
#define VB_MME_SZ_NAS_ENC_KEY                     16
#define VB_MME_SZ_NAS_INT_KEY                     16
#define VB_MME_SZ_ENB_KEY                         32
#define VB_MME_SZ_NEXT_HOP_KEY                    32

#define VB_MME_ENC_MAX_ALG                        7
#define VB_MME_INT_MAX_ALG                        7

#define VB_MME_SEC_CTXT_NATV                      0    
#define VB_MME_SEC_CTXT_NON_NATV                  1
    
#define VB_MME_SEC_CTXT_NON_CRNT                  0
#define VB_MME_SEC_CTXT_CRNT                      1

#define VB_MME_NATIVE_KEY                         0
#define VB_MME_MAPPED_KEY                         1

#define VB_MME_NO_KEY_AVAILABLE                   7

#define VB_MME_NAS_SEC_PARAMS_LEN                 6
#define VB_MAX_INTG_MSG_LEN                       994
#define VB_MAX_INTG_COMP_MSG_LEN                  1000

#endif /*__VB_MME_SEC_H__*/
