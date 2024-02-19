#pragma once
#include "Animation.h"

namespace eg {
	
	class Animator {
	public:
		Animator();
		Animator(Ref<std::vector<Animation>> animations, float speed);
		
		void Play();
		void Pause();
		void Stop();
		void Update(float dt);
		void ChangeAnimation(int animationIndex);
		void SetAnimations(Ref<std::vector<Animation>> animations);
		void SetAnimation(int index, const Animation& animation);
		void AddAnimation(const Animation& animation);
		void ResizeAnimations(int size);
		void AddEmptyAnimation();
		void AddAnimationWithName(const std::string& name);
		void RemoveAnimation(int index);
		void RemoveLastAnimation();
		void SetSpeed(float speed) { m_Speed = speed; }

		Ref<std::vector<Animation>> GetAnimations() { return m_Animations; }
		const Ref<std::vector<Animation>> GetAnimations() const { return m_Animations; }
		const Animation& GetAnimation(int index) { return (*m_Animations)[index]; }
		const Animation& GetCurrentAnimation() { return (*m_Animations)[m_AnimationIndex]; }
		float GetSpeed() const { return m_Speed; }
		float* GetSpeedPtr() { return &m_Speed; }
		int GetAnimationIndex() const { return m_AnimationIndex; }
		int* GetAnimationIndexPtr() { return &m_AnimationIndex; }

	private:
		int m_AnimationIndex = 0;
		float m_Speed = 0;
		Ref<std::vector<Animation>> m_Animations;
	};
}