// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


//******구조체 학습부분******
USTRUCT()
struct FStudentData
{
	GENERATED_BODY()
	//작성하는 매크로들이 public으로 끝나게 되는게 구조체가 원래 가지는 기본 접근 지시자인 public을 그대로 사용하면 된다.

	FStudentData()
	{
		Name = TEXT("홍길동");
		Order = -1;
	}
	
	FStudentData(FString InName, int32 InOrder) : Name(InName), Order(InOrder) {};

	/*FStudentData를 TSet에 쓰기 위한 수정 부분*/
	bool operator==(const FStudentData& InOther) const
	{
		return Order == InOther.Order;
	}
	friend FORCEINLINE uint32 GetTypeHash(const FStudentData& InStudentData)
	{
		//해쉬값은 uint32로 반환해주고 겟타입해쉬를 만들어주되 인자로 레퍼런스를 넣어준다.
		return GetTypeHash(InStudentData.Order);
		//우리가 이미 가지고 있는 integer order에 대한 해쉬값을 리턴해주는 것으로 Student에 대한 해쉬값을 지정해준다.
	}
	/*FStudentData를 TSet에 쓰기 위한 수정 부분*/


	UPROPERTY() 
	/*
	우리가 UPROPERTY를 넣어도 되고 안넣어도 무방한데 UPROPERTY를 넣을때는 넣는 목적이 명확히 있어야 한다. 
	예를 들어 리플렉션을 통해 보여준다던지 스크립트 랭귀지인 블루프린트와 
	호환을 시킨다든지 또한 언리얼 오브젝트 포인터를 멤버변수로 가진다면 
	UPROPERTY매크로를 넣어야 한다.
	*/
	FString Name;

	UPROPERTY()
	int32 Order;
};
//******구조체 학습부분******


/**
 * 
 */
UCLASS()
class UNREALCONTAINER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

private:

	TArray<FStudentData> StudentsData;
	//위 경우는 값 타입이기 때문에 메모리를 관리할 필요가 없다. 
	//따라서 리플렉션 기능으로 먼가 조회하는게 아니라면 UPROPERTY를 붙일 필요가 없다.

	UPROPERTY() // TArray에 내부적으로 포인터를 관리하게 된다면 이거 꼭 해줘야함.
	TArray<TObjectPtr<class UStudent>> Students;
	//언리얼 오브젝트 헤더에서 언리얼 오브젝트 포인터를 선언할 때는 TObjectPtr로 감싸줘야 한다.
	//또한 전방선언으로 헤더 의존성을 최소화 시킨다.
	//TArray에 내부적으로 포인터를 관리하게 된다면 반드시 자동으로 언리얼엔진이 메모리를 관리할수 있게 UPROPERTY()매크로를 반드시 붙여줘야 한다.


	TMap<int32, FString> StudentsMap;
	//혹시나 key나 value에 언리얼 오브젝트 포인터가 들어가게 된다면 반드시 UPROPERTY()를 선언해줘야 한다.
	
};
