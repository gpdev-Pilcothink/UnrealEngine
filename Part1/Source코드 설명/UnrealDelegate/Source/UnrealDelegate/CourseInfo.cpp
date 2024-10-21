// Fill out your copyright notice in the Description page of Project Settings.


#include "CourseInfo.h"

UCourseInfo::UCourseInfo()
{
	Contents = TEXT("기존 학사 정보");
}

void UCourseInfo::ChangeCourseInfo(const FString& InSchoolName, const FString& InNewContents)
{
	Contents = InNewContents;
	UE_LOG(LogTemp, Log, TEXT("[CourseInfo] 학사정보가 변경되어 알림."));
	OnChanged.Broadcast(InSchoolName, Contents);
	//이 부분까지 마쳤으면 학생도 수정을 해줘야함
}
