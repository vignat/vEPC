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
 
     Name:     Trillium LTE CNE - MME, S-GW, PDN-GW Lower Interface Module
  
     Type:     C source file
  
     Desc:     C source code for LIM Module
  
     File:     vb_mme_lim.c 
  
     Sid:      vb_mme_lim.c@@/main/1 - Mon Jun 28 19:35:39 2010
  
     Prg:      sv
  
*********************************************************************21*/

/* Header include files (.h) */
#include "envopt.h"        /* Environment options        */ 
#include "envdep.h"        /* Environment dependent      */ 
#include "envind.h"        /* Environment independent    */ 
#include "gen.h"           /* General                    */ 
#include "ssi.h"           /* System services            */ 
#include "cm5.h"           /* Common timer library       */ 
#include "cm_llist.h"      /* Common linked list library */ 
#include "cm_hash.h"       /* Common hash library        */ 
#include "cm_tpt.h"        /* Common transport library   */ 
#include "cm_tkns.h"       /* Common tokens              */ 
#include "cm_mblk.h"       /* Common memory allocation   */ 
#include "cm_inet.h"       /* Common socket library      */ 
#include "cm_pasn.h"       /* Common ASN.1 library       */ 
#include "sct.h"           /* SCT interface defines      */ 
#include "egt.h"           /* EG Upper Interface         */ 
#include "leg.h"           /* EG LM Interface            */ 
#include "szt.h"           /* S1AP Upper Interface       */ 
#include "lsz.h"           /* S1AP LM Interface          */ 
#include "szt_asn.h"       /* S1AP ASN                   */ 
#include "lvb.h"           /* CNE Layer management       */ 
#include "vb_hss.h"        /* CNE Application defines    */ 
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines    */ 
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* CNE Application defines    */ 
#include "cm_esm.h"        /* CNE Application defines    */ 
#include "cm_emm_esm.h"
#include "vbsm.h"
#include "vb_err.h"
/* header/extern include files (.x) */
#include "gen.x"           /* General layer              */ 
#include "ssi.x"           /* System services interface  */ 
#include "cm5.x"           /* Common timer module        */ 
#include "cm_lib.x"        /* Common library function    */ 
#include "cm_llist.x"      /* Common link list library   */ 
#include "cm_hash.x"       /* Common hash list library   */ 
#include "cm_tkns.x"       /* Common tokens              */ 
#include "cm_tpt.x"        /* Common transport library   */ 
#include "cm_mblk.x"       /* Common memory allocation   */ 
#include "cm_inet.x"       /* Common socket library      */ 
#include "cm_pasn.x"       /* Common ASN.1 Library       */ 
#include "sct.x"           /* SCT interface defines      */ 
#include "egt.x"           /* EG Upper Interface         */ 
#include "leg.x"           /* EG LM Interface            */ 
#include "szt_asn.x"       /* S1AP ASN                   */ 
#include "szt.x"           /* S1AP Upper Interface       */ 
#include "lsz.x"           /* S1AP LM Interface          */ 
#include "lvb.x"           /* CNE Layer management       */ 
#include "cm_esm.x"        /* CNE Application structures */ 
#include "cm_emm.x"        /* CNE Application structures */ 
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures */ 
#include "vb.x"            /* CNE Application structures */ 
#include "vbsm.x"
#include "vb_s10_hndlr.x"  /* S10 Handler Functions      */ 
#include "vb_s10_utl.x"    /* S10 Utility Functions      */ 

#include "vb_s3_hndlr.x"  /* S3 Handler Functions      */ 
#include "vb_s3_utl.x"    /* S3 Utility Functions      */ 

#ifdef __cplusplus
EXTERN "C" {
#endif

PRIVATE S16 vbMmeLimHdlIncUeConMsg ARGS ((SztConInd  *conEvnt,
                                          U8         msgType));

PRIVATE S16 vbMmeLimHdlIncUeConCfm ARGS ((SztConCfm  *conEvnt,
														U8          msgType));

PRIVATE S16 vbMmeLimHdlIncUeMsg ARGS ((SztDatEvntInd  *datEvnt,
                                       U8              msgType));

PRIVATE S16 vbMmeLimHdlUeEvnt ARGS ((VbMmeEnbCb    *enbCb,
                                     VbMmeUeCtxt   *ueCtxt,
                                     CmNasEvnt     *vbEvnt));

PRIVATE S16 vbMmeHndlEgtBndCfm ARGS((VbMmeEgtSapCb *egtSapCb,
                                     U8             status));


/*
*
*       Fun:   UzLiSztBndCfm
*       
*       Desc:  Call handler for Bind Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztBndCfm
(
Pst        *pst,
SuId        suId,
U8          status
)
#else
PUBLIC S16 UzLiSztBndCfm(pst, suId, status)
Pst        *pst;
SuId        suId;
U8          status;
#endif
{
   VB_TRC3(UzLiSztBndCfm);

   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztBndCfm(pst, suId(%d), "\
         "status(%d))", suId, status));
 
   if(suId != (vbMmeCb.sztSapCb->suId)) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB025, (ErrVal) suId,\
                      "UzLiSztBndCfm: Invalid SAP Identification From S1AP ");
      RETVALUE(RFAILED);
   }

   if(CM_BND_OK == status)
   {
      vbMmeCb.sztSapCb->state = LVB_SAP_ISBOUND;

      /* Generate an Alarm to stack manager that MME is now
         bound to S1AP */
      vbSendLmAlarm(LCM_CATEGORY_INTERFACE, LVB_EVENT_MME_S1AP_BNDCFM, 
                    LCM_CAUSE_UNKNOWN);
   }
   else
   {
      vbMmeCb.sztSapCb->state = LVB_SAP_UBND;

      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB026, ERRZERO, 
                      "Bind Failure With S1AP");
   }
  
   RETVALUE(ROK);
} /* End of UzLiSztBndCfm */

/*
*
*       Fun:   UzLiSztAudCfm
*
*       Desc:  Call handler for Aduit Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztAudCfm
(
Pst            *pst,
SuId           suId,
SztAudEvnt     *audEvnt
)
#else
PUBLIC S16 UzLiSztAudCfm(pst, suId, audEvnt)
Pst            *pst;
SuId           suId;
SztAudEvnt     *audEvnt;
#endif /* ANSI */
{
   VB_TRC3(UzLiSztAudCfm);
   
   RETVALUE(ROK);
} /* End of UzLiSztAudCfm */

/*
*
*       Fun:   UzLiSztStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztStaInd
(
Pst            *post,
SuId           suId,
SztStaInd      *sztSta
)
#else
PUBLIC S16 UzLiSztStaInd(post, suId, sztSta)
Pst            *post;
SuId           suId;
SztStaInd      *sztSta;
#endif /* ANSI */
{
   VB_TRC3(UzLiSztStaInd);

   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztStaInd(pst, suId(%d), peerId(%d), "\
         "type(%d), status(%d))", suId, sztSta->peerId.val, sztSta->type, 
         sztSta->status));

   RETVALUE(ROK);
} /* End of UzLiSztStaInd */

/*
*
*       Fun:   UzLiSztFlcInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztFlcInd
(
Pst            *post,
SuId           suId,
SztFlcInd      *flcInd
)
#else
PUBLIC S16 UzLiSztFlcInd(post, suId, flcInd)
Pst            *post;
SuId           suId;
SztFlcInd      *flcInd;
#endif /* ANSI */
{
   VB_TRC3(UzLiSztFlcInd);
 
   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztFlcInd(pst, suId(%d))", suId));

   RETVALUE(ROK);
} /* End of UzLiSztFlcInd */

/*
*
*       Fun:   UzLiSztErrInd
*
*       Desc:  Call handler for Error Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztErrInd
(
Pst               *post,
SuId              suId,
SztLclErrInd      *sztLclErrInd
)
#else
PUBLIC S16 UzLiSztErrInd(post, suId, sztLclErrInd)
Pst               *post;
SuId              suId;
SztLclErrInd      *sztLclErrInd;
#endif /* ANSI */
{
   VbMmeUeCb    *ueCb  = NULLP;
   VbMmeEnbCb   *enbCb = NULLP;
   U32          peerId = 0;
   S16          ret    = ROK;
   
   VB_TRC2(UzLiSztErrInd);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "suId(%d), sztLclErrInd->suConnId(%d), sztLclErrInd->spConnId(%d), \
            sztLclErrInd->transId(%d), sztLclErrInd->causeType (%d), sztLclErrInd->causeValue (%d)", suId, sztLclErrInd->suConnId, \
             sztLclErrInd->spConnId, sztLclErrInd->transId.val, sztLclErrInd->causeType, sztLclErrInd->causeValue));

   if(PRSNT_NODEF == sztLclErrInd->transId.pres)
   {
      VB_MME_GET_PEERID(sztLclErrInd->transId.val, peerId)
      if(ROK != vbMmeUtlFndEnbCb(&enbCb, peerId))
      {
#ifdef ALIGN_64BIT      
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Unexpected eNodeB Cb PeerId(%d)", peerId));
#else 
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Unexpected eNodeB Cb PeerId(%ld)", peerId));
#endif
          RETVALUE(ret);
      }

      ret = vbMmeUtlFndUeCbOnConnId(enbCb, &ueCb, sztLclErrInd->transId.val);

      if((ROK != ret) || (NULLP == ueCb))
      {
#ifdef ALIGN_64BIT      
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "UE Cb Not Found For UeId(%d)", sztLclErrInd->transId.val));
#else 
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "UE Cb Not Found For UeId(%ld)", sztLclErrInd->transId.val));
#endif
         RETVALUE(RFAILED);
      }

      /* Inform MME Controller */
#ifndef SZ      
      ret = vbMmeS1apErrInd(ueCb, sztLclErrInd);
#else
      ueCb->errIndCauseType = sztLclErrInd->causeType;
      vbMmeStartTmr((PTR) ueCb, VB_TMR_INT_MGMT, 1);
