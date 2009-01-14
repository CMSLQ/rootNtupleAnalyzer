#ifndef baseClass_h
#define baseClass_h

#include "rootNtupleClass.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>
#include <iomanip>
#include <TH1F.h>
#include <math.h>

#define STDOUT(STRING) {		   \
	std::cout << __FILE__ <<" - Line "<<__LINE__<<" - "<<__FUNCTION__<<": "<< STRING <<std::endl;   \
}

using namespace std;

struct cut {
  string variableName;
  double minValue1;
  double maxValue1;
  double minValue2;
  double maxValue2;
  //  double resetValue;
  int level_int;
  string level_str;
  int histoNbins;
  double histoMin;
  double histoMax;
  // Not filled from file
  int id;
  TH1F histo1;
  TH1F histo2;
  TH1F histo3;
  TH1F histo4;
  TH1F histo5;
  // Filled event by event
  bool filled;
  double value;
  bool passed;
  int nEvtInput;
  int nEvtPassed;
  double effRel;
  double effRelErr;
  double effAbs;
  double effAbsErr;
};

class baseClass : public rootNtupleClass {
  public :
  map<string, bool> combCutName_passed_;

  bool baseClass::resetCuts();
  bool fillVariableWithValue(const std::string&, const double&);
  bool baseClass::evaluateCuts();
  bool baseClass::passedCut(const string& s);
  bool baseClass::passedAllPreviousCuts(const string& s);
  bool baseClass::passedAllOtherCuts(const string& s);
  bool baseClass::passedAllOtherSameLevelCuts(const string& s);

  baseClass(string * inputList, string * cutFile, string * treeName, TString *outputFileName=0, string * cutEfficFile=0);
  virtual ~baseClass();

  private :
  string * configFile_;
  TString * outputFileName_; 
  TFile * output_root_;
  string * inputList_;
  string * cutFile_;
  string * treeName_; // Name of input tree objects in (.root) files
  TTree * tree_;
  string * cutEfficFile_;
  map<string, cut> cutName_cut_;
  vector<string> orderedCutNames_; 
  void baseClass::init();
  void baseClass::readInputList();
  void baseClass::readCutFile();
  bool baseClass::fillCutHistos();
  bool baseClass::writeCutHistos();
  bool baseClass::updateCutEffic();
  bool baseClass::writeCutEfficFile();
  bool baseClass::sortCuts(const cut&, const cut&);
  vector<string> split(const string& s);
};

#endif

#ifdef baseClass_cxx

#endif // #ifdef baseClass_cxx
