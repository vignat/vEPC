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

     Name:     eGTP Protocol - upper interface

     Type:     C source file

     Desc:     C Source code for the eGTP Protocol upper interface primitives.

     File:     eg_ptui.c

     Sid:      eg_ptui.c@@/main/3 - Fri Feb  4 18:18:12 2011

     Prg:      kc

*********************************************************************21*/


/* header include files (.h) */

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */
#include "gen.h"           /* General */
#include "ssi.h"           /* System Services */
#include "cm_tkns.h"       /* common tokens */
#include "cm_hash.h"       /* common structs 1 */
#include "cm_mblk.h"       /* common memory */
#include "cm_llist.h"      /* cm link list */
#include "cm5.h"           /* common structs 3 */
#include "cm_inet.h"       /* common tokens  */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport */
#include "cm_dns.h"        /* common transport */
#include "hit.h"           /* TUCL Layer */
/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_psf.h"
#include "sht.h"
#endif
#include "egt.h"           /* eGTP Upper Interface */
#include "leg.h"           /* eGTP LM Interface */
#include "eg.h"            /* eGTP Layer */
#include "eg_err.h"        /* Error codes */
#include "eg_util.h"       /* eGTP Utility library */
#include "eg_edm.h"        /* EDM Module structures            */
#ifdef TIME_MES
#include <time.h>
#endif

/* Header Include Files (.x) */

#include "gen.x"           /* General */
#include "ssi.x"           /* System Services */
#include "cm_tkns.x"       /* common tokens */
#include "cm_hash.x"       /* common structs 1 */
#include "cm_lib.x"        /* common library */
#include "cm_mblk.x"       /* common memory */
#include "cm_llist.x"      /* cm link list */
#include "cm5.x"           /* common structs 3 */
#include "cm_inet.x"       /* common transport */
#include "cm_tpt.x"        /* common transport */
#include "cm_dns.x"        /* common transport */
#include "hit.x"           /* TUCL Layer */
/* eg004.201 Header files added for eGTP-C PSF*/
#ifdef HW
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"
#include "sht.x"
#endif
#include "egt.x"           /* eGTP Upper Interface */
#include "leg.x"           /* eGTP LM Interface */
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* eGTP Layer */
#include "eg_util.x"       /* eGTP Utility library */
/* eg004.201 Header files added for eGTP-C PSF*/
#include "eg_tpt.x"            /* EGTP TPT module defines */
#ifdef HW
#include "lhw.x"
#include "hw.x"          /* EGTPC UA                       */
#endif

/* local function definition */

/* local defines */
#define EG_MAX_UIEGT_SEL      3

#if (!defined(LWLCEGUIEGT) || !defined(LCEGUIEGT))
#define PTEGUIEGT
#else
#ifndef EU
#define PTEGUIEGT
#else
#endif
#endif

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

#ifdef PTEGUIEGT

/* portable functions */
PRIVATE S16 PtUiEgtBndCfm ARGS (( Pst *pst, SuId suId, U8 status));

#ifdef EGTP_C
PRIVATE S16 PtUiEgtErrInd ARGS((Pst *pst, SuId suId, EgErrEvnt *errEvnt));
PRIVATE S16 PtUiEgtSigInd ARGS (( Pst *pst, SuId suId, CmTptAddr *localAddr,
                                  CmTptAddr *remAddr, TknU32 lclTeid, EgMsg *egMsg));

PRIVATE S16 PtUiEgtSigCfm ARGS (( Pst *pst, SuId suId, CmTptAddr *localAddr,
                                  CmTptAddr *remAddr, TknU32 transId,TknU32 lclTeid,
                                  EgMsg *egMsg));

PRIVATE S16 PtUiEgtLclDelCfm ARGS (( Pst *pst, SuId suId, U32 transId, U32 teid,
                                     CmStatus status));
