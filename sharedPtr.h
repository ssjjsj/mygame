#ifndef SharedPtr_H
#define  SharedPtr_H

template<typename TYPE> class Ptr
{
public:
	Ptr();
	/// construct from C++ pointer
	Ptr(TYPE* p);
	/// construct from smart pointer
	Ptr(const Ptr<TYPE>& p);
	/// destructor
	~Ptr();
	/// assignment operator
	void operator=(const Ptr<TYPE>& rhs);
	/// assignment operator
	void operator=(TYPE* rhs);
	/// equality operator
	bool operator==(const Ptr<TYPE>& rhs) const;
	/// inequality operator
	bool operator!=(const Ptr<TYPE>& rhs) const;
	/// shortcut equality operator
	bool operator==(const TYPE* rhs) const;
	/// shortcut inequality operator
	bool operator!=(const TYPE* rhs) const;
	/// safe -> operator
	TYPE* operator->() const;
	/// safe dereference operator
	TYPE& operator*() const;

	TYPE* get() const;

private:
	TYPE *ptr;
};

template<typename TYPE> Ptr<TYPE>::Ptr()
{
	ptr = NULL;
}

template<typename TYPE> Ptr<TYPE>::Ptr(TYPE* p)
{
	ptr = p;
	if (ptr != NULL)
		ptr->AddRef();
}

template<typename TYPE> Ptr<TYPE>::~Ptr()
{
	if (ptr != NULL)
		ptr->Release();
}

template<typename TYPE> TYPE* Ptr<TYPE>::get() const
{
	return ptr;
}

template <typename TYPE> Ptr<TYPE>::Ptr(const Ptr<TYPE>& p)
{
	ptr = p.get();
	if (ptr != NULL)
		ptr->AddRef();
}

template <typename TYPE> void Ptr<TYPE>::operator=(const Ptr<TYPE>& p)
{
	if (ptr != p.get())
	{
		if (ptr != NULL)
			ptr->Release();
		ptr = p.get();
		if (ptr != NULL)
			ptr->AddRef();
	}
}


template <typename TYPE> bool Ptr<TYPE>::operator==(const Ptr<TYPE>& p) const
{
	return ptr == p.get();
}

template <typename TYPE> bool Ptr<TYPE>::operator!=(const Ptr<TYPE>& p) const
{
	return ptr != p.get();
}

template <typename TYPE> void Ptr<TYPE>::operator=(TYPE *p)
{
	if (p != ptr)
	{
		if (ptr != NULL)
			ptr->Release();
		ptr = p;
		if (ptr != NULL)
			ptr->AddRef();
	}
}

template<typename TYPE> TYPE* Ptr<TYPE>::operator->() const
{
	return ptr;
}


template<typename TYPE> TYPE& Ptr<TYPE>::operator*() const
{
	return *ptr;
}

#endif