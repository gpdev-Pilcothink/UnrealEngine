// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS()
class OBJECTREFLETION_API UPerson : public UObject
{
	GENERATED_BODY()
public:
	UPerson(); //기본값 설정하기 위핸 생성자 코드 추가.

	UFUNCTION()
	virtual void DoLesson();

	const FString& GetName() const;
	void SetName(const FString& InName);

	//외부에서 프로퍼티의 코드로 접근하기 위해 getter와 setter 선언부임
	//const로 레퍼런스를 반환하도록 하고 변경할것이 아니니깐 const 지시자를 명확하게 쓰자.

protected: //앞으로 스튜던트와 티처가 상속받을 속성을 만들것임.
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int32 Year;

private:
	
};
