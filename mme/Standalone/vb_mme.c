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

     Desc:    This file contains vb application message routing
              source code

     File:    vb_mme.c

     Sid:     vb_mme.c@@/main/1 - Mon Jun 28 19:35:38 2010

     Prg:     nn and sk 
*********************************************************************21*/

/* Header include files (.h)                                  */
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent           */
#include "envind.h"        /* Environment independent         */

#include "gen.h"           /* General                         */
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
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* CNE Application defines         */
#include "cm_esm.h"        /* CNE Application defines         */
#include "cm_emm_esm.h"    /* EMM/ESM EDM                     */

#include <stdlib.h>        /* For atoi()                      */
#ifdef VB_MME_AUTH
#include "vb_hss_autn.h"
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

#include "szt_asn.x"       /* S1AP ASN */
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
#include "vb_s3_hndlr.x"
#include "vb_hss.x"        /* CNE Application structures      */
#ifdef VB_MME_AUTH
#include "vb_hss_autn.x"   /* CNE Application structures     */
#endif /* VB_MME_AUTH */

#ifdef VB_IRAT_ACC_TEST
#include "vb_acc_s3.x"
#endif
#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

PRIVATE  U32      gXaTransId = 0;

PRIVATE Txt   prBuf[PRNTSZE];   /* SPrint buffer */


PRIVATE S16 vbMmeParseCmdLine ARGS((Void));

/*
*
*       Fun:   vbMmeAppInit
*       
*       Desc:  Initializes application data structures
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeAppInit
(
Void
)
#else
PUBLIC S16 vbMmeAppInit(Void)
#endif
{
   S16 ret;
   CmEdmInit init = {0};

   VbMmeUeCb   tmpUeCb;
   U16  offset = 0;

   VB_TRC2(vbMmeAppInit);

   /* This may be triggered by layer manager interface as part of
     MME general configuration */ 
   if((ret = vbMmeParseCmdLine())!= ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Initialize EMM/ESM EDM */
   init.ent = vbMmeCb.init.ent;
   init.inst = vbMmeCb.init.inst;
   init.region = vbMmeCb.init.region;
   init.pool = vbMmeCb.init.pool;
   init.procId = vbMmeCb.init.procId;
#ifdef DEBUGP
   init.dbgMask = vbMmeCb.init.dbgMask;
#endif
   if(cmEmmEsmEdmInit(init) != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                        "EMM/ESM Init Failure"));
      RETVALUE(RFAILED);
   }
  /* Can consider initializing HSS module here */
   ret = vbHssReadUeDB(); 
  
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                        "UE DB Read Failure"));
      RETVALUE(ret);
   }
   offset = (U16) ((PTR)&tmpUeCb.ueSTmsiHashEnt - (PTR)&tmpUeCb); 
   ret = vbMmeUtlInitSTmsiUeCbs(VB_MAX_UECB_BIN, offset, FALSE, 
                                CM_HASH_KEYTYPE_ANY);
   if(ROK != ret)      
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                        "STMSI Based Hashlist Init Failed"));
      RETVALUE(ret);
   }

   offset = (U16) ((PTR)&tmpUeCb.ueImsiHashEnt - (PTR)&tmpUeCb);
   ret = vbMmeUtlInitImsiUeCbs(VB_MAX_UECB_BIN, offset, FALSE, 
                               CM_HASH_KEYTYPE_ANY);
 
   RETVALUE(ret);
} /* end of vbMmeAppInit */


/*
 *
 *       Fun:    vbMmeParseCmdLine
 *
 *       Desc:   Adding the Command Line options
 *
 *       Ret:    
 *
 *       Notes:  None
 *
         File:   vb_mme.c
 *
 */

#ifdef ANSI
PRIVATE S16 vbMmeParseCmdLine
(
Void
)
#else
PRIVATE S16 vbMmeParseCmdLine()
#endif /* ANSI */
{
   S16          ret;      
   Bool         loop;

   VB_TRC2(vbMmeParseCmdLine)

   /*-- SSI options skipped--*/
   loop = TRUE;
   /* parse the command line parameters  */
   while ((loop == TRUE) &&
          ((ret = SGetOpt(msArgc, msArgv, "d:M:S:P:")) != EOF))
   {
      switch(ret)
      {
         case 'd':
         {
            /*-- Set debug level --*/
            vbMmeCb.debugLevel = atoi(msOptArg);
            break;
         }
         case 'M':
         { 
            cmInetAddr(msOptArg, &vbMmeCb.mmeCfg.selfAddr.u.ipv4TptAddr.address);
            break;
         }
         case 'S':
         { 
            cmInetAddr(msOptArg, &vbMmeCb.mmeCfg.sgwAddr.u.ipv4TptAddr.address);
            break;
         }
         case 'P':
         {
            cmInetAddr(msOptArg, &vbMmeCb.mmeCfg.pgwAddr.u.ipv4TptAddr.address);
            break;
         }
         default:
         {
            /*-- Reset parsed instance --*/
            loop = FALSE;
            sprintf(prBuf, "\t[-d ]    -           Set the Debuglevel \n");
            SPrint(prBuf);
            sprintf(prBuf, "\t[-M IPAddr of MME] -Change IP Address of MME \n");
            SPrint(prBuf);
            sprintf(prBuf, "\t[-S IPAddr of SGW] -Change IP Address of SGW \n");
            SPrint(prBuf);
            sprintf(prBuf, "\t[-P IPAddr of PGW] -Change IP Address of PGW \n");
            SPrint(prBuf);
            break;
         } 
      }
   }

   RETVALUE(ROK);
} /*end of vbMmeParseCmdLine */



/*
 *
 *       Fun:    vbMmeParseRdConQ
 *
 *       Desc:   
 *
 *       Ret:    
 *
 *       Notes:  None
 *
         File:   vb_mme.c
 *
 */

#ifdef ANSI
PUBLIC S16  vbMmeParseRdConQ
(
Data data
)
#else
PUBLIC S16  vbMmeParseRdConQ(data)
Data data;
#endif
{
   U32  memAvail;

   TRC2(vbMmeParseRdConQ)

   switch (data)
   {
      case 'm' :
      case 'M' :
         SRegInfoShow(vbMmeCb.init.region, &memAvail);
         break;

#ifdef VB_IRAT_ACC_TEST

      case 'h' :
      case 'H' :
         vbAccTriggeriRATHO();
         break;
#endif         

      default :
         break;
   }

   RETVALUE(ROK);
} /*  end of vbMmeParseRdConQ */


