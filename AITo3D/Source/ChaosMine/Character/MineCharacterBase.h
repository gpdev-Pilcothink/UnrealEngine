// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Map.h"
#include "GameFramework/Character.h"
#include "Interface/MineAnimationAttackInterface.h"
#include "Interface/MineCharacterWidgetInterface.h"
#include "Interface/MineCharacterItemInterface.h"
#include "MineCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMineCharacter, Log, All);

class UNiagaraSystem;
class UNiagaraComponent;


// Movement 그룹 (기본, 저속, 고속)
UENUM(BlueprintType)
enum class ESpeedGroup : uint8
{
	Basic UMETA(DisplayName = "Basic"),
	Low   UMETA(DisplayName = "Low"),
	High  UMETA(DisplayName = "High")
};

// 이동 타입 (걷기, 달리기, 앉은 걷기, 앉은 달리기, 수영, 수영달리기, 점프강도)
UENUM(BlueprintType)
enum class EMovementType : uint8
{
	Walk          UMETA(DisplayName = "Walk"),
	Sprint        UMETA(DisplayName = "Sprint"),
	CrouchWalk    UMETA(DisplayName = "CrouchWalk"),
	CrouchSprint  UMETA(DisplayName = "CrouchSprint"),
	Swim          UMETA(DisplayName = "Swim"),
	SwimSprint    UMETA(DisplayName = "SwimSprint"),
	JumpZVelocity UMETA(DisplayName = "JumpZVelocity")
};


//인칭 설정
UENUM()
enum class ECharacterControlType : uint8
{
	FirstPerson,
	ThirdPerson
};


//아이템을 위한 델리게이트 선언(배열 관리를 위해 구조체도 만들어줌)
DECLARE_DELEGATE_OneParam(FOnTakeItemDelegate, class UMineItemData* /*InItemData*/);
USTRUCT(BlueprintType)
struct FTakeItemDelegateWrapper
{
	GENERATED_BODY()
	FTakeItemDelegateWrapper() { }
	FTakeItemDelegateWrapper(const FOnTakeItemDelegate& InItemDelegate) : ItemDelegate(InItemDelegate) {}
	FOnTakeItemDelegate ItemDelegate;
};

UCLASS()
class CHAOSMINE_API AMineCharacterBase : public ACharacter, public IMineAnimationAttackInterface, public IMineCharacterWidgetInterface, public IMineCharacterItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMineCharacterBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void SetCharacterControlData(const class UMineCharacterControlData* CharacterControlData);


	UPROPERTY(EditAnyWhere, Category = CharacterControl, Meta = (AllowPrivateAccess = "true"));
	TMap<ECharacterControlType, class UMineCharacterControlData *> CharacterControlManager;

	
/****************************************************
* 캐릭터 무브먼트 기본값 설정 (캡슐화)
*****************************************************/
public:
	/***********************************
	 * Getter / Setter - 속도 데이터
	 ***********************************/

	 // Getter: 그룹과 이동타입에 해당하는 속도를 반환
	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovement(ESpeedGroup Group, EMovementType Type) const
	{
		const TPair<ESpeedGroup, EMovementType> Key(Group, Type);
		if (const float* Found = MovementMap.Find(Key))
		{
			return *Found;
		}
		return 0.f; // 기본값
	}

	// Setter: 그룹과 이동타입에 해당하는 속도를 설정
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovement(ESpeedGroup Group, EMovementType Type, float NewSpeed)
	{
		MovementMap.Add(TPair<ESpeedGroup, EMovementType>(Group, Type), NewSpeed);
	}

	
private:
	/***********************************
	* 움직임 데이터 TMap
	* Key: (SpeedGroup, MovementType)
	* Value: 속도 값 (float)
	***********************************/
	TMap<TPair<ESpeedGroup, EMovementType>, float> MovementMap;


// Combo Action Section
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ComboActionMontage; //콥보 액션 몽타주
	UPROPERTY(EditAnyWhere, blueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"));
	TObjectPtr<class UMineComboActionData> ComboActionData; //콤보 액션 데이터

	void ProcessComboCommand();

	/**************************콤보액션 설정**************************
	*ComboActionEnd는 몽타주에 설정된 델리게이트를 통해 바로 호출될 수 있도록 파라미터를 맞춰볼거임.
	*이렇게 델리게이트를 통해 파라미터를 맞추게 되면 이벤트 기반이 되기 때문에 콜백함수처럼 작동함 
	*따라서 직접 체크할 필요가 없기에 리소스 낭비도 줄일 뿐더러 코드가 단순해짐.
	****************************************************************/
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded); 
	void SetComboCheckTimer(); //타이머를 발동시킬 함수.
	void ComboCheck();//타이머가 작동시 입력을 체크하는 함수
	
	/****************************************************
	* FTimerHandle이라고 하는 구조체를 선언하면 언리얼 엔진 워드에서 제공되는 타이머 기능을 활용하여
	* 원하는 시간에 특정함수를 호출하도록 설정 가능.(연속적인 액션을 하거나 중간에 끊기 위함.)
	* HasnextComboCommand는 발동한 타이머 이전에 입력 커맨드가 들어왔는지 점검하는 Bool값임.
	* 내부 로직에서만 사용하기에 UPROPERTY를 사용하지 않음.
	****************************************************/
	int32 CurrentCombo = 0; //콤보의 진행 상황(0이면 아직 콤보가 시작도 안함.)
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;



/******************************************************
* Attack Hit Section
******************************************************/
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



/******************************************************
* Dead Section
******************************************************/
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	virtual void SetDead(); //죽는 상태
	void PlayDeadAnimation(); //죽는 몽타주 재생.

	float DeadEventDelayTime = 5.0f;


/******************************************************
* Stat Section
******************************************************/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMineCharacterStatComponent> Stat;



/******************************************************
* UI Widget Section
******************************************************/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMineWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(class UMineUserWidget* InUserWidget) override;

/******************************************************
* Item Section
******************************************************/

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TArray<FTakeItemDelegateWrapper> TakeItemActions; //TakeItemActions 에는 3가지 함수가 바인딩 될거임.


	virtual void TakeItem(class UMineItemData* InItemData) override;
	virtual void DrinkPotion(class UMineItemData* InItemData);
	virtual void EquipWeapon(class UMineItemData* InItemData);
	virtual void ReadScroll(class UMineItemData* InItemData);


/******************************************************
* Niagara Section
******************************************************/
public:
	static class UNiagaraSystem* SwordFireFX;
	UPROPERTY(Transient)
	UNiagaraComponent* CurrentTrailFX = nullptr;
};