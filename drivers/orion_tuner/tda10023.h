#ifndef	_TUNER_FUNCTION_P10023_H_
#define	_TUNER_FUNCTION_P10023_H_

#include "orion_tuner.h"
int p10023_tuner_set_pll(unsigned int frequency);
int p10023_demodu(unsigned int frequency, unsigned int symbol_rate,
		  TUNER_QAM_MODULATION_E modulation,
		  TUNER_SPECTRAL_INVERSION_E inversion);
int p10023_tuner_lock_check(void);
int p10023_demodu_init(void);

#endif
