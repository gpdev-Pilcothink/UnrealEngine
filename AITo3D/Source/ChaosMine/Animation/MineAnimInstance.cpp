// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MineAnimInstance.h"
#include "Character//MineCharacterPlayer.h"
#include "GameFramework//Character.h"
#include "GameFramework//CharacterMovementComponent.h"

UMineAnimInstance::UMineAnimInstance()
{
	/**********************
	* �Ӱ谪 ����
	**********************/
	MovingThreshold = 3.0f; 
	JumpingThreshold = 100.0f;
	SwimmingThreshold = 100.0f;
}

void UMineAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AMineCharacterPlayer>(GetOwningActor());
	if(Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UMineAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if(Movement)
	{
		/******************
		* �����Ʈ �⺻ ����
		******************/
		// 1) ĳ���� �̵� �ӵ�
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();

		// 2) ĳ���Ͱ� �ٶ󺸴� ���� �� ���� (���� / ���� ����)
		const FVector Forward = Owner->GetActorForwardVector();
		const FVector Right = Owner->GetActorRightVector();
	
		// 3) �⺻ ����
		bIsIdle = GroundSpeed < MovingThreshold;
		bIsSwimming = false;
		bIsFalling = Movement-> IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshold);
		bIsCrouching = Owner->bIsCrouched;
		if(!bIsSwimming && !bIsCrouching)
			bIsStanding = true;
		else
			bIsStanding = false;
		//UE_LOG(LogTemp, Warning, TEXT("bIsStanding: %d, bIsCrouching: %d, bIsSwimming: %d"), bIsStanding, bIsCrouching, bIsSwimming);

		// 4) ī�޶� ����
		/*********************************
		* ���� �ϰ��� �ϴ� ��
		* ���ִ� ���¿����� �ް����̳� ���ȱ� ���� ������
		* �����̳� crouch���¿����� �Ϲ����� ��찡 �ƴϴϱ� buseControllRotationYaw�� false �׸��� bOrientRotationToMovement�� true�� ����.
		*********************************/
		if(bIsStanding)
		{	//standing && idle���� ==> ������� ĳ�� ����
			if (bIsIdle)
			{
				Movement->bOrientRotationToMovement = false;
				Owner->bUseControllerRotationYaw = false;
			}
			else
			{
				Movement->bOrientRotationToMovement = false;
				Owner->bUseControllerRotationYaw = true;
			}
		}
		else //Swimming�̳� Crouch�����ΰ����.
		{
			Movement->bOrientRotationToMovement = true;
			Owner->bUseControllerRotationYaw=false;
		}

		// 5) ������
		if(!bIsCrouching) //�ɾ����� �ӵ�
		{
			ForwardValue = FVector::DotProduct(Velocity, Forward) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::Sprint);
		}
		else //�Ͼ���� �ӵ�
		{
			ForwardValue = FVector::DotProduct(Velocity, Forward) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::CrouchSprint);
		}

		// 6) ������
		if (!bIsCrouching) //�ɾ����� �ӵ�
		{
			RightValue = FVector::DotProduct(Velocity, Right) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::Sprint);

		}
		else //�Ͼ���� �ӵ�
		{
			RightValue = FVector::DotProduct(Velocity, Right) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::CrouchSprint);
		}

	}
}
