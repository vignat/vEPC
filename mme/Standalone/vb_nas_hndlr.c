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

     Desc:    This file contains vb application's nas procedure
              handling source code

     File:    vb_mme_nas_handler.c

     Sid:     vb_mme_nas_handler.c@@/main/1 - Mon Jun 28 19:35:38 2010

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
#ifdef VB_MME_NAS_SEC
#include "vb_sec_intf.x"
#endif
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/*
 *
 *       Fun:   vbMmeHndlAttachOnNasEvnt
 *       
 *       Desc:  This function handles all the Attach Procedure
 *               
 *       
 *       Ret:   ROK on sucess
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme.c
 *       
 */

#ifdef ANSI
PUBLIC S16 vbMmeHndlAttachOnNasEvnt 
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlAttachOnNasEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{
  S16            ret     = ROK;

  VB_TRC2(vbMmeHndlAttachOnNasEvnt)

  switch(emmMsgType)
  {
     case CM_EMM_MSG_ATTACH_REQ:
     {
        ret = vbMmeHndlAttachReq(ueCb, evnt);
        break;
     }
     case CM_EMM_MSG_ID_RSP:
     {
        ret = vbMmeHndlIdentityResp(ueCb, evnt);
        if(ROK != ret)
        {
           /* Send the Attach Reject */
           ret  = vbMmeSndAttachReject(ueCb, ueCb->emmCb.cause, 0);
           break;
        }

        break;
     }

#ifdef VB_MME_AUTH 
     case CM_EMM_MSG_AUTH_RSP:
     {
        ret = vbMmeHndlAuthResp(ueCb, evnt);
        if(RFAILED == ret)
        {
           ret = vbMmeSndAttachReject(ueCb, CM_EMM_ILLEGAL_UE, 0);
        }
        break;
     } 
     case CM_EMM_MSG_AUTH_FAIL:
     {
        ret = vbMmeHndlAuthFail(ueCb, evnt);
        if(RFAILED == ret)
        {
           ret = vbMmeSndAttachReject(ueCb, CM_EMM_ILLEGAL_UE, 0);
        }
        break;
     } 
#endif /* VB_MME_AUTH */

#ifdef VB_MME_NAS_SEC
     case CM_EMM_MSG_SEC_MODE_CMP:
     {
        ret = vbMmeHndlSecModeCmp(ueCb, evnt);
        if (RFAILED == ret)
        {
           ret = vbMmeSndAttachReject(ueCb, ueCb->emmCb.cause, 0);
           break;
        }

        break;
     }
     case CM_EMM_MSG_SEC_MODE_REJ:
     {
        ret = vbMmeHndlSecModeRej(ueCb, evnt);
        ret = vbMmeSndAttachReject(ueCb, ueCb->emmCb.cause, 0);
        break;
     }
#endif /* VB_MME_NAS_SEC */

     case CM_EMM_MSG_ATTACH_CMP:
     {
        ret = vbMmeHndlAttachCmp(ueCb, evnt);
        break;
     }
     case CM_EMM_MSG_DETACH_REQ:
     {
        /* Jump to detach procedure */
        ueCb->proc = VB_MME_DETACH_UE_PROC;
        ret = vbMmeHndlUeDetachOnNasEvnt(ueCb, evnt, emmMsgType);
        break;
     }
     default:
     {
#ifdef ALIGN_64BIT     
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
           "Unhandled Msg(%d) For UE(%d)", emmMsgType, ueCb->ueCbKey));
#else
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
           "Unhandled Msg(%d) For UE(%ld)", emmMsgType, ueCb->ueCbKey));
#endif
        break;
     }

   } /* end of switch(msgType) */ 
   
   if (RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Attach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   
   RETVALUE(ret);
} /* end of vbMmeHndlAttachOnNasEvnt */


