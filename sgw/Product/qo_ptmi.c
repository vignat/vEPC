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

     Name:     SGW Portable Management Interface

     Type:     C source file

     Desc:     C source code for SGW Layer portable Management Interface

     File:     qo_ptmi.c

     Sid:      qo_ptmi.c@@/main/1 - Thu Jul  8 21:22:20 2010

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
#include "qosm.h"

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
#include "qosm.x"

/* local defines */
#define SM_MAX_MILQO_SEL              3

#ifndef LCSMQOMILQO
#define PTSMQOMILQO
#else
#ifndef SM
#define PTSMQOMILQO
#else
#endif
#endif

/* Forward references */

PRIVATE S16 PtMiLqoCfgCfm ARGS((Pst * pst, QoMngmt * cfm));

PRIVATE S16 PtMiLqoStsCfm ARGS((Pst * pst, QoMngmt * cfm));

PRIVATE S16 PtMiLqoStaCfm ARGS((Pst * pst, QoMngmt * cfm));

PRIVATE S16 PtMiLqoStaInd ARGS((Pst * pst, QoMngmt * usta));

PRIVATE S16 PtMiLqoCntrlCfm ARGS((Pst * pst, QoMngmt * cfm));

PRIVATE S16 PtMiLqoTrcInd ARGS((Pst * pst, QoMngmt * trc, Buffer * trcBuf));


PRIVATE CONSTANT LqoCfgCfm QoMiLqoCfgCfmMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCQOMILQO
   cmPkLqoCfgCfm,
#else
   PtMiLqoCfgCfm,
#endif
#ifdef SM
   SmMiLqoCfgCfm,
#else
   PtMiLqoCfgCfm,
#endif
#ifdef LWLCQOMILQO
   cmPkLqoCfgCfm
#else
   PtMiLqoCfgCfm
#endif
};

PRIVATE CONSTANT LqoStsCfm QoMiLqoStsCfmMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCQOMILQO
   cmPkLqoStsCfm,
#else
   PtMiLqoStsCfm,
#endif
#ifdef SM
   SmMiLqoStsCfm,
#else
   PtMiLqoStsCfm,
#endif
#ifdef LWLCQOMILQO
   cmPkLqoStsCfm
#else
   PtMiLqoStsCfm
#endif
};

PRIVATE CONSTANT LqoStaCfm QoMiLqoStaCfmMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCQOMILQO
   cmPkLqoStaCfm,
#else
   PtMiLqoStaCfm,
#endif
#ifdef SM
   SmMiLqoStaCfm,
#else
   PtMiLqoStaCfm,
#endif
#ifdef LWLCQOMILQO
   cmPkLqoStaCfm
#else
   PtMiLqoStaCfm
#endif
};

PRIVATE CONSTANT LqoStaInd QoMiLqoStaIndMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCQOMILQO
   cmPkLqoStaInd,
#else
   PtMiLqoStaInd,
#endif
#ifdef SM
   SmMiLqoStaInd,
#else
   PtMiLqoStaInd,
#endif
#ifdef LWLCQOMILQO
   cmPkLqoStaInd
#else
   PtMiLqoStaInd
#endif
};

PRIVATE CONSTANT LqoCntrlCfm QoMiLqoCntrlCfmMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCQOMILQO
   cmPkLqoCntrlCfm,
#else
   PtMiLqoCntrlCfm,
#endif
#ifdef SM
   SmMiLqoCntrlCfm,
#else
   PtMiLqoCntrlCfm,
#endif
#ifdef LWLCQOMILQO
   cmPkLqoCntrlCfm
#else
   PtMiLqoCntrlCfm
#endif
};

PRIVATE CONSTANT LqoTrcInd QoMiLqoTrcIndMt[SM_MAX_MILQO_SEL] =
{
#ifdef LCQOMILQO
   cmPkLqoTrcInd,
#else
   PtMiLqoTrcInd,
#endif
#ifdef SM
   SmMiLqoTrcInd,
#else
   PtMiLqoTrcInd,
#endif
#ifdef LWLCQOMILQO
   cmPkLqoTrcInd
#else
   PtMiLqoTrcInd
#endif
};


#ifdef QO


#ifdef ANSI
PUBLIC S16 QoMiLqoCfgCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PUBLIC S16 QoMiLqoCfgCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{
   TRC3(QoMiLqoCfgCfm)

   (*QoMiLqoCfgCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 QoMiLqoStsCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PUBLIC S16 QoMiLqoStsCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{

   TRC3(QoMiLqoStsCfm)

   (*QoMiLqoStsCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);


}


#ifdef ANSI
PUBLIC S16 QoMiLqoStaCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PUBLIC S16 QoMiLqoStaCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{
   TRC3(QoMiLqoStaCfm)

   (*QoMiLqoStaCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 QoMiLqoStaInd
(
Pst     *pst,
QoMngmt *usta
)
#else
PUBLIC S16 QoMiLqoStaInd(pst, usta)
Pst     *pst;
QoMngmt *usta;
#endif
{

   TRC3(QoMiLqoStaInd)

   (*QoMiLqoStaIndMt[pst->selector])(pst, usta);

   RETVALUE(ROK);


}


#ifdef ANSI
PUBLIC S16 QoMiLqoCntrlCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PUBLIC S16 QoMiLqoCntrlCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{

   TRC3(QoMiLqoCntrlCfm)

   (*QoMiLqoCntrlCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 QoMiLqoTrcInd
(
Pst     *pst,
QoMngmt *trc,
Buffer  *trcBuf
)
#else
PUBLIC S16 QoMiLqoTrcInd(pst, trc, trcBuf)
Pst     *pst;
QoMngmt *trc;
Buffer  *trcBuf;
#endif
{

   TRC3(QoMiLqoTrcInd)

   (*QoMiLqoTrcIndMt[pst->selector])(pst, trc, trcBuf);

   RETVALUE(ROK);

}

#endif

#ifdef ANSI
PRIVATE S16 PtMiLqoCfgCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PRIVATE S16 PtMiLqoCfgCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{

   TRC3(PtMiLqoCfgCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLqoStsCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PRIVATE S16 PtMiLqoStsCfm(pst,  cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{

   TRC3(PtMiLqoStsCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 PtMiLqoStaCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PRIVATE S16 PtMiLqoStaCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{

   TRC3(PtMiLqoStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}
                         

#ifdef ANSI
PRIVATE S16 PtMiLqoStaInd
(
Pst     *pst,
QoMngmt *usta
)
#else
PRIVATE S16 PtMiLqoStaInd(pst, usta)
Pst     *pst;
QoMngmt *usta;
#endif
{

   TRC3(PtMiLqoStaInd)

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLqoCntrlCfm
(
Pst     *pst,
QoMngmt *cfm
)
#else
PRIVATE S16 PtMiLqoCntrlCfm(pst, cfm)
Pst     *pst;
QoMngmt *cfm;
#endif
{

   TRC3(PtMiLqoCntrlCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLqoTrcInd
(
Pst     *pst,
QoMngmt *trc,
Buffer  *trcBuf
)
#else
PRIVATE S16 PtMiLqoTrcInd(pst, trc, trcBuf)
Pst     *pst;
QoMngmt *trc;
Buffer  *trcBuf;
#endif
{

   TRC3(PtMiLqoTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(trcBuf);

   RETVALUE(ROK);

}



/********************************************************************30**

         End of file:     qo_ptmi.c@@/main/1 - Thu Jul  8 21:22:20 2010

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
/main/1      -         sp                   1.LTE-CNE:SGW Initial Release
*********************************************************************91*/
