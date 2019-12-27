/*
 * xemacps_phy.c
 *
 *  Created on: 2019Äê5ÔÂ13ÈÕ
 *      Author: Administrator
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xemacps_hw.h"
#include"xemacps_phy.h"

void XEmacPs_PhyRead(u32 PhyAddress,u32 RegisterNum, u16 *PhyDataPtr)
{

	u32 Mgtcr;
	volatile u32 Ipisr;
	u32 IpReadTemp;
	/* Make sure no other PHY operation is currently in progress */
	unsigned int status;
	status = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWSR_OFFSET);

	if(!(status & XEMACPS_NWSR_MDIOIDLE_MASK))
	{

		xil_printf("PHY management not idle! \r\n");
	}
	xil_printf("PHY management idle! \r\n");

	/* Construct Mgtcr mask for the operation */
	Mgtcr = XEMACPS_PHYMNTNC_OP_MASK | XEMACPS_PHYMNTNC_OP_R_MASK |
			(PhyAddress << XEMACPS_PHYMNTNC_PHAD_SHFT_MSK) |
			(RegisterNum << XEMACPS_PHYMNTNC_PREG_SHFT_MSK);

	/* Write Mgtcr and wait for completion */
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_PHYMNTNC_OFFSET,Mgtcr);

	do {

			Ipisr = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWSR_OFFSET);
			IpReadTemp = Ipisr;
		} while ((IpReadTemp & XEMACPS_NWSR_MDIOIDLE_MASK) == 0x00000000U);

	*PhyDataPtr = (u16)Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_PHYMNTNC_OFFSET);


}


void XEmacPs_PhyWrite(u32 PhyAddress,u32 RegisterNum, u16 PhyData)
{
	u32 Mgtcr;
	volatile u32 Ipisr;
	u32 IpReadTemp;
	/* Make sure no other PHY operation is currently in progress */
	unsigned int status;
	status = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWSR_OFFSET);

	if(!(status & XEMACPS_NWSR_MDIOIDLE_MASK))
	{

		xil_printf("PHY management not idle! \r\n");
	}
	xil_printf("PHY management idle! \r\n");

	/* Construct Mgtcr mask for the operation */
	Mgtcr = XEMACPS_PHYMNTNC_OP_MASK | XEMACPS_PHYMNTNC_OP_W_MASK |
			(PhyAddress << XEMACPS_PHYMNTNC_PHAD_SHFT_MSK) |
			(RegisterNum << XEMACPS_PHYMNTNC_PREG_SHFT_MSK) | (u32)PhyData;

	/* Write Mgtcr and wait for completion */
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_PHYMNTNC_OFFSET,Mgtcr);



	/* Construct Mgtcr mask for the operation */
	Mgtcr = XEMACPS_PHYMNTNC_OP_MASK | XEMACPS_PHYMNTNC_OP_W_MASK |
			(PhyAddress << XEMACPS_PHYMNTNC_PHAD_SHFT_MSK) |
			(RegisterNum << XEMACPS_PHYMNTNC_PREG_SHFT_MSK) | (u32)PhyData;

	/* Write Mgtcr and wait for completion */
	Xil_Out32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_PHYMNTNC_OFFSET,Mgtcr);

	do {
			Ipisr = Xil_In32(XEMACPS_GEM0_BAYSE_ADDR + XEMACPS_NWSR_OFFSET);
			IpReadTemp = Ipisr;
		} while ((IpReadTemp & XEMACPS_NWSR_MDIOIDLE_MASK) == 0x00000000U);

}




