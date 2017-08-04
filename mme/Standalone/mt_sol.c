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
  
     Name:     MTSS - source
  
     Type:     C source file
  
     Desc:     MTSS specific System Services
 
     File:     mt_sol.c

     Sid:      pt_sol.c@@/main/1 - Mon Apr  5 18:19:19 2010
  
     Prg:      ada
  
*********************************************************************21*/
 

/* header include files (.h) */
#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system service interface */
#include "cm5.h"           /* common timer must come first! */
#include "mt.h"            /* mt.h */
#include "mt_err.h"        /* mt err */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer */
#include "ssi.x"           /* system service interface */
#include "cm5.x"           /* common timer must come first! */
#include "mt.x"            /* mt.x */

#ifdef MT_SOL
#include <thread.h>
#include <signal.h>
#endif /* MT_SOL */

/* local defines */

/* local typedefs */

typedef struct _sigList
{
   S32 nmbHndlrs;               /* number of handlers */
   PFVOID sigHndlrs[MT_MAX_SIG_HNDLRS];
}SigList;

typedef struct _exitList
{
   S32     nmbFncts;
   MtThrd  *exitFncts[MT_MAX_EXIT_FNCTS];
   Void    *exitArgs[MT_MAX_EXIT_FNCTS];
}ExitList;

PRIVATE SigList sigs[NSIG]; /* list of signal handlers */
PRIVATE ExitList exitList;


/*
 * The following tables store pre-allocated (in this case at compile time)
 * data required to manage threads (mutexes, conditions, threads). The reason
 * for this is to hide from the user the need to include system dependent
 * data structures like mutex_t, con_t, and thread_t. This adds some overhead,
 * and runtime limitations but should be a win come portation time
 */

/*
 * mutex list
 */
typedef struct _mtxLst
{
   mutex_t mutex;               /* mutex */
   S32 mCnt;                    /* mutex count */
   Bool used[MT_MAX_MUTEX];     /* used */
   mutex_t mtx[MT_MAX_MUTEX];   /* mutex list  */
   MtThrdId owner[MT_MAX_MUTEX];
}MtxLst;

/*
 * condition list
 */
typedef struct _cndLst
{
   mutex_t mutex;               /* mutex */
   S32 cCnt;                    /* count */
   Bool used[MT_MAX_COND];      /* used */
   cond_t cnd[MT_MAX_COND];     /* cond list  */
}CndLst;

typedef struct _thrdLst
{
   mutex_t mutex;               /* mutex */
   S32 tCnt;                    /* count */
   Bool used[MT_MAX_THREAD];    /* used */
   thread_t thrd[MT_MAX_THREAD]; /* thread list  */
}ThrdLst;

/* local externs */
  
/* forward references */


PUBLIC S16 mtInit ARGS((void));
PUBLIC Void *mtIntrThrd ARGS((Ptr ptrVal));
PUBLIC Void mtTmrVect ARGS((void));

/* local function definition */
PUBLIC S16 mtThrdSelf ARGS((MtThrdId *tId));
PUBLIC S16 mtPutVect ARGS((VectNmb vNmb, PIF vFnct));
PRIVATE Void mtPrcTmr ARGS((void));

/* functions in other modules */
EXTERN Void mtTmrEvnt ARGS((PTR tCb, S16 evnt));
/* this is cludgy but, thr_sigsetmask is not prototype when strict ansi
 * is turned on with gcc.
 */
#if (__STDC__ - 0 == 1)
EXTERN S32 thr_sigsetmask ARGS((S32, CONSTANT sigset_t *, sigset_t *));
#endif /* (__STDC__ - 0 == 1) see /usr/include/thread.h */
/* can't find this in any header files... */
EXTERN S32 sigwait ARGS((sigset_t *));


/* public variable declarations */
EXTERN MtOsCp mtOsCp;
/* control variables for testing */

/* private variable declarations */
PRIVATE MtxLst mtxList;
PRIVATE CndLst cndList;
PRIVATE ThrdLst thrList;

/*
 * support functions
 */
#ifdef MT