#endif
   }
 
   RETVALUE(ret);
} /* End of UzLiSztErrInd */

/*
*
*       Fun:   UzLiSztUDatInd
*
*       Desc:  Call handler for Connection-less data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztUDatInd
(
Pst            *pst,
SuId           suId,
SztUDatEvnt    *uDatEvnt
)
#else
PUBLIC S16 UzLiSztUDatInd(pst, suId, uDatEvnt)
Pst            *pst;
SuId           suId;
SztUDatEvnt    *uDatEvnt;
#endif /* ANSI */
{
   S16  ret;
   VB_TRC3(UzLiSztUDatInd);

   ret = RFAILED;
   
   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztUDatInd(pst, suId(%d), "\
         "procCode(%u))", suId, 
          uDatEvnt->pdu->pdu.val.initiatingMsg.procedureCode.val));
 
   switch(uDatEvnt->pdu->pdu.choice.val)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         switch(uDatEvnt->pdu->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Sztid_S1Setup: /* Incoming S1 Setup */
            {
               ret = vbMmeHdlIncSetup(uDatEvnt);
               break;
            }
            case Sztid_Reset: /* Incoming S1 Reset */
            {
               ret = vbMmeHdlIncReset(uDatEvnt);
               break;
            }
            default:
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Procedure Code: %lu",
                               uDatEvnt->pdu->pdu.val.initiatingMsg.procedureCode.val));

            }
         }/*End of Initiating Message */
         break;
      }/* End of PDU Choice */
      case S1AP_PDU_SUCCESSFULOUTCOME:
      {
         switch(uDatEvnt->pdu->pdu.val.successfulOutcome.procedureCode.val)
         {
            case Sztid_Reset: /* S1 Reset Ack */
            {
               ret = vbMmeHdlIncResetAck(uDatEvnt);
               break;
            }
            default:
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unhandled Procedure Code: %lu",
                               uDatEvnt->pdu->pdu.val.successfulOutcome.procedureCode.val));
            }
         }/*End of SuccessfulOutCome Message */
         break;
      }/* End of PDU Choice */
      default:
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB028, ERRZERO, 
               "Invalid Pdu Type");
         break;
      } 
   } 

   /* This message is not an EMM/ESM Message for any UE. */
   VB_MME_FREE_S1AP_PDU(uDatEvnt->pdu);

   RETVALUE(ret);
} /* End of UzLiSztUDatInd */

/*
*
*       Fun:   UzLiSztConInd
*
*       Desc:  Call handler for Connection Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztConInd
(
Pst            *pst,
SuId            suId,
SztConInd      *conInd
)
#else
PUBLIC S16 UzLiSztConInd(pst, suId, conInd)
Pst            *pst;
SuId            suId;
SztConInd      *conInd;
#endif /* ANSI */
{
   S16     ret;

   VB_TRC3(UzLiSztConInd);
   ret = RFAILED;
   
   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztConInd(pst, suId(%d))", suId));
   if(suId != (vbMmeCb.sztSapCb->suId)) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB029, (ErrVal) suId,\
                      "UzLiSztConInd: Invalid SAP Identification From S1AP ");

      VB_MME_FREE_S1AP_PDU(conInd->pdu);
      RETVALUE(RFAILED);
   }

   switch(conInd->pdu->pdu.choice.val)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         switch(conInd->pdu->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Sztid_initialUEMsg:
            {
               ret = vbMmeLimHdlIncUeConMsg(conInd, Sztid_initialUEMsg);
               break;
            }
#ifdef X2_HO
            case Sztid_PathSwRqst:
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_PathSwRqst"));
               ret = vbMmeLimHdlIncUeConMsg(conInd, Sztid_PathSwRqst);
               break;
            }
#endif
            default:
            {
               VB_MME_LOGERROR(ERRCLS_DEBUG, EVB030, ERRZERO, 
                     "Invalid Procedure Code");
               break;
            }
         }
         break;
      }

      default:
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB031, ERRZERO, 
            "Invalid / Unrecognized Pdu Type");
         break; 
      }
   } 

   /* Once any UE related message EMM/ESM is
      delegated up, to handle through EMM/ESM Module upto the
      controller we shouldnt use/free it again. 
      Respective delegated module or controller shall be responsible
      to free it, associating the S1AP PDU with MME's event structure.
      But we try to free the S1AP PDU here to handle the cases of not
      freeing the PDU if not delegated through the upper modules
      in all error legs. NULLP check is within macro. */
   VB_MME_FREE_S1AP_PDU(conInd->pdu);

   RETVALUE(ret);
} /* End of UzLiSztConInd */

/*
*
*       Fun:   UzLiSztConCfm
*
*       Desc:  Call handler for Connection Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztConCfm
(
Pst            *pst,
SuId            suId,
SztConCfm      *conCfm
)
#else
PUBLIC S16 UzLiSztConCfm(pst, suId, conCfm)
Pst            *pst;
SuId            suId;
SztConCfm      *conCfm;
#endif /* ANSI */
{
   S16         ret = ROK;

   VB_TRC3(UzLiSztConCfm);
   
   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztConCfm(pst, suId(%d))", suId));
   if(suId != (vbMmeCb.sztSapCb->suId)) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB032, (ErrVal) suId,\
                      "Invalid SAP Identification From S1AP");
      VB_MME_FREE_S1AP_PDU(conCfm->pdu);
      RETVALUE(RFAILED);
   }

   switch(conCfm->pdu->pdu.choice.val)
   {
      case S1AP_PDU_SUCCESSFULOUTCOME:
      {
         switch(conCfm->pdu->pdu.val.successfulOutcome.procedureCode.val)
         {
            case Sztid_HovrResourceAllocn:
            {
               ret = vbMmeLimHdlIncUeConCfm(conCfm, Sztid_HovrResourceAllocn);
               break;
            }

            default:
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Procedure Code"));
               break;
            }
         }

         break;
      }

      case S1AP_PDU_UNSUCCESSFULOUTCOME:
      {
         switch(conCfm->pdu->pdu.val.unsuccessfulOutcome.procedureCode.val)
         {
            case Sztid_HovrResourceAllocn:
            {
               ret = vbMmeLimHdlIncUeConCfm(conCfm, Sztid_HovrResourceAllocn);
               break;
            }

            default:
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Procedure Code"));
               break;
            }
         }

         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid/Unknown Pdu Type"));
         break; 
      }
   } 

   VB_MME_FREE_S1AP_PDU(conCfm->pdu);
   RETVALUE(ret);
} /* End of UzLiSztConCfm */

/*
*
*       Fun:   UzLiSztRelInd
*
*       Desc:  Call handler for Release Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztRelInd
(
Pst            *pst,
SuId           suId,
SztRelInd      *relInd
)
#else
PUBLIC S16 UzLiSztRelInd(pst, suId, relInd)
Pst            *pst;
SuId           suId;
SztRelInd      *relInd;
#endif /* ANSI */
{
   S16                ret = ROK;

   VB_TRC3(UzLiSztRelInd);

   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztRelInd(pst, suId(%d))", suId));
   if(suId != (vbMmeCb.sztSapCb->suId)) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB033, (ErrVal) suId,\
            "Invalid SAP Identification From S1AP ");

      VB_MME_FREE_S1AP_PDU(relInd->pdu);
      RETVALUE(RFAILED);
   }

   if(0 == relInd->suConnId.val)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Connection Id"));
      VB_MME_FREE_S1AP_PDU(relInd->pdu);
      RETVALUE(RFAILED);
   }

   switch (relInd->pdu->pdu.choice.val)
   {
      case S1AP_PDU_UNSUCCESSFULOUTCOME:
      {
         switch (relInd->pdu->pdu.val.unsuccessfulOutcome.procedureCode.val)
         {
            case Sztid_HovrResourceAllocn:
            {
               ret = vbMmeRcvS1apRelInd(relInd->suConnId.val, (Ptr*)relInd,\
                     Sztid_HovrResourceAllocn);
               break;
            }

            default:
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Procedure Code"));
               break;
            }
         }

         break;
      }

      default:
      {
         ret = vbMmeRcvS1apMsg(relInd->suConnId.val, (Ptr*)relInd,\
               Sztid_UECntxtRlsRqst);
         break;
      }
   }

   VB_MME_FREE_S1AP_PDU(relInd->pdu);

   RETVALUE(ret);
} /* End of UzLiSztRelInd */

/*
*
*       Fun:   UzLiSztRelCfm
*
*       Desc:  Call handler for Release Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztRelCfm
(
Pst          *pst,
SuId          suId,
SztRelCfm    *relCfm
)
#else
PUBLIC S16 UzLiSztRelCfm(pst, suId, relCfm)
Pst          *pst;
SuId          suId;
SztRelCfm    *relCfm;
#endif /* ANSI */
{
   S16                ret;

   VB_TRC3(UzLiSztRelCfm);

   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztRelCfm(pst, suId(%d))", suId));

   if(suId != (vbMmeCb.sztSapCb->suId)) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB034, (ErrVal) suId,\
            "Invalid SAP Identification From S1AP");

      VB_MME_FREE_S1AP_PDU(relCfm->pdu);
      RETVALUE(RFAILED);
   }


   if(0 == relCfm->suConnId)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid Connection Id"));
      VB_MME_FREE_S1AP_PDU(relCfm->pdu);
      RETVALUE(RFAILED);
   }

   /* Inform the MME Controller */
   ret = vbMmeRcvS1apMsg(relCfm->suConnId, (Ptr*)relCfm, 
         Sztid_UECntxtRls);

   VB_MME_FREE_S1AP_PDU(relCfm->pdu);

   RETVALUE(ret);
} /* End of UzLiSztRelCfm */

