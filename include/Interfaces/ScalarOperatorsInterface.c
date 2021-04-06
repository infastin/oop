#include "ScalarOperatorsInterface.h"
#include "Selectors.h"
#include "Object.h"
#include "Exception.h"

void scadd(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scadd == NULL)
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
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scadd == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scadd' method of 'ScalarOperatorsInterface'!",
				class->name);

	sc->scadd(_self, ap);
}

// Subtract scalar from the variable of some type
void scsub(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scsub == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scsub' method of 'ScalarOperatorsInterface'!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	sc->scsub(_self, &ap);
	va_end(ap);
}

void vscsub(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scsub == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scsub' method of 'ScalarOperatorsInterface'!",
				class->name);

	sc->scsub(_self, ap);
}

// Multiply on scalar variable of some type
void scmulti(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scmulti == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scmulti' method of 'ScalarOperatorsInterface'!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	sc->scmulti(_self, &ap);
	va_end(ap);
}

void vscmulti(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scmulti == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scmulti' method of 'ScalarOperatorsInterface'!",
				class->name);

	sc->scmulti(_self, ap);
}

// Divide by scalar variable of some type
void scdivide(void *_self, ...)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scdivide == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scdivide' method of 'ScalarOperatorsInterface'!",
				class->name);

	va_list ap;
	va_start(ap, _self);
	sc->scdivide(_self, &ap);
	va_end(ap);
}

void vscdivide(void *_self, va_list *ap)
{
	const struct Class *class = classOf(_self);
	const struct ScalarOperatorsInterface *sc = icast(ScalarOperatorsInterface(), class);

	if (sc->scdivide == NULL)
		throw(ScalarOperatorsException(), "Error: Class '%s' doesn't implement 'scdivide' method of 'ScalarOperatorsInterface'!",
				class->name);

	sc->scdivide(_self, ap);
}

/*
 * Interface Initialization
 */

InterfaceImpl(ScalarOperatorsInterface)
{
	if (!_ScalarOperatorsInterface)
	{
		_ScalarOperatorsInterface = inew("ScalarOperatorsInterface", 0);
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
