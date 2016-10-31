/// \file main.cpp 
/// \brief Main file for for Demo 3.

/// \mainpage Demo 3: The Sprite
///
/// Now we see our first simple real-time animation. A plane moves from
/// right to left across the background. The plane is represented by
/// an instance of the class CGameObject, which is an abstraction
/// of a game object including physical properties such as location
/// and speed, and a pointer to its image, which is an instance of
/// the class C3DSprite (the term <em>sprite</em> means a small image 
/// to be pasted in the correct place in the overall scene).
///
/// The executable file for this app now has an icon of a red plane
/// instead of the default Windows icon.

#include <windows.h>
#include <windowsx.h>

#include "defines.h"
#include "abort.h"
#include "gamerenderer.h"

#include "imagefilenamelist.h"
#include "debug.h"
#include "timer.h"
#include "sprite.h"
#include "object.h"

//globals
BOOL g_bActiveApp;  ///< TRUE if this is the active application
HWND g_HwndApp; ///< Application window handle.
HINSTANCE g_hInstance; ///< Application instance handle.
char g_szGameName[256]; ///< Name of this game
char g_szShaderModel[256]; ///< The shader model version used
CImageFileNameList g_cImageFileName; ///< List of image file names.
CTimer g_cTimer; ///< The game timer
static int currLocation = 0; // 0: Front, 1: Top, 2: Left, 3: Right, 4: Bottom  'Rotation Location of Briefcase

int xPos;
int yPos;

C3DSprite* g_pPlaneSprite = nullptr; ///< Pointer to the plane sprite.
C3DSprite* g_pBarcodeSprite = nullptr; ///< Pointer to the barcode sprite
CGameObject* g_pPlane = nullptr; ///< Pointer to the plane object.
CGameObject* g_pBarcode = nullptr;

C3DSprite* g_pCluesSprite = nullptr;

const float HAMSTER_HT = 128.0f;
const float HALF_HAMSTER_HT = HAMSTER_HT / 2.0f;


//graphics settings
int g_nScreenWidth; ///< Screen width.
int g_nScreenHeight; ///< Screen height.
BOOL g_bWireFrame = FALSE; ///< TRUE for wireframe rendering.

//XML settings
tinyxml2::XMLDocument g_xmlDocument; ///< TinyXML document for settings.
XMLElement* g_xmlSettings = nullptr; ///< TinyXML element for settings tag.

//debug variables
#ifdef DEBUG_ON
  CDebugManager g_cDebugManager; ///< The debug manager.
#endif //DEBUG_ON

CGameRenderer GameRenderer; ///< The game renderer.
//functions in Window.cpp
void InitGraphics();

HWND CreateDefaultWindow(char* name, HINSTANCE hInstance, int nCmdShow);

/// \brief Initialize XML settings.
///
/// Open an XML file and prepare to read settings from it. Settings
/// tag is loaded to XML element g_xmlSettings for later processing. Abort if it
/// cannot load the file or cannot find settings tag in loaded file.

