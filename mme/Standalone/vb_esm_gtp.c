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

     Name:    MME

     Type:    C Include file

     Desc:    This file contains the ESM-GTP module source code

     File:    vb_esm_gtp.c

     Sid:      vb_esm_gtp.c@@/main/1 - Mon Jun 28 19:35:35 2010

     Prg:     aj
*********************************************************************21*/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options         */ 
#include "envdep.h"        /* Environment dependent       */ 
#include "envind.h"        /* Environment independent     */ 

#include "gen.h"           /* General                     */ 
#include "ssi.h"           /* System services             */ 
#include "cm5.h"           /* Common timer library        */ 
#include "cm_llist.h"      /* Common linked list library  */ 
#include "cm_hash.h"       /* Common hash library         */ 
#include "cm_tpt.h"        /* Common transport library    */ 
#include "cm_tkns.h"       /* Common tokens               */ 
#include "cm_mblk.h"       /* Common memory allocation    */ 
#include "cm_inet.h"       /* Common socket library       */ 
#include "cm_pasn.h"       /* Common ASN.1 library        */ 
#include "sct.h"           /* SCT interface defines       */ 

#include "egt.h"           /* EGTP Upper Interface        */ 
#include "leg.h"           /* EGTP LM Interface           */ 

#include "szt.h"           /* S1AP Upper Interface        */ 
#include "lsz.h"           /* S1AP LM Interface           */ 
#include "szt_asn.h"       /* S1AP ASN                    */ 

#include "lvb.h"           /* CNE Layer management        */ 
#include "vb_hss.h"        /* CNE HSS Application defines */ 
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines     */ 
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* CNE EMM defines             */ 
#include "cm_esm.h"        /* CNE EDM defines             */ 
#include "vb_err.h"
/* vb005.101 Changes */
#ifdef EG_REL_930
#include "eg_dbutl.h" /* eGTP Message Dictionary related macros */
#endif /* EG_REL_930 */

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
#include "cm_pasn.x"       /* Common ASN.1 Library            */ 
#include "sct.x"           /* SCT interface defines           */ 
#include "egt.x"           /* EGTP Upper Interface            */ 
#include "leg.x"           /* EGTP LM Interface               */ 
#include "eg_util.x"
#include "szt_asn.x"       /* S1AP ASN                        */ 
#include "szt.x"           /* S1AP Upper Interface            */ 
#include "lsz.x"           /* S1AP LM Interface               */ 
#include "lvb.x"           /* CNE Layer management            */ 
#include "cm_esm.x"        /* CNE ESM structures              */
#include "cm_emm.x"        /* CNE EMM structures              */
#include "cm_emm_esm.x"    /* CNE EMM-ESM structures          */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE HSS Application structures  */
#include "vb.x"            /* CNE Application structures      */
#include "vb_gtp_utl.x"
#include "vb_s10_hndlr.x"  /* S10 Handler Functions           */
#include "vb_s10_utl.x"    /* S10 Utility Functions           */
#ifdef VB_IRAT_ACC_TEST
#include "vb_s3_hndlr.x"  /* S10 Handler Functions           */
#endif
#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */


/**********************************************************************/
/*PRIVATE S16 vbMmeCreateTunnelReq ARGS((VbMmeUeCb    *ueCb));*/

/**********************************************************************/
TmrCfg t3;
/**********************************************************************/

/*
 *
 *       Fun:   vbUtlGenerateCTransId
 *
 *       Desc:  This function is used to generate CTransid
 *
 *       Ret:  
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
PRIVATE Void  vbUtlGenerateCTransId(U32 *transId)
{
   /* VB_TODO_SECOND_PHASE  implement rollover of counter */
   *transId = ++vbMmeCb.transIdCntr;
}

/*
 *
 *       Fun:   vbMmeSndCSReq
 *
 *       Desc:  This function used to send create session request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndCSReq
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb
)
#else
PUBLIC S16 vbMmeSndCSReq(ueCb, esmCb)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
#endif
{
   TknU32  teidTkn;
   EgMsg  *egMsg = NULLP;
	/* This is intialized to avoid klocworks tool warning */
   TknU8   create = {0};
   S16     ret = ROK;
   U8      bId = 0;

   VB_TRC2(vbMmeSndCSReq)

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));

   if ((VB_MME_ATTACH_PROC == ueCb->proc) || (VB_MME_PDN_CONN_PROC == esmCb->proc))
   {
      ret = vbMmeUtlAllocBId(ueCb, &esmCb, &bId, 0);
      if(ROK != ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Allocation Of Bearer Id Failed"));
         RETVALUE(RFAILED);
      }
   }

   vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
   if (VB_MME_ATTACH_PROC == ueCb->proc)
   {
      ueCb->emmCb.piggyAction.bId = bId;
   }

   /* Send create session request to SGW */
   if ((VB_MME_ATTACH_PROC == ueCb->proc) || \
       (VB_MME_HANDOVER_PROC == ueCb->proc))
   {
      ret = vbMmeCreateTunnelReq(ueCb);
      if (ret != ROK)
      {
#ifdef ALIGN_64BIT   
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Tunnel ueCb(%d)",
               ueCb->ueCbKey));
#else             
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Tunnel ueCb(%ld)",
               ueCb->ueCbKey));
#endif               
         RETVALUE(RFAILED);
      } 
   } 

   /* This Utility should  act on the List for 
    * which createSession request should be sent */
   ret = vbMmeUtlBldCSReq(ueCb->ueTunnelCb, esmCb, &egMsg);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Session Request "
            "ueCb(%d)", ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Session Request "
            "ueCb(%ld)", ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val  = ueCb->ueTunnelCb->localTeIdKey;

   /* 
    * If the MME receive standalone PDN connectivity request,
    * MME should not create the new C-Plane tunnel towards the SGW.
    * Instead MME should use the existing C-plane tunnels 
    */

   if(TRUE != esmCb->esmPiggyBacked)
   {
      create.pres  = NOTPRSNT;
      create.val = FALSE;
   }
   else
   {
      create.pres  = PRSNT_NODEF;
      create.val = TRUE;
   }
   t3.enb = TRUE;
   t3.val = VB_MME_EGT_T3_RESP_TMR;
 
   if ((VB_MME_ATTACH_PROC == ueCb->proc) ||
       (VB_MME_HANDOVER_PROC == ueCb->proc))
   {
      ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        (ueCb->ueTunnelCb->transId), create, t3, egMsg);

      cmMemcpy((U8*)&ueCb->transId, (U8*)&ueCb->ueTunnelCb->transId,\
            sizeof(TknU32));
   }
   else
   {
      ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        esmCb->tranId, create, t3, egMsg);

      cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));
   }

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeSndMBReq
 *
 *       Desc:  This function is used to send modify bearer request.
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndMBReq
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
Bool         isRelease /* vb001.101: Added new argument */
)
#else
PUBLIC S16 vbMmeSndMBReq(ueCb, esmCb, isRelease)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
Bool         isRelease;
#endif
{
	/* create variable not intialized to avoid klockworks report */
   TknU32   teidTkn = {0};
   EgMsg   *egMsg = NULLP;
	/* create variable not intialized to avoid klockworks report */
   TknU8    create = {0};
   S16 ret = ROK;

   VB_TRC2(vbMmeSndMBReq)

   if (NULLP == ueCb->ueTunnelCb)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UE Tunnel Not Found (%d)",\
            ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UE Tunnel Not Found (%ld)",\
            ueCb->ueCbKey));
