#pragma once

namespace MRI::JsonUtility
{
	inline void UpdateJson(nlohmann::json& a_targetJson , const nlohmann::json& a_patchJson)
	{
		if (a_patchJson.is_null()) { return; }
		a_targetJson.update(a_patchJson);
	}

	// 読み込んだ"json"が配列かどうかを確認
	inline bool IsArray(const nlohmann::json& a_json , const std::string_view a_key)
	{
		// 文字列が空でなくそのキーが含まれているかをチェック
		if (a_json.is_null() || a_key.empty() || !a_json.contains(a_key.data()))
		{
			return false;
		}

		// もし文字列が空(デフォルト引数)なら"json"自体が配列なのでキーを含めずに配列化チェック
		// そうでなければキーに連続した配列なのでキーを含めた配列チェックを行う
		const bool l_isArray = a_key.empty() ? a_json.is_array() : a_json[a_key.data()].is_array();
		return l_isArray;
	}

	// "RVO"を実行するため"return"の際は一時オブジェクトを生成している
	// 戻り値を変数に格納してしまうと"RVO"が絶対に働くことが保証されないため
	inline Math::Vector3 DeserializeVec3(const nlohmann::json& a_json , const std::string_view a_key)
	{
		// "json"を読み込めるか確認、読み込めなければ"return"
		if (a_json.is_null() || a_key.empty() || !a_json.contains(a_key.data()))
		{
			return Math::Vector3();
		}

		const auto& l_json = a_json[a_key.data()];
		if (l_json.is_null()) 
		{
			return Math::Vector3();
		}

		return Math::Vector3
		{
			l_json.value("X" , Math::Vector3::Zero.x) ,
			l_json.value("Y" , Math::Vector3::Zero.y) ,
			l_json.value("Z" , Math::Vector3::Zero.z)
		};
	}

	inline nlohmann::json SerializeVec3(const Math::Vector3& a_value , const std::string_view a_key)
	{
		// キーとなる文字列がなければ空の"json"を返す
		if (a_key.empty())
		{
			return nlohmann::json();
		}

		return nlohmann::json
		{
			{
				a_key.data() ,
				{
					{ "X" , a_value.x } ,
					{ "Y" , a_value.y } ,
					{ "Z" , a_value.z }
				}
			}
		};
	}

	inline Math::Quaternion DeserializeQuaternion(const nlohmann::json& a_json , const std::string_view a_key)
	{
		// "json"を読み込めるか確認、読み込めなければ"return"
		if (a_json.is_null() || a_key.empty() || !a_json.contains(a_key.data()))
		{
			return Math::Quaternion();
		}

		const auto& l_json = a_json[a_key.data()];
		if (l_json.is_null()) 
		{
			return Math::Quaternion();
		}

		return Math::Quaternion
		{
			l_json.value("X" , Math::Quaternion::Identity.x) ,
			l_json.value("Y" , Math::Quaternion::Identity.y) ,
			l_json.value("Z" , Math::Quaternion::Identity.z) ,
			l_json.value("W" , Math::Quaternion::Identity.w) ,
		};
	}

	inline nlohmann::json SerializeQuaternion(const Math::Quaternion& a_value , const std::string_view a_key)
	{
		// キーとなる文字列がなければ空の"json"を返す
		if (a_key.empty())
		{
			return nlohmann::json();
		}

		return nlohmann::json
		{
			{
				a_key.data() ,
				{
					{ "X" , a_value.x } ,
					{ "Y" , a_value.y } ,
					{ "Z" , a_value.z } ,
					{ "W" , a_value.w } ,
				}
			}
		};
	}

