/// \file object.h
/// \brief Interface for the game object class CGameObject.

#pragma once

#include "sprite.h"
#include "defines.h"

/// \brief The game object. 
///
/// Game objects are responsible for remembering information about 
/// themselves - including their image, location, and speed - and
/// for moving and drawing themselves.

class CGameObject{ //class for a game object
  friend class CObjectManager;
  friend class CGameRenderer;

  private:
    Vector3 m_vPos; ///< Current location.
    Vector3 m_vVelocity; ///< Current velocity.
    int m_nLastMoveTime; ///< Last time moved.

    C3DSprite *m_pSprite; ///< Pointer to sprite.

  public:
    CGameObject(const Vector3& s, const Vector3& v, C3DSprite *sprite); ///< Constructor.
    void draw(); ///< Draw at current location.
    void move(); ///< Change location depending on time and speed
	void jump(); // Jump function
}; //CGameObject

