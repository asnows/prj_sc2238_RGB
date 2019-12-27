/*
 * xemacps_hw.c
 *
 *  Created on: 2019年5月13日
 *      Author: Administrator
 */


#include"include.h"

struct DESC_LIST_DEF Rx_List __attribute__ ((aligned(64)));
struct DESC_LIST_DEF Tx_List __attribute__ ((aligned(64)));
struct BUF_DEF Rx_buff;
struct BUF_DEF Tx_buff;
struct HEAD_DEF *p_header;
struct ETH0_DEF eth0_ctrl;
//struct DESC_LIST_DEF *p_Rx_List;
struct RX_BD_LIST_DEF *p_Rx_List;


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
	unsigned int Divisor  = 0x07;//<< XEMACPS_NWCFG_MDC_SHIFT_MASK
//1. Program the Network Configuration register (gem.net_cfg)
	cfg_value = XEMACPS_NWCFG_FDEN_MASK
			| XEMACPS_NWCFG_1000_MASK
			| XEMACPS_NWCFG_LENERRDSCRD_MASK
			| XEMACPS_NWCFG_RXCHKSUMEN_MASK
			| XEMACPS_NWCFG_PAUSEEN_MASK
			| XEMACPS_NWCFG_FCSREM_MASK
			| XEMACPS_NWCFG_UCASTHASHEN_MASK
			| (Divisor << XEMACPS_NWCFG_MDC_SHIFT_MASK);
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
			| XEMACPS_DMACR_RXSIZE_MASK
			| XEMACPS_DMACR_TXSIZE_MASK
			| XEMACPS_DMACR_DISC_WHEN_NO_AHB
			| XEMACPS_DMACR_TCPCKSUM_MASK
			| XEMACPS_DMACR_ENDIAN_MASK
			| XEMACPS_DMACR_INCR16_AHB_BURST;

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
		Rx_buff.p_addr[i] = (unsigned char*)(RX_BUFFER0_DADDR + i*1536);
	    Tx_buff.p_addr[i] = (unsigned char*)(TX_BUFFER0_DADDR + i*1536);

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
	//Enable interrupts
	/* Enable TX and RX interrupts */

	u32 mask = XEMACPS_IXR_TX_ERR_MASK
			|XEMACPS_IXR_RX_ERR_MASK
			|XEMACPS_IXR_FRAMERX_MASK
			|XEMACPS_IXR_TXCOMPL_MASK;

	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_IER_OFFSET,mask);
//
//    XEmacPs_IntEnable(InstancePtr, (XEMACPS_IXR_TX_ERR_MASK |
//XEMACPS_IXR_RX_ERR_MASK | (u32)XEMACPS_IXR_FRAMERX_MASK |
//(u32)XEMACPS_IXR_TXCOMPL_MASK));


}

void Enable_Controller()
{

	u32 value;

	//1. Enable the Transmitter. Write a 1 to gem.net_ctrl[tx_en]
	//2. Enable the Receiver. Write a 1 to gem.net_ctrl[rx_en].

	value = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET);
	value &= ~XEMACPS_NWCTRL_RXEN_MASK;

	value = value | XEMACPS_NWCTRL_TXEN_MASK | XEMACPS_NWCTRL_RXEN_MASK;
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

	//printf("len = %d\n",len);

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
	//xil_printf("%x,%x,%x,%x,%x\n",b[0],b[1],b[2],b[3],my_ip);

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
		//xil_printf("p_data[%d] = %x \r\n",i,*p_data);
		sum += *p_data;
		p_data ++;
		//xil_printf("sum = %x \r\n",sum);


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



void header_init(u16 data_length)
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
	p_header->ip_Flag = 0;
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

	p_header->udp_Length = sizeof(struct HEAD_DEF)  + data_length - 20 -14;
	p_header->ip_PacketLength = p_header->udp_Length + 20;


	//user define header
	p_header->net_cmd_struct.cmd_c = 0x63;
	p_header->net_cmd_struct.cmd_m = 0x6d;
	p_header->net_cmd_struct.cmd_d = 0x64;
	p_header->net_cmd_struct.cmd_reserve = 0x00;
	p_header->net_cmd_struct.cmd = 0x00;
	p_header->net_cmd_struct.img_width  = IMG_WIDTH;
	p_header->net_cmd_struct.img_height = IMG_HEIGHT;
	p_header->net_cmd_struct.frame = 0x00;
	p_header->net_cmd_struct.frame_num[0] = 0;
	p_header->net_cmd_struct.frame_num[1] = 0;
	p_header->net_cmd_struct.frame_num[2] = 0;
	p_header->net_cmd_struct.frame_num[3] = 0;

	send_ip_checksum();
	HeaderEndianSwap();

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
	Xil_DCacheFlushRange((UINTPTR)p_data, sizeof(struct HEAD_DEF));
}


