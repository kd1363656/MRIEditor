#pragma once

namespace MRI::AxisUtility
{
	// その軸を適用していいかどうかを判断する関数
	template <MRI::Concept::IsDerivedAxisTagBaseConcept Type>
	inline bool CanAddAdaptDirection(const std::unordered_set<std::uint32_t>& a_adaptDirectionSet)
	{
		const MRI::TypeInfo& l_typeInfo = MRI::GetTypeInfo<Type>();
		return a_adaptDirectionSet.contains(l_typeInfo.k_id);
	}

	// 適用すべき軸に加算
	inline void AddAdaptAxisValue(const std::unordered_set<std::uint32_t>& a_adaptDirectionSet , Math::Vector3& a_adaptDirection , const Math::Vector3& a_patch)
	{
		// "X"軸の変更を適用
		if (CanAddAdaptDirection<MRI::Tag::AxisTagX>(a_adaptDirectionSet))
		{
			a_adaptDirection.x += a_patch.x;
		}
		if (CanAddAdaptDirection<MRI::Tag::AxisTagY>(a_adaptDirectionSet))
		{
			a_adaptDirection.y += a_patch.y;
		}
		if (CanAddAdaptDirection<MRI::Tag::AxisTagZ>(a_adaptDirectionSet))
		{
			a_adaptDirection.z += a_patch.z;
		}
	}

	// 適用すべき出ない軸の初期化を行う関数
	inline void ResetUnusedAxis(const std::unordered_set<std::uint32_t>& a_adaptDirectionSet , Math::Vector3& a_adaptDirection)
	{
		// "X"軸の変更を適用
		if (!CanAddAdaptDirection<MRI::Tag::AxisTagX>(a_adaptDirectionSet))
		{
			a_adaptDirection.x = 0.0F;
		}
		if (!CanAddAdaptDirection<MRI::Tag::AxisTagY>(a_adaptDirectionSet))
		{
			a_adaptDirection.y = 0.0F;
		}
		if (!CanAddAdaptDirection<MRI::Tag::AxisTagZ>(a_adaptDirectionSet))
		{
			a_adaptDirection.z = 0.0F;
		}
	}

	// 指定された入力タグから仮想キーコードを取得しベクトルを足しこむ
	template <MRI::Concept::IsDerivedInputEventTagBaseConcept Type>
	void ApplyDirectionIfKeyHeld(Math::Vector3& a_target , const Math::Vector3& a_inputDirection)
	{
		auto l_playerKeyConfigCache = MRI::SceneManager::GetInstance().GetPlayerKeyConfigCache().lock();
		if (!l_playerKeyConfigCache) { return; }

		if (!l_playerKeyConfigCache->IsInputHeld<Type>()) { return; }
		MRI::MathUtility::AddDirection(a_target, a_inputDirection);
	}
}