#!/bin/sh

SRC_ROOT="../../src"

CC=arm-linux-gcc
LD=arm-linux-ld
OBJCOPY=arm-linux-objcopy
OBJDUMP=arm-linux-objdump

UBOOT="../../.."
AFLAGS="-O -g"
CFLAGS="-O -g -I${UBOOT}/include" 

LDFLAGS="-g -T ${SRC_ROOT}/boot/link.lds"

BOOT_OBJ="secu_boot.o"
MAIN_OBJ="main.o"
COMMON_OBJ="hw_if.o clock.o init_ddr.o"
#DISP_OBJ="cve5.o gfx_disp.o"
SECU_SYS_OBJ="sys_func.o secu_hw_if.o nor_func.o nand_func.o spi_func.o spi_read1k.o"
STDLIB_OBJ="x_clib.o x_assert.o"

OBJS="${BOOT_OBJ} ${MAIN_OBJ} ${COMMON_OBJ} ${DISP_OBJ} ${SECU_SYS_OBJ} ${STDLIB_OBJ}"
TARGET=secu_loader

rm -f *.o ${TARGET} ${TARGET}.*

echo " Compiling ............"

${CC} ${AFLAGS} -o secu_boot.o    -c ${SRC_ROOT}/boot/secu_boot.s
${CC} ${CFLAGS} -o main.o         -c ${SRC_ROOT}/main.c

${CC} ${CFLAGS} -o hw_if.o        -c ${SRC_ROOT}/common/hw_if.c
${CC} ${CFLAGS} -o clock.o        -c ${SRC_ROOT}/common/clock.c
${CC} ${CFLAGS} -o init_ddr.o     -c ${SRC_ROOT}/common/init_ddr.c

#${CC} ${CFLAGS} -o cve5.o         -c ${SRC_ROOT}/cve5/cve5.c
#${CC} ${CFLAGS} -o gfx_disp.o     -c ${SRC_ROOT}/disp/gfx_disp.c

${CC} ${CFLAGS} -o sys_func.o     -c ${SRC_ROOT}/sys/sys_func.c
${CC} ${CFLAGS} -o secu_hw_if.o   -c ${SRC_ROOT}/sys/secu_hw_if.c
${CC} ${CFLAGS} -o nor_func.o     -c ${SRC_ROOT}/sys/nor_func.c
${CC} ${CFLAGS} -o nand_func.o    -c ${SRC_ROOT}/sys/nand_func.c
${CC} ${CFLAGS} -o spi_func.o     -c ${SRC_ROOT}/sys/spi_func.c
${CC} ${AFLAGS} -o spi_read1k.o     -c ${SRC_ROOT}/sys/spi_read1k.S

${CC} ${CFLAGS} -o x_clib.o       -c ${SRC_ROOT}/stdlib/x_clib.c
${CC} ${CFLAGS} -o x_assert.o     -c ${SRC_ROOT}/stdlib/x_assert.c

echo " Linking ............" 
${LD} ${LDFLAGS} ${OBJS} -o ${TARGET}.elf
mv -f *.o ./temp/
echo " Obj Dump ............"
${OBJDUMP} -ald ${TARGET}.elf > ${TARGET}.s
${OBJDUMP} -S   ${TARGET}.elf > ${TARGET}.asm.c
${OBJDUMP} -h   ${TARGET}.elf > ${TARGET}.headinfo

echo " Obj Copy ............"
${OBJCOPY} -O binary -j .text ${TARGET}.elf ${TARGET}.text.bin
${OBJCOPY} -O binary -j .data ${TARGET}.elf ${TARGET}.data.bin
echo " Merging ............"
cat ${TARGET}.text.bin ${TARGET}.data.bin >${TARGET}.rom.bin
echo " Merge Finished!"


