#pragma once
#include "CoreMinimal.h"

namespace RMEHelpers
{
	/* Retorna o caminho absoluto da pasta Downloads do usuário,
	   independente de SO ou de build (Editor / Shipping).             */
	inline FString GetDownloadsDirectory()
	{
#if PLATFORM_WINDOWS
		const FString Home = FPlatformMisc::GetEnvironmentVariable(TEXT("USERPROFILE"));
#elif PLATFORM_LINUX || PLATFORM_MAC
		const FString Home = FPlatformMisc::GetEnvironmentVariable(TEXT("HOME"));
#else
		const FString Home = FPlatformProcess::UserDir();   // fallback genérico
#endif
		FString Downloads = FPaths::Combine(Home, TEXT("Downloads"));

		// Caso não exista (ex.: em distros Linux minimalistas) criamos.
		IFileManager::Get().MakeDirectory(*Downloads, /*Tree*/ true);
		return Downloads;
	}
}