void briefcaseRotation(int x, int y){
	// Rotations from Front View
	if (currLocation == 0) {
		// Top Arrow Selected from Front View
		if (x >= 0 && y >= 100 && y <= 144) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Top
			if (!g_pCluesSprite->Load(g_cImageFileName[5]))
				ABORT("Platform image %s not found.", g_cImageFileName[5]);
			currLocation = 1; // Top Location
		}
		// Bottom Arrow Selected
		if (x >= 0 && y >= 680 && y <= 768) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Front
			if (!g_pCluesSprite->Load(g_cImageFileName[8]))
				ABORT("Platform image %s not found.", g_cImageFileName[8]);
			currLocation = 4;
		}
		// Left Arrow Selected
		if (x <= 160 && y >= 100 && y <= 768) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Left
			if (!g_pCluesSprite->Load(g_cImageFileName[7]))
				ABORT("Platform image %s not found.", g_cImageFileName[7]);
			currLocation = 2;
		}
		// Right Arrow Selected
		if (x >= 865 && y >= 100 && y <= 768) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Right
			if (!g_pCluesSprite->Load(g_cImageFileName[6]))
				ABORT("Platform image %s not found.", g_cImageFileName[6]);
			currLocation = 3;
		}
	}
	// Rotations from Top View
	else if (currLocation == 1) {
		// Bottom Arrow Selected
		if (x >= 0 && y >= 600 && y <= 768) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Front
			if (!g_pCluesSprite->Load(g_cImageFileName[4]))
				ABORT("Platform image %s not found.", g_cImageFileName[4]);
			currLocation = 0;
		}
	}
	// Rotations from Left View
	else if (currLocation == 2) {
		// Right Arrow Selected
		if (x >= 800 && y >= 100 && y <= 768) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Front
			if (!g_pCluesSprite->Load(g_cImageFileName[4]))
				ABORT("Platform image %s not found.", g_cImageFileName[4]);
			currLocation = 0;
		}		
	}
	// Rotations from Right View
	else if (currLocation == 3) {
		// Left Arrow Selected
		if (x <= 160 && y >= 100 && y <= 768) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Front
			if (!g_pCluesSprite->Load(g_cImageFileName[4]))
				ABORT("Platform image %s not found.", g_cImageFileName[4]);
			currLocation = 0;
		}
	}
	// Rotations from Bottom View
	else if (currLocation == 4) {
		// Top Arrow Selected
		if (x >= 0 && y >= 100 && y <= 160) {
			g_pCluesSprite = new C3DSprite();				// Briefcase Front
			if (!g_pCluesSprite->Load(g_cImageFileName[4]))
				ABORT("Platform image %s not found.", g_cImageFileName[4]);
			currLocation = 0;
		}
	}
}

void DrawBriefcase() {
	float y = g_nScreenHeight - 410;
	if(currLocation == 1 || currLocation == 4)
		g_pCluesSprite->Draw(Vector3(515, y-40, 450));
	else
		g_pCluesSprite->Draw(Vector3(515, y, 450));
} //DrawBriefcase

BOOL inRange(const int index, const int limit) {
	return index >= 0 && index < limit;
} //inRange


void InitXMLSettings(){
  //open and load XML file
  const char* xmlFileName = "gamesettings.xml"; //Settings file name.
  if(g_xmlDocument.LoadFile(xmlFileName) != 0)
    ABORT("Cannot load settings file %s.", xmlFileName);

  //get settings tag
  g_xmlSettings = g_xmlDocument.FirstChildElement("settings"); //settings tag
  if(g_xmlSettings == nullptr) //abort if tag not found
    ABORT("Cannot find <settings> tag in %s.", xmlFileName);
} //InitXMLSettings

/// \brief Load game settings.
///
/// Load and parse game settings from a TinyXML element g_xmlSettings.

void LoadGameSettings(){
  if(!g_xmlSettings)return; //bail and fail

  //get game name
  XMLElement* ist = g_xmlSettings->FirstChildElement("game"); 
  if(ist){
    size_t len = strlen(ist->Attribute("name")); //length of name string
    strncpy_s(g_szGameName, len+1, ist->Attribute("name"), len); 
  } //if

  //get renderer settings
  XMLElement* renderSettings = 
    g_xmlSettings->FirstChildElement("renderer"); //renderer tag
  if(renderSettings){ //read renderer tag attributes
    g_nScreenWidth = renderSettings->IntAttribute("width");
    g_nScreenHeight = renderSettings->IntAttribute("height");

    size_t len = strlen(renderSettings->Attribute("shadermodel")); //length shader model string
    strncpy_s(g_szShaderModel, len + 1, renderSettings->Attribute("shadermodel"), len);
  } //if

  //get image file names
  g_cImageFileName.GetImageFileNames(g_xmlSettings);

  //get debug settings
  #ifdef DEBUG_ON
    g_cDebugManager.GetDebugSettings(g_xmlSettings);
  #endif //DEBUG_ON
} //LoadGameSettings

/// \brief Create game objects. 
///
/// Create a plane object, we'll add more objects in later demos.

void CreateObjects() {
	g_pPlane = new CGameObject(
		Vector3((float)g_nScreenWidth / 2.0f, 718, 580),
		Vector3(0, 0, 0), g_pPlaneSprite);

	g_pBarcode = new CGameObject(
		Vector3(150, 720, 579),
		Vector3(0, 0, 0), g_pBarcodeSprite);
} //CreateObjects

