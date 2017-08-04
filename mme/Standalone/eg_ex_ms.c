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

     Name:    Egtp  

     Type:    C source file

     Desc:    Functions required for scheduling and initialization.

     File:    eg_ex_ms.c

     Sid:      pg_ex_ms.c@@/main/3 - Fri Feb  4 18:18:15 2011

     Prg:     pc 

*********************************************************************21*/



/************************************************************************
 * H E A D E R     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "envopt.h"        /* Environment options             */
#include "envdep.h"        /* Environment dependent options   */
#include "envind.h"        /* Environment independent options */
#include "gen.h"           /* General layer                   */
#include "ssi.h"           /* System service interface        */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* Common transport library        */
#include "cm_dns.h"        /* common transport */
#include "cm_tkns.h"       /* Common tokens                   */
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "hit.h"           /* HIT interface defines           */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"           /* SHT include file                */
#endif
#include "egt.h"           /* EGT interface defines           */
#include "eg_util.h"       /* Egtp Utility library            */
#include "leg.h"           /* Layer management, Egtp          */
#include "eg.h"            /* Egtp layer defines              */
#include "eg_err.h"        /* Error codes                     */
#include "eg_edm.h"        /* EDM Module structures            */


/************************************************************************
 * E X T E R N     I N C L U D E        F I L E S                       *
 ***********************************************************************/
#include "gen.x"           /* General layer                   */
#include "ssi.x"           /* System services interface       */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* Common tokens                   */
#include "cm_tpt.x"        /* Common transport library        */
#include "cm_dns.x"        /* common transport */
#include "cm_mblk.x"       /* Common memory allocation        */
#include "cm_inet.x"       /* Common socket library           */
#include "hit.x"           /* HIT interface defines           */
/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"           /* SHT include file                */
#endif
#include "egt.x"           /* SOT interface defines           */
#include "eg_util.x"       /* Egtp Utility library */
#include "leg.x"           /* Layer management Egtp            */
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* Egtp layer structures            */
#include "eg_tpt.x"            /* EGTP TPT module defines */

