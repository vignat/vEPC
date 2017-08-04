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
  
     Name:     TCP UDP Convergence Layer - service user file
  
     Type:     C source file
  
     Desc:     Code for Upper Interface and Management Interface 
               primitives supplied by TRILLIUM
              
     File:     l5_ex_ms.c
  
     Sid:      l5_ex_ms.c@@/main/5 - Mon Mar  3 20:09:35 2008

     Prg:      asa
  
*********************************************************************21*/

/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */

/* external headers */
#ifdef HI_TLS
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>
#endif

#include "cm_inet.h"       /* common sockets */
#include "cm_tpt.h"        /* common transport defines */
#include "lhi.h"           /* layer management, TUCL  */
#include "hit.h"           /* upper interface */
#ifdef FTHA
#include "sht.h"
#endif
#include "hi_acc.h"        /* acceptance defines */
  
/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm_inet.x"       /* common sockets */
#include "cm_lib.x"        /* common library functions */
#include "cm_tpt.x"        /* common transport typedefs */
#include "lhi.x"           /* layer management TUCL */
#ifdef HI_LKSCTP
#include "sct.h" 
#include "sct.x"
#endif
#include "hit.x"           /* upper interface */
#ifdef FTHA
#include "sht.x"
#endif
#include "hi_acc.x"        /* acceptance typedefs */
#ifdef HI_LKSCTP
#include "sct.h"    
#include "sct.x"   
#endif

/* local defines, if any */

/* local typedefs, if any */
  
/* local externs, if any */
/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from Upper Layer
*
*       Ret:    ROK
*
*       Notes:  None
*
*       File:   l5_ex_ms.c
*
*/
#ifdef ANSI
PUBLIC S16 hcActvTsk
(
Pst       *pst,             /* post structure */
Buffer    *mBuf             /* message buffer */
)
#else
PUBLIC S16 hcActvTsk(pst, mBuf)
Pst       *pst;             /* post structure */
Buffer    *mBuf;            /* message buffer */
#endif
{
   S16   ret = ROK;
#ifdef IPV6_OPTS_SUPPORTED   
   Mem memInfo;             /* meminfo for cmUnpkCmIphdrParm() */
#endif /* IPV6_OPTS_SUPPORTED */

   TRC3(hcActvTsk)

   switch(pst->event)
   {
#ifdef LCHCLIHIT
      case EVTHITCONIND:             /* Connect indication */
         ret = cmUnpkHitConInd(HcLiHitConInd, pst, mBuf);
         break;

      case EVTHITCONCFM:             /* Connect confirm*/
         ret = cmUnpkHitConCfm(HcLiHitConCfm, pst, mBuf);
         break;

      case EVTHITBNDCFM:             /* Bind  confirm*/
         ret = cmUnpkHitBndCfm(HcLiHitBndCfm, pst, mBuf);
         break;

      case EVTHITDATIND:             /* TCP Data indication */
         ret = cmUnpkHitDatInd(HcLiHitDatInd, pst, mBuf);
         break;

      case EVTHITUDATIND:            /* UDP Data indication */         
#ifdef IPV6_OPTS_SUPPORTED         
              memInfo.region = hiAccCb.init.region;  
         memInfo.pool = hiAccCb.init.pool;
         ret = cmUnpkHitUDatInd(HcLiHitUDatInd, pst, mBuf, &memInfo);
#else
         ret = cmUnpkHitUDatInd(HcLiHitUDatInd, pst, mBuf);
#endif /* IPV6_OPTS_SUPPORTED */        
         break;

      case EVTHITDISCIND:            /* Disconnect indication */
         ret = cmUnpkHitDiscInd(HcLiHitDiscInd, pst, mBuf);
         break;

      case EVTHITDISCCFM:            /* Disconnect confirm */
         ret = cmUnpkHitDiscCfm(HcLiHitDiscCfm, pst, mBuf);
         break;


      case EVTHITFLCIND:             /* Flow Control indication */
         ret = cmUnpkHitFlcInd(HcLiHitFlcInd, pst, mBuf);
         break;

#ifdef HI_LKSCTP
      case SCT_EVTBNDCFM:
         ret = cmUnpkSctBndCfm(HcLiSctBndCfm, pst, mBuf);
         break;

      case SCT_EVTENDPOPENCFM:
         ret = cmUnpkSctEndpOpenCfm(HcLiSctEndpOpenCfm, pst, mBuf);
         break;

      case SCT_EVTENDPCLOSECFM:
         ret = cmUnpkSctEndpCloseCfm(HcLiSctEndpCloseCfm, pst, mBuf);
         break;

      case SCT_EVTASSOCCFM:
         ret = cmUnpkSctAssocCfm(HcLiSctAssocCfm, pst, mBuf);
         break;

      case SCT_EVTASSOCIND:
         ret = cmUnpkSctAssocInd(HcLiSctAssocInd, pst, mBuf);
         break;

      case SCT_EVTDATIND:
         ret = cmUnpkSctDatInd(HcLiSctDatInd, pst, mBuf);
         break;

      case SCT_EVTTERMCFM:
         ret = cmUnpkSctTermCfm(HcLiSctTermCfm, pst, mBuf);
         break;

      case SCT_EVTTERMIND:
         ret = cmUnpkSctTermInd(HcLiSctTermInd, pst, mBuf);
         break;

      case SCT_EVTSETPRICFM:
         ret = cmUnpkSctSetPriCfm(HcLiSctSetPriCfm, pst, mBuf);
         break;

      case SCT_EVTHBEATCFM:
         ret = cmUnpkSctHBeatCfm(HcLiSctHBeatCfm, pst, mBuf);
         break;

      case SCT_EVTSTACFM:
         ret = cmUnpkSctStaCfm(HcLiSctStaCfm, pst, mBuf);
         break;

      case SCT_EVTSTAIND:
         ret = cmUnpkSctStaInd(HcLiSctStaInd, pst, mBuf);
         break;

      case SCT_EVTFLCIND:
         ret = cmUnpkSctFlcInd(HcLiSctFlcInd, pst, mBuf);
         break;
#endif

#endif
      default:
      printf("\nKSCTP ACC......evt %d\n", pst->event);
#if (ERRCLASS & ERRCLS_DEBUG)
        SLogError(pst->dstEnt, pst->dstInst, pst->dstProcId, __FILE__, \
                   __LINE__, (ErrCls) ERRCLS_DEBUG, (ErrVal)EHC025, 
                   (ErrVal)pst->event, "hcActvTsk () Failed"); 
#endif
     SPutMsg(mBuf);
     break;
   }

   SExitTsk();

   RETVALUE(ret);
} /* end of hcActvTsk */


/********************************************************************30**
 
         End of file:     l5_ex_ms.c@@/main/5 - Mon Mar  3 20:09:35 2008

*********************************************************************31*/
 
/********************************************************************40**
 
        Notes:
 
*********************************************************************41*/
 
/********************************************************************50**
 
*********************************************************************51*/
 
/********************************************************************60**
 
        Revision history:
 
*********************************************************************61*/
/********************************************************************70**
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat     init                  description
----------- --------- ---- -----------------------------------------------
1.1         ---       asa  1. initial release.
/main/2     ---       cvp  1. changed the copyright header.
/main/3     ---       cvp  1. changed the copyright header.
/main/3+    hi009.104 mmh  1. included sht.h and sht.x for Rolling Upgrade.
/main/4      ---       kp   1. Updated for release 1.5.
/main/5      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
