// Transform2D.cpp : Defines the entry point for the console application.
//
#include <chrono>
#include <iostream>
#include <math.h>

//TODO: Actually get the right PI constant!
const float DEGREE_TO_RADIANS   = 3.14f / 180.0f;
//END TODO

const float RIGHT_ANGLE_RADIANS = 90.0f * DEGREE_TO_RADIANS;

struct Matrix3X3
{
	float m_Matrix[9];

	Matrix3X3() :m_Matrix{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f } {};
	Matrix3X3(float a, float b, float c, float d, float e, float f, float g, float h, float i):m_Matrix{a,b,c,d,e,f,g,h,i} {};

	Matrix3X3(const Matrix3X3& Other):m_Matrix{ Other.m_Matrix[0], Other.m_Matrix[1], Other.m_Matrix[2], 
												Other.m_Matrix[3], Other.m_Matrix[4], Other.m_Matrix[5], 
												Other.m_Matrix[6], Other.m_Matrix[7], Other.m_Matrix[8] }{};
};

const Matrix3X3 Identity(1.0f, 0.0f, 0.0f,
						 0.0f, 1.0f, 0.0f,
						 0.0f, 0.0f, 1.0f);

struct Vector2D
{
	float coords[2]; //x,y

	Vector2D():coords{ 0.0f, 0.0f } {};
	Vector2D(float x, float y):coords{ x, y } {};

	inline float X() const { return coords[0]; }
	inline float Y() const { return coords[1]; }
};


struct Transform2D
{
	Transform2D* m_Parent;
	Transform2D* m_Child;

	Matrix3X3 m_LocalMatrix;

	Transform2D() :m_Parent(nullptr), m_Child(nullptr), m_LocalMatrix(Identity){};

	//YDown Support?
	//Flip (x&y) Support?
};

void CreateTranslationMatrix(const Vector2D& TranslationVector, Matrix3X3& Result)
{
	Result.m_Matrix[2] = TranslationVector.X();
	Result.m_Matrix[5] = TranslationVector.Y();

	Result.m_Matrix[0] = 1.0f;
	Result.m_Matrix[4] = 1.0f;
	Result.m_Matrix[8] = 1.0f;
}

void CreateRotationMatrix(const float& rotation, Matrix3X3& Result)
{
	float cosRot = cos(rotation);
	float sinRot = sin(rotation);

	Result.m_Matrix[0] =  cosRot;
	Result.m_Matrix[1] = -sinRot;
	Result.m_Matrix[3] =  sinRot;
	Result.m_Matrix[4] =  cosRot;

	Result.m_Matrix[8] =  1.0f;
}

void CreateScaleMatrix(const Vector2D& ScalingVector, Matrix3X3& Result)
{
	Result.m_Matrix[0] = ScalingVector.X();
	Result.m_Matrix[4] = ScalingVector.Y();

	Result.m_Matrix[8] = 1.0f;
}

void CreateShearMatrix(const Vector2D& ShearingVector, Matrix3X3& Result)
{
	Result.m_Matrix[1] = ShearingVector.Y();
	Result.m_Matrix[3] = ShearingVector.X();

	Result.m_Matrix[0] = 1.0f;
	Result.m_Matrix[4] = 1.0f;
	Result.m_Matrix[8] = 1.0f;
}

