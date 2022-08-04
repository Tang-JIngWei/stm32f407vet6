/*************************���� ��λ�� ͨ��*****************************/
#include "computer_uart.h"

uint8_t Computer_Rxstate = 0;							        //����״̬
uint8_t Computer_Rxcount = 0;							        //���մ���
uint8_t Computer_Rxbuffer = 0;							      //���ջ���
uint16_t Computer_Rxbuff[200] = {0};   			      //������
uint16_t Computer_Data[COMPUTER_MAX_LENGTH] = {0};	//����������ʵ����


/* ��λ������ */
void Computer_Receive_Data()
{
		static uint16_t Computer_Data_temp = 0; //��ʱ��ż�������
		static uint8_t Computer_Split_count = 0; //�ָ���ÿ�����ݵ�Ԫ����
		
		if(Computer_Rxstate == 0 && Computer_Rxbuffer == '#')			//֡ͷ
		{
				Computer_Rxstate = 1;	//֡��ʼ
				memset(Computer_Rxbuff, 0, sizeof(200));
		}
		else if(Computer_Rxstate == 1 && Computer_Rxbuffer == '$')		//֡β
		{	
				if(Computer_Rxcount == (COMPUTER_MAX_LENGTH * COMPUTER_MAX_DATA_BYTE))		//�������(MAX_LENGTH * MAX_DATA_BYTE)λ����
				{
					for(int i=0;i<COMPUTER_MAX_LENGTH;i++)
					{
						for(int j=0;j<COMPUTER_MAX_DATA_BYTE;j++)
						{
              	//printf("%c\r\n",Computer_Rxbuff[i*4+j]); //����
								#if(COMPUTER_ASCII == 1)	//�ַ�����
								Computer_Data_temp += (Computer_Rxbuff[i * COMPUTER_MAX_DATA_BYTE + j] - '0') * pow(10,(COMPUTER_MAX_DATA_BYTE-1 - j));
								#else						//���ݴ���
								Computer_Data_temp += (Computer_Rxbuff[i * COMPUTER_MAX_DATA_BYTE + j]) * pow(10,(COMPUTER_MAX_DATA_BYTE-1 - j));
								#endif
						}
						Computer_Data[i] = Computer_Data_temp;
						Computer_Data_temp = 0;
					}
					//printf("%d %d %d %d\r\n",Computer_Data[0],Computer_Data[1],Computer_Data[2],Computer_Data[3]); //��������
					Computer_Data_Handle();
					Computer_Rxstate = 0;			//֡����		
					Computer_Rxcount = 0;					
					Computer_Rxbuffer = 0;				  
					memset(Computer_Rxbuff, 0, sizeof(200));  
				}
				else
				{
					//����ȷЭ��
				}
		}
		else if(Computer_Rxstate == 1 && Computer_Rxbuffer == ',')		//��&����Э������ӷ��������壬�㷨����ʱ����
		{
				if(Computer_Split_count != COMPUTER_MAX_DATA_BYTE)   //ÿ�����ݶ�Ҫ��MAX_DATA_BYTEλ����ȷ
				{
					//����ȷЭ��
					Computer_Rxstate = 0;					
					Computer_Rxcount = 0;					
					Computer_Rxbuffer = 0;				  
					memset(Computer_Rxbuff, 0, sizeof(200)); 
				}
				Computer_Split_count = 0;	//���ü���
				
		}
		else if(Computer_Rxstate == 1)				//Ble_Rxstate == 1��ʾ���ڽ�������
		{
				Computer_Rxbuff[Computer_Rxcount++] = Computer_Rxbuffer; 
				Computer_Split_count++;
		}
				
		HAL_UART_Receive_IT(&ComputerInterrupt,(uint8_t *)&Computer_Rxbuffer,1);	//���ж�
}




/*  �ı������Ʊ���  */
void Computer_Data_Handle()
{
	/* ��λ�����Ե�� */
//	Now_Left_Dir = Computer_Data[0];
//	Now_Left_Pwm = Computer_Data[1];
//	Now_Right_Dir = Computer_Data[2];
//	Now_Right_Pwm = Computer_Data[3];
//	printf("%d %d %d %d\r\n",Now_Left_Dir,Now_Left_Pwm,Now_Right_Dir,Now_Right_Pwm);
//	Motors_Control(Now_Left_Dir, Now_Left_Pwm, Now_Right_Dir, Now_Right_Pwm);
	
	/* ��λ�����Զ�� */
	int pwm = Computer_Data[0];
	__HAL_TIM_SetCompare(&Up_Servo_Interrrupt, Up_Servo_Channel, pwm);

//	pid_left_speed.Kp =	(float)Computer_Data[0] / 100.0;
//	pid_left_speed.Ki = (float)Computer_Data[1] / 100.0;
//	pid_right_speed.Kp = (float)Computer_Data[2] / 100.0;
//  pid_right_speed.Ki =(float)Computer_Data[3] / 100.0;
//	
//	printf("%f %f %f %f\r\n",pid_left_speed.Kp,pid_left_speed.Ki,pid_right_speed.Kp,pid_right_speed.Ki);
	//PID_param_init();
}



