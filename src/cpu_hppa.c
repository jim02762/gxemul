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
 *  $Id: cpu_hppa.c,v 1.1 2005-03-09 07:27:00 debug Exp $
 *
 *  HPPA CPU emulation.
 *
 *  TODO: This is just a dummy so far.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "misc.h"


#ifndef	ENABLE_HPPA


#include "cpu_hppa.h"


/*
 *  hppa_cpu_family_init():
 *
 *  Bogus, when ENABLE_HPPA isn't defined.
 */
int hppa_cpu_family_init(struct cpu_family *fp)
{
	return 0;
}


#else	/*  ENABLE_HPPA  */


#include "cpu.h"
#include "cpu_hppa.h"
#include "machine.h"
#include "memory.h"
#include "symbol.h"


extern volatile int single_step;
extern int old_show_trace_tree;   
extern int old_instruction_trace;
extern int old_quiet_mode;
extern int quiet_mode;


/*
 *  hppa_cpu_new():
 *
 *  Create a new HPPA cpu object.
 */
struct cpu *hppa_cpu_new(struct memory *mem, struct machine *machine,
	int cpu_id, char *cpu_type_name)
{
	struct cpu *cpu;

	if (cpu_type_name == NULL || strcmp(cpu_type_name, "HPPA2.0") != 0)
		return NULL;

	cpu = malloc(sizeof(struct cpu));
	if (cpu == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}

	memset(cpu, 0, sizeof(struct cpu));
	cpu->memory_rw          = hppa_memory_rw;
	cpu->name               = cpu_type_name;
	cpu->mem                = mem;
	cpu->machine            = machine;
	cpu->cpu_id             = cpu_id;
	cpu->byte_order         = EMUL_BIG_ENDIAN;
	cpu->bootstrap_cpu_flag = 0;
	cpu->running            = 0;

	/*  Only show name and caches etc for CPU nr 0 (in SMP machines):  */
	if (cpu_id == 0) {
		debug("%s", cpu->name);
	}

	return cpu;
}


/*
 *  hppa_cpu_dumpinfo():
 */
void hppa_cpu_dumpinfo(struct cpu *cpu)
{
	debug("\n");

	/*  TODO  */
}


/*
 *  hppa_cpu_list_available_types():
 *
 *  Print a list of available HPPA CPU types.
 */
void hppa_cpu_list_available_types(void)
{
	/*  TODO  */

	debug("HPPA2.0\n");
}


/*
 *  hppa_cpu_register_match():
 */
void hppa_cpu_register_match(struct machine *m, char *name,
	int writeflag, uint64_t *valuep, int *match_register)
{
	int cpunr = 0;

	/*  CPU number:  */

	/*  TODO  */

	/*  Register name:  */
	if (strcasecmp(name, "pc") == 0) {
		if (writeflag) {
			m->cpus[cpunr]->pc = *valuep;
		} else
			*valuep = m->cpus[cpunr]->pc;
		*match_register = 1;
	}

	/*  TODO: _LOTS_ of stuff.  */
}


#define MEMORY_RW	hppa_memory_rw
#define MEM_HPPA
#include "memory_rw.c"
#undef MEM_HPPA
#undef MEMORY_RW


/*
 *  hppa_cpu_family_init():
 *
 *  Fill in the cpu_family struct for HPPA.
 */
int hppa_cpu_family_init(struct cpu_family *fp)
{
	fp->name = "HPPA";
	fp->cpu_new = hppa_cpu_new;
	fp->list_available_types = hppa_cpu_list_available_types;
	fp->register_match = hppa_cpu_register_match;
	/*  fp->disassemble_instr = hppa_cpu_disassemble_instr;  */
	/*  fp->register_dump = hppa_cpu_register_dump;  */
	/*  fp->run = hppa_cpu_run;  */
	fp->dumpinfo = hppa_cpu_dumpinfo;
	/*  fp->show_full_statistics = hppa_cpu_show_full_statistics;  */
	/*  fp->tlbdump = hppa_cpu_tlbdump;  */
	/*  fp->interrupt = hppa_cpu_interrupt;  */
	/*  fp->interrupt_ack = hppa_cpu_interrupt_ack;  */
	return 1;
}

#endif	/*  ENABLE_HPPA  */
