/*
 *  Copyright (C) 2004 by Anders Gavare.  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright  
 *     notice, this list of conditions and the following disclaimer in the 
 *     documentation and/or other materials provided with the distribution.
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
 *  $Id: bintrans.c,v 1.4 2004-01-26 14:30:57 debug Exp $
 *
 *  Binary translation.
 *
 *  TODO:  This is just a brainstorming scratch area so far.
 *
 *
 *	Keep a cache of a certain number of blocks. Least-recently-used
 *		blocks are replaced.
 *
 *	Don't translate blindly. (?)  Try to wait until we are sure that
 *		a block is actualy used more than once before translating
 *		it. (We can either keep an absolute count of lots of
 *		memory addresses, or utilize some kind of random function.
 *		In the later case, if a block is run many times, it will
 *		have a higher probability of being translated.)
 *
 *	Simple basic-block stuff. Only simple-enough instructions are
 *		translated. (for example, the 'cache' and 'tlbwr' instructions
 *		are NOT simple enough)
 *
 *	Invalidate a block if it is overwritten.
 *
 *	Translate code in physical ram, not virtual.
 *		Why?  This will keep things translated over process
 *		switches, and TLB updates.
 *
 *	Do not translate over hardware (4KB) page boundaries.
 *
 *	Check before running a basic block that no external
 *		exceptions will occur for the duration of the
 *		block.  (External = from a clock device or other
 *		hardware device.)
 *
 *	Check for exceptions inside the block, for those instructions
 *		that require that.  Update the instruction counter by
 *		the number of successfully executed instructions only.
 *
 *	Register allocation, set registers before running the block
 *		and read them back afterwards (for example on Alpha)
 *	OR:
 *		manually manipulate the emulated cpu's registers in the
 *		host's "struct cpu". (For example on i386.)
 *
 *	Multiple target archs (alpha, i386, sparc, mips :-), ...)
 *		Try to use arch specific optimizations, such as prefetch
 *		on alphas that support that.
 *		Not all instructions will be easily translated to all
 *		backends.
 *
 *	Allow load/store if all such load/stores are confined
 *		to a specific page of virtual memory (or somewhere in kernel
 *		memory, in which case access are allowed to cross page
 *		boundaries), so that any code not requiring TLB updates
 *		will still run without intervention.
 *		The loads/stores will go to physical RAM, so they have
 *		to be translated (once) via the TLB.
 *
 *	Testing:  Running regression tests with and without the binary
 *		translator enabled should obviously result in the exact
 *		same results, or something is wrong.
 *
 *	How about loops?
 */

/*
The general idea would be something like this:
A block of code in the host's memory would have to conform with
the C function calling ABI on that platform, so that the code block
can be called easily from C. That is, entry and exit code needs to
be added, in addition to the translated instructions.

	o)  Check for the current PC in the translation cache.
	o)  If the current PC is not found, then make a decision
		regarding making a translation attempt, or not.
		Fallback to normal instruction execution in cpu.c.
	o)  If there is a code block for the current PC in the
		translation cache, do a couple of checks and then
		run the block. Update registers and other values
		as neccessary.

The checks would include:
	boundaries for load/store memory accesses  (these must be
		to pages that are accessible without modifying the TLB,
		that is, they either have to be to addresses which
		are in the TLB or to kseg0/1 addresses)
	no external interrupts should occur for enough number
		of cycles

 */

