#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>

template<class T>
class RDCSSDescriptor {
public:
	atomic<T*> a1;
	atomic<T*> a2;
	T o1, o2, n2;

	RDCSSDescriptor<T>() {
	}

	//RDCSSDescriptor<T>(T * a1, T o1, T * a2, T o2, T n2) {
	//	r->a1 = a1;
	//	r->o1 = o1;
	//	r->a2 = a2;
	//	r->o2 = o2;
	//	r->n2 = n2;
	//}
};

template<class T>
class RDCSS {
public: 
	T rdcss(RDCSSDescriptor<T>* d);
	T rdcssRead(T t);
	T CAS1(atomic<T*> a, T o, T n);
	void complete(RDCSSDescriptor<T>* d);
};

template<class T>
inline T RDCSS<T>::rdcss(RDCSSDescriptor<T>* d)
{
	T r;
	do {
		r = CAS1((d->a2), d->o2, (T)d); /* C1 */
		if (typeid(r) == typeid(RDCSSDescriptor))
			complete((RDCSSDescriptor*)r); /* H1 */
	} while (typeid(r) == typeid(d)); /* B1 */
	if (r == d->o2)
		complete((RDCSSDescriptor*)d);
	return T();
}

template<class T>
inline T RDCSS<T>::rdcssRead(T addr)
{
	T r;
	do {
		r = *addr; /* R1 */
		if (typeid(r) == typeid(RDCSSDescriptor))
			complete((RDCSSDescriptor*)r); /* H2 */
	} while (typeid(r)==typeid(RDCSSDescriptor)) /* B2 */
	return T();
}

template<class T>
inline T RDCSS<T>::CAS1(atomic<T*> a, T o, T n)
{
	T r = a.load();
	return r;
	//return __sync_val_compare_and_swap(a, o, n);
}

template<class T>
inline void RDCSS<T>::complete(RDCSSDescriptor<T>* d)
{
	T v;
	v = *(d->a1.load()); /* R2 */
	if (v == d->o1)
		CAS1(d->a2, (T)d, d->n2); /* C2 */
	else
		CAS1(d->a2, (T)d, d->o2); /* C3 */
}
