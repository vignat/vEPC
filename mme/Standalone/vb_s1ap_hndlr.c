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
 
     Name:     Trillium LTE CNE - MME
  
     Type:     C File
  
     Desc:     S1AP Handlers
  
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
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures      */
#include "vb.x"            /* CNE Application structures      */
#include "vb_hss.x"
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */
#include "vb_s3_hndlr.x"

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */
/* 
  Handles S1 Rcvd Msg during  Attach Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlAttachOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlAttachOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16 ret = ROK;

   SztDatEvntInd  *datEvnt; 
   SztCause *sztCause = NULLP;

       
   VB_TRC2(vbMmeHndlAttachOnS1apEvnt)
   
   switch(msgType)
   {
      case Sztid_InitCntxtSetup:
      {
         datEvnt = (SztDatEvntInd*)ptr;
         if(S1AP_PDU_SUCCESSFULOUTCOME == datEvnt->pdu->pdu.choice.val)
         {
            ret = vbMmeHdlInitialCtxtSetupRsp(ueCb, datEvnt);
            if(ROK != ret) 
            {
               /* Internal failure, wait till attach complete received from UE 
                * Set number of bearer to be modified as 0 */
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,\
                     "Initial Context Response Processing Failed"));
               ret = vbMmeUtlDelPdnConn(ueCb, EGT_GTPC_CAUSE_LOC_DETACH);
            }
            else
            {
               ret = vbMmeSndMBReq(ueCb, ueCb->esmList[0], FALSE);
               if(ROK != ret)
               {
                  ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                        SztCauseNasnormal_releaseEnum, 0);
               }
            }
         }
         else
         {
            datEvnt = (SztDatEvntInd*)ptr;
            ret = vbMmeHdlInitialCtxtSetupFail(ueCb, datEvnt);
            ret = vbMmeAbortAttachProc(ueCb);
         }

         break;
      }
      case Sztid_UECntxtRlsRqst:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received UE Context Release Request For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received UE Context Release Request For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif
         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);

         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }
         ret = vbMmeAbortAttachProc(ueCb);
         break;
      }
      case Sztid_NASNonDlvryInd:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received NAS Non Delivery Indication For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received NAS Non Delivery Indication For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif
         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);

         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         ret = vbMmeAbortAttachProc(ueCb);
         break;
      }
      case Sztid_ErrInd:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received Error Indication For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received Error Indication For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif
         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);

         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         ret = vbMmeAbortAttachProc(ueCb);
         break;
      }
      default:
      { 
         /* Add if anything else to handle or log event*/
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Msg(%d)", msgType));
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Attach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret); 
} /* end of vbMmeHndlAttachOnS1apEvnt */


/*
  Handles S1 Rcvd Msg during UE Detach  Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlUeDetachOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlUeDetachOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16 ret = ROK;
   
   VB_TRC2(vbMmeHndlUeDetachOnS1apEvnt)

   switch(msgType)
   {
      case Sztid_UECntxtRls:
      {
         ret = vbMmeHdlIncUeRelMsg(ueCb, ptr, msgType);
         break;
      } 
      case Sztid_NASNonDlvryInd:
      {
         ret = vbMmeHdlIncNASNonDlvryIndMsg(ueCb, ptr, msgType);
         break;
      }
      case Sztid_E_RABRlsInd:
      {

         break;
      }
      case Sztid_ErrInd:
      {
         ret = vbMmeHdlIncErrIndMsg(ueCb, ptr, msgType);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Event(%d)", msgType));
         break;
      }
   }
 
   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Detach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret);
}
 


/*
  Handles S1 Rcvd Msg during Network Detach Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlNwDetachOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlNwDetachOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16 ret = RFAILED;
   VB_TRC2(vbMmeHndlNwDetachOnS1apEvnt)
 
   switch(msgType)
   {
      case Sztid_UECntxtRls:
      {
         break;
      }
      case Sztid_NASNonDlvryInd:
      {

         break;
      }
      case Sztid_E_RABRlsInd:
      {

         break;
      }
      default:
      {
         /* Add if anything else to handle or log event*/
         break;
      }
   }
   
   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Detach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret);
}

