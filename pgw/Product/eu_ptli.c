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

     Name:     acc test file

     Type:    C Include file

     Desc:    This file contains the acc source code

     File:    eu_ptli.c

     Sid:      eu_ptli.c@@/main/3 - Fri Feb  4 18:18:21 2011

     Prg:     an 
*********************************************************************21*/

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"         /* common DNS libraru defines */

#include "leg.h"           /* layer management defines for EGCP */
#include "eg_edm.h"        /* EDM Module structures            */
#include "eg.h"            /* defines and macros for EG */
#include "egt.h"            /* defines and macros for EG */
#include "eg_err.h"        /* EG error defines */
#include "egac_acc.h"        /* defines for EG acceptance tests */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#include "hi.h"
#endif

#include "cm_xta.h" 


/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"         /* common DNS libraru defines */

#include "egt.x"           /* EGT Interface Structures */
#include "leg.x"           /* layer management typedefs for EGCP */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#include "hi.x"
#endif
/*eg004.201 Header files added for eGTPC PSF Upgrade*/
/* eg014.201 - ccpu00139510 Egtpc release 10 upgrade */
#ifdef HW
#include "cm_ftha.h"
#include "cm_psf.h"
#include "cm_psfft.h"
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_psf.x"

#endif

#include "cm_xta.x" 
#include "eg_edm.x"        /* EDM Module structures            */
#include "eg.x"            /* typedefs for EGCP */
#include "eu.x"            /* typedefs for EGCP */
#include "egac_acc.x"        /* typedefs for EGCP acceptance tests */

/*eg004.201 Header files added for eGTPC PSF Upgrade*/
#ifdef HW
#include "lhw.x"
#include "hw.x"
#endif



#if (!defined(LWLCEULIEGT) || !defined(LCEULIEGT))
#define PTEULIEGT
#else
#ifndef EG
#define PTEULIEGT
#else
#endif
#endif

#if (!defined(LCEULIHIT))
#define PTEULIHIT
#else
#ifndef HI
#define PTEULIHIT
#else
#endif
#endif


/* ---- eGTP service user lower interface ---- */
/*
 *     this software may be combined with the following TRILLIUM
 *     software:
 *
 *     part no.                      description
 *     --------    ----------------------------------------------
 *     1000XXX                      eGTP v 1.1
 */


/************************************************************************
                              Prototypes  
************************************************************************/

#ifdef PTEULIEGT

PRIVATE S16 PtLiEgtBndReq       ARGS((Pst *pst, SuId suId, SpId spId));
PRIVATE S16 PtLiEgtUbndReq      ARGS((Pst *pst, SpId spId, Reason reason));
#ifdef EGTP_C
PRIVATE S16 PtLiEgtSigReq       ARGS((Pst *pst, SpId spId,                
               CmTptAddr   *localAddr, /* Local node address */
               CmTptAddr   *remAddr,   /* Remote node address */
               TknU32      lclTeid,    /* Local Tunnel Identifier */
               TknU32      transId,    /* Transaction Identifier */
               TknU8       crtFlag,    /* Create tunnel or not */
               TmrCfg      T3,         /* Request timer expiry */
               EgMsg    *egMsg));

PRIVATE S16 PtLiEgtTransSigReq       ARGS((Pst *pst, SpId spId,                
               CmTptAddr   *localAddr, /* Local node address */
               CmTptAddr   *remAddr,   /* Remote node address */
               TknU32      lclTeid,    /* Local Tunnel Identifier */
               TknU32      transId,    /* Transaction Identifier */
               TknU8       crtFlag,    /* Create tunnel or not */
               TmrCfg      T3,         /* Request timer expiry */
               EgMsg    *egMsg));

PRIVATE S16 PtLiEgtSigRsp       ARGS((Pst *pst, SpId spId, 
               TknU32      lclTeid,   /* Local TEID */
               EgMsg    *egMsg));

PRIVATE S16 PtLiEgtTransSigRsp       ARGS((Pst *pst, SpId spId, 
               TknU32      lclTeid,   /* Local TEID */
               EgMsg    *egMsg));


PRIVATE S16 PtLiEgtStaReq       ARGS((Pst *pst, SpId spId, 
               U8      eventType,   /* Local TEID */
               EgPathInfo   *pathInfo));

