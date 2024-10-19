#include "simplify.hpp"
#include "simplifications/reduceRationals.hpp"
#include "simplifications/mergeCommutativeOperators.hpp"
#include "simplifications/reduceBasicArithmatic.hpp"
#include "simplifications/reduceSingleTermOps.hpp"
#include "simplifications/evaluateDerivatives.hpp"
#include "../mathEngine/expr.hpp"

std::shared_ptr<mathEngine::expr> mathEngine::simplify(std::shared_ptr<expr> exp){
	auto phase1 = simplification::reduceRationals(exp);
	auto phase2 = simplification::mergeCommutativeOperators(phase1);
	auto phase3 = simplification::reduceBasicArithmatic(phase2);
	auto phase4 = simplification::reduceSingleTermOps(phase3);
	auto phase5 = simplification::evaluateDerivatives(phase4);

	return phase5;
}

std::shared_ptr<mathEngine::expr> mathEngine::fullySimplify(std::shared_ptr<expr> exp){
	std::size_t oldHash = exp->hash();
	while(true){
		exp = simplify(exp);
		std::size_t newHash = exp->hash();
		if(oldHash == newHash)
			return exp;
		oldHash = newHash;
	}
}

mathEngine::equation mathEngine::simplify(const equation& eq){
	equation eq_res;
	eq_res.lhs = simplify(eq.lhs);
	eq_res.rhs = simplify(eq.rhs);
	return eq_res;
}

mathEngine::equation mathEngine::fullySimplify(const equation& e){
	equation eq = e;
	std::size_t oldHash = eq.hash();
	while(true){
		eq = simplify(eq);
		std::size_t newHash = eq.hash();
		if(oldHash == newHash)
			return eq;
		oldHash = newHash;
	}
}

