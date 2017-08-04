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

     Name:    EGTP - 

     Type:    C source file

     Desc:    

     File:    eg_tunn.h

     Sid:      eg_tunn.h@@/main/3 - Fri Feb  4 18:18:13 2011

     Prg:    mm  

*********************************************************************21*/

#ifndef __EG_TUNN_H__
#define __EG_TUNN_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 *                             MACROS                                         *
 ******************************************************************************/

#define ZERO 0

/******************************************************************************
* Macro to send an alarm in case of hash failure *
 ******************************************************************************/
#define SEND_HASH_FAIL_ALARM()\
   {\
      EgUsta     sta; \
      cmMemset( (U8* )&sta, 0, sizeof(EgUsta));     \
      sta.dgn.dgnVal[0].type    = LEG_PAR_GEN; \
      egSendLmAlarm(LCM_CATEGORY_INTERNAL, LCM_EVENT_LYR_SPECIFIC,\
            LCM_CAUSE_HASH_FAIL, &sta.dgn);\
   }

/******************************************************************************
* macro to get the pointer to the tunnel Control block given the event *
 ******************************************************************************/

#define GET_TUNN_FRM_EVENT(event, teCCb)\
   {\
      if((event->lclTeid.pres != PRSNT_NODEF) || (event->lclTeid.val == 0))\
      {\
         *teCCb = NULLP;\
         EG_DBG_ERR(EG_DBG_TUNN, 0, (egp, \
                  "Message received on a non-existent tunnel.")); \
         event->resultType = EGT_ERR_UNKNOWN_TEID_MSG; \
         egSendErrInd(event, usap); \
         EG_RETVALUE( RFAILED); \
      }\
      else\
      {\
         EgTECCb **teCCbIdx; \
         teCCbIdx = NULLP; \
         teCCbIdx = egGetTunnelCb(event->lclTeid.val);\
         /* eg012.201:klockwork fix */\
         if ((teCCbIdx != NULLP) && (*teCCbIdx != NULLP))\
         {\
            *teCCb = *teCCbIdx;\
         }\
         else\
         {\
            EG_RETVALUE(RFAILED);\
         }\
      }\
}


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __EG_TUNN_H__ */


  
/********************************************************************30**

         End of file:     eg_tunn.h@@/main/3 - Fri Feb  4 18:18:13 2011

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

*********************************************************************81*/

/********************************************************************90**

    ver       pat    init                  description
----------- -------- ---- -----------------------------------------------
/main/1      ---      mm                1. Created for Initial Release 1.1
/main/2      ---      rsharon           1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
/main/3   eg012.201   shpandey         1. Added klockwork fix
*********************************************************************91*/
