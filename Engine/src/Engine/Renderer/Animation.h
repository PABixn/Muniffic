#pragma once
#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "vector"
#include "unordered_map"

namespace eg {

	class FrameData {
	public:
		FrameData();
		FrameData(const UUID& id);
		FrameData(const UUID& id, const Ref<SubTexture2D>& subTexture, int frameDuration, bool isKeyFrame, const std::string& className, const std::string& functionCallName);
		~FrameData() = default;

		static Ref<FrameData> Create();
		static Ref<FrameData> Create(const UUID& id);
		static Ref<FrameData> Create(const UUID& id, const Ref<SubTexture2D>& subTexture, int frameDuration, bool isKeyFrame, const std::string& className, const std::string& functionCallName);

		inline void const SetFrameID(const UUID& id) { FrameID = id; }
		inline void const SetSubTexture(const Ref<SubTexture2D>& subTexture) { SubTexture = subTexture; }
		inline void const SetFrameDuration(int duration) { FrameDuration = duration; }
		inline void const SetIsKeyFrame(bool isKey) { isKeyFrame = isKey; }
		inline void const SetClassName(const std::string& className) { ClassName = className; }
		inline void const SetFunctionCallName(const std::string& functionCallName) { FunctionCallName = functionCallName; }
		inline void const SetEntityID(const UUID& id) { EntityID = id; }

		inline const UUID& GetFrameID() const { return FrameID; }
		inline const Ref<SubTexture2D>& GetSubTexture() const { return SubTexture; }
		inline const int GetFrameDuration() const { return FrameDuration; }
		inline const bool GetIsKeyFrame() const { return isKeyFrame; }
		inline const std::string& GetClassname() const { return ClassName; }
		inline const std::string& GetFunctionCallName() const { return FunctionCallName; }
		inline const UUID& GetEntityID() const { return EntityID; }

		void Save();
		void CallFunction(UUID entityid);

		static void DeleteAsset(UUID id);
		static void DeleteAssets(const std::vector<UUID>& ids);

	private:
		int FrameDuration = 1;
		bool isKeyFrame = false;
		
		UUID EntityID = 0;
		UUID FrameID = 0;
		Ref<SubTexture2D> SubTexture;
		std::string ClassName = "";
		std::string FunctionCallName = "";
	};

	class Animation {
	public:
		
		//Do not use constructors, use Animation::Create instead
		Animation();
		Animation(const UUID& id, const std::vector<Ref<FrameData>>& frames, float frameRate = 1.0f, bool loop = true);
		Animation(const std::string& path);
		Animation(const UUID&, const std::string& path);
		Animation(const std::vector<Ref<FrameData>>& frames, float frameRate = 1.0f, bool loop = true);
		~Animation() = default;

		inline static Ref<Animation> Create() { return CreateRef<Animation>();};
		static Ref<Animation> Create(const std::string& path);
		static Ref<Animation> Create(const std::vector<Ref<FrameData>>& frames, float frameRate = 1.0f, bool loop = true);
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
		void SetEntityID(const UUID& id);
		void Save();
		//void SetFrameSize(int frameSize);

		void RemoveFrame(int index);
		void ClearFrames();
		Ref<FrameData> AddFrame(const Ref<FrameData>& frame);
		void SwapFrames(int index1, int index2);
		void MoveFrame(int index, int newIndex);

		inline const std::vector<Ref<FrameData>>& GetFrames() const { return m_frames; }
		const Ref<FrameData> GetFrame() const;
		const Ref<FrameData> GetFrame(int frame) const;
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
		inline const UUID& GetEntityID() const { return m_EntityID; }

		const size_t GetAnimDuration();

	private:
		int m_frameRate;
		float m_frame = 0;
		float m_PreviousFrame = 0;
		bool m_loop = false;
		bool m_playing = false;
		bool m_AnimationEnded = false;
		UUID m_AnimationID;
		UUID m_EntityID;
		std::vector<Ref<FrameData>> m_frames;
		std::string m_name = "";
	};
}