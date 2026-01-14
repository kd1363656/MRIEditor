#pragma once

namespace MRI::Component
{
	class TransformComponent;
}

namespace MRI::Strategy
{
	class MatrixStrategyCreateSTRParentT final : public MRI::Strategy::MatrixStrategyBase
	{
	public:

		MatrixStrategyCreateSTRParentT ()          = default;
		~MatrixStrategyCreateSTRParentT() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void Execute(std::weak_ptr<MRI::Component::TransformComponent>& a_ownerCache) override;
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Strategy::MatrixStrategyCreateSTRParentT , MRI::Strategy::MatrixStrategyBase);