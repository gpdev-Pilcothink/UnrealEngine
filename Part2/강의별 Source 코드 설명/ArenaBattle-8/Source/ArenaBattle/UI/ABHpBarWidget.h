// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	//UUserWidget같은 경우에는 일반적인 생산자가 아니라 FObjectInitializer라는 인자를 하나 가진 생성자만 지원한다.

public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected: 
	virtual void NativeConstruct() override;//UUserWidget에서 제공함.
	
public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) {MaxHp = NewMaxHp;}
	void UpdateHpBar(float NewCurrentHp);
	//MaxHp가 설정이 됬다면 현재 HP상태에 따라서 Hp바의 비율을 계산하여 업데이트 해야한다. 

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	//위젯이 초기화될 때 HpProgressBar를 가져와서 포인터를 가져오는 기능 추가해줘야한다 아니면 Null값이라 동작 안함.

	UPROPERTY()
	float MaxHp;
	//이 MaxHp값은 우선 위젯에서는 음수로 지정함. 
};
