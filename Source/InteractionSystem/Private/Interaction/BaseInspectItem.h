// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "GameFramework/Actor.h"
#include "Widgets/W_Inspect.h"
#include "Interaction/InteractionInterface.h"
#include "Widgets/W_Note.h"
#include "BaseInspectItem.generated.h"

UCLASS()
class ABaseInspectItem : public AActor,public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInspectItem();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ReadNote_Implementation(ACharacter* Character, const FText& NoteText) override;

	virtual void Inspect_Implementation(ACharacter* Character, UStaticMesh* InspectedItemMesh, const FText& ItemName, const FText& ItemDescription) override;


	UPROPERTY(BlueprintReadOnly, Category= "public | Reference")
	ACharacter* PlayerCharacterReference;

	UPROPERTY(BlueprintReadOnly, Category= "public | Reference")
	APlayerController* PlayerController;

	UPROPERTY()
	USceneCaptureComponent2D*  SceneCaptureComponent;

	UPROPERTY(BlueprintReadOnly, Category = "public | Reference")
	UW_Note* NoteWidgetReference;

	UPROPERTY(BlueprintReadOnly, Category = "public | Reference")
	UW_Inspect* InspectWidgetReference;
	
	//*************************//
	//            UI           //
	//*************************//

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	TSubclassOf<UW_Note> NoteWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	TSubclassOf<UW_Inspect> InspectWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "public | Reference")
	UBorder* NoteReadBorder;

	//*************************//
	//         Settings        //
	//*************************//
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	FRotator InitialRotation;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	bool bIsRotating = false;

	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	bool bIsInNote = false;

	//*************************//
	//         Mesh            //
	//*************************//
	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Mesh")
	UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category = "public | Reference | Mesh")
	UStaticMeshComponent* ItemPivotMeshComponent;

private:
	float MouseX, MouseY;
	


	


	

};
