// TestPractice.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <iostream>
#include <string>
#include <random>
//Forward decleration example

//#include "SomeHeader.h"
class SomeHeader; //Use this if all you have are ptr members!!

				  //End Forward decleration example

//Binary Heap Example
// Prototype of a utility function to swap two integers
void heapswap(int *x, int *y);
 
// A class for Min Heap
class MinHeap
{
    int *harr; // pointer to array of elements in heap
    int capacity; // maximum possible size of min heap
    int heap_size; // Current number of elements in min heap
public:
    // Constructor
    MinHeap(int capacity);
 
    // to heapify a subtree with root at given index
    void MinHeapify(int );
 
    int parent(int i) { return (i-1)/2; }
 
    // to get index of left child of node at index i
    int left(int i) { return (2*i + 1); }
 
    // to get index of right child of node at index i
    int right(int i) { return (2*i + 2); }
 
    // to extract the root which is the minimum element
    int extractMin();
 
    // Decreases key value of key at index i to new_val
    void decreaseKey(int i, int new_val);
 
    // Returns the minimum key (key at root) from min heap
    int getMin() { return harr[0]; }
 
    // Deletes a key stored at index i
    void deleteKey(int i);
 
    // Inserts a new key 'k'
    void insertKey(int k);
};
 
// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap(int cap)
{
    heap_size = 0;
    capacity = cap;
    harr = new int[cap];
}
 
// Inserts a new key 'k'
void MinHeap::insertKey(int k)
{
    if (heap_size == capacity)
    {
        std::cout << "\nOverflow: Could not insertKey\n";
        return;
    }
 
    // First insert the new key at the end
    heap_size++;
    int i = heap_size - 1;
    harr[i] = k;
 
    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)] > harr[i])
    {
       heapswap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
}
 
// Decreases value of key at index 'i' to new_val.  It is assumed that
// new_val is smaller than harr[i].
void MinHeap::decreaseKey(int i, int new_val)
{
    harr[i] = new_val;
    while (i != 0 && harr[parent(i)] > harr[i])
    {
       heapswap(&harr[i], &harr[parent(i)]);
       i = parent(i);
    }
}
 
// Method to remove minimum element (or root) from min heap
int MinHeap::extractMin()
{
    if (heap_size <= 0)
        return INT_MAX;
    if (heap_size == 1)
    {
        heap_size--;
        return harr[0];
    }
 
    // Store the minimum value, and remove it from heap
    int root = harr[0];
    harr[0] = harr[heap_size-1];
    heap_size--;
    MinHeapify(0);
 
    return root;
}
 
 
// This function deletes key at index i. It first reduced value to minus
// infinite, then calls extractMin()
void MinHeap::deleteKey(int i)
{
    decreaseKey(i, INT_MIN);
    extractMin();
}
 
// A recursive method to heapify a subtree with root at given index
// This method assumes that the subtrees are already heapified
void MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l] < harr[i])
        smallest = l;
    if (r < heap_size && harr[r] < harr[smallest])
        smallest = r;
    if (smallest != i)
    {
        heapswap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}
 
// A utility function to swap two elements
void heapswap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}


//Use XOR swap method that takes advantage of Commutative property.
void XORSwap(char& x, char& y)
{
	x ^= y;
	y ^= x;
	x ^= y;
}

//Palindrome examble

bool IsPalindrome(std::string toCheck)
{
	int i = 0;
	int j = toCheck.length() - 1;

	while (i<j)
	{
		if (toCheck[i] != toCheck[j])
		{
			return false;
		}

		i++;
		j--;
	}

	return true;
}

//end




//fibonacci number
int fib(int n)
{

	if (n <= 1)
	{
		return n;
	}

	return fib(n - 1) + fib(n - 2);
}

//Binary Search example


// A recursive binary search function. It returns location of x in
// given array arr[l..r] is present, otherwise -1
int binarySearch(int arr[], int l, int r, int x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		// If the element is present at the middle itself
		if (arr[mid] == x)
		{
			return mid;
		}

		// If element is smaller than mid, then it can only be present
		// in left subarray
		if (arr[mid] > x)
		{
			return binarySearch(arr, l, mid - 1, x);
		}

		// Else the element can only be present in right subarray
		return binarySearch(arr, mid + 1, r, x);
	}

	// We reach here when element is not present in array
	return -1;
}

//END Binary search


