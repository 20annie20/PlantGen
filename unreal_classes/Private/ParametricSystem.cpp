#include "ParametricSystem.h"
#include "MeshCoordBuilder.h"
#include "TreeGenPluginModule.h"
#include <random>

#define UE_LOG_NAMESPACE TreeGen

ParametricSystem::ParametricSystem(TEnumAsByte<Species> species, int tree_age)
{
	this->species = (Species)species;
	this->branches.Add(Branch()); // start with apical branch - root
	this->width_reduction_factor = 0.1;
	this->tree_age = tree_age; // maximum tree cycles
}

TArray<Branch> ParametricSystem::GrowBranches() {

	TArray<Branch*> active_buds;
	for (auto& branch : branches) {
		if (branch.state == ACTIVE) {
			active_buds.Add(&branch);
		}
	}

	SpeciesParams specimen = speciesTable[species];
	int growth_rate = round(specimen.GR);

	// add all internodes to branches
	for (int i = 0; i < tree_age; i++ ) {
		TArray<Branch> new_branches;
		for (int32 idx = 0; idx < branches.Num(); idx++) { // check branches
			Branch& branch = branches[idx];
			for (auto& node : branch.nodes) {
				node.age++;
				if (node.age == specimen.MBA) {
					int32 bud_count = 0;
					// check alive buds to spawn branches
					for (auto& b : node.buds)
					{
						if (b) {
							Branch new_branch = Branch(&branch, idx);
							UE_LOG(TreeGen, Log, TEXT("Creating new branch from %d"), idx);
							new_branches.Add(new_branch); ///< Zapisujesz adres lokalnej zmiennej new_branch.
							bud_count++;
						}
					}
					UE_LOG(TreeGen, Log, TEXT("Branch bud count = %d"), bud_count);
				}
				if (node.age < specimen.MBA) {
					// kill buds with probability
					for (auto& b : node.buds)
					{
						float randomValue = FMath::FRand();
						if (randomValue < specimen.LKP) {
							b = false;
						}
					}
				}
			}

			if (branch.state == ACTIVE) { // grow only living branches
				// check if kill branch
				if (specimen.AKP > 0) {
					float randomValue = FMath::FRand();
					if (randomValue < specimen.AKP) {
						branch.state = STALE;
						continue;
					}
					
				}
				for (int node = 0; node < growth_rate; node++) {
					branch.AddNode(specimen.NLB); // also adds buds to the new nodes
				}
			}
		}
		for (auto& branch : new_branches) {
			branches.Add(branch);
		}
		new_branches.Empty();
	}

	CalculateCoords();

	return MergeLists();
}

void ParametricSystem::CalculateCoords() {
	ParametricMeshBuilder mb = ParametricMeshBuilder(speciesTable[species], tree_age);

	for (int i = 0; i < branches.Num(); i++) {
		auto parent_idx = branches[i].parent_idx;
		mb.CalcBranch(branches[i], branches[parent_idx]);
	}
}

TArray<Branch> ParametricSystem::MergeLists()
{
	TArray<Branch> merged;
	Branch current_branch;

	while (branches.Num() > 0) {
		current_branch = branches[0];
		branches.RemoveAt(0); ///< Je¿eli Branch::parent jest pointerem do tej arrayki, to tutaj siê psuje.

		int idx = 0;
		while (idx < branches.Num()) {
			auto end_node = current_branch.nodes.Last().coordinates;

			if (FVector::PointsAreSame(end_node, branches[idx].nodes[0].coordinates)) {

				current_branch.nodes.Append(branches[idx].nodes.GetData() + 1, branches[idx].nodes.Num() - 1);
				branches.RemoveAt(idx);
				idx = 0;
			}
			else {
				idx++;
			}
		}
		merged.Add(current_branch);
	}

	return merged;
}