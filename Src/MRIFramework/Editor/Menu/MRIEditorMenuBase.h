#pragma once

namespace MRI::Editor
{
	class EditorMenuBase
	{
	public:

		EditorMenuBase         () = default;
		virtual ~EditorMenuBase() = default;

		virtual std::uint32_t GetTypeID() const = 0;

		virtual void Init      () { /*必要に応じてオーバーライドしてください*/ };
		virtual void DrawEditor() = 0;
	};
}