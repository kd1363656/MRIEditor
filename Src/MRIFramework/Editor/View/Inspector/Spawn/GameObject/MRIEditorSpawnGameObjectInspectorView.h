#pragma once

namespace MRI::Editor
{
	class EditorSpawnGameObjectInspectorView final : public MRI::Editor::EditorViewBase
	{
	public:

		EditorSpawnGameObjectInspectorView ()          = default;
		~EditorSpawnGameObjectInspectorView() override = default;

		std::uint32_t GetTypeID() const override { return StaticID::GetTypeID<MRI::Editor::EditorSpawnGameObjectInspectorView>(); }

		void DrawEditor() override;
	};
}