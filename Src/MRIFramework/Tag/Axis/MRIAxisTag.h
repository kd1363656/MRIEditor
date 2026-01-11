#pragma once

namespace MRI::Tag
{
	struct AxisTagBase : MRI::Tag::TagBase {};

	struct AxisTagX : MRI::Tag::AxisTagBase {};
	struct AxisTagY : MRI::Tag::AxisTagBase {};
	struct AxisTagZ : MRI::Tag::AxisTagBase {};
}

MRI_REGISTER_TYPE_INFO(MRI::Tag::AxisTagBase , MRI::Tag::TagBase);

MRI_REGISTER_TYPE_INFO(MRI::Tag::AxisTagX , MRI::Tag::AxisTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::AxisTagY , MRI::Tag::AxisTagBase);
MRI_REGISTER_TYPE_INFO(MRI::Tag::AxisTagZ , MRI::Tag::AxisTagBase);