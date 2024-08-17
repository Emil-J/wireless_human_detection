# Photographs Of The Project IRL
## Image Showing The Project Working
![WhatsApp Slika 2024-07-01 u 20 56 36_93c8a0d8](https://github.com/user-attachments/assets/9b79b3a8-5e72-4399-9378-4bbb81049f3a)

## RX Side Of The Project
![Slika Komponenti - Copy](https://github.com/user-attachments/assets/eceefe91-18c2-4593-a17d-893e1687222a)

## TX Side Of The Project
![Slika Komponenti - Copy](https://github.com/user-attachments/assets/ffc01259-6000-4b1a-8c93-7a0d3917437c)

# Short Explanation Of The Project
The practical aim of this project was to familiarize myeself with the functionalities of the CMSIS 
RTOS v2 interface in the STM32Cube integrated development environment, while also 
exploring the CC1101 module, which functions as a transceiver communication module 
operating on the radio frequency of 433 MHz.

In short, the project is about an intruder alarm on the entrance door of a house. This is made with two separate Nucleo boards, one has a function to be a data receiver and a feedback device while the other has a function to be a human detector and data transmitter. The intruder is detected via the LD2410B 24GHz radar module and this data is processed thru a CMSIS v2 RTOS transmitter program on the Nucleo-L476RG board. This data is then forwarded to the very reliable Texas Instruments CC1101 Transceiver chip which then sends the data over the air across the 433MHz radiofrequency to the Nucleo-F446RE board. This signal then is detected by the receiving module via the CC1101 chip and the data is processed in another CMSIS v2 RTOS receiving program which then runs the feedback alerts if there is a person detected.

# Photograph Of The Wiring

# List Of All The Used Components
## Hardware
* [Nucleo-F446RE Board](https://www.st.com/en/evaluation-tools/nucleo-f446re.html)
* [Nucleo-L476RG Board](https://www.st.com/en/evaluation-tools/nucleo-l476rg.html)
* [CC1101 Transceiver](https://www.ti.com/product/CC1101?keyMatch=CC1101&tisearch=search-everything&usecase=GPN-ALT&bm-verify=AAQAAAAJ_____z_JZDjlRNkQf0g6q4npV2w8d8BiAECWO0JyIodHEgjOSV90qO7CPvih_sx8H5Xi7qSPs5BnjfTnKsCtXrI39b1obV-YYNCvUpR60A3YU1Yl4pQ8UvShZvoKL4jhoeFrQk7rgdsREO0qrzl8Fe5DqJcSGsh7O2QwkmafS3I9XvAcWQd9CyHZGWXDCJKs2MP-WMQ-SZeythrjvuBssvxYVhK__0zWtyyEB1ciQMf_a6LIg4pkYjajAUe9OR4L97nmWlnJaoo3KZzPaKhF4B4r-Wr8M2xAyWBQLZbB0HhP5mJxTRvv94HIRAcm9BDbk6Hh72nBrMtj9yrJbg2JKAGnDND_oCma78IIX10uXuirOZ_6ytI)
* [HLK-LD2410B Human Radar Detection Module](https://www.hlktech.net/index.php?id=1094)
* [LCD1602_I2C Display](https://sparkfun.com/datasheets/LCD/HD44780.pdf)
* [Buzzer](https://arduinomodules.info/ky-012-active-buzzer-module/)
## Software
* [STM32CUBEMX Program](https://www.st.com/en/development-tools/stm32cubemx.html)
* [STM32CUBEIDE Program](https://www.st.com/en/development-tools/stm32cubeide.html)
* [CMSIS-V2 RTOS](https://arm-software.github.io/CMSIS_5/develop/RTOS2/html/index.html)
## Tools
### Hardware Tool
* Serial USB-TTL Device
### Software Tool
* [SmartRF Studio 7 Tool](https://www.ti.com/tool/download/SMARTRF-STUDIO-7/2.31.0)
* [LD2410 Tool](https://github.com/Emil-J/wireless_human_detection/tree/main/LD2410B%20Tools%20%26%20Documents/LD2410%20Tool)
# Overview Of The Files
* ZAVRSNI_RX_F446RE -> This file contains the RX side of the project for STM32CUBEIDE.
* ZAVRSNI_TX_L476RG -> This file contains the TX side of the project for STM32CUBEIDE.
* CC1101 Tools & Documents -> This file contains all the tools & documents for the CC1101 used that made this project possible.
* LD2410B Tools & Documents -> This file contains all the tools & documents for the LD2410B used that made this project possible.
