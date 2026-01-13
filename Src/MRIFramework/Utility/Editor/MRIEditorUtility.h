#pragma once

namespace MRI::EditorUtility
{
	//===================================================================
	// セレクター
	//===================================================================
	template <MRI::Concept::IsDerivedTagBaseConcept Type>
	inline void TagRadioButtonSelector(const char* a_label , std::uint32_t& a_wantChangeTag , const float a_width = MRI::EditorCommonConstant::k_comboWidth)
	{
		ImGui::SetNextItemWidth(a_width);
		ImGui::BeginGroup      ();

		auto& l_typeInfoRegistry = MRI::TypeInfoRegistry::GetInstance();

		// 現在選択されているタグ名、もし存在しなければ"Unknown"を表示
		std::string l_tagName = MRI::CommonConstant::k_stringUnknown.data();
		if (auto l_typeInfo = l_typeInfoRegistry.FetchTypeInfoByID(a_wantChangeTag))
		{
			l_tagName = l_typeInfo->k_name.data();
		}

		// もしコンボリストが開かれていなければ早期リターン
		if (!ImGui::BeginCombo(a_label , l_tagName.c_str()))
		{
			ImGui::EndGroup();
			return;
		}

		// 基底クラス、基底構造体ををベースに対応するタグを選び選ばれたタグを引数に格納
		auto l_derivedTypeInfoList = l_typeInfoRegistry.FetchTypeInfoDerivedFromBaseList<Type>();
		for (const auto& l_typeInfo : l_derivedTypeInfoList)
		{
			if (!l_typeInfo)									   { continue; }
			if (l_typeInfo->k_id == MRI::GetTypeInfo<Type>().k_id) { continue; }	// 同じ型"ID"なら"returnn"(基底構造体のタグはいらないから)

			// ポインタを"PushID"に格納
			ImGui::PushID(l_typeInfo);
			bool l_isSelected = a_wantChangeTag == l_typeInfo->k_id;

			if (!ImGui::RadioButton(l_typeInfo->k_name.data() , l_isSelected))
			{
				ImGui::PopID();
				continue;
			}

			// 選択された項目にカーソルを当てる
			if (l_isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
			
			a_wantChangeTag = l_typeInfo->k_id;

			ImGui::PopID();
		}

		ImGui::EndCombo();
		ImGui::EndGroup();
	};
	
	// ファクトリーから生成するクラスを選べるセレクター
	template <typename FactoryType , typename Type>
	inline bool FactoryRadioButtonSelector(const char* a_label , Type& a_wantChange , const float a_width = MRI::EditorCommonConstant::k_comboWidth)
	{
		bool l_isCreate = false;

		if constexpr (!MRI::Concept::IsSmartPTRConcept<Type>) 
		{
			return false; 
		}
		
		ImGui::SetNextItemWidth(a_width);
		ImGui::BeginGroup      ();

		std::string l_nowStrategyName = MRI::CommonConstant::k_stringUnknown.data();

		// もしストラテジーが既にインスタンス化されているなら文字列を取得
		if (a_wantChange)
		{
			l_nowStrategyName = a_wantChange->GetTypeInfo().k_name;
		}

		if (!ImGui::BeginCombo(a_label , l_nowStrategyName.c_str()))
		{
			ImGui::EndGroup();
			return false;
		}

		auto& l_factory = FactoryType::GetInstance();

		for (const auto& [l_key , l_value] : l_factory.GetFactoryMap())
		{
			bool l_isSelected = false;
			l_isSelected      = l_nowStrategyName == l_key;

			ImGui::PushID(&l_value);

			if (!ImGui::RadioButton(l_key.c_str() , l_isSelected))
			{
				ImGui::PopID();
				continue;
			}

			// 選択された項目にカーソルを当てる
			if (l_isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}

			a_wantChange = l_value();
			l_isCreate   = true;

			ImGui::PopID();
		}

		ImGui::EndCombo();
		ImGui::EndGroup();

		return l_isCreate;
	}

	//==================================================================
	// ドラッグアンドドロップ
	//==================================================================
	template <typename Type>
		requires MRI::Concept::IsSmartPTRConcept<Type>
	inline bool DragDropSource(const char*  a_label , const Type& a_payload)
	{
		// ポインタの種類情報を取得
		constexpr MRI::TypeTrait::PTRKind l_kind = MRI::TypeTrait::PTRType<Type>::k_kind;

		bool l_isDrag = false;
		ImGui::PushID(&a_payload);

		if (!ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			ImGui::PopID();
			return false;
		}

		l_isDrag = true;

		// 値渡し
		if constexpr (l_kind == MRI::TypeTrait::PTRKind::None)
		{
			ImGui::SetDragDropPayload(a_label , &a_payload , sizeof(Type));
		}
		// 生ポインタ、シェアードポインタ
		else if constexpr (l_kind == MRI::TypeTrait::PTRKind::Raw || l_kind == MRI::TypeTrait::PTRKind::Shared)
		{
			if (a_payload)
			{
				ImGui::SetDragDropPayload(a_label , &a_payload , sizeof(a_payload));
			}
		}

		ImGui::Text             ("%s" , a_label);
		ImGui::EndDragDropSource();
		ImGui::PopID            ();

		return l_isDrag;
	}
	template <typename Type>
		requires MRI::Concept::IsSmartPTRConcept<Type>
	inline bool DragDropTarget(const char* a_label , Type& a_outPayload)
	{
		// ポインタの種類情報を取得
		constexpr MRI::TypeTrait::PTRKind l_kind = MRI::TypeTrait::PTRType<Type>::k_kind;
		
		bool l_isDropped = false;
		ImGui::PushID(&a_outPayload);

		if (!ImGui::BeginDragDropTarget())
		{
			ImGui::PopID();
			return false;
		}

		const ImGuiPayload* l_payload = ImGui::AcceptDragDropPayload(a_label);
		if (!l_payload)
		{
			ImGui::PopID			();
			ImGui::EndDragDropTarget();
			return false;
		}

		if constexpr (l_kind == MRI::TypeTrait::PTRKind::None)
		{
			if (l_payload->DataSize == sizeof(Type))
			{
				a_outPayload = *static_cast<const Type*>(l_payload->Data);
			}
		}
		else if constexpr (l_kind == MRI::TypeTrait::PTRKind::Raw || l_kind == MRI::TypeTrait::PTRKind::Shared)
		{
			const auto* l_ptr = static_cast<const Type*>(l_payload->Data);
			if (l_ptr)
			{
				a_outPayload = *l_ptr;
				l_isDropped  = true;
			}
		}

		ImGui::PopID			();
		ImGui::EndDragDropTarget();

		return l_isDropped;
	}

	//===================================================================
	// テキストボックス
	//===================================================================
	inline void InputTextWithCustomWidth(const char* a_label , std::string& a_inputText , const float a_width = MRI::EditorCommonConstant::k_comboWidth)
	{
		ImGui::SetNextItemWidth(a_width);
		ImGui::InputText       (a_label , &a_inputText);
	}

	//===================================================================
	// ボタン
	//===================================================================
	inline bool SameLineButton(const char* a_label)
	{
		// "ImGui"の"Begin"呼び出し直後でないかをチェック
		if (!ImGui::IsWindowAppearing())
		{
			ImGui::SameLine();
		}
		
		if (ImGui::Button(a_label))
		{
			return true;
		}

		return false;
	}

	inline bool SameLineSmallButton(const char* a_label)
	{
		// "ImGui"の"Begin"呼び出し直後でないかをチェック
		if (!ImGui::IsWindowAppearing())
		{
			ImGui::SameLine();
		}
		
		if (ImGui::SmallButton(a_label))
		{
			return true;
		}

		return false;
	}

	//===================================================================
	// 視覚補助
	//===================================================================
	inline void DrawRectArea(const ImVec2& a_rectMin , const ImVec2& a_rectMax , const ImColor& a_color = MRI::EditorCommonConstant::k_whiteColor)
	{
		ImDrawList* l_drawList = ImGui::GetWindowDrawList();
		l_drawList->AddRectFilled(a_rectMin , a_rectMax , a_color);
	}

	//===================================================================
	// マウスと"ImGui"の"Window"の矩形当たり判定
	//===================================================================}
	inline bool IsMouseInWindowRect()
	{
		// ウィンドウ描画範囲の取得
		const auto* l_window = ImGui::GetCurrentWindow();
		if (!l_window)
		{ 
			return false;
		}

		// 株一定範囲を解除ゾーンとする
		const ImVec2 l_rectMin  = ImGui::GetCursorScreenPos();
		const ImVec2 l_rectMax  = l_window->ContentRegionRect.Max;
		const ImVec2 l_mousePos = ImGui::GetMousePos();
 
		return l_mousePos.x >= l_rectMin.x && 
			   l_mousePos.x <= l_rectMax.x &&
			   l_mousePos.y >= l_rectMin.y && 
			   l_mousePos.y <= l_rectMax.y;
	}
	
	//===================================================================
	// 文字列変換
	//===================================================================
	inline const char* BoolToString(const bool a_flag)
	{
		return a_flag ? "True" : "False";
	};

	inline const char* BoolToString(const std::uint32_t a_flag)
	{
		if (a_flag == MRI::StaticID::GetTypeID<MRI::Tag::BoolTagTrue>())
		{ 
			return "True";
		}
		else if (a_flag == MRI::StaticID::GetTypeID<MRI::Tag::BoolTagFalse>()) 
		{
			return "False"; 
		}

		return "Neither";
	};

	//===================================================================
	// ファイルダイアログ
	//===================================================================
	inline bool OpenFileDialog(std::string& a_filePath , const std::string_view& a_title = MRI::EditorCommonConstant::k_defaultTitleOpen , const char* const a_filters = MRI::EditorCommonConstant::k_defaultFilters)
	{
		// 現在のカレントディレクトリ保存
		auto l_current = std::filesystem::current_path();

		// ファイル名のみ
		auto l_fileName = std::filesystem::path(a_filePath).filename();

		// 結果用
		static char l_fName[MRI::EditorCommonConstant::k_maxFilePathSize];
		strcpy_s(l_fName , sizeof(l_fName) , l_fileName.string().c_str());

		// 初期表示するディレクトリをフォルダに指定
		std::string l_dir = std::filesystem::path(MRI::EditorCommonConstant::k_defaultAssetDirectory).string() + "\\";
		
		OPENFILENAMEA l_o;
		ZeroMemory(&l_o , sizeof(l_o));

		l_o.lStructSize     = sizeof(l_o);								        // 構造体のサイズ指定(必須)
		l_o.hwndOwner       = nullptr;									        // ダイアログのオーなウィンドウ
		l_o.lpstrInitialDir = l_dir.c_str();							        // 初期表示されるディレクトリパス
		l_o.lpstrFile       = l_fName;								            // ファイルパスを格納するバッファ
		l_o.nMaxFile        = sizeof(l_fName);							        // "lpstrFile"のサイズ
		l_o.lpstrFilter     = a_filters;								        // ファイル選択時のフィルター
		l_o.lpstrDefExt     = "";										        // 拡張子を自動で補完するためのデフォルト拡張子
		l_o.lpstrTitle      = a_title.data();							        // ダイアログのタイトル文字列
		l_o.nFilterIndex    = MRI::EditorCommonConstant::k_defaultFilterIndex;	// フィルターの何番目を初期選択するか

		// ユーザーがファイルを選んだら実行
		if (GetOpenFileNameA(&l_o))
		{
			// カレントディレクトリを元に戻す
			std::filesystem::current_path(l_current);

			// 相対パスへ変換
			a_filePath = std::filesystem::relative(l_fName).string();
			return true;
		}

		// カレントディレクトリをもとに戻す
		std::filesystem::current_path(l_current);

		return false;
	}

	inline bool SaveFileDialog(std::string&            a_filePath						                         ,  
							   const std::string_view& a_title   = MRI::EditorCommonConstant::k_defaultTitleSave ,
							   const char* const       a_filters = MRI::EditorCommonConstant::k_defaultFilters   ,
							   const char* const       a_defExt  = "")
	{
		// 現在のカレントディレクトリを保存
		auto l_current = std::filesystem::current_path();
		// ファイル名のみ
		auto l_fileName = std::filesystem::path(a_filePath).filename();

		// 結果用
		static char l_fName[MRI::EditorCommonConstant::k_maxFilePathSize];
		strcpy_s(l_fName , sizeof(l_fName) , l_fileName.string().c_str());

		// デフォルトフォルダ
		std::string l_dir = std::filesystem::path(MRI::EditorCommonConstant::k_defaultAssetDirectory).string() + "\\";

		OPENFILENAMEA l_o;
		ZeroMemory(&l_o , sizeof(l_o));

		l_o.lStructSize     = sizeof(l_o);										// 構造体サイズ(必須)
		l_o.hwndOwner       = nullptr;											// ダイアログの親ウィンドウ
		l_o.lpstrInitialDir = l_dir.c_str();									// 初期表示されるディレクトリ
		l_o.lpstrFile       = l_fName;											// ファイル名バッファ
		l_o.nMaxFile        = sizeof(l_fName);									// バッファサイズ
		l_o.lpstrFilter     = a_filters;										// ファイル形式フィルタ
		l_o.lpstrDefExt     = a_defExt;											// 拡張子の自動付加
		l_o.lpstrTitle      = a_title.data ();									// ダイアログのタイトル名
		l_o.nFilterIndex    = MRI::EditorCommonConstant::k_defaultFilterIndex;	// 最初に選択するフィルタ
		l_o.Flags           = OFN_FILEMUSTEXIST;								// 複数のオプションフラグ(保存先として既存ファイルしか選ばせない、上書きを警告する)

		if (GetSaveFileNameA(&l_o))
		{
			// カレントディレクトを元に戻す
			std::filesystem::current_path(l_current);
			// 相対パスへ変換
			a_filePath = std::filesystem::relative(l_fName).string();

			bool l_isSuccess = false;

			// 選んだファイルパスにファイルが存在しているか、存在していなければ
			// 自動生成するが自動生成がうまくいっているかどうかでうまくいっていたら"return"
			if (std::filesystem::exists(a_filePath))
			{
				l_isSuccess = true;
			}
			else
			{
				l_isSuccess = MRI::FileIOUtility::CreateEmptyFile(a_filePath);
			}

			return l_isSuccess;
		}

		std::filesystem::current_path(l_current); // カレントディレクトリを元に戻す
		return false;
	}

	bool IsPrefabGameObjectInSceneGameObjectList   (const std::string& a_prefabName);
	bool HasSameUUIDGameObjectInSceneGameObjectList(const UUID&        a_uuid);
	void DeletePrefabIfAllInstanceRemoved          (const std::string& a_prefabName);
	void DeleteGameObjectIfPrefabRemoved           (const std::string& a_prefabName);
	void CreateGameObjectAndRegisterScenePrefab    (const std::string& a_prefabName , const std::string& a_prefabFilePath);
}