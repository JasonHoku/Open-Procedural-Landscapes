#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "Engine/World.h"
#include "ProceduralMeshComponent.h"

#include "Core/Public/HAL/Runnable.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"

#include "ProcLandscape_Actor_C.generated.h"

class UProceduralMeshComponent;
UCLASS()


class PROCEDURALMESHLAND_API AProcLandscape_Actor_C : public AActor
{
	GENERATED_BODY()

public:

	AProcLandscape_Actor_C();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MuProceduralMesh")
		float Calculations;
	void InitCalculations(float Calculations);


protected:
	virtual void BeginPlay() override;

	void PrintCalcData();

	int32 ProcessedCalculation;

	class FLoadVoxelThreadCalculator* CalcThread = nullptr;

	FRunnableThread* CurrentRunningThread = nullptr;

public:


	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	TQueue<int32> ThreadCalcQueue;

	UPROPERTY()
		TArray<FVector> normals;
	UPROPERTY()
		TArray<FVector2D> uvs;
	UPROPERTY()
		TArray<FColor> vertexColors;
	UPROPERTY()
		TArray<FProcMeshTangent> tangents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MuProceduralMesh")
		int32 height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MuProceduralMesh")
		int32 width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MuProceduralMesh")
		float spacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MuProceduralMesh")
		bool generateMesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		AActor* VoxelSpawnerLocationActor;


	//UPROPERTY(EditAnywhere, Category = "Config")
	//TSubclassOf<AActor> *CubeClass;

	// World() = GetWorld();

	// SpawnActor = UWorld::SpawnActor()


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int32 WorldSize;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> VoxelActorToSpawn;


	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", WorldContext = WorldContextObject))
		UObject* WorldContextObject;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TArray<UObject*> IgnoreCollissionObjectArray;

	//	UFUNCTION(BlueprintCallable)
	//	void RunLoadVoxelsTask(int32 VoxelCount, TSubclassOf<AActor> ActorToSpawn );


	FRunnableThread* Thread;



protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"))
		TArray<FVector> Vertices;

	UPROPERTY(EditAnywhere)
		TArray<int> Triangles;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* material;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector GetPlayerLocation();
	virtual void GenMesh();
	virtual void LoadMesh();
	FTimerHandle MemberTimerHandle;

	FVector LastLoadedChunkVec = FVector(0, 0, 0);

	FVector PlayerStartZ = FVector(-9, -9, -9);

	bool hasThreadStarted = false;

	static int OutThreadInt;


	// UFUNCTION(BlueprintCallable)
		// void RunLoadVoxelsTaskOnMain(int32 VoxelCount);

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> NewActorToSpawn;

	UProceduralMeshComponent* ProceduralMesh;

};



class LoadVoxelsTask : public FRunnable
{

public:

	LoadVoxelsTask(int32 _voxelData, TSubclassOf<AActor> _ActorToSpawn, UWorld* _GotWorld, int _inWorkValue);

	virtual ~LoadVoxelsTask() override;


	// Overriden from FRunnable
	// Do not call these functions youself, that will happen automatically
	bool Init() override; // Do your setup here, allocate memory, ect.
	uint32 Run() override; // Main data processing happens here
	void Stop() override; // Clean up any memory you allocated here

	bool bInputReady = false;



	// Declare the variables that are the inputs and outputs here.
	// You can have as many as you want. Remember not to use pointers, this must be
	// plain old data
	// For example:
	int ExampleIntInput = 0;
	float ExampleFloatOutput = 0.0f;



	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(LoadVoxelsTask, STATGROUP_ThreadPoolAsyncTasks);
	}

	int32 VoxelData;
	int inWorkValue;


	// UClass CubeClass;
	TSubclassOf<AActor> ActorToSpawn;

	UWorld* GotWorld;

	UObject* WorldContextObject;

	// UWorld* GotWorld = ALoadVoxelThreadActor::World;




private:

	// Thread handle. Control the thread using this, with operators like Kill and Suspend
	FRunnableThread* Thread;

	// Used to know when the thread should exit, changed in Stop(), read in Run()
	bool bRunThread;



};