#include "egpch.h"
#include "Animation.h"
#include "Engine/Resources/ResourceDatabase.h"
#include "Engine/Resources/Systems/ResourceSystem.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Resources/AssetDirectoryManager.h"

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

	Animation::Animation(const std::vector<Ref<FrameData>>& frames, float frameRate, bool loop)
		: m_frames(frames), m_frameRate(frameRate), m_loop(loop), m_playing(false), m_frame(0), m_AnimationID(UUID())
	{
	}

	Animation::Animation(const UUID& id, const std::vector<Ref<FrameData>>& frames, float frameRate, bool loop)
		: m_frames(frames), m_frameRate(frameRate), m_loop(loop), m_playing(false), m_frame(0), m_AnimationID(id)
	{
	}

	Ref<Animation> Animation::Create(const std::string& path)
	{
		return CreateRef<Animation>(path);
	}

	Ref<Animation> Animation::Create(const std::vector<Ref<FrameData>>& frames, float frameRate, bool loop)
	{
		return CreateRef<Animation>(frames, frameRate, loop);
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

		for (int i = 0; i<animData->Frames.size(); i++)
		{
			Ref<FrameData> frame = FrameData::Create(animData->Frames[i]);
			if (!frame)
				return nullptr;
			anim->m_frames.push_back(frame);
		}
		return anim;
	}

	void Animation::Update(float dt, float speed)
	{
		EG_PROFILE_FUNCTION();
		if (m_playing && m_frames.size() > 0)
		{
			m_AnimationEnded = false;
			m_frame += m_frameRate * speed * dt * (float)(1.0f/GetFrame()->GetFrameDuration());
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
			if ((int)m_frame < m_frame && (int)m_frame >= m_PreviousFrame && m_frames[(int)m_frame]->GetClassname() != "" && m_frames[(int)m_frame]->GetFunctionCallName() != "")
			{
				GetFrame((int)m_frame)->CallFunction(m_EntityID);
			}
			m_PreviousFrame = m_frame;
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

	void Animation::SetEntityID(const UUID& id)
	{
		m_EntityID = id;
		for (auto& frame : m_frames)
		{
			frame->SetEntityID(id);
		}
	}

	void Animation::Save() {
		EG_PROFILE_FUNCTION();
		AnimationResourceData* animData = (AnimationResourceData*)ResourceDatabase::GetResourceData(m_AnimationID);
		animData->FrameRate = m_frameRate;
		animData->FrameCount = m_frames.size();
		animData->Loop = m_loop;
		animData->Frames.clear();
		for(auto& frame : m_frames) {
			animData->Frames.push_back(frame->GetFrameID());
			frame->Save();
		}
	}

	void Animation::ClearFrames()
	{
		EG_PROFILE_FUNCTION();
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

	Ref<FrameData> Animation::AddFrame(const Ref<FrameData>& frame)
	{
		EG_PROFILE_FUNCTION();
		if (!frame->GetSubTexture())
		return FrameData::Create();
		m_frames.push_back(frame);
		return frame;
	}

	void Animation::SwapFrames(int index1, int index2)
	{
		EG_PROFILE_FUNCTION();
		if (index1 < m_frames.size() && index2 < m_frames.size())
		{
			Ref<FrameData> temp = m_frames[index1];
			m_frames[index1] = m_frames[index2];
			m_frames[index2] = temp;
		}
	}

	void Animation::MoveFrame(int from, int to)
	{
		EG_PROFILE_FUNCTION();
		if (from < m_frames.size() && to < m_frames.size())
		{
			Ref<FrameData> temp = m_frames[from];
			m_frames.erase(m_frames.begin() + from);
			m_frames.insert(m_frames.begin() + to, temp);
		}
	}

	const Ref<FrameData> Animation::GetFrame(int frame) const
	{
		EG_PROFILE_FUNCTION();
		if (m_frames.size() > 0 && frame < m_frames.size() && frame >= 0)
			return m_frames.at(frame);
		return FrameData::Create();
	}

	const Ref<FrameData> Animation::GetFrame() const
	{
		EG_PROFILE_FUNCTION();
		return GetFrame((int)m_frame);
	}

	Ref<FrameData> FrameData::Create()
	{
		return CreateRef<FrameData>();
	}

	Ref<FrameData> FrameData::Create(const UUID& id)
	{
		EG_PROFILE_FUNCTION();
		
		Ref<FrameData> frame = CreateRef<FrameData>();

		FrameResourceData* frameData = (FrameResourceData*)ResourceDatabase::GetResourceData(id);
		EG_ASSERT(frameData, "FrameData::Create() - FrameResourceData is null");
		frame->FrameID = id;
		frame->FrameDuration = frameData->Duration;
		frame->ClassName = frameData->ClassName;
		frame->FunctionCallName = frameData->FunctionCallName;
		frame->SubTexture = SubTexture2D::Create(frameData->SubTexture);
		

		return frame;
	
	}

	Ref<FrameData> FrameData::Create(const UUID& id, const Ref<SubTexture2D>& subTexture, int frameDuration, bool isKeyFrame, const std::string& className, const std::string& functionCallName)
	{
		EG_PROFILE_FUNCTION();
		Ref<FrameData> frame = CreateRef<FrameData>();
		frame->FrameID = id;
		frame->SubTexture = subTexture;
		frame->FrameDuration = frameDuration;
		frame->isKeyFrame = isKeyFrame;
		frame->ClassName = className;
		frame->FunctionCallName = functionCallName;
		frame->Save();
		return frame;
	}

	FrameData::FrameData()
		: FrameDuration(1), isKeyFrame(false), ClassName(""), FunctionCallName("")//, SubTexture(SubTexture2D::Create())
	{
	}

	FrameData::FrameData(const UUID& id)
		: FrameID(id)
	{
	}

	FrameData::FrameData(const UUID& id, const Ref<SubTexture2D>& subTexture, int frameDuration, bool isKeyFrame, const std::string& className, const std::string& functionCallName)
		: FrameID(id), FrameDuration(frameDuration), isKeyFrame(isKeyFrame), ClassName(className), FunctionCallName(functionCallName), SubTexture(subTexture)
	{
	}

	void FrameData::Save()
	{
		EG_PROFILE_FUNCTION();
		FrameResourceData* frameData = (FrameResourceData*)ResourceDatabase::GetResourceData(FrameID);
		EG_ASSERT(frameData, "FrameData::Save() - FrameResourceData is null");
		frameData->Duration = FrameDuration;
		frameData->ClassName = ClassName;
		frameData->FunctionCallName = FunctionCallName;
		frameData->SubTexture = SubTexture->GetId();
	}

	void FrameData::CallFunction(UUID entityID)
	{
		EG_PROFILE_FUNCTION();
		bool callMethod = ClassName != "" && FunctionCallName != "" && ScriptEngine::GetMethodParameterCount(ClassName, FunctionCallName) == 0;
		if (callMethod)
		{
			ScriptEngine::CallMethod(entityID, ClassName, FunctionCallName);
		}
	}

	void FrameData::DeleteAsset(UUID id)
	{
		EG_PROFILE_FUNCTION();
		UUID directoryID = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Frame);
		AssetDirectoryManager::removeAsset(directoryID, id);
	}

	void FrameData::DeleteAssets(const std::vector<UUID>& ids)
	{
		EG_PROFILE_FUNCTION();
		UUID directoryID = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Frame);
		for (auto& id : ids)
		{
			AssetDirectoryManager::removeAsset(directoryID, id);
		}
	}

}