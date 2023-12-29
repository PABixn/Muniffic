#include "egpch.h"
#include "SceneHierarchyPanel.h"
#include "Engine/Scripting/ScriptEngine.h"

#include <Imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <Imgui/imgui_internal.h>
#include <cstring>
#include "../Commands/Commands.h"

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace eg {

	extern const std::filesystem::path g_AssetPath;

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
		Commands::Command* command = nullptr;
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
					command = new Commands::CreateEntityCommand(m_Context, m_SelectionContext);
					command->Execute({ "Empty Entity" });
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
		Commands::Command* command = nullptr;
		auto& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
			m_SelectionContext = entity;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				command = new Commands::DeleteEntityCommand(m_Context, m_SelectionContext);
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

		if (command != nullptr)
		{
			command->Execute({ "", entity });
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
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
		if(ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.15f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.25f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.15f,0.8f,1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
		
		ImGui::Columns(1);
		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		Commands::Command* command = nullptr;
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

			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					command = new Commands::RemoveComponentCommand<T>(entity);
				ImGui::EndPopup();
			}
			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}
			if (command != nullptr)
				command->Execute({ "", entity });
			//ImGui::PopStyleVar();
		}
	}

	template<typename T>
	void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
		if (!m_SelectionContext.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				Commands::Command* command = new Commands::AddComponentCommand<T>(m_SelectionContext);
				command->Execute({ "", m_SelectionContext});
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

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
		
		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translation", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component) {
			auto& camera = component.Camera;

			ImGui::Checkbox("Primary", &component.Primary);

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
						camera.SetProjectionType((SceneCamera::ProjectionType)i);
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
					camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

				float perspectiveNear = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
					camera.SetPerspectiveNearClip(perspectiveNear);

				float perspectiveFar = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
					camera.SetPerspectiveFarClip(perspectiveFar);
			}

			if (component.Camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &size))
					camera.SetOrthographicSize(size);

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near", &nearClip))
					camera.SetOrthographicNearClip(nearClip);

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far", &farClip))
					camera.SetOrthographicFarClip(farClip);

				ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
			}
			});

		DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
			{
				bool scriptExists = ScriptEngine::EntityClassExists(component.Name);

				static char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), component.Name.c_str());

				if(!scriptExists)
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f,0.0f,0.0f,1.0f });

				if (ImGui::InputText("Class", buffer, sizeof(buffer)))
				{
					component.Name = std::string(buffer);
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

			});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				
				ImGui::Button("Texture", {100.0f, 0.0f});

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserPanel"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							EG_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);	
				
			});

		DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

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
						component.Type = (RigidBody2DComponent::BodyType)i;
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component) {
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f);
			ImGui::DragFloat2("Size", glm::value_ptr(component.Size), 0.1f);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
		});

		DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component) {
			ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset), 0.1f);
			ImGui::DragFloat("Radius", &component.Radius, 0.1f);
			ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});
	}


}