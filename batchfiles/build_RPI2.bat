cd c:\JivenJive\
call "C:\ArmFreeToolChain\bin\gccvar.bat"
pushd RPI2build
del a.o

del sdk_c.o
del sdk_c.asm

del JivenJive_RPI2.o
del JivenJive_RPI2.asm

del kernel7.elf
del kernel7.map
del kernel7.img
@echo START BUILD: %time%
arm-none-eabi-as -I C:\RaspberryPi\RPI2_ANR_SDK\src\ -march=armv7-a -mfloat-abi=hard -mfpu=neon C:\RaspberryPi\RPI2_ANR_SDK\src\main.asm -o a.o

arm-none-eabi-gcc -march=armv7-a -mfloat-abi=hard -mfpu=neon -nostdlib -mno-thumb-interwork -c C:\RaspberryPi\RPI2_ANR_SDK\src\sdk.c -o sdk_c.o
arm-none-eabi-gcc -march=armv7-a -mfloat-abi=hard -mfpu=neon -nostdlib -mno-thumb-interwork -S C:\RaspberryPi\RPI2_ANR_SDK\src\sdk.c -o sdk_c.asm

arm-none-eabi-gcc -fno-exceptions -march=armv7-a -mfloat-abi=hard -mfpu=neon -finput-charset=UTF-8 -fextended-identifiers -nostdinc++ -nostdlib -std=c++11 -c C:\JivenJive\src\JivenJive_RPI2.cpp -o JivenJive_RPI2.o	-I C:\JivenJive\src
arm-none-eabi-gcc -fno-exceptions -march=armv7-a -mfloat-abi=hard -mfpu=neon -finput-charset=UTF-8 -fextended-identifiers -nostdinc++ -nostdlib -std=c++11 -S C:\JivenJive\src\JivenJive_RPI2.cpp -o JivenJive_RPI2.asm -I C:\JivenJive\src

arm-none-eabi-ld -nolibc -nostdlib a.o sdk_c.o JivenJive_RPI2.o -o kernel7.elf -Map kernel7.map
arm-none-eabi-objcopy -O binary kernel7.elf kernel7.img
@echo END BUILD: %time%
popd