/*
 *
 *       Fun:   vbMmeHndlAttachOnNasEvnt
 *       
 *       Desc:  This function handles all the UE Init Detach Procedure
 *               
 *       Ret:   ROK on sucess
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeHndlUeDetachOnNasEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlUeDetachOnNasEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{

   S16      ret = ROK;
         
   VB_TRC2(vbMmeHndlUeDetachOnNasEvnt)

   switch(emmMsgType)
   {
      case CM_EMM_MSG_DETACH_REQ:
      {
         ret = vbMmeHandlDetachReq(ueCb, evnt);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Message(%d)", emmMsgType));
         break;
      }
   }
    
   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Detach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   } 

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlNwDetachOnNasEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlNwDetachOnNasEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEv
U8           emmMsgType;
#endif
{

   S16      ret = RFAILED;
   
   VB_TRC2(vbMmeHndlNwDetachOnNasEvnt)

   switch(emmMsgType)
   {
      /* TODO: NW and UE Detatch Collission Case */
      case CM_EMM_MSG_DETACH_REQ:
      {
         break;
      }
      case CM_EMM_MSG_DETACH_ACC:
      {
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK != ret)
         {
            /* Release the connection */
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
                  SztCauseNasdetachEnum, 0);
         }
         break;
      }
      default:
      {
         /* Add anything else to handle */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Message(%d)", emmMsgType));
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Detach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   } 
 
   RETVALUE(ret);
}
#ifdef ANSI
PUBLIC S16 vbMmeHndlServReqOnNasEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlServReqOnNasEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{
   S16      ret = ROK;
   
   VB_TRC2(handleServReqOnNasEvnt)

   switch(emmMsgType)
   {
      case CM_EMM_MSG_SERVICE_REQ:
      {
         ret = vbMmeHandlServiceReq(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Service Request Processing Failed"));
            ret = vbMmeSndServiceRej(ueCb, CM_EMM_NW_FAILURE);
         }
         break;
      }
#ifdef VB_MME_NAS_SEC 
      case CM_EMM_MSG_ID_RSP:
      {
         ret = vbMmeHndlIdentityResp(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Identity Response Processing Failed"));
            ret = vbMmeSndServiceRej(ueCb, CM_EMM_ILLEGAL_UE);
            break;
         }

         break;
      }
      case CM_EMM_MSG_AUTH_RSP:
      {
         ret = vbMmeHndlAuthResp(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Response Processing Failed"));
            ret = vbMmeSndServiceRej(ueCb, CM_EMM_ILLEGAL_UE);
         }
         break;
      } 
      case CM_EMM_MSG_AUTH_FAIL:
      {
         ret = vbMmeHndlAuthFail(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Failure"));
            ret = vbMmeSndServiceRej(ueCb, CM_EMM_ILLEGAL_UE);
         }
         break;
      } 
      case CM_EMM_MSG_SEC_MODE_CMP:
      {
         ret = vbMmeHndlSecModeCmp(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Security Mode Complete "\
                  "Processing Failed"));
            ret = vbMmeSndServiceRej(ueCb, CM_EMM_ILLEGAL_UE);
            break;
         }

         ret = vbMmeSndInitialCtxtSetup(ueCb);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initial Context Setup Failed"));
            ret = vbMmeSndServiceRej(ueCb, CM_EMM_NW_FAILURE);
         }
         break;
      }
      case CM_EMM_MSG_SEC_MODE_REJ:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Security Mode Reject Message "\
               "Received"));
         vbMmeHndlSecModeRej(ueCb, evnt);
         ret = vbMmeSndServiceRej(ueCb, CM_EMM_ILLEGAL_UE);
         break;
      }
