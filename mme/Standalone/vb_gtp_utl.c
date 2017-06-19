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

     Desc:    This file contains the GTPC messages validating and 
              creation functions

     File:    vb_mme_gtp_utl.c

     Sid:     vb_mme_gtp_utl.c@@/main/1 - Mon Oct 24 10:20:20 2011

*********************************************************************21*/
/* Header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "cm_pasn.h"       /* Common ASN.1 library            */
#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EGTP Upper Interface */
#include "leg.h"           /* EGTP LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "vb_hss.h"        /* CNE HSS Application defines    */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#include "cm_emm.h"        /* CNE EMM defines                */
#include "cm_esm.h"        /* CNE EDM defines                */
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
#include "eg_util.x"
#include "leg.x"           /* EGTP LM Interface               */

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
#include "egt_util.x"      /* EGTP utils                      */
#include "vb_gtp_utl.x"    /* VB EGTP utils                   */

EXTERN VbGUMMEI *selfGummei;

PRIVATE S16 vbMmeUtlFillGtpBitRate ARGS ((EgDatStr8 *gtpQoS, 
                                          U32       qos));

PRIVATE S16 vbMmeUtlFillEpsQoS ARGS ((U32      qos,
                                      U8      *bitFld, 
                                      U8      *bitFldExt));
/*
 *
 *       Fun:   vbMmeVldNStrCSRsp
 *
 *       Desc:  This function used to validate the Crease Session Response
 *              received from SGW and store the values in the uE context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrCSRsp
(
VbMmeUeCb   *ueCb,            /* UE control block */
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrCSRsp(ueCb, egMsg, cause)
VbMmeUeCb   *ueCb,            /* UE control block */
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
#endif
{
   EgIeLevelInfo     ieLvlInfo         = {0};
   EgIe              *egIe             = NULLP;
   U16               iePos             = 0;
   U8                count             = 0;
   U16               ieOcr             = 0;
   S16               ret               = ROK;
   VbMmeUeTunnelCb   *cpTun            = NULLP;
   VbMmeEsmCb        *esmCb            = NULLP;

   VB_TRC2(vbMmeVldNStrCSRsp)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Create Session "\
         "Response"));

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   *cause = EGT_GTPC_CAUSE_SYS_FAIL;
   cpTun = ueCb->ueTunnelCb;
   esmCb = ueCb->esmList[0];

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)", cpTun->localTeIdKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)", cpTun->localTeIdKey));
#endif

   /* Check the cause IE */
   ieLvlInfo.level                  = 0;
   ieLvlInfo.ieInfo[0].ieTypePres   = TRUE;
   ieLvlInfo.ieInfo[0].ieType       = EG_CAUSE_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst       = 0;
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
   if(ret != ROK)
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Cause IE Could Not Be "
                        "Retrieved"));
   }

	/* egIe NULLP checks has been introduced to avoid klocworks tool warning */
   if ((ret == ROK) && egIe != NULLP && (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ))
   {
      ret      = RFAILED;
      *cause = egIe->t.valUnSgnd8;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Create Session Request Not accepted"
                        "Cause Value(%d)", egIe->t.valUnSgnd8));
   }
   
   /* SGW F-TEID */
   if (ret == ROK)
   {
      ieLvlInfo.level                  = 0;
      ieLvlInfo.ieInfo[0].ieTypePres   = TRUE;
      ieLvlInfo.ieInfo[0].ieType       = EG_FTEID_IETYPE;
      ieLvlInfo.ieInfo[0].ieInst       = 0;

      ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
      if (ret != ROK)
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Sender FTEID IE Not "
                           "Retrieved"));
      }
      else
      {
			/* This check has been put to avoid klocworks warning */
			if (egIe != NULLP)
         cpTun->remoteTeId = egIe->t.fTeid.teid;
      }
   }

   /* PGW F-TEID */
   if (ROK == ret )
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "PGW S5/S8 C-FTEID IE"));
      ret = vbMmeEgtUtlGetIe(egMsg, &egIe, EG_FTEID_IETYPE, 1);
      if (ret != ROK)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "PGW S5/S8 C-FTEID IE"));
      }
      else
      {
			/* This NULLP check has been put to avoid klocworks tool warning */
			if (egIe != NULLP)
			{
				esmCb->pdnCb->s5PgwTeId = egIe->t.fTeid.teid;
				esmCb->pdnCb->s5PgwAddr.type = CM_INET_IPV4ADDR_TYPE;
				vbUtlIpv4ArrToInet((egIe->t.fTeid.ip4Addr),\
					&(esmCb->pdnCb->s5PgwAddr.u.ipv4NetAddr));
			}	
      }
   }

   /* PAA */
   if (ROK == ret) 
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting PAA IE"));
      ret = vbMmeEgtUtlGetIe(egMsg, &egIe, EG_PAA_IETYPE, 0);
      if (ROK == ret && NULLP != egIe)
      {
         ueCb->ueTunnelCb->ipAddr.type = CM_INET_IPV4ADDR_TYPE;
         ueCb->ueTunnelCb->ipAddr.u.ipv4NetAddr = 
            (U32)((egIe->t.paa.u.pdnIpv4Addr[0]) << 24) |
            (U32)((egIe->t.paa.u.pdnIpv4Addr[1]) << 16) |
            (U32)((egIe->t.paa.u.pdnIpv4Addr[2]) << 8 ) |
            (U32)((egIe->t.paa.u.pdnIpv4Addr[3]));
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting PAA IE"));
      }
   }

   /* Bearer Context to be Created */
   if (ROK == ret)
   {
      cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
      ieLvlInfo.level = 0;
      ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
      ieLvlInfo.ieInfo[0].ieInst = 0;
    
      /* Get the number of Bearer ID IEs */
      ret = EgUtilGmGetIeOccrCnt(egMsg, &ieLvlInfo, &ieOcr);
      for(count=0; count < ieOcr; count++ )
      {
         /* Get the EBI Id */
         cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
         egIe = NULLP;
         ieLvlInfo.level = 1;
         ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[0].occrPres = TRUE;
         ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
         ieLvlInfo.ieInfo[0].ieInst = 0;
         ieLvlInfo.ieInfo[0].occr = count + 1;
   
         /* EPS Bearer Id */
         ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[1].ieType = EG_EBI_IETYPE;
         ieLvlInfo.ieInfo[1].ieInst = 0;
   
         ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
         if(ret != ROK)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "EBI IE Could Not Be "
                           "Retrieved"));
            ret = RFAILED;
            break;
         }

			/* This check has been put to avoid klocworks tool warning */
			if (egIe != NULLP)
			{
				/* EBI is of type U8 */
				ret = vbMmeUtlFndEsmCb(ueCb, &esmCb, egIe->t.valUnSgnd8, 0);
			}

			/* This check has been put to avoid klocworks tool warning */
         if(ROK !=ret && egIe != NULLP)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid EBI(%d)",
                              egIe->t.valUnSgnd8));

            *cause = EGT_GTPC_CAUSE_INV_RPL_FRM_PEER;
            RETVALUE(RFAILED);
         }

         /* For each occurence get the FTEID */
         cmMemset((U8*)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
         egIe = NULLP;
         ieLvlInfo.level = 1;
         ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[0].occrPres = TRUE;
         ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
         ieLvlInfo.ieInfo[0].ieInst = 0;
         ieLvlInfo.ieInfo[0].occr = count+1;
         /* S1-U SGW FTEID */
         ieLvlInfo.ieInfo[1].ieTypePres = TRUE;
         ieLvlInfo.ieInfo[1].ieType = EG_FTEID_IETYPE;
         ieLvlInfo.ieInfo[1].ieInst = 0;

         ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
         if(ret != ROK)
         {
            ret = RFAILED;
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Getting SGW U F-TEID IE"));
            RETVALUE(ret);
         }

			/* This check has been put to avoid klocworks warning */
			if (egIe != NULLP)
			{
				esmCb->s1SgwTeId = egIe->t.fTeid.teid;
				esmCb->s1SgwAddr.type = CM_INET_IPV4ADDR_TYPE;
				esmCb->s1SgwAddr.u.ipv4NetAddr = 
            (U32)(egIe->t.fTeid.ip4Addr[0] << 24) |  
            (U32)(egIe->t.fTeid.ip4Addr[1] << 16) | 
            (U32)(egIe->t.fTeid.ip4Addr[2] << 8) |
            (U32)(egIe->t.fTeid.ip4Addr[3]);
			}
   
         /* PGW s5/s8 U FTEID */
         if ((ueCb->proc == VB_MME_ATTACH_PROC) || (esmCb->proc == VB_MME_PDN_CONN_PROC))
         {
            ieLvlInfo.ieInfo[1].ieInst = 2;
            ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
            if(ret != ROK)
            {
               ret = RFAILED;
               VB_MME_DBG_ERROR((VB_MME_PRNTBUF,"Getting PGW U F-TEID IE"));
               RETVALUE(ret);
            }

				/* This check has been put to avoid klocworks warning */
				if (egIe != NULLP)
				{
					 esmCb->s5PgwUTeid = egIe->t.fTeid.teid;
					 esmCb->s5PgwUAddr.type = CM_INET_IPV4ADDR_TYPE;
					 esmCb->s5PgwUAddr.u.ipv4NetAddr = 
						  (U32)(egIe->t.fTeid.ip4Addr[0] << 24) |  
						  (U32)(egIe->t.fTeid.ip4Addr[1] << 16) | 
						  (U32)(egIe->t.fTeid.ip4Addr[2] << 8) |
						  (U32)(egIe->t.fTeid.ip4Addr[3]);
				}
         }
      }
   }

   /* PCO */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Getting PCO IE"));
      ret = vbMmeEgtUtlGetIe(egMsg, &egIe, EG_PCO_IETYPE, 0);
      if (ROK == ret && NULLP != egIe)
      {
         ret = vbUtlArrToProtoCfgOpt(egIe->t.valStr16.val,\
               (U8)egIe->t.valStr16.length, &esmCb->pco);
      }
      else
      {
         ret = ROK;
         esmCb->pco.pres = FALSE;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Getting PCO IE"));
      }
   }


   RETVALUE(ret);
}


/*
 *
 *       Fun:   vbMmeVldNStrMBRsp
 *
 *       Desc:  This function used to validate the Modify Bearer Response
 *              received from SGW and store the values in the uE context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrMBRsp
(
VbMmeUeCb   *ueCb,            /* UE control block */
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8         *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrMBRsp(ueCb, egMsg, cause)
VbMmeUeCb   *ueCb,            /* UE control block */
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
#endif
{
   EgIeLevelInfo     ieLvlInfo         = {0};
   EgIe              *egIe             = NULLP;
   U16               iePos             = 0;
   S16               ret               = ROK;
   U32               ipAddr            = 0;

   VB_TRC2(vbMmeVldNStrMBRsp)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Validating and Storing Modify Bearer "
         "Response"));

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   *cause = VB_ESM_INV_CAUSE;
#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)",
                  ueCb->ueTunnelCb->localTeIdKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)",
                  ueCb->ueTunnelCb->localTeIdKey));
