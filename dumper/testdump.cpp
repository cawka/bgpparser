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

// testr.cpp : Defines the entry point for the console application.
// Author: Jason Ryder
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
#include "MRTBgp4MPEntry.h"
#include "MRTBgp4MPSnapshot.h"
#include "MRTTblDump.h"
#include "MRTTblDumpV2PeerIndexTbl.h"
#include "MRTTblDumpV2RibIPv4Unicast.h"
#include "MRTTblDumpV2RibIPv4Multicast.h"
#include "MRTTblDumpV2RibIPv6Unicast.h"
#include "MRTTblDumpV2RibIPv6Multicast.h"

/* BGP classes */
#include "BGPCommonHeader.h"
#include "BGPUpdate.h"

#include "AttributeType.h"
#include "AttributeTypeOrigin.h"

/* Dumper class */
#include "Dumper.h"
#include "BGPMessageDumper.h"
#include "BGPDumper.h"
#include "MRTTblDumpV1Dumper.h"
#include "MRTTblDumpV2Dumper.h"
#include "MRTBgp4MPMessageDumper.h"
#include "MRTBgp4MPStateChangeDumper.h"

#define MAX_MRT_TYPE_NUM 49
#define MAX_MRT_SUBTYPE_NUM 6

/* Constants */
#define MRT_COMMON_HDR_LEN 12

extern "C" {
#include "cfile_tools.h"
#include "xmlinternal.h"
}

int dump_type = 0; // 1: TABLE_DUMP1
                   // 2: TABLE_DUMP2
                   // 0: BGP4MP

const char* MRTTypes[] = {
  "NULL",
  "START", // 1
  "DIE",
  "I_AM_DEAD", // 3
  "PEER_DOWN",
  "BGP", // --5--
  "RIP",           "IDRP",    "RIPNG",   "BGP4PLUS",
  "BGP4PLUS_01",   // --10--
  "OSPF",          // 11
  "TABLE_DUMP",    // 12
  "TABLE_DUMP_V2", // 13
  "UNKNOWN",
  "UNKNOWN",   // --15--
  "BGP4MP",    // 16
  "BGP4MP_ET", // 17
  "UNKNOWN",       "UNKNOWN",
  "UNKNOWN", // --20--
  "UNKNOWN",       "UNKNOWN", "UNKNOWN", "UNKNOWN",
  "UNKNOWN", // --25--
  "UNKNOWN",       "UNKNOWN", "UNKNOWN", "UNKNOWN",
  "UNKNOWN", // --30--
  "UNKNOWN",
  "ISIS",    // 32
  "ISIS_ET", // 33
  "UNKNOWN",
  "UNKNOWN", // --35--
  "UNKNOWN",       "UNKNOWN", "UNKNOWN", "UNKNOWN",
  "UNKNOWN", // --40--
  "UNKNOWN",       "UNKNOWN", "UNKNOWN", "UNKNOWN",
  "UNKNOWN", // --45--
  "UNKNOWN",       "UNKNOWN",
  "OSPFv3",   // 48
  "OSPFv3_ET" // 49
};