#endif
		/* This RFAILED has been put to avoid klocworks warning */
		RETVALUE(RFAILED);
   }

   ret = vbMmeUtlBldMBReq(ueCb->ueTunnelCb, esmCb, &egMsg, isRelease);
   if (ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Modify Bearer Request"));
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;
   create.pres = PRSNT_NODEF;
   create.val = FALSE;

   /* Send Modify session request to PGW */
   if (!isRelease)
   {
      VbLiEgtSigReq(&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),\
            &(ueCb->ueTunnelCb->locAddr), &(ueCb->ueTunnelCb->remAddr),\
            teidTkn, esmCb->tranId, create, t3, egMsg);

      cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));
   }
   else
   {
      VbLiEgtSigReq(&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),\
            &(ueCb->ueTunnelCb->locAddr), &(ueCb->ueTunnelCb->remAddr),\
            teidTkn, ueCb->ueTunnelCb->transId, create, t3, egMsg);

      cmMemcpy((U8*)&ueCb->transId, (U8*)&ueCb->ueTunnelCb->transId,\
            sizeof(TknU32));
   }

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeSndMBReqiRAT
 *
 *       Desc:  This function is used to send modify bearer request.
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndMBReqiRAT
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
Bool         isRelease /* vb001.101: Added new argument */
)
#else
PUBLIC S16 vbMmeSndMBReqiRAT(ueCb, esmCb, isRelease)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
Bool         isRelease;
#endif
{
	/* create variable not intialized to avoid klockworks report */
   TknU32   teidTkn = {0};
   EgMsg   *egMsg = NULLP;
	/* create variable not intialized to avoid klockworks report */
   TknU8    create = {0};
   S16 ret = ROK;

   VB_TRC2(vbMmeSndMBReqiRAT);
   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Creating Tunnel CB Towards S-GW"));
   ret = vbMmeCreateTunnelReq(ueCb);
   if (ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Tunnel ueCb(%d)",
               ueCb->ueCbKey));
#else             
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Tunnel ueCb(%ld)",
               ueCb->ueCbKey));
#endif               
      RETVALUE(RFAILED);
   } 


   ueCb->ueTunnelCb->remoteTeId = ueCb->pHoData->sgwTeId;
   ueCb->proc = VB_MME_HANDOVER_PROC; 
   ret = vbMmeUtlBldMBReq(ueCb->ueTunnelCb, esmCb, &egMsg, isRelease);
   if (ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Modify Bearer Request"));
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;
   create.pres = PRSNT_NODEF;
   create.val = TRUE;

   t3.enb = TRUE;
   t3.val = VB_MME_EGT_T3_RESP_TMR;
   
   VbLiEgtSigReq(&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),\
         &(ueCb->ueTunnelCb->locAddr), &(ueCb->ueTunnelCb->remAddr),\
         teidTkn, ueCb->ueTunnelCb->transId, create, t3, egMsg);

   cmMemcpy((U8*)&ueCb->transId, (U8*)&ueCb->ueTunnelCb->transId,\
         sizeof(TknU32));
   
   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeSndRABReq
 *
 *       Desc:  This function is used to Release Access Bearer Request to
 *              SGW
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndRABReq
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeSndRABReq(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   S16            ret     = ROK;
   EgMsg          *pEgMsg = NULLP;
   VbMmeEsmCb     *pEsmCb = NULLP;
   TknU8    create = {0};
   TknU32   teidTkn = {0};

   VB_TRC2(vbMmeSndRABReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Release Access Bearer Request"));
   ret = vbMmeUtlBldRABReq(ueCb, &pEgMsg);
   if (ROK == ret)
   {
      pEsmCb       = ueCb->esmList[0];
      teidTkn.pres = PRSNT_NODEF;
      teidTkn.val  = ueCb->ueTunnelCb->localTeIdKey;
      create.pres  = PRSNT_NODEF;
      create.val   = FALSE;

      VbLiEgtSigReq(&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
            &(ueCb->ueTunnelCb->locAddr), &(ueCb->ueTunnelCb->remAddr),\
            teidTkn, pEsmCb->tranId, create, t3, pEgMsg);

      cmMemcpy((U8*)&ueCb->transId, (U8*)&pEsmCb->tranId, sizeof(TknU32));
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Building Release Access Bearer "\
            "Request"));
   }

   RETVALUE(ret);
} /* vbMmeSndRABReq */

/*
 *
 *       Fun:   vbMmeSndDSReq
 *
 *       Desc:  This function is used to send delete session request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndDSReq
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
U8           cause
)
#else
PUBLIC S16 vbMmeSndDSReq(ueCb, esmCb, cause)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
U8           cause;
#endif
{
   TknU32   teidTkn = {0};
   EgMsg   *egMsg = NULLP;
   TknU8    create = {0};
   S16      ret = ROK;

   VB_TRC2(vbMmeSndDSReq)

   if (NULLP == ueCb->ueTunnelCb)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UE Tunnel Not Found (%d)",\
            ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UE Tunnel Not Found (%ld)",\
            ueCb->ueCbKey));
#endif
		/* This check has been introduced to avoid klocworks tool warning */
		RETVALUE(RFAILED);
   }

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));
   ret = vbMmeUtlBldDSReq(ueCb->ueTunnelCb, esmCb, &egMsg, cause);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Delete Session Request "
                        "ueCb(%d)", ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Send Delete Session Request "
                        "ueCb(%ld)", ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;
   create.pres = PRSNT_NODEF;
   create.val = FALSE;

   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        esmCb->tranId, create , t3, egMsg);

   cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));

   RETVALUE(ret);
}

