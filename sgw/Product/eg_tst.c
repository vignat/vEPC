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
  
     Name:     eGTP - test
  
     Type:     C source file
  
     Desc:     Sample C source code used to link eGTP in a
               TRILLIUM development environment.

     File:     eg_tst.c
  
     Sid:      eg_tst.c@@/main/3 - Fri Feb  4 18:18:12 2011
  
     Prg:      kc
  
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
extern "C" {
#endif

extern  int main ARGS((int argc,char **argv ));
extern  void exit ARGS((int status ));

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
*       File:  eg_tst.c
*
*/
  
#ifdef ANSI
int main
(
int argc,                   /* argument count */
char **argv                 /* argument values */
)
#else
int main(argc, argv)
int argc;                   /* argument count */
char **argv;                /* argument values */
#endif
{
   TRC1(main)

   UNUSED(argc);
   UNUSED(argv);
   exit(0);
   RETVALUE(OK);
} /* end of main */


/********************************************************************30**

         End of file:     eg_tst.c@@/main/3 - Fri Feb  4 18:18:12 2011

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
/main/1      ---      ad                1. Created for Initial Release 1.1
/main/2      ---      nkottapalli       1. Initial for eGTP 1.2 Release
/main/3      ---      pmacharla       Initial Release of eGTP 2.1 release
*********************************************************************91*/
