#pragma once

namespace MRI::Tag
{
	struct InputEventTagBase : MRI::Tag::TagBase {};

	// 右手系座標なので手前を前とした座標系(前 : "Backward" , 後ろ : "Forward")
	struct InputRightEventTag    : MRI::Tag::InputEventTagBase {};
	struct InputLeftEventTag     : MRI::Tag::InputEventTagBase {};
	struct InputBackwardEventTag : MRI::Tag::InputEventTagBase {};
	struct InputForwardEventTag  : MRI::Tag::InputEventTagBase {};
}

MRI_REGISTER_TYPE_INFO(MRI::Tag::InputEventTagBase , MRI::Tag::TagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::InputRightEventTag    , MRI::Tag::InputEventTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::InputLeftEventTag     , MRI::Tag::InputEventTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::InputForwardEventTag  , MRI::Tag::InputEventTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::InputBackwardEventTag , MRI::Tag::InputEventTagBase);