/* vb001.101: Added new function */
/* 
*
*       Fun:  vbMmeIncDwnLnkDatNot 
*
*       Desc:  This function handles the downlink data notification from SGW
*
*       Desc:  It doesnt use EMM-ESM interpretations for this message.
*       Ret:   ROK
*
*       Notes: None
*
*       File:  vb_mme.c
*
*/
#ifdef ANSI
PUBLIC S16 vbMmeIncDwnLnkDatNot 
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 vbMmeIncDwnLnkDatNot(localAddr, remAddr, teid,
                              transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
  VbMmeUeCb       *ueCb = NULLP;
  VbMmeUeTunnelCb *tunEnt = NULLP;
  S16             ret = ROK;

  VB_TRC2(vbMmeIncDwnLnkDatNot)

#ifdef ALIGN_64BIT
  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)",teid.val));
#else
  VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)",teid.val));
#endif
  /* get the ueCb from the tunnelCb */
  ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), 
                        (U8 *)&(teid.val),
                        sizeof(teid.val), 0, (PTR *)&tunEnt);
  if(ret != ROK)
  {
#ifdef ALIGN_64BIT  
     VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Tunnel Not Found for Trans Id(%d)", transId.val));
#else
     VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Tunnel Not Found for Trans Id(%ld)", transId.val));
#endif
     RETVALUE(RFAILED);
  }
  ueCb =  tunEnt->ueCb;
  if (ueCb == NULLP)
  {
#ifdef ALIGN_64BIT  
     VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UeCb Not Found For Trans Id(%d)", transId.val));
#else
     VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"UeCb Not Found For Trans Id(%ld)", transId.val));
#endif

     RETVALUE(RFAILED);
  } 

  ret = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, tunEnt->ueCb, VB_EGTPC_DWN_LNK_DATA_NOTIFY,
                        egMsg, &transId);
  RETVALUE(ROK);
}
/* 
*
*       Fun:  vbMmeSndDwnLnkNotAck 
*
*       Desc:  This function sends the downlink data notification Ack to SGW
*
*       Desc:  It doesnt use EMM-ESM interpretations for this message.
*       Ret:   ROK
*
*       Notes: None
*
*       File:  vb_mme.c
*
*/
#ifdef ANSI
PUBLIC S16 vbMmeSndDwnLnkNotAck 
(
VbMmeUeCb    *ueCb,
U8           cause
)
#else
PUBLIC S16 vbMmeSndDwnLnkNotAck(ueCb, cause)
VbMmeUeCb    *ueCb;
U8           cause;
#endif
{
   TknU32  teidTkn = {0};
   EgMsg  *egMsg = NULLP;
   TknU8   create = {0};
   S16     ret = ROK;
   VB_TRC2(vbMmeSndDwnLnkNotAck)

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%d)",ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%ld)",ueCb->ueCbKey));
#endif

   /* build the downlink notification Ack */
   ret = vbMmeUtlBldEgtDwnLnkNotAck(ueCb->ueTunnelCb, &egMsg, cause);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
     VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Downlink Notification Ack For ueCb(%d)",
               ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Downlink Notification Ack For ueCb(%ld)",
               ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;

   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;

   create.pres=PRSNT_NODEF;
   create.val = FALSE;

   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr),
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        (ueCb->ueTunnelCb->transId), create, t3, egMsg);

   cmMemcpy((U8*)&ueCb->transId, (U8*)&ueCb->ueTunnelCb->transId, sizeof(TknU32));

   RETVALUE(ret);
}

/* 
*
*       Fun:  vbMmeSndDwnLnkNotFailInd
*
*       Desc:  This function sends the downlink data notification 
*              failure indication to SGW
*
*       Desc:  It doesnt use EMM-ESM interpretations for this message.
*       Ret:   ROK
*
*       Notes: None
*
*       File:  vb_mme.c
*
*/
#ifdef ANSI
PUBLIC S16 vbMmeSndDwnLnkNotFailInd 
(
VbMmeUeCb    *ueCb,
U8           cause
)
#else
PUBLIC S16 vbMmeSndDwnLnkNotFailInd(ueCb, cause)
VbMmeUeCb    *ueCb;
U8           cause;
#endif
{
   TknU32  teidTkn = {0};
   EgMsg  *egMsg = NULLP;
   TknU8   create = {0};
   S16     ret = ROK;
   VB_TRC2(vbMmeSndDwnLnkNotFailInd)

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%d)",ueCb->ueCbKey));
#endif

   /* build the downlink notification Ack */
   ret = vbMmeUtlBldEgtDwnLnkNotFailInd(ueCb->ueTunnelCb, &egMsg, cause);

   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
     VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Downlink Notification Ind For ueCb(%d)",
               ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Create Downlink Notification Ind For ueCb(%ld)",
               ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;
   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;

   create.pres=PRSNT_NODEF;
   create.val = FALSE;

   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr),
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        (ueCb->ueTunnelCb->transId), create, t3, egMsg);
   cmMemcpy((U8*)&ueCb->transId, (U8*)&ueCb->ueTunnelCb->transId, sizeof(TknU32));
   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeCreateTunnelReq
 *
 *       Desc:  This function is used to create tunnel request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeCreateTunnelReq
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeCreateTunnelReq(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   VbMmeUeTunnelCb  *tunCb = NULLP;
   U32   teid = 0;
   U32   transId;
#ifndef EG_REL_930
   PRIVATE U16 seqNum = 0;
#else
   PRIVATE U32 seqNum = 0;
#endif /* EG_REL_930 */
   S16   ret = ROK;

   VB_MME_TRC2(vbMmeCreateTunnelReq)

   if(ueCb->ueTunnelCb)
   {
      RETVALUE(ROK);
   }

   VB_MME_ALLOC(&tunCb, sizeof(VbMmeUeTunnelCb));
   if(tunCb == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB001, ERRZERO,
            "Memory Allocation Failed");
      RETVALUE(RFAILED);
   }

   /* Not required vbUtlGenerateGtpCTeid(&teid) ;*/
   vbUtlGenerateCTransId(&transId);

   tunCb->bearerId =  VB_MME_EPS_DFLT_BEARER_ID;
   tunCb->localTeIdKey = teid;
   tunCb->transId.pres = PRSNT_NODEF;
   tunCb->transId.val = transId;
   cmMemcpy ((U8 *)&tunCb->locAddr, (U8 *)&vbMmeCb.mmeCfg.selfAddr, sizeof (CmTptAddr));
   cmMemcpy ((U8 *)&tunCb->remAddr, (U8 *)&vbMmeCb.mmeCfg.sgwAddr,  sizeof (CmTptAddr));
   tunCb->seqNum = seqNum++;
 
   ret = cmHashListInsert(&(vbMmeCb.egtEchoSapCb->tunnPendCbHlCp), (PTR)tunCb,
         (U8 *)&tunCb->transId.val, sizeof(tunCb->transId.val));
   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB002, ERRZERO,
            "Failed To Insert Tunnel In Hash List.\n");
      VB_MME_FREE(tunCb, sizeof(VbMmeUeTunnelCb));
      RETVALUE(RFAILED);
   }

   /* Make the memory Links */
   tunCb->ueCb = ueCb;
   ueCb->ueTunnelCb = tunCb;

   RETVALUE(ROK);
}



