#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::shared_ptr<expr> reduceRationals(std::shared_ptr<expr> exp);
	}
}
