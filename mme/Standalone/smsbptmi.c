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

     Name:     smsbptmi.c - SCTP layer portable management interface

     Type:     C source file

     Desc:     C source code for the SCTP layer layer management
               service user primitives used in loosely coupled
               systems.

     File:     smsbptmi.c

     Sid:      smsbptmi.c@@/main/4 - Thu Jul 26 07:54:06 2007

     Prg:      mrw

*********************************************************************21*/

/*

  smsbptmi.c -

Following functions are provided in this file:
*/

/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000163                      SCTP layer
 */

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_hash.h"       /* common hashing */
#include "cm_llist.h"      /* common linked list */
#include "cm5.h"           /* common timer */
#include "cm_inet.h"       /* common network address */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"        /* Common DNS library */
#ifdef SB_FTHA
#include "sht.h"           /* SHT Interface header file */
#endif /* SB_FTHA */
#include "lsb.h"           /* layer management, SCTP  */
#include "sct.h"           /* SCT interface */
#include "hit.h"           /* HIT interface */
#include "sb.h"            /* SCTP internal defines */
#include "sb_err.h"        /* SCTP error */
#ifdef SB_ACC
#include "sb_acc.h"
#endif /*SB_ACC*/


/* header/extern include files (.x) */

#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_hash.x"       /* common hashing */
#include "cm_llist.x"      /* common linked list */
#include "cm_lib.x"        /* common library */
#include "cm5.x"           /* common timer */
#include "cm_inet.x"       /* common network address */
#include "cm_tpt.x"        /* common transport address */
#include "cm_dns.x"        /* Common DNS library */
#ifdef SB_FTHA
#include "sht.x"           /* SHT Interface header file */
#endif /* SB_FTHA */
#include "sct.x"           /* SCT interface */
#include "lsb.x"           /* layer management SCTP */
#include "hit.x"           /* HIT interface */
#ifdef SB_ACC
#include "sb_acc.x"        /* typedefs for the SCTP test controller */
#endif /*SB_ACC*/



/* local defines */

#define MAXSBMI                2

#ifndef LCSMSBMILSB
#define PTSMSBMILSB
#else
#ifndef SB
#define PTSMSBMILSB
#else
#endif
#endif

/* sb022.103: C++ compilation error fix */
/* local typedefs */

/* local externs */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* forward references */
#ifdef PTSMSBMILSB
PRIVATE S16 PtMiLsbCfgReq   ARGS((Pst *pst, SbMgmt *cfg ));
PRIVATE S16 PtMiLsbStaReq   ARGS((Pst *pst, SbMgmt *sta ));
PRIVATE S16 PtMiLsbStsReq   ARGS((Pst *pst, Action action, SbMgmt *sts ));
PRIVATE S16 PtMiLsbCntrlReq ARGS((Pst *pst, SbMgmt *cntrl ));
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/* functions in other modules */

/* public variable declarations */

/* private variable declarations */


/*
The following matrices define the mapping between the primitives
called by the layer management interface of SCTP layer and the corresponding
primitives in the driver.

The parameter MAXSBMI defines the maximum number of layer manager entities
on top of the driver. There is an array of functions per primitive insbked by
the driver. Every array is MAXSBMI long (i.e. there are as many functions as the
number of service users).

The dispatching is performed by the configurable variable: selector.
The selector is configured during general configuration.

The selectors are:

   0 - loosely coupled (#define LCSMSBMILSB)
   1 - Lsb (#define SB)

*/

/* Configuration request primitive */

PRIVATE LsbCfgReq SmMiLsbCfgReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbCfgReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */

PRIVATE LsbStsReq SmMiLsbStsReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbStsReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbStsReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */

PRIVATE LsbStaReq SmMiLsbStaReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbStaReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */

PRIVATE LsbCntrlReq SmMiLsbCntrlReqMt[MAXSBMI] =
{
#ifdef LCSMSBMILSB
   cmPkLsbCntrlReq,        /* 0 - loosely coupled - bc */
#else
   PtMiLsbCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SB
   SbMiLsbCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLsbCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/*
*     layer management interface functions
*/

/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbCfgReq
(
Pst *pst,                 /* post structure */
SbMgmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLsbCfgReq(pst, cfg)
Pst *pst;                 /* post structure */
SbMgmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLsbCfgReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbCfgReqMt[pst->selector])(pst, cfg);
   RETVALUE(ROK);
} /* end of SmMiLsbCfgReq */

