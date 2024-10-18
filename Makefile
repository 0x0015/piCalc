OBJS	= main.cpp.o\
	  mathEngine/constVal.cpp.o \
	  mathEngine/equation.cpp.o \
	  mathEngine/exprs/add.cpp.o \
	  mathEngine/exprs/constant.cpp.o \
	  mathEngine/exprs/exponent.cpp.o \
	  mathEngine/exprs/multiply.cpp.o \
	  mathEngine/exprs/variable.cpp.o \
	  mathEngine/simplify.cpp.o \
	  mathEngine/simplifications/reduceRationals.cpp.o \
	  mathEngine/simplifications/reduceBasicArithmatic.cpp.o \
	  parser/parseUtil.cpp.o \
	  parser/tokenize/basicTokenize.cpp.o \
	  parser/tokenize/mediumTokenize.cpp.o \
	  parser/tokenize/sourceDetails.cpp.o \
	  parser/ptParse/ptParse.cpp.o \
	  parser/ptParse/parseEquation.cpp.o \
	  parser/ptParse/parseExpression.cpp.o \
	  parser/ptParse/parseAdd.cpp.o \
	  parser/ptParse/parseMul.cpp.o \
	  parser/ptParse/parseConst.cpp.o


OUT	= main
CXX	= g++
CC      = gcc
CC_ACCEL = ccache
BUILD_CXX_FLAGS	 = -Wall -std=c++20 -g -Wno-reorder
BUILD_CC_FLAGS   =
LINK_OPTS	 = 

all: $(OBJS)
	$(CC_ACCEL) $(CXX) $(OBJS) -o $(OUT) $(LINK_OPTS)

%.cpp.o: %.cpp
	$(CC_ACCEL) $(CXX) $< $(BUILD_CXX_FLAGS) -g -c -o $@

%.c.o: %.c
	$(CC_ACCEL) $(CXX) $< $(BUILD_CXX_FLAGS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT)
