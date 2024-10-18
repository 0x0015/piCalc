#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::shared_ptr<expr> reduceBasicArithmatic(std::shared_ptr<expr> exp);
	}
}
