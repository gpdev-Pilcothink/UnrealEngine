// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ABAnimationAttackinterface.h"
#include "Interface/ABCharacterWidgetInterface.h"
#include "ABCharacterBase.generated.h"


UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter, public IABAnimationAttackInterface, public IABCharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();
	
	/*
	* 캐릭터에서 셋업을 종료하는 시점에서 Begin Play가 실행되기전인 Stat의 델리게이트를 등록하여 죽었을때 죽는 모션을 수행하도록 처리하자. 
	*/
	virtual void PostInitializeComponents() override; //begin play에서 구현하거나 생성자에서 바인딩 가능


public:
	virtual void SetCharacterControlData(const class UABCharacterControlData* CharacterControlData);
	//SetCharacterControlData라는 함수를 추가하고 여기서는 캐릭터 컨트롤 데이터 에셋을 입력으로 받로록 설정할것임.

	//enum을 바탕으로 두 가지 에셋 오브젝트를 바로 얻어올 수 있도록 맵(TMap)으로 데이터 선언
	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterControlType, class UABCharacterControlData*> CharacterControlManager;
	//TMap은 키와 자료형으로 해시테이블 느낌이라 생각하자.


/*콤보 액션 부분*/
protected:

	UPROPERTY(EditAnyWhere,BluePrintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Animation, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABComboActionData> ComboActionData;
	//데이터 에셋하도록 변수 추가.

	void ProcessComboCommand();
	//입력을 통해서 커맨드가 지시가 나오면 이것을 통해서 몽타주를 재생시켜 볼 것이다.

	void ComboActionBegin();
	//몽타주가 시작될 때 호출하는 ComboActionBegin함수
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	//몽타주가 모두 종료되었을때 호출하는  ComboActionBegin함수
	//이때 ComboActionBegin의 경우 몽타주에 설정된 델리게이트를 통해서 바로 호출될 수 있도록 파라미터[(class UAnimMontage* TargetMontage, bool IsProperlyEnded)]를 맞출거임.

	void SetComboCheckTimer();//타이머를 발동시킬 SetComboCheckTimer함수 선언
	void ComboCheck();//입력이 들어왔는지 안들어왔는지를 체크하는 ComboCheck라는 함수 선언

	int32 CurrentCombo = 0;
	//현재 콤보가 어디까지 진행되는지를 저장하기 위해서 integer형으로 CurrentCombo라는 값을 설정할거임.
	//내부에서만 사용할것이기에 별도의 UPROPERTY()는 붙이지 않을것임.
	//콤보가 0 이면 아직 콤보가 시작되지 않은것이고 1이상이면 콤보가 시작된것임.
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;
	//HasnextComboCommand는 내부 로직구현에서만 사용하기에 UPROPERTY는 붙이지 않음.
	//따라서 지난시간과 같이 Integer로 선언할 필요가 없다.


//Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//언리얼엔진 액터 설정에 TakeDamage라는 함수가 있음 [이미 액터에서 구현이 되어 있어서 오버라이드 받아 구현 하면 됨.]


//Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;
	//앞서서 설정한 Dead몽타주를 선언함

	virtual void SetDead();
	//죽는 상태를 구현하는 함수
	void PlayDeadAnimation();


	float DeadEventDelayTime = 5.0f;


//Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABCharacterStatComponent> Stat;

//UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UABWidgetComponent> HpBar;


	virtual void SetupCharacterWidget(class UABUserWidget* InUserWidget) override;





};