#endif                  

   /* Check the cause IE */
   ieLvlInfo.level                  = 0;
   ieLvlInfo.ieInfo[0].ieTypePres   = TRUE;
   ieLvlInfo.ieInfo[0].ieType       = EG_CAUSE_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst       = 0;
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
   if(ret != ROK)
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "CAUSE IE Could Not be "
                        "Retrieved"));
   }

   /* egIe != NULLP check has been put to avoid klocworks tool warning */
   if ((ret == ROK) && egIe != NULLP && 
      (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ))
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Modify Bearer Request Not Accepted"
                        "Cause Value(%d)", egIe->t.valUnSgnd8));
   }

   /* This is enuff for Initial attach. Update the function when handling
    * for TAU/HO
    */

   if( (ueCb->pHoData) && (ueCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE) )
   { 
      /* EBI IE */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB DL FTEID IE "\
                  "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(egMsg, 0, 1, EG_EBI_IETYPE, 0,\
            &egIe);
      if (ROK == ret && egIe != NULLP)
      {
         if( ueCb->esmList[0]->bId != egIe->t.valUnSgnd8 )
         {
            ret = RFAILED;
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Mismatching Bearer ID "\
               "in Bearer Context IE"));
         }
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB DL FTEID IE "\
               "in Bearer Context IE"));
      }

      /* CAUSE */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB DL FTEID IE "\
                  "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(egMsg, 0, 1, EG_CAUSE_IETYPE, 0,\
            &egIe);
      if ((ret == ROK) && egIe != NULLP && 
         (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ))
      {
         ret = RFAILED;
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Modify Bearer Request Not Accepted"
            "for BID (%d)Cause Value(%d)",ueCb->esmList[0]->bId,
            egIe->t.valUnSgnd8));
      }
      /* S1-U FTEID */
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving ENB DL FTEID IE "\
                  "in Bearer Context IE"));
      ret = vbMmeEgtUtlGetBCtxIe(egMsg, 0, 1, EG_FTEID_IETYPE, 2,\
            &egIe);
      if (ROK == ret && egIe != NULLP)
      {
         if((ueCb->esmList[0]->s4SgwUTeid == egIe->t.fTeid.teid)&&
            (ueCb->esmList[0]->s4SgwUAddr.type == egIe->t.fTeid.isIPv4Pres))
         {
            vbMmeEgtUtlIpv4ArrToInet(egIe->t.fTeid.ip4Addr,&ipAddr);
            if(ueCb->esmList[0]->s4SgwUAddr.u.ipv4NetAddr != ipAddr)
            {
               ret = RFAILED;
               VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Mismatching S-GW IP address "\
               "in Bearer Context IE"));
            }
         }
         else
         {
            ret = RFAILED;
            VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Mismatching S-GW UL FTEID "\
               "in Bearer Context IE"));
         }
      }
      else
      {
         ret = RFAILED;
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Retrieving S-GW UL FTEID IE "\
               "in Bearer Context IE"));
      }        
   }
   
   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeVldNStrCrtBrReq
 *
 *       Desc:  This function used to validate the Create Bearer Req
 *              received from SGW and store the values in the uE context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrCrtBrReq
(
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrCrtBrReq(ueCb, esmCb, egMsg, cause)
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
#endif
{
   S16 ret                    = ROK;
   EgIeLevelInfo    ieLvlInfo = {0};
   EgIe             *egIe     = NULLP;
   U16              iePos     = 0;
  
   VB_TRC2(vbMmeVldNStrCrtBrReq)

   /* Overwrite the TFT Parameter as received in this message */
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieType = EG_BEARER_CNTX_IETYPE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst = 0;
   ieLvlInfo.ieInfo[ieLvlInfo.level].occrPres = TRUE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].occr = 1;

   ieLvlInfo.level = 1;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieType = EG_BEARER_TFT_IETYPE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst = 0; /* EBI's Instance */
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);

   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "TFT Retrieval Failed"));
      *cause = EGT_GTPC_CAUSE_SYS_FAIL;
      RETVALUE(RFAILED);
   }

	/* This check has been put to avoid klocworks warning */
	if (egIe != NULLP)
	{
		esmCb->tft.len = egIe->t.valStr.length;
		cmMemcpy((U8 *)esmCb->tft.buf, (U8 *)egIe->t.valStr.val,  egIe->t.valStr.length);
	}

   /* Get the teId which will be forwarded later on E-RAB Set up Req */
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieType = EG_BEARER_CNTX_IETYPE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst = 0;
   ieLvlInfo.ieInfo[ieLvlInfo.level].occrPres = TRUE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].occr = 1;

   ieLvlInfo.level = 1;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieType = EG_FTEID_IETYPE;
   ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst = 0; /* EBI's Instance */
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
   if(ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "FTEID Retrieval Failed"));
      *cause = EGT_GTPC_CAUSE_SYS_FAIL;
      RETVALUE(RFAILED);
   }

	/* This check has been put to avoid klocworks warning */
	if (egIe != NULLP)
	{
		esmCb->s1SgwTeId =  egIe->t.fTeid.teid ;
		esmCb->s1SgwAddr.type = CM_INET_IPV4ADDR_TYPE;
		esmCb->s1SgwAddr.u.ipv4NetAddr = (U32)(egIe->t.fTeid.ip4Addr[0] << 24)|
      (U32)(egIe->t.fTeid.ip4Addr[1]<< 16) |
      (U32)(egIe->t.fTeid.ip4Addr[2]<< 8) |
      (U32)(egIe->t.fTeid.ip4Addr[3]);
	}
    ieLvlInfo.ieInfo[ieLvlInfo.level].ieTypePres = TRUE;
    ieLvlInfo.ieInfo[ieLvlInfo.level].ieType = EG_BEARER_QOS_IETYPE;
    ieLvlInfo.ieInfo[ieLvlInfo.level].ieInst = 0;

    ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
    if (ROK != ret) {
        VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "QCI couldnt be retrieved from message  in CBReq"));
    }

    esmCb->epsQos.qci = egIe->t.bQos.qci;
    VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
           "########:TESTING QCI value %d \n", egIe->t.bQos.qci));
   RETVALUE(ret);
} 

