#ifndef ORGEMESHPASER
#define ORGEMESHPASER
#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"
using namespace std;
#include <string>
#include "Vertex.h"


class OrgeMeshPaser
{
private:
	static OrgeMeshPaser paser;
public:
	static OrgeMeshPaser Instance()
	{
		return paser;
	}
	vector<ModelData> parseMesh(string fileName);
	vector<ModelData> parseMeshUseShardedVertex(string filename);
};
#endif;