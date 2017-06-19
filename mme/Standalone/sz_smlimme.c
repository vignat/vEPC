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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for lower interface state machine on MME

     File:    sq_smlimme.c

     Sid:      sq_smlimme.c@@/main/nodal_main/3 - Fri Nov 22 17:55:51 2013

     Prg:      sn

*********************************************************************21*/
/* Header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "sct.h"           /* SCTP Layer */
#include "szt.h"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP LM Interface */
#include "sz.h"            /* S1AP Layer */
#include "sz_err.h"
#include "szt_asn.h"
#include "cm_pasn.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

/* Header include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCTP Layer */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* S1AP Upper Interface */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP LM Interface */
#include "cm_pasn.x"
#include "sz.x"            /* S1AP Layer */
#include "szt_asn.x"

#ifdef SZ_MME
#ifdef __cplusplus
EXTERN "C" {
#endif  /* __cplusplus */
PUBLIC S16 szSmRcvUeCtxRlsCmp ARGS((SzConCb *conCb,
                                    SzPeerCb *peerCb,
                                    S1apPdu *pdu));
#ifdef __cplusplus
}
#endif /* __cplusplus */

PFRAM szSmLiMme[SZ_MAX_MSG_ID][SZ_MAX_ST] =
{
   /* E-RAB setup request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* E-RAB setup response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* E-RAB modify request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* E-RAB modify response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* E-RAB release command */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /*  E-RAB release complete */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* E-RAB release request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Initial Context setup request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Initial Context setup response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Initial Context setup failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* UE Context release request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* UE Context release command */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /*  UE Context release complete */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmRcvUeCtxRlsCmp,    /* Releasing   */
   },
   /* UE Context modification request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* UE Context modification response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* UE Context modification failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Initial UE message */
   {
      szSmRcvIdleStMsg,      /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Downlink NAS transport */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Uplink NAS transport */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmRcvConnStMsg,      /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* NAS non delivery indication */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Error indication */
   {
      szSmRcvIdleStErrInd,   /* Idle        */
      szSmRcvConnStErrInd,   /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmLclErrState        /* Releasing   */
   },
   /* No Data */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Reset */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Reset Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Setup Request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Setup Response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Setup Fail */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Paging */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Required */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Command */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Preperation Failure */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Request */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Request Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmRcvConnStMsg,      /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Request Failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmRcvConnStHoFail,   /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Notify */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Path Switch Request */
   {
      szSmRcvIdleStMsg,      /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Path Switch Request Acknowledge */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Path Switch Request Failure */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Handover Cancel */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Handover Cancel Acknowledge */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* ENB Status Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Status Transfer */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Deactivate Trace */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Trace Start */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Trace Failure Indication */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Location Reporting Control */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* Location Report Failure Indication */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Location Report */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Update */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Update Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Update Failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Update */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Update Acknowledge */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Update Failure */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Uplink Cdma 2000 Tunneling */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Downlink CDMA 2000 Tunneling */
   {
      szSmSemPeerErr,       /* Idle        */
      szSmSemPeerErr,       /* Connecting  */
      szSmSemPeerErr,       /* Estbalished */
      szSmSemPeerErr        /* Releasing   */
   },
   /* UE Capability Info */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Overload Start */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Overload Stop */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Write Replace Warning Request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Write Replace Warning Response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Information Direct Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Information Direct Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Private Message */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* ENB Configuration Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* MME Configuration Transfer */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Cell Traffic Trace */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Estbalished */
      szSmPeerErrHdlr        /* Releasing   */
   }
#ifndef S1AP_REL8A0
   ,
   /* Kill Request */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Kill Response */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Downlink UE Associated LPPa Transport  */
   {
      szSmSemPeerErr,        /* Idle        */
      szSmSemPeerErr,        /* Connecting  */
      szSmSemPeerErr,        /* Established */
      szSmSemPeerErr         /* Releasing   */
   },
   /* Uplink UE Associated LPPa Transport  */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmRcvEstStMsg,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Downlink non-UE Associated LPPa Transport  */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   },
   /* Uplink non-UE Associated LPPa Transport  */
   {
      szSmIdleStPeerErr,     /* Idle        */
      szSmPeerErrHdlr,       /* Connecting  */
      szSmPeerErrHdlr,       /* Established */
      szSmPeerErrHdlr        /* Releasing   */
   }
#endif /* S1AP_REL8A0 */
}; /* End of  szSmLiMme */

/*
*
*       Fun:  szSmRcvUeCtxRlsCmp 
*
*       Desc:  Called to receive UE context release complete 
*
*       Ret:   ok:     ROK
*              failed: RFAILED
*
*       Notes: None
*
*       File:  sq_smlimme.c
*
*/

#ifdef ANSI
PUBLIC S16 szSmRcvUeCtxRlsCmp
(
SzConCb  *conCb,
SzPeerCb *peerCb,
S1apPdu  *pdu
)
#else /* ANSI */
PUBLIC S16 szSmRcvUeCtxRlsCmp(conCb, peerCb, pdu)
SzConCb  *conCb;
SzPeerCb *peerCb;
S1apPdu  *pdu;
#endif /* ANSI */
{
   S16 ret = RFAILED;
   SztRelCfm relCfm;

   TRC2(szSmRcvUeCtxRlsCmp)

   SZ_ZERO(&relCfm, sizeof(SztRelCfm));
  
   relCfm.suConnId = conCb->cd.suConnId;
   relCfm.spConnId = conCb->cd.spConnId;
   relCfm.relType  = SZT_CXT_REL;
   relCfm.pdu      = pdu; 

   /* Send decoded data to its User */
   ret = szUiSndRelCfm(conCb, &relCfm);
                                                                                
#if (ERRCLASS & ERRCLS_DEBUG)
   if(ret == RFAILED)
   {
      SZLOGERROR(ERRCLS_DEBUG, ESZ165, (ErrVal) conCb->lclConRefNo,
           "szSmRcvUeCtxRlsCmp: Upper Interface send failed");
      SZ_FREE_SDU(pdu);
      RETVALUE(ret);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
   /* Deallocate connection control block */
   ret = szNdbDeallocConCb(conCb->lclConRefNo,peerCb, SZ_CONN_REF_LCL);
#if (ERRCLASS & ERRCLS_DEBUG)
   if(ret == RFAILED)
   {
      SZLOGERROR(ERRCLS_DEBUG, ESZ166, (ErrVal) conCb->lclConRefNo,
           "szSmRcvUeCtxRlsCmp: Couldn't retrieve Connection CB");
   }
#endif  /* ERRCLASS & ERRCLS_DEBUG */
   RETVALUE(ret);
} /* end of szSmRcvUeCtxRlsCmp */
#endif /* SZ_MME */

/**********************************************************************
 
         End of file:     sq_smlimme.c@@/main/nodal_main/3 - Fri Nov 22 17:55:51 2013
 
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
/main/5      ---      va          1. Updated for S1AP release 4.1 
*********************************************************************91*/