/*
 *
 *       Fun:   vbMmeVldNStrDelBrReq
 *
 *       Desc:  This function used to validate the Delete Bearer Req
 *              received from SGW and store the values in the uE context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrDelBrReq
(
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrDelBrReq(ueCb, esmCb, egMsg, cause)
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
#endif
{
   S16 ret = ROK;
  
   VB_TRC2(vbMmeVldNStrDelBrReq)

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeVldNStrUpdBrReq
 *
 *       Desc:  This function used to validate the Update Bearer Req
 *              received from SGW and store the values in the uE context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrUpdBrReq
(
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrUpdBrReq(ueCb, esmCb, egMsg, cause)
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
#endif
{
   S16 ret = ROK;
  
   VB_TRC2(vbMmeVldNStrUpdBrReq)

   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeUtlBldCSReq
 *
 *       Desc:  Builds EGTP-C Create Session Request
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldCSReq
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg
)
#else
PUBLIC S16 vbMmeUtlBldCSReq(ueTunlCb, esmCb, egMsg)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb;
EgMsg           **egMsg;
#endif
{
   U16               ret = ROK;
   VbMmeUeInfo       *pUeCtx = NULLP;
   Mem               memInfo = {0};
   EgIeLevelInfo     egLvlInfo;
   TknU8             bearerId;
   VbHssPdnSubCntxt  *pdnCtxt = NULLP;
   EgBearQos         bearerQos;
   U8                ip4Addr[4];
   U8                mcc[LVB_MAX_MCC_LEN];
   U8                mnc[LVB_MAX_MNC_LEN];
   U8                protCfgArr[255]; 
   U8                len = 0;
   EgIe              fteidArr[1];
   U8                i = 0;

   VB_TRC2(vbMmeUtlBldCSReq)

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Create Session Request"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   pUeCtx = &ueTunlCb->ueCb->ueCtxt;
   if (egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: EgMsg"));
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),\
         EGT_GTPC_MSG_CS_REQ, ++ueTunlCb->seqNum);

   /* Add IMSI */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   /*KWORKS Fix*/
   if(ueTunlCb->ueCb->ueCtxt.ueImsiLen <= VB_HSS_IMSI_LEN)
   {
   egUtlEgtAddImsi(*egMsg, &egLvlInfo, ueTunlCb->ueCb->ueCtxt.ueImsi, 
        ueTunlCb->ueCb->ueCtxt.ueImsiLen, 0);
   }
   else
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Invalid IMSI Length for lpUeId(%d).",esmCb->ueCb->ueCtxt.ueHssCtxt.ueEntry));
      cmFreeMem((Ptr)egMsg);
      RETVALUE(RFAILED);
   }

   /* Add MSISDN in case of attach procedure */
   if (VB_MME_CHK_MASK(pUeCtx->ueHssCtxt.pres, VB_MME_HSS_CTX_MSISDN_PRESENT))
   {
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      egUtlEgtAddMsisdn(*egMsg, &egLvlInfo, pUeCtx->ueHssCtxt.msisdn,\
            sizeof(pUeCtx->ueHssCtxt.msisdn), 0); 
   }
   
   /* Encoding of ULI */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   vbUtlEgtAddUli(*egMsg, &egLvlInfo, 0, &(ueTunlCb->ueCb->ueCtxt.tai),
                  &(ueTunlCb->ueCb->ueCtxt.eCgi));

   /* Add Sewrving Network */
   mcc[0] = ueTunlCb->ueCb->ueCtxt.tai.plmnId.plmnId[0] & 0x0F;
   mcc[1] = ueTunlCb->ueCb->ueCtxt.tai.plmnId.plmnId[0] & 0xF0 >> 4;
   mcc[2] = ueTunlCb->ueCb->ueCtxt.tai.plmnId.plmnId[1] & 0x0F;
   mnc[0] = ueTunlCb->ueCb->ueCtxt.tai.plmnId.plmnId[2] & 0x0F;
   mnc[1] = ueTunlCb->ueCb->ueCtxt.tai.plmnId.plmnId[2] & 0xF0 >> 4;
   mnc[2] = ueTunlCb->ueCb->ueCtxt.tai.plmnId.plmnId[1] & 0xF0 >> 4;
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddSrvNwk(*egMsg, &egLvlInfo, 0, mcc,mnc);

   /* Add RAT Type */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddRatType(*egMsg, &egLvlInfo, VB_RAT_EUTRAN, 0);

   /* Add Indication type */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   VbUtlAddIndType(*egMsg, &egLvlInfo, 0);

   /* S11 Fteid */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   vbUtlIpv4InetToArr((ip4Addr), &(ueTunlCb->locAddr.u.ipv4TptAddr.address));
   egUtlEgtAddFteid(*egMsg, &egLvlInfo, ueTunlCb->localTeIdKey,\
         &ip4Addr[0], EGT_INT_S11_MME_GTP_C, 0);

   /* PGW S5/S8 C-FTEID */
   if (esmCb->pdnCb->s5PgwTeId != 0)
   {
      /* Handover with MME relocation */
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      vbUtlIpv4InetToArr(ip4Addr, &(esmCb->pdnCb->s5PgwAddr.u.ipv4NetAddr));
      egUtlEgtAddFteid(*egMsg, &egLvlInfo, esmCb->pdnCb->s5PgwTeId,\
            (U8 *)ip4Addr, EGT_INT_S5_S8_PGW_GTP_C, 1);
   }
   else
   {
      vbUtlIpv4InetToArr((ip4Addr), &(pUeCtx->ueHssCtxt.subCntxt[0].\
            pdnGwAddr.u.ipv4NetAddr));
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      egUtlEgtAddFteid(*egMsg, &egLvlInfo, 0, (U8 *)ip4Addr,\
         EGT_INT_S5_S8_PGW_GTP_C, 1);
   }

   /* APN */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   if (esmCb->apn.pres)
   {
      egUtlEgtAddApn(*egMsg,&egLvlInfo, (U8 *)esmCb->apn.apn, 0);
   }
   else
   {
      egUtlEgtAddApn(*egMsg,&egLvlInfo, (U8 *)pUeCtx->ueHssCtxt.\
            subCntxt[0].apn, 0);
   }

   /* PCO */
   if(esmCb->pco.pres)
   {
      if(ROK == vbUtlProtoCfgOptToArr(protCfgArr, &len, &esmCb->pco))
      {
         cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
         egUtlEgtAddPCO(*egMsg, &egLvlInfo, protCfgArr, len, 0);
      }
   }

   /* Add selection mode . value 0 means UE Subscribed and verified.*/
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddSelMode(*egMsg, &egLvlInfo, 0);

   /* Add PDN Type .value 0 menas Ipv4 and 1 means IpV6 */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddPdnType(*egMsg,&egLvlInfo, 0);

   /* Add PDN Address allocation */
   if ((ueTunlCb->ueCb->proc == VB_MME_HANDOVER_PROC) && \
      (NULL != ueTunlCb->ueCb->pHoData))
   {
      if (ueTunlCb->ueCb->pHoData->ueAddr.u.ipv4NetAddr != 0)
      {
         cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
         vbUtlIpv4InetToArr((ip4Addr),\
               &(ueTunlCb->ueCb->pHoData->ueAddr.u.ipv4NetAddr));
      }
   }
   else
   {
      cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
      vbUtlIpv4InetToArr((ip4Addr),
               &(ueTunlCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[0].
               pdnAddrLst.nAddr[0].u.ipv4NetAddr));
   }
   egUtlEgtAddPAA(*egMsg, &egLvlInfo, ip4Addr, 0);

   /* Maximum APN Restriction, value set to 0 = No context restriction */
   cmMemset((U8 *)&egLvlInfo, 0, sizeof(EgIeLevelInfo));
   egUtlEgtAddMaxApnRestrict(*egMsg, &egLvlInfo, 0);

   /* Add Bearer Context */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egLvlInfo.level = 0;
   egLvlInfo.ieInfo[0].ieTypePres = TRUE;
   egLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
   egLvlInfo.ieInfo[0].ieInst = 0;
   egLvlInfo.ieInfo[0].refValue = FALSE;
   bearerId.pres = TRUE;
   bearerId.val  = esmCb->bId;

   /* Bearer QoS */
   if (VB_MME_HANDOVER_PROC == ueTunlCb->ueCb->proc)
   {
      bearerQos.qci = esmCb->epsQos.qci;
      vbMmeUtlFillGtpBitRate(&(bearerQos.upMBR), esmCb->epsQos.maxBitRateUL);
      vbMmeUtlFillGtpBitRate(&(bearerQos.dnMBR), esmCb->epsQos.maxBitRateDL);
      vbMmeUtlFillGtpBitRate(&(bearerQos.upGBR), 0);
      vbMmeUtlFillGtpBitRate(&(bearerQos.dnGBR), 0);
   }
   else
   {
      /* 
       * If the Message is standalone PDN connectivity request 
       * enter inside if condition and select the subCntxt according to the 
       * UE requested APN.
       */
      if(VB_MME_PDN_CONN_PROC == esmCb->proc)
      {
         /* filling of subCntxt should be based on the apn, UE has requested */
         for(i = 0; i < VB_HSS_MAX_PDN_SUB_CTXT; i++)
         {
            if ((ret = cmMemcmp((U8 *)&esmCb->apn.apn, \
                        (U8 *)&ueTunlCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i].apn, \
                        esmCb->apn.len)) == 0)
            {
               pdnCtxt = &ueTunlCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[i];
               break;
            }
         }
      }

      /* 
       * If the message is inttial atttach request 
       * select the default APN, ie subCntxt[0]
       */
      else if(VB_MME_ATTACH_PROC == ueTunlCb->ueCb->proc)
      {
         pdnCtxt = &ueTunlCb->ueCb->ueCtxt.ueHssCtxt.subCntxt[0];
      }

      /*KWORKS Fix*/
      if(pdnCtxt != NULLP)
      {
      bearerQos.qci = pdnCtxt->qos.qci;
      vbMmeUtlFillGtpBitRate(&(bearerQos.upMBR), pdnCtxt->qos.maxBitRateUL);
      vbMmeUtlFillGtpBitRate(&(bearerQos.dnMBR), pdnCtxt->qos.maxBitRateDL);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieved pdnCtxt is NULL "));
      }
      vbMmeUtlFillGtpBitRate(&(bearerQos.upGBR), 0);
      vbMmeUtlFillGtpBitRate(&(bearerQos.dnGBR), 0);

      esmCb->epsQos.pres = TRUE;
      esmCb->epsQos.lenQosCont  = 9;
      /*KWORKS Fix*/
      if(pdnCtxt != NULLP)
      {
      esmCb->epsQos.qci = pdnCtxt->qos.qci;

      vbMmeUtlFillEpsQoS(pdnCtxt->qos.maxBitRateUL,\
            &esmCb->epsQos.maxBitRateUL, &esmCb->epsQos.maxBitRateULExt);
      vbMmeUtlFillEpsQoS(pdnCtxt->qos.maxBitRateDL,\
            &esmCb->epsQos.maxBitRateDL, &esmCb->epsQos.maxBitRateDLExt);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieved pdnCtxt is NULL "));
      }
      vbMmeUtlFillEpsQoS(0, &esmCb->epsQos.guaraBitRateUL,\
            &esmCb->epsQos.guaraBitRateULExt);
      vbMmeUtlFillEpsQoS(0, &esmCb->epsQos.guaraBitRateDL,\
            &esmCb->epsQos.guaraBitRateDLExt);
   }

   if ((VB_MME_ATTACH_PROC == ueTunlCb->ueCb->proc) || (VB_MME_PDN_CONN_PROC == esmCb->proc))
   {   
   #ifndef VB_ACC_IRAT_TEST
      esmCb->apn.pres = TRUE;
      /*KWORKS Fix*/
      if(pdnCtxt != NULLP)
      {
      esmCb->apn.len = cmStrlen((U8 *)pdnCtxt->apn);
      cmMemcpy((U8 *)(esmCb->apn.apn), (U8 *)pdnCtxt->apn, esmCb->apn.len);
      }
      else
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Retrieved pdnCtxt is NULL "));
      }
#endif
   }

   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Bearer Context to be Created"));
   if ((VB_MME_ATTACH_PROC == ueTunlCb->ueCb->proc) || (VB_MME_PDN_CONN_PROC == esmCb->proc))
   {
      ret = egUtlEgtAddBearerCxt(*egMsg, &egLvlInfo, 1, &bearerId, NULLP, 0,\
            NULLP, 0, &bearerQos);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Bearer Context to be "\
               "Created"));
      }
   }
   else if (VB_MME_HANDOVER_PROC == ueTunlCb->ueCb->proc)
   {
      /* if HO is with MME relocation, send PGW s5/s8 u-teid */
      if (VB_MME_CHK_MASK(ueTunlCb->ueCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE))
      {
         fteidArr[0].ieType = EG_FTEID_IETYPE;
         fteidArr[0].ieInst = 3;
         fteidArr[0].dataType = EG_FTEID;
         fteidArr[0].t.fTeid.intfType = EGT_INT_S5_S8_PGW_GTP_U;
         fteidArr[0].t.fTeid.teid = esmCb->s5PgwUTeid;
         vbUtlIpv4InetToArr((fteidArr[0].t.fTeid.ip4Addr),
               &(esmCb->s5PgwUAddr.u.ipv4NetAddr));

         ret = egUtlEgtAddBearerCxt(*egMsg, &egLvlInfo, 1, &bearerId,\
               NULLP, 0, fteidArr, 1, &bearerQos);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Bearer Context to be "\
                  "Created"));
         }
      }
   }
   
   RETVALUE(ROK);
}/* End of function vbMmeUtlBldCSReq */

#ifdef ANSI
PRIVATE S16 vbMmeUtlFillGtpBitRate
(
EgDatStr8 *gtpQoS, 
U32 qos
)
#else
PRIVATE S16 vbMmeUtlFillGtpBitRate(gtpQoS, qos)
(
EgDatStr8 *gtpQoS; 
U32       qos;
)
#endif
{
   U8 indx=0;

   cmMemset((U8*)gtpQoS, 0, sizeof(EgDatStr8));

   gtpQoS->length = 5;
   qos /= 1000; /*Converting to KBits*/
   for(indx = 0; indx < gtpQoS->length; indx++)
   {
      gtpQoS->val[indx] = (qos>>(8*(gtpQoS->length-indx-1))) & 0xFF;
   }
   RETVALUE(ROK);
}

#ifdef ANSI
PRIVATE S16 vbMmeUtlFillEpsQoS
(
U32      qos,
U8      *bitFld, 
U8      *bitFldExt 
)
#else
PRIVATE S16 vbMmeUtlFillEpsQoS(qos, bitFld, bitFldExt)
(
U32      qos;
U8       *bitFld; 
U8       *bitFldExt; 
)
#endif
{
   U16 kbps = 1000;
   *bitFldExt = 0;
   if(qos == 0)
   {
      *bitFld = 255;
   }
   else if(qos <= (63*kbps))
   {
      *bitFld = qos/kbps;
   }
   else if(qos <=(568*kbps))
   {  
      *bitFld = (qos-64*kbps)/(8*kbps) + 64;
   }
   else if(qos <=(8640*kbps))
   {  
      *bitFld = (qos-576*kbps)/(64*kbps) + 128;
   }
   else if(qos <= (16000*kbps))
   {
      *bitFld = 254;
      *bitFldExt = (qos-8600*kbps)/(100*kbps);
   }
   else if(qos <= (128*kbps*kbps))
   {
      *bitFld = 254;
      *bitFldExt = (qos-16*kbps*kbps)/(kbps*kbps)+74;
   }
   else if(qos <= (256*kbps*kbps))
   {
      *bitFld = 254;
      *bitFldExt = (qos-128*kbps*kbps)/(2*kbps*kbps)+186;
   }
   else
   {
      *bitFld = 254;
      *bitFldExt = 250;
   }

   RETVALUE(ROK);
   
}