//Sort Example
void intswap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void bubbleSort(int arr[], int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++)
	{
		// Last i elements are already in place   
		for (j = 0; j < n - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				intswap(&arr[j], &arr[j + 1]);
			}
		}
	}
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition(int arr[], int low, int high)
{
	int pivot = arr[high];    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot)
		{
			i++;    // increment index of smaller element
			intswap(&arr[i], &arr[j]);
		}
	}
	intswap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
arr[] --> Array to be sorted,
low  --> Starting index,
high  --> Ending index */
void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

void printArray(int arr[], int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		std::cout << arr[i] << ",";
	}

	std::cout << std::endl;
}


void RunSortTest()
{
	int test1[] = { 50,4,23,2,1,67,0,999 };
	int test2[] = { 50,4,23,2,1,67,0,999 };
	int n = sizeof(test1) / sizeof(test1[0]);

	bubbleSort(test1, n);
	printArray(test1, n);

	quickSort(test2, 0, n - 1);
	printArray(test2, n);

	int index = binarySearch(test2, 0, n - 1, 23);
	std::cout << "found 23 at: " << index << std::endl;

}
//End Sort Example


//Shuffle Example

//use fisher yates shuffle, at each iteration
//get a random index between iterator and end
//then swap the objects at those indices.
void Shuffle(std::string& strToShuffle)
{
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator

	for (int i = 0; i < strToShuffle.length() - 1; i++)
	{
		std::uniform_int_distribution<> distr(i, strToShuffle.length() - 1);
		int j = distr(eng);
		XORSwap(strToShuffle[i], strToShuffle[j]);
	}
}

//End Shuffle Example


//XOR Add

//Add in a similar way that ALU's add.
//use XOR and bitshift when there is a carry.
int XORAdd(int x, int y)
{
	// Iterate till there is no carry  
	while (y != 0)
	{
		// carry now contains common set bits of x and y
		int carry = x & y;

		// Sum of bits of x and y where at least one of the bits is not set
		x = x ^ y;

		// Carry is shifted by one so that adding it to x gives the required sum
		y = carry << 1;
	}
	return x;
}

//END XOR ADD


//Binary Search Tree Example
class TreeNode
{
public:
	explicit TreeNode(int argKey) : key(argKey), left(nullptr), right(nullptr) { }

	int key;
	TreeNode *left;
	TreeNode *right;
};

TreeNode* bst_insert(TreeNode*& root, int key)
{
	if (root == nullptr)
	{
		root = new TreeNode(key);
	}
	else if (key < root->key)
	{
		root->left = bst_insert(root->left, key);
	}
	else
	{
		root->right = bst_insert(root->right, key);
	}

	return root;
}

TreeNode* bst_search_recursively(int key, TreeNode* node)
{
	if (node == nullptr || node->key == key)
	{
		return node;
	}

	if (key < node->key)
	{
		return bst_search_recursively(key, node->left);
	}
	else
	{
		return bst_search_recursively(key, node->right);
	}
}

void bst_traverse_in_order(TreeNode* node)
{
	if (node == nullptr)
	{
		return;
	}

	//Changing order of access changes traverse type

	//std::cout << node->key << ","; //Pre-Order

	bst_traverse_in_order(node->left);

	std::cout << node->key << ",";  //In-Order (sorted)

	bst_traverse_in_order(node->right);

	//std::cout << node->key << ","; //Post-Order
}

void RunBinaryTreeExample()
{
	TreeNode* root = new TreeNode(5);

	bst_insert(root, 2);
	bst_insert(root, 50);
	bst_insert(root, 1);
	bst_insert(root, 7);
	bst_insert(root, 9);

	TreeNode* Found = bst_search_recursively(50, root);

	if (Found != nullptr)
	{
		std::cout << "Bst search found: " << Found->key << std::endl;
	}
	else
	{
		std::cout << "bst search found nada" << std::endl;
	}

	Found = bst_search_recursively(90, root);

	if (Found != nullptr)
	{
		std::cout << "Bst search found: " << Found->key << std::endl;
	}
	else
	{
		std::cout << "bst search found nada" << std::endl;
	}

	bst_traverse_in_order(root);
	std::cout << std::endl;
}

//End BST Example



//Operator Overload Example

class ComplexNum
{

private:
	ComplexNum() {}

public:
	int real;
	int imaginary;

	ComplexNum(int argReal, int argImaginary) : real(argReal), imaginary(argImaginary) {}

