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
  
     Name:     smszexms.c - Layer manager for S1AP - external interface for loose
               coupling
  
     Type:     C source file
  
     Desc:     C code for smSzActvTsk - layer manager activation function for
               S1AP
  
     File:     smszexms.c
  
     Sid:      smszexms.c@@/main/nodal_main/1 - Fri Nov 22 15:56:20 2013
  
     Prg:      pk
  
*********************************************************************21*/

/*

  smszexms.c - external interface for loose coupling in MG->SM direction

Following functions are provided in this file:
         prUnpkMiLprCfgReq    - Unpack configuration request
*/

/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000XXX                      S1AP v 1.1
 */

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"         /* common DNS libraru defines */

#include "sct.h"           /* SB (SCTP) layer */
#ifdef SZ_FTHA            
#include "sht.h"           /* SHT Interface header file */
#endif /* SZ_FTHA */
#include "lsz.h"           /* layer management defines for SZCP */
#include "sz.h"            /* defines and macros for SZ */
#include "szt.h"            /* defines and macros for SZ */
#include "szt_asn.h"
#include "sz_err.h"        /* SZ error defines */
#ifdef SZ_ACC
#include "szac_acc.h"        /* defines for SZ acceptance tests */
#endif /* SZ_ACC */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif /* HI */

#ifdef    SB
#include "lsb.h"
#include "sb_mtu.h"
#include "sb.h"
#endif    /* SB */

#include "cm_pasn.h"

#ifdef SZ_ACC
#include "cm_xta.h" 
#endif /* SZ_ACC */
#ifdef SZ_FTHA
#include "cm_ftha.h"        /* common ftha library          */
#include "cm_psfft.h"       /* common psf fast trans library*/
#include "cm_pasn.h"        /* common pasn library          */
#include "cm_psf.h"         /* common psf library           */
#endif /* SZ_FTHA */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif
/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"         /* common DNS libraru defines */
#ifdef SZ_FTHA
#include "cm_ftha.x"       /* common ftha library          */
#include "cm_psfft.x"      /* common psf fast trans library*/
#include "cm_pasn.x"       /* common pasn library          */
#include "cm_psf.x"        /* common pdf library           */
#endif /* SZ_FTHA */
 
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCTP Interface Structures */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* SZT Interface Structures */
#ifdef SZ_FTHA            
#include "sht.x"           /* SHT Interface header file */
#endif /* SZ_FTHA */
#include "lsz.x"           /* layer management typedefs for SZCP */
#ifdef SZ_FTHA
#include "lyt.x"
#endif /* SZ_FTHA */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif /* HI */

#ifdef    SB
#include "lsb.x"
#include "sct.x"           /* SB (SCTP) layer */
#include "sb_mtu.x"
#include "sb.x"
#endif    /* SB */
#include "cm_pasn.x"

#ifdef SZ_ACC
#include "cm_xta.x" 
#include "sz.x"            /* typedefs for SZCP */
#include "szac_acc.x"        /* typedefs for SZCP acceptance tests */
#include "szac_lsz.x"
#endif /* SZ_ACC */

/* local defines */

/* local typedefs */

/* forward references */

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */
/*
 *
 *       Fun:    smSzActvTsk - SM task activation for MG->SM
 *
 *       Desc:   calls SmMiLszXXXCfm/Ind based on post->event
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszexms.c
 *
 */

#ifdef ANSI
PUBLIC S16 smSzActvTsk
(
Pst      *post,
Buffer   *mBuf
)
#else /* ANSI */
PUBLIC S16 smSzActvTsk(post, mBuf)
Pst      *post;
Buffer   *mBuf;
#endif /* ANSI */
{
   TRC3(smSzActvTsk)
   if (post == (Pst *)NULLP)
   {
      SZ_FREE_BUF(mBuf)
      RETVALUE(RFAILED);
   }
   switch (post->event)
   {
#ifdef LCSMSZMILSZ
      case EVTLSZSTAIND:             /* Status Indication */
      {
         (Void)cmUnpkLszStaInd(SmMiLszStaInd, post, mBuf);
         break;
      }
      case EVTLSZTRCIND:
      {
         (Void)cmUnpkLszTrcInd(SmMiLszTrcInd, post, mBuf);
         break;
      }
      case EVTLSZCFGCFM:
      {
         (Void)cmUnpkLszCfgCfm(SmMiLszCfgCfm, post, mBuf);
         break;
      }
      case EVTLSZCNTRLCFM:
      {
         (Void)cmUnpkLszCntrlCfm(SmMiLszCntrlCfm, post, mBuf);
         break;
      }
      case EVTLSZSTACFM:
      {
         (Void)cmUnpkLszStaCfm(SmMiLszStaCfm, post, mBuf);
         break;
      }
      case EVTLSZSTSCFM:
      {
         (Void)cmUnpkLszStsCfm(SmMiLszStsCfm, post, mBuf);
         break;
      }
#endif /* LCSMSZMILSZ */
      default:
      {
#ifdef YT
         smYtActvTsk(post, mBuf);
#endif /* YT */
         break;
      }
   }
   SExitTsk();
   RETVALUE(ROK);
} /* end of smSzActvTsk() */

/********************************************************************30**
  
         End of file:     smszexms.c@@/main/nodal_main/1 - Fri Nov 22 15:56:20 2013
  
*********************************************************************31*/

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
----------- -------- ---- -----------------------------------------------
/main/4      ---     va     1. Updated for S1AP release 4.1 
*********************************************************************91*/
