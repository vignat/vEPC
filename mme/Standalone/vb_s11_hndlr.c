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

     Name:    Trillium LTE CNE - MME Controller Module

     Type:    C Include file

     Desc:    This file contains the vb application source code

     File:    vb_mme.c

     Sid:      vb_mme.c@@/main/1 - Mon Jun 28 19:35:38 2010

     Prg:     sv 
*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "ss_queue.h"      /* System services interface       */
#include "ss_mem.h"        /* System services interface       */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */
#ifdef EG_REL_930
#include "eg_dbutl.h" /* eGTP Message Dictionary related macros */
#endif /* EG_REL_930 */

#include "szt.h"           /* S1AP Upper Interface */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "cm_emm_esm.h"    /* EMM/ESM EDM */

#include <stdlib.h> /* For atoi() */
#ifdef VB_MME_AUTH
#include "vb_hss_autn.h"
#endif /* VB_MME_AUTH */
#ifdef VB_PERF_MEAS
#include <sys/time.h>
#endif

/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "ss_queue.x"      /* System services interface       */
#include "ss_task.x"        /* System services interface       */
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
#include "cm_pasn.x"       /* Common ASN.1 Library           */

#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */

#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */

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
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */
#include "vb_s10_hndlr.x"
#include "vb_s3_hndlr.x"

/*
*
*       Fun:   vbMmeSendHOReq
*       
*       Desc:  This function sends HandoverReq message.
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
PRIVATE S16 vbMmeSendHOReq
(
VbMmeUeCb      *pUeCb
)
{
   S16 ret = ROK;

   VB_TRC2(vbMmeSendHOReq)

   if (pUeCb  == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ueCb is NULL"));
      RETVALUE(RFAILED);
   }

   ret = vbMmeSndHOReq(pUeCb, pUeCb->eNodebCb);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Request "\
               "to eNodeB"));

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Delete Session "\
               "Request to SGW"));
      ret = vbMmeUtlDelPdnConn(pUeCb, EGT_GTPC_CAUSE_LOC_DETACH);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Delete Session"
                  " Request to SGW"));
      }
   }

   RETVALUE(ret);
}


/*
*
*       Fun:   vbMmeHndlAttachProcOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Attach procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlAttachOnSgwEvnt
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,   /* vb001.101: Added new argument */
TknU32         *transId
)
#else
PUBLIC S16 vbMmeHndlAttachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;
TknU32         *transId
#endif
{
   S16                   ret      = ROK;
   U8                    gtpCause = 0;
   VbMmeUeTunnelCb    *tunEnt  = NULLP;
   VbMmeEsmCb            *esmCb   = NULLP;
   U8                    esmIdx   = 0;

   VB_TRC2(vbMmeHndlAttachOnSgwEvnt)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW ueId(%d),"
                  "MsgType(%d)", ueCb->ueCbKey, msgType));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW ueId(%ld),"
                  "MsgType(%d)", ueCb->ueCbKey, msgType));
#endif                  

   switch (msgType)
   {
      case VB_EGTPC_CREATE_SESSION_RESPONSE:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                      "Receved Create Session Response From SGW"));

         ret = vbMmeVldNStrCSRsp(ueCb, egMsg, &gtpCause);
         if (ROK == ret)
         {
            /* Default bearer setup is successful */
            ueCb->emmCb.piggyAction.status = VB_MME_BEAR_STUP_SUCCESS;
#ifdef VB_IRAT_ACC_TEST
            esmCb = ueCb->esmList[0];
            esmCb->s1EnbTeId = 1234;
            vbMmeSndMBReq(ueCb, esmCb, FALSE );
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Attach Accept To UE"));
            ret = vbMmeSndAttachAccept(ueCb);

            if(ROK != ret)
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Attach Accept"));
               ret = vbMmeUtlDelPdnConn(ueCb,
                                EGT_GTPC_CAUSE_LOC_DETACH);
               
               if(ROK != ret)
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                           "Sending Delete Session Request Failed"));

                  ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                            SztCauseNasdetachEnum, 0);
               }
            }
#endif            
         }
         else
         {
            /* Invalid Create Session Response, reject the Attach */
            /* NOTE:Not Sending delete session in this case */ 
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                           "Sending Attach Reject To UE"));
            ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, ueCb->emmCb.piggyAction.bId, 
                                          ueCb->emmCb.piggyAction.tId);

            if((ret == ROK) && (esmCb != NULLP))
            {
               vbMmeUtlMapGtpToNasCause(gtpCause,\
                     &ueCb->emmCb.piggyAction.cause);
               ret = vbMmeSndAttachReject(ueCb, CM_EMM_ESM_FAILURE, esmCb);
            }
            else
            {
               ret = vbMmeSndAttachReject(ueCb, CM_EMM_NO_EPS_BEARER_CXT_ACTD,\
                     NULLP);
            }
         }

         break;
      }

      case VB_EGTPC_MODIFY_BEARER_RESPONSE:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                        "Received Modify Bearer Response From SGW"));
         ret = vbMmeVldNStrMBRsp(ueCb, egMsg, &gtpCause);
         ueCb->emmCb.esmPiggyBacked = FALSE;

         if (ROK != ret)
         {
            /* Begin network initiated detach by sending Detach Request
             * to the UE and Delete Session Request to the SGW */
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                           "Invalid Modify Bearer Response"));
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,
                    SztCauseMiscunspecifiedEnum, 0 /* Need to send RAB Req */);
         }
         else
         {
            VB_MME_END_EMM_PROC(ueCb);
         }
         break;
      }

      case VB_EGTPC_DELETE_SESSION_RESPONSE:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                        "Receiving Delete Session Response From SGW"));

         ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
         if(ROK == ret)
         {
            vbMmeUtlDelEsmCb(esmCb, ueCb);
            /* Delete More PDN */
            ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
            if(ROK == ret)
            {
                VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                      "Expecting More Delete Session Response From SGW"));
               RETVALUE(ROK);
            }
         }

         /* All PDN Deleted Successfully */
         ret = ROK;

         /*Send the Local Delete Request for the Tunnel */
         VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                        ueCb->ueTunnelCb->localTeIdKey);

         tunEnt =  ueCb->ueTunnelCb;
         ueCb->ueTunnelCb = NULLP;
         cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt); 
         VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));


         if(VB_MME_BEAR_STUP_ONGOING == ueCb->emmCb.piggyAction.status)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                                     "Sending Create Session Request"));

            for(esmIdx=0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
            {
               vbMmeUtlDelEsmCb(ueCb->esmList[esmIdx], ueCb);
            } 

            ret = vbMmeGetFrmTrnList(ueCb, &esmCb, 
                                     ueCb->emmCb.piggyAction.tId);

            if(ROK == ret)
            {
               ret = vbMmeSndCSReq(ueCb, esmCb);
            }
            if(ROK != ret)
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                                     "Sending Create Session Request Failed"));
               /* Send the Attach Reject */
               ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
               break;
            }
         }
         else
         {
            ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
                                ueCb->relCause.val.nas.val, 0 /* Need to send RAB Req */);

         }
 
         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message Type (%d)",
                           msgType));
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
}


