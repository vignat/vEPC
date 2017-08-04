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

     Name:    EGTP 

     Type:    C source file

     Desc:    C code for controlling acceptance tests execution.

     File:    egac_egtreg.c

     Sid:      smeg_s5utl.c@@/main/3 - Fri Feb  4 18:18:27 2011

     Prg:     pc

*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"         /* environment options          */
#include "envdep.h"         /* environment dependent        */
#include "envind.h"         /* environment independent      */
#include "gen.h"            /* general layer                */
#include "ssi.h"            /* system services              */
#include "cm5.h"            /* common timer module          */
#include "cm_hash.h"        /* common hash list             */
#include "cm_tkns.h"        /* common tokens                */
#include "cm_tpt.h"         /* Transport  header file       */
#include "cm_mblk.h"        /* common memory allocation     */

#/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#endif

#include "cm_llist.h"      /* common link list */
#include "cm_dns.h"
#include "cm_xta.h"

#include "egac_acc.h"
#include "eg_util.h"       /* Egtpeter Utility library */
#include "egt.h"           /* Egtpeter Upper Interface */
#include "eg.h"           /* Egtpeter Upper Interface */
#include "eg_edm.h"
#include "egac_edm.h"           /* Egtpeter Upper Interface */
#include "leg.h"           /* Egtpeter Layer Manager   */
#include "eg_dbs5.h"           /* Egtpeter Layer Manager   */
#include "eg_dbutl.h"           /* Egtpeter Layer Manager   */

/* header/extern include files (.x) */
#include "gen.x"            /* general layer                */
#include "ssi.x"            /* system services              */
#include "cm5.x"            /* common timer module          */
#include "cm_lib.x"         /* common library               */
#include "cm_hash.x"        /* common hash list             */
#include "cm_tkns.x"        /* common tokens                */
#include "cm_mblk.x"        /* common memory allocation     */
#include "cm_inet.x"        /* Inet header file             */
#include "cm_tpt.x"         /* Transport  header file       */
#include "cm_llist.x"      /* common link list */
#include "cm_dns.x"

#include "egt.x"           /* Eg Upper Interface */
#include "leg.x"           /* Eg layer manager  */
/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif
#include "eg_edm.x"
#include "eg.x"
#include "cm_xta.x"
#include "egac_acc.x"
#include "egac_edm.x"
#include "eg_util.x"       /* Eg Utility library */
#include "smeg_apputl.x" 
#include "smeg_ieutl.x" 

#ifdef EGTP_C

PUBLIC Void  egEgtBuildS5CrtSesReqMsg ARGS
((
CmXtaTCCb         *tcCbS5,
CmXtaSpCb         *spCb,
EgMsg           **egMsg,
Mem              *memInfo
));
PUBLIC Void  egEgtBuildS5CrtSesRspMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5CrtBreReqMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5CrtBreRspMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5BreResCmdMsg  ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5BreResFlrIndMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5ModBreReqMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5ModBreRspMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5DelSesReqMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5DelBreReqMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5DelSesRspMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5DelBreRspMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5ModBreCmdMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5ModBreFlrIndMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5UpdBreReqMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5UpdBreRspMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5DeActvBreCmdMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

PUBLIC Void egEgtBuildS5DeActvBreFlrIndMsg ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
Mem         *memInfo
));

EXTERN Void  egAcEgtBuildEgIeHeader ARGS
((
EgIe         *egIe,
U8           ieType,
U8           ieInst,
U8           dataType
));

