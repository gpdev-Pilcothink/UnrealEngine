// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/MineItemData.h"
#include "MineWeaponItemData.generated.h"







/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineWeaponItemData : public UMineItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
