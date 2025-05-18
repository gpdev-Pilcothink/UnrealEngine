// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MineItemData.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	weapon = 0,
	Potion,
	Scroll
};




/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
	
};
