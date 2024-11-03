#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::optional<std::pair<std::shared_ptr<mathEngine::expr>, std::shared_ptr<mathEngine::expr>>> getTrigVals(std::shared_ptr<expr> exp);
		std::shared_ptr<expr> reduceTrig(std::shared_ptr<expr> exp);
	}
}