EXTERN Void  egAcEgtBuildEgHeader ARGS
((
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsgHdr         *egHdr,
U8                msgType
));

 
/*
 *
 *       Fun:   egEgtBuildS5CrtSesReqMsg
 *
 *       Desc:  This function is used to Build CSR message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5CrtSesReqMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5CrtSesReqMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;
   Bool                  unknownIe=FALSE;
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5CrtSesReqMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5CrtSesReqMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CS_REQ, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_CS_REQ);
   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_IMSI_IETYPE:         
            { 
               /*****************
                * Add IMSI code *
                ****************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_MSISDN_IETYPE:                
            {
               /* Add MSISDN */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMsisdnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_MEI_IETYPE:                       
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMeiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_ULI_IETYPE:        
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
         case EG_SIG_PRIOR_IND_IETYPE :
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSigPriorIndIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
#endif
         case EG_SERVING_NWK_IETYPE:              
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RAT_TYPE_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRatTypeIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_INDICATION_IETYPE:           
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_FTEID_IETYPE:         
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_APN_IETYPE:             
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAccessPntNameIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_SEL_MODE_IETYPE: 
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSelModeIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PDN_TYPE_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPdnTypeIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_APN_RESTRICT_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddMaxApnRestIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_BEARER_CNTX_IETYPE: 
            {
               /*  Based on instance */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PAA_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPaaIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }

         case  EG_AMBR_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }

         case  EG_PCO_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }

         case  EG_TRACE_INFO_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddTrcIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_RECOVERY_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }   
         case  EG_CSID_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }   

         case  EG_LDN_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddLdnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_PRIVATE_EXT_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }   
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */
   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, "unknownIe", &unknownIe);
   if (unknownIe)
   {
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb,spCb,*egMsg,&egLvlInfo);
   }

}  /*  egEgtBuildS5CrtSesReqMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5CrtSesRspMsg
 *
 *       Desc:  This function is used to Build CSR message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5CrtSesRspMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5CrtSesRspMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/ 
   EG_TRC2(egEgtBuildS5CrtSesRspMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,
                  "egacEgtBuildCSRMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CS_REQ, EGT_GTP_INT_S5_S8);

   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_CS_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*****************
                * Add IMSI code *
                ****************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,
                     gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_FTEID_IETYPE:         
            {
               /*  Based on instance */
               /*   Add User Name AVP      */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PAA_IETYPE:
            {
               /* Add Proxy Info AVP    */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPaaIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_APN_RESTRICT_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddMaxApnRestIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_AMBR_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_PCO_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_BEARER_CNTX_IETYPE: 
            {
               /*  Based on instance */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_TRACE_INFO_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddTrcIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_RECOVERY_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }   
         case  EG_CSID_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }   
            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
         case  EG_LDN_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddLdnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_EPC_TMR_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddEPCTimer(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
#endif
         case  EG_PRIVATE_EXT_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }   
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5SesRspMsg     */ 

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelBreCmdMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelBreCmdMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5DelBreCmdMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelBreCmdMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DB_CMND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_CMND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelBreCmdMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_BEARER_CNTX_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}


#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelBreFlrIndMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelBreFlrIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5DelBreFlrIndMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelBreFlrIndMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DB_FAIL_IN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_FAIL_IN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelBreFlrIndMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_CAUSE_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_BEARER_CNTX_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
            case EG_RECOVERY_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}



