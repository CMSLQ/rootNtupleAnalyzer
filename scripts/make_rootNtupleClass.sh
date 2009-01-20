#!/bin/sh

usage ()
{
        echo "Usage:   $0 -f rootFile -t TTreeName "
        echo "where:   rootFile is the input root file and TTreeName is the name of the TTree to be analyzed"
        echo "Example: $0 -f data/input/Exotica_LQtoUE_M250__Summer08_IDEAL_V9_v1__GEN-SIM-RECO_1.root -t RootTupleMaker"
        exit 1;
}

if [ $# -le 3 ]; then usage; fi;
while [ $# -gt 0 ]; # till there are parameters
do
  case "$1" in
    -f) FILENAME="$2"; shift ;;
    -t) TTREENAME="$2"; shift ;;
    *) usage ;;
  esac
  shift  # get following parameters
done

cd `dirname $0`/../ ; # go to the directory rootNtupleAnalyzer/

cat > temporaryMacro.C <<EOF

{
  TFile f("$FILENAME");
  $TTREENAME->MakeClass("rootNtupleClass");
}

EOF

root -l -q temporaryMacro.C
rm temporaryMacro.C
if [ -f "rootNtupleClass.h" ] && [ -f "rootNtupleClass.C" ]; then
    echo "Moving rootNtupleClass.h/C to ./include/ and ./src/ directories ..."
    mv -i rootNtupleClass.h include/
    mv -i rootNtupleClass.C src/
    #if [ -f "include/rootNtupleClass.h" ] && [ -f "src/rootNtupleClass.C" ]; then echo "... done."; fi;

    #echo "Creating src/analysisClass.C ..."
    #cp -i src/analysisClass_template.C src/analysisClass.C

    echo "done";    
else
    echo "Error: files rootNtupleClass.h/C have not been created."
fi







