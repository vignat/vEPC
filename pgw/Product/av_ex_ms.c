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

     Name:     Trillium LTE SGW Application
  
     Type:     C source file
  
     Desc:     Functions required for scheduling and initialization.
  
     File:     jg_ex_ms.c
  
     Sid:      av_ex_ms.c@@/main/1 - Tue Aug 17 18:54:33 2010
   
     Prg:       sp
  
*********************************************************************21*/


/************************************************************************
 
/* header include files (.h)            */

#include "envopt.h"        /* environment options */


#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm_inet.h"       /* Common Socket Library */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_hash.h"       /* common hash */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm5.h"           /* common timer */
#include "leg.h"           /* EGTP layer manager */
#include "egt.h"           /* EGTP upper interface */

#include "lav.h"
#include "av.h"
#include "av_err.h"

#ifndef AV_PERF
#include <pcap.h>
#endif
/* header/extern include files (.x) */
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* Common Socket Library */
#include "cm5.x"           /* common timer */
#include "cm_hash.x"       /* common hash */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport structures */
#include "leg.x"           /* layer management typedefs for EGTP */
#include "egt.x"           /* EGTP Upper interface */

#include "lav.x"
#include "av.x"

/* local defines                        */
/* local typedefs                       */
/* local externs                        */
/* forward references                   */
/* local function definition            */
/* forward references                   */
/* functions in other modules           */
/* public variable declarations         */
/* private variable declarations        */




/* Set Of Primitives for EGTP Interface */
/* Defines for the primitive events  
#define    EVTEGTBNDREQ             1      Bind Request 
#define    EVTEGTBNDCFM             2      Bind Confirm 
#define    EVTEGTUBNDREQ            3      Unbind Request 
#define    EVTEGTSIGREQ             4      Request for ANY Signalling MSG
#define    EVTEGTSIGIND             5      Indication for ANY Signalling MSG
#define    EVTEGTSIGRSP             6      Response for ANY Signalling MSG
#define    EVTEGTSIGCFM             7      Confirmation for ANY Signalling MSG
#define    EVTEGTSTAREQ             8      Status Request
#define    EVTEGTSTACFM             9      Status Cfm
#define    EVTEGTSTAIND             10     Status Indication
#define    EVTEGTLCLDELCFM          11     Local Tunnel Delete confirm
#define    EVTEGTLCLDELREQ          12     Local Tunnel Delete request
#define    EVTEGTERRIND             13     Error Indication

added event types for eGTP-U 
#define    EVTEGTUDATREQ            14     eGTP-U Data Request
#define    EVTEGTUDATIND            15     eGTP-U Data Indication
#define    EVTEGTULCLTNLMGMTREQ     16     eGTP-U Tunnel Management Request
#define    EVTEGTULCLTNLMGMTCFM     17     eGTP-U Tunnel Management Confirm
#define    EVTEGTUSTAREQ            18     eGTP-U Status Request
#define    EVTEGTUSTACFM            19     eGTP-U Status Confirm
#define    EVTEGTUUSTAIND           20     eGTP-U Unsolicited Status Indication
#define    EVTEGTUERRIND            21     eGTP-U Error Indication

 Status Request/Confirm : eventType(s)
#define    EGT_STATYPE_PATH         1      Path status 
#define    EGT_STATYPE_TUNNEL       2      Tunnel status */


