# LightWhale project's changelog

## Sprint #1
> (October 4th, 2024—November 3rd, 2024)
- Set project structure
- prepare changelog for all features and commons
- separate project into 4 parts
    - **commons** : libraries usable for all features and/or frameworks
    - **drivers** (internal or external): components for the network cards i.e., interface 
between card <> computer or MCU/Module drivers like STM32CubeWL and radio's driver. 
    - **features** : every part of the lightwhale project as the communication protocol, compression tools and autoencoders set. 
    - **frameworks** : embedded software or features deployment. 
- /!\ Structure may change during the development. 

## Sprint #2
> (November 4th, 2024 - December 1st, 2024)
- Add all STM32 drivers for the STM32wlxx MCU + Nucleo64 BSP (wl55jc1)
  - Create specific drivers using STM32CubeMX for the nucleo board
  - Create nucleo board framework (embedded software’s deployment) with first HMI implementation