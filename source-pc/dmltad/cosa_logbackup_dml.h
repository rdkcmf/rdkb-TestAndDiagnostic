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

#ifndef  _COSA_LOGBACKUP_DML_H
#define  _COSA_LOGBACKUP_DML_H

/***********************************************************************

/***********************************************************************

 APIs for Object:

    LogBackup.

    *  LogBackup_GetParamBoolValue
    *  LogBackup_SetParamBoolValue
    *  LogBackup_GetParamUlongValue
    *  LogBackup_SetParamUlongValue
    *  LogBackup_Validate
    *  LogBackup_Commit
    *  LogBackup_Rollback

***********************************************************************/

BOOL LogBackup_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                        bValue
    );

BOOL LogBackup_SetParamBoolValue  
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    );

BOOL
LogBackup_GetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG*                      puLong
    );

BOOL
LogBackup_SetParamUlongValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        ULONG                       uValue
    );

BOOL
LogBackup_Validate
   (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    );

ULONG
LogBackup_Commit
    (
        ANSC_HANDLE                 hInsContext
    );

ULONG
LogBackup_Rollback
    (
        ANSC_HANDLE                 hInsContext
    );

get_logbackupcfg();



#endif
