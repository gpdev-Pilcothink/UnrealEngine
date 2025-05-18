// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MineHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/MineCharacterWidgetInterface.h"

UMineHpBarWidget::UMineHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UMineHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IMineCharacterWidgetInterface* CharacterWidget = Cast<IMineCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

}

void UMineHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);

	}
}
