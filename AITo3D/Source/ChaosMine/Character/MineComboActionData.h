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
	FString MontageSectionNamePrefix; //��Ÿ�� ���� �̸�

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount; //�޺� ����

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate; //������ ���� ��� �ӵ�

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount; //�Է��� ������ �ԷµǾ����� �����ϴ� ������
};
