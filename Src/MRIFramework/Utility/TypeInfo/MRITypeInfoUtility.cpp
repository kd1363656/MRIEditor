#include "MRITypeInfoUtility.h"

void MRI::TypeInfoUtility::RegisterAll()
{
	auto& l_registry = MRI::TypeInfoRegistry::GetInstance();

	MRI::TypeInfoUtility::RegisterTag                 (l_registry);
	MRI::TypeInfoUtility::RegisterComponent           (l_registry);
	MRI::TypeInfoUtility::RegisterStrategy            (l_registry);
	MRI::TypeInfoUtility::RegisterComponentMode       (l_registry);
	MRI::TypeInfoUtility::RegisterInterpolatorModifier(l_registry);
}

void MRI::TypeInfoUtility::RegisterTag(MRI::TypeInfoRegistry& a_registry)
{
	// レジストリへの登録は"try_emplace"で重複チェックしているので問題なし
	// ルートタグ
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::TagBase>());

	// ルートタグ"SceneShiftEventTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::SceneShiftEventTagBase>());

	MRI::TypeInfoUtility::RegisterInputEventTag(a_registry);
	MRI::TypeInfoUtility::RegisterBoolTag	   (a_registry);
	MRI::TypeInfoUtility::RegisterEventLaneTag (a_registry);
	MRI::TypeInfoUtility::RegisterCameraKindTag(a_registry);
	MRI::TypeInfoUtility::RegisterShaderTypeTag(a_registry);
	MRI::TypeInfoUtility::RegisterAxisTag      (a_registry);
}
void MRI::TypeInfoUtility::RegisterBoolTag(MRI::TypeInfoRegistry& a_registry)
{
	// ルート"BoolTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::BoolTagBase>());

	// "Bool"タグ
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::BoolTagTrue>   ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::BoolTagFalse>  ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::BoolTagNeither>());
}
void MRI::TypeInfoUtility::RegisterEventLaneTag(MRI::TypeInfoRegistry& a_registry)
{
	// ルート"BoolTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::EventLaneTagBase>());

	// "Bool"タグ
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::EventLaneKeep>  ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::EventLaneMoment>());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::EventLaneNone>  ());
}
void MRI::TypeInfoUtility::RegisterCameraKindTag(MRI::TypeInfoRegistry& a_registry)
{
	// ルートタグ"CameraTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::CameraTagBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::CameraMainTag>());
}
void MRI::TypeInfoUtility::RegisterShaderTypeTag(MRI::TypeInfoRegistry& a_registry)
{
	// ルートタグ"ShaderTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::ShaderTagBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::ShaderTagGenerateDepthMapFromLight>());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::ShaderTagUnLit>					  ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::ShaderTagLit>					  ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::ShaderTagEffect>					  ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::ShaderTagBright>					  ());
}
void MRI::TypeInfoUtility::RegisterInputEventTag(MRI::TypeInfoRegistry& a_registry)
{
	// ルートタグ"InputEventTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::InputEventTagBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::InputRightEventTag>   ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::InputLeftEventTag>    ());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::InputBackwardEventTag>());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::InputForwardEventTag> ());
}

void MRI::TypeInfoUtility::RegisterAxisTag(MRI::TypeInfoRegistry& a_registry)
{
	// ルートタグ"AxisTagBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::AxisTagBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::AxisTagX>());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::AxisTagY>());
	a_registry.Register(MRI::GetTypeInfo<MRI::Tag::AxisTagZ>());
}

void MRI::TypeInfoUtility::RegisterComponent(MRI::TypeInfoRegistry& a_registry)
{
	// ルートクラス"ComponentBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::ComponentBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Component::TransformComponent>			());
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::CameraComponent>				());
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::RenderModelComponentBase>		());
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::RenderModelAnimationComponent>	());
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::RenderModelStaticComponent>	());
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::RotationComponent>				());
	a_registry.Register(MRI::GetTypeInfo<MRI::Component::InputComponent>				());
}

void MRI::TypeInfoUtility::RegisterComponentMode(MRI::TypeInfoRegistry& a_registry)
{
	// ルートクラス"ComponentModeBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::ComponentModeBase>());

	// 回転
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentModeBase>         ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLinearModeBase>   ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLinearByMouseMode>());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLerpModeBase>     ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::RotationComponentLerpByMouseMode>  ());

	// 移動
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentModeBase>                 ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLinearModeBase>           ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLinearFacingDirectionMode>());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::MoveComponentLerpModeBase>             ());

	// 入力
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::InputComponentModeBase>       ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::InputComponentMoveModeBase>   ());
	a_registry.Register(MRI::GetTypeInfo<MRI::ComponentMode::InputComponentFourWayMoveMode>());
}

void MRI::TypeInfoUtility::RegisterStrategy(MRI::TypeInfoRegistry& a_registry)
{
	// ルートクラス"MatrixStrategyBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Strategy::MatrixStrategyBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Strategy::MatrixStrategyCreateSRT>());
}

void MRI::TypeInfoUtility::RegisterInterpolatorModifier(MRI::TypeInfoRegistry& a_registry)
{
	// ルートクラス"InterpolatorModifierBase"
	a_registry.Register(MRI::GetTypeInfo<MRI::Modifier::InterpolatorModifierBase>());

	a_registry.Register(MRI::GetTypeInfo<MRI::Modifier::HermiteInterpolatorModifier>());
}