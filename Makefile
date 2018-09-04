TARGET         = loader.bin
KERNEL         = vmlinux
XZ_EMBEDDED    = xz-embedded
CROSS-COMPILE  = mipsel-softfloat-linux-gnu-
CC             = $(CROSS-COMPILE)gcc -march=r3000 -fno-pic -mno-abicalls -fno-stack-protector
LD             = $(CROSS-COMPILE)ld
OBJCOPY        = $(CROSS-COMPILE)objcopy
STRIP          = $(CROSS-COMPILE)strip
CFLAGS         = -Wall -I $(XZ_EMBEDDED)/linux/include/linux -I $(XZ_EMBEDDED)/linux/include -I $(XZ_EMBEDDED)/linux/lib/xz -I include -g -O2
EXTOBJS        = $(XZ_EMBEDDED)/linux/lib/xz/xz_crc32.o $(XZ_EMBEDDED)/linux/lib/xz/xz_dec_stream.o $(XZ_EMBEDDED)/linux/lib/xz/xz_dec_lzma2.o $(XZ_EMBEDDED)/linux/lib/xz/xz_dec_bcj.o

all: $(TARGET)

.SUFFIXES: .S .c .o .elf .bin

.S.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.elf.bin:
	$(OBJCOPY) -j.text -Obinary $< $@

loader.elf: entry.o main.o util.o util_s.o loadkern.o kerndata.o io.o trap.o $(EXTOBJS)
	$(LD) -Tsys.ld -o $@ $> $^

$(KERNEL).stripped: $(KERNEL)
	-rm -f $@
	cp $> $^ $@
	$(STRIP) -s $@

$(KERNEL).xz: $(KERNEL).stripped
	xz -C crc32 -ce < $> $^ > $@

kerndata.o: $(KERNEL).xz
	$(CC) $(CFLAGS) -DKERNEL_FILE=$(KERNEL).xz -c -o $@ kerndata.S

clean:
	-rm -f *.o *.elf *.bin *.xz *.stripped
