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

     Sid:      smeg_s3utl.c@@/main/2 - Fri Feb  4 18:18:27 2011

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
/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#endif

#include "cm_dns.h"
#include "cm_xta.h"

#include "egac_acc.h"
#include "eg_util.h"       /* Egtpeter Utility library */
#include "egt.h"           /* Egtpeter Upper Interface */
#include "eg.h"           /* Egtpeter Upper Interface */
#include "eg_edm.h"
#include "egac_edm.h"           /* Egtpeter Upper Interface */
#include "leg.h"           /* Egtpeter Layer Manager   */
#include "eg_dbs3.h"           /* Egtpeter Layer Manager   */
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
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"
#include "cm_xta.x"
#include "egac_acc.x"
#include "egac_edm.x"
#include "eg_util.x"       /* Eg Utility library */
#include "eg_dbutl.x" 
#include "eg_dbs3.x" 
#include "smeg_ieutl.x" 


#ifdef EGTP_C

/*
*
*       Fun:   egEgtBuildS3FwdRelReqMsg
*
*       Desc:  This function is used to Build Forward Relocation Request message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s3utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS3FwdRelReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3FwdRelReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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
   U8 pdnRepeat = TRUE;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS3FwdRelReqMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelReqMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_FWDRELREQ_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelReqMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EG_S3_FWDRELREQ_CMD_CODE);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_IMSI_IETYPE:
         {
            /* Add IMSI */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         } /* end of case */
         /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
         case EG_MSISDN_IETYPE:
         {
            /* Add IMSI */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddMsisdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         } /* end of case */

            case EG_UCI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUciIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

         case EG_ADD_FLAG_FOR_SRVCC_IETYPE:
         {
            /*   Add F-TEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFlagSRVCCIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
         case EG_ADD_MM_CTXT_FOR_SRVCC_IETYPE:
         {
            /*   Add F-TEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddAddMBMSMMSRVCCIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
#endif
         case EG_FTEID_IETYPE:
         {
            /*   Add F-TEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

            case EG_RFSP_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddRfspIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
         case EG_FQDN_IETYPE:
           {
              cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

           /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
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
#if defined(EG_REL_930) || defined(EG_REL_AC0)
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

         case EG_PDN_CONN_IETYPE:
         {
            /* Add EPS Security Context Quadruplets Quintuplets */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddPdnConnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"pdnRepeat",  &pdnRepeat);
            if(pdnRepeat)
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               egLvlInfo.ieInfo[0].occrPres = TRUE;
               egLvlInfo.ieInfo[0].occr = 2;

               /* Common function which frame message based on instance value */
              makeAndAddPdnConnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            }

            break;
         } /* end of case */

         case EG_GSM_KEY_TRIP_IETYPE:
         {
            /*   Add GSM Key Triplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddGsmKeyTripIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
         {
            /* Add UMTS Key Cipher Quintuplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_GSM_KEY_CIPH_QUIN_IETYPE:
         {
            /* Add GSM Key Cipher Quintuplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
          case EG_UMTS_KEY_QUAD_QUIN_IETYPE:
          {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuadQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */


         case EG_UMTS_KEY_QUIN_IETYPE:
         {
            /* Add UMTS Key Quintuplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_EPS_SEC_QUAD_QUIN_IETYPE:
         {
            /* Add EPS Security Context Quadruplets Quintuplets */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddEpsSecQdQtIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_INDICATION_IETYPE:
         {
            /*   Add Indication */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_F_CONTAINER_IETYPE:
         {
            /*   Add F-Container */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFContainerIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_TARGET_ID_IETYPE:
         {
            /*   Add Target Id */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddTargetIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_IPADDR_IETYPE:
         {
            /* Add IP Address */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_F_CAUSE_IETYPE:
         {
            /* Add F-Cause */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
         case EG_SEL_PLMN_ID_IETYPE:
         {
            /*   Add Selected PLMN Id */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddSelPlmnIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
         case EG_RECOVERY_IETYPE:
         {
            /*   Add Recovery */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         }

#if defined(EG_REL_930) || defined(EG_REL_AC0)
         case EG_CSG_ID_IETYPE:
         {
            /*   Add Recovery */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCsgIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         }
#endif
#ifdef EG_REL_AC0
         case EG_STN_SR_IETYPE:
         {
            /*   Add STN_SR */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddSTNSRIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         }
         case EG_LDN_IETYPE:
         {
            /*   Add LDN */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         }
#endif
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
} /* end of egEgtBuildS3FwdRelReqMsg */ 


/*
*
*       Fun:   egEgtBuildS3FwdRelRspMsg
*
*       Desc:  This function is used to Build Forward Relocation Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s3utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS3FwdRelRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3FwdRelRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS3FwdRelRspMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelRspMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_FWDRELRSP_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelRspMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EG_S3_FWDRELRSP_CMD_CODE);

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

#ifdef EG_REL_AC0
         case EG_FQDN_IETYPE:
           {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
#endif
         case EG_FTEID_IETYPE:
         {
            /*   Add F-TEID */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_INDICATION_IETYPE:
         {
            /*   Add Indication flags */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_BEARER_CNTX_IETYPE:
         {
            /*   Add Bearer Context */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddBrerCntxIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_F_CAUSE_IETYPE:
         {
            /* Add F-Cause */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_F_CONTAINER_IETYPE:
         {
            /*   Add F-Container */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddFContainerIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */
         case EG_GSM_KEY_TRIP_IETYPE:
         {
            /*   Add GSM Key Triplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddGsmKeyTripIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
         {
            /* Add UMTS Key Cipher Quintuplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_GSM_KEY_CIPH_QUIN_IETYPE:
         {
            /* Add GSM Key Cipher Quintuplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_UMTS_KEY_QUIN_IETYPE:
         {
            /* Add UMTS Key Quintuplet */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_EPS_SEC_QUAD_QUIN_IETYPE:
         {
            /* Add EPS Security Context Quadruplets Quintuplets */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddEpsSecQdQtIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

/* until the issue of grouped under grouped is solved, mask this IE */
#ifdef EG_REL_AC0
         case  EG_LDN_IETYPE:
         {
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

#endif
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
} /* end of egEgtBuildS3FwdRelRspMsg */ 


/*
*
*       Fun:   egEgtBuildS3FwdRelCmpNtfMsg
*
*       Desc:  This function is used to Build Forward Relocation Complete
*              Notification message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s3utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS3FwdRelCmpNtfMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3FwdRelCmpNtfMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS3FwdRelCmpNtfMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelCmpNtfMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_FWDRELCMPNTF_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelCmpNtfMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EG_S3_FWDRELCMPNTF_CMD_CODE);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_INDICATION_IETYPE:
         {
            /*   Add Indication flags */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

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
} /* end of egEgtBuildS3FwdRelCmpNtfMsg */ 


/*
*
*       Fun:   egEgtBuildS3FwdRelCmpAckMsg
*
*       Desc:  This function is used to Build Forward Relocation Complete
*              Notification message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s3utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS3FwdRelCmpAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3FwdRelCmpAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS3FwdRelCmpAckMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelCmpAckMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_FWDRELCMPACK_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3FwdRelCmpAckMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EG_S3_FWDRELCMPACK_CMD_CODE);

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

         case EG_RECOVERY_IETYPE:
         {
            /*   Add Recovery */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         }

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
} /* end of egEgtBuildS3FwdRelCmpAckMsg */ 


/*
*
*       Fun:   egEgtBuildS3DetNtfMsg
*
*       Desc:  This function is used to Build Detach Notification message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s3utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS3DetNtfMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3DetNtfMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS3DetNtfMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3DetNtfMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_DETNTF_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3DetNtfMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EG_S3_DETNTF_CMD_CODE);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_IMSI_IETYPE:
         {
            /* Add IMSI */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddImsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         } /* end of case */

         case EG_CAUSE_IETYPE:
         {
            /* Add Cause */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         } /* end of case */
#ifdef EG_REL_AC0
         case EG_DETACH_TYPE_IETYPE:
         {
            /* Add Detach type*/
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddDetachTypeIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
            break;
         } /* end of case */
#endif

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
} /* end of egEgtBuildS3DetNtfMsg */ 


/*
*
*       Fun:   egEgtBuildS3DetAckMsg
*
*       Desc:  This function is used to Build Detach Acknowledge message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s3utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS3DetAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3DetAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egEgtBuildS3DetAckMsg)

   if (EgUtilAllocGmMsg(egMsg, memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3DetAckMsg: Unable to allocate memory for EGTP message\n"));

      RETVOID;
   }

   if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_DETACK_CMD_CODE, intfType)) == NULLP)
   {
      CMXTA_DBG_ERR((_cmxtap,
                    "egEgtBuildS3DetAckMsg: Unable to retrieve signature\n"));

      RETVOID;
   }

   cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &( (*egMsg)->msgHdr), EG_S3_DETACK_CMD_CODE);

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

         case EG_RECOVERY_IETYPE:
         {
            /*   Add Recovery */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddRecoveryIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
         }

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
} /* end of egEgtBuildS3DetAckMsg */





#ifdef ANSI
PUBLIC Void  egEgtBuildS3IdentReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3IdentReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3IdentReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3IdentReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_IDENTREQ_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_IDENTREQ_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3IdentReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_SIG_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiSigIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_IPADDR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PORT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPortIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_HOP_CNTR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddHopCntrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS3IdentRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3IdentRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3IdentRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3IdentRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_IDENTRSP_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_IDENTRSP_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3IdentRspMsg: Unable to message signature\n"));
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

            case EG_UMTS_KEY_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GSM_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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

            case EG_TRACE_INFO_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTrcIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS3CtxReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3CtxReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3CtxReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CtxReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_CTXREQ_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_CTXREQ_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CtxReqMsg: Unable to message signature\n"));
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
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_SIG_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiSigIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_COMP_REQ_MSG_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCompReqMsgIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PORT_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPortIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_HOP_CNTR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddHopCntrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_SERVING_NWK_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServNwIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

#ifdef EG_REL_AC0
            case EG_FQDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS3CtxRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3CtxRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3CtxRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CtxRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_CTXRSP_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_CTXRSP_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CtxRspMsg: Unable to message signature\n"));
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

            case EG_UMTS_KEY_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GSM_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGsmKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_CIPH_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyCiphQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_UMTS_KEY_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUmtsKeyQuadQuinIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EPS_SEC_QUAD_QUIN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEpsSecQdQtIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_FQDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
            case EG_FTEID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFTeidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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

            case EG_IPADDR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIpAddrIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
           case EG_PDN_CONN_IETYPE:
           {
            /* Add EPS Security Context Quadruplets Quintuplets */
            cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddPdnConnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);

            break;
           } /* end of case */
#ifdef EG_REL_AC0
            case EG_LDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddLdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
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
PUBLIC Void  egEgtBuildS3CtxAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3CtxAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3CtxAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CtxAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_CTXACK_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_CTXACK_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CtxAckMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3RelCancelReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3RelCancelReqMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3RelCancelReqMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3RelCancelReqMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_RELCANCELREQ_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_RELCANCELREQ_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3RelCancelReqMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_F_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

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
PUBLIC Void  egEgtBuildS3RelCancelRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3RelCancelRspMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3RelCancelRspMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3RelCancelRspMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_RELCANCELRSP_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_RELCANCELRSP_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3RelCancelRspMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3CsPageIndMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3CsPageIndMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3CsPageIndMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CsPageIndMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_CSPAGEIND_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_CSPAGEIND_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3CsPageIndMsg: Unable to message signature\n"));
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

            case EG_FQDN_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddFqdnIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_TMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_GBL_CN_ID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddGblCnIdIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_CHNL_NEEDED_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddChnlNeededIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_EMLPP_PRIOR_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddEmlppPriorIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */


#if defined(EG_REL_930) || defined(EG_REL_AC0)
            case EG_SERV_IND_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddServIndIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS3RanInfoRlyMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3RanInfoRlyMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3RanInfoRlyMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3RanInfoRlyMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_RANINFORLY_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_RANINFORLY_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3RanInfoRlyMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3AlrtMmeNotFnMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3AlrtMmeNotFnMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3AlrtMmeNotFnMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3AlrtMmeNotFnMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_ALRTMMENOTFN_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_ALRTMMENOTFN_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3AlrtMmeNotFnMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3AlrtMmeAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3AlrtMmeAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3AlrtMmeAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3AlrtMmeAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_ALRTMMEACK_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_ALRTMMEACK_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3AlrtMmeAckMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3UeActNotFnMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3UeActNotFnMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3UeActNotFnMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3UeActNotFnMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_UEACTNOTFN_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_UEACTNOTFN_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3UeActNotFnMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3UeActAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3UeActAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3UeActAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3UeActAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_UEACTACK_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_UEACTACK_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3UeActAckMsg: Unable to message signature\n"));
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
PUBLIC Void  egEgtBuildS3SuspndNotFnMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3SuspndNotFnMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3SuspndNotFnMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3SuspndNotFnMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_SUSPNDNOTFN_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_SUSPNDNOTFN_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3SuspndNotFnMsg: Unable to message signature\n"));
         RETVOID;
      }

      for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
      {
         switch(gmEntry->ieProperties[idx].ieType)
         {
            case EG_ULI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddUsrLocInfoIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_PTMSI_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddPTmsiIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
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
PUBLIC Void  egEgtBuildS3SuspndAckMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg             **egMsg,
U8                intfType,
Mem               *memInfo
)
#else
PUBLIC Void egEgtBuildS3SuspndAckMsg(tcCb, spCb, egMsg, intfType, memInfo)
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

   TRC2(egEgtBuildS3SuspndAckMsg)

   if (EgUtilAllocGmMsg(egMsg,memInfo) != ROK)
   {
      CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3SuspndAckMsg: Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb, spCb, &((*egMsg)->msgHdr), EG_S3_SUSPNDACK_CMD_CODE);

   /* if no IEs are to be appended, only unknown will be appended */
   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);

   if (!noIEs)
   {
      if ( (gmEntry  = egGetMsgFrmGmDict(EG_S3_SUSPNDACK_CMD_CODE, intfType)) == NULLP)
      {
         CMXTA_DBG_ERR((_cmxtap, "egEgtBuildS3SuspndAckMsg: Unable to message signature\n"));
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

         End of file:     smeg_s3utl.c@@/main/2 - Fri Feb  4 18:18:27 2011

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
/main/1      ---      nk   1. Initial for eGTP 1.2 Release
/main/2      ---      pmacharla         Initial for eGTP release 2.1
/main/2     eg003.201 psingh      1. Merged code from eg006.102 to Fix TRACE5
                                     macro related issue
/main/2     eg004.201 magnihotri 1. Header files added for eGTPC PSF Upgrade
/main/3     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

