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

     Desc:    This file contains the vb application's s1ap utility
              source code

     File:    vb_mme_s1ap_utl.c

     Sid:     vb_mme_s1ap_utl.c@@/main/1 - Mon Jun 28 19:35:38 2010

     Prg:     sv 
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
#include "vb_s10_utl.x"

#include "vb_s3_hndlr.x"
#include "vb_s3_utl.x"
#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/*************** PRIVATE FUNCTION DECLARATIONS *******************************/
PRIVATE S16 vbMmeUtlGetTargetMme
(
VbPlmnId                *pPlmnId,
CmTptAddr               *pTgtMmeIpAddr
);

PRIVATE S16 vbMmeUtlGetTargetSgsn
(
U8*                     pPlmnId,
CmTptAddr               *pTgtSgsnIpAddr
);


PRIVATE S16 vbMmeUtlBldHOCancelAck
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoCancelAck
);

PRIVATE S16 vbMmeHndlHOReqAck
(
VbMmeUeCb      *pUeCb,
SztConCfm      *pConCfm
);

PRIVATE S16 vbMmeHndlS10HOReqAck
(
VbMmeUeCb      *pUeCb
);

PRIVATE S16 vbMmeHndlS3HOReqAck
(
VbMmeUeCb      *pUeCb
);

PRIVATE S16 vbMmeUtlValNStoreEnbStatusTfr
(
VbMmeUeCb      *pUeCb,
S1apPdu        *pPdu
);

PRIVATE S16 vbMmeUtlValNStoreHOReqAck
(
VbMmeUeCb      *pUeCb,
S1apPdu        *pPdu
);

PRIVATE S16 vbMmeUtlValNStoreHONotif
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
);

PUBLIC S16 vbMmeUtlBldHOReq
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoReq
);

PUBLIC S16 vbMmeUtlBldMmeStatusTfr
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoReq
);


PRIVATE S16 vbMmeUtlBldHOCmd
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoCmd
);

PRIVATE S16 vbMmeUtlBldHOPrepFail
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoPrepFail
);

/*****************************************************************************/


/*
*
*       Fun:  vbMmeHdlOutPagingReq
*       
*       Desc:  Entry function for sending out PAGING towarsd enB
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlOutPagingReq
(
VbMmeUeCb  *ueCb
)
#else
PUBLIC S16 vbMmeHdlOutPagingReq(ueCb)
VbMmeUeCb  *ueCb;
#endif
{
    S16           ret = RFAILED;
    S1apPdu      *s1PagingReq = NULLP;
    VbMmeEnbCb   *eNbCb = NULLP;
    VB_TRC2( vbMmeHdlOutPagingReq)
 
    eNbCb = ueCb->eNodebCb;   
    if(eNbCb == NULLP)
    {
       VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "NULL eNodeB Cb"));
       RETVALUE(RFAILED);
    }
    /* build the paging request */ 
    ret = vbMmeUtlBldS1PagingReq(eNbCb,ueCb, TRUE, &s1PagingReq);
    if(ROK != ret)
    {
        VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "S1 Paging Request Build Failure"));
        RETVALUE(ret); 
    }
    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Sending Paging Request To eNodeB"));
    /* Send the S1 paging request */
    ret = vbMmeLimSndS1APMsg(eNbCb, NULLP, s1PagingReq, Sztid_Pag);
 
    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Sent Paging Request Successfully"));
    RETVALUE(ret);
}

/*
*
*       FUN:   vbMmeUtlRemLsbPaddingBits
*       
*       DESC:  Function removes the 4LSB bits of the received enodeBID
*              from ENodeB.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/

#ifdef ANSI
PRIVATE S16 vbMmeUtlRemLsbPaddingBits
(
U8* enbId
)
#else
PRIVATE S16 vbMmeUtlRemLsbPaddingBits(enbId)
   U8* enbId;
#endif
{

   U8 prevNibble = 0;
   U8 parityNibble = 0;
   U8 temp = 0;
   U8 i;
   
   VB_TRC2(vbMmeUtlRemLsbPaddingBits);
   
   for( i = 0;i <4;i++)
   {   
      temp = enbId[i];
      parityNibble = enbId[i]&0x0f; /* Save the parity nibble here */
      temp = temp>>4 | (prevNibble << 4)&0xf0; 
      enbId[i] = temp; 
      prevNibble = parityNibble; /* Save the current parity nibble to previous nibble*/
   }   
   
   RETVALUE(ROK);
}
 
/*
*
*       Fun:   vbMmeHdlIncSetup
*       
*       Desc:  Entry function for handling selective 
*              incoming S1AP messages without EMM-ESM interpretations
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlIncSetup
(
SztUDatEvnt    *uDatEvnt
)
#else
PUBLIC S16 vbMmeHdlIncSetup(uDatEvnt)
SztUDatEvnt    *uDatEvnt;
#endif
{
    
    S16              ret = RFAILED;
    S16              index1,index2;
 
    VbMmeEnbCb       *enbCb = NULLP;
    S1apPdu          *s1SetupRspPdu = NULLP;
    S1apPdu          *s1SetupFailPdu = NULLP;
    SztGlobal_ENB_ID *sztGlobal_ENB_ID = NULLP;
    SztENBname       *sztENBname = NULLP;
    SztSuppTAs       *sztSuppTAs = NULLP;
    SztPagDRX        *sztPagDRX = NULLP;
    PTR              prevEnt = NULLP;
    VbMmeUeCb        *ueCb = NULLP;
    U32              enodeBId = 0;

    VB_TRC2(vbMmeAppIncSetup);
  
    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received S1 Setup Request"));
 
    if(ROK != vbMmeUtlFndEnbCb(&enbCb, uDatEvnt->peerId.val))
    {
#ifdef ALIGN_64BIT    
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb(%d)",
                uDatEvnt->peerId.val));
#else
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb(%ld)",
                uDatEvnt->peerId.val));
#endif
        RETVALUE(ret); /* Drop message */
    }

    /*Release all s1ap resource here*/
    prevEnt = NULLP;
    for(;;)
    {
       /* get next entry */
       if (cmHashListGetNext(&enbCb->ueCbHlCp, prevEnt, (PTR *) &ueCb) != ROK)
       {
          /* no more entries */
          break;
       }

       /* Release the Connection Locally */
       vbMmeRelLocS1Con(ueCb);
       VB_MME_END_EMM_PROC(ueCb);
       /* do not update prevEnt pointer */
       prevEnt = NULLP;
    }

    /* Reset application data */
    cmMemset((U8*)&enbCb->enbStupInfo, 0, sizeof(EnbStupInfo));

    /* Update the global eNB Identifier */
    vbMmeUtlGetS1apIE(&(uDatEvnt->pdu->pdu), Sztid_Global_ENB_ID, 
                        (TknU8 ** )&sztGlobal_ENB_ID);
    if((NULLP != sztGlobal_ENB_ID) && 
                     (sztGlobal_ENB_ID->pres.pres == PRSNT_NODEF))
    {
        cmMemcpy((U8*)&enbCb->enbStupInfo.gEnbId.enbId, 
                         (U8*)&sztGlobal_ENB_ID->eNB_ID, sizeof(SztENB_ID));
      
        /* UMAPATHI To Avoid the 4 LSB padding, we are shifting the home enodeBID by 4 bits.
         * Same should be done for Macro EnodeB ID also in future. */
        vbMmeUtlRemLsbPaddingBits((U8*)enbCb->enbStupInfo.gEnbId.enbId.val.homeENB_ID.val);
       
        if(sztGlobal_ENB_ID->pLMNidentity.len)
        {
            cmMemcpy((U8*)enbCb->enbStupInfo.gEnbId.plmnId.plmnId, 
                        sztGlobal_ENB_ID->pLMNidentity.val, VB_PLMN_ID_LEN);
        }
    }
 
    vbMmeUtlGetS1apIE(&(uDatEvnt->pdu->pdu), Sztid_eNBname, 
                        (TknU8 ** )&sztENBname);
    if((NULLP != sztENBname) && (PRSNT_NODEF == sztENBname->pres))
    {
        enbCb->enbStupInfo.enbName.len = 
                         ((sztENBname->len) > VB_ENB_NAME_LEN)? 
                           VB_ENB_NAME_LEN:sztENBname->len; 
        cmMemcpy(enbCb->enbStupInfo.enbName.name, sztENBname->val, 
                           enbCb->enbStupInfo.enbName.len);
        enbCb->enbStupInfo.enbName.pres.pres = PRSNT_NODEF;
    }
    
 
    vbMmeUtlGetS1apIE(&(uDatEvnt->pdu->pdu), Sztid_SuppTAs, 
                         (TknU8 ** )&sztSuppTAs);
    if((NULLP != sztSuppTAs) && sztSuppTAs->noComp.val)
    {
       enbCb->enbStupInfo.supportedTAs.nmbOfTAs = 
          (sztSuppTAs->noComp.val > VB_TAC_LIST_LEN)? 
          VB_TAC_LIST_LEN:sztSuppTAs->noComp.val;

       for(index1=0; index1<enbCb->enbStupInfo.supportedTAs.nmbOfTAs; index1++)
       {
          enbCb->enbStupInfo.supportedTAs.supportedTAItem[index1].tac = 
             sztSuppTAs->member[index1].tAC.val[0]<<8 |
             sztSuppTAs->member[index1].tAC.val[1];

          enbCb->enbStupInfo.supportedTAs.supportedTAItem[index1].nmbOfPlmn =
             (sztSuppTAs->member[index1].broadcastPLMNs.noComp.val > 
              VB_PLMN_LIST_LEN)? VB_PLMN_LIST_LEN:
             sztSuppTAs->member[index1].broadcastPLMNs.noComp.val;

          for(index2=0; index2<enbCb->enbStupInfo.supportedTAs.
                supportedTAItem[index1].nmbOfPlmn; index2++)
          {
             cmMemcpy(&enbCb->enbStupInfo.supportedTAs.supportedTAItem[index1].
                   plmnId[index2][0], sztSuppTAs->member[index1].
                   broadcastPLMNs.member[index2].val, VB_PLMN_ID_LEN);
          }
       }
    }
 
    /* fetch and store Paging DRX */
    vbMmeUtlGetS1apIE(&(uDatEvnt->pdu->pdu), Sztid_DefaultPagDRX, 
                        (TknU8 ** )&sztPagDRX);
    if((NULLP != sztPagDRX) && (sztPagDRX->pres == PRSNT_NODEF))
    {
        enbCb->enbStupInfo.dfltPgingDRX = sztPagDRX->val; 
    }
 
    ret = vbMmeUtlVldS1SetupPlmn(&enbCb->enbStupInfo.supportedTAs);
    if(ret != ROK)
    {
       ret = vbMmeUtlBldS1SetupFail(enbCb, &s1SetupFailPdu, CAUSE_MISC, 
             SztCauseMiscunknown_PLMNEnum);
       if(ROK != ret)
       {
          VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "S1 Setup Failure Build Failure"));
          RETVALUE(ret); /* Should we send S1 Setup Failure back to eNB */
       }
       /* Send the S1 Setup Response back */
       ret = vbMmeLimSndS1APMsg(enbCb, NULLP, s1SetupFailPdu, Sztid_S1Setup);

       VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "S1 Setup Failure Sent Successfully"));
       RETVALUE(ret);
    }
 
    /* Update any other information required */
    /* Send the S1 Setup Response back */
    ret = vbMmeUtlBldS1SetupRsp(enbCb, &s1SetupRspPdu);
    if(ROK != ret)
    {
       VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "S1 Setup Rsp Build Failure"));
       RETVALUE(ret); /* Should we send S1 Setup Failure back to eNB */
    }
 
    enbCb->state = VB_MME_ENB_UP;
 
    /* Send the S1 Setup Response back */
    ret = vbMmeLimSndS1APMsg(enbCb, NULLP, s1SetupRspPdu, Sztid_S1Setup); 
 
    RETVALUE(ret);
} /* End of vbMmeHdlIncSetup */
 
/*
*
*       Fun:   vbMmeSndS1Reset
*       
*       Desc:  Function used to send S1 Reset message.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeSndS1Reset
(
VbMmeEnbCb     *enbCb,
SztCause       *cause,
U8             flag
)
#else
PUBLIC S16 vbMmeSndS1Reset(enbCb, cause, flag)
VbMmeEnbCb     *enbCb;
SztCause       *cause;
U8             flag;
#endif
{
    
    S16              ret = RFAILED;
    S1apPdu          *s1ResetPdu = NULLP;
 
    VB_TRC2(vbMmeSndS1Reset);
  
    if (NULLP == enbCb)
    {
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Received enbCb is NULL"));
       RETVALUE(ret);
    }

    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending S1 Reset"));

    /* Building S1 Reset */
    ret = vbMmeUtlBldS1Reset(&s1ResetPdu, cause, flag);
    if(ROK != ret)
    {
       VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "S1 Reset Build Failure"));
       RETVALUE(ret); 
    }
 
    /* Send the S1 Reset  */
    ret = vbMmeLimSndS1APMsg(enbCb, NULLP, s1ResetPdu, Sztid_Reset);
 
    RETVALUE(ret);
} /* End of vbMmeSndS1Reset */