const char* MRTSubTypes[][MAX_MRT_SUBTYPE_NUM + 1] = {
  {/* 0 */},
  {/* 1 */},
  {/* 2 */},
  {/* 3 */},
  {/* 4 */},
  {/* 5 */},
  {/* 6 */},
  {/* 7 */},
  {/* 8 */},
  {/* 9 */},
  {/* 10 */},
  {"OSPF_STATE_CHANGE", "OSPF_LSA_UPDATE"}, // 11
  {"UNKNOWN", "AFI_IPv4", "AFI_IPv6"},      // 12
  {"UNKNOWN", "PEER_INDEX_TABLE",           // 13
   "RIB_IPV4_UNICAST", "RIB_IPV4_MULTICAST", "RIB_IPV6_UNICAST", "RIB_IPV6_MULTICAST",
   "RIB_GENERIC"},
  {/* 14 */},
  {/* 15 */},
  {"BGP4MP_STATE_CHANGE", "BGP4MP_MESSAGE", // 16
   "BGP4MP_ENTRY", "BGP4MP_SNAPSHOT", "BGP4MP_MESSAGE_32BIT_AS", "BGP4MP_MESSAGE_AS4"},
  {/* 17 */},
  {/* 18 */},
  {/* 19 */},
  {/* 20 */},
  {/* 21 */},
  {/* 22 */},
  {/* 23 */},
  {/* 24 */},
  {/* 25 */},
  {/* 26 */},
  {/* 27 */},
  {/* 28 */},
  {/* 29 */},
  {/* 30 */},
  {/* 31 */},
  {"UNKNOWN"},
  {"UNKNOWN"}, // 32, 33
  {/* 34 */},
  {/* 35 */},
  {/* 36 */},
  {/* 37 */},
  {/* 38 */},
  {/* 39 */},
  {/* 40 */},
  {/* 41 */},
  {/* 42 */},
  {/* 43 */},
  {/* 44 */},
  {/* 45 */},
  {/* 46 */},
  {/* 47 */},
  {"UNKNOWN"},
  {"UNKNOWN"} // 48, 49
};

/*
 * getChildNodeByName() : Get a pointer to the child with the given name
 * Returns a pointer to the data, not a copy of it
 */
xmlNodePtr
getChildNodeByName(xmlNodePtr parent_node, char* name)
{
  xmlNodePtr child_node = NULL;

  if (parent_node == NULL || name == NULL)
    return NULL;
  for (child_node = parent_node->children; child_node != NULL; child_node = child_node->next) {
    if (child_node->name && (strcmp((char*)child_node->name, name) == 0)) {
      return child_node;
    }
  }
  return NULL;
}

string
asPlainToDot(char* as_str)
{
  if (!as_str)
    return "";

  unsigned int as = atoi(as_str);
  ostringstream ostr;
  if (as < 65536) {
    ostr << as;
  }
  else {
    int as_h = as / 65536;
    int as_l = as % 65536;
    ostr << as_h << "." << as_l;
  }
  return ostr.str();
}

