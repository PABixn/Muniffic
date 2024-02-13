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

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace eg {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		m_Context = scene;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		if (m_Context) {
			m_Context->m_Registry.each([&](auto entityID)
				{
					Entity entity{ entityID, m_Context.get() };
					DrawEntityNode(entity);
				});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				m_SelectionContext = {};

			// Right click on blank space
			if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
				{
					Commands::ExecuteCommand<Commands::CreateEntityCommand>(Commands::CommandArgs("Empty Entity", {}, m_Context, m_SelectionContext));
				}
				ImGui::EndPopup();
			}	
		}

		ImGui::Begin("Properties");
		if (m_SelectionContext)
		{
			DrawComponents(m_SelectionContext);
		}
		ImGui::End();

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				Commands::ExecuteCommand<Commands::DeleteEntityCommand>(Commands::CommandArgs("", entity, m_Context, m_SelectionContext));
			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		float x = values.x, y = values.y, z = values.z;

		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			Commands::ExecuteRawValueCommand<float>(&values.x, x, label + std::string("##X"), true);
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
	
		if (ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f"))
			Commands::ExecuteRawValueCommand<float>(&values.x, x, label + std::string("##X"));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			Commands::ExecuteRawValueCommand<float>(&values.y, y, label + std::string("##Y"), true);
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if(ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f"))
			Commands::ExecuteRawValueCommand<float>(&values.y, y, label + std::string("##Y"));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.15f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.25f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.15f,0.8f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			Commands::ExecuteRawValueCommand<float>(&values.z, z, label + std::string("##Z"), true);
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		if(ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f"))
			Commands::ExecuteRawValueCommand<float>(&values.z, z, label + std::string("##Z"));
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		
		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction, Ref<Scene>& context)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight,lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (name != std::string("Transform"))
				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove Component"))
						Commands::ExecuteCommand<Commands::RemoveComponentCommand<T>>(Commands::CommandArgs("", entity, context, entity));
					ImGui::EndPopup();
				}
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
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
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
				std::cout<<tag<<std::endl;
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		if (ImGui::Button("Add Component"))
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
			DisplayAddComponentEntry<SpriteComponentSTComponent>("SubTexture Sprite Renderer 2D");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
		
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				//glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", component.Rotation);
				//component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			}, m_Context);

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.Camera;

			if(ImGui::Checkbox("Primary", &component.Primary))
				Commands::ExecuteRawValueCommand<bool>(&component.Primary, !component.Primary, "CameraComponent-Primary");

			const char* projectionTypeString[] = { "Perspective","Orthographic" };
			const char* currentProjectionTypeString = projectionTypeString[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeString[i];
					if (ImGui::Selectable(projectionTypeString[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeString[i];
						Commands::ExecuteValueCommand<SceneCamera::ProjectionType>([&camera](SceneCamera::ProjectionType projectionType)
							{
								camera.SetProjectionType(projectionType);
							}, (SceneCamera::ProjectionType)i, camera.GetProjectionType(), "CameraComponent-ProjectionType", true);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
				if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
					//camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));
					Commands::ExecuteValueCommand<float>([&camera](float verticalFov)
						{
							camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));
						}, perspectiveVerticalFov, glm::degrees(camera.GetPerspectiveVerticalFOV()), "CameraComponent-Vertical FOV");

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					Commands::ExecuteValueCommand<float>([&camera](float nearClip)
						{
							camera.SetPerspectiveNearClip(nearClip);
						}, perspectiveNear, camera.GetPerspectiveNearClip(), "CameraComponent-Near");

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					Commands::ExecuteValueCommand<float>([&camera](float farClip)
						{
							camera.SetPerspectiveFarClip(farClip);
						}, perspectiveFar, camera.GetPerspectiveFarClip(), "CameraComponent-Far");
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &size))
					Commands::ExecuteValueCommand<float>([&camera](float size)
						{
							camera.SetOrthographicSize(size);
						}, size, camera.GetOrthographicSize(), "CameraComponent-Size");

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &nearClip))
					Commands::ExecuteValueCommand<float>([&camera](float nearClip)
						{
							camera.SetOrthographicNearClip(nearClip);
						}, nearClip, camera.GetOrthographicNearClip(), "CameraComponent-Near");

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &farClip))
					Commands::ExecuteValueCommand<float>([&camera](float farClip)
						{
							camera.SetOrthographicFarClip(farClip);
						}, farClip, camera.GetOrthographicFarClip(), "CameraComponent-Far");

				if(ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio))
					Commands::ExecuteRawValueCommand<bool>(&component.FixedAspectRatio, !component.FixedAspectRatio, "CameraComponent-Fixed Aspect Ratio");
			}
			}, m_Context);

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
			{
				bool scriptExists = ScriptEngine::EntityClassExists(component.Name);

				static char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), component.Name.c_str());

				UI::ScopedStyleColor styleColor(ImGuiCol_Text, ImVec4{ 1.0f,0.0f,0.0f,1.0f }, !scriptExists);


				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				{
					component.Name = std::string(buffer);
					return;
				}
				//Fields
				
				bool sceneRunning = scene->IsRunning();
				
				if(sceneRunning)
				{
					//If Scene running
					Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
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
						Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.Name);
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

				if(!scriptExists)
					ImGui::PopStyleColor();

			}, m_Context);

		DrawComponent<SpriteRendererComponent>("Sprite Renderer 2D", entity, [](auto& component)
			{
				glm::vec4 color = component.Color;

				if(ImGui::ColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand(&component.Color, color, "SpriteRendererComponent-Color");

				
				ImGui::Button("Texture", {100.0f, 0.0f});

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(path);
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
						{
							Ref<Texture2D> oldTexture = component.Texture;
							component.Texture = texture;
							Commands::ExecuteRawValueCommand<Ref<Texture2D>>(&component.Texture, oldTexture, "SpriteRendererComponent-Texture", true);
						}
						else
							EG_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				float factor = component.TilingFactor;
				if(ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f))
					Commands::ExecuteRawValueCommand(&component.TilingFactor, factor, "SpriteRendererComponent-Tiling Factor");
				
			}, m_Context);

		DrawComponent<SpriteComponentSTComponent>("SubTexture Sprite Renderer 2D", entity, [](auto& component)
			{
				glm::vec4 color = component.Color;

				if (ImGui::ColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand(&component.Color, color, "SpriteRendererComponent-Color");


				ImGui::Button("Texture", { 100.0f, 0.0f });

				

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(path);
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
						{
							Ref<Texture2D> oldTexture = component.SubTexture->GetTexture();
							component.SubTexture->SetTexture(texture);
							Ref<Texture2D> newTexture = component.SubTexture->GetTexture();
							Commands::ExecuteRawValueCommand<Ref<Texture2D>>(&newTexture, oldTexture, "SpriteRendererComponent-Texture", true);
						}
						else
							EG_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				
					//ImGui::Text("Texture Coordinates:")
					ImGui::Text("Min Coords:");
					ImGui::SameLine();
					ImGui::PushID(0);
					glm::vec2 minCoords = component.SubTexture->GetCoords(0);
					if (ImGui::DragFloat2("##UV", (float*)component.SubTexture->GetCoordsPtr(0), 0.01f, 0.0f, 1.0f))
					{
						glm::vec2 newCoords = component.SubTexture->GetCoords(0);
						std::vector<glm::vec2> oldCoords = {minCoords, component.SubTexture->GetCoords(1),component.SubTexture->GetCoords(3) };
						std::vector<glm::vec2> newCoordsVec = {newCoords, { component.SubTexture->GetCoords(2).x, newCoords.y }, { newCoords.x, component.SubTexture->GetCoords(2).y } };
						component.SubTexture->SetTexCoords(1, newCoordsVec[1]);
						component.SubTexture->SetTexCoords(3, newCoordsVec[2]);
						Commands::ExecuteValueCommand<std::vector<glm::vec2>>([&component](std::vector<glm::vec2> coords)
							{
								component.SubTexture->SetTexCoords(0, coords[0]);
								component.SubTexture->SetTexCoords(1, coords[1]);
								component.SubTexture->SetTexCoords(3, coords[2]);
							}, newCoordsVec, oldCoords, "SpriteRendererComponent-MinTexCoords", true);
					}
					ImGui::PopID();

					ImGui::Text("Max Coords:");
					ImGui::SameLine();
					ImGui::PushID(2);
					glm::vec2 maxCoords = component.SubTexture->GetCoords(2);
					if (ImGui::DragFloat2("##UV", (float*)component.SubTexture->GetCoordsPtr(2), 0.01f, 0.0f, 1.0f))
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
							}, newCoordsVec, oldCoords, "SpriteRendererComponent-MinTexCoords", true);
					}
					ImGui::PopID();
				
					float factor = component.TilingFactor;
					if (ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f))
						Commands::ExecuteRawValueCommand(&component.TilingFactor, factor, "SpriteRendererComponent-Tiling Factor");

			}, m_Context);

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				float thickness = component.Thickness, fade = component.Fade;
				glm::vec4 color = component.Color;
				if(ImGui::ColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand(&component.Color, color, "CircleRendererComponent-Color");

				if(ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f))
					Commands::ExecuteRawValueCommand(&component.Thickness, thickness, "CircleRendererComponent-Thickness");
				if(ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f))
					Commands::ExecuteRawValueCommand(&component.Fade, fade, "CircleRendererComponent-Fade");
			}, m_Context);

		DrawComponent<RigidBody2DComponent>("Rigidbody 2d", entity, [](auto& component) {
			const char* bodyTypeString[] = { "Static", "Dynamic", "Kinematic"};
			const char* currentBodyTypeString = bodyTypeString[(int)component.Type];
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
						Commands::ExecuteRawValueCommand(&component.Type, type, "RigidBody2DComponent-Body Type", true);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			if(ImGui::Checkbox("Fixed Rotation", &component.FixedRotation))
				Commands::ExecuteRawValueCommand<bool>(&component.FixedRotation, !component.FixedRotation, "RigidBody2DComponent-Fixed Rotation");
		}, m_Context);

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component) {
			float density = component.Density, friction = component.Friction, restitution = component.Restitution, restitutionThreshold = component.RestitutionThreshold;
			glm::vec2 offset = component.Offset, size = component.Size;

			if(ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f))
				Commands::ExecuteRawValueCommand(&component.Offset, offset, "BoxCollider2DComponent-Offset");
			if(ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.1f))
				Commands::ExecuteRawValueCommand(&component.Size, size, "BoxCollider2DComponent-Size");
			if(ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand(&component.Density, density, "BoxCollider2DComponent-Density");
			if(ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand(&component.Friction, friction, "BoxCollider2DComponent-Friction");
			if(ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand(&component.Restitution, restitution, "BoxCollider2DComponent-Restitution");
			if(ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f))
				Commands::ExecuteRawValueCommand(&component.RestitutionThreshold, restitutionThreshold, "BoxCollider2DComponent-Restitution Threshold");
		}, m_Context);

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component) {
			float radius = component.Radius, density = component.Density, friction = component.Friction, restitution = component.Restitution, restitutionThreshold = component.RestitutionThreshold;
			glm::vec2 offset = component.Offset;

			if(ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f))
				Commands::ExecuteRawValueCommand(&component.Offset, offset, "CircleCollider2DComponent-Offset");
			if(ImGui::DragFloat("Radius", &component.Radius, 0.1f))
				Commands::ExecuteRawValueCommand(&component.Radius, radius, "CircleCollider2DComponent-Radius");
			if(ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand(&component.Density, density, "CircleCollider2DComponent-Density");
			if(ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand(&component.Friction, friction, "CircleCollider2DComponent-Friction");
			if(ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f))
				Commands::ExecuteRawValueCommand(&component.Restitution, restitution, "CircleCollider2DComponent-Restitution");
			if(ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f))
				Commands::ExecuteRawValueCommand(&component.RestitutionThreshold, restitutionThreshold, "CircleCollider2DComponent-Restitution Threshold");
			}, m_Context);

		DrawComponent<TextComponent>("Text Renderer", entity, [](auto& component)
			{
				float kerning = component.Kerning, lineSpacing = component.LineSpacing;
				glm::vec4 color = component.Color;

				ImGui::InputTextMultiline("Text String", &component.TextString);
				if(ImGui::ColorEdit4("Color", glm::value_ptr(component.Color)))
					Commands::ExecuteRawValueCommand(&component.Color, color, "TextComponent-Color");
				if(ImGui::DragFloat("Kerning", &component.Kerning, 0.025f))
					Commands::ExecuteRawValueCommand(&component.Kerning, kerning, "TextComponent-Kerning");
				if(ImGui::DragFloat("Line Spacing", &component.LineSpacing, 0.025f))
					Commands::ExecuteRawValueCommand(&component.LineSpacing, lineSpacing, "TextComponent-Line Spacing");
			}, m_Context);
	}


}