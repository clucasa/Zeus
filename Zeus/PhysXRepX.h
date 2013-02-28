//***************************************************************************************
// PhysXRepX.h
//***************************************************************************************
#ifndef PHYSX_REPX_H
#define PHYSX_REPX_H

#include "PhysX.h"
#include < PxToolkit.h >

#include "PhysXRepX.h"

#pragma comment(lib, "RepXUpgrader3.lib")

using namespace std;
using namespace physx;

class PhysXRepX
{
public:
    PhysXRepX();
    ~PhysXRepX();

	void LoadCollection(const char* filename, PxPhysics& physics, PxCooking& cooking, PxScene& scene);

private:

    
};

#endif