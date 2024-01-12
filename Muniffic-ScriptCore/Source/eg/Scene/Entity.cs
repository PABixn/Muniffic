using System;
using System.Runtime.CompilerServices;

namespace eg
{
    public class DefaultBehaviour
    {
        public Entity entity;

        protected DefaultBehaviour()
        {
            entity = new Entity(0);
        }
        internal DefaultBehaviour(ulong id)
        {
            entity = new Entity(id);
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(entity.ID, componentType);
        }

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

        public string Name
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

        public T As<T>() where T : DefaultBehaviour, new()
        {
            object instance = InternalCalls.Entity_GetScriptInstance(ID);
            return instance as T;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
            {
                return null;
            }

            T component = new T() { Entity = this };
            return component;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            InternalCalls.Entity_AddComponent(ID, componentType);

            return GetComponent<T>();
        }

        public void RemoveComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            InternalCalls.Entity_RemoveComponent(ID, componentType);
        }
        #endregion

        #region Static
        public static Entity FindEntityByName(string name)
        {
            ulong entityID = InternalCalls.Entity_FindEntityByName(name);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }

        public static Entity Create(string name)
        {
            ulong entityID = InternalCalls.Entity_Create(name);
            if (entityID == 0)
                return null;

            return new Entity(entityID);
        }

        public static void Destroy(Entity entity)
        {
            InternalCalls.Entity_Destroy(entity.ID);
        }

        public static void Destroy(ulong ID)
        {
            InternalCalls.Entity_Destroy(ID);
        }

        #endregion
    }
}