/*
 *
 *       Fun:   vbMmeIncCSRsp
 *
 *       Desc:  This function used to handle create session response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncCSRsp
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 vbMmeIncCSRsp(localAddr,remAddr,teid,transId,egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   VbMmeUeCb         *ueCb             = NULLP;
   VbMmeUeTunnelCb   *tunEnt           = NULLP;
   EgIeLevelInfo     ieLvlInfo         = {0};
   S16               ret               = ROK;

   VB_TRC2(vbMmeIncCSRsp)

   cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)", teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)", teid.val));
#endif

   /*Find based on local teid first in case of multiple PDN */
   ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
         sizeof(teid.val), 0,(PTR *)&tunEnt);
   if(ROK == ret)
   {
      ueCb =  tunEnt->ueCb;
   }
   else
   {
      ret = cmHashListFind(&(vbMmeCb.egtEchoSapCb->tunnPendCbHlCp), 
            (U8 *)&(transId.val), sizeof(transId.val), 0, (PTR *)&tunEnt);
      if(ret != ROK)
      {
#ifdef ALIGN_64BIT   
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Transaction Not Found For "\
               "Transid(%d)", transId.val));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Transaction Not Found For "\
               "Transid(%ld)", transId.val));
#endif
         RETVALUE(RFAILED);
      }

      ueCb =  tunEnt->ueCb;
      tunEnt->localTeIdKey = teid.val;

      cmHashListDelete(&(vbMmeCb.egtEchoSapCb->tunnPendCbHlCp), (PTR) tunEnt);

      ret = cmHashListInsert(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt,
                           (U8 *)&tunEnt->localTeIdKey,
                           sizeof(tunEnt->localTeIdKey));
      if(ret != ROK)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB003, ERRZERO, "Hash Inset Failed");
         RETVALUE(RFAILED);
      }
   }

   ret = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, ueCb,\
         VB_EGTPC_CREATE_SESSION_RESPONSE, egMsg, &transId);

   RETVALUE(ret);
}


/***************************************************************/
/*
 *
 *       Fun:   vbMmeIncMBRsp
 *
 *       Desc:  This function used to handle modify bearer response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncMBRsp
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 vbMmeIncMBRsp (localAddr, remAddr, teid,
                                    transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   VbMmeUeTunnelCb *tunEnt;
   S16             ret = ROK;

   VB_TRC2(vbMmeIncMBRsp)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)", teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)", teid.val));
#endif
   /* When Modify bearer response is received, first search  in 
   Pending Hash list for the tunnel CB. If not found proceed to the normal 
   tunnel CB search. This change is introduced as part of the inter-RAT HO 
   from UTRA to EUTRA where the first message sent to SGW from MME is
   Modify bearer request. So while sending the MBR tunnel CB is created and 
   inserted to Pending list. */
   ret = cmHashListFind(&(vbMmeCb.egtEchoSapCb->tunnPendCbHlCp), 
      (U8 *)&(transId.val), sizeof(transId.val), 0, (PTR *)&tunEnt);
   if(ret == ROK)
   {
      tunEnt->localTeIdKey = teid.val;

      cmHashListDelete(&(vbMmeCb.egtEchoSapCb->tunnPendCbHlCp), (PTR) tunEnt);

      ret = cmHashListInsert(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt,
                           (U8 *)&tunEnt->localTeIdKey,
                           sizeof(tunEnt->localTeIdKey));
      if(ret != ROK)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB003, ERRZERO, "Hash Inset Failed");
         RETVALUE(RFAILED);
      }

   }
   else
   { 
      /*  During Normal Attrach */
      ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
            sizeof(teid.val), 0,(PTR *)&tunEnt);
      if(ret != ROK)
      {
#ifdef ALIGN_64BIT   
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%d)",
                           teid.val));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%ld)",
                           teid.val));
#endif
         RETVALUE(RFAILED);
      }
   }

   ret = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, tunEnt->ueCb,
                        VB_EGTPC_MODIFY_BEARER_RESPONSE, egMsg, &transId);
#ifdef VB_IRAT_ACC_TEST
   vbMmeSndS3FRReq(tunEnt->ueCb, &vbMmeCb.mmeCfg.sgsnIpAddr);
#endif
   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeIncDSRsp
 *
 *       Desc:  This function used to handle delete session response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncDSRsp
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg          /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 vbMmeIncDSRsp (localAddr, remAddr, teid,
                                  transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   S16                     ret         = ROK;
   VbMmeUeTunnelCb         *tunEnt     = NULLP;

   VB_MME_TRC2(vbMmeIncDSRsp)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)", teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)", teid.val));
#endif
   ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
                        sizeof(teid.val), 0,(PTR *)&tunEnt);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%d)",
                        teid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%ld)",
                        teid.val));
#endif
      RETVALUE(RFAILED);
   }

   ret = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, tunEnt->ueCb,
                        VB_EGTPC_DELETE_SESSION_RESPONSE, egMsg, &transId);

   RETVALUE(ret);
}

/* vb001.101: Added new function */
/*
 *
 *       Fun:   vbMmeSendCreateBearerCmd
 *
 *       Desc:  This function used to send create session request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSendCreateBearerCmd
(
 VbMmeEsmCb  *esmCb,
 Bool        isRelease
)
#else
PUBLIC S16 vbMmeSendCreateBearerCmd(esmCb, isRelease)
VbMmeEsmCb  *esmCb;
Bool         isRelease;
#endif
{
   VbMmeUeCb    *ueCb = NULLP;
   TknU32       teidTkn = {0};
   EgMsg        *egMsg  = NULLP;
   TknU8        create = {0};
   S16          ret      = ROK;

   VB_TRC2(vbMmeSendCreateBearerCmd)
   
   ueCb = esmCb->ueCb;

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%d)", ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%ld)", ueCb->ueCbKey));
#endif

   /* This Utility should  act on the List for 
    * which createSession request should be sent */
   ret = vbMmeUtlBldEgtBearResCmd(ueCb->ueTunnelCb, esmCb, &egMsg, isRelease);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not Create Bearer Resource Command ueCb(%d)",
               ueCb->ueCbKey));
