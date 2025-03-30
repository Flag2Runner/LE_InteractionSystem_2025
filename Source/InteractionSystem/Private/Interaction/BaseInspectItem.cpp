// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/BaseInspectItem.h"

// Sets default values
ABaseInspectItem::ABaseInspectItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseInspectItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseInspectItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

