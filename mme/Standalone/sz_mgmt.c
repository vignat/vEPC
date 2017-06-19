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
 
     Name:     S1AP - Common Procedures Module
  
     Type:     C source file
  
     Desc:     C source code for Common / Management procedures Module
  
     File:     sz_mgmt.c
  
     Sid:      sq_mgmt.c@@/main/nodal_main/6 - Fri Nov 22 18:33:24 2013
  
     Prg:      nm
  
*********************************************************************21*/


/************************************************************************
 
     Note: 
 
************************************************************************/



/* header include files -- defines (.h) */
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
#include "cm_pasn.h"

#ifdef SZ_FTHA
#include "cm_psfft.h"
#include "cm_ftha.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */

#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* SZT interface defines           */
#include "szt_asn.h"
#include "lsz.h"           /* S1AP Layer management           */
#ifdef YT
#include "lyt.h"            /* S1AP-PSF                        */
#include "yt.h"            /* S1AP-PSF                        */
#endif /* YT */
#include "sz.h"            /* S1AP layer defines              */
#include "sz_err.h"        /* S1AP - error */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif

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
#include "cm_pasn.x"
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_pftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */

#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCT interface defines           */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* SZT interface structures        */
#include "lsz.x"           /* S1AP Layer management           */
#ifdef YT
#include "lyt.x"            /* S1AP-PSF                        */
#include "yt.x"            /* S1AP-PSF                        */
#endif /* YT */
#include "sz.x"            /* S1AP layer structures           */
  
/* local defines */
  
/* local externs */
  
/* forward references */

/* public variable declarations */
  
/* This structure holds all the global structs we need. */

/* private variable declarations */

/* Private function declarations */
/* sz008.301: fix for kwork fix */
PRIVATE Bool szMgmtChkConnLst ARGS((
        SztResetTyp *tknIE,
        S1apPdu     *pdu,
        U8          *cause
        ));

/*******************************************************************************

  Management module handles the following Common Procedures :

  *   Paging and
  *   S1 Interface Management Procedures :
         *  S1 Setup
         *  Reset and
         *  Error Indication.  
  
*******************************************************************************/


