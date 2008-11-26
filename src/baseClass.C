#define baseClass_cxx
#include "baseClass.h"

baseClass::baseClass(string * inputList, string * treeName, TString * outputFileName)
{
  std::cout << "baseClass::baseClass(): begins " << std::endl;
  inputList_ = inputList;
  treeName_= treeName;
  outputFileName_ = outputFileName;
  init();
  std::cout << "baseClass::baseClass(): ends " << std::endl;
}

baseClass::~baseClass()
{
  std::cout << "baseClass::~baseClass(): begin " << std::endl;
  output_root_->Close();
  std::cout << "baseClass::~baseClass(): end " << std::endl;
}

void baseClass::init()
{
  std::cout << "baseClass::init(): begin " << std::endl;

  tree_ = NULL;
  readInputList();
  if(tree_ == NULL){
    std::cout << "baseClass::init(): ERROR: tree_ = NULL " << std::endl;
    return;
  }
  Init(tree_);

  char output_root_title[200];
  sprintf(output_root_title,"%s%s",&std::string(*outputFileName_)[0],".root");
  output_root_ = new TFile(&output_root_title[0],"RECREATE");

  std::cout << "baseClass::init(): end " << std::endl;
}

void baseClass::readInputList()
{

  TChain *chain = new TChain(treeName_->c_str());
  char pName[500];

  std::cout << "baseClass::readinputList(): inputList_ =  "<< inputList_ << std::endl;

  ifstream is(inputList_->c_str());
  if(is.good())
    {
      cout << "baseClass::readInputList: Reading list: " << inputList_ << " ......." << endl;
      while( is.getline(pName, 500, '\n') )
        {
          if (pName[0] == '#') continue;
          cout << "baseClass::readInputList: Adding file: " << pName << endl;
          chain->Add(pName);
        }
      tree_ = chain;
      cout << "baseClass::readInputList: Finished reading list: " << inputList_ << endl;
    }
  else
    {
      cout << "baseClass::readInputList: ERROR opening inputList:" << inputList_ << endl;
      exit (1);
    }
  is.close();

}


