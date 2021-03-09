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

		private:
			class BoneConfig;
			struct Bone;

			class Impl
			{
				Impl& operator=(const Impl&) = delete;
				Impl(const Impl&) = delete;

				friend class RenderSkeleton;

			private:
				static Impl& MakeImpl(
					const string& rootBone,
					const vector<BoneConfig>& boneConfigs
				);

				const string rootBone;
				const unordered_map<string, Bone*> bones;
				const vector<glm::ivec4> vertexBoneInfluenceIndices;
				const vector<glm::vec4> vertexBoneInfluenceWeights;

				Impl(
					const string& rootBone,
					unordered_map<string, Bone*>& bones,
					vector<glm::ivec4>& vertexBoneInfluenceIndices,
					vector<glm::vec4>& vertexBoneInfluenceWeights
				);
				~Impl();

				void GetCurrentStateRecurse(
					const string currentBoneName,
					const glm::mat4& parentTransform,
					vector<glm::mat4>& result
				);
			};

			struct Bone
			{
				Bone& operator=(const Bone&) = delete;
				Bone(const Bone&) = delete;

				const int id;
				const vector<string> children;
				const glm::mat4 bindPos;
				glm::mat4 localMatrix;

				Bone(
					int id,
					const vector<string>& children,
					const glm::mat4& bindPos
				);
			};

		public:
			class BoneConfig
			{
				friend class Impl;

				const string _name;
				const glm::mat4 _bindPos;
				vector<string> _children;
				vector<pair<int, float>> _vertexInfluences;

			public:
				BoneConfig(const string& name, const glm::mat4& bindPos);
				void AddChild(const string& child);
				void AddVertexInfluence(int vertexId, float weight);
			};

			void SetBoneLocalMatrix(const string bone, const glm::mat4& localMatrix);

			const vector<glm::ivec4>& GetVertexBoneInfluenceIndices();
			const vector<glm::vec4>& GetVertexBoneInfluenceWeights();

		private:
			Impl& _impl;

			RenderSkeleton(const string& rootBone, const vector<BoneConfig>& bones);
			~RenderSkeleton();

			void GetCurrentState(vector<glm::mat4>& result);
		};
	} // namespace RenderingModule
} // namespace PRE