/******************************************************************************
 *
 *       Fun  :  PDN-GW Send Activate task
 *
 *       Desc :  
 *
 *       Ret  :  ROK  - ok
 *
 *       Notes:  None
 *
 *       File :  av_ex_ms.c 
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwSndActvTsk
(
Pst     *pst,                                   /* post structure       */
Buffer  *mBuf                                   /* message buffer       */
)
#else
PUBLIC S16 avPgwSndActvTsk(pst, mBuf)
Pst     *pst;                                   /* post structure       */
Buffer  *mBuf;                                  /* message buffer       */
#endif
{
   TRC3(avPgwSndActvTsk)
  
   switch(pst->srcEnt)
   {
/* Event from Stack Manager */
      case ENTSM:
      {
         switch(pst->event)
         {
#if ((defined(LCAVMILAV)) || (defined(LWCAVMILAV)))
            case EVTLAVCFGREQ:
            {
               /* Process a config request */
               cmUnpkLavCfgReq(AvMiLavCfgReq, pst, mBuf);
               break;
            }
            case EVTLAVCNTRLREQ:
            {  
               /* Process a control request */
               cmUnpkLavCntrlReq(AvMiLavCntrlReq, pst, mBuf);
               break;
            }
            case EVTLAVSTAREQ:
            {
               /* Process a status request  */
               cmUnpkLavStaReq(AvMiLavStaReq, pst, mBuf);
               break;
            } 
            case EVTLAVSTSREQ:
            {
               /* Process a statistics request */
               cmUnpkLavStsReq(AvMiLavStsReq, pst, mBuf);
               break;
            }
#endif /* ((defined(LCAVMILAV)) || ((defined(LWCAVMILAV)))) */
            default:
            {
               AV_PGW_SND_LOGERROR(ERRCLS_DEBUG, EAV002, ERRZERO, 
                   "Invalid event from Layer Manager: avPgwSndActvTsk Failed");
               SPutMsg(mBuf);
               break;
            }
         }
         break;
      } /* End of Stack Manager */
      case ENTEG: /* EGTP */ 
      {
         switch(pst->event)
         {
            case EVTEGTBNDCFM:
               cmUnpkEgtBndCfm(EuLiEgtBndCfm, pst, mBuf);
               break;

            case EVTEGTSIGIND:
               cmUnpkEgtSigInd(EuLiEgtSigInd, pst, mBuf);
               break;

            case EVTEGTSTAIND:
               cmUnpkEgtStaInd(EuLiEgtStaInd, pst, mBuf);
               break;

            case EVTEGTSTACFM:
               cmUnpkEgtStaCfm(EuLiEgtStaCfm, pst, mBuf);
               break;
            case EVTEGTLCLDELCFM:
               cmUnpkEgtLclDelCfm(EuLiEgtLclDelCfm, pst, mBuf);
               break;

            case EVTEGTERRIND:
               cmUnpkEgtErrInd(EuLiEgtErrInd, pst, mBuf);
               break;

            case EVTEGTULCLTNLMGMTCFM:
               cmUnpkEgtEguLclTnlMgmtCfm(EuLiEgtEguLclTnlMgmtCfm, pst, mBuf);
               break;

            case EVTEGTUDATIND:
               cmUnpkEgtEguDatInd(EuLiEgtEguDatInd, pst, mBuf);
               break;

            case EVTEGTUERRIND:
               cmUnpkEgtEguErrInd(EuLiEgtEguErrInd, pst, mBuf);
               break;

            default:
            {
#if (ERRCLASS & ERRCLS_DEBUG)
                SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__,
                 __LINE__, (ErrCls) ERRCLS_DEBUG, EAV003, (ErrVal)pst->event,
                   " Invalid event from E-GTP : avPgwActvTsk () Failed");
#endif
            }

            SPutMsg(mBuf);
            break;
         }
         break;
      }

      default:
      {
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__,
                __LINE__, (ErrCls) ERRCLS_DEBUG, EAV004, (ErrVal)pst->srcEnt,
                " Event Received from Invalid Entity : avPgwActvTsk () Failed");
#endif

         SPutMsg(mBuf);
         break;
      }
   }

   SExitTsk();
   RETVALUE(ROK);
} /* end of avPgwSndActvTsk */





/******************************************************************************
 *
 *       Fun  :  PDN-GW Receive Activate task
 *
 *       Desc :  
 *
 *       Ret  :  ROK  - ok
 *
 *       Notes:  None
 *
 *       File :  av_ex_ms.c 
 *
 */
#ifdef ANSI
PUBLIC S16 avPgwRcvActvTsk
(
Pst     *pst,                                   /* post structure       */
Buffer  *mBuf                                   /* message buffer       */
)
#else
PUBLIC S16 avPgwRcvActvTsk(pst, mBuf)
Pst     *pst;                                   /* post structure       */
Buffer  *mBuf;                                  /* message buffer       */
#endif
{
   TRC3(avPgwRcvActvTsk)
  
   switch(pst->srcEnt)
   {
      case ENTEU: /* Self Posted event */
      {
         switch(pst->event)
         {
            case EVTAVRCVARPREQ:
            {
#ifndef AV_PERF
               SPutMsg(mBuf);

               /* Trigger event to receive packets from external network */
               (Void *)avPgwPktReceiver(NULLP);

               /* We are here - something defintely wrong on pcap adaptor */
 
               AV_PGW_RCV_LOGERROR(ERRCLS_DEBUG,EAV005,0,
                              "Failed to contact external node\n");
               RETVALUE(RFAILED);
#else               
               break;
#endif /* AV_PERF */               
            }
#ifdef AV_PERF
            case EVTAVSENDDLDATREQ:
            {
               SPutMsg(mBuf);
               (Void )avSendEgDLData();       
               /* We are here - something defintely wrong with DL data load
                  generator */
 
               AV_PGW_RCV_LOGERROR(ERRCLS_DEBUG,EAVXXX,0,
                              "Failed to send DL data to S-GW\n");
               RETVALUE(RFAILED);
            }
#endif
            default:
            {
#if (ERRCLASS & ERRCLS_DEBUG)
                SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__,
                __LINE__, (ErrCls) ERRCLS_DEBUG, EAV006, (ErrVal)pst->event,
                        " Invalid event : avPgwRcvActvTsk () Failed");
#endif
               break;
            }

         }
         break;
      }

      default:
      {
#if (ERRCLASS & ERRCLS_DEBUG)
         SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__,
                   __LINE__, (ErrCls) ERRCLS_DEBUG, EAV007, (ErrVal)pst->srcEnt,
                   " Event Received from Invalid Entity : avPgwRcvActvTsk () Failed");
#endif

         break;
      }
   }

   SPutMsg(mBuf);
   SExitTsk();
   RETVALUE(ROK);
} /* end of avPgwRcvActvTsk */

/********************************************************************30**
  
         End of file:     av_ex_ms.c@@/main/1 - Tue Aug 17 18:54:33 2010
   
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
/main/1      ---     sp                1.LTE-CNE:PGW initial release.
*********************************************************************91*/

