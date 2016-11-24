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
#include "GameController.h"

//globals
BOOL g_bActiveApp;  ///< TRUE if this is the active application
HWND g_HwndApp; ///< Application window handle.
HINSTANCE g_hInstance; ///< Application instance handle.
char g_szGameName[256]; ///< Name of this game
char g_szShaderModel[256]; ///< The shader model version used
CImageFileNameList g_cImageFileName; ///< List of image file names.
CTimer g_cTimer; ///< The game timer

GameController mainController;

static int currLocation = 0; // 0: Front, 1: Top, 2: Left, 3: Right, 4: Bottom  'Rotation Location of Briefcase

int xPos;
int yPos;
int lockNum = 9;
bool stage1Complete = false;
bool win = false;
bool fail = false;
C3DSprite* g_pPlaneSprite = nullptr; ///< Pointer to the plane sprite.
C3DSprite* g_pBarcodeSprite = nullptr; ///< Pointer to the barcode sprite
CGameObject* g_pPlane = nullptr; ///< Pointer to the plane object.

CGameObject* g_pBarcode = nullptr;
CGameObject* g_pBarcodeCase = nullptr;

C3DSprite* g_pBarcodeCaseSprite = nullptr;

C3DSprite* g_pCluesSprite = nullptr;
C3DSprite* g_numberSprite0 = nullptr;
C3DSprite* g_numberSprite1 = nullptr;
C3DSprite* g_numberSprite2 = nullptr;
C3DSprite* g_numberSprite3 = nullptr;

C3DSprite* g_numberBarcodeSprite0 = nullptr;
C3DSprite* g_numberBarcodeSprite1 = nullptr;
C3DSprite* g_numberBarcodeSprite2 = nullptr;
C3DSprite* g_numberBarcodeSprite3 = nullptr;

C3DSprite* g_StageTwo = nullptr;
C3DSprite* g_failScreen = nullptr;


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
void CreateObjects();

HWND CreateDefaultWindow(char* name, HINSTANCE hInstance, int nCmdShow);

void checkStageOne(int complete) {
	if (complete == 1) {
		g_StageTwo = new C3DSprite();
		if (!g_StageTwo->Load(g_cImageFileName[21]))
			ABORT("Platform image %s not found.", g_cImageFileName[21]);
		win = true;
	}
	else if (complete == 2) {
		g_failScreen = new C3DSprite();
		if (!g_failScreen->Load(g_cImageFileName[22]))
			ABORT("Platform image %s not found.", g_cImageFileName[22]);
		win = false;
		fail = true;
	}
	else {
		// Strike has been made. Create strike animation
	}
}

void lockNumCtrl(int step, int fieldLoc) {
	if (fieldLoc == 0) {
		g_numberSprite0 = new C3DSprite();	//digit
		if (!g_numberSprite0->Load(g_cImageFileName[step]))
			ABORT("Platform image %s not found.", g_cImageFileName[step]);
	}
	else if (fieldLoc == 1) {
		g_numberSprite1 = new C3DSprite();	//digit
		if (!g_numberSprite1->Load(g_cImageFileName[step]))
			ABORT("Platform image %s not found.", g_cImageFileName[step]);
	}
	else if (fieldLoc == 2) {
		g_numberSprite2 = new C3DSprite();	//digit
		if (!g_numberSprite2->Load(g_cImageFileName[step]))
			ABORT("Platform image %s not found.", g_cImageFileName[step]);
	}
	else if (fieldLoc == 3) {
		g_numberSprite3 = new C3DSprite();	//digit
		if (!g_numberSprite3->Load(g_cImageFileName[step]))
			ABORT("Platform image %s not found.", g_cImageFileName[step]);
	}
}
void briefcaseRotation(int caseImage) {
	// Briefcase Rotations
	if (caseImage != -1) {
		g_pCluesSprite = new C3DSprite();
		if (!g_pCluesSprite->Load(g_cImageFileName[caseImage]))
			ABORT("Platform image %s not found.", g_cImageFileName[caseImage]);
		CreateObjects();
	}
	if (caseImage == 4) {
		
	}
}

