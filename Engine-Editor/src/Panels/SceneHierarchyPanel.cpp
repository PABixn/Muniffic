#include "egpch.h"
#include "SceneHierarchyPanel.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/UI/UI.h"

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

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace eg {

	static const float WidthOfProperty = 0.75f;

	const char* add(const char* begining, const char* middle, const char* ending) {
		size_t resultLength = strlen(middle) + strlen(begining) + strlen(ending);
		char* resultMsg = new char[resultLength];
		strcpy(resultMsg, begining);
		strcat(resultMsg, middle);
		strcat(resultMsg, ending);
		return resultMsg;
	}

	float CalculatePreferedComponentPropertyWidth(const char* label) {
		float WidthOfLabel = ImGui::CalcTextSize(label).x;
		if (WidthOfLabel > ImGui::GetContentRegionAvail().x * (1.f - WidthOfProperty) - 4.f)
		{
			if (WidthOfLabel > ImGui::GetContentRegionAvail().x * .5f - 4.f)
			{
				return ImGui::GetContentRegionAvail().x * .5f - 14.f;
			}
			else
			{
				return ImGui::GetContentRegionAvail().x - WidthOfLabel - 14.f;
			}
		}
		else
		{
			return ImGui::GetContentRegionAvail().x * WidthOfProperty - 4.f;
		}
	}
	
	static void PropertyLabel(const char* label) {
		ImGui::SameLine();
		bool s = ImGui::TextWrappedWithLineLimit(label, 1);

		if (s && ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(100.f);
			ImGui::TextWrapped(label);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();

		}
	}

	static bool PrettyButton(const char* label, bool fullWidth = false) {
		if (fullWidth)
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(50, 7));
		else 
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 7));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
		float size = ImGui::CalcTextSize(label).x + ImGui::GetStyle().FramePadding.x * 2.0f + 20;
		float avail = ImGui::GetContentRegionAvail().x;

		float off = (avail - size) * 0.5f;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
		bool OpenPopup = ImGui::Button(label);
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		return OpenPopup;
	}

	void ComponentPropertyBeforeDraw(const char* label) {
		ImGui::PushID(label);
		ImGui::PushItemWidth(CalculatePreferedComponentPropertyWidth(label));
	}
	void ComponentPropertyAfterDraw(const char* label) {
		ImGui::PopItemWidth();
		PropertyLabel(label);
		ImGui::PopID();

	}

	static void DrawVec3Control(Entity entity, const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f, bool firstValue = false)
	{
		float x = values.x, y = values.y, z = values.z;
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		float columnsWidth = ImGui::GetContentRegionAvail().x * WidthOfProperty-68.f;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		ImGui::PushMultiItemsWidths(3, columnsWidth);
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight - 1.f, lineHeight };
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.65f,0.15f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.76f,0.23f,0.23f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.42f,0.1f,0.1f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::StylisedButton("X", buttonSize, ImDrawFlags_RoundCornersLeft))
		{
			float check = values.x;
			values.x = resetValue;
			Commands::ExecuteRawValueCommand<float, TransformComponent>(&values.x, x, entity, label + std::string("##X"), check != 0 ? true : false);
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();

		if (ImGui::DragFloatRounded("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f", 0, ImDrawFlags_RoundCornersRight))
			Commands::ExecuteRawValueCommand<float, TransformComponent>(&values.x, x, entity, label + std::string("##X"));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.65f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.0f,0.5f,0.0f,1.0f });
		ImGui::PushFont(boldFont);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3.f);
		if (ImGui::StylisedButton("Y", buttonSize, ImDrawFlags_RoundCornersLeft))
		{
			float check = values.y;
			values.y = resetValue;
			Commands::ExecuteRawValueCommand<float, TransformComponent>(&values.y, y, entity, label + std::string("##Y"), check != 0 ? true : false);
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (ImGui::DragFloatRounded("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f", 0, ImDrawFlags_RoundCornersRight))
			Commands::ExecuteRawValueCommand<float, TransformComponent>(&values.y, y, entity, label + std::string("##Y"));
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.65f, 0.06f,0.83f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.f, 0.45f,1.f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.f,0.8f,1.0f });
		ImGui::PushFont(boldFont);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3.f);
		if (ImGui::StylisedButton("Z", buttonSize, ImDrawFlags_RoundCornersLeft))
		{
			float check = values.z;
			values.z = resetValue;
			Commands::ExecuteRawValueCommand<float, TransformComponent>(&values.z, z, entity, label + std::string("##Z"), check != 0 ? true : false);
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if (ImGui::DragFloatRounded("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f", 0, ImDrawFlags_RoundCornersRight))
			Commands::ExecuteRawValueCommand<float, TransformComponent>(&values.z, z, entity, label + std::string("##Z"));
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		PropertyLabel(label.c_str());
		ImGui::PopID();
	}

	static bool DrawComponentPropertyFloat(const char* label, float* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f") {

		ComponentPropertyBeforeDraw(label);
		bool changed_value = ImGui::DragFloat("", value, speed, min, max, format);
		ComponentPropertyAfterDraw(label);
		return changed_value;
	}

	static bool DrawComponentPropertyFloat2(const char* label, float value[2], float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f") {

		ComponentPropertyBeforeDraw(label);
		bool changed_value = ImGui::DragFloat2("", value, speed, min, max, format);
		ComponentPropertyAfterDraw(label);
		return changed_value;
	}

	static bool DrawComponentPropertyFloat3(const char* label, float value[3], float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f") {

		ComponentPropertyBeforeDraw(label);
		bool changed_value = ImGui::DragFloat3("", value, speed, min, max, format);
		ComponentPropertyAfterDraw(label);
		return changed_value;
	}

	static bool DrawComponentPropertyFloat4(const char* label, float value[4], float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f") {

		ComponentPropertyBeforeDraw(label);
		bool changed_value = ImGui::DragFloat4("", value, speed, min, max, format);
		ComponentPropertyAfterDraw(label);
		return changed_value;
	}

	static bool DrawComponentPropertyInt(const char* label, int* value, float speed = 1.0f, float min = 0.0f, float max = 0.0f, const char* format = "%.3f") {

		ComponentPropertyBeforeDraw(label);
		bool changed_value = ImGui::DragInt("", value, speed, min, max, format);
		ComponentPropertyAfterDraw(label);
		return changed_value;
	}

	static bool DrawComponentPropertyColorEdit4(const char* label, float col[4]) {

		ComponentPropertyBeforeDraw(label);
		bool changed_value = ImGui::ColorEdit4("", col);
		ComponentPropertyAfterDraw(label);
		return changed_value;
	}

	static bool DrawComponentPropertyInputText(const char* label, char* buf, size_t buf_size) {
		ComponentPropertyBeforeDraw(label);
		bool changed = ImGui::InputText("", buf, buf_size);
		ComponentPropertyAfterDraw(label);
		return changed;
	}

	static bool DrawComponentPropertyInputText(const char* label, std::string* val) {
		ComponentPropertyBeforeDraw(label);
		bool changed = ImGui::InputText("", val);
		ComponentPropertyAfterDraw(label);
		return changed;
	}

	static void DrawComponentPropertyFileReference(const char* label, UUID& uuid, std::function<void(int64_t* what)> fun) {
		ComponentPropertyBeforeDraw(label);
		ImGui::Button((uuid == 0)? "none": ResourceDatabase::GetResourceName(uuid).c_str(), {CalculatePreferedComponentPropertyWidth(label), 0});
		if (ImGui::BeginDragDropTarget())
		{

			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
			{
				fun((int64_t*)payload->Data);
			}
			ImGui::EndDragDropTarget();
		}
		ComponentPropertyAfterDraw(label);
	}

	static bool DrawComponentPropertyCheckbox(const char* label, bool* CheckedValue) {
		ImGui::PushID(label);
		bool changed_value = ImGui::Checkbox("", CheckedValue);
		PropertyLabel(label);
		ImGui::PopID();
		return changed_value;
	}

	static void DrawComponentPropertyCombo(const char* label, std::vector<const char*> possiblevalues, int currentSelected, std::function<void(int number)> fun) {
		ImGui::PushID(label);
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * WidthOfProperty - 4.f);
		char* currentValueString = (char*)possiblevalues[currentSelected];
		if (ImGui::BeginCombo("", currentValueString))
		{
			for (int i = 0; i < possiblevalues.size(); i++)
			{
				bool isSelected = possiblevalues[currentSelected] == possiblevalues[i];
				if (ImGui::Selectable(possiblevalues[i], isSelected))
				{
					currentValueString = (char*)possiblevalues[i];
					fun(i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		PropertyLabel(label);
		ImGui::PopID();
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		//See ComponentIcon enum (SceneHierarchyPanel.h)
		m_ComponentIcons = std::vector<Ref<Texture2D>>();
		std::string IconPath = "resources/icons/hierarchyPanel/Components/";
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"ComponentIcon.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"Transform.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"Camera.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"Script.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"SpriteRen.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"SubTexRen.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"CircleRen.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"RigidBody.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"BoxCol.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"CircleCol.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"TextRen.png"));
		m_ComponentIcons.push_back(Texture2D::Create(IconPath+"Animator.png"));
		m_Context = scene;
		m_SelectionContext = {};
		m_ImagePanel = CreateRef<ImagePanel>();
		m_ListOfEntityDisplayed = std::vector<EntityDisplayInfo>();
		Search();
		m_PuzzleIcon = Texture2D::Create("resources/icons/hierarchyPanel/puzzle_rotated.png");
	}

	std::optional<EntityDisplayInfo> SceneHierarchyPanel::SearchEntity(Entity entity) {                                 
		bool searched = false, childsearched = false;
		std::string silli = entity.GetName();
		EntityDisplayInfo currentEntityDisplayInfo = EntityDisplayInfo();
		for (Entity e : entity.GetChildren()) {
			std::optional<EntityDisplayInfo> s = SearchEntity(e);
			if (s.has_value()) {
				childsearched = true;
				currentEntityDisplayInfo.entity = entity;
				currentEntityDisplayInfo.childInfo.push_back(s.value());
			}
		}
		if (entity.GetName().contains(m_Search)) {
			currentEntityDisplayInfo.entity = entity;
			searched = true;
		}
		if (childsearched || searched) {
			currentEntityDisplayInfo.res = (searched && childsearched) ? searchRes::bothSearched : ((searched) ? searchRes::thisSearched : searchRes::ChildSearched);
			return currentEntityDisplayInfo;
		}
		return {};
	}

	void SceneHierarchyPanel::Search()
	{
		m_ListOfEntityDisplayed.clear();
		m_Context->m_Registry.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			if (entity.GetParent() == std::nullopt) {
				std::string silli = entity.GetName();
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
		});
		m_FirstDrawAfterSearch = true;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		ImGui::Begin("Scene Hierarchy");
		ImGui::PopStyleVar();

		float paddingTop = 10.f;
		float paddingLeft = 15.f;
		float rightAndLeftFreeSpace = 110.f;
		ImGui::SetCursorPosX(rightAndLeftFreeSpace / 4 - paddingLeft +3.f);
		ImGui::SetCursorPosY(45.f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingLeft/2 +3.f, paddingTop/2));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30.f);
		if (ImGui::Button("+")) {
			Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs("Empty Entity", {}, m_Context, m_SelectionContext));
			Search();
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		//search bar
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(paddingLeft, paddingTop));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 30.f);
		ImGui::PushItemWidth(ImGui::GetWindowWidth()-rightAndLeftFreeSpace);
		ImGui::SetCursorPosX(rightAndLeftFreeSpace/2);
		ImGui::SetCursorPosY(40.f);
		if (ImGui::InputText("##entitySearch", &m_Search) || ImGui::GetFrameCount()==2) {
			Search();
		}
		for (EntityDisplayInfo e : m_ListOfEntityDisplayed) {
			DrawEntityNode(e);
		}
		if (m_FirstDrawAfterSearch)
			m_FirstDrawAfterSearch = false;
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		if (m_Search == "") {
			ImGui::SetCursorPosX((rightAndLeftFreeSpace/2)+paddingLeft);
			ImGui::SetCursorPosY(40.f+paddingTop);
			ImGui::Text("search");
			ImGui::NewLine();
		}
		if (m_Context)
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
				{
					Entity draggedEntity = *(Entity*)payload->Data;
					Commands::ExecuteChangeParentCommand(draggedEntity, std::nullopt, m_Context);
				}

				ImGui::EndDragDropTarget();
			}

			
			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
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
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f)); 
		ImGui::Begin("Properties");
		ImGui::PopStyleVar();
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();
		if (m_PreviewAbsoluteImagePath != "" ) {
			ImGui::Begin("Preview");
			static GLuint my_opengl_texture; 
			static unsigned char* image = nullptr;
			for (const std::pair<UUID, TextureResourceData*>& pairOfUUIDAndData : ResourceDatabase::GetTextureResourceDataCache()) {
				auto CacheImageData = (pairOfUUIDAndData.second);
				if (ResourceDatabase::GetResourcePath(pairOfUUIDAndData.first) == m_PreviewAbsoluteImagePath) {
					if (m_ReevaluatePreview)
					{
						if (image)
						{
							stbi_image_free(image);
						}
						image = stbi_load(m_PreviewAbsoluteImagePath.string().c_str(), &(CacheImageData->Width), &(CacheImageData->Height), &(CacheImageData->Channels), STBI_rgb_alpha);
						if (image) {
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
						if (ImGui::Button("open")) { 
							std::string path = m_PreviewAbsoluteImagePath.string(); 
							ShellExecuteA(NULL, "open", path.c_str(), NULL, NULL, SW_SHOWDEFAULT); 
						}
						ImGui::TextWrapped(add("size: ", std::to_string(CacheImageData->Width).c_str(), add(" x ", std::to_string(CacheImageData->Height).c_str(), "")));
						ImGui::TextWrapped(add("scale of preview: ", (std::to_string(std::round(s / CacheImageData->Width * 100.0) / 100.0)).substr(0, 4).c_str(), ""));
						ImGui::Image((void*)(intptr_t)my_opengl_texture, ImVec2((int)s, (int)((s * (CacheImageData->Height)) / (CacheImageData->Width))));
						//stbi_image_free(image);
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
	}

	void SceneHierarchyPanel::Update(float dt)
	{
		if (m_SelectionContext)
		{
			if (m_SelectionContext.HasComponent<AnimatorComponent>())
			{
				if (!m_Context->IsRunning()){
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
		Entity entity = entityDisplayInfo.entity;

		if (!entity.IsDrawable()) {
			Search();
			return;
		}
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		bool opened = false;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Entity | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanFullWidth/* = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)| ImGuiTreeNodeFlags_OpenOnArrow*/;
		if (entity.GetChildren().size() != 0) {
			flags |= ImGuiTreeNodeFlags_EntityWithChildren;
		}
		if (m_FirstDrawAfterSearch && (entityDisplayInfo.res == searchRes::ChildSearched || entityDisplayInfo.res == searchRes::bothSearched)) {
			flags |= ImGuiTreeNodeFlags_ForceOpen;
		}
		if (m_FirstDrawAfterSearch && entityDisplayInfo.res == searchRes::thisSearched) {
			flags |= ImGuiTreeNodeFlags_ForceClose;
		}
		if (entityDisplayInfo.entity == this->GetSelectedEntity()) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}
		//flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(3.f, 5.f));
		opened = ImGui::CustomTreeNodeEx((void*)(uint64_t)entity.GetUUID(), flags, tag.c_str());
		ImGui::PopStyleVar();
		if (ImGui::BeginDragDropSource())
		{
			ImGui::SetDragDropPayload("Entity", &entity, sizeof(Entity));
			ImGui::Text(tag.c_str());
			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity"))
			{
				Entity draggedEntity = *(Entity*)payload->Data;
				Commands::ExecuteChangeParentCommand(draggedEntity, entity, m_Context);
				Search();
			}

			ImGui::EndDragDropTarget();
		}

		if (ImGui::IsItemClicked())
			SetSelectedEntity(entity);

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", entity, m_Context, m_SelectionContext));

			if(ImGui::MenuItem("Create Child Entity"))
				Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs("Empty Child Entity", {}, m_Context, m_SelectionContext, entity));

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (entityDisplayInfo.res>=searchRes::thisSearched)
			{
				if (entity.Exists())
				{
					for (Entity& child : entity.GetChildren()) {
						DrawEntityNode(child);
					}
				}
			}
			else {
				for (EntityDisplayInfo edi : entityDisplayInfo.childInfo) {
					DrawEntityNode(edi);
				}
			}
			/*
			if (entity.Exists())
			{
				for (Entity& child : entity.GetChildren()) {
					DrawEntityNode(child);
				}
			}
			*/
			ImGui::TreePop();
		}
	}
	template<typename T, typename UIFunction>
	void SceneHierarchyPanel::DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, Ref<Scene>& context, int iconOrType)
	{
		static const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_NoArrow | ImGuiTreeNodeFlags_PropertiesComponent;
		if (entity.HasComponent<T>())
		{
			std::string ajDzik(entity.GetName() + std::to_string(iconOrType));
			ImGui::PushID(ajDzik.c_str());
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			//ImGui::Separator();
			Ref<Texture2D> iconID = m_ComponentIcons[iconOrType];

			bool open;
			if(entity.GetChildren().size() > 0 && entity.GetInheritableComponent<T>()->isInheritedInChildren)
				open = ImGui::CustomTreeNodeWithPicEx((void*)typeid(T).hash_code(), (treeNodeFlags| ImGuiTreeNodeFlags_CopyingToChildren), name.c_str(), (ImTextureID)(iconID->GetRendererID()));
			else	
				open = ImGui::CustomTreeNodeWithPicEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str(), (ImTextureID)(iconID->GetRendererID()));
			ImGui::PopStyleVar();
			if (!(name == std::string("Transform") && !(entity.GetChildren().size() > 0 || entity.GetParent().has_value()))) {
				ImGui::SameLine(contentRegionAvailable.x - lineHeight);
				if (ImGui::Button("...", ImVec2{ lineHeight,lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettings");
				}
			}

			
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (name != std::string("Transform"))
				{
					if (ImGui::MenuItem("Remove Component"))
						Commands::ExecuteCommand<Commands::RemoveComponentCommand<T>>(Commands::CommandArgs("", entity, context, entity));
				}

				if (entity.HasComponent<T>())
				{
					if (entity.GetChildren().size() > 0)
					{
						if (ImGui::MenuItem(entity.GetInheritableComponent<T>()->isInheritedInChildren == false ?
							"Inherit component in children" :
							"Stop inheriting component in children"))
							Commands::ExecuteInheritComponentCommand<T>(entity, context, entity.GetInheritableComponent<T>()->isInheritedInChildren);

						if(ImGui::MenuItem("Copy to children"))
							Commands::ExecuteManageComponentInheritanceCommand<T>(entity, context, Commands::InheritanceCommandType::COPY_COMPONENT);

						if(ImGui::MenuItem("Remove from children"))
							Commands::ExecuteManageComponentInheritanceCommand<T>(entity, context, Commands::InheritanceCommandType::COPY_COMPONENT, true);

						if(ImGui::MenuItem("Copy values to children"))
							Commands::ExecuteManageComponentInheritanceCommand<T>(entity, context, Commands::InheritanceCommandType::COPY_COMPONENT_VALUES);

						if(ImGui::MenuItem("Copy component with values to children"))
							Commands::ExecuteManageComponentInheritanceCommand<T>(entity, context, Commands::InheritanceCommandType::COPY_COMPONENT_AND_VALUES);
					}

					if (entity.GetParent().has_value())
					{
						if (ImGui::MenuItem(entity.GetInheritableComponent<T>()->isInherited == false ?
							"Inherit from parent" :
							"Stop inheriting from parent"))
							Commands::ExecuteInheritComponentCommand<T>(entity, context, entity.GetInheritableComponent<T>()->isInherited, true);
					}
				}
					
				ImGui::EndPopup();
			}
			

			if (open)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
				uiFunction(component);
				ImGui::TreePop();
				ImGui::PopStyleVar();
			}
			ImGui::PopID();
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				Commands::ExecuteCommand<Commands::AddComponentCommand<T>>(Commands::CommandArgs("", m_SelectionContext, m_Context, m_SelectionContext));
				ImGui::CloseCurrentPopup();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10,7));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			ImGui::SetCursorPos({ImGui::GetCursorPosX() + 40, ImGui::GetCursorPosY()+20});
			ImGui::Image((ImTextureID)m_PuzzleIcon->GetRendererID(), {30,30});
			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 120);
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX()+10);
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer)); 
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer), ImGuiInputTextFlags_Wrapped))
			{
				tag = std::string(buffer);
			}
			ImGui::PopItemWidth();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();

		}

		ImGui::PushItemWidth(-1);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 7);
		
		if (PrettyButton("Add Component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<ScriptComponent>("Script");
			DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
			DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
			DisplayAddComponentEntry<RigidBody2DComponent>("Rigidbody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");
			DisplayAddComponentEntry<TextComponent>("Text Component");
			DisplayAddComponentEntry<AudioSourceComponent>("Audio Source");
			DisplayAddComponentEntry<SpriteRendererSTComponent>("SubTexture Sprite Renderer 2D");
			DisplayAddComponentEntry<AnimatorComponent>("Animator");

			ImGui::EndPopup();
		}
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [entity](auto& component)
			{
				DrawVec3Control(entity, "Translation", component.Translation, 0.0f, 100.f, true);
				DrawVec3Control(entity, "Rotation", component.Rotation);
				DrawVec3Control(entity, "Scale", component.Scale, 1.0f);
			}, m_Context, ComponentIcons::TransformIcon);

		DrawComponent<CameraComponent>("Camera", entity, [entity](auto& component) {
			auto& camera = component.Camera;

			if (DrawComponentPropertyCheckbox("Primary", &component.Primary))
				Commands::ExecuteRawValueCommand<bool>(&component.Primary, !component.Primary, "CameraComponent-Primary");

			const char* projectionTypeString[] = { "Perspective","Orthographic" };
			DrawComponentPropertyCombo("Projection", std::vector<const char*>(projectionTypeString, projectionTypeString + sizeof projectionTypeString / sizeof projectionTypeString[0]), (int)camera.GetProjectionType(), [&camera](int number) {
				Commands::ExecuteValueCommand<SceneCamera::ProjectionType>([&camera](SceneCamera::ProjectionType projectionType)
				{
					camera.SetProjectionType(projectionType);
				}, (SceneCamera::ProjectionType)number, camera.GetProjectionType(), "CameraComponent-ProjectionType", true);
			});

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (DrawComponentPropertyFloat("Vertical FOV", &perspectiveVerticalFov))
					//camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));
					Commands::ExecuteValueCommand<float>([&camera](float verticalFov)
						{
							camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));
						}, perspectiveVerticalFov, glm::degrees(camera.GetPerspectiveVerticalFOV()), "CameraComponent-Vertical FOV");

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (DrawComponentPropertyFloat("Near", &perspectiveNear))
					Commands::ExecuteValueCommand<float>([&camera](float nearClip)
						{
							camera.SetPerspectiveNearClip(nearClip);
						}, perspectiveNear, camera.GetPerspectiveNearClip(), "CameraComponent-Near");

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (DrawComponentPropertyFloat("Far", &perspectiveFar))
					Commands::ExecuteValueCommand<float>([&camera](float farClip)
						{
							camera.SetPerspectiveFarClip(farClip);
						}, perspectiveFar, camera.GetPerspectiveFarClip(), "CameraComponent-Far");
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float size = camera.GetOrthographicSize();
				if (DrawComponentPropertyFloat("Size", &size))
					Commands::ExecuteValueCommand<float>([&camera](float size)
						{
							camera.SetOrthographicSize(size);
						}, size, camera.GetOrthographicSize(), "CameraComponent-Size");

				float nearClip = camera.GetOrthographicNearClip();
				if (DrawComponentPropertyFloat("Near", &nearClip))
					Commands::ExecuteValueCommand<float>([&camera](float nearClip)
						{
							camera.SetOrthographicNearClip(nearClip);
						}, nearClip, camera.GetOrthographicNearClip(), "CameraComponent-Near");

				float farClip = camera.GetOrthographicFarClip();
				if (DrawComponentPropertyFloat("Far", &farClip))
					Commands::ExecuteValueCommand<float>([&camera](float farClip)
						{
							camera.SetOrthographicFarClip(farClip);
						}, farClip, camera.GetOrthographicFarClip(), "CameraComponent-Far");

				if(DrawComponentPropertyCheckbox("Fixed Aspect Ratio", &component.FixedAspectRatio))
					Commands::ExecuteRawValueCommand<bool>(&component.FixedAspectRatio, !component.FixedAspectRatio, "CameraComponent-Fixed Aspect Ratio");
			}
			}, m_Context, ComponentIcons::CameraIcon);

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
			{
			//zna1
				ImGui::Unindent();
				if (PrettyButton("Add Script"))
				{
					ScriptResourceData* data = new ScriptResourceData();
					data->ResourceName = "";
					data->Extension = ".cs";
					data->ParentDirectory = AssetDirectoryManager::GetRootAssetTypeDirectory(ResourceType::Script);
					data->Type = ResourceType::Script;

					UUID uuid = ResourceDatabase::AddResource(AssetDirectoryManager::getDirectoryPath(data->ParentDirectory), data, ResourceType::Script);
					component.Scripts.push_back(uuid);
				}
				static std::vector<char*> buffers;

				int i = 0;

				for (UUID scriptUUID : component.Scripts)
				{
					ScriptResourceData* data = (ScriptResourceData*)ResourceDatabase::GetResourceData(scriptUUID);
					std::string& scriptName = data->ResourceName;
					bool open = false;
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

					ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
					ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 2,2 });
					float lineHeight = GImGui->Font->FontSize+ GImGui->Style.FramePadding.y * 2.0f;
					//ImGui::Separator();
					ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(.3f, .3f, .3f, 1.f));
					open = ImGui::TreeNodeEx((void*)(uint64_t)scriptUUID, flags, scriptName.c_str());
					ImGui::PopStyleColor();
					ImGui::PopStyleVar();
					ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);

					if (ImGui::Button("X", ImVec2{ lineHeight,lineHeight })){
						component.Scripts.erase(std::remove(component.Scripts.begin(), component.Scripts.end(), scriptUUID), component.Scripts.end());

						if(open)
							ImGui::TreePop();

						return;
					}

					if (open)
					{
						bool scriptExists = ScriptEngine::EntityClassExists(scriptName);

						if (buffers.size() <= i)
						{
							char* buffer = new char[256];
							buffers.push_back(buffer);
						}

						char* buffer = buffers.at(i);

						memset(buffer, 0, 256);
						strcpy_s(buffer, 256, scriptName.c_str());

						i++;

						UI::ScopedStyleColor styleColor(ImGuiCol_Text, ImVec4{ 1.0f,0.0f,0.0f,1.0f }, !scriptExists);

						if (ImGui::InputText(std::string("Class " + std::to_string(i)).c_str(), buffer, 256))
						{
							scriptName = std::string(buffer);
							ImGui::TreePop();
							return;
						}


						ImGui::Checkbox("Enabled", &data->IsEnabled);

						//Fields

						bool sceneRunning = scene->IsRunning();

						if (sceneRunning)
						{
							//If Scene running
							Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID(), scriptName);
							if (scriptInstance)
							{
								const auto& fields = scriptInstance->GetScriptClass()->GetFields();
								for (const auto& [name, field] : fields)
								{
									//float a = 5.0f;
									//ImGui::DragFloat(name.c_str(), &a, 0.1f);
									switch (field.Type)
									{
									case ScriptFieldType::Float:
									{
										float value = scriptInstance->GetFieldValue<float>(name);
										if (ImGui::DragFloat(name.c_str(), &value, 0.1f))
											scriptInstance->SetFieldValue<float>(name, value);
										break;
									}
									case ScriptFieldType::Int32:
									{
										int value = scriptInstance->GetFieldValue<int>(name);
										if (ImGui::DragInt(name.c_str(), &value, 1.0f))
											scriptInstance->SetFieldValue<int>(name, value);
										break;
									}
									case ScriptFieldType::Bool:
									{
										bool value = scriptInstance->GetFieldValue<bool>(name);
										if (ImGui::Checkbox(name.c_str(), &value))
											scriptInstance->SetFieldValue<bool>(name, value);
										break;
									}
									case ScriptFieldType::String:
									{
										//td::string value = scriptInstance->GetFieldValue<std::string>(name);
										//har buffer[256];
										//emset(buffer, 0, sizeof(buffer));
										//trcpy(buffer, value.c_str());
										//f(ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
										//	scriptInstance->SetFieldValue<std::string>(name, std::string(buffer));
										//reak;
									}
									case ScriptFieldType::Vector2:
									{
										glm::vec2 value = scriptInstance->GetFieldValue<glm::vec2>(name);
										if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(value), 0.1f))
											scriptInstance->SetFieldValue<glm::vec2>(name, value);
										break;
									}
									case ScriptFieldType::Vector3:
									{
										glm::vec3 value = scriptInstance->GetFieldValue<glm::vec3>(name);
										if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(value), 0.1f))
											scriptInstance->SetFieldValue<glm::vec3>(name, value);
										break;
									}
									case ScriptFieldType::Vector4:
									{
										glm::vec4 value = scriptInstance->GetFieldValue<glm::vec4>(name);
										if (ImGui::DragFloat4(name.c_str(), glm::value_ptr(value), 0.1f))
											scriptInstance->SetFieldValue<glm::vec4>(name, value);
										break;
									}
									default:
										break;
									}
								};
							}
						}
						else
						{
							if (scriptExists)
							{
								Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(scriptName);
								const auto& fields = entityClass->GetFields();

								auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);

								for (const auto& [name, field] : fields)
								{
									if (entityFields.find(name) != entityFields.end())
									{
										// Field has been set in editor
										ScriptFieldInstance& scriptField = entityFields.at(name);
										switch (field.Type)
										{
										case ScriptFieldType::Float:
										{
											float data = scriptField.GetValue<float>();
											if (ImGui::DragFloat(name.c_str(), &data, 0.1f))
											{
												scriptField.SetValue(data);
											}
											break;
										}
										case ScriptFieldType::Int32:
										{
											int data = scriptField.GetValue<int>();
											if (ImGui::DragInt(name.c_str(), &data, 1.0f))
											{
												scriptField.SetValue(data);
											}
											break;
										}
										case ScriptFieldType::Bool:
										{
											bool data = scriptField.GetValue<bool>();
											if (ImGui::Checkbox(name.c_str(), &data))
											{
												scriptField.SetValue(data);
											}
											break;
										}
										case ScriptFieldType::String:
										{
											//std::string data = scriptField.GetValue<std::string>();
											//char* buffer[256];
											//memset(buffer, 0, sizeof(buffer));
											//strcpy(buffer, data.c_str());
											//if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
											//{
											//	scriptField.SetValue(std::string(buffer));
											//}
											//break;
										}
										case ScriptFieldType::Vector2:
										{
											glm::vec2 data = scriptField.GetValue<glm::vec2>();
											if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(data), 0.1f))
											{
												scriptField.SetValue(data);
											}
											break;
										}
										case ScriptFieldType::Vector3:
										{
											glm::vec3 data = scriptField.GetValue<glm::vec3>();
											if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(data), 0.1f))
											{
												scriptField.SetValue(data);
											}
											break;
										}
										case ScriptFieldType::Vector4:
										{
											glm::vec4 data = scriptField.GetValue<glm::vec4>();
											if (ImGui::DragFloat4(name.c_str(), glm::value_ptr(data), 0.1f))
											{
												scriptField.SetValue(data);
											}
											break;
										}
										}
									}
									else {
										// Display control to set it
										switch (field.Type)
										{
										case ScriptFieldType::Float:
										{
											float data = 0.0f;
											if (ImGui::DragFloat(name.c_str(), &data))
											{
												ScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue<float>(data);
												float a = fieldInstance.GetValue<float>();
											}
											break;
										}
										case ScriptFieldType::Int32:
										{
											int data = 0;
											if (ImGui::DragInt(name.c_str(), &data))
											{
												ScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue<int>(data);
											}
											break;
										}
										case ScriptFieldType::Bool:
										{
											bool data = false;
											if (ImGui::Checkbox(name.c_str(), &data))
											{
												ScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue<bool>(data);
											}
											break;
										}
										case ScriptFieldType::String:
										{
											//std::string data = "";
											//char buffer[256];
											//memset(buffer, 0, sizeof(buffer));
											//strcpy(buffer, data.c_str());
											//if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
											//{
											//	ScriptFieldInstance& fieldInstance = entityFields[name];
											//	fieldInstance.Field = field;
											//	fieldInstance.SetValue<std::string>(std::string(buffer));
											//}
											//break;
										}
										case ScriptFieldType::Vector2:
										{
											glm::vec2 data = glm::vec2(0.0f);
											if (ImGui::DragFloat2(name.c_str(), glm::value_ptr(data), 0.1f))
											{
												ScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue<glm::vec2>(data);
											}
											break;
										}
										case ScriptFieldType::Vector3:
										{
											glm::vec3 data = glm::vec3(0.0f);
											if (ImGui::DragFloat3(name.c_str(), glm::value_ptr(data), 0.1f))
											{
												ScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue<glm::vec3>(data);
											}
											break;
										}
										case ScriptFieldType::Vector4:
										{
											glm::vec4 data = glm::vec4(0.0f);
											if (ImGui::DragFloat4(name.c_str(), glm::value_ptr(data), 0.1f))
											{
												ScriptFieldInstance& fieldInstance = entityFields[name];
												fieldInstance.Field = field;
												fieldInstance.SetValue<glm::vec4>(data);
											}
											break;
										}
										}
									}
								}
							}
						}

						ImGui::TreePop();
					}
				}


			}, m_Context, ComponentIcons::ScriptIcon);

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [entity](auto& component)
			{
				glm::vec4 color = component.Color;

				if(DrawComponentPropertyColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand<glm::vec4, SpriteRendererComponent>(&component.Color, color, entity, "SpriteRendererComponent-Color");

				DrawComponentPropertyFileReference("Texture", component.TextureUUID, [&component, &entity](int64_t* what) {
					Ref<Texture2D> texture = ResourceDatabase::GetTextureRuntimeResource(*what);
					if (texture->IsLoaded())
					{
						component.TextureUUID = *what;
						Ref<Texture2D> oldTexture = component.Texture;
						component.Texture = texture;
						Commands::ExecuteRawValueCommand<Ref<Texture2D>, SpriteRendererComponent>(&component.Texture, oldTexture, entity, "SpriteRendererComponent-Texture", true);
					}
					else
						EG_WARN("Could not load texture {0}", ResourceDatabase::GetResourcePath(*what));
				});
				float factor = component.TilingFactor;
				if (DrawComponentPropertyFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f))
					Commands::ExecuteRawValueCommand<float, SpriteRendererComponent>(&component.TilingFactor, factor, entity, "SpriteRendererComponent-Tiling Factor");
				
			}, m_Context, ComponentIcons::SpriteRendererIcon);

		DrawComponent<SpriteRendererSTComponent>("SubTexture Sprite Renderer 2D", entity, [](auto& component)
			{
				glm::vec4 color = component.Color;

				if (DrawComponentPropertyColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand(&component.Color, color, "SpriteRendererComponent-Color");
				DrawComponentPropertyFileReference("Texture", component.SubTextureUUID, [&component](int64_t* what) {
					Ref<Texture2D> texture = ResourceDatabase::GetTextureRuntimeResource(*what);

					if (texture->IsLoaded())
					{
						Ref<Texture2D> oldTexture = component.SubTexture->GetTexture();
						component.SubTexture->SetTexture(texture);
						Ref<Texture2D> newTexture = component.SubTexture->GetTexture();
						component.SubTextureUUID = *what;
						Commands::ExecuteRawValueCommand<Ref<Texture2D>>(&newTexture, oldTexture, "SpriteRendererComponent-Texture", true);
					}
					else
						EG_WARN("Could not load texture {0}", ResourceDatabase::GetResourcePath(*what));
				});

				glm::vec2 minCoords = component.SubTexture->GetCoords(0);
				if (DrawComponentPropertyFloat2("Min coords", (float*)component.SubTexture->GetCoordsPtr(0), 0.01f, 0.0f, 1.0f))
				{
					glm::vec2 newCoords = component.SubTexture->GetCoords(0);
					std::vector<glm::vec2> oldCoords = { minCoords, component.SubTexture->GetCoords(1),component.SubTexture->GetCoords(3) };
					std::vector<glm::vec2> newCoordsVec = { newCoords, { component.SubTexture->GetCoords(2).x, newCoords.y }, { newCoords.x, component.SubTexture->GetCoords(2).y } };
					component.SubTexture->SetTexCoords(1, newCoordsVec[1]);
					component.SubTexture->SetTexCoords(3, newCoordsVec[2]);
					Commands::ExecuteValueCommand<std::vector<glm::vec2>>([&component](std::vector<glm::vec2> coords)
						{
							component.SubTexture->SetTexCoords(0, coords[0]);
							component.SubTexture->SetTexCoords(1, coords[1]);
							component.SubTexture->SetTexCoords(3, coords[2]);
						}, newCoordsVec, oldCoords, "SpriteRendererComponent-MinTexCoords", true);
				}
				glm::vec2 maxCoords = component.SubTexture->GetCoords(2);
				if (DrawComponentPropertyFloat2("Max coords", (float*)component.SubTexture->GetCoordsPtr(2), 0.01f, 0.0f, 1.0f))
				{
					glm::vec2 newCoords = component.SubTexture->GetCoords(2);
					std::vector<glm::vec2> oldCoords = { component.SubTexture->GetCoords(1), maxCoords, component.SubTexture->GetCoords(3) };
					std::vector<glm::vec2> newCoordsVec = { { newCoords.x, component.SubTexture->GetCoords(0).x }, newCoords, { component.SubTexture->GetCoords(0).x ,newCoords.y } };
					component.SubTexture->SetTexCoords(1, newCoordsVec[1]);
					component.SubTexture->SetTexCoords(3, newCoordsVec[2]);
					Commands::ExecuteValueCommand<std::vector<glm::vec2>>([&component](std::vector<glm::vec2> coords)
						{
							component.SubTexture->SetTexCoords(1, coords[0]);
							component.SubTexture->SetTexCoords(2, coords[1]);
							component.SubTexture->SetTexCoords(3, coords[2]);
						}, newCoordsVec, oldCoords, "SpriteRendererComponent-MaxTexCoords", true);
				}

				float factor = component.TilingFactor;
				if (DrawComponentPropertyFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f))
					Commands::ExecuteRawValueCommand(&component.TilingFactor, factor, "SpriteRendererComponent-Tiling Factor");

			}, m_Context, ComponentIcons::SubTextureRendererIcon);

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [entity](auto& component)
			{
				float thickness = component.Thickness, fade = component.Fade;
				glm::vec4 color = component.Color;
				if(DrawComponentPropertyColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand<glm::vec4, CircleRendererComponent>(&component.Color, color, entity, "CircleRendererComponent-Color");

				if(DrawComponentPropertyFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f))
					Commands::ExecuteRawValueCommand<float, CircleRendererComponent>(&component.Thickness, thickness, entity, "CircleRendererComponent-Thickness");
				if(DrawComponentPropertyFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f))
					Commands::ExecuteRawValueCommand<float, CircleRendererComponent>(&component.Fade, fade, entity, "CircleRendererComponent-Fade");
			}, m_Context, ComponentIcons::CircleRendererIcon);

		DrawComponent<RigidBody2DComponent>("Rigidbody 2d", entity, [entity](auto& component) {
			const char* bodyTypeString[] = { "Static", "Dynamic", "Kinematic"};
			const char* currentBodyTypeString = bodyTypeString[(int)component.Type];

			DrawComponentPropertyCombo("BodyType", std::vector<const char*>(bodyTypeString, bodyTypeString + sizeof bodyTypeString / sizeof bodyTypeString[0]), (int)component.Type, [&component, &entity](int number) {
				RigidBody2DComponent::BodyType type = component.Type;
				component.Type = (RigidBody2DComponent::BodyType)number;
				Commands::ExecuteRawValueCommand<RigidBody2DComponent::BodyType, RigidBody2DComponent>(&component.Type, type, entity, "RigidBody2DComponent-Body Type", true);
			});
			/*
			if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentBodyTypeString == bodyTypeString[i];
					if (ImGui::Selectable(bodyTypeString[i], isSelected))
					{
						currentBodyTypeString = bodyTypeString[i];
						RigidBody2DComponent::BodyType type = component.Type;
						component.Type = (RigidBody2DComponent::BodyType)i;
						Commands::ExecuteRawValueCommand<RigidBody2DComponent::BodyType, RigidBody2DComponent>(&component.Type, type, entity, "RigidBody2DComponent-Body Type", true);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}
			*/
			if(DrawComponentPropertyCheckbox("Fixed Rotation", &component.FixedRotation))
				Commands::ExecuteRawValueCommand<bool, RigidBody2DComponent>(&component.FixedRotation, !component.FixedRotation, entity, "RigidBody2DComponent-Fixed Rotation");
		}, m_Context, ComponentIcons::RigidBodyIcon);

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [entity](auto& component) {
			float density = component.Density, friction = component.Friction, restitution = component.Restitution, restitutionThreshold = component.RestitutionThreshold;
			glm::vec2 offset = component.Offset, size = component.Size;

			if(DrawComponentPropertyFloat2("Offset", glm::value_ptr(component.Offset), 0.1f))
				Commands::ExecuteRawValueCommand<glm::vec2, BoxCollider2DComponent>(&component.Offset, offset, entity, "BoxCollider2DComponent-Offset");
			if(DrawComponentPropertyFloat2("Size", glm::value_ptr(component.Size), 0.1f))
				Commands::ExecuteRawValueCommand<glm::vec2, BoxCollider2DComponent>(&component.Size, size, entity, "BoxCollider2DComponent-Size");
			if(DrawComponentPropertyFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&component.Density, density, entity, "BoxCollider2DComponent-Density");
			if(DrawComponentPropertyFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&component.Friction, friction, entity, "BoxCollider2DComponent-Friction");
			if(DrawComponentPropertyFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&component.Restitution, restitution, entity, "BoxCollider2DComponent-Restitution");
			if(DrawComponentPropertyFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f))
				Commands::ExecuteRawValueCommand<float, BoxCollider2DComponent>(&component.RestitutionThreshold, restitutionThreshold, entity, "BoxCollider2DComponent-Restitution Threshold");
		}, m_Context, ComponentIcons::BoxColliderIcon);

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [entity](auto& component) {
			float radius = component.Radius, density = component.Density, friction = component.Friction, restitution = component.Restitution, restitutionThreshold = component.RestitutionThreshold;
			glm::vec2 offset = component.Offset;

			if(DrawComponentPropertyFloat2("Offset", glm::value_ptr(component.Offset), 0.1f))
				Commands::ExecuteRawValueCommand<glm::vec2, CircleCollider2DComponent>(&component.Offset, offset, entity, "CircleCollider2DComponent-Offset");
			if(DrawComponentPropertyFloat("Radius", &component.Radius, 0.1f))
				Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&component.Radius, radius, entity, "CircleCollider2DComponent-Radius");
			if(DrawComponentPropertyFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&component.Density, density, entity, "CircleCollider2DComponent-Density");
			if(DrawComponentPropertyFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&component.Friction, friction, entity, "CircleCollider2DComponent-Friction");
			if(DrawComponentPropertyFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&component.Restitution, restitution, entity, "CircleCollider2DComponent-Restitution");
			if(DrawComponentPropertyFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f))
				Commands::ExecuteRawValueCommand<float, CircleCollider2DComponent>(&component.RestitutionThreshold, restitutionThreshold, entity, "CircleCollider2DComponent-Restitution Threshold");
			}, m_Context, ComponentIcons::CircleColliderIcon);

		DrawComponent<TextComponent>("Text Renderer", entity, [entity](auto& component)
			{
				float kerning = component.Kerning, lineSpacing = component.LineSpacing;
				glm::vec4 color = component.Color;

				if(DrawComponentPropertyInputText("Text String", &component.TextString))
					Commands::ExecuteRawValueCommand<std::string, TextComponent>(&component.TextString, component.TextString, entity, "TextComponent-Text String");

				std::string selectedFontName = "OpenSans-Regular";

				if(ResourceDatabase::FindResourceData(component.FontAsset, ResourceType::Font))
					selectedFontName = ResourceDatabase::GetResourceName(component.FontAsset);

				ComponentPropertyBeforeDraw("Font");
				if (ImGui::BeginCombo("", selectedFontName.c_str(), ImGuiComboFlags_None))
				{
					for (const auto& [uuid, font] : ResourceDatabase::GetFontResourceDataCache())
					{
						std::string name = ResourceDatabase::GetResourceName(uuid);
						bool isSelected = component.FontAsset == uuid;
						if (ImGui::Selectable(name.c_str(), isSelected))
						{
							UUID oldFont = component.FontAsset;
							component.FontAsset = uuid;
							component.RuntimeFont = ResourceDatabase::GetFontRuntimeResource(uuid);
							Commands::ExecuteRawValueCommand<UUID, TextComponent>(&component.FontAsset, oldFont, entity, "TextComponent-Font", true);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ComponentPropertyAfterDraw("Font");
				if(DrawComponentPropertyColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand<glm::vec4, TextComponent>(&component.Color, color, entity, "TextComponent-Color");
				if(DrawComponentPropertyFloat("Kerning", &component.Kerning, 0.025f))
					Commands::ExecuteRawValueCommand<float, TextComponent>(&component.Kerning, kerning, entity, "TextComponent-Kerning");
				if(DrawComponentPropertyFloat("Line Spacing", &component.LineSpacing, 0.025f))
					Commands::ExecuteRawValueCommand<float, TextComponent>(&component.LineSpacing, lineSpacing, entity, "TextComponent-Line Spacing");
			}, m_Context, ComponentIcons::TextRendererIcon);

		DrawComponent<AnimatorComponent>("Animator", entity, [entity, this](auto& component)
			{
			bool ShowAnimationPreview = !(m_Context->IsRunning());
				if (PrettyButton("Add Animation", true))
				{
					std::string name = "Animation" + std::to_string(component.Animator2D->GetAnimations()->size());
					component.Animator2D->AddAnimationWithName(name);
					Commands::ExecuteVectorCommand<Ref<Animation>>({ component.Animator2D->GetAnimations(), Commands::VectorCommandType::REMOVE_LAST, Commands::VectorCommandType::ADD, nullptr, CreateRef<Animation>() });
				}
				const Ref<std::vector<Ref<Animation>>> animations = component.Animator2D->GetAnimations();
				if (animations->size() > 0)
				{

					float speed = component.Animator2D->GetSpeed();
					if (DrawComponentPropertyFloat("Speed", component.Animator2D->GetSpeedPtr(), 0.1f, 0.0f, 10.0f))
					{
						Commands::ExecuteRawValueCommand<float>(component.Animator2D->GetSpeedPtr(), speed, "AnimatorComponent-Speed");
					}
					if (!ShowAnimationPreview)
					{
						if (component.Animator2D->GetCurrentAnimation() != nullptr && animations->size() > 0)
						{
							ImGui::TextWithLineLimitAndToolTipV(std::string("Current Animation: "+component.Animator2D->GetCurrentAnimation()->GetName()).c_str(),1, 0);
							if (component.Animator2D->GetCurrentAnimation()->GetFrame())
							{
								Ref<SubTexture2D> subtexture = component.Animator2D->GetCurrentAnimation()->GetFrame();
								ImVec2 minCoords = { subtexture->GetMinImGuiCoords().x, subtexture->GetMinImGuiCoords().y };
								ImVec2 maxCoords = { subtexture->GetMaxImGuiCoords().x, subtexture->GetMaxImGuiCoords().y };
								ImGui::Image((void*)(intptr_t)component.Animator2D->GetCurrentAnimation()->GetFrame()->GetTexture()->GetRendererID(), { 100.0f, 100.0f }, minCoords, maxCoords);
							}
						}
					}
				
					//ImGui::Text("Animations:");
					int i = 0;
					size_t index = -1;
					for (auto animation : *animations)
					{
						
						ImGui::PushID("Anim" + i);
						ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.f,1.f, 1.f, 0.05f));
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,4 });
						ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
						bool opened = ImGui::TreeNodeEx(animation->GetName().c_str(), ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow );
						ImGui::PopStyleVar();
						ImGui::PopStyleVar();
						ImGui::PopStyleColor();
						if (opened)
						{
							if (animation->GetFrameCount() != 0)
							{
								if ((std::find(m_PreviewedAnimations.begin(), m_PreviewedAnimations.end(), animation) == m_PreviewedAnimations.end()) && animation->IsPlayingPtr())
								{
									m_PreviewedAnimations.push_back(animation);
								}
								if (ShowAnimationPreview && animation->GetFrame())
								{
									ImGui::Columns(2, 0, false);
									ImGui::SetColumnWidth(0, 100.f);
									Ref<SubTexture2D> subtexture = animation->GetFrame();
									ImVec2 minCoords = { subtexture->GetMinImGuiCoords().x, subtexture->GetMinImGuiCoords().y };
									ImVec2 maxCoords = { subtexture->GetMaxImGuiCoords().x, subtexture->GetMaxImGuiCoords().y };
									ImGui::Image((void*)(intptr_t)animation->GetFrame()->GetTexture()->GetRendererID(), { 100.0f, 100.0f }, minCoords, maxCoords);
									ImGui::NextColumn();

								}
								//ImGui::Text("Frame rate: %d", animation->GetFrameRate());
								DrawComponentPropertyCheckbox("Looped", animation->IsLoopedPtr());
								DrawComponentPropertyCheckbox("Playing", animation->IsPlayingPtr());
							}
							ImGui::PushID("Frames" + i);
							std::string h;
							if (animation->GetFrameCount() != 0)
							{
								h = "Frames (";
								h += std::to_string(animation->GetFrames().size());
								h += ")";
							}
							else
							{
								h = "Drop Animation here";
							}
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 8,4 });
							ImGui::Button(h.c_str());
							ImGui::PopStyleVar();
							if (ImGui::BeginDragDropTarget())
							{
								uint64_t* uuid;

								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
								{
									uuid = (uint64_t*)payload->Data;

									Ref<Animation> animation = Animation::Create(*uuid);
									if (animation)
									{
										Ref<Animation> oldAnim = component.Animator2D->GetAnimation(i);
										component.Animator2D->SetAnimation(i, animation);
										Ref<Animation> newAnim = component.Animator2D->GetAnimation(i);
										Commands::ExecuteRefValueCommand<Animation>(newAnim, oldAnim, "AnimatorComponent-ChangeAnimation", true);
									}
									else
										EG_WARN("Could not load animation from {0}", ResourceDatabase::GetResourcePath(*uuid));
								}
								ImGui::EndDragDropTarget();
							}
							/* {
								for (int j = 0; j < animation->GetFrames().size(); j++)
								{
									ImGui::PushID("Frame:" + j);
									//TODO: Display the name of the frame and the texture of the frame
									ImGui::Text("Frame: %d", j);
									ImGui::PopID();
								}
								ImGui::TreePop();
							}*/
							if (ImGui::Button("Delete"))
							{
								index = std::find((*animations).begin(), (*animations).end(), animation) - (*animations).begin();
							}
							if (ShowAnimationPreview && animation->GetFrame())
							{
								ImGui::EndColumns();
							}
							ImGui::PopID();
							//TODO: Playing the animation if the animation is looped play non-stop else play once and show button on top to play again
							// 
							//TODO: Display the animation properties if someone wants to change the animation properties show the popup window which will ask if the user wants to change the properties in the scene or in the prefab
							//ImGui::PopID();
							ImGui::TreePop();
						}
						else if (std::find(m_PreviewedAnimations.begin(), m_PreviewedAnimations.end(), animation) != m_PreviewedAnimations.end()){
							m_PreviewedAnimations.erase(std::find(m_PreviewedAnimations.begin(), m_PreviewedAnimations.end(), animation));
						}
						ImGui::PopID();
						i++;
					}
					ImGui::Separator();
					if (index !=-1)
					{
						Commands::ExecuteVectorCommand<Ref<Animation>>({ animations, Commands::VectorCommandType::ADD_AT, Commands::VectorCommandType::REMOVE_AT, animations->at(index) ,nullptr, index});
						component.Animator2D->RemoveAnimation(index);
						if (animations->size() < 2)
						{
							component.Animator2D->GetTransitions()->clear();
						}
					}
					if (animations->size() >=2)
					{
						Ref<std::vector<std::pair<size_t, size_t>>> transitions = component.Animator2D->GetTransitions();
						if (PrettyButton("Add Transition", true))
						{
							ImGui::OpenPopup("AddTransition");
						}
						if (ImGui::BeginPopup("AddTransition"))
						{
							for (size_t i = 0; i < animations->size(); i++)
							{
								for (size_t j = 0; j < animations->size(); j++)
								{
									std::pair<size_t, size_t> transition = std::make_pair(i, j);
									auto hasTransition = std::find(transitions->begin(), transitions->end(), transition);
									if (i != j && hasTransition == transitions->end())
									{
										std::string name = animations->at(i)->GetName() + " to " + animations->at(j)->GetName();
										if (ImGui::Button(name.c_str()))
										{
											component.Animator2D->AddTransition(transition);
											Commands::ExecuteVectorCommand<std::pair<size_t, size_t>>({ component.Animator2D->GetTransitions(), Commands::VectorCommandType::REMOVE_LAST, Commands::VectorCommandType::ADD, transition, transition });
											ImGui::CloseCurrentPopup();
										}
									}
								}
							}
							if (ImGui::Button("Cancel"))
							{
								ImGui::CloseCurrentPopup();
							}
							ImGui::EndPopup();
						}
						if (transitions->size() > 0)
						{
							ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(1.f, 1.f, 1.f, 0.05f));
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0,4 });
							ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
							bool opened = ImGui::TreeNodeEx("Transitions", ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_MoreSpaceBetweenTextAndArrow);
							ImGui::PopStyleVar();
							ImGui::PopStyleVar();
							ImGui::PopStyleColor();
							if (opened)
							{
								int i = 0;
								for (const auto& transition : *transitions)
								{
									ImGui::PushID(i);
									std::string s = "Transition from ";
									s += animations->at(transition.first)->GetName().c_str();
									s += " to ";
									s += animations->at(transition.second)->GetName().c_str();
									float r = ImGui::GetContentRegionMax().x - ImGui::GetCursorPosX();
									ImGui::PushTextWrapPos(r);
									if (ImGui::TextWithLineLimitV(s.c_str(), 1, 0) && ImGui::IsItemHovered()) {
										ImGui::BeginTooltip();
										ImGui::PushTextWrapPos(r);
										ImGui::TextWrapped(s.c_str());
										ImGui::PopTextWrapPos();
										ImGui::EndTooltip();
									}
									ImGui::PopTextWrapPos();
									ImGui::SameLine();
									if (ImGui::Button("..."))
									{
										ImGui::OpenPopup("##Transition Popup");

									}
									if (ImGui::BeginPopup("##Transition Popup"))
									{
										if (ImGui::MenuItem("Remove Transition"))
										{
											component.Animator2D->RemoveTransition(transition);
											Commands::ExecuteVectorCommand<std::pair<size_t, size_t>>({ component.Animator2D->GetTransitions(), Commands::VectorCommandType::ADD, Commands::VectorCommandType::DELETE_FIRST_ENTRY_BY_VALUE, transition, transition, 0, transition, transition });
											ImGui::CloseCurrentPopup();
										}
										ImGui::EndPopup();
									}

									ImGui::PopID();
									i++;
								}
								ImGui::TreePop();
							}
						}
					}
				}
			}, m_Context, ComponentIcons::AnimatorIcon);
		
	}

}