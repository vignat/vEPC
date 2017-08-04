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
  
     Name:     multi-threaded
  
     Type:     C include file
  
     Desc:     Structures, variables and typedefs required by the
               multi-threaded
  
     File:     mt.x
  
     Sid:      mt.x@@/main/1 - Mon Apr  5 18:19:16 2010

     Prg:      ada

*********************************************************************21*/
  
#ifndef __MTX__
#define __MTX__


/* typedefs */

/*
 * dynamic pool
 */
typedef struct mtDPool
{
   MtMtxId  mtx;                /* mutex for this pool */
   Queue    bufQ;               /* Queue of messages */
   S16      size;               /* Pool size - absolute value */
   U16      status;             /* Status indicator */
} MtDPool;

/*
 * static pool
 */
typedef struct mtSPool
{
   MtMtxId mtx;                 /* mutext */
   Size  memResv;               /* memory reservation (SGetSMem) */
   Data *memStrt;               /* memory start */
   Data *memEnd;                /* memory end */
   SBuf *avlStrt;               /* available start */
   Data *avlEnd;                /* available end */
   Size crntSize;               /* current size */
   Size bufSize;                /* buffer size */
   Size minSize;                /* minimum size */
   Size maxSize;                /* maximum size */
} MtSPool;

typedef struct mtTmrCp
{
   CmTqCp   tmrTqCp;
   CmTqType tmrTq[MT_TQNUMENT];
   MtMtxId  mtx;            /* mutex id */
   MtCondId cond;           /* condition id  */
   MtThrdId tmrThrd;        /* interrupt handler thread (tmr interrupts) */
}MtTmrCp;

/*
 * each pool of dBufs has the following structure:
 */
typedef struct mtDq             /* demand queue */
{
   MtMtxId  mtx;                /* mutex for this queue */
   MtCondId cond;               /* condition variable */
   Queue    bufQ;               /* Queue of events (messages) */
} MtDq;

typedef struct mtTsk
{
   Ent        ent;              /* Entity */
   Inst       inst;             /* Instance */
   ActvTsk    actvTsk;          /* activation function */
}MtTsk;

typedef struct mtDrvr
{
   Inst       inst;             /* instance */
   ProcId     low;              /* low processor id */
   ProcId     high;             /* high processor id */
   ActvTsk    actvTsk;          /* activation function */
}MtDrvr;

typedef struct mtDrvrEntry
{
   U32        index;              /* task index */
   MtDq       dq;                 /* demand queue */
   MtThrdId   tId;                /* task id */
   U32        count;              /* activity counter */
   Inst       inst;             /* instance */
   ProcId     low;              /* low processor id */
   ProcId     high;             /* high processor id */
   ActvTsk    actvTsk;          /* activation function */
} MtDrvrEntry;


typedef struct mtTskEntry
{
   U32        index;              /* task index */
   U32        type;               /* task type */
   MtDq       dq;                 /* demand queue */
   MtThrdId   tId;                /* task id */
   U32        count;              /* activity counter */
   U32        tskCnt;             /* # of tasks supported by this thread */
   MtTsk      tsks[MT_MAX_TSKCNT];/* maximum tasks */
} MtTskEntry;

typedef struct mtTmrPlace
{
   MtDq       dq;                 /* demand queue pointer */
   MtThrdId   tId;                /* task id */
} MtTmrPlace;   
 
typedef struct mtTmrEntry
{
   S32        timerId;          /* timer id */
   Ent        ent;              /* timer pst */
   Inst       inst;             /* instance */
   PFS16      tmrFnct;          /* timer function */
   MtDq       *dq;              /* demand queue pointer */
   U16        delay;            /* delay - timer period */
   U32        sent;             /* # of msgs sent */
   U32        drop;             /* # of time-outs dropped */
   U32        received;         /* # of msgs received */
   CmTimer    timers[TMR_DEF_MAX]; /* timers */
} MtTmrEntry;

typedef struct mtOsCp
{
   MtDPool dPool[MT_NMB_MSG_POOL];    /* dynamic pools */
   MtSPool sPool;                     /* statis pool */
   MtMtxId tskTblMtx;                 /* task table mutex */
   MtTskEntry *tskTbl[MT_MAX_TASK];   /* task table */
   MtMtxId drvrTblMtx;                /* driver table mutex */
   MtDrvrEntry *drvrTbl[MT_MAX_DRVR]; /* driver table */
   MtMtxId tmrTblMtx;                 /* timer table mutex */
   MtTmrEntry *tmrTbl[MT_MAX_TIMER];  /* timer table */
   MtTmrCp tmrCp;                     /* control point for timers */
   MtTmrPlace tmrPlace;               /* timer placement on expiry */
   MtThrdId intHndlrThrd;             /* thread to handle all interrupts */
   U32 tskTblIdx;                     /* task table index */
   U32 drvrTblIdx;                    /* driver table index */
   U32 tmrTblIdx;                     /* timer table index */
   ProcId ownProcId;                  /* own proc id */
   Bool permTask;                     /* permanent task threads */
   Ticks sysTicks;                    /* system ticks */
   MtThrdId rdConThrdId;              /* read console task */
} MtOsCp;                             /* operating system control point */


typedef struct mtPoolInfo
{
   MsgLen strtIdx;              /* start index */
   MsgLen endIdx;
   S32 dSize;
   S32 nmbBufs;
}MtPoolInfo;



/* partition */

/* message */

/* queue */

/* tasks */


#endif

  
/********************************************************************30**

         End of file:     mt.x@@/main/1 - Mon Apr  5 18:19:16 2010

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

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------
1.1          ---  ada   1. initial release 

*********************************************************************81*/

