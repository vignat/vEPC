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

     Name:     SGW-test 

     Type:    C source file

     Desc:    SGW tst file   

     File:    qosm_tst.c

     Sid:      qosm_tst.c@@/main/1 - Thu Jul  8 21:22:23 2010

     Prg:      sp    
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

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
/* qo003.101: HI .[hx] include files removed. */
#endif

#include "lqo.h"
#include "qosm.h"        /* defines for S1AP acceptance tests */
#include "qo.h"        
#include "qo_err.h"

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
/* qo003.101: HI .[hx] include files removed. */
#endif

#include "lqo.x"
#include "qosm.x"        
#include "qo.x"        

SSTskId       smTskId;          /* main task ID for Stack Manager */
SSTskId       hiTskId;          /* main task ID for TUCL */
SSTskId       euTskId;          /* main task ID for S-GW, EGTP */



/* Function prototypes */

PRIVATE S16 qoRegTsk ARGS((Void));

PRIVATE S16 qoCreateTsk ARGS((Void));

PRIVATE S16 qoAttachTsk ARGS((Void));


#ifdef ANSI
PUBLIC S16  qosmParseRdConQ
(
Data data
)
#else
PUBLIC S16  qosmParseRdConQ(data)
Data data;
#endif
{
   U32  memAvail;

   TRC2( qosmParseRdConQ)

   switch (data)
   {
      case 'm' :
      case 'M' :
         SRegInfoShow(qoSgwCb.init.region, &memAvail);
         break;

      default :
         break;
   }

   RETVALUE(ROK);
} /*  qoSgwParseRdConQ */



/*
 *
 *       Fun:    qoAttachTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   qosm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 qoAttachTsk
(
Void
)
#else
PRIVATE S16 qoAttachTsk()
#endif /* ANSI */
{
   SM_TRC2(qoAttachTsk);
   /* Attach Stack Manager */
   if (SAttachTTsk (
            ENTSM, (Inst)0, smTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF,"ENTSM, SAttachTTsk failed\n"));

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);
      RETVALUE(RFAILED);
   }

   /* TUCL */
   if (SAttachTTsk (
            ENTHI, (Inst)0, hiTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF,"ENTHI, SAttachTTsk failed\n"));

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
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "ENTHI, SAttachTTsk failed for perm task\n"));

      SDeregTTsk(hiTskId,1);
      SDeregTTsk(hiTskId,0);
      SDestroySTsk(hiTskId);

      SDeregTTsk(ENTSM,0);
      SDestroySTsk(smTskId);

      RETVALUE(RFAILED);
   }
#endif /* HI_MULTI_THREADED */

#ifdef  QO_SGW

   /* SGW */
   if (SAttachTTsk (
          ENTEU, (Inst)0, euTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "ENTEU, SAttachTTsk failed\n"));

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
   /* S-GW and EGTP belong to same system task */

   if (SAttachTTsk (
          ENTEG, (Inst)0, euTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "ENTEG, SAttachTTsk failed\n"));

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
#endif /* QO_SGW */

   QOSM_DBG_INFO((QOSM_PRNTBUF,"Attach SUCCESS ...\n"));
   RETVALUE(ROK);

}
/*
 *
 *       Fun:    qoCreateTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   qosm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 qoCreateTsk
(
Void
)
#else
PRIVATE S16 qoCreateTsk()
#endif /* ANSI */
{
   SM_TRC2(qoCreateTsk);
   /* Create system task for Stack Manager */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &smTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "SCreateSTsk failed for stack manager\n"));
      RETVALUE(RFAILED);
   }

   /* Create system task for TUCL  */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &hiTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "SCreateSTsk failed for TUCL\n"));
      RETVALUE(RFAILED);
   }

#ifdef QO_SGW
 
   /* Create system task for SGW Application and EGTP  */
   /* The same task is used by PDN-GW Sending TAPA task */
   if (SCreateSTsk ((SSTskPrior) PRIOR0, &euTskId) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "SCreateSTsk failed for SGW application\n"));
      RETVALUE(RFAILED);
   }
#endif

   RETVALUE(ROK);

}
/*
 *
 *       Fun:    qoRegTsk()
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   qosm_tst.c
 *
 */


