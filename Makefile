# $Id$

all: upload.cgi delete.cgi

upload.cgi: upload.c
	gcc $? -o $@

delete.cgi: delete.c
	gcc $? -o $@
