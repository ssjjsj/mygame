#ifndef SingleTon_H
#define  SingleTon_H
template<class T> class Singleton
{
public:
	Singleton() {};
	~Singleton() { delete instace; };
	static T& Instance();
private:
	static T* instace;
};

template<typename T> T* Singleton<T>::instace = NULL;


template<class T> T& Singleton<T>::Instance()
{
	if (instace == NULL)
		instace = new T;
	return *instace;
}
#endif