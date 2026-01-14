#pragma once

namespace MRI::Editor
{
	class EditorHierarchyView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorHierarchyView ()          = default;
		~EditorHierarchyView() override = default;

		std::uint32_t GetTypeID() const override { return MRI::StaticID::GetTypeID<MRI::Editor::EditorHierarchyView>(); }

		
	private:


	};
}