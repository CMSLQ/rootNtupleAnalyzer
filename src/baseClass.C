#define baseClass_cxx
#include "baseClass.h"

baseClass::baseClass(string * inputList, string * cutFile, string * treeName, TString * outputFileName, string * cutEfficFile)
{
  std::cout << "baseClass::baseClass(): begins " << std::endl;
  inputList_ = inputList;
  cutFile_ = cutFile;
  treeName_= treeName;
  outputFileName_ = outputFileName;
  cutEfficFile_ = cutEfficFile;
  init();
  std::cout << "baseClass::baseClass(): ends " << std::endl;
}

baseClass::~baseClass()
{
  std::cout << "baseClass::~baseClass(): begin " << std::endl;
  if( !writeCutHistos() )
    {
      STDOUT("ERROR: writeCutHistos did not complete successfully.");
    }
  output_root_->Close();
  if( !writeCutEfficFile() )
    {
      STDOUT("ERROR: writeStatFile did not complete successfully.");
    }
  std::cout << "baseClass::~baseClass(): end " << std::endl;
}

void baseClass::init()
{
  STDOUT("begin")
    //std::cout << "baseClass::init(): begin " << std::endl;

  tree_ = NULL;
  readInputList();
  readCutFile();
  if(tree_ == NULL){
    std::cout << "baseClass::init(): ERROR: tree_ = NULL " << std::endl;
    return;
  }
  Init(tree_);

  char output_root_title[200];
  sprintf(output_root_title,"%s%s",&std::string(*outputFileName_)[0],".root");
  output_root_ = new TFile(&output_root_title[0],"RECREATE");

  //  for (map<string, cut>::iterator it = cutName_cut_.begin(); 
  //   it != cutName_cut_.end(); it++) STDOUT("cutName_cut->first = "<<it->first)
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) STDOUT("orderedCutNames_ = "<<*it)

  std::cout << "baseClass::init(): end " << std::endl;
}

void baseClass::readInputList()
{

  TChain *chain = new TChain(treeName_->c_str());
  char pName[500];

  std::cout << "baseClass::readinputList(): inputList_ =  "<< *inputList_ << std::endl;

  ifstream is(inputList_->c_str());
  if(is.good())
    {
      cout << "baseClass::readInputList: Reading list: " << *inputList_ << " ......." << endl;
      while( is.getline(pName, 500, '\n') )
        {
          if (pName[0] == '#') continue;
          cout << "baseClass::readInputList: Adding file: " << pName << endl;
          chain->Add(pName);
        }
      tree_ = chain;
      cout << "baseClass::readInputList: Finished reading list: " << *inputList_ << endl;
    }
  else
    {
      cout << "baseClass::readInputList: ERROR opening inputList:" << *inputList_ << endl;
      exit (1);
    }
  is.close();

}

void baseClass::readCutFile()
{
  string s;
  STDOUT("Reading cutFile_ = "<< *cutFile_)

  ifstream is(cutFile_->c_str());
  if(is.good())
    {
      STDOUT("Reading file: " << *cutFile_ );
      int id=0;
      while( getline(is,s) )
        {
          STDOUT("read line: " << s);
          if (s[0] == '#') continue;
	  cut thisCut;
	  vector<string> v = split(s);
	  // for(vector<string>::size_type i = 0; i != v.size(); ++i){ STDOUT("i, v[i] ="<<i<<", "<<v[i]); }
	  thisCut.variableName =     v[0];
	  thisCut.minValue1  = atof( v[1].c_str() );
	  thisCut.maxValue1  = atof( v[2].c_str() );
	  thisCut.minValue2  = atof( v[3].c_str() );
	  thisCut.maxValue2  = atof( v[4].c_str() );
	  thisCut.level_int  = atoi( v[5].c_str() );
	  thisCut.level_str  =       v[5];
	  thisCut.histoNbins = atoi( v[6].c_str() );
	  thisCut.histoMin   = atof( v[7].c_str() );
	  thisCut.histoMax   = atof( v[8].c_str() );
	  // Not filled from file
	  thisCut.id=++id;
	  string s1 = "cutHisto_noCuts___________" + thisCut.variableName;
	  string s2 = "cutHisto_allPreviousCuts__" + thisCut.variableName;
	  string s3 = "cutHisto_allOthrSmLvlCuts_" + thisCut.variableName;
	  string s4 = "cutHisto_allOtherCuts_____" + thisCut.variableName;
	  string s5 = "cutHisto_allCuts__________" + thisCut.variableName;
	  thisCut.histo1 = TH1F (s1.c_str(),"", thisCut.histoNbins, thisCut.histoMin, thisCut.histoMax);
	  thisCut.histo2 = TH1F (s2.c_str(),"", thisCut.histoNbins, thisCut.histoMin, thisCut.histoMax);
	  thisCut.histo3 = TH1F (s3.c_str(),"", thisCut.histoNbins, thisCut.histoMin, thisCut.histoMax);
	  thisCut.histo4 = TH1F (s4.c_str(),"", thisCut.histoNbins, thisCut.histoMin, thisCut.histoMax);
	  thisCut.histo5 = TH1F (s5.c_str(),"", thisCut.histoNbins, thisCut.histoMin, thisCut.histoMax);
	  thisCut.histo1.Sumw2();
	  thisCut.histo2.Sumw2();
	  thisCut.histo3.Sumw2();
	  thisCut.histo4.Sumw2();
	  thisCut.histo5.Sumw2();
	  // Filled event by event
	  thisCut.filled = false;
	  thisCut.value = 0;
	  thisCut.passed = false;
	  thisCut.nEvtInput=0;
	  thisCut.nEvtPassed=0;
	  thisCut.effRel=0;
	  thisCut.effRelErr=0;
	  thisCut.effAbs=0;
	  thisCut.effAbsErr=0;

	  orderedCutNames_.push_back(thisCut.variableName);
	  cutName_cut_[thisCut.variableName]=thisCut;
        }
      cout << "baseClass::readCutFile: Finished reading cutFile: " << *cutFile_ << endl;
    }
  else
    {
      cout << "baseClass::readCutFile: ERROR opening cutFile:" << *cutFile_ << endl;
      exit (1);
    }
  is.close();

}

