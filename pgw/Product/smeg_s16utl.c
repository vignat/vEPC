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

     Sid:      smeg_s16utl.c@@/main/2 - Fri Feb  4 18:18:26 2011

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
/*eg004.201 Added Header Files for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#endif
#include "cm_dns.h"
#include "cm_xta.h"

#include "eg_util.h"       /* Egtpeter Utility library */
#include "egt.h"           /* Egtpeter Upper Interface */
#include "eg.h"           /* Egtpeter Upper Interface */
#include "eg_edm.h"
#include "egac_edm.h"           /* Egtpeter Upper Interface */
#include "leg.h"           /* Egtpeter Layer Manager   */
#include "eg_dbs16.h"           /* Egtpeter Layer Manager   */
#include "eg_dbutl.h"           /* Egtpeter Layer Manager   */
#include "egac_acc.h"

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
/*eg004.201 Added Header Files for eGTPC PSF Upgrade*/
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
#include "eg_dbs16.x" 
#include "smeg_ieutl.x" 


#ifdef EGTP_C

/*
*
*       Fun:   egEgtBuildIdReqMsg
*
*       Desc:  This function is used to Build Identification Request message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s16utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS16IdReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16IdReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg006.102 changes --*/
   EG_TRC2(egEgtBuildS16IdReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16IdReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_ID_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_ID_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16IdReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_GUTI_IETYPE:
            { 
               /*****************
                 * Add GUTI     *
                 ****************/
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGutiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               /* Add ULI */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_PTMSI_IETYPE:
            {
               /* Add P-TMSI */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_PTMSI_SIG_IETYPE:
            {
               /* Add P-TMSI Signature */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiSigIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_COMP_REQ_MSG_IETYPE:
            {
               /* Add Complete Request Message */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCompReqMsgIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_IPADDR_IETYPE:
            {
               /* Add IP Address */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_PORT_IETYPE:
            {
               /* Add Port */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPortIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case EG_HOP_CNTR_IETYPE:
            {
               /* Add Hop Counter */
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddHopCntrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

               break;
            } /* end of case */

            case  EG_PRIVATE_EXT_IETYPE:  
            {
               /* Add Private Extension */
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
} /* end of egEgtBuildS16IdReqMsg */


/*
*
*       Fun:   egEgtBuildS16IdRspMsg
*
*       Desc:  This function is used to Build Identification Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s16utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS16IdRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16IdRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg             **egMsg;
U8                intfType;
Mem               *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo   egLvlInfo;
   LegMsgEntry     *gmEntry= NULLP;
   U16             idx = 0;

   /*-- eg006.102 changes --*/
   EG_TRC2(egEgtBuildS16IdRspMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS16IdRspMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_ID_RSP, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS16IdRspMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EGT_GTPC_MSG_ID_RSP);

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

         case EG_IMSI_IETYPE:
         {
            /* Add IMSI */
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
            case EG_GSM_KEY_TRIP_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyTripIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GSM_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EPS_SEC_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEpsSecQdQtIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuadQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

         case  EG_PRIVATE_EXT_IETYPE:
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

   RETVOID;
} /* end of egEgtBuildS16IdRspMsg */




#ifdef ANSI
PUBLIC Void  egEgtBuildS16CtxReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16CtxReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16CtxReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16CtxReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CNTXT_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ((gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CNTXT_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16CtxReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_IMSI_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_SIG_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiSigIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_COMP_REQ_MSG_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCompReqMsgIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_FTEID_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PORT_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPortIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_RAT_TYPE_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRatTypeIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_HOP_CNTR_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddHopCntrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
            case EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_PRIVATE_EXT_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
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
      makeAndAddUnknownIe(tcCb, spCb, *egMsg, &egLvlInfo);
   }

   RETVOID;
}



#ifdef ANSI
PUBLIC Void  egEgtBuildS16CtxRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16CtxRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16CctxRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16CctxRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CNTXT_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CNTXT_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16CctxRspMsg: Unable to message signature\n"));
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

            case EG_IMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GSM_KEY_TRIP_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyTripIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GSM_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EPS_SEC_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEpsSecQdQtIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuadQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PDN_CONN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPdnConnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_FQDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_TRACE_INFO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
            case EG_UCI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUciIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UE_TIME_ZONE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_IPADDR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#ifdef EG_REL_AC0
            case EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_RFSP_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRfspIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16CtxAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16CtxAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16CtxAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16CtxAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_CNTXT_ACK);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CNTXT_ACK, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16CtxAckMsg: Unable to message signature\n"));
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

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16FwdRelReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16FwdRelReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16FwdRelReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdRelReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_FR_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_FR_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdRelReqMsg: Unable to message signature\n"));
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

            case EG_MSISDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddMsisdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#ifdef EG_REL_AC0
            case EG_ADD_FLAG_FOR_SRVCC_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFlagSRVCCIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddAddMBMSMMSRVCCIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

#endif
            case EG_PDN_CONN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPdnConnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

#ifdef EG_REL_AC0
            case EG_UCI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUciIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_UE_TIME_ZONE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUeTimeZoneIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_FQDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_GSM_KEY_TRIP_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyTripIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GSM_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EPS_SEC_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEpsSecQdQtIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuadQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_F_CONTAINER_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFContainerIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_F_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_TARGET_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTargetIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_IPADDR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SRC_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSrcIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SEL_PLMN_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSelPlmnIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_RECOVERY_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#if defined(EG_REL_930) || defined(EG_REL_AC0)

            case EG_CSG_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsgIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_CMI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCmiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_TRACE_INFO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#ifdef EG_REL_AC0
            case EG_STN_SR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSTNSRIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_RFSP_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRfspIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16FwdRelRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16FwdRelRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16FfwdRelRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FfwdRelRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_FR_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_FR_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FfwdRelRspMsg: Unable to message signature\n"));
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

#ifdef EG_REL_AC0
            case EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_BEARER_CNTX_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_F_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_F_CONTAINER_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFContainerIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16FwdRelCmpntfMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16FwdRelCmpntfMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16FwdRelCmpntfMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdRelCmpntfMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_FR_COMP_NOTFN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_FR_COMP_NOTFN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdRelCmpntfMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16FwdAccsCtxntfMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16FwdAccsCtxntfMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16FwdAccsCtxntfMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdAccsCtxntfMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_FWD_ACC_CNTXT_NOTFN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdAccsCtxntfMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_RAB_CONTEXT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRabContextIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SRC_RNC_PDCP_CTX_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddSrcRncPdcpCtxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PDU_NUMBER_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPduNumberIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16FwdRelCmpAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16FwdRelCmpAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16FwdRelCmpAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdRelCmpAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_FR_COMP_ACK);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_FR_COMP_ACK, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdRelCmpAckMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS16FwdAccsCtxAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16FwdAccsCtxAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16FwdAccsCtxAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdAccsCtxAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_FWD_ACC_CNTXT_ACK, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16FwdAccsCtxAckMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS16RelCancelReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16RelCancelReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16RelCancelReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16RelCancelReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_RC_REQ);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_RC_REQ, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16RelCancelReqMsg: Unable to message signature\n"));
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
            case EG_F_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16RelCancelRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16RelCancelRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16RelCancelRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16RelCancelRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_RC_RSP);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_RC_RSP, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16RelCancelRspMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS16RanInforLyMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16RanInforLyMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16RanInforLyMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16RanInforLyMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_RAN_INFO_RELAY);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_RAN_INFO_RELAY, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16RanInforLyMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_F_CONTAINER_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFContainerIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_TARGET_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTargetIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16SuspndNotfnMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16SuspndNotfnMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16SuspndNotfnMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16SuspndNotfnMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_SSPND_NOTFN);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_SSPND_NOTFN, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16SuspndNotfnMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
#if defined(EG_REL_930) || defined(EG_REL_AC0)
            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUliIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS16SuspndAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS16SuspndAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS16SuspndAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16SuspndAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EGT_GTPC_MSG_SSPND_ACK);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_SSPND_ACK, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS16SuspndAckMsg: Unable to message signature\n"));
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


#endif /* EGTP_C */
/**********************************************************************

         End of file:     smeg_s16utl.c@@/main/2 - Fri Feb  4 18:18:26 2011

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
/main/1      ---      nk   1. Initial Release
/main/1      ---      rss               1. Initial for eGTP 1.2 Release
/main/2      ---      pmacharla         Initial for eGTP release 2.1
/main/2      eg006.102    pmacharla         1. Fixed TRACE5 macro related issue
/main/2      eg004.201   magnihotri         1. Added Header Files for eGTPC PSF Upgrade
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

