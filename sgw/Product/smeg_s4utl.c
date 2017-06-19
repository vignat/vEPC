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

     Name:    EGTP 

     Type:    C source file

     Desc:    C code for controlling acceptance tests execution.

     File:    egac_egtreg.c

     Sid:      smeg_s4utl.c@@/main/2 - Fri Feb  4 18:18:27 2011

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
#include "cm_llist.h"      /* common link list */

/*eg004.201 Added Header files for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#endif

#include "cm_xta.h"
#include "cm_dns.h"

#include "egac_acc.h"
#include "eg_util.h"       /* Egtpeter Utility library */
#include "egt.h"           /* Egtpeter Upper Interface */
#include "eg.h"           /* Egtpeter Upper Interface */
#include "eg_edm.h"
#include "egac_edm.h"           /* Egtpeter Upper Interface */
#include "leg.h"           /* Egtpeter Layer Manager   */
#include "eg_dbs4.h"           /* Egtpeter Layer Manager   */
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
/*eg004.201 Added Header files for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"
#include "cm_xta.x"
#include "egac_acc.x"
#include "egac_edm.x"
#include "eg_util.x"       /* Eg Utility library */
#include "eg_dbutl.x" 
#include "eg_dbs4.x" 
#include "smeg_ieutl.x" 


#ifdef EGTP_C

/*
*
*       Fun:   egEgtBuildS4CrtSesReqMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s4utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS4CrtSesReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4CrtSesReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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
   U8             breRepeat = FALSE;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS4CrtSesReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS4CrtSesReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CS_REQ, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS4CrtSesReqMsg: Unable to Retrieve message from Dictionary\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb, &( (*egMsg)->msgHdr), EGT_GTPC_MSG_CS_REQ);

   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);
   if(!noIEs)
   {
      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_IMSI_IETYPE:
            { 
               /* Add IMSI code */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_MSISDN_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMsisdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_UE_TIME_ZONE_IETYPE:
            {
               /* Add User location info */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_CHARGING_CHAR_IETYPE:
            { 
               /* Add IMSI code */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddChargCharIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_930) || (EG_REL_AC0)
            case EG_UCI_IETYPE:
            { 
               /* Add IMSI code */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUciIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_EBI_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
            case EG_SIG_PRIOR_IND_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSigPriorIndIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#endif
            case EG_MEI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMeiIe(tcCb, spCb, *egMsg, &egLvlInfo,gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo,gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_RAT_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRatTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_APN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAccessPntNameIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_SEL_MODE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSelModeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_PDN_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPdnTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_PAA_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPaaIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case  EG_APN_RESTRICT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddMaxApnRestIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case  EG_AMBR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddAmbrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

           case  EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case  EG_BEARER_CNTX_IETYPE: 
            {
                /*  Based on instance */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                              (Txt *)"breRepeat",  &breRepeat);
               if (breRepeat && (gmEntry->ieProperties[idx].ieInst == 1))
               {
                  cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                  egLvlInfo.ieInfo[0].occrPres = TRUE;
                  egLvlInfo.ieInfo[0].occr = 2;

                  /* Common function which frame message based on instance value */
                  makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               }

               break;
            } /* end of case */

            case  EG_TRACE_INFO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case  EG_RECOVERY_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
             }    /* end of case */

            case  EG_CSID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddCsidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#if defined(EG_REL_AC0)
            case  EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#endif

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
   } /* end of if */

   /* Handle the unknown IE here */
   cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);

   if (unknownIe)
   {
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      /* Common function which frame message based on instance value */
      makeAndAddUnknownIe(tcCb,spCb,*egMsg,&egLvlInfo);
   }

   RETVOID;
} /* end of egEgtBuildS4CrtSesReqMsg */ 


