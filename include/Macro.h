#ifndef MACRO_H_K08PG7ES
#define MACRO_H_K08PG7ES

/*
 * Base macro
 */

#define TOSTR0(X) #X
#define TOSTR(X) TOSTR0(X)

#define CAT0(X, Y) X##_##Y
#define CAT(X, Y) CAT0(X, Y)

#define CATS0(X, Y) X Y
#define CATS(X, Y) CATS0(X, Y)

/*
 * Class and object macro
 */

#define CH(T) extern const void* const T (void)
#define OH(T) extern void* const T (void)

#define ClassHeader(T) CH(T)
#define ObjectHeader(T) OH(T)

#define CI(T)  					\
	static const void *_ ## T; 	\
	const void* const T(void)

#define OI(T)  					\
	static void *_ ## T; 		\
	void* const T(void)

#define ClassImpl(T) CI(T)
#define ObjectImpl(T) OI(T)

/*
 * Template macro
 */

#define TEMPLATE(X, Y) CAT0(X, Y)

#define PRIVATE0(X) _##X
#define PRIVATE(X) PRIVATE0(X)

#endif /* end of include guard: MACRO_H_K08PG7ES */
