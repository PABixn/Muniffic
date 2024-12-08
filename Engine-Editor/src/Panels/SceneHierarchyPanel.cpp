#include "egpch.h"
#include "SceneHierarchyPanel.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/UI/UI.h"
#include "Engine/Imgui/ImguiLayer.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Imgui/imgui_internal.h>
#include <cstring>
#include "../Commands/Commands.h"
#include <imgui/misc/cpp/imgui_stdlib.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "iostream"
#include "shellapi.h"
#include "functional"
#include "ConsolePanel.h"
#include "../EditorActions.h"

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace eg
{

	static const float WidthOfProperty = 0.75f;

	const char *add(const char *begining, const char *middle, const char *ending)
	{
        EG_PROFILE_FUNCTION();
		size_t resultLength = strlen(middle) + strlen(begining) + strlen(ending);
		char *resultMsg = new char[resultLength];
		strcpy(resultMsg, begining);
		strcat(resultMsg, middle);
		strcat(resultMsg, ending);
		return resultMsg;
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> &scene)
	{
        EG_PROFILE_FUNCTION();
		SetContext(scene);
		
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> &scene)
	{
        EG_PROFILE_FUNCTION();
		m_Context = scene;
		EditorActions::SetScene(scene);
		m_SelectionContext = {};
		EditorActions::SetSelectionContext(&m_SelectionContext);
		m_ImagePanel = CreateRef<ImagePanel>();
		m_PropertiesPanel = CreateRef<EntityPropertiesPanel>();
		m_ListOfEntityDisplayed = std::vector<EntityDisplayInfo>();
		Commands::s_SceneHierarchyPanelPtr = this;
		Search();
	}

	std::optional<EntityDisplayInfo> SceneHierarchyPanel::SearchEntity(Entity entity)
	{
        EG_PROFILE_FUNCTION();
		bool searched = false, childsearched = false;
		EntityDisplayInfo currentEntityDisplayInfo = EntityDisplayInfo();
		Ref<std::vector<Entity>> children = entity.GetChildren();
		for (Entity e : *children)
		{
			std::optional<EntityDisplayInfo> s = SearchEntity(e);
			if (s.has_value())
			{
				childsearched = true;
				currentEntityDisplayInfo.entity = entity;
				currentEntityDisplayInfo.childInfo.push_back(s.value());
			}
		}
		if (entity.GetName().contains(m_Search))
		{
			currentEntityDisplayInfo.entity = entity;
			searched = true;
		}
		if (childsearched || searched)
		{
			currentEntityDisplayInfo.res = (searched && childsearched) ? searchRes::bothSearched : ((searched) ? searchRes::thisSearched : searchRes::ChildSearched);
			return currentEntityDisplayInfo;
		}
		return {};
	}

	void SceneHierarchyPanel::Search()
	{
		EG_PROFILE_FUNCTION();
		m_ListOfEntityDisplayed.clear();
		m_Context->m_Registry.each([&](auto entityID)
								   {
			Entity entity{ entityID, m_Context.get() };
			if (entity.Exists())
			{
				if (entity.GetParent() == std::nullopt)
				{
					if (m_Search != "")
					{
						std::optional<EntityDisplayInfo> found = SearchEntity(entity);
						if (found.has_value()) {
							m_ListOfEntityDisplayed.push_back(found.value());
						}
					}
					else
					{
						m_ListOfEntityDisplayed.push_back(EntityDisplayInfo(entity, searchRes::thisSearched));
					}

				}
			}
			});
		m_FirstDrawAfterSearch = true;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		EG_PROFILE_FUNCTION();
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Scene Hierarchy");
		ImGui::PopStyleVar();
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		float paddingTop = 10.f;
		float paddingLeft = 15.f;
		float rightAndLeftFreeSpace = 110.f;
		ImGui::SetCursorPosX(rightAndLeftFreeSpace / 4 - paddingLeft + 3.f);
		ImGui::SetCursorPosY(45.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingLeft / 2 + 3.f, paddingTop / 2));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30.f);
		if (ImGui::Button("+"))
		{
			Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs("Empty Entity", {}, m_Context, m_SelectionContext));
			Search();
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		// search bar
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingLeft, paddingTop));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30.f);
		ImGui::PushItemWidth(ImGui::GetWindowWidth() - rightAndLeftFreeSpace);
		ImGui::SetCursorPosX(rightAndLeftFreeSpace / 2);
		ImGui::SetCursorPosY(40.f);

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 padd = ImGui::GetStyle().FramePadding; 
		ImVec2 inputSize = ImVec2(300, 30);

		Search();

		if (ImGui::InputText("##entitySearch", &m_Search) || ImGui::GetFrameCount() == 2) {
			Search();
		}

		bool isHovered = ImGui::IsItemHovered();
		bool isActive = ImGui::IsItemActive();

		if (isHovered || isActive) {
			ImVec4 hoverColor = isHovered || isActive ? ImVec4(
				ImGuiLayer::m_LightShade.x + (1.0f - ImGuiLayer::m_LightShade.x) * 0.225f,
				ImGuiLayer::m_LightShade.y + (1.0f - ImGuiLayer::m_LightShade.y) * 0.225f,
				ImGuiLayer::m_LightShade.z + (1.0f - ImGuiLayer::m_LightShade.z) * 0.225f,
				1.0f
			) : ImVec4(0, 0, 0, 0);
			ImColor borderColor = ImColor(hoverColor);

			float totalWidth = ImGui::CalcItemWidth();

			drawList->AddRect(pos, ImVec2(pos.x + totalWidth, pos.y + inputSize.y + padd.y), borderColor, 30.0f, 0, 2.0f);
		}
		for (EntityDisplayInfo e : m_ListOfEntityDisplayed) {
			DrawEntityNode(e);
		}
		if (m_FirstDrawAfterSearch)
			m_FirstDrawAfterSearch = false;
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		if (m_Search == "")
		{
			ImGui::SetCursorPosX((rightAndLeftFreeSpace / 2) + paddingLeft);
			ImGui::SetCursorPosY(40.f + paddingTop);
			ImGui::Text("search");
			ImGui::NewLine();
		}
		if (m_Context)
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("Entity"))
				{
					Entity draggedEntity = *(Entity *)payload->Data;
					Commands::ExecuteChangeParentCommand(draggedEntity, std::nullopt, m_Context);
				}

				ImGui::EndDragDropTarget();
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered() || !m_Context->GetRegistry().valid(m_SelectionContext))
			{
				m_SelectionContext = {};
			}

			// Right click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs("Empty Entity", {}, m_Context, m_SelectionContext));
					Search();
				}
				ImGui::EndPopup();
			}
		}

		if (m_SelectionContext) {
			m_PropertiesPanel->SetEntity(m_SelectionContext);
			m_PropertiesPanel->SetScene(m_Context);
		}
		if (m_PreviewAbsoluteImagePath != "")
		{
			ImGui::Begin("Preview");
			static GLuint my_opengl_texture;
			static unsigned char *image = nullptr;
			for (const std::pair<UUID, TextureResourceData *> &pairOfUUIDAndData : ResourceDatabase::GetTextureResourceDataCache())
			{
				auto CacheImageData = (pairOfUUIDAndData.second);
				if (ResourceDatabase::GetResourcePath(pairOfUUIDAndData.first) == m_PreviewAbsoluteImagePath)
				{
					if (m_ReevaluatePreview)
					{
						if (image)
						{
							stbi_image_free(image);
						}
						image = stbi_load(m_PreviewAbsoluteImagePath.string().c_str(), &(CacheImageData->Width), &(CacheImageData->Height), &(CacheImageData->Channels), STBI_rgb_alpha);
						if (image)
						{
							glGenTextures(1, &my_opengl_texture);
							glBindTexture(GL_TEXTURE_2D, my_opengl_texture);
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (CacheImageData->Width), (CacheImageData->Height), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
							glGenerateMipmap(GL_TEXTURE_2D);
						}
						m_ReevaluatePreview = false;
					}
					if (image)
					{
						auto s = ImGui::GetWindowWidth();
						std::string absImgName = m_PreviewAbsoluteImagePath.filename().string();
						ImGui::TextWrapped(add("file: ", absImgName.c_str(), ""));
						if (ImGui::Button("open"))
						{
							std::string path = m_PreviewAbsoluteImagePath.string();
							ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
						}
						ImGui::TextWrapped(add("size: ", std::to_string(CacheImageData->Width).c_str(), add(" x ", std::to_string(CacheImageData->Height).c_str(), "")));
						ImGui::TextWrapped(add("scale of preview: ", (std::to_string(std::round(s / CacheImageData->Width * 100.0) / 100.0)).substr(0, 4).c_str(), ""));
						ImGui::Image((void *)(intptr_t)my_opengl_texture, ImVec2((int)s, (int)((s * (CacheImageData->Height)) / (CacheImageData->Width))));
						// stbi_image_free(image);
					}
					else
					{
						std::string absImgPath = m_PreviewAbsoluteImagePath.string();
						ImGui::TextWrapped(add("file: ", absImgPath.c_str(), " not found"));
					}
					ImGui::End();
					ImGui::End();
					return;
				}
			}
			ImGui::TextWrapped("The file you are trying to access is not inside the project cache");
			if (ImGui::Button("add this resource to project cache"))
			{
				bool initialized = m_ImagePanel->OpenImagePanel(m_PreviewAbsoluteImagePath);
				if (initialized)
					m_ImagePanel->ShowImagePanel(true);
			}
			m_ImagePanel->OnImGuiRender();
			ImGui::End();
		}
		ImGui::End();
		m_PropertiesPanel->OnImGuiRender();
	}

	void SceneHierarchyPanel::Update(float dt)
	{
		EG_PROFILE_FUNCTION();
		if (m_SelectionContext)
		{
			if (m_SelectionContext.HasComponent<AnimatorComponent>())
			{
				if (!m_Context->IsRunning())
				{
					for (Ref<Animation> an : m_PreviewedAnimations)
					{
						an->Update(dt, m_SelectionContext.GetComponent<AnimatorComponent>().Animator2D->GetSpeed());
					}
				}
			}
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(EntityDisplayInfo entityDisplayInfo)
	{
		EG_PROFILE_FUNCTION();
		Entity entity = entityDisplayInfo.entity;

		if (!entity.IsDrawable())
		{
			Search();
			return;
		}
		auto &tag = entity.GetComponent<TagComponent>().Tag;
		bool opened = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Entity | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanFullWidth /* = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow*/;
		if (entity.GetChildCount() != 0)
		{
			flags |= ImGuiTreeNodeFlags_EntityWithChildren;
		}
		if (m_FirstDrawAfterSearch && (entityDisplayInfo.res == searchRes::ChildSearched || entityDisplayInfo.res == searchRes::bothSearched))
		{
			flags |= ImGuiTreeNodeFlags_ForceOpen;
		}
		if (m_FirstDrawAfterSearch && entityDisplayInfo.res == searchRes::thisSearched)
		{
			flags |= ImGuiTreeNodeFlags_ForceClose;
		}
		if (entityDisplayInfo.entity == this->GetSelectedEntity())
		{
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		// flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 5.f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImGuiLayer::m_NormalShade);
		ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImGuiLayer::m_NormalShade);
		opened = ImGui::CustomTreeNodeEx((void *)(int64_t)entity.GetUUID(), (ImTextureID)IconLoader::GetIcon(Icons::Component_Puzzle)->GetRendererID(), flags, tag.c_str());
		ImGui::PopStyleColor(2);
		ImGui::PopStyleVar();
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("Entity", &entity, sizeof(Entity));
			ImGui::Text(tag.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("Entity"))
			{
				Entity draggedEntity = *(Entity *)payload->Data;
				Commands::ExecuteChangeParentCommand(draggedEntity, entity, m_Context);
				Search();
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
		{
			SetSelectedEntity(entity);
			if (ImGui::FindWindowByName("Properties") != NULL)
			{
				if (ImGui::FindWindowByName("Properties")->DockIsActive)
				{
					ImGuiTabBar* tabbar = ImGui::FindWindowByName("Properties")->DockNode->TabBar;
					ImGui::TabBarQueueFocus(tabbar, ImGui::TabBarFindTabByID(tabbar, ImGui::FindWindowByName("Properties")->TabId));
				}
			}
		}

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", entity, m_Context, m_SelectionContext));

			if (ImGui::MenuItem("Create Child Entity"))
				Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs("Empty Child Entity", {}, m_Context, m_SelectionContext, entity));

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (entityDisplayInfo.res >= searchRes::thisSearched)
			{
				if (entity.Exists())
				{
					Ref<std::vector<Entity>> children = entity.GetChildren();
					for (Entity child : *children)
					{
						DrawEntityNode(child);
					}
				}
			}
			else
			{
				for (EntityDisplayInfo edi : entityDisplayInfo.childInfo)
				{
					DrawEntityNode(edi);
				}
			}
			ImGui::TreePop();
		}
	}
}