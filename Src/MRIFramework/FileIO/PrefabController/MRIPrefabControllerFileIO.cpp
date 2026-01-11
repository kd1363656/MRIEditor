#include "MRIPrefabControllerFileIO.h"

void MRI::FileIO::PrefabControllerFileIO::LoadFile()
{
	auto l_prefabControllerCache = m_prefabControllerCache.lock();
	if (!l_prefabControllerCache) { return; }

	for (const auto& [l_key , l_value] : l_prefabControllerCache->GetPrefabMap())
	{
		if (!l_value) { continue; }
		l_value->LoadPrefab();
	}
}
void MRI::FileIO::PrefabControllerFileIO::SaveFile()
{
	auto l_prefabControllerCache = m_prefabControllerCache.lock();
	if (!l_prefabControllerCache) { return; }

	for (const auto& [l_key, l_value] : l_prefabControllerCache->GetPrefabMap())
	{
		if (!l_value) { continue; }
		l_value->SavePrefab();
	}
}