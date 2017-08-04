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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for Upper Interface procedures

     File:     sz_ui.c

     Sid:      sq_ui.c@@/main/nodal_main/3 - Fri Nov 22 18:01:36 2013

     Prg:      rs

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
#include "cm_pasn.h"
#ifdef SZ_FTHA
#include "cm_psfft.h"
#include "cm_ftha.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#endif /* SZ_FTHA */

#include "sct.h"           /* SCTP Layer */
#include "szt.h"           /* S1AP Upper Interface */
#include "szt_asn.h"
#ifdef SZ_FTHA
#include "sht.h"           /* System Agent */
#include "lyt.h"
#endif /* SZ_FTHA */

#include "lsz.h"           /* S1AP LM Interface */
#ifdef YT
#include "yt.h"            /* S1AP-PSF Layer */
#endif /* YT */
#include "sz.h"            /* S1AP Layer */
#include "sz_err.h"
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
#include "cm_pasn.x"
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_pftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#endif /* SZ_FTHA */

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
#include "sht.x"           /* System Agent */
#include "lyt.x"           /* PSF Layer Manager */
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP LM Interface */
#ifdef YT
#include "yt.x"
#endif /* YT */
#include "sz.x"            /* S1AP Layer */
#ifdef SS_DIAG
/* sz008.301 : Change for Diagnostics */
#include "ss_diag.h"        /* Include for the log file */
#endif

PRIVATE S16 szUiGetAndChkSztSap ARGS ((SpId spId, SzSztSapCb **uSapCb));
PRIVATE S16 szUiDoAudit ARGS ((SztAudEvnt *audit, 
                               SzSztSapCb *sztSapCb, 
                               U16 *cause));
#ifdef SZ_MME
PRIVATE S16 szValidatePeerAssocRsp ARGS ((SzPeerCb *peerCb, 
                                          SztPeerAssocRsp *peerAssocRsp, 
                                          U8* reason));
#endif /* SZ_MME */

PUBLIC S16 szHdlEncReq ARGS ((Mem *mem,
                                SzSztSapCb *uSapCb,
                                SztEncDecEvnt *tempEncDecEvnt, 
                                SztEncDecEvnt *encDecEvnt));

PUBLIC S16 szHdlDecReq ARGS ((Mem *mem,
                                SzSztSapCb *uSapCb,
                                SztEncDecEvnt *tempEncDecEvnt, 
                                SztEncDecEvnt *encDecEvnt));
PUBLIC S16 szFreeEncDecEvnt ARGS ((SztEncDecEvnt *encDecEvnt));
PUBLIC S16 szValDatReq ARGS ((SpId spId, SztDatEvntReq *datEvnt, SzSztSapCb **uSapCb,
                              SzConCb **conCb, U8 *evnt));
PUBLIC S16 szValRelReq ARGS ((SpId spId, SztRelReq *relReq, SzSztSapCb **uSapCb,
                              SzConCb **conCb));
PUBLIC S16 szErrIndUDatReq ARGS ((U8 cause, SztUDatEvnt *uDatEvnt, SpId spId,             /* service provider id */
                                  SzSztSapCb *uSapCb));
PUBLIC S16 szValConReq ARGS ((SpId spId, SzSztSapCb **uSapCb, SztConReq *conReq,
                              SzPeerCb **peer, U8 *event));
/*
 *
 *       Fun:   SzUiSztBndReq
 *
 *       Desc:  This function binds a User to the S1AP upper interface.
 *              S1AP layer software registers this new User and records 
 *              the identity of the service user.
 *              It also issues a SzUiSztBndCfm to the service user after
 *              completing a successful bind. 
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: None
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztBndReq
(
Pst *pst,             /* Post structure */
SuId suId,            /* service user id */
SpId spId             /* service provider id */
)
#else /* ANSI */
PUBLIC S16 SzUiSztBndReq(pst, suId, spId)
Pst *pst;             /* Post Structure */
SuId suId;            /* service user id */
SpId spId;            /* service provider id */
#endif /* ANSI */
{
   S16        ret;      /* return value */
   SzUstaDgn  dgn;      /* unsolicited status diagnostics structure */     
   SzSztSapCb   *uSapCb;  /* pointer to current SAP */
#ifdef SZ_RUG
   U8         found = FALSE; /* Valid interface version found */
   U16        idx;           /* Index variable */
#endif /* SZ_RUG */

 
   TRC3(SzUiSztBndReq);
#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (ret != ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ173,0,
                "SzUiSztBndReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#endif /*SS_MULTIPLE_PROCS */

#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
          "SzUiSztBndReq(pst, suId(%d), spId(%d))\n", suId, spId));
#endif /* DEBUGP */
   uSapCb = NULLP;

   /* Validation of input parameters */
#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */ 
  SZ_DIAG_LVL0(0x03160001, SZ_DIAG_NA, SS_DIAG_INV_ARG,
               "Received BndReq from Application, spId = %d, suId  = %d",
                spId, suId, 0, 0);
#endif

   ret = RFAILED;

   if ((spId < szCb.genCfg.nmbSztSaps) && (spId >= 0))
   { 
      uSapCb = szCb.sztSapLst[spId]; 
      if (uSapCb != NULLP)
      {
         ret = ROK;
      }
   }

   if (ret == RFAILED)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR_INTFC(ESZ174, (ErrVal) spId, "SzUiSztBndReq: invalid spId");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      dgn.type    = LSZ_USTA_DGNVAL_SAPID;
      dgn.u.sapId = spId;

      /* Send Alarm to the layer manager */
      szSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
            LCM_CAUSE_INV_SPID, &dgn);

      RETVALUE(RFAILED);
   }

   /* Update thee interface version which is configured for layer */
#ifdef SZ_RUG
   /* Check if the version is valid and already configured */
   if (uSapCb->remIntfValid == FALSE)
   {
      for (idx = 0; idx < szCb.nmbIntfInfo; idx++)
      {
         if (szCb.intfInfo[idx].intf.intfId == SZTIF)
         {
            switch (szCb.intfInfo[idx].grpType)
            {
               case SHT_GRPTYPE_ALL:

                  if (szCb.intfInfo[idx].dstProcId == pst->srcProcId &&
                      szCb.intfInfo[idx].dstEnt.ent == pst->srcEnt &&
                      szCb.intfInfo[idx].dstEnt.inst == pst->srcInst)
                  {
                     found = TRUE;
                  }
                  break;

               case SHT_GRPTYPE_ENT:

                  if (szCb.intfInfo[idx].dstEnt.ent == pst->srcEnt &&
                      szCb.intfInfo[idx].dstEnt.inst == pst->srcInst)
                  {
                     found = TRUE;
                  }
                  break;
               default:
                  break;
            }
         }

         /* Break from the loop if matching version is found */
         if (found == TRUE)
         {
            break;
         }
      }

      /* Update the version info if it is found. Else return failure */
      if (found == TRUE)
      {
         uSapCb->pst.intfVer = szCb.intfInfo[idx].intf.intfVer;
         uSapCb->remIntfValid = TRUE;
      }
      else
      {
#if (ERRCLASS & ERRCLS_INT_PAR)
         SZLOGERROR_INTFC(ESZ175, (ErrVal) spId,
               "SzUiSztBndReq: Version info not avilable for SAP");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
         dgn.type    = LSZ_USTA_DGNVAL_SAPID;
         dgn.u.sapId = spId;

         /* Send Alarm to the layer manager */
         szSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
               LCM_CAUSE_SWVER_NAVAIL, &dgn);

         RETVALUE(RFAILED);
      }
   }
#endif /* SZ_RUG */

   /* Check if the SAP is already bound */
   if (uSapCb->state != LSZ_SAP_ISBOUND)
   {
      /* copy bind configuration parameters in sap */
      uSapCb->suId          = suId;
      uSapCb->pst.dstProcId = pst->srcProcId;
      uSapCb->pst.dstEnt    = pst->srcEnt;
      uSapCb->pst.dstInst   = pst->srcInst;
 
      /* state bound and enabled */
      uSapCb->state = LSZ_SAP_ISBOUND;

   }

   /* if the sap is already bound, a Bind confirm is given to
      the user and no state change is made */


   /* send an acknowledgement back to the user */
   (Void) SzUiSztBndCfm(&uSapCb->pst, suId, CM_BND_OK);

   /* Send the runtime update message to update the sap state */
#ifdef YT
   YT_RTUPD_MOD(YT_UPD_SZT_SAP_CB, uSapCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);
} /* end of SzUiSztBndReq */


/*
 *
 *       Fun:   SzUiSztUbndReq    
 *
 *       Desc:  This function unbinds the user SAP. The service access point
 *              is not de-allocated, so a new bind bind request can be issued
 *              on this SAP (without reconfiguring the SAP in the service
 *              provider).
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztUbndReq 
(
Pst *pst,             /* Post structure */
SpId spId,             /* service provider id */
Reason reason          /* Reason for unbinding */
)
#else /* ANSI */
PUBLIC S16 SzUiSztUbndReq(pst, spId, reason)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
Reason reason;         /* Reason for unbinding */
#endif /* ANSI */
{
   S16         ret;     /* return value */
   SzSztSapCb   *uSapCb;  /* pointer to Upper SAP */
 
   TRC3(SzUiSztUbndReq);
#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret != ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ176,0,
                "SzUiSztUbndReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif
#else /* SS_MULTIPROCS */
   UNUSED(pst);
#endif /*SS_MULTIPLE_PROCS */

#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03160002, SZ_DIAG_NA, SS_DIAG_INV_ARG,
               "Received UbndReq from Application, spId = %d", spId, 0, 0, 0);
#endif

   UNUSED(reason);
 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztUbndReq(pst, spId(%d), reason(%d))\n", spId, reason));
#endif  /* DEBUGP */
   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztUbndReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      RETVALUE(RFAILED);
   }
   
   /* release all the connections associated with the SAP */
   ret = szUbndSztSap (uSapCb->spId, SZ_UBND_SAP);

   /* Send the queued update message to peer */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);
} /* end of SzUiSztUbndReq */


