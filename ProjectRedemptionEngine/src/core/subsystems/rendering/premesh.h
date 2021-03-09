#pragma once
#include <glm/glm.hpp>

#include <include/modules/rendering.h>

namespace PRE
{
	namespace Core
	{
		class PRERendering;

		using PRE::RenderingModule::RenderMesh;

		class PREMesh
		{
			PREMesh& operator=(const PREMesh&) = delete;
			PREMesh(const PREMesh&) = delete;

			friend class PRERendering;

		public:
			void SetRenderedTriangles(
				unsigned int startTriangleIndex,
				unsigned int endTriangleIndex
			);

		private:
			RenderMesh& _mesh;

			PREMesh(RenderMesh& mesh);
			~PREMesh();
		};
	} // namespace Core
} // namespace PRE