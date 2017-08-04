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
  
     Name:     Stack Manager - portable TUCL
  
     Type:     C source file
  
     Desc:     Code for TCP UDP Convergence Layer  layer management
               primitives. 
              
     File:     smhiptmi.c
  
     Sid:      smhiptmi.c@@/main/5 - Mon Mar  3 20:09:32 2008

     Prg:      asa
  
*********************************************************************21*/

/*
  
Layer management provides the necessary functions to control and
monitor the condition of each protocol layer.

The following functions are provided in this file:

     SmMiLhiCfgReq      Configure Request
     SmMiLhiStaReq      Status Request
     SmMiLhiStsReq      Statistics Request
     SmMiLhiCntrlReq    Control Request
   
It is assumed that the following functions are provided in the
stack management body files:

     SmMiLhiStaInd      Status Indication
     SmMiLhiStaCfm      Status Confirm
     SmMiLhiStsCfm      Statistics Confirm
     SmMiLhiTrcInd      Trace Indication
   
*/   


/* header include files (.h) */
  
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general layer */
#include "cm5.h"           /* common timer */
#include "ssi.h"           /* system services */
#include "cm_inet.h"       /* common sockets */
#include "lhi.h"           /* HI layer management */
#include "cm_err.h"        /* common error */
#include "smhi_err.h"      /* HI error */

/* header/extern include files (.x) */
  
#include "gen.x"           /* general layer */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common sockets */
#include "lhi.x"           /* hi layer management */

#define  MAXHIMI  2

#ifndef  LCSMHIMILHI
#define  PTSMHIMILHI
#else
#ifndef   HI
#define  PTSMHIMILHI
#endif
#endif

#ifdef PTSMHIMILHI
PRIVATE S16 PtMiLhiCfgReq    ARGS((Pst *pst, HiMngmt *cfg));
PRIVATE S16 PtMiLhiStsReq    ARGS((Pst *pst, Action action, HiMngmt *sts));
PRIVATE S16 PtMiLhiCntrlReq  ARGS((Pst *pst, HiMngmt *cntrl));
PRIVATE S16 PtMiLhiStaReq    ARGS((Pst *pst, HiMngmt *sta));
#endif


/*
the following matrices define the mapping between the primitives
called by the layer management interface of TCP UDP Convergence Layer
and the corresponding primitives in TUCL
 
The parameter MAXHIMI defines the maximum number of layer manager
entities on top of TUCL . There is an array of functions per primitive
invoked by TCP UDP Conbvergence Layer. Every array is MAXHIMI long
(i.e. there are as many functions as the number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMHIMILHI) 2 - Lhi (#define HI)

*/


/* Configuration request primitive */
 
PRIVATE LhiCfgReq SmMiLhiCfgReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCfgReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCfgReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Control request primitive */
 
PRIVATE LhiCntrlReq SmMiLhiCntrlReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiCntrlReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Statistics request primitive */
 
PRIVATE LhiStsReq SmMiLhiStsReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStsReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStsReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Status request primitive */
 
PRIVATE LhiStaReq SmMiLhiStaReqMt[MAXHIMI] =
{
#ifdef LCSMHIMILHI
   cmPkLhiStaReq,          /* 0 - loosely coupled  */
#else
   PtMiLhiStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef HI
   HiMiLhiStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLhiStaReq,          /* 1 - tightly coupled, portable */
#endif
};


/*
*     layer management interface functions 
*/
 
/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure  TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiCfgReq
(
Pst *spst,                /* post structure */
HiMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLhiCfgReq(spst, cfg)
Pst *spst;                /* post structure */   
HiMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLhiCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCfgReqMt[spst->selector])(spst, cfg); 
   RETVALUE(ROK);
} /* end of SmMiLhiCfgReq */



/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStsReq
(
Pst *spst,                /* post structure */
Action action,
HiMngmt *sts              /* statistics */
)
#else
PUBLIC S16 SmMiLhiStsReq(spst, action, sts)
Pst *spst;                /* post structure */   
Action action;
HiMngmt *sts;             /* statistics */
#endif
{
   TRC3(SmMiLhiStsReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStsReqMt[spst->selector])(spst, action, sts); 
   RETVALUE(ROK);
} /* end of SmMiLhiStsReq */


/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to 
*              TUCL
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiCntrlReq
(
Pst *spst,                 /* post structure */
HiMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLhiCntrlReq(spst, cntrl)
Pst *spst;                 /* post structure */   
HiMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLhiCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiCntrlReqMt[spst->selector])(spst, cntrl); 
   RETVALUE(ROK);
} /* end of SmMiLhiCntrlReq */


