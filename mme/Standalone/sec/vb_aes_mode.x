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

     Name:    Trillium - AES Mode Module

     Type:    C source file

     Desc:    This file contains AES mode module c structure

     File:    vb_aes_mode.x

     Sid:     vb_aes_mode.x@@/main/1 - Mon Jun 01 19:35:38 2012

     Prg:     sk 
*********************************************************************21*/

#ifndef __VB_AES_MODE_X__
#define __VB_AES_MODE_X__ 1


PUBLIC S16 vbAESCTR ARGS(( U8    *key,
                          U16   bitKeyLen,
                          U8    *counterBlk,
                          U16   bitCounterBlkLen,
                          U8    *data,
                          U32   bitDataLen 
                       ));

PUBLIC U16 vbAESCMAC ARGS(( U8    *key,
                            U16   bitKeyLen, /* Bits Len */
                            U8    *data,
                            U32   bitDataLen, /* Bits Len */
                            U8    *mac,
                            U8    byteMacLen
                         ));

#endif
