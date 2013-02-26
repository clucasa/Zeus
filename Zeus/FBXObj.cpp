//***************************************************************************************
// FBXObj.cpp
//***************************************************************************************
#include "FBXObj.h"



FBXObj::FBXObj() : 
    mVB(0),
    mIB(0),
    mIndexCount(0),
    mVertexCount(0)
{

}

FBXObj::~FBXObj()
{
    /*if(mVB)
    {
        mVB->Release();
    }

    if(mIB)
    {
        mIB->Release();
    }

    mTextureArray.clear();
    mNormalArray.clear();*/
}


void FBXObj::Import(char* filename, ID3D11Device* dev)
{
    vector<XMFLOAT3> positions;
    vector<int> indices;
    vector<XMFLOAT3> norms;
	vector<XMFLOAT2> texVec;
    vector<int> texNum;

    mFBXImporter.Import(filename, &positions, &indices, &norms, &texVec, &texNum);

    std::vector<Vertex::PosNormalTexTan> vertices;
    Vertex::PosNormalTexTan tempVert;

	XMVECTOR p;
	XMVECTOR n;
	for(int i = 0; i < positions.size(); i++)
	{
		p = XMVectorSet(positions[i].x, positions[i].y,positions[i].z, 1.0 );
		n = XMVectorSet(norms[i].x, norms[i].y, norms[i].z, 1.0 );
		XMVECTOR cross = XMVector3Cross(p, n);
		XMStoreFloat3(&tempVert.TangentU, cross);

		tempVert.Pos = positions[i];
		tempVert.Normal = norms[i];
		tempVert.Tex = texVec[i];
		tempVert.TexNum = texNum[i];

		vertices.push_back(tempVert);
	}

    mIndexCount = indices.size();
    mVertexCount = positions.size();
    mVertices = positions;
    mIndices = indices;

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * positions.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(dev->CreateBuffer(&vbd, &vinitData, &mVB));

    //
    // Pack the indices of all the meshes into one index buffer.
    //

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * mIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(dev->CreateBuffer(&ibd, &iinitData, &mIB));
}


void FBXObj::Import(string filename, ID3D11Device* dev)
{
	char *cstr = new char[filename.length() + 1];
	strcpy(cstr, filename.c_str());

	Import(cstr,dev);

	delete [] cstr;
}

void FBXObj::LoadTexture(ID3D11Device* dev, string filename)
{
	wchar_t* wide_string = new wchar_t[ filename.length() + 1 ];
	std::copy( filename.begin(), filename.end(), wide_string );
	wide_string[ filename.length() ] = 0;

	LoadTexture( dev, wide_string );

	delete [] wide_string;
}

void FBXObj::LoadTexture(ID3D11Device* dev, wchar_t* filename)
{
	ID3D11ShaderResourceView* texture;
	HR(D3DX11CreateShaderResourceViewFromFile(dev, 
        filename, 0, 0, &texture, 0 ));
	mTextureArray.push_back(texture);
}

void FBXObj::LoadNormal(ID3D11Device* dev, string filename)
{
	wchar_t* wide_string = new wchar_t[ filename.length() + 1 ];
	std::copy( filename.begin(), filename.end(), wide_string );
	wide_string[ filename.length() ] = 0;

	LoadNormal( dev, wide_string );

	delete [] wide_string;
}

void FBXObj::LoadNormal(ID3D11Device* dev, wchar_t* filename)
{
	ID3D11ShaderResourceView* texture;
	HR(D3DX11CreateShaderResourceViewFromFile(dev, 
        filename, 0, 0, &texture, 0 ));
	mNormalArray.push_back(texture);
}


void FBXObj::LoadTextures(ID3D11Device* dev, vector<wchar_t*> filenames)
{
	for(int i = 0; i < filenames.size() ; i++)
	{
		ID3D11ShaderResourceView* texture;
		HR(D3DX11CreateShaderResourceViewFromFile(dev, 
			filenames[i], 0, 0, &texture, 0 ));
		mTextureArray.push_back(texture);
	}
}

void FBXObj::LoadNormals(ID3D11Device* dev, vector<wchar_t*> filenames)
{
	for(int i = 0; i < filenames.size() ; i++)
	{
		ID3D11ShaderResourceView* texture;
		HR(D3DX11CreateShaderResourceViewFromFile(dev, 
			filenames[i], 0, 0, &texture, 0 ));
		mNormalArray.push_back(texture);
	}
}

void FBXObj::LoadTextures(ID3D11Device* dev, vector<string> filenames)
{
	for(int i = 0; i < filenames.size() ; i++)
	{
		wchar_t* wide_string = new wchar_t[ filenames[i].length() + 1 ];
		std::copy( filenames[i].begin(), filenames[i].end(), wide_string );
		wide_string[ filenames[i].length() ] = 0;
		ID3D11ShaderResourceView* texture;
		HR(D3DX11CreateShaderResourceViewFromFile(dev, 
			wide_string, 0, 0, &texture, 0 ));
		mTextureArray.push_back(texture);
	}
}

void FBXObj::LoadNormals(ID3D11Device* dev, vector<string> filenames)
{
	for(int i = 0; i < filenames.size() ; i++)
	{
		wchar_t* wide_string = new wchar_t[ filenames[i].length() + 1 ];
		std::copy( filenames[i].begin(), filenames[i].end(), wide_string );
		wide_string[ filenames[i].length() ] = 0;
		ID3D11ShaderResourceView* texture;
		HR(D3DX11CreateShaderResourceViewFromFile(dev, 
			wide_string, 0, 0, &texture, 0 ));
		mNormalArray.push_back(texture);
	}
}

void FBXObj::LoadObjectInfo(objectInfo objectInfo)
{
	mInformations.push_back(objectInfo);
}

void FBXObj::LoadObjectInfos(vector<objectInfo> objectInfos)
{
	for(int i = 0; i < objectInfos.size() ; i++)
	{
		mInformations.push_back(objectInfos[i]);
	}
}

UINT FBXObj::GetIndexCount()
{
    return mIndexCount;
}

UINT FBXObj::GetVertexCount()
{
    return mVertexCount;
}

vector<int> FBXObj::GetIndices()
{
    return mIndices;
}

vector<XMFLOAT3> FBXObj::GetVertices()
{
    return mVertices;
}

ID3D11Buffer* FBXObj::GetVB()
{
    return mVB;
}

ID3D11Buffer* FBXObj::GetIB()
{
    return mIB;
}

vector<ID3D11ShaderResourceView*> FBXObj::GetTextureArray()
{
    return mTextureArray;
}

vector<ID3D11ShaderResourceView*> FBXObj::GetNormalArray()
{
    return mNormalArray;
}

vector<objectInfo> FBXObj::GetObjectInfos()
{
	return mInformations;
}
