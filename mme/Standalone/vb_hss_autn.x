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
 
    Name:   HSS
 
    Type:   C source file
 
    Desc:   Data Structures required by the HSS authentication.
 
    File:   vb_hss_autn.x
 
    Sid:      vb_hss_autn.x@@/main/1 - Mon Jun 28 19:35:37 2010
 
    Prg:    mm
 
*********************************************************************21*/
 
#ifndef __HSSAUTNX__
#define __HSSAUTNX__

/* VB_MME_AUTH Authentication feature added
 * New File for Authentication implementation */
   /*Generate randon number 
   As part of Authentication Procedure HSS will generate the following fields:
	NAS Key Set Identifier: to identify pair of Ciphering and Integrity Keys (Type V)
	It is coded using 4 bits, and value range: 0 to 6. choose any number within this range.
	RAND: Random number, chosen randomly using SRandom() function in SSI. 16 byte random value (Type V).
	AUTN: Authentication Token, it consists of:
	(SQN xor AK) || AMF || MAC  ( || means concatenation)
	SQN = SEQHE || IND, 
	SEQHE is the sequence no maintained in HSS per UE (starts from 0 to 243 - 1, start with zero)
	IND is an index starts from 0 to 31 ( 5 bit value), choose any value within this range
	AMF: Authentication Management field
	It is 2 byte data elements and value range from 128 to 33,022. choose 32,783
	Use the Key (K) for the test USIM {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F}
*/

EXTERN S16 vbHssGenXorAuthVectors
(
U8                *imsi,
VbHssUeSecInfo    *vector
);

EXTERN S16 vbHssResync(U8 *imsi, U8 *auts, VbHssUeSecInfo *vector);
EXTERN S16 vbHssUpdSeqNum(U8 *imsi, VbHssUeSecInfo *vector);
/*
 * implement in MME
EXTERN S16 vbHssAuthVerify(VbHssUeAuth *ueAuth);
EXTERN S16 vbHssAuthFailure(VbHssUeAuth *ueAuth);
*/
#endif /*  __HSSAUTNX__ */


/********************************************************************30**
         End of file:     vb_hss_autn.x@@/main/1 - Mon Jun 28 19:35:37 2010
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
/main/1      ---     mm          1. Initial version for LTE-CNE Phase 1.
*********************************************************************91*/
