#pragma once

namespace MRI::Tag
{
	struct InputEventTagBase : MRI::Tag::TagBase {};

	struct InputRightEventTag : MRI::Tag::InputEventTagBase {};
	struct InputLeftEventTag  : MRI::Tag::InputEventTagBase {};
	struct InputUpEventTag    : MRI::Tag::InputEventTagBase {};
	struct InputDownEventTag  : MRI::Tag::InputEventTagBase {};
}

MRI_REGISTER_TYPE_INFO(MRI::Tag::InputEventTagBase , MRI::Tag::TagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::InputRightEventTag , MRI::Tag::InputEventTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::InputLeftEventTag  , MRI::Tag::InputEventTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::InputDownEventTag  , MRI::Tag::InputEventTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::InputUpEventTag    , MRI::Tag::InputEventTagBase);