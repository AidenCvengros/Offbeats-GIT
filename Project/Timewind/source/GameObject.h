/*************************************************************************************************/
/*!
\file GameObject.h
\author Aiden Cvengros
\par email: ajcvengros\@gmail.com
\date 2023.11.28
\brief
    Defines the base game object class that makes up every object in the game world

    Public Functions:
        + FILL
		
	Private Functions:
		+ FILL

Copyright (c) 2023 Aiden Cvengros
*/
/*************************************************************************************************/

#ifndef Syncopatience_GameObject_H_
#define Syncopatience_GameObject_H_

#pragma once

//-------------------------------------------------------------------------------------------------
// Include Header Files
//-------------------------------------------------------------------------------------------------

#include "stdafx.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "InputManager.h"

#include "Texture.h"

//-------------------------------------------------------------------------------------------------
// Forward References
//-------------------------------------------------------------------------------------------------

class Scene;

//-------------------------------------------------------------------------------------------------
// Public Constants
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Classes
//-------------------------------------------------------------------------------------------------

/*************************************************************************************************/
/*!
	\brief
		The game object class
*/
/*************************************************************************************************/
class GameObject
{
public:
	//---------------------------------------------------------------------------------------------
	// Public Consts
	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	// Public Structures
	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	// Public Variables
	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	// Public Function Declarations
	//---------------------------------------------------------------------------------------------

	/*************************************************************************************************/
	/*!
		\brief
			Constructor for the game object class (without texture)

		\param pos
			The position of the game object

		\param rot
			The rotation of the game object

		\param sca
			The scale of the game object

		\param drawPriority_
			Higher draw priorities are drawn in front of objects with lower priority

		\param inMap_
			Whether this game object is in the map

		\param color_
			The color of the game object, defaults to clear

		\param mapCoords_
			The map coordinates that the game object is in, defaults to an invalid tile
	*/
	/*************************************************************************************************/
	GameObject(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, bool inMap_, glm::vec4 color_ = { 0.0f, 0.0f, 0.0f, 0.0f }, std::pair<int, int> mapCoords_ = { -1, -1 }) :
		active(true), toBeDestroyed(false),
		position(pos), rotation(rot), scale(sca),
		drawPriority(drawPriority_),
		moving(false),
		moveOriginalPosition(glm::vec2(0.0f, 0.0f)),
		moveNewPosition(glm::vec2(0.0f, 0.0f)),
		moveTime(0.0), moveTimeLeft(0.0),
		moveSmooth(false),
		render(false),
		texture(NULL),
		color(color_),
		inMap(inMap_),
		mapCoords(mapCoords_) {}

