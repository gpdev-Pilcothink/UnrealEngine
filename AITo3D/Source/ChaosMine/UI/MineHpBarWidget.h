// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MineUserWidget.h"
#include "MineHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineHpBarWidget : public UMineUserWidget
{
	GENERATED_BODY()
	
public:
	UMineHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	
	UPROPERTY()
	float MaxHp;
};
