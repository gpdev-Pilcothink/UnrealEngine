// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	//��� ���� �Լ��� �� super::�Լ��� �����ؾ��Ѵ�.
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	//FInputModeGameOnly�� ����ü��.
	SetInputMode(GameOnlyInputMode);
}
