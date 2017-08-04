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

/*eg009.201:Initial Release for S2A and S2B*/
/********************************************************************20**

     Name:    EGTP 

     Type:    C source file

     Desc:    C code for controlling acceptance tests execution.

     File:    egac_egtreg.c

     Sid:      smeg_s11utl.c@@/main/3 - Fri Feb  4 18:18:26 2011

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
#include "eg_dbs11.h"           /* Egtpeter Layer Manager   */
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
#include "eg_dbs11.x" 
#include "smeg_ieutl.x" 


/* eg012.201 : flag dependencies added */
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#if defined(EG_REL_AC0) || defined(EG_S2A_SUPP)

#ifdef EGTP_C

/*
*
*       Fun:   egEgtBuildS2AS2BCrtSesReqMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BCrtSesReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
U8     intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BCrtSesReqMsg(tcCb,spCb,egMsg,intfType, memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg **egMsg;
U8     intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;
   U8                      noIEs= FALSE;
   Bool                  unknownIe=FALSE;
   EG_TRC2(egEgtBuildS2AS2BCrtSesReqMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildCrtSesReqMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CS_REQ, intfType);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_CS_REQ);

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
         case  EG_PRIVATE_EXT_IETYPE:  
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
    }   
         case  EG_LDN_IETYPE:  
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddLdnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }   

        case  EG_APCO_IETYPE:
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddApcoIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }

        case EG_CHARGING_CHAR_IETYPE:
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddChargCharIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }
         default:
         {
            break;
         }
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
    
}  /*  egEgtBuildS2AS2BCrtSesReqMsg     */ 

/*
*
*       Fun:   egEgtBuildS2AS2BCrtSesRspMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BCrtSesRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BCrtSesRspMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;
   U8     brercntxRepeat = FALSE;

   EG_TRC2(egEgtBuildCrtSesRspMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,
      "egEgtBuildS2AS2BCrtSesRspMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CS_RSP, intfType);

   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_CS_RSP);
   
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
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddFTeidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);

            cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"brercntxRepeat",  &brercntxRepeat);
            if(brercntxRepeat)
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               egLvlInfo.ieInfo[0].occrPres = TRUE;
               egLvlInfo.ieInfo[0].occr = 2;
              
               /* Common function which frame message based on instance value */
               makeAndAddFTeidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            }
            break;
         }

         case EG_PAA_IETYPE:
         {
            /* Add Proxy Info AVP    */
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
         case  EG_BEARER_CNTX_IETYPE: 
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);

            cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"brercntxRepeat",  &brercntxRepeat);
            if(brercntxRepeat)
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               egLvlInfo.ieInfo[0].occrPres = TRUE;
               egLvlInfo.ieInfo[0].occr = 2;
              
               /* Common function which frame message based on instance value */
               makeAndAddBrerCntxIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            }
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
         case  EG_PRIVATE_EXT_IETYPE:  
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddPvtExtnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }
         case  EG_LDN_IETYPE:  
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddLdnIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }   

        case  EG_APCO_IETYPE:
         {
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            /* Common function which frame message based on instance value */
            makeAndAddApcoIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }

         default:
         {
            break;
         }
      } /*end of switch  */
    } /* end of for */

}  /*  egEgtBuildS2AS2BCrtSesRspMsg     */ 

/*
*
*       Fun:   egEgtBuildS2AS2BCrtBreReqMsg
*
*       Desc:  This function is used to Create Bearer Request message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BCrtBreReqMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BCrtBreReqMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   EG_TRC2(egEgtBuildS2AS2BCrtBreReqMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BCrtBreReqMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CB_REQ, intfType);
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

}  /*  egEgtBuildS2AS2BCrtBreReqMsg     */ 

/*
*
*       Fun:   egEgtBuildS2AS2BCrtBreRspMsg
*
*       Desc:  This function is used to Create Bearer Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BCrtBreRspMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BCrtBreRspMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   EG_TRC2(egEgtBuildS2AS2BCrtBreRspMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BCrtBreRspMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_CB_RSP, intfType);
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

}  /*  egEgtBuildS2AS2BCrtBreRspMsg     */ 



