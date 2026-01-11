#pragma once

namespace MRI::Tag
{
	struct BoolTagBase : MRI::Tag::TagBase {};
	
	struct BoolTagTrue    final : MRI::Tag::BoolTagBase {};
	struct BoolTagFalse   final : MRI::Tag::BoolTagBase {};
	struct BoolTagNeither final : MRI::Tag::BoolTagBase {};
}

MRI_REGISTER_TYPE_INFO(MRI::Tag::BoolTagBase , MRI::Tag::TagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::BoolTagTrue    , MRI::Tag::BoolTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::BoolTagFalse   , MRI::Tag::BoolTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::BoolTagNeither , MRI::Tag::BoolTagBase);