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

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C Source code for Upper Interface procedures

     File:     sq_ptui.c

     Sid:      sq_ptui.c@@/main/nodal_main/3 - Fri Nov 22 17:58:26 2013

     Prg:      rs

*********************************************************************21*/

/* ---- SZT interface ---- */
/*

the following matrices define the mapping between the primitives
called by the upper interface of S1AP and the corresponding
primitives of the S1AP service user(s).

The parameter MAXSZUISZT defines the maximum number of service users on
top of S1AP. There is an array of functions per primitive
invoked by S1AP.

The dispatching is performed by the configurable variable: selector.
The selectors are:

   0 - loosely coupled (#define LCUZLISZT)
   1 - application (#define SZ)
   3 - Light weight loosely coupled

*/
/* header include files (.h) --*/
#include "envopt.h"        /* environment options --*/
#include "envdep.h"        /* environment dependent --*/
#include "envind.h"        /* environment independent --*/
#include "gen.h"           /* general --*/
#include "ssi.h"           /* system services --*/
#include "cm5.h"           /* common timer library --*/
#include "cm_hash.h"       /* common hash library --*/
#include "cm_llist.h"      /* common linked list library --*/
#include "cm_inet.h"       /* common socket library --*/
#include "cm_tpt.h"        /* common transport library --*/
#include "cm_tkns.h"       /* common tokens --*/
#include "cm_mblk.h"       /* Common memory allocation        */
#include "cm_inet.h"       /* Common socket library           */
#include "sct.h"           /* SCT interface defines           */
#include "szt.h"           /* SZT interface defines           */
#include "szt_asn.h"
#ifdef SZ_FTHA
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.h"           /* S1AP Layer management           */
#include "sz.h"            /* S1AP layer defines              */
#include "sz_err.h"            /* S1AP layer defines              */
#include "cm_pasn.h"
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.h"
#endif


/* header/extern include files (.x) --*/
#include "gen.x"           /* general --*/
#include "ssi.x"           /* system services --*/
#include "cm5.x"           /* common timer library --*/
#include "cm_hash.x"       /* common hash library --*/
#include "cm_llist.x"      /* common linked list library --*/
#include "cm_inet.x"       /* common socket library --*/
#include "cm_mblk.x"       /* common mem alloc defines --*/
#include "cm_tkns.x"       /* common tokens --*/
#include "cm_tpt.x"        /* common transport library --*/
#include "cm_lib.x"
#include "cm_inet.x"       /* Common socket library           */
#include"szt_asn.x"        /* common ss7 */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#include "sct.x"           /* SCT interface defines           */
#ifdef SZTV3
/* sz008.301 Support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* SZT interface structures        */
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#include "lsz.x"           /* S1AP Layer management           */
#include "cm_pasn.x"

#include "sz.x"            /* S1AP layer structures           */

#if  !(defined(LCSZUISZT) && defined(LWLCSZUISZT) && defined(UZ))
#undef PTSZUISZT
#define PTSZUISZT /* Portable Upper Interface */
#endif /* defined(LCSZUISZT) && defined(LWLCSZUISZT) && defined(UZ) */

#define SZ_MAX_UISZT_SEL    3   /* maximum number of Upper interface */

/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTSZUISZT

EXTERN S16 PtUiSztBndCfm       ARGS((Pst *pst, SuId suId, U8 status));
EXTERN S16 PtUiSztStaInd       ARGS((Pst *pst, SuId suId, SztStaInd *sztSta));
EXTERN S16 PtUiSztFlcInd       ARGS((Pst *pst, SuId suId, SztFlcInd *flcInd));
EXTERN S16 PtUiSztErrInd       ARGS((Pst *pst, SuId suId,
                                     SztLclErrInd *sztErrInd));
EXTERN S16 PtUiSztUDatInd      ARGS((Pst *pst, SuId suId,
                                     SztUDatEvnt *uDatEvnt));