/*
 *
 *       Fun:   SzUiSztUDatReq    
 *
 *       Desc:  This function processes the connectionless signalling request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztUDatReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztUDatEvnt *uDatEvnt  /* connectionless sdus */
)
#else /* ANSI */
PUBLIC S16 SzUiSztUDatReq(pst, spId, uDatEvnt)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztUDatEvnt *uDatEvnt; /* connectionless sdus */
#endif /* ANSI */
{
   U8          evnt;          /* Event type */
   S16         ret = RFAILED; /* return value */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzPeerCb    *peer;
   U8          cause = SZT_CAUSE_UNEXP_MSG; /* Error cause returned from Mgmt module */

   TRC3(SzUiSztUDatReq);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK|| (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ177,0,
                "SzUiSztUDatReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztUDatReq(pst, spId(%d))\n", spId));
#endif  /* DEBUGP */

   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztUDatReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      SZ_FREE_SDU(uDatEvnt->pdu);
      RETVALUE(RFAILED);
   }

   if(uDatEvnt == NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, \
              "SzUiSztUDatReq: Data received is null\n"));  
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
      RETVALUE(RFAILED);
   }

   ret = szNdbChkPeer(uDatEvnt->peerId,&peer); 
   if(ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val, 
                          uSapCb, SZT_CAUSE_INV_PEERID, uDatEvnt->peerId.val);
      SZ_FREE_SDU(uDatEvnt->pdu);
      RETVALUE(ret);
   }
 
   ret = RFAILED;
   
   szUtilsGetMsgIdx (&evnt, uDatEvnt->pdu);

#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */ 
  SZ_DIAG_LVL0(0x03160003, SZ_DIAG_PEERID, uDatEvnt->peerId.val,
               "Received UDatReq from Application, Event = %d", evnt, 0, 0, 0);
#endif

#ifdef SZ_MME
   if(peer->nodeType == LSZ_NODE_ENB)
   {
      ret = szMgmtUiMme[evnt](peer, uDatEvnt->pdu, uSapCb, &cause);
   } 
#endif /* SZ_MME */


   /* Send ErrInd if the Mgmt module returns failure with errCause */
   if (ret != ROK)
   {
      if ((szErrIndUDatReq(cause, uDatEvnt, spId, uSapCb)) != ROK)
      {
         RETVALUE(RFAILED);
      }
   }

   /* Send the queued update message to peer */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzUiSztUDatReq */

/*
 *
 *       Fun:   szErrIndUDatReq    
 *
 *       Desc:  This function processes the error indication for
 *              the UDat request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szErrIndUDatReq 
(
U8          cause,
SztUDatEvnt *uDatEvnt, /* connectionless sdus */
SpId        spId,             /* service provider id */
SzSztSapCb  *uSapCb       /* Upper Sap */
)
#else /* ANSI */
PUBLIC S16 szErrIndUDatReq(cause, uDatEvnt, spId, uSapCb)
U8          cause;
SztUDatEvnt *uDatEvnt; /* connectionless sdus */
SpId        spId;             /* service provider id */
SzSztSapCb  *uSapCb;       /* Upper Sap */
#endif /* ANSI */
{

   TRC3(szErrIndUDatReq);

   switch (cause)
      {
         case SZT_CAUSE_INV_STATE:
         {
#ifdef DEBUGP
            SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, \
                  "SzUiSztUDatReq : Node is not in valid state \
                  to receive message from user\n"));
#endif /* DEBUGP */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val, 
                                uSapCb, SZT_CAUSE_INV_STATE, spId);
            SZ_FREE_SDU(uDatEvnt->pdu);
            break;
         }
         case SZT_CAUSE_MSG_NOT_DELIVERED:
         {
#ifdef DEBUGP
            SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, \
                    "SzUiSztUDatReq :Failed to encode and \
                     send data to the peer\n"));  
#endif /* DEBUGP */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val, 
                                uSapCb, SZT_CAUSE_MSG_NOT_DELIVERED,
                                uDatEvnt->peerId.val);
            break;
         }

         case SZT_CAUSE_PC_TX_SYNTAX_ERR:
         {
#ifdef DEBUGP
            SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, \
                "SzUiSztUDatReq :Failed to encode :transfer syntax error\n"));  
#endif /* DEBUGP */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val, 
                                uSapCb, SZT_CAUSE_PC_TX_SYNTAX_ERR,
                                uDatEvnt->peerId.val);
            break;
         }

         case SZT_CAUSE_PC_ABS_SYNTAX_ERR:
         {
#ifdef DEBUGP
            SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, \
                "SzUiSztUDatReq :Failed to encode :abstract syntax error\n"));  
#endif /* DEBUGP */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val, 
                                uSapCb, SZT_CAUSE_PC_ABS_SYNTAX_ERR,
                                uDatEvnt->peerId.val);
            break;
         }

         case SZT_CAUSE_UNEXP_MSG:
         {
            /* Send error indication to user */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val,
                                uSapCb, SZT_CAUSE_UNEXP_MSG, 0);
            SZ_FREE_SDU(uDatEvnt->pdu);
            RETVALUE(RFAILED);
            break;
         }

         /* sz008.301: Fix for kwork. */
         case SZT_CAUSE_MSING_MNDTRY_PARM:
         {
            /* Send error indication to user */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val,
                  uSapCb, SZT_CAUSE_MSING_MNDTRY_PARM, 0);
            SZ_FREE_SDU(uDatEvnt->pdu);
            RETVALUE(RFAILED);
         }
         
         case SZT_CAUSE_MEM_ALLOC_FAIL:
         {
            /* Send error indication to user */
            SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, uDatEvnt->transId.val,
                  uSapCb, SZT_CAUSE_MEM_ALLOC_FAIL, 0);
            SZ_FREE_SDU(uDatEvnt->pdu);
            RETVALUE(RFAILED);
         }

         default:
         break;
      } 
   RETVALUE(ROK);
}

/*
 *
 *       Fun:   SzUiSztConReq    
 *
 *       Desc:  This function processes the signalling request to initiate 
 *              the new connection
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztConReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztConReq *conReq      /*  connect sdus */
)
#else /* ANSI */
PUBLIC S16 SzUiSztConReq(pst, spId, conReq)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztConReq *conReq;     /*  connect sdus */
#endif /* ANSI */
{
   SzConCb     *conCb;        /* Connection control block */
   S16         ret;           /* return value */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzPeerCb    *peer;
   U8          evnt;          /* Event type */
   /* sz008.301: Adding new variables, enbID and mmes1apID */
#ifdef SZ_MME
   TknU8       *mmeS1apId = NULLP;
#endif

   TRC3(SzUiSztConReq);

   conCb = NULLP;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ178,0,
                "SzUiSztConReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztConReq(pst, spId(%d))\n", spId));
#endif  /* DEBUGP */

   if ((szValConReq(spId, &uSapCb, conReq, &peer, &evnt)) != ROK)
   {
      RETVALUE(RFAILED);
   }


   if ((U32)szCb.genSts.nmbConn >= szCb.genCfg.nmbConn)
   {
      /* Reject the request: maximum supported connections reached */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId, 
                          uSapCb, SZT_CAUSE_MEM_ALLOC_FAIL, 0);
      SZ_FREE_SDU(conReq->pdu);

#ifdef DEBUGP
#ifndef ALIGN_64BIT
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, "SzUiSztConReq:\
             nmbConn (%ld) exceeds the configured maximum,\
             discarding the ConReq ", szCb.genSts.nmbConn));
#else /* ALIGN_64BIT */
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf, "SzUiSztConReq:\
             nmbConn (%d) exceeds the configured maximum,\
             discarding the ConReq ", szCb.genSts.nmbConn));
   
#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */ 
  SZ_DIAG_LVL0(0x03160004, SZ_DIAG_PEERID, conReq->peerId.val,
             "Received ConReq from Application, Event = %d, suConnId = %ld",
                evnt, conReq->u.suConnId, 0, 0);
#endif
#endif /* ALIGN_64BIT */
#endif /* DEBUGP */
      RETVALUE (RFAILED);
   }

   /* Search for duplicate SuConnId */
   ret = cmHashListFind(&uSapCb->suConnLst, (U8 *)&conReq->u.suConnId, 
                        sizeof (UConnId), 0, (PTR *)&conCb);
   
   if(ret == ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId,
                          uSapCb, SZT_CAUSE_INV_CONID, conReq->u.suConnId);
      /* For local release, pdu is not filled */
      SZ_FREE_SDU(conReq->pdu);
      RETVALUE(ret);
   }

      /* sz008.301 : fix for kowrk. */
#ifdef SZ_MME
   if (peer->nodeType == LSZ_NODE_ENB)
   {
     szGetIE(&(conReq->pdu->pdu), Sztid_MME_UE_S1AP_ID, &mmeS1apId);
     if(mmeS1apId == NULLP)
      {
         /* Error Indication to the user */
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId,
              uSapCb, SZT_CAUSE_MSING_MNDTRY_PARM, SZT_CAUSE_NOT_APPL);
         SZ_FREE_SDU(conReq->pdu);
         RETVALUE(RFAILED);
      }
   }

#endif


   if ((ret = szNdbAllocConCb(&conCb, peer)) != ROK)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId,
                          uSapCb, SZT_CAUSE_MEM_ALLOC_FAIL, 0);
      SZ_FREE_SDU(conReq->pdu);
      RETVALUE(ret);
   }

   conCb->cd.suConnId   = conReq->u.suConnId;
   conCb->conType   = SZ_OUTGNG_CONN;

   /* 
    * insert the connection control block SAP Hash List
    */ 
   ret = cmHashListInsert (&uSapCb->suConnLst, (PTR)conCb,
                           (U8 *)&conCb->cd.suConnId, sizeof (UConnId)); 
   if (ret != ROK)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId,
                          uSapCb, SZT_CAUSE_HASH_INS_FAIL, 0);
      SZ_FREE_SDU(conReq->pdu);
      RETVALUE(ret);
   }

   conCb->cd.peerCb     = peer;

   /* This function will always return ROK */
   szNdbGetStream(peer, &conCb->cd.streamNo);

   szSmExcMt(conCb, peer, conReq->pdu, evnt, SZ_UI);

   /* Send the queued update message to peer */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzUiSztConReq */


