#include "motor.h"

/* ����ı��������¼�������㵽������������uint ֻ����int */
int8_t Motor_Direction = 0;			 //���ת������
int32_t Motor_Capture_Count = 0; //��ǰ������ֵ
float Motor_Shaft_Speed = 0;		 //ת���ٶ�

long Left_Once_Pulse = 0; //�����ֵ��ζ�ȡ������������
long Right_Once_Pulse = 0;
long Left_Total_Pulse = 0; //�������ۼƻ��������
long Right_Total_Pulse = 0;

/*  ��¼ ��� ����(DIR)|�ٶ�(PWM) --ȫ�ֵ���-- */
uint8_t Now_Left_Dir = 0;
uint8_t Now_Right_Dir = 0;

uint16_t Now_Left_Pwm = 0;
uint16_t Now_Right_Pwm = 0;

float last_car_yaw = 0;	//��¼ת��ǰʱ�̵ĽǶ�
float now_car_yaw = 0;  //ת���ĵ�ǰ�Ƕ�
float target_yaw = 0; //ת��Ŀ��Ƕ�

uint8_t Line_flag = 0, Turn_flag = 0, Stop_flag = 0; //����flag
uint8_t spin_start_flag, spin_succeed_flag;					 //ת��flag
uint8_t left_en_flag = 0, right_en_flag = 0;				 //���ҵ��ʹ��flag

double Left_journey_pulse, Right_journey_pulse; //���������߾���

/* ֱ��Ŀ�������� ��������ͬ */
double LineGo_Target_Maichong ;		

/* ��ת��Ŀ�������� ������һ����һ����*/
double SpinLeft90_Target_Maichong = 2000;
double SpinRight90_Target_Maichong = 1700;
double SpinBack180_Target_Maichong = 2800;

uint16_t stop_count, spin_count; // pid����ĩβʱ����İڶ�����

/*  ��� PID ���  */
float L_Pwm_Outval, L_Location_Outval; //����pid��Ŀ�����
float R_Pwm_Outval, R_Location2_Outval;

/*********************************************/

/*************** tb6612����	���� *******************/

/**
	* @brief  ����ܿ�
	* @param  ����  ��PWM, �ҷ���, ��PWM
			����:1��STP��0��
				 2��FWD��1��
				 3��BCK��2��
	* @retval ��
	*/
void Motors_Control(motor_dir L_dir, uint16_t L_pwm, motor_dir R_dir, uint16_t R_pwm)
{
	/* ��ʱ�� periodֵ ����Ϊ 1000 */

	// pwm ��� PWM���һ��Ҫ�ȴ�
//		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
//		HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	
	/*  ����  */
	Set_Left_Dir(L_dir);
	if (L_pwm >= PWM_PERIOD_COUNT - 100) //������
	{
		L_pwm = PWM_PERIOD_COUNT - 100 ;
	}
	Set_Left_Speed(L_pwm);

	/*  ����  */
	Set_Right_Dir(R_dir);
	if (R_pwm >= PWM_PERIOD_COUNT - 100) //������
	{
		R_pwm = PWM_PERIOD_COUNT - 100 ;
	}
	Set_Right_Speed(R_pwm);
}

/**
 * @brief  ֱ�ߺ���
 * @param  ����ֱ�еľ��� cm ��Ϊ������������
 * @retval ��
 */
void Car_Go(int32_t desire_cm)
{
	int32_t Target_Maichong = 0;

	Line_flag = 1;
	Stop_flag = 0;

	spin_start_flag = 0;
	spin_succeed_flag = 0;

	/* ��Ŀ�����ת���������� */
	LineGo_Target_Maichong = (desire_cm / (4.4 * 3.14)) * (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION);
	// printf("maichong: %d\r\n",Target_Maichong);

	/*  PID ����target */
	set_pid_target(&pid_left_location, LineGo_Target_Maichong);
	set_pid_target(&pid_right_location, LineGo_Target_Maichong);

	
	Left_Enable(); //ʹ�������׼��ǰ��
	Right_Enable();

	Left_Total_Pulse = 0;
	Right_Total_Pulse = 0;
}

/**
 * @brief  ��ת��
 * @param  ת��Ƕ� left_90 right_90 back_180
 * @retval ��
 */
