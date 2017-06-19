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

     Type:    C Include file

     Desc:    This file contains the vb application nas utility functions

     File:    vb_mme_nas_utl.c

     Sid:     vb_mme_nas_utl.c@@/main/1 - Mon Jun 28 19:35:38 2010

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

#include "egt.h"           /* EG Upper Interface              */
#include "leg.h"           /* EG LM Interface                 */

#include "szt.h"           /* S1AP Upper Interface            */
#include "szt_asn.h"       /* S1AP ASN                        */

#include "lvb.h"           /* CNE Layer management            */
#include "vb_hss.h"        /* CNE Application defines         */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines         */
#include "cm_emm.h"        /* CNE Application defines         */
#include "cm_esm.h"        /* CNE Application defines         */
#include "cm_emm_esm.h"    /* EMM/ESM EDM                     */

#include <stdlib.h>        /* For atoi()                      */
#ifdef VB_MME_AUTH
#include "vb_hss_autn.h"
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#ifdef VB_PERF_MEAS
#include <sys/time.h>
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

#include "egt.x"           /* EG Upper Interface              */
#include "leg.x"           /* EG LM Interface                 */

#include "szt_asn.x"       /* S1AP ASN                        */
#include "szt.x"           /* S1AP Upper Interface            */

#include "lvb.x"           /* CNE Layer management            */
#include "cm_esm.x"        /* CNE Application structures      */
#include "cm_emm.x"        /* CNE Application structures      */
#include "cm_emm_esm.x"    /* CNE Application structures      */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures      */
#include "vb.x"            /* CNE Application structures      */
#include "vb_s10_utl.x"
#include "vb_hss.x"        /* CNE Application structures      */
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */
#ifdef VB_MME_NAS_SEC
#include "vb_sec_intf.x"
#endif
#include "vb_s10_hndlr.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */


/***************************************************************/
/*                                                             */
/*           All NAS out going message functions               */
/*                                                             */
/***************************************************************/

/*
*
*       Fun:   vbMmeSndAttachReject
*       
*       Desc:  This function sends Attach Reject message to UE using EMM through 
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndAttachReject
(
VbMmeUeCb    *ueCb,
U16           emmCause,
VbMmeEsmCb   *esmCb 
)
#else
PUBLIC S16 vbMmeSndAttachReject(ueCb, emmCause, esmCb)
VbMmeUeCb    *ueCb;
U16           emmCause;
VbMmeEsmCb   *esmCb 
#endif
{
   CmNasEvnt  *attachRejEvnt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16         ret;

   VB_TRC2(vbMmeSndAttachReject)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Attach Reject ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Attach Reject ueId(%ld)",ueCb->ueCbKey));
#endif            

   ret = vbMmeUtlBldAttachReject(ueCb, &attachRejEvnt, emmCause, esmCb);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Attach Reject ueCb(%d)"
               ,ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Attach Reject ueCb(%ld)"
               ,ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   ret = vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, attachRejEvnt);

   /*Return value of vbMmeEmmHdlOutUeEvnt is ignored here */
   if(CM_EMM_ILLEGAL_UE == emmCause) 
   {
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
            SztCauseNasauthentication_failureEnum, 0);
   }
#if 0
   else if (CM_EMM_CONGESTION == emmCause)
   {
      /* The redirection cause is sent to eNB so that, the UE can
       * be redirected to PS network 
       */
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_RADIONW,\
            SztCauseRadioNwinterrat_redirectionEnum, 0);
   }
#endif
   else
   {
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
            SztCauseNasunspecifiedEnum, 0);
   }

   RETVALUE(ret);
}