/*
  Handles S1 Rcvd Msg during  Service Request Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlServReqOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlServReqOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16            ret         = ROK;
   SztDatEvntInd  *datEvnt;
   SztCause       *sztCause   = NULLP; 

   VB_TRC2(vbMmeHndlServReqOnS1apEvnt)

   switch(msgType)
   {
      case Sztid_InitCntxtSetup:
      {
         datEvnt = (SztDatEvntInd*)ptr;
         if (S1AP_PDU_SUCCESSFULOUTCOME == datEvnt->pdu->pdu.choice.val)
         {
            ret = vbMmeHdlInitialCtxtSetupRsp(ueCb, datEvnt);
            if (ROK != ret) 
            {
               ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                     SztCauseNasnormal_releaseEnum, 0);
               break;
            }
            else
            {
               /* Modify PDN Connections, Send Modify Bearer Request
                * to SGW */
               ret = vbMmeUtlModPdnConn(ueCb, 0);
               if(ROK != ret)
               {
                  ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                        SztCauseNasnormal_releaseEnum, 0);
               }                 
            } 
         }
         else
         {
            datEvnt = (SztDatEvntInd*)ptr;
            ret = vbMmeHdlInitialCtxtSetupFail(ueCb, datEvnt);
            if (ROK != ret)
            {
               ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                  SztCauseNasnormal_releaseEnum, 0);
            }                 
         }

         break;
      }
      case Sztid_UECntxtRlsRqst: /* UE Context Release Request */
      {

#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received UE Context Release Request For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received UE Context Release Request For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         /* Abort Service Request Procedure */
         ret = vbMmeAbortServProc(ueCb);
         break;
      }
      case Sztid_NASNonDlvryInd:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received NAS Non Delivery Indication For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received NAS Non Delivery Indication For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         /* Abort Service Request Procedure */
         ret = vbMmeAbortServProc(ueCb);
         break;
      }
      case Sztid_ErrInd:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received Error Indication For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received Error Indication For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         /* Abort Service Request Procedure */
         ret = vbMmeAbortServProc(ueCb);
         break;
      }
      case Sztid_E_RABRlsInd:
      {

         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Event(%d)", msgType));
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Service Req Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret);
}

/*
  Handles S1 Rcvd Msg during  TAU Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlTAUOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlTAUOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16            ret       = ROK;
   SztDatEvntInd  *datEvnt;
   SztCause       *sztCause = NULLP;

   VB_TRC2(vbMmeHndlTAUOnS1apEvnt)

   switch(msgType)
   {
      case Sztid_InitCntxtSetup:
      {
         datEvnt = (SztDatEvntInd*)ptr;
         if(S1AP_PDU_SUCCESSFULOUTCOME == datEvnt->pdu->pdu.choice.val)
         {
            ret = vbMmeHdlInitialCtxtSetupRsp(ueCb, datEvnt);

            if(ROK != ret) 
            {
               /* Return value is ignored */
               ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                       SztCauseNasnormal_releaseEnum, 0 /* No need to send RAB Req */);
               break;
            }
            else
            {
               /* Start modfying all connection */
               ret = vbMmeUtlModPdnConn(ueCb, 0);
               if(ROK != ret)
               {
                  ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                         SztCauseNasnormal_releaseEnum, 0 /* No need to send RAB Req */);
               }                 
            } 
         }
         else
         {
            datEvnt = (SztDatEvntInd*)ptr;
            ret = vbMmeHdlInitialCtxtSetupFail(ueCb, datEvnt);
            /* Locally detach UE */
            ueCb->relCause.choice.val  = CAUSE_NAS;
            ueCb->relCause.val.nas.val = SztCauseNasdetachEnum;
            
            /* Abort TAU Procedure */
            ret = vbMmeAbortTAUProc(ueCb);
         }

         break;
      }
      case Sztid_UECntxtRlsRqst: /* UE Context Release Request */
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received UE Context Release Request For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received UE Context Release Request For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         /* Abort Service Request Procedure */
         ret = vbMmeAbortTAUProc(ueCb);
         break;
      }
      case Sztid_NASNonDlvryInd:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received NAS Non Delivery Ind For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received NAS Non Delivery Ind For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         /* Abort Service Request Procedure */
         ret = vbMmeAbortTAUProc(ueCb);
         break;
      }
      case Sztid_ErrInd:
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received Error Ind For UE(%d)",
                    (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Received Error Ind For UE(%ld)",
                    (ueCb->ueCbKey)));
