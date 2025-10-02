/*************************************************************************************************/
/*!
\file File.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2025.9.10
\brief
    Class to read and write files

    Functions include:
        + FILL

Copyright (c) 2025 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "File.h"
#include "cppShortcuts.h"

#include <fstream>

//-------------------------------------------------------------------------------------------------
// Private Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Function Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Private Function Declarations
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Constructor for the File class

	\param _filename
		The name of the file to be opened

	\param read
		Whether we're reading the file

	\param write
		Whether we're writing the file
*/
/*************************************************************************************************/
File::File(const std::string& _filename, bool read, bool write)
{
	// Saves the filename
	filename = _filename;

	// Sets the internal booleans
	readingFile = read;
	writingFile = write;

	// Checks if we are both reading and writing the file
	if (read && write)
	{
		
	}
	// Otherwise reads in the file
	if (read)
	{
		fileData = ReadFile(filename);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for File class
*/
/*************************************************************************************************/
File::~File()
{

}

/*********************************************************************************************/
/*!
	\brief
		Reads in the given file

	\param filename
		The given file

	\return
		A string of the file
*/
/*********************************************************************************************/
std::vector<char> File::ReadFile(const std::string& filename)
{
	// Opens the file
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	// If the file opened correctly
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	// Gets the file size
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	// Reads the file from the start
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	// Closes the file
	file.close();

	return buffer;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
