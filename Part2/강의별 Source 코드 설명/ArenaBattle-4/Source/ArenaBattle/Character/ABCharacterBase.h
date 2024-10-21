// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"


UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

public:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);
	//SetCharacterControlData라는 함수를 추가하고 여기서는 캐릭터 컨트롤 데이터 에셋을 입력으로 받로록 설정할것임.

	//enum을 바탕으로 두 가지 에셋 오브젝트를 바로 얻어올 수 있도록 맵(TMap)으로 데이터 선언
	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;
	//TMap은 키와 자료형으로 해시테이블 느낌이라 생각하자.
};
