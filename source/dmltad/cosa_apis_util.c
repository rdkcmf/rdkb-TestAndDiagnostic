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

    module:	cosa_apis_util.h

        This is base file for all parameters H files.

    ---------------------------------------------------------------

    description:

        This file contains all utility functions for COSA DML API development.

    ---------------------------------------------------------------

    environment:

        COSA independent

    ---------------------------------------------------------------

    author:

        Roger Hu

    ---------------------------------------------------------------

    revision:

        01/30/2011    initial revision.

**********************************************************************/



#include "cosa_apis.h"
#include "plugin_main_apis.h"
#include "safec_lib_common.h"

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>

#include "ansc_platform.h"


ANSC_STATUS
CosaUtilStringToHex
    (
        char          *str,
        unsigned char *hex_str
    )
{
    INT   i = 0, index = 0, val = 0;
    CHAR  byte[3]       = {'\0'};

    while(str[i] != '\0')
    {
        byte[0] = str[i];
        byte[1] = str[i+1];
        byte[2] = '\0';
        if(_ansc_sscanf(byte, "%x", &val) != 1)
            break;
	hex_str[index] = val;
        i += 2;
        index++;
    }
    if(index != 8)
        return ANSC_STATUS_FAILURE;

    return ANSC_STATUS_SUCCESS;
}

ULONG
CosaUtilGetIfAddr
    (
        char*       netdev
    )
{
    ANSC_IPV4_ADDRESS       ip4_addr;
    errno_t rc = -1;
    rc = memset_s(&ip4_addr, sizeof(ANSC_IPV4_ADDRESS), 0, sizeof(ANSC_IPV4_ADDRESS));
    ERR_CHK(rc);


    struct ifreq            ifr;
    int                     fd = 0;

    rc = strcpy_s(ifr.ifr_name, sizeof(ifr.ifr_name) , netdev);
    ERR_CHK(rc);

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0)
    {
        if (!ioctl(fd, SIOCGIFADDR, &ifr))
        {
           rc = memcpy_s(&ip4_addr.Value, sizeof(ip4_addr.Value) , ifr.ifr_ifru.ifru_addr.sa_data + 2,4);
           ERR_CHK(rc);
        }
        else
           perror("CosaUtilGetIfAddr IOCTL failure.");

        close(fd);
    }
    else
        perror("CosaUtilGetIfAddr failed to open socket.");


    return ip4_addr.Value;

}

ANSC_STATUS
CosaSListPushEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        PCOSA_CONTEXT_LINK_OBJECT   pCosaContext
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;

    if ( pListHead->Depth == 0 )
    {
        AnscSListPushEntryAtBack(pListHead, &pCosaContext->Linkage);
    }
    else
    {
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
        {
            pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

            if ( pCosaContext->InstanceNumber < pCosaContextEntry->InstanceNumber )
            {
                AnscSListPushEntryByIndex(pListHead, &pCosaContext->Linkage, ulIndex);

                return ANSC_STATUS_SUCCESS;
            }
        }

        AnscSListPushEntryAtBack(pListHead, &pCosaContext->Linkage);
    }

    return ANSC_STATUS_SUCCESS;
}

PCOSA_CONTEXT_LINK_OBJECT
CosaSListGetEntryByInsNum
    (
        PSLIST_HEADER               pListHead,
        ULONG                       InstanceNumber
    )
{
    PCOSA_CONTEXT_LINK_OBJECT       pCosaContextEntry = (PCOSA_CONTEXT_LINK_OBJECT)NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry       = (PSINGLE_LINK_ENTRY       )NULL;
    ULONG                           ulIndex           = 0;

    if ( pListHead->Depth == 0 )
    {
        return NULL;
    }
    else
    {
        pSLinkEntry = AnscSListGetFirstEntry(pListHead);

        for ( ulIndex = 0; ulIndex < pListHead->Depth; ulIndex++ )
        {
            pCosaContextEntry = ACCESS_COSA_CONTEXT_LINK_OBJECT(pSLinkEntry);
            pSLinkEntry       = AnscSListGetNextEntry(pSLinkEntry);

            if ( pCosaContextEntry->InstanceNumber == InstanceNumber )
            {
                return pCosaContextEntry;
            }
        }
    }

    return NULL;
}

