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
	SPI_CS_WRITE(0);
	delay_us(1);
	SPI_CS_WRITE(1);
	delay_us(41);

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	command_strobe(SRES);
	SPI_CS_WRITE(1);
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
	command_strobe(SFTX);			// Flush TX FIFO buffer
	command_strobe(SFRX);			// Flush RX FIFO buffer
}

void write_tx_cc1101_config()
{
	write_single_byte(PATABLE, CC1101_POSTAVKA_PA_TABLE);
	write_single_byte(IOCFG0, CC1101_POSTAVKA_IOCFG0);
	write_single_byte(FIFOTHR, CC1101_POSTAVKA_FIFOTHR);
	write_single_byte(SYNC1, CC1101_POSTAVKA_SYNC1);
	write_single_byte(SYNC0, CC1101_POSTAVKA_SYNC0);
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
	write_single_byte(AGCCTRL2, CC1101_POSTAVKA_AGCCTRL2);
	write_single_byte(FSCAL3, CC1101_POSTAVKA_FSCAL3);
	write_single_byte(FSCAL2, CC1101_POSTAVKA_FSCAL2);
	write_single_byte(FSCAL1, CC1101_POSTAVKA_FSCAL1);
	write_single_byte(FSCAL0, CC1101_POSTAVKA_FSCAL0);
	write_single_byte(TEST2, CC1101_POSTAVKA_TEST2);
	write_single_byte(TEST1, CC1101_POSTAVKA_TEST1);
	write_single_byte(TEST0, CC1101_POSTAVKA_TEST0);
}

void write_rx_cc1101_config()
{
	write_single_byte(PATABLE, CC1101_POSTAVKA_PA_TABLE);
	write_single_byte(IOCFG2, CC1101_POSTAVKA_IOCFG2);
	write_single_byte(IOCFG0, CC1101_POSTAVKA_IOCFG0);
	write_single_byte(FIFOTHR, CC1101_POSTAVKA_FIFOTHR_RX);
	write_single_byte(SYNC1, CC1101_POSTAVKA_SYNC1);
	write_single_byte(SYNC0, CC1101_POSTAVKA_SYNC0);
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
	write_single_byte(MDMCFG1, CC1101_POSTAVKA_MDMCFG1);
	write_single_byte(DEVIATN, CC1101_POSTAVKA_DEVIATN);
	write_single_byte(MCSM0, CC1101_POSTAVKA_MCSM0);
	write_single_byte(FOCCFG, CC1101_POSTAVKA_FOCCFG);
	write_single_byte(AGCCTRL2, CC1101_POSTAVKA_AGCCTRL2);
	write_single_byte(FSCAL3, CC1101_POSTAVKA_FSCAL3);
	write_single_byte(FSCAL2, CC1101_POSTAVKA_FSCAL2);
	write_single_byte(FSCAL1, CC1101_POSTAVKA_FSCAL1);
	write_single_byte(FSCAL0, CC1101_POSTAVKA_FSCAL0);
	write_single_byte(TEST2, CC1101_POSTAVKA_TEST2);
	write_single_byte(TEST1, CC1101_POSTAVKA_TEST1);
	write_single_byte(TEST0, CC1101_POSTAVKA_TEST0);
}

uint8_t command_strobe(uint8_t addr)
{
	uint8_t status;
	delay_us(1000);					// Postavljen delay na pocetak, zbog mogucih krivih podataka od prijasnjeg strobe-a

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	status = HAL_SPI_TransmitReceive(&hspi2, &addr, &status, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);

	return status;
}

