#pragma once
#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "vector"
#include "unordered_map"

namespace eg {
	
	class Animation {
	public:
		Animation();
		Animation(const std::string& path);
		Animation(const UUID&, const std::string& path);
		Animation(const std::vector<Ref<SubTexture2D>>& frames, float frameRate = 1.0f, bool loop = true);
		Animation(const UUID& id, const std::vector<Ref<SubTexture2D>>& frames, float frameRate = 1.0f, bool loop = true);
		~Animation() = default;

		static Ref<Animation> Create(const std::string& path);
		static Ref<Animation> Create(const std::vector<Ref<SubTexture2D>>& frames, float frameRate = 1.0f, bool loop = true);

		void Update(float dt, float speed);
		void Play();
		void Pause();
		void Stop();
		void SetLoop(bool loop);
		void SetFrame(int frame);
		void SetFrameRate(float frameRate);
		void SetName(const std::string& name);
		void SetID(const UUID& id);
		//void SetFrameSize(int frameSize);

		void RemoveFrame(int index);
		void ClearFrames();
		void AddFrame(const Ref<SubTexture2D>& frame);
		void AddFrames(const std::vector<Ref<SubTexture2D>>& frames);

		inline const std::vector<Ref<SubTexture2D>>& GetFrames() const { return m_frames; }
		inline const Ref<SubTexture2D>& GetFrame() const { return m_frames[(int)m_frame]; }
		inline const Ref<SubTexture2D>& GetFrame(int frame) const { return m_frames[frame]; }
		inline int GetFrameCount() const { return m_frameCount; }
		inline float GetCurrentFrame() const { return m_frame; }
		inline float* GetCurrentFramePtr() { return &m_frame; }
		inline bool IsPlaying() const { return m_playing; }
		inline bool* IsPlayingPtr() { return &m_playing; }
		inline bool IsLooping() const { return m_loop; }
		inline bool* IsLoopingPtr() { return &m_loop; }
		inline float GetFrameRate() const { return m_frameRate; }
		inline float* GetFrameRatePtr() { return &m_frameRate; }
		inline const std::string& GetName() const { return m_name; }
		inline const UUID& GetID() const { return m_AnimationID; }

	private:
		float m_frameRate;
		int m_frameCount;
		float m_frame = 0;
		bool m_loop;
		bool m_playing;
		UUID m_AnimationID;
		std::vector<Ref<SubTexture2D>> m_frames;

		std::string m_name;
	};
}