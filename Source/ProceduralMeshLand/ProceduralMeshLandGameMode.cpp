// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProceduralMeshLandGameMode.h"
#include "ProceduralMeshLandCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProceduralMeshLandGameMode::AProceduralMeshLandGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
