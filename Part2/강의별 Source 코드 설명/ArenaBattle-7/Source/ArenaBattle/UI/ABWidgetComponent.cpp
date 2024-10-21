// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h" //우리가 소유한 위젯이 특별한 ABUserWidget이라고 가정

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();
	//InitWidget이 호출될 때는 위젯에 대한 인스턴스가 생성이 된 직후이다.
	//위젯에 관련된 UI요소들이 모두 완료되면 NativeConstruct함수가 호출이 됨.
	//위젯에 대한 인스턴스가 생성이 됬을 때 이때 관련된 액터 정보를 얻어서 위젯의 OwningActor 변수에 집어넣을것이다.
	//수퍼 이후에 위젯에 대한 인스턴스가 생성이 됨.
	
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget()); //GetWidget()을 통해 현재 컴포넌트가 소유중인 위젯을 가져온다.
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner()); //GetOwner()를 통해 자신이 소유중인 액터정보를 가져와서 넘겨준다.

	}
}