/*
*
*       Fun:   UzLiSztDatInd
*
*       Desc:  Call handler for Connection based data indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztDatInd
(
Pst            *pst,
SuId           suId,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 UzLiSztDatInd(pst, suId, datEvnt)
Pst            *pst;
SuId           suId;
SztDatEvntInd  *datEvnt;
#endif /* ANSI */
{
   S16             ret = RFAILED;

   VB_TRC3(UzLiSztDatInd);

   if(suId != (vbMmeCb.sztSapCb->suId)) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB035, (ErrVal) suId,\
            "Invalid SAP Identification From S1AP");

      VB_MME_FREE_S1AP_PDU(datEvnt->pdu);
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztDatInd(pst, suId(%d))", suId));
  
    switch(datEvnt->pdu->pdu.choice.val)
    {
        case S1AP_PDU_INITIATINGMSG:
        {
            switch(datEvnt->pdu->pdu.val.initiatingMsg.procedureCode.val)
            {
                case Sztid_uplinkNASTport: /* Received Uplink NAS Transport */
                {
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_uplinkNASTport"));
                    ret = vbMmeLimHdlIncUeMsg(datEvnt, Sztid_uplinkNASTport); 
                    break;
                }
                /* vb001.101: Handled UE context release request */
                case Sztid_UECntxtRlsRqst:
                {
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_UECntxtRlsRqst suConnId(%d)", datEvnt->suConnId));
                    ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt, 
                                              Sztid_UECntxtRlsRqst);
                    break;
                }
                /* vb003.101: Added UE Capblty Info Hdlings */
                case Sztid_UECapbltyInfoInd:
                {
#ifdef UE_RAD_CAP
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_UECapbltyInfoInd"));
                    ret = vbMmeLimHdlIncUeMsg(datEvnt, Sztid_UECapbltyInfoInd); 
#else
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_UECapbltyInfoInd, But UE_RAD_CAP flag is disabled so droping the message"));
                    /* Drop the message here */
#endif
                    break;
                }

               /* Received NAS Non-Delivery Indication */
               case Sztid_NASNonDlvryInd:
               {
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_NASNonDlvryInd"));
                    ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt, 
                                              Sztid_NASNonDlvryInd); 
                    break;
               }
               case Sztid_E_RABRlsInd: /* Received E-RAB Release Ind */
               {
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_E_RABRlsInd"));
                   ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt, 
                                             Sztid_E_RABRlsInd); 
                   break;
               }
               case Sztid_ErrInd: /* Received Error Indication */
               {
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_ErrInd"));
                   ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt, 
                                             Sztid_ErrInd); 
                   break;
               }
               case Sztid_HovrPrep:
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_HovrPrep"));
                  ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt,\
                        Sztid_HovrPrep); 
                  break;
               }

               case Sztid_HovrNotification:
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_HovrNotification"));
                  ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt,\
                        Sztid_HovrNotification); 
                  break;
               }
               case Sztid_eNBStatusTfr:
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_eNBStatusTfr"));
                  ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt,\
                        Sztid_eNBStatusTfr); 
                  break;
               }

               case Sztid_HovrCancel:
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_HovrCancel"));
                  ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt,\
                        Sztid_HovrCancel); 
                  break;
               }
               default:
               {
                   VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                      "Invalid Procedure Code With Initiating Message Procedure_code(%d)", \
                      datEvnt->pdu->pdu.val.initiatingMsg.procedureCode.val));
                   break;
               }
            }
            break;
        }

        case S1AP_PDU_SUCCESSFULOUTCOME:
        {
            switch(datEvnt->pdu->pdu.val.successfulOutcome.procedureCode.val)
            {
                case Sztid_InitCntxtSetup:
                {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received S1AP_PDU_SUCCESSFULOUTCOME:Sztid_InitCntxtSetup"));
                   /* Received Initial Context Setup Response */
                   ret = vbMmeLimHdlIncUeMsg(datEvnt, Sztid_InitCntxtSetup);
                   break;
                }
                case  Sztid_E_RABSetup:
                {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_E_RABSetup"));
                     /* Received E-RAB Setup Response */
                    ret = vbMmeLimHdlIncUeMsg(datEvnt,  Sztid_E_RABSetup);
                    break;
                }
                case  Sztid_E_RABRls: /* Received E-RAB Release Response */
                {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_E_RABRls"));
                    ret = vbMmeLimHdlIncUeMsg(datEvnt,  Sztid_E_RABRls);
                    break;
                }
                case  Sztid_E_RABMdfy: /* Received E-RAB Modify Response */
                {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_E_RABMdfy"));
                    ret = vbMmeLimHdlIncUeMsg(datEvnt,  Sztid_E_RABMdfy);
                    break;
                }
                case Sztid_UECntxtModification:
                {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_UECntxtModification"));
                     ret = vbMmeLimHdlIncUeMsg(datEvnt,\
                           Sztid_UECntxtModification);
                    break;
                }
                case Sztid_HovrResourceAllocn:
                {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_HovrResourceAllocn"));
                   /* Handover resource allocation,
                    * Handover Request Acknowledge
                    * Handover Failure
                    */
                   ret = vbMmeLimHdlIncUeMsg(datEvnt, Sztid_HovrResourceAllocn);
                   break;
                }
                default:
                { 
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                      "Invalid Procedure Code With Successful Outcome Procedure_code(%d)", \
                      datEvnt->pdu->pdu.val.successfulOutcome.procedureCode.val));
                    break; 
                }
            }
            break;
        }
        case S1AP_PDU_UNSUCCESSFULOUTCOME:
        {
            switch(datEvnt->pdu->pdu.val.unsuccessfulOutcome.procedureCode.val)
            {
               case Sztid_InitCntxtSetup: /* Received Initial Context Setup
                                         Failure */
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received S1AP_PDU_UNSUCCESSFULOUTCOME:Sztid_InitCntxtSetup"));
                   ret = vbMmeLimHdlIncUeMsg(datEvnt, Sztid_InitCntxtSetup);
                   break;
               }
               case Sztid_UECntxtModification:
               {
                  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Received Sztid_UECntxtModification"));
                   ret = vbMmeLimHdlIncUeMsg(datEvnt,\
                        Sztid_UECntxtModification);
                   break;
               }
               default:
               { 
                   VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                      "Invalid Procedure Code With Successful Outcome Procedure_code(%d)", datEvnt->pdu->pdu.val.unsuccessfulOutcome.procedureCode.val));
                   break; 
               }
            }
            break;
        }

        default:
        {
            VB_MME_LOGERROR(ERRCLS_DEBUG, EVB038, ERRZERO, 
               "Invalid / Unhandled Pdu Type");
            break;
        }
    } 

    /* Once any UE related message EMM/ESM is
       delegated up, to handle through EMM/ESM Module upto the
       controller we shouldnt use/free it again. 
       Respective delegated module or controller shall be responsible
       to free it, associating the S1AP PDU with MME's event structure.
       But we try to free the S1AP PDU here to handle the cases of not
       freeing the PDU if not delegated through the upper modules
       in all error legs. NULLP check is within macro. */

    VB_MME_FREE_S1AP_PDU(datEvnt->pdu);

    RETVALUE(ret);
} /* End of UzLiSztDatInd */

/*
*
*       Fun:   UzLiSztEncDecCfm
*
*       Desc:  Call handler for Encode/Decode Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztEncDecCfm
(
Pst            *pst,
SuId           suId,
SztEncDecEvnt  *sztEncDecEvnt,
SztReqStatus   *status
)
#else
PUBLIC S16 UzLiSztEncDecCfm(pst, suId, sztEncDecEvnt,status)
Pst            *pst;
SuId           suId;
SztEncDecEvnt  *sztEncDecEvnt;
SztReqStatus   *status;
#endif /* ANSI */
{
   VB_TRC3(UzLiSztEncDecCfm);
   
   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztEncDecCfm(pst, suId(%d))", suId));
 
   if(status->status == SZT_PRIM_OK)
   {
      if (sztEncDecEvnt->type.pres == PRSNT_NODEF)
      {
         if (sztEncDecEvnt->type.val == SZT_REQ_TYPE_DECODE)
         {
            VB_MME_FREE_S1AP_PDU(sztEncDecEvnt->u.pdu);
         }
         else if (sztEncDecEvnt->type.val == SZT_REQ_TYPE_ENCODE)
         {
            VB_FREE_BUF(sztEncDecEvnt->u.mBuf);
         }
      }
   }

   RETVALUE(ROK);
} /* End of UzLiSztEncDecCfm */

/*
*
*       Fun:   UzLiSztPeerAssocInd
*
*       Desc:  Call handler for Peer Assoc Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_lim.c
*       
*/
#ifdef ANSI
PUBLIC S16 UzLiSztPeerAssocInd
(
Pst              *pst,
SuId             suId,
SztPeerAssocInd  *peerAssocInd
)
#else
PUBLIC S16 UzLiSztPeerAssocInd(pst, suId, peerAssocInd)
Pst              *pst;
SuId             suId;
SztPeerAssocInd  *peerAssocInd;
#endif /* ANSI */
{
   TRC2(UzLiSztPeerAssocInd);
   
   VB_MME_DBG_PRNT((VB_MME_PRNTBUF, "UzLiSztPeerAssocInd(pst, suId(%d))",suId));

   RETVALUE(ROK);
} /* End of UzLiSztPeerAssocInd */



/* This is a common wrapper to send the message to S1AP */

