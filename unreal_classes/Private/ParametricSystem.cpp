#include "ParametricSystem.h"
#include "MeshCoordBuilder.h"

ParametricSystem::ParametricSystem(TEnumAsByte<Species> species)
{
	this->species = (Species)species;
	this->branches.Add(Branch());
	this->width_reduction_factor = 0.05;
}

TArray<Branch> ParametricSystem::GrowBranches() {

	TArray<Branch*> active_buds;
	for (auto& branch : branches) {
		if (branch.state == ACTIVE) {
			active_buds.Add(&branch);
		}
	}

	int iterations = 5;
	SpeciesParams specimen = speciesTable[species];
	int growth_rate = round(specimen.GR);

	// add all internodes to branches
	for (int i = 0; i < iterations; i++ ) {
		for (auto& branch : active_buds) {

			for (int node = 0; node < growth_rate; node++) {
				branch->AddNode();
			}
			auto width = branch->start_width;
			branch->end_width = width / (width * width_reduction_factor);
			// TODO - branching patterns
		}
	}

	CalculateCoords();

	return MergeLists();
}

void ParametricSystem::CalculateCoords() {
	ParametricMeshBuilder mb = ParametricMeshBuilder(speciesTable[species]);

	for (int i = 0; i < branches.Num(); i++) {
		mb.CalcBranch(branches[i]);
	}
}

TArray<Branch> ParametricSystem::MergeLists()
{
	TArray<Branch> merged;
	Branch current_branch;

	while (branches.Num() > 0) {
		current_branch = branches[0];
		branches.RemoveAt(0);

		int idx = 0;
		while (idx < branches.Num()) {
			auto end_node = current_branch.nodes.Last().coordinates;

			if (FVector::PointsAreSame(end_node, branches[idx].nodes[0].coordinates)) {

				current_branch.nodes.Append(branches[idx].nodes.GetData() + 1, branches[idx].nodes_count);
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