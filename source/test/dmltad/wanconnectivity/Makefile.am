##########################################################################
# If not stated otherwise in this file or this component's LICENSE
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

AM_CPPFLAGS = -Wall -g -Werror
AM_CXXFLAGS = -std=c++11

ACLOCAL_AMFLAGS = -I m4
bin_PROGRAMS = CosaWanCnctvtyChk_gtest.bin
CosaWanCnctvtyChk_gtest_bin_CPPFLAGS = -I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/gtest \
                                  -I${top_srcdir}/gtest/include \
                                  -I${top_srcdir}/source \
                                  -I${top_srcdir}/source/include \
                                  -I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/dbus-1.0 \
                                  -I$(PKG_CONFIG_SYSROOT_DIR)$(libdir)/dbus-1.0/include \
                                  -I$(PKG_CONFIG_SYSROOT_DIR)$(includedir)/ccsp \
                                  -I${PKG_CONFIG_SYSROOT_DIR}$(includedir)/rbus \
                                  -I${PKG_CONFIG_SYSROOT_DIR}$(includedir)/libsafec \
                                  -I${PKG_CONFIG_SYSROOT_DIR}$(includedir)/syscfg \
                                  -I${PKG_CONFIG_SYSROOT_DIR}$(includedir) \
                                  -I$(top_srcdir)/source/diagnostic/include \
                                  -I$(top_srcdir)/source/diagnostic/BbhmDiagIpPing \
                                  -I$(top_srcdir)/source/dmltad \
                                  -I$(top_srcdir)/source/TandDSsp

CosaWanCnctvtyChk_gtest_bin_SOURCES =  CosaWanCnctvtyChkTest.cpp \
                                       gtest_main.cpp

CosaWanCnctvtyChk_gtest_bin_LDADD = $(top_builddir)/source/dmltad/libdmltad.la \
                                    $(top_builddir)/source-arm/dmltad/libdmlarm.la

CosaWanCnctvtyChk_gtest_bin_LDFLAGS = -lccsp_common -lsyscfg -lsysevent -lsecure_wrapper -lrbus \
                                      -lresolv -lev -lpcap -lgtest -lgmock -lgcov