/// \brief Keyboard handler.
///
/// Handler for keyboard messages from the Windows API. Takes the appropriate
/// action when the user presses a key on the keyboard.
/// \param keystroke Virtual key code for the key pressed
/// \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM keystroke){ 
  switch(keystroke){
    case VK_ESCAPE: //exit game
      return TRUE; //exit keyboard handler
      break;

    case VK_F1: //flip camera mode
      GameRenderer.FlipCameraMode(); 
      break;

    case VK_F2: //toggle wireframe mode
      g_bWireFrame = !g_bWireFrame;
      GameRenderer.SetWireFrameMode(g_bWireFrame);
      break;
	case VK_SPACE:
		g_pPlane->jump();
		break;
  } //switch

  return FALSE; //normal exit
} //KeyboardHandler

/// \brief Window procedure.
///
/// Handler for messages from the Windows API. 
/// \param hwnd Window handle
/// \param message Message code
/// \param wParam Parameter for message 
/// \param lParam Second parameter for message
/// \return 0 if message is handled

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
  switch(message){ //handle message
    case WM_ACTIVATEAPP: g_bActiveApp = (BOOL)wParam; break; //iconize

	case WM_KEYDOWN: //keyboard hit
      if(KeyboardHandler(wParam))DestroyWindow(hwnd);
      break;
	case WM_LBUTTONDOWN:
	{
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		printf("x: %d - y: %d", xPos, yPos);
		briefcaseRotation(xPos, yPos);
	}
		break;
    case WM_DESTROY: //on exit
      GameRenderer.Release(); //release textures
      delete g_pPlane; //delete the plane object
      delete g_pPlaneSprite; //delete the plane sprite
	  delete g_pCluesSprite;
      PostQuitMessage(0); //this is the last thing to do on exit
      break;

    default: //default window procedure
      return DefWindowProc(hwnd, message, wParam, lParam);
  } //switch(message)

  return 0;
} //WindowProc

/// \brief Winmain.  
///         
/// Main entry point for this application. 
/// \param hInst Handle to the current instance of this application.
/// \param hPrevInst Handle to previous instance, deprecated.
/// \param lpCmdLine Command line string, unused. 
/// \param nShow Specifies how the window is to be shown.
/// \return TRUE if application terminates correctly.

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShow){
  MSG msg; //current message
  HWND hwnd; //handle to fullscreen window

  #ifdef DEBUG_ON
    g_cDebugManager.open(); //open debug streams, settings came from XML file
  #endif //DEBUG_ON

  g_hInstance = hInst;
  g_cTimer.start(); //start game timer
  InitXMLSettings(); //initialize XML settings reader
  LoadGameSettings();

  //create fullscreen window
  hwnd = CreateDefaultWindow(g_szGameName, hInst, nShow);
  if(!hwnd)return FALSE; //bail if problem creating window
  g_HwndApp = hwnd; //save window handle
  
  InitGraphics(); //initialize graphics
  g_pPlaneSprite = new C3DSprite(); //make a sprite
  g_pBarcodeSprite = new C3DSprite();
  GameRenderer.LoadTextures(); //load images

  if(!g_pPlaneSprite->Load(g_cImageFileName[3])) // Top Clues Banner
    ABORT("Plane image %s not found.", g_cImageFileName[3]);

  if (!g_pBarcodeSprite->Load(g_cImageFileName[9]))
	  ABORT("Platform image %s not found.", g_cImageFileName[9]);

  CreateObjects(); //create game objects

  g_pCluesSprite = new C3DSprite();				// Briefcase 
  if (!g_pCluesSprite->Load(g_cImageFileName[4]))
	  ABORT("Platform image %s not found.", g_cImageFileName[4]);
  //message loop
  while(TRUE)
    if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)){ //if message waiting
      if(!GetMessage(&msg, nullptr, 0, 0))return (int)msg.wParam; //get it
      TranslateMessage(&msg); DispatchMessage(&msg); //translate it
    } //if
    else 
      if(g_bActiveApp)
        GameRenderer.ProcessFrame(); 
      else WaitMessage();
} //WinMain