/*
*
*       Fun:   vbMmeRcvEmmMsg
*       
*       Desc:  This function routes all the incoming EMM messages from UE
*              through eNodeB on S1AP. 
*       
*       Ret:   ROK
*       
*       Notes: None
*              
*       File:  vb_mme.c
*       
*/
#ifdef ANSI
PUBLIC S16 vbMmeRcvEmmMsg
(
VbMmeUeCb    *ueCb,
CmNasEvnt    *evnt,
U8           emmMsgType
)
#else
PUBLIC S16 vbMmeRcvEmmMsg(ueCb, evnt, emmMsgType)
VbMmeUeCb    *ueCb;
CmNasEvnt    *evnt;
U8           emmMsgType;
#endif
{
   S16      ret = RFAILED;
   U8       loop = 0;

   VB_TRC2(vbMmeRcvEmmMsg);

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "Received EMM Msg With MsgType(%d)", emmMsgType));

   switch(ueCb->proc)
   {
      case VB_MME_ATTACH_PROC:
      {
         ret = vbMmeHndlAttachOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      case VB_MME_DETACH_UE_PROC:
      {
         ret = vbMmeHndlUeDetachOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      case VB_MME_DETACH_NW_PROC:
      {
         ret = vbMmeHndlNwDetachOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      case VB_MME_SERVICE_REQ_PROC:
      {
         ret = vbMmeHndlServReqOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      case VB_MME_TAU_PROC:
      {
         ret = vbMmeHndlTauOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      case VB_MME_GUTI_REALLOC_PROC:
      {
         ret = vbMmeHndlGutiReallocOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      case VB_MME_ESM_PROC:
      {
         ret = vbMmeHndlEmmOnEsmUeEvnt(ueCb, evnt, emmMsgType);
         loop = 1;
         break;
      }
      case VB_MME_NULL_PROC:
      {
         ret = vbMmeHndlEmmOnUeEvnt(ueCb, evnt, emmMsgType);
         loop = 1;
         break;
      }
      case VB_MME_HANDOVER_PROC:
      {
         /* This is the case where, midway of Handover procedure
          * the UE comes back to the source network due to some rach
          * failure or other radio failures at the target
          */
         ret = vbMmeHndlHandoverOnNasEvnt(ueCb, evnt, emmMsgType);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Invalid Procedure(%d)",\
               ueCb->proc));
         break;
      } 
   }
   
   if(!loop)
   {
      CM_FREE_NASEVNT(&evnt);
   }
  
   RETVALUE(ret);
} /* end of vbMmeRcvEmmMsg */


/*
 *
 *       Fun:   vbMmeRcvEsmMsg
 *       
 *       Desc:  This function routes all the incoming ESM messages from UE
 *              through eNodeB on S1AP. 
 *       
 *       Ret:   
 *       
 *       Notes: None
 *              
 *       File:  vb_mme.c
 *       
 */

#ifdef ANSI
PUBLIC S16 vbMmeRcvEsmMsg
(
VbMmeEsmCb   *esmCb,
CmNasEvnt    *evnt,
U8           esmMsgType
)
#else
PUBLIC S16 vbMmeRcvEsmMsg(esmCb, evnt, esmMsgType)
VbMmeUeCb    *esmCb;
CmNasEvnt    *evnt;
U8           esmMsgType;
#endif
{
   S16 ret = RFAILED;
   U8  loop = 0;

   VB_TRC2(vbMmeRcvEsmMsg)

   /* Added one more check for passing mPDN */
   if (!((VB_MME_NULL_PROC == esmCb->ueCb->proc) || \
      (VB_MME_ESM_PROC == esmCb->ueCb->proc) || ( VB_MME_PDN_CONN_PROC == esmCb->ueCb->proc )))
   {

      /* ESM Info Request is part of attach procedure */
      if(VB_MME_ATTACH_PROC == esmCb->ueCb->proc)
      {
         if(CM_ESM_MSG_ESM_INFO_RSP == esmMsgType)
         {
            ret = vbMmeHndlEsmInfoRsp(esmCb, evnt);
            if(ROK != ret)
            {
               ret = vbMmeSndAttachReject(esmCb->ueCb,\
                     esmCb->ueCb->emmCb.cause, 0);
            }
            CM_FREE_NASEVNT(&evnt);
            RETVALUE(ret);
         }
      }

      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
                         "ignoring Esm Evnt While Emm On Progress"));
      if((CM_ESM_MSG_PDN_CONN_REQ == esmMsgType) ||
          (CM_ESM_MSG_BEAR_RES_ALLOC_REQ == esmMsgType) ||
          (CM_ESM_MSG_PDN_DISCONN_REQ == esmMsgType) ||
          (CM_ESM_MSG_BEAR_RES_MOD_REQ == esmMsgType))
      {
         vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
         VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
      }
      CM_FREE_NASEVNT(&evnt);
      RETVALUE(ret);
   }

   switch(esmCb->proc)
   {
      case VB_MME_ACT_DED_BEAR_PROC:
      {  
         ret = vbMmeHndlActDedBearOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_ACT_DEF_BEAR_PROC:
      {  
         ret = vbMmeHndlActDefBearOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_DEACT_DED_BEAR_PROC:
      {  
         ret = vbMmeHndlDeactDedBearOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_DEACT_DEF_BEAR_PROC:
      {  
         ret = vbMmeHndlDeactDefBearOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_RES_ALLOC_PROC:
      {  
         ret = vbMmeHndlResAllocOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_PDN_CONN_PROC:
      {  
         ret = vbMmeHndlPdnConnOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_PDN_DIS_CONN_PROC:
      {  
         ret = vbMmeHndlPdnDisConnOnNasEvnt(esmCb, evnt, esmMsgType);
         break;
      }
      case VB_MME_NULL_PROC:
      {
         ret = vbMmeHndlEsmOnUeEvnt(esmCb, evnt, esmMsgType);
         loop = 1;
         break;
      }
      default:
      {  
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Invalid Procedure(%d)",\
               esmCb->proc));
         break;
      }
   }
   
   if(!loop)
   {
      CM_FREE_NASEVNT(&evnt);
   }

   RETVALUE(ret);
} /* end of vbMmeRcvEsmMsg */




/*
 *
 *       Fun:   vbMmeRcvS1apMsg
 *       
 *       Desc:  This function routes all the incoming UE specific
 *              messages from eNodeB wihout NAS messages
 *       
 *       Ret:   
 *       
 *       Notes: None
 *              
 *       File:  vb_mme.c
 *       
 */
#ifdef ANSI
PUBLIC S16 vbMmeRcvS1apMsg
(
U32            suConnId,
Ptr            *evnt,
U8             msgType
)
#else
PUBLIC S16 vbMmeRcvS1apMsg(suConnId, evnt, msgType)
U32            suConnId;
Ptr            *evnt
U8             msgType
#endif
{
   S16          ret      = RFAILED;
   VbMmeUeCb    *ueCb    = NULLP;   
   VbMmeEnbCb   *enbCb   = NULLP;   
   U32          ueCbKey  = 0;
   U32          peerId   = 0;
   
   VB_TRC2(vbMmeRcvS1apMsg)
  
   if(NULLP == evnt)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Event Received"));
      RETVALUE(ret);
   }

   VB_MME_GET_PEERID(suConnId, peerId); 
   if (ROK != vbMmeUtlFndEnbCb(&enbCb, peerId))
   {
#ifdef ALIGN_64BIT   
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
              "Unexpected eNodeB Cb PeerId(%d)",peerId));
#else
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
              "Unexpected eNodeB Cb PeerId(%ld)",peerId));
#endif
      RETVALUE(ret); /* Should we send S1 Setup Failure back to eNB ? */
   }

   VB_MME_GET_UEID(suConnId, ueCbKey);

   /* Retrieve the UeCb pointer from the eNodeB block */
   ret = vbMmeUtlFndUeCbOnConnId(enbCb, &ueCb, ueCbKey);
   if(ROK != ret)
   {
      /* We cannot perform UE Context Release as UE Cb itself is not
         found */
#ifdef ALIGN_64BIT         
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unable To Find The ueCb For ueId(%d)",\
                        ueCbKey));
#else                        
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unable To Find The ueCb For ueId(%ld)",\
                        ueCbKey));
#endif                        
      RETVALUE(ret); 
   }    

   switch(ueCb->proc)
   {
      case VB_MME_ATTACH_PROC:
      {
         ret = vbMmeHndlAttachOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_DETACH_UE_PROC:
      {
         ret =  vbMmeHndlUeDetachOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_DETACH_NW_PROC:
      {
         ret = vbMmeHndlNwDetachOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_SERVICE_REQ_PROC:
      {
         ret = vbMmeHndlServReqOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_PAGING_PROC:
      {
         ret = vbMmeHndlPagingOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_TAU_PROC:
      {
         ret = vbMmeHndlTAUOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_CTXT_REL_PROC:
      {
         ret = vbMmeHndlCtxtRelOnS1apEvnt(ueCb, evnt, msgType);
         break;
      } 
      /* mPDN:  when erab setup rsp comes we should call the below fn*/
      case VB_MME_PDN_CONN_PROC: 
      case VB_MME_ESM_PROC:
      {
         ret = vbMmeHndlEsmProcOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_UE_CTXT_MOD_PROC:
      {
         ret = vbMmeHndlUeCtxtModOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_BEAR_REL_IND_PROC:
      {
         ret = vbMmeHndlBearRelIndOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_HANDOVER_PROC:
      {
         ret = vbMmeHndlHandoverOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }
      case VB_MME_NULL_PROC:
      {
         ret = vbMmeHndlS1apOnEnbEvnt(ueCb, evnt, msgType);
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Invalid Procedure(%d)",\
               ueCb->proc));
         break;
      }
   }
   
   RETVALUE(ret);
} /* end of vbMmeRcvS1apMsg */

/*
*
*       Fun:  vbMmeRcvSgwMsg
*       
*       Desc: This function routes all the incoming EGTPC messages from S-GW
*             Interface
*       
*       Ret:  
*       
*       Notes: None
*              
*       File:  vb_mme.c
*/

#ifdef ANSI
PUBLIC S16 vbMmeRcvSgwMsg
(
VbMmeEgtSapCb  *egtSapCb,
VbMmeUeCb      *ueCb,
VbGtpcMsg      msgType,
EgMsg          *egMsg,   /* vb001.101: Added new argument */
TknU32         *transId
)
#else
PUBLIC S16 vbMmeRcvSgwMsg(egtSapCb, ueCb, msgType, esmCb, transId)
VbMmeEgtSapCb  *egtSapCb;
VbMmeUeCb      *ueCb;
VbGtpcMsg      msgType;
EgMsg          *egMsg;
TknU32         *transId;
#endif
{
   S16 ret = ROK;

   VB_TRC2(vbMmeRcvSgwMsg)

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Receiving Msg From SGW UeId(%d),\
         MsgType(%d)", ueCb->ueCbKey,msgType));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Receiving Msg From SGW UeId(%ld),\
         MsgType(%d)", ueCb->ueCbKey,msgType));
#endif
   
   switch(ueCb->proc)
   {
      case VB_MME_ATTACH_PROC:
      {
         ret = vbMmeHndlAttachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
                  transId);
         break;
      }
      case VB_MME_DETACH_UE_PROC:
      {
         ret = vbMmeHndlUeDetachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_DETACH_NW_PROC:
      {
         ret = vbMmeHndlNwDetachOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_SERVICE_REQ_PROC:
      {
         ret = vbMmeHndlServReqOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_TAU_PROC:
      {
         ret = vbMmeHndlTAUOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_IMPLICIT_DETACH_PROC:
      {
         ret = vbMmeHndlImplicitDetachOnSgwEvnt(egtSapCb, ueCb, msgType,\
               egMsg, transId);
         break;
      }
      case VB_MME_PAGING_PROC:
      {
         ret = vbMmeHndlPagingOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_CTXT_REL_PROC:
      {
         ret = vbMmeHndlUeCtxRelOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_PDN_CONN_PROC:
      case VB_MME_ESM_PROC :
      {
         ret = vbMmeHndlEsmProcOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
 
      case VB_MME_BEAR_REL_IND_PROC:
      {
         ret = vbMmeHndlBrRelIndProcOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_HANDOVER_PROC:
      {
         ret = vbMmeHndlHandoverOnSgwEvnt(egtSapCb, ueCb, msgType, egMsg,\
               transId);
         break;
      }
      case VB_MME_NULL_PROC:
      {
         ret = vbMmeHndlNwTrigSgwEvnt(egtSapCb, ueCb, msgType, egMsg, transId);
                        
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Invalid Procedure(%d)",\
               ueCb->proc));
         break;
      }
   }

   RETVALUE(ret);
} /* end of vbMmeRcvSgwMsg */

#ifdef ANSI
PUBLIC S16 vbMmeEmmFailInd
(
VbMmeUeCb       *ueCb,
VbMmeEmmFlrInd  *flrInd
)
#else
PUBLIC S16 vbMmeEmmFailInd(ueCb, flrInd)
VbMmeEmmFlrInd *ueCb;
VbMmeEmmFlrInd *flrInd;
#endif
{
   S16 ret = ROK;

   VB_TRC2(vbMmeEmmFailInd)
   
   switch(flrInd->trg)
   {
      case CM_EMM_MSG_ATTACH_REQ:
      {
         /*Send Attach Reject To UE */
         ret = vbMmeSndAttachReject(ueCb, flrInd->cause, NULLP);
         
         /*Implicitly detach */
         ret = RFAILED;
         if((NULLP != ueCb->ueTunnelCb) &&
            (0 != ueCb->ueTunnelCb->remoteTeId))
         {
            /* Local clean up */
            ueCb->proc = VB_MME_IMPLICIT_DETACH_PROC;
            ret = vbMmeUtlDelPdnConn(ueCb,
                      EGT_GTPC_CAUSE_SYS_FAIL);
            if(ROK != ret)
            {
               ueCb->proc = VB_MME_NULL_PROC;
            }
         }
         if(ROK != ret)
         {
           ret =  vbMmeUtlLocDel(ueCb);
         }
         break;
      }
      default:
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Unhandled Event(%d)\n",\
               flrInd->trg));
         break;
      }
   }

   if(NULLP != flrInd->evntPtr)
   { 
      CM_FREE_NASEVNT((CmNasEvnt**)&flrInd->evntPtr);
   }

   RETVALUE(ret);
}


#ifdef ANSI
PUBLIC S16 vbMmeEmmMaxTmrExpInd
(
VbMmeUeCb       *ueCb,
S16             tmrEvnt
)
#else
PUBLIC S16 vbMmeEmmMaxTmrExpInd(ueCb)
VbMmeUeCb       *ueCb;
S16             tmrEvnt;
#endif
{
   S16 ret = ROK;

   VB_TRC2(vbMmeEmmMaxTmrExpInd)

   switch(ueCb->proc)
   {
       case VB_MME_DETACH_NW_PROC:
       {
          /* Abort Detach Procedure  */
          if (ROK != vbMmeUtlDelPdnConn(ueCb, FALSE))
          {
             VB_MME_DBG_ERROR(( VB_MME_PRNTBUF,\
                  "Start Ue Context Release Procedure"));
             ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
                  SztCauseNasdetachEnum, 0);
          } 
          break;
       }
       case VB_MME_GUTI_REALLOC_PROC:
       {
          VB_MME_DBG_ERROR(( VB_MME_PRNTBUF,\
               "Start Ue Context Release Procedure"));
          ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, SztCauseNasdetachEnum, 0);
          break;
       }
       case VB_MME_TAU_PROC:
       case VB_MME_ATTACH_PROC:
       case VB_MME_SERVICE_REQ_PROC:
       {
          /* Abort Attach Procedure  */
          ueCb->emmCb.piggyAction.status = VB_MME_BEAR_STUP_FAILURE;
          if (VB_TMR_EMM_3450 == tmrEvnt) 
          {
             /* Change Current Proc in UeCb to avoid sending attachReject 
             while receiving delete session response from SGW */
             ueCb->proc = VB_MME_DETACH_NW_PROC;

             ueCb->relCause.choice.val  = CAUSE_NAS;
             ueCb->relCause.val.nas.val = SztCauseNasdetachEnum;
             if (ROK != vbMmeUtlDelPdnConn(ueCb, FALSE))
             {
                VB_MME_DBG_ERROR(( VB_MME_PRNTBUF,\
                     "Start Ue Context Release Procedure"));
                ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val, 
                                          ueCb->relCause.val.nas.val, 0);
             } 
          }
          else if(VB_TMR_EMM_3460 == tmrEvnt)
          {
             ueCb->relCause.choice.val  = CAUSE_NAS;
             ueCb->relCause.val.nas.val = SztCauseNasauthentication_failureEnum;
             if (ROK != vbMmeUtlDelPdnConn(ueCb, FALSE))
             {
                VB_MME_DBG_ERROR(( VB_MME_PRNTBUF,\
                     "Start Ue Context Release Procedure"));
                ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val, 
                                          ueCb->relCause.val.nas.val, 0);
             } 
          }
          else if(VB_TMR_EMM_3470 == tmrEvnt)
          {
             ueCb->relCause.choice.val  = CAUSE_NAS;
             ueCb->relCause.val.nas.val = SztCauseNasdetachEnum;
             if (ROK != vbMmeUtlDelPdnConn(ueCb, FALSE))
             {
                VB_MME_DBG_ERROR(( VB_MME_PRNTBUF,\
                     "Start Ue Context Release Procedure"));
                ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val, 
                                          ueCb->relCause.val.nas.val, 0);
             } 
          }
          else 
          {
            /* Max Retransmission reached for Authentication/Security message */
             VB_MME_DBG_INFO(( VB_MME_PRNTBUF,\
                  "Start Ue Context Release Procedure"));
             ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
                  SztCauseNasdetachEnum, 0);
          }
          break;
       }
       default:
       {
          /* Abort Attach Procedure  */
          if(VB_TMR_EMM_3450 == tmrEvnt)
          {
             /* Change Current Proc in UeCb to avoid sending attachReject
                while receiving delete session response from SGW */
             ueCb->proc = VB_MME_DETACH_NW_PROC;

             if (ROK != vbMmeUtlDelPdnConn(ueCb, FALSE))
             {
                VB_MME_DBG_ERROR(( VB_MME_PRNTBUF,\
                     "Start Ue Context Release Procedure"));
                ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,\
                     SztCauseNasdetachEnum, 0);
                RETVALUE(ROK);
             }
             RETVALUE(ROK);
          }
          else if(VB_TMR_EMM_IMPLI_DTCH_TMR == tmrEvnt)
          {
             ueCb->proc = VB_MME_IMPLICIT_DETACH_PROC;
             if(ROK != vbMmeUtlDelPdnConn(ueCb, FALSE))
             {
                VB_MME_END_EMM_PROC(ueCb);
                vbMmeUtlLocDel(ueCb); 
                RETVALUE(RFAILED);
             }
             RETVALUE(ROK);
          }
          else if(VB_TMR_EMM_3413 == tmrEvnt)
          {
             vbMmeSndDwnLnkNotFailInd(ueCb, EGT_GTPC_CAUSE_UE_NO_RSP);  
             RETVALUE(ROK);
          }

          VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Invalid Timer Evnt"));
          break;
      }
   }
   
   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeEsmMaxTmrExpInd
(
VbMmeEsmCb       *esmCb,
S16             tmrEvnt
)
#else
PUBLIC S16 vbMmeEsmMaxTmrExpInd(esmCb, tmrEvnt)
VbMmeEsmCb       *esmCb;
S16             tmrEvnt;
#endif
{
   VbMmeUeCb   *ueCb = NULLP;
   S16         ret   = ROK;

   VB_TRC2(vbMmeEsmMaxTmrExpInd)
    
   UNUSED(tmrEvnt);

   /* Retrieved ueCb from esmCb */
   ueCb = esmCb->ueCb;

   if ((VB_MME_ESM_PROC == ueCb->proc)) 
   {
      if (VB_MME_ACT_DED_BEAR_PROC == esmCb->proc)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,\
               "Send Create Bearer Response with failure\n"));
         ret = vbMmeSndCBRsp(esmCb,
         VB_MME_BEAR_STUP_FAILURE, EGT_GTPC_CAUSE_UE_REFUSE);

         /* Abort ESM procedure */
         VB_MME_END_ESM_PROC(esmCb);
         ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
      }
      else if (VB_MME_ACT_DEF_BEAR_PROC == esmCb->proc)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,\
               "Send Modify Bearer Response with failure\n"));
         ret = vbMmeSndDSReq (ueCb, esmCb, 0);
         /* Abort ESM procedure */
         VB_MME_END_ESM_PROC(esmCb);
         ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
      }
      else if(VB_MME_DEACT_DED_BEAR_PROC == esmCb->proc)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"\
               Send Delete Bearer Response with failure\n"));
         ret = vbMmeSendDeleteBearerRsp(esmCb,
          VB_MME_BEAR_REL_FAILURE, EGT_GTPC_CAUSE_UE_REFUSE);
         /* Abort ESM procedure */
         VB_MME_END_ESM_PROC(esmCb);
         ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
      }
      else if(VB_MME_DEACT_DEF_BEAR_PROC == esmCb->proc)
      {
         /* Abort ESM procedure */
         VB_MME_END_ESM_PROC(esmCb);
         ret = vbMmeUtlDelEsmCb(esmCb, ueCb);
      }
      else
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,\
               "MaxRetransmissionTmrExp for Invalid Procedure\n"));
      }
   }
   /* ESM Info Procedure is part of attach procedure */
   else if(VB_MME_ATTACH_PROC == ueCb->proc)
   {
      if(VB_TMR_ESM_3489 == tmrEvnt)
      {
         vbMmeAbortAttachProc(ueCb);
      }
   }

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeEgtpErrInd
(
VbMmeUeCb       *ueCb,
EgErrEvnt       *egErrEvnt
)
#else
PUBLIC S16 vbMmeEgtpErrInd(ueCb, egErrEvnt)
VbMmeUeCb       *ueCb;
EgErrEvnt       *egErrEvnt;
#endif
{
   VbMmeUeTunnelCb *tunEnt    = NULLP;
   VbMmeEsmCb      *esmCb     = NULLP;
   TknU32          transId;
   U8              idx;
   S16             ret        = ROK;

   VB_TRC2(vbMmeEgtErrInd)
   
   if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_CS_REQ)
   {
      if(VB_MME_ATTACH_PROC == ueCb->proc)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending Create Session Req Failed"));
         /* Send the Attach Reject */
         ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
      }
      else if(VB_MME_ESM_PROC == ueCb->proc) 
      {
         transId.val  = egErrEvnt->transId;
         transId.pres = TRUE;
         ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, &transId);
         if(ROK == ret)
         {
            /* Pdn Connectivity Procedure */
            ret = vbMmeSndPdnConnRej(esmCb, VB_ESM_NETWORK_FAILURE);
            /* Clean up esm cb */
            VB_MME_END_ESM_PROC(esmCb);
            /* Remove esmCb from transaction list */
            vbMmeRmvFrmTrnList(esmCb->ueCb, esmCb->tId);
            /* Release esmCb memory */
            VB_MME_FREE(esmCb, sizeof(VbMmeEsmCb));
         } 
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Transaction not find"));
            RETVALUE(RFAILED);
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandle procedure"));
         RETVALUE(RFAILED);
      }
   } 
   else if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_MB_REQ)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending Modify Bearer Req Failed"));
      if(VB_MME_ESM_PROC == ueCb->proc) 
      {
         /* Pdn Connectivity Procedure */
         transId.val  = egErrEvnt->transId;
         transId.pres = TRUE;
         ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, &transId);
         if(ROK == ret)
         {
            /* Release the bearer */
            vbMmeUtlDelEsmCb(esmCb, ueCb);
            /* Release the connection */
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,
                  SztCauseMiscunspecifiedEnum, 0);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Transaction not find"));
            RETVALUE(RFAILED);
         }
      }
      else if((VB_MME_ATTACH_PROC == ueCb->proc) ||
               (VB_MME_TAU_PROC == ueCb->proc)    ||
               (VB_MME_SERVICE_REQ_PROC == ueCb->proc))
      {
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,\
               SztCauseMiscunspecifiedEnum, 0);
      }
      else if(VB_MME_CTXT_REL_PROC == ueCb->proc)
      {
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,\
               SztCauseMiscunspecifiedEnum, 0);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandle procedure"));
      }
   }
   else if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_DS_REQ)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending Delete Session Request Failed"));

      transId.val  = egErrEvnt->transId;
      transId.pres = TRUE;
      ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, &transId);

      if(VB_MME_ESM_PROC == ueCb->proc)
      {
         if(ROK == ret)
         {
            if(VB_MME_ACT_DEF_BEAR_PROC == esmCb->proc)
            {
               VB_MME_END_ESM_PROC(esmCb);
               vbMmeUtlDelEsmCb(esmCb, ueCb);
            }
            else if (VB_MME_PDN_DIS_CONN_PROC == esmCb->proc)
            {
                vbMmeSndPdnDisConnRej(esmCb, VB_ESM_NETWORK_FAILURE);
                vbMmeUtlDelEsmCb(esmCb, ueCb);
            }
            else
            {
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandle Procedure"));
               RETVALUE(RFAILED);
            }
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Transaction Not Found"));
            RETVALUE(RFAILED);
         }

         RETVALUE(ROK);
      }

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
      /*Send the Local Delete Request for the Tunnel */
      VbLiEgtLclDelReq(&(vbMmeCb.egtSapCb->pst), vbMmeCb.egtSapCb->spId, 0,
                       ueCb->ueTunnelCb->localTeIdKey);
      tunEnt =  ueCb->ueTunnelCb;
      ueCb->ueTunnelCb = NULLP;
      cmHashListDelete(&(vbMmeCb.egtSapCb->ueTunnelCbHlCp), (PTR)tunEnt);
      VB_MME_FREE(tunEnt, sizeof(VbMmeUeTunnelCb));


      if(VB_MME_DETACH_UE_PROC == ueCb->proc)
      {
         /* send Detach accept if the UE is not Switched Off */
         if (!VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_DETACH_TYPE_SWITCHOFF))
         {
            ret = vbMmeSndDetachAcc(ueCb);
         }   
         /* Send the UE Context Release command */
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                  SztCauseNasdetachEnum, 0 /* No need to send RAB Req */);
      }
      else if(VB_MME_DETACH_NW_PROC == ueCb->proc)
      {
         /* Start Ue Context Release Procedure */
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS, SztCauseNasdetachEnum, 0);
      }
      else if(VB_MME_ATTACH_PROC == ueCb->proc)
      {
         if(VB_MME_BEAR_STUP_ONGOING == ueCb->emmCb.piggyAction.status)
         {

            for(idx=0; idx < CM_ESM_MAX_BEARER_ID; idx++)
            {
               if(ueCb->esmList[idx])
               {
                  vbMmeUtlDelEsmCb(ueCb->esmList[idx], ueCb);
               }
            }

            ret = vbMmeGetFrmTrnList(ueCb, &esmCb,
                                     ueCb->emmCb.piggyAction.tId);
            if(ROK == ret)
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                                "Sending Create Session Request"));
               ret = vbMmeSndCSReq(ueCb, esmCb);
            }
            if(ROK != ret)
            {
               VB_MME_DBG_INFO((VB_MME_PRNTBUF,
                                     "Sending Create Session Request Failed"));
               /* Send the Attach Reject */
               ret = vbMmeSndAttachReject(ueCb, CM_EMM_NW_FAILURE, 0);
            }
         }
         else
         {
            /* Attach Accept Timer Expiry Case */
            ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
                                ueCb->relCause.val.nas.val, 0);

         }
      }
      else if(VB_MME_TAU_PROC == ueCb->proc)
      {
         ret = vbMmeInitCtxtRelProc(ueCb, ueCb->relCause.choice.val,
                ueCb->relCause.val.nas.val, 0 /* No need to send RAB Req */);
      }
      else if(VB_MME_IMPLICIT_DETACH_PROC == ueCb->proc)
      {
         vbMmeUtlLocDel(ueCb);
      }
      else if(VB_MME_CTXT_REL_PROC == ueCb->proc)
      {
         vbMmeUtlLocDel(ueCb);
      }
   }
   else if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_CB_RSP)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Sending Create Bearer Resp Failed"));
      if(VB_MME_ESM_PROC == ueCb->proc)
      {
         /* Send the UE Context Release command */
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,
               SztCauseMiscunspecifiedEnum, 0);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandle Procedure"));
      }
   }
   else if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_BR_CMND)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,\
            "Sending Bearer Resource Command Failed"));
      if(VB_MME_ESM_PROC == ueCb->proc)
      {
         transId.val  = egErrEvnt->transId;
         transId.pres = TRUE;
         ret = vbMmeUtlGetEsmCbOnS11TranId(ueCb, &esmCb, &transId);
         if(ROK == ret)
         {
            /* Sent the Bearer Resource Allocation Reject to UE */
            ret = vbMmeSndResAllocRej(esmCb, VB_ESM_NETWORK_FAILURE);
         }
         else
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Transaction Not Found"));
         }
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unhandle Procedure"));
      }
   }
   else if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_FR_REQ)
   {
      if (VB_MME_HANDOVER_PROC == ueCb->proc)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Sending Handover Preparation "\
               "Failure to eNodeB"));
         ret = vbMmeSndHOPrepFail(ueCb);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sending Handover Preparation "\
                  "Failure to eNodeB"));
         }

         VB_MME_END_EMM_PROC(ueCb);
         vbMmeUtlDelS10Tun(ueCb);
         vbMmeUtlDelHoData(ueCb);
      }
   }
   else if(egErrEvnt->failedMsgType == EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, \
            "Sending Downlink Data Notification Ack Failed"));
   }
   else
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF,"Invalid EG Msg Type"));
   }

   RETVALUE(ret);
}

