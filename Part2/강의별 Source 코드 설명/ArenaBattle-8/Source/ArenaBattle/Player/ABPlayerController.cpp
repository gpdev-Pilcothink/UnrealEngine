// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	//상속 받은 함수라 꼭 super::함수를 구현해야한다.
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	//FInputModeGameOnly은 구조체다.
	SetInputMode(GameOnlyInputMode);
}