/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to 
*              TUCL
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
  
#ifdef ANSI
PUBLIC S16 SmMiLhiStaReq
(
Pst *spst,                /* post structure */
HiMngmt *sta              /* status */
)
#else
PUBLIC S16 SmMiLhiStaReq(spst, sta)
Pst *spst;                /* post structure */   
HiMngmt *sta;             /* status */
#endif
{
   TRC3(SmMiLhiStaReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLhiStaReqMt[spst->selector])(spst, sta); 
   RETVALUE(ROK);
} /* end of SmMiLhiStaReq */

#ifdef PTSMHIMILHI

/*
 *             Portable Functions
 */

/*
*
*       Fun:   Portable configure Request- TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLhiCfgReq
(
Pst *spst,                  /* post structure */
HiMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLhiCfgReq(spst, cfg)
Pst *spst;                  /* post structure */
HiMngmt *cfg;               /* configure */
#endif
{
  TRC3(PtMiLhiCfgReq) 

  UNUSED(spst);
  UNUSED(cfg);

#if (ERRCLASS & ERRCLS_DEBUG)
   SMHILOGERROR(spst->srcEnt, spst->srcInst, spst->srcProcId, \
              ERRCLS_DEBUG, ESMHI002, 0, "PtMiLhiCfgReq () Failed");
#endif

  RETVALUE(ROK);
} /* end of PtMiLhiCfgReq */



/*
*
*       Fun:   Portable statistics Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLhiStsReq
(
Pst *spst,                  /* post structure */
Action action,
HiMngmt *sts                /* statistics */
)
#else
PRIVATE S16 PtMiLhiStsReq(spst, action, sts)
Pst *spst;                  /* post structure */
Action action;
HiMngmt *sts;               /* statistics */
#endif
{
  TRC3(PtMiLhiStsReq) 

  UNUSED(spst);
  UNUSED(action);
  UNUSED(sts);

#if (ERRCLASS & ERRCLS_DEBUG)
   SMHILOGERROR(spst->srcEnt, spst->srcInst, spst->srcProcId, \
                ERRCLS_DEBUG, ESMHI003, 0, "PtMiLhiStsReq () Failed"); 
#endif
  RETVALUE(ROK);
} /* end of PtMiLhiStsReq */


/*
*
*       Fun:   Portable control Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
#ifdef ANSI
PRIVATE S16 PtMiLhiCntrlReq
(
Pst *spst,                  /* post structure */
HiMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLhiCntrlReq(spst, cntrl)
Pst *spst;                  /* post structure */
HiMngmt *cntrl;             /* control */
#endif
{
  TRC3(PtMiLhiCntrlReq)

  UNUSED(spst);
  UNUSED(cntrl);

#if (ERRCLASS & ERRCLS_DEBUG)
   SMHILOGERROR(spst->srcEnt, spst->srcInst, spst->srcProcId, \
             ERRCLS_DEBUG, ESMHI004, 0, "PtMiLhiCntrlReq () Failed");
#endif
  RETVALUE(ROK);
} /* end of PtMiLhiCntrlReq */


/*
*
*       Fun:   Portable status Request TUCL
*
*       Desc:  
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smhiptmi.c
*
*/
  
#ifdef ANSI
PRIVATE S16 PtMiLhiStaReq
(
Pst *spst,                  /* post structure */
HiMngmt *sta                /* status */
)
#else
PRIVATE S16 PtMiLhiStaReq(spst, sta)
Pst *spst;                  /* post structure */
HiMngmt *sta;               /* status */
#endif
{
  TRC3(PtMiLhiStaReq);

  UNUSED(spst);
  UNUSED(sta);

#if (ERRCLASS & ERRCLS_DEBUG)
   SMHILOGERROR(spst->srcEnt, spst->srcInst, spst->srcProcId, \
                ERRCLS_DEBUG,ESMHI005, 0, "PtMiLhiStaReq () Failed");
#endif
  RETVALUE(ROK);
} /* end of PtMiLhiStaReq */

#endif /* PTSMHIMILHI */


/********************************************************************30**
 
         End of file:     smhiptmi.c@@/main/5 - Mon Mar  3 20:09:32 2008

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
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
1.1         ---      asa  1. initial release.
/main/2     ---      cvp  1. changed the copyright header. 
/main/3     ---      cvp  1. changed the copyright header. 
/main/4      ---      kp   1. Updated for release 1.5.
/main/5      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