/*
*
*       Fun:   vbMmeSndAttachAccept
*       
*       Desc:  This function sends Attach Accept message to UE using EMM through
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndAttachAccept
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndAttachAccept(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt   *attachAccEvnt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16         ret;

   VB_TRC2(vbMmeSndAttachAccept)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Attach Accept ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Attach Accept ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldAttachAccept(ueCb, &attachAccEvnt);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Attach Accept UE(%d)",
             ueCb->ueCbKey)); 
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Attach Accept UE(%ld)",
             ueCb->ueCbKey)); 
#endif

      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, attachAccEvnt));

}
/*
*
*       Fun:   vbMmeSndServiceRej
*       
*       Desc:  This function sends Service Reject to UE
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndServiceRej
(
VbMmeUeCb    *ueCb,
U8            cause
)
#else
PUBLIC S16 vbMmeSndServiceRej(ueCb, cause)
VbMmeUeCb    *ueCb;
U8            cause;
#endif
{
   CmNasEvnt *serviceRejEvnt = NULLP; 
   VbMmeUeCtxt  ueCtxt;
   S16  ret = ROK;

   VB_TRC2(vbMmeSndServiceRej)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Service Reject ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Service Reject ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldServiceReject(ueCb, &serviceRejEvnt, cause);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Build Service Reject ueCb(%d)"
               ,ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Build Service Reject ueCb(%ld)"
               ,ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   ret = vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, serviceRejEvnt);

   /* Service Request Failed Release S11 connection */
   if((NULLP != ueCb->ueTunnelCb) &&
         (0 != ueCb->ueTunnelCb->remoteTeId))
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Sending Delete Session Request To S-GW"));
      ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);
      if(ROK == ret)
      {
         /* Delete session sent successfully,
          *                so we are expecting response from SGW */
         RETVALUE(ROK);
      }
   }

   /* If nothing to release on S11 interface, release 
    * the S1-MME connection 
    */
   ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, 
              SztCauseNasauthentication_failureEnum, 0);

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeSndGutiReallocCmd
*       
*       Desc:  This function sends GUTI Reallocation Cmd to UE
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndGutiReallocCmd
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndGutiRealloccmd(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt    *gutiReallocCmdEvnt = NULLP; 
   VbMmeUeCtxt  ueCtxt;
   S16          ret                 = ROK;

   VB_TRC2(vbMmeSndGutiRealloccmd)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending GUTI Reallocation Command To UeId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending GUTI Reallocation Command To UeId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldGutiReallocCmd(ueCb, &gutiReallocCmdEvnt);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Build GUTI Reallocation \
               Cmd For UeCb(%d)", ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Build GUTI Reallocation \
               Cmd For UeCb(%ld)", ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   ret = vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, gutiReallocCmdEvnt);

   RETVALUE(ret);
}
/*
*
*       Fun:   vbMmeSndResAllocRej
*       
*       Desc:  This function sends Res Alloc Rej to UE
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndResAllocRej
(
VbMmeEsmCb    *esmCb,
U8            esmCause
)
#else
PUBLIC S16 vbMmeSndResAllocRej(esmCb, esmCause)
VbMmeEsmCb    *esmCb;
U8            esmCause;
#endif
{
   CmNasEvnt *resAllocRejEvnt = NULLP; 
   S16  ret = ROK;

   VB_TRC2(vbMmeSndResAllocRej)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Bearer Res Alloc Reject ueId(%d)",esmCb->ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Bearer Res Alloc Reject ueId(%ld)",esmCb->ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldResAllocReject(esmCb, &resAllocRejEvnt, esmCause);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Bearer Res Alloc Reject ueCb(%d)", esmCb->ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Bearer Res Alloc Reject ueCb(%ld)", esmCb->ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ret = vbEsmHdlOutUeEvnt(esmCb, resAllocRejEvnt);

   RETVALUE(ret);
}
/*
*
*       Fun:   vbMmeSndResModRej
*       
*       Desc:  This function sends Res Modification Rej to UE
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndResModRej
(
VbMmeEsmCb    *esmCb,
U8            esmCause
)
#else
PUBLIC S16 vbMmeSndResModRej(esmCb, esmCause)
VbMmeEsmCb    *esmCb;
U8            esmCause;
#endif
{
   CmNasEvnt *resModRejEvnt = NULLP; 
   S16  ret = ROK;

   VB_TRC2(vbMmeSndResModRej)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Bearer Res Mod Reject ueId(%d)",esmCb->ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Bearer Res Mod Reject ueId(%ld)",esmCb->ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldResModReject(esmCb, &resModRejEvnt, esmCause);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Bearer Res Mod Reject ueCb(%d)", esmCb->ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Bearer Res Mod Reject ueCb(%ld)", esmCb->ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ret = vbEsmHdlOutUeEvnt(esmCb, resModRejEvnt);

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeSndPdnConnRej
*       
*       Desc:  This function sends PDN Conn Reject to UE
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndPdnConnRej
(
VbMmeEsmCb    *esmCb,
U8            esmCause
)
#else
PUBLIC S16 vbMmeSndPdnConnRej(esmCb, esmCause)
VbMmeEsmCb    *esmCb;
U8            esmCause;
#endif
{
   S16       ret             = ROK;
   CmNasEvnt *pdnConnRejEvnt = NULLP; 

   VB_TRC2(vbMmeSndPdnConnRej)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending PDN Conn Reject To ueId(%d)",esmCb->ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending PDN Conn Reject To ueId(%ld)",esmCb->ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldPdnConnReject(esmCb, &pdnConnRejEvnt, esmCause);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Pdn Conn Reject For ueCb(%d)", esmCb->ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Pdn Conn Reject For ueCb(%ld)", esmCb->ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ret = vbEsmHdlOutUeEvnt(esmCb, pdnConnRejEvnt);

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeSndPdnDisConnRej
*       
*       Desc:  This function sends PDN DisConn Reject to UE
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndPdnDisConnRej
(
VbMmeEsmCb    *esmCb,
U8            esmCause
)
#else
PUBLIC S16 vbMmeSndPdnDisConnRej(esmCb, esmCause)
VbMmeEsmCb    *esmCb;
U8            esmCause;
#endif
{
   S16       ret             = ROK;
   CmNasEvnt *pdnDisConnRejEvnt = NULLP; 

   VB_TRC2(vbMmeSndPdnDisConnRej)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending PDN DisConn Reject To ueId(%d)",esmCb->ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending PDN DisConn Reject To ueId(%ld)",esmCb->ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldPdnDisConnReject(esmCb, &pdnDisConnRejEvnt, esmCause);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Pdn Conn Reject For ueCb(%d)", esmCb->ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Could Not Build Pdn Conn Reject For ueCb(%ld)", esmCb->ueCb->ueCbKey));
#endif
      /* Reject couldnt be built trace here */
      RETVALUE(ret);
   }

   ret = vbEsmHdlOutUeEvnt(esmCb, pdnDisConnRejEvnt);

   RETVALUE(ret);
}
/*
*
*       Fun:   vbMmeSndActDedBearerReq
*       
*       Desc:  This function sends Activate Dedicated Bearer Req
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndActDedBearerReq
(
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeSndActDedBearerReq(esmCb)
VbMmeEsmCb    *esmCb;
#endif
{
   CmNasEvnt *actDedBearer = NULLP;
   VbMmeUeCb    *ueCb = NULLP;
   S16 ret;
   VB_TRC2(vbMmeSndActDedBearerReq)

   ueCb = esmCb->ueCb;   

#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Act Ded Bearer Request For ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Act Ded Bearer Request For ueId(%ld)",ueCb->ueCbKey));
#endif
   ret = vbMmeUtlBldActvDedBearerReq(esmCb, &actDedBearer);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Building Act Ded Bearer Req Failed"));; 
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ret = vbEsmHdlOutUeEvnt (esmCb, actDedBearer);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The Act Ded Bearer Req(%d)",
             ueCb->ueCbKey)); 
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The Act Ded Bearer Req(%ld)",
             ueCb->ueCbKey)); 
#endif
   } 
   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeSndActDefBearerReq
*       
*       Desc:  This function sends Activate Default Bearer Req
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndActDefBearerReq
(
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeSndActDefBearerReq(esmCb)
VbMmeEsmCb    *esmCb;
#endif
{
   CmNasEvnt *actDefBearer = NULLP;
   VbMmeUeCb    *ueCb = NULLP;
   S16 ret;
   VB_TRC2(vbMmeSndActDefBearerReq)
   ueCb = esmCb->ueCb;   

#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Act Def Bearer Request For ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Act Def Bearer Request For ueId(%ld)",ueCb->ueCbKey));
#endif
   ret = vbMmeUtlBldDfltActvBearerReq(esmCb, &actDefBearer);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Building Act Def Bearer Req Failed"));; 
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ret = vbEsmHdlOutUeEvnt (esmCb, actDefBearer);
   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeSndDeActDedBearerReq
*       
*       Desc:  This function sends Deactivated Dedicated Bearer Req
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndDeActDedBearerReq
(
VbMmeEsmCb    *esmCb,
CmEsmCause     cause
)
#else
PUBLIC S16 vbMmeSndDeActDedBearerReq(esmCb)
VbMmeEsmCb    *esmCb;
CmEsmCause     cause;
#endif
{
   CmNasEvnt *deActDedBearer = NULLP;
   VbMmeUeCb    *ueCb = NULLP;
   S16 ret;
   VB_TRC2(vbMmeSndDeActDedBearerReq)
   ueCb = esmCb->ueCb;   

#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending DeAct Ded Bearer Request For ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending DeAct Ded Bearer Request For ueId(%ld)",ueCb->ueCbKey));
#endif
   ret = vbMmeUtlBldDeActvDedBearerReq(esmCb, &deActDedBearer, cause);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Building DeAct Ded Bearer Req Failed"));; 
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ret = vbEsmHdlOutUeEvnt(esmCb, deActDedBearer);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The DeAct Ded Bearer Req(%d)",
             ueCb->ueCbKey)); 
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The DeAct Ded Bearer Req(%ld)",
             ueCb->ueCbKey)); 
#endif
   } 
   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeSndDeActDefBearerReq
*       
*       Desc:  This function sends Deactivated Default Bearer Req
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndDeActDefBearerReq
(
VbMmeEsmCb    *esmCb,
CmEsmCause     cause
)
#else
PUBLIC S16 vbMmeSndDeActDefBearerReq(esmCb)
VbMmeEsmCb    *esmCb;
CmEsmCause     cause;
#endif
{
   CmNasEvnt    *deActDefBearer = NULLP;
   VbMmeUeCb    *ueCb           = NULLP;
   S16          ret;

   VB_TRC2(vbMmeSndDeActDefBearerReq)

   ueCb = esmCb->ueCb;   

#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending DeAct Def Bearer Request For ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending DeAct Def Bearer Request For ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldDeActvDefBearerReq(esmCb, &deActDefBearer, cause);

   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Building DeAct Def Bearer Req Failed"));; 
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ret = vbEsmHdlOutUeEvnt(esmCb, deActDefBearer);
   if (ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The DeAct Def Bearer Req(%d)",
             ueCb->ueCbKey)); 
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The DeAct Def Bearer Req(%ld)",
             ueCb->ueCbKey)); 
#endif
   } 

   RETVALUE(ret);
}
/*
*
*       Fun:   vbMmeSndModBearerCtxtReq
*       
*       Desc:  This function sends Modify Bearer Ctxt Req
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndModBearerCtxtReq
(
VbMmeEsmCb    *esmCb,
CmEsmCause     cause
)
#else
PUBLIC S16 vbMmeSndModBearerCtxtReq(esmCb)
VbMmeEsmCb    *esmCb;
CmEsmCause     cause;
#endif
{
   CmNasEvnt *modDedBearer = NULLP;
   VbMmeUeCb    *ueCb = NULLP;
   S16 ret;
   VB_TRC2(vbMmeSndModBearerCtxtReq)
   ueCb = esmCb->ueCb;   

#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Mod Bearer Ctxt Request For ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Mod Bearer Ctxt Request For ueId(%ld)",ueCb->ueCbKey));
#endif
   ret = vbMmeUtlBldModBearerCtxtReq(esmCb, &modDedBearer, cause);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Building Mod Bearer Ctxt Req Failed"));; 
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ret = vbEsmHdlOutUeEvnt(esmCb, modDedBearer);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The Mod Bearer Ctxt Req(%d)",
             ueCb->ueCbKey)); 
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send The Mod Bearer Ctxt Req(%ld)",
             ueCb->ueCbKey)); 
#endif
   } 
   RETVALUE(ret);
}

#ifdef VB_MME_AUTH
/*
*
*       Fun:   vbMmeSndAuthRequest
*       
*       Desc:  This function sends Authentication Request message to UE using
*              EMM through eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeSndAuthRequest
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndAuthRequest(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt *authReqEvnt = NULLP;
   S16 ret;
   VbMmeUeCtxt ueCtxt;

   VB_TRC2(vbMmeSndAuthRequest)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Auth Req ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Auth Req ueId(%ld)",ueCb->ueCbKey));
#endif
   ret = vbMmeUtlBldAuthRequest(&ueCb->emmCb, &authReqEvnt);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Building Auth Req Failed")); 

      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, authReqEvnt));

}


/*
*
*       Fun:   vbMmeSndAuthReject
*       
*       Desc:  This function sends Authentication Reject message to UE using
*              EMM through eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndAuthReject
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndAuthReject(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt *authRejEvnt = NULLP;
   S16 ret;
   VbMmeUeCtxt ueCtxt;
   VB_TRC2(vbMmeSndAuthReject)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Sending Auth Reject ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Sending Auth Reject ueId(%ld)",ueCb->ueCbKey));
#endif
   ret = vbMmeUtlBldAuthReject(&ueCb->emmCb, &authRejEvnt);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Building Auth Rej Failed")); 

      /* Reject couldnt be built trace here */
      RETVALUE(ret); 
   } 
   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, authRejEvnt));
}
#endif /* VB_MME_AUTH */

#ifdef VB_MME_NAS_SEC
/*
*
*       Fun:   vbMmeSndSecModCmd
*       
*       Desc:  This function sends Security Mode Command message to UE using
*              EMM through eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeSndSecModCmd
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndSecModCmd(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt *secModeCmdEvnt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16 ret;

   VB_TRC2(vbMmeSndSecModCmd)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Sec Mode ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Sec Mode ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldSecModCmd(&(ueCb->emmCb), &secModeCmdEvnt);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Building Sec Mode Req Failed"));
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 
   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, secModeCmdEvnt));
} /* vbMmeSndSecModCmd */
#endif /* VB_MME_NAS_SEC */


/*
*
*       Fun:   vbMmeSndIdentityRequest
*       
*       Desc:  This function sends Identity Request message to UE using
*              EMM through eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndIdentityRequest
(
VbMmeUeCb    *ueCb,
U8           type
)
#else
PUBLIC S16 vbMmeSndIdentityRequest(ueCb, type)
VbMmeUeCb    *ueCb;
U8           type;
#endif
{
    CmNasEvnt   *idReqEvnt = NULLP;
    VbMmeUeCtxt ueCtxt;
    S16         ret;

    VB_TRC2(vbMmeSndIdentityRequest)

#ifdef ALIGN_64BIT
    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
             "Sending Identity Request ueId(%d)",ueCb->ueCbKey));
#else
    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
             "Sending Identity Request ueId(%ld)",ueCb->ueCbKey));
#endif
    ret = vbMmeUtlBldIdentityReq(&(ueCb->emmCb), &idReqEvnt, type);

    if(ROK != ret)
    {
#ifdef ALIGN_64BIT    
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Buidling Identity Request(%d)",
                  ueCb->ueCbKey));
#else
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Buidling Identity Request(%ld)",
                  ueCb->ueCbKey));
#endif
        RETVALUE(ret); 
    }  
    ueCtxt.suConnId = ueCb->suConnId;
    ueCtxt.spConnId = ueCb->spConnId;

    RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, idReqEvnt));
} /* vbMmeSndIdentityRequest */


/*
 *
 *       Fun:   vbMmeSndDetachReq
 *       
 *       Desc:  This function sends Detach Reqest message to UE using EMM through 
 *       
 *       Ret:   ROK
 *       
 *       Notes: None
 *              
 *       File:  vb_emm.c
 *       
 */

#ifdef ANSI
PUBLIC S16 vbMmeSndDetachReq
(
 VbMmeUeCb    *ueCb,
 U8           detachType,
 U16          emmCause
 )
#else
PUBLIC S16 vbMmeSndDetachReq(ueCb, detachType, emmCause)
   VbMmeUeCb    *ueCb;
   U8           detachType;
   U16          emmCause;
#endif
{
   CmNasEvnt   *detachReqEvt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16         ret = ROK;

   VB_TRC2(vbMmeSndDetachReq);

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Detach Request ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Detach Request ueId(%ld)",ueCb->ueCbKey));
#endif
   /* Network detach timer expired , initiate Detach procedure */
   /* Build detach request */
   ret = vbMmeUtlBldDetachReq(&ueCb->emmCb, &detachReqEvt, detachType,\
         emmCause);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Detach Request Failed"));
      RETVALUE(ret);
   }

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;
   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, detachReqEvt));
} /* vbMmeSndDetachReq */



