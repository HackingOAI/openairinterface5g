/*******************************************************************************
    OpenAirInterface
    Copyright(c) 1999 - 2014 Eurecom

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

  Address      : Eurecom, Compus SophiaTech 450, route des chappes, 06451 Biot, France.

 *******************************************************************************/
/*****************************************************************************
Source      user_defs.h

Version     0.1

Date        2016/07/01

Product     NAS stack

Subsystem   NAS main process

Author      Frederic Leroy

Description NAS type definition to manage a user equipment

*****************************************************************************/
#ifndef __USER_DEFS_H__
#define __USER_DEFS_H__

#include "nas_proc_defs.h"
#include "esmData.h"
#include "esm_pt_defs.h"
#include "EMM/emm_fsm_defs.h"
#include "EMM/emmData.h"
#include "EMM/IdleMode_defs.h"

typedef struct {
  int fd;
  proc_data_t proc;
  esm_data_t *esm_data; // ESM internal data (used within ESM only)
  esm_pt_data_t *esm_pt_data;
  esm_ebr_data_t *esm_ebr_data;  // EPS bearer contexts
  emm_fsm_state_t emm_fsm_status; // Current EPS Mobility Management status
  emm_data_t *emm_data; // EPS mobility management data
  emm_plmn_list_t *emm_plmn_list; // list of PLMN identities
} nas_user_t;

#endif
