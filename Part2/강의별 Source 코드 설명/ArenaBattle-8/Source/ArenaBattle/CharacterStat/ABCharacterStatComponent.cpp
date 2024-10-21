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
	//���� �������� -�ϼ��� �ִµ�  ���� ActualDamage�� ���̳ʽ��� �ǰų� �츮�� ����Ҽ� �ִ� ������ �Ѿ�� �ʵ��� ActualDamage�� ����

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER) {
	//���� CurrentHp���� �츮�� ����ϴ� ������ ���� ������ �۴ٸ� ��� ���� ���¶�� �� �� �ִ�.
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);
	//�̷��� �ؼ� HP���� ������ ��ٸ� �ռ� ������ FOnHpZeroDelegate��� ��������Ʈ�� ����� �𸮾� ������Ʈ�� C++�Լ� �ƴϸ� �������Ʈ�Լ�����
	//���� HP�� ��������� �˾Ƽ� ó���϶�� ��ȣ�� ������ ��.

	OnHpChanged.Broadcast(CurrentHp);
	//HP������ �ɶ����� Ǫ���˸��� �ڵ����� ���� �Ǵµ� CurrentHp���� ���ڷ� �־��ָ� �ȴ�.
}




