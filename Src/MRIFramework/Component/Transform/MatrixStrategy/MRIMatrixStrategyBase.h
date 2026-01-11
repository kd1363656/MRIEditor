#pragma once

namespace MRI::Strategy
{
	// 基底クラスと派生クラスの"TypeInfo"を整理するための基底クラス
	class MatrixStrategyBase : public MRI::Strategy::StrategyBase<std::weak_ptr<MRI::Component::TransformComponent>>
	{
	public:
	
		MatrixStrategyBase ()          = default;
		~MatrixStrategyBase() override = default;
	
		const MRI::TypeInfo& GetTypeInfo() const override;
	};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::Strategy::MatrixStrategyBase);