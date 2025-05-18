// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MineComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

public:
	UMineComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix; //몽타주 섹션 이름

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount; //콤보 개수

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate; //프레임 기준 재생 속도

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount; //입력이 사전에 입력되었는지 감지하는 프레임
};
