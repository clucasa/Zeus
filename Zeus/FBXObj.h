//***************************************************************************************
// FBXObj.h
//***************************************************************************************
#ifndef FBX_OBJ_H
#define FBX_OBJ_H

#include "importer.h"
#include <vector>
#include <D3D11.h>
#include "Vertex.h"

using namespace std;

struct objectInfo
{
	float x,y,z;
	float sx,sy,sz;
	float rx, ry, rz;
};


class FBXObj
{
public:
    FBXObj();
    ~FBXObj();

    void Import(char* filename, ID3D11Device* dev);
	void Import(string filename, ID3D11Device* dev);
	
	void LoadTexture(ID3D11Device* dev, string filename);
	void LoadTexture(ID3D11Device* dev, wchar_t* filename);
	void LoadNormal(ID3D11Device* dev, string filename);
	void LoadNormal(ID3D11Device* dev, wchar_t* filename);

	void LoadTextures(ID3D11Device* dev, vector<wchar_t*> filenames);
	void LoadTextures(ID3D11Device* dev, vector<string> filenames);
	void LoadNormals(ID3D11Device* dev, vector<wchar_t*> filenames);
	void LoadNormals(ID3D11Device* dev, vector<string> filename);

	void LoadObjectInfo(objectInfo objectInfo);
	void LoadObjectInfos(vector<objectInfo> objectInfos);

    UINT GetIndexCount();
    UINT GetVertexCount();
    vector<int> GetIndices();
    vector<XMFLOAT3> GetVertices();
    ID3D11Buffer* GetVB();
    ID3D11Buffer* GetIB();
	vector<objectInfo> GetObjectInfos();
	

    vector<ID3D11ShaderResourceView*> GetTextureArray();
    vector<ID3D11ShaderResourceView*> GetNormalArray();

private:

    
};

#endif