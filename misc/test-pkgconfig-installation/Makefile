CXX = g++
CXXFLAGS = -std=c++11 -fPIC -O2
CXXFLAGS := $(CXXFLAGS) $(shell pkg-config ldsCtrlEst --cflags)

LDFLAGS = $(shell pkg-config ldsCtrlEst --libs)

# $(OBJ_DIR)/%.o: %.cpp
# 	@mkdir -p $(@D)
# 	$(CXX) $(CXXFLAGS) -o $@ -c $<

test: src/main.o
	mkdir -p build
	$(CXX) $(CXXFLAGS) -o build/test src/main.o $(LDFLAGS)

src/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ -c $<
