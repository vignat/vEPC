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

     File:     jg_ptmi.c

     Sid:      av_ptmi.c@@/main/1 - Tue Aug 17 18:54:34 2010

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
#include "avsm.h"

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
#include "avsm.x"

/* local defines */
#define SM_MAX_MILAV_SEL              3

#ifndef LCSMAVMILAV
#define PTSMAVMILAV
#else
#ifndef SM
#define PTSMAVMILAV
#else
#endif
#endif

/* Forward references */

PRIVATE S16 PtMiLavCfgCfm ARGS((Pst * pst, AvMngmt * cfm));

PRIVATE S16 PtMiLavStsCfm ARGS((Pst * pst, AvMngmt * cfm));

PRIVATE S16 PtMiLavStaCfm ARGS((Pst * pst, AvMngmt * cfm));

PRIVATE S16 PtMiLavStaInd ARGS((Pst * pst, AvMngmt * usta));

PRIVATE S16 PtMiLavCntrlCfm ARGS((Pst * pst, AvMngmt * cfm));

PRIVATE S16 PtMiLavTrcInd ARGS((Pst * pst, AvMngmt * trc, Buffer * trcBuf));


PRIVATE CONSTANT LavCfgCfm AvMiLavCfgCfmMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCAVMILAV
   cmPkLavCfgCfm,
#else
   PtMiLavCfgCfm,
#endif
#ifdef SM
   SmMiLavCfgCfm,
#else
   PtMiLavCfgCfm,
#endif
#ifdef LWLCAVMILAV
   cmPkLavCfgCfm
#else
   PtMiLavCfgCfm
#endif
};

PRIVATE CONSTANT LavStsCfm AvMiLavStsCfmMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCAVMILAV
   cmPkLavStsCfm,
#else
   PtMiLavStsCfm,
#endif
#ifdef SM
   SmMiLavStsCfm,
#else
   PtMiLavStsCfm,
#endif
#ifdef LWLCAVMILAV
   cmPkLavStsCfm
#else
   PtMiLavStsCfm
#endif
};

PRIVATE CONSTANT LavStaCfm AvMiLavStaCfmMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCAVMILAV
   cmPkLavStaCfm,
#else
   PtMiLavStaCfm,
#endif
#ifdef SM
   SmMiLavStaCfm,
#else
   PtMiLavStaCfm,
#endif
#ifdef LWLCAVMILAV
   cmPkLavStaCfm
#else
   PtMiLavStaCfm
#endif
};

PRIVATE CONSTANT LavStaInd AvMiLavStaIndMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCAVMILAV
   cmPkLavStaInd,
#else
   PtMiLavStaInd,
#endif
#ifdef SM
   SmMiLavStaInd,
#else
   PtMiLavStaInd,
#endif
#ifdef LWLCAVMILAV
   cmPkLavStaInd
#else
   PtMiLavStaInd
#endif
};

PRIVATE CONSTANT LavCntrlCfm AvMiLavCntrlCfmMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCAVMILAV
   cmPkLavCntrlCfm,
#else
   PtMiLavCntrlCfm,
#endif
#ifdef SM
   SmMiLavCntrlCfm,
#else
   PtMiLavCntrlCfm,
#endif
#ifdef LWLCAVMILAV
   cmPkLavCntrlCfm
#else
   PtMiLavCntrlCfm
#endif
};

PRIVATE CONSTANT LavTrcInd AvMiLavTrcIndMt[SM_MAX_MILAV_SEL] =
{
#ifdef LCAVMILAV
   cmPkLavTrcInd,
#else
   PtMiLavTrcInd,
#endif
#ifdef SM
   SmMiLavTrcInd,
#else
   PtMiLavTrcInd,
#endif
#ifdef LWLCAVMILAV
   cmPkLavTrcInd
#else
   PtMiLavTrcInd
#endif
};


#ifdef AV


#ifdef ANSI
PUBLIC S16 AvMiLavCfgCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PUBLIC S16 AvMiLavCfgCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{
   TRC3(AvMiLavCfgCfm)

   (*AvMiLavCfgCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 AvMiLavStsCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PUBLIC S16 AvMiLavStsCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{

   TRC3(AvMiLavStsCfm)

   (*AvMiLavStsCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);


}


#ifdef ANSI
PUBLIC S16 AvMiLavStaCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PUBLIC S16 AvMiLavStaCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{
   TRC3(AvMiLavStaCfm)

   (*AvMiLavStaCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 AvMiLavStaInd
(
Pst     *pst,
AvMngmt *usta
)
#else
PUBLIC S16 AvMiLavStaInd(pst, usta)
Pst     *pst;
AvMngmt *usta;
#endif
{

   TRC3(AvMiLavStaInd)

   (*AvMiLavStaIndMt[pst->selector])(pst, usta);

   RETVALUE(ROK);


}


#ifdef ANSI
PUBLIC S16 AvMiLavCntrlCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PUBLIC S16 AvMiLavCntrlCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{

   TRC3(AvMiLavCntrlCfm)

   (*AvMiLavCntrlCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 AvMiLavTrcInd
(
Pst     *pst,
AvMngmt *trc,
Buffer  *trcBuf
)
#else
PUBLIC S16 AvMiLavTrcInd(pst, trc, trcBuf)
Pst     *pst;
AvMngmt *trc;
Buffer  *trcBuf;
#endif
{

   TRC3(AvMiLavTrcInd)

   (*AvMiLavTrcIndMt[pst->selector])(pst, trc, trcBuf);

   RETVALUE(ROK);

}

#endif

#ifdef ANSI
PRIVATE S16 PtMiLavCfgCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PRIVATE S16 PtMiLavCfgCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{

   TRC3(PtMiLavCfgCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLavStsCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PRIVATE S16 PtMiLavStsCfm(pst,  cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{

   TRC3(PtMiLavStsCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 PtMiLavStaCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PRIVATE S16 PtMiLavStaCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{

   TRC3(PtMiLavStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}
                         

#ifdef ANSI
PRIVATE S16 PtMiLavStaInd
(
Pst     *pst,
AvMngmt *usta
)
#else
PRIVATE S16 PtMiLavStaInd(pst, usta)
Pst     *pst;
AvMngmt *usta;
#endif
{

   TRC3(PtMiLavStaInd)

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLavCntrlCfm
(
Pst     *pst,
AvMngmt *cfm
)
#else
PRIVATE S16 PtMiLavCntrlCfm(pst, cfm)
Pst     *pst;
AvMngmt *cfm;
#endif
{

   TRC3(PtMiLavCntrlCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLavTrcInd
(
Pst     *pst,
AvMngmt *trc,
Buffer  *trcBuf
)
#else
PRIVATE S16 PtMiLavTrcInd(pst, trc, trcBuf)
Pst     *pst;
AvMngmt *trc;
Buffer  *trcBuf;
#endif
{

   TRC3(PtMiLavTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(trcBuf);

   RETVALUE(ROK);

}



/********************************************************************30**

         End of file:     av_ptmi.c@@/main/1 - Tue Aug 17 18:54:34 2010

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
/main/1      -      sp                1.LTE-CNE:PGW Initial Release
*********************************************************************91*/
