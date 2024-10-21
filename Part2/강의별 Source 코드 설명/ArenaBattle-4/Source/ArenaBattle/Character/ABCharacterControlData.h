// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UABCharacterControlData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	uint32 bUseControllerRotationYaw : 1;
	//앞서서 살펴본 폰 섹션에서 Control Rotation의 yaw값을 주로 체크한다. 따라서 해당 값만 체크 할지 안할지 결정하기 위한 변수 추가.

	/*Movement부분임*/

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bOrientRotationToMovement : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	uint32 bUseControllerDesiredRotation : 1;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;
	/*Movement부분임*/

	/*입력매핑 컨텍스트를 담을 멤버변수*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;
	//우리가 앞서서 설정한 컨트롤 데이터에는 각각의 컨트롤 데이터의 '인풋 매핑 컨텍스트'를 지정하게 되어있다.
	//따라서 이 컨트롤 데이터가 가지고 있는 인풋 매핑 컨텍스트를 런타임에서 바꿔치기 하도록 선언해볼것이다.
	/*입력매핑 컨텍스트를 담을 멤버변수*/

	/*스프링암에 관련된 여러가지 속성들을 지정할 수 있는 변수 설정*/

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bUsePawnControlRotation : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritPitch : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritYaw : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bInheritRoll : 1;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	uint32 bDoCollisionTest : 1;
	/*스프링암에 관련된 여러가지 속성들을 지정할 수 있는 변수 설정*/
	
};