uint8_t read_status_reg(uint8_t addr, uint8_t *statusByte)
{
	uint8_t statusReg = addr | burst_byte_read;
	uint8_t statusRegValue;
	uint8_t txDummy = 0x01;

	if(addr >= 0x30 && addr <= 0x3D)
	{
		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
		HAL_SPI_TransmitReceive(&hspi2, &statusReg, statusByte, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);		//delay od 100ns
		HAL_SPI_TransmitReceive(&hspi2, &txDummy, &statusRegValue, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);		//delay od 100ns
		SPI_CS_WRITE(1);

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
	uint8_t len = 3;
	uint8_t statusByte;
	uint8_t TXOvf,TXregByte;
	uint8_t txData[3] = {0x02, 0xCA, 0xFF};

	command_strobe(SIDLE);			// IDLE
	command_strobe(SFTX);			// Flush TX FIFO

	write_burst_byte(TX_FIFO, &statusByte, txData, len);	//Napuni TX FIFO

	command_strobe(STX);			// Prebaci iz IDLE stanja u TX stanje
	delay_us(1000);			// Vrijeme potrebno za tranziciju IDLE->TX + TX->IDLE
	while(!HAL_GPIO_ReadPin(GDO0_GPIO_Port, GDO0_Pin));		//Pričekaj do slanja sync word-a
	while(HAL_GPIO_ReadPin(GDO0_GPIO_Port, GDO0_Pin));		// Pričekaj do kraja slanja payload-a

	TXregByte = read_status_reg(TXBYTES, &statusByte);
	TXOvf = TXregByte & 0x80;
	if(TXOvf == 0x80)
	{
		command_strobe(SFTX);
	}

	return statusByte;
}

uint8_t receive_burst_byte_433(uint8_t *rxData)
{
	uint32_t cnt = 0;
	uint8_t RX_FIFO = 0x3F;
	uint8_t statusByte,num_rxBytes,rxfifo_OF;

	command_strobe(SIDLE);									// IDLE
	command_strobe(SFRX);									// Flush RX FIFO

	command_strobe(SRX);									// Prebaci iz IDLE stanja u RX stanje
	delay_us(1000);											// Vrijeme potrebno za tranziciju IDLE->RX + RX->IDLE
	while(!HAL_GPIO_ReadPin(GDO0_GPIO_Port, GDO0_Pin))		//Pričekaj do primanja sync word-a
	{
		if(cnt==1000)
		{
			break;
		}
		delay_us(1000);
		cnt++;
	}
	cnt = 0;
	while(HAL_GPIO_ReadPin(GDO0_GPIO_Port, GDO0_Pin))		// Pričekaj do kraja primanja payload-a
	{
		if(cnt==1000)
		{
			break;
		}
		delay_us(1000);
		cnt++;
	}

	num_rxBytes = read_status_reg(RXBYTES, &statusByte);
	rxfifo_OF = num_rxBytes & 0x80;
	num_rxBytes = num_rxBytes & 0x7F;

	if(num_rxBytes == 5)
	{
		uint8_t data[num_rxBytes];
		read_burst_byte(RX_FIFO, &statusByte, data, num_rxBytes);
		for(int i = 0; i< num_rxBytes; i++)
		{
			rxData[i] = data[i];
			data[i] = 0;
		}

	}

	if(rxfifo_OF == 0x80)
	{
		command_strobe(SFRX);								// Flush RX FIFO
	}

	return num_rxBytes;
}

uint8_t read_single_byte(uint8_t addr, uint8_t *statusByte)
{
	uint8_t dataRx;
	uint8_t txData = addr | single_byte_read;
	uint8_t txDummy = 0x01;

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	HAL_SPI_TransmitReceive(&hspi2, &txData, statusByte, 1, HAL_MAX_DELAY);
	delay_6_25ns(16);										//delay od 100ns
	HAL_SPI_TransmitReceive(&hspi2, &txDummy, &dataRx, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);

	return dataRx;
}

uint8_t write_single_byte(uint8_t addr, uint8_t txData)
{
	uint8_t statusByte;
	uint8_t txAddr = addr | single_byte_write;
	uint8_t dataTx = txData;

	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	HAL_SPI_TransmitReceive(&hspi2, &txAddr, &statusByte, 1, HAL_MAX_DELAY);
	delay_6_25ns(16);										//delay od 100ns
	HAL_SPI_TransmitReceive(&hspi2, &dataTx, &statusByte, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);

	return statusByte;
}

void read_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *rxData, uint8_t len)
{
	uint8_t txDummy = 0x01;

	if(startAddr <= 0x2F || startAddr == 0x3E || startAddr == 0x3F)
	{
		uint8_t addr = startAddr | burst_byte_read;

		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
		HAL_SPI_TransmitReceive(&hspi2, &addr, statusByte, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);									//delay od 100ns
		if(len>64)
		{
			len=64;
		}
		for(int i=0; i < len; i++)
		{
			HAL_SPI_TransmitReceive(&hspi2, &txDummy, &rxData[i], 1, HAL_MAX_DELAY);
			if(i<(len-1))
			{
				delay_6_25ns(16);							//delay od 100ns, cc1101 datasheet pg.30
			}
			else
			{
				delay_6_25ns(4);							//delay od 25ns
			}
		}
		SPI_CS_WRITE(1);
	}
	else
	{
		*statusByte = 1;									// Error: 1, oznacava da je start addr u krivom rasponu
	}

}

void write_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *txData, uint8_t len)
{
	if(startAddr <= 0x2F || startAddr == 0x3E || startAddr == 0x3F)
	{
		uint8_t addr = startAddr | burst_byte_write;
		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
		HAL_SPI_TransmitReceive(&hspi2, &addr, statusByte, 1, HAL_MAX_DELAY);
		delay_6_25ns(16);									//delay od 100ns, cc1101 datasheet pg.30
		for(int i=0; i < len; i++)
		{
			HAL_SPI_TransmitReceive(&hspi2, &txData[i], statusByte, 1, HAL_MAX_DELAY);
			if(i<(len-1))
			{
				delay_6_25ns(16);							//delay od 100ns, cc1101 datasheet pg.30
			}
			else
			{
				delay_6_25ns(4);							//delay od 25ns
			}
		}
		SPI_CS_WRITE(1);
	}
	else
	{
		*statusByte = 1;									// Error: 1, oznacava da je start addr u krivom rasponu
	}
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
	HAL_UART_Transmit(&huart2, (uint8_t*)status_message, strlen(status_message), HAL_MAX_DELAY);
}

void hal_errorCode(uint32_t SPIerrorCode, HAL_StatusTypeDef spi_status1, HAL_StatusTypeDef spi_status2)
{
	if (spi_status1 != HAL_OK || spi_status2 != HAL_OK)
	{
		SPIerrorCode = HAL_SPI_GetError(&hspi2);
		char status_message[50];

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