#endif

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                           (TknU8 ** )&sztCause);
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
            ueCb->relCause.choice.val  = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }

         /* Abort Tracking Area Update Procedure */
         ret = vbMmeAbortTAUProc(ueCb);
         break;
      }
      case Sztid_E_RABRlsInd:
      {

         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Event(%d)", msgType));
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "TAU Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret);
}
   
/*
  Handles S1 Rcvd Msg during Paging Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlPagingOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlPagingOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16 ret = RFAILED;
   VB_TRC2(vbMmeHndlPagingOnS1apEvnt)

   switch(msgType)
   {
      case Sztid_UECntxtRls:
      {
         break;
      }
      case Sztid_NASNonDlvryInd:
      {
         break;
      }
      case Sztid_ErrInd:
      {
         break;
      }
      default:
      {
         /* Add if anything else to handle or log event*/
         break;
      }
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Paging Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }

   RETVALUE(ret);
}


/*
  Handles S1 Rcvd Msg during Ctxt Release Procedure
*/

#ifdef ANSI
PUBLIC S16 vbMmeHndlCtxtRelOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlCtxtRelOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16 ret = ROK;
   VB_TRC2(vbMmeHndlCtxtRelOnS1apEvnt)
 
   switch(msgType)
   {
      case Sztid_UECntxtRlsRqst:
      case Sztid_UECntxtRls:
      case Sztid_ErrInd:
      {
         ret = vbMmeHdlIncUeRelMsg(ueCb, ptr, msgType);
         break;
      }
      case Sztid_E_RABRlsInd:
      {

         break;
      }
      default:
      {
         /* Add if anything else to handle or log event */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Event(%d)", msgType));
         break;
      }
   } 

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Ctxt Rel Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret);
}

/*
  Handles S1 Rcvd Msg during ESM Procedure
*/

#ifdef ANSI
PUBLIC S16 vbMmeHndlEsmProcOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlEsmProcOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16           ret       = ROK;
   VbMmeS1apEvnt s1apEvnt;

   VB_TRC2(vbMmeHndlEsmProcOnS1apEvnt)
 
   switch(msgType)
   {
      case Sztid_E_RABSetup:
      {
         ret = vbMmeHdlERABSetupRsp(ueCb, (SztDatEvntInd*)ptr);
         break;
      }
      case Sztid_E_RABRls:
      {
         ret = vbMmeHdlERABRlsRsp(ueCb, (SztDatEvntInd*)ptr);
         break;
      }
      case Sztid_UECntxtRlsRqst:
      {
         /* Queue the release procedure */
         cmMemcpy((U8*)&s1apEvnt.evnt.pdu, (U8*)ptr, sizeof(SztDatEvntInd));
         ret = vbMmeUtlSpndS1apProc(ueCb, &s1apEvnt, 
                                    ((SztDatEvntInd*)ptr)->pdu, msgType);
         if(ret == ROK)
         {
            /*Do not release memory */
            ((SztDatEvntInd*)ptr)->pdu = NULLP;
            /* Abort esm proc currently running */
            vbMmeAbortEsmProc(ueCb);
         }
         else
         {
            /* Should not enter here */
            ret = vbMmeHdlIncUeRelMsg(ueCb, ptr, msgType);
         }
         break;
      }
      case Sztid_NASNonDlvryInd:
      case Sztid_ErrInd:
      {
         /*Do not release memory */
         ((SztDatEvntInd*)ptr)->pdu = NULLP;
         /* Abort esm proc currently running */
         vbMmeAbortEsmProc(ueCb);
         break;
      }
      case Sztid_E_RABRlsInd:
      {
         ret = vbMmeHdlERABRelInd(ueCb, (SztDatEvntInd*)ptr);
         break;
      }
      default:
      {
         /* Add if anything else to handle or log event */
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Event(%d)", msgType));
         break;
      }
   } 

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,  "Dedicated Bearer  Management Proc "\
            "Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeHndlS1apOnEnbEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlS1apOnEnbEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType;
#endif
{
   S16 ret =  RFAILED;
   
   VB_TRC2(vbMmeHndlS1apOnEnbEvnt)

   switch (msgType)
   {
      case Sztid_UECntxtRlsRqst:
      {
         ueCb->proc = VB_MME_CTXT_REL_PROC;
         break;
      }

      case Sztid_E_RABRlsInd:
      {
         ueCb->proc = VB_MME_ESM_PROC;
         break;
      }

      case Sztid_HovrPrep:
      {
         ueCb->proc = VB_MME_HANDOVER_PROC;
         break;
      }

      default:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Unhandled Event(%d)", msgType));
         RETVALUE(ret);
         break;
      }
   }
   
   ret = vbMmeRcvS1apMsg(ueCb->suConnId, ptr, msgType);
   RETVALUE(ret);
}

