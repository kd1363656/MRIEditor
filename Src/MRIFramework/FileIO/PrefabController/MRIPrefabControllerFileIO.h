#pragma once

namespace MRI::FileIO
{
	class PrefabControllerFileIO final : public MRI::FileIO::FileIOBase
	{
	public:

		PrefabControllerFileIO ()          = default;
		~PrefabControllerFileIO() override = default;

		void LoadFile() override;
		void SaveFile() override;

		void SetPrefabControllerCache(const std::weak_ptr<MRI::Resource::PrefabController>& a_set) { m_prefabControllerCache = a_set; }

	private:

		std::weak_ptr<MRI::Resource::PrefabController> m_prefabControllerCache;
	};
}