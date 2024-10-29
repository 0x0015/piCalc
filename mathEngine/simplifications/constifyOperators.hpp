#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::shared_ptr<expr> constifyOperators(std::shared_ptr<expr> exp);
		std::shared_ptr<expr> unconstifyOverconstedOperators(std::shared_ptr<expr> exp);
	}
}
