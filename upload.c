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
 * $Id: upload.c,v 1.3 2008/12/22 10:25:55 student45 Exp student45 $
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#define CONTENT_TYPE getenv("CONTENT_TYPE")

int
main()
{
	FILE           *fp;
	char           *input;
	int		content_length;
	char           *content_type;
	char           *boundary;
	char           *p;
	char           *temp_string;
	int		temp_length;
	char		filename  [200];
	char           *filename_no_d;
	int		i;
	int		got_it = 0;
	struct tm      *tm1;

	printf("Content type: text/html\n\n");

	content_type = CONTENT_TYPE;
	temp_string = "boundary=";
	temp_length = strlen(temp_string);

	for (p = content_type; p != NULL && *p != ' '; p++) {
		if (!strncmp(p + 2, temp_string, temp_length)) {
			boundary = p + 2 + temp_length;
			break;
		}
	}

	if (boundary == NULL) {
		printf("Input Error: Wrong form data type, check form/data!");
		exit(1);
	}
	if (!strcmp(getenv("REQUEST_METHOD"), "POST")) {
		if (getenv("CONTENT_LENGTH") != NULL) {
			content_length = atoi(getenv("CONTENT_LENGTH"));
			input = malloc(sizeof(char) * content_length + 1);
			if (fread(input, sizeof(char), content_length, stdin) != content_length) {
				printf("error: input length <CONTENT_LENGTH ");
				exit(1);
			}
		} else {
			printf("content_length is zero: CONTENT_LENGTH is zero");
			exit(1);
		}
	} else {
		printf("input method error: method should be POST,but yours is not");
		exit(1);
	}

	/* For Debug Usage */
	/* printf("%s\n", input); */

	for (p = input; got_it == 0; p++)
		if (strncmp(p, "filename=", strlen("filename=")) == 0) {
			p += strlen("filename=") + 1;
			for (i = 0; i >= 0; i++) {
				if (*p == '"')
					break;
				filename[i] = *(p++);
			}
			filename[i] = '\0';
			got_it = 1;
			break;
		}
	/* Cut directory name if uploaded by IE. */
	filename_no_d = filename;
	if (strstr(filename, "\\") != NULL)
		for (filename_no_d = strstr(filename, "\\"); strstr(filename_no_d, "\\") != NULL; filename_no_d++) {
			filename_no_d = strstr(filename_no_d, "\\");
		}

	/* Find file content start point. */
	for (; p != "\n"; p++) {
		if (strncmp(p, "Content_Type", strlen("Content_Type")) == 0)
			p++;
		if (strncmp(p, "\r\n\r\n", 4) == 0) {
			p += 4;
			break;
		}
	}

	pid_t		pid;
	char           *clientport;
	time_t		ctime;
	char		guid      [50];
	ctime = time(NULL);

	if ((pid = getpid()) < 0) {
		perror("unable to get pid");
	}
	clientport = getenv("REMOTE_PORT");
	tm1 = gmtime(&ctime);
	sprintf(guid, "%d", tm1->tm_year + 1900);
	if (tm1->tm_mon >= 9)
		sprintf(guid, "%s%d", guid, tm1->tm_mon + 1);
	else
		sprintf(guid, "%s0%d", guid, tm1->tm_mon + 1);
	if (tm1->tm_mday >= 10)
		sprintf(guid, "%s%d", guid, tm1->tm_mday);
	else
		sprintf(guid, "%s0%d", guid, tm1->tm_mday);
	if (tm1->tm_hour >= 10)
		sprintf(guid, "%s%d", guid, tm1->tm_hour);
	else
		sprintf(guid, "%s0%d", guid, tm1->tm_hour);
	if (tm1->tm_min >= 10)
		sprintf(guid, "%s%d", guid, tm1->tm_min);
	else
		sprintf(guid, "%s0%d", guid, tm1->tm_min);
	if (tm1->tm_sec >= 10)
		sprintf(guid, "%s%d", guid, tm1->tm_sec);
	else
		sprintf(guid, "%s0%d", guid, tm1->tm_sec);
	sprintf(guid, "%s_%s_%d_%s", guid, getenv("SERVER_NAME"), pid, clientport);
	printf("%s\n", guid);

	if ((fp = fopen(guid, "w+")) == NULL) {
		printf("open guid file error");
		exit(1);
	}
	/* Write file content to local file. */
	for (; strncmp(p + 4, boundary, strlen(boundary)) != 0; p++)
		fputc(*p, fp);
	free(input);
	fclose(fp);

	if ((fp = fopen(strcat(guid, ".file"), "w+")) == NULL) {
		printf("open .file file error");
		exit(1);
	}
	fputs(filename_no_d, fp);
	fclose(fp);

	return 0;
}
