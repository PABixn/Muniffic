#include "egpch.h"
#include "Animation.h"

namespace eg
{
	Animation::Animation()
		: m_frameRate(1.0f), m_loop(true), m_playing(false), m_frameCount(0), m_frame(0)
	{
	}

	Animation::Animation(const std::string& path)
		: m_frameRate(1.0f), m_loop(true), m_playing(false), m_frameCount(0), m_frame(0)
	{
		m_name = path;
	}

	Animation::Animation(const std::vector<Ref<SubTexture2D>>& frames, float frameRate, bool loop)
		: m_frames(frames), m_frameRate(frameRate), m_loop(loop), m_playing(false), m_frameCount(frames.size()), m_frame(0)
	{
	}

	Ref<Animation> Animation::Create(const std::string& path)
	{
		return CreateRef<Animation>(path);
	}

	Ref<Animation> Animation::Create(const std::vector<Ref<SubTexture2D>>& frames, float frameRate, bool loop)
	{
		return CreateRef<Animation>(frames, frameRate, loop);
	}

	void Animation::Update(float dt, float speed)
	{
		if (m_playing)
		{
			m_frame += m_frameRate * speed * dt;
			if (m_frame >= m_frameCount)
			{
				if (m_loop)
				{
					m_frame = 0;
				}
				else
				{
					m_frame = m_frameCount - 1;
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
		if (m_frame >= m_frameCount)
			m_frame = m_frameCount - 1;
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

	void Animation::ClearFrames()
	{
		m_frames.clear();
		m_frameCount = 0;
	}

	void Animation::RemoveFrame(int index)
	{
		if (index < m_frameCount)
		{
			m_frames.erase(m_frames.begin() + index);
			m_frameCount--;
		}
	}

	void Animation::AddFrame(const Ref<SubTexture2D>& frame)
	{
		m_frames.push_back(frame);
		m_frameCount++;
	}

	void Animation::AddFrames(const std::vector<Ref<SubTexture2D>>& frames)
	{
		m_frames.insert(m_frames.end(), frames.begin(), frames.end());
		m_frameCount += frames.size();
	}

}