/*
*
*       Fun:   egEgtBuildS2AS2BDelSesReqMsg
*
*       Desc:  This function is used to Build Delete Session Request message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BDelSesReqMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BDelSesReqMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   EG_TRC2(egEgtBuildS2AS2BDelSesReqMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BDelSesReqMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DS_REQ, intfType);
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

}  /*  egEgtBuildS2AS2BDelSesReqMsg     */ 
/*
*
*       Fun:   egEgtBuildS2AS2BDelBreReqMsg
*
*       Desc:  This function is used to Build Modify Bearer Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BDelBreReqMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BDelBreReqMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;
   U8     brercntxRepeat = FALSE;

   EG_TRC2(egEgtBuildS2AS2BDelBreReqMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BDelBreReqMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_REQ, intfType);
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
            /* Common function which frame message based on instance value */
            makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);

            cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"brercntxRepeat",  &brercntxRepeat);
            if(brercntxRepeat)
            {
               cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               egLvlInfo.ieInfo[0].occrPres = TRUE;
               egLvlInfo.ieInfo[0].occr = 2;
              
               /* Common function which frame message based on instance value */
               makeAndAddEbiIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            }
            break;
         }    
         case EG_CSID_IETYPE:                
         {
            /*   Add CSID */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCsidIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }
         case EG_CAUSE_IETYPE:         
         { 
            /*************
             * Add Cause *
             ************/
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddCauseIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
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

}  /*  egEgtBuildS2AS2BDelBreReqMsg     */

/*
*       Fun:   egEgtBuildS2AS2BDelSesRspMsg
*
*       Desc:  This function is used to Build Modify Bearer Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BDelSesRspMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BDelSesRspMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BDelSesRspMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BDelSesRspMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DS_RSP, intfType);
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

}  /*  egEgtBuildS2AS2BDelSesRspMsg     */ 
/*
*
*       Fun:   egEgtBuildS2AS2BDelBreRspMsg
*
*       Desc:  This function is used to Build Modify Bearer Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BDelBreRspMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BDelBreRspMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BDelBreRspMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BDelBreRspMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DB_RSP, intfType);
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
            printf("Unknow IE to Build the message\n");
            break;
         }
      } /*end of switch  */
    } /* end of for */

}  /*  egEgtBuildS2AS2BDelBreRspMsg     */ 



/*
*
*       Fun:   egEgtBuildS2AS2BModBreCmdMsg
*
*       Desc:  This function is used to Build Modify Bearer Command
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BModBreCmdMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BModBreCmdMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo         egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   EG_TRC2(egEgtBuildS2AS2BModBreCmdMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BModBreCmdMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MB_CMND, intfType);
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

}  /*  egEgtBuildS2AS2BModBreCmdMsg     */ 

/*
*
*       Fun:   egEgtBuildS2AS2BModBreFlrIndMsg
*
*       Desc:  This function is used to Build Modify Bearer Failure Indication
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BModBreFlrIndMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BModBreFlrIndMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BModBreFlrIndMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BModBreFlrIndMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_MBFAIL_IND, intfType);
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

}  /* egEgtBuildS2AS2BModBreFlrIndMsg     */ 

/*
*
*       Fun:   egEgtBuildS2AS2BUpdBreReqMsg
*
*       Desc:  This function is used to Build Update Bearer Request message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BUpdBreReqMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BUpdBreReqMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BUpdBreReqMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BUpdBreReqMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UB_REQ, intfType);
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
         case EG_AMBR_IETYPE:
         {
            /*   Add Aggregate Maximum Bit Rate */
            cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
            makeAndAddAmbrIe(tcCb,spCb,*egMsg,&egLvlInfo,gmEntry->ieProperties[idx]);
            break;
         }
            case EG_CSID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

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

}  /*  egEgtBuildS2AS2BUpdBreReqMsg     */ 
/*
*
*       Fun:   egEgtBuildS2AS2BUpdBreRspMsg
*
*       Desc:  This function is used to Build Update Bearer Response message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BUpdBreRspMsg
(
CmXtaTCCb        *tcCb,
CmXtaSpCb        *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BUpdBreRspMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb        *tcCb;
CmXtaSpCb        *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                    idx = 0;

   EG_TRC2(egEgtBuildS2AS2BUpdBreRspMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BUpdBreRspMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }
   
   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_UB_RSP, intfType);
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
            case EG_CSID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */

            case EG_INDICATION_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddIndicationIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            } /* end of case */
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

}  /* egEgtBuildS2AS2BUpdBreRspMsg */ 

