#include "CC1101_E07_M1101D_SMA.h"

void SPI_CS_WRITE(bool state)	 // State machine za CS
{
	if(state)
	  {
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
	  }
	else
	  {
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
	  }
}

void manual_POR()
{
	uint8_t status = 0x00;
	char serialStatus[10];

	SPI_CS_WRITE(0);
	delay_us(1);
	SPI_CS_WRITE(1);
	delay_us(41);

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	status = command_strobe(SRES);
	SPI_CS_WRITE(1);

	snprintf(serialStatus, sizeof(serialStatus), "0x%02X\r\n", status);
	HAL_UART_Transmit(&huart2, (uint8_t*)serialStatus, strlen(serialStatus), HAL_MAX_DELAY);
}

void cc1101_init(bool state)
{
	if(state)
	{
		write_tx_cc1101_config();	// Konfiguracija tx cc1101 čipa
	}
	else
	{
		write_rx_cc1101_config();	// Konfiguracija rx cc1101 čipa
	}
	command_strobe(SFTX);		// Flush TX FIFO buffer
	command_strobe(SFRX);		// Flush RX FIFO buffer

}
// Prilikom prelaženja u sleep stanje, ovi registri izgube svoje default vrijednosti.
// Stoga prilikom povratka iz sleep stanja u IDLE stanje, rekonfiguracijska funkcija je potrebna.
void write_cc1101_config_sleep()
{
	write_single_byte(FSTEST, CC1101_POSTAVKA_FSTEST);
	write_single_byte(PTEST, CC1101_POSTAVKA_PTEST);
	write_single_byte(AGCTEST, CC1101_POSTAVKA_AGCTEST);
	write_single_byte(TEST2, CC1101_POSTAVKA_TEST2);
	write_single_byte(TEST1, CC1101_POSTAVKA_TEST1);
	write_single_byte(TEST0, CC1101_POSTAVKA_TEST0);
}


void write_tx_cc1101_config()
{
	write_single_byte(PATABLE, 0x60);
	write_single_byte(IOCFG0, CC1101_POSTAVKA_IOCFG0);
	write_single_byte(FIFOTHR, CC1101_POSTAVKA_FIFOTHR);
	write_single_byte(PKTLEN, CC1101_POSTAVKA_PKTLEN);
	write_single_byte(PKTCTRL0, CC1101_POSTAVKA_PKTCTRL0);
	write_single_byte(FSCTRL1, CC1101_POSTAVKA_FSCTRL1);
	write_single_byte(FREQ2, CC1101_POSTAVKA_FREQ2);
	write_single_byte(FREQ1, CC1101_POSTAVKA_FREQ1);
	write_single_byte(FREQ0, CC1101_POSTAVKA_FREQ0);
	write_single_byte(MDMCFG4, CC1101_POSTAVKA_MDMCFG4);
	write_single_byte(MDMCFG3, CC1101_POSTAVKA_MDMCFG3);
	write_single_byte(MDMCFG2, CC1101_POSTAVKA_MDMCFG2);
	write_single_byte(MDMCFG1, CC1101_POSTAVKA_MDMCFG1);
	write_single_byte(DEVIATN, CC1101_POSTAVKA_DEVIATN);
	write_single_byte(MCSM0, CC1101_POSTAVKA_MCSM0);
	write_single_byte(FOCCFG, CC1101_POSTAVKA_FOCCFG);
	write_single_byte(WORCTRL, CC1101_POSTAVKA_WORCTRL);
	write_single_byte(FSCAL3, CC1101_POSTAVKA_FSCAL3);
	write_single_byte(FSCAL2, CC1101_POSTAVKA_FSCAL2);
	write_single_byte(FSCAL1, CC1101_POSTAVKA_FSCAL1);
	write_single_byte(FSCAL0, CC1101_POSTAVKA_FSCAL0);
	write_single_byte(FSTEST, CC1101_POSTAVKA_FSTEST);
	write_single_byte(PTEST, CC1101_POSTAVKA_PTEST);
	write_single_byte(AGCTEST, CC1101_POSTAVKA_AGCTEST);
	write_single_byte(TEST2, CC1101_POSTAVKA_TEST2);
	write_single_byte(TEST1, CC1101_POSTAVKA_TEST1);
	write_single_byte(TEST0, CC1101_POSTAVKA_TEST0);

}

