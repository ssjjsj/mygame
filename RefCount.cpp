#include "RefCount.h"

RefCount::RefCount()
	:count(0)
{

}

RefCount::~RefCount()
{

}



void RefCount::AddRef()
{
	count++;
}

void RefCount::Release()
{
	count--;
	if (count == 0)
		delete this;
}