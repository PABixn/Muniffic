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
        #region Console
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Console_Log(string msg, DebugConsole.LogType type);
        #endregion

        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsInheritedFromParent(long ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsInheritedInChildren(long ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static long Entity_SetParent(long ID, long parentID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static long Entity_GetParent(long ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsChild(long ID, long childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_IsChildOfAny(long ID, long childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_AddChild(long ID, long childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveChild(long ID, long childID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveAnyChildren(long ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_CopyComponentValuesToChildren(long ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_CopyComponentToChildren(long ID, Type componentType, bool isUndo);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_InheritComponent(long ID, Type componentType, bool isUndo);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static IntPtr Entity_GetAnyChildren(long ID, out int size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static IntPtr Entity_GetChildren(long ID, out int size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static long Entity_FindEntityByName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object Entity_GetScriptInstance(long UUID, string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(long ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_AddComponent(long ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveComponent(long ID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static long Entity_Create(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_Destroy(long ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Entity_GetName(long ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_SetName(long ID, string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_Exists(long ID);
        #endregion

        #region Transform
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(long UUID, ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(long UUID, out Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetRotation(long UUID, ref Vector3 quat);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRotation(long UUID, out Vector3 quat);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetScale(long UUID, ref Vector3 vec);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetScale(long UUID, out Vector3 vec);
        #endregion

        #region SpriteRenderer
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_GetColor(long UUID, out Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_SetColor(long UUID, ref Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string SpriteRendererComponent_GetTexture(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_SetTexture(long UUID, ref string texturePath);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float SpriteRendererComponent_GetTilingFactor(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void SpriteRendererComponent_SetTilingFactor(long UUID, ref float tilingFactor);
        #endregion

        #region CircleRenderer
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_GetColor(long UUID, out Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_SetColor(long UUID, ref Vector4 color);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleRendererComponent_GetThickness(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_SetThickness(long UUID, ref float thickness);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleRendererComponent_GetFade(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleRendererComponent_SetFade(long UUID, ref float fade);
        #endregion

        #region Animator
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_PlayAnimation(long UUID, string animationName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_StopAnimation(long UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_UpdateAnimation(long UUID, float dt);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_ChangeAnimation(long UUID, string animation);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_PauseAnimation(long UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_SetSpeed(long UUID, float speed);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float AnimatorComponent_GetSpeed(long UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_AddAnimation(long UUID, string animationName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveAnimation(long UUID, string animationName);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveLastAnimation(long UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_Transition(long UUID, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_TransitionByIndex(long UUID, int to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_AddTransition(long UUID, string from, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_AddTransitionByIndex(long UUID, int from, int to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveTransition(long UUID, string from, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_RemoveTransitionByIndex(long UUID, int from, int to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AnimatorComponent_CanTransition(long UUID, string from, string to);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool AnimatorComponent_CanTransitionByIndex(long UUID, int from, int to);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string AnimatorComponent_GetCurrentAnimation(long UUID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void AnimatorComponent_SetCurrentAnimation(long UUID, string animationName);

        #endregion

        #region Camera
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CameraComponent_IsPrimary(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPrimary(long UUID, ref bool isPrimary);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CameraComponent_IsFixedAspectRatio(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetFixedAspectRatio(long UUID, ref bool isFixedAspectRatio);

        #region SceneCamera
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographic(long UUID, ref float size, ref float nearClip, ref float farClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspective(long UUID, ref float verticalFov, ref float nearClip, ref float farClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static CameraComponent.ProjectionType CameraComponent_GetProjectionType(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetProjectionType(long UUID, CameraComponent.ProjectionType type);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetOrthographicSize(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographicSize(long UUID, ref float size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetOrthographicNearClip(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographicNearClip(long UUID, ref float nearClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetOrthographicFarClip(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetOrthographicFarClip(long UUID, ref float farClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPerspectiveVerticalFOV(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspectiveVerticalFOV(long UUID, ref float verticalFov);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPerspectiveNearClip(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspectiveNearClip(long UUID, ref float nearClip);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CameraComponent_GetPerspectiveFarClip(long UUID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetPerspectiveFarClip(long UUID, ref float farClip);
        #endregion
        #endregion

        #region RigidBody2D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_AwakeRuntimeBody(long ID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_ApplyLinearImpulse(long ID, ref Vector2 impulse, ref Vector2 point, bool wake);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_GetLinearVelocity(long entityID, out Vector2 linearVelocity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetLinearVelocity(long entityID, ref Vector2 linearVelocity);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static RigidBody2DComponent.BodyType RigidBody2DComponent_GetType(long entityID);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetType(long entityID, RigidBody2DComponent.BodyType type);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_ApplyLinearImpulseToCenter(long iD, ref Vector2 impulse, bool wake);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_IsFixedRotation(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetFixedRotation(long iD, ref bool fixedRotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetGravityMultiplier(long iD, float newMultiplier);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBody2DComponent_GetGravityMultiplier(long iD);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyForce(long iD, ref Vector2 force, ref Vector2 point, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyForceToCenter(long iD, ref Vector2 force, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_ApplyTorque(long iD, float torque, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetEnabled(long iD, bool enabled);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_IsEnabled(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetSleepingAllowed(long iD, bool allowed);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_IsSleepingAllowed(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBody2DComponent_GetAngularDamping(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetAngularDamping(long iD, float angularDamping);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBody2DComponent_GetAngularVelocity(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetAngularVelocity(long iD, float velocity);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void RigidBody2DComponent_GetContactList(long iD);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //internal extern static void RigidBody2DComponent_GetJointList(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBody2DComponent_GetLinearDamping(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetLinearDamping(long iD, float linearDamping);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Vector2 RigidBody2DComponent_GetLocalPoint(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Vector2 RigidBody2DComponent_GetLocalVector(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBody2DComponent_GetMass(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float RigidBody2DComponent_GetMassData(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetMassData(long iD, float massData);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Vector2 RigidBody2DComponent_GetPosition(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static B2transform RigidBody2DComponent_GetTransform(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetTransform(long iD, B2transform transform);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
       // internal extern static ref Entity RigidBody2DComponent_GetUserData(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Vector2 RigidBody2DComponent_GetWorldPoint(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Vector2 RigidBody2DComponent_GetWorldVector(long iD);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_IsAwake(long iD);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetAwake(long iD, bool awake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool RigidBody2DComponent_IsBullet(long iD);


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetBullet(long iD, bool bullet);







        #endregion

        #region BoxCollider2D

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_GetSize(long entityID, out Vector2 size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetSize(long entityID, ref Vector2 size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_GetOffset(long entityID, out Vector2 offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetOffset(long entityID, ref Vector2 offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetDensity(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetDensity(long entityID, ref float density);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetFriction(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetFriction(long entityID, ref float friction);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetRestitution(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetRestitution(long entityID, ref float restitution);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float BoxCollider2DComponent_GetRestitutionThreshold(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetRestitutionThreshold(long entityID, ref float restitutionThreshold);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_IsSensor(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void BoxCollider2DComponent_SetSensor(long entityID, ref bool isSensor);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWith(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithCircle(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithBox(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithPoint(long entityID, ref Vector2 point);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithTopEdge(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithBottomEdge(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithLeftEdge(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithRightEdge(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithBoxCoords(long entityID, ref Vector2 center, ref Vector2 size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithCircleCoords(long entityID, ref Vector2 center, float radius);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool BoxCollider2DComponent_CollidesWithEdgeCoords(long entityID, ref Vector2 start, ref Vector2 end);

        #endregion

        #region CircleCollider2D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetRadius(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetRadius(long entityID, ref float radius);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetOffset(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetOffset(long entityID, ref float offset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetDensity(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetDensity(long entityID, ref float density);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetFriction(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetFriction(long entityID, ref float friction);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetRestitution(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetRestitution(long entityID, ref float restitution);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float CircleCollider2DComponent_GetRestitutionThreshold(long entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetRestitutionThreshold(long entityID, ref float restitutionThreshold);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_IsSensor(long entityID);

        [MethodImplAttribute (MethodImplOptions.InternalCall)]
        internal extern static void CircleCollider2DComponent_SetSensor(long entityID, ref bool isSensor);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWith(long entityID, long otherEntityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithBox(long entityID, long otherEntity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithCircle(long entityID, long otherEntity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithPoint(long entityID, ref Vector2 point);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithTopEdge(long entityID, long otherEntity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithBottomEdge(long entityID, long otherEntity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithLeftEdge(long entityID, long otherEntity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithRightEdge(long entityID, long otherEntity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithBoxCoords(long entityID, ref Vector2 center, ref Vector2 size);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithCircleCoords(long entityID, ref Vector2 center, float radius);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool CircleCollider2DComponent_CollidesWithEdgeCoords(long entityID, ref Vector2 start, ref Vector2 end);

        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Input_GetClipboardContent();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_SetClipboardContent(string content);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string Input_GetKeyName(KeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyPressed(KeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyReleased(KeyCode keycode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseButtonPressed(MouseCode button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsMouseButtonReleased(MouseCode button);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsCursorOnWindow();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetCursorPositonX();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float Input_GetCursorPositonY();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_SetCursorMode(int mode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_SetStickyKeysEnabled(bool enable);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Input_SetStickyMouseButtonsEnabled(bool enable);
        #endregion

        #region TextComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string TextComponent_GetText(long entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetText(long entityID, string text);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_GetColor(long entityID, out Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetColor(long entityID, ref Vector4 color);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextComponent_GetKerning(long entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetKerning(long entityID, float kerning);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float TextComponent_GetLineSpacing(long entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TextComponent_SetLineSpacing(long entityID, float lineSpacing);
        #endregion

    }
}
