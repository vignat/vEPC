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

     Name:    Trillium LTE CNE - Stack Manager Initialization File

     Type:    C Source file

     Desc:    

     File:    vbsm_tst.c

     Sid:      vbsm_tst.c@@/main/1 - Mon Jun 28 19:35:42 2010

     Prg:     
*********************************************************************21*/

#include <stdlib.h>
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

#include "leg.h"
#include "egt.h"
#include "sct.h"           /* SB (SCTP) layer */
#include "lsz.h"           /* layer management defines for S1AP */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */
/* vb007.101 s1ap include file order has changed */
#include "szt_asn.h" 
#include "szt.h"            /* defines and macros for S1AP */
#include "vbsm.h"        /* defines for S1AP acceptance tests */
#include "lvb.h"
#include "vb.h"
#include "cm_esm.h"
#include "cm_emm.h"
#include "vb_hss.h"

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

#include "leg.x"
#include "egt.x"
#include "sct.x"           /* SCTP Interface Structures */
#include "szt_asn.x" 
#include "szt.x"           /* SCTP Interface Structures */
#include "lsz.x"           /* S1AP types */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

#include "lvb.x"
#include "vbsm.x"        

SSTskId       smTskId;          /* main task ID for Stack Manager */
SSTskId       uzTskId;          /* main task ID for MME and S1AP */
SSTskId       sbTskId;          /* main task ID for SCTP */
SSTskId       hiTskId;          /* main task ID for TUCL */
SSTskId       euTskId;          /* main task ID for S-GW, EGTP */
/* vb006.101: Fixed error for g++ compilation on Linux*/



/* Function prototypes */

PRIVATE S16 vbRegTsk ARGS((Void));

PRIVATE S16 vbCreateTsk ARGS((Void));

PRIVATE S16 vbAttachTsk ARGS((Void));

#ifdef __cplusplus
EXTERN "C" {
#endif

#ifdef __cplusplus
}
#endif
/*
 *
 *       Fun:    vbAttachTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   vbsm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 vbAttachTsk
(
Void
)
#else
PRIVATE S16 vbAttachTsk()
#endif /* ANSI */
{
   SM_TRC2(vbAttachTsk);
   /* Attach Stack Manager */
   if (SAttachTTsk (
            ENTSM, (Inst)0, smTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"ENTSM, SAttachTTsk failed\n"));

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);
      RETVALUE(RFAILED);
   }

 
   /* Attach MME Application */
   if (SAttachTTsk (
            ENTUZ, (Inst)0, uzTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"ENTUZ, SAttachTTsk failed\n"));

      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }


   /* Attach S1AP */
   if (SAttachTTsk (
            ENTSZ, (Inst)0, uzTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "ENTSZ, SAttachTTsk failed\n"));

      /* S1AP and MME App belong to same system task */
      SDeregTTsk(ENTSZ,0);
      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
  
   /* SCTP */
   if (SAttachTTsk (
            ENTSB, (Inst)0, sbTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "ENTSB, SAttachTTsk failed\n"));
      SDeregTTsk(sbTskId,0);
      SDestroySTsk(sbTskId);

      SDeregTTsk(ENTSZ,0);
      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
   
   /* TUCL */
   if (SAttachTTsk (
            ENTHI, (Inst)0, hiTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"ENTHI, SAttachTTsk failed\n"));

      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(sbTskId,0);
      SDestroySTsk(sbTskId);

      SDeregTTsk(ENTSZ,0);
      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);
      RETVALUE(RFAILED);
   }

#ifndef HI_MULTI_THREADED
   if (SAttachTTsk (
            ENTHI, (Inst)1, hiTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "ENTHI, SAttachTTsk failed for perm task\n"));

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(sbTskId,0);
      SDestroySTsk(sbTskId);

      SDeregTTsk(ENTSZ,0);
      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
#endif /* HI_MULTI_THREADED */

   if (SAttachTTsk (
          ENTEU, (Inst)0, uzTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "ENTEU, SAttachTTsk failed\n"));

      SDeregTTsk(ENTEU,0);

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(sbTskId,0);
      SDestroySTsk(sbTskId);

      SDeregTTsk(ENTSZ,0);
      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }

#ifdef EG
   /* Attach EGTP */
   /* MME and EGTP belong to same system task */

   if (SAttachTTsk (
          ENTEG, (Inst)0, uzTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "ENTEG, SAttachTTsk failed\n"));

      SDeregTTsk(ENTEG,0);
      SDeregTTsk(ENTEU,0);

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(sbTskId,0);
      SDestroySTsk(sbTskId);

      SDeregTTsk(ENTSZ,0);
      SDeregTTsk(ENTUZ,0);
      SDestroySTsk(uzTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
#endif /* EG */

   VBSM_DBG_INFO((VBSM_PRNTBUF,"Attach SUCCESS for All LAYERS...\n"));
   RETVALUE(ROK);

}
/*
 *
 *       Fun:    vbCreateTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   vbsm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 vbCreateTsk
(
Void
)
#else
PRIVATE S16 vbCreateTsk()
#endif /* ANSI */
{
   SM_TRC2(vbCreateTsk);
   /* Create system task for Stack Manager */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &smTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SCreateSTsk failed for stack manager\n"));
      RETVALUE(RFAILED);
   }

   /* Create system task for MME Application, S1AP and EGTP-C */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &uzTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"SCreateSTsk failed for MME application\n"));
      RETVALUE(RFAILED);
   }

   /* Create system task for SCTP */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &sbTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SCreateSTsk failed for SCTP\n"));
      RETVALUE(RFAILED);
   }

   /* Create system task for TUCL  */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &hiTskId) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SCreateSTsk failed for TUCL\n"));
      RETVALUE(RFAILED);
   }


   RETVALUE(ROK);

}
/*
 *
 *       Fun:    vbRegTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   vbsm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 vbRegTsk
(
Void
)
#else
PRIVATE S16 vbRegTsk()
#endif /* ANSI */
{
   SM_TRC2(vbRegTsk);
   /* Register all procIds */
   SSetProcId(VBSM_DEFAULT_PROCID);

   /* Register MME- S1AP, SCTP, TUCL and stack manager, perm tasks and E-GTPC  */

   /* Register Stack Manager */
   if (SRegTTsk(
            (Ent)ENTSM, (Inst)0, (Ttype)TTNORM, PRIOR0,
            smActvInit, smActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"SRegTTsk() failed for smActvInit()\n"));
      RETVALUE(RFAILED);
   }
   
   /* Register MME */
   if (SRegTTsk(
           (Ent)ENTUZ, (Inst)0, (Ttype)TTNORM, PRIOR0,
           vbMmeActvInit, vbMmeActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SRegTTsk() failed for MME Application \n"));
      SDeregTTsk(ENTSM, 0);
      RETVALUE(RFAILED);
   }

   /* Register S1AP */
   if (SRegTTsk(
           (Ent)ENTSZ, (Inst)0, (Ttype)TTNORM, PRIOR0,
           szActvInit, szActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SRegTTsk() failed for S1AP()\n"));
      SDeregTTsk(ENTUZ, 0);
      SDeregTTsk(ENTSM, 0);
      RETVALUE(RFAILED);
   }
 
   /* Register SCTP */
   if (SRegTTsk(
            (Ent)ENTSB, (Inst)0, (Ttype)TTNORM, PRIOR0,
             sbActvInit, sbActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SRegTTsk() failed for sbActvInit()\n"));
      SDeregTTsk(ENTSZ, 0);
      SDeregTTsk(ENTUZ, 0);
      SDeregTTsk(ENTSM, 0);
      RETVALUE(RFAILED);
   }

   /* Register TUCL */
   if (SRegTTsk(
            (Ent)ENTHI, (Inst)0, (Ttype)TTNORM, PRIOR0,
            hiActvInit, hiActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SRegTTsk() failed for hiActvInit()\n"));
      SDeregTTsk(ENTSB, 0);
      SDeregTTsk(ENTSZ, 0);
      SDeregTTsk(ENTUZ, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
   
#ifndef HI_MULTI_THREADED
   if (SRegTTsk (
            (Ent)ENTHI, (Inst)1, (Ttype)TTPERM, PRIOR0,
            NULLP, hiScanPermTsk) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF,"SRegTTsk() failed for hiActvInit perm task()\n"));
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSB, 0);
      SDeregTTsk(ENTSZ, 0);
      SDeregTTsk(ENTUZ, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif /* HI_MULTI_THREADED */


#ifdef EG
   /* Register EGTP-C for S11 interface with S-GW */
   if (SRegTTsk(
            (Ent)ENTEG, (Inst)0, (Ttype)TTNORM, PRIOR0,
            egActvInit, egActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SRegTTsk() failed for egActvInit()\n"));

      SDeregTTsk(ENTHI, 1); 
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSB, 0);
      SDeregTTsk(ENTSZ, 0);
      SDeregTTsk(ENTUZ, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif /* EG */
   
   /* Register S11 Interface application with S-GW */
   if (SRegTTsk(
            (Ent)ENTEU, (Inst)0, (Ttype)TTNORM, PRIOR0,
            vbMmeActvInit, vbMmeActvTsk ) != ROK)
   {
      VBSM_DBG_ERROR((VBSM_PRNTBUF, "SRegTTsk() failed for MME S11 application\n"));
      SDeregTTsk(ENTEG, 0);
      SDeregTTsk(ENTHI, 1); 
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSB, 0);
      SDeregTTsk(ENTSZ, 0);
      SDeregTTsk(ENTUZ, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);

}
/*
 *
 *       Fun:    tst - 
 *
 *       Desc:   This function is the first invoked by SInit(). It registers the
 *               protocol layers and the permanent task.
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   vbsm_tst.c
 *
 */

/*
 *            Changed the order of registration and attaching of
 *            tasks. The order now is -
 *            1) Register all the tasks with SSI
 *            2) Create a system thread
 *            3) Attach all the tasks with the thread
 *
 *            Also, attaching of the permanent test task has been
 *            moved towards the end of this function after the
 *            initialization code has been completed. This is
 *            done so as not to run into issues on Windows in
 *            multithreaded mode.
 */

#ifdef ANSI
PUBLIC S16 tst
(
Void
)
#else
PUBLIC S16 tst()
#endif /* ANSI */
{
   S16 ret;

   SM_TRC2(tst);
   
   if((ret = vbRegTsk())!= ROK)
   {
      RETVALUE(RFAILED);
   }
   
   if((ret = vbCreateTsk())!= ROK)
   {
      RETVALUE(RFAILED);
   }
   
   if((ret = vbAttachTsk()) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Trigger State machine from here */  
   vbSmMain();

   RETVALUE(ROK);
} /* end of tst() */


/*
 *
 *       Fun:    vbSmExit - graceful exit from acceptance tests
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
         File:   vbsm_tst.c
 *
 */

#ifdef ANSI
PUBLIC Void vbSmExit
(
Void
)
#else
PUBLIC Void vbSmExit()
#endif /* ANSI */
{
   SM_TRC2(vbSmExit);

   SDetachTTsk(
        (Ent)ENTSM, (Inst)0);
   SDeregTTsk(
        (Ent)ENTSM, (Inst)0);

   SDetachTTsk(
         (Ent)ENTUZ,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTUZ, (Inst)0);

   SDetachTTsk(
         (Ent)ENTSZ,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTSZ, (Inst)0);

   SDetachTTsk(
         (Ent)ENTSB,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTSB, (Inst)0);

   SDetachTTsk(
         (Ent)ENTHI,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTHI, (Inst)0);

#ifndef HI_MULTI_THREADED
   SDetachTTsk(
         (Ent)ENTHI,(Inst)1 );
   SDeregTTsk(
         (Ent)ENTHI, (Inst)1);
#endif /* HI_MULTI_THREADED */

   SDetachTTsk(
         (Ent)ENTEG,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTEG, (Inst)0);


   SDestroySTsk(uzTskId);
   SDestroySTsk(sbTskId);
   SDestroySTsk(hiTskId);

   SDestroySTsk(smTskId);

   exit(0);
   RETVOID;
} /* end of vbSmExit() */


/*
*
*       Fun:   rdConQ
*
*       Desc:  This function is used to read the console queue.
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  vbsm_tst.c
*
*/

#ifdef ANSI
PUBLIC S16 rdConQ
(
Data data
)
#else
PUBLIC S16 rdConQ(data)
Data data;
#endif
{
   SM_TRC2(rdConQ);

   if ((data == 0x03) ||
       (data == 27))
   {
      vbSmExit();
   }
   vbMmeParseRdConQ(data);
   
   RETVALUE(OK);
}/* end of  rdConQ()*/


#ifdef VB_PT
/* These functions and variable are define for making PT build only */

PUBLIC S16 msArgc;              /* argc */
PUBLIC Txt **msArgv;            /* argv */
PUBLIC S16 msOptInd;            /* SGetOpt vars */
PUBLIC S8 *msOptArg;            /* SGetOpt vars */


/*
 *
 *      Fun:   szActvInit
 *
 *      Desc:  Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  sz_mi.c
 *
*/
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 szActvInit
(
ProcId procId,
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason,          /* reason */
Void **xxCb           /* Protocol control Block */
)
#else
PUBLIC S16 szActvInit(procId, entity, inst, region, reason,xxCb)
ProcId procId;         /* Proc Id */
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
Void **xxCb;           /* protocol control block */
#endif

#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI 
PUBLIC S16 szActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 szActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
#endif /*SS_MULTIPLE_PROCS */
{
   
      RETVALUE(ROK);
}

/*
 *
 *      Fun:   sbActvInit
 *
 *      Desc:  Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  sz_mi.c
 *
*/
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 sbActvInit
(
ProcId procId,
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason,          /* reason */
Void **xxCb           /* Protocol control Block */
)
#else
PUBLIC S16 sbActvInit(procId, entity, inst, region, reason,xxCb)
ProcId procId;         /* Proc Id */
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
Void **xxCb;           /* protocol control block */
#endif

#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI 
PUBLIC S16 sbActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 sbActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
#endif /*SS_MULTIPLE_PROCS */
{
   
      RETVALUE(ROK);
}


/*
 *
 *      Fun:   hiActvInit
 *
 *      Desc:  Activate Task - Initialize. 
 *
 *      Ret:   ROK   - ok
 *
 *      Notes: Invoked by system services to initialize a task.
 *
 *      File:  sz_mi.c
 *
*/
#ifdef SS_MULTIPLE_PROCS
#ifdef ANSI
PUBLIC S16 hiActvInit
(
ProcId procId,
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason,          /* reason */
Void **xxCb           /* Protocol control Block */
)
#else
PUBLIC S16 hiActvInit(procId, entity, inst, region, reason,xxCb)
ProcId procId;         /* Proc Id */
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
Void **xxCb;           /* protocol control block */
#endif

#else /* SS_MULTIPLE_PROCS */
#ifdef ANSI 
PUBLIC S16 hiActvInit
(
Ent entity,            /* entity */
Inst inst,             /* instance */
Region region,         /* region */
Reason reason          /* reason */
)
#else
PUBLIC S16 hiActvInit(entity, inst, region, reason)
Ent entity;            /* entity */
Inst inst;             /* instance */
Region region;         /* region */
Reason reason;         /* reason */
#endif
#endif /*SS_MULTIPLE_PROCS */
{
   
      RETVALUE(ROK);
}



/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from Upper/Lower Layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   sb_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC S16 sbActvTsk
(
Pst      *pst,              /* post */
Buffer   *mBuf              /* message buffer */
)
#else
PUBLIC S16 sbActvTsk(pst, mBuf)
Pst      *pst;              /* post */
Buffer   *mBuf;             /* message buffer */
#endif
{
   TRC3(sbActvTsk)

   UNUSED(pst);
   UNUSED(mBuf);

   SExitTsk();

   RETVALUE(ROK);
} /* end of sbActvTsk */


/*
*
*       Fun:    activate task
*
*       Desc:   Processes received event from Upper/Lower Layer
*
*       Ret:    ROK  - ok
*
*       Notes:  None
*
*       File:   sb_ex_pt.c
*
*/
#ifdef ANSI
PUBLIC S16 hiActvTsk
(
Pst      *pst,              /* post */
Buffer   *mBuf              /* message buffer */
)
#else
PUBLIC S16 hiActvTsk(pst, mBuf)
Pst      *pst;              /* post */
Buffer   *mBuf;             /* message buffer */
#endif
{
   TRC3(hiActvTsk)

   UNUSED(pst);
   UNUSED(mBuf);

   SExitTsk();

   RETVALUE(ROK);
} /* end of hiActvTsk */


/*********************************************************************************
 *
 *       Fun:   EgMiLegCfgReq
 *
 *       Desc:  This function is used by the layer manager to configure
 *              the eGTP layer.  eGTP Protocol responds with a
 *              configuration confirm "EgMiLegCfgCfm" to the layer manager
 *              after completing configuration.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: Configuration must be performed in the following
 *              sequence:
 *              1) General configuration (STEGGEN)
 *              2) Upper sap configuration
 *              3) Lower sap configuration (STEGTSAP)
 *              4) Transport Server Configuration
 *              5) Message DB Configuration
 *              6) IE Dictionary Configuration
 *
 *       File:  eg_mi.c
 *
 *********************************************************************************/
#ifdef ANSI
PUBLIC S16 EgMiLegCfgReq
(
Pst       *pst,              /* Post structure */
EgMngmt   *cfgReq            /* management structure */
)
#else
PUBLIC S16 EgMiLegCfgReq(pst, cfgReq)
Pst       *pst;              /* Post structure */
EgMngmt   *cfgReq;           /* management structure */
#endif
{
      RETVALUE(ROK);

}



/***********************************************************************************
 *
 *       Fun:   EgMiLegCntrlReq
 *
 *       Desc:  This function is used by the layer manager to control
 *              the eGTP layer.  eGTP Protocol responds with a
 *              control confirm "EgMiLegCntrlCfm" to the layer manager
 *              after completing control request.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  eg_mi.c
 *
 ***********************************************************************************/

#ifdef ANSI
PUBLIC S16 EgMiLegCntrlReq
(
Pst       *pst,                 /* Post structure */
EgMngmt   *cntrlReq             /* management structure */
)
#else
PUBLIC S16 EgMiLegCntrlReq(pst, cntrlReq)
Pst       *pst;                 /* Post structure */
EgMngmt   *cntrlReq;            /* management structure */
#endif
{
      RETVALUE(ROK);

}


/**********************************************************************************
 *
 *       Fun:   EgMiLegStaReq
 *
 *       Desc:  This function is used by the layer manager to get
 *              the status of eGTP layer.  eGTP Protocol responds with a
 *              status confirm "EgMiLegStaCfm" to the layer manager
 *              after completing status request.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  eg_mi.c
 *
 **********************************************************************************/

#ifdef ANSI
PUBLIC S16 EgMiLegStaReq
(
Pst       *pst,                 /* Post structure */
EgMngmt   *staReq               /* management structure */
)
#else
PUBLIC S16 EgMiLegStaReq(pst, staReq)
Pst       *pst;                 /* Post structure */
EgMngmt   *staReq;              /* management structure */
#endif
{
      RETVALUE(ROK);
}


/***********************************************************************************
 *
 *       Fun:   EgMiLegStsReq
 *
 *       Desc:  This function is used by the layer manager to get
 *              the statistics of eGTP layer.  eGTP Protocol responds with a
 *              statistics confirm "EgMiLegStsCfm" to the layer manager
 *              after completing statistics request.
 *
 *       Ret:   ROK     - ok
 *              RFAILED - failure
 *
 *       Notes: N/A
 *
 *       File:  eg_mi.c
 *
 ***********************************************************************************/

#ifdef ANSI
PUBLIC S16 EgMiLegStsReq
(
Pst       *pst,                  /* Post structure */
Action    action,                /* action to be taken */
EgMngmt   *stsReq                /* management structure */
)
#else
PUBLIC S16 EgMiLegStsReq(pst, action, stsReq)
Pst       *pst;                  /* Post structure */
Action    action;                /* action to be taken */
EgMngmt   *stsReq;               /* management structure */
#endif
{
      RETVALUE(ROK);
}


/*
*
*       Fun:    HiMiLhiCfgReq
*
*       Desc:   Configure the layer. Responds with a HiMiLhiCfgCfm
*               primitive.
*
*       Ret:    ROK     - ok
*
*       Notes:  Configuration must be performed in the following 
*               sequence:
*               1) general configuration (STGEN)
*               2) transport sap configuration (STTSAP)
*               3) TLS context configuration (STCTX)
*
*       File:   hi_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 HiMiLhiCfgReq
(
Pst             *pst,           /* post structure */
HiMngmt         *cfg            /* configuration structure */
)
#else
PUBLIC S16 HiMiLhiCfgReq(pst, cfg)
Pst             *pst;           /* post structure */
HiMngmt         *cfg;           /* configuration structure */
#endif
{
     RETVALUE(ROK);

}


/*
*
*       Fun:    HiMiLhiCntrlReq
*
*       Desc:   Control the specified element: enable or diable
*               trace and alarm (unsolicited status) generation,
*               delete or disable a SAP or a group of SAPs, enable
*               or disable debug printing.
*
*       Ret:    ROK     - ok
*
*       Notes:  None
*
*       File:   hi_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 HiMiLhiCntrlReq
(
Pst             *pst,           /* post structure */              
HiMngmt         *ctl            /* pointer to control structure */
)
#else
PUBLIC S16 HiMiLhiCntrlReq(pst, ctl)
Pst             *pst;           /* post structure */              
HiMngmt         *ctl;           /* pointer to control structure */
#endif
{
     RETVALUE(ROK);


}


/*
*
*       Fun:    HiMiLhiStsReq
*
*       Desc:   Get statistics information. Statistics are
*               returned by a HiMiLhiStsCfm primitive. The
*               statistics counters can be reset using the action
*               parameter:
*                  ZEROSTS      - zero statistics counters
*                  NOZEROSTS    - do not zero statistics counters
*
*       Ret:    ROK     - ok
*
*       Notes:  None
*
*       File:   hi_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 HiMiLhiStsReq
(
Pst             *pst,           /* post structure */
Action          action,         /* action to be done */
HiMngmt         *sts            /* statistics structure */
)
#else
PUBLIC S16 HiMiLhiStsReq(pst, action, sts)
Pst             *pst;           /* post structure */
Action          action;         /* action to be done */
HiMngmt         *sts;           /* statistics structure */
#endif
{
     RETVALUE(ROK);

}


/*
*
*       Fun:    HiMiLhiStaReq
*
*       Desc:   Get status information. Responds with a
*               HiMiLhiStaCfm primitive.
*
*       Ret:    ROK     - ok
*
*       Notes:  None
*
*       File:   hi_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 HiMiLhiStaReq
(
Pst             *pst,           /* post structure */
HiMngmt         *sta            /* management structure */
)
#else
PUBLIC S16 HiMiLhiStaReq(pst, sta)
Pst             *pst;           /* post structure */
HiMngmt         *sta;           /* management structure */
#endif
{
     RETVALUE(ROK);

}


/*
*
*       Fun:   SbMiLsbCfgReq
*
*       Desc:  This function is used by the Layer Management to
*              configure the layer. The SCTP layer responds with a
*              Configuration Confirm to the layer manager. This primitive
*              can also be used to reconfigure the layer.
*
*       Ret:   Success: ROK
*              Failure: RFAILED
*
*       Notes: Configuration must be performed in the following
*              sequence:
*              1) general configuration (STGEN);
*              2) sap configuration (STSCTSAP and/or STTSAP).
*
*       File:  sb_bdy1.c
*
*/
#ifdef ANSI
PUBLIC S16 SbMiLsbCfgReq
(
Pst     *pst,                /* post structure */
SbMgmt  *cfg                 /* configuration structure */
)
#else
PUBLIC S16 SbMiLsbCfgReq(pst, cfg)
Pst     *pst;                /* post structure */
SbMgmt  *cfg;                /* configuration structure */
#endif
{

     RETVALUE(ROK);
}

/*
*
*       Fun:   Statistics Request Primitive
*
*       Desc:  This primitive is used by the Layer Manager to solicit
*              statistics information. The statistics values are
*              returned by SbMiLsbStsCfm primitive.The statistics
*              counters can be initialized to NULL using the "action"
*              parameter. The possible values for "action" are:
*
*              ZEROSTS:    zero statistics counters
*              NOZEROSTS:  do not zero statistics counters
*
*       Ret:   ROK
*
*       Notes: <none>
*
*       File:  sb_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 SbMiLsbStsReq
(
Pst     *pst,                /* post structure */
Action  action,              /* action to be done */
SbMgmt *sts                  /* statistics structure */
)
#else
PUBLIC S16 SbMiLsbStsReq(pst, action, sts)
Pst     *pst;                /* post structure */
Action  action;              /* action to be done */
SbMgmt *sts;                 /* statistics structure */
#endif
{

     RETVALUE(ROK);

}

/*
*
*       Fun:   Status Request Primitive
*
*       Desc:  This primitive is used by the Layer Manager to solicit
*              status information. The information is returned via the
*              SbMiLsbStaCfm primitive.
*
*       Ret:   ROK      - ok
*
*       Notes: <none>
*
*       File:  sb_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 SbMiLsbStaReq
(
Pst    *pst,                   /* post structure */
SbMgmt *sta                    /* status structure */
)
#else
PUBLIC S16 SbMiLsbStaReq(pst, sta)
Pst    *pst;                   /* post structure */
SbMgmt *sta;                   /* status structure */
#endif
{

     RETVALUE(ROK);

}

/*
*
*       Fun:   Control Request Primitive
*
*       Desc:  This primitive is used to control the specified element.
*              It can be used to enable or disable the general elements
*              (trace, alarm and debug prints.)
*              It can also be used to: delete or disable a SAP or a group of
*              SAPs; bind or enable a SAP or group of SAPs or
*              Shutdown all the operations.
*              A control confirm is sent to acknowledge the request
*
*       Ret:   Success:       ROK
*              Failure:       RFAILED
*
*       Notes: <none>
*
*       File:  sb_bdy1.c
*
*/

#ifdef ANSI
PUBLIC S16 SbMiLsbCntrlReq
(
Pst    *pst,                   /* post structure */
SbMgmt *cntrl                  /* pointer to control structure */
)
#else
PUBLIC S16 SbMiLsbCntrlReq(pst, cntrl)
Pst    *pst;                   /* post structure */
SbMgmt *cntrl;                 /* pointer to control structure */
#endif
{
     RETVALUE(ROK);

}

/*
*
*       Fun:   SRegInfoShow
*
*       Desc:  This function displays the memory usage information
*              for the destined region. It will show the usage of
*              each configured bucket and the heap for the specified region.
*
*       Ret:   ROK      OK
*              RFAILED     Region not registered
*
*       Notes: A Sample Output from the function 
*       Bucket Memory: region 1
*       ====================================================
*       Bucket  Number of Blks configured  Size  Allocated
*       ====================================================
*       0                     1             16         1
*       1                     1             32         0
*       2                     1             80         0
*       3                     1            256         0
*       4                     1            320         0
*
*       ---------------
*       Heap Memory: region 1
*       Heap Size: 0
*       Heap Allocated: 0
*       Heap Segmented blocks: 0
*
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SRegInfoShow
(
Region region,
U32 *availmem
)
#else
PUBLIC S16 SRegInfoShow(region, availmem)
Region region;
U32 *availmem;
#endif
{
   RETVALUE(ROK);

}


/*
*
*       Fun:   SGetOpt
*
*       Desc:  Get options from command line
*
*       Ret:   option  - success
*              '?'     - fail
*              EOF     - end of options
*
*       Notes: Handles command lines like the following
*
*              if opts = "abc:d" 
*                 then command line should look like this...
*                    -a foo -b foo1 -c -d foo
*
*              code usage:
*
*              while ((ret = SGetOpt(msArgc, msArgv, "ls")) != EOF )
*              {
*                 switch(ret)
*                 {
*                    case 'l':
*                       nloops = atoi(msArgv[msOptInd]);
*                       break;
*                    case 's':
*                       state1 = atoi(msArgv[msOptInd]);
*                    case '?':
*                    default:
*                       break;
*                 }
*              }
*
*       File:  mt_ss.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetOpt
(
int argc,                   /* argument count */
char **argv,                /* argument value */
char *opts                  /* options */
)
#else
PUBLIC S16 SGetOpt(argc, argv, opts)
int argc;                   /* argument count */
char **argv;                /* argument value */
char *opts;                 /* options */
#endif
{
   RETVALUE(ROK);

}



#endif /* VB_PT */

/**********************************************************************
  
         End of file:     vbsm_tst.c@@/main/1 - Mon Jun 28 19:35:42 2010
   
**********************************************************************/
 
/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      ---                      1. LTE-CNE Initial Release.
/main/1      ---      rk              1. Updated for release
/main/1     vb006.101   ragrawal      1. Fixed error for g++ compilation on Linux.
/main/1     vb007.101   akaranth      1. s1ap include files order has changed.
*********************************************************************91*/