/*
 *
 *       Fun:   vbMmeSndDetachAcc
 *       
 *       Desc:  This function sends Detach Accept message to UE using EMM 
 *              through. This funtion is invoked when UE sends a detach 
 *              request without SwitchOff indication
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndDetachAcc
(
 VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndDetachAcc(ueCb)
   VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt         *detachAccEvt = NULLP;
   VbMmeUeCtxt       ueCtxt;
   S16               ret = ROK;

   VB_TRC2(VbMmeSndDetachAccept)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Detach Accept ueId(%d)",
            ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Detach Accept ueId(%ld)",
            ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldDetachAcc(&ueCb->emmCb, &detachAccEvt);
   
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Could Build Create Detach Request Failed"));
      RETVALUE(ret);
   }

   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, detachAccEvt));

} /* end of vbMmeSndDetachAcc */


/*
*
*       Fun:   vbMmeSndTAUAcc
*       
*       Desc:  This function sends TAU Accept message to UE using EMM through
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndTAUAcc
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndTAUAcc(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   CmNasEvnt   *tauAccEvnt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16         ret;

   VB_TRC2(vbMmeSndTAUAcc)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending TAU Accept ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending TAU Accept ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldTAUAcc(&ueCb->emmCb, &tauAccEvnt);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send TAU Accept UE(%d)",
             ueCb->ueCbKey)); 
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send TAU Accept UE(%ld)",
             ueCb->ueCbKey)); 
#endif

      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 
   
   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, tauAccEvnt));
}


/*
*
*       Fun:   vbMmeSndTAURej
*       
*       Desc:  This function sends TAU Reject message to UE using EMM through
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndTAURej
(
VbMmeUeCb    *ueCb,
U8           cause
)
#else
PUBLIC S16 vbMmeSndTAURej(ueCb)
VbMmeUeCb    *ueCb;
U8           cause;
#endif
{
   CmNasEvnt   *tauRejEvnt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16         ret;

   VB_TRC2(vbMmeSndTAURej)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending TAU Reject ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending TAU Reject ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldTAURej(&ueCb->emmCb, &tauRejEvnt, cause);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send TAU Reject UE(%d)",
             ueCb->ueCbKey)); 
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send TAU Reject UE(%ld)",
             ueCb->ueCbKey)); 
#endif

      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 
   
   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, tauRejEvnt));
}


/*
*
*       Fun:   vbMmeSndEmmStatus
*       
*       Desc:  This function sends Emm Status message to UE using EMM through
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndEmmStatus
(
VbMmeUeCb    *ueCb,
U8           cause
)
#else
PUBLIC S16 vbMmeSndEmmStatus(ueCb)
VbMmeUeCb    *ueCb;
U8           cause;
#endif
{
   CmNasEvnt   *emmStatusEvnt = NULLP;
   VbMmeUeCtxt ueCtxt;
   S16         ret;

   VB_TRC2(vbMmeSndEmmStatus)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Emm Status ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Emm Status ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldEmmStatus(&ueCb->emmCb, &emmStatusEvnt, cause);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Emm Status UE(%d)",
             ueCb->ueCbKey)); 
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Emm Status UE(%ld)",
             ueCb->ueCbKey)); 
#endif

      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 
   
   ueCtxt.suConnId = ueCb->suConnId;
   ueCtxt.spConnId = ueCb->spConnId;

   RETVALUE(vbMmeEmmHdlOutUeEvnt(ueCb->eNodebCb, ueCb, &ueCtxt, emmStatusEvnt));
}

/*
*
*       Fun:   vbMmeSndEsmStatus
*       
*       Desc:  This function sends Esm Status message to UE using EMM through
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndEsmStatus
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
U8           pti,
U8           brId,
U8           cause
)
#else
PUBLIC S16 vbMmeSndEsmStatus(ueCb, esmCb, pti, brId, cause)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
U8           pti;
U8           brId;
U8           cause;
#endif
{
   CmNasEvnt   *esmStatusEvnt = NULLP;
   S16         ret  = ROK;

   VB_TRC2(vbMmeSndEsmStatus)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Esm Status ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Esm Status ueId(%ld)",ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldEsmStatus(ueCb, &esmStatusEvnt, pti, brId, cause);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Esm Status UE(%d)",
             ueCb->ueCbKey)); 
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Esm Status UE(%ld)",
             ueCb->ueCbKey)); 
#endif
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 
   
   RETVALUE(vbEsmHdlOutUeEvnt(esmCb, esmStatusEvnt));
}

/*
*
*       Fun:   vbMmeSndEsmInfoReq
*       
*       Desc:  This function sends Esm Info Request message to UE using EMM through
*              eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_nas_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndEsmInfoReq
(
VbMmeEsmCb   *esmCb
)
#else
PUBLIC S16 vbMmeSndEsmInfoReq(esmCb)
VbMmeEsmCb   *esmCb;
#endif
{
   CmNasEvnt   *esmInfoReqEvnt = NULLP;
   S16         ret;

   VB_TRC2(vbMmeSndEsmInfoReq)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Esm Info Req ueId(%d)", esmCb->ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending Esm Info Req ueId(%ld)", esmCb->ueCb->ueCbKey));
#endif

   ret = vbMmeUtlBldEsmInfoReq(esmCb, &esmInfoReqEvnt);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Esm Info Req UE(%d)",
             esmCb->ueCb->ueCbKey)); 
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Esm Info Req UE(%ld)",
             esmCb->ueCb->ueCbKey)); 
#endif
      /* Accept couldnt be built trace here */
      RETVALUE(ret); 
   } 
   
   RETVALUE(vbEsmHdlOutUeEvnt(esmCb, esmInfoReqEvnt));
}

/***************************************************************/
/*                                                             */
/*                 All NAS incoming message functions          */
/*                                                             */
/***************************************************************/


/*
 *
 *       Fun:   vbMmeHndlAttachReq 
 *       
 *       Desc:  This function handles incoming Attach Reqest message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */

#ifdef ANSI
PUBLIC S16 vbMmeHndlAttachReq
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlAttachReq(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   VbMmeHssULRCmd ulrCmd;
   S16            ret     = ROK;
   VbMmeEsmCb     *esmCb  = NULLP;

   VB_TRC2(vbMmeHndlAttachReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Attach Request From UE"));

   /* Stop All Timer If Running */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_UE_CTX_DEL);
   ueCb->ueCtxt.suspendPaging = FALSE;

   ret = vbUtlGetS1APTaId(&((S1apPdu*)evnt->pdu)->pdu, 
              ((S1apPdu*)evnt->pdu)->pdu.val.initiatingMsg.procedureCode.val, 
              &ueCb->ueCtxt.tai);
   if(ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TAI Retrieval Failed"));
      ret  = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
      RETVALUE(ret);
   }

   cmMemcpy(ueCb->ueCtxt.srvPlmn, ueCb->ueCtxt.tai.plmnId.plmnId,
            VB_PLMN_ID_LEN);
   vbMmeAddToUETaiLst(ueCb, &ueCb->ueCtxt.tai);

   ret = vbUtlGetS1APeCgi(&((S1apPdu*)evnt->pdu)->pdu, 
           ((S1apPdu*)evnt->pdu)->pdu.val.initiatingMsg.procedureCode.val, 
           &ueCb->ueCtxt.eCgi); 
   if(ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "ECGI Retrieval Failed"));
      ret  = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
      RETVALUE(ret);
   }

   /* Copy the UE capabilities received in Attach Request */
   cmMemcpy((U8 *)&(ueCb->ueCtxt.ueSecCap), 
            (U8 *)&evnt->m.emmEvnt->u.atchReq.ueNwCap,
            sizeof(CmEmmUeNwCap));
   if (evnt->m.emmEvnt->u.atchReq.msNwCap.pres)
   {
      ueCb->ueCtxt.ueSecCap.gea7 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea7;
      ueCb->ueCtxt.ueSecCap.gea6 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea6;
      ueCb->ueCtxt.ueSecCap.gea5 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea5;
      ueCb->ueCtxt.ueSecCap.gea4 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea4;
      ueCb->ueCtxt.ueSecCap.gea3 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea3;
      ueCb->ueCtxt.ueSecCap.gea2 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea2;
      ueCb->ueCtxt.ueSecCap.gea1 = evnt->m.emmEvnt->u.atchReq.msNwCap.gea1;
      /* if the vcc_1xcsr bit was present we would have anyways copied an extra byte 
      when copying from ueNwCap to ueSecCap - so then no need to increment the capLenn again .
      If the vxx_1xsr was not present then yes we need to increment length by 1 */
      if ( ! evnt->m.emmEvnt->u.atchReq.ueNwCap.vcc_1xsr ) 
      {
      ueCb->ueCtxt.ueSecCap.len++;
   }
   }

   /* Check the attach type */
   if (TRUE == evnt->m.emmEvnt->u.atchReq.epsAtchType.pres) 
   {
      if (CM_EMM_COMBINED_ATTACH == evnt->m.emmEvnt->u.atchReq.epsAtchType.type)
      {
         VB_MME_SET_MASK(ueCb->bitmask, VB_EMM_COMBINED_ATTACH);
      }
      else
      {
         VB_MME_UNSET_MASK(ueCb->bitmask, VB_EMM_COMBINED_ATTACH);
      }
   }
   else
   {
      VB_MME_UNSET_MASK(ueCb->bitmask, VB_EMM_COMBINED_ATTACH);
   }

   /* if IMSI received in Attach Request */
   if (CM_EMM_MID_TYPE_IMSI == evnt->m.emmEvnt->u.atchReq.epsMi.type)
   {   
      ueCb->ueCtxt.ueImsiLen = evnt->m.emmEvnt->u.atchReq.epsMi.len; 
      cmMemcpy(ueCb->ueCtxt.ueImsi, evnt->m.emmEvnt->u.atchReq.epsMi.u.
               imsi.id, evnt->m.emmEvnt->u.atchReq.epsMi.len);
       
      if (0 != ueCb->ueCtxt.ueImsiLen)
      {
         vbMmeUtlFndNDelUeCb(ueCb->ueCtxt.ueImsi, ueCb->ueCtxt.ueImsiLen);

         ret = vbMmeUtlAddUeCbOnImsi(ueCb->ueCtxt.ueImsi,\
               ueCb->ueCtxt.ueImsiLen, ueCb);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding UE Cb into IMSI based"\
                  "Hash Table"));

            ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
            RETVALUE(ret);
         }
      }
   }
