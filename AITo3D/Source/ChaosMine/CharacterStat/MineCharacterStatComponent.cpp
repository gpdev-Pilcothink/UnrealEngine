// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/MineCharacterStatComponent.h"

// Sets default values for this component's properties
UMineCharacterStatComponent::UMineCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}

// Called when the game starts
void UMineCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);

}

float UMineCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage); //허용 범위 조절

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) //죽은 상태
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UMineCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}