/*
*
*       Fun:   vbMmeHdlIncReset
*       
*       Desc:  Entry function for handling of
*              incoming S1 Reset message.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlIncReset
(
SztUDatEvnt    *uDatEvnt
)
#else
PUBLIC S16 vbMmeHdlIncReset(uDatEvnt)
SztUDatEvnt    *uDatEvnt;
#endif
{
    
    S16              ret = RFAILED;
    SztMME_UE_S1AP_ID    mme_ue_s1ap_id; 
    SztS1AP_PDU      *s1ap_pdu = NULLP;
    VbMmeEnbCb       *enbCb = NULLP;
    S1apPdu          *s1ResetAckPdu = NULLP;
    SztCause         *sztCause = NULLP;
    SztResetTyp      *sztResetTyp = NULLP; 
    PTR              prevEnt = NULLP;
    PTR              lastEnt = NULLP;
    VbMmeUeCb        *ueCb = NULLP;
    U8               flag = FALSE;
    SztProtIE_Field_ResetIEs     *ie2 = NULLP;
    SztProtIE_SingleCont_UE_assocLogS1_ConItemRes *firstItem = NULLP;
    U16 numComp = 0;
    U16 idx = 0;

    VB_TRC2(vbMmeHdlIncReset);
  
    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received S1 Reset"));
 
    if(ROK != vbMmeUtlFndEnbCb(&enbCb, uDatEvnt->peerId.val))
    {
#ifdef ALIGN_64BIT    
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb(%d)",
                uDatEvnt->peerId.val));
#else
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb(%ld)",
                uDatEvnt->peerId.val));
#endif
        RETVALUE(ret); /* Drop message */

    }

    /* Retreive Cause */
    vbMmeUtlGetS1apIE(&(uDatEvnt->pdu->pdu), Sztid_Cause, 
                        (TknU8 ** )&sztCause);

    if((NULLP != sztCause) && 
                     (sztCause->choice.pres == PRSNT_NODEF))
    {
       VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                         "Received S1 Reset cause type=[%d]", 
                         sztCause->choice.val));
       /* Update in eNbCb if required */
    }


    /* Retreive Type */
    vbMmeUtlGetS1apIE(&(uDatEvnt->pdu->pdu), Sztid_ResetTyp, 
                        (TknU8 ** )&sztResetTyp);

    if((NULLP != sztResetTyp) && (PRSNT_NODEF == sztResetTyp->choice.pres))
    {
       if (0 == sztResetTyp->choice.val)
       {
          VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                         "Received Choice Reset Type: S1 interface")); 
       }
       else
       {
          VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                         "Received Choice Reset Type: Part of S1 interface")); 
          flag = TRUE;
       }
    }

    /* Take action only of those UE's which present in the UE associated 
     logical connection list */
    if (TRUE == flag)
    {
       s1ap_pdu = &(uDatEvnt->pdu->pdu);
       /* Retrieve no of item present in the list of s1 reset message*/
       ie2 = &s1ap_pdu->val.initiatingMsg.value.u.sztReset.protocolIEs.member[1];
       /* Number of item present in the list */
       numComp = ie2->value.u.sztResetTyp.val.partOfS1_Intf.noComp.val;
       firstItem = &ie2->value.u.sztResetTyp.val.partOfS1_Intf.member[0];

       for (idx = 0 ; idx < numComp; idx++)
       {
          if (NOTPRSNT == (firstItem+idx)->value.u.sztUE_assocLogS1_ConItem.pres.pres)
          {
             continue;
          }

          mme_ue_s1ap_id = (firstItem+idx)->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID;

          /*Release all s1ap resource here*/
          prevEnt = NULLP;
          for(;;)
          {
             /* get next entry */
             if (cmHashListGetNext(&enbCb->ueCbHlCp, prevEnt,
                                         (PTR *) &ueCb) != ROK)
             {
                /* no more entries */
                break;
             }
             lastEnt = prevEnt;
             prevEnt = (PTR)ueCb;
             if (ueCb->mMeUeS1apId == (U16)mme_ue_s1ap_id.val)
             {
                /* UE In Idle Mode */
                if ((VB_MME_NULL_PROC == ueCb->proc)  &&
                         (VB_ECM_CONNECTED == ueCb->ecmCb.state))
                {   
                   vbMmeSndRABReq(ueCb);
                   if(ueCb->eNodebCb->eNbCfg.t3412Tmr)
                   {
                      vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE,
                            (ueCb->eNodebCb->eNbCfg.t3412Tmr + \
                            VB_TMR_EMM_MOB_REACHABLE_OFFSET));
                   }
                }
                vbMmeUtlDelPndTrns(ueCb);
                ueCb->ecmCb.state = VB_ECM_IDLE;
                ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;
                /*Remove UE from the list */
                ret = vbMmeUtlDelUeCbOnConnId(ueCb);
                if(ROK == ret)
                {
                   prevEnt = lastEnt;
                }
                else
                {
                   prevEnt = (PTR)ueCb;
                }

                /* Check for any running procedure and take action */
                if (VB_MME_ESM_PROC == ueCb->proc)
                {
                   vbMmeAbortEsmProc(ueCb);
                }
                else if (VB_MME_ATTACH_PROC == ueCb->proc)
                {
                   printf("Reset Reached while ATTACH Proc is running\n");
                   vbMmeAbortAttachProc(ueCb);
                   printf("Abort of Attach Proc done\n");
                }
                else if (VB_MME_TAU_PROC == ueCb->proc)
                {
                   vbMmeAbortTAUProc(ueCb);
                }
                else if (VB_MME_SERVICE_REQ_PROC == ueCb->proc)
                {
                   vbMmeAbortServProc(ueCb);
                }
                else if (VB_MME_CTXT_REL_PROC == ueCb->proc)
                {
                   vbMmeAbortUeCtxtRelProc(ueCb);
                   prevEnt = lastEnt;
                }
                else
                {
                   VB_MME_END_EMM_PROC(ueCb);
                }
             }
          }
       }
    }    
    else  /* for reset all */
    {
       /* Release all s1ap resource here*/
       prevEnt = NULLP;
       for(;;)
       {
          /* get next entry */
          if (cmHashListGetNext(&enbCb->ueCbHlCp, prevEnt,
                            (PTR *) &ueCb) != ROK)
          {
             /* no more entries */
             break;
          }

          prevEnt = (PTR)ueCb;

          /* UE In Idle Mode */
          if ((VB_MME_NULL_PROC == ueCb->proc)  &&
                  (VB_ECM_CONNECTED == ueCb->ecmCb.state))
          {
             vbMmeSndRABReq(ueCb);
             if(ueCb->eNodebCb->eNbCfg.t3412Tmr)
             {
                vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE,
                      (ueCb->eNodebCb->eNbCfg.t3412Tmr + VB_TMR_EMM_MOB_REACHABLE_OFFSET));
             }

          }
          vbMmeUtlDelPndTrns(ueCb);
          ueCb->ecmCb.state = VB_ECM_IDLE;
          ueCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_INIT;
          /*Remove UE from the list */
          ret = vbMmeUtlDelUeCbOnConnId(ueCb);
          if(ROK == ret)
          {
             prevEnt = (PTR)NULLP;
          }
          else
          {
             prevEnt = (PTR)ueCb;
          }

          /* Check for any running procedure and take action */
          if (VB_MME_ESM_PROC == ueCb->proc)
          {
             vbMmeAbortEsmProc(ueCb);
          }
          else if (VB_MME_ATTACH_PROC == ueCb->proc)
          {
             vbMmeAbortAttachProc(ueCb);
          }
          else if (VB_MME_TAU_PROC == ueCb->proc)
          {
             vbMmeAbortTAUProc(ueCb);
          }
          else if (VB_MME_SERVICE_REQ_PROC == ueCb->proc)
          {
             vbMmeAbortServProc(ueCb);
          }
          else if (VB_MME_CTXT_REL_PROC == ueCb->proc)
          {
             vbMmeAbortUeCtxtRelProc(ueCb);
             prevEnt = NULLP;
          }
          else
          {
             VB_MME_END_EMM_PROC(ueCb);
          }
       }
    }

    /* Send the S1 Reset Ack */
    ret = vbMmeUtlBldS1ResetAck(flag, uDatEvnt, &s1ResetAckPdu);
    if(ROK != ret)
    {
       VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "S1 Reset Ack Build Failure"));
       RETVALUE(ret); 
    }
 
    /* Send the S1 Reset Ack back */
    ret = vbMmeLimSndS1APMsg(enbCb, NULLP, s1ResetAckPdu, Sztid_Reset);
 
    RETVALUE(ret);
} /* End of vbMmeHdlIncReset */

/*
*
*       Fun:   vbMmeHdlIncResetAck
*       
*       Desc:  Entry function for handling of
*              incoming S1 Reset Ack message.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlIncResetAck
(
SztUDatEvnt    *uDatEvnt
)
#else
PUBLIC S16 vbMmeHdlIncResetAck(uDatEvnt)
SztUDatEvnt    *uDatEvnt;
#endif
{
    
    S16              ret = RFAILED;
    VbMmeEnbCb       *enbCb = NULLP;
 
    VB_TRC2(vbMmeHdlIncResetAck);
  
    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received S1 Reset Ack"));
 
    if(ROK != vbMmeUtlFndEnbCb(&enbCb, uDatEvnt->peerId.val))
    {
#ifdef ALIGN_64BIT    
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb(%d)",
                uDatEvnt->peerId.val));
#else
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unexpected eNodeB Cb(%ld)",
                uDatEvnt->peerId.val));
#endif
        RETVALUE(ret); /* Drop message */

    }

    RETVALUE(ret);
} /* End of vbMmeHdlIncResetAck */


/*
*
*       Fun:   vbMmeHdlInitialCtxtSetupRsp
*       
*       Desc:  Function for handling incoming
*              Initial Context Setup Response from eNodeB.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlInitialCtxtSetupRsp
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlInitialCtxtSetupRsp(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
   VbMmeEsmCb                  *esmCb            = NULLP;
   SztE_RABSetupLstCtxtSURes   *eNbRABInfo       = NULLP;
   SztE_RABLst                 *eNbFailedRABInfo = NULLP;
   U16                         indx              = 0;
   S16                         ret               = ROK;
   U8                          bearerId          = 0;
   Bool                        brStupFlag        = FALSE;

   VB_TRC2(vbMmeHdlInitialCtxtSetupRsp);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handling Initial Context Setup "\
         "Response"));

   /* Extract the eNB related Bearer Info */
   vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_E_RABSetupLstCtxtSURes,\
         (TknU8 ** )&eNbRABInfo);
   if(eNbRABInfo == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "eNbRABInfo Is NULL"));
      RETVALUE(RFAILED);
   }
 
   for (indx = 0; indx < eNbRABInfo->noComp.val; indx++)
   {
      bearerId = eNbRABInfo->member[indx].value.u.\
            sztE_RABSetupItemCtxtSURes.e_RAB_ID.val;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Finding ESM CB For "\
            "Bearer Id [%d]", bearerId));
      ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Finding ESM CB For "\
               "Bearer Id [%d]", bearerId));
         RETVALUE(RFAILED);
      }

      brStupFlag       = TRUE;
      esmCb->s1EnbTeId = 0;
      VB_MME_DEC_S1AP_TEID(esmCb->s1EnbTeId, eNbRABInfo->member[indx].\
            value.u.sztE_RABSetupItemCtxtSURes.gTP_TEID);
 
      esmCb->s1EnbAddr.type = CM_INET_IPV4ADDR_TYPE; 
      VB_MME_DEC_S1AP_IPADDR(esmCb->s1EnbAddr, eNbRABInfo->member[indx].\
            value.u.sztE_RABSetupItemCtxtSURes.transportLyrAddr);
   }
   
   /* Failed to setup all bearer */
   if (FALSE == brStupFlag)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Bearer Set Up Failed"));
      RETVALUE(RFAILED);
   }

   /* Process failed to setup bearer */
   vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu),\
         Sztid_E_RABFailedToSetupLstCtxtSURes, (TknU8 ** )&eNbFailedRABInfo);
   if (NULLP == eNbFailedRABInfo)
   {
      RETVALUE(ret); 
   } 
    
   for (indx = 0; indx < eNbFailedRABInfo->noComp.val; indx++)
   {
      bearerId = eNbFailedRABInfo->member[indx].value.u.\
            sztE_RABItem.e_RAB_ID.val;

      ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "ESM CB Not Found For "\
               "Bearer Id [%d]", bearerId));
         continue;
      }

      /* Move into delete list */
      vbMmeMoveToEsmCbDelList(ueCb, esmCb);
   }

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHdlInitialCtxtSetupFail
*       
*       Desc:  Function for handling incoming
*              Initial Context Setup Failure from eNodeB.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlInitialCtxtSetupFail
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlInitialCtxtSetupFail(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
   SztCause     *sztCause = NULLP; 
   S16          ret       = ROK;

   VB_TRC2(vbMmeHdlInitialCtxtSetupFail);

   vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause, (TknU8 ** )&sztCause);
   if(NULLP != sztCause)
   {
      cmMemcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
   }
   else
   {
      /* Most common reason would be resource alloc failure */
      ueCb->relCause.choice.val = CAUSE_TRANSPORT; 
      ueCb->relCause.val.transport.val = \
            SztCauseTporttransport_resource_unavailableEnum;  
   }
    
   RETVALUE(ret);  
} /* end of vbMmeHdlInitialCtxtSetupFail */

/*
*
*       Fun:   vbMmeHdlUeCtxtModRsp
*       
*       Desc:  Function for handling incoming
*              Ue Context Modification Response from eNodeB.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlUeCtxtModRsp
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlUeCtxtModRsp(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
   S16   ret        = ROK;

   VB_TRC2(vbMmeHdlUeCtxtModRsp)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
         "Received Ue Context Modification Rsp"));

   RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHdlUeCtxtModFail
*       
*       Desc:  Function for handling incoming
*              Ue Context Mod Failure from eNodeB.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlUeCtxtModFail
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlUeCtxtModFail(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
   SztCause     *sztCause = NULLP; 
   S16          ret       = ROK;

   VB_TRC2(vbMmeHdlUeCtxtModFail)

   vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                                      (TknU8 ** )&sztCause);
   if(NULLP != sztCause)
   {
      cmMemcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
   }
   else
   {
      /* Most common reason would be resource alloc failure */
      ueCb->relCause.choice.val = CAUSE_TRANSPORT; 
      ueCb->relCause.val.transport.val = SztCauseTporttransport_resource_unavailableEnum;  
   }
    
   RETVALUE(ret);  
} /* end of vbMmeHdlUeCtxtModFail */


/* vb001.101: Added new function */
/*
*
*       Fun:   vbMmeHdlERABSetupRsp
*       
*       Desc:  Function for handling incoming
*              E-RAB Setup Response 
*       
*       Ret:   ROK
*       
*       Notes: Handle singe bearer at a time
*              creBid should not be reset here and should be update before 
*              sending E-RAB Setup Request
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlERABSetupRsp
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlERABSetupRsp(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
    SztE_RABSetupLstBrSURes *rspMember = NULLP;
    SztE_RABLst             *failedRspMember = NULLP;
    SztGTP_TEID             *gtpTeId;
    SztTportLyrAddr         *transAddr;
    VbMmeEsmCb              *esmCb;
    U32                     addrMask;
    U16                     teidIdx;
    S16                     ret        = ROK;
    U8                      bearerId;
    U8                      shiftBits;
 
    VB_TRC2(vbMmeHdlERABSetupRsp)
 
    /* Extract the eNB related Address here */
    vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu),Sztid_E_RABSetupLstBrSURes, 
                       (TknU8 ** )&rspMember);

    /* Note: As of now supporting one 1 E-RAB for dedicated bearer support 
     * So either ERAB setup list will present or ERAB failed to setup list */
    if(rspMember != NULLP)
    {
       bearerId = rspMember->member[0].value.u.sztE_RABSetupItemBrSURes.e_RAB_ID.val;

       VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Successful E-RASetup Rsp For Bearer Id(%d)",bearerId));

       if (ROK != vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0))
       {
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Find esmCb From Received Bearer Id(%d)", bearerId));

          /* Abort the procedure */
          vbMmeAbortEsmProc(ueCb);

          RETVALUE(RFAILED);
       }

       gtpTeId = &(rspMember->member[0].value.u.sztE_RABSetupItemBrSURes.gTP_TEID);
       shiftBits = (gtpTeId->len)-1;
       addrMask = 0xFF000000;
       for(teidIdx = 0; teidIdx < (gtpTeId->len); teidIdx++)
       {
          esmCb->s1EnbTeId |= 
             ((U32)(gtpTeId->val[teidIdx] << (8*shiftBits)) & addrMask);
          addrMask = addrMask>>8;
          shiftBits--;
       }

       transAddr = &(rspMember->member[0].value.u.sztE_RABSetupItemBrSURes.
                                     transportLyrAddr);

       /* IPV4 Address only - Length would be in bits */
       if((transAddr->len/8) <= CM_INET_IPV4ADDR_SIZE)
       {
          U16 addrIdx;
          esmCb->s1EnbAddr.type = CM_INET_IPV4ADDR_TYPE; 
          shiftBits = (transAddr->len/8)-1;
          addrMask = 0xFF000000;
          for(addrIdx=0;addrIdx<(transAddr->len/8);addrIdx++)
          {
             esmCb->s1EnbAddr.u.ipv4NetAddr |=
             ((U32)(transAddr->val[addrIdx] << (8*shiftBits)) & addrMask); 
             addrMask = addrMask>>8;
             shiftBits--;
          }
       }
       /* If mPDN procedure was on and we have rxd E-RAB setup rsp after NAS activate dflt bearer accept
        * then we should send mbr towards SGW
        */
       if ( ueCb->proc == VB_MME_PDN_CONN_PROC && esmCb->proc == VB_MME_ACT_DEF_BEAR_PROC )
       {
          if ( ueCb->rxd_dflt_bearer_acc)
          {
             ret = vbMmeSndMBReq (esmCb->ueCb, esmCb, 0);
             if(ret != ROK)
             {
                VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Sending modify bearer Request failed"));
                RETVALUE(RFAILED);

             }
             /* unset the flag for subsequent PDN conn procedures by the UE */
             ueCb->rxd_dflt_bearer_acc = FALSE;
          }
          else
          {
             ueCb->rxd_erab_rsp = TRUE;
          }
       }
    }
    else
    {
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"eNbRABInfo Is NULL"));
       /* Extract the Failed to Setup ERAB list here */
       vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_E_RABFailedToSetupLstBrSURes, 
                      (TknU8 ** )&failedRspMember);

       if(NULLP == failedRspMember)
       {
          /* Abort the procedure */
          vbMmeAbortEsmProc(ueCb);
          RETVALUE(RFAILED);
       }
        
       /* Get bearer Id from failed to erab setup list */
       bearerId = failedRspMember->member[0].value.u.sztE_RABItem.e_RAB_ID.val;

       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failure E-RABSetup Rsp For" 
                                        " Bearer Id(%d)",bearerId));

       if (ROK != vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0))
       {
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Find esmCb From" 
                                            " Received Bearer Id(%d)", bearerId));
          /* Extract the Failed to Setup ERAB list here */
          /* Abort the procedure */
          vbMmeAbortEsmProc(ueCb);
          RETVALUE(RFAILED);
       }

       /* Terminate on going procedure */
       if(VB_MME_ACT_DED_BEAR_PROC == esmCb->proc)
       {
          /* call Create Bearer Response handler to send msg to SGW */
          ret = vbMmeSndCBRsp(esmCb, VB_MME_BEAR_STUP_FAILURE, 
                                              EGT_GTPC_CAUSE_REQ_REJECT); 
          /*stop the timer T3485 */
          vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
          VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/);

          VB_MME_END_ESM_PROC(esmCb);
          /* Remove esmCb from the list */
          ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
       }
       else if(VB_MME_ACT_DEF_BEAR_PROC == esmCb->proc)
       {
          ret = vbMmeSndDSReq(ueCb, esmCb, 0);

          vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
          VB_MME_FREE(esmCb->retrBuf.val, CM_MAX_EMM_ESM_PDU /*esmCb->retrBuf.len*/); 
          /* Donot receive message from UE in error scenario */
          esmCb->state = VB_ESM_ST_BEARER_INACTIVE;

          if(ROK != ret)
          {
             VB_MME_END_ESM_PROC(esmCb);
             vbMmeUtlDelEsmCb(esmCb, ueCb);
          }
          else
          {
             /* Wait For Response */
          }
       }
    }

    RETVALUE(ret);
}


