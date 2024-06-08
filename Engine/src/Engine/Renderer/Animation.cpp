#include "egpch.h"
#include "Animation.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Resources/Systems/ResourceSystem.h"
namespace eg
{
	Animation::Animation()
		: m_frameRate(1.0f), m_loop(true), m_playing(false), m_frame(0), m_AnimationID(UUID())
	{
	}

	Animation::Animation(const std::string& path)
		: m_frameRate(1.0f), m_loop(true), m_playing(false), m_frame(0), m_AnimationID(UUID())
	{
		m_name = path;
	}

	Animation::Animation(const UUID&, const std::string& path)
		: m_frameRate(1.0f), m_loop(true), m_playing(false), m_frame(0), m_AnimationID(UUID())
	{
		m_name = path;
	}

	Animation::Animation(const std::vector<FrameData>& frames, float frameRate, bool loop)
		: m_frames(frames), m_frameRate(frameRate), m_loop(loop), m_playing(false), m_frame(0), m_AnimationID(UUID())
	{
	}

	Animation::Animation(const UUID& id, const std::vector<FrameData>& frames, float frameRate, bool loop)
		: m_frames(frames), m_frameRate(frameRate), m_loop(loop), m_playing(false), m_frame(0), m_AnimationID(id)
	{
	}

	/*Ref<Animation> Animation::Create()
	 {
		return CreateRef<Animation>();
	}*/

	Ref<Animation> Animation::Create(const std::string& path)
	{
		return CreateRef<Animation>(path);
	}

	Ref<Animation> Animation::Create(const std::vector<FrameData>& frames, float frameRate, bool loop)
	{
		return CreateRef<Animation>(frames, frameRate, loop);
		//return nullptr;
	}

	Ref<Animation> Animation::Create(const UUID& id)
	{
		EG_PROFILE_FUNCTION();
		if (!ResourceDatabase::FindResourceData(id, ResourceType::Animation))
			return nullptr;
		AnimationResourceData* animData = (AnimationResourceData*)ResourceDatabase::GetResourceData(id);
		Ref<Animation> anim = CreateRef<Animation>();
		anim->m_AnimationID = id;
		anim->m_name = animData->ResourceName;
		anim->m_frameRate = animData->FrameRate;
		anim->m_loop = animData->Loop;
		anim->m_frame = 0;

		for (auto& frame : animData->Frames)
		{
			if (!anim->AddFrame(FrameData(SubTexture2D::Create(frame),1.0f)).SubTexture)
				return nullptr;
		}
		return anim;
	}

	void Animation::Update(float dt, float speed)
	{
		if (m_playing && m_frames.size() > 0)
		{
			m_AnimationEnded = false;
			m_frame += m_frameRate * speed * dt * (float)(1.0f/GetFrame().FrameDuration);
			if ((int)m_frame >= m_frames.size())
			{
				m_AnimationEnded = true;
				if (m_loop)
				{
					m_frame = 0;
				}
				else
				{
					m_frame = m_frames.size() - 1;
					m_playing = false;
				}
			}
		}
	}

	void Animation::Play()
	{
		m_playing = true;
	}

	void Animation::Pause()
	{
		m_playing = false;
	}

	void Animation::Start()
	{
		m_playing = true;
		m_frame = 0;
	}

	void Animation::Stop()
	{
		m_playing = false;
		m_frame = 0;
	}

	void Animation::SetLoop(bool loop)
	{
		m_loop = loop;
	}

	void Animation::SetFrame(int frame)
	{
		m_frame = frame;
		if (m_frame >= m_frames.size())
			m_frame = m_frames.size() - 1;
		if (m_frame < 0)
			m_frame = 0;
	}

	void Animation::SetFrameRate(float frameRate)
	{
		m_frameRate = frameRate;
	}

	void Animation::SetName(const std::string& name)
	{
		m_name = name;
	}

	void Animation::SetID(const UUID& id)
	{
		m_AnimationID = id;
	}

	void Animation::ClearFrames()
	{
		m_frames.clear();
	}

	void Animation::RemoveFrame(int index)
	{
		EG_PROFILE_FUNCTION();
		if (index < m_frames.size())
		{
			m_frames.erase(m_frames.begin() + index);
		}
	}

	Animation::FrameData Animation::AddFrame(const FrameData& frame)
	{
		EG_PROFILE_FUNCTION();
		if (!frame.SubTexture)
			return Animation::FrameData{};
		m_frames.push_back(frame);
		return frame;
	}

	void Animation::AddFrames(const std::vector<Ref<SubTexture2D>>& frames)
	{
		EG_PROFILE_FUNCTION();
		m_frames.insert(m_frames.end(), frames.begin(), frames.end());
	}
	const Animation::FrameData& Animation::GetFrame(int frame) const
	{
		EG_PROFILE_FUNCTION();
		if (m_frames.size() > 0 && frame < m_frames.size() && frame >= 0)
			return m_frames[frame];
		return Animation::FrameData{};
	}
	const Animation::FrameData& Animation::GetFrame() const
	{
		EG_PROFILE_FUNCTION();
		return GetFrame((int)m_frame);
	}
}