/*
*
*       Fun:   vbMmeHndlUeDetachOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to UE Detach procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlUeDetachOnSgwEvnt
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,   /* vb001.101: Added new argument */
TknU32         *transId
)
#else
PUBLIC S16 vbMmeHndlUeDetachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;
TknU32         *transId;
#endif
{
   S16                  ret      = ROK;
   VbMmeUeTunnelCb      *tunEnt  = NULLP;
   VbMmeEsmCb           *esmCb   = NULLP;

   VB_TRC2(vbMmeHndlUeDetachOnSgwEvnt);
#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW"
                  "UeId(%d), MsgType(%d)", ueCb->ueCbKey, msgType));
#else                  
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW"
                  "UeId(%ld), MsgType(%d)", ueCb->ueCbKey, msgType));
#endif                  

   if (VB_EGTPC_DELETE_SESSION_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Receiving Delete Session Response From SGW"));

      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK == ret)
      {
         vbMmeUtlDelEsmCb(esmCb, ueCb);
         /* Delete More PDN */
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Expecting More Delete Session Response From SGW"));
            RETVALUE(ROK);
         }
      } 

      /* All PDN Deleted Successfully */
      ret = ROK;

      /*Send the Local Delete Request for the Tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                     ueCb->ueTunnelCb->localTeIdKey);

      /* send Detach accept if the UE is not Switched Off               *
       * TODO: setting of the VB_ESM_DETACH_TYPE_SWITCHOFF bitmask      *
       * when UE Detach request is received                             */
      if (!VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_DETACH_TYPE_SWITCHOFF))
      {
#if ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                        "Sending Detach Accept To UE(%d) ", ueCb->ueCbKey));
#else                        
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                        "Sending Detach Accept To UE(%ld) ", ueCb->ueCbKey));
#endif                        

         ret = vbMmeSndDetachAcc(ueCb);
      }

      tunEnt =  ueCb->ueTunnelCb;
      ueCb->ueTunnelCb = NULLP;
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt); 
      VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));

      /* All bearers are released on S-GW end */
      /* Send the UE Context Release command */
      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                 SztCauseNasdetachEnum, 0 /* No need to send RAB Req */);
#ifdef ALIGN_64BIT    
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Sending UE Ctx Release "
                     "Command To UE(%d) ", ueCb->ueCbKey));
#else
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Sending UE Ctx Release "
                     "Command To UE(%ld) ", ueCb->ueCbKey));
#endif
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message Type (%d)",
                        msgType));
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
*
*       Fun:   vbMmeHndlNwDetachOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to nework initiated detach  procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlNwDetachOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg      msgType,
EgMsg         *egMsg,
TknU32        *transId
)
#else
PUBLIC S16 vbMmeHndlNwDetachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg      msgType;
EgMsg         *egMsg;
TknU32        *transId;
#endif
{
   S16                  ret      = ROK;
   VbMmeUeTunnelCb      *tunEnt  = NULLP;
   VbMmeEsmCb           *esmCb   = NULLP;

   VB_TRC2(vbMmeHndlNwDetachOnSgwEvnt);
#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW"
                  "UeId(%d), MsgType(%d)", ueCb->ueCbKey, msgType));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW"
                  "UeId(%ld), MsgType(%d)", ueCb->ueCbKey, msgType));
