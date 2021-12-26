#include <iostream>

using namespace std;

/* SMIC .13um PLL rules:
    CLK_OUT = XIN / ( 1 << OD ) * (N / M)

    (1) M: 8bit, >= 2
    (2) N: 5bit, >= 2
    (3) OD: 2bit
    (4) 1MHZ <= (XIN / N) <= 25MHZ
    (5) 200MHZ <= (CLK_OUT * (1 << OD)) <= 1000MHZ
*/

static double calc_pll_out(double xin, unsigned int od, unsigned int m, unsigned int n)
{
    double no;
    double clk_out;

    if(od >= 4 || m < 2 || m >= 256 || n < 2 || n >= 32)
        return 0;

    if((xin / (double)n) < 1.0 || (xin / (double)n) > 25.0)
        return 0;

    no = (double)(1 << od);
    
    clk_out = xin / no * (double)m / (double)n;

    if(clk_out * no < 200.0 || clk_out * no > 1000.0)
        return 0;
    else
        return clk_out;
}

int main()
{
    double XIN;
    double CLK_MIN;
    double CLK_MAX;
    double CLK_OUT;

    unsigned int OD;
    unsigned int M;
    unsigned int N;

    cout << "Input XIN (27.0):";
    cin >> XIN;

    cout << "Input CLK_MIN:";
    cin >> CLK_MIN;
    
    cout << "Input CLK_MAX:";
    cin >> CLK_MAX;    


    for(OD = 0; OD < 4; OD++)
        for(M = 2; M < 256; M++)
            for(N = 2; N < 32; N++) {
                CLK_OUT = calc_pll_out(XIN, OD, M, N);
                if(CLK_OUT != 0 && CLK_OUT >= CLK_MIN && CLK_OUT <= CLK_MAX)
                    printf("CLK_OUT: %03.3f OD:%1d M:%3d N:%2d ID_SCS_VALUE: 0x0000%04x\n",
                            CLK_OUT, OD, M, N, (OD << 14 | N << 9 | M));
            }

}
