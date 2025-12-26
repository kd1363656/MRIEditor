#pragma once

namespace MRI::Editor
{
	class EditorViewBase
	{
	public:

		EditorViewBase         () = default;
		virtual ~EditorViewBase() = default;

		virtual std::uint32_t GetTypeID() const = 0;

		// "EditorManager"内で関数を通してリストに生成していいかどうか
		virtual bool IsAllowCreateInList() const { return true; }

		virtual void Init      () { /*必要に応じてオーバーライドしてください*/ };
		virtual void DrawEditor() = 0;

		virtual void LoadFile() { /* 必要に応じてオーバーライドしてください */ };
		virtual void SaveFile() { /* 必要に応じてオーバーライドしてください */ };
	};
}