#include "MyOctant.h"

MyOctant::MyOctant(uint level, uint height, vector3 center, bool root)
{
	this->level = level;
	this->height = height;
	this->center = center;
	this.root = root;
}

MyOctant::~MyOctant()
{
}
