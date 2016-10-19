/// \file main.cpp
/// \brief Main file for Demo 0.

/// \mainpage Demo 0: The Black Screen of Death
///
/// This is our first Windows application, Hello World for Windows in a sense.
/// It fires up a black window with a text message on it and waits for user
/// to hit the ESC key to exit. You will learn how to register a window, 
/// create it, draw graphics on it using the Windows GDI, respond to user 
/// keyboard input, and shut down the application gracefully. There is no 
/// gameplay yet and no DirectX graphics, but hey, you've gotta 
/// learn to crawl before you can walk, right? You can change the resolution
///  of the window by changing g_nScreenWidth and g_nScreenHeight.
#include <iostream>
#include <windows.h>
#include <windowsx.h>

//globals
BOOL g_bActiveApp; ///< TRUE if this is the active application
HWND g_HwndApp; ///< Application window handle.
HINSTANCE g_hInstance; ///< Application instance handle.

//graphics settings
int g_nScreenWidth = 1024; ///< Screen width.
int g_nScreenHeight = 768; ///< Screen height.

//functions in Window.cpp
void InitGraphics();
HWND CreateDefaultWindow(char* name, HINSTANCE hInstance, int nCmdShow);

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

LRESULT CALLBACK WindowProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam){
  switch(message){ //handle message
    case WM_ACTIVATEAPP: g_bActiveApp = (BOOL)wParam; break; //iconize

    case WM_KEYDOWN: //keyboard hit
      if(KeyboardHandler(wParam))DestroyWindow(hwnd);
      break;

	case WM_DESTROY: {  //on exit
		/*
		--- Destroy window changed
		*/
		int response = MessageBox(nullptr, "Would you like to play again?",
			"Game Over", MB_YESNO | MB_ICONQUESTION);
		PostQuitMessage(0); //this is the last thing to do on exit
		break;
	}

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
  HWND hwnd; //handle to window

  g_hInstance = hInst; 

  //create default window
  hwnd = CreateDefaultWindow("Bomb Diffuse", hInst, nShow);
  if(!hwnd)return FALSE; //bail if problem creating window
  g_HwndApp = hwnd; //save window handle

  InitGraphics(); //initialize graphics

  //message loop
  while(TRUE)
    if(PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE)){ //if message waiting
      if(!GetMessage(&msg, nullptr, 0, 0))return (int)msg.wParam; //get it
      TranslateMessage(&msg); DispatchMessage(&msg); //translate it
    } //if
    else if(!g_bActiveApp)WaitMessage(); //else if not active, wait for msg
} //WinMain