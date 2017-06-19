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

     Name:     PGW - test
 
     Type:     C source file
  
     Desc:     PGW tst file.
  
     File:     jgsm_tst.c
  
     Sid:      avsm_tst.c@@/main/1 - Tue Aug 17 18:54:37 2010
   
     Prg:       sp
  
*********************************************************************21*/


/************************************************************************/
 
/* header include files (.h)            */


#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

/* av001.101: for removing warnings */
#include <stdlib.h>        /* for exit function */

#ifndef AV_PERF
#include <pcap.h>
#endif
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

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
/*#include "hi.h"*/
#endif

#include "lav.h"
#include "avsm.h"        /* defines for S1AP acceptance tests */
#include "av.h"

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

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "lav.x"
#include "avsm.x"
#include "av.x"

SSTskId       smTskId;          /* main task ID for Stack Manager */
SSTskId       hiTskId;          /* main task ID for TUCL */
SSTskId       euTskId;          /* main task ID for PGW, EGTP */



/* Function prototypes */

PRIVATE S16 avRegTsk ARGS((Void));

PRIVATE S16 avCreateTsk ARGS((Void));

PRIVATE S16 avAttachTsk ARGS((Void));


#ifdef ANSI
PUBLIC S16  avsmParseRdConQ
(
Data data
)
#else
PUBLIC S16  avsmParseRdConQ(data)
Data data;
#endif
{
   U32  memAvail;

   TRC2( avsmParseRdConQ)

   switch (data)
   {
      case 'm' :
      case 'M' :
         SRegInfoShow(avPgwCb.init.region, &memAvail);
         break;

      default :
         break;
   }

   RETVALUE(ROK);
} /*  avMmeParseRdConQ */



/*
 *
 *       Fun:    avAttachTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   avsm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 avAttachTsk
(
Void
)
#else
PRIVATE S16 avAttachTsk()
#endif /* ANSI */
{
   SM_TRC2(avAttachTsk);
   /* Attach Stack Manager */
   if (SAttachTTsk (
            ENTSM, (Inst)0, smTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF,"ENTSM, SAttachTTsk failed\n"));

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);
      RETVALUE(RFAILED);
   }

   /* TUCL */
   if (SAttachTTsk (
            ENTHI, (Inst)0, hiTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF,"ENTHI, SAttachTTsk failed\n"));

      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);
      RETVALUE(RFAILED);
   }

#ifndef HI_MULTI_THREADED
   if (SAttachTTsk (
            ENTHI, (Inst)1, hiTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "ENTHI, SAttachTTsk failed for perm task\n"));

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
#endif /* HI_MULTI_THREADED */

#ifdef  AV

   /* PGW */
   if (SAttachTTsk (
          ENTEU, (Inst)0, euTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "ENTEU, SAttachTTsk failed\n"));

      SDeregTTsk(ENTEU,0);
      SDestroySTsk(euTskId);

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }

#ifdef EG
   /* Attach EGTP */
   /* PGW and EGTP belong to same system task */

   if (SAttachTTsk (
          ENTEG, (Inst)0, euTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "ENTEG, SAttachTTsk failed\n"));

      SDeregTTsk(ENTEG,0);
      SDeregTTsk(ENTEU,0);
      SDestroySTsk(euTskId);

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
#endif /* EG */
#endif /* AV */

   AVSM_DBG_INFO((AVSM_PRNTBUF,"Attach SUCCESS ...\n"));
   RETVALUE(ROK);

}
/*
 *
 *       Fun:    avCreateTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   avsm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 avCreateTsk
(
Void
)
#else
PRIVATE S16 avCreateTsk()
#endif /* ANSI */
{
   SM_TRC2(avCreateTsk);
   /* Create system task for Stack Manager */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &smTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "SCreateSTsk failed for stack manager\n"));
      RETVALUE(RFAILED);
   }

   /* Create system task for TUCL  */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &hiTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "SCreateSTsk failed for TUCL\n"));
      RETVALUE(RFAILED);
   }

