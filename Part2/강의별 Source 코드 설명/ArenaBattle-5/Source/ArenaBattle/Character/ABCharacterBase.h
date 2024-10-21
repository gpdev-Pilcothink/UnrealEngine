// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ABCharacterBase.generated.h"


UENUM()
enum class ECharacterControlType : uint8
{
	Shoulder,
	Quater
};


UCLASS()
class ARENABATTLE_API AABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AABCharacterBase();

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

};