/*
*
*       Fun:   vbMmeHdlERABRlsRsp
*       
*       Desc:  Function for handling incoming
*              E-RAB Release Response 
*       
*       Ret:   ROK
*       
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlERABRlsRsp
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlERABRlsRsp(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
    SztE_RABRlsLstBrRelComp *rspMember = NULLP;
    SztE_RABLst             *failedRspMember = NULLP;
    VbMmeEsmCb              *esmCb;
    S16                     ret = ROK;
    U8                      bearerId;
 
    VB_TRC2(vbMmeHdlERABRlsRsp)
 
    vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu),Sztid_E_RABRlsLstBrRelComp, 
                       (TknU8 ** )&rspMember);

    /* Note: As of now supporting one 1 E-RAB for dedicated bearer support 
     * So either ERAB Release list will present or ERAB failed to release list */
    if(rspMember != NULLP)
    {
       bearerId = rspMember->member[0].value.u.sztE_RABRlsItemBrRelComp.e_RAB_ID.val;

       VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                "Successful E-RABRlsRsp For Bearer Id(%d)",bearerId));

       if (ROK != vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0))
       {
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Find esmCb From Received Bearer Id(%d)", bearerId));
          vbMmeAbortEsmProc(ueCb);
          RETVALUE(RFAILED);
       }
    }
    else
    {
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"eNbRABInfo Is NULL"));
       /* Extract the Failed to Release ERAB list here */
       vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_E_RABFailedToRlsLst, 
                      (TknU8 ** )&failedRspMember);

       if(NULLP == failedRspMember)
       {
          vbMmeAbortEsmProc(ueCb);
          RETVALUE(RFAILED);
       }
        
       /* Get bearer Id from failed to erab release list */
       bearerId = failedRspMember->member[0].value.u.sztE_RABItem.e_RAB_ID.val;

       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failure E-RABRls Rsp For" 
                                        " Bearer Id(%d)",bearerId));

       if (ROK != vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0))
       {
          VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Find esmCb From" 
                                     " Received Bearer Id(%d)", bearerId));
          vbMmeAbortEsmProc(ueCb);
          RETVALUE(RFAILED);
       }
    }

    RETVALUE(ret);
}

/*
*
*       Fun:   vbMmeHdlERABRelInd
*       
*       Desc:  Function for handling incoming
*              E-RAB Release Indication msg 
*       
*       Ret:   ROK
*       
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlERABRelInd
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlERABRelInd(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
   SztE_RABLst    *relList = NULLP;
   VbMmeEsmCb      *esmCb = NULLP;
   S16                     ret        = ROK;
   U8                      bearerId = 0;  /*KWORKS Fix*/

   VB_TRC2(vbMmeHdlERABRelInd)

   vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu),Sztid_E_RABRlsdLst, 
                      (TknU8 ** )&relList);

   /* Currently supporting one dedicated bearer */
   if(relList != NULLP)
   {
      bearerId = relList->member[0].value.u.sztE_RABItem.e_RAB_ID.val;

      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
             "Received ERAB Release Ind For Bearer Id(%d)",bearerId));

      if (ROK != vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0))
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Find esmCb " 
                                          "From Received Bearer Id(%d)", bearerId));
         vbMmeAbortEsmProc(ueCb);
         RETVALUE(RFAILED);
      }
   }
   vbMmeSendDeleteBearerCmd(esmCb); 

   /* Change the state of UeCb->proc so that
      it does not make any conflict with Ue initiated bearer release procedure 
      once it receive Delete bearer req for enodeb initiated erab release indication procedure */
   ueCb->proc = VB_MME_BEAR_REL_IND_PROC;
   
   /* set state of esmCb->proc */
   esmCb->proc = VB_MME_BEAR_REL_PROC;

   RETVALUE(ret);
}

/* vb003.101: Added functions */
#ifdef UE_RAD_CAP
/*
*
*       Fun:   vbMmeHdlUeCapInfoInd
*       
*       Desc:  Function for handling incoming
*              Ue Capabality Info Ind msg. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlUeCapInfoInd
(
VbMmeUeCb      *ueCb,
SztDatEvntInd  *datEvnt
)
#else
PUBLIC S16 vbMmeHdlUeCapInfoInd(ueCb, datEvnt)
VbMmeUeCb      *ueCb;
SztDatEvntInd  *datEvnt;
#endif
{
    SztUERadioCapblty *rspMember = NULLP;
    S16               ret        = ROK;
 
 
    VB_TRC2(vbMmeHdlUeCapInfoInd)
 
    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Ue Capability Info Ind Message Is Received"));
 
    /* Extract the eNB related Address here */
    vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_UERadioCapblty, 
                       (TknU8 ** )&rspMember);

    if(NULLP == rspMember)
    {
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "UE Radio Capability Is NULL"));
        RETVALUE(RFAILED);
    }
    /* if Octet Str Already Exists , remove and Add new */
    if(ueCb->ueCapbInfo.pres)
    {
        SPutSBuf(vbMmeCb.init.region, vbMmeCb.init.pool, 
          (Data *)ueCb->ueCapbInfo.val, ueCb->ueCapbInfo.len);
        ueCb->ueCapbInfo.val = NULLP; 
        ueCb->ueCapbInfo.len = 0; 
    }

    ueCb->ueCapbInfo.pres = TRUE;
    ueCb->ueCapbInfo.len = rspMember->len; 
 
    VB_MME_ALLOC(&ueCb->ueCapbInfo.val, rspMember->len);

    if(NULLP == (ueCb->ueCapbInfo.val))
    {
         /* Debug print here */
         RETVALUE(RFAILED);
    }
    cmMemcpy((U8*)ueCb->ueCapbInfo.val,(U8*)rspMember->val,
                rspMember->len);
 
    RETVALUE(ret);
}
#endif

/*
*
*       Fun:   vbMmeHdlIncUeRelMsg
*       
*       Desc:  Entry function for handling selective 
*              incoming S1AP release messages for UEs
*              without EMM-ESM interpretations.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlIncUeRelMsg
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHdlIncUeRelMsg(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType;
#endif
{
   SztDatEvntInd  *datEvnt  = NULLP;
   SztCause       *sztCause = NULLP;
   S16            ret       = RFAILED;

   VB_TRC2(vbMmeHdlIncUeRelMsg);

 
   switch(msgType)
   {
      case Sztid_UECntxtRls: /* UE Context Release Complete */
      case Sztid_ErrInd:
      {
         if (Sztid_UECntxtRls == msgType)
         {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received UE Context Release "\
               "Complete For UE(%d)", (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received UE Context Release "\
               "Complete For UE(%ld)", (ueCb->ueCbKey)));
#endif
         }
         else
         {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
              "Received Error Indication For UE(%d)", (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
              "Received Error Indication For UE(%ld)", (ueCb->ueCbKey)));
#endif
         }

         VB_MME_END_EMM_PROC(ueCb);

         /* vb001.101: Based on the cause take action */
         if((CAUSE_NAS == ueCb->relCause.choice.val) && 
           ((ueCb->relCause.val.nas.val == SztCauseNasdetachEnum) ||
           (ueCb->relCause.val.nas.val == \
           SztCauseNasauthentication_failureEnum)))
         {
            ret = vbMmeUtlDelUeCb(ueCb);
         }  
         else if (((CAUSE_RADIONW == ueCb->relCause.choice.val) && \
            (ueCb->relCause.val.radioNw.val == \
            SztCauseRadioNwuser_inactivityEnum)) ||
            ((CAUSE_NAS == ueCb->relCause.choice.val) && \
            (ueCb->relCause.val.nas.val == SztCauseNasnormal_releaseEnum)))
         {
            /* if the cause is because of user inactivity donot delete
             * the Ue Control block as we have to retain the context.
             * Change the State of the corresponding UE in ECM_IDLE 
             * state
             */
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
         else if(CAUSE_NAS == ueCb->relCause.choice.val)
         {
            ret = vbMmeUtlDelUeCb(ueCb);
         }
         /* In case of abnormal error, delete complete ueCb */
         else if((CAUSE_PROTOCOL == ueCb->relCause.choice.val) ||
                 (CAUSE_TRANSPORT == ueCb->relCause.choice.val))
         {
            ret = vbMmeUtlDelUeCb(ueCb); 
         }
         else if (((CAUSE_RADIONW == ueCb->relCause.choice.val) && \
            (ueCb->relCause.val.radioNw.val == \
            SztCauseRadioNwinterrat_redirectionEnum)))
         {
            ret = vbMmeUtlDelUeCb(ueCb); 
         }
         else if ((CAUSE_RADIONW == ueCb->relCause.choice.val) && \
            (ueCb->relCause.val.radioNw.val == \
            SztCauseRadioNwsuccessful_handoverEnum))
         {
            /* Check whether MME is changing in case of HO*/
            if (VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
            {
               ret = vbMmeUtlDelUeCb(ueCb);
            }
            else
            {
               vbMmeUtlDelHOUeCb(ueCb);
               ret = ROK;
            }            
         }
          else if ((CAUSE_RADIONW == ueCb->relCause.choice.val) && \
            (ueCb->relCause.val.radioNw.val == \
            SztCauseRadioNwhandover_cancelledEnum)) 
         {
            vbMmeUtlDelHOUeCb(ueCb);
            ret = ROK;
         }
         else
         {
#ifdef ALIGN_64BIT         
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                   "Unhandled Cause: Choice(%d), Cause(%d)",
                   ueCb->relCause.choice.val, ueCb->relCause.val.nas.val));
#else
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                   "Unhandled Cause: Choice(%d), Cause(%ld)",
                   ueCb->relCause.choice.val, ueCb->relCause.val.nas.val));
#endif

            /* Currently handled as user inactivity */
            VB_MME_END_EMM_PROC(ueCb);

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

         break;
      } /* end of release complete cause */

      case Sztid_UECntxtRlsRqst: /* UE Context Release Request */
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received UE Context Release "\
               "Request For UE(%d)", (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received UE Context Release "\
               "Request For UE(%ld)", (ueCb->ueCbKey)));
#endif
         if (VB_EMM_UE_REG_INIT == ueCb->emmCb.state)
         {
#ifdef ALIGN_64BIT
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Ignoring UE Context Release "\
                  "Request For UE(%d) In REG_INIT State", (ueCb->ueCbKey)));
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Ignoring UE Context Release "\
                  "Request For UE(%ld) In REG_INIT State", (ueCb->ueCbKey)));
#endif             
            RETVALUE(RFAILED);
         }         

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,\
               (TknU8 ** )&sztCause); 
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
#ifdef ALIGN_64BIT         
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Cause IE Retrieval Failure For UE(%d)", ueCb->ueCbKey));
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Cause IE Retrieval Failure For UE(%ld)", ueCb->ueCbKey));
#endif
            sztCause->choice.val  = CAUSE_RADIONW;
            sztCause->val.nas.val = SztCauseRadioNwunspecifiedEnum;
         }
          
         ret = vbMmeInitCtxtRelProc(ueCb, sztCause->choice.val,\
               sztCause->val.nas.val, 1);
         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,  "UE Context Release Procedure"));
         }

         break;
      }  

      default:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Invalid Message(%d)", msgType));
         break;
      }
   } 
  
   RETVALUE(ret);
} /* End of vbMmeHdlIncUeRelMsg */
 

/*
*
*       Fun:   vbMmeHdlIncNASNonDlvryIndMsg
*       
*       Desc:  Entry function for handling NASNonDlvryInd 
*              message for UEs
*              without EMM-ESM interpretations.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlIncNASNonDlvryIndMsg
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHdlIncNASNonDlvryIndMsg(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType;
#endif
{
   SztDatEvntInd  *datEvnt  = NULLP;
   SztCause       *sztCause = NULLP;
   S16            ret       = RFAILED;

   VB_TRC2(vbMmeHdlIncNASNonDlvryIndMsg);

 
   switch(msgType)
   {
      case Sztid_NASNonDlvryInd: 
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received NAS Non Delivery Ind For UE(%d)", (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received NAS Non Delivery Ind For UE(%ld)", (ueCb->ueCbKey)));
#endif
         if(VB_EMM_UE_REG_INIT == ueCb->emmCb.state)
         {
#ifdef ALIGN_64BIT
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Ignoring NAS Non Delivery Ind For UE(%d) In REG_INIT State",
                                                          (ueCb->ueCbKey)));
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Ignoring NAS Non Delivery For UE(%ld) In REG_INIT State",
                                                          (ueCb->ueCbKey)));
#endif             
            RETVALUE(RFAILED);
         }         

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                                            (TknU8 ** )&sztCause); 
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
#ifdef ALIGN_64BIT         
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Cause IE Retrieval Failure For UE(%d)", ueCb->ueCbKey));
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Cause IE Retrieval Failure For UE(%ld)", ueCb->ueCbKey));
#endif
            /*KWORKS Fix*/
#if 0
            sztCause->choice.val  = CAUSE_RADIONW;
            sztCause->val.nas.val = SztCauseRadioNwunspecifiedEnum;
#else
            ueCb->relCause.choice.val = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
#endif
         }
         /*KWORKS Fix*/ 
         ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
                    ueCb->relCause.val.nas.val, 1 /* Need to send RAB Req */);

         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                       "UE Ctxt Release Procedure Failed"));
         }

         break;
      }  

      default:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Invalid Message(%d)", msgType));
         break;
      }
   } 
  
   RETVALUE(ret);
} /* End of vbMmeHdlIncNASNonDlvryIndMsg */
 
/*
*
*       Fun:   vbMmeHdlIncErrIndMsg
*       
*       Desc:  Entry function for handling ErrInd 
*              message for UEs
*              without EMM-ESM interpretations.
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeHdlIncErrIndMsg
(
VbMmeUeCb      *ueCb,
Ptr            *ptr,
U8             msgType
)
#else
PUBLIC S16 vbMmeHdlIncErrIndMsg(ueCb, ptr, msgType)
VbMmeUeCb      *ueCb;
Ptr            *ptr;
U8             msgType;
#endif
{
   SztDatEvntInd  *datEvnt  = NULLP;
   SztCause       *sztCause = NULLP;
   S16            ret       = RFAILED;

   VB_TRC2(vbMmeHdlIncErrIndMsg);

 
   switch(msgType)
   {
      case Sztid_ErrInd: 
      {
#ifdef ALIGN_64BIT      
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received Error Ind For UE(%d)", (ueCb->ueCbKey)));
#else
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received Error Ind For UE(%ld)", (ueCb->ueCbKey)));
#endif
         if(VB_EMM_UE_REG_INIT == ueCb->emmCb.state)
         {
#ifdef ALIGN_64BIT
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Ignoring Error Ind For UE(%d) In REG_INIT State",
                                                          (ueCb->ueCbKey)));
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Ignoring Error Ind For UE(%ld) In REG_INIT State",
                                                          (ueCb->ueCbKey)));
#endif             
            RETVALUE(RFAILED);
         }         

         datEvnt = (SztDatEvntInd*)ptr;
         vbMmeUtlGetS1apIE(&(datEvnt->pdu->pdu), Sztid_Cause,
                                            (TknU8 ** )&sztCause); 
         if(NULLP != sztCause)
         {
            memcpy((U8*)&ueCb->relCause, (U8*)sztCause, sizeof(ueCb->relCause));
         }
         else
         {
#ifdef ALIGN_64BIT         
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Cause IE Retrieval Failure For UE(%d)", ueCb->ueCbKey));
#else
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
              "Cause IE Retrieval Failure For UE(%ld)", ueCb->ueCbKey));
#endif
            /*KWORKS Fix*/