#ifdef VB_MME_AUTH 
   if ( (evnt->m.emmEvnt->u.atchReq.nasKsi.id == CM_EMM_NONAS_KEY_AVAILABLE) || 
        ( evnt->m.emmEvnt->u.atchReq.nasKsi.id !=  ueCb->ueCtxt.secCtxt.ksi ) )
   {
      vbHssResetSecCtxt(ueCb->ueCtxt.ueImsi);
      ueCb->ueCtxt.secCtxt.lstUsdCnt = 0;
      ueCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb = 0;
      ueCb->ueCtxt.secCtxt.dlSeqNmb.ovrflwCnt = 0;
      ueCb->ueCtxt.secCtxt.ulSeqNmb.seqNmb = 0;
      ueCb->ueCtxt.secCtxt.ulSeqNmb.ovrflwCnt = 0;
      /* generate a NAS KSI which is not 7 & not whatever the UE sent - ccpu00135303 */
      VB_MME_GET_NEW_KSI(evnt->m.emmEvnt->u.atchReq.nasKsi.id,\
                        ueCb->ueCtxt.secCtxt.ksi);
   }
#endif
   /* typeOfId will be used incase of Auth Failure cases*/ 
   ueCb->emmCb.typeOfId = evnt->m.emmEvnt->u.atchReq.epsMi.type;   
   if (0 == ueCb->ueCtxt.ueImsiLen) /* if not IMSI present */
   {
      ueCb->emmCb.cause = CM_EMM_UE_ID_NOT_DERIVED;
      ret = vbMmeSndIdentityRequest(ueCb, CM_EMM_MID_TYPE_IMSI);
      if(ROK != ret)
      {
         ret  = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
         RETVALUE(ret);
      }
      else
      {
         RETVALUE(ROK);
      }
   }

   /* Validate on HSS if this is a valid UE */
   /* This should trigger ULR-ULA internally with HSS */
   if(ueCb->ueCtxt.ueImsiLen > CM_EMM_MAX_MOBILE_ID_DIGS)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid IMSI Received With \
                       Attach Request For UE(%d)", ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid IMSI Received With " 
                         "Attach Request For UE(%ld)", ueCb->ueCbKey));
#endif
      /* Send the Attach Reject */
      ret  = vbMmeSndAttachReject(ueCb, CM_EMM_IMSI_UNKNOWN, 0);
      RETVALUE(ret);
   }

   cmMemcpy(ulrCmd.imsi.val, ueCb->ueCtxt.ueImsi, ueCb->ueCtxt.ueImsiLen);
   ulrCmd.imsi.len = ueCb->ueCtxt.ueImsiLen;
   ret = vbMmeHssULReqCmd(&ulrCmd, &ueCb->ueCtxt.ueHssCtxt);
   if(ROK != ret)
   {
      /* Cause should be filled here as the IMSI is unknown to HSS
         because it is synchronous func call
       */
      ret  = vbMmeSndAttachReject(ueCb, CM_EMM_IMSI_UNKNOWN, 0);
      RETVALUE(ret);
   }


   ret = vbMmeGetFrmTrnList(ueCb, &esmCb, ueCb->emmCb.piggyAction.tId);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Finding ESM CB"));
      RETVALUE(ret);
   }

#ifdef VB_MME_AUTH
   /*NAS Message Is Already Secure*/
   if (VB_MME_SEC_SUCCESS == evnt->secStatus)
   {
      /* Generate KeNb - not expecting failure here */
      vbMmeGenerateEnbKey(&ueCb->ueCtxt.secCtxt,\
            ueCb->ueCtxt.secCtxt.lstUsdCnt);

      /* Release if there is old S11 connection */
      if ((NULLP != ueCb->ueTunnelCb) && (0 != ueCb->ueTunnelCb->remoteTeId))
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                    "Sending Delete Session Request To S-GW"));
         ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);
         if(ROK == ret)
         {
            RETVALUE(ROK);
         }
      }

#ifdef VB_MME_SEC
      /* Get esm info from UE*/
      if((ROK == ret) && (TRUE == esmCb->infoTransFlg))
      {
         ret = vbMmeSndEsmInfoReq(esmCb);
         if(ROK == ret)
         {
            RETVALUE(ROK);
         }
      }
#endif

      if(ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Create Session Request "\
               "To S-GW"));
         ret = vbMmeSndCSReq(ueCb, esmCb);
      }

      if(ROK != ret)
      {
         ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, esmCb);
         RETVALUE(ret);
      }

      RETVALUE(ret);
   }
   else 
   {
      vbMmeUtlGetNasAlg(ueCb, &ueCb->ueCtxt.secCtxt.encAlg,\
            &ueCb->ueCtxt.secCtxt.intAlg, &ueCb->ueCtxt.authInfo.authAlg);
      /* Generate Authentication Vector */
      ret = vbMmeGetAuthVector(ueCb);
      if (ret != ROK)
      {
         ret  = vbMmeSndAttachReject(ueCb, CM_EMM_IMSI_UNKNOWN, esmCb);
         RETVALUE(ret);
      }

#ifdef ALIGN_64BIT    
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Authentication Request "\
               "To UE(%d)\n", ueCb->ueCbKey));
#else
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Authentication Request "\
               "To UE(%ld)\n", ueCb->ueCbKey));
#endif
      ret = vbMmeSndAuthRequest(ueCb);
      if(ROK != ret)
      {
         /* Send the Attach Reject */
         ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, esmCb);
         RETVALUE(ret);
      }
   }

#else /* VB_MME_AUTH */

   if((NULLP != ueCb->ueTunnelCb) && (0 != ueCb->ueTunnelCb->remoteTeId))
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                  "Sending Delete Session Request To S-GW"));
      ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);
      if(ROK == ret)
      {
         /* Delete session sent successfully,
         so we are expecting response from SGW */
         RETVALUE(ROK);
      }
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Create Session Request To S-GW"));
   if(ROK == ret)
   {
      ret = vbMmeSndCSReq(ueCb, esmCb);
   }

   if (ROK != ret)
   {
      /* Send the Attach Reject */
      ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
      RETVALUE(ret);
   }
#endif /* VB_MME_AUTH */

   RETVALUE(ret);
}


/*
 *
 *       Fun:   vbMmeHndlIdentityResp 
 *       
 *       Desc:  This function handles incoming Identity Response message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */

#ifdef ANSI
/* Generic handlers */
PUBLIC S16 vbMmeHndlIdentityResp
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlIdentityResp(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16               ret       = RFAILED;
   S16               indx      = 0;
   VbMmeUeCb         *tempUeCb = NULLP;
#ifndef VB_MME_AUTH
   VbMmeEsmCb        *esmCb  = NULLP;
#endif
   VbMmeHssULRCmd    ulrCmd;

   VB_TRC2(vbMmeHndlIdentityResp);

   if (CM_EMM_UE_ID_NOT_DERIVED == ueCb->emmCb.cause)
   {
      if(CM_EMM_MID_TYPE_IMSI == evnt->m.emmEvnt->u.idRsp.msId.type)
      {
         /* Copy the IMSI received in Identity Response */
         cmMemcpy(ueCb->ueCtxt.ueImsi, evnt->m.emmEvnt->u.idRsp.msId.u.
               imsi.id, evnt->m.emmEvnt->u.idRsp.msId.len);

         /* Copy the IMSI length received in Identity Response */
         ueCb->ueCtxt.ueImsiLen = evnt->m.emmEvnt->u.idRsp.msId.len;

         /* No neeed to perform identity procedure on 
            authentication failure due to MAC */
         ueCb->emmCb.typeOfId = CM_EMM_MID_TYPE_IMSI;

         /*
            Duplicate-UeCb corner case description:
            Scenario 1: UE trying to attach to epc, but it fails due to 
            enb/ue issue and 
            ueCb is not deleted from MME. UE is trying to reattach to EPC
            with unknown GUTI, 
            so '2nd ueCb' is  created for the same UE and it is being
            realized after 
            it grown upto identity procedure.
            Hence MME need to delete old ueCb by deleting context on S11
            interface.       
            Scenario 2: UE attached to EPC-In-A-Box:1 successfully.
            Attaches again to 
            EPC-In-A-Box:2 without detaching from EPC-In-A-Box:1 (may be 
            because of enb/ue issues). Comes back to EPC-In-A-Box:1 with GUTI 
            allocated by EPC-In-A-Box:2 So EPC-In-A-Box:1 will endup having
            `2nd ueCb`. 
            It is being realized only after Identity Response.
            Hence MME need to delete old ueCb by deleting context on S11
            interface.
         */
         if (ueCb->ueCtxt.ueImsiLen)
         {
            /* + Duplicate-UeCb corner case + */
            ret = vbMmeUtlFndUeCbOnImsi(ueCb->ueCtxt.ueImsi, 
                  ueCb->ueCtxt.ueImsiLen, &tempUeCb);
            if(ret == ROK)
            {
               /* Move required data from old ueCb to new ueCb to 
                  delete s11 context later on */
               if(NULLP != tempUeCb->ueTunnelCb)
               {
                  /* Move s11 tunnel */
                  ueCb->ueTunnelCb       = tempUeCb->ueTunnelCb;
                  tempUeCb->ueTunnelCb   = NULLP;

                  /* Chain with new ueCb */
                  ueCb->ueTunnelCb->ueCb = ueCb;

                  /* Move PDN Cb */
                  for(indx = 0; indx < VB_MME_MAX_PDN_CONN; indx++)
                  {
                     ueCb->pdnList[indx] = tempUeCb->pdnList[indx];
                     tempUeCb->pdnList[indx] = NULLP;
                  }

                  /* Move ESM Cb */
                  for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
                  {
                     ueCb->esmList[indx] = tempUeCb->esmList[indx];
                     tempUeCb->esmList[indx] = NULLP;
                  }

                  /* Copy ESM map */
                  cmMemcpy((U8*)&ueCb->creBid, (U8*)&tempUeCb->creBid,\
                        sizeof(VbBearerIdLst));
                  cmMemset((U8*)&tempUeCb->creBid, 0, sizeof(VbBearerIdLst));
               } /* if(NULLP != tempUeCb->ueTunnelCb) */

               /* Delete old ueCb */
               vbMmeUtlDelUeCb(tempUeCb);
            } /* if(ret == ROK) */

            /* Add new ueCb */
            ret = vbMmeUtlAddUeCbOnImsi(ueCb->ueCtxt.ueImsi, 
                  ueCb->ueCtxt.ueImsiLen, ueCb);
         } /* if (ueCb->ueCtxt.ueImsiLen) */
      }  /* if(CM_EMM_MID_TYPE_IMSI == evnt->m.emmEvnt->u.idRsp.msId.type)*/
   } /* if(CM_EMM_UE_ID_NOT_DERIVED == ueCb->emmCb.cause) */
   else if (CM_EMM_ILLEGAL_UE == ueCb->emmCb.cause)
   {
      if(ueCb->ueCtxt.ueImsiLen == evnt->m.emmEvnt->u.idRsp.msId.len)
      {
         if(!cmMemcmp(ueCb->ueCtxt.ueImsi, evnt->m.emmEvnt->u.idRsp.
                  msId.u.imsi.id, evnt->m.emmEvnt->u.idRsp.msId.len))
         {
            ret = ROK;
         }
      } 
   }
   else
   {
      /*Do not know what to do here */
      ret = ROK;
   }

   cmMemcpy(ulrCmd.imsi.val, ueCb->ueCtxt.ueImsi, ueCb->ueCtxt.ueImsiLen);
   ulrCmd.imsi.len = ueCb->ueCtxt.ueImsiLen;
   ret = vbMmeHssULReqCmd(&ulrCmd, &ueCb->ueCtxt.ueHssCtxt);
   if (ROK != ret)
   {
      /* Cause should be filled here as the IMSI is unknown to HSS
         because it is synchronous func call
         */
      ret  = vbMmeSndAttachReject(ueCb, CM_EMM_IMSI_UNKNOWN, 0);
      RETVALUE(ret);
   }


#ifdef VB_MME_AUTH
   vbMmeUtlGetNasAlg(ueCb, &ueCb->ueCtxt.secCtxt.encAlg,\
         &ueCb->ueCtxt.secCtxt.intAlg, &ueCb->ueCtxt.authInfo.authAlg);

   ret = vbMmeGetAuthVector(ueCb);
   if (ret != ROK)
   {
      ret  = vbMmeSndAttachReject(ueCb, CM_EMM_IMSI_UNKNOWN, 0);
      RETVALUE(ret);
   }

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Authentication Request" 
            "To UE(%d)\n", ueCb->ueCbKey));