int
xmlNodeToTxt(xmlNodePtr node)
{
  //
  // Dump to string
  // (Time consuming)
  //
  int len = 0;
  xmlBufferPtr buff = xmlBufferCreate();
  len = xmlNodeDump(buff, NULL, node, 0, 0);
  char* xml_line = (char*)buff->content;
  // printf("%s", (char *) buff->content);
  // printf("\n");

  //
  // Parse XML string
  // (Time consuming)
  //
  xmlDocPtr bgp_data = xmlReadMemory(xml_line, strlen(xml_line), "parse_xml_line.xml", NULL, 0);
  // if (bgp_data == NULL || (_schema > 0 && is_valid(bgp_data, schema_file) < 0) ) {
  if (bgp_data == NULL) {
    fprintf(stderr, "Failed to parse and validate document!\n");
    return 1;
  }

  xmlNodePtr bgp_message = xmlDocGetRootElement(bgp_data); // Get the root element node
  if (!bgp_message->name)
    return 0;

  map<const char*, string> data;
  vector<xmlChar*> withdrawn;
  vector<xmlChar*> nlri;
  vector<string> communy;
  vector<string> as_path;
  vector<string> as4_path;
  string mrt_type;

  if (dump_type == 0) {
    mrt_type = "BGP4MP";
  }
  else if (dump_type == 1) {
    mrt_type = "TABLE_DUMP";
  }
  else if (dump_type == 2) {
    mrt_type = "TABLE_DUMP2";
  }
  else {
    mrt_type = "BGP4MP";
  }

  /* Default values */
  data["ATOMIC_AGGREGATE"] = string("NAG");
  data["LOCAL_PREF"] = string("0");
  data["MULTI_EXIT_DISC"] = string("0");

  /* High level nodes */
  xmlNodePtr time_node, peering_node, ascii_msg_node, status_msg_node;
  time_node = NULL;
  peering_node = NULL;
  ascii_msg_node = NULL;
  status_msg_node = NULL;
  ;
  for (xmlNodePtr child_node = bgp_message->children; child_node != NULL;
       child_node = child_node->next) {
    if (!child_node->name)
      continue;
    // cout << child_node->name;
    /* Nodes */
    if ((strcmp((char*)child_node->name, "TIME") == 0))
      time_node = child_node;
    else if ((strcmp((char*)child_node->name, "PEERING") == 0))
      peering_node = child_node;
    else if ((strcmp((char*)child_node->name, "ASCII_MSG") == 0))
      ascii_msg_node = child_node;
    else if ((strcmp((char*)child_node->name, "STATUS_MSG") == 0))
      status_msg_node = child_node;
  }

  if (!time_node) {
    cerr << "Undefined xml node: TIME" << endl;
    return 0;
  }
  if (!peering_node) {
    cerr << "Undefined xml node: PEERING" << endl;
    return 0;
  }

  /* Timestamp */
  {
    xmlNodePtr ts_data = getChildNodeByName(time_node, "TIMESTAMP");
    xmlChar* ts = xmlNodeGetContent(ts_data);
    data["TIMESTAMP"] = string((char*)ts);
    xmlFree(ts);
  }

  /* Peer Addr / AS */
  {
    xmlNodePtr peer_addr = getChildNodeByName(peering_node, "DST_ADDR");
    xmlChar* addr = xmlNodeGetContent(peer_addr);
    xmlNodePtr peer_as = getChildNodeByName(peering_node, "DST_AS");
    xmlChar* as = xmlNodeGetContent(peer_as);
    data["PEER_ADDR"] = string((char*)addr);
    data["PEER_AS"] = asPlainToDot((char*)as);
    xmlFree(addr);
    xmlFree(as);
  }

  if (ascii_msg_node) {
    xmlNodePtr update_node = getChildNodeByName(ascii_msg_node, "UPDATE");
    if (update_node) {
      //----------------------------------
      // Child node of update
      //----------------------------------
      xmlNodePtr with_node, pa_node, nlri_node;
      with_node = NULL;
      pa_node = NULL;
      nlri_node = NULL;
      for (xmlNodePtr child_node = update_node->children; child_node != NULL;
           child_node = child_node->next) {
        if (!child_node->name)
          continue;
        // cout << child_node->name;
        /* Nodes */
        if ((strcmp((char*)child_node->name, "WITHDRAWN") == 0))
          with_node = child_node;
        else if ((strcmp((char*)child_node->name, "PATH_ATTRIBUTES") == 0))
          pa_node = child_node;
        else if ((strcmp((char*)child_node->name, "NLRI") == 0))
          nlri_node = child_node;
      }

      //----------------------------------
      // Path Attributes
      //----------------------------------
      if (pa_node) {
        for (xmlNodePtr attr_node = pa_node->children; attr_node != NULL;
             attr_node = attr_node->next) {
          if (!attr_node->name)
            continue;

          // Type
          xmlNodePtr type_node = getChildNodeByName(attr_node, "TYPE");
          xmlChar* type = xmlNodeGetContent(type_node);
          // Value
          xmlNodePtr value_node = getChildNodeByName(attr_node, (char*)type);
          xmlChar* value = xmlNodeGetContent(value_node);

          if (strcmp((char*)type, "ORIGIN") == 0) {
            data["ORIGIN"] = string((char*)value);
          }
          else if (strcmp((char*)type, "NEXT_HOP") == 0) {
            data["NEXT_HOP"] = string((char*)value);
          }
          else if (strcmp((char*)type, "LOCAL_PREF") == 0) {
            data["LOCAL_PREF"] = string((char*)value);
          }
          else if (strcmp((char*)type, "MULTI_EXIT_DISC") == 0) {
            data["MULTI_EXIT_DISC"] = string((char*)value);
          }
          else if (strcmp((char*)type, "ATOMIC_AGGREGATE") == 0) {
            data["ATOMIC_AGGREGATE"] = string("AG");
          }
          else if (strcmp((char*)type, "AGGREGATOR") == 0) {
            xmlNodePtr as_node = getChildNodeByName(value_node, "AS");
            xmlChar* agg_as = xmlNodeGetContent(as_node);
            xmlNodePtr addr_node = getChildNodeByName(value_node, "ADDR");
            xmlChar* agg_addr = xmlNodeGetContent(addr_node);

            ostringstream ostr;
            ostr << asPlainToDot((char*)agg_as) << " " << agg_addr;
            data["AGGREGATOR"] = ostr.str();

            xmlFree(agg_as);
            xmlFree(agg_addr);
          }
          else if (strcmp((char*)type, "AS4_AGGREGATOR") == 0) {
            xmlNodePtr as_node = getChildNodeByName(value_node, "AS");
            xmlChar* agg_as = xmlNodeGetContent(as_node);
            xmlNodePtr addr_node = getChildNodeByName(value_node, "ADDR");
            xmlChar* agg_addr = xmlNodeGetContent(addr_node);

            ostringstream ostr;
            ostr << asPlainToDot((char*)agg_as) << " " << agg_addr;
            data["AGGREGATOR"] = ostr.str();

            xmlFree(agg_as);
            xmlFree(agg_addr);
          }
          else if (strcmp((char*)type, "COMMUNITIES") == 0) {
            bool first = true;
            ostringstream ostr;
            for (xmlNodePtr c_node = value_node->children; c_node != NULL; c_node = c_node->next) {
              if (!c_node->name)
                continue;

              if (strcmp((char*)c_node->name, "NO_EXPORT") == 0) {
                if (first) {
                  first = false;
                }
                else {
                  ostr << " ";
                }
                ostr << "no-export";
              }
              else if (strcmp((char*)c_node->name, "NO_ADVERTISE") == 0) {
                if (first) {
                  first = false;
                }
                else {
                  ostr << " ";
                }
                ostr << "no-advertise";
              }
              else if (strcmp((char*)c_node->name, "NO_EXPORT_SUBCONFED") == 0) {
                if (first) {
                  first = false;
                }
                else {
                  ostr << " ";
                }
                ostr << "no-export-subconfed";
              }
              else if ((strcmp((char*)c_node->name, "COMMUNITY") == 0)
                       || (strcmp((char*)c_node->name, "RESERVED_COMMUNITY") == 0)) {
                xmlNodePtr as_node = getChildNodeByName(c_node, "AS");
                xmlChar* as = xmlNodeGetContent(as_node);
                xmlNodePtr v_node = getChildNodeByName(c_node, "VALUE");
                xmlChar* value = xmlNodeGetContent(v_node);

                if (first) {
                  first = false;
                }
                else {
                  ostr << " ";
                }
                ostr << asPlainToDot((char*)as) << ":" << value;

                xmlFree(as);
                xmlFree(value);
              }
            }
            data["COMMUNITIES"] = ostr.str();
          }
          else if ((strcmp((char*)type, "AS_PATH") == 0)
                   || (strcmp((char*)type, "AS4_PATH") == 0)) {
            vector<string>* as_path_ref = NULL;
            if ((strcmp((char*)type, "AS_PATH") == 0))
              as_path_ref = &as_path;
            else if ((strcmp((char*)type, "AS4_PATH") == 0))
              as_path_ref = &as4_path;

            bool first_seg = true;
            for (xmlNodePtr seg_node = value_node->children; seg_node != NULL;
                 seg_node = seg_node->next) {
              string sep = " ";
              string head = "";
              string tail = "";
              xmlChar* seg_type = xmlGetProp(seg_node, BAD_CAST "type");

              if (strcmp((char*)seg_type, "AS_SEQUENCE") == 0) {
                sep = " ";
                head = "";
                tail = "";
              }
              else if (strcmp((char*)seg_type, "AS_SET") == 0) {
                sep = ",";
                head = "{";
                tail = "}";
              }
              else if (strcmp((char*)seg_type, "AS_CONFED_SET") == 0) {
                sep = ",";
                head = "[";
                tail = "]";
              }
              else if (strcmp((char*)seg_type, "AS_CONFED_SEQUENCE") == 0) {
                sep = " ";
                head = "(";
                tail = ")";
              }

              if (first_seg) {
                first_seg = false;
              }
              else {
                as_path_ref->push_back(" ");
              }
              // Head, ex: {
              as_path_ref->push_back(head);

              bool first = true;
              for (xmlNodePtr as_node = seg_node->children; as_node != NULL;
                   as_node = as_node->next) {
                xmlChar* as = xmlNodeGetContent(as_node);
                if (first) {
                  first = false;
                }
                else {
                  as_path_ref->push_back(sep);
                }
                as_path_ref->push_back(asPlainToDot((char*)as));
                xmlFree(as);
              }

              // Tail, ex: }
              as_path_ref->push_back(tail);
              xmlFree(seg_type);
            }
          }
          else if (strcmp((char*)type, "MP_REACH_NLRI") == 0) {
            xmlNodePtr mp_nh_node = getChildNodeByName(value_node, "NEXT_HOP");
            xmlChar* mp_nh = xmlNodeGetContent(mp_nh_node);
            data["NEXT_HOP"] = string((char*)mp_nh);
            xmlFree(mp_nh);

            xmlNodePtr mp_nlri_node = getChildNodeByName(value_node, "NLRI");
            for (xmlNodePtr mp_prefix_node = mp_nlri_node->children; mp_prefix_node != NULL;
                 mp_prefix_node = mp_prefix_node->next) {
              if (!mp_prefix_node->name)
                continue;
              nlri.push_back(xmlNodeGetContent(mp_prefix_node));
            }
          }
          else if (strcmp((char*)type, "MP_UNREACH_NLRI") == 0) {
            xmlNodePtr nlri_node = getChildNodeByName(value_node, "WITHDRAWN");
            for (xmlNodePtr prefix_node = nlri_node->children; prefix_node != NULL;
                 prefix_node = prefix_node->next) {
              if (!prefix_node->name)
                continue;
              withdrawn.push_back(xmlNodeGetContent(prefix_node));
            }
          }
          xmlFree(type);
          xmlFree(value);
        }
      }

      {
        ostringstream ostr;
        unsigned int as_path_len = as_path.size();
        unsigned int as4_path_len = as4_path.size();
        int offset = as_path_len - as4_path_len;

        for (int i = 0; i < offset; i++) {
          ostr << as_path[i];
        }
        if (as4_path_len > 0) {
          if (offset > 0)
            ostr << " ";
          for (unsigned int i = 0; i < as4_path_len; i++) {
            ostr << as4_path[i];
          }
        }
        data["AS_PATH"] = ostr.str();
      }

      //----------------------------------
      // Withdrawn
      //----------------------------------
      if (with_node) {
        for (xmlNodePtr prefix_node = with_node->children; prefix_node != NULL;
             prefix_node = prefix_node->next) {
          if (!prefix_node->name)
            continue;
          withdrawn.push_back(xmlNodeGetContent(prefix_node));
        }
      }
      if (withdrawn.size() > 0) {
        for (unsigned int i = 0; i < withdrawn.size(); i++) {
          cout << mrt_type << "|" << data["TIMESTAMP"] << "|W"
               << "|" << data["PEER_ADDR"] << "|" << data["PEER_AS"] << "|" << withdrawn.at(i)
               << endl;
        }
      }

      //----------------------------------
      // NLRI
      //----------------------------------
      string anno_type = "A";
      if (dump_type > 0) {
        anno_type = "B";
      }
      if (nlri_node) {
        for (xmlNodePtr prefix_node = nlri_node->children; prefix_node != NULL;
             prefix_node = prefix_node->next) {
          if (!prefix_node->name)
            continue;
          nlri.push_back(xmlNodeGetContent(prefix_node));
        }
      }

      if (nlri.size() > 0) {
        for (unsigned int i = 0; i < nlri.size(); i++) {
          cout << mrt_type << "|" << data["TIMESTAMP"] << "|" << anno_type << "|"
               << data["PEER_ADDR"] << "|" << data["PEER_AS"] << "|" << nlri.at(i) << "|"
               << data["AS_PATH"] << "|" << data["ORIGIN"] << "|" << data["NEXT_HOP"] << "|"
               << data["LOCAL_PREF"] << "|" << data["MULTI_EXIT_DISC"] << "|" << data["COMMUNITIES"]
               << "|" << data["ATOMIC_AGGREGATE"] << "|" << data["AGGREGATOR"] << "|" << endl;
        }
      }
    }
  }
  else if (status_msg_node) {
    xmlNodePtr session_status_node = getChildNodeByName(status_msg_node, "SESSION_STATUS");
    xmlNodePtr session_node = getChildNodeByName(session_status_node, "SESSION");
    xmlNodePtr sc_node = getChildNodeByName(session_node, "STATE_CHANGE");
    xmlNodePtr old_state_node = getChildNodeByName(sc_node, "OLD_STATE");
    xmlChar* old_state = xmlNodeGetContent(old_state_node);
    xmlNodePtr new_state_node = getChildNodeByName(sc_node, "NEW_STATE");
    xmlChar* new_state = xmlNodeGetContent(new_state_node);

    data["OLD_STATE"] = string((char*)old_state);
    data["NEW_STATE"] = string((char*)new_state);
    xmlFree(old_state);
    xmlFree(new_state);

    cout << "BGP4MP"
         << "|" << data["TIMESTAMP"] << "|STATE"
         << "|" << data["PEER_ADDR"] << "|" << data["PEER_AS"] << "|" << data["OLD_STATE"] << "|"
         << data["NEW_STATE"] << endl;
  }

  //----------------------------------------
  // Clean up xmlChar*
  //----------------------------------------
  for (unsigned int i = 0; i < withdrawn.size(); i++) {
    xmlFree(withdrawn[i]);
  }
  for (unsigned int i = 0; i < nlri.size(); i++) {
    xmlFree(nlri[i]);
  }

  xmlFreeDoc(bgp_data);
  xmlBufferFree(buff); /* free xml buffer       */

  return 0;
}

