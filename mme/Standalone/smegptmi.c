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

     Name:     EGTP layer portable management interface

     Type:     C source file

     Desc:     C source code for the EGTP layer layer management
               service user primitives used in loosely coupled
               systems.

     File:     smegptmi.c

     Sid:      smegptmi.c@@/main/3 - Fri Feb  4 18:18:29 2011

     Prg:      ad

*********************************************************************21*/


/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm_tpt.h"        /* common transport */
#include "cm_tkns.h"       /* common tokens */
#include "leg.h"           /* EGTP layer manager interface */
#include "eg_err.h"        /* EGTP error defines */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport */
#include "cm_mblk.x"       /* cm_mblk */
#include "leg.x"           /* EGTP layer manager interface */

/* local defines */
#define SM_MAX_MILEG_SEL              2

#ifndef LCSMEGMILEG
#define PTSMEGMILEG
#else
#ifndef SM
#define PTSMEGMILEG
#else
#endif
#endif
#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/*-- eg003.201 : changes from eg008.102 : Adding funcition fwd references --*/
PUBLIC S16 SmMiLegCfgReq   ARGS(( Pst     *pst,
                                           EgMngmt *cfg ));

PUBLIC S16 SmMiLegCntrlReq ARGS(( Pst     *pst,
                                           EgMngmt *cntrl ));

PUBLIC S16 SmMiLegStaReq   ARGS(( Pst     *pst,
                                           EgMngmt *sta ));

PUBLIC S16 SmMiLegStsReq   ARGS(( Pst     *pst,
                                           Action  action,
                                           EgMngmt *sts ));


/* forward references */
#ifdef PTSMEGMILEG
PRIVATE S16 PtMiLegCfgReq     ARGS((Pst             *pst,
                                    EgMngmt         *cfg));
PRIVATE S16 PtMiLegCntrlReq   ARGS((Pst             *pst,
                                    EgMngmt         *cntrl));
PRIVATE S16 PtMiLegStaReq     ARGS((Pst             *pst,
                                    EgMngmt         *sta));
PRIVATE S16 PtMiLegStsReq     ARGS((Pst             *pst,
                                    Action          action,
                                    EgMngmt         *sts));

#endif /* PTSMEGMILEG */

/* Configuration request primitive */
PRIVATE LegCfgReq SmMiLegCfgReqMt[SM_MAX_MILEG_SEL] =
{
#ifdef LCSMEGMILEG
   cmPkLegCfgReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLegCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SM
   EgMiLegCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLegCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */
PRIVATE LegCntrlReq SmMiLegCntrlReqMt[SM_MAX_MILEG_SEL] =
{
#ifdef LCSMEGMILEG
   cmPkLegCntrlReq,        /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLegCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SM
   EgMiLegCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLegCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */
PRIVATE LegStaReq SmMiLegStaReqMt[SM_MAX_MILEG_SEL] =
{
#ifdef LCSMEGMILEG
   cmPkLegStaReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLegStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SM
   EgMiLegStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLegStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */
PRIVATE LegStsReq SmMiLegStsReqMt[SM_MAX_MILEG_SEL] =
{
#ifdef LCSMEGMILEG
   cmPkLegStsReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLegStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef SM
   EgMiLegStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLegStsReq,          /* 1 - tightly coupled, portable */
#endif
};



/* Primitive Mapping Dispatching Functions */

/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure SIP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegCfgReq
(
Pst     *pst,             /* post structure */
EgMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLegCfgReq(pst, cfg)
Pst     *pst;             /* post structure */
EgMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLegCfgReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLegCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);
} /* SmMiLegCfgReq */

/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to control SIP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegCntrlReq
(
Pst     *pst,             /* post structure */
EgMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLegCntrlReq(pst, cntrl)
Pst     *pst;             /* post structure */
EgMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLegCntrlReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLegCntrlReqMt[pst->selector])(pst, cntrl);

   RETVALUE(ROK);
} /* SmMiLegCntrlReq */

/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a Status Request to SIP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegStaReq
(
Pst     *pst,             /* post structure */
EgMngmt *sta              /* solicited status */
)
#else
PUBLIC S16 SmMiLegStaReq(pst, sta)
Pst     *pst;             /* post structure */
EgMngmt *sta;             /* solicited status */
#endif
{
   TRC3(SmMiLegStaReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLegStaReqMt[pst->selector])(pst, sta);

   RETVALUE(ROK);
} /* SmMiLegStaReq */

/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to send a Statistics request to SIP layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLegStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* zeroing action */
EgMngmt *sts            /* statistics */
)
#else
PUBLIC S16 SmMiLegStsReq(pst, action, sts)
Pst     *pst;           /* post structure */
Action  action;         /* zeroing action */
EgMngmt *sts;           /* statistics */
#endif
{
   TRC3(SmMiLegStsReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLegStsReqMt[pst->selector])(pst, action, sts);

   RETVALUE(ROK);
} /* SmMiLegStsReq */



#ifdef PTSMEGMILEG
/* Portable Stub Functions */

/*
*
*       Fun:   Portable configure Request for SIP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLegCfgReq
(
Pst     *pst,               /* post structure */
EgMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLegCfgReq(pst, cfg)
Pst     *pst;               /* post structure */
EgMngmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLegCfgReq);

   EGLOGINVSEL;
   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);
} /* PtMiLegCfgReq */

/*
*
*       Fun:   Portable Control Request for SIP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLegCntrlReq
(
Pst     *pst,               /* post structure */
EgMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLegCntrlReq(pst, cntrl)
Pst     *pst;               /* post structure */
EgMngmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLegCntrlReq);

   EGLOGINVSEL;
   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);
} /* PtMiLegCntrlReq */

/*
*
*       Fun:   Portable Status Request for SIP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLegStaReq
(
Pst     *pst,               /* post structure */
EgMngmt *sta                /* solicited status */
)
#else
PRIVATE S16 PtMiLegStaReq(pst, sta)
Pst     *pst;               /* post structure */
EgMngmt *sta;               /* solicited status */
#endif
{
   TRC3(PtMiLegStaReq);

   EGLOGINVSEL;
   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* PtMiLegStaReq */

/*
*
*       Fun:   Portable Statistics Request for SIP layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smegptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLegStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* zeroing action */
EgMngmt *sts            /* statistics */
)
#else
PRIVATE S16 PtMiLegStsReq(pst, action, sts)
Pst     *pst;               /* post structure */
Action  action;             /* zeroing action */
EgMngmt *sts;               /* statistics */
#endif
{
   TRC3(PtMiLegStsReq);

   EGLOGINVSEL;
   UNUSED(pst);
   UNUSED(action);
   UNUSED(sts);

   RETVALUE(ROK);
} /* PtMiLegStsReq */


#endif /* PTSMEGMILEG */
#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/********************************************************************30**

         End of file:     smegptmi.c@@/main/3 - Fri Feb  4 18:18:29 2011

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
/main/1      ----      ad         1. Initial Rel EGTP 1.1
/main/2      ---      akulkarni          1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3    eg003.201  psingh             1. Merged code from eg008.102 to
                                            add the function prototypes
*********************************************************************91*/
