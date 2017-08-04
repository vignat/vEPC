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

     Name:     Trillium LTE SGW Application 

     Type:     C source file

     Desc:     C Upper Interface structures

     File:     qo_eu.x

     Sid:      qo_eu.x@@/main/1 - Thu Jul  8 21:22:19 2010

     Prg:      rs

*********************************************************************21*/
#ifndef __EU_X__
#define __EU_X__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*
*   eu.x - eGTP user typedefs and function declarations
*
*   typedefs and function prototypes declared in this file correspond to 
*   typedefs function prototypes used by the following TRILLIUM software:
*     part no.                      description
*     --------    ----------------------------------------------
*     1000XXX                      eGTP
*/

/* function declarations */

#ifdef SS_MULTIPLE_PROCS
EXTERN S16 euActvInit ARGS((ProcId procId,Ent ent,Inst inst,Region region,Reason reason,
                            Void **xxCb));
#else /*SS_MULTIPLE_PROCS*/
EXTERN S16 euActvInit       ARGS((Ent ent, Inst inst, Region region, Reason
                                  reason));
#endif /*SS_MULTIPLE_PROCS*/
EXTERN S16 euInitExt        ARGS((void));
EXTERN S16 euActvTsk        ARGS((Pst *pst, Buffer *mBuf));


#ifdef __cplusplus
}
#endif /* _cplusplus */
#endif /* __EU_X__ */

/**********************************************************************

         End of file:     qo_eu.x@@/main/1 - Thu Jul  8 21:22:19 2010

**********************************************************************/

/********************************************************************40**

        Notes:

*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

/********************************************************************60**

        Revision history:

*********************************************************************61*/

/********************************************************************80**

*********************************************************************81*/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -      sp                   1.LTE-CNE:SGW Initial Release
*********************************************************************91*/

