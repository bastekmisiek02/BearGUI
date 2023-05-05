#include "Vertex.h"

namespace Bear
{
	namespace GUI
	{
		static UInt nextID = 0;

		Vertex::Vertex()
			: position(), color(), ID(nextID)
		{
		}

		Vertex::Vertex(const Vec2& position, const Vec4& color)
			: position(position), color(color), ID(nextID)
		{
		}

		void Vertex::NextID()
		{
			nextID++;
		}
	}
}