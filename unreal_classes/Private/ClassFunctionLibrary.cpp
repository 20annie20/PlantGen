#include "ClassFunctionLibrary.h"

UObject* UClassFunctionLibrary::GetClassDefaultObject(UClass* c)
{
	return c->GetDefaultObject();
}

void UClassFunctionLibrary::GetTreeSplineCoordinates(AActor* tree)
{
	UE_LOG(LogTemp, Warning, TEXT("Generating Splines."));

	FString initial_tree = "X";
	float angle = 45.0;
	int iterations = 5;
	float scale = 1.0f / iterations;

	TMap<TCHAR, TArray<FString>> rules = {
		{ 'X', {"F[+X][-X]FX"}},
		{ 'F', {"FF"}}
	};

	LSystem simulator = LSystem(initial_tree, rules, iterations);

	FString word = simulator.produce_word();
	TArray<TArray<FVector>> branches = simulator.join_branches_for_splines(angle, scale);
	
	int it = 0;
	for (auto branch : branches)
	{
		if (it > 100) break;
		
		if (branch.Num() > iterations ) {
			USplineComponent* spline = NewObject<USplineComponent>(tree);
			spline->ClearSplinePoints();
			it++;
			for (auto point : branch) {
				spline->AddSplineLocalPoint(point);
			}
		}
	}
}
	
