#!/usr/bin/env python

#---Import
import sys
import string
from optparse import OptionParser
import os.path
from ROOT import *
import re


#--- ROOT general options
gStyle.SetOptStat(0)
gStyle.SetPalette(1)
gStyle.SetCanvasBorderMode(0)
gStyle.SetFrameBorderMode(0)
gStyle.SetCanvasColor(kWhite)
gStyle.SetPadTickX(1);
gStyle.SetPadTickY(1);
#--- TODO: WHY IT DOES NOT LOAD THE DEFAULT ROOTLOGON.C ? ---#

#---Option Parser
#--- TODO: WHY PARSER DOES NOT WORK IN CMSSW ENVIRONMENT? ---#
usage = "usage: %prog [options] \nExample: ./makeStackHistoTemplate.py -i ../data/output/analysisClass_template_plots.root -n cutHisto_allPreviousCuts__pT1stEle -x p_T1st_ele_\(GeV\) -y number_of_entries"

parser = OptionParser(usage=usage)

parser.add_option("-n", "--nameHisto", dest="nameHisto",
                  help="name of the histogram to be plotted",
                  metavar="NAMEHISTO")

parser.add_option("-i", "--inputRootFile", dest="inputRootFile",
                  help="the rootfile containing the histograms",
                  metavar="INPUTROOTFILE")

parser.add_option("-x", "--xAxisTitle", dest="xAxisTitle",
                  help="x axis title; words must be separated by _",
                  metavar="XAXISTITLE")

parser.add_option("-y", "--yAxisTitle", dest="yAxisTitle",
                  help="y axis title; words must be separated by _",
                  metavar="YAXISTITLE")

(options, args) = parser.parse_args()

if len(sys.argv)<8:
    print usage
    sys.exit()


xaxistitle = ""
yaxistitle = ""
for word in string.split(options.xAxisTitle,"_"):
    xaxistitle += word + " "
for word in string.split(options.yAxisTitle,"_"):
    yaxistitle += word + " "



#---Check input rootfile
if(os.path.isfile(options.inputRootFile) == False):
    print "ERROR: file " + options.inputRootFile + " not found"
    print "exiting..."
    sys.exit()
inputfile = TFile(options.inputRootFile)
inputfile.ls()

#---Open output rootfile
outputFileName = string.split(string.split(sys.argv[0],"/")[-1],".")[0] + "_" + options.nameHisto + ".root"
outputfile = TFile(outputFileName, "RECREATE")

#---Create legend
legend = TLegend(0.519328,0.593985,0.865546,0.862155);
legend.SetFillColor(kWhite);

#---Edit histograms taken from rootfile
rebin = 2

#LQtoUE_M250
histo_LQtoUE_M250 = TH1F()
prefix = "histo1D__"+ "LQtoUE_M250" + "__"
color = 1
fillStyle = 1000
markerStyle = 20
histo_LQtoUE_M250 = inputfile.Get( prefix + options.nameHisto)
if( not histo_LQtoUE_M250):
    print "ERROR: histo " + prefix + options.nameHisto + " not found in " + options.inputRootFile
    print "exiting..."
    sys.exit()
histo_LQtoUE_M250.SetFillColor(color)
histo_LQtoUE_M250.SetFillStyle(fillStyle)
histo_LQtoUE_M250.SetMarkerStyle(markerStyle)
histo_LQtoUE_M250.SetMarkerColor(color)
histo_LQtoUE_M250.Rebin(rebin)

legend.AddEntry(histo_LQtoUE_M250,"LQtoUE_M250","p");

#LQtoUE_M400
histo_LQtoUE_M400 = TH1F()
prefix = "histo1D__"+ "LQtoUE_M400" + "__" 
color = 2
fillStyle = 1000
markerStyle = 22
histo_LQtoUE_M400 = inputfile.Get(prefix + options.nameHisto)
if( not histo_LQtoUE_M400):
    print "ERROR: histo " + prefix + options.nameHisto + " not found in " + options.inputRootFile
    print "exiting..."
    sys.exit()
