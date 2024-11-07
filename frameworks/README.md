# LightWhale's frameworks

> [!NOTE] 
> Frameworks represents deployed version of a software or a library

**List of libs and executable**
- LR/HT control protocol @ [/frameworks/lrhtcp](lrhtcp)
- Feathering (compression lib) @ [/frameworks/feathering](feathering)
- SharpWhale (autoencoders) @ [/frameworks/sharp_whale](sharp_whale)

**List of embedded software (network cards, ...)**
- Network card eSoftware @ [/frameworks/netcards](netcards)
    - ST-Micro [Nucleo-WL55JC](https://www.st.com/en/evaluation-tools/nucleo-wl55jc.html) (high-band 865-915MHz) @ [stm-nucleo-wl55jc1](netcards/stm-nucleo-wl55jc1/)

## Install libs and executable on your system (linux/unix-like)

From this folder, execute these commands :
```sh
mkdir build && cd build
cmake ..
make -j lightwhale-toolset && make install
```

## Deploy toolset inside a network card

From this folder, execute these commands :
```sh
# create build directory
mkdir build && cd build

# configure and generate make files 
cmake -DNETCARD_EMBEDDED_FEATHERING=ON -DNETCARD_EMBEDDED_SHARPWHALE=ON .. 

# Build
make -j <network-card-reference> # cf. folders inside netcards folder
                                 # e.g: stm-nucleo-wl55jc1

# Flash
make <network-card-reference>.flash # e.g: stm-nucleo-wl55jc1.flash
```