/*
*
*       Fun:   MtInit
*
*       Desc:  Initialize MT routines
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: main initialization routine 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 mtInit
(
void
)
#else
PUBLIC S16 mtInit()
#endif
{
   REG1 S32 i;
   sigset_t set;

   TRC2(mtInit)

   /* Block all signals in main thread */
   /* All threads created after this will inherit the same signal mask */
   sigfillset(&set);      /* initialize set to include all signals defined */
                          /* by the system */
   thr_sigsetmask(SIG_SETMASK, &set, NULL); /* mask all signals defined by */
                                            /* set */

   /*
    * initialize mutex list
    */
   if (mutex_init(&mtxList.mutex, USYNC_THREAD, NULL) != 0)
      RETVALUE(RFAILED);
   mtxList.mCnt = 0;
   for (i = 0; i < MT_MAX_MUTEX; i++)
      mtxList.used[i] = FALSE;

   /*
    * initialize condition list
    */
   if (mutex_init(&cndList.mutex, USYNC_THREAD, NULL) != 0)
      RETVALUE(RFAILED);
   cndList.cCnt = 0;
   for (i = 0; i < MT_MAX_COND; i++)
      cndList.used[i] = FALSE;

   if (mutex_init(&thrList.mutex, USYNC_THREAD, NULL) != 0)
      RETVALUE(RFAILED);
   thrList.tCnt = 0;
   for (i = 0; i < MT_MAX_COND; i++)
      thrList.used[i] = FALSE;
   
   for (i = 0; i < NSIG; i++)
   {
      sigs[i].nmbHndlrs = 0;
   }

   exitList.nmbFncts = 0;
   
   RETVALUE(ROK);
} /* end of mtInit */


/*
*
*       Fun:   SGetMutex
*
*       Desc:  create and initialize a mutex
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: create a mutex
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetMutex
(
MtMtxId *mId                    /* mutex (returned) */
)
#else
PUBLIC S16 SGetMutex(mId)
MtMtxId *mId;                   /* mutex (returned) */
#endif
{
   REG1 S32 i;
   
   TRC2(SGetMutex)

   if (mutex_lock(&mtxList.mutex) != 0)
   {
      RETVALUE(RFAILED);
   }      
   
   if (mtxList.mCnt < MT_MAX_MUTEX)
   {
      for ( i = 0; i < MT_MAX_MUTEX; i++)
      {
         if (mtxList.used[i] == FALSE)
         {
            if (mutex_init(&mtxList.mtx[i], USYNC_THREAD, NULL) != 0)
               RETVALUE(RFAILED);
            mtxList.used[i] = TRUE;
            mtxList.mCnt++;
            if (mutex_unlock(&mtxList.mutex) != 0)
               RETVALUE(RFAILED);
            *mId = i;
            RETVALUE(ROK);
         }
      }
   } 
   
   mutex_unlock(&mtxList.mutex);
   RETVALUE(RFAILED);
} /* end of SGetMutex */
   

/*
*
*       Fun:   SPutMutex
*
*       Desc:  destroy any state associated with a mutex
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: Destroy a mutex 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SPutMutex
(
MtMtxId mId                     /* mutex id */
)
#else
PUBLIC S16 SPutMutex(mId)
MtMtxId mId;                    /* mutex id */
#endif
{

   TRC2(SPutMutex)

   if (mutex_lock(&mtxList.mtx[mId]) != 0)
      RETVALUE(RFAILED);
   
   if (mutex_destroy(&mtxList.mtx[mId]) != 0)
      RETVALUE(RFAILED);

   mtxList.used[mId] = FALSE;
   mtxList.mCnt--;
   
   mutex_unlock(&mtxList.mtx[mId]);
   RETVALUE(ROK);
} /* end of SPutMutex */


/*
*
*       Fun:   SLockMutex
*
*       Desc:  lock a mutex
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SLockMutex
(
MtMtxId mId                     /* mutex id */
)
#else
PUBLIC S16 SLockMutex(mId)
MtMtxId mId;
#endif
{
   TRC2(SLockMutex)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mtxList.used[mId] != TRUE)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT066, mId, "illegal mutex id");      
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (mutex_lock(&mtxList.mtx[mId]) == 0)
   {
      MtThrdId tId;
      
      /*
       * stamp owner for debugging purposes
       */
      mtThrdSelf(&tId);
      mtxList.owner[mId] = tId;
      RETVALUE(ROK);
   }

   /* mutex_lock must have failed */
   RETVALUE(RFAILED);
} /* end of SLockMutex */


