#pragma once
#include "Engine.h"
//#include "ConsolePanel.h"

namespace eg {

	class SceneHierarchyPanel {
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
		void Update(float dt);

		Entity GetSelectedEntity() const { return m_SelectionContext; }
		void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; SetPreviewAbsoluteImagePath(""); SetPreviewRelativeImagePath(""); }
		void SetPreviewAbsoluteImagePath(std::filesystem::path path) { m_PreviewAbsoluteImagePath = path; }
		void SetPreviewRelativeImagePath(std::filesystem::path path) { m_PreviewRelativeImagePath = path; }
	private:
		template<typename T>
		void DisplayAddComponentEntry(const std::string& entryName);
		void DrawEntityNode(Entity entity, bool forceDraw = false);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
		std::filesystem::path m_PreviewAbsoluteImagePath;
		std::filesystem::path m_PreviewRelativeImagePath;
	};
}