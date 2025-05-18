// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MineAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CHAOSMINE_API UMineAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMineAnimInstance();

protected:

	/*****************
	* 이벤트 함수 설정
	******************/
	virtual void NativeInitializeAnimation() override; //처음 생성될 때 한번 호출

	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //프레임 마다 호출

	/*****************
	* 변수 설정
	******************/

	//AnimInstance 정보를 소유한 캐릭터 정보를 담음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class AMineCharacterBase> Owner;

	//CharacterMovement에 대한 객체포인터를 담음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	//값 변수들 설정
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	FVector Velocity; //현재 캐릭터의 속도를 보관

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed; //나는지 서있는지 앉아있는지 검사

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float ForwardValue; //땅에서의 속력

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float RightValue; //나는지 서있는지 앉아있는지 검사

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float SwimmingSpeed; //물속에서의 속력 (구현 가능시 활용할 변수임)

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1; // uint8s는 1바이트이지만 비트플래그로 인해 1비트만 사용.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsStanding : 1; //standing상태인지 검사.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1; //떨어지고 있는지 검사

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1; //점프하고 있는지 검사

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsCrouching : 1; //앉고 있는지 검사.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	uint8 bIsSwimming : 1; //수영하고 있는지 검사

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float MovingThreshold; //무빙중인지 검사함.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshold; //점프중인지 검사함.

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Character)
	float SwimmingThreshold; //점프중인지 검사함.

};
