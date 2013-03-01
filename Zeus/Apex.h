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
    

    
};

#endif