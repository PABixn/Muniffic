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

    /// <summary>
    /// Represents an animator component that controls animations for an entity.
    /// </summary>
    public class AnimatorComponent : Component
    {
        /// <summary>
        /// Gets or sets the speed of the animations.
        /// </summary>
        public float Speed
        {
            get => InternalCalls.AnimatorComponent_GetSpeed(Entity.ID);
            set => InternalCalls.AnimatorComponent_SetSpeed(Entity.ID, value);
        }

        /// <summary>
        /// Plays the specified animation.
        /// </summary>
        /// <param name="animationName">The name of the animation to play.</param>
        public void Play(string animationName)
        {
            InternalCalls.AnimatorComponent_PlayAnimation(Entity.ID, animationName);
        }

        /// <summary>
        /// Stops the currently playing animation.
        /// </summary>
        public void Stop()
        {
            InternalCalls.AnimatorComponent_StopAnimation(Entity.ID);
        }

        /// <summary>
        /// Pauses the currently playing animation.
        /// </summary>
        public void Pause()
        {
            InternalCalls.AnimatorComponent_PauseAnimation(Entity.ID);
        }

        /// <summary>
        /// Updates the animation with the specified delta time.
        /// </summary>
        /// <param name="deltaTime">The time elapsed since the last update.</param>
        public void Update(float deltaTime)
        {
            InternalCalls.AnimatorComponent_UpdateAnimation(Entity.ID, deltaTime);
        }

        /// <summary>
        /// Changes the current animation to the specified animation.
        /// </summary>
        /// <param name="animationName">The name of the animation to change to.</param>
        public void ChangeAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_ChangeAnimation(Entity.ID, animationName);
        }

        /// <summary>
        /// Adds a new animation with the specified name.
        /// </summary>
        /// <param name="animationName">The name of the animation to add.</param>
        public void AddAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_AddAnimation(Entity.ID, animationName);
        }

        /// <summary>
        /// Removes the animation with the specified name.
        /// </summary>
        /// <param name="animationName">The name of the animation to remove.</param>
        public void RemoveAnimation(string animationName)
        {
            InternalCalls.AnimatorComponent_RemoveAnimation(Entity.ID, animationName);
        }

        /// <summary>
        /// Removes the last added animation.
        /// </summary>
        public void RemoveLastAnimation()
        {
            InternalCalls.AnimatorComponent_RemoveLastAnimation(Entity.ID);
        }

        /// <summary>
        /// Transitions the animation to the specified animation.
        /// </summary>
        /// <param name="to">The name of the animation to transition to.</param>
        public void Transition(string to)
        {
            InternalCalls.AnimatorComponent_Transition(Entity.ID, to);
        }

        /// <summary>
        /// Transitions the animation to the animation at the specified index.
        /// </summary>
        /// <param name="index">The index of the animation to transition to.</param>
        public void TransitionByIndex(int index)
        {
            InternalCalls.AnimatorComponent_TransitionByIndex(Entity.ID, index);
        }

        /// <summary>
        /// Adds a transition between two animations.
        /// </summary>
        /// <param name="from">The name of the animation to transition from.</param>
        /// <param name="to">The name of the animation to transition to.</param>
        public void AddTransition(string from, string to)
        {
            InternalCalls.AnimatorComponent_AddTransition(Entity.ID, from, to);
        }

        /// <summary>
        /// Removes a transition between two animations.
        /// </summary>
        /// <param name="from">The name of the animation to transition from.</param>
        /// <param name="to">The name of the animation to transition to.</param>
        public void RemoveTransition(string from, string to)
        {
            InternalCalls.AnimatorComponent_RemoveTransition(Entity.ID, from, to);
        }

        /// <summary>
        /// Adds a transition between two animations using their indices.
        /// </summary>
        /// <param name="from">The index of the animation to transition from.</param>
        /// <param name="to">The index of the animation to transition to.</param>
        public void AddTransitionByIndex(int from, int to)
        {
            InternalCalls.AnimatorComponent_AddTransitionByIndex(Entity.ID, from, to);
        }

        /// <summary>
        /// Removes a transition between two animations using their indices.
        /// </summary>
        /// <param name="from">The index of the animation to transition from.</param>
        /// <param name="to">The index of the animation to transition to.</param>
        public void RemoveTransitionByIndex(int from, int to)
        {
            InternalCalls.AnimatorComponent_RemoveTransitionByIndex(Entity.ID, from, to);
        }

        /// <summary>
        /// Checks if a transition between two animations is possible.
        /// </summary>
        /// <param name="from">The name of the animation to transition from.</param>
        /// <param name="to">The name of the animation to transition to.</param>
        public void CanTransition(string from, string to)
        {
            InternalCalls.AnimatorComponent_CanTransition(Entity.ID, from, to);
        }

        /// <summary>
        /// Checks if a transition between two animations is possible using their indices.
        /// </summary>
        /// <param name="from">The index of the animation to transition from.</param>
        /// <param name="to">The index of the animation to transition to.</param>
        public void CanTransitionByIndex(int from, int to)
        {
            InternalCalls.AnimatorComponent_CanTransitionByIndex(Entity.ID, from, to);
        }

        /// <summary>
        /// Gets the name of the currently playing animation.
        /// </summary>
        /// <returns>The name of the currently playing animation.</returns>
        public string GetCurrentAnimation()
        {
            return InternalCalls.AnimatorComponent_GetCurrentAnimation(Entity.ID);
        }

        /// <summary>
        /// Sets the current animation to the specified animation.
        /// </summary>
        /// <param name="animationName">The name of the animation to set as current.</param>
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
    }

    public enum Side {
            LEFT = 0, RIGHT, TOP, BOTTOM
        }

    /// <summary>
    /// Represents a 2D box collider component.
    /// </summary>
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

        /// <summary>
        /// Density of the collider.
        /// </summary>
        public float density
        {
            get => InternalCalls.BoxCollider2DComponent_GetDensity(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetDensity(Entity.ID, ref value);
        }

        /// <summary>
        /// Friction of the collider.
        /// </summary>
        public float friction
        {
            get => InternalCalls.BoxCollider2DComponent_GetFriction(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetFriction(Entity.ID, ref value);
        }

        /// <summary>
        /// Restitution of the collider.
        /// </summary>
        public float restitution
        {
            get => InternalCalls.BoxCollider2DComponent_GetRestitution(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetRestitution(Entity.ID, ref value);
        }

        /// <summary>
        /// Restitution threshold of the collider.
        /// </summary>
        public float restitutionThreshold
        {
            get => InternalCalls.BoxCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetRestitutionThreshold(Entity.ID, ref value);
        }

        /// <summary>
        /// Checks if the collider collides with the specified entity.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the collider collides with the entity, otherwise false.</returns>
        public bool CollidesWith(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWith(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the specified box collider entity.
        /// </summary>
        /// <param name="other">The box collider entity to check collision with.</param>
        /// <returns>True if the collider collides with the box collider entity, otherwise false.</returns>
        public bool CollidesWithBox(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithBox(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the specified circle collider entity.
        /// </summary>
        /// <param name="other">The circle collider entity to check collision with.</param>
        /// <returns>True if the collider collides with the circle collider entity, otherwise false.</returns>
        public bool CollidesWithCircle(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithCircle(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the specified circle defined by center and radius.
        /// </summary>
        /// <param name="center">The center of the circle.</param>
        /// <param name="radius">The radius of the circle.</param>
        /// <returns>True if the collider collides with the circle, otherwise false.</returns>
        public bool CollidesWithCircle(Vector2 center, float radius)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithCircleCoords(Entity.ID, ref center, radius);
        }

        /// <summary>
        /// Checks if the collider collides with the specified point.
        /// </summary>
        /// <param name="point">The point to check collision with.</param>
        /// <returns>True if the collider collides with the point, otherwise false.</returns>
        public bool CollidesWithPoint(Vector2 point)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithPoint(Entity.ID, ref point);
        }

        /// <summary>
        /// Checks if the collider collides with the specified edge defined by start and end points.
        /// </summary>
        /// <param name="start">The start point of the edge.</param>
        /// <param name="end">The end point of the edge.</param>
        /// <returns>True if the collider collides with the edge, otherwise false.</returns>
        public bool CollidesWithEdge(Vector2 start, Vector2 end)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithEdgeCoords(Entity.ID, ref start, ref end);
        }

        /// <summary>
        /// Checks if the collider collides with the bottom edge of the specified entity.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the collider collides with the bottom edge of the entity, otherwise false.</returns>
        public bool CollidesWithBottomEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithBottomEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the top edge of the specified entity.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the collider collides with the top edge of the entity, otherwise false.</returns>
        public bool CollidesWithTopEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithTopEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the left edge of the specified entity.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the collider collides with the left edge of the entity, otherwise false.</returns>
        public bool CollidesWithLeftEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithLeftEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the right edge of the specified entity.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the collider collides with the right edge of the entity, otherwise false.</returns>
        public bool CollidesWithRightEdge(Entity other)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithRightEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the collider collides with the specified box defined by center and size.
        /// </summary>
        /// <param name="center">The center of the box.</param>
        /// <param name="size">The size of the box.</param>
        /// <returns>True if the collider collides with the box, otherwise false.</returns>
        public bool CollidesWithBox(Vector2 center, Vector2 size)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWithBoxCoords(Entity.ID, ref center, ref size);
        }


        /// <summary>
        /// Checks if the collider collides with the specified entity.
        /// </summary>
        /// <param name="entityID">The ID of the entity to check collision with.</param>
        /// <returns>True if the collider collides with the entity, otherwise false.</returns>
        public bool CollidesWith(long entityID)
        {
            return InternalCalls.BoxCollider2DComponent_CollidesWith(Entity.ID, entityID);
        }

        /// <summary>
        /// Checks if the collider collides with the specified side of the specified entity.
        /// </summary>
        /// <param name="entity">The entity to check collision with.</param>
        /// <param name="side">The side to check collision with.</param>
        /// <returns>True if the collider collides with the specified side of the entity, otherwise false.</returns>
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

        /// <summary>
        /// Checks if the collider collides with the specified entity.
        /// </summary>
        /// <param name="entityName">The name of the entity to check collision with.</param>
        /// <returns>True if the collider collides with the entity, otherwise false.</returns>
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

    /// <summary>
    /// Represents a circle collider component for 2D entities.
    /// </summary>
    public class CircleCollider2DComponent : Component
    {
        /// <summary>
        /// Gets or sets the offset of the circle collider.
        /// </summary>
        public float offset
        {
            get => InternalCalls.CircleCollider2DComponent_GetOffset(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetOffset(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the density of the circle collider.
        /// </summary>
        public float density
        {
            get => InternalCalls.CircleCollider2DComponent_GetDensity(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetDensity(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the friction of the circle collider.
        /// </summary>
        public float friction
        {
            get => InternalCalls.CircleCollider2DComponent_GetFriction(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetFriction(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the restitution of the circle collider.
        /// </summary>
        public float restitution
        {
            get => InternalCalls.CircleCollider2DComponent_GetRestitution(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRestitution(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the restitution threshold of the circle collider.
        /// </summary>
        public float restitutionThreshold
        {
            get => InternalCalls.CircleCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRestitutionThreshold(Entity.ID, ref value);
        }

        /// <summary>
        /// Gets or sets the radius of the circle collider.
        /// </summary>
        public float radius
        {
            get => InternalCalls.CircleCollider2DComponent_GetRadius(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRadius(Entity.ID, ref value);
        }

        /// <summary>
        /// Checks if the circle collider collides with the specified entity.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the circle collider collides with the entity, otherwise false.</returns>
        public bool CollidesWith(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWith(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with another circle collider.
        /// </summary>
        /// <param name="other">The other circle collider to check collision with.</param>
        /// <returns>True if the circle collider collides with the other circle collider, otherwise false.</returns>
        public bool CollidesWithCircle(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithCircle(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with a circle defined by the center and radius.
        /// </summary>
        /// <param name="center">The center of the circle.</param>
        /// <param name="radius">The radius of the circle.</param>
        /// <returns>True if the circle collider collides with the circle, otherwise false.</returns>
        public bool CollidesWithCircle(Vector2 center, float radius)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithCircleCoords(Entity.ID, ref center, radius);
        }

        /// <summary>
        /// Checks if the circle collider collides with a box collider.
        /// </summary>
        /// <param name="other">The box collider to check collision with.</param>
        /// <returns>True if the circle collider collides with the box collider, otherwise false.</returns>
        public bool CollidesWithBox(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithBox(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with a box defined by the center and size.
        /// </summary>
        /// <param name="center">The center of the box.</param>
        /// <param name="size">The size of the box.</param>
        /// <returns>True if the circle collider collides with the box, otherwise false.</returns>
        public bool CollidesWithBox(Vector2 center, Vector2 size)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithBoxCoords(Entity.ID, ref center, ref size);
        }

        /// <summary>
        /// Checks if the circle collider collides with a point.
        /// </summary>
        /// <param name="point">The point to check collision with.</param>
        /// <returns>True if the circle collider collides with the point, otherwise false.</returns>
        public bool CollidesWithPoint(Vector2 point)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithPoint(Entity.ID, ref point);
        }

        /// <summary>
        /// Checks if the circle collider collides with an entity with the specified ID.
        /// </summary>
        /// <param name="entityID">The ID of the entity to check collision with.</param>
        /// <returns>True if the circle collider collides with the entity, otherwise false.</returns>
        public bool CollidesWithEntity(long entityID)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWith(Entity.ID, entityID);
        }

        /// <summary>
        /// Checks if the circle collider collides with an entity with the specified name.
        /// </summary>
        /// <param name="entityName">The name of the entity to check collision with.</param>
        /// <returns>True if the circle collider collides with the entity, otherwise false.</returns>
        public bool CollidesWithEntity(string entityName)
        {
            Entity entity = Entity.FindEntityByName(entityName);
            if (entity == null)
            {
                return false;
            }

            return CollidesWith(entity);
        }

        /// <summary>
        /// Checks if the circle collider collides with an entity on the specified side.
        /// </summary>
        /// <param name="entityID">The ID of the entity to check collision with.</param>
        /// <param name="side">The side to check collision with.</param>
        /// <returns>True if the circle collider collides with the entity on the specified side, otherwise false.</returns>
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

        /// <summary>
        /// Checks if the circle collider collides with entities on the left edge.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the circle collider collides with entities on the left edge, otherwise false.</returns>
        public bool CollidesWithEntitiesLeftEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithLeftEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with entities on the right edge.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the circle collider collides with entities on the right edge, otherwise false.</returns>
        public bool CollidesWithEntitiesRightEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithRightEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with entities on the top edge.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the circle collider collides with entities on the top edge, otherwise false.</returns>
        public bool CollidesWithEntitiesTopEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithTopEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with entities on the bottom edge.
        /// </summary>
        /// <param name="other">The entity to check collision with.</param>
        /// <returns>True if the circle collider collides with entities on the bottom edge, otherwise false.</returns>
        public bool CollidesWithEntitiesBottomEdge(Entity other)
        {
            return InternalCalls.CircleCollider2DComponent_CollidesWithBottomEdge(Entity.ID, other.ID);
        }

        /// <summary>
        /// Checks if the circle collider collides with an edge defined by the start and end points.
        /// </summary>
        /// <param name="start">The start point of the edge.</param>
        /// <param name="end">The end point of the edge.</param>
        /// <returns>True if the circle collider collides with the edge, otherwise false.</returns>
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
