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

    File: eg_dbs3.h

    Sid:      eg_dbs3.h@@/main/2 - Fri Feb  4 18:18:05 2011

    Prg: an

************************************************************************/

#ifdef EGTP_C

/* Message Codes   */

#define EG_S3_ALRTMMEACK_CMD_CODE      154       
#define EG_S3_ALRTMMEACK_NMB_IES       2         
#define EG_S3_ALRTMMENOTFN_CMD_CODE    153       
#define EG_S3_ALRTMMENOTFN_NMB_IES     1         
#define EG_S3_CSPAGEIND_CMD_CODE       151       

#if defined(EG_REL_AC0) || defined(EG_REL_930)
#define EG_S3_CSPAGEIND_NMB_IES        9         
#else
#define EG_S3_CSPAGEIND_NMB_IES        8
#endif /* defined(EG_REL_AC0) || defined(EG_REL_930) */

#define EG_S3_CTXACK_CMD_CODE          132       
#define EG_S3_CTXACK_NMB_IES           3         
#define EG_S3_CTXREQ_CMD_CODE          130       

#ifdef EG_REL_AC0
#define EG_S3_CTXREQ_NMB_IES           15        
#else /* EG_REL_AC0 */
#define EG_S3_CTXREQ_NMB_IES           12
#endif /* EG_REL_AC0 */

#define EG_S3_CTXRSP_CMD_CODE          131       

#ifdef EG_REL_AC0
#define EG_S3_CTXRSP_NMB_IES           24        
#else
#define EG_S3_CTXRSP_NMB_IES           19
#endif /* EG_REL_AC0 */

#ifdef EG_REL_AC0
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 16
#elif EG_REL_930
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 13
#else /* EG_REL_930 */
#define EG_CTXRSP_PDN_CONN0_GRP_NMB_IES 12
#endif /* EG_REL_AC0 */

#define EG_CTXRSP_BEARER_CNTX00_GRP_NMB_IES 7         
#define EG_S3_DETACK_CMD_CODE          150       
#define EG_S3_DETACK_NMB_IES           3         
#define EG_S3_DETNTF_CMD_CODE          149       

#ifdef EG_REL_AC0
#define EG_S3_DETNTF_NMB_IES           3         
#else
#define EG_S3_DETNTF_NMB_IES           2
#endif /* EG_REL_AC0 */

#define EG_S3_FWDRELCMPACK_CMD_CODE    136       
#define EG_S3_FWDRELCMPACK_NMB_IES     3         
#define EG_S3_FWDRELCMPNTF_CMD_CODE    135       
#define EG_S3_FWDRELCMPNTF_NMB_IES     2         
#define EG_S3_FWDRELREQ_CMD_CODE       133       

#ifdef EG_REL_AC0
#define EG_S3_FWDRELREQ_NMB_IES        39        
#elif EG_REL_930
#define EG_S3_FWDRELREQ_NMB_IES        29
#else /* EG_REL_930 */
#define EG_S3_FWDRELREQ_NMB_IES        27
#endif /* EG_REL_AC0 */

#ifdef EG_REL_AC0
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 16        
#elif EG_REL_930
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 13
#else /* EG_REL_930 */
#define EG_FWDRELREQ_PDN_CONN0_GRP_NMB_IES 12
#endif /* EG_REL_AC0 */

#define EG_FWDRELREQ_BEARER_CNTX00_GRP_NMB_IES 8         
#define EG_S3_FWDRELRSP_CMD_CODE       134       

/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
#define EG_S3_FWDRELRSP_NMB_IES        15        
#else
#define EG_S3_FWDRELRSP_NMB_IES        12
#endif

#define EG_FWDRELRSP_BEARER_CNTX1_GRP_NMB_IES 8         
#define EG_FWDRELRSP_BEARER_CNTX2_GRP_NMB_IES 8         
#define EG_S3_IDENTREQ_CMD_CODE        128       
#define EG_S3_IDENTREQ_NMB_IES         8         
#define EG_S3_IDENTRSP_CMD_CODE        129       
#define EG_S3_IDENTRSP_NMB_IES         10        
#define EG_S3_RANINFORLY_CMD_CODE      152       
#define EG_S3_RANINFORLY_NMB_IES       3         
#define EG_S3_RELCANCELREQ_CMD_CODE    139       

/* eg014.201: ccpu00139510 Egtpc release 10 upgrade */
#ifdef EG_REL_AC0
#define EG_S3_RELCANCELREQ_NMB_IES     5         
#else
#define EG_S3_RELCANCELREQ_NMB_IES     3
#endif /* EG_REL_AC0 */

#define EG_S3_RELCANCELRSP_CMD_CODE    140       
#define EG_S3_RELCANCELRSP_NMB_IES     2         
#define EG_S3_SUSPNDACK_CMD_CODE       163       
#define EG_S3_SUSPNDACK_NMB_IES        2         
#define EG_S3_SUSPNDNOTFN_CMD_CODE     162       
#define EG_S3_SUSPNDNOTFN_NMB_IES      3         
#define EG_S3_UEACTACK_CMD_CODE        156       
#define EG_S3_UEACTACK_NMB_IES         2         
#define EG_S3_UEACTNOTFN_CMD_CODE      155       
#define EG_S3_UEACTNOTFN_NMB_IES       1         


#endif
/********************************************************************30**

         End of file:     eg_dbs3.h@@/main/2 - Fri Feb  4 18:18:05 2011

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

