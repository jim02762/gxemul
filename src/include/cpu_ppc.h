#ifndef	CPU_PPC_H
#define	CPU_PPC_H

/*
 *  Copyright (C) 2005  Anders Gavare.  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright  
 *     notice, this list of conditions and the following disclaimer in the 
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE   
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 *  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 *  SUCH DAMAGE.
 *
 *
 *  $Id: cpu_ppc.h,v 1.7 2005-02-02 19:10:03 debug Exp $
 */

#include "misc.h"


#ifndef ENABLE_PPC


struct ppc_cpu {
	int	dummy;
};

#else	/*  ENABLE_PPC  */


struct cpu_family;

#define	MODE_PPC		0
#define	MODE_POWER		1

/*  PPC CPU types:  */
struct ppc_cpu_type_def { 
	char		*name;
	int		icache_shift;
	int		iway;
	int		dcache_shift;
	int		dway;
	int		l2cache_shift;
	int		l2way;

	/*  TODO: 64-bit-ness? POWER vs PowerPC?  */
};

/*  TODO: Most of these just bogus  */

#define PPC_CPU_TYPE_DEFS	{				\
	{ "G4e", 15, 8, 15, 8, 18, 8 },				\
	{ "PPC405GP", 15, 2, 15, 2, 20, 1, },		 	\
	{ "PPC750", 15, 2, 15, 2, 20, 1 },			\
	{ "PPC970", 16, 1, 15, 2, 19, 1 },			\
	{ NULL, 0,0, 0,0, 0,0 }					\
	};

#define	PPC_NGPRS		32
#define	PPC_NFPRS		32

struct ppc_cpu {
	struct ppc_cpu_type_def cpu_type;

	uint64_t	pc;		/*  Program Counter (TODO: CIA?)  */
	uint64_t	pc_last;

	int		mode;		/*  MODE_PPC or MODE_POWER  */
	int		bits;		/*  32 or 64  */

	uint32_t	cr;		/*  Condition Register  */
	uint32_t	fpscr;		/*  FP Status and Control Register  */
	uint64_t	lr;		/*  Link Register  */
	uint64_t	ctr;		/*  Count Register  */
	uint64_t	gpr[PPC_NGPRS];	/*  General Purpose Registers  */
	uint64_t	xer;		/*  FP Exception Register  */
	uint64_t	fpr[PPC_NFPRS];	/*  Floating-Point Registers  */
};


/*  cpu_ppc.c:  */
struct cpu *ppc_cpu_new(struct memory *mem, struct machine *machine,
        int cpu_id, char *cpu_type_name);
void ppc_cpu_show_full_statistics(struct machine *m);
void ppc_cpu_tlbdump(struct machine *m, int x, int rawflag);
void ppc_cpu_register_match(struct machine *m, char *name, 
	int writeflag, uint64_t *valuep, int *match_register);
void ppc_cpu_register_dump(struct cpu *cpu, int gprs, int coprocs);
void ppc_cpu_disassemble_instr(struct cpu *cpu, unsigned char *instr,
        int running, uint64_t addr, int bintrans);
int ppc_cpu_interrupt(struct cpu *cpu, int irq_nr);
int ppc_cpu_interrupt_ack(struct cpu *cpu, int irq_nr);
void ppc_cpu_exception(struct cpu *cpu, int exccode, int tlb, uint64_t vaddr,
        /*  uint64_t pagemask,  */  int coproc_nr, uint64_t vaddr_vpn2,
        int vaddr_asid, int x_64);
int ppc_cpu_run(struct emul *emul, struct machine *machine);
void ppc_cpu_dumpinfo(struct cpu *cpu);
void ppc_cpu_list_available_types(void);
int ppc_cpu_family_init(struct cpu_family *);


#endif	/*  ENABLE_PPC  */

#endif	/*  CPU_PPC_H  */