void start_trasnission()
{
	u32 value;

	value = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWCTRL_OFFSET,value |XEMACPS_NWCTRL_STARTTX_MASK);

}



/*
void BdBaseAddr(u32 addr)
{

	//write gem.tx_qbar register
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXQBASE_OFFSET,addr);

}


void BdSetLength(struct DESC_DEF *p_BD)
{
	p_BD->word1 = p_header->ip_PacketLength + 14;

}

void BdClearTxUsed(struct DESC_DEF *p_BD)
{
	p_BD->word1 &= (~XEMACPS_TXBUF_USED_MASK);

}

void BdSetLast(struct DESC_DEF *p_BD)
{
	p_BD->word1 = p_BD->word1 | XEMACPS_TXBUF_LAST_MASK;// | XEMACPS_TXBUF_WRAP_MASK;
}
*/


void BdBaseAddr(struct DESC_LIST_DEF *p_List)
{


	p_List->Descriptor[0].word0 = (u32)p_header->eth_remote_mac;
	p_List->Descriptor[1].word0 = eth0_ctrl.frame_addr[0];
	//write gem.tx_qbar register
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXQBASE_OFFSET,(u32)Tx_List.Descriptor);
	Xil_DCacheFlushRange((UINTPTR)(u32)p_header->eth_remote_mac, sizeof(struct HEAD_DEF));


}


void BdSetLength(struct DESC_LIST_DEF *p_List)
{


	p_List->Descriptor[0].word1 = sizeof(struct HEAD_DEF);//p_header->ip_IHL*5 + 14 + 8;
	p_List->Descriptor[1].word1 = PAYLOAD_BYTES;


}


void BdClearTxUsed(struct DESC_LIST_DEF *p_List)
{

	p_List->Descriptor[0].word1 &=(~XEMACPS_TXBUF_USED_MASK);
	p_List->Descriptor[1].word1 &=(~XEMACPS_TXBUF_USED_MASK);

}



void BdSetLast(struct DESC_LIST_DEF *p_List)
{
	p_List->Descriptor[1].word1 = p_List->Descriptor[1].word1 | XEMACPS_TXBUF_LAST_MASK |XEMACPS_TXBUF_WRAP_MASK;
}

//trans buffer descriptor init
void Tx_BD_init()
{





}




//receive buffer descriptor init
void Rx_BD_init()
{

	char *p_revBuf;
	u32 buf_size = 1536;
	u8  bd_cnt;

	p_revBuf = (char *)DDR_RX_BUF_START_ADDR;
	p_Rx_List = (struct RX_BD_LIST_DEF *)DDR_RX_BD_START_ADDR;
	memset(p_Rx_List,0,sizeof(struct RX_BD_LIST_DEF));

	bd_cnt = sizeof(struct RX_BD_LIST_DEF)/8;

	// init BD
	for(u8 i=0;i<bd_cnt;i++)
	{

		if(i == (bd_cnt- 1))
		{
			//最后一个BD
			p_Rx_List->Descriptor[i].word0 = ((u32)p_revBuf | XEMACPS_RXBUF_WRAP_MASK)& (~XEMACPS_RXBUF_NEW_MASK);

		}
		else
		{
			p_Rx_List->Descriptor[i].word0 = (u32)p_revBuf & (~XEMACPS_RXBUF_WRAP_MASK) & (~XEMACPS_RXBUF_NEW_MASK);

		}
		p_revBuf += buf_size;


	}

	//write gem.rx_qbar register
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXQBASE_OFFSET,(u32)p_Rx_List->Descriptor);
	Xil_DCacheFlushRange((UINTPTR)(u32)p_Rx_List->Descriptor, sizeof(struct RX_BD_LIST_DEF));





}