#else                                 
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Authentication Request" 
            "To UE(%ld)\n", ueCb->ueCbKey));
#endif
   ret = vbMmeSndAuthRequest(ueCb);
   if(ROK != ret)
   {
      ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
      RETVALUE(ret);
   }
#else /* VB_MME_AUTH */

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
            "Sending Create Session Request To S-GW"));
   if ((NULLP != ueCb->ueTunnelCb) && (0 != ueCb->ueTunnelCb->remoteTeId))
   {
      ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);
   }

   /* Fetch piggy back esmCb */
   ret = vbMmeGetFrmTrnList(ueCb, &esmCb, ueCb->emmCb.piggyAction.tId);
   if(ROK == ret)
   {
      ret = vbMmeSndCSReq(ueCb, esmCb);
      if(ROK != ret)
      {
         /* Send the Attach Reject */
         ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
      }
   }
#endif /* VB_MME_AUTH */

   RETVALUE(ret);
}


#ifdef VB_MME_AUTH
/*
 *
 *       Fun:   vbMmeHndlAuthResp 
 *       
 *       Desc:  This function handles incoming Authentication Response message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlAuthResp
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlAuthResp(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16         ret = RFAILED;
#ifndef VB_MME_NAS_SEC
   VbMmeEsmCb  *esmCb = NULLP;
#endif
   
   VB_TRC2(vbMmeHndlAuthResp)

   ret = vbMmeVerifyAuthResp(ueCb, evnt);
   if(ret == ROK)
   {
#ifdef VB_MME_NAS_SEC
      /*Construct Security Context*/
      ueCb->ueCtxt.secCtxt.tsc    = VB_MME_SEC_CTXT_NATV;
      ueCb->ueCtxt.secCtxt.status = VB_MME_SEC_CTXT_CRNT;
      cmMemcpy(ueCb->ueCtxt.secCtxt.kasme, ueCb->ueCtxt.authVec.kasme, 
               VB_MME_SZ_KASME_KEY);

      /* DO NOT genrate keys if encryption and integrity algorithms
       * selected are NULL
       */
      if ((ueCb->ueCtxt.secCtxt.encAlg != VB_MME_EEA0) || \
          (ueCb->ueCtxt.secCtxt.intAlg != VB_MME_EIA0))
      {
         ret = vbMmeGenerateNasKey(&ueCb->ueCtxt.secCtxt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NAS Key Generation Failed"));
            RETVALUE(ret);
         }
      }

      ret = vbMmeSndSecModCmd(ueCb);
      if(ROK != ret)
      {
         ueCb->ueCtxt.secCtxt.status = VB_MME_SEC_CTXT_NON_CRNT;
      }
#else /* VB_MME_NAS_SEC */
      ret = vbMmeGetFrmTrnList(ueCb, &esmCb, ueCb->emmCb.piggyAction.tId);
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Create Session Request "\
               "to SGW"));
         ret = vbMmeSndCSReq(esmCb->ueCb, esmCb);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Create Session Request "\
                  "to SGW, FAILED"));
         }
      }
#endif
   }
   else
   {
      /* Identity received in Init NAS message is not IMSI */
      if(CM_EMM_MID_TYPE_IMSI != ueCb->emmCb.typeOfId)
      {
         ueCb->emmCb.cause = CM_EMM_ILLEGAL_UE;
         ret = vbMmeSndIdentityRequest(ueCb, CM_EMM_MID_TYPE_IMSI);
        
         if(ROK !=ret)
         {
            vbMmeSndAuthReject(ueCb); /* return failure back */
         }
      }
   }

   RETVALUE(ret);
}


/*
 *
 *       Fun:   vbMmeHndlAuthFail 
 *       
 *       Desc:  This function handles incoming Authentication Failure message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */

#ifdef ANSI
PUBLIC S16 vbMmeHndlAuthFail
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlAuthFail(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
    S16  ret = RFAILED;
    U8   rnd[ CM_EMM_MAX_RAND];

    U8 *AUTS = NULLP;

    VB_TRC2(vbMmeHndlAuthFail)

    /* Delete all authentication vectors and generate new ones from HSS
     * Using the AUTS parameter received in failure message
     * Send the Authentication request again with new authentication vector */
    ueCb->ueCtxt.ueHssCtxt.ueSecInfo.pres = NOTPRSNT;
    if((evnt->m.emmEvnt->u.authFlr.cause.pres == PRSNT_NODEF) &&
          (evnt->m.emmEvnt->u.authFlr.cause.cause == CM_EMM_SYNC_FAILURE))
    {
       if ( ueCb->ueCtxt.authInfo.authAlg == VB_MME_AUTH_XOR )
       {   
        if((evnt->m.emmEvnt->u.authFlr.failrPrm.pres == PRSNT_NODEF) &&
              (evnt->m.emmEvnt->u.authFlr.failrPrm.len == 6))
        {
            AUTS = evnt->m.emmEvnt->u.authFlr.failrPrm.authFailPrm;
            /* Authentication fails if consequetive Sync Fails are recved */
            if(ueCb->ueCtxt.ueHssCtxt.ueSecInfo.resync == FALSE)
            {
                ret = vbHssResync(ueCb->ueCtxt.ueImsi, AUTS,
                      &(ueCb->ueCtxt.ueHssCtxt.ueSecInfo));
                if(ret == ROK)
                {
                   ret = vbMmeSndAuthRequest(ueCb);
                }
            }
        }
    }  
       else /* MILANAGE based calculation */
       {
          if(FALSE == ueCb->ueCtxt.ueHssCtxt.ueSecInfo.resync)
          {
             cmMemcpy( rnd, ueCb->ueCtxt.authVec.rndNmb, CM_EMM_MAX_RAND );
             ret = vbMmeProcessSyncFlr( ueCb, evnt->m.emmEvnt->u.authFlr.failrPrm.authFailPrm,
                                     rnd );
             if ( ROK == ret )
             {
                ret = vbMmeSndAuthRequest(ueCb);
             }
          }
       }
    }  
 
    RETVALUE(ret);
}


#ifdef VB_MME_NAS_SEC
/*
 *
 *       Fun:   vbMmeHndlAuthFail 
 *       
 *       Desc:  This function handles incoming Authentication Failure message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlSecModeCmp
(
VbMmeUeCb    *pUeCb,
CmNasEvnt    *pNasEvnt
)
#else
PUBLIC S16 vbMmeHndlSecModeCmp(pUeCb, pNasEvnt)
VbMmeUeCb    *pUeCb;
CmNasEvnt    *pNasEvnt;
#endif
{
   S16            ret     = ROK;
   VbMmeEsmCb     *esmCb  = NULLP;

   VB_TRC2(vbMmeHndlSecModeCmp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling Security Mode Complete"));

   if ((NULLP != pUeCb->ueTunnelCb) && (0 != pUeCb->ueTunnelCb->remoteTeId))
   {
      ret = vbMmeUtlDelPdnConn(pUeCb, EGT_GTPC_CAUSE_LOC_DETACH);
      if (ROK == ret)
      {
         /* Delete session sent successfully,
          * so we are expecting response from SGW */
         RETVALUE(ret);
      }
   }

   ret = vbMmeGenerateEnbKey(&pUeCb->ueCtxt.secCtxt,\
         pUeCb->ueCtxt.secCtxt.lstUsdCnt);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ENB Key Generation Failed"));
      RETVALUE(ret);
   }

   ret = vbMmeGenerateNextHop(&pUeCb->ueCtxt.secCtxt,\
         pUeCb->ueCtxt.secCtxt.enbKey);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Next Hop Key Generation Failed"));
      RETVALUE(ret);
   }

   ret = vbMmeGetFrmTrnList(pUeCb, &esmCb, pUeCb->emmCb.piggyAction.tId);
   if ((ROK == ret) && (TRUE == esmCb->infoTransFlg))
   {
      ret = vbMmeSndEsmInfoReq(esmCb);
      if (ROK == ret)
      {
         RETVALUE(ROK);
      }
   }

   if (ROK == ret)
   {
      ret = vbMmeSndCSReq(pUeCb, esmCb);
   }

   RETVALUE(ROK);
}
#endif

/*
 *
 *       Fun:   vbMmeHndlAuthFail 
 *       
 *       Desc:  This function handles incoming Authentication Failure message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlSecModeRej
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlSecModeRej(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
    VB_TRC2(vbMmeHndlSecModeRej)

    ueCb->emmCb.cause = CM_EMM_IMPLICITLY_DETACHED;
    RETVALUE(ROK);
}

#endif /* end of #if VB_MME_AUTH */


