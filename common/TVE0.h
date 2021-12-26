#ifndef  _TVE0_H_
#define  _TVE0_H_
int InitTVE0( int DispFormat );
int InitTVE0Raw( int DispFormat , int EnableColorBar);
int TVE0SetClk( int DispFormat , int EnableColorBar);
int TestTVE0( int IsDACYPbPr );
void TestColorBar_TVE0(void);
int SetVideoDACOutPutMode_TVE0( int IsDACYPbPr  );
#endif
