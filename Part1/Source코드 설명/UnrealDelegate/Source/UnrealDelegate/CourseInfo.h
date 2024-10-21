// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CourseInfo.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FCourseInfoOnChangedSignature,const FString&, const FString&)

/**
 * 
 */
UCLASS()
class UNREALDELEGATE_API UCourseInfo : public UObject
{
	GENERATED_BODY()
	
public:
	UCourseInfo();

	FCourseInfoOnChangedSignature OnChanged; //마치 객체처럼 델리게이트 변수를 멤버변수로 써준다.
	//우리가 이제 델리게이트의 어떤 함수가 와서 바인딩 한다면 우리가 송출해줘야 하는데 우리는 "내용이 변경되면 바로 송출해준다."

	void ChangeCourseInfo(const FString& InSchoolName, const FString& InNewContents);
	//윗줄이 "내용이 변경되면 바로 송출해준다." 부분임.
private:
	FString Contents;

	
};
