#pragma once
#include "Engine.h"
#include "ResourcesPanels/ImagePanel.h"
//#include "ConsolePanel.h"

namespace eg {

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
		void SetPreviewAbsoluteImagePath(std::filesystem::path path) { m_PreviewAbsoluteImagePath = path; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
		void DrawEntityNode(EntityDisplayInfo entityDisplayInfo);
		void DrawComponents(Entity entity);
		void Search();
		std::optional<EntityDisplayInfo> SearchEntity(Entity entity);
	private:
		std::string m_Search;
		Ref<ImagePanel> m_ImagePanel;
		Ref<Scene> m_Context;
		Ref<Texture2D> m_PuzzleIcon;
		Entity m_SelectionContext;
		std::filesystem::path m_PreviewAbsoluteImagePath;
		std::vector<EntityDisplayInfo> m_ListOfEntityDisplayed;
		bool m_FirstDrawAfterSearch;
	};
}