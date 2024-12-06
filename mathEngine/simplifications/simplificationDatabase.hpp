#pragma once
#include "../equation.hpp"

namespace mathEngine{
	namespace simplification{
		void loadSimplificationDatabase(const std::string_view filename);
		std::shared_ptr<expr> simplifyByDatabase(std::shared_ptr<expr> exp);
	}
}
