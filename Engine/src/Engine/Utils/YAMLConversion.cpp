#include "egpch.h"
#include "YAMLConversion.h"


namespace YAML
{
	Node convert<glm::vec2>::encode(const glm::vec2& rhs)
	{
        EG_PROFILE_FUNCTION();
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		//node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	bool convert<glm::vec2>::decode(const Node& node, glm::vec2& rhs)
	{
        EG_PROFILE_FUNCTION();
		if (!node.IsSequence() || node.size() != 2)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		return true;
	}

	Node convert<glm::vec3>::encode(const glm::vec3& rhs)
	{
        EG_PROFILE_FUNCTION();
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		//node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	bool convert<glm::vec3>::decode(const Node& node, glm::vec3& rhs)
	{
        EG_PROFILE_FUNCTION();
		if (!node.IsSequence() || node.size() != 3)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		return true;
	}

	Node convert<glm::vec4>::encode(const glm::vec4& rhs)
	{
        EG_PROFILE_FUNCTION();
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);
		//node.SetStyle(EmitterStyle::Flow);
		return node;
	}

	bool convert<glm::vec4>::decode(const Node& node, glm::vec4& rhs)
	{
        EG_PROFILE_FUNCTION();
		if (!node.IsSequence() || node.size() != 4)
			return false;

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();
		return true;
	}

	Node convert<eg::UUID>::encode(const eg::UUID& uuid)
	{
        EG_PROFILE_FUNCTION();
		Node node;
		node.push_back((int64_t)uuid);
		return node;
	}

	bool convert<eg::UUID>::decode(const Node& node, eg::UUID& uuid)
	{
        EG_PROFILE_FUNCTION();
		uuid = node.as<int64_t>();
		return true;
	}
	Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
        EG_PROFILE_FUNCTION();
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
        EG_PROFILE_FUNCTION();
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
        EG_PROFILE_FUNCTION();
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

}