#endif

   if (VB_EGTPC_DELETE_SESSION_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Receiving Delete Session Response From SGW"));

      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK == ret)
      {
         vbMmeUtlDelEsmCb(esmCb, ueCb);

         /* Delete More PDN */
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Expecting More Delete Session Response From SGW"));
            RETVALUE(ROK);
         }
      }

      /* All PDN Deleted Successfully */
      ret = ROK;

      /*Send the Local Delete Request for the Tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                     ueCb->ueTunnelCb->localTeIdKey);

      tunEnt =  ueCb->ueTunnelCb;
      ueCb->ueTunnelCb = NULLP;
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt); 
      VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));

      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, SztCauseNasdetachEnum, 0);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message Type (%d)",
                        msgType));
   }
   RETVALUE(ret);
}


/*
*
*       Fun:   vbMmeHndlServReqOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Service request procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlServReqOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg      msgType,
EgMsg         *egMsg,
TknU32        *transId
)
#else
PUBLIC S16 vbMmeHndlServReqOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg      msgType;
EgMsg         *egMsg;
TknU32        *transId;
#endif
{
   S16               ret      = RFAILED;
   U8                gtpCause = 0;
   U8                nasCause = 0;
   VbMmeEsmCb        *esmCb   = NULLP;
   EgIeLevelInfo     ieLvlInfo;
   EgIe              *egIe    = NULLP;
   U16               iePos    = 0;

   VB_TRC2(vbMmeHndlServReqOnSgwEvnt)

   if(VB_EGTPC_MODIFY_BEARER_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                     "Received Modify Bearer Response From SGW"));
      ret = vbMmeVldNStrMBRsp(ueCb, egMsg, &gtpCause);
      if (ROK != ret)
      {
         /* Begin network initiated detach by sending Detach Request
          * to the UE and Delete Session Request to the SGW */
         vbMmeUtlMapGtpToNasCause(gtpCause, &nasCause);
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                         "Invalid Modify Bearer Response"));

         ret = vbMmeSndServiceRej(ueCb, nasCause);

         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                 SztCauseNasnormal_releaseEnum, 1 /* Need to send RAB Req */);
      }
      else
      {
         ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
         if(ROK == ret)
         {
            ret = vbMmeUtlModPdnConn(ueCb, esmCb->pdnCb->id);
         }

         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                 "Expecting More Modify Bearer Response From SGW"));
            RETVALUE(ROK);
         }
         /* All PDN Modified Successfully */
         ret = ROK;

         /* Start Releasing All Deactivated Bearer */
         ret = vbMmeUtlRelBearer(ueCb, 0, NULLP);
         if(RFAILED == ret)
         {
            vbMmeDelAllFrmDelList(ueCb);
            VB_MME_END_EMM_PROC(ueCb);
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Waiting To Release More Bearer"));
         }
      }
   }
   else if(VB_EGTPC_DELETE_BEARER_REQ == msgType)
   {
      /*Get esm cb based on bearer id */
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                              = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 1;
      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);

      if(ROK != ret)
      {
         /*Get esm cb based on bearer id */
         cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
         ieLvlInfo.level                              = 0;
         ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
         ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 0;
         ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EBI Could Not Retrieved"));
         }
      }

      if(ROK == ret)
      {
         /*Bearer id cannot be zero */
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Bearer ID(%d)", egIe->t.valUnSgnd8));
         }
      }

      if(ret == ROK)
      {
         ret = vbMmeUtlRelBearer(ueCb, egIe->t.valUnSgnd8, NULLP);
      }
     
      if(RFAILED == ret)
      {
         VB_MME_END_EMM_PROC(ueCb);
         vbMmeDelAllFrmDelList(ueCb);
         /* All Bearers Deleted Successfully */
         ret = ROK;
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Waiting To Release More Bearer"));
      }
   }
   else if(VB_EGTPC_BEARER_RESOURCE_FAIL_IND == msgType)
   {
      /* Since it is network initiated procedure
         PTI will be zero. Transaction id used as reference to 
         get esmCb */

      ret = RFAILED;
      if(NULLP != transId)
      {
         ret = vbMmeUtlRelBearer(ueCb, 0, transId);
      }

      if(RFAILED == ret)
      {
         VB_MME_END_EMM_PROC(ueCb);
         vbMmeDelAllFrmDelList(ueCb);
         /* All Bearers Deleted Successfully */
         ret = ROK;
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Waiting To Release More Bearer"));
      }
   }

   if (VB_EGTPC_DELETE_SESSION_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Receiving Delete Session Response From SGW"));

      if(ROK == vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId))
      {
         vbMmeUtlDelEsmCb(esmCb, ueCb);
         /* Delete More PDN */
         if(ROK == vbMmeUtlDelPdnConn(ueCb, FALSE))
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Expecting More Delete Session Response From SGW"));
            RETVALUE(ROK);
         }
      } 

      /* All PDN Deleted Successfully */
      /* Send the Local Delete Request for the tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                     ueCb->ueTunnelCb->localTeIdKey);

      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), 
                        (PTR)ueCb->ueTunnelCb); 
      VB_MME_FREE(ueCb->ueTunnelCb, sizeof(VbMmeUeTunnelCb));
      ueCb->ueTunnelCb = NULLP;

      /* All bearers are released on S-GW end */
      /* Send the UE Context Release command */
      ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
                                ueCb->relCause.val.nas.val, 0);
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
*
*       Fun:   vbMmeHndlTAUOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to TAU procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlTAUOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg      msgType,
EgMsg         *egMsg,
TknU32        *transId
)
#else
PUBLIC S16 vbMmeHndlTAUOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg      msgType;
EgMsg         *egMsg;
TknU32        *transId;
#endif
{
   U8                gtpCause = 0;
   U8                nasCause = 0;
   S16               ret      = RFAILED;
   VbMmeUeTunnelCb   *tunEnt  = NULLP;
   VbMmeEsmCb        *esmCb   = NULLP;
   EgIeLevelInfo     ieLvlInfo;
   EgIe              *egIe    = NULLP;
   U16               iePos    = 0;


   VB_TRC2(vbMmeHndlTAUOnSgwEvnt)

   if(VB_EGTPC_MODIFY_BEARER_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                     "Received Modify Bearer Response From SGW"));
      ret = vbMmeVldNStrMBRsp(ueCb, egMsg, &gtpCause);
      if (ROK != ret)
      {
         /* Begin network initiated detach by sending Detach Request
          * to the UE and Delete Session Request to the SGW */
         vbMmeUtlMapGtpToNasCause(gtpCause, &nasCause);
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                         "Invalid Modify Bearer Response"));

         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                 SztCauseNasnormal_releaseEnum, 1 /* Need to send RAB Req */);
      }
      else
      {
         ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
         if(ROK == ret)
         {
            ret = vbMmeUtlModPdnConn(ueCb, esmCb->pdnCb->id);

         }

         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                 "Expecting More Modify Bearer Response From SGW"));
            RETVALUE(ROK);
         }

         /* All Bearers Are Modified Successfully */
         ret = ROK;
         
         ret = vbMmeUtlRelBearer(ueCb, 0, NULLP);
         if(RFAILED == ret)
         {
            vbMmeDelAllFrmDelList(ueCb);
            VB_MME_END_EMM_PROC(ueCb);

            /* No Bearer To Delete */
            ret = ROK;
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Waiting To Release More Bearer"));
         }
      }
   }
   else if(VB_EGTPC_DELETE_BEARER_REQ == msgType)
   {
      /*Get esm cb based on bearer id */
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                              = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 1;
      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);

      if(ROK != ret)
      {
         /*Get esm cb based on bearer id */
         cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
         ieLvlInfo.level                              = 0;
         ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
         ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 0;
         ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EBI Could Not Retrieved"));
         }
      }

      if(ROK == ret)
      {
         /*Bearer id cannot be zero */
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Bearer ID(%d)", egIe->t.valUnSgnd8));
         }
      }

      if(ret == ROK)
      {
         ret = vbMmeUtlRelBearer(ueCb, egIe->t.valUnSgnd8, NULLP);
      }
     
      if(RFAILED == ret)
      {
         VB_MME_END_EMM_PROC(ueCb);
         vbMmeDelAllFrmDelList(ueCb);
         /* All Bearers Deleted Successfully */
         ret = ROK;
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Waiting To Release More Bearer"));
      }
   }
   else if(msgType == VB_EGTPC_BEARER_RESOURCE_FAIL_IND)
   {
      /* Since it is network initiated procedure
         PTI will be zero. Transaction id used as reference to 
         get esmCb */

      ret = RFAILED;
      if(NULLP != transId)
      {
         ret = vbMmeUtlRelBearer(ueCb, 0, transId);
      }

      if(RFAILED == ret)
      {
         VB_MME_END_EMM_PROC(ueCb);
         vbMmeDelAllFrmDelList(ueCb);
         /* All Bearers Deleted Successfully */
         ret = ROK;
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Waiting To Release More Bearer"));
      }       
   }
   else if(VB_EGTPC_DELETE_SESSION_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Delete Session Response From SGW"));

      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK == ret)
      {
         vbMmeUtlDelEsmCb(esmCb, ueCb);
         /* Delete More PDN */
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Expecting More Delete Session Response From SGW"));
            RETVALUE(ROK);
         }
      }

      /* All PDN Deleted Successfully */
      ret = ROK;

      /*Send the Local Delete Request for the Tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                       ueCb->ueTunnelCb->localTeIdKey);

      tunEnt =  ueCb->ueTunnelCb;
      ueCb->ueTunnelCb = NULLP;
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt);
      VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));

      ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                SztCauseNasdetachEnum, 0 /* No need to send RAB Req */);
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
*
*       Fun:   vbMmeHndlImplicitDetachOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to TAU procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlImplicitDetachOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg      msgType,
EgMsg         *egMsg,
TknU32        *transId
)
#else
PUBLIC S16 vbMmeHndlImplicitDetachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg      msgType;
EgMsg         *egMsg;
TknU32        *transId;
#endif
{
   S16               ret     = ROK;
   VbMmeUeTunnelCb   *tunEnt = NULLP;
   VbMmeEsmCb        *esmCb  = NULLP;

   VB_TRC2(vbMmeHndlImplicitDetachOnSgwEvnt)

   if(VB_EGTPC_DELETE_SESSION_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Delete Session Response From SGW"));

      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK == ret)
      {
         vbMmeUtlDelEsmCb(esmCb, ueCb);
         /* Delete More PDN */
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Expecting More Delete Session Response From SGW"));
            RETVALUE(ROK);
         }
      }

      /* All PDN Deleted Successfully */
      ret = ROK;

      /*Send the Local Delete Request for the Tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                       ueCb->ueTunnelCb->localTeIdKey);

      tunEnt =  ueCb->ueTunnelCb;
      ueCb->ueTunnelCb = NULLP;
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt);
      VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));
      
      vbMmeUtlLocDel(ueCb);
   }

   if(RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
             "Impicit Detach Proc Failed. Terminating The Procedure"));
      VB_MME_END_EMM_PROC(ueCb);
   }
   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHndlPagingOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to paging procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlPagingOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg      msgType,
EgMsg         *egMsg,
TknU32        *transId
)
#else
PUBLIC S16 vbMmeHndlPagingOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg      msgType;
EgMsg         *egMsg;
TknU32        *transId;
#endif
{

   S16 ret      = ROK;
   U8  cause    = EGT_GTPC_CAUSE_ACC_REQ;

   VB_TRC2(vbMmeHndlPagingOnSgwEvnt)
   
   if(msgType == VB_EGTPC_DWN_LNK_DATA_NOTIFY)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Paging Request To eNodeB"));


      if((VB_EMM_UE_DEREGISTERED == ueCb->emmCb.state) || 
         (VB_EMM_UE_DEREG_INIT == ueCb->emmCb.state))
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
            Paging Request for Ue Id Because UE Is Deregistered(%d)",ueCb->ueCbKey));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
            Paging Request for Ue Id Because UE Is Deregistered(%ld)",ueCb->ueCbKey));
#endif
          cause = EGT_GTPC_CAUSE_LOC_DETACH;
      }
      else if(TRUE == ueCb->ueCtxt.suspendPaging)
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
            Paging Request for Ue Id Because Paging Is Suspended For UE(%d)",ueCb->ueCbKey));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
            Paging Request for Ue Id Because Paging Is Suspended For UE(%ld)",ueCb->ueCbKey));
#endif
         cause = EGT_GTPC_CAUSE_UNBLE_PAGE_UE_SUSPEN;
      }
      
      if(vbMmeHdlOutPagingReq(ueCb)!= ROK)
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
            Paging Request for Ue Id(%d)",ueCb->ueCbKey));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
            Paging Request for Ue Id(%ld)",ueCb->ueCbKey));
#endif
         cause = EGT_GTPC_CAUSE_SYS_FAIL;
      }
      else
      {
         vbMmeStartTmr((PTR)ueCb, VB_TMR_EMM_3413, 
                       ueCb->eNodebCb->eNbCfg.t3413Tmr);
      }

      /* send out the downlink data notifcation Ack towards SGW */
      ueCb->ueTunnelCb->seqNum = egMsg->msgHdr.seqNumber;

      if(vbMmeSndDwnLnkNotAck(ueCb, cause)!= ROK)
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
              Downlink Notification Ack For Ue ID(%d)",ueCb->ueCbKey));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Send \
              Downlink Notification Ack For Ue ID(%ld)",ueCb->ueCbKey));
