#ifndef _BYYC_OLED_H_
#define _BYYC_OLED_H_

#ifdef __cplusplus
extern "C"
{
#endif

//#include "byyc_gpio.h"
#include "main.h"

	
#define PRI_BUF 300     // OLED��ʾ����ַ���
#define Brightness 0x0F // OLED����
#define X_WIDTH 128     // OLED���
#define Y_WIDTH 64      // OLED�߶�

// OLED�˿ڶ���
#define OLED_RST_Clr() HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_RESET); // RES����0Ӳ����λ��
#define OLED_RST_Set() HAL_GPIO_WritePin(RES_GPIO_Port, RES_Pin, GPIO_PIN_SET);   // RES

#define OLED_RS_Clr() HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET); // DC����0Ϊд���
#define OLED_RS_Set() HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET);   // DC����1Ϊд���ݣ�

#define OLED_SCLK_Clr() HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, GPIO_PIN_RESET); // D0��ʱ���ߣ���0����д�����ݣ�
#define OLED_SCLK_Set() HAL_GPIO_WritePin(D0_GPIO_Port, D0_Pin, GPIO_PIN_SET);   // D0��ʱ���ߣ���1д�����ݣ�������д�룩��

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, GPIO_PIN_RESET); // D1�������ߣ�д0��
#define OLED_SDIN_Set() HAL_GPIO_WritePin(D1_GPIO_Port, D1_Pin, GPIO_PIN_SET);   // D1�������ߣ�д1��

#define OLED_CMD 0  //д����
#define OLED_DATA 1 //д����

extern unsigned char oled_picture[8][128];

void OLED_Init(void);
void OLED_Display_On(void);                                               //����OLED��ʾ
void OLED_Display_Off(void);                                              //�ر�OLED��ʾ
void OLED_Fill(unsigned char bmp_dat);                                    // OLED��䣨ÿҳ��ÿ��������bmp_dat����λ���ϣ�1Ϊ����
void OLED_CLS(void);                                                      // OLED��λ����OLED_Fill(0x00) ��������0��
void OLED_P6x8Char(unsigned char x, unsigned char y, unsigned char ch);   //��ʾ6*8һ����׼ASCII�ַ�,��ʾ�����꣨x,y����yΪҳ��Χ0��7
void OLED_P6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);  //��ʾ6*8һ���׼ASCII�ַ���,��ʾ�����꣨x,y����yΪҳ��Χ0��7
void OLED_P8x16Char(unsigned char x, unsigned char y, unsigned char ch);  //��ʾ8*16��16<8��������Ҫ2ҳ��һ����׼ASCII�ַ�,��ʾ�����꣨x,y����yΪҳ��Χ0��7
void OLED_P8x16Str(unsigned char x, unsigned char y, unsigned char ch[]); //��ʾ8*16һ���׼ASCII�ַ�����16<8��������Ҫ2ҳ������ʾ�����꣨x,y����yΪҳ��Χ0��7
void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char n);    //��ʾ16*16������ʾ���֣�,��ʾ�����꣨x,y����yΪҳ��Χ0��7��NΪ��������
void OLED_my8x16Ch(u8 x, u8 y, u8 ch);                                    //��ʾ8*16һ���Լ��趨���֣�16<8��������Ҫ2ҳ������ʾ�����꣨x,y����yΪҳ��Χ0��7
void OLED_Show_Position(u8 x, u8 y, const char *format, ...);             //�̶�λ����ʾ���ֻ��ַ�

void OLED_Show_Printf(u8 x, u8 y, u8 z, const char *format, ...);
void OLED_Draw_BMP(unsigned char BMP[][128]); //��ʾ��ʾBMPͼƬ128��64��ʼ������(0,0),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7

void OLEDPritnf(const char *format, ...);
void OLED_P6x8CharPutc(unsigned char *x, unsigned char *y, unsigned char ch);
void OLED_P8x16CharPutc(unsigned char *x, unsigned char *y, unsigned char ch);

#ifdef __cplusplus
}
#endif

#endif
