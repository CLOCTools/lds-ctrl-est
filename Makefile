BUILD_DIR				:= ./build
LIB_DIR 				:= $(BUILD_DIR)/lib
BIN_DIR 				:= $(BUILD_DIR)/bin
OBJ_DIR  				:= $(BUILD_DIR)/objects
OBJ_DIR_STATIC  := $(BUILD_DIR)/static-objects

SRC							:= $(wildcard src/*.cpp)
OBJECTS					:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)
OBJECTS_STATIC	:= $(SRC:%.cpp=$(OBJ_DIR_STATIC)/%.o)

LIB_NAME 					:= libldsCtrlEst
LIB_VERSION_MAJOR	:= 0
LIB_VERSION_MINOR	:= 4.0
LIB_DYNAMIC				:= $(LIB_NAME).so.$(LIB_VERSION_MAJOR).$(LIB_VERSION_MINOR)
LIB_STATIC				:= $(LIB_NAME).a.$(LIB_VERSION_MAJOR).$(LIB_VERSION_MINOR)

# For static library, going to build openblas INTO the library rather than linking it dynamically because this wreaks all sorts of havoc when using with matlab!!
# Took me a few days to come to this solution, but if the binaries are built into a static library. For some reason, dynamically linking against gfortran is still okay.
# n.b., OpenBLAS must have been compiled with -fPIC.
# Probably easiest to grab/compile static library then extract objects (ar -x).
OPENBLAS_OBJECTS_DIR := build/static-objects/openblas

# Figure out OS and architecture
OS	:= $(shell uname)
CXX	= g++

CXXFLAGS := $(CXXFLAGS) -O3 -std=c++11 -fPIC -Iinclude

ifneq ($(OS),Darwin)
CXXFLAGS := $(CXXFLAGS) -Wl,--export-dynamic
endif

ifneq ($(OS),Darwin)
INSTALL_PRE := /usr
else
INSTALL_PRE := /opt/local
endif

# not going to use the armadillo wrapper for the static library, but instead build openblas into binary.
CXXFLAGS_STATIC	:= $(CXXFLAGS) $(shell pkg-config --cflags armadillo matio) -DARMA_DONT_USE_WRAPPER

# armadillo- and matio-specific stuff
# armadillo is used heavily throughout; matio is only used in the test programs for saving matlab variables.
CXXFLAGS	:= $(CXXFLAGS) $(shell pkg-config --cflags armadillo matio)
LDFLAGS		:= $(LDFLAGS) $(shell pkg-config --libs armadillo matio)
#LDFLAGS		:= $(LDFLAGS) $(-Wl,-rpath,/usr/local/lib -L/usr/local/lib)
all: build $(LIB_DIR)/$(LIB_DYNAMIC)
all-static: build $(LIB_DIR)/$(LIB_STATIC)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

$(OBJ_DIR_STATIC)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS_STATIC) -o $@ -c $<

$(LIB_DIR)/$(LIB_DYNAMIC): $(OBJECTS)
	@mkdir -p $(@D)

ifneq ($(OS),Darwin)
	$(CXX) -shared \
	$(CXXFLAGS) \
	-o $(LIB_DIR)/$(LIB_DYNAMIC) $(OBJECTS) \
	$(LDFLAGS)
else
	$(CXX) -dynamiclib \
	$(CXXFLAGS) \
	-Wl,-install_name,"@rpath/$(LIB_DYNAMIC)" \
	-o $(LIB_DIR)/$(LIB_DYNAMIC) $(OBJECTS) \
	$(LDFLAGS)
endif

	# make the symlinked versions...
	ln -f -s $(LIB_DYNAMIC) $(LIB_DIR)/$(LIB_NAME).so.$(LIB_VERSION_MAJOR)
	ln -f -s $(LIB_DYNAMIC) $(LIB_DIR)/$(LIB_NAME).so

$(LIB_DIR)/$(LIB_STATIC): $(OBJECTS_STATIC)
	@mkdir -p $(@D)

ifneq ($(OS),Darwin)
	ar -rsc $(LIB_DIR)/$(LIB_STATIC) $(OBJECTS_STATIC) $(OPENBLAS_OBJECTS_DIR)/*.o
else
	ar -rsc $(LIB_DIR)/$(LIB_STATIC) $(OBJECTS_STATIC)
endif

	# make the symlinked versions...
	ln -f -s $(LIB_STATIC) $(LIB_DIR)/$(LIB_NAME).a.$(LIB_VERSION_MAJOR)
	ln -f -s $(LIB_STATIC) $(LIB_DIR)/$(LIB_NAME).a

test_plds_est: test/test_plds_est.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(shell pkg-config --cflags ldsCtrlEst) test/test_plds_est.cpp -o $(BIN_DIR)/test_plds_est $(LDFLAGS) $(shell pkg-config --libs ldsCtrlEst)

test_plds_logLinCtrl: test/test_plds_logLinCtrl.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(shell pkg-config --cflags ldsCtrlEst) test/test_plds_logLinCtrl.cpp -o $(BIN_DIR)/test_plds_logLinCtrl $(LDFLAGS) $(shell pkg-config --libs ldsCtrlEst)

test_plds_switched_logLinCtrl: test/test_plds_switched_logLinCtrl.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(shell pkg-config --cflags ldsCtrlEst) test/test_plds_switched_logLinCtrl.cpp -o $(BIN_DIR)/test_plds_switched_logLinCtrl $(LDFLAGS) $(shell pkg-config --libs ldsCtrlEst)

test_glds_ctrl: test/test_glds_ctrl.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(shell pkg-config --cflags ldsCtrlEst) test/test_glds_ctrl.cpp -o $(BIN_DIR)/test_glds_ctrl $(LDFLAGS) $(shell pkg-config --libs ldsCtrlEst)

build:
	@mkdir -p $(LIB_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR_STATIC)

clean:
	-@rm -rvf $(OBJECTS)
	-@rm -rvf $(OBJECTS_STATIC)
	-@rm -rvf $(LIB_DIR)/*

install:
	cp -f -a $(LIB_DIR)/$(LIB_NAME)* $(INSTALL_PRE)/lib/
	cp -rf include/ldsCtrlEst* $(INSTALL_PRE)/include/
ifneq ($(OS),Darwin)
	cp ldsCtrlEst.linux.pc $(INSTALL_PRE)/lib/pkgconfig/ldsCtrlEst.pc
	cp ldsCtrlEst-static.linux.pc $(INSTALL_PRE)/lib/pkgconfig/ldsCtrlEst-static.pc
else
	cp ldsCtrlEst.macOS.pc $(INSTALL_PRE)/lib/pkgconfig/ldsCtrlEst.pc
	cp ldsCtrlEst-static.macOS.pc $(INSTALL_PRE)/lib/pkgconfig/ldsCtrlEst-static.pc
endif

uninstall:
	rm -rf $(INSTALL_PRE)/lib/$(LIB_NAME)*
	rm -rf $(INSTALL_PRE)/include/ldsCtrlEst*
	rm -f $(INSTALL_PRE)/lib/pkgconfig/ldsCtrlEst.pc
	rm -f $(INSTALL_PRE)/lib/pkgconfig/ldsCtrlEst-static.pc

.PHONY: all build clean install uninstall test_plds_est test_plds_logLinCtrl test_glds_ctrl test_plds_switched_logLinCtrl

print-%  : ; @echo $* = $($*)
