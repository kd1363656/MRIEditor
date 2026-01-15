#pragma once

namespace MRI::Tag
{
	struct CameraTagBase : MRI::Tag::TagBase {};

	struct CameraMainTag  final : public MRI::Tag::CameraTagBase {};
	struct CameraDebugTag final : public MRI::Tag::CameraTagBase {};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::Tag::CameraTagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::CameraMainTag  , MRI::Tag::CameraTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::CameraDebugTag , MRI::Tag::CameraTagBase);