/*
*
*       Fun:   SUnlockMutex
*
*       Desc:  Unlock a mutex
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SUnlockMutex
(
MtMtxId mId
)
#else
PUBLIC S16 SUnlockMutex(mId)
MtMtxId mId;
#endif
{

   TRC2(SUnlockMutex)
      
#if (ERRCLASS & ERRCLS_INT_PAR)
   if (mtxList.used[mId] != TRUE)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT067, mId, "Illegal mutex id");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   mtxList.owner[mId] = -1;
   if (mutex_unlock(&mtxList.mtx[mId]) != 0)
      RETVALUE(RFAILED);
   
   RETVALUE(ROK);
} /* end of SUnlockMutex */


/*
*
*       Fun:   SGetCond
*
*       Desc:  create and initialize a condition variable
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetCond
(
MtCondId *cId                    /* condition id (returned) */
)
#else
PUBLIC S16 SGetCond(cId)
MtCondId *cId;                   /* condition id (returned) */
#endif
{
   REG1 S32 i;
   
   TRC2(SGetCond)

  if (mutex_lock(&cndList.mutex) != 0)
     RETVALUE(RFAILED);

   if (cndList.cCnt < MT_MAX_COND)
   {
      for ( i = 0; i < MT_MAX_COND; i++)
      {
         if (cndList.used[i] == FALSE)
         {
            if (cond_init(&cndList.cnd[i], USYNC_THREAD, NULL) != 0)
            {
               mutex_unlock(&cndList.mutex);
               RETVALUE(RFAILED);
            }
            cndList.used[i] = TRUE;
            cndList.cCnt++;
            *cId = i;
            if (mutex_unlock(&cndList.mutex) != 0)
               RETVALUE(RFAILED);
            RETVALUE(ROK);
         }
      }
   } 

   mutex_unlock(&cndList.mutex);
   RETVALUE(RFAILED);
} /* end of SGetCond */
   

/*
*
*       Fun:   SPutCond
*
*       Desc:  destroy any state associated with a Cond
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SPutCond
(
MtCondId cId                     /* Cond id */
)
#else
PUBLIC S16 SPutCond(cId)
MtCondId cId;                    /* Cond id */
#endif
{
   
   TRC2(SPutCond)

   if (mutex_lock(&cndList.mutex) != 0)
      RETVALUE(RFAILED);
      
   if (cond_destroy(&cndList.cnd[cId]) != 0)
      RETVALUE(RFAILED);
   
   cndList.used[cId] = FALSE;
   cndList.cCnt--;
   
   mutex_unlock(&cndList.mutex);
   RETVALUE(ROK);
} /* end of SPutCond */


/*
*
*       Fun:   SCondWait
*
*       Desc:  condition wait
*
*       Ret:   ROK on success
*              
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SCondWait
(
MtMtxId mId,
MtCondId cId
)
#else
PUBLIC S16 SCondWait(mId, cId)
MtMtxId mId;
MtCondId cId;
#endif
{
   S32 ret;
   
   TRC2(SCondWait)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (!mtxList.used[mId])
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT068, mId, "illegal mutex");   
      RETVALUE(RFAILED);
   }
   
   if (!cndList.used[cId])
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT069, cId, "illegal condition");      
      RETVALUE(RFAILED);
   }
   
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */   

   if ((ret = cond_wait(&cndList.cnd[cId], &mtxList.mtx[mId])) != 0)
      RETVALUE(RFAILED);
   
   RETVALUE(ROK);
} /* end of SCondWait */


/*
*
*       Fun:   SCondSignal
*
*       Desc:  Condition Signal
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SCondSignal
(
MtCondId cId
)
#else
PUBLIC S16 SCondSignal(cId)
MtCondId cId;
#endif
{

   TRC2(SCondSignal)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (!cndList.used[cId])
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT070, cId, "illegal condition");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (cond_signal(&cndList.cnd[cId]) != 0)
      RETVALUE(RFAILED);
         
   RETVALUE(ROK);
} /* end of SCondSignal */

