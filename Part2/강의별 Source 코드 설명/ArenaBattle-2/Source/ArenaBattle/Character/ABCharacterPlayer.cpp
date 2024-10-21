// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

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
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ArenaBattle/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArenaBattle/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

}

void AABCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	//이것도 마찬가지로 컨트롤러가 플레이어 컨트롤러 즉 플레이어를 대상으로 설계된 전용 캐릭터 클래스이기 때문에 CastChecked를 써줘야 한다.

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		//여기서 Subsystem이라고 하는 인풋시스템을 가져와서 매핑컨텍스트 에셋을 추가를 해준다.
		Subsystem->AddMappingContext(DefaultMappingContext, 0);//=> 여기에 우선순위르 맨끝에 지정하여 다양한 입력이 겹치더라도 우선순위가 높은 입력들에 대해서 액션들이 바인딩에서 수행할 수 있도록 지정할 수 있다.
		//Subsystem->RemoveMappingContext(DefaultMappingContext); ==>언제나 런타임에서 RemoveMappingContext함수를 통해 빼거나 추가하거나 자유롭게 조절이 가능하다.
	}
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
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AABCharacterPlayer::Look);
}
/* 위 함수는 관련 헤더를 3개 추가해야 한다.
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
*/

void AABCharacterPlayer::Move(const FInputActionValue& Value)
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

void AABCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

