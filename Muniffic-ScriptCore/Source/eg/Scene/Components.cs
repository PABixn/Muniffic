using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public class DebugConsole
    {
        public enum LogType
        {
            Info = 0, Warning = 1, Error = 2
        }
        public static void Log(string message, LogType logType)
        {
            InternalCalls.Console_Log(message, logType);
        }
    }

    /// <summary>
    /// Base class for all components.
    /// </summary>
    public abstract class Component
    {
        /// <summary>
        /// Reference to entity the component is attached to.
        /// </summary>
        public Entity Entity { get; internal set; }

        /// <summary>
        /// Gets a value indicating whether this component is inherited from its parent entity.
        /// </summary>
        /// <returns><c>true</c> if this component is inherited from its parent entity; otherwise, <c>false</c>.</returns>
        public bool isInherited
        {
            get
            {
                return InternalCalls.Entity_IsInheritedFromParent(Entity.ID, this.GetType());
            }
        }

        /// <summary>
        /// Gets a value indicating whether this component is inherited in any of its child entities.
        /// </summary>
        /// <returns><c>true</c> if this component is inherited in any of its child entities; otherwise, <c>false</c>.</returns>
        public bool isInheritedInChildren
        {
            get
            {
                return InternalCalls.Entity_IsInheritedInChildren(Entity.ID, this.GetType());
            }
        }
    }

    public class TransformComponent : Component
    {
        /// <summary>
        /// Translation of the entity.
        /// </summary>
        public Vector3 translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }

            set => InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
        }

        /// <summary>
        /// Scale of the entity.
        /// </summary>
        public Vector3 scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }

            set => InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
        }

        /// <summary>
        /// Rotation of the entity.
        /// </summary>
        /// <remarks>Rotation is given in degrees.</remarks>
        public Vector3 rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation;
            }

            set => InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
        }
    }

    public class SpriteRendererComponent : Component
    {
        /// <summary>
        /// Color of the SpriteRenderer component.
        /// </summary>
        /// <remarks>Color is given in RGBA format.</remarks>
        public Color color
        {
            get
            {
                InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return new Color(color);
            }

            set
            {
                Vector4 colorVec = Color.ToVector4(value);
                InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref colorVec);
            }
        }

        /// <summary>
        /// Texture of the SpriteRenderer component.
        /// </summary>
        /// <remarks>Path to the texture file.</remarks>
        public string texture
        {
            get => InternalCalls.SpriteRendererComponent_GetTexture(Entity.ID);
            set => InternalCalls.SpriteRendererComponent_SetTexture(Entity.ID, ref value);
        }

        /// <summary>
        /// Tile size of the SpriteRenderer component.
        /// </summary>
        public float tilingFactor
        {
            get => InternalCalls.SpriteRendererComponent_GetTilingFactor(Entity.ID);
            set => InternalCalls.SpriteRendererComponent_SetTilingFactor(Entity.ID, ref value);
        }
    }

    public class CircleRendererComponent : Component
    {
        /// <summary>
        /// Color of the SpriteRenderer component.
        /// </summary>
        /// <remarks>Color is given in RGBA format.</remarks>
        public Color color
        {
            get
            {
                InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return new Color(color);
            }

            set
            {
                Vector4 colorVec = Color.ToVector4(value);
                InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref colorVec);
            }
        }

        /// <summary>
        /// Radius of the CircleRenderer component.
        /// </summary>
        public float thickness
        {
            get => InternalCalls.CircleRendererComponent_GetThickness(Entity.ID);
            set => InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, ref value);
        }

        /// <summary>
        /// Opacity of the CircleRenderer component.
        /// </summary>
        public float fade
        {
            get => InternalCalls.CircleRendererComponent_GetFade(Entity.ID);
            set => InternalCalls.CircleRendererComponent_SetFade(Entity.ID, ref value);
        }
    }

    public class AnimatorComponent : Component
    {
        public float Speed
        {
            get => InternalCalls.AnimatorComponent_GetSpeed(Entity.ID);
            set => InternalCalls.AnimatorComponent_SetSpeed(Entity.ID, value);
        }

        public void Play(string animationName)
        {
            InternalCalls.AnimatorComponent_PlayAnimation(Entity.ID, animationName);
        }

        public void Stop()
        {
            InternalCalls.AnimatorComponent_StopAnimation(Entity.ID);
        }

        public void Pause()
        {
            InternalCalls.AnimatorComponent_PauseAnimation(Entity.ID);
        }

        public void Update(float deltaTime)
        {
            InternalCalls.AnimatorComponent_UpdateAnimation(Entity.ID, deltaTime);
        }

        public void ChangeAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_ChangeAnimation(Entity.ID, animationName);
        }

        public void AddAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_AddAnimation(Entity.ID, animationName);
        }

        public void RemoveAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_RemoveAnimation(Entity.ID, animationName);
        }

        public void RemoveLastAnimation()
        {
            InternalCalls.AnimatorComponent_RemoveLastAnimation(Entity.ID);
        }

        public void Transition(string to)
        {
            InternalCalls.AnimatorComponent_Transition(Entity.ID, to);
        }

        public void TransitionByIndex(int index)
        {
            InternalCalls.AnimatorComponent_TransitionByIndex(Entity.ID, index);
        }

        public void AddTransition(string from, string to)
        {
            InternalCalls.AnimatorComponent_AddTransition(Entity.ID, from, to);
        }

        public void RemoveTransition(string from, string to)
        {
            InternalCalls.AnimatorComponent_RemoveTransition(Entity.ID, from, to);
        }

        public void AddTransitionByIndex(int from, int to)
        {
            InternalCalls.AnimatorComponent_AddTransitionByIndex(Entity.ID, from, to);
        }

        public void RemoveTransitionByIndex(int from, int to)
        {
            InternalCalls.AnimatorComponent_RemoveTransitionByIndex(Entity.ID, from, to);
        }

        public void CanTransition(string from, string to)
        {
            InternalCalls.AnimatorComponent_CanTransition(Entity.ID, from, to);
        }

        public void CanTransitionByIndex(int from, int to)
        {
            InternalCalls.AnimatorComponent_CanTransitionByIndex(Entity.ID, from, to);
        }

        public string GetCurrentAnimation()
        {
            return InternalCalls.AnimatorComponent_GetCurrentAnimation(Entity.ID);
        }
        public void SetCurrentAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_SetCurrentAnimation(Entity.ID, animationName);
        }

    }

    public class CameraComponent : Component
    {
        /// <summary>
        /// Type of projection used by the camera.
        /// </summary>
        public enum ProjectionType { Perspective = 0, Ortographic = 1 }

        /// <summary>
        /// Specifies if the camera is primary.
        /// </summary>
        public bool primary
        {
            get => InternalCalls.CameraComponent_IsPrimary(Entity.ID);
            set => InternalCalls.CameraComponent_SetPrimary(Entity.ID, ref value);
        }

        /// <summary>
        /// Specifies if the camera has fixed aspect ratio.
        /// </summary>
        public bool fixedAspectRatio
        {
            get => InternalCalls.CameraComponent_IsFixedAspectRatio(Entity.ID);
            set => InternalCalls.CameraComponent_SetFixedAspectRatio(Entity.ID, ref value);
        }

        /// <summary>
        /// Type of projection used by the camera.
        /// </summary>
        public ProjectionType type
        {
            get => InternalCalls.CameraComponent_GetProjectionType(Entity.ID);
            set => InternalCalls.CameraComponent_SetProjectionType(Entity.ID, value);
        }

        /// <summary>
        /// Sets projection type to perspective and specifies all values of perspective projection type.
        /// </summary>
        public void SetPerspective(float verticalFov, float nearClip, float farClip)
        {
            InternalCalls.CameraComponent_SetPerspective(Entity.ID, ref verticalFov, ref nearClip, ref farClip);
            ProjectionType type = ProjectionType.Perspective;
            InternalCalls.CameraComponent_SetProjectionType(Entity.ID, type);
        }

        /// <summary>
        /// Sets projection type to ortographic and specifies all values of ortographic projection type.
        /// </summary>
        public void SetOrthographic(float size, float nearClip, float farClip)
        {
            InternalCalls.CameraComponent_SetOrthographic(Entity.ID, ref size, ref nearClip, ref farClip);
            ProjectionType type = ProjectionType.Ortographic;
            InternalCalls.CameraComponent_SetProjectionType(Entity.ID, type);
        }

        /// <summary>
        /// Gets or sets the orthographic size of the camera.
        /// </summary>
        public float ortographicSize
        {
            get => InternalCalls.CameraComponent_GetOrthographicSize(Entity.ID);
            set => InternalCalls.CameraComponent_SetOrthographicSize(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the near clip plane distance of the camera in orthographic projection.
        /// </summary>
        public float ortographicNearClip
        {
            get => InternalCalls.CameraComponent_GetOrthographicNearClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetOrthographicNearClip(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the far clip plane distance of the camera in orthographic projection.
        /// </summary>
        public float ortographicFarClip
        {
            get => InternalCalls.CameraComponent_GetOrthographicFarClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetOrthographicFarClip(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the vertical field of view of the camera in perspective projection.
        /// </summary>
        public float perspectiveVerticalFov
        {
            get => InternalCalls.CameraComponent_GetPerspectiveVerticalFOV(Entity.ID);
            set => InternalCalls.CameraComponent_SetPerspectiveVerticalFOV(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the near clip plane distance of the camera in perspective projection.
        /// </summary>
        public float perspectiveNearClip
        {
            get => InternalCalls.CameraComponent_GetPerspectiveNearClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetPerspectiveNearClip(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the far clip plane distance of the camera in perspective projection.
        /// </summary>
        public float perspectiveFarClip
        {
            get => InternalCalls.CameraComponent_GetPerspectiveFarClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetPerspectiveFarClip(Entity.ID, ref value);
        }
    }

    public class RigidBody2DComponent : Component
    {
        /// <summary>
        /// Type of the body.
        /// </summary>
        /// <remarks>Static bodies do not move, dynamic bodies are affected by forces and kinematic bodies are moved by user.</remarks>
        public enum BodyType { Static = 0, Dynamic = 1, Kinematic = 2 }

        /// <summary>
        /// Retrieves linear velocity of the body as Vector2.
        /// </summary>
        public Vector2 linearVelocity
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 velocity);
                return velocity;
            }
            set => InternalCalls.RigidBody2DComponent_SetLinearVelocity(Entity.ID, ref value);
        }

        /// <summary>
        /// Type of the body.
        /// </summary>
        /// <remarks>Static bodies do not move, dynamic bodies are affected by forces and kinematic bodies are moved by user.</remarks>
        public BodyType type
        {
            get => InternalCalls.RigidBody2DComponent_GetType(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetType(Entity.ID, value);
        }

        //GravityMultiplier
        public float GravityMultiplier 
        {
            get => InternalCalls.RigidBody2DComponent_GetGravityMultiplier(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetGravityMultiplier(Entity.ID, value);
        }


        /// <summary>
        /// Initializes the runtime body of the RigidBody2D component.
        /// </summary>
        public void AwakeRuntimeBody()
        {
            InternalCalls.RigidBody2DComponent_AwakeRuntimeBody(Entity.ID);
        }

        /// <summary>
        /// Applies force to the body at given world position.
        /// </summary>
        /// <param name="impulse"></param>
        /// <param name="worldPosition"></param>
        /// <param name="wake"></param>
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        }

        /// <summary>
        /// Applies force to the body at center of mass.
        /// </summary>
        /// <param name="impulse"></param>
        /// <param name="wake"></param>
        public void ApplyLinearImpulse(Vector2 impulse, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }

        /// <summary>
        /// Specifies if the body can rotate.
        /// </summary>
        public bool fixedRotation
        {
            get => InternalCalls.RigidBody2DComponent_IsFixedRotation(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetFixedRotation(Entity.ID, ref value);
        }

        public bool enabled
        {
            get => InternalCalls.RigidBody2DComponent_IsEnabled(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetEnabled(Entity.ID, value);
        }

        public bool sleepingAllowed
        {
            get => InternalCalls.RigidBody2DComponent_IsSleepingAllowed(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetSleepingAllowed(Entity.ID, value);
        }

        public float angularDamping
        {
            get => InternalCalls.RigidBody2DComponent_GetAngularDamping(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetAngularDamping(Entity.ID, value);
        }

        public float angularVelocity
        {
            get => InternalCalls.RigidBody2DComponent_GetAngularVelocity(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetAngularVelocity(Entity.ID, value);
        }

        //public ? contactList
        //{
        //    get => InternalCalls.RigidBody2DComponent_GetContactList(Entity.ID);
        //}

        //public ? jointList
        //{
        //    get => InternalCalls.RigidBody2DComponent_GetJointList(Entity.ID);
        //}

        public float linearDamping
        {
            get => InternalCalls.RigidBody2DComponent_GetLinearDamping(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetLinearDamping(Entity.ID, value);
        }

        public Vector2 localPoint
        {
            get => InternalCalls.RigidBody2DComponent_GetLocalPoint(Entity.ID);
        }

        public Vector2 localVector
        {
            get => InternalCalls.RigidBody2DComponent_GetLocalVector(Entity.ID);
        }

        public float mass
        {
            get => InternalCalls.RigidBody2DComponent_GetMass(Entity.ID);
        }

        public float massData
        {
          get => InternalCalls.RigidBody2DComponent_GetMassData(Entity.ID);
          set => InternalCalls.RigidBody2DComponent_SetMassData(Entity.ID, value);
        }

        public Vector2 position 
        {
            get => InternalCalls.RigidBody2DComponent_GetPosition(Entity.ID);
        }

        public B2transform transform
        {
          get => InternalCalls.RigidBody2DComponent_GetTransform(Entity.ID);
          set => InternalCalls.RigidBody2DComponent_SetTransform(Entity.ID, value);
        }



        public Vector2 worldPoint
        {
            get => InternalCalls.RigidBody2DComponent_GetWorldPoint(Entity.ID);
        }

        public Vector2 worldVector
        {
            get => InternalCalls.RigidBody2DComponent_GetWorldVector(Entity.ID);
        }

        public bool awake
        {
            get => InternalCalls.RigidBody2DComponent_IsAwake(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetAwake(Entity.ID, value);
        }

        public bool bullet
        {
            get => InternalCalls.RigidBody2DComponent_IsBullet(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetBullet(Entity.ID, value);
        }

       // public ref Entity GetUserData()
        //{
         //   return InternalCalls.RigidBody2DComponent_GetUserData(Entity.ID);
        //}
        

        
        public void ApplyForce(Vector2 force, Vector2 point, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyForce(Entity.ID, ref force, ref point, wake);
        }

        public void ApplyForceToCenter(Vector2 force, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyForceToCenter(Entity.ID, ref force, wake);
        }

        public void ApplyTorque(float torque, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyTorque(Entity.ID, torque, wake);
        }


    }

    public enum Side
    {
        LEFT = 0, RIGHT, TOP, BOTTOM
    }

    public class BoxCollider2DComponent : Component
    {
        /// <summary>
        /// Size of the collider given in Vector2.
        /// </summary>
        public Vector2 size
        {
            get
            {
                InternalCalls.BoxCollider2DComponent_GetSize(Entity.ID, out Vector2 size);
                return size;
            }

            set => InternalCalls.BoxCollider2DComponent_SetSize(Entity.ID, ref value);
        }

        /// <summary>
        /// Offset of the collider given in Vector2.
        /// </summary>
        public Vector2 offset
        {
            get
            {
                InternalCalls.BoxCollider2DComponent_GetOffset(Entity.ID, out Vector2 offset);
                return offset;
            }

            set => InternalCalls.BoxCollider2DComponent_SetOffset(Entity.ID, ref value);
        }

        public float density
        {
            get => InternalCalls.BoxCollider2DComponent_GetDensity(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetDensity(Entity.ID, ref value);
        }

        public float friction
        {
            get => InternalCalls.BoxCollider2DComponent_GetFriction(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetFriction(Entity.ID, ref value);
        }

        public float restitution
        {
            get => InternalCalls.BoxCollider2DComponent_GetRestitution(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetRestitution(Entity.ID, ref value);
        }

        public float restitutionThreshold
        {
            get => InternalCalls.BoxCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetRestitutionThreshold(Entity.ID, ref value);
        }

        public bool isSensor
        {
            get => InternalCalls.BoxCollider2DComponent_IsSensor(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetSensor(Entity.ID, ref value);
        }

        public bool CollidesWith(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWith(Entity.ID, other.ID);
        }

        public bool CollidesWithBox(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithBox(Entity.ID, other.ID);
        }

        public bool CollidesWithCircle(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithCircle(Entity.ID, other.ID);
        }

        public bool CollidesWithCircle(Vector2 center, float radius)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithCircleCoords(Entity.ID, ref center, radius);
        }

        public bool CollidesWithPoint(Vector2 point)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithPoint(Entity.ID, ref point);
        }

        public bool CollidesWithEdge(Vector2 start, Vector2 end)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithEdgeCoords(Entity.ID, ref start, ref end);
        }

        public bool CollidesWithBottomEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithBottomEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithTopEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithTopEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithLeftEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithLeftEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithRightEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithRightEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithBox(Vector2 center, Vector2 size)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithBoxCoords(Entity.ID, ref center, ref size);
        }


        public bool CollidesWith(ulong entityID)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWith(Entity.ID, entityID);
        }

        bool CollidesWithEntitiesSide(Entity entity, Side side)
        {
            if (entity == null)
            {
                return false;
            }

            switch (side)
            {
                case Side.LEFT:
                    return this.CollidesWithLeftEdge(entity);
                case Side.RIGHT:
                    return this.CollidesWithRightEdge(entity);
                case Side.TOP:
                    return this.CollidesWithTopEdge(entity);
                case Side.BOTTOM:
                    return this.CollidesWithBottomEdge(entity);
            }

            return false;
        }

        bool CollidesWithEntity(string entityName)
        {
            Entity entity = Entity.FindEntityByName(entityName);
            if (entity == null)
            {
                return false;
            }

            return CollidesWith(entity);
        }
    }

    public class CircleCollider2DComponent : Component
    {
        public float offset
        {
            get => InternalCalls.CircleCollider2DComponent_GetOffset(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetOffset(Entity.ID, ref value);
        }

        public float density
        {
            get => InternalCalls.CircleCollider2DComponent_GetDensity(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetDensity(Entity.ID, ref value);
        }

        public float friction
        {
            get => InternalCalls.CircleCollider2DComponent_GetFriction(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetFriction(Entity.ID, ref value);
        }

        public float restitution
        {
            get => InternalCalls.CircleCollider2DComponent_GetRestitution(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRestitution(Entity.ID, ref value);
        }

        public float restitutionThreshold
        {
            get => InternalCalls.CircleCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRestitutionThreshold(Entity.ID, ref value);
        }

        public float radius
        {
            get => InternalCalls.CircleCollider2DComponent_GetRadius(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRadius(Entity.ID, ref value);
        }
        public bool isSensor
        {
            get => InternalCalls.CircleCollider2DComponent_IsSensor(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetSensor(Entity.ID, ref value);
        }

        public bool CollidesWith(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWith(Entity.ID, other.ID);
        }

        public bool CollidesWithCircle(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithCircle(Entity.ID, other.ID);
        }

        public bool CollidesWithCircle(Vector2 center, float radius)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithCircleCoords(Entity.ID, ref center, radius);
        }

        public bool CollidesWithBox(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithBox(Entity.ID, other.ID);
        }

        public bool CollidesWithBox(Vector2 center, Vector2 size)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithBoxCoords(Entity.ID, ref center, ref size);
        }

        public bool CollidesWithPoint(Vector2 point)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithPoint(Entity.ID, ref point);
        }

        public bool CollidesWithEntity(long entityID)
        {

            return InternalCalls.CircleCollider2DComponent_CollidesWith(Entity.ID, entityID);
        }

        public bool CollidesWithEntity(string entityName)
        {
            Entity entity = Entity.FindEntityByName(entityName);
            if (entity == null)
            {
                return false;
            }

            return CollidesWith(entity);
        }

        public bool CollidesWithEntitySide(long entityID, Side side)
        {
            Entity entity = Entity.FindEntityByID(entityID);
            if (entity == null)
            {
                return false;
            }

            switch (side)
            {
                case Side.LEFT:
                    return InternalCalls.CircleCollider2DComponent_CollidesWithLeftEdge(Entity.ID, entity.ID);
                case Side.RIGHT:
                    return InternalCalls.CircleCollider2DComponent_CollidesWithRightEdge(Entity.ID, entity.ID);
                case Side.TOP:
                    return InternalCalls.CircleCollider2DComponent_CollidesWithTopEdge(Entity.ID, entity.ID);
                case Side.BOTTOM:
                    return InternalCalls.CircleCollider2DComponent_CollidesWithBottomEdge(Entity.ID, entity.ID);
                default:
                    return false;
            }
        }

        public bool CollidesWithEntitiesLeftEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithLeftEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithEntitiesRightEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithRightEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithEntitiesTopEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithTopEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithEntitiesBottomEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithBottomEdge(Entity.ID, other.ID);
        }

        public bool CollidesWithEdge(Vector2 start, Vector2 end)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithEdgeCoords(Entity.ID, ref start, ref end);
        }

    }

    public class TextComponent : Component
    {
        /// <summary>
        /// Content of the text component.
        /// </summary>
        public string text
        {
            get => InternalCalls.TextComponent_GetText(Entity.ID);
            set => InternalCalls.TextComponent_SetText(Entity.ID, value);
        }

        /// <summary>
        /// Color of the text.
        /// </summary>
        /// <remarks>Color is given in RGBA format.</remarks>
        public Color color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(Entity.ID, out Vector4 color);
                return new Color(color);
            }

            set
            {
                Vector4 colorVec = Color.ToVector4(value);
                InternalCalls.TextComponent_SetColor(Entity.ID, ref colorVec);
            }
        }

        /// <summary>
        /// Distance between characters.
        /// </summary>
        public float kerning
        {
            get => InternalCalls.TextComponent_GetKerning(Entity.ID);
            set => InternalCalls.TextComponent_SetKerning(Entity.ID, value);
        }

        /// <summary>
        /// Distance between lines.
        /// </summary>
        public float lineSpacing
        {
            get => InternalCalls.TextComponent_GetLineSpacing(Entity.ID);
            set => InternalCalls.TextComponent_SetLineSpacing(Entity.ID, value);
        }

    }
}
