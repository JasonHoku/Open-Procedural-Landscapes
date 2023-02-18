
// #include "Engine/World.h"
#include "ProcLandscape_Actor_C.h"

#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/ActorComponent.h"

#include "ProceduralMeshComponent.h"

// Sets default values
AProcLandscape_Actor_C::AProcLandscape_Actor_C()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;

	void CallCreateToWorld();
	void GetWorld();

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());



	AProcLandscape_Actor_C::BeginPlay();

}



void AProcLandscape_Actor_C::PrintCalcData() {
	if (!ThreadCalcQueue.IsEmpty() && ThreadCalcQueue.Dequeue(ProcessedCalculation)) {




		UE_LOG(LogTemp, Warning, TEXT("Worker thread result integer: %i"), ProcessedCalculation);
		// 
		// // int v float
		// UE_LOG(LogTemp, Warning, TEXT("================== : %d"), ProcessedCalculation);
		//UE_LOG(LogTemp, Warning, TEXT("Worker thread result float: %f"), ProcessedCalculation);
		//UE_LOG(LogTemp, Log, TEXT("___ ======= Expecting:   %f ____ "), PlayerStartZ.X);

		return;
	}
}


void AProcLandscape_Actor_C::InitCalculations(float  _Calculations) {

	if (_Calculations > 0) {

		//CalcThread = new FLoadVoxelThreadCalculator(_Calculations, this);
		//CurrentRunningThread = FRunnableThread::Create(CalcThread, TEXT("Calc Thread"));
	}
	else {

		UE_LOG(LogTemp, Warning, TEXT("Calculations must be bigger than 0"));

	}
}


void AProcLandscape_Actor_C::EndPlay(EEndPlayReason::Type EndPlayReason) {


	if (CurrentRunningThread && CalcThread) {

		CurrentRunningThread->Suspend(true);
		//CalcThread->bStopThread = true;
		//CalcThread->Calculations = 10;
		//CurrentRunningThread->Suspend(false);
		//CurrentRunningThread->Kill(false);
		//CurrentRunningThread->WaitForCompletion();
		//delete CalcThread;
	}


}

int AProcLandscape_Actor_C::OutThreadInt;

