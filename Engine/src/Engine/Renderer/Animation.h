#pragma once
#include "vector"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/SubTexture2D.h"

namespace eg {
	
	class Animation {
	public:
		Animation();
		Animation(const std::vector<Ref<SubTexture2D>>& frames, float frameRate = 1.0f, bool loop = true);
		~Animation() = default;

		void Update(float dt);
		void Play();
		void Pause();
		void Stop();
		void SetSpeed(float speed);
		void SetLoop(bool loop);
		void SetFrame(int frame);
		void SetFrameRate(float frameRate);
		//void SetFrameSize(int frameSize);

		inline const std::vector<Ref<SubTexture2D>>& GetFrames() const { return m_frames; }
		inline const Ref<SubTexture2D>& GetFrame() const { return m_frames[(int)m_frame]; }
		inline const Ref<SubTexture2D>& GetFrame(int frame) const { return m_frames[frame]; }
		inline int GetFrameCount() const { return m_frameCount; }
		inline int GetCurrentFrame() const { return m_frame; }
		inline bool IsPlaying() const { return m_playing; }
		inline bool IsLooping() const { return m_loop; }
		inline float GetSpeed() const { return m_speed; }
		inline float GetFrameRate() const { return m_frameRate; }

	private:
		float m_speed;
		float m_frameRate;
		int m_frameCount;
		int m_frame;
		bool m_loop;
		bool m_playing;
		std::vector<Ref<SubTexture2D>> m_frames;
	};
}