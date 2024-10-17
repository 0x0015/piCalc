#pragma once
#include "equation.hpp"

namespace mathEngine{
	std::shared_ptr<expr> simplify(std::shared_ptr<expr> exp);
	equation simplify(const equation& eq);
}
