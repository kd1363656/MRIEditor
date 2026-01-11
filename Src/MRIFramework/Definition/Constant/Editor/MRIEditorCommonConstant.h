#pragma once

namespace MRI::EditorCommonConstant
{
	static constexpr ImColor k_whiteColor = ImColor(255.0F , 255.0F , 255.0F , 1.0F);

	static constexpr std::string_view k_defaultTitleSave = "ファイルを保存";
	static constexpr std::string_view k_defaultTitleOpen = "ファイルを開く";

	static constexpr std::string_view k_defaultAssetDirectory = "Asset";

	static constexpr float k_defaultDragValue     = 0.1F;
	static constexpr float k_defaultComboBoxWidth = 150.0F;
	static constexpr float k_comboWidth           = 200.0F;

	static constexpr std::size_t k_maxFilePathSize = 1000LLU;

	static constexpr UINT k_defaultFilterIndex = 1U;

	static const char* const k_defaultFilters     = "すべてのファイル\0*.*\0";
	static const char* const k_jsonFilePathFilter = "JSONファイル (*.json)\0*.json\0";
	static const char* const k_gltfFilePathFilter = "GLTFファイル (*.gltf)\0*.gltf\0";

	const std::string k_saveSuccessPopupText = "Save completed successfully";
}