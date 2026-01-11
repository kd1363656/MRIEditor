#pragma once

namespace MRI::Tag
{
	struct EventLaneTagBase : MRI::Tag::TagBase {};
	
	struct EventLaneNone   final : MRI::Tag::EventLaneTagBase {};
	struct EventLaneMoment final : MRI::Tag::EventLaneTagBase {};
	struct EventLaneKeep   final : MRI::Tag::EventLaneTagBase {};
}

MRI_REGISTER_TYPE_INFO(MRI::Tag::EventLaneTagBase , MRI::Tag::TagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::EventLaneNone   , MRI::Tag::EventLaneTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::EventLaneMoment , MRI::Tag::EventLaneTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::EventLaneKeep   , MRI::Tag::EventLaneTagBase);