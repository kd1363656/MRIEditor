#pragma once

namespace MRI::ComponentMode
{
	// ヘルパーはなるべき継承を使わない
	// 他のクラスに欲しい機能をまとめたモジュールとして扱う
	class ComponentModeBase
	{
	public:
	
		ComponentModeBase         () = default;
		virtual ~ComponentModeBase() = default;
	
		virtual const MRI::TypeInfo& GetTypeInfo() const;

		virtual void Init        ()											 { /* 必要に応じてオーバーライドしてください */ };
		virtual void PostLoadInit(std::shared_ptr<MRI::GameObject>& a_owner) { /* 必要に応じてオーバーライドしてください */ };
	
		virtual void PreUpdate  () { /* 必要に応じてオーバーライドしてください */ };
		virtual void EarlyUpdate() { /* 必要に応じてオーバーライドしてください */ };
		virtual void Update     () { /* 必要に応じてオーバーライドしてください */ };
		virtual void LateUpdate () { /* 必要に応じてオーバーライドしてください */ };
		virtual void FixMatrix  () { /* 必要に応じてオーバーライドしてください */ };

		virtual void EditSpawnInspector () { /* 必要に応じてオーバーライドしてください */ };
		virtual void EditPrefabInspector() { /* 必要に応じてオーバーライドしてください */ };
	
		virtual void DeserializeSpawn (const nlohmann::json& a_json) { /* 必要に応じてオーバーライドしてください */ };
		virtual void DeserializePrefab(const nlohmann::json& a_json) { /* 必要に応じてオーバーライドしてください */ };
	
		virtual nlohmann::json SerializeSpawn () { return nlohmann::json(); }
		virtual nlohmann::json SerializePrefab() { return nlohmann::json(); }
	};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::ComponentMode::ComponentModeBase);