#endif              
        
         VB_MME_END_EMM_PROC(ueCb);

         RETVALUE(RFAILED);
      }

      /* Terminate the procedure */
      VB_MME_END_EMM_PROC(ueCb);
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sent Downlink Notification Ack \
                                        Successfully"));
   }
   else
   {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled MsgType(%d)", msgType));
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
*
*       Fun:   vbMmeHndlUeCtxRelOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to UE Context Release procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlUeCtxRelOnSgwEvnt
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,
TknU32         *transId
)
#else
PUBLIC S16 vbMmeHndlUeCtxRelOnSgwEvnt(egtSapCb, ueCb, msgType, esmCb, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;
TknU32         *transId;
#endif
{
   S16             ret      = ROK;
   U8              esmCause = 0;
   VbMmeUeTunnelCb *tunEnt  = NULLP;
   VbMmeEsmCb      *esmCb   = NULLP;

   VB_TRC2(vbMmeHndlUeCtxRelOnSgwEvnt)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW "
                  "UE(%d), MsgType(%d)", ueCb->ueCbKey, msgType));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Receiving Msg From SGW "
                  "UE(%ld), MsgType(%d)", ueCb->ueCbKey, msgType));
#endif

   if (VB_EGTPC_RELEASE_ACCESS_BEARER_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                 "Receiving Release Access Bearer Response From SGW"));

      ret = vbMmeVldNStrRABRsp(ueCb, egMsg, &esmCause);

      /* Return value is ignored here*/
#ifdef ALIGN_64BIT      
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Sending UE Ctx Release Command To UE(%d)", ueCb->ueCbKey));
#else                
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Sending UE Ctx Release Command To UE(%ld)", ueCb->ueCbKey));
#endif                
      ret = vbMmeSndUeCtxtRelCmd(ueCb, ueCb->relCause.choice.val,
                                 ueCb->relCause.val.nas.val);
   }
   else if (VB_EGTPC_MODIFY_BEARER_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                    "Received Modify Bearer Response From SGW"));