#ifdef AV
 
   /* Create system task for PGW Application and EGTP  */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &euTskId) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "SCreateSTsk failed for PGW application\n"));
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);

}
/*
 *
 *       Fun:    avRegTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   avsm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 avRegTsk
(
Void
)
#else
PRIVATE S16 avRegTsk()
#endif /* ANSI */
{
   SM_TRC2(avRegTsk);
   /* Register all procIds */
   SSetProcId(AVSM_DEFAULT_PROCID);

   /* Register TUCL, stack manager and E-GTP 
       */

   /* Register Stack Manager */
   if (SRegTTsk(
            (Ent)ENTSM, (Inst)0, (Ttype)TTNORM, PRIOR0,
            smActvInit, smActvTsk ) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF,"SRegTTsk() failed for smActvInit()\n"));
      RETVALUE(RFAILED);
   }
   

   /* Register TUCL */
   if (SRegTTsk(
            (Ent)ENTHI, (Inst)0, (Ttype)TTNORM, PRIOR0,
            hiActvInit, hiActvTsk ) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "SRegTTsk() failed for hiActvInit()\n"));
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
   
#ifndef HI_MULTI_THREADED
   if (SRegTTsk (
            (Ent)ENTHI, (Inst)1, (Ttype)TTPERM, PRIOR0,
            NULLP, hiScanPermTsk) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF,"SRegTTsk() failed for hiActvInit perm task()\n"));
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif /* HI_MULTI_THREADED */


#ifdef AV
#ifdef EG
   /* Register EGTP */
   if (SRegTTsk(
            (Ent)ENTEG, (Inst)0, (Ttype)TTNORM, PRIOR0,
            egActvInit, egActvTsk ) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "SRegTTsk() failed for egActvInit()\n"));

      SDeregTTsk(ENTHI, 1); 
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif /* EG */

   /* Register PGW task */
   if (SRegTTsk(
            (Ent)ENTEU, (Inst)0, (Ttype)TTNORM, PRIOR0,
            avPgwSndActvInit, avPgwSndActvTsk ) != ROK)
   {
      AVSM_DBG_ERROR((AVSM_PRNTBUF, "SRegTTsk() failed for avPgwSndActvInit()\n"));
      SDeregTTsk(ENTEG, 0);
      SDeregTTsk(ENTHI, 1); 
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif


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
         File:   avsm_tst.c
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

   if((ret = avRegTsk())!= ROK)
   {
      RETVALUE(RFAILED);
   }
   
   if((ret = avCreateTsk())!= ROK)
   {
      RETVALUE(RFAILED);
   }
   
   if((ret = avAttachTsk()) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Trigger State machine from here */  
   avSmMain();


   RETVALUE(ROK);
} /* end of tst() */


/*
 *
 *       Fun:    avSmExit - graceful exit from acceptance tests
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
         File:   avsm_tst.c
 *
 */

#ifdef ANSI
PUBLIC Void avSmExit
(
Void
)
#else
PUBLIC Void avSmExit()
#endif /* ANSI */
{
   SM_TRC2(avSmExit);

   SDetachTTsk(
              (Ent)ENTSM, (Inst)0);
   SDeregTTsk(
             (Ent)ENTSM, (Inst)0);

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

#ifdef AV

   SDetachTTsk(
         (Ent)ENTEG,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTEG, (Inst)0);

   SDetachTTsk(
         (Ent)ENTEU,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTEU, (Inst)0);
#endif

   SDestroySTsk(hiTskId);

#ifdef AV
   SDestroySTsk(euTskId);
#endif
   SDestroySTsk(smTskId);

   exit(0);
   RETVOID;
} /* end of avSmExit() */

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
*       File:  avsm_tst.c
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
      avSmExit();
   }
   avsmParseRdConQ(data);

   
   RETVALUE(OK);
}/* end of  rdConQ()*/

  
/********************************************************************30**
  
         End of file:     avsm_tst.c@@/main/1 - Tue Aug 17 18:54:37 2010
   
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************70**
  
*********************************************************************71*/

/********************************************************************80**
 
*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -       sp               1.LTE-CNE:PGW Initial Release
/main/1      av001.101 rp             1.For removing warnings
*********************************************************************91*/






