
#include"include.h"







u32 vdma_version(XAxiVdma *Vdma) {
	return XAxiVdma_GetVersion(Vdma);
}

int vdma_read_start(XAxiVdma *Vdma) {
	int Status;

	// MM2S Startup
	Status = XAxiVdma_DmaStart(Vdma, XAXIVDMA_READ);
	if (Status != XST_SUCCESS)
	{
	   xil_printf("Start read transfer failed %d\n\r", Status);
	   return XST_FAILURE;
	}

	return XST_SUCCESS;
}


int vdma_read_stop(XAxiVdma *Vdma) {
	XAxiVdma_DmaStop(Vdma, XAXIVDMA_READ);
	return XST_SUCCESS;
}


int vdma_read_init(short DeviceID,short HoriSizeInput,short VertSizeInput,short Stride,unsigned int FrameStoreStartAddr)
{
	XAxiVdma Vdma;
	XAxiVdma_Config *Config;
	XAxiVdma_DmaSetup ReadCfg;
	int Status;


	Config = XAxiVdma_LookupConfig(DeviceID);
	if (NULL == Config) {
		xil_printf("XAxiVdma_LookupConfig failure\r\n");
		return XST_FAILURE;
	}

	Status = XAxiVdma_CfgInitialize(&Vdma, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("XAxiVdma_CfgInitialize failure\r\n");
		return XST_FAILURE;
	}

	

	ReadCfg.EnableCircularBuf = 1;
	ReadCfg.EnableFrameCounter = 0;
	ReadCfg.FixedFrameStoreAddr = 0;

	ReadCfg.EnableSync = 1;
	ReadCfg.PointNum = 1;

	ReadCfg.FrameDelay = 0;

	ReadCfg.VertSizeInput = VertSizeInput;
	ReadCfg.HoriSizeInput = HoriSizeInput;
	ReadCfg.Stride = Stride;

	Status = XAxiVdma_DmaConfig(&Vdma, XAXIVDMA_READ, &ReadCfg);
	if (Status != XST_SUCCESS) {
			xdbg_printf(XDBG_DEBUG_ERROR,
				"Read channel config failed %d\r\n", Status);

			return XST_FAILURE;
	}


	ReadCfg.FrameStoreStartAddr[0] = FrameStoreStartAddr;

	Status = XAxiVdma_DmaSetBufferAddr(&Vdma, XAXIVDMA_READ, ReadCfg.FrameStoreStartAddr);
	if (Status != XST_SUCCESS) {
			xdbg_printf(XDBG_DEBUG_ERROR,"Read channel set buffer address failed %d\r\n", Status);
			return XST_FAILURE;
	}


	Status = vdma_read_start(&Vdma);
	if (Status != XST_SUCCESS) {
		   xil_printf("error starting VDMA..!");
		   return Status;
	}
	return XST_SUCCESS;

}


int vdma_write_start(XAxiVdma *Vdma) {
	int Status;

	// MM2S Startup
	Status = XAxiVdma_DmaStart(Vdma, XAXIVDMA_WRITE);
	if (Status != XST_SUCCESS)
	{
	   xil_printf("Start write transfer failed %d\n\r", Status);
	   return XST_FAILURE;
	}

	return XST_SUCCESS;
}


int vdma_write_stop(XAxiVdma *Vdma) {
	XAxiVdma_DmaStop(Vdma, XAXIVDMA_WRITE);
	return XST_SUCCESS;
}


int vdma1_write_init(short DeviceID,short HoriSizeInput,short VertSizeInput,short Stride,unsigned int *FrameStoreStartAddr)
{
	XAxiVdma Vdma;
	XAxiVdma_Config *Config;
	XAxiVdma_DmaSetup WriteCfg;
	int Status;

	// ��ʼ��vdma

	Config = XAxiVdma_LookupConfig(DeviceID);
	if (NULL == Config) {
		xil_printf("XAxiVdma_LookupConfig failure\r\n");
		return XST_FAILURE;
	}

	Status = XAxiVdma_CfgInitialize(&Vdma, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("XAxiVdma_CfgInitialize failure\r\n");
		return XST_FAILURE;
	}

	

	//��ʼ������dma�Ĳ���
	WriteCfg.EnableCircularBuf = 1;
	WriteCfg.EnableFrameCounter = 0;
	WriteCfg.FixedFrameStoreAddr = 0;

	WriteCfg.EnableSync = 1;
	WriteCfg.PointNum = 1;

	WriteCfg.FrameDelay = 0;

	WriteCfg.VertSizeInput = VertSizeInput;
	WriteCfg.HoriSizeInput = HoriSizeInput;
	WriteCfg.Stride = Stride;

	xil_printf("VertSizeInput = %x\r\n",VertSizeInput);
	xil_printf("HoriSizeInput = %x\r\n",HoriSizeInput);
	xil_printf("Stride = %x\r\n",Stride);


	//ʹ�������������Ϣ������dmaͨ��
	Status = XAxiVdma_DmaConfig(&Vdma, XAXIVDMA_WRITE, &WriteCfg);
	if (Status != XST_SUCCESS) {
			xdbg_printf(XDBG_DEBUG_ERROR,
				"Read channel config failed %d\r\n", Status);

			return XST_FAILURE;
	}

	 int i;
	 for(i=0;i<4;i++)
    {
		 WriteCfg.FrameStoreStartAddr[i] = FrameStoreStartAddr[i];
		 xil_printf("FrameStoreStartAddr[%x] = %x\r\n",i,FrameStoreStartAddr[i]);
    }



	//��֡�洢�ĳ�ʼ��ַд�뵽vdma
	Status = XAxiVdma_DmaSetBufferAddr(&Vdma, XAXIVDMA_WRITE, WriteCfg.FrameStoreStartAddr);
	if (Status != XST_SUCCESS) {
			xdbg_printf(XDBG_DEBUG_ERROR,"Write channel set buffer address failed %d\r\n", Status);
			return XST_FAILURE;
	}

	//��ʼ�����ݰ��˵���ʾ������

	Status = vdma_write_start(&Vdma);
	if (Status != XST_SUCCESS) {
		   xil_printf("error starting VDMA..!");
		   return Status;
	}
	return XST_SUCCESS;

}