#endif 
      case CM_EMM_MSG_DETACH_REQ:
      {
         /* Jump to detach procedure */
         ueCb->proc = VB_MME_DETACH_UE_PROC;
         ret = vbMmeHndlUeDetachOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      default:   
      {
         /* Add if anything else to be handle */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Message(%d)", emmMsgType));
         break;
      }
   }  
   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Service Req Proc Failed." 
                                        "Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   } 
   
    RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlTauOnNasEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlTauOnNasEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{
   S16      ret = RFAILED;
   
   VB_TRC2(vbMmeHndlTauOnNasEvnt)

   switch(emmMsgType)
   {
      case CM_EMM_MSG_TAU_REQ:
      {
         ret = vbMmeHndlTAUReq(ueCb, evnt);
         break;
      }
      case CM_EMM_MSG_TAU_CMP:
      {
         ret = vbMmeHndlTAUCmp(ueCb, evnt);
         break;
      }

#ifdef VB_MME_NAS_SEC
      case CM_EMM_MSG_ID_RSP:
      {
         ret = vbMmeHndlIdentityResp(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Identity Response Processing "\
                  "Failed"));
            vbMmeSndTAURej(ueCb, CM_EMM_ILLEGAL_UE);
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
            vbMmeAbortTAUProc(ueCb);
            break;
         }

         /* Synchronous function call so cause is CM_EMM_IMSI_UNKNOWN 
          * otherwise it is CM_EMM_NW_FAILURE
          */
         ret = vbMmeGetAuthVector(ueCb);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Vector Gen Failed"));
            vbMmeSndTAURej(ueCb, CM_EMM_NW_FAILURE);
            ueCb->relCause.choice.val  = CAUSE_MISC;
            ueCb->relCause.val.nas.val = SztCauseMiscunspecifiedEnum;
            vbMmeAbortTAUProc(ueCb);
            break;
         }

#ifdef ALIGN_64BIT
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Authentication Request"\
               "To UE(%d)\n", ueCb->ueCbKey));
#else                                 
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Authentication Request"\
               "To UE(%ld)\n", ueCb->ueCbKey));
#endif
         ret = vbMmeSndAuthRequest(ueCb);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Request Sending Failed"));
            vbMmeSndTAURej(ueCb, CM_EMM_NW_FAILURE);
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
            vbMmeAbortTAUProc(ueCb);
         }

         break;
      }
      case CM_EMM_MSG_AUTH_RSP:
      {
         ret = vbMmeHndlAuthResp(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Response Processing"));
            vbMmeSndTAURej(ueCb, CM_EMM_ILLEGAL_UE);
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
            vbMmeAbortTAUProc(ueCb);
         }
         break;
      } 
      case CM_EMM_MSG_AUTH_FAIL:
      {
         ret = vbMmeHndlAuthFail(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Auth Failure"));
            vbMmeSndTAURej(ueCb, CM_EMM_ILLEGAL_UE);
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
            vbMmeAbortTAUProc(ueCb);
         }
         break;
      } 
      case CM_EMM_MSG_SEC_MODE_CMP:
      {
         ret = vbMmeHndlSecModeCmp(ueCb, evnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Security Mode Complete "\
                  "Processing"));
            vbMmeSndTAURej(ueCb, CM_EMM_ILLEGAL_UE);
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
            vbMmeAbortTAUProc(ueCb);
            break;
         }

         if(ROK != vbMmeSndTAUAcc(ueCb))
         {
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
            vbMmeAbortTAUProc(ueCb);
            RETVALUE(ROK);
         }

         /* If it is periodic update in IDLE mode, bearer est not requested 
          * and GUTI is not assinged */
         if (VB_MME_CHK_MASK(ueCb->bitmask,VB_EMM_INIT_UE_MSG) &&
            (TRUE != ueCb->ueCtxt.tauActv) && (TRUE !=ueCb->ueCtxt.gutiAlloc))
         {
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
                  SztCauseNasnormal_releaseEnum, 0);
         }

         break;
      }
#endif
      case CM_EMM_MSG_DETACH_REQ:
      {
         /* Jump to detach procedure */
         ueCb->proc = VB_MME_DETACH_UE_PROC;
         RETVALUE(vbMmeHndlUeDetachOnNasEvnt(ueCb, evnt, emmMsgType));
      }
      default:
      {
         /* Add anything else to vbMmeHndl */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Message(%d)",emmMsgType));
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "TAU Procedure Failed, Terminating "\
            "the Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   } 

   RETVALUE(ret);
}


