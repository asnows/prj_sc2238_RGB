/*
 * interrupt.h
 *
 *  Created on: 2019��5��20��
 *      Author: Administrator
 */

#ifndef SRC_INTERRUPT_H_
#define SRC_INTERRUPT_H_
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include"xemacps_hw.h"


#define DIRM_1          0xE000A244
#define INT_EN_1        0xE000A250
#define INT_DIS_1       0xE000A254
#define INT_STAT_1      0xE000A258//READ:1-int has occurred WRITE:1-clear int status bit
#define INT_TYPE_1      0xE000A25C
#define INT_POLARITY_1  0xE000A260
#define INT_ANY_1       0xE000A264
//ICC
#define ICCICR          0xF8F00100//CPU Interface Control Register  ����CPU�ӿ�
#define ICCPMR      	0xF8F00104//Interrupt priority mask Register ����CPU�ж����ȼ�
#define ICCIAR          0xF8F0010C//Interrupt Acknowledge Register
#define ICCEOIR         0xF8F00110//End Of Interrupt Register cpu������Ӧ���ж�״̬��active->inactive

//ICD
#define ICDDCR      	0xF8F01000//Distributor Control Register ���ƿ�����ر��ж�����
#define ICDISER0        0xF8F01100//Interrupt Set-enable Register ʹ��ICD�жϼĴ���
#define ICDISER1        0xF8F01104//Interrupt Set-enable Register ʹ��ICD�жϼĴ���
#define ICDISER2        0xF8F01108//Interrupt Set-enable Register ʹ��ICD�жϼĴ���

#define ICDICER0        0xF8F01180//Interrupt clear-enable Register ��ʹ��ICD�жϼĴ���
#define ICDICER1        0xF8F01184//Interrupt clear-enable Register ��ʹ��ICD�жϼĴ���
#define ICDICER2        0xF8F01188//Interrupt clear-enable Register ��ʹ��ICD�жϼĴ���

#define ICDISPR0 		0xF8F01200//Interrupt Set-pending Register_0

#define ICDIPR0         0xf8f01400//Interrupt priority Register ICD�ж����ȼ�
#define ICDIPR13        0xf8f01434//Interrupt priority Register ICD�ж����ȼ�
#define ICDIPTR13       0xF8F01834//Interrupt Processor Targets Register ����CPU�ӿ�ѡ��

#define ICDICFR0		0xF8F01C00//Interrupt Configuration Register ����ICD�жϴ���ģʽ
#define ICDICFR3        0xF8F01C0C//Interrupt Configuration Register ����ICD�жϴ���ģʽ

#define ICDICPR0        0xf8f01280//Interrupt clear-pending Register ����жϼĴ���
#define ICDICPR1        0xf8f01284//Interrupt clear-pending Register ����жϼĴ���
#define ICDICPR2        0xf8f01288//Interrupt clear-pending Register ����жϼĴ���

#define SPI_STATUS_0    0xF8F01D04//SPI Status Register 0

#define XEMACPS_ISR     0xE000B024//




// define interrupt number

#define INT_ETHERNET0	54 //Ethernet 0




void Int_init(void);

#endif /* SRC_INTERRUPT_H_ */
