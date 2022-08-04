/*************************���� ���� ͨ��*****************************/
#ifndef BLE
#define BLE

#include "main.h"

//����----Э�鶨���������೤�ȡ��������ݳ��ȡ�������������
#define BLE_MAX_LENGTH       2
#define BLE_MAX_DATA_BYTE    1
#define BLE_ASCII 					 1

//�������ں궨��
#define BleUsart          USART2   
#define BleInterrupt      huart2   

extern uint8_t Ble_Rxstate;						  	//����״̬
extern uint8_t Ble_Rxcount;						  	//���մ���
extern uint8_t Ble_Rxbuffer;							//���ջ���
extern uint16_t Ble_Rxbuff[200];   			  //������
extern uint16_t Ble_Data[BLE_MAX_LENGTH];	//����������ʵ����


/* ������������ */
void Ble_Receive_Data(void);

/* �������ݴ��� */
void Ble_Data_Handle(void);

/* �������ݸ����� */
void SendDataToBle();

#endif

