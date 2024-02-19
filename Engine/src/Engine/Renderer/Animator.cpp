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

	void Animator::AddAnimationWithName(const std::string& name)
	{
		m_Animations->push_back(Animation(name));
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