	ComplexNum operator + (ComplexNum const &obj)
	{
		ComplexNum res;
		res.real = real + obj.real;
		res.imaginary = imaginary + obj.imaginary;

		return res;
	}
};

void RunOperatorOverloadTest()
{
	ComplexNum c1(10, 5);
	ComplexNum c2(2, 4);

	ComplexNum c3 = c1 + c2;

	std::cout << "overload: " << c3.real << "," << c3.imaginary << std::endl;
}
//END Operator Overload Example


//Template Example
template<class t1>
class TemplatedLinkedListNode
{

public:

	explicit TemplatedLinkedListNode(t1 argData) : data(argData), Next(nullptr) {}

	TemplatedLinkedListNode *Next;
	t1 data;
};

void RunTemplateExample()
{
	TemplatedLinkedListNode<int> n1(10);
	TemplatedLinkedListNode<int> n2(20);
	TemplatedLinkedListNode<int> n3(40);

	n1.Next = &n2;
	n2.Next = &n3;

	TemplatedLinkedListNode<int>* current = &n1;

	while (current != nullptr)
	{
		std::cout << "template node: " << current->data << " ";
		current = current->Next;
	}

	std::cout << std::endl;
}

//End Template Example




//Auto and Iterator Example

void RunItrExample()
{
	std::vector<int> myIntVec;

	myIntVec.push_back(1);
	myIntVec.push_back(5);
	myIntVec.push_back(8);

	for (auto iterator = myIntVec.begin();
		iterator != myIntVec.end();
		iterator++)
	{
		std::cout << (*iterator) << " ";
	}

	std::cout << " iteraror!" << std::endl;
}


//End Auto and Iterator Example



//Function Pointer / Delegate Example

float ExampleDelegateConcreteFunc1(float arg)
{
	return (arg + 9.0f);
}

float ExampleDelegateConcreteFunc2(float arg)
{
	return (arg + 20.0f);
}

typedef float(*ExampleDelegate) (float arg);


void ExampleDelegateRun()
{
	float original = 20.0f;

	ExampleDelegate function = &ExampleDelegateConcreteFunc1;

	std::cout << "delegate1: " << function(original) << std::endl;

	function = &ExampleDelegateConcreteFunc2;

	std::cout << "delegate2: " << function(original) << std::endl;
}
//END Function pointer / delegate example



//Type Conversion Example
class ExampleImplicitConversion
{

public:
	int data;

	//Since explicit is not used the compiler can do implicit conversions!
	ExampleImplicitConversion(int n) : data(n) { }
};

class ExampleExplicitConstructor
{

public:
	int data;

	//Since explicit is used no implicit conversions!
	explicit ExampleExplicitConstructor(int n) : data(n) { }
};

void ConversionExamples()
{
	//Valid, can use constructor implicitly.
	//But will cause '6' to be converted to ASCII int val 54.
	ExampleImplicitConversion ImplicitConvert = '6';

	std::cout << "Implicit Convert: " << ImplicitConvert.data << std::endl;

	//INVALID, constructor is explicit!!!
	//ExampleExplicitConstructor ExplicitConvert = '6'; 

	//Need to use constructor explicitly!!!!
	ExampleExplicitConstructor ExplicitConstruct(6);

	std::cout << "Explicit Constructor: " << ExplicitConstruct.data << std::endl;


	//TYPE CASTING

	//Notes on casting types!!!
	//-----------------------------------

	//static_cast, used for most intuitive literal type conversions. 
	//When used for inheritance does NOT return nullptr on invalid casts!!

	//dynamic_cast, used for most polymorphic or inherited casts
	//returns null ptr when fails.

	//const_cast, used to remove const.

	//C style cast (int) - Tries static_cast, then reintp_cast, also applies const_cast if needed.



	int testInt = 10;
	float testFloat = 0.0f;

	int* testIntPtr = &testInt;
	float* testFloatPrt = &testFloat;

	testFloat = static_cast<float>(testInt); //Valid, you can go from float to int

	std::cout << "Static cast: " << testFloat << std::endl;

	//INVALID, this is conversion doesn't make intuitive sense. 
	//Unless mangled a float* can't point to an int*.

	//testFloatPrt = static_cast<float*>(testIntPtr); 

	//reinterp cast lets you do whatever crazy shit you want!!!
	//this example treats the bits represented by (int)10 directly as a float!!!(1.4104 E-44)
	testFloatPrt = reinterpret_cast<float*>(testIntPtr);

	std::cout << "Reintrp cast: " << (*testFloatPrt) << std::endl;
}