PRIVATE S16 PtUiEgtStaInd ARGS (( Pst *pst, SuId suId, U8 eventType,
                                  EgPathStatus *status));

PRIVATE S16 PtUiEgtStaCfm ARGS (( Pst *pst, SuId suId, U8 eventType,
                                  EgPathInfo *pathInfo, CmStatus status));

PUBLIC S16 PtUiEgtTunnModCfm ARGS ((Pst *pst, SuId suId, U32 transId, U32 teid, 
                                    CmTptAddr oldDstIpAddr, CmTptAddr newDstIpAddr, CmStatus     status));
#endif
#ifdef EGTP_U
PUBLIC S16 PtUiEgtEguLclTnlMgmtCfm ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
PUBLIC S16 PtUiEgtEguStaCfm ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
PUBLIC S16 PtUiEgtEguUStaInd ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
PUBLIC S16 PtUiEgtEguErrInd ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
PUBLIC S16 PtUiEgtEguDatInd ARGS((Pst *pst, SuId suId, EgtUEvnt *egtUEvnt));
#endif
#endif /*PTEGUIEGT */

#ifdef __cplusplus
} /* end of EXTERN "C" */
#endif /* end of __cplusplus */

/*
   The following matrices define the mapping between the primitives
   called by the upper interface of eGTP Base Protocol layer and
   the corresponding primitives of the eGTP Base Protocol layer
   service user(s).

   The parameter EG_MAX_UIEGT_SEL defines the maximum number of service users on
   top of eGTP Base Protocol. There is an array of functions per
   primitive invoked by eGTP Base Protocol. Every array is EG_MAX_UIEGT_SEL
   long (i.e. there are as many functions as the number of service users).

   The dispatching is performed by the configurable variable: selector.
   The selector is configured on a per SAP basis. The selectors are:

   EGT_SEL_LC - loosely coupled
   EGT_SEL_TC - tightly coupled

*/

/* Bind Confirm primitive */
PRIVATE CONSTANT EgtBndCfm EgUiEgtBndCfmMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtBndCfm,          /* 0 - loosely coupled */
#else
   PtUiEgtBndCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtBndCfm,          /* 1 - loosely coupled */
#else
   PtUiEgtBndCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtBndCfm,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtBndCfm,          /* 1 - tightly coupled, portable */
#endif
};

#ifdef EGTP_C
/* eGTP Signal Indication primitive */
PRIVATE CONSTANT EgtSigInd EgUiEgtSigIndMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtSigInd,          /* 0 - loosely coupled */
#else
   PtUiEgtSigInd,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtSigInd,          /* 1 - LW loosely coupled */
#else
   PtUiEgtSigInd,          /* 1 - LW loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtSigInd,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtSigInd,          /* 1 - tightly coupled, portable */
#endif
};

/* eGTP Signal Confirm primitive */
PRIVATE CONSTANT EgtSigCfm EgUiEgtSigCfmMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtSigCfm,          /* 0 - loosely coupled */
#else
   PtUiEgtSigCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtSigCfm,          /* 1 - loosely coupled */
#else
   PtUiEgtSigCfm,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtSigCfm,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtSigCfm,          /* 1 - tightly coupled, portable */
#endif
};

/* Error Indication primitive */
PRIVATE CONSTANT EgtErrInd EgUiEgtErrIndMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtErrInd,          /* 0 - loosely coupled */
#else
   PtUiEgtErrInd,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtErrInd,          /* 1 - loosely coupled */
#else
   PtUiEgtErrInd,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtErrInd,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtErrInd,          /* 1 - tightly coupled, portable */
#endif
};

/* Flow Status Indication primitive */
PRIVATE CONSTANT EgtStaInd EgUiEgtStaIndMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtStaInd,          /* 0 - loosely coupled */
#else
   PtUiEgtStaInd,          /* 0 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtStaInd,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtStaInd,          /* 1 - tightly coupled, portable */
#endif
};

