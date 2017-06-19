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

     Name:     eGTP Layer

     Type:     C source file

     Desc:     C Upper Interface structures

     File:     smeg_apputl.x

     Sid:      smeg_apputl.x@@/main/3 - Fri Feb  4 18:18:24 2011

     Prg:      ad

*********************************************************************21*/


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


PUBLIC Void egMakeIeEntry ARGS
((
LegIeEntry      *ieEntry,
U8             ieCode,
Bool           valid,
U8             dataType,
Ptr            fncPtr,
U16             minLen,
U16             maxLen
));

PUBLIC Void egMakeAppChildIeEntry ARGS
((
LegGroupedIe *grpIe,
U8             count,
U8            ieCode,
U8            ieClass,
U8            ieCr,
U8            ieInst,
U8             nmbGrpIe
));

PUBLIC Void egMakeAppIeProp ARGS
((
LegIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass,     /* Mandatory/Conditional/Optional */
U8             instance,     /* Instance  */
U8                  cr
));


#ifdef __cplusplus
}
#endif /* __cplusplus */

/**********************************************************************
 
         End of file:     smeg_apputl.x@@/main/3 - Fri Feb  4 18:18:24 2011
 
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
/main/1      ---      sn   Initial Release
/main/2      ---      rkumar   1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla         Initial for eGTP release 2.1
*********************************************************************91*/
