// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "GameFramework/Actor.h"
#include "Widgets/W_Inspect.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
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

	void OnMove(const FInputActionValue& Value); // Function to handle movement input
	
	/** Called for Interaction Input **/
	void Interact(const FInputActionValue& Value);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void ReadNote_Implementation(ACharacter* Character, const FText& NoteText) override;

	virtual void Inspect_Implementation(ACharacter* Character, UStaticMesh* InspectedItemMesh, const FText& ItemName, const FText& ItemDescription) override;

	UFUNCTION()
	void SetInspectRotationScaleOffset(const FRotator& ItemRotation, float NewInspectionOffset, const FVector& InspectionScale);

	UFUNCTION()
	void InitVariables(TSubclassOf<UW_Note> NewNoteWidgetClass, TSubclassOf<UW_Inspect> NewInspectWidgetClass,
		const UInputMappingContext* NewInputMappingContext, const UInputAction* NewMoveAction, const UInputAction* NewInteractAction, UTextureRenderTarget2D* NewRenderTarget2D);

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

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	const UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	const UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Prefab")
	const UInputAction* InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Public | UI | Teture")
	UTextureRenderTarget2D* RenderTarget2D;
	

	UPROPERTY(BlueprintReadOnly, Category = "public | Reference")
	UBorder* NoteReadBorder;

	//*************************//
	//         Settings        //
	//*************************//
	UPROPERTY(BlueprintReadOnly, BlueprintReadOnly, Category = "Public | Interaction | Settings")
	FRotator InitialRotation;

	UPROPERTY()
	FVector InspectionOffset;

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