#ifdef ANSI
PUBLIC S16 vbMmeHndlGutiReallocOnNasEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlGutiReallocOnNasEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{

   S16      ret = ROK;
   
   VB_TRC2(vbMmeHndlGutiReallocOnNasEvnt)

   switch(emmMsgType)
   {
      case CM_EMM_MSG_GUTI_RLC_CMP:
      {
         VB_MME_END_EMM_PROC(ueCb);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandled Evnt(%d)", emmMsgType));
         break;
      }
   }
   
   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlEmmOnUeEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlEmmOnUeEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{
   S16 ret =   RFAILED;

   VB_TRC2(vbMmeHndlEmmOnUeEvnt)

   if(CM_EMM_MSG_ATTACH_REQ == emmMsgType)
   {
      ueCb->proc = VB_MME_ATTACH_PROC;
   }
   else if(CM_EMM_MSG_DETACH_REQ == emmMsgType)
   {
      ueCb->proc = VB_MME_DETACH_UE_PROC;
   }
   else if(CM_EMM_MSG_SERVICE_REQ == emmMsgType)
   {
      ueCb->proc = VB_MME_SERVICE_REQ_PROC;
   }
   else if(CM_EMM_MSG_TAU_REQ == emmMsgType)
   {
      ueCb->proc = VB_MME_TAU_PROC;
   }
   else if (CM_EMM_MSG_ATTACH_CMP == emmMsgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Attach Procedure Completed"));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(ROK);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", emmMsgType)); 
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(ret);
   }

   /* New procedure has been initiated, 
      so post back the message */   
   ret = vbMmeRcvEmmMsg(ueCb, evnt, emmMsgType);

   RETVALUE(ret); 
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlEmmOnEsmUeEvnt
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeHndlEmmOnEsmUeEvnt(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{
   S16 ret =   RFAILED;

   VB_TRC2(vbMmeHndlEmmOnEsmUeEvnt)

   if(CM_EMM_MSG_DETACH_REQ == emmMsgType)
   {
      if(ROK == vbMmeUtlSpndEmmProc(ueCb, evnt, emmMsgType))
      {
         vbMmeAbortEsmProc(ueCb);
         RETVALUE(ROK);
      }
      /* Should not come here */
      ueCb->proc = VB_MME_DETACH_UE_PROC;
   }
   else if(CM_EMM_MSG_TAU_REQ == emmMsgType)
   {
      if(ROK == vbMmeUtlSpndEmmProc(ueCb, evnt, emmMsgType))
      {
         vbMmeAbortEsmProc(ueCb);
         RETVALUE(ROK);
      }
      /* Should not come here */
      ueCb->proc = VB_MME_TAU_PROC;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Only Deatch/TAU Procedure Collision\
                                         With ESM Proc Handled(%d)", emmMsgType)); 
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(ret);
   }

   /* New procedure has been initiated, 
      so post back the message */   
   ret = vbMmeRcvEmmMsg(ueCb, evnt, emmMsgType);
   RETVALUE(ret);
}

/*
 * ESM Proc handler 
 */

#ifdef ANSI
PUBLIC S16 vbMmeHndlActDedBearOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlActDedBearOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16       ret   = ROK;
   VbMmeUeCb *ueCb = esmCb->ueCb;
  
   VB_TRC2(vbMmeHndlActDedBearOnNasEvnt)

   switch(esmMsgType)
   {
      case CM_ESM_MSG_ACTV_DED_BEAR_ACC:
      {
         ret = vbMmeSndCBRsp(esmCb,
                          VB_MME_BEAR_STUP_SUCCESS, 0);

         /* End current esm proc */
         VB_MME_END_ESM_PROC(esmCb);

         if(ROK != ret)
         {        
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Create Bearer Response Failed"));

            /*Delete ESM Cb  here */
            ret = vbMmeUtlDelEsmCb(esmCb, ueCb);

            /* Sending Create Br Rsp failed; To keep UE in sync with CNE */
            /* Release signalling connection and lets UE establish connection again*/  
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, 
                              SztCauseNasnormal_releaseEnum, 1 /*Send RAB*/);
            
            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Ctxt Rel Procedure Failed"));
               RETVALUE(RFAILED);
            }
         }
         break;
      }
      case CM_ESM_MSG_ACTV_DED_BEAR_REJ:
      {
         ret = vbMmeSndCBRsp(esmCb,
                          VB_MME_BEAR_STUP_FAILURE, EGT_GTPC_CAUSE_UE_REFUSE);

         /* End current esm proc */
         VB_MME_END_ESM_PROC(esmCb);

         /*Delete ESM Cb  here */
         ret = vbMmeUtlDelEsmCb(esmCb, ueCb);

         break;
      }
      default:
      {
         /* Log event */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType)); 
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Ded Bearer Act Proc Failed. "
                                        "Terminating The Procedure"));
      VB_MME_END_ESM_PROC(esmCb);
   } 
   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlActDefBearOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlActDefBearOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16       ret   = ROK;
   VbMmeUeCb *ueCb = esmCb->ueCb;
  
   VB_TRC2(vbMmeHndlActDefBearOnNasEvnt)

   switch(esmMsgType)
   {
      case CM_ESM_MSG_ACTV_DEF_BEAR_ACC:
      {
            /* check if the mPDN request from UE procedure is on - if yes then make sure that we have rxd
             * E_RAB setup rsp from eNodeB before sending mbr to SGW.
             */
            if ( ueCb->proc == VB_MME_PDN_CONN_PROC && esmCb->proc == VB_MME_ACT_DEF_BEAR_PROC )
            {
               if ( ueCb->rxd_erab_rsp )
               {
                  ret = vbMmeSndMBReq (esmCb->ueCb, esmCb, 0);
                  /* unset the flag for subsequent PDN conn procedures by the UE */
                  ueCb->rxd_erab_rsp = FALSE;
               }
               else
               {
                  ueCb->rxd_dflt_bearer_acc = TRUE;
               }
            }
            else
            {
         ret = vbMmeSndMBReq (esmCb->ueCb, esmCb, 0);
            }

         if(ROK != ret)
         {        
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Modify Req Failed"));

            /* End current esm proc */
            VB_MME_END_ESM_PROC(esmCb);

            /* Sending Create Br Rsp failed; To keep UE in sync with CNE */
            /* Release signalling connection and lets UE establish connection again*/  
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, 
                              SztCauseNasnormal_releaseEnum, 1 /*Send RAB*/);
            
            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Ctxt Rel Procedure Failed"));
               RETVALUE(RFAILED);
            }
         }
         break;
      }
      case CM_ESM_MSG_ACTV_DEF_BEAR_REJ:
      {
         ret = vbMmeSndDSReq(ueCb, esmCb, 0);

         if(ROK != ret)
         {
            /* End current esm proc */
            VB_MME_END_ESM_PROC(esmCb);
            /*Delete ESM Cb  here */
            ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
            RETVALUE(ret);
         }

         break;
      }
      default:
      {
         /* Log event */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType)); 
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Def Bearer Act Proc Failed. "
                                        "Terminating The Procedure"));
      VB_MME_END_ESM_PROC(esmCb);
   } 
   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlDeactDedBearOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlDeactDedBearOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16 ret = ROK;

   VB_TRC2(vbMmeHndlDeactDedBearOnNasEvnt)

   switch(esmMsgType)
   {
      case CM_ESM_MSG_DEACTV_BEAR_ACC:
      {
         ret = vbMmeSendDeleteBearerRsp(esmCb,
                               VB_MME_BEAR_REL_SUCCESS, 0);

         /* Terminate ESM Proc */
         VB_MME_END_ESM_PROC(esmCb);
         /* Remove esmCb from the list */
         vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);

         break;
      }
      default:
      {
         /* Log event */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType)); 
         break;
      }
   }

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlDeactDefBearOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlDeactDefBearOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeEsmCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16 ret = ROK;
   U16 i = 0;

   VB_TRC2(vbMmeHndlDeactDefBearOnNasEvnt)

   switch(esmMsgType)
   {
      case CM_ESM_MSG_DEACTV_BEAR_ACC:
      {
            /* Mark the apn inuse flag to false for the apn for this ESM cb */
            for(i = 0; i < VB_HSS_MAX_PDN_SUB_CTXT; i++)
            {
               /* 
                * indexing apn[1], becasue apn[0] is the length of the apn name.
                */
               if((ret = (cmMemcmp((U8 *)&esmCb->apn.apn,
                              (U8 *)&esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].apn,
                              esmCb->apn.len))) == 0)
               {
                  esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].subCntxtinUse = FALSE;
                  break;
               }
            }
         /* Terminate ESM Proc */
         VB_MME_END_ESM_PROC(esmCb);
         /* Remove esmCb from the list */
         vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);

         break;
      }
      default:
      {
         /* Log event */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType)); 
         break;
      }
   }

   RETVALUE(ret);
}



