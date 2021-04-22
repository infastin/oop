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
#define IH(T) extern const void* const T (void)

#define InterfaceHeader(T) IH(T)

#define ClassHeader(T) CH(T)
#define ObjectHeader(T) OH(T)

#define CI(T)  					\
	static const void *_ ## T; 	\
	const void* const T(void)

#define OI(T)  					\
	static void *_ ## T; 		\
	void* const T(void)

#define II(T) 					\
	static const void *_ ## T; 	\
	const void* const T(void)

#define ClassImpl(T) CI(T)
#define ObjectImpl(T) OI(T)
#define InterfaceImpl(T) II(T)

#define go_to_field(self, structure, field) ((void*) ((char*) self + offsetof(structure, field)))

#define set_method(self, t, s, m) \
	((self)->s.tag = (t), (self)->s.selector = (voidf) (s), (self)->s.method = (voidf) (m))

#define init_method(self, s) \
	((self)->s.tag = TOSTR(s), (self)->s.selector = (voidf) (s), (self)->s.method = (voidf) NULL)

#define update_method(self, s, m) \
	((self)->s.method = (voidf) (m))

/*
 * Template macro
 */

#define TEMPLATE(X, Y) CAT0(X, Y)

#define PRIVATE0(X) _##X
#define PRIVATE(X) PRIVATE0(X)

/*
 * Array macro
 */
#define ARRAY_REMOVE(arr, len, index) (memmove(&arr[index], &arr[index + 1], sizeof *(arr) * (len - index - 1)), len--)

#endif /* end of include guard: MACRO_H_K08PG7ES */
