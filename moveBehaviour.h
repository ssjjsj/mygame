#ifndef MOVEBEHAVIOUR_H
#define MOVEBEHAVIOUR_H

#include <vector>
#include "worldMap.h"
using namespace std;

class MoveBehaviour : public Behaviour
{
public:
	MoveBehaviour();
	~MoveBehaviour();

	void Update(float deltaTime);
	void SetPos(int x, int y);
	void moveTo(int x, int y);

private:
	vector<PathNode> indexPath;
	int curX, curY;
	float totalDelta;

private:
	void clearPath();
	void changeDirection(int x, int y);
};
#endif // !MOVEBEHAVIOUR_H