#ifdef ANSI
PUBLIC S16 vbMmeS1apErrInd
(
VbMmeUeCb         *ueCb,
SztLclErrInd      *sztLclErrInd
)
#else
PUBLIC S16 vbMmeS1apErrInd(ueCb, sztLclErrInd)
VbMmeUeCb         *ueCb;
SztLclErrInd      *sztLclErrInd;
#endif
{
   U8 ret        = ROK;

   VB_TRC2(vbMmeS1apErrInd)

   switch(sztLclErrInd->causeType)
   {
      case SZT_CAUSE_MSG_NOT_DELIVERED: 
      case SZT_CAUSE_LOWER_LAYER_SND_FAILED: 
      case SZT_CAUSE_INV_PEERID:        
      case SZT_CAUSE_SAP_NOTPRST:       
      case SZT_CAUSE_ASSOC_NOTPRST:     
      case SZT_CAUSE_PC_TX_SYNTAX_ERR: 
      case SZT_CAUSE_MEM_ALLOC_FAIL:   
      case SZT_CAUSE_HASH_INS_FAIL:     
      case SZT_CAUSE_UNEXP_MSG:        
      case SZT_CAUSE_INV_CONID:        
      case SZT_CAUSE_INV_TYPE:         
      case SZT_CAUSE_NOT_APPL:         
      case SZT_CAUSE_PC_ABS_SYNTAX_ERR: 
      case SZT_CAUSE_INV_STATE:         
      case SZT_CAUSE_RX_ERRONEOUS_RSP:  
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "S1-Connection Issue"));
         /* Take error specific action */
         break;
      }
      default:
      {
#ifdef ALIGN_64BIT   
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,\
               "Unhandled Cause Type(%d) For UE(%d)", 
               sztLclErrInd->causeType, ueCb->ueCbKey));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF,\
               "Unhandled Cause Type(%d) For UE(%ld)", 
               sztLclErrInd->causeType, ueCb->ueCbKey));
