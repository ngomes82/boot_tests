// Transform2D.cpp : Defines the entry point for the console application.
//
#include <chrono>
#include <iostream>
#include <math.h>

const int PERFORMANCE_TEST_COUNT = 2500000;

//TODO: Actually get the right PI constant!
const float  PI_F				= 3.14159265358979f;
const float DEGREE_TO_RADIANS   = PI_F / 180.0f;
//END TODO

const float RIGHT_ANGLE_RADIANS = 90.0f * DEGREE_TO_RADIANS;

struct Matrix3X3
{
	__declspec(align(16)) float m_MatrixRow0[3];
	__declspec(align(16)) float m_MatrixRow1[3];
	__declspec(align(16)) float m_MatrixRow2[3];

	Matrix3X3() :m_MatrixRow0{ 0.0f, 0.0f, 0.0f }, m_MatrixRow1{ 0.0f, 0.0f, 0.0 }, m_MatrixRow2{ 0.0f, 0.0f, 0.0f } {};
	Matrix3X3(float a, float b, float c, float d, float e, float f, float g, float h, float i) :m_MatrixRow0{ a,b,c }, m_MatrixRow1{ d,e,f }, m_MatrixRow2{ g,h,i} {};

	Matrix3X3(const Matrix3X3& Other) : m_MatrixRow0{ Other.m_MatrixRow0[0], Other.m_MatrixRow0[1], Other.m_MatrixRow0[2] },
										m_MatrixRow1{ Other.m_MatrixRow1[0], Other.m_MatrixRow1[1], Other.m_MatrixRow1[2] }, 
										m_MatrixRow2{ Other.m_MatrixRow2[0], Other.m_MatrixRow2[1], Other.m_MatrixRow2[2] }{};

	/*									m_MatrixRow0{ Other.m_MatrixRow0[0], Other.m_MatrixRow0[1], Other.m_MatrixRow0[2] },
										m_MatrixRow1{ Other.m_MatrixRow1[3], Other.m_MatrixRow1[4], Other.m_MatrixRow1[5] }, /
										m_MatrixRow2{ Other.m_MatrixRow2[6], Other.m_MatrixRow2[7], Other.m_MatrixRow2[8] }{};*/
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
	Result.m_MatrixRow0[2] = TranslationVector.X();
	Result.m_MatrixRow1[2] = TranslationVector.Y();

	Result.m_MatrixRow0[0] = 1.0f;
	Result.m_MatrixRow1[1] = 1.0f;
	Result.m_MatrixRow2[2] = 1.0f;
}

void CreateRotationMatrix(const float& rotation, Matrix3X3& Result)
{
	float cosRot = cos(rotation);
	float sinRot = sin(rotation);

	Result.m_MatrixRow0[0] =  cosRot;
	Result.m_MatrixRow0[1] = -sinRot;
	Result.m_MatrixRow1[0] =  sinRot;
	Result.m_MatrixRow1[1] =  cosRot;

	Result.m_MatrixRow2[2] =  1.0f;
}

void CreateScaleMatrix(const Vector2D& ScalingVector, Matrix3X3& Result)
{
	Result.m_MatrixRow0[0] = ScalingVector.X();
	Result.m_MatrixRow1[1] = ScalingVector.Y();

	Result.m_MatrixRow2[2] = 1.0f;
}

void CreateShearMatrix(const Vector2D& ShearingVector, Matrix3X3& Result)
{
	Result.m_MatrixRow0[1] = ShearingVector.Y();
	Result.m_MatrixRow1[0] = ShearingVector.X();

	Result.m_MatrixRow0[0] = 1.0f;
	Result.m_MatrixRow1[1] = 1.0f;
	Result.m_MatrixRow2[2] = 1.0f;
}