/*
 *
 *       Fun:   vbMmeUtlBldEgtCrtBearRsp
 *
 *       Desc:  Builds EGTP-C Create Bearer Response
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtCrtBearRsp
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb, 
EgMsg           **egMsg,
U8              status,
U8              cse
)
#else
PUBLIC S16 vbMmeUtlBldEgtCrtBearRsp(ueTunlCb, esmCb, egMsg, status, cse)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb; 
EgMsg           **egMsg;
U8              status;
U8              cause;
#endif
{
   Mem            memInfo;
   EgIe egIeArr[3];
   TknU8          bearerId;
   TknU8          cause;   
   EgIeLevelInfo  ieLvlInfo;
   U8             count = 0;
   S16            ret = ROK;


   VB_TRC2( vbMmeUtlBldEgtCrtBearRsp);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   cause.pres = TRUE;

   if(VB_MME_BEAR_STUP_SUCCESS == status)
   {
      cause.val = EGT_GTPC_CAUSE_ACC_REQ;
   }
   else
   {
      cause.val = cse;
   }

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating memory for the GTP Message failed\n");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   /* vb005.101 Sequence number is 24bit long, so MSB is at 24th bit */
#ifndef EG_REL_930
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
         EGT_GTPC_MSG_CB_RSP, (ueTunlCb->seqNum));
#else
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
         EGT_GTPC_MSG_CB_RSP, (ueTunlCb->seqNum ));
#endif /* EG_REL_930 */


   /* Add cause */
   cmMemset((U8 *)&ieLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddCause(*egMsg, &ieLvlInfo, cause.val , 0);

   /* Add the Bearer Context */
   bearerId.pres = TRUE;
   bearerId.val  = esmCb->bId;

  
   /* Add ENodeB FTEID */
   cmMemset((U8 *)egIeArr, 0, sizeof(EgIe)*2);
   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));

   if(VB_MME_BEAR_STUP_SUCCESS == status)
   {
      /* convert first */
      vbUtlIpv4InetToArr((egIeArr[count].t.fTeid.ip4Addr),
         &(esmCb->s1EnbAddr.u.ipv4NetAddr));
      egIeArr[count].ieType = EG_FTEID_IETYPE;
      egIeArr[count].t.fTeid.isIPv4Pres = TRUE;
      egIeArr[count].t.fTeid.teid = esmCb->s1EnbTeId;
      egIeArr[count].t.fTeid.intfType = EGT_INT_S1_U_ENB_GTP_U;
#ifndef EG_REL_930
      egIeArr[count].t.fTeid.ebi = bearerId.val;
#endif /* EG_REL_930 */
      egIeArr[count].ieInst = 0;
      count++;
   }
   /* convert first */
   vbUtlIpv4InetToArr((egIeArr[count].t.fTeid.ip4Addr),
         &(esmCb->s1SgwAddr.u.ipv4NetAddr));
   egIeArr[count].ieType = EG_FTEID_IETYPE;
   egIeArr[count].t.fTeid.isIPv4Pres = TRUE;
   egIeArr[count].t.fTeid.teid = esmCb->s1SgwTeId;
   egIeArr[count].t.fTeid.intfType = EGT_INT_S1_U_SGW_GTP_U;
#ifndef EG_REL_930
   egIeArr[count].t.fTeid.ebi = bearerId.val;
#endif /* EG_REL_930 */
   egIeArr[count].ieInst = 1;
   count++;


   ieLvlInfo.level = 0;
   ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
   ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst = 0;
   ieLvlInfo.ieInfo[0].refValue = FALSE;

   bearerId.pres = TRUE;
   bearerId.val = esmCb->bId;
   egUtlEgtAddBearerCxt(*egMsg, &ieLvlInfo, 1, &bearerId, &cause,
         0, egIeArr, count, NULLP);

   /* Add Recovery IE to the egMsg as its mandatory IE in the 
    * S11 Interface dictionary */
   cmMemset((U8 *)&ieLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtBuildEgIeHeader(&egIeArr[2], EG_RECOVERY_IETYPE, 0, EG_UNSIGNED_8);

   egIeArr[2].t.valUnSgnd8 = 1; /* some random value */

   ieLvlInfo.level = 0;
   ieLvlInfo.ieInfo[0].ieTypePres= TRUE;
   ieLvlInfo.ieInfo[0].ieType= EG_RECOVERY_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst= 0;

   ret = EgUtilGmAppendIe(*egMsg, &ieLvlInfo, &egIeArr[2]);
   if(ret != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Appending Recovery IE failed\n");
      RETVALUE(RFAILED);
   }
  
   RETVALUE(ROK);

}/* vbMmeUtlBldEgtBearRsp*/

/*
 *
 *       Fun:   vbMmeUtlBldEgtDelBearRsp
 *
 *       Desc:  Builds EGTP-C Delete Bearer Response
 *
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtDelBearRsp
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb, 
EgMsg           **egMsg,
U8              status,
U8              cse
)
#else
PUBLIC S16 vbMmeUtlBldEgtDelBearRsp(ueTunlCb, esmCb, egMsg, status, cse)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb; 
EgMsg           **egMsg;
U8              status;
U8              cse;
#endif
{
   Mem            memInfo;
   TknU8          bearerId;
   TknU8          cause;   
   EgIeLevelInfo  ieLvlInfo;
   U8             count = 0;


   VB_TRC2( vbMmeUtlBldEgtDelBearRsp);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   cause.pres = TRUE;

   if(VB_MME_BEAR_REL_SUCCESS == status)
   {
      cause.val = EGT_GTPC_CAUSE_ACC_REQ;
   }
   else
   {
      cause.val = cse;
   }

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating memory for the GTP Message failed\n");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   /* vb005.101 Sequence number is 24bit long, so MSB is at 24th bit */
#ifndef EG_REL_930
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
         EGT_GTPC_MSG_DB_RSP, (ueTunlCb->seqNum | (0xff << 8)));
#else
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
         EGT_GTPC_MSG_DB_RSP, (ueTunlCb->seqNum | (0x800000)));
#endif /* EG_REL_930 */


   /* Add cause */
   cmMemset((U8 *)&ieLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddCause(*egMsg, &ieLvlInfo, cause.val , 0);


   if(CM_ESM_EPS_DEF_BEARER == esmCb->bearerType)
   {
     cmMemset((U8 *)&ieLvlInfo, 0 , sizeof(EgIeLevelInfo));
     egUtlEgtAddEpsBId(*egMsg, &ieLvlInfo, esmCb->bId, 0);
   }
   else
   {
      /* Add the Bearer Context */
      bearerId.pres = TRUE;
      bearerId.val  = esmCb->bId;
  
      cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));

      if(VB_MME_BEAR_REL_SUCCESS == status)
      {

      }
   
      ieLvlInfo.level = 0;
      ieLvlInfo.ieInfo[0].ieTypePres = TRUE;
      ieLvlInfo.ieInfo[0].ieType = EG_BEARER_CNTX_IETYPE;
      ieLvlInfo.ieInfo[0].ieInst = 0;
      ieLvlInfo.ieInfo[0].refValue = FALSE;

      bearerId.pres = TRUE;
      bearerId.val = esmCb->bId;
      egUtlEgtAddBearerCxt(*egMsg, &ieLvlInfo, 1, &bearerId, &cause,
         0, NULLP, count, NULLP);
   }

   RETVALUE(ROK);
}



/*
 *
 *       Fun:   vbMmeUtlBldDSReq
 *
 *       Desc:  Builds EGTP-C Delete Session Request
 *             
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldDSReq
(
VbMmeUeTunnelCb  *ueTunlCb,
VbMmeEsmCb       *esmCb,
EgMsg            **egMsg,
U8               cause
)
#else
PUBLIC S16 vbMmeUtlBldDSReq(ueTunlCb, esmCb, egMsg, cause)
VbMmeUeTunnelCb  *ueTunlCb;
VbMmeEsmCb       *esmCb;
EgMsg            **egMsg;
U8               cause;
#endif
{
   EgIeLevelInfo        egLvlInfo   = {0};         /* IE info */
   Mem                  memInfo     = {0};         /* memory information */
   VbMmeUeCb            *ueCb       = NULLP;       /* UE control block */
   EgIe                 egIe;

   VB_TRC2(vbMmeUtlBldDSReq)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Delete Session Request "));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   if ( egUtlAllocEgMsg(egMsg, &memInfo) != ROK)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB048, ERRZERO,
                     "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_DS_REQ, ++(ueTunlCb->seqNum));

   /* Linked EPS Bearer ID  */
   ueCb = ueTunlCb->ueCb;

   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
   egUtlEgtAddEpsBId(*egMsg, &egLvlInfo, esmCb->bId, 0);

   /* Add Indication type */
   /* Operation Indication: This flag shall be set over S4/S11 interface
    * if the SGW needs to forward the Delete Session Request message to 
    * the PGW.
    * Scope indication flag shall be set if its TAU/HO with SGW relocation */ 
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   /* 
    * This check is not required, because in all procedures
    * we need to delete the session in PGW, No use of deleting
    * session at MME, SGW and keeping session at PGW */
#if 0
   if ((VB_MME_DETACH_UE_PROC == ueCb->proc) ||
       (VB_MME_DETACH_NW_PROC == ueCb->proc) ||
       (VB_MME_ATTACH_PROC == ueCb->proc)    ||
       (VB_MME_IMPLICIT_DETACH_PROC == ueCb->proc) ||
       (VB_MME_PDN_DIS_CONN_PROC == esmCb->proc) ||
       (VB_MME_CTXT_REL_PROC  == ueCb->proc) ||
       (VB_MME_LCL_UE_REL_PROC == ueCb->proc))
   {
      egIe.t.indication.isOIPres = TRUE;
   }
#endif
   egIe.t.indication.isOIPres = TRUE;
   if (VB_MME_CHK_MASK(ueCb->bitmask, VB_EMM_TAU_WITH_SGW_CHANGE))
   {
      egIe.t.indication.isSIPres = TRUE;
   }
   vbMmeEgtUtlAppendIe(*egMsg, EG_INDICATION_IETYPE, 0, EG_INDICATION,\
         &egIe);

   RETVALUE(ROK);
}/* End of function vbMmeUtlBldDSReq */


