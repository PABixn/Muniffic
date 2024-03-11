#include "egpch.h"
#include "Animator.h"

namespace eg {
		
	Animator::Animator()
		: m_AnimationIndex(0), m_Speed(1.0f), m_Animations(CreateRef<std::vector<Animation>>())
	{
	}

	Animator::Animator(Ref<std::vector<Animation>> animations, float speed)
		:m_AnimationIndex(0), m_Animations(animations), m_Speed(speed)
	{
		m_Transitions.resize(animations->size());
	}

	void Animator::Play()
	{
		(*m_Animations)[m_AnimationIndex].Play();
	}

	void Animator::Pause()
	{
		(*m_Animations)[m_AnimationIndex].Pause();
	}

	void Animator::Stop()
	{
		(*m_Animations)[m_AnimationIndex].Stop();
	}

	void Animator::Update(float dt)
	{
		(*m_Animations)[m_AnimationIndex].Update(dt, m_Speed);
	}

	void Animator::ChangeAnimation(size_t animationIndex)
	{
		if (!CanTransition(m_AnimationIndex, animationIndex))
			return;
		(*m_Animations)[m_AnimationIndex].Stop();
		m_AnimationIndex = animationIndex;
		(*m_Animations)[m_AnimationIndex].Play();
	}

	void Animator::ChangeAnimation(const std::string& animationName)
	{
		int index = GetAnimationIndex(animationName);
		if (index >= 0)
			ChangeAnimation(index);
	
	}

	void Animator::SetAnimations(Ref<std::vector<Animation>> animations)
	{
		m_Animations = animations;
	}

	void Animator::SetAnimation(size_t index, const Animation& animation)
	{
		(*m_Animations)[index] = animation;
	}

	void Animator::AddAnimation(const Animation& animation)
	{
		m_Animations->push_back(animation);
		m_Transitions.push_back(std::vector<size_t>());
	}

	void Animator::ResizeAnimations(size_t size)
	{
		m_Animations->resize(size);
		m_Transitions.resize(size);
	}

	void Animator::AddEmptyAnimation()
	{
		m_Animations->push_back(Animation());
		m_Transitions.push_back(std::vector<size_t>());
	}

	void Animator::AddAnimationWithName(const std::string& name)
	{
		m_Animations->push_back(Animation(name));
	}

	void Animator::RemoveAnimation(size_t index)
	{
		if (index >= m_Animations->size())
			return;
		m_Animations->erase(m_Animations->begin() + index);
	}

	void Animator::RemoveAnimation(const std::string& name)
	{
		size_t animIndex = GetAnimationIndex(name);
		if (animIndex >= 0)
			m_Animations->erase(m_Animations->begin() + animIndex);
	}

	void Animator::RemoveLastAnimation()
	{
		m_Animations->pop_back();
	}

	void Animator::AddTransition(size_t fromIndex, size_t toIndex)
	{
		if (fromIndex < m_Transitions.size() && toIndex != fromIndex && CanTransition(fromIndex, toIndex))
			m_Transitions[fromIndex].push_back(toIndex);
	}

	void Animator::AddTransition(const std::string& fromName, const std::string& toName)
	{
		int fromAnimIndex = GetAnimationIndex(fromName);
		if (fromAnimIndex < 0)
			return;
		int toAnimIndex = GetAnimationIndex(toName);
		if (toAnimIndex < 0)
			return;
		if (CanTransition(fromAnimIndex, toAnimIndex))
			m_Transitions[fromAnimIndex].push_back(toAnimIndex);
	}

	void Animator::RemoveTransition(size_t fromIndex, size_t toIndex)
	{
		if (fromIndex < m_Transitions.size())
		{
			auto it = std::find(m_Transitions[fromIndex].begin(), m_Transitions[fromIndex].end(), toIndex);
			if (it != m_Transitions[fromIndex].end())
				m_Transitions[fromIndex].erase(it);
		}
	}

	void Animator::RemoveTransition(const std::string& fromName, const std::string& toName)
	{
		int fromAnimIndex = GetAnimationIndex(fromName);
		if (fromAnimIndex < 0)
			return;
		int toAnimIndex = GetAnimationIndex(toName);
		if (toAnimIndex < 0)
			return;
		RemoveTransition(fromAnimIndex, toAnimIndex);
	}

	bool Animator::CanTransition(size_t fromIndex, size_t toIndex)
	{
		return std::find(m_Transitions[fromIndex].begin(), m_Transitions[fromIndex].end(), toIndex) != m_Transitions[fromIndex].end();
	}

	bool Animator::CanTransition(const std::string& fromName, const std::string& toName)
	{
		int fromAnimIndex = GetAnimationIndex(fromName);
		if (fromAnimIndex < 0)
			return false;
		int toAnimIndex = GetAnimationIndex(toName);
		if (toAnimIndex < 0)
			return false;
		return CanTransition(fromAnimIndex, toAnimIndex);
	}

	Animation* Animator::GetAnimation(const std::string& name)
	{
		for (Animation anim : *m_Animations)
		{
			if (anim.GetName() == name)
				return (&anim);
		}

		return nullptr;
	}

	int Animator::GetAnimationIndex(const std::string& name)
	{
		int i = 0;
		for (Animation anim : *m_Animations)
		{
			if (anim.GetName() == name)
				return i;
			i++;
		}

		return -1;
	}
}