#endif
      }
   }

   /* Take procedure specific action */
   switch(ueCb->proc)
   {
      case VB_MME_ATTACH_PROC:
      {
         vbMmeAbortAttachProc(ueCb);
         break;
      }
      case VB_MME_DETACH_UE_PROC:
      {
         vbMmeUtlLocDel(ueCb);
         break;
      }
      case VB_MME_DETACH_NW_PROC:
      {
         ret = vbMmeUtlDelPdnConn(ueCb, FALSE);
         if(ROK != ret)
         {
            /* Release the connection */
            ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_NAS,
                                       SztCauseNasdetachEnum, 0);
         }
         break;
      }
      case VB_MME_SERVICE_REQ_PROC:
      {
         /* Release the connection */
         ret = vbMmeInitCtxtRelProc(ueCb, CAUSE_MISC,
                                       SztCauseMiscunspecifiedEnum, 0);
         break;
      }
      case VB_MME_TAU_PROC:
      {
         vbMmeAbortTAUProc(ueCb);
         break;
      }
      case VB_MME_PAGING_PROC:
      {
         /* Send Failure Ind */
         vbMmeSndDwnLnkNotFailInd(ueCb, EGT_GTPC_CAUSE_UE_NO_RSP);
         break;
      }
      case VB_MME_ESM_PROC:
      {
         vbMmeAbortEsmProc(ueCb);
         break;
      }
      case VB_MME_GUTI_REALLOC_PROC:
      {
         /*Do no what to do */
         break;
      }
      case VB_MME_CTXT_REL_PROC:
      {
         vbMmeRelLocS1Con(ueCb);
         VB_MME_END_EMM_PROC(ueCb);
         vbMmeUtlLocDel(ueCb);
         break;
      }
      case VB_MME_IMPLICIT_DETACH_PROC:
      {
         vbMmeRelLocS1Con(ueCb);
         VB_MME_END_EMM_PROC(ueCb);
         vbMmeUtlLocDel(ueCb);
         break;
      }
      default:
      {
#ifdef ALIGN_64BIT   
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, \
            "Unhandled Procedure (%d) for UE(%d)", 
            ueCb->proc, ueCb->ueCbKey));