#ifdef ANSI
PUBLIC S16 vbMmeHndlResAllocOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlResAllocOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16        ret        = ROK;
   VbMmeEsmCb *tempEsmCb = NULLP;
   U8         cause      = VB_ESM_NETWORK_FAILURE;

   VB_TRC2(vbMmeHndlAllocResOnNasEvnt)
  
   switch(esmMsgType)
   {
      case CM_ESM_MSG_BEAR_RES_ALLOC_REQ:
      {
         /**********************************************/
         /* Error Check                                */
         /**********************************************/
         /* Bearer Id Should Be Zero */
         if(esmCb->bId)
         {
            cause = VB_ESM_INV_EPS_BEARER_ID;
            ret   = RFAILED;
         } 

         /*Linked Bearer Should Be A PDN */
         if(ROK == ret)
         {
            ret = vbMmeUtlFndEsmCb(esmCb->ueCb, &tempEsmCb, esmCb->linkBearId, 0); 
            if(ROK != ret)
            {
               cause = VB_ESM_INV_EPS_BEARER_ID;
            }
            else
            {
               if(CM_ESM_EPS_DEF_BEARER != tempEsmCb->bearerType)
               {
                  cause = VB_ESM_INV_EPS_BEARER_ID;
                  ret   = RFAILED;
               }
            }
         }

         /* Send Message To SGW */
         if(ROK == ret)
         {
            ret = vbMmeSendCreateBearerCmd(esmCb, 0);
         }
         
         if(ROK != ret)
         {
            ret = vbMmeSndResAllocRej(esmCb, cause);

            /* Clean up esm cb */
            VB_MME_END_ESM_PROC(esmCb);
            /* Remove from transaction list */
            vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
            /* Clean up memory */
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));

            RETVALUE(ret);
         }
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType));
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Alloc Resource Proc Failed. "
                                        "Terminating The Procedure"));
      /* Clean up esm cb */
      VB_MME_END_ESM_PROC(esmCb);
      /* Remove from transaction list */
      vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
      /* Clean up memory */
      VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
   } 
   RETVALUE(ret);
}



