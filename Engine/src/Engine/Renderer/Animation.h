#pragma once
#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "vector"
#include "unordered_map"

namespace eg {
	
	class Animation {
	public:
		struct FrameData {
			Ref<SubTexture2D> SubTexture;
			int FrameDuration;
			//float FramePosition;
			bool isKeyFrame;
		};
		//Do not use constructors, use Animation::Create instead
		Animation();
		Animation(const UUID& id, const std::vector<FrameData>& frames, float frameRate = 1.0f, bool loop = true);
		Animation(const std::string& path);
		Animation(const UUID&, const std::string& path);
		Animation(const std::vector<FrameData>& frames, float frameRate = 1.0f, bool loop = true);
		~Animation() = default;

		inline static Ref<Animation> Create() { return CreateRef<Animation>();};
		static Ref<Animation> Create(const std::string& path);
		static Ref<Animation> Create(const std::vector<FrameData>& frames, float frameRate = 1.0f, bool loop = true);
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
		void Save();
		//void SetFrameSize(int frameSize);

		void RemoveFrame(int index);
		void ClearFrames();
		FrameData AddFrame(const FrameData& frame);
		void AddFrames(const std::vector<Ref<SubTexture2D>>& frames);

		inline const std::vector<FrameData>& GetFrames() const { return m_frames; }
		const FrameData& GetFrame() const;
		const FrameData& GetFrame(int frame) const;
		inline float GetCurrentFrame() const { return m_frame; }
		inline float* GetCurrentFramePtr() { return &m_frame; }
		inline const float GetFrameCount() const { return m_frames.size(); }
		inline bool IsPlaying() const { return m_playing; }
		inline bool* IsPlayingPtr() { return &m_playing; }
		inline bool IsLooped() const { return m_loop; }
		inline bool* IsLoopedPtr() { return &m_loop; }
		inline bool DidAnimationEnd() const { return m_AnimationEnded; }
		inline int GetFrameRate() const { return m_frameRate; }
		inline int* GetFrameRatePtr() { return &m_frameRate; }
		inline const std::string& GetName() const { return m_name; }
		inline const UUID& GetID() const { return m_AnimationID; }

	private:
		int m_frameRate;
		float m_frame = 0;
		bool m_loop = false;
		bool m_playing = false;
		bool m_AnimationEnded = false;
		UUID m_AnimationID;
		std::vector<FrameData> m_frames;

		std::string m_name = "";
	};
}