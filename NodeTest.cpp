#include "NodeTest.h"



NodeTest::NodeTest()
{
}


NodeTest::~NodeTest()
{
}


void NodeTest::Update()
{
	Position[0] += 50.0f;
	Position[1] += 50.0f;
	Rotation	+= 50.0f;
}
