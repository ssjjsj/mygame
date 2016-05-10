#ifndef Behaviour_H
#define Behaviour_H

class Behaviour
{
public:
	Behaviour();
	~Behaviour();

	virtual void Start();
	virtual void Update();
	virtual void Destroy();
};
#endif // !Behaviour_H
