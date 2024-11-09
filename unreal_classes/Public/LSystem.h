# pragma once

#include "CoreMinimal.h"
#include "Parser.h"
#include "Containers/Deque.h"

class LSystem {
	uint64 iterations;
	TMap<TCHAR, TArray<FString>> rules;
	FString axiom;
	FString produced_word;
	FString ignore_chars;

public:
	LSystem(const FString& initial_word, const TMap<TCHAR, TArray<FString>>& rules, uint64 iterations, const FString& ignore_chars="");

	FString produce_word(bool do_ignore=false);

	FString apply_rule(const FString& word, const FString& rule, int idx, bool ignore);

	TArray<TArray<FVector>> join_branches_for_splines(const float angle, const float scale);

	TArray<TArray<FVector>> merge_lists(TArray<TArray<FVector>> spline_coords);
};