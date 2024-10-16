#pragma once
#include <variant>
#include <memory>
#include "rational.hpp"

namespace mathEngine{
	enum constantName{
		PI = 0,
		E = 1
	};
	constexpr double constantDoubleValues[] = {
		3.14159265358979323846264338327950288419716939937510582097494459,
		2.71828182845904523536028747135266249775724709369995957496696762
	};//entries match up with the constantNames
	constexpr std::string_view constantLatexNames[] = {
		"\\pi",
		"e"
	};
	class expr;
	class constVal{
		public:
		using valueType = std::variant<double, rational, constantName, std::shared_ptr<const expr>>;
		valueType value;
		double toDouble() const;
		std::string toLatex() const;
	};
	constVal operator+(const constVal& a, const constVal& b);
	constVal operator*(const constVal& a, const constVal& b);
	//the following don't really exist as unique operations, but are useful as shorthands
	constVal operator-(const constVal& a, const constVal& b);
	constVal operator/(const constVal& a, const constVal& b);
}
