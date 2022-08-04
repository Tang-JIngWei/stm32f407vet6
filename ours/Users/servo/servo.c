#include "servo.h"

float servo_angle = 0;

/**
 * @brief  ���ת�����
 * @param  channel ͨ��, angle �Ƕ�(+-)
 * @retval ��
 */
void Servo_Move(servo_code code, float angle)
{
	uint16_t pwm;

	/* �޷� */
	if (angle > 180)
		angle = 180;
	if (angle < 0)
		angle = 0;

	if (code == down_servo) /* �¶�� ���� */
	{
		if (angle < 0)
		{
			pwm = (-1) * angle * 11.11 + 500; /* �ǶȻ����pwm */
		}
		else
		{
			pwm = angle * 11.11 + 500; /* �ǶȻ����pwm */
		}
		
		__HAL_TIM_SetCompare(&Down_Servo_Interrrupt, Down_Servo_Channel, pwm);
	}

	else if (code == up_servo) /* �϶�� ���� */
	{
		if (angle < 0)
		{
			pwm = (-1) * angle * 11.11 + 500; /* �ǶȻ����pwm */
		}
		else
		{
			pwm = angle * 11.11 + 500; /* �ǶȻ����pwm */
		}
		
		__HAL_TIM_SetCompare(&Up_Servo_Interrrupt, Up_Servo_Channel, pwm);
	}
}

/**
 * @brief  ���¶����λ
 * @param  ��
 * @retval ��
 */
void Servos_Relocate()
{
	/* pwm�ź� */
}

/**
 * @brief  ���¶��ʹ��
 * @param  ��
 * @retval ��
 */
void Servos_Enable()
{
	HAL_TIM_PWM_Start(&Up_Servo_Interrrupt, Up_Servo_Channel);
	HAL_TIM_PWM_Start(&Down_Servo_Interrrupt, Down_Servo_Channel);
}
