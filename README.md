# Zephyr Device Tree (ST7789 + LVGL)

![](demo.gif)

> $ west flash
>   
> $ st-info --probe  
> $ st-flash write build/zephyr/zephyr.bin 0x8000000  
>   
> $ pyocd list  
> $ pyocd flash -t stm32l475xe -a 0x8000000 ~/mcuboot/zephyr.bin   

## Quick Start

Step 1: Install Zephyr

```
$ pip install west
$ west init ~/zephyrproject
$ cd ~/zephyrproject
$ west update
$ west zephyr-export
$ pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt
```

Step 2: Install Zephyr-SDK

```
$ cd ~
$ wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz
$ wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/sha256.sum | shasum --check --ignore-missing
$ tar xvf zephyr-sdk-0.16.8_linux-x86_64.tar.xz
$ cd zephyr-sdk-0.16.8
$ ./setup.sh

$ sudo cp ~/zephyr-sdk-0.16.8/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
$ sudo udevadm control --reload-rules
$ sudo udevadm trigger
```


Step 3: Hello-World Example

```
$ source ~/zephyrproject/zephyr/zephyr-env.sh

$ git clone https://github.com/wuhanstudio/stm32l475-zephyr-bsp
$ cd stm32l475-zephyr-bsp/hello_world

$ west build -b pandora_stm32l475
$ west flash
```

Step 4: Add modules (Optional)

```
$ west build -b pandora_stm32l475 -t menuconfig
```
```
Subsystems and OS Services  --->
  [*] Shell  --->
Device Drivers  --->
  [*] General-Purpose Input/Output (GPIO) drivers  --->
    [*] GPIO Shell
  [*] Inter-Integrated Circuit (I2C) bus drivers  --->
    [*] I2C Shell
[D] Save minimal config (advanced)
```
```
$ cat build/zephyr/kconfig/defconfig > prj.conf
$ west build -b pandora_stm32l475 -p
$ west flash
```

## Project 1: GPIO and I2C:

- i2c device tree: https://github.com/zephyrproject-rtos/zephyr/blob/main/dts/arm/st/l4/stm32l4.dtsi
- gpio pinctrl definition: https://github.com/zephyrproject-rtos/hal_stm32/blob/main/dts/st/l4/stm32l475v(c-e-g)tx-pinctrl.dtsi

```
$ cd hello_world

$ west build -b pandora_stm32l475
$ west flash
```

```
# LED: PE8
uart:~$ gpio conf gpio@48001000 8 o
uart:~$ gpio set gpio@48001000 8 0
uart:~$ gpio set gpio@48001000 8 1

# I2C3: i2c3_scl_pc0 &i2c3_sda_pc1
uart:~$ i2c scan i2c@40005c00 
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:             -- -- -- -- -- -- -- -- -- -- -- -- 
10: 10 -- -- -- -- -- -- -- -- -- -- -- -- -- 1e -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         
3 devices found on i2c@40005c00
```

## Project 2: SPI LCD (st7789 240x240):

- spi device tree: https://github.com/zephyrproject-rtos/zephyr/blob/main/boards/shields/st7789v_generic/st7789v_tl019fqv01.overlay

```
$ cd hello_st7789

$ west build -b pandora_stm32l475
$ west flash
```

## Project 3: LVGL (MIPI DBI):

```
$ cd hello_lvgl

$ west build -b pandora_stm32l475
$ west flash
```

## Project 4: MCUBoot

```
# Step 1: Build MCUBoot (bootloader)
$ git clone https://github.com/mcu-tools/mcuboot
$ cp mcuboot.overlay mcuboot/boot/zephyr/app.overlay
$ cd mcuboot/boot/zephyr
$ west build -b pandora_stm32l475
$ west flash

# Step 2: Build Zephyr Application
$ cd hello_mcuboot
$ west build -b pandora_stm32l475

# Step 3: Sign the firmware
$ ../mcuboot/scripts/imgtool.py sign -k ../mcuboot/root-rsa-2048.pem build/zephyr/zephyr.bin build/zephyr/zephyr.signed.bin -v 0.0.1 --header-size 0x200 --slot-size 0x3a000
$ pyocd flash -t stm32l475xe -a 0x800c000 build/zephyr/zephyr.signed.bin
```

