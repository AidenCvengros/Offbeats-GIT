/*************************************************************************************************/
/*!
\file Vertex.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.11.12
\brief
    

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "Vertex.h"
#include "cppShortcuts.h"

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
		Gets the description that tells vulkan how to load vertices

	\return
		The description struct
*/
/*************************************************************************************************/
VkVertexInputBindingDescription Vertex::getBindingDescription()
{
    // Defines the description for vertex binding
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    // Returns the vertex binding description struct
    return bindingDescription;
}

/*************************************************************************************************/
	/*!
		\brief
			Defines how vertices are laid out in memory

		\return
			The description structs (position and color)
	*/
	/*************************************************************************************************/
std::array<VkVertexInputAttributeDescription, 3> Vertex::getAttributeDescriptions()
{
    // Creates the attibute description structs
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

    // Defines the first struct as a vec2
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    // Defines the second struct as a vec3
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    // Defines the third struct as a vec2
    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    // Returns the description structs
    return attributeDescriptions;
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
