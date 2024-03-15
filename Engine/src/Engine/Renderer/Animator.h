#pragma once
#include "Animation.h"

namespace eg {
	
	class Animator {
	public:
		Animator();
		Animator(Ref<std::vector<Ref<Animation>>> animations, float speed);
		
		void Play();
		void Pause();
		void Stop();
		void Update(float dt);
		void ChangeAnimation(size_t animationIndex);
		void ChangeAnimation(const std::string& animationName);
		void SetAnimations(Ref<std::vector<Ref<Animation>>> animations);
		void SetAnimation(size_t index, Ref<Animation> animation);
		void AddAnimation(Ref<Animation> animation);
		void ResizeAnimations(size_t size);
		void AddEmptyAnimation();
		void AddAnimationWithName(const std::string& name);
		void RemoveAnimation(size_t index);
		void RemoveAnimation(const std::string& name);
		void RemoveLastAnimation();
		void SetSpeed(float speed) { m_Speed = speed; }

		Ref<Animation> GetCurrentAnimation() const;
		//const Animation& GetCurrentAnimationRef() const { return *m_Animations[m_AnimationIndex]; }
		const Ref<std::vector<Ref<Animation>>>& GetAnimations() const { return m_Animations; }
		Ref<Animation> GetAnimation(const std::string& name);
		Ref<Animation> GetAnimation(int index) { return (*m_Animations)[index]; }

		float GetSpeed() const { return m_Speed; }
		float* GetSpeedPtr() { return &m_Speed; }
		size_t GetAnimationIndex() const { return m_AnimationIndex; }
		size_t* GetAnimationIndexPtr() { return &m_AnimationIndex; }
		const std::vector<std::pair<size_t, size_t>>& GetTransitions() const { return m_Transitions; }
		
		
		void AddTransition(size_t fromIndex, size_t toIndex);
		void AddTransition(const std::string& fromName, const std::string& toName);
		void RemoveTransition(size_t fromIndex, size_t toIndex);
		void RemoveTransition(const std::string& fromName, const std::string& toName);
		bool CanTransition(size_t fromIndex, size_t toIndex);
		bool CanTransition(const std::string& fromName, const std::string& toName);

	private:
		int GetAnimationIndex(const std::string& name);

	private:
		float m_Speed = 0;
		size_t m_AnimationIndex = 0;
		Ref<std::vector<Ref<Animation>>> m_Animations;
		std::vector<std::pair<size_t, size_t>> m_Transitions;
	};
}