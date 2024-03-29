/*-
 * Copyright (c) 2008 Sunry Chen
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
 *
 * $Id: delete.c,v 1.3 2008/12/22 10:25:02 student45 Exp student45 $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int 
main()
{
	char           *p;

	printf("Content type: text/html\n\n");

	/* For Debug Usage */
	/* printf("%s\n", getenv("QUERY_STRING")); */

	p = getenv("QUERY_STRING");

	for (; *p != '='; p++);
	p++;

	if (remove(p) != 0)
		printf("Error: delete guid file %s error\n", p);
	else
		printf("Done: delete guid file %s\n", p);

	if (remove(strcat(p, ".file")) != 0)
		printf("Error: delete %s error\n", p);
	else
		printf("Done: delete %s\n", p);
}
