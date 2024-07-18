﻿//Entities

/// <summary>
/// Base class for all scripts, from which all scripts should be derived.
/// </summary>
/// <summary>
/// Base class for all scripts, from which all scripts should be derived.
/// </summary>
public class DefaultBehaviour
{
    /// <summary>
    /// The entity this script is attached to.
    /// </summary>
    public Entity entity;

    protected DefaultBehaviour()
    {
        entity = new Entity(0);
    }

    internal DefaultBehaviour(long id)
    {
        entity = new Entity(id);
    }

    /// <summary>
    /// Checks if entity has given component;
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    /// <returns>true if entity has the component and false if not.</returns>
    ///<remarks>Component must be derived from Component class.</remarks>
    public bool HasComponent<T>() where T : Component, new()
    {
        Type componentType = typeof(T);
        return InternalCalls.Entity_HasComponent(entity.ID, componentType);
    }

    /// <summary>
    /// Retrieves component of given type from entity.
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    /// <returns>Component reference or null if entity does not have component.</returns>
    ///<remarks>Component must be derived from Component class.</remarks>
    public T GetComponent<T>() where T : Component, new()
    {
        if (!HasComponent<T>())
        {
            return null;
        }

        T component = new T() { Entity = entity };
        return component;
    }

    /// <summary>
    /// Adds component of given type to entity.
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    /// <returns>Component reference or null if component has not been added to the entity.</returns>
    public T AddComponent<T>() where T : Component, new()
    {
        Type componentType = typeof(T);
        InternalCalls.Entity_AddComponent(entity.ID, componentType);
        return GetComponent<T>();
    }

    /// <summary>
    /// Removes the specified component from the entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    public void RemoveComponent<T>() where T : Component, new()
    {
        Type componentType = typeof(T);
        InternalCalls.Entity_RemoveComponent(entity.ID, componentType);
    }
}

public struct Entity
{
    /// <summary>
    /// UUID of the entity, given by the engine at creation.
    /// </summary>
    public long ID;

    public Entity(long id)
    {
        this.ID = id;
    }

    #region Operators
    public static bool operator ==(Entity a, Entity b)
    {
        return a.ID == b.ID;
    }

    public static bool operator !=(Entity a, Entity b)
    {
        return a.ID != b.ID;
    }

    public static bool operator ==(Entity a, long b)
    {
        return a.ID == b;
    }

    public static bool operator !=(Entity a, long b)
    {
        return a.ID != b;
    }

    public static bool operator ==(long a, Entity b)
    {
        return a == b.ID;
    }

    public static bool operator !=(long a, Entity b)
    {
        return a != b.ID;
    }
    #endregion

    #region Instance

    /// <summary>
    /// Gets or sets the name of the entity.
    /// </summary>
    public string name
    {
        get
        {
            return InternalCalls.Entity_GetName(ID);
        }
        set
        {
            InternalCalls.Entity_SetName(ID, value);
        }
    }

    /// <summary>
    /// Gets or sets the parent entity of the current entity.
    /// </summary>
    public Entity parent
    {
        get
        {
            long parentID = InternalCalls.Entity_GetParent(ID);
            if (parentID == 0)
                return new Entity(0);

            return new Entity(parentID);
        }
        set
        {
            InternalCalls.Entity_SetParent(ID, value.ID);
        }
    }

    /// <summary>
    /// Checks if the specified component is inherited in any level of the children entities of the current entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    /// <returns>true if the component is inherited in any level of the children entities; otherwise, false.</returns>
    public bool IsInheritedInChildren<T>() where T : Component, new()
    {
        return InternalCalls.Entity_IsInheritedInChildren(ID, typeof(T));
    }

    /// <summary>
    /// Checks if the specified component is inherited from the parent entity of the current entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    /// <returns>true if the component is inherited from the parent entity; otherwise, false.</returns>
    public bool IsInheritedFromParent<T>() where T : Component, new()
    {
        return InternalCalls.Entity_IsInheritedFromParent(ID, typeof(T));
    }

    /// <summary>
    /// Stops inheriting the specified component from the parent entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    public void StopInheritingComponent<T>() where T : Component, new()
    {
        InternalCalls.Entity_InheritComponent(ID, typeof(T), true);
    }

    /// <summary>
    /// Inherits the specified component from the parent entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    public void InheritComponent<T>() where T : Component, new()
    {
        InternalCalls.Entity_InheritComponent(ID, typeof(T), false);
    }

    /// <summary>
    /// Sets the parent entity of the current entity.
    /// </summary>
    /// <param name="entity">The UUID of the parent entity.</param>
    public void SetParent(long entity)
    {
        InternalCalls.Entity_SetParent(ID, entity);
    }

    /// <summary>
    /// Checks if the param entity is any level child of the current entity.
    /// </summary>
    /// <param name="entity">The entity to check.</param>
    /// <returns>true if the current entity is a child of the specified entity; otherwise, false.</returns>
    public bool IsChildOfAny(Entity entity)
    {
        return InternalCalls.Entity_IsChildOfAny(ID, entity.ID);
    }

    /// <summary>
    /// Checks if the param entity is any level child of the current entity.
    /// </summary>
    /// <param name="entity">The entity to check.</param>
    /// <returns>true if the current entity is a child of the specified entity; otherwise, false.</returns>
    public bool IsChildOfAny(long entity)
    {
        return InternalCalls.Entity_IsChildOfAny(ID, entity);
    }

    /// <summary>
    /// Checks if the param entity is first level child of the current entity.
    /// </summary>
    /// <param name="entity">The UUID of the entity to check.</param>
    /// <returns>true if the current entity is a child of the specified entity; otherwise, false.</returns>
    public bool IsChild(Entity entity)
    {
        return InternalCalls.Entity_IsChild(ID, entity.ID);
    }

    /// <summary>
    /// Checks if the current entity is a child of the specified entity.
    /// </summary>
    /// <param name="entity">The UUID of the entity to check.</param>
    /// <returns>true if the current entity is a child of the specified entity; otherwise, false.</returns>
    public bool IsChild(long entity)
    {
        return InternalCalls.Entity_IsChild(ID, entity);
    }

    /// <summary>
    /// Adds the specified entity as a child of the current entity.
    /// </summary>
    /// <param name="entity">The entity to add as a child.</param>
    public void AddChild(Entity entity)
    {
        InternalCalls.Entity_AddChild(ID, entity.ID);
    }

