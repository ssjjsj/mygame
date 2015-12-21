#ifndef ORGEMESHPASER
#define ORGEMESHPASER
#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"
using namespace std;
#include <string>
#include "Vertex.h"


class OrgeMeshPaser
{
public:
	vector<ModelData> parseMesh(string fileName);
	vector<ModelData> parseMeshUseShardedVertex(string filename);
};
#endif;