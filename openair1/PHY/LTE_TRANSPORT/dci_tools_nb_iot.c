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

/*! \file PHY/LTE_TRANSPORT/dci_tools.c
 * \brief PHY Support routines (eNB/UE) for filling PDSCH/PUSCH/DLSCH/ULSCH data structures based on DCI PDUs generated by eNB MAC scheduler.
 * \author R. Knopp
 * \date 2011
 * \version 0.1
 * \company Eurecom
 * \email: knopp@eurecom.fr
 * \note
 * \warning
 */
#include "PHY/defs.h"
#include "PHY/extern.h"
#include "SCHED/defs.h"
#ifdef DEBUG_DCI_TOOLS
#include "PHY/vars.h"
#endif
#include "assertions.h"


//#define DEBUG_HARQ

#include "LAYER2/MAC/extern.h"
#include "LAYER2/MAC/defs.h"

#include "PHY/defs_nb_iot.h"

//#define DEBUG_DCI

void NB_add_dci(DCI_PDU_NB *DCI_pdu,void *pdu,rnti_t rnti,unsigned char dci_size_bytes,unsigned char aggregation,unsigned char dci_size_bits,unsigned char dci_fmt)
{

  memcpy(&DCI_pdu->dci_alloc[DCI_pdu->Num_dci].dci_pdu[0],pdu,dci_size_bytes);
  DCI_pdu->dci_alloc[DCI_pdu->Num_dci].dci_length = dci_size_bits;
  DCI_pdu->dci_alloc[DCI_pdu->Num_dci].L          = aggregation;
  DCI_pdu->dci_alloc[DCI_pdu->Num_dci].rnti       = rnti;
  DCI_pdu->dci_alloc[DCI_pdu->Num_dci].format     = dci_fmt;

  DCI_pdu->Num_dci++;

  LOG_D(MAC,"add ue specific dci format %d for rnti %x \n",dci_fmt,rnti);
}


int NB_generate_eNB_ulsch_params_from_dci(PHY_VARS_eNB_NB *eNB,
                                       eNB_rxtx_proc_NB_t *proc,
                                       DCI_CONTENT *DCI_Content,
                                       uint16_t rnti,
                                       DCI_format_NB_t dci_format,
                                       uint8_t UE_id,
                                       uint8_t aggregation,
                                       uint8_t Num_dci
                                      )
{

  LTE_eNB_ULSCH_t *ulsch=eNB->ulsch[UE_id];
  NB_DL_FRAME_PARMS *frame_parms = &eNB->frame_parms;
  int subframe = proc->subframe_tx;
  void *ULSCH_DCI_NB = NULL;
  DCI_PDU_NB *DCI_pdu;

  /// type = 0 => DCI Format N0, type = 1 => DCI Format N1, 1 bits
  uint8_t type;
  /// Subcarrier indication, 6 bits
  uint8_t scind;
  /// Resourse Assignment (RU Assignment), 3 bits
  uint8_t ResAssign;
  /// Modulation and Coding Scheme, 4 bits
  uint8_t mcs;
  /// New Data Indicator, 1 bits
  uint8_t ndi;
  /// Scheduling Delay, 2 bits
  uint8_t Scheddly;
  /// Repetition Number, 3 bits
  uint8_t RepNum;
  /// Redundancy version for HARQ (only use 0 and 2), 1 bits
  uint8_t rv;
  /// DCI subframe repetition Number, 2 bits
  uint8_t DCIRep;
  
  if (dci_format == DCIFormatN0) 
    {
      
      type        = DCI_Content->DCIN0.type;
      scind       = DCI_Content->DCIN0.scind;
      ResAssign   = DCI_Content->DCIN0.ResAssign;
      mcs         = DCI_Content->DCIN0.mcs;
      ndi         = DCI_Content->DCIN0.ndi;
      Scheddly    = DCI_Content->DCIN0.Scheddly;
      RepNum      = DCI_Content->DCIN0.RepNum;
      rv          = DCI_Content->DCIN0.rv;
      DCIRep      = DCI_Content->DCIN0.DCIRep;

      /*Packed DCI here*/
      ((DCIN0_t *)ULSCH_DCI_NB)->type      =type;
      ((DCIN0_t *)ULSCH_DCI_NB)->scind     =scind;
      ((DCIN0_t *)ULSCH_DCI_NB)->ResAssign =ResAssign;
      ((DCIN0_t *)ULSCH_DCI_NB)->Scheddly  =Scheddly;
      ((DCIN0_t *)ULSCH_DCI_NB)->mcs       =mcs;
      ((DCIN0_t *)ULSCH_DCI_NB)->rv        =rv;
      ((DCIN0_t *)ULSCH_DCI_NB)->RepNum    =RepNum;
      ((DCIN0_t *)ULSCH_DCI_NB)->ndi       =ndi;
      ((DCIN0_t *)ULSCH_DCI_NB)->DCIRep    =DCIRep;

      DCI_pdu->Num_dci = Num_dci;

      NB_add_dci(DCI_pdu,ULSCH_DCI_NB,rnti,sizeof(DCIN0_t),aggregation,sizeof_DCIN0_t,DCIFormatN0);

      // use this value to configure PHY both harq_processes and resource mapping.


      
      return(0);
    } 
  else 
    {
        LOG_E(PHY,"generate_eNB_ulsch_params_from_dci, Illegal dci_format %d\n",dci_format);
        return(-1);
    }
}

