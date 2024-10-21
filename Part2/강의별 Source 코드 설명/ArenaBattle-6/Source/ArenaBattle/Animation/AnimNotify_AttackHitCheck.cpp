// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/ABAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	/*
	* Notify�Լ��� ���ڵ��� ����  �ִϸ��̼��� �����ϴ� ���ɷ�Ż �޽� ������Ʈ�� �ִϸ��̼� ���� �׸��� �߰����� ���۷��� ������ �䱸�Ѵ�.
	* �켱 �� �����κ��� �츮�� ���Ϳ� ����� ������ �ȴ�.
	*/
	if (MeshComp)//���� �޽�������Ʈ�� ������� �츮�� Owner�� �����;��Ѵ�.
	{
		MeshComp->GetOwner(); 
		/*Owner�� �������� ĳ�������� üũ���ְ� ĳ���Ͷ�� ������ �����۶�� ����� ������ �Ѵ�.[ĳ���� ��� �߰��ؾ��Ѵ�.]
		�ٸ� ������ ����� �߰��ϴ� ���� ��¶�� �������� ����.
		���������� ĳ���Ͱ� ������ ���� ���Ǵ� ���� ������ �̶����� �̷��� ����� �߰��ϴ� ���� �ٶ��� ���� �ʴ�.
		���� �츮�� �̰��� �� �� ���������� ����ϱ� ���ؼ��� �������̽��� �����ϴ� ���� ����.
		�츮�� ��¿�� ���� �������� �߻��Ǵ� ��쿡�� ������ �������̽��� ���ؼ� �����ϵ��� �����ϸ� ���� ���������� �̷��� ��Ƽ���� ��ɵ��� ����� �� �ְ� �ȴ�.
		*/

		IABAnimationAttackInterface* AttackPawn = Cast<IABAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn)
		{
			AttackPawn->AttackHitCheck();
		}
	
	}
	
}
