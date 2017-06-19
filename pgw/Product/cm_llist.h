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
  
     Name:     common - linked list functions
  
     Type:     C include file
  
     Desc:     macros for linked list functions
 
     File:     cm_llist.h
  
     Sid:      cm_llist.h@@/main/nodal_main/1 - Thu Nov 21 09:40:13 2013
    
     Prg:      ak
 
*********************************************************************21*/
  
#ifndef __CMLLISTH__
#define __CMLLISTH__


/* cm_llist_h_001.main_8 */
#define cmLListFirst(l)  ((l)->crnt = (l)->first)
#define cmLListLast(l)   ((l)->crnt = (l)->last)
#define cmLListCrnt(l)   ((l)->crnt)
#define cmLListNext(l)   ((l)->crnt = ((l)->crnt ? (l)->crnt->next : \
                                       (l)->first))
#define cmLListPrev(l)   ((l)->crnt = ((l)->crnt ? (l)->crnt->prev : \
                                       (l)->first))
#define cmLListLen(l)    ((l)->count)
#define cmLListNode(n)   ((n)->node)

#define CM_LLIST_FIRST_NODE(l, n)  ((((n) = cmLListFirst(l)) != NULLP) ? (n)->node : NULLP)
#define CM_LLIST_NEXT_NODE(l, n)   ((((n) = cmLListNext(l))!= NULLP) ? (n)->node : NULLP)
#define CM_LLIST_PREV_NODE(l, n)   ((((n) = cmLListPrev(l)) != NULLP) ? (n)->node : NULLP)


#endif /* __CMLLISTH__ */

 
/********************************************************************30**
  
         End of file:     cm_llist.h@@/main/nodal_main/1 - Thu Nov 21 09:40:13 2013
  
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
1.1          ---  ak    1. initial release

1.2          ---  bw    1. add 2 more macros for faster access
             ---  rrb   1. changed the macros CM_LLIST_FIRST_NODE and 
                           CM_LLIST_NEXT_NODE.
*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
1.3          ---      rrb  1. changed the macros CM_LLIST_FIRST_NODE and
                              CM_LLIST_NEXT_NODE.
1.4          ---      bbk  1. Changed copyright header date.
1.5          ---      ag   1. Added CM_LLIST_PREV_NODE 
             ---      ag   2. Changed copyright header
/main/7      ---     rbabu 1. Updated for NBAP software release 1.2
cm_llist_h_001.main_7 --- rbhat 1. Modified Linked List Macros
/main/8      ---      
cm_llist_h_001.main_8 --- rbhat 1.Modified Linked list Macros for lvalue 
/main/9      ---      
 *********************************************************************91*/
