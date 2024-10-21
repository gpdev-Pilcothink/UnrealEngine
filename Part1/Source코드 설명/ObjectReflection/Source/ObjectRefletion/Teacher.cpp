// Fill out your copyright notice in the Description page of Project Settings.


#include "Teacher.h"

UTeacher::UTeacher()
{
	Name = TEXT("이선생");
	Year = 3;
	Id = 1;
}

void UTeacher::DoLesson()
{
	Super::DoLesson();
	// 여기서 만약 DoLesson()에 대해서 자동완성이 안된다면 컴파일이 반영 안된 거임
	// 헤더파일을 리프레시(줄바꿈, 띄어쓰기 등등 한 후에 컴파일 하면 generated.h가 재생성 되면서 상위클래스 정보를 얻을 수 있을거임 
	UE_LOG(LogTemp, Log, TEXT("%d년차 선생님 %s님이 수업을 강의하십니다."),Year, *Name);
}
