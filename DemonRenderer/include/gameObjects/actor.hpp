/** \file actor.hpp */
#pragma once

#include "gameObjects/transformable.hpp"
#include "gameObjects/renderable.hpp"
#include "gameObjects/scriptable.hpp"


/**	\class Actor 
*	\brief Combine behavoiur into one class as a game actor
*	Essentially this is an example of polymoprhic game objects
*/

class Actor : public Transformable, public Renderable, public Scriptable
{
public:
	Actor() = default; //!< Default constructor
};