bool baseClass::resetCuts()
{
  bool ret=true;
  for (map<string, cut>::iterator cc = cutName_cut_.begin(); cc != cutName_cut_.end(); cc++) 
    {
      cut * c = & (cc->second);
      c->filled = false;
      c->value = 0;
      c->passed = false;
    }
  combCutName_passed_.clear();
  return ret;
}

bool baseClass::fillVariableWithValue(const string& s, const double& d)
{
  bool ret = true;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() )
    {
      STDOUT("variableName = "<< s << "not found in cutName_cut_.");
      return false;
    } 
  else
    {
      cut * c = & (cc->second);
      c->filled = true;
      c->value = d;
    }
  return ret;
}

bool baseClass::evaluateCuts()
{
  bool ret = true;
  //  resetCuts();
  combCutName_passed_.clear();
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      if( ! ( c->filled && (c->minValue1 < c->value && c->value <= c->maxValue1 || c->minValue2 < c->value && c->value <= c->maxValue2) ) )
	{
          c->passed = false;
          combCutName_passed_[c->level_str.c_str()] = false;
          combCutName_passed_["all"] = false;
	}
      else
	{
	  c->passed = true;
	  map<string,bool>::iterator cp = combCutName_passed_.find( c->level_str.c_str() );
	  combCutName_passed_[c->level_str.c_str()] = (cp==combCutName_passed_.end()?true:cp->second);
	  map<string,bool>::iterator ap = combCutName_passed_.find( "all" );
	  combCutName_passed_["all"] = (ap==combCutName_passed_.end()?true:ap->second);
	}
    }
  //for(map<string, bool>::iterator cp=combCutName_passed_.begin(); cp !=combCutName_passed_.end(); cp++) STDOUT("combCutName_passed_ = "<<cp->first<<", "<<cp->second);

//   STDOUT("-------");
//   for (vector<string>::iterator it = orderedCutNames_.begin(); 
//        it != orderedCutNames_.end(); it++) 
//     {
//       cut * c = & (cutName_cut_.find(*it)->second);
//       STDOUT("variableName filled value passed = "<<c->variableName<<" "<<c->filled<<" "<<c->value<<" "<<c->passed);
//       STDOUT("passedAllPreviousCuts = "<<passedAllPreviousCuts(c->variableName));
//       STDOUT("passedCut = "<<passedCut(c->variableName));
//     }



  if( !fillCutHistos() )
    {
      STDOUT("ERROR: fillCutHistos did not complete successfully.");
      ret = false;
    }

  if( !updateCutEffic() )
    {
      STDOUT("ERROR: updateCutEffic did not complete successfully.");
      ret = false;
    }

  return ret;
}

bool baseClass::passedCut(const string& s)
{
  bool ret = false;
  //  map<string, bool>::iterator vp = cutName_passed_.find(s);
  //  if( vp != cutName_passed_.end() ) return ret = vp->second;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc != cutName_cut_.end() ) 
    {
      cut * c = & (cutName_cut_.find(s)->second);
      return (c->filled && c->passed);
    }
  map<string, bool>::iterator cp = combCutName_passed_.find(s);
  if( cp != combCutName_passed_.end() ) 
    {
      return ret = cp->second;
    }
  STDOUT("ERROR: did not find variableName = "<<s<<" neither in cutName_cut_ nor combCutName_passed_. Returning false.");
  return (ret=false);
}

