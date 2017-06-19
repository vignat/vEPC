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

     Name:     HSS

     Type:     C source file

     Desc:     C source code for HSS authentication procedures.

     File:     vb_hss_autn.c
  
     Sid:      vb_hss_autn.c@@/main/1 - Mon Jun 28 19:35:36 2010
 
     Prg:      ad

*********************************************************************21*/

/* Header include files (.h) */
#include <stdlib.h>
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library           */
#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "vb_hss.h"
#ifdef VB_MME_AUTH
#include "vb_hss_autn.h"
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common library function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "sct.x"           /* SCT interface defines           */

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */
/* vb007.101: S1AP include files order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */
#include "vb_hss.x"
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"
#endif /* VB_MME_AUTH */

/* VB_MME_AUTH Authentication feature added  */
#ifdef VB_MME_AUTH
/* AUTN Computation
 *	XDOUT[bits 0,1,….127] = 
 *	   K [bits 0,1,….., 127] xor RAND[bits 0,1,….,127]
 */
PUBLIC S16 vbHssGenXorAuthVectors
(
U8                *imsi,
VbHssUeSecInfo    *vector
)
{
   S16 ret = ROK;
   U8  amf[2] = {0x80, 0x0F};
   U8                cdOut[8] = {'\0'};
   U8                ak[6]    = {'\0'};
   U8                mac[8]   = {'\0'};
   VbHssUeInfo       *ueInfo  = NULLP;

   VB_TRC2(vbHssGenXorAuthVectors)

   ret = vbHssFindImsi(imsi, &ueInfo);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Finding UE Info in HSS DB"));
      RETVALUE(RFAILED);
   }

   VB_HSS_AUTN_GET_AK(ueInfo, ueInfo->ueSecInfo.rand, ak);
   VB_HSS_AUTN_GET_CK(ueInfo, ueInfo->ueSecInfo.rand,\
         ueInfo->ueSecInfo.ck);
   VB_HSS_AUTN_GET_XRES(ueInfo, ueInfo->ueSecInfo.rand,\
         ueInfo->ueSecInfo.xres );
   VB_HSS_AUTN_GET_IK(ueInfo, ueInfo->ueSecInfo.rand, ueInfo->ueSecInfo.ik);
   VB_HSS_AUTN_GET_CDOUT(ueInfo->ueSecInfo.sqnHE, amf, cdOut);
   VB_HSS_AUTN_GET_MAC(ueInfo, ueInfo->ueSecInfo.rand, cdOut, mac);
   VB_HSS_AUTN_GET_AUTN(ueInfo, ueInfo->ueSecInfo.rand, ak,\
         ueInfo->ueSecInfo.sqnHE, amf, mac, ueInfo->ueSecInfo.autn);

   /* nasKsi should be allocated by MME */
   ueInfo->ueSecInfo.nasKsi.pres = PRSNT_NODEF;
   ueInfo->ueSecInfo.nasKsi.id = 0;
   ueInfo->ueSecInfo.nasKsi.tsc = 0;
   ueInfo->ueSecInfo.pres = PRSNT_NODEF;
   cmMemcpy((U8 *)vector, (U8 *)&(ueInfo->ueSecInfo), sizeof(VbHssUeSecInfo));

   /* Increment IND number for next time use */
   VB_HSS_AUTN_GET_IND(ueInfo);

   /* Increment sequence number for next time use */
   VB_HSS_AUTN_GET_SQN(ueInfo);

   RETVALUE(ret);

} /* vbHssGenXorAuthVectors */
#endif /* VB_MME_AUTH */

