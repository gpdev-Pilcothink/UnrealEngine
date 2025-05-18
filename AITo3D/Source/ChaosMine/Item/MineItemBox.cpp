// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MineItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/MineCollision.h"
#include "Interface/MineCharacterItemInterface.h"



// Sets default values
AMineItemBox::AMineItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	Effect->SetupAttachment(Trigger);

	Trigger->SetCollisionProfileName(CPROFILE_MINETRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	//OnComponentBeginOverlap�� ���� ������� ��������Ʈ�� �������Ʈ�ε� ����� �� �ֵ��� ���̳��� ��������Ʈ�� ����Ǿ� ����.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMineItemBox::OnOverlapBegin); 


	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/AIMakeAssset/Objects/OldCrate/OldCrate.OldCrate'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	
	
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Hit/P_Asphalt.P_Asphalt'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object);
		Effect->bAutoActivate = false;
	}
}

void AMineItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{ 
	if (nullptr == Item)
	{
		Destroy();
		return;
	}
	
	IMineCharacterItemInterface* OverlappingPawn = Cast< IMineCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}

	//���⼭ ����Ʈ �ߵ��ǵ�������
	Effect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	Effect->OnSystemFinished.AddDynamic(this, &AMineItemBox::OnEffectFinished);
}

void AMineItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}

