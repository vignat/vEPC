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

     Name:    Trillium LTE CNE - MME Controller Module

     Type:    C source file

     Desc:    This file contains vb application security module
	          interface code

     File:    vb_sec_intf.c

     Sid:     vb_sec_intf.c@@/main/1 - Mon Jun 28 19:35:38 2010

     Prg:     sk 
*********************************************************************21*/
/* Header include files (.h)                                  */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent           */
#include "envind.h"        /* Environment independent         */
#include "gen.h"           /* General                         */
#include "ssi.h"           /* System services                 */
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
#include "sct.h"           /* SCT interface defines      */ 
#include "egt.h"           /* EG Upper Interface         */ 
#include "leg.h"           /* EG LM Interface            */ 
#include "szt.h"           /* S1AP Upper Interface       */ 
#include "lsz.h"           /* S1AP LM Interface          */ 
#include "szt_asn.h"       /* S1AP ASN                   */ 
#include "lvb.h"           /* CNE Layer management            */
#include "vb_hss.h"        /* CNE Application defines         */
#include "vb.h"            /* CNE Application defines         */
#include "cm_emm.h"        /* CNE Application defines         */
#include "cm_esm.h"        /* CNE Application defines         */
#include "cm_emm_esm.h"    /* EMM/ESM EDM                     */
#include <stdlib.h>        /* For atoi()                      */
#include "vb_hss_autn.h"   /* CNE Application structures      */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#include "vb_hss_auth.h"
#endif

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
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common ASN.1 Library            */
#include "sct.x"           /* SCT interface defines      */ 
#include "egt.x"           /* EG Upper Interface         */ 
#include "leg.x"           /* EG LM Interface            */ 
#include "szt_asn.x"       /* S1AP ASN                   */ 
#include "szt.x"           /* S1AP Upper Interface       */ 
#include "lsz.x"           /* S1AP LM Interface          */ 
#include "lvb.x"           /* CNE Layer management            */
#include "cm_esm.x"        /* CNE Application structures      */
#include "cm_emm.x"        /* CNE Application structures      */
#include "cm_emm_esm.x"    /* CNE Application structures      */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif
#include "vb_hss_common.x" /* CNE Application structures      */
#include "vb.x"            /* CNE Application structures      */
#include "vb_hss.x"        /* CNE Application structures      */
#include "vb_hss_autn.x"   /* CNE Application structures      */

#ifdef VB_MME_NAS_SEC
#include "vb_sec_intf.x"
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

#ifdef VB_MME_NAS_SEC
/*
*
*       Fun:   vbMmeGetAuthVector
*       
*       Desc:  Inteface for auth vector generator function
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_sec_intf.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeGetAuthVector
(
VbMmeUeCb *ueCb
)
#else
PUBLIC S16 vbMmeGetAuthVector(ueCb)
(
VbMmeUeCb *ueCb;
)
#endif
{
   S16             ret          = RFAILED;
   VbHssUeInfo     *ueInfo      = NULLP;
   VbHssAuthVec    hssAuthVec;

   VB_MME_TRC2(vbMmeGetAuthVector);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Finding UE Info in HSS DB"));

   ret = vbHssFindImsi(ueCb->ueCtxt.ueImsi, &ueInfo);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Finding UE Info in HSS DB"));
      RETVALUE(RFAILED);
   }

   cmMemcpy(ueCb->ueCtxt.authInfo.sharedKey, ueInfo->ueSecInfo.authKey,\
         VB_HSS_SHARED_KEY_SIZE);
   cmMemcpy(ueCb->ueCtxt.authInfo.opKey, ueInfo->ueSecInfo.opKey,\
         VB_HSS_OP_KEY_SIZE);
   cmMemcpy(ueCb->ueCtxt.authInfo.seqNmb, ueInfo->ueSecInfo.sqnHE,\
         VB_HSS_SEQ_NMB_SIZE);

   cmMemset((U8 *)&hssAuthVec, 0, sizeof(VbHssAuthVec));
   cmMemcpy(hssAuthVec.vec.rndNmb, ueInfo->ueSecInfo.rand, VB_RAND_SIZE);
   ret = vbHssGenAuthVec(&ueCb->ueCtxt.authInfo, &hssAuthVec,\
         ueCb->ueCtxt.srvPlmn);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Vector Not Found"));
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8*)&ueCb->ueCtxt.authVec, (U8*)&hssAuthVec.vec,\
         sizeof(ueCb->ueCtxt.authVec));

   ueCb->ueCtxt.nasKsi.pres = ueInfo->ueSecInfo.nasKsi.pres;
   ueCb->ueCtxt.nasKsi.id = ueInfo->ueSecInfo.nasKsi.id;
   ueCb->ueCtxt.nasKsi.tsc = ueInfo->ueSecInfo.nasKsi.tsc;

   /* Update sequence number for future use */
   cmMemcpy(ueInfo->ueSecInfo.sqnHE, ueCb->ueCtxt.authInfo.seqNmb, 
            VB_HSS_SEQ_NMB_SIZE);

   ueCb->ueCtxt.resync = FALSE;

   RETVALUE(ret);
}


/*
*
*       Fun:   vbMmeProcessSyncFlr
*       
*       Desc:  Inteface for auth sync failure function
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_sec_intf.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeProcessSyncFlr
(
VbMmeUeCb  *ueCb,
U8         *auts,
U8         *rnd
)
#else
PUBLIC S16 vbMmeProcessSyncFlr(ueCb, auts, rand)
(
VbMmeUeCb  *ueCb;
U8         *auts;
U8         *rnd;
)
#endif
{
   S16             ret          = RFAILED;
   VbHssAuthVec    hssAuthVec;
   VbHssUeInfo     *ueInfo      = NULLP;

   VB_MME_TRC2(vbMmeProcessSyncFlr)

   ret = vbHssFindImsi(ueCb->ueCtxt.ueImsi, &ueInfo);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Ue Info Not Found"));
      RETVALUE(RFAILED);
   }

   ret = vbHssPrcsSyncFlr(&ueCb->ueCtxt.authInfo, auts, rnd, 
                    &hssAuthVec, ueCb->ueCtxt.srvPlmn);
   ueCb->ueCtxt.resync = TRUE;
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Resync Failure"));
      RETVALUE(RFAILED);
   } 

   cmMemcpy((U8*)&ueCb->ueCtxt.authVec, (U8*)&hssAuthVec.vec, 
             sizeof(ueCb->ueCtxt.authVec));

   /* Update sequence number for future use */
   cmMemcpy(ueInfo->ueSecInfo.sqnHE, ueCb->ueCtxt.authInfo.seqNmb, 
            VB_HSS_SEQ_NMB_SIZE);
   
   RETVALUE(ret); 
}



#endif


#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