#ifdef ANSI
PUBLIC S16 vbMmeHndlPdnConnOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlPdnConnOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16 ret   = ROK;
   U8  cause = VB_ESM_NETWORK_FAILURE;
   
   VB_TRC2(vbMmeHndlPdnConnOnNasEvnt)

   switch(esmMsgType)
   {
      case CM_ESM_MSG_PDN_CONN_REQ:
      {
         /**********************************************/
         /* Error Check                                */
         /**********************************************/
         if(!esmCb->apn.pres)
         {
            cause = VB_ESM_INV_MAND_INFO;
            ret   = RFAILED; 
         }

         if(esmCb->bId)
         {
            cause = VB_ESM_INV_EPS_BEARER_ID;
            ret   = RFAILED; 
         }
        
         if(ROK == ret)
         {
            ret = vbMmeSndCSReq(esmCb->ueCb, esmCb);
         }
         if (ROK != ret)
         {
            ret = vbMmeSndPdnConnRej(esmCb, cause);
            /* Clean up esm cb */
            VB_MME_END_ESM_PROC(esmCb);
            /* Remove esmCb from transaction list */
            vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
            /* Release esmCb memory */
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
            RETVALUE(ret);
         }

         break;
      }
      default:
      {
         /* Log event */ 
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType)); 
         break;
      }
   }

   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Pdn Connectivity Procedure. "
                                        "Terminating The Procedure"));
      VB_MME_END_ESM_PROC(esmCb);
      /* Delete ESM Cb */
      vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);
   } 
   RETVALUE(ret);
}


