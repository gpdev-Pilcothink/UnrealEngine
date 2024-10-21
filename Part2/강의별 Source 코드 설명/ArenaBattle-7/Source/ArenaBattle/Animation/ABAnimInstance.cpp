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
	//GetOwningActor�Լ��� ����ϸ� ���� AnimInstance�� �����ϰ� �ִ� ���������� �����´�.
	//����Ÿ������ �޾ƿ� ĳ�������� �˼� ���⿡ Cast�Լ��� ���� ĳ���ͷ� ����ȯ �Ѵ�.
	/*
	���͸� ĳ���������� ��ȯ�� ���� ��� �߰� �ؾ��Ѵ�
	#include "GameFramework/Character.h"
	#include "GameFramework/CharacterMovementComponent.h"
	*/

	if (Owner)//����ȯ�� �������̸� null���� �ƴϰ� �ٷ� ����
	{
		Movement = Owner->GetCharacterMovement();
		//GetCharacterMovement�� ���� ĳ���� �����Ʈ�� ���� ���� ������ �� �ִ�.
	}
}

void UABAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();//Z���� ������ xy�� ũ�⸸ ���Ͽ� ������ �ӷ��� ���ϸ� �ȴ�.
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
	//�� 5���� �Ӽ��� �� �����Ӹ��� ��� �ݺ��� �ȴ�.
}