/*
 *
 *       Fun:   vbMmeUtlBldMBReq
 *
 *       Desc:  Builds EGTP-C Modify Bearer Request
 *             
 *       Ret:   
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldMBReq
(
VbMmeUeTunnelCb *ueTunlCb,
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg,
Bool            isRelease
)
#else
PUBLIC S16 vbMmeUtlBldMBReq(ueTunlCb, esmCb, egMsg, isRelease)
VbMmeUeTunnelCb *ueTunlCb;
VbMmeEsmCb      *esmCb,
EgMsg           **egMsg;
Bool            isRelease;
#endif
{
   S16            ret            = ROK;
   Mem            memInfo        = {0};
   EgIe           egIe;   
   EgIeLevelInfo     egLvlInfo;

   VB_TRC2(vbMmeUtlBldMBReq);
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Building Modify Bearer Request"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if (egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB049, ERRZERO, "Allocating Memory: "\
            "EgMsg");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),\
         EGT_GTPC_MSG_MB_REQ, ++ueTunlCb->seqNum);

   /* In case of RAB release, do not add any bearer info */
   if (TRUE == isRelease)
   {
      RETVALUE(ROK);
   }

   /* RAT Type */
   /* Incase of iRAT, RAT Type IE has to be added to modify bearer request */
   if( (ueTunlCb->ueCb->pHoData) && 
      (ueTunlCb->ueCb->pHoData->hoType == VB_EMM_HO_UTRAN_TO_LTE) )
   {
      cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));
      egUtlEgtAddRatType(*egMsg, &egLvlInfo, VB_RAT_EUTRAN, 0);
   }


   /* Add Indication type */
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Indication IE"));
   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   ret = vbMmeEgtUtlAppendIe(*egMsg, EG_INDICATION_IETYPE, 0,\
         EG_INDICATION, &egIe);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Indication IE"));
      EgUtilDeAllocGmMsg(egMsg);
      RETVALUE(ret);
   }

   /* Sender F-TEID for Control Plane, Send this IE only in case of TAU/HO 
    * with MME change */
   if ((VB_MME_CHK_MASK(ueTunlCb->ueCb->bitmask, VB_EMM_TAU_WITH_MME_CHANGE)) ||
       (VB_MME_CHK_MASK(ueTunlCb->ueCb->bitmask, VB_EMM_HO_WITH_MME_CHANGE)))
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding eNB MME S11 C-FTEID IE"));
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.ieInst = 0;
      egIe.t.fTeid.teid = ueTunlCb->localTeIdKey;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S11_MME_GTP_C;
      vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
            &(ueTunlCb->locAddr.u.ipv4TptAddr.address));
      ret = vbMmeEgtUtlAppendIe(*egMsg, EG_FTEID_IETYPE, 0,\
            EG_FTEID, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding eNB MME S11 C-FTEID IE"));
         EgUtilDeAllocGmMsg(egMsg);
         RETVALUE(ret);
      }
   }

   /* In case pdn connectivity procedure  */
   /* Only default bearer is handled      */
   if ((VB_MME_PDN_CONN_PROC == ueTunlCb->ueCb->proc) || \
       (VB_MME_ATTACH_PROC == ueTunlCb->ueCb->proc) || \
      (VB_MME_HANDOVER_PROC == ueTunlCb->ueCb->proc) || \
      (VB_MME_SERVICE_REQ_PROC == ueTunlCb->ueCb->proc) || \
      ((VB_MME_ESM_PROC == ueTunlCb->ueCb->proc) &&
      (VB_MME_ACT_DEF_BEAR_PROC == esmCb->proc)))
   {
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Bearer Context to be "\
            "Modified IE"));
      ret = vbMmeEgtUtlAppendIe(*egMsg, EG_BEARER_CNTX_IETYPE, 0,\
            EG_GROUPED, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Bearer Context to be "\
               "Modified IE"));
      }

      /* Add EBI ie into Bearer Context */
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding EBI IE into Bearer "\
               "Context to be Modified IE"));
         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.t.valUnSgnd8 = esmCb->bId;
         ret = vbMmeEgtUtlAddBCtxIe(*egMsg, EG_EBI_IETYPE, 0,\
               EG_UNSIGNED_8, &egIe);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding EBI IE into Bearer "\
                  "Context to be Modified IE"));
         }
      }

      /* Add eNB S1-U F-TEID */
      if (ROK == ret)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding eNB S1-U FTEID IE into "\
               "Bearer Context Created IE"));

         cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
         egIe.ieInst = 0;
         egIe.t.fTeid.teid = esmCb->s1EnbTeId;
         egIe.t.fTeid.isIPv4Pres = TRUE;
         egIe.t.fTeid.intfType = EGT_INT_S1_U_ENB_GTP_U;
         vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
               &(esmCb->s1EnbAddr.u.ipv4NetAddr));
         ret = vbMmeEgtUtlAddBCtxIe(*egMsg, EG_FTEID_IETYPE, 0,\
               EG_FTEID, &egIe);
         if (ROK != ret)
         {
            VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding eNB S1-U FTEID IE into "\
                  "Bearer Context Created IE"));
         }
      }
   }

   if (ROK != ret)
   {
      EgUtilDeAllocGmMsg(egMsg);
   }

   RETVALUE(ret);
}/* End of function vbMmeUtlBldMBReq */

/* 
 *
 *       Fun:   vbMmeUtlBldEgtDwnLnkNotAck 
 *
 *       Desc:  Builds EGTP-C downlink notification Ack 
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtDwnLnkNotAck 
(
VbMmeUeTunnelCb *ueTunlCb,
EgMsg           **egMsg,
U8              cause
)
#else
PUBLIC S16 vbMmeUtlBldEgtDwnLnkNotAck(ueTunlCb, egMsg, cause)
VbMmeUeTunnelCb *ueTunlCb;
EgMsg           **egMsg;
U8              casue;
#endif
{
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;

   VB_TRC2(vbMmeUtlBldEgtDwnLnkNotAck)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
               "Building Downlink Notification Ack"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_DWN_DATA_NOTFN_ACK, ueTunlCb->seqNum);

   /* set cause as it is mandatory IE */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /*egUtlEgtAddCause(*egMsg, &egLvlInfo, EGT_GTPC_CAUSE_PAGE, 0);*/
   egUtlEgtAddCause(*egMsg, &egLvlInfo, cause, 0);

   RETVALUE(ROK);
}

/* 
 *
 *       Fun:   vbMmeUtlBldEgtDwnLnkNotFailInd 
 *
 *       Desc:  Builds EGTP-C downlink notification failure indication 
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */


#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtDwnLnkNotFailInd
(
VbMmeUeTunnelCb *ueTunlCb,
EgMsg           **egMsg,
U8              cause
)
#else
PUBLIC S16 vbMmeUtlBldEgtDwnLnkNotFailInd(ueTunlCb, egMsg, cause)
VbMmeUeTunnelCb *ueTunlCb;
EgMsg           **egMsg;
U8              casue;
#endif
{
   Mem            memInfo;
   EgIeLevelInfo  egLvlInfo;

   VB_TRC2(vbMmeUtlBldEgtDwnLnkNotFailInd)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
               "Building Downlink Notification Fail Ind"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_DD_NOTFN_FAIL_IND, ++ueTunlCb->seqNum);

   /* set cause as it is mandatory IE */
   cmMemset((U8 *)&egLvlInfo, 0 , sizeof(EgIeLevelInfo));

   /*egUtlEgtAddCause(*egMsg, &egLvlInfo, EGT_GTPC_CAUSE_PAGE, 0);*/
   egUtlEgtAddCause(*egMsg, &egLvlInfo, cause, 0);

   RETVALUE(ROK);
}
/* 
 *
 *       Fun:   vbMmeUtlBldEgtRABReq
 *
 *       Desc:  Builds EGTP-C Release Access Bearer Request
 *
 *       Ret:
 *
 *       Notes: None
 *
 *       File:  vb_mme_utl.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeUtlBldEgtRABReq
(
VbMmeUeTunnelCb *ueTunlCb,
EgMsg           **egMsg
)
#else
PUBLIC S16 vbMmeUtlBldEgtRABReq(ueTunlCb,egMsg)
VbMmeUeTunnelCb *ueTunlCb;
EgMsg           **egMsg;
#endif
{
   Mem            memInfo;

   VB_TRC2(vbMmeUtlBldEgtRABReq)
   VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
               "Building Release Access Bearer Request"));

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;

   if( egUtlAllocEgMsg(egMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB047, ERRZERO,
            "Allocating Memory For The GTP Message Failed");
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&((*egMsg)->msgHdr), (ueTunlCb->remoteTeId),
                      EGT_GTPC_MSG_RAB_REQ, ++ueTunlCb->seqNum);

   RETVALUE(ROK); 
}



/*
 *
 *       Fun:   VbUtlAddIndType
 *
 *       Desc:  forms the Indication IE type info for encoding
 *              This function is re-written from egUtlEgtAddIndType
 *              for future uses in handling TAU/HO with SGW change
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
PUBLIC S16 VbUtlAddIndType
(
EgMsg            *egMsg,
EgIeLevelInfo    *egLvlInfo,
U8               inst
)
#else
PUBLIC S16 VbUtlAddIndType(egMsg, egLvlInfo, inst)
EgMsg            *egMsg;
EgIeLevelInfo    *egLvlInfo;
U8               inst;
#endif
{
   EgIe        egIe;

   /* Fill the ie properties */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   egIe.t.indication.isDFIPres = 1; /* X2 interface avilable */
   egIe.t.indication.isMSVPres = 1; /* UE is authenticated */

#ifndef EG_REL_930
   egUtlEgtBuildEgIeHeader(&egIe, EG_INDICATION_IETYPE, inst, 0, EG_INDICATION);
#else
   egUtlEgtBuildEgIeHeader(&egIe, EG_INDICATION_IETYPE, inst, EG_INDICATION);
#endif /* EG_REL_930 */

   if (EgUtilGmAppendIe(egMsg, egLvlInfo, &egIe) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 *
 *       Fun:   vbMmeVldNStrRABRsp
 *
 *       Desc:  This function used to validate the Release Access Bearer
 *              Response received from SGW and store the values in the uE
 *              context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrRABRsp
(
VbMmeUeCb   *ueCb,            /* UE control block */
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrRABRsp(ueCb, egMsg, cause)
VbMmeUeCb   *ueCb;            /* UE control block */
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause;        /* Cause of failure if any */
#endif
{
   EgIeLevelInfo     ieLvlInfo         = {0};
   EgIe              *egIe             = NULLP;
   U16               iePos             = 0;
   S16               ret               = ROK;

   VB_TRC2(vbMmeVldNStrRABRsp)

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   *cause = VB_ESM_INV_CAUSE;

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)",
                  ueCb->ueTunnelCb->localTeIdKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)",
                  ueCb->ueTunnelCb->localTeIdKey));
#endif                  

   /* Check the cause IE */
   ieLvlInfo.level                  = 0;
   ieLvlInfo.ieInfo[0].ieTypePres   = TRUE;
   ieLvlInfo.ieInfo[0].ieType       = EG_CAUSE_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst       = 0;
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
   if(ret != ROK)
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "CAUSE IE Could Not Be "
                        "Retrieved"));
   }
	
	/* This check has been introduced to avoid klocworks tool warning */
   if ((ret == ROK) && egIe != NULLP && (egIe->t.valUnSgnd8 != EGT_GTPC_CAUSE_ACC_REQ))
   {
      ret = RFAILED;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Release Access Bearer Request Not "
                        "Accepted, Cause Value(%d)", egIe->t.valUnSgnd8));
   }

   /* This is enuff for Initial attach. Update the function when handling
    * for TAU/HO is done 
    */
   
   RETVALUE(ret);
}