#ifdef VB_MME_AUTH
PUBLIC S16 vbHssResync(U8 *imsi, U8 *auts, VbHssUeSecInfo* vector)
{
   S16 ret = ROK;

   VbHssUeInfo *ueInfo;
   /*
    * Authentication Failure from UE with cause value "SQN Failure"
    * AMF = 0x00 0x00 0x00 0x00 (all zeroes)
    * XDOUT, AK  calculation same as above
    * CDOUT [bits 0,1,…,63] = SQNMS[bits 0,1,..47] || AMF[bits 0,1,…15]
    * SQNMS = AUTS[bits 0,1,…47] xor AK[bits 0,1,…47] where AUTS is the 
    * received resync parameter from UE.
    */
   U8 amf[2] = {0xFF, 0xFF};

   U8 cdOut[8];
   U8 ak[6]; 
   U8 mac[8];
   VB_TRC2(vbHssResync)
   
   ret = vbHssFindImsi(imsi, &ueInfo);
   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UE CB is not found"));
      RETVALUE(ret);
   }

   /* Generate random number */
   VB_HSS_AUTN_GET_RAND(ueInfo->ueSecInfo.rand);

   VB_HSS_AUTN_GET_AK(ueInfo, ueInfo->ueSecInfo.rand, ak);
   VB_HSS_AUTN_GET_SQN_MS(ueInfo, ueInfo->ueSecInfo.rand,\
         ueInfo->ueSecInfo.sqnMS, auts, ak);
   VB_HSS_AUTN_GET_CDOUT(ueInfo->ueSecInfo.sqnMS, amf, cdOut);
   VB_HSS_AUTN_GET_CK_N_XRES(ueInfo, ueInfo->ueSecInfo.rand,\
         ueInfo->ueSecInfo.ck, ueInfo->ueSecInfo.xres);
   VB_HSS_AUTN_GET_IK(ueInfo, ueInfo->ueSecInfo.rand, ueInfo->ueSecInfo.ik);
   VB_HSS_AUTN_GET_MAC(ueInfo, ueInfo->ueSecInfo.rand, cdOut, mac);
   VB_HSS_AUTN_GET_AUTN(ueInfo, ueInfo->ueSecInfo.rand, ak,\
         ueInfo->ueSecInfo.sqnMS, amf, mac, ueInfo->ueSecInfo.autn);
   ueInfo->ueSecInfo.pres = PRSNT_NODEF;

   /* nasKsi should be allocated by MME */
   ueInfo->ueSecInfo.nasKsi.pres = PRSNT_NODEF;
   ueInfo->ueSecInfo.nasKsi.id = 0;
   ueInfo->ueSecInfo.nasKsi.tsc = 0;
   ueInfo->ueSecInfo.resync = TRUE;
   cmMemcpy((U8 *)vector, (U8 *)&(ueInfo->ueSecInfo), sizeof(VbHssUeSecInfo));
   RETVALUE(ret);
}
#endif /* VB_MME_AUTH */

#ifdef VB_MME_AUTH
EXTERN S16 vbHssUpdSeqNum(U8 *imsi, VbHssUeSecInfo* vector)
{
   S16 ret = ROK;
   VbHssUeInfo *ueInfo;
   U8 byte;

   VB_TRC2(vbHssUpdSeqNum)
   ret = vbHssFindImsi(imsi, &ueInfo);
   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UE CB is not found"));
      RETVALUE(ret);
   }
   if(ueInfo->ueSecInfo.resync == FALSE)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Resync was not initiated to updated "\
            "the Seq numbers.\n"));
      RETVALUE(RFAILED);
   }
   for(byte=0; byte<6; byte++)
   {
      ueInfo->ueSecInfo.sqnHE[byte] = ueInfo->ueSecInfo.sqnMS[byte];
      vector->sqnHE[byte] = vector->sqnMS[byte];
   }
   ueInfo->ueSecInfo.resync = FALSE;
   vector->resync = FALSE;
   RETVALUE(ret);
}
#endif /* VB_MME_AUTH */

/********************************************************************30**
 
         End of file:     vb_hss_autn.c@@/main/1 - Mon Jun 28 19:35:36 2010
 
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
/main/1      ---     mm        1. Initial version for LTE-CNE Phase 1.
/main/1   vb007.101 akaranth   1. S1AP include files order has changed.
*********************************************************************91*/
