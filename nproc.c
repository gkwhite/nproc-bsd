
/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2018 Greg White (gkwhite@gmail.com).  All rights reserved.
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

// ToDo:  capsicum

static char *version = "0.04";

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <getopt.h>

void usage() {
	printf("Usage: nproc [OPTION]...\n");
	printf("Print the number of available processing units.\n");
	printf("      --all       print the number of available processors\n");
	printf("      --ignore=N  ignore N processors (minimum result is 1)\n");
	printf("      --help      display help and exit\n");
	printf("      --version   dispaly version and exit\n");
}

int main(int argc, char **argv) {
	int bflag,ch, ignore_count;
	int option_index=0;
	static struct option long_options[] = {
		{"all",     no_argument,       NULL, 'a'},
		{"ignore",  required_argument, NULL, 'i'},
		{"help",    no_argument,       NULL, 'h'},
		{"version", no_argument,       NULL, 'v'},
		{NULL,      0,                 NULL, 0  }
	};

	int mib[2], ncpu, ignorecpu=0;
	size_t len;

	while( (ch = getopt_long(argc, argv, "ai:hv", long_options, NULL)) != -1) {
		switch (ch) {
			case 'a':
				break;
			case 'i':
				// ignore invalid or negative number of CPUs to ignore
				// ignore zero CPUs is ok
				ignorecpu=strtol(optarg,NULL,10);
				if( ((ignorecpu==0) && ((errno==EINVAL) || (errno==ERANGE)) )
					|| (ignorecpu<0) ) {
					printf("nproc: invalid number: %s\n",optarg);
					exit(EXIT_FAILURE);
				}
				break;
			case 'v':
				printf("nproc for BSD, version %s\n", version);
				printf("Copyright 2018, Greg White (gkwhite@gmail.com)\n");
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

    // get number of cpus from sysctl
	mib[0] = CTL_HW;
	mib[1] = HW_NCPU;
	len = sizeof(ncpu);
	if(sysctl(mib, 2, &ncpu, &len, NULL, 0) == -1)
		perror("sysctl failed");

    ncpu=ncpu-ignorecpu;
    // there is always one cpu available
    if(ncpu<1) ncpu=1;

	// print result
	printf("%i\n", ncpu);
}