#ifdef ANSI
PUBLIC S16 vbMmeLimSndS1APMsg
(
VbMmeEnbCb     *eNbCb,
VbMmeUeCb      *ueCb,
S1apPdu        *spdu,
U8             msgType
)
#else
PUBLIC S16 vbMmeLimSndS1APMsg(eNbCb, ueCb, spdu, msgType)
VbMmeEnbCb     *eNbCb;
VbMmeUeCb      *ueCb;
S1apPdu        *spdu;
U8             msgType;
#endif
{
   SztUDatEvnt    s1SetupRsp;
   SztUDatEvnt    s1Reset;
   SztUDatEvnt    paging;
   SztConRsp      conRsp;
   SztConReq      conReq;
   SztDatEvntReq  datReq;
   SztRelReq      relReq;
   S16            ret = ROK;

   VB_TRC2(vbMmeLimSndS1APMsg);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "MsgType [%d]", msgType));
   switch(msgType)
   {
      case Sztid_S1Setup:
      {
         cmMemset((U8*)&s1SetupRsp, 0, sizeof(SztUDatEvnt));

         s1SetupRsp.peerId.pres = PRSNT_NODEF;
         s1SetupRsp.peerId.val = eNbCb->peerId;
         s1SetupRsp.pdu = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztUDatReq to S1AP Layer msgType=Sztid_S1Setup"));
         ret = VbLiSztUDatReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                              &s1SetupRsp);
         break;
      }
      case Sztid_Reset:
      {
         s1Reset.peerId.pres = PRSNT_NODEF;
         s1Reset.peerId.val  = eNbCb->peerId;
         s1Reset.pdu         = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztUDatReq to S1AP Layer msgType=Sztid_Reset"));
         ret = VbLiSztUDatReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                              &s1Reset);
         break;
      }
      case Sztid_HovrResourceAllocn:
      {
         conReq.peerId.pres = PRSNT_NODEF;
         conReq.peerId.val  = eNbCb->peerId;
         conReq.u.suConnId  = ueCb->suConnId;
         conReq.pdu         = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztUDatReq to S1AP Layer msgType=Sztid_HovrResourceAllocn"));
         ret = VbLiSztConReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId,\
                     &conReq);
         ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;
         break;
      }
      case Sztid_eNBStatusTfr:
      {
         datReq.spConnId = ueCb->spConnId;
         datReq.pdu      = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztDatReq to S1AP Layer"));
         ret = VbLiSztDatReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
               &datReq);

         break;
      }
      case Sztid_HovrPrep:
      case Sztid_HovrCancel:
      {
         datReq.spConnId = ueCb->spConnId;
         datReq.pdu      = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztDatReq to S1AP "\
               "Layer"));
         ret = VbLiSztDatReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
               &datReq);
         break;
      }
      case Sztid_InitCntxtSetup:
      case Sztid_downlinkNASTport:
      case Sztid_E_RABSetup:
      case Sztid_E_RABRls:
      case Sztid_E_RABMdfy:
      case Sztid_UECntxtModification:
      {
         if (ueCb->ueCtxt.s1apConState == VB_EMM_UE_S1AP_INIT)
         {
            conRsp.suConnId           = ueCb->suConnId;
            conRsp.spConnId           = ueCb->spConnId;
            conRsp.pdu                = spdu;
            ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_CONNECTED;

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztConRsp to S1AP "\
                  "Layer suConId %d SpConId %d\n",conRsp.suConnId,conRsp.spConnId));
            ret = VbLiSztConRsp(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                  &conRsp);
         }
         else
         {
            datReq.spConnId = ueCb->spConnId;
            datReq.pdu      = spdu;

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztDatReq to S1AP "\
                  "Layer"));
            ret = VbLiSztDatReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                  &datReq);
         }
         break;
      }
      case Sztid_UECntxtRls:
      {
         relReq.connIdType = SZT_CONN_TYPE_SP;
         relReq.relType    = SZT_CXT_REL;
         relReq.connId     = ueCb->spConnId;
         relReq.pdu        = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztRelReq to S1AP Layer"));
         ret = VbLiSztRelReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                              &relReq);
         break;  
      }
      case Sztid_Pag:
      {
         paging.peerId.pres = PRSNT_NODEF;
         paging.peerId.val  = eNbCb->peerId;
         paging.pdu         = spdu;

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending SztUDatReq to S1AP Layer"));
         ret = VbLiSztUDatReq(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                              &paging);
         break;
      }
#ifdef X2_HO
       /* vb006.101: Added new Switch case Sztid_PathSwRqst */
      case Sztid_PathSwRqst:     
      {
         cmMemset((U8*)&conRsp, 0, sizeof(SztConRsp));

         conRsp.suConnId = ueCb->suConnId;
         conRsp.spConnId = ueCb->spConnId;
         conRsp.pdu = spdu;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Sztid_PathSwRqst to S1AP Layer suConnId(%d) spConnId(%d)\n",
                                                            ueCb->suConnId,
                                                            ueCb->spConnId));
         ret = VbLiSztConRsp(&(eNbCb->sztSapCb->pst), eNbCb->sztSapCb->spId, 
                              &conRsp);
         ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_CONNECTED;
      }
      break;
#endif
      default:
      {
         break;
      } 
   }

   RETVALUE(ret);

} /* vbMmeLimSndS1APMsg */


/*
*
*       Fun:   vbMmeEdmEncode
*       
*       Desc:  Entry function for EMM/ESM encoder
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeEdmEncode
(
CmNasEvnt    *mmeEvnt,
SztNAS_PDU   *nasPdu
)
#else
PUBLIC S16 vbMmeEdmEncode(mmeEvnt, nasPdu)
CmNasEvnt    *mmeEvnt;
SztNAS_PDU   *nasPdu;
#endif
{

   VB_TRC2(vbMmeEdmEncode);

   if (NULLP == mmeEvnt)
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB039, (ErrVal)0,
                      "Received Invalid Event");
      RETVALUE(RFAILED);
   }

   /* Check the protocol discriminator and call respective encoder */
   switch (mmeEvnt->protDisc)
   {
      case CM_EMM_PD:
      {
         RETVALUE(cmEmmEncMsg(mmeEvnt,
                            &nasPdu->val,
                            &nasPdu->len));
      }
      case CM_ESM_PD:
      {
         RETVALUE(cmEsmEncMsg(mmeEvnt,
                            &nasPdu->val,
                            &nasPdu->len));
      }
      default:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Protocol Discriminator(%d) Not "
                      "Supported In NAS Pdu", mmeEvnt->protDisc));
         RETVALUE(RFAILED);
      }
   }

} /* vbMmeEdmEncode */

#ifdef ANSI
PUBLIC S16 vbMmeLimSndToLower
(
VbMmeUeCb     *ueCb,
CmNasEvnt     *vbEvnt,
SztNAS_PDU    *nasPdu,
U8             msgType,
Bool           nasPduPres /* vb001.101: Added new argument */
)
#else
PUBLIC S16 vbMmeLimSndToLower(ueCb, vbEvnt, nasPdu, msgType, nasPduPres)
VbMmeUeCb     *ueCb;
CmNasEvnt     *vbEvnt;
SztNAS_PDU    *nasPdu;
U8             msgType;
Bool           nasPduPres
#endif
{
   S1apPdu     *s1apPdu = NULLP;
   S16          ret;

#ifdef VB_MME_NAS_SEC
   VbMmeMsg     srcMsg;
   VbMmeMsg     dstMsg;
#endif

   VB_TRC2(vbMmeLimSndToLower)

   /* Encode the PDU */
   if (nasPduPres == TRUE)
   {
      ret = vbMmeEdmEncode(vbEvnt, nasPdu);
      if (ret != ROK)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Encoding Failed(%d)", ret));

         /* Free the event structure what we have */
         vbEvnt->pdu = NULLP;
         CM_FREE_NASEVNT(&vbEvnt);

         RETVALUE(ret);
      }
#ifdef VB_MME_NAS_SEC
      /*Keep the header type for retransmission*/
      ueCb->lstMsgHdrType = vbEvnt->secHT;

      if (vbEvnt->secHT != CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG)
      {
         srcMsg.val = nasPdu->val;
         srcMsg.len = nasPdu->len;

         ret = vbMmeCompDLSec(&ueCb->ueCtxt.secCtxt, vbEvnt->secHT,\
               &srcMsg, &dstMsg);         
         if(ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Downlink Security Failed"));
            vbEvnt->pdu = NULLP;
            CM_FREE_NASEVNT(&vbEvnt);
            RETVALUE(ret);
         }
         nasPdu->val = dstMsg.val;
         nasPdu->len = dstMsg.len;
      }  
#endif
    } /* end of if statement */

   /* Build the associated S1AP PDU */
   switch(msgType)
   {
      case Sztid_downlinkNASTport:
      {
         /* Invoke Encode function here */
         ret = vbMmeUtlBldDlNasTransport (ueCb, nasPdu, &s1apPdu);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                   "S1AP Pdu Couldn't Be Built MsgType(%d) ",msgType));
            break;
         }

         /* Finally delegate the message to send to S1AP */
         ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
         break;
      }
      case Sztid_InitCntxtSetup:
      {
         /* Invoke Encode function here */
         ret = vbMmeUtlBldInitCxtSetupReq(ueCb, nasPdu, &s1apPdu);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built "\
                  "MsgType(%d) ",msgType));
            break;
         }
         /* Finally delegate the message to send to S1AP */
         ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Error In Sending Init "\
                  "Ctxt Setup"));
            break;
         }
          
         break;
      }
      /* vb001.101: Handled ERAB Setup message */
      case Sztid_E_RABSetup :
      {
         VbMmeEsmCb   *esmCb;
         esmCb = ueCb->esmList[vbEvnt->m.esmEvnt->bearerId - \
               CM_ESM_BEARER_ID_INDX]; 
         /*new case added */
         /* Invoke Encode function here */
         ret = vbMmeUtlBldERABSetReq(esmCb, nasPdu, &s1apPdu);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built " \
                  "MsgType(%d) ", msgType));
            break;
         }
         /* Finally delegate the message to send to S1AP */
         ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending E-RAB Set Up With Act "\
               "Ded Bearer"));


         break;
      }
      case Sztid_E_RABMdfy:
      {
         VbMmeEsmCb   *esmCb;
         esmCb = ueCb->esmList[vbEvnt->m.esmEvnt->bearerId - \
               CM_ESM_BEARER_ID_INDX]; 

         /*new case added */
         /* Invoke Encode function here */
         ret = vbMmeUtlBldERABModReq(esmCb, nasPdu, &s1apPdu);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built "\
                  "MsgType(%d) ", msgType));
            break;
         }
         /* Finally delegate the message to send to S1AP */
         ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending E-RAB Modify Req with "\
               "ModifyBearerCtxtReq"));
         break;
      }

      case Sztid_E_RABRls:
      {
         VbMmeEsmCb   *esmCb;
         esmCb = ueCb->esmList[vbEvnt->m.esmEvnt->bearerId - CM_ESM_BEARER_ID_INDX]; 
         /*new case added */
         /* Invoke Encode function here */
         ret = vbMmeUtlBldERABRlsCmd(esmCb, nasPdu, &s1apPdu);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built "\
                  "MsgType(%d) ", msgType));
            break;
         }
         /* Finally delegate the message to send to S1AP */
         ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending E-RAB Release Cmd With "\
               "DeAct Ded Bearer Req"));

         break;
      }
      case Sztid_UECntxtModification:
      {
         /* Invoke Encode function here */
         ret = vbMmeUtlBldUeCxtModReq(ueCb, nasPdu, &s1apPdu);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built "\
                  "MsgType(%d) ",msgType));
            break;
         }
         /* Finally delegate the message to send to S1AP */
         ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Error In Sending Ue Ctxt "\
                  "Modification msg"));
            break;
         }
        break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid S1AP Pdu MsgType(%d)",
                           msgType));

         ret = RFAILED;
         break; 
      }
   }
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built MsgType(%d)",
               msgType));

      /* Free the encoded pdu since building / sending this message
         itself wasnt success */
   }

   /* Free the event structure what we have */
   /* This essentially shouldnt contain S1AP PDU, as we encode the 
      EMM-ESM message only. S1AP Pdu is built locally when trigger from
      upper interface only. however making it NULLP */
   if (nasPduPres == TRUE)
   {
      vbEvnt->pdu = NULLP;
      CM_FREE_NASEVNT(&vbEvnt);
#ifdef VB_MME_NAS_SEC
      if(ueCb->lstMsgHdrType != CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG)
      {
         nasPdu->val = srcMsg.val;
         nasPdu->len = srcMsg.len;
      }
#endif
   }

   RETVALUE(ret);
} /* vbMmeLimSndToLower */

