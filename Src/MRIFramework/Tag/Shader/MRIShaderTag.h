#pragma once

namespace MRI::Tag
{
	struct ShaderTagBase : MRI::Tag::TagBase {};

	struct ShaderTagGenerateDepthMapFromLight : MRI::Tag::ShaderTagBase {};
	struct ShaderTagUnLit					  : MRI::Tag::ShaderTagBase {};
	struct ShaderTagLit						  : MRI::Tag::ShaderTagBase {};
	struct ShaderTagEffect					  : MRI::Tag::ShaderTagBase {};
	struct ShaderTagBright					  : MRI::Tag::ShaderTagBase {};
}

MRI_REGISTER_TYPE_INFO(MRI::Tag::ShaderTagBase , MRI::Tag::TagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::ShaderTagGenerateDepthMapFromLight , MRI::Tag::ShaderTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::ShaderTagUnLit					    , MRI::Tag::ShaderTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::ShaderTagLit					    , MRI::Tag::ShaderTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::ShaderTagEffect					, MRI::Tag::ShaderTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::ShaderTagBright					, MRI::Tag::ShaderTagBase);