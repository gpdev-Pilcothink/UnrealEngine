// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h" //이 헤더는 UMG모듈에서 제공함
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) //부모 클래스에게  ObjectInitializer인자 그대로 넘겨주면 됨.
{
	MaxHp = -1.0f;
	//이 MaxHp값은 우선 위젯에서는 음수로 지정함. 
	//MaxHp가 설정이 안되어 있다면 Hp바를 표시하는데 여러가지 문제가 많기 때문에 초기화 될대 반드시 올바른 값으로 지정하도록 우리가 안내를 해줘야 한다.
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	//GetWidgetFromName사용하여 이름으로 찾자
	ensure(HpProgressBar);
	//해당 컨트롤은 반드시 있어야 하기에 ensure로 확인하자

	IABCharacterWidgetInterface* CharacterWidget = Cast< IABCharacterWidgetInterface>(OwingActor); 
	//OwingActor가 해당 인터페이스로 형변환 되는지 조사.
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}

	/*
	* 이제 사용 가능한 OwingActor라는 액터 정보에다가 
	* 내가 가진 정보를 UpdateHpBar라고 하는 함수정보를 전달해서 우리가 델리게이트에 등록시키면
	* 앞으로 스탯이 업데이트 될 때마다 이 함수가 UpdateHpBar가 호출되면서 HpBar가 변하게 될 것이다.
	*/
	/*
	* 기본 세팅을 마무리한 후 이제 위젯이 자기를 소유한 액터 정보를 얻었을때
	* 액터 정보로부터 UpdatehpBar라는 멤버 함수를 등록하는 로직을 추가해야 한다.
	* 이를 위해서 ABCharacterClass를 직접 참고하면 의존성이 발생함.
	* 따라서 이러한 위젯 컴포넌트들은 캐릭터와 무관하게 다양한 캐릭터에 적용될 수 있도록 인터페이스를 하나 선언해주고 이것을 통해 전달하자.
	*/
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.0f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		//UMG모듈의 경우에는 기본적으로 포함이 안되어있어서 빌드 설정가서 수정해야한다.
	}
}
