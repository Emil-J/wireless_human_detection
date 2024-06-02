/*
 * Ebyte E07 M1101D SMA Transceiver modul.
 * Zasnovan na Texas Instruments CC1101 433MHz čipu.
 *
 * Autor: Emil Jagnić
 * Izrađeno: 16.05.2024
 *
 */

#ifndef CC1101_E07_M1101D_SMA_h
#define CC1101_E07_M1101D_SMA_h

/*
 *
 *  Pozvani header fileovi zbog gpio defineova, hal librarya i config-a uart-a i spi-ja
 *
 */
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"
#include "stdbool.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

/*
 *
 * Define-ovi
 *
 */
#define single_byte_write	0x00
#define single_byte_read	0x80
#define burst_byte_write	0x40
#define burst_byte_read		0xC0

/*
 *
 * RF Postavke za konfiguriranje CC1101 čipa
 * Stvoreno uz pomoć Texas Instruments SmartRF 7 v2.30 programa i CC1101 tehničke dokumentacije.
 *
 */
/* Address Config = No address check */
/* Base Frequency = 433.919830 */
/* CRC Autoflush = false */
/* CRC Enable = true */
/* Carrier Frequency = 433.919830 */
/* Channel Number = 0 */
/* Channel Spacing = 199.951172 */
/* Data Format = Normal mode */
/* Data Rate = 1.19948 */
/* Deviation = 5.157471 */
/* Device Address = 0 */
/* Manchester Enable = false */
/* Modulated = true */
/* Modulation Format = GFSK */
/* PA Ramping = false */
/* Packet Length = 2 */
/* Packet Length Mode = Fixed packet length mode. Length configured in PKTLEN register */
/* Preamble Count = 4 */
/* RX Filter BW = 58.035714 */
/* Sync Word Qualifier Mode = 15/16 sync word bits detected */
/* TX Power = 0 */
/* Whitening = true */
/* PA table */
#define PA_TABLE {0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00}

#define CC1101_POSTAVKA_IOCFG0     0x06
#define CC1101_POSTAVKA_FIFOTHR    0x47
#define CC1101_POSTAVKA_PKTLEN     0x05
#define CC1101_POSTAVKA_PKTCTRL1   0x05
#define CC1101_POSTAVKA_PKTCTRL0   0x44
#define CC1101_POSTAVKA_ADDR       0x11
#define CC1101_POSTAVKA_FSCTRL1    0x06
#define CC1101_POSTAVKA_FREQ2      0x10
#define CC1101_POSTAVKA_FREQ1      0xA7
#define CC1101_POSTAVKA_FREQ0      0x62
#define CC1101_POSTAVKA_MDMCFG4    0xF5
#define CC1101_POSTAVKA_MDMCFG3    0x83
#define CC1101_POSTAVKA_MDMCFG2    0x91
#define CC1101_POSTAVKA_MDMCFG1    0x22
#define CC1101_POSTAVKA_DEVIATN    0x15
#define CC1101_POSTAVKA_MCSM0      0x18
#define CC1101_POSTAVKA_FOCCFG     0x16
#define CC1101_POSTAVKA_WORCTRL    0xFB
#define CC1101_POSTAVKA_FSCAL3     0xE9
#define CC1101_POSTAVKA_FSCAL2     0x2A
#define CC1101_POSTAVKA_FSCAL1     0x00
#define CC1101_POSTAVKA_FSCAL0     0x1F
#define CC1101_POSTAVKA_FSTEST     0x59
#define CC1101_POSTAVKA_PTEST      0x7F
#define CC1101_POSTAVKA_AGCTEST    0x3F
#define CC1101_POSTAVKA_TEST2      0x81
#define CC1101_POSTAVKA_TEST1      0x35
#define CC1101_POSTAVKA_TEST0      0x09

/*
 *
 * Command strobe registri
 *
 * Command strobe je u principu naredba gdje se šalje samo jedan header byte koji
 * se sastoji od jednog R/W bita(može biti 0 ili 1), burst access bita(mora bit u 0)
 * i 6 bita koji se koriste za adresu (u rasponu od 0x30-0x3D)
 *
 * Ukratko, command strobe-om se može uključiti receive mode, ugasiti kristalni oscilator
 * ili uključiti wake on radio
 *
 */
#define	SRES		0x30	//SOFTWARE RESET CHIP
#define	SFSTXON		0x31	//UKLJUCI (|| KALIBRIRAJ) FREKVENTNI SINTETIZATOR
#define	SXOFF		0x32	//UGASI KRISTALNI OSCILATOR
#define	SCAL		0x33	//KALIBRIRAJ FREKVENTNI SINTETIZATOR
#define	SRX			0x34	//UKLJUCI RX
#define	STX			0x35	//UKLJUCI TX
#define	SIDLE		0x36	//ISKLJUCI RX/TX,
#define	SWOR		0x38	//WAKE ON RADIO
#define	SPWD		0x39	//UDI U power down mode KADA CSn POSTANE HIGH
#define	SFRX		0x3A	//OCISTI RX FIFO BUFFER (ovisi o par uvjeta)
#define	SFTX		0x3B	//OCISTI TX FIFO BUFFER (ovisi o par uvjeta)
#define	SWORRST		0x3C
#define	SNOP		0x3D	//BEZ OPERACIJE, MOŽE SE KORISTITI ZA PRISTUP CHIP STATUS BYTE-u

/*
 *
 * Konfiguracijski registri
 *
 */
