#include "egpch.h"
#include "Animator.h"

namespace eg {
		
	Animator::Animator()
		: m_AnimationIndex(0), m_Speed(1.0f)
	{
		m_Animations = CreateRef<std::vector<Ref<Animation>>>();
	}

	Animator::Animator(Ref<std::vector<Ref<Animation>>> animations, float speed)
		:m_AnimationIndex(0), m_Animations(animations), m_Speed(speed)
	{
		m_Transitions.resize(animations->size());
	}

	void Animator::Play()
	{
		(*m_Animations)[m_AnimationIndex]->Play();
	}

	void Animator::Pause()
	{
		(*m_Animations)[m_AnimationIndex]->Pause();
	}

	void Animator::Stop()
	{
		(*m_Animations)[m_AnimationIndex]->Stop();
	}

	void Animator::Update(float dt)
	{
		if(m_Animations->size() > 0)
			(*m_Animations)[m_AnimationIndex]->Update(dt, m_Speed);
	}

	void Animator::ChangeAnimation(size_t animationIndex)
	{
		if (!CanTransition(m_AnimationIndex, animationIndex))
			return;
		(*m_Animations)[m_AnimationIndex]->Stop();
		m_AnimationIndex = animationIndex;
		(*m_Animations)[m_AnimationIndex]->Play();
	}

	void Animator::ChangeAnimation(const std::string& animationName)
	{
		int index = GetAnimationIndex(animationName);
		if (index >= 0)
			ChangeAnimation(index);
	
	}

	void Animator::SetAnimations(Ref<std::vector<Ref<Animation>>> animations)
	{
		m_Animations = animations;
	}

	void Animator::SetAnimation(size_t index, Ref<Animation> animation)
	{
		(*m_Animations)[index] = animation;
	}

	void Animator::AddAnimation(Ref<Animation> animation)
	{
		m_Animations->push_back(animation);
	}

	void Animator::ResizeAnimations(size_t size)
	{
		m_Animations->resize(size);
		m_Transitions.resize(size);
	}

	void Animator::AddEmptyAnimation()
	{
		m_Animations->push_back(CreateRef<Animation>());
	}

	void Animator::AddAnimationWithName(const std::string& name)
	{
		m_Animations->push_back(CreateRef<Animation>(name));
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

	Ref<Animation> Animator::GetCurrentAnimation() const
	{
		if(m_Animations->size() > 0)
			return (*m_Animations)[m_AnimationIndex];
		return nullptr;
	}

	void Animator::AddTransition(size_t fromIndex, size_t toIndex)
	{
		if (fromIndex < m_Transitions.size() && toIndex != fromIndex && !CanTransition(fromIndex, toIndex))
			m_Transitions.push_back(std::make_pair(fromIndex, toIndex));
	}

	void Animator::AddTransition(const std::string& fromName, const std::string& toName)
	{
		int fromAnimIndex = GetAnimationIndex(fromName);
		if (fromAnimIndex < 0)
			return;
		int toAnimIndex = GetAnimationIndex(toName);
		if (toAnimIndex < 0)
			return;
		AddTransition(fromAnimIndex, toAnimIndex);
	}

	void Animator::RemoveTransition(size_t fromIndex, size_t toIndex)
	{
		if (fromIndex < m_Transitions.size() &&  toIndex < m_Transitions.size())
		{
			for(int i = 0; i < m_Transitions.size(); i++)
				if (m_Transitions[i].first == fromIndex && m_Transitions[i].second == toIndex)
					m_Transitions.erase(m_Transitions.begin() + i);
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
		for (int i = 0; i < m_Transitions.size(); i++)
			if (m_Transitions[i].first == fromIndex && m_Transitions[i].second == toIndex)
				return true;
		return false;
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

	Ref<Animation> Animator::GetAnimation(const std::string& name)
	{
		{
			int index = GetAnimationIndex(name);
			if (index < 0)
				return nullptr;
			return (*m_Animations)[GetAnimationIndex(name)]; }
	}

	int Animator::GetAnimationIndex(const std::string& name)
	{
		int i = 0;
		for (Ref<Animation> anim : (*m_Animations))
		{
			if (anim->GetName() == name)
				return i;
			i++;
		}

		return -1;
	}
}