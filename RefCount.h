#ifndef RefCount_H
#define  RefCount_H

class  RefCount
{
public:
	 RefCount();
	~ RefCount();

public:
	void AddRef();
	void Release();

private:
	int count;
};

#endif