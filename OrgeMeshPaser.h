#ifndef ORGEMESHPASER
#define ORGEMESHPASER
#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"
using namespace std;
#include <string>
#include "Vertex.h"


namespace OrgeMeshPaser
{
	vector<MyVertex::ModelData> parseMesh(string fileName);
	vector<MyVertex::ModelData> parseMeshUseShardedVertex(string filename);
	vector<MyVertex::ModelData> parseObjMesh(string fileName);
};
#endif;