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

	void Update();

private:
	vector<PathNode> indexPath;
	int curX, curY;

private:
	void clearPath();
	void moveTo(int x, int y);
};
#endif // !MOVEBEHAVIOUR_H
