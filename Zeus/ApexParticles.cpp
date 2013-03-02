//***************************************************************************************
// ApexParticles.cpp
//***************************************************************************************
#include "ApexParticles.h"

ApexParticles::ApexParticles()
{

}

ApexParticles::~ApexParticles()
{

}

void ApexParticles::Init(NxApexSDK* gApexSDK)
{
    PX_ASSERT(gApexSDK);
    mParticleIosModule = static_cast<NxModuleParticleIos*>(gApexSDK->createModule("ParticleIOS"));
	PX_ASSERT(mParticleIosModule);
    if(mParticleIosModule)
    {
        NxParameterized::Interface* params = mParticleIosModule->getDefaultModuleDesc();
	    mParticleIosModule->init(*params);
    }

    mEmitterModule = static_cast<NxModuleEmitter*> ( gApexSDK->createModule("Emitter"));
    PX_ASSERT(mEmitterModule);
    if(mEmitterModule)
    {
        NxParameterized::Interface* params = mEmitterModule->getDefaultModuleDesc();
	    mEmitterModule->init(*params);
        PxU32 numScalables = mEmitterModule->getNbParameters();
	    NxApexParameter** m_emitterModuleScalables = mEmitterModule->getParameters();
	    for (physx::PxU32 i = 0; i < numScalables; i++)
	    {
		    NxApexParameter& p = *m_emitterModuleScalables[i];
		    mEmitterModule->setIntValue(i, p.range.maximum);
	    }
    }

    mIofxModule = static_cast<NxModuleIofx*>(gApexSDK->createModule("IOFX"));
	PX_ASSERT(mIofxModule);
	if (mIofxModule)
	{
		NxParameterized::Interface* params = mIofxModule->getDefaultModuleDesc();
		mIofxModule->init(*params);

		//m_apexIofxModule->disableCudaInterop();
		//m_apexIofxModule->disableCudaModifiers();
	}

}