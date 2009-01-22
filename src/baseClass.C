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
  STDOUT("begin");
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
  //  for (vector<string>::iterator it = orderedCutNames_.begin(); 
  //       it != orderedCutNames_.end(); it++) STDOUT("orderedCutNames_ = "<<*it)
  STDOUT("end");
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
      //      STDOUT("Reading file: " << *cutFile_ );
      int id=0;
      while( getline(is,s) )
        {
          STDOUT("read line: " << s);
          if (s[0] == '#') continue;
	  vector<string> v = split(s);
	  map<string, cut>::iterator cc = cutName_cut_.find(v[0]);
	  if( cc != cutName_cut_.end() )
	    {
	      STDOUT("ERROR: variableName = "<< v[0] << " exists already in cutName_cut_. Returning.");
	      return;
	    } 

	  int level_int = atoi( v[5].c_str() );
	  if(level_int == -1)
	    {
	      map<string, preCut>::iterator cc = preCutName_cut_.find(v[0]);
	      if( cc != preCutName_cut_.end() )
		{
		  STDOUT("ERROR: variableName = "<< v[0] << " exists already in preCutName_cut_. Returning.");
		  return;
		} 
	      preCutInfo_ << "### Preliminary cut values: " << s <<endl;
	      preCut thisPreCut;
	      thisPreCut.variableName =     v[0];
	      thisPreCut.value1  = decodeCutValue( v[1] );
	      thisPreCut.value2  = decodeCutValue( v[2] );
	      thisPreCut.value3  = decodeCutValue( v[3] );
	      thisPreCut.value4  = decodeCutValue( v[4] );
	      preCutName_cut_[thisPreCut.variableName]=thisPreCut;
	      continue;
	    }
	  cut thisCut;
	  thisCut.variableName =     v[0];
	  string m1=v[1];
	  string M1=v[2];
	  string m2=v[3];
	  string M2=v[4];
	  if( m1=="-" || M1=="-" ) 
	    {
	      STDOUT("ERROR: minValue1 and maxValue2 have to be provided. Returning."); 
	      return; // FIXME implement exception
	    } 
	  if( (m2=="-" && M2!="-") || (m2!="-" && M2=="-") ) 
	    {
	      STDOUT("ERROR: if any of minValue2 and maxValue2 is -, then both have to be -. Returning");
	      return; // FIXME implement exception
	    }
	  if( m2=="-") m2="+inf";
	  if( M2=="-") M2="-inf";
	  thisCut.minValue1  = decodeCutValue( m1 );
	  thisCut.maxValue1  = decodeCutValue( M1 );
	  thisCut.minValue2  = decodeCutValue( m2 );
	  thisCut.maxValue2  = decodeCutValue( M2 );
	  thisCut.level_int  = level_int;
	  thisCut.level_str  =       v[5];
	  thisCut.histoNbins = atoi( v[6].c_str() );
	  thisCut.histoMin   = atof( v[7].c_str() );
	  thisCut.histoMax   = atof( v[8].c_str() );
	  // Not filled from file
	  thisCut.id=++id;
	  string s1 = "cutHisto_noCuts_________________" + thisCut.variableName;
	  string s2 = "cutHisto_allPreviousCuts________" + thisCut.variableName;
	  string s3 = "cutHisto_allOthrSmAndLwrLvlCuts_" + thisCut.variableName;
	  string s4 = "cutHisto_allOtherCuts___________" + thisCut.variableName;
	  string s5 = "cutHisto_allCuts________________" + thisCut.variableName;
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

	  orderedCutNames_.push_back(thisCut.variableName);
	  cutName_cut_[thisCut.variableName]=thisCut;

	}
      STDOUT( "baseClass::readCutFile: Finished reading cutFile: " << *cutFile_ );
    }
  else
    {
      STDOUT("ERROR opening cutFile:" << *cutFile_ );
      exit (1);
    }
  is.close();

}

void baseClass::resetCuts()
{
  for (map<string, cut>::iterator cc = cutName_cut_.begin(); cc != cutName_cut_.end(); cc++) 
    {
      cut * c = & (cc->second);
      c->filled = false;
      c->value = 0;
      c->passed = false;
    }
  combCutName_passed_.clear();
  return;
}

void baseClass::fillVariableWithValue(const string& s, const double& d)
{
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() )
    {
      STDOUT("variableName = "<< s << "not found in cutName_cut_.");
      return;
    } 
  else
    {
      cut * c = & (cc->second);
      c->filled = true;
      c->value = d;
    }
  return;
}

