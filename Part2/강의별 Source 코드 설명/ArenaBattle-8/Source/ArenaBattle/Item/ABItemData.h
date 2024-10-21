// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"


UENUM(BlueprintType) //블루프린트와 호환될 수 있게 지정
enum class EItemType : uint8
{
	Weapon =0,
	Potion,
	Scroll
};



/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Type)
	EItemType Type ;
};