#ifdef ANSI
PUBLIC S16 vbMmeLimSndStoredBuf
(
VbMmeUeCb     *ueCb,
U8             msgType
)
#else
PUBLIC S16 vbMmeLimSndStoredBuf(ueCb, msgType)
VbMmeUeCb     *ueCb;
U8             msgType;
#endif
{
   S1apPdu *s1apPdu = NULLP;
   S16      ret = RFAILED;
#ifdef VB_MME_NAS_SEC
   VbMmeMsg     srcMsg;
   VbMmeMsg     dstMsg;
#endif

   VB_TRC2(vbMmeLimSndStoredBuf)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"MsgType(%d)", 
                   msgType));

#ifdef VB_MME_NAS_SEC
   if(ueCb->lstMsgHdrType != CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG)
   {
      srcMsg.val = ueCb->emmCb.retrBuf.val;
      srcMsg.len = ueCb->emmCb.retrBuf.len;

      ret = vbMmeCompDLSec(&ueCb->ueCtxt.secCtxt, ueCb->lstMsgHdrType,\
            &srcMsg, &dstMsg);         
      if(ROK != ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Downlink Security Failed"));
         RETVALUE(ret);
      }
      ueCb->emmCb.retrBuf.val = dstMsg.val;
      ueCb->emmCb.retrBuf.len = dstMsg.len;
   }  
#endif

   /* Send the appropriate S1AP Buffer we stored */
   switch(msgType)
   {
      case Sztid_downlinkNASTport:
      {
         /* Invoke Encode function here */
         ret = vbMmeUtlBldDlNasTransport (ueCb, &(ueCb->emmCb.retrBuf), 
                                          &s1apPdu);
         if( s1apPdu == NULLP )
         {
            ret = RFAILED;
         }
         /* Do not Free the NAS PDU as we may need it for further retxs */
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,\
               "Invalid S1AP Pdu To Build MsgType(%d)", msgType));
      }
   }
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built MsgType(%d)",
                                       msgType));
   }
   else
   {
      /* Finally delegate the message to send to S1AP */
      ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
   }

#ifdef VB_MME_NAS_SEC
   if(ueCb->lstMsgHdrType != CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG)
   {
      ueCb->emmCb.retrBuf.val = srcMsg.val;
      ueCb->emmCb.retrBuf.len = srcMsg.len;
   }
#endif

   RETVALUE(ret);
} /* vbMmeLimSndStoredBuf */

#ifdef ANSI
PUBLIC S16 vbMmeLimSndEsmStoredBuf
(
VbMmeEsmCb     *esmCb,
U8             msgType
)
#else
PUBLIC S16 vbMmeLimSndEsmStoredBuf(esmCb, msgType)
VbMmeUeCb     *esmCb;
U8             msgType;
#endif
{
   S1apPdu *s1apPdu = NULLP;
   VbMmeUeCb  *ueCb = NULLP;
   S16      ret = RFAILED;
#ifdef VB_MME_NAS_SEC
   VbMmeMsg     srcMsg;
   VbMmeMsg     dstMsg;
#endif

   VB_TRC2(vbMmeLimSndEsmStoredBuf)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"MsgType(%d)", msgType));
#ifdef VB_MME_NAS_SEC
   if(esmCb->ueCb->lstMsgHdrType != CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG)
   {
      srcMsg.val = esmCb->retrBuf.val;
      srcMsg.len = esmCb->retrBuf.len;

      ret = vbMmeCompDLSec(&esmCb->ueCb->ueCtxt.secCtxt,\
            esmCb->ueCb->lstMsgHdrType, &srcMsg, &dstMsg);         
      if(ROK != ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Downlink Security Failed"));
         RETVALUE(ret);
      }
      esmCb->retrBuf.val = dstMsg.val;
      esmCb->retrBuf.len = dstMsg.len;
   }  
#endif

   /* Send the appropriate S1AP Buffer we stored */
   switch(msgType)
   {
      case Sztid_downlinkNASTport:
      {
         ueCb = esmCb->ueCb;
         /* Invoke Encode function here */
         ret = vbMmeUtlBldDlNasTransport (ueCb, &(esmCb->retrBuf), 
                                          &s1apPdu);
         /* Do not Free the NAS PDU as we may need it for further retxs */
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid S1AP Pdu To Build MsgType(%d)",
                                          msgType));
      }
   }
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"S1AP Pdu Couldn't Be Built MsgType(%d)",
                                       msgType));
   }
   else
   {
		/* NULL check has been introduced to avoid warnings */
		if (ueCb != NULLP)
		{
			/* Finally delegate the message to send to S1AP */
			ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, s1apPdu, msgType);
		}
   }

#ifdef VB_MME_NAS_SEC
   if(esmCb->ueCb->lstMsgHdrType != CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG)
   {
      esmCb->retrBuf.val = srcMsg.val;
      esmCb->retrBuf.len = srcMsg.len;
   }
#endif

   RETVALUE(ret);
} /* vbMmeLimSndEsmStoredBuf */

/*
*
*       Fun:   vbMmeEdmDecode
*       
*       Desc:  Entry function for EMM/ESM decoder
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeEdmDecode
(
SztNAS_PDU   *nasPdu,
CmNasEvnt    **mmeEvnt
)
#else
PUBLIC S16 vbMmeEdmDecode(nasPdu, mmeEvnt)
SztNAS_PDU   *nasPdu;
CmNasEvnt    **mmeEvnt;
#endif
{

   VB_TRC2(vbMmeEdmDecode);

   /* check the protocol discriminator and call respective decoder */
   switch (nasPdu->val[0] & 0x0f)
   {
      case CM_EMM_PD:
      {
         RETVALUE(cmEmmDecMsg(nasPdu->val, mmeEvnt, nasPdu->len));
      }
      case CM_ESM_PD:
      {
         RETVALUE(cmEsmDecMsg(nasPdu->val, mmeEvnt, nasPdu->len));
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                  "Protocol Discriminator(%d) Not Supported In NAS Pdu",
                   nasPdu->val[0] & 0x0f));
         RETVALUE(RFAILED);
      }
   }
   /* fixed compilation warning - unreachanble code*/
   /*RETVALUE(ROK);*/
} /* vbMmeEdmDecode */

/*
*
*       Fun:   vbMmeLimHdlIncUeConMsg
*       
*       Desc:  Entry function for handling selective 
*              incoming S1AP messages for UEs with EMM-ESM interpretations
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme.c
*       
*/
#ifdef ANSI
PRIVATE S16 vbMmeLimHdlIncUeConMsg
(
SztConInd  *conEvnt,
U8          msgType
)
#else
PRIVATE S16 vbMmeLimHdlIncUeConMsg(conEvnt, msgType)
SztConInd *conEvnt;
U8         msgType;
#endif
{
   VbMmeEnbCb    *eNbCb = NULLP;
   SztNAS_PDU    *nasPdu = NULLP;
   CmNasEvnt     *ueMmeEvnt = NULLP;
   VbMmeUeCtxt    ueCtxt = {0, 0};
   S16            ret = ROK;

   VB_TRC2(vbMmeLimHdlIncUeConMsg);
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received Ue ConMsg eNodeb(%d)", conEvnt->peerId.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received Ue ConMsg eNodeb(%ld)", conEvnt->peerId.val));
#endif

   ret = vbMmeUtlFndEnbCb(&eNbCb, conEvnt->peerId.val);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb PeerId(%d)",
               conEvnt->peerId.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb PeerId(%ld)",
               conEvnt->peerId.val));
