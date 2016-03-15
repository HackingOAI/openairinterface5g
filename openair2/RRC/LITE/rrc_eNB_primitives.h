/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2015 Eurecom

    OpenAirInterface is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.


    OpenAirInterface is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with OpenAirInterface.The full GNU General Public License is
    included in this distribution in the file called "COPYING". If not,
    see <http://www.gnu.org/licenses/>.

   Contact Information
   OpenAirInterface Admin: openair_admin@eurecom.fr
   OpenAirInterface Tech : openair_tech@eurecom.fr
   OpenAirInterface Dev  : openair4g-devel@lists.eurecom.fr

   Address      : Eurecom, Campus SophiaTech, 450 Route des Chappes, CS 50193 - 06904 Biot Sophia Antipolis cedex, FRANCE

 *******************************************************************************/

/*! \file event_handler.h
* \brief primitives to handle event acting on oai
* \author Konstantinos Alexandris,
* \date 2015
* \version 0.5
* @ingroup _oai
*/

#include "UTIL/OCG/OCG.h"

// HO parameters initialization
void init_HO(Handover_info* ho_info);

// Getters/setters for HO parameters
void set_hys(int enb_module_id,long hys);

long get_hys(int enb_module_id);

void set_ttt_ms(int enb_module_id,long ttt_ms);

long get_ttt_ms(int enb_module_id);

void set_ofn(int enb_module_id,long ofn);

long get_ofn(int enb_module_id);

void set_ocn(int enb_module_id,long ocn);

long get_ocn(int enb_module_id);

void set_ofs(int enb_module_id,long ofs);

long get_ofs(int enb_module_id);

void set_ocs(int enb_module_id,long ocs);

long get_ocs(int enb_module_id);

void set_off(int enb_module_id,long off);

long get_off(int enb_module_id);

void set_rsrp_filter_coeff(int enb_module_id,float rsrp_filter_coeff);

float get_rsrp_filter_coeff(int enb_module_id);

void set_rsrq_filter_coeff(int enb_module_id,float rsrq_filter_coeff);

float get_rsrq_filter_coeff(int enb_module_id);