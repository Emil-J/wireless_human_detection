#include "CC1101_E07_M1101D_SMA.h"

void SPI_CS_WRITE(bool state)	 // State machine za CS
{
	if(state)
	  {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	  }
	else
	  {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
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

void cc1101_init()
{
	write_cc1101_config();		// Konfiguracija cc1101 čipa
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


void write_cc1101_config()
{
	write_single_byte(PATABLE, 0x60);
	write_single_byte(IOCFG0, CC1101_POSTAVKA_IOCFG0);
	write_single_byte(FIFOTHR, CC1101_POSTAVKA_FIFOTHR);
	write_single_byte(PKTLEN, CC1101_POSTAVKA_PKTLEN);
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
	write_single_byte(WORCTRL, CC1101_POSTAVKA_WORCTRL);
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
	delay_us(1000);					// Postavljen delay na pocetak, zbog mogucih krivih podataka od prijasnjeg strobe-a
	HAL_StatusTypeDef SPI_status;
	char status_message[15];

	uint8_t status;
	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
	SPI_status = HAL_SPI_TransmitReceive(&hspi2, &addr, &status, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);
	switch(SPI_status)
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

	return status;
}

// TREBAM JOS ODVOJENE FUNKCIJE ZA TX I RX FIFO I PATABLE!
uint8_t read_status_reg(uint8_t addr, uint8_t *statusByte)
{
	uint8_t statusReg = addr | burst_byte_read;
	uint8_t statusRegValue;
	if(addr >= 0x30 && addr <= 0x3D)
	{
		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
			delay_25ns(1);		//delay od 25ns
		}
		HAL_SPI_TransmitReceive(&hspi2, &statusReg, statusByte, 1, HAL_MAX_DELAY);
		delay_25ns(4);		//delay od 100ns, cc1101 datasheet pg.30
		HAL_SPI_TransmitReceive(&hspi2, NULL, &statusRegValue, 1, HAL_MAX_DELAY);
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
	uint8_t len = 2;
	uint8_t statusByte;
	uint8_t mask = 0x70;
	char serialStatus[50];
	uint8_t txData[2] = {0x11, 0xFF};
	command_strobe(SFTX);			// Flush TX FIFO
	write_burst_byte(TX_FIFO, &statusByte, txData, len);	//Napuni TX FIFO
	statusByte = command_strobe(STX);			// Prebaci iz IDLE stanja u TX stanje
	delay_us(800);					// Vrijeme potrebno za tranziciju IDLE->TX + TX->IDLE

	statusByte = command_strobe(SNOP);

	snprintf(serialStatus, sizeof(serialStatus), "loop tx: 0x%02X\r\n", statusByte);
	HAL_UART_Transmit(&huart2, (uint8_t*)serialStatus, strlen(serialStatus), HAL_MAX_DELAY);

	mask = statusByte & mask;
	if(mask == 0x70)
	{
		statusByte = command_strobe(SFTX);
	}
	return statusByte;
}

//uint8_t receive_burst_byte_433()
//{
//
//}

uint8_t read_single_byte(uint8_t addr, uint8_t *statusByte)
{
	uint8_t dataRx;
	uint8_t txData = addr | single_byte_read;
	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2))
	{
		delay_25ns(1);		//delay od 25ns
	}
	HAL_SPI_TransmitReceive(&hspi2, &txData, statusByte, 1, HAL_MAX_DELAY);
	delay_25ns(3);		//delay od 75ns, cc1101 datasheet pg.30
	HAL_SPI_TransmitReceive(&hspi2, NULL, &dataRx, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);
	return dataRx;
}

uint8_t write_single_byte(uint8_t addr, uint8_t txData)
{
	uint8_t statusByte;
	uint8_t txAddr = addr | single_byte_write;
	uint8_t dataTx = txData;
	SPI_CS_WRITE(0);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
		delay_25ns(1);		//delay od 25ns
	}
	HAL_SPI_TransmitReceive(&hspi2, &txAddr, &statusByte, 1, HAL_MAX_DELAY);
	delay_25ns(3);		//delay od 75ns, cc1101 datasheet pg.30
	HAL_SPI_TransmitReceive(&hspi2, &dataTx, &statusByte, 1, HAL_MAX_DELAY);
	SPI_CS_WRITE(1);
	return statusByte;
}

void read_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *rxData, uint8_t len)
{
	if(startAddr <= 0x2F || startAddr == 0x3E || startAddr == 0x3F)
	{
		uint8_t addr = startAddr | burst_byte_read;
		SPI_CS_WRITE(0);
		while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
			delay_25ns(1);		//delay od 25ns
		}
		HAL_SPI_TransmitReceive(&hspi2, &addr, statusByte, 1, HAL_MAX_DELAY);
		delay_25ns(4);		//delay od 100ns, cc1101 datasheet pg.30
		for(int i=0; i < len; i++)
		{
			HAL_SPI_TransmitReceive(&hspi2, NULL, &rxData[i], 1, HAL_MAX_DELAY);
			if(i<(len-1))
			{
				delay_25ns(4);		//delay od 100ns, cc1101 datasheet pg.30
			}
			else
			{
				delay_25ns(1);
			}
		}
		SPI_CS_WRITE(1);
	}
	else
	{
		*statusByte = 1;		// Error: 1, oznacava da je start addr u krivom rasponu
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
//		delay_25ns(4);		//delay od 100ns, cc1101 datasheet pg.30
		for(int i=0; i < len; i++)
		{
			HAL_SPI_TransmitReceive(&hspi2, &txData[i], statusByte, 1, HAL_MAX_DELAY);
//			if(i<(len-1))
//			{
//				delay_25ns(4);		//delay od 100ns, cc1101 datasheet pg.30
//			}
//			else
//			{
//				delay_25ns(1);
//			}
		}
		SPI_CS_WRITE(1);
	}
	else
	{
		*statusByte = 1;		// Error: 1, oznacava da je start addr u krivom rasponu
	}
}
