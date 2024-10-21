// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/) //여기에서는 변경된 현재 HP값을 구독한 객체들에게 보내도록 float이라는 한가지 인자값을 설정할거임.


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

/*Tick을 호출하지 않고 필요한 때만 호출하도록 최대한 부하를 줄이도록 구현 해볼 것임.*/


public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	//델리게이트 변수 두개 선언.

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp;}
	float ApplyDamage(float InDamage);


protected:
	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat) 
	//이러한 속성들은 기본값이지만, 
	//우리가 배치한 캐릭터마다 다른 값을 가지기 때문에
	//인스턴스마다 별도로수향되어야 한다 따라서 VisibleInstanceOnly라는 키워드 사용 
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	//Transient는 우리가 이런 스탯 컴포넌트라는 오브젝트를 저장할 때 이러한 속성들이 모두 디스크에 저장된다.
	//그런데 현재 HP 같은 경우에는 게임을 할 때마다 새롭게 지정이 되기 때문에 디스크에 저장할 필요가 없을 수도 있다.
	//만약 이렇게 디스크에 저장할 필요가 없다면 Transient를 지정하여 불피요한 공간이 낭비되지 않도록 지정 가능.
	float CurrentHp;

		
};
