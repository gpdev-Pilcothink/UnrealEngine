// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "MyGameInstance.generated.h"

struct FStudentData
{
	FStudentData() {}
	FStudentData(int32 InOrder, const FString& InName) : Order(InOrder), Name(InName) {}
	
	friend FArchive& operator<<(FArchive& Ar, FStudentData& InStudentData)
	{
		Ar << InStudentData.Order;
		Ar << InStudentData.Name;
		return Ar;
	}
	
	int32 Order = -1;
	FString Name = TEXT("홍길동");
};


/**
 * 
 */
UCLASS()
class UNREALSERIALIZATION_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	
public:
	UMyGameInstance();
	virtual void Init() override;

	void SaveStudentPackage() const;//1-13
	void LoadStudentPackage() const;//1-14
	void LoadStudentObject() const;//1-14 후반


	//가장 낮은 단계의 직렬화 시작
private:

	/*패키지 이름 지정*/
	static const FString PackageName;
	static const FString Assetname;
	/*패키지 이름 지정*/

	UPROPERTY()
	TObjectPtr<class UStudent> StudentSrc; //저장할 대상임

	/*1-14 후반부 비동기 로딩 구현*/
	FStreamableManager StreamableManager;
	//FStreamableManager는 포인터가 아니고 그냥선언하는거라 헤더추가 해야함
	TSharedPtr<FStreamableHandle> Handle;
	//스트리밍된 에셋을 관리할수 있는 핸들을 하나 지정해준다.


};
