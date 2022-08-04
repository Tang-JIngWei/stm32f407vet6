/*************************���� ��λ�� ͨ��*****************************/

#ifndef COMPUTER_UART_H
#define COMPUTER_UART_H


#include "main.h"

//��λ��----Э�鶨���������೤�ȡ��������ݳ��ȡ�������������
#define COMPUTER_MAX_LENGTH       4
#define COMPUTER_MAX_DATA_BYTE    4
#define COMPUTER_ASCII 			    	1

//��λ�����ں궨��
#define ComputerUsart          USART6   
#define ComputerInterrupt      huart6   

extern uint8_t Computer_Rxstate;						        //����״̬
extern uint8_t Computer_Rxcount;						        //���մ���
extern uint8_t Computer_Rxbuffer;							      //���ջ���
extern uint16_t Computer_Rxbuff[200];   			        //������
extern uint16_t Computer_Data[COMPUTER_MAX_LENGTH];	//����������ʵ����


/* ��λ��-�������� */
void Computer_Receive_Data();

/* ��λ��-���ݴ��� */
void Computer_Data_Handle();

#endif

