#pragma once

namespace MRI
{
	class TypeInfoRegistry;
}

namespace MRI::TypeInfoUtility
{
	void RegisterAll();

	void RegisterTag          (MRI::TypeInfoRegistry& a_registry);
	void RegisterBoolTag      (MRI::TypeInfoRegistry& a_registry);
	void RegisterEventLaneTag (MRI::TypeInfoRegistry& a_registry);
	void RegisterCameraKindTag(MRI::TypeInfoRegistry& a_registry);
	void RegisterShaderTypeTag(MRI::TypeInfoRegistry& a_registry);
	void RegisterInputEventTag(MRI::TypeInfoRegistry& a_registry);
	void RegisterAxisTag      (MRI::TypeInfoRegistry& a_registry);

	void RegisterComponent(MRI::TypeInfoRegistry& a_registry);

	void RegisterComponentMode(MRI::TypeInfoRegistry& a_registry);

	void RegisterStrategy(MRI::TypeInfoRegistry& a_registry);

	void RegisterInterpolatorModifier(MRI::TypeInfoRegistry& a_registry);

	inline bool IsDerivedFrom(const MRI::TypeInfo& a_typeInfo , const MRI::TypeInfo& a_baseTypeInfo)
	{
		const MRI::TypeInfo* l_currentInfo = &a_typeInfo;
		while (l_currentInfo)
		{
			if (l_currentInfo->k_id == a_baseTypeInfo.k_id)
			{
				return true;
			}

			l_currentInfo = l_currentInfo->k_baseInfo;
		}

		return false;
	}

	// ※注意 このキャストを使うには"TypeInfo"の登録が"Type"と"BaseType"型のオブジェクトに必要不可欠
	template <typename BaseType , typename Type>
	std::weak_ptr<BaseType> SafeCast(const std::shared_ptr<Type> a_instance)
	{
		if (!a_instance) 
		{
			return std::weak_ptr<BaseType>();
		}

		const auto& l_typeInfo     = a_instance->GetTypeInfo   ();
		const auto& l_baseTypeInfo = MRI::GetTypeInfo<BaseType>();

		// もし継承関係が成立しなければ空の"std::weak_ptr<BaseType>"を返す
		const bool l_isDerivedFrom = MRI::TypeInfoUtility::IsDerivedFrom(l_typeInfo , l_baseTypeInfo);
		if (!l_isDerivedFrom) 
		{
			return std::weak_ptr<BaseType>();
		}

		return std::static_pointer_cast<BaseType>(a_instance);
	}
}