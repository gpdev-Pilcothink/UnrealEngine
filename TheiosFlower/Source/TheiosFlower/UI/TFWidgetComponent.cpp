// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TFWidgetComponent.h"
#include "TFUserWidget.h"

void UTFWidgetComponent::InitWidget()
{
	Super::InitWidget();
	//InitWidget�� ȣ��� ���� ������ ���� �ν��Ͻ��� ������ �� �����̴�.
	//������ ���õ� UI��ҵ��� ��� �Ϸ�Ǹ� NativeConstruct�Լ��� ȣ���� ��.
	//������ ���� �ν��Ͻ��� ������ ���� �� �̶� ���õ� ���� ������ �� ������ OwningActor ������ ����������̴�.
	//���� ���Ŀ� ������ ���� �ν��Ͻ��� ������ ��.

	UTFUserWidget* TFUserWidget = Cast<UTFUserWidget>(GetWidget()); //GetWidget()�� ���� ���� ������Ʈ�� �������� ������ �����´�.
	if (TFUserWidget)
	{
		TFUserWidget->SetOwningActor(GetOwner()); //GetOwner()�� ���� �ڽ��� �������� ���������� �����ͼ� �Ѱ��ش�.

	}
}