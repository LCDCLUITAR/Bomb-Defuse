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
int strikes = 0;
bool stage1Complete = false;
bool win = false;
bool fail = false;
bool menu_Screen = true;
bool shapeOnScreen = false;
bool clearShapes = false;
static int counterShape = 0;
int shapeResult = 0;
bool shapesPuzzleSolved = false;
C3DSprite* g_pTopCluesBannerSprite = nullptr; ///< Pointer to the plane sprite.
C3DSprite* g_pBarcodeSprite = nullptr; ///< Pointer to the barcode sprite
CGameObject* g_pTopClues = nullptr; ///< Pointer to the plane object.
C3DSprite* g_mainMenu = nullptr;

CGameObject* g_pBarcode = nullptr;
CGameObject* g_pBarcodeCase = nullptr;

C3DSprite* g_pBarcodeCaseSprite = nullptr;

C3DSprite* g_pBriefcaseSprite = nullptr;
C3DSprite* g_pArrows = nullptr;

C3DSprite* g_pLedLitSprite = nullptr;

C3DSprite* g_pStrike1 = nullptr;
C3DSprite* g_pStrike2 = nullptr;
C3DSprite* g_pStrike3 = nullptr;

C3DSprite* g_pArrows0 = nullptr;
C3DSprite* g_pArrows1 = nullptr;
C3DSprite* g_pArrows2 = nullptr;
C3DSprite* g_pArrows3 = nullptr;

C3DSprite* g_numberSprite0 = nullptr;
C3DSprite* g_numberSprite1 = nullptr;
C3DSprite* g_numberSprite2 = nullptr;
C3DSprite* g_numberSprite3 = nullptr;

C3DSprite* g_numberBarcodeSprite0 = nullptr;
C3DSprite* g_numberBarcodeSprite1 = nullptr;
C3DSprite* g_numberBarcodeSprite2 = nullptr;
C3DSprite* g_numberBarcodeSprite3 = nullptr;

C3DSprite* g_pTimerNumber0 = nullptr;
C3DSprite* g_pTimerNumber1 = nullptr;
C3DSprite* g_pTimerNumber2 = nullptr;
C3DSprite* g_pTimerNumber3 = nullptr;
C3DSprite* g_pTimerNumber4 = nullptr;
C3DSprite* g_pTimerNumber5 = nullptr;
C3DSprite* g_pTimerNumber6 = nullptr;
C3DSprite* g_pTimerNumber7 = nullptr;
C3DSprite* g_pTimerNumber8 = nullptr;
C3DSprite* g_pTimerNumber9 = nullptr;

C3DSprite* g_shapeClueSprite1 = nullptr;
C3DSprite* g_shapeClueSprite2 = nullptr;
C3DSprite* g_shapeClueSprite3 = nullptr;
C3DSprite* g_shapeClueSprite4 = nullptr;

C3DSprite* g_StageTwo = nullptr;
C3DSprite* g_failScreen = nullptr;

C3DSprite* g_pMinuteRightSprite = nullptr;
C3DSprite* g_pSecondsRightSprite = nullptr;
C3DSprite* g_pSecondsLefttSprite = nullptr;

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

void checkStage2(string clue) {
	if (clue == "CheckShape") {
		if (shapeResult != 4) {
			mainController.addStrike();
		}
		else {
			shapesPuzzleSolved = true;
			g_pLedLitSprite = new C3DSprite();
			if (!g_pLedLitSprite->Load(g_cImageFileName[47]))
				ABORT("Platform image %s not found.", g_cImageFileName[47]);
		}
	}
}

