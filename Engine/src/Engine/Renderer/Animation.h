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
		static Ref<Animation> Create(const UUID& id);

		void Update(float dt, float speed);
		void Play();
		void Pause();
		void Start();
		void Stop();
		void SetLoop(bool loop);
		void SetFrame(int frame);
		void SetFrameRate(float frameRate);
		void SetName(const std::string& name);
		void SetID(const UUID& id);
		//void SetFrameSize(int frameSize);

		void RemoveFrame(int index);
		void ClearFrames();
		Ref<SubTexture2D> AddFrame(const Ref<SubTexture2D>& frame);
		void AddFrames(const std::vector<Ref<SubTexture2D>>& frames);

		inline const std::vector<Ref<SubTexture2D>>& GetFrames() const { return m_frames; }
		const Ref<SubTexture2D>& GetFrame() const;
		const Ref<SubTexture2D>& GetFrame(int frame) const;
		inline float GetCurrentFrame() const { return m_frame; }
		inline float* GetCurrentFramePtr() { return &m_frame; }
		inline const float GetFrameCount() const { return m_frames.size(); }
		inline bool IsPlaying() const { return m_playing; }
		inline bool* IsPlayingPtr() { return &m_playing; }
		inline bool IsLooped() const { return m_loop; }
		inline bool* IsLoopedPtr() { return &m_loop; }
		inline float GetFrameRate() const { return m_frameRate; }
		inline float* GetFrameRatePtr() { return &m_frameRate; }
		inline const std::string& GetName() const { return m_name; }
		inline const UUID& GetID() const { return m_AnimationID; }

	private:
		float m_frameRate;
		float m_frame = 0;
		bool m_loop = false;
		bool m_playing = false;
		UUID m_AnimationID;
		std::vector<Ref<SubTexture2D>> m_frames;

		std::string m_name = "";
	};
}