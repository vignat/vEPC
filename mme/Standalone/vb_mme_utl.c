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

     Desc:    This file contains utility functions for mme

     File:    vb_mme_utl.c

     Sid:      vb_mme_utl.c@@/main/1 - Mon Jun 28 19:35:39 2010

     Prg:
*********************************************************************21*/
/* header include files -- defines (.h) */
#include <stdlib.h>
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common pasn define              */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */
#include "lvb.h"
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"
#include "cm_esm.h"
#include "cm_emm_esm.h"
#include "vb_err.h"
/* vb005.101 Changes */
#ifdef EG_REL_930
#include "eg_dbutl.h"       /* eGTP Message dictionary related macros */
#endif /* EG_REL_930 */
#include "vbsm.h"


/* header/extern include files (.x) */
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "cm_pasn.x"       /* Common pasn library             */
#include "sct.x"           /* S1AP Upper Interface */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */
#include "egt.x"           /* EGTP Upper Interface */
#include "eg_util.x"
#include "egt_util.x"      /* EGTP Upper Interface utilities */

#include "lvb.x"
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */
#include "vbsm.x"

#ifdef IPV6_SUPPORTED
PRIVATE S8 HToA[] = "0123456789ABCDEF";
#endif /* IPV6_SUPPORTED */

PUBLIC VbGUMMEI *selfGummei =
        &(vbMmeCb.mmeCfg.srvGLst.gummeiLst[LVB_MME_SELF_GUMMEI_IDX]);

PRIVATE S16 vbMmeUtlGetBitRate ARGS ((U8 bitFld,
                                      U8 bitFldExt,
                                      U8 *bitRate));

/* +--- Patch 1.0.5 ---+ */
PRIVATE U32 vbMmeUtlGenRandomNum ARGS((Void));
/* ---- Patch 1.0.5 ---- */

PRIVATE U16 vbMmeSzElmSize[][VB_S1AP_MAX_PROC_ID] =
{
   {
      /* Msg Id 0 - Sztid_HovrPrep */
      sizeof(SztProtIE_Field_HovrReqdIEs),
      /* Msg Id 1 - Sztid_HovrResourceAllocn */
      sizeof(SztProtIE_Field_HovrRqstIEs),
      /* Msg Id 2 - Sztid_HovrNotification */
      sizeof(SztProtIE_Field_HovrNtfyIEs),
      /* Msg Id 3 - Sztid_PathSwRqst */
      sizeof(SztProtIE_Field_PathSwRqstIEs),
      /* Msg Id 4 - Sztid_HovrCancel */
      sizeof(SztProtIE_Field_HovrCancelIEs),
      /* Msg Id 5 - Sztid_E_RABSetup */
      sizeof(SztProtIE_Field_E_RABSetupRqstIEs),
      /* Msg Id 6 - Sztid_E_RABMdfy */
      sizeof(SztProtIE_Field_E_RABMdfyRqstIEs),
      /* Msg Id 7 - Sztid_E_RABRls cmd */
      sizeof(SztProtIE_Field_E_RABRlsCmmdIEs),
      /* Msg Id 8 - Sztid_E_RABRlsInd */
      sizeof(SztProtIE_Field_E_RABRlsIndIEs),
      /* Msg Id 9 - Sztid_InitCntxtSetup */
      sizeof(SztProtIE_Field_InitCntxtSetupRqstIEs),
      /* Msg Id 10 - Sztid_Pag */
      sizeof(SztProtIE_Field_PagIEs),
      /* Msg Id 11 - Sztid_downlinkNASTport */
      sizeof(SztProtIE_Field_DlnkNASTport_IEs),
      /* Msg Id 12 - Sztid_initialUEMsg */
      sizeof(SztProtIE_Field_InitUEMsg_IEs),
      /* Msg Id 13 - Sztid_uplinkNASTport */
      sizeof(SztProtIE_Field_UlnkNASTport_IEs),
      /* Msg Id 14 - Sztid_Reset */
      sizeof(SztProtIE_Field_ResetIEs),
      /* Msg Id 15 - Sztid_ErrInd */
      sizeof(SztProtIE_Field_ErrIndIEs),
      /* Msg Id 16 - Sztid_NASNonDlvryInd */
      sizeof(SztProtIE_Field_NASNonDlvryInd_IEs),
      /* Msg Id 17 - Sztid_S1Setup */
      sizeof(SztProtIE_Field_S1SetupRqstIEs),
      /* Msg Id 18 - Sztid_UECntxtRlsRqst */
      sizeof(SztProtIE_Field_UECntxtRlsRqst_IEs),
      /* Msg Id 19 - Sztid_DlnkS1cdma2000tunneling */
      sizeof(SztProtIE_Field_DlnkS1cdma2000tunnelingIEs),
      /* Msg Id 20 - Sztid_UlnkS1cdma2000tunneling */
      sizeof(SztProtIE_Field_UlnkS1cdma2000tunnelingIEs),
      /* Msg Id 21 - Sztid_UECntxtModification */
      sizeof(SztProtIE_Field_UECntxtModificationRqstIEs),
      /* Msg Id 22 - Sztid_UECapbltyInfoInd */
      sizeof(SztProtIE_Field_UECapbltyInfoIndIEs),
      /* Msg Id 23 - Sztid_UECntxtRls */
      sizeof(SztProtIE_Field_UECntxtRlsCmmd_IEs),
      /* Msg Id 24 - Sztid_eNBStatusTfr */
      sizeof(SztProtIE_Field_ENBStatusTfrIEs),
      /* Msg Id 25 - Sztid_MMEStatusTfr */
      sizeof(SztProtIE_Field_MMEStatusTfrIEs),
      /* Msg Id 26 - Sztid_DeactvTrace */
      sizeof(SztProtIE_Field_DeactvTraceIEs),
      /* Msg Id 27 - Sztid_TraceStart */
      sizeof(SztProtIE_Field_TraceStartIEs),
      /* Msg Id 28 - Sztid_TraceFailInd */
      sizeof(SztProtIE_Field_TraceFailIndIEs),
      /* Msg Id 29 - Sztid_ENBConfigUpd */
      sizeof(SztProtIE_Field_ENBConfigUpdIEs),
      /* Msg Id 30 - Sztid_MMEConfigUpd */
      sizeof(SztProtIE_Field_MMEConfigUpdIEs),
      /* Msg Id 31 - Sztid_LocRprtngCntrl */
      sizeof(SztProtIE_Field_LocRprtngCntrlIEs),
      /* Msg Id 32 - Sztid_LocRprtngFailInd */
      sizeof(SztProtIE_Field_LocRprtngFailIndIEs),
      /* Msg Id 33 - Sztid_LocReport */
      sizeof(SztProtIE_Field_LocReportIEs),
      /* Msg Id 34 - Sztid_OverloadStart */
      sizeof(SztProtIE_Field_OverloadStartIEs),
      /* Msg Id 35 - Sztid_OverloadStop */
      sizeof(SztProtIE_Cont_OverloadStopIEs),
      /* Msg Id 36 - Sztid_WriteReplaceWarning */
      sizeof(SztProtIE_Field_WriteReplaceWarningRqstIEs),
      /* Msg Id 37 - Sztid_eNBDirectInformTfr */
      sizeof(SztProtIE_Field_ENBDirectInformTfrIEs),
      /* Msg Id 38 - Sztid_MMEDirectInformTfr */
      sizeof(SztProtIE_Cont_MMEDirectInformTfrIEs)
   },

   {
      /* Msg Id 0 - Sztid_HovrPrep */
      sizeof(SztProtIE_Field_HovrCmmdIEs),
      /* Msg Id 1 - Sztid_HovrResourceAllocn */
      sizeof(SztProtIE_Field_HovrRqstAckgIEs),
      /* Msg Id 2 - Sztid_HovrNotification */
      0,
      /* Msg Id 3 - Sztid_PathSwRqst */
      sizeof(SztProtIE_Field_PathSwRqstAckgIEs),
      /* Msg Id 4 - Sztid_HovrCancel */
      sizeof(SztProtIE_Field_HovrCancelAckgIEs),
      /* Msg Id 5 - Sztid_E_RABSetup */
      sizeof(SztProtIE_Field_E_RABSetupRespIEs),
      /* Msg Id 6 - Sztid_E_RABMdfy */
      sizeof(SztProtIE_Field_E_RABMdfyRespIEs),
      /* Msg Id 7 - Sztid_E_RABRls cmd */
      sizeof(SztProtIE_Field_E_RABRlsRespIEs),
      /* Msg Id 8 - Sztid_E_RABRlsInd */
      0,
      /* Msg Id 9 - Sztid_InitCntxtSetup */
      sizeof(SztProtIE_Field_InitCntxtSetupRespIEs),
      /* Msg Id 10 - Sztid_Pag */
      0,
      /* Msg Id 11 - Sztid_downlinkNASTport */
      0,
      /* Msg Id 12 - Sztid_initialUEMsg */
      0,
      /* Msg Id 13 - Sztid_uplinkNASTport */
      0,
      /* Msg Id 14 - Sztid_Reset */
      sizeof(SztProtIE_Field_ResetAckgIEs),
      /* Msg Id 15 - Sztid_ErrInd */
      0,
      /* Msg Id 16 - Sztid_NASNonDlvryInd */
      0,
      /* Msg Id 17 - Sztid_S1Setup */
      sizeof(SztProtIE_Field_S1SetupRespIEs),
      /* Msg Id 18 - Sztid_UECntxtRlsRqst */
      0,
      /* Msg Id 19 - Sztid_DlnkS1cdma2000tunneling */
      0,
      /* Msg Id 20 - Sztid_UlnkS1cdma2000tunneling */
      0,
      /* Msg Id 21 - Sztid_UECntxtModification */
      sizeof(SztProtIE_Field_UECntxtModificationRespIEs),
      /* Msg Id 22 - Sztid_UECapbltyInfoInd */
      0,
      /* Msg Id 23 - Sztid_UECntxtRls */
      sizeof(SztProtIE_Field_UECntxtRlsComplete_IEs),
      /* Msg Id 24 - Sztid_eNBStatusTfr */
      0,
      /* Msg Id 25 - Sztid_MMEStatusTfr */
      0,
      /* Msg Id 26 - Sztid_DeactvTrace */
      0,
      /* Msg Id 27 - Sztid_TraceStart */
      0,
      /* Msg Id 28 - Sztid_TraceFailInd */
      0,
      /* Msg Id 29 - Sztid_ENBConfigUpd */
      sizeof(SztProtIE_Field_ENBConfigUpdAckgIEs),
      /* Msg Id 30 - Sztid_MMEConfigUpd */
      sizeof(SztProtIE_Field_MMEConfigUpdFailIEs),
      /* Msg Id 31 - Sztid_LocRprtngCntrl */
      0,
      /* Msg Id 32 - Sztid_LocRprtngFailInd */
      0,
      /* Msg Id 33 - Sztid_LocReport */
      0,
      /* Msg Id 34 - Sztid_OverloadStart */
      0,
      /* Msg Id 35 - Sztid_OverloadStop */
      0,
      /* Msg Id 36 - Sztid_WriteReplaceWarning */
      sizeof(SztProtIE_Field_WriteReplaceWarningRespIEs),
      /* Msg Id 37 - Sztid_eNBDirectInformTfr */
      0,
      /* Msg Id 38 - Sztid_MMEDirectInformTfr */
      0
   },

   {
      /* Msg Id 0 - Sztid_HovrPrep */
      sizeof(SztProtIE_Field_HovrPrepFailIEs),
      /* Msg Id 1 - Sztid_HovrResourceAllocn */
      sizeof(SztProtIE_Field_HovrFailIEs),
      /* Msg Id 2 - Sztid_HovrNotification */
      0,
      /* Msg Id 3 - Sztid_PathSwRqst */
      sizeof(SztProtIE_Field_PathSwRqstFailIEs),
      /* Msg Id 4 - Sztid_HovrCancel */
      0,
      /* Msg Id 5 - Sztid_E_RABSetup */
      0,
      /* Msg Id 6 - Sztid_E_RABMdfy */
      0,
      /* Msg Id 7 - Sztid_E_RABRls cmd */
      0,
      /* Msg Id 8 - Sztid_E_RABRlsInd */
      0,
      /* Msg Id 9 - Sztid_InitCntxtSetup */
      sizeof(SztProtIE_Field_InitCntxtSetupFailIEs),
      /* Msg Id 10 - Sztid_Pag */
      0,
      /* Msg Id 11 - Sztid_downlinkNASTport */
      0,
      /* Msg Id 12 - Sztid_initialUEMsg */
      0,
      /* Msg Id 13 - Sztid_uplinkNASTport */
      0,
      /* Msg Id 14 - Sztid_Reset */
      0,
      /* Msg Id 15 - Sztid_ErrInd */
      0,
      /* Msg Id 16 - Sztid_NASNonDlvryInd */
      0,
      /* Msg Id 17 - Sztid_S1Setup */
      sizeof(SztProtIE_Field_S1SetupFailIEs),
      /* Msg Id 18 - Sztid_UECntxtRlsRqst */
      0,
      /* Msg Id 19 - Sztid_DlnkS1cdma2000tunneling */
      0,
      /* Msg Id 20 - Sztid_UlnkS1cdma2000tunneling */
      0,
      /* Msg Id 21 - Sztid_UECntxtModification */
      sizeof(SztProtIE_Field_UECntxtModificationFailIEs),
      /* Msg Id 22 - Sztid_UECapbltyInfoInd */
      0,
      /* Msg Id 23 - Sztid_UECntxtRls */
      0,
      /* Msg Id 24 - Sztid_eNBStatusTfr */
      0,
      /* Msg Id 25 - Sztid_MMEStatusTfr */
      0,
      /* Msg Id 26 - Sztid_DeactvTrace */
      0,
      /* Msg Id 27 - Sztid_TraceStart */
      0,
      /* Msg Id 28 - Sztid_TraceFailInd */
      0,
      /* Msg Id 29 - Sztid_ENBConfigUpd */
      sizeof(SztProtIE_Field_ENBConfigUpdFailIEs),
      /* Msg Id 30 - Sztid_MMEConfigUpd */
      sizeof(SztProtIE_Field_MMEConfigUpdFailIEs),
      /* Msg Id 31 - Sztid_LocRprtngCntrl */
      0,
      /* Msg Id 32 - Sztid_LocRprtngFailInd */
      0,
      /* Msg Id 33 - Sztid_LocReport */
      0,
      /* Msg Id 34 - Sztid_OverloadStart */
      0,
      /* Msg Id 35 - Sztid_OverloadStop */
      0,
      /* Msg Id 36 - Sztid_WriteReplaceWarning */
      0,
      /* Msg Id 37 - Sztid_eNBDirectInformTfr */
      0,
      /* Msg Id 38 - Sztid_MMEDirectInformTfr */
      0
   }
};

/************************ Prototypes **********************************/

/**********************************************************************/

/*
 *
 *       Fun:   vbMmeUtlInitUeTnlCbs
 *
 *       Desc:  Hash List initialization of EGTP control tunnel blocks
 *
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlInitUeTnlCbs
(
VbMmeEgtSapCb *egtSapCb,
U16            nmbBins,
U16            offset,
Bool           dupFlg,
U16            keyType
)
#else
PUBLIC S16 vbMmeUtlInitUeTnlCbs (egtSapCb, nmbBins, offset, dupFlg, keyType)
VbMmeEgtSapCb *egtSapCb;
U16            nmbBins;
U16            offset;
Bool           dupFlg;
U16            keyType;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlInitUeTnlCbs)

   ret = cmHashListInit(&egtSapCb->ueTunnelCbHlCp, nmbBins, offset,
         dupFlg, keyType, vbMmeCb.init.region, vbMmeCb.init.pool);

   ret = cmHashListInit(&egtSapCb->tunnPendCbHlCp, nmbBins, offset,
         dupFlg, keyType, vbMmeCb.init.region, vbMmeCb.init.pool);

   RETVALUE(ret);
} /* end of vbMmeUtlInitUeTnlCbs */


/*
 *
 *       Fun:   vbMmeUtlInitENodeBs
 *
 *       Desc: Hash List initialization for eNodeBs; Invoked by the application
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlInitENodeBs
(
U16          nmbBins,
U16          offset,
Bool         dupFlg,
U16          keyType
)
#else
PUBLIC S16 vbMmeUtlInitENodeBs(nmbBins, offset, dupFlg, keyType)
U16          nmbBins;
U16          offset;
Bool         dupFlg;
U16          keyType;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlInitENodeBs)

   ret = cmHashListInit(&vbMmeCb.eNbLst, nmbBins, offset, dupFlg, keyType,
         vbMmeCb.init.region, vbMmeCb.init.pool);

   RETVALUE(ret);
} /* end of vbMmeUtlInitENodeBs */

/*
 *
 *       Fun:   vbMmeUtlInitUeCbs
 *
 *       Desc:  Hash List initialization for ueCbs; Invoked by the application
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlInitUeCbs
(
VbMmeEnbCb   *enbCb,
U16          nmbBins,
U16          offset,
Bool         dupFlg,
U16          keyType
)
#else
PUBLIC S16 vbMmeUtlInitUeCbs(enbCb, nmbBins, offset, dupFlg, keyType)
VbMmeEnbCb   *enbCb;
U16          nmbBins;
U16          offset;
Bool         dupFlg;
U16          keyType;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlInitUeCbs)

   ret = cmHashListInit(&enbCb->ueCbHlCp, nmbBins, offset, dupFlg, keyType,
         vbMmeCb.init.region, vbMmeCb.init.pool);

   RETVALUE(ret);
} /* end of vbMmeUtlInitUeCbs */

/* vb001.101: Added new function */
/*
 *
 *       Fun:   vbMmeUtlInitSTmsiUeCbs
 *
 *       Desc:  Hash List initialization for ueCbs; Invoked by the application
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlInitSTmsiUeCbs
(
U16          nmbBins,
U16          offset,
Bool         dupFlg,
U16          keyType
)
#else
PUBLIC S16 vbMmeUtlInitSTmsiUeCbs(nmbBins, offset, dupFlg, keyType)
U16          nmbBins;
U16          offset;
Bool         dupFlg;
U16          keyType;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlInitSTmsiUeCbs)

   ret = cmHashListInit(&vbMmeCb.ueSTmsiCbHlCp, nmbBins, offset, dupFlg, keyType,
         vbMmeCb.init.region, vbMmeCb.init.pool);

   RETVALUE(ret);
} /* end of vbMmeUtlInitSTmsiUeCbs */


/*
 *
 *       Fun:   vbMmeUtlInitImsiUeCbs
 *
 *       Desc:  HashList initialization for ueCbs; Invoked by the application
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlInitImsiUeCbs
(
U16          nmbBins,
U16          offset,
Bool         dupFlg,
U16          keyType
)
#else
PUBLIC S16 vbMmeUtlInitImsiUeCbs(nmbBins, offset, dupFlg, keyType)
U16          nmbBins;
U16          offset;
Bool         dupFlg;
U16          keyType;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlInitImsiUeCbs)

   ret = cmHashListInit(&vbMmeCb.ueImsiCbHlCp, nmbBins, offset, dupFlg, keyType,
         vbMmeCb.init.region, vbMmeCb.init.pool);

   RETVALUE(ret);
} /* end of vbMmeUtlInitImsiUeCbs */


/*
 *
 *       Fun:   vbMmeUtlAddEnbCb
 *
 *       Desc:  EnbCb Insertion into hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAddEnbCb
(
U32           peerId,
VbMmeEnbCb    **mmeEnbCb
)
#else
PUBLIC S16 vbMmeUtlAddEnbCb(peerId, mmeEnbCb)
U32           peerId;
VbMmeEnbCb    **mmeEnbCb;
#endif
{
   VbMmeEnbCb  *enbCb = NULLP;
   S16  ret;

   VB_TRC2(vbMmeUtlAddEnbCb)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Adding Enb Peer(%d)",peerId ));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Adding Enb Peer(%ld)",peerId ));
#endif
   /* Allocate enbCb */
   VB_MME_ALLOC(&enbCb, sizeof(VbMmeEnbCb));
   if(NULLP == (enbCb))
   {
       /* Debug print here */
       RETVALUE(RFAILED);
   }
   /* Initialize the hash list of UE control context */
   ret = vbMmeUtlInitUeCbs(enbCb, VB_MAX_UECB_BIN, 0, FALSE,
                           CM_HASH_KEYTYPE_ANY);

   if(ROK != ret)
   {
      /* Debug print here */
      VB_MME_FREE(enbCb, sizeof(VbMmeEnbCb));
      RETVALUE(ret);
   }

   (enbCb)->peerId  = peerId;
   (enbCb)->state   = VB_MME_ENB_DOWN;
   (enbCb)->sztSapCb = vbMmeCb.sztSapCb;
   (enbCb)->ueConnIdCntr = 0;
   (enbCb)->isSecUeAllowed = FALSE;

   /* Copy the elements */
   cmMemcpy((U8 *)&((enbCb)->eNbCfg), (U8 *)&vbMmeCb.mmeCfg.eNbCfg[peerId],
            sizeof(VbEnbCbCfg));

   ret = cmHashListInsert(&vbMmeCb.eNbLst, (PTR)(enbCb),
            (U8 *)&((enbCb)->peerId), ((U16)sizeof((enbCb)->peerId)));
   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB043, ERRZERO,
               "vbMmeUtlAddEnbCb: Failed to insert into hash list");
      RETVALUE(ret);
   }

   *mmeEnbCb = enbCb;

   RETVALUE(ret);
} /* end of vbMmeUtlAddEnbCb */

/*
 *
 *       Fun:   vbMmeUtlFndEnbCb
 *
 *       Desc:  EnbCb Finding in hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndEnbCb
(
VbMmeEnbCb    **enbCb,
U32           peerId
)
#else
PUBLIC S16 vbMmeUtlFndEnbCb(enbCb, peerId)
VbMmeEnbCb    **enbCb;
U32           peerId;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlFndEnbCb)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Finding Enb CB for Peer ID: %d)",peerId));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Finding Enb CB for Peer ID: %ld)",peerId));
#endif

   ret = cmHashListFind(&vbMmeCb.eNbLst, (U8 *)&peerId, (U16)sizeof(peerId),\
         0, (PTR *)enbCb);

   RETVALUE(ret);
} /* end of vbMmeUtlFndEnbCb */

/*
 *
 *       Fun:   vbMmeUtlDelEnbCb
 *
 *       Desc:  EnbCb deletion from the hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelEnbCb
(
VbMmeEnbCb    *enbCb
)
#else
PUBLIC S16 vbMmeUtlDelEnbCb(enbCb)
VbMmeEnbCb    *enbCb;
#endif
{
   VbMmeUeCb  *ueCb = NULLP;
   S16  ret;
   U32  enbId = 0;

   VB_TRC2(vbMmeUtlDelEnbCb)

   if(enbCb->enbStupInfo.gEnbId.enbId.choice.val)
   {
       enbId = (enbCb->enbStupInfo.gEnbId.enbId.val.macroENB_ID.val[0]<<24) |
               (enbCb->enbStupInfo.gEnbId.enbId.val.macroENB_ID.val[1]<<16) |
               (enbCb->enbStupInfo.gEnbId.enbId.val.macroENB_ID.val[2]<<8)  |
               (enbCb->enbStupInfo.gEnbId.enbId.val.macroENB_ID.val[3]);
   }
   else
   {
       enbId = (enbCb->enbStupInfo.gEnbId.enbId.val.homeENB_ID.val[0]<<24) |
               (enbCb->enbStupInfo.gEnbId.enbId.val.homeENB_ID.val[1]<<16) |
               (enbCb->enbStupInfo.gEnbId.enbId.val.homeENB_ID.val[2]<<8)  |
               (enbCb->enbStupInfo.gEnbId.enbId.val.homeENB_ID.val[3]);
   }

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Deleting Enb (%d)", enbId ));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Deleting Enb (%ld)", enbId ));
#endif

   while (cmHashListGetNext (&enbCb->ueCbHlCp, (PTR) NULLP,
            (PTR *) &ueCb) == ROK)
   {
      /* Delete the Ue Context in the enbCb */
      vbMmeUtlDelUeCb(ueCb);
   }

   cmHashListDeinit(&(enbCb->ueCbHlCp));
   ret = cmHashListDelete(&vbMmeCb.eNbLst,(PTR)enbCb);

   if(ret != ROK)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF," Failed To Delete eNodeB(%d)",
                        enbId));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Failed To Delete eNodeB(%ld)",
                        enbId));
#endif
   }
   VB_MME_FREE(enbCb, sizeof(VbMmeEnbCb));

   RETVALUE(ret);
} /* end of vbMmeUtlDelEnbCb */

/* +--- Patch 1.0.5 ---+ */
/*
 *
 *       Fun:   vbMmeUtlGenRandomNum
 *
 *       Desc:  Generates random number
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PRIVATE U32 vbMmeUtlGenRandomNum
(
)
#else
PRIVATE U32 vbMmeUtlGenRandomNum()
#endif
{
   PRIVATE Bool firstTime = TRUE;
   VB_TRC2(vbMmeUtlGenRandomNum)

   /* TODO:Find better way of generate random number */
   if(firstTime)
   {
      srandom((U32)time(NULLP));
      firstTime = FALSE;
   }

   RETVALUE(random()); /* stdlib call */
} /* vbMmeUtlGenRandomNum */
/* ---- Patch 1.0.5 ---- */

/*
 *
 *       Fun:   vbMmeUtlAllocGuti
 *
 *       Desc:  Allocates a new GUTI for the UE
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */

#ifdef ANSI
PUBLIC S16 vbMmeUtlAllocGuti
(
GUTI *guti
)
#else
PUBLIC S16 vbMmeUtlAllocGuti(guti)
GUTI *guti;
#endif
{
   VB_TRC2(vbMmeUtlAllocGuti);

   /* Copy the parameters */
   guti->pres = TRUE;
   cmMemcpy(guti->mcc, selfGummei->nwId.mcc, 3);
   cmMemcpy(guti->mnc, selfGummei->nwId.mnc, 3);
   guti->mmeGrpId = selfGummei->grpIdLst.grpId[LVB_MME_SELF_BASE_GRPID_INDX];
   guti->mmeCode = selfGummei->codeLst.mmeCode[LVB_MME_SELF_BASE_CODE_INDX];

   /* +--- Patch 1.0.5 ---+ */
   /* Increment the GUTI counter */
#if 0
   guti->mTMSI = ++vbMmeCb.gutiCntr;
#endif
   guti->mTMSI = vbMmeUtlGenRandomNum();
   /* ---- Patch 1.0.5 ---- */

   RETVALUE(ROK);
} /* vbMmeUtlAllocGuti */

/*
 *
 *       FUN:   vbMmeUtlAddUeCb
 *
 *       Desc:  Create and Add new UeCb into hashlists
 *
 *       Ret:   RFAILED on failure
 *              ROK on successful insertion
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAddUeCb
(
VbMmeEnbCb    *enbCb,
VbMmeUeCtxt   *ueCtxt,
VbMmeUeCb     **ueCb
)
#else
PUBLIC S16 vbMmeUtlAddUeCb(enbCb, ueCtxt, ueCb)
VbMmeEnbCb    *enbCb;
VbMmeUeCtxt   *ueCtxt;
VbMmeUeCb     **ueCb;
#endif
{
   S16  ret;
   U16  count = 0;

   VB_TRC2(vbMmeUtlAddUeCb);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Creating  UECb"));
   if (ueCtxt == NULL)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB044, ERRZERO, "UE Context is NULL");
      RETVALUE(RFAILED);
   }

   VB_MME_ALLOC(ueCb, sizeof(VbMmeUeCb));
   if(*ueCb == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB046, ERRZERO, "Allocating Memory: "\
            "VbMmeUeCb");
      RETVALUE(RFAILED);
   }

   (*ueCb)->eNodebCb = enbCb;
   VB_MME_GET_UEID(ueCtxt->suConnId, (*ueCb)->ueCbKey);
   for(count=0; count<CM_ESM_MAX_BEARER_ID; count++)
   {
      (*ueCb)->esmList[count] = NULLP;
   }

   (*ueCb)->suConnId = ueCtxt->suConnId;
   (*ueCb)->spConnId = ueCtxt->spConnId;
   (*ueCb)->emmCb.ueCb = *ueCb;
   (*ueCb)->mMeUeS1apId = ueCtxt->spConnId;
   cmInitTimers((*ueCb)->emmCb.emmTimers, VB_EMM_MAX_TIMERS);

   ret = vbMmeUtlAddUeCbOnConnId((*ueCb)->ueCbKey, enbCb, *ueCb);
   if(ret != ROK)
   {
      /* freeing the correct pointer */
      VB_MME_FREE(*ueCb, sizeof(VbMmeUeCb));
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB048, ERRZERO, "Adding into suConnId "\
            "hash list");
      RETVALUE(RFAILED);
   }

   RETVALUE(ret);
} /* end of vbMmeUtlAddUeCb */



/*
 *
 *       Fun:   vbMmeUtlAddUeCbOnConnId
 *
 *       Desc:  UeCb insertion into ueSTmsiCbHlCp hashlist based on connection id of ue
 *
 *       Ret:   RFAILED on failure
 *              ROK on successful insertion
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAddUeCbOnConnId
(
U32           connId,
VbMmeEnbCb    *enbCb,
VbMmeUeCb     *ueCb
)
#else
PUBLIC S16 vbMmeUtlAddUeCbOnConnId(connId, enbCb, ueCb)
U32           connId;
VbMmeEnbCb    *enbCb;
VbMmeUeCb     *ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlAddUeCbOnConnId)

   if((ueCb == NULLP) || (enbCb == NULLP))
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB051, ERRZERO, "NULL Params");
       RETVALUE(RFAILED);
   }

   ueCb->ueCbKey = connId;
   ret = cmHashListInsert(&(enbCb->ueCbHlCp), (PTR)ueCb,
         (U8 *)&ueCb->ueCbKey, (U16)sizeof(ueCb->ueCbKey));
   if(ret != ROK)
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB052, ERRZERO,
            "Failed to insert into hash list");
       RETVALUE(RFAILED);
   }

   return(ROK);
} /* end of vbMmeUtlAddUeCbOnConnId */

/*
 *
 *       Fun:   vbMmeUtlAddUeCbOnSTmsi
 *
 *       Desc:  UeCb insertion into ueSTmsiCbHlCp hashlist based on STMSI of ue
 *
 *       Ret:   RFAILED on failure
 *              ROK on successful insertion
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAddUeCbOnSTmsi
(
U8            *stmsi,
VbMmeUeCb     *ueCb
)
#else
PUBLIC S16 vbMmeUtlAddUeCbOnSTmsi(stmsi, ueCb)
U8            *stmsi,
VbMmeUeCb     *ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlAddUeCbOnSTmsi)

   if((ueCb == NULLP) || (stmsi == NULLP))
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB053, ERRZERO,
            "vbMmeUtlAddUeCbOnSTmsi: null pointer error");
       RETVALUE(RFAILED);
   }

   cmMemcpy(ueCb->ueSTmsiCbKey, stmsi, 5);

   ret = cmHashListInsert(&(vbMmeCb.ueSTmsiCbHlCp), (PTR)ueCb,
         (U8 *)&ueCb->ueSTmsiCbKey, (U16)5);
   if(ret != ROK)
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB054, ERRZERO,
            "vbMmeUtlAddUeCbOnSTmsi: failed to insert into hashlist");
       RETVALUE(RFAILED);
   }
   RETVALUE(ROK);

} /* end of vbMmeUtlAddUeCbOnSTmsi */

/*
 *
 *       Fun:   vbMmeUtlAddUeCbOnImsi
 *
 *       Desc:  UeCb insertion into ueSTmsiCbHlCp hashlist based on IMSI of ue
 *
 *       Ret:   RFAILED on failure
 *              ROK on successful insertion
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAddUeCbOnImsi
(
U8            *imsi,
U8            imsiLen,
VbMmeUeCb     *ueCb
)
#else
PUBLIC S16 vbMmeUtlAddUeCbOnImsi(imsi, imsiLen, ueCb)
U8            *imsi;
U8            imsiLen;
VbMmeUeCb     *ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlAddUeCbOnImsi)

   if((ueCb == NULLP) || (imsi == NULLP))
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB055, ERRZERO,
            "vbMmeUtlAddUeCbOnSTmsi: null pointer error");
       RETVALUE(RFAILED);
   }

   ret = cmHashListInsert(&(vbMmeCb.ueImsiCbHlCp), (PTR)ueCb,
         (U8 *)imsi, (U16)imsiLen);
   if(ret != ROK)
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB056, ERRZERO,
            "vbMmeUtlAddUeCbOnImsi: failed to insert into hashlist");
       RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of vbMmeUtlAddUeCbOnImsi */


/*
 *
 *       Fun:   vbMmeUtlFndUeCb
 *
 *       Desc:  Finds UE Cb in in hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndUeCb
(
VbMmeEnbCb    *enbCb,
VbMmeUeCb    **ueCb,
U32            ueId
)
#else
PUBLIC S16 vbMmeUtlFndUeCb(enbCb, ueCb, ueId)
VbMmeEnbCb    *enbCb;
VbMmeUeCb    **ueCb;
U32            ueId;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlFndUeCb)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding  UE(%d)", ueId));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding  UE(%ld)", ueId));
#endif

   ret = vbMmeUtlFndUeCbOnEvent(enbCb, ueCb, ueId, 0, NULL);

   if(ret != ROK)
   {
#ifdef ALIGN_64BIT
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UE(%d)",
                                         ueId));
#else
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UE(%ld)",
                                         ueId));
#endif
   }

   RETVALUE(ret);
} /* end of vbMmeUtlFndUeCb */


