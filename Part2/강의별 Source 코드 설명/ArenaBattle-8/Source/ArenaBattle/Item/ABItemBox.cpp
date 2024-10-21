// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"

// Sets default values
AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggetBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));
	//CreateDefaultSubobject�� �����Ͽ� �� ������ �����Ϳ� ���� ��ü ����


	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
	//Ʈ������ ��� �츮�� �ռ� �����ߴ� Ʈ���ſ� ���� �ݸ��� ������ ���
	//OnComponentBeginOverlap�� �������Ʈ������ ��밡���ϵ��� ���̳��� ��������Ʈ�� ������ �Ǿ� �ִ�.
	//���� �츮�� �����ϴ� �Լ��� ��쿡�� UFUNCTION���� ������ ����� �Ѵ�.

	/*Mesh Component ����*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if(BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision")); //���ڴ� �����ֱ⸸ �ϰ� �浹�� �Ͼ�� �ʵ��� NoCollision ������ ����

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		//SetTemplate�� ����  EffectRef.Object ����
		Effect->bAutoActivate = false;
		//ó���� �ٷ� �ߵ����� �ʵ��� bAutoActivate=false�� �Ͽ� ��Ȱ��ȭ
	}

}

void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	IABCharacterItemInterface* OverlappingPawn = Cast< IABCharacterItemInterface>(OtherActor);
	//�ε�ģ ���Ϳ� ���ؼ� ����ȯ ����
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}



	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished);

	/*������ ����
	* �츮�� �������� ���� ���� �� ĳ���ͷκ��� �������� �����ߴٰ� ������ �� �ִ� �������̽��� �ϳ� �߰�����.
	* �������� �̵����� ĳ���ʹ� ���� ���̾��̱� ������ �츮�� �������̽��� ���ؼ� ���������� ȣ������� �Ѵ�.
	*/
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}



    