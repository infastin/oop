#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>

#include "Exception.h"
#include "Object.h"
#include "Selectors.h"

/*
 * Methods
 */

static void* Exception_ctor(void *_self, va_list *ap)
{
	struct Exception *self = super_ctor(Exception(), _self, ap);

	self->active = 0;
	self->depth = 0;
	self->obj = NULL;
	memset(self->buffers, 0, sizeof(jmp_buf*) * EXCEPTION_MAX_DEPTH);
	
	return self;
}

/*
 * Private methods
 */

static jmp_buf* Exception_buffer(struct Exception *self)
{
	if (self->depth == 0)
	{
		fprintf(stderr, "Fatal Error: Exception Buffer Out Of Bound!\n");
		abort();
	}

	return self->buffers[self->depth-1];
}


#if defined(__unix__)

static void Exception_backtrace(void)
{
	void* trace[EXCEPTION_MAX_TRACE];
	size_t trace_count = backtrace(trace, EXCEPTION_MAX_TRACE);
	char **symbols = backtrace_symbols(trace, trace_count);

	fprintf(stderr, "*** Stack Trace: \n");

	for (int i = 0; i < trace_count; i++)
	{
		fprintf(stderr, "[%d] %s\n", i, symbols[i]);
	}

	fprintf(stderr, "\n");
	free(symbols);
}

#elif defined(_WIN32)

static void Exception_Backtrace(void) 
{
	HANDLE process = GetCurrentProcess();
	HANDLE thread = GetCurrentThread();

	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&context);

	SymSetOptions(SYMOPT_UNDNAME|SYMOPT_LOAD_LINES);
	SymInitialize(process, NULL, TRUE);

	DWORD image;
	STACKFRAME64 stackframe;
	ZeroMemory(&stackframe, sizeof(STACKFRAME64));

#ifdef _M_IX86
	image = IMAGE_FILE_MACHINE_I386;
	stackframe.AddrPC.Offset = context.Eip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Ebp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Esp;
	stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_X64
	image = IMAGE_FILE_MACHINE_AMD64;
	stackframe.AddrPC.Offset = context.Rip;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.Rsp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.Rsp;
	stackframe.AddrStack.Mode = AddrModeFlat;
#elif _M_IA64
	image = IMAGE_FILE_MACHINE_IA64;
	stackframe.AddrPC.Offset = context.StIIP;
	stackframe.AddrPC.Mode = AddrModeFlat;
	stackframe.AddrFrame.Offset = context.IntSp;
	stackframe.AddrFrame.Mode = AddrModeFlat;
	stackframe.AddrBStore.Offset = context.RsBSP;
	stackframe.AddrBStore.Mode = AddrModeFlat;
	stackframe.AddrStack.Offset = context.IntSp;
	stackframe.AddrStack.Mode = AddrModeFlat;
#endif

	fprintf(stderr, "*** Stack Trace: \n");

	for (size_t i = 0; i < EXCEPTION_MAX_STRACE; i++) 
	{
		BOOL result = StackWalk64(
				image, process, thread,
				&stackframe, &context, NULL, 
				SymFunctionTableAccess64, SymGetModuleBase64, NULL);

		if (!result) break;

		char* filename = "";
		char* symbolname = "???";
		int lineno = 0;

		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO symbol = (PSYMBOL_INFO)buffer;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;

		DWORD64 displacement = 0;
		if (SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol))
			symbolname = symbol->Name;
		else
			symbolname = "???";

		IMAGEHLP_LINE64 line;
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		DWORD displacementline = 0;
		if (SymGetLineFromAddr64(process, 
					stackframe.AddrPC.Offset, &displacementline, &line)) 
		{
			lineno = line.LineNumber;
			filename = line.FileName;
		} 
		else 
		{
			lineno = 0;
			filename = "";
		}

		if (strcmp(filename, "") == 0)
			fprintf(stderr, "[%d] %s\n", i, symbolname);
		else
			fprintf(stderr, "[%d] %s: %d %s\n", i, filename, lineno, symbolname);
	}

	fprintf(stderr, "\n");

	SymCleanup(process);
}

#else

static void Exception_backtrace(void) {};

#endif

static void Exception_error(struct Exception *self)
{
	fprintf(stderr, "\n");
	fprintf(stderr, "*** Terminating app due to uncaught exception '%s'\n", self->obj->name);
	fprintf(stderr, "[*] At: %s:%d: %s\n", self->obj->file, self->obj->line, self->obj->func);
	fprintf(stderr, "[*] Reason: %s\n\n", self->obj->msg);

	Exception_backtrace();

	exit(EXIT_FAILURE);
}

/*
 * Exception `selectors`
 */

const void* exception_catch(void *_self, ...)
{
	struct Exception *self = cast(Exception(), _self);

	if (!self->active)
		return NULL;

	va_list ap;
	va_start(ap, _self);
	
	size_t len = 0;
	const void *arg;

	while((arg = va_arg(ap, const void*)) != 0)
	{
		if (arg == self->obj)
			return self->obj;

		len++;
	}

	va_end(ap);

	if (len == 0) return self->obj;

	if (self->depth >= 1)
		longjmp(*Exception_buffer(self), 1);
	else
		Exception_error(self);

	return NULL;
}

void exception_throw(void *_self, const void *_obj, char *file, int line, const char *func, char *fmt, ...)
{
	struct Exception *self = _cast("Exception()", "_self", file, line, func, Exception(), _self);
	struct ExceptionObject *obj = _cast("ExceptionObject()", "_obj", file, line, func, ExceptionObject(), _obj);

	if (obj->msg)
		free(obj->msg);

	va_list ap, ap_copy;
	
	va_start(ap, fmt);
	va_copy(ap_copy, ap);

	size_t size = vsnprintf(NULL, 0, fmt, ap);
	obj->msg = (char*)calloc(sizeof(char), size + 1);
	vsnprintf(obj->msg, size + 1, fmt, ap_copy);
	
	va_end(ap_copy);
	va_end(ap);

	obj->file = file;
	obj->line = line;
	obj->func = func;

	self->obj = obj;

	if(self->depth >= 1)
		longjmp(*Exception_buffer(self), 1);
	else
		Exception_error(self);
}

void exception_try(void *_self, jmp_buf *env)
{
	struct Exception *self = cast(Exception(), _self);

	if (self->depth == EXCEPTION_MAX_DEPTH)
	{
		fprintf(stderr, "Fatal Error: Exception Buffer Overflow!\n");
		abort();
	}

	self->buffers[self->depth] = env;
	self->depth++;
	self->active = 0;
}

void exception_try_end(void *_self)
{
	struct Exception *self = cast(Exception(), _self);

	if (self->depth == 0)
	{
		fprintf(stderr,  "Fatal Error: Exception Buffer Underflow!\n");
		abort();
	}

	self->depth--;
}

void exception_try_fail(void *_self)
{
	struct Exception *self = cast(Exception(), _self);
	self->active = 1;
}

/*
 * Initialization
 */

ClassImpl(Exception)
{
	if (!_Exception)
	{
		_Exception = new(Class(), "Exception",
				Object(), sizeof(struct Exception),
				ctor, Exception_ctor,
				0);
	}

	return _Exception;
}

ObjectImpl(GlobalException)
{
	if (!_GlobalException)
	{
		_GlobalException = new(Exception());
	}

	return _GlobalException;
}
