
/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2018-2019 Greg White (gkwhite@gmail.com).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static char const copyright[] =
"@(#) Copyright (c) 2018-2019\n\
	Greg White (gkwhite@gmail.com).  All rights reserved.\n";
#endif /* not lint */


#ifndef lint
static char sccsid[] = "@(#)nproc.c	0.09 (Berkeley) 01/06/19";
#endif /* not lint */

static char *version = "0.08";

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <sys/sysctl.h>

#include <capsicum_helpers.h>
#include <err.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Print the usage text for the nproc command
 */
void usage() {
	printf("Usage: nproc [-a | --all] [-i N | --ignore=N] [-h | --help] [-v | --version]\n");
	printf("Print the number of available processing units.\n");
	printf("      -a,   --all       print the number of available processors\n");
	printf("      -i N, --ignore=N  ignore N processors (minimum result is 1)\n");
	printf("      -h,   --help      display help and exit\n");
	printf("      -v,   --version   display version and exit\n");
}

/*
 * Return the number of processing units.  Allow the user to ignore
 * a certain number of processors, but always return at least 1
 */
int main(int argc, char **argv) {

	/* For argument parsing */
	int ch;
	static struct option long_options[] = {
		{"all",     no_argument,       NULL, 'a'},
		{"ignore",  required_argument, NULL, 'i'},
		{"help",    no_argument,       NULL, 'h'},
		{"version", no_argument,       NULL, 'v'},
		{NULL,      0,                 NULL, 0  }
	};

	/* For returning number of processors */
	int ignore_ncpu, mib[2], ncpu;
	size_t len;

	/* Enter capsicum with mininum rights */
	if (caph_limit_stdio() < 0 || (cap_enter() < 0 && errno != ENOSYS))
		err(1, "capsicum");

	ignore_ncpu = 0;

	/* Parse Arguments */
	while( (ch = getopt_long(argc, argv, "ai:hv", long_options, NULL)) != -1) {
		switch (ch) {
			case 'a':
				break;
			case 'i':
				/* 
				 * Error on invalid or negative number of CPUs to ignore
				 * Ignore zero processors is OK
				 */
				ignore_ncpu=strtol(optarg, NULL, 10);
				if( ((ignore_ncpu==0) && ((errno==EINVAL) || (errno==ERANGE)) ) ||
				    (ignore_ncpu<0) ) {
					printf("nproc: invalid number: %s\n", optarg);
					exit(EXIT_FAILURE);
				}
				break;
			case 'v':
				printf("nproc for BSD, version %s\n", version);
				printf("Copyright 2018-2019, Greg White <gkwhite@gmail.com>\n");
				printf("License BSD-2-Clause-FreeBSD\n");
				exit(EXIT_SUCCESS);
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}

	/* Get number of cpus from sysctl() */
	mib[0] = CTL_HW;
	mib[1] = HW_NCPU;
	len = sizeof(ncpu);
	if(sysctl(mib, 2, &ncpu, &len, NULL, 0) == -1)
		perror("sysctl failed");

	ncpu=ncpu-ignore_ncpu;

	/* There is always one cpu available */
	if(ncpu<1)
		ncpu=1;

	/* Return result */
	printf("%i\n", ncpu);
}
