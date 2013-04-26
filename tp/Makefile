$(VERBOSE).SILENT:


FLAGS = -Wall -W
PROFFLAGS = $(FLAGS) -pg

CXX = g++ -std=c++0x -Ih/ $(FLAGS)
PROFCXX = g++ -std=c++0x -Ih/ $(PROFFLAGS)

OBJFILES = $(patsubst src/%,o/%,$(patsubst %.cpp,%.o,$(shell find src | grep \\.cpp$)))
PROFOBJFILES = $(patsubst src/%,prof/%,$(patsubst %.cpp,%.o,$(shell find src | grep \\.cpp$)))

PROGNAME = run

LIBS =

all: clear prepare $(PROGNAME)

test: all
	cd test && make

$(PROGNAME): $(OBJFILES)
	@echo "Linking "$@
	@echo "    CXX        -o "$(PROGNAME)"        "$(OBJFILES)
	$(CXX) -o $(PROGNAME) $(OBJFILES)

o/%.o: src/%.cpp
	@echo "Compiling "$@
	@echo "    CXX       "$<
	$(CXX) -c $< -o $@
	
prepare:
	mkdir -p o
	mkdir -p prof

clear:
	clear
		
clean:
	@echo "Cleaning object files"
	@echo "    rm -f       "$(OBJFILES)
	rm -f $(OBJFILES)

flush: clean
	@echo "Removing program"
	@echo "    rm -f    "$(PROGNAME)
	rm -f $(PROGNAME)
	
rebuild: flush all

recompile: rebuild
	cd test; make rebuild; cd ..;

strip: $(PROGNAME)
	@echo "Stripping "$(PROGNAME)
	echo -n "Size of "$(PROGNAME)" before strip is "
	ls -sh $(PROGNAME) | cut -d' ' -f1
	@echo "    strip     "$(PROGNAME)
	strip $(PROGNAME)
	echo -n "Size of "$(PROGNAME)" after strip is "
	ls -sh $(PROGNAME) | cut -d' ' -f1
 
nothing:
	@echo "Nothing to do; quitting  :("
	@echo "HINT: Try make all"
