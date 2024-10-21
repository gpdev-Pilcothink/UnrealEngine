// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	
public:
	UABComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;
	//몽타주 섹션 이름을 지정한다.

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;
	//총 몇개의 콤보 액션이 있는지 지정

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;
	//입력이 사전에 입력됬는지를 감지하는 프레임 지정해야함
	//프레임의 기준 재생 속도를 여기다가 지정하여 정확한 타이밍에 체크가 일어나도록 설정할거임.

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float> EffectiveFrameCount;
	

};
