#include <modules/rendering/model/renderskeleton.h>

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <glm/glm.hpp>

namespace PRE
{
	namespace RenderingModule
	{
		using std::pair;
		using std::string;
		using std::unordered_map;
		using std::vector;

		RenderSkeleton::Impl& RenderSkeleton::Impl::MakeImpl(
			const string& rootBone,
			const vector<BoneConfig>& boneConfigs
		)
		{
			unordered_map<string, Bone*> bones;
			vector<glm::ivec4> vertexBoneInfluenceIndices;
			vector<glm::vec4> vertexBoneInfluenceWeights;

			// magic happens here.

			return *(new Impl(
				rootBone,
				bones,
				vertexBoneInfluenceIndices,
				vertexBoneInfluenceWeights
			));
		}

		RenderSkeleton::Impl::Impl(
			const string& rootBone,
			unordered_map<string, Bone*>& bones,
			vector<glm::ivec4>& vertexBoneInfluenceIndices,
			vector<glm::vec4>& vertexBoneInfluenceWeights
		)
			:
			rootBone(rootBone),
			bones(std::move(bones)),
			vertexBoneInfluenceIndices(std::move(vertexBoneInfluenceIndices)),
			vertexBoneInfluenceWeights(std::move(vertexBoneInfluenceWeights)) {}

		RenderSkeleton::Impl::~Impl()
		{
			for (auto it = bones.begin(); it != bones.end(); ++it)
			{
				delete it->second;
			}
		}

		void RenderSkeleton::Impl::GetCurrentStateRecurse(
			const string currentBoneName,
			const glm::mat4& parentTransform,
			vector<glm::mat4>& result
		)
		{
			auto pCurrentBone = bones.find(currentBoneName)->second;
			auto currentTransform = parentTransform * pCurrentBone->localMatrix;
			result[pCurrentBone->id] = currentTransform * pCurrentBone->bindPos;
			for (
				auto it = pCurrentBone->children.begin();
				it != pCurrentBone->children.end();
				++it
			)
			{
				GetCurrentStateRecurse(*it, currentTransform, result);
			}
		}

		RenderSkeleton::Bone::Bone(
			int id,
			const vector<string>& children,
			const glm::mat4& bindPos
		)
			:
			id(id),
			children(children),
			bindPos(bindPos),
			localMatrix() {}

		RenderSkeleton::BoneConfig::BoneConfig(
			const string& name,
			const glm::mat4& bindPos
		)
			:
			_name(name),
			_bindPos(bindPos) {}

		void RenderSkeleton::BoneConfig::AddChild(const string& child)
		{
			_children.push_back(child);
		}

		void RenderSkeleton::BoneConfig::AddVertexInfluence(
			int vertexId,
			float weight
		)
		{
			_vertexInfluences.push_back(std::make_pair(vertexId, weight));
		}

		void RenderSkeleton::SetBoneLocalMatrix(
			const string bone,
			const glm::mat4& localMatrix
		)
		{
			auto it = _impl.bones.find(bone);
			if (it != _impl.bones.end())
			{
				it->second->localMatrix = localMatrix;
			}
		}

		const vector<glm::ivec4>& RenderSkeleton::GetVertexBoneInfluenceIndices()
		{
			return _impl.vertexBoneInfluenceIndices;
		}

		const vector<glm::vec4>& RenderSkeleton::GetVertexBoneInfluenceWeights()
		{
			return _impl.vertexBoneInfluenceWeights;
		}

		RenderSkeleton::RenderSkeleton(
			const string& rootBone,
			const vector<BoneConfig>& bones
		)
			:
			_impl(Impl::MakeImpl(rootBone, bones)) {}

		RenderSkeleton::~RenderSkeleton()
		{
			delete &_impl;
		}

		void RenderSkeleton::GetCurrentState(vector<glm::mat4>& result)
		{
			result.clear();
			result.reserve(_impl.bones.size());
			_impl.GetCurrentStateRecurse(
				_impl.rootBone,
				glm::mat4(),
				result
			);
		}
	} // namespace RenderingModule
} // namespace PRE