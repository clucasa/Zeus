//***************************************************************************************
// Apex.h
//
//
// 
//
//***************************************************************************************
#ifndef APEX_H
#define APEX_H

#include "PhysX.h"
#include "ApexParticles.h"

#include "NxApex.h"
#include "NxApexSDK.h"
#include "UserErrorCallback.h"
#include "SampleApexRenderer.h"

#include <vector>

#pragma comment(lib ,"ApexFramework_x86")

using namespace std;

class Apex
{
public:
    Apex();
    ~Apex();

    void Init(PxPhysics* physics, PxCooking* cooking, PxScene* scene);

    void InitParticles();

private:
    
    NxApexSDK*              gApexSDK;
    NxApexScene*            gApexScene;

    UserErrorCallback*		mErrorCallback;
    SampleRenderer::Renderer*            renderer;
    physx::apex::NxUserRenderResourceManager*	m_renderResourceManager;

    bool                    initialized;
    
};

#endif