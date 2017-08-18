// TestPractice.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>


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

//Reverse a string in any way.
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



	while (true)
	{

	}


    return 0;
}


