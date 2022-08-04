#include "openmv.h"

uint8_t  Openmv_Rxstate;							      //����״̬
uint8_t  Openmv_Rxcount;							      //���մ���
uint8_t  Openmv_Rxbuffer;						        //���ջ���
uint16_t  Openmv_Rxbuff[200];   		        //������
uint16_t  Openmv_Data[OPENMV_MAX_LENGTH];	  //����������ʵ����

/* ѭ������ */
uint8_t err_dir = 0;
uint8_t err_dir2 = 0;
uint8_t err_data_angel = 0;
uint8_t err_data_dis = 0;
         

/* openmv ����  */
void Openmv_Recive_Data()
{
	//printf("%c",Openmv_Rxbuffer);
  static uint16_t  Openmv_Data_temp = 0; //��ʱ��ż�������
	static uint8_t  Openmv_Split_count = 0; //�ָ���ÿ�����ݵ�Ԫ����
		
		if(Openmv_Rxstate == 0 && Openmv_Rxbuffer == '#')			//֡ͷ
		{
			//printf("��ʼ\r\n");
				Openmv_Rxstate = 1;	//֡��ʼ
				memset(Openmv_Rxbuff, 0, sizeof(200));
		}
		else if(Openmv_Rxstate == 1 && Openmv_Rxbuffer == '$')		//֡β
		{	
				if(Openmv_Rxcount == (OPENMV_MAX_LENGTH * OPENMV_MAX_DATA_BYTE))		//�������(MAX_LENGTH * MAX_DATA_BYTE)λ����
				{
					for(int i=0;i<OPENMV_MAX_LENGTH;i++)
					{
						for(int j=0;j<OPENMV_MAX_DATA_BYTE;j++)
						{
								//printf("%c\r\n",Computer_Rxbuff[i*4+j]); //����
								#if(OPENMV_ASCII == 1)	//�ַ�����
								Openmv_Data_temp += (Openmv_Rxbuff[i * OPENMV_MAX_DATA_BYTE + j] - '0') * pow(10,(OPENMV_MAX_DATA_BYTE-1 - j));
								#else										//���ݴ���
								Openmv_Data_temp += (Openmv_Rxbuff[i * OPENMV_MAX_DATA_BYTE + j]) * pow(10,(OPENMV_MAX_DATA_BYTE-1 - j));
								#endif
						}
						Openmv_Data[i] = Openmv_Data_temp;
						Openmv_Data_temp = 0;
					}
					//printf("%d %d %d %d\r\n",Openmv_Data[0],Openmv_Data[1],Openmv_Data[2],Openmv_Data[3]); //��������
					Openmv_Data_Handle();
					Openmv_Rxstate = 0;			//֡����		
					Openmv_Rxcount = 0;					
					Openmv_Rxbuffer = 0;				  
					memset(Openmv_Rxbuff, 0, sizeof(200));  
				}
				else
				{
					//printf("����\r\n");
					//����ȷЭ��
				}
		}
		else if(Openmv_Rxstate == 1 && Openmv_Rxbuffer == ',')		//��&����Э������ӷ��������壬�㷨����ʱ����
		{
				if(Openmv_Split_count != OPENMV_MAX_DATA_BYTE)   //ÿ�����ݶ�Ҫ��MAX_DATA_BYTEλ����ȷ
				{
					//����ȷЭ��
					Openmv_Rxstate = 0;					
					Openmv_Rxcount = 0;					
					Openmv_Rxbuffer = 0;				  
					memset(Openmv_Rxbuff, 0, sizeof(200)); 
				}
				Openmv_Split_count = 0;	//���ü���
				
		}
		else if(Openmv_Rxstate == 1)				//Openmv_Rxstate == 1��ʾ���ڽ�������
		{
				Openmv_Rxbuff[Openmv_Rxcount++] = Openmv_Rxbuffer; 
				Openmv_Split_count++;
		}
				
		HAL_UART_Receive_IT(&OpenmvInterrupt,(uint8_t *)&Openmv_Rxbuffer,1);	//���ж�


}

/* 
	֡��ʽ��

	#__DIR__,__ERR__,$ 

*/

/* openmv �������ݴ��� */
void Openmv_Data_Handle()
{
	//printf("%d %d\r\n",Openmv_Data[0], Openmv_Data[1]);
	
	/* ���������� ����ָ�� */
	if(Openmv_Data[0] == 1)
	{
		if(Openmv_Data[1])
		{
			Line_flag = Openmv_Data[1];
		}
		if(Openmv_Data[2])
		{
			Turn_flag = Openmv_Data[2];
		}
		if(Openmv_Data[3])
		{
			Stop_flag = Openmv_Data[3];
		}
		
		Car_Task = Openmv_Data[4];
		task_dir = Openmv_Data[5];
	}
	
	/* ���������� ����� */
	else if(Openmv_Data[0] == 2)
	{
		err_dir = Openmv_Data[1]; 
		err_data_angel = Openmv_Data[2];	//ƫ���
		
		err_dir2 = Openmv_Data[3]; 
		err_data_dis = Openmv_Data[4];		//ƫ�����
		// Openmv_Data[5];
	}
	
}


/*
	֡��ʽ��

	#__TASK__,__DIR__,$ 

*/
void SendDataToOpenmv()
{
	uint8_t sendBuff[4];
	
//	sprintf((char *)sendBuff, "*%d%d&", TASK, TargetNum);
//	HAL_UART_Transmit(&OpenmvInterrupt, sendBuff, sizeof(sendBuff), 1000);
	
}


void Openmv_Search()
{
	switch(err_dir2)
	{
		case 1:	//��ƫ z
			Search_Buchang = err_data_dis * 15 * (-1);
		break;
		
		case 2:	//��ƫ y
			Search_Buchang = err_data_dis * 15;
		break;
	}
}