/*
 *
 *       Fun:   vbMmeHndlAuthFail 
 *       
 *       Desc:  This function handles incoming Authentication Failure message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlAttachCmp
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlAttachCmp(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16        ret    = ROK;
   VbMmeEsmCb *esmCb = NULLP;

   VB_TRC2(vbMmeHndlAttachCmp)
      ueCb->emmCb.esmPiggyBacked = FALSE;

   ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, 
         evnt->m.emmEvnt->u.atchCmp.esmEvnt->m.esmEvnt->bearerId, 
         evnt->m.emmEvnt->u.atchCmp.esmEvnt->m.esmEvnt->prTxnId);
   if((ROK ==ret) && (NULLP != esmCb))
   {
      cmMemcpy((U8*)&esmCb->pco, (U8*)&evnt->m.emmEvnt->u.atchCmp.
            esmEvnt->m.esmEvnt->u.actAcc.protCfgOpt, sizeof(CmEsmProtCfgOpt));
   }

   RETVALUE(ret);
} 


/*
 *
 *       Fun:   vbMmeHandlDetachReq 
 *       
 *       Desc:  This function handles incoming Detach Request message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHandlDetachReq
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHandlDetachReq(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16 ret = RFAILED;

   VB_TRC2(vbMmeHandlDetachReq)

   if(CM_EMM_MID_TYPE_GUTI == evnt->m.emmEvnt->u.dtchReq.epsMi.type)
   {
      /* Fix Added*/
      if(cmMemcmp((U8*)&ueCb->ueCtxt.ueGuti, 
          (U8*)&evnt->m.emmEvnt->u.dtchReq.epsMi.u.guti, sizeof(ueCb->ueCtxt.ueGuti)))
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "GUTI Not Matching"));
         /* Since it is detach procedure, ignore the failure */
         /* RETVALUE(RFAILED); */
      }
   }
   else
   {
      if(ueCb->ueCtxt.ueImsiLen != evnt->m.emmEvnt->u.dtchReq.epsMi.len)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                         "vbMmeHandlDetachReq: IMSI Not Matching"));
         RETVALUE(RFAILED);
      }

      if(cmMemcmp(ueCb->ueCtxt.ueImsi, evnt->m.emmEvnt->u.dtchReq.epsMi.u.imsi.id, 
                   evnt->m.emmEvnt->u.dtchReq.epsMi.len)) 
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                         "vbMmeHandlDetachReq: IMSI Not Matching"));
         RETVALUE(RFAILED);
      }
   }

   /* Stop All Timer If Running */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_UE_CTX_DEL);
   ueCb->ueCtxt.suspendPaging = FALSE;

   if(1 == evnt->m.emmEvnt->u.dtchReq.detchType.switchOff)
   {
      VB_MME_SET_MASK(ueCb->bitmask, VB_EMM_DETACH_TYPE_SWITCHOFF)
   }
   else
   {
      VB_MME_UNSET_MASK(ueCb->bitmask, VB_EMM_DETACH_TYPE_SWITCHOFF)
   }

   ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);   
   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Delete Session Request"));
      if(!evnt->m.emmEvnt->u.dtchReq.detchType.switchOff)
      {
         ret = vbMmeSndDetachAcc(ueCb);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Detach Accept Failed"));
         }
      }

      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, SztCauseNasdetachEnum, 0);
   }
   
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "UE Ctxt Rls Cmd Failed"));
      ret = vbMmeUtlDelUeCb(ueCb);
   }

   RETVALUE(ret);
}


/*
 *
 *       Fun:   vbMmeHandlServiceReq 
 *       
 *       Desc:  This function handles incoming Service Request message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHandlServiceReq
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHandlServiceReq(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16         ret = RFAILED;

   VB_TRC2(vbMmeHandlServiceReq);

   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_UE_CTX_DEL);
   ueCb->ueCtxt.suspendPaging = FALSE;

   /* Initialize delete list */
   cmMemset((U8*)&ueCb->esmDelList, 0, sizeof(ueCb->esmDelList));
   ret = vbUtlGetS1APTaId(&((S1apPdu*)evnt->pdu)->pdu,\
         ((S1apPdu*)evnt->pdu)->pdu.val.initiatingMsg.procedureCode.val,\
         &ueCb->ueCtxt.tai);
   if (ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TAI Retrieval Failed"));
      ret = vbMmeSndServiceRej(ueCb, CM_EMM_NW_FAILURE);
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,\
            SztCauseMiscunspecifiedEnum, 0);
      RETVALUE(ret);
   }     

   ueCb->emmCb.typeOfId = CM_EMM_MID_TYPE_GUTI;
   cmMemcpy(ueCb->ueCtxt.srvPlmn, ueCb->ueCtxt.tai.plmnId.plmnId,\
         VB_PLMN_ID_LEN);
   vbMmeAddToUETaiLst(ueCb, &ueCb->ueCtxt.tai);
#ifdef VB_MME_AUTH 
   ret = vbMmeGenerateEnbKey(&ueCb->ueCtxt.secCtxt,\
         ueCb->ueCtxt.secCtxt.lstUsdCnt);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ENB Key Generation Failed"));
      ret = vbMmeSndServiceRej(ueCb, CM_EMM_NW_FAILURE);
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,\
            SztCauseMiscunspecifiedEnum, 0);
      RETVALUE(ret);
   }

   ueCb->ueCtxt.secCtxt.nxtHopChainCnt = 0;
   ret = vbMmeGenerateNextHop(&ueCb->ueCtxt.secCtxt,\
         ueCb->ueCtxt.secCtxt.enbKey);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Next Hop Key Generation Failed"));
      ret = vbMmeSndServiceRej(ueCb, CM_EMM_NW_FAILURE);
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,\
            SztCauseMiscunspecifiedEnum, 0);
      RETVALUE(ret);
   }