#endif
      RETVALUE(ret); 

   }
 
    switch(msgType)
    {
        case Sztid_initialUEMsg : /* Incoming Initial UE Message */
        {
            /* Retrieve the NAS PDU IE from message */
            vbMmeUtlGetS1apIE(&(conEvnt->pdu->pdu), Sztid_NAS_PDU, 
                            (TknU8 ** )&nasPdu);

            if(NULLP == nasPdu) 
            {
                VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                      "Init UE Message NAS Retrieval Failure"));
                RETVALUE(RFAILED); /* Should we send Failure back to eNB */
            }

            ret = vbMmeEdmDecode(nasPdu, &ueMmeEvnt);
				/* NULLP check has been introduced to avoid warning */
            if(ROK != ret || ueMmeEvnt == NULLP)
            {
                VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                       "Init UE Message NAS Decode Failure"));

                RETVALUE(ret); /* Should we send Failure back to eNB */
            }

#ifdef VB_MME_NAS_SEC
            if(CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG == ueMmeEvnt->secHT) 
            {
                ueMmeEvnt->secStatus = VB_MME_SEC_NOTPRTD;
            }
            else if(CM_NAS_SEC_HDR_TYPE_SER_REQ == ueMmeEvnt->secHT)
            {
                ueMmeEvnt->secStatus = VB_MME_SEC_SERVREQ;
            }
            else if(CM_NAS_SEC_HDR_TYPE_INT_PRTD == ueMmeEvnt->secHT)
            {
                ueMmeEvnt->secStatus = VB_MME_SEC_INTPRTD;
            }
            else
            {
                VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Init UE Message Is "\
                     "ENCRYPTED, Ignoring The Message"));

                ueMmeEvnt->pdu = NULLP;
                CM_FREE_NASEVNT(&ueMmeEvnt);
                RETVALUE(RFAILED);
            }
#endif
            /* Allocate a new Service user connection identifier */
            vbMmeUtlAllocConId(eNbCb, &ueCtxt.suConnId);
            if(0 == ueCtxt.suConnId)
            {
                VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                   "Init UE Message:Service User Connection Id Alloc Failure"));
                RETVALUE(ret); /* Should we send Failure back to eNB */
            } 

            /* Associate the S1AP PDU to MME's event structure.
             * Here onwards S1AP Pdu is accessed throuh MME's event
             * structure. This will help in freeing the S1AP PDU 
             * in caller too.
             */
            ueMmeEvnt->pdu = conEvnt->pdu;
            conEvnt->pdu = NULLP;

            ueCtxt.spConnId = conEvnt->u.spConnId;

            VB_MME_DBG_INFO((VB_MME_PRNTBUF," Allocated new SuConnid(%d), spConnid(%d) \n", ueCtxt.suConnId, ueCtxt.spConnId));
            /* Delegate now to a common entry function to handle
               EMM-ESM messages */
            ret = vbMmeLimHdlUeEvnt(eNbCb, &ueCtxt, ueMmeEvnt);

            break;
        }  
#ifdef X2_HO
        case Sztid_PathSwRqst:
        {
            ret = vbMmeHdlPathSwRqst(eNbCb, conEvnt);
           break;
        }
#endif
        default:
        {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                 "Invalid Message Type(%d)", msgType));
            break;
        }
    }
  
    RETVALUE(ret);
} /* End of vbMmeLimHdlIncUeConMsg */



/*
*
*       Fun:   vbMmeLimHdlIncUeMsg
*       
*       Desc:  Entry function for handling selective 
*              incoming S1AP messages for UEs with EMM-ESM interpretations
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme.c
*       
*/
#ifdef ANSI
PRIVATE S16 vbMmeLimHdlIncUeMsg
(
SztDatEvntInd  *datEvnt,
U8              msgType
)
#else
PRIVATE S16 vbMmeLimHdlIncUeMsg(datEvnt, msgType)
SztDatEvntInd  *datEvnt;
U8              msgType;
#endif
{
   VbMmeEnbCb   *eNbCb        = NULLP;
   VbMmeUeCtxt   ueCtxt       = {0, 0};
   SztNAS_PDU   *nasPdu       = NULLP;
   CmNasEvnt    *ueMmeEvnt    = NULLP;
   U32          peerId;
   S16          ret           = ROK;
#ifdef VB_MME_NAS_SEC
   SztNAS_PDU   secPdu;
   VbMmeMsg     srcMsg;
   VbMmeMsg     dstMsg;
   VbMmeUeCb    *ueCb         = NULLP;
#endif

   VB_TRC2(vbMmeLimHdlIncUeMsg);
 
   VB_MME_GET_PEERID(datEvnt->suConnId, peerId);   
#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
          "Incoming UeMsg eNodeB(%d)", peerId ));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
          "Incoming UeMsg eNodeB(%ld)", peerId ));
#endif
   
   if(ROK != vbMmeUtlFndEnbCb(&eNbCb, peerId))
   {
#ifdef ALIGN_64BIT   
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Unexpected eNodeB Cb PeerId(%d)", peerId));
#else
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Unexpected eNodeB Cb PeerId(%ld)", peerId));
#endif
       RETVALUE(ret); /* Should we send S1 Setup Failure back to eNB ? */
   }

   switch(msgType)
   {
      case Sztid_uplinkNASTport : /* Uplink NAS Transport */
      {
         /* Retrieve the NAS PDU IE from message */
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_NAS_PDU, 
                            (TknU8 ** )&nasPdu);
         if(NULLP == nasPdu) 
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                      "Init UE Message NAS Retrieval Failure"));
            RETVALUE(RFAILED); /* Should we send Failure back to eNB */
         }
   
         ret = vbMmeEdmDecode(nasPdu, &ueMmeEvnt);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                      "Uplink NAS Transport Decode Failure"));
            RETVALUE(ret); /* Should we send Failure back to eNB */
         }

         /* Associate the S1AP PDU to MME's event structure.
            Here onwards S1AP Pdu is accessed throuh MME's event
            structure. This will help in freeing the S1AP PDU 
            in caller too */
			/* To avoid klocworks warning this check has been introduced */
			if (ueMmeEvnt == NULLP)
			{
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                      "Failed to Decode Nas PDU"));
            RETVALUE(RFAILED); /* Should we send Failure back to eNB */
			}

			ueMmeEvnt->pdu = datEvnt->pdu;
         datEvnt->pdu = NULLP;
         ueCtxt.spConnId = 0;
         ueCtxt.suConnId =  datEvnt->suConnId;

#ifdef VB_MME_NAS_SEC
         if((CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC == ueMmeEvnt->secHT) ||
            (CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC_NEW_SEC_CTXT == ueMmeEvnt->secHT))
         {
            if(0 == ueCtxt.suConnId)
            {
               ret = vbMmeUtlFndUeCbOnEvent(eNbCb, &ueCb, ueCtxt.suConnId,\
                     ueCtxt.spConnId, ueMmeEvnt);
            }
            else
            {
               ret = vbMmeUtlFndUeCbOnConnId(eNbCb, &ueCb, datEvnt->suConnId); 
            }
              
            if(ROK != ret)
            {
#ifdef ALIGN_64BIT 				
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                    "Ue Not Found Base On Conn Id(%d)", datEvnt->suConnId));
#else
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                    "Ue Not Found Base On Conn Id(%ld)", datEvnt->suConnId));
#endif
               /*Ignore the event*/
               ueMmeEvnt->pdu = NULLP;
               CM_FREE_NASEVNT(&ueMmeEvnt);
               RETVALUE(RFAILED);
            }
            srcMsg.val = nasPdu->val;
            srcMsg.len = nasPdu->len;
            ret = vbMmeVldULSec(&ueCb->ueCtxt.secCtxt, &srcMsg, &dstMsg);
            if(ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                    "Nas Security Validation Failed, Ignore The Message"));
               /*Ignore the event*/
               ueMmeEvnt->pdu = NULLP;
               CM_FREE_NASEVNT(&ueMmeEvnt);
               RETVALUE(RFAILED);
            }

            secPdu.val = dstMsg.val;
            secPdu.len = dstMsg.len;
            ret = vbMmeEdmDecode(&secPdu, &ueMmeEvnt);
				/* This check has been introduced to avoid klocworks tool warning */
            if(ROK != ret || ueMmeEvnt == NULLP)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                     "Uplink NAS Decode Failure"));
               RETVALUE(ret); /* Should we send Failure back to eNB */
            }
         }
                
         if(CM_NAS_SEC_HDR_TYPE_PLAIN_NAS_MSG == ueMmeEvnt->secHT) 
         {
            ueMmeEvnt->secStatus = VB_MME_SEC_NOTPRTD;
         }
         else if(CM_NAS_SEC_HDR_TYPE_SER_REQ == ueMmeEvnt->secHT)
         {
            ueMmeEvnt->secStatus = VB_MME_SEC_SERVREQ;
         }
         else if(CM_NAS_SEC_HDR_TYPE_INT_PRTD == ueMmeEvnt->secHT)
         {
            ueMmeEvnt->secStatus = VB_MME_SEC_INTPRTD;
         }
         else
         {
            ueMmeEvnt->secStatus = VB_MME_SEC_SUCCESS;
         }
#endif
         /* Delegate now to a common entry function to handle
            EMM-ESM messages */
         ret = vbMmeLimHdlUeEvnt(eNbCb, &ueCtxt, ueMmeEvnt);
  
         break;
      }
#ifdef UE_RAD_CAP
      case Sztid_UECapbltyInfoInd:
#endif
      case Sztid_E_RABSetup:
      case Sztid_E_RABRls:
      case Sztid_E_RABMdfy:
      case Sztid_InitCntxtSetup : /* Initial Context Setup Response */
      case Sztid_UECntxtModification: /* Ue Context Modification Response */
      case Sztid_NASNonDlvryInd:
      case Sztid_HovrResourceAllocn:
      {
         ret = vbMmeRcvS1apMsg(datEvnt->suConnId, (Ptr*)datEvnt, msgType);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                    "Invalid Message Type(%d)", msgType));
         ret = RFAILED;
         break;
      }
   }
    
   RETVALUE(ret);
} /* End of vbMmeLimHdlIncUeMsg */


