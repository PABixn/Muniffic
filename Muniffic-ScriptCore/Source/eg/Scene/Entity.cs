using System;
using System.Runtime.CompilerServices;

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