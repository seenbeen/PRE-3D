#pragma once
#include <unordered_set>

#include <glad/glad.h>

namespace PRE
{
	namespace RenderingModule
	{
		class CompositingTarget;
		class RenderTexture;
		class Renderer;

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
				unordered_set<CompositingNode*> compositingNodeDependencies;

				static Impl& MakeImpl();

				Impl();
				~Impl();
			};

		public:
			enum class CompositingAttachment { POSITION, NORMALS, ALBEDO_SPECULAR };

		private:
			const unsigned int _renderTag;
			CompositingTarget& _compositingTarget;

			Impl& _impl;

			CompositingNode(unsigned int renderTag, CompositingTarget& compositingTarget);
			~CompositingNode();

			void AddDependency(CompositingNode& compositingNode);
			void RemoveDependency(CompositingNode& compositingNode);

			const unordered_set<CompositingNode*>& GetDependencies();
		};
	} // namespace RenderingModule
} // namespace PRE