#ifdef SZ_MME
PFSZMGMTUIMSGHANDLER szMgmtUiMme[SZ_MAX_MSG_ID] =
{
   szMgmtUiInvldMsgHandler,     /* E-RAB setup request */
   szMgmtUiInvldMsgHandler,     /* E-RAB setup response */
   szMgmtUiInvldMsgHandler,     /* E-RAB modify request */
   szMgmtUiInvldMsgHandler,     /* E-RAB modify response */
   szMgmtUiInvldMsgHandler,     /* E-RAB release command */
   szMgmtUiInvldMsgHandler,     /* E-RAB release complete */
   szMgmtUiInvldMsgHandler,     /* E-RAB release request */
   szMgmtUiInvldMsgHandler,     /* Initial Context setup request */
   szMgmtUiInvldMsgHandler,     /* Initial Context setup response */
   szMgmtUiInvldMsgHandler,     /* Initial Context setup failure */
   szMgmtUiInvldMsgHandler,     /* UE Context release request */
   szMgmtUiInvldMsgHandler,     /* UE Context release command */
   szMgmtUiInvldMsgHandler,     /* UE Context release complete */
   szMgmtUiInvldMsgHandler,     /* UE Context modification request */
   szMgmtUiInvldMsgHandler,     /* UE Context modification response */
   szMgmtUiInvldMsgHandler,     /* UE Context modification failure */
   szMgmtUiInvldMsgHandler,     /* Initial UE message */
   szMgmtUiInvldMsgHandler,     /* Downlink NAS transport */
   szMgmtUiInvldMsgHandler,     /* Uplink NAS transport */
   szMgmtUiInvldMsgHandler,     /* NAS non delivery indication */
   szMgmtUiErrInd,              /* Error indication */
   szMgmtUiInvldMsgHandler,     /* No Data */
   szMgmtUiRst,                 /* Reset */
   szMgmtUiRstAck,              /* Reset Acknowledge */
   szMgmtUiInvldMsgHandler,     /* Setup Request */
   szMgmtUiSetupRsp,            /* Setup Response */
   szMgmtUiSetupFailure,        /* Setup Fail */
   szMgmtUiMsgHandler,          /* Paging */
   szMgmtUiInvldMsgHandler,     /* Handover Required */
   szMgmtUiInvldMsgHandler,     /* Handover Command */
   szMgmtUiInvldMsgHandler,     /* Handover Preperation Failure */
   szMgmtUiInvldMsgHandler,     /* Handover Request */
   szMgmtUiInvldMsgHandler,     /* Handover Request Acknowledge */
   szMgmtUiInvldMsgHandler,     /* Handover Request Failure */
   szMgmtUiInvldMsgHandler,     /* Handover Notify */
   szMgmtUiInvldMsgHandler,     /* Path Switch Request */
   szMgmtUiInvldMsgHandler,     /* Path Switch Request Acknowledge */
   szMgmtUiInvldMsgHandler,     /* Path Switch Request Failure */
   szMgmtUiInvldMsgHandler,     /* Handover Cancel */
   szMgmtUiInvldMsgHandler,     /* Handover Cancel Acknowledge */
   szMgmtUiInvldMsgHandler,     /* ENB Status Transfer */
   szMgmtUiInvldMsgHandler,     /* MME Status Transfer */
   szMgmtUiInvldMsgHandler,     /* Deactivate Trace */
   szMgmtUiInvldMsgHandler,     /* Trace Start */
   szMgmtUiInvldMsgHandler,     /* Trace Failure Indication */
   szMgmtUiInvldMsgHandler,     /* Location Reporting Control */
   szMgmtUiInvldMsgHandler,     /* Location Report Failure Indication */
   szMgmtUiInvldMsgHandler,     /* Location Report */
   szMgmtUiInvldMsgHandler,     /* ENB Configuration Update */
   szMgmtUiMsgHandler,          /* ENB Configuration Update Acknowledge */
   szMgmtUiMsgHandler,          /* ENB Configuration Update Failure */
   szMgmtUiMsgHandler,          /* MME Configuration Update */
   szMgmtUiInvldMsgHandler,     /* MME Configuration Update Acknowledge */
   szMgmtUiInvldMsgHandler,     /* MME Configuration Update Failure */
   szMgmtUiInvldMsgHandler,     /* Uplink CDMA 2000 Tunneling */
   szMgmtUiInvldMsgHandler,     /* Downlink CDMA 2000 Tunneling */
   szMgmtUiInvldMsgHandler,     /* UE Capability Info Indication */
   szMgmtUiMsgHandler,          /* Overload Start */
   szMgmtUiMsgHandler,          /* Overload Stop */
   szMgmtUiMsgHandler,          /* Write Replace Warning Request */
   szMgmtUiInvldMsgHandler,     /* Write Replace Warning Response */
   szMgmtUiInvldMsgHandler,     /* ENB Information Direct Transfer */
   szMgmtUiMsgHandler,          /* MME Information Direct Transfer */
   szMgmtUiInvldMsgHandler,     /* Private message */
   szMgmtUiInvldMsgHandler,     /* ENB Configuration Transfer */
   szMgmtUiMsgHandler,          /* MME Configuration Transfer */
   szMgmtUiInvldMsgHandler     /* Cell Traffic Trace */
#ifndef S1AP_REL8A0
   ,
   szMgmtUiMsgHandler,          /* Kill Request */
   szMgmtUiInvldMsgHandler,     /* Kill Response */
   szMgmtUiInvldMsgHandler,     /* Downlink UE Associated LPPa Transport */
   szMgmtUiInvldMsgHandler,     /* Uplink UE Associated LPPa Transport */
   szMgmtUiMsgHandler,          /* Downlink non-UE Associated LPPa Transport */
   szMgmtUiInvldMsgHandler      /* Uplink non-UE Associated LPPa Transport */
#endif /* S1AP_REL8A0 */
};
#endif /* SZ_MME */

