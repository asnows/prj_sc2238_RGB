/*
 * xemacps_hw.c
 *
 *  Created on: 2019年5月13日
 *      Author: Administrator
 */
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xemacps_hw.h"
#include "stdlib.h"

struct DESC_LIST_DEF Rx_List;
struct DESC_LIST_DEF Tx_List;
struct BUF_DEF Rx_buff;
struct BUF_DEF Tx_buff;
struct HEAD_DEF *p_header;

//Initialize the Controller
void Initialize_Controller()
{
	//1. Clear the Network Control register. Write 0x0 to gem.net_ctrl register.
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,0x00000000);

	//2. Clear the Statistics registers. Write a 1 to gem.net_ctrl[clear_stat_regs].
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,XEMACPS_NWCTRL_STATCLR_MASK);

	//3. Clear the Status registers. Write a 1 to the Status registers. gem.rx_status = 0x0F andgem.tx_status = 0xFF.
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET,0x0f);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXSR_OFFSET,0xff);

	//4. Disable all interrupts. Write 0x7FF_FEFF to the gem.intr_dis register.
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_IDR_OFFSET,0x7FFFEFF);


	//5. Clear the buffer queues. Write 0x0 to the gem.rx_qbar and gem.tx_qbar registers.
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR  + XEMACPS_RXQBASE_OFFSET,0x00000000);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXQBASE_OFFSET,0x00000000);

}

//Configure the Controller
void Configure_Controller()
{
	unsigned int cfg_value;
	unsigned int Divisor  = 0x03;//<< XEMACPS_NWCFG_MDC_SHIFT_MASK
//1. Program the Network Configuration register (gem.net_cfg)
	cfg_value = XEMACPS_NWCFG_FDEN_MASK
			+ XEMACPS_NWCFG_1000_MASK
			//+ XEMACPS_NWCFG_100_MASK
			+ XEMACPS_NWCFG_RXCHKSUMEN_MASK
			+ XEMACPS_NWCFG_PAUSEEN_MASK
			//+ XEMACPS_NWCFG_MDCCLKDIV_MASK;
			+ (Divisor << XEMACPS_NWCFG_MDC_SHIFT_MASK);
	///////------------------------------------///
//	cfg_value= XEMACPS_NWCFG_MDCCLKDIV_MASK;
//	cfg_value = cfg_value | (u32)XEMACPS_NWCFG_100_MASK |
//			(u32)XEMACPS_NWCFG_FDEN_MASK |
//			(u32)XEMACPS_NWCFG_UCASTHASHEN_MASK;

	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCFG_OFFSET,cfg_value);
	xil_printf("cfg_value = %x \r\n",cfg_value);

//2. Set the MAC address
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR+XEMACPS_LADDR1L_OFFSET,LOCAL_MAC0);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR+XEMACPS_LADDR1H_OFFSET,LOCAL_MAC1);

	xil_printf("LADDR1L = %x \r\n",Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_LADDR1L_OFFSET));
	xil_printf("LADDR1H = %x \r\n",Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_LADDR1H_OFFSET));

//3. Program the DMA Configuration register
	cfg_value = XEMACPS_DMACR_RXBUF_MASK
			+ XEMACPS_DMACR_RXSIZE_MASK
			+ XEMACPS_DMACR_TXSIZE_MASK
			+ XEMACPS_DMACR_TCPCKSUM_MASK
			+ XEMACPS_DMACR_ENDIAN_MASK
			+ XEMACPS_DMACR_INCR16_AHB_BURST;

//	cfg_value = (((((u32)XEMACPS_RX_BUF_SIZE / (u32)XEMACPS_RX_BUF_UNIT) +
//					(((((u32)XEMACPS_RX_BUF_SIZE %
//					(u32)XEMACPS_RX_BUF_UNIT))!=(u32)0) ? 1U : 0U)) <<
//					(u32)(XEMACPS_DMACR_RXBUF_SHIFT)) &
//					(u32)(XEMACPS_DMACR_RXBUF_MASK)) |
//					(u32)XEMACPS_DMACR_RXSIZE_MASK |
//					(u32)XEMACPS_DMACR_TXSIZE_MASK;
	xil_printf("cfg_value_dma = %x \r\n",cfg_value);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_DMACR_OFFSET,cfg_value);

//4. Program the Network Control Register (gem.net_ctrl)
	cfg_value = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET);
	cfg_value = cfg_value
			| XEMACPS_NWCTRL_MDEN_MASK;
			//+ XEMACPS_NWCTRL_TXEN_MASK
			//+ XEMACPS_NWCTRL_RXEN_MASK;
   Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,cfg_value);


}