	/*************************************************************************************************/
	/*!
		\brief
			Constructor for the game object class (with texture)

		\param pos
			The position of the game object

		\param rot
			The rotation of the game object

		\param sca
			The scale of the game object

		\param drawPriority_
			Higher draw priorities are drawn in front of objects with lower priority (Player is at 50, Background is at 0)

		\param texture_
			The texture for the game object

		\param inMap_
			Whether this game object is in the map

		\param color_
			The color of the game object, defaults to clear

		\param mapCoords_
			The map coordinates that the game object is in, defaults to an invalid tile
	*/
	/*************************************************************************************************/
	GameObject(glm::vec2 pos, float rot, glm::vec2 sca, int drawPriority_, Texture* texture_, bool inMap_, glm::vec4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f }, std::pair<int, int> mapCoords_ = { -1, -1 }) :
		active(true), toBeDestroyed(false),
		position(pos), rotation(rot), scale(sca),
		drawPriority(drawPriority_),
		moving(false),
		moveOriginalPosition(glm::vec2(0.0f, 0.0f)),
		moveNewPosition(glm::vec2(0.0f, 0.0f)),
		moveTime(0.0), moveTimeLeft(0.0),
		moveSmooth(false),
		render(true),
		texture(texture_),
		color(color_),
		inMap(inMap_),
		mapCoords(mapCoords_) {}
	
	/*************************************************************************************************/
	/*!
		\brief
			Destructor for the game object class
	*/
	/*************************************************************************************************/
	~GameObject() {}

	/*************************************************************************************************/
	/*!
		\brief
			Updates the game object. Can be overwritten by derived classes

		\param dt
			The time elapsed since the previous frame

		\param inputManager
			Allows the game objects to check inputs
	*/
	/*************************************************************************************************/
	virtual void Update(double dt, InputManager* inputManager);

	/*************************************************************************************************/
	/*!
		\brief
			Draws the child game objects

		\param window
			The game window the objects are being drawn to
	*/
	/*************************************************************************************************/
	virtual void DrawChildObjects(Window* window) {}

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether the object is active

		\return
			True if the object is active, false if not.
	*/
	/*************************************************************************************************/
	bool GetActive() { return active; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether the object is to be destroyed

		\return
			True if the object is to be destroyed, false if not
	*/
	/*************************************************************************************************/
	bool GetToBeDestroyed() { return toBeDestroyed; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the transformation matrix for the game object

		\return
			The game object's transformation matrix
	*/
	/*************************************************************************************************/
	glm::mat4x4 GetTranformationMatrix();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the position of the game object

		\return
			The game object's position
	*/
	/*************************************************************************************************/
	glm::vec2 GetPosition();

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether the game object is moving

		\return
			Whether the game object is moving
	*/
	/*************************************************************************************************/
	bool GetMoving();

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether the game object should be rendered

		\return
			Whether the game object should be rendered
	*/
	/*************************************************************************************************/
	bool GetRender() { return render; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the game object's texture

		\return
			The game object's texture
	*/
	/*************************************************************************************************/
	Texture* GetTexture() { return texture; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns whether the object is moving

		\return
			Whether the object is moving
	*/
	/*************************************************************************************************/
	bool GetIsMoving() { return moving; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the object's draw position
	
		\return
			The object's draw position
	*/
	/*************************************************************************************************/
	int GetDrawPriority();

	/*************************************************************************************************/
	/*!
		\brief
			Returns the object's color

		\return
			The object's color
	*/
	/*************************************************************************************************/
	glm::vec4 GetColor() { return color; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the original position of the current move

		\return
			The original position of the current move
	*/
	/*************************************************************************************************/
	glm::vec2 GetOriginalMovePosition() { return moveOriginalPosition; }

	/*************************************************************************************************/
	/*!
		\brief
			Returns the original position of the current move

		\return
			The original position of the current move
	*/
	/*************************************************************************************************/
	std::pair<int, int> GetMapCoords() { return mapCoords; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets whether the game object is active

		\param newActive
			The game object's new render boolean
	*/
	/*************************************************************************************************/
	void SetActive(bool newActive) { active = newActive; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets whether the game object should be destroyed

		\param newToBeDestroyed
			Whether the game object should be destroyed
	*/
	/*************************************************************************************************/
	void SetToBeDestroyed(bool newToBeDestroyed) { toBeDestroyed = newToBeDestroyed; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets the position of the game object

		\param newPosition
			The game object's new position
	*/
	/*************************************************************************************************/
	void SetPosition(glm::vec2 newPosition);

	/*************************************************************************************************/
	/*!
		\brief
			Sets the rotation of the game object

		\param newRotation
			The game object's new rotation
	*/
	/*************************************************************************************************/
	void SetRotation(float newRotation);

	/*************************************************************************************************/
	/*!
		\brief
			Sets the scale of the game object

		\param newScale
			The game object's new scale
	*/
	/*************************************************************************************************/
	void SetScale(glm::vec2 newScale);

	/*************************************************************************************************/
	/*!
		\brief
			Sets whether the game object should be drawn

		\param newRender
			The game object's new render boolean
	*/
	/*************************************************************************************************/
	void SetRender(bool newRender) { render = newRender; }

	/*************************************************************************************************/
	/*!
		\brief
			Sets the game object's color

		\param newColor
			The game object's new color
	*/
	/*************************************************************************************************/
	void SetColor(glm::vec4 newColor) { color = newColor; }

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
	void MoveTo(glm::vec2 newPosition, double time, bool smooth);

	/*************************************************************************************************/
	/*!
		\brief
			Sets the position of the game object

		\param dt
			The time elapsed since the previous frame
	*/
	/*************************************************************************************************/
	void MoveToUpdate(double dt);
	
private:
	//---------------------------------------------------------------------------------------------
	// Private Consts
	//---------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------------------------
	// Private Structures
	//---------------------------------------------------------------------------------------------
	
	//---------------------------------------------------------------------------------------------
	// Private Variables
	//---------------------------------------------------------------------------------------------

	bool active;								// Is the game object on and being updated
	bool toBeDestroyed;							// Boolean for the game object manager to delete the object
	
	glm::vec2 position;							// The position of the game object
	float rotation;								// The rotation (in 2d space) of the game object
	glm::vec2 scale;							// The scale of the game object
	int drawPriority;							// How forward the game object is drawn

	bool moving;								// Whether the move-to action is active
	glm::vec2 moveOriginalPosition;				// The starting position of the movement
	glm::vec2 moveNewPosition;					// The position the object is moving to
	double moveTime;							// How long the whole move should take
	double moveTimeLeft;						// How much longer the move has
	bool moveSmooth;							// Should the movement be smooth

	bool render;								// Says whether this game object should be rendered
	Texture* texture;							// Pointer to the object's texture (will not be referenced if render is false)
	glm::vec4 color;							// The color of the game object

	bool inMap;									// Whether the game object is in the map
	std::pair<int, int> mapCoords;				// The coordinates of the object in the map

	//---------------------------------------------------------------------------------------------
	// Private Function Declarations
	//---------------------------------------------------------------------------------------------
};

//-------------------------------------------------------------------------------------------------
// Public Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Public Functions
//-------------------------------------------------------------------------------------------------

#endif // Syncopatience_GameObject_H_