/* Remove Status Confirm primitive */
PRIVATE CONSTANT EgtStaCfm EgUiEgtStaCfmMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtStaCfm,          /* 0 - loosely coupled */
#else
   PtUiEgtStaCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtStaCfm,          /* 1 - loosely coupled */
#else
   PtUiEgtStaCfm,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtStaCfm,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtStaCfm,          /* 1 - tightly coupled, portable */
#endif
};

/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtLclDelCfm EgUiEgtLclDelCfmMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtLclDelCfm,          /* 0 - loosely coupled */
#else
   PtUiEgtLclDelCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtLclDelCfm,          /* 1 - loosely coupled */
#else
   PtUiEgtLclDelCfm,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtLclDelCfm,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtLclDelCfm,          /* 1 - tightly coupled, portable */
#endif
};
PRIVATE CONSTANT EgtTunnModCfm EgUiEgtTunnModCfmMt [EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtTunnModCfm,         /* 0 - loosely coupled */
#else
   PtUiEgtTunnModCfm,         /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtTunnModCfm,         /* 1 - loosely coupled */
#else
   PtUiEgtTunnModCfm,         /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtTunnModCfm,         /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtTunnModCfm,         /* 1 - tightly coupled, portable */
#endif
};

#endif /* EGTP_C */
#ifdef EGTP_U

/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguLclTnlMgmtCfm EgUiEgtEguLclTnlMgmtCfmMt[EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguLclTnlMgmtCfm,          /* 0 - loosely coupled */
#else
   PtUiEgtEguLclTnlMgmtCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguLclTnlMgmtCfm,          /* 1 - loosely coupled */
#else
   PtUiEgtEguLclTnlMgmtCfm,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtEguLclTnlMgmtCfm,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtEguLclTnlMgmtCfm,          /* 1 - tightly coupled, portable */
#endif
};


/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguStaCfm EgUiEgtEguStaCfmMt[EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguStaCfm,          /* 0 - loosely coupled */
#else
   PtUiEgtEguStaCfm,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguStaCfm,          /* 1 - loosely coupled */
#else
   PtUiEgtEguStaCfm,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtEguStaCfm,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtEguStaCfm,          /* 1 - tightly coupled, portable */
#endif
};

/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguUStaInd EgUiEgtEguUStaIndMt[EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguUStaInd,          /* 0 - loosely coupled */
#else
   PtUiEgtEguUStaInd,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguUStaInd,          /* 1 - loosely coupled */
#else
   PtUiEgtEguUStaInd,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtEguUStaInd,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtEguUStaInd,          /* 1 - tightly coupled, portable */
#endif
};



PRIVATE CONSTANT EgtEguErrInd EgUiEgtEguErrIndMt[EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguErrInd,          /* 0 - loosely coupled */
#else
   PtUiEgtEguErrInd,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguErrInd,          /* 1 - loosely coupled */
#else
   PtUiEgtEguErrInd,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtEguErrInd,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtEguErrInd,          /* 1 - tightly coupled, portable */
#endif
};

PRIVATE CONSTANT EgtEguDatInd EgUiEgtEguDatIndMt[EG_MAX_UIEGT_SEL] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguDatInd,          /* 0 - loosely coupled */
#else
   PtUiEgtEguDatInd,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguDatInd,          /* 1 - loosely coupled */
#else
   PtUiEgtEguDatInd,          /* 1 - loosely coupled, portable */
#endif
#ifdef EU
   EuLiEgtEguDatInd,          /* 1 - tightly coupled, service user EU */
#else
   PtUiEgtEguDatInd,          /* 1 - tightly coupled, portable */
#endif
};

#endif /* EGTP_U */















/* Primitive Mapping Dispatching Functions */


