/*************************************************************************************************/
/*!
\file Text.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2026.4.08
\brief
    The text game object for displaying text on the screen

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Text.h"
#include "../Engine/cppShortcuts.h"

// Includes the window to interface with vulkan instance
#include "../Engine/Window.h"
#include "../Engine/Vertex.h"
#include "../Engine/Font.h"

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
		Constructor for the text object class

	\param scene
		The scene that this game object is in

	\param pos
		The position of the game object

	\param rot
		The rotation of the game object

	\param sca
		The scale of the game object
*/
/*************************************************************************************************/
Text::Text(const std::string& _text, Font* _font, int _fontSize, glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority, glm::vec4 color_) :
	GameObject(pos, rot, sca, drawPriority, true, color_),
	font(_font), fontSize(_fontSize), alignment(-1),
	vertexBuffer(NULL), vertexBufferMemory(NULL), indexBuffer(NULL), indexBufferMemory(NULL), indexCount(0),
	textScroll(false), textScrollSpeed(1.0f) 
{
	SetText(_text);
	SetRender(2);
}

/*************************************************************************************************/
/*!
	\brief
		Destructor for the text game object class
*/
/*************************************************************************************************/
Text::~Text()
{
	FreeBuffers();
}

/*************************************************************************************************/
/*!
	\brief
		Sets the given text as the new text

	\param new text
		The new string to display
*/
/*************************************************************************************************/
void Text::SetText(const std::string& newText)
{
	if (newText.length() > 0)
	{
		// Sets the text variable
		text = newText;

		// Frees the old buffers
		FreeBuffers();
		indexCount = 0;

		// Variables to track vertices and indices for rendering text
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		// Variables for the loop
		int indexOffset = 0;
		float xPos = 0.0f;
		float yPos = 0.0f;

		// Loops through the string
		for (size_t i = 0; i < text.length(); i++)
		{
			char c = text[i];
			Font::Character fontData = font->GetCharacter(c);
			float localXPos = xPos + fontData.bearing.x * fontSize / font->GetBmpHeight();
			float localYPos = yPos + (fontData.bearing.y - fontData.size.y) * fontSize / font->GetBmpHeight();// (character.Size.y - character.Bearing.y)* scale;
			//localXPos *= -1;
			//localYPos *= -1;

			float w = (float)fontData.size.x * fontSize / font->GetBmpHeight();
			float hraw = (float)fontData.size.y;
			float h = hraw * fontSize / font->GetBmpHeight();
			float u0 = (float)fontData.offset * font->GetInvertedBmpWidth();
			float v = (hraw) / font->GetBmpHeight();
			float u1 = (float)(fontData.offset + fontData.size.x) * font->GetInvertedBmpWidth();

			Vertex bottomRight({ localXPos - w, localYPos }, GetColor(), { u1, 1.0f - v });
			Vertex bottomLeft({ localXPos, localYPos }, GetColor(), { u0, 1.f - v });
			Vertex topLeft({ localXPos, localYPos + h }, GetColor(), { u0, 1.0f });
			Vertex topRight = { {localXPos - w, localYPos + h }, GetColor(), { u1, 1.0f } };
			vertices.push_back(bottomRight);
			vertices.push_back(bottomLeft);
			vertices.push_back(topLeft);
			vertices.push_back(topRight);
			indices.push_back(indexOffset + 0);
			indices.push_back(indexOffset + 1);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 2);
			indices.push_back(indexOffset + 3);
			indices.push_back(indexOffset + 0);
			indexOffset += 4;
			xPos -= (fontData.advance >> 6) * fontSize / font->GetBmpHeight();
		}

		// Creates the vertex and indices buffers
		_Window->CreateVulkanBuffer(vertexBuffer, vertexBufferMemory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(Vertex) * vertices.size(), (void*)vertices.data());
		_Window->CreateVulkanBuffer(indexBuffer, indexBufferMemory, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, sizeof(uint32_t) * indices.size(), (void*)indices.data());
		indexCount = indices.size();
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		Frees the text object's index buffer and vertex buffer
*/
/*************************************************************************************************/
void Text::FreeBuffers()
{
	if (indexBuffer || vertexBuffer)
	{
		_Window->WaitForDrawFinished();

		if (indexBuffer)
		{
			// Destroys the old index buffer
			vkUnmapMemory(_Window->GetLogicalDevice(), indexBufferMemory);
			vkDestroyBuffer(_Window->GetLogicalDevice(), indexBuffer, NULL);
			vkFreeMemory(_Window->GetLogicalDevice(), indexBufferMemory, NULL);
		}

		if (vertexBuffer)
		{
			// Destroys the old vertex buffer
			vkUnmapMemory(_Window->GetLogicalDevice(), vertexBufferMemory);
			vkDestroyBuffer(_Window->GetLogicalDevice(), vertexBuffer, NULL);
			vkFreeMemory(_Window->GetLogicalDevice(), vertexBufferMemory, NULL);
		}
	}
}