histo_LQtoUE_M400.SetFillColor(color)
histo_LQtoUE_M400.SetFillStyle(fillStyle)
histo_LQtoUE_M400.SetMarkerStyle(markerStyle)
histo_LQtoUE_M400.SetMarkerColor(color)
histo_LQtoUE_M400.Rebin(rebin)
legend.AddEntry(histo_LQtoUE_M400,"LQtoUE_M400","p");

#QCD
histo_QCD = TH1F()
prefix = "histo1D__"+ "QCD" + "__" 
color = 3
fillStyle = 3001
histo_QCD = inputfile.Get(prefix + options.nameHisto)
if( not histo_QCD):
    print "ERROR: histo " + prefix + options.nameHisto + " not found in " + options.inputRootFile
    print "exiting..."
    sys.exit()
histo_QCD.SetFillColor(color)
histo_QCD.SetFillStyle(fillStyle)
histo_QCD.Rebin(rebin)
legend.AddEntry(histo_QCD,"QCD","f");

#TTbar
histo_TTbar = TH1F()
prefix = "histo1D__"+ "TTbar" + "__" 
color = 6
fillStyle = 3004
histo_TTbar = inputfile.Get(prefix + options.nameHisto)
if( not histo_TTbar):
    print "ERROR: histo " + prefix + options.nameHisto + " not found in " + options.inputRootFile
    print "exiting..."
    sys.exit()
histo_TTbar.SetFillColor(color)
histo_TTbar.SetFillStyle(fillStyle)
histo_TTbar.Rebin(rebin)
legend.AddEntry(histo_TTbar,"TBAR","f");

#Z
histo_Z = TH1F()
prefix = "histo1D__"+ "Z" + "__" 
color = 4
fillStyle = 3002
histo_Z = inputfile.Get(prefix + options.nameHisto)
if( not histo_Z):
    print "ERROR: histo " + prefix + options.nameHisto + " not found in " + options.inputRootFile
    print "exiting..."
    sys.exit()
histo_Z.SetFillColor(color)
histo_Z.SetFillStyle(fillStyle)
histo_Z.Rebin(rebin)
legend.AddEntry(histo_Z,"Z+jet","f");

#ALLBKG
histo_ALLBKG = TH1F()
prefix = "histo1D__"+ "ALLBKG" + "__" 
color = 5
fillStyle = 3003
histo_ALLBKG = inputfile.Get(prefix + options.nameHisto)
if( not histo_ALLBKG):
    print "ERROR: histo " + prefix + options.nameHisto + " not found in " + options.inputRootFile
    print "exiting..."
    sys.exit()
histo_ALLBKG.SetFillColor(color)
histo_ALLBKG.SetFillStyle(fillStyle)
histo_ALLBKG.SetLineWidth(3)
histo_ALLBKG.Rebin(rebin)
legend.AddEntry(histo_ALLBKG,"All Bkgrnd","f");

print "\n"
print "plotting " + options.nameHisto + " ..."

#---Canvas
canvas = TCanvas()
stackName = "stack_" + options.nameHisto
canvas.SetName(stackName)
canvas.SetLogy();
histo_LQtoUE_M250.Draw("")
histo_LQtoUE_M250.GetXaxis().SetTitle(xaxistitle)
histo_LQtoUE_M250.GetYaxis().SetTitle(yaxistitle)
histo_LQtoUE_M250.SetStats(0)

histo_LQtoUE_M400.Draw("same")

histo_ALLBKG.Draw("HISTsame")

histo_Z.Draw("HISTsame")

histo_QCD.Draw("HISTsame")

histo_TTbar.Draw("HISTsame")

legend.Draw()
gPad.RedrawAxis()
gPad.Modified()
canvas.Update()

#---Write 
histo_ALLBKG.Write()
histo_QCD.Write()
histo_Z.Write()
histo_LQtoUE_M250.Write()
histo_LQtoUE_M400.Write()
legend.Write()
canvas.Write()
canvas.SaveAs(stackName + ".gif")
canvas.SaveAs(stackName + ".eps")

print "writing " + outputFileName + " ..."

inputfile.Close()
outputfile.Close()