#ifdef ALIGN_64BIT
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Sending UE Ctx Release Command To UE(%d)", ueCb->ueCbKey));
#else
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Sending UE Ctx Release Command To UE(%ld)", ueCb->ueCbKey));
#endif
      ret = vbMmeSndUeCtxtRelCmd(ueCb, ueCb->relCause.choice.val,
                                  ueCb->relCause.val.nas.val);
   }
   else if(VB_EGTPC_DWN_LNK_DATA_NOTIFY == msgType)
   {
      /* Corner case: UE Context Release Command is sent to eNB but
       * eNb not responded with UE Context Release Complete.
       * But Down link data notificatio received from SGW.
       * Fix: Locally release the connection and process the message from SGW */
      vbMmeRelLocS1Con(ueCb);
      /* Repost back the message */
      ret = vbMmeRcvSgwMsg(egtSapCb, ueCb, msgType, egMsg, transId);
   }
   else if(VB_EGTPC_DELETE_SESSION_RESPONSE == msgType)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                    "Receiving Delete Session Response From SGW"));

      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK == ret)
      {
         vbMmeUtlDelEsmCb(esmCb, ueCb);
         /* Delete More PDN */
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                  "Expecting More Delete Session Response From SGW"));
            RETVALUE(ROK);
         }
      }

      /* All PDN Deleted Successfully */
      ret = ROK;

      /*Send the Local Delete Request for the Tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                        ueCb->ueTunnelCb->localTeIdKey);

      tunEnt =  ueCb->ueTunnelCb;
      ueCb->ueTunnelCb = NULLP;
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt);
      VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));
      ret = vbMmeSndUeCtxtRelCmd(ueCb, ueCb->relCause.choice.val,
                                  ueCb->relCause.val.nas.val);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Failed Ctxt Rel Procedure"));
         VB_MME_END_EMM_PROC(ueCb);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message Type (%d)", msgType));
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
*
*       Fun:   vbMmeHndlEsmProcOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Esm  procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/

#ifdef ANSI
PUBLIC S16 vbMmeHndlEsmProcOnSgwEvnt
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,
TknU32         *transId
)
#else
PUBLIC S16 vbMmeHndlEsmProcOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;
TknU32         *transId
#endif
{
   S16              ret         = RFAILED;
   EgIeLevelInfo    ieLvlInfo;
   EgIe             *egIe       = NULLP;
   U16              iePos       = 0;
   VbMmeEsmCb       *esmCb      = NULLP;
   VbMmeEsmCb       *newEsmCb   = NULLP;

   VB_TRC2(vbMmeHndlEsmProcOnSgwEvnt)

   if((msgType == VB_EGTPC_CREATE_BEARER_REQ) || 
      (msgType == VB_EGTPC_BEARER_RESOURCE_FAIL_IND))
   {
      /* Get procedure transaction id */
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                               = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres  = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType      = EG_PROCEDURE_TID_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst      = 0;
      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
      if(ret != ROK)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Procedure Transaction Id IE Could Not Retrieved"));
      }

      if(ROK == ret)
      {
         /* It can be zero only incase of N/W initiated procedure */ 
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = vbMmeUtlCrtEsmCbOnTrnId(ueCb, &esmCb, 0, egIe->t.valUnSgnd8);
            esmCb->proc = VB_MME_RES_ALLOC_PROC;
            esmCb->ueCb->proc = VB_MME_ESM_PROC;
         }
         else
         {
            /* Get esm cb based on procedure transaction id */
            ret = vbMmeGetFrmTrnList(ueCb, &esmCb, egIe->t.valUnSgnd8);
            if(ROK != ret)
            {  
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found For Trans ID(%d)", egIe->t.valUnSgnd8));
            }
         }
      }

      if(ROK != ret)
      {
         ret = vbMmeAbortBrMgmtProc(ueCb, VB_MME_RES_ALLOC_PROC, FALSE);
         if((ROK != ret) && (VB_EGTPC_BEARER_RESOURCE_FAIL_IND == msgType))
         {
            ret = vbMmeAbortBrMgmtProc(ueCb, VB_MME_RES_MOD_PROC, FALSE);
         }
         RETVALUE(ret);
      }
   }
   else if(msgType == VB_EGTPC_DELETE_BEARER_REQ)
   {
      /*Get esm cb based on bearer id */
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                              = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 1;
      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);

      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EBI Could Not Retrieved"));
      }

      if(ROK == ret)
      {
         /*Bearer id cannot be zero */
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Bearer ID(%d)", egIe->t.valUnSgnd8));
         }
      }

      if(ROK == ret)
      {
         /* Find esmCb from bearer Id */
         ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, egIe->t.valUnSgnd8, 0);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Cb Not Found For Bearer Id (%d)", egIe->t.valUnSgnd8));
         }
      }

      if(ROK == ret)
      {
         /* Get temporary esmcb based on PTI */ 
         ret = vbMmeGetFrmTrnList(ueCb, &newEsmCb, esmCb->tId);
         if(ROK != ret)
         { 
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found For Trans ID(%d)", esmCb->tId));
         }
      }

      if(ROK != ret)
      {
         ret = vbMmeAbortBrMgmtProc(ueCb, VB_MME_RES_ALLOC_PROC, TRUE);
         RETVALUE(ret);
      }
      /* Take temporary esmCb */
      esmCb = newEsmCb;
  
   }
   else if(msgType == VB_EGTPC_UPDATE_BEARER_REQ)
   {
      /* Get esm cb based on esmCb */
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                              = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_BEARER_CNTX_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 0;

      ieLvlInfo.level                              = 1;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 0;

      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EPS Bearer Id Could Not Retrieved"));
      }

      if(ROK == ret)
      {
         /*KWORKS Fix*/
         if(egIe != NULLP)
         {
         /*Bearer Id should not be zero */
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Bearer ID"));
         }
      }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EPS Bearer Id Could Not Retrieved"));
         }
      }

         /* Find esmCb from bearer Id and update required parameters */
      {
         /*KWORKS Fix*/
         if(egIe != NULLP)
         {
         ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, egIe->t.valUnSgnd8, 0);
         if(ROK != ret)
         {   
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found For Bearer Id(%d)", egIe->t.valUnSgnd8));
         }
      }
      }

      if(ROK == ret)
      {
         /*KWORKS Fix*/
         if(esmCb != NULLP)
         {
         /* Get temporary esmcb based on PTI */ 
         ret = vbMmeGetFrmTrnList(ueCb, &newEsmCb, esmCb->tId);
         if(ROK != ret)
         {   
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found For Trans ID(%d)", esmCb->tId));
         }
      }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found."));
         }
      }

      if(ROK != ret)
      {
         ret = vbMmeAbortBrMgmtProc(ueCb, VB_MME_RES_MOD_PROC, FALSE);
         RETVALUE(ret);
      }
      /* Take temporary esmCb */
      esmCb = newEsmCb;
   }
   else if(msgType == VB_EGTPC_CREATE_SESSION_RESPONSE)
   {
      /*Get based on bearer id*/
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                              = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_BEARER_CNTX_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 0;
      ieLvlInfo.level                              = 1;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 0;

      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EPS Bearer Id Could Not Retrieved"));
      }

      if(ROK == ret)
      {
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Bearer ID"));
         }
      }
      
      if(ROK == ret)
      {
         /* Find esmCb from bearer Id and update required parameters */
         ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, egIe->t.valUnSgnd8, 0);
         if(ROK != ret)
         { 
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found For Bearer Id(%d)", egIe->t.valUnSgnd8));
         }
      }

      if(ROK != ret)
      {
         ret = vbMmeAbortBrMgmtProc(ueCb, VB_MME_PDN_CONN_PROC, FALSE);
         RETVALUE(ret);
      }
   }
   else if(msgType == VB_EGTPC_DELETE_SESSION_RESPONSE)
   {
      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK != ret)
      {
#ifdef ALIGN_64BIT
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found S11 Trans Id(%d)",\
               transId->val));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found S11 Trans Id(%ld)",\
               transId->val));
#endif
         /* Abort PDN Disconnectivity Procedure  Only */
         ret = vbMmeAbortBrMgmtProc(ueCb, VB_MME_PDN_DIS_CONN_PROC, FALSE);
         RETVALUE(ret);
      }
   }
   else if(msgType == VB_EGTPC_MODIFY_BEARER_RESPONSE)
   {
      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, transId);
      if(ROK != ret)
      {
#ifdef ALIGN_64BIT
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found S11 Trans Id(%d)",\
               transId->val));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Esm Cb Not Found S11 Trans Id(%ld)",\
               transId->val));
