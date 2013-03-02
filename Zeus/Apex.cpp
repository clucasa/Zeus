//***************************************************************************************
// Apex.cpp
//***************************************************************************************
#include "Apex.h"

Apex::Apex() :
    gApexSDK(NULL),
    gApexScene(NULL)
{
    //gApexScene = NULL;
    initialized = false;
}

Apex::~Apex()
{
    //gApexScene->setPhysXScene((PxScene *)0);

    //// Now, it's safe to release the NxScene...
    //gApexScene->fetchResults(true, NULL);                 // ensure scene is not busy
    //gApexScene->release();

}

void Apex::Init(PxPhysics* physics, PxCooking* cooking, PxScene* scene)
{
    mErrorCallback = ::new UserErrorCallback("ApexErrors.txt", "a",true, true);
    renderer = SampleRenderer::Renderer;
    m_renderResourceManager = new SampleApexRenderResourceManager(*renderer);

    NxApexSDKDesc     apexDesc;
    NxApexCreateError  errorCode;

    apexDesc.physXSDK = physics;
    apexDesc.cooking = cooking;
    apexDesc.outputStream = mErrorCallback;
    apexDesc.renderResourceManager = m_renderResourceManager;
    if(!apexDesc.isValid())
    {
        return;
    }
    initialized = true;
    gApexSDK = NxCreateApexSDK(apexDesc, &errorCode);
    PX_ASSERT(gApexSDK);
    // Create the APEX scene
    NxApexSceneDesc        apexSceneDesc;
    apexSceneDesc.scene = scene;
    if (!apexSceneDesc.isValid())
	{
		return;
	}
    gApexScene = gApexSDK->createScene(apexSceneDesc);
    if (!gApexScene)
	{
		return;
	}
    
}

void Apex::InitParticles()
{
    ApexParticles* gApexParticles = new ApexParticles();
    if(initialized)
        gApexParticles->Init(gApexSDK);
}