/*
 *
 *       Fun:   szMgmtUiInvldMsgHandler - Handler for Setup Response.
 *
 *       Desc:  This function handles the Invalid management message
 *              received from the User
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: Incase if the bypassing of UE state machine is enabled,
 *              This function will encode and send the message to peer
 *              Node.
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiInvldMsgHandler
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */
)
#else  /* ANSI */
PUBLIC S16 szMgmtUiInvldMsgHandler(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb  *uSapCb;                  /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif  /* ANSI */
{
   S16        ret = RFAILED;
   
   TRC2(szMgmtUiInvldMsgHandler);

   /* Validate if the bypassing of UE state machine is enabled, if it is
    * enabled will ignore the UE state machine */
#ifdef LSZV1
   if(peerCb->skipUeSm)
   {
      /* Check state of the node */
      if (peerCb->state != SZT_ND_ST_NORMAL)
      {
         *cause = SZT_CAUSE_INV_STATE;
         RETVALUE(ret);
      } 

      /* Send encoded data to the peer */
      if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
      {
         if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
               (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
         {
            *cause = (U8) ret; 
         }
         else
         {
            *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
         }
         RETVALUE(ret);
      } 
      ret = ROK;
   }
   else
   {
#endif /* LSZV1 */

   /* Report error incase if the skip UE state machine is not enabled */

   *cause = SZT_CAUSE_UNEXP_MSG;
#ifdef LSZV1
   }
#endif /* LSZV1 */

   RETVALUE(ret);
} /* szMgmtUiInvldMsgHandler */

#ifdef SZ_MME

/*
 *
 *       Fun:   szMgmtUiSetupRsp - Handler for Setup Response.
 *
 *       Desc:  This function handles the Setup response message
 *              received from the User
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiSetupRsp
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */
)
#else  /* ANSI */
PUBLIC S16 szMgmtUiSetupRsp(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb  *uSapCb;                  /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif  /* ANSI */
{
   S16        ret = ROK;             /* return value */ 

   TRC2(szMgmtUiSetupRsp);

   /* Check the state of the node */
   if (peerCb->state != SZT_ND_ST_SETUP_IN_PRG)
   {
      *cause = SZT_CAUSE_INV_STATE;
      RETVALUE(RFAILED);
   } 

   /* Change the state to NORMAL */ 
   peerCb->state = SZT_ND_ST_NORMAL;

   /* Send encoded data to the peer */
   if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         *cause = (U8) ret; 
      }
      else
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
      }
   }    
   /* Send the runtime update message for PeerCb */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_PEER_CB, peerCb);
#endif /* YT */

   RETVALUE(ret);

} /* szMgmtUiSetupRsp */

/*
 *
 *       Fun:   szMgmtUiSetupFailure - Handler for Setup Failure.
 *
 *       Desc:  This function handles the Setup failure message
 *              received from the User
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiSetupFailure
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */
)
#else  /* ANSI */
PUBLIC S16 szMgmtUiSetupFailure(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb  *uSapCb;                 /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif  /* ANSI */
{
   S16        ret = ROK;             /* return value */ 

   TRC2(szMgmtUiSetupFailure);

   /* Check the state of the node */
   if (peerCb->state != SZT_ND_ST_SETUP_IN_PRG)
   {
      *cause = SZT_CAUSE_INV_STATE;
      RETVALUE(RFAILED);
   } 

   /* Change the state to IDLE; setup failed, so it can not 
      be moved to normal state */ 

   peerCb->state = SZT_ND_ST_IDLE;

   /* Send encoded data to the peer */
   if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         *cause = (U8) ret; 
      }
      else
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
      }
   }    
   
   RETVALUE(ret);

} /* szMgmtUiSetupFailure */

/*
 *
 *       Fun:   szMgmtLiSetupReq - Handler for Setup Request
 *
 *       Desc:  This function handles Setup Request message
 *              received from the Peer.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtLiSetupReq
(
SzPeerCb   *peerCb,                  /* Peer control block */
S1apPdu    *pdu                      /* S1AP PDU */
)
#else  /* ANSI */
PUBLIC S16 szMgmtLiSetupReq(peerCb, pdu)
SzPeerCb   *peerCb;                  /* Peer control block */
S1apPdu    *pdu;                     /* S1AP PDU */
#endif  /* ANSI */
{
   S16         ret = ROK;            /* Return value */ 
   SztUDatEvnt uDatEvnt;             /* Connectionless data */

   TRC2(szMgmtLiSetupReq);

   /* Change state to setup in progress */
   peerCb->state = SZT_ND_ST_SETUP_IN_PRG;

   SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));

   uDatEvnt.transId.pres = NOTPRSNT; 
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = peerCb->peerId;
   uDatEvnt.pdu = pdu;

#ifdef LSZV1
   if(peerCb->skipUeSm == FALSE)
   {
#endif /* LSZV1 */
      (Void) szNdbRelConns(peerCb);
#ifdef LSZV1
   }
#endif /* LSZV1 */

   /* Send data to the user */
   ret = szUiSndUDatInd(peerCb, &uDatEvnt);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SZLOGERROR(ERRCLS_DEBUG, ESZ064, (ErrVal)peerCb->peerId,
                 "szMgmtLiSetupReq : Failed to send data to the user");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   RETVALUE(ret);

} /* szMgmtLiSetupReq */
#endif /* SZ_MME */


