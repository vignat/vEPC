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




/**********************************************************************

     Name:     S1AP Layer

     Type:     C source file

     Desc:     C source code for DB

     File:     sq_db.c

     Sid:      sq_db.c@@/main/nodal_main/3 - Fri Nov 22 17:39:46 2013

     Prg:      hsingh

**********************************************************************/

#include "envopt.h"           /* environment options */
#include "envdep.h"           /* environment dependent */
#include "envind.h"           /* environment independent */
#include "gen.h"              /* general layer */
#include "ssi.h"              /* system services */
#include "cm_mblk.h"
#include "cm_tkns.h"
#include "cm_hash.h"
#include "cm_pasn.h"          /* common defines for ASN */
#include "cm5.h"

#include "cm_tpt.h"
#include "sct.h"
#include "cm_inet.h"
#include "szt.h"
#ifdef SZ_FTHA
#include "cm_ftha.h"
#include "cm_psfft.h"
#include "cm_pftha.h"
#include "cm_psf.h"
#include "sht.h"           /* System Agent */
#endif /* SZ_FTHA */
#ifndef SZ_ENC_DEC
#include "lsz.h"
#endif /* SZ_ENC_DEC */
#include "sz.h"

/* header/extern include files (.x) */
#include "cm_llist.h"         /* */
#include "cm_hash.h"
#include "cm_umts.h"
#include "gen.x"              /* general layer */
#include "ssi.x"              /* system services */
#include "cm5.x"              /* */
#include "cm_tkns.x"          /* */
#include "cm_mblk.x"
#include "cm_llist.x"
#include "cm_hash.x"
#include "cm_umts.x"
#include "cm_pasn.x"          /* PASN */
#include "szt_asn.h"
#include "szt_asn.x"

#include "cm_inet.x"
#include "cm_tpt.x"
#include "sct.x"
#ifdef SZTV3
/* sz008.301 support for source RNC to target RNC */
#include "szt_3gasn.x"
#endif
#include "szt.x"
#ifdef SZ_FTHA
#include "cm_ftha.x"
#include "cm_psfft.x"
#include "cm_pftha.x"
#include "cm_psf.x"
#include "sht.x"           /* System Agent */
#endif /* SZ_FTHA */
#ifndef SZ_ENC_DEC
#include "lsz.x"
#include "sz.x"
#else  /* SZ_ENC_DEC */
#endif /* SZ_ENC_DEC */

 
#ifdef S1AP_REL8A0     /* S1ap release 8.10.0 */

#include "sz_dbr8.c"

#elif S1AP_REL9A6      /* S1ap release 9.6.0 */

#include "sz_dbr9.c"

#else                  /* S1ap release 10.5.0 */

#include "sz_dbr10.c"
 
#endif                 /* S1ap release specification flag */


PUBLIC CmPAsnElmDef *szMsgDbIe[][2] =
{
   {
      NULLP
   },
   {
      &szSrceNB_ToTgeteNB_TprntCont,
      NULLP
   },
   {
      &szTgeteNB_ToSrceNB_TprntCont,
      NULLP
   }
};



/**********************************************************************

         End of file:     sq_db.c@@/main/nodal_main/3 - Fri Nov 22 17:39:46 2013

**********************************************************************/

/**********************************************************************
        Revision History:

**********************************************************************/
/********************************************************************90**

     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/5      ---        va    1. Updated for S1AP release 4.1
*********************************************************************91*/
