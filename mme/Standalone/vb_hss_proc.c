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

     Desc:     C source code for HSS procedures.

     File:     vb_hss_proc.c
  
     Sid:      vb_hss_proc.c@@/main/1 - Mon Jun 28 19:35:37 2010
 
     Prg:      ad

*********************************************************************21*/

/* Header include files (.h) */
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
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "vb_hss.h"

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

/* vb007.101 :S1AP include files order has changed. */
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

#ifdef __cplusplus
EXTERN "C" 
{
#endif

EXTERN VbGUMMEI *selfGummei;


/**

   @brief Handles Update Location Request(ULR) Command.
          This is invoked by MME. If the entry is found for the 
          received IMSI in HSS DB, it returns Success and copies the 
          Ue Info.

   @param[in] ulrCmd UL-Request Command Structure
   @param[out] ueInfo Ue Information as stored in HSS.

   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PUBLIC S16 vbMmeHssULReqCmd 
(
VbMmeHssULRCmd  *ulrCmd,
VbHssUeInfo     *ueInfo
)
#else
PUBLIC S16 vbMmeHssULReqCmd(ulrCmd, ueInfo)
VbMmeHssULRCmd  *ulrCmd;
VbHssUeInfo     *ueInfo;
#endif
{
   S16 ret = RFAILED;
   U8 count = 0;

   VB_TRC2(vbMmeHssULReqCmd)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Handling the UL Request Command"));

   /* MME Reference Application Code*/
   for (count = 0; count < vbHssUeDB.numUEEntries; count++)
   {
      ret = cmMemcmp((U8 *)ulrCmd->imsi.val,\
            (CONSTANT U8*)&vbHssUeDB.ueInfo[count].imsi, VB_HSS_IMSI_LEN);
      if (ret == ROK)
      {
         /* Copy the UE Data */
         cmMemcpy((U8 *)ueInfo, (U8 *)&vbHssUeDB.ueInfo[count],\
               sizeof(VbHssUeInfo));

         vbHssUeDB.ueInfo[count].ueStatus = TRUE;
         RETVALUE(ret);
      }
   }/* for loop */

   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "No entry found for this IMSI in DB"));

   RETVALUE(ret);
} /* vbMmeHssULReqCmd */

/* vb005.101: Added support for multiple Attach and Detach */
/**

   @brief Handles to reset UE in detach
          This is invoked by MME. If the entry is found for the 
          received IMSI in HSS DB, it reset ue Status
          Ue Info.


   @return 
      - ROK on Success
      - RFAILED on Failure

 */
#ifdef ANSI
PUBLIC S16 vbMmeHssReset
(
VbMmeHssULRCmd  *ulrCmd
)
#else
PUBLIC S16 vbMmeHssReset(ulrCmd)
VbMmeHssULRCmd  *ulrCmd;
#endif
{
   S16      ret = RFAILED;
   U8       count = 0;

   VB_TRC2(vbMmeHssReset)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Handling HSS reset in UE Detach"));

   for (count = 0; count < vbHssUeDB.numUEEntries; count++)
   {
      ret = cmMemcmp((CONSTANT U8*)ulrCmd->imsi.val,\
            (CONSTANT U8*)&vbHssUeDB.ueInfo[count].imsi.val,\
            VB_HSS_IMSI_LEN);
      if (ret == ROK)
      {
			 vbHssUeDB.ueInfo[count].ueStatus = FALSE;
			 break;
      }
   }

   RETVALUE(ret);
} /* vbMmeHssReset */

#ifdef __cplusplus
}
#endif

/********************************************************************30**
 
         End of file:     vb_hss_proc.c@@/main/1 - Mon Jun 28 19:35:37 2010
 
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
/main/1      ---     ad        1. Initial version for LTE-CNE Phase 1.
/main/1    vb005.101  rk   1. Added support for multiple Attach and Detach
/main/1    vb007.101 akaranth   1. S1AP include files order has changed.
*********************************************************************91*/
