// TestPractice.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>

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
	virtual int ExampleOverride() { return 10;  }
};

class ChildExample : BaseExample
{
public:
	//Calling the base class constructor. Specific name needs to be used since c++ supports multiple inheritance. No Super.
	ChildExample(int ArgChildData) : BaseExample(ArgChildData){} 

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
		int SwapIndex		  = (StrToReverse.length() - 1) - i;
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
		StrToReverse[left]  ^= StrToReverse[right];
		StrToReverse[right] ^= StrToReverse[left];

		++left;
		--right;
	}
}

void swap(char& x, char& y)
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
			swap(a[l], a[i]);		//swap a child node with its neighbor
			permute(a, l + 1, r);   //permute the resultant string with substring to the right.
			swap(a[l], a[i]);	    //back track
		}
	}
}


//Combination (order doesn't matter ex AB == BA   so don't print dupes)
//


//Power Set print {"A", "B", "AB", "BA"}


int main()
{

	std::string helloStrInPlace = "inplaceswap";
	std::string helloStr = "hello";
	std::cout << helloStr << std::endl;

	std::string Reversed = ReverseString(helloStr);
	std::cout << Reversed << std::endl;

	ReverseStringInPlace(helloStrInPlace);
	std::cout << helloStrInPlace << std::endl;


	std::string permutationStr = "abcd";
	permute(permutationStr, 0, permutationStr.length() - 1);


	LinkedListNode* LLNodeA = new LinkedListNode();
	LinkedListNode* LLNodeB = new LinkedListNode();
	LinkedListNode* LLNodeC = new LinkedListNode();
	LinkedListNode* LLNodeD = new LinkedListNode();

	LLNodeA->Next = LLNodeB;
	LLNodeB->Next = LLNodeC;
	LLNodeC->Next = LLNodeD;

	if( HasLoop(LLNodeA) )
	{
		std::cout << "Test1: Found a loop" << std::endl;
	}
	else
	{
		std::cout << "Test1: No loop" << std::endl;
	}

	LLNodeD->Next = LLNodeA;

	if( HasLoop(LLNodeA) )
	{
		std::cout << "Test2: Found a loop" << std::endl;
	}
	else
	{
		std::cout << "Test2: No loop" << std::endl;
	}


	while (true)
	{

	}


    return 0;
}


