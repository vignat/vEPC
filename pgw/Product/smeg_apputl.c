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

    Name: Source file for filling LM configuration

    Type: C Source

    Desc: 

    File: smeg_apputl.c

    Sid:      smeg_apputl.c@@/main/3 - Fri Feb  4 18:18:24 2011

    Prg : an

************************************************************************/

/* Header Include Files (.h) */
#include "envopt.h"            /* Environment Options */
#include "envdep.h"            /* Environment Dependent */
#include "envind.h"            /* Environment Independent */
#include "gen.h"            /* General */
#include "ssi.h"            /* System Services */
#include "cm_tkns.h"         /* Common Tokens */
#include "cm_hash.h"         /* Common Hash List */
#include "cm_mblk.h"         /* Common memory */
#include "cm_llist.h"           /* Common Link List */
#include "cm5.h"               /* Common Structures */
#include "cm_inet.h"           /* Common IP network */
#include "cm_tpt.h"           /* Common Transport */
#include "hit.h"            /* TUCL layer */
#include "egt.h"            /* EGT INTERFACE  */
#include "leg.h"            /* Stack manager */

/* Header Include Files (.x) */

#include "gen.x"               /* General */
#include "ssi.x"          /* System Services */
#include "cm_tkns.x"          /* Common Tokens */
#include "cm_hash.x"          /* Common Hash List */
#include "cm_mblk.x"          /* Common memory */
#include "cm_llist.x"           /* Common Link List */
#include "cm5.x"           /* Common Structures */
#include "cm_inet.x"           /* Common IP network */
#include "cm_tpt.x"           /* Common Transport */
#include "hit.x"               /* TUCL layer */
#include "egt.x"            /* EGT INTERFACE  */
#include "leg.x"            /* Stack manager */
#include "smeg_apputl.x"            /* Stack manager */

#ifdef __cplusplus
EXTERN "C" {
#endif /* end of __cplusplus */

/************************************************************************
*
*  Func  : egMakeIeEntry
*
*  Desc  : This function populates the IE Properties in IE
           Dictionary.
*
*  Return: No Return value
*
*  File  : smeg_apputl.c
*
*************************************************************************/
#ifdef ANSI
PUBLIC Void egMakeIeEntry
(
LegIeEntry      *ieEntry,
U8             ieCode,
Bool           valid,
U8             dataType,
Ptr            fncPtr,
U16             minLen,
U16             maxLen
)
#else
PUBLIC Void egMakeIeEntry(ieEntry,ieCode,valid, dataType, fncPtr,minLen, maxLen)
LegIeEntry    *ieEntry;
U8             ieCode;
Bool           valid;
U8             dataType;
Ptr            fncPtr;
U16             minLen;
U16             maxLen;
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakeIeEntry)

   /* Populate the IE Entry */
   ieEntry->ieType  = ieCode;
   ieEntry->dataType = dataType;
   ieEntry->minLen = minLen;
   ieEntry->maxLen = maxLen;

} /* End of egMakeIeEntry */

/************************************************************************
*
*  Func  : egMakeAppChildIeEntry
*
*  Desc  : This function populates the Application IEs Properties
*          of a grouped IE
*
*  Return: No Return value
*
*  File  : smeg_apputl.c
*
*************************************************************************/

#ifdef ANSI
PUBLIC Void egMakeAppChildIeEntry
(
LegGroupedIe *grpIe,
U8             count,
U8            ieCode,
U8            ieClass,
U8            ieCr,
U8            ieInst,
U8             nmbGrpIe
)
#else
PUBLIC Void egMakeAppChildIeEntry(grpIe,count,ieCode,ieClass,
                                 ieCr ,ieInst, nmbGrpIe)
LegGroupedIe *grpIe;
U8             count;
U8            ieCode;
U8            ieClass;
U8            ieCr;
U8            ieInst;
U8             nmbGrpIe;
#endif
{
   Mem    memInfo;
   U16   ret;

   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakeAppChildIeEntry)

   memInfo.region =      DFLT_REGION;
   memInfo.pool   =      DFLT_POOL;
   ret = 0;

   /* If it is a grouped IE the value of nmbGrpIe is greater than 0 */
   if(nmbGrpIe > 0)
   {
      grpIe->nmbGrpIe   =   nmbGrpIe;
      ret = SGetSBuf(memInfo.region, memInfo.pool,(Data **)&(grpIe->ieEntry) ,(sizeof(LegIeEntry) *(nmbGrpIe)));
      if( ret != ROK)
      {
         /* Unable to allocate memory for the Grouped IEs  */
      }
      cmMemset( (U8* )grpIe->ieEntry, 0, (sizeof(LegIeEntry) *(nmbGrpIe)));
   } /* End of if statement */

   /* Populate the IE Entry */
   if( count < grpIe->nmbGrpIe )
   {
      grpIe->ieEntry[count].ieType  = ieCode;
      grpIe->ieEntry[count].ieClass  = ieClass;
      grpIe->ieEntry[count].cr  = ieCr;
      grpIe->ieEntry[count].ieInst  = ieInst;

   }
   else
   {
      /* Trying to access array out of bounds */
   }

} /* End of egMakeAppChildIeEntry */



/**********************************************************************************
*
*     Func  : egMakeAppIeProp
*
*     Desc  : This function populates the IE Properties within the eGTP message
*
*     Return: None
*
*     File  : smeg_apputl.c
*
**************************************************************************/


#ifdef ANSI
PUBLIC Void egMakeAppIeProp
(
LegIeProperties *ieProp,     /* Ie Property to be filled */
U8             ieCode,      /* Ie Code */
U8             ieClass,     /* Mandatory/Conditional/Optional */
U8             instance,     /* Instance  */
U8                  cr     /* Comprehension Required */
)
#else
PUBLIC Void egMakeAppIeProp(ieProp, ieCode, ieClass , instance, cr)
LegIeProperties *ieProp;
U8             ieCode;         /*  IE Code  */
U8             ieClass;        /* Mandatory/Conditional/Optional */
U8             instance;       /* Instance  */
U8           cr;             /* Comprehension Required */
#endif
{
   /*-- eg003.201 : TRC changes from eg006.102 --*/
   EG_TRC2(egMakeAppIeProp)

   /* Populate the IE Properties */
   ieProp->ieType   =  ieCode;
   ieProp->ieClass  =  ieClass;
   ieProp->cr       =  cr;
   ieProp->ieInst =  instance;

} /* End of aqMakeAppIeProp */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* end of __cplusplus */


/**********************************************************************
         End of file:     smeg_apputl.c@@/main/3 - Fri Feb  4 18:18:24 2011

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
/main/2      ---      1. Initial for egtp release 1.2
/main/3      ---      pmacharla         Initial for eGTP release 2.1
/main/3     eg003.201 psingh             1. Fixed TRACE5 macro related issue from eg006.102
*********************************************************************91*/

