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

     Name:     LTE CNE layer portable management interface

     Type:     C source file

     Desc:     C source code for the LTE CNE layer layer management
               service user primitives used in loosely coupled
               systems.

     File:     vb_ptmi.c

     Sid:      vb_ptmi.c@@/main/1 - Mon Jun 28 19:35:40 2010

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
#include "szt_asn.h"       /* S1AP ASN */
#include "szt.h"           /* S1AP */
#include "lvb.h"           /* LTE CNE layer manager interface */
#include "vbsm.h"

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
/* vb007.101 s1ap include files order has changed */
#include "szt_asn.x"       /* S1AP ASN */
#include "szt.x"           /* S1AP */
#include "lvb.x"           /* LTE CNE layer manager interface */
#include "vbsm.x"

/* local defines */
#define SM_MAX_MILVB_SEL              3

#ifndef LCSMVBMILVB
#define PTSMVBMILVB
#else
#ifndef SM
#define PTSMVBMILVB
#else
#endif
#endif

/* Forward references */

PRIVATE S16 PtMiLvbCfgCfm ARGS((Pst * pst, VbMngmt * cfm));

PRIVATE S16 PtMiLvbStsCfm ARGS((Pst * pst, VbMngmt * cfm));

PRIVATE S16 PtMiLvbStaCfm ARGS((Pst * pst, VbMngmt * cfm));

PRIVATE S16 PtMiLvbStaInd ARGS((Pst * pst, VbMngmt * usta));

PRIVATE S16 PtMiLvbCntrlCfm ARGS((Pst * pst, VbMngmt * cfm));

PRIVATE S16 PtMiLvbTrcInd ARGS((Pst * pst, VbMngmt * trc, Buffer * trcBuf));


PRIVATE CONSTANT LvbCfgCfm VbMiLvbCfgCfmMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCVBMILVB
   cmPkLvbCfgCfm,
#else
   PtMiLvbCfgCfm,
#endif
#ifdef SM
   SmMiLvbCfgCfm,
#else
   PtMiLvbCfgCfm,
#endif
#ifdef LWLCVBMILVB
   cmPkLvbCfgCfm
#else
   PtMiLvbCfgCfm
#endif
};

PRIVATE CONSTANT LvbStsCfm VbMiLvbStsCfmMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCVBMILVB
   cmPkLvbStsCfm,
#else
   PtMiLvbStsCfm,
#endif
#ifdef SM
   SmMiLvbStsCfm,
#else
   PtMiLvbStsCfm,
#endif
#ifdef LWLCVBMILVB
   cmPkLvbStsCfm
#else
   PtMiLvbStsCfm
#endif
};

PRIVATE CONSTANT LvbStaCfm VbMiLvbStaCfmMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCVBMILVB
   cmPkLvbStaCfm,
#else
   PtMiLvbStaCfm,
#endif
#ifdef SM
   SmMiLvbStaCfm,
#else
   PtMiLvbStaCfm,
#endif
#ifdef LWLCVBMILVB
   cmPkLvbStaCfm
#else
   PtMiLvbStaCfm
#endif
};

PRIVATE CONSTANT LvbStaInd VbMiLvbStaIndMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCVBMILVB
   cmPkLvbStaInd,
#else
   PtMiLvbStaInd,
#endif
#ifdef SM
   SmMiLvbStaInd,
#else
   PtMiLvbStaInd,
#endif
#ifdef LWLCVBMILVB
   cmPkLvbStaInd
#else
   PtMiLvbStaInd
#endif
};

PRIVATE CONSTANT LvbCntrlCfm VbMiLvbCntrlCfmMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCVBMILVB
   cmPkLvbCntrlCfm,
#else
   PtMiLvbCntrlCfm,
#endif
#ifdef SM
   SmMiLvbCntrlCfm,
#else
   PtMiLvbCntrlCfm,
#endif
#ifdef LWLCVBMILVB
   cmPkLvbCntrlCfm
#else
   PtMiLvbCntrlCfm
#endif
};

PRIVATE CONSTANT LvbTrcInd VbMiLvbTrcIndMt[SM_MAX_MILVB_SEL] =
{
#ifdef LCVBMILVB
   cmPkLvbTrcInd,
#else
   PtMiLvbTrcInd,
#endif
#ifdef SM
   SmMiLvbTrcInd,
#else
   PtMiLvbTrcInd,
#endif
#ifdef LWLCVBMILVB
   cmPkLvbTrcInd
#else
   PtMiLvbTrcInd
#endif
};


