using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public static class InternalCalls
    {
        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsInheritedFromParent(ulong ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsInheritedInChildren(ulong ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_SetParent(ulong ID, ulong parentID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_GetParent(ulong ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsChild(ulong ID, ulong childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsChildOfAny(ulong ID, ulong childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_AddChild(ulong ID, ulong childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveChild(ulong ID, ulong childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveAnyChildren(ulong ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_CopyComponentValuesToChildren(ulong ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_CopyComponentToChildren(ulong ID, Type componentType, bool isUndo);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_InheritComponent(ulong ID, Type componentType, bool isUndo);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetAnyChildren(ulong ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetChildren(ulong ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_FindEntityByName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetScriptInstance(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_AddComponent(ulong ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveComponent(ulong ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_Create(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_Destroy(ulong ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetName(ulong ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetName(ulong ID, string name);
        #endregion

        #region Transform
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong UUID, ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong UUID, out Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(ulong UUID, ref Vector3 quat);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(ulong UUID, out Vector3 quat);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(ulong UUID, ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(ulong UUID, out Vector3 vec);
        #endregion

        #region SpriteRenderer
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_GetColor(ulong UUID, out Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_SetColor(ulong UUID, ref Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string SpriteRendererComponent_GetTexture(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_SetTexture(ulong UUID, ref string texturePath);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float SpriteRendererComponent_GetTilingFactor(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_SetTilingFactor(ulong UUID, ref float tilingFactor);
        #endregion

        #region CircleRenderer
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_GetColor(ulong UUID, out Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_SetColor(ulong UUID, ref Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleRendererComponent_GetThickness(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_SetThickness(ulong UUID, ref float thickness);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleRendererComponent_GetFade(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_SetFade(ulong UUID, ref float fade);
        #endregion

        #region Animator
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_PlayAnimation(ulong UUID, ref string animationName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_StopAnimation(ulong UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_UpdateAnimation(ulong UUID, float dt);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_ChangeAnimation(ulong UUID, string animation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_PauseAnimation(ulong UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_SetSpeed(ulong UUID, float speed);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float AnimatorComponent_GetSpeed(ulong UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_AddAnimation(ulong UUID, string animationName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveAnimation(ulong UUID, string animationName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveLastAnimation(ulong UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_AddTransition(ulong UUID, string from, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_AddTransitionByIndex(ulong UUID, int from, int to);
        internal extern static void AnimatorComponent_RemoveTransition(ulong UUID, string from, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveTransitionByIndex(ulong UUID, int from, int to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AnimatorComponent_CanTransition(ulong UUID, string from, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AnimatorComponent_CanTransitionByIndex(ulong UUID, int from, int to);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string AnimatorComponent_GetCurrentAnimation(ulong UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_SetCurrentAnimation(ulong UUID, ref string animationName);

        #endregion

        #region Camera
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CameraComponent_IsPrimary(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPrimary(ulong UUID, ref bool isPrimary);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CameraComponent_IsFixedAspectRatio(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetFixedAspectRatio(ulong UUID, ref bool isFixedAspectRatio);

        #region SceneCamera
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographic(ulong UUID, ref float size, ref float nearClip, ref float farClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspective(ulong UUID, ref float verticalFov, ref float nearClip, ref float farClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static CameraComponent.ProjectionType CameraComponent_GetProjectionType(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetProjectionType(ulong UUID, ref CameraComponent.ProjectionType type);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetOrthographicSize(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographicSize(ulong UUID, ref float size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetOrthographicNearClip(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographicNearClip(ulong UUID, ref float nearClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetOrthographicFarClip(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographicFarClip(ulong UUID, ref float farClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPerspectiveVerticalFOV(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspectiveVerticalFOV(ulong UUID, ref float verticalFov);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPerspectiveNearClip(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspectiveNearClip(ulong UUID, ref float nearClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPerspectiveFarClip(ulong UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspectiveFarClip(ulong UUID, ref float farClip);
        #endregion
        #endregion

        #region RigidBody2D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_ApplyLinearImpulse(ulong iD, ref Vector2 impulse, ref Vector2 point, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_GetLinearVelocity(ulong entityID, out Vector2 linearVelocity);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static RigidBody2DComponent.BodyType RigidBody2DComponent_GetType(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetType(ulong entityID, RigidBody2DComponent.BodyType type);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_ApplyLinearImpulseToCenter(ulong iD, ref Vector2 impulse, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_IsFixedRotation(ulong iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetFixedRotation(ulong iD, ref bool fixedRotation);
        #endregion

        #region BoxCollider2D

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_GetSize(ulong entityID, out Vector2 size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetSize(ulong entityID, ref Vector2 size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_GetOffset(ulong entityID, out Vector2 offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetOffset(ulong entityID, ref Vector2 offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetDensity(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetDensity(ulong entityID, ref float density);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetFriction(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetFriction(ulong entityID, ref float friction);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetRestitution(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetRestitution(ulong entityID, ref float restitution);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetRestitutionThreshold(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetRestitutionThreshold(ulong entityID, ref float restitutionThreshold);

        #endregion

        #region CircleCollider2D

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetOffset(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetOffset(ulong entityID, ref float offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetDensity(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetDensity(ulong entityID, ref float density);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetFriction(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetFriction(ulong entityID, ref float friction);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetRestitution(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetRestitution(ulong entityID, ref float restitution);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetRestitutionThreshold(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetRestitutionThreshold(ulong entityID, ref float restitutionThreshold);
        #endregion

        #region Input

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keyCode);
        #endregion

        #region TextComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TextComponent_GetText(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetText(ulong entityID, string text);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_GetColor(ulong entityID, out Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetColor(ulong entityID, ref Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextComponent_GetKerning(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetKerning(ulong entityID, float kerning);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextComponent_GetLineSpacing(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetLineSpacing(ulong entityID, float lineSpacing);
        #endregion

    }
}
