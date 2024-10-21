// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/TFItemData.h"
#include "TFWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class THEIOSFLOWER_API UTFWeaponItemData : public UTFItemData
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditAnyWhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
};
