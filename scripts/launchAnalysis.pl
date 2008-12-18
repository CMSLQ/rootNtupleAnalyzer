#!/usr/local/bin/perl

###################################
## Code to launch the analysis   ##
###################################

#--------------------------------------------------------------
# Francesco Santanastasio  <francesco.santanastasio@cern.ch>
#--------------------------------------------------------------

print "Starting...\n";

use Time::Local;
use Getopt::Std;

## input info

my $inputList;
my $outputDir;
my $treename;

getopts('h:i:o:n:');

if(!$opt_i) {help();}
if(!$opt_o) {help();}
if(!$opt_n) {help();}

if($opt_h) {help();}
if($opt_i) {$inputList = $opt_i;}
if($opt_o) {$outputDir = $opt_o;}
if($opt_n) {$treename = $opt_n;}

system "mkdir -p $outputDir";

open (INPUTLIST, "<$inputList") || die ("...error reading file $inputList $!");
@inputList = <INPUTLIST>;
#print @inputList;
close(INPUTLIST);

open (FILE, "ls -l ../src/analysisClass.C |") || die ("...error reading file $inputList $!");
$analysisClassFull = <FILE>;
close(FILE);

## 1st split
my @array = split(/\s+/ , $analysisClassFull );
#print "$analysisClassFull\n";
#print "$array[10]\n";

## 2nd split
my @codenameC = split(/\// , $array[10] );
#print "@codenameC\n";
#print "$codenameC[scalar(@codenameC)-1]\n";

## 3rd split
my ($codename,$EXT) = split(/\./ , $codenameC[scalar(@codenameC)-1] );
#print "$codename\n";

for $line(@inputList)
{
    chomp($line);

    my @array1 = split(/\// , $line );
    my ($dataset,$EXT) = split(/\./ , $array1[scalar(@array1)-1] );

    print "../main $line $treename $outputDir/$codename\_\_\_$dataset\n";
    system "../main $line $treename $outputDir/$codename\_\_\_$dataset";
}

#---------------------------------------------------------#

sub help(){
    print "Usage: ./launchAnalysis.pl -i <inputList> -n <treename> -o <outputDir> [-h <help?>] \n";
    print "Example: ./launchAnalysis.pl -i /home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer/config/inputListAllCurrent.txt -n RootTupleMaker -o /home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer/data/output\n";
    print "Options:\n";
    print "-i <inputList>:      choose the file containing all the input lists for the analysis\n";
    print "-n <treename>:       choose the name of the TTree of the .root files you want to analyze\n";
    print "-o <outputDir>:      choose the output directory where the .root list files will be stored\n";
    print "-h <yes> :           to print the help \n";
    die "please, try again...\n";
}