void Spin_Turn(spin_angle angle)
{
	Line_flag = 0; //ֱ�б�־ ��0
  Stop_flag = 0;

  Turn_flag = 1; //ת��λ ��1
  spin_succeed_flag = 0; //��ʼת�䣬�ɹ���־λ ��0

	/*
		 note��������ת�Ƕ����� ��ת�Ƕȼ�С
	*/
	
  /* �� 90 */
  if (angle == left_90)
  {
		last_car_yaw = Mpu_Read_Yaw();
    target_yaw = 12;
    Motors_Control(MOTOR_BCK, 500, MOTOR_FWD, 500);
  }
  /* �� 90 */
  else if (angle == right_90)
  {
		last_car_yaw = Mpu_Read_Yaw();
    target_yaw = -12;
    Motors_Control(MOTOR_FWD, 500, MOTOR_BCK, 500);
  }
  /* ��ͷ 180 */
  else if (angle == back_180)
  {
		last_car_yaw = Mpu_Read_Yaw();
    target_yaw = 23;
    Motors_Control(MOTOR_BCK, 500, MOTOR_FWD, 500);	//��ͷ������ת
  }
}


/**
 * @brief  ����ֱ��
 * @param  speed: �ٶȣ���λʱ����������
 * @retval ��
 */
void YunSu_GO(float speed)
{
	Line_flag = 1;
	Stop_flag = 0;
	
	set_pid_target(&pid_left_speed, speed);
	set_pid_target(&pid_right_speed, speed);
	
	Left_Enable(); //ʹ�������׼��ǰ��
	Right_Enable();

	Left_Total_Pulse = 0;
	Right_Total_Pulse = 0;
}

/**
 * @brief  ���õ���ٶ�
 * @param  x_pwm: �ٶȣ�ռ�ձȣ�
 * @retval ��
 */
void Set_Left_Speed(uint16_t L_pwm)
{
	/* �ı�compareֵ�Ըı�ռ�ձ� */
	__HAL_TIM_SetCompare(&Left_PWM_Interrupt, Left_PWM_Channel, L_pwm);
	Now_Left_Pwm = L_pwm;
}

void Set_Right_Speed(uint16_t R_pwm)
{
	/* �ı�compareֵ�Ըı�ռ�ձ� */
	__HAL_TIM_SetCompare(&Right_PWM_Interrupt, Right_PWM_Channel, R_pwm);
	Now_Right_Pwm = R_pwm;
}

/**
 * @brief  ���õ������
 * @param  x_dir
 * @retval ��
 */
void Set_Left_Dir(motor_dir L_dir)
{
	if (L_dir == MOTOR_FWD) // 1
	{
		Motor_L_forward();
		Now_Left_Dir = MOTOR_FWD;
	}
	else if (L_dir == MOTOR_BCK) // 2
	{
		Motor_L_back();
		Now_Left_Dir = MOTOR_BCK;
	}
	else // 0
	{
		Motor_L_stop();
		Now_Left_Dir = MOTOR_STP;
	}
}

void Set_Right_Dir(motor_dir R_dir)
{
	if (R_dir == MOTOR_FWD) // 1
	{
		Motor_R_forward();
		Now_Right_Dir = MOTOR_FWD;
	}
	else if (R_dir == MOTOR_BCK) // 2
	{
		Motor_R_back();
		Now_Right_Dir = MOTOR_BCK;
	}
	else // 0
	{
		Motor_R_stop();
		Now_Right_Dir = MOTOR_STP;
	}
}

/**
 * @brief  �����������
 * @param  ��
 * @retval ��
 */
void Motor_L_forward()
{
	/*  AIN1---AIN2  */
	/*   0------1  */
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_SET);
	left_en_flag = 1;
}

void Motor_L_back()
{
	/*  AIN1---AIN2  */
	/*   1------0  */
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
	left_en_flag = 1;
}

void Motor_L_stop()
{
	/*  AIN1---AIN2  */
	/*   0------0  */
	HAL_GPIO_WritePin(AIN1_GPIO_Port, AIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(AIN2_GPIO_Port, AIN2_Pin, GPIO_PIN_RESET);
	left_en_flag = 0;
}

void Motor_R_forward()
{
	/*  BIN1---BIN2  */
	/*   0------1  */
	HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
	right_en_flag = 1;
}

void Motor_R_back()
{
	/*  BIN1---BIN2  */
	/*   1------0  */
	HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_SET);
	right_en_flag = 1;
}

void Motor_R_stop()
{
	/*  BIN1---BIN2  */
	/*   0------0  */
	HAL_GPIO_WritePin(BIN1_GPIO_Port, BIN1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BIN2_GPIO_Port, BIN2_Pin, GPIO_PIN_RESET);
	//right_en_flag = 0;
}

/**
 * @brief  ����ʹ�� ��pwm�����⣩
 * @param  ��
 * @retval ��
 */
void Left_Enable()
{
	HAL_TIM_PWM_Start(&Left_PWM_Interrupt, Left_PWM_Channel);
	left_en_flag = 1;
}

