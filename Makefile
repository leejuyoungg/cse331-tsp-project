# Makefile (project-root/)
CXX      := g++
CXXFLAGS := -std=c++17 -O2 -I src

SRC      := src/io.cpp src/mst2x.cpp
TEST_SRC := experiments/test_mst2x.cpp
TEST_BIN := experiments/test_mst2x

.PHONY: all clean run

all: $(TEST_BIN)

$(TEST_BIN): $(SRC) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 데이터 디렉터리에 있는 xql662.tsp와 xql662.opt.tour를 사용해 실행
run: all
	cd experiments && ./test_mst2x ../data/xql662.tsp ../data/xql662.opt.tour

clean:
	rm -f $(TEST_BIN)