/*
*
*       Fun:   EgUiEgtBndCfm
*
*       Desc:  This function resolves the EgtBndCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtBndCfm
(
Pst  *pst,              /* post structure */
SuId suId,              /* service user id */
U8   status             /* status of bind request */
)
#else
PUBLIC S16 EgUiEgtBndCfm (pst, suId, status)
Pst   *pst;              /* post structure */
SuId  suId;              /* service user id */
U8    status;            /* status of bind request */
#endif
{
   TRC3(EgUiEgtBndCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtBndCfm(pst, suId(%d), status(%d))\n", suId, status)); 

   (*EgUiEgtBndCfmMt[pst->selector])(pst, suId, status);

   RETVALUE(ROK);
} /* end of EgUiEgtBndCfm */

#ifdef EGTP_C

/*
*
*       Fun:   EgUiEgtSigInd
*
*       Desc:  This function resolves the EgtSigInd primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtSigInd
(
Pst         *pst,       /* post structure */
SuId        suId,       /* eGTP service user SAP ID */
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
EgMsg       *egMsg      /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EgUiEgtSigInd (pst, suId, localAddr, remAddr, lclTeid, egMsg)
Pst         *pst;       /* post structure */
SuId        suId;       /* eGTP service user SAP ID */
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
EgMsg       *egMsg;      /* structure for carrying eGTP MSG info */
#endif
{
   TRC3(EgUiEgtSigInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
        "EgUiEgtSigInd(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtSigIndMt[pst->selector])(pst, suId, localAddr, remAddr, lclTeid, egMsg);

   RETVALUE(ROK);
} /* end of EgUiEgtSigInd */


/*
*
*       Fun:   EgUiEgtSigCfm
*
*       Desc:  This function resolves the EgtSigCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtSigCfm
(
Pst         *pst,              /* post structure */
SuId        suId,              /* service user id */
CmTptAddr   *localAddr,/* Local node address */
CmTptAddr   *remAddr,  /* Remote node address */
TknU32      transId,   /* Transaction Identifier */
TknU32      lclTeid,   /* Local Tunnel Identifier */
EgMsg       *egMsg     /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 EgUiEgtSigCfm (pst, suId, localAddr, remAddr, transId, lclTeid, egMsg)
Pst         *pst;              /* post structure */
SuId        suId;              /* service user id */
CmTptAddr   *localAddr;/* Local node address */
CmTptAddr   *remAddr;  /* Remote node address */
TknU32      transId;   /* Transaction Identifier */
TknU32      lclTeid;   /* Local Tunnel Identifier */
EgMsg       *egMsg;     /* structure for carrying eGTP MSG info */
#endif
{
   TRC3(EgUiEgtSigCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtSigCfm(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtSigCfmMt[pst->selector])(pst, suId, localAddr, remAddr, transId, lclTeid, egMsg);

   RETVALUE(ROK);
} /* end of EgUiEgtSigCfm */


/*
*
*       Fun:   EgUiEgtErrInd
*
*       Desc:  This function resolves the EgtErrInd primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtErrInd
(
Pst         *pst,              /* post structure */
SuId        suId,              /* service user id */
EgErrEvnt   *errEvnt           /* error event */
)
#else
PUBLIC S16 EgUiEgtErrInd (pst, suId, errEvnt)
Pst         *pst;              /* post structure */
SuId        suId;              /* service user id */
EgErrEvnt   *errEvnt;          /* error event */
#endif
{
   TRC3(EgUiEgtErrInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtErrInd(pst, suId(%d), errorCause(%d), ieType(%d))\n",
          suId, errEvnt->errCause, errEvnt->failedIeType));

   (*EgUiEgtErrIndMt[pst->selector])(pst, suId, errEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtErrInd */


/*
*
*       Fun:   EgUiEgtStaCfm
*
*       Desc:  This function resolves the EgtStaCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtStaCfm
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U8           eventType,    /* Status event type */
EgPathInfo   *pathInfo,    /* path information */
CmStatus     status   /* Status of request primitive */
)
#else
PUBLIC S16 EgUiEgtStaCfm (pst, suId, eventType, pathInfo,status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U8           eventType;    /* Status event type */
EgPathInfo   *pathInfo;    /* path information */
CmStatus     status;   /* Status of request primitive */
#endif
{
   TRC3(EgUiEgtStaCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaCfm(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtStaCfmMt[pst->selector])(pst, suId, eventType, pathInfo,status);

   RETVALUE(ROK);
} /* end of EgUiEgtStaCfm */


/*
*
*       Fun:   EgUiEgtStaInd
*
*       Desc:  This function resolves the EgtStaInd primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtStaInd
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U8           eventType,  /* Status event type */
EgPathStatus *status /* Status structure */
)
#else
PUBLIC S16 EgUiEgtStaInd (pst, suId, eventType, status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U8           eventType;  /* Status event type */
EgPathStatus *status; /* Status structure */
#endif
{

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaInd(pst, suId(%d) ....)\n", suId));

   (*EgUiEgtStaIndMt[pst->selector])(pst, suId, eventType, status);

   RETVALUE(ROK);
} /* end of EgUiEgtStaInd */




/*
*
*       Fun:   EgUiEgtLclDelCfm
*
*       Desc:  This function resolves the EgtLclDelCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtLclDelCfm
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U32          transId,           /* Transaction ID */
U32          teid,      /* self TEID */
CmStatus     status   /* Status of request primitive */
)
#else
PUBLIC S16 EgUiEgtLclDelCfm (pst, suId, transId, teid, status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U32          transId;           /* Transaction ID */
U32          teid;      /* self TEID */
CmStatus     status;   /* Status of request primitive */
#endif
{

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtLclDelCfm(pst, suId(%d) ....)\n", suId));

   (*EgUiEgtLclDelCfmMt[pst->selector])(pst, suId, transId, teid, status);

   RETVALUE(ROK);
} /* end of EgUiEgtLclDelCfm */

/*
*
*       Fun:   EgUiEgtTunnModCfm
*
*       Desc:  This function resolves the EgtTunnModCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtTunnModCfm
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U32          transId,           /* Transaction ID */
U32          teid,              /* self TEID */
CmTptAddr    oldDstIpAddr,
CmTptAddr    newDstIpAddr,
CmStatus     status             /* Status of request primitive */
)
#else
PUBLIC S16 EgUiEgtTunnModCfm(pst, suId, transId, teid, oldDstIpAddr, newDstIpAddr, status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U32          transId;           /* Transaction ID */
U32          teid;              /* self TEID */
CmTptAddr    oldDstIpAddr;
CmTptAddr    newDstIpAddr;
CmStatus     status;            /* Status of request primitive */
#endif
{

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtTunnModCfm(pst, suId(%d) ....)\n", suId));

   (*EgUiEgtTunnModCfmMt[pst->selector])(pst, suId, transId, teid, oldDstIpAddr, newDstIpAddr, status);

   RETVALUE(ROK);
} /* end of EgUiEgtTunnModCfm */

#endif /* EGTP_C */

#ifdef EGTP_U

/*
*
*       Fun: EgUiEgtEguLclTnlMgmtCfm 
*
*       Desc:  This function resolves the EgtStaCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguLclTnlMgmtCfm 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EgUiEgtEguLclTnlMgmtCfm(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(EgUiEgtEguLclTnlMgmtCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaCfm(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtEguLclTnlMgmtCfmMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguLclTnlMgmtCfm() */

/*
*
*       Fun: EgUiEgtEguStaCfm 
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguStaCfm 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EgUiEgtEguStaCfm(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(EgUiEgtStaCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaCfm(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtEguStaCfmMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  EgUiEgtEguUStaInd
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguUStaInd 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EgUiEgtEguUStaInd(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(EgUiEgtEguUStaInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtEguUStaInd(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtEguUStaIndMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  EgUiEgtEguErrInd
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguErrInd
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EgUiEgtEguErrInd(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(EgUiEgtEguErrInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtEguErrInd(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtEguErrIndMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguErrInd() */

/*
*
*       Fun:  EgUiEgtEguDatInd
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 EgUiEgtEguDatInd 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EgUiEgtEguDatInd(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
#ifdef TIME_MES
#ifdef SS_MT_CB
   if(exitRx[pst->srcInst-1].call < MAX_MSG)
   {
      clock_gettime(0, &exitRx[pst->srcInst-1].time[exitRx[pst->srcInst-1].call++]);
   }
#else
   PRIVATE U32 call;
   if(call < MAX_MSG)
   {
      clock_gettime(0, &exitRx2[call]);
      call ++;
   }
#endif
#endif
   TRC3(EgUiEgtEguDatInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtEguDatInd(pst, suId(%d) ...)\n", suId));

   (*EgUiEgtEguDatIndMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguDatInd() */

#endif /* EGTP_U */

#ifdef PTEGUIEGT


/*
*
*       Fun:   PtUiEgtBndCfm
*
*       Desc:  portable version of EgtBndCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtBndCfm
(
Pst  *pst,              /* post structure */
SuId suId,              /* service user id */
U8   status             /* status of bind request */
)
#else
PUBLIC S16 PtUiEgtBndCfm (pst, suId, status)
Pst   *pst;              /* post structure */
SuId  suId;              /* service user id */
U8    status;            /* status of bind request */
#endif
{
   TRC3(PtUiEgtBndCfm);

   EGLOGERROR(ERRCLS_DEBUG, EEG015, ERRZERO, "PtUiEgtBndCfm: Invalid selector");

   UNUSED(suId);
   UNUSED(status);

   RETVALUE(ROK);
} /* end of PtUiEgtBndCfm */

#ifdef EGTP_C

/*
*
*       Fun:   PtUiEgtSigInd
*
*       Desc:  portable version of EgtSigInd primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtSigInd
(
Pst         *pst,       /* post structure */
SuId        suId,       /* eGTP service user SAP ID */
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
EgMsg       *egMsg      /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 PtUiEgtSigInd (pst, suId, localAddr, remAddr, lclTeid, egMsg)
Pst         *pst;       /* post structure */
SuId        suId;       /* eGTP service user SAP ID */
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
EgMsg       *egMsg;      /* structure for carrying eGTP MSG info */
#endif
{
   TRC3(PtUiEgtSigInd);

   EGLOGERROR(ERRCLS_DEBUG, EEG016, ERRZERO, "PtUiEgtSigInd: Invalid selector");

   UNUSED(suId);
   UNUSED(localAddr);
   UNUSED(remAddr);
   UNUSED(lclTeid);
   UNUSED(egMsg);

   RETVALUE(ROK);
} /* end of PtUiEgtSigInd */


/*
*
*       Fun:   PtUiEgtSigCfm
*
*       Desc:  portable version of EgtSigCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtSigCfm
(
Pst         *pst,              /* post structure */
SuId        suId,              /* service user id */
CmTptAddr   *localAddr,/* Local node address */
CmTptAddr   *remAddr,  /* Remote node address */
TknU32      transId,   /* Transaction Identifier */
TknU32      lclTeid,   /* Local Tunnel Identifier */
EgMsg       *egMsg     /* structure for carrying eGTP MSG info */
)
#else
PUBLIC S16 PtUiEgtSigCfm (pst, suId, localAddr, remAddr, transId, lclTeid, egMsg)
Pst         *pst;              /* post structure */
SuId        suId;              /* service user id */
CmTptAddr   *localAddr;/* Local node address */
CmTptAddr   *remAddr;  /* Remote node address */
TknU32      transId;   /* Transaction Identifier */
TknU32      lclTeid;   /* Local Tunnel Identifier */
EgMsg       *egMsg;     /* structure for carrying eGTP MSG info */
#endif
{
   TRC3(PtUiEgtSigCfm);

   EGLOGERROR(ERRCLS_DEBUG, EEG017, ERRZERO, "PtUiEgtSigCfm: Invalid selector");

   UNUSED(suId);
   UNUSED(localAddr);
   UNUSED(remAddr);
   UNUSED(transId);
   UNUSED(lclTeid);
   UNUSED(egMsg);

   RETVALUE(ROK);
} /* end of PtUiEgtSigCfm */

/*
*
*       Fun:   PtUiEgtErrInd
*
*       Desc:  portable version of EgtErrInd primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtErrInd
(
Pst         *pst,              /* post structure */
SuId        suId,              /* service user id */
EgErrEvnt   *errEvnt           /* error event */
)
#else
PUBLIC S16 PtUiEgtErrInd (pst, suId, errEvnt)
Pst         *pst;              /* post structure */
SuId        suId;              /* service user id */
EgErrEvnt   *errEvnt;          /* error event */
#endif
{
   TRC3(PtUiEgtErrInd);

   EGLOGERROR(ERRCLS_DEBUG, EEG018, ERRZERO, "PtUiEgtErrInd: Invalid selector");

   UNUSED(suId);
   UNUSED(errEvnt);

   RETVALUE(ROK);
} /* end of PtUiEgtErrInd */

/*
*
*       Fun:   PtUiEgtStaCfm
*
*       Desc:  portable version of EgtStaCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtStaCfm
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U8           eventType,    /* Status event type */
EgPathInfo   *pathInfo,    /* path information */
CmStatus     status        /* Status of request primitive */
)
#else
PUBLIC S16 PtUiEgtStaCfm (pst, suId, eventType, pathInfo,status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U8           eventType;    /* Status event type */
EgPathInfo   *pathInfo;    /* path information */
CmStatus     status;       /* Status of request primitive */
#endif
{
   TRC3(PtUiEgtStaCfm);

   EGLOGERROR(ERRCLS_DEBUG, EEG019, ERRZERO, "PtUiEgtStaCfm: Invalid selector");

   UNUSED(suId);
   UNUSED(eventType);
   UNUSED(pathInfo);
   UNUSED(status);

   RETVALUE(ROK);
} /* end of PtUiEgtStaCfm */

/*
*
*       Fun:   PtUiEgtStaInd
*
*       Desc:  portable version of EgtStaInd primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtStaInd
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U8           eventType,  /* Status event type */
EgPathStatus *status /* Status structure */
)
#else
PUBLIC S16 PtUiEgtStaInd (pst, suId, eventType, status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U8           eventType;  /* Status event type */
EgPathStatus *status; /* Status structure */
#endif
{
   TRC3(PtUiEgtStaInd);

   EGLOGERROR(ERRCLS_DEBUG, EEG020, ERRZERO, "PtUiEgtStaInd: Invalid selector");

   UNUSED(suId);
   UNUSED(eventType);
   UNUSED(status);

   RETVALUE(ROK);
} /* end of PtUiEgtStaInd */


/*
*
*       Fun:   PtUiEgtLclDelCfm
*
*       Desc:  This function resolves the EgtLclDelCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtLclDelCfm
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
U32          transId,    /* Transaction ID */
U32          teid,      /* self TEID */
CmStatus     status   /* Status of request primitive */
)
#else
PUBLIC S16 PtUiEgtLclDelCfm (pst, suId, transId, teid, status)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
U32          transId;    /* Transaction ID */
U32          teid;      /* self TEID */
CmStatus     status;   /* Status of request primitive */
#endif
{

   TRC3(PtUiEgtLclDelCfm);

   EGLOGERROR(ERRCLS_DEBUG, EEG021, ERRZERO, "PtUiEgtLclDelCfm: Invalid selector");

   UNUSED(suId);
   UNUSED(transId);
   UNUSED(teid);
   UNUSED(status);

   RETVALUE(ROK);
} /* end of PtUiEgtLclDelCfm */