/*
 *
 *       Fun:   egEgtBuildS5CrtBreReqMsg
 *
 *       Desc:  This function is used to Create Bearer Request message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5CrtBreReqMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5CrtBreReqMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5CrtBreReqMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildCrtBreReqMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CB_REQ, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_CB_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_PROCEDURE_TID_IETYPE:         
            { 
               /*********************************
                * Add Procedure Transaction     *
                ********************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_EBI_IETYPE:                
            {
               /* Add LBI */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_AMBR_IETYPE:                        
            {
               /*   Add APN AMBR */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PCO_IETYPE:        
            {
               /*   Add Protocol configuration option */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_CSID_IETYPE:                
            {
               /*   Add CSID */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5CrtBreReqMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5CrtBreRspMsg
 *
 *       Desc:  This function is used to Create Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5CrtBreRspMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5CrtBreRspMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5CrtBreRspMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5CrtBreRspMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CB_RSP, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_CB_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_CSID_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5CrtBreRspMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5BreResCmdMsg
 *
 *       Desc:  This function is used to Build CSR message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5BreResCmdMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5BreResCmdMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5BreResCmdMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5BreResCmdMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_BR_CMND, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_BR_CMND);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_EBI_IETYPE:         
            { 
               /****************************
                * Add Linked EPS Bearer ID *
                ***************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_PROCEDURE_TID_IETYPE:                
            {
               /* Add Procedure Transaction ID */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_FLOW_QOS_IETYPE:                       
            {
               /*   Add Flow Quality of Services */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFlowQosIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_TAD_IETYPE:        
            {
               /*   Add Traffic Aggregation Description */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTadIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
#ifdef EG_REL_AC0
         case EG_SIG_PRIOR_IND_IETYPE :
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSigPriorIndIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
#endif
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5BreResCmdMsg     */ 


/*
 *
 *       Fun:   egEgtBuildS5BreResFlrIndMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5BreResFlrIndMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5BreResFlrIndMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5BreResFlrIndMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5BreResFlrIndMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_BR_FAIL_IND, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_BR_FAIL_IND);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_EBI_IETYPE:                
            {
               /* Add LBI */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PROCEDURE_TID_IETYPE:                        
            {
               /*   Add Procedure transaction ID */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5BreResFlrIndMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5ModBreReqMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Request message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5ModBreReqMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ModBreReqMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5ModBreReqMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5ModBreReqMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_REQ, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_MB_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_MEI_IETYPE:         
            { 
               /*********************************
                * Add Mobile Equipment identity *
                ********************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMeiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_ULI_IETYPE:                
            {
               /* Add User location info */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_SERVING_NWK_IETYPE:                        
            {
               /*   Add Serving Network */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RAT_TYPE_IETYPE:        
            {
               /*   Add RAT Type */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRatTypeIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_INDICATION_IETYPE:              
            {
               /*   Add Indication flags */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_FTEID_IETYPE:              
            {
               /*   Add Fteid */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_AMBR_IETYPE:              
            {
               /*   Add Aggregate Maximum Bit Rate */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_DELAY_IETYPE:              
            {
               /*   Add delay value */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddDelayIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case  EG_LDN_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddLdnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5ModBreReqMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5ModBreRspMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5ModBreRspMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ModBreRspMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo egLvlInfo;
   LegMsgEntry *gmEntry = NULLP;
   U16 idx              = 0;
   U8 bearerRepeat      = TRUE;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5ModBreRspMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5ModBreRspMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_RSP, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_MB_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:
            { 
               /* Add Cause */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
         case EG_EBI_IETYPE:
            {
               /****************************
                * Add Linked EPS Bearer ID *
                ***************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;

            }

         case EG_MSISDN_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMsisdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case EG_AMBR_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case EG_APN_RESTRICT_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMaxApnRestIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case EG_CNG_RPT_ACT_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCngRptActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         case EG_CSG_INFO_REP_ACT_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsgInfoRepActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#endif

         case EG_FQDN_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case EG_IPADDR_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
         case EG_FTEID_IETYPE:
            {
               /*   Add Fteid */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case EG_PCO_IETYPE:
            {
               /*   Add Protocol configuration option */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case EG_BEARER_CNTX_IETYPE:
            {
               /*   Add Bearer Context */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                     (Txt *)"bearerRepeat",  &bearerRepeat);
               if(bearerRepeat)
               {
                  cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  egLvlInfo.ieInfo[0].occrPres = TRUE;
                  egLvlInfo.ieInfo[0].occr = 2;
                  /* Common function which frame message based on instance value */
                  makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               }
               break;
            } /* end of case */

         case EG_RECOVERY_IETYPE:
            {
               /*   Add Recovery */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#ifdef EG_REL_AC0
         case  EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
         case EG_INDICATION_IETYPE:                
            {
               /* Add Indication Flags */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
#endif

         case EG_PRIVATE_EXT_IETYPE:
            {
               /*   Add Private Extension */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         default:
            {
               break;
            } /* end of case */
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5ModBreRspMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5DelSesReqMsg
 *
 *       Desc:  This function is used to Build Delete Session Request message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelSesReqMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelSesReqMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5DelSesReqMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5DelSesReqMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DS_REQ, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DS_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_EBI_IETYPE:         
            { 
               /****************************
                * Add Linked EPS Bearer ID *
                ***************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_INDICATION_IETYPE:                
            {
               /* Add Indication Flags */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PCO_IETYPE:                        
            {
               /*   Add protocol configuration options */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5DelSesReqMsg     */ 
/*
 *
 *       Fun:   egEgtBuildS5DelBreReqMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelBreReqMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelBreReqMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5DelBreReqMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5DelBreReqMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_REQ, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DB_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_EBI_IETYPE:         
            { 
               /****************************
                * Add Linked EPS bearer ID *
                ***************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PROCEDURE_TID_IETYPE:                
            {
               /*   Add Procedure Transaction ID */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_AMBR_IETYPE:                
            {
               /*   Add APN AMBR */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PCO_IETYPE:        
            {
               /*   Add Protocol configuration option */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_CSID_IETYPE:                
            {
               /*   Add CSID */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5DelBreReqMsg     */

/*
 *
 *       Fun:   egEgtBuildS5DelSesRspMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelSesRspMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelSesRspMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5DelSesRspMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5DelSesRspMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DS_RSP, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DS_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PCO_IETYPE:        
            {
               /*   Add Protocol configuration option */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5DelSesRspMsg     */ 
/*
 *
 *       Fun:   egEgtBuildS5DelBreRspMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelBreRspMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelBreRspMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5DelBreRspMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5DelBreRspMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_RSP, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DB_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_EBI_IETYPE:                
            {
               /* Add LBI */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_CSID_IETYPE:                
            {
               /*   Add CSID */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5DelBreRspMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5ModBreCmdMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Command
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5ModBreCmdMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ModBreCmdMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5ModBreCmdMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5ModBreCmdMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_CMND, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_MB_CMND);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case  EG_AMBR_IETYPE:  
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }

         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }

         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5ModBreCmdMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5ModBreFlrIndMsg
 *
 *       Desc:  This function is used to Build Modify Bearer Failure Indication
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5ModBreFlrIndMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ModBreFlrIndMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5ModBreFlrIndMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5ModBreFlrIndMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MBFAIL_IND, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_MBFAIL_IND);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /* egEgtBuildS5ModBreFlrIndMsg     */ 

/*
 *
 *       Fun:   egEgtBuildS5UpdBreReqMsg
 *
 *       Desc:  This function is used to Build Update Bearer Request message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5UpdBreReqMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5UpdBreReqMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5UpdBreReqMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5UpdBreReqMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UB_REQ, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_UB_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_IMSI_IETYPE:
            {
               /*****************
                * Add IMSI code *
                ****************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_BEARER_CNTX_IETYPE:
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PROCEDURE_TID_IETYPE:
            {
               /*********************************
                * Add Procedure Transaction     *
                ********************************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PCO_IETYPE:        
            {
               /*   Add Protocol configuration option */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_AMBR_IETYPE:
            {
               /*   Add Aggregate Maximum Bit Rate */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_TRACE_INFO_IETYPE:
            {
               /*   Add Trace Information */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTrcIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5UpdBreReqMsg     */ 
/*
 *
 *       Fun:   egEgtBuildS5UpdBreRspMsg
 *
 *       Desc:  This function is used to Build Update Bearer Response message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5UpdBreRspMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5UpdBreRspMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5UpdBreRspMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5UpdBreRspMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UB_RSP, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_UB_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PCO_IETYPE:        
            {
               /*   Add Protocol configuration option */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /* egEgtBuildS5UpdBreRspMsg */ 
/*
 *
 *       Fun:   egEgtBuildS5DeActvBreCmdMsg
 *
 *       Desc:  This function is used to Build Deactivate Bearer Command
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DeActvBreCmdMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DeActvBreCmdMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5DeActvBreCmdMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildDeActvBreCmdMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_CMND, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DB_CMND);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /*  egEgtBuildS5DeActvBreCmdMsg     */ 
/*
 *
 *       Fun:   egEgtBuildS5DeActvBreFlrIndMsg
 *
 *       Desc:  This function is used to Build Deactivate Bearer Failure Indication
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5DeActvBreFlrIndMsg
(
 CmXtaTCCb        *tcCb,
 CmXtaSpCb        *spCb,
 EgMsg **egMsg,
 Mem         *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DeActvBreFlrIndMsg(tcCb,spCb,egMsg,memInfo)
   CmXtaTCCb        *tcCb;
   CmXtaSpCb        *spCb;
   EgMsg **egMsg;
   Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS5DeActvBreFlrIndMsg)

      if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
      {
         CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS5DeActvBreFlrIndMsg() : Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_FAIL_IN, EGT_GTP_INT_S5_S8);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DB_FAIL_IN);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:         
            { 
               /*************
                * Add Cause *
                ************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }    
         case EG_BEARER_CNTX_IETYPE:              
            {
               /*   Add Bearer Context */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_RECOVERY_IETYPE:                
            {
               /*   Add Recovery */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE:              
            {
               /*   Add Private Extension */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
         default:
            {
               break;
            }
      } /*end of switch  */
   } /* end of for */

}  /* egEgtBuildS5DeActvBreFlrIndMsg     */




#ifdef ANSI
PUBLIC Void  egEgtBuildS5ChgNtfReqMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ChgNtfReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5ChgNtfReqMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5ChgNtfReqMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CN_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CN_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5ChgNtfReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_IMSI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_ULI_IETYPE:        
               {
                  cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddUsrLocInfoIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
                  break;
               }

#if defined(EG_REL_930) || defined(EG_REL_AC0)
            case EG_UCI_IETYPE:        
               {
                  cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddUciIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
                  break;
               }
#endif
#ifdef EG_REL_AC0
            case EG_INDICATION_IETYPE:           
               {
                  cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddIndicationIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
                  break;
               }
            case EG_MEI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddMeiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
#endif
            case EG_RAT_TYPE_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddRatTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
            case EG_EBI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
#endif

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}



#ifdef ANSI
PUBLIC Void  egEgtBuildS5ChgNtfRspMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ChgNtfRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5ChgNtfRspMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5ChgNtfRspMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CN_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CN_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5ChgNtfRspMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_IMSI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

#ifdef EG_REL_AC0
            case EG_MEI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddMeiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
#endif

            case EG_CNG_RPT_ACT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCngRptActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
            case EG_CSG_INFO_REP_ACT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCsgInfoRepActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
#endif
            case EG_CAUSE_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}




#ifdef ANSI
PUBLIC Void  egEgtBuildS5TrcSesActMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5TrcSesActMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5TrcSesActMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5TrcSesActMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_TRC_SESS_ACTVN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_TRC_SESS_ACTVN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5TrcSesActMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_IMSI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_MEI_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddMeiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */
            case EG_TRACE_INFO_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}


#ifdef ANSI
PUBLIC Void  egEgtBuildS5TrcSesDeactMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5TrcSesDeactMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5TrcSesDeactMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5TrcSesDeactMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_TRC_SESS_DEACTVN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_TRC_SESS_DEACTVN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5TrcSesDeactMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_TRACE_REF_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddTraceRefIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}




#ifdef ANSI
PUBLIC Void  egEgtBuildS5StopPagingIndMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5StopPagingIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5StopPagingIndMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5StopPagingIndMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_STOP_PAGING_IND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_STOP_PAGING_IND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5StopPagingIndMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}



#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelPdnConsetReqMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelPdnConsetReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5DelPdnConsetReqMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelPdnConsetReqMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DPDN_CON_SET_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DPDN_CON_SET_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelPdnConsetReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_CSID_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCsidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}


#ifdef ANSI
PUBLIC Void  egEgtBuildS5DelPdnConsetRspMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5DelPdnConsetRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5DelPdnConsetRspMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelPdnConsetRspMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DPDN_CON_SET_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DPDN_CON_SET_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5DelPdnConsetRspMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_CAUSE_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}




#ifdef ANSI
PUBLIC Void  egEgtBuildS5UpdPdnConsetReqMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5UpdPdnConsetReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5UpdPdnConsetReqMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5UpdPdnConsetReqMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UPD_PDN_CON_SET_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5UpdPdnConsetReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_CSID_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCsidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}


#ifdef ANSI
PUBLIC Void  egEgtBuildS5UpdPdnConsetRspMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5UpdPdnConsetRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;
   U8             noIEs= FALSE;
   Bool           unknownIe=FALSE;

   TRC2(egEgtBuildS5UpdPdnConsetRspMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5UpdPdnConsetRspMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
         (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UPD_PDN_CON_SET_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS5UpdPdnConsetRspMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_CAUSE_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            case EG_PRIVATE_EXT_IETYPE:
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
                  break;
               } /* end of case */

            default:
               {
                  break;
               } /* end of case */
         } /*end of switch  */
      } /* end of for */
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}


/*
 *
 *       Fun:   egEgtBuildS5SuspendNotMsg
 *
 *       Desc:  This function is used to Build Suspend Notification message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5SuspendNotMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS11SuspendNotMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;

   EG_TRC2(egEgtBuildS5SuspendNotMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap,
                  "egEgtBuildS11SuspendNotMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S5_SUSPENDNOT_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS11SuspendNotMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S5_SUSPENDNOT_CMD_CODE);
   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case  EG_IMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case  EG_PRIVATE_EXT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         default:
            {
               break;
            } /* end of case */
      } /*end of switch  */
   } /* end of for */

   RETVOID;
} /* end of egEgtBuildS5SuspendNotMsg */

/*
 *
 *       Fun:   egEgtBuildS11SuspendAckMsg
 *
 *       Desc:  This function is used to Build Suspend Acknowledgement message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5SuspendAckMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5SuspendAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;

   EG_TRC2(egEgtBuildS5SuspendAckMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap,
                  "egEgtBuildS11SuspendAckMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S5_SUSPENDACK_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS11SuspendAckMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S5_SUSPENDACK_CMD_CODE);
   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case  EG_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case  EG_PRIVATE_EXT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         default:
            {
               break;
            } /* end of case */
      } /*end of switch  */
   } /* end of for */

   RETVOID;
} /* end of egEgtBuildS5SuspendAckMsg */


/*
 *  
 *       Fun:   egEgtBuildS5ResumeNotMsg
 *
 *       Desc:  This function is used to Build Resume Notification message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s5utl.c
 *  
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5ResumeNotMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ResumeNotMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;

   EG_TRC2(egEgtBuildS5ResumeNotMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap,
                  "egEgtBuildS5ResumeNotMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S5_RESUMENOT_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS5ResumeNotMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S5_RESUMENOT_CMD_CODE);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case  EG_IMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case  EG_PRIVATE_EXT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         default:
            {
               break;
            } /* end of case */
      } /*end of switch  */
   } /* end of for */

   RETVOID;
} /* end of egEgtBuildS5ResumeNotMsg */



/*
 *
 *       Fun:   egEgtBuildS5ResumeAckMsg
 *
 *       Desc:  This function is used to Build Resume Acknowledgement message
 *
 *       Ret:   None
 *
 *       Notes: None
 *
 *       File:  smeg_s11utl.c
 *
 */

#ifdef ANSI
PUBLIC Void  egEgtBuildS5ResumeAckMsg
(
 CmXtaTCCb         *tcCb,
 CmXtaSpCb         *spCb,
 EgMsg             **egMsg,
 U8                intfType,
 Mem               *memInfo
 )
#else
PUBLIC Void egEgtBuildS5ResumeAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
   CmXtaTCCb         *tcCb;
   CmXtaSpCb         *spCb;
   EgMsg             **egMsg;
   U8                intfType;
   Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo  egLvlInfo;
   LegMsgEntry    *gmEntry= NULLP;
   U16            idx = 0;

   EG_TRC2(egEgtBuildS5ResumeAckMsg)

      if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
      {
         CMXTA_DBG_ERR((_cmxtap,
                  "egEgtBuildS11ResumeAckMsg: Unable to allocate memory for EGTP message\n"));
         RETVOID;
      }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S5_RESUMEACK_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS11ResumeAckMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S5_RESUMEACK_CMD_CODE);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case  EG_CAUSE_IETYPE:
            {

               /* Add Cause code */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         case  EG_PRIVATE_EXT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

         default:
            {
               break;
            } /* end of case */
      } /*end of switch  */
   } /* end of for */

   RETVOID;
} /* end of egEgtBuildS5ResumeAckMsg */

#endif
/**********************************************************************

         End of file:     smeg_s5utl.c@@/main/3 - Fri Feb  4 18:18:27 2011

**********************************************************************/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      an              1. Initial Release
/main/2      ---      akulkarni          1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3     eg003.201 psingh             1. Merged code from eg006.102 to Fix TRACE5 
/main/3     eg004.201 magnihotri      1. Header files added for eGTPC PSF Upgrade   
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/