//END Type Conversion Example


//Const correctness example
//Const is used to prevent modificaiton of a value or pointer.
//Use in function and memeber decleration to ensure the 
//value remains un-modified throughout it's use.
void ConstCorrectness()
{
	int i1 = 0;
	int i2 = 1;
	int i3 = 2;
	int i4 = 3;
	int i5 = 4;
	int i6 = 5;

	const int* p1 = &i1;		  //Pointer to constant int, Value pointed to by p1 CAN'T change.
	int* const p2 = &i2;		  //Constant Pointer to an int, location pointed by p2 CAN'T change.
	const int* const p3 = &i3;		  //Constant Pointer to a constant int, Value and Location CAN'T Change.
	int const* p4 = &i4;		  // Same as const int*  , but looks and reads weird...
	const int const* p5 = &i5;		  //This works, but is redundant. Duplicate const decleration!
	const int const* const p6 = &i6;  //This works, but is redundant. Duplicate const decleration!


									  //	(*p1) = 20;  //INVALID
	p1 = &i2; //VALID

	(*p2) = 20;  //VALID
				 //	p2	  = &i1; //INVALID

				 //	(*p3) = 900; //INVALID
				 //	p3 	  = &i4; //INVALID
}
//End const correctness example



//c++ string to int convert using lib
void StringIntConvertUsingSTD()
{

	std::string str = std::to_string(20);

	std::cout << str << std::endl;

	int i = std::stoi("900");

	std::cout << i << std::endl;
}
//


//Pass by Val, Ref, Ptr

//Pass by val will copy the argument,
//Then modify the LOCAL Copy only. The
//original, outside of the context will be unchanged.
void PassByValue(int val)
{
	val += 10;
}

//Pass by reference passes a reference to the original
//into the function which allows you to modify the original
// value. Keeps the syntax of a value type so no pointer 
// operations are needed. Useful when you are just modifying the underlying.
void PassByReference(int& val)
{
	val += 10;
}

//Passes in a pointer to the original.
//Requires pointer operations to dereference
//and modify the value. Useful mostly for when you want
//to transfer ownership, you should prefer pass by reference
//if all that's needed is modification of the value.
void PassByPointer(int* val)
{
	(*val) += 20;
}
//END pass by val, reference and ptr


//Interface Example
class IExampleInterface
{

public:
	//Pure Virtual Function, any class that derives
	//from this interface is required to override this.
	virtual void ExamplePureVirtualFunc() = 0;

	//Prevents this class from ever being intialized, makes this an abstract class.
	//Abstract classes can never be constructed by themselves, always require derivation.
	virtual ~IExampleInterface() = 0;
};

//Need to Provide implementation of pure virtual destructor
IExampleInterface::~IExampleInterface() {}

class DeriveInterfaceExample : IExampleInterface
{
	//This is required to be implemented!
	void ExamplePureVirtualFunc()
	{
		std::cout << "Example Interface Implementation" << std::endl;
	}
};

//END Interface Example


//Constructor Example in C++
class ConstructorExample
{
public:
	int Count;
	int* ExampleArr;

	//Example of an initialization list, Order of member decleration is order of initialization.
	ConstructorExample() : Count(5), ExampleArr(new int[Count])
	{
		std::cout << "Default Constructor Called" << std::endl;
	}

	//Example of an assignment initialization. Initialization occurs just before body of constructor
	//then constructor body executes and assignments occur.
	ConstructorExample(int ArgCount)
	{
		std::cout << "Second Constructor Called" << std::endl;

		Count = ArgCount;
		ExampleArr = new int[Count];
	}


	//Copy Constructor, used when assigning or creating 
	//an instance of ConstructionExample using ANOTHER instance of ConstructionExample.
	ConstructorExample(const ConstructorExample& Other) : Count(Other.Count), ExampleArr(new int[Count])
	{
		std::cout << "Copy Constructor Called" << std::endl;

		for (int i = 0; i<Count; i++)
		{
			ExampleArr[i] = 444;
		}
	}
};
//End Constructor Example


//Example of inheritance in c++ 
//Remember in final code the deceleration / definitions of functions will be in different files!
class BaseExample
{

public:

	BaseExample(int ArgData)
	{
		data = ArgData;
	}

