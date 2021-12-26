#ifndef  _CVE5_H_
#define  _CVE5_H_

int InitCVE5( int DispFormat );
int InitCVE5Raw( int DispFormat , int EnableColorBar);
int TestCVE5( int IsDACYPbPr );
void TestColorBar(void);
int SetVideoDACOutPutMode( int IsDACYPbPr  );
#endif
