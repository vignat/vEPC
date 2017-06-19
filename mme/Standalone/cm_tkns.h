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
  
     Name:     general layer
  
     Type:     C include file
  
     Desc:     defines , required by cm_tkns.c and cm_tkns.h
   
     File:     cm_tkns.h
  
     Sid:      cm_tkns.h@@/main/5 - Thu Oct 27 00:36:59 2005
  
     Prg:      mb
  
*********************************************************************21*/

#ifndef __CMTKNSH
#define __CMTKNSH

  
/*
*     The structures and variables declared in this file
*     correspond to structures and variables used by
*     the following TRILLIUM software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*
*/
  
/*
*     This software may be combined with the following TRILLIUM
*     software:
*
*     part no.                      description
*     --------    ----------------------------------------------
*
*/


/* defines */
#define cmPkTknPres(x, mBuf)       cmPkTknU8(x, mBuf)

#define cmUnpkTknPres(x, mBuf)     cmUnpkTknU8(x, mBuf)


/* packing and unpacking for token bit strings */

#define CMPKTKNBSTR(tknStr, mBuf)                   \
{                                                  \
   Cntr i;                                         \
   Cntr len;                                       \
                                                   \
   if(tknStr->pres)                                \
   {                                               \
      if (tknStr->len % 8)                         \
         len = (tknStr->len/8 ) + 1;               \
      else                                         \
         len = (tknStr->len/8 );                   \
                                                   \
      /* Value */                                  \
      for (i = 0; i < (S16) len; i++)              \
      {                                            \
         CMCHKPK(SPkU8, tknStr->val[i], mBuf);     \
      }                                            \
                                                   \
      /* Length */                                 \
      CMCHKPK(SPkU8, tknStr->len, mBuf);           \
   }                                               \
                                                   \
   /* Token Header */                              \
   CMCHKPK(SPkU8, tknStr->pres, mBuf);             \
}

#define CMUNPKTKNBSTR(tknStr, mBuf)                                 \
{                                                                  \
   Cntr i;                                                         \
   Cntr len;                                                       \
                                                                   \
   /* Token Header */                                              \
   CMCHKUNPK(SUnpkU8, &tknStr->pres, mBuf);                        \
                                                                   \
   if(tknStr->pres)                                                \
   {                                                               \
     /* Length */                                                  \
      CMCHKUNPK(SUnpkU8, &tknStr->len, mBuf);                      \
                                                                   \
      if (tknStr->len % 8)                                         \
         len = (tknStr->len/8 ) + 1;                               \
      else                                                         \
         len = (tknStr->len/8 );                                   \
                                                                   \
      /* Value */                                                  \
      for (i = 1; i <= (S16) len; i++)                             \
      {                                                            \
         CMCHKUNPK(SUnpkU8, &tknStr->val[len - i], mBuf);          \
      }                                                            \
   }                                                               \
                                                                   \
}

#endif /* __CMTKNSH */

  
/********************************************************************30**
  
         End of file:     cm_tkns.h@@/main/5 - Thu Oct 27 00:36:59 2005
    
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


********************************************************************81*/

/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
  1.1        ---      mb   1. Initial Release
  /main/3    ---      dw   1. Added pk/unpk macros for TKNBSTR
/main/4      ---      cvp  1. Merging with latest branch.
/main/5      ---      rbabu 1. Updated for NBAP software release 1.2
*********************************************************************91*/
