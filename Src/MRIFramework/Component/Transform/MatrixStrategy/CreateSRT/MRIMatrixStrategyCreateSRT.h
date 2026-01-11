#pragma once

namespace MRI::Component
{
	class TransformComponent;
}

namespace MRI::Strategy
{
	class MatrixStrategyCreateSRT final : public MRI::Strategy::MatrixStrategyBase
	{
	public:
	
		MatrixStrategyCreateSRT ()          = default;
		~MatrixStrategyCreateSRT() override = default;
	
		const MRI::TypeInfo& GetTypeInfo() const override;

		void Execute(std::weak_ptr<MRI::Component::TransformComponent>& a_owner) override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Strategy::MatrixStrategyCreateSRT , MRI::Strategy::MatrixStrategyBase);