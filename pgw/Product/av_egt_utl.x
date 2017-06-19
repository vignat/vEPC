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
 
     Name:     Trillium LTE CNE - PDN-GW 
  
     Type:     C include file
  
     Desc:     PGW EGT Utility functions
  
     File:     av_egt_utl.x 
  
*********************************************************************21*/
#ifndef __AV_EGT_UTL_X__
#define __AV_EGT_UTL_X__

PUBLIC S16 avPgwValMBReq ARGS((
AvPgwPdnCb        *pdnCb,
EgMsg             *egMsg,
U32               *cause
));

PUBLIC S16 avPgwEguTunnelModReq ARGS((
AvPgwUeTunnelCb         *pUTun,
EgMsg                   *pEgMsg
));

PUBLIC S16 avPgwBldMBRsp ARGS((
AvPgwPdnCb        *pdnCb, 
EgMsg             **egMsg
));

#endif /* __AV_EGT_UTL_X__ */
