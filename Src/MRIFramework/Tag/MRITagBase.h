#pragma once

// エディターとのシナジーが良い
// ビットシフトに比べて処理速度が落ちてしまう(複数のタグを持つとき、引きくするときに配列になるから)
namespace MRI::Tag
{
	struct TagBase {};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::Tag::TagBase);