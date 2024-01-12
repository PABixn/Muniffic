﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace eg
{
    public abstract class Component
    { 
        public Entity Entity { get; internal set; }

    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }

            set => InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }

            set => InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
        }

        public Vector3 Rotation
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
        public Vector4 Color
        {
            get
            {
                InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }

            set => InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref value);
        }

        public string Texture
        {
            get => InternalCalls.SpriteRendererComponent_GetTexture(Entity.ID);
            set => InternalCalls.SpriteRendererComponent_SetTexture(Entity.ID, ref value);
        }

        public float TilingFactor
        {
            get => InternalCalls.SpriteRendererComponent_GetTilingFactor(Entity.ID);
            set => InternalCalls.SpriteRendererComponent_SetTilingFactor(Entity.ID, ref value);
        }
    }

    public class CircleRendererComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }

            set => InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref value);
        }

        public float Thickness
        {
            get => InternalCalls.CircleRendererComponent_GetThickness(Entity.ID);
            set => InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, ref value);
        }

        public float Fade
        {
            get => InternalCalls.CircleRendererComponent_GetFade(Entity.ID);
            set => InternalCalls.CircleRendererComponent_SetFade(Entity.ID, ref value);
        }
    }

    public class CameraComponent : Component
    {
        public enum ProjectionType { Perspective = 0, Ortographic = 1 }

        public bool Primary
        {
            get => InternalCalls.CameraComponent_IsPrimary(Entity.ID);
            set => InternalCalls.CameraComponent_SetPrimary(Entity.ID, ref value);
        }

        public bool FixedAspectRatio
        {
            get => InternalCalls.CameraComponent_IsFixedAspectRatio(Entity.ID);
            set => InternalCalls.CameraComponent_SetFixedAspectRatio(Entity.ID, ref value);
        }

        public ProjectionType Type
        {
            get => InternalCalls.CameraComponent_GetProjectionType(Entity.ID);
            set => InternalCalls.CameraComponent_SetProjectionType(Entity.ID, ref value);
        }

        public void SetPerspective(float verticalFov, float nearClip, float farClip)
        {
            InternalCalls.CameraComponent_SetPerspective(Entity.ID, ref verticalFov, ref nearClip, ref farClip);
        }

        public void SetOrthographic(float size, float nearClip, float farClip)
        {
            InternalCalls.CameraComponent_SetOrthographic(Entity.ID, ref size, ref nearClip, ref farClip);
        }

        public float OrtographicSize
        {
            get => InternalCalls.CameraComponent_GetOrthographicSize(Entity.ID);
            set => InternalCalls.CameraComponent_SetOrthographicSize(Entity.ID, ref value);
        }

        public float OrtographicNearClip
        {
            get => InternalCalls.CameraComponent_GetOrthographicNearClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetOrthographicNearClip(Entity.ID, ref value);
        }

        public float OrtographicFarClip
        {
            get => InternalCalls.CameraComponent_GetOrthographicFarClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetOrthographicFarClip(Entity.ID, ref value);
        }

        public float PerspectiveVerticalFov
        {
            get => InternalCalls.CameraComponent_GetPerspectiveVerticalFOV(Entity.ID);
            set => InternalCalls.CameraComponent_SetPerspectiveVerticalFOV(Entity.ID, ref value);
        }

        public float PerspectiveNearClip
        {
            get => InternalCalls.CameraComponent_GetPerspectiveNearClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetPerspectiveNearClip(Entity.ID, ref value);
        }

        public float PerspectiveFarClip
        {
            get => InternalCalls.CameraComponent_GetPerspectiveFarClip(Entity.ID);
            set => InternalCalls.CameraComponent_SetPerspectiveFarClip(Entity.ID, ref value);
        }
    }

    public class RigidBody2DComponent : Component
    {
        public enum BodyType { Static = 0, Dynamic, Kinematic }

        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.RigidBody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 velocity);
                return velocity;
            }
        }

        public BodyType Type
        {
            get => InternalCalls.RigidBody2DComponent_GetType(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetType(Entity.ID, value);
        }

        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake = true)
        {
            InternalCalls.RigidBody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }

        public bool FixedRotation
        {
            get => InternalCalls.RigidBody2DComponent_IsFixedRotation(Entity.ID);
            set => InternalCalls.RigidBody2DComponent_SetFixedRotation(Entity.ID, ref value);
        }
    }

    public class BoxCollider2DComponent : Component
    {
        public Vector2 Size
        {
            get
            {
                InternalCalls.BoxCollider2DComponent_GetSize(Entity.ID, out Vector2 size);
                return size;
            }

            set => InternalCalls.BoxCollider2DComponent_SetSize(Entity.ID, ref value);
        }

        public Vector2 Offset
        {
            get
            {
                InternalCalls.BoxCollider2DComponent_GetOffset(Entity.ID, out Vector2 offset);
                return offset;
            }

            set => InternalCalls.BoxCollider2DComponent_SetOffset(Entity.ID, ref value);
        }

        public float Density
        {
            get => InternalCalls.BoxCollider2DComponent_GetDensity(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetDensity(Entity.ID, ref value);
        }

        public float Friction
        {
            get => InternalCalls.BoxCollider2DComponent_GetFriction(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetFriction(Entity.ID, ref value);
        }

        public float Restitution
        {
            get => InternalCalls.BoxCollider2DComponent_GetRestitution(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetRestitution(Entity.ID, ref value);
        }

        public float RestitutionThreshold
        {
            get => InternalCalls.BoxCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            set => InternalCalls.BoxCollider2DComponent_SetRestitutionThreshold(Entity.ID, ref value);
        }
    }

    public class CircleCollider2DComponent : Component
    {
        public float Offset
        {
            get => InternalCalls.CircleCollider2DComponent_GetOffset(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetOffset(Entity.ID, ref value);
        }

        public float Density
        {
            get => InternalCalls.CircleCollider2DComponent_GetDensity(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetDensity(Entity.ID, ref value);
        }

        public float Friction
        {
            get => InternalCalls.CircleCollider2DComponent_GetFriction(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetFriction(Entity.ID, ref value);
        }

        public float Restitution
        {
            get => InternalCalls.CircleCollider2DComponent_GetRestitution(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRestitution(Entity.ID, ref value);
        }

        public float RestitutionThreshold
        {
            get => InternalCalls.CircleCollider2DComponent_GetRestitutionThreshold(Entity.ID);
            set => InternalCalls.CircleCollider2DComponent_SetRestitutionThreshold(Entity.ID, ref value);
        }
    }

    public class TextComponent : Component
    {

        public string Text
        {
            get => InternalCalls.TextComponent_GetText(Entity.ID);
            set => InternalCalls.TextComponent_SetText(Entity.ID, value);
        }

        public Vector4 Color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }

            set
            {
                InternalCalls.TextComponent_SetColor(Entity.ID, ref value);
            }
        }

        public float Kerning
        {
            get => InternalCalls.TextComponent_GetKerning(Entity.ID);
            set => InternalCalls.TextComponent_SetKerning(Entity.ID, value);
        }

        public float LineSpacing
        {
            get => InternalCalls.TextComponent_GetLineSpacing(Entity.ID);
            set => InternalCalls.TextComponent_SetLineSpacing(Entity.ID, value);
        }

    }
}
