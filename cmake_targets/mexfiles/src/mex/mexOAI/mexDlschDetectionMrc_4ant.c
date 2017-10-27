#include "mex.h"
#include "PHY/defs.h"
//#include "PHY/vars.h"
/*==============================================================================
* dlsch_dual_stream_correlation.c
*
* Returns the correlation coefficient Heff1'Heff0.
*
* example: rho = dlsch_dual_stream_correlation(Heff0,Heff1,simparms,slot);
*
* Author: Sebastian Wagner
* Date: 24-07-2012
*
===============================================================================*/

/*==============================================================================
* dlsch_detection_mrc.c
*
* Combines the parameters of both receive antennas.
*
* example: [ymf heff hmag hmagb] = dlsch_detection_mrc(ymf0,ymf1,hmag0,hmag1,hamgb0,hmagb1,rho10,struct,slot)
*
* Author: Sebastian Wagner
* Date: 23-07-2012
*
===============================================================================*/

//#define DEBUG_DETECTION_MRC

void mexDlschDetectionMrc( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[]
                )
{
  /* Declare */
  int *ymf0, *ymf1, *Hmag0, *Hmag1, *Hmag0b, *Hmag1b, *rho10;
  int *ymf0_out, *ymf1_out, *Hmag0_out, *Hmag1_out, *Hmag0b_out, *Hmag1b_out, *rho10_out;
  int *ymf0_cpy, *ymf1_cpy, *Hmag0_cpy, *Hmag1_cpy, *Hmag0b_cpy, *Hmag1b_cpy, *rho10_cpy;
  int *rxdataF_comp[NB_ANTENNAS_RX*NB_ANTENNAS_TX], *rxdataF_comp_i[NB_ANTENNAS_RX*NB_ANTENNAS_TX], *rho_i[NB_ANTENNAS_RX*NB_ANTENNAS_TX];
  int *dl_ch_mag[NB_ANTENNAS_RX*NB_ANTENNAS_TX], *dl_ch_mag_i[NB_ANTENNAS_RX*NB_ANTENNAS_TX], *dl_ch_magb[NB_ANTENNAS_RX*NB_ANTENNAS_TX], *dl_ch_magb_i[NB_ANTENNAS_RX*NB_ANTENNAS_TX];
  int i, ii;
  mxArray *tmp;
  PHY_VARS_UE *ue;

  unsigned int nb_re_per_symbol, nb_re_per_frame=4200;
  unsigned char symbol, dual_stream_UE;

  /* Check proper input and output. */
  if(nrhs!=9)
    mexErrMsgTxt("9 inputs required.");
  else if(nlhs > 7)
    mexErrMsgTxt("Too many output arguments.");
  else if(!mxIsStruct(prhs[7]))
    mexErrMsgTxt("8th input must be a structure.");

  if(!mxIsInt16(prhs[0]))
    mexErrMsgTxt("First argument must belong to Int16 class.");

  if(!mxIsInt16(prhs[1]))
    mexErrMsgTxt("Second argument must belong to Int16 class.");

  if(!mxIsInt16(prhs[2]))
    mexErrMsgTxt("Third argument must belong to Int16 class.");

  if(!mxIsInt16(prhs[3]))
    mexErrMsgTxt("Fourth argument must belong to Int16 class.");

  if(!mxIsInt16(prhs[4]))
    mexErrMsgTxt("Fifth argument must belong to Int16 class.");

  if(!mxIsInt16(prhs[5]))
    mexErrMsgTxt("Sixth argument must belong to Int16 class.");

  if(!mxIsInt16(prhs[6]))
    mexErrMsgTxt("Seventh argument must belong to Int16 class.");

  ue = mxCalloc(1,sizeof(PHY_VARS_UE));

  /* Allocate input */
  ymf0 = (int*) mxGetData(prhs[0]);
  Hmag0 = (int*) mxGetData(prhs[2]);
  Hmag1 = (int*) mxGetData(prhs[3]);
  Hmag0b = (int*) mxGetData(prhs[4]);
  Hmag1b = (int*) mxGetData(prhs[5]);
  rho10 = (int*) mxGetData(prhs[6]);
  symbol = (unsigned char) mxGetScalar(prhs[8]);


  tmp = mxGetField(prhs[7],0,"nb_re_per_frame");
  if (tmp == NULL) {
    mexErrMsgTxt("Non-existing field 'nb_re_per_frame' in input argument 8.");
  } else {
    nb_re_per_frame = (unsigned int) mxGetScalar(tmp);
  }

  tmp = mxGetField(prhs[7],0,"nb_rb");
  if (tmp == NULL) {
    mexErrMsgTxt("Non-existing field 'nb_rb' in input argument 8.");
  } else {
    ue->frame_parms.N_RB_DL = (unsigned char) mxGetScalar(tmp);
    }
  tmp = mxGetField(prhs[7],0,"nb_antennas_rx");
  if (tmp == NULL) {
    mexErrMsgTxt("Non-existing field 'nb_antennas_rx' in input argument 8.");
  } else {
    ue->frame_parms.nb_antennas_rx = (unsigned char) mxGetScalar(tmp);
    }
  tmp = mxGetField(prhs[7],0,"nb_antennas_tx_eNB");
  if (tmp == NULL) {
    mexErrMsgTxt("Non-existing field 'nb_antennas_tx_eNB' in input argument 8.");
  } else {
    ue->frame_parms.nb_antennas_tx = (unsigned char) mxGetScalar(tmp);
    }

  nb_re_per_symbol = ue->frame_parms.N_RB_DL*12;

  if (ue->frame_parms.nb_antennas_rx>1)
    dual_stream_UE = 1;

  // Make copies of inputs to avoid override
  ymf0_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));
  ymf1_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));
  Hmag0_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));
  Hmag1_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));
  Hmag0b_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));
  Hmag1b_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));
  rho10_cpy = (int*) mxCalloc(nb_re_per_frame*ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, sizeof(int));

  memcpy(ymf0_cpy,ymf0,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2);
  memcpy(ymf1_cpy,ymf1,(ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2));
  memcpy(Hmag0_cpy,Hmag0,(ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2));
  memcpy(Hmag1_cpy,Hmag1,(ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2));
  memcpy(Hmag0b_cpy,Hmag0b,(ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2));
  memcpy(Hmag1b_cpy,Hmag1b,(ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2));
  memcpy(rho10_cpy,rho10,(ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx*nb_re_per_frame<<2));

  // Assign correct inputs to algo
  rxdataF_comp[0] = ymf0_cpy;
  rho_i[0] = rho10_cpy;
  dl_ch_mag[0] = Hmag0_cpy;
  dl_ch_mag_i[0] = Hmag1_cpy;

  for (ii=1; ii<ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx; ii++ ){
    rxdataF_comp[ii] = &ymf0_cpy[ii*nb_re_per_frame];
    rho_i[ii] = &rho10_cpy[ii*nb_re_per_frame];
    dl_ch_mag[ii] = &Hmag0_cpy[ii*nb_re_per_frame];
    dl_ch_magb[ii] = &Hmag0b_cpy[ii*nb_re_per_frame];
  }

  /* Allocate Output */
  plhs[0] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  ymf0_out = (int*) mxGetPr(plhs[0]);

  plhs[1] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  ymf1_out = (int*) mxGetPr(plhs[1]);

  plhs[2] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  Hmag0_out = (int*) mxGetPr(plhs[2]);

  plhs[3] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  Hmag0b_out = (int*) mxGetPr(plhs[3]);

  plhs[4] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  Hmag1_out = (int*) mxGetPr(plhs[4]);

  plhs[5] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  Hmag1b_out = (int*) mxGetPr(plhs[5]);

  plhs[6] = mxCreateNumericMatrix(2*nb_re_per_symbol,ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx, mxINT16_CLASS, mxREAL);
  rho10_out = (int*) mxGetPr(plhs[6]);

