#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::optional<std::shared_ptr<expr>> evaluateIntegral(std::shared_ptr<expr> exp, std::string_view wrtVar);
		std::shared_ptr<expr> evaluateIntegrals(std::shared_ptr<expr> exp);
	}
}