inline void MultiplyMatrices3X3(const Matrix3X3& Left, const Matrix3X3& Right, Matrix3X3& Result)
{
	Result.m_MatrixRow0[0] = Left.m_MatrixRow0[0] * Right.m_MatrixRow0[0] + Left.m_MatrixRow0[1] * Right.m_MatrixRow1[0] + Left.m_MatrixRow0[2] * Right.m_MatrixRow2[0];
	Result.m_MatrixRow0[1] = Left.m_MatrixRow0[0] * Right.m_MatrixRow0[1] + Left.m_MatrixRow0[1] * Right.m_MatrixRow1[1] + Left.m_MatrixRow0[2] * Right.m_MatrixRow2[1];
	Result.m_MatrixRow0[2] = Left.m_MatrixRow0[0] * Right.m_MatrixRow0[2] + Left.m_MatrixRow0[1] * Right.m_MatrixRow1[2] + Left.m_MatrixRow0[2] * Right.m_MatrixRow2[2];

	Result.m_MatrixRow1[0] = Left.m_MatrixRow1[0] * Right.m_MatrixRow0[0] + Left.m_MatrixRow1[1] * Right.m_MatrixRow1[0] + Left.m_MatrixRow1[2] * Right.m_MatrixRow2[0];
	Result.m_MatrixRow1[1] = Left.m_MatrixRow1[0] * Right.m_MatrixRow0[1] + Left.m_MatrixRow1[1] * Right.m_MatrixRow1[1] + Left.m_MatrixRow1[2] * Right.m_MatrixRow2[1];
	Result.m_MatrixRow1[2] = Left.m_MatrixRow1[0] * Right.m_MatrixRow0[2] + Left.m_MatrixRow1[1] * Right.m_MatrixRow1[2] + Left.m_MatrixRow1[2] * Right.m_MatrixRow2[2];

	Result.m_MatrixRow2[0] = Left.m_MatrixRow2[0] * Right.m_MatrixRow0[0] + Left.m_MatrixRow2[1] * Right.m_MatrixRow1[0] + Left.m_MatrixRow2[2] * Right.m_MatrixRow2[0];
	Result.m_MatrixRow2[1] = Left.m_MatrixRow2[0] * Right.m_MatrixRow0[1] + Left.m_MatrixRow2[1] * Right.m_MatrixRow1[1] + Left.m_MatrixRow2[2] * Right.m_MatrixRow2[1];
	Result.m_MatrixRow2[2] = Left.m_MatrixRow2[0] * Right.m_MatrixRow0[2] + Left.m_MatrixRow2[1] * Right.m_MatrixRow1[2] + Left.m_MatrixRow2[2] * Right.m_MatrixRow2[2];
}


inline void MultiplyMatrices3X3SIMD(const Matrix3X3& Left, const Matrix3X3& Right, Matrix3X3& Result)
{
	__m128 LeftMatrix0 = _mm_load1_ps(&Left.m_MatrixRow0[0]);
	__m128 LeftMatrix1 = _mm_load1_ps(&Left.m_MatrixRow0[1]);
	__m128 LeftMatrix2 = _mm_load1_ps(&Left.m_MatrixRow0[2]);
	__m128 LeftMatrix3 = _mm_load1_ps(&Left.m_MatrixRow1[0]);
	__m128 LeftMatrix4 = _mm_load1_ps(&Left.m_MatrixRow1[1]);
	__m128 LeftMatrix5 = _mm_load1_ps(&Left.m_MatrixRow1[2]);
	__m128 LeftMatrix6 = _mm_load1_ps(&Left.m_MatrixRow2[0]);
	__m128 Leftmatrix7 = _mm_load1_ps(&Left.m_MatrixRow2[1]);
	__m128 LeftMatrix8 = _mm_load1_ps(&Left.m_MatrixRow2[2]);

	__m128 RightMatrix0 = _mm_load_ps(&Right.m_MatrixRow0[0]);
	__m128 RightMatrix1 = _mm_load_ps(&Right.m_MatrixRow1[0]); 
	__m128 RightMatrix2 = _mm_load_ps(&Right.m_MatrixRow2[0]); 




	__m128 LeftRightMul1 = _mm_mul_ps(LeftMatrix0, RightMatrix0);  //LM[0] * RM[0,1,2]
	__m128 LeftRightMul2 = _mm_mul_ps(LeftMatrix1, RightMatrix1);  //LM[1] * RM[3,4,5]
	__m128 LeftRightMul3 = _mm_mul_ps(LeftMatrix2, RightMatrix2);  //LM[2] * RM[6,7,8]

	__m128 LeftRightMul4 = _mm_mul_ps(LeftMatrix3, RightMatrix0); //LM[3] * RM[0,1,2]
	__m128 LeftRightMul5 = _mm_mul_ps(LeftMatrix4, RightMatrix1); //LM[4] * RM[3,4,5]
	__m128 LeftRightMul6 = _mm_mul_ps(LeftMatrix5, RightMatrix2); //LM[5] * RM[6,7,8]

	__m128 LeftRightMul7 = _mm_mul_ps(LeftMatrix6, RightMatrix0); //LM[6] * RM[0,1,2]
	__m128 LeftRightMul8 = _mm_mul_ps(Leftmatrix7, RightMatrix1); //LM[7] * RM[3,4,5]
	__m128 LeftRightMul9 = _mm_mul_ps(LeftMatrix8, RightMatrix2); //LM[8] * RM[6,7,8]


	//Add up the results of the multiplication
	__m128 FirstResultChunk = _mm_add_ps(LeftRightMul1, LeftRightMul2);
	FirstResultChunk		= _mm_add_ps(FirstResultChunk, LeftRightMul3);

	__m128 SecondResultChunk = _mm_add_ps(LeftRightMul4, LeftRightMul5);
	SecondResultChunk		 = _mm_add_ps(SecondResultChunk, LeftRightMul6);

	__m128 ThirdResultChunk = _mm_add_ps(LeftRightMul7, LeftRightMul8);
	ThirdResultChunk		= _mm_add_ps(ThirdResultChunk, LeftRightMul9);


	float* firstResultChunkArr  = (float*)  (&FirstResultChunk);
	float* secondResultChunkArr = (float*)  (&SecondResultChunk);
	float* thirdResultChunkArr  = (float*)  (&ThirdResultChunk);

	Result.m_MatrixRow0[0] = firstResultChunkArr[0];
	Result.m_MatrixRow0[1] = firstResultChunkArr[1];
	Result.m_MatrixRow0[2] = firstResultChunkArr[2];

	Result.m_MatrixRow1[0] = secondResultChunkArr[0];
	Result.m_MatrixRow1[1] = secondResultChunkArr[1];
	Result.m_MatrixRow1[2] = secondResultChunkArr[2];

	Result.m_MatrixRow2[0] = thirdResultChunkArr[0];
	Result.m_MatrixRow2[1] = thirdResultChunkArr[1];
	Result.m_MatrixRow2[2] = thirdResultChunkArr[2];
}


