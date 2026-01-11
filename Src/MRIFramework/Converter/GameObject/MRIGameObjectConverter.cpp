#include "MRIGameObjectConverter.h"

void MRI::Converter::GameObjectConverter::Deserialize(const nlohmann::json& a_json) const
{
	if (a_json.is_null()) { return; }

	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache) { return; }

	auto l_sceneCache = SceneManager::GetInstance().GetSceneCache().lock();
	if (!l_sceneCache) { return; }

	auto l_prefabControllerCache = l_sceneCache->GetPrefabControllerCache().lock();
	if (!l_prefabControllerCache) { return; }

	// ゲームオブジェクトの名前を読み取る
	const std::string l_prefabName = a_json.value("PrefabName" , std::string());
	l_ownerCache->SetPrefabName					 (l_prefabName);

	// ゲームオブジェクトの名前からプレハブ情報を取得
	auto l_prefabCache = l_prefabControllerCache->FetchPrefabCache(l_prefabName).lock();
	if (!l_prefabCache) 
	{
		MRI_ADD_LOG("GameObject\"%s\"のプレハブが存在しません。" , l_prefabName.c_str());
		return; 
	}

	auto l_prefabJson = l_prefabCache->GetJson();
	if (l_prefabJson.is_null()) { return; }

	std::vector<std::shared_ptr<MRI::Component::ComponentBase>> l_componentLoadList;
	std::vector<MRI::CommonStruct::ChildLoad>                   l_childLoadList;

	// 子も含むプレハブをデシリアライズしてからスポーン情報をデシリアライズ
	DeserializePrefab(l_prefabJson , l_componentLoadList , l_childLoadList);
	DeserializeSpawn (a_json       , l_componentLoadList , l_childLoadList);

	// コンポーネントを親、子に再帰的に追加
	MRI::GameObjectUtility::RecursiveAddComponent(l_ownerCache , l_componentLoadList , l_childLoadList);

	// 親子関係を再帰的に構築
	MRI::GameObjectUtility::RecursiveAddChild(l_ownerCache , l_childLoadList);
}

void MRI::Converter::GameObjectConverter::DeserializePrefab(const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const
{
	if (a_json.is_null()) { return; }

	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache) { return; }

	auto l_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// "TransformComponent"のプレハブ情報を読み込む
	// 型情報から名前を取得、こうすること"TransformComponent"の名前が変わってもコードを変更する必要がなくなる
	if (const char* l_key = l_selfTransformComponentCache->GetTypeInfo().k_name.data();
		a_json.contains(l_key))
	{
		l_selfTransformComponentCache->DeserializePrefab(a_json[l_key]);
	}
	
	// コンポーネント読み込み用"json"の存在確認、なければ"return"
	if (!MRI::JsonUtility::IsArray(a_json , "ComponentList")) { return; }

	const auto& l_componentFactory = MRI::SharedFactory::Component::GetInstance();

	// コンポーネントの読み込みと生成
	for (const auto& l_json : a_json["ComponentList"])
	{
		if (l_json.is_null()) { continue; }

		// コンポーネントの名前を取得
		const std::string l_typeName = l_json.value("TypeName" , std::string());

		// コンポーネントの名前をもとにファクトリーから生成
		// もしインスタンス化されていなければ"continue"
		auto l_component = l_componentFactory.Create(l_typeName);
		if (!l_component) { continue; }

		// コンポーネントにゲームオブジェクト自身のキャッシュを与え初期化しデシリアライズ
		l_component->SetOwnerCache    (m_ownerCache);
		l_component->Init             ();
		l_component->DeserializePrefab(l_json);

		// コンポーネントの実行順序は"json"に保存された順番であり"Spawn"情報も
		// 読み込む必要があるため順番を崩さないようにリストで管理
		a_componentList.emplace_back(l_component);
	}

	// コンポーネントオブザーバーと子ゲームオブジェクトをデシリアライズ
	DeserializeComponentObserver(a_json);
	DeserializeChildPrefab      (a_json , a_childLoadList);
}
void MRI::Converter::GameObjectConverter::DeserializeSpawn(const nlohmann::json& a_json , std::vector<std::shared_ptr<MRI::Component::ComponentBase>>& a_componentList , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const
{
	if (a_json.is_null()) { return; }

	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache) { return; }

	auto l_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) { return; }

	// 保存していた"UUID"を取得
	UUID l_uuid = MRI::JsonUtility::DeserializeUUID(a_json);

	// "UUID"の読み取りが失敗していたら再出力
	if (l_uuid == GUID_NULL)
	{
		MRI_ADD_LOG("\"GameObject\の\"GUID\"の読み込みに失敗しました。");
		MRI::UUIDUtility::GenerateUUID(l_uuid);
	}

	l_ownerCache->SetUUID(l_uuid);

	// "TransformComponent"のスポーン情報を読み込む
	if (const char* l_key = l_selfTransformComponentCache->GetTypeInfo().k_name.data();
		a_json.contains(l_key))
	{
		l_selfTransformComponentCache->DeserializeSpawn(a_json[l_key]);
	}

	// もし読み込んだコンポーネントのリストがなければ"return"
	if (a_componentList.empty())							  { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "ComponentList")) { return; }

	// 各インデックスに対応する"Spawn"情報を読み込む
	for(std::size_t l_i = 0LLU; l_i < a_json["ComponentList"].size(); ++l_i)
	{
		// もしコンポーネントリストよりも配列サイズを超えてしまった場合
		// 強制的に"for"文を抜ける
		if (l_i >= a_componentList.size())
		{
			break;
		}

		auto& l_json = a_json["ComponentList"][l_i];
		
		// もし"json"が空なら"continue"
		if (l_json.is_null()) 
		{ 
			continue; 
		}

		// 各コンポーネントの"Spawn"情報を読み込む"jsonArray"で保存する関係上
		// 絶対に前回と同じコンポーネントの格納順番なので安全にインデックスを指定して
		// 読み込むことが可能
		a_componentList[l_i]->DeserializeSpawn(l_json);
	}

	DeserializeChildSpawn(a_json , a_childLoadList);
}