#else
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, \
               "Unhandled Procedure (%d) for UE(%ld)", 
               ueCb->proc, ueCb->ueCbKey));
#endif
         break;
      }
   }

   RETVALUE(ROK);
}

#ifdef VB_PERF_MEAS


#ifdef ANSI
PUBLIC Void vbMmePerfMeasEnd
(
VbMmeUeCb *ueCb
)
#else
PUBLIC Void vbMmePerfMeasEnd(ueCb)
VbMmeUeCb *ueCb;
#endif
{
   struct timeval uePresentTime;
   float ueRate;
   float numUsec=0;
   float numMsec;
   float diffUsec;

   gettimeofday(&uePresentTime, NULL);

   if (uePresentTime.tv_usec > ueCb->eNodebCb->uePrevTime.tv_usec) 
   {
      diffUsec = (float)
                 (((float)(uePresentTime.tv_usec - \
                 ueCb->eNodebCb->uePrevTime.tv_usec)) / 1000);
   }
   else
   {
      diffUsec = (float)(((1000000 - numUsec) + uePresentTime.tv_usec) / 1000);   
      uePresentTime.tv_sec--;
   }
   numMsec = (float) 
             (((float)(uePresentTime.tv_sec - \
             ueCb->eNodebCb->uePrevTime.tv_sec) * 1000)
                       + diffUsec); 
      
   ueRate = \
      (float)((float)(ueCb->eNodebCb->eNbCfg.maxUeNum)/(float)(numMsec / 1000));
   printf("UE rate = %f ps processed for %d UEs\n", ueRate, 
           ueCb->eNodebCb->eNbCfg.maxUeNum);

   RETVOID;
}

