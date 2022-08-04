/*************************�ⲿ�ж� �ص�*****************************/
#include "callback_exti.h"

/**
 * @brief  �ⲿ�жϻص�����
 * @param  ��
 * @retval ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/*  ��紫���� �ⲿ�ж� ˫���ش���  */
	if (GPIO_Pin == GuangDian_Pin)
	{
		/* ���õ�ƽ״̬Ϊ1 �ж��������� */
		if(HAL_GPIO_ReadPin(GuangDian_GPIO_Port,GuangDian_Pin) == 1)
		{
			LoadOrNot = 1;
			RedLed_off();
			GreenLed_off();
		}
		else	//�½���
		{
			RedLed_off();
			if(LoadOrNot == 1) LoadOrNot = 0;
		}
		
	}
}


