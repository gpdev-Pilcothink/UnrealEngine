// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ABAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UABAnimInstance::UABAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
}

void UABAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	//GetOwningActor함수를 사용하면 현재 AnimInstance를 소유하고 있는 액터정보를 가져온다.
	//액터타입으로 받아와 캐릭터인지 알수 없기에 Cast함수를 통해 캐릭터로 형변환 한다.
	/*
	액터를 캐릭터형으로 변환을 위해 헤더 추가 해야한다
	#include "GameFramework/Character.h"
	#include "GameFramework/CharacterMovementComponent.h"
	*/

	if (Owner)//형변환이 성공적이면 null값이 아니게 바로 들어옴
	{
		Movement = Owner->GetCharacterMovement();
		//GetCharacterMovement를 통해 캐릭터 무브먼트에 대한 값을 가져올 수 있다.
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();//Z값을 제외한 xy의 크기만 구하여 지면의 속력을 구하면 된다.
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
	//위 5가지 속성은 매 프레임마다 계속 반복이 된다.
}