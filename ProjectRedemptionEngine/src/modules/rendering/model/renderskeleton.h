#pragma once
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		class Renderer;
		class RenderModel;

		using std::pair;
		using std::string;
		using std::unordered_map;
		using std::vector;

		class RenderSkeleton
		{
			RenderSkeleton& operator=(const RenderSkeleton&) = delete;
			RenderSkeleton(const RenderSkeleton&) = delete;

			friend class Renderer;
			friend class RenderModel;

		public:
			class BoneConfig;

		private:
			struct Bone;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderSkeleton;

			private:
				static Impl& MakeImpl(
					const BoneConfig& rootBoneConfig
				);

				static Bone& MakeImplRecurse(
					const BoneConfig& config,
					unordered_map<string, Bone*>& bones
				);
				static void DestroyImplRecurse(const Bone& bone);

				const Bone& rootBone;
				const unordered_map<string, Bone*> bones;

				Impl(
					const Bone& rootBone,
					unordered_map<string, Bone*>& bones
				);
				~Impl();

				void GetCurrentStateRecurse(
					const Bone& currentBone,
					const glm::mat4& parentTransform,
					vector<glm::mat4>& result
				);
			};

			struct Bone
			{
				Bone& operator=(const Bone&) = delete;
				Bone(const Bone&) = delete;

				const int id;
				const vector<Bone*> children;
				const glm::mat4 bindPos;
				glm::mat4 localMatrix;

				Bone(
					int id,
					const vector<Bone*>& children,
					const glm::mat4& bindPos
				);
			};

		public:
			class BoneConfig
			{
				friend class Impl;

				const unsigned int _id;
				const string _name;
				const glm::mat4 _bindPos;
				vector<const BoneConfig*> _children;

			public:
				BoneConfig(unsigned int id, const string& name, const glm::mat4& bindPos);

				void AddChild(const BoneConfig& child);
			};

			void SetBoneLocalMatrix(const string bone, const glm::mat4& localMatrix);

		private:
			Impl& _impl;

			RenderSkeleton(const BoneConfig& rootBoneConfig);
			~RenderSkeleton();

			void GetCurrentState(vector<glm::mat4>& result);
		};
	} // namespace RenderingModule
} // namespace PRE