////////////////////////////////////////////////////////////////////////////////////////////////////
//File Name: TVE1.h
//Function: 1. TVE1  register address definition
//          2. DF_VIDEO_FORMAT definition
//Author  : Hanll
//Version : 1.0
//History:
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef  _TVE1_H_
#define  _TVE1_H_

int InitTVE1( int DispFormat );
int InitTVE1Raw( int DispFormat , int EnableColorBar, int IsSysPll);
int TVE1SetClk( int DispFormat , int EnableColorBar, int IsSysPll);
int TestTVE1( int IsDACYPbPr );
void TestColorBar_TVE1(void);
int SetVideoDACOutPutMode_TVE1( int IsDACYPbPr  );

void DACOutConfig(int Cfg_id);

//////////////// DACOutConfig function description ////////////////
//
//   Cfg_id = 0 --> DAC0: tve1_cvbs DAC1: tve0_r/g/b
//   Cfg_id = 1 --> DAC0: tve1_cvbs DAC1: DF_r/g/b
//   Cfg_id = 2 --> DAC0: tve1_cvbs DAC1: tve1_r/g/b
//   Cfg_id = 4 --> DAC0: tve1_r/g/b DAC1: tve0_r/g/b
//   Cfg_id = 5 --> DAC0: tve1_r/g/b DAC1: DF_r/g/b
//   Cfg_id = 6 --> DAC0: tve1_r/g/b DAC1: tve1_r/g/b
//   Cfg_id = other --> DAC0: tve1_cvbs DAC1: tve0_r/g/b
//
//////////////////////////// end ///////////////////////////

#endif