#define IOCFG2       0x00        // Konfiguracija GDO2 output pin
#define IOCFG1       0x01        // Konfiguracija GDO1 output pin
#define IOCFG0       0x02        // Konfiguracija GDO0 output pin
#define FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define SYNC1        0x04        // Sync word, high byte
#define SYNC0        0x05        // Sync word, low byte
#define PKTLEN       0x06        // Packet length
#define PKTCTRL1     0x07        // Kontrola packet automatizacije
#define PKTCTRL0     0x08        // Kontrola packet automatizacije
#define ADDR         0x09        // Device address
#define CHANNR       0x0A        // Channel number
#define FSCTRL1      0x0B        // Kontrola Frequency synthesizer-a
#define FSCTRL0      0x0C        // Kontrola Frequency synthesizer-a
#define FREQ2        0x0D        // Kontrola Frekvencije, high byte
#define FREQ1        0x0E        // Kontrola Frekvencije, middle byte
#define FREQ0        0x0F        // Kontrola Frekvencije, low byte
#define MDMCFG4      0x10        // Konfiguracija Modem-a
#define MDMCFG3      0x11        // Konfiguracija Modem-a
#define MDMCFG2      0x12        // Konfiguracija Modem-a
#define MDMCFG1      0x13        // Konfiguracija Modem-a
#define MDMCFG0      0x14        // Konfiguracija Modem-a
#define DEVIATN      0x15        // Postavke Modem deviation-a
#define MCSM2        0x16        // Konfiguracija Main Radio Control State Machine
#define MCSM1        0x17        // Konfiguracija Main Radio Control State Machine
#define MCSM0        0x18        // Konfiguracija Main Radio Control State Machine
#define FOCCFG       0x19        // Konfiguracija Frequency Offset Compensation-a
#define BSCFG        0x1A        // Konfiguracija Bit Synchronization-a
#define AGCCTRL2     0x1B        // Kontrola AGC-a
#define AGCCTRL1     0x1C        // Kontrola AGC-a
#define AGCCTRL0     0x1D        // Kontrola AGC-a
#define WOREVT1      0x1E        // High INT8U Event 0 timeout
#define WOREVT0      0x1F        // Low INT8U Event 0 timeout
#define WORCTRL      0x20        // Wake On Radio control
#define FREND1       0x21        // Front end RX konfiguracija
#define FREND0       0x22        // Front end TX konfiguracija
#define FSCAL3       0x23        // Kalibracija frequency synthesizer-a
#define FSCAL2       0x24        // Kalibracija frequency synthesizer-a
#define FSCAL1       0x25        // Kalibracija frequency synthesizer-a
#define FSCAL0       0x26        // Kalibracija frequency synthesizer-a
#define RCCTRL1      0x27        // Konfiguracija RC oscillator-a
#define RCCTRL0      0x28        // Konfiguracija RC oscillator-a
#define FSTEST       0x29        // Kontrola frequency synthesizer kalibracije
#define PTEST        0x2A        // Test proizvodnje
#define AGCTEST      0x2B        // Test AGC-a
#define TEST2        0x2C        // Razne postavke za testiranje
#define TEST1        0x2D        // Razne postavke za testiranje
#define TEST0        0x2E        // Razne postavke za testiranje
#define PATABLE		 0x3E		 // PATABLE Access

/*
 *
 * Status registri
 * Status registrima se može pristupiti samo s spi read burst naredbom
 *
 * Komentar za errata CC1101: Mogućnost da se pokvari podatak tokom SPI Read-a je 1 u svakih 4000 byte readova(pg.05).
 *
 */
#define PARTNUM 		0x30
#define VERSION 		0x31
#define FREQEST 		0x32	//dok je receiver aktivan, Errata cc1101, pg.05, spi read tvornicki problem
#define LQI		 		0x33
#define RSSI	 		0x34	//dok je receiver aktivan, Errata cc1101, pg.05, spi read tvornicki problem
#define MARCSTATE 		0x35	//dok nije u IDLE, Errata cc1101, pg.05, spi read tvornicki problem
#define WORTIME1 		0x36	//bilo kad, Errata cc1101, pg.05, spi read tvornicki problem
#define WORTIME0 		0x37	//bilo kad, Errata cc1101, pg.05, spi read tvornicki problem
#define PKTSTATUS 		0x38
#define VCO_VC_DAC 		0x39
#define TXBYTES 		0x3A	//dok šalje paket, Errata cc1101, pg.05, spi read tvornicki problem
#define RXBYTES 		0x3B	//dok prima paket, Errata cc1101, pg.05, spi read tvornicki problem
#define RCCTRL1_STATUS 	0x3C
#define RCCTRL0_STATUS 	0x3D

void SPI_CS_WRITE(bool state);

void manual_POR();

void cc1101_init(bool state);

void write_cc1101_config_sleep();

void write_tx_cc1101_config();

void write_rx_cc1101_config();

uint8_t command_strobe(uint8_t addr);

uint8_t read_status_reg(uint8_t addr, uint8_t *statusByte);

uint8_t transmit_burst_byte_433();

uint8_t receive_burst_byte_433(uint8_t *rxData);

uint8_t read_single_byte(uint8_t addr, uint8_t *chStatus);

uint8_t write_single_byte(uint8_t addr, uint8_t data);

void read_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *rxData, uint8_t len);

void write_burst_byte(uint8_t startAddr, uint8_t *statusByte, uint8_t *txData, uint8_t len);

void hal_status(HAL_StatusTypeDef SPIstatus);

#endif
