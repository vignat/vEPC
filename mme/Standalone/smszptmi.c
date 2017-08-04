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
  
     Name:     smszptmi.c - Layer Management Interface for S1AP
  
     Type:     C source file
  
     Desc:     C code for SmMiLszXxxReq functions
  
      File:    smszptmi.c
  
     Sid:      smszptmi.c@@/main/nodal_main/akaranth_feat_lpdev_branch/1 - Tue Sep 29 21:39:57 2015
  
     Prg:      pk
  
*********************************************************************21*/
/*

  smszptmi.c - Layer Management Interface for S1AP - SmMiLszXxxReq functions

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
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.h"           /* layer management defines for SZCP */
#include "sz.h"            /* defines and macros for SZ */
#include "szt.h"            /* defines and macros for SZ */
#include "szt_asn.h"
#include "sz_err.h"        /* SZ error defines */

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

#include "smsz_err.h"
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
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* layer management typedefs for SZCP */

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

#include "sz.x"            /* typedefs for SZCP */

/*local defines */
#define SM_MAX_SZ_SEL 4

/*local typedefs */

/*forward references */

PUBLIC S16 PtMiLszCfgReq      ARGS((Pst *post, SzMngmt *cfg));
PUBLIC S16 PtMiLszStsReq      ARGS((Pst *post, Action action, SzMngmt *sts));
PUBLIC S16 PtMiLszStaReq      ARGS((Pst *post, SzMngmt *ssta));
PUBLIC S16 PtMiLszCntrlReq    ARGS((Pst *post, SzMngmt *cntrl));

/* public variable declarations */

/* public variable definitions */

/* private variable definitions */

/* public routines */

/* Following matrices define the mapping between the primitives called by the
 * layer manager (as SmMiLszXXXReq) and the corresponding primitives of the 
 * S1AP layer (layer management service provider) (as SzMiLszXXXReq).
 * Each primitive is mapped to one of SM_MAX_SZ_SEL functions in the array.
 * The mapping is based on post->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCSMSZMILSZ  loose         cmPkMiLszXXXReq
 * 1                 MG          tight         SzMiLszXXXReq
 * 2+                            tight         PtMiLszXXXReq
 */

PRIVATE LszCfgReq smMiLszCfgReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszCfgReq,
#else /* LCSMSZMILSZ */
   PtMiLszCfgReq,
#endif /* LCSMSZMILSZ */

#ifdef SZ
   SzMiLszCfgReq,
#else /* SZ */
   PtMiLszCfgReq,
#endif /* SZ*/

};


PRIVATE LszStsReq smMiLszStsReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszStsReq,
#else /* LCSMSZMILSZ */
   PtMiLszStsReq,
#endif /* LCSMSZMILSZ */

#ifdef SZ
   SzMiLszStsReq,
#else /* SZ */
   PtMiLszStsReq,
#endif /* SZ */

};


PRIVATE LszStaReq smMiLszStaReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszStaReq,
#else /* LCSMSZMILSZ */
   PtMiLszStaReq,
#endif /* LCSMSZMILSZ */

#ifdef SZ
   SzMiLszStaReq,
#else /* SZ */
   PtMiLszStaReq,
#endif /* SZ */

};


PRIVATE LszCntrlReq smMiLszCntrlReqMt[] =
{
#ifdef LCSMSZMILSZ
   cmPkLszCntrlReq,
#else /* LCSMSZMILSZ */
   PtMiLszCntrlReq,
#endif /* LCSMSZMILSZ */

#ifdef SZ
   SzMiLszCntrlReq,
#else /* SZ */
   PtMiLszCntrlReq,
#endif /* SZ */

};


