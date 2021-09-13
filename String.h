#ifndef STRING_H
#define STRING_H 1
//Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Replacement string for C++ strings, NUL terminated
struct String
{
private:
  //Changes the default size for the string on creation
  #define START_SIZE 64

public:
  //Variables to keep track of string values
  int length;
private:
  //Keeps track of how much memory is allocated
  int allocated;
  //Char array for all the characters in the string
  char* characters;

  //Initializes the string's memory
  bool Init()
  {
    //Try to allocate memory
    characters = (char*)malloc(START_SIZE * sizeof(char));
    //Check if allocation was successful
    if (characters == NULL)
    {
      printf("ERROR: Could not allocate %i bytes for String!\n", START_SIZE);
      return false;
    }
    //Set how much space has been allocated
    allocated = START_SIZE;
		//Return success
		return true;
  }

  //Checks to reallocate space for the string
  bool Allocation(int desired)
  {
    //Keep track of size
    int newSize = allocated;
    //Get new allocation size, make sure there is room for NUL termination
    while(newSize <= desired) newSize *= 2;

    //Check if a resize is not needed
    if (newSize <= allocated) return true;

    //Resizes the memory, and set allocated amount to new
    characters = (char*)realloc(characters, newSize);
    //Check if reallocation was successful
    if (characters == NULL)
    {
      //Print error
      printf("ERROR: Was unable to realloc String from %i to %i bytes!\n", allocated, newSize);
      //Return failure
      return false;
    }
    //Set allocated amount to new size
    allocated = newSize;

    //Return success
    return true;
  }

public:
  //Constructor
  String(const char* str = "")
  {
    //Initially set values
    length = 0;

    //Allocate initial memory
    Init();

    //Check the desired size
    int desired = strlen(str);
    Allocation(desired);

    //Insert string into characters
    InsertAtEnd(str);
  }

  //Destructor
  ~String()
  {
    //Free memory used for characters
    free(characters);
  }

	//Clears the whole string
	void Clear()
	{
		//Clears the characters of the String, makes them all NUL termination
		for (int i = 0; i < length; i++) characters[i] = '\0';
		//Sets length to 0
		length = 0;
	}

	//Inserts a string at an index in the string
	bool Insert(const char* str, int index)
	{
		//Check if index is valid
		if (index >= length) return false;

		//Get length of string
    int len = strlen(str);
		//Check allocation
		if (!Allocation(length + len)) return false;

		//Move/Copy the characters backwards to make space for insertion
		for (int i = length - index + len; i >= 0; i--)
		{
			characters[index + len + i] = characters[index + i];
		}
		//Puts the string at the desired index
    for (int i = 0; i < len; i++)
    {
      characters[index + i] = str[i];
    }
    //Add desired to length
    length += len;
		//Make NUL termination
    characters[length] = '\0';
		//Return success
		return true;
	}

  //Inserts a string on the end of this String
  bool InsertAtEnd(const char* str)
  {
    //Get length of string
    int len = strlen(str);
		//Check allocation
		if (!Allocation(length + len)) return false;
    //Puts the string in on the end
    for (int i = 0; i < len; i++)
    {
      characters[length + i] = str[i];
    }
    //Add desired to length
    length += len;
    //Make NUL termination
    characters[length] = '\0';
		//Return success
		return true;
  }

	//Get a substring copy from within the string
	String Substring(int start, int end)
	{
		//Make sure start is before end, return empty string if not
		if (start >= end) return String();
		//Check if end index is after String length
		if (length < end) end = length;

		//Allocate space for the substring
		char* substr = (char*)malloc(sizeof(char) * (end - start));
		//Dump substring char after char
		for(int i = 0; i < end - start; i++)
		{
			substr[i] = characters[start + i];
		}

		//Create String with array
		String result = String(substr);
		//Clear memory for substring
		free(substr);
		//Return result
		return result;
	}

	//Get a substring copy within the string, starting at 0
	String Substring(int end)
	{
		return Substring(0, end);
	}

  //Finds a string within this string, and returns the index of where it starts
  int Find(const char* find)
  {
    //Storage for the index
    int index = -1;

    //Get the length of the string
    int findLen = strlen(find);

    //Search for the first character
    for (int i = 0; i < length - findLen; i++)
    {
      //Check if not the first character of string to find and skip
      if (characters[i] != find[0]) continue;

      //Set index premptively
      index = i;

      //Check for the rest of the string
      for (int j = 1; j < findLen; j++)
      {
        //Check if characters don't match string to find, wipe index if they don't
        if (characters[i + j] != find[j])
        {
          index = -1;
          break;
        }
      }

      //Check if index is not wiped, break if it's still set
      if (index != -1) break;
    }

    //Return the index, or -1 if we found nothing
    return index;
  }

  //Returns the raw string
  char* GetRawString()
  {
    return characters;
  }

	//Gets the length of the string
	int GetLength()
	{
		return strlen(characters);
	}

  //Gets the size of the char allocation in bytes
  size_t AllocatedSize()
  {
    return allocated;
  }

  //Operator overload for index
  char operator[](int index)
  {
    //Check if valid index, if not return -1
    if (index >= length)
    {
      //Return error
      printf("Could not access index \"%i\" in String with size %i!\n", index, length);
      return -1;
    }
    //Return character at index
    return characters[index];
  }

  //= operator overloads
  void operator=(const char* rhs)
  {
    //Clear string contents
    Clear();

    //Insert new contents
    InsertAtEnd(rhs);
  }

  void operator=(String rhs)
  {
    //Clear string contents
    Clear();

    //Insert new contents
    InsertAtEnd(rhs.characters);
  }

  //+ operator overloads
  String operator+(const char* rhs)
  {
    //Make a copy of this string
    String result = String(characters);
    //Add other string to the end
    result.InsertAtEnd(rhs);
    //Return result
    return result;
  }

  String operator+(String rhs)
  {
    //Make a copy of this string
    String result = String(characters);
    //Add other string to the end
    result.InsertAtEnd(rhs.characters);
    //Return result
    return result;
  }

  //+= operator overloads (More efficient than +)
  void operator+=(const char* rhs)
  {
    //Inserts String contents at the end
    InsertAtEnd(rhs);
  }

  void operator+=(String rhs)
  {
    //Inserts String contents at the end
    InsertAtEnd(rhs.characters);
  }
};

#endif
