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

     Name:     Trillium  LTE PGW Application
     
     Type:     C source file
  
     Desc:     This file contains utility function required  by PGW 
  
     File:     avsm_utl.c
  
     Sid:      avsm_utl.c@@/main/1 - Tue Aug 17 18:54:37 2010
   
     Prg:       sp
  
*********************************************************************21*/


/************************************************************************
 
/* header include files (.h)            */


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

#ifdef HI
#include "lhi.h"
#include "hit.h"           /* HI layer */
#endif

#include "lav.h"
#include "avsm.h"        /* defines for S1AP acceptance tests */

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

#ifdef HI
#include "lhi.x"
#include "hit.x"           /* HI layer */
#endif

#include "lav.x"
#include "avsm.x"        /* typedefs for S1AP acceptance tests */


/*
 *
 *       Fun:    avSmDefHdr - fill in default management header
 *
 *       Desc:   
 *
 *       Ret:    ROK     - succeeded
 *               RFAILED - failed
 *
 *       Notes:  None
 *
         File:   avsm_utl.c
 *
 */

#ifdef ANSI
PUBLIC Void avSmDefHdr
(
Header *hdr,
Ent    ent,
Elmnt  elem,
Selector sel
)
#else
PUBLIC Void avSmDefHdr(hdr, ent, elem, sel)
Header *hdr;
Ent    ent;
Elmnt  elem;
Selector sel;
#endif /* ANSI */
{
   TRC2(avSmDefHdr)

   hdr->transId             = (TranId)AVSM_TRANSID;
   hdr->entId.ent           = ent;
   hdr->entId.inst          = 0;
   hdr->elmId.elmnt         = elem;
   hdr->response.selector   = (Selector)sel;
   hdr->response.prior      = (Prior)AVSM_MSGPRIOR;
   hdr->response.route      = (Route)RTESPEC;
   hdr->response.mem.region = (Region)avSmCb.init.region;
   hdr->response.mem.pool   = (Pool)avSmCb.init.pool;
   RETVOID;
} /* end of avSmDefHdr() */

  
/********************************************************************30**
  
         End of file:     avsm_utl.c@@/main/1 - Tue Aug 17 18:54:37 2010
   
*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************70**
  
*********************************************************************71*/

/********************************************************************80**
 
*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/1      -     sp                  1.LTE-CNE:PGW Initial Release
*********************************************************************91*/
