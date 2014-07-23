#include "estk.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#ifndef __WIN32__
#define COL_BASE "\033[0m"
#define COL_INFO "\033[94m"
#define COL_SUCC "\033[92m"
#define COL_WARN "\033[93m"
#define COL_ERRO "\033[91m"
#endif

#define LOG_OUTPUT stderr

static esLogMessage log_cb = NULL;

void esLogCallback(esLogMessage callback) {
	log_cb = callback;
}

static const char *get_prefix(int class) {
	static int count = 0;
	static int atty = 0;

	if (count++ == 0) {
#ifndef __WIN32__
		atty = isatty(fileno(LOG_OUTPUT));
#endif
	}

	if (atty) {
		switch (class) {
			default :
			case ES_INFO : return COL_INFO "INFO" COL_BASE;
			case ES_WARN : return COL_WARN "WARN" COL_BASE;
			case ES_ERRO : return COL_ERRO "ERRO" COL_BASE;
			case ES_DEBU : return COL_WARN "DEBU" COL_BASE;
		}
	} else {
		switch (class) {
			default :
			case ES_INFO : return "INFO";
			case ES_WARN : return "WARN";
			case ES_ERRO : return "ERRO";
			case ES_DEBU : return "DEBU";
		}
	}
}

void esLog(int class, const char *fmt, ...) {
	char buf[200];
	va_list argPtr;

	va_start(argPtr, fmt);
	vsnprintf(buf, 200, fmt, argPtr);
	va_end(argPtr);

	if (log_cb) {
		log_cb(class, buf);
	} else {
		fprintf(LOG_OUTPUT, "estk %s: %s\n", get_prefix(class), buf);
	}
}