#endif

/**************************************************************************//**
 *
 * @brief 
 *    S1AP Release Indication during Handover
 *             
 * @param[in] suConnId
 * @param[in] evnt
 *    S1AP Release indication event
 * @param[in] msgType
 *    Message type
 *    
 * @return
 *    SUCCESS/FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeRcvS1apRelInd
(
U32            suConnId,
Ptr            *evnt,
U8             msgType
)
{
   S16          ret      = RFAILED;
   VbMmeUeCb    *ueCb    = NULLP;   
   VbMmeEnbCb   *enbCb   = NULLP;   
   U32          ueCbKey  = 0;
   U32          peerId   = 0;
   
   VB_TRC2(vbMmeRcvS1apRelInd)
  
   if (NULLP == evnt)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "NULL Event Received"));
      RETVALUE(ret);
   }

   VB_MME_GET_PEERID(suConnId, peerId); 
   if (ROK != vbMmeUtlFndEnbCb(&enbCb, peerId))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Unexpected eNodeB Cb PeerId(%ld)",\
            peerId));
      RETVALUE(ret);
   }

   VB_MME_GET_UEID(suConnId, ueCbKey);
   ret = vbMmeUtlFndUeCbOnConnId(enbCb, &ueCb, ueCbKey);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unable To Find The ueCb For "\
            "ueId(%ld)", ueCbKey));
      RETVALUE(ret); 
   }    
   
   switch (ueCb->proc)
   {
      case VB_MME_HANDOVER_PROC:
      {
         ret = vbMmeHndlHandoverFailOnS1apEvnt(ueCb, evnt, msgType);
         break;
      }

      default:
      {
         VB_MME_DBG_ERROR(( VB_MME_PRNTBUF, "Invalid Procedure(%d)",\
               ueCb->proc));
         break;
      }
   }
   
   RETVALUE(ret);
} /* vbMmeRcvS1apRelInd */