/*
*
*       Fun:   egEgtBuildS4CrtSesRspMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s4utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS4CrtSesRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4CrtSesRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS4CrtSesRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS4CrtSesRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CS_RSP, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS4CrtSesRspMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CS_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:
         {
            /* Add IMSI code */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
         case EG_CNG_RPT_ACT_IETYPE:
         {
            /*  FTEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCngRptActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

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
         case EG_FTEID_IETYPE:
         {
            /*  FTEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         case EG_CSG_INFO_REP_ACT_IETYPE:
         {
            /*  FTEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCsgInfoRepActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
#endif
         case EG_PAA_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddPaaIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_APN_RESTRICT_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddMaxApnRestIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_AMBR_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddAmbrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_PCO_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_BEARER_CNTX_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_TRACE_INFO_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_RECOVERY_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case  EG_CSID_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddCsidIe(tcCb, spCb, *egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);

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
         case  EG_EPC_TMR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               /* Common function which frame message based on instance value */
               makeAndAddEPCTimer(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#endif
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
} /* end of egEgtBuildS4CrtSesRspMsg */ 

/*
*  
*       Fun:   egEgtBuildS4ResumeNotMsg
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
PUBLIC Void  egEgtBuildS4ResumeNotMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4ResumeNotMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   EG_TRC2(egEgtBuildS4ResumeNotMsg)
if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS5ResumeNotMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S4_RESUMENOT_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS4ResumeNotMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S4_RESUMENOT_CMD_CODE);

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
#ifdef EG_REL_AC0
        case EG_EBI_IETYPE:
         {
            /****************************
        * Add Linked EPS Bearer ID *
        ***************************/
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
          break;

         }
#endif
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
} /* end of egEgtBuildS4ResumeNotMsg */

/*
*
*       Fun:   egEgtBuildS4ResumeAckMsg
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
PUBLIC Void  egEgtBuildS4ResumeAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4ResumeAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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
 EG_TRC2(egEgtBuildS4ResumeAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS4ResumeAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S4_RESUMEACK_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS4ResumeAckMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S4_RESUMEACK_CMD_CODE);

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
} /* end of egEgtBuildS4ResumeAckMsg */






/*
*
*       Fun:   egEgtBuildS4SuspendNotMsg
*
*       Desc:  This function is used to Build Suspend Notification message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s4utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS4SuspendNotMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4SuspendNotMsg(tcCb, spCb, egMsg, intfType, memInfo)
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
EG_TRC2(egEgtBuildS4SuspendNotMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS11SuspendNotMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S4_SUSPENDNOT_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS11SuspendNotMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S4_SUSPENDNOT_CMD_CODE);
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

#if defined(EG_REL_930) || defined(EG_REL_AC0)
        case EG_EBI_IETYPE:
         {
            /****************************
        * Add Linked EPS Bearer ID *
        ***************************/
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
          break;

         }
#endif
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
} /* end of egEgtBuildS4SuspendNotMsg */

/*
*
*       Fun:   egEgtBuildS4SuspendAckMsg
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
PUBLIC Void  egEgtBuildS4SuspendAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4SuspendAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   EG_TRC2(egEgtBuildS4SuspendAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS11SuspendAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S4_SUSPENDACK_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS11SuspendAckMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }
cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S4_SUSPENDACK_CMD_CODE);
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
} /* end of egEgtBuildS4SuspendAckMsg */


#ifdef EG_REL_AC0
#ifdef ANSI
PUBLIC Void  egEgtBuildS4PgwRstNotMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4PgwRstNotMsg(tcCb, spCb, egMsg, intfType, memInfo)
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
    U8 ipaddrtypeRepeat = FALSE;

   TRC2(egEgtBuildS4PgwRstNotMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4PgwRstNotMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_PGW_RST_NOTFN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_PGW_RST_NOTFN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4PgwRstNotMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_IPADDR_IETYPE:
            {

               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);

               cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"ipaddrtypeRepeat",  &ipaddrtypeRepeat);
               if(ipaddrtypeRepeat)
                 {
                    cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
                    egLvlInfo.ieInfo[0].occrPres = TRUE;
                    egLvlInfo.ieInfo[0].occr = 2;

                    /* Common function which frame message based on instance value */
                 makeAndAddIpAddrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
                 }

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
#endif


