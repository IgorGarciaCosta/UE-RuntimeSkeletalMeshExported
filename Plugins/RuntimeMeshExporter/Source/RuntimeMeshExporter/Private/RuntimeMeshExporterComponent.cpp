#include "RuntimeMeshExporterComponent.h"

/* --- Includes específicos ---------------------------------------- */
#include "Components/SkeletalMeshComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/PositionVertexBuffer.h"
#include "SkeletalRenderPublic.h"          // FFinalSkinVertex  (depende de Renderer)

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "Logging/LogMacros.h"
#include "RuntimeMeshExporterHelpers.h"

/*------------------------------------------------------------------*/
URuntimeMeshExporterComponent::URuntimeMeshExporterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URuntimeMeshExporterComponent::BeginPlay()
{
	Super::BeginPlay();
}

/*------------------------------------------------------------------*/
void URuntimeMeshExporterComponent::ExportToOBJ(USkeletalMeshComponent* SkeletalMeshComp)
{
	/* ---------- 0) Validações ------------------------------------ */
	if (!SkeletalMeshComp || !SkeletalMeshComp->GetSkinnedAsset())
	{
		UE_LOG(LogTemp, Warning, TEXT("[RME] SkeletalMeshComponent inválido."));
		return;
	}
	if (!SkeletalMeshComp->MeshObject)        // ainda não foi renderizado ≥1 frame
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[RME] MeshObject ainda não criado; aguarde um frame."));
		return;
	}

	/* ---------- 1) Pasta Downloads -------------------------------- */
	FString DownloadDir = RMEHelpers::GetDownloadsDirectory();

	if (!IFileManager::Get().DirectoryExists(*DownloadDir))
	{
		DownloadDir = FPaths::ProjectSavedDir();        // fallback
	}
	IFileManager::Get().MakeDirectory(*DownloadDir, /*Tree*/ true);

	/* ---------- 2) Render data (LOD0) ----------------------------- */
	const FSkeletalMeshRenderData* RenderData =
		SkeletalMeshComp->GetSkeletalMeshRenderData();
	if (!RenderData || RenderData->LODRenderData.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[RME] LODRenderData ausente."));
		return;
	}
	const FSkeletalMeshLODRenderData& LOD = RenderData->LODRenderData[0];

	/* ---------- 3) Vértices skinados ------------------------------ */
	TArray<FFinalSkinVertex> SkinnedVerts;
	SkeletalMeshComp->GetCPUSkinnedVertices(SkinnedVerts, 0);
	if (SkinnedVerts.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("[RME] 0 vértices (Allow CPU Access?)."));
		return;
	}

	/* ---------- 4) Nome de arquivo único -------------------------- */
	const FString BaseName = SkeletalMeshComp->GetName().IsEmpty()
		? TEXT("Mesh") : SkeletalMeshComp->GetName();

	FString FileName = BaseName + TEXT(".obj");
	FString FullPath = FPaths::Combine(DownloadDir, FileName);
	int32   Counter = 1;
	while (FPaths::FileExists(FullPath))
	{
		FileName = FString::Printf(TEXT("%s_%d.obj"), *BaseName, Counter++);
		FullPath = FPaths::Combine(DownloadDir, FileName);
	}

	/* ---------- 5) Construir texto OBJ ---------------------------- */
	FString Obj;
	Obj.Reserve(SkinnedVerts.Num() * 32);

	const FTransform& CompTM = SkeletalMeshComp->GetComponentTransform();

	for (const FFinalSkinVertex& V : SkinnedVerts)
	{
		const FVector P = CompTM.TransformPosition(FVector(V.Position));
		Obj += FString::Printf(TEXT("v %f %f %f\n"), P.X, P.Y, P.Z);
	}

	const auto* Idx = LOD.MultiSizeIndexContainer.GetIndexBuffer();
	for (uint32 i = 0, n = Idx->Num(); i < n; i += 3)
	{
		Obj += FString::Printf(TEXT("f %d %d %d\n"),
			Idx->Get(i) + 1, Idx->Get(i + 1) + 1, Idx->Get(i + 2) + 1);
	}

	/* ---------- 6) Salvar ----------------------------------------- */
	if (FFileHelper::SaveStringToFile(Obj, *FullPath))
	{
		UE_LOG(LogTemp, Log, TEXT("[RME] OBJ salvo em: %s"), *FullPath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[RME] Falha ao salvar %s"), *FullPath);
	}
}