void AProcLandscape_Actor_C::GenMesh()
{

	uint32 playerXLocInt = FMath::RoundToInt32(PlayerStartZ.X);


	// Run Task On New Thread

	//if (hasThreadStarted == false){
	//InitCalculations(playerXLocInt);
	//PrintCalcData();
	//hasThreadStarted = true;

	//}

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("===============================: "));

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Running Check Gen Mesh Tick: "));

	// UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("===============================: "));

	// UE_LOG(LogTemp, Log, TEXT("___Sending int32 :  %i ____ "), FMath::RoundToInt32(PlayerStartZ.X));
	// UE_LOG(LogTemp, Log, TEXT("___Sending float : %f ____ "), PlayerStartZ.X);


	if (GetWorld() != NULL && hasThreadStarted == false) {

		hasThreadStarted = true;

		// AProcLandscape_Actor_C::Tick(0);

	}


	if (WorldSize == NULL ) {
		return;
	}

	/*
	if ( VoxelActorToSpawn == NULL) {
		return;
	}
	*/
	//LoadVoxelsTask* Worker = new LoadVoxelsTask(WorldSize, VoxelActorToSpawn, GetWorld(), 1337);

	//Thread = FRunnableThread::Create(Worker, TEXT("VoxelThread"));

	//if (Worker)
	//{
	//	if (!Worker->bInputReady)

	//	{

	//		//Worker->ExampleFloatOutput = 500 * FMath::RandRange(0, 100);


	//		Worker->ExampleIntInput = PlayerStartZ.X;
	//		Worker->inWorkValue = PlayerStartZ.X;


	//		UE_LOG(LogTemp, Warning, TEXT("Game thread: value sent to worker thread: %s"), &Worker->ExampleIntInput);

	//		//FString outString = FString::SanitizeFloat(Worker->ExampleFloatOutput);

	//		 UE_LOG(LogTemp, Log, TEXT("___ %f ____ "), &Worker->ExampleFloatOutput);
	//		 
	//		//UE_LOG(LogClass, Log, TEXT("Game thread: value gotten from worker thread: %d"), outString);

	//		

	//		Worker->bInputReady = true;

	//	}
	//}

	//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Running Generate Mesh "));

	// for every face is 4 quad verts and 6 triangle verts

	if (height == NULL || width == NULL || spacing == NULL) {
		return;
	}
	

	if (GetWorld()->GetFirstPlayerController()->GetPawn() == NULL) {
		return;
	}



	int32 tileSize = 200;
	int32 gridSize = 12;


	//TArray<FVector2D> UV;


	float uvSpacing = 1.0f / FMath::Max(height, width);



	FVector PlayLoc = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//FTransform PlayTransform = GetWorld()->GetFirstPlayerController()->GetPawn()->GetTransform();


	// If player is is new chunk load new chunk


	// Call Stored Last Updated Player Pos



	// If Player.X >= LastUpdatedPos.X + spacing

	if (LastLoadedChunkVec == FVector(0, 0, 0)

		||

		LastLoadedChunkVec.X >= PlayLoc.X + spacing
		||

		LastLoadedChunkVec.X <= PlayLoc.X - spacing
		||

		LastLoadedChunkVec.Y >= PlayLoc.Y + spacing
		||

		LastLoadedChunkVec.Y <= PlayLoc.Y - spacing

		)

	{




	}
	else {


		UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Running Update Chunks Task: "));


		// if not in new chunk wait 0.01 and check again

		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AProcLandscape_Actor_C::GenMesh, 0.01f, true, 0.01f);
		return;
	}


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Player Location: %s"), *PlayLoc.ToString()));

	Vertices.Empty();
	normals.Empty();
	uvs.Empty();
	vertexColors.Empty();
	tangents.Empty();
	Triangles.Empty();


	LastLoadedChunkVec = PlayLoc;

	FString PlayLocString = PlayLoc.ToString();


	int playerX = PlayLoc.X;
	int playerY = PlayLoc.Y;
	int spacingInt = spacing;
	float playerXR = playerX % spacingInt;
	float playerYR = playerY % spacingInt;

	float roundedMultPlayerX = FMath::RoundToZero(PlayLoc.X) - playerXR;
	float roundedMultPlayerY = FMath::RoundToZero(PlayLoc.Y) - playerYR;


	// UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(PlayLocString));

	FString RMPX = FString::SanitizeFloat(roundedMultPlayerX);
	FString RMPY = FString::SanitizeFloat(roundedMultPlayerY);

	if (PlayerStartZ == FVector(-9, -9, -9)) {

		PlayerStartZ = PlayLoc;


	}

	FString MainLocationString = AActor::GetActorLocation().ToString();
	FVector MainLocationVec = AActor::GetActorLocation();
	FString MainLocZ = FString::SanitizeFloat(MainLocationVec.Z);



	//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(MainLocationString));
	//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *MainLocZ);


	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), false, this);

	RV_TraceParams.AddIgnoredActor(GetWorld()->GetFirstPlayerController()->GetPawn());
	RV_TraceParams.AddIgnoredActor(VoxelSpawnerLocationActor);



	if (IgnoreCollissionObjectArray.Num() <= 1) {


		for (size_t i = 0; i < IgnoreCollissionObjectArray.Num(); i++)
		{

			UObject* GotActorObject = IgnoreCollissionObjectArray[i];

			TSubclassOf<AActor> IgnoreActor = GotActorObject->GetClass();
			TArray<AActor*> OutActors;

			//UGameplayStatics::GetAllActorsOfClass(GetWorld(), IgnoreActor, OutActors);

			AActor* GotActor = UGameplayStatics::GetActorOfClass(GEngine->GetWorld(), IgnoreActor);


			RV_TraceParams.AddIgnoredActor(GotActor);


			UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("AddedIgnore"));

		}

	}





	for (int32 y = 0; y < height; y++)
	{
		for (int32 x = 0; x < width; x++)
		{
			if (x == 1 && y == 1) {

				//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(RMPX));
				//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(RMPY));
			}

			// perlin test
			// float PerlinFloat = FMath::PerlinNoise2D(FVector2D(x * spacing + (PlayLoc.X / spacing), (y * spacing + (PlayLoc.Y / spacing))));

			// simple sin displace 
			// float zSin = sin((x + (roundedMultPlayerX / spacing)) + (y + (roundedMultPlayerY / spacing)));

			float xVertIndex = (x * spacing + PlayLoc.X);
			float yVertIndex = (y * spacing + PlayLoc.Y);

			//

			float roundedPlayerX = FMath::RoundToZero(PlayLoc.X);
			float roundedPlayerY = FMath::RoundToZero(PlayLoc.Y);

			float spacing2 = spacing * 10;
			float width2 = width * 10;
			float x2 = x * 10;
			float y2 = y * 10;

			float zSinRP = sin((x * spacing + (roundedMultPlayerX / spacing)) + (y * spacing + (roundedMultPlayerY / spacing)));

			FVector GridVertPoint = FVector(roundedMultPlayerX + x * spacing - ((width * spacing) / 2), roundedMultPlayerY + y * spacing - ((width * spacing) / 2), PlayerStartZ.Z);
			//FVector GridVertPoint2 = FVector(roundedMultPlayerX + x2 * spacing2 - ((width2 * spacing2) / 2) , roundedMultPlayerY + y2 * spacing2 - ((width2 * spacing2) / 2) , PlayerStartZ.Z);

			FHitResult Hit(ForceInit);
			FVector start = GridVertPoint + FVector(0, 0, 1000);
			FVector End = GridVertPoint - FVector(0, 0, 1000);


			//FVector start2 = GridVertPoint2 + FVector(0, 0, 1000);
			//FVector End2 = GridVertPoint2 - FVector(0, 0, 1000);






			//DrawDebugLine(GetWorld(), start, End, FColor::Green, true, 2.f, false, 4.f);
			//DrawDebugLine(GetWorld(), start2, End2, FColor::Blue, false, 20.f, false, 4.f);

			GetWorld()->LineTraceSingleByChannel(Hit, start, End, ECC_WorldDynamic, RV_TraceParams);

			float finalZ = 0;

			if (Hit.Location != FVector(0, 0, 0))
			{

				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Hit Location: %s"), *Hit.Location.ToString()));
				//finalZ = Hit.Location.Z;
			}
			else {
				// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("No Hit Location: %s"), *Hit.Location.ToString()));
				//finalZ = PlayerStartZ.Z;
			}

			finalZ = zSinRP;


			// Vertices.Add(FVector(PlayLoc.X + x * spacing - ((width * spacing) / 2), PlayLoc.Y + y * spacing - ((width * spacing) / 2), zSin * 300));

			Vertices.Add(FVector(roundedMultPlayerX + x * spacing - ((width * spacing) / 2), roundedMultPlayerY + y * spacing - ((width * spacing) / 2), finalZ * 300));


			normals.Add(FVector(0.0f, 0.0f, 1.0f));
			uvs.Add(FVector2D(x * uvSpacing, y * uvSpacing));
			vertexColors.Add(FColor().MakeRandomColor());
			tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f));
		}
	}
	for (int32 y = 0; y < height - 1; y++)
	{
		for (int32 x = 0; x < width - 1; x++)
		{
			Triangles.Add(x + (y * width));					//current vertex
			Triangles.Add(x + (y * width) + width);			//current vertex + row
			Triangles.Add(x + (y * width) + width + 1);		//current vertex + row + one right

			Triangles.Add(x + (y * width));					//current vertex
			Triangles.Add(x + (y * width) + width + 1);		//current vertex + row + one right
			Triangles.Add(x + (y * width) + 1);				//current vertex + one right
		}
	}

	PlayerStartZ = PlayLoc;


	/*if (GetWorld() != NULL){
		AProcLandscape_Actor_C::LoadMesh();
	}



	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("===============================: "));

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Loading Mesh "));

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("===============================: "));*/


	if (Vertices.Num() >= 99){
	FString MainVerticesString = Vertices[100].ToString();
	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(MainLocationString));
	}

	int vertLength = Vertices.Num();
	FString floatStringVert = FString::FromInt(vertLength);

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(floatStringVert));


	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Player Location "));

	FString playerLocString = PlayLoc.ToString();
	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(playerLocString));



	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Set Of Cells To Load Based On Player Loc"));
	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString(playerLocString));



	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, normals, uvs, vertexColors, tangents, true);
	ProceduralMesh->SetMaterial(0, material);
	ProceduralMesh->SetScalarParameterValueOnMaterials("ValueX", roundedMultPlayerX / 100);
	ProceduralMesh->SetScalarParameterValueOnMaterials("ValueY", roundedMultPlayerY / 100);


	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AProcLandscape_Actor_C::GenMesh, 0.01f, true, 0.01f);


}


