// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TFHpBarWidget.h"
#include "Components/ProgressBar.h" //�� ����� UMG��⿡�� ������
#include "Interface/TFCharacterWidgetInterface.h"


UTFHpBarWidget::UTFHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //�θ� Ŭ��������  ObjectInitializer���� �״�� �Ѱ��ָ� ��.
{
	MaxHp = -1.0f;
	//�� MaxHp���� �켱 ���������� ������ ������. 
	//MaxHp�� ������ �ȵǾ� �ִٸ� Hp�ٸ� ǥ���ϴµ� �������� ������ ���� ������ �ʱ�ȭ �ɴ� �ݵ�� �ùٸ� ������ �����ϵ��� �츮�� �ȳ��� ����� �Ѵ�.
}

void UTFHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	//GetWidgetFromName����Ͽ� �̸����� ã��
	ensure(HpProgressBar);
	//�ش� ��Ʈ���� �ݵ�� �־�� �ϱ⿡ ensure�� Ȯ������

	ITFCharacterWidgetInterface* CharacterWidget = Cast< ITFCharacterWidgetInterface>(OwingActor);
	//OwingActor�� �ش� �������̽��� ����ȯ �Ǵ��� ����.
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

	

	/*
	* ���� ��� ������ OwingActor��� ���� �������ٰ�
	* ���� ���� ������ UpdateHpBar��� �ϴ� �Լ������� �����ؼ� �츮�� ��������Ʈ�� ��Ͻ�Ű��
	* ������ ������ ������Ʈ �� ������ �� �Լ��� UpdateHpBar�� ȣ��Ǹ鼭 HpBar�� ���ϰ� �� ���̴�.
	*/
	/*
	* �⺻ ������ �������� �� ���� ������ �ڱ⸦ ������ ���� ������ �������
	* ���� �����κ��� UpdatehpBar��� ��� �Լ��� ����ϴ� ������ �߰��ؾ� �Ѵ�.
	* �̸� ���ؼ� ABCharacterClass�� ���� �����ϸ� �������� �߻���.
	* ���� �̷��� ���� ������Ʈ���� ĳ���Ϳ� �����ϰ� �پ��� ĳ���Ϳ� ����� �� �ֵ��� �������̽��� �ϳ� �������ְ� �̰��� ���� ��������.
	*/
}

void UTFHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		//UMG����� ��쿡�� �⺻������ ������ �ȵǾ��־ ���� �������� �����ؾ��Ѵ�.
	}
}