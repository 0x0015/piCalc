#pragma once
#include <optional>
#include "../parseResult.hpp"

namespace parser{
	namespace ptParse{
		std::optional<parseResult> parse(const std::string_view str);
	}
}
