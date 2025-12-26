#pragma once

namespace MRI::Editor
{
	class FileMenuEditor final : public MRI::Editor::EditorMenuBase
	{
	public:

		FileMenuEditor ()          = default;
		~FileMenuEditor() override = default;

		void Init      () override;
		void DrawEditor() override;

	private:

		void DrawMenuFile     ();
		void UpdateShortCutKey();
		void DrawPopUp        ();

		std::unique_ptr<MRI::Editor::EditorPopup> m_saveSuccessPopup = nullptr;
	};
}