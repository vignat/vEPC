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
 
    Name:   HSS
 
    Type:   C source file
 
    Desc:   Macros required by the HSS 
 
    File:   vb_hss.h
 
    Sid:      vb_hss.h@@/main/eNB_3.1_mme_DwPh0_br/1 - Wed Dec 14 23:08:11 2011
 
    Prg:    ad
 
*********************************************************************21*/
 

#ifndef __HSSH__
#define __HSSH__


/******************************************************************************
 * Macros for HSS
 *****************************************************************************/


/* Maximum Number of UEs in HSS Batabase */
#define VB_HSS_MAX_UES          1200

#define VB_HSS_IMSI_LEN        15
#define VB_HSS_MSISDN_LEN      10
#define VB_HSS_IMEI_LEN        15
#define VB_HSS_IMEISV_LEN      16
#define VB_HSS_AUTN_LEN        255

/* KeyWord String length */
#define VB_HSS_SLEN          64        

/* Maximum Number of PDN Subscriptions Context*/
#define VB_HSS_MAX_PDN_SUB_CNTXT_NUM   8

/* Maximum PDN Addresses */
#define VB_HSS_MAX_PDN_ADDR_NUM     5

/* Maximum APN Length*/
#define VB_HSS_APN_LEN         100 /*KWORKS Fix - Updated according to CM_ESM_MAX_LEN_ACCESS_PTNAME*/

/* For Auth */
#define VB_HSS_RAND_LEN       16
#define VB_HSS_XRES_LEN       16
#define VB_HSS_KASME_LEN      16
#define VB_HSS_KENB_LEN      16
#define VB_HSS_CK_LEN         16
#define VB_HSS_IK_LEN         16

#define VB_HSS_DB_LINE_LEN    100 
#define VB_HSS_DB_MAX_ARGS    100
#define VB_HSS_MAX_U8        0xff

/* Number for elements in struct HssAMBR */
#define VB_HSS_NUM_AMBR_COMP     6

/* Number for elements in struct HssEPSSubQOSPrf */
#define VB_HSS_NUM_QOS_COMP     12

/* UE Status */
#define VB_HSS_UE_NOTAVAILABLE  0 /* Switched off/detached/out of coverage */
#define VB_HSS_UE_AVAILABLE     1 /* After CS attach */
#define VB_HSS_UE_ENGAGED       2 /* Busy/Call going on */

#define VB_HSS_MAX_PDN_NET_ADDRS     5
/* vb006.101: Modified to 35 from 5 to accomidate 35 UE's */
#define VB_HSS_MAX_PDN_SUB_CTXT      VB_MME_MAX_PDN_CONN  /* This parameter gives no of ue subscription profile */

/* HSS Debug Macro */
#define VB_HSS_DEBUGP(_arg) \
{                        \
Txt tmpBuf[255];         \
sprintf(tmpBuf, "[%s ] %s:%d ", "HSS", __FILE__, __LINE__); \
SPrint(tmpBuf);          \
SPrint(_arg);\
}



#define VB_MME_HSS_CTX_MSISDN_PRESENT        (1 << 0)

#define VB_HSS_GET_TRANS_ID(_id)                                     \
{                                                                    \
   (_id) = ++gXaTransId;                                             \
}

#endif /* __HSSH__ */



/********************************************************************30**

         End of file:     vb_hss.h@@/main/eNB_3.1_mme_DwPh0_br/1 - Wed Dec 14 23:08:11 2011
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
/main/1      ---     ad               1. Initial version for LTE_CNE Phase 1.
/main/1    vb006.101 ragrawal         1. Modified VB_HSS_MAX_PDN_SUB_CTXT to 35 
                                         from 5 to accomidate 35 UE's.
*********************************************************************91*/
