#pragma once
class NodeTest
{
public:

	float Position[2]; //8 bytes (8)
	float Rotation;    //4 Bytes (12, compiler pads to 16)

	NodeTest();
	~NodeTest();

	void Update();
};

