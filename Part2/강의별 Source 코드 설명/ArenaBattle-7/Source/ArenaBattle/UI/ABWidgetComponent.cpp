// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABWidgetComponent.h"
#include "ABUserWidget.h" //�츮�� ������ ������ Ư���� ABUserWidget�̶�� ����

void UABWidgetComponent::InitWidget()
{
	Super::InitWidget();
	//InitWidget�� ȣ��� ���� ������ ���� �ν��Ͻ��� ������ �� �����̴�.
	//������ ���õ� UI��ҵ��� ��� �Ϸ�Ǹ� NativeConstruct�Լ��� ȣ���� ��.
	//������ ���� �ν��Ͻ��� ������ ���� �� �̶� ���õ� ���� ������ �� ������ OwningActor ������ ����������̴�.
	//���� ���Ŀ� ������ ���� �ν��Ͻ��� ������ ��.
	
	UABUserWidget* ABUserWidget = Cast<UABUserWidget>(GetWidget()); //GetWidget()�� ���� ���� ������Ʈ�� �������� ������ �����´�.
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner()); //GetOwner()�� ���� �ڽ��� �������� ���������� �����ͼ� �Ѱ��ش�.

	}
}
