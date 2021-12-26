#include <common.h>
#include <exports.h>
#include <orion_tuner.h>

int main()
{
	int err_code = 0;
	TUNER_PARAMS_S tuner_param;
	TUNER_STATUS_E tuner_status = 0;

	printf("Into main. \n");

	err_code = CSTUNER_Open(TUNER_DVBS);
	if(err_code < 0)
	{
		printf("Error: CSTUNER_Init.\n");
		return err_code;
	}	
	printf("1 success CSTUNER_Open. \n");

	err_code = CSTUNER_Init();
	if(err_code < 0)
	{
		printf("Error: CSTUNER_Init.\n");
		return err_code;
	}
	printf("1 success CSTUNER_Init. \n");

	tuner_param.frequency = 4150;
	tuner_param.symbol_rate = 25000;
	tuner_param.LnbFrequency = 5150;
	tuner_param.Polarization = 1;
	tuner_param.FECRates = TUNER_FEC_NONE;
	err_code = CSTUNER_SetFrequency(&tuner_param);
	if(err_code < 0) {
		printf("Error: CSTUNER_SetFrequency.\n");
		return err_code;
	}	

	printf("1 success CSTUNER_SetFrequency. OK . \n");

	int cnt = 0;
	while(cnt++ < 5) {
		CSTUNER_ReadStatus(&tuner_status);
		udelay(2000000);
	}

	printf("Out of Main, Success!\n");

	return 0;
}
