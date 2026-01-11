#include "MRITypeInfoRegistry.h"

void MRI::TypeInfoRegistry::Register(const TypeInfo& a_info)
{
	const bool l_isInvalidID   = a_info.k_id   == MRI::CommonConstant::k_invalidStaticID;
	const bool l_isInvalidName = a_info.k_name == std::string_view();

	assert((!l_isInvalidID && !l_isInvalidName) && "登録されていない\"MRI::TypeInfo\"の\"k_id\"か\"k_name\"を検出。");

	m_allTypeInfoIDMap.try_emplace  (a_info.k_id   , &a_info);
	m_allTypeInfoNameMap.try_emplace(a_info.k_name , &a_info);
}

const MRI::TypeInfo* MRI::TypeInfoRegistry::FetchTypeInfoByID(const std::uint32_t a_id) const
{
	auto l_itr = m_allTypeInfoIDMap.find(a_id);
	if (l_itr == m_allTypeInfoIDMap.end()) 
	{ 
		return nullptr;
	}

	return l_itr->second;
}

const MRI::TypeInfo* MRI::TypeInfoRegistry::FetchTypeInfoByName(const std::string_view& a_name) const
{
	auto l_itr = m_allTypeInfoNameMap.find(a_name);
	if (l_itr == m_allTypeInfoNameMap.end()) 
	{
		return nullptr; 
	}

	return l_itr->second;
}