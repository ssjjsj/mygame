#include "sharedPtr.h"

template<class TYPE> Ptr<TYPE>::Ptr(TYPE* p)
{
	ptr = p;
	p->AddRef();
}

template<class TYPE> Ptr<TYPE>::~Ptr()
{
	ptr->Release();
}

template<class TYPE> TYPE* Ptr<TYPE>::get()
{
	return ptr;
}

template <class TYPE> Ptr::Ptr(const Ptr<TYPE>& p)
{
	ptr = p.get();
	ptr->AddRef();
}

template <class TYPE> void Ptr<TYPE>::operator=(const Ptr<TYPE>& p)
{
	if (ptr != p.get())
	{
		ptr->Release();
		ptr = p.get();
		ptr->AddRef();
	}
}

template <class TYPE> void Ptr<TYPE>::operator =(TYPE *p)
{
	if (p != ptr)
	{
		p->Release();
		ptr = p;
		ptr->AddRef();
	}
}

template<class TYPE> TYPE* Ptr<TYPE>::operator->()
{
	return ptr;
}