/*
 *
 *       Fun:   szMgmtUiMsgHandler - Common handler for connection-less message 
 *
 *       Desc:  This function does common handling of the connection-less
 *              message received from the User
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiMsgHandler
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */
)
#else  /* ANSI */
PUBLIC S16 szMgmtUiMsgHandler(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb  *uSapCb;                 /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif  /* ANSI */
{
   S16        ret = ROK;             /* return value */ 

   TRC2(szMgmtUiMsgHandler);

   /* Check state of the node */
   if (peerCb->state != SZT_ND_ST_NORMAL)
   {
      *cause = SZT_CAUSE_INV_STATE;
      RETVALUE(RFAILED);
   } 

   /* Send encoded data to the peer */
   if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         *cause = (U8) ret; 
      }
      else
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
      }
   }    

   RETVALUE(ret);

} /* szMgmtUiMsgHandler */

/*
 *
 *       Fun:   szMgmtUiRst - Handler for Reset 
 *
 *       Desc:  This function handles the Reset message received from the User.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiRst
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */
)
#else  /* ANSI */
PUBLIC S16 szMgmtUiRst(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb  *uSapCb;                 /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif  /* ANSI */
{
   S16         ret = ROK;             /* return value */
   S1apPdu     *rstAckPdu = NULLP;    /* PDU for Reset Ack */
   TknU8       *tknIE = NULLP;        /* Reset type IE */
   Mem         mem;                   /* Memory block */
   U8          idx;                   /* Index */
   U16         nmbConns;              /* Number of connections in Reset */
   SzConRefNo  conRefNo;             /* Connection Id received in the Reset */
   SztUDatEvnt uDatEvnt;             /* Connectionless data */
   U8          conRefType;           /* Connection reference type */

   TRC2(szMgmtUiRst);

   SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));

#ifdef LSZV1
   if(peerCb->skipUeSm == FALSE)
   {
#endif /* LSZV1 */
      /* Get S1 connection list from DB */
      szGetIE(&pdu->pdu, Sztid_ResetTyp, &tknIE);

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(tknIE == NULLP)
      {
         SZ_FREE_SDU(pdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      mem.region = uSapCb->pst.region; 
      mem.pool   = uSapCb->pst.pool;

      /* Check if the node is in RESET state already */
      if (peerCb->state == SZT_ND_ST_RESET)
      {
         /* If connections are similar, send reset ack to the user */
         /* sz008.301:  Fix for kwork */
         if ((ret = szMgmtChkConnLst(((SztResetTyp *)tknIE), peerCb->resetPdu, cause)) == TRUE)
         {
            uDatEvnt.transId.pres = NOTPRSNT;
            uDatEvnt.peerId.pres = PRSNT_NODEF;
            uDatEvnt.peerId.val = peerCb->peerId;

            /* Fill Reset Ack to the user */
            /* sz008.301: fix for Kwork */
            ret = szDbFillResetAck(pdu, &rstAckPdu, mem, cause);
            if(ret != ROK)
            {
               RETVALUE(RFAILED);
            }
            uDatEvnt.pdu = rstAckPdu;
            SZ_FREE_SDU(pdu);
            ret = szUiSndUDatInd(peerCb, &uDatEvnt);
            RETVALUE(ret);
         }
         /* sz008.301: fix for Kwork */
         else if(*cause == SZT_CAUSE_MSING_MNDTRY_PARM)
         {
            RETVALUE(RFAILED);
         }
      }
#ifdef LSZV1
   }
#endif /* LSZV1 */

   /* Change the state of the node to Reset */
   peerCb->state = SZT_ND_ST_RESET;  

#ifdef LSZV1
   if(peerCb->skipUeSm == FALSE)
   {
#endif /* LSZV1 */
      /* Release all the connections or part of the connections 
         as per the choice in the reset */
      if (((SztResetTyp *)tknIE)->choice.val == SZ_FULL_RESET)
      {
         (Void) szNdbRelConns(peerCb);
      }
      else
      {
         /* Partial reset */
         nmbConns = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.noComp.val;
         for (idx = 0; idx < nmbConns; idx++)
         {
            /* Deallocate ConCb */
            if(((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].value.u.\
                  sztUE_assocLogS1_ConItem.pres.pres != NOTPRSNT)
            {
               /* Assigne the connection reference type */
               conRefType = SZ_CONN_REF_LCL;

               if (((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].value.u.\
                     sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres != NOTPRSNT)
               {
                  conRefNo = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].\
                             value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.val;
                  if(peerCb->nodeType == LSZ_NODE_MME)
                  {
                     conRefType = SZ_CONN_REF_RMT;
                  }
               }
               else if(((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].value.u.\
                     sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.pres != NOTPRSNT)
               {
                  conRefNo = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].\
                             value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.val;

                  if(peerCb->nodeType == LSZ_NODE_ENB)
                  {
                     conRefType = SZ_CONN_REF_RMT;
                  }
               }
               else
               {
                  uDatEvnt.transId.pres = NOTPRSNT;
                  uDatEvnt.peerId.pres = PRSNT_NODEF;
                  uDatEvnt.peerId.val = peerCb->peerId;

                  /* Fill & Keep Reset Ack to the user */
                  szDbFillResetAck(pdu, &rstAckPdu, mem, cause);
                  uDatEvnt.pdu = rstAckPdu;
                  SZ_FREE_SDU(pdu);
                  ret = szUiSndUDatInd(peerCb, &uDatEvnt);
                  RETVALUE(ret);
               }

               /* Mention the connection reference type */
               ret = szNdbDeallocConCb(conRefNo, peerCb, conRefType);
            }
         }
      }

      /* Store this connection list in peerCb, temporarily */
      if ((ret = szDbFillResetFrmPdu(pdu, &peerCb->resetPdu, mem)) != ROK)
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
         SZ_FREE_SDU(pdu);
         RETVALUE(ret);
      }
#ifdef LSZV1
   }
#endif /* LSZV1 */

   /* Send encoded data to the peer */
   if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
   { 
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         *cause = (U8) ret; 
      }
      else
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
      }
      
      SZ_FREE_SDU(peerCb->resetPdu);
      RETVALUE(ret);
   }


   RETVALUE(ret);

} /* szMgmtUiRst */

