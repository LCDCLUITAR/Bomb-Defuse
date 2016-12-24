/// \file imagefilenamelist.cpp 
///
/// \brief Image file name class CImageFileNameList.
///
/// The image file name list reads the image file names
/// from an XML document and holds the names in an array.

#include "imagefilenamelist.h"
#include "debug.h"

CImageFileNameList::CImageFileNameList(void){ //constructor
  m_lplpImageFileName = nullptr;
  m_nImageFileCount = 0;
} //constructor

CImageFileNameList::~CImageFileNameList(void){ //destructor
  for (int i = 0; i < m_nImageFileCount; i++) //for each string
    delete[] m_lplpImageFileName[i]; //delete the string
  delete[] m_lplpImageFileName; //delete the array
} //destructor

/// The overloaded index operator, which behaves safely even when the
/// index is out of range.
/// \param ix Index into name list
/// \return Pointer to name string if valid index, pointer to "NotAValidFileName.bmp" otherwise

char* CImageFileNameList::operator[](const int ix){ //safe index into name list
  static char errname[] = "NotAValidFileName.bmp";
  if(ix >= 0 && ix < m_nImageFileCount) //if it's safe
    return m_lplpImageFileName[ix]; //return indexed string
  else return errname; //else return a default string
} //operator[]

/// Load image file names from tags in a TinyXML element.
/// \param settings TinyXML element containing settings tags

void CImageFileNameList::GetImageFileNames(XMLElement* settings){
  //image file names
  XMLElement* ist = settings->FirstChildElement("images");
  if(ist){
    XMLElement* img; //image element

    //count number of images
    m_nImageFileCount = 0;
    for (img = ist->FirstChildElement("image"); img; img = img->NextSiblingElement("image"))
      ++m_nImageFileCount;

    //create file name array
    m_lplpImageFileName = new char*[m_nImageFileCount];

    //get image file names
    int i = 0; //counter for number of names
    for (img = ist->FirstChildElement("image"); img; img = img->NextSiblingElement("image")){
      int len = (int)strlen(img->Attribute("src")); //length of name string
      m_lplpImageFileName[i] = new char[len + 1]; //create array space (+1 for nullptr)
      strncpy_s(m_lplpImageFileName[i], len + 1, img->Attribute("src"), len); //copy file name string
      m_lplpImageFileName[i][len] = '\0'; //nullptr at end of string
      i++; //next filename
    } //for
  } //if
} //GetImageFileNames