/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a status request to SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbStaReq
(
Pst *pst,                 /* post structure */
SbMgmt *sta              /* status */
)
#else
PUBLIC S16 SmMiLsbStaReq(pst, sta)
Pst *pst;                 /* post structure */
SbMgmt *sta;             /* status */
#endif
{
   TRC3(SmMiLsbStaReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbStaReqMt[pst->selector])(pst, sta);
   RETVALUE(ROK);
} /* end of SmMiLsbStaReq */

/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to request statistics from SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbStsReq
(
Pst *pst,                 /* post structure */
Action action,
SbMgmt *sts              /* statistics */
)
#else
PUBLIC S16 SmMiLsbStsReq(pst, action, sts)
Pst *pst;                 /* post structure */
Action action;
SbMgmt *sts;             /* statistics */
#endif
{
   TRC3(SmMiLsbStsReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbStsReqMt[pst->selector])(pst, action, sts);
   RETVALUE(ROK);
} /* end of SmMiLsbStsReq */

/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to send control request to SCTP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLsbCntrlReq
(
Pst *pst,                 /* post structure */
SbMgmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLsbCntrlReq(pst, cntrl)
Pst *pst;                 /* post structure */
SbMgmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLsbCntrlReq)
   /* jump to specific primitive depending on configured selector */
   (*SmMiLsbCntrlReqMt[pst->selector])(pst, cntrl);
   RETVALUE(ROK);
} /* end of SmMiLsbCntrlReq */


#ifdef PTSMSBMILSB
/*
*
*       Fun:   Portable configure Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbCfgReq
(
Pst *pst,                   /* post structure */
SbMgmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLsbCfgReq(pst, cfg)
Pst *pst;                   /* post structure */
SbMgmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLsbCfgReq);
   SBLOGINVSEL;
   UNUSED(pst);
   UNUSED(cfg);
   RETVALUE(ROK);
} /* end of PtMiLsbCfgReq */


/*
*
*       Fun:   Portable status Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbStaReq
(
Pst *pst,                   /* post structure */
SbMgmt *sta                /* status */
)
#else
PRIVATE S16 PtMiLsbStaReq(pst, sta)
Pst *pst;                   /* post structure */
SbMgmt *sta;               /* status */
#endif
{
   TRC3(PtMiLsbStaReq);
   SBLOGINVSEL;
   UNUSED(pst);
   UNUSED(sta);
   RETVALUE(ROK);
} /* end of PtMiLsbStaReq */


/*
*
*       Fun:   Portable statistics Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbStsReq
(
Pst *pst,                   /* post structure */
Action action,
SbMgmt *sts                /* statistics */
)
#else
PRIVATE S16 PtMiLsbStsReq(pst, action, sts)
Pst *pst;                   /* post structure */
Action action;
SbMgmt *sts;               /* statistics */
#endif
{
   TRC3(PtMiLsbStsReq);
   SBLOGINVSEL;
   UNUSED(pst);
   UNUSED(action);
   UNUSED(sts);
   RETVALUE(ROK);
} /* end of PtMiLsbStsReq */


/*
*
*       Fun:   Portable control Request SCTP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smsbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLsbCntrlReq
(
Pst *pst,                   /* post structure */
SbMgmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLsbCntrlReq(pst, cntrl)
Pst *pst;                   /* post structure */
SbMgmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLsbCntrlReq);
   SBLOGINVSEL;
   UNUSED(pst);
   UNUSED(cntrl);
   RETVALUE(ROK);
} /* end of PtMiLsbCntrlReq */

#endif

/********************************************************************30**

         End of file:     smsbptmi.c@@/main/4 - Thu Jul 26 07:54:06 2007

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
1.1          ---      mrw  1. Initial version
/main/3      ---      sb   1. Modified for SCTP release based on 
                              RFC-2960 'Oct 2000
          sb042.102   hl   1. Added change for SHT interface and Rolling
                              UpGrade
/main/4      ---   rsr/ag  1. Updated for Release of 1.3
/main/4   sb022.103 pkaX   1. C++ compilation error fix.
*********************************************************************91*/
