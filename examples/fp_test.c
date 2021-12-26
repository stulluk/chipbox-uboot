#include <common.h>
#include <exports.h>
#include <orion_fp.h>


int main()
{
	int err_code = 0;
	unsigned char char_value, raw_value[5], key, valid, counts = 14, write_char;
	

	err_code = fp_set_led_display_pos(2);	
	err_code |= fp_set_led_dispaly_mode(FP_LEDDISP_BLINK);	
	char_value = '7';
	err_code |= fp_set_led_dispaly_char(&char_value);

	err_code |= fp_set_led_display_pos(0);	
	err_code |= fp_set_led_dispaly_mode(FP_LEDDISP_ON);
	raw_value[0] = 0;
	raw_value[1] = 0;
	raw_value[2] = 0;
	raw_value[3] = 0;
	err_code |= fp_set_led_display_raw(&raw_value, 4);
	

	err_code |= fp_enable_keyscan();

	err_code |= fp_set_keyscan_map(0, 0x00);
	err_code |= fp_set_keyscan_map(1, 0x11);
	err_code |= fp_set_keyscan_map(2, 0x22);
	err_code |= fp_set_keyscan_map(3, 0x33);
	err_code |= fp_set_keyscan_map(4, 0x44);
	err_code |= fp_set_keyscan_map(5, 0x55);
	err_code |= fp_set_keyscan_map(6, 0x66);
	err_code |= fp_set_keyscan_map(7, 0x77);
	err_code |= fp_set_keyscan_map(8, 0x88);
	err_code |= fp_set_keyscan_map(9, 0x99);
	err_code |= fp_set_keyscan_map(10, 0xaa);
	err_code |= fp_set_keyscan_map(11, 0xbb);
	err_code |= fp_set_keyscan_map(12, 0xcc);
	err_code |= fp_set_keyscan_map(13, 0xdd);
	err_code |= fp_set_keyscan_map(14, 0xee);
	err_code |= fp_set_keyscan_map(15, 0xff);

	printf("Please press key 12 times...\n");
	while(1){
		err_code |= fp_get_keyscan_input (&key, &valid);
		if(err_code != 0)
			printf("######## fp_get_keyscan_input error ##########\n");
		if(valid == 1){
			counts--;
			printf("key press, code: 0x%x\n", key);
		}
		if(counts < 3)
			break;
		udelay(200000);
	}


	err_code |= fp_enable_serial_in();
	err_code |= fp_reset_serial_in();
	err_code |= fp_set_serial_in_attr(115200, 2);
	write_char = 'A';
	err_code |= fp_write_serial_in(&write_char);
	
	
	if(err_code == 0)
		printf("\n############## no error ################\n\n");
	else
		printf("@@@@@@@@@@@@@@@  error: %d  @@@@@@@@@@@@@@@\n", err_code);

	return 0;
}

