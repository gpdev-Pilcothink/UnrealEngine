// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MineWidgetComponent.h"
#include "MineUserWidget.h"

void UMineWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//InitWidget 이후 인스턴스가 만들어짐. 따라서 객체 가져오기 가능
	UMineUserWidget* MineUserWidget = Cast<UMineUserWidget>(GetWidget());
	if (MineUserWidget)
	{
		MineUserWidget->SetOwningActor(GetOwner());
	}
}
