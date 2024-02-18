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


	private:
		int m_AnimationIndex;
		Ref<std::vector<Animation>> m_Animations;
	};
}