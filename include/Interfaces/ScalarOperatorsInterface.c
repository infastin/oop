#include "ScalarOperatorsInterface.h"
#include "Selectors.h"
#include "Object.h"
#include "Exception.h"

void scadd(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scadd.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scadd' method of 'ScalarOperatorsInterface'!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	vscadd(_self, &ap);
	va_end(ap);
}

void vscadd(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scadd.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scadd' method of 'ScalarOperatorsInterface'!",
				class->name);

	typedef void  (*scadd_f)(void *self, va_list *ap);

	((scadd_f) sc->scadd.method)(_self, ap);
}

// Subtract scalar from the variable of some type
void scsub(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scsub.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scsub' method of 'ScalarOperatorsInterface'!",
				class->name);

	typedef void  (*scsub_f)(void *self, va_list *ap);

	va_list ap;
	va_start(ap, _self);
	((scsub_f) sc->scsub.method)(_self, &ap);
	va_end(ap);
}

void vscsub(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scsub.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scsub' method of 'ScalarOperatorsInterface'!",
				class->name);

	typedef void  (*scsub_f)(void *self, va_list *ap);

	((scsub_f) sc->scsub.method)(_self, ap);
}

// Multiply on scalar variable of some type
void scmulti(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scmulti.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scmulti' method of 'ScalarOperatorsInterface'!",
				class->name);
	
	typedef void  (*scmulti_f)(void *self, va_list *ap);

	va_list ap;
	va_start(ap, _self);
	((scmulti_f) sc->scmulti.method)(_self, &ap);
	va_end(ap);
}

void vscmulti(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scmulti.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scmulti' method of 'ScalarOperatorsInterface'!",
				class->name);

	typedef void  (*scmulti_f)(void *self, va_list *ap);

	((scmulti_f) sc->scmulti.method)(_self, ap);
}

// Divide by scalar variable of some type
void scdivide(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scdivide.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scdivide' method of 'ScalarOperatorsInterface'!",
				class->name);

	typedef void  (*scdivide_f)(void *self, va_list *ap);

	va_list ap;
	va_start(ap, _self);
	((scdivide_f) sc->scdivide.method)(_self, &ap);
	va_end(ap);
}

void vscdivide(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface, _self);

	if (sc->scdivide.method == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scdivide' method of 'ScalarOperatorsInterface'!",
				class->name);

	typedef void  (*scdivide_f)(void *self, va_list *ap);

	((scdivide_f) sc->scdivide.method)(_self, ap);
}

/*
 * Interface Initialization
 */

InterfaceImpl(ScalarOperatorsInterface)
{
	if (!_ScalarOperatorsInterface)
	{
		_ScalarOperatorsInterface = inew("ScalarOperatorsInterface", 0, 4,
				(voidf) scadd,
				(voidf) scsub,
				(voidf) scmulti,
				(voidf) scdivide);
	}

	return _ScalarOperatorsInterface;
}

/*
 * Exception Initialization
 */

ObjectImpl(ScalarOperatorsException)
{
	if (_ScalarOperatorsException)
	{
		_ScalarOperatorsException = new(ExceptionObject(), "ScalarOperatorsException");
	}

	return _ScalarOperatorsException;
}
