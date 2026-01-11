#pragma once

namespace MRI::TagUtility
{
	inline std::uint32_t InvertBoolFlagTag(const std::uint32_t a_flag)
	{
		if (a_flag == MRI::StaticID::GetTypeID<MRI::Tag::BoolTagTrue>())
		{
			return MRI::StaticID::GetTypeID<MRI::Tag::BoolTagFalse>();
		}
		else if (a_flag == MRI::StaticID::GetTypeID<MRI::Tag::BoolTagFalse>())
		{
			return MRI::StaticID::GetTypeID<MRI::Tag::BoolTagTrue>();
		}

		return MRI::StaticID::GetTypeID<MRI::Tag::BoolTagNeither>();
	}
}