/*eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

/*
*
*       Fun:    activate task
*
*       Desc:   Processes received events from the Egtp interfaces.
*               Primitives are unpacked and the correct functions are called.
*
*       Ret:    ROK  -    ok
*               RFAILED - failed
*
*       Notes:  This is only used for dispatching to the unpacking routines.
*               If tightly coupled then we should never be in this file.
*
*       File:   eg_ex_ms.c
*
*/
#ifdef ANSI
PUBLIC S16 egActvTsk
(
Pst    *pst,                /* post message                       */
Buffer *mBuf                /* message buffer                     */
)
#else
PUBLIC S16 egActvTsk(pst, mBuf)
Pst    *pst;                /* post                               */
Buffer *mBuf;               /* message buffer                     */
#endif
{
   /* local variables */
   S16         retVal = ROK;/* return code                        */
   Mem         sMem;        /* mem block passed to unpacking code */
#ifdef EGTP_U   
#ifdef EGTP_U_MULTI_THREADED 
   EgUEvnt *eguEvntMsg;     /*for messages coming for worker thread*/
#endif /* EGTP_U */
#endif /* EGTP_U */
   
   EG_TRC2(egActvTsk);

#ifdef SS_MULTIPLE_PROCS
   if((SGetXxCb(pst->dstProcId, pst->dstEnt,\
                egCb.init.inst,(Void **)&egCbPtr))!=ROK)
   {
      EGLOGERROR(ERRCLS_DEBUG, EEG094, 0,\
                 "egActvTsk() failed, cannot derive egCb");
      RETVALUE(RFAILED);
   }

   EG_DBG_INFO(0,0, (egp,\
          "-----EGTP-----(proc(%d), entt(%d), inst(%d))------\n"\
                   ,pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif/* SS_MULTIPLE_PROCS */
 
   sMem.region = egCb.init.region;
   sMem.pool   = egCb.init.pool;                         
   
   /********************************************
   *  Check if the received mBUF if null?      *
   ********************************************/
   if(mBuf == NULLP)
   {
      EGLOGDBGERR( EEG095, pst->event,
         "egActvTsk: received mBuf is NULLP");
      EG_RETVALUE(RFAILED);
   } 

   switch(pst->srcEnt)
   {
#ifdef LCEGLIHIT
      /********************************************
      *    loosely coupled lower interface        *
      ********************************************/                
      case ENTHI:
      {
         switch(pst->event)
         {
            /********************************************
            *             BIND   CONFIRM                *
            ********************************************/                
            case EVTHITBNDCFM:  
            {
               retVal = cmUnpkHitBndCfm(EgLiHitBndCfm, pst, mBuf);
               break;
            } 

            /********************************************
            *             CONNECTION   CONFIRM                *
            ********************************************/                
            case EVTHITCONCFM:  
            {
               retVal = cmUnpkHitConCfm(EgLiHitConCfm, pst, mBuf);
               break;
            } 
#ifdef EG_PHASE2            
            /********************************************
            *             FLOW   INDICATION             *
            ********************************************/                
            case EVTHITFLCIND:
            {
               retVal = cmUnpkHitFlcInd(EgLiHitFlcInd, pst, mBuf);
               break;
            }
#endif /* EG_PHASE2 */
            /********************************************
            *         UDP DATA INDICATION               *
            ********************************************/                
            case EVTHITUDATIND:         
            {
               retVal = cmUnpkHitUDatInd(EgLiHitUDatInd, pst, mBuf);
               break;
            } 

            /********************************************
            *             DISC   INDICATION             *
            ********************************************/                
            case EVTHITDISCIND:           
            {
               retVal = cmUnpkHitDiscInd(EgLiHitDiscInd, pst, mBuf);
               break;
            }

            /********************************************
            *             DISC   CONFIRM                *
            ********************************************/                
            case EVTHITDISCCFM:
            {
               retVal = cmUnpkHitDiscCfm(EgLiHitDiscCfm, pst, mBuf);
               break;
            }

#ifdef ERRCHK
            /********************************************
            *      Should hopefully never get here      *
            ********************************************/                
            default:
            {
               EG_FREEMBUF(mBuf);
               EGLOGDBGERR( EEG096, pst->event,
                          "egActvTsk: Invalid event");
               retVal = RFAILED;
               break;
            }
#endif
         } 
         break;
      } 
#endif /* LCEGLIHIT */

#if (defined(LCEGUIEGT) || defined(LWLCEGUIEGT))         
      /********************************************
      *    Loosely Coupled UPPER INTERFACE        *
      ********************************************/                
      case ENTSC: /*SGSN application */
      case ENTEU:
      case ENTVE:
      {
         switch(pst->event)
         {
            /********************************************
            *               BIND  REQUEST               *
            ********************************************/                
            case EVTEGTBNDREQ:
            {
               retVal = cmUnpkEgtBndReq(EgUiEgtBndReq, pst, mBuf);
               break;
            }

            /********************************************
            *             UNBIND  REQUEST               *
            ********************************************/                
            case EVTEGTUBNDREQ:
            {
               retVal = cmUnpkEgtUbndReq(EgUiEgtUbndReq, pst, mBuf);
               break;
            }
            
#ifdef EGTP_C
            /********************************************
            *               SIG  REQUEST                *
            ********************************************/                
            case EVTEGTSIGREQ:
            {
               retVal = cmUnpkEgtSigReq(EgUiEgtSigReq, pst, mBuf, &sMem);
               break;
            }

            /********************************************
            *               STATUS  REQUEST             *
            ********************************************/                
            case EVTEGTSTAREQ:
            {
               retVal = cmUnpkEgtStaReq(EgUiEgtStaReq, pst, mBuf);
               break;
            }

            /********************************************
            *          LOCAL DELETE REQUEST             *
            ********************************************/                
            case EVTEGTLCLDELREQ:
            {
               retVal = cmUnpkEgtLclDelReq(EgUiEgtLclDelReq, pst, mBuf);
               break;
            }

            /********************************************
            *               SIG  RESPONSE               *
            ********************************************/                
            case EVTEGTSIGRSP:
            {
/* eg001.201  Updating the Procedure arguments */
               retVal = cmUnpkEgtSigRsp(EgUiEgtSigRsp, pst, mBuf, &sMem);
               break;
            }
            /* ccpu00137922 */
            /********************************************
            *               SIG  REQUEST                *
            ********************************************/                
            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
            case EVTEGTTRANSSIGREQ:
            {
               retVal = cmUnpkEgtTransSigReq(EgUiEgtTransSigReq, pst, mBuf, &sMem);
               break;
            }

            /********************************************
            *               SIG  RESPONSE               *
            ********************************************/                
            /* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
            case EVTEGTTRANSSIGRSP:
            {
               retVal = cmUnpkEgtTransSigRsp(EgUiEgtTransSigRsp, pst, mBuf, &sMem);
               break;
            }

#endif /* EGTP_C */
#ifdef EGTP_U
            /********************************************
            *           UNIT DATA REQUEST                *
            ********************************************/                
            case EVTEGTUDATREQ:
            {
/* eg001.201  Updating the Procedure arguments */
               retVal = cmUnpkEgtEguDatReq(EgUiEgtEguDatReq, pst, mBuf, &sMem);
               break;
            }

            /********************************************
            *    TUNNEL MANAGEMENT  REQUEST             *
            ********************************************/                
            case EVTEGTULCLTNLMGMTREQ:
            {
/* eg001.201  Updating the Procedure arguments */
               retVal = cmUnpkEgtEguLclTnlMgmtReq(EgUiEgtEguLclTnlMgmtReq, pst, mBuf, &sMem);
               break;
            }

            /********************************************
            *                STATUS REQUEST             *
            ********************************************/                
            case EVTEGTUSTAREQ:
            {
/* eg001.201  Updating the Procedure arguments */
               retVal = cmUnpkEgtEguStaReq(EgUiEgtEguStaReq, pst, mBuf, &sMem);
               break;
            }

#endif /* EGTP_U */

#ifdef ERRCHK
            /********************************************
            *      Should hopefully never get here      *
            ********************************************/                
            default:
            {
               EG_FREEMBUF(mBuf);
               EGLOGDBGERR( EEG097, pst->event,
                          "egActvTsk: Invalid event");
               retVal = RFAILED;
               break;
            }
#endif
         }

         break;
      } 
#endif /* LCEGUIEGU or LWLCEGUIEGU */

#ifdef LCSMEGMILEG
      /********************************************
      *  Loosely coupled LAYER MANAGER INTFERFACE *
      ********************************************/                
      case ENTSM:
      {
         switch(pst->event)
         {
            /********************************************
            *             CONFIG  REQUEST               *
            ********************************************/                
            case EVTLEGCFGREQ:
            {
               retVal = cmUnpkLegCfgReq(EgMiLegCfgReq, pst, mBuf);
               break;
            }

            /********************************************
            *            CONTROL  REQUEST               *
            ********************************************/                
            case EVTLEGCNTRLREQ:
            {
               retVal = cmUnpkLegCntrlReq(EgMiLegCntrlReq, pst, mBuf);
               break;
            }

            /********************************************
            *               STATUS  REQUEST             *
            ********************************************/                
            case EVTLEGSTAREQ:
            {
               retVal = cmUnpkLegStaReq(EgMiLegStaReq, pst, mBuf);
               break;
            }

            /********************************************
            *           STATISTICS  REQUEST             *
            ********************************************/                
            case EVTLEGSTSREQ:
            {
               retVal = cmUnpkLegStsReq(EgMiLegStsReq, pst, mBuf);
               break;
            }

            /********************************************
            *      Should hopefully never get here      *
            ********************************************/                
            default:
/*eg004.201 Added for eGTP-C PSF Upgrade:PSF Active task handler to handle PSF event*/
#ifdef HW
               hwActvTsk(pst, mBuf);
#else
            {
               EG_FREEMBUF(mBuf);
               EGLOGDBGERR( EEG098, pst->event, "egActvTsk: Invalid event");
               retVal = RFAILED;
               break;
            }
#endif
         }

         break;
      }
#endif /* LCSMEGMILEG */
#ifdef EGTP_U_MULTI_THREADED 
      case ENTEG:
      {
        /**********************************************
         ** Unpack the Egu event message from the mBuf *
         ***********************************************/

         CMCHKUNPKLOG(cmUnpkPtr, (PTR*)&eguEvntMsg, mBuf, EEG099, pst);
         EG_FREEMBUF(mBuf);

         /*****************
          ** free the mBuf *
          ** ***************/
         switch (pst->event)
         {
            case EGTEVTLEGCNTRLCFM:
               {
                  EgMiHdlCntrlCfmFrmWrkr(eguEvntMsg); 
                  break;
               }/* End of case */
            case EGTEVTEGUTNLCFM:
               {
                  egUTunnDeleteTunnel(eguEvntMsg);
                  break;
               }
            case EGTEVTEGUTNLREORDCFM:
               {
                  egUTunnReordCfm(eguEvntMsg);
                  break;
               }

            case EGTEVTUSAPUBNDCFM:

               {
                  egUiProcUbndReq(eguEvntMsg);
                  break;
               }
            default :
               {
                  EGLOGDBGERR( EEG100, pst->event,
                               "egActvTsk: Invalid event");
                  EG_FREEMBUF(mBuf);
                  retVal = RFAILED;
                  break;
               }
         }/* end of switch() */
         break;
      }
#endif
/*eg004.201 Added for eGTP-C PSF Upgrade:PSF Event PACK/UNPACK followed by invoking respective
 *event hadlers 
 */
#ifdef TDS_CORE2
      case ENTSH:
      switch (pst->event)
      {
#ifdef EG_FTHA
         case EVTSHTCNTRLREQ:            /* system agent control request */
            cmUnpkMiShtCntrlReq(EgMiShtCntrlReq, pst, mBuf);
            break;
#endif
         default:
#ifdef HW
            hwActvTsk(pst, mBuf);
#else
               EG_FREEMBUF(mBuf);
               EGLOGDBGERR( EEG101, pst->event,
                    "egActvTsk: Invalid event");
#endif
            break;
      }
      break;
#endif
#ifdef HW
      case ENTEG:
      hwActvTsk(pst, mBuf);
      break; 
#endif
      /********************************************
      *      Should hopefully never get here      *
      ********************************************/                
      default:
      {
         EGLOGDBGERR( EEG101, pst->event,
                    "egActvTsk: Invalid event");
         EG_FREEMBUF(mBuf);
         retVal = RFAILED;
         break;
      }
   }

   SExitTsk();
   EG_RETVALUE(retVal);
}


#ifdef EGTP_U_MULTI_THREADED

/******************************************************************************
*
*       Fun:    egWrkrActvTsk 
*
*       Desc:   Processes received events from the Egtp Main Thread 
*               Primitives are unpacked and the correct functions are called.
*
*       Ret:    ROK  -    ok
*               RFAILED - failed
*
*       Notes:  This is only used for dispatching to the unpacking routines.
*               If tightly coupled then we should never be in this file.
*
*       File:   eg_ex_ms.c
*
*******************************************************************************/
#ifdef ANSI
PUBLIC S16 egWrkrActvTsk
(
Pst    *pst,                /* post message                       */
Buffer *mBuf                /* message buffer                     */
)
#else
PUBLIC S16 egWrkrActvTsk(pst, mBuf)
Pst    *pst;                /* post                               */
Buffer *mBuf;               /* message buffer                     */
#endif
{
   /* local variables */
   S16         retVal = ROK;/* return code                        */
   Mem         sMem;        /* mem block passed to unpacking code */
   EgUEvnt     *evntMsg;         /* for Msgs coming from worker thread */
   Buffer      *cfmMbuf;               /* message buffer                     */
   EgUstaDgn   dgn;
   Inst         tmpInst = 0;
   EgUEvnt     *eguEvntMsg = NULLP;
   Pst pstMult;
   Pst *tmpPst;



   EG_TRC2(egWrkrActvTsk);

#ifdef SS_MULTIPLE_PROCS
   if((SGetXxCb(pst->dstProcId, pst->dstEnt,\
               egCb.init.inst,(Void **)&egCbPtr))!=ROK)
   {
      EGLOGERROR(ERRCLS_DEBUG, EEG102, 0,\
            "egWrkrActvTsk() failed, cannot derive egCb");
      RETVALUE(RFAILED);
   }

   EG_DBG_INFO(0,0, (egp,\
            "-----EGTP-----(proc(%d), entt(%d), inst(%d))------\n"\
            ,pst->dstProcId, pst->dstEnt, egCb.init.inst));

#endif/* SS_MULTIPLE_PROCS */

   sMem.region = egCb.init.region;
   sMem.pool   = egCb.init.pool;                         
   evntMsg = NULLP;

   /********************************************
    *  Check if the received mBUF if null?      *
    ********************************************/
   if(mBuf == NULLP)
   {
      EGLOGDBGERR( EEG103, pst->event,
            "egWrkrActvTsk: received mBuf is NULLP");
      EG_RETVALUE(RFAILED);
   } 

   switch(pst->srcEnt)
   {

      case ENTEG:
         {

            /**********************************************
             * Unpack the Egu event message from the mBuf *
             **********************************************/
            CMCHKUNPKLOG(cmUnpkPtr, (PTR*)&eguEvntMsg, mBuf, EEG104, pst);
            EG_FREEMBUF(mBuf);
            /*****************
             * free the mBuf *
             *****************/
            switch(pst->event)
            {
               /********************************************
                *       Posting Worker Thread Message       *
                *********************************************/
               case EVTEGUMTWRK:
                  {
                     egUCpmProcMsg(eguEvntMsg, pst);
                     break;
                  }

               case EGTEVTCNTRLREQ:
               case EGTEVTTNLDELREQ:
               case EGTEVTUSAPUBNDREQ:
                  {
                     /***********************************************
                      *If re-ordering enabled then flush the buffer,* 
                      *else send confiramtion.                      *
                      ***********************************************/
                     if(eguEvntMsg->teIdCb->reOrdering)
                     {
                        /*set event type*/
                        /* Stop the re-order timer */
                        egSchedreOrdTmr((Ptr)eguEvntMsg->teIdCb, EG_TMR_REORD_EXP, TMR_STOP, egCb.genCfg.reOrderTmr.val);
                        /* Flush all the data */
                        egFlushAllPackets(eguEvntMsg->teIdCb);
                        /* start tunnel delete timer */
                        egSchedreOrdTmr((Ptr) eguEvntMsg, EG_TMR_TUNDEL_EXP, TMR_START,(U8) 1);
                     }
                     else
                     {
                        retVal = SGetMsg(eguEvntMsg->region, eguEvntMsg->pool, &cfmMbuf);
                        if(retVal != ROK)
                        {
                           /********************************************************
                            * Memory allocation failed, send alarm to LM and error *
                            *********************************************************/
                           EGU_SND_ALARM_MEM_ALLOC_FAIL(eguEvntMsg->region, eguEvntMsg->pool, dgn);
                           EGU_FREEEVNT(eguEvntMsg);
                           EG_RETVALUE(RFAILED);
                        }/* end of if */

                        pstMult.dstProcId = egCb.init.procId;
                        pstMult.srcProcId = egCb.init.procId;
                        pstMult.srcEnt = egCb.init.ent;
                        pstMult.dstEnt = egCb.init.ent;
                        pstMult.srcInst = pst->dstInst;
                        pstMult.dstInst = egCb.init.inst;
                        pstMult.prior = PRIOR0;
                        pstMult.route = RTESPEC;
                        pstMult.region = eguEvntMsg->region;
                        pstMult.pool = eguEvntMsg->pool;
                        pstMult.selector = 0;
                        if(pst->event == EGTEVTTNLDELREQ)
                        {
                           pstMult.event = EGTEVTEGUTNLCFM;
                        }
                        else if(pst->event == EGTEVTCNTRLREQ)
                        {
                           pstMult.event = EGTEVTLEGCNTRLCFM;
                           if(eguEvntMsg->eventType == EGTEVTUSAPUBNDREQ)
                           {
                              pstMult.event = EGTEVTUSAPUBNDCFM;
                           }
                        }
                        tmpPst = & pstMult;
                        CMCHKPKLOG(cmPkPtr, (PTR)eguEvntMsg, cfmMbuf, EEG105, tmpPst);
                        SPstTsk(&pstMult, cfmMbuf);

                     }
                     EG_RETVALUE(ROK);
                     break;
                  }/* end of case EGT_EVT_EGT_TNL_DEL_CFM */
               case EGTEVTTNLREORDDIS:
                  {
                     egFlushAllPackets(eguEvntMsg->teIdCb);
                     eguEvntMsg->teIdCb->reOrdering = FALSE;
                     retVal = SGetMsg(eguEvntMsg->region, eguEvntMsg->pool, &cfmMbuf);
                     if(retVal != ROK)
                     {
                        /********************************************************
                         * Memory allocation failed, send alarm to LM and error *
                         *********************************************************/
                        EGU_SND_ALARM_MEM_ALLOC_FAIL(eguEvntMsg->region, eguEvntMsg->pool, dgn);
                        EGU_FREEEVNT(eguEvntMsg);
                        EG_RETVALUE(RFAILED);
                     }/* end of if */

                     pstMult.dstProcId = egCb.init.procId;
                     pstMult.srcProcId = egCb.init.procId;
                     pstMult.srcEnt = egCb.init.ent;
                     pstMult.dstEnt = egCb.init.ent;
                     pstMult.srcInst = pst->dstInst;
                     pstMult.dstInst = egCb.init.inst;
                     pstMult.prior = PRIOR0;
                     pstMult.route = RTESPEC;
                     pstMult.region = eguEvntMsg->region;
                     pstMult.pool = eguEvntMsg->pool;
                     pstMult.selector = 0;
                     pstMult.event = EGTEVTEGUTNLREORDCFM;
                     tmpPst = &pstMult;
                     CMCHKPKLOG(cmPkPtr, (PTR)eguEvntMsg, cfmMbuf, EEG106, tmpPst);
                     SPstTsk(&pstMult, cfmMbuf);
                     break;
                  }
               case EGTEVTTSAPCNTRLREQ:
               case EGTEVTTPTCNTRLREQ:
                  {
                     retVal = SGetMsg(eguEvntMsg->region, eguEvntMsg->pool, &cfmMbuf);
                     if(retVal != ROK)
                     {
                        /********************************************************
                         * Memory allocation failed, send alarm to LM and error *
                         *********************************************************/
                        EGU_SND_ALARM_MEM_ALLOC_FAIL(eguEvntMsg->region, eguEvntMsg->pool, dgn);
                        EGU_FREEEVNT(eguEvntMsg);
                        EG_RETVALUE(RFAILED);
                     }/* end of if */
                     tmpInst = pst->dstInst;
                     pst->dstInst = pst->srcInst;
                     pst->srcInst = tmpInst;
                     pst->event = EGTEVTLEGCNTRLCFM;
                     CMCHKPKLOG(cmPkPtr, (PTR)eguEvntMsg, cfmMbuf, EEG107, pst);
                     SPstTsk(pst, cfmMbuf);
                     break;
                  }
               default:
                  break;
            }/*End of if */   
            break;
         }/* End of case ENTEG */

#ifdef SS_MT_CB
#if (defined(LCEGUIEGT) || defined(LWLCEGUIEGT))         
        case ENTEU:
        {
               switch(pst->event)
            {
               /********************************************
                *          UNIT DATA REQUEST                *
                ********************************************/                
               case EVTEGTUDATREQ:
               {
                  retVal = cmUnpkEgtEguDatReq(EgUiEgtEguDatReq, pst, mBuf);
                  break;
               }
            }/* End of Switch */
            break;
       }/* end of case ENTEU */

#endif /* LCEGUIEGT */
#ifdef LCEGLIHIT
       case ENTHI:
       {
            switch(pst->event)
            {
               /********************************************
                *         UDP DATA INDICATION               *
                 ********************************************/                
                case EVTHITUDATIND:         
                {
                  retVal = cmUnpkHitUDatInd(EgLiHitUDatInd, pst, mBuf);
                  break;
                } 
      default:
        break;
              }/* End of switch */

            break;
        }/* end of case ENTHI */
#endif /* LCEGLIHIT */
#endif /* SS_MT_CB */
         

         /********************************************
          *      Should hopefully never get here      *
          ********************************************/                
      default:
         {
            EG_FREEMBUF(mBuf);
            EGLOGDBGERR( EEG108, pst->event,
                  "egWrkrActvTsk: Invalid event");
            retVal = RFAILED;
            break;
         }
   } /* END of FIRST switch */

   SExitTsk();
   EG_RETVALUE(retVal);
}

#endif /* EGTP_U_MULTI_THREADED */

/********************************************************************30**

        End of file:     pg_ex_ms.c@@/main/3 - Fri Feb  4 18:18:15 2011

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
/main/1      ---         ad              1. Created for Initial Release 1.1
/main/2      ---         pmacharla       1. Initial for eGTP 1.2 Release
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss               2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3     eg001.201   asaurabh        1. Updating the procedure arguments 
/main/3     eg004.201   magnihotri        1. Header files added for eGTP-C PSF
                                          2. Added for eGTP-C PSF Upgrade:PSF Active task 
                                             handler to handle PSF event
                                          3. Added for eGTP-C PSF Upgrade:PSF Event PACK/UNPACK followed 
                                             by invoking respective
                                             event hadlers
/main/3     eg014.201    rss                1. Egtpc release 10 upgrade
*********************************************************************91*/
