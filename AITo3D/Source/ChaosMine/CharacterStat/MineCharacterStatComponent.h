// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MineCharacterStatComponent.generated.h"

//HpBarWidget을 위한 델리게이트 선언.
DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CHAOSMINE_API UMineCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMineCharacterStatComponent();


public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	//HP가 변경이 되려면 무조건 SetHp를 통해 변경되도록 하자
	void SetHp(float NewHp);


	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	//Transient는 게임을 할때마다 디스크에 저장할 필요가 없는 변수임을 지정해 줌.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

		
};
