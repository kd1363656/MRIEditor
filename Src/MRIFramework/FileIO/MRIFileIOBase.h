#pragma once

namespace MRI::FileIO
{
	class FileIOBase
	{
	public:

		FileIOBase         () = default;
		virtual ~FileIOBase() = default;

		virtual void Init() { m_fileIOPath = std::string(); };

		virtual void LoadFile() { /*必要に応じてオーバーライドしてください*/ };
		virtual void SaveFile() { /*必要に応じてオーバーライドしてください*/ };

		void SetFileIOPath(const std::string& a_set) { m_fileIOPath = a_set; }

	private:

		std::string m_fileIOPath = std::string();

	protected:

		const std::string& GetFileIOPath() const { return m_fileIOPath; }
	};
}