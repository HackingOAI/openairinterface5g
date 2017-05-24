/***********************************************************************

**********************************************************************/
/*! \file PHY/LTE_REFSIG/primary_synch_NB_IoT.c
* \Narrowband Primary Synchronisation Signal(NPSS) for NB-IoT,  TS 36-212, V13.4.0 2017-02
* \author M. KANJ
* \date 2017
* \version 0.0
* \company bcom
* \email: matthieu.kanj@b-com.com
* \note
* \warning
*/

// 12x11= 132 RE ( x 2= 264 since Re,Img).
// For the In-band case: the RE dedicated to LTE pilots are supposed to overwrite 16 REs of the NPSS signal.

short primary_synch_NB_IoT[264] = {32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,-32767,0,31439,9231,21457,24763,4663,-32434,31439,-9232,-13612,-29806,31439,-9232,4663,-32434,21457,24763,31439,9231,-32767,0,0,0,-32767,0,31439,9231,21457,24763,4663,-32434,31439,-9232,-13612,-29806,31439,-9232,4663,-32434,21457,24763,31439,9231,-32767,0,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0,-32767,0,31439,9231,21457,24763,4663,-32434,31439,-9232,-13612,-29806,31439,-9232,4663,-32434,21457,24763,31439,9231,-32767,0,0,0,32767,0,-31440,-9232,-21458,-24764,-4664,32433,-31440,9231,13611,29805,-31440,9231,-4664,32433,-21458,-24764,-31440,-9232,32767,-1,0,0};