/*
 *
 *       Fun:   vbMmeVldNStrBearResFailInd
 *
 *       Desc:  This function used to validate the Bearer Resource Failure
 *              Indication received from SGW and store the values in the uE
 *              context
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  vb_esm_gtp.c
 *
 */
#ifdef ANSI
PUBLIC S16 vbMmeVldNStrBearResFailInd
(
VbMmeUeCb   *ueCb,            /* UE control block */
VbMmeEsmCb  *esmCb, 
EgMsg       *egMsg,           /* structure for carrying eGTP MSG info */
U8          *cause         /* Cause of failure if any */
)
#else
PUBLIC S16 vbMmeVldNStrBearResFailInd(ueCb, esmCb, egMsg, cause)
VbMmeUeCb   *ueCb;            /* UE control block */
VbMmeEsmCb  *esmCb;
EgMsg       *egMsg;           /* structure for carrying eGTP MSG info */
U8          *cause;        /* Cause of failure if any */
#endif
{
   EgIeLevelInfo     ieLvlInfo         = {0};
   EgIe              *egIe             = NULLP;
   U16               iePos             = 0;
   S16               ret               = ROK;

   VB_TRC2(vbMmeVldNStrBearResFailInd)

   cmMemset((U8 *)&ieLvlInfo, 0, sizeof(EgIeLevelInfo));
   *cause = VB_ESM_INV_CAUSE;

#ifdef ALIGN_64BIT
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%d)",
                  ueCb->ueTunnelCb->localTeIdKey));
#else
   VB_MME_DBG_INFO((VB_MME_PRNTBUF,"TeId(%ld)",
                  ueCb->ueTunnelCb->localTeIdKey));
#endif                  

   /* Check the cause IE */
   ieLvlInfo.level                  = 0;
   ieLvlInfo.ieInfo[0].ieTypePres   = TRUE;
   ieLvlInfo.ieInfo[0].ieType       = EG_CAUSE_IETYPE;
   ieLvlInfo.ieInfo[0].ieInst       = 0;
   ret = EgUtilGmGetIe(egMsg, &ieLvlInfo, &egIe, &iePos);
   if(ret != ROK)
   {
      ret = RFAILED;
      *cause = EGT_GTPC_CAUSE_SYS_FAIL;
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "CAUSE IE Could Not Be "
                        "Retrieved"));
      RETVALUE(ret);
   }
   if (ret == ROK)
   {
      if (egIe != NULLP)
      {
         VB_MME_DBG_INFO((VB_MME_PRNTBUF, 
                        "Bearer Resource Failure Ind Cause Value(%d)",
                        egIe->t.valUnSgnd8));
      }
   }
  
   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Retrieves the GTP IE of specific type and instance from EgMsg
 *             
 * @param[in]  pEgMsg
 *    Pointer to gtp message containing the IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 * @param[in]  ieType
 *    Type of the GTP IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * 
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetIe
(
EgMsg   *pEgMsg,
EgIe    **ppEgIe,
U8      ieType,
U8      ieInst
)
{  
   EgIeLevelInfo ieLevelInfo;
   U16           iePos = 0;
   S16           ret = ROK;
   
   VB_TRC2(vbMmeEgtUtlGetIe);

   *ppEgIe = NULLP;
   cmMemset((U8 *)&ieLevelInfo, 0, sizeof(EgIeLevelInfo));

   ieLevelInfo.level = 0;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieTypePres = TRUE;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieType = ieType;
   ieLevelInfo.ieInfo[ieLevelInfo.level].ieInst = ieInst;

   ret = EgUtilGmGetIe(pEgMsg, &ieLevelInfo, ppEgIe, &iePos);

   RETVALUE(ret);
} /* vbMmeEgtUtlGetIe */


/**************************************************************************//**
 *
 * @brief 
 *    Converts 4 byte IP Address array into U32
 *             
 * @param[in]  pAddrArr
 *    Pointer to address buffer array
 * @param[out] pAddr
 *    Pointer to u32 bit IP Address
 * 
 * @return
 *    No return values
 *
 *****************************************************************************/
PUBLIC Void vbMmeEgtUtlIpv4ArrToInet
(
U8             *pAddrArr,
U32            *pAddr
)
{
   *pAddr = ((U32)pAddrArr[0] << 24)|  ((U32)pAddrArr[1] << 16)   | 
            ((U32)pAddrArr[2] << 8)  |  (U32)(pAddrArr[3]);
} /* vbMmeEgtUtlIpv4ArrToInet */


/**************************************************************************//**
 *
 * @brief 
 *    Gets IE within PDN Connection IE 
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 * 
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetEpsPdnConnIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgIe           **ppEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   U16               iePos     = 0;
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlGetEpsPdnConnIe);
   
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_PDN_CONN_IETYPE, 0);
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   ret = EgUtilGmGetIe(pEgMsg, &ieLvlInfo, ppEgIe, &iePos);
   
   RETVALUE(ret);
} /* vbMmeEgtUtlGetEpsPdnConnIe */

/**************************************************************************//**
 *
 * @brief 
 *    Gets IE within Bearer Context IE 
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 * 
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetBCtxIe
(
EgMsg          *pEgMsg,       /* gtp-c msg */
U8             bearerInst,    /* Instance of Bearer Ctx */
U8             bearerCnt,     /* occr count of bearer */
U32            ieType,        /* type of Ie in bearer ctx */
U8             ieInst,        /* Instance of ie in bearer ctx */
EgIe           **ppEgIe       /* ie */
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   U16               iePos     = 0;
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlGetBCtxIe);
   
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, bearerInst);
   ieLvlInfo.ieInfo[0].occrPres = TRUE;
   ieLvlInfo.ieInfo[0].occr = bearerCnt;

   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   ret = EgUtilGmGetIe(pEgMsg, &ieLvlInfo, ppEgIe, &iePos);
   
   RETVALUE(ret);
} /* vbMmeEgtUtlGetBCtxIe */


/**************************************************************************//**
 *
 * @brief 
 *    Gets IE within Bearer Context ID within PDN Connection IE 
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[out]  ppEgIe
 *    Pointer address of GTP IE
 * 
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetBCtxIeInPdnConn
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgIe           **ppEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   U16               iePos     = 0;
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlGetBCtxIeInPdnConn);
   
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_PDN_CONN_IETYPE, 0);
   ieLvlInfo.ieInfo[0].occrPres = TRUE;
   ieLvlInfo.ieInfo[0].occr = TRUE;
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, EG_BEARER_CNTX_IETYPE, 0);
   ieLvlInfo.ieInfo[1].occrPres = TRUE;
   ieLvlInfo.ieInfo[1].occr = TRUE;
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 2, ieType, ieInst);
   ret = EgUtilGmGetIe(pEgMsg, &ieLvlInfo, ppEgIe, &iePos);
   
   RETVALUE(ret);
} /* vbMmeEgtUtlGetBCtxIeInPdnConn */

/**************************************************************************//**
 *
 * @brief 
 *    Adds and IE into GTP Message
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  ieType
 *    Type of IE
 * @param[in]  ieInst
 *    Instance of the GTP IE
 * @param[in]  dataType
 *    Data type of the GTP IE
 * @param[out]  pEgIe
 *    Pointer to GTP IE to be added into GTP message
 * 
 * @return
 *    SUCCESS if IE is retrieved successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAppendIe
(
EgMsg          *pEgMsg,  
U32            ieType,  
U8             ieInst,  
EgDataTypes    dataType,
EgIe           *pEgIe    
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret = ROK;

   VB_TRC2(vbMmeEgtUtlAppendIe)

   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, ieType, ieInst);

   egUtlEgtBuildEgIeHeader(pEgIe, ieType, ieInst, dataType);
   
   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, pEgIe);

   RETVALUE(ret);
} /* vbMmeEgtUtlAppendIe */



/**************************************************************************//**
 *
 * @brief 
 *    Encodes UE Security Capabilities into U8 buffer
 *             
 * @param[in]  pUeSecCap
 *    Pointer to UE Security capability
 * @param[inout]  pBuf
 *    String buffer
 * @param[inout] pLen
 *    Length of pBuf after encoding 
 * 
 * @return
 *    No return values
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC Void vbMmeEgtUtlEncUeSecCap
(
CmEmmUeSecCap     *pUeSecCap,
U8                *pBuf,
U16               *pBufLen
)
{
   U16      indx = 0;

   VB_TRC2(vbMmeEgtUtlEncUeSecCap);

   /* Encode EEA bits */
   pBuf[indx]  = (pUeSecCap->eea7 & 0x01);
   pBuf[indx] |= (pUeSecCap->eea6 & 0x01) << 1;
   pBuf[indx] |= (pUeSecCap->eea5 & 0x01) << 2;
   pBuf[indx] |= (pUeSecCap->eea4 & 0x01) << 3;
   pBuf[indx] |= (pUeSecCap->eea3 & 0x01) << 4;
   pBuf[indx] |= (pUeSecCap->eea2_128 & 0x01) << 5;
   pBuf[indx] |= (pUeSecCap->eea1_128 & 0x01) << 6;
   pBuf[indx++] |= (pUeSecCap->eea0 & 0x01) << 7;

   /* Encode EIA bits */
   pBuf[indx]  = (pUeSecCap->eia7 & 0x01);
   pBuf[indx] |= (pUeSecCap->eia6 & 0x01) << 1;
   pBuf[indx] |= (pUeSecCap->eia5 & 0x01) << 2;
   pBuf[indx] |= (pUeSecCap->eia4 & 0x01) << 3;
   pBuf[indx] |= (pUeSecCap->eia3 & 0x01) << 4;
   pBuf[indx] |= (pUeSecCap->eia2_128 & 0x01) << 5;
   pBuf[indx] |= (pUeSecCap->eia1_128 & 0x01) << 6;
   pBuf[indx++] |= (pUeSecCap->eia0 & 0x01) << 7;

   /* Encode UEA bits */
   pBuf[indx]  = (pUeSecCap->uea7 & 0x01);
   pBuf[indx] |= (pUeSecCap->uea6 & 0x01) << 1;
   pBuf[indx] |= (pUeSecCap->uea5 & 0x01) << 2;
   pBuf[indx] |= (pUeSecCap->uea4 & 0x01) << 3;
   pBuf[indx] |= (pUeSecCap->uea3 & 0x01) << 4;
   pBuf[indx] |= (pUeSecCap->uea2 & 0x01) << 5;
   pBuf[indx] |= (pUeSecCap->uea1 & 0x01) << 6;
   pBuf[indx++] |= (pUeSecCap->uea0 & 0x01) << 7;

   /* Encode UIA bits */
   pBuf[indx]  = (pUeSecCap->uia7 & 0x01);
   pBuf[indx] |= (pUeSecCap->uia6 & 0x01) << 1;
   pBuf[indx] |= (pUeSecCap->uia5 & 0x01) << 2;
   pBuf[indx] |= (pUeSecCap->uia4 & 0x01) << 3;
   pBuf[indx] |= (pUeSecCap->uia3 & 0x01) << 4;
   pBuf[indx] |= (pUeSecCap->uia2 & 0x01) << 5;
   pBuf[indx++] |= (pUeSecCap->uia1 & 0x01) << 6;

   *pBufLen = indx;

   RETVOID;
}

