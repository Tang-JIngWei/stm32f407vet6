#include "callback_tim.h"


/**
 * @brief  ��ʱ�������¼��ص�����
 * @param  ��
 * @retval ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* ������� TIM6 20ms���*/
	if (htim->Instance == TIM6)
	{
		/* �ɻ�õ�ǰ�ٶ�once �� �ۼ�����total */
		GetMotorPulse();
		
		/* ѭ����pwm���� ����*/
		Gray_Search();
		
		/* Openmv ѭ������ */
		//Openmv_Search();
		
		/* ���������� */
		now_car_yaw = Mpu_Read_Yaw();
		
		/* ֱ�� */
		if (Line_flag == 1)
		{
			/*  ʹ��״̬�²ſ���  */
			if (left_en_flag == 1 || right_en_flag == 1)
			{
				/* ����pid�������������pwm�ź� */
				YunSu_Pid_Control();
				
				/* ѭ������ ����ϵ��ʹ��ѭ������������󣬵��µ���ʱ���� */
				Now_Left_Pwm += (Search_Buchang * (float)(Now_Left_Pwm / 800.0));
				Now_Right_Pwm -= (Search_Buchang * (float)(Now_Left_Pwm / 800.0));
				
				Motors_Control(Now_Left_Dir, Now_Left_Pwm, Now_Right_Dir, Now_Right_Pwm);
				
				/* ����λ�����������Χ�ж� ������ͬʱ�ж�*/
//				if (
//							((Left_Total_Pulse <= (LineGo_Target_Maichong + 100)) && (Left_Total_Pulse >= (LineGo_Target_Maichong - 100))) ||
//							((Right_Total_Pulse <= (LineGo_Target_Maichong + 100)) && (Right_Total_Pulse >= (LineGo_Target_Maichong - 100)))
//					 )
//				{
//					stop_count++; //�ڶ�����
//					if (stop_count == 5)
//					{
//						Line_flag = 0;	//ֱ����ɣ���Ŀ�����
//						Stop_flag = 1;	//С������ֹͣ״̬
//						stop_count = 0; //��λ����

//						Motor_L_stop(); //С��ֹͣ��stop
//						Motor_R_stop();
//					}
//				}
				
			}
		}

		/* ת�� */
		if (Turn_flag == 1)
		{
			/*  ʹ��״̬�²ſ���  */
			if (left_en_flag == 1 || right_en_flag == 1)
			{
				float delt_yaw;	//�ǶȲ�ֵ
				delt_yaw = now_car_yaw - last_car_yaw;
				
				/* �����ǿ���ת�� */	
				if((delt_yaw <= (target_yaw+2)) && (delt_yaw >= (target_yaw-2)))
				{
					/*spin_count�����ת��ʱ��--200*10(�жϼ��)=2000ms 2s */
					spin_count++;
					if (spin_count == 2)
					{
						Turn_flag = 0;	
						spin_succeed_flag = 1;//ת�����
						Stop_flag = 1;	//ֹͣλ ��1
						spin_count = 0; //������0
	
						Motor_L_stop(); //С��ֹͣ��stop
						Motor_R_stop();
					}
				}
					
			}
		}
	}
}



