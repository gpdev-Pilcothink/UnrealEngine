// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MineAnimInstance.h"
#include "Character//MineCharacterPlayer.h"
#include "GameFramework//Character.h"
#include "GameFramework//CharacterMovementComponent.h"

UMineAnimInstance::UMineAnimInstance()
{
	/**********************
	* 임계값 설정
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
		* 무브번트 기본 설정
		******************/
		// 1) 캐릭터 이동 속도
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();

		// 2) 캐릭터가 바라보는 기준 축 추출 (전방 / 우측 벡터)
		const FVector Forward = Owner->GetActorForwardVector();
		const FVector Right = Owner->GetActorRightVector();
	
		// 3) 기본 설정
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

		// 4) 카메라 설정
		/*********************************
		* 내가 하고자 하는 것
		* 서있는 상태에서는 뒷걸음이나 옆걷기 등이 존재함
		* 수영이나 crouch상태에서는 일반적인 경우가 아니니깐 buseControllRotationYaw를 false 그리고 bOrientRotationToMovement는 true로 하자.
		*********************************/
		if(bIsStanding)
		{	//standing && idle상태 ==> 마음대로 캐릭 구경
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
		else //Swimming이나 Crouch상태인경우임.
		{
			Movement->bOrientRotationToMovement = true;
			Owner->bUseControllerRotationYaw=false;
		}

		// 5) 전방축
		if(!bIsCrouching) //앉았을때 속도
		{
			ForwardValue = FVector::DotProduct(Velocity, Forward) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::Sprint);
		}
		else //일어났을때 속도
		{
			ForwardValue = FVector::DotProduct(Velocity, Forward) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::CrouchSprint);
		}

		// 6) 우측축
		if (!bIsCrouching) //앉았을때 속도
		{
			RightValue = FVector::DotProduct(Velocity, Right) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::Sprint);

		}
		else //일어났을때 속도
		{
			RightValue = FVector::DotProduct(Velocity, Right) / Owner->GetMovement(ESpeedGroup::Basic, EMovementType::CrouchSprint);
		}

	}
}