/* 
  Handles S1 Rcvd Msg during Ue Context Modification Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlUeCtxtModOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlUeCtxtModOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16 ret = ROK;
   SztDatEvntInd  *datEvnt; 

   VB_TRC2(vbMmeHndlUeCtxtModOnS1apEvnt)
   
   switch(msgType)
   {
      case Sztid_UECntxtModification:
      {
        
         datEvnt = (SztDatEvntInd*)ptr;
         if(S1AP_PDU_SUCCESSFULOUTCOME == datEvnt->pdu->pdu.choice.val)
         {
            ret = vbMmeHdlUeCtxtModRsp(ueCb, datEvnt);
            if(ROK != ret) 
            {
               /* Internal failure, wait till attach complete received from UE 
                * Set number of bearer to be modified as 0 */
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Ue Context Mod Resp "\
                     "Processing Failed"));
               ueCb->proc = VB_MME_NULL_PROC;
               RETVALUE(ret);
            }
            else
            {
                /* Take action after successful processing of Response Message */
                /* Reset ueCb->proc */
                ueCb->proc = VB_MME_NULL_PROC;
            }
         }
         else /*Failed Out Come */
         {
            datEvnt = (SztDatEvntInd*)ptr;
            ret = vbMmeHdlUeCtxtModFail(ueCb, datEvnt);
            ueCb->proc = VB_MME_NULL_PROC;
         }

         break;
      }
      case Sztid_UECntxtRlsRqst: /* UE Context Release Request */
      {
         ret = vbMmeHdlIncUeRelMsg(ueCb, ptr, msgType);
         break;
      }
      case Sztid_ErrInd:
      {
         /* Make ueCb->proc NULL */
         ueCb->proc = VB_MME_NULL_PROC;
         ret = vbMmeHdlIncErrIndMsg(ueCb, ptr, msgType);
         break;
      }
      default:
      { 
         /* Add if anything else to handle or log event*/
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Msg(%d)", msgType));
         break;
      }
   }

   RETVALUE(ret); 
} /* end of vbMmeHndlUeCtxtModOnS1apEvnt */

/* 
  Handles S1 Rcvd Msg during ERAB Release Ind Procedure
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlBearRelIndOnS1apEvnt
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHndlBearRelIndOnS1apEvnt(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType
#endif
{
   S16             ret = ROK;
   VbMmeS1apEvnt   s1apEvnt;
       
   VB_TRC2(vbMmeHndlBearRelIndOnS1apEvnt)
   
   switch(msgType)
   {
      case Sztid_UECntxtRlsRqst: /* UE Context Release Request */
      {
         /* Queue the release procedure */
         cmMemcpy((U8*)&s1apEvnt.evnt.pdu, (U8*)ptr, sizeof(SztDatEvntInd));
         ret = vbMmeUtlSpndS1apProc(ueCb, &s1apEvnt, 
                                    ((SztDatEvntInd*)ptr)->pdu, msgType);
         if(ret == ROK)
         {
            /*Do not release memory */
            ((SztDatEvntInd*)ptr)->pdu = NULLP;
         }
         else
         {
            /* Should not enter here */
            ret = vbMmeHdlIncUeRelMsg(ueCb, ptr, msgType);
         }
         break;
      }
      default:
      { 
         /* Add if anything else to handle or log event*/
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled Msg(%d)", msgType));
         break;
      }
   }
   RETVALUE(ret); 
} /* end of vbMmeHndlBearRelIndOnS1apEvnt */