#endif
         RETVALUE(RFAILED);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandled Msg(%d)", msgType));
      RETVALUE(RFAILED);
   }

   /* Check for esmCb */   
   if(NULLP == esmCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"ESM Cb Not Found"));
      RETVALUE(RFAILED);
   }

   switch(esmCb->proc)
   {
      case VB_MME_RES_ALLOC_PROC:
      {
         ret = vbMmeHndlResAllocOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      case VB_MME_RES_MOD_PROC:
      {
         ret = vbMmeHndlResModOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      case VB_MME_PDN_CONN_PROC:
      {
         ret = vbMmeHndlPdnConnOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      case VB_MME_PDN_DIS_CONN_PROC:
      {
         ret = vbMmeHndlPdnDisConnOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      case VB_MME_ACT_DED_BEAR_PROC:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Proc(%d)", esmCb->proc));
         break;
      }
      case VB_MME_ACT_DEF_BEAR_PROC:
      {
         ret = vbMmeHndlActDefBearOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      case VB_MME_BEAR_REL_PROC:
      {
         ret = vbMmeHndlBearRelOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Proc(%d)", esmCb->proc));
         break;
      }
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   vbMmeHndlBrRelIndProcOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to ERAB Release Ind  procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/

#ifdef ANSI
PUBLIC S16 vbMmeHndlBrRelIndProcOnSgwEvnt
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,
TknU32         *transId
)
#else
PUBLIC S16 vbMmeHndlBrRelIndProcOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;
TknU32         *transId
#endif
{
   S16              ret         = RFAILED;
   EgIeLevelInfo    ieLvlInfo;
   EgIe             *egIe       = NULLP;
   U16              iePos       = 0;
   VbMmeEsmCb       *esmCb      = NULLP;

   VB_TRC2(vbMmeHndlBrRelIndProcOnSgwEvnt)

   if(msgType == VB_EGTPC_DELETE_BEARER_REQ)
   {
      /*Get esm cb based on bearer id */
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(ieLvlInfo));
      ieLvlInfo.level                              = 0;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieType     = EG_EBI_IETYPE;
      ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst     = 1;
      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);

      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EBI Could Not Retrieved"));
      }

      if(ROK == ret)
      {
         /*Bearer id cannot be zero */
         if(0 == egIe->t.valUnSgnd8)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Bearer ID(%d)", egIe->t.valUnSgnd8));
         }
      }

      if(ROK == ret)
      {
         /* Find esmCb from bearer Id */
         ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, egIe->t.valUnSgnd8, 0);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Cb Not Found For Bearer Id (%d)", egIe->t.valUnSgnd8));
            RETVALUE(ret);
         }
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandled Msg(%d)", msgType));
      RETVALUE(RFAILED);
   }

   /* Check for esmCb */   
   if(NULLP == esmCb)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"ESM Cb Not Found"));
      RETVALUE(RFAILED);
   }

   switch(esmCb->proc)
   {
      case VB_MME_BEAR_REL_PROC:
      {
         ret = vbMmeHndlBearRelOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Proc(%d)", esmCb->proc));
         break;
      }
   }

   RETVALUE(ROK);
}

/*
*
*       Fun:   vbMmeHndlNwTrigSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to network triggered events such as CBReq, UBReq
*              DBReq etc..
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlNwTrigSgwEvnt
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,   /* vb001.101: Added new argument */
TknU32         *transId
)
#else
PUBLIC S16 vbMmeHndlNwTrigSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;   /* vb001.101: Added new argument */
TknU32         *transId;
#endif
{
   S16 ret = ROK;

   VB_TRC2(vbMmeHndlNwTrigSgwEvnt)

   if(msgType == VB_EGTPC_DWN_LNK_DATA_NOTIFY)
   {
      ueCb->proc = VB_MME_PAGING_PROC;
   }
   else if(msgType == VB_EGTPC_CREATE_BEARER_REQ)
   {
      ueCb->proc = VB_MME_ESM_PROC;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Message(%d)", msgType));
      RETVALUE(RFAILED);
   }

   if(ROK == ret)
   { 
      /* Posting back the message */
      ret = vbMmeRcvSgwMsg(egtSapCb, ueCb, msgType, egMsg, transId);
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHndlResAllocOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Res Alloc Proc
*              DBReq etc..
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlResAllocOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg     msgType,
EgMsg         *egMsg,   
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeHndlResAllocOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg     msgType;
EgMsg         *egMsg; 
VbMmeEsmCb    *esmCb;
#endif
{
   S16 ret      = ROK;
   U8  nasCause = 0;
   U8  gtpCause = 0;
   U8  bearerId = 0;
   VB_TRC2(vbMmeHndlResAllocOnSgwEvnt)
   
   if(msgType == VB_EGTPC_CREATE_BEARER_REQ)
   {
      ret = vbMmeVldNStrCrtBrReq(ueCb, esmCb, egMsg, &gtpCause);

      if(ROK != ret)
      {
          ret = vbMmeSndCBRsp(esmCb, 
                               VB_MME_BEAR_STUP_FAILURE, gtpCause);
          vbMmeUtlMapGtpToNasCause(gtpCause, &nasCause);
          vbMmeSndResAllocRej(esmCb, nasCause);

          /* Terminate the procedure */
          VB_MME_END_ESM_PROC(esmCb);
          /* Remove from transaction list */
          vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
          /* Free memory */
          VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb)); 
          RETVALUE(ret);
      }
      else
      {
         ret = vbMmeUtlAllocBId(ueCb, &esmCb, &bearerId, 0);
         if(ROK == ret)
         {
            ret = vbMmeSndActDedBearerReq(esmCb);
         }
         if(ROK != ret)
         {
            vbMmeSndCBRsp(esmCb, VB_MME_BEAR_STUP_FAILURE, 
                                    EGT_GTPC_CAUSE_SYS_FAIL);

            vbMmeSndResAllocRej(esmCb, VB_ESM_NETWORK_FAILURE);
            /* Terminate the procedure */
            VB_MME_END_ESM_PROC(esmCb);
            /* Remove from transaction list */
            vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
            /* Free memory */
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb)); 
            RETVALUE(ret);
         }
         else
         {
            /* Remove from transaction list */
            vbMmeRmvFrmTrnList(ueCb, esmCb->tId);

            /* Updated procedure to dedicated proc */
            esmCb->proc = VB_MME_ACT_DED_BEAR_PROC;
         }
      }
   }
   else if(msgType == VB_EGTPC_BEARER_RESOURCE_FAIL_IND)
   {
      ret = vbMmeVldNStrBearResFailInd(ueCb, esmCb, egMsg, &gtpCause);

      vbMmeUtlMapGtpToNasCause(gtpCause, &nasCause);
      if (esmCb->proc == VB_MME_RES_ALLOC_PROC)
      {
         vbMmeSndResAllocRej(esmCb, nasCause);
      }
      else if(esmCb->proc == VB_MME_RES_MOD_PROC)
      {
         vbMmeSndResModRej(esmCb, nasCause);
      }

      /* Terminate the procedure */
      VB_MME_END_ESM_PROC(esmCb);
      /* Remove from transaction list */
      vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
      /* Free memory */
      VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb)); 
      RETVALUE(ret);
   }
   else
   {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled MsgType(%d)", msgType));
   }

   RETVALUE(ret);

}