#ifdef ANSI
PRIVATE S16 qoRegTsk
(
Void
)
#else
PRIVATE S16 qoRegTsk()
#endif /* ANSI */
{
   SM_TRC2(qoRegTsk);
   /* Register all procIds */
   SSetProcId(QOSM_DEFAULT_PROCID);


   /* Register Stack Manager */
   if (SRegTTsk(
            (Ent)ENTSM, (Inst)0, (Ttype)TTNORM, PRIOR0,
            smActvInit, smActvTsk ) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF,"SRegTTsk() failed for smActvInit()\n"));
      RETVALUE(RFAILED);
   }
   

   /* Register TUCL */
   if (SRegTTsk(
            (Ent)ENTHI, (Inst)0, (Ttype)TTNORM, PRIOR0,
            hiActvInit, hiActvTsk ) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "SRegTTsk() failed for hiActvInit()\n"));
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
   
#ifndef HI_MULTI_THREADED
   if (SRegTTsk (
            (Ent)ENTHI, (Inst)1, (Ttype)TTPERM, PRIOR0,
            NULLP, hiScanPermTsk) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF,"SRegTTsk() failed for hiActvInit perm task()\n"));
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif /* HI_MULTI_THREADED */


   /* All S-GW related */
#ifdef QO_SGW

#ifdef EG
   /* Register EGTPU for S-GW */
   if (SRegTTsk(
            (Ent)ENTEG, (Inst)0, (Ttype)TTNORM, PRIOR0,
            egActvInit, egActvTsk ) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "SRegTTsk() failed for egActvInit()\n"));

      SDeregTTsk(ENTHI, 1); 
      SDeregTTsk(ENTHI, 0);
      SDeregTTsk(ENTSM, 0);

      RETVALUE(RFAILED);
   }
#endif /* EG */
   
   /* Register SGW */
   if (SRegTTsk(
            (Ent)ENTEU, (Inst)0, (Ttype)TTNORM, PRIOR0,
            QoActvInit, QoActvTsk ) != ROK)
   {
      QOSM_DBG_ERROR((QOSM_PRNTBUF, "SRegTTsk() failed for qoSgwActvInit()\n"));
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
         File:   qosm_tst.c
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
  
   /* qo003.101 : CR 7468 debug mask correction */
   if((ret = qoRegTsk())!= ROK)
   {
      RETVALUE(RFAILED);
   }
   
   if((ret = qoCreateTsk())!= ROK)
   {
      RETVALUE(RFAILED);
   }
   
   if((ret = qoAttachTsk()) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Trigger State machine from here */  
   qoSmMain();


   RETVALUE(ROK);
} /* end of tst() */


/*
 *
 *       Fun:    qoSmExit - graceful exit from acceptance tests
 *
 *       Desc:   
 *
 *       Ret:    Void
 *
 *       Notes:  None
 *
         File:   qosm_tst.c
 *
 */

#ifdef ANSI
PUBLIC Void qoSmExit
(
Void
)
#else
PUBLIC Void qoSmExit()
#endif /* ANSI */
{
   SM_TRC2(qoSmExit);

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

#ifdef QO_SGW
      SDetachTTsk(   
      (Ent)ENTEG, (Inst)0);

        SDeregTTsk(
       (Ent)ENTEG, (Inst)0);
         
   SDetachTTsk(
         (Ent)ENTEU,(Inst)0 );
   SDeregTTsk(
         (Ent)ENTEU, (Inst)0);
#endif

   SDestroySTsk(hiTskId);

#ifdef QO_SGW
   SDestroySTsk(euTskId);
#endif
   SDestroySTsk(smTskId);

   exit(0);
   /*klockworks fix : after exit(0) the return is unreachable*/
} /* end of qoSmExit() */

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
*       File:  it_acc1.c
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
      qoSmExit();
   }
   qosmParseRdConQ(data);

   
   RETVALUE(OK);
}/* end of  rdConQ()*/


/**********************************************************************

         End of file:     qosm_tst.c@@/main/1 - Thu Jul  8 21:22:23 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -          sp                1.LTE-CNE:SGW Initial Release 
/main/1    qo003.101    ve                1.HI .[hx] include files removed.
                        ms                1.debug mask correction.
*********************************************************************91*/
