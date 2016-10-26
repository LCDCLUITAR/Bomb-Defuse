/// \file object.cpp
/// \brief Code for the game object class CGameObject.

#include "object.h"

#include "defines.h" 
#include "timer.h" 
#include "debug.h"

extern CTimer g_cTimer;
extern int g_nScreenWidth;
extern int g_nScreenHeight;
extern XMLElement* g_xmlSettings;


/// Initialize a game object. Gets object-dependent settings from g_xmlSettings
/// from the "object" tag that has the same "name" attribute as parameter name.
/// Assumes that the sprite manager has loaded the sprites already.
/// \param s Initial location of object 
/// \param v Initial velocity
/// \param sprite Pointer to the sprite for this object.

CGameObject::CGameObject(const Vector3& s, const Vector3& v, C3DSprite *sprite){ 
  m_nLastMoveTime = g_cTimer.time(); //time
  m_vPos =s; //location
  m_vVelocity = v; //velocity
  m_pSprite = sprite; //sprite pointer
} //constructor

/// Draw the current sprite frame at the current position, then
/// compute which frame is to be drawn next time.

void CGameObject::draw(){ //draw
  if(m_pSprite) //if there is a sprite
    m_pSprite->Draw(m_vPos); //draw in correct place
} //draw
 
/// The distance that an object moves depends on its speed, 
/// and the amount of time since it last moved.

void CGameObject::move(){ //move object
  const float SCALE = 32.0f; //to scale back motion
  const float MARGIN = 200.0f; //margin on top of page

  const int time = g_cTimer.time(); //current time
  const int tdelta = time - m_nLastMoveTime; //time since last move
  const float tfactor = tdelta/SCALE; //scaled time factor


  m_nLastMoveTime = time; //record time of move
} //move
void CGameObject::jump() {

} //jump