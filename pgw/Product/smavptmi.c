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

     Name:     LTE CNE layer portable management interface

     Type:     C source file

     Desc:     C source code for the LTE CNE layer layer management
               service user primitives used in loosely coupled
               systems.

     File:     smavptmi.c

     Sid:      smavptmi.c@@/main/1 - Tue Aug 17 18:54:43 2010

     Prg:      ap

*********************************************************************21*/


/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* Common timer library            */
#include "cm_llist.h"      /* Common linked list library      */
#include "cm_hash.h"       /* Common hash library             */
#include "cm_tpt.h"        /* common transport */
#include "cm_tkns.h"       /* common tokens */
#include "lav.h"           /* LTE CNE layer manager interface */

/* header/extern include files (.x) */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */
#include "cm5.x"           /* Common timer module             */
#include "cm_lib.x"        /* Common linrary function         */
#include "cm_llist.x"      /* Common link list library        */
#include "cm_hash.x"       /* Common hash list library        */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport */
#include "cm_mblk.x"       /* cm_mblk */
#include "lav.x"           /* LTE CNE layer manager interface */

/* local defines */
#define SM_MAX_MILAV_SEL              2

#ifndef LCSMAVMILAV
#define PTSMAVMILAV
#else
#ifndef SM
#define PTSMAVMILAV
#else
#endif
#endif

/* forward references */
#if (!defined(AV) || !defined(LCSMAVMILAV))
PRIVATE S16 PtMiLavCfgReq     ARGS((Pst             *pst,
                                    AvMngmt         *cfg));
PRIVATE S16 PtMiLavCntrlReq   ARGS((Pst             *pst,
                                    AvMngmt         *cntrl));
PRIVATE S16 PtMiLavStaReq     ARGS((Pst             *pst,
                                    AvMngmt         *sta));
PRIVATE S16 PtMiLavStsReq     ARGS((Pst             *pst,
                                    Action          action,
                                    AvMngmt         *sts));

#endif /* #if (!defined(AV) || !defined(LCSMAVMILAV)) */

/* Configuration request primitive */
PRIVATE LavCfgReq SmMiLavCfgReqMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCSMAVMILAV
   cmPkLavCfgReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLavCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef AV
   AvMiLavCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLavCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */
PRIVATE LavCntrlReq SmMiLavCntrlReqMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCSMAVMILAV
   cmPkLavCntrlReq,        /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLavCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef AV
   AvMiLavCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLavCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */
PRIVATE LavStaReq SmMiLavStaReqMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCSMAVMILAV
   cmPkLavStaReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLavStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef AV
   AvMiLavStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLavStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */
PRIVATE LavStsReq SmMiLavStsReqMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCSMAVMILAV
   cmPkLavStsReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLavStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef AV
   AvMiLavStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLavStsReq,          /* 1 - tightly coupled, portable */
#endif
};



/* Primitive Mapping Dispatching Functions */

/*
*
*       Fun:   Configuration request
*
*       Desc:  This function is used to configure LTE CNE layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavCfgReq
(
Pst     *pst,             /* post structure */
AvMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLavCfgReq(pst, cfg)
Pst     *pst;             /* post structure */
AvMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLavCfgReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLavCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);
} /* SmMiLavCfgReq */

/*
*
*       Fun:   Control request
*
*       Desc:  This function is used to control LTE CNE layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavCntrlReq
(
Pst     *pst,             /* post structure */
AvMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLavCntrlReq(pst, cntrl)
Pst     *pst;             /* post structure */
AvMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLavCntrlReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLavCntrlReqMt[pst->selector])(pst, cntrl);

   RETVALUE(ROK);
} /* SmMiLavCntrlReq */

/*
*
*       Fun:   Status request
*
*       Desc:  This function is used to send a Status Request to LTE CNE layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavStaReq
(
Pst     *pst,             /* post structure */
AvMngmt *sta              /* solicited status */
)
#else
PUBLIC S16 SmMiLavStaReq(pst, sta)
Pst     *pst;             /* post structure */
AvMngmt *sta;             /* solicited status */
#endif
{
   TRC3(SmMiLavStaReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLavStaReqMt[pst->selector])(pst, sta);

   RETVALUE(ROK);
} /* SmMiLavStaReq */

/*
*
*       Fun:   Statistics request
*
*       Desc:  This function is used to send a Statistics request to LTE CNE layer
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLavStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* zeroing action */
AvMngmt *sts            /* statistics */
)
#else
PUBLIC S16 SmMiLavStsReq(pst, action, sts)
Pst     *pst;           /* post structure */
Action  action;         /* zeroing action */
AvMngmt *sts;           /* statistics */
#endif
{
   TRC3(SmMiLavStsReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLavStsReqMt[pst->selector])(pst, action, sts);

   RETVALUE(ROK);
} /* SmMiLavStsReq */



#if (!defined(AV) || !defined(LCSMAVMILAV))
/* Portable Stub Functions */

/*
*
*       Fun:   Portable configure Request for LTE CNE layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLavCfgReq
(
Pst     *pst,               /* post structure */
AvMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLavCfgReq(pst, cfg)
Pst     *pst;               /* post structure */
AvMngmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLavCfgReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);
} /* PtMiLavCfgReq */

/*
*
*       Fun:   Portable Control Request for LTE CNE layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLavCntrlReq
(
Pst     *pst,               /* post structure */
AvMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLavCntrlReq(pst, cntrl)
Pst     *pst;               /* post structure */
AvMngmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLavCntrlReq);

   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);
} /* PtMiLavCntrlReq */

/*
*
*       Fun:   Portable Status Request for LTE CNE layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLavStaReq
(
Pst     *pst,               /* post structure */
AvMngmt *sta                /* solicited status */
)
#else
PRIVATE S16 PtMiLavStaReq(pst, sta)
Pst     *pst;               /* post structure */
AvMngmt *sta;               /* solicited status */
#endif
{
   TRC3(PtMiLavStaReq);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* PtMiLavStaReq */

/*
*
*       Fun:   Portable Statistics Request for LTE CNE layer
*
*       Desc:
*
*       Ret:   ROK      - ok
*
*       Notes: None
*
*       File:  smavptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLavStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* action         */
AvMngmt *sts            /* statistics */
)
#else
PRIVATE S16 PtMiLavStsReq(pst, action, sts)
Pst     *pst;               /* post structure */
Action  action;             /* action         */
AvMngmt *sts;               /* statistics */
#endif
{
   TRC3(PtMiLavStsReq);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* PtMiLavStsReq */


#endif /* #if (!defined(AV) || !defined(LCSMAVMILAV)) */


/********************************************************************30**

         End of file:     smavptmi.c@@/main/1 - Tue Aug 17 18:54:43 2010

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
/main/1      -      sp             1. LTE-CNE:PGW Initial Release.
*********************************************************************91*/
