#include "simplify.hpp"
#include "simplifications/reduceRationals.hpp"
#include "simplifications/mergeCommutativeOperators.hpp"
#include "simplifications/reduceBasicArithmatic.hpp"
#include "simplifications/reduceSingleTermOps.hpp"
#include "simplifications/evaluateDerivatives.hpp"
#include "simplifications/reduceTrig.hpp"
#include "../mathEngine/expr.hpp"

template<auto fn> std::shared_ptr<mathEngine::expr> applyUntilStabilization(std::shared_ptr<mathEngine::expr> exp){
	std::shared_ptr<mathEngine::expr> output = exp;
	std::size_t oldHash = exp->hash();
	while(true){
		output = fn(output);
		std::size_t newHash = exp->hash();
		if(oldHash == newHash)
			return output;
		oldHash = newHash;
	}
}

std::shared_ptr<mathEngine::expr> mathEngine::simplify(std::shared_ptr<expr> exp){
	auto phase1 = applyUntilStabilization<simplification::reduceRationals>(exp);
	auto phase2 = applyUntilStabilization<simplification::mergeCommutativeOperators>(phase1);
	auto phase3 = applyUntilStabilization<simplification::reduceBasicArithmatic>(phase2);
	auto phase4 = applyUntilStabilization<simplification::reduceSingleTermOps>(phase3);
	auto phase5 = applyUntilStabilization<simplification::reduceTrig>(phase4);
	auto phase6 = applyUntilStabilization<simplification::evaluateDerivatives>(phase5);

	return phase6;
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