#if 0
            sztCause->choice.val  = CAUSE_RADIONW;
            sztCause->val.nas.val = SztCauseRadioNwunspecifiedEnum;
#else
            ueCb->relCause.choice.val = CAUSE_RADIONW;
            ueCb->relCause.val.nas.val = SztCauseRadioNwunspecifiedEnum;
#endif
         }
         /*KWORKS Fix*/ 
         ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
                    ueCb->relCause.val.nas.val, 1 /* Need to send RAB Req */);

         if(ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, 
                       "UE Ctxt Release Procedure Failed"));
         }

         break;
      }  
      default:
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Invalid Message(%d)", msgType));
         break;
      }
   } 
  
   RETVALUE(ret);
} /* End of vbMmeHdlIncErrIndMsg */

/*
 *
 *       Fun:   vbMmeSndUeCtxtRelCmd
 *       
 *       Desc:  This function sends UE Context Release Command message to
 *              eNodeB on S1AP. 
 *       
 *       Desc:  It doesnt use EMM-ESM interpretations for this message.
 *
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_s1ap_utl.c
 *       
 */

#ifdef ANSI
PUBLIC S16 vbMmeSndUeCtxtRelCmd
(
VbMmeUeCb     *ueCb,
U8            choice, /* UE context release choice value */
U8            cause  /* cause value */
)
#else
PUBLIC S16 vbMmeSndUeCtxtRelCmd(ueCb, choice, cause)
VbMmeUeCb     *ueCb;
U8            choice; /* UE context release choice value */
U8            cause;  /* cause value */
#endif
{
   S1apPdu *ueRlsPdu;
   S16     ret;

   VB_TRC2(vbMmeSndUeCtxtRelCmd);
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending UE Cxt ueId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending UE Cxt ueId(%ld)",ueCb->ueCbKey));
#endif
   
   ret = vbMmeUtlBldUeCxtRlsCmd(ueCb, &ueRlsPdu, choice, cause);
   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Could Not Send The UE Context Request UE(%d)" ,ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Could Not Send The UE Context Request UE(%ld)" ,ueCb->ueCbKey));
#endif
      /* UE Context Release couldnt be built trace here */
      RETVALUE(ret); 
   } 

   /* Start Mobile Reachable Timer */
   if( (cause != SztCauseRadioNwsuccessful_handoverEnum) &&
       (ueCb->eNodebCb->eNbCfg.t3412Tmr))
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
           "Starting Mobile Reachable Timer With Value (%d)" , 
           (ueCb->eNodebCb->eNbCfg.t3412Tmr + VB_TMR_EMM_MOB_REACHABLE_OFFSET)));
#else
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
           "Starting Mobile Reachable Timer With Value (%d)" , 
           (ueCb->eNodebCb->eNbCfg.t3412Tmr + VB_TMR_EMM_MOB_REACHABLE_OFFSET)));
#endif

       vbMmeStartTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE, 
           (ueCb->eNodebCb->eNbCfg.t3412Tmr + VB_TMR_EMM_MOB_REACHABLE_OFFSET));
   }

   /* Delegate it to s1ap layer */
   ret = vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, ueRlsPdu, Sztid_UECntxtRls);
   if(ROK == ret)
   {
      ueCb->ecmCb.state = VB_ECM_IDLE;
   }
   
   RETVALUE(ret);
}


/*
*
*       Fun:   vbMmeSndInitialCtxtSetup
*       
*       Desc:  This function sends Initial Context Setup Request message to
*              eNodeB on S1AP. 
*              It doesnt use EMM-ESM interpretations for this message.
*              This function is not used anymore since we are sending
*              attach accept with init ctxt setup req and so it is built
*              and sent from SndToLower function
*
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme_s1ap_utl.c
*       
*/

#ifdef ANSI
PUBLIC S16 vbMmeSndInitialCtxtSetup
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndInitialCtxtSetup(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
    S1apPdu *initCtxtSetpPdu;
    S16      ret;

    VB_TRC2(vbMmeSndInitialCtxtSetup)

#ifdef ALIGN_64BIT
    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
             "Sending Initial CxtSetup ueId(%d)", ueCb->ueCbKey));
#else
    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
             "Sending Initial CxtSetup ueId(%ld)", ueCb->ueCbKey));
#endif
 
    ret = vbMmeUtlBldInitCxtSetupReq(ueCb,NULLP, &initCtxtSetpPdu);
    if(ROK != ret)
    {
#ifdef ALIGN_64BIT    
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Could Not Send InitialContextSetupReq ueCb(%d)", ueCb->ueCbKey));
#else
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Could Not Send InitialContextSetupReq ueCb(%ld)", ueCb->ueCbKey));
#endif

        /* Init Context Setup Request couldnt be built trace here */
        RETVALUE(ret); 
    } 

    RETVALUE(vbMmeLimSndS1APMsg(ueCb->eNodebCb, ueCb, initCtxtSetpPdu,\
         Sztid_InitCntxtSetup));
}
 
/*
 *
 *       Fun:   vbMmeSndUeCtxtModReq
 *       
 *       Desc:  This function sends UE Context Modification Req message to
 *              eNodeB on S1AP. 
 *       
 *       Desc:  It doesnt use EMM-ESM interpretations for this message.
 *
 *       Ret:   ROK on success
 *              RFAILED on failure
 *       
 *       Notes: None
 *              
 *       File:  vb_mme_s1ap_utl.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndUeCtxtModReq
(
VbMmeUeCb     *ueCb,
CmNasEvnt     *vbEvnt,
SztNAS_PDU    *nasPdu,
Bool          nasPduPres
)
#else
PUBLIC S16 vbMmeSndUeCtxtModReq(ueCb, vbEvnt, nasPdu, nasPduPres)
VbMmeUeCb     *ueCb;
CmNasEvnt     *vbEvnt;
SztNAS_PDU    *nasPdu;
Bool          nasPduPres;
#endif
{
   S16     ret;

   VB_TRC2(vbMmeSndUeCtxtModReq)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending UE Cxt Modification Req For ueid(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Sending UE Cxt Modification REq For ueid(%ld)",ueCb->ueCbKey));
#endif
   
   ret = vbMmeLimSndToLower(ueCb, vbEvnt, nasPdu, Sztid_UECntxtModification, nasPduPres);

   if(ROK != ret)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Could Not Send The UE Context Modification Req UE(%d)" ,ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Could Not Send The UE Context Modification Req UE(%ld)" ,ueCb->ueCbKey));
#endif
      RETVALUE(ret); 
   } 
 
   if(ROK == ret)
   {
      ueCb->proc = VB_MME_UE_CTXT_MOD_PROC;
   }
   
   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Preparation Procedure
 *    Handover Required message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Hanodver Preparation is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOPrep
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
)
{
   VbMmeUeCtxt    ueCtxt       = {0, 0};
   VbMmeEnbCb     *pTgtEnbCb   = NULLP;
   S16            ret          = ROK;
   VbMmeUeCb      *pNewUeCb    = NULLP;

   VB_TRC2(vbMmeHndlHOPrep);

   ret = vbMmeUtlValNStoreHOReqd(pUeCb, &pEvnt->pdu->pdu);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation and Storing of "\
            "Handover Required message"));
   }

   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Genrating Next Hop Key"));
#ifdef VB_MME_AUTH
      ret = vbMmeGenerateNextHop(&pUeCb->ueCtxt.secCtxt,\
            pUeCb->ueCtxt.secCtxt.nxtHop);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Genrating Next Hop Key"));
      }
#endif
   }

   if (ROK == ret)
   {
      if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
      {
         if( pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE )
         {
            /* S1 Handover with MME relocation. Send Fwd Relocation Request
                  * to target MME
                  */
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target MME Details for "\
                  "Handover"));
            ret = vbMmeUtlGetTargetMme(&pUeCb->pHoData->hoTgt.gEnbId.plmnId,
                  &pUeCb->pHoData->hoTgt.mmeTptAddr);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target MME Details "\
                     " for Handover"));
            }
            else
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation"\
                     "Request to target MME over S10 interface"));
               ret = vbMmeSndFRReq(pUeCb, &pUeCb->pHoData->hoTgt.mmeTptAddr);
               if (ROK != ret)
               {
                  VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Reloc"\
                        "Request to MME over S10 interface, FAILURE"));
               }
            }
         }
         else if( pUeCb->pHoData->hoType == VB_EMM_HO_LTE_TO_UTRAN )
         {
            /* inter-RAT Handover with SGSN. Send Fwd Reloc Request to target SGSN*/
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target MME Details for "\
                  "Handover"));
            ret = vbMmeUtlGetTargetSgsn(
                        pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val,
                        &pUeCb->pHoData->hoTgt.sgsnTptAddr);
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target SGSN Details "\
                     " for Handover"));
            }
            else
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation"\
                     "Request to target SGSN over S3 interface"));
               ret = vbMmeSndS3FRReq(pUeCb, &pUeCb->pHoData->hoTgt.sgsnTptAddr);
               if (ROK != ret)
               {
                  VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Reloc"\
                        "Request to SGSN over S3 interface, FAILURE"));
               }
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid Handover Type is"\
                        "Requested"));
            ret = RFAILED;
         }
      }
      else
      {
         /* S1 Handover without MME relocation. Send Handover Request to
          * Target eNodeB
          */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Target eNB Details for "\
               "Handover"));
         ret = vbMmeUtlGetTgtEnbCb(&pUeCb->pHoData->hoTgt.gEnbId,
               &pTgtEnbCb);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Target eNB Details for"\
                  " Handover"));
         }
         else
         {
            vbMmeUtlAllocConId(pTgtEnbCb, &(ueCtxt.suConnId));

            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb on Connection "\
                  "ID [%ld]", ueCtxt.suConnId));

            ret = vbMmeUtlAddUeCb(pTgtEnbCb, &ueCtxt, &pNewUeCb);
            if (ROK == ret)
            {
               vbMmeUtlCopyUeCbData(pNewUeCb, pUeCb);
               pNewUeCb->pHoData->pOldUeCb = pUeCb;
               pNewUeCb->pHoData->pSrcEnbCb = pUeCb->eNodebCb;
               pUeCb->pHoData->pTgtEnbCb = pTgtEnbCb;
               pUeCb->pHoData->pNewUeCb = pNewUeCb;

               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Request "\
                     "to target eNB"));

               ret = vbMmeSndHOReq(pNewUeCb, pTgtEnbCb);
               if (ROK != ret)
               {
                  vbMmeUtlDelUeCb(pNewUeCb);
                  VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Request "\
                        "to target eNB"));
               }
            }
            else
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb on Connection "\
                     "ID [%ld]", ueCtxt.suConnId));
            }
         }
      }
   }

   if (ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Preparation "\
            "Failure message to eNB"));

      vbMmeSndHOPrepFail(pUeCb);
      vbMmeUtlDelHoData(pUeCb);
   }

   RETVALUE(ret); 
} /* vbMmeHndlHOPrep */

/**************************************************************************//**
 *
 * @brief 
 *    Based on Tracking Area Code, this function fetches the target MME's
 *    IP Address for handover. The IP Address is retrieved from Configuration
 *    DB
 *             
 * @param[in]  pPlmnId
 *    PLMN ID of target MME
 * @param[out] pTgtMmeIpAddr
 *    Address of Target MME's IP Address
 * 
 * @return
 *    SUCCESS Target MME IP Address is fetched successfully
 *
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlGetTargetMme
(
VbPlmnId             *pPlmnId,
CmTptAddr            *pTgtMmeIpAddr
)
{
   S16         ret = RFAILED;

   VB_TRC2(vbMmeUtlGetTargetMme);

   if (cmStrncmp(pPlmnId->plmnId, vbMmeCb.mmeCfg.tgtMmePlmnId,\
      VB_PLMN_ID_LEN) == 0)
   {
      *pTgtMmeIpAddr = vbMmeCb.mmeCfg.tgtMmeIpAddr;
      ret = ROK;
   }

   RETVALUE(ret);
} /* vbMmeUtlGetTargetMme */

/**************************************************************************//**
 *
 * @brief 
 *    This function fetches the target SGSN's IP Address for handover. 
 *    The IP Address is retrieved from Configuration
 *    DB
 *             
 * @param[in]  pPlmnId
 *    PLMN ID of target MME
 * @param[out] pTgtMmeIpAddr
 *    Address of Target MME's IP Address
 * 
 * @return
 *    SUCCESS Target MME IP Address is fetched successfully
 *
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlGetTargetSgsn
(
U8*                  pPlmnId,
CmTptAddr            *pTgtSgsnIpAddr
)
{
   S16         ret = RFAILED;

   VB_TRC2(vbMmeUtlGetTargetSgsn);

/*   if (cmStrncmp(pPlmnId, vbMmeCb.mmeCfg.sgsnPlmnId,\
      VB_PLMN_ID_LEN) == 0)
   {
      *pTgtSgsnIpAddr = vbMmeCb.mmeCfg.sgsnIpAddr;
      ret = ROK;
   }*/

   *pTgtSgsnIpAddr = vbMmeCb.mmeCfg.sgsnIpAddr;
   ret = ROK;
   RETVALUE(ret);
} /* vbMmeUtlGetTargetSgsn */


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Request Acknowledge message received from eNodeB
 *    as a part of Resource Allocation procedure.
 *    Sends Forward Relocation Response to MME
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pPdu
 *    Pointer to s1ap connection confirm
 * 
 * @return
 *    SUCCESS  Session Request is sent to PGW successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOResAlloc
(
VbMmeUeCb      *pUeCb,
SztConCfm      *pConCfm
)
{
   S16            ret = ROK;
   S1apPdu        *pPdu = (pConCfm->pdu);
       
   VB_TRC2(vbMmeHndlHOResAlloc);

   if (S1AP_PDU_SUCCESSFULOUTCOME == pPdu->pdu.choice.val)
   {
      /* Successfull Outcome
       * Send Forward Relocation Response with Success cause to MME
       */
      ret = vbMmeHndlHOReqAck(pUeCb, pConCfm);
      if (ROK == ret)
      {
         pUeCb->ueCtxt.s1apConState = VB_EMM_UE_S1AP_CONNECTED;
         pUeCb->ecmCb.state = VB_ECM_CONNECTED;
      }
   }
   else
   {
      /* Unsuccessfull Outcome
       * Send Forwar Relocation Response with Failure cause to MME
       * Send Delete Session Request to SGW if HO with SGW relocation
       */
      ret = vbMmeHndlHOFailure(pUeCb);
   }

   RETVALUE(ret); 
}


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Request Acknowledge Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Handover Request Acknowledge message is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeHndlHOReqAck
(
VbMmeUeCb      *pUeCb,
SztConCfm      *pConCfm
)
{
   S16            ret = ROK;
   S1apPdu        *pPdu = pConCfm->pdu;
       
   VB_TRC2(vbMmeHndlHOReqAck);

   ret = vbMmeUtlValNStoreHOReqAck(pUeCb, pPdu);
   if (ROK == ret)
   {
      pUeCb->spConnId = pConCfm->spConnId;
      if( pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE )
      {
         ret = vbMmeHndlS10HOReqAck(pUeCb);
      }
      else if( pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE )
      {
         ret = vbMmeHndlS3HOReqAck(pUeCb);
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Validation of Handover Request Ack"));
   }

   /* Handling of HO Request Ack failed. Send HO Preparation Failure to
    * src eNB */
   if (ROK != ret)
   {
      if ( VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE) )
      {
         if( pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE )
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Response with Failure Cause to source SGSN"));
         
            ret = vbMmeSndS3FRRsp(pUeCb, EGT_GTPC_CAUSE_REQ_REJECT);
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Response with Failure Cause to source MME"));
         
            ret = vbMmeSndFRRsp(pUeCb, EGT_GTPC_CAUSE_REQ_REJECT);
         }
         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
                  "Response with Failure Cause to source eNB"));
         }
      }
      else
      {
           /* In this case MME is not changing, so MME sends a Handover  
                         preperation failure directly to source eNodeB*/
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Preparation "\
               "Failure to source eNB"));
         ret = vbMmeSndHOPrepFail(pUeCb);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Preparation "\
                  "Failure to source eNB"));
         }
      }

      vbMmeUtlDelHoData(pUeCb);
   }

   RETVALUE(ret); 
} /* vbMmeHndlHOReqAck */

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Request Acknowledge Message received from eNodeB for S10
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
  * 
 * @return
 *    SUCCESS if Handover Request Acknowledge message is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeHndlS10HOReqAck
