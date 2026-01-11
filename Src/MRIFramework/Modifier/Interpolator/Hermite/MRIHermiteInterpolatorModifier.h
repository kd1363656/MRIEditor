#pragma once

namespace MRI::Modifier
{
	class HermiteInterpolatorModifier final : public MRI::Modifier::InterpolatorModifierBase
	{
	private:

		struct KeyFrame
		{
			float time    = 0.0F;
			float value   = 0.0F;
			float tangent = 0.0F;
		};

	public:

		HermiteInterpolatorModifier ()          = default;
		~HermiteInterpolatorModifier() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init() override;

		void Update() override;

		void EditPrefabInspector() override;

		void DeserializePrefab(const nlohmann::json& a_json) override;

		nlohmann::json SerializePrefab() override;

	private:

		void EditSortList      ();
		void EditDrawGraph     ();
		void EditHermiteCurve  (ImDrawList* const a_drawList , const ImVec2& a_canvasPos);
		void EditKeyFrame      (ImDrawList* const a_drawList , const ImVec2& a_canvasPos);
		void EditAddKeyFrame   (const ImVec2&	  a_mousePos , const ImVec2& a_canvasPos);
		void EditDeleteKeyFrame();
		void EditDrawGridLine  (ImDrawList* const a_drawList , const ImVec2& a_canvasPos) const;
		void EditGraphSize     (ImDrawList* const a_drawList , const ImVec2& a_canvasPos);

		static constexpr ImVec2 k_debugMinGraphSize        = { 10.0F  , 10.0F  };
		static constexpr ImVec2 k_debugMouseHitRangeRadius = { 5.0F   , 5.0F   };
		static constexpr ImVec2 k_debugInitialGraphSize    = { 300.0F , 200.0F };
		static constexpr ImVec2 k_resizeHandleSize		   = { 10.0F  , 10.0F  };
		static constexpr ImVec2 k_graphSizeMin		       = { 100.0F , 100.0F };

		static constexpr ImU32 k_graphBorderColor      = IM_COL32(100.0F , 100.0F , 100.0F , 255.0F);
		static constexpr ImU32 k_graphGuidYZeroColor   = IM_COL32(255.0F , 255.0F , 255.0F , 255.0F);
		static constexpr ImU32 k_graphGuidLineColor    = IM_COL32(80.0F  , 80.0F  , 80.0F  , 255.0F);
		static constexpr ImU32 k_selectedKeyFrameColor = IM_COL32(255.0F , 255.0F , 0.0F   , 255.0F);
		static constexpr ImU32 k_graphDragBoxColor     = IM_COL32(180.0F , 180.0F , 180.0F , 255.0F);
		static constexpr ImU32 k_defaultKeyFrameColor  = IM_COL32(255.0F , 200.0F , 0.0F   , 255.0F);
		static constexpr ImU32 k_progressColor         = IM_COL32(0.0F   , 255.0F , 0.0F   , 255.0F);

		static constexpr std::size_t k_minSizeToNextListAccess = 2ULL;
		
		static constexpr float k_debugCircleFiledRadius             =  5.0f;
		static constexpr float k_debugAutoIncrementKeyFrameTimeStep =  0.1F;
		static constexpr float k_debugKeyFrameDrawStep			    =  0.05F;
		static constexpr float k_maxDebugKeyFrameDrawStep			=  1.0F;
		static constexpr float k_debugHermiteTangentMinSlider       = -10.0F;
		static constexpr float k_debugHermiteTangentMaxSlider       =  10.0F;
		static constexpr float k_lineThickness                      =  2.0F;

		static constexpr int k_maxDebugGridNum   = 10;
		static constexpr int k_maxDebugStepNum   = 32;
		static constexpr int k_debugInvalidIndex = -1;

		std::vector<MRI::Modifier::HermiteInterpolatorModifier::KeyFrame> m_keyFrameList = {};

		ImVec2 m_debugContextClickPos = {};
		ImVec2 m_graphSize            = k_debugInitialGraphSize;

		int m_debugDraggingSelectedIndex = k_debugInvalidIndex;
		int m_debugDeleteSelectedIndex   = k_debugInvalidIndex;

		bool m_debugShowAddContext = false;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Modifier::HermiteInterpolatorModifier , MRI::Modifier::InterpolatorModifierBase);