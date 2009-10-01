/*
 * Copyright (c) 2008,2009, University of California, Los Angeles All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of NLnetLabs nor the names of its
 *     contributors may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

// mrt2xml.cpp : Conver mrt to xml output
// Author: Pei-chun Cheng
//
#define _USE_32BIT_TIME_T
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemas.h>
#include <getopt.h>
#include <map>

/* MRT classes */
#include "MRTCommonHeader.h"
#include "MRTBgp4MPStateChange.h"
#include "MRTBgp4MPMessage.h"
#include "MRTTblDump.h"
#include "MRTTblDumpV2PeerIndexTbl.h"

/* Dumper class */
#include "Dumper.h"
#include "MRTTblDumpV1Dumper.h"
#include "MRTTblDumpV2Dumper.h"
#include "MRTBgp4MPMessageDumper.h"
#include "MRTBgp4MPStateChangeDumper.h"

#define MAX_MRT_TYPE_NUM    49
#define MAX_MRT_SUBTYPE_NUM  6

/* Constants */
#define MRT_COMMON_HDR_LEN  12

extern "C" {
#include "cfile_tools.h"
#include "xmlinternal.h"
}

int dump_type = 0;   // 1: TABLE_DUMP1
                     // 2: TABLE_DUMP2
                     // 0: BGP4MP


int procBgpMessage(xmlNodePtr node, int flag_format, int flag_newline, int flag_bgpdump)
{
    Dumper::printXmlNode(node, flag_format, flag_newline);
    return 0;
}

int procBgpMessages(xmlNodePtr node, int flag_format, int flag_newline, int flag_bgpdump)
{
    for (xmlNodePtr child_node=node->children; child_node!= NULL; child_node=child_node->next)
    {
        procBgpMessage(child_node, flag_format, flag_newline, flag_bgpdump);
    }
    return 1;
}

int procMsg(Dumper *dumper, int flag_format, int flag_newline, int flag_bgpdump)
{
    if (flag_bgpdump)
    {
        dumper->printAscii(flag_format, flag_newline);
    }
    else
    {
        xmlNodePtr root_node = dumper->genXml();

        if (!root_node->name) return 0;
        if      ((strcmp((char*)root_node->name, "BGP_MESSAGES") == 0)) procBgpMessages(root_node, flag_format, flag_newline, flag_bgpdump);
        else if ((strcmp((char*)root_node->name, "BGP_MESSAGE" ) == 0)) procBgpMessage(root_node,  flag_format, flag_newline, flag_bgpdump);

        xmlUnlinkNode(root_node);
        xmlFreeNode(root_node);
    }
    return 0;
}

void print_usage()
{
    char *usage = "Usage: bgpparser [-f] [-n] [-m] [-h] mrt_file\n\
	-f format the xml bgp message (default: YES),\n\
	-n insert a newline character between each xml bgp message (default: YES),\n\
	-m compact format, equivalent to disable -f and enable -n option (default: NO),\n\
	-r include a root element (default: NO),\n\
	-h dispaly this message\n\
	   \n";
    cout << usage;
}