#else
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not Create Bearer Resource Command ueCb(%ld)",
               ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;

   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;

   create.pres=PRSNT_NODEF;
   create.val = FALSE;

   t3.enb = TRUE;
   t3.val = VB_MME_EGT_T3_RESP_TMR;
 
   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        esmCb->tranId, create, t3, egMsg);
   cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));

   RETVALUE(ret);
}



/*
 *
 *       Fun:   vbMmeSendDeleteBearerCmd
 *
 *       Desc:  This function used to send Delete Bearer Cmd
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSendDeleteBearerCmd
(
 VbMmeEsmCb  *esmCb
)
#else
PUBLIC S16 vbMmeSendDeleteBearerCmd(esmCb)
VbMmeEsmCb  *esmCb;
#endif
{
   VbMmeUeCb    *ueCb = NULLP;
   TknU32  teidTkn = {0};
   EgMsg  *egMsg = NULLP;
   TknU8   create = {0};
   S16     ret = ROK;

   VB_TRC2(vbMmeSendDeleteBearerCmd)
   
   ueCb = esmCb->ueCb;

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%d)", ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"UeId(%ld)", ueCb->ueCbKey));
#endif

   /* This Utility should  act on the List for 
    * which Delete Bearer Cmd should be sent */
   ret = vbMmeUtlBldEgtDeleteBearCmd(ueCb->ueTunnelCb, esmCb, &egMsg);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not build Delete Bearer Command ueCb(%d)",
               ueCb->ueCbKey));
#else
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not build Delete Bearer Command ueCb(%ld)",
               ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;

   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;

   create.pres=PRSNT_NODEF;
   create.val = FALSE;

   t3.enb = TRUE;
   t3.val = VB_MME_EGT_T3_RESP_TMR;
 
   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        esmCb->tranId, create, t3, egMsg);

   cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));

   RETVALUE(ret);
}


/*
 *
 *       Fun:   vbMmeSndCBRsp
 *
 *       Desc:  This function used to send create Bearer response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSndCBRsp
(
 VbMmeEsmCb  *esmCb,
 U8          status,
 U8          cause
)
#else
PUBLIC S16 vbMmeSndCBRsp(esmCb, status, cause)
VbMmeEsmCb  *esmCb;
U8          status;
U8          cause;
#endif
{
   VbMmeUeCb    *ueCb = NULLP;
   TknU32  teidTkn = {0};
   EgMsg  *egMsg = NULLP;
   TknU8   create = {0};
   S16     ret = ROK;

   VB_TRC2(vbMmeSndCBRsp)
   
   ueCb = esmCb->ueCb;

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "UeId(%d)", ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "UeId(%ld)", ueCb->ueCbKey));
#endif

   /* This Utility should  act on the List for 
    * which createSession request should be sent */
   ret = vbMmeUtlBldEgtCrtBearRsp(ueCb->ueTunnelCb, esmCb, &egMsg, status, cause);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not Create Bearer Response UeCb(%d)",
               ueCb->ueCbKey));
#else
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not Create Bearer Response UeCb(%ld)",
               ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;

   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;

   create.pres=PRSNT_NODEF;
   create.val = FALSE;

   t3.enb = TRUE;
   t3.val = VB_MME_EGT_T3_RESP_TMR;
 
   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        esmCb->tranId, create, t3, egMsg);

   cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));

   RETVALUE(ret);
}


/***************************************************************/
/*
 *
 *       Fun:   vbMmeIncCreateBearerReq
 *
 *       Desc:  This function used to handle create bearer request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncCreateBearerReq
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16  vbMmeIncCreateBearerReq(localAddr, remAddr, teid,
                                    transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   VbMmeUeTunnelCb *tunEnt;
   S16   ret = ROK;


   VB_TRC2(vbMmeIncCreateBearerReq)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TeId(%d)",teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TeId(%ld)",teid.val));
#endif
   ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
         sizeof(teid.val), 0,(PTR *)&tunEnt);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%d)",teid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%ld)",teid.val));
#endif
      RETVALUE(RFAILED);
   }
   /*DEDICATED BEARER*/
   if(egMsg != NULLP)
   {
      tunEnt->seqNum = egMsg->msgHdr.seqNumber;
   }
   ret = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, tunEnt->ueCb, VB_EGTPC_CREATE_BEARER_REQ,
                        egMsg, &transId);
   RETVALUE(ret);
}/* vbMmeIncCreateBearerReq */

/***************************************************************/
/*
 *
 *       Fun:   vbMmeIncDeleteBearerReq
 *
 *       Desc:  This function used to handle Delete bearer request
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncDeleteBearerReq
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16  vbMmeIncDeleteBearerReq(localAddr, remAddr, teid,
                                    transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   VbMmeUeTunnelCb *tunEnt;
   S16   ret = ROK;


   VB_TRC2(vbMmeIncDeleteBearerReq)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TeId(%d)",teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "TeId(%ld)",teid.val));
#endif
   ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
         sizeof(teid.val), 0,(PTR *)&tunEnt);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%d)",teid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%ld)",teid.val));
#endif
      RETVALUE(RFAILED);
   }

   ret = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, tunEnt->ueCb, VB_EGTPC_DELETE_BEARER_REQ,
                        egMsg, &transId);
   RETVALUE(ret);
}/* vbMmeIncDeleteBearerReq */



/*
 *
 *       Fun:   vbMmeSendDeleteBearerRsp
 *
 *       Desc:  This function used to send Delete Bearer response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeSendDeleteBearerRsp
(
 VbMmeEsmCb  *esmCb,
 U8          status,
 U8          cause
)
#else
PUBLIC S16 vbMmeSendDeleteBearerRsp(esmCb, status, cause)
VbMmeEsmCb  *esmCb;
U8          status;
U8          cause;
#endif
{
   VbMmeUeCb    *ueCb = NULLP;
   TknU32  teidTkn = {0};
   EgMsg  *egMsg = NULLP;
   TknU8   create = {0};
   S16     ret = ROK;

   VB_TRC2(vbMmeSendDeleteBearerRsp)
   
   ueCb=esmCb->ueCb;

   cmMemset((U8*)&teidTkn, 0, sizeof(TknU32));
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "UeId(%d)", ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "UeId(%ld)", ueCb->ueCbKey));
#endif

   /* This Utility should  act on the List for 
    * which createSession request should be sent */
   ret = vbMmeUtlBldEgtDelBearRsp(ueCb->ueTunnelCb, esmCb, &egMsg, status, cause);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not Build Delete Bearer Response UeCb(%d)",
               ueCb->ueCbKey));