(
VbMmeUeCb      *pUeCb
)
{
   S16            ret = ROK;
   VbMmeUeCb      *pOldUeCb = NULLP;
   VbMmeEsmCb     *pEsmCb = NULLP;

   VB_TRC2(vbMmeHndlS10HOReqAck);


  if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb into imsi hash table"));
      ret = vbMmeUtlAddUeCbOnImsi(pUeCb->ueCtxt.ueImsi,\
            pUeCb->ueCtxt.ueImsiLen, pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb into imsi "\
               "hash table"));
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Response with Success to MME"));


         ret = vbMmeSndFRRsp(pUeCb, EGT_GTPC_CAUSE_ACC_REQ);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forard Relocation "\
                  "Response with Success to MME"));
         }
         else
         {
            pUeCb->eNodebCb = pUeCb->pHoData->pTgtEnbCb;
         }

         pEsmCb = pUeCb->esmList[0];
         pEsmCb->ueCb->ueTunnelCb->ipAddr.type = CM_INET_IPV4ADDR_TYPE;
         pEsmCb->ueCb->ueTunnelCb->ipAddr.u.ipv4NetAddr = \
               pUeCb->pHoData->ueAddr.u.ipv4NetAddr;
      }
      
   }
   else
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Command "\
            "to source eNB"));

      pOldUeCb = pUeCb->pHoData->pOldUeCb;
      if (pUeCb->pHoData->tgtToSrcTCont.len > 0)
      {
         VB_MME_ALLOC(&pOldUeCb->pHoData->tgtToSrcTCont.pVal,\
               pUeCb->pHoData->tgtToSrcTCont.len);
         if (NULLP != pOldUeCb->pHoData->tgtToSrcTCont.pVal)
         {
            pOldUeCb->pHoData->tgtToSrcTCont.len = pUeCb->pHoData->\
                  tgtToSrcTCont.len;
            cmMemcpy(pOldUeCb->pHoData->tgtToSrcTCont.pVal,
                  pUeCb->pHoData->tgtToSrcTCont.pVal,\
                  pUeCb->pHoData->tgtToSrcTCont.len);
         }
      }

      ret = vbMmeSndHOCmd(pOldUeCb, pUeCb->pHoData->pSrcEnbCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Command "\
               "to source eNB"));
      }
   }
   RETVALUE(ret); 
}

/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Request Acknowledge Message received from eNodeB for S3
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
  * 
 * @return
 *    SUCCESS if Handover Request Acknowledge message is handled successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeHndlS3HOReqAck
(
VbMmeUeCb      *pUeCb
)
{
   S16            ret = ROK;
   VbMmeEsmCb     *pEsmCb = NULLP;

   VB_TRC2(vbMmeHndlS3HOReqAck);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb into imsi hash table"));
   ret = vbMmeUtlAddUeCbOnImsi(pUeCb->ueCtxt.ueImsi,\
         pUeCb->ueCtxt.ueImsiLen, pUeCb);
   if (ROK != ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb into imsi "\
            "hash table"));
   }
   else
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
            "Response with Success to SGSN"));

      ret = vbMmeSndS3FRRsp(pUeCb, EGT_GTPC_CAUSE_ACC_REQ);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Response with Success to SGSN"));
      }
      else
      {
         pUeCb->eNodebCb = pUeCb->pHoData->pTgtEnbCb;
      }
/*
      pEsmCb = pUeCb->esmList[0];
      pEsmCb->ueCb->ueTunnelCb->ipAddr.type = CM_INET_IPV4ADDR_TYPE;
      pEsmCb->ueCb->ueTunnelCb->ipAddr.u.ipv4NetAddr = \
            pUeCb->pHoData->ueAddr.u.ipv4NetAddr;*/
   }
   RETVALUE(ret); 
}


/**************************************************************************//**
 *
 * @brief 
 *    Validates and store Handover Request Acknowledge received from eNodeB
 *    Stores the information received into internal data structures
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pPdu
 *    Pointer to S1AP pdu
 * 
 * @return
 *    SUCCESS if Validation and Storing successful
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlValNStoreHOReqAck
(
VbMmeUeCb      *pUeCb,
S1apPdu        *pPdu
)
{
   S16                        ret            = ROK;
   TknU32                     *ie            = NULLP;
   SztE_RABAdmtdLst           *rabIe         = NULLP;
   VbMmeEsmCb                 *pEsmCb        = NULLP;
   SztTget_ToSrc_TprntCont    *pSztTransCont = NULLP;
       
   VB_TRC2(vbMmeUtlValNStoreHOReqAck);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating Handover Request Ack"));

   /* MME UE S1AP ID */
   vbMmeUtlGetS1apIE(&pPdu->pdu, Sztid_MME_UE_S1AP_ID, (TknU8**)&ie);
   /*KWORKS Fix*/
   if(ie != NULLP)
   {
   pUeCb->mMeUeS1apId = ie->val;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unable to Retrieve MME UE S1AP ID"));
      RETVALUE(RFAILED);
   }

   /* ENB UE S1AP ID */
   vbMmeUtlGetS1apIE(&pPdu->pdu, Sztid_eNB_UE_S1AP_ID, (TknU8**)&ie);
   /*KWORKS Fix*/
   if(ie != NULLP)
   {
   pUeCb->eNbUeS1apId = ie->val;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unable to Retrieve ENB UE S1AP ID"));
   }
   
   /* Handling only for default bearer */
   pEsmCb = pUeCb->esmList[0];

   /* RAB List */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving RAB Admitted List"));
   ret = vbMmeUtlGetS1apIE(&pPdu->pdu, Sztid_E_RABAdmtdLst,\
         (TknU8**)&rabIe);
   /*KWORKS Fix*/
   if ((ROK == ret) && (rabIe != NULLP))
   {
      /* Bearer ID */
      if (pEsmCb->bId == rabIe->member[0].value.u.sztE_RABAdmtdItem.\
         e_RAB_ID.val)
      {
         /* eNB GTP DL IP Address and Teid */
         VB_MME_DEC_S1AP_IPADDR(pEsmCb->s1EnbAddr,\
               rabIe->member[0].value.u.sztE_RABAdmtdItem.transportLyrAddr);
         VB_MME_DEC_S1AP_TEID(pEsmCb->s1EnbTeId,\
               rabIe->member[0].value.u.sztE_RABAdmtdItem.gTP_TEID);

         /* DL Trasport Layaer address and TEID */
         if (0 != rabIe->member[0].value.u.sztE_RABAdmtdItem.dL_gTP_TEID.len)
         {
            VB_MME_DEC_S1AP_IPADDR(pUeCb->pHoData->dlTransportLyrAddr,
                  rabIe->member[0].value.u.sztE_RABAdmtdItem.\
                  dL_transportLyrAddr);
            VB_MME_DEC_U32(pUeCb->pHoData->dlGtpTeid,\
                  rabIe->member[0].value.u.sztE_RABAdmtdItem.dL_gTP_TEID.val);
         }

         /* UL Trasport Layaer address and TEID */
         if (0 != rabIe->member[0].value.u.sztE_RABAdmtdItem.uL_GTP_TEID.len)
         {
            VB_MME_DEC_S1AP_IPADDR(pUeCb->pHoData->ulTransportLyrAddr,
                  rabIe->member[0].value.u.sztE_RABAdmtdItem.uL_TportLyrAddr);
            VB_MME_DEC_U32(pUeCb->pHoData->ulGtpTeid,\
                  rabIe->member[0].value.u.sztE_RABAdmtdItem.uL_GTP_TEID.val);
         }
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Bearer ID Mismatch"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving RAB Admitted List"));
   }

   /* Target to Srouce Transparent container */
   vbMmeUtlGetS1apIE(&pPdu->pdu, Sztid_Tget_ToSrc_TprntCont,\
         (TknU8**)&pSztTransCont);
   VB_MME_ALLOC(&pUeCb->pHoData->tgtToSrcTCont.pVal, pSztTransCont->len);
   if (NULLP != pUeCb->pHoData->tgtToSrcTCont.pVal)
   {
      pUeCb->pHoData->tgtToSrcTCont.len = pSztTransCont->len;
      cmMemcpy(pUeCb->pHoData->tgtToSrcTCont.pVal,
            pSztTransCont->val, pSztTransCont->len);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: VbMmeEUTransCont"));
      ret = RFAILED;
   }

   RETVALUE(ret); 
} /* vbMmeUtlValNStoreHOReqAck */


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Notification Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Handover Notifiy is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHONotif
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
)
{
   S16            ret = ROK;
   VbMmeEsmCb     *pEsmCb = NULLP;
       
   VB_TRC2(vbMmeHndlHONotif);
   
   VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Received Handover Notify from eNodeB"));

   ret = vbMmeUtlValNStoreHONotif(pUeCb, pEvnt);
   if (ROK == ret)
   {
      if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
      {
         if( pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE )
         {
            VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Complete Notification to target MME"));
            ret = vbMmeSndFRCmplNotif(pUeCb);
       
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Forard Relocation "\
                     "Complete Notification Failed"));
            }
         }
         else
         {
            VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Complete Notification to Source SGSN"));
            ret = vbMmeSndS3FRCmplNotif(pUeCb);
       
            if (ROK != ret)
            {
               VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Forard Relocation "\
                     "Complete Notification Failed"));
            }
         }
      }
      else
      {
         VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Modify Bearer Request "\
               "to SGW"));
         pEsmCb = pUeCb->esmList[0];
         ret = vbMmeSndMBReq(pUeCb, pEsmCb, FALSE);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Modify Bearer Request "\
                  "to SGW"));
         }
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Validating Handover Notify"));
   }

   RETVALUE(ret); 
} /* vbMmeHndlHONotif */


/**************************************************************************//**
 *
 * @brief 
 *    Validates and Stores the details of Handover Notify message
 *    received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover Notify is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Handover Notifiy is validated successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlValNStoreHONotif
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
)
{
   S16         ret      = ROK;
       
   VB_TRC2(vbMmeUtlValNStoreHONotif);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Handover "\
         "Notification"));

   /* ECGI */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving TAI"));
   ret = vbUtlGetS1APeCgi(&(pEvnt->pdu->pdu), Sztid_HovrNotification,\
         &pUeCb->ueCtxt.eCgi);
   if( ret != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving ECGI"));
   }

   /*TAI */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving TAI"));
   ret = vbUtlGetS1APTaId(&(pEvnt->pdu->pdu), Sztid_HovrNotification,\
         &pUeCb->ueCtxt.tai);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieving TAI"));
   }
   
   RETVALUE(ret); 
}


/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Request message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pTgtEnbCb
 *    Target eNodeB cb
 * 
 * @return
 *    SUCCESS if Hanodver Request is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOReq
(
VbMmeUeCb      *pUeCb,
VbMmeEnbCb     *pTgtEnbCb
)
{
   S16            ret = ROK;
   S1apPdu        *pHoReq = NULLP;

   VB_TRC2(vbMmeSndHOReq);

   ret = vbMmeUtlBldHOReq(pUeCb, &pHoReq);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Handover Request to "\
            "S1AP Layer"));

      ret = vbMmeLimSndS1APMsg(pTgtEnbCb, pUeCb, pHoReq,\
            Sztid_HovrResourceAllocn);
      if (ROK != ret)
      {
         cmFreeMem((Ptr)pHoReq);
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Handover Request to "\
               "S1AP Layer"));
      }
      else
      {
         pUeCb->pHoData->pTgtEnbCb = pTgtEnbCb;
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Building Handover Request"));
   }

   RETVALUE(ret); 
} /* vbMmeSndHOReq */

/**************************************************************************//**
 *
 * @brief 
 *    Builds Handover Request Message as a part of Handover Resource
 *    allocation Procedure.
 *    Allocates memory for the message
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[out] ppHoReq
 *    Pointer to the address of HO Reqeust message
 * 
 * @return
 *    SUCCESS if Hanodver Request built successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldHOReq
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoReq
)
{
   S1apPdu                       *pPdu                  = NULLP;
   S16                           ret                    = ROK;
   U16                           numComp                = 0;
   U8                            dlBitrate[8]           = {'\0'};
   U8                            ulBitrate[8]           = {'\0'};
   U8                            encAlgo                = 0;
   U8                            intProtAlgo            = 0;
   Data                          val[4]                 = {'\0'};
   Mem                           mem                    = {0, 0};
   VbMmeEsmCb                    *esmCb                 = NULLP;
   VbHssPdnSubCntxt              *pdnCtxt               = NULLP;
   U8                            gTPId[10]              = {'\0'};
   U8                            tptAddr[16]            = {'\0'};
   SztProtIE_Field_HovrRqstIEs   *ie                    = NULLP;
   SztProtIE_SingleCont_E_RABToBeSetupItemHOReqIEs *ie1 = NULLP;
#ifndef VB_MME_NAS_SEC
  /* U8                            nullKenb[CM_EMM_MAX_KENB] = {'\0'};*/
#endif
   U32                           isPrempCap = 0;
   U32                           isPrempVul = 0;
#ifdef VB_MME_NAS_SEC 
   U32                           nonce = 0;  
   U8                            nasSecParams[VB_MME_NAS_SEC_PARAMS_LEN] = {0};