    /// <summary>
    /// Adds the specified entity as a child of the current entity.
    /// </summary>
    /// <param name="entity">The UUID of the entity to add as a child.</param>
    public void AddChild(long entity)
    {
        InternalCalls.Entity_AddChild(ID, entity);
    }

    /// <summary>
    /// Removes the specified entity as a child of the current entity.
    /// </summary>
    /// <param name="entity">The entity to remove as a child.</param>
    public void RemoveChild(Entity entity)
    {
        InternalCalls.Entity_RemoveChild(ID, entity.ID);
    }

    /// <summary>
    /// Removes the specified entity as a child of the current entity.
    /// </summary>
    /// <param name="entity">The UUID of the entity to remove as a child.</param>
    public void RemoveChild(long entity)
    {
        InternalCalls.Entity_RemoveChild(ID, entity);
    }

    /// <summary>
    /// Removes all children entities of the current entity.
    /// </summary>
    public void RemoveAnyChildren()
    {
        InternalCalls.Entity_RemoveAnyChildren(ID);
    }

    /// <summary>
    /// Copies the component values of the specified type to all children entities of the current entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    public void CopyComponentValuesToChildren<T>() where T : Component, new()
    {
        InternalCalls.Entity_CopyComponentValuesToChildren(ID, typeof(T));
    }

    /// <summary>
    /// Removes the component of the specified type from all children entities of the current entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    public void RemoveComponentFromChildren<T>() where T : Component, new()
    {
        InternalCalls.Entity_CopyComponentToChildren(ID, typeof(T), true);
    }

    /// <summary>
    /// Copies the component of the specified type to all children entities of the current entity.
    /// </summary>
    /// <typeparam name="T">The type of the component.</typeparam>
    public void CopyComponentToChildren<T>() where T : Component, new()
    {
        InternalCalls.Entity_CopyComponentToChildren(ID, typeof(T), false);
    }

    /// <summary>
    /// Retrieves all children entities of the current entity, including the children of the children.
    /// </summary>
    /// <returns>A list of Entity objects representing the children entities.</returns>
    public List<Entity> GetAnyChildren()
    {
        IntPtr ptr = InternalCalls.Entity_GetAnyChildren(ID, out int size);

        long[] managedArray = new long[size];

        Marshal.Copy(ptr, managedArray, 0, size);

        List<Entity> entities = new List<Entity>();

        foreach (long entityID in managedArray)
        {
            entities.Add(new Entity(entityID));
        }

        return entities;
    }

    /// <summary>
    /// Retrieves all direct children entities of the current entity, excluding the children of the children.
    /// </summary>
    /// <returns>A list of Entity objects representing the children entities.</returns>
    public List<Entity> GetChildren()
    {
        IntPtr ptr = InternalCalls.Entity_GetChildren(ID, out int size);

        long[] managedArray = new long[size];

        Marshal.Copy(ptr, managedArray, 0, size);

        List<Entity> entities = new List<Entity>();

        foreach (long entityID in managedArray)
        {
            entities.Add(new Entity(entityID));
        }

        return entities;
    }

    /// <summary>
    /// Retrieves script instance of given type (class) from entity.
    /// </summary>
    /// <typeparam name="T">Script class.</typeparam>
    /// <returns>Script instance as an object.</returns>
    public T As<T>() where T : DefaultBehaviour, new()
    {
        object instance = InternalCalls.Entity_GetScriptInstance(ID);
        return instance as T;
    }

    /// <summary>
    /// Checks if entity has given component;
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    /// <returns>true if entity has the component and false if not.</returns>
    ///<remarks>Component must be derived from Component class.</remarks>
    public bool HasComponent<T>() where T : Component, new()
    {
        Type componentType = typeof(T);
        return InternalCalls.Entity_HasComponent(ID, componentType);
    }

    /// <summary>
    /// Retrieves component of given type from entity.
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    /// <returns>Component reference or null if entity does not have component.</returns>
    ///<remarks>Component must be derived from Component class.</remarks>
    public T GetComponent<T>() where T : Component, new()
    {
        if (!HasComponent<T>())
        {
            return null;
        }

        T component = new T() { Entity = this };
        return component;
    }

    /// <summary>
    /// Adds component of given type to entity.
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    /// <returns>Component reference or null if component has not been added to the entity.</returns>
    public T AddComponent<T>() where T : Component, new()
    {
        Type componentType = typeof(T);
        InternalCalls.Entity_AddComponent(ID, componentType);

        return GetComponent<T>();
    }

    /// <summary>
    /// Removes component of given type from entity.
    /// </summary>
    /// <typeparam name="T">Component type.</typeparam>
    public void RemoveComponent<T>() where T : Component, new()
    {
        Type componentType = typeof(T);
        InternalCalls.Entity_RemoveComponent(ID, componentType);
    }

    #endregion

    #region Static

    /// <summary>
    /// Checks if the entity with the specified ID exists.
    /// </summary>
    /// <param name="ID">The ID of the entity.</param>
    /// <returns>true if the entity exists; otherwise, false.</returns>
    public static bool Exists(long ID)
    {
        return InternalCalls.Entity_Exists(ID);
    }

    /// <summary>
    /// Retrieves entity from scene by ID.
    /// </summary>
    /// <param name="ID">The ID of the entity.</param>
    /// <returns>The entity with the specified ID, or null if the entity does not exist.</returns>
    public static Entity FindEntityByID(long ID)
    {
        if (!Exists(ID))
            return new Entity(0);

        return new Entity(ID);
    }

    /// <summary>
    /// Retrieves entity from scene by name.
    /// </summary>
    /// <param name="name">Entity name.</param>
    /// <returns>Entity reference or null if entity has not been found.</returns>
    public static Entity FindEntityByName(string name)
    {
        long entityID = InternalCalls.Entity_FindEntityByName(name);
        if (entityID == 0)
            return new Entity(0);

        return new Entity(entityID);
    }

    /// <summary>
    /// Creates new entity.
    /// </summary>
    /// <param name="name">Entity name.</param>
    /// <returns>Entity reference or null if entity has not been created.</returns>
    public static Entity Create(string name)
    {
        long entityID = InternalCalls.Entity_Create(name);
        if (entityID == 0)
            return new Entity(0);

        return new Entity(entityID);
    }