#else
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                "Could Not Build Delete Bearer Response UeCb(%ld)",
               ueCb->ueCbKey));
#endif
      RETVALUE(RFAILED);
   }

   teidTkn.pres = PRSNT_NODEF;

   teidTkn.val = ueCb->ueTunnelCb->localTeIdKey;

   create.pres=PRSNT_NODEF;
   create.val = FALSE;

   t3.enb = TRUE;
   t3.val = VB_MME_EGT_T3_RESP_TMR;
 
   /* Send out on GTP-C */
   ret = VbLiEgtSigReq (&(vbMmeCb.egtSapCb->pst), (vbMmeCb.egtSapCb->spId),
                        &(ueCb->ueTunnelCb->locAddr), 
                        &(ueCb->ueTunnelCb->remAddr), teidTkn,
                        esmCb->tranId, create, t3, egMsg);

   cmMemcpy((U8*)&ueCb->transId, (U8*)&esmCb->tranId, sizeof(TknU32));

   RETVALUE(ret);
}


/***************************************************************/
/*
 *
 *       Fun:   vbMmeIncRlsAccBearerRsp
 *
 *       Desc:  This function used to handle Release Access bearer response
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncRlsAccBearerRsp
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 vbMmeIncRlsAccBearerRsp (localAddr, remAddr, teid,
                                    transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   VbMmeUeCb         *ueCb       = NULLP;
   VbMmeUeTunnelCb   *tunEnt     = NULLP;
   S16               ret         = ROK;

   VB_TRC2(vbMmeIncRlsAccBearerRsp)
#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)",
                  teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)",
                  teid.val));
#endif
   ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
                        sizeof(teid.val), 0,(PTR *)&tunEnt);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%d)",
                        teid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%ld)",
                        teid.val));
#endif
      RETVALUE(RFAILED);
   }

   ueCb =  tunEnt->ueCb;
   ret  = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, ueCb,
                        VB_EGTPC_RELEASE_ACCESS_BEARER_RESPONSE, egMsg, &transId);
   RETVALUE(ret);
}

/***************************************************************/
/*
 *
 *       Fun:   vbMmeIncBrResFailInd
 *
 *       Desc:  This function used to handle Bearer Resource Failure Indication
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeIncBrResFailInd
(
CmTptAddr   *localAddr,     /* Local node address */
CmTptAddr   *remAddr,       /* Remote node address */
TknU32       teid,          /* Tunnel Id */
TknU32       transId,       /* Transaction Id */
EgMsg       *egMsg         /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 vbMmeIncBrResFailInd (localAddr, remAddr, teid,
                                    transId, egMsg)
CmTptAddr   *localAddr;     /* Local node address */
CmTptAddr   *remAddr;       /* Remote node address */
TknU32       teid;          /* Tunnel Id */
TknU32       transId;       /* Transaction Id */
EgMsg       *egMsg;         /* structure for carrying eGTP MSG info */
#endif
{
   VbMmeUeCb         *ueCb       = NULLP;
   VbMmeUeTunnelCb   *tunEnt     = NULLP;
   S16               ret         = ROK;

   VB_TRC2(vbMmeIncBrResFailInd)
#ifdef ALIGN_64BIT   
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)",
                  teid.val));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)",
                  teid.val));
#endif
   ret = cmHashListFind(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (U8 *)&(teid.val),
                        sizeof(teid.val), 0,(PTR *)&tunEnt);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%d)",
                        teid.val));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Could Not Find The TunCB(%ld)",
                        teid.val));
#endif
      RETVALUE(RFAILED);
   }

   ueCb =  tunEnt->ueCb;
   ret  = vbMmeRcvSgwMsg(vbMmeCb.egtSapCb, ueCb,
                        VB_EGTPC_BEARER_RESOURCE_FAIL_IND, egMsg, &transId);
   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlDelPdnConn
(
VbMmeUeCb  *ueCb,
U8         cause
)
#else
PUBLIC S16 vbMmeUtlDelPdnConn(ueCb, U8 cause)
(
VbMmeUeCb  *ueCb;
U8         cause;
)
#endif
{
   U8  indx  = 0;
   S16 ret   = RFAILED;

   VB_TRC2(vbMmeUtlDelPdnConn)

   for(indx = 0; indx < VB_MME_MAX_PDN_CONN; indx++)
   {
      if(NULLP != ueCb->pdnList[indx])
      {
         ret = vbMmeSndDSReq(ueCb, ueCb->pdnList[indx]->esmCb,\
               cause);
         if(ROK != ret)
         {
            vbMmeUtlDelEsmCb(ueCb->pdnList[indx]->esmCb, ueCb);
         }

         break;
      }
   }
 
   RETVALUE(ret);
}  

#ifdef ANSI
PUBLIC S16 vbMmeUtlModPdnConn
(
VbMmeUeCb  *ueCb,
U8         lastPdnId
)
#else
PUBLIC S16 vbMmeUtlModPdnConn(ueCb, lastPdnId)
(
VbMmeUeCb  *ueCb;
U8         lastPdnId;
)
#endif
{
   U8         indx  = 0;
   S16        ret   = ROK;

   VB_TRC2(vbMmeUtlModPdnConn);

   for (indx = lastPdnId; indx < VB_MME_MAX_PDN_CONN; indx++)
   { 
      if (NULLP != ueCb->pdnList[indx])
      {
         ret = vbMmeSndMBReq(ueCb, ueCb->pdnList[indx]->esmCb, FALSE);
         if (ret == ROK)
         {
            RETVALUE(ROK);
         }
      }
   }
  
   RETVALUE(RFAILED);
} 

