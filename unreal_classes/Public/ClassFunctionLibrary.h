// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/SplineComponent.h"

#include "LSystem.h"
#include "ClassFunctionLibrary.generated.h"


/**
 * 
 */
UCLASS()
class TREEGENPLUGIN_API UClassFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) 
	static UObject* GetClassDefaultObject(UClass* c);


	UFUNCTION(BlueprintCallable) 
	static void GetTreeSplineCoordinates(AActor* tree);
};
