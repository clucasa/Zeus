//*********************************************************
// importer.h
//********************************************************
#ifndef FBX_IMPORTER_H
#define FBX_IMPORTER_H
#include <fbxsdk.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <d3d10_1.h>
#include <D3D10.h>
#include <D3DX10.h>
#include <D3Dcompiler.h>
#include <xnamath.h>
#include <direct.h>
#include "Vertex.h"
using namespace std;

class FBXImporter
{
public:
    FBXImporter();
    ~FBXImporter();

	int Import( char* filename, vector<Vertex::PosNormalTexTan>* vert );
	void LoadScene(char* filename);
	void ProcessScene();
	void ProcessNode( FbxNode* node, int attributeType);
	void ProcessMesh( FbxNode* node );
	FbxVector2 GetTexCoords( FbxMesh* mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex );

    //void loadVectors(KFbxNode* pNode, int polygonCount, FbxMesh *mesh, FbxVector4* meshVertices);
    /*void DisplayContent(KFbxNode* pNode);
    void DisplayContent(FbxScene* pScene);
    int Import(char* filename, vector<XMFLOAT3> *vec, vector<int> *ind, vector<XMFLOAT3> *norm, vector<XMFLOAT2> *tex, vector<int> *num );
*/
};
#endif