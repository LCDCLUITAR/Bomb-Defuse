/// \file Window.cpp 
/// \brief Helper functions for creating a Windows window with graphics.
/// Windows stuff that won't change after Demo 3 is hidden away in this file
/// so you won't have to keep looking at it.

#include <windows.h>
#include <windowsx.h>

#include "gamerenderer.h"
#include "abort.h"

#include "..\resource.h" //for IDI_ICON1, the red plane icon

extern HWND g_HwndApp; 
extern HINSTANCE g_hInstance;
extern int g_nScreenWidth; 
extern int g_nScreenHeight; 

extern CGameRenderer GameRenderer; 

LRESULT CALLBACK WindowProc(HWND hwnd,UINT message, WPARAM wParam, LPARAM lParam);

/// \brief Initialize graphics.
///
/// Initialize the graphics using DirectX.
/// Draws a floor and a backdrop.

void InitGraphics(){ 
  if(!GameRenderer.InitD3D(g_hInstance, g_HwndApp))
    ABORT("Unable to initialize DirectX.");
  GameRenderer.InitBackground();
} //InitGraphics

/// \brief Create a default window.
///
/// Register and create a window. It takes some shenanigans
/// to make sure that the client area of the window is exactly g_nScreenWidth
/// by g_nScreenHeight since Windows usually includes the banner and border
/// when specifying window size. The window is centered on the desktop too.
/// \param name The name of this application.
/// \param hInstance Handle to the current instance of this application.
/// \param nCmdShow Specifies how the window is to be shown.
/// \return Handle to the application window.

HWND CreateDefaultWindow(char* name, HINSTANCE hInstance, int nCmdShow){
  WNDCLASS wc; //window registration info

  //fill in registration information wc
  wc.style = CS_HREDRAW | CS_VREDRAW; //style
  wc.lpfnWndProc = WindowProc; //window message handler
  wc.cbClsExtra = wc.cbWndExtra = 0;
  wc.hInstance = hInstance; //instance handle
  wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
  wc.hCursor = nullptr; //no cursor
  wc.hbrBackground = nullptr; //we will draw background
  wc.lpszMenuName = nullptr; //no menu
  wc.lpszClassName = name; //app name provided as parameter

  RegisterClass(&wc); //register window
 
  //create and set up window 
  HWND hwnd; //window handle
  
  //get resolution that user's monitor is currently set at
  int nDevScreenWidth = GetSystemMetrics(SM_CXSCREEN);
  int nDevScreenHeight = GetSystemMetrics(SM_CYSCREEN);

  RECT r; //desired window rectangle

  //initialize r to size of client area in window
  r.left = 0; r.right = g_nScreenWidth; 
  r.top = 0; r.bottom = g_nScreenHeight;

  //adjust r to include Windows border and stuff
  AdjustWindowRectEx(&r, WS_POPUP | WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW, 
    FALSE, WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME); 
  
  int w = r.right - r.left, h = r.bottom - r.top;

  //create window
  hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, name, name,
    WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0,
    w, h, nullptr, nullptr, hInstance, nullptr);
  
  //center window on screen
  int x = (nDevScreenWidth - g_nScreenWidth)/2;
  int y = (nDevScreenHeight - g_nScreenHeight)/2;
  ::SetWindowPos(hwnd, nullptr, x, y, w, h, SWP_NOZORDER | SWP_SHOWWINDOW);

  if(hwnd){ //if successfully created window
    ShowWindow(hwnd, nCmdShow); UpdateWindow(hwnd); //show and update
    SetFocus(hwnd); //get input from keyboard
  } //if

  return hwnd; //return window handle
} //CreateDefaultWindow