#ifdef ANSI
PRIVATE S16 vbMmeLimHdlUeEvnt
(
VbMmeEnbCb    *enbCb,
VbMmeUeCtxt   *ueCtxt,
CmNasEvnt     *vbEvnt
)
#else
PRIVATE S16 vbMmeLimHdlUeEvnt(enbCb, ueCtxt, vbEvnt)
VbMmeEnbCb    *enbCb;
VbMmeUeCtxt   *ueCtxt;
CmNasEvnt     *vbEvnt;
#endif
{
   S16 ret = RFAILED;

   VB_TRC2(vbMmeLimHdlUeEvnt);
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Incoming UeMsg suConnId(%d)",
                                   ueCtxt->suConnId ));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,\
         "Incoming UeMsg suConnId(%ld)", ueCtxt->suConnId ));
#endif

   /* Invoke the EMM / ESM Handler further */
   switch(vbEvnt->protDisc)
   {
      case CM_EMM_PD:
      {
         ret = vbMmeEmmHdlIncUeEvnt(enbCb, ueCtxt, vbEvnt);
         break;
      }
      case CM_ESM_PD:
      {
         ret = vbEsmHdlIncUeEvnt(enbCb, ueCtxt, vbEvnt);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                  "Invalid Protocol Discriminator(%d) NAS Message Type",
                  vbEvnt->protDisc));
      }
   }
   RETVALUE(ret);
} /* vbMmeLimHdlUeEvnt */


/* All EGTP-C Lower interface functions with EGTP-C */

 /*
 *
 *       Fun:   EuLiEgtSigCfm
 *
 *       Desc:  This function 
 *
 *       Ret:  
 *
 *       Notes: none
 *
 *       File:  vb_mme_lim.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtSigCfm
(
Pst         *pst,           /* post structure */
SuId         suId,          /* Use values from EgwNodeSapId */
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       transId,       /* */
TknU32       teid,          /* */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EuLiEgtSigCfm(pst,suId,localAddr,remAddr,transId,teid,egMsg)
Pst         *pst;           /* post structure */
SuId         suId;          /* Use values from EgwNodeSapId */
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       transId;       /* */
TknU32       teid;          /* */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   S16 ret = ROK;
   VB_TRC2(EuLiEgtSigCfm)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"EuLiEgtSigCfm: TeId(%d)", teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"EuLiEgtSigCfm: TeId(%ld)", teid.val));
#endif
   if(vbMmeCb.init.cfgDone != TRUE)
   {
      RETVALUE(RFAILED);
   }

   switch(egMsg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_CS_RSP:
      {
         ret = vbMmeIncCSRsp(localAddr, remAddr,
                                     teid, transId, egMsg);
         break;
      }
      case EGT_GTPC_MSG_MB_RSP:
      {
          ret = vbMmeIncMBRsp(localAddr, remAddr,
                                        teid, transId, egMsg);
         break;
      }
      case EGT_GTPC_MSG_DS_RSP:
      {
         ret = vbMmeIncDSRsp(localAddr, remAddr,
                                     teid, transId, egMsg);
         break;
      }
      case EGT_GTPC_MSG_RAB_RSP:
      {
         ret = vbMmeIncRlsAccBearerRsp(localAddr, remAddr, teid, transId,
                                       egMsg);
         break;
      }
      case EGT_GTPC_MSG_BR_FAIL_IND:
      {
         ret = vbMmeIncBrResFailInd(localAddr, remAddr, teid, transId,
                                       egMsg);
         break;
      }

      case EGT_GTPC_MSG_FR_RSP:
      {
         if( suId == VBSM_EG_S10_SUID )
         {
            ret = vbMmeIncFRRsp(localAddr, remAddr, teid, transId, egMsg);
         }
         else if( suId == VBSM_EG_S3_SUID )
         {
            ret = vbMmeIncS3FRRsp(localAddr, remAddr, teid, transId, egMsg);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF," Invalid SAP :%d\n ",suId));
         }
         break;
      }

      case EGT_GTPC_MSG_FR_COMP_ACK:
      {
         if( suId == VBSM_EG_S10_SUID )
         {
            ret = vbMmeIncFRCmplAck(localAddr, remAddr, teid, transId,\
               egMsg);
         }
         else if( suId == VBSM_EG_S3_SUID )
         {
            ret = vbMmeIncS3FRCmplAck(localAddr, remAddr, teid, transId,\
               egMsg);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF," Invalid SAP :%d\n ",suId));
         }
         
         break;
      }

      case EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK:
      {
         ret = vbMmeIncFACtxAck(localAddr, remAddr, teid, transId,\
               egMsg);
         break;
      }

      case EGT_GTPC_MSG_RC_RSP:
      {
         ret = vbMmeIncRCRsp(localAddr, remAddr, teid, transId,\
               egMsg);
         break;
      }

      default:
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid EgMsg(%d)",
                           egMsg->msgHdr.msgType));
         break;
      }
   }

   /* Free the egMsg here.Using the Utlity provided */
   EgUtilDeAllocGmMsg(&egMsg);

   RETVALUE(ret);
} /* EuLiEgtSigCfm */


/*
*
*       Fun:   EuLiEgtBndCfm
*      
*       Desc:  Call handler for Bind Confirmation
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_esm_gtp.c
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtBndCfm
(
Pst            *pst,
SuId           suId,
U8             status
)
#else
PUBLIC S16 EuLiEgtBndCfm(pst, suId, status)
Pst            *pst;
SuId           suId;
U8             status;
#endif
{
   VB_TRC3(EuLiEgtBndCfm);

   VB_MME_DBG_PRNT((VB_MME_PRNTBUF,\
         "EuLiEgtBndCfm(pst, suId(%d), status(%d))", suId, status));

   if(suId > vbMmeCb.mmeCfg.maxNmbEgtSaps) /* Invalid SAP Identification */
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB040, (ErrVal) suId,\
                      "Invalid SAP Identification From EGTP ");
      RETVALUE(ROK);
   }

   if(suId == (vbMmeCb.egtSapCb->suId))
   {
      vbMmeHndlEgtBndCfm(vbMmeCb.egtSapCb, status);
   }
   else if(suId == (vbMmeCb.egtEchoSapCb->suId))
   {
      vbMmeHndlEgtBndCfm(vbMmeCb.egtEchoSapCb, status);
   }
   else if(suId == (vbMmeCb.egtS10SapCb->suId))
   {
      vbMmeHndlEgtBndCfm(vbMmeCb.egtS10SapCb, status);
   }
   else if( suId == (vbMmeCb.egtS3SapCb->suId)) /* handle the bind confirm for S3 sap */
   {
      vbMmeHndlEgtBndCfm(vbMmeCb.egtS3SapCb, status);
      
   }
   else
   {
      VB_MME_LOGERROR(ERRCLS_INT_PAR, EVB041, (ErrVal) suId,\
                      "Invalid SAP Identification From EGTP ");
      RETVALUE(ROK);
   }
   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 vbMmeHndlEgtBndCfm
(
VbMmeEgtSapCb *egtSapCb,
U8             status
)
#else
PRIVATE S16 vbMmeHndlEgtBndCfm(egtSapCb, status)
VbMmeEgtSapCb *egtSapCb;
U8             status;
#endif
{

   if(CM_BND_OK == status)
   {
      egtSapCb->state = LVB_SAP_ISBOUND;

      /* Generate an Alarm to stack manager that MME is now
         bound to S1AP */
      vbSendLmAlarm(LCM_CATEGORY_INTERFACE, LVB_EVENT_MME_EGTP_BNDCFM, 
                    LCM_CAUSE_UNKNOWN);
      RETVALUE(ROK);
   }
   else
   {
      egtSapCb->state = LVB_SAP_UBND;

      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB042, ERRZERO, 
                      "Bind Failure With EGTP");
      RETVALUE(RFAILED);
   }
  
} /* End of EuLiEgtBndCfm */



/*
 *
 *       Fun:   EuLiEgtSigInd
 *
 *       Desc:  This function 
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_mme_lim.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtSigInd
(
Pst         *pst,           /* post structure */
SuId         suId,          /* Use values from EgwNodeSapId */
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EuLiEgtSigInd (pst,suId,localAddr,remAddr,teid,egMsg)
Pst         *pst;           /* post structure */
SuId         suId;          /* Use values from EgwNodeSapId */
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   S16 ret = ROK;
   TknU32       transId;

   VB_TRC2(EuLiEgtSigInd)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"EuLiEgtSigInd: TeId(%d)",teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"EuLiEgtSigInd: TeId(%ld)",teid.val));
#endif
   
   /* vb001.101: Supported EgtSigInd*/
   if(vbMmeCb.init.cfgDone != TRUE)
   {
      RETVALUE(RFAILED);
   }
   
   switch(egMsg->msgHdr.msgType)
   {
      case EGT_GTPC_MSG_DWN_DATA_NOTFN:
      {
         ret = vbMmeIncDwnLnkDatNot(localAddr, remAddr,
                          teid, transId, egMsg);
         break;
      } /* end of Downlink data notification case */
      case EGT_GTPC_MSG_CB_REQ:
      {
         ret =  vbMmeIncCreateBearerReq(localAddr, remAddr,
                                       teid, transId, egMsg);
         break;

      }
      case EGT_GTPC_MSG_DB_REQ:
      {
         ret =  vbMmeIncDeleteBearerReq(localAddr, remAddr,
                                       teid, transId, egMsg);
         break;
      }

      case EGT_GTPC_MSG_FR_REQ:
      {
         if( suId == VBSM_EG_S10_SUID )
         {
            ret = vbMmeIncFRReq(localAddr, remAddr, teid, transId, egMsg);
         }
         else if( suId == VBSM_EG_S3_SUID )
         {
            ret = vbMmeIncS3FRReq(localAddr, remAddr, teid, transId, egMsg);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Forward Relocation Request is"\
                                "received on invalid SAP :%d\n ",suId));
         }
         break;
      }

      case EGT_GTPC_MSG_FR_COMP_NOTFN:
      {
         if( suId == VBSM_EG_S10_SUID )
         {
            ret = vbMmeIncFRCmplNotif(localAddr, remAddr, teid, transId,\
               egMsg);
         }
         else if( suId == VBSM_EG_S3_SUID )
         {
            ret = vbMmeIncS3FRCmplNotif(localAddr, remAddr, teid, transId,\
               egMsg);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Forward Relocation Notification "\
                                "is received on invalid SAP :%d\n ",suId));
         }
         
         break;
      }

      case EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN:
      {
         ret = vbMmeIncFACtxNotif(localAddr, remAddr, teid, transId,\
               egMsg);
         break;
      }

      case EGT_GTPC_MSG_RC_REQ:
      {
         ret = vbMmeIncRCReq(localAddr, remAddr, teid, transId,\
               egMsg);
         break;
      }

      default:
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid EgMsg(%d)",
                                          egMsg->msgHdr.msgType));
         break;
      } /* end of default */
   } /* end of switch statement */

   /* Free the egMsg here.Using the Utlity provided */
   EgUtilDeAllocGmMsg(&egMsg);

   RETVALUE(ret);
} /* EuLiEgtSigInd */