	inline Math::Color DeserializeColor(const nlohmann::json& a_json , const std::string_view a_key)
	{
		// "json"を読み込めるか確認、読み込めなければ"return"
		if (a_json.is_null() || a_key.empty() || !a_json.contains(a_key.data()))
		{
			return Math::Color();
		}

		const auto& l_json = a_json[a_key.data()];
		if (l_json.is_null())
		{
			return Math::Color();
		}

		return Math::Color
		{
			l_json.value("X" , MRI::CommonConstant::k_colorLess.x) ,
			l_json.value("Y" , MRI::CommonConstant::k_colorLess.y) ,
			l_json.value("Z" , MRI::CommonConstant::k_colorLess.z) ,
			l_json.value("W" , MRI::CommonConstant::k_colorLess.w)
		};
	}

	inline nlohmann::json SerializeColor(const Math::Color& a_value , const std::string_view a_key)
	{
		// キーとなる文字列がなければ空の"json"を返す
		if (a_key.empty())
		{
			return nlohmann::json();
		}

		return nlohmann::json
		{
			{
				a_key.data() ,
				{
					{ "X" , a_value.x } ,
					{ "Y" , a_value.y } ,
					{ "Z" , a_value.z } ,
					{ "W" , a_value.w } ,
				}
			}
		};
	}

	inline UUID DeserializeUUID(const nlohmann::json& a_json)
	{ 
		return MRI::UUIDUtility::StringToUUID(a_json.value("UUID" , MRI::CommonConstant::k_stringUnknown.data()));
	}
	inline nlohmann::json SerializeUUID(const UUID& a_uuid)
	{ 
		return nlohmann::json
		{ 
			{ "UUID" , MRI::UUIDUtility::UUIDToString(a_uuid) }
		};
	}

	inline std::uint32_t DeserializeTag(const nlohmann::json& a_json)
	{
		const auto& l_tagRegistry = MRI::TypeInfoRegistry::GetInstance();

		std::string l_tagName = a_json.value("TagName" , MRI::CommonConstant::k_stringUnknown.data());
		
		const MRI::TypeInfo* l_typeInfo = l_tagRegistry.FetchTypeInfoByName(l_tagName);
		if(!l_typeInfo) 
		{
			return MRI::CommonConstant::k_invalidStaticID;
		}

		return l_typeInfo->k_id;
	}

	inline nlohmann::json SerializeTag(const std::uint32_t a_tag)
	{
		const auto& l_tagRegistry = MRI::TypeInfoRegistry::GetInstance();

		const TypeInfo* l_typeInfo = l_tagRegistry.FetchTypeInfoByID(a_tag);
		if (!l_typeInfo) 
		{
			// デバックしやすいように明示意的に"Unknown"を含んだ"json"を返す
			return nlohmann::json
			{ 
				{"TagName" , MRI::CommonConstant::k_stringUnknown.data()}
			};
		}

		return nlohmann::json
		{ 
			{"TagName" , l_typeInfo->k_name.data()}
		};
	}

	// ファクトリーからインスタンスを生成しプレハブ情報を読み込む
	template <typename FactoryType , typename Type>
		requires MRI::Concept::IsSmartPTRConcept<Type>
	inline void DeserializeInstancePrefab(const nlohmann::json& a_json , const char* a_key , Type& a_instance)
	{
		if (a_json.is_null()) { return; }

		const auto& l_factory = FactoryType::GetInstance();

		const std::string l_createName = a_json.value(a_key , std::string());
		if (l_createName.empty()) { return; }

		a_instance = l_factory.Create(l_createName.c_str());

		if (a_instance)
		{
			a_instance->Init             ();
			a_instance->DeserializePrefab(a_json);
		}
	}

	// インスタンスから型情報を保存しプレハブ情報を保存
	template <typename Type>
		requires MRI::Concept::IsSmartPTRConcept<Type>
	inline nlohmann::json SerializeInstancePrefab(const char* a_key , Type& a_instance)
	{
		if (!a_instance) 
		{
			return nlohmann::json(); 
		}

		auto l_rootJson = nlohmann::json();

		l_rootJson[a_key] = a_instance->GetTypeInfo().k_name.data();

		UpdateJson(l_rootJson , a_instance->SerializePrefab());

		return l_rootJson;
	}
}