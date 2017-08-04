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


/***********************************************************************

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: eg_dbsn.h

    Sid:      eg_dbsn.h@@/main/1 - Fri Feb  4 18:18:09 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_SN_MBMSESNSTOPREQ_CMD_CODE  235       
#define EG_SN_MBMSESNSTOPREQ_NMB_IES   2         
#define EG_SN_MBMSESNSTOPRSP_CMD_CODE  236       
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_SN_MBMSESNSTOPRSP_NMB_IES   3         
#else 
#define EG_SN_MBMSESNSTOPRSP_NMB_IES   2         
#endif 
#define EG_SN_MBMSESNSTRTREQ_CMD_CODE  231       
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_SN_MBMSESNSTRTREQ_NMB_IES   11        
#else 
#define EG_SN_MBMSESNSTRTREQ_NMB_IES   10        
#endif 
#define EG_SN_MBMSESNSTRTRSP_CMD_CODE  232       
#define EG_SN_MBMSESNSTRTRSP_NMB_IES   6         
#define EG_SN_MBMSESNUPDREQ_CMD_CODE   233       
#ifdef EG_REL_AC0 
#define EG_SN_MBMSESNUPDREQ_NMB_IES    9         
#else 
#define EG_SN_MBMSESNUPDREQ_NMB_IES    8 
#endif 
#define EG_SN_MBMSESNUPDRSP_CMD_CODE   234       
#define EG_SN_MBMSESNUPDRSP_NMB_IES    5         


#endif
/********************************************************************30**

         End of file:     eg_dbsn.h@@/main/1 - Fri Feb  4 18:18:09 2011

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
/main/1      ---      svenkat           1. Initial for eGTP 2.1 Release
/main/1     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

