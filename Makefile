CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -fsanitize=address

SCIPDIR = /home/vercee/libraries/SCIPOptSuite-9.2.3-Linux
INCLUDES = -I$(SCIPDIR)/include -I$(SCIPDIR)/src
LIBS = -L$(SCIPDIR)/lib -lscip -lmetis -lm -ldl -lz
RPATH = -Wl,-rpath,$(SCIPDIR)/lib

TARGET = schedule
SRC = schedule.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) $(INCLUDES) $(LIBS) $(RPATH) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
