#ifndef Behaviour_H
#define Behaviour_H

class GameObject;

class Behaviour
{
public:
	Behaviour();
	~Behaviour();

	GameObject *gameObject;

	virtual void Start();
	virtual void Update();
	virtual void Destroy();
};
#endif // !Behaviour_H