#ifdef DEBUG_DETECTION_MRC
    mexPrintf("Got until here\n");
#endif


    /* Algo */
  dlsch_detection_mrc(&ue->frame_parms,
                      rxdataF_comp,
                      rxdataF_comp_i,
                      NULL,
                      rho_i,
                      dl_ch_mag,
                      dl_ch_magb,
                      dl_ch_mag_i,
                      dl_ch_magb_i,
                      symbol,
                      ue->frame_parms.N_RB_DL,
                      dual_stream_UE);

#ifdef DEBUG_DETECTION_MRC
    mexPrintf("Got until here too\n");
#endif

  // memcpy copies bytes!
  memcpy(ymf0_out,&rxdataF_comp[0][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
  memcpy(ymf1_out,&rxdataF_comp_i[0][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
  memcpy(Hmag0b_out,&dl_ch_magb[0][symbol*ue->frame_parms.N_RB_DL*12], nb_re_per_symbol<<2);
  memcpy(Hmag1_out,&dl_ch_mag_i[0][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
  memcpy(Hmag1b_out, &dl_ch_magb_i[0][symbol*ue->frame_parms.N_RB_DL*12], nb_re_per_symbol<<2);
  memcpy(rho10_out, &rho_i[0][symbol*ue->frame_parms.N_RB_DL*12], nb_re_per_symbol<<2);

  for (ii=1; ii<ue->frame_parms.nb_antennas_rx*ue->frame_parms.nb_antennas_tx; ii++ ){
    memcpy(&ymf0_out[ii*nb_re_per_symbol],&rxdataF_comp[ii][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
    memcpy(&ymf1_out[ii*nb_re_per_symbol],&rxdataF_comp_i[ii][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
    memcpy(&Hmag0_out[ii*nb_re_per_symbol],&dl_ch_mag[ii][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
    memcpy(&Hmag1_out[ii*nb_re_per_symbol],&dl_ch_mag_i[ii][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
    memcpy(&rho10_out[ii*nb_re_per_symbol],&rho_i[ii][symbol*ue->frame_parms.N_RB_DL*12],nb_re_per_symbol<<2);
  }



  /* free */
  mxFree(ue);
  mxFree(ymf0_cpy);
  mxFree(ymf1_cpy);
  mxFree(Hmag0_cpy);
  mxFree(Hmag1_cpy);
  mxFree(Hmag0b_cpy);
  mxFree(Hmag1b_cpy);
  mxFree(rho10_cpy);
}