int NB_generate_eNB_dlsch_params_from_dci(int frame,
                                       uint8_t subframe,
                                       DCI_CONTENT *DCI_Content,
                                       uint16_t rnti,
                                       DCI_format_NB_t dci_format,
                                       LTE_eNB_DLSCH_t **dlsch,
                                       NB_DL_FRAME_PARMS *frame_parms,
                                       uint8_t aggregation,
                                       uint8_t Num_dci
                                       )
{

  void *DLSCH_DCI_NB = NULL;
  DCI_PDU_NB *DCI_pdu;


  //N1 start

  /// type = 0 => DCI Format N0, type = 1 => DCI Format N1, 1 bits
  uint8_t type;
  //NPDCCH order indicator (set to 0),1 bits
  uint8_t orderIndicator;
  // Scheduling Delay, 3 bits
  uint8_t Scheddly;
  // Resourse Assignment (RU Assignment), 3 bits
  uint8_t ResAssign;
  // Modulation and Coding Scheme, 4 bits
  uint8_t mcs;
  // Repetition Number, 4 bits
  uint8_t RepNum;
  // Reserved 5 bits
  uint8_t Reserved;
  // DCI subframe repetition Number, 2 bits
  uint8_t DCIRep;
  // New Data Indicator,1 bits
  uint8_t ndi;
  // HARQ-ACK resource,4 bits
  uint8_t HARQackRes;

  //N2 start

  //Direct indication information, 8 bits
  uint8_t directIndInf;
  // Reserved information bits, 6 bits
  uint8_t resInfoBits;

  //   printf("Generate eNB DCI, format %d, rnti %x (pdu %p)\n",dci_format,rnti,dci_pdu);

  switch (dci_format) {

  case DCIFormatN0:
    return(-1);
    break;

  case DCIFormatN1_RAR:  // This is DLSCH allocation for control traffic

    type               = DCI_Content->DCIN1_RAR.type;
    orderIndicator     = DCI_Content->DCIN1_RAR.orderIndicator; 
    Scheddly           = DCI_Content->DCIN1_RAR.Scheddly; 
    ResAssign          = DCI_Content->DCIN1_RAR.ResAssign; 
    mcs                = DCI_Content->DCIN1_RAR.mcs; 
    RepNum             = DCI_Content->DCIN1_RAR.RepNum; 
    Reserved           = DCI_Content->DCIN1_RAR.Reserved; 
    DCIRep             = DCI_Content->DCIN1_RAR.DCIRep;
    
    /*Packed DCI here*/
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->type           =type;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->orderIndicator =orderIndicator;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->Scheddly       =Scheddly;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->ResAssign      =ResAssign;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->mcs            =mcs;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->RepNum         =RepNum;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->DCIRep         =DCIRep;
    ((DCIN1_RAR_t *)DLSCH_DCI_NB)->Reserved       =Reserved;

    DCI_pdu->Num_dci = Num_dci;

    NB_add_dci(DCI_pdu,DLSCH_DCI_NB,rnti,sizeof(DCIN1_RAR_t),aggregation,sizeof_DCIN1_RAR_t,DCIFormatN1_RAR);


    // use this value to configure PHY both harq_processes and resource mapping.



  case DCIFormatN1: // for user data

    type               = DCI_Content->DCIN1.type;
    orderIndicator     = DCI_Content->DCIN1.orderIndicator; 
    Scheddly           = DCI_Content->DCIN1.Scheddly; 
    ResAssign          = DCI_Content->DCIN1.ResAssign; 
    mcs                = DCI_Content->DCIN1.mcs; 
    RepNum             = DCI_Content->DCIN1.RepNum; 
    ndi                = DCI_Content->DCIN1.ndi;
    HARQackRes         = DCI_Content->DCIN1.HARQackRes; 
    DCIRep             = DCI_Content->DCIN1.DCIRep;
    
    /*Packed DCI here*/
    ((DCIN1_t *)DLSCH_DCI_NB)->type           =type;
    ((DCIN1_t *)DLSCH_DCI_NB)->orderIndicator =orderIndicator;
    ((DCIN1_t *)DLSCH_DCI_NB)->Scheddly       =Scheddly;
    ((DCIN1_t *)DLSCH_DCI_NB)->ResAssign      =ResAssign;
    ((DCIN1_t *)DLSCH_DCI_NB)->mcs            =mcs;
    ((DCIN1_t *)DLSCH_DCI_NB)->RepNum         =RepNum;
    ((DCIN1_t *)DLSCH_DCI_NB)->ndi            =ndi;
    ((DCIN1_t *)DLSCH_DCI_NB)->HARQackRes     =HARQackRes;
    ((DCIN1_t *)DLSCH_DCI_NB)->DCIRep         =DCIRep;

    DCI_pdu->Num_dci = Num_dci;

    NB_add_dci(DCI_pdu,DLSCH_DCI_NB,rnti,sizeof(DCIN1_t),aggregation,sizeof_DCIN1_t,DCIFormatN1);

    // use this value to configure PHY both harq_processes and resource mapping.

  case DCIFormatN2_Ind: 

    type               = DCI_Content->DCIN2_Ind.type;
    directIndInf       = DCI_Content->DCIN2_Ind.directIndInf; 
    resInfoBits        = DCI_Content->DCIN2_Ind.resInfoBits; 

    /*Packed DCI here*/
    ((DCIN2_Ind_t *)DLSCH_DCI_NB)->type           =type;
    ((DCIN2_Ind_t *)DLSCH_DCI_NB)->directIndInf   =directIndInf;
    ((DCIN2_Ind_t *)DLSCH_DCI_NB)->resInfoBits    =resInfoBits;

    DCI_pdu->Num_dci = Num_dci;

    NB_add_dci(DCI_pdu,DLSCH_DCI_NB,rnti,sizeof(DCIN2_Ind_t),aggregation,sizeof_DCIN2_Ind_t,DCIFormatN2_Ind);

    // use this value to configure PHY both harq_processes and resource mapping.

    
  case DCIFormatN2_Pag:

    type               = DCI_Content->DCIN2_Pag.type;
    ResAssign          = DCI_Content->DCIN2_Pag.ResAssign; 
    mcs                = DCI_Content->DCIN2_Pag.mcs; 
    RepNum             = DCI_Content->DCIN2_Pag.RepNum; 
    DCIRep             = DCI_Content->DCIN2_Pag.DCIRep; 

    /*Packed DCI here*/
    ((DCIN2_Pag_t *)DLSCH_DCI_NB)->type      =type;
    ((DCIN2_Pag_t *)DLSCH_DCI_NB)->ResAssign =ResAssign;
    ((DCIN2_Pag_t *)DLSCH_DCI_NB)->mcs       =mcs;
    ((DCIN2_Pag_t *)DLSCH_DCI_NB)->RepNum    =RepNum;
    ((DCIN2_Pag_t *)DLSCH_DCI_NB)->DCIRep    =DCIRep;

    DCI_pdu->Num_dci = Num_dci;

    NB_add_dci(DCI_pdu,DLSCH_DCI_NB,rnti,sizeof(DCIN2_Pag_t),aggregation,sizeof_DCIN2_Pag_t,DCIFormatN2_Pag);

    // use this value to configure PHY both harq_processes and resource mapping.


  default:
    LOG_E(PHY,"Unknown DCI format\n");
    return(-1);
    break;
  }




  // compute DL power control parameters



  return(0);
}