#pragma once

namespace MRI::Editor
{
	class EditorPopup final
	{
	public:

		EditorPopup () = default;
		~EditorPopup() = default;

		void Init();

		void OpenPopup();
		void DrawPopup();

		void SetPopupText(const std::string& a_set) { m_popupText = a_set; }

		std::string_view GetPopupText() const { return m_popupText; }

	private:

		std::string m_popupText = MRI::CommonConstant::k_stringEmpty.data();

		bool m_isAppear = false;
	};
}