// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TFComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class THEIOSFLOWER_API UTFComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	UTFComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;
	//��Ÿ�� ���� �̸��� �����Ѵ�.

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;
	//�� ��� �޺� �׼��� �ִ��� ����

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;
	//�Է��� ������ �Է������ �����ϴ� ������ �����ؾ���
	//�������� ���� ��� �ӵ��� ����ٰ� �����Ͽ� ��Ȯ�� Ÿ�ֿ̹� üũ�� �Ͼ���� �����Ұ���.

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float> EffectiveFrameCount;

};