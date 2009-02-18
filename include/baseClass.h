#ifndef baseClass_h
#define baseClass_h

#include "rootNtupleClass.h"
#include <iostream>
#include <sstream>
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
  int level_int;
  string level_str;
  int histoNBins;
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
};

struct preCut {
  string variableName;
  double value1;
  double value2;
  double value3;
  double value4;
  int level_int;
  string level_str;
};

class baseClass : public rootNtupleClass {
  public :
  map<string, bool> combCutName_passed_;

  void baseClass::resetCuts();
  void fillVariableWithValue(const std::string&, const double&);
  void baseClass::evaluateCuts();
  bool baseClass::passedCut(const string& s);
  bool baseClass::passedAllPreviousCuts(const string& s);
  bool baseClass::passedAllOtherCuts(const string& s);
  bool baseClass::passedAllOtherSameAndLowerLevelCuts(const string& s);
  double baseClass::getPreCutValue1(const string& s);
  double baseClass::getPreCutValue2(const string& s);
  double baseClass::getPreCutValue3(const string& s);
  double baseClass::getPreCutValue4(const string& s);
  double baseClass::getCutMinValue1(const string& s);
  double baseClass::getCutMaxValue1(const string& s);
  double baseClass::getCutMinValue2(const string& s);
  double baseClass::getCutMaxValue2(const string& s);

  const TH1F& baseClass::getHisto_noCuts_or_skim(const string& s);
  const TH1F& baseClass::getHisto_allPreviousCuts(const string& s);
  const TH1F& baseClass::getHisto_allOthrSmAndLwrLvlCuts(const string& s);
  const TH1F& baseClass::getHisto_allOtherCuts(const string& s);
  const TH1F& baseClass::getHisto_allCuts(const string& s);

  int    baseClass::getHistoNBins(const string& s);
  double baseClass::getHistoMin(const string& s);
  double baseClass::getHistoMax(const string& s);


  baseClass(string * inputList, string * cutFile, string * treeName, TString *outputFileName=0, string * cutEfficFile=0);
  virtual ~baseClass();

  private :
  string * configFile_;
  TString * outputFileName_; 
  TFile * output_root_;
  string * inputList_;
  string * cutFile_;
  string * treeName_; // Name of input tree objects in (.root) files
  TTree * tree_; // main tree
  TTree * tree2_; // tree for globalInfo
  string * cutEfficFile_;
  std::stringstream preCutInfo_;
  map<string, preCut> preCutName_cut_;
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
  double decodeCutValue(const string& s);
  bool skimWasMade_;
  int getGlobalInfoNstart( char* );
  int NBeforeSkim_;
};

#endif

#ifdef baseClass_cxx

#endif // #ifdef baseClass_cxx