void DrawTimer() {
	bool started = false;
	int timerTime, currTime, elapsedTime, minutes, seconds, secondsLeft;
	float y = g_nScreenHeight - 410;

	int TimerTotalMinutes = 5;				// Timer in minutes
	
	if(started)
		g_cTimer.start();

	timerTime = TimerTotalMinutes * 60;		// 5 Minutes in seconds
	currTime = g_cTimer.time() / 1000;		// Convert Current Time to Seconds
	elapsedTime = timerTime - currTime;		// Seconds Left untl time out

	if (elapsedTime <= 0) {
		mainController.addStrike();
		mainController.addStrike();
		mainController.addStrike();
		fail = true;
		return;
	}
	minutes = elapsedTime / 60;
	secondsLeft = ( (elapsedTime % 120) % 60 ) / 10;
	seconds = elapsedTime % 10;
	// If stage 1 is complete draw timer
	if (stage1Complete) {
		// Draw Minutes on screen
		switch (minutes) {
			case 1: g_pTimerNumber1->Draw(Vector3(450, y - 140, 550));
				break;
			case 2:	g_pTimerNumber2->Draw(Vector3(450, y - 140, 550));
				break;
			case 3:	g_pTimerNumber3->Draw(Vector3(450, y - 140, 550));
				break;
			case 4:	g_pTimerNumber4->Draw(Vector3(450, y - 140, 550));
				break;
			case 5:	g_pTimerNumber5->Draw(Vector3(450, y - 140, 550));
				break;
			case 6:	g_pTimerNumber6->Draw(Vector3(450, y - 140, 550));
				break;
			case 7:	g_pTimerNumber7->Draw(Vector3(450, y - 140, 550));
				break;
			case 8:	g_pTimerNumber8->Draw(Vector3(450, y - 140, 550));
				break;
			case 9:	g_pTimerNumber9->Draw(Vector3(450, y - 140, 550));
				break;
			default:
				g_pTimerNumber0->Draw(Vector3(450, y - 140, 550));
				break;
		}
		// Draw Seconds Left on screen
		switch (secondsLeft) {
			case 1: g_pTimerNumber1->Draw(Vector3(520, y - 140, 550));
				break;
			case 2: g_pTimerNumber2->Draw(Vector3(520, y - 140, 550));
				break;
			case 3: g_pTimerNumber3->Draw(Vector3(520, y - 140, 550));
				break;
			case 4: g_pTimerNumber4->Draw(Vector3(520, y - 140, 550));
				break;
			case 5: g_pTimerNumber5->Draw(Vector3(520, y - 140, 550));
				break;
			case 6: g_pTimerNumber6->Draw(Vector3(520, y - 140, 550));
				break;
			case 7: g_pTimerNumber7->Draw(Vector3(520, y - 140, 550));
				break;
			case 8: g_pTimerNumber8->Draw(Vector3(520, y - 140, 550));
				break;
			case 9: g_pTimerNumber9->Draw(Vector3(520, y - 140, 550));
				break;
			default: g_pTimerNumber0->Draw(Vector3(520, y - 140, 550));
				break;
		}
		// Draw Seconds Right on screen
		switch (seconds) {
			case 1: g_pTimerNumber1->Draw(Vector3(570, y - 140, 550));
				break;
			case 2: g_pTimerNumber2->Draw(Vector3(570, y - 140, 550));
				break;
			case 3: g_pTimerNumber3->Draw(Vector3(570, y - 140, 550));
				break;
			case 4: g_pTimerNumber4->Draw(Vector3(570, y - 140, 550));
				break;
			case 5: g_pTimerNumber5->Draw(Vector3(570, y - 140, 550));
				break;
			case 6: g_pTimerNumber6->Draw(Vector3(570, y - 140, 550));
				break;
			case 7: g_pTimerNumber7->Draw(Vector3(570, y - 140, 550));
				break;
			case 8: g_pTimerNumber8->Draw(Vector3(570, y - 140, 550));
				break;
			case 9: g_pTimerNumber9->Draw(Vector3(570, y - 140, 550));
				break;
			default: g_pTimerNumber0->Draw(Vector3(570, y - 140, 550));
				break;
		}
	}
	started = true;
}