void eth0_init()
{
	Initialize_Controller();
	Configure_Controller();
	init_phy();
	//Configure_Buffer_Descriptors();
	Configure_Interrupts();

	header_init(PAYLOAD_BYTES);
	//package_frame(Tx_buff.p_addr[0]);
	BdBaseAddr(&Tx_List);
	BdSetLength(&Tx_List);
	//BdClearTxUsed(&Tx_List);
	BdSetLast(&Tx_List);

	Rx_BD_init();
	Enable_Controller();



}





void Trans_Frames()
{


	u32 txsr;
	u32 *p_dptr;

	p_dptr = (u32 *)p_header->net_cmd_struct.frame_num;
	//eth0_ctrl.gray_code = Xil_In32(XPAR_AXI_GPIO_0_BASEADDR);
	//xil_printf("gray_code = %d \r\n",eth0_ctrl.gray_code);

	txsr  = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXSR_OFFSET);
	txsr &= XEMACPS_TXSR_TXCOMPL_MASK;
	if(eth0_ctrl.flg | txsr)
	{

		eth0_ctrl.flg  = 0;

		// clear trans compelet bit
		Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXSR_OFFSET,(txsr & XEMACPS_TXSR_TXCOMPL_MASK));
		BdClearTxUsed(&Tx_List);

		u32 value = sizeof(struct DESC_LIST_DEF);
		Xil_DCacheFlushRange((UINTPTR)Tx_List.Descriptor, value);

		Enable_Controller();
		start_trasnission();

		// 更新帧序号 和帧地址
		*p_dptr +=1;
		if(*p_dptr > eth0_ctrl.frame_total)
		{
			*p_dptr = 0;
			eth0_ctrl.gray_code ++;
			if(eth0_ctrl.gray_code >3)
			{
				eth0_ctrl.gray_code = 0;

			}
			Tx_List.Descriptor[1].word0 = eth0_ctrl.frame_addr[eth0_ctrl.gray_code];

		}
		Xil_DCacheFlushRange((UINTPTR)(u32)p_header->eth_remote_mac, sizeof(struct HEAD_DEF));
		Tx_List.Descriptor[1].word0 += PAYLOAD_BYTES;


	}


}

void trans_arp(u8* p_data)
{
	// 比较ip地址


	//if(*((u32*)(p_data + 38)) =  *(u32 *)p_header->ip_SourceAddress)
	{
		xil_printf("trans_arp == %x \r\n",*((u32*)(p_data + 38)));
		//组态arp 头
		u8 arp_buf[42];
		u32 tx_bd[2];
		memcpy(&arp_buf[0],(p_data+6),6);// dst  mac
		memcpy(&arp_buf[6],p_header->eth_local_mac,6);//src mac
		// 帧类型 arp = 0x0806
		arp_buf[12] = 0x08;
		arp_buf[13] = 0x06;
		//硬件类型
		arp_buf[14] = 0x00;
		arp_buf[15] = 0x01;
		//协议类型
		arp_buf[16] = 0x08;
		arp_buf[17] = 0x00;
		// 硬件地址长度
		arp_buf[18] = 0x06;
		// 协议地址长度
		arp_buf[19] = 0x04;
		// 操作字段
		arp_buf[20] = 0x00;
		arp_buf[21] = 0x02;
		//发送方以太网地址
		memcpy(&arp_buf[22],p_header->eth_local_mac,6);//src mac
		//发送方IP 地址
		memcpy(&arp_buf[28],p_header->ip_SourceAddress,4);//src ip
		//接收方以太网地址
		memcpy(&arp_buf[32],(p_data+6),6);// dst  mac
		//接收方IP 地址
		memcpy(&arp_buf[38],(p_data+28),4);// dst  ip

		//组态trans bd
		tx_bd[0] = arp_buf;
		tx_bd[1] = 0x802a;

		//write gem.tx_qbar register
		Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXQBASE_OFFSET,(u32)tx_bd);
		Xil_DCacheFlushRange((UINTPTR)arp_buf, 42);
		Xil_DCacheFlushRange((UINTPTR)tx_bd, 8);
		//Xil_DCacheFlushRange((UINTPTR)(u32)p_header->eth_remote_mac, sizeof(struct HEAD_DEF));

		Enable_Controller();
		start_trasnission();


	}




}


