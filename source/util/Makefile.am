##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

AM_CFLAGS =

if ENABLE_MTA
AM_CFLAGS  += -DENABLE_MTA
endif

ACLOCAL_AMFLAGS = -I m4

AM_LDFLAGS =  -lpthread -lz

bin_PROGRAMS = Sub64
Sub64_CPPFLAGS = -I$(top_srcdir)/source -I$(top_srcdir)/source/util
Sub64_SOURCES = sub64.c

bin_PROGRAMS += Selfhealutil
AM_CFLAGS += $(AM_CPPFLAGS)
if ENABLE_MTA
AM_LDFLAGS += -lhal_mta
endif
Selfhealutil_CPPFLAGS =
Selfhealutil_SOURCES = selfheal_utility.c

bin_PROGRAMS = RxTx100
if CORE_NET_LIB_FEATURE_SUPPORT
AM_LDFLAGS += -lnet
endif
AM_LDFLAGS += -lsyscfg -lsecure_wrapper -ltelemetry_msgsender
RxTx100_CPPFLAGS = -I$(top_srcdir)/source -I$(top_srcdir)/source/util
RxTx100_SOURCES = rxtx_100.c
