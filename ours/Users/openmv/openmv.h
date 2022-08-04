#ifndef OPENMV
#define OPENMV


#include "main.h"


//Э�鶨���������೤�ȡ��������ݳ��ȡ�������������
#define OPENMV_MAX_LENGTH          6
#define OPENMV_MAX_DATA_BYTE       2
#define OPENMV_ASCII 				       1

//openmv���ں궨��
#define OpenmvUsart      USART1   
#define OpenmvInterrupt  huart1   

extern uint8_t  Openmv_Rxstate;							      //����״̬
extern uint8_t  Openmv_Rxcount;							      //���մ���
extern uint8_t  Openmv_Rxbuffer;						      //���ջ���
extern uint16_t  Openmv_Rxbuff[200];   		        //������
extern uint16_t  Openmv_Data[OPENMV_MAX_LENGTH];	  //����������ʵ����

/* ѭ������ */
extern uint8_t err_dir;
extern uint8_t err_dir2;
extern uint8_t err_data_angel;
extern uint8_t err_data_dis;

/* openmv ����  */
void Openmv_Recive_Data(void);

/* openmv �������ݴ��� */
void Openmv_Data_Handle(void);

/* openmv ѭ�� */
void Openmv_Search(void);


#endif