PUCHAR
CosaUtilGetLowerLayers
    (
        PUCHAR                      pTableName,
        PUCHAR                      pKeyword
    )
{

    ULONG                           ulNumOfEntries              = 0;
    ULONG                           i                           = 0;
    ULONG                           j                           = 0;
    ULONG                           ulEntryNameLen              = 256;
    char                            ucEntryParamName[256+12]    = {0};
    char                            ucEntryNameValue[256]       = {0};
    char                            ucEntryFullPath[256]        = {0};
    char                            ucLowerEntryPath[256+21]    = {0};
    UCHAR                           ucLowerEntryName[256+7]     = {0};
    ULONG                           ulEntryInstanceNum          = 0;
    ULONG                           ulEntryPortNum              = 0;
    PUCHAR                          pMatchedLowerLayer          = NULL;
    PANSC_TOKEN_CHAIN               pTableListTokenChain        = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pTableStringToken           = (PANSC_STRING_TOKEN)NULL;
    errno_t                         rc                          = -1;

    if ( !pTableName || AnscSizeOfString(pTableName) == 0 ||
         !pKeyword   || AnscSizeOfString(pKeyword) == 0
       )
    {
        return NULL;
    }

    pTableListTokenChain = AnscTcAllocate(pTableName, ",");

    if ( !pTableListTokenChain )
    {
        return NULL;
    }

    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)))
    {
        if ( pTableStringToken->Name )
        {
            if ( AnscEqualString(pTableStringToken->Name, "Device.Ethernet.Interface.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.Ethernet.InterfaceNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Ethernet.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath) , "Device.Ethernet.Interface.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }


                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) , "%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
               
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.IP.Interface.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.IP.InterfaceNumberOfEntries");
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.IP.Interface.", i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath) , "Device.IP.Interface.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) , "%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }

                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.USB.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.HPNA.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.DSL.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.WiFi.Radio.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.WiFi.RadioNumberOfEntries");

                for (i = 0; i < ulNumOfEntries; i++)
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.WiFi.Radio.", i);
                    
                    if (ulEntryInstanceNum)
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath) , "Device.WiFi.Radio.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                        
                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) , "%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                        
                        if (( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                            AnscEqualString(ucEntryNameValue, pKeyword, TRUE) )
                        {
                            pMatchedLowerLayer = AnscCloneString(ucEntryFullPath);
                            
                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.HomePlug.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.MoCA.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.UPA.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.ATM.Link.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.PTM.Link.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.Ethernet.Link.", TRUE ) )
            {
                ulNumOfEntries =       CosaGetParamValueUlong("Device.Ethernet.LinkNumberOfEntries");

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Ethernet.Link.", i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath) , "Device.Ethernet.Link.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) ,"%s.Name", ucEntryFullPath);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
               
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.Ethernet.VLANTermination.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.WiFi.SSID.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.Bridging.Bridge.", TRUE ) )
            {
                ulNumOfEntries =  CosaGetParamValueUlong("Device.Bridging.BridgeNumberOfEntries");
                AnscTraceFlow(("----------CosaUtilGetLowerLayers, bridgenum:%lu\n", ulNumOfEntries));
                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex("Device.Bridging.Bridge.", i);
                    AnscTraceFlow(("----------CosaUtilGetLowerLayers, instance num:%lu\n", ulEntryInstanceNum));

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath) ,"Device.Bridging.Bridge.%lu", ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                        rc = sprintf_s(ucLowerEntryPath, sizeof(ucLowerEntryPath) , "%s.PortNumberOfEntries", ucEntryFullPath); 
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
                        
                        ulEntryPortNum = CosaGetParamValueUlong(ucLowerEntryPath);  
                        AnscTraceFlow(("----------CosaUtilGetLowerLayers, Param:%s,port num:%lu\n",ucLowerEntryPath, ulEntryPortNum));

                        for ( j = 1; j<= ulEntryPortNum; j++) {
                            rc = sprintf_s(ucLowerEntryName, sizeof(ucLowerEntryName) , "%s.Port.%lu", ucEntryFullPath, j);
                            if(rc < EOK)
                            {
                                ERR_CHK(rc);
                            }
                            rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) ,"%s.Port.%lu.Name", ucEntryFullPath, j);
                            if(rc < EOK)
                            {
                                ERR_CHK(rc);
                            }
                            AnscTraceFlow(("----------CosaUtilGetLowerLayers, Param:%s,Param2:%s\n", ucLowerEntryName, ucEntryParamName));
                        
                            if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                                 AnscEqualString(ucEntryNameValue, pKeyword , TRUE ) )
                            {
                                pMatchedLowerLayer =  AnscCloneString(ucLowerEntryName);
                                AnscTraceFlow(("----------CosaUtilGetLowerLayers, J:%lu, LowerLayer:%s\n", j, pMatchedLowerLayer));
                                break;
                            }
                        }
                    }
                }
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.PPP.Interface.", TRUE ) )
            {
            }
            else if ( AnscEqualString(pTableStringToken->Name, "Device.DSL.Channel.", TRUE ) )
            {
            }
            
            if ( pMatchedLowerLayer )
            {
                AnscFreeMemory(pTableStringToken);
                break;
            }
        }

        AnscFreeMemory(pTableStringToken);
    }

    if ( pTableListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTableListTokenChain);
    }

    AnscTraceWarning
        ((
            "CosaUtilGetLowerLayers: %s matched LowerLayer(%s) with keyword %s in the table %s\n",
            pMatchedLowerLayer ? "Found a":"Not find any",
            pMatchedLowerLayer ? (char*)pMatchedLowerLayer : "",
            pKeyword,
            pTableName
        ));

    return pMatchedLowerLayer;
}

