/*************************���� ���� ͨ��*****************************/
#include "ble.h"

uint8_t Ble_Rxstate = 0;							  //����״̬
uint8_t Ble_Rxcount = 0;							  //���մ���
uint8_t Ble_Rxbuffer = 0;							  //���ջ���
uint16_t Ble_Rxbuff[200] = {0};   			  //������
uint16_t Ble_Data[BLE_MAX_LENGTH] = {0};	//����������ʵ����



void Ble_Receive_Data()
{
  static uint16_t  Ble_Data_temp = 0; //��ʱ��ż�������
	static uint8_t  Ble_Split_count = 0; //�ָ���ÿ�����ݵ�Ԫ����
		
		if(Ble_Rxstate == 0 && Ble_Rxbuffer == '#')			//֡ͷ
		{
				Ble_Rxstate = 1;	//֡��ʼ
				memset(Ble_Rxbuff, 0, sizeof(200));
		}
		else if(Ble_Rxstate == 1 && Ble_Rxbuffer == '$')		//֡β
		{	
				if(Ble_Rxcount == (BLE_MAX_LENGTH * BLE_MAX_DATA_BYTE))		//�������(MAX_LENGTH * MAX_DATA_BYTE)λ����
				{
					for(int i=0;i<BLE_MAX_LENGTH;i++)
					{
						for(int j=0;j<BLE_MAX_DATA_BYTE;j++)
						{
								#if(BLE_ASCII == 1)	//�ַ�����
								Ble_Data_temp += (Ble_Rxbuff[i * BLE_MAX_DATA_BYTE + j] - '0') * pow(10,(BLE_MAX_DATA_BYTE-1 - j));
								#else						//���ݴ���
								Ble_Data_temp += (Ble_Rxbuff[i * BLE_MAX_DATA_BYTE + j]) * pow(10,(BLE_MAX_DATA_BYTE-1 - j));
								#endif
						}
						Ble_Data[i] = Ble_Data_temp;
						Ble_Data_temp = 0;
					}
					//printf("%d %d %d %d\r\n",Ble_Data[0],Ble_Data[1],Ble_Data[2],Ble_Data[3]); //��������
					Ble_Data_Handle();
					Ble_Rxstate = 0;			//֡����		
					Ble_Rxcount = 0;					
					Ble_Rxbuffer = 0;				  
					memset(Ble_Rxbuff, 0, sizeof(200));  
				}
				else
				{
					//����ȷЭ��
				}
		}
		else if(Ble_Rxstate == 1 && Ble_Rxbuffer == ',')		//��&����Э������ӷ��������壬�㷨����ʱ����
		{
				if(Ble_Split_count != BLE_MAX_DATA_BYTE)   //ÿ�����ݶ�Ҫ��MAX_DATA_BYTEλ����ȷ
				{
					//����ȷЭ��
					Ble_Rxstate = 0;					
					Ble_Rxcount = 0;					
					Ble_Rxbuffer = 0;				  
					memset(Ble_Rxbuff, 0, sizeof(200)); 
				}
				Ble_Split_count = 0;	//���ü���
				
		}
		else if(Ble_Rxstate == 1)				//Ble_Rxstate == 1��ʾ���ڽ�������
		{
				Ble_Rxbuff[Ble_Rxcount++] = Ble_Rxbuffer; 
				Ble_Split_count++;
		}
				
		HAL_UART_Receive_IT(&BleInterrupt,(uint8_t *)&Ble_Rxbuffer,1);	//���ж�


}


/*  �ı������Ʊ���  */
void Ble_Data_Handle()
{
	
	Car_Task = Ble_Data[0]; 
	switch(Car_Task)
	{
		case 1:
			task1_dir = Ble_Data[1];
		break;
		case 2:
			task2_dir = Ble_Data[1];
		break;
	}
//	Now_Left_Dir = Ble_Data[0];
//	Now_Left_Pwm = Ble_Data[1];
//	
//	Now_Right_Dir = Ble_Data[2];
//	Now_Right_Pwm = Ble_Data[3];
	
	//Motors_Control(Now_Left_Dir, Now_Left_Pwm, Now_Right_Dir, Now_Right_Pwm);
}


/* �������ݸ����� */
void SendDataToBle()
{
	uint8_t sendBuff[6];
	
//	sprintf((char *)sendBuff, "*%d%d&", TASK, TargetNum);
	HAL_UART_Transmit(&BleInterrupt, sendBuff, sizeof(sendBuff), 1000);

}