/*
 *
 *       Fun:   szValConReq    
 *
 *       Desc:  This function processes the checking and validating 
 *              the pointers value.
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szValConReq
(
SpId          spId,       /* service provider id */
SzSztSapCb    **uSapCb,   /* Upper Sap */
SztConReq     *conReq,    /*  connect sdus */
SzPeerCb      **peer,     /* Connection control block */
U8            *evnt
)
#else /* ANSI */
PUBLIC S16 szValConReq(spId, uSapCb, conReq, peer, evnt)
SpId          spId;       /* service provider id */
SzSztSapCb    **uSapCb;   /* Upper Sap */
SztConReq     *conReq;    /*  connect sdus */
SzPeerCb      **peer;     /* Connection control block */
U8            *evnt;
#endif /* ANSI */
{
   S16         ret;       /* return value */
   SzSztSapCb  *tmpuSapCb;/* Upper Sap */
   SzPeerCb    *tmpPeer;  /* Connection control block */

   TRC3(szValConReq);

   if(szUiGetAndChkSztSap(spId, uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztConReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      if (conReq != NULLP)
      {
         SZ_FREE_SDU(conReq->pdu);
      }
      RETVALUE(RFAILED);
   }

   tmpuSapCb = *uSapCb;

   if(conReq == NULLP)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(NOTPRSNT, 0, tmpuSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      RETVALUE(RFAILED);
   }
   
   ret = szNdbChkPeer(conReq->peerId, peer);
   if(ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId, tmpuSapCb,
                          SZT_CAUSE_INV_PEERID, conReq->peerId.val);
      SZ_FREE_SDU(conReq->pdu);

      RETVALUE(RFAILED);
   }
   
   tmpPeer = *peer;
   if(tmpPeer->state == SZT_ND_ST_RESET)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId, tmpuSapCb, 
                          SZT_CAUSE_NODE_RESET, tmpPeer->state);
      SZ_FREE_SDU(conReq->pdu);

      RETVALUE(RFAILED);
   }

   /* Validate the if the bypass of UE state machine is enabled for peer */
#ifdef LSZV1
   if(tmpPeer->skipUeSm)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId, tmpuSapCb,
                          SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(conReq->pdu);

      RETVALUE(RFAILED);
   }
#endif /* LSZV1 */
   
   ret = szValidateSdus(conReq->pdu, SZ_UI_CON_REQ, evnt);

   if (ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conReq->u.suConnId,
                          tmpuSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(conReq->pdu);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* end of szValConReq */ 



/*
 *
 *       Fun:   SzUiSztConRsp    
 *
 *       Desc:  This function processes the connectionless signalling response
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztConRsp 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztConRsp *conRsp      /*  connect sdus */
)
#else /* ANSI */
PUBLIC S16 SzUiSztConRsp(pst, spId, conRsp)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztConRsp *conRsp;     /*  connect sdus */
#endif /* ANSI */
{
   SzConCb    *conCb;        /* Connection control block */
   S16         ret;           /* return value */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzPeerCb    *peer;
   U8          evnt;          /* Event type */
   /* sz008.301:  Declaring new variables */
   TknU8       *eNbId = NULLP;
   TknU8       *mmeS1apId = NULLP;

   TRC3(SzUiSztConRsp);

   conCb = NULLP;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ179,0,
                "SzUiSztConRsp() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztConRsp(pst, spId(%d))\n", spId));
#endif /* DEBUGP */
 
   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztConRsp: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      if (conRsp != NULLP)
      {
         SZ_FREE_SDU(conRsp->pdu);
      }
      RETVALUE(RFAILED);
   }

   if(conRsp == NULLP)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(NOTPRSNT, 0, uSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      RETVALUE(RFAILED);
   }

   ret = szValidateSdus(conRsp->pdu, SZ_UI_CON_RSP, &evnt);
#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */ 
  SZ_DIAG_LVL0(0x03160005, SZ_DIAG_SPCONID, conRsp->spConnId,
          "Received ConRsp from Application, Event = %d ConnId = %ld",
                evnt, conRsp->suConnId, 0, 0);
#endif

   if (ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conRsp->spConnId, 
                          uSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(conRsp->pdu);
      RETVALUE(RFAILED);
   }

   /* Search for duplicate SuConnId */
   ret = cmHashListFind(&uSapCb->suConnLst, (U8 *)&conRsp->suConnId, 
                        sizeof (UConnId), 0, (PTR *)&conCb);
   
   if(ret == ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conRsp->suConnId,
                          uSapCb, SZT_CAUSE_INV_CONID, conRsp->suConnId);
      SZ_FREE_SDU(conRsp->pdu);
      /* For local release, pdu is not filled */
      RETVALUE(ret);
   }

   /* Retrieve the control block for the connection endpoint that is being */
   /* established */
   if ((ret = szNdbGetConCb(conRsp->spConnId, &conCb)) != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conRsp->spConnId,
                          uSapCb, SZT_CAUSE_INV_CONID, conRsp->spConnId);
      SZ_FREE_SDU(conRsp->pdu);
      RETVALUE(ret);
   }

   /* sz008.301 : fix for kowrk */
   szGetIE(&(conRsp->pdu->pdu), Sztid_eNB_UE_S1AP_ID, &eNbId);
   szGetIE(&(conRsp->pdu->pdu), Sztid_MME_UE_S1AP_ID, &mmeS1apId);
   if((eNbId == NULLP) || (mmeS1apId == NULLP))
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conCb->cd.spConnId, 
           uSapCb, SZT_CAUSE_MSING_MNDTRY_PARM, SZT_CAUSE_NOT_APPL);
      SZ_FREE_SDU(conRsp->pdu);
      RETVALUE(RFAILED);
   }

   peer = conCb->cd.peerCb;

   if(peer->state == SZT_ND_ST_RESET)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conRsp->spConnId, uSapCb,
                          SZT_CAUSE_NODE_RESET, peer->state);
      SZ_FREE_SDU(conRsp->pdu);

      RETVALUE(RFAILED);
   }

   conCb->cd.suConnId = conRsp->suConnId; /* RCS */

   /* 
    * insert the connection control block SAP Hash List
    */ 
   ret = cmHashListInsert (&uSapCb->suConnLst, (PTR)conCb,
                        (U8 *)&conCb->cd.suConnId, sizeof (UConnId)); 
   if (ret != ROK)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, conRsp->suConnId,
                          uSapCb, SZT_CAUSE_HASH_INS_FAIL, 0);
      SZ_FREE_SDU(conRsp->pdu);
      RETVALUE(ret);
   }

   szSmExcMt(conCb, peer, conRsp->pdu, evnt, SZ_UI);
   
   /* Send the queued update message to peer */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzUiSztConRsp */


/*
 *
 *       Fun:   SzUiSztRelReq    
 *
 *       Desc:  This function processes the release request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztRelReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztRelReq *relReq      /*  connect sdus */
)
#else /* ANSI */
PUBLIC S16 SzUiSztRelReq(pst, spId, relReq)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztRelReq *relReq;     /*  connect sdus */
#endif /* ANSI */
{
   SzConCb     *conCb;        /* Connection control block */
   S16         ret;           /* return value */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzPeerCb    *peer;

   TRC3(SzUiSztRelReq);

   conCb = NULLP;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ180,0,
                "SzUiSztRelReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztRelReq(pst, spId(%d))\n", spId));
#endif /* DEBUGP */
 
   if ((ret = szValRelReq(spId, relReq, &uSapCb, &conCb)) != ROK)
   {
      RETVALUE(ret);
   }


   peer = conCb->cd.peerCb;

   if(peer->state == SZT_ND_ST_RESET)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, relReq->connId,
                          uSapCb, SZT_CAUSE_NODE_RESET, peer->state);
      SZ_FREE_SDU(relReq->pdu);
      RETVALUE(RFAILED);
   }

   if (relReq->relType == SZT_LOCAL_REL)
   {
   SztRelCfm  relCfm;

   SZ_ZERO(&relCfm, sizeof(SztRelCfm));

   relCfm.suConnId = conCb->cd.suConnId;
   relCfm.spConnId = conCb->cd.spConnId;
   relCfm.relType  = relReq->relType;
   relCfm.pdu      = NULLP; 

   ret = RFAILED;

   /* Send release confirm to User */
   ret = szUiSndRelCfm(conCb, &relCfm);
   if(ret == RFAILED)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
           SZLOGERROR(ERRCLS_INT_PAR, ESZ181, (ErrVal) conCb->lclConRefNo,
      "SzUiSztRelReq: failed to send release confirm to user");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
           RETVALUE(ret);
   }

   /* Deallocate connection control block */
   ret = szNdbDeallocConCb(conCb->lclConRefNo, conCb->cd.peerCb,
                           SZ_CONN_REF_LCL);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret == RFAILED)
   {
           SZLOGERROR(ERRCLS_INT_PAR, ESZ182, (ErrVal) conCb->lclConRefNo,
      "SzUiSztRelReq: failed to deallocate connection control block");
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   RETVALUE(ret);
   }
   else
   {
      U8    evnt;

      ret = szValidateSdus(relReq->pdu, SZ_UI_REL_REQ, &evnt);
      if (ret != ROK)
      {
           /* Error Indication to the user */
           SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, relReq->connId, 
            uSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(relReq->pdu);
           RETVALUE(RFAILED);
      }

      szSmExcMt(conCb, peer, relReq->pdu, evnt, SZ_UI);
   }

   RETVALUE(ROK);

} /* end of SzUiSztRelReq */


