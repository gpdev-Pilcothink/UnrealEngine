// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MineWidgetComponent.h"
#include "MineUserWidget.h"

void UMineWidgetComponent::InitWidget()
{
	Super::InitWidget();

	//InitWidget ���� �ν��Ͻ��� �������. ���� ��ü �������� ����
	UMineUserWidget* MineUserWidget = Cast<UMineUserWidget>(GetWidget());
	if (MineUserWidget)
	{
		MineUserWidget->SetOwningActor(GetOwner());
	}
}