/*
 *
 *       Fun:   vbMmeUtlFndUeCbOnEvent
 *
 *       Desc:  Finds UE Cb in in hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndUeCbOnEvent
(
VbMmeEnbCb    *enbCb,
VbMmeUeCb     **ueCb,
U32           suConnId,
U32           spConnId,
CmNasEvnt     *evnt
)
#else
PUBLIC S16 vbMmeUtlFndUeCbOnEvent(enbCb, ueCb, suConnId, spConnId, evnt)
VbMmeEnbCb    *enbCb;
VbMmeUeCb     **ueCb;
U32           suConnId;
U32           spConnId;
CmNasEvnt     *evnt;
#endif
{
   S16  ret = RFAILED;
   SztS_TMSI *sztS_TMSI = NULLP;
   SztENB_UE_S1AP_ID *eNbIdTkn = NULLP;
   U8  stmsi[5];
   U32 ueCbKey = 0;

   VB_TRC2(vbMmeUtlFndUeCbOnEvent);

   if(NULLP == enbCb)
   {
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Mull Pointer Error"));
       RETVALUE(RFAILED);
   }

   if(NULLP == evnt ) /* nas message is not present */
   {
       ret = vbMmeUtlFndUeCbOnConnId(enbCb, ueCb, suConnId);
       RETVALUE(ret);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF," evnt (%d), suconId (%d), spconid (%d)\n", ((S1apPdu*)evnt->pdu)->pdu.choice.val, suConnId, spConnId));
   switch(((S1apPdu*)evnt->pdu)->pdu.choice.val)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         if((((S1apPdu *)evnt->pdu)->pdu.val.initiatingMsg.procedureCode.val !=
                  Sztid_initialUEMsg))
         {
            ret = vbMmeUtlFndUeCbOnConnId(enbCb, ueCb, suConnId);
            RETVALUE(ret);
         }

         vbMmeUtlGetS1apIE(&(((S1apPdu*)evnt->pdu)->pdu),Sztid_S_TMSI,
               (TknU8 ** )&sztS_TMSI);
         if((sztS_TMSI != NULLP) && (sztS_TMSI->pres.pres == PRSNT_NODEF))
         {
            VB_MME_GET_STMSI_VAL(stmsi, sztS_TMSI);

            ret = vbMmeUtlFndUeCbOnSTmsi(stmsi, ueCb);
            if((ROK == ret) && (NULLP != (*ueCb)) &&
                  (VB_EMM_UE_REGISTERED == (*ueCb)->emmCb.state) &&
                  (VB_EMM_UE_S1AP_CONNECTED == (*ueCb)->ueCtxt.s1apConState))
            {
               if (VB_MME_HANDOVER_PROC == (*ueCb)->proc)
               {
                  vbMmeAbortHOProc(*ueCb);
               }
               vbMmeRelLocS1Con(*ueCb);
               VB_MME_END_EMM_PROC(*ueCb);
            }
         }
         if((ret != ROK) && (evnt->protDisc == CM_EMM_PD))
         {
            switch(evnt->m.emmEvnt->msgId)
            {
               case CM_EMM_MSG_ATTACH_REQ:
               {
                  /* IMSI */
                  if(CM_EMM_MID_TYPE_IMSI == \
                     evnt->m.emmEvnt->u.atchReq.epsMi.type)
                  {
                     ret = vbMmeUtlFndUeCbOnImsi(evnt->m.emmEvnt->u.
                           atchReq.epsMi.u.imsi.id,
                           evnt->m.emmEvnt->u.atchReq.epsMi.len, ueCb);
                  }
                  /* GUTI */
                  else if(CM_EMM_MID_TYPE_GUTI == \
                     evnt->m.emmEvnt->u.atchReq.epsMi.type)
                  {
                     stmsi[0]= evnt->m.emmEvnt->u.atchReq.epsMi.u.guti.
                        mmeCode;
                     stmsi[1]= (evnt->m.emmEvnt->u.atchReq.epsMi.u.guti.
                           mTMSI>>24) & 0xff;
                     stmsi[2]= (evnt->m.emmEvnt->u.atchReq.epsMi.u.guti.
                           mTMSI>>16) & 0xff;
                     stmsi[3]= (evnt->m.emmEvnt->u.atchReq.epsMi.u.guti.
                           mTMSI>>8) & 0xff;
                     stmsi[4]= (evnt->m.emmEvnt->u.atchReq.epsMi.u.guti.
                           mTMSI) & 0xff;
                     ret= vbMmeUtlFndUeCbOnSTmsi(stmsi, ueCb);
                  }
                  else
                  {
                     /* should not enter here */
                  }

                  /* Special case */
                  /* Scenario: UE is in Registered and Connected Mode,
                     Lost Radio Connection and eNodeB did not inform MME.
                     UE comes up with attach procedure again, eNodeB sends
                     initial UE message to MME, since MME in registered and
                     connected mode release the old connection locally
                     connected */
                  if((NULLP != (*ueCb)) && \
                     (VB_EMM_UE_REGISTERED == (*ueCb)->emmCb.state) &&
                     (VB_EMM_UE_S1AP_CONNECTED == (*ueCb)->ueCtxt.s1apConState))
                  {
                     vbMmeRelLocS1Con(*ueCb);
                     VB_MME_END_EMM_PROC(*ueCb);
                  }

                  break;
               }
               case CM_EMM_MSG_DETACH_REQ:
               {
                  /* IMSI */
                  if(1 == evnt->m.emmEvnt->u.dtchReq.epsMi.type)
                  {
                     ret = vbMmeUtlFndUeCbOnImsi(evnt->m.emmEvnt->u.
                           dtchReq.epsMi.u.imsi.id,
                           evnt->m.emmEvnt->u.dtchReq.epsMi.len, ueCb);
                  }
                  /* GUTI */
                  else if(6 == evnt->m.emmEvnt->u.dtchReq.epsMi.type)
                  {
                     stmsi[0]= evnt->m.emmEvnt->u.dtchReq.epsMi.u.guti.
                        mmeCode;
                     stmsi[1]= (evnt->m.emmEvnt->u.dtchReq.epsMi.u.guti.
                           mTMSI>>24) & 0xff;
                     stmsi[2]= (evnt->m.emmEvnt->u.dtchReq.epsMi.u.guti.
                           mTMSI>>16) & 0xff;
                     stmsi[3]= (evnt->m.emmEvnt->u.dtchReq.epsMi.u.guti.
                           mTMSI>>8) & 0xff;
                     stmsi[4]= (evnt->m.emmEvnt->u.dtchReq.epsMi.u.guti.
                           mTMSI) & 0xff;
                     ret = vbMmeUtlFndUeCbOnSTmsi(stmsi, ueCb);
                  }
                  else
                  {
                     /* should not enter here */
                  }
                  break;
               }
               case CM_EMM_MSG_SERVICE_REQ:
               {
                  /*Should not enter here. S-TMSI must be present */
                  break;
               }
               case CM_EMM_MSG_TAU_REQ:
               {
                  /* GUTI */
                  if(CM_EMM_MID_TYPE_GUTI == evnt->m.emmEvnt->u.tauReq.guti.type)
                  {
                     stmsi[0]= evnt->m.emmEvnt->u.tauReq.guti.u.guti.
                        mmeCode;
                     stmsi[1]= (evnt->m.emmEvnt->u.tauReq.guti.u.guti.
                           mTMSI>>24) & 0xff;
                     stmsi[2]= (evnt->m.emmEvnt->u.tauReq.guti.u.guti.
                           mTMSI>>16) & 0xff;
                     stmsi[3]= (evnt->m.emmEvnt->u.tauReq.guti.u.guti.
                           mTMSI>>8) & 0xff;
                     stmsi[4]= (evnt->m.emmEvnt->u.tauReq.guti.u.guti.
                           mTMSI) & 0xff;
                     ret= vbMmeUtlFndUeCbOnSTmsi(stmsi, ueCb);
                     if((ROK == ret) && (NULLP != (*ueCb)) &&
                        (VB_EMM_UE_REGISTERED == (*ueCb)->emmCb.state) &&
                        (VB_EMM_UE_S1AP_CONNECTED == \
                        (*ueCb)->ueCtxt.s1apConState))
                     {
                        vbMmeRelLocS1Con(*ueCb);
                        VB_MME_END_EMM_PROC(*ueCb);
                     }
                  }
                  break;
               }
               default:
               {
                  break;
               }
            } /* switch(evnt->m.emmEvnt->msgId) */
         } /* if(evnt->protDisc == CM_EMM_PD) */

         if((ROK == ret) && (NULLP != *ueCb))
         {
            /* Find and Update the eNodeB UE S1APID*/
            vbMmeUtlGetS1apIE((SztS1AP_PDU *)&(((S1apPdu *)(evnt->pdu))->pdu),
                  Sztid_eNB_UE_S1AP_ID, (TknU8 **)&eNbIdTkn);
            if(eNbIdTkn == NULLP)
            {
               vbMmeUtlDelUeCb(*ueCb);
               RETVALUE(RFAILED);
            }

            (*ueCb)->eNbUeS1apId = (U16)(eNbIdTkn->val);

            VB_MME_GET_UEID(suConnId, ueCbKey);

            /* Populate the required info into the ueCB */
            if (0 != (*ueCb)->ueCbKey)
            {
               vbMmeUtlDelUeCbOnConnId(*ueCb);
            }

            (*ueCb)->suConnId = suConnId;
            (*ueCb)->spConnId = spConnId;
            (*ueCb)->ueCbKey = ueCbKey;
            (*ueCb)->mMeUeS1apId = spConnId;

            ret = vbMmeUtlAddUeCbOnConnId(suConnId, enbCb, *ueCb);
            if(ROK != ret)
            {
               vbMmeUtlDelUeCb(*ueCb);
               RETVALUE(RFAILED);
            }
         }

         break;
      }
      default:
      {
         ret = vbMmeUtlFndUeCbOnConnId(enbCb, ueCb, suConnId);
         break;
      }
   }

   RETVALUE(ret);
} /* end of vbMmeUtlFndUeCbOnEvent */


/*
 *
 *       Fun:   vbMmeUtlFndUeCbOnConnId
 *
 *       Desc:  Finds UeCb in ueCbHlCp hashlist
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndUeCbOnConnId
(
VbMmeEnbCb    *enbCb,
VbMmeUeCb    **ueCb,
U32            ueId
)
#else
PUBLIC S16 vbMmeUtlFndUeCbOnConnId(enbCb, ueCb, ueId)
VbMmeEnbCb    *enbCb;
VbMmeUeCb    **ueCb;
U32            ueId;
#endif
{
   S16      ret = ROK;

   VB_TRC2(vbMmeUtlFndUeCbOnConnId)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding  UE(%d)", ueId));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding  UE(%ld)", ueId));
#endif
   ret = cmHashListFind(&enbCb->ueCbHlCp, (U8 *)&ueId, (U16)sizeof(ueId),\
         0, (PTR*)ueCb);
   if(ret != ROK)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb(%d)", ueId));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb(%ld)", ueId));
#endif
   }

   RETVALUE(ret);
}

#ifdef X2_HO
/* VB006.101: ADDED NEW FUNCTION VbMmeUtlFndUeCbMmeS1apId() */
/*
 *
 *       Fun:   vbMmeUtlFndUeCbMmeS1apId
 *
 *       Desc:  Finds UE Cb in in hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndUeCbMmeS1apId
(
VbMmeUeCb    **ueCb,
U32            ueId
)
#else
PUBLIC S16 vbMmeUtlFndUeCbMmeS1apId(ueCb, ueId)
VbMmeUeCb    **ueCb;
U32            ueId;
#endif
{
   VbMmeEnbCb        *enbCb = NULLP;
   VbMmeUeCb         *tempUeCb = NULLP;
   VbMmeUeCb         *prevUeCb = NULLP;
   S16               ret = 0;
   U8                enbIdx = 0;

   VB_TRC2(vbMmeUtlFndUeCb)
   *ueCb = NULLP;

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding UE lpueId(%d) MaxEnb(%d)\n", ueId,
            vbMmeCb.mmeCfg.maxNmbEnbs));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding UE lpueId(%ld)", ueId));
#endif

/* Loop through all the peer eNodeBs which needs to be configured */
   for (enbIdx = 0; enbIdx < (vbMmeCb.mmeCfg.maxNmbEnbs);enbIdx++)
   {

      ret = vbMmeUtlFndEnbCb(&enbCb, vbMmeCb.mmeCfg.eNbCfg[enbIdx].peerId);
      if(ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"\n Invalid enodebCb\n"));
         RETVALUE(RFAILED);
      }
      while (cmHashListGetNext (&enbCb->ueCbHlCp, (PTR) prevUeCb,
               (PTR *) &tempUeCb) == ROK)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Find UECB in enodeb peerId(%d) mmeUeId(%d) ueId(%d)\n",
                  enbCb->peerId, tempUeCb->mMeUeS1apId, ueId));
         if(tempUeCb->mMeUeS1apId == ueId)
         {
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Find UECB in enodeb peerId(%d) ueId(%d)\n", ueId, enbCb->peerId));
            *ueCb = tempUeCb;
            RETVALUE(ROK);
         }
         prevUeCb = tempUeCb;
      }
      prevUeCb = NULLP;
      tempUeCb = NULLP;
   } /* end of for loop */

   RETVALUE(RFAILED);
} /* end of vbMmeUtlFndUeCbMmeS1apId */
#endif

/*
 *
 *       Fun:   vbMmeUtlFndUeCbOnSTmsi
 *
 *       Desc:  Finds UeCb in ueSTmsiCbHlCp hashlist
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndUeCbOnSTmsi
(
U8           *stmsi,
VbMmeUeCb    **ueCb
)
#else
PUBLIC S16 vbMmeUtlFndUeCbOnSTmsi(stmsi, ueCb)
U8            *stmsi;
VbMmeUeCb    **ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlFndUeCbOnSTmsi)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding UE(%s)", stmsi));

   ret = cmHashListFind(&vbMmeCb.ueSTmsiCbHlCp, stmsi, (U16)5,
                        0, (PTR*)ueCb);

   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb(%s)",
                                        stmsi));
   }

   RETVALUE(ret);
} /* end of vbMmeUtlFndUeCbOnSTmsi */

/*
 *
 *       Fun:   vbMmeUtlFndUeCbOnImsi
 *
 *       Desc:  Finds UeCb in ueImsiCbHlCp hashlist
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndUeCbOnImsi
(
U8           *imsi,
U8           len,
VbMmeUeCb    **ueCb
)
#else
PUBLIC S16 vbMmeUtlFndUeCbOnImsi(imsi, len, ueCb)
VbMmeUeCb    **ueCb;
U8           *imsi;
U8           len;
VbMmeUeCb    **ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlFndUeCbOnImsi)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding  UE(%s)", imsi));

   ret = cmHashListFind(&vbMmeCb.ueImsiCbHlCp, imsi, (U16)len, 0, (PTR*)ueCb);
   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb(%s)", imsi));
   }

   RETVALUE(ret);
} /* end of vbMmeUtlFndUeCbOnImsi */

/*
 *
 *       Fun:   vbMmeUtlLocDel
 *
 *       Desc:  Deletes UE Cb from hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlLocDel
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlLocDel(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   U16              esmIdx                           = 0;
   U8               imsi[CM_EMM_MAX_MOBILE_ID_DIGS];
   U8               imsiLen                          = 0;

   VB_TRC2(vbMmeUtlLocDel)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Deleting  UE(%d)",
                                   ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Deleting  UE(%ld)",
                                    ueCb->ueCbKey));
#endif

   vbMmeUtlDelUeCbOnConnId(ueCb);
   vbMmeUtlDelPndTrns(ueCb);

   /* Free all esm Cbs */
   for(esmIdx=0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
   {
      vbMmeUtlDelEsmCb(ueCb->esmList[esmIdx], ueCb);
   }
   /* Reset EMM Cb */
   ueCb->emmCb.state = VB_EMM_UE_DEREGISTERED;
   ueCb->emmCb.emmCommProcSubState = VB_EMM_COMM_PROC_IDLE;
   ueCb->emmCb.emmCommProcInProgress =  VB_EMM_COM_PRC_NONE;
   ueCb->emmCb.cause = 0;
   ueCb->emmCb.typeOfId = 0;
   ueCb->emmCb.esmPiggyBacked = FALSE;
   cmMemset((U8*)&ueCb->emmCb.piggyAction, 0, sizeof(ueCb->emmCb.piggyAction));
   if(ueCb->emmCb.retrBuf.val)
   {
      VB_MME_FREE(ueCb->emmCb.retrBuf.val, CM_MAX_EMM_ESM_PDU /*ueCb->emmCb.retrBuf.len*/);
   }

   /* Reset UE Cb */
   cmMemset((U8*)&ueCb->ueCtxt.s1apConState, 0,
                             sizeof(ueCb->ueCtxt.s1apConState));
   cmMemset((U8*)&ueCb->creBid, 0, sizeof(ueCb->creBid));
   cmMemset((U8*)&ueCb->esmTrnList, 0,  sizeof(ueCb->esmTrnList));
   cmMemset((U8*)&ueCb->esmDelList, 0, sizeof(ueCb->esmDelList));
   ueCb->lstMsgHdrType = 0;
   ueCb->proc          = VB_MME_NULL_PROC;
   ueCb->bitmask       = 0;

   /* Reset UE Context except IMSI and Security Context*/
   cmMemcpy(imsi, ueCb->ueCtxt.ueImsi, CM_EMM_MAX_MOBILE_ID_DIGS);
   imsiLen = ueCb->ueCtxt.ueImsiLen;

   cmMemset((U8*)&ueCb->ueCtxt, 0, sizeof(ueCb->ueCtxt));

   cmMemcpy(ueCb->ueCtxt.ueImsi, imsi, CM_EMM_MAX_MOBILE_ID_DIGS);
   ueCb->ueCtxt.ueImsiLen = imsiLen;

   /* Stop Timer */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_INT_MGMT);
   /* Stop all EMM Timer as a precaution */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3422);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3450);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3470);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_UE_CTX_DEL);

   ueCb->ueCtxt.suspendPaging = FALSE;

   RETVALUE(ROK);
} /* end of vbMmeUtlLocDel */


/*
 *
 *       Fun:   vbMmeUtlDelUeCb
 *
 *       Desc:  Deletes UE Cb from hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelUeCb
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlDelUeCb(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   U16             esmIdx    = 0;
   VbMmeHssULRCmd  ulrCmd;

   VB_TRC2(vbMmeUtlDelUeCb)
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Deleting  UE(%d)", ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Deleting  UE(%ld)", ueCb->ueCbKey));
#endif

   vbMmeUtlDelUeCbOnConnId(ueCb);
   vbMmeUtlDelUeCbOnSTmsi(ueCb);
   vbMmeUtlDelUeCbOnImsi(ueCb);
   vbMmeUtlDelPndTrns(ueCb);

   /* Stop Timer */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_INT_MGMT);

   /* Stop all EMM Timer as a precaution */
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3422);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3450);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_EMM_3470);
   vbMmeStopTmr((PTR) ueCb, VB_TMR_UE_CTX_DEL);

   /* Free all esm Cbs */
   for(esmIdx=0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
   {
      vbMmeUtlDelEsmCb(ueCb->esmList[esmIdx], ueCb);
   }

   /* Release S11 tunnel locally */
   if(NULLP != ueCb->ueTunnelCb)
   {
      if (ueCb->ueTunnelCb->localTeIdKey != 0)
      {
         VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
               ueCb->ueTunnelCb->localTeIdKey);
      }
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp),\
            (PTR)ueCb->ueTunnelCb);
      VB_MME_FREE(ueCb->ueTunnelCb, sizeof(VbMmeUeTunnelCb));
   }

   /* Retting the HSS ueStatus flag */
   cmMemset((U8 *)&ulrCmd, 0, sizeof(VbMmeHssULRCmd));
   ulrCmd.imsi.len = ueCb->ueCtxt.ueImsiLen;
   cmMemcpy(ulrCmd.imsi.val, ueCb->ueCtxt.ueImsi, ulrCmd.imsi.len);
   vbMmeHssReset(&ulrCmd);
   vbMmeUtlDelHoData(ueCb);

   /* Freeing UeCb */
   VB_MME_FREE(ueCb, sizeof(VbMmeUeCb));

   RETVALUE(ROK);
} /* end of vbMmeUtlDelUeCb */


/*
 *
 *       Fun:   vbMmeUtlDelPndTrns
 *
 *       Desc:  Deletes UE Cb from hash list
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelPndTrns
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlDelPndTrns(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   U8              indx      = 0;
   VB_TRC2(vbMmeUtlDelPndTrns)
   /* Delete All Pending Transaction */
   for (indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if (ueCb->esmTrnList.esmInfo[indx].status)
      {
         if (ueCb->esmTrnList.esmInfo[indx].esmCb)
         {
            ueCb->esmList[indx] = NULLP;
            VB_MME_FREE(ueCb->esmTrnList.esmInfo[indx].esmCb,\
                        sizeof(VbMmeEsmCb));
         }

         ueCb->esmTrnList.esmInfo[indx].esmCb =  NULL;
         ueCb->esmTrnList.esmInfo[indx].trnId =  0;
         ueCb->esmTrnList.esmInfo[indx].status = 0;
      }
   }
   /* Remove All Queued Procedure */
   for(indx = 0; indx < VB_MME_MAX_SPND_PROC; indx++)
   {
      if(VB_MME_SPND_ESM_EVNT == ueCb->spndProc[indx].choice)
      {
         if(ueCb->spndProc[indx].proc.esmProc.evnt)
         {
           CM_FREE_NASEVNT(&ueCb->spndProc[indx].proc.esmProc.evnt);
         }
      }
      if(VB_MME_SPND_EMM_EVNT == ueCb->spndProc[indx].choice)
      {
         if(ueCb->spndProc[indx].proc.emmProc.evnt)
         {
           CM_FREE_NASEVNT(&ueCb->spndProc[indx].proc.emmProc.evnt);
         }
      }
      if(VB_MME_SPND_S1AP_EVNT == ueCb->spndProc[indx].choice)
      {
         if(ueCb->spndProc[indx].proc.s1apProc.s1apPdu)
         {
           VB_MME_FREE_S1AP_PDU(ueCb->spndProc[indx].proc.s1apProc.s1apPdu);
         }
      }
      cmMemset((U8*)&ueCb->spndProc[indx], 0, sizeof(VbMmeSpndProc));
   }
   RETVALUE(ROK);
}
/*
 *
 *       Fun:   vbMmeUtlDelUeCbOnconnId
 *
 *       Desc:  Deletes UeCb from ueCbHlCp hashlist
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelUeCbOnConnId
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlDelUeCbOnConnId(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlDelUeCbOnConnId)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Deleting  UE(%d)",
                                   ueCb->ueCbKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Deleting  UE(%ld)",
                                    ueCb->ueCbKey));
#endif

   ret = cmHashListDelete(&ueCb->eNodebCb->ueCbHlCp,(PTR)ueCb);

   if(ret != ROK)
   {
#ifdef ALIGN_64BIT
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb(%d)",
                                        ueCb->ueCbKey));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb(%ld)",
                                        ueCb->ueCbKey));
#endif
   }

   /* Reset the key */
   ueCb->ueCbKey = 0;

   RETVALUE(ret);
} /* end of vbMmeUtlDelUeCbOnConnId */

/*
 *
 *       Fun:   vbMmeUtlDelUeCbOnSTmsi
 *
 *       Desc:  Deletes UeCb from ueSTmsiCbHlCp hashlist
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelUeCbOnSTmsi
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlDelUeCbOnSTmsi(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlDelUeCbOnSTmsi)

   ret = cmHashListDelete(&vbMmeCb.ueSTmsiCbHlCp,(PTR)ueCb);

   if(ret != ROK)
   {
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb"));
   }

   RETVALUE(ret);
} /* end of vbMmeUtlDelUeCbOnSTmsi */

/*
 *
 *       Fun:   vbMmeUtlDelUeCbOnImsi
 *
 *       Desc:  Deletes UeCb from ueImsiCbHlCp hashlist
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelUeCbOnImsi
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlDelUeCbOnImsi(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   S16  ret;

   VB_TRC2(vbMmeUtlDelUeCbOnImsi)

   ret = cmHashListDelete(&vbMmeCb.ueImsiCbHlCp,(PTR)ueCb);
   if (ret != ROK)
   {
       VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get UeCb"));
   }

   RETVALUE(ret);
} /* end of vbMmeUtlDelUeCbOnSTmsi */

/*
 *
 *       Fun:   vbMmeUtlAddEsmCb
 *
 *       Desc:  Creates ESM Cb
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAddEsmCb
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   **esmCb,
U8           ueEsmBId,
U8           ueEsmTId
)
#else
PUBLIC S16 vbMmeUtlAddEsmCb(ueCb, esmCb, ueEsmBId, ueEsmTId)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   **esmCb;
U8           ueEsmBId;
U8           ueEsmTId;
#endif
{
   VbMmeEsmCb *newEsmCb;
   U8 i = 0;

   VB_TRC2(vbMmeUtlAddEsmCb)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding ESM Cb"));
   if(ueEsmBId != 0 && ((ueEsmBId < CM_ESM_BEARER_ID_INDX) ||
   (ueEsmBId >= (CM_ESM_MAX_BEARER_ID + CM_ESM_BEARER_ID_INDX))))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Invalid BearerId(%d)",
                                        ueEsmBId ));
      RETVALUE(RFAILED);
   }

   /* Allocate esmCb */
   VB_MME_ALLOC(&newEsmCb, sizeof(VbMmeEsmCb));
   if(newEsmCb == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB046, ERRZERO,
            "Failed To Allocate Memory");
      RETVALUE(RFAILED);
   }

   /* Allocating transaction id */
   newEsmCb->tranId.pres = PRSNT_NODEF;
   newEsmCb->tranId.val  = ++(ueCb->transCntr);

   if(ueEsmBId != 0 && ((ueEsmBId >= CM_ESM_BEARER_ID_INDX) &&
            (ueEsmBId < (CM_ESM_MAX_BEARER_ID + CM_ESM_BEARER_ID_INDX))))
   {
      if(ueCb->esmList[ueEsmBId-CM_ESM_BEARER_ID_INDX] != NULLP)
      {
         VB_MME_FREE(newEsmCb, sizeof(VbMmeEsmCb));
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get Available Beraer Id"));
         RETVALUE(RFAILED);
      }
      newEsmCb->bId = ueEsmBId;
      ueCb->esmList[ueEsmBId-CM_ESM_BEARER_ID_INDX] = newEsmCb;
   }
   else
   {
      /* find out freely available bearerId */
      for(i=0; i < CM_ESM_MAX_BEARER_ID; i++)
      {
         if(ueCb->esmList[i] == NULLP)
         {
            newEsmCb->bId = i + CM_ESM_BEARER_ID_INDX;
            ueCb->esmList[i] = newEsmCb;
            break;
         }
      }
      if(newEsmCb->bId == 0)
      {
         VB_MME_FREE(newEsmCb, sizeof(VbMmeEsmCb));
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Get Available Beraer Id"));
         RETVALUE(RFAILED);
      }
   }
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Esm Bearer Id:(%d)\n", newEsmCb->bId));

   newEsmCb->tId = ueEsmTId;
   newEsmCb->ueCb = ueCb;
   newEsmCb->state = VB_ESM_ST_BEARER_INACTIVE;
   newEsmCb->prevstate = VB_ESM_ST_BEARER_INACTIVE;

   /* Initialize timers */
   cmInitTimers(newEsmCb->esmTimers, VB_ESM_MAX_TIMERS);

   ueCb->creBid.bearerId[ueCb->creBid.nmbBearerId] = newEsmCb->bId;
   ueCb->creBid.nmbBearerId++;

   *esmCb = newEsmCb;
   RETVALUE(ROK);
} /* end of vbMmeUtlAddEsmCb */


/*
 *
 *       Fun:   vbMmeUtlAllocBId
 *
 *       Desc:  Creates ESM Cb
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlAllocBId
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   **esmCb,
U8           *ueEsmBId,
U8           memAllocReq
)
#else
PUBLIC S16 vbMmeUtlAllocBId(ueCb, esmCb, ueEsmBId, memAllocReq)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   **esmCb;
U8           *ueEsmBId;
U8           memAllocReq;
#endif
{
   VbMmeEsmCb *newEsmCb = NULLP;
   VbMmePdnCb *pdnCb    = NULLP;
   U8         indx      = 0;
   U8         ret       = ROK;

   VB_TRC2(vbMmeUtlAllocBId)

   /* Allocate a pdn */
   if(0 == memAllocReq)
   {
      if(CM_ESM_EPS_DEF_BEARER == (*esmCb)->bearerType)
      {
         ret = vbMmeUtlAddPdnCb(ueCb, *esmCb, &pdnCb);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "PDN Allocation Failed"));
            RETVALUE(RFAILED);
         }
      }
   }

   /* find out free available bearerId */
   *ueEsmBId = 0;
   for(indx=0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if(ueCb->esmList[indx] == NULLP)
      {
         *ueEsmBId = indx + CM_ESM_BEARER_ID_INDX;
         break;
      }
   }

   if(0 == *ueEsmBId)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                   "Free Bearer Id Not Found"));
      RETVALUE(RFAILED);
   }

   /*TODO:Need different implementation in future*/
   if(1 == memAllocReq)
   {
      /* Allocate esmCb */
      VB_MME_ALLOC(&newEsmCb, sizeof(VbMmeEsmCb));

      *esmCb = newEsmCb;
      if(newEsmCb == NULLP)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB046, ERRZERO,
            "Failed To Allocate Memory");
         RETVALUE(RFAILED);
      }
      /* Allocating transaction id */
      newEsmCb->tranId.pres = PRSNT_NODEF;
      newEsmCb->tranId.val  = ++(ueCb->transCntr);
   }
   else
   {
      newEsmCb = *esmCb;
   }

   ueCb->esmList[(*ueEsmBId) - CM_ESM_BEARER_ID_INDX] = newEsmCb;
   newEsmCb->bId = *ueEsmBId;
   newEsmCb->ueCb = ueCb;
   newEsmCb->state = VB_ESM_ST_BEARER_INACTIVE;
   newEsmCb->prevstate = VB_ESM_ST_BEARER_INACTIVE;

   /* Initialize timers */
   cmInitTimers(newEsmCb->esmTimers, VB_ESM_MAX_TIMERS);

   ueCb->creBid.bearerId[ueCb->creBid.nmbBearerId++] = newEsmCb->bId;


   RETVALUE(ROK);
} /* end of vbMmeUtlAllocBId */


/*
 *
 *       Fun:   vbMmeUtlCrtEsmCbTrnId
 *
 *       Desc:  Creates ESM Cb
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlCrtEsmCbOnTrnId
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   **esmCb,
U8           ueEsmBId,
U8           ueEsmTId
)
#else
PUBLIC S16 vbMmeUtlCrtEsmCbOnTrnId(ueCb, esmCb, ueEsmBId, ueEsmTId)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   **esmCb;
U8           ueEsmBId;
U8           ueEsmTId;
#endif
{
   VbMmeEsmCb *newEsmCb;

   VB_TRC2(vbMmeUtlCrtEsmCbOnTrnId)

   VB_MME_ALLOC(&newEsmCb, sizeof(VbMmeEsmCb));

   if(newEsmCb == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB046, ERRZERO,
            "Failed to allocate memory");
      RETVALUE(RFAILED);
   }
   /* Allocating transaction id */
   newEsmCb->tranId.pres = PRSNT_NODEF;
   newEsmCb->tranId.val  = ++(ueCb->transCntr);

   if(RFAILED == vbMmeAddToTrnList(ueCb, newEsmCb, ueEsmTId))
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB046, ERRZERO,
            "Adding Transaction List Failed");
      VB_MME_FREE(newEsmCb, sizeof(VbMmeEsmCb));
      RETVALUE(RFAILED);
   }

   newEsmCb->tId = ueEsmTId;
   newEsmCb->ueCb = ueCb;
   newEsmCb->state = VB_ESM_ST_BEARER_INACTIVE;
   newEsmCb->prevstate = VB_ESM_ST_BEARER_INACTIVE;

   /* Initialize timers */
   cmInitTimers(newEsmCb->esmTimers, VB_ESM_MAX_TIMERS);
   *esmCb = newEsmCb;

   RETVALUE(ROK);
} /* end of vbMmeUtlCrtEsmCbOnTrnId */


#ifdef ANSI
PUBLIC S16 vbMmeAddToTrnList
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
U8           ueEsmTId
)
#else
PUBLIC S16 vbMmeAddToTrnList(ueCb, esmCb, ueEsmTId)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
U8           ueEsmTId;
#endif
{
   U8 indx = 0;

   VB_TRC2(vbMmeAddToTrnList)

   /* Check pti already present */
   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if((ueCb->esmTrnList.esmInfo[indx].status) &&
         (ueCb->esmTrnList.esmInfo[indx].trnId ==  ueEsmTId))
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Transaction Already In Progress (%d)",
                                           ueEsmTId));
         RETVALUE(RFAILED);
      }
   }

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if(!ueCb->esmTrnList.esmInfo[indx].status) /*free*/
      {
         ueCb->esmTrnList.esmInfo[indx].esmCb =  esmCb;
         ueCb->esmTrnList.esmInfo[indx].trnId =  ueEsmTId;
         ueCb->esmTrnList.esmInfo[indx].status = 1; /*busy*/
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}/* end of VbMmeAddToTrnList */


#ifdef ANSI
PUBLIC S16 vbMmeRmvFrmTrnList
(
VbMmeUeCb    *ueCb,
U8           ueEsmTId
)
#else
PUBLIC S16 vbMmeRmvFrmTrnList(ueCb, ueEsmTId)
VbMmeUeCb    *ueCb;
U8           ueEsmTId;
#endif
{
   U8 indx = 0;

   VB_TRC2(vbMmeRmvFrmTrnList)

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if((ueCb->esmTrnList.esmInfo[indx].status) &&
         (ueCb->esmTrnList.esmInfo[indx].trnId ==  ueEsmTId))
      {
          ueCb->esmTrnList.esmInfo[indx].status = 0;
          ueCb->esmTrnList.esmInfo[indx].esmCb = NULLP;
          ueCb->esmTrnList.esmInfo[indx].trnId = 0;
          RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}/* end of vbMmeRmvFrmTrnList */



#ifdef ANSI
PUBLIC S16 vbMmeGetFrmTrnList
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   **esmCb,
U8           ueEsmTId
)
#else
PUBLIC S16 vbMmeGetFrmTrnList(ueCb, esmCb, ueEsmTId)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   **esmCb;
U8           ueEsmTId;
#endif
{
   U8 indx = 0;
   VB_TRC2(vbMmeGetFrmTrnList)

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if((ueCb->esmTrnList.esmInfo[indx].status) &&
         (ueCb->esmTrnList.esmInfo[indx].trnId ==  ueEsmTId))
      {
          *esmCb = ueCb->esmTrnList.esmInfo[indx].esmCb;
          RETVALUE(ROK);
      }
   }

   *esmCb = NULLP;

   RETVALUE(RFAILED);
} /* end of VbMmeGetFrmTrnList */


