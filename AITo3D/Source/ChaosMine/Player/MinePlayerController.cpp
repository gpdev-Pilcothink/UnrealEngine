// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MinePlayerController.h"

void AMinePlayerController::BeginPlay()
{
	Super::BeginPlay();

	//mouse Cursor enter in viewport
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}
