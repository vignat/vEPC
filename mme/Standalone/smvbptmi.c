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

     File:     smvbptmi.c

     Sid:      smvbptmi.c@@/main/1 - Mon Jun 28 19:35:33 2010

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
#include "sct.h"           /* sct interface  */
#include "cm_tkns.h"       /* common tokens */
#include "lvb.h"           /* LTE CNE layer manager interface */

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
#include "lvb.x"           /* LTE CNE layer manager interface */

/* local defines */
#define SM_MAX_MILVB_SEL              2

#ifndef LCSMVBMILVB
#define PTSMVBMILVB
#else
#ifndef SM
#define PTSMVBMILVB
#else
#endif
#endif

/* forward references */
#if (!defined(VB) || !defined(LCSMVBMILVB))
PRIVATE S16 PtMiLvbCfgReq     ARGS((Pst             *pst,
                                    VbMngmt         *cfg));
PRIVATE S16 PtMiLvbCntrlReq   ARGS((Pst             *pst,
                                    VbMngmt         *cntrl));
PRIVATE S16 PtMiLvbStaReq     ARGS((Pst             *pst,
                                    VbMngmt         *sta));
PRIVATE S16 PtMiLvbStsReq     ARGS((Pst             *pst,
                                    Action          action,
                                    VbMngmt         *sts));

#endif /* #if (!defined(VB) || !defined(LCSMVBMILVB)) */

/* Configuration request primitive */
PRIVATE LvbCfgReq SmMiLvbCfgReqMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCSMVBMILVB
   cmPkLvbCfgReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLvbCfgReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef VB
   VbMiLvbCfgReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLvbCfgReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Control request primitive */
PRIVATE LvbCntrlReq SmMiLvbCntrlReqMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCSMVBMILVB
   cmPkLvbCntrlReq,        /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLvbCntrlReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef VB
   VbMiLvbCntrlReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLvbCntrlReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Status request primitive */
PRIVATE LvbStaReq SmMiLvbStaReqMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCSMVBMILVB
   cmPkLvbStaReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLvbStaReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef VB
   VbMiLvbStaReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLvbStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Statistics request primitive */
PRIVATE LvbStsReq SmMiLvbStsReqMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCSMVBMILVB
   cmPkLvbStsReq,          /* 0 - loosely coupled (default mechanism) */
#else
   PtMiLvbStsReq,          /* 0 - tightly coupled, portable */
#endif
#ifdef VB
   VbMiLvbStsReq,          /* 1 - tightly coupled, layer management */
#else
   PtMiLvbStsReq,          /* 1 - tightly coupled, portable */
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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbCfgReq
(
Pst     *pst,             /* post structure */
VbMngmt *cfg              /* configure */
)
#else
PUBLIC S16 SmMiLvbCfgReq(pst, cfg)
Pst     *pst;             /* post structure */
VbMngmt *cfg;             /* configure */
#endif
{
   TRC3(SmMiLvbCfgReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLvbCfgReqMt[pst->selector])(pst, cfg);

   RETVALUE(ROK);
} /* SmMiLvbCfgReq */

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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbCntrlReq
(
Pst     *pst,             /* post structure */
VbMngmt *cntrl            /* control */
)
#else
PUBLIC S16 SmMiLvbCntrlReq(pst, cntrl)
Pst     *pst;             /* post structure */
VbMngmt *cntrl;           /* control */
#endif
{
   TRC3(SmMiLvbCntrlReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLvbCntrlReqMt[pst->selector])(pst, cntrl);

   RETVALUE(ROK);
} /* SmMiLvbCntrlReq */

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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbStaReq
(
Pst     *pst,             /* post structure */
VbMngmt *sta              /* solicited status */
)
#else
PUBLIC S16 SmMiLvbStaReq(pst, sta)
Pst     *pst;             /* post structure */
VbMngmt *sta;             /* solicited status */
#endif
{
   TRC3(SmMiLvbStaReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLvbStaReqMt[pst->selector])(pst, sta);

   RETVALUE(ROK);
} /* SmMiLvbStaReq */

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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PUBLIC S16 SmMiLvbStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* zeroing action */
VbMngmt *sts            /* statistics */
)
#else
PUBLIC S16 SmMiLvbStsReq(pst, action, sts)
Pst     *pst;           /* post structure */
Action  action;         /* zeroing action */
VbMngmt *sts;           /* statistics */
#endif
{
   TRC3(SmMiLvbStsReq)

   /* jump to specific primitive depending on configured selector */
   (*SmMiLvbStsReqMt[pst->selector])(pst, action, sts);

   RETVALUE(ROK);
} /* SmMiLvbStsReq */



#if (!defined(VB) || !defined(LCSMVBMILVB))
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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLvbCfgReq
(
Pst     *pst,               /* post structure */
VbMngmt *cfg                /* configure */
)
#else
PRIVATE S16 PtMiLvbCfgReq(pst, cfg)
Pst     *pst;               /* post structure */
VbMngmt *cfg;               /* configure */
#endif
{
   TRC3(PtMiLvbCfgReq);

   UNUSED(pst);
   UNUSED(cfg);

   RETVALUE(ROK);
} /* PtMiLvbCfgReq */

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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLvbCntrlReq
(
Pst     *pst,               /* post structure */
VbMngmt *cntrl              /* control */
)
#else
PRIVATE S16 PtMiLvbCntrlReq(pst, cntrl)
Pst     *pst;               /* post structure */
VbMngmt *cntrl;             /* control */
#endif
{
   TRC3(PtMiLvbCntrlReq);

   UNUSED(pst);
   UNUSED(cntrl);

   RETVALUE(ROK);
} /* PtMiLvbCntrlReq */

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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLvbStaReq
(
Pst     *pst,               /* post structure */
VbMngmt *sta                /* solicited status */
)
#else
PRIVATE S16 PtMiLvbStaReq(pst, sta)
Pst     *pst;               /* post structure */
VbMngmt *sta;               /* solicited status */
#endif
{
   TRC3(PtMiLvbStaReq);

   UNUSED(pst);
   UNUSED(sta);

   RETVALUE(ROK);
} /* PtMiLvbStaReq */

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
*       File:  smvbptmi.c
*
*/

#ifdef ANSI
PRIVATE S16 PtMiLvbStsReq
(
Pst     *pst,           /* post structure */
Action  action,         /* action         */
VbMngmt *sts            /* statistics */
)
#else
PRIVATE S16 PtMiLvbStsReq(pst, action, sts)
Pst     *pst;               /* post structure */
Action  action;             /* action         */
VbMngmt *sts;               /* statistics */
#endif
{
   TRC3(PtMiLvbStsReq);

   UNUSED(pst);
   UNUSED(sts);

   RETVALUE(ROK);
} /* PtMiLvbStsReq */


#endif /* #if (!defined(VB) || !defined(LCSMVBMILVB)) */


/********************************************************************30**

         End of file:     smvbptmi.c@@/main/1 - Mon Jun 28 19:35:33 2010

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
/main/1      -       ad            1. Initial Release for LTE-MME 1.1
*********************************************************************91*/