PRIVATE S16 PtLiEgtLclDelReq       ARGS((Pst *pst, SpId spId, 
               U32      transId,   /* Local TEID */
               U32   teid));
#endif /* EGTP_C */
#ifdef EGTP_U
PUBLIC S16 PtLiEgtEguLclTnlMgmtReq ARGS((
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
));

PUBLIC S16 PtLiEgtEguStaReq ARGS((
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
));

PUBLIC S16 PtLiEgtEguDatReq ARGS((
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
));
#endif /* EGTP_U */
#endif


/* Following matrices define the mapping between the primitives called by the
 * eGTP user (as EuLiEgtXXXReq) and the corresponding primitives of the 
 * eGTP layer (service provider) (as EgUiEgtXXXReq).
 * The mapping is based on pst->selector.
 * Selector          #define     Coupling      Primitive
 * 0 (SEL_LC_NEW)   LCEULIEGT    loose         cmPkEgtXXXReq
 * 1                 EG          tight         EgUiEgtXXXReq
 * 2                LWLCEULIEGT  LW loose      cmPkEgtXXXReq
 */


PUBLIC EgtBndReq EuLiEgtBndReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtBndReq,        /* 0 - loosely coupled */
#else
   PtLiEgtBndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtBndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtBndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtBndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtBndReq,        /* 2 - tightly coupled, portable */
#endif
};


PUBLIC EgtUbndReq EuLiEgtUbndReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtUbndReq,        /* 0 - loosely coupled */
#else
   PtLiEgtUbndReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtUbndReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtUbndReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtUbndReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtUbndReq,        /* 2 - tightly coupled, portable */
#endif
};


#ifdef EGTP_C
PUBLIC EgtSigReq EuLiEgtSigReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtSigReq,        /* 0 - loosely coupled */
#else
   PtLiEgtSigReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtSigReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtSigReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtSigReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtSigReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtTransSigReq EuLiEgtTransSigReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtTransSigReq,        /* 0 - loosely coupled */
#else
   PtLiEgtTransSigReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtTransSigReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtTransSigReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtTransSigReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtTransSigReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtSigRsp EuLiEgtSigRspMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtSigRsp,        /* 0 - loosely coupled */
#else
   PtLiEgtSigRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtSigRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtSigRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtSigRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtSigRsp,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtTransSigRsp EuLiEgtTransSigRspMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtTransSigRsp,        /* 0 - loosely coupled */
#else
   PtLiEgtTransSigRsp,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtTransSigRsp,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtTransSigRsp,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtTransSigRsp,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtTransSigRsp,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtStaReq EuLiEgtStaReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtStaReq,        /* 0 - loosely coupled */
#else
   PtLiEgtStaReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtStaReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtStaReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtStaReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtStaReq,        /* 2 - tightly coupled, portable */
#endif
};

PUBLIC EgtLclDelReq EuLiEgtLclDelReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtLclDelReq,        /* 0 - loosely coupled */
#else
   PtLiEgtLclDelReq,        /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtLclDelReq,        /* 1 - light weight loosely coupled */
#else
   PtLiEgtLclDelReq,        /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtLclDelReq,        /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtLclDelReq,        /* 2 - tightly coupled, portable */
#endif
};

#ifdef EGTP_C_PTH_MGT
PUBLIC EgtTunnModReq EuLiEgtTunnModReqMt [] =
{
#ifdef LCEULIEGT
   cmPkEgtTunnModReq,       /* 0 - loosely coupled */
#else
   PtLiEgtTunnModReq,       /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEULIEGT
   cmPkEgtTunnModReq,       /* 1 - light weight loosely coupled */
#else
   PtLiEgtTunnModReq,       /* 1 - light weight loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtTunnModReq,       /* 2 - tightly coupled, stub layer */
#else
   PtLiEgtTunnModReq,       /* 2 - tightly coupled, portable */
#endif
};
#endif

#endif /* EGTP_C */

#ifdef EGTP_U

/* Delete Local Tunnel primitive */
PUBLIC EgtEguLclTnlMgmtReq EuLiEgtEguLclTnlMgmtReqMt[] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguLclTnlMgmtReq,          /* 0 - loosely coupled */
#else
   PtLiEgtEguLclTnlMgmtReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguLclTnlMgmtReq,          /* 1 - loosely coupled */