void rev_frame_decode(u8* p_data)
{
	u8 *p_dptr;
	u8 *p_revData;
	u16 cmd;
	p_dptr = &p_header->net_cmd_struct.cmd_c;


	p_revData = (p_data + 42);
	xil_printf("p_revData = %x \r\n",*((u32 *)(p_data + 42)));
	xil_printf("p_dptr = %x \r\n",*((u32 *)p_dptr));

	// 获取 接收到的是否有c m d 标识
	if(*((u32 *)p_revData) !=  *((u32 *)p_dptr))
	{
		 return;
	}


	//提取命令
	*(u8*)(&cmd) 	   = *(p_data + 46);
	*((u8*)(&cmd) + 1) = *(p_data + 47);

	xil_printf("cmd = %x \r\n",cmd);
	switch(cmd)
	{
		case CMD_UDP_LINK:
			xil_printf("CMD_UDP_LINK \r\n");
			eth0_ctrl.net_cmd = CMD_UDP_LINK;
			break;
		case CMD_PIC_STREAM:
			xil_printf("CMD_PIC_STREAM \r\n");
			eth0_ctrl.net_cmd = CMD_PIC_STREAM;
			break;
		case CMD_PIC_CAPTURE:
			xil_printf("CMD_PIC_CAPTURE \r\n");
			eth0_ctrl.net_cmd = CMD_PIC_CAPTURE;
			break;
		case CMD_PIC_STOP:
			xil_printf("CMD_PIC_STOP \r\n");
			eth0_ctrl.net_cmd = CMD_PIC_STOP;
			break;
		default:
			break;

	}


}


void Rev_Frame_process()
{
	u8 bd_cnt;
	u32 tmp;
	u32 isr;
	u8* p_data;
	u8 i;
	u32 srcAddr;
	u32 srcLength;
	struct DESC_DEF *p_dptr;
	u8 *p_buf;
	p_buf =DDR_REV_BUF_START_ADDR;


	bd_cnt = sizeof(struct RX_BD_LIST_DEF)/8;

	// 查询已使的BD
	for(i=0;i<bd_cnt;i++)
	{
		tmp = p_Rx_List->Descriptor[i].word0 & XEMACPS_RXSR_BUFFNA_MASK;
		xil_printf("i = %d tmp = %d \r\n",i,tmp);

		if(tmp)
		{
			// clear Ownership
			srcAddr = p_Rx_List->Descriptor[i].word0 & (~XEMACPS_RXSR_BUFFNA_MASK);
			srcLength = p_Rx_List->Descriptor[i].word1 & 0x00001fff;
			xil_printf("p_buf = %x srcAddr = %x \r\n",p_buf,srcAddr);
			memset(p_buf,0,srcLength);
			memcpy(p_buf,srcAddr,srcLength);
			p_Rx_List->Descriptor[i].word0 = p_Rx_List->Descriptor[i].word0 & (~XEMACPS_RXSR_BUFFNA_MASK);
			p_dptr = &p_Rx_List->Descriptor[i];


			xil_printf("p_buf = %x srcAddr = %x,srcLength = %d \r\n",p_buf,srcAddr,srcLength);


			//Xil_DCacheFlushRange((UINTPTR)(u32)p_Rx_List->Descriptor, sizeof(struct RX_BD_LIST_DEF));

			break;
		}

	}

	//write gem.rx_qbar register
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXQBASE_OFFSET,(u32)p_Rx_List->Descriptor);
	Xil_DCacheFlushRange((UINTPTR)(u32)p_Rx_List->Descriptor, sizeof(struct RX_BD_LIST_DEF));

	// 超出范围
	if(i >= bd_cnt)
	{
		return;
	}

	isr = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET);
	//Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET,0xffffffff);
	xil_printf("isr = %d\r\n",isr);
	if(isr & XEMACPS_IXR_FRAMERX_MASK == 0)
	{

		return;
	}

	p_data = p_dptr->word0 & (~XEMACPS_RXSR_BUFFNA_MASK);
	// 判断IP是否
	if(*((u32*)(p_buf + 38)) =  *(u32 *)p_header->ip_SourceAddress)
	{
//		for(u8 i=0;i<16;i++)
//		{
//			xil_printf("p_data[%d] = %x \r\n",i,*(p_data + i));
//		}
//
//		xil_printf("eth_type = %x \r\n",*((u16 *)(p_data + 12)));
		//p_data = p_dptr->word0 & (~XEMACPS_RXSR_BUFFNA_MASK);
		//eth_type 网络协议判断
		switch(*((u16 *)(p_buf + 12)))
		{

		// ARP
		case 0x0608:
			printf(" rev ARP \r\n");
			trans_arp(p_buf);

			break;

		//IP
		case 0x0008:
			printf(" rev Ip \r\n");
			rev_frame_decode(p_buf);
			break;

		default:
			break;


		}

	}
}




