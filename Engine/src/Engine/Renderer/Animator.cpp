#include "egpch.h"
#include "Animator.h"

namespace eg {
		
	Animator::Animator()
		: m_AnimationIndex(0), m_Speed(1.0f)
	{
		m_Animations = CreateRef<std::vector<Ref<Animation>>>();
		m_Transitions = CreateRef<std::vector<std::pair<size_t, size_t>>>();
	}

	Animator::Animator(Ref<std::vector<Ref<Animation>>> animations, float speed)
		:m_AnimationIndex(0), m_Animations(animations), m_Speed(speed)
	{
	}

	void Animator::Play()
	{
		EG_PROFILE_FUNCTION();
		(*m_Animations)[m_AnimationIndex]->Play();
	}

	void Animator::Pause()
	{
		EG_PROFILE_FUNCTION();
		(*m_Animations)[m_AnimationIndex]->Pause();
	}

	void Animator::Stop()
	{
		EG_PROFILE_FUNCTION();
		(*m_Animations)[m_AnimationIndex]->Stop();
	}

	void Animator::Update(float dt)
	{
		EG_PROFILE_FUNCTION();
		if (m_Animations->size() > 0)
		{
			(*m_Animations)[m_AnimationIndex]->Update(dt, m_Speed);
			if (m_NextAnimationIndex >= 0 && (*m_Animations)[m_AnimationIndex]->DidAnimationEnd())
			{
				(*m_Animations)[m_AnimationIndex]->Stop();
				m_AnimationIndex = m_NextAnimationIndex;
				m_NextAnimationIndex = -1;
				(*m_Animations)[m_AnimationIndex]->Start();
			}
		}
	}

	void Animator::ChangeAnimation(size_t animationIndex)
	{
		EG_PROFILE_FUNCTION();
		if (!CanTransition(m_AnimationIndex, animationIndex))
			return;
		(*m_Animations)[m_AnimationIndex]->Stop();
		m_AnimationIndex = animationIndex;
		(*m_Animations)[m_AnimationIndex]->Play();
	}

	void Animator::ChangeAnimation(const std::string& animationName)
	{
		EG_PROFILE_FUNCTION();
		int index = GetAnimationIndex(animationName);
		if (index >= 0)
			ChangeAnimation(index);
	
	}

	void Animator::SetAnimations(Ref<std::vector<Ref<Animation>>> animations)
	{
		EG_PROFILE_FUNCTION();
		m_Animations = animations;
	}

	void Animator::SetAnimation(size_t index, Ref<Animation> animation)
	{
		EG_PROFILE_FUNCTION();
		(*m_Animations)[index] = animation;
	}

	void Animator::AddAnimation(Ref<Animation> animation)
	{
		EG_PROFILE_FUNCTION();
		m_Animations->push_back(animation);
	}

	void Animator::ResizeAnimations(size_t size)
	{
		EG_PROFILE_FUNCTION();
		if(size > m_Animations->size())
			m_Animations->resize(size);
	}

	void Animator::AddEmptyAnimation()
	{
		EG_PROFILE_FUNCTION();
		m_Animations->push_back(CreateRef<Animation>());
	}

	void Animator::AddAnimationWithName(const std::string& name)
	{
		EG_PROFILE_FUNCTION();
		m_Animations->push_back(CreateRef<Animation>(name));
	}

	void Animator::RemoveAnimation(size_t index)
	{
		EG_PROFILE_FUNCTION();
		if (index >= m_Animations->size())
			return;
		m_Animations->erase(m_Animations->begin() + index);
	}

	void Animator::RemoveAnimation(const std::string& name)
	{
		EG_PROFILE_FUNCTION();
		size_t animIndex = GetAnimationIndex(name);
		if (animIndex >= 0)
			m_Animations->erase(m_Animations->begin() + animIndex);
	}

	void Animator::RemoveLastAnimation()
	{
		EG_PROFILE_FUNCTION();
		m_Animations->pop_back();
	}

	Ref<Animation> Animator::GetCurrentAnimation() const
	{
		EG_PROFILE_FUNCTION();
		if(m_Animations->size() > 0)
			return (*m_Animations)[m_AnimationIndex];
		return nullptr;
	}

	void Animator::Transition(size_t toIndex)
	{
		EG_PROFILE_FUNCTION();
		if (!CanTransition(m_AnimationIndex, toIndex))
			return;
		m_NextAnimationIndex = toIndex;
	}

	void Animator::Transition(const std::string& toName)
	{
		EG_PROFILE_FUNCTION();
		int index = GetAnimationIndex(toName);
		if (index >= 0)
			Transition(index);
	}

	void Animator::AddTransition(size_t fromIndex, size_t toIndex)
	{
		EG_PROFILE_FUNCTION();
		if (toIndex != fromIndex && !CanTransition(fromIndex, toIndex))
			m_Transitions->push_back(std::make_pair(fromIndex, toIndex));
	}

	void Animator::AddTransition(const std::pair<size_t, size_t>& transition)
	{
		EG_PROFILE_FUNCTION();
		if (transition.second != transition.first && !CanTransition(transition.first, transition.second))
			m_Transitions->push_back(transition);
	}

	void Animator::AddTransition(const std::string& fromName, const std::string& toName)
	{
		EG_PROFILE_FUNCTION();
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
		EG_PROFILE_FUNCTION();
		if (fromIndex >= 0 && fromIndex < m_Animations->size() && toIndex < m_Animations->size() && toIndex >= 0)
		{
			for (int i = 0; i < m_Transitions->size(); i++)
			{
				const std::pair<size_t, size_t>& transition = (*m_Transitions)[i];
				if (transition.first == fromIndex && transition.second == toIndex)
					m_Transitions->erase(m_Transitions->begin() + i);
			}
		}
	}

	void Animator::RemoveTransition(const std::string& fromName, const std::string& toName)
	{
		EG_PROFILE_FUNCTION();
		int fromAnimIndex = GetAnimationIndex(fromName);
		if (fromAnimIndex < 0)
			return;
		int toAnimIndex = GetAnimationIndex(toName);
		if (toAnimIndex < 0)
			return;
		RemoveTransition(fromAnimIndex, toAnimIndex);
	}

	void Animator::RemoveTransition(const std::pair<size_t, size_t>& transition)
	{
		EG_PROFILE_FUNCTION();
		RemoveTransition(transition.first, transition.second);
	}

	bool Animator::CanTransition(size_t fromIndex, size_t toIndex)
	{
		EG_PROFILE_FUNCTION();
		for (int i = 0; i < m_Transitions->size(); i++)
			if ((*m_Transitions)[i].first == fromIndex && (*m_Transitions)[i].second == toIndex)
				return true;
		return false;
	}

	bool Animator::CanTransition(const std::string& fromName, const std::string& toName)
	{
		EG_PROFILE_FUNCTION();
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
		EG_PROFILE_FUNCTION();
		int index = GetAnimationIndex(name);
		if (index < 0)
			return nullptr;
		return (*m_Animations)[GetAnimationIndex(name)];
	}

	int Animator::GetAnimationIndex(const std::string& name)
	{
		EG_PROFILE_FUNCTION();
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