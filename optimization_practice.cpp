#include <windows.h>
#include <iostream>
#include <chrono>
#include <process.h>
#include <winbase.h>
#include <processthreadsapi.h>
#include "ChildTest.h"
#include "Test.h"
#include "NodeTest.h"


int activeThreadCount;
const int NumTestCases = 26750000;

struct FooUpdateIn {
	float m_Velocity[2];
	float m_Rotation;
};

struct ThreadParams
{
public:

	ThreadParams(int ArgStartIndex, int ArgCount, __m128* ArgAddResultArr, float* ArgAArr, __m128* ArgPtrB )
	{
		StartIndex		= ArgStartIndex;
		Count			= ArgCount;
		addResultArr	= ArgAddResultArr;
		a				= ArgAArr;
		ptrB			= ArgPtrB;
	};

	int StartIndex;
	int Count;
	__m128* addResultArr;
	__declspec(align(16)) float* a;
	__m128* ptrB;

};

void UpdateFoos(FooUpdateIn* in, size_t count, float f)
{
	for (size_t i = 0; i < count; ++i) 
	{
		in[i].m_Velocity[0] += f;
		in[i].m_Velocity[1] += f;
		in[i].m_Rotation    += f;
	}
}

//Naive implementation, L2 Cache misses galore
void Test1()
{
	NodeTest* NodeArr = new NodeTest[NumTestCases];

	std::cout << "Misses L2 Cache Test Results" << std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < NumTestCases; i++)
	{
		NodeArr[i].Update();
	}
	

	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;

	//delete NodeArr;
}

//Don't miss L2 Cache when doing the inner loop
void Test2()
{
	FooUpdateIn* UpdateArrr = new FooUpdateIn[NumTestCases];

	std::cout << "Hits L2 Cache Test Results" << std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();

	UpdateFoos(UpdateArrr, NumTestCases, 50.0f);

	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;

	delete UpdateArrr;
}

//Use SSE to do the additions
void Test3()
{
	int combinedArrSize				= 3 * NumTestCases; // 3 variables (x,y,rot)
	__m128* addResultArr			= new __m128[NumTestCases];
	__declspec(align(16)) float* a  = new float[combinedArrSize];
	__declspec(align(16)) float b[] = { 50.0f, 51.0f, 52.0f, 53.0f }; //add 3 variables with one overlap (dx,dy,drot,dx)
	__m128* ptrB					= (__m128*) b;

	//Initialize combined array
	for (int i = 0; i < combinedArrSize; i++)
	{
		a[i] = 0.0f;
	}


	//Run the test
	std::cout << "SSE Addition Test Results"<< std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < NumTestCases; i+=4)
	{
		__m128* ptrA    = (__m128*) &a[i];
		addResultArr[i/4] = _mm_add_ps(*ptrA, *ptrB);
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;

	//make sure it added correctly.
	/*for (int i = 0; i < 10; i++)
	{
		float* fptr = (float*) &addResultArr[i];
		std::cout << fptr[0] << "," << fptr[1] << "," << fptr[2] << "," << fptr[3] << std::endl;
	}*/

	delete addResultArr;
	delete a;
}

void ThreadExec(void* params)
{
	ThreadParams* ParamStruct  = (ThreadParams*)params;
	int FinalIndex			   = ParamStruct->StartIndex + ParamStruct->Count;
	__m128 ptrB				   = *(ParamStruct->ptrB);

	for (int i = ParamStruct->StartIndex; i < FinalIndex; i+=4)
	{
		__m128 ptrA = *( (__m128*) &ParamStruct->a[i]);
		ParamStruct->addResultArr[i/4] = _mm_add_ps(ptrA, ptrB);
	}
	
	activeThreadCount--;
	_endthread();
}

//Use Multi-Threaded SSE to do the additions
void Test4()
{
	activeThreadCount = 0;
		
	int numThreads					= 8;
	int combinedArrSize			    = 3 * NumTestCases;
	int countPerThread				=  NumTestCases / numThreads;
	__m128* addResultArr		    = new __m128[NumTestCases];
	__declspec(align(16)) float* a  = new float[combinedArrSize];
	__declspec(align(16)) float* b  = new float[4];//{ 50.0f, 50.0f, 50.0f, 50.0f };
	__m128* ptrB = (__m128*) b;

	//Initialize combined array
	for (int i = 0; i < combinedArrSize; i++)
	{
		a[i] = 0.0f;
	}

	b[0] = 50.0f;
	b[1] = 51.0f;
	b[2] = 52.0f;
	b[3] = 53.0f;

	std::cout << "Multi-Threaded SSE Addition Test Results" << std::endl;
	auto start_time = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numThreads; i++)
	{
		int startIndex		 = i * countPerThread;
		ThreadParams* Params = new ThreadParams(startIndex, countPerThread, addResultArr, a, ptrB);

		activeThreadCount++;
		HANDLE threadHandle = (HANDLE) _beginthread(ThreadExec, 0, (void*)Params);
		SetThreadPriority(threadHandle, THREAD_PRIORITY_TIME_CRITICAL);
	}

	while (activeThreadCount > 0){}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count() << "s" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count() << "us" << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms" << std::endl << std::endl;


	//make sure it added correctly.
	/*for (int i = 0; i < 10; i++)
	{
		float* fptr = (float*) &addResultArr[i];
 		std::cout << fptr[0] << "," << fptr[1] << "," << fptr[2] << "," << fptr[3] << std::endl;
	}*/

	delete addResultArr;
	delete a;
}

int main()
{

	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	int testInt  = 20;
	int* testInt2;

	testInt2 = &testInt;

	ChildTest *myChild			  = new ChildTest(5);
	Parent    *myParent			  = myChild;
	ITestInterface *testInterface = myChild;



	testInterface->method1();
	myChild->method2();



	Test *t = new Test();
	Test *d = new Test();
	Test *x = new Test();
	

	std::cout << "t pointer addr: " << myChild->myParentVar << "\nd pointer addr: " << myParent->myParentVar << "\nx bytes in hex: " << x << "\n size of test: " << sizeof(Test) << std::endl;
	std::cout << "size of node: " << sizeof(NodeTest) << std::endl;
	
	

	std::cout << "precision: " << std::chrono::high_resolution_clock::period::den << " (ticks per second)" << std::endl << std::endl << "Number of Tests: " << NumTestCases << std::endl << std::endl;
	
	Test1();
	Test2();
	Test3();
	Test4();

	delete t;


	int testInput;
	std::cin >> testInput;

	return *testInt2;
}
