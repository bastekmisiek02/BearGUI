#include "Label.h"

namespace Bear
{
	namespace GUI
	{
		Label::Label()
			: Base()
		{
		}

		Label::Label(Base* parent, const IVec2& position, const Vec4& color, const String& text, const char& anchor, const char& textAlign, const String& name)
			: Base(parent, position, color, text, anchor, textAlign, name)
		{
			vertices.AddCollection(Collections::MakeDynamicArray<Vertex>
				(
					Vertex
					{
						{-0.5f, 0.5f},
						{0.5f, 0.5f, 0.5f, 1.0f}
					},

					Vertex
					{
						{0.5f, 0.5f},
						{0.5f, 0.5f, 0.5f, 1.0f}
					},

					Vertex
					{
						{0.5f, -0.5f},
						{0.5f, 0.5f, 0.5f, 1.0f}
					},

					Vertex
					{
						{-0.5f, -0.5f},
						{0.5f, 0.5f, 0.5f, 1.0f}
					}
			));

			indices.AddCollection(Collections::MakeDynamicArray<UInt>
				(
					0U, 1U, 2U,
					2U, 3U, 0U
				));
		}
	}
}