#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::shared_ptr<expr> reduceSingleTermOps(std::shared_ptr<expr> exp);
	}
}
