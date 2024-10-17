#include "simplify.hpp"
#include "simplifications/reduceRationals.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplify(std::shared_ptr<expr> exp){
	auto phase1 = simplification::reduceRationals(exp);

	return phase1;
}

mathEngine::equation mathEngine::simplify(const equation& eq){
	return {simplify(eq.lhs), simplify(eq.rhs)};
}

