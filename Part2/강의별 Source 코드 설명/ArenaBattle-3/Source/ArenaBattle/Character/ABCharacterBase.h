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
	//SetCharacterControlData��� �Լ��� �߰��ϰ� ���⼭�� ĳ���� ��Ʈ�� ������ ������ �Է����� �޷η� �����Ұ���.

	//enum�� �������� �� ���� ���� ������Ʈ�� �ٷ� ���� �� �ֵ��� ��(TMap)���� ������ ����
	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;
	//TMap�� Ű�� �ڷ������� �ؽ����̺� �����̶� ��������.
};