#else
   PtLiEgtEguLclTnlMgmtReq,          /* 1 - loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtEguLclTnlMgmtReq,          /* 1 - tightly coupled, service user EU */
#else
   PtLiEgtEguLclTnlMgmtReq,          /* 1 - tightly coupled, portable */
#endif
};


/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguStaReq EuLiEgtEguStaReqMt[] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguStaReq,          /* 0 - loosely coupled */
#else
   PtLiEgtEguStaReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguStaReq,          /* 1 - loosely coupled */
#else
   PtLiEgtEguStaReq,          /* 1 - loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtEguStaReq,          /* 1 - tightly coupled, service user EU */
#else
   PtLiEgtEguStaReq,          /* 1 - tightly coupled, portable */
#endif
};

/* Delete Local Tunnel primitive */
PRIVATE CONSTANT EgtEguDatReq EuLiEgtEguDatReqMt[] =
{
#ifdef LCEGUIEGT
   cmPkEgtEguDatReq,          /* 0 - loosely coupled */
#else
   PtLiEgtEguDatReq,          /* 0 - loosely coupled, portable */
#endif
#ifdef LWLCEGUIEGT
   cmPkEgtEguDatReq,          /* 1 - loosely coupled */
#else
   PtLiEgtEguDatReq,          /* 1 - loosely coupled, portable */
#endif
#ifdef EG
   EgUiEgtEguDatReq,          /* 1 - tightly coupled, service user EU */
#else
   PtLiEgtEguDatReq,          /* 1 - tightly coupled, portable */
#endif
};

#endif /* EGTP_U */


/************************************************************************
                       EGT Interface Functions
************************************************************************/

/*
 *
 *       Fun:   EuLiEgtBndReq
 *
 *       Desc:  This function resolves the EgtBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PUBLIC S16 EuLiEgtBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtBndReq)

   (*EuLiEgtBndReqMt[pst->selector])
      (pst, suId, spId);

   RETVALUE(ROK);
} /* EuLiEgtBndReq */

/*
 *
 *       Fun:   EuLiEgtUbndReq
 *
 *       Desc:  This function resolves the EgtUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PUBLIC S16 EuLiEgtUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtUbndReq)

   (*EuLiEgtUbndReqMt[pst->selector])
      (pst, spId, reason);

   RETVALUE(ROK);
} /* EuLiEgtUbndReq */

#ifdef EGTP_C
/*
 *
 *       Fun:   EuLiEgtSigReq
 *
 *       Desc:  This function resolves the EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtSigReq
(
Pst         *pst,
SpId        spId,
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
TknU32      transId,    /* Transaction Identifier */
TknU8       crtFlag,    /* Create tunnel or not */
TmrCfg      T3,         /* Request timer expiry */
EgMsg       *egMsg
)
#else 
PUBLIC S16 EuLiEgtSigReq (pst, spId, localAddr, remAddr, lclTeid, transId, 
                          crtFlag, T3, egMsg)
Pst         *pst;
SpId        spId;
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
TknU32      transId;    /* Transaction Identifier */
TknU8       crtFlag;    /* Create tunnel or not */
TmrCfg      T3;         /* Request timer expiry */
EgMsg       *egMsg;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtSigReq)

   (*EuLiEgtSigReqMt[pst->selector])
      (pst, spId, localAddr,remAddr, lclTeid, transId, crtFlag, T3, egMsg);

   RETVALUE(ROK);
} /* EuLiEgtSigReq */

/*
 *
 *       Fun:   EuLiEgtTransSigReq
 *
 *       Desc:  This function resolves the EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtTransSigReq
(
Pst         *pst,
SpId        spId,
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
TknU32      transId,    /* Transaction Identifier */
TmrCfg      T3,         /* Request timer expiry */
EgMsg       *egMsg
)
#else 
PUBLIC S16 EuLiEgtTransSigReq (pst, spId, localAddr, remAddr, lclTeid, transId, 
                          crtFlag, T3, egMsg)
Pst         *pst;
SpId        spId;
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
TknU32      transId;    /* Transaction Identifier */
TmrCfg      T3;         /* Request timer expiry */
EgMsg       *egMsg;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtSigReq)

   (*EuLiEgtTransSigReqMt[pst->selector])
      (pst, spId, localAddr,remAddr, lclTeid, transId, T3, egMsg);

   RETVALUE(ROK);
} /* EuLiEgtSigReq */

