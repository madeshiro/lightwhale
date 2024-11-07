<div align="center">
  <img alt="lightwhale logo" src="resources/LightWhaleNoir.SVG" style="height: 180px"/>
  <img alt="lightwhale logo" src="resources/LightWhaleBicolor.SVG" style="height: 180px"/>
  <img alt="lightwhale logo" src="resources/LightWhaleBlanc.SVG" style="height: 180px"/>
</div>

---

[![mpl2.0](https://img.shields.io/badge/license-Mozilla%20Public%20License%202.0-orange)](https://www.mozilla.org/en-US/MPL/2.0/)
&nbsp;
![main workflow](https://github.com/madeshiro/lightwhale/actions/workflows/main.yml/badge.svg?branch=main)

# LightWhale
> Low-Rate / Heavy Transmission Control Protocol, specialized for images (and aim for becoming multiple purposes)

The LightWhale project is a regroupment of multiples subprojects intended for IoT Wireless communication using LoRa communication system. It is separated into two differents Git:
- The Software/Hardware project, regrouping the custom communication protocol, drivers and hardware description of the home-made network card.
- The algorithms research git containing a bunch of python scripts and libs for image compression and encoding

The aim of the LightWhale project is to be able to communicate heavy kind of information (HD images) through the LoRa network using differents tricks like data compression/reduction and full usage of LoRa potential. 
To do so, a custom network card based on differents LoRa chips is currently being designed. 

> [!NOTE]
> This project is a master’s degree end-of-study’s project in the context of the SETSIS master 
> (Embedded Systems and Signal Processing for Images and Sounds)
> at the Clermont-Auvergne University, France.

> [!IMPORTANT]
> This project is under the [Mozilla Public License 2.0](https://www.mozilla.org/en-US/MPL/2.0/). 

## 1. Setup the project

> [!NOTE]
> The project requires some dependencies when it comes to hardware. For instance,
> embedded development are performed with an STM microcontroller (STM32WL55JC).

### 1.1 Install GCC/G++ for ARM
#### 1.1.1 For Linux

```sh
sudo apt-get install gcc-arm-none-eabi binutils-arm-none-eabi libnewlib-arm-none-eabi
```
### 1.2 Install STM32CubeWL
#### 1.2.1 For Linux

Clone the [STM32CubeWL](https://github.com/STMicroelectronics/STM32CubeWL) wherever you want (recommand in a folder like `/opt/STMicroelectronics` using `su`).

```sh 
# From where you want to clone the project
git clone --recursive https://github.com/STMicroelectronics/STM32CubeWL.git
```
Then, make sure to get the latest update:
```sh 
git pull
git submodule update --init --recursive
```
And checkout to the desired version:
```sh
git checkout v1.3.0
```
Then, export the following variable environment:
```sh
# Replace the path by your own installation path
echo "export STM32_CUBE_WL=/opt/STMicroelectronics/STM32CubeWL" >> ~/.bashrc
```

### 1.3 Install STLink to flash boards
#### 1.3.1 For Linux (using `apt-get`)

Install the [STLink](https://github.com/stlink-org/stlink) toolset by launching the following command:
```sh
sudo apt install stlink-tools
```

---
<div align="center">
  
  [![eupi](https://eupi.uca.fr/uas/Eupi/LOGO/eupi_long.png)](https://eupi.uca.fr)
  
</div>
