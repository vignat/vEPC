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
 
    Desc:   Data Structures required by the HSS. These will be used by HSS only.
 
    File:   vb_hss.x
 
    Sid:      vb_hss.x@@/main/1 - Mon Jun 28 19:35:36 2010
 
    Prg:    ad
 
*********************************************************************21*/

#ifndef __HSSX__
#define __HSSX__


/******************************************************************************
 * Structures for HSS entity
 *****************************************************************************/

/** Token which is used for DB file operations. */
typedef struct _hssTkn
{
   TknStrM  tok;
   U16      nmbArgs;
   TknStrM  args[VB_HSS_DB_MAX_ARGS];
}VbHssTkn;


/* HSS Token List for DB file*/
typedef enum _vbHssDbTkn
{
      VB_HSS_TKN_BEGIN_CFG = 0,  
      VB_HSS_TKN_NUM_UE_ENTRIES,    /*  1 */
      VB_HSS_TKN_UE_ENTRY,          /* 2  */  
      VB_HSS_TKN_IMSI_NUM_DGTS,     /* 3  */  
      VB_HSS_TKN_IMSI,              /* 4  */   
      VB_HSS_TKN_MSISDN,            /* 5  */  
      VB_HSS_TKN_IMEI,              /* 6  */  
      VB_HSS_TKN_IMEISV,            /* 7  */  
      VB_HSS_TKN_MME_ADDR,          /* 8  */  
      VB_HSS_TKN_PURGED,            /* 9  */  
      VB_HSS_TKN_UE_AMBR,           /* 10 */ 
      VB_HSS_TKN_CNTXT_ID,          /* 11 */   
      VB_HSS_TKN_PDN_ADDR_NUM,      /* 12 */  
      VB_HSS_TKN_PDN_ADDR_LST,      /* 13 */   
      VB_HSS_TKN_PDN_TYPE,          /* 14 */   
      VB_HSS_TKN_APN,               /* 15 */   
      VB_HSS_TKN_QOS_PROFILE,       /* 16 */   
      VB_HSS_TKN_SUB_APN_AMBR,      /* 17 */   
      VB_HSS_TKN_PDN_GW_ADDR,       /* 18 */   
      VB_HSS_TKN_PDN_GW_ALLC_TYPE,  /* 19 */   
      VB_HSS_TKN_AUTH_KEY,          /* 20 */
      VB_HSS_TKN_OP_KEY,            /* 21 */
      VB_HSS_TKN_ROAM_STATUS,       /* 22 */
      VB_HSS_TKN_REDIR_STATUS,      /* 23 */
      VB_HSS_TKN_AUTH_TYPE,         /* 24 */
      VB_HSS_TKN_INTEGRITY_TYPE,    /* 25 */
      VB_HSS_TKN_CIPHERING_TYPE,    /* 26 */
      VB_HSS_TKN_UE_STATUS,         /* 27 */
      VB_HSS_TKN_END_CFG = 50
}VbHssDbTkn;

/** HSS UE Database */
typedef struct _hssUeDB
{
   U32            numUEEntries;           /* Number of UE entries in the DB*/
   VbHssUeInfo    ueInfo[VB_HSS_MAX_UES]; /* UE Information*/   
   CmLListCp      xaTransLst;             /**< Xa message transactin List */
}VbHssUeDB;

EXTERN VbHssUeDB  vbHssUeDB;          /* HSS Ue DB*/

/* HSS Function Prototypes */
EXTERN S16 vbHssReadUeDB  ARGS((Void));
EXTERN S16 vbHssWriteUeDB  ARGS((Void ));
/* VB_MME_AUTH Authentication feature added 
 * Passing double pointer to ueInfo */
EXTERN S16 vbHssFindImsi ARGS(( U8  *imsi, VbHssUeInfo **ueInfo ));

/**************************************************************************//**
 *
 * @brief 
 *    Resets UE Security context details
 *             
 * @param[in]  pImsi
 *    IMSI
 * @param[in]  imsiLen
 *    Length of IMSI
 * 
 * @return
 *
 *****************************************************************************/
PUBLIC Void vbHssResetSecCtxt
(
U8       *pImsi
);

#endif /*  __HSSX__ */


/********************************************************************30**
         End of file:     vb_hss.x@@/main/1 - Mon Jun 28 19:35:36 2010
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
/main/1      ---     ad         1. Initial version for LTE-CNE Phase 1.
*********************************************************************91*/