#ifdef X2_HO
/*
 * 
 *        Fun:   vbMmeHdlPathSwRqst
 *        
 *        Desc:  Function for handling incoming
 *               Path switch Request msg. 
 *        
 *        Ret:   ROK
 *        
 *        Notes: None
 *               
 *        File:  vb_mme.c
 *        
 **/
#ifdef ANSI
   PUBLIC S16 vbMmeHdlPathSwRqst
(
 VbMmeEnbCb     *eNbCb,
 SztConInd      *datEvnt
)
#else
PUBLIC S16 vbMmeHdlPathSwRqst(eNbCb, datEvnt)
   VbMmeEnbCb     *eNbCb;
   SztConInd      *datEvnt;
#endif
{
   VbMmeUeCb              *ueCb = NULLP;
   VbMmeEnbCb             *oldENbCb;
   U32                     ueCbKey = 0;
   SztE_RABToBeSwedDLLst  *erabToBeSwedDLLst = NULLP;
   SztMME_UE_S1AP_ID      *srcMmeUeS1Id = NULLP;
   SztENB_UE_S1AP_ID      *srcEnbId = NULLP;
   S16                     ret = ROK;
   VbMmeEsmCb              *esmCb = NULLP;
   U8                      bearerId;

   VB_TRC2(vbMmeHdlPathSwRqst)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,
            "vbMmeHdlPathSwRqst: Incoming Path Switch Request Message"));

   /* Get the source mme ue s1ap IE from the Path switch request */
   vbMmeUtlGetS1apIE(&(((S1apPdu*)datEvnt->pdu)->pdu), Sztid_SrcMME_UE_S1AP_ID, (TknU8**)&srcMmeUeS1Id);
   if(srcMmeUeS1Id == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unable to find the MME UE S1AP ID\n"));
      RETVALUE(RFAILED); 
   }
   vbMmeUtlGetS1apIE(&(((S1apPdu*)datEvnt->pdu)->pdu), Sztid_eNB_UE_S1AP_ID, (TknU8**)&srcEnbId);
   if(srcEnbId == NULLP)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unable to find the ENB UE S1AP ID\n"));
      RETVALUE(RFAILED); 
   }

   /* Retrieve the UeCb pointer from the eNodeB block  */
   ret = vbMmeUtlFndUeCbMmeS1apId(&ueCb, srcMmeUeS1Id->val); 
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Unable to find the ueCb ueId(%ld)",\
               ueCbKey));
      RETVALUE(ret); 
   }

   /* Get the IE from the Path switch request */
   vbMmeUtlGetS1apIE(&datEvnt->pdu->pdu, Sztid_E_RABToBeSwedDLLst, (TknU8**)&erabToBeSwedDLLst);
   ueCbKey = ueCb->ueCbKey; 
   ret = vbMmeUtlDelUeCbOnConnId(ueCb);
   if(ret != ROK)
   {
      /* freeing the correct pointer */
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"delete enb suconnid hash list"));
      RETVALUE(RFAILED);
   }

   vbMmeUtlAllocConId(eNbCb, &ueCbKey);
   if(0 == ueCbKey)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,
               "Init UE Message:Service User Connection Id Alloc Failure"));
      RETVALUE(ret); /* Should we send Failure back to eNB */
   } 

   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"adding UE(%ld) peerId(%d)", ueCbKey, eNbCb->peerId));
   ret = vbMmeUtlAddUeCbOnConnId(ueCbKey, eNbCb, ueCb);
   if(ret != ROK)
   {
      /* freeing the correct pointer */
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"adding into suconnid hash list"));
      RETVALUE(RFAILED);
   }
   ueCb->eNodebCb = eNbCb;
   ueCb->proc = VB_MME_HANDOVER_PROC; 
   ueCb->hoType = VB_MME_X2_HO;
   ueCb->pthSwthSpConnId = datEvnt->u.spConnId;
   ueCb->pthSwtchReq = TRUE;
   ueCb->mMeUeS1apId = datEvnt->u.spConnId;
   ueCb->eNbUeS1apId =  srcEnbId->val;
   ueCb->suConnId = ueCbKey; 
   ueCb->spConnId = datEvnt->u.spConnId; 

   if(erabToBeSwedDLLst->noComp.pres == TRUE)
   {
      bearerId =  erabToBeSwedDLLst->member->value.u.sztE_RABToBeSwedDLItem.e_RAB_ID.val;
   }
   else 
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF," Erab to be switched is Zero in path switch request message from Enb "));
      RETVALUE(RFAILED);
   }
   if (ROK != vbMmeUtlFndEsmCb(ueCb, &esmCb, bearerId, 0))
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Failed To Find esmCb From Received Bearer Id(%d)", bearerId));
      RETVALUE(RFAILED);
   }

   esmCb->s1EnbAddr.type = CM_INET_IPV4ADDR_TYPE;

   VB_MME_DEC_S1AP_IPADDR(esmCb->s1EnbAddr, erabToBeSwedDLLst->member->value.u.sztE_RABToBeSwedDLItem.transportLyrAddr);
   VB_MME_DEC_S1AP_TEID(esmCb->s1EnbTeId, erabToBeSwedDLLst->member->value.u.sztE_RABToBeSwedDLItem.gTP_TEID);
   ret = vbMmeSndMBReq (ueCb, esmCb, 0);
   if(ret != ROK)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Sending modify bearer Request failed"));
      RETVALUE(RFAILED);

   }
   RETVALUE(ret);
}
#endif

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
/**********************************************************************
  
         End of file:     vb_mme.c@@/main/1 - Mon Jun 28 19:35:38 2010
   
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
/main/1  vb006.101   ragrawal         1. TIC_ID:ccpu00117534 Fixed warning for gcc compilation on CentOS 
                                      2. CRID:ccpu00117391 ERAB RELEASE support. 
                                      3. ccpu00118283 new case added to handle Identity Response in vbMmeRcvEmmMsg
                                      4. Test Code for OverLoading Feature.
                                      5. Fix for Klockwork issue.
                                      6. Added new function vbMmeSndPathSwtchAck() and
                                         vbMmeHdlPathSwRqst() and vbMmeHdleNBUpd().
                                      7. Moved function for printing IMSI value below
                                         in vbMmeRcvEmmMsg().
                                      8. Added IMSI_REATTACH support.
/main/1  vb007.101   akaranth         1. Added Tracking area update feature.
                                      2. Kworks fix.
*********************************************************************91*/
