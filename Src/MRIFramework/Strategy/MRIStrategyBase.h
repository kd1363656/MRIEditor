#pragma once

// 継承のルートクラスだが継承関係を辿る必要もないかつテンプレートを使っている関係上
// 継承関係を辿るのが難しいので、ルートクラスとして各派生クラスを扱う
namespace MRI::Strategy
{
	template <typename Argument>
	class StrategyBase
	{
	public:
	
		StrategyBase         () = default;
		virtual ~StrategyBase() = default;

		virtual const MRI::TypeInfo& GetTypeInfo() const = 0;
	
		virtual void Execute(Argument& a_argument) = 0;
	};
}