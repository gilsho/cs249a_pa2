# This is a Makefile for the tests for the example 249a project. It is meant to
# show how Google tests are configured. This Makefile is verbose for the
# purpose of showing how all the pieces fit together.

# #################
# Source location Configuration
# #################

CXX = g++-4.6

# Where the files you are testing will be located
SRC_PATH += ../src/

# This is where the base installation folder is. In this case it is the current
# folder so it is just the gtest folder.
GUNIT_BASE += gtest-1.6.0

# This is the full path we need for inclusion of gunit code.
GUNIT_PATH += $(GUNIT_BASE)/include

# The main source file names you will need to test.
MAIN_FILES += Engine ActivityImpl

# The objects corresponding to the tested files.
MAIN_OBJ_PATH = $(addsuffix .o, $(addprefix $(SRC_PATH), $(MAIN_FILES)))

# ##################
# Compiler parameters.
# ##################

PREPROCESSOR_FLAGS += -I$(GUNIT_PATH) -I$(SRC_PATH)
COMPILER_FLAGS += -g -Wall

# ##################
# Testing parameters.
# ##################

# This is a list of tests. You would add the name of any new tests you add here
# and define it below.
FILES += EngineTest

# This is a list of object files.
OBJS = $(addsuffix .o, $(FILES))

# ##################
# Targets for each file you are testing.
# ##################
EngineTest: EngineTest.cpp $(SRC_PATH)/Engine.cpp $(SRC_PATH)/Engine.h $(SRC_PATH)/Engine.o
	$(CXX) $(PREPROCESSOR_FLAGS) $(COMPILER_FLAGS) -c EngineTest.cpp

# ##################
# Main Suite
# ##################

# This creates the final binary by pulling in each individual test defined
# above. It links the main google test library which has a main function that
# pulls in our tests and runs them.
main: $(FILES)
	$(CXX) $(PREPROCESSOR_FLAGS) $(COMPILER_FLAGS) $(MAIN_OBJ_PATH) $(GUNIT_BASE)/make/gtest_main.a -lpthread $(OBJS) -o UnitTest

all: main

clean:
	rm -rf *.o UnitTest