#ifdef ANSI
PUBLIC S16 vbMmeMoveToEsmCbDelList
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb
)
#else
PUBLIC S16 vbMmeMoveToEsmCbDelList(ueCb, esmCb)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
#endif
{
   U8 indx   = 0;
   U8 esmIdx = 0;

   VB_TRC2(vbMmeMoveToEsmDelList)

   /* All linked bearer */
   if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
   {
      for(esmIdx = 0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
      {
         if((ueCb->esmList[esmIdx] != NULLP) &&
            (ueCb->esmList[esmIdx]->bId != esmCb->bId) &&
            (ueCb->esmList[esmIdx]->bearerType != CM_ESM_EPS_DEF_BEARER) &&
            (ueCb->esmList[esmIdx]->linkBearId == esmCb->bId))
         {
            vbMmeMoveToEsmCbDelList(ueCb, ueCb->esmList[esmIdx]);
         }
      }
   }

   ueCb->esmDelList.esmInfo[ueCb->esmDelList.count++].esmCb = esmCb;

   for(esmIdx=0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
   {
      if(ueCb->esmList[esmIdx] != NULLP && ueCb->esmList[esmIdx]->bId == esmCb->bId)
      {
         ueCb->esmList[esmIdx] = NULLP;
         break;
      }
   }
   for(esmIdx = 0; esmIdx < ueCb->creBid.nmbBearerId; esmIdx++)
   {
      if(ueCb->creBid.bearerId[esmIdx] == esmCb->bId)
      {
         break;
      }
   }

   if(indx != ueCb->creBid.nmbBearerId && ueCb->creBid.nmbBearerId != 0)
   {
      for(indx = esmIdx; indx < (ueCb->creBid.nmbBearerId - 1); indx++)
      {
         ueCb->creBid.bearerId[indx] = ueCb->creBid.bearerId[indx+1];
      }
      ueCb->creBid.bearerId[ueCb->creBid.nmbBearerId-1] = 0;
      ueCb->creBid.nmbBearerId--;
   }

   RETVALUE(RFAILED);
} /* end of vbMmeMoveToEsmDelList */

#ifdef ANSI
PUBLIC S16 vbMmeDelAllFrmDelList
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeDelAllFrmDelList(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   U8 indx           = 0;
   VbMmeEsmCb *esmCb = NULLP;

   VB_TRC2(vbMmeDelAllFrmDelList)
   for(indx = 0; indx < ueCb->esmDelList.count; indx++)
   {
      if(NULLP == ueCb->esmDelList.esmInfo[indx].esmCb)
      {
         continue;
      }
      esmCb = ueCb->esmDelList.esmInfo[indx].esmCb;

      if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
      {
         vbMmeUtlDelPdnCb(ueCb, esmCb, esmCb->pdnCb);
      }
      VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
      ueCb->esmDelList.esmInfo[indx].esmCb = NULLP;
   }

   ueCb->esmDelList.count = 0;
   RETVALUE(ROK);
}
/*
 *
 *       Fun:   vbMmeUtlFndEsmCb
 *
 *       Desc:  Finds the esmCb
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlFndEsmCb
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   **esmCb,
U8           ueEsmBId,
U8           ueEsmTId
)
#else
PUBLIC S16 vbMmeUtlFndEsmCb(ueCb, esmCb, ueEsmBId, ueEsmTId)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   **esmCb;
U8           ueEsmBId;
U8           ueEsmTId;
#endif
{
   U8  i;

   VB_TRC2(vbMmeUtlFndEsmCb)

   *esmCb = NULLP;

   if(ueEsmBId != 0)
   {
      /* vb001.101: Checking for valid bearer id, valid = 5 to 15 */
      if(((ueEsmBId < CM_ESM_BEARER_ID_INDX) ||
               (ueEsmBId > (CM_ESM_MAX_BEARER_ID + CM_ESM_BEARER_ID_INDX))))
      {
         RETVALUE(RFAILED);
      }
      for(i=0; i < CM_ESM_MAX_BEARER_ID; i++)
      {
         if(ueCb->esmList[i] != NULLP && ueCb->esmList[i]->bId == ueEsmBId)
         {
            *esmCb = ueCb->esmList[i];
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Esm Cb Found For Bearer Id:(%d)", ueEsmBId));
            RETVALUE(ROK);
         }
      }
   }
   if(ueEsmTId != 0)
   {
      for(i=0; i < CM_ESM_MAX_BEARER_ID; i++)
      {
         if(ueCb->esmList[i] != NULLP && ueCb->esmList[i]->tId == ueEsmTId)
         {
            *esmCb = ueCb->esmList[i];
            VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                     "Esm Cb Found For Transaction Id(%d)", ueEsmTId));
            RETVALUE(ROK);
         }
      }
   }
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Esm Cb Not Found For Bearer Id(%d) And "
            "Transaction Id(%d)", ueEsmBId, ueEsmTId));

   RETVALUE(RFAILED);
} /* end of vbMmeUtlFndEsmCb */

/*
 *
 *       Fun:   vbMmeUtlDelEsmCb
 *
 *       Desc:  Deletes the esmCb
 *
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlDelEsmCb
(
VbMmeEsmCb   *esmCb,
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlDelEsmCb(esmCb, ueCb)
VbMmeEsmCb   *esmCb;
VbMmeUeCb    *ueCb;
#endif
{
   U8 esmIdx = 0;
   U8 indx = 0;

   VB_TRC2(vbMmeUtlDelEsmCb)

   if(NULLP == esmCb)
   {
      /* Debug print here */
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Deleting Esm Cb For Bearer Id(%d)", esmCb->bId));

   /*If UE init proc not grown till bearer id allocation then
     bearer id is 0 */
   if(0 == esmCb->bId)
   {
      vbMmeRmvFrmTrnList(ueCb, esmCb->tId);
   }
   else
   {
      /* Remove pdn and all linked bearer */
      if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
      {
         vbMmeUtlDelPdnCb(ueCb, esmCb, esmCb->pdnCb);
         for(esmIdx = 0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
         {
            if((ueCb->esmList[esmIdx] != NULLP) &&
               (ueCb->esmList[esmIdx]->bId != esmCb->bId) &&
               (ueCb->esmList[esmIdx]->bearerType != CM_ESM_EPS_DEF_BEARER) &&
               (ueCb->esmList[esmIdx]->linkBearId == esmCb->bId))
            {
               vbMmeUtlDelEsmCb(ueCb->esmList[esmIdx], ueCb);
            }
         }
      }
      for(esmIdx=0; esmIdx < CM_ESM_MAX_BEARER_ID; esmIdx++)
      {
         if(ueCb->esmList[esmIdx] != NULLP && ueCb->esmList[esmIdx]->bId == esmCb->bId)
         {
            ueCb->esmList[esmIdx] = NULLP;
            break;
         }
      }
      for(esmIdx = 0; esmIdx < ueCb->creBid.nmbBearerId; esmIdx++)
      {
         if(ueCb->creBid.bearerId[esmIdx] == esmCb->bId)
         {
            break;
         }
      }

      if(indx != ueCb->creBid.nmbBearerId && ueCb->creBid.nmbBearerId != 0)
      {
         for(indx = esmIdx; indx < (ueCb->creBid.nmbBearerId - 1); indx++)
         {
            ueCb->creBid.bearerId[indx] = ueCb->creBid.bearerId[indx+1];
         }
         ueCb->creBid.bearerId[ueCb->creBid.nmbBearerId-1] = 0;
         ueCb->creBid.nmbBearerId--;
      }
   }

   /*  Stop all ESM timer as a precaution */
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3485);
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3486);
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3489);
   vbMmeStopTmr((PTR)esmCb, VB_TMR_ESM_3495);

   VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
   RETVALUE(ROK);
} /* end of vbMmeUtlDelEsmCb */


/*
 *
 *       Fun:   vbMmeUtlAllocConId
 *
 *       Desc:  Allocates a new service user connection identifier
 *              for a new UE connection control block on S1
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */

#ifdef ANSI
PUBLIC S16 vbMmeUtlAllocConId
(
VbMmeEnbCb  *eNbCb,
UConnId     *newSuConnId
)
#else
PUBLIC S16 vbMmeUtlAllocConId(eNbCb, newSuConnId)
VbMmeEnbCb  *eNbCb;
UConnId     *newSuConnId;
#endif
{
   *newSuConnId = GetLoWord(eNbCb->peerId);

   /* Insert PeerId in the first half [HSW] */
   *newSuConnId = PutHiWord(*newSuConnId, eNbCb->peerId);

   ++eNbCb->ueConnIdCntr;

   /* Insert actual Ue Id in the second half [LSW] */
   *newSuConnId = PutLoWord(*newSuConnId, eNbCb->ueConnIdCntr);

   RETVALUE(ROK);
}/*vbMmeUtlAllocConId */

/*
 *
 *       Fun:   vbMmeUtlFreeConId
 *
 *       Desc:  Frees up the service user connection identifier
 *              for an existing UE connection control block on S1.
 *              This is invoked whenever we delete the UE Connection
 *              block, to make sure we dont run out of user connection
 *              identifiers.
 *
 *       Ret:   ROK      - ok
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */

#ifdef ANSI
PUBLIC S16 vbMmeUtlFreeConId
(
 VbMmeEnbCb  *eNbCb
 )
#else
PUBLIC S16 vbMmeUtlFreeConId(eNbCb)
   VbMmeEnbCb  *eNbCb;
#endif
{
   eNbCb->ueConnIdCntr--;
   RETVALUE(ROK);
}/*vbMmeUtlFreeConId */

/* vb005.101 : UeId Index calculation ccpu0011748 */
/*
 *
 *       Fun:  vbMmeUtlChar2BCD
 *
 *       Desc:  This function converts Character
 *              to BCD.
 *
 *
 *       Ret:   ROK        success
 *
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */

#ifdef ANSI
PUBLIC S16 vbMmeUtlChar2BCD
(
 U8          *imsi,
 U8          imsiLen,
 U32         *imsiInt
 )
#else
PUBLIC S16 vbMmeUtlChar2BCD(imsi, imsiLen, imsiInt)
   U8          *imsi;
   U8          imsiLen;
   U32         *imsiInt
#endif
{
   U32 temp = 0;
   U8  idx;

   for (idx=0; idx<imsiLen; idx++)
   {
      temp = temp*10 + imsi[idx];
   }
   *imsiInt = temp;

   RETVALUE (ROK);
}

/* ======================================================= */
/*
 *
 *       Fun:   vbMmeUtlBldS1PagingReq
 *
 *       Desc:  This function builds S1 Paging Request PDU
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldS1PagingReq
(
VbMmeEnbCb  *eNbCb,    /* eNodeB Cb */
VbMmeUeCb   *ueCb,     /* UE Cb */
Bool        sTmsiBased,
S1apPdu     **s1PagingReqPduPtr /* S1 Paging Request Pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldS1PagingReq(eNbCb, ueCb, s1PagingReqPduPtr)
VbMmeEnbCb  *eNbCb;    /* eNodeB Cb */
VbMmeUeCb   *ueCb;     /* UE Cb */
Bool        sTmsiBased;
S1apPdu     **s1PagingReqPduPtr; /* S1 Paging Request Pdu to be built */
#endif
{
   S1apPdu *szPagingPdu = NULLP;
   S16  ret;
   Mem  mem;
   U8   pLMNId[10];
   U16  len;
   U32  idx;
   SztProtIE_Field_PagIEs *ie;
   SztProtIE_Field_TAIItemIEs *ie1;
   U32 imsiInt = 0;
   U16  numComp;

   VB_TRC2(vbMmeUtlBldS1PagingReq);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Paging Request"));

   /* Initialize the pdu ptr with NULLP for failure cases */
   *s1PagingReqPduPtr = NULLP;
   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&szPagingPdu);
   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "cmAllocEvnt Failed");

      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(szPagingPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(szPagingPdu->pdu.choice), SZT_TRGR_INTI_MSG);
   vbUtlFillTknU8(&(szPagingPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(szPagingPdu->pdu.val.initiatingMsg.procedureCode),
                  Sztid_Pag);
   vbUtlFillTknU32(&(szPagingPdu->pdu.val.initiatingMsg.criticality),
                  SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.pres), PRSNT_NODEF);

   /* Filling 5 mandatory headers */
   numComp = 5;
   vbUtlFillTknU16(&(szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.noComp), numComp);

   if ((cmGetMem(szPagingPdu, (numComp * sizeof(SztProtIE_Field_PagIEs)),
       (Ptr *)&szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO, "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   /**********************************************
    IE1 - UE identity index value
    **********************************************/
   ie = &szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_UEIdentityIdxValue);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   /*vb005.101 : UeId Index calculation ccpu0011748*/
   vbMmeUtlChar2BCD(ueCb->ueCtxt.ueImsi, ueCb->ueCtxt.ueImsiLen, &imsiInt);
   ie->value.u.sztUEIdentityIdxValue.val[0] =(U8)((imsiInt%1024)>> 2) ;
   ie->value.u.sztUEIdentityIdxValue.val[1] = (U8)(((imsiInt%1024) &  0x03) << 6) ;
   ie->value.u.sztUEIdentityIdxValue.len = 10;
   ie->value.u.sztUEIdentityIdxValue.pres = PRSNT_NODEF;

   /****************************************************************
   IE2 - UE Paging Id
   *****************************************************************/
   ie = &szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_UEPagID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   if(sTmsiBased != TRUE)
   {
      vbUtlFillTknU8(&(ie->value.u.sztUEPagID.choice), UEPAGID_IMSI);
      /* Fill IMSI digits to Imsi Octect ( 2 digits in a single byte) */
      vbUtlFillTknStrOSXL2(&(ie->value.u.sztUEPagID.val.iMSI),
               ueCb->ueCtxt.ueImsiLen, ueCb->ueCtxt.ueImsi, &szPagingPdu);
    }
    else
    {
      vbUtlFillTknU8(&(ie->value.u.sztUEPagID.choice), UEPAGID_S_TMSI);
      vbUtlFillTknU8(&(ie->value.u.sztUEPagID.val.s_TMSI.pres), PRSNT_NODEF);
      len = 1;
      vbUtlFillTknStr4(&(ie->value.u.sztUEPagID.val.s_TMSI.mMEC), (U8)len, ueCb->ueCtxt.ueGuti.mmeCode);
      len = 4;
      vbUtlFillTknStrOSXL1(&(ie->value.u.sztUEPagID.val.s_TMSI.m_TMSI), len, ueCb->ueCtxt.ueGuti.mTMSI, &szPagingPdu);
    }

   /*****************************************************************
   IE3 - Paging DRX
   ******************************************************************/
   ie = &szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member[2];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_pagingDRX);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU32(&(ie->value.u.sztPagDRX), eNbCb->enbStupInfo.dfltPgingDRX);

   /*****************************************************************
   IE4 - Filling paging cause
   *****************************************************************/
   ie = &szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member[3];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_CNDomain);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU32(&(ie->value.u.sztCNDomain), SztCNDomainpsEnum);

   /*****************************************************************
   IE5 - Filling paging TAI list
   ******************************************************************/
   ie = &szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member[4];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_TAILst);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   numComp = ueCb->ueCtxt.taList.len;
   /*numComp = 1;*/
   vbUtlFillTknU16(&(ie->value.u.sztTAILst.noComp), numComp);

   if ((cmGetMem(szPagingPdu, (numComp * sizeof(SztProtIE_Field_TAIItemIEs)),
       (Ptr *)&(ie->value.u.sztTAILst.member)) !=ROK))
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "vbMmeUtlBldS1PagingReq: cmAllocEvnt failed");
      RETVALUE(RFAILED);
   }

   /* Fill TA's list in S1AP paging message */
   for (idx = 0; idx < numComp; idx++)
   {
      ie1 = &(ie->value.u.sztTAILst.member[idx]);

      vbUtlFillTknU8(&(ie1->pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(ie1->id), Sztid_TAIItem);
      vbUtlFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
      vbUtlFillTknU8(&(ie1->value.u.sztTAIItem.pres), PRSNT_NODEF);
      vbUtlFillTknU8(&(ie1->value.u.sztTAIItem.tAI.pres), PRSNT_NODEF);

      /* make plmn Identity from mcc and mnc array */
      cmMemset(pLMNId,0,sizeof(pLMNId));
      vbUtlMccMncToPlmn((U8 *)&pLMNId, &(ueCb->ueCtxt.taList.partLst[idx].u.type1.tai));

      len = 3;
      vbUtlFillTknStrOSXL(&(ie1->value.u.sztTAIItem.tAI.pLMNidentity), len, pLMNId, &szPagingPdu);
      len = 2;

      /* Filling TAC from taList stored inside Ue Context */
      ie1->value.u.sztTAIItem.tAI.tAC.pres = PRSNT_NODEF;
      ie1->value.u.sztTAIItem.tAI.tAC.len = len;
      ie1->value.u.sztTAIItem.tAI.tAC.val[0] = (U8)((ueCb->ueCtxt.taList.partLst[idx].u.type1.tai.tac & 0xFF00) >> 8);
      ie1->value.u.sztTAIItem.tAI.tAC.val[1] = (U8)(ueCb->ueCtxt.taList.partLst[idx].u.type1.tai.tac & 0x00FF);
  }

#if 0
   /* Currently not supported CSG list */
   /* adding CSG Id list */
   /* IE6 - Filling CSG ID list */
   ie = &szPagingPdu->pdu.val.initiatingMsg.value.u.sztPag.protocolIEs.member[5];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_CSG_IdLst);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   numComp = 1;
   vbUtlFillTknU16(&(ie->value.u.sztCSG_IdLst.noComp), numComp);

   if ((cmGetMem(szPagingPdu, numComp * sizeof(SztCSG_IdLst_Item),
             (Ptr*)&(ie->value.u.sztCSG_IdLst.member))) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "vbMmeUtlBldS1PagingReq: cmAllocEvnt failed");
      RETVALUE(RFAILED);
   }

   ie2 = &ie->value.u.sztCSG_IdLst.member[0];
   vbUtlFillTknU8(&(ie2->pres), PRSNT_NODEF);
   len = 27;
   vbUtlFillTknBStr32WithVal(&(ie2->cSG_Id), len, 100);
#endif

  *s1PagingReqPduPtr = szPagingPdu;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:   vbMmeUtlVldS1SetupPlmn
 *
 *       Desc:  This function validates received plmn from eNodeB.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlVldS1SetupPlmn
(
SupportedTAs *tAs
)
#else
PUBLIC S16 vbMmeUtlVldS1SetupPlmn(tAs)
SupportedTAs *tAs;
#endif
{
    UNUSED(tAs);
    RETVALUE(ROK);
}


/*
 *
 *       Fun:   vbMmeUtlBldS1SetupRsp
 *
 *       Desc:  This function builds an S1 Setup Response PDU
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldS1SetupRsp
(
VbMmeEnbCb  *eNbCb,    /* eNodeB Cb */
S1apPdu     **s1SetupRspPduPtr /* S1 Setup Response Pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldS1SetupRsp(eNbCb, s1SetupRspPduPtr)
VbMmeEnbCb  *eNbCb;    /* eNodeB Cb */
S1apPdu     **s1SetupRspPduPtr; /* S1 Setup Response Pdu to be built */
#endif
{

   S16 ret = ROK;
   SztProtIE_Field_S1SetupRespIEs *ie;
   SztSrvdGUMMEIsItem             *gummei;
   SztMME_Group_ID                *mmeGrpId1;
   SztMME_Code                    *mmecode1;
   U16                            indx;
   U8                             plmnId[3];


   SztPLMNidentity *plmnItem;
   VbGUMMEI *vbGummei;
   Mem mem;
   S1apPdu  *s1SetupRspPdu = NULLP;
   U16 numComp = 0;
   U16 noOfGummei = 0;
   U16 idx = 0;
   U8 len = 0;

   VB_TRC2(vbMmeUtlBldS1SetupRsp);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Setup Response"));


   /* Initialize the pdu ptr with NULLP for failure cases */
   *s1SetupRspPduPtr = NULLP;
   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&s1SetupRspPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "vbMmeUtlBldS1SetupRsp: cmAllocEvnt failed");

      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(s1SetupRspPdu->pdu.choice), VB_S1AP_SUCCESFUL_OUTCOME);
   vbUtlFillTknU8(&(s1SetupRspPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(s1SetupRspPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_S1Setup);
   vbUtlFillTknU32(&(s1SetupRspPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(s1SetupRspPdu->pdu.val.successfulOutcome.value.u.
            sztS1SetupResp.pres),PRSNT_NODEF);

   /* Filling 3 mandatory IEs */

   numComp = 3;
   vbUtlFillTknU16(&(s1SetupRspPdu->pdu.val.successfulOutcome.value.u.
            sztS1SetupResp.protocolIEs.noComp),numComp);

   /* Allocte memory for 4 IEs */
   if ((cmGetMem(s1SetupRspPdu, numComp*sizeof(SztProtIE_Field_S1SetupRespIEs)
               ,(Ptr*)&s1SetupRspPdu->pdu.val.successfulOutcome.value.u.
               sztS1SetupResp.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB051, ERRZERO,
            "vbUtlBldS1SetupRsp: cmGetMem failed failed\n");
      cmFreeMem((Ptr)s1SetupRspPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME Name */
   ie = &s1SetupRspPdu->pdu.val.successfulOutcome.value.u.sztS1SetupResp.
      protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_MMEname);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);

   ret = vbUtlFillTknStrOSXL(&(ie->value.u.sztMMEname),
                             vbMmeCb.mmeCfg.mmeName.length,
                             vbMmeCb.mmeCfg.mmeName.name, &s1SetupRspPdu);

   if(ROK != ret)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB052, ERRZERO,
            "vbUtlBldS1SetupRsp:Filling MME Name failed");
      cmFreeMem((Ptr)s1SetupRspPdu);
      RETVALUE(RFAILED);
   }

   /* IE2 - Filling served  GUMMEIs*/
   ie = &s1SetupRspPdu->pdu.val.successfulOutcome.value.u.sztS1SetupResp.
      protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_SrvdGUMMEIs);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   noOfGummei = vbMmeCb.mmeCfg.srvGLst.noOfGummeis;

   /* IE2 - Allocate memory for GUMMEIs */
   if ((!noOfGummei) ||
         ((cmGetMem(s1SetupRspPdu, noOfGummei*sizeof(SztSrvdGUMMEIsItem),
          (Ptr*)&ie->value.u.sztSrvdGUMMEIs.member))) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB053, ERRZERO,
            "vbUtlBldS1SetupRsp: cmGetMem failed while filling sztSrvdGUMMEIs\n");
      cmFreeMem((Ptr)s1SetupRspPdu);
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU16(&(ie->value.u.sztSrvdGUMMEIs.noComp),noOfGummei);
   for (idx = 0; idx < noOfGummei; idx++)
   {
      gummei = &(ie->value.u.sztSrvdGUMMEIs.member[idx]);
      vbGummei = &(vbMmeCb.mmeCfg.srvGLst.gummeiLst[idx]);
      numComp =  vbGummei->pLst.noOfPlmns ;
      vbUtlFillTknU8(&(gummei->pres),PRSNT_NODEF);
      if ((!numComp) || ((cmGetMem(s1SetupRspPdu, numComp*sizeof(SztPLMNidentity),
                  (Ptr*)&gummei->servedPLMNs.member)) !=ROK))
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB054, ERRZERO,
               "vbUtlBldS1SetupRsp: cmGetMem failed filling SztPLMNidentity\n");
         cmFreeMem((Ptr)s1SetupRspPdu);
         RETVALUE(RFAILED);
      }

      vbUtlFillTknU16(&(gummei->servedPLMNs.noComp),numComp);
      for(indx = 0; indx < numComp; indx++)
      {
         plmnItem = &(gummei->servedPLMNs.member[indx]);

         /* + Convert NAS to S1AP format + */
         plmnId[0] = vbGummei->pLst.plmnLst[indx].plmnId[0];
         if(0xF0 == (vbGummei->pLst.plmnLst[indx].plmnId[1] & 0xF0))
         {
            plmnId[1] = vbGummei->pLst.plmnLst[indx].plmnId[1];
            plmnId[2] = vbGummei->pLst.plmnLst[indx].plmnId[2];
         }
         else
         {
            plmnId[1] = ((vbGummei->pLst.plmnLst[indx].plmnId[2] & 0x0F) <<4) |
                        (vbGummei->pLst.plmnLst[indx].plmnId[1] & 0x0F);

            plmnId[2] = (vbGummei->pLst.plmnLst[indx].plmnId[1] & 0xF0) |
                        ((vbGummei->pLst.plmnLst[indx].plmnId[2] & 0xF0) >>4);
         }
         /* - Convert NAS to S1AP format - */

         ret = vbUtlFillTknStrOSXL(plmnItem, 3,
                plmnId, &s1SetupRspPdu);
         if(ROK != ret)
         {
            VB_MME_LOGERROR(ERRCLS_DEBUG, EVB055, ERRZERO,
                  "vbUtlBldS1SetupRsp:Filling PlmnId failed");
            cmFreeMem((Ptr)s1SetupRspPdu);
            RETVALUE(RFAILED);
         }
      }
      numComp = vbGummei->grpIdLst.noOfGrpIds ;
      if ((!numComp) || ((cmGetMem(s1SetupRspPdu, numComp*sizeof(SztMME_Group_ID),
                  (Ptr*)&gummei->servedGroupIDs.member)) !=ROK))
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB056, ERRZERO,
               "vbUtlBldS1SetupRsp:Filling GroupIds failed");
         cmFreeMem((Ptr)s1SetupRspPdu);
         RETVALUE(RFAILED);
      }
      vbUtlFillTknU16(&(gummei->servedGroupIDs.noComp),numComp);
      for(indx = 0; indx < numComp; indx++)
      {
         mmeGrpId1 = &(gummei->servedGroupIDs.member[indx]);
         len = sizeof(vbGummei->grpIdLst.grpId[indx]) ;
         /*ret = vbUtlFillTknStr4(mmeGrpId1, len, vbGummei->grpIdLst.grpId[indx]);*/
         ret = vbUtlFill16TknStr4(mmeGrpId1, len, vbGummei->grpIdLst.grpId[indx]);
         if(ROK != ret)
         {
            VB_MME_LOGERROR(ERRCLS_DEBUG, EVB057, ERRZERO,
                  "vbUtlBldS1SetupRsp:Filling GroupId failed");
            cmFreeMem((Ptr)s1SetupRspPdu);
            RETVALUE(RFAILED);
         }
      }

      numComp = vbGummei->codeLst.noOfCodes;
      vbUtlFillTknU16(&(gummei->servedMMECs.noComp),numComp);

      if  ((!numComp) || ((cmGetMem(s1SetupRspPdu, numComp*sizeof(SztMME_Code),
                  (Ptr*)&gummei->servedMMECs.member)) !=ROK))
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB058, ERRZERO,
               "vbUtlBldS1SetupRsp:Filling MME Code failed");
         cmFreeMem((Ptr)s1SetupRspPdu);
         RETVALUE(RFAILED);
      }

      for(indx = 0; indx < numComp; indx++)
      {

         mmecode1 = &(gummei->servedMMECs.member[indx]);

         /* No Of Component is 1 as propagated from the last */
         vbUtlFillTknU16(&(gummei->servedMMECs.noComp),numComp);
         mmecode1 = &(gummei->servedMMECs.member[indx]);
         len = sizeof(vbGummei->codeLst.mmeCode[indx]);
         vbUtlFillTknStr4(mmecode1, len, vbGummei->codeLst.mmeCode[indx]);
      }

      gummei->iE_Extns.noComp.pres = NOTPRSNT;
   }


   /* IE4 - Filling Relative MME Capacity */
   ie = &(s1SetupRspPdu->pdu.val.successfulOutcome.value.u.sztS1SetupResp.
         protocolIEs.member[2]);

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_RelativeMMECapacity);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);

   vbUtlFillTknU32(&(ie->value.u.sztRelativeMMECapacity), LVB_MME_REL_CAP);
   /*Assign the Pointer to the corresponding Msg PDU*/
   *s1SetupRspPduPtr = s1SetupRspPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldS1SetupRsp */

/*
 *
 *       Fun:   vbMmeUtlBldS1SetupFail
 *
 *       Desc:  This function builds an S1 Setup Failure PDU
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldS1SetupFail
(
VbMmeEnbCb   *eNbCb,    /* eNodeB Cb */
S1apPdu     **s1SetupFailPduPtr, /* S1 Setup Fail Pdu to be built */
U8            type,   /* Type of error that occurred */
U8            cause   /* cause value */
)
#else
PUBLIC S16 vbMmeUtlBldS1SetupFail(eNbCb, s1SetupFailPduPtr, type, cause)
VbMmeEnbCb   *eNbCb;    /* eNodeB Cb */
S1apPdu     **s1SetupFailPduPtr; /* S1 Setup Fail Pdu to be built */
U8            type;   /* Type of error that occurred */
U8            cause;   /* cause value */
#endif
{
   S16 ret = ROK;
   U8 numComp;
   Mem mem;
   S1apPdu *s1SetupFailPdu;
   SztProtIE_Field_S1SetupFailIEs *ie = NULLP;

   VB_TRC2(vbMmeUtlBldS1SetupFail);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Setup Failure Message"));

   *s1SetupFailPduPtr = NULLP;
   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&s1SetupFailPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB059, ERRZERO,
            "vbMmeUtlBldS1SetupFail: cmAllocEvnt  failed");
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(s1SetupFailPdu->pdu.choice), VB_S1AP_UNSUCCESFUL_OUTCOME);
   vbUtlFillTknU8(&(s1SetupFailPdu->pdu.val.unsuccessfulOutcome.pres),
         PRSNT_NODEF);
   vbUtlFillTknU32(&(s1SetupFailPdu->pdu.val.unsuccessfulOutcome.procedureCode),
         Sztid_S1Setup);
   vbUtlFillTknU32(&(s1SetupFailPdu->pdu.val.unsuccessfulOutcome.criticality),
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(s1SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.
            sztS1SetupFail.pres),PRSNT_NODEF);

   /* Filling 1 mandatory header */

   numComp = 1;
   vbUtlFillTknU16(&(s1SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.
            sztS1SetupFail.protocolIEs.noComp),numComp);

   if ((cmGetMem(s1SetupFailPdu, numComp*sizeof(SztProtIE_Field_S1SetupFailIEs)
               ,(Ptr*)&s1SetupFailPdu->pdu.val.unsuccessfulOutcome.value.
               u.sztS1SetupFail.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB060, ERRZERO,
            "vbMmeUtlBldS1SetupFail: cmGetMem failed failed\n");
      cmFreeMem((Ptr)s1SetupFailPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling cause */
   ie = &s1SetupFailPdu->pdu.val.unsuccessfulOutcome.value.u.sztS1SetupFail.
      protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_Cause);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(ie->value.u.sztCause.choice),type);
   vbUtlFillTknU32(&(ie->value.u.sztCause.val.radioNw),cause);

   *s1SetupFailPduPtr = s1SetupFailPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldS1SetupFail */

/*
 *
 *       Fun:   vbMmeUtlBldS1Reset
 *
 *       Desc:  This function builds an S1 Reset PDU
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldS1Reset
(
S1apPdu          **s1ResetPduPtr, /* S1 Reset Pdu to be built */
SztCause           *cause,  /* Need to fill since it is mandatory field */
U8                 flag /* if TRUE then list of ueassociated connection will present*/
)
#else
PUBLIC S16 vbMmeUtlBldS1Reset(s1ResetPduPtr, cause, flag)
S1apPdu     **s1ResetPduPtr; /* S1 Reset Pdu to be built */
SztCause             *cause;
U8                   flag;
#endif
{
   S1apPdu  *s1ResetPdu = NULLP;
   SztProtIE_Field_ResetIEs *ie = NULLP;
   SztProtIE_SingleCont_UE_assocLogS1_ConItemRes *ie1 = NULLP;
   S16 ret = ROK;
   Mem      mem;
   U16 numComp = 0;
   U16 idx = 0;

   VB_TRC2(vbMmeUtlBldS1Reset);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Reset"));


   /* Initialize the pdu ptr with NULLP for failure cases */
   *s1ResetPduPtr = NULLP;
   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&s1ResetPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "vbMmeUtlBldS1Reset: cmAllocEvnt failed");

      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(s1ResetPdu->pdu.choice), VB_S1AP_INIT_MSG);
   vbUtlFillTknU8(&(s1ResetPdu->pdu.val.initiatingMsg.pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(s1ResetPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_Reset);
   vbUtlFillTknU32(&(s1ResetPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(s1ResetPdu->pdu.val.initiatingMsg.value.u.
            sztReset.pres),PRSNT_NODEF);

   /* Filling 2 Mandatory IEs */
   numComp = 2;
   vbUtlFillTknU16(&(s1ResetPdu->pdu.val.initiatingMsg.value.u.sztReset.protocolIEs.noComp),
         numComp);

   /* Get memory for numComp IEs */
   if ((cmGetMem(s1ResetPdu, numComp * sizeof(SztProtIE_Field_ResetIEs),\
               (Ptr*)&s1ResetPdu->pdu.val.initiatingMsg.value.u.sztReset.\
               protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "vbMmeUtlBldS1Reset: Getting memory for protoColIEs failed");
      RETVALUE(RFAILED);
   }


   /* IE1 */
   ie = &s1ResetPdu->pdu.val.initiatingMsg.value.u.sztReset.\
        protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_Cause);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(ie->value.u.sztCause.choice), cause->choice.val);
   vbUtlFillTknU32(&(ie->value.u.sztCause.val.transport), cause->val.transport.val);

   /* IE2 */
   ie = &s1ResetPdu->pdu.val.initiatingMsg.value.u.sztReset.\
        protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_ResetTyp);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);


   if (FALSE == flag) /* for complete interface of S1 */
   {
      vbUtlFillTknU8(&(ie->value.u.sztResetTyp.choice), 0);
      vbUtlFillTknU32(&(ie->value.u.sztResetTyp.val.s1_Intf), SztResetAllreset_allEnum);
   }
   else
   {
      vbUtlFillTknU8(&(ie->value.u.sztResetTyp.choice), 1);
      /* Fill the value no of connection exist currently in the n/w */
      /* As of now using only 1 */
      numComp = 1;
      vbUtlFillTknU16(&(ie->value.u.sztResetTyp.val.partOfS1_Intf.noComp), numComp);
      if ((cmGetMem(s1ResetPdu, numComp*sizeof(SztProtIE_SingleCont_UE_assocLogS1_ConItemRes),\
                                      (Ptr*)&ie->value.u.sztResetTyp.val.partOfS1_Intf.member)) !=ROK)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
             "vbMmeUtlBldS1Reset: Getting memory for UE Associated Logical Connection list failed");

              RETVALUE(RFAILED);
      }
      for (idx = 0; idx < numComp; idx++)
      {
         ie1 = &ie->value.u.sztResetTyp.val.partOfS1_Intf.member[idx];
         vbUtlFillTknU8(&(ie1->pres),PRSNT_NODEF);
         vbUtlFillTknU32(&(ie1->id),Sztid_UE_assocLogS1_ConItem);
         vbUtlFillTknU32(&(ie1->criticality),SztCriticalityrejectEnum);
         vbUtlFillTknU8(&(ie1->value.u.sztUE_assocLogS1_ConItem.pres), PRSNT_NODEF);
         /* As of now supporting only for 1 UE connection later need to update */
         vbUtlFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID), 1);
         vbUtlFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID), 1);
      }

   }

   /*Assign the Pointer to the corresponding Msg PDU*/
   *s1ResetPduPtr = s1ResetPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldS1Reset */


