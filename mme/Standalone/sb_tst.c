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
  
     Name:     Stream Control Transmission Protocol - Test file 
  
     Type:     C source file
  
     Desc:     Sample C source code used to link SCTP in TRILLIUM 
               development environment.

     File:     sb_tst.c
  
     Sid:      sb_tst.c@@/main/3 - Thu Jul 26 07:54:36 2007
  
     Prg:      asa
  
*********************************************************************21*/
  

/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#ifdef DOS                 /* dos environment */
#include <process.h>       /* dos - process */  
#endif

/* header/extern include files (.x), if any */
  
/* local defines, if any */
  
/* local typedefs, if any */
  
/* local externs, if any */
  
/* forward references, if any */

#ifdef __cplusplus
EXTERN "C" {
#endif

EXTERN  int   main  ARGS((int argc, char **argv ));
EXTERN  void  exit  ARGS((int status ));
  
#ifdef __cplusplus
}
#endif

/* functions in other modules, if any */
  
/* public variable declarations, if any */
  
/* private variable declarations, if any */
  

/* functions */
  
/*
*
*       Fun:   Main
*
*       Desc:  Program execution starts here.
*
*       Ret:   None
*
*       Notes: None
*
*       File:  sb_tst.c
*
*/
  
#ifdef ANSI
int main
(
int      argc,                   /* argument count */
char   **argv                 /* argument values */
)
#else
int main(argc, argv)
int      argc;                   /* argument count */
char   **argv;                /* argument values */
#endif
{
   TRC1(main)

   exit(0);

   UNUSED(argc);
   UNUSED(argv);

   RETVALUE(OK);
} /* end of main */

  
/********************************************************************30**
  
         End of file:     sb_tst.c@@/main/3 - Thu Jul 26 07:54:36 2007
  
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

  version    pat  init                   description
----------- ----- ----  ------------------------------------------------

*********************************************************************81*/

/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
/main/2      ---      sb   1. initial release.
 /main/2     ---      sb   1. Modified for SCTP release based on 
                              RFC-2960 'Oct 2000.
/main/3      ---   rsr/ag  1. Updated for Release of 1.3  
*********************************************************************91*/