/*
*
*       Fun:   PtUiEgtTunnModCfm
*
*       Desc:  This function resolves the EgtTunnModCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtTunnModCfm
(
Pst          *pst,        /* post structure */
SuId         suId,        /* service user id */
U32          transId,     /* Transaction ID */
U32          teid,        /* self TEID */
CmTptAddr    oldDstIpAddr,
CmTptAddr    newDstIpAddr,
CmStatus     status       /* Status of request primitive */
)
#else
PUBLIC S16 PtUiEgtTunnModCfm(pst, suId, transId, teid, oldDstIpAddr, newDstIpAddr, status)
Pst          *pst;        /* post structure */
SuId         suId;        /* service user id */
U32          transId;     /* Transaction ID */
U32          teid;        /* self TEID */
CmTptAddr    oldDstIpAddr;
CmTptAddr    newDstIpAddr;
CmStatus     status;      /* Status of request primitive */
#endif
{

   TRC3(PtUiEgtTunnModCfm);

   EGLOGERROR(ERRCLS_DEBUG, EEG021, ERRZERO, "PtUiEgtTunnModCfm: Invalid selector");

   UNUSED(suId);
   UNUSED(transId);
   UNUSED(teid);
   UNUSED(status);
   UNUSED(oldDstIpAddr);
   UNUSED(newDstIpAddr);

   RETVALUE(ROK);
} /* end of PtUiEgtTunnModCfm */

