#!/usr/bin/env python

#---Import
import sys
import string
from optparse import OptionParser
import os.path
from ROOT import *
import re

#---Declare efficiency tables
table_LQtoUE = {}
table_LQtoUE_M250 = {}
table_LQtoUE_M400 = {}

#---# #---# #---#

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




#--- Functions

#def AddHisto(inputHistoName, outputHisto, inputRootFileName, currentWeight,
#             rebin=int(1), currentColor=int(1), currentFillStyle=int(1001), currentMarker=int(1)):

def UpdateTable(inputTable, outputTable):
    if not outputTable:
        for j,line in enumerate( inputTable ):
            outputTable[int(j)]={'name': inputTable[j]['name'],
                                 'min': inputTable[j]['min'],
                                 'max': inputTable[j]['max'],
                                 'N':       float(inputTable[j]['N']),
                                 'errN':    pow( float(inputTable[j]['errN']), 2 ),
                                 'Npass':       float(inputTable[j]['Npass']),
                                 'errNpass':    pow( float(inputTable[j]['errNpass']), 2 ),
                                 'EffRel':      float(0),
                                 'errEffRel':   float(0),
                                 'EffAbs':      float(0),
                                 'errEffAbs':   float(0),
                                 }
    else:
        for j,line in enumerate( inputTable ):
            outputTable[int(j)]={'name': inputTable[j]['name'],
                                 'min':  inputTable[j]['min'],
                                 'max':  inputTable[j]['max'],
                                 'N':       float(outputTable[int(j)]['N']) + float(inputTable[j]['N']),
                                 'errN':    float(outputTable[int(j)]['errN']) + pow( float(inputTable[j]['errN']), 2 ),
                                 'Npass':       float(outputTable[int(j)]['Npass']) + float(inputTable[j]['Npass']),
                                 'errNpass':    float(outputTable[int(j)]['errNpass']) + pow( float(inputTable[j]['errNpass']), 2 ),
                                 'EffRel':      float(0),
                                 'errEffRel':   float(0),
                                 'EffAbs':      float(0),
                                 'errEffAbs':   float(0),
                                 }
    return
            

def CalculateEfficiency(table):
    for j,line in enumerate( table ):
        if( j == 0):
            table[int(j)] = {'name':       table[int(j)]['name'],
                             'min':        table[int(j)]['min'],
                             'max':        table[int(j)]['max'],
                             'N':          float(table[j]['N']) ,
                             'errN':       int(0), 
                             'Npass':      float(table[j]['Npass']) ,
                             'errNpass':   int(0), 
                             'EffRel':     int(1),
                             'errEffRel':  int(0),
                             'EffAbs':     int(1),
                             'errEffAbs':  int(0),
                             }
        else:
            N = float(table[j]['N']) 
            errN = sqrt(float(table[j]["errN"])) 
            errRelN = errN / N 

            Npass = float(table[j]['Npass']) 
            errNpass = sqrt(float(table[j]["errNpass"]))
            errRelNpass = errNpass / Npass 

            EffRel = Npass / N
            errRelEffRel = sqrt( errRelNpass*errRelNpass + errRelN*errRelN )
            errEffRel = errRelEffRel * EffRel
            
            EffAbs = Npass / float(table[0]['N'])
            errEffAbs = errNpass / float(table[0]['N'])
            
            table[int(j)]={'name': table[int(j)]['name'],
                           'min': table[int(j)]['min'],
                           'max': table[int(j)]['max'],
                           'N':       N,
                           'errN':    errN, 
                           'Npass':       Npass,
                           'errNpass':    errNpass, 
                           'EffRel':      EffRel,
                           'errEffRel':   errEffRel,
                           'EffAbs':      EffAbs,
                           'errEffAbs':   errEffAbs,
                           }
            #print table[j]
    return