void Left_Disable()
{
	HAL_TIM_PWM_Stop(&Left_PWM_Interrupt, Left_PWM_Channel);
	left_en_flag = 0;
}

/**
 * @brief  �ҵ��ʹ��
 * @param  ��
 * @retval ��
 */
void Right_Enable()
{
	HAL_TIM_PWM_Start(&Right_PWM_Interrupt, Right_PWM_Channel);
	right_en_flag = 1;
}

void Right_Disable()
{
	HAL_TIM_PWM_Stop(&Right_PWM_Interrupt, Right_PWM_Channel);
	right_en_flag = 0;
}

/**************************���ҵ���ٶ�pid����*****************************/
/**
 * @brief  pid���� ·�̻�+�ٶȻ�
 * @param  ��
 * @retval ��
 */
void Speeds_Pid_Control()
{
  float temp_pwm = 0, temp_speed = 0, now_left_speed = 0, now_right_speed = 0;

	/* ����pid */
	Left_journey_pulse = Left_Total_Pulse;
	temp_speed = Location_pid_realize(&pid_left_location,Left_journey_pulse);
	set_pid_target(&pid_left_speed,temp_speed);
	now_left_speed = (float)(Left_Once_Pulse * 1000 * 60) / (((ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) * SPEED_PID_PERIOD)) ;
	temp_pwm = speed_pid_realize(&pid_left_speed, now_left_speed);
	if (temp_pwm > 0)
	{
		Now_Left_Dir = MOTOR_FWD;
		Now_Left_Pwm = temp_pwm;
	}
	else if (temp_pwm < 0)
	{
		Now_Left_Dir = MOTOR_BCK;
		Now_Left_Pwm = -temp_pwm;
	}
	else
	{
		Now_Left_Dir = MOTOR_STP;
		Now_Left_Pwm = 0;
	}

	/* �ҵ��pid */
	Right_journey_pulse = Right_Total_Pulse;
	temp_speed = Location_pid_realize(&pid_right_location,Right_journey_pulse);
	set_pid_target(&pid_right_speed,temp_speed);
	now_right_speed = (float)(Right_Once_Pulse * 1000 * 60) / (((ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) * SPEED_PID_PERIOD)) ;
	temp_pwm = speed_pid_realize(&pid_right_speed, now_right_speed);
	if (temp_pwm > 0)
	{
		Now_Right_Dir = MOTOR_FWD;
		Now_Right_Pwm = temp_pwm;
	}
	else if (temp_pwm < 0)
	{
		Now_Right_Dir = MOTOR_BCK;
		Now_Right_Pwm = -temp_pwm;
	}
	else
	{
		Now_Right_Dir = MOTOR_STP;
		Now_Right_Pwm = 0;
	}
	/* ��ӡ�������� */
	//printf("%lf,%lf,%lf,%lf,%lf\r\n", (100 / (4.4 * 3.14)) * (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION), Left_journey_pulse, Right_journey_pulse, now_left_speed, now_right_speed);
	printf("%lf,%lf,%lf\r\n", pid_right_speed.target_val, now_left_speed, now_right_speed);
}

/**
 * @brief  pid�������� ֻ�����ٶȻ�
 * @param  ��
 * @retval ��
 */
void YunSu_Pid_Control()
{
	 float temp_pwm = 0, temp_speed = 0, now_left_speed = 0, now_right_speed = 0;

	/* ����pid */
	now_left_speed = (float)(Left_Once_Pulse * 1000 * 60) / (((ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) * SPEED_PID_PERIOD)) ;
	temp_pwm = speed_pid_realize(&pid_left_speed, now_left_speed);
	printf("%.2f,",temp_pwm);
	if (temp_pwm > 0)
	{
		Now_Left_Dir = MOTOR_FWD;
		Now_Left_Pwm = temp_pwm;
	}
	else if (temp_pwm < 0)
	{
		Now_Left_Dir = MOTOR_BCK;
		Now_Left_Pwm = -temp_pwm;
	}
	else
	{
		Now_Left_Dir = MOTOR_STP;
		Now_Left_Pwm = 0;
	}

	/* �ҵ��pid */
	now_right_speed = (float)(Right_Once_Pulse * 1000 * 60) / (((ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) * SPEED_PID_PERIOD)) ;
	temp_pwm = speed_pid_realize(&pid_right_speed, now_right_speed);
	printf("%.2f\r\n",temp_pwm);
	if (temp_pwm > 0)
	{
		Now_Right_Dir = MOTOR_FWD;
		Now_Right_Pwm = temp_pwm;
	}
	else if (temp_pwm < 0)
	{
		Now_Right_Dir = MOTOR_BCK;
		Now_Right_Pwm = -temp_pwm;
	}
	else
	{
		Now_Right_Dir = MOTOR_STP;
		Now_Right_Pwm = 0;
	}
	/* ��ӡ�������� */
	//printf("%lf,%lf,%lf,%lf,%lf\r\n", (100 / (4.4 * 3.14)) * (REDUCTION_RATIO * ENCODER_TOTAL_RESOLUTION), Left_journey_pulse, Right_journey_pulse, now_left_speed, now_right_speed);
	printf("%lf,%lf,%lf\r\n", pid_right_speed.target_val, now_left_speed, now_right_speed);

}