/*
 *
 *       Fun:   szValRelReq    
 *
 *       Desc:  This function processes the checking and validating 
 *              the pointers value.
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szValRelReq
(
SpId          spId,       /* service provider id */
SztRelReq     *relReq,     /*  connect sdus */
SzSztSapCb    **uSapCb,   /* Upper Sap */
SzConCb       **conCb    /* Connection control block */
)
#else /* ANSI */
PUBLIC S16 szValRelReq(spId, relReq, uSapCb, conCb)
SpId          spId;       /* service provider id */
SztRelReq     *relReq;     /*  connect sdus */
SzSztSapCb    **uSapCb;   /* Upper Sap */
SzConCb       **conCb;    /* Connection control block */
#endif /* ANSI */
{
   S16         ret;       /* return value */
   SzSztSapCb  *tmpuSapCb;/* Upper Sap */

   TRC3(szValRelReq);

   if(szUiGetAndChkSztSap(spId, uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztRelReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      if (relReq != NULLP)
      {
         SZ_FREE_SDU(relReq->pdu);
      }
      RETVALUE(RFAILED);
   }

   tmpuSapCb = *uSapCb;

   if(relReq == NULLP)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(NOTPRSNT, 0, tmpuSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      RETVALUE(RFAILED);
   }
#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03160006, SZ_DIAG_CONNID, relReq->connId,
               "Received RelReq from Application, ConnId Type = %d, Release Type = %d",
                relReq->connIdType, relReq->relType, 0, 0)
#endif

   /* Retrieve the control block for the connection endpoint that is being */
   /* established */
   if (relReq->connIdType == SZT_CONN_TYPE_SU)
   {
      ret = cmHashListFind(&tmpuSapCb->suConnLst, (U8 *)&relReq->connId, 
                           sizeof (UConnId), 0, (PTR *)conCb);
   }
   else
   {
      ret = szNdbGetConCb(relReq->connId, conCb);
   }

   if (ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, relReq->connId,
                       tmpuSapCb, SZT_CAUSE_INV_CONID, relReq->connId);
      SZ_FREE_SDU(relReq->pdu);
      /* For local release, pdu is not filled */
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* end of szUiGetAndChkSztSap */ 


/*
 *
 *       Fun:   SzUiSztRelRsp    
 *
 *       Desc:  This function processes the release request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztRelRsp 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztRelRsp *relRsp      /*  connect sdus */
)
#else /* ANSI */
PUBLIC S16 SzUiSztRelRsp(pst, spId, relRsp)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztRelRsp *relRsp;     /*  connect sdus */
#endif /* ANSI */
{
   SzConCb     *conCb;        /* Connection control block */
   S16         ret;           /* return value */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzPeerCb    *peer;
   U8          evnt;          /* Event type */

   TRC3(SzUiSztRelRsp);

   conCb = NULLP;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ183,0,
                "SzUiSztRelRsp() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztRelRsp(pst, spId(%d))\n", spId));
#endif  /* DEBUGP */
   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztRelRsp: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      if (relRsp != NULLP)
      {
         SZ_FREE_SDU(relRsp->pdu);
      }
      RETVALUE(RFAILED);
   }

   if(relRsp == NULLP)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(NOTPRSNT, 0, uSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      RETVALUE(RFAILED);
   }
   
   ret = szValidateSdus(relRsp->pdu, SZ_UI_REL_RSP, &evnt);
   if (ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, relRsp->spConnId, uSapCb, 
                          SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(relRsp->pdu);
      RETVALUE(RFAILED);
   }

   /* Retrieve the control block for the connection endpoint that is being */
   /* established */
   if ((ret = szNdbGetConCb(relRsp->spConnId, &conCb)) != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, relRsp->spConnId, 
                          uSapCb, SZT_CAUSE_INV_CONID, relRsp->spConnId);
      SZ_FREE_SDU(relRsp->pdu);
      RETVALUE(ret);
   }
 
   peer = conCb->cd.peerCb;

   if(peer->state == SZT_ND_ST_RESET)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, relRsp->spConnId, uSapCb,
                          SZT_CAUSE_NODE_RESET, peer->state);
      SZ_FREE_SDU(relRsp->pdu);

      RETVALUE(RFAILED);
   }

#ifdef SS_DIAG 
  /* sz008.301 : Change for Diagnostics */
  SZ_DIAG_LVL0(0x03160007, SZ_DIAG_SPCONID, relRsp->spConnId,
               "Received RelResp from Application, PeerId = %ld Event  = %d",
                peer->peerId, evnt, 0, 0)
#endif
   szSmExcMt(conCb, peer, relRsp->pdu, evnt, SZ_UI);

   /* Send the queued update message to peer */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);

} /* end of SzUiSztRelRsp */


/*
 *
 *       Fun:   SzUiSztDatReq    
 *
 *       Desc:  This function processes connection oriented data request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztDatReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztDatEvntReq *datEvnt    /*  cn st sdus */
)
#else /* ANSI */
PUBLIC S16 SzUiSztDatReq(pst, spId, datEvnt)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztDatEvntReq *datEvnt;   /*  cn st sdus */
#endif /* ANSI */
{
   SzConCb     *conCb;        /* Connection control block */
   S16         ret;           /* return value */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SzPeerCb    *peer;
   U8          evnt;          /* Event type */
   /* sz008.301: Removed the gurading of variable under SZ_ENB , SZ_MME */
   TknU8       *eNbId = NULLP;
   TknU8       *mmeS1apId = NULLP;

   TRC3(SzUiSztDatReq);

   conCb = NULLP;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ184,0,
                "SzUiSztDatReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */

#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztDatReq(pst, spId(%d))\n", spId));
#endif  /* DEBUGP */

   if ((ret = szValDatReq(spId, datEvnt, &uSapCb, &conCb, &evnt)) != ROK)
   {
      RETVALUE(ret);
   }

   peer = conCb->cd.peerCb;

   /* Validate the if the bypass of UE state machine is enabled for peer */
#ifdef LSZV1
   if(peer->skipUeSm)
   {
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, datEvnt->spConnId, uSapCb,
                          SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(datEvnt->pdu);

      RETVALUE(RFAILED);
   }
#endif /* LSZV1 */


#ifdef SZ_MME
   if(peer->nodeType == LSZ_NODE_ENB)
   {
      szGetIE(&(datEvnt->pdu->pdu), Sztid_MME_UE_S1AP_ID, &mmeS1apId);
      if(((SztMME_UE_S1AP_ID *)mmeS1apId)->val != conCb->lclConRefNo)
      {
         /* Error Indication to the user */
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, datEvnt->spConnId,
                          uSapCb, SZT_CAUSE_INV_CONID,
                          ((SztMME_UE_S1AP_ID *)mmeS1apId)->val);
         SZ_FREE_SDU(datEvnt->pdu);
         RETVALUE(ret);
      }
   }
#endif

   if(peer->state == SZT_ND_ST_RESET)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, datEvnt->spConnId, 
                          uSapCb, SZT_CAUSE_NODE_RESET, peer->state);
      SZ_FREE_SDU(datEvnt->pdu);

      RETVALUE(RFAILED);
   }

   szSmExcMt(conCb, peer, datEvnt->pdu, evnt, SZ_UI);

   RETVALUE(ROK);

} /* end of SzUiSztDatReq */


/*
 *
 *       Fun:   szValDatReq    
 *
 *       Desc:  This function processes the checking and validating 
 *              the pointers value.
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szValDatReq
(
SpId          spId,       /* service provider id */
SztDatEvntReq *datEvnt,   /*  cn st sdus */
SzSztSapCb    **uSapCb,   /* Upper Sap */
SzConCb       **conCb,    /* Connection control block */
U8            *evnt
)
#else /* ANSI */
PUBLIC S16 szValDatReq(spId, datEvnt, uSapCb, conCb, evnt)
SpId          spId;       /* service provider id */
SztDatEvntReq *datEvnt;   /*  cn st sdus */
SzSztSapCb    **uSapCb;   /* Upper Sap */
SzConCb       **conCb;    /* Connection control block */
U8            *evnt;
#endif /* ANSI */
{
   S16         ret;       /* return value */
   SzSztSapCb  *tmpuSapCb;/* Upper Sap */

   TRC3(szValDatReq);


   if(szUiGetAndChkSztSap(spId, uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztDatReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      if (datEvnt != NULLP)
      {
        SZ_FREE_SDU(datEvnt->pdu);
      }
      RETVALUE(RFAILED);
   }

   tmpuSapCb = *uSapCb;

   if ((datEvnt == NULLP) || (datEvnt->pdu == NULLP))
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(NOTPRSNT, 0, tmpuSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      RETVALUE(RFAILED);
   }
   
   ret = szValidateSdus(datEvnt->pdu, SZ_UI_DAT_REQ, evnt);

   if (ret != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, datEvnt->spConnId, 
                          tmpuSapCb, SZT_CAUSE_UNEXP_MSG, 0);
      SZ_FREE_SDU(datEvnt->pdu);
      RETVALUE(RFAILED);
   }

   /* Retrieve the control block for the connection endpoint that is being */
   /* established */
   if ((ret = szNdbGetConCb(datEvnt->spConnId, conCb)) != ROK)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, datEvnt->spConnId, tmpuSapCb, 
                          SZT_CAUSE_INV_CONID, datEvnt->spConnId);
      SZ_FREE_SDU(datEvnt->pdu);
      RETVALUE(ret);
   }

   RETVALUE(ROK);
} /* end of szValDatReq */ 

