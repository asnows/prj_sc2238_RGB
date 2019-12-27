/*
 * sc030_fpga.h
 *
 *  Created on: 2018��7��31��
 *      Author: Administrator
 */

#ifndef SC030_FPGA_H_
#define SC030_FPGA_H_
void SC30_XCK(void);
void SC30_PWDN(void);//PWDN�ϵ������ź�
void SC30_RESETB(void);//��λ�ź�
void SC30_Set_HHVVCC(unsigned short HH,unsigned short VV,unsigned char CC);
void SC30_Set_AutoRate(float Rate);
void SC30_Set_SoftExposure(void);
void vdma1_enable(void);
void vdma1_disable(void);
void vdma0_enable(void);
void vdma0_disable(void);
int vdma0_rd_status(void);
void hist_ctl(unsigned char  val);
void dpc_thread(unsigned int val);
void dpc_ctrl(unsigned char val);
void debug_cmd1(unsigned int cmd);
void debug_cmd2(unsigned int cmd);
void E_OUT1(unsigned int flg);
void E_OUT2(unsigned int flg);
void GAUS_EN(void);
void BayerType(unsigned int Type );




#endif /* SC030_FPGA_H_ */