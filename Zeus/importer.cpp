//*********************************************************
// importer.cpp
//********************************************************
#include "importer.h"

/*
Make sure to include the fbx sdk and to link
fbxsdk-2013.3d.lib
or 
fbxsdk-2013.3.lib
*/

using namespace std;

FbxScene*	lFbxScene;
FbxManager* lSdkManager;
string		lFileName;

int size = 0;

void LoadScene(char* filename);
void ProcessScene();
void ProcessNode( FbxNode* node, int attributeType);
void ProcessMesh( FbxNode* node );
FbxVector2 GetTexCoords( FbxMesh* mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex );


vector<Vertex::PosNormalTexTan>* Vertices;
int TexCount = 0;

int meshMatMax = 0;

// We just use the main function to get the party started. From here we will call everything in a straight line hopefully
int FBXImporter::Import( char* filename, vector<Vertex::PosNormalTexTan>* vert )
{
	size = 0;
	TexCount = 0;

	Vertices = vert;

	LoadScene( filename );

	cout << "Finished" << endl;
	int a;
	cin >> a;

	return Vertices->size();
}


// Get the scene initialization running
void FBXImporter::LoadScene(char* filename)
{
	// Create the FBX SDK manager
	lSdkManager = FbxManager::Create();

	// Create an IOSettings object.
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
	lSdkManager->SetIOSettings(ios);

	// Create the scene
	lFbxScene = FbxScene::Create( lSdkManager, "" );

	// Create an Importer
	FbxImporter* lImporter = FbxImporter::Create( lSdkManager, "" );

	lImporter->Initialize( filename );
	lImporter->Import( lFbxScene );

	lFileName = lImporter->GetFileName().Buffer();

	lImporter->Destroy();

	ProcessScene();
}

void FBXImporter::ProcessScene()
{
	ProcessNode( lFbxScene->GetRootNode(), FbxNodeAttribute::eMesh );
}

void FBXImporter::ProcessNode(FbxNode* node, int attributeType)
{
	if( !node )
		return;

	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if( nodeAttribute )
	{
		switch( nodeAttribute->GetAttributeType() )
		{
		case FbxNodeAttribute::eMesh:
			ProcessMesh( node );
		}
	}

	for( int i = 0; i < node->GetChildCount(); i++ )
		ProcessNode( node->GetChild( i ), attributeType );

}

void FBXImporter::ProcessMesh( FbxNode* node )
{
	FbxGeometryConverter GeometryConverter( node->GetFbxManager() );
	if( !GeometryConverter.TriangulateInPlace( node ) )
		return;

	FbxMesh* mesh = node->GetMesh();
	if( !mesh )
		return;

	int vertexCount = mesh->GetControlPointsCount();
	if( vertexCount <= 0 )
		return;

	cout << node->GetName() << "\t" << vertexCount << endl;

	FbxVector4* controlPoints = mesh->GetControlPoints();

	for( int i = 0; i < mesh->GetPolygonCount(); ++i )
	{
		meshMatMax = 0;
		FbxLayerElementMaterial* pLayerMaterial = mesh->GetLayer(0)->GetMaterials();
		FbxLayerElementArrayTemplate<int> *tmpArray = &pLayerMaterial->GetIndexArray();
		if( tmpArray->GetAt( i ) > meshMatMax )
			meshMatMax = tmpArray->GetAt( i );

		for( int j = 0; j < 3; ++j )
		{
			int vertexIndex = mesh->GetPolygonVertex( i, j );

			if( vertexIndex < 0 || vertexIndex >= vertexCount )
				continue;

			// Get vertices
			FbxVector4 position = controlPoints[vertexIndex];

			// Get normal
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal( i, j, normal );

			// Get TexCoords
			FbxVector2 texcoord = GetTexCoords( mesh, 0, i, j, vertexIndex );
			
			Vertex::PosNormalTexTan temp;
			temp.Pos = XMFLOAT3( position[0], position[1], position[2] );
			temp.Normal   = XMFLOAT3( normal[0], normal[1], normal[2] );
			temp.Tex = XMFLOAT2( texcoord[0], 1.0 - texcoord[1] );
			temp.TexNum = tmpArray->GetAt(i) + TexCount;

			// Add to the Vector
			Vertices->push_back( temp );
		}

	}
	TexCount += meshMatMax+1;
}