/*
 *
 *       Fun:   EuLiEgtSigRsp
 *
 *       Desc:  This function resolves the EgtConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtSigRsp
(
Pst *pst,
SpId spId,
TknU32      lclTeid,   /* Local TEID */
EgMsg    *egMsg
)
#else 
PUBLIC S16 EuLiEgtSigRsp (pst, spId, lclTeid, egMsg)
Pst *pst;
SpId spId;
TknU32      lclTeid;   /* Local TEID */
EgMsg    *egMsg;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtSigRsp)

   (*EuLiEgtSigRspMt[pst->selector])
      (pst, spId, lclTeid, egMsg);

   RETVALUE(ROK);
} /* EuLiEgtSigRsp */


/*
 *
 *       Fun:   EuLiEgtTransSigRsp
 *
 *       Desc:  This function resolves the EgtConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtTransSigRsp
(
Pst         *pst,
SpId        spId,
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
TknU32      transId,    /* Transaction Identifier */
EgMsg       *egMsg
)
#else 
PUBLIC S16 EuLiEgtTransSigRsp (pst, spId, localAddr, remAddr, lclTeid, transId, 
                          crtFlag, T3, egMsg)
Pst         *pst;
SpId        spId;
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
TknU32      transId;    /* Transaction Identifier */
EgMsg       *egMsg;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtSigRsp)

   (*EuLiEgtTransSigRspMt[pst->selector])
      (pst, spId, localAddr,remAddr, lclTeid, transId, egMsg);

   RETVALUE(ROK);
} /* EuLiEgtSigRsp */

/*
 *
 *       Fun:   EuLiEgtStaReq
 *
 *       Desc:  This function resolves the EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtStaReq
(
Pst *pst,
SpId spId,
U8       eventType,         /* Request timer expiry */
EgPathInfo    *pathInfo
)
#else 
PUBLIC S16 EuLiEgtStaReq (pst, spId, eventType, pathInfo)
Pst *pst;
SpId spId;
U8       eventType;         /* Request timer expiry */
EgPathInfo    *pathInfo;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtStaReq)

   (*EuLiEgtStaReqMt[pst->selector])
      (pst, spId, eventType, pathInfo);

   RETVALUE(ROK);
} /* EuLiEgtStaReq */


/*
 *
 *       Fun:   EuLiEgtLclDelReq
 *
 *       Desc:  This function resolves the EgtLclDelReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PUBLIC S16 EuLiEgtLclDelReq
(
Pst *pst,
SpId spId,
U32 transId, 
U32 teid
)
#else 
PUBLIC S16 EuLiEgtLclDelReq (pst, spId, transId, teid)
Pst *pst;
SpId spId;
U32 transId;
U32 teid;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtLclDelReq)

   (*EuLiEgtLclDelReqMt[pst->selector])
      (pst, spId, transId, teid);

   RETVALUE(ROK);
} /* EuLiEgtLclDelReq */


#endif /* EGTP_C */

#ifdef EGTP_U

/*
*
*       Fun: EuLiEgtEguLclTnlMgmtReq 
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
PUBLIC S16 EuLiEgtEguLclTnlMgmtReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EuLiEgtEguLclTnlMgmtReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtEguLclTnlMgmtReq);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EgUiEgtStaCfm(pst, suId(%d) ...)\n", suId));

   (*EuLiEgtEguLclTnlMgmtReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguLclTnlMgmtCfm() */

/*
*
*       Fun: EuLiEgtEguStaReq 
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
PUBLIC S16 EuLiEgtEguStaReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EuLiEgtEguStaReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtEguStaReq);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EuLiEgtEguStaReq(pst, suId(%d) ...)\n", suId));

   (*EuLiEgtEguStaReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  EuLiEgtEguDatReq
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
PUBLIC S16 EuLiEgtEguDatReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 EuLiEgtEguDatReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(EuLiEgtEguDatReq);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "EuLiEgtEguDatReq(pst, suId(%d) ...)\n", suId));

   (*EuLiEgtEguDatReqMt[pst->selector])(pst, suId, egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguDatInd() */