#endif

   VB_TRC2(vbMmeUtlBldHOReq);

   cmMemset((U8*)&mem, 0, sizeof(mem));
   pdnCtxt = &pUeCb->ueCtxt.ueHssCtxt.subCntxt[0]; 

   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem, (Ptr *)&pPdu);
   if (ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation: S1apPdu"));
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(pPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(pPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pPdu->pdu.val.initiatingMsg.procedureCode),\
         Sztid_HovrResourceAllocn);
   vbUtlFillTknU32(&(pPdu->pdu.val.initiatingMsg.criticality),\
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(pPdu->pdu.val.initiatingMsg.value.u.\
         sztHovrRqst.pres),PRSNT_NODEF);

   numComp = 9;
   vbUtlFillTknU16(&(pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.\
         protocolIEs.noComp), numComp);
   if ((cmGetMem(&pPdu->memCp, (numComp * sizeof(SztProtIE_Field_HovrRqstIEs)),
      (Ptr *)&pPdu->pdu.val.initiatingMsg.value.u.
      sztHovrRqst.protocolIEs.member)) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, 1, ERRZERO, "Memory Allocation"\
            "SztProtIE_Field_HovrRqstIEs");
      cmFreeMem((Ptr)pPdu);
      RETVALUE(RFAILED);
   }

   /* MME UE S1AP ID */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[0];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), pUeCb->mMeUeS1apId);

   /* Handover Type */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[1];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_HovrTyp);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   if(pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE)
   {
      vbUtlFillTknU32(&(ie->value.u.sztHovrTyp), SztHovrTypintralteEnum);
   }
   if(pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE)
   {
      vbUtlFillTknU32(&(ie->value.u.sztHovrTyp), SztHovrTyputrantolteEnum);
   }
   /* Cause */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[2];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_Cause);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(ie->value.u.sztCause.choice), 
                          pUeCb->pHoData->hoCause.causeType);
   vbUtlFillTknU32(&(ie->value.u.sztCause.val.radioNw),
                          pUeCb->pHoData->hoCause.cause);

   pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.dl = 10;
   pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.ul = 10;
   /* UE AMBR */
   VB_MME_ENC_U32(pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.dl, dlBitrate);
   VB_MME_ENC_U32(pUeCb->ueCtxt.ueHssCtxt.subUeAMBR.ul, ulBitrate);

   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[3];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_uEaggregateMaxBitrate);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztUEAggMaxBitrate.pres), PRSNT_NODEF);
   vbUtlFillTknStrOSXL(&(ie->value.u.sztUEAggMaxBitrate.\
         uEaggregateMaxBitRateDL), 4, dlBitrate, &pPdu);
   vbUtlFillTknStrOSXL(&(ie->value.u.sztUEAggMaxBitrate.\
         uEaggregateMaxBitRateUL), 4, ulBitrate, &pPdu);
   ie->value.u.sztUEAggMaxBitrate.iE_Extns.noComp.pres = NOTPRSNT;

   /* RAB to Be Setup List (Considering on Default Bearer)  */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[4];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_E_RABToBeSetupLstHOReq);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   esmCb = pUeCb->esmList[0];
   if (NULLP != esmCb)
   {
      vbUtlFillTknU16(&(ie->value.u.sztE_RABToBeSetupLstHOReq.noComp), 1);
      if ((cmGetMem(pPdu,\
         sizeof(SztProtIE_SingleCont_E_RABToBeSetupItemHOReqIEs),\
         (Ptr *)&(ie->value.u.sztE_RABToBeSetupLstHOReq.member)) != ROK))
      {
         cmFreeMem((Ptr)pPdu);
         VB_MME_LOGERROR(ERRCLS_DEBUG, 1, ERRZERO, "Memory Allocation"\
               "SztProtIE_SingleCont_E_RABToBeSetupItemHOReqIEs");
         RETVALUE(RFAILED);
      }

      ie1 = &(ie->value.u.sztE_RABToBeSetupLstHOReq.member[0]);

      vbUtlFillTknU8(&(ie1->pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(ie1->id), Sztid_E_RABToBeSetupItemHOReq);
      vbUtlFillTknU32(&(ie1->criticality), SztCriticalityrejectEnum);
      vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.\
            pres), PRSNT_NODEF);
      
      /* RAB ID */
      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.e_RAB_ID),\
            esmCb->bId);

      /* Transport Address */
      if( pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE )
      {
         VB_MME_ENC_U32(esmCb->s4SgwUAddr.u.ipv4NetAddr, tptAddr);
      }
      else
      {
         VB_MME_ENC_U32(esmCb->s1SgwAddr.u.ipv4NetAddr, tptAddr);
      }
      vbUtlFillTknStrBSXL(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.\
            transportLyrAddr), CM_INET_IPV4ADDR_SIZE, tptAddr, &pPdu);

      /* QoS */
      vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.pres), PRSNT_NODEF);

      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
            e_RABlevelQosParams.qCI), pUeCb->ueCtxt.ueHssCtxt.
            subCntxt[0].qos.qci); 

      vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
            e_RABlevelQosParams.allocationRetentionPriority.pres),
            PRSNT_NODEF);

      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
            e_RABlevelQosParams.allocationRetentionPriority.priorityLvl),
            pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.priLevel);
      if( pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE )
      {
         isPrempCap = pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.preCapbFlg;
         isPrempVul = pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.preVlnbFlg;
      }
      else
      {
         isPrempCap = SztPre_emptionCapbltymay_trigger_pre_emptionEnum;
         isPrempVul = SztPre_emptionVulnerabilitypre_emptableEnum;
      } 
      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
            e_RABlevelQosParams.allocationRetentionPriority.
            pre_emptionCapblty),\
            isPrempCap);

      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
            e_RABlevelQosParams.allocationRetentionPriority.
            pre_emptionVulnerability),
            isPrempVul);
      ie1->value.u.sztE_RABToBeSetupItemHOReq.e_RABlevelQosParams.
         allocationRetentionPriority.iE_Extns.noComp.pres = NOTPRSNT;

      if (pdnCtxt->qos.maxBitRateDL != 0)
      {
         VB_MME_ENC_U32(pdnCtxt->qos.maxBitRateDL, dlBitrate);
         vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.pres), PRSNT_NODEF);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.e_RAB_MaxBitrateDL), 4,
               dlBitrate, &pPdu);
      }

      if (pdnCtxt->qos.maxBitRateUL != 0)
      {
         VB_MME_ENC_U32(pdnCtxt->qos.maxBitRateUL, ulBitrate);
         vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.pres), PRSNT_NODEF);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.e_RAB_MaxBitrateUL), 4,
               ulBitrate, &pPdu);
      }

      if (pdnCtxt->qos.guaraBitRateDL != 0)
      {
         VB_MME_ENC_U32(pdnCtxt->qos.guaraBitRateDL, dlBitrate);
         vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.pres), PRSNT_NODEF);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.e_RAB_GuaranteedBitrateDL), 4,
               dlBitrate, &pPdu);
      }

      if (pdnCtxt->qos.guaraBitRateUL != 0)
      {
         VB_MME_ENC_U32(pdnCtxt->qos.guaraBitRateUL, ulBitrate);
         vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.pres), PRSNT_NODEF);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
               e_RABlevelQosParams.gbrQosInform.e_RAB_GuaranteedBitrateUL), 4,
               ulBitrate, &pPdu);
      }

      ie1->value.u.sztE_RABToBeSetupItemHOReq.e_RABlevelQosParams.
         iE_Extns.noComp.pres = NOTPRSNT;
      if(pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE)
      {
         VB_MME_ENC_U32(esmCb->s1SgwTeId,gTPId);
      }
      else if(pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE)
      {
         VB_MME_ENC_U32(esmCb->s4SgwUTeid, gTPId);
      }
      else
      {
         cmFreeMem((Ptr)pPdu);
         VB_MME_LOGERROR(ERRCLS_DEBUG, 1, ERRZERO, "Invalid "\
               "Handover Type");
         RETVALUE(RFAILED);
      }

      vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemHOReq.
            gTP_TEID), 4, gTPId, &pPdu);
   }

   /* Source to Target Transparent Container */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[5];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_Src_ToTget_TprntCont);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknStrOSXL(&(ie->value.u.sztSrc_ToTget_TprntCont),\
         pUeCb->pHoData->srcToTgtTCont.len,\
         (Data *)pUeCb->pHoData->srcToTgtTCont.pVal, &pPdu);

   /* UE Security Capabilities */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[6];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_UESecurCapabilities);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztUESecurCapabilities.pres), PRSNT_NODEF);

   pUeCb->ueCtxt.ueSecCap.eea0 = 1;
   if (pUeCb->ueCtxt.ueSecCap.eea0 || pUeCb->ueCtxt.ueSecCap.eea1_128 
      || pUeCb->ueCtxt.ueSecCap.eea2_128)
   {
      encAlgo = (U8)(pUeCb->ueCtxt.ueSecCap.eea0 << 8 )| \
            (U8)(pUeCb->ueCtxt.ueSecCap.eea1_128 << 7) | \
            (U8)(pUeCb->ueCtxt.ueSecCap.eea2_128  << 6);
   }
   val[0] = encAlgo;
   val[1] = 0;
   vbUtlFillTknBStr32(&(ie->value.u.sztUESecurCapabilities.\
         encryptionAlgorithms), 2, val);

   pUeCb->ueCtxt.ueSecCap.eia1_128 = 1;
   intProtAlgo = (U8)(pUeCb->ueCtxt.ueSecCap.eia0 << 8 )| \
         (U8)(pUeCb->ueCtxt.ueSecCap.eia1_128 << 7 ) | \
         (U8)(pUeCb->ueCtxt.ueSecCap.eia2_128 << 6 );
   val[0] = intProtAlgo;
   val[1] = 0;
   vbUtlFillTknBStr32(&(ie->value.u.sztUESecurCapabilities.
            integrityProtectionAlgorithms), 2, val);
   ie->value.u.sztUESecurCapabilities.iE_Extns.noComp.pres = NOTPRSNT;

   /* UE Security Context */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.protocolIEs.member[7];
#ifdef VB_MME_NAS_SEC
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_SecurCntxt);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztSecurCntxt.pres), PRSNT_NODEF);
   if(pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE)
   {
      vbUtlFillTknU32(&(ie->value.u.sztSecurCntxt.nextHopChainingCount),\
         pUeCb->ueCtxt.secCtxt.nxtHopChainCnt);
   }
   if(pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE)
   {
      /* In case inter RAT HO in to LTE Next hop chaining count is 0 */
      vbUtlFillTknU32(&(ie->value.u.sztSecurCntxt.nextHopChainingCount),0);
   }
   if(pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE)
   {
      /* Within LTE the NH will be retrieved from EPS MM context and stored 
            * in security context. MM context is received over Forward Relocation 
            * Request  from source eNodeB */
      vbUtlFillTknStrBSXL(&(ie->value.u.sztSecurCntxt.nextHopParameter),\
            VB_NEXT_HOP_SIZE, pUeCb->ueCtxt.secCtxt.nxtHop, &pPdu);
   }
   else
   {
      /* In case of inter RAT HO from UTRAN to EUTRAN, MME will generate 
              KeNB with the received mapped UMTS MM context.Generated KeNB
              is send as NH in the security context. */
      vbUtlFillTknStrBSXL(&(ie->value.u.sztSecurCntxt.nextHopParameter),\
            VB_NEXT_HOP_SIZE, pUeCb->ueCtxt.mappedSecCtxt.enbKey, &pPdu);
   }
#else
   /* This is required only for testing in PAL setup so that
    * S1ap doesnt crib for invalid security key encoding 
    */
#if 0 
   vbUtlFillTknStrBSXL(&(ie->value.u.sztSecurKey), CM_EMM_MAX_KENB,\
         nullKenb, &initCxtSetupReqPdu);
#endif
#endif

#ifdef VB_MME_NAS_SEC
   /* NAS Security parameters to EUTRAN IE */
   if(pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE)
   {
      /* Nonce MME value (4 octets) */
      nonce = pUeCb->ueCtxt.mappedSecCtxt.nonce ;

      nasSecParams[0] = (nonce >> 24) & 0xff;
      nasSecParams[1] = (nonce >> 16) & 0xff;
      nasSecParams[2] = (nonce >> 8) & 0xff;
      nasSecParams[3] = nonce & 0xff;

      pUeCb->ueCtxt.mappedSecCtxt.intAlg = VB_MME_EIA1_128; /* Hardcoding the Int-Prot algo to EIA1 */
      pUeCb->ueCtxt.mappedSecCtxt.encAlg = VB_MME_EEA0; /* Hardcoding the enc-Prot algo to EEA0 */
      /* Ciphering Algo and Integrity Algo (1 octet) */
      nasSecParams[4] = (pUeCb->ueCtxt.mappedSecCtxt.encAlg & 0x07) << 4;
      nasSecParams[4] |= (pUeCb->ueCtxt.mappedSecCtxt.intAlg & 0x07);

      /* Type of security context and NAS KSI value(1 octet)*/
      nasSecParams[5] = (pUeCb->ueCtxt.mappedSecCtxt.tsc & 0x01) << 3;
      nasSecParams[5] |= (pUeCb->ueCtxt.mappedSecCtxt.ksi & 0x07);
      
      ie = &pPdu->pdu.val.initiatingMsg.value.u.sztHovrRqst.\
            protocolIEs.member[8];
      vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(ie->id), Sztid_NASSecurParamstoE_UTRAN);
      vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
      vbUtlFillTknStrOSXL(&(ie->value.u.sztNASSecurParamstoE_UTRAN),\
            VB_MME_NAS_SEC_PARAMS_LEN,(Data *)nasSecParams, &pPdu);
   }
#endif
   ie->value.u.sztSecurCntxt.iE_Extns.noComp.pres = NOTPRSNT;

   *ppHoReq = pPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldHOReq */


/**************************************************************************//**
 *
 * @brief 
 *    Builds Handover Command Message as a part of Handover Preparation
 *    Procedure.
 *    Allocates memory for the message
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[out] ppHoCmd
 *    Pointer to the address of HO Command message
 * 
 * @return
 *    SUCCESS if Hanodver Command built successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlBldHOCmd
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoCmd
)
{
   S16         ret         = ROK;
   S1apPdu     *pPdu       = NULLP;
   Mem         mem         = {0, 0};
   SztProtIE_Field_HovrCmmdIEs                  *ie     = NULLP;
   SztProtIE_SingleCont_E_RABDataFwdingItemIEs  *pRabIe = NULLP;
   U8                            tptAddr[16]            = {'\0'};
   U8         idx = 0;
   U8         nasSecParamsFromEUtran = 0;

   VB_TRC2(vbMmeUtlBldHOCmd);
   VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Building Handover Command Message"));

   cmMemset( (U8*) &mem, 0, sizeof ( mem));
   /* cmMemset((U8 *)&(pPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp)); */

   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem, (Ptr *)&pPdu);
   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(pPdu->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   vbUtlFillTknU8(&(pPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pPdu->pdu.val.successfulOutcome.procedureCode),\
         Sztid_HovrPrep);
   vbUtlFillTknU32(&(pPdu->pdu.val.successfulOutcome.criticality),\
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.pres),\
         PRSNT_NODEF);
/* there are 6 items being filled */
   vbUtlFillTknU16(&(pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.\
         protocolIEs.noComp), 6);
   if ((cmGetMem(&pPdu->memCp, (6 * sizeof(SztProtIE_Field_HovrCmmdIEs)), 
         (Ptr *)&pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.\
         protocolIEs.member)) != ROK)
   {
      cmFreeMem((Ptr)pPdu);
      RETVALUE(RFAILED);
   }

   /* MME UE S1AP ID */
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.protocolIEs.\
         member[idx++];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID),\
         pUeCb->mMeUeS1apId);

   /* ENB UE S1AP ID */
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.protocolIEs.\
         member[idx++];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID),\
         pUeCb->eNbUeS1apId);

   /* Handover Type */ 
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.protocolIEs.\
         member[idx++];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_HovrTyp);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztHovrTyp),pUeCb->pHoData->hoType);
   
#ifdef VB_MME_NAS_SEC

   if( pUeCb->pHoData->hoType == VB_EMM_HO_LTE_TO_UTRAN )
   {
      /* Todo NAS Security Parameters from E-UTRAN */
      ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.protocolIEs.\
            member[idx++];
      vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(ie->id), Sztid_NASSecurParamsfromE_UTRAN);
      vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
      /* NAS security params is combinatin of 4LSB of DL NAS Count used for CK 
      & IK Key generation. Key is subracted by 1 to get the previous DL NAS 
      count.*/
      nasSecParamsFromEUtran = (pUeCb->ueCtxt.secCtxt.dlSeqNmb.seqNmb - 1)&0x0f;
      vbUtlFillTknStrOSXL(&(ie->value.u.sztNASSecurParamsfromE_UTRAN),\
            1,&nasSecParamsFromEUtran, &pPdu);
   }
#endif

   /* RAB Subjected to Forwarding (Only Default Bearer) */
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.protocolIEs.\
         member[idx++];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_E_RABSubjtoDataFwdingLst);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU16(&(ie->value.u.sztE_RABSubjtoDataFwdingLst.noComp), 1);

   if ((cmGetMem(pPdu, sizeof(SztProtIE_SingleCont_E_RABDataFwdingItemIEs), 
         (Ptr *)&ie->value.u.sztE_RABSubjtoDataFwdingLst.member)) !=ROK )
   {
      cmFreeMem((Ptr)pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.\
            protocolIEs.member);
      cmFreeMem((Ptr)pPdu);
      RETVALUE(RFAILED);
   }

   /* RAB ID */
   pRabIe = &(ie->value.u.sztE_RABSubjtoDataFwdingLst.member[0]);
   vbUtlFillTknU8(&(pRabIe->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pRabIe->id), Sztid_E_RABDataFwdingItem);
   vbUtlFillTknU32(&(pRabIe->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(pRabIe->value.u.sztE_RABDataFwdingItem.pres), PRSNT_NODEF);

   vbUtlFillTknU32(&(pRabIe->value.u.sztE_RABDataFwdingItem.e_RAB_ID),\
         pUeCb->esmList[0]->bId);
   /* vbUtlFillTknU8(&(pRabIe->value.u.sztE_RABDataFwdingItem.\
         pres), PRSNT_NODEF); */

   /* DL Tansport Layer Address */
   VB_MME_ENC_U32(pUeCb->esmList[0]->rncIpAddr.u.ipv4NetAddr, tptAddr);
   vbUtlFillTknStrBSXL(&(pRabIe->value.u.sztE_RABDataFwdingItem.dL_transportLyrAddr)
      ,CM_INET_IPV4ADDR_SIZE, tptAddr, &pPdu);

   
   /* DL GTP-TEID */
   vbUtlFillTknStrOSXL(&(pRabIe->value.u.sztE_RABDataFwdingItem.dL_gTP_TEID),\
                             4,(Data*)&pUeCb->esmList[0]->rncDlTeId, &pPdu);
   
   
   /* Target to Source Transparent Container */
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCmmd.protocolIEs.\
         member[idx++];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_Tget_ToSrc_TprntCont);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknStrOSXL(&(ie->value.u.sztTget_ToSrc_TprntCont),\
         pUeCb->pHoData->tgtToSrcTCont.len,\
         (Data *)pUeCb->pHoData->tgtToSrcTCont.pVal, &pPdu);

   *ppHoCmd = pPdu;

   RETVALUE(ret);
} /* vbMmeUtlBldHOCmd */


