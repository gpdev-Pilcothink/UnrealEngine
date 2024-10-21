// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"

/*
처음에 ABPlaterController를 게임모드에 적용하기 위해 다음 밑의 헤더를 추가해줬겠지만 이제 경로를 통해 클래스를 직접 받아옴으로써 필요 없어짐
#include "Player/ABPlayerController.h"
*/

AABGameMode::AABGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	if (ThirdPersonClassRef.Class != nullptr)
	{
		DefaultPawnClass = ThirdPersonClassRef.Class;
	}
	/*AABGamemode의 생성자 구현부에는 멤버변수의 값을 설정해줘야 한다.*/

	//DefaultPawnClass 값을 먼저 설정해줘야함
	//그 다음에 PlayerControllerClass값을 설정해주면 됨
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}


}