static u32 get_phy_speed_ksz9031()
{
	u16 temp;
	u16 control;
	u16 status;
	u16 status_speed;
	u32 timeout_counter = 0;
	//u32 temp_speed;
	//u32 phyregtemp;
	u32 phy_addr = PHY_ADDRESS;

	xil_printf("Start PHY autonegotiation \r\n");

	XEmacPs_PhyWrite(phy_addr, IEEE_PAGE_ADDRESS_REGISTER, 2);
	XEmacPs_PhyRead( phy_addr, IEEE_CONTROL_REG_MAC, &control);
	control |= IEEE_RGMII_TXRX_CLOCK_DELAYED_MASK;
	XEmacPs_PhyWrite( phy_addr, IEEE_CONTROL_REG_MAC, control);

	XEmacPs_PhyWrite( phy_addr, IEEE_PAGE_ADDRESS_REGISTER, 0);

	XEmacPs_PhyRead( phy_addr, IEEE_AUTONEGO_ADVERTISE_REG, &control);
	control |= IEEE_ASYMMETRIC_PAUSE_MASK;
	control |= IEEE_PAUSE_MASK;
	control |= ADVERTISE_100;
	control |= ADVERTISE_10;
	XEmacPs_PhyWrite( phy_addr, IEEE_AUTONEGO_ADVERTISE_REG, control);

	XEmacPs_PhyRead( phy_addr, IEEE_1000_ADVERTISE_REG_OFFSET,
					&control);
	control |= ADVERTISE_1000;
	XEmacPs_PhyWrite( phy_addr, IEEE_1000_ADVERTISE_REG_OFFSET,
					control);

	XEmacPs_PhyWrite( phy_addr, IEEE_PAGE_ADDRESS_REGISTER, 0);
	XEmacPs_PhyRead( phy_addr, IEEE_COPPER_SPECIFIC_CONTROL_REG,
																&control);
	control |= (7 << 12);	/* max number of gigabit attempts */
	control |= (1 << 11);	/* enable downshift */
	XEmacPs_PhyWrite( phy_addr, IEEE_COPPER_SPECIFIC_CONTROL_REG,
																control);
	XEmacPs_PhyRead( phy_addr, IEEE_CONTROL_REG_OFFSET, &control);
	control |= IEEE_CTRL_AUTONEGOTIATE_ENABLE;
	control |= IEEE_STAT_AUTONEGOTIATE_RESTART;
	XEmacPs_PhyWrite( phy_addr, IEEE_CONTROL_REG_OFFSET, control);

	XEmacPs_PhyRead( phy_addr, IEEE_CONTROL_REG_OFFSET, &control);
	control |= IEEE_CTRL_RESET_MASK;
	XEmacPs_PhyWrite( phy_addr, IEEE_CONTROL_REG_OFFSET, control);

	while (1) {
		XEmacPs_PhyRead( phy_addr, IEEE_CONTROL_REG_OFFSET, &control);
		if (control & IEEE_CTRL_RESET_MASK)
			continue;
		else
			break;
	}

	XEmacPs_PhyRead( phy_addr, IEEE_STATUS_REG_OFFSET, &status);

	xil_printf("Waiting for PHY to complete autonegotiation.\r\n");

	while ( !(status & IEEE_STAT_AUTONEGOTIATE_COMPLETE) ) {
		sleep(1);
		XEmacPs_PhyRead( phy_addr,
						IEEE_COPPER_SPECIFIC_STATUS_REG_2,  &temp);
		timeout_counter++;

		if (timeout_counter == 30) {
			xil_printf("Auto negotiation error \r\n");
			return XST_FAILURE;
		}
		XEmacPs_PhyRead( phy_addr, IEEE_STATUS_REG_OFFSET, &status);
	}
	xil_printf("autonegotiation complete \r\n");

	XEmacPs_PhyRead( phy_addr,0x1f,
					&status_speed);

	if ( (status_speed & 0x40) == 0x40)/* 1000Mbps */
		return 1000;
	else if ( (status_speed & 0x20) == 0x20)/* 100Mbps */
		return 100;
	else if ( (status_speed & 0x10) == 0x10)/* 10Mbps */
		return 10;
	else
		return 0;
	return XST_SUCCESS;
}

void init_phy()
{
	u32 link_speed;

	link_speed = get_phy_speed_ksz9031();
	xil_printf("link speed  %d\r\n",link_speed);

}
