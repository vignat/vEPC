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

     Name:    MME

     Type:    C Source file

     Desc:    

     File:    vb_mme_ex_ms.c

     Sid:      vb_mme_ex_ms.c@@/main/1 - Mon Jun 28 19:35:39 2010

     Prg:     ap
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
#include "cm_pasn.h"       /* Common ASN.1 library           */
#include "sct.h"           /* SCT interface defines           */

#include "egt.h"           /* EG Upper Interface */
#include "leg.h"           /* EG LM Interface */

#include "szt.h"           /* S1AP Upper Interface */
#include "lsz.h"           /* S1AP LM Interface */
#include "szt_asn.h"       /* S1AP ASN */

#include "lvb.h"           /* CNE Layer management           */
#include "vb_hss.h"        /* CNE Application defines        */
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.h"
#endif /* VB_MME_NAS_SEC */
#include "vb.h"            /* CNE Application defines        */
#ifdef VB_MME_AUTH
#include "vb_hss_auth.h"
#endif /* VB_MME_AUTH */
#include "cm_emm.h"        /* CNE Application defines        */
#include "cm_esm.h"        /* CNE Application defines        */
#include "vb_err.h"

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
#include "cm_pasn.x"       /* Common ASN.1 Library           */
#include "sct.x"           /* SCT interface defines           */
#include "egt.x"           /* EG Upper Interface */
#include "leg.x"           /* EG LM Interface */
/* vb007.101 : s1ap include files order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP Upper Interface */
#include "lsz.x"           /* S1AP LM Interface */

#include "lvb.x"           /* CNE Layer management           */
#include "cm_esm.x"        /* CNE Application structures     */
#include "cm_emm.x"        /* CNE Application structures     */
#include "cm_emm_esm.x"
#ifdef VB_MME_NAS_SEC
#include "vb_mme_sec.x"
#include "vb_hss_auth.x"
#endif /* VB_MME_NAS_SEC */
#include "vb_hss_common.x" /* CNE Application structures     */
#include "vb.x"            /* CNE Application structures     */



/*
 *
 *       Fun:    vbMmeActvTsk - MME Activation
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   vb_mme_ex_ms.c
 *
 */

