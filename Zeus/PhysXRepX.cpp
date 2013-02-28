//***************************************************************************************
// PhysXRepX.h
//***************************************************************************************
#include "PhysXRepX.h"



using namespace PxToolkit;

PhysXRepX::PhysXRepX()
{

}

PhysXRepX::~PhysXRepX()
{

}

void PhysXRepX::LoadCollection(const char* filename, PxPhysics& physics, PxCooking& cooking, PxScene& scene )
{
	PxStringTable* stringTable = NULL;
	RepXCollection* collection = loadCollection(filename);
	addObjectsToScene( *collection, physics, cooking, scene, stringTable );
}