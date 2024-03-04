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
		void ChangeAnimation(size_t animationIndex);
		void SetAnimations(Ref<std::vector<Animation>> animations);
		void SetAnimation(size_t index, const Animation& animation);
		void AddAnimation(const Animation& animation);
		void ResizeAnimations(size_t size);
		void AddEmptyAnimation();
		void AddAnimationWithName(const std::string& name);
		void RemoveAnimation(size_t index);
		void RemoveAnimation(const std::string& name);
		void RemoveLastAnimation();
		void SetSpeed(float speed) { m_Speed = speed; }

		Ref<std::vector<Animation>> GetAnimations() { return m_Animations; }
		const Ref<std::vector<Animation>> GetAnimations() const { return m_Animations; }
		const Animation& GetAnimation(int index) { return (*m_Animations)[index]; }
		const Animation& GetCurrentAnimation() { return (*m_Animations)[m_AnimationIndex]; }
		float GetSpeed() const { return m_Speed; }
		float* GetSpeedPtr() { return &m_Speed; }
		size_t GetAnimationIndex() const { return m_AnimationIndex; }
		size_t* GetAnimationIndexPtr() { return &m_AnimationIndex; }
		
		
		void AddTransition(size_t fromIndex, size_t toIndex);
		void AddTransition(const std::string& fromName, const std::string& toName);
		bool CanTransition(size_t fromIndex, size_t toIndex);
		bool CanTransition(const std::string& fromName, const std::string& toName);

	private:
		Animation* GetAnimation(const std::string& name);
		int GetAnimationIndex(const std::string& name);

	private:
		float m_Speed = 0;
		size_t m_AnimationIndex = 0;
		Ref<std::vector<Animation>> m_Animations;
		std::vector<std::vector<size_t>> m_Transitions;
	};
}