#ifndef MOVEBEHAVIOUR_H
#define MOVEBEHAVIOUR_H

#include <vector>
using namespace std;

class MoveBehaviour
{
public:
	MoveBehaviour();
	~MoveBehaviour();

private:
	vector<XMFLOAT2> indexPath;
	int curX, curY;

private:
	void clearPath();
	void moveTo(int x, int y);
};
#endif // !MOVEBEHAVIOUR_H
