#!/usr/bin/env python

#---Import
import sys
import string
from optparse import OptionParser
import os.path
from ROOT import *
import re


#---Option Parser
#--- TODO: WHY PARSER DOES NOT WORK IN CMSSW ENVIRONMENT? ---#
usage = "usage: %prog [options] \nExample: \n./combinePlotsTemplate.py -i /home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer/config/inputListAllCurrent.txt -c analysisClass_genStudies -d /home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer/data/output -l 100 -x /home/santanas/Data/Leptoquarks/RootNtuples/V00-00-06_2008121_163513/xsection_pb_default.txt -o /home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer/data/output"

parser = OptionParser(usage=usage)

parser.add_option("-i", "--inputList", dest="inputList",
                  help="list of all datasets to be used (full path required)",
                  metavar="LIST")

parser.add_option("-c", "--code", dest="analysisCode",
                  help="name of the CODE.C code used to generate the rootfiles",
                  metavar="CODE")

parser.add_option("-d", "--inputDir", dest="inputDir",
                  help="the directory INDIR contains the rootfiles with the histograms to be combined (full path required)",
                  metavar="INDIR")

parser.add_option("-l", "--intLumi", dest="intLumi",
                  help="results are rescaled to the integrated luminosity INTLUMI (in pb-1)",
                  metavar="INTLUMI")

parser.add_option("-x", "--xsection", dest="xsection",
                  help="the file XSEC contains the cross sections (in pb) for all the datasets (full path required)",
                  metavar="XSEC")

parser.add_option("-o", "--outputDir", dest="outputDir",
                  help="the directory OUTDIR contains the output of the program (full path required)",
                  metavar="OUTDIR")

(options, args) = parser.parse_args()

if len(sys.argv)<12:
    print "ERROR: not enough arguments --> ./combinePlotsTemplate.py -h or ./combinePlotsTemplate.py --help for options"
    sys.exit()


if len(sys.argv)<2:
    print "./combinePlotsTemplate.py -h or ./combinePlotsTemplate.py --help for options"
    sys.exit()

#print options.analysisCode


#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%begin

#--- Declare histograms
dictSamples = {"LQtoUE_M250": ["Exotica_LQtoUE_M250"],
               "LQtoUE_M400": ["Exotica_LQtoUE_M400"],
               "Z": ["Zee","ZJets-madgraph"],
               "QCD": ["HerwigQCDPt","PYTHIA8PhotonJetPt","QCDDiJetPt"],
               "TTBAR": ["TTJets-madgraph"] }
dictFinalHisto = {}

#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%end

#--- functions
def UpdateHistograms(name, N, listHisto, inputRootFile, weight, dataset_mod, listNames):
    file = TFile(inputRootFile);
    #file.ls()
    print N
    print name
    nHistos = int( file.GetListOfKeys().GetEntries() )

    htemp = TH1F()

    for h in range(0, nHistos):
        print file.GetListOfKeys()[h].GetName()

        htemp = file.Get(file.GetListOfKeys()[h].GetName())
        #print htemp.GetEntries()

        listHisto[name][h].GetEntries()

        toBeUpdated = False
        for i, String in enumerate (listNames[name]):
            print String
            if( re.search(String, dataset_mod) ):
                print "toBeUpdated"
                toBeUpdated = True
                break

        print toBeUpdated

        if(toBeUpdated):
            listHisto[name][h].Add(htemp, weight)

            #    outputHisto.Add(htemp, currentWeight)

    file.Close()
    return

