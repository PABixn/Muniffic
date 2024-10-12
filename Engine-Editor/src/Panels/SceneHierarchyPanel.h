#pragma once
#include "Engine.h"
#include "ResourcesPanels/ImagePanel.h"
//#include "ConsolePanel.h"
#include "Imgui/imgui.h"
#include "../IconLoader.h"

namespace eg {
	enum ComponentIcons {
		ComponentIcon = 0,
		TransformIcon,
		CameraIcon,
		ScriptIcon,
		SpriteRendererIcon,
		SubTextureRendererIcon,
		CircleRendererIcon,
		RigidBodyIcon,
		BoxColliderIcon,
		CircleColliderIcon,
		TextRendererIcon,
		AnimatorIcon
	};
	enum searchRes
	{
		ChildSearched,
		thisSearched,
		parentSearched,
		bothSearched
	};
	struct EntityDisplayInfo
	{
		EntityDisplayInfo() {
			childInfo = std::vector<EntityDisplayInfo>();
		};
		EntityDisplayInfo(Entity e) :entity(e) {
			res = searchRes::parentSearched;
			childInfo = std::vector<EntityDisplayInfo>();
		};
		EntityDisplayInfo(Entity ent, bool Searched, bool childSearched) : entity(ent), res((Searched&& childSearched) ? searchRes::bothSearched : ((Searched) ? searchRes::thisSearched : searchRes::ChildSearched))
		{
			childInfo = std::vector<EntityDisplayInfo>();
		};
		EntityDisplayInfo(Entity ent, searchRes resa) : entity(ent), res(resa) {
			childInfo = std::vector<EntityDisplayInfo>();
		};
		Entity entity;
		searchRes res;
		std::vector<EntityDisplayInfo> childInfo;
	};
	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();
		void Update(float dt);

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; SetPreviewAbsoluteImagePath(""); }
		void SetPreviewAbsoluteImagePath(std::filesystem::path path) { m_PreviewAbsoluteImagePath = path; m_ReevaluatePreview = true; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
		void DrawEntityNode(EntityDisplayInfo entityDisplayInfo);
		void DrawComponents(Entity entity);
		void Search();
		std::optional<EntityDisplayInfo> SearchEntity(Entity entity);
		template<typename T, typename UIFunction>
		void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, Ref<Scene>& context, Icons icon);
	private:
		std::string m_Search;
		Ref<ImagePanel> m_ImagePanel;
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::filesystem::path m_PreviewAbsoluteImagePath;
		bool m_ReevaluatePreview = true;
		std::vector<EntityDisplayInfo> m_ListOfEntityDisplayed;
		bool m_FirstDrawAfterSearch;
		std::vector<Ref<Animation>> m_PreviewedAnimations;
	};
}