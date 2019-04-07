#pragma once
#include "Definitions.h"
#include "MyEntityManager.h"
#include "MyOctant.h"
using namespace Simplex;

class Octree
{
	//typedef MyOctant* POctant;
	//POctant* octants;

public:
	Octree(uint levels);
	~Octree();
	void Display();
	uint levels;
	void checkCollisions();

private:
	MeshManager* meshManager;
	MyEntityManager* entityManager;
	uint octantCount;
	MyOctant* octants[585];

	//POctant* octants;
	//std::vector<MyOctant*> octants;
	void Subdivide(MyOctant* parent);
	void SetupNewOctant(MyOctant* parent, float height, vector3 center, float halfHeight);
};