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
  
     Name:     multi-thread (unix)
  
     Type:     C include file
  
     Desc:     Defines required by mt ss implementation
  
     File:     mt.h
  
     Sid:      mt.h@@/main/1 - Mon Apr  5 18:19:16 2010

     Prg:      ada
  
*********************************************************************21*/

#ifndef __MTH__
#define __MTH__


/* defines */

/*
 * TUNE THESE TO YOUR ENVIRONMENT 
 */
#define MT_MAX_TSKCNT       5   /* maximum # of tasks per thread */
#define MT_MAX_TASK         40  /* maximum # of activation tasks */
#define MT_MAX_DRVR         5   /* maximum # of drivers */
#define MT_MAX_TIMER        40  /* maxumum # of timers */
#define MT_MAX_SIG_HNDLRS   5   /* number of signal handlers (Vects) */
#define MT_MAX_EXIT_FNCTS   5   /* number of "AtExit" functions */
#define MT_MAX_MUTEX        40  /* maximum # of mutexes */
#define MT_MAX_COND         40  /* maximum # of conditions */
#define MT_MAX_THREAD       60  /* maximum # of threads */

#define MT_NMB_MSG_POOL     4

/* pool three used for message buffers (mBufs) */
#define MT_POOL_3_ID        MT_MBUF_POOL
#define MT_POOL_3_NMB_BUFS  1000
#define MT_POOL_3_DSIZE     0
#define MT_POOL_3_STRTIDX   0
#define MT_POOL_3_ENDIDX    0

/* pool two used for timer buffers (dBufs) */
#define MT_POOL_2_ID        MT_TMR_POOL
#define MT_POOL_2_NMB_BUFS  200
#define MT_POOL_2_DSIZE     32
#define MT_POOL_2_STRTIDX   16
#define MT_POOL_2_ENDIDX    (MT_POOL_2_DSIZE-1)

/* pool one used for wrapper protocol traffic (dBufs) */
#define MT_POOL_1_ID        MT_WRAP_POOL
#define MT_POOL_1_NMB_BUFS  300
#define MT_POOL_1_DSIZE     1600 /* 1576 max ethernet packet */
#define MT_POOL_1_STRTIDX   100
#define MT_POOL_1_ENDIDX    (MT_POOL_1_DSIZE-1)

/* pool zero used for normal protocol traffic (dBufs) */
#define MT_POOL_0_ID        MT_PROT_POOL
#define MT_POOL_0_NMB_BUFS  1000
#define MT_POOL_0_DSIZE     1000
#define MT_POOL_0_STRTIDX   300
#define MT_POOL_0_ENDIDX    (MT_POOL_0_DSIZE-1)

#define MT_SPOOL_SIZE       0x80000 /* static pool size (512K) */

#define MT_MSG_POOL_UNINITIALIZED     0
#define MT_MSG_POOL_INITIALIZED       1

#define MT_TMR_EVNT            1
#define MT_MSG_EVNT            2

#define MT_TSK_NULL            0
#define MT_TSK_NORM            1
#define MT_TSK_PERM            2

#define MT_THRDID_NULL         -1
#define MT_TMRID_NULL          -1

#define MT_TICK_CNT         0x186A0      /* how many micro sec per 
                                            .1 seconds */

#define MT_TQNUMENT         64           /* # of buckets */

#ifndef NOCONAVL
#define CONAVL                           /* console available? */
#endif  /* NOCONAVL */

#endif /* __MTH__ */

  
/********************************************************************30**
  
         End of file:     mt.h@@/main/1 - Mon Apr  5 18:19:16 2010

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
1.1          ---  ada    1. initial release

*********************************************************************81*/