/*
    CosaUtilGetFullPathNameByKeyword
    
   Description:
        This funcation serves for searching other pathname  except lowerlayer.
        
    PUCHAR                      pTableName
        This is the Table names divided by ",". For example 
        "Device.Ethernet.Interface., Device.Dhcpv4." 
        
    PUCHAR                      pParameterName
        This is the parameter name which hold the keyword. eg: "name"
        
    PUCHAR                      pKeyword
        This is keyword. eg: "wan0".

    return value
        return result string which need be free by the caller.
*/
PUCHAR
CosaUtilGetFullPathNameByKeyword
    (
        PUCHAR                      pTableName,
        PUCHAR                      pParameterName,
        PUCHAR                      pKeyword
    )
{

    ULONG                           ulNumOfEntries              = 0;
    ULONG                           i                           = 0;
    ULONG                           ulEntryNameLen              = 256;
    char                            ucEntryParamName[256+300]   = {0};
    char                            ucEntryNameValue[256]       = {0};
    UCHAR                           ucTmp[128+920]              = {0};
    char                            ucTmp2[128+400]             = {0};
    char                            ucEntryFullPath[256+300]    = {0};
    PUCHAR                          pMatchedLowerLayer          = NULL;
    ULONG                           ulEntryInstanceNum          = 0;   
    PANSC_TOKEN_CHAIN               pTableListTokenChain        = (PANSC_TOKEN_CHAIN)NULL;
    PANSC_STRING_TOKEN              pTableStringToken           = (PANSC_STRING_TOKEN)NULL;
    PUCHAR                          pString                     = NULL;
    PUCHAR                          pString2                    = NULL;
    errno_t                         rc                          = -1;

    if ( !pTableName || AnscSizeOfString(pTableName) == 0 ||
         !pKeyword   || AnscSizeOfString(pKeyword) == 0   ||
         !pParameterName   || AnscSizeOfString(pParameterName) == 0
       )
    {
        return NULL;
    }

    pTableListTokenChain = AnscTcAllocate(pTableName, ",");

    if ( !pTableListTokenChain )
    {
        return NULL;
    }

    while ((pTableStringToken = AnscTcUnlinkToken(pTableListTokenChain)))
    {
        if ( pTableStringToken->Name )
        {
            /* Get the string XXXNumberOfEntries */
            pString2 = &pTableStringToken->Name[0];
            pString  = pString2;
            for (i = 0;pTableStringToken->Name[i]; i++)
            {
                if ( pTableStringToken->Name[i] == '.' )
                {
                    pString2 = pString;
                    pString  = &pTableStringToken->Name[i+1];
                }
            }

            pString--;
            pString[0] = '\0';
            rc = sprintf_s(ucTmp2, sizeof(ucTmp2) , "%sNumberOfEntries", pString2); 
            if(rc < EOK)
            {
                ERR_CHK(rc);
            }
            pString[0] = '.';

            /* Enumerate the entry in this table */
            if ( TRUE )
            {
                pString2--;
                pString2[0]='\0';
                rc = sprintf_s(ucTmp, sizeof(ucTmp) , "%s.%s", pTableStringToken->Name, ucTmp2); 
                if(rc < EOK)
                {
                    ERR_CHK(rc);
                }
                pString2[0]='.';
                ulNumOfEntries =       CosaGetParamValueUlong(ucTmp);

                for ( i = 0 ; i < ulNumOfEntries; i++ )
                {
                    ulEntryInstanceNum = CosaGetInstanceNumberByIndex(pTableStringToken->Name, i);

                    if ( ulEntryInstanceNum )
                    {
                        rc = sprintf_s(ucEntryFullPath, sizeof(ucEntryFullPath) , "%s%lu.", pTableStringToken->Name, ulEntryInstanceNum);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }

                        rc = sprintf_s(ucEntryParamName, sizeof(ucEntryParamName) , "%s%s", ucEntryFullPath, pParameterName);
                        if(rc < EOK)
                        {
                            ERR_CHK(rc);
                        }
               
                        if ( ( 0 == CosaGetParamValueString(ucEntryParamName, ucEntryNameValue, &ulEntryNameLen)) &&
                             AnscEqualString(ucEntryNameValue, pKeyword, TRUE ) )
                        {
                            pMatchedLowerLayer =  AnscCloneString(ucEntryFullPath);

                            break;
                        }
                    }
                }
            }

            if ( pMatchedLowerLayer )
            {
                AnscFreeMemory(pTableStringToken);
                break;
            }
        }

        AnscFreeMemory(pTableStringToken);
    }

    if ( pTableListTokenChain )
    {
        AnscTcFree((ANSC_HANDLE)pTableListTokenChain);
    }

    AnscTraceWarning
        ((
            "CosaUtilGetFullPathNameByKeyword: %s matched parameters(%s) with keyword %s in the table %s(%s)\n",
            pMatchedLowerLayer ? "Found a":"Not find any",
            pMatchedLowerLayer ? (char*)pMatchedLowerLayer : "",
            pKeyword,
            pTableName,
            pParameterName
        ));

    return pMatchedLowerLayer;
}

ANSC_STATUS
CosaUtilGetStaticRouteTable
    (
        UINT                        *count,
        StaticRoute                 **out_sroute
    )
{
	return CosaUtilGetStaticRouteTablePriv(count, out_sroute);
}