EXTERN S16 PtUiSztConInd       ARGS((Pst *pst, SuId suId, SztConInd *conInd));
EXTERN S16 PtUiSztConCfm       ARGS((Pst *pst, SuId suId, SztConCfm *conCfm));
EXTERN S16 PtUiSztRelInd       ARGS((Pst *pst, SuId suId, SztRelInd *relInd));
EXTERN S16 PtUiSztRelCfm       ARGS((Pst *pst, SuId suId, SztRelCfm *relCfm));
EXTERN S16 PtUiSztDatInd       ARGS((Pst *pst, SuId suId,
                                     SztDatEvntInd *datEvnt));
EXTERN S16 PtUiSztEncDecCfm    ARGS((Pst *pst, SuId suId,
                                     SztEncDecEvnt *encDecEvnt,
                                     SztReqStatus *status));
EXTERN S16 PtUiSztAudCfm       ARGS((Pst *pst, SuId suId,
                                     SztAudEvnt *audEvnt));
#ifdef SZ_MME
EXTERN S16 PtUiSztPeerAssocInd ARGS((Pst *pst, SuId suId,
                                     SztPeerAssocInd *peerAssocInd));
#endif /* SZ_MME */
#ifdef SZTV2
EXTERN S16 PtUiSztAbortS1Cfm   ARGS((Pst *pst, SuId suId, SztAbortS1 *abortS1));
#endif /* SZTV2 */

#endif /* PTSZUISZT */

