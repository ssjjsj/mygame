#include "singleTon.h"

template<class T> T& Singleton<T>::Instance()
{
	if (instace == NULL)
		instace = new T;
	return instace;
}

