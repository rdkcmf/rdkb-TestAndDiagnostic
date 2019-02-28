/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/


/**********************************************************************

    module: bbhm_download_exported_api.h

        For Broadband Home Manager Model Implementation (BBHM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the Bbhm Download Diagnostics
        Object.

       Bbhm Diagnostics are defined at TR143

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

       Jinghua Xu

    ---------------------------------------------------------------

    revision:

        06/01/2011    initial revision.

**********************************************************************/


#ifndef  _BBHM_DOWNLOAD_EXPORTED_API_
#define  _BBHM_DOWNLOAD_EXPORTED_API_



/***********************************************************
       FUNCTIONS IMPLEMENTED IN BBHM_DOWNLOAD_INTERFACE.C
***********************************************************/

ANSC_HANDLE
BbhmCreateDownloadDiag
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


/***********************************************************
          FUNCTIONS IMPLEMENTED IN BBHM_DOWNLOAD_BASE.C
***********************************************************/

ANSC_HANDLE
BbhmDownloadCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BbhmDownloadRemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BbhmDownloadEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BbhmDownloadInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
