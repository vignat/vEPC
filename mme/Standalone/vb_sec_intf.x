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

     Name:    Trillium LTE CNE - MME Controller Module

     Type:    C Include file

     Desc:    This file contains c structure for security module inteface

     File:    vb_mme_sec_intf.x

     Sid:     vb_mme_sec_intf.x@@/main/1 - Mon Jun 28 19:35:38 2010

     Prg:     sk 
*********************************************************************21*/

#ifndef __VB_MME_SEC_INTF__
#define __VB_MME_SEC_INTF__ 1

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

#ifdef VB_MME_NAS_SEC

EXTERN S16 vbMmeGetAuthVector ARGS((VbMmeUeCb *ueCb));

EXTERN S16 vbMmeProcessSyncFlr ARGS((VbMmeUeCb  *ueCb,
                                     U8         *auts,
                                     U8         *rnd));
#endif

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */
#endif