#ifdef EG_REL_AC0
#ifdef ANSI
PUBLIC Void  egEgtBuildS4PgwRstNotAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4PgwRstNotAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4PgwRstNotAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4PgwRstNotAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
    egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_PGW_RST_NOTFN_ACK);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_PGW_RST_NOTFN_ACK, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4PgwRstNotAckMsg: Unable to message signature\n"));
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
#endif

/*
*
*       Fun:   egEgtBuildS4ModBreReqMsg
*
*       Desc:  This function is used to Build Modify Bearer Request message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s4utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS4ModBreReqMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg            **egMsg,
U8               intfType,
Mem              *memInfo
)
#else
PUBLIC Void egEgtBuildS4ModBreReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg            **egMsg;
U8               intfType;
Mem              *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo egLvlInfo;
   LegMsgEntry   *gmEntry= NULLP;
   U16           idx = 0;
   U8 bearerRepeat = TRUE;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS4ModBreReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS4ModBreReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_REQ, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS4ModBreReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_MB_REQ);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_MEI_IETYPE:
         { 
            /* Add Mobile Equipment identity */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddMeiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_ULI_IETYPE:
         {
            /* Add User location info */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_UE_TIME_ZONE_IETYPE:
         {
            /* Add User location info */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
#if defined(EG_REL_930) || defined(EG_REL_AC0)
         case EG_UCI_IETYPE:
         {
            /* Add User location info */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUciIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
#endif
         case EG_SERVING_NWK_IETYPE:
         {
            /*   Add Serving Network */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_RAT_TYPE_IETYPE:
         {
            /*   Add RAT Type */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddRatTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_INDICATION_IETYPE:
         {
            /*   Add Indication flags */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_FTEID_IETYPE:
         {
            /*   Add Fteid */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_AMBR_IETYPE:
         {
            /*   Add Aggregate Maximum Bit Rate */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddAmbrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_DELAY_IETYPE:
         {
            /*   Add delay value */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddDelayIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_BEARER_CNTX_IETYPE:
         {
            /*   Add Private Extension */
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

         case  EG_RECOVERY_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
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

   RETVOID;
} /* end of egEgtBuildS4ModBreReqMsg */ 


/*
*
*       Fun:   egEgtBuildS4ModBreRspMsg
*
*       Desc:  This function is used to Build Modify Bearer Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s4utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS4ModBreRspMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg            **egMsg,
U8               intfType,
Mem              *memInfo
)
#else
PUBLIC Void egEgtBuildS4ModBreRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg            **egMsg;
U8               intfType;
Mem              *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo egLvlInfo;
   LegMsgEntry   *gmEntry= NULLP;
   U16           idx = 0;
   U8 bearerRepeat = TRUE;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS4ModBreRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS4ModBreRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_RSP, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS4ModBreRspMsg: Unable to get EGTP message signature\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_MB_RSP);

   for (idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch (gmEntry->ieProperties[idx].ieType)
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

   RETVOID;
} /* end of egEgtBuildS4ModBreRspMsg */




#ifdef ANSI
PUBLIC Void  egEgtBuildS4DelSesReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelSesReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelSesReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelSesReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DS_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DS_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelSesReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_NODE_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddNodeTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4DelSesRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelSesRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelSesRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelSesRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DS_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DS_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelSesRspMsg: Unable to message signature\n"));
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

            case EG_RECOVERY_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4ChgNtfReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4ChgNtfReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4ChgNtfReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ChgNtfReqMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ChgNtfReqMsg: Unable to message signature\n"));
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
            case EG_RAT_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRatTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
            case EG_UCI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUciIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif

#ifdef EG_REL_AC0
            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_IPADDR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4ChgNtfRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4ChgNtfRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4ChgNtfRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ChgNtfRspMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ChgNtfRspMsg: Unable to message signature\n"));
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

            case EG_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

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
PUBLIC Void  egEgtBuildS4ModBreCmdMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4ModBreCmdMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4ModBreCmdMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ModBreCmdMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_MB_CMND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_CMND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ModBreCmdMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_AMBR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

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
PUBLIC Void  egEgtBuildS4ModBreFlrIndMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4ModBreFlrIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4ModBreFlrIndMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ModBreFlrIndMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_MBFAIL_IND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MBFAIL_IND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4ModBreFlrIndMsg: Unable to message signature\n"));
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



#ifdef ANSI
PUBLIC Void  egEgtBuildS4DelBreCmdMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelBreCmdMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelBreCmdMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreCmdMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreCmdMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS4DelBreFlrIndMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelBreFlrIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelBreFlrIndMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreFlrIndMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreFlrIndMsg: Unable to message signature\n"));
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


#ifdef ANSI
PUBLIC Void  egEgtBuildS4BreResCmdMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4BreResCmdMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4BreResCmdMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4BreResCmdMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_BR_CMND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_BR_CMND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4BreResCmdMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PROCEDURE_TID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddProcedureTidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#ifdef EG_REL_AC0
           case EG_SIG_PRIOR_IND_IETYPE:
            {
               /* Add MSISDN */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSigPriorIndIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */
#endif
            case EG_FLOW_QOS_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFlowQosIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_RAT_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRatTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_TAD_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTadIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4BreResFlrIndMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4BreResFlrIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4BreResFlrIndMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4BreResFlrIndMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_BR_FAIL_IND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_BR_FAIL_IND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4BreResFlrIndMsg: Unable to message signature\n"));
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

            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PROCEDURE_TID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddProcedureTidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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

#ifdef ANSI
PUBLIC Void  egEgtBuildDwDatLnkNotAckMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildDwDatLnkNotAckMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb     *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildDwDatLnkNotAckMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildDwDatLnkNotAckMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK, intfType);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
/* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK);
   
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
         case EG_DELAY_IETYPE:                
         {
            /* Add Data Notifictaion Delay */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddDelayIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }
#ifdef EG_REL_AC0
         case EG_THROTTLING_IETYPE :
         {
            /* Add throttling  */
             cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
             makeAndAddThrottlingIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
             break;
         }

         case EG_IMSI_IETYPE :
         {
            /* Add throttling  */
             cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
             makeAndAddImsiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
             break;
         }
#endif
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

}  /*  egEgtBuildDwLnkDatNotAckMsg     */ 




#ifdef ANSI
PUBLIC Void  egEgtBuildS4DwlnkDatNotflrIndMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DwlnkDatNotflrIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DwlnkDatNotflrIndMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DwlnkDatNotflrIndMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DD_NOTFN_FAIL_IND);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DD_NOTFN_FAIL_IND, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DwlnkDatNotflrIndMsg: Unable to message signature\n"));
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
#ifdef EG_REL_AC0
            case EG_IMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_NODE_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddNodeTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4TraceSessActvMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4TraceSessActvMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4TraceSessActvMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4TraceSessActvMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4TraceSessActvMsg: Unable to message signature\n"));
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

            case EG_TRACE_INFO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_MEI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMeiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4TraceSessDeActvMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4TraceSessDeActvMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4TraceSessDeActvMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4TraceSessDeActvMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4TraceSessDeActvMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS4StopPagingIndMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4StopPagingIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4StopPagingIndMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4StopPagingIndMsg: Unable to allocate memory for EGTP message\n"));
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
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4StopPagingIndMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS4CrtBreReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4CrtBreReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4CrtBreReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtBreReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CB_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CB_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtBreReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_PROCEDURE_TID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddProcedureTidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_BEARER_CNTX_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

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
PUBLIC Void  egEgtBuildS4CrtBreRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4CrtBreRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4CrtBreRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtBreRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CB_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CB_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtBreRspMsg: Unable to message signature\n"));
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
            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UE_TIME_ZONE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4UpdBreReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4UpdBreReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4UpdBreReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4UpdBreReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_UB_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UB_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4UpdBreReqMsg: Unable to message signature\n"));
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

            case EG_PROCEDURE_TID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddProcedureTidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_AMBR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAmbrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_CNG_RPT_ACT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCngRptActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

#if defined(EG_REL_930) || defined(EG_REL_AC0)
         case EG_CSG_INFO_REP_ACT_IETYPE:
         {
            /*  FTEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCsgInfoRepActIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

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
PUBLIC Void  egEgtBuildS4UpdBreRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4UpdBreRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4UpdBreRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4UpdBreRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_UB_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UB_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4UpdBreRspMsg: Unable to message signature\n"));
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

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_RECOVERY_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UE_TIME_ZONE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4DelBreReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelBreReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelBreReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DB_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PROCEDURE_TID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddProcedureTidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

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
PUBLIC Void  egEgtBuildS4DelBreRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelBreRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelBreRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DB_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelBreRspMsg: Unable to message signature\n"));
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

            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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

            case EG_PCO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPCOIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UE_TIME_ZONE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4CrtIndDatFwdTunReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4CrtIndDatFwdTunReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4CrtIndDatFwdTunReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtIndDatFwdTunReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CIDF_TUNN_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CIDF_TUNN_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtIndDatFwdTunReqMsg: Unable to message signature\n"));
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

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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


#ifdef ANSI
PUBLIC Void  egEgtBuildS4CrtIndDatFwdTunRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4CrtIndDatFwdTunRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4CrtIindDatFwdTunRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtIindDatFwdTunRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CIDF_TUNN_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CIDF_TUNN_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4CrtIindDatFwdTunRspMsg: Unable to message signature\n"));
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

            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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



#ifdef ANSI
PUBLIC Void  egEgtBuildS4DelIndDatFwdTunReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelIndDatFwdTunReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelIndDatFwdTunReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelIndDatFwdTunReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DIDF_TUNN_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DIDF_TUNN_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelIndDatFwdTunReqMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS4DelIndDatFwdTunRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DelIndDatFwdTunRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DelIndDatFwdTunRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelIndDatFwdTunRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DIDF_TUNN_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DIDF_TUNN_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DelIndDatFwdTunRspMsg: Unable to message signature\n"));
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



#ifdef ANSI
PUBLIC Void  egEgtBuildS4RelAccBreReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4RelAccBreReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4RelAccBreReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4RelAccBreReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_RAB_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_RAB_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4RelAccBreReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_EBI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#ifdef EG_REL_AC0
            case EG_NODE_TYPE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddNodeTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS4RelAccBreRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4RelAccBreRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4RelAccBreRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4RelAccBreRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_RAB_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_RAB_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4RelAccBreRspMsg: Unable to message signature\n"));
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




#ifdef ANSI
PUBLIC Void  egEgtBuildS4DwLnkDatNotMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DwLnkDatNotMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DwLnkDatNotMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DwLnkDatNotMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DWN_DATA_NOTFN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DWN_DATA_NOTFN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DwLnkDatNotMsg: Unable to message signature\n"));
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
#ifdef EG_REL_AC0
            case EG_ARP_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddARPIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_PRIVATE_EXT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPvtExtnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#ifdef EG_REL_AC0
            case EG_EBI_IETYPE:                
            {
               /* Add LBI */
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }

            case EG_IMSI_IETYPE:
            {
               /*****************
                * Add IMSI code *
                ****************/
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
               break;
            }
#endif
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
PUBLIC Void  egEgtBuildS4DwLnkDatNotAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS4DwLnkDatNotAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS4DwLnkDatNotAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DwLnkDatnNotAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS4DwLnkDatnNotAckMsg: Unable to message signature\n"));
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
#ifdef EG_REL_AC0
            case EG_IMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_THROTTLING_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddThrottlingIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_DELAY_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddDelayIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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




#endif /* EGTP_C */
/**********************************************************************

         End of file:     smeg_s4utl.c@@/main/2 - Fri Feb  4 18:18:27 2011

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
/main/1      ---      an   1. Initial Release
/main/1      ---      akulkarni          1. Initial for eGTP 1.2 Release
/main/2      ---      pmacharla         Initial for eGTP release 2.1
/main/2     eg003.201 psingh             1. Merged code from eg006.102 to Fix 
                                        TRACE5 issue
/main/2     eg004.201 magnihotri       1.Added Header files for eGTPC PSF code 
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

