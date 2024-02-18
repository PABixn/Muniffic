#pragma once
#include "Animation.h"

namespace eg {
	
	class Animator {
	public:
		Animator();
		Animator(Ref<std::vector<Animation>> animations);
		
		void Play();
		void Pause();
		void Stop();
		void ChangeAnimation(int animationIndex);
		void SetAnimations(Ref<std::vector<Animation>> animations);
		void SetAnimation(int index, const Animation& animation);
		void AddAnimation(const Animation& animation);
		void ResizeAnimations(int size);
		void AddEmptyAnimation();
		void RemoveAnimation(int index);
		void RemoveLastAnimation();
		void SetSpeed(float speed) { m_Speed = speed; }

		Ref<std::vector<Animation>> GetAnimations() { return m_Animations; }
		const Ref<std::vector<Animation>> GetAnimations() const { return m_Animations; }
		const Animation& GetAnimation(int index) { return (*m_Animations)[index]; }
		float GetSpeed() const { return m_Speed; }

	private:
		int m_AnimationIndex;
		float m_Speed;
		Ref<std::vector<Animation>> m_Animations;
	};
}