void Configure_Buffer_Descriptors()
{

	u32 addr;
	u32 Wrap;
	u32 Used = 0;

	//init buff;
	for(unsigned char i=0;i<4;i++)
	{
		Rx_buff.p_addr[i] = (unsigned char*)(RX_BUFFER0_DADDR + i*IMG_BYTES);
	    Tx_buff.p_addr[i] = (unsigned char*)(TX_BUFFER0_DADDR + i*IMG_BYTES);

	    xil_printf("Rx_buff.p_addr[%d] = %x\r\n",i,Rx_buff.p_addr[i]);
	    xil_printf("Tx_buff.p_addr[%d] = %x \r\n",i,Tx_buff.p_addr[i]);
	}


	//init Descriptors list
	for(unsigned char i=0;i<4;i++)
	{
		addr = (u32)Rx_buff.p_addr[i];

		if(i == 3)
		{
			Wrap = 0x01;
		}
		else
		{
			Wrap = 0x00;
		}

		Rx_List.Descriptor[i].word0 = (addr << 2) + (Wrap <<1) ;

		Tx_List.Descriptor[i].word0 = (u32)Tx_buff.p_addr[i];
		Tx_List.Descriptor[i].word1 = (Used << 31) + (Wrap <<30) ;

		xil_printf("Rx_List.Descriptor[%d].word0 = %x\r\n",i,Rx_List.Descriptor[i].word0 >> 2);
	    xil_printf("Tx_List.Descriptor[%d].word0 = %x \r\n",i,Tx_List.Descriptor[i].word0);

	}
   // write gem.rx_qbar register
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXQBASE_OFFSET,(u32)Rx_List.Descriptor);
	//write gem.tx_qbar register
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXQBASE_OFFSET,(u32)Tx_List.Descriptor);

	xil_printf("XEMACPS_RXQBASE_OFFSET = %x \r\n",Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXQBASE_OFFSET));
	xil_printf("XEMACPS_TXQBASE_OFFSET = %x \r\n",Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXQBASE_OFFSET));

}


void Configure_Interrupts()
{
	//Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_IDR_OFFSET,0x00000000);
	//Enable interrupts
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_IER_OFFSET,0x7FFFEFF);


}

void Enable_Controller()
{
	u32 tx_en = 1;
	u32 rx_en = 1;
	u32 value;

	//1. Enable the Transmitter. Write a 1 to gem.net_ctrl[tx_en]
	//2. Enable the Receiver. Write a 1 to gem.net_ctrl[rx_en].
	tx_en <<=3;
	rx_en <<=2;
	value = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET);
	value = value | tx_en | rx_en;
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,value);

}

u32 get_ip(char*ip)
{

	//char *ip = "201.123.118.110";

	char a[3];

	int b[4];

	char *str = ip;

	int i = 0, j = 0;

	int len = strlen(ip);

	printf("len = %d\n",len);

	//unsigned long my_ip;
	u32 my_ip;


	len++;
	while(len--)
	{
	  if(*str!='.'&&*str!='\0')
	  {
		  a[i] = *str;
		  i++;
		  str++;
	  }
	  else
	  {
		  str++;
		  i = 0;
		  b[j] = atoi(a);
		  j++;

	  }
	}
	my_ip = b[0]+((b[1]&0xff)<<8)+((b[2]&0xff)<<16)+((b[3]&0xff)<<24);
	xil_printf("%x,%x,%x,%x,%x\n",b[0],b[1],b[2],b[3],my_ip);

	return my_ip;
}


void send_ip_checksum()
{
	unsigned short * p_data;
	unsigned int sum = 0;
	unsigned int lWord = 0;
	unsigned int hWord = 0;
	unsigned short checksum = 0;

	p_header->ip_HeaderChecksum  = 0;

	p_data = (unsigned short * )p_header;
	p_data += 7;

	for(char i=0; i<10;i++)
	{
		xil_printf("p_data[%d] = %x \r\n",i,*p_data);
		sum += *p_data;
		p_data ++;
		xil_printf("sum = %x \r\n",sum);


	}

	lWord = sum &0x0000ffff;
	hWord = sum >> 16;

	checksum = lWord + hWord;
	checksum = ~checksum;

	p_header->ip_HeaderChecksum =  checksum;
}

void HeaderEndianSwap()
{
	u16 data;
	data = p_header->eth_type;
	p_header->eth_type = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->ip_IHL;
	p_header->ip_IHL=p_header->ip_Ver;
	p_header->ip_Ver = data;

	data = p_header->ip_PacketLength;
	p_header->ip_PacketLength = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->ip_Identification;
	p_header->ip_Identification = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->ip_HeaderChecksum;
	p_header->ip_HeaderChecksum = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->udp_SourcePort;
	p_header->udp_SourcePort = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->udp_DestiPort;
	p_header->udp_DestiPort = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->udp_Length;
	p_header->udp_Length = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);

	data = p_header->udp_Checksum;
	p_header->udp_Checksum = ((data >> 8) & 0x00ff) + ((data << 8)&0xff00);


}