/*
 *
 *       Fun:   szMgmtUiRstAck - Handler for Reset Ack
 *
 *       Desc:  This function handles the Reset Ack received from the User.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiRstAck 
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */ 
)
#else  /* ANSI */
PUBLIC S16 szMgmtUiRstAck(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb *uSapCb;                  /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif  /* ANSI */
{
   S16         ret = ROK;             /* return value */

   TRC2(szMgmtUiRstAck);

   /* Send encoded data to the peer */
   if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         *cause = (U8) ret; 
      }
      else
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
      }
      RETVALUE(ret);
   }

   /* Change the state of the node to normal */
   peerCb->state = SZT_ND_ST_NORMAL;

   /* Remove the temporarily stored reset info from peerCb */
    SZ_FREE_SDU(peerCb->resetPdu); 

   RETVALUE(ret);

} /* szMgmtUiRstAck */

/*
 *
 *       Fun:   szMgmtLiRst - Handler for Reset 
 *
 *       Desc:  This function handles the Reset message received from the Peer.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtLiRst 
(
SzPeerCb   *peerCb,                  /* Peer control block */
S1apPdu    *pdu                      /* S1AP PDU */
)
#else  /* ANSI */
PUBLIC S16 szMgmtLiRst(peerCb, pdu)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
#endif  /* ANSI */
{
   S16         ret = ROK;            /* Return value */
   SztUDatEvnt uDatEvnt;             /* Connectionless data */
   TknU8       *tknIE = NULLP;       /* Reset Type IE */
   S1apPdu     *rstAckPdu = NULLP;   /* Reset Ack PDU */
   Mem         mem;                  /* Memory block */
   U8          idx;                  /* Index */
   U16         nmbConns = 0;         /* Number of connections in Reset */
   SzConRefNo  conRefNo;             /* Connection Id received in the Reset */
   U8          conRefType;           /* Connection reference type */
   /* sz008.301: fix for kwork */
   U8          cause;

   TRC2(szMgmtLiRst);

   SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));

#ifdef LSZV1
   if(peerCb->skipUeSm == FALSE)
   {
#endif /* LSZV1 */
      /* Get S1 connection list from DB */
      szGetIE(&pdu->pdu, Sztid_ResetTyp, &tknIE); 

#if (ERRCLASS & ERRCLS_INT_PAR)
      if(tknIE == NULLP)
      {
         SZ_FREE_SDU(pdu);
         RETVALUE(RFAILED);
      }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      mem.region = peerCb->sctSapCb->pst.region;
      mem.pool   = peerCb->sctSapCb->pst.pool;

      /* Check if the node is in RESET state already */
      if (peerCb->state == SZT_ND_ST_RESET)
      {
         /* If connections are similar, send reset ack to the peer */ 
         /* sz008.301: Fix for kwork */
         if ((ret = szMgmtChkConnLst(((SztResetTyp *)tknIE), peerCb->resetPdu, &cause)) == TRUE)
         {
            /* Fill & Keep Reset Ack to the peer */
            /* sz008.301: fix for kwork,
             * added check for return value, as we are returning other than ROK 
             * from szDbFillResetAck in error scenario */

            ret = szDbFillResetAck(pdu, &rstAckPdu, mem, &cause);
            if (ret != ROK)
            {
               SZ_FREE_SDU(pdu);
               RETVALUE(RFAILED);
            }
            szLiSndToLower(peerCb, NULLP, rstAckPdu);

            SZ_FREE_SDU(pdu);
            RETVALUE(ROK); 
         }
          /* sz008.301: fix for Kwork */
         else if(cause  == SZT_CAUSE_MSING_MNDTRY_PARM)
         {
            RETVALUE(RFAILED);
         }

      }
#ifdef LSZV1
   }
