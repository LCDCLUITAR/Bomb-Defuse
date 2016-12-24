/// \file abort.cpp
/// \brief Code for aborting when something catastrophic goes wrong.

#include "abort.h"

#include <stdio.h>
#include <windows.h>

/// This is the function that actually really does all of the
/// work of aborting, despite all of the redirections in
/// the code. Normally we will call this function using the ABORT macro.
/// This allows an error message to be displayed in a dialog box (provided things 
/// don't go TOO wrong) last thing on exit by calling the 
/// ABORT macro anyplace in the code. The parameters to this function
/// are identical to those of the stdio function printf.
/// \param fmt Printf style format string.

void reallyAbort(const char *fmt, ...){ 
  char buffer[1024];

  //format the error message into our buffer
  va_list ap;
  va_start(ap, fmt);
  _vsnprintf_s(buffer, sizeof(buffer)-1, fmt, ap);
  va_end(ap);
  //flag the error so the app exits cleanly
  FatalAppExit(0, buffer);
} //reallyAbort