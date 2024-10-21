// Fill out your copyright notice in the Description page of Project Settings.


#include "Teacher.h"
#include "Card.h"

UTeacher::UTeacher() //생성자의 경우에는 부모클래스의 생성자가 호출된 이후에 이코드가 실행된. 이후에 코드가 실행되기 때문에 ==>CreateDefaultSubobject를 할필요가 없다.
{
	Name = TEXT("이선생");
	Card->SetCardType(ECardType::Teacher);
}

void UTeacher::DoLesson()
{
	ILessonInterface::DoLesson();
	UE_LOG(LogTemp, Log, TEXT("%s님은 가르칩니다."), *Name);
}