#endif

   ret = vbMmeSndInitialCtxtSetup(ueCb); 
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initial Context Setup Failed"));
      ret = vbMmeSndServiceRej(ueCb, CM_EMM_NW_FAILURE);
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,\
            SztCauseMiscunspecifiedEnum, 0);
   }

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeHndlTAUReq 
 *       
 *       Desc:  This function handles incoming TAU Request message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlTAUReq
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlTAUReq(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16            ret        = ROK;
   U8             bearerFlag = 11;
   VB_TRC2(vbMmeHndlTAUReq)

   ueCb->ueCtxt.tauType = evnt->m.emmEvnt->u.tauReq.epsUpdType.type;
   ueCb->ueCtxt.tauActv = evnt->m.emmEvnt->u.tauReq.epsUpdType.actv;

   if((TRUE == ueCb->ueCtxt.tauActv) && 
      (VB_EMM_UE_S1AP_INIT != ueCb->ueCtxt.s1apConState))
   {
      ret = vbMmeSndTAURej(ueCb, CM_EMM_INV_IE_TYPE);
      ueCb->relCause.choice.val  = CAUSE_NAS;
      ueCb->relCause.val.nas.val = SztCauseNasdetachEnum;
      ret = vbMmeAbortTAUProc(ueCb);

      RETVALUE(ret);
   }

   /* Stop All Timer If Running */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_UE_CTX_DEL);
   ueCb->ueCtxt.suspendPaging = FALSE;
   ueCb->ueCtxt.gutiAlloc = TRUE;
   
   /* Setting type of id as GUTI */
   ueCb->emmCb.typeOfId = CM_EMM_MID_TYPE_GUTI;

   /* Start: Store TAI */
   ret = vbUtlGetS1APTaId(&((S1apPdu*)evnt->pdu)->pdu,\
         ((S1apPdu*)evnt->pdu)->pdu.val.initiatingMsg.procedureCode.val,\
         &ueCb->ueCtxt.tai);
   if(ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TAI Retrieval Failed"));
      ret  = vbMmeSndTAURej(ueCb, CM_EMM_NW_FAILURE);
      /* Abort the procedure */
      ueCb->relCause.choice.val  = CAUSE_NAS;
      ueCb->relCause.val.nas.val = SztCauseNasunspecifiedEnum;
      ret = vbMmeAbortTAUProc(ueCb);

      RETVALUE(ret);
   }

   if (Sztid_initialUEMsg == ((S1apPdu*)evnt->pdu)->pdu.val.initiatingMsg.\
      procedureCode.val)
   {
      VB_MME_SET_MASK(ueCb->bitmask,VB_EMM_INIT_UE_MSG);
   }
   else
   {
      VB_MME_UNSET_MASK(ueCb->bitmask,VB_EMM_INIT_UE_MSG);
   }

   cmMemcpy(ueCb->ueCtxt.srvPlmn, ueCb->ueCtxt.tai.plmnId.plmnId,\
         VB_PLMN_ID_LEN);
   vbMmeAddToUETaiLst(ueCb, &ueCb->ueCtxt.tai);
   /* End: Store TAI */
   
   /* Initialize delete list */
   cmMemset((U8*)&ueCb->esmDelList, 0, sizeof(ueCb->esmDelList));
   if(TRUE == evnt->m.emmEvnt->u.tauReq.bearerSts.pres)
   {
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi05)
      {
         if(NULLP != ueCb->esmList[0]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[0]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi06)
      {
         if(NULLP != ueCb->esmList[1]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[1]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi07)
      {
         if(NULLP != ueCb->esmList[2]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[2]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi08)
      {
         if(NULLP != ueCb->esmList[3]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[3]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi09)
      {
         if(NULLP != ueCb->esmList[4]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[4]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi10)
      {
         if(NULLP != ueCb->esmList[5]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[5]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi11)
      {
         if(NULLP != ueCb->esmList[6]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[6]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi12)
      {
         if(NULLP != ueCb->esmList[7]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[7]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi13)
      {
         if(NULLP != ueCb->esmList[8]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[8]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi14)
      {
         if(NULLP != ueCb->esmList[9]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[9]);
         }
         bearerFlag--;
      }
      if(!evnt->m.emmEvnt->u.tauReq.bearerSts.ebi15)
      {
         if(NULLP != ueCb->esmList[10]) 
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[10]);
         }
         bearerFlag--;
      }

      if((!bearerFlag) || (!ueCb->creBid.nmbBearerId))
      {
         ret = vbMmeSndTAURej(ueCb, CM_EMM_NO_EPS_BEARER_CXT_ACTD);
         /* Abort the procdure */
         ueCb->relCause.choice.val  = CAUSE_NAS;
         ueCb->relCause.val.nas.val = SztCauseNasdetachEnum;
         ret = vbMmeAbortTAUProc(ueCb);
         
         RETVALUE(ret);
      }
   }

   ret = vbMmeSndTAUAcc(ueCb);
   if(ROK != ret)
   {
      ueCb->relCause.choice.val  = CAUSE_MISC;
      ueCb->relCause.val.nas.val = SztCauseMiscunspecifiedEnum;
      ret = vbMmeAbortTAUProc(ueCb);
      RETVALUE(ret);
   }

   /* If it is periodic update in IDLE mode, bearer est not requested 
      and GUTI is not assinged */
   if (VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_INIT_UE_MSG) && 
      (TRUE != ueCb->ueCtxt.tauActv) && (TRUE != ueCb->ueCtxt.gutiAlloc))
   {
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
            SztCauseNasnormal_releaseEnum, 0);
   }

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeHndlTAUCmp 
 *       
 *       Desc:  This function handles incoming TAU Complete message 
 *              to UE using EMM through OA
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlTAUCmp
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeHndlTAUCmp(ueCb, evnt)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
#endif
{
   S16 ret = ROK;
   VB_TRC2(vbMmeHndlTAUCmp)

   if(TRUE != ueCb->ueCtxt.tauActv) 
   {
      if (VB_MME_CHK_MASK(ueCb->bitmask,VB_EMM_INIT_UE_MSG))
      {
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
               SztCauseNasnormal_releaseEnum, 0);
      }
      else
      {
          VB_MME_END_EMM_PROC(ueCb);
      }
   }

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeHndlEsmInfoRsp
 *       
 *       Desc:  This function handles incoming Esm Info Response message 
 *              to UE using EMM through eNB
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlEsmInfoRsp
(
VbMmeEsmCb    *esmCb,
CmNasEvnt     *evnt
)
#else
PUBLIC S16 vbMmeHndlEsmInfoRsp(esmCb, evnt)
VbMmeEsmCb    *esmCb;
CmNasEvnt     *evnt;
#endif
{
   S16 ret = ROK;
   VB_TRC2(vbMmeHndlEsmInfoRsp)

   ret = vbMmeSndCSReq(esmCb->ueCb, esmCb);

   RETVALUE(ret);
}

/***************************************************************/
/***************************************************************/
/*                                                             */
/*                 All NAS utility functions                   */
/*                                                             */
/***************************************************************/

/*
 *
 *       Fun:   vbMmeHndlAuthFail 
 *       
 *       Desc:  This function handles incoming Authentication Failure message 
 *              to UE using EMM through 
 *       
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_nas_utl.c
 *       
 */
#ifdef VB_MME_AUTH
/* Function to verify authentication response added */
#ifdef ANSI
PUBLIC S16 vbMmeVerifyAuthResp
(
VbMmeUeCb    *ueCb, 
CmNasEvnt    *evnt
)
#else
PUBLIC S16 vbMmeVerifyAuthResp(ueCb, evnt)
VbMmeUeCb    *ueCb; 
CmNasEvnt    *evnt;
#endif
{
   U16      cnt = 0;

   VB_MME_TRC2(vbMmeVerifyAuthResp)

   for(cnt = 0; cnt < evnt->m.emmEvnt->u.authRsp.RES.len; cnt++)
   {
      if (evnt->m.emmEvnt->u.authRsp.RES.val[cnt] !=
         ueCb->ueCtxt.authVec.xres[cnt])
      {
         /* Mark the AKA parameters as invlid */
         ueCb->ueCtxt.ueHssCtxt.ueSecInfo.pres = NOTPRSNT;
         RETVALUE(RFAILED);
      }
   }

   /* If this was resync response update the sequence number */
   if(ueCb->ueCtxt.ueHssCtxt.ueSecInfo.resync == TRUE)
   {
      vbHssUpdSeqNum(ueCb->ueCtxt.ueImsi, &(ueCb->ueCtxt.ueHssCtxt.ueSecInfo));
   }

   RETVALUE(ROK);
}
#endif /* VB_MME_AUTH */



/* Function to abort Attach procedure */
#ifdef ANSI
PUBLIC S16 vbMmeAbortAttachProc
(
VbMmeUeCb    *ueCb 
)
#else
PUBLIC S16 vbMmeAbortAttachProc(ueCb)
VbMmeUeCb    *ueCb; 
#endif
{
   S16  ret = ROK;

   VB_TRC2(vbMmeAbortAttachProc);

   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }

   /*Stop Attach Accept Timer*/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.piggyAction.status = VB_MME_BEAR_STUP_FAILURE;
   vbMmeUtlDelPndTrns(ueCb); 

   /* Free any PDU preserved to track the retransmission
      of attach accept */
   if(NULLP != ueCb->emmCb.retrBuf.val)
   {
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, ueCb->emmCb.retrBuf.len);
      ueCb->emmCb.retrBuf.len = 0;
   }

   ret = RFAILED;
   if((NULLP != ueCb->ueTunnelCb) && (0 != ueCb->ueTunnelCb->remoteTeId))
   {
      /* Local clean up */
      /* TODO:Proper cause value setting */
      ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_SYS_FAIL);
   }

   if (ROK != ret)
   {
      ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
            ueCb->relCause.val.nas.val, 0);
   }

   if (ROK != ret)
   {
      /* Delete UE Cb As Attach Proc Failed */
      ret = vbMmeUtlDelUeCb(ueCb);
   }
   
   RETVALUE(ret);
}

/* Function to abort service request */
#ifdef ANSI
PUBLIC S16 vbMmeAbortServProc
(
VbMmeUeCb    *ueCb 
)
#else
PUBLIC S16 vbMmeAbortServProc(ueCb)
VbMmeUeCb    *ueCb; 
#endif
{
   S16  ret = ROK;
   VB_TRC2(vbMmeAbortServProc);
   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   ueCb->emmCb.state = VB_EMM_UE_REGISTERED;
   vbMmeUtlDelPndTrns(ueCb);

   ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
             ueCb->relCause.val.nas.val, 0 /* No need to send RAB Req */);
   RETVALUE(ret);
}

/* Function to abort Ue Ctxt Release proc */
#ifdef ANSI
PUBLIC S16 vbMmeAbortUeCtxtRelProc
(
VbMmeUeCb    *ueCb 
)
#else
PUBLIC S16 vbMmeAbortUeCtxtRelProc(ueCb)
VbMmeUeCb    *ueCb; 
#endif
{
   S16  ret = ROK;
   VB_TRC2(vbMmeAbortUeCtxtRelProc);

   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }

   vbMmeUtlDelPndTrns(ueCb);
   VB_MME_END_EMM_PROC(ueCb);

   /* vb001.101: Based on the cause take action */
   if((CAUSE_NAS == ueCb->relCause.choice.val) &&
     ((ueCb->relCause.val.nas.val == SztCauseNasdetachEnum) ||
     (ueCb->relCause.val.nas.val == SztCauseNasauthentication_failureEnum)))
   {
      /* +--- Patch 1.0.3 ---+ */
      ret = vbMmeUtlLocDel(ueCb);
      /* ret = vbMmeUtlDelUeCb(ueCb); */
      /* ---- Patch 1.0.3 ---- */
   }
   else if (((CAUSE_RADIONW == ueCb->relCause.choice.val) &&
             (ueCb->relCause.val.radioNw.val == SztCauseRadioNwuser_inactivityEnum)) ||
            ((CAUSE_NAS == ueCb->relCause.choice.val) &&
            (ueCb->relCause.val.nas.val == SztCauseNasnormal_releaseEnum)))
   {
      ueCb->ecmCb.state = VB_ECM_IDLE;
      ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;

      ret = vbMmeUtlDelUeCbOnConnId(ueCb);

      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                         "Unable To Delete From Hash List"));
         RETVALUE(RFAILED);
         /* During race conditions this may happen
          * 1. During eNB triggered UE Context Release process,
          * if eNB fwd Detach procedure to  MME 
          */
      }
   }
   /* Need to re-check */
   else if(CAUSE_NAS == ueCb->relCause.choice.val)
   {
      /* +--- Patch 1.0.3 ---+ */
      ret =vbMmeUtlLocDel(ueCb);
      /* ---- Patch 1.0.3 ---- */
   }
   else
   {
      /* Currently handled as user inactivity */
      ueCb->ecmCb.state = VB_ECM_IDLE;
      ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;
      ret = vbMmeUtlDelUeCbOnConnId(ueCb);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                          "Unable To Delete From Hash List"));
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ret);
}

/* Function to abort TAU procedure */
#ifdef ANSI
PUBLIC S16 vbMmeAbortTAUProc
(
VbMmeUeCb    *ueCb 
)
#else
PUBLIC S16 vbMmeAbortTAUProc(ueCb)
VbMmeUeCb    *ueCb; 
#endif
{
   S16  ret = ROK;
   VB_TRC2(vbMmeAbortTAUProc);

   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }

   /*Stop Attach Accept Timer*/
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3450);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR)ueCb, VB_TMR_EMM_3470);
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress = VB_EMM_COM_PRC_NONE;
   ueCb->emmCb.state = VB_EMM_UE_REGISTERED;
   vbMmeUtlDelPndTrns(ueCb);

   ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);
   if(ROK != ret)
   {

      ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
             ueCb->relCause.val.nas.val, 0 /* No need to send RAB Req */);
   }

   RETVALUE(ret);
}

