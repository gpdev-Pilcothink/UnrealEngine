// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ABCharacterControlData.h"

AABCharacterPlayer::AABCharacterPlayer()
{
	//카메라 관련
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	/*
	스프링암의 경우 루트 컴포넌트에 부착함.
	길이는4m
	CameraBoom->bUsePawnControlRotation = true;는 회전에 대한 설정으로 이후 카메라를 다룰때 자세히 설명
	*/


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	/*
	카메라 컴포넌트는 생성을하고 카메라 지지대인 스프링 암에 부착을 함.
	부착을 할때 특정 위치를 지정하는것이 아닌 SocketName이라고 하는 이름 지시자를 지정해준다면 스프링 암의 끝에 자동으로 달라붙게 됨.
	소켓 이름은 F12키를 눌러 직접 확인 가능하다.
	*/

	// Input
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ChangerControl.IA_ChangerControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}


	CurrentCharacterControlType = ECharacterControlType::Quater;
	

}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	SetCharacterControl(CurrentCharacterControlType);
	//SetCharacterControl를 호출하여 쿼터뷰 컨트롤로 바꿔치기 하도록 변경하면 된다.
}



void AABCharacterPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	//여기서 입력을 받을때 앞선 슬라이드에서 EnhancedInputComponent를 사용한다 라고 지정했다. 따라서 만약에 이것이 사용되지 않은 경우가 있을수도 있는데 
	//사용되지 않는경우 CastChecked를 사용하여 반드시 EnhancedInputComponent를 사용하도록 코드를 만듬.
	//성공적으로 캐스팅 되었다면 이제 앞서서 지정한 언리얼 오브젝트들과 Move와 Look함수에 대해서 바인딩을 진행하고 
	//JumpAction의 경우엔 실제로 캐릭터가 제공하는 Jump함수와 StopJumping함수를 직접 연결하여 관련된 액션을 수행ㅌ하도록 연결을 시켜 준다.
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ChangerCharaceterControl);
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderMove);
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::ShoulderLook);
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::QuaterMove);
}
/* 위 함수는 관련 헤더를 3개 추가해야 한다.
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
*/

void AABCharacterPlayer::ChangerCharaceterControl()
{
	//V키를 눌러 컨트롤을 변경하는 함수의 경우 ENUM값을 검사하여 반대쪽 ENUM으로 변경되도록 간단하게 구현후 자세한건 SetCharacterControl여기서 구현

	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void AABCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	UABCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl); //컨트롤 데이터가 반드시 있어야 하기에 check함수 써주자.

	SetCharacterControlData(NewCharacterControl);

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController()); //컨트롤러 반드시 가져와야 해서 CastChecked 사용함.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//우리가 실제 인풋 매핑 컨텍스트를 운영하는 Subsystem을 가져온다.
		Subsystem->ClearAllMappings(); //바꿔치기 하기전에 기존에 있던 모든매핑을 지워준다.
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}

	CurrentCharacterControlType = NewCharacterControlType;
}

void AABCharacterPlayer::SetCharacterControlData(const UABCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);

	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}


void AABCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	//FInputActionValue에서 X와 Y값을 가져운 후에 이를 무브먼트 컴포넌트와 연결하여 실질적으로 캐릭터를 이동시키거나 
	//컨트롤러의 회전을 설정함으로써 스프링암이 해당 컨트롤러를 바라보도록 설정하였다.
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AABCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void AABCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	//FInputActionValue에서 X와 Y값을 가져운 후에 이를 무브먼트 컴포넌트와 연결하여 실질적으로 캐릭터를 이동시키거나 
	//컨트롤러의 회전을 설정함으로써 스프링암이 해당 컨트롤러를 바라보도록 설정하였다.
	FVector2D MovementVector = Value.Get<FVector2D>(); //현재 무브먼트 받기
	float InputSizeSquared = MovementVector.SquaredLength();
	float MovementVectorSize = 1.0f; //크기 1로 조절
	float MovementVectorSizeSquared = MovementVector.SquaredLength();
	if (MovementVectorSizeSquared > 1.0f)
	{
		MovementVector.Normalize();
		MovementVectorSizeSquared = 1.0f;
	}
	else
	{
		MovementVectorSize = FMath::Sqrt(MovementVectorSizeSquared);
	}

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f); //X와 Y값을 Modifiers로 조절했기 때문에 그대로 둬도 됨.
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator()); 
	//ControlRotation을  MoveDirection(foward방향)으로 지정해주면 무브먼트 컴포넌트에서 설정한 옵션에 의해 캐릭터가 자동으로 이동하는 방향으로 회전하게 된다.
	AddMovementInput(MoveDirection, MovementVectorSize);
}


