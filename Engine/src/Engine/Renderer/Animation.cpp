#include "egpch.h"
#include "Animation.h"

namespace eg
{
	Animation::Animation()
		: m_frameRate(1.0f), m_loop(true), m_playing(false), m_frameCount(0), m_frame(0), m_speed(1.0f)
	{
	}

	Animation::Animation(const std::vector<Ref<SubTexture2D>>& frames, float frameRate, bool loop)
		: m_frames(frames), m_frameRate(frameRate), m_loop(loop), m_playing(false), m_frameCount(frames.size()), m_frame(0), m_speed(1.0f)
	{
	}

	void Animation::Update(float dt)
	{
		if (m_playing)
		{
			m_frame += m_frameRate * m_speed * dt;
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

	void Animation::SetSpeed(float speed)
	{
		m_speed = speed;
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


}