#endif /* EGTP_C */
#ifdef EGTP_U

/*
*
*       Fun:  PtUiEgtEguTnlMgmtCfm 
*
*       Desc:  This function resolves the EgtStaCfm primitive
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtEguLclTnlMgmtCfm 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtUiEgtEguLclTnlMgmtCfm(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtUiEgtEguLclTnlMgmtCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaCfm(pst, suId(%d) ...)\n", suId));

   RETVALUE(ROK);
} /* end of PtUiEgtEguLclTnlMgmtCfm() */

/*
*
*       Fun: PtUiEgtEguStaCfm 
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtEguStaCfm 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtUiEgtEguStaCfm(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(EgUiEgtStaCfm);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaCfm(pst, suId(%d) ...)\n", suId));

   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  PtUiEgtEguUStaInd
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtEguUStaInd 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtUiEgtEguUStaInd(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtUiEgtEguUStaInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "PtUiEgtEguUStaInd(pst, suId(%d) ...)\n", suId));


   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  PtUiEgtEguErrInd
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtEguErrInd
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtUiEgtEguErrInd(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtUiEgtEguErrInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "PtUiEgtEguErrInd(pst, suId(%d) ...)\n", suId));

   RETVALUE(ROK);
} /* end of EgUiEgtEguErrInd() */


/*
*
*       Fun:  PtUiEgtEguDatInd 
*
*       Desc:  
*
*       Ret:  ROK -      ok
*
*       Notes: none
*
*       File:  eg_ptui.c
*
*/
#ifdef ANSI
PUBLIC S16 PtUiEgtEguDatInd 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtUiEgtEguDatInd(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   TRC3(PtUiEgtEguDatInd);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "PtUiEgtEguDatInd(pst, suId(%d) ...)\n", suId));

   RETVALUE(ROK);
} /* end of PtUiEgtEguDatInd() */

#endif /* EGTP_U */
#endif /* PTEGUIEGT */

/********************************************************************30**

         End of file:     eg_ptui.c@@/main/3 - Fri Feb  4 18:18:12 2011

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
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      nkottapalli       1. Initial for eGTP 1.2 Release
/main/2      eg001.102    snatarajan        1. EGTP_C Path management functionality is introduced. 
                          rss                 2. Multithreaded performance changes gaurded under SS_MT_CB flag.
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3     eg004.201 magnihotri        1. Header files added for eGTP-C PSF
*********************************************************************91*/
