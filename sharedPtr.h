#ifndef SharedPtr_H
#define  SharedPtr_H

template<class TYPE> class Ptr
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
	/// safe pointer cast operator
	operator TYPE*() const;

	TYPE* get() const;
	/// return direct pointer (returns null pointer)
	TYPE* get_unsafe() const;

private:
	TYPE *ptr;
};

#endif