void AProcLandscape_Actor_C::LoadMesh()
{




}


// Called when the game starts or when spawned
void AProcLandscape_Actor_C::BeginPlay()
{


	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("===============================: "));

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Loading Proc Mesh C Actor "));

	UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("===============================: "));


	if (GetWorld() != NULL) {
		UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("=== Got World "));
		AProcLandscape_Actor_C::GenMesh();
	}
	else {

		UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("=== Loading Proc Mesh C Actor "));
	}




	//FRotator Rotation(0.0f, 0.0f, 0.0f);
	//FActorSpawnParameters SpawnInfo;
	//UWorld* world = GetWorld();
	//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Calling Get World: "));
	//if (world == NULL) {
	//	return;
	//}
	//else {

	//	for (int32 i = 0; i < WorldSize; i++) {
	//		for (int32 i2 = 0; i2 < WorldSize; i2++) {
	//			FVector Location(0.0f + (100 * i), 0.0f + (100 * i2), 10.0f);

	//			// Game Threaded Build World
	//			// world->UWorld::SpawnActor <AActor>(VoxelActorToSpawn, Location, Rotation, SpawnInfo);
	//		}
	//	}
	//}

}



FVector AProcLandscape_Actor_C::GetPlayerLocation() {

	return PlayerStartZ;
}