/*
*
*       Fun:   SCondBroadcast
*
*       Desc:  Send a condition signal to all threads waiting on this
*              condition
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: Condition Broadcast
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SCondBroadcast
(
MtCondId cId
)
#else
PUBLIC S16 SCondBroadcast(cId)
MtCondId cId;
#endif
{

   TRC2(SCondBroadcast)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (!cndList.used[cId])
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT071, cId, "illegal condition");      
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */

   if (cond_broadcast(&cndList.cnd[cId]) != 0)
      RETVALUE(RFAILED);
         
   RETVALUE(ROK);
} /* end of SCondBroadcast */


/*
*
*       Fun:   SGetThread
*
*       Desc:  Get Thread 
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: Creates a thread of execution
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SGetThread
(
MtThrd thrd,                    /* thread function */
MtThrdFlags thr_flgs,           /* thread flags */
Ptr arg,                        /* thread function argument */
MtThrdId *thrdId                /* thread function id */
)
#else
PUBLIC S16 SGetThread(thrd, thr_flgs, arg, thrdId, type)
MtThrd thrd;                    /* thread function */
MtThrdFlags thr_flgs;           /* thread flags */
Ptr arg;                        /* thread function argument */
MtThrdId *thrdId;               /* thread function id */
#endif
{
   REG1 S32 i;
   S32 flags;
   
   TRC2(SGetThread)
      
   /* 
    * convert portable flags
    */
   if (thr_flgs != MT_THR_NOFLAGS)
   {
      /* gather flags */
      flags = 0;
      if (thr_flgs & MT_THR_SUSPENDED)
         flags |= THR_SUSPENDED;
      if (thr_flgs & MT_THR_DETACHED)
         flags |= THR_DETACHED;
      if (thr_flgs & MT_THR_BOUND)
         flags |= THR_BOUND;
      if (thr_flgs & MT_THR_NEW_LWP)
         flags |= THR_NEW_LWP;
      if (thr_flgs & MT_THR_DAEMON)
         flags |= THR_DAEMON;
   }
   else
      flags = 0;
   
   if (mutex_lock(&thrList.mutex) != 0)
      RETVALUE(RFAILED);
   

   if (thrList.tCnt < MT_MAX_THREAD)
   {
      for ( i = 0; i < MT_MAX_THREAD; i++)
      {
         if (thrList.used[i] == FALSE)
         {
            if (thr_create((void*)NULL, (size_t)0, thrd, (void*)arg, 
                           flags, &thrList.thrd[i]) != 0)
            {
               mutex_unlock(&mtxList.mutex);
               RETVALUE(RFAILED);
            }
            thrList.used[i] = TRUE;
            thrList.tCnt++;
            *thrdId = i;
            if (mutex_unlock(&thrList.mutex) != 0)
               RETVALUE(RFAILED);
            RETVALUE(ROK);
         }
      }
   } 
   
   mutex_unlock(&thrList.mutex);
   RETVALUE(RFAILED);
} /* end of SGetThread */


/*
*
*       Fun:   SPutThread
*
*       Desc:  Free a thread
*
*       Ret:   None
*
*       Notes: SPutThread must be called by the same thread which 
*              is being freed. It is assumed that all shared resources
*              (i.e. mutexes, etc) have been freed prior to this call.
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SPutThread
(
MtThrdId tId                /* thread function id */
)
#else
PUBLIC S16 SPutThread(tId)
MtThrdId tId;               /* thread function id */
#endif
{
   TRC2(SPutThread)
   
   mutex_lock(&thrList.mutex);  /* lock the thread list table */
   
   if ((tId >= MT_MAX_THREAD) || 
       (thrList.used[tId] != FALSE) ||
       (thrList.thrd[tId] != thr_self()))
   {
      mutex_unlock(&thrList.mutex); /* unlock the thread list table */
      RETVALUE(RFAILED);
   }
   
   thrList.used[tId] = FALSE;   /* set the table "used" entry to FALSE */
   thrList.tCnt--;              /* decrement count */
   
   mutex_unlock(&thrList.mutex); /* unlock the thread list table */

   thr_exit((void*)0);          /* destroy this thread! */
   RETVALUE(ROK);               /* NOTREACHED */
} /* end of SPutThread */