/*
 *
 *       Fun:   vbMmeUtlBldS1ResetAck
 *
 *       Desc:  This function builds an S1 Reset Ack Response PDU
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldS1ResetAck
(
U8                   flag,
SztUDatEvnt          *uDatEvnt,
S1apPdu          **s1ResetAckPduPtr /* S1 Reset Pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldS1ResetAck(flag, uDatEvnt, s1ResetAckPduPtr)
U8                   flag;
SztUDatEvnt          *uDatEvnt;
S1apPdu     **s1ResetAckPduPtr; /* S1 Reset Ack Pdu to be built */
#endif
{
   SztS1AP_PDU   *s1ap_pdu = NULLP;
   S1apPdu  *s1ResetAckPdu = NULLP;
   SztProtIE_Field_ResetAckgIEs *ie = NULLP;
   SztProtIE_SingleCont_UE_assocLogS1_ConItemResAck *ie1 = NULLP;
   SztProtIE_Field_ResetIEs     *ie2 = NULLP;
   SztProtIE_SingleCont_UE_assocLogS1_ConItemRes *firstItem = NULLP;
   S16 ret = ROK;
   Mem      mem;
   U16 numComp = 0;
   U16 idx = 0;

   VB_TRC2(vbMmeUtlBldS1ResetAck);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Reset Ack"));


   /* Initialize the pdu ptr with NULLP for failure cases */
   *s1ResetAckPduPtr = NULLP;
   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&s1ResetAckPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB050, ERRZERO,
           "vbMmeUtlBldS1ResetAck: cmAllocEvnt failed");

      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(s1ResetAckPdu->pdu.choice), VB_S1AP_SUCCESFUL_OUTCOME);
   vbUtlFillTknU8(&(s1ResetAckPdu->pdu.val.successfulOutcome.pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(s1ResetAckPdu->pdu.val.successfulOutcome.procedureCode),
         Sztid_Reset);
   vbUtlFillTknU32(&(s1ResetAckPdu->pdu.val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(s1ResetAckPdu->pdu.val.successfulOutcome.value.u.
            sztResetAckg.pres),PRSNT_NODEF);
   vbUtlFillTknU16(&(s1ResetAckPdu->pdu.val.successfulOutcome.value.u.
            sztResetAckg.protocolIEs.noComp),NOTPRSNT);

   if (TRUE == flag)
   {
      numComp = 1;
      vbUtlFillTknU16(&(s1ResetAckPdu->pdu.val.successfulOutcome.value.u.
            sztResetAckg.protocolIEs.noComp), numComp);

      /* Allocte memory for 1 IE */
      if ((cmGetMem(s1ResetAckPdu, numComp*sizeof(SztProtIE_Field_ResetAckgIEs)
                  ,(Ptr*)&s1ResetAckPdu->pdu.val.successfulOutcome.value.u.
                  sztResetAckg.protocolIEs.member)) !=ROK)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB051, ERRZERO,
               "vbUtlBldS1ResetAck: cmGetMem failed failed\n");
         cmFreeMem((Ptr)s1ResetAckPdu);
         RETVALUE(RFAILED);
      }

      /* Find the Item from list received in Reset message need to fil in reset ack */
      s1ap_pdu = &(uDatEvnt->pdu->pdu);
      /* Retrieve no of item present in the list of s1 reset message*/
      ie2 = &s1ap_pdu->val.initiatingMsg.value.u.sztReset.protocolIEs.member[1];
      /* Number of item present in the list */
      numComp = ie2->value.u.sztResetTyp.val.partOfS1_Intf.noComp.val;
      /* Pointer to first element in the list */
      firstItem = &ie2->value.u.sztResetTyp.val.partOfS1_Intf.member[0];

      /* IE1 - Filling UE- Associated logical s1- connection list */
      ie = &s1ResetAckPdu->pdu.val.successfulOutcome.value.u.sztResetAckg.
         protocolIEs.member[0];
      vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
      vbUtlFillTknU32(&(ie->id),Sztid_UE_assocLogS1_ConLstResAck);
      vbUtlFillTknU32(&(ie->criticality),SztCriticalityignoreEnum);
      vbUtlFillTknU16(&(ie->value.u.sztUE_assocLogS1_ConLstResAck.noComp), numComp);

      if ((cmGetMem(s1ResetAckPdu, numComp *
                  sizeof(SztProtIE_SingleCont_UE_assocLogS1_ConItemResAck),
                  (Ptr *)&ie->value.u.sztUE_assocLogS1_ConLstResAck.member)) !=ROK)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB065, ERRZERO, "cmGetMem Failed");
         cmFreeMem((Ptr)s1ResetAckPdu);
         RETVALUE(RFAILED);
      }

      /* IE4 - Fill in UE - Associated S1- logical connection list items */
      for(idx = 0; idx < numComp; idx++)
      {
         /* Iterate through list and fill */

         ie1 = &ie->value.u.sztUE_assocLogS1_ConLstResAck.member[idx];

         vbUtlFillTknU8(&(ie1->pres),PRSNT_NODEF);
         vbUtlFillTknU32(&(ie1->id), Sztid_UE_assocLogS1_ConItem);
         vbUtlFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);

         vbUtlFillTknU8(&(ie1->value.u.sztUE_assocLogS1_ConItem.pres), PRSNT_NODEF);
         vbUtlFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID),
                                (firstItem + idx)->value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.val);
         vbUtlFillTknU32(&(ie1->value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID),
                                (firstItem + idx)->value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.val);
      }
   }

   /*Assign the Pointer to the corresponding Msg PDU*/
   *s1ResetAckPduPtr = s1ResetAckPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldS1ResetAck */


/*
 *
 *       Fun:   vbMmeUtlBldInitCxtSetupReq
 *
 *       Desc:  This function builds an Initial context setup request PDU
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldInitCxtSetupReq
(
VbMmeUeCb  *ueCb,    /* UE Cb */
SztNAS_PDU *sztNAS_PDU,     /* new param for nasPdu */
S1apPdu   **initCxtSetupReqPduPtr /* S1 Initial Context Setup Request Pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldInitCxtSetupReq(ueCb,sztNAS_PDU,initCxtSetupReqPduPtr)
VbMmeUeCb  *ueCb;    /* UE Cb */
SztNAS_PDU *sztNAS_PDU;     /* new param for nasPdu */
S1apPdu   **initCxtSetupReqPduPtr; /* S1 Initial Context Setup Request Pdu to be built */
#endif
{
   S16  ret;
   Mem  mem;
   U16  idx;
   U16  numComp;
   U8   gTPId[10];
   U8   tptAddr[16];
   U8   dlBitrate[12];
   U8   ulBitrate[12];
   U16  len;
   U8   encAlgo;
   U8   intProtAlgo;
   SztProtIE_Field_InitCntxtSetupRqstIEs *ie;
   SztProtIE_SingleCont_E_RABToBeSetupItemCtxtSUReqIEs *ie1;
   VbMmeEsmCb *esmCb;
   S1apPdu *initCxtSetupReqPdu;
   Data     val[4];
   Bool     nasPduFilled = FALSE;
#ifndef VB_MME_NAS_SEC
   U8       nullKenb[CM_EMM_MAX_KENB] = {'\0'};
#endif

   VB_TRC2(vbMmeUtlBldInitCxtSetupReq);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Inital Context Setup Request"));

   *initCxtSetupReqPduPtr = NULLP;

   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&initCxtSetupReqPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB061, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(initCxtSetupReqPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(initCxtSetupReqPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(initCxtSetupReqPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_InitCntxtSetup);
   vbUtlFillTknU32(&(initCxtSetupReqPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   vbUtlFillTknU8(&(initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
            sztInitCntxtSetupRqst.pres),PRSNT_NODEF);

   /* Allocate memory for 5 mandatory IEs */
/* vb003.101: Include Ue Cap IE if present */
#ifdef UE_RAD_CAP
   if(ueCb->ueCapbInfo.pres)
   {
      numComp = 7;
   }
   else
   {
      numComp = 6;
   }
#else
   numComp = 6;
/* vb003.101: Include Ue Cap IE if present */
#endif
   vbUtlFillTknU16(&(initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
            sztInitCntxtSetupRqst.protocolIEs.noComp), numComp);

   if ((cmGetMem(initCxtSetupReqPdu,
               (numComp * sizeof(SztProtIE_Field_InitCntxtSetupRqstIEs)),
               (Ptr *)&initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
               sztInitCntxtSetupRqst.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB062, ERRZERO, "cmGetMem Failed");
      cmFreeMem((Ptr)initCxtSetupReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztInitCntxtSetupRqst.protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

   /* IE2 - Filling ENB ID */
   ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztInitCntxtSetupRqst.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE3 - Filling Aggregate Max bit rate */
   ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztInitCntxtSetupRqst.protocolIEs.member[2];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_uEaggregateMaxBitrate);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztUEAggMaxBitrate.pres), PRSNT_NODEF);
   dlBitrate[0] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl >> 24 ) & 0xff;
   dlBitrate[1] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl >> 16 ) & 0xff;
   dlBitrate[2] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl >> 8 ) & 0xff;
   dlBitrate[3] = ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl & 0xff;
   ulBitrate[0] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul >> 24 ) & 0xff;
   ulBitrate[1] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul >> 16 ) & 0xff;
   ulBitrate[2] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul >> 8 ) & 0xff;
   ulBitrate[3] = ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul & 0xff;
   len = 4;
   vbUtlFillTknStrOSXL(&(ie->value.u.sztUEAggMaxBitrate.\
         uEaggregateMaxBitRateDL), len, dlBitrate, &initCxtSetupReqPdu);

   vbUtlFillTknStrOSXL(&(ie->value.u.sztUEAggMaxBitrate.\
         uEaggregateMaxBitRateUL), len, ulBitrate, &initCxtSetupReqPdu);

   /* IE4 - Filling E-RAB br setup list */
   /* vb002.101: Find out the number of esmCbs present */
   numComp = ueCb->creBid.nmbBearerId;

   ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztInitCntxtSetupRqst.protocolIEs.member[3];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_E_RABToBeSetupLstCtxtSUReq);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU16(&(ie->value.u.sztE_RABToBeSetupLstCtxtSUReq.noComp),numComp);

   if ((cmGetMem(initCxtSetupReqPdu, numComp *
      sizeof(SztProtIE_SingleCont_E_RABToBeSetupItemCtxtSUReqIEs),
      (Ptr *)&ie->value.u.sztE_RABToBeSetupLstCtxtSUReq.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB065, ERRZERO, "cmGetMem Failed");
      cmFreeMem((Ptr)initCxtSetupReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE4 - Fill in setup List items */
   for(idx = 0; idx < numComp; idx++)
   {
      ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, ueCb->creBid.bearerId[idx], 0);

      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB065, ERRZERO,
                                      "ESM CB Not Found");
         cmFreeMem((Ptr)initCxtSetupReqPdu);
         RETVALUE(RFAILED);
      }

      ie1 = &ie->value.u.sztE_RABToBeSetupLstCtxtSUReq.member[idx];

      vbUtlFillTknU8(&(ie1->pres),PRSNT_NODEF);
      vbUtlFillTknU32(&(ie1->id), Sztid_E_RABToBeSetupItemCtxtSUReq);
      vbUtlFillTknU32(&(ie1->criticality), SztCriticalityrejectEnum);

      vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.pres),
            PRSNT_NODEF);
      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.e_RAB_ID),
            esmCb->bId);

      vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.qCI), ueCb->ueCtxt.ueHssCtxt.
               subCntxt[0].qos.qci);

      vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.allocationRetentionPriority.pres),
            PRSNT_NODEF);

      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.allocationRetentionPriority.priorityLvl),
               ueCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.priLevel);

      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
            e_RABlevelQoSParams.allocationRetentionPriority.pre_emptionCapblty),
            SztPre_emptionCapbltymay_trigger_pre_emptionEnum);

      vbUtlFillTknU32(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
            e_RABlevelQoSParams.allocationRetentionPriority.
            pre_emptionVulnerability),
            SztPre_emptionVulnerabilitypre_emptableEnum);

      ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.e_RABlevelQoSParams.
         allocationRetentionPriority.iE_Extns.noComp.pres = NOTPRSNT;

      if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
      {
         ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.e_RABlevelQoSParams.\
               gbrQosInform.pres.pres = NOTPRSNT;
      }
      else
      {
         vbMmeUtlGetBitRate(esmCb->epsQos.maxBitRateDL,\
               esmCb->epsQos.maxBitRateDLExt, dlBitrate);
         vbUtlFillTknU8(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.gbrQosInform.pres), PRSNT_NODEF);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.\
               e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateDL), 4,
               dlBitrate, &initCxtSetupReqPdu);

         vbMmeUtlGetBitRate(esmCb->epsQos.maxBitRateUL,\
               esmCb->epsQos.maxBitRateULExt, ulBitrate);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateUL), 4,
               ulBitrate, &initCxtSetupReqPdu);

         vbMmeUtlGetBitRate(esmCb->epsQos.guaraBitRateDL,\
               esmCb->epsQos.guaraBitRateDLExt, dlBitrate);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateDL), 4,
               dlBitrate, &initCxtSetupReqPdu);

         vbMmeUtlGetBitRate(esmCb->epsQos.guaraBitRateUL,\
               esmCb->epsQos.guaraBitRateULExt, ulBitrate);
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateUL), 4,
               ulBitrate, &initCxtSetupReqPdu);
      }

      ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.e_RABlevelQoSParams.
         iE_Extns.noComp.pres = NOTPRSNT;
      if (esmCb->s1SgwAddr.type == CM_INET_IPV4ADDR_TYPE)
      {
         tptAddr[0] = (U8)((esmCb->s1SgwAddr.u.ipv4NetAddr >> 24) & 0xff);
         tptAddr[1] = (U8)((esmCb->s1SgwAddr.u.ipv4NetAddr >> 16) & 0xff);
         tptAddr[2] = (U8)((esmCb->s1SgwAddr.u.ipv4NetAddr >> 8) & 0xff);
         tptAddr[3] = (U8)(esmCb->s1SgwAddr.u.ipv4NetAddr & 0xff);
         vbUtlFillTknStrBSXL(&(ie1->value.u.\
               sztE_RABToBeSetupItemCtxtSUReq.transportLyrAddr), 4,\
               tptAddr, &initCxtSetupReqPdu);
      }
#ifdef IPV6_SUPPORTED
      else if (esmCb->s1SgwAddr.type == CM_INET_IPV6ADDR_TYPE)
      {
         for (len = 0; len < 16; len++)
         {
            tptAddr[len] = esmCb->s1SgwAddr.u.ipv6NetAddr[len];
         }

         vbUtlFillTknStrBSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.\
               transportLyrAddr), len, tptAddr, &initCxtSetupReqPdu);
      }
#endif /* IPV6_SUPPORTED */

      gTPId[0] = (U8)((esmCb->s1SgwTeId >> 24) & 0xff);
      gTPId[1] = (U8)(esmCb->s1SgwTeId >> 16) & 0xff;
      gTPId[2] = (U8)(esmCb->s1SgwTeId >> 8) & 0xff;
      gTPId[3] = (U8)(esmCb->s1SgwTeId & 0xff);
      vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.\
            gTP_TEID), 4, gTPId, &initCxtSetupReqPdu);

      /* fill the attach acc nas pdu here */
      if(sztNAS_PDU && !nasPduFilled)
      {
         ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.nAS_PDU.pres = PRSNT_NODEF;
         vbUtlFillTknStrOSXL(&(ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.\
               nAS_PDU), sztNAS_PDU->len,sztNAS_PDU->val, &initCxtSetupReqPdu);
         nasPduFilled = TRUE;
      }
      /*done with adding nas PDU */

      ie1->value.u.sztE_RABToBeSetupItemCtxtSUReq.iE_Extns.noComp.
         pres = NOTPRSNT;
   }

   /* IE5 - Filling UE Security Capabilities */
   ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztInitCntxtSetupRqst.protocolIEs.member[4];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_UESecurCapabilities);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztUESecurCapabilities.pres), PRSNT_NODEF);
   encAlgo = (U8)(ueCb->ueCtxt.ueSecCap.eea0 << 7) |
      (U8)(ueCb->ueCtxt.ueSecCap.eea1_128 << 6) |
      (U8)(ueCb->ueCtxt.ueSecCap.eea2_128  << 5);
   len = 2;
   val[0]=encAlgo;
   val[1] = 0;
   vbUtlFillTknBStr32(&(ie->value.u.sztUESecurCapabilities.\
         encryptionAlgorithms), len, val);
   intProtAlgo = (U8)(ueCb->ueCtxt.ueSecCap.eia1_128 << 7 ) |
      (U8)(ueCb->ueCtxt.ueSecCap.eia2_128 << 6 );
   len = 2;
   val[0] =intProtAlgo;
   val[1] = 0;
   vbUtlFillTknBStr32(&(ie->value.u.sztUESecurCapabilities.
            integrityProtectionAlgorithms), len, val);

   ie->value.u.sztUESecurCapabilities.iE_Extns.noComp.pres = NOTPRSNT;

   /* IE6 - Filling security */
   ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztInitCntxtSetupRqst.protocolIEs.member[5];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_SecurKey);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
#ifdef VB_MME_NAS_SEC
   vbUtlFillTknStrBSXL(&(ie->value.u.sztSecurKey), CM_EMM_MAX_KENB,\
         ueCb->ueCtxt.secCtxt.enbKey, &initCxtSetupReqPdu);
#else
   /* This is required only for testing in PAL setup so that
    * S1ap doesnt crib for invalid security key encoding
    */
   vbUtlFillTknStrBSXL(&(ie->value.u.sztSecurKey), CM_EMM_MAX_KENB,\
         nullKenb, &initCxtSetupReqPdu);
#endif

   /* vb003.101: Include Ue Cap IE if present */
#ifdef UE_RAD_CAP
   if(ueCb->ueCapbInfo.pres == TRUE)
   {
      ie = &initCxtSetupReqPdu->pdu.val.initiatingMsg.value.u.
         sztInitCntxtSetupRqst.protocolIEs.member[6];

      vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(ie->id), Sztid_UERadioCapblty);
      vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      len = ueCb->ueCapbInfo.len;
      vbUtlFillTknStrBSXL(&(ie->value.u.sztUERadioCapblty), len,
            ueCb->ueCapbInfo.val,
            &initCxtSetupReqPdu);
   }
#endif

   *initCxtSetupReqPduPtr = initCxtSetupReqPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldInitCxtSetupReq */

/*
 *
 *       Fun:   vbMmeUtlBldUeCxtModReq
 *
 *       Desc:  This function builds an Ue Context Modify Request PDU
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldUeCxtModReq
(
VbMmeUeCb  *ueCb,    /* UE Cb */
SztNAS_PDU *sztNAS_PDU,     /* new param for nasPdu */
S1apPdu   **ueCxtModReqPduPtr /* S1 UE Context Modify Request Pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldUeCxtModReq(ueCb,sztNAS_PDU,ueCxtModReqPduPtr)
VbMmeUeCb  *ueCb;    /* UE Cb */
SztNAS_PDU *sztNAS_PDU;     /* new param for nasPdu */
S1apPdu   **ueCxtModReqPduPtr; /* S1 UE Context Modify Request Pdu to be built */
#endif
{
   S16  ret;
   Mem  mem;
   U16  numComp;
   U8   dlBitrate[12];
   U8   ulBitrate[12];
   U16  len;

   SztProtIE_Field_UECntxtModificationRqstIEs *ie;
   S1apPdu *ueCxtModReqPdu;

   VB_TRC2(vbMmeUtlBldUeCxtModReq);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Ue Context Modify Request"));

   *ueCxtModReqPduPtr = NULLP;

   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&ueCxtModReqPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB061, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(ueCxtModReqPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(ueCxtModReqPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ueCxtModReqPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_UECntxtModification);
   vbUtlFillTknU32(&(ueCxtModReqPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   vbUtlFillTknU8(&(ueCxtModReqPdu->pdu.val.initiatingMsg.value.u.
            sztUECntxtModificationRqst.pres),PRSNT_NODEF);

   /* Allocate memory for 2 mandatory and 1 optional IEs */
   numComp = 3;
   vbUtlFillTknU16(&(ueCxtModReqPdu->pdu.val.initiatingMsg.value.u.
            sztUECntxtModificationRqst.protocolIEs.noComp), numComp);

   if ((cmGetMem(ueCxtModReqPdu,
               (numComp * sizeof(SztProtIE_Field_UECntxtModificationRqstIEs)),
               (Ptr *)&ueCxtModReqPdu->pdu.val.initiatingMsg.value.u.
               sztUECntxtModificationRqst.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB062, ERRZERO, "cmGetMem Failed");
      cmFreeMem((Ptr)ueCxtModReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &ueCxtModReqPdu->pdu.val.initiatingMsg.value.u.
      sztUECntxtModificationRqst.protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

   /* IE2 - Filling ENB ID */
   ie = &ueCxtModReqPdu->pdu.val.initiatingMsg.value.u.
      sztUECntxtModificationRqst.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE3 - Filling Aggregate Max bit rate */
   ie = &ueCxtModReqPdu->pdu.val.initiatingMsg.value.u.
      sztUECntxtModificationRqst.protocolIEs.member[2];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_uEaggregateMaxBitrate);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztUEAggMaxBitrate.pres), PRSNT_NODEF);
   dlBitrate[0] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl >> 24 ) & 0xff;
   dlBitrate[1] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl >> 16 ) & 0xff;
   dlBitrate[2] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl >> 8 ) & 0xff;
   dlBitrate[3] = ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl & 0xff;
   ulBitrate[0] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul >> 24 ) & 0xff;
   ulBitrate[1] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul >> 16 ) & 0xff;
   ulBitrate[2] = (ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul >> 8 ) & 0xff;
   ulBitrate[3] = ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul & 0xff;
   len = 4;
   ret = vbUtlFillTknStrOSXL(&(ie->value.u.sztUEAggMaxBitrate.
                               uEaggregateMaxBitRateDL),
                             len, dlBitrate, &ueCxtModReqPdu);
   if(ROK != ret)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB063, ERRZERO,
            "Filling UE Aggregate Max Bit Rate DL Failed");
      cmFreeMem((Ptr)ueCxtModReqPdu);
      RETVALUE(RFAILED);
   }

   ret = vbUtlFillTknStrOSXL(&(ie->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateUL),
                             len, ulBitrate, &ueCxtModReqPdu);
   if(ROK != ret)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB064, ERRZERO,
            "Filling UE Aggregate Max Bit Rate UL Failed");
      cmFreeMem((Ptr)ueCxtModReqPdu);
      RETVALUE(RFAILED);
   }

   *ueCxtModReqPduPtr = ueCxtModReqPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldUeCxtModReq */

#ifdef ANSI
PRIVATE S16 vbMmeUtlGetBitRate
(
U8 bitFld,
U8 bitFldExt,
U8 *bitRate
)
#else
PRIVATE S16 vbMmeUtlGetBitRate(bitFld, bitFldExt, bitRate)
(
U8 bitFld;
U8 bitFldExt;
U8 *bitRate;
)
#endif
{
   U16 kbps = 1000;
   U32 qos  = 0;

   if(NULLP == bitRate)
   {
      RETVALUE(RFAILED);
   }

   if(bitFld == 255)
   {
      qos = 0;
   }
   else if(bitFld < 64)
   {
      qos = bitFld* kbps;
   }
   else if(bitFld <128)
   {
      qos= (bitFld-64) * 8 * kbps + 64*kbps;
   }
   else if(bitFld <254)
   {
      qos= (bitFld-128) * 64 * kbps + 574*kbps;
   }
   else if(bitFld == 254)
   {
      if(bitFldExt == 0)
      {
         qos = 8640 *kbps;
      }
      else if(bitFldExt < 74)
      {
         qos = 8640 * kbps + 74 * 100 * kbps;
      }
      else if(bitFldExt < 186)
      {
         qos = 8640 *kbps + (bitFldExt - 74) * kbps * kbps + 16000 * kbps ;
      }
      else if(bitFldExt < 250)
      {
         qos = 8640 *kbps + (bitFldExt - 186) * 2*kbps*kbps + 128 * kbps * kbps ;
      }
      else
      {
         qos = 8640 *kbps + (250 - 186) * 2*kbps*kbps + 128 * kbps * kbps ;
      }
   }
   bitRate [0] = (qos>>24) & 0xFF;
   bitRate [1] = (qos>>16) & 0xFF;
   bitRate [2] = (qos>>8) & 0xFF;
   bitRate [3] = (qos) & 0xFF;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:   vbMmeUtlBldDlNasTransport
 *
 *       Desc:  This function builds DOWNLINK NAS TRANSPORT
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldDlNasTransport
(
VbMmeUeCb  *ueCb,    /* UE Cb */
SztNAS_PDU *sztNAS_PDU, /* nas pdu */
S1apPdu    **dlNasTpt   /* S1 downlink NAS transport pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldDlNasTransport (ueCb, sztNAS_PDU, dlNasTpt)
VbMmeUeCb  *ueCb;    /* UE Cb */
SztNAS_PDU *sztNAS_PDU; /* nas pdu */
S1apPdu    **dlNasTpt;   /* S1 downlink NAS transport pdu to be built */
#endif
{
   S16      ret;
   Mem      mem;
   U16      numComp;
   S1apPdu  *nasTpt;
   SztProtIE_Field_DlnkNASTport_IEs *ie;

   VB_TRC2(vbMmeUtlBldDlNasTransport);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building NAS Transport Req"));

   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&nasTpt);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB070, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(nasTpt->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(nasTpt->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(nasTpt->pdu.val.initiatingMsg.procedureCode),
         Sztid_downlinkNASTport);
   vbUtlFillTknU32(&(nasTpt->pdu.val.initiatingMsg.criticality),
         SztCriticalityignoreEnum);

   vbUtlFillTknU8(&(nasTpt->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.pres),
         PRSNT_NODEF);

   /* Allocate memory for 3 mandatory IEs */
   numComp = 3;
   vbUtlFillTknU16(&(nasTpt->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.
            protocolIEs.noComp), numComp);

   if ((cmGetMem(nasTpt, (numComp * sizeof(SztProtIE_Field_DlnkNASTport_IEs)),
               (Ptr *)&nasTpt->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.
               protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB071, ERRZERO,
            "cmGetMem  Failed");
      cmFreeMem((Ptr)nasTpt);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling MME ID */
   ie = &nasTpt->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.protocolIEs.
      member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

   /* IE2 - Filling ENB ID */
   ie = &nasTpt->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.
      protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE3 - Filling NAS PDU */
   ie = &nasTpt->pdu.val.initiatingMsg.value.u.sztDlnkNASTport.protocolIEs.
      member[2];

   vbUtlFillTknU8(&(ie->pres),PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id),Sztid_NAS_PDU);
   vbUtlFillTknU32(&(ie->criticality),SztCriticalityrejectEnum);
   ret = vbUtlFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), sztNAS_PDU->len,
         sztNAS_PDU->val, &nasTpt);
   if(ROK != ret)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB072, ERRZERO,
            "Filling NAS Pdu Failed");
      cmFreeMem((Ptr)nasTpt);
   }

   *dlNasTpt = nasTpt;
   RETVALUE(ROK);
} /* vbMmeUtlBldDlNasTransport */

