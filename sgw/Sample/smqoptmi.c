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

     File:     smqoptmi.c

     Sid:      smqoptmi.c@@/main/1 - Thu Jul  8 21:22:24 2010

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
#include "lqo.h"           /* LTE CNE layer manager interface */

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
#include "lqo.x"           /* LTE CNE layer manager interface */

/* local defines */
#define SM_MAX_MILQO_SEL              2

#ifndef LCSMQOMILQO
#define PTSMQOMILQO
#else
#ifndef SM
#define PTSMQOMILQO
#else
#endif
#endif

/* forward references */
#if (!defined(QO) || !defined(LCSMQOMILQO))
PRIVATE S16 PtMiLqoCfgReq     ARGS((Pst             *pst,
                                    QoMngmt         *cfg));
PRIVATE S16 PtMiLqoCntrlReq   ARGS((Pst             *pst,
                                    QoMngmt         *cntrl));
PRIVATE S16 PtMiLqoStaReq     ARGS((Pst             *pst,
                                    QoMngmt         *sta));
PRIVATE S16 PtMiLqoStsReq     ARGS((Pst             *pst,
                                    Action          action,
                                    QoMngmt         *sts));

#endif /* #if (!defined(QO) || !defined(LCSMQOMILQO)) */

/* Configuration request primitive */
PRIVATE LqoCfgReq SmMiLqoCfgReqMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCSMQOMILQO
   cmPkLqoCfgReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLqoCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef QO
   QoMiLqoCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLqoCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */
PRIVATE LqoCntrlReq SmMiLqoCntrlReqMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCSMQOMILQO
   cmPkLqoCntrlReq,        /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLqoCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef QO
   QoMiLqoCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLqoCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */
PRIVATE LqoStaReq SmMiLqoStaReqMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCSMQOMILQO
   cmPkLqoStaReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLqoStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef QO
   QoMiLqoStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLqoStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */
PRIVATE LqoStsReq SmMiLqoStsReqMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCSMQOMILQO
   cmPkLqoStsReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLqoStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef QO
   QoMiLqoStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLqoStsReq,          /* 1 - tightly coupled, portable */
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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoCfgReq
(
Pst     *pst,             /* post structure */
QoMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLqoCfgReq(pst, cfg)
Pst     *pst;             /* post structure */
QoMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLqoCfgReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLqoCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);
} /* SmMiLqoCfgReq */

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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoCntrlReq
(
Pst     *pst,             /* post structure */
QoMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLqoCntrlReq(pst, cntrl)
Pst     *pst;             /* post structure */
QoMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLqoCntrlReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLqoCntrlReqMt[pst->selector])(pst, cntrl);

   RETVALUE(ROK);
} /* SmMiLqoCntrlReq */

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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoStaReq
(
Pst     *pst,             /* post structure */
QoMngmt *sta              /* solicited status */
)
#else
PUBLIC S16 SmMiLqoStaReq(pst, sta)
Pst     *pst;             /* post structure */
QoMngmt *sta;             /* solicited status */
#endif
{
   TRC3(SmMiLqoStaReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLqoStaReqMt[pst->selector])(pst, sta);

   RETVALUE(ROK);
} /* SmMiLqoStaReq */

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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLqoStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* zeroing action */
QoMngmt *sts            /* statistics */
)
#else
PUBLIC S16 SmMiLqoStsReq(pst, action, sts)
Pst     *pst;           /* post structure */
Action  action;         /* zeroing action */
QoMngmt *sts;           /* statistics */
#endif
{
   TRC3(SmMiLqoStsReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLqoStsReqMt[pst->selector])(pst, action, sts);

   RETVALUE(ROK);
} /* SmMiLqoStsReq */



#if (!defined(QO) || !defined(LCSMQOMILQO))
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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLqoCfgReq
(
Pst     *pst,               /* post structure */
QoMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLqoCfgReq(pst, cfg)
Pst     *pst;               /* post structure */
QoMngmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLqoCfgReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);
} /* PtMiLqoCfgReq */

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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLqoCntrlReq
(
Pst     *pst,               /* post structure */
QoMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLqoCntrlReq(pst, cntrl)
Pst     *pst;               /* post structure */
QoMngmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLqoCntrlReq);

   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);
} /* PtMiLqoCntrlReq */

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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLqoStaReq
(
Pst     *pst,               /* post structure */
QoMngmt *sta                /* solicited status */
)
#else
PRIVATE S16 PtMiLqoStaReq(pst, sta)
Pst     *pst;               /* post structure */
QoMngmt *sta;               /* solicited status */
#endif
{
   TRC3(PtMiLqoStaReq);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* PtMiLqoStaReq */

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
*       File:  smqoptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLqoStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* action         */
QoMngmt *sts            /* statistics */
)
#else
PRIVATE S16 PtMiLqoStsReq(pst, action, sts)
Pst     *pst;               /* post structure */
Action  action;             /* action         */
QoMngmt *sts;               /* statistics */
#endif
{
   TRC3(PtMiLqoStsReq);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* PtMiLqoStsReq */


#endif /* #if (!defined(QO) || !defined(LCSMQOMILQO)) */


/********************************************************************30**

         End of file:     smqoptmi.c@@/main/1 - Thu Jul  8 21:22:24 2010

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
/main/1      -         sp                  1.LTE-CNE:SGW Initial Release
***************************************************i******************91*/