bool baseClass::passedAllPreviousCuts(const string& s)
{
  //STDOUT("Examining variableName = "<<s);

  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {  
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning false.");
      return false;
    }

  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      if( c->variableName == s ) 
	{
	  return true;
	}
      else
	{
	  if( ! (c->filled && c->passed) ) return false;
	}
    }
  STDOUT("ERROR. It should never pass from here.");
}

bool baseClass::passedAllOtherCuts(const string& s)
{
  //STDOUT("Examining variableName = "<<s);
  bool ret = true;

  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {  
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning false.");
      return false;
    }

  for (map<string, cut>::iterator cc = cutName_cut_.begin(); cc != cutName_cut_.end(); cc++) 
    {
      cut * c = & (cc->second);
      if( c->variableName == s ) 
	{
	  continue;
	}
      else
	{
	  if( ! (c->filled && c->passed) ) return false;
	}
    }
  return ret;
}

bool baseClass::passedAllOtherSameLevelCuts(const string& s)
{
  //STDOUT("Examining variableName = "<<s);
  bool ret = true;
  int cutLevel;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {  
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning false.");
      return false;
    }
  else
    {
      cutLevel = cc->second.level_int; 
    }

  for (map<string, cut>::iterator cc = cutName_cut_.begin(); cc != cutName_cut_.end(); cc++) 
    {
      cut * c = & (cc->second);
      if( c->level_int > cutLevel || c->variableName == s ) 
	{
	  continue;
	}
      else 
	{
	  if( ! (c->filled && c->passed) ) return false;
	}
    }
  return ret;
}

bool baseClass::fillCutHistos()
{
  bool ret = true;
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      if( c->filled )
	{
	  c->histo1.Fill( c->value );
	  if( passedAllPreviousCuts(c->variableName) )        c->histo2.Fill( c->value );
	  if( passedAllOtherSameLevelCuts(c->variableName) )  c->histo3.Fill( c->value );
	  if( passedAllOtherCuts(c->variableName) )           c->histo4.Fill( c->value );
	  if( passedCut("all") )                              c->histo5.Fill( c->value );
	}
    }
  return ret;
}

bool baseClass::writeCutHistos()
{
  bool ret = true;
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      c->histo1.Write();
      c->histo2.Write();
      c->histo3.Write();
      c->histo4.Write();
      c->histo5.Write();
    }
  // Any failure mode to implement?
  return ret;
}

bool baseClass::updateCutEffic()
{
  bool ret = true;
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      if( passedAllPreviousCuts(c->variableName) )  
	{
	  c->nEvtInput++;
	  if( passedCut(c->variableName) )              c->nEvtPassed++;
	}
    }
  return ret;
}

bool baseClass::writeCutEfficFile()
{
  bool ret = true;
  int nEnt = fChain->GetEntriesFast();
  string cutEfficFile = *cutEfficFile_ + ".dat";
  ofstream os(cutEfficFile.c_str());
  os <<"idx           name           min1           max1           min2           max2              N          Npass         EffRel      errEffRel         EffAbs      errEffAbs"<<endl;

  os << fixed; 
  os.precision(4); 

  os << setw(3) << "0"
     << setw(15) << "nocut" 
     << setw(15) << "-9999999"
     << setw(15) <<  "9999999"
     << setw(15) << "-9999999"
     << setw(15) <<  "9999999"
     << setw(15) << nEnt
     << setw(15) << nEnt
     << setw(15) << "100"
     << setw(15) << "0"
     << setw(15) << "100"
     << setw(15) << "0"
     << endl;
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      c->effRel = (double) c->nEvtPassed / (double) c->nEvtInput;
      c->effRelErr = sqrt( (double) c->effRel * (1.0 - (double) c->effRel) / (double) c->nEvtInput );
      c->effAbs = (double) c->nEvtPassed / (double) nEnt;
      c->effAbsErr = sqrt( (double) c->effAbs * (1.0 - (double) c->effAbs) / (double) nEnt );

      os << setw(3) << c->id 
	 << setw(15) << c->variableName 
	 << setw(15) << fixed << c->minValue1
	 << setw(15) << c->maxValue1
	 << setw(15) << c->minValue2
	 << setw(15) << c->maxValue2
	 << setw(15) << c->nEvtInput
	 << setw(15) << c->nEvtPassed
	 << setw(15) << c->effRel
	 << setw(15) << c->effRelErr
	 << setw(15) << c->effAbs
	 << setw(15) << c->effAbsErr
	 << endl;
    }
  // Any failure mode to implement?
  return ret;
}

bool baseClass::sortCuts(const cut& X, const cut& Y)
{
  return X.id < Y.id;
}


vector<string> baseClass::split(const string& s)
{
  vector<string> ret;
  string::size_type i =0;
  while (i != s.size()){
    while (i != s.size() && isspace(s[i]))
      ++i;
    string::size_type j = i;
    while (j != s.size() && !isspace(s[j]))
      ++j;
    if (i != j){
      ret.push_back(s.substr(i, j -i));
      i = j;
    }
  }
  return ret;
}

