// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/) //���⿡���� ����� ���� HP���� ������ ��ü�鿡�� �������� float�̶�� �Ѱ��� ���ڰ��� �����Ұ���.


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UABCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

/*Tick�� ȣ������ �ʰ� �ʿ��� ���� ȣ���ϵ��� �ִ��� ���ϸ� ���̵��� ���� �غ� ����.*/


public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	//��������Ʈ ���� �ΰ� ����.

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp;}
	float ApplyDamage(float InDamage);


protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat) 
	//�̷��� �Ӽ����� �⺻��������, 
	//�츮�� ��ġ�� ĳ���͸��� �ٸ� ���� ������ ������
	//�ν��Ͻ����� �����μ���Ǿ�� �Ѵ� ���� VisibleInstanceOnly��� Ű���� ��� 
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	//Transient�� �츮�� �̷� ���� ������Ʈ��� ������Ʈ�� ������ �� �̷��� �Ӽ����� ��� ��ũ�� ����ȴ�.
	//�׷��� ���� HP ���� ��쿡�� ������ �� ������ ���Ӱ� ������ �Ǳ� ������ ��ũ�� ������ �ʿ䰡 ���� ���� �ִ�.
	//���� �̷��� ��ũ�� ������ �ʿ䰡 ���ٸ� Transient�� �����Ͽ� ���ǿ��� ������ ������� �ʵ��� ���� ����.
	float CurrentHp;

		
};
