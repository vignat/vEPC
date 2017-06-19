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
  
     Name:     TCP UDP Convergence Layer Sample Stack Manager
  
     Type:     C include file
  
     Desc:     Error file 
              
     File:     smhi_err.h
  
     Sid:      smhi_err.h@@/main/5 - Mon Mar  3 20:09:28 2008

     Prg:      asa
  
*********************************************************************21*/

#ifndef __SMHIERRH_
#define __SMHIERRH_

/* error codes */
#define SMHILOGERROR(ent, inst, procId, errCls, errCode, errVal, errDesc) \
        SLogError(ent, inst, procId, __FILE__, __LINE__,                  \
                  (ErrCls)errCls, (ErrCode)errCode, (ErrVal)errVal,       \
                  (Txt*)errDesc)

/* Error codes for TUCL sample layer manager */
#define   ERRSMHIBASE   0
#define   ERRSMHI       ERRSMHIBASE

#define   ESMHI001      (ERRSMHI +    1)    /*   smhiexms.c: 202 */

#define   ESMHI002      (ERRSMHI +    2)    /*   smhiptmi.c: 418 */
#define   ESMHI003      (ERRSMHI +    3)    /*   smhiptmi.c: 461 */
#define   ESMHI004      (ERRSMHI +    4)    /*   smhiptmi.c: 499 */
#define   ESMHI005      (ERRSMHI +    5)    /*   smhiptmi.c: 538 */

#endif /* __SMHIERRH_*/


/********************************************************************30**
 
         End of file:     smhi_err.h@@/main/5 - Mon Mar  3 20:09:28 2008

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
  
  version    initials                   description
-----------  ---------  ------------------------------------------------
 
*********************************************************************71*/
 
/********************************************************************80**
 
*********************************************************************81*/
/********************************************************************90**
 
    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
1.1         ---      asa  1. initial release.
/main/2     ---      cvp  1. changed the copyright header.
/main/3     ---      cvp  1. changed the copyright header.
/main/4      ---      kp   1. Updated for release 1.5.
/main/5      ---       hs   1. Updated for release of 2.1
*********************************************************************91*/