#---Loop over datasets
print "\n"
for n, lin in enumerate( open( options.inputList ) ):

    lin = string.strip(lin,"\n")
    #print lin
    
    dataset_mod = string.split( string.split(lin, "/" )[-1], ".")[0]
    print "\n" + str(n) + " " + dataset_mod + " ... "

    inputRootFile = options.inputDir + "/" + options.analysisCode + "___" + dataset_mod + ".root"
    inputDataFile = options.inputDir + "/" + options.analysisCode + "___" + dataset_mod + ".dat"

    #print inputRootFile
    #print inputDataFile

    #---Check if .root and .dat file exist
    if(os.path.isfile(inputRootFile) == False):
        print "ERROR: file " + inputRootFile + " not found in " + options.inputDir
        print "exiting..."
        sys.exit()

    if(os.path.isfile(inputDataFile) == False):
        print "ERROR: file " + inputDataFile + " not found in " + options.inputDir
        print "exiting..."
        sys.exit()

    #---Find xsection correspondent to the current dataset
    if(os.path.isfile(options.xsection) == False):
        print "ERROR: file " + options.xsection + " not found"
        print "exiting..."
        sys.exit()

    for lin1 in open( options.xsection ):

        lin1 = string.strip(lin1,"\n")

        (dataset , xsection_val) = string.split(lin1)
        #print dataset + " " + xsection_val

        dataset_mod_1 = dataset[1:].replace('/','__')
        #print dataset_mod_1 + " " + xsection_val

        if(dataset_mod_1 == dataset_mod):
            xsectionIsFound = True
            break

    if(xsectionIsFound == False):
        print "ERROR: xsection for dataset" + dataset + " not found in " + options.xsection
        print "exiting..."
        sys.exit()
        
    #this is the current cross section
    #print xsection_val

    #---Read .dat table for current dataset
    data={}
    column=[]
    
    for j,line in enumerate( open( inputDataFile ) ):
        line = string.strip(line,"\n")
        #print line
        
        if j == 0:
            for i,piece in enumerate(line.split()):
                column.append(piece)
        else:
            for i,piece in enumerate(line.split()):
                if i == 0:
                    data[int(piece)] = {}
                    row = int(piece)
                else:
                    data[row][ column[i] ] = piece
                    #print data[row][ column[i] ] 

    # example
    #Ntot = int(data[0]['N'])
    #print Ntot

    #---Calculate weight
    Ntot = int(data[0]['N'])
    weight = float(xsection_val) * float(options.intLumi) / Ntot 
    #print "weight: " + str(weight)

    
    #---Combine histograms using PYROOT

    file = TFile(inputRootFile)
    nHistos = int( file.GetListOfKeys().GetEntries() )
    #print "nHistos: " , nHistos, "\n"

    # loop over samples
    for S,sample in enumerate( dictSamples ):
        #print "current sample is : " , sample

        if( n == 0): 
            dictFinalHisto[sample] = {}

        # loop over histograms in rootfile
        for h in range(0, nHistos):
            histoName = file.GetListOfKeys()[h].GetName()
            htemp = file.Get(histoName)

            if(n == 0):
                dictFinalHisto[sample][h] = TH1F()
                dictFinalHisto[sample][h].SetName("histo_" + sample + "_" + histoName )
                dictFinalHisto[sample][h].SetBins(htemp.GetNbinsX(), htemp.GetXaxis().GetXmin(), htemp.GetXaxis().GetXmax())

            #print "current histo is : " , dictFinalHisto[sample][h].GetName()

            #update histo

            #print "dataset strings in this sample"
            toBeUpdated = False
            for mS, matchString in enumerate (dictSamples[sample]):
                #print matchString
                if( re.search(matchString, dataset_mod) ):
                    #print "toBeUpdated"
                    toBeUpdated = True
                    break
            #print toBeUpdated
            if(toBeUpdated):
                dictFinalHisto[sample][h].Add(htemp, weight)

    #print "--> TEST <--"
    #for S,sample in enumerate( dictSamples ):
        #print "--> current sample: " + sample
        #print dictFinalHisto[sample] 
    
    #---End of the loop over datasets---#

print "\n"

outputTfile = TFile( options.analysisCode + "_plots.root" , "RECREATE")

for sample in ( dictFinalHisto ):
    for h, histo in enumerate ( dictFinalHisto[sample] ):
        print "writing histo: " , dictFinalHisto[sample][histo].GetName()
        dictFinalHisto[sample][histo].Write()

outputTfile.Close()
    
