#pragma once

namespace MRI::FileIO
{
	class SceneManagerFileIO final : public MRI::FileIO::FileIOBase
	{
	public:

		SceneManagerFileIO ()          = default;
		~SceneManagerFileIO() override = default;

		void LoadFile() override;
		void SaveFile() override;
	};
}