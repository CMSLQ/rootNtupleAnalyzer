#!/usr/bin/env python

#---Import
import sys
import string
from optparse import OptionParser
import os.path
from ROOT import *
import re

#--- ROOT general options
gStyle.SetOptStat(1)
gStyle.SetPalette(1)

#--- Declare canvas 
#c1 = TCanvas('c1','Example',200,10,600,400)
#c1.SetFillColor(kWhite)

#--- Declare histograms
h_LQmassAlgo2_With3Jets_LQ = TH1F()
h_LQmassAlgo_With2Jets_LQ = TH1F()
h_LQmassAlgo2_With3Jets_LQ250 = TH1F()
h_LQmassAlgo_With2Jets_LQ250 = TH1F()
h_LQmassAlgo2_With3Jets_LQ400 = TH1F()
h_LQmassAlgo_With2Jets_LQ400 = TH1F() 

h_LQmassAlgo2_With3Jets_LQ.SetName("h_LQmassAlgo2_With3Jets_LQ")
h_LQmassAlgo_With2Jets_LQ.SetName("h_LQmassAlgo_With2Jets_LQ") 
h_LQmassAlgo2_With3Jets_LQ250.SetName("h_LQmassAlgo2_With3Jets_LQ250")
h_LQmassAlgo2_With3Jets_LQ400.SetName("h_LQmassAlgo2_With3Jets_LQ400")
h_LQmassAlgo_With2Jets_LQ250.SetName("h_LQmassAlgo_With2Jets_LQ250")
h_LQmassAlgo_With2Jets_LQ400.SetName("h_LQmassAlgo_With2Jets_LQ400") 

#---# #---# #---#


#--- functions
def AddHisto(inputHistoName, outputHisto, inputRootFileName, currentWeight,
             rebin=int(1), currentColor=int(1), currentFillStyle=int(1001), currentMarker=int(1)):
    file = TFile(inputRootFile);
    #file.ls()
    htemp = file.Get(inputHistoName)
    htemp.Rebin(rebin)
    if( outputHisto.GetNbinsX() != htemp.GetNbinsX() ):
        outputHisto.SetStats(1)
        outputHisto.SetBins(htemp.GetNbinsX(), htemp.GetXaxis().GetXmin(), htemp.GetXaxis().GetXmax())
    outputHisto.Add(htemp, currentWeight)
    outputHisto.SetFillColor(currentColor)
    outputHisto.SetFillStyle(currentFillStyle)
    outputHisto.SetMarkerStyle(currentMarker)
    outputHisto.SetMarkerColor(currentColor)

    file.Close()
    return

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


#---Loop over datasets
print "\n"
for n, lin in enumerate( open( options.inputList ) ):

    lin = string.strip(lin,"\n")
    #print lin
    
    dataset_mod = string.split( string.split(lin, "/" )[-1], ".")[0]
    print dataset_mod + " ... "

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
        print line
        
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
    print "weight: " + str(weight)

    #---Combine histograms using PYROOT

    #--- TODO: IMPROVE DRAW OPTIONS (*MARKERS*??, LINE STYLE)---#
    #--- TODO: EXAMPLE WITH SEVERAL DATASETS ---#
    #--- TODO: ADD LEGEND WITH THE NAME OF THE SAMPLE ---#

    #AddHisto(inputHistoName, outputHisto, inputRootFileName, datasetIdx, currentWeight):

    #-legend
    #color = int(1) #black
    #color = int(2) #red
    #color = int(3) #green
    #fillstyle = int(0) #hollow
    #fillstyle = int(3002) #dots
    #fillstyle = int(3004) #lines
    #marker = int(20) #circle solid
    #marker = int(24) #circle empty
    #marker = int(21) #square solid
    #marker = int(25) #square empty
    #marker = int(22) #trinagle solid
    #marker = int(26) #triangle empty
    
    #LQtoUE
    name = "LQtoUE"
    rebin = int (10)
    color = int(1) #black
    fillstyle = int(1001) #solid
    marker = int(20) #circle solid
    
    if( re.search(name, dataset_mod) ):
        AddHisto("h_LQmassAlgo_With2Jets", h_LQmassAlgo_With2Jets_LQ, inputRootFile, weight, rebin, color, fillstyle, marker)
        AddHisto("h_LQmassAlgo2_With3Jets", h_LQmassAlgo2_With3Jets_LQ, inputRootFile, weight, rebin, color, fillstyle, marker)

    #LQtoUE_M250
    name = "LQtoUE_M250"
    rebin = int (10)
    color = int(2) #red
    fillstyle = int(1001) #solid
    marker = int(25) #square empty

    if( re.search(name, dataset_mod) ):
        AddHisto("h_LQmassAlgo_With2Jets", h_LQmassAlgo_With2Jets_LQ250, inputRootFile, weight, rebin, color, fillstyle, marker)
        AddHisto("h_LQmassAlgo2_With3Jets", h_LQmassAlgo2_With3Jets_LQ250, inputRootFile, weight, rebin, color, fillstyle, marker)

    #LQtoUE_M400
    name = "LQtoUE_M400"
    rebin = int (10)
    color = int(3) #green
    fillstyle = int(3004) #lines
    marker = int(26) #triangle empty

    if( re.search(name, dataset_mod) ):
        AddHisto("h_LQmassAlgo_With2Jets", h_LQmassAlgo_With2Jets_LQ400, inputRootFile, weight, rebin, color, fillstyle, marker)
        AddHisto("h_LQmassAlgo2_With3Jets", h_LQmassAlgo2_With3Jets_LQ400, inputRootFile, weight, rebin, color, fillstyle, marker)


    #---End of the loop over datasets---#


#--- Create final plots (stacks)

#--- TODO: ADD AXIS TITLES ---#
#--- TODO: ADD CORRECT STATISTICS ---#
#--- TODO: ADD LEGEND TO THE STACK ---#

h_LQmassAlgo_With2Jets_LQstack = THStack("h_LQmassAlgo_With2Jets_LQstack","h_LQmassAlgo_With2Jets_LQstack")
h_LQmassAlgo_With2Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ250)
h_LQmassAlgo_With2Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ400)

h_LQmassAlgo2_With3Jets_LQstack = THStack("h_LQmassAlgo2_With3Jets_LQstack","h_LQmassAlgo2_With3Jets_LQstack")
h_LQmassAlgo2_With3Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ250)
h_LQmassAlgo2_With3Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ400)

#--- Draw and Save final plots

#---# #---# #---# don't modify this 
outputTfile = TFile( options.analysisCode + "_plots.root" , "RECREATE")
#---# #---# #---# 

#--- TODO: DRAW PLOTS ON .PS FILE ---#

h_LQmassAlgo_With2Jets_LQ250.Write()
h_LQmassAlgo2_With3Jets_LQ250.Write()
h_LQmassAlgo_With2Jets_LQ400.Write()
h_LQmassAlgo2_With3Jets_LQ400.Write()
h_LQmassAlgo_With2Jets_LQ.Write()
h_LQmassAlgo2_With3Jets_LQ.Write()
h_LQmassAlgo_With2Jets_LQstack.Write()
h_LQmassAlgo2_With3Jets_LQstack.Write()

outputTfile.Close()