#endif /* LSZV1 */

   /* Proceed with the reset if the connections are not similar to the previous
      Reset received from user */

   /* Change the state of the node to reset */
   peerCb->state = SZT_ND_ST_RESET;

#ifdef LSZV1
   if(peerCb->skipUeSm == FALSE)
   {
#endif /* LSZV1 */

      /* Release all the connections or part of the connections 
         as per the choice in the reset */
      if (((SztResetTyp *)tknIE)->choice.val == SZ_FULL_RESET)   
      {
         (Void) szNdbRelConns(peerCb);
      }
      else
      {
         /* Partial reset */  
         nmbConns = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.noComp.val;
         for (idx = 0; idx < nmbConns; idx++)
         {
            /* Deallocate ConCb */
            if (((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].value.u.\
                  sztUE_assocLogS1_ConItem.pres.pres != NOTPRSNT)
            {
               /* Assigne the connection refence type */
               conRefType = SZ_CONN_REF_LCL;

               if (((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].value.u.\
                     sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.pres != NOTPRSNT)
               {
                  conRefNo = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].\
                             value.u.sztUE_assocLogS1_ConItem.mME_UE_S1AP_ID.val;

                  if(peerCb->nodeType == LSZ_NODE_MME)
                  {
                     conRefType = SZ_CONN_REF_RMT;
                  }
               }
               else if(((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].value.\
                     u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.pres != NOTPRSNT)
               {
                  conRefNo = ((SztResetTyp *)tknIE)->val.partOfS1_Intf.member[idx].\
                             value.u.sztUE_assocLogS1_ConItem.eNB_UE_S1AP_ID.val;

                  if(peerCb->nodeType == LSZ_NODE_ENB)
                  {
                     conRefType = SZ_CONN_REF_RMT;
                  }
               }
               else
               {
                  /* Fill & Keep Reset Ack to the peer */
                  szDbFillResetAck(pdu, &rstAckPdu, mem, &cause);
                  szLiSndToLower(peerCb, NULLP, rstAckPdu);

                  SZ_FREE_SDU(pdu);
                  RETVALUE(ROK);
               }

               /* Mention the connection reference type */
               ret = szNdbDeallocConCb(conRefNo, peerCb, conRefType);
            }
         }
      }
#ifdef LSZV1
   }
#endif /* LSZV1 */

   uDatEvnt.transId.pres = NOTPRSNT;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = peerCb->peerId;
   uDatEvnt.pdu = pdu;

#ifdef LSZV1
   if(peerCb->skipUeSm == FALSE)
   {
#endif /* LSZV1 */

      /* Store this connection list in peerCb, temporarily */
      ret = szDbFillResetFrmPdu(pdu, &peerCb->resetPdu, mem);
#if (ERRCLASS & ERRCLS_DEBUG)
      if (ret != ROK)
      {
         SZLOGERROR(ERRCLS_DEBUG, ESZ069, (ErrVal)peerCb->peerId,
               "szMgmtLiRst: Failed to make a copy Reset pdu");
      }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
#ifdef LSZV1
   }
#endif /* LSZV1 */

   /* Send Reset to the user */
   if ((ret = szUiSndUDatInd(peerCb, &uDatEvnt)) != ROK)
   {
#if (ERRCLASS & ERRCLS_DEBUG)
      SZLOGERROR(ERRCLS_DEBUG, ESZ070, (ErrVal)peerCb->peerId,
                 "szMgmtLiRst: Failed to send Reset to the user");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

      SZ_FREE_SDU(peerCb->resetPdu);
   }

   RETVALUE(ret);

} /* szMgmtLiRst */

/*
 *
 *       Fun:   szMgmtLiRstAck - Handler for Reset Ack
 *
 *       Desc:  This function handles the Reset Ack received from the Peer.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtLiRstAck 
(
SzPeerCb   *peerCb,                  /* Peer control block */
S1apPdu    *pdu                      /* S1AP PDU */
)
#else  /* ANSI */
PUBLIC S16 szMgmtLiRstAck(peerCb, pdu)
SzPeerCb   *peerCb;                  /* Peer control block */
S1apPdu    *pdu;                     /* S1AP PDU */
#endif /* ANSI */
{
   S16         ret = ROK;            /* Return value */
   SztUDatEvnt uDatEvnt;             /* Connectionless data */

   TRC2(szMgmtLiRstAck);

   SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));

   uDatEvnt.transId.pres = NOTPRSNT;
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val = peerCb->peerId;
   uDatEvnt.pdu = pdu;

   /* Change the state of the node to normal */
   peerCb->state     = SZT_ND_ST_NORMAL;

   /* Send the Reset Ack to the user */
   ret = szUiSndUDatInd(peerCb, &uDatEvnt);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ret != ROK)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ071, (ErrVal)peerCb->peerId,
                 "szMgmtLiRstAck: Failed to send Reset Ack to the user");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   /* Remove the temporarily stored reset info from peerCb */
   SZ_FREE_SDU(peerCb->resetPdu); 

   RETVALUE(ret);

} /* szMgmtLiRstAck */

