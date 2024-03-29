/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2020 RDK Management
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

#ifndef MOCK_WANCNCTVTYCHK_H
#define MOCK_WANCNCTVTYCHK_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define  ANSC_STATUS_SUCCESS    0
#define  ANSC_STATUS_FAILURE    0xFFFFFFFF
typedef  unsigned long          ULONG;
typedef  ULONG                  ANSC_STATUS;

class WanCnctvtyChkInterface {
public:
        virtual ~WanCnctvtyChkInterface() {}
        virtual ANSC_STATUS WanCnctvtyChk_GetParameterValue(const char *, char *) = 0;
};

class WanCnctvtyChkMock: public WanCnctvtyChkInterface {
public:
        virtual ~WanCnctvtyChkMock() {}
        MOCK_METHOD2(WanCnctvtyChk_GetParameterValue, ANSC_STATUS(const char *, char *));
};

#endif
