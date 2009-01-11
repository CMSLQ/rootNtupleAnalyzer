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
c1 = TCanvas('c1','Example',200,10,600,400)
c1.SetFillColor(kWhite)

#--- Declare histograms
h_LQmassAlgo2_With3Jets_LQ = TH1F() 
h_LQmassAlgo_With2Jets_LQ = TH1F() 

h_LQmassAlgo2_With3Jets_LQ250 = TH1F() 
h_LQmassAlgo_With2Jets_LQ250 = TH1F() 

h_LQmassAlgo2_With3Jets_LQ400 = TH1F() 
h_LQmassAlgo_With2Jets_LQ400 = TH1F() 

#---# #---# #---#

#--- functions
def AddHisto(inputHistoName, outputHisto, inputRootFileName, currentWeight, currentColor):
    file = TFile(inputRootFile);
    #file.ls()
    htemp = file.Get(inputHistoName)
    if( outputHisto.GetNbinsX() != htemp.GetNbinsX() ):
        outputHisto.SetStats(1)
        outputHisto.SetBins(htemp.GetNbinsX(), htemp.GetXaxis().GetXmin(), htemp.GetXaxis().GetXmax())
    outputHisto.Add(htemp, currentWeight)
    file.Close()
    outputHisto.SetFillColor(currentColor)
    return

#---Option Parser
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
                  help="the file XSEC contains the cross sections for all the datasets (full path required)",
                  metavar="XSEC")

parser.add_option("-o", "--outputDir", dest="outputDir",
                  help="the directory OUTDIR contains the output of the program (full path required)",
                  metavar="OUTDIR")

(options, args) = parser.parse_args()

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
    #Ntot = int(data[1]['N'])
    #print Ntot

    #---Calculate weight
    Ntot = int(data[1]['N'])
    weight = float(xsection_val) * float(options.intLumi) / Ntot 
    print "weight: " + str(weight)

    #---Combine histograms using PYROOT
    #AddHisto(inputHistoName, outputHisto, inputRootFileName, datasetIdx, currentWeight):


    #--- TODO: IMPROVE DRAW OPTIONS (COLORS, MARKERS, LINE STYLE)---#
    #--- TODO: IMPROVE REBINNING OPTIONS ---#
    #--- TODO: EXAMPLE WITH SEVERAL DATASETS ---#

    #LQtoUE
    name = "LQtoUE"
    color = int(1) #black
    if( re.search(name, dataset_mod) ):
        AddHisto("h_LQmassAlgo_With2Jets", h_LQmassAlgo_With2Jets_LQ, inputRootFile, weight, color)
        AddHisto("h_LQmassAlgo2_With3Jets", h_LQmassAlgo2_With3Jets_LQ, inputRootFile, weight, color)


    #LQtoUE_M250
    name = "LQtoUE_M250"
    color = int(2) #red
    if( re.search(name, dataset_mod) ):
        AddHisto("h_LQmassAlgo_With2Jets", h_LQmassAlgo_With2Jets_LQ250, inputRootFile, weight, color)
        AddHisto("h_LQmassAlgo2_With3Jets", h_LQmassAlgo2_With3Jets_LQ250, inputRootFile, weight, color)


    #LQtoUE_M400
    name = "LQtoUE_M400"
    color = int(3) #green
    if( re.search(name, dataset_mod) ):
        AddHisto("h_LQmassAlgo_With2Jets", h_LQmassAlgo_With2Jets_LQ400, inputRootFile, weight, color)
        AddHisto("h_LQmassAlgo2_With3Jets", h_LQmassAlgo2_With3Jets_LQ400, inputRootFile, weight, color)


    #---End of the loop over datasets---#


#--- Create final plots (stacks)

#--- TODO: ADD AXIS TITLES ---#
#--- TODO: ADD CORRECT STATISTICS ---#

h_LQmassAlgo_With2Jets_LQstack = THStack()
h_LQmassAlgo_With2Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ250)
h_LQmassAlgo_With2Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ400)

h_LQmassAlgo2_With3Jets_LQstack = THStack()
h_LQmassAlgo2_With3Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ250)
h_LQmassAlgo2_With3Jets_LQstack.Add(h_LQmassAlgo_With2Jets_LQ400)

#--- Draw and Save final plots

#--- TODO: DRAW PLOTS ON .PS FILE ---#

c1.Divide(4,2)

c1.cd(1)
c1_1.SetLogy();
h_LQmassAlgo_With2Jets_LQ250.Draw()
c1.Update()

c1.cd(2)
c1_2.SetLogy();
h_LQmassAlgo2_With3Jets_LQ250.Draw()
c1.Update()

c1.cd(3)
c1_3.SetLogy();
h_LQmassAlgo_With2Jets_LQ400.Draw()
c1.Update()

c1.cd(4)
c1_4.SetLogy();
h_LQmassAlgo2_With3Jets_LQ400.Draw()
c1.Update()

c1.cd(5)
c1_5.SetLogy();
h_LQmassAlgo_With2Jets_LQ.Draw()
c1.Update()

c1.cd(6)
c1_6.SetLogy();
h_LQmassAlgo2_With3Jets_LQ.Draw()
c1.Update()

c1.cd(7)
c1_7.SetLogy();
h_LQmassAlgo_With2Jets_LQstack.Draw()
c1.Update()

c1.cd(8)
c1_8.SetLogy();
h_LQmassAlgo2_With3Jets_LQstack.Draw()
c1.Update()

c1.SaveAs("Histo.root")