/*
 *
 *       Fun:   szMgmtUiErrInd - Handler for Error Indication
 *
 *       Desc:  This function handles Error Indication received from the user.
 *       Desc:  This function handles Error scenarios upon receiving the 
 *              message from the User.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtUiErrInd
(
SzPeerCb    *peerCb,                  /* Peer control block */
S1apPdu     *pdu,                     /* S1AP PDU */
SzSztSapCb  *uSapCb,                  /* SZT Sap control block */
U8          *cause                    /* Error cause */
)
#else /* ANSI */
PUBLIC S16 szMgmtUiErrInd(peerCb, pdu, uSapCb, cause)
SzPeerCb    *peerCb;                 /* Peer control block */
S1apPdu     *pdu;                    /* S1AP PDU */
SzSztSapCb  *uSapCb;                  /* SZT Sap control block */
U8          *cause;                  /* Error cause */
#endif /* ANSI */
{
   S16        ret = ROK;             /* return value */

   TRC2(szMgmtUiErrInd);

   /* Send encoded data to the peer */
   if ((ret = szLiSndToLower(peerCb, NULLP, pdu)) != ROK)
   {
      if((ret == SZT_CAUSE_PC_TX_SYNTAX_ERR) || 
            (ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR))
      {
         *cause = (U8) ret; 
      }
      else
      {
         *cause = SZT_CAUSE_MSG_NOT_DELIVERED;
      }
   }

   RETVALUE(ret);

} /* szMgmtUiErrInd */

/*
 *
 *       Fun:   szMgmtLiMsgHandler - Common handler for connection-less message 
 *
 *       Desc:  This function does common handling of the connection-less
 *              message received from the peer 
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PUBLIC S16 szMgmtLiMsgHandler
(
SzPeerCb   *peerCb,                  /* Peer control block */
S1apPdu    *pdu                      /* S1AP PDU */
)
#else /* ANSI */
PUBLIC S16 szMgmtLiMsgHandler(peerCb, pdu)
SzPeerCb   *peerCb;                  /* Peer control block */
S1apPdu    *pdu;                     /* S1AP PDU */
#endif /* ANSI */
{
   S16         ret = ROK;            /* Return value */ 
   SztUDatEvnt uDatEvnt;             /* Connectionless data */

   TRC2(szMgmtLiMsgHandler);

   /* Check the state of the node */
   if (peerCb->state != SZT_ND_ST_NORMAL)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ072, (ErrVal)peerCb->peerId,
                 "szMgmtLiMsgHandler : Node is not in valid state \
                  to receive message");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
      RETVALUE(RFAILED);
   }

   /* State will remain same - NORMAL */

   SZ_ZERO(&uDatEvnt, sizeof(SztUDatEvnt));

   uDatEvnt.transId.pres = NOTPRSNT; 
   uDatEvnt.peerId.pres = PRSNT_NODEF;
   uDatEvnt.peerId.val   = peerCb->peerId;
   uDatEvnt.pdu          = pdu;

   /* Send data to the user */
   ret = szUiSndUDatInd(peerCb, &uDatEvnt);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
      SZLOGERROR(ERRCLS_DEBUG, ESZ073, (ErrVal)peerCb->peerId,
                 "szMgmtLiMsgHandler : Failed to send data to the user");
#endif /* (ERRCLASS & ERRCLS_DEBUG) */
  

   RETVALUE(ret);

} /* szMgmtLiMsgHandler */

/*
 *
 *       Fun:   szMgmtChkConnLst - Compare S1 connection list
 *
 *       Desc:  This function compares S1 connection list with the temporarily 
 *              stored connection list in peerCb.
 *
 *       Ret:   ROK     - TRUE 
 *              RFAILED - FALSE 
 *
 *       Notes: None
 *              
 *       File:  sz_mgmt.c
 *
 */
