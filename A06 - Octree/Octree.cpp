#include "Octree.h"

Octree::Octree(uint levels)
{
	this->levels = levels;
	meshManager = MeshManager::GetInstance();
	// Find the min and max of all entities
	entityManager = MyEntityManager::GetInstance();
	uint entityCount = entityManager->GetEntityCount();
	std::vector<vector3> minMaxOfAllEntities;

	// Create RB around all enetities to get size of root octant
	for (uint i = 0; i < entityCount; i++)
	{
		MyEntity* entity = entityManager->GetEntity(i);
		MyRigidBody* rb = entity->GetRigidBody();
		minMaxOfAllEntities.push_back(rb->GetMaxGlobal());
		minMaxOfAllEntities.push_back(rb->GetMinGlobal());
	}
	MyRigidBody* rootRB = new MyRigidBody(minMaxOfAllEntities);
	vector3 center = rootRB->GetCenterGlobal();
	vector3 rbHalfWidth = rootRB->GetHalfWidth();
	float halfHeight = rbHalfWidth.x;
	for (uint i = 0; i < 3; i++)
	{
		if (halfHeight < rbHalfWidth[i]) {
			halfHeight = rbHalfWidth[i];
		}
	}

	// Create root octant, give it ID, and set min / max vectors
	octants[0] = new MyOctant(1, halfHeight * 2, center, true, levels);
	if (levels == 1) { octants[0]->setLeafTrue(); }
	octantCount++;

	//octants.push_back(new MyOctant(1, halfHeight * 2, center, true, levels));
	octants[0]->onctantID = 0;
	octants[0]->setMaxVector(center + rbHalfWidth);
	octants[0]->setMinVector(center - rbHalfWidth);

	// Put all entities in root octant entity list
	for (uint i = 0; i < entityCount; i++)
	{
		octants[0]->entitiesInOctant.push_back(i);
	}

	// Subdivide root octant
	Subdivide(octants[0]);

}

Octree::~Octree()
{
	for (uint i = 0; i < octantCount; i++)
	{
		SafeDelete(octants[i]);
	}
}

void Octree::Display()
{
	// add wirecube to mesh manager for each octant
	for (uint i = 0; i < octantCount; i++)
	{
		meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, octants[i]->getCenter()) * glm::scale(vector3(octants[i]->getHeight())), C_YELLOW, RENDER_WIRE);
	}
}

void Octree::Subdivide(MyOctant* parent)
{
	// Find centers for 8 new octants and create them
	uint currentLevel = parent->getLevel();

	if (currentLevel == this->levels) { return; };

	vector3 parentCenter = parent->getCenter();
	float height = parent->getHeight() / 2;
	float halfHeight = height / 2;

	vector3 center = vector3(parentCenter.x + halfHeight, parentCenter.y + halfHeight, parentCenter.z - halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x - halfHeight, parentCenter.y + halfHeight, parentCenter.z - halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x + halfHeight, parentCenter.y - halfHeight, parentCenter.z - halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x - halfHeight, parentCenter.y - halfHeight, parentCenter.z - halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x + halfHeight, parentCenter.y + halfHeight, parentCenter.z + halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x - halfHeight, parentCenter.y + halfHeight, parentCenter.z + halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x + halfHeight, parentCenter.y - halfHeight, parentCenter.z + halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

	center = vector3(parentCenter.x - halfHeight, parentCenter.y - halfHeight, parentCenter.z + halfHeight);
	SetupNewOctant(parent, height, center, halfHeight);

}

void Octree::SetupNewOctant(MyOctant* parent, float height, vector3 center, float halfHeight)
{
	// Create new octant and set values
	octants[octantCount] = new MyOctant(parent->getLevel() + 1, height, center, false, parent->getMaxLevel());
	octants[octantCount]->onctantID = octantCount;
	octants[octantCount]->setMaxVector(center + vector3(halfHeight));
	octants[octantCount]->setMinVector(center - vector3(halfHeight));

	if (parent->getLevel() + 1 == levels)
	{
		octants[octantCount]->setLeafTrue();
	}

	// Check each entity in parent octant. If its colliding with new octant, 
	// add it to the octants list of entities
	for (uint i = 0; i < parent->entitiesInOctant.size(); i++)
	{
		MyEntity* entity = entityManager->GetEntity(parent->entitiesInOctant[i]);
		MyRigidBody* rb = entity->GetRigidBody();

		bool colliding = true;

		if (octants[octantCount]->getMaxVector().x < rb->GetMinGlobal().x)
			colliding = false;
		if (octants[octantCount]->getMinVector().x > rb->GetMaxGlobal().x)
			colliding = false;

		if (octants[octantCount]->getMaxVector().y < rb->GetMinGlobal().y)
			colliding = false;
		if (octants[octantCount]->getMinVector().y > rb->GetMaxGlobal().y)
			colliding = false;

		if (octants[octantCount]->getMaxVector().z < rb->GetMinGlobal().z)
			colliding = false;
		if (octants[octantCount]->getMinVector().z > rb->GetMaxGlobal().z)
			colliding = false;

		if (colliding)
		{
			octants[octantCount]->entitiesInOctant.push_back(parent->entitiesInOctant[i]);
		}
	}
	octantCount++;

	// If the new octant is not a leaf, subdivide it
	if (!octants[octantCount - 1]->getLeaf())
	{
		Subdivide(octants[octantCount-1]);
	}
}

void Octree::checkCollisions()
{
	// Check if octant is a leaf and if it has at least two entities, then check collisions
	for (uint i = 0; i < octantCount; i++)
	{
		if (octants[i]->getLeaf())
		{
			if (octants[i]->entitiesInOctant.size() > 2)
			{
				for (uint j = 0; j < octants[i]->entitiesInOctant.size(); j++)
				{
					for (uint k = j + 1; k < octants[i]->entitiesInOctant.size(); k++)
					{
						MyEntity* entity1 = entityManager->GetEntity(octants[i]->entitiesInOctant[j]);
						MyEntity* entity2 = entityManager->GetEntity(octants[i]->entitiesInOctant[k]);
						entity1->IsColliding(entity2);
					}
				}
			}
		}
	}
}



