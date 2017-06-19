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
 
     Name:     LTE CNE - Management Interface Module
  
     Type:     C source file
  
     Desc:     
  
     File:     vbsm_utl.c     
  
     Sid:      vbsm_utl.c@@/main/1 - Mon Jun 28 19:35:43 2010
  
     Prg:      
  
*********************************************************************21*/

/************************************************************************
 
     Note: 
 
************************************************************************/

#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general layer */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timers defines */
#include "cm_hash.h"       /* common hash list defines */
#include "cm_inet.h"       /* common INET defines */
#include "cm_llist.h"      /* common linked list defines */
#include "cm_mblk.h"       /* memory management */
#include "cm_tkns.h"       /* common tokens */
#include "cm_tpt.h"        /* common transport defines */
#include "cm_dns.h"         /* common DNS libraru defines */

#include "sct.h"           /* SB (SCTP) layer */
#include "lsz.h"           /* layer management defines for S1AP */
#include "lvb.h"            /* typedefs for S1AP */

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
/* #include "hi.h" */
#endif

#ifdef    SB
#include "lsb.h"
#endif    /* SB */
/* vb007.101 s1ap include files order has changed */
/* #include "sz.h"*/            /* defines and macros for S1AP */
#include "szt_asn.h" 
#include "szt.h"            /* defines and macros for S1AP */
/* #include "sz_err.h" */       /* S1AP error defines */
#include "vbsm.h"        /* defines for S1AP acceptance tests */

/* header/extern include files (.x) */

#include "gen.x"           /* general layer typedefs */
#include "ssi.x"           /* system services typedefs */
#include "cm5.x"           /* common timers */
#include "cm_hash.x"       /* common hash list */
#include "cm_inet.x"       /* common INET */
#include "cm_lib.x"        /* common library */
#include "cm_llist.x"      /* common linked list */
#include "cm_mblk.x"       /* memory management */
#include "cm_tkns.x"       /* common tokens */
#include "cm_tpt.x"        /* common transport types */
#include "cm_dns.x"         /* common DNS libraru defines */
#include "szt_asn.x" 
#include "sct.x"           /* SCTP Interface Structures */
#include "szt.x"           /* SCTP Interface Structures */
#include "lsz.x"           /* S1AP types */

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#ifdef    SB
#include "lsb.x"
#endif    /* SB */

#include "lvb.x"            /* typedefs for S1AP */
#include "vbsm.x"        /* typedefs for S1AP acceptance tests */


/*
 *
 *       Fun:    vbSmDefHdr - fill in default management header
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   vbsm_utl.c
 *
 */

#ifdef ANSI
PUBLIC Void vbSmDefHdr
(
Header *hdr,
Ent    ent,
Elmnt  elem,
Selector sel
)
#else
PUBLIC Void vbSmDefHdr(hdr, ent, elem, sel)
Header *hdr;
Ent    ent;
Elmnt  elem;
Selector sel;
#endif /* ANSI */
{
   TRC2(vbSmDefHdr)

   hdr->transId             = (TranId)VBSM_TRANSID;
   hdr->entId.ent           = ent;
   hdr->entId.inst          = 0;
   hdr->elmId.elmnt         = elem;
   hdr->response.selector   = (Selector)sel;
   hdr->response.prior      = (Prior)VBSM_MSGPRIOR;
   hdr->response.route      = (Route)RTESPEC;
   hdr->response.mem.region = (Region)vbSmCb.init.region;
   hdr->response.mem.pool   = (Pool)vbSmCb.init.pool;
   RETVOID;
} /* end of vbSmDefHdr() */

/**********************************************************************

         End of file:     vbsm_utl.c@@/main/1 - Mon Jun 28 19:35:43 2010

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -         -               1. LTE-CNE Initial Release.
/main/1  vb007.101   akaranth          1. s1ap inlcude files oder has changed. 
*********************************************************************91*/
