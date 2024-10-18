#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::shared_ptr<expr> mergeCommutativeOperators(std::shared_ptr<expr> exp);
	}
}