void Rev_Frames(void)
{

	u32 rxsr;
	u32 isr;


	isr = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET);
	rxsr = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET);
	//xil_printf("rxsr = %d\r\n",rxsr);

	xil_printf("isr = %d\r\n",isr);
	// receive succeed
	if(isr & XEMACPS_IXR_FRAMERX_MASK)
	//if( rxsr & (XEMACPS_RXSR_FRAMERX_MASK | XEMACPS_RXSR_BUFFNA_MASK))
	{
		Rev_Frame_process();
	}

	// clear the gem.rx_status
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET,XEMACPS_RXSR_ERROR_MASK | XEMACPS_RXSR_FRAMERX_MASK);


}


u32 int_cnt = 0;

//中断处理函数
void eth0_handle()
{
	// clear int
	u32 isr_status;
	u32 rx_status;
	u32 tx_status;


	isr_status = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET);
	tx_status = Xil_In32 (XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_TXSR_OFFSET);
	rx_status = Xil_In32 (XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET);

	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET,0xffffffff);
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET,0xffffffff);
	xil_printf("isr_status = %x \r\n",isr_status);
	xil_printf("rx_status = %x \r\n",rx_status);
	xil_printf("RXQBASE_ = %x \r\n",Xil_In32 (XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXQBASE_OFFSET));


	//send succeed
	if(isr_status & XEMACPS_TXSR_TXCOMPL_MASK)
	{
		//Trans_Frames();

		//xil_printf("tans succeed \r\n");

	}

	// rev_succeed
	if((isr_status & XEMACPS_IXR_FRAMERX_MASK)  | (rx_status & (XEMACPS_RXSR_FRAMERX_MASK | XEMACPS_RXSR_BUFFNA_MASK)))
	//if((isr_status & XEMACPS_IXR_FRAMERX_MASK))
	{


		//Rev_Frames();
		Rev_Frame_process();

		int_cnt ++;
		xil_printf("rev succeed  %d\r\n",int_cnt);
//		xil_printf("Descriptor[0].word0  = %x, Descriptor[0].word1 =%x \r\n",p_Rx_List->Descriptor[0].word0,p_Rx_List->Descriptor[0].word1 );
//		xil_printf("Descriptor[1].word0  = %x, Descriptor[1].word1 =%x \r\n",p_Rx_List->Descriptor[1].word0,p_Rx_List->Descriptor[1].word1);

	}


	if(isr_status & XEMACPS_IXR_HRESPNOK_MASK)
	{
		//Trans_Frames();
		int_cnt ++;

		xil_printf("int_cnt == %d \r\n",int_cnt);
		if(tx_status & XEMACPS_TXSR_HRESPNOK_MASK)
		{
			xil_printf("Transmit hresp not OK\r\n");
		}

		if(rx_status & XEMACPS_RXSR_HRESPNOK_MASK)
		{
			xil_printf("Receive hresp not OK\r\n");
			Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET,0xffffffff);
			//Rx_BD_init();
		}


	}

	//xil_printf("XEMACPS_ISR = %x \r\n",Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET));
	//Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET,0xffffffff);
	//Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_RXSR_OFFSET,0xffffffff);
	//xil_printf("XEMACPS_ISR_handle = %x \r\n",Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_ISR_OFFSET));

}

// eth0控制参数初始化
void eth0_ctrl_Init(void)
{
	u32 tmp;
	eth0_ctrl.flg  = 1;
	eth0_ctrl.frame_total = IMG_BYTES/PAYLOAD_BYTES;
	tmp = IMG_BYTES%PAYLOAD_BYTES;

	if(tmp > 0)
	{
		eth0_ctrl.frame_total ++;
	}

	eth0_ctrl.frame_addr[0] = VDMA_IMGSTREAM_START_ADDR1;
	eth0_ctrl.frame_addr[1] = VDMA_IMGSTREAM_START_ADDR2;
	eth0_ctrl.frame_addr[2] = VDMA_IMGSTREAM_START_ADDR3;
	eth0_ctrl.frame_addr[3] = VDMA_IMGSTREAM_START_ADDR4;
	eth0_ctrl.gray_code = 0;


}