#ifdef VB


#ifdef ANSI
PUBLIC S16 VbMiLvbCfgCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PUBLIC S16 VbMiLvbCfgCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{
   TRC3(VbMiLvbCfgCfm)

   (*VbMiLvbCfgCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 VbMiLvbStsCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PUBLIC S16 VbMiLvbStsCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{

   TRC3(VbMiLvbStsCfm)

   (*VbMiLvbStsCfmMt[pst->selector])(pst,cfm);

   RETVALUE(ROK);


}


#ifdef ANSI
PUBLIC S16 VbMiLvbStaCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PUBLIC S16 VbMiLvbStaCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{
   TRC3(VbMiLvbStaCfm)

   (*VbMiLvbStaCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 VbMiLvbStaInd
(
Pst     *pst,
VbMngmt *usta
)
#else
PUBLIC S16 VbMiLvbStaInd(pst, usta)
Pst     *pst;
VbMngmt *usta;
#endif
{

   TRC3(VbMiLvbStaInd)

   (*VbMiLvbStaIndMt[pst->selector])(pst, usta);

   RETVALUE(ROK);


}


#ifdef ANSI
PUBLIC S16 VbMiLvbCntrlCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PUBLIC S16 VbMiLvbCntrlCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{

   TRC3(VbMiLvbCntrlCfm)

   (*VbMiLvbCntrlCfmMt[pst->selector])(pst, cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PUBLIC S16 VbMiLvbTrcInd
(
Pst     *pst,
VbMngmt *trc,
Buffer  *trcBuf
)
#else
PUBLIC S16 VbMiLvbTrcInd(pst, trc, trcBuf)
Pst     *pst;
VbMngmt *trc;
Buffer  *trcBuf;
#endif
{

   TRC3(VbMiLvbTrcInd)

   (*VbMiLvbTrcIndMt[pst->selector])(pst, trc, trcBuf);

   RETVALUE(ROK);

}

#endif

#ifdef ANSI
PRIVATE S16 PtMiLvbCfgCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PRIVATE S16 PtMiLvbCfgCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{

   TRC3(PtMiLvbCfgCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLvbStsCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PRIVATE S16 PtMiLvbStsCfm(pst,  cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{

   TRC3(PtMiLvbStsCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);
}


#ifdef ANSI
PRIVATE S16 PtMiLvbStaCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PRIVATE S16 PtMiLvbStaCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{

   TRC3(PtMiLvbStaCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}
                         

#ifdef ANSI
PRIVATE S16 PtMiLvbStaInd
(
Pst     *pst,
VbMngmt *usta
)
#else
PRIVATE S16 PtMiLvbStaInd(pst, usta)
Pst     *pst;
VbMngmt *usta;
#endif
{

   TRC3(PtMiLvbStaInd)

   UNUSED(pst);
   UNUSED(usta);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLvbCntrlCfm
(
Pst     *pst,
VbMngmt *cfm
)
#else
PRIVATE S16 PtMiLvbCntrlCfm(pst, cfm)
Pst     *pst;
VbMngmt *cfm;
#endif
{

   TRC3(PtMiLvbCntrlCfm)

   UNUSED(pst);
   UNUSED(cfm);

   RETVALUE(ROK);

}


#ifdef ANSI
PRIVATE S16 PtMiLvbTrcInd
(
Pst     *pst,
VbMngmt *trc,
Buffer  *trcBuf
)
#else
PRIVATE S16 PtMiLvbTrcInd(pst, trc, trcBuf)
Pst     *pst;
VbMngmt *trc;
Buffer  *trcBuf;
#endif
{

   TRC3(PtMiLvbTrcInd)

   UNUSED(pst);
   UNUSED(trc);
   UNUSED(trcBuf);

   RETVALUE(ROK);

}



/********************************************************************30**

         End of file:     vb_ptmi.c@@/main/1 - Mon Jun 28 19:35:40 2010

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
/main/1      -         sp              1. LTE-CNE Initial Release.
/main/1   vb007.101 akaranth           1. s1ap include files order has changed.
*********************************************************************91*/