/*
 *
 *       Fun:   vbMmeUtlBldUeCxtRlsCmd
 *
 *       Desc:  This function builds a UE context release command
 *              S1AP PDU
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldUeCxtRlsCmd
(
VbMmeUeCb  *ueCb,    /* UE Cb */
S1apPdu   **ueCxtRlsCmdPduPtr, /* UE context release command Pdu to be built */
U8          choice, /* UE context release choice value */
U8          cause   /* cause value */
)
#else
PUBLIC S16 vbMmeUtlBldUeCxtRlsCmd (ueCb, ueCxtRlsCmdPduPtr,choice,cause)
VbMmeUeCb  *ueCb;    /* UE Cb */
S1apPdu   **ueCxtRlsCmdPduPtr; /* UE context release command Pdu to be built */
U8          choice; /* UE context release choice value */
U8          cause;  /* cause value */
#endif
{
   S16 ret = ROK;
   Mem mem;
   S1apPdu *ueCxtRlsCmdPdu;
   U16 numComp = 0;
   SztProtIE_Field_UECntxtRlsCmmd_IEs *ie = NULLP;

   VB_TRC2(vbMmeUtlBldUeCxtRlsCmd);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building UE Ctxt Rls Command"));

   *ueCxtRlsCmdPduPtr = NULLP;
   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&ueCxtRlsCmdPdu);
   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB073, ERRZERO,
            "cmAllocEvnt  Failed");

      RETVALUE(RFAILED);
   }

   vbUtlFillTknU8(&(ueCxtRlsCmdPdu->pdu.choice), VB_S1AP_INIT_MSG);
   vbUtlFillTknU8(&(ueCxtRlsCmdPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ueCxtRlsCmdPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_UECntxtRls);
   vbUtlFillTknU32(&(ueCxtRlsCmdPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   vbUtlFillTknU8(&(ueCxtRlsCmdPdu->pdu.val.initiatingMsg.value.u.
            sztUECntxtRlsCmmd.pres), PRSNT_NODEF);

   numComp = 2;
   vbUtlFillTknU16(&(ueCxtRlsCmdPdu->pdu.val.initiatingMsg.value.u.
            sztUECntxtRlsCmmd.protocolIEs.noComp), numComp);

   if ((cmGetMem(ueCxtRlsCmdPdu,
               (numComp * sizeof(SztProtIE_Field_UECntxtRlsCmmd_IEs)),
               (Ptr *)&ueCxtRlsCmdPdu->pdu.val.initiatingMsg.
               value.u.sztUECntxtRlsCmmd.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB074, ERRZERO,
            "cmGetMem  Failed");
      cmFreeMem((Ptr)ueCxtRlsCmdPdu);
      RETVALUE(RFAILED);
   }

   /* IE1 - Filling UE IDs - MME and ENB ID */
   ie = &ueCxtRlsCmdPdu->pdu.val.initiatingMsg.value.u.
      sztUECntxtRlsCmmd.protocolIEs.member[0];
   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_UE_S1AP_IDs);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU8(&(ie->value.u.sztUE_S1AP_IDs.choice),
         UE_S1AP_IDS_UE_S1AP_ID_PAIR);
   vbUtlFillTknU8(&(ie->value.u.sztUE_S1AP_IDs.val.uE_S1AP_ID_pair.pres),
         PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->value.u.sztUE_S1AP_IDs.val.uE_S1AP_ID_pair.
            mME_UE_S1AP_ID), ueCb->mMeUeS1apId);
   vbUtlFillTknU32(&(ie->value.u.sztUE_S1AP_IDs.val.uE_S1AP_ID_pair.
            eNB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE2 - Filling cause */
   ie = &ueCxtRlsCmdPdu->pdu.val.initiatingMsg.value.u.
      sztUECntxtRlsCmmd.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_Cause);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU8(&(ie->value.u.sztCause.choice),choice);
   /* vb001.101: Filling cause */
   switch(choice)
   {
      case CAUSE_NAS:
      {
         vbUtlFillTknU32(&(ie->value.u.sztCause.val.nas),cause);
         break;
      }
      case CAUSE_RADIONW:
      {
         vbUtlFillTknU32(&(ie->value.u.sztCause.val.radioNw),cause);
         break;
      }
      case CAUSE_TRANSPORT:
      {
         vbUtlFillTknU32(&(ie->value.u.sztCause.val.transport),cause);
         break;
      }
      case CAUSE_PROTOCOL:
      {
         vbUtlFillTknU32(&(ie->value.u.sztCause.val.protocol),cause);
         break;
      }
      case CAUSE_MISC:
      {
         vbUtlFillTknU32(&(ie->value.u.sztCause.val.misc),cause);
         break;
      }
      default:
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB074, ERRZERO,
            "Unhandled Choice");
      }
   } /* end of switch statement */

   /*store the release cause for future use */
   cmMemcpy((U8*)&ueCb->relCause, (U8*)&ie->value.u.sztCause,
                                   sizeof(ueCb->relCause));

   *ueCxtRlsCmdPduPtr = ueCxtRlsCmdPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldUeCxtRlsCmd */


/*
 *
 *       Fun:   vbMmeAddToUETaiLst
 *
 *       Desc:  This function adds tai to UE's TAI list
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeAddToUETaiLst
(
VbMmeUeCb *ueCb,
VbUeTai   *tai
)
#else
PUBLIC S16 vbMmeAddToUETaiLst(ueCb, tai)
VbMmeUeCb *ueCb;
VbUeTai   *tai;
#endif
{
   U8 mcc[LVB_MAX_MCC_LEN];
   U8 mnc[LVB_MAX_MNC_LEN];

   CmEmmTaiLst *evntLst = &ueCb->ueCtxt.taList;

   /* Validate maximum TA in list reached */
   if(evntLst->len >= CM_EMM_MAX_TAI_LIST)
   {
      RETVALUE(ROK);
   }

   /* Type 1 TA list */
   evntLst->pres = PRSNT_NODEF;
   evntLst->partLst[0].type = 0x01;

   mcc[0] = tai->plmnId.plmnId[0] & 0x0f;
   mcc[1] = (tai->plmnId.plmnId[0] & 0xf0) >> 4;
   mcc[2] = tai->plmnId.plmnId[1] & 0x0f;
   mnc[0] = tai->plmnId.plmnId[2] & 0x0f;
   mnc[1] = (tai->plmnId.plmnId[2] & 0xf0) >> 4;
   mnc[2] = (tai->plmnId.plmnId[1] & 0xf0) >> 4;

   /* Preparing TA List with only one TAI */
   cmMemcpy((U8*)evntLst->partLst[0].u.type1.tai.mcc,
             mcc, LVB_MAX_MCC_LEN);
   cmMemcpy((U8*)evntLst->partLst[0].u.type1.tai.mnc,
            mnc, LVB_MAX_MNC_LEN);
   evntLst->partLst[0].u.type1.tai.tac = tai->tac;
   evntLst->partLst[0].numElm = 1;
   evntLst->len = 1;

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   vbMmeUtlBldAttachAccept
 *
 *       Desc:  This function builds an EMM Attach Accept message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldAttachAccept
(
VbMmeUeCb   *ueCb,    /* UE Cb */
CmNasEvnt   **evnt /* Attach accept pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldAttachAccept (ueCb, evnt)
VbMmeUeCb   *ueCb;    /* UE Cb */
CmNasEvnt   **evnt; /* Attach accept pdu to be built */
#endif
{
   CmEmmAttachAccept  *atchAcc;
   CmEmmMsg           *emmMsg;
   S16                ret = ROK;
   VbMmeEsmCb         *esmCb = NULLP;
   U8                 stmsi[5];

   VB_TRC2(vbMmeUtlBldAttachAccept);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Attach Accept Request"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);


   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB075, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;
   atchAcc = &((*evnt)->m.emmEvnt->u.atchAcc);

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_ATTACH_ACC;
   emmMsg->protDisc = CM_EMM_PD;

   /* Fill mandatory IEs */
   /* For now attach result is "EPS only" */
   atchAcc->epsAtchRes.res = CM_EMM_ATCH_RES_EPS_ONLY;

   /* Fill T3412 timer value */
   atchAcc->t3412.pres = TRUE;

   /* + Deactivate timer if t3412 is set zero + */
   if(ueCb->eNodebCb->eNbCfg.t3412Tmr == 0)
   {
      atchAcc->t3412.val = 0;
      atchAcc->t3412.unit = 7;
   }
   else if(ueCb->eNodebCb->eNbCfg.t3412Tmr <= 64)
  /* - Deactivate timer if t3412 is set zero - */
   {
      atchAcc->t3412.val = ueCb->eNodebCb->eNbCfg.t3412Tmr/2;
      atchAcc->t3412.unit = 0;
   }
   else if(ueCb->eNodebCb->eNbCfg.t3412Tmr <= 32*60)
   {
      atchAcc->t3412.val = ueCb->eNodebCb->eNbCfg.t3412Tmr/60;
      atchAcc->t3412.unit = 1;
   }
   else
   {
      atchAcc->t3412.val = ueCb->eNodebCb->eNbCfg.t3412Tmr/(360);
      atchAcc->t3412.unit = 2;
   }

   /* Fill tracking area id list */
   cmMemcpy((U8*)&atchAcc->taiLst, (U8*)&ueCb->ueCtxt.taList,\
         sizeof(atchAcc->taiLst));

   /* Fill the ESM pdu */
   ret = vbMmeUtlFndEsmCb(ueCb, &esmCb,
                          ueCb->emmCb.piggyAction.bId,
                          ueCb->emmCb.piggyAction.tId);
   if(ROK != ret)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB076, ERRZERO,
            "ESM Cb Not Found");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   if (ROK != vbMmeUtlBldDfltActvBearerReq(esmCb, &atchAcc->esmEvnt))
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB076, ERRZERO,
            "Building Default Br Req Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   ret = vbMmeUtlDelUeCbOnSTmsi(ueCb);
   /* Allocate a new GUTI for the UE */
   vbMmeUtlAllocGuti(&ueCb->ueCtxt.ueGuti);

   /* Add to hashlist based on STMSI */
   stmsi[0] = ueCb->ueCtxt.ueGuti.mmeCode & 0xff;
   stmsi[1] = (ueCb->ueCtxt.ueGuti.mTMSI>>24) & 0xff;
   stmsi[2] = (ueCb->ueCtxt.ueGuti.mTMSI>>16) & 0xff;
   stmsi[3] = (ueCb->ueCtxt.ueGuti.mTMSI>>8) & 0xff;
   stmsi[4] = (ueCb->ueCtxt.ueGuti.mTMSI) & 0xff;

   ret = vbMmeUtlAddUeCbOnSTmsi(stmsi, ueCb);
   if(ret != ROK)
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB049, ERRZERO,
           "Failed To Insert Into Stmsi Hashlist");
       RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&atchAcc->guti.u.guti, (U8 *)&ueCb->ueCtxt.ueGuti,
                                        sizeof(atchAcc->guti.u.guti));
   atchAcc->guti.pres = TRUE;
   atchAcc->guti.type = CM_EMM_MID_TYPE_GUTI;
   /*It is 0 in case of GUTI as per the Spec */
   atchAcc->guti.evenOddInd = 0;

   if(VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_COMBINED_ATTACH))
   {
      atchAcc->cause.pres = TRUE;
      atchAcc->cause.cause = CM_EMM_CS_DOM_NA;
      /* if the UE has requested for COMBINED attach we are accepting it with cause set as CS
       domain not available */
      atchAcc->epsAtchRes.res = CM_EMM_ATCH_RES_COMB_EPS;
   }

   RETVALUE(ROK);
} /* vbMmeUtlBldAttachAccept */

/*
 *
 *       Fun:   vbMmeUtlBldAttachReject
 *
 *       Desc:  This function builds an EMM Attach Reject message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldAttachReject
(
VbMmeUeCb  *ueCb,   /* UE Cb */
CmNasEvnt  **evnt,    /* Attach Reject pdu to be built */
U8         emmCause,/* EMM Cause value attach reject */
VbMmeEsmCb *esmCb
)
#else
PUBLIC S16 vbMmeUtlBldAttachReject (ueCb, evnt, emmCause, esmCb)
VbMmeUeCb  *ueCb;   /* UE Cb */
CmNasEvnt   **evnt;  /* Attach Reject pdu to be built */
U8          emmCause; /* EMM Cause value attach reject */
VbMmeEsmCb  *esmCb;
#endif
{
   S16                ret = ROK;
   CmEmmAttachReject  *atchRej = NULLP;
   CmEmmMsg           *emmMsg = NULLP;

   VB_TRC2(vbMmeUtlBldAttachReject);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Attach Reject"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt  Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;
   atchRej = &(*evnt)->m.emmEvnt->u.atchRej;

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_ATTACH_REJ;

/* vb004.101 : 116186: added protocol descriptor type */
    emmMsg->protDisc = CM_EMM_PD;

   /* Fill mandatory IEs */
   /* Fill the attach reject cause */
   atchRej->emmCause.cause = emmCause;
   /* vb004.101: Missing pres value added. */
   atchRej->emmCause.pres = 1;

   /* Fill Optional Parameters */
   /* Fill the ESM pdu */
   if ((emmCause == CM_EMM_ESM_FAILURE) && esmCb)
   {
      ret = vbMmeUtlBldPdnConnReject(esmCb, &(atchRej->esmEvnt),
                                     ueCb->emmCb.piggyAction.cause);
      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB079, ERRZERO,
               "ESM Container PDN Connectivity Failed");
         CM_FREE_NASEVNT(evnt);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);

} /* vbMmeUtlBldAttachReject */

/*
 *
 *       Fun:   vbMmeUtlBldServiceReject
 *
 *       Desc:  This function builds an EMM Service Reject message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldServiceReject
(
VbMmeUeCb  *ueCb,   /* UE Cb */
CmNasEvnt  **evnt,    /* Service Reject pdu to be built */
U8         emmCause/* EMM Cause value service reject */
)
#else
PUBLIC S16 vbMmeUtlBldServiceReject (ueCb, evnt, emmCause)
VbMmeUeCb  *ueCb;   /* UE Cb */
CmNasEvnt   **evnt;  /* Service Reject pdu to be built */
U8          emmCause; /* EMM Cause value service reject */
#endif
{
   CmEmmServiceRej   *srvRej = NULLP;
   CmEmmMsg           *emmMsg = NULLP;

   VB_TRC2(vbMmeUtlBldServiceReject)

   UNUSED(*ueCb);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Service Reject Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   /* Reset allocated buffer */
   cmMemset((U8*)emmMsg, 0, sizeof(CmEmmMsg));

   (*evnt)->m.emmEvnt = emmMsg;
   srvRej = &(*evnt)->m.emmEvnt->u.srvRej;

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_SERVICE_REJ;
   emmMsg->protDisc = CM_EMM_PD;

   /* Fill mandatory IE */
   /* Fill the attach reject cause */
   srvRej->emmCause.cause = emmCause;
   srvRej->emmCause.pres = PRSNT_NODEF;

   RETVALUE(ROK);

} /* vbMmeUtlBldServiceReject */



/*
 *
 *       Fun:   vbMmeUtlBldGutiReallocCmd
 *
 *       Desc:  This function builds an EMM GUTI Reallocation
 *              Command message structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldGutiReallocCmd
(
VbMmeUeCb   *ueCb,    /* UE Cb */
CmNasEvnt   **evnt    /* GUTI Reallocation Cmd pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldGutiReallocCmd (ueCb, evnt)
VbMmeUeCb   *ueCb;    /* UE Cb */
CmNasEvnt   **evnt;   /* GUTI Reallocation Cmd pdu to be built */
#endif
{
   CmEmmGUTIReallocCmd  *gutiCmd;
   CmEmmMsg             *emmMsg;
   S16                  ret = ROK;
   U8                   stmsi[5];

   VB_TRC2(vbMmeUtlBldGutiReallocCmd);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building GUTI Reallocation Cmd Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB075, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;
   gutiCmd = &((*evnt)->m.emmEvnt->u.gutiReallocCmd);

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_GUTI_RLC_CMD;
   emmMsg->protDisc = CM_EMM_PD;

   /*Release old GUTI */
   ret = vbMmeUtlDelUeCbOnSTmsi(ueCb);
   /* Allocate a new GUTI for the UE */
   vbMmeUtlAllocGuti(&ueCb->ueCtxt.ueGuti);

   /* Add to hashlist based on STMSI */
   stmsi[0] = ueCb->ueCtxt.ueGuti.mmeCode & 0xff;
   stmsi[1] = (ueCb->ueCtxt.ueGuti.mTMSI>>24) & 0xff;
   stmsi[2] = (ueCb->ueCtxt.ueGuti.mTMSI>>16) & 0xff;
   stmsi[3] = (ueCb->ueCtxt.ueGuti.mTMSI>>8) & 0xff;
   stmsi[4] = (ueCb->ueCtxt.ueGuti.mTMSI) & 0xff;

   ret = vbMmeUtlAddUeCbOnSTmsi(stmsi, ueCb);
   if(ret != ROK)
   {
       VB_MME_LOGERROR(ERRCLS_DEBUG, EVB049, ERRZERO,
           "Failed To Insert Into Stmsi Hashlist");
       RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&gutiCmd->guti.u.guti, (U8 *)&ueCb->ueCtxt.ueGuti,
                                        sizeof(gutiCmd->guti.u.guti));
   gutiCmd->guti.pres = TRUE;
   gutiCmd->guti.type = CM_EMM_MID_TYPE_GUTI;
   /*It is 0 in case of GUTI as per the Spec */
   gutiCmd->guti.evenOddInd = 0;

   RETVALUE(ROK);
}

/*
 *
 *       Fun:   vbMmeUtlBldResAllocReject
 *
 *       Desc:  This function builds an ESM Bearer Resource Allocation
 *              Reject message structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldResAllocReject
(
VbMmeEsmCb  *esmCb,   /* ESM Cb */
CmNasEvnt  **evnt,    /* Bearer ResAllocReject pdu to be built */
U8         esmCause /* ESM Cause value resource allocation reject */
)
#else
PUBLIC S16 vbMmeUtlBldResAllocReject (esmCb, evnt, esmCause)
VbMmeEsmCb  *esmCb;   /* ESM Cb */
CmNasEvnt   **evnt;  /* Bearer ResAllocReject pdu to be built */
U8          esmCause; /* ESM Cause value resource allocation reject */
#endif
{
   CmEsmdBearResAllocRej   *resAllocRej = NULLP;
   CmEsmMsg           *esmMsg = NULLP;

   VB_TRC2(vbMmeUtlBldResAllocReject);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Bearer Resource Allocation Reject Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&esmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   /* Reset allocated buffer */
   cmMemset((U8*)esmMsg, 0, sizeof(CmEsmMsg));

   (*evnt)->m.esmEvnt = esmMsg;
   resAllocRej = &(*evnt)->m.esmEvnt->u.bearAllocRej;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* Fill Header information */
   esmMsg->protDisc = CM_ESM_PD;
   esmMsg->bearerId = 0;
   esmMsg->prTxnId = esmCb->tId;
   esmMsg->msgType = CM_ESM_MSG_BEAR_RES_ALLOC_REJ;

   /* Fill mandatory IE */
   /* Fill the resource allocation reject cause */
   resAllocRej->cause.val = esmCause;
   resAllocRej->cause.pres = PRSNT_NODEF;

   RETVALUE(ROK);

} /* vbMmeUtlBldServiceReject */

/*
 *
 *       Fun:   vbMmeUtlBldResModReject
 *
 *       Desc:  This function builds an ESM Bearer Resource Modification
 *              Reject message structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldResModReject
(
VbMmeEsmCb  *esmCb,   /* ESM Cb */
CmNasEvnt  **evnt,    /* Bearer ResAllocReject pdu to be built */
U8         esmCause /* ESM Cause value resource modification reject */
)
#else
PUBLIC S16 vbMmeUtlBldResModReject (esmCb, evnt, esmCause)
VbMmeEsmCb  *esmCb;   /* ESM Cb */
CmNasEvnt   **evnt;  /* Bearer ResAllocReject pdu to be built */
U8          esmCause; /* ESM Cause value resource modification reject */
#endif
{
   CmEsmdBearResModRej   *resModRej = NULLP;
   CmEsmMsg           *esmMsg = NULLP;

   VB_TRC2(vbMmeUtlBldResModReject);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Bearer Resource Modification Reject Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&esmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   /* Reset allocated buffer */
   cmMemset((U8*)esmMsg, 0, sizeof(CmEsmMsg));

   (*evnt)->m.esmEvnt = esmMsg;
   resModRej = &(*evnt)->m.esmEvnt->u.bearModRej;

#if VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* Fill Header information */
   esmMsg->protDisc = CM_ESM_PD;
   esmMsg->bearerId = esmCb->bId;
   esmMsg->prTxnId = esmCb->tId;
   esmMsg->msgType = CM_ESM_MSG_BEAR_RES_MOD_REJ;

   /* Fill mandatory IE */
   /* Fill the resource allocation reject cause */
   resModRej->cause.pres = PRSNT_NODEF;
   resModRej->cause.val = esmCause;

   RETVALUE(ROK);
}


#ifdef VB_MME_AUTH
/*
 *
 *       Fun:   vbMmeUtlBldAuthRequest
 *
 *       Desc:  This function builds an EMM Authentication request message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldAuthRequest
(
VbMmeEmmCb  *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt    **evnt /* Attach accept pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldAuthRequest (ueEmmCb, evnt)
VbMmeEmmCb  *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt    **evnt; /* Attach accept pdu to be built */
#endif
{
   CmEmmAuthReq   *authReq;
   CmEmmMsg       *emmMsg;
   VbMmeAuthVec   *authVec;

   VB_TRC2(vbMmeUtlBldAuthRequest);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Auth Request"));


    /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB080, ERRZERO,
            "cmAllocEvnt Failed");

      RETVALUE(RFAILED);
   }
   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB081, ERRZERO,
            "cmGetMem Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;
   authReq  = &(*evnt)->m.emmEvnt->u.authReq;
   authVec = &ueEmmCb->ueCb->ueCtxt.authVec;

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->protDisc = CM_EMM_PD;
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_AUTH_REQ;

#if 0
   /* Fill NAS key set identifierASME */
   cmMemcpy((U8 *)&authReq->nasKsi, (U8 *)&ueSecInfo->nasKsi,\
         sizeof(CmEmmNasKsi));

   /* Fill Authentication parameter RAND */
   authReq->RAND.pres = TRUE;
   cmMemcpy((U8 *)&authReq->RAND.val, (U8 *)&(ueSecInfo->rand),\
         CM_EMM_MAX_RAND);

   /* Fill Authentication parameter AUTN */
   authReq->AUTN.pres = TRUE;
   authReq->AUTN.len  = CM_EMM_MAX_AUTN;
   cmMemcpy((U8 *)&authReq->AUTN.val, (U8 *)&(ueSecInfo->autn),\
         authReq->AUTN.len);
#endif
   /* Fill NAS key set identifierASME */
   authReq->nasKsi.tsc = ueEmmCb->ueCb->ueCtxt.secCtxt.tsc;
   authReq->nasKsi.id = ueEmmCb->ueCb->ueCtxt.secCtxt.ksi;
   authReq->nasKsi.pres = TRUE;

   /* Fill Authentication parameter RAND */
   authReq->RAND.pres = TRUE;
   cmMemcpy((U8 *)&authReq->RAND.val, (U8 *)&(authVec->rndNmb),\
         CM_EMM_MAX_RAND);

   /* Fill Authentication parameter AUTN */
   authReq->AUTN.pres = TRUE;
   authReq->AUTN.len  = CM_EMM_MAX_AUTN;
   cmMemcpy((U8 *)&authReq->AUTN.val, (U8 *)&(authVec->autn),\
         authReq->AUTN.len);

   RETVALUE(ROK);
} /* vbMmeUtlBldAuthRequest */

/* VB_MME_AUTH Authentication feature added
 * Function to build authentication reject added */
/*
 *
 *       Fun:   vbMmeUtlBldAuthReject
 *
 *       Desc:  This function builds an EMM Authentication reject message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldAuthReject
(
VbMmeEmmCb  *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt   **evnt /* Attach accept pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldAuthReject (ueEmmCb, evnt)
VbMmeEmmCb  *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt   **evnt; /* Attach accept pdu to be built */
#endif
{
   CmEmmMsg      *emmMsg;

   VB_TRC2(vbMmeUtlBldAuthReject);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Auth Reject"));


    /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVBXXX, ERRZERO,
            "cmAllocEvnt Failed");

      RETVALUE(RFAILED);
   }
   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVBXXX, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_AUTH_REJ;
   emmMsg->protDisc = CM_EMM_PD;

   RETVALUE(ROK);
} /* vbMmeUtlBldAuthReject */
#endif


#ifdef VB_MME_NAS_SEC
/*
 *
 *       Fun:   vbMmeUtlBldSecModCmd
 *
 *       Desc:  This function builds an EMM Security Mode Command message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldSecModCmd
(
VbMmeEmmCb  *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt    **evnt /* Attach accept pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldSecModCmd (ueEmmCb, evnt)
VbMmeEmmCb  *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt    **evnt; /* Attach accept pdu to be built */
#endif
{
   CmEmmSecModeCmd *secModCmd = NULLP;
   CmEmmMsg        *emmMsg = NULLP;
   CmEmmUeSecCap   ueSecCap;

   VB_TRC2(vbMmeUtlBldSecModCmd);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Security Mode Cmd Request"));
   if(ueEmmCb->ueCb)
   {
      ueSecCap = ueEmmCb->ueCb->ueCtxt.ueSecCap;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "UeSecCap Not Found"));
      RETVALUE(RFAILED);
   }

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);
   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB082, ERRZERO,
           "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }
   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB083, ERRZERO,
           "cmGetMem Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;
   secModCmd = &(*evnt)->m.emmEvnt->u.secModCmd;
#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_NEW_SEC_CTXT;
#endif

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_SEC_MODE_CMD;
   emmMsg->protDisc = CM_EMM_PD;

   /* Fill mandatory IEs */
   /* Fill Selected NAS security algorithms */
   secModCmd->selNasSecAlgo.pres = TRUE;
   secModCmd->selNasSecAlgo.intProtAlgo = ueEmmCb->ueCb->ueCtxt.secCtxt.intAlg;
   secModCmd->selNasSecAlgo.ciphAlgo = ueEmmCb->ueCb->ueCtxt.secCtxt.encAlg;
   secModCmd->nasKsi.tsc = ueEmmCb->ueCb->ueCtxt.secCtxt.tsc;
   secModCmd->nasKsi.id = ueEmmCb->ueCb->ueCtxt.secCtxt.ksi;
   secModCmd->nasKsi.pres = TRUE;

   /* Fill Replayed UE security capabilities */
   cmMemcpy((U8 *)&secModCmd->rpldUeSecCap, (U8 *)&ueSecCap,\
         sizeof(CmEmmUeSecCap));

   RETVALUE(ROK);
} /* vbMmeUtlBldSecModCmd */
#endif /* VB_MME_NAS_SEC */



/*
 *
 *       Fun:   vbMmeUtlBldIdentityReq
 *
 *       Desc:  This function builds an EMM Identity Request message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldIdentityReq
(
VbMmeEmmCb   *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt    **evnt,      /* Identity Request pdu to be built */
U8           type         /* Type of identity */
)
#else
PUBLIC S16 vbMmeUtlBldIdentityReq(ueEmmCb, evnt, type)
VbMmeEmmCb   *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt    **evnt;      /* Identity Request pdu to be built */
U8           type;        /* Type of identity */
#endif
{

    CmEmmMsg        *emmMsg;

    VB_TRC2(vbMmeUtlBldIdentityReq);

    VB_MME_DBG_INFO((VB_MME_PRNTBUF,
             "Building Identity Request"));

    /* Allocate memory for pdu */
    CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

    if(*evnt == NULLP)
    {
        VB_MME_LOGERROR(ERRCLS_DEBUG, EVB082, ERRZERO,
             "cmAllocEvnt Failed");
        RETVALUE(RFAILED);
    }

    if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
    {
        VB_MME_LOGERROR(ERRCLS_DEBUG, EVB083, ERRZERO,
                      "cmGetMem Failed");
        CM_FREE_NASEVNT(evnt);
        RETVALUE(RFAILED);
    }

    (*evnt)->m.emmEvnt = emmMsg;

    /* Fill Header information */
    /* Security header type is "Plain NAS message, not security protected" */
    emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
    emmMsg->msgId = CM_EMM_MSG_ID_REQ;
    emmMsg->protDisc = CM_EMM_PD;

    (*evnt)->m.emmEvnt->u.idReq.idType.pres = TRUE;
    (*evnt)->m.emmEvnt->u.idReq.idType.type = type;

    RETVALUE(ROK);
}


/*
 *
 *       Fun:   vbMmeUtlBldDetachAcc
 *
 *       Desc:  This function builds an EMM Detach Accept message
 *              structure.
 *
 *       Ret:   ROK     on   success
 *              RFAILED on   failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldDetachAcc
(
VbMmeEmmCb   *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt    **evnt       /* Identity Request pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldDetachAcc(ueEmmCb, evnt)
VbMmeEmmCb   *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt    **evnt;      /* Identity Request pdu to be built */
#endif
{
    CmEmmMsg        *emmMsg;

    VB_TRC2(vbMmeUtlBldDetachAcc);

    VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Detach Accept"));

    CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);
    if(*evnt == NULLP)
    {
        VB_MME_LOGERROR(ERRCLS_DEBUG, EVB082, ERRZERO, "cmAllocEvnt Failed");
        RETVALUE(RFAILED);
    }

    if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
    {
        VB_MME_LOGERROR(ERRCLS_DEBUG, EVB083, ERRZERO, "cmGetMem Failed");
        CM_FREE_NASEVNT(evnt);
        RETVALUE(RFAILED);
    }

#ifdef VB_MME_NAS_SEC
    (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

    (*evnt)->m.emmEvnt = emmMsg;

    /* Fill Header information */
    /* Security header type is "Plain NAS message, not security protected" */
    emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
    emmMsg->msgId = CM_EMM_MSG_DETACH_ACC;
    emmMsg->protDisc = CM_EMM_PD;

    RETVALUE(ROK);
}