/*
 *
 *       Fun:   SzUiSztEncDecReq    
 *
 *       Desc:  This function processes the encode decode request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztEncDecReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztEncDecEvnt *encDecEvnt
)
#else /* ANSI */
PUBLIC S16 SzUiSztEncDecReq(pst, spId, encDecEvnt)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztEncDecEvnt *encDecEvnt;
#endif /* ANSI */
{
   SztReqStatus  status;
   Mem           mem;
   Buffer        *mBuf;         /* Buffer */
   SztEncDecEvnt tempEncDecEvnt;
   SzSztSapCb    *uSapCb;       /* Upper Sap */
   MsgLen        msgLen;
#ifdef SS_MULTIPLE_PROCS
   S16           ret;
#endif /* SS_MULTIPLE_PROCS */

   TRC3(SzUiSztEncDecReq);
   
   mBuf   = NULLP;
   msgLen = 0;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ185,0,
                "SzUiSztEncDecReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */

#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztEncDecReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */

      szFreeEncDecEvnt(encDecEvnt);

      RETVALUE(RFAILED);
   }
 
   if ((encDecEvnt == NULLP) || (encDecEvnt->type.pres != PRSNT_NODEF))
   {
      status.status = SZT_PRIM_NOK;
      status.cause = SZT_CAUSE_UNEXP_MSG;

      szFreeEncDecEvnt(encDecEvnt);

      SzUiSztEncDecCfm(&uSapCb->pst, uSapCb->suId, encDecEvnt, &status);
      RETVALUE(RFAILED);
   }

   mem.region = uSapCb->pst.region;
   mem.pool = uSapCb->pst.pool;

   tempEncDecEvnt.transId = encDecEvnt->transId;
   cmMemcpy ( (U8 *)&tempEncDecEvnt.type, (U8 *)&encDecEvnt->type,
            sizeof (TknU8));

#ifdef SZTV1
   cmMemcpy ( (U8 *)&tempEncDecEvnt.ieType, (U8 *)&encDecEvnt->ieType,
            sizeof (TknU8));
#endif  /* SZTV1 */ 

   if (encDecEvnt->type.val == SZT_REQ_TYPE_DECODE ||
         encDecEvnt->type.val == SZT_REQ_TYPE_DECODE_IE)
   {
      if (szHdlDecReq(&mem, uSapCb, &tempEncDecEvnt, encDecEvnt) == RFAILED)
      {
         RETVALUE(RFAILED);
      }
   }
   else if (encDecEvnt->type.val == SZT_REQ_TYPE_ENCODE ||
         encDecEvnt->type.val == SZT_REQ_TYPE_ENCODE_IE)
   {
      if (szHdlEncReq(&mem, uSapCb, &tempEncDecEvnt, encDecEvnt) == RFAILED)
      {
         RETVALUE(RFAILED);
      }
   }
   else
   {  
      status.status = SZT_PRIM_NOK;
      status.cause = SZT_CAUSE_INV_TYPE;

      SZ_FREE_SDU(encDecEvnt->u.pdu);

      SzUiSztEncDecCfm(&uSapCb->pst, uSapCb->suId, encDecEvnt, &status);

      RETVALUE(RFAILED);
   }
   
   status.status = SZT_PRIM_OK;
   status.cause = SZT_CAUSE_NOT_APPL;
   
   SzUiSztEncDecCfm(&uSapCb->pst, uSapCb->suId, &tempEncDecEvnt, &status);

   RETVALUE(ROK);

} /* end of SzUiSztEncDecReq */

/*
 *
 *       Fun:   szFreeEncDecEvnt    
 *
 *       Desc:  This function processes the checking that teh pointer is 
 *              present or not
 *
 *       Ret:   ROK     - success
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szFreeEncDecEvnt
(
SztEncDecEvnt *encDecEvnt
)
#else /* ANSI */
PUBLIC S16 szFreeEncDecEvnt(encDecEvnt)
SztEncDecEvnt *encDecEvnt;
#endif /* ANSI */
{
    TRC3(szFreeEncDecEvnt);

    if (encDecEvnt != NULLP)
      {    
         if (encDecEvnt->type.val == SZT_REQ_TYPE_DECODE ||
               encDecEvnt->type.val == SZT_REQ_TYPE_ENCODE)
         {
            SZ_FREE_SDU(encDecEvnt->u.pdu);
         }
#ifdef SZTV1
         else
         {
            SZ_FREE_SDU(encDecEvnt->u.iePdu);
         }
#endif  /* SZTV1 */ 
      }
    RETVALUE(ROK); 
} /* end of szFreeEncDecEvnt */

/*
 *
 *       Fun:   szHdlDecReq    
 *
 *       Desc:  This function processes the Decode request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szHdlDecReq 
(
Mem           *mem,
SzSztSapCb    *uSapCb,
SztEncDecEvnt *tempEncDecEvnt, 
SztEncDecEvnt *encDecEvnt
)
#else /* ANSI */
PUBLIC S16 szHdlDecReq(mem, uSapCb, tempEncDecEvnt, encDecEvnt)
Mem           *mem;
SzSztSapCb    *uSapCb;
SztEncDecEvnt *tempEncDecEvnt;
SztEncDecEvnt *encDecEvnt;
#endif /* ANSI */
{

   S16           ret = RFAILED;
   SztReqStatus  status;
   MsgLen        msgLen;
   CmPAsnErr     asnErr;

   TRC3(szHdlDecReq);

   msgLen = 0;

 if (encDecEvnt->u.mBuf != NULLP)
      {
         SFndLenMsg (encDecEvnt->u.mBuf, &msgLen);
      }

      if(encDecEvnt->u.mBuf == NULLP || msgLen == 0)
      {
         status.status = SZT_PRIM_NOK;
         status.cause = SZT_CAUSE_UNEXP_MSG;

         SzUiSztEncDecCfm(&uSapCb->pst, uSapCb->suId, encDecEvnt, &status);
         RETVALUE(RFAILED);
      }
      if (encDecEvnt->type.val == SZT_REQ_TYPE_DECODE)
      {
         ret = szDecode(mem, &asnErr, &(tempEncDecEvnt->u.pdu),
               &(encDecEvnt->u.mBuf));
         SZ_FREE_BUF(encDecEvnt->u.mBuf);
      }
#ifdef SZTV1
      else
      {
         ret = szIeDecode(mem, &asnErr, encDecEvnt);
         tempEncDecEvnt->u.iePdu = encDecEvnt->u.iePdu;
      }
#endif  /* SZTV1 */
      if (ret != ROK)
      {

         status.status = SZT_PRIM_NOK;
         if(ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR)
         {
            status.cause = SZT_CAUSE_PC_ABS_SYNTAX_ERR;
         }
         else
         {
            status.cause = SZT_CAUSE_PC_TX_SYNTAX_ERR;
         }

         SzUiSztEncDecCfm(&uSapCb->pst, uSapCb->suId, tempEncDecEvnt,
                                     &status);
         RETVALUE(RFAILED);
      }
   RETVALUE(ROK);
} /* end of szHdlDecReq */

/*
 *
 *       Fun:   szHdlEncReq    
 *
 *       Desc:  This function processes the Encode request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szHdlEncReq 
(
Mem           *mem,
SzSztSapCb    *uSapCb,
SztEncDecEvnt *tempEncDecEvnt, 
SztEncDecEvnt *encDecEvnt
)
#else /* ANSI */
PUBLIC S16 szHdlEncReq(mem, uSapCb, tempEncDecEvnt, encDecEvnt)
Mem           *mem;
SzSztSapCb    *uSapCb;
SztEncDecEvnt *tempEncDecEvnt;
SztEncDecEvnt *encDecEvnt;
#endif /* ANSI */
{

   S16           ret = RFAILED;
   SztReqStatus  status;
   MsgLen        msgLen;

   TRC3(szHdlEncReq);

   msgLen = 0;


     if (encDecEvnt->type.val == SZT_REQ_TYPE_ENCODE)
      {
         ret = szEncode(mem, &(encDecEvnt->u.pdu), &(tempEncDecEvnt->u.mBuf));
         SZ_FREE_SDU(encDecEvnt->u.pdu);
      }
#ifdef SZTV1
      else
      {
         ret = szIeEncode(mem, encDecEvnt);
         tempEncDecEvnt->u.mBuf = encDecEvnt->u.mBuf;
      }     
#endif   /* SZTV1 */

      if (ret != ROK)
      {
         status.status = SZT_PRIM_NOK;
         if(ret == SZT_CAUSE_PC_ABS_SYNTAX_ERR)
         {
            status.cause = SZT_CAUSE_PC_ABS_SYNTAX_ERR;
         }
#ifdef SZTV1
         else if (ret == SZT_CAUSE_UNEXP_MSG)
         {
            status.cause = SZT_CAUSE_UNEXP_MSG;
         }
#endif  /* SZTV1 */
         else
         {
            status.cause = SZT_CAUSE_PC_TX_SYNTAX_ERR;
         }

         SzUiSztEncDecCfm(&uSapCb->pst, uSapCb->suId, tempEncDecEvnt, &status);
         RETVALUE(RFAILED);
      }

   RETVALUE(ROK);
} /* end of szHdlEncReq */

/*
 *
 *       Fun:   SzUiSztAudReq    
 *
 *       Desc:  This function processes the Audit Request
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztAudReq 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztAudEvnt *audEvnt    /* Audit Event */
)
#else /* ANSI */
PUBLIC S16 SzUiSztAudReq(pst, spId, audEvnt)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztAudEvnt *audEvnt;   /* Audit Event */
#endif /* ANSI */
{
   SzConCb    *connCb;        /* Connection control block */
   SzSztSapCb  *uSapCb;       /* Upper Sap */
   SztAudEvnt  audCfm;        /* Audit Event */
   U16         cause;
#ifdef SS_MULTIPLE_PROCS
   S16         ret;          /* return value */
#endif /* SS_MULTIPLE_PROCS */

   TRC3(SzUiSztAudReq);

   connCb = NULLP;

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_DEBUG,ESZ186,0,
                "SzUiSztAudReq() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztAudReq(pst, spId(%d))\n", spId));
#endif /* DEBUGP */
 
   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztAudReq: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      RETVALUE(RFAILED);
   }

   if(audEvnt == NULLP)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztAudReq: No Audit Event Present \n"));
