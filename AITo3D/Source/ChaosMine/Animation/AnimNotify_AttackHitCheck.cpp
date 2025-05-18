// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_AttackHitCheck.h"
#include "Interface/MineAnimationAttackInterface.h"

void UAnimNotify_AttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	/**********************************************
	*�޽�������Ʈ�� �����ϰ� �ִ� Owner�� �����´�.
	* �׸��� �� Owner�� �츮�� ������ ĳ���������� üũ
	* ĳ���Ͱ� �´ٸ� ���� ����� ���� �� ����.
	* ////////////////////////////////////////
	* �ٵ� ���⼭ ĳ���͸� �ٷ� ����ϱ� ���ؼ��� ����� �߰������ �Ѵ�.(������ �߻�)
	* ��Ƽ���� ����� ��� ��������  ���� ������ ĳ���Ͱ� ������ ���� ���Ǵ� ���� ������ �̶����� ����� �߰��ϴ°� �ٶ������� ����
	* ���� �츮�� �������� ����� �����ϰ� ���� �������̽��� ��������.
	* �׸��� �޾ƿ� MeshComponent�� Owner�� �������̽��� �����ߴ��� üũ���ָ� ��.
	**********************************************/

	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp)
	{
		IMineAnimationAttackInterface* AttackPawn = Cast<IMineAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawn) //�������̽� ��������� üũ
		{
			AttackPawn->AttackHitCheck();
		}
	}
}
