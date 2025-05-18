// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MineGameMode.h"


AMineGameMode::AMineGameMode()
{
	/*DefaultPawnClass
	 static ConstructorHelpers::FClassFinder<APawn> FirstPersonClassRef(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C"));
	 if(FirstPersonClassRef.Class)
	 {
		DefaultPawnClass= FirstPersonClassRef.Class;
	 }
	 */


	 static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/ChaosMine/ViewPerson/BP_MineCharacterPlayer.BP_MineCharacterPlayer_C'"));
	 if (DefaultPawnClassRef.Class)
	 {
		 DefaultPawnClass = DefaultPawnClassRef.Class;
	 }


	//PlayerControllerClass
	static ConstructorHelpers::FClassFinder<APlayerController> FirstPersonControllerClassRef(TEXT("/Script/ChaosMine.MinePlayerController"));
	if(FirstPersonControllerClassRef.Class)
	{
		PlayerControllerClass= FirstPersonControllerClassRef.Class;
	}


}