int
procBgpMessage(xmlNodePtr node, int flag_format, int flag_newline, int flag_bgpdump)
{
  if (!flag_bgpdump) {
    Dumper::printXmlNode(node, flag_format, flag_newline);
  }
  else {
    // <Test Code>
    xmlNodeToTxt(node);
    // </Test Code>
  }
  return 0;
}

int
procBgpMessages(xmlNodePtr node, int flag_format, int flag_newline, int flag_bgpdump)
{
  for (xmlNodePtr child_node = node->children; child_node != NULL; child_node = child_node->next) {
    procBgpMessage(child_node, flag_format, flag_newline, flag_bgpdump);
  }
  return 1;
}

int
procXml(Dumper* dumper, int flag_format, int flag_newline, int flag_bgpdump)
{
  xmlNodePtr root_node = dumper->genXml();

  if (!root_node->name)
    return 0;

  if ((strcmp((char*)root_node->name, "BGP_MESSAGES") == 0))
    procBgpMessages(root_node, flag_format, flag_newline, flag_bgpdump);
  else if ((strcmp((char*)root_node->name, "BGP_MESSAGE") == 0))
    procBgpMessage(root_node, flag_format, flag_newline, flag_bgpdump);

  xmlUnlinkNode(root_node);
  xmlFreeNode(root_node);
  return 0;
}