//TODO: Double check that this is giving the correct results.
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
	std::cout << ToPrint.m_MatrixRow0[0] << "," << ToPrint.m_MatrixRow0[1] << "," << ToPrint.m_MatrixRow0[2] << "\n"
			 << ToPrint.m_MatrixRow1[0] << "," << ToPrint.m_MatrixRow1[1] << "," << ToPrint.m_MatrixRow1[2] << "\n"
			 << ToPrint.m_MatrixRow2[0] << "," << ToPrint.m_MatrixRow2[1] << "," << ToPrint.m_MatrixRow2[2] << "\n\n";
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
	float rotation = 45.0f * DEGREE_TO_RADIANS;


	Matrix3X3 finalTransformationMatrix;
	Matrix3X3 rotationMatrix;
	Matrix3X3 translationMatrix;
	Matrix3X3 childWorldMatrix = Identity;


	CreateTranslationMatrix(Translation, translationMatrix);
	CreateRotationMatrix(rotation, rotationMatrix);
	MultiplyMatrices3X3(translationMatrix, rotationMatrix, finalTransformationMatrix);

	std::cout << "Multiply matrices" << std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < PERFORMANCE_TEST_COUNT; i++)
	{
		MultiplyMatrices3X3(Parent.m_LocalMatrix, finalTransformationMatrix, Parent.m_LocalMatrix);
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;


	std::cout << "Multiply matrices SIMD" << std::endl;
	 start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < PERFORMANCE_TEST_COUNT; i++)
	{
		MultiplyMatrices3X3SIMD(Parent.m_LocalMatrix, finalTransformationMatrix, Parent.m_LocalMatrix);
	}

	 end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;


	MultiplyMatrices3X3(Child.m_LocalMatrix, translationMatrix, Child.m_LocalMatrix);
	

	std::cout << "Get child world matrix" << std::endl;
	start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < PERFORMANCE_TEST_COUNT; i++)
	{
		GetWorldTransformMatrix(Child, childWorldMatrix);
	}

	end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;


	PrintMatrix3X3(Parent.m_LocalMatrix);
	PrintMatrix3X3(Child.m_LocalMatrix);
	PrintMatrix3X3(childWorldMatrix);






	//Test matrix multiply 
	Matrix3X3 LeftTest = Matrix3X3(10.0f, 3.0f, 2.0f,
								   1.0f, 5.0f,  6.0f,
								   12.0f, 10.0f, 8.0f);

	Matrix3X3 RightTest = Matrix3X3(1.0f, 9.0f, 8.0f, 
									14.0f, 12.0f, 5.0f,
									6.0f, 11.0f, 12.0f);

	

	Matrix3X3 Result;
	MultiplyMatrices3X3(LeftTest, RightTest, Result);
	//MultiplyMatrices3X3SIMD(LeftTest, RightTest, Result);
	PrintMatrix3X3(Result);
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