nlohmann::json MRI::Converter::GameObjectConverter::SerializePrefab() const
{
	auto l_rootJson = nlohmann::json();

	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache)
	{ 
		return nlohmann::json(); 
	}

	auto l_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache) 
	{
		return nlohmann::json(); 
	}

	// ゲームオブジェクトの名前を保存
	l_rootJson["PrefabName"] = l_ownerCache->GetPrefabName().data();

	// "TransformComponent"のプレハブ情報を保存
	// 出力した"l_json"が"null"でなければ"l_rootJson"に格納
	if (const auto& l_json = l_selfTransformComponentCache->SerializePrefab();
		!l_json.is_null())
	{
		const char* l_key = l_selfTransformComponentCache->GetTypeInfo().k_name.data();
		l_rootJson[l_key] = l_json;
	}

	// 保存順序を保つために"json::array"で保存
	auto l_componentJsonArray = nlohmann::json::array();
	for (const auto& l_component : l_ownerCache->GetComponentList())
	{
		if (!l_component) { continue; }

		auto l_json = nlohmann::json();

		// コンポーネントの名前とそのプレハブ情報を保存
		l_json["TypeName"] = l_component->GetTypeInfo().k_name.data						   ();
		MRI::JsonUtility::UpdateJson				 (l_json , l_component->SerializePrefab());
		
		// もし出力結果が"null"なら"continue"
		if (l_json.is_null()) { continue; }

		l_componentJsonArray.emplace_back(l_json);
	}

	// 自身を構成するコンポーネントを保存
	l_rootJson["ComponentList"] = l_componentJsonArray;

	// コンポーネントオブザーバーをシリアライズ
	MRI::JsonUtility::UpdateJson(l_rootJson , SerializeComponentObserver());
	
	// 子ゲームオブジェクトの保存
	MRI::JsonUtility::UpdateJson(l_rootJson , SerializeChildPrefab());
	
	return l_rootJson;
}
nlohmann::json MRI::Converter::GameObjectConverter::SerializeSpawn() const
{
	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache)
	{
		return nlohmann::json();
	}

	auto l_selfTransformComponentCache = l_ownerCache->GetSelfTransformComponentCache().lock();
	if (!l_selfTransformComponentCache)
	{
		return nlohmann::json();
	}

	auto l_rootJson = nlohmann::json();

	// ゲームオブジェクトの名前を保存
	l_rootJson["PrefabName"] = l_ownerCache->GetPrefabName().data();

	// "UUID"の保存
	MRI::JsonUtility::UpdateJson(l_rootJson , MRI::JsonUtility::SerializeUUID(l_ownerCache->GetUUID()));

	// "TransformComponent"の"Spawn"情報を保存する
	if (const auto& l_json = l_selfTransformComponentCache->SerializeSpawn();
		!l_json.is_null())
	{
		const char* l_key = l_selfTransformComponentCache->GetTypeInfo().k_name.data();
		l_rootJson[l_key] = l_json;
	}

	// 保存順序を保つために"json::array"でコンポーネントを保存
	auto l_componentJsonArray = nlohmann::json::array();

	for (const auto& l_component : l_ownerCache->GetComponentList())
	{
		if (!l_component) { continue; }

		auto l_json = nlohmann::json();

		l_json["TypeName"] = l_component->GetTypeInfo().k_name.data						  ();
		MRI::JsonUtility::UpdateJson				 (l_json , l_component->SerializeSpawn());

		// もし出力結果が"null"なら"continue"
		if (l_json.is_null()) { continue; }

		l_componentJsonArray.emplace_back(l_json);
	}

	// 自身を構成するコンポーネントを保存
	l_rootJson["ComponentList"] = l_componentJsonArray;

	// 子ゲームオブジェクトの保存
	MRI::JsonUtility::UpdateJson(l_rootJson , SerializeChildSpawn());

	return l_rootJson;
}

