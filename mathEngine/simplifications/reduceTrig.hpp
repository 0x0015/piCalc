#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		std::optional<std::shared_ptr<expr>> simplifySin(std::shared_ptr<expr> exp);
		std::optional<std::shared_ptr<expr>> simplifyCos(std::shared_ptr<expr> exp);
		std::shared_ptr<expr> reduceTrig(std::shared_ptr<expr> exp);
	}
}