/**************************************************************************//**
 *
 * @brief 
 *    Decodes UE Security Capabilities into U8 buffer
 *             
 * @param[in]  pBuf
 *    String buffer
 * @param[in] pLen
 *    Length of pBuf
 * @param[inout]  pUeSecCap
 *    Pointer to UE Security capability
 * 
 * @return
 *    No return values
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC Void vbMmeEgtUtlDecUeSecCap
(
U8                *pBuf,
U16               bufLen,
CmEmmUeSecCap     *pUeSecCap
)
{
   U16      indx = 0;

   VB_TRC2(vbMmeEgtUtlDecUeSecCap);

   pUeSecCap->pres = TRUE;
   pUeSecCap->len = bufLen;

   /* Decode EEA bits */
   pUeSecCap->eea7 = pBuf[(indx)] & 0x01;
   pUeSecCap->eea6 = ((pBuf[(indx)] >> 1) & 0x01);
   pUeSecCap->eea5 = ((pBuf[(indx)] >> 2) & 0x01);
   pUeSecCap->eea4 = ((pBuf[(indx)] >> 3) & 0x01);
   pUeSecCap->eea3 = ((pBuf[(indx)] >> 4) & 0x01);
   pUeSecCap->eea2_128 = ((pBuf[indx] >> 5) & 0x01);
   pUeSecCap->eea1_128 = ((pBuf[indx] >> 6) & 0x01);
   pUeSecCap->eea0 = ((pBuf[indx++] >> 7) & 0x01);

   /* Decode EIA bits */
   pUeSecCap->eia7 = pBuf[indx] & 0x01;
   pUeSecCap->eia6 = ((pBuf[(indx)] >> 1) & 0x01);
   pUeSecCap->eia5 = ((pBuf[(indx)] >> 2) & 0x01);
   pUeSecCap->eia4 = ((pBuf[(indx)] >> 3) & 0x01);
   pUeSecCap->eia3 = ((pBuf[(indx)] >> 4) & 0x01);
   pUeSecCap->eia2_128 = ((pBuf[indx] >> 5) & 0x01);
   pUeSecCap->eia1_128 = ((pBuf[indx] >> 6) & 0x01);
   pUeSecCap->eia0 = ((pBuf[indx++] >> 7) & 0x01);

   /* Decode UEA bits */
   pUeSecCap->uea7 = (pBuf[(indx)] & 0x01);
   pUeSecCap->uea6 = ((pBuf[indx] >> 1) & 0x01);
   pUeSecCap->uea5 = ((pBuf[indx] >> 2) & 0x01);
   pUeSecCap->uea4 = ((pBuf[indx] >> 3) & 0x01);
   pUeSecCap->uea3 = ((pBuf[indx] >> 4) & 0x01);
   pUeSecCap->uea2 = ((pBuf[indx] >> 5) & 0x01);
   pUeSecCap->uea1 = ((pBuf[indx] >> 6) & 0x01);
   pUeSecCap->uea0 = ((pBuf[indx++] >> 7) & 0x01);

   /* Decode UIA bits */
   pUeSecCap->uia7 = (pBuf[(indx)] & 0x01);
   pUeSecCap->uia6 = ((pBuf[indx] >> 1) & 0x01);
   pUeSecCap->uia5 = ((pBuf[indx] >> 2) & 0x01);
   pUeSecCap->uia4 = ((pBuf[indx] >> 3) & 0x01);
   pUeSecCap->uia3 = ((pBuf[indx] >> 4) & 0x01);
   pUeSecCap->uia2 = ((pBuf[indx] >> 5) & 0x01);
   pUeSecCap->uia1 = ((pBuf[indx++] >> 6) & 0x01);

   RETVOID;
}


/**************************************************************************//**
 *
 * @brief 
 *    Adds and EPC PDN Connection IE into GTP Message
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to Ue CB
 * 
 * @return
 *    SUCCESS if PDN Connection IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddEpsPdn
(
EgMsg          *pEgMsg,
VbMmeUeCb      *pUeCb
)
{
   EgIe              egIe;
   EgIeLevelInfo     ieLvlInfo;
   S16               ret     = ROK;
   VbMmeEsmCb        *pEsmCb = NULLP;
   VbHssEPSSubQOSPrf *pQos   = NULLP;

   VB_TRC2(vbMmeEgtUtlAddEpsPdn);

   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_PDN_CONN_IETYPE, 0);
   egUtlEgtBuildEgIeHeader(&egIe, EG_PDN_CONN_IETYPE, 0, EG_GROUPED);
   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, &egIe);
   if (ROK != ret)
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding EPS PDN Connection IE"));
      RETVALUE(ret);
   }

   /* Handling only for default bearer */
   pEsmCb = pUeCb->esmList[0];

   /* APN */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding EPC APN IE into EPS PDN "\
            "Connection IE"));

      egIe.ieInst = 0;
      egIe.t.valStr132.length = pEsmCb->apn.len;
      cmMemcpy(egIe.t.valStr132.val, (U8 *)pEsmCb->apn.apn,\
            pEsmCb->apn.len);
      ret = vbMmeEgtUtlAddEpsPdnConnIe(pEgMsg, EG_APN_IETYPE, 0,\
            EG_STRING_132, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding EPC APN IE into EPS PDN "\
               "Connection IE"));
      }
   }

   /* IPv4 Address */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding IPv4 Address IE into EPS PDN "\
            "Connection IE"));
      egIe.t.ipAddr.type = pEsmCb->ueCb->ueTunnelCb->ipAddr.type;
      egIe.t.ipAddr.u.ipv4 = pEsmCb->ueCb->ueTunnelCb->ipAddr.u.ipv4NetAddr;
      ret = vbMmeEgtUtlAddEpsPdnConnIe(pEgMsg, EG_IPADDR_IETYPE, 0,\
            EG_IP_ADDR, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding IPv4 Address IE into EPS "\
               "PDN Connection IE"));
      }
   }

   /* LBI */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding LBI IE into EPS PDN "\
            "Connection IE"));
      egIe.t.valUnSgnd8 = pEsmCb->bId;
      ret = vbMmeEgtUtlAddEpsPdnConnIe(pEgMsg, EG_EBI_IETYPE, 0,\
            EG_UNSIGNED_8, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding LBI IE into EPS "\
               "PDN Connection IE"));
      }
   }

   /* PGW S5/S8 F-TEID */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding PGW S5/S8 F-TEID IE into "\
            "EPS PDN Connection IE"));

      egIe.ieInst = 0;
      egIe.t.fTeid.teid = pEsmCb->pdnCb->s5PgwTeId;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S5_S8_PGW_GTP_C;
      vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
            &(pEsmCb->pdnCb->s5PgwAddr.u.ipv4NetAddr));
      ret = vbMmeEgtUtlAddEpsPdnConnIe(pEgMsg, EG_FTEID_IETYPE, 0,\
            EG_FTEID, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding PGW S5/S8 F-TEID IE into "\
               "EPS PDN Connection IE"));
      }
   }

   /* Bearer Context IE */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Bearer Context IE into "\
            "EPS PDN Connection IE"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      ret = vbMmeEgtUtlAddEpsPdnConnIe(pEgMsg, EG_BEARER_CNTX_IETYPE, 0,\
            EG_GROUPED, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Bearer Context IE into "\
               "EPS PDN Connection IE"));
      }
   }

   /* EBI in Bearer Context IE in PDN Connection IE*/
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding EBI Ie in Bearer Context IE "
            "of EPS PDN Connection IE"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.t.valUnSgnd8 = pEsmCb->bId;
      ret = vbMmeEgtUtlAddEpsPdnConnBCtxIe(pEgMsg, EG_EBI_IETYPE, 0,\
            EG_UNSIGNED_8, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding EBI Ie in Bearer Context IE"\
               " of EPS PDN Connection IE"));
      }
   }

   /* SGW S1U F-TEID in Bearer Context IE in PDN Connection IE*/
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding SGW S1-U FTEID Ie in "\
            "Bearer Context IE of EPS PDN Connection IE"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.ieInst = 0;
      egIe.t.fTeid.teid = pEsmCb->s1SgwTeId;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S1_U_SGW_GTP_U;
      vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
            &(pEsmCb->s1SgwAddr.u.ipv4NetAddr));
      ret = vbMmeEgtUtlAddEpsPdnConnBCtxIe(pEgMsg, EG_FTEID_IETYPE, 0,\
            EG_FTEID, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding SGW S1-U FTEID Ie in "\
               "Bearer Context IE of EPS PDN Connection IE"));
      }
   }

   /* PGW S5/S8 U FTEID in Bearer Context IE in PDN Connection IE*/
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding PGW S5/S8-U FTEID Ie in "\
            "Bearer Context IE of EPS PDN Connection IE"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.ieInst = 0;
      egIe.t.fTeid.teid = pEsmCb->s5PgwUTeid;
      egIe.t.fTeid.isIPv4Pres = TRUE;
      egIe.t.fTeid.intfType = EGT_INT_S5_S8_SGW_GTP_U;
      vbUtlIpv4InetToArr((egIe.t.fTeid.ip4Addr),\
            &(pEsmCb->s5PgwUAddr.u.ipv4NetAddr));
      ret = vbMmeEgtUtlAddEpsPdnConnBCtxIe(pEgMsg, EG_FTEID_IETYPE, 1,\
            EG_FTEID, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding PGW S5/S8-U FTEID Ie in "\
               "Bearer Context IE of EPS PDN Connection IE"));
      }
   }

   /* Bearer QoS in Bearer Context IE in PDN Connection IE*/
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding Bearer QoS Ie in "\
            "Bearer Context IE of EPS PDN Connection IE"));

      pQos = &pUeCb->ueCtxt.ueHssCtxt.subCntxt[0].qos;
      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.t.bQos.qci = pQos->qci;
      egIe.t.bQos.prityLvl = pQos->arp.priLevel;
      egIe.t.bQos.isPCIPres = pQos->arp.preCapbFlg;
      egIe.t.bQos.isPVIPres = pQos->arp.preVlnbFlg;
      vbMmeUtlFillGtpBitRate(&(egIe.t.bQos.upMBR), 0);
      vbMmeUtlFillGtpBitRate(&(egIe.t.bQos.dnMBR), 0);
      vbMmeUtlFillGtpBitRate(&(egIe.t.bQos.upGBR), 0);
      vbMmeUtlFillGtpBitRate(&(egIe.t.bQos.dnGBR), 0);

      ret = vbMmeEgtUtlAddEpsPdnConnBCtxIe(pEgMsg, EG_BEARER_QOS_IETYPE, 0,\
            EG_BQOS_IEDATA, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding Bearer QoS Ie in "\
               "Bearer Context IE of EPS PDN Connection IE"));
      }
   }

   /* APN AMBR */
   if (ROK == ret)
   {
      VB_MME_DBG_INFO((VB_MME_PRNTBUF, "Adding APN-AMBR IE into "\
            "EPS PDN Connection IE"));

      cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
      egIe.t.ambr.dnlAPNAMBR = pEsmCb->ueCb->ueCtxt.ueHssCtxt.subUeAMBR.dl;
      egIe.t.ambr.uplAPNAMBR = pEsmCb->ueCb->ueCtxt.ueHssCtxt.subUeAMBR.ul;
      ret = vbMmeEgtUtlAddEpsPdnConnIe(pEgMsg, EG_AMBR_IETYPE, 0,\
            EG_AMBR, &egIe);
      if (ROK != ret)
      {
         VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Adding APN-AMBR IE into "\
               "EPS PDN Connection IE"));
      }
   }

   RETVALUE(ret);
} /* vbMmeEgtUtlAddEpsPdn */


