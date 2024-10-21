// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ABCharacterStatComponent.h"

// Sets default values for this component's properties
UABCharacterStatComponent::UABCharacterStatComponent()
{
	MaxHp = 200.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UABCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
	
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);
	//들어온 데미지가 -일수도 있는데  만약 ActualDamage가 마이너스가 되거나 우리가 허용할수 있는 범위를 넘어서지 않도록 ActualDamage로 조정

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) {
	//만약 CurrentHp값이 우리가 허용하는 굉장히 작은 값보다 작다면 사실 죽은 상태라고 볼 수 있다.
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	//이렇게 해서 HP값이 변동이 됬다면 앞서 설정한 FOnHpZeroDelegate라는 델리게이트에 등록한 언리얼 오브젝트나 C++함수 아니면 블루프린트함수에게
	//내가 HP가 변경됬으니 알아서 처리하라고 신호를 날리면 됨.

	OnHpChanged.Broadcast(CurrentHp);
	//HP변경이 될때마다 푸리알림이 자동으로 가게 되는데 CurrentHp값을 인자로 넣어주면 된다.
}