def WriteTable(table, name, file):
    print >>file, name
    print >>file, "name".rjust(10),
    print >>file, "min".rjust(10),
    print >>file, "max".rjust(10),
    print >>file, "Npass".rjust(10),
    print >>file, "errNpass".rjust(10),
    print >>file, "EffRel".rjust(10),
    print >>file, "errEffRel".rjust(10),
    print >>file, "EffAbs".rjust(10),
    print >>file, "errEffAbs".rjust(10)

    for j, line in enumerate(table):
        print >>file, table[j]['name'].rjust(10),
        print >>file, table[j]['min'].rjust(10),
        print >>file, table[j]['max'].rjust(10),
        print >>file, ("%.01f" % table[j]['Npass']).rjust(10),
        print >>file, ("%.01f" % table[j]['errNpass']).rjust(10),
        print >>file, ("%.02f" % table[j]['EffRel']).rjust(10),
        print >>file, ("%.02f" % table[j]['errEffRel']).rjust(10),
        print >>file, ("%.02f" % table[j]['EffAbs']).rjust(10),
        print >>file, ("%.02f" % table[j]['errEffAbs']).rjust(10)

    print >>file, "\n"

    #--- print to screen
    
    print "\n"
    print name
    print "name".rjust(10),
    print "min".rjust(10),
    print "max".rjust(10),
    print "Npass".rjust(10),
    print "errNpass".rjust(10),
    print "EffRel".rjust(10),
    print "errEffRel".rjust(10),
    print "EffAbs".rjust(10),
    print "errEffAbs".rjust(10)

    for j, line in enumerate(table):
        print table[j]['name'].rjust(10),
        print table[j]['min'].rjust(10),
        print table[j]['max'].rjust(10),
        print ("%.01f" % table[j]['Npass']).rjust(10),
        print ("%.01f" % table[j]['errNpass']).rjust(10),
        print ("%.02f" % table[j]['EffRel']).rjust(10),
        print ("%.02f" % table[j]['errEffRel']).rjust(10),
        print ("%.02f" % table[j]['EffAbs']).rjust(10),
        print ("%.02f" % table[j]['errEffAbs']).rjust(10)

    return

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
    xsection_X_intLumi = float(xsection_val) * float(options.intLumi)
    weight = xsection_X_intLumi / Ntot 
    print "weight: " + str(weight)
    
    #---Create new table using weight
    newtable={}
    
    for j,line in enumerate( data ):
        if(j == 0):
            newtable[int(j)]={'name': data[j]['name'],
                              'min': "-",
                              'max': "-",
                              'N': "%.01f" % ( Ntot * weight ),
                              'errN': int(0),
                              'Npass': "%.01f" % ( Ntot * weight ),
                              'errNpass': int(0),
                              }

        else:
            N = ( float(data[j]['N']) * weight )
            errN = ( float(data[j-1]["errEffAbs"]) * xsection_X_intLumi )
            errRelN = errN / N 
            
            Npass = ( float(data[j]['Npass']) * weight) 
            errNpass = ( float(data[j]["errEffAbs"]) * xsection_X_intLumi )
            errRelNpass = errNpass / Npass 
            
            newtable[int(j)]={'name': data[j]['name'],
                              'min': data[j]['min'],
                              'max': data[j]['max'],
                              'N':           "%.01f" % N,
                              'errN':        "%.01f" % errN,
                              'Npass':       "%.01f" % Npass,
                              'errNpass':    "%.01f" % errNpass,
                              }
            
    #---# #---# #---#

    #---Combine tables from different datasets

    #LQtoUE
    name = "LQtoUE"
    if( re.search(name, dataset_mod) ):
        UpdateTable(newtable, table_LQtoUE)

    name = "LQtoUE_M250"
    if( re.search(name, dataset_mod) ):
        UpdateTable(newtable, table_LQtoUE_M250)

    name = "LQtoUE_M400"
    if( re.search(name, dataset_mod) ):
        UpdateTable(newtable, table_LQtoUE_M400)

    #---End of the loop over datasets---#

#--- Create final tables 
CalculateEfficiency(table_LQtoUE)
CalculateEfficiency(table_LQtoUE_M250)
CalculateEfficiency(table_LQtoUE_M400)
        
#---Print table on screen

#---# #---# #---# don't modify this 
outputTableFile = open(options.analysisCode + "_tables.dat",'w')
#---# #---# #---# 

WriteTable(table_LQtoUE, "#--- All LQtoUE ---#", outputTableFile)
WriteTable(table_LQtoUE_M250, "#--- LQtoUE M=250 GeV---#", outputTableFile)
WriteTable(table_LQtoUE_M400, "#--- LQtoUE M=400 GeV---#", outputTableFile)

#---# #---# #---# don't modify this 
outputTableFile.close
#---# #---# #---# 

#--- TODO CREATE LATEX TABLE (PYTEX?) ---#


