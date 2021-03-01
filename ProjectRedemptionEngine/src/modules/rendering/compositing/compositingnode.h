#pragma once
#include <string>
#include <unordered_set>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class Camera;
		class CompositingTarget;
		class RenderTexture;
		class Renderer;

		using std::string;
		using std::unordered_set;

		class CompositingNode
		{
			CompositingNode& operator=(const CompositingNode&) = delete;
			CompositingNode(const CompositingNode&) = delete;

			friend class Renderer;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class CompositingNode;

			private:
				CompositingTarget& target;
				unordered_set<CompositingNode*> compositingNodeDependencies;

				static Impl& MakeImpl(unsigned int width, unsigned int height);

				Impl(CompositingTarget& target);
				~Impl();
			};

		public:
			enum class CompositingAttachment { POSITION, NORMALS, ALBEDO_SPECULAR };

		private:
			const Camera& _camera;
			const unsigned int _renderTag;
			Impl& _impl;

			CompositingNode(const Camera& camera, unsigned int renderTag, unsigned int width, unsigned int height);
			~CompositingNode();

			void BindTarget();

			void AddDependency(CompositingNode& compositingNode);
			void RemoveDependency(CompositingNode& compositingNode);

			RenderTexture& GetAttachment(CompositingAttachment attachment);

			const unordered_set<CompositingNode*>& GetDependencies();
		};
	} // namespace RenderingModule
} // namespace PRE