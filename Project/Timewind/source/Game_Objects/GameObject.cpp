/*************************************************************************************************/
/*!
\file GameObject.cpp
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.11.28
\brief
    Defines the game object class for every game object in the game world

    Functions include:
        + FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

// Base includes
#include "GameObject.h"
#include "../Engine/cppShortcuts.h"

// Includes the map matrix so the game object can move within the map
#include "../Gameplay/MapMatrix.h"

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
		Constructor for the game object class (no texture (not rendered), in map, centered on map tile)

	\param mapCoords_
		The map coordinates that the game object is in, defaults to an invalid tile
*/
/*************************************************************************************************/
GameObject::GameObject(std::pair<int, int> mapCoords_) :
	active(true), toBeDestroyed(false),
	rotation(0.0f), scale({ 2.0f, 2.0f }),
	drawPriority(-100), facingRight(true),
	moving(false),
	moveTime(0.0), moveTimeLeft(0.0), moveSmooth(false),
	render(false), texture(NULL), color(0.0f),
	inMap(true), mapCoords(mapCoords_)
{
	_MapMatrix->UpdateObjectPosition(mapCoords_, this);
	moveOriginalPosition = position;
	moveNewPosition = position;
}

/*************************************************************************************************/
/*!
	\brief
		Constructor for the game object class (no texture (solid color), in map, default position in tile)

	\param drawPriority_
		Higher draw priorities are drawn in front of objects with lower priority

	\param color_
		The color of the game object, defaults to clear

	\param mapCoords_
		The map coordinates that the game object is in, defaults to an invalid tile
*/
/*************************************************************************************************/
GameObject::GameObject(int drawPriority_, glm::vec4 color_, std::pair<int, int> mapCoords_) :
	active(true), toBeDestroyed(false),
	rotation(0.0f), scale({ 2.0f, 2.0f }),
	drawPriority(drawPriority_), facingRight(true),
	moving(false), moveOriginalPosition(glm::vec2(0.0f, 0.0f)), moveNewPosition(glm::vec2(0.0f, 0.0f)),
	moveTime(0.0), moveTimeLeft(0.0), moveSmooth(false),
	render(false), texture(NULL), color(color_),
	inMap(true), mapCoords(mapCoords_)
{
	_MapMatrix->UpdateObjectPosition(mapCoords_, this);
	moveOriginalPosition = position;
	moveNewPosition = position;
}

/*************************************************************************************************/
/*!
	\brief
		Constructor for the game object class (with texture, in map, centered on map tile)

	\param drawPriority_
		Higher draw priorities are drawn in front of objects with lower priority (Player is at 50, Background is at 0)

	\param texture_
		The texture for the game object

	\param color_
		The color of the game object, defaults to clear

	\param mapCoords_
		The map coordinates that the game object is in, defaults to an invalid tile
*/
/*************************************************************************************************/
GameObject::GameObject(int drawPriority_, Texture* texture_, glm::vec4 color_, std::pair<int, int> mapCoords_) :
	active(true), toBeDestroyed(false),
	rotation(0.0f), scale({ 2.0f, 2.0f }),
	drawPriority(drawPriority_), facingRight(true),
	moving(false), moveOriginalPosition(glm::vec2(0.0f, 0.0f)), moveNewPosition(glm::vec2(0.0f, 0.0f)),
	moveTime(0.0), moveTimeLeft(0.0), moveSmooth(false),
	render(true), texture(texture_), color(color_),
	inMap(true), mapCoords(mapCoords_)
{
	_MapMatrix->UpdateObjectPosition(mapCoords_, this);
	moveOriginalPosition = position;
	moveNewPosition = position;
}

/*************************************************************************************************/
/*!
	\brief
		Updates the game object. Can be overwritten by derived classes

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void GameObject::Update(double dt)
{
	// Updates movements if any are active
	if (moving)
	{
		MoveToUpdate(dt);
	}
}

/*************************************************************************************************/
/*!
	\brief
		Returns the transformation matrix for the game object

	\return
		The game object's transformation matrix
*/
/*************************************************************************************************/
glm::mat4x4 GameObject::GetTranformationMatrix()
{
	// Adjusts transformation to flip the sprite if facing left
	float directionModifier;
	if (facingRight)
	{
		directionModifier = -1;
	}
	else
	{
		directionModifier = 1;
	}

	// Calculates and returns the transformation matrix
	return glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.0f))
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation * directionModifier), glm::vec3(0.0f, 0.0f, 1.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(scale.x * directionModifier, scale.y, 1.0f));
}

/*************************************************************************************************/
/*!
	\brief
		Returns the position of the game object

	\return
		The game object's position
*/
/*************************************************************************************************/
glm::vec2 GameObject::GetPosition()
{
	return position;
}

/*************************************************************************************************/
/*!
	\brief
		Returns whether the game object is moving

	\return
		Whether the game object is moving
*/
/*************************************************************************************************/
bool GameObject::GetMoving()
{
	return moving;
}