/*
 *
 *       Fun:   vbMmeUtlBldTAUAcc
 *
 *       Desc:  This function builds an EMM TAU Accept message
 *              structure.
 *
 *       Ret:   ROK     on   success
 *              RFAILED on   failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldTAUAcc
(
VbMmeEmmCb   *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt    **evnt
)
#else
PUBLIC S16 vbMmeUtlBldTAUAcc(ueEmmCb, evnt)
VbMmeEmmCb   *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt    **evnt;
#endif
{
   S16             ret = ROK;
   CmEmmMsg        *emmMsg;
   U8              stmsi[5];
   VB_TRC2(vbMmeUtlBldTAUAcc)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building TAU Accept"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB082, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB083, ERRZERO,
                    "cmGetMem Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD;
#endif

   (*evnt)->m.emmEvnt = emmMsg;

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_TAU_ACC;
   emmMsg->protDisc = CM_EMM_PD;

   /*******************************************************************
    * Fill Message Body
    ******************************************************************/
   /* Fill EPS Update Result */
   emmMsg->u.tauAcc.epsUpdRes.pres = TRUE;
   /*RAJESH */
   if(CM_EMM_COMB_TA_LA_UPDATING_IMSI_ATTACH == ueEmmCb->ueCb->ueCtxt.tauType)
   {
       emmMsg->u.tauAcc.epsUpdRes.res = 1; /*CM_EMM_COMB_TA_LA_UPDATING*/;
   }
   else
   {
       emmMsg->u.tauAcc.epsUpdRes.res = 0; /* TA Updated */
   }

   /* Fill T3412 timer value */
   emmMsg->u.tauAcc.t3412.pres = TRUE;

   /* + Deactivate timer if t3412 is set zero + */
   if(ueEmmCb->ueCb->eNodebCb->eNbCfg.t3412Tmr == 0)
   {
      emmMsg->u.tauAcc.t3412.val = 0;
      emmMsg->u.tauAcc.t3412.unit = 7;
   }
   else if(ueEmmCb->ueCb->eNodebCb->eNbCfg.t3412Tmr <= 64)
   /* - Deactivate timer if t3412 is set zero - */
   {
      emmMsg->u.tauAcc.t3412.val = ueEmmCb->ueCb->eNodebCb->eNbCfg.t3412Tmr/2;
      emmMsg->u.tauAcc.t3412.unit = 0;
   }
   else if(ueEmmCb->ueCb->eNodebCb->eNbCfg.t3412Tmr <= 32*60)
   {
      emmMsg->u.tauAcc.t3412.val = ueEmmCb->ueCb->eNodebCb->eNbCfg.t3412Tmr/60;
      emmMsg->u.tauAcc.t3412.unit = 1;
   }
   else
   {
      emmMsg->u.tauAcc.t3412.val = ueEmmCb->ueCb->eNodebCb->eNbCfg.t3412Tmr/360;
      emmMsg->u.tauAcc.t3412.unit = 2;
   }

   /* Fill guti */
   if(TRUE == ueEmmCb->ueCb->ueCtxt.gutiAlloc)
   {
      ret = vbMmeUtlDelUeCbOnSTmsi(ueEmmCb->ueCb);
      if(ret != ROK)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "STmsi Based Deletion Failed"));
      }
      cmMemset((U8*)&ueEmmCb->ueCb->ueCtxt.ueGuti, 0, sizeof(GUTI));

      /* Allocate a new GUTI for the UE */
      vbMmeUtlAllocGuti(&ueEmmCb->ueCb->ueCtxt.ueGuti);

      /* Add to hashlist based on STMSI */
      stmsi[0] = ueEmmCb->ueCb->ueCtxt.ueGuti.mmeCode & 0xff;
      stmsi[1] = (ueEmmCb->ueCb->ueCtxt.ueGuti.mTMSI>>24) & 0xff;
      stmsi[2] = (ueEmmCb->ueCb->ueCtxt.ueGuti.mTMSI>>16) & 0xff;
      stmsi[3] = (ueEmmCb->ueCb->ueCtxt.ueGuti.mTMSI>>8) & 0xff;
      stmsi[4] = (ueEmmCb->ueCb->ueCtxt.ueGuti.mTMSI) & 0xff;

      ret = vbMmeUtlAddUeCbOnSTmsi(stmsi, ueEmmCb->ueCb);
      if(ret != ROK)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB049, ERRZERO,
           "Failed To Insert Into Stmsi Hashlist");
         RETVALUE(RFAILED);
      }

      cmMemcpy((U8 *)&emmMsg->u.tauAcc.guti.u.guti,\
            (U8 *)&ueEmmCb->ueCb->ueCtxt.ueGuti, sizeof(GUTI));
      emmMsg->u.tauAcc.guti.pres = TRUE;
      emmMsg->u.tauAcc.guti.type = CM_EMM_MID_TYPE_GUTI;
      /*It is 0 in case of GUTI as per the Spec */
      emmMsg->u.tauAcc.guti.evenOddInd = 0;
   }

   if(CM_EMM_COMB_TA_LA_UPDATING_IMSI_ATTACH == ueEmmCb->ueCb->ueCtxt.tauType)
   {
      emmMsg->u.tauAcc.lai.pres = TRUE;
      emmMsg->u.tauAcc.lai.mcc[0] = 2;
      emmMsg->u.tauAcc.lai.mcc[1] = 0;
      emmMsg->u.tauAcc.lai.mcc[2] = 8;
      emmMsg->u.tauAcc.lai.mnc[0] = 0;
      emmMsg->u.tauAcc.lai.mnc[1] = 1;
      emmMsg->u.tauAcc.lai.mnc[2] = 15;
      emmMsg->u.tauAcc.lai.lac = 6000 ;

      emmMsg->u.tauAcc.feature.pres = TRUE;
      emmMsg->u.tauAcc.feature.imsVops = 0;
   }

   if((CM_EMM_COMB_TA_LA_UPDATING == ueEmmCb->ueCb->ueCtxt.tauType) /*||
      (CM_EMM_COMB_TA_LA_UPDATING_IMSI_ATTACH == ueEmmCb->ueCb->ueCtxt.tauType)*/)
   {
      emmMsg->u.tauAcc.cause.pres = TRUE;
      emmMsg->u.tauAcc.cause.cause =  CM_EMM_CS_DOM_NA;
   }
   /* + Always send bearer context + */
   /*if(1 == ueEmmCb->ueCb->ueCtxt.tauActv)*/
   /* - Always send bearer context - */
   {
      emmMsg->u.tauAcc.bearerSts.pres = TRUE;
      emmMsg->u.tauAcc.bearerSts.ebi00 = 0;
      emmMsg->u.tauAcc.bearerSts.ebi01 = 0;
      emmMsg->u.tauAcc.bearerSts.ebi02 = 0;
      emmMsg->u.tauAcc.bearerSts.ebi03 = 0;
      emmMsg->u.tauAcc.bearerSts.ebi04 = 0;

      emmMsg->u.tauAcc.bearerSts.ebi05 = (ueEmmCb->ueCb->esmList[0] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi06 = (ueEmmCb->ueCb->esmList[1] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi07 = (ueEmmCb->ueCb->esmList[2] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi08 = (ueEmmCb->ueCb->esmList[3] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi09 = (ueEmmCb->ueCb->esmList[4] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi10 = (ueEmmCb->ueCb->esmList[5] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi11 = (ueEmmCb->ueCb->esmList[6] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi12 = (ueEmmCb->ueCb->esmList[7] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi13 = (ueEmmCb->ueCb->esmList[8] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi14 = (ueEmmCb->ueCb->esmList[9] == NULL)? 0:1;
      emmMsg->u.tauAcc.bearerSts.ebi15 = (ueEmmCb->ueCb->esmList[10] == NULL)? 0:1;
   }
#if 1  /**RAJESH */
    emmMsg->u.tauAcc.addUpdRes.pres = 1;
    emmMsg->u.tauAcc.addUpdRes.addUpdRes = 0;
#endif
   /* Fill tracking area id list */
   cmMemcpy((U8*)&emmMsg->u.tauAcc.taiLst, (U8*)&ueEmmCb->ueCb->ueCtxt.taList,
            sizeof(emmMsg->u.tauAcc.taiLst));

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeUtlBldTAURej
 *
 *       Desc:  This function builds an EMM TAU Rej message
 *              structure.
 *
 *       Ret:   ROK     on   success
 *              RFAILED on   failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldTAURej
(
VbMmeEmmCb   *ueEmmCb,    /* UE EMM Cb */
CmNasEvnt    **evnt,
U8           cause
)
#else
PUBLIC S16 vbMmeUtlBldTAURej(ueEmmCb, evnt, cause)
VbMmeEmmCb   *ueEmmCb;    /* UE EMM Cb */
CmNasEvnt    **evnt;
U8           cause;
#endif
{
   S16 ret = ROK;
   CmEmmMsg        *emmMsg;
   VB_TRC2(vbMmeUtlBldTAURej)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building TAU Rej"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB082, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB083, ERRZERO,
                    "cmGetMem Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

#ifdef VB_MME_NAS_SEC
   if(VB_MME_SEC_CTXT_CRNT == ueEmmCb->ueCb->ueCtxt.secCtxt.status)
   {
      (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
   }
#endif

   (*evnt)->m.emmEvnt = emmMsg;

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_TAU_REJ;
   emmMsg->protDisc = CM_EMM_PD;

   /* Filling Message Body */
   emmMsg->u.tauRej.cause.pres = TRUE;
   emmMsg->u.tauRej.cause.cause = cause;

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeUtlBldEmmStatus
 *
 *       Desc:  This function builds an EMM Status message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEmmStatus
(
VbMmeEmmCb  *emmCb,   /* UE Cb */
CmNasEvnt  **evnt,    /* Attach Reject pdu to be built */
U8         emmCause/* EMM Cause value attach reject */
)
#else
PUBLIC S16 vbMmeUtlBldEmmStatus (emmCb, evnt, emmCause)
VbMmeEmmCb  *emmCb;   /* UE Cb */
CmNasEvnt   **evnt;  /* Attach Reject pdu to be built */
U8          emmCause; /* EMM Cause value attach reject */
#endif
{
   CmEmmMsg           *emmMsg    = NULLP;

   VB_TRC2(vbMmeUtlBldEmmStatus);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Emm Status Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt  Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;

#ifdef VB_MME_NAS_SEC
  if(VB_MME_SEC_CTXT_CRNT == emmCb->ueCb->ueCtxt.secCtxt.status)
  {
     (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
  }
#endif

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_EMM_STS;

/* vb004.101 : 116186: added protocol descriptor type */
   emmMsg->protDisc = CM_EMM_PD;

   /* Fill mandatory IEs */
   /* Fill the attach reject cause */
   (*evnt)->m.emmEvnt->u.emmSts.emmCause.cause = emmCause;
   /* vb004.101: Missing pres value added. */
   (*evnt)->m.emmEvnt->u.emmSts.emmCause.pres = 1;

   RETVALUE(ROK);

} /* vbMmeUtlBldEmmStatus */

/*******************************************************/
/*      Functions to build ESM Messages                */
/*******************************************************/

/*
*
*       Fun:   vbMmeUtlBldDfltActvBearerReq
*
*       Desc:  This function is used to build ESM Def activate bearer request
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldDfltActvBearerReq
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   **evnt
)
#else
PUBLIC S16 vbMmeUtlBldDfltActvBearerReq (esmCb, evnt)
VbMmeEsmCb  *esmCb;
CmNasEvnt    **evnt;
#endif /* ANSI */
{
   U32               finalApnLen = 0;
   CmEsmMsg          *msg = NULLP;
   CmInetNetAddr     pdnAddr;
   VbHssPdnSubCntxt  *pdnCtxt    = NULLP;
   U16               ipaddr1 = 0;
   U16               ipaddr2 = 0;
   U32               dnsAddr = 0;
   U32               pcscfAddr = 3;

   VB_TRC2(vbMmeUtlBldDfltActvBearerReq);

   if(esmCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Default Activate Bearer Request"));

   if(esmCb->ueCb->ueTunnelCb != NULLP)
   {
      pdnAddr = esmCb->ueCb->ueTunnelCb->ipAddr;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "UeTunnelCb Not Found"));
      RETVALUE(RFAILED);
   }

   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB084, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB085, ERRZERO,
            "Failed To Allocate Memory To ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* fill ESM Activiate default bearer request message */
   pdnCtxt= &esmCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[0];
   /* Mark this APN as allocated APN */
   pdnCtxt->subCntxtinUse = TRUE;
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = esmCb->bId;
   msg->prTxnId  = esmCb->tId;
   msg->msgType  = CM_ESM_MSG_ACTV_DEF_BEAR_REQ;

   /* fill EPS Qos. For default bearer MBR/BGR are not applicable */
#if 0
	cmMemcpy((U8 *)&(msg->u.actReq.epsQos), (U8*)&esmCb->epsQos,\
			sizeof(CmEsmEpsQos));
#endif
   msg->u.actReq.epsQos.pres = TRUE;
   msg->u.actReq.epsQos.lenQosCont = 1;   /* Only QCI to be sent to UE  *
                                           * In default bearer          */
   msg->u.actReq.epsQos.qci = esmCb->epsQos.qci;

   /* Fill Access point name */
   if (esmCb->apn.pres)
   {
      msg->u.actReq.apn.pres = TRUE;
      msg->u.actReq.apn.apn[finalApnLen++] = esmCb->apn.len;
      cmMemcpy((U8 *)(msg->u.actReq.apn.apn + 1), (U8 *)esmCb->apn.apn,
            esmCb->apn.len);
      finalApnLen += esmCb->apn.len;
   }
   else
   {
      msg->u.actReq.apn.pres = TRUE;
      msg->u.actReq.apn.apn[finalApnLen++] = cmStrlen((U8 *)pdnCtxt->apn);
      cmMemcpy((U8 *)(msg->u.actReq.apn.apn + 1), (U8 *)pdnCtxt->apn,
            cmStrlen((U8 *)pdnCtxt->apn));
      finalApnLen += cmStrlen((U8 *)pdnCtxt->apn);
   }

   /* mnc of apn*/
   msg->u.actReq.apn.apn[finalApnLen++] = 6;
   msg->u.actReq.apn.apn[finalApnLen++] = 'm';
   msg->u.actReq.apn.apn[finalApnLen++] = 'n';
   msg->u.actReq.apn.apn[finalApnLen++] = 'c';
   msg->u.actReq.apn.apn[finalApnLen++] = esmCb->ueCb->ueCtxt.ueImsi[3] + '0';
   msg->u.actReq.apn.apn[finalApnLen++] = esmCb->ueCb->ueCtxt.ueImsi[4] + '0';
   msg->u.actReq.apn.apn[finalApnLen++] = 0x30;

   /* mcc of apn*/
   msg->u.actReq.apn.apn[finalApnLen++] = 6;
   msg->u.actReq.apn.apn[finalApnLen++] = 'm';
   msg->u.actReq.apn.apn[finalApnLen++] = 'c';
   msg->u.actReq.apn.apn[finalApnLen++] = 'c';
   msg->u.actReq.apn.apn[finalApnLen++] = esmCb->ueCb->ueCtxt.ueImsi[0] + '0';
   msg->u.actReq.apn.apn[finalApnLen++] = esmCb->ueCb->ueCtxt.ueImsi[1] + '0';
   msg->u.actReq.apn.apn[finalApnLen++] = esmCb->ueCb->ueCtxt.ueImsi[2] + '0';

   /* gprs of apn */
   msg->u.actReq.apn.apn[finalApnLen++] = 4;
   msg->u.actReq.apn.apn[finalApnLen++] = 'g';
   msg->u.actReq.apn.apn[finalApnLen++] = 'p';
   msg->u.actReq.apn.apn[finalApnLen++] = 'r';
   msg->u.actReq.apn.apn[finalApnLen++] = 's';

   msg->u.actReq.apn.len = finalApnLen;

   /* PDN Address */
   msg->u.actReq.pAddr.pres  = TRUE;
   if(pdnCtxt->pdnAddrType == CM_TPTADDR_IPV4)
   {
      ipaddr1 = GetHiWord(pdnAddr.u.ipv4NetAddr);
      ipaddr2 = GetLoWord(pdnAddr.u.ipv4NetAddr);

      msg->u.actReq.pAddr.pdnType  = CM_ESM_PDN_IPV4;
      msg->u.actReq.pAddr.len  = 5;
      msg->u.actReq.pAddr.addrInfo[0] = GetHiByte(ipaddr1);
      msg->u.actReq.pAddr.addrInfo[1] = GetLoByte(ipaddr1);
      msg->u.actReq.pAddr.addrInfo[2] = GetHiByte(ipaddr2);
      msg->u.actReq.pAddr.addrInfo[3] = GetLoByte(ipaddr2);
   }
#ifdef IPV6_SUPPORTED
   else if(pdnCtxt->pdnAddrType == CM_TPTADDR_IPV6)
   {
      msg->u.actReq.pAddr.pdnType  = CM_ESM_PDN_IPV6;
      msg->u.actReq.pAddr.len  = 7;
      cmMemcpy((U8 *)(msg->u.actReq.pAddr.addrInfo),
            (U8 *)(&(pdnAddr.u.ipv6NetAddr)), 7);
   }
   else
   {
      msg->u.actReq.pAddr.pdnType  = CM_ESM_PDN_IPV4V6;
      msg->u.actReq.pAddr.len  = 11;
      cmMemcpy((U8 *)msg->u.actReq.pAddr.addrInfo,&(pdnAddr.u.ipv4NetAddr), 11);
   }
#endif
   /* fill transaction id
    * Note: Need to check the UE network capability before filling this IE
    * */

    msg->u.actReq.txnId.pres   = TRUE;
    msg->u.actReq.txnId.len    = 1;
    msg->u.actReq.txnId.tiVal  = 0;
    msg->u.actReq.txnId.tiFlag = 0;
    msg->u.actReq.txnId.tie    = 0;
    msg->u.actReq.txnId.tiExt  = 0;

   /* fill QOS
    * Note: Need to check the UE network capability before filling this IE
    * */
    msg->u.actReq.qos.pres           = TRUE;
    msg->u.actReq.qos.lenQosCont     = 16;
    msg->u.actReq.qos.relClass       = 3;
    msg->u.actReq.qos.delayClass     = 1;
    msg->u.actReq.qos.precClass      = 2;
    msg->u.actReq.qos.peakTp         = 5;
    msg->u.actReq.qos.meanTp         = 31;
    msg->u.actReq.qos.deliveryErrSdu = 3;
    msg->u.actReq.qos.deliveryOrder  = 2;
    msg->u.actReq.qos.trafficClass   = 3;
    msg->u.actReq.qos.maxSdu         = 140;
    msg->u.actReq.qos.maxBitRateUL   = 254;
    msg->u.actReq.qos.maxBitRateDL   = 254;
    msg->u.actReq.qos.sduErrRatio    = 6;
    msg->u.actReq.qos.residualBer    = 7;
    msg->u.actReq.qos.trafHandPrio   = 3;
    msg->u.actReq.qos.transferDelay  = 18;
    msg->u.actReq.qos.guaraBitRateUL = 86;
    msg->u.actReq.qos.guaraBitRateDL = 86;
    msg->u.actReq.qos.srcStatDesc    = 0;
    msg->u.actReq.qos.signalInd      = 0;
    msg->u.actReq.qos.maxBitRateDLExt = 108;
    msg->u.actReq.qos.guaraBitRateDLExt = 0;
    msg->u.actReq.qos.maxBitRateULExt = 108;
    msg->u.actReq.qos.guaraBitRateULExt = 0;


   /* fill apn ambr */
   msg->u.actReq.apnAmbr.pres = TRUE;
   msg->u.actReq.apnAmbr.len = 6;
   msg->u.actReq.apnAmbr.dl = pdnCtxt->ambr.dl;
   msg->u.actReq.apnAmbr.ul = pdnCtxt->ambr.ul;
   msg->u.actReq.apnAmbr.dlExt = pdnCtxt->ambr.dlExt;
   msg->u.actReq.apnAmbr.ulExt = pdnCtxt->ambr.ulExt;
   msg->u.actReq.apnAmbr.dlExt2 = pdnCtxt->ambr.dlExt2;
   msg->u.actReq.apnAmbr.ulExt2 = pdnCtxt->ambr.ulExt2;

   /* fill PCO */
   msg->u.actReq.protCfgOpt.pres = TRUE;
   msg->u.actReq.protCfgOpt.len = esmCb->pco.len;
   msg->u.actReq.protCfgOpt.cfgProt = esmCb->pco.cfgProt;
   msg->u.actReq.protCfgOpt.ext = esmCb->pco.ext;
   msg->u.actReq.protCfgOpt.p[0].pid = esmCb->pco.p[0].pid;
   msg->u.actReq.protCfgOpt.p[0].len = esmCb->pco.p[0].len;
   cmMemcpy((U8* ) msg->u.actReq.protCfgOpt.p[0].val, (U8* )esmCb->pco.p[0].val, esmCb->pco.p[0].len);
   pdnCtxt->pDnsAddr.type = CM_INET_IPV4ADDR_TYPE;
   pdnCtxt->pDnsAddr.u.ipv4NetAddr = msg->u.actReq.protCfgOpt.p[0].val[6];
   pdnCtxt->pDnsAddr.u.ipv4NetAddr = (pdnCtxt->pDnsAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.p[0].val[7];
   pdnCtxt->pDnsAddr.u.ipv4NetAddr = (pdnCtxt->pDnsAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.p[0].val[8];
   pdnCtxt->pDnsAddr.u.ipv4NetAddr = (pdnCtxt->pDnsAddr.u.ipv4NetAddr << 8) |msg->u.actReq.protCfgOpt.p[0].val[9];
   pdnCtxt->sDnsAddr.type = CM_INET_IPV4ADDR_TYPE;
   pdnCtxt->sDnsAddr.u.ipv4NetAddr = msg->u.actReq.protCfgOpt.p[0].val[12];
   pdnCtxt->sDnsAddr.u.ipv4NetAddr = (pdnCtxt->sDnsAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.p[0].val[13];
   pdnCtxt->sDnsAddr.u.ipv4NetAddr = (pdnCtxt->sDnsAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.p[0].val[14];
   pdnCtxt->sDnsAddr.u.ipv4NetAddr = (pdnCtxt->sDnsAddr.u.ipv4NetAddr << 8) |msg->u.actReq.protCfgOpt.p[0].val[15];

   msg->u.actReq.protCfgOpt.c[0].cid = esmCb->pco.c[0].cid; /* P-CSCF */
   msg->u.actReq.protCfgOpt.c[0].len = esmCb->pco.c[0].len;
   cmMemcpy((U8* ) msg->u.actReq.protCfgOpt.c[0].val, (U8* )esmCb->pco.c[0].val, esmCb->pco.c[0].len);
   pdnCtxt->pcscfAddr.type = CM_INET_IPV4ADDR_TYPE;
   pdnCtxt->pcscfAddr.u.ipv4NetAddr = msg->u.actReq.protCfgOpt.c[0].val[0];
   pdnCtxt->pcscfAddr.u.ipv4NetAddr = (pdnCtxt->pcscfAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.c[0].val[1];
   pdnCtxt->pcscfAddr.u.ipv4NetAddr = (pdnCtxt->pcscfAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.c[0].val[2];
   pdnCtxt->pcscfAddr.u.ipv4NetAddr = (pdnCtxt->pcscfAddr.u.ipv4NetAddr << 8) | msg->u.actReq.protCfgOpt.c[0].val[3];

   if((CM_ESM_PDN_IPV6 == esmCb->pdnType) || \
      (CM_ESM_PDN_IPV4V6 == esmCb->pdnType))
   {
      msg->u.actReq.cause.pres = TRUE;
      msg->u.actReq.cause.val = VB_ESM_PDN_IPV4_ONLY_ALWD;
   }

   RETVALUE(ROK);
} /* vbMmeUtlBldDfltActvBearerReq */


/* vb001.101: Added new function */
/*
*
*       Fun:   vbMmeUtlBldActvDedBearerReq
*
*       Desc:  This function is used to build ESM activate Ded bearer request
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldActvDedBearerReq
(
VbMmeEsmCb  *ueEsmCb,
CmNasEvnt   **evnt
)
#else
PUBLIC S16 vbMmeUtlBldActvDedBearerReq (ueEsmCb, evnt)
VbMmeEsmCb  *ueEsmCb;
CmNasEvnt   **evnt;
#endif /* ANSI */
{
   CmEsmMsg         *msg = NULLP;
   VbHssPdnSubCntxt pdnCtxt;
   VbMmeUeCb  *ueCb;

   VB_TRC2(vbMmeUtlBldActvDedBearerReq);

   ueCb = ueEsmCb->ueCb;

   if(ueCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Dedicated Bearer Request"));


   pdnCtxt = ueCb->ueCtxt.ueHssCtxt.subCntxt[0];

   if (ueCb->ueTunnelCb == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "UeTunnelCb Not Found"));
      RETVALUE(RFAILED);
   }

   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB084, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB085, ERRZERO,
            "Failed To Allocate Memory To ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;
#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* fill ESM Activiate dedicated bearer request message */
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = ueEsmCb->bId;
   msg->prTxnId  = ueEsmCb->tId;
   msg->msgType  = CM_ESM_MSG_ACTV_DED_BEAR_REQ;

   /* fill EPS Qos */
   msg->u.actDedBearReq.epsQos.pres        = TRUE;
   msg->u.actDedBearReq.epsQos.qci         = ueEsmCb->epsQos.qci;
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
           "########filling qci value: %d \n", ueEsmCb->epsQos.qci));
   if(msg->u.actDedBearReq.epsQos.qci >= 5)
   {
      msg->u.actDedBearReq.epsQos.lenQosCont  = 1;
   }
   else
   {
   msg->u.actDedBearReq.epsQos.lenQosCont  = 9;
   msg->u.actDedBearReq.epsQos.maxBitRateUL  = pdnCtxt.qos.maxBitRateUL;
   msg->u.actDedBearReq.epsQos.maxBitRateDL  = pdnCtxt.qos.maxBitRateDL;
   msg->u.actDedBearReq.epsQos.guaraBitRateUL  = pdnCtxt.qos.guaraBitRateUL;
   msg->u.actDedBearReq.epsQos.guaraBitRateDL  = pdnCtxt.qos.guaraBitRateDL;
   }
   ueEsmCb->linkBearId = ueCb->esmList[0]->bId;
   /* Fill the Linked Bearer ID */
   msg->u.actDedBearReq.bearerId  = ueEsmCb->linkBearId ;

   msg->u.actDedBearReq.tft.len = ueEsmCb->tft.len;
   /* Fill the TFT */
   cmMemcpy((U8*)&msg->u.actDedBearReq.tft,(U8*)&ueEsmCb->tft,sizeof(CmEsmTft));

   RETVALUE(ROK);
} /* vbMmeUtlBldActvDedBearerReq */

/*
*
*       Fun:   vbMmeUtlBldDeActvDedBearerReq
*
*       Desc:  This function is used to build ESM deactivate Ded bearer request
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldDeActvDedBearerReq
(
VbMmeEsmCb  *ueEsmCb,
CmNasEvnt   **evnt,
CmEsmCause   cause
)
#else
PUBLIC S16 vbMmeUtlBldDeActvDedBearerReq (ueEsmCb, evnt)
VbMmeEsmCb  *ueEsmCb;
CmNasEvnt   **evnt;
CmEsmCause   cause;
#endif /* ANSI */
{
   CmEsmMsg         *msg = NULLP;
   VbMmeUeCb  *ueCb;

   VB_TRC2(vbMmeUtlBldDeActvDedBearerReq);

   ueCb = ueEsmCb->ueCb;

   if(ueCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building DeActDedicated Bearer Request"));


   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB084, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB085, ERRZERO,
            "Failed To Allocate Memory To ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* fill ESM DeActiviate dedicated bearer request message */
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = ueEsmCb->bId;
   msg->prTxnId  = ueEsmCb->tId;
   msg->msgType  = CM_ESM_MSG_DEACTV_BEAR_REQ;

   /* Fill the  Bearer ID */
 /*  msg->u.deactBearReq.bearerId  = ueEsmCb->bId ;*/

   /* fill the cause */
   msg->u.deactBearReq.cause   = cause;

   RETVALUE(ROK);
} /* vbMmeUtlBldDeActvDedBearerReq */

/*
*
*       Fun:   vbMmeUtlBldDeActvDefBearerReq
*
*       Desc:  This function is used to build ESM deactivate Def bearer request
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldDeActvDefBearerReq
(
VbMmeEsmCb  *ueEsmCb,
CmNasEvnt   **evnt,
CmEsmCause   cause
)
#else
PUBLIC S16 vbMmeUtlBldDeActvDefBearerReq (ueEsmCb, evnt)
VbMmeEsmCb  *ueEsmCb;
CmNasEvnt   **evnt;
CmEsmCause   cause;
#endif /* ANSI */
{
   CmEsmMsg         *msg = NULLP;
   VbMmeUeCb        *ueCb;

   VB_TRC2(vbMmeUtlBldDeActvDefBearerReq);

   ueCb = ueEsmCb->ueCb;

   if(ueCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building DeActDedicated Bearer Request"));


   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB084, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB085, ERRZERO,
            "Failed To Allocate Memory To ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif


   /* fill ESM DeActiviate dedicated bearer request message */
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = ueEsmCb->bId;
   msg->prTxnId  = ueEsmCb->tId;
   msg->msgType  = CM_ESM_MSG_DEACTV_BEAR_REQ;

   /* Fill the  Bearer ID */
 /*  msg->u.deactBearReq.bearerId  = ueEsmCb->bId ;*/

   /* fill the cause */
   msg->u.deactBearReq.cause   = cause;

   RETVALUE(ROK);
} /* vbMmeUtlBldDeActvDefBearerReq */

/*
*
*       Fun:   vbMmeUtlBldModBearerCtxtReq
*
*       Desc:  This function is used to build ESM Modify bearer Ctxt request
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldModBearerCtxtReq
(
VbMmeEsmCb  *ueEsmCb,
CmNasEvnt   **evnt,
CmEsmCause   cause
)
#else
PUBLIC S16 vbMmeUtlBldModBearerCtxtReq (ueEsmCb, evnt)
VbMmeEsmCb  *ueEsmCb;
CmNasEvnt   **evnt;
CmEsmCause   cause;
#endif /* ANSI */
{
   CmEsmMsg         *msg = NULLP;
   VbMmeUeCb  *ueCb;

   VB_TRC2(vbMmeUtlBldModBearerCtxtReq);

   UNUSED(cause);
   ueCb = ueEsmCb->ueCb;

   if(ueCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building Mod Bearer Ctxt Request"));


   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB084, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB085, ERRZERO,
            "Failed To Allocate Memory To ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif


   /* fill ESM Modify bearer ctxt request message */
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = ueEsmCb->bId;
   msg->prTxnId  = ueEsmCb->tId;
   msg->msgType  = CM_ESM_MSG_MOD_BEAR_REQ;

   RETVALUE(ROK);
} /* vbMmeUtlBldModBearerCtxtReq */

/*
 *
 *       Fun:   vbMmeUtlBldEgtBearResCmd
 *
 *       Desc:  Builds EGTP-C Create Bearer Command Request
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtBearResCmd
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg,
Bool            isRelease
)
#else
PUBLIC S16 vbMmeUtlBldEgtBearResCmd(ueTunlCb, esmCb, egMsg, isRelease)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb;
EgMsg           **egMsg;
Bool            isRelease;
#endif
{
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   EgIe           egIe;
   CmEsmEpsQos    *epsQos;



   VB_TRC2(vbMmeUtlBldEgtBearResCmd)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Building Bearer Resource Command"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
    /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   epsQos = &esmCb->epsQos;

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   /* vb005.101 Sequence number is 24bit long, so MSB is at 24th bit */
#ifndef EG_REL_930
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_BR_CMND, (ueTunlCb->seqNum | (0xff << 8)) );
#else
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_BR_CMND, (++ueTunlCb->seqNum | (0x800000)) );
#endif /* EG_REL_930 */
   /* Add Linked EPS Bearer ID */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddEpsBId(*egMsg, &egLvlInfo, 5 /* ueTunlCb->bearerId*/, 0);

   /* Add PTI Network */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtilEgtAddPTidIe(*egMsg,esmCb->tId,&egLvlInfo ,0);


   /* Fill if esmCause not received in Bearer Res Mod Req msg */
    if(!isRelease)
   {
      /* Add epsQos */
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      egIe.t.fQos.qci = epsQos->qci;
      egIe.t.fQos.upMBR.length = 5;
      vbMmeUtlGetBitRate(epsQos->maxBitRateUL, epsQos->maxBitRateULExt, &egIe.t.fQos.upMBR.val[1]);
      egIe.t.fQos.dnMBR.length = 5;
      vbMmeUtlGetBitRate(epsQos->maxBitRateDL, epsQos->maxBitRateDLExt, &egIe.t.fQos.dnMBR.val[1]);
      egIe.t.fQos.upGBR.length = 5;
      vbMmeUtlGetBitRate(epsQos->guaraBitRateUL, epsQos->guaraBitRateULExt, &egIe.t.fQos.upGBR.val[1]);
      egIe.t.fQos.dnGBR.length = 5;
      vbMmeUtlGetBitRate(epsQos->guaraBitRateDL, epsQos->guaraBitRateDLExt, &egIe.t.fQos.dnGBR.val[1]);
      egUtilEgtAddFlowQosIe(&egIe, *egMsg, &egLvlInfo,0);
    }
    /* Add TAD Type */
    cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
    /*TODO Get the TFT from the ESM CB. Get the esmCb as the argument
    * of this function tad 0 will containt the length*/
    egUtilEgtAddTadIe(*egMsg, &egLvlInfo, 0,esmCb->tft.buf, esmCb->tft.len);

   if(isRelease)
   {
      /* Add EPS Bearer ID */
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      egUtlEgtAddEpsBId(*egMsg, &egLvlInfo, esmCb->bId , 0);
   }

   RETVALUE(ROK);
}/* End of function vbMmeUtlBldEgtBearResCmd */



/*
 *
 *       Fun:   vbMmeUtlBldEgtModifyBearCmd
 *
 *       Desc:  Builds EGTP-C Modify Bearer Command
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtModifyBearCmd
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg
)
#else
PUBLIC S16 vbMmeUtlBldEgtModifyBearCmd(ueTunlCb, esmCb, egMsg)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb;
EgMsg           **egMsg;
#endif
{
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   TknU8          bearerId;
   VbHssSubUeAMBR ambr;

   VB_TRC2(vbMmeUtlBldEgtModifyBearCmd)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Building Modify Bearer Command"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&bearerId, 0 , sizeof(TknU8));
   /* Build the Header  with Default values */
   /* vb005.101 Sequence number is 24bit long, so MSB is at 24th bit */
#ifndef EG_REL_930
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_MB_CMND, (ueTunlCb->seqNum | (0xff << 8)) );
#else
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_MB_CMND, (++ueTunlCb->seqNum | (0x800000)) );
#endif /* EG_REL_930 */

   /* Add APN-AMBR */
   cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
   ambr = esmCb->ueCb->ueCtxt.ueHssCtxt.subUeAMBR;
   egUtlEgtAddAmbr(*egMsg, &egLvlInfo,(U8 *)&ambr, 0);

   /* Add  BearerCtxt */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   bearerId.pres = PRSNT_NODEF;
   bearerId.val = esmCb->bId;
   egUtlEgtAddBearerCxt(*egMsg, &egLvlInfo, 1, &bearerId, NULLP, 0, NULLP, 0, NULLP);

   RETVALUE(ROK);
}/* End of function vbMmeUtlBldEgtModifyBearCmd */


/*
 *
 *       Fun:   vbMmeUtlBldEgtDeleteBearCmd
 *
 *       Desc:  Builds EGTP-C Delete Bearer Command
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtDeleteBearCmd
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg
)
#else
PUBLIC S16 vbMmeUtlBldEgtDeleteBearCmd(ueTunlCb, esmCb, egMsg)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb;
EgMsg           **egMsg;
#endif
{
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;
   TknU8          bearerId;

   VB_TRC2(vbMmeUtlBldEgtDeleteBearCmd)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
               "Building Delete Bearer Command"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&bearerId, 0 , sizeof(TknU8));
   /* Build the Header  with Default values */
   /* vb005.101 Sequence number is 24bit long, so MSB is at 24th bit */
#ifndef EG_REL_930
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_DB_CMND, (ueTunlCb->seqNum | (0xff << 8)) );
#else
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_DB_CMND, (++ueTunlCb->seqNum | (0x800000)) );
#endif /* EG_REL_930 */

   /* Add  BearerCtxt */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   bearerId.pres = PRSNT_NODEF;
   bearerId.val = esmCb->bId;
   egUtlEgtAddBearerCxt(*egMsg, &egLvlInfo, 1, &bearerId, NULLP, 0, NULLP, 0, NULLP);

   RETVALUE(ROK);
}/* End of function vbMmeUtlBldEgtDeleteBearCmd */

/*
*
*       Fun:   vbMmeUtlBldPdnConnReject
*
*       Desc:  This function is used to build ESM PDN conn reject
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldPdnConnReject
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   **evnt,
U8          cause
)
#else
PUBLIC S16 vbMmeUtlBldPdnConnReject (esmCb, evnt, cause)
VbMmeEsmCb  *esmCb;
CmNasEvnt   **evnt;
U8          cause;
#endif /* ANSI */
{
   CmEsmMsg    *msg = NULLP;

   VB_TRC2(vbMmeUtlBldPdnConnReject);

   if(esmCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building PDN Connectivity Reject"));

   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB086, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB087, ERRZERO,
            "Failed To Allocate Memory For ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* fill ESM PDN connectivity reject message */
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = 0 /* esmCb->bId*/;
   msg->prTxnId  = esmCb->tId;
   msg->msgType  = CM_ESM_MSG_PDN_CONN_REJ;

   /* fill ESM cause */
   msg->u.conRej.cause.pres = TRUE;
   msg->u.conRej.cause.val  = cause;

   RETVALUE(ROK);
} /* vbMmeUtlBldPdnConnReject */


/*
*
*       Fun:   vbMmeUtlBldPdnDisConnReject
*
*       Desc:  This function is used to build ESM PDN Disconn reject
*
*       Ret:  ROK - ok; RFAILED - failed
*
*       Notes: None
*
*       File:  vb_mme_utl.c
*
*/

#ifdef ANSI
PUBLIC S16  vbMmeUtlBldPdnDisConnReject
(
VbMmeEsmCb  *esmCb,
CmNasEvnt   **evnt,
U8          cause
)
#else
PUBLIC S16 vbMmeUtlBldPdnDisConnReject (esmCb, evnt, cause)
VbMmeEsmCb  *esmCb;
CmNasEvnt   **evnt;
U8          cause;
#endif /* ANSI */
{
   CmEsmMsg    *msg = NULLP;

   VB_TRC2(vbMmeUtlBldPdnDisConnReject);

   if(esmCb == NULLP || evnt == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
            "Invalid esmCb Or Message"));
      RETVALUE(RFAILED);
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Building PDN Dis Connectivity Reject"));

   /* Create mme evnt */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);
   if (*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB086, ERRZERO,
            "Failed To Create Mme Event");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&msg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB087, ERRZERO,
            "Failed To Allocate Memory For ESM Message");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = msg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* fill ESM PDN connectivity reject message */
   msg->protDisc = CM_ESM_PD;
   msg->bearerId = 0 /* esmCb->bId*/;
   msg->prTxnId  = esmCb->tId;
   msg->msgType  = CM_ESM_MSG_PDN_DISCONN_REJ;

   /* fill ESM cause */
   msg->u.disConRej.cause.pres = TRUE;
   msg->u.disConRej.cause.val  = cause;
   RETVALUE(ROK);
}

/*
 *
 *      Fun:   vbMmeUtlGetIE
 *
 *      Desc:  Retrieve message index
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: None.
 *
 *      File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlGetS1apIE
(
SztS1AP_PDU *pdu,
U32          id,
TknU8      **ie
)
#else
PUBLIC S16 vbMmeUtlGetS1apIE (pdu, id, ie)
SztS1AP_PDU *pdu;
U32          id;
TknU8      **ie;
#endif
{
   TknU8   *currStr;    /* Start definition */
   TknU8   *startStr;   /* Start event structure */
   U8      choiceType;
   U16     numComp;
   U16     recIdx;
   U32     recId;
   U32     procCode;
   U16     size;

   VB_TRC2(vbMmeUtlGetS1apIE)

   /* Initialize the return pointer Tkn to NOTPRSNT */
   *ie = NULLP;
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,  "S1ap IE Id(%d)",id));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,  "S1ap IE Id(%ld)",id));
#endif


   numComp = 0;
   recIdx = 0;

   if(pdu->choice.pres != PRSNT_NODEF)
   {
      RETVALUE(ROK);
   }

   choiceType = pdu->choice.val;

   /* Make start string point to the message in the PDU   */
   switch(choiceType)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         if (pdu->val.initiatingMsg.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.initiatingMsg.value.u));
         procCode  = pdu->val.initiatingMsg.procedureCode.val;
         break;
      }
      case S1AP_PDU_SUCCESSFULOUTCOME:
      {
         if (pdu->val.successfulOutcome.pres.pres != PRSNT_NODEF)
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.successfulOutcome.value.u));
         procCode  = pdu->val.successfulOutcome.procedureCode.val;
         break;
      }
      default:
      {
         if ((pdu->val.unsuccessfulOutcome.pres.pres != PRSNT_NODEF) ||
            (choiceType != S1AP_PDU_UNSUCCESSFULOUTCOME))
            RETVALUE(ROK);

         startStr  = (TknU8 *)(&(pdu->val.unsuccessfulOutcome.value.u));
         procCode  = pdu->val.unsuccessfulOutcome.procedureCode.val;
         break;
      }
   }

   if (((TknU8 *)startStr)->pres == PRSNT_NODEF)
   {
      /* Increment it by TknU8 for TknPres */
      currStr  = (TknU8 *) ((PTR)startStr + sizeof(TknU8));
   }
   else
   {
      RETVALUE(ROK);
   }

   if (((TknU16 *)(currStr))->pres == PRSNT_NODEF)
   {
      /* get the Number of IEs received : noComp */
      numComp = ((TknU16 *)(currStr))->val;
   }

   /*  Increment currStr by TknU16 for skipping noComp */
   currStr = (TknU8 *)((PTR)currStr + sizeof(TknU16));

   currStr = *(TknU8**)currStr;

   while (recIdx < numComp)
   {
      startStr = currStr;

      if (((TknPres *)currStr)->pres == PRSNT_NODEF)
      {
         /* Increment it by TknU8 for TknPres */
         currStr  = (TknU8 *) ((PTR)currStr + sizeof(TknPres));

         /* Get the received Id  */
         recId  = ((SztProtIE_ID *)currStr)->val;

         /* Compare the two Ids */
         if (recId == id)
         {
            startStr = (TknU8 *) ((PTR)currStr + sizeof(SztProtIE_ID) + sizeof(SztCriticality));

            *ie = startStr;
            break;
         }
      }

      size = vbMmeSzElmSize[choiceType][procCode];
      currStr  = (TknU8 *) ((PTR)startStr + size);
      recIdx++;
   }

   RETVALUE(ROK);
} /* end of vbMmeUtlGetS1apIE */

