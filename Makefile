OBJS	= mathEngine/constVal.cpp.o \
	  mathEngine/expr.cpp.o \
	  mathEngine/equation.cpp.o \
	  mathEngine/exprs/add.cpp.o \
	  mathEngine/exprs/constant.cpp.o \
	  mathEngine/exprs/exponent.cpp.o \
	  mathEngine/exprs/multiply.cpp.o \
	  mathEngine/exprs/variable.cpp.o \
	  mathEngine/exprs/derivative.cpp.o \
	  mathEngine/exprs/sine.cpp.o \
	  mathEngine/exprs/cosine.cpp.o \
	  mathEngine/simplify.cpp.o \
	  mathEngine/simplifications/reduceRationals.cpp.o \
	  mathEngine/simplifications/mergeCommutativeOperators.cpp.o \
	  mathEngine/simplifications/reduceBasicArithmatic.cpp.o \
	  mathEngine/simplifications/reduceSingleTermOps.cpp.o \
	  mathEngine/simplifications/evaluateDerivatives.cpp.o \
	  mathEngine/simplifications/reduceTrig.cpp.o \
	  parser/parseUtil.cpp.o \
	  parser/tokenize/basicTokenize.cpp.o \
	  parser/tokenize/mediumTokenize.cpp.o \
	  parser/tokenize/sourceDetails.cpp.o \
	  parser/ptParse/ptParse.cpp.o \
	  parser/ptParse/parseEquation.cpp.o \
	  parser/ptParse/parseExpression.cpp.o \
	  parser/ptParse/parseAdd.cpp.o \
	  parser/ptParse/parseMul.cpp.o \
	  parser/ptParse/parseConst.cpp.o \
	  parser/ptParse/parseDerivative.cpp.o \
	  parser/ptParse/parseTrig.cpp.o


OUT	= main
STATIC_OUT = piCalc.a
CXX	= g++
CC      = gcc
AR      = ar
CC_ACCEL = ccache
BUILD_CXX_FLAGS	 = -Wall -std=c++20 -g -Wno-reorder
BUILD_CC_FLAGS   =
LINK_OPTS	 = 

all: $(OBJS) main.cpp.o
	$(CC_ACCEL) $(CXX) $(OBJS) main.cpp.o -o $(OUT) $(LINK_OPTS)

static: $(OBJS)
	$(AR) rvs $(STATIC_OUT) $(OBJS)

%.cpp.o: %.cpp
	$(CC_ACCEL) $(CXX) $< $(BUILD_CXX_FLAGS) -g -c -o $@

%.c.o: %.c
	$(CC_ACCEL) $(CXX) $< $(BUILD_CXX_FLAGS) -g -c -o $@

clean:
	rm -f $(OBJS) $(OUT) $(STATIC_OUT) main.cpp.o
