#!/usr/local/bin/perl

###################################
## Code to create the input list ##
###################################

#--------------------------------------------------------------
# Francesco Santanastasio  <francesco.santanastasio@cern.ch>
#--------------------------------------------------------------

print "Starting...\n";

use Time::Local;
use Getopt::Std;

## input info

my $inputDir;
my $outputDir;
my $MATCH;

getopts('h:d:o:m:');

if(!$opt_d) {help();}
if(!$opt_o) {help();}
if(!$opt_m) {help();}

if($opt_h) {help();}
if($opt_d) {$inputDir = $opt_d;}
if($opt_o) {$outputDir = $opt_o;}
if($opt_m) {$MATCH = $opt_m;}

system "mkdir -p $outputDir";

open (INPUTLIST, "ls $inputDir | grep $MATCH |") || die ("...error reading file $inputDir $!");
@inputList = <INPUTLIST>;
#print @inputList;
close(INPUTLIST);


my @AllLists;
my $AllListsFilename="$outputDir/inputListAllCurrent.txt";
system "rm -f $AllListsFilename";

#first loop to remove the old files
for $file(@inputList)
{

    ## split each line
    my ($dataset) = split( /\_\d+\.root/ , $file );
    #print "$dataset\n";

    $listfilename="$dataset.txt";
    #print "$listfilename\n";

    system "rm -f $outputDir/$listfilename";

}


#second loop to create the new files
for $file(@inputList)
{
    chomp($file);
    #print "$file\n";

    ## split each line
    my ($dataset) = split( /\_\d+\.root/ , $file );
    #print "$dataset\n";

    $listfilename="$outputDir/$dataset.txt";
    #print "$listfilename\n";

    system "touch $listfilename";

    my $IsPresent=0;
    for $line(@AllLists)
    {
	if($IsPresent==0)
	{
	    if($listfilename eq $line)
	    {
		$IsPresent=1;
	    }
	}
    }

    if($IsPresent==0)
    {
	print "$listfilename\n";
	push @AllLists, $listfilename;
    }

    open(LISTFILENAME,">>$listfilename");
    print LISTFILENAME "$inputDir/$file\n";
    close(LISTFILENAME);
}


open(LISTALLFILENAME,">$AllListsFilename");
for $txtfile(@AllLists)
{
    chomp($txtfile);
    print LISTALLFILENAME "$txtfile\n";
}
close(LISTALLFILENAME);

print "List created: $AllListsFilename\n";

#print "@AllLists\n";

#---------------------------------------------------------#

sub help(){
    print "Usage: ./createList.pl -d <inputDir> -m <match> -o <outputDir> [-h <help?>] \n";
    print "Example: ./createList.pl -d /home/santanas/Data/Leptoquarks/RootNtuples/V00-00-06_2008121_163513/output -m root -o /home/santanas/Workspace/Leptoquarks/rootNtupleAnalyzer/config \n";
    print "Options:\n";
    print "-d <inputDir>:       choose the input directory containing the files (FULL PATH REQUIRED + NO SLASH AT THE END)\n";
    print "-m <match>:          choose the parameter MATCH, will be used to select only the files whose filename matches with the string MATCH\n";
    print "-o <outputDir>:      choose the output directory where the .txt list files will be stored (FULL PATH REQUIRED NO SLASH AT THE END)\n";
    print "-h <yes> :           to print the help \n";
    die "please, try again...\n";
}