/*************************************************************************************************/
/*!
	\brief
		Returns the object's draw position

	\return
		The object's draw position
*/
/*************************************************************************************************/
int GameObject::GetDrawPriority()
{
	return drawPriority;
}

/*************************************************************************************************/
/*!
	\brief
		Returns whether the object is facing right or left

	\return
		True if the object is facing right, false if facing left
*/
/*************************************************************************************************/
bool GameObject::GetIsFacingRight()
{
	return facingRight;
}

/*************************************************************************************************/
/*!
	\brief
		Sets the position of the game object

	\param newPosition
		The game object's new position
*/
/*************************************************************************************************/
void GameObject::SetPosition(glm::vec2 newPosition)
{
	position = { newPosition.x, newPosition.y };
}

/*************************************************************************************************/
/*!
	\brief
		Sets the rotation of the game object

	\param newRotation
		The game object's new rotation
*/
/*************************************************************************************************/
void GameObject::SetRotation(float newRotation)
{
	rotation = newRotation;
}

/*************************************************************************************************/
/*!
	\brief
		Sets the scale of the game object

	\param newScale
		The game object's new scale
*/
/*************************************************************************************************/
void GameObject::SetScale(glm::vec2 newScale)
{
	scale = newScale;
}

/*************************************************************************************************/
/*!
	\brief
		Sets whether the object is facing right or left

	\param newIsFacingRight
		True if facing right, false if facing left
*/
/*************************************************************************************************/
void GameObject::SetIsFacingRight(bool newIsFacingRight)
{
	facingRight = newIsFacingRight;
}

/*************************************************************************************************/
/*!
	\brief
		Sets whether the game object should be drawn

	\param newRender
		The game object's new render boolean
*/
/*************************************************************************************************/
void GameObject::SetRender(bool newRender)
{
	// Sets the new render boolean and turns off draw this frame
	render = newRender;
	drawThisFrame = false;
}

/*************************************************************************************************/
/*!
	\brief
		Marks this game object to be drawn this frame

	\param _drawThisFrame
		What the draw this frame variable should be set to
*/
/*************************************************************************************************/
void GameObject::DrawThisFrame(bool _drawThisFrame)
{
	// If we are drawing this frame
	if (_drawThisFrame)
	{
		// Sets the game object to be drawn
		render = true;
	}

	// Sets that the game should become inactive after this frame
	drawThisFrame = _drawThisFrame;
}

/*************************************************************************************************/
/*!
	\brief
		Sets a new movement

	\param newPosition
		The game object's new position

	\param time
		How long it takes the object to arrive there

	\param smooth
		Whether to move linearly of smooth into/out of the motion
*/
/*************************************************************************************************/
void GameObject::MoveTo(glm::vec2 newPosition, double time, bool smooth)
{
	// Sets the variable for the new move
	moving = true;
	moveOriginalPosition = position;
	moveNewPosition = newPosition;
	moveTime = time;
	moveTimeLeft = time;
	moveSmooth = smooth;
}

/*************************************************************************************************/
/*!
	\brief
		Updates an object's active movement

	\param dt
		The time elapsed since the previous frame
*/
/*************************************************************************************************/
void GameObject::MoveToUpdate(double dt)
{
	// Decrements the time left by dt
	moveTimeLeft -= dt;

	// Checks that the movement is not done
	if (moveTimeLeft > 0.0)
	{
		// Moves the object
		SetPosition((moveNewPosition - moveOriginalPosition) * (float)(1 - (moveTimeLeft / moveTime)) + moveOriginalPosition);
	}
	// Otherwise sets the position to the final position and ends the movement
	else
	{
		SetPosition(moveNewPosition);
		moving = false;
	}
}


/*************************************************************************************************/
/*!
	\brief
		Calculates the world position of the game object at the relative position

	\param position
		The enum saying what position to return

	\return
		The calculated position
*/
/*************************************************************************************************/
glm::vec2 GameObject::CalculateRelativePositions(GameObject::Positions anchorPoint)
{
	// Gets the game object's position
	glm::vec2 position = GetPosition();

	// Calculate the relative position
	switch (anchorPoint)
	{
	case GameObject::Positions::Center:
		return position + (GetScale() * 0.5f);
		break;
	case GameObject::Positions::BottomLeft:
		return { position.x, position.y };
		break;
	case GameObject::Positions::BottomRight:
		return { position.x + GetScale().x, position.y };
		break;
	case GameObject::Positions::TopLeft:
		return { position.x, position.y + GetScale().y };
		break;
	case GameObject::Positions::TopRight:
		return { position.x + GetScale().x, position.y + GetScale().y };
		break;
	case GameObject::Positions::TopCenter:
		return { position.x + (GetScale().x * 0.5f), position.y + GetScale().y };
		break;
	default:
		// If something went wrong, returns (-1, -1)
		return { -1, -1 };
		break;
	}
}

//-------------------------------------------------------------------------------------------------
// Private Function Definitions
//-------------------------------------------------------------------------------------------------
