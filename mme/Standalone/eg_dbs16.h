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


/***********************************************************************

    Name: Header file for defines for LM configuration

    Type: C Source

    Desc: 

    File: eg_dbs16.h

    Sid:      eg_dbs16.h@@/main/2 - Fri Feb  4 18:18:04 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_S16_CTXACK_CMD_CODE         132       
#define EG_S16_CTXACK_NMB_IES          3         
#define EG_S16_CTXREQ_CMD_CODE         130       
#ifdef EG_REL_AC0 
#define EG_S16_CTXREQ_NMB_IES          13        
#else 
#define EG_S16_CTXREQ_NMB_IES          12        
#endif 
#define EG_S16_CTXRSP_CMD_CODE         131       
#ifdef EG_REL_AC0
#define EG_S16_CTXRSP_NMB_IES          22        
#else
#define EG_S16_CTXRSP_NMB_IES          19        
#endif
#ifdef EG_REL_AC0 
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 15        
#elif EG_REL_930 
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 12        
#else
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 11
#endif 
#define EG_CTXRSP_BEARER_CNTX00_GRP_NMB_IES 7         
#define EG_S16_FWDACCSCTXACK_CMD_CODE  138       
#define EG_S16_FWDACCSCTXACK_NMB_IES   2         
#define EG_S16_FWDACCSCTXNTF_CMD_CODE  137       
#define EG_S16_FWDACCSCTXNTF_NMB_IES   4         
#define EG_S16_FWDRELCMPACK_CMD_CODE   136       
#define EG_S16_FWDRELCMPACK_NMB_IES    3         
#define EG_S16_FWDRELCMPNTF_CMD_CODE   135       
#define EG_S16_FWDRELCMPNTF_NMB_IES    2         
#define EG_S16_FWDRELREQ_CMD_CODE      133       
#ifdef EG_REL_AC0
#define EG_S16_FWDRELREQ_NMB_IES       38        
#elif EG_REL_930
#define EG_S16_FWDRELREQ_NMB_IES       30        
#else
#define EG_S16_FWDRELREQ_NMB_IES       28
#endif
#ifdef EG_REL_AC0
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 16        
#elif EG_REL_930
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 13        
#else
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 12        
#endif
#define EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES 8         
#define EG_S16_FWDRELRSP_CMD_CODE      134       
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
#define EG_S16_FWDRELRSP_NMB_IES       12        
#else
#define EG_S16_FWDRELRSP_NMB_IES       11        
#endif
#define EG_FWDRELRSP_BEARER_CNTX1_GRP_NMB_IES 8         
#define EG_FWDRELRSP_BEARER_CNTX2_GRP_NMB_IES 8         
#define EG_S16_IDENTREQ_CMD_CODE       128       
#define EG_S16_IDENTREQ_NMB_IES        8         
#define EG_S16_IDENTRSP_CMD_CODE       129       
#define EG_S16_IDENTRSP_NMB_IES        10        
#define EG_S16_RANINFORLY_CMD_CODE     152       
#define EG_S16_RANINFORLY_NMB_IES      3         
#define EG_S16_RELCANCELREQ_CMD_CODE   139       
/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_S16_RELCANCELREQ_NMB_IES    5         
#else 
#define EG_S16_RELCANCELREQ_NMB_IES    3         
#endif 
#define EG_S16_RELCANCELRSP_CMD_CODE   140       
#define EG_S16_RELCANCELRSP_NMB_IES    2         
#define EG_S16_SUSPNDACK_CMD_CODE      163       
#define EG_S16_SUSPNDACK_NMB_IES       2         
#define EG_S16_SUSPNDNOTFN_CMD_CODE    162       

/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0 
#define EG_S16_SUSPNDNOTFN_NMB_IES  3
#elif EG_REL_930 
#define EG_S16_SUSPNDNOTFN_NMB_IES 3
#else
#define EG_S16_SUSPNDNOTFN_NMB_IES 1
#endif 

#endif

/********************************************************************30**

         End of file:     eg_dbs16.h@@/main/2 - Fri Feb  4 18:18:04 2011

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
/main/1      ---      an                1. Initial Release
/main/1      ---      kchaitanya        1. Initial for eGTP 1.2 Release
/main/2      ---      svenkat           1. Initial for eGTP 2.1 Release
/main/2     eg014.201    rss            1. Egtpc release 10 upgrade
*********************************************************************91*/