void MRI::Converter::GameObjectConverter::DeserializeComponentObserver(const nlohmann::json& a_json) const
{
	if (a_json.is_null())					   { return; }
	if (!a_json.contains("ComponentObserver")) { return; }

	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache) { return; }

	auto l_componentObserverCache = l_ownerCache->GetComponentObserverCache().lock();
	if (!l_componentObserverCache) { return; }

	// コンポーネントオブザーバーの読み込み
	l_componentObserverCache->Deserialize(a_json["ComponentObserver"]);	
}
void MRI::Converter::GameObjectConverter::DeserializeChildPrefab(const nlohmann::json& a_json , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const
{
	if (a_json.is_null())								  { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "ChildList")) { return; }

	// 子はプレハブから読み込む生成する(再帰的な処理なので注意)
	for (const auto& l_json : a_json["ChildList"])
	{
		if (l_json.is_null()) { continue; }

		// リストが回るたびに子ゲームオブジェクトが"json"保存前に存在していたということなので
		// 毎回ゲームオブジェクトを生成
		MRI::CommonStruct::ChildLoad l_childLoad;

		// 子を初期化
		auto l_child = MRI::GameObjectUtility::CreateGameObject();
		l_child->Init();
	
		// 子のプレハブ名を読み込む
		const std::string l_prefabName = l_json.value("PrefabName" , MRI::CommonConstant::k_stringUnknown.data());
		l_child->SetPrefabName						 (l_prefabName);

		// プレハブ情報をデシリアライズ
		l_child->DeserializePrefab(l_json , l_childLoad.componentLoadList , l_childLoad.childLoadList);

		// "child"を自身に格納
		l_childLoad.self = l_child;

		// ロード用リストに格納
		a_childLoadList.emplace_back(l_childLoad);
	}
}
void MRI::Converter::GameObjectConverter::DeserializeChildSpawn(const nlohmann::json& a_json , std::vector<MRI::CommonStruct::ChildLoad>& a_childLoadList) const
{
	// リストが空なら読み込めていないので"return"
	if (a_childLoadList.empty())						  { return; }
	if (a_json.is_null())								  { return; }
	if (!MRI::JsonUtility::IsArray(a_json , "ChildList")) { return; }

	// 子の保存順序は"json::array"で保証されているので安全に"Spawn"情報を読み取ることができる
	for (std::size_t l_i = 0LLU; l_i < a_json["ChildList"].size(); ++l_i)
	{
		// もしプレハブで読み取った子以上の要素数を示すなら
		// 強制的に"for"文を抜ける
		if (l_i >= a_childLoadList.size())
		{
			break;
		}

		auto& l_json = a_json["ChildList"][l_i];

		// もし"json"が空なら"continue"
		if (l_json.is_null())
		{
			continue;
		}

		auto& l_childLoad = a_childLoadList[l_i];

		l_childLoad.self->DeserializeSpawn(l_json , l_childLoad.componentLoadList , l_childLoad.childLoadList);
	}
}

nlohmann::json MRI::Converter::GameObjectConverter::SerializeComponentObserver() const
{
	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache) 
	{
		return nlohmann::json();
	}

	auto l_componentObserverCache = l_ownerCache->GetComponentObserverCache().lock();
	if (!l_componentObserverCache)
	{
		return nlohmann::json();
	}

	auto l_rootJson = nlohmann::json();

	// コンポーネントオブザーバーのプレハブ情報を保存
	const auto& l_json = l_componentObserverCache->Serialize();
	if (!l_json.is_null())
	{
		l_rootJson["ComponentObserver"] = l_json;
	}

	return l_rootJson;
}
nlohmann::json MRI::Converter::GameObjectConverter::SerializeChildPrefab() const
{
	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache)
	{
		return nlohmann::json();
	}

	auto l_rootJson       = nlohmann::json       ();
	auto l_childJsonArray = nlohmann::json::array();

	// ルートから全ての子情報を再帰的に保存していく
	for (const auto& l_childCache : l_ownerCache->GetChildCacheList())
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { continue; }

		auto l_json = l_child->SerializePrefab();
		if (!l_json.is_null())
		{
			l_childJsonArray.emplace_back(l_json);
		}
	}

	l_rootJson["ChildList"] = l_childJsonArray;

	return l_rootJson;
}
nlohmann::json MRI::Converter::GameObjectConverter::SerializeChildSpawn() const
{
	auto l_ownerCache = m_ownerCache.lock();
	if (!l_ownerCache)
	{
		return nlohmann::json();
	}

	auto l_rootJson       = nlohmann::json       ();
	auto l_childJsonArray = nlohmann::json::array();

	// ルートから全ての子情報を再帰的に保存していく
	for (const auto& l_childCache : l_ownerCache->GetChildCacheList())
	{
		auto l_child = l_childCache.lock();
		if (!l_child) { continue; }

		auto l_json = l_child->SerializeSpawn();
		if (!l_json.is_null())
		{
			l_childJsonArray.emplace_back(l_json);
		}
	}

	l_rootJson["ChildList"] = l_childJsonArray;

	return l_rootJson;
}