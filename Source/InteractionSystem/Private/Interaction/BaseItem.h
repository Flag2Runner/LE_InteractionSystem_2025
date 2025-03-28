// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractionInterface.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class ABaseItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// virtual void OnInteract_Implementation() override;
	//
	// virtual void Inspect_Implementation(ACharacter* Character, UStaticMesh* InspectedItemMesh, FText ItemName, FText ItemDescription) override;
	// virtual void ReadNote_Implementation(ACharacter* Character, FText NoteText) override;



	//*************************//
	//      Interaction        //
	//*************************//
public:
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Settings")
	float InteractionSphereRadius = 150.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Settings")
	float NotifySphereRadius = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Settings")
	FVector IconLocation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Settings")
	bool bIsDebug;

	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Inspection")
	bool bIsInspect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Inspection")
	FRotator DefaultInspectionRotation;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Inspection")
	FVector DefaultInspectionScale = FVector(1.f,1.f,1.f);
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | Interaction | Inspection")
	float DefaultInspectionOffset;
	
	//*************************//
	//      Reference          //
	//*************************//
private:
	UPROPERTY()
	ACharacter* PlayerCharacterReference;
	
	//UPROPERTY()
	//class UUserWidget* NoteWidgetReference;
	
	
	
};
