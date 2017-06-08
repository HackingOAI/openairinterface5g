/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.0  (the "License"); you may not use this file
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


#ifndef __INIT_DEFS_NB_IoT__H__
#define __INIT_DEFS_NB_IoT__H__

#include "PHY/defs.h"
#include "openair2/PHY_INTERFACE/IF_Module_nb_iot.h"

#include "SystemInformationBlockType2.h"
//#include "RadioResourceConfigCommonSIB.h"
#include "RadioResourceConfigDedicated.h"
#include "TDD-Config.h"
#include "MBSFN-SubframeConfigList.h"
#include "MobilityControlInfo.h"
#if defined(Rel10) || defined(Rel14)
#include "SCellToAddMod-r10.h"
#endif

/*brief Configure LTE_DL_FRAME_PARMS with components derived after initial synchronization (MIB-NB decoding + primary/secondary synch).*/
void NB_phy_config_mib_eNB(int     Mod_id,
			int                    CC_id,
			int                    eutra_band,
			int                    Nid_cell,
			int                    Ncp,
			int                    p_eNB,
			uint32_t               dl_CarrierFreq,
			uint32_t               ul_CarrierFreq);
			//struct MasterInformationBlock_NB__operationModeInfo_r13 operationModeInfo);


/*NB_phy_config_sib1_eNB is not needed since NB-IoT use only FDD mode*/

/*brief Configure LTE_DL_FRAME_PARMS with components of SIB2-NB (at eNB).*/

//void NB_phy_config_sib2_eNB(module_id_t                            Mod_id,
//                         int                                		CC_id,
//                         RadioResourceConfigCommonSIB_NB_r13_t      *radioResourceConfigCommon
//                         );

void NB_phy_config_sib2_eNB(uint8_t Mod_id,
                            int CC_id,
                            nb_iot_config_t *config,
			    rf_config_t *rf_config
                            );

void NB_phy_config_dedicated_eNB(module_id_t Mod_id,
                              int CC_id,
                              rnti_t rnti,
                              PhysicalConfigDedicated_NB_r13_t *physicalConfigDedicated);



#endif

