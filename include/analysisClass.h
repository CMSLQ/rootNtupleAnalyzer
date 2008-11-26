#ifndef analysisClass_h
#define analysisClass_h

#include "baseClass.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iomanip>


using namespace std;

class analysisClass : public baseClass {
public :
  analysisClass(string * inputList, string * treeName,  TString *outputFileName=0 );
  virtual ~analysisClass();
  void Loop();
};

#endif

#ifdef analysisClass_cxx

#endif // #ifdef analysisClass_cxx
