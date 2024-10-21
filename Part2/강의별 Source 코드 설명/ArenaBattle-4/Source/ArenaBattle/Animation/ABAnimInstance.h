// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UABAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	//NativeInitializeAnimation()함수는 AnimInstance가 처음 생설될때 한번 호출

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//NativeUpdateAnimation함수는 프레임마다 호출된다.

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;
	//AnimInstance를 소유하고 있는 캐릭터의 정보를 담고 있는 Owner 변수 추가

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;
	//캐릭터 무브먼트 컴포넌트에 대한 객체 포인터를 담는 Movement 변수 추가.

	/*여기부터 값 변수 추가 VisibleAnywhere->EditAnywhere로 교체*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	//현재 캐릭터의 속도를 보관할 수 있는 Velocity 변수 추가

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;
	//땅에서의 속력을 지정할 수 있는 GroundSpeed값 지정

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;
	//현재 Idle상태인지 나타내기 위한 불리언값 지정
	//불타입은 사이즈를 모르기 때문에 언리얼 엔진은 인테저형으로 선언하되 앞에 접두사 'b'를 붙이고 비트 플래그를 추가로 달아준다.

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;
	//움직이고 있는지 쉬고있는지 점검하는 MovingThreshould 추가

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;
	//떨어지고 있는지 검사하는 bIsFalling 변수 추가

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;
	//현재 점프하고 있는지를 검색하는 bIsJumping 변수 추가

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	//현재 점핑중인지 나타내는 JumpingThreshould 변수 추가.
};