	int data;

	//Declaring a virtual function.
	virtual int ExampleOverride() { return 10; }
};

class ChildExample : BaseExample
{
public:
	//Calling the base class constructor. Specific name needs to be used since c++ supports multiple inheritance. No Super.
	ChildExample(int ArgChildData) : BaseExample(ArgChildData) {}

	//Overriding a parent function, also calling the parent. Specific name needs to be used due to multiple inheritance. No Super.
	int ExampleOverride() override { return BaseExample::ExampleOverride() + 10; }
};


//Final class, this class can't be derived from. 
class FinalChildExample final : ChildExample
{

public:
	FinalChildExample() : ChildExample(5) {}

	//final function, the function can not be overridden by children (which can't exist b/c class is final, but this is just an example).
	int ExampleOverride() final override { return ChildExample::ExampleOverride() + 10; }
};


//Multiple Inheritance is bad because of diamond problem. Suppose you make a base class A.
// Then you make two second tier classes (B,C) that inherit from A. 
// Finally you make a class D that inherits from both (B,C) using multiple inheritance.
// Now you make an overridable function on A. Then B and C override that function BUT the final class D DOES NOT.
// If you call D->ExampleOverride()..... what gets called? does it call B::ExampleOverride() or C::ExampleOverride()??? Undefined and ambiguous behaviour.


//END Inheritance examples


//Simple implementation of the data structure needed to represent a linked list.
class LinkedListNode
{

public:

	LinkedListNode() : data(-1), Next(nullptr) {}

	LinkedListNode* Next;
	//LinkedListNode* Previous; Doubly linked list.
	int data;
};


//Discover if a linked list has a cycle by using the
//Tortise and hare method. In a cyclic list the faster
//Moving pointer has to eventually catch up to the slower moving pointer.
bool HasLoop(LinkedListNode* First)
{

	if (First == nullptr)
	{
		return false;
	}

	LinkedListNode* slow;
	LinkedListNode* fast;

	slow = fast = First;

	while (true)
	{
		slow = slow->Next;

		if (fast->Next != nullptr)
		{
			fast = fast->Next->Next;
		}
		else
		{
			return false; //Found an end point
		}

		if (slow == nullptr || fast == nullptr)
		{
			return false; //Found an end point
		}

		if (slow == fast)
		{
			return true; //the faster pointer has caught up with the slower, found a cycle.
		}
	}
}

//Reverse a string using any method.
//Just allocate the exact amount of space as the other string,
//Then iterate through it in reverse and place last element first.
std::string ReverseString(std::string StrToReverse)
{
	std::string ToReturn = StrToReverse;

	for (int i = StrToReverse.length() - 1; i >= 0; i--)
	{
		int SwapIndex = (StrToReverse.length() - 1) - i;
		ToReturn[SwapIndex] = StrToReverse[i];
	}

	return ToReturn;
}

//Reverse a string with no secondary array or storage. 
//Use XOR swap method that takes advantage of Commutative property.
//Since we're swapping, meet in the middle to avoid overswapping....
//Which would have happened if you used the above code.
void ReverseStringInPlace(std::string& StrToReverse)
{
	int right = StrToReverse.length() - 1;
	int left = 0;
	while (left < right)
	{
		StrToReverse[right] ^= StrToReverse[left];
		StrToReverse[left] ^= StrToReverse[right];
		StrToReverse[right] ^= StrToReverse[left];

		++left;
		--right;
	}
}

void charswap(char& x, char& y)
{
	char temp;
	temp = x;
	x = y;
	y = temp;
}

//permutation, order matters so AB and BA are unique
void permute(std::string a, int l, int r)
{
	int i;
	if (l == r)
	{
		std::cout << a << std::endl; //you've reached the original length of the string, found a permutation!
	}
	else
	{
		for (i = l; i <= r; i++) //Gotta go through ever single child node,
		{
			charswap(a[l], a[i]);		//swap a child node with its neighbor
			permute(a, l + 1, r);   //permute the resultant string with substring to the right.
			charswap(a[l], a[i]);	    //back track
		}
	}
}


//Combination (order doesn't matter ex AB == BA   so don't print dupes)
//


//Power Set print {"A", "B", "AB", "BA"}


