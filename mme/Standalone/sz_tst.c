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
  
     Name:     s1ap - test
  
     Type:     C source file
  
     Desc:     Sample C source code used to link s1ap in a
               TRILLIUM development environment.

     File:     sq_tst.c
  
     Sid:      sq_tst.c@@/main/4 - Tue May  8 16:04:55 2012
  
     Prg:      sn
  
*********************************************************************21*/
  
  
/* header include files (.h) */

#include "envopt.h"        /* environment options */  
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#ifdef DOS                 /* dos environment */
#include <process.h>       /* dos - process */  
#endif /* DOS */

/* header/extern include files (.x), if any */
  
/* local defines, if any */
  
/* local typedefs, if any */
  
/* local externs, if any */
  
/* forward references, if any */

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

EXTERN  int main ARGS((int argc,char **argv ));
EXTERN  void exit ARGS((int status ));
  
#ifdef __cplusplus
}
#endif /* __cplusplus */

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
 *       File:  ra_tst.c
 *
 */
  
#ifdef ANSI
int main
(
int argc,                   /* argument count */
char **argv                 /* argument values */
)
#else /* ANSI */
int main(argc, argv)
int argc;                   /* argument count */
char **argv;                /* argument values */
#endif  /* ANSI */
{
   TRC1(main)

   UNUSED(argc);
   UNUSED(argv);

   exit(0);

   return(OK);
} /* end of main */

/********************************************************************30**
  
         End of file:     sq_tst.c@@/main/4 - Tue May  8 16:04:55 2012
  
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
/main/4      ---      va    1. Updated for S1AP release 4.1
*********************************************************************91*/