/*
*
*       Fun:   vbMmeHndlResModOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Res Mod Proc
*              DBReq etc..
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlResModOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg     msgType,
EgMsg         *egMsg,   
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeHndlResModOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg     msgType;
EgMsg         *egMsg; 
VbMmeEsmCb    *esmCb;
#endif
{
   S16          ret        = ROK;
   U8           nasCause   = 0;
   U8           gtpCause   = 0;
   CmEsmCause   cause;
   VbMmeEsmCb   *tempEsmCb = NULLP;    

   VB_TRC2(vbMmeHndlResModOnSgwEvnt)
   
   if(msgType == VB_EGTPC_DELETE_BEARER_REQ)
   {
      ret = vbMmeVldNStrDelBrReq(ueCb, esmCb, egMsg, &gtpCause);

      if(ROK != ret)
      {
          vbMmeSendDeleteBearerRsp(esmCb, 
                               VB_MME_BEAR_REL_FAILURE, gtpCause);
          vbMmeUtlMapGtpToNasCause(gtpCause, &nasCause);
          vbMmeSndResModRej(esmCb, nasCause);

          /*Resourece clena up */
          VB_MME_END_ESM_PROC(esmCb);
          vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
          VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
          RETVALUE(ret);
      }
      else
      {
         ret = vbMmeUtlFndEsmCb(ueCb, &tempEsmCb, esmCb->bId, 0);
         if(ROK == ret)
         {
            cause.pres = PRSNT_NODEF;
            cause.val = VB_ESM_REGULAR_DEACTIVATION;
            ret = vbMmeSndDeActDedBearerReq(tempEsmCb, cause);
         }
         if(ROK != ret)
         {
            vbMmeSendDeleteBearerRsp(esmCb, VB_MME_BEAR_REL_FAILURE, 
                                    EGT_GTPC_CAUSE_SYS_FAIL);

            vbMmeSndResModRej(esmCb, VB_ESM_NETWORK_FAILURE);

            /*Resourece clena up */
            VB_MME_END_ESM_PROC(esmCb);
            vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
            RETVALUE(ret);
         }
         else
         {
            vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
            /* Updated procedure to dedicated proc */
            tempEsmCb->proc = VB_MME_DEACT_DED_BEAR_PROC;
         }
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled MsgType(%d)", msgType));
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHndlBearRelOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Bear Rel Proc
*              DBReq etc..
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlBearRelOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg     msgType,
EgMsg         *egMsg,   
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeHndlBearRelOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg     msgType;
EgMsg         *egMsg; 
VbMmeEsmCb    *esmCb;
#endif
{
   S16          ret        = ROK;
   U8           gtpCause   = 0;

   VB_TRC2(vbMmeHndlBearRelOnSgwEvnt)
   
   if(msgType == VB_EGTPC_DELETE_BEARER_REQ)
   {
      ret = vbMmeVldNStrDelBrReq(ueCb, esmCb, egMsg, &gtpCause);

      if(ROK != ret)
      {
          vbMmeSendDeleteBearerRsp(esmCb, 
                               VB_MME_BEAR_REL_FAILURE, gtpCause);

          /*Resourece clena up */
          VB_MME_END_ESM_PROC(esmCb);
          RETVALUE(ret);
      }
      else
      {
         vbMmeSendDeleteBearerRsp(esmCb, VB_MME_BEAR_REL_SUCCESS,
               EGT_GTPC_CAUSE_ACC_REQ);

         /*Resourece clena up */
         VB_MME_END_ESM_PROC(esmCb);

         /* Delete EsmCb */
         vbMmeUtlDelEsmCb(esmCb, ueCb);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                    "Unhandled MsgType(%d)", msgType));
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHndlPdnConnOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Pdn Connectivity procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlPdnConnOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg     msgType,
EgMsg         *egMsg,   
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeHndlPdnConnOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg     msgType;
EgMsg         *egMsg; 
VbMmeEsmCb    *esmCb;
#endif
{
   S16 ret = ROK;
   U8  nasCause = 0;
   U8  gtpCause = 0;
   VB_TRC2(vbMmeHndlPdnConnOnSgwEvnt)
   
   if(msgType == VB_EGTPC_CREATE_SESSION_RESPONSE)
   {
      ret = vbMmeVldNStrCSRsp(ueCb, egMsg, &gtpCause);

      if(ROK != ret)
      {
          vbMmeUtlMapGtpToNasCause(gtpCause, &nasCause);
          vbMmeSndPdnConnRej(esmCb, nasCause);
          VB_MME_END_ESM_PROC(esmCb);
          vbMmeUtlDelEsmCb(esmCb, ueCb); 
      }
      else
      {
         ret = vbMmeSndActDefBearerReq(esmCb);
         if(ROK != ret)
         {
            vbMmeSndPdnConnRej(esmCb, VB_ESM_NETWORK_FAILURE);
            ret = vbMmeSndDSReq(ueCb, esmCb, FALSE);
            if(ROK != ret)
            {
               VB_MME_END_ESM_PROC(esmCb);
               ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
            }            
         }
         else
         {
            /* Updated procedure to default bearer proc */
            esmCb->proc = VB_MME_ACT_DEF_BEAR_PROC;
         }
      }
   }
   else
   {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled MsgType(%d)", msgType));
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHndlPdnDisConnOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Pdn DisConnectivity procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlPdnDisConnOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg     msgType,
EgMsg         *egMsg,   
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeHndlPdnDisConnOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg     msgType;
EgMsg         *egMsg; 
VbMmeEsmCb    *esmCb;
#endif
{
   S16          ret        = ROK;
   CmEsmCause   cause;
   VbMmeEsmCb   *tempEsmCb = NULLP;

   VB_TRC2(vbMmeHndlPdnDisConnOnSgwEvnt)
   
   if(msgType == VB_EGTPC_DELETE_SESSION_RESPONSE)
   {
      /*TODO:Validation of message content */
      cause.pres = PRSNT_NODEF;
      cause.val = VB_ESM_REGULAR_DEACTIVATION;

      ret = vbMmeUtlFndEsmCb(ueCb, &tempEsmCb, esmCb->linkBearId, 0);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM Cb Not Found (%d)", esmCb->linkBearId));
         RETVALUE(RFAILED);
      }

      /* Remove temp esm cb from list */
      tempEsmCb->tId = esmCb->tId;
      vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
      VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));

      /* Act on permanent esmCb */
      ret = vbMmeSndDeActDefBearerReq(tempEsmCb, cause);
      if(ROK != ret)
      {
         VB_MME_END_ESM_PROC(tempEsmCb);
         vbMmeUtlDelEsmCb(tempEsmCb, ueCb);
      }
      else
      {
         /* Updated procedure to deact default bearer proc */
         tempEsmCb->proc = VB_MME_DEACT_DEF_BEAR_PROC;
      }
   }
   else
   {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                           "Unhandled MsgType(%d)", msgType));
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHndlActDefBearOnSgwEvnt
*       
*       Desc:  This function handles all the incoming EGTPC messages from S-GW
*              with respect to Activate Def Bearer Context procedure
*        
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme_s11_hndlr.c
*/
#ifdef ANSI
PUBLIC S16 vbMmeHndlActDefBearOnSgwEvnt
(
VbMmeEgtSapCb *egtSapCb,
VbMmeUeCb     *ueCb,
VbGtpcMsg     msgType,
EgMsg         *egMsg,   
VbMmeEsmCb    *esmCb
)
#else
PUBLIC S16 vbMmeHndlActDefBearOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg, esmCb)
VbMmeEgtSapCb *egtSapCb;
VbMmeUeCb     *ueCb;
VbGtpcMsg     msgType;
EgMsg         *egMsg; 
VbMmeEsmCb    *esmCb;
#endif
{
   VB_TRC2(vbMmeHndlActDefBearOnSgwEvnt)
   
   if(msgType == VB_EGTPC_MODIFY_BEARER_RESPONSE)
   {
      VB_MME_END_ESM_PROC(esmCb);
   }
   else if(msgType == VB_EGTPC_DELETE_SESSION_RESPONSE)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Delete Session Response Received"));
      VB_MME_END_ESM_PROC(esmCb);
      vbMmeUtlDelEsmCb(esmCb, ueCb);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Evnt(%d)", msgType));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/**************************************************************************//**
 *
 * @brief 
 *    Handles Create Session Response recieved as a part of S1 Handover
 *    Procedure
 *             
 * @param[in]  pEgtSapCb
 *    Pointer to EGT Sap control block
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[out] msgType
 *    GTP message type
 * @param[out] pEgMsg
 *    Pointer to Egtp message structure
 * @param[out] pTransId
 *    Pointer to transaction ID
 * 
 * @return
 *    SUCCESS if Create Session Response is handled Successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHandoverOnSgwEvnt
(
VbMmeEgtSapCb  *pEgtSapCb,
VbMmeUeCb      *pUeCb,
VbGtpcMsg      msgType,
EgMsg          *pEgMsg,
TknU32         *pTransId
)
{
   S16                  ret      = ROK;
   U8                   gtpCause = 0;

   VB_TRC2(vbMmeHndlHandoverOnSgwEvnt)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Receiving Msg From SGW ueId(%d),"
         "MsgType(%d)", pUeCb->ueCbKey, msgType));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Receiving Msg From SGW ueId(%ld),"
         "MsgType(%d)", pUeCb->ueCbKey, msgType));
