#pragma once

namespace MRI
{
	// "GenerateTypeID"をカプセル化するためだけにクラス化
	class StaticID final
	{
	public:
	
		// テンプレートを使った関数の特殊化を用いて動的に
		// 一意な"ID"を生成している
		template <typename Type>
		static std::uint32_t GetTypeID()
		{
			static std::uint32_t l_id = GenerateTypeID();
			return l_id;
		}
	
	private:
	
		static std::uint32_t GenerateTypeID()
		{
			// "0"が絶対に発行されないことを保証するために"1"から始める
			static std::uint32_t l_id = k_initialID;
			return l_id++;
		}
	
		static constexpr std::uint32_t k_initialID = 1u;
		
		StaticID () = default;
		~StaticID() = default;
	};
}