/*
 *
 *       Fun:    SmMiLszCfgReq - Configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCfgReq
(
Pst     *post,       /* post structure */
SzMngmt *cfg        /* management structure */
)
#else /* ANSI */
PUBLIC S16 SmMiLszCfgReq(post, cfg)
Pst     *post;       /* post structure */
SzMngmt *cfg;       /* management structure */
#endif /* ANSI */
{

   TRC3(SmMiLszCfgReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE((*(smMiLszCfgReqMt[post->selector]))(post, cfg));
} /* end of SmMiLszCfgReq() */

/*
 *
 *       Fun:    SmMiLszStsReq - Statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStsReq
(
Pst     *post,       /* post structure */
Action action,
SzMngmt *sts        /* management structure */
)
#else /* ANSI */
PUBLIC S16 SmMiLszStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
SzMngmt *sts;       /* management structure */
#endif /* ANSI */
{
   TRC3(SmMiLszStsReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE((*(smMiLszStsReqMt[post->selector]))(post, action, sts));
} /* end of SmMiLszStsReq() */

/*
 *
 *       Fun:    SmMiLszStaReq - Status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszStaReq
(
Pst     *post,       /* post structure */
SzMngmt *ssta        /* management structure */
)
#else /* ANSI */
PUBLIC S16 SmMiLszStaReq(post, ssta)
Pst     *post;       /* post structure */
SzMngmt *ssta;       /* management structure */
#endif /* ANSI */
{
   TRC3(SmMiLszStaReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE((*(smMiLszStaReqMt[post->selector]))(post, ssta));
} /* end of SmMiLszStaReq() */

/*
 *
 *       Fun:    SmMiLszCntrlReq - Control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 SmMiLszCntrlReq
(
Pst     *post,       /* post structure */
SzMngmt *cntrl        /* management structure */
)
#else /* ANSI */
PUBLIC S16 SmMiLszCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
SzMngmt *cntrl;       /* management structure */
#endif /* ANSI */
{
   TRC3(SmMiLszCntrlReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   if (post == (Pst *) NULLP || post->selector >= SM_MAX_SZ_SEL)
   {
      RETVALUE(RFAILED);
   }
#endif /* ERRCLASS & ERRCLS_DEBUG */
   /*KWORKS Fix*/
   if(post->selector < 2)
   {
   RETVALUE((*(smMiLszCntrlReqMt[post->selector]))(post, cntrl));
   }
   else
   {
      RETVALUE(RFAILED);
   }
} /* end of SmMiLszCntrlReq() */

/*
 *
 *       Fun:    PtMiLszCfgReq - portable configuration request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszCfgReq
(
Pst     *post,       /* post structure */
SzMngmt *cfg        /* management structure */
)
#else /* ANSI */
PUBLIC S16 PtMiLszCfgReq(post, cfg)
Pst     *post;       /* post structure */
SzMngmt *cfg;       /* management structure */
#endif /* ANSI */
{
   TRC3(PtMiLszCfgReq)

   UNUSED(post);
   UNUSED(cfg);
   RETVALUE(RFAILED);
} /* end of PtMiLszCfgReq() */

/*
 *
 *       Fun:    PtMiLszStsReq - portable statistics request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszStsReq
(
Pst     *post,       /* post structure */
Action action,
SzMngmt *sts        /* management structure */
)
#else /* ANSI */
PUBLIC S16 PtMiLszStsReq(post, action, sts)
Pst     *post;       /* post structure */
Action action;
SzMngmt *sts;       /* management structure */
#endif /* ANSI */
{
   TRC3(PtMiLszStsReq)

   UNUSED(post);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(RFAILED);
} /* end of PtMiLszStsReq() */

/*
 *
 *       Fun:    PtMiLszStaReq - portable status request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszStaReq
(
Pst     *post,       /* post structure */
SzMngmt *ssta        /* management structure */
)
#else /* ANSI */
PUBLIC S16 PtMiLszStaReq(post, ssta)
Pst     *post;       /* post structure */
SzMngmt *ssta;       /* management structure */
#endif /* ANSI */
{
   TRC3(PtMiLszStaReq)

   UNUSED(post);
   UNUSED(ssta);
   RETVALUE(RFAILED);
} /* end of PtMiLszStaReq() */

/*
 *
 *       Fun:    PtMiLszCntrlReq - portable control request
 *
 *       Desc:  
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   smszptmi.c
 *
 */

#ifdef ANSI
PUBLIC S16 PtMiLszCntrlReq
(
Pst     *post,       /* post structure */
SzMngmt *cntrl        /* management structure */
)
#else /* ANSI */
PUBLIC S16 PtMiLszCntrlReq(post, cntrl)
Pst     *post;       /* post structure */
SzMngmt *cntrl;       /* management structure */
#endif /* ANSI */
{
   TRC3(PtMiLszCntrlReq)

   UNUSED(post);
   UNUSED(cntrl);
   RETVALUE(RFAILED);
} /* end of PtMiLszCntrlReq() */

/********************************************************************30**
  
         End of file:     smszptmi.c@@/main/nodal_main/akaranth_feat_lpdev_branch/1 - Tue Sep 29 21:39:57 2015
  
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
/main/5      ---      va         1. Updated for S1AP release 4.1
*********************************************************************91*/
