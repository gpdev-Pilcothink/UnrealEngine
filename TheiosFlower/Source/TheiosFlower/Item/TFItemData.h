// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TFItemData.generated.h"

UENUM(BlueprintType)//��������Ʈ�� ȣȯ�� �� �ְ� ����
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};


/**
 * 
 */
UCLASS()
class THEIOSFLOWER_API UTFItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
	
};