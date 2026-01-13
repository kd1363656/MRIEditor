#pragma once

namespace MRI::Helper
{
	// ヘルパーはなるべき継承を使わない
	// 他のクラスに欲しい機能をまとめたモジュールとして扱う
	class ComponentHelperBase
	{
	public:
	
		ComponentHelperBase         () = default;
		virtual ~ComponentHelperBase() = default;
	
		virtual void Init        ()											 { /* 必要に応じてオーバーライドしてください */ };
		virtual void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) { /* 必要に応じてオーバーライドしてください */ };
	
		virtual void Update() { /* 必要に応じてオーバーライドしてください */ };

		virtual void EditSpawnInspector () { /* 必要に応じてオーバーライドしてください */ };
		virtual void EditPrefabInspector() { /* 必要に応じてオーバーライドしてください */ };
	
		virtual void DeserializeSpawn (const nlohmann::json& a_json) { /* 必要に応じてオーバーライドしてください */ };
		virtual void DeserializePrefab(const nlohmann::json& a_json) { /* 必要に応じてオーバーライドしてください */ };
	
		virtual nlohmann::json SerializeSpawn () { return nlohmann::json(); }
		virtual nlohmann::json SerializePrefab() { return nlohmann::json(); }
	};
}