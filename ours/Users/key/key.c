#include "key.h"

uint8_t key_count = 1;

void KeyScan()
{
	if(KEY1_SCAN() == 0)
	{
		key_count++;
		if(key_count == 2)
		{
			/* �û����뿪ʼ */
			
			RedLed_Tog();
			/* �û�������� */
			
			key_count = 0;
		}while(KEY1_SCAN() == 0);
	}
	
	else if(KEY2_SCAN() == 0)
	{
		key_count++;
		if(key_count == 2)
		{
			/* �û����뿪ʼ */
			
			BlueLed_Tog();
			/* �û����뿪ʼ */
			
			key_count = 0;
		}while(KEY2_SCAN() == 0);
	}
}