/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Command message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pEnbCb
 *    Pointer to source eNodeB CB
 * 
 * @return
 *    SUCCESS if Hanodver Command is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOCmd
(
VbMmeUeCb      *pUeCb,
VbMmeEnbCb     *pEnbCb
)
{
   S16            ret = ROK;
   S1apPdu        *pHoCmd = NULLP;

   VB_TRC2(vbMmeSndHOCmd);

   ret = vbMmeUtlBldHOCmd(pUeCb, &pHoCmd);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Handover Command to "\
            "S1AP Layer"));
      ret = vbMmeLimSndS1APMsg(pEnbCb, pUeCb, pHoCmd, Sztid_HovrPrep);
      if (ROK != ret)
      {
         cmFreeMem((Ptr)pHoCmd);
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Handover Command to "\
               "S1AP Layer"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Building Handover Command"));
   }
   
   RETVALUE(ret); 
} /* vbMmeSndHOCmd */

/**************************************************************************//**
 *
 * @brief 
 *    Validates and stores the Handover Required Message as a part
 *    of Hanodver Preparation Procedure
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pPdu
 *    Pointer to S1AP Pdu with HO Required Message
 * 
 * @return
 *    SUCCESS if Hanodver Required message is validate and information is
 *    stored successfully otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlValNStoreHOReqd
(
VbMmeUeCb      *pUeCb,
SztS1AP_PDU    *pPdu
)
{
   S16                        ret            = ROK;
   SztTgetID                  *pSztTgtId     = NULLP;
   VbMmeEnbCb                 *pTgtEnbCb     = NULLP;
   SztSrc_ToTget_TprntCont    *pSztTransCont = NULLP;
   SztHovrTyp                 *pHoType        = NULLP;
   SztCause                   *pCause         = NULLP;

   VB_TRC2(vbMmeUtlValNStoreHOReqd);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Handover Required"\
         " message from eNB"));

   VB_MME_ALLOC(&pUeCb->pHoData, sizeof(VbMmeHoData));
   if (NULLP == pUeCb->pHoData)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: vbMmeHoData"));
      RETVALUE(RFAILED);
   }

   /* Handover Type IE */
   ret = vbMmeUtlGetS1apIE(pPdu, Sztid_HovrTyp, (TknU8 ** )&pHoType);
   pUeCb->pHoData->hoType = pHoType->val;

   if( pHoType->val == SztHovrTypintralteEnum )
   {
      /* get the target eNB id and find whether MME has to be relocated or not */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving Target eNB ID"));
      ret = vbMmeUtlGetS1apIE(pPdu, Sztid_TgetID, (TknU8 ** )&pSztTgtId);
      if (ROK == ret && NULLP != pSztTgtId)
      {
         /* check the eNBs which are connected to this MME with the Target 
              *  eNodeB id. If connected, then MME relocation is not required 
              */
         pUeCb->pHoData->hoMmeType = VB_MME_HO_SRC_MME;
         pUeCb->pHoData->hoTgt.gEnbId.enbId = pSztTgtId->val.targeteNB_ID.\
               global_ENB_ID.eNB_ID;
         cmMemcpy(pUeCb->pHoData->hoTgt.gEnbId.plmnId.plmnId,\
               pSztTgtId->val.targeteNB_ID.global_ENB_ID.pLMNidentity.val,
               pSztTgtId->val.targeteNB_ID.global_ENB_ID.pLMNidentity.len);

         ret = vbMmeUtlGetTgtEnbCb(&pUeCb->pHoData->hoTgt.gEnbId, &pTgtEnbCb);
         if (ROK != ret)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "eNB is NOT managed by this MME"));
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover with MME "\
                     "Relocation Required"));

            VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);
         }
         else
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "eNB is managed by this MME"));
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Handover without MME "\
                     "Relocation Required"));

            VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_MME_CHANGE);
         }

         ret = ROK;
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Retrieving Target eNB ID"));
      }
   }
   else if( pHoType->val == SztHovrTypltetoutranEnum )
   {
      /* Get the RNC ID and find whether SGSN is connected to MME */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving RNC ID"));
      ret = vbMmeUtlGetS1apIE(pPdu, Sztid_TgetID, (TknU8 ** )&pSztTgtId);
      if (ROK == ret && NULLP != pSztTgtId)
      {
         pUeCb->pHoData->hoMmeType = VB_MME_HO_SRC_MME;
         /*LAI*/

         /*PLMN ID */
         VB_MME_ALLOC(&pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val,
            pSztTgtId->val.targetRNC_ID.lAI.pLMNidentity.len);
         if( pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val == NULL )
         {
            VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Allocation Failure"));
            RETVALUE(RFAILED);
         }
         cmMemcpy(pUeCb->pHoData->hoTgt.tgtRncId.laiId.pLMNidentity.val,\
                        pSztTgtId->val.targetRNC_ID.lAI.pLMNidentity.val,\
                        pSztTgtId->val.targetRNC_ID.lAI.pLMNidentity.len);
         /* LAC */
         cmMemcpy(pUeCb->pHoData->hoTgt.tgtRncId.laiId.lAC.val,\
                  pSztTgtId->val.targetRNC_ID.lAI.lAC.val,\
                  pSztTgtId->val.targetRNC_ID.lAI.lAC.len);

         /* RNC ID  */
         pUeCb->pHoData->hoTgt.tgtRncId.rncId = pSztTgtId->val.targetRNC_ID.rNC_ID;
         
         VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);
         ret = ROK;
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Retrieving Target RNC ID"));
      }
   }
   else
   {
       ret = RFAILED;
       VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Handover Type is not supported"));
   }

   if( pUeCb->pHoData->hoType == VB_EMM_HO_LTE_TO_UTRAN )
   /* RAN CAUSE */
   if (ret == ROK)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting RAN Cause for HO"));
      ret = vbMmeUtlGetS1apIE(pPdu, Sztid_Cause, (TknU8 ** )&pCause);
      if (ROK == ret)
      {
         pUeCb->pHoData->hoCause.causeType = pCause->choice.val;
         if( pCause->choice.val == CAUSE_RADIONW )
         {
            pUeCb->pHoData->hoCause.cause = pCause->val.radioNw.val;
         }
         else if( pCause->choice.val == CAUSE_TRANSPORT )
         {
            pUeCb->pHoData->hoCause.cause = pCause->val.transport.val;
         }
         else if( pCause->choice.val == CAUSE_NAS)
         {
            pUeCb->pHoData->hoCause.cause = pCause->val.nas.val;
         }
         else if( pCause->choice.val == CAUSE_PROTOCOL)
         {
            pUeCb->pHoData->hoCause.cause = pCause->val.protocol.val;
         }
         else if( pCause->choice.val == CAUSE_MISC)
         {
            pUeCb->pHoData->hoCause.cause = pCause->val.misc.val;
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting RAN cause for HO"));
            ret = RFAILED;
         }
         
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting RAN cause for HO"));
      }
    }
   
   /* Source to Target Transparent container */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting Src to Tgt Transparent "\
            "Container"));
      ret = vbMmeUtlGetS1apIE(pPdu, Sztid_Src_ToTget_TprntCont,\
            (TknU8 ** )&pSztTransCont);
      if (ROK == ret && pSztTransCont != NULLP)
      {
         VB_MME_ALLOC(&pUeCb->pHoData->srcToTgtTCont.pVal, pSztTransCont->len);
         if (NULLP == pUeCb->pHoData->srcToTgtTCont.pVal)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: "\
                  "VbMmeEUTransCont"));
            ret = RFAILED;
         }
         else
         {
            pUeCb->pHoData->srcToTgtTCont.len = pSztTransCont->len;
            cmMemcpy(pUeCb->pHoData->srcToTgtTCont.pVal,
                  pSztTransCont->val, pSztTransCont->len);
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting Src to Tgt Transparent "\
               "Container"));
      }

      
   }

   
   RETVALUE(ret); 
} /* vbMmeUtlValNStoreHOReqd */

/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Preparation Failure message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Hanodver Preparation is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOPrepFail
(
VbMmeUeCb      *pUeCb
)
{
   S16            ret = ROK;
   S1apPdu        *pHoPrepFail = NULLP;

   VB_TRC2(vbMmeSndHOPrepFail);

   ret = vbMmeUtlBldHOPrepFail(pUeCb, &pHoPrepFail);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Handover Preparation "\
            "Failure to S1AP Layer"));
      ret = vbMmeLimSndS1APMsg(pUeCb->eNodebCb, pUeCb, pHoPrepFail,\
            Sztid_HovrPrep);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Handover Preparation "\
               "Failure to S1AP Layer"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Building Handover Prepration "\
            "Failure"));
   }
   
   if (ROK != ret)
   {
      if (NULL != pHoPrepFail)
      {
         cmFreeMem((Ptr)pHoPrepFail);
      }
   }

   RETVALUE(ret); 
} /* vbMmeSndHOPrepFail */


/**************************************************************************//**
 *
 * @brief 
 *    Builds Handover Preparation Failure Message 
 *    Allocates memory for the message
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[out] ppHoPrepFail
 *    Pointer to the address of HO Prep Failure message
 * 
 * @return
 *    SUCCESS if Hanodver Preparation Failure built successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlBldHOPrepFail
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoPrepFail
)
{
   U16         numComp = 0;
   S16         ret     = ROK;
   S1apPdu     *pPdu   = NULLP;
   Mem         mem     = {0, 0};
   SztProtIE_Field_HovrPrepFailIEs *ie = NULLP;

   VB_TRC2(vbMmeUtlBldHOPrepFail);

   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem, (Ptr *)&pPdu);
   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: S1apPdu"));
      RETVALUE(RFAILED);
   }
    
   cmMemset((U8 *)&(pPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(pPdu->pdu.choice), S1AP_PDU_UNSUCCESSFULOUTCOME);
   vbUtlFillTknU8(&(pPdu->pdu.val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pPdu->pdu.val.unsuccessfulOutcome.procedureCode),\
         Sztid_HovrPrep);
   vbUtlFillTknU32(&(pPdu->pdu.val.unsuccessfulOutcome.criticality),\
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(pPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail.\
         pres), PRSNT_NODEF);

   numComp = 3;
   vbUtlFillTknU16(&(pPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail.\
         protocolIEs.noComp), numComp);
   if ((cmGetMem(pPdu, (numComp * sizeof(SztProtIE_Field_HovrPrepFailIEs)), 
         (Ptr *)&pPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail.\
         protocolIEs.member)) != ROK)
   {
      cmFreeMem((Ptr)pPdu);
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: "\
            "SztProtIE_Field_HovrPrepFailIEs"));
      ret = RFAILED;
   }

   /* MME UE S1AP ID */
   ie = &pPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail.\
         protocolIEs.member[0];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), pUeCb->mMeUeS1apId);

   /* eNB UE S1AP ID */
   ie = &pPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail.\
         protocolIEs.member[1];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), pUeCb->eNbUeS1apId);

   /* Cause */
   ie = &pPdu->pdu.val.unsuccessfulOutcome.value.u.sztHovrPrepFail.\
         protocolIEs.member[2];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_Cause);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(ie->value.u.sztCause.choice), CAUSE_PROTOCOL);
   vbUtlFillTknU32(&(ie->value.u.sztCause.val.protocol),\
         SztCauseProtunspecifiedEnum);

   *ppHoPrepFail = pPdu;

   RETVALUE(ret);
} /* vbMmeUtlBldHOPrepFail */


/**************************************************************************//**
 *
 * @brief 
 *    Handles eNB status transfer message 
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for enb status transfer is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if eNB status transfer is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndleNBStatusTfr
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
)
{
   S16            ret = ROK;
   VbMmeUeCb      *pNewUeCb = NULLP;
       
   VB_TRC2(vbMmeHndleNBStatusTfr);
   
   VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Received eNB Status Transfer"));

   ret = vbMmeUtlValNStoreEnbStatusTfr(pUeCb, pEvnt->pdu);
   if (ROK == ret)
   {
      if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
      {
         VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Forward Acceess "\
               "Context Notification to target MME"));
         ret = vbMmeSndFACtxNotif(pUeCb, &pUeCb->pHoData->hoTgt.mmeTptAddr);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Forard Access "\
                  "Context Notification Failed"));
         }
      }
      else
      {
         VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending eNB Status Transfer "\
               "to target eNB"));

         pNewUeCb = pUeCb->pHoData->pNewUeCb;
         if (pUeCb->pHoData->staTfrTCont.len > 0)
         {
            VB_MME_ALLOC(&pNewUeCb->pHoData->staTfrTCont.pVal,\
                  pUeCb->pHoData->staTfrTCont.len);
            if (NULLP != pNewUeCb->pHoData->staTfrTCont.pVal)
            {
               pNewUeCb->pHoData->staTfrTCont.len = \
                     pUeCb->pHoData->staTfrTCont.len;
               cmMemcpy(pNewUeCb->pHoData->staTfrTCont.pVal,
                     pUeCb->pHoData->staTfrTCont.pVal,\
                     pUeCb->pHoData->staTfrTCont.len);

               ret = vbMmeSndMmeStatusTfr(pNewUeCb, pNewUeCb->eNodebCb);
               if (ROK != ret)
               {
                  VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending eNB Status "\
                        "Transfer to target eNB"));
               }
            }
            else
            {
               VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Allocating Memory: eNB "\
                     "Status transfer Transparent Container"));
            }
         }
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Validating eNB Status Transfer"));
   }

   RETVALUE(ret); 
} /* vbMmeHndleNBStatusTfr */


/**************************************************************************//**
 *
 * @brief 
 *    Handles UE Context Release request received Midway of Handover
 *    Procedure
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for enb status transfer is received
 * 
 * @return
 *    SUCCESS if UE Context Release comand is send successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndUeCtxRlsCmdDuringiRATHO
(
VbMmeUeCb      *pUeCb
)
{
   S16            ret        = ROK;
       
   VB_TRC2(vbMmeSndUeCtxRlsCmdDuringiRATHO);
   
   pUeCb->relCause.choice.val  = CAUSE_RADIONW;
   pUeCb->relCause.val.radioNw.val = SztCauseRadioNwsuccessful_handoverEnum;

   vbMmeUtlDelS3Tun(pUeCb);
         
   ret = vbMmeInitCtxtRelProc(pUeCb, pUeCb->relCause.choice.val,\
               pUeCb->relCause.val.radioNw.val, 0);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Initial Context "\
            "Setup Release Procedure"));

      VB_MME_END_EMM_PROC(pUeCb);
      vbMmeUtlDelHoData(pUeCb);
   }
   RETVALUE(ret); 
} /* vbMmeHndlUeCtlRlsReqDuringHO */

