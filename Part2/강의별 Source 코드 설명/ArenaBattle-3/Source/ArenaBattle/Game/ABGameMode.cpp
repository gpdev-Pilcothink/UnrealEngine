// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ABGameMode.h"

/*
ó���� ABPlaterController�� ���Ӹ�忡 �����ϱ� ���� ���� ���� ����� �߰���������� ���� ��θ� ���� Ŭ������ ���� �޾ƿ����ν� �ʿ� ������
#include "Player/ABPlayerController.h"
*/

AABGameMode::AABGameMode()
{

	/****1������ ������ Pawn ���� ���****/

	//static ConstructorHelpers::FClassFinder<APawn> ThirdPersonClassRef(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C"));
	//if (ThirdPersonClassRef.Class != nullptr)
	//{
	//	DefaultPawnClass = ThirdPersonClassRef.Class;
	//}
	/*AABGamemode�� ������ �����ο��� ��������� ���� ��������� �Ѵ�.*/

	/****1������ ������ Pawn ���� ���****/


	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/ArenaBattle.ABCharacterPlayer"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass=DefaultPawnClassRef.Class;
	}



	//DefaultPawnClass ���� ���� �����������
	//�� ������ PlayerControllerClass���� �������ָ� ��
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/ArenaBattle.ABPlayerController"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}


}