void DrawBriefcase() {
	float y = g_nScreenHeight - 410;
	if (currLocation == 1 || currLocation == 4) {
		// If player clicks on number control
		if (currLocation == 1) {
			g_numberSprite0->Draw(Vector3(444, 360, 450));
			g_numberSprite1->Draw(Vector3(476, 360, 450));
			g_numberSprite2->Draw(Vector3(508, 360, 450));
			g_numberSprite3->Draw(Vector3(541, 360, 450));
		}
		// If player advances to stage 2
		if (win == true) {
			g_StageTwo->Draw(Vector3(515, y + 50, 700));
			currLocation = 5;
			mainController.setCaseLocation(5);
			stage1Complete = true;

			g_numberSprite0->Release();
			g_numberSprite1->Release();
			g_numberSprite2->Release();
			g_numberSprite3->Release();
			g_pBarcodeCaseSprite->Release();
			g_pCluesSprite->Release();
		}
		else if (fail == true)
			g_failScreen->Draw(Vector3(515, y, 445));
		else
			g_pCluesSprite->Draw(Vector3(515, y - 40, 450));
	}
	else if (currLocation == 5){
		g_StageTwo->Draw(Vector3(515, y+50, 700));	
	}
	else
		g_pCluesSprite->Draw(Vector3(515, y, 450));


	g_numberBarcodeSprite2->Draw(Vector3(200, 690, 556));
	g_numberBarcodeSprite3->Draw(Vector3(215, 690, 556));
	if (currLocation == 4) {
		g_numberBarcodeSprite0->Draw(Vector3(780, y - 50, 445));
		g_numberBarcodeSprite1->Draw(Vector3(795, y - 50, 445));
	}											  
	else {
		g_numberBarcodeSprite0->Draw(Vector3(780, y - 40, 6000));
		g_numberBarcodeSprite1->Draw(Vector3(781, y - 40, 6000));
	}
} //DrawBriefcase

BOOL inRange(const int index, const int limit) {
	return index >= 0 && index < limit;
} //inRange


void InitXMLSettings() {
	//open and load XML file
	const char* xmlFileName = "gamesettings.xml"; //Settings file name.
	if (g_xmlDocument.LoadFile(xmlFileName) != 0)
		ABORT("Cannot load settings file %s.", xmlFileName);

	//get settings tag
	g_xmlSettings = g_xmlDocument.FirstChildElement("settings"); //settings tag
	if (g_xmlSettings == nullptr) //abort if tag not found
		ABORT("Cannot find <settings> tag in %s.", xmlFileName);
} //InitXMLSettings

  /// \brief Load game settings.
  ///
  /// Load and parse game settings from a TinyXML element g_xmlSettings.