#ifdef ANSI
PUBLIC S16 vbMmeHndlPdnDisConnOnNasEvnt
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlPdnDisConnOnNasEvnt(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16        ret        = ROK;
   U8         cause      = VB_ESM_NETWORK_FAILURE;
   VbMmeEsmCb *tempEsmCb = NULLP;
   
   VB_TRC2(vbMmeHndlPdnDisConnOnNasEvnt)

   switch(esmMsgType)
   {
      case CM_ESM_MSG_PDN_DISCONN_REQ:
      {
         /**********************************************/
         /* Error Check                                */
         /**********************************************/
         if(esmCb->bId)
         {
            cause = VB_ESM_INV_EPS_BEARER_ID;
            ret   = RFAILED; 
         }
         /* Linked Bearer Should Be A PDN */
         if(ROK == ret)
         {
            ret = vbMmeUtlFndEsmCb(esmCb->ueCb, &tempEsmCb, esmCb->linkBearId, 0); 
            if(ROK != ret)
            {
               cause = VB_ESM_INV_EPS_BEARER_ID;
            }
            else
            {
               if(CM_ESM_EPS_DEF_BEARER != tempEsmCb->bearerType)
               {
                  cause = VB_ESM_INV_EPS_BEARER_ID;
                  ret   = RFAILED;
               }
            }
         }
         if(ROK == ret)
         {
            /* Update bearer id */
            esmCb->bId = esmCb->linkBearId;
            ret = vbMmeSndDSReq(esmCb->ueCb, esmCb, FALSE);
         }
 
         if(ROK != ret)
         {
            ret = vbMmeSndPdnDisConnRej(esmCb, cause);
            /* Clean up esm cb */
            VB_MME_END_ESM_PROC(esmCb);
            /* Remove esmCb from transaction list */
            vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
            /* Release esmCb memory */
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
            RETVALUE(ret);
         }
         break;
      }
      default:
      {
         /* Log event */ 
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType)); 
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Pdn Dis Connectivity Procedure. "
                                        "Terminating The Procedure"));
      VB_MME_END_ESM_PROC(esmCb);
      vbMmeUtlDelEsmCb(esmCb, esmCb->ueCb);
   } 
   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlEsmOnUeEvnt 
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeHndlEsmOnUeEvnt(esmCb, evnt, esmMsgType)
VbMmeEsmCb   *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16   ret =     ROK;
   VB_TRC2(vbMmeHndlEsmOnUeEvnt)

   if((VB_MME_NULL_PROC != esmCb->ueCb->proc) &&
      ((CM_ESM_MSG_PDN_CONN_REQ == esmMsgType) ||
      (CM_ESM_MSG_BEAR_RES_ALLOC_REQ == esmMsgType) ||
      (CM_ESM_MSG_PDN_DISCONN_REQ == esmMsgType) ||
      (CM_ESM_MSG_BEAR_RES_MOD_REQ == esmMsgType)))
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Suspending ESM Procedure"));
      ret = vbMmeUtlSpndEsmProc(esmCb, evnt, esmMsgType);
      if(ret != ROK)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Suspending ESM Procedure Failed, "  
                                               "Ignoring The Message"));
         /* Clean up the transaction */
         vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
         VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));  
         CM_FREE_NASEVNT(&evnt);
         RETVALUE(ret);
      }

      RETVALUE(ROK);
   }

   if(CM_ESM_MSG_BEAR_RES_ALLOC_REQ == esmMsgType)
   {
      esmCb->proc = VB_MME_RES_ALLOC_PROC; 
      esmCb->ueCb->proc = VB_MME_ESM_PROC;
   }
   else if(CM_ESM_MSG_BEAR_RES_MOD_REQ == esmMsgType)
   {
      esmCb->proc = VB_MME_RES_MOD_PROC; 
      esmCb->ueCb->proc = VB_MME_ESM_PROC;
   }
   else if(CM_ESM_MSG_PDN_CONN_REQ == esmMsgType)
   {
      esmCb->proc = VB_MME_PDN_CONN_PROC; 
      esmCb->ueCb->proc = VB_MME_PDN_CONN_PROC;
   }
   else if(CM_ESM_MSG_PDN_DISCONN_REQ == esmMsgType)
   {
      esmCb->proc = VB_MME_PDN_DIS_CONN_PROC; 
      esmCb->ueCb->proc = VB_MME_ESM_PROC;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", esmMsgType));
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(ret);
   }
   
   if(ROK == ret)
   {
      ret = vbMmeRcvEsmMsg(esmCb, evnt, esmMsgType);     
      RETVALUE(ret);
   }

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Nas message received during Handover
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block 
 * @param[in]  pEvnt
 *    Pointer to Emm NAS Event
 * @param[in]  msgType
 *    Type of NAS message received
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverOnNasEvnt
(
VbMmeUeCb      *pUeCb,
CmNasEvnt      *pEvnt,
U8             emmMsgType 
)
{
   S16            ret = ROK;
       
   VB_TRC2(vbMmeHndlHandoverOnNasEvnt);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "NAS Message Event during Handover"));

   switch (emmMsgType)
   {
      case CM_EMM_MSG_TAU_REQ:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Tracking Area Update Request "\
               "Received"));

         if (VB_MME_HO_SRC_MME == pUeCb->pHoData->hoMmeType)
         {
            /* Handover is going on and the UE came back to source
             * May be because of some radio failures.
             * Delete the Handover Details and S10 Tunnel and Continue
             * with Handover Procedure
             */
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Ending Handover Procedure"));
            VB_MME_END_EMM_PROC(pUeCb);
            vbMmeUtlDelHoData(pUeCb);

            if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
            {
               vbMmeUtlDelS10Tun(pUeCb);
            }

            pUeCb->proc = VB_MME_TAU_PROC;

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling Tracking Area "\
                  "Update Request"));
            ret = vbMmeHndlTauOnNasEvnt(pUeCb, pEvnt, emmMsgType);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Handling Tracking Area "\
                     "Update Request"));
            }
         }
         else if (VB_MME_HO_TGT_MME == pUeCb->pHoData->hoMmeType)
         {
            pUeCb->proc = VB_MME_TAU_PROC;

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling Tracking Area "\
                  "Update Request"));
            ret = vbMmeHndlTauOnNasEvnt(pUeCb, pEvnt, emmMsgType);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Handling Tracking Area "\
                     "Update Request"));
            }
         }
         else
         {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid hoMmeType "\
                     "during handover"));
         }
         break;
      }

      case CM_EMM_MSG_DETACH_REQ:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Detach Request Received"));
         ret = vbMmeHandlDetachReq(pUeCb, pEvnt);
         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling of Detach Request"));
         }

         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", emmMsgType));
         break;
      }
   }

   RETVALUE(ret); 
} /* vbMmeHndlHandoverOnS1apEvnt */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