/*
 *
 *       Fun:   EuLiEgtLclDelCfm
 *
 *       Desc:  This function 
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_mme_lim.c
 *
 */

#ifdef ANSI
PUBLIC S16 EuLiEgtLclDelCfm
(
Pst          *pst,      /* post structure */
SuId         suId,      /* eGTP service user SAP ID */
U32          transId,    /* Transaction ID */
U32          teid,      /* self TEID */
CmStatus     status     /* Status of request primitive */
)
#else
PUBLIC S16  EuLiEgtLclDelCfm(pst, suId, transId, teid, status)
Pst          *pst;      /* post structure */
SuId         suId;      /* eGTP service user SAP ID */
U32          transId;    /* Transaction ID */
U32          teid;      /* self TEID */
CmStatus     status;    /* Status of request primitive */
#endif
{
   S16 ret = ROK;
   VB_TRC2(EuLiEgtLclDelCfm)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"EuLiEgtLclDelCfm: TeId(%d)", teid));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"EuLiEgtLclDelCfm: TeId(%ld)", teid));
#endif

   /* VB_TODO_SECOND_PHASE  ATUL To be handled with care in later phases */ 
   if(status.status == LCM_PRIM_NOK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Local Delete Tunnel Failed"));
   }

   RETVALUE(ret);
}

/*
 * Handler for C-plane modification confirm from the egtp.
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtTunnModCfm
(
Pst      *pst,             /* Pst structure for the sap */
SuId      suId,            /* Service user ID of the SAP */
U32       transId,         /* Transaction ID */
U32       teid,            /* teid of for which cfm is received */
CmTptAddr oldDstIpAddr,    /* Old dst Ip associated with tunnel */
CmTptAddr newDstIpAddr,    /* New IP or new peer address */
CmStatus  status           /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
)
#else
PUBLIC S16 EuLiEgtTunnModCfm(pst, suId, transId, teid, oldDstIpAddr, newDstIpAddr, status)
Pst      *pst;             /* Pst structure for the sap */
SuId      suId;            /* Service user ID of the SAP */
U32       transId;         /* Transaction ID */
U32       teid;            /* teid of for which cfm is received */
CmTptAddr oldDstIpAddr;    /* Old dst Ip associated with tunnel */
CmTptAddr newDstIpAddr;    /* New IP or new peer address */
CmStatus  status;          /* Status of the confirm, LCM_PRIM_OK or LCM_PRIM_NOK */
#endif
{
   VB_MME_TRC2(EuLiEgtTunnModCfm)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Tunnel Mod Cfm Received From EGTP"));
   RETVALUE(ROK);

}

/*
*
*       Fun:   EuLiEgtStaInd
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtStaInd
(
Pst            *pst,
SuId           suId,
U8           eventType,    /* Status event type */
EgPathStatus   *pathStatus
)
#else
PUBLIC S16 EuLiEgtStaInd(pst, suId, eventType, pathStatus)
Pst            *pst;
SuId           suId;
U8           eventType;    /* Status event type */
EgPathStatus   *pathStatus;
#endif /* ANSI */
{
   VB_MME_TRC2(EuLiEgtStaInd);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Status Indication Received From EGTP"));
   RETVALUE(ROK);
} /* End of EuLiEgtStaInd */

/*
*
*       Fun:   EuLiEgtStaCfm
*
*       Desc:  Call handler for Status Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtStaCfm
(
Pst            *pst,
SuId           suId,
U8           eventType,    /* Status event type */
EgPathInfo   *pathInfo,
CmStatus     status
)
#else
PUBLIC S16 EuLiEgtStaCfm(pst, suId, eventType, pathInfo, status)
Pst            *pst;
SuId           suId;
U8           eventType;    /* Status event type */
EgPathInfo   *pathInfo;
CmStatus     status;
#endif /* ANSI */
{
   VB_MME_TRC2(EuLiEgtStaCfm);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Status Confirmation Received From EGTP"));
   RETVALUE(ROK);
} /* End of EuLiEgtStaCfm */



/*
*
*       Fun:   EuLiEgtErrInd
*
*       Desc:  Call handler for Error Indication
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  
*       
*/
#ifdef ANSI
PUBLIC S16 EuLiEgtErrInd
(
Pst            *pst,
SuId           suId,
EgErrEvnt      *errEvnt
)
#else
PUBLIC S16 EuLiEgtErrInd(pst, suId, errEvnt)
Pst            *pst;
SuId           suId;
EgErrEvnt      *errEvnt;
#endif /* ANSI */
{
   S16                     ret         = ROK;
   VbMmeUeTunnelCb         *tunEnt     = NULLP;

   VB_MME_TRC2(EuLiEgtErrInd);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Error Indication Received From EGTP"));
   
   if (EGT_GTPC_MSG_CS_REQ == errEvnt->failedMsgType)
   {
      ret = cmHashListFind(&(vbMmeCb.egtEchoSapCb->tunnPendCbHlCp),
                        (U8 *)&(errEvnt->transId),
                        sizeof(errEvnt->transId), 0, (PTR *)&tunEnt);
   }
   else if (EGT_GTPC_MSG_FR_REQ == errEvnt->failedMsgType)
   {
      ret = cmHashListFind(&(vbMmeCb.egtS10SapCb->tunnPendCbHlCp),\
            (U8 *)&(errEvnt->transId), sizeof(errEvnt->transId), 0,\
            (PTR *)&tunEnt);
   }
   else if (EGT_GTPC_MSG_FR_COMP_NOTFN == errEvnt->failedMsgType)
   {
      ret = cmHashListFind(&(vbMmeCb.egtS10SapCb->ueTunnelCbHlCp),\
            (U8 *)&(errEvnt->transId), sizeof(errEvnt->transId), 0,\
            (PTR *)&tunEnt);
   }
   else
   {
      ret = cmHashListFind(&(vbMmeCb.egtS10SapCb->tunnPendCbHlCp),\
            (U8 *)&(errEvnt->transId), sizeof(errEvnt->transId), 0,\
            (PTR *)&tunEnt);
   }

   if(ret != ROK)
   {
#ifdef ALIGN_64BIT      
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,\
            "Transaction Not Found For Transid(%d)", errEvnt->transId));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,\
            "Transaction Not Found For Transid(%ld)", errEvnt->transId));
#endif
   }
   else
   {
      ret = ROK;
      vbMmeEgtpErrInd(tunEnt->ueCb, errEvnt);  
   }

   RETVALUE(ret);
} /* End of EuLiEgtErrInd */


/**************************************************************************//**
 *
 * @brief 
 *    Handles S1ap connection confrimation messages
 *             
 * @param[in]  pConCfm
 *    Pointer to s1ap connenction confirmation event
 * @param[in]  msgType
 *    Type of S1ap message received
 * 
 * @return
 *    SUCCESS if the UE details are present in DB therwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeLimHdlIncUeConCfm
(
SztConCfm  *pConCfm,
U8          msgType
)
{
   S16            ret = ROK;

   VB_TRC2(vbMmeLimHdlIncUeConCfm);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Ue ConMsg eNodeb(%ld)",\
         pConCfm->spConnId));

   switch(msgType)
   {
      case Sztid_HovrResourceAllocn:
      {
         ret = vbMmeRcvS1apMsg(pConCfm->suConnId, (Ptr*)pConCfm, msgType);
         break;
      }

      default:
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Message Type(%d)",\
               msgType));
         break;
      }
   }
  
   RETVALUE(ret);
} /* vbMmeLimHdlIncUeConCfm */

#ifdef __cplusplus
}
#endif

/**********************************************************************
  
         End of file:     vb_mme_lim.c@@/main/1 - Mon Jun 28 19:35:39 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---        sv               1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1  vb001.101    rk              1. Find eNodeB based on peerId & 
                                         ueCB based on ueId.
                                      2. Handled UE context release request.
                                      3. Handled ERAB Setup messages.
                                      4. Handled paging message.
                                      5. Added new argument nasPduPres in vbMmeLimSndToLower
                                      6. Supported EgtSigInd
/main/1  vb003.101   chekar           1. Added Ue Rad Capability support.
/main/1  vb004.101   nj               1. Added for AttachRej as DatReq not ConRsp 
/main/1  vb006.101 ragrawal           1. CRID:ccpu00117391 ERAB RELEASE support. 
                                      2. ccpu00118283: Added for Identity request, AUTH req to be sent as ConRsp 
                                      3. Test Code for OverLoading Feature.
                                      4. Added new Switch case Sztid_PathSwRqst.
                                      5. Fix for g++ compilation Warning.
                                      6. Modified function vbMmeLimSndStoredBuf().
                                      7. Added new local variable and made changes 
                                         to make eNodeB id as cell Id in function
                                         vbMmeLimHdlIncUeConMsg().
/main/1  vb007.101 akaranth            1. Fix for Multi UE context release.
                                       2. Removed unused varaible to fix compiler warning.
*********************************************************************91*/
