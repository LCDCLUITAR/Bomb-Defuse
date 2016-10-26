/// \file imagefilenamelist.h 
/// \brief Definition of the image file name class CImageFileNameList.

#pragma once

#include "defines.h"

/// \brief The image file name list. 
///
/// The image file name list stores a list of image file names, with the
/// capability to import them from an XML document. Also provides a safe index 
/// operation. If an attempt is made to access a file name at an invalid index, 
/// the string NotAValidFileName.bmp is returned instead.

class CImageFileNameList{
  private:
    char** m_lplpImageFileName; ///< Array of file name strings.
    int m_nImageFileCount; ///< Number of image file names stored.

  public:
    CImageFileNameList(); ///< Constructor.
    ~CImageFileNameList(); ///< Destructor.
    void GetImageFileNames(XMLElement* xmlSettings); ///< Get names from XML element.
    char* operator[](const int); ///< Safe index into name list.
}; //CImageFileNameList