/*
 *
 *       Fun:   vbMmeUtlBldERABSetReq
 *
 *       Desc:  This function builds ERAB Setup Request PDU
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldERABSetReq
(
VbMmeEsmCb *esmCb,    /* ESM Cb */
SztNAS_PDU *sztNAS_PDU,     /* NASPdu */
S1apPdu    **eRABSetReqPduPtr /* ERAB Setup Request */
)
#else
PUBLIC S16 vbMmeUtlBldERABSetReq(esmCb, sztNAS_PDU, eRABSetReqPduPtr)
VbMmeEsmCb *esmCb;    /* ESM Cb */
SztNAS_PDU *sztNAS_PDU;     /* NASPdu */
S1apPdu    **eRABSetReqPduPtr; /* ERAB Setup Request */
#endif
{
   S16  ret;
   Mem  mem;
   U16  idx;
   U16  numComp;
   U8   gTPId[10];
   U8   tptAddr[16];
   U8   dlBitrate[12];
   U8   ulBitrate[12];
   U16  len;

   VbMmeUeCb     *ueCb = NULLP;
   SztProtIE_Field_E_RABSetupRqstIEs *ie;
   SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs *protIe;

   VbHssPdnSubCntxt pdnCtxt;
   S1apPdu *eRABSetupReqPdu;

   VB_TRC2(vbMmeUtlBldERABSetReq);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building E-RAB Setup Request"));

   *eRABSetReqPduPtr = NULLP;

   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   ueCb  = (VbMmeUeCb *)esmCb->ueCb;
   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&eRABSetupReqPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB061, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&(eRABSetupReqPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(eRABSetupReqPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(eRABSetupReqPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(eRABSetupReqPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_E_RABSetup);
   vbUtlFillTknU32(&(eRABSetupReqPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   vbUtlFillTknU8(&(eRABSetupReqPdu->pdu.val.initiatingMsg.value.u.
            sztE_RABSetupRqst.pres),PRSNT_NODEF);

   /* Allocate memory for 4 mandatory IEs */
   numComp = 3;
   vbUtlFillTknU16(&(eRABSetupReqPdu->pdu.val.initiatingMsg.value.u.
            sztE_RABSetupRqst.protocolIEs.noComp), numComp);

   if ((cmGetMem(eRABSetupReqPdu,
               (numComp * sizeof(SztProtIE_Field_E_RABSetupRqstIEs)),
               (Ptr *)&eRABSetupReqPdu->pdu.val.initiatingMsg.value.u.
               sztE_RABSetupRqst.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB062, ERRZERO,
            "cmGetMem failed");
      cmFreeMem((Ptr)eRABSetupReqPdu);
      RETVALUE(RFAILED);
   }

   /* protIe - Filling MME ID */
   ie = &eRABSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABSetupRqst.protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

   /* IE2 - Filling ENB ID */
   ie = &eRABSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABSetupRqst.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE4 - Filling E-RAB br setup list */
   ie = &eRABSetupReqPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABSetupRqst.protocolIEs.member[2];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_E_RABToBeSetupLstBrSUReq);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU16(&(ie->value.u.sztE_RABToBeSetupLstBrSUReq.noComp), 1);

   numComp = 1;

   if ((cmGetMem(eRABSetupReqPdu, numComp *
               sizeof(SztProtIE_SingleCont_E_RABToBeSetupItemBrSUReqIEs),
               (Ptr *)&ie->value.u.sztE_RABToBeSetupLstBrSUReq.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB065, ERRZERO,
            "cmGetMem Failed ");
      cmFreeMem((Ptr)eRABSetupReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE4 - Fill in setup List items */
   for(idx = 0; idx < numComp; idx++)
   {

      protIe = &ie->value.u.sztE_RABToBeSetupLstBrSUReq.member[idx];

      vbUtlFillTknU8(&(protIe->pres),PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->id),Sztid_E_RABToBeSetupItemBrSUReq );
      vbUtlFillTknU32(&(protIe->criticality), SztCriticalityrejectEnum);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.pres),
            PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.e_RAB_ID),
            esmCb->bId);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.qCI), esmCb->epsQos.qci /*ueCb->ueCtxt.ueHssCtxt.
            subCntxt[0].qos.qci*/);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.allocationRetentionPriority.pres),
            PRSNT_NODEF);

      /* Assign the priority as '1', the highest for now. */
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
            e_RABlevelQoSParams.allocationRetentionPriority.priorityLvl),\
            ueCb->ueCtxt.ueHssCtxt.subCntxt[0].qos.arp.priLevel);

      /* we may need to get this inform from esmCb in future if we have it.
         for now fill both the below values as Pre-emptable*/
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.allocationRetentionPriority.
               pre_emptionCapblty),
            SztPre_emptionCapbltymay_trigger_pre_emptionEnum);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.allocationRetentionPriority.
               pre_emptionVulnerability),
            SztPre_emptionVulnerabilitypre_emptableEnum);
      protIe->value.u.sztE_RABToBeSetupItemBrSUReq.e_RABlevelQoSParams.
         allocationRetentionPriority.iE_Extns.noComp.pres = NOTPRSNT;

      pdnCtxt= ueCb->ueCtxt.ueHssCtxt.subCntxt[0];

      dlBitrate[0] = (pdnCtxt.qos.maxBitRateDL >> 24 ) & 0xff;
      dlBitrate[1] = (pdnCtxt.qos.maxBitRateDL >> 16 ) & 0xff;
      dlBitrate[2] = (pdnCtxt.qos.maxBitRateDL >> 8 ) & 0xff;
      dlBitrate[3] = (pdnCtxt.qos.maxBitRateDL & 0xff);
      len = 4;

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.gbrQosInform.pres), PRSNT_NODEF);

      ret = vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateDL),
            len, dlBitrate, &eRABSetupReqPdu);
      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB063, ERRZERO,
               "Filling UE Qos Max Bit Rate DL Failed");
         cmFreeMem((Ptr)eRABSetupReqPdu);
         RETVALUE(RFAILED);
      }

      ulBitrate[0] = (pdnCtxt.qos.maxBitRateUL >> 24 ) & 0xff;
      ulBitrate[1] = (pdnCtxt.qos.maxBitRateUL >> 16 ) & 0xff;
      ulBitrate[2] = (pdnCtxt.qos.maxBitRateUL >> 8 ) & 0xff;
      ulBitrate[3] = (pdnCtxt.qos.maxBitRateUL & 0xff);
      ret = vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_MaxBitrateUL),
            len, ulBitrate, &eRABSetupReqPdu);
      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB063, ERRZERO,
               "Filling UE Qos Max Bit Rate UL Failed");
         cmFreeMem((Ptr)eRABSetupReqPdu);
         RETVALUE(RFAILED);
      }

      dlBitrate[0] = (pdnCtxt.qos.guaraBitRateDL >> 24 ) & 0xff;
      dlBitrate[1] = (pdnCtxt.qos.guaraBitRateDL >> 16 ) & 0xff;
      dlBitrate[2] = (pdnCtxt.qos.guaraBitRateDL >> 8 ) & 0xff;
      dlBitrate[3] = (pdnCtxt.qos.guaraBitRateDL & 0xff);
      ret = vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateDL),
            len, dlBitrate, &eRABSetupReqPdu);
      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB063, ERRZERO,
               "Filling UE Qos Guaranteed Bit Rate DL Failed");
         cmFreeMem((Ptr)eRABSetupReqPdu);
         RETVALUE(RFAILED);
      }

      ulBitrate[0] = (pdnCtxt.qos.guaraBitRateUL >> 24 ) & 0xff;
      ulBitrate[1] = (pdnCtxt.qos.guaraBitRateUL >> 16 ) & 0xff;
      ulBitrate[2] = (pdnCtxt.qos.guaraBitRateUL >> 8 ) & 0xff;
      ulBitrate[3] = (pdnCtxt.qos.guaraBitRateUL & 0xff);
      ret = vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               e_RABlevelQoSParams.gbrQosInform.e_RAB_GuaranteedBitrateUL),
            len, ulBitrate, &eRABSetupReqPdu);
      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB063, ERRZERO,
               "Filling UE Qos Guaranteed Bit Rate UL Failed");
         cmFreeMem((Ptr)eRABSetupReqPdu);
         RETVALUE(RFAILED);
      }

      protIe->value.u.sztE_RABToBeSetupItemBrSUReq.e_RABlevelQoSParams.
         iE_Extns.noComp.pres = NOTPRSNT;

      if (esmCb->s1SgwAddr.type == CM_INET_IPV4ADDR_TYPE)
      {
         len = 4;
         tptAddr[0] = (U8)((esmCb->s1SgwAddr.u.ipv4NetAddr >> 24) & 0xff);
         tptAddr[1] = (U8)((esmCb->s1SgwAddr.u.ipv4NetAddr >> 16) & 0xff);
         tptAddr[2] = (U8)((esmCb->s1SgwAddr.u.ipv4NetAddr >> 8) & 0xff);
         tptAddr[3] = (U8)(esmCb->s1SgwAddr.u.ipv4NetAddr & 0xff);
         ret = vbUtlFillTknStrBSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
                  transportLyrAddr),
               len, tptAddr, &eRABSetupReqPdu);
         if(ROK != ret)
         {
            VB_MME_LOGERROR(ERRCLS_DEBUG, EVB066, ERRZERO,
                  "Filling E-RAB Setup S-GW Transport Address Failed");
            cmFreeMem((Ptr)eRABSetupReqPdu);
            RETVALUE(RFAILED);
         }
      }
#ifdef IPV6_SUPPORTED
      else if (esmCb->s1SgwAddr.type == CM_INET_IPV6ADDR_TYPE)
      {
         for (len = 0; len < 16; len++)
         {
            tptAddr[len] = esmCb->s1SgwAddr.u.ipv6NetAddr[len];
         }
         ret = vbUtlFillTknStrBSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
                  transportLyrAddr),
               len, tptAddr, &eRABSetupReqPdu);
         if(ROK != ret)
         {
            VB_MME_LOGERROR(ERRCLS_DEBUG, EVB067, ERRZERO,
                  "Filling E-RAB Setup S-GW Transport Address Failed");
            cmFreeMem((Ptr)eRABSetupReqPdu);
            RETVALUE(RFAILED);
         }
      }
#endif /* IPV6_SUPPORTED */

      len = 4;
      gTPId[0] = (U8)((esmCb->s1SgwTeId >> 24) & 0xff);
      gTPId[1] = (U8)(esmCb->s1SgwTeId >> 16) & 0xff;
      gTPId[2] = (U8)(esmCb->s1SgwTeId >> 8) & 0xff;
      gTPId[3] = (U8)(esmCb->s1SgwTeId & 0xff);
      ret = vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.
               gTP_TEID),
            len, gTPId, &eRABSetupReqPdu);
      if(ROK != ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB068, ERRZERO,
               "Filling E-RAB Setup S-GW GTP TE ID Failed");
         cmFreeMem((Ptr)eRABSetupReqPdu);
         RETVALUE(RFAILED);
      }
      /* fill the attach acc nas pdu here */
      protIe->value.u.sztE_RABToBeSetupItemBrSUReq.nAS_PDU.pres = PRSNT_NODEF;
      ret = vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeSetupItemBrSUReq.nAS_PDU),
            sztNAS_PDU->len,sztNAS_PDU->val, &eRABSetupReqPdu);
      if(ROK !=ret)
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVBXXX, ERRZERO,
               "Filling NAS Pdu Failed");
         cmFreeMem((Ptr)eRABSetupReqPdu);
         RETVALUE(RFAILED);
      }
      /*done with adding nas PDU */

   }



   *eRABSetReqPduPtr = eRABSetupReqPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldERABSetReq */

/*
 *
 *       Fun:   vbMmeUtlBldERABModReq
 *
 *       Desc:  This function builds ERAB Modify Req PDU
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldERABModReq
(
VbMmeEsmCb *esmCb,    /* ESM Cb */
SztNAS_PDU *sztNAS_PDU,     /* NASPdu */
S1apPdu    **eRABModReqPduPtr /* ERAB Modify Req */
)
#else
PUBLIC S16 vbMmeUtlBldERABModReq(esmCb, sztNAS_PDU, eRABModReqPduPtr)
VbMmeEsmCb *esmCb;    /* ESM Cb */
SztNAS_PDU *sztNAS_PDU;     /* NASPdu */
S1apPdu    **eRABModReqPduPtr; /* ERAB Modify Req */
#endif
{
   S16  ret;
   Mem  mem;
   U16  idx;
   U16  numComp;

   VbMmeUeCb     *ueCb = NULLP;
   SztProtIE_Field_E_RABMdfyRqstIEs *ie;
   SztProtIE_SingleCont_E_RABToBeMdfdItemBrModReqIEs *protIe;
   S1apPdu *eRABModReqPdu;

   VB_TRC2(vbMmeUtlBldERABModReq);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building E-RAB Modify Req"));

   *eRABModReqPduPtr = NULLP;

   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   ueCb  = (VbMmeUeCb *)esmCb->ueCb;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&eRABModReqPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB061, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&(eRABModReqPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(eRABModReqPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(eRABModReqPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(eRABModReqPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_E_RABMdfy);
   vbUtlFillTknU32(&(eRABModReqPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   vbUtlFillTknU8(&(eRABModReqPdu->pdu.val.initiatingMsg.value.u.
            sztE_RABMdfyRqst.pres),PRSNT_NODEF);

   /* Allocate memory for 3 mandatory IEs */
   numComp = 3;
   vbUtlFillTknU16(&(eRABModReqPdu->pdu.val.initiatingMsg.value.u.
            sztE_RABMdfyRqst.protocolIEs.noComp), numComp);

   if ((cmGetMem(eRABModReqPdu,
               (numComp * sizeof(SztProtIE_Field_E_RABMdfyRqstIEs)),
               (Ptr *)&eRABModReqPdu->pdu.val.initiatingMsg.value.u.
               sztE_RABMdfyRqst.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB062, ERRZERO,
            "cmGetMem failed");
      cmFreeMem((Ptr)eRABModReqPdu);
      RETVALUE(RFAILED);
   }

   /* protIe - Filling UE MME ID */
   ie = &eRABModReqPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABMdfyRqst.protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

   /* IE2 - Filling UE ENB ID */
   ie = &eRABModReqPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABMdfyRqst.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE4 - Filling E-RAB To be Modified list */
   ie = &eRABModReqPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABMdfyRqst.protocolIEs.member[2];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_E_RABToBeMdfdLstBrModReq);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU16(&(ie->value.u.sztE_RABToBeMdfdLstBrModReq.noComp), 1);

   numComp = 1;

   if ((cmGetMem(eRABModReqPdu, numComp *
               sizeof(SztProtIE_SingleCont_E_RABToBeMdfdItemBrModReqIEs),
               (Ptr *)&ie->value.u.sztE_RABToBeMdfdLstBrModReq.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB065, ERRZERO,
            "cmGetMem Failed ");
      cmFreeMem((Ptr)eRABModReqPdu);
      RETVALUE(RFAILED);
   }

   /* IE4 - Fill in To be Modified List items */
   for(idx = 0; idx < numComp; idx++)
   {

      protIe = &ie->value.u.sztE_RABToBeMdfdLstBrModReq.member[idx];

      vbUtlFillTknU8(&(protIe->pres),PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->id),Sztid_E_RABToBeMdfdItemBrModReq);
      vbUtlFillTknU32(&(protIe->criticality), SztCriticalityrejectEnum);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.pres),
            PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RAB_ID),
            esmCb->bId);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RABLvlQoSParams.pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RABLvlQoSParams.qCI), esmCb->epsQos.qci);


      /* Allocation Retention Priority */
      vbUtlFillTknU8(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RABLvlQoSParams.
                     allocationRetentionPriority.pres), PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RABLvlQoSParams.
                     allocationRetentionPriority.priorityLvl), 1);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RABLvlQoSParams.
                     allocationRetentionPriority.pre_emptionCapblty), 1);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.e_RABLvlQoSParams.
                     allocationRetentionPriority.pre_emptionVulnerability), 1);

      /* GbrQos is optional */
      /* Fillin NAS- PDU*/
      vbUtlFillTknStrOSXL(&(protIe->value.u.sztE_RABToBeMdfdItemBrModReq.nAS_PDU), sztNAS_PDU->len,
                         sztNAS_PDU->val, &eRABModReqPdu);
   }

   *eRABModReqPduPtr = eRABModReqPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldERABModReq */

/*
 *
 *       Fun:   vbMmeUtlBldERABRlsCmd
 *
 *       Desc:  This function builds ERAB Release Command PDU
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldERABRlsCmd
(
VbMmeEsmCb *esmCb,    /* ESM Cb */
SztNAS_PDU *sztNAS_PDU,     /* NASPdu */
S1apPdu    **eRABRlsCmdPduPtr /* ERAB Release Command */
)
#else
PUBLIC S16 vbMmeUtlBldERABRlsCmd(esmCb, sztNAS_PDU, eRABRlsCmdPduPtr)
VbMmeEsmCb *esmCb;    /* ESM Cb */
SztNAS_PDU *sztNAS_PDU;     /* NASPdu */
S1apPdu    **eRABRlsCmdPduPtr; /* ERAB Release Command */
#endif
{
   S16  ret;
   Mem  mem;
   U16  idx;
   U16  numComp;

   VbMmeUeCb     *ueCb = NULLP;
   SztProtIE_Field_E_RABRlsCmmdIEs *ie;
   SztProtIE_SingleCont_E_RABItemIEs *protIe;

   S1apPdu *eRABRlsCmdPdu;

   VB_TRC2(vbMmeUtlBldERABRlsCmd);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building E-RAB Release Command"));

   *eRABRlsCmdPduPtr = NULLP;

   mem.region = vbMmeCb.init.region;
   mem.pool   = vbMmeCb.init.pool;

   ueCb  = (VbMmeUeCb *)esmCb->ueCb;

   /* Allocate memory for pdu */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem,
         (Ptr *)&eRABRlsCmdPdu);

   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB061, ERRZERO,
            "cmAllocEvnt Failed");
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&(eRABRlsCmdPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(eRABRlsCmdPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   vbUtlFillTknU8(&(eRABRlsCmdPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(eRABRlsCmdPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_E_RABRls);
   vbUtlFillTknU32(&(eRABRlsCmdPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityrejectEnum);

   vbUtlFillTknU8(&(eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
            sztE_RABRlsCmmd.pres),PRSNT_NODEF);

   /* Allocate memory for 4 mandatory IEs */
   numComp = 4;
   vbUtlFillTknU16(&(eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
            sztE_RABRlsCmmd.protocolIEs.noComp), numComp);

   if ((cmGetMem(eRABRlsCmdPdu,
               (numComp * sizeof(SztProtIE_Field_E_RABRlsCmmdIEs)),
               (Ptr *)&eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
               sztE_RABRlsCmmd.protocolIEs.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB062, ERRZERO,
            "cmGetMem failed");
      cmFreeMem((Ptr)eRABRlsCmdPdu);
      RETVALUE(RFAILED);
   }

   /* protIe - Filling UE MME ID */
   ie = &eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABRlsCmmd.protocolIEs.member[0];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

   /* IE2 - Filling UE ENB ID */
   ie = &eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABRlsCmmd.protocolIEs.member[1];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

   /* IE4 - Filling E-RAB br Release list */
   ie = &eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABRlsCmmd.protocolIEs.member[2];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_E_RABToBeRlsdLst);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   vbUtlFillTknU16(&(ie->value.u.sztE_RABLst.noComp), 1);

   numComp = 1;

   if ((cmGetMem(eRABRlsCmdPdu, numComp *
               sizeof(SztProtIE_SingleCont_E_RABItemIEs),
               (Ptr *)&ie->value.u.sztE_RABLst.member)) !=ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB065, ERRZERO,
            "cmGetMem Failed ");
      cmFreeMem((Ptr)eRABRlsCmdPdu);
      RETVALUE(RFAILED);
   }

   /* IE4 - Fill in Release List items */
   for(idx = 0; idx < numComp; idx++)
   {

      protIe = &ie->value.u.sztE_RABLst.member[idx];

      vbUtlFillTknU8(&(protIe->pres),PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->id),Sztid_E_RABItem);
      vbUtlFillTknU32(&(protIe->criticality), SztCriticalityrejectEnum);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABItem.pres),
            PRSNT_NODEF);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABItem.e_RAB_ID),
            esmCb->bId);

      vbUtlFillTknU8(&(protIe->value.u.sztE_RABItem.cause.choice),0);
      vbUtlFillTknU32(&(protIe->value.u.sztE_RABItem.cause.val.radioNw),0);
   }

   /* IE4 - Filling NAS - PDU */
   ie = &eRABRlsCmdPdu->pdu.val.initiatingMsg.value.u.
      sztE_RABRlsCmmd.protocolIEs.member[3];

   vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(ie->id), Sztid_NAS_PDU);
   vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   ret = vbUtlFillTknStrOSXL(&(ie->value.u.sztNAS_PDU), sztNAS_PDU->len,
          sztNAS_PDU->val, &eRABRlsCmdPdu);

   if(ROK !=ret)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVBXXX, ERRZERO,
            "Filling NAS Pdu Failed");
      cmFreeMem((Ptr)eRABRlsCmdPdu);
      RETVALUE(RFAILED);
   }

   *eRABRlsCmdPduPtr = eRABRlsCmdPdu;

   RETVALUE(ROK);
} /* vbMmeUtlBldERABRlsCmd */

/*
 *
 *       Fun:   vbMmeUtlBldDetachReq
 *
 *       Desc:  This function builds an EMM Detach Request message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldDetachReq
(
VbMmeEmmCb  *emmCb,
CmNasEvnt  **evnt,
U8         detachType,
U16        emmCause
)
#else
PUBLIC S16 vbMmeUtlBldDetachReq (emmCb, evnt, detachType, emmCause)
VbMmeUeCb  *emmCb;
CmNasEvnt  **evnt;
U8         detachType;
U16        emmCause;
#endif
{
   CmEmmDetachReq* detachReq;
   CmEmmMsg* emmMsg;

   VB_TRC2(vbMmeUtlBldDetachReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Detach Request"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_EMM_PD);
   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB075, ERRZERO,
            "Event Allocation Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEmmMsg), (Ptr *)&emmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB075, ERRZERO,
            "cmGetMem Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.emmEvnt = emmMsg;
   detachReq = &((*evnt)->m.emmEvnt->u.dtchReq);

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif
   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   emmMsg->secHdrType = CM_EMM_SEC_HDR_TYPE_PLAIN_NAS_MSG;
   emmMsg->msgId = CM_EMM_MSG_DETACH_REQ;
   emmMsg->protDisc = CM_EMM_PD;

   /* Fill mandatory IEs */
   /*detach type IE*/
   detachReq->detchType.pres = TRUE;
   detachReq->detchType.type = detachType;

   /* Fill the detach req cause */
   detachReq->cause.cause = emmCause;
   detachReq->cause.pres = 1;

   RETVALUE(ROK);
} /* end of vbMmeUtlBldDetachReq */


/*
 *
 *       Fun:   vbMmeUtlBldEsmStatus
 *
 *       Desc:  This function builds an ESM Status message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEsmStatus
(
VbMmeUeCb  *ueCb,   /* UE Cb */
CmNasEvnt  **evnt,    /* Attach Reject pdu to be built */
U8         pti,
U8         brId,
U8         esmCause/* ESM Cause value */
)
#else
PUBLIC S16 vbMmeUtlBldEsmStatus (ueCb, evnt, pti, brId, esmCause)
VbMmeUeCb   *ueCb;   /* UE Cb */
CmNasEvnt   **evnt;  /* pdu to built */
U8          pti;
U8          brId;
U8          esmCause; /* ESM Cause */
#endif
{
   CmEsmMsg           *esmMsg    = NULLP;

   VB_TRC2(vbMmeUtlBldEsmStatus);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Esm Status Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt  Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&esmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = esmMsg;

#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   esmMsg->msgType = CM_ESM_MSG_ESM_STATUS;
   esmMsg->prTxnId = pti;
   esmMsg->bearerId = brId;

/* vb004.101 : 116186: added protocol descriptor type */
   esmMsg->protDisc = CM_ESM_PD;

   /* Fill mandatory IEs */
   /* Fill the attach reject cause */
   (*evnt)->m.esmEvnt->u.esmSts.esmCause.val = esmCause;
   /* vb004.101: Missing pres value added. */
   (*evnt)->m.esmEvnt->u.esmSts.esmCause.pres = 1;

   RETVALUE(ROK);

} /* vbMmeUtlBldEsmStatus */

/*
 *
 *       Fun:   vbMmeUtlBldEsmInfoReq
 *
 *       Desc:  This function builds an ESM Info Req message
 *              structure.
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEsmInfoReq
(
VbMmeEsmCb  *esmCb,    /* ESM Cb */
CmNasEvnt   **evnt     /* ESM Info Req pdu to be built */
)
#else
PUBLIC S16 vbMmeUtlBldEsmInfoReq (esmCb, evnt)
VbMmeEsmCb   *esmCb;   /* ESM Cb */
CmNasEvnt    **evnt;   /* pdu to built */
#endif
{
   CmEsmMsg           *esmMsg    = NULLP;

   VB_TRC2(vbMmeUtlBldEsmInfoReq);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Esm Info Req Message"));

   /* Allocate memory for pdu */
   CM_ALLOC_NASEVNT (evnt, CM_ESM_PD);

   if(*evnt == NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB077, ERRZERO,
            "cmAllocEvnt  Failed");
      RETVALUE(RFAILED);
   }

   if (cmGetMem(&((*evnt)->memCp), sizeof(CmEsmMsg), (Ptr *)&esmMsg) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB078, ERRZERO,
            "cmGetMem  Failed");
      CM_FREE_NASEVNT(evnt);
      RETVALUE(RFAILED);
   }

   (*evnt)->m.esmEvnt = esmMsg;
#ifdef VB_MME_NAS_SEC
   (*evnt)->secHT = CM_NAS_SEC_HDR_TYPE_INT_PRTD_ENC;
#endif

   /* Fill Header information */
   /* Security header type is "Plain NAS message, not security protected" */
   esmMsg->msgType = CM_ESM_MSG_ESM_INFO_REQ;
   esmMsg->prTxnId = esmCb->tId;
   esmMsg->bearerId = esmCb->bId;
   esmMsg->protDisc = CM_ESM_PD;

   RETVALUE(ROK);

} /* vbMmeUtlBldEsmInfoReq */

/*
 *
 *       Fun:   vbUtlEgtAddUli
 *
 *       Desc:  This function builds the ULI to be encoded into gtp-c msg
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbUtlEgtAddUli
(
EgMsg            *egMsg,
EgIeLevelInfo    *egLvlInfo,
U8               inst,
VbUeTai          *tai,
VbEutranCgi      *eCgi
)
#else
PUBLIC S16 vbUtlEgtAddUli(egMsg, egLvlInfo, inst, tai, eCgi)
EgMsg            *egMsg;
EgIeLevelInfo    *egLvlInfo;
U8               inst;
VbUeTai          *tai,
VbEutranCgi      *eCgi
#endif
{
   EgIe      egIe;

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe, EG_ULI_IETYPE, inst, 0, EG_ULI_DATA);
#else
   egUtlEgtBuildEgIeHeader(&egIe, EG_ULI_IETYPE, inst, EG_ULI_DATA);
#endif /* EG_REL_930 */

   /* copy the tai */
   egIe.t.uli.isTAIPres          = TRUE;
   egIe.t.uli.tai.tac            = tai->tac;
   egIe.t.uli.tai.sNwk.mccDigit1 = (U8)(tai->plmnId.plmnId[0] & 0x0F);
   egIe.t.uli.tai.sNwk.mccDigit2 = (U8)(tai->plmnId.plmnId[0] & 0xF0) >> 4;
   egIe.t.uli.tai.sNwk.mccDigit3 = (U8)(tai->plmnId.plmnId[1] & 0x0F);

   egIe.t.uli.tai.sNwk.mncDigit1 = (U8)(tai->plmnId.plmnId[2] & 0x0F);
   egIe.t.uli.tai.sNwk.mncDigit2 = (U8)(tai->plmnId.plmnId[2] & 0xF0) >> 4;
   egIe.t.uli.tai.sNwk.mncDigit3 = (U8)(tai->plmnId.plmnId[1] & 0xF0) >> 4;

   /* copy ecgi */
   egIe.t.uli.isECGIPres            = TRUE;
   egIe.t.uli.ecgi.ecId             = eCgi->cellId;
   egIe.t.uli.ecgi.sNwk.mccDigit1   = (U8)(eCgi->plmnId.plmnId[0] & 0x0F);
   egIe.t.uli.ecgi.sNwk.mccDigit2   = (U8)(eCgi->plmnId.plmnId[0] & 0xF0) >> 4;
   egIe.t.uli.ecgi.sNwk.mccDigit3   = (U8)(eCgi->plmnId.plmnId[1] & 0x0F);
   egIe.t.uli.ecgi.sNwk.mncDigit1 = (U8)(tai->plmnId.plmnId[2] & 0x0F);
   egIe.t.uli.ecgi.sNwk.mncDigit2 = (U8)(eCgi->plmnId.plmnId[2] & 0xF0) >> 4;
   egIe.t.uli.ecgi.sNwk.mncDigit3 = (U8)(eCgi->plmnId.plmnId[1] & 0xF0) >> 4;

   if (EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/*
 *
 *       Fun:   vbUtlGetS1APTaId
 *
 *       Desc:  Fetches the TAID from S1AP msg
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbUtlGetS1APTaId
(
SztS1AP_PDU       *pS1apPdu,
U8                msgType,
VbUeTai             *tai
)
#else
PUBLIC S16 vbUtlGetS1APTaId(pS1apPdu, msgType, tai)
SztS1AP_PDU       *pS1apPdu,
U8                msgType,
VbUeTai           *tai
#endif
{
   SztTAI         *sztTai = NULLP;

   if (Sztid_initialUEMsg == msgType || Sztid_uplinkNASTport == msgType || \
       Sztid_HovrNotification == msgType)
   {
      vbMmeUtlGetS1apIE(pS1apPdu, Sztid_TAI, (TknU8 **)&sztTai);
      if (NULLP != sztTai)
      {
         /* + Convert plmn from S1AP to NAS format + */
         tai->plmnId.plmnId[0] = sztTai->pLMNidentity.val[0];
         if(0xF0 != (sztTai->pLMNidentity.val[1] & 0xF0))
         {
            tai->plmnId.plmnId[1] = \
                  (sztTai->pLMNidentity.val[2] & 0xF0) | \
                  (sztTai->pLMNidentity.val[1] & 0x0F);
            tai->plmnId.plmnId[2] = \
                  ((sztTai->pLMNidentity.val[1] & 0xF0) >> 4) | \
                  ((sztTai->pLMNidentity.val[2] & 0x0F) << 4);
         }
         else
         {
            tai->plmnId.plmnId[1] = sztTai->pLMNidentity.val[1];
            tai->plmnId.plmnId[2] = sztTai->pLMNidentity.val[2];
         }

         /* - Convert plmn from S1AP to NAS format - */
         VB_MME_GET_U16(tai->tac, sztTai->tAC.val);
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}

/*
 *
 *       Fun:   vbUtlGetS1APeCgi
 *
 *       Desc:  Fetches the Eutran CGI from S1AP msg
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbUtlGetS1APeCgi
(
SztS1AP_PDU       *pS1apPdu,
U8                msgType,
VbEutranCgi       *eCgi
)
#else
PUBLIC S16 vbUtlGetS1APeCgi(pS1apPdu, msgType, eCgi)
SztS1AP_PDU       *pS1apPdu,
U8                msgType
VbEutranCgi       *eCgi
#endif
{
   SztEUTRAN_CGI     *sztEcgi = NULL;

   if ((Sztid_initialUEMsg == msgType) ||
       (Sztid_uplinkNASTport == msgType))
   {
      vbMmeUtlGetS1apIE(pS1apPdu, Sztid_EUTRAN_CGI, (TknU8 **)&sztEcgi);
      if (NULLP != eCgi)
      {
         cmMemcpy(eCgi->plmnId.plmnId, (U8 *)(sztEcgi->pLMNidentity.val),
                  sztEcgi->pLMNidentity.len);
         VB_MME_DEC_U32(eCgi->cellId, sztEcgi->cell_ID.val);
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}


/*
 *
 *       Fun:   vbMmeUtlMapGtpToNasCause
 *
 *       Desc:
 *
 *       Ret:   ROK        success
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlMapGtpToNasCause
(
U8   gtpCause,
U8   *nasCause
)
#else
PUBLIC S16 vbMmeUtlMapGtpToNasCause(gtpCause, nasCause)
U8   gtpCause;
U8   *nasCause;
#endif
{
   VB_TRC2(vbMmeUtlMapGtpToNasCause)

   switch(gtpCause)
   {
      case EGT_GTPC_CAUSE_SYS_FAIL:
      {
         *nasCause = VB_ESM_NETWORK_FAILURE;
         break;
      }
      case EGT_GTPC_CAUSE_NO_RES_AVAIL:
      {
         *nasCause = VB_ESM_INSUF_RES;
         break;
      }
      default:
      {
         *nasCause = VB_ESM_NETWORK_FAILURE;
         break;
      }
   }

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 vbMmeInitCtxtRelProc
(
VbMmeUeCb  *ueCb,
U8         choice,
U8         cause,
U8         rabReq
)
#else
PUBLIC S16 vbMmeInitCtxtRelProc(ueCb, choice, cause, rabReq)
VbMmeUeCb  *ueCb;
U8         choice;
U8         cause;
U8         rabReq;
#endif
{
   S16  ret = ROK;

   VB_TRC2(vbMmeInitCtxtRelProc)

   /* Update proc code */
   ueCb->proc = VB_MME_CTXT_REL_PROC;

   /* If Rel Acc Bearer need to be send */
   if (1 == rabReq)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Release Access Bearer "\
            "Request to SGW"));
      if(ROK == vbMmeSndRABReq(ueCb))
      {
         /* Store casue for sending Ctxt Rel Cmd after receiving
          *  Rel Acc Br Rsp
          */
         ueCb->relCause.choice.val = choice;
         ueCb->relCause.val.nas.val = cause;
         RETVALUE(ROK);
      }

      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Release Access Bearer "\
            "Request to SGW"));
   }

   if (ueCb->ecmCb.state == VB_ECM_CONNECTED)
   {
      ret = vbMmeSndUeCtxtRelCmd(ueCb, choice, cause);
      if(ROK != ret)
      {
         /* Locally release the s1 connection */
         vbMmeRelLocS1Con(ueCb);
         VB_MME_END_EMM_PROC(ueCb);
      }
   }
   else
   {
      ueCb->proc = VB_MME_NULL_PROC;
   }

   RETVALUE(ret);
}

