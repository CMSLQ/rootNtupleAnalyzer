#ifndef baseClass_h
#define baseClass_h

#include "rootNtupleClass.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <iomanip>


using namespace std;

class baseClass : public rootNtupleClass {
public :
  TString * outputFileName_; 
  TFile * output_root_;
  string * inputList_;
  string * treeName_; // Name of input tree objects in (.root) files
  TTree * tree_;
  baseClass(string * inputList, string * treeName, TString *outputFileName=0 );
  virtual ~baseClass();
  void baseClass::init();
  void baseClass::readInputList();
};

#endif

#ifdef baseClass_cxx

#endif // #ifdef baseClass_cxx