// Called every Game frame
void AProcLandscape_Actor_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}




LoadVoxelsTask::LoadVoxelsTask(int32 _voxelData, TSubclassOf<AActor> _ActorToSpawn, UWorld* _GotWorld, int _inWorkValue)
{


	VoxelData = _voxelData;
	ActorToSpawn = _ActorToSpawn;
	GotWorld = _GotWorld;
	inWorkValue = _inWorkValue;

	// Thread = FRunnableThread::Create(this, TEXT("VoxelThread"));

	//UWorld* GotWorld = AProcLandscape_Actor_C::GotWorld;

	//UClass* CubeActor = CubeClass;
	// ToSpawn = ToSpawn;
}




LoadVoxelsTask::~LoadVoxelsTask()
{
	if (Thread)
	{
		// Kill() is a blocking call, it waits for the thread to finish.
		// Hopefully that doesn't take too long
		Thread->Kill();
		delete Thread;
	}

}

bool LoadVoxelsTask::Init()
{
	UE_LOG(LogTemp, Warning, TEXT(" 0=== Init LoadVoxelTask"))

		// Return false if you want to abort the thread
		return true;
}
void LoadVoxelsTask::Stop()
{
	// Clean up memory usage here, and make sure the Run() function stops soon
	// The main thread will be stopped until this finishes!

	// For this example, we just need to terminate the while loop
	// It will finish in <= 1 sec, due to the Sleep()
	bRunThread = false;
}

uint32 LoadVoxelsTask::Run()
{

	int32 runCounter = 0;


	while (bRunThread)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Thread Compiler running 0.1.0 "))


		if (bInputReady) {
			// ExampleFloatOutput = ExampleIntInput;

			//AProcLandscape_Actor_C getLoc;
			//FVector gotPlayerLoc =  getLoc.GetPlayerLocation();


			//UE_LOG(LogTemp, Warning, TEXT("Worker thread starting calculation on: %d"), ExampleIntInput)
			//	UE_LOG(LogTemp, Warning, TEXT("Worker thread starting calculation on: %d"), inWorkValue)


			//auto MeshAsset = ConstructorHelpers::FObjectFinder<AActor>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

			//AActor* GotActor =
			//UGameplayStatics::GetActorOfClass(GEngine->GetWorld(), ActorToSpawn);


			// Create a static mesh component

			// Load the Cube mesh
		//	UStaticMesh* cubeMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")).Object;


			VoxelData = 2;


			int tilesX = VoxelData;
			int tilesY = VoxelData;

			//UWorld * World = GEngine->GetWorldFromContextObject(WorldContextObject);


			for (int32 i = 0; i <= tilesX; i++) {
				for (int32 i2 = 0; i2 <= tilesY; i2++) {
					runCounter++;

					//FString IntString =  FString("Create 1: ") + FString::FromInt(i) + FString::FromInt(i2);

					//AProcLandscape_Actor_C::CallGetWorld(ActorToSpawn);

					// UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *IntString);



					//FString IntString2 = FString("Spawn 1: ") + FString::FromInt(i) + FString::FromInt(i2);
					//UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *IntString2);


					//FVector Location(0.0f + (i * 100), 0.0f, 0.0f);
					//FRotator Rotation(0.0f, 0.0f, 0.0f);
					//FActorSpawnParameters SpawnInfo;

					// AActor* CubeActor = World->SpawnActor<AActor>(	
					//	CubeClass, Location, Rotation);

					//GotWorld->SpawnActor<AActor>(ActorToSpawn, Location, Rotation, SpawnInfo);

					// UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), success);

					// UE_LOG(LogTemp, Log, TEXT("___ %s ____ "), *FString("Finished Thread Calculation "));

					// inWorkValue = i + i2 * i + i2 * i2 * i2 ;
					// LoadVoxelsTask::ExampleFloatOutput = LoadVoxelsTask::ExampleFloatOutput + 1 ;


					ExampleFloatOutput = 5.0f;


				}





				if (runCounter == VoxelData * VoxelData) {
					LoadVoxelsTask::Stop();
					bRunThread = false;
				}

				bInputReady = false;
			}
		}
		FPlatformProcess::Sleep(0.02f);

	}
	return 0;
}