void LoadGameSettings() {
	if (!g_xmlSettings)return; //bail and fail

							   //get game name
	XMLElement* ist = g_xmlSettings->FirstChildElement("game");
	if (ist) {
		size_t len = strlen(ist->Attribute("name")); //length of name string
		strncpy_s(g_szGameName, len + 1, ist->Attribute("name"), len);
	} //if

	  //get renderer settings
	XMLElement* renderSettings =
		g_xmlSettings->FirstChildElement("renderer"); //renderer tag
	if (renderSettings) { //read renderer tag attributes
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
	float y = g_nScreenHeight - 410;
	g_pPlane = new CGameObject(
		Vector3((float)g_nScreenWidth / 2.0f, 718, 580),
		Vector3(0, 0, 0), g_pPlaneSprite);

	g_pBarcode = new CGameObject(
		Vector3(150, 720, 579),
		Vector3(0, 0, 0), g_pBarcodeSprite);

	g_pBarcodeCase = new CGameObject(
		Vector3(780, y - 40, 450),
		Vector3(0, 0, 0), g_pBarcodeCaseSprite);

	if (currLocation == 4) {
		g_pBarcodeCase = new CGameObject(
			Vector3(780, y - 40, 445),
			Vector3(0, 0, 0), g_pBarcodeCaseSprite);
	}


} //CreateObjects

  /// \brief Keyboard handler.
  ///
  /// Handler for keyboard messages from the Windows API. Takes the appropriate
  /// action when the user presses a key on the keyboard.
  /// \param keystroke Virtual key code for the key pressed
  /// \return TRUE if the game is to exit

BOOL KeyboardHandler(WPARAM keystroke) {
	switch (keystroke) {
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

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) { //handle message
	case WM_ACTIVATEAPP: g_bActiveApp = (BOOL)wParam; break; //iconize

	case WM_KEYDOWN: //keyboard hit
		if (KeyboardHandler(wParam))DestroyWindow(hwnd);
		break;
	case WM_LBUTTONDOWN:
	{
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		// Stage One rotation mouse methods briefcase
		if (!stage1Complete)
			briefcaseRotation(mainController.caseRotation(xPos, yPos, currLocation));
		// Stage One lock number controller mouse methods
		if (currLocation == 1 && !stage1Complete)
			lockNumCtrl(lockNum, mainController.lockNumberCtrl(xPos, yPos, currLocation, lockNum));
		// Check Stage one win
		if (xPos >= 555 && xPos <= 600 && yPos <= 450 && yPos >= 380 && currLocation == 1)
			checkStageOne(mainController.checkStage1());
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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShow) {
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
	if (!hwnd)return FALSE; //bail if problem creating window
	g_HwndApp = hwnd; //save window handle

	InitGraphics(); //initialize graphics
	g_pPlaneSprite = new C3DSprite(); //make a sprite
	g_pBarcodeSprite = new C3DSprite();
	g_pBarcodeCaseSprite = new C3DSprite();
	GameRenderer.LoadTextures(); //load images

	if (!g_pPlaneSprite->Load(g_cImageFileName[3])) // Top Clues Banner
		ABORT("Plane image %s not found.", g_cImageFileName[3]);

	if (!g_pBarcodeSprite->Load(g_cImageFileName[19]))
		ABORT("Platform image %s not found.", g_cImageFileName[19]);

	if (!g_pBarcodeCaseSprite->Load(g_cImageFileName[20]))
		ABORT("Platform image %s not found.", g_cImageFileName[20]);

	CreateObjects(); //create game objects

	g_pCluesSprite = new C3DSprite();				// Briefcase 
	if (!g_pCluesSprite->Load(g_cImageFileName[4]))
		ABORT("Platform image %s not found.", g_cImageFileName[4]);

	g_numberSprite0 = new C3DSprite();				// digits 
	if (!g_numberSprite0->Load(g_cImageFileName[lockNum]))
		ABORT("Platform image %s not found.", g_cImageFileName[lockNum]);
	g_numberSprite1 = new C3DSprite();				// digits 
	if (!g_numberSprite1->Load(g_cImageFileName[lockNum]))
		ABORT("Platform image %s not found.", g_cImageFileName[lockNum]);
	g_numberSprite2 = new C3DSprite();				// digits 
	if (!g_numberSprite2->Load(g_cImageFileName[lockNum]))
		ABORT("Platform image %s not found.", g_cImageFileName[lockNum]);
	g_numberSprite3 = new C3DSprite();				// digits 
	if (!g_numberSprite3->Load(g_cImageFileName[lockNum]))
		ABORT("Platform image %s not found.", g_cImageFileName[lockNum]);

	int arr[4];
	mainController.getBarcodeArr(arr);

	g_numberBarcodeSprite0 = new C3DSprite();	//digit
	if (!g_numberBarcodeSprite0->Load(g_cImageFileName[ arr[4]+9 ]))
		ABORT("Platform image %s not found.", g_cImageFileName[arr[4] + 9]);
	g_numberBarcodeSprite1 = new C3DSprite();	//digit
	if (!g_numberBarcodeSprite1->Load(g_cImageFileName[arr[1] + 9]))
		ABORT("Platform image %s not found.", g_cImageFileName[arr[1] + 9]);
	g_numberBarcodeSprite2 = new C3DSprite();	//digit
	if (!g_numberBarcodeSprite2->Load(g_cImageFileName[arr[2] + 9]))
		ABORT("Platform image %s not found.", g_cImageFileName[arr[2] + 9]);
	g_numberBarcodeSprite3 = new C3DSprite();	//digit
	if (!g_numberBarcodeSprite3->Load(g_cImageFileName[arr[3] + 9]))
		ABORT("Platform image %s not found.", g_cImageFileName[arr[3] + 9]);

	//message loop
	while (TRUE)
		if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)) { //if message waiting
			if (!GetMessage(&msg, nullptr, 0, 0))return (int)msg.wParam; //get it
			TranslateMessage(&msg); DispatchMessage(&msg); //translate it
		} //if
		else
			if (g_bActiveApp)
				GameRenderer.ProcessFrame();
			else WaitMessage();
} //WinMain