#endif
      RETVALUE(ROK);

   }

   cmMemcpy ((U8 *)&audCfm, (U8 *)audEvnt, sizeof (SztAudEvnt));

   szCb.genSts.audSts.nmbAudReq++;

   switch(audEvnt->type)
   {
      case SZT_AUD_CONN:
      case SZT_AUD_PEER:
      {
         if (szUiDoAudit(&audCfm, uSapCb, &cause) != ROK)
         {
            audCfm.status.status = SZT_PRIM_NOK;
            audCfm.status.cause  = cause;

            SzUiSztAudCfm(&uSapCb->pst, uSapCb->suId, &audCfm);

            RETVALUE(RFAILED);
         }
      }
      break;

      case SZT_AUD_FLC:
      {
         SzPeerCb    *peer;
         TknU32      peerId;

         peerId.pres = PRSNT_NODEF;
         peerId.val = audCfm.u.audPeer.peerId;

         if(szNdbChkPeer(peerId,&peer) != ROK)
         {
            audCfm.status.status = SZT_PRIM_NOK;
            audCfm.status.cause = SZT_CAUSE_INV_PEERID;

            SzUiSztAudCfm(&uSapCb->pst, uSapCb->suId, &audCfm);

            RETVALUE(RFAILED);
         }

         szCb.genSts.audSts.nmbAudFlc++;

         if(peer->sctSapCb == NULLP)
         {
            audCfm.status.status = SZT_PRIM_NOK;
            audCfm.status.cause = SZT_CAUSE_SAP_NOTPRST;

            SzUiSztAudCfm(&uSapCb->pst, uSapCb->suId, &audCfm);

            RETVALUE(RFAILED);
         }

         if(peer->assocCb == NULLP || 
            peer->assocCb->assocState != SZ_ASSOC_UP)
         {
            audCfm.status.status = SZT_PRIM_NOK;
            audCfm.status.cause = SZT_CAUSE_ASSOC_NOTPRST;

            SzUiSztAudCfm(&uSapCb->pst, uSapCb->suId, &audCfm);

            RETVALUE(RFAILED);
         }

         /* Send a status request to the sctp layer */
         SzLiSctStaReq(&peer->sctSapCb->pst, peer->sctSapCb->spId, 
                        peer->assocCb->spAssocId, 
                        &peer->assocCb->assocCfg->priDstAddr, 
                        SCT_GET_FLC_INFO);
      }
      break;

      default:
         audCfm.status.status = SZT_PRIM_NOK;
         audCfm.status.cause = SZT_CAUSE_INV_TYPE;

         SzUiSztAudCfm(&uSapCb->pst, uSapCb->suId, &audCfm);

         RETVALUE(RFAILED);
         break;

   }

   audCfm.status.status = SZT_PRIM_OK;
   audCfm.status.cause = SZT_CAUSE_NOT_APPL;

   SzUiSztAudCfm(&uSapCb->pst, uSapCb->suId, &audCfm);

   RETVALUE(ROK);

} /* end of SzUiSztAudReq */


#ifdef SZ_MME
/*
 *
 *       Fun:   SzUiSztPeerAssocRsp    
 *
 *       Desc:  This function processes the response received from the user
 *              for the configuration of dynamic peer.
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztPeerAssocRsp 
(
Pst *pst,              /* Post structure */
SpId spId,             /* service provider id */
SztPeerAssocRsp *peerAssocRsp
)
#else /* ANSI */
PUBLIC S16 SzUiSztPeerAssocRsp(pst, spId, peerAssocRsp)
Pst *pst;              /* Post structure */
SpId spId;             /* service provider id */
SztPeerAssocRsp *peerAssocRsp;
#endif /* ANSI */
{
   S16                 ret;          /* return value */
   U16                 idx;          /* return value */
   SzSztSapCb         *uSapCb;       /* Upper Sap */
   SzPeerCb           *peerCb;
   SctAssocIndParams   assocParams;
   SctNetAddrLst       sctAddrLst;
   SztStaInd           sta;
   U8                  reason;

   TRC3(SzUiSztPeerAssocRsp);

#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if(ret !=ROK || (szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ187,0,
                "SzUiSztPeerAssocRsp() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_INT_PAR */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

 
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
         "SzUiSztPeerAssocRsp(pst, spId(%d))\n", spId));
#endif /* DEBUGP */
 
   cmMemset((U8*)&sta, 0, sizeof(SztStaInd));

   if(szUiGetAndChkSztSap(spId, &uSapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztPeerAssocRsp: Invalid SZT sap\n"));
#endif  /* DEBUGP */
      RETVALUE(RFAILED);
   }

   if(peerAssocRsp == NULLP)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztPeerAssocRsp: peerAssocRsp received is null\n"));
#endif  /* DEBUGP */
      RETVALUE(RFAILED);
   }
   
   /* search for the peerAssoc control block */
   sctAddrLst.nmb = peerAssocRsp->dstAddrLst.nmb;
   for (idx = 0; idx < peerAssocRsp->dstAddrLst.nmb; idx++)
      sctAddrLst.nAddr[idx] = peerAssocRsp->dstAddrLst.nAddr[idx];

   ret = szNdbFindPeerAddrPortCb(&sctAddrLst, peerAssocRsp->dstPort, &peerCb);
   if ((ret != ROK) || (peerCb->isDynamCfgd == FALSE))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ188, (ErrVal) 0,
      "SzUiSztPeerAssocRsp: failed to find the cb for dyamic peer\n");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      if (peerAssocRsp->status.status == SZT_PRIM_OK)
      {
         /* Populate the sta indication structure */
         sta.peerId.pres = TRUE;
         sta.peerId.val  = peerAssocRsp->peerId;
         sta.type        = SZT_STA_TYPE_PEER;
          sta.status      = SZT_STATUS_DYNAM_PEERCFG_NOK;
          sta.reason      = SZT_REASON_NOT_APPL;

         /* Send status indication to user */
         SzUiSztStaInd(&uSapCb->pst, uSapCb->suId, &sta);
      }

      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)&assocParams,(U8 *)peerCb->assocParams,
            sizeof (SctAssocIndParams));

   if (peerAssocRsp->status.status != SZT_PRIM_OK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ189, (ErrVal) 0,
       "SzUiSztPeerAssocRsp: user responded not to cfg the dynamic peer\n");
#endif
#ifdef SCT3
      SzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                      peerCb->sctSapCb->endpCb.locSpEndpId,
                      &assocParams, 0, SCT_NOK, NULLP);
#else
      SzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                      peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams,
                      SCT_NOK, NULLP);
#endif /* end of SCT3 */
 
#ifdef YT
#ifdef SZ_MME
      YT_RTUPD_DEL(YT_UPD_DYNPEER_CB, peerCb);
      YT_DEL_MAPPING(YT_UPD_DYNPEER_CB, peerCb);
#endif /* SZ_MME */
#endif /* YT */
 
      szNdbRemPeerAddrPortCb(peerCb);
      szNdbDeallocDynamPeerCb(peerCb);

      RETVALUE(ROK);
   }

   ret = szValidatePeerAssocRsp(peerCb, peerAssocRsp, &reason);
   if (ret != ROK)
   {
#ifdef SCT3
      SzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                      peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams,
                      0, SCT_NOK, NULLP);
#else
      SzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                      peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams,
                      SCT_NOK, NULLP);
#endif /* end of SCT3 */

      szNdbRemPeerAddrPortCb(peerCb);
      szNdbDeallocDynamPeerCb(peerCb);

      /* Populate the sta indication structure */
      sta.peerId.pres = TRUE;
      sta.peerId.val  = peerAssocRsp->peerId;
      sta.type        = SZT_STA_TYPE_PEER;
      sta.status      = SZT_STATUS_DYNAM_PEERCFG_NOK;
      sta.reason      = reason;

      /* Send status indication to user */
      SzUiSztStaInd(&uSapCb->pst, uSapCb->suId, &sta);

      RETVALUE(RFAILED);
   }

   /* fill the remaining fields of peer and assoc cb */
   peerCb->peerId                = peerAssocRsp->peerId;
   peerCb->comStreamId           = peerAssocRsp->comProcStrm;

   peerCb->assocCb->suAssocId                  = peerAssocRsp->peerId;
   peerCb->assocCb->assocCfg->comProcStrm.pres = PRSNT_NODEF;
   peerCb->assocCb->assocCfg->comProcStrm.val  = peerAssocRsp->comProcStrm;
   peerCb->assocCb->assocCfg->priDstAddr       = peerAssocRsp->priDstAddr;

   /* The peer type will be always eNB as self node is MME */
   peerCb->nodeType = LSZ_NODE_ENB;

   if (assocParams.type == SCT_ASSOC_IND_COOKIE)
   {
      /* Provided the flexibility to not to configure the 
       * out streams from application. If the parameter is set to zero,
       * then, it will not be used */
#ifdef SCT6
      if((peerAssocRsp->locOutStrms > 0) &&
         (peerAssocRsp->locOutStrms <= peerCb->assocCb->assocCfg->locOutStrms))
      {
         peerCb->assocCb->assocCfg->locOutStrms = peerAssocRsp->locOutStrms;
      }
      else
      {
         /* Provide the Local error indication to application and use the 
            streams provided by SCTP */
         SZ_SEND_LCL_ERR_IND(PRSNT_NODEF, peerAssocRsp->peerId, uSapCb,
                                SZT_CAUSE_INV_STREAMS,  peerAssocRsp->locOutStrms);
      }
#else
      peerCb->assocCb->assocCfg->locOutStrms = peerAssocRsp->locOutStrms;
#endif
      assocParams.t.cookieParams.suAssocId   = peerCb->assocCb->suAssocId;
   }
   /* updating the SAP CB from the response  */
   peerCb->sztSapCb = uSapCb;

   cmHashListInsert (&szCb.peerLst, (PTR)peerCb,
                     (U8 *)&peerCb->peerId, sizeof (U32));

   /* remove the transient peerAssoc control blocks */
   SZ_FREE(peerCb->assocParams, sizeof(SctAssocIndParams));

   /* Send an alarm indicating dynamic peer is configured */
   SZ_SEND_PEER_ALARM(peerCb->peerId, LSZ_EVENT_UI_PEERASSOC_RSP,
         LSZ_CAUSE_DYNAM_PEERCFG_OK);

   /* Send the association response */
#ifdef SCT3
   SzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                   peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams,
                   0, SCT_OK, NULLP);
