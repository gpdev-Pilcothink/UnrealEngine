// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "InputActionValue.h"
#include "ABCharacterPlayer.generated.h"


/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABCharacterPlayer : public AABCharacterBase
{
	GENERATED_BODY()
	
public:
	AABCharacterPlayer();

protected:
	virtual void BeginPlay() override;
	//BeginPlay()는 여기서 입력 매핑 컨텍스트를 할당하는 역할을 진행할것임

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//SetupPlayerInputComponent는 언리얼 엔진의 Input System에서 입력액션과 move와 Look 함수를 여기서 매핑 시켜주는것을 미리 완료해줄것임.
	//여기까지 해서 입력 설정을 만들어주고 실질적으로 이것을 키보드를 통해서 입력을 받을지 키패드를 통해서 받을지는 BeginPlay의 DefaultMappingContext를 통해서 진행시켜 줄것임.


// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class USpringArmComponent> CameraBoom;
	//스프링암이라고 카메라를 지탱해주는 지지대 역할을 하는 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UCameraComponent> FollowCamera;
	//이건 진짜 카메라 컴포넌트 

	/*윗 두 컴포넌트에 보면 Meta라고 하는 추가 지정자를 추가로 선언했는데 
	이 경우에는 private로 선언한 언리얼 오브젝트 객체들을 블루프린트로 접근 가능하도록 해주는 특별한 지시자이다. */


// Input Section
protected:
	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnyWhere, BluePrintReadOnly, Category = Input, Meta=(AllowPrivateAccess = "true"));
	TObjectPtr<class UInputAction> LookAction;
	/*전체적인 설정은 비슷한데 이 에셋들의 경우에는 다른 에셋으로 변경 할 수 있도록 설계하기 위해 지정자를 EditAnyWhere로 설정함
	한가지 매핑컨텍스트와 세가지 액션이 있는데 이것들은 3인칭 템플릿이 제공하는것을 그대로 사용함.*/

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& value);
	//FInputActionValue를 받기 위해서는 #include "InputActionValue.h" 헤더추가해야함.
	//이후 ArenaBattle.Build.cs에 가서 EnhancedInput 모듈 추가 해줘야 작동함.

};
