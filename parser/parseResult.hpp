#pragma once
#include <unordered_set>
#include <memory>
#include "../mathEngine/equation.hpp"

namespace parser{
	struct parseResult{
			std::unordered_set<std::string> vars;
			std::variant<mathEngine::equation, std::shared_ptr<mathEngine::expr>> value;
	};
}