/*								������									*/

/*******************�����������ʼ��************************/
void Encoder_Configuration(void)
{
	/* ��������� */
	__HAL_TIM_SET_COUNTER(&Left_MOTOR_Interrupt, 0);
	__HAL_TIM_SET_COUNTER(&Right_MOTOR_Interrupt, 0);

	/* �����жϱ�־λ */
	__HAL_TIM_CLEAR_IT(&Left_MOTOR_Interrupt, TIM_IT_UPDATE);
	__HAL_TIM_CLEAR_IT(&Right_MOTOR_Interrupt, TIM_IT_UPDATE);

	/* ʹ�ܶ�ʱ���ĸ����¼��ж� */
	__HAL_TIM_ENABLE_IT(&Left_MOTOR_Interrupt, TIM_IT_UPDATE);
	__HAL_TIM_ENABLE_IT(&Right_MOTOR_Interrupt, TIM_IT_UPDATE);

	/* ���ø����¼�����ԴΪ����������� */
	__HAL_TIM_URS_ENABLE(&Left_MOTOR_Interrupt);
	__HAL_TIM_URS_ENABLE(&Right_MOTOR_Interrupt);

	/* ʹ�ܱ������ӿ� */
	HAL_TIM_Encoder_Start(&Left_MOTOR_Interrupt, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&Right_MOTOR_Interrupt, TIM_CHANNEL_ALL);
}

/*******************ʵ������ʱ��ȡ��������ֵ************************/
void GetMotorPulse(void) //��ȡ�������
{
	/*  ��  */
	/* �����ת���� = �������������� */
	int i = __HAL_TIM_IS_TIM_COUNTING_DOWN(&Left_MOTOR_Interrupt);
	Left_Once_Pulse = (short)(__HAL_TIM_GET_COUNTER(&Left_MOTOR_Interrupt)); /* ������ֵ */
	__HAL_TIM_SET_COUNTER(&Left_MOTOR_Interrupt, 0);												 /* Left_MOTOR_Interrupt��TIM���������� */
	//printf("left maichong: %d\r\n",Left_Once_Pulse);

	/*  ��  */
	/* �����ת���� = �������������� */
	int j = __HAL_TIM_IS_TIM_COUNTING_DOWN(&Right_MOTOR_Interrupt);
	Right_Once_Pulse = (short)(__HAL_TIM_GET_COUNTER(&Right_MOTOR_Interrupt)); /* ������ֵ */
	Right_Once_Pulse = -Right_Once_Pulse;
	__HAL_TIM_SET_COUNTER(&Right_MOTOR_Interrupt, 0); /* Right_MOTOR_Interrupt���������� */

	Left_Total_Pulse += Left_Once_Pulse; //�ۼ�����	���һ��pid֮��Ҫ��0
	Right_Total_Pulse += Right_Once_Pulse;

	/* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ���=(����������ֱ���/4) * ʱ��ϵ��  */
//	Motor_Shaft_Speed = (float)(Left_Once_Pulse * 1000 * 60) / (((ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) * SPEED_PID_PERIOD)) ;
//	
//	printf("�������%d\r\n", i);
//	printf("���ת�ᴦת�٣�%.2f ת/�� \r\n", Motor_Shaft_Speed);
//	printf("��������ת�٣�%.2f ת/�� \r\n\n", Motor_Shaft_Speed / 20.0); /* �����ת�� = ת��ת�� / ���ٱ� */

//	Motor_Shaft_Speed = (float)(Right_Once_Pulse * 1000 * 60) / ((ENCODER_TOTAL_RESOLUTION * REDUCTION_RATIO) * SPEED_PID_PERIOD) ;
//	
//	printf("�������%d\r\n", j);
//	printf("���ת�ᴦת�٣�%.2f ת/�� \r\n", Motor_Shaft_Speed);
//	printf("��������ת�٣�%.2f ת/�� \r\n\n", Motor_Shaft_Speed / 20.0); /* �����ת�� = ת��ת�� / ���ٱ� */

//	printf("--------------------------------\r\n");
}
