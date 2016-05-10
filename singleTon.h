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
#endif