//int _tmain(int argc, _TCHAR* argv[])
int main(int argc, char** argv)
{
    /* -------------------------- */
    /* Get options                */
    /* -------------------------- */
    const char* const short_options = "hrmnfBt:";
    const struct option long_options[] = {
        { "help",    0, NULL, 'h' },
        { "compact", 0, NULL, 'm' },
        { "format" , 0, NULL, 'f' },
        { "newline", 0, NULL, 'n' },
        { "root",    0, NULL, 'r' },
        { "bgpdump", 0, NULL, 'B' },
        { NULL,      0, NULL, 0   }
    };
    int next_options;
    int flag_compact = 0;
    int flag_format  = 1;
    int flag_newline = 1;
    int flag_root    = 0;
    int flag_bgpdump = 0;

    do {
        next_options=getopt_long(argc,argv,short_options,long_options,NULL);
        switch (next_options){
            case 'h': 
                      print_usage();
                      exit(0);
            case 'm': flag_compact = 1; flag_format = 0; flag_newline = 1; break;
            case 'f': flag_format  = 1; break;
            case 'n': flag_newline = 1; break;
            case 'r': flag_root    = 1; break;
            case 'B': flag_bgpdump = 1; break;
        }
    } while (next_options != -1);

    /* -------------------------- */
    /* File                       */
    /* -------------------------- */
    char* fileName;
    if(optind < argc){
        fileName = argv[optind];
    }
    else if (optind == argc)
    {
        print_usage();
        exit(0);
    }

    /* -------------------------- */
    /* Process file               */
    /* -------------------------- */
    char rchMRTCommonHeader[MRT_COMMON_HDR_LEN];
    CFRFILE *f;
    f = cfr_open(fileName);
    if (f)
    {
        if (flag_root) cout << "<BGP_MESSAGES>" << endl;

        unsigned int unTotalBytesRead = 0;
        // The index table needs to persist over the processing
        MRTTblDumpV2PeerIndexTbl* peerIndexTbl = NULL; 
        MRTTblDumpV2Dumper *mrt_tblv2_dumper = new MRTTblDumpV2Dumper();

        while (!cfr_eof(f))
        {
            bool   saveMsg     = false;
            size_t szBytesRead = 0;

            /* -------------------------- */
            /* Extrace Binary MRT Message */
            /* -------------------------- */
            /* Read header */
            szBytesRead += cfr_read_n(f, rchMRTCommonHeader, MRT_COMMON_HDR_LEN);
            if( szBytesRead < MRT_COMMON_HDR_LEN ) { break; }

            /* Read length */
            unsigned int unMRTBodyLen = ntohl(*(unsigned int *)(rchMRTCommonHeader+8));

            /* Compute MRT message length */
            unsigned int unMRTMsgLen = MRT_COMMON_HDR_LEN + unMRTBodyLen;
            char *pchMRTMsg = (char *)malloc(unMRTMsgLen);
            //memset(pchMRTMsg, 0, unMRTMsgLen); // No need to initialize to 0?


            /* Copy the whole MRT record */
            memcpy(pchMRTMsg, (char *)rchMRTCommonHeader, MRT_COMMON_HDR_LEN);
            size_t szBodyRead = cfr_read_n(f, pchMRTMsg + MRT_COMMON_HDR_LEN, unMRTBodyLen);
            if( szBodyRead < unMRTBodyLen ) { break; }
            unTotalBytesRead += szBodyRead;

            /* -------------------------- */
            /* Parse the MRT message      */
            /* -------------------------- */
            //char *pchMRTMsgTemp = reinterpret_cast<char *>(pchMRTMsg);
            uint8_t *pchMRTMsgTemp = reinterpret_cast<uint8_t *>(pchMRTMsg);
            MRTMessage* msg = MRTCommonHeader::newMessage(&pchMRTMsgTemp);

            if (msg == NULL) { delete [] pchMRTMsg; continue; } /* EOF? */
            fflush(stdout);

            switch (msg->getType())
            {
                // -------------------------- //
                // TABLE_DUMP                 //
                // -------------------------- //
                case TABLE_DUMP:
                {
                    dump_type = 1;
                    switch(msg->getSubType())
                    {
                        case AFI_IPv4:
                        case AFI_IPv6:
                        {
                            // Prepare BGP4MP Message pointer ./
                            MRTTblDump* tblDump = (MRTTblDump*)msg;

                            // Dumper for BGP Message ./
                            MRTTblDumpV1Dumper *mrt_tblv1_dumper = new MRTTblDumpV1Dumper();
                            mrt_tblv1_dumper->setTblDumpMsg(tblDump);
                            procMsg((Dumper *)mrt_tblv1_dumper, flag_format, flag_newline, flag_bgpdump);
                            delete mrt_tblv1_dumper;
                        }
                        break;

                        default:
                            cerr << "Undefined Subtype (TABLE_DUMP_V1) :" <<  msg->getSubType() << endl;
                            break;
                    }		
                }
                break;

                // -------------------------- //
                // TABLE_DUMP_V2              //
                // -------------------------- //
                case TABLE_DUMP_V2:
                {
                    dump_type = 2;
                    // Dumper for Table Dumper v2 //
                    switch (msg->getSubType())
                    {
                        case RIB_IPV4_UNICAST:
                        case RIB_IPV4_MULTICAST:
                        case RIB_IPV6_UNICAST:
                        case RIB_IPV6_MULTICAST:
                        {
                            mrt_tblv2_dumper->setTblDumpMsg((MRTTblDumpV2RibHeader*)msg);
                            procMsg((Dumper *)mrt_tblv2_dumper, flag_format, flag_newline, flag_bgpdump);
                        }
                        break;

                        case PEER_INDEX_TABLE:
                        {
                            saveMsg = true; // This message has to be preserved //
                            peerIndexTbl = (MRTTblDumpV2PeerIndexTbl*)msg;
                            mrt_tblv2_dumper->setPeerIndexTbl((MRTTblDumpV2PeerIndexTbl*)msg);
                        }
                        break;

                        default:
                            cerr << "Undefined Subtype (TABLE_DUMP_V2) :" <<  msg->getSubType() << endl;
                        break;
                    }
                    break;
                }
                break;

                // -------------------------- //
                // BGP4MP                     //
                // -------------------------- //
                case BGP4MP:
                {
                    dump_type = 0;
                    switch (msg->getSubType())
                    {
                        case BGP4MP_MESSAGE:
                        case BGP4MP_MESSAGE_AS4:
                        {
                            // Prepare BGP4MP Message pointer ./
                            MRTBgp4MPMessage* bgp4MPmsg = (MRTBgp4MPMessage*)msg;

                            // Dumper for BGP Message ./
                            MRTBgp4MPMessageDumper *mrt_bgp4mp_msg_dumper = MRTBgp4MPMessageDumper::newDumper(bgp4MPmsg);
                            procMsg((Dumper *)mrt_bgp4mp_msg_dumper, flag_format, flag_newline, flag_bgpdump);
                            delete mrt_bgp4mp_msg_dumper;
                        }
                        break;

                        case BGP4MP_STATE_CHANGE:
                        case BGP4MP_STATE_CHANGE_AS4:
                        {
                            // Prepare BGP4MP State Change pointer //
                            MRTBgp4MPStateChange* bgp4MPmsg = (MRTBgp4MPStateChange*)msg;

                            // Dumper for BGP State Change //
                            MRTBgp4MPStateChangeDumper *mrt_bgp4mp_sc_dumper = MRTBgp4MPStateChangeDumper::newDumper(bgp4MPmsg);
                            procMsg((Dumper *)mrt_bgp4mp_sc_dumper, flag_format, flag_newline, flag_bgpdump);
                            delete mrt_bgp4mp_sc_dumper;
                        }
                        break;

                        default:
                            cerr << "Undefined Subtype (BGP4MP) :" <<  msg->getSubType() << endl;
                        break;
                    }
                }
                break;

                default:
                    cerr << "Undefined Type:" <<  msg->getType() << endl;
                break;
            }

            /* Clean/Delete per message stuff */
            delete [] pchMRTMsg;
            if (!saveMsg)
            {
                delete msg;
                msg =  NULL;
            }
        }

        /* Clean/Delete Global stuff */
        if (peerIndexTbl    ) { delete peerIndexTbl;     peerIndexTbl     = NULL; }
        if (mrt_tblv2_dumper) { delete mrt_tblv2_dumper; mrt_tblv2_dumper = NULL; }

        if (flag_root) cout << "</BGP_MESSAGES>" << endl;
    }
    cfr_close(f);
    return 0;
}


// vim: sw=4 ts=4 sts=4 expandtab
