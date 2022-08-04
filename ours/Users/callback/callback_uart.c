#include "callback_uart.h"

/**
 * @description: ���ڽ����жϺ���
	Э��˵����
	<1>
		��λ-----------------��λ
			#xxxx,xxxx,xxxx,xxxx,$
		֡��-----------------֡β	
	<2>
		xxxx:  x��ʾһ���ֽڳ��ȵ����ݣ�xxxx��ʾ�����ĸ��ֽ�
	 ��,��:  ��ʾ�ָ���(���Զ���)��Ble_Split_count���м������ж�ÿ�������Ƿ���ȷ��
					 �������Ϊ ����,��Ϊֹ��ǰһ����������Ϊ���󣬸�֡���������ñ����������¸�֡
	<3>
		�㷨ԭ��1.�����ַ�ƴ�� --> �ַ�ת���֣�ASCII == 1��
						  2.ֱ�Ӵ������ݣ���ȻҲҪ�涨��������ݳ��Ⱥ�������ٻ��зָ�����ASCII == 0��
		�㷨�ο���http://www.51hei.com/bbs/dpj-207742-1.html
 * @param {UART_HandleTypeDef} *huart
 * @author: �ƾ�ΰ
 * 
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//printf("hello\r\n");
	if(huart->Instance == BleUsart)  //�����ж�
	{
		/* Receive_Data() ǰ�治Ҫ��printf�������Ӱ����� */
		Ble_Receive_Data();
	}
	
	if(huart->Instance == ComputerUsart)  //��λ���ж�
	{
		/* Receive_Data() ǰ�治Ҫ��printf�������Ӱ����� */
		Computer_Receive_Data();
	}
	
	if(huart->Instance == OpenmvUsart)	//Openmv�ж�
	{
		/* Receive_Data() ǰ�治Ҫ��printf�������Ӱ����� */
		Openmv_Recive_Data();
	}

}





