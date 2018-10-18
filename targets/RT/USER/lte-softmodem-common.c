/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file lte-softmodem-common.c
 * \brief Top-level threads for eNodeB
 * \author Nokia BellLabs France, francois Taburet
 * \date 2012
 * \version 0.1
 * \company Eurecom
 * \email: francois.taburet@nokia-bell-labs.com
 * \note
 * \warning
 */

#include "lte-softmodem.h"
#include "UTIL/OPT/opt.h"
#include "common/config/config_userapi.h"
#include "common/utils/load_module_shlib.h"
static softmodem_params_t softmodem_params;

uint64_t get_softmodem_optmask(void) {
     return softmodem_params.optmask;
}

uint64_t set_softmodem_optmask(uint64_t bitmask) {
     softmodem_params.optmask = softmodem_params.optmask | bitmask;
     return softmodem_params.optmask;
}

softmodem_params_t* get_softmodem_params(void) {
     return &softmodem_params;
}

void set_parallel_conf(char *parallel_conf)
{
  if(strcmp(parallel_conf,"PARALLEL_SINGLE_THREAD")==0)           softmodem_params.thread_struct.parallel_conf = PARALLEL_SINGLE_THREAD;
  else if(strcmp(parallel_conf,"PARALLEL_RU_L1_SPLIT")==0)        softmodem_params.thread_struct.parallel_conf = PARALLEL_RU_L1_SPLIT;
  else if(strcmp(parallel_conf,"PARALLEL_RU_L1_TRX_SPLIT")==0)    softmodem_params.thread_struct.parallel_conf = PARALLEL_RU_L1_TRX_SPLIT;
  printf("[CONFIG] parallel conf is set to %d\n",softmodem_params.thread_struct.parallel_conf);
} 
void set_worker_conf(char *worker_conf)
{
  if(strcmp(worker_conf,"WORKER_DISABLE")==0)                     softmodem_params.thread_struct.worker_conf = WORKER_DISABLE;
  else if(strcmp(worker_conf,"WORKER_ENABLE")==0)                 softmodem_params.thread_struct.worker_conf = WORKER_ENABLE;
  printf("[CONFIG] worker conf is set to %d\n",softmodem_params.thread_struct.worker_conf);
} 
PARALLEL_CONF_t get_thread_parallel_conf(void)
{
  return softmodem_params.thread_struct.parallel_conf;
} 
WORKER_CONF_t get_thread_worker_conf(void)
{
  return softmodem_params.thread_struct.worker_conf;
}  
void get_common_options(void)
{
char *parallel_config=NULL;
char *worker_config=NULL;
int nonbiotflag;


uint32_t online_log_messages;
uint32_t glog_level ;
uint32_t start_telnetsrv;
uint32_t noS1;
uint32_t nokrnmod;
paramdef_t cmdline_params[] =CMDLINE_PARAMS_DESC ;
paramdef_t cmdline_logparams[] =CMDLINE_LOGPARAMS_DESC ;



  config_process_cmdline( cmdline_params,sizeof(cmdline_params)/sizeof(paramdef_t),NULL); 

  if (strlen(in_path) > 0) {
      opt_type = OPT_PCAP;
      opt_enabled=1;
      printf("Enabling OPT for PCAP  with the following file %s \n",in_path);
  }
  if (strlen(in_ip) > 0) {
      opt_enabled=1;
      opt_type = OPT_WIRESHARK;
      printf("Enabling OPT for wireshark for local interface");
  }

  config_process_cmdline( cmdline_logparams,sizeof(cmdline_logparams)/sizeof(paramdef_t),NULL);

  if(config_isparamset(cmdline_logparams,CMDLINE_ONLINELOG_IDX)) {
      set_glog_onlinelog(online_log_messages);
  }
  if(config_isparamset(cmdline_logparams,CMDLINE_GLOGLEVEL_IDX)) {
      set_glog(glog_level);
  }
  if (start_telnetsrv) {
     load_module_shlib("telnetsrv",NULL,0,NULL);
  }

  if (noS1) {
     set_softmodem_optmask(SOFTMODEM_NOS1_BIT);
  }
  if (nokrnmod) {
     set_softmodem_optmask(SOFTMODEM_NOKRNMOD_BIT);
  } 
  if(parallel_config != NULL) set_parallel_conf(parallel_config);
  if(worker_config != NULL)   set_worker_conf(worker_config);
}