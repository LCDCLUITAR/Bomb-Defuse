/// \file defines.h
/// \brief Essential defines.

#pragma once

#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>

#include "SimpleMath.h"
#include "tinyxml2.h"

using namespace DirectX;
using namespace SimpleMath;
using namespace tinyxml2;
using namespace std;

/// Safe release of a pointer to a Windows COM object. If
/// the pointer is not null, then release what it is pointing
/// to and set it to null.

//Beware the extra parentheses in this #define. They are there
//for a reason. 

#define SAFE_RELEASE(p) if(p){(p)->Release(); (p) = nullptr;}

/// Safe delete of a pointer. Note that it is OK to delete a null
/// pointer. Note the brackets and braces.

#define SAFE_DELETE(p) {delete (p); (p) = nullptr;}

/// \brief Billboard vertex structure. 
///
/// Custom vertex format for representing a vanilla billboard object. Consists 
/// of position, and texture coordinates.

struct BILLBOARDVERTEX{ 
  Vector3 p; ///< Position.
  float tu; ///< Texture U coordinate.
  float tv; ///< Texture V coordinate.
}; //BILLBOARDVERTEX

/// Constant buffer for use by shaders.

struct ConstantBuffer{
  XMFLOAT4X4 wvp; ///< World View Projection matrix
}; //ConstantBuffer
