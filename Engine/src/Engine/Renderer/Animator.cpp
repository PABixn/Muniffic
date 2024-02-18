#include "egpch.h"
#include "Animator.h"

namespace eg {
		
	Animator::Animator()
		: m_AnimationIndex(0)
	{
	}

	Animator::Animator(Ref<std::vector<Animation>> animations)
		:m_AnimationIndex(0), m_Animations(animations)
	{
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

	void Animator::ChangeAnimation(int animationIndex)
	{
		(*m_Animations)[m_AnimationIndex].Stop();
		m_AnimationIndex = animationIndex;
		(*m_Animations)[m_AnimationIndex].Play();
	}

	void Animator::SetAnimations(Ref<std::vector<Animation>> animations)
	{
		m_Animations = animations;
	}

	void Animator::SetAnimation(int index, const Animation& animation)
	{
		(*m_Animations)[index] = animation;
	}

	void Animator::AddAnimation(const Animation& animation)
	{
		m_Animations->push_back(animation);
	}

	void Animator::ResizeAnimations(int size)
	{
		m_Animations->resize(size);
	}

	void Animator::AddEmptyAnimation()
	{
		m_Animations->push_back(Animation());
	}

	void Animator::RemoveAnimation(int index)
	{
		m_Animations->erase(m_Animations->begin() + index);
	}

	void Animator::RemoveLastAnimation()
	{
		m_Animations->pop_back();
	}

}