#pragma once

namespace MRI::ComponentMode
{
	class InputComponentFourWayMoveMode final : public MRI::ComponentMode::InputComponentMoveModeBase
	{
	public:

		InputComponentFourWayMoveMode ()          = default;
		~InputComponentFourWayMoveMode() override = default;

		const MRI::TypeInfo& GetTypeInfo() const override;

		void EarlyUpdate() override;

	private:

		// 指定された入力タグから仮想キーコードを取得しベクトルを足しこむ
		template <MRI::Concept::IsDerivedInputEventTagBaseConcept Type>
		void AddMoveInputIfKeyPressed(Math::Vector3& a_target , const Math::Vector3& a_inputDirection)
		{
			auto l_playerKeyConfigCache = MRI::SceneManager::GetInstance().GetPlayerKeyConfigCache().lock();
			if (!l_playerKeyConfigCache) { return; }

			if (!l_playerKeyConfigCache->IsInputHeld<Type>()) { return; }
			MRI::MathUtility::AddDirection(a_target , a_inputDirection);
		}
	};
}

MRI_REGISTER_TYPE_INFO(MRI::ComponentMode::InputComponentFourWayMoveMode , MRI::ComponentMode::InputComponentMoveModeBase);