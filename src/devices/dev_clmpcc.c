/*
 *  Copyright (C) 2007  Anders Gavare.  All rights reserved.
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
 *  $Id: dev_clmpcc.c,v 1.1 2007-05-15 12:35:14 debug Exp $
 *
 *  Cirrus Logic Four Channel Multi-Protocol Communications Controller
 *  (CD2400/CD2401)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "device.h"
#include "emul.h"
#include "machine.h"
#include "memory.h"
#include "misc.h"


#include "clmpccreg.h"


#define	CLMPCC_LEN		0x200

struct clmpcc_data {
	unsigned char	reg[CLMPCC_LEN];
};


DEVICE_ACCESS(clmpcc)
{
	struct clmpcc_data *d = (struct clmpcc_data *) extra;
	uint64_t idata = 0, odata = 0;

	if (writeflag == MEM_WRITE)
		idata = memory_readmax64(cpu, data, len);

	if (writeflag == MEM_READ)
		odata = d->reg[relative_addr];

	switch (relative_addr) {

	case 0:	/*  Used by OpenBSD/mvme88k when probing...  */
		break;

	default:if (writeflag == MEM_READ)
			fatal("[ clmpcc: unimplemented READ from offset 0x%x ]"
			    "\n", (int)relative_addr);
		else
			fatal("[ clmpcc: unimplemented WRITE to offset 0x%x: "
			    "0x%x ]\n", (int)relative_addr, (int)idata);
//		exit(1);
	}

	if (writeflag == MEM_READ)
		memory_writemax64(cpu, data, len, odata);

	return 1;
}


DEVINIT(clmpcc)
{
	struct clmpcc_data *d = malloc(sizeof(struct clmpcc_data));
	if (d == NULL) {
		fprintf(stderr, "out of memory\n");
		exit(1);
	}
	memset(d, 0, sizeof(struct clmpcc_data));

	memory_device_register(devinit->machine->memory, devinit->name,
	    devinit->addr, CLMPCC_LEN, dev_clmpcc_access, (void *)d,
	    DM_DEFAULT, NULL);

	return 1;
}