PUBLIC SztBndCfm szUiSztBndCfmMt [SZ_MAX_UISZT_SEL] =
{
#ifdef LCSZUISZT
   cmPkSztBndCfm,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztBndCfm,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztBndCfm,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztBndCfm,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztBndCfm,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztBndCfm,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztStatusInd szUiSztStaIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztStaInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztStaInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztStaInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztStaInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztStaInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztStaInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztFlowControlInd szUiSztFlcIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztFlcInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztFlcInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztFlcInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztFlcInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztFlcInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztFlcInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztErrorInd szUiSztErrIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztLclErrInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztErrInd,           /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztLclErrInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztErrInd,           /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztErrInd,           /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztErrInd,           /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztUDatInd szUiSztUDatIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztUDatInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztUDatInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztUDatInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztUDatInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztUDatInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztUDatInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztConnInd szUiSztConIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztConInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztConInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztConInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztConInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztConInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztConInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztConnCfm szUiSztConCfmMt [] =
{
#ifdef LCSZUISZT
   cmPkSztConCfm,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztConCfm,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztConCfm,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztConCfm,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztConCfm,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztConCfm,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

#ifdef SZTV2
PUBLIC SztAbortS1Cfm szUiSztAbortS1CfmMt [] =
{
#ifdef LCSZUISZT
   cmPkSztAbortS1Cfm,        /* 0 - loosely coupled */
#else
   PtUiSztAbortS1Cfm,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCSZUISZT
   cmPkSztAbortS1Cfm,        /* 1 - light weight loosely coupled */
#else
   PtUiSztAbortS1Cfm,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef UZ
   UzLiSztAbortS1Cfm,        /* 2 - tightly coupled, stub layer */
#else
   PtUiSztAbortS1Cfm,        /* 2 - tightly coupled, portable */
#endif
};
#endif /* SZTV2 */


PUBLIC SztRelsInd szUiSztRelIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztRelInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztRelInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztRelInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztRelInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztRelInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztRelInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
 
};

PUBLIC SztRelsCfm szUiSztRelCfmMt [] =
{
#ifdef LCSZUISZT
   cmPkSztRelCfm,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztRelCfm,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztRelCfm,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztRelCfm,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztRelCfm,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztRelCfm,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztDatInd szUiSztDatIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztDatInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztDatInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztDatInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztDatInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztDatInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztDatInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztEncDecCfm szUiSztEncDecCfmMt [] =
{
#ifdef LCSZUISZT
   cmPkSztEncDecCfm,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztEncDecCfm,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztEncDecCfm,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztEncDecCfm,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztEncDecCfm,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztEncDecCfm,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

PUBLIC SztAudCfm szUiSztAudCfmMt [] =
{
#ifdef LCSZUISZT
   cmPkSztAudCfm,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztAudCfm,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztAudCfm,        /* 1 - light weight loosely coupled */
#else  /* LWLCSZUISZT */
   PtUiSztAudCfm,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztAudCfm,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztAudCfm,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};

#ifdef SZ_MME
PUBLIC SztDynamPeerAssocInd szUiSztPeerAssocIndMt [] =
{
#ifdef LCSZUISZT
   cmPkSztPeerAssocInd,        /* 0 - loosely coupled */
#else /* LCSZUISZT */
   PtUiSztPeerAssocInd,        /* 0 - loosely coupled, portable */
#endif /* LCSZUISZT */
#ifdef LWLCSZUISZT
   cmPkSztPeerAssocInd,        /* 1 - light weight loosely coupled */
#else /* LWLCSZUISZT */
   PtUiSztPeerAssocInd,        /* 1 - light weight loosely coupled, portable */
#endif /* LWLCSZUISZT */
#ifdef UZ
   UzLiSztPeerAssocInd,        /* 2 - tightly coupled, stub layer */
#else /* UZ */
   PtUiSztPeerAssocInd,        /* 2 - tightly coupled, portable */
#endif /* UZ */
};
#endif /* SZ_MME */

/************************************************************************
                       Upper Interface Functions
************************************************************************/

/*
 *
 *       Fun:   SzUiSztBndCfm
 *
 *       Desc:  This function resolves the SztBndCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else /* ANSI */
PUBLIC S16 SzUiSztBndCfm (pst, suId, status)
Pst *pst;
SuId suId;
U8 status;
#endif /* ANSI */
{
   TRC3(SzUiSztBndCfm)

   (*szUiSztBndCfmMt[pst->selector])
      (pst, suId, status);

   RETVALUE(ROK);
} /* SzUiSztBndCfm */

/*
 *
 *       Fun:   SzUiSztStaInd
 *
 *       Desc:  This function resolves the SztStaInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztStaInd
(
Pst *pst,
SuId suId,
SztStaInd *sztSta
)
#else  /* ANSI */
PUBLIC S16 SzUiSztStaInd (pst, suId, sztSta)
Pst *pst;
SuId suId;
SztStaInd *sztSta;
#endif /* ANSI */
{
   TRC3(SzUiSztStaInd)

   (*szUiSztStaIndMt[pst->selector])
      (pst, suId, sztSta);

   RETVALUE(ROK);
} /* SzUiSztStaInd */

/*
 *
 *       Fun:   SzUiSztFlcInd
 *
 *       Desc:  This function resolves the SztFlcInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztFlcInd
(
Pst       *pst,
SuId       suId,
SztFlcInd *flcInd
)
#else /* ANSI */ 
PUBLIC S16 SzUiSztFlcInd (pst, suId, flcInd)
Pst       *pst;
SuId       suId;
SztFlcInd *flcInd;
#endif /* ANSI */
{
   TRC3(SzUiSztFlcInd)

   (*szUiSztFlcIndMt[pst->selector])
      (pst, suId, flcInd);

   RETVALUE(ROK);
} /* SzUiSztFlcInd */

/*
 *
 *       Fun:   SzUiSztErrInd
 *
 *       Desc:  This function resolves the SztErrInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztErrInd
(
Pst *pst,
SuId suId,
SztLclErrInd *sztErrInd
)
#else  /* ANSI */
PUBLIC S16 SzUiSztErrInd (pst, suId, sztErrInd)
Pst *pst;
SuId suId;
SztLclErrInd *sztErrInd;
#endif /* ANSI */
{
   TRC3(SzUiSztErrInd)

   (*szUiSztErrIndMt[pst->selector])
      (pst, suId, sztErrInd);

   RETVALUE(ROK);
} /* SzUiSztErrInd */

/*
 *
 *       Fun:   SzUiSztUDatInd
 *
 *       Desc:  This function resolves the SztUDatInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztUDatInd
(
Pst *pst,
SuId suId,
SztUDatEvnt *uDatEvnt
)
#else /* ANSI */  
PUBLIC S16 SzUiSztUDatInd (pst, suId, uDatEvnt)
Pst *pst;
SuId suId;
SztUDatEvnt *uDatEvnt;
#endif /* ANSI */
{
   TRC3(SzUiSztUDatInd)

   (*szUiSztUDatIndMt[pst->selector])
      (pst, suId, uDatEvnt);

   RETVALUE(ROK);
} /* SzUiSztUDatInd */

/*
 *
 *       Fun:   SzUiSztConInd
 *
 *       Desc:  This function resolves the SztConInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztConInd
(
Pst *pst,
SuId suId,
SztConInd *conInd
)
#else  /* ANSI */
PUBLIC S16 SzUiSztConInd (pst, suId, conInd)
Pst *pst;
SuId suId;
SztConInd *conInd;
#endif /* ANSI */
{
   TRC3(SzUiSztConInd)

   (*szUiSztConIndMt[pst->selector])
      (pst, suId, conInd);

   RETVALUE(ROK);
} /* SzUiSztConInd */

#ifdef SZTV2
/*
 *
 *       Fun:   SzUiSztAbortS1Cfm
 *
 *       Desc:  This function resolves the SztAbortS1Cfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztAbortS1Cfm
(
Pst          *pst,
SuId         suId,
SztAbortS1   *abortS1
)
#else 
PUBLIC S16 SzUiSztAbortS1Cfm (pst, suId, abortS1)
Pst          *pst;
SuId         suId;
SztAbortS1   *abortS1;
#endif
{
   TRC3(SzUiSztAbortS1Cfm)

   (*szUiSztAbortS1CfmMt[pst->selector])
      (pst, suId, abortS1);

   RETVALUE(ROK);
} /* SzUiSztAbortS1Cfm */
#endif /* SZTV2 */

/*
 *
 *       Fun:   SzUiSztConCfm
 *
 *       Desc:  This function resolves the SztConCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztConCfm
(
Pst *pst,
SuId suId,
SztConCfm *conCfm
)
#else  /* ANSI */
PUBLIC S16 SzUiSztConCfm (pst, suId, conCfm)
Pst *pst;
SuId suId;
SztConCfm *conCfm;
#endif /* ANSI */
{
   TRC3(SzUiSztConCfm)

   (*szUiSztConCfmMt[pst->selector])
      (pst, suId, conCfm);

   RETVALUE(ROK);
} /* SzUiSztConCfm */


/*
 *
 *       Fun:   SzUiSztRelInd
 *
 *       Desc:  This function resolves the SztRelInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztRelInd
(
Pst *pst,
SuId suId,
SztRelInd *relInd
)
#else  /* ANSI */ 
PUBLIC S16 SzUiSztRelInd (pst, suId, relInd)
Pst *pst;
SuId suId;
SztRelInd *relInd;
#endif /* ANSI */
{
   TRC3(SzUiSztRelInd)

   (*szUiSztRelIndMt[pst->selector])
      (pst, suId, relInd);

   RETVALUE(ROK);
} /* SzUiSztRelInd */

/*
 *
 *       Fun:   SzUiSztRelCfm
 *
 *       Desc:  This function resolves the SztRelCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztRelCfm
(
Pst *pst,
SuId suId,
SztRelCfm *relCfm
)
#else  /* ANSI */
PUBLIC S16 SzUiSztRelCfm (pst, suId, relCfm)
Pst *pst;
SuId suId;
SztRelCfm *relCfm;
#endif /* ANSI */
{
   TRC3(SzUiSztRelCfm)

   (*szUiSztRelCfmMt[pst->selector])
      (pst, suId, relCfm);

   RETVALUE(ROK);
} /* SzUiSztRelCfm */

/*
 *
 *       Fun:   SzUiSztDatInd
 *
 *       Desc:  This function resolves the SztDatInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztDatInd
(
Pst *pst,
SuId suId,
SztDatEvntInd *datEvnt
)
#else  /* ANSI */
PUBLIC S16 SzUiSztDatInd (pst, suId, datEvnt)
Pst *pst;
SuId suId;
SztDatEvntInd *datEvnt;
#endif /* ANSI */
{
   TRC3(SzUiSztDatInd)

   (*szUiSztDatIndMt[pst->selector])
      (pst, suId, datEvnt);

   RETVALUE(ROK);
} /* SzUiSztDatInd */

/*
 *
 *       Fun:   SzUiSztEncDecCfm
 *
 *       Desc:  This function resolves the SztDatInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztEncDecCfm
(
Pst *pst,
SuId suId,
SztEncDecEvnt *encDecEvnt,
SztReqStatus *status
)
#else  /* ANSI */
PUBLIC S16 SzUiSztEncDecCfm (pst, suId, encDecEvnt, status)
Pst *pst;
SuId suId;
SztEncDecEvnt *encDecEvnt;
SztReqStatus *status;
#endif /* ANSI */
{
   TRC3(SzUiSztEncDecCfm)

   (*szUiSztEncDecCfmMt[pst->selector])
      (pst, suId, encDecEvnt, status);

   RETVALUE(ROK);
} /* SzUiSztEncDecCfm */

/*
 *
 *       Fun:   SzUiSztAudCfm
 *
 *       Desc:  This function resolves the SztAudCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztAudCfm
(
Pst *pst,
SuId suId,
SztAudEvnt *audEvnt
)
#else  /* ANSI */
PUBLIC S16 SzUiSztAudCfm (pst, suId, audEvnt)
Pst *pst;
SuId suId;
SztAudEvnt *audEvnt;
#endif /* ANSI */
{
   TRC3(SzUiSztAudCfm)

   (*szUiSztAudCfmMt[pst->selector])
      (pst, suId, audEvnt);

   RETVALUE(ROK);
} /* SzUiSztAudCfm */

#ifdef SZ_MME
/*
 *
 *       Fun:   SzUiSztPeerAssocInd
 *
 *       Desc:  This function resolves the SztPeerAssocInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 SzUiSztPeerAssocInd
(
Pst *pst,
SuId suId,
SztPeerAssocInd *peerAssocInd
)
#else  /* ANSI */
PUBLIC S16 SzUiSztPeerAssocInd (pst, suId, peerAssocInd)
Pst *pst;
SuId suId;
SztPeerAssocInd *peerAssocInd;
#endif /* ANSI */
{
   TRC3(SzUiSztPeerAssocInd)

   (*szUiSztPeerAssocIndMt[pst->selector])
      (pst, suId, peerAssocInd);

   RETVALUE(ROK);
} /* SzUiSztPeerAssocInd */
#endif /* SZ_MME */


/************************************************************************
                          Portable  Functions
************************************************************************/


#ifdef PTSZUISZT

/*
 *
 *       Fun:   PtUiSztBndCfm
 *
 *       Desc:  Portable version of SztBndCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztBndCfm
(
Pst *pst,
SuId suId,
U8 status
)
#else  /* ANSI */
PUBLIC S16 PtUiSztBndCfm (pst, suId, status)
Pst *pst;
SuId suId;
U8 status;
#endif /* ANSI */
{
   TRC3(PtUiSztBndCfm)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ132, (ErrVal) 0, 
              "PtUiSztBndCfm() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztBndCfm */

/*
 *
 *       Fun:   PtUiSztStaInd
 *
 *       Desc:  Portable version of SztStaInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztStaInd
(
Pst *pst,
SuId suId,
SztStaInd *sztSta
)
#else  /* ANSI */
PUBLIC S16 PtUiSztStaInd (pst, suId, sztSta)
Pst *pst;
SuId suId;
SztStaInd *sztSta;
#endif /* ANSI */
{
   TRC3(PtUiSztStaInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ133, (ErrVal) 0, 
              "PtUiSztStaInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztStaInd */

/*
 *
 *       Fun:   PtUiSztFlcInd
 *
 *       Desc:  Portable version of SztFlcInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
 *       File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztFlcInd
(
Pst       *pst,
SuId       suId,
SztFlcInd *flcInd
)
#else  /* ANSI */
PUBLIC S16 PtUiSztFlcInd (pst, suId, flcInd)
Pst       *pst;
SuId       suId;
SztFlcInd *flcInd;
#endif /* ANSI */
{
   TRC3(PtUiSztFlcInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ134, (ErrVal) 0, 
              "PtUiSztFlcInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztFlcInd */

/*
 *
 *       Fun:   PtUiSztErrInd
 *
 *       Desc:  Portable version of SztErrInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztErrInd
(
Pst *pst,
SuId suId,
SztLclErrInd *sztErrInd
)
#else  /* ANSI */
PUBLIC S16 PtUiSztErrInd (pst, suId, sztErrInd)
Pst *pst;
SuId suId;
SztLclErrInd *sztErrInd;
#endif /* ANSI */
{
   TRC3(PtUiSztErrInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ135, (ErrVal) 0, 
              "PtUiSztErrInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztErrInd */

/*
 *
 *       Fun:   PtUiSztUDatInd
 *
 *       Desc:  Portable version of SztUDatInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztUDatInd
(
Pst *pst,
SuId suId,
SztUDatEvnt *uDatEvnt
)
#else  /* ANSI */
PUBLIC S16 PtUiSztUDatInd (pst, suId, uDatEvnt)
Pst *pst;
SuId suId;
SztUDatEvnt *uDatEvnt;
#endif /* ANSI */
{
   TRC3(PtUiSztUDatInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ136, (ErrVal) 0, 
              "PtUiSztUDatInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztUDatInd */

/*
 *
 *       Fun:   PtUiSztConInd
 *
 *       Desc:  Portable version of SztConInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztConInd
(
Pst *pst,
SuId suId,
SztConInd *conInd
)
#else  /* ANSI */
PUBLIC S16 PtUiSztConInd (pst, suId, conInd)
Pst *pst;
SuId suId;
SztConInd *conInd;
#endif /* ANSI */
{
   TRC3(PtUiSztConInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ137, (ErrVal) 0, 
              "PtUiSztConInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztConInd */

/*
 *
 *       Fun:   PtUiSztConCfm
 *
 *       Desc:  Portable version of SztConCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztConCfm
(
Pst *pst,
SuId suId,
SztConCfm *conCfm
)
#else  /* ANSI */
PUBLIC S16 PtUiSztConCfm (pst, suId, conCfm)
Pst *pst;
SuId suId;
SztConCfm *conCfm;
#endif /* ANSI */
{
   TRC3(PtUiSztConCfm)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ138, (ErrVal) 0, 
              "PtUiSztConCfm() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztConCfm */

#ifdef SZTV2
/*
 *
 *       Fun:   PtUiSztAbortS1Cfm
 *
 *       Desc:  Portable version of SztAbortS1Cfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztAbortS1Cfm
(
Pst          *pst,
SuId         suId,
SztAbortS1   *abortS1
)
#else 
PUBLIC S16 PtUiSztAbortS1Cfm (pst, suId, abortS1)
Pst          *pst;
SuId         suId;
SztAbortS1   *abortS1;
#endif
{
   TRC3(PtUiSztAbortS1Cfm)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ139, (ErrVal) 0, 
              "PtUiSztAbortS1Cfm() called");
#endif

   RETVALUE(ROK);
} /* PtUiSztAbortS1Cfm */
#endif /* SZTV2 */

/*
 *
 *       Fun:   PtUiSztRelInd
 *
 *       Desc:  Portable version of SztRelInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztRelInd
(
Pst *pst,
SuId suId,
SztRelInd *relInd
)
#else  /* ANSI */
PUBLIC S16 PtUiSztRelInd (pst, suId, relInd)
Pst *pst;
SuId suId;
SztRelInd *relInd;
#endif /* ANSI */
{
   TRC3(PtUiSztRelInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ140, (ErrVal) 0, 
              "PtUiSztRelInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztRelInd */

/*
 *
 *       Fun:   PtUiSztRelCfm
 *
 *       Desc:  Portable version of SztRelCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztRelCfm
(
Pst *pst,
SuId suId,
SztRelCfm *relCfm
)
#else  /* ANSI */
PUBLIC S16 PtUiSztRelCfm (pst, suId, relCfm)
Pst *pst;
SuId suId;
SztRelCfm *relCfm;
#endif /* ANSI */
{
   TRC3(PtUiSztRelCfm)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ141, (ErrVal) 0, 
              "PtUiSztRelCfm() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztRelCfm */

/*
 *
 *       Fun:   PtUiSztDatInd
 *
 *       Desc:  Portable version of SztDatInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztDatInd
(
Pst *pst,
SuId suId,
SztDatEvntInd *datEvnt
)
#else  /* ANSI */
PUBLIC S16 PtUiSztDatInd (pst, suId, datEvnt)
Pst *pst;
SuId suId;
SztDatEvntInd *datEvnt;
#endif /* ANSI */
{
   TRC3(PtUiSztDatInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ142, (ErrVal) 0, 
              "PtUiSztDatInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztDatInd */

/*
 *
 *       Fun:   PtUiSztEncDecCfm
 *
 *       Desc:  Portable version of SztDatInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztEncDecCfm
(
Pst *pst,
SuId suId,
SztEncDecEvnt *encDecEvnt,
SztReqStatus *status
)
#else  /* ANSI */
PUBLIC S16 PtUiSztEncDecCfm (pst, suId, encDecEvnt, status)
Pst *pst;
SuId suId;
SztEncDecEvnt *encDecEvnt;
SztReqStatus *status;
#endif /* ANSI */
{
   TRC3(PtUiSztEncDecCfm)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ143, (ErrVal) 0, 
              "PtUiSztEncDecCfm() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztEncDecCfm */

/*
 *
 *       Fun:   PtUiSztAudCfm
 *
 *       Desc:  Portable version of SztAudCfm primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztAudCfm
(
Pst *pst,
SuId suId,
SztAudEvnt *audEvnt
)
#else  /* ANSI */
PUBLIC S16 PtUiSztAudCfm (pst, suId, audEvnt)
Pst *pst;
SuId suId;
SztAudEvnt *audEvnt;
#endif /* ANSI */
{
   TRC3(PtUiSztAudCfm)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ144, (ErrVal) 0, 
              "PtUiSztAudCfm() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztAudCfm */

#ifdef SZ_MME
/*
 *
 *       Fun:   PtUiSztPeerAssocInd
 *
 *       Desc:  Portable version of SztPeerAssocInd primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  sz_ptui.c
 *
 */
#ifdef ANSI
PUBLIC S16 PtUiSztPeerAssocInd
(
Pst *pst,
SuId suId,
SztPeerAssocInd *peerAssocInd
)
#else  /* ANSI */
PUBLIC S16 PtUiSztPeerAssocInd (pst, suId, peerAssocInd)
Pst *pst;
SuId suId;
SztPeerAssocInd *peerAssocInd;
#endif /* ANSI */
{
   TRC3(PtUiSztPeerAssocInd)

#if (ERRCLASS & ERRCLS_DEBUG)
   SZLOGERROR(ERRCLS_DEBUG, ESZ145, (ErrVal) 0, 
              "PtUiSztPeerAssocInd() called");
#endif /* ERRCLASS & ERRCLS_DEBUG */

   RETVALUE(ROK);
} /* PtUiSztPeerAssocInd */
#endif /* SZ_MME */


#endif /* PTSZUISZT */


/**********************************************************************

         End of file:     sq_ptui.c@@/main/nodal_main/3 - Fri Nov 22 17:58:26 2013

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/5      ---      va    1. Updated for S1AP release 4.1
*********************************************************************91*/
