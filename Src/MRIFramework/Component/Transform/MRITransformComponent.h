#pragma once

namespace MRI::Strategy
{
	class MatrixStrategyBase;
}

namespace MRI::Component
{
	class TransformComponent final : public MRI::Component::ComponentBase									, 
									 public std::enable_shared_from_this<MRI::Component::TransformComponent>
	{
	public:

		struct Transform final
		{
			Math::Vector3    scale    = Math::Vector3::Zero;
			Math::Quaternion rotation = Math::Quaternion::Identity;
			Math::Vector3    pos      = Math::Vector3::Zero;
			Math::Matrix     matrix   = Math::Matrix::Identity;
		};

	public:
	
		TransformComponent ();
		~TransformComponent() override;
	
		const MRI::TypeInfo& GetTypeInfo() const override;

		void Init        () override;
		void PostLoadInit() override;

		void EditSpawnInspector () override;
		void EditPrefabInspector() override;
	
		void DeserializeSpawn (const nlohmann::json& a_json) override;
		void DeserializePrefab(const nlohmann::json& a_json) override;
	
		nlohmann::json SerializeSpawn () override;
		nlohmann::json SerializePrefab() override;
		
		void FixMatrix();
		
		Math::Matrix CalcScaleMatrix      () const { return Math::Matrix::CreateScale         (m_transform.scale);    }
		Math::Matrix CalcRotationMatrix   () const { return Math::Matrix::CreateFromQuaternion(m_transform.rotation); }
		Math::Matrix CalcTranslationMatrix() const { return Math::Matrix::CreateTranslation   (m_transform.pos);	  }
	
		const auto& GetTransform() const { return m_transform; }

		const Math::Matrix& GetMatrix() const { return m_transform.matrix; }

		const Math::Quaternion& GetRotation() const { return m_transform.rotation; }
		const Math::Vector3&    GetPos     () const { return m_transform.pos;      }

		void SetParentTransformComponentCache(const std::weak_ptr<MRI::Component::TransformComponent> a_set) { m_parentTransformComponentCache = a_set; }
	
		void SetMatrix(const Math::Matrix& a_set) { m_transform.matrix = a_set; }
	
		void SetPos(const Math::Vector3& a_set) { m_transform.pos = a_set; }

		void SetRotation(const Math::Quaternion& a_set) { m_transform.rotation = a_set; }

	private:
	
		bool EditCommonInspector();
		
		void		   DeserializeCommon(const nlohmann::json& a_json);
		nlohmann::json SerializeCommon  ()							   const;

		void FixMatrixStrategy();
	
		std::weak_ptr<MRI::Component::TransformComponent> m_parentTransformComponentCache;
	
		std::unique_ptr<MRI::Strategy::MatrixStrategyBase> m_fixMatrixStrategy = nullptr;
	
		MRI::Component::TransformComponent::Transform m_transform = {};
	};
}

MRI_REGISTER_TYPE_INFO(MRI::Component::TransformComponent , MRI::Component::ComponentBase);