    /// <summary>
    /// Destroys entity.
    /// </summary>
    /// <param name="entity">Reference to the entity that should be destroyed.</param>
    /// <remarks>Only UUID of the entity is used in this method.</remarks>
    public static void Destroy(Entity entity)
    {
        InternalCalls.Entity_Destroy(entity.ID);
    }

    /// <summary>
    /// Destroys the entity with the specified name.
    /// </summary>
    /// <param name="name">The name of the entity to destroy.</param>
    public static void Destroy(string name)
    {
        long id = InternalCalls.Entity_FindEntityByName(name);
        InternalCalls.Entity_Destroy(id);
    }

    /// <summary>
    /// Destroys entity.
    /// </summary>
    /// <param name="ID">UUID of the entity that should be destroyed.</param>
    public static void Destroy(long ID)
    {
        InternalCalls.Entity_Destroy(ID);
    }

    #endregion
}


//Components

/// <summary>
/// Base class for all components.
/// </summary>
public abstract class Component
{
    /// <summary>
    /// Entity the component is attached to.
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
    /// Translation (position) of the entity relative to point (0,0,0).
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
    public float speed
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
    public enum ProjectionType { Perspective = 0, Orthographic = 1 }

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
        ProjectionType type = ProjectionType.Orthographic;
        InternalCalls.CameraComponent_SetProjectionType(Entity.ID, type);
    }

    /// <summary>
    /// Gets or sets the orthographic size of the camera.
    /// </summary>
    public float orthographicSize
    {
        get => InternalCalls.CameraComponent_GetOrthographicSize(Entity.ID);
        set => InternalCalls.CameraComponent_SetOrthographicSize(Entity.ID, ref value);
    }

    /// <summary>
    /// Gets or sets the near clip plane distance of the camera in orthographic projection.
    /// </summary>
    public float orthographicNearClip
    {
        get => InternalCalls.CameraComponent_GetOrthographicNearClip(Entity.ID);
        set => InternalCalls.CameraComponent_SetOrthographicNearClip(Entity.ID, ref value);
    }

    /// <summary>
    /// Gets or sets the far clip plane distance of the camera in orthographic projection.
    /// </summary>
    public float orthographicFarClip
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

public enum Side
{
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
    public bool CollidesWithEntitiesSide(Entity entity, Side side)
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
    public bool CollidesWithEntity(string entityName)
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
    public bool CollidesWithEntitiesSide(long entityID, Side side)
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


//Collisions

/// <summary>
/// Represents a 2D collision event.
/// </summary>
public struct Collision2D
{
    /// <summary>
    /// Initializes a new instance of the <see cref="Collision2D"/> struct.
    /// </summary>
    /// <param name="entityID">The ID of the other entity involved in the collision.</param>
    /// <param name="points">The contact points of the collision.</param>
    /// <param name="friction">The friction coefficient of the collision.</param>
    /// <param name="restitution">The restitution coefficient of the collision.</param>
    /// <param name="tangentSpeed">The tangent speed of the collision.</param>
    public Collision2D(long entityID, Vector2 points, float friction, float restitution, float tangentSpeed)
    {
        this.otherEntity = new Entity(entityID);
        this.contactPoints = points;
        this.friction = friction;
        this.restitution = restitution;
        this.tangentSpeed = tangentSpeed;
    }

    /// <summary>
    /// Gets or sets the other entity involved in the collision.
    /// </summary>
    public Entity otherEntity;

    /// <summary>
    /// Gets or sets the contact points of the collision.
    /// </summary>
    public Vector2 contactPoints;

    /// <summary>
    /// Gets or sets the friction coefficient of the collision.
    /// </summary>
    public float friction;

    /// <summary>
    /// Gets or sets the restitution coefficient of the collision.
    /// </summary>
    public float restitution;

    /// <summary>
    /// Gets or sets the tangent speed of the collision.
    /// </summary>
    public float tangentSpeed;
}


//Input system

public class Input
{
    /// <summary>
    /// Checks if given key is pressed.
    /// </summary>
    /// <param name="key">Code of the key.</param>
    /// <returns>true if the key is currently pressed and false if not.</returns>
    public static bool IsKeyDown(KeyCode key)
    {
        return InternalCalls.Input_IsKeyDown(key);
    }
}

public enum KeyCode
{

    // From glfw3.h
    Space = 32,
    Apostrophe = 39, /* ' */
    Comma = 44, /* , */
    Minus = 45, /* - */
    Period = 46, /* . */
    Slash = 47, /* / */

    D0 = 48, /* 0 */
    D1 = 49, /* 1 */
    D2 = 50, /* 2 */
    D3 = 51, /* 3 */
    D4 = 52, /* 4 */
    D5 = 53, /* 5 */
    D6 = 54, /* 6 */
    D7 = 55, /* 7 */
    D8 = 56, /* 8 */
    D9 = 57, /* 9 */

    Semicolon = 59, /* ; */
    Equal = 61, /* = */

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    LeftBracket = 91,  /* [ */
    Backslash = 92,  /* \ */
    RightBracket = 93,  /* ] */
    GraveAccent = 96,  /* ` */

    World1 = 161, /* non-US #1 */
    World2 = 162, /* non-US #2 */

    /* Function keys */
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,

    /* Keypad */
    KP0 = 320,
    KP1 = 321,
    KP2 = 322,
    KP3 = 323,
    KP4 = 324,
    KP5 = 325,
    KP6 = 326,
    KP7 = 327,
    KP8 = 328,
    KP9 = 329,
    KPDecimal = 330,
    KPDivide = 331,
    KPMultiply = 332,
    KPSubtract = 333,
    KPAdd = 334,
    KPEnter = 335,
    KPEqual = 336,

    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348
}


//Colors

/// <summary>
/// Color in default RGBA Color format.
/// </summary>
public class Color
{
    public float r, g, b, alpha;

    #region Constructors

    /// <summary>
    /// Creates new Color with RGB values and sets alpha to 1.
    /// </summary>
    public Color(float red, float green, float blue)
    {
        red = red > 1 ? 1 : red < 0 ? 0 : red;
        green = green > 1 ? 1 : green < 0 ? 0 : green;
        blue = blue > 1 ? 1 : blue < 0 ? 0 : blue;

        this.r = red;
        this.g = green;
        this.b = blue;
        alpha = 1f;
    }