void DrawStrikes(){
	float y = g_nScreenHeight - 410;

	strikes = mainController.getStrikes();
	if (strikes >= 0) {
		if (strikes < 3)
			g_pStrike1->Draw(Vector3(900, 90, 449));
		if(strikes < 2)
			g_pStrike2->Draw(Vector3(850, 90, 449));
		if(strikes < 1)
			g_pStrike3->Draw(Vector3(800, 90, 449));
	}
	if (strikes >= 3) {
		g_failScreen = new C3DSprite();
		if (!g_failScreen->Load(g_cImageFileName[22]))
			ABORT("Platform image %s not found.", g_cImageFileName[22]);
		fail = true;
	}
	if (fail == true) {
		g_failScreen->Draw(Vector3(515, y, 445));

		g_pStrike1->Release();
		g_pStrike2->Release();
		g_pStrike3->Release();
	}
}

void drawShapeScreen() {
	float y = g_nScreenHeight - 410;

	if (counterShape >= 1)
		g_shapeClueSprite1->Draw(Vector3(502, y + 20, 500));
	if (counterShape >= 2)
		g_shapeClueSprite2->Draw(Vector3(562, y + 20, 500));
	if (counterShape >= 3)
		g_shapeClueSprite3->Draw(Vector3(622, y + 20, 500));
	if (counterShape >= 4)
		g_shapeClueSprite4->Draw(Vector3(682, y + 20, 500));
}

void loadShapeScreen(int shapeIndex) {
	if (shapeIndex == -1)
		return;

	int colorShifter = 0;
	counterShape++;
	shapeOnScreen = true;

	string indexColor = mainController.isShapeResult(shapeIndex, counterShape, clearShapes);
	if (indexColor == "Green") {
		colorShifter = 0;
		shapeResult++;
	}
	else if (indexColor == "Yellow")
		colorShifter = 1;
	else if (indexColor == "Red")
		colorShifter = 2;

	if (counterShape == 1) {
		g_shapeClueSprite1 = new C3DSprite();
		if (!g_shapeClueSprite1->Load(g_cImageFileName[shapeIndex + colorShifter]))
			ABORT("Platform image %s not found.", g_cImageFileName[shapeIndex + colorShifter]);
	}

	else if (counterShape == 2) {
		g_shapeClueSprite2 = new C3DSprite();
		if (!g_shapeClueSprite2->Load(g_cImageFileName[shapeIndex + colorShifter]))
			ABORT("Platform image %s not found.", g_cImageFileName[shapeIndex + colorShifter]);
	}

	else if (counterShape == 3) {
		g_shapeClueSprite3 = new C3DSprite();
		if (!g_shapeClueSprite3->Load(g_cImageFileName[shapeIndex + colorShifter]))
			ABORT("Platform image %s not found.", g_cImageFileName[shapeIndex + colorShifter]);
	}

	else if (counterShape == 4) {
		g_shapeClueSprite4 = new C3DSprite();
		if (!g_shapeClueSprite4->Load(g_cImageFileName[shapeIndex + colorShifter]))
			ABORT("Platform image %s not found.", g_cImageFileName[shapeIndex + colorShifter]);
	}
}