void header_init()
{
	u32 remot_ip;
	u32 local_ip;
	unsigned char local_MAC[]  = {0x00, 0x0a, 0x35, 0x01, 0x02, 0x03};
	unsigned char remote_MAC[] = {0x08, 0x57, 0x00, 0xf4, 0xae, 0xe5};

	p_header = malloc(sizeof(struct HEAD_DEF));

	remot_ip = get_ip((char *)&REMOT_IP_ADDR);
	local_ip = get_ip((char *)&LOCAL_IP_ADDR);

	//eth header
	for(unsigned char i=0;i<6;i++)
	{
		p_header->eth_remote_mac[i] = remote_MAC[i];
		p_header->eth_local_mac[i]  = local_MAC[i];
	}


	p_header->eth_type = 0x0800;

	//ip header
	p_header->ip_Ver = 4;
	p_header->ip_IHL = 5;
	p_header->ip_ServiceType = 0;
	p_header->ip_PacketLength = 5;
	p_header->ip_Identification = 0;
	p_header->ip_Flag = 2;
	p_header->ip_FragOffset =0;
	p_header->ip_TTL = 128;
	p_header->ip_Protocol = 17;
	p_header->ip_HeaderChecksum = 0;

	for(u8 i=0;i<4;i++)
	{
		p_header->ip_SourceAddress[i] = ((u8 *)&local_ip)[i];
		p_header->ip_DestiAddress[i] = ((u8*)&remot_ip)[i];
	}



	// udp header
	p_header->udp_SourcePort = LOCAL_PORT;
	p_header->udp_DestiPort = REMOT_PORT;
	p_header->udp_Length = 8;
	p_header->udp_Checksum = 0;



//	union{
//	short i;
//	char a[2];
//	}u;//联合体u
//
//	u.a[0] = 0x11;
//	u.a[1] = 0x22;
//
//	printf ("联合体u = 0x%x\n", u.i); //0x2211 为小端 0x1122 为大端


}

void update_header_param(u16 data_length)
{

	p_header->udp_Length = 8 + data_length;
	p_header->ip_PacketLength = p_header->udp_Length + 20;
	send_ip_checksum();
	HeaderEndianSwap();
}


void package_frame(u8 *p_TxBuff)
{

	u8 *p_data;
	p_data = p_TxBuff;

	memcpy(p_data,p_header,sizeof(struct HEAD_DEF));



}


void enable_trasnission()
{
	u32 value;

	value = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET);
	value = value | (0x01 << 9);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,value);

}

u8 trans_complete()
{
	u32 trans_status;
	u32 int_status;

	u8 i=255;
	while(i)
	{
		trans_status = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXSR_OFFSET);
		usleep(100);
		int_status = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET);
		xil_printf("trans_complete i= %d,int_status = %x \r\n",i,int_status);
		xil_printf("trans_complete i= %d,trans_status = %x \r\n",i,trans_status);

		if(trans_status & 0x20)
		{
			i = 0;
			trans_status = trans_status &(~(0x01 << 5));
			Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,trans_status);
			return 1;
		}
		i--;
	}

	return 0;


}


void Transmitting_Frames()
{
	u32 word1;

	u16 data_length;

	update_header_param(64);
	package_frame(Tx_buff.p_addr[0]);

	xil_printf("word1= %x !\r\n",Tx_List.Descriptor[0].word1);

	//bit 15 (Last buffer bit) of the word 1 must also be set
	data_length = p_header->ip_PacketLength;

	word1 = Tx_List.Descriptor[0].word1;
	word1 = word1 | (0x01 << 15) +  data_length;
	Tx_List.Descriptor[0].word1 = word1;
	xil_printf("word1= %x !\r\n",Tx_List.Descriptor[0].word1);

	Enable_Controller();
	enable_trasnission();
	xil_printf("word1= %x !\r\n",Tx_List.Descriptor[0].word1);

	if(trans_complete())
	{
		xil_printf("trans_complete succeed !\r\n");
	}
	else
	{

		xil_printf("trans_complete failed !\r\n");
	}
	xil_printf("word1= %x !\r\n",Tx_List.Descriptor[0].word1);
}

void emacps_init()
{
	Initialize_Controller();
	Configure_Controller();
	//init_phy();
	Configure_Buffer_Descriptors();
	Configure_Interrupts();
	header_init();
	//Enable_Controller();

	while(1)
	{
		Transmitting_Frames();

		sleep(1);
	}
    //Configure_Buffer_Descriptors();
    //init_phy();
}