#ifdef ANSI
PUBLIC S16 vbMmeUtlRelBearer
(
VbMmeUeCb  *ueCb,
U8         bId ,
TknU32     *transId
)
#else
PUBLIC S16 vbMmeUtlRelBearer(ueCb, bId, transId)
(
VbMmeUeCb  *ueCb;
U8         bId; 
TknU32     *transId
)
#endif
{
   U8         indx   = 0;
   S16        ret    = ROK;
   VbMmeEsmCb *esmCb = NULLP;

   VB_TRC2(vbMmeUtlRelBearer);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Bearer Id:%d", bId));

   if(bId != 0)
   {
      for(indx = 0; indx < ueCb->esmDelList.count; indx++)
      {
         if(NULLP == ueCb->esmDelList.esmInfo[indx].esmCb)
         {
            continue;
         }
         if(bId == ueCb->esmDelList.esmInfo[indx].esmCb->bId)
         {
            esmCb = ueCb->esmDelList.esmInfo[indx].esmCb;
            vbMmeSendDeleteBearerRsp(esmCb, VB_MME_BEAR_REL_SUCCESS, 
                                            EGT_GTPC_CAUSE_ACC_REQ);
            if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
            {
              vbMmeUtlDelPdnCb(ueCb, esmCb, esmCb->pdnCb);
            }
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
            ueCb->esmDelList.esmInfo[indx].esmCb = NULLP;
         }
      }
   }

   if(transId && transId->val)
   {
      /*GTP stack issue work arround */
      cmMemcpy((U8*)transId, (U8*)&ueCb->transId, sizeof(TknU32));
      for(indx = 0; indx < ueCb->esmDelList.count; indx++)
      {
         if(NULLP == ueCb->esmDelList.esmInfo[indx].esmCb)
         {
            continue;
         }
         if(transId->val == ueCb->esmDelList.esmInfo[indx].esmCb->tranId.val)
         {
            esmCb = ueCb->esmDelList.esmInfo[indx].esmCb;
            if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
            {
              vbMmeUtlDelPdnCb(ueCb, esmCb, esmCb->pdnCb);
            }
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
            ueCb->esmDelList.esmInfo[indx].esmCb = NULLP;
         }
      }
   }

   for( indx = 0; indx < ueCb->esmDelList.count; indx++)
   { 
      if(NULLP != ueCb->esmDelList.esmInfo[indx].esmCb)
      {
         ret = vbMmeSendDeleteBearerCmd(ueCb->esmDelList.esmInfo[indx].esmCb); 
         if(ret == ROK)
         {
            RETVALUE(ROK);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Sending Delete Bearer Command "\
                  "Failed"));
            if(CM_ESM_EPS_DEF_BEARER == \
               ueCb->esmDelList.esmInfo[indx].esmCb->bearerType)
            {
               vbMmeUtlDelPdnCb(ueCb, ueCb->esmDelList.esmInfo[indx].esmCb, 
                     ueCb->esmDelList.esmInfo[indx].esmCb->pdnCb);
            }

            VB_MME_FREE(ueCb->esmDelList.esmInfo[indx].esmCb,\
                  sizeof(VbMmeEsmCb));
            ueCb->esmDelList.esmInfo[indx].esmCb = NULLP;
         }
      }
   }
   
   RETVALUE(RFAILED);
} 


/**************************************************************************//**
 *
 * @brief 
 *    Handles Delete Session Response received as a part of Handover
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pTransId
 *    Pointer to Transaction ID
 * 
 * @return
 *    SUCCESS if Delete Session Request is handled successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHODSRsp
(
VbMmeUeCb      *pUeCb,
EgMsg          *pEgMsg,
TknU32         *pTransId
)
{
   S16               ret = ROK;
   VbMmeEsmCb        *pEsmCb = NULLP;
   VbMmeUeTunnelCb   *pTunEnt = NULLP;

   VB_TRC2(vbMmeHndlHODSRsp);

   ret = vbMmeUtlGetEsmCbOnS11TranId(pUeCb, &pEsmCb, pTransId);
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting ESM CB for Default Bearer"));
      vbMmeUtlDelEsmCb(pEsmCb, pUeCb);
   }

   /* Deleting the local Control Plane Tunnel */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Locally Deleting S11 C-Tunnel"));
   VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,\
         pUeCb->ueTunnelCb->localTeIdKey);

   pTunEnt = pUeCb->ueTunnelCb;
   pUeCb->ueTunnelCb = NULLP;
   cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)pTunEnt);
   VB_MME_FREE(pTunEnt, sizeof(VbMmeUeTunnelCb));
   
   /* If target MME, and HO has happened, Delete Session Response
    * is received only in case of HO with MME and SGW chanes has failed
    * Send Forward Relocation failure response to source MME 
    */
   if ((VB_MME_HO_TGT_MME == pUeCb->pHoData->hoMmeType) &&
       (VB_MME_CHK_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE)))
   {
      if (VB_MME_CHK_MASK(pUeCb->pHoData->bitmask, VB_MME_HO_FAILURE_RCVD))
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Forward Relocation "\
               "Response with Failure cause to Source MME"));

         ret = vbMmeSndFRRsp(pUeCb, EGT_GTPC_CAUSE_REQ_REJECT);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Forward Relocation "\
                  "Response with Failure cause to Source MME"));
         }

         vbMmeUtlDelS10Tun(pUeCb);
         vbMmeUtlDelHoData(pUeCb);
         vbMmeUtlDelUeCb(pUeCb);
      }
      else if (VB_MME_CHK_MASK(pUeCb->pHoData->bitmask,\
            VB_MME_RELOC_CANCEL_RCVD))
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Relocation Cancel "\
               "Response with Success Cause to Source MME"));
         ret = vbMmeSndRCRsp(pUeCb, EGT_GTPC_CAUSE_ACC_REQ);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Relocation Cancel "\
                  "Response with Success Cause to Source MME"));
         }

         vbMmeUtlDelS10Tun(pUeCb);
         vbMmeUtlDelHoData(pUeCb);

         /* Deleting the UE Context */
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Initiating Intial Context Setup "\
               "Release Procedure"));
         ret = vbMmeInitCtxtRelProc(pUeCb, CAUSE_NAS, SztCauseNasdetachEnum, 0);
         if (ROK != ret)
         {
            VB_MME_END_EMM_PROC(pUeCb);
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Intial Context "\
                  "Setup Release Procedure"));
         }

      }
   }
   else if (VB_MME_HO_SRC_MME == pUeCb->pHoData->hoMmeType)
   {
      /* Delete Session Response is received part of successfull
       * Handover procedure completion. Send Intial Context Release
       * command to eNode B
       */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Initiating Intial Context Setup "\
            "Release Procedure"));
      ret = vbMmeInitCtxtRelProc(pUeCb, CAUSE_NAS, SztCauseNasdetachEnum, 0);
      if (ROK != ret)
      {
         VB_MME_END_EMM_PROC(pUeCb);
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Intial Context Setup "\
               "Release Procedure"));
      }
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Locally Deleting the UE Cb"));

   RETVALUE(ROK);

} /* vbMmeHndlHODSRsp */


