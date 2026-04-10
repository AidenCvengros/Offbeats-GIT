/*************************************************************************************************/
/*!
\file Font.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.4.06
\brief
    The font class

    Functions include:
        + [FILL]

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------


// Base includes
#include "Font.h"
#include "cppShortcuts.h"

// Includes the window class to get vulkan access to make texture objects
#include "Window.h"

// Includes freetype
#include <ft2build.h>
#include FT_FREETYPE_H  

// Extra includes
#include <unordered_map>
#include "../Engine/Texture.h"

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
		Constructor for the font class

	\param filename_
		The name and location of the font file
*/
/*************************************************************************************************/
Font::Font(std::string filename_)
{
	// Sets the given variables
	filename = filename_;

	// Initializes the freetype library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	// Reads in a face
	FT_Face face;
	if (FT_New_Face(ft, filename_.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	
	// Sets the pixel size of the font
	FT_Set_Pixel_Sizes(face, 0, 48);
	uint32_t bmpWidth = 0;
	std::vector<uint8_t> pixels;
	std::unordered_map<char, std::vector<uint8_t>> data;

	// Loops through the list of normal character slots
	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		bmpHeight = max(bmpHeight, face->glyph->bitmap.rows);

		unsigned int pitch = face->glyph->bitmap.pitch;
		Character character =
		{
			glm::ivec2(face->glyph->bitmap.width,face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left,face->glyph->bitmap_top),
			bmpWidth,
			static_cast<unsigned int>(face->glyph->advance.x)
		};

		characters.insert(std::pair<char, Character>(c, character));
		if (face->glyph->bitmap.width > 0)
		{
			void* ptr = face->glyph->bitmap.buffer;

			std::vector<uint8_t> charData(face->glyph->bitmap.width * face->glyph->bitmap.rows * 4);

			int rows = face->glyph->bitmap.rows;
			int width = face->glyph->bitmap.width;
			for (int i = 0; i < rows; i++)
			{
				for (int j = 0; j < width; j++)
				{
					uint8_t byte = face->glyph->bitmap.buffer[i * pitch + j];
					charData[(i * pitch + j) * 4] = byte;
				}
			}
			data.insert(std::pair<char, std::vector<uint8_t>>(c, charData));
		}
		bmpWidth += face->glyph->bitmap.width;
		characters.insert(std::pair<char, Character>(c, character));
	}

	// Closes up freetype
	FT_Done_Face(face);

	invertedBmpWidth = 1.0f / (float)bmpWidth;

	uint8_t* buffer = new uint8_t[bmpHeight * bmpWidth * 4];
	memset(buffer, 0, bmpHeight * bmpWidth);

	uint32_t xpos = 0;
	for (unsigned char c = 0; c < 128; c++)
	{
		Character& character = characters[c];

		std::vector<uint8_t>& charData = data[c];
		uint32_t width = character.size.x;
		uint32_t height = character.size.y;
		for (uint32_t i = 0; i < height; i++) {
			for (uint32_t j = 0; j < width; j++) {
				uint8_t byte = charData[i * width + j];
				buffer[(i * bmpWidth + xpos + j) * 4] = 1;
				buffer[(i * bmpWidth + xpos + j) * 4 + 1] = 1;
				buffer[(i * bmpWidth + xpos + j) * 4 + 2] = 1;
				buffer[(i * bmpWidth + xpos + j) * 4 + 3] = byte;
			}
		}
		xpos += width;
	}

	texture = new Texture(bmpWidth, bmpHeight, buffer);

	FT_Done_FreeType(ft);
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for the camera class
*/
/*************************************************************************************************/
Font::~Font()
{
	// Frees the texture from memory if it is getting destroyed
	Free();
}

/*************************************************************************************************/
/*!
	\brief
		Frees the texture from memory
*/
/*************************************************************************************************/
void Font::Free()
{
	texture->Free();
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
