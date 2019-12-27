/*
 * interrupt.c
 *
 *  Created on: 2019年5月20日
 *      Author: Administrator
 */
#include "interrupt.h"
#include "xscugic.h"
#include "xemacps_hw.h"

//定义中断向量表结构体,Handler为函数，Data为函数Handler的参数
typedef struct {
    Xil_ExceptionHandler Handler;
    void *Data;
} XExc_VectorTableEntry;
//申明中断向量表
extern XExc_VectorTableEntry XExc_VectorTable[];

void IntAllDisabled(void)
{
	/* Remove current CPU from interrupt target register */

	Xil_Out32(ICDICER0, 0xffffffff);//不使能中断
	Xil_Out32(ICDICER1, 0xffffffff);//不使能中断
	Xil_Out32(ICDICER2, 0xffffffff);//不使能中断
	//disables the distributor
	Xil_Out32(ICDDCR,0x00);

}

void DistributorInit(void)
{
	u32 reg_addr;
	reg_addr = ICDICFR0;
	u32	default_priority = 0xa0a0a0a0;

	//清除触发模式
	for(char i=0;i<6;i++)
	{
		reg_addr += i*4;
		Xil_Out32(reg_addr,0x00000000);

	}



	// 设置default 优先级
	reg_addr = ICDIPR0;
	for(char i=0;i<24;i++)
	{
		reg_addr += i*4;
		Xil_Out32(reg_addr,default_priority);

	}

	// enables the distributor
	Xil_Out32(ICDDCR,0x01);

}

void CPU_Init(void)
{
    //中断优先级都是A0,优先级高于F0，CPU可接受这些中断
    Xil_Out32(ICCPMR,0xF0);
    //处理器能接收IRQ，使能中断信号连接到处理器
    Xil_Out32(ICCICR,0x07);
}

void Ethernet0_Int_Init(void)
{



    Xil_Out32(ICDICER1, 0x400000);//不使能 #54


    Xil_Out32(ICDICFR3, 0x1000);//电平触发
    Xil_Out32(ICDIPR13, 0xA00000);//优先级A0
    Xil_Out32(ICDIPTR13,0x010000);//处理器为CPU1

    Xil_Out32(ICDISER1, 0x400000);//使能 #54

}


void InterruptHandler_IRQ(u32 data)
{
    u32 i;
    u32 IntID;
    u32 IntIDFull;
    u32 isr;
    //xil_printf("data = %d\r\n",data);
    //for (i = 0; i < 50000000; ++i) {}//延时防抖动
    //获取目前发生的中断号
    IntIDFull = Xil_In32(ICCIAR);
    IntID = IntIDFull & 0x3FF;
    xil_printf("ICCIAR = %d\r\n",IntIDFull);
   //xil_printf("ICDICPR0 = %x\r\n",Xil_In32(ICDICPR0));
   //xil_printf("ICDICPR1 = %x\r\n",Xil_In32(ICDICPR1));


    switch(IntID)
    {
    case INT_ETHERNET0:
    		eth0_handle();
    	break;
    default:
    	break;

    }

    Xil_Out32(ICCEOIR, IntIDFull);//cpu结束响应，中断 状态由active->inactive


}


 void Int_init(void)
{
	 u32 data = 30;
	Xil_ExceptionInit();
    XExc_VectorTable[5].Handler =(Xil_ExceptionHandler)InterruptHandler_IRQ;
    XExc_VectorTable[5].Data = data;

    //initialize
    IntAllDisabled();
    DistributorInit();
    CPU_Init();
    Ethernet0_Int_Init();


    xil_printf("begin\r\n");
    Xil_ExceptionEnable();//这句放在这里,避免开机中断

}