#ifdef VB_MME_NAS_SEC
/*
 *
 *       Fun:  vbMmeUtlGetNasAlg
 *
 *       Desc:
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
PUBLIC S16 vbMmeUtlGetNasAlg
(
VbMmeUeCb      *ueCb,
VbMmeEncAlg    *encAlg,
VbMmeIntAlg    *intAlg,
VbMmeAuthAlg   *authAlg
)
{
   /*
    AES-128 Algorithm:
       VB_MME_EEA2_128 : Ciphering
       VB_MME_EIA2_128 : Integrity
    SNOW 3G Algorithm:
       VB_MME_EEA1_128 : Ciphering
       VB_MME_EIA1_128 : Integrity
    NULL Algorithm:
       VB_MME_EEA0     : Ciphering
       VB_MME_EIA0: Integrity
   */

   /* Set algorithm preference here */
   VbMmeEncAlg encAlgPref = ueCb->ueCtxt.ueHssCtxt.ueSecInfo.ciphType;
   VbMmeIntAlg intAlgPref = ueCb->ueCtxt.ueHssCtxt.ueSecInfo.intType;
   *authAlg = ueCb->ueCtxt.ueHssCtxt.ueSecInfo.authType;

   /*
       Algorithm selection order
       1. Preference set to AES-128
          Order: AES-128, SNOW-3G, NULL
       2. Preference set to SNOW-3G
          Order: SNOW-3G, AES-128, NULL
       3. Preference set to NULL
          Order: NULL, AES-128, SNOW-3G
    */


   switch(encAlgPref)
   {
      case VB_MME_EEA0:
      {
         if(ueCb->ueCtxt.ueSecCap.eea0)
         {
            *encAlg = VB_MME_EEA0;
         }
         else if(ueCb->ueCtxt.ueSecCap.eea1_128)
         {
            *encAlg = VB_MME_EEA1_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eea2_128)
         {
            *encAlg = VB_MME_EEA2_128;
         }
         else
         {
            RETVALUE(RFAILED);
         }
         break;
      }
      case VB_MME_EEA1_128:
      {
         if(ueCb->ueCtxt.ueSecCap.eea1_128)
         {
            *encAlg = VB_MME_EEA1_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eea2_128)
         {
            *encAlg = VB_MME_EEA2_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eea0)
         {
            *encAlg = VB_MME_EEA0;
         }
         else
         {
            RETVALUE(RFAILED);
         }
         break;
      }
      case VB_MME_EEA2_128:
      {
         if(ueCb->ueCtxt.ueSecCap.eea2_128)
         {
            *encAlg = VB_MME_EEA2_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eea1_128)
         {
            *encAlg = VB_MME_EEA1_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eea0)
         {
            *encAlg = VB_MME_EEA0;
         }
         else
         {
            RETVALUE(RFAILED);
         }
         break;
      }
      default:
         RETVALUE(RFAILED);
   }

   switch(intAlgPref)
   {
      case VB_MME_EIA0:
      {
         if(ueCb->ueCtxt.ueSecCap.eia0)
         {
            *intAlg = VB_MME_EIA0;
         }
         else if(ueCb->ueCtxt.ueSecCap.eia1_128)
         {
            *intAlg = VB_MME_EIA1_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eia2_128)
         {
            *intAlg = VB_MME_EIA2_128;
         }
         else
         {
            RETVALUE(RFAILED);
         }
         break;
      }
      case VB_MME_EIA1_128:
      {
         if(ueCb->ueCtxt.ueSecCap.eia1_128)
         {
            *intAlg = VB_MME_EIA1_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eia2_128)
         {
            *intAlg = VB_MME_EIA2_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eia0)
         {
            *intAlg = VB_MME_EIA0;
         }
         else
         {
            RETVALUE(RFAILED);
         }
         break;
      }
      case VB_MME_EIA2_128:
      {
         if(ueCb->ueCtxt.ueSecCap.eia2_128)
         {
            *intAlg = VB_MME_EIA2_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eia1_128)
         {
            *intAlg = VB_MME_EIA1_128;
         }
         else if(ueCb->ueCtxt.ueSecCap.eia0)
         {
            *intAlg = VB_MME_EIA0;
         }
         else
         {
            RETVALUE(RFAILED);
         }
         break;
      }
      default:
         RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:  vbMmeUtlGetNasMsg
 *
 *       Desc:
 *
 *       Ret:   ROK        success
 *              RFAILED    failure
 *
 *       Notes:
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlGetNasMsg
(
CmNasEvnt *evnt,
U8        **val,
U16       *len
)
#else
PUBLIC S16 vbMmeUtlGetNasMsg
(
CmNasEvnt *evnt;
U8        **val;
U16       *len;
)
#endif
{
   SztNAS_PDU   *nasPdu = NULLP;
   switch(((S1apPdu*)evnt->pdu)->pdu.choice.val)
   {
      case S1AP_PDU_INITIATINGMSG:
      {
         switch(((S1apPdu*)evnt->pdu)->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Sztid_initialUEMsg:
            case Sztid_uplinkNASTport:
            {
               vbMmeUtlGetS1apIE(&(((S1apPdu*)evnt->pdu)->pdu), Sztid_NAS_PDU,
                            (TknU8 ** )&nasPdu);
               if(NULLP != nasPdu)
               {
                  *val = nasPdu->val;
                  *len = nasPdu->len;
                  RETVALUE(ROK);
               }
               break;
            }
            default:
            {
               break;
            }
         }
      }
      default:
      {
         break;
      }
   }

   RETVALUE(RFAILED);
} /* vbMmeUtlGetNasAlg */
#endif /* VB_MME_NAS_SEC */

#ifdef ANSI
PUBLIC S16 vbMmeUtlSpndEsmProc
(
VbMmeEsmCb    *esmCb,
CmNasEvnt     *evnt,
U8            msgType
)
#else
PUBLIC S16 vbMmeUtlSpndEsmProc(esmCb, evnt, msgType)
VbMmeEsmCb     *esmCb;
CmNasEvnt      *evnt;
U8             msgType;
#endif
{
   S16 indx  = 0;

   VB_TRC2(vbMmeUtlSpndEsmProc);

   for(indx = 0; indx < VB_MME_MAX_SPND_PROC; indx++)
   {
      if(VB_MME_SPND_NULL_EVNT == esmCb->ueCb->spndProc[indx].choice)
      {
         esmCb->ueCb->spndProc[indx].choice               = VB_MME_SPND_ESM_EVNT;
         esmCb->ueCb->spndProc[indx].proc.esmProc.esmCb   = esmCb;
         esmCb->ueCb->spndProc[indx].proc.esmProc.evnt    = evnt;
         esmCb->ueCb->spndProc[indx].proc.esmProc.msgType = msgType;
         RETVALUE(ROK);
      }
   }

   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unable To Add To Queue"));
   RETVALUE(RFAILED);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlSpndEmmProc
(
VbMmeUeCb    *ueCb,
CmNasEvnt     *evnt,
U8            msgType
)
#else
PUBLIC S16 vbMmeUtlSpndEmmProc(ueCb, evnt, msgType)
VbMmeEsmCb     *ueCb;
CmNasEvnt      *evnt;
U8             msgType;
#endif
{
   S16 indx  = 0;

   VB_TRC2(vbMmeUtlSpndEmmProc);

   for(indx = 0; indx < VB_MME_MAX_SPND_PROC; indx++)
   {
      if(VB_MME_SPND_NULL_EVNT == ueCb->spndProc[indx].choice)
      {
         ueCb->spndProc[indx].choice               = VB_MME_SPND_EMM_EVNT;
         ueCb->spndProc[indx].proc.emmProc.ueCb    = ueCb;
         ueCb->spndProc[indx].proc.emmProc.evnt    = evnt;
         ueCb->spndProc[indx].proc.emmProc.msgType = msgType;
         RETVALUE(ROK);
      }
   }

   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unable To Add To Queue"));
   RETVALUE(RFAILED);
}
#ifdef ANSI
PUBLIC S16 vbMmeUtlSpndS1apProc
(
VbMmeUeCb     *ueCb,
VbMmeS1apEvnt *s1apEvnt,
S1apPdu       *s1apPdu,
U8            msgType
)
#else
PUBLIC S16 vbMmeUtlSpndS1apProc(ueCb, s1apEvnt, s1apPdu, msgType)
VbMmeUeCb     *ueCb;
VbMmeS1apEvnt *s1apEvnt;
S1apPdu       *s1apPdu;
U8            msgType;
#endif
{
   S16 indx  = 0;

   VB_TRC2(vbMmeUtlSpndS1apProc);

   for(indx = 0; indx < VB_MME_MAX_SPND_PROC; indx++)
   {
      if(VB_MME_SPND_NULL_EVNT == ueCb->spndProc[indx].choice)
      {
         ueCb->spndProc[indx].choice                = VB_MME_SPND_S1AP_EVNT;
         ueCb->spndProc[indx].proc.s1apProc.ueCb    = ueCb;
         ueCb->spndProc[indx].proc.s1apProc.s1apPdu = s1apPdu;

         cmMemcpy((U8*)&ueCb->spndProc[indx].proc.s1apProc.s1apEvnt,
                  (U8*)s1apEvnt, sizeof(VbMmeS1apEvnt));
         ueCb->spndProc[indx].proc.s1apProc.msgType = msgType;
         RETVALUE(ROK);
      }
   }

   VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unable To Add To Queue"));
   RETVALUE(RFAILED);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlResumeProc
(
VbMmeUeCb    *ueCb
)
#else
PUBLIC S16 vbMmeUtlResumeProc(ueCb)
VbMmeUeCb    *ueCb;
#endif
{
   S16 indx  = 0;

   VB_TRC2(vbMmeUtlResumeProc);

   for(indx = 0; indx < VB_MME_MAX_SPND_PROC; indx++)
   {
      if(VB_MME_SPND_ESM_EVNT == ueCb->spndProc[indx].choice)
      {
         ueCb->spndProc[indx].choice = VB_MME_SPND_NULL_EVNT;

         vbMmeRcvEsmMsg(ueCb->spndProc[indx].proc.esmProc.esmCb,
                        ueCb->spndProc[indx].proc.esmProc.evnt,
                        ueCb->spndProc[indx].proc.esmProc.msgType);

         ueCb->spndProc[indx].proc.esmProc.esmCb   = NULLP;
         ueCb->spndProc[indx].proc.esmProc.evnt    = NULLP;
         ueCb->spndProc[indx].proc.esmProc.msgType = 0;
         RETVALUE(ROK);
      }
      if(VB_MME_SPND_EMM_EVNT == ueCb->spndProc[indx].choice)
      {
         ueCb->spndProc[indx].choice = VB_MME_SPND_NULL_EVNT;

         vbMmeRcvEmmMsg(ueCb->spndProc[indx].proc.emmProc.ueCb,
                        ueCb->spndProc[indx].proc.emmProc.evnt,
                        ueCb->spndProc[indx].proc.emmProc.msgType);

         ueCb->spndProc[indx].proc.emmProc.ueCb   = NULLP;
         ueCb->spndProc[indx].proc.emmProc.evnt    = NULLP;
         ueCb->spndProc[indx].proc.emmProc.msgType = 0;
         RETVALUE(ROK);
      }
      else if(VB_MME_SPND_S1AP_EVNT == ueCb->spndProc[indx].choice)
      {
         ueCb->spndProc[indx].choice = VB_MME_SPND_NULL_EVNT;
         vbMmeRcvS1apMsg(ueCb->suConnId,
               (Ptr*)&ueCb->spndProc[indx].proc.s1apProc.s1apEvnt.evnt.pdu,
               ueCb->spndProc[indx].proc.s1apProc.msgType);
         ueCb->spndProc[indx].proc.s1apProc.msgType = 0;
         cmMemset((U8*)&ueCb->spndProc[indx].proc.s1apProc.s1apEvnt,
                   0, sizeof(VbMmeS1apEvnt));
         VB_MME_FREE_S1AP_PDU(ueCb->spndProc[indx].proc.s1apProc.s1apPdu);
         ueCb->spndProc[indx].proc.s1apProc.s1apPdu = NULLP;
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlAddPdnCb
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
VbMmePdnCb   **pdnCb
)
#else
PUBLIC S16 vbMmeUtlAddPdnCb(ueCb, esmCb, pdnCb)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
VbMmePdnCb   **pdnCb;
#endif
{
   S16 indx;
   VB_TRC2(vbMmeUtlAddPdnCb)

   /* Allocate enbCb */
   VB_MME_ALLOC(pdnCb, sizeof(VbMmePdnCb));
   if(NULLP == *pdnCb)
   {
      RETVALUE(RFAILED);
   }

   for(indx = 0; indx < VB_MME_MAX_PDN_CONN; indx++)
   {
      if(NULLP == ueCb->pdnList[indx])
      {
         ueCb->pdnList[indx] = *pdnCb;
         (*pdnCb)->id = indx + 1;
         break;
      }
   }

   if(VB_MME_MAX_PDN_CONN == indx)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Exceeded Max PDNs"));
      VB_MME_FREE(*pdnCb, sizeof(VbMmePdnCb));
      RETVALUE(RFAILED);
   }

   (*pdnCb)->esmCb = esmCb;
   esmCb->pdnCb    = *pdnCb;

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlDelPdnCb
(
VbMmeUeCb    *ueCb,
VbMmeEsmCb   *esmCb,
VbMmePdnCb   *pdnCb
)
#else
PUBLIC S16 vbMmeUtlDelPdnCb(ueCb, esmCb, pdnCb)
VbMmeUeCb    *ueCb;
VbMmeEsmCb   *esmCb;
VbMmePdnCb   *pdnCb;
#endif
{
   VB_TRC2(vbMmeUtlDelPdnCb)

   if(NULLP != pdnCb)
   {
      ueCb->pdnList[(pdnCb->id) - 1] = NULLP;
      VB_MME_FREE(pdnCb, sizeof(VbMmePdnCb));
   }

   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlGetEsmCbOnS11TranId
(
VbMmeUeCb      *ueCb,
VbMmeEsmCb     **esmCb,
TknU32         *transId
)
#else
PUBLIC S16 vbMmeUtlGetEsmCbOnS11TranId(ueCb, esmCb, transId)
VbMmeUeCb      *ueCb;
VbMmeEsmCb     **esmCb;
TknU32         *transId;
#endif
{
   U8 indx = 0;

   /* Replacing recevied transaction id with stored
      as work arround for GTP stack issue */
   cmMemcpy((U8*)transId, (U8*)&ueCb->transId, sizeof(TknU32));

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if(ueCb->esmList[indx] &&
        (ueCb->esmList[indx]->tranId.val == transId->val))
      {
         *esmCb = ueCb->esmList[indx];
         RETVALUE(ROK);
      }
   }

   for(indx = 0; indx < CM_ESM_MAX_BEARER_ID; indx++)
   {
      if((ueCb->esmTrnList.esmInfo[indx].status) &&
         (ueCb->esmTrnList.esmInfo[indx].esmCb) &&
         (ueCb->esmTrnList.esmInfo[indx].esmCb->tranId.val ==  transId->val))
      {
         *esmCb = ueCb->esmTrnList.esmInfo[indx].esmCb;
         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}

#ifdef ANSI
PUBLIC S16 vbMmeUtlIsPlmnSupported(VbMmeUeCb *ueCb)
#else
PUBLIC S16 vbMmeUtlIsPlmnSupported(ueCb)
VbMmeUeCb *ueCb;
#endif
{
   U8      indx = 0;

   for(indx = 0; indx < vbMmeCb.mmeCfg.srvGLst.noOfGummeis; indx++)
   {
      if (vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mcc[0] != \
         ueCb->ueCtxt.ueImsi[0])
      {
         continue;
      }

      if (vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mcc[1] != \
         ueCb->ueCtxt.ueImsi[1])
      {
         continue;
      }

      if (vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mcc[2] != \
         ueCb->ueCtxt.ueImsi[2])
      {
         continue;
      }

      if (vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mnc[0] != \
         ueCb->ueCtxt.ueImsi[3])
      {
         continue;
      }

      if (vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mnc[1] != \
         ueCb->ueCtxt.ueImsi[4])
      {
         continue;
      }

      if ((vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mnc[2] != 0x0F) &&
         (vbMmeCb.mmeCfg.srvGLst.gummeiLst[indx].nwId.mnc[2] != \
         ueCb->ueCtxt.ueImsi[5]))
      {
         continue;
      }

      RETVALUE(ROK);
   }

   RETVALUE(RFAILED);
}


/**************************************************************************//**
 *
 * @brief
 *    Finds eNodeB CB based on the Home eNB ID
 *
 * @param[in]  pEnbId
 *    Pointer to Home eNB ID
 * @param[out] pEnbCb
 *    Pointer to eNodeB CB
 *
 * @return
 *    SUCCESS if enode cb is found otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlGetTgtEnbCb
(
VbMmeGlbEnbId  *pEnbId,
VbMmeEnbCb     **pEnbCb
)
{
   PTR               prevEnt  = NULLP;
   VbMmeEnbCb        *pEnbHashEnt = NULLP;

   VB_TRC2(vbMmeUtlGetTgtEnbCb);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Finding Target eNB CB"));

   while (ROK == cmHashListGetNext(&vbMmeCb.eNbLst, prevEnt,\
         (PTR *)&pEnbHashEnt))
   {
      if ((cmMemcmp(pEnbHashEnt->enbStupInfo.gEnbId.enbId.val.homeENB_ID.val,\
         pEnbId->enbId.val.homeENB_ID.val, 4) == 0)/* && \
         (cmMemcmp(pEnbHashEnt->enbStupInfo.gEnbId.plmnId.plmnId,\
         pEnbId->plmnId.plmnId, VB_PLMN_ID_LEN) == 0)*/)
      {
         *pEnbCb = pEnbHashEnt;
         RETVALUE(ROK);
         break;
      }
      else
      {
         prevEnt = (PTR)(pEnbHashEnt);
      }
   }

   RETVALUE(RFAILED);
} /* vbMmeUtlGetTgtEnbCb */


/**************************************************************************//**
 *
 * @brief
 *    Copies HO Data from src UE cb to Dst UE cb
 *
 * @param[in]  pDstUeCb
 *    Pointer to UE Control block
 * @param[in]  pSrcUeCb
 *    Pointer to UE Control block
 *
 * @return
 *    SUCCESS if values are copied otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeUtlCopyUeCbData
(
VbMmeUeCb   *pTgtUeCb,
VbMmeUeCb   *pSrcUeCb
)
{
   S16         ret   = ROK;
   VbMmeEsmCb  *pNewEsmCb = NULLP;
   VbMmePdnCb  *pPdnCb = NULLP;

   VB_TRC2(vbMmeUtlCopyUeCbData);

   cmMemcpy(pTgtUeCb->ueSTmsiCbKey, pSrcUeCb->ueSTmsiCbKey, 5);

   pTgtUeCb->bitmask          = pSrcUeCb->bitmask;
   pTgtUeCb->ueCtxt           = pSrcUeCb->ueCtxt;
   pTgtUeCb->ecmCb            = pSrcUeCb->ecmCb;
   pTgtUeCb->ueTunnelCb       = pSrcUeCb->ueTunnelCb;
   pTgtUeCb->ueTunnelCb->ueCb = pTgtUeCb;
   pTgtUeCb->proc             = pSrcUeCb->proc;
   pTgtUeCb->transId          = pSrcUeCb->transId;

   if (NULLP != pSrcUeCb->esmList[0])
   {
      VB_MME_ALLOC(&pNewEsmCb, sizeof(VbMmeEsmCb));
      if (NULLP != pNewEsmCb)
      {
         cmMemcpy((U8 *)pNewEsmCb, (U8 *)pSrcUeCb->esmList[0],\
               sizeof(VbMmeEsmCb));
         pNewEsmCb->ueCb = pTgtUeCb;
         pTgtUeCb->esmList[0] = pNewEsmCb;
         cmInitTimers(pNewEsmCb->esmTimers, VB_ESM_MAX_TIMERS);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: VbMmeEsmCb"));
         RETVALUE(RFAILED);
      }
   }

	/* This check has been introducded to avoid klocwork warning */
	if (pNewEsmCb != NULLP)
	{
		ret = vbMmeUtlAddPdnCb(pTgtUeCb, pNewEsmCb, &pPdnCb);
	}

	/* This check has been introducded to avoid klocwork warning */
   if (ROK == ret && pNewEsmCb != NULLP)
   {
      cmMemcpy((U8 *)pPdnCb, (U8 *)pSrcUeCb->pdnList[0], sizeof(VbMmePdnCb));
      pPdnCb->esmCb = pNewEsmCb;
      pNewEsmCb->pdnCb = pPdnCb;
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Creating PDN Cb"));
      RETVALUE(RFAILED);
   }

   VB_MME_ALLOC(&pTgtUeCb->pHoData, sizeof(VbMmeHoData));
   if (NULLP == pTgtUeCb->pHoData)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: vbMmeHoData"));
      RETVALUE(RFAILED);
   }

   pTgtUeCb->pHoData->bitmask = pSrcUeCb->pHoData->bitmask;

   if (pSrcUeCb->pHoData->srcToTgtTCont.len != 0)
   {
      VB_MME_ALLOC(&pTgtUeCb->pHoData->srcToTgtTCont.pVal,\
            pSrcUeCb->pHoData->srcToTgtTCont.len);
      if (NULLP == pSrcUeCb->pHoData->srcToTgtTCont.pVal)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: srcToTgtTCont"));
         RETVALUE(RFAILED);
      }

      pTgtUeCb->pHoData->srcToTgtTCont.len = pSrcUeCb->pHoData->\
            srcToTgtTCont.len;
      cmMemcpy(pTgtUeCb->pHoData->srcToTgtTCont.pVal, pSrcUeCb->pHoData->\
            srcToTgtTCont.pVal, pTgtUeCb->pHoData->srcToTgtTCont.len);
   }

   pTgtUeCb->pHoData->oldsuConnId = pSrcUeCb->pHoData->oldsuConnId;
   pTgtUeCb->pHoData->oldspConnId = pSrcUeCb->pHoData->oldspConnId;
   pTgtUeCb->pHoData->hoTgt = pSrcUeCb->pHoData->hoTgt;
   pTgtUeCb->pHoData->hoMmeType = pSrcUeCb->pHoData->hoMmeType;
   pTgtUeCb->pHoData->mmeUeS1apId = pSrcUeCb->pHoData->mmeUeS1apId;
   pTgtUeCb->pHoData->ueAddr = pSrcUeCb->pHoData->ueAddr;
   pTgtUeCb->pHoData->dlTransportLyrAddr = pSrcUeCb->pHoData->\
         dlTransportLyrAddr;
   pTgtUeCb->pHoData->dlGtpTeid = pSrcUeCb->pHoData->dlGtpTeid;
   pTgtUeCb->pHoData->ulTransportLyrAddr = pSrcUeCb->pHoData->\
         ulTransportLyrAddr;
   pTgtUeCb->pHoData->ulGtpTeid = pSrcUeCb->pHoData->ulGtpTeid;

   RETVALUE(ret);
} /* vbMmeUtlCopyUeCbData */



/**************************************************************************//**
 *
 * @brief
 *    Frees memory allocated for vbMmeHoData
 *
 * @param[in]  pUeCb
 *    Pointer to UE Cb whose handover data has to be deleted
 *
 * @return
 *    No return values
 *
 *****************************************************************************/
PUBLIC Void vbMmeUtlDelHoData
(
VbMmeUeCb    *pUeCb
)
{
   VbMmeHoData       *pHoData = pUeCb->pHoData;

   VB_TRC2(vbMmeUtlDelHoData);

   if (NULLP == pHoData)
   {
      RETVOID;
   }

   if (NULLP != pHoData->srcToTgtTCont.pVal)
   {
      VB_MME_FREE(pHoData->srcToTgtTCont.pVal, pHoData->srcToTgtTCont.len);
      pHoData->srcToTgtTCont.pVal = NULLP;
      pHoData->srcToTgtTCont.len = 0;
   }

   if (NULLP != pHoData->tgtToSrcTCont.pVal)
   {
      VB_MME_FREE(pHoData->tgtToSrcTCont.pVal, pHoData->tgtToSrcTCont.len);
      pHoData->tgtToSrcTCont.pVal = NULLP;
      pHoData->tgtToSrcTCont.len = 0;
   }

   if (NULLP != pHoData->staTfrTCont.pVal)
   {
      VB_MME_FREE(pHoData->staTfrTCont.pVal, pHoData->staTfrTCont.len);
      pHoData->staTfrTCont.pVal = NULLP;
      pHoData->staTfrTCont.len = 0;
   }

   VB_MME_FREE(pHoData, sizeof(VbMmeHoData));
   pUeCb->pHoData = NULLP;

   RETVOID;
} /* vbMmeUtlDelHoData */


PUBLIC Void vbMmeUtlFndNDelUeCb
(
U8       *pImsi,
U32      imsiLen
)
{
   S16               ret    = ROK;
   VbMmeUeCb         *pUeCb = NULLP;

   VB_TRC2(vbMmeUtlFndNDelUeCb);

   ret = cmHashListFind(&vbMmeCb.ueImsiCbHlCp, pImsi, (U16)imsiLen, 0,\
         (PTR *)&pUeCb);
   if (ret == ROK)
   {
      vbMmeUtlDelUeCbOnImsi(pUeCb);
   }

   RETVOID;
}



/**************************************************************************//**
 *
 * @brief
 *    Deletes the Old UE Cb, after a successfull S1 Handover without
 *    MME relocation. Deallocated memory allocated for UE Cb,
 *    removes from Connection ID hash list
 *
 * @param[in]  pUeCb
 *    Pointer to UE CB
 *
 * @return
 *    Void
 *
 *****************************************************************************/
PUBLIC Void vbMmeUtlDelHOUeCb
(
VbMmeUeCb   *pUeCb
)
{
   VB_TRC2(vbMmeUtlDelHOUeCb);

   VB_MME_FREE(pUeCb->pdnList[0], sizeof(VbMmePdnCb));
   pUeCb->pdnList[0] = NULLP;
   VB_MME_FREE(pUeCb->esmList[0], sizeof(VbMmeEsmCb));
   pUeCb->esmList[0] = NULLP;
   pUeCb->esmTrnList.esmInfo[0].esmCb = NULLP;

   vbMmeUtlDelPndTrns(pUeCb);
   vbMmeUtlDelHoData(pUeCb);
   vbMmeUtlDelUeCbOnConnId(pUeCb);
   vbMmeUtlDelUeCbOnImsi(pUeCb);
   vbMmeUtlDelUeCbOnSTmsi(pUeCb);

   /* Stop Timer */
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_MOB_REACHABLE);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_3413);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_IMPLI_DTCH_TMR);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_INT_MGMT);

   /* Stop all EMM Timer as a precaution */
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_3422);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_3450);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_3460);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_EMM_3470);
   vbMmeStopTmr((PTR)pUeCb, VB_TMR_UE_CTX_DEL);

   /* Freeing UeCb */
   VB_MME_FREE(pUeCb, sizeof(VbMmeUeCb));

   RETVOID;
} /* vbMmeUtlDelHOUeCb */

#ifdef X2_HO
/* Added new function vbMmeUtlBldPathSwRqstAckg() */
/*
 * *
 * *       Fun:   vbMmeUtlBldPathSwRqstAckg
 * *
 * *       Desc:  Call handler to fill Path Switch Request Acknowledge message
 * *
 * *       Ret:   ROK
 * *
 * *       Notes: None
 * *
 * *       File:  vb_mme_utl.c
 * *
 * */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldPathSwRqstAckg
(
 VbMmeUeCb  *ueCb,
 S1apPdu    **pdu
 )
#else
PUBLIC S16 vbMmeUtlBldPathSwRqstAckg( ueCb, pdu)
 VbMmeUeCb  *ueCb;
 S1apPdu    **pdu;
#endif
{
   S1apPdu   *pathSwRqstAckgPdu = NULLP;
   S16        numComp;
   S16        ret;
   Mem        mem;
   U16        len;
   U8         secKeyVal[256];
   S16        idx;
   SztProtIE_Field_PathSwRqstAckgIEs *ie;

   VB_TRC2(vbMmeUtlBldPathSwRqstAckg)

   mem.region = 0;
   mem.pool   = 0;

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), VB_UTL_S1_SDU_SIZE, &mem, (Ptr *)&pathSwRqstAckgPdu);
   if(ret != ROK)
   {
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(pathSwRqstAckgPdu->pdu), 0, sizeof(S1apPdu) - sizeof(CmMemListCp));

   vbUtlFillTknU8(&(pathSwRqstAckgPdu->pdu.choice), S1AP_PDU_SUCCESSFULOUTCOME);
   vbUtlFillTknU8(&(pathSwRqstAckgPdu->pdu.val.successfulOutcome.pres), PRSNT_NODEF);
   vbUtlFillTknU32(&(pathSwRqstAckgPdu->pdu.val.successfulOutcome.procedureCode), Sztid_PathSwRqst);
   vbUtlFillTknU32(&(pathSwRqstAckgPdu->pdu.val.successfulOutcome.criticality), SztCriticalityrejectEnum);

   {
      numComp = 3;
      vbUtlFillTknU8(&(pathSwRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztPathSwRqstAckg.pres), PRSNT_NODEF);
      vbUtlFillTknU16(&(pathSwRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztPathSwRqstAckg.protocolIEs.noComp), numComp);

      if ((cmGetMem(pathSwRqstAckgPdu, (numComp * sizeof(SztProtIE_Field_PathSwRqstAckgIEs)),
                  (Ptr *)&pathSwRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztPathSwRqstAckg.protocolIEs.member)) !=ROK)
      {
         RETVALUE(RFAILED);
      }

      {

         ie = &pathSwRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztPathSwRqstAckg.protocolIEs.member[0];

         vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
         vbUtlFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
         vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
         vbUtlFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), ueCb->mMeUeS1apId);

         ie = &pathSwRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztPathSwRqstAckg.protocolIEs.member[1];

         vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
         vbUtlFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
         vbUtlFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
         vbUtlFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), ueCb->eNbUeS1apId);

         /* Currently we do not fill E-RAB to be switched in uplink list */
         ie = &pathSwRqstAckgPdu->pdu.val.successfulOutcome.value.u.sztPathSwRqstAckg.protocolIEs.member[2];

         vbUtlFillTknU8(&(ie->pres), PRSNT_NODEF);
         vbUtlFillTknU32(&(ie->id), Sztid_SecurCntxt);
         vbUtlFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

         vbUtlFillTknU8(&(ie->value.u.sztSecurCntxt.pres), PRSNT_NODEF);

         len = 3;


         vbUtlFillTknU32(&ie->value.u.sztSecurCntxt.nextHopChainingCount, ueCb->ueCtxt.secCtxt.nxtHopChainCnt);
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Genrating Next HopChainCnt(%d)\n", ueCb->ueCtxt.secCtxt.nxtHopChainCnt));
         vbUtlFillTknStrBSXL(&(ie->value.u.sztSecurCntxt.nextHopParameter), CM_EMM_MAX_KENB,\
               ueCb->ueCtxt.secCtxt.nxtHop, &pathSwRqstAckgPdu);
         ret = vbMmeGenerateNextHop(&ueCb->ueCtxt.secCtxt, ueCb->ueCtxt.secCtxt.nxtHop);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Genrating Next Hop Key"));
         }

         ie->value.u.sztSecurCntxt.iE_Extns.noComp.pres = NOTPRSNT;
      }
   }

   *pdu = pathSwRqstAckgPdu;

   RETVALUE(ROK);
}
#endif


/********************************************************************30**

         End of file:     vb_mme_utl.c@@/main/1 - Mon Jun 28 19:35:39 2010

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
/main/1      -          sp            1. LTE-CNE Initial Release.
/main/1  vb001.101    rk              1. Added new functions vbMmeUtlInitSTmsiUeCbs,
                                         vbMmeUtlFndUeCbSTmsi, vbMmeUtlBldEgtDwnLnkNotAck,
                                         vbMmeUtlBldEgtRelAccessBearerReq, vbMmeUtlBldS1PagingReq,
                                         vbMmeUtlBldEgtCrtBearRsp, vbMmeUtlBldActvDedBearerReq,
                                         vbMmeUtlBldEgtBearResCmd, vbMmeUtlBldERABSetReq
                                      2. Added ieRelease arg in vbMmeUtlBldMBReq
/main/1  vb002.101    rk              1. Dedicated Bearer Paging support changes
/main/1  vb003.101    chekar          1. Added Ue Rad Capability support.
/main/1  vb004.101    ve              1. Missing pres value added.
                      nj              2. added protocol descriptor type: EMM  while buiding attachReject
/main/1  vb005.101    rk              1. Added support for Network initiated Detach
                      pkd             2. UeId Index calculation ccpu00117480
                      pmacharla       1. upgraded to eGTP 2.1 under EG_REL_930 flag
/main/1 vb005.101     psb             1. TIC_ID:ccpu00117534 Fixed warning for gcc compilation on CentOS
/main/1  vb006.101   ragrawal         1. ccpu00118282  Added new local variable and code to support multiple
                                         UE entries by changing subCntxt[0] to subCntxt[ueEntry-1].
                                      2. CRID:ccpu00117391 ERAB RELEASE support.
                                      3. ccpu00118283 Added new functions vbMmeUtlInitImsiGutiMappings,vbMmeUtlPrintImsi,
                                         vbMmeUtlPrintGuti,vbMmeUtlAddImsiGutiMap,vbMmeUtlFndImsiGutiMapCb to support
                                         storing of mapping between IMSI and GUTI in a hash list.
                                      4. ccpu00119943 Added new function vbMmeUtlBldIdReq.
                                      5. Fix for compilation warning.
                                      6. Added new local variable and code to make ueCb NULL in
                                         IMSI GUTI Map CB, while deleting ueCbin vbMmeUtlDelUeCb().
                                      7. ccpu00118283 Add IMSI GUTI Mapping into hash list
                                      8. Added new function VbMmeUtlFndUeCbMmeS1apId(),
                                         vbMmeUtlBldIdReq()
                                         and vbMmeUtlBldMBReqForPthSwch().
                                      9. Test Code for OverLoading Feature.
                                     10. Calling new function vbUtlIpv4InetToArr_HO().
                                     11. ccpu00121202:MOD:IDCC,Decoding for algo corrected.
/main/1 vb007.101 akaranth            1. Added check for GBR and NGBR.
                                      2. Added support to send DNS IP.
                                      3. Added Tracking area update feature
                                      4. PLMN ID and TAC Code encoded properly as per spec.
*********************************************************************91*/