/**************************************************************************//**
 *
 * @brief 
 *    Sends UE Context  command to Source eNodeB after iRAT Handover
 *    Procedure is successful
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for enb status transfer is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if UE Context Release Request is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlUeCtxRlsReqDuringHO
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
)
{
   S16            ret        = ROK;
   SztCause       *pSztCause = NULLP;
       
   VB_TRC2(vbMmeHndlUeCtxRlsReqDuringHO);
   
   VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Handling UE Context Release Request "\
         "During Handover"));

   ret = vbMmeUtlGetS1apIE(&(pEvnt->pdu->pdu), Sztid_Cause,\
         (TknU8 ** )&pSztCause);
   if ((ROK == ret) && (pSztCause != NULLP))
   {
      cmMemcpy((U8*)&pUeCb->relCause, (U8 *)pSztCause, sizeof(SztCause));
   }
   else
   {
      pUeCb->relCause.choice.val  = CAUSE_RADIONW;
      pUeCb->relCause.val.radioNw.val = SztCauseRadioNwunspecifiedEnum;
   }
#ifdef X2_HO
   if(pUeCb->hoType == VB_MME_X2_HO)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Initial Context "\
                  "Setup Release Procedure After X2HO peerId(%d)",
                  pUeCb->eNodebCb->peerId));
      ret = vbMmeInitCtxtRelProc(pUeCb, pUeCb->relCause.choice.val,\
            pUeCb->relCause.val.radioNw.val, 0);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Initial Context "\
                  "Setup Release Procedure"));

         VB_MME_END_EMM_PROC(pUeCb);
      }
      RETVALUE(ROK); 
   }
#endif
   if (VB_MME_HO_TGT_MME == pUeCb->pHoData->hoMmeType)
   {
      /* Abort Handover Procedure, Delete S10 Tunnel, Delete
       * PDN Connections and GTP Sessions SGW 
       */
      vbMmeAbortHOProc(pUeCb);
   }
   else
   {
      /* Applicable for Handover without MME relocation
       * OR
       * UE Context Release received due to radio interface procedure
       * failures
       */
      if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Initiating Initial Context "\
               "Setup Release Procedure"));

         if ((NULLP != pUeCb->pUeS10TunCb) && \
            (pUeCb->pUeS10TunCb->remoteTeId != 0))
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Relocation Cancel "\
                  "Request to target MME"));
            ret = vbMmeSndRCReq(pUeCb);
            if (ROK != NULLP)
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Relocation Cancel"\
                     " Request to target MME"));
            }

            vbMmeUtlDelS10Tun(pUeCb);
         }

         ret = vbMmeInitCtxtRelProc(pUeCb, pUeCb->relCause.choice.val,\
               pUeCb->relCause.val.radioNw.val, 0);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Initial Context "\
                  "Setup Release Procedure"));

            VB_MME_END_EMM_PROC(pUeCb);
            vbMmeUtlDelHoData(pUeCb);
         }
      }
      else
      {
         /* TODO */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TODO: Handling of UE Context "\
               "Release Request. Handover without MME Relocation"));
      }
   }

   RETVALUE(ret); 
} /* vbMmeHndlUeCtlRlsReqDuringHO */



/**************************************************************************//**
 *
 * @brief 
 *    Validates and store eNB Status Transfer
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which enB Status transfer is received
 * @param[in]  pPdu
 *    Pointer to S1AP pdu
 * 
 * @return
 *    SUCCESS if Validation and Storing successful
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlValNStoreEnbStatusTfr
(
VbMmeUeCb      *pUeCb,
S1apPdu        *pPdu
)
{
   S16                        ret            = ROK;
   SztENB_StatusTfr_TprntCont *pSztTransCont = NULLP;
       
   VB_TRC2(vbMmeUtlValNStoreEnbStatusTfr);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating eNB Status Transfer"));

   /* Target to Srouce Transparent container */
   vbMmeUtlGetS1apIE(&pPdu->pdu, Sztid_eNB_StatusTfr_TprntCont,\
         (TknU8**)&pSztTransCont);
   VB_MME_ALLOC(&pUeCb->pHoData->staTfrTCont.pVal,\
         sizeof(SztBrs_SubjToStatusTfr_Item));
   /*KWORKS Fix*/
   if ((NULLP != pUeCb->pHoData->staTfrTCont.pVal) && (pSztTransCont != NULLP))
   {
      pUeCb->pHoData->staTfrTCont.len = sizeof(SztBrs_SubjToStatusTfr_Item);
      cmMemcpy(pUeCb->pHoData->staTfrTCont.pVal,
            (U8 *)&(pSztTransCont->bearers_SubjToStatusTfrLst.member[0].\
            value.u.sztBrs_SubjToStatusTfr_Item),\
            pUeCb->pHoData->staTfrTCont.len);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: "\
            "VbMmeStaTfrTransCont"));
      ret = RFAILED;
   }

   RETVALUE(ret); 
} /* vbMmeUtlValNStoreEnbStatusTfr */


/**************************************************************************//**
 *
 * @brief 
 *    Sends eNB Status Transfer message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[in]  pTgtEnbCb
 *    Target eNodeB cb
 * 
 * @return
 *    SUCCESS if eNB Status Transfer is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndMmeStatusTfr
(
VbMmeUeCb      *pUeCb,
VbMmeEnbCb     *pTgtEnbCb
)
{
   S16                           ret = ROK;
   S1apPdu                       *pStaTfr = NULLP;

   VB_TRC2(vbMmeSndMmeStatusTfr);

   ret = vbMmeUtlBldMmeStatusTfr(pUeCb, &pStaTfr);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending MME Status Transfer to "\
            "S1AP Layer"));

      ret = vbMmeLimSndS1APMsg(pTgtEnbCb, pUeCb, pStaTfr,\
            Sztid_eNBStatusTfr);
      if (ROK != ret)
      {
         cmFreeMem((Ptr)pStaTfr);
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending eNB Status Transfer to "\
               "S1AP Layer"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Building eNB Status Transfer"));
   }

   RETVALUE(ret); 
} /* vbMmeSndMmeStatusTfr */


/**************************************************************************//**
 *
 * @brief 
 *    Builds ENB Status Transfer Message
 *    Allocates memory for the message
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * @param[out] ppEnbStaTfr
 *    Pointer to the address of eNB Status Transfer
 * 
 * @return
 *    SUCCESS if eNB Status Transfer built successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldMmeStatusTfr
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppEnbStaTfr
)
{
   S1apPdu                       *pPdu                  = NULLP;
   S16                           ret                    = ROK;
   U16                           numComp                = 0;
   Mem                           mem                    = {0, 0};
   SztProtIE_Field_MMEStatusTfrIEs *ie                  = NULLP;
   SztENB_StatusTfr_TprntCont    *trCont                = NULLP;
   SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs *trItem = NULLP;
   SztBrs_SubjToStatusTfr_Item   *brsTfrItem            = NULLP;

   VB_TRC2(vbMmeUtlBldMmeStatusTfr);

   cmMemset((U8*)&mem, 0, sizeof(mem));

   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem, (Ptr *)&pPdu);
   if (ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Memory Allocation: S1apPdu"));
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(pPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(pPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pPdu->pdu.val.initiatingMsg.procedureCode),\
         Sztid_MMEStatusTfr);
   vbUtlFillTknU32(&(pPdu->pdu.val.initiatingMsg.criticality),\
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(pPdu->pdu.val.initiatingMsg.value.u.\
         sztENBStatusTfr.pres),PRSNT_NODEF);

   numComp = 3;
   vbUtlFillTknU16(&(pPdu->pdu.val.initiatingMsg.value.u.sztENBStatusTfr.\
         protocolIEs.noComp), numComp);
   if ((cmGetMem(&pPdu->memCp,\
      (numComp * sizeof(SztProtIE_Field_MMEStatusTfrIEs)),
      (Ptr *)&pPdu->pdu.val.initiatingMsg.value.u.
      sztHovrRqst.protocolIEs.member)) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, 1, ERRZERO, "Memory Allocation"\
            "SztProtIE_Field_MMEStatusTfrIEs");
      cmFreeMem((Ptr)pPdu);
      RETVALUE(RFAILED);
   }

   /* MME UE S1AP ID */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztMMEStatusTfr.\
         protocolIEs.member[0];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), pUeCb->mMeUeS1apId);

   /* eNB UE S1AP ID */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztMMEStatusTfr.\
         protocolIEs.member[1];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), pUeCb->eNbUeS1apId);

   /* Transparent Container */
   ie = &pPdu->pdu.val.initiatingMsg.value.u.sztMMEStatusTfr.\
         protocolIEs.member[2];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_StatusTfr_TprntCont);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztENB_StatusTfr_TprntCont.pres), PRSNT_NODEF); 
   ie->value.u.sztENB_StatusTfr_TprntCont.iE_Extns.noComp.pres = NOTPRSNT;

   trCont = &ie->value.u.sztENB_StatusTfr_TprntCont;
   trCont->bearers_SubjToStatusTfrLst.noComp.pres = PRSNT_NODEF;
   trCont->bearers_SubjToStatusTfrLst.noComp.val = 1;

   if ((cmGetMem(pPdu,\
      sizeof(SztProtIE_SingleCont_Brs_SubjToStatusTfr_ItemIEs),\
      (Ptr *)&(trCont->bearers_SubjToStatusTfrLst.member)) != ROK))
   {
      cmFreeMem((Ptr)pPdu);
      RETVALUE(RFAILED);
   }

   trItem = trCont->bearers_SubjToStatusTfrLst.member;

   vbUtlFillTknU8(&(trItem->pres), PRSNT_NODEF);  
   vbUtlFillTknU32(&(trItem->id), Sztid_Brs_SubjToStatusTfr_Item);   
   vbUtlFillTknU32(&(trItem->criticality), SztCriticalityignoreEnum);   
   vbUtlFillTknU8(&(trItem->value.u.sztBrs_SubjToStatusTfr_Item.pres),\
         PRSNT_NODEF);

   brsTfrItem = (SztBrs_SubjToStatusTfr_Item *)pUeCb->pHoData->staTfrTCont.pVal;

   /* RAB ID */
   vbUtlFillTknU32(&trItem->value.u.sztBrs_SubjToStatusTfr_Item.e_RAB_ID,\
         brsTfrItem->e_RAB_ID.val);

   /* UL Count Value */
   vbUtlFillTknU8(&(trItem->value.u.sztBrs_SubjToStatusTfr_Item.\
         uL_COUNTvalue.pres), PRSNT_NODEF);  
   trItem->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.iE_Extns.\
         noComp.pres = NOTPRSNT;
   vbUtlFillTknU32(&trItem->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.\
         hFN, brsTfrItem->uL_COUNTvalue.hFN.val);
   vbUtlFillTknU32(&trItem->value.u.sztBrs_SubjToStatusTfr_Item.uL_COUNTvalue.\
         pDCP_SN, brsTfrItem->uL_COUNTvalue.pDCP_SN.val);

   /* DL Count Value */
   vbUtlFillTknU8(&(trItem->value.u.sztBrs_SubjToStatusTfr_Item.\
         dL_COUNTvalue.pres), PRSNT_NODEF);  
   trItem->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.iE_Extns.\
         noComp.pres = NOTPRSNT;
   vbUtlFillTknU32(&trItem->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.\
         hFN, brsTfrItem->dL_COUNTvalue.hFN.val);
   vbUtlFillTknU32(&trItem->value.u.sztBrs_SubjToStatusTfr_Item.dL_COUNTvalue.\
         pDCP_SN, brsTfrItem->dL_COUNTvalue.pDCP_SN.val);

   *ppEnbStaTfr = pPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldMmeStatusTfr */


/**************************************************************************//**
 *
 * @brief 
 *    Handles Handover Cancel Message received from eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover Cancel is received
 * @param[in]  pEvnt
 *    Pointer to S1AP event
 * 
 * @return
 *    SUCCESS if Handover Cancel is hanlded successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOCancel
(
VbMmeUeCb      *pUeCb,
SztDatEvntInd  *pEvnt
)
{
   S16            ret       = ROK;
   VbMmeUeCb      *pNewUeCb = NULLP;
       
   VB_TRC2(vbMmeHndlHOCancel);
   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Received Handover Cancel from eNodeB"));

   if (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Relocation Cancel Request "\
            "to target MME"));
      ret = vbMmeSndRCReq(pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Relocation Cancel Request "\
               "to target MME"));
      }
      else
      {
         VB_MME_END_EMM_PROC(pUeCb);
         vbMmeUtlDelHoData(pUeCb);
         vbMmeUtlDelS10Tun(pUeCb);

         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Cancel "\
               "Acknowledge to eNB"));
         ret = vbMmeSndHOCancelAck(pUeCb);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Cancel "\
                  "Acknowledge to eNB"));
         }
      }
   }
   else
   {
      VB_MME_END_EMM_PROC(pUeCb);
      pNewUeCb = pUeCb->pHoData->pNewUeCb;
      vbMmeUtlDelHoData(pUeCb);

      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Cancel "\
               "Acknowledge to eNB"));
      ret = vbMmeSndHOCancelAck(pUeCb);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Cancel "\
                  "Acknowledge to eNB"));
      }

      /* Initiate UE Context Release procedure with the target eNB */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Initiating Intial Context "\
               "Setup Release Procedure"));
      ret = vbMmeInitCtxtRelProc(pNewUeCb, CAUSE_RADIONW,\
            SztCauseRadioNwhandover_cancelledEnum, 0);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Intial "\
                  "Context Setup Release Procedure"));
         VB_MME_END_EMM_PROC(pNewUeCb);
         vbMmeUtlDelHOUeCb(pNewUeCb);
      }
   }

   RETVALUE(ret); 
} /* vbMmeHndlHOCancel */

/**************************************************************************//**
 *
 * @brief 
 *    Sends Handover Cancel Ack message to eNodeB
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover required is received
 * 
 * @return
 *    SUCCESS if Hanodver Cancel Ack is sent successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeSndHOCancelAck
(
VbMmeUeCb      *pUeCb
)
{
   S16            ret = ROK;
   S1apPdu        *pHoCancelAck = NULLP;

   VB_TRC2(vbMmeSndHOCancelAck);

   ret = vbMmeUtlBldHOCancelAck(pUeCb, &pHoCancelAck);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO(( VB_MME_PRNTBUF, "Sending Handover Cancel "\
            "Acknowledge to S1AP Layer"));
      ret = vbMmeLimSndS1APMsg(pUeCb->eNodebCb, pUeCb, pHoCancelAck,\
            Sztid_HovrCancel);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Sending Handover Cancel "\
               "Acknowledge to S1AP Layer"));
      }
   }
   else
   {
      VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Building Handover Cancel "\
            "Acknowledge"));
   }
   
   if (ROK != ret)
   {
      if (NULL != pHoCancelAck)
      {
         cmFreeMem((Ptr)pHoCancelAck);
      }
   }

   RETVALUE(ret); 
} /* vbMmeSndHOPrepFail */


/**************************************************************************//**
 *
 * @brief 
 *    Builds Handover Cancel Ack Message 
 *    Allocates memory for the message
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block for which handover Cancel is received
 * @param[out] ppHoPrepFail
 *    Pointer to the address of HO Cancel Ack message
 * 
 * @return
 *    SUCCESS if Hanodver Cancel Ack built successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PRIVATE S16 vbMmeUtlBldHOCancelAck
(
VbMmeUeCb    *pUeCb,
S1apPdu      **ppHoCancelAck
)
{
   U16         numComp = 0;
   S16         ret     = ROK;
   S1apPdu     *pPdu   = NULLP;
   Mem         mem     = {0, 0};
   SztProtIE_Field_HovrCancelAckgIEs *ie = NULLP;

   VB_TRC2(vbMmeUtlBldHOCancelAck);

   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem, (Ptr *)&pPdu);
   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: S1apPdu"));
      RETVALUE(RFAILED);
   }
    
   cmMemset((U8 *)&(pPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(pPdu->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   vbUtlFillTknU8(&(pPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pPdu->pdu.val.successfulOutcome.procedureCode),\
         Sztid_HovrCancel);
   vbUtlFillTknU32(&(pPdu->pdu.val.successfulOutcome.criticality),\
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(pPdu->pdu.val.successfulOutcome.value.u.sztHovrCancelAckg.\
         pres), PRSNT_NODEF);

   numComp = 2;
   vbUtlFillTknU16(&(pPdu->pdu.val.successfulOutcome.value.u.\
         sztHovrCancelAckg.protocolIEs.noComp), numComp);
   if ((cmGetMem(pPdu, (numComp * sizeof(SztProtIE_Field_HovrCancelAckgIEs)), 
         (Ptr *)&pPdu->pdu.val.successfulOutcome.value.u.sztHovrCancelAckg.\
         protocolIEs.member)) != ROK)
   {
      cmFreeMem((Ptr)pPdu);
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: "\
            "SztProtIE_Field_HovrCancelAckgIEs"));
      ret = RFAILED;
   }

   /* MME UE S1AP ID */
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCancelAckg.\
         protocolIEs.member[0];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), pUeCb->mMeUeS1apId);

   /* eNB UE S1AP ID */
   ie = &pPdu->pdu.val.successfulOutcome.value.u.sztHovrCancelAckg.\
         protocolIEs.member[1];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), pUeCb->eNbUeS1apId);

   *ppHoCancelAck = pPdu;

   RETVALUE(ret);
} /* vbMmeUtlBldHOCancelAck */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/**********************************************************************
  
         End of file:     vb_mme_s1ap_utl.c@@/main/1 - Mon Jun 28 19:35:38 2010
   
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