/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Related S1AP Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * @param[in]  msgType
 *    Type of message received on S1-MME interface
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverOnS1apEvnt
(
VbMmeUeCb      *pUeCb,
Ptr            *pEvnt,
U8             msgType
)
{
   S16            ret = ROK;
       
   VB_TRC2(vbMmeHndlHandoverOnS1apEvnt)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover Event of S1AP"));

   switch (msgType)
   {
      case Sztid_HovrPrep:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied Handover Preparation"));
         ret = vbMmeHndlHOPrep(pUeCb, (SztDatEvntInd*)pEvnt);
         break;
      }

      case Sztid_HovrResourceAllocn:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied Handover Resource "
               "Allocation"));
         ret = vbMmeHndlHOResAlloc(pUeCb, (SztConCfm *)pEvnt);

         break;
      }

      case Sztid_HovrNotification:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied Handover Notification"));
         ret = vbMmeHndlHONotif(pUeCb, (SztDatEvntInd*)pEvnt);
         break;
      }

      case Sztid_eNBStatusTfr:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied Handover Notification"));
         ret = vbMmeHndleNBStatusTfr(pUeCb, (SztDatEvntInd*)pEvnt);
         break;
      }

      case Sztid_UECntxtRlsRqst:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied UE Context Release "\
               "Request"));
         ret = vbMmeHndlUeCtxRlsReqDuringHO(pUeCb, (SztDatEvntInd*)pEvnt);
         break;
      }

      case Sztid_HovrCancel:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied Handover Cancel"));
         ret = vbMmeHndlHOCancel(pUeCb, (SztDatEvntInd*)pEvnt);
         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", msgType));
         break;
      }
   }

   if (ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover Procedure failed, "\
            "Terminating the Procedure"));
      VB_MME_END_EMM_PROC(pUeCb);
   }

   RETVALUE(ret); 
} /* vbMmeHndlHandoverOnS1apEvnt */


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Failure S1AP Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * @param[in]  msgType
 *    Type of message received on S1-MME interface
 * 
 * @return
 *    SUCCESS if Create Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverFailOnS1apEvnt
(
VbMmeUeCb      *pUeCb,
Ptr            *pEvnt,
U8             msgType
)
{
   S16            ret = ROK;
       
   VB_TRC2(vbMmeHndlHandoverFailOnS1apEvnt);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover Event of S1AP"));

   switch (msgType)
   {
      case Sztid_HovrResourceAllocn:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recevied Handover Resource "
               "Allocation"));
         ret = vbMmeHndlHOFailure(pUeCb);
         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Event(%d)", msgType));
         break;
      }
   }

   if (ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover Procedure failed, "\
            "Terminating the Procedure"));
      VB_MME_END_EMM_PROC(pUeCb);
   }

   RETVALUE(ret); 
} /* vbMmeHndlHandoverOnS1apEvnt */


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Failure Message received from eNodeB
 *    Sends Delete Session Request to SGW
 *    Sends Forward Relocation Response with failure cause to MME
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Handover Failure is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOFailure
(
VbMmeUeCb      *pUeCb
)
{
   S16             ret = ROK;
       
   VB_TRC2(vbMmeHndlHOFailure);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover Failure received from"
         " Target eNodeB"));
   VB_MME_SET_MASK(pUeCb->pHoData->bitmask, VB_MME_HO_FAILURE_RCVD);
   
   /* Support only for S1 Handover with MME and SGW change */
   if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
   {
      if( pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE )
      {
          /* SGW has changed during Handover, So delete the SGW sessions
                * by sending Delete Session Request. When Delete Session Response
                * is received send Forward Relocation Failure Response to source MME
                * if MME too has changed during handover
                */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Delete Session "\
               "Request to SGW"));
         ret = vbMmeUtlDelPdnConn(pUeCb, EGT_GTPC_CAUSE_SERVICE_DENIED);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Delete Session"
                  " Request to SGW"));
         }
      }
      else if( pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE )
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Response with Failure Cause to source SGSN"));
         /* In case of inter RAT handover, MME has to trigger Forward 
               *relocation response with "Request Rejected" to Source SGSN */
         ret = vbMmeSndS3FRRsp(pUeCb, EGT_GTPC_CAUSE_REQ_REJECT);
         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
                  "Response with Failure Cause to source eNB"));
         }
      }
   }
   else
   {
      /* Handover without MME relocation
       * Case were UE moves from one PS eNB to another PS eNB
       */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Preparation "\
            "Failure to source eNodeB"));
      ret = vbMmeSndHOPrepFail(pUeCb->pHoData->pOldUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Preparation "\
               "Failure to source eNodeB"));
      }

      vbMmeUtlDelHoData(pUeCb);
      vbMmeUtlDelUeCb(pUeCb);
   }

   RETVALUE(ret); 
} /* vbMmeHndlHOFailure */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
/******************************************************************************

         End of file:     vb_mme_s1ap_handler.x@@/main/1

******************************************************************************/

