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
 
     Name:     System Services -- Queueing
 
     Type:     C Include file
 
     Desc:     System Services queuing functions.
 
     File:     ss_queue.h
 
     Sid:      ss_queue.h@@/main/2 - Mon Apr  5 18:01:19 2010
 
     Prg:      bsr
  
*********************************************************************21*/


#ifndef __SSQUEUEH__
#define __SSQUEUEH__

#ifdef __cplusplus
extern "C" {
#endif


#define SS_MAX_TASK_PRI         4               /* maximum task priorities */
#define SS_MAX_MSG_PRI          4               /* maximum msg priorities */


#define SS_DQ_FIRST             0               /* queue at beginning */
#define SS_DQ_LAST              1               /* queue at end */


#define SS_MAX_NUM_DQ           (SS_MAX_TASK_PRI * SS_MAX_MSG_PRI)
                                                /* number of queues */
#define SS_DQ_BIT_MASK_LEN      ((SS_MAX_NUM_DQ - 1) /8 + 1)
                                                /* length of bitmask */

#define SS_MAX_DQ_PRIOR         (SS_MAX_TASK_PRI * SS_MAX_MSG_PRI) 


/* macros for first and last calls */
#define ssDmndQPutFirst(dQueue, mBuf, priority) \
        ssDmndQPut(dQueue, mBuf, (Prior)priority, SS_DQ_FIRST)
 
#define ssDmndQPutLast(dQueue, mBuf, priority) \
        ssDmndQPut(dQueue, mBuf, (Prior)priority, SS_DQ_LAST)
 
#define ssDmndQGetFirst(dQueue, mBuf) \
        ssDmndQGet(dQueue, mBuf, SS_DQ_FIRST)

#define ssDmndQGetLast(dQueue, mBuf) \
         ssDmndQGet(dQueue, mBuf, SS_DQ_LAST)
 

#ifdef __cplusplus
}
#endif

#endif /* __SSQUEUEH__ */



/********************************************************************30**
  
         End of file:     ss_queue.h@@/main/2 - Mon Apr  5 18:01:19 2010
  
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
1.1          ---      bsr  1. initial release.

1.2          ---      kp   1. C++ compilable, cosmetic changes

1.3          ---      kp   1. Corrected a typo

/main/4      ---      rp   1. SSI enhancements for Multi-core architecture
                              support
*********************************************************************91*/