#ifdef ANSI
PRIVATE Bool szMgmtChkConnLst 
(
SztResetTyp *tknIE,                  /* Connection list IE */
S1apPdu     *pdu,                    /* Reset Pdu */
U8          *cause
)
#else /* ANSI */
PRIVATE Bool szMgmtChkConnLst(tknIE, pdu, cause)
SztResetTyp *tknIE;                  /* Connection list IE */
S1apPdu     *pdu;                    /* Reset Pdu */
U8          *cause;
#endif /* ANSI */
{
   U8          i;                     /* Index variable for the outer loop */
   U8          j;                     /* Index variable for the inner loop */
   U16         nmbConns = 0;          /* Number of connections in Reset */
   SzConRefNo  conRefNo = 0;          /* Connection Id received in the Reset */ 
   TknU8       *tmpReset = NULLP;     /* Reset type IE stored in Peer */
   U16         tmpConns = 0;          /* Number of connections in Reset stored in Peer */
   SzConRefNo  tmpCon;                /* Connection Id stored in Peer */
   Bool        isEqual = FALSE;       /* isEqual flag */
   SztUE_assocLogS1_ConItem *conItem, *tmpConItem; /* conItem pointers */

   TRC2(szMgmtChkConnLst);

   /* sz008.301: fix for kwork */
   *cause = LCM_CAUSE_UNKNOWN;
   /* Get S1 connection list from DB for the reset pdu stored in peerCb */
   szGetIE(&pdu->pdu, Sztid_ResetTyp, &tmpReset);
   /* sz008.301: fix for Kwork */
   if((tknIE == NULLP) || (tmpReset == NULLP))
   {
      *cause = SZT_CAUSE_MSING_MNDTRY_PARM;
      RETVALUE(FALSE);
   }

   /* Check if we received full reset before and the current one is 
      partial/full reset */
   if (((SztResetTyp *)tmpReset)->choice.val == SZ_FULL_RESET) 
   {
      if ((tknIE->choice.val == SZ_PARTIAL_RESET) ||
          (tknIE->choice.val == SZ_FULL_RESET))
         RETVALUE(TRUE); 
   }

   /* Number of connections */
   tmpConns = ((SztResetTyp *)tmpReset)->val.partOfS1_Intf.noComp.val; 
   nmbConns = tknIE->val.partOfS1_Intf.noComp.val;

   /* Compare the connections received in Reset 
      with the connections stored in Peer */
   for (i = 0; i < nmbConns; i++)
   {
      conItem = &(tknIE->val.partOfS1_Intf.member[i].value.u.sztUE_assocLogS1_ConItem);

      if (conItem->pres.pres != NOTPRSNT)
      {
         if (conItem->mME_UE_S1AP_ID.pres != NOTPRSNT)
         {
            conRefNo = conItem->mME_UE_S1AP_ID.val;
         }
         else if (conItem->eNB_UE_S1AP_ID.pres != NOTPRSNT)
         {
            conRefNo = conItem->eNB_UE_S1AP_ID.val;
         }
         else
         {
            RETVALUE(TRUE); /* Empty list */
         }
      }
      else
      {
         /* Connection block is empty */
         SZDBGP(DBGMASK_LYR, (szCb.init.prntBuf,
                "szMgmtChkConnLst: S1 Connection is empty \n"));
      }

      isEqual = FALSE;
      for (j = 0; j < tmpConns; j++)
      {
        tmpConItem = &(((SztResetTyp *)tmpReset)->val.partOfS1_Intf.member[j].value.u.\
                  sztUE_assocLogS1_ConItem);

        if (tmpConItem->pres.pres != NOTPRSNT) 
        {
            if (tmpConItem->mME_UE_S1AP_ID.pres != NOTPRSNT)
            {
               tmpCon = tmpConItem->mME_UE_S1AP_ID.val;
            }
            else
            {
               tmpCon = tmpConItem->eNB_UE_S1AP_ID.val;
            }

            if (conRefNo == tmpCon) 
            {
               isEqual = TRUE;
               break;
            }
        }
      }
      if (!isEqual)
      {
         RETVALUE(isEqual);
      }
   }

   RETVALUE(isEqual);

} /* szMgmtChkConnLst */

/**********************************************************************

         End of file:     sq_mgmt.c@@/main/nodal_main/6 - Fri Nov 22 18:33:24 2013

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/5      ---       va      1. Updated for S1AP release 4.1
*********************************************************************91*/