/* Function to abort ESM procedure */
#ifdef ANSI
PUBLIC S16 vbMmeAbortEsmProc
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeAbortEsmProc(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   U8         indx   = 0;
   U8         ret    = 0;
   VbMmeEsmCb *esmCb = NULLP;

   VB_TRC2(vbMmeAbortEsmProc)

   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if(ueCb->esmList[indx])
      {
         esmCb = ueCb->esmList[indx];
         if(VB_MME_NULL_PROC == esmCb->proc)
         {
            continue;
         }
         else if(esmCb->proc == VB_MME_ACT_DED_BEAR_PROC)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Aborting Act Ded Bearer Procedure"));
            /* 1. stop the timer T3485 */
            vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
            VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);

            /* Inform To GW */
            vbMmeSndCBRsp(esmCb, 
                                     VB_MME_BEAR_STUP_FAILURE,
                                     EGT_GTPC_CAUSE_REQ_REJECT);
            VB_MME_END_ESM_PROC(esmCb);
            vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);
         }
         else if(esmCb->proc == VB_MME_ACT_DEF_BEAR_PROC)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Aborting Act Default Bearer Procedure"));
            vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
            VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);

            /* Donot receive response from UE */
            esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

            /* Inform To GW */
            ret = vbMmeSndDSReq(ueCb, esmCb,
                                     EGT_GTPC_CAUSE_UE_REFUSE);
            if(ROK != ret)
            {
               VB_MME_END_ESM_PROC(esmCb);
               vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);
            }
            else
            {
               /* Wait for response from SGW */
            }
         }  
         else if(esmCb->proc == VB_MME_DEACT_DED_BEAR_PROC)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Aborting Deact Ded Bearer Procedure"));
            /* 1. stop the timer T3495 */
            vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3495);
            VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);

            /* Inform SGW */
            vbMmeSendDeleteBearerRsp(esmCb, 
                                     VB_MME_BEAR_REL_FAILURE,
                                     EGT_GTPC_CAUSE_REQ_REJECT);
            VB_MME_END_ESM_PROC(esmCb);
            vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);
         }
         else if(esmCb->proc == VB_MME_DEACT_DEF_BEAR_PROC)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Aborting Deact Def Bearer Procedure"));
            /* 1. stop the timer T3495 */
            vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3495);
            VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);

            VB_MME_END_ESM_PROC(esmCb);
            vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);
         }
         else
         {
             VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                     "Unhandled Esm Proc(%d)", esmCb->proc));
         }
      } /* if */
   } /* for */
   
   /* Remove All Queued ESM Procedure */
   for(indx = 0; indx < VB_MME_MAX_SPND_PROC; indx++)
   {
      if(VB_MME_SPND_ESM_EVNT == ueCb->spndProc[indx].choice)
      {
         if(ueCb->spndProc[indx].proc.esmProc.evnt)
         {
           CM_FREE_NASEVNT(&ueCb->spndProc[indx].proc.esmProc.evnt);
         }
         ueCb->spndProc[indx].choice = VB_MME_SPND_NULL_EVNT;
         ueCb->spndProc[indx].proc.esmProc.esmCb   = NULLP;
         ueCb->spndProc[indx].proc.esmProc.evnt    = NULLP;
         ueCb->spndProc[indx].proc.esmProc.msgType = 0;
      }
   }

   /* Delete All Pending ESM Transaction */
   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if(ueCb->esmTrnList.esmInfo[indx].status) 
      {
         if(ueCb->esmTrnList.esmInfo[indx].esmCb)
         {
            VB_MME_END_ESM_PROC(ueCb->esmTrnList.esmInfo[indx].esmCb);
            VB_MME_FREE(ueCb->esmTrnList.esmInfo[indx].esmCb, 
                        sizeof(VbMmeEsmCb));
         }
         ueCb->esmTrnList.esmInfo[indx].esmCb =  NULL;
         ueCb->esmTrnList.esmInfo[indx].trnId =  0;
         ueCb->esmTrnList.esmInfo[indx].status = 0;
      }
   }

   RETVALUE(ROK);
}

/* Function to release the s1-connection locally */
#ifdef ANSI
PUBLIC S16 vbMmeRelLocS1Con
(
VbMmeUeCb    *ueCb 
)
#else
PUBLIC S16 vbMmeRelLocS1Con(ueCb)
VbMmeUeCb    *ueCb; 
#endif
{
   S16              ret = ROK;
   SztRelReq        relReq;

   VB_TRC2(vbMmeRelLocS1Con);

   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }

   /* process and delete this entry */
   if(VB_EMM_UE_S1AP_CONNECTED == ueCb->ueCtxt.s1apConState)
   {
      cmMemset((U8*)&relReq, 0, sizeof(SztRelReq));
      relReq.connIdType = SZT_CONN_TYPE_SP;
      relReq.relType = SZT_LOCAL_REL;
      relReq.connId = ueCb->spConnId;
      relReq.pdu = NULLP;

      ret = VbLiSztRelReq(&(ueCb->eNodebCb->sztSapCb->pst),\
            ueCb->eNodebCb->sztSapCb->spId, &relReq);

      /*Start mobile reachable timer to clean up the UE Context */
      if(ueCb->eNodebCb->eNbCfg.t3412Tmr)
      {
         vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE, 
              (ueCb->eNodebCb->eNbCfg.t3412Tmr + \
              VB_TMR_EMM_MOB_REACHABLE_OFFSET));
      }
   }

   /* UE In Idle Mode */
   vbMmeUtlDelPndTrns(ueCb);
   VB_MME_END_EMM_PROC(ueCb);
   ueCb->ecmCb.state = VB_ECM_IDLE;
   ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;

   /*Remove UE from the list */
   ret = vbMmeUtlDelUeCbOnConnId(ueCb);

   RETVALUE(ret);
}

/* Abort  Bearer Management Procedure */
#ifdef ANSI
PUBLIC S16 vbMmeAbortBrMgmtProc
(
VbMmeUeCb    *ueCb,
U8           proc,
Bool         isRel
)
#else
PUBLIC S16 vbMmeAbortBrMgmtProc(ueCb, proc, isRel)
VbMmeUeCb    *ueCb;
U8           proc;
Bool         isRel;
#endif
{
   U8           indx   = 0;
   VbMmeEsmCb   *esmCb = NULLP;

   VB_TRC2(vbMmeAbortBrMgmtProc);

   if(NULLP == ueCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Pointer Passed\n"));
      RETVALUE(RFAILED);
   }

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if((ueCb->esmTrnList.esmInfo[indx].status) &&
         (ueCb->esmTrnList.esmInfo[indx].esmCb)  &&
         (ueCb->esmTrnList.esmInfo[indx].esmCb->proc == proc))
      {
         esmCb = ueCb->esmTrnList.esmInfo[indx].esmCb;
         break;
      }
   }
    
   if(NULLP == esmCb)
   {
      if(VB_MME_PDN_CONN_PROC == proc)
      {
         for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
         {
            if(ueCb->esmList[indx] != NULLP && 
               ueCb->esmList[indx]->proc == proc)
            {
               esmCb = ueCb->esmList[indx];
            }         
         }
      }
      
      if(NULLP == esmCb)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Cb Not Found"));
         /* Cannot do anything terminate the procedure */
         VB_MME_END_EMM_PROC(ueCb);

         RETVALUE(RFAILED);
      }
   }       
             
   switch(proc)
   {
      case VB_MME_MOD_BEAR_PROC:
      {
         if(isRel)
         {
            vbMmeSendDeleteBearerRsp(esmCb, VB_MME_BEAR_REL_FAILURE,
                                     EGT_GTPC_CAUSE_SYS_FAIL);
         }

         vbMmeSndResModRej(esmCb, VB_ESM_NETWORK_FAILURE);
         break;
      }
      case VB_MME_RES_ALLOC_PROC:
      {
         vbMmeSndCBRsp(esmCb, VB_MME_BEAR_STUP_FAILURE,
                                  EGT_GTPC_CAUSE_SYS_FAIL);
         vbMmeSndResAllocRej(esmCb, VB_ESM_NETWORK_FAILURE);
         break;
      }
      case VB_MME_PDN_DIS_CONN_PROC:
      {
         vbMmeSndPdnDisConnRej(esmCb, VB_ESM_NETWORK_FAILURE);
         break;
      }
      case VB_MME_PDN_CONN_PROC:
      {
         vbMmeSndPdnConnRej(esmCb, VB_ESM_NETWORK_FAILURE);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Procedure"));
         RETVALUE(RFAILED);
      }
   }

   if(VB_MME_PDN_CONN_PROC == proc)
   {
      VB_MME_END_ESM_PROC(esmCb);
      vbMmeUtlDelEsmCb(esmCb, ueCb);
   }
   else
   {
      vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
      VB_MME_END_ESM_PROC(esmCb);
      VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
   }
   RETVALUE(ROK);
}


/**************************************************************************//**
 *
 * @brief 
 *    Aborts Handover Procedure.
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * 
 * @return
 *    No return Values
 *
 *****************************************************************************/
PUBLIC Void vbMmeAbortHOProc
(
VbMmeUeCb      *pUeCb
)
{
   S16         ret = RFAILED;
       
   VB_TRC2(vbMmeAbortHOProc);

   if (NULLP == pUeCb || NULLP == pUeCb->pHoData)
   {
      RETVOID;
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Aborting Handover Procedure"));

   if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
   {
      if (VB_MME_HO_TGT_MME == pUeCb->pHoData->hoMmeType)
      {
         vbMmeUtlDelS10Tun(pUeCb);

         if ((NULLP != pUeCb->ueTunnelCb) && \
            (0 != pUeCb->ueTunnelCb->remoteTeId))
         {
            ret = vbMmeUtlDelPdnConn(pUeCb, EGT_GTPC_CAUSE_SYS_FAIL);
         }

         if (ROK != ret)
         {
            ret = vbMmeInitCtxtRelProc(pUeCb, pUeCb->relCause.choice.val,\
                  pUeCb->relCause.val.nas.val, 0);
         }
      }
      else
      {
         if ((NULLP != pUeCb->pUeS10TunCb) && \
            (pUeCb->pUeS10TunCb->remoteTeId != 0))
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Relocation Cancel "\
                  "Request to target MME"));
            ret = vbMmeSndRCReq(pUeCb);
            if (ROK != NULLP)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Relocation Cancel "\
                     "Request to target MME"));
            }

            vbMmeUtlDelS10Tun(pUeCb);
         }

         vbMmeUtlDelHoData(pUeCb);
      }
   }
   else
   {
      /* TODO */ 
   }

   VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);
   VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_MME_CHANGE);
   VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_SGW_CHANGE);
   VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_SGW_CHANGE);

   RETVOID;
} /* vbMmeAbortHOProc */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */

/**********************************************************************
  
  End of file:     vb_mme_nas_utl.c@@/main/1 - Mon Jun 28 19:35:38 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sv               1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1  vb001.101    rk              1. Added new functions vbMmeSndActDedBearerReq,
                                         vbMmeHdlOutPagingReq, vbMmeHdlERABSetupRsp,
                                         vbMmeRcvEsmMsg, vbMmeSndActDedBearerReq
                                      2. Added new arg sztCause in vbMmeHdlIncUeRelMsg.
                                      3. Handled service request message.
                                      4. Modified function name from vbMmeSndUeCtxtRelease
                                         to vbMmeSndUeCtxtRelCmd
                                      5. Handled egtp create bearer req message
/main/1  vb002.101    rk              1. Dedicated Bearer Paging support changes
/main/1  vb003.101    chekar          1. Added Ue Rad Capability support.
/main/1  vb004.101    ve              1. debug print included under VB_MME_AUTH flag.
/main/1  vb005.101    rk              1. Added support for multiple Attach and Detach 
*********************************************************************91*/