void write_rx_cc1101_config()
{
	write_single_byte(PATABLE, 0x60);
	write_single_byte(IOCFG0, CC1101_POSTAVKA_IOCFG0);
	write_single_byte(FIFOTHR, CC1101_POSTAVKA_FIFOTHR);
	write_single_byte(PKTLEN, CC1101_POSTAVKA_PKTLEN);
	write_single_byte(PKTCTRL1, CC1101_POSTAVKA_PKTCTRL1);
	write_single_byte(PKTCTRL0, CC1101_POSTAVKA_PKTCTRL0);
	write_single_byte(ADDR, CC1101_POSTAVKA_ADDR);
	write_single_byte(FSCTRL1, CC1101_POSTAVKA_FSCTRL1);
	write_single_byte(FREQ2, CC1101_POSTAVKA_FREQ2);
	write_single_byte(FREQ1, CC1101_POSTAVKA_FREQ1);
	write_single_byte(FREQ0, CC1101_POSTAVKA_FREQ0);
	write_single_byte(MDMCFG4, CC1101_POSTAVKA_MDMCFG4);
	write_single_byte(MDMCFG3, CC1101_POSTAVKA_MDMCFG3);
	write_single_byte(MDMCFG2, CC1101_POSTAVKA_MDMCFG2);
	write_single_byte(DEVIATN, CC1101_POSTAVKA_DEVIATN);
	write_single_byte(MCSM0, CC1101_POSTAVKA_MCSM0);
	write_single_byte(FOCCFG, CC1101_POSTAVKA_FOCCFG);
	write_single_byte(WORCTRL, CC1101_POSTAVKA_WORCTRL);
	write_single_byte(FSCAL3, CC1101_POSTAVKA_FSCAL3);
	write_single_byte(FSCAL2, CC1101_POSTAVKA_FSCAL2);
	write_single_byte(FSCAL1, CC1101_POSTAVKA_FSCAL1);
	write_single_byte(FSCAL0, CC1101_POSTAVKA_FSCAL0);
	//	fstest
	//	ptest
	//	agctest
	write_single_byte(TEST2, CC1101_POSTAVKA_TEST2);
	write_single_byte(TEST1, CC1101_POSTAVKA_TEST1);
	write_single_byte(TEST0, CC1101_POSTAVKA_TEST0);

}

uint8_t command_strobe(uint8_t addr)
{
	delay_us(1000);					// Postavljen delay na pocetak, zbog mogucih krivih podataka od prijasnjeg strobe-a
	HAL_StatusTypeDef SPI_status;
	uint8_t status;

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	SPI_status = HAL_SPI_TransmitReceive(&hspi2, &addr, &status, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);
	hal_status(SPI_status);

	return status;
}

// TREBAM JOS ODVOJENE FUNKCIJE ZA TX I RX FIFO I PATABLE!
uint8_t read_status_reg(uint8_t addr, uint8_t *statusByte)
{
	uint8_t statusReg = addr | burst_byte_read;
	uint8_t statusRegValue;
	uint8_t txDummy = 0x01;
	HAL_StatusTypeDef SPI_status,SPI_status1;

	if(addr >= 0x30 && addr <= 0x3D)
	{
		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
		SPI_status = HAL_SPI_TransmitReceive(&hspi2, &statusReg, statusByte, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);		//delay od 100ns
		SPI_status1 = HAL_SPI_TransmitReceive(&hspi2, &txDummy, &statusRegValue, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);		//delay od 100ns
		SPI_CS_WRITE(1);

		hal_status(SPI_status);
		hal_status(SPI_status1);

		return statusRegValue;
	}
	else
	{
		return 1;		// Error: 1, oznacava da je start addr u krivom rasponu
	}
}

uint8_t transmit_burst_byte_433()	// Čovjek je prisutan
{
	uint8_t TX_FIFO = 0x3F;
	uint8_t len = 2;
	uint8_t statusByte;
	uint8_t txData[2] = {0x11, 0xFF};
	command_strobe(SFTX);			// Flush TX FIFO
	write_burst_byte(TX_FIFO, &statusByte, txData, len);	//Napuni TX FIFO
	command_strobe(STX);			// Prebaci iz IDLE stanja u TX stanje
	delay_us(1000);			// Vrijeme potrebno za tranziciju IDLE->RX + RX->IDLE

	return statusByte;
}

