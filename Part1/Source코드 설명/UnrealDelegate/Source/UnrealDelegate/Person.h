// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Person.generated.h"

/**
 * 
 */
UCLASS()
class UNREALDELEGATE_API UPerson : public UObject
{
	GENERATED_BODY()
	
public:
	UPerson();
	
	FORCEINLINE const FString& GetName() const { return Name; }
	// GetName() 뒤에 const지시자를 붙여주는게 좋지만 쓰게 된다면 문제가 생기는데 왜냐하면 변경하지 않겠다고 const를 설정했는데 리턴값을 레퍼런스(&)로 받고 있다.
	// 리턴값을 레퍼런스로 받으면 받은측에서 변경이 가능하기 때문에 const지시자가 없다.
	//레퍼런스로 반환할때 const로 지시하면 FString 앞에도 const로 반환해주어야 한다.
	FORCEINLINE void SetName(const FString& InName) { Name = InName; }

	FORCEINLINE class UCard* GetCard() const { return Card;}
	FORCEINLINE void SetCard(class UCard* InCard) { Card = InCard; }

protected:
	UPROPERTY()
	FString Name;

	//카드 객체를 소유하도록 지정하기
	//Card* Card;
	/*
	우리가 보통 이렇게 카드에 대한 객체를 선언하기 위해서는 헤더를 위에 선언해야 한다.
	하지만 우리가 컴포지션 관계에 있을 때는 선언 해서 전방 선언을 하는 것이 좋다.
	전방 선언을 하면 헤더를 포함하지 않고 오브젝트는 포인터로 관리하기 때문에
	우리가 정확한 구현부는 알 수 없지만 포인터 크기를 가지기 때문에 전방 선언을 통해 의존성을 없앨 수 있다.
	*/

	// 이부분부터 전방선언
	/*
	UPROPERTY()
	class UCard* Card; 
	*/
	//이렇게 언리얼 오브젝트를 선언하여 컴포지션 관계를 구축 함.
	/*
	이버전이 4버전까지는 정석이었지만
	5버전부터는 다른 방식으로 선언하라고 새로운 표준을 들고옴
	언리얼 엔진 5 마이그레이션 가이드 (4->5 바꿀때 주의할점) 를 보면
	c++ 오브젝트 포인터 프로퍼티 라는 항목을 보면 기존에 사용하던 원시 포인터 UPROPERTY변수에 원시포인터가 있었던
	선언들을 TObjectPtr을 사용하여 변경하라는 내용이 있다.
	그래서 선택 사항이지만, 모두 포인터로 선언된 것들을 TObjectPtr이라는 템플릿으로 감싸서 선언하라고 되어 있다.
	따라서 선언에 대한 부분만 이렇게 TObjectPtr을 사용하고 구현부에서는 포인터를 사용해도 큰 문제가 없다고 명시한다.
	*/

	//여기부터 진짜 언리얼 5버전 전방선언
	UPROPERTY()
	TObjectPtr<class UCard> Card;


	
};