//TEST CASES 
//--------------------
void TestPalindrom()
{
	std::string palin = "racecar";
	std::string notPalin = "jsutastring";

	if (IsPalindrome(palin))
	{
		std::cout << "first string is palin ";
	}

	if (!IsPalindrome(notPalin))
	{
		std::cout << "second string not palin!" << std::endl;
	}
}

void TestShuffle()
{
	std::string toShuffle = "abcdefghijklmnopqrstuvwxyz";
	Shuffle(toShuffle);

	std::cout << "shuffled: " << toShuffle << std::endl;
}


void TestXORAdd()
{
	int a = 50634;
	int b = 7789345;

	int result = XORAdd(a, b);

	std::cout << "xor add: " << result << std::endl;
}

void TestXORSwap()
{
	char a = 'a';
	char b = 'b';

	XORSwap(a, b);

	std::cout << a << "," << b << std::endl;
}

void TestOperatorOverload()
{
	RunOperatorOverloadTest();
}

void TestTemplateExample()
{
	RunTemplateExample();
}

void TestIterExample()
{
	RunItrExample();
}

void TestExampleDelegate()
{
	ExampleDelegateRun();
}

void TestExampleTypeConversion()
{
	ConversionExamples();
}

void TestPassing()
{
	int original = 5;

	PassByValue(original);
	std::cout << "pass by val: " << original << std::endl;

	PassByReference(original);
	std::cout << "pass by ref: " << original << std::endl;

	PassByPointer(&original);
	std::cout << "pass by ptr: " << original << std::endl;
}

void TestInterface()
{
	DeriveInterfaceExample* DerivedExample = new DeriveInterfaceExample();
	IExampleInterface* 		ExampleInterface = (IExampleInterface*)DerivedExample;

	ExampleInterface->ExamplePureVirtualFunc();

	delete ExampleInterface;
}

void TestConstructor()
{
	ConstructorExample* ConstructorInstance = new ConstructorExample();

	std::cout << ConstructorInstance->Count << std::endl;
	std::cout << ConstructorInstance->ExampleArr[3] << std::endl;

	ConstructorExample SecondConstructorExample(10);
	ConstructorExample  TestCopy(SecondConstructorExample);

	std::cout << TestCopy.ExampleArr[7] << std::endl;

	delete ConstructorInstance;
}

void TestReverseString()
{
	std::string helloStr = "hello";
	std::cout << helloStr << std::endl;

	std::string Reversed = ReverseString(helloStr);
	std::cout << Reversed << std::endl;
}

void TestReverseSrtingInPlace()
{
	std::string helloStrInPlace = "inplaceswap";
	ReverseStringInPlace(helloStrInPlace);
	std::cout << helloStrInPlace << std::endl;

}

void TestPermutations()
{
	std::string permutationStr = "abcd";
	permute(permutationStr, 0, permutationStr.length() - 1);
}

void TestLinkedList()
{
	LinkedListNode* LLNodeA = new LinkedListNode();
	LinkedListNode* LLNodeB = new LinkedListNode();
	LinkedListNode* LLNodeC = new LinkedListNode();
	LinkedListNode* LLNodeD = new LinkedListNode();

	LLNodeA->Next = LLNodeB;
	LLNodeB->Next = LLNodeC;
	LLNodeC->Next = LLNodeD;

	if (HasLoop(LLNodeA))
	{
		std::cout << "Test1: Found a loop" << std::endl;
	}
	else
	{
		std::cout << "Test1: No loop" << std::endl;
	}

	LLNodeD->Next = LLNodeA;

	if (HasLoop(LLNodeA))
	{
		std::cout << "Test2: Found a loop" << std::endl;
	}
	else
	{
		std::cout << "Test2: No loop" << std::endl;
	}

	delete LLNodeA;
	delete LLNodeB;
	delete LLNodeC;
	delete LLNodeD;
}
//-----------------
//END TEST CASES


int main()
{

	TestReverseString();
	TestReverseSrtingInPlace();
	TestPermutations();
	TestLinkedList();
	TestConstructor();
	TestInterface();
	TestPassing();
	StringIntConvertUsingSTD();
	TestExampleTypeConversion();
	TestExampleDelegate();
	TestIterExample();
	TestTemplateExample();
	TestOperatorOverload();
	RunBinaryTreeExample();
	TestXORSwap();
	TestXORAdd();
	TestShuffle();
	RunSortTest();

	std::cout << "fib: " << fib(9) << std::endl;

	TestPalindrom();

	while (true)
	{

	}


	return 0;
}