uint8_t receive_burst_byte_433(uint8_t *rxData)
{
	uint8_t statusByte;
	char status_message[50];
	uint8_t RX_FIFO = 0x3F;
	uint8_t statusRXByte1,statusRXByte2,statusRXByte3,statusRXByte4;
	uint8_t num_rxBytes,num_rxBytes1,num_rxBytes2;
	uint8_t marcstate1,marcstate2,rxfifo_OF1,rxfifo_OF2;


	command_strobe(SIDLE);			// IDLE
	command_strobe(SFRX);			// Flush RX FIFO

	snprintf(status_message, sizeof(status_message), "Prosao sam IDLE & FLUSH\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	num_rxBytes1 = read_status_reg(RXBYTES, &statusRXByte1);
	rxfifo_OF1 = num_rxBytes1 & 0x80;
	num_rxBytes1 = num_rxBytes1 & 0x7F;
	num_rxBytes = num_rxBytes1;


	marcstate1 = read_status_reg(MARCSTATE, &statusRXByte2);
	marcstate1 = marcstate1 & 0x1F;

	//statusByte = command_strobe(SRX);			// Prebaci iz IDLE stanja u RX stanje
	//delay_6_25ns(2000);							// Vrijeme potrebno za tranziciju IDLE->RX + RX->IDLE
	//HAL_Delay(2000);				// Pošto je brzina 1.2kBaud, cca. 5 sekundi treba za +/-10 byte-a podataka.

	command_strobe(SRX);			// Prebaci iz IDLE stanja u RX stanje
	delay_us(1000);			// Vrijeme potrebno za tranziciju IDLE->RX + RX->IDLE
	while(!HAL_GPIO_ReadPin(GDO0_GPIO_Port, GDO0_Pin));		//Pričekaj do slanja sync word-a
	while(HAL_GPIO_ReadPin(GDO0_GPIO_Port, GDO0_Pin));		// Pričekaj do kraja slanja payload-a

	while(num_rxBytes < 5)
	{

		num_rxBytes2 = read_status_reg(RXBYTES, &statusRXByte3);
		rxfifo_OF2 = num_rxBytes2 & 0x80;
		num_rxBytes2 = num_rxBytes2 & 0x7F;
		num_rxBytes = num_rxBytes2;

		marcstate2 = read_status_reg(MARCSTATE, &statusRXByte4);
		marcstate2 = marcstate2 & 0x1F;

		if(num_rxBytes>=2)
		{
			read_burst_byte(RX_FIFO, &statusByte, rxData, num_rxBytes);	//Napuni TX FIFO
			delay_us(1000);					// Vrijeme potrebno za tranziciju IDLE->RX + RX->IDLE

		}
	}

	command_strobe(SIDLE);			// IDLE
	HAL_Delay(1000);
	command_strobe(SFRX);			// Flush RX FIFO
	HAL_Delay(1000);

	sprintf(status_message, "RXBYTES ChipStatus before: 0x%02X\r\n", statusRXByte1);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "RXFIFO OVERFLOW before: 0x%02X\r\n", rxfifo_OF1);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "No. RXBYTES before: %d\r\n", num_rxBytes1);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);


	sprintf(status_message, "MARCSTATE before: 0x%02X\r\n", marcstate1);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "MARCSTATE ChipStatus before: 0x%02X\r\n", statusRXByte2);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);


	sprintf(status_message, "No. RXBYTES after: %d\r\n", num_rxBytes2);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "RXFIFO OVERFLOW after: 0x%02X\r\n", rxfifo_OF2);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "RXBYTES ChipStatus after: 0x%02X\r\n", statusRXByte3);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "MARCSTATE after: 0x%02X\r\n", marcstate2);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	sprintf(status_message, "MARCSTATE ChipStatus after: 0x%02X\r\n", statusRXByte4);
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);

	return num_rxBytes;
}

uint8_t read_single_byte(uint8_t addr, uint8_t *statusByte)
{
	uint8_t dataRx;
	uint8_t txData = addr | single_byte_read;
	uint8_t txDummy = 0x01;
	HAL_StatusTypeDef SPI_status1,SPI_status2;

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	SPI_status1 = HAL_SPI_TransmitReceive(&hspi2, &txData, statusByte, 1, HAL_MAX_DELAY);
	delay_6_25ns(16);		//delay od 100ns
	SPI_status2 = HAL_SPI_TransmitReceive(&hspi2, &txDummy, &dataRx, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);
	hal_status(SPI_status1);
	hal_status(SPI_status2);

	return dataRx;
}

uint8_t write_single_byte(uint8_t addr, uint8_t txData)
{
	uint8_t statusByte;
	uint8_t txAddr = addr | single_byte_write;
	uint8_t dataTx = txData;
	HAL_StatusTypeDef SPI_status1,SPI_status2;

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	SPI_status1 = HAL_SPI_TransmitReceive(&hspi2, &txAddr, &statusByte, 1, HAL_MAX_DELAY);
	delay_6_25ns(16);		//delay od 100ns
	SPI_status2 = HAL_SPI_TransmitReceive(&hspi2, &dataTx, &statusByte, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);

	hal_status(SPI_status1);
	hal_status(SPI_status2);

	return statusByte;
}