void baseClass::evaluateCuts()
{
  //  resetCuts();
  combCutName_passed_.clear();
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      if( ! ( c->filled && (c->minValue1 < c->value && c->value <= c->maxValue1 || c->minValue2 < c->value && c->value <= c->maxValue2 ) ) )
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

  if( !fillCutHistos() )
    {
      STDOUT("ERROR: fillCutHistos did not complete successfully.");
    }

  if( !updateCutEffic() )
    {
      STDOUT("ERROR: updateCutEffic did not complete successfully.");
    }

  return ;
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

bool baseClass::passedAllOtherSameAndLowerLevelCuts(const string& s)
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

double baseClass::getPreCutValue1(const string& s)
{
  double ret;
  map<string, preCut>::iterator cc = preCutName_cut_.find(s);
  if( cc == preCutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in preCutName_cut_. Returning");
    }
  preCut * c = & (cc->second);
  if(c->value1 == -9999999) STDOUT("ERROR: value1 of preliminary cut "<<s<<" was not provided.");
  return (c->value1);
}

double baseClass::getPreCutValue2(const string& s)
{
  double ret;
  map<string, preCut>::iterator cc = preCutName_cut_.find(s);
  if( cc == preCutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in preCutName_cut_. Returning");
    }
  preCut * c = & (cc->second);
  if(c->value2 == -9999999) STDOUT("ERROR: value2 of preliminary cut "<<s<<" was not provided.");
  return (c->value2);
}

double baseClass::getPreCutValue3(const string& s)
{
  double ret;
  map<string, preCut>::iterator cc = preCutName_cut_.find(s);
  if( cc == preCutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in preCutName_cut_. Returning");
    }
  preCut * c = & (cc->second);
  if(c->value3 == -9999999) STDOUT("ERROR: value3 of preliminary cut "<<s<<" was not provided.");
  return (c->value3);
}

double baseClass::getPreCutValue4(const string& s)
{
  double ret;
  map<string, preCut>::iterator cc = preCutName_cut_.find(s);
  if( cc == preCutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in preCutName_cut_. Returning");
    }
  preCut * c = & (cc->second);
  if(c->value4 == -9999999) STDOUT("ERROR: value4 of preliminary cut "<<s<<" was not provided.");
  return (c->value4);
}


double baseClass::getCutMinValue1(const string& s)
{
  double ret;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning");
    }
  cut * c = & (cc->second);
  return (c->minValue1);
}

double baseClass::getCutMaxValue1(const string& s)
{
  double ret;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning");
    }
  cut * c = & (cc->second);
  return (c->maxValue1);
}

double baseClass::getCutMinValue2(const string& s)
{
  double ret;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning");
    }
  cut * c = & (cc->second);
  return (c->minValue2);
}

double baseClass::getCutMaxValue2(const string& s)
{
  double ret;
  map<string, cut>::iterator cc = cutName_cut_.find(s);
  if( cc == cutName_cut_.end() ) 
    {
      STDOUT("ERROR: did not find variableName = "<<s<<" in cutName_cut_. Returning");
    }
  cut * c = & (cc->second);
  return (c->maxValue2);
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
	  if( passedAllPreviousCuts(c->variableName) )                c->histo2.Fill( c->value );
	  if( passedAllOtherSameAndLowerLevelCuts(c->variableName) )  c->histo3.Fill( c->value );
	  if( passedAllOtherCuts(c->variableName) )                   c->histo4.Fill( c->value );
	  if( passedCut("all") )                                      c->histo5.Fill( c->value );
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

  os << "################################## Preliminary Cut Values ###################################################################\n"
     << "########################### variableName              value1       value2               value3          value4          level\n"
     << preCutInfo_.str();

  os.precision(4); 
  os << "################################## Cuts #####################################################################################\n"
     <<"#id   variableName           min1           max1           min2           max2          level              N          Npass         EffRel      errEffRel         EffAbs      errEffAbs"<<endl
     << fixed
     << setw(3) << "0"
     << setw(15) << "nocut" 
     << setprecision(4) 
     << setw(15) << "-"
     << setw(15) << "-"
     << setw(15) << "-"
     << setw(15) << "-"
     << setw(15) << "-"
     << setw(15) << nEnt
     << setw(15) << nEnt
     << setprecision(5) 
     << setw(15) << "1.00000"
     << setw(15) << "0.00000"
     << setw(15) << "1.00000"
     << setw(15) << "0.00000"
     << endl;
  for (vector<string>::iterator it = orderedCutNames_.begin(); 
       it != orderedCutNames_.end(); it++) 
    {
      cut * c = & (cutName_cut_.find(*it)->second);
      double effRel = (double) c->nEvtPassed / (double) c->nEvtInput;
      double effRelErr = sqrt( (double) effRel * (1.0 - (double) effRel) / (double) c->nEvtInput );
      double effAbs = (double) c->nEvtPassed / (double) nEnt;
      double effAbsErr = sqrt( (double) effAbs * (1.0 - (double) effAbs) / (double) nEnt );

      std::stringstream ssm1, ssM1, ssm2,ssM2;
      ssm1 << fixed << setprecision(4) << c->minValue1;
      ssM1 << fixed << setprecision(4) << c->maxValue1;
      if(c->minValue2 == -9999999) 
	{
	  ssm2 << "-inf";
	}
      else
	{
	  ssm2 << fixed << setprecision(4) << c->minValue2;
	}
      if(c->maxValue2 ==  9999999) 
	{
	  ssM2 << "+inf";
	}
      else
	{
	  ssM2 << fixed << setprecision(4) << c->maxValue2;
	}
      os << setw(3) << c->id 
	 << setw(15) << c->variableName 
	 << setprecision(4)
	 << fixed 
	 << setw(15) << ( ( c->minValue1 == -9999999.0 ) ? "-inf" : ssm1.str() )
	 << setw(15) << ( ( c->maxValue1 ==  9999999.0 ) ? "+inf" : ssM1.str() )
	 << setw(15) << ( ( c->minValue2 > c->maxValue2 ) ? "-" : ssm2.str() )
	 << setw(15) << ( ( c->minValue2 > c->maxValue2 ) ? "-" : ssM2.str() )
	 << setw(15) << c->level_int
	 << setw(15) << c->nEvtInput
	 << setw(15) << c->nEvtPassed
	 << setprecision(5) 
	 << setw(15) << effRel
	 << setw(15) << effRelErr
	 << setw(15) << effAbs
	 << setw(15) << effAbsErr
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

double baseClass::decodeCutValue(const string& s)
{
  //STDOUT("s = "<<s);
  double ret;
  if( s == "inf" || s == "+inf" )
    {
       ret = 9999999;
    }
  else if ( s == "-inf" || s == "-" )
    {
       ret = -9999999;
    }
  else
    {
       ret = atof( s.c_str() );
    }
  return ret;
}
