/// \file gamerenderer.h 
/// \brief Definition of the renderer class CGameRenderer.

#pragma once

#include "renderer.h"
#include "defines.h"
#include "Shader.h"

/// \brief The game renderer.
///
/// The game renderer class handles all the nasty Direct3D details associated
/// with game related rendering tasks, including drawing the game background.

class CGameRenderer: public CRenderer{
  private: 
    //Direct3D stuff for background floor and wall
    ID3D11Buffer* m_pBackgroundVB;  ///< Vertex buffer.
    ID3D11ShaderResourceView* m_pWallTexture; ///< Texture for wall.
    ID3D11ShaderResourceView* m_pFloorTexture; ///< Texture for floor.
    ID3D11ShaderResourceView* m_pWireframeTexture; ///< Texture for showing wireframe, all black.
    ID3D11Buffer* m_pConstantBuffer; ///< Constant buffer for shader.
    CShader* m_pShader; ///< Pointer to an instance of the shader class.

    BOOL m_bCameraDefaultMode; ///< Camera in default mode.
 
  public:
    CGameRenderer(); ///< Constructor.

    void InitBackground(); ///< Initialize the background.
    void DrawBackground(); ///< Draw the background.
  
    void LoadTextures(); ///< Load textures for image storage.
    void Release(); ///< Release offscreen images.

    void ComposeFrame(); ///< Compose a frame of animation.
    void ProcessFrame(); ///< Process a frame of animation.

    void FlipCameraMode(); ///< Flip the camera mode.
}; //CGameRenderer 