/**************************************************************************//**
 *
 * @brief 
 *    Handles Modify Bearer Response received as a part of Handover
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pTransId
 *    Pointer to Transaction ID
 * 
 * @return
 *    SUCCESS if Modify Bearer Response is handled successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeHndlHOMBrsp
(
VbMmeUeCb      *pUeCb,
EgMsg          *pEgMsg,
TknU32         *pTransId
)
{
    S16               ret             = ROK;
    VbMmeEsmCb        *pEsmCb         = NULLP;
    U8                gtpCause        = 0;
    VbMmeUeCb         *pOldUeCb       = NULLP;
    U8                ueSTmsiCbKey[5] = {'\0'};

    VB_TRC2(vbMmeHndlHOMBrsp);

    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting ESM CB TransId [%ld]",\
                pTransId->val));
    ret = vbMmeUtlGetEsmCbOnS11TranId(pUeCb, &pEsmCb, pTransId);
    if (ROK == ret)
    {
        ret = vbMmeVldNStrMBRsp(pUeCb, pEgMsg, &gtpCause);
        if (ROK != ret)
        {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Stroing Modify "\
                        "Bearer Response"));
        }
        else
        {
            if( pUeCb->pHoData && pUeCb->pHoData->hoType == VB_EMM_HO_INTRA_LTE )
            {
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_MME_CHANGE);
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_SGW_CHANGE);
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_SGW_CHANGE);

                VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_TAU_FOR_HO_REQD);
                if (VB_MME_HO_TGT_MME == pUeCb->pHoData->hoMmeType)
                {
                    VB_MME_END_EMM_PROC(pUeCb);
                    pUeCb->emmCb.state = VB_EMM_UE_REGISTERED;
                }
                else if (VB_MME_HO_SRC_MME == pUeCb->pHoData->hoMmeType)
                {
                    /* Delete Session Response is received part of successfull
                     * Handover procedure completion. Send Intial Context Release
                     * command to eNode B
                     */
                    VB_MME_END_EMM_PROC(pUeCb);
                    pOldUeCb = pUeCb->pHoData->pOldUeCb;
                    vbMmeUtlDelHoData(pUeCb);
                    cmMemcpy(ueSTmsiCbKey, pOldUeCb->ueSTmsiCbKey, 5);

                    /* Remove the old UE CB from hash tables */
                    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Removing OLD UE Cb from IMSI "\
                                "and S-TMSI Hash Tables"));
                    vbMmeUtlDelUeCbOnImsi(pOldUeCb);
                    vbMmeUtlDelUeCbOnSTmsi(pOldUeCb);

                    /* Add the UE CBs into imsi and TMSI Hash tables */
                    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb into IMSI "\
                                "Hash Table"));
                    ret = vbMmeUtlAddUeCbOnImsi(pUeCb->ueCtxt.ueImsi,\
                            pUeCb->ueCtxt.ueImsiLen, pUeCb);
                    if (ROK == ret)
                    {
                        VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding UE Cb into S-TMSI "\
                                    "Hash Table"));
                        ret = vbMmeUtlAddUeCbOnSTmsi(ueSTmsiCbKey, pUeCb);
                        if (ROK != ret)
                        {
                            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding UE Cb into S-TMSI "\
                                        "Hash Table"));
                            vbMmeUtlDelUeCbOnImsi(pUeCb);
                        }
                        else
                        {
                            pUeCb->emmCb.state = VB_EMM_UE_REGISTERED;
                        }
                    }
                    else
                    {
                        VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding UE Cb into IMSI "\
                                    "Hash Table"));

                        VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Deleting PDN Connection"));
                        ret = vbMmeUtlDelPdnConn(pUeCb, EGT_GTPC_CAUSE_RELOC_FAIL);
                        if (ROK != ret)
                        {
                            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Deleting PDN Connection"));
                            vbMmeUtlDelUeCb(pUeCb);
                        }
                    }

                    if (ROK == ret)
                    {
                        VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Initiating Intial Context "\
                                    "Setup Release Procedure"));
                        ret = vbMmeInitCtxtRelProc(pOldUeCb, CAUSE_RADIONW,\
                                SztCauseRadioNwsuccessful_handoverEnum, 0);
                        if (ROK != ret)
                        {
                            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Initiating Intial "\
                                        "Context Setup Release Procedure"));
                            VB_MME_END_EMM_PROC(pOldUeCb);
                            vbMmeUtlDelHOUeCb(pOldUeCb);
                        }
                    }
                }
            }
            else if( pUeCb->pHoData && pUeCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE )
            {
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE);
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_MME_CHANGE);
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITH_SGW_CHANGE);
                VB_MME_UNSET_MASK(pUeCb->bitmask, VB_EMM_HO_WITHOUT_SGW_CHANGE);

                VB_MME_SET_MASK(pUeCb->bitmask, VB_EMM_TAU_FOR_HO_REQD);
                pUeCb->emmCb.state = VB_EMM_UE_REGISTERED;
                pEsmCb = pUeCb->esmList[0];
                pEsmCb->ueCb->ueTunnelCb->ipAddr.type = CM_INET_IPV4ADDR_TYPE;
                pEsmCb->ueCb->ueTunnelCb->ipAddr.u.ipv4NetAddr = \
                                                                 pUeCb->pHoData->ueAddr.u.ipv4NetAddr;
                vbMmeUtlDelHoData(pUeCb);
            }
        }
    }
    else
    {
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting ESM CB TransId [%ld]",\
                    pTransId->val));
    }

    RETVALUE(ROK);

} /* vbMmeHndlHOMBrsp */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/********************************************************************30**

         End of file:     vb_esm_gtp.c@@/main/1 - Mon Jun 28 19:35:35 2010

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
/main/1      ---      aj              1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1  vb001.101    rk              1. Added new argument in vbMmeSndMBReq
                                      2. Added new function vbMmeIncDwnLnkDatNot
                                      3. Added new function vbMmeSendCreateBearerCmd
/main/1  vb005.101    rk              1. Added network intiate detach ccpu00117691
                      pmacharla       1. upgraded to eGTP 2.1 under EG_REL_930 flag
                                         and other eNB GPR4 CRs
*********************************************************************91*/