#else
   SzLiSctAssocRsp(&peerCb->sctSapCb->pst, peerCb->sctSapCb->spId, 
                   peerCb->sctSapCb->endpCb.locSpEndpId, &assocParams,
                   SCT_OK, NULLP);
#endif /* end of SCT3 */

   /* Populate the sta indication structure */
   sta.peerId.pres = TRUE;
   sta.peerId.val  = peerCb->peerId;
   sta.type        = SZT_STA_TYPE_PEER;
   sta.status      = SZT_STATUS_DYNAM_PEERCFG_OK;
   sta.reason      = SZT_REASON_NOT_APPL;

   /* Send status indication to user */
   SzUiSztStaInd(&uSapCb->pst, uSapCb->suId, &sta);

   /* Add the mapping for the peerCb as it is required to send in CS */
#ifdef YT
#ifdef SZ_MME
   YT_RTUPD_MOD(YT_UPD_DYNPEER_CB, peerCb);
#endif /* SZ_MME */
   YT_ADD_MAPPING(YT_UPD_ASSOC_CB, peerCb->assocCb);
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK);
} /* end of SzUiSztPeerAssocRsp */
#endif

#ifdef SZTV2
/*
 *
 *       Fun:   SzUiSztAbortS1Req    
 *
 *       Desc:  This function terminates the association.
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 SzUiSztAbortS1Req
(
Pst          *pst,
SpId         spId,
SztAbortS1   *abortS1
)
#else  /* ANSI */
PUBLIC S16 SzUiSztAbortS1Req(pst, spId, abortS1)
Pst          *pst;
SpId         spId;
SztAbortS1   *abortS1;
#endif /* ANSI */
{

   S16        ret = ROK;
   SzPeerCb   *peerCb = NULLP;
   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */

   TRC3(SzUiSztAbortS1Req);


#ifdef SS_MULTIPLE_PROCS
   ret = SGetXxCb(pst->dstProcId,pst->dstEnt,pst->dstInst,(Void **)&szCbPtr);
#if (ERRCLASS & ERRCLS_INT_PAR)
   if((ret !=ROK)||( szCbPtr == NULLP))
   {
      SLOGERROR(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, __LINE__,\
                ERRCLS_INT_PAR,ESZ190,0,
                "SzUiSztAbortS1Req() failed, cannot derive szCb");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
#ifdef DEBUGP
   SZDBGP(DBGMASK_UI,(szCb.init.prntBuf,
         "--------S1AP-----(proc(%d),ent(%d),inst(%d))-------\n",
         pst->dstProcId, pst->dstEnt, pst->dstInst));
#endif /* DEBUGP */
#else /* SS_MULTIPLE_PROCS */
   UNUSED(pst);
#endif /* SS_MULTIPLE_PROCS */

   /* sz008.301 : Kwork Fix,
    * Rearranging the NULL checks */
   if(szUiGetAndChkSztSap(spId, &sapCb) != ROK)
   {
#ifdef DEBUGP
      SZDBGP(DBGMASK_UI, (szCb.init.prntBuf,
            "SzUiSztAbortS1Req: Invalid SZT sap\n"));
#endif /* DEBUGP */ 
      RETVALUE(RFAILED);
   }

   if(abortS1 == NULLP)
   {
      /* Error Indication to the user */
      SZ_SEND_LCL_ERR_IND(NOTPRSNT, 0, sapCb, SZT_CAUSE_UNEXP_MSG, 0);
      RETVALUE(RFAILED);
   }

   abortS1->abortS1Cfm.status = SZT_PRIM_OK;
   abortS1->abortS1Cfm.reason = SZT_CAUSE_NOT_APPL;

   ret = szNdbChkPeer(abortS1->peerId, &peerCb);
   if (ret != ROK)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ191, (ErrVal) 0,
                 "SzUiSztAbortS1Req: PeerCb not found \n");
#endif /* ERRCLS */
      abortS1->abortS1Cfm.status = SZT_PRIM_NOK;
      abortS1->abortS1Cfm.reason = SZT_CAUSE_INV_PEERID;

      SzUiSztAbortS1Cfm(&sapCb->pst, sapCb->suId, abortS1); 
      RETVALUE(ret);
   }

   if ((peerCb->state == SZT_ND_ST_IDLE) && (peerCb->assocCb->assocState == SZ_ASSOC_DOWN))
   {      
      SzUiSztAbortS1Cfm(&sapCb->pst, sapCb->suId, abortS1);
      RETVALUE(ret);
   }

   peerCb->s1Abort = TRUE;
   /* sz008.301 : purify Fix: If abort is after sending the setupReq,
    * we need to free the setupReq memorey first. */
   SZ_FREE_SDU(peerCb->setupReq); 

   /* Terminate the association */
   ret = szLiTermAssoc(&peerCb, TRUE);
#if (ERRCLASS & ERRCLS_DEBUG)
   if (ret != ROK)
   {
      SZLOGERROR(ERRCLS_DEBUG, ESZ192, (ErrVal)peerCb->peerId,
            "SzUiSztAbortS1Req: Failed to terminate an association");
   }
#endif /* (ERRCLASS & ERRCLS_DEBUG) */

   /* Send the queued update message to peer */
#ifdef YT
   YT_UPD_PEER();
#endif /* YT */

   RETVALUE(ROK); 
} /* SzUiSztAbortS1Req */

#endif /* SZTV2 */

/*
 *
 *       Fun:   szUiSndConInd
 *
 *       Desc:  Connection Indication wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndConInd 
(
SzConCb *conCb, 
SztConInd *conInd
)
#else /* ANSI */
PUBLIC S16 szUiSndConInd(conCb, conInd)
SzConCb *conCb;
SztConInd *conInd;
#endif /* ANSI */
{

   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */
   SzPeerCb   *peer;

   TRC3(szUiSndConInd);

   peer = conCb->cd.peerCb;

   sapCb = peer->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ193, (ErrVal) sapCb->suId, 
            "szUiSndConInd: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(RFAILED);
   }

   /* Update statistics for this peerCb, sapCb and genSts */
   SZ_STS_INC_DATA_RX_SZT(peer, conInd->pdu, conCb);

   SzUiSztConInd(&sapCb->pst, sapCb->suId, conInd);

   RETVALUE(ROK);
} /* szUiSndConInd */


/*
 *
 *       Fun:   szUiSndConCfm
 *
 *       Desc:  Connection Confirmation wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndConCfm 
(
SzConCb *conCb, 
SztConCfm *conCfm
)
#else /* ANSI */
PUBLIC S16 szUiSndConCfm(conCb, conCfm)
SzConCb *conCb;
SztConCfm *conCfm;
#endif /* ANSI */
{

   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */
   SzPeerCb   *peer;

   TRC3(szUiSndConCfm);

   peer = conCb->cd.peerCb;

   sapCb = peer->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ194, (ErrVal) sapCb->suId, 
            "szUiSndConCfm: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      SZ_FREE_SDU(conCfm->pdu);
      RETVALUE(RFAILED);
   }

   /* Update statistics for this peerCb, sapCb and genSts */
   SZ_STS_INC_DATA_RX_SZT(peer, conCfm->pdu, conCb);

   SzUiSztConCfm(&sapCb->pst, sapCb->suId, conCfm);

   RETVALUE(ROK);
} /* szUiSndConCfm */


/*
 *
 *       Fun:   szUiSndDatInd
 *
 *       Desc:  Data Indication wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndDatInd 
(
SzConCb *conCb, 
SztDatEvntInd *datEvn
)
#else /* ANSI */
PUBLIC S16 szUiSndDatInd(conCb, datEvn)
SzConCb *conCb;
SztDatEvntInd *datEvn;
#endif /* ANSI */
{

   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */
   SzPeerCb   *peer;

   TRC3(szUiSndDatInd);

   peer  = conCb->cd.peerCb;

   sapCb = peer->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ195, (ErrVal) sapCb->suId, 
            "szUiSndDatInd: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(RFAILED);
   }

   /* Update statistics for this peerCb, sapCb and genSts */
   SZ_STS_INC_DATA_RX_SZT(peer, datEvn->pdu, conCb);

   SzUiSztDatInd(&sapCb->pst, sapCb->suId, datEvn);

   RETVALUE(ROK);
} /* szUiSndDatInd */

/*
 *
 *       Fun:   szUiSndUDatInd
 *
 *       Desc:  Unit data indication wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndUDatInd 
(
SzPeerCb *peerCb, 
SztUDatEvnt *uDatEvn
)
#else /* ANSI */
PUBLIC S16 szUiSndUDatInd(peerCb, uDatEvn)
SzPeerCb *peerCb;
SztUDatEvnt *uDatEvn;
#endif /* ANSI */
{

   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */

   TRC3(szUiSndUDatInd);

   sapCb = peerCb->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ196, (ErrVal) sapCb->suId, 
            "szUiSndUDatInd: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      SZ_FREE_SDU(uDatEvn->pdu);
      RETVALUE(RFAILED);
   }

   /* Update statistics for this peerCb, sapCb and genSts */
   peerCb->peerSts.cfm.szComProcSts.rx++; 
   peerCb->sctSapCb->sts.cfm.szComProcSts.rx++;

   SzUiSztUDatInd(&sapCb->pst, sapCb->suId, uDatEvn);

   RETVALUE(ROK);
} /* szUiSndDatInd */

/*
 *
 *       Fun:   szUiSndRelInd
 *
 *       Desc:  Release Indication wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndRelInd 
(
SzConCb *conCb, 
SztRelInd *relInd
)
#else /* ANSI */
PUBLIC S16 szUiSndRelInd(conCb, relInd)
SzConCb *conCb;
SztRelInd *relInd;
#endif /* ANSI */
{

   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */
   SzPeerCb   *peer;

   TRC3(szUiSndRelInd);

   peer = conCb->cd.peerCb;

   sapCb = peer->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ197, (ErrVal) sapCb->suId, 
            "szUiSndRelInd: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      SZ_FREE_SDU(relInd->pdu);
      RETVALUE(RFAILED);
   }

   /* Update statistics for this peerCb, sapCb and genSts */
   SZ_STS_INC_DATA_RX_SZT(peer, relInd->pdu, conCb);

   SzUiSztRelInd(&sapCb->pst, sapCb->suId, relInd);

   RETVALUE(ROK);
} /* szUiSndRelInd */