#endif                  

   switch (msgType)
   {
      case VB_EGTPC_CREATE_SESSION_RESPONSE:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recived Create Session Response"
               " from SGW (S1 Handover)"));

         ret = vbMmeVldNStrCSRsp(pUeCb, pEgMsg, &gtpCause);
         if (ROK == ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Request to "
                  "eNB"));

            /* Call function to send HandoverRequest Msg */
            ret = vbMmeSendHOReq(pUeCb);
         }

         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward"
                  "Relocation Response with Failure cause to MME"));
            ret = vbMmeSndFRRsp(pUeCb, EGT_GTPC_CAUSE_SYS_FAIL);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward "
                     "Relocation Response with Failure cause to MME"));
            }
         }

         break;
      }

      case VB_EGTPC_MODIFY_BEARER_RESPONSE:
      {
#ifdef X2_HO
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Recived Modify Bearer Response"
               " from SGW cellId(%d) ueId(%d) pthSwtchReq(%d)\n",
                                pUeCb->eNodebCb->lpAppcellId,
                                pUeCb->ueCtxt.ueHssCtxt.ueEntry, 
                                pUeCb->pthSwtchReq));
        /* vb006.101: Added code to Send the path request ack to enodeb */
        if(pUeCb->pthSwtchReq == TRUE)
        {
           VB_MME_DBG_INFO((VB_MME_PRNTBUF, "MME: Sending path switch acknowledge"));

           ret = vbMmeSndPathSwtchAck(pUeCb);
           if(ROK != ret)
           {
              VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Not able to send the Path switch req ack"));
              break;
           }
           pUeCb->pthSwtchReq = FALSE;
         }
         else
#endif
         {
            ret = vbMmeHndlHOMBrsp(pUeCb, pEgMsg, pTransId);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Handling of Modify Bearer "
                        "Response"));
            }
         }
         break;
      }

      case VB_EGTPC_DELETE_SESSION_RESPONSE:
      {
         /* Delete Session Request is sent to if SGW is relocated 
          * during handover
          * 1. to Source SGW after a successful HO
          * 2. to a target SGW if HO is failed (Handover Failure)
          */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling Delete Session Response "\
               "received as a part of Handover Procedure"));
         ret = vbMmeHndlHODSRsp(pUeCb, pEgMsg, pTransId);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Handling Delete Session "\
                  "Response"));
         }
         
         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message Type (%d)",\
               msgType));
         break;
      }
   }

   if (RFAILED == ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Handover procedure Failed "\
            "Terminating The Procedure"));
      VB_MME_END_EMM_PROC(pUeCb);
   }
   
   RETVALUE(ret);
} /* vbMmeHndlHandoverOnSgwEvnt */

#ifdef X2_HO
/* vb006.101: Added new function vbMmeSndPathSwtchAck() */
/*
*
*       Fun:   vbMmeSndPathSwtchAck
*
*       Desc:  This function sends Path switch Ack message to
*              eNodeB on S1AP.
*
*       Desc:
*       Ret:   ROK
*
*       Notes: None
*
*       File:  vb_mme.c
*
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndPathSwtchAck
(
 VbMmeUeCb    *ueCb
 )
#else
PUBLIC S16 vbMmeSndPathSwtchAck(ueCb)
   VbMmeUeCb    *ueCb;
#endif
{
   S1apPdu *pdu;
   S16 ret;

   VB_TRC2(vbMmeSndPathSwtchAck)

      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "vbMmeSndPathSwtchAck:Sending UE Cxt ueId(%ld)",ueCb->ueCbKey));
   /* updating the spConnId after successfull modify bearer response in CNE */
   ueCb->spConnId = ueCb->pthSwthSpConnId;
   
   /* To do: Need to perform the local release for suconId 
    * at the S1AP layer. Currently if we are doing the same
    * path switch procedure is failing in the tightly coupled
    * case. To avoid this case we are not releaseing the same
    * at the S1AP layer. Also we are not freeing the existing 
    * suConId here and not generating the new suConId */
   /* free the existing suConnId */
   /*vbMmeUtlFreeConId(ueCb->eNodebCb);*/

   /* allocate a new suConnId */
   /* Allocate a new Service user connection identifier */
   vbMmeUtlAllocConId(ueCb->eNodebCb, &ueCb->suConnId);

   if(0 == ueCb->suConnId)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
             "Init UE Message: Service User Connection Id alloc failure"));
      RETVALUE(RFAILED);
   }

   /* Build path switch request Ack */
   ret = vbMmeUtlBldPathSwRqstAckg(ueCb, &pdu);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could not build the path switch req ack (%ld)"
               ,ueCb->ueCbKey));
      RETVALUE(ret);
   }

   /* Send the S1AP message */  
   RETVALUE(vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, pdu,
            Sztid_PathSwRqst));

} /* end of vbMmeSndPathSwtchAck */
#endif



/**********************************************************************
  
         End of file:     vb_mme_sgw_hdrll.c@@/main/1 - Mon Jun 28 19:35:38 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sv               1. LTE-CNE Initial Release.

**************************************************************************/
/**********************************************************************
  
         End of file:     vb_mme_sgw_hdrll.c@@/main/1 - Mon Jun 28 19:35:38 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sv               1. LTE-CNE Initial Release.

**************************************************************************/