/**************************************************************************//**
 *
 * @brief 
 *    Adds and E-UTRAN Transparent Container IE into GTP Message
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to EUTRAN transaprent container
 * 
 * @return
 *    SUCCESS if E-UTRAN Transparent container IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddEUTransCont
(
EgMsg             *pEgMsg,
VbMmeEUTransCont  *pEuTransCont
)
{
   S16               ret = ROK;
   EgIe              egIe;
   EgIeLevelInfo     ieLvlInfo;

   VB_TRC2(vbMmeEgtUtlAddEUTransCont);

   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_F_CONTAINER_IETYPE, 0);
   egIe.t.fContainer.containerType = 3; /* EUTRAN Transparent container */
   egIe.t.fContainer.fContainer.length = pEuTransCont->len;
   cmMemcpy(egIe.t.fContainer.fContainer.val,\
         pEuTransCont->pVal, pEuTransCont->len);
   egUtlEgtBuildEgIeHeader(&egIe, EG_F_CONTAINER_IETYPE, 0, EG_F_CONT);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, &egIe);

   RETVALUE(ret);
} /* vbMmeEgtUtlAddEUTransCont */

/**************************************************************************//**
 *
 * @brief 
 *    Adds and UTRAN Transparent Container IE into GTP Message
 *             
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * @param[in]  pUeCb
 *    Pointer to UTRAN transaprent container
 * 
 * @return
 *    SUCCESS if UTRAN Transparent container IE is added successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddUtranTransCont
(
EgMsg             *pEgMsg,
VbMmeEUTransCont  *pEuTransCont
)
{
   S16               ret = ROK;
   EgIe              egIe;
   EgIeLevelInfo     ieLvlInfo;

   VB_TRC2(vbMmeEgtUtlAddUtranTransCont);

   cmMemset((U8 *)&egIe, 0, sizeof(EgIe));
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_F_CONTAINER_IETYPE, 0);
   egIe.t.fContainer.containerType = 1; /* UTRAN Transparent container */
   egIe.t.fContainer.fContainer.length = pEuTransCont->len;
   cmMemcpy(egIe.t.fContainer.fContainer.val,\
         pEuTransCont->pVal, pEuTransCont->len);
   egUtlEgtBuildEgIeHeader(&egIe, EG_F_CONTAINER_IETYPE, 0, EG_F_CONT);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, &egIe);

   RETVALUE(ret);
} /* vbMmeEgtUtlAddEUTransCont */

/**************************************************************************//**
 *
 * @brief 
 *    Adds an IE inside RAB Bearer Context IE
 *             
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into Bearer Context IE
 * @param[in]  ieInst
 *    Instance of IE to be added into Bearer Context IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into Bearer Context IE
 * @param[in]  pEgIe 
 *    Pointer to ie details data structure
 * 
 * @return
 *    SUCCESS if IE is added into Bearer Context IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddRABBCtxIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlAddRABBCtxIe);

   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 1);
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   egUtlEgtBuildEgIeHeader(pEgIe, ieType, ieInst, ieDataType);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, pEgIe);

   RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief 
 *    Adds an IE inside Bearer Context IE
 *             
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into Bearer Context IE
 * @param[in]  ieInst
 *    Instance of IE to be added into Bearer Context IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into Bearer Context IE
 * @param[in]  pEgIe 
 *    Pointer to ie details data structure
 * 
 * @return
 *    SUCCESS if IE is added into Bearer Context IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddBCtxIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlAddBCtxIe);

   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_BEARER_CNTX_IETYPE, 0);
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   egUtlEgtBuildEgIeHeader(pEgIe, ieType, ieInst, ieDataType);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, pEgIe);

   RETVALUE(ret);
}

/**************************************************************************//**
 *
 * @brief 
 *    Adds an IE inside EPS PDN Connection
 *             
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into EPS PDN Connection IE
 * @param[in]  ieInst
 *    Instance of IE to be added into EPS PDN Connection IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into EPS PDN Connection IE
 * @param[in]  pEgIe 
 *    Pointer to ie details data structure
 * 
 * @return
 *    SUCCESS if IE is added into EPS PDN Connection IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddEpsPdnConnIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlAddEpsPdnConnIe);

   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_PDN_CONN_IETYPE, 0);
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, ieType, ieInst);
   egUtlEgtBuildEgIeHeader(pEgIe, ieType, ieInst, ieDataType);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, pEgIe);

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Adds an IE inside Bearer Context IE in EPS PDN Connection
 *             
 * @param[inout]  pEgMsg
 *    Pointer to GTP message structure
 * @param[in]  ieType
 *    Type of IE to be added into Bearer Context IE
 * @param[in]  ieInst
 *    Instance of IE to be added into Bearer Context IE
 * @param[in]  ieDataType
 *    Data Type of IE to be added into Bearer Context IE
 * @param[in]  pEgIe 
 *    Pointer to ie details data structure
 * 
 * @return
 *    SUCCESS if IE is added into EPS PDN Connection IE successfully
 *    otherwise FAILURE
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlAddEpsPdnConnBCtxIe
(
EgMsg          *pEgMsg,
U32            ieType,
U8             ieInst,
EgDataTypes    ieDataType,
EgIe           *pEgIe
)
{
   EgIeLevelInfo     ieLvlInfo = {0};
   S16               ret       = ROK;

   VB_TRC2(vbMmeEgtUtlAddEpsPdnConnBCtxIe);

   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 0, EG_PDN_CONN_IETYPE, 0);
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 1, EG_BEARER_CNTX_IETYPE, 0);
   VB_MME_SET_IE_LVL_INFO(ieLvlInfo, 2, ieType, ieInst);
   egUtlEgtBuildEgIeHeader(pEgIe, ieType, ieInst, ieDataType);

   ret = EgUtilGmAppendIe(pEgMsg, &ieLvlInfo, pEgIe);

   RETVALUE(ret);
}


/**************************************************************************//**
 *
 * @brief 
 *    Sends failure response to SGW
 *             
 * @param[in]  locTeid
 *    Local S11 Teid, if available. if not available pass 0
 * @param[in]  remTeid
 *    Remove Tunnel Teid, if available. if not available pass 0
 * @param[in]  msgType
 *    Type Response message
 * @param[in]  transId
 *    Transaction ID
 * @param[in]  cause
 *    Cause value to be sent in response
 * 
 * @return
 *    SUCCESS if response message is sent successfully to SGW
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlSndFailureRsp
(
U32            locTeid,
U32            remTeid,
EgtMsgType     msgType,
U32            transId,
U32            cause  
)
{
   TknU32         localTeId = {0};
   EgMsg          *rspMsg = NULLP;
   Mem            memInfo;
   EgIe           egIe;

   VB_TRC2(vbMmeEgtUtlSndFailureRsp);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   localTeId.pres = TRUE;
   localTeId.val  = locTeid;

   if (egUtlAllocEgMsg(&rspMsg, &memInfo) != ROK )
   {
      VB_MME_DBG_ERROR((VB_MME_PRNTBUF, "Allocating Memory: GTP Message"));
      RETVALUE(RFAILED);
   }

   /* Build the Header  with Default values */
   egUtlBuildEgHeader(&(rspMsg->msgHdr), remTeid, msgType, transId);

   /* Cause IE */
   cmMemset((U8 *)&egIe, 0 , sizeof(EgIe));
   egIe.t.cause.causeVal = cause;
   vbMmeEgtUtlAppendIe(rspMsg, EG_CAUSE_IETYPE, 0, EG_CAUSE, &egIe);

   VbLiEgtSigRsp(&(vbMmeCb.egtS10SapCb->pst), (vbMmeCb.egtS10SapCb->spId),\
         localTeId, rspMsg);

   RETVALUE(ROK);
} /* vbMmeEgtUtlSndFailureRsp */


/**************************************************************************//**
 *
 * @brief 
 *    Retrieves IMSI from GTP message to be store in 15 digit string
 *             
 * @param[in]  imsi
 *    gtp imsi buffer
 * @param[in]  len
 *    gtp imsi buffer length
 * @param[inout]  ueImsi
 *    15 digit imsi buffer
 * 
 * @return
 *    Length of imsi
 *
 *****************************************************************************/
PUBLIC S16 vbMmeEgtUtlGetImsi
(
U8             *imsi,
U32            len,
U8             *ueImsi
)
{
   S16      ueImsiLen = 0;
   U32      indx = 0;

   VB_TRC2(vbMmeEgtUtlGetImsi);

   for (indx = 0; indx < len; indx++)
   {
      ueImsi[ueImsiLen] = imsi[indx] & 0x0f; 
      ueImsiLen++;

      if (ueImsiLen < VB_HSS_IMSI_LEN)
      {
         ueImsi[ueImsiLen] = (imsi[indx] & 0xf0) >> 4; 
         ueImsiLen++;
      }
   }

   RETVALUE(ueImsiLen);
} /* vbMmeEgtUtlGetImsi */


/**************************************************************************//**
 *
 * @brief 
 *    Builds Release Access bearer Request
 *             
 * @param[in]  pUeCb
 *    Pointer to UE Control Block
 * @param[in]  pEgMsg
 *    Pointer to GTP message
 * 
 * @return
 *    SUCCESS if Release Access Bearer Request is built successfully
 *    otherwise FAILURE
 *****************************************************************************/
PUBLIC S16 vbMmeUtlBldRABReq
(
VbMmeUeCb         *pUeCb,
EgMsg             **ppEgMsg
)
{
   Mem               memInfo        = {0};
   VbMmeUeTunnelCb   *pS11Tun    = NULLP;

   VB_TRC2(vbMmeUtlBldRABReq);

   memInfo.region = vbMmeCb.init.region;
   memInfo.pool   = vbMmeCb.init.pool;
   if (egUtlAllocEgMsg(ppEgMsg, &memInfo) != ROK )
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB049, ERRZERO, "Allocating Memory: "\
            "EgMsg");
      RETVALUE(RFAILED);
   }

   pS11Tun = pUeCb->ueTunnelCb;
   egUtlBuildEgHeader(&((*ppEgMsg)->msgHdr), pS11Tun->remoteTeId,\
         EGT_GTPC_MSG_RAB_REQ, ++pS11Tun->seqNum);

   RETVALUE(ROK);
} /* vbMmeUtlBldRABReq */


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
*********************************************************************91*/