/*
 *
 *       Fun:   szUiSndRelCfm
 *
 *       Desc:  Release Confirmation wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndRelCfm 
(
SzConCb *conCb, 
SztRelCfm *relCfm
)
#else /* ANSI */
PUBLIC S16 szUiSndRelCfm(conCb, relCfm)
SzConCb *conCb;
SztRelCfm *relCfm;
#endif /* ANSI */
{

   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */
   SzPeerCb   *peer;

   TRC3(szUiSndRelCfm);

   peer  = conCb->cd.peerCb;

   sapCb = peer->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ198, (ErrVal) sapCb->suId, 
            "szUiSndRelCfm: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      SZ_FREE_SDU(relCfm->pdu);
      RETVALUE(RFAILED);
   }

   /* Update statistics for this peerCb, sapCb and genSts */
   if (relCfm->relType != SZT_LOCAL_REL)
   {
      SZ_STS_INC_DATA_RX_SZT(peer, relCfm->pdu, conCb);
   }

   SzUiSztRelCfm(&sapCb->pst, sapCb->suId, relCfm);

   RETVALUE(ROK);
} /* szUiSndRelCfm */

#ifdef SZ_MME
/*
 *
 *       Fun:   szUiSndPeerAssocInd
 *
 *       Desc:  Dynamic peer assoc Indication wrapper function
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szUiSndPeerAssocInd 
(
SzPeerCb        *peerCb,
SztPeerAssocInd *peerAssocInd
)
#else /* ANSI */
PUBLIC S16 szUiSndPeerAssocInd(peerCb, peerAssocInd)
SzPeerCb        *peerCb;
SztPeerAssocInd *peerAssocInd;
#endif /* ANSI */
{
   SzSztSapCb *sapCb; /* SZT SAP Control Block pointer */

   TRC3(szUiSndPeerAssocInd);

   sapCb = peerCb->sztSapCb;

   if (sapCb->state != LSZ_SAP_ISBOUND)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ199, (ErrVal) sapCb->suId, 
            "szUiSndPeerAssocInd: Invalid SAP State");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE(RFAILED);
   }

   SzUiSztPeerAssocInd(&sapCb->pst, sapCb->suId, peerAssocInd);

   RETVALUE(ROK);
} /* szUiSndPeerAssocInd */

#endif

/*
 *
 *       Fun:   szUiGetAndChkSztSap
 *
 *       Desc:  This function checks SAP state and returns 
 *              sapCb if present
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PRIVATE S16 szUiGetAndChkSztSap 
(
SpId       spId,
SzSztSapCb **uSapCb
)
#else /* ANSI */
PRIVATE S16 szUiGetAndChkSztSap(spId, uSapCb)
SpId       spId;
SzSztSapCb **uSapCb;
#endif /* ANSI */
{
#if (ERRCLASS & ERRCLS_INT_PAR)
   SzUstaDgn dgn; 
#endif /* ERRCLASS & ERRCLS_INT_PAR */

   TRC3(szUiGetAndChkSztSap);

   *uSapCb = NULLP; 

   if ((spId < szCb.genCfg.nmbSztSaps) && spId >= 0) 
   { 
      *uSapCb = szCb.sztSapLst[spId]; 

      if ((*uSapCb != NULLP) && 
         ((*uSapCb)->state == LSZ_SAP_ISBOUND)) 
      { 
         RETVALUE(ROK); 
      } 
      else
      { 
#if (ERRCLASS & ERRCLS_INT_PAR)
         dgn.type = LSZ_USTA_DGNVAL_SAPID; 
         dgn.u.sapId = spId; 
         szSendLmAlarm (LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
                        LCM_CAUSE_INV_SAP, &dgn); 

#endif /* ERRCLASS & ERRCLS_INT_PAR */
         RETVALUE(RFAILED); 
      } 
   } 
   else 
   { 
#if (ERRCLASS & ERRCLS_INT_PAR)
      dgn.type = LSZ_USTA_DGNVAL_SAPID; 
      dgn.u.sapId = spId; 
      szSendLmAlarm (LCM_CATEGORY_INTERFACE, LCM_EVENT_UI_INV_EVT, 
                     LCM_CAUSE_INV_SPID, &dgn); 
#endif /* ERRCLASS & ERRCLS_INT_PAR */
      RETVALUE (RFAILED); 
   } 
} /* szUiGetAndChkSztSap */

/*
 *
 *       Fun:   szUiDoAudit    
 *
 *       Desc:  This function do the Audit on connection/peer 
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PRIVATE S16 szUiDoAudit 
(
SztAudEvnt *audit,   /* Audit Event */
SzSztSapCb *sztSapCb,  /* Upper Sap */
U16        *cause
)
#else /* ANSI */
PRIVATE S16 szUiDoAudit(audit, sztSapCb, cause)
SztAudEvnt *audit;   /* Audit Event */
SzSztSapCb *sztSapCb;  /* Upper Sap */
U16        *cause;
#endif /* ANSI */
{
   SzConCb    *connCb;        /* Connection control block */
   S16         ret;           /* return value */

   TRC3(szUiDoAudit);

   connCb = NULLP;
   *cause = SZT_CAUSE_NOT_APPL;

   switch(audit->type)
   {
      case SZT_AUD_CONN:
      {
         U8   loopCnt;

         for (loopCnt = 0; loopCnt < audit->u.audConn.nmbConn; loopCnt++)
         {
            audit->u.audConn.connSt[loopCnt].state =  SZT_CON_ST_NON_EXST;

            if (audit->u.audConn.connIdType == SZT_CONN_TYPE_SP)
            {
               if ((ret = szNdbGetConCb(audit->u.audConn.connSt[loopCnt]\
                                          .connId, &connCb)) == ROK)
               {
                  audit->u.audConn.connSt[loopCnt].state = connCb->state; 
               }
            }
            else
            {
               ret = cmHashListFind(&sztSapCb->suConnLst, 
                                    (U8 *)&(audit->u.audConn.connSt[loopCnt]\
                                    .connId), sizeof (UConnId), 0,
                                    (PTR *)&connCb);
               if (ret == ROK)
               {
                  audit->u.audConn.connSt[loopCnt].state = connCb->state; 
               }
            }

            /* Update the audit statistics after the connection
             * is scanned */
            if(audit->u.audConn.connSt[loopCnt].state != SZT_CON_ST_NON_EXST)
            {
               szCb.genSts.audSts.nmbAudConn++;
            }
         }
      }
      break;

      case SZT_AUD_PEER:
      {
         SzPeerCb    *peer;
         TknU32      peerId;

         peerId.pres = PRSNT_NODEF;
         peerId.val = audit->u.audPeer.peerId;

         if (szNdbChkPeer(peerId, &peer) != ROK)
         {
            *cause = SZT_CAUSE_INV_PEERID;

            RETVALUE(RFAILED);
         }

         audit->u.audPeer.peerState = peer->state;
         /* Update the audit statistics after the peer is audited
          * successfully */
         szCb.genSts.audSts.nmbAudPeer++;
      }
      break;
      default:
      break;
   }

   RETVALUE (ROK); 
}

#ifdef SZ_MME
/*
 *
 *       Fun:   szValidatePeerAssocRsp    
 *
 *       Desc:  This function validates the response received from the user
 *              for the configuration of dynamic peer.
 *
 *       Ret:   ROK     - success
 *              RFAILED - failure
 *
 *       Notes: None 
 *
 *       File:  sz_ui.c
 *
 */
  
#ifdef ANSI
PUBLIC S16 szValidatePeerAssocRsp 
(
SzPeerCb           *peerCb,
SztPeerAssocRsp    *peerAssocRsp,
U8                 *reason
)
#else /* ANSI */
PUBLIC S16 szValidatePeerAssocRsp(peerCb, peerAssocRsp, reason)
SzPeerCb           *peerCb;
SztPeerAssocRsp    *peerAssocRsp;
U8                 *reason;
#endif /* ANSI */
{
   S16           ret; 
   TknU32        peerIdTkn;
   SzPeerCb     *peer;
   SztStrmId     locOutStrms;
                                                                        
   TRC3(szValidatePeerAssocRsp);

   /* check if a peer already exists with received peerId */
   peerIdTkn.pres = 1;
   peerIdTkn.val  = peerAssocRsp->peerId;

   ret = szNdbChkPeer(peerIdTkn, &peer);
   if (peer != NULLP)
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ200, (ErrVal) peerAssocRsp->peerId,
       "szValidatePeerAssocRsp: peer already exists with received peerId\n");
#endif /* ERRCLASS & ERRCLS_INT_PAR */
   
      *reason = SZT_REASON_DUP_PEERID;

      RETVALUE(RFAILED);
   } 

   if (peerCb->assocParams->type == SCT_ASSOC_IND_COOKIE)
   {
      locOutStrms = peerAssocRsp->locOutStrms;
   }
   else
   {
      locOutStrms = peerCb->assocCb->assocCfg->locOutStrms;
   }

   if ((peerAssocRsp->comProcStrm != 0) &&
       (peerAssocRsp->comProcStrm != (locOutStrms - 1)))
   {
#if (ERRCLASS & ERRCLS_INT_PAR)
      SZLOGERROR(ERRCLS_INT_PAR, ESZ201, (ErrVal) peerAssocRsp->peerId,
       "szValidatePeerAssocRsp: common procedure\
       stream id recieved is invalid\n");
#endif /* ERRCLASS & ERRCLS_INT_PAR */

      *reason = SZT_REASON_INV_PARAM;

      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
#endif

/**********************************************************************
 
         End of file:     sq_ui.c@@/main/nodal_main/3 - Fri Nov 22 18:01:36 2013
 
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
/main/5      ---      va    1. Updated for S1AP release 4.1
*********************************************************************91*/