/*
*
*       Fun:   SThreadYield
*
*       Desc:  yield to another thread of equal or higher priority
*
*       Ret:   None
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC INLINE Void SThreadYield
(
void
)
#else
PUBLIC INLINE Void SThreadYield()
#endif
{

   TRC2(SThreadYield)

   thr_yield();
   
   RETVOID;
} /* end of SThreadYield */


/*
*
*       Fun:   SThreadExit
*
*       Desc:  let thread exit...
*
*       Ret:   None
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC Void SThreadExit
(
Ptr *status                     /* exit status */
)
#else
PUBLIC Void SThreadExit(status)
Ptr *status;                   /* exit status */
#endif
{
   MtThrdId tId;
   
   TRC2(SThreadExit)

   mtThrdSelf(&tId);            /* find our thread id */
   
   mutex_lock(&thrList.mutex);  /* lock the thread list table */
   
   thrList.used[tId] = FALSE;   /* set the table "used" entry to FALSE */
   
   mutex_unlock(&thrList.mutex); /* unlock the thread list table */
   
   thr_exit(status);            /* exit this thread */
   
   RETVOID;
} /* end of SThreadExit */


/*
*
*       Fun:   mtIntrThrd
*
*       Desc:  Handle all interrupts for this process
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: This thread handles ALL signals!
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC Void *mtIntrThrd
(
Ptr ptrVal
)
#else
PUBLIC Void *mtIntrThrd(ptrVal)
Ptr ptrVal;
#endif
{
   struct itimerval locTimer;
   sigset_t set;
   S32 sig;
   REG1 S32 i;
   
   TRC2(mtIntrThrd)
      
   sigfillset(&set);      

   locTimer.it_interval.tv_sec = 0;
   locTimer.it_interval.tv_usec = MT_TICK_CNT;
   locTimer.it_value.tv_sec = 0;
   locTimer.it_value.tv_usec = MT_TICK_CNT; 
   setitimer(ITIMER_REAL, &locTimer, NULLP);
   SPutVect(SIGALRM, mtPrcTmr);
   
   while(1)
   {
      sig = sigwait(&set);      /* block waiting for signal */
      for (i = 0; i < sigs[sig].nmbHndlrs; i++)
      {
         sigs[sig].sigHndlrs[i](); /* handle signal */
      }
   }
      
   /* NOTREACHED */
} /* end of mtIntrThrd */


/*
*
*       Fun:   SSetThrdPrior
*
*       Desc:  set a threads priority
*
*       Ret:   None
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC Void SSetThrdPrior
(
MtThrdId tId,                   /* thread id */
MtThrdPrior tPr                 /* new priority  */
)
#else
PUBLIC Void SSetThrdPrior(tId, tPr)
MtThrdId tId;                   /* thread id */
MtThrdPrior tPr;                /* new priority */
#endif
{

   TRC2(SSetThrdPrior)

   if (thrList.used[tId] != TRUE)
      RETVOID;
   
   (Void)thr_setprio(thrList.thrd[tId], tPr);
   
   RETVOID;
} /* end of SSetThrdPrior */


/*
*
*       Fun:   SGetThrdPrior
*
*       Desc:  get a threads priority
*
*       Ret:   None
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC Void SGetThrdPrior
(
MtThrdId tId,                   /* thread id */
MtThrdPrior *tPr                /* priority (returned) */
)
#else
PUBLIC Void SGetThrdPrior(tId, tPr)
MtThrdId tId;                   /* thread id */
MtThrdPrior *tPr;               /* priority (returned) */
#endif
{

   TRC2(SGetThrdPrior)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (thrList.used[tId] != TRUE)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT072, tId, "illegal thread");   
      RETVOID;
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */
   
   (Void)thr_getprio(thrList.thrd[tId], (int*)tPr);
   
   RETVOID;
} /* end of SGetThrdPrior */


