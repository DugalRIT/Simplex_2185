#include "MyOctant.h"

MyOctant::MyOctant(uint level, float height, vector3 center, bool root, uint maxLevel)
{
	this->level = level;
	this->height = height;
	this->center = center;
	this->root = root;
	this->maxLevel = maxLevel;
	leaf = false;
}

MyOctant::~MyOctant()
{
}

vector3 MyOctant::getCenter()
{
	return center;
}

float MyOctant::getHeight()
{
	return height;
}

uint MyOctant::getLevel()
{
	return level;
}

void MyOctant::setMaxVector(vector3 max)
{
	maxVector = max;
}

void MyOctant::setMinVector(vector3 min)
{
	minVector = min;
}

vector3 MyOctant::getMaxVector()
{
	return maxVector;
}

vector3 MyOctant::getMinVector()
{
	return minVector;
}

uint MyOctant::getMaxLevel()
{
	return maxLevel;
}

void MyOctant::setLeafTrue()
{
	leaf = true;
}

bool MyOctant::getLeaf()
{
	return leaf;
}