#endif /* EGTP_U */





/************************************************************************
                       Portable Functions
************************************************************************/

#ifdef PTEULIEGT
/*
 *
 *       Fun:   PtLiEgtBndReq
 *
 *       Desc:  Portable version of EgtBndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtBndReq
(
Pst *pst,
SuId suId,
SpId spId
)
#else 
PRIVATE S16 PtLiEgtBndReq (pst, suId, spId)
Pst *pst;
SuId suId; 
SpId spId;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtBndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   EGLOGERROR(ERRCLS_DEBUG, EEG076, (ErrVal) 0, 
              "PtLiEgtBndReq() called");
#endif
   UNUSED(pst);
   UNUSED(suId);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiEgtBndReq */

/*
 *
 *       Fun:   PtLiEgtUbndReq
 *
 *       Desc:  Portable version of EgtUbndReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtUbndReq
(
Pst *pst,
SpId spId,
Reason reason
)
#else 
PRIVATE S16 PtLiEgtUbndReq (pst, spId, reason)
Pst *pst;
SpId spId;
Reason reason;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtUbndReq)

#if (ERRCLASS & ERRCLS_DEBUG)
   EGLOGERROR(ERRCLS_DEBUG, EEG077, (ErrVal) 0, 
              "PtLiEgtUbndReq() called");
#endif
   UNUSED(pst);
   UNUSED(spId);

   RETVALUE(RFAILED);
} /* PtLiEgtUbndReq */


#ifdef EGTP_C
/*
 *
 *       Fun:   PtLiEgtSigReq
 *
 *       Desc:  Portable version of EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtSigReq
(
Pst *pst,
SpId spId,
CmTptAddr   *localAddr, /* Local node address */
CmTptAddr   *remAddr,   /* Remote node address */
TknU32      lclTeid,    /* Local Tunnel Identifier */
TknU32      transId,    /* Transaction Identifier */
TknU8       crtFlag,    /* Create tunnel or not */
TmrCfg      T3,         /* Request timer expiry */
EgMsg    *egMsg
)
#else 
PRIVATE S16 PtLiEgtSigReq (pst, spId, localAddr, remAddr, lclTeid, transId,
                           crtFlag, T3, egMsg)
Pst *pst;
SpId spId;
CmTptAddr   *localAddr; /* Local node address */
CmTptAddr   *remAddr;   /* Remote node address */
TknU32      lclTeid;    /* Local Tunnel Identifier */
TknU32      transId;    /* Transaction Identifier */
TknU8       crtFlag;    /* Create tunnel or not */
TmrCfg      T3;         /* Request timer expiry */
EgMsg       *egMsg;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtSigReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(egMsg);

   RETVALUE(RFAILED);
} /* PtLiEgtSigReq */

/*
 *
 *       Fun:   PtLiEgtSigRsp
 *
 *       Desc:  Portable version of EgtConRsp primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtSigRsp
(
Pst *pst,
SpId spId,
TknU32      lclTeid,   /* Local TEID */
EgMsg    *egMsg
)
#else 
PRIVATE S16 PtLiEgtSigRsp (pst, spId, lclTeid, egMsg)
Pst      *pst;
SpId     spId;
TknU32   lclTeid;   /* Local TEID */
EgMsg    *egMsg;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtSigRsp)


   UNUSED(pst);
   UNUSED(spId);
   UNUSED(lclTeid);
   UNUSED(egMsg);

   RETVALUE(RFAILED);
} /* PtLiEgtSigRsp */

/*
 *
 *       Fun:   PtLiEgtStaReq
 *
 *       Desc:  Portable version of EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtStaReq
(
Pst *pst,
SpId spId,
U8   eventType,    /* Status event type */
EgPathInfo   *pathInfo
)
#else 
PRIVATE S16 PtLiEgtStaReq (pst, spId, eventType, pathInfo)
Pst *pst;
SpId spId;
U8   eventType;    /* Status event type */
EgPathInfo   *pathInfo;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtStaReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(eventType);
   UNUSED(pathInfo);

   RETVALUE(RFAILED);
} /* PtLiEgtStaReq */

