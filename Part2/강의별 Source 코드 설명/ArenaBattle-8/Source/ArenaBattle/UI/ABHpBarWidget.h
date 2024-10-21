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
	//UUserWidget���� ��쿡�� �Ϲ����� �����ڰ� �ƴ϶� FObjectInitializer��� ���ڸ� �ϳ� ���� �����ڸ� �����Ѵ�.

public:
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected: 
	virtual void NativeConstruct() override;//UUserWidget���� ������.
	
public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) {MaxHp = NewMaxHp;}
	void UpdateHpBar(float NewCurrentHp);
	//MaxHp�� ������ ��ٸ� ���� HP���¿� ���� Hp���� ������ ����Ͽ� ������Ʈ �ؾ��Ѵ�. 

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;
	//������ �ʱ�ȭ�� �� HpProgressBar�� �����ͼ� �����͸� �������� ��� �߰�������Ѵ� �ƴϸ� Null���̶� ���� ����.

	UPROPERTY()
	float MaxHp;
	//�� MaxHp���� �켱 ���������� ������ ������. 
};
