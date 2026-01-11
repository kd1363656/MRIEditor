#pragma once

namespace MRI
{
	class GameObject;
}

namespace MRI::Component
{
	class ComponentBase
	{
	public:

		ComponentBase         () = default;
		virtual ~ComponentBase() = default;

		// "CRTP"を使おうと考えたがコンパイル時間がコンポーネント数に
		// 比例して長くなるため純粋仮想関数で実装
		virtual const MRI::TypeInfo& GetTypeInfo() const;

		// 複数所持していいコンポーネントかどうか、基本的には禁止
		virtual bool IsAllowMultiple() const { return false; }

		virtual void Init() { /*必要に応じてオーバーライドしてください*/ };

		// "Json"ファイルのデシリアライズ後に呼ばれる、主にコンポーネント間の
		// 参照を結びつけるといった処理を行う
		virtual void PostLoadInit() { /*必要に応じてオーバーライドしてください*/ };

		virtual void PreUpdate  () { /*必要に応じてオーバーライドしてください*/ };
		virtual void EarlyUpdate() { /*必要に応じてオーバーライドしてください*/ };
		virtual void Update     () { /*必要に応じてオーバーライドしてください*/ };
		virtual void LateUpdate () { /*必要に応じてオーバーライドしてください*/ };
	
		virtual void EditSpawnInspector () { /*必要に応じてオーバーライドしてください*/ };
		virtual void EditPrefabInspector() { /*必要に応じてオーバーライドしてください*/ };
	
		virtual void DeserializeSpawn (const nlohmann::json& a_json) { /*必要に応じてオーバーライドしてください*/ };
		virtual void DeserializePrefab(const nlohmann::json& a_json) { /*必要に応じてオーバーライドしてください*/ };
	
		virtual nlohmann::json SerializeSpawn () { return nlohmann::json(); }
		virtual nlohmann::json SerializePrefab() { return nlohmann::json(); }
		
		void Enable ();
		void Disable();

		bool GetIsEnabled() const { return m_isEnabled; }

		void SetOwnerCache(const std::weak_ptr<MRI::GameObject>& a_set) { m_ownerCache = a_set; }

	private:

		std::weak_ptr<MRI::GameObject> m_ownerCache;

		bool m_isEnabled = true;

	protected:

		std::weak_ptr<MRI::GameObject>& GetWorkOwnerCache() { return m_ownerCache; }
	};
}

MRI_REGISTER_TYPE_INFO_ROOT(MRI::Component::ComponentBase);