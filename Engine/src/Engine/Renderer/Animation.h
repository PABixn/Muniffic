#pragma once
#include "vector"
#include "unordered_map"
#include "Engine/Core/Core.h"
#include "Engine/Renderer/SubTexture2D.h"

namespace eg {
	
	class Animation {
	public:
		Animation();
		Animation(const std::vector<Ref<SubTexture2D>>& frames, float frameRate = 1.0f, bool loop = true);
		~Animation() = default;

		static const Animation& Create(const std::string& path);

		void Update(float dt, float speed);
		void Play();
		void Pause();
		void Stop();
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
		inline float GetFrameRate() const { return m_frameRate; }
		inline const std::string& GetName() const { return m_name; }

	private:
		float m_frameRate;
		int m_frameCount;
		int m_frame;
		bool m_loop;
		bool m_playing;
		std::vector<Ref<SubTexture2D>> m_frames;
		std::string m_name;
	};
}