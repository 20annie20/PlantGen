#include "LSystem.h"
#include "MeshCoordBuilder.h"

LSystem::LSystem(const FString& initial_word, const TMap<TCHAR, TArray<FString>>& rules, uint64 iterations, const FString& ignore_chars)
{
	this->iterations = iterations;
	this->produced_word = initial_word;
	this->axiom = initial_word;
	this->rules = rules;
	this->ignore_chars = ignore_chars;
}

FString LSystem::produce_word(bool do_ignore)
{
	FString new_word = "";
	if (rules.IsEmpty()) {
		return produced_word;
	}
	while (iterations > 0) {
		new_word = "";
		for (size_t i = 0; i < produced_word.Len(); ++i) {
			auto symbol = produced_word[i];
			if (rules.Contains(symbol)) {
				auto applicable_rules = rules[symbol];
				if (applicable_rules.Num() > 0) {
					auto rule = applicable_rules[rand() % applicable_rules.Num()];
					new_word += apply_rule(produced_word, rule, i, do_ignore);
					continue;
				}
			}
			new_word += symbol;
		}
		iterations--;
		produced_word = new_word;
	}

	return produced_word;
}

FString LSystem::apply_rule(const FString& word, const FString& rule, int idx, bool do_ignore)
{
	//TCHAR left_context;
	//TCHAR right_context;
	FString LeftS, RightS;
	const FString rule_split = "=";
	bool was_found = rule.Split(rule_split, &LeftS, &RightS);
	return was_found == true ? RightS : rule;
}

TArray<TArray<FVector>> LSystem::join_branches_for_splines(const float angle, const float scale)
{
	MeshBuilder mb = MeshBuilder(angle, scale);
	Parser parser = Parser(angle, scale, &mb);
	TArray<TArray<FVector>> spline_coords;

	spline_coords.Add(TArray<FVector>({ (UE::Math::TVector(0.0)) }));

	int branch_idx = 0;
	for (size_t i = 0; i < produced_word.Len(); ++i) {
		auto symbol = produced_word[i];
		auto operation = parser.map_symbol(symbol);
		operation();
		if (symbol == '[' && spline_coords[branch_idx].Num() > 0) {
			branch_idx++;
			spline_coords.Add(TArray<FVector>({ mb.get_curr_pos()}));
		}
		else if(symbol == ']') {
			branch_idx++;
			spline_coords.Add(TArray<FVector>({ mb.get_curr_pos() }));
		}
		else {
			spline_coords[branch_idx].Add(mb.get_curr_pos());
		}
	}
	auto merged = this->merge_lists(spline_coords);
	return spline_coords;
}

TArray<TArray<FVector>> LSystem::merge_lists(TArray<TArray<FVector>> spline_coords)
{
	TArray<TArray<FVector>> merged;
	TArray<FVector> current_list;

	while (spline_coords.Num() > 0) {
		current_list = spline_coords[0];
		spline_coords.RemoveAt(0);
		int idx = 0;
		while (idx < spline_coords.Num()) {
			auto var = current_list.Last();
			if (FVector::PointsAreSame(var, spline_coords[idx][0])) {
				current_list.Append(spline_coords[idx].GetData() + 1, spline_coords[idx].Num() - 1);
				spline_coords.RemoveAt(idx);
				idx = 0;
			}
			else {
				idx++;
			}
		}
		merged.Add(current_list);
	}

	return merged;
}
