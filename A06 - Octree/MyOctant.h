#pragma once
#include "Definitions.h"
#include "MyEntityManager.h"
using namespace Simplex;

class MyOctant
{
public:
	MyOctant(uint level, float height, vector3 center, bool root, uint maxLevel);
	~MyOctant();
	uint onctantID;
	std::vector<uint> entitiesInOctant;
	vector3 getCenter();
	float getHeight();
	uint getLevel();
	void setMaxVector(vector3 max);
	void setMinVector(vector3 min);
	vector3 getMaxVector();
	vector3 getMinVector();
	uint getMaxLevel();
	void setLeafTrue();
	bool getLeaf();

private:
	uint level;
	float height;
	vector3 center;
	vector3 maxVector;
	vector3 minVector;
	bool root;
	uint maxLevel;
	bool leaf;
	
};