void read_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *rxData, uint8_t len)
{
	char status_message[100];
	uint8_t txDummy = 0x01;
	HAL_StatusTypeDef SPI_status,SPI_status1;

	if(startAddr <= 0x2F || startAddr == 0x3E || startAddr == 0x3F)
	{
		uint8_t addr = startAddr | burst_byte_read;

		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
		SPI_status = HAL_SPI_TransmitReceive(&hspi2, &addr, statusByte, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);		//delay od 100ns
		if(len>64)
		{
			len=64;
		}
		for(int i=0; i < len; i++)
		{
			SPI_status1 = HAL_SPI_TransmitReceive(&hspi2, &txDummy, &rxData[i], 1, HAL_MAX_DELAY);
			if(i<(len-1))
			{
				delay_6_25ns(16);		//delay od 100ns, cc1101 datasheet pg.30
			}
			else
			{
				delay_6_25ns(4);		//delay od 25ns
			}
		}
		SPI_CS_WRITE(1);
	}
	else
	{
		*statusByte = 1;		// Error: 1, oznacava da je start addr u krivom rasponu
	}


	// UART PRINTOUT DEBUG
	if(len>64)
	{
		len=64;
		for(int i=0; i < len; i++)
		{
			sprintf(status_message, "rxData: 0x%02X\r\n", rxData[i]);
			HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);
		}
	}
	hal_status(SPI_status);
	hal_status(SPI_status1);

}

void write_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *txData, uint8_t len)
{
	char status_message[100];
	HAL_StatusTypeDef SPI_status,SPI_status1;

	if(startAddr <= 0x2F || startAddr == 0x3E || startAddr == 0x3F)
	{
		uint8_t addr = startAddr | burst_byte_write;
		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
		SPI_status = HAL_SPI_TransmitReceive(&hspi2, &addr, statusByte, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);		//delay od 100ns, cc1101 datasheet pg.30
		for(int i=0; i < len; i++)
		{
			SPI_status1 = HAL_SPI_TransmitReceive(&hspi2, &txData[i], statusByte, 1, HAL_MAX_DELAY);
			if(i<(len-1))
			{
				delay_6_25ns(16);		//delay od 100ns, cc1101 datasheet pg.30
			}
			else
			{
				delay_6_25ns(4);		//delay od 25ns
			}
		}
		SPI_CS_WRITE(1);
	}
	else
	{
		*statusByte = 1;		// Error: 1, oznacava da je start addr u krivom rasponu
	}

	// UART PRINTOUT DEBUG
	if(len>64)
	{
		len=64;
		for(int i=0; i < len; i++)
		{
			sprintf(status_message, "txData: 0x%02X\r\n", txData[i]);
			HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);
		}
	}
	hal_status(SPI_status);
	hal_status(SPI_status1);
}

void hal_status(HAL_StatusTypeDef SPIstatus)
{
	char status_message[50];
	switch(SPIstatus)
		{
			case HAL_OK:
				sprintf(status_message, "HAL OK.\r\n");
				break;
			case HAL_ERROR:
				sprintf(status_message, "HAL ERROR.\r\n");
				break;
			case HAL_BUSY:
				sprintf(status_message, "HAL BUSY.\r\n");
				break;
			case HAL_TIMEOUT:
				sprintf(status_message, "HAL TIMEOUT.\r\n");
				break;
		}
	//HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);
}

void hal_errorCode(uint32_t SPIerrorCode, HAL_StatusTypeDef spi_status1, HAL_StatusTypeDef spi_status2)
{
	if (spi_status1 != HAL_OK || spi_status2 != HAL_OK)
	{
		SPIerrorCode = HAL_SPI_GetError(&hspi2);
		char status_message[50]; // Adjust size as necessary

		// Handle specific errors
		switch(SPIerrorCode)
		{
			case HAL_SPI_ERROR_NONE:
				snprintf(status_message, sizeof(status_message), "No error\r\n");
				break;
			case HAL_SPI_ERROR_MODF:
				snprintf(status_message, sizeof(status_message), "Mode Fault error\r\n");
				break;
			case HAL_SPI_ERROR_CRC:
				snprintf(status_message, sizeof(status_message), "CRC error\r\n");
				break;
			case HAL_SPI_ERROR_OVR:
				snprintf(status_message, sizeof(status_message), "Overrun error\r\n");
				break;
			case HAL_SPI_ERROR_FRE:
				snprintf(status_message, sizeof(status_message), "Frame error\r\n");
				break;
			case HAL_SPI_ERROR_DMA:
				snprintf(status_message, sizeof(status_message), "DMA transfer error\r\n");
				break;
			case HAL_SPI_ERROR_FLAG:
				snprintf(status_message, sizeof(status_message), "Flag error: RXNE, TXE, or BSY\r\n");
				break;
			case HAL_SPI_ERROR_ABORT:
				snprintf(status_message, sizeof(status_message), "Error during SPI Abort procedure\r\n");
				break;
		}
		HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);
		HAL_Delay(2500);
	}
}
