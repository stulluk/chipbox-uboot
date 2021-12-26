#ifndef _CLOCK_H_
#define _CLOCK_H_

int InitClkReg();
int SetTVEncoderClk(int IsHD, int IsProgressive);
int SetVideoDACOutPutMode(int IsYPbPr);
int SetVideoDACMux(int IsYPbPr);
int SetDDRPLLClk(float Clk);

#endif