FbxVector2 FBXImporter::GetTexCoords( FbxMesh* mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex )
{
	int layerCount = mesh->GetLayerCount();

	if( layerIndex < layerCount )
	{
		FbxLayer* layer = mesh->GetLayer( layerIndex );

		if( layer )
		{
			FbxLayerElementUV* uv = layer->GetUVs( FbxLayerElement::eTextureDiffuse );

			if( uv )
			{
				FbxLayerElement::EMappingMode mappingMode = uv->GetMappingMode();
				FbxLayerElement::EReferenceMode referenceMode = uv->GetReferenceMode();

				const FbxLayerElementArrayTemplate<KFbxVector2>& pUVArray = uv->GetDirectArray();
				const FbxLayerElementArrayTemplate<int>& pUVIndexArray = uv->GetIndexArray();

				switch(mappingMode)
				{
				case FbxLayerElement::eByControlPoint:
					{
						int mappingIndex = vertexIndex;
						switch(referenceMode)
						{
						case KFbxLayerElement::eDirect:
							if( mappingIndex < pUVArray.GetCount() )
							{
								return pUVArray.GetAt( mappingIndex );
							}
							break;
						case KFbxLayerElement::eIndexToDirect:
							if( mappingIndex < pUVIndexArray.GetCount() )
							{
								int nIndex = pUVIndexArray.GetAt( mappingIndex );
								if( nIndex < pUVArray.GetCount() )
								{
									return pUVArray.GetAt( nIndex );
								}
							}
							break;
						};
					}
					break;

				case KFbxLayerElement::eByPolygonVertex:
					{
						int mappingIndex = mesh->GetTextureUVIndex( polygonIndex, polygonVertexIndex, KFbxLayerElement::eTextureDiffuse );
						switch(referenceMode)
						{
						case KFbxLayerElement::eDirect:
						case KFbxLayerElement::eIndexToDirect: //I have no idea why the index array is not used in this case.
							if( mappingIndex < pUVArray.GetCount() )
							{
								return pUVArray.GetAt( mappingIndex );
							}
							break;
						};
					}
					break;
				}
			}
		}
		return FbxVector2();
	}
}

//
////const char* lFilename = "traingles.fbx";
//const char* lFilename;
//
//int size;
//int cornerIndex;
//int *index;
//
//int texNo = -1;
//
//FbxVector4 fbxVertex;
//FbxVector4 fbxNormal;
//FbxVector2 fbxUV;
//
//vector<XMFLOAT3> *vertices;
//vector<XMFLOAT3> *normals;
//vector<XMFLOAT2> *texcoords;
//vector<int>	 *indices;
//vector<int>    *texnum;
//
FBXImporter::FBXImporter()
{
}

