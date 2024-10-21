// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TFGameMode.h"

ATFGameMode::ATFGameMode()
{

	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/TheiosFlower/BluePrint/BP_TFCharacterPlayer.BP_TFCharacterPlayer_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/TheiosFlower.TFPlayerController"));
	if (PlayerControllerClassRef.Class)
	{

		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
