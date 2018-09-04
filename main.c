#include <stdint.h>
#include <stddef.h>
#include <elf.h>
#include "util.h"
#include "loadkern.h"
#include "io.h"

#define CLOCKFREQ 50000000
extern uint8_t MEMORY_BASE;

void die()
{
	outs("fatal error detected, entering dead loop\r\n");
	for (;;) {};
}

void checkelf(Elf32_Ehdr *elf)
{
#define CHECK(expr) if (!(expr)) die()
	CHECK(elf->e_ident[EI_MAG0] == ELFMAG0);
	CHECK(elf->e_ident[EI_MAG1] == ELFMAG1);
	CHECK(elf->e_ident[EI_MAG2] == ELFMAG2);
	CHECK(elf->e_ident[EI_MAG3] == ELFMAG3);
	CHECK(elf->e_ident[EI_CLASS] == ELFCLASS32);
	CHECK(elf->e_ident[EI_DATA] == ELFDATA2LSB);
	CHECK(elf->e_type == ET_EXEC);
	CHECK(elf->e_machine == EM_MIPS); /* not EM_MIPS_RS3_LE */
#undef CHECK
}

void copy(uint32_t *dst, uint32_t *src, size_t size)
{
	uint32_t *pd, *ps;
	int incr;
	size /= sizeof(uint32_t);
	if (dst < src) {
		incr = 1;
		pd = dst;
		ps = src;
	} else {
		incr = -1;
		pd = dst + size;
		ps = src + size;
	}
	for (; size > 0; size--) {
		*pd = *ps;
		pd += incr;
		ps += incr;
	}
}

void extract(uint8_t *elf, Elf32_Phdr *phdr)
{
	if (phdr->p_type != PT_LOAD)
		return;
	copy((uint32_t *) phdr->p_vaddr, (uint32_t *) (elf + phdr->p_offset),
			phdr->p_filesz);
	zerofill((uint32_t *) (phdr->p_vaddr + phdr->p_filesz),
			phdr->p_memsz - phdr->p_filesz);
}

void phload(uint8_t *elf, size_t n)
{
	Elf32_Ehdr *eh = (Elf32_Ehdr *) elf;
	if (eh->e_phnum <= n)
		return;
	Elf32_Phdr ph = *((Elf32_Phdr *) (elf + eh->e_phoff) +
			eh->e_phentsize * n);
	phload(elf, n + 1);
	outs("extracting data to 0x");
	outw((uint32_t) ph.p_vaddr);
	outs(", size = 0x");
	outw((uint32_t) ph.p_memsz);
	outs("\r\n");
	extract(elf, &ph);
}

void bootkern(uint8_t *kern)
{
	Elf32_Ehdr *eh = (Elf32_Ehdr *) kern;
	void *entry = (void *) eh->e_entry;
	checkelf(eh);
	outs("found ELF entry 0x");
	outw((uint32_t) entry);
	outs("\r\n");
	phload(kern, 0);
	outs("jumping to new entry\r\n");
	jumpto(entry);
}

void bench(void)
{
	outs("clock frequency 0x");
	outw(CLOCKFREQ / 1000000);
	outs(" MHz\r\n");

	outs("benchmarking pipeline performance\r\n");
	volatile uint32_t i, n;
	n = CLOCKFREQ / 10000;
	outs("itering 0x");
	outw(n);
	outs(" times...\r\n");
	clearcount();
	for (i = 0; i < n; i++) {};
	i = readcount();
	outs(" took 0x");
	outw(i);
	outs(" cycles (0x");
	outw(i / (CLOCKFREQ / 1000));
	outs(" ms, 0x");
	outw(i / (n / 1000));
	outs(" cycles / 1000 times\r\n");

	outs("benchmarking memory performance\r\n");
	const uint32_t bench_size = 1024 * 1024;
	uint8_t *src = &MEMORY_BASE;
	uint8_t *dst = src + bench_size;
	outs("copying 0x");
	outw(bench_size);
	outs(" bytes\r\n");
	clearcount();
	c_memmove(dst, src, bench_size);
	i = readcount();
	outs(" took 0x");
	outw(i);
	outs(" cycles (0x");
	outw(i / (CLOCKFREQ / 1000));
	outs(" ms), speed 0x");
	outw(CLOCKFREQ / i * bench_size);
	outs(" B/s\r\n");
}

int main(void)
{
	meminit();
	io_init();
	outs("BIT-MIPS firmware and stage 0 bootloader\r\n");
	bench();
	uint8_t *kern = loadkern();
	if (kern == NULL)
		die();
	outs("booting ELF kernel\r\n");
	bootkern(kern);
	die();
}
