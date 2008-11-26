COMP=c++ 
ROOTLIBS = `root-config --glibs --cflags` -lMinuit 
INC= -I.. -I. -I./include  -I${CLHEP}/include 
ROOTINC= -I${ROOTSYS}/include
LIBS= -L.  ${ROOTLIBS} -L${CLHEP}/lib -L${CLHEP}/lib 
SRC= ./src
SELECTIONLIB = $(SRC)/rootNtupleClass.o $(SRC)/baseClass.o $(SRC)/analysisClass.o
EXE = main

# ********** TEMPLATE *************
# mainProg: mainProg.o $(SELECTIONLIB)
#	$(COMP) $(INC) $(ROOTINC) $(LIBS) $(ROOTLIBS) -o $@  $(SELECTIONLIB) $@.o
# *********************************

all: ${EXE}

main: $(SRC)/main.o $(SELECTIONLIB)
	$(COMP) $(INC) $(ROOTINC) $(LIBS) -o $@  $(SELECTIONLIB) $(SRC)/$@.o

clean:
	rm -f src/*.o *.lo core core.*
	rm -f *~	
	rm -f *.exe
	rm -f $(EXE)
.cpp.o:
	$(COMP) -c $(INC) $(ROOTINC) -o $@ $<

.cc.o:
	$(COMP) -m32 -c $(INC) $(ROOTINC) -o $@ $<

.cxx.o:
	$(COMP) -c $(INC) $(ROOTINC) -o $@ $<

.C.o:
	$(COMP) -c $(INC) $(ROOTINC) -o $@ $<


