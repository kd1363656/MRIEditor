#pragma once

namespace MRI::Editor
{
	template <typename Type>
	concept IsDerivedEditorViewBaseConcept = MRI::Concept::IsDerivedBaseConcept<Type , MRI::Editor::EditorViewBase>;
}

namespace MRI::Editor
{
	class EditorManager final : public MRI::SingletonBase<MRI::Editor::EditorManager>
	{
	public:

		void Init      ();
		void DrawEditor();

		void LoadFile() const;
		void SaveFile() const;

		void AddLog(const char* a_format , ...);

		// マップ内を捜査し該当するエディタービューが存在すれば取得
		template <MRI::Editor::IsDerivedEditorViewBaseConcept Type>
		std::weak_ptr<Type> FetchEditorViewCache()
		{
			const std::uint32_t l_typeID = MRI::StaticID::GetTypeID<Type>();

			auto l_itr = m_editorViewCacheMap.find(l_typeID);

			if (l_itr == m_editorViewCacheMap.end())
			{
				return std::weak_ptr<Type>();
			}

			auto l_view = l_itr->second.lock();
			if (!l_view) 
			{
				return std::weak_ptr<Type>();
			}

			return std::static_pointer_cast<Type>(l_view);
		}

	private:

		void InitEditor   ();
		void ReleaseEditor() const;

		void DrawDockSpace   () const;
		void DrawEditorWindow();
		void PostDrawEditor  ();

		template <MRI::Editor::IsDerivedEditorViewBaseConcept Type>
		void CreateEditorView()
		{
			// もし一つでも同じ"Editor"が存在すれば"return"("Editor"は一つしか存在しなくてよいものだから)
			if (!FetchEditorViewCache<Type>().expired()) { return; }

			auto l_view = std::make_shared<Type>();

			// もしリスト内に生成してもいいクラスでない場合"return"
			if (!l_view->IsAllowCreateInList()) { return; }

			const std::uint32_t l_typeID = l_view->GetTypeID();

			m_editorViewList.emplace_back   (l_view);
			m_editorViewCacheMap.try_emplace(l_typeID , l_view);
		}

		static constexpr size_t k_logBufferSize = 1024ULL;

		static constexpr float k_fontSize = 13.0F;

		static const int k_numView = 1;

		std::vector<std::shared_ptr<MRI::Editor::EditorViewBase>>					   m_editorViewList;
		std::unordered_map<std::uint32_t , std::weak_ptr<MRI::Editor::EditorViewBase>> m_editorViewCacheMap;

		std::unique_ptr<MRI::Editor::EditorMenuBar> m_editorMenuBar = nullptr;
		std::unique_ptr<MRI::Editor::EditorLogView> m_editorLogView = nullptr;

		//===============================
		// シングルトン
		//===============================
		friend class MRI::SingletonBase<MRI::Editor::EditorManager>;

		EditorManager ();
		~EditorManager() override;
	};
}

#define MRI_ADD_LOG(Format , ...) \
MRI::Editor::EditorManager::GetInstance().AddLog(U8("[%s : %d][%s]\n" Format "\n") , __FILE__ , __LINE__ , __func__ , ##__VA_ARGS__);