void vdma0_write_init(void)
{


	Xil_Out32(VDMA0_BASE_ADDR+S2MM_VDMACR_OFSET,0x8b);
	Xil_Out32(VDMA0_BASE_ADDR+S2MM_START_ADDRESS1_OFSET,VDMA_IMGCAPTURE_START_ADDR0);
	Xil_Out32(VDMA0_BASE_ADDR+S2MM_FRMDLY_STRIDE_OFSET,0xf00);
	Xil_Out32(VDMA0_BASE_ADDR+S2MM_HSIZE_OFSET,0xf00);
	Xil_Out32(VDMA0_BASE_ADDR+S2MM_VSIZE_OFSET,0x654);//0x21C

//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0x30, 0x4); //reset
//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0x30, 0x8); //genlock

//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0xAC, 0x17000000);
//	XAxiVdma_WriteReg(XPAR_VIDEO_PIPE_1_AXI_VDMA_1_BASEADDR, 0xAC+4, 0x0A000000);
//	XAxiVdma_WriteReg(XPAR_VIDEO_PIPE_1_AXI_VDMA_1_BASEADDR, 0xAC+8, 0x09000000);
//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0xA4, 0xf00);
//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0xA8, 0xf00);
//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0x30, 0x3);
//	XAxiVdma_WriteReg(VDMA0_BASE_ADDR, 0xA0, 0x654);


}


void vdma_imgStream_init(void)
{


	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_VDMACR_OFSET,0x8b);
	//Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_VDMASR_OFSET,0x4990);
	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_START_ADDRESS1_OFSET,VDMA_IMGSTREAM_START_ADDR1);//VDMA_IMGSTREAM_START_ADDR1
	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_START_ADDRESS2_OFSET,VDMA_IMGSTREAM_START_ADDR2);
	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_START_ADDRESS3_OFSET,VDMA_IMGSTREAM_START_ADDR3);
	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_START_ADDRESS4_OFSET,VDMA_IMGSTREAM_START_ADDR4);

	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_FRMDLY_STRIDE_OFSET,0xf00);
	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_HSIZE_OFSET,0xf00);
	Xil_Out32(VDMA_IMGSTREAM_BASE_ADDR+S2MM_VSIZE_OFSET,0x654);//0x21C

	xil_printf("VDMA_IMGSTREAM_START_ADDR1 = %x \r\n",VDMA_IMGSTREAM_START_ADDR1);
	xil_printf("VDMA_IMGSTREAM_START_ADDR2 = %x \r\n",VDMA_IMGSTREAM_START_ADDR2);
	xil_printf("VDMA_IMGSTREAM_START_ADDR3 = %x \r\n",VDMA_IMGSTREAM_START_ADDR3);
	xil_printf("VDMA_IMGSTREAM_START_ADDR4 = %x \r\n",VDMA_IMGSTREAM_START_ADDR4);


}

void vdma_imgcopy_init(void)
{



	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_VDMACR_OFSET,0x8b);
	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_START_ADDRESS1_OFSET,0x18000000);
//	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_START_ADDRESS2_OFSET,0x18000000);
//	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_START_ADDRESS3_OFSET,0x18000000);

	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_FRMDLY_STRIDE_OFSET,0x280);
	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_HSIZE_OFSET,0x280);
	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_VSIZE_OFSET,0x1E0);


	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+MM2S_VDMACR_OFSET,0x8b);

	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+MM2S_START_ADDRESS1_OFSET,0x17000000);
//	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_START_ADDRESS2_OFSET,0x17000000);
//	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+S2MM_START_ADDRESS3_OFSET,0x17000000);

	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+MM2S_FRMDLY_STRIDE_OFSET,0x280);
	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+MM2S_HSIZE_OFSET,0x280);
	Xil_Out32(VDMA_ImgCopy_BASE_ADDR+MM2S_VSIZE_OFSET,0x1E0);



}