using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics.PerformanceData;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace eg
{
    /// <summary>
    /// Base class for all scripts.
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
        internal DefaultBehaviour(ulong id)
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
            if(!HasComponent<T>())
            {
                return null;
            }

            T component = new T() { Entity = entity };
            return component;
        }
    }

    public class Entity
    {
        /// <summary>
        /// UUID of the entity, given by the engine at creation.
        /// </summary>
        public readonly ulong ID;

        protected Entity()
        {
            ID = 0;
        }
        internal Entity(ulong id)
        {
            ID = id;
        }

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
                ulong parentID = InternalCalls.Entity_GetParent(ID);
                if (parentID == 0)
                    return null;

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
        public void SetParent(ulong entity)
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
        public bool IsChildOfAny(ulong entity)
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
        public bool IsChild(ulong entity)
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
        public void AddChild(ulong entity)
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
        public void RemoveChild(ulong entity)
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
        /// Retrieves all children entities of the current entity, including entities with empty IDs.
        /// </summary>
        /// <returns>A list of Entity objects representing the children entities.</returns>
        public List<Entity> GetAnyChildren()
        {
            string children = InternalCalls.Entity_GetAnyChildren(ID);

            string[] entityIDs = children.Split(',');

            List<Entity> entities = new List<Entity>();

            foreach (string entityID in entityIDs)
            {
                if (entityID != string.Empty)
                    entities.Add(new Entity(ulong.Parse(entityID)));
            }

            return entities;
        }

        /// <summary>
        /// Retrieves all children entities of the current entity, excluding entities with empty IDs.
        /// </summary>
        /// <returns>A list of Entity objects representing the children entities.</returns>
        public List<Entity> GetChildren()
        {
            string children = InternalCalls.Entity_GetChildren(ID);

            string[] entityIDs = children.Split(',');

            List<Entity> entities = new List<Entity>();

            foreach (string entityID in entityIDs)
            {
                if (entityID != string.Empty)
                    entities.Add(new Entity(ulong.Parse(entityID)));
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
        /// Retrieves entity from scene by name.
        /// </summary>
        /// <param name="name">Entity name.</param>
        /// <returns>Entity reference or null if entity has not been found.</returns>
        public static Entity FindEntityByName(string name)
        {
            ulong entityID = InternalCalls.Entity_FindEntityByName(name);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }

        /// <summary>
        /// Creates new entity.
        /// </summary>
        /// <param name="name">Entity name.</param>
        /// <returns>Entity reference or null if entity has not been created.</returns>
        public static Entity Create(string name)
        {
            ulong entityID = InternalCalls.Entity_Create(name);
            if (entityID == 0)
                return null;

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
        /// Destroys entity.
        /// </summary>
        /// <param name="ID">UUID of the entity that should be destroyed.</param>
        public static void Destroy(ulong ID)
        {
            InternalCalls.Entity_Destroy(ID);
        }

        #endregion
    }
}