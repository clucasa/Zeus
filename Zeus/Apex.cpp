//***************************************************************************************
// Apex.cpp
//***************************************************************************************
#include "Apex.h"

Apex::Apex()
{
    //gApexScene = NULL;
}

Apex::~Apex()
{
    //gApexScene->setPhysXScene((PxScene *)0);

    //// Now, it's safe to release the NxScene...
    //gApexScene->fetchResults(true, NULL);                 // ensure scene is not busy
    //gApexScene->release();

}
NxApexSDK*              gApexSDK;
    NxApexScene*            gApexScene;
void Apex::Init(PxPhysics* physics, PxCooking* cooking, PxScene* scene)
{
    NxApexSDKDesc     apexDesc;
    NxApexCreateError  errorCode;

    apexDesc.physXSDK = physics;
    apexDesc.cooking = cooking;

    //apexDesc.renderResourceManager = new NxUserRenderResourceManager
    if(!apexDesc.isValid())
    {
        return;
    }

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
    gApexParticles->Init(gApexSDK);
}