/*
*
*       Fun:   mtThreadSelf
*
*       Desc:  find the current thread id
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 mtThrdSelf
(
MtThrdId *tId
)
#else
PUBLIC S16 mtThrdSelf(tId)
MtThrdId *tId;
#endif
{
   REG1 S32 i;
   thread_t thr;
   
   TRC2(mtThrdSelf)

   thr = thr_self();
   for (i = 0; i < thrList.tCnt; i++)
   {
      if (thrList.thrd[i] == thr)
      {
         *tId = (MtThrdId)i;
         RETVALUE(ROK);
      }      
   }
   
   RETVALUE(RFAILED);
} /* end of mtThrdSelf */


/*
*
*       Fun:   mtPutVect
*
*       Desc:  put vector into vector list
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 mtPutVect
(
VectNmb vectNmb,            /* vector number */
PIF vectFnct                /* vector function */
)
#else
PUBLIC S16 mtPutVect(vectNmb, vectFnct)
VectNmb vectNmb;            /* vector number */
PIF vectFnct;               /* vector function */
#endif
{

   TRC2(mtPutVect)

#if (ERRCLASS & ERRCLS_INT_PAR)
   if (vectNmb >= NSIG)
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT073, vectNmb, "vector out of range");
      RETVALUE(RFAILED);
   }
   
   if (sigs[vectNmb].nmbHndlrs == (MT_MAX_SIG_HNDLRS -1))
   {
      MTLOGERROR(ERRCLS_INT_PAR, EMT074, sigs[vectNmb].nmbHndlrs, 
                 "signal table full");
      RETVALUE(RFAILED);
   }
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */   

   sigs[vectNmb].sigHndlrs[sigs[vectNmb].nmbHndlrs] = (PFVOID)vectFnct;
   sigs[vectNmb].nmbHndlrs++;

   RETVALUE(ROK);
} /* end of mtPutVect */


/*
*
*       Fun:   mtPrcTmr
*
*       Desc:  process timer signal
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC Void mtPrcTmr
(
void
)
#else
PUBLIC Void mtPrcTmr()
#endif
{

   TRC2(mtPrcTmr)
      
   mtOsCp.sysTicks++;
   cmPrcTmr(&mtOsCp.tmrCp.tmrTqCp, mtOsCp.tmrCp.tmrTq, mtTmrEvnt);

   RETVOID;
   
} /* end of mtPrcTmr */


/*
*
*       Fun:   SAtExit
*
*       Desc:  register function to call at exit
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC S16 SAtExit
(
MtThrd exitFnct,
Void *arg
)
#else
PUBLIC S16 SAtExit(exitFnct, arg)
MtThrd exitFnct;
Void *arg;
#endif
{

   TRC2(SAtExit)

   if (exitList.nmbFncts < MT_MAX_EXIT_FNCTS)
   {
      exitList.exitFncts[exitList.nmbFncts] = exitFnct;
      exitList.exitArgs[exitList.nmbFncts++]= arg;
      RETVALUE(ROK);
   }
   else
   {
#if (ERRCLASS & ERRCLS_INT_PAR)   
      MTLOGERROR(ERRCLS_INT_PAR, EMT075, exitList.nmbFncts , 
                 "at exit table full");
#endif /* (ERRCLASS & ERRCLS_INT_PAR) */   
   }
   RETVALUE(RFAILED);   
} /* end of SAtExit */


/*
*
*       Fun:   SExit
*
*       Desc:  call all the registered exit functions then exit
*
*       Ret:   ROK on success
*              RFAILED on error
*
*       Notes: 
*
*       File:  mt_sol.c
*
*/
#ifdef ANSI
PUBLIC Void SExit
(
void
)
#else
PUBLIC Void SExit()
#endif
{
   REG1 S32 i;

   TRC2(SExit)
      
   for (i = 0; i < exitList.nmbFncts; i++)
      exitList.exitFncts[i](exitList.exitArgs[i]);
   

   exit(0);
} /* end of SExit */


#endif /* MT */
  
/********************************************************************30**
  
         End of file:     pt_sol.c@@/main/1 - Mon Apr  5 18:19:19 2010
  
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

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
1.1          ---  ada   1. initial release.
/main/1      ---  ch   1. SSI Multicore Support release.
*********************************************************************91*/