void
print_usage()
{
  char* usage = "Usage: mrt2xml [-m] [-f] [-n] [-h] [-M] [-t dump|change ] input_binary_file(s)\n\
	-m and -M produce two different kinds of binary formats,\n\
	-t defines whether timestamps in machine-readable format should be the timestamp of\n\
	   when the dump was made, or when the dumped route was last change (only effective for RIB dumps)\n";
  cout << usage;
}

// int _tmain(int argc, _TCHAR* argv[])
int
main(int argc, char** argv)
{
  /* -------------------------- */
  /* Get options                */
  /* -------------------------- */
  const char* const short_options = "hmnfBt:";
  const struct option long_options[] = {{"help", 0, NULL, 'h'},
                                        {"compact", 0, NULL, 'm'},
                                        {"format", 0, NULL, 'f'},
                                        {"newline", 0, NULL, 'n'},
                                        {"bgpdump", 0, NULL, 'B'},
                                        {"type", 1, NULL, 't'},
                                        {NULL, 0, NULL, 0}};
  int next_options;
  int flag_compact = 0;
  int flag_format = 1;
  int flag_newline = 1;
  int flag_bgpdump = 0;

  do {
    next_options = getopt_long(argc, argv, short_options, long_options, NULL);
    switch (next_options) {
    case 'h':
      print_usage();
      break;
    case 'm':
      flag_compact = 1;
      flag_format = 0;
      flag_newline = 1;
      break;
    case 'f':
      flag_format = 1;
      break;
    case 'n':
      flag_newline = 1;
      break;
    case 'B':
      flag_bgpdump = 1;
      break;
    }
  } while (next_options != -1);

  /* -------------------------- */
  /* File                       */
  /* -------------------------- */
  char* fileName;
  if (optind < argc) {
    fileName = argv[optind];
  }

  cout << optind << endl;
  cout << argc;
  exit(0);

  /* -------------------------- */
  /* Process file               */
  /* -------------------------- */
  char rchMRTCommonHeader[MRT_COMMON_HDR_LEN];
  CFRFILE* f;
  f = cfr_open(fileName);
  if (f) {
    unsigned int unTotalBytesRead = 0;
    // The index table needs to persist over the processing
    MRTTblDumpV2PeerIndexTbl* peerIndexTbl = NULL;
    MRTTblDumpV2Dumper* mrt_tblv2_dumper = new MRTTblDumpV2Dumper();

    while (!cfr_eof(f)) {
      bool saveMsg = false;
      size_t szBytesRead = 0;

      /* -------------------------- */
      /* Extrace Binary MRT Message */
      /* -------------------------- */
      /* Read header */
      szBytesRead += cfr_read_n(f, rchMRTCommonHeader, MRT_COMMON_HDR_LEN);
      if (szBytesRead < MRT_COMMON_HDR_LEN) {
        break;
      }

      /* Read length */
      unsigned int unMRTBodyLen = ntohl(*(unsigned int*)(rchMRTCommonHeader + 8));

      /* Compute MRT message length */
      unsigned int unMRTMsgLen = MRT_COMMON_HDR_LEN + unMRTBodyLen;
      char* pchMRTMsg = (char*)malloc(unMRTMsgLen);
      // memset(pchMRTMsg, 0, unMRTMsgLen); // No need to initialize to 0?

      /* Copy the whole MRT record */
      memcpy(pchMRTMsg, (char*)rchMRTCommonHeader, MRT_COMMON_HDR_LEN);
      size_t szBodyRead = cfr_read_n(f, pchMRTMsg + MRT_COMMON_HDR_LEN, unMRTBodyLen);
      if (szBodyRead < unMRTBodyLen) {
        break;
      }
      unTotalBytesRead += szBodyRead;

      /* -------------------------- */
      /* Parse the MRT message      */
      /* -------------------------- */
      // char *pchMRTMsgTemp = reinterpret_cast<char *>(pchMRTMsg);
      uint8_t* pchMRTMsgTemp = reinterpret_cast<uint8_t*>(pchMRTMsg);
      MRTMessage* msg = MRTCommonHeader::newMessage(&pchMRTMsgTemp);

      if (msg == NULL) {
        delete[] pchMRTMsg;
        continue;
      } /* EOF? */
      fflush(stdout);

      switch (msg->getType()) {
      // -------------------------- //
      // [TODO] TABLE_DUMP          //
      // -------------------------- //
      case TABLE_DUMP: {
        dump_type = 1;
        switch (msg->getSubType()) {
        case AFI_IPv4:
        case AFI_IPv6: {
          // Prepare BGP4MP Message pointer ./
          MRTTblDump* tblDump = (MRTTblDump*)msg;

          // Dumper for BGP Message ./
          MRTTblDumpV1Dumper* mrt_tblv1_dumper = new MRTTblDumpV1Dumper();
          mrt_tblv1_dumper->setTblDumpMsg(tblDump);
          procXml((Dumper*)mrt_tblv1_dumper, flag_format, flag_newline, flag_bgpdump);
          delete mrt_tblv1_dumper;
        } break;

        default:
          cerr << "Undefined Subtype (TABLE_DUMP_V1) :" << msg->getSubType() << endl;
          break;
        }
      } break;

      // -------------------------- //
      // TABLE_DUMP_V2              //
      // -------------------------- //
      case TABLE_DUMP_V2: {
        dump_type = 2;
        // Dumper for Table Dumper v2 //
        switch (msg->getSubType()) {
        case RIB_IPV4_UNICAST:
        case RIB_IPV4_MULTICAST:
        case RIB_IPV6_UNICAST:
        case RIB_IPV6_MULTICAST: {
          mrt_tblv2_dumper->setTblDumpMsg((MRTTblDumpV2RibHeader*)msg);
          procXml((Dumper*)mrt_tblv2_dumper, flag_format, flag_newline, flag_bgpdump);
        } break;

        case PEER_INDEX_TABLE: {
          saveMsg = true; // This message has to be preserved //
          peerIndexTbl = (MRTTblDumpV2PeerIndexTbl*)msg;
          mrt_tblv2_dumper->setPeerIndexTbl((MRTTblDumpV2PeerIndexTbl*)msg);
        } break;

        default:
          cerr << "Undefined Subtype (TABLE_DUMP_V2) :" << msg->getSubType() << endl;
          break;
        }
        break;
      } break;

      // -------------------------- //
      // BGP4MP                     //
      // -------------------------- //
      case BGP4MP: {
        dump_type = 0;
        switch (msg->getSubType()) {
        case BGP4MP_MESSAGE:
        case BGP4MP_MESSAGE_AS4: {
          // Prepare BGP4MP Message pointer ./
          MRTBgp4MPMessage* bgp4MPmsg = (MRTBgp4MPMessage*)msg;

          // Dumper for BGP Message ./
          MRTBgp4MPMessageDumper* mrt_bgp4mp_msg_dumper =
            MRTBgp4MPMessageDumper::newDumper(bgp4MPmsg);
          procXml((Dumper*)mrt_bgp4mp_msg_dumper, flag_format, flag_newline, flag_bgpdump);
          delete mrt_bgp4mp_msg_dumper;
        } break;

        case BGP4MP_STATE_CHANGE:
        case BGP4MP_STATE_CHANGE_AS4: {
          // Prepare BGP4MP State Change pointer //
          MRTBgp4MPStateChange* bgp4MPmsg = (MRTBgp4MPStateChange*)msg;

          // Dumper for BGP State Change //
          MRTBgp4MPStateChangeDumper* mrt_bgp4mp_sc_dumper =
            MRTBgp4MPStateChangeDumper::newDumper(bgp4MPmsg);
          procXml((Dumper*)mrt_bgp4mp_sc_dumper, flag_format, flag_newline, flag_bgpdump);
          delete mrt_bgp4mp_sc_dumper;
        } break;

        default:
          cerr << "Undefined Subtype (BGP4MP) :" << msg->getSubType() << endl;
          break;
        }
      } break;

      default:
        cerr << "Undefined Type:" << msg->getType() << endl;
        break;
      }

      /* Clean/Delete per message stuff */
      delete[] pchMRTMsg;
      if (!saveMsg) {
        delete msg;
        msg = NULL;
      }
    }

    /* Clean/Delete Global stuff */
    if (peerIndexTbl) {
      delete peerIndexTbl;
      peerIndexTbl = NULL;
    }
    if (mrt_tblv2_dumper) {
      delete mrt_tblv2_dumper;
      mrt_tblv2_dumper = NULL;
    }
  }
  cfr_close(f);
  return 0;
}

// vim: sw=4 ts=4 sts=4 expandtab