#ifdef ANSI
PUBLIC S16 vbMmeActvTsk
(
Pst      *pst,
Buffer   *mBuf
)
#else
PUBLIC S16 vbMmeActvTsk(pst, mBuf)
Pst      *pst;
Buffer   *mBuf;
#endif /* ANSI */
{
   Mem sMem;

   TRC3(vbMmeActvTsk)
   if (pst == (Pst *)NULLP)
   {
      if (mBuf != (Buffer *)NULLP)
      {
         (Void)SPutMsg(mBuf);
      }
      RETVALUE(RFAILED);
   }
   if (mBuf == (Buffer *)NULLP)
   {
      VB_MME_LOGERROR(ERRCLS_DEBUG, EVB102, ERRZERO, 
         "Null mBuf in vbMmeActvTsk\n");
      RETVALUE(RFAILED);
   }

   sMem.region = vbMmeCb.init.region;
   sMem.pool = vbMmeCb.init.pool;

   switch (pst->srcEnt)
   {
      /* Event from Stack Manager */
      case ENTSM:
      {
         switch(pst->event)
         {
#if ((defined(LCVBMILVB)) || (defined(LWCVBMILVB)))
            case EVTLVBCFGREQ:
            {
               /* Process a config. request */
               cmUnpkLvbCfgReq(VbMiLvbCfgReq, pst, mBuf);
               break;
            }
            case EVTLVBCNTRLREQ:
            {  
               /* Process a control request */
               cmUnpkLvbCntrlReq(VbMiLvbCntrlReq, pst, mBuf);
               break;
            }
            case EVTLVBSTAREQ:
            {
               /* Process a status request  */
               cmUnpkLvbStaReq(VbMiLvbStaReq, pst, mBuf);
               break;
            } 
            case EVTLVBSTSREQ:
            {
               /* Process a statistics request */
               cmUnpkLvbStsReq(VbMiLvbStsReq, pst, mBuf);
               break;
            }
#endif /* ((defined(LCVBMILVB)) || ((defined(LWCVBMILVB)))) */
            default:
            {
               VB_MME_LOGERROR(ERRCLS_DEBUG, EVB022, ERRZERO, 
                      "Invalid event from Layer Manager: vbMmeActvTsk Failed");
               SPutMsg(mBuf);
               break;
            }
         }
         break;
      } /* End of Stack Manager */

      /* Event from S1AP */
      case ENTSZ:
      {
         switch (pst->event)
         {
#if ((defined(LCVBLISZT)) || (defined(LWLCVBLISZT)))
            case EVTSZTBNDCFM:
            {
               (Void)cmUnpkSztBndCfm(UzLiSztBndCfm, pst, mBuf);
               break;
            }
            case EVTSZTAUDCFM:
            {
               (Void)cmUnpkSztAudCfm(UzLiSztAudCfm, pst, mBuf, &sMem); 
               break;
            }
            case EVTSZTSTAIND:
            {
               (Void)cmUnpkSztStaInd(UzLiSztStaInd, pst, mBuf); 
               break;
            }
            case EVTSZTFLCIND:
            {
               (Void)cmUnpkSztFlcInd(UzLiSztFlcInd, pst, mBuf); 
               break;
            }
            case EVTSZTERRIND:
            {
               (Void)cmUnpkSztLclErrInd(UzLiSztErrInd, pst, mBuf); 
                  break;
            }
            case EVTSZTUDATIND:
            {
               (Void)cmUnpkSztUDatInd(UzLiSztUDatInd, pst, mBuf, &sMem);
               break;
            }
            case EVTSZTCONIND:
            {
               (Void)cmUnpkSztConInd(UzLiSztConInd, pst, mBuf, &sMem);
               break;
            }
            case EVTSZTCONCFM:
            {
               (Void)cmUnpkSztConCfm(UzLiSztConCfm, pst, mBuf, &sMem);
               break;
            }
            case EVTSZTRELIND:
            {
               (Void)cmUnpkSztRelInd(UzLiSztRelInd, pst, mBuf, &sMem);
               break;
            }
            case EVTSZTRELCFM:
            {
               (Void)cmUnpkSztRelCfm(UzLiSztRelCfm, pst, mBuf, &sMem);
               break;
            }
            case EVTSZTDATIND:
            {
               (Void)cmUnpkSztDatInd(UzLiSztDatInd, pst, mBuf, &sMem);
               break;
            }
            case EVTSZTENCDECCFM:
            {
               (Void)cmUnpkSztEncDecCfm(UzLiSztEncDecCfm, pst, mBuf, &sMem);
               break;
            }
#endif /* LCVBLISZT */
            default:
            {
               VB_MME_LOGERROR(ERRCLS_DEBUG, EVB023, ERRZERO, 
                  "invalid pst->event in vbMmeActvTsk\n");
               (Void)SPutMsg(mBuf);
               break;
            }
         }
         break;
      } /* End of S1AP */

      case ENTEG: /* EGTP */ 
      {
         switch(pst->event)
         {
#if ((defined(LCEULIEGT)) || ((defined(LWLCEULIEGT))))
#ifdef EGTP_C
            case EVTEGTBNDCFM:
            {
               cmUnpkEgtBndCfm(EuLiEgtBndCfm, pst, mBuf);
               break;
            }
            case EVTEGTSIGIND:
            {
               cmUnpkEgtSigInd(EuLiEgtSigInd, pst, mBuf);
               break;
            }
            case EVTEGTSIGCFM:
            {
               (Void)cmUnpkEgtSigCfm(EuLiEgtSigCfm, pst, mBuf); 
               break;
            }
            case EVTEGTLCLDELCFM:
            {
               cmUnpkEgtLclDelCfm(EuLiEgtLclDelCfm, pst, mBuf);
               break;
            }

            /* Will be used with actual EGTP */
            case EVTEGTSTAIND:
            {
               cmUnpkEgtStaInd(EuLiEgtStaInd, pst, mBuf);
               break;
            }
            case EVTEGTSTACFM:
            {
               cmUnpkEgtStaCfm(EuLiEgtStaCfm, pst, mBuf);
               break;
            }
            case EVTEGTERRIND:
            {
               cmUnpkEgtErrInd(EuLiEgtErrInd, pst, mBuf);
               break;
            }
#endif
#endif
            default:
            {
               VB_MME_LOGERROR(ERRCLS_DEBUG, EVB024,(ErrVal)pst->event, 
                      "Invalid event from E-GTP: vbMmeActvTsk Failed");
            }

            SPutMsg(mBuf);
            break;
         }
         break;
      } /* End of event from S-GW / EGTP */ 


      default:
      {
         VB_MME_LOGERROR(ERRCLS_DEBUG, EVB102, ERRZERO, 
               "invalid pst->srcEnt in vbMmeActvTsk\n");
         (Void)SPutMsg(mBuf);
         break;
      }
   }

   (Void)SExitTsk();
   RETVALUE(ROK);
} /* end of vbMmeActvTsk() */

/********************************************************************30**

         End of file:     vb_mme_ex_ms.c@@/main/1 - Mon Jun 28 19:35:39 2010

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
/main/1      -          sp              1. LTE-CNE Initial Release.
/main/1    vb007.101 akaranth           1. s1ap include files order has changed.
*********************************************************************91*/