void MultiplyMatrices3X3(const Matrix3X3& Left, const Matrix3X3& Right, Matrix3X3& Result)
{
	Result.m_Matrix[0] = Left.m_Matrix[0] * Right.m_Matrix[0] + Left.m_Matrix[1] * Right.m_Matrix[3] + Left.m_Matrix[2] * Right.m_Matrix[6];
	Result.m_Matrix[1] = Left.m_Matrix[0] * Right.m_Matrix[1] + Left.m_Matrix[1] * Right.m_Matrix[4] + Left.m_Matrix[2] * Right.m_Matrix[7];
	Result.m_Matrix[2] = Left.m_Matrix[0] * Right.m_Matrix[2] + Left.m_Matrix[1] * Right.m_Matrix[5] + Left.m_Matrix[2] * Right.m_Matrix[8];

	Result.m_Matrix[3] = Left.m_Matrix[3] * Right.m_Matrix[0] + Left.m_Matrix[4] * Right.m_Matrix[3] + Left.m_Matrix[5] * Right.m_Matrix[6];
	Result.m_Matrix[4] = Left.m_Matrix[3] * Right.m_Matrix[1] + Left.m_Matrix[4] * Right.m_Matrix[4] + Left.m_Matrix[5] * Right.m_Matrix[7];
	Result.m_Matrix[5] = Left.m_Matrix[3] * Right.m_Matrix[2] + Left.m_Matrix[4] * Right.m_Matrix[5] + Left.m_Matrix[5] * Right.m_Matrix[8];

	Result.m_Matrix[6] = Left.m_Matrix[6] * Right.m_Matrix[0] + Left.m_Matrix[7] * Right.m_Matrix[3] + Left.m_Matrix[8] * Right.m_Matrix[6];
	Result.m_Matrix[7] = Left.m_Matrix[6] * Right.m_Matrix[1] + Left.m_Matrix[7] * Right.m_Matrix[4] + Left.m_Matrix[8] * Right.m_Matrix[7];
	Result.m_Matrix[8] = Left.m_Matrix[6] * Right.m_Matrix[2] + Left.m_Matrix[7] * Right.m_Matrix[5] + Left.m_Matrix[8] * Right.m_Matrix[8];
}

//TODO: Double check that this is givin the correct results.
void GetWorldTransformMatrix(const Transform2D& Transform, Matrix3X3& Result)
{
	const Transform2D* ParentRoot = &Transform;
	while (ParentRoot->m_Parent != nullptr)
	{
		ParentRoot = ParentRoot->m_Parent;
	}

	while (ParentRoot != nullptr)
	{
		MultiplyMatrices3X3(ParentRoot->m_LocalMatrix, Result, Result);
		ParentRoot = ParentRoot->m_Child;
	}
}
//end todo.

void PrintMatrix3X3(const Matrix3X3& ToPrint)
{
	std::cout << ToPrint.m_Matrix[0] << "," << ToPrint.m_Matrix[1] << "," << ToPrint.m_Matrix[2] << "\n"
			 << ToPrint.m_Matrix[3] << "," << ToPrint.m_Matrix[4] << "," << ToPrint.m_Matrix[5] << "\n"
			 << ToPrint.m_Matrix[6] << "," << ToPrint.m_Matrix[7] << "," << ToPrint.m_Matrix[8] << "\n\n";
}

int main()
{

	//Testing parent -> child transformations
	Transform2D Parent;
	Transform2D Child;

	Child.m_Parent = &Parent;
	Parent.m_Child = &Child;

	Vector2D Translation(50.0f, -1.0f);
	Vector2D Scale(1.0f, 1.0f);
	Vector2D Shear(0.0f, 0.0f);
	float rotation = 180.0f * DEGREE_TO_RADIANS;


	Matrix3X3 finalTransformationMatrix;
	Matrix3X3 rotationMatrix;
	Matrix3X3 translationMatrix;
	CreateTranslationMatrix(Translation, translationMatrix);
	CreateRotationMatrix(rotation, rotationMatrix);
	MultiplyMatrices3X3(translationMatrix, rotationMatrix, finalTransformationMatrix);

	MultiplyMatrices3X3(Parent.m_LocalMatrix, finalTransformationMatrix, Parent.m_LocalMatrix);
	MultiplyMatrices3X3(Child.m_LocalMatrix, translationMatrix, Child.m_LocalMatrix);

	PrintMatrix3X3(Parent.m_LocalMatrix);
	PrintMatrix3X3(Child.m_LocalMatrix);



	Matrix3X3 childWorldMatrix = Identity;
	GetWorldTransformMatrix(Child, childWorldMatrix);
	PrintMatrix3X3(childWorldMatrix);






	//Test matrix multiply 
	/*Matrix3X3 LeftTest = Matrix3X3(10.0f, 3.0f, 2.0f,
								   1.0f, 5.0f,  6.0f,
								   12.0f, 10.0f, 8.0f);

	Matrix3X3 RightTest = Matrix3X3(1.0f, 9.0f, 8.0f, 
									14.0f, 12.0f, 5.0f,
									6.0f, 11.0f, 12.0f);

	

	Matrix3X3 Result;
	MultiplyMatrices3X3(LeftTest, RightTest, Result);
	PrintMatrix3X3(Result);*/
	//Expected Result
	/*
	/* 	C1	C2	C3
	1	64	148	119
	2	107	135	105
	3	200	316	242
	*/

	while (1) {}

    return 0;
}

