#pragma once
#include <Engine.h>
#include "Commands/Commands.h"

namespace eg
{
	class EditorActions
	{
	public:
		static std::string ExecuteEntityAction(const std::string& actionName, const std::vector<std::string>& params);
		static std::string ExecuteComponentAction(const std::string& actionName, const std::vector<std::string>& params);

		static void SetScene(const Ref<Scene>& scene) { m_Scene = scene; }
		static void SetSelectionContext(Entity* entity) { m_SelectedEntity = entity; }
		
    private:
        static std::string GetUUID(const std::vector<std::string>& params);
        static std::string Exists(const std::vector<std::string>& params);
        static std::string GetAllEntities(const std::vector<std::string>& params);
        static std::string ChangeName(const std::vector<std::string>& params);
        static std::string CreateEntity(const std::vector<std::string>& params);
        static std::string DeleteEntity(const std::vector<std::string>& params);
        static std::string GetAnyChildren(const std::vector<std::string>& params);
        static std::string GetChildren(const std::vector<std::string>& params);
        static std::string GetParent(const std::vector<std::string>& params);
        static std::string SetParent(const std::vector<std::string>& params);
        static std::string IsChild(const std::vector<std::string>& params);
        static std::string IsChildOfAny(const std::vector<std::string>& params);

        static std::string AddComponent(const std::vector<std::string>& params);
        static std::string RemoveComponent(const std::vector<std::string>& params);
        static std::string HasComponent(const std::vector<std::string>& params);
        static std::string InheritComponentInChildren(const std::vector<std::string>& params);
        static std::string InheritComponentFromParent(const std::vector<std::string>& params);
        static std::string CopyComponentToChildren(const std::vector<std::string>& params);
        static std::string CopyComponentValuesToChildren(const std::vector<std::string>& params);
        static std::string CopyComponentWithValuesToChildren(const std::vector<std::string>& params);
        static std::string IsInheritedFromParent(const std::vector<std::string>& params);
        static std::string IsInheritedInChildren(const std::vector<std::string>& params);

        static std::string GetTransformComponent(const std::vector<std::string>& params);
        static std::string SetTransformComponent(const std::vector<std::string>& params);

        static std::string GetCameraComponent(const std::vector<std::string>& params);
        static std::string SetCameraComponent(const std::vector<std::string>& params);

        static std::string GetSpriteRendererComponent(const std::vector<std::string>& params);
        static std::string SetSpriteRendererComponent(const std::vector<std::string>& params);

        static std::string GetCircleRendererComponent(const std::vector<std::string>& params);
        static std::string SetCircleRendererComponent(const std::vector<std::string>& params);

        static std::string GetRigidBody2DComponent(const std::vector<std::string>& params);
        static std::string SetRigidBody2DComponent(const std::vector<std::string>& params);

        static std::string GetBoxCollider2DComponent(const std::vector<std::string>& params);
        static std::string SetBoxCollider2DComponent(const std::vector<std::string>& params);

        static std::string GetCircleCollider2DComponent(const std::vector<std::string>& params);
        static std::string SetCircleCollider2DComponent(const std::vector<std::string>& params);

        static std::string GetTextComponent(const std::vector<std::string>& params);
        static std::string SetTextComponent(const std::vector<std::string>& params);


	private:
		static Ref<Scene> m_Scene;
		static Entity* m_SelectedEntity;
	};
}