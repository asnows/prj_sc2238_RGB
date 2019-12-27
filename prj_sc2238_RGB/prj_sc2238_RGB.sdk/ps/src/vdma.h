
#ifndef VDMA_H_
#define VDMA_H_

#include "xaxivdma.h"




#define VDMA_IMGSTREAM_START_ADDR1  (0x15000000 + 0* IMG_BYTES)
#define VDMA_IMGSTREAM_START_ADDR2  (0x15000000 + 1* IMG_BYTES)
#define VDMA_IMGSTREAM_START_ADDR3  (0x15000000 + 2* IMG_BYTES)
#define VDMA_IMGSTREAM_START_ADDR4  (0x15000000 + 3* IMG_BYTES)

#define VDMA_IMGCAPTURE_START_ADDR0 0x17000000


#define  VDMA_IMGSTREAM_BASE_ADDR 	0x43000000
#define  VDMA0_BASE_ADDR     		0x43010000
#define  VDMA_ImgCopy_BASE_ADDR     0x43020000

#define  S2MM_VDMACR_OFSET          0x30
#define  S2MM_VDMASR_OFSET          0x34
#define  S2MM_VDMA_IRQ_MASK_OFSET   0x3c
#define  S2MM_REG_INDEX_OFSET       0x44
#define  S2MM_VSIZE_OFSET           0xA0
#define  S2MM_HSIZE_OFSET           0xA4
#define  S2MM_FRMDLY_STRIDE_OFSET   0xA8
#define  S2MM_START_ADDRESS1_OFSET  0xAC
#define  S2MM_START_ADDRESS2_OFSET  0xB0
#define  S2MM_START_ADDRESS3_OFSET  0xB4
#define  S2MM_START_ADDRESS4_OFSET  0xB8

#define  MM2S_VDMACR_OFSET          0x00
#define  MM2S_VDMASR_OFSET          0x04
//#define  MM2S_VDMA_IRQ_MASK_OFSET   0x3c
#define  MM2S_REG_INDEX_OFSET       0x14
#define  MM2S_VSIZE_OFSET           0x50
#define  MM2S_HSIZE_OFSET           0x54
#define  MM2S_FRMDLY_STRIDE_OFSET   0x58
#define  MM2S_START_ADDRESS1_OFSET  0x5C
#define  MM2S_START_ADDRESS2_OFSET  0x60
#define  MM2S_START_ADDRESS3_OFSET  0x64


int vdma_read_init(short DeviceID,short HoriSizeInput,short VertSizeInput,short Stride,unsigned int FrameStoreStartAddr);
int vdma1_write_init(short DeviceID,short HoriSizeInput,short VertSizeInput,short Stride,unsigned int *FrameStoreStartAddr);
void vdma0_write_init(void);
void vdma_imgcopy_init(void);
void vdma_imgStream_init(void);
u32 vdma_version();

#endif /* VDMA_H_ */