/*
 *
 *       Fun:   PtLiEgtLclDelReq
 *
 *       Desc:  Portable version of EgtConReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtLclDelReq
(
Pst *pst,
SpId spId,
U32   transId,    /* Status event type */
U32          teid
)
#else 
PRIVATE S16 PtLiEgtLclDelReq (pst, spId, transId, teid)
Pst *pst;
SpId spId;
U32 transId;    /* Status event type */
U32          teid;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtLclDelReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(teid);

   RETVALUE(RFAILED);
} /* PtLiEgtLclDelReq */

#ifdef EGTP_C_PTH_MGT
/*
 *
 *       Fun:   PtLiEgtTunnModReq
 *
 *       Desc:  Portable version of EgtTunnModReq primitive
 *
 *       Ret:  ROK - ok; RFAILED - failed
 *
 *       Notes: none
 *
         File:  eu_ptli.c
 *
 */
#ifdef ANSI
PRIVATE S16 PtLiEgtTunnModReq
(
Pst          *pst,
SpId         spId,
U32          transId,
U32          teid,
CmTptAddr    oldDstIpAddr,
CmTptAddr    newDstIpAddr
)
#else 
PRIVATE S16 PtLiEgtTunnModReq(pst, spId, transId, teid)
Pst          *pst;
SpId         spId;
U32          transId;
U32          teid;
CmTptAddr    oldDstIpAddr;
CmTptAddr    newDstIpAddr;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtTunnModReq)

   UNUSED(pst);
   UNUSED(spId);
   UNUSED(transId);
   UNUSED(teid);
   UNUSED(oldDestIpAddr);
   UNUSED(newDestIpAddr);

   RETVALUE(RFAILED);
} /* PtLiEgtTunnModReq */
#endif

#endif /* EGTP_C */

#ifdef EGTP_U
/*
*
*       Fun:   PtLiEgtEguLclTnlMgmtReq 
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
PUBLIC S16 PtLiEgtEguLclTnlMgmtReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtLiEgtEguLclTnlMgmtReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtEguLclTnlMgmtReq);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "PtLiEgtEguLclTnlMgmtReq(pst, suId(%d) ...)\n", suId));

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(egtUEvnt);

   RETVALUE(ROK);
} /* end of PtLiEgtEguLclTnlMgmtReq() */

/*
*
*       Fun: PtLiEgtEguStaReq 
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
PUBLIC S16 PtLiEgtEguStaReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtLiEgtEguStaReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtEguStaReq);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "PtLiEgtEguStaReq(pst, suId(%d) ...)\n", suId));

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(egtUEvnt);

   RETVALUE(ROK);
} /* end of EgUiEgtEguTnlMgmtCfm() */

/*
*
*       Fun:  PtLiEgtEguDatReq
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
PUBLIC S16 PtLiEgtEguDatReq 
(
Pst          *pst,              /* post structure */
SuId         suId,              /* service user id */
EgtUEvnt     *egtUEvnt
)
#else
PUBLIC S16 PtLiEgtEguDatReq(pst, suId, egtUEvnt)
Pst          *pst;              /* post structure */
SuId         suId;              /* service user id */
EgtUEvnt     *egtUEvnt;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102--*/
   EG_TRC3(PtLiEgtEguDatReq);

   EGDBGP(DBGMASK_UI, (egCb.init.prntBuf,
          "PtLiEgtEguDatReq(pst, suId(%d) ...)\n", suId));

   UNUSED(pst);
   UNUSED(suId);
   UNUSED(egtUEvnt);

   RETVALUE(ROK);
} /* end of PtLiEgtEguDatReq() */

#endif /* EGTP_U */
#endif /* PTEULIEGT */


/********************************************************************30**

         End of file:     eu_ptli.c@@/main/3 - Fri Feb  4 18:18:21 2011

*********************************************************************31*/


/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/


/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---      an       1. Updated for Release eGTP
/main/2      ---      rkumar   1. Initial for eGTP 1.2 Release
/main/2      eg001.102   rss   1. Multithreaded performance changes 
                                  gaurded under SS_MT_CB flag.
                               2. EGTP_C Path management functionality
                                  is introduced
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3      eg003.201 psingh           1. Merged code from eg006.102 to Fix
                                           TRACE5 macro related issue
/main/3      eg004.201 magnihotri     1. Header files added for eGTPC PSF Upgrade
/main/3      eg014.201    rss     1. Egtpc release 10 upgrade
*********************************************************************91*/