    /// <summary>
    /// Creates new Color with RGB values and specified alpha.
    /// </summary>
    public Color(float red, float green, float blue, float alpha)
    {
        red = red > 1 ? 1 : red < 0 ? 0 : red;
        green = green > 1 ? 1 : green < 0 ? 0 : green;
        blue = blue > 1 ? 1 : blue < 0 ? 0 : blue;
        alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

        this.r = red;
        this.g = green;
        this.b = blue;
        this.alpha = alpha;
    }

    /// <summary>
    /// Creates new Color based on Vector4.
    /// </summary>
    /// <param name="color">Vector3 with corresponding RGBA values.</param>
    public Color(Vector4 color)
    {
        r = color.X;
        g = color.Y;
        b = color.Z;
        alpha = color.W;
    }

    /// <summary>
    /// Creates new Color based on Vector3 and sets alpha to 1.
    /// </summary>
    /// <param name="color">Vector3 with corresponding RGB values.</param>
    public Color(Vector3 color)
    {
        r = color.X;
        g = color.Y;
        b = color.Z;
        alpha = 1f;
    }

    /// <summary>
    /// Creates new Color from hex string.
    /// </summary>
    /// <param name="hex">String with color in hex format.</param>
    /// <remarks>Giving 6 digit hex will result in alpha being set to 1.</remarks>
    public Color(string hex)
    {
        try
        {
            if (hex.StartsWith("#"))
            {
                hex = hex.Substring(1);
            }

            if (hex.Length == 6)
            {
                r = int.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                g = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                b = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                alpha = 1f;
            }
            else if (hex.Length == 8)
            {
                r = int.Parse(hex.Substring(0, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                g = int.Parse(hex.Substring(2, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                b = int.Parse(hex.Substring(4, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
                alpha = int.Parse(hex.Substring(6, 2), System.Globalization.NumberStyles.HexNumber) / 255f;
            }
            else
            {
                r = 0.0f;
                g = 0.0f;
                b = 0.0f;
                alpha = 0.0f; //error
            }
        }
        catch (System.FormatException)
        {
            r = 0.0f;
            g = 0.0f;
            b = 0.0f;
            alpha = 0.0f; //error
        }
    }

    #endregion

    /// <summary>
    /// Creates new Color with all values set to 0.
    /// </summary>
    public static Color Zero()
    {
        return new Color(0, 0, 0, 0);
    }

    /// <summary>
    /// Converts Color to hex string.
    /// </summary>
    /// <param name="color">Color in default RGBA color format.</param>
    /// <returns>String with color in hex format.</returns>
    public static string ToHex(Color color)
    {
        return string.Format("#{0:X2}{1:X2}{2:X2}{3:X2}", (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255), (int)(color.alpha * 255));
    }

    /// <summary>
    /// Converts Color to Vector4.
    /// </summary>
    /// <param name="color">Color in default RGBA color format.</param>
    /// <returns>Vector4 with corresponding RGBA values.</returns>
    public static Vector4 ToVector4(Color color)
    {
        return new Vector4(color.r, color.g, color.b, color.alpha);
    }

    /// <summary>
    /// Converts Color to Vector3.
    /// </summary>
    /// <param name="color">Color in default RGBA color format.</param>
    /// <returns>Vector4 with corresponding RGB values.</returns>
    public static Vector3 ToVector3(Color color)
    {
        return new Vector3(color.r, color.g, color.b);
    }

    #region Operators

    public static Color operator +(Color a, Color b)
    {
        return new Color(a.r + b.r, a.g + b.g, a.b + b.b, a.alpha + b.alpha);
    }

    public static Color operator -(Color a, Color b)
    {
        return new Color(a.r - b.r, a.g - b.g, a.b - b.b, a.alpha - b.alpha);
    }

    public static Color operator *(Color a, Color b)
    {
        return new Color(a.r * b.r, a.g * b.g, a.b * b.b, a.alpha * b.alpha);
    }

    public static Color operator /(Color a, Color b)
    {
        return new Color(a.r / b.r, a.g / b.g, a.b / b.b, a.alpha / b.alpha);
    }

    public static bool operator ==(Color a, Color b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.alpha == b.alpha;
    }

    public static bool operator !=(Color a, Color b)
    {
        return a.r != b.r || a.g != b.g || a.b != b.b || a.alpha != b.alpha;
    }

    public override string ToString()
    {
        return string.Format("Red: {0}, Green: {1}, Blue: {2}, Alpha: {3}", red, green, blue, alpha);

    }

    public override bool Equals(object obj)
    {
        if (obj == null)
        {
            return false;
        }

        Color color = obj as Color;
        if ((object)color == null)
        {
            return false;
        }

        return r == color.r && g == color.g && b == color.b && alpha == color.alpha;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    #endregion

    #region Named Colors

    public static Color white { get => new Color(1f, 1f, 1f); }
    public static Color black { get => new Color(0f, 0f, 0f); }
    public static Color red { get => new Color(1f, 0f, 0f); }
    public static Color green { get => new Color(0f, 1f, 0f); }
    public static Color blue { get => new Color(0f, 0f, 1f); }
    public static Color yellow { get => new Color(1f, 1f, 0f); }
    public static Color cyan { get => new Color(0f, 1f, 1f); }
    public static Color magenta { get => new Color(1f, 0f, 1f); }
    public static Color gray { get => new Color(0.5f, 0.5f, 0.5f); }
    public static Color silver { get => new Color(0.75f, 0.75f, 0.75f); }
    public static Color maroon { get => new Color(0.5f, 0f, 0f); }
    public static Color olive { get => new Color(0.5f, 0.5f, 0f); }
    public static Color lime { get => new Color(0f, 1f, 0f); }
    public static Color aqua { get => new Color(0f, 1f, 1f); }
    public static Color teal { get => new Color(0f, 0.5f, 0.5f); }
    public static Color navy { get => new Color(0f, 0f, 0.5f); }
    public static Color fuchsia { get => new Color(1f, 0f, 1f); }
    public static Color purple { get => new Color(0.5f, 0f, 0.5f); }
    public static Color orange { get => new Color(1f, 0.647f, 0f); }
    public static Color brown { get => new Color(0.647f, 0.165f, 0.165f); }
    public static Color darkGray { get => new Color(0.3f, 0.3f, 0.3f); }
    public static Color mediumGray { get => new Color(0.6f, 0.6f, 0.6f); }
    public static Color lightGray { get => new Color(0.8f, 0.8f, 0.8f); }
    public static Color violet { get => new Color(0.933f, 0.509f, 0.933f); }
    public static Color indigo { get => new Color(0.294f, 0.0f, 0.51f); }
    public static Color blueViolet { get => new Color(0.541f, 0.169f, 0.886f); }
    public static Color royalBlue { get => new Color(0.255f, 0.412f, 0.882f); }
    public static Color cornflowerBlue { get => new Color(0.392f, 0.584f, 0.929f); }
    public static Color dodgerBlue { get => new Color(0.118f, 0.565f, 1.0f); }
    public static Color steelBlue { get => new Color(0.275f, 0.51f, 0.706f); }
    public static Color skyBlue { get => new Color(0.529f, 0.808f, 0.922f); }
    public static Color deepSkyBlue { get => new Color(0.0f, 0.749f, 1.0f); }
    public static Color lightSkyBlue { get => new Color(0.529f, 0.808f, 0.98f); }
    public static Color pink { get => new Color(1.0f, 0.753f, 0.796f); }
    public static Color peach { get => new Color(1.0f, 0.855f, 0.725f); }
    public static Color lavender { get => new Color(0.902f, 0.902f, 0.98f); }
    public static Color mint { get => new Color(0.686f, 0.933f, 0.686f); }
    public static Color coral { get => new Color(1.0f, 0.498f, 0.314f); }
    public static Color turquoise { get => new Color(0.251f, 0.878f, 0.816f); }
    public static Color gold { get => new Color(1.0f, 0.843f, 0.0f); }
    public static Color silverGray { get => new Color(0.749f, 0.749f, 0.749f); }
    public static Color darkSlateGray { get => new Color(0.184f, 0.31f, 0.31f); }
    public static Color salmon { get => new Color(0.98f, 0.502f, 0.447f); }
    public static Color pastelPink { get => new Color(1.0f, 0.769f, 0.796f); }
    public static Color pastelBlue { get => new Color(0.68f, 0.78f, 0.937f); }
    public static Color pastelGreen { get => new Color(0.596f, 0.984f, 0.596f); }
    public static Color pastelYellow { get => new Color(0.961f, 0.961f, 0.863f); }
    public static Color pastelPurple { get => new Color(0.7f, 0.507f, 0.714f); }
    public static Color sienna { get => new Color(0.627f, 0.322f, 0.176f); }
    public static Color khaki { get => new Color(0.941f, 0.902f, 0.549f); }
    public static Color oliveDrab { get => new Color(0.419f, 0.556f, 0.137f); }
    public static Color sandyBrown { get => new Color(0.957f, 0.643f, 0.376f); }
    public static Color rosyBrown { get => new Color(0.737f, 0.561f, 0.561f); }
    #endregion
}

/// <summary>
/// Color in HSV format.
/// </summary>
public class HSVColor
{
    public float hue, saturation, value, alpha;

    #region Constructors

    /// <summary>
    /// Creates new HSVColor with HSV values and sets alpha to 1.
    /// </summary>
    public HSVColor(float hue, float saturation, float value)
    {
        hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
        saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
        value = value > 1 ? 1 : value < 0 ? 0 : value;

        this.hue = hue;
        this.saturation = saturation;
        this.value = value;
        alpha = 1f;
    }

    /// <summary>
    /// Creates new HSVColor with HSV values and specified alpha.
    /// </summary>
    public HSVColor(float hue, float saturation, float value, float alpha)
    {
        hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
        saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
        value = value > 1 ? 1 : value < 0 ? 0 : value;
        alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

        this.hue = hue;
        this.saturation = saturation;
        this.value = value;
        this.alpha = alpha;
    }

    /// <summary>
    /// Creates new HSVColor from default RGBA color format.
    /// </summary>
    /// <param name="color">Color in default RGBA color format.</param>
    public HSVColor(Color color)
    {
        HSVColor hsv = RGBAToHSV(color);
        hue = hsv.hue;
        saturation = hsv.saturation;
        value = hsv.value;
        alpha = hsv.alpha;
    }

    #endregion

    /// <summary>
    /// Creates new HSVColor with all values set to 0.
    /// </summary>
    public static HSVColor Zero()
    {
        return new HSVColor(0, 0, 0, 0);
    }

    /// <summary>
    /// Converts HSVColor to default RGBA color format.
    /// </summary>
    /// <param name="color">Color in HSVColor format.</param>
    /// <returns>Color in default RGBA Color format.</returns>
    public static Color ToColor(HSVColor color)
    {
        return HSVToRGBA(color);
    }

    #region Operators

    public static HSVColor operator +(HSVColor a, HSVColor b)
    {
        return new HSVColor(a.hue + b.hue, a.saturation + b.saturation, a.value + b.value, a.alpha + b.alpha);
    }

    public static HSVColor operator -(HSVColor a, HSVColor b)
    {
        return new HSVColor(a.hue - b.hue, a.saturation - b.saturation, a.value - b.value, a.alpha - b.alpha);
    }

    public static HSVColor operator *(HSVColor a, HSVColor b)
    {
        return new HSVColor(a.hue * b.hue, a.saturation * b.saturation, a.value * b.value, a.alpha * b.alpha);
    }

    public static HSVColor operator /(HSVColor a, HSVColor b)
    {
        return new HSVColor(a.hue / b.hue, a.saturation / b.saturation, a.value / b.value, a.alpha / b.alpha);
    }

    public static bool operator ==(HSVColor a, HSVColor b)
    {
        return a.hue == b.hue && a.saturation == b.saturation && a.value == b.value && a.alpha == b.alpha;
    }

    public static bool operator !=(HSVColor a, HSVColor b)
    {
        return a.hue != b.hue || a.saturation != b.saturation || a.value != b.value || a.alpha != b.alpha;
    }

    public override string ToString()
    {
        return string.Format("Hue: {0}, Saturation: {1}, Value: {2}, Alpha: {3}", hue, saturation, value, alpha);
    }

    public override bool Equals(object obj)
    {
        if (obj == null)
        {
            return false;
        }

        HSVColor color = obj as HSVColor;
        if ((object)color == null)
        {
            return false;
        }

        return hue == color.hue && saturation == color.saturation && value == color.value && alpha == color.alpha;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    #endregion

    #region Private

    public static Color HSVToRGBA(HSVColor hsv)
    {
        float c = hsv.value / 100.0f * hsv.saturation / 100.0f;
        float x = c * (1 - Math.Abs((hsv.hue / 60.0f) % 2 - 1));
        float m = hsv.value / 100.0f - c;

        float r, g, b;
        if (hsv.hue >= 0 && hsv.hue < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (hsv.hue >= 60 && hsv.hue < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (hsv.hue >= 120 && hsv.hue < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (hsv.hue >= 180 && hsv.hue < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (hsv.hue >= 240 && hsv.hue < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else
        {
            r = c;
            g = 0;
            b = x;
        }

        return new Color(
            (float)Math.Round((byte)Math.Round((r + m) * 255f) / 255f, 2),
            (float)Math.Round((byte)Math.Round((g + m) * 255f) / 255f, 2),
            (float)Math.Round((byte)Math.Round((b + m) * 255f) / 255f, 2)
        );
    }

    public static HSVColor RGBAToHSV(Color color)
    {
        float[] hsv = new float[3];

        float max = new[] { color.r, color.g, color.b }.Max();
        float min = new[] { color.r, color.g, color.b }.Min();

        float delta = max - min;

        hsv[1] = max != 0 ? delta / max : 0;
        hsv[2] = max;

        if (hsv[1] == 0)
        {
            return new HSVColor(hsv[0] / 100f, (float)Math.Round(hsv[1], 2), (float)Math.Round(hsv[2], 2));
        }
        if (color.r == max)
        {
            hsv[0] = ((color.g - color.b) / delta);
        }
        else if (color.g == max)
        {
            hsv[0] = ((color.b - color.r) / delta) + 2.0f;
        }
        else if (color.b == max)
        {
            hsv[0] = ((color.r - color.g) / delta) + 4.0f;
        }

        hsv[0] *= 60.0f;

        if (hsv[0] < 0)
        {
            hsv[0] += 360.0f;
        }

        return new HSVColor(hsv[0] / 100f, (float)Math.Round(hsv[1], 2), (float)Math.Round(hsv[2], 2));
    }

    #endregion
}

/// <summary>
/// Color in HSL format.
/// </summary>
public class HSLColor
{
    public float hue, saturation, lightness, alpha;

    #region Constructors

    /// <summary>
    /// Creates new HSLColor with HSL values and sets alpha to 1.
    /// </summary>
    public HSLColor(float hue, float saturation, float lightness)
    {
        hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
        saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
        lightness = lightness > 1 ? 1 : lightness < 0 ? 0 : lightness;

        this.hue = hue;
        this.saturation = saturation;
        this.lightness = lightness;
        alpha = 1f;
    }

    /// <summary>
    /// Creates new HSLColor with HSL values and specified alpha.
    /// </summary>
    public HSLColor(float hue, float saturation, float lightness, float alpha)
    {
        hue = hue > 1 ? 1 : hue < 0 ? 0 : hue;
        saturation = saturation > 1 ? 1 : saturation < 0 ? 0 : saturation;
        lightness = lightness > 1 ? 1 : lightness < 0 ? 0 : lightness;
        alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

        this.hue = hue;
        this.saturation = saturation;
        this.lightness = lightness;
        this.alpha = alpha;
    }

    /// <summary>
    /// Creates new HSLColor from default RGBA color format.
    /// </summary>
    /// <param name="color">Color in default RGBA color format.</param>
    public HSLColor(Color color)
    {
        HSLColor hsl = RGBAToHSL(color);
        hue = hsl.hue;
        saturation = hsl.saturation;
        lightness = hsl.lightness;
        alpha = hsl.alpha;
    }

    #endregion

    /// <summary>
    /// Creates new HSLColor with all values set to 0.
    /// </summary>
    public static HSLColor Zero()
    {
        return new HSLColor(0, 0, 0, 0);
    }

    /// <summary>
    /// Converts HSLColor to default RGBA color format.
    /// </summary>
    /// <param name="color">Color in HSL color format.</param>
    /// <returns>Color in default RGBA color format.</returns>
    public static Color ToColor(HSLColor color)
    {
        return HSLToRGBA(color);
    }

    #region Operators

    public static HSLColor operator +(HSLColor a, HSLColor b)
    {
        return new HSLColor(a.hue + b.hue, a.saturation + b.saturation, a.lightness + b.lightness, a.alpha + b.alpha);
    }

    public static HSLColor operator -(HSLColor a, HSLColor b)
    {
        return new HSLColor(a.hue - b.hue, a.saturation - b.saturation, a.lightness - b.lightness, a.alpha - b.alpha);
    }

    public static HSLColor operator *(HSLColor a, HSLColor b)
    {
        return new HSLColor(a.hue * b.hue, a.saturation * b.saturation, a.lightness * b.lightness, a.alpha * b.alpha);
    }

    public static HSLColor operator /(HSLColor a, HSLColor b)
    {
        return new HSLColor(a.hue / b.hue, a.saturation / b.saturation, a.lightness / b.lightness, a.alpha / b.alpha);
    }

    public static bool operator ==(HSLColor a, HSLColor b)
    {
        return a.hue == b.hue && a.saturation == b.saturation && a.lightness == b.lightness && a.alpha == b.alpha;
    }

    public static bool operator !=(HSLColor a, HSLColor b)
    {
        return a.hue != b.hue || a.saturation != b.saturation || a.lightness != b.lightness || a.alpha != b.alpha;
    }

    public override string ToString()
    {
        return string.Format("Hue: {0}, Saturation: {1}, Lightness: {2}, Alpha: {3}", hue, saturation, lightness, alpha);
    }

    public override bool Equals(object obj)
    {
        if (obj == null)
        {
            return false;
        }

        HSLColor color = obj as HSLColor;
        if ((object)color == null)
        {
            return false;
        }

        return hue == color.hue && saturation == color.saturation && lightness == color.lightness && alpha == color.alpha;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    #endregion

    #region Private

    public static Color HSLToRGBA(HSLColor hsl)
    {
        float c = (1 - Math.Abs(2 * hsl.lightness / 100.0f - 1)) * hsl.saturation / 100.0f;
        float x = c * (1 - Math.Abs((hsl.hue / 60.0f) % 2 - 1));
        float m = hsl.lightness / 100.0f - c / 2.0f;

        float r, g, b;
        if (hsl.hue >= 0 && hsl.hue < 60)
        {
            r = c;
            g = x;
            b = 0;
        }
        else if (hsl.hue >= 60 && hsl.hue < 120)
        {
            r = x;
            g = c;
            b = 0;
        }
        else if (hsl.hue >= 120 && hsl.hue < 180)
        {
            r = 0;
            g = c;
            b = x;
        }
        else if (hsl.hue >= 180 && hsl.hue < 240)
        {
            r = 0;
            g = x;
            b = c;
        }
        else if (hsl.hue >= 240 && hsl.hue < 300)
        {
            r = x;
            g = 0;
            b = c;
        }
        else
        {
            r = c;
            g = 0;
            b = x;
        }

        return new Color(
            (float)Math.Round((byte)Math.Round((r + m) * 255f) / 255f, 2),
            (float)Math.Round((byte)Math.Round((g + m) * 255f) / 255f, 2),
            (float)Math.Round((byte)Math.Round((b + m) * 255f) / 255f, 2)
        );
    }


    public static HSLColor RGBAToHSL(Color color)
    {
        float[] hsl = new float[3];

        float max = new[] { color.r, color.g, color.b }.Max();
        float min = new[] { color.r, color.g, color.b }.Min();

        float delta = max - min;

        hsl[2] = (max + min) / 2;

        if (delta == 0)
        {
            hsl[0] = 0;
            hsl[1] = 0;
        }
        else
        {
            hsl[1] = hsl[2] < 0.5 ? delta / (max + min) : delta / (2 - max - min);

            if (max == color.r)
            {
                hsl[0] = (color.g - color.b) / delta + (color.g < color.b ? 6 : 0);
            }
            else if (max == color.g)
            {
                hsl[0] = (color.b - color.r) / delta + 2;
            }
            else if (max == color.b)
            {
                hsl[0] = (color.r - color.g) / delta + 4;
            }

            hsl[0] *= 60;
        }

        return new HSLColor(hsl[0] / 100f, (float)Math.Round(hsl[1], 2), (float)Math.Round(hsl[2], 2));
    }

    #endregion
}

/// <summary>
/// Color in CMYK format.
/// </summary>
public class CMYKColor
{
    public float cyan, magenta, yellow, black, alpha;

    #region Constructors

    /// <summary>
    /// Creates new CMYKColor with CMYK values and sets alpha to 1.
    /// </summary>
    public CMYKColor(float cyan, float magenta, float yellow, float black)
    {
        cyan = cyan > 1 ? 1 : cyan < 0 ? 0 : cyan;
        magenta = magenta > 1 ? 1 : magenta < 0 ? 0 : magenta;
        yellow = yellow > 1 ? 1 : yellow < 0 ? 0 : yellow;
        black = black > 1 ? 1 : black < 0 ? 0 : black;

        this.cyan = cyan;
        this.magenta = magenta;
        this.yellow = yellow;
        this.black = black;
        alpha = 1f;
    }

    /// <summary>
    /// Creates new CMYKColor with CMYK values and specified alpha.
    /// </summary>
    public CMYKColor(float cyan, float magenta, float yellow, float black, float alpha)
    {
        cyan = cyan > 1 ? 1 : cyan < 0 ? 0 : cyan;
        magenta = magenta > 1 ? 1 : magenta < 0 ? 0 : magenta;
        yellow = yellow > 1 ? 1 : yellow < 0 ? 0 : yellow;
        black = black > 1 ? 1 : black < 0 ? 0 : black;
        alpha = alpha > 1 ? 1 : alpha < 0 ? 0 : alpha;

        this.cyan = cyan;
        this.magenta = magenta;
        this.yellow = yellow;
        this.black = black;
        this.alpha = alpha;
    }

    /// <summary>
    /// Creates new CMYKColor from default RGBA color format.
    /// </summary>
    /// <param name="color">Color in default RGBA color format.</param>
    public CMYKColor(Color color)
    {
        CMYKColor cmyk = RGBAToCMYK(color);
        cyan = cmyk.cyan;
        magenta = cmyk.magenta;
        yellow = cmyk.yellow;
        black = cmyk.black;
        alpha = cmyk.alpha;
    }

    #endregion

    /// <summary>
    /// Creates new CMYKColor with all values set to 0.
    /// </summary>
    public static CMYKColor Zero()
    {
        return new CMYKColor(0, 0, 0, 0, 0);
    }

    /// <summary>
    /// Converts CMYKColor to default RGBA color format.
    /// </summary>
    /// <param name="color">Color in CMYK color format.</param>
    /// <returns>Color in default RGBA color format.</returns>
    public static Color ToColor(CMYKColor color)
    {
        return CMYKToRGBA(color);
    }

    #region Operators

    public static CMYKColor operator +(CMYKColor a, CMYKColor b)
    {
        return new CMYKColor(a.cyan + b.cyan, a.magenta + b.magenta, a.yellow + b.yellow, a.black + b.black, a.alpha + b.alpha);
    }

    public static CMYKColor operator -(CMYKColor a, CMYKColor b)
    {
        return new CMYKColor(a.cyan - b.cyan, a.magenta - b.magenta, a.yellow - b.yellow, a.black - b.black, a.alpha - b.alpha);
    }

    public static CMYKColor operator *(CMYKColor a, CMYKColor b)
    {
        return new CMYKColor(a.cyan * b.cyan, a.magenta * b.magenta, a.yellow * b.yellow, a.black * b.black, a.alpha * b.alpha);
    }

    public static CMYKColor operator /(CMYKColor a, CMYKColor b)
    {
        return new CMYKColor(a.cyan / b.cyan, a.magenta / b.magenta, a.yellow / b.yellow, a.black / b.black, a.alpha / b.alpha);
    }

    public static bool operator ==(CMYKColor a, CMYKColor b)
    {
        return a.cyan == b.cyan && a.magenta == b.magenta && a.yellow == b.yellow && a.black == b.black && a.alpha == b.alpha;
    }

    public static bool operator !=(CMYKColor a, CMYKColor b)
    {
        return a.cyan != b.cyan || a.magenta != b.magenta || a.yellow != b.yellow || a.black != b.black || a.alpha != b.alpha;
    }

    public override string ToString()
    {
        return string.Format("Cyan: {0}, Magenta: {1}, Yellow: {2}, Black: {3}, Alpha: {4}", cyan, magenta, yellow, black, alpha);
    }

    public override bool Equals(object obj)
    {
        if (obj == null)
        {
            return false;
        }

        CMYKColor color = obj as CMYKColor;
        if ((object)color == null)
        {
            return false;
        }

        return cyan == color.cyan && magenta == color.magenta && yellow == color.yellow && black == color.black && alpha == color.alpha;
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    #endregion

    #region Private

    private static CMYKColor RGBAToCMYK(Color color)
    {
        float c = 1 - color.r;
        float m = 1 - color.g;
        float y = 1 - color.b;
        float k = Math.Min(c, Math.Min(m, y));

        if (k == 1)
        {
            return new CMYKColor(0, 0, 0, 1);
        }

        return new CMYKColor(
                (c - k) / (1 - k),
                (m - k) / (1 - k),
                (y - k) / (1 - k),
                k);
    }


    private static Color CMYKToRGBA(CMYKColor cmyk)
    {
        return new Color((1 - cmyk.cyan) * (1 - cmyk.black),
                         (1 - cmyk.magenta) * (1 - cmyk.black),
                         (1 - cmyk.yellow) * (1 - cmyk.black),
                         cmyk.alpha);
    }

    #endregion
}


//Vectors

public struct Vector2
{
    public float X, Y;

    public static Vector2 Zero = new Vector2(0f, 0f);

    public Vector2(Vector2 vec)
    {
        X = vec.X;
        Y = vec.Y;
    }

    public Vector2(float x)
    {
        X = x;
        Y = x;
    }

    public Vector2(float x, float y)
    {
        X = x;
        Y = y;
    }
    public float LengthSquared()
    {
        return X * X + Y * Y;
    }

    public float Length()
    {
        return (float)Math.Sqrt(LengthSquared());
    }

    static public Vector2 operator +(Vector2 a, Vector2 b)
    {
        return new Vector2(a.X + b.X, a.Y + b.Y);
    }

    static public Vector2 operator +(Vector2 a, float b)
    {
        return new Vector2(a.X + b, a.Y + b);
    }

    static public Vector2 operator -(Vector2 a, Vector2 b)
    {
        return new Vector2(a.X - b.X, a.Y - b.Y);
    }

    static public Vector2 operator -(Vector2 a, float b)
    {
        return new Vector2(a.X - b, a.Y - b);
    }

    static public Vector2 operator *(Vector2 a, float b)
    {
        return new Vector2(a.X * b, a.Y * b);
    }

    static public Vector2 operator /(Vector2 a, float b)
    {
        return new Vector2(a.X / b, a.Y / b);
    }

    public static float LengthSquared(Vector2 vector)
    {
        return vector.X * vector.X + vector.Y * vector.Y;
    }

    public static float Length(Vector2 vector)
    {
        return (float)Math.Sqrt(LengthSquared(vector));
    }

    public static Vector2 NormalizeTo(Vector2 vector, float length)
    {
        float currentLength = Length(vector);

        if (currentLength == 0)
        {
            return Vector2.Zero;
        }

        float scale = length / currentLength;
        vector.X *= scale;
        vector.Y *= scale;

        return vector;
    }

    public void NormalizeTo(float length)
    {
        float currentLength = Length();
        if (currentLength == 0)
        {
            return;
        }
        float scale = length / currentLength;
        X *= scale;
        Y *= scale;
    }

    public void Normalize()
    {
        NormalizeTo(1.0f);
    }

    public static Vector2 FindVector(Vector2 from, Vector2 to)
    {
        Vector2 vector = new Vector2();
        vector.X = to.X - from.X;
        vector.Y = to.Y - from.Y;
        return vector;
    }
};

public struct Vector3
{
    public float X, Y, Z;

    public static Vector3 Zero = new Vector3(0f, 0f, 0f);

    public Vector3(Vector3 vec)
    {
        X = vec.X;
        Y = vec.Y;
        Z = vec.Z;
    }

    public Vector3(Vector2 vec)
    {
        X = vec.X;
        Y = vec.Y;
        Z = 0f;
    }

    public Vector3(Vector2 vec, float z)
    {
        X = vec.X;
        Y = vec.Y;
        Z = z;
    }

    public Vector3(float x)
    {
        X = x;
        Y = x;
        Z = x;
    }

    public Vector3(float x, float y, float z)
    {
        X = x;
        Y = y;
        Z = z;
    }

    public Vector2 XY
    {
        get => new Vector2(X, Y);
        set { X = value.X; Y = value.Y; }
    }

    static public Vector3 operator +(Vector3 a, Vector3 b)
    {
        return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
    }

    static public Vector3 operator -(Vector3 a, Vector3 b)
    {
        return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    }

    static public Vector3 operator *(Vector3 a, float b)
    {
        return new Vector3(a.X * b, a.Y * b, a.Z * b);
    }
};

public struct Vector4
{
    public float X, Y, Z, W;

    public static Vector4 Zero = new Vector4(0f, 0f, 0f, 0f);

    public Vector4(Vector4 vec)
    {
        X = vec.X;
        Y = vec.Y;
        Z = vec.Z;
        W = vec.W;
    }

    public Vector4(float x)
    {
        X = x;
        Y = x;
        Z = x;
        W = x;
    }

    public Vector4(float x, float y, float z, float w)
    {
        X = x;
        Y = y;
        Z = z;
        W = w;
    }

    public Vector2 XY
    {
        get => new Vector2(X, Y);
        set { X = value.X; Y = value.Y; }
    }

    static public Vector4 operator +(Vector4 a, Vector4 b)
    {
        return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W * b.W);
    }

    static public Vector4 operator -(Vector4 a, Vector4 b)
    {
        return new Vector4(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
    }

    static public Vector4 operator *(Vector4 a, float b)
    {
        return new Vector4(a.X * b, a.Y * b, a.Z * b, a.W * b);
    }
};


//Debug console

/// <summary>
/// Represents a debug console for logging messages.
/// </summary>
public class DebugConsole
{
    /// <summary>
    /// Severity of log message.
    /// </summary>
    public enum LogType
    {
        Info = 0, Warning = 1, Error = 2
    }

    /// <summary>
    /// Logs a message to the debug console.
    /// </summary>
    /// <param name="message">The message to log.</param>
    /// <param name="logType">The type of log message.</param>
    public static void Log(string message, LogType logType = LogType.Info)
    {
        InternalCalls.Console_Log(message, logType);
    }
}