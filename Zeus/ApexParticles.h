//***************************************************************************************
// ApexParticles.cpp
//
//
// 
//
//***************************************************************************************
#ifndef APEX_PARTICLES_H
#define APEX_PARTICLES_H

#include "Apex.h"
#include "NxModuleIofx.h"
#include "NxModuleEmitter.h"
#include "NxModuleParticleIos.h"

#include <vector>

using namespace std;

class ApexParticles
{
public:
    ApexParticles();
    ~ApexParticles();

    void Init(NxApexSDK* gApexSDK);

private:
    NxModuleParticleIos*        mParticleIosModule;
    NxModuleEmitter*            mEmitterModule;
    NxModuleIofx*               mIofxModule;
};

#endif