/*
*
*       Fun:   egEgtBuildS2AS2BDelPdnConReqMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BDelPdnConReqMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
U8     intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BDelPdnConReqMsg(tcCb,spCb,egMsg,intfType, memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg **egMsg;
U8     intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo egLvlInfo;
   LegMsgEntry   *gmEntry  = NULLP;
   U16           idx       = 0;
   U8            noIEs     = FALSE;
   Bool          unknownIe = FALSE;

   EG_TRC2(egEgtBuildS2AS2BDelPdnConReqMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,"egEgtBuildS2AS2BDelPdnConReqMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DPDN_CON_SET_REQ, intfType);
   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DPDN_CON_SET_REQ);

   cmXtaXmlGetVal(NULLP, spCb->childStep, CMXTA_ATTRIB_ENUM_U8,
                  (Txt *)"noIEs",  &noIEs);
   if(!noIEs)
   {
   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CSID_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCsidIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE :
            {
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
   } /* end of if */
   /* Handle the unknown IE here */
    cmXtaXmlGetVal(NULLP,spCb->childStep, CMXTA_ATTRIB_ENUM_U8, (Txt *)"unknownIe", &unknownIe);
    if (unknownIe)
    {
        cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
        /* Common function which frame message based on instance value */
        makeAndAddUnknownIe(tcCb,spCb,*egMsg,&egLvlInfo);
    }

}  /*  egEgtBuildS2AS2BDelPdnConReqMsg     */

/*
*
*       Fun:   egEgtBuildS2AS2BDelPdnConRspMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BDelPdnConRspMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BDelPdnConRspMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{
   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BDelPdnConRspMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS2AS2BDelPdnConRspMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_DPDN_CON_SET_RSP, intfType);

   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_DPDN_CON_SET_RSP);

   for(idx = 0; idx < gmEntry->nmbIeProp; idx++)
   {
      switch(gmEntry->ieProperties[idx].ieType)
      {
         case EG_CAUSE_IETYPE:
            {
               cmMemset( (U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
               makeAndAddCauseIe(tcCb, spCb, *egMsg, &egLvlInfo, gmEntry->ieProperties[idx]);
               break;
            }
         case EG_PRIVATE_EXT_IETYPE :
            {
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

}  /*  egEgtBuildS2AS2BDelPdnConRspMsg     */

/*
*
*       Fun:   egEgtBuildS2AS2BTrcSessActMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BTrcSessActMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BTrcSessActMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{

   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BTrcSessActMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS2AS2BTrcSessActMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_TRC_SESS_ACTVN, intfType);

   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_TRC_SESS_ACTVN);

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
            }
      } /*end of switch  */
   } /* end of for */

} /*egEgtBuildS2AS2BTrcSessActMsg */


/*
*
*       Fun:   egEgtBuildS2AS2BTrcSessDeactMsg
*
*       Desc:  This function is used to Build CSR message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  smeg_s11utl.c
*
*/

#ifdef ANSI
PUBLIC Void  egEgtBuildS2AS2BTrcSessDeactMsg
(
CmXtaTCCb         *tcCb,
CmXtaSpCb         *spCb,
EgMsg **egMsg,
U8      intfType,
Mem         *memInfo
)
#else
PUBLIC Void egEgtBuildS2AS2BTrcSessDeactMsg(tcCb,spCb,egMsg,intfType,memInfo)
CmXtaTCCb         *tcCb;
CmXtaSpCb         *spCb;
EgMsg **egMsg;
U8      intfType;
Mem         *memInfo;
#endif /* ANSI */
{

   EgIeLevelInfo           egLvlInfo;
   LegMsgEntry              *gmEntry= NULLP;
   U16                     idx = 0;

   EG_TRC2(egEgtBuildS2AS2BTrcSessDeactMsg)

   if( EgUtilAllocGmMsg(egMsg,memInfo) != ROK )
   {
      CMXTA_DBG_ERR((_cmxtap,
               "egEgtBuildS2AS2BTrcSessDeactMsg() : Unable to allocate memory for EGTP message\n"));
      RETVOID;
   }

   gmEntry  = egGetMsgFrmGmDict(EGT_GTPC_MSG_TRC_SESS_DEACTVN, intfType);

   if(gmEntry == NULLP)
   {
      RETVOID;
   }

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /* Build the Header  with Default values and also by taking from XML*/
   egAcEgtBuildEgHeader(tcCb,spCb,&((*egMsg)->msgHdr),EGT_GTPC_MSG_TRC_SESS_DEACTVN);

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
            }
      } /*end of switch  */
   } /* end of for */

}/* egEgtBuildS2AS2BTrcSessDeactMsg */

#endif /* EGTP_C */

#endif /* EG_REL_AC0 || EG_S2A_SUPP */
/**********************************************************************

         End of file:     smeg_s11utl.c@@/main/3 - Fri Feb  4 18:18:26 2011

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
/main/1      eg009.201  asaurabh   1. Initial Release for S2A and S2B
/main/1      eg012.201  shpandey   1. Added flags for compilation dependencies
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

