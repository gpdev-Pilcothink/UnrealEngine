// Fill out your copyright notice in the Description page of Project Settings.

#include "Person.h"
#include "Card.h" //구현부라 include 해줘야 한다.

UPerson::UPerson()
{
	Name = TEXT("홍길동");
	Card = CreateDefaultSubobject<UCard>(TEXT("NAME_Card"));
	//생성자 Card에 대해서 CDO에서 구현할땐 CreateDefaultSubobject라는 API를 사용해야 한다.
	//인자에는 FName이라는 식별자를 넣어줘야 하는데 고유한 Name이면 된다.
	//FName임을 명시적으로 알려줄려면 NAME_이라는 접두사를 써주는게 좋다.

}