void drawMenuScreen() {
	float y = g_nScreenHeight - 410;

	g_mainMenu = new C3DSprite();
	if (!g_mainMenu->Load(g_cImageFileName[2]))
		ABORT("Platform image %s not found.", g_cImageFileName[2]);
	g_mainMenu->Draw(Vector3(515, y, 445));
}

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
	bool rotationStart = false;

	if (rotationStart) {
		g_pBriefcaseSprite->Release();
		g_pArrows->Release();
	}
	// Briefcase Rotations
	if (caseImage != -1) {
		g_pBriefcaseSprite = new C3DSprite();
		if (!g_pBriefcaseSprite->Load(g_cImageFileName[caseImage]))
			ABORT("Platform image %s not found.", g_cImageFileName[caseImage]);

		g_pArrows = new C3DSprite();
		if (!g_pArrows->Load(g_cImageFileName[caseImage + 37]))
			ABORT("Platform image %s not found.", g_cImageFileName[caseImage + 37]);
		rotationStart = true;
		CreateObjects();
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
			g_pArrows->Draw(Vector3(515, y - 270, 470));
		}
		else if (currLocation == 4) {
			g_pArrows->Draw(Vector3(515, y + 190, 400));
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
			g_pBriefcaseSprite->Release();
			g_pArrows->Release();
			g_pArrows0->Release();
			g_pArrows1->Release();
			g_pArrows2->Release();
			g_pArrows3->Release();
		}
		else if (fail == true)
			g_failScreen->Draw(Vector3(515, y, 445));
		else{
			g_pBriefcaseSprite->Draw(Vector3(515, y - 40, 450));
		}
	}
	else if (currLocation == 5) {
		g_StageTwo->Draw(Vector3(515, y + 50, 700));
		g_pLedLitSprite->Draw(Vector3(750, y + 50, 500));
		
	}
	else {
		g_pBriefcaseSprite->Draw(Vector3(515, y, 450));
		if (currLocation == 0) {
			g_pArrows0->Draw(Vector3(520, y - 255, 470));	// Bottom
			g_pArrows1->Draw(Vector3(120, y, 449));			// Left 
			g_pArrows2->Draw(Vector3(904, y, 449));			// Right
			g_pArrows3->Draw(Vector3(520, y + 210, 400));	// Top
			
		}
		else if (currLocation == 2)
			g_pArrows->Draw(Vector3(774, y, 449));
		else if (currLocation == 3)
			g_pArrows->Draw(Vector3(250, y, 449));
	}


	g_numberBarcodeSprite2->Draw(Vector3(210, 698, 556));
	g_numberBarcodeSprite3->Draw(Vector3(220, 698, 556));
	if (currLocation == 4) {
		g_numberBarcodeSprite0->Draw(Vector3(790, y - 54
			, 445));
		g_numberBarcodeSprite1->Draw(Vector3(800, y - 54, 445));
	}
	else {
		g_numberBarcodeSprite0->Draw(Vector3(780, y - 30, 6000));
		g_numberBarcodeSprite1->Draw(Vector3(781, y - 30, 6000));
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
	g_pTopClues = new CGameObject(
		Vector3((float)g_nScreenWidth / 2.0f, 718, 580),
		Vector3(0, 0, 0), g_pTopCluesBannerSprite);

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
		g_pTopClues->jump();
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
		int x = xPos;
		int y = yPos;

		// Menu Screen
		if (menu_Screen && x >= 253 && x <= 767 && y >= 382 && y <= 497) {
			menu_Screen = false;
			g_cTimer.start(); //start game timer
		}
		// Stage One rotation mouse methods briefcase
		if (!stage1Complete && !menu_Screen)
			briefcaseRotation(mainController.caseRotation(xPos, yPos, currLocation));
		// Stage One lock number controller mouse methods
		if (currLocation == 1 && !stage1Complete)
			lockNumCtrl(lockNum, mainController.lockNumberCtrl(xPos, yPos, currLocation, lockNum));
		// Check Stage one win
		if (xPos >= 555 && xPos <= 600 && yPos <= 450 && yPos >= 380 && currLocation == 1 && !stage1Complete)
			checkStageOne(mainController.checkStage1());
		// Check Stage one win
		if (xPos >= 224 && xPos <= 392 && yPos <= 463 && yPos >= 350 && stage1Complete && counterShape <= 4) {
			loadShapeScreen(mainController.shapeScreen(x, y));
		}
		// Clear Shape Screen
		if (xPos >= 505 && xPos <= 586 && yPos <= 467 && yPos >= 441 && stage1Complete && shapeOnScreen && !shapesPuzzleSolved) {
			counterShape = 0;
			g_shapeClueSprite1->Release();
			g_shapeClueSprite2->Release();
			g_shapeClueSprite3->Release();
			g_shapeClueSprite4->Release();
			clearShapes = true;
			shapeResult = 0;
		}
		// Submit Shape Screen
		if (xPos >= 606 && xPos <= 690 && yPos <= 467 && yPos >= 441 && stage1Complete && shapeOnScreen && !shapesPuzzleSolved)
			checkStage2("CheckShape");
	}
	break;
	case WM_DESTROY: //on exit
		GameRenderer.Release(); //release textures
		delete g_pTopClues; //delete the plane object
		delete g_pTopCluesBannerSprite; //delete the plane sprite
		delete g_pBriefcaseSprite;
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
	InitXMLSettings(); //initialize XML settings reader
	LoadGameSettings();

	//create fullscreen window
	hwnd = CreateDefaultWindow(g_szGameName, hInst, nShow);
	if (!hwnd)return FALSE; //bail if problem creating window
	g_HwndApp = hwnd; //save window handle

	InitGraphics(); //initialize graphics
	g_pTopCluesBannerSprite = new C3DSprite(); //make a sprite
	g_pBarcodeSprite = new C3DSprite();
	g_pBarcodeCaseSprite = new C3DSprite();
	GameRenderer.LoadTextures(); //load images

	if (!g_pTopCluesBannerSprite->Load(g_cImageFileName[3])) // Top Clues Banner
		ABORT("Plane image %s not found.", g_cImageFileName[3]);

	if (!g_pBarcodeSprite->Load(g_cImageFileName[19]))
		ABORT("Platform image %s not found.", g_cImageFileName[19]);

	if (!g_pBarcodeCaseSprite->Load(g_cImageFileName[20]))
		ABORT("Platform image %s not found.", g_cImageFileName[20]);

	CreateObjects(); //create game objects

	g_pBriefcaseSprite = new C3DSprite();				// Briefcase 
	if (!g_pBriefcaseSprite->Load(g_cImageFileName[4]))
		ABORT("Platform image %s not found.", g_cImageFileName[4]);
	g_pArrows = new C3DSprite();
	if (!g_pArrows->Load(g_cImageFileName[41]))
		ABORT("Platform image %s not found.", g_cImageFileName[41]);

	g_pArrows0 = new C3DSprite();				// Arrow Down
	if (!g_pArrows0->Load(g_cImageFileName[42]))
		ABORT("Platform image %s not found.", g_cImageFileName[42]);
	g_pArrows1 = new C3DSprite();				// Arrow Left
	if (!g_pArrows1->Load(g_cImageFileName[43]))
		ABORT("Platform image %s not found.", g_cImageFileName[43]);
	g_pArrows2 = new C3DSprite();				// Arrow Right
	if (!g_pArrows2->Load(g_cImageFileName[44]))
		ABORT("Platform image %s not found.", g_cImageFileName[44]);
	g_pArrows3 = new C3DSprite();				// Arrow up 
	if (!g_pArrows3->Load(g_cImageFileName[45]))
		ABORT("Platform image %s not found.", g_cImageFileName[45]);

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

	if (!stage1Complete) {
		int arr[5];
		int spacer0, spacer1, spacer2, spacer3;
		spacer0 = spacer1 = spacer2 = spacer3 = 0;
		mainController.getBarcodeArr(arr);

		if (arr[4] == 0)
			spacer0 = 10;
		if (arr[1] == 0)
			spacer1 = 10;
		if (arr[2] == 0)
			spacer2 = 10;
		if (arr[3] == 0)
			spacer3 = 10;

		g_numberBarcodeSprite0 = new C3DSprite();	//digit
		if (!g_numberBarcodeSprite0->Load(g_cImageFileName[arr[4] + 60 + spacer0]))
			ABORT("Platform image %s not found.", g_cImageFileName[arr[4] + 60 + spacer0]);
		g_numberBarcodeSprite1 = new C3DSprite();	//digit
		if (!g_numberBarcodeSprite1->Load(g_cImageFileName[arr[1] + 60 + spacer1]))
			ABORT("Platform image %s not found.", g_cImageFileName[arr[1] + 60 + spacer1]);
		g_numberBarcodeSprite2 = new C3DSprite();	//digit
		if (!g_numberBarcodeSprite2->Load(g_cImageFileName[arr[2] + 60 + spacer2]))
			ABORT("Platform image %s not found.", g_cImageFileName[arr[2] + 60 + spacer2]);
		g_numberBarcodeSprite3 = new C3DSprite();	//digit
		if (!g_numberBarcodeSprite3->Load(g_cImageFileName[arr[3] + 60 + spacer3]))
			ABORT("Platform image %s not found.", g_cImageFileName[arr[3] + 60 + spacer3]);
	}
	g_pLedLitSprite = new C3DSprite();
	if (!g_pLedLitSprite->Load(g_cImageFileName[46]))
		ABORT("Platform image %s not found.", g_cImageFileName[46]);


	g_pStrike1 = new C3DSprite();
	if (!g_pStrike1->Load(g_cImageFileName[48]))
		ABORT("Platform image %s not found.", g_cImageFileName[48]);
	g_pStrike2 = new C3DSprite();
	if (!g_pStrike2->Load(g_cImageFileName[49]))
		ABORT("Platform image %s not found.", g_cImageFileName[49]);
	g_pStrike3 = new C3DSprite();
	if (!g_pStrike3->Load(g_cImageFileName[50]))
		ABORT("Platform image %s not found.", g_cImageFileName[50]);

	g_pTimerNumber0 = new C3DSprite();
	if (!g_pTimerNumber0->Load(g_cImageFileName[60]))
		ABORT("Platform image %s not found.", g_cImageFileName[60]);
	g_pTimerNumber1 = new C3DSprite();
	if (!g_pTimerNumber1->Load(g_cImageFileName[51]))
		ABORT("Platform image %s not found.", g_cImageFileName[51]);
	g_pTimerNumber2 = new C3DSprite();
	if (!g_pTimerNumber2->Load(g_cImageFileName[52]))
		ABORT("Platform image %s not found.", g_cImageFileName[52]);
	g_pTimerNumber3 = new C3DSprite();
	if (!g_pTimerNumber3->Load(g_cImageFileName[53]))
		ABORT("Platform image %s not found.", g_cImageFileName[53]);
	g_pTimerNumber4 = new C3DSprite();
	if (!g_pTimerNumber4->Load(g_cImageFileName[54]))
		ABORT("Platform image %s not found.", g_cImageFileName[54]);
	g_pTimerNumber5 = new C3DSprite();
	if (!g_pTimerNumber5->Load(g_cImageFileName[55]))
		ABORT("Platform image %s not found.", g_cImageFileName[55]);
	g_pTimerNumber6 = new C3DSprite();
	if (!g_pTimerNumber6->Load(g_cImageFileName[56]))
		ABORT("Platform image %s not found.", g_cImageFileName[56]);
	g_pTimerNumber7 = new C3DSprite();
	if (!g_pTimerNumber7->Load(g_cImageFileName[57]))
		ABORT("Platform image %s not found.", g_cImageFileName[57]);
	g_pTimerNumber8 = new C3DSprite();
	if (!g_pTimerNumber8->Load(g_cImageFileName[58]))
		ABORT("Platform image %s not found.", g_cImageFileName[58]);
	g_pTimerNumber9 = new C3DSprite();
	if (!g_pTimerNumber9->Load(g_cImageFileName[59]))
		ABORT("Platform image %s not found.", g_cImageFileName[59]);

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