FBXImporter::~FBXImporter()
{
}
//
//// recursive DisplayContent Where the magic happens
//void FBXImporter::DisplayContent(KFbxNode* pNode)
//{
//	FbxVector4 v4; // Default translation values
//	cout << "Child name: " << pNode->GetName() << endl;
//
//	FbxMesh *mesh = pNode->GetMesh();
//
//	if(mesh)
//	{
//		FbxVector4* meshVertices = mesh->GetControlPoints();
//		int polygonCount = mesh->GetPolygonCount();
//		cout << "Polygon Count: " << polygonCount << endl;
//
//		int numIndices=mesh->GetPolygonVertexCount();
//		index = new int[numIndices];
//		index = mesh->GetPolygonVertices();
//		cout << "Num Indices: " << numIndices << endl;
//
//		//Because our engine doesnt support multiple meshes
//		size = vertices->size();
//
//		for( int i = 0; i< polygonCount; i++)
//		{
//			int TriSize = mesh->GetPolygonSize(i);//Should be 3
//
//			for( int j =0; j<TriSize; j++)
//			{			
//				int index = mesh->GetPolygonVertex(i,j);
//				indices->push_back(index+size);
//
//			}
//			//cout << endl;
//		}
//
//		//loadVectors( pNode, polygonCount, mesh, meshVertices );
//		int i, lControlPointsCount = mesh->GetControlPointsCount();
//		FbxVector4* lControlPoints = mesh->GetControlPoints();
//
//		if( lControlPointsCount > 0 )
//			texNo++;
//
//		//		cout << "    Control Points" << endl;
//
//		for (i = 0; i < lControlPointsCount; i++)
//		{
//			//** VERTICES **//
//			vertices->push_back( XMFLOAT3((float)lControlPoints[i][0], (float)lControlPoints[i][1], (float)lControlPoints[i][2]));
//			texnum->push_back( texNo );
//
//			//** NORMALS **//
//			for (int j = 0; j < mesh->GetElementNormalCount(); j++)
//			{
//				FbxGeometryElementNormal* leNormals = mesh->GetElementNormal( j);
//
//				if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
//				{
//					char header[100];
//					//					cout << "            normal Vector: "; 
//					if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
//					{
//						normals->push_back( XMFLOAT3( leNormals->GetDirectArray().GetAt(i)[0], 
//						leNormals->GetDirectArray().GetAt(i)[1], 
//						leNormals->GetDirectArray().GetAt(i)[2] ));
//					}
//				}
//			}
//		}
//
//	}
//	texcoords->resize(vertices->size());
//
//	if( mesh )
//	{
//		FbxStringList lUVSetNameList;
//		mesh->GetUVSetNames(lUVSetNameList);
//
//		for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
//		{
//			//get lUVSetIndex-th uv set
//			const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
//			const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);
//
//			if(!lUVElement)
//				continue;
//
//			// only support mapping mode eByPolygonVertex and eByControlPoint
//			if( lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
//				lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint )
//				return;
//
//			//index array, where holds the index referenced to the uv data
//			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
//			const int lIndexCount= (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
//
//			//iterating through the data by polygon
//			const int lPolyCount = mesh->GetPolygonCount();
//
//			if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
//			{
//				int lPolyIndexCounter = 0;
//				for( int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex )
//				{
//					// build the max index array that we need to pass into MakePoly
//					const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
//					for( int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex )
//					{
//						if (lPolyIndexCounter < lIndexCount)
//						{
//							FbxVector2 lUVValue;
//
//							//the UV index depends on the reference mode
//							int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
//
//							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
//							int lControlPointIndex = mesh->GetPolygonVertex(lPolyIndex, lVertIndex);
//
//							cout << lControlPointIndex+size << ": " << lUVValue[0] << " " << lUVValue[1] << " " << size << endl;
//
//							texcoords[0][lControlPointIndex+size] = XMFLOAT2( (float)lUVValue[0], 1.0f-(float) lUVValue[1] );
//
//							lPolyIndexCounter++;
//						}
//					}
//				}
//			}
//		}
//	}
//	for(int i = 0; i < pNode->GetChildCount(); i++)
//	{
//		DisplayContent(pNode->GetChild(i));
//	}
//}
//
//// Non-recursive DisplayContent() is called once
//void FBXImporter::DisplayContent(FbxScene* pScene)
//{
//	cout << "Starting Recursion" << endl;
//
//	int i;
//	FbxNode* lNode = pScene->GetRootNode();
//	if(lNode)
//	{
//		for(i = 0; i < lNode->GetChildCount(); i++)
//		{
//			// Call recursive DisplayContent() once for each child of the root node
//			DisplayContent(lNode->GetChild(i));
//		}
//	}
//
//	cout << "Done" << endl;
//}
//
//
//int FBXImporter::Import(char *name, vector<XMFLOAT3> *vec, vector<int> *ind, vector<XMFLOAT3> *norm, vector<XMFLOAT2> *tex, vector<int> *num )
//{
//
//	texNo = -1;
//	//	char* lFilename = "traingles.fbx";
//	lFilename = name;
//
//	// point to the vectors for changes to be made
//	vertices = vec;
//	indices = ind;
//	normals = norm;
//	texcoords = tex;
//	texnum = num;
//
//	//const std::string &fileName = "sadirohyea.fbx";
//	FbxVector4 fbxVertex;
//	FbxVector4 fbxNormal;
//	FbxVector2 fbxUV;
//	int cornerIndex;
//	// Create the FBX SDK manager
//	FbxManager* lSdkManager = FbxManager::Create();
//
//	// Create an IOSettings object.
//	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
//	lSdkManager->SetIOSettings(ios);
//
//	// Create an importer.
//	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
//
//	// Initialize the importer.
//	bool lImportStatus = lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings());
//
//	// Check to make sure the file exists
//	if(!lImportStatus) {
//		printf("Call to FbxImporter::Initialize() failed.\n");
//		printf("Error returned: %s\n\n", lImporter->GetLastErrorString());
//		cin >> cornerIndex;
//		exit(-1);
//	}
//
//	// Create a new scene so it can be populated by the imported file.
//	FbxScene* lScene = FbxScene::Create(lSdkManager, "MyScene");
//
//	// Import the contents of the file into the scene.
//	lImporter->Import(lScene);
//
//	// The file has been imported; we can get rid of the importer.
//	lImporter->Destroy();
//
//	FbxNode* rootNode = lScene->GetRootNode();
//	if(!rootNode)
//		cout << "failed. no rootNode" << endl;
//
//	// Traverse through the trees
//	DisplayContent( lScene );
//
//	return texNo;
//}