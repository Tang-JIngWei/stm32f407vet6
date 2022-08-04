#include "capture.h"


/*

	��ʱ�����벶�� �ص�����

*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  static uint32_t capture_buf[2]={0,0}; //����������
  static uint8_t  capture_cnt=0;  		  //ѭ������
  static uint32_t high_time;    			  //�ߵ�ƽʱ�䣬����������һ�ε�ʱ�䣬us
  
  UNUSED(htim);
	//��Ҫ�����괥���źź��ȿ��������ز���
	if(TIM3== htim->Instance)
	{
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
		{
		  switch(capture_cnt)
			{
			  case 0://���������أ���¼������ǰʱ��
				capture_buf[0]= __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_3);
			    __HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_3,TIM_INPUTCHANNELPOLARITY_FALLING); //���������½���
				  capture_cnt++;
			 break;
			 case 1://�����½��أ���¼�´˿�ʱ��
				  capture_buf[1]= __HAL_TIM_GET_COMPARE(htim,TIM_CHANNEL_3); 
				  capture_cnt=0;
				  HAL_TIM_IC_Stop_IT(htim,TIM_CHANNEL_3); //ֹͣ���� 
				  high_time = capture_buf[1]- capture_buf[0];
//				  if(capture_buf[1]>capture_buf[0])
//						printf("DIST = %.1fCM\r\n",17.0*high_time/1000); 
		     break;				 
		     default: break;
			}		
			
			HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);  //�������벶���ж�
		}	
				
		
	}	
}




