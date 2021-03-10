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
			const BoneConfig& rootBoneConfig
		)
		{
			unordered_map<string, Bone*> bones;
			auto& rootBone = MakeImplRecurse(rootBoneConfig, bones);
			return *(new Impl(rootBone, bones));
		}

		RenderSkeleton::Bone& RenderSkeleton::Impl::MakeImplRecurse(
			const BoneConfig& config,
			unordered_map<string, Bone*> bones
		)
		{
			vector<Bone*> children;
			for (auto it = config._children.begin(); it != config._children.end(); ++it)
			{
				children.push_back(&MakeImplRecurse(**it, bones));
			}
			
			auto pBone = new Bone(config._id, children, config._bindPos);

			bones[config._name] = pBone;

			return *pBone;
		}

		void RenderSkeleton::Impl::DestroyImplRecurse(const Bone& bone)
		{
			for (auto it = bone.children.begin(); it != bone.children.end(); ++it)
			{
				DestroyImplRecurse(**it);
			}
			delete &bone;
		}

		RenderSkeleton::Impl::Impl(
			const Bone& rootBone,
			unordered_map<string, Bone*>& bones
		)
			:
			rootBone(rootBone),
			bones(std::move(bones)) {}

		RenderSkeleton::Impl::~Impl()
		{
			DestroyImplRecurse(rootBone);
		}

		void RenderSkeleton::Impl::GetCurrentStateRecurse(
			const Bone& currentBone,
			const glm::mat4& parentTransform,
			vector<glm::mat4>& result
		)
		{
			auto currentTransform = parentTransform * currentBone.localMatrix;
			result[currentBone.id] = currentTransform * currentBone.bindPos;
			for (
				auto it = currentBone.children.begin();
				it != currentBone.children.end();
				++it
			)
			{
				GetCurrentStateRecurse(**it, currentTransform, result);
			}
		}

		RenderSkeleton::Bone::Bone(
			int id,
			const vector<Bone*>& children,
			const glm::mat4& bindPos
		)
			:
			id(id),
			children(children),
			bindPos(bindPos),
			localMatrix() {}

		RenderSkeleton::BoneConfig::BoneConfig(
			unsigned int it,
			const string& name,
			const glm::mat4& bindPos
		)
			:
			_id(it),
			_name(name),
			_bindPos(bindPos) {}

		void RenderSkeleton::BoneConfig::AddChild(const BoneConfig& child)
		{
			_children.push_back(&child);
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

		RenderSkeleton::RenderSkeleton(const BoneConfig& rootBoneConfig)
			:
			_impl(Impl::MakeImpl(rootBoneConfig)) {}

		RenderSkeleton::~RenderSkeleton()
		{
			delete &_impl;
		}

		void RenderSkeleton::GetCurrentState(vector<glm::mat4>& result)
		{
			result.clear();
			result.reserve(_impl.bones.size());

			for (auto i = 0; i < _impl.bones.size(); ++i)
			{
				result.push_back(glm::mat4());
			}

			_impl.GetCurrentStateRecurse(
				_impl.rootBone,
				glm::mat4(),
				result
			);
		}
	} // namespace RenderingModule
} // namespace PRE