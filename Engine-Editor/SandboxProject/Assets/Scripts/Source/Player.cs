using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Sandbox
{
    public class Player : DefaultBehaviour
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_RigidBody2D;
        private AnimatorComponent m_Animator;
        private bool m_HasAnimator;
        Camera camera;

        public float Speed;
        public float Time = 0.0f;
        public Vector2 Velocity = new Vector2(0);
        void OnCreate()
        {
            Console.WriteLine("Player created! - " + entity.ID);
            m_Transform = GetComponent<TransformComponent>();
            m_RigidBody2D = GetComponent<RigidBody2DComponent>();
            if (HasComponent<AnimatorComponent>())
            {
                m_Animator = GetComponent<AnimatorComponent>();
                m_HasAnimator = true;
                m_Animator.Play("walking");

            }
            bool hasTransform = HasComponent<TransformComponent>();
            Console.WriteLine("HasComponent {0}", hasTransform);
        }

        void OnUpdate(float ts)
        {
            Entity cameraEntity = Entity.FindEntityByName("Camera");
            if (cameraEntity != null)
            {

                camera = cameraEntity.As<Camera>();
            }
            else
            {
                Console.WriteLine("Camera is null");
            }
            Time += ts;
            Vector3 velocity = new Vector3(0);
            if (Input.IsKeyDown(KeyCode.W))
            {
                m_Animator.ChangeAnimation("flight");
                velocity.Y = Speed;

            }
            else if (Input.IsKeyDown(KeyCode.S))
            {
                velocity.Y = -Speed;
            }
            else
            {
                velocity.Y = 0f;
            }
            if (Input.IsKeyDown(KeyCode.A))
            {
                
                velocity.X = -Speed;
            }
            else if (Input.IsKeyDown(KeyCode.D))
            {
                velocity.X = Speed;
                
            }
            else
            {
                velocity.X = 0f;
            }
            if (Input.IsKeyDown(KeyCode.Z) && m_HasAnimator)
            {
                m_Animator.ChangeAnimation("deathDone");
            }

            if (Input.IsKeyDown(KeyCode.Q))
            {
                camera.DistanceFromPlayer += 1f * ts;
            }
            else if (Input.IsKeyDown(KeyCode.E))
            {
                camera.DistanceFromPlayer -= 1f * ts;
            }
            if(Input.IsKeyDown(KeyCode.T)) 
            {
                m_Animator.ChangeAnimation("walking");
            }
            
            







            Vector3 translation = m_Transform.translation;

           
            translation += velocity * ts;

            m_RigidBody2D.ApplyLinearImpulse(velocity.XY, true);


            m_Transform.translation = translation;

            /*if(translation.Y == -18)
            {
                m_Animator.ChangeAnimation("deathDone");
            }
            if(translation.Y < -19.5)
            {
                m_Animator.ChangeAnimation("realdeath");
            }*/

            if (translation.Y < -19.5)
            {
                m_Animator.ChangeAnimation("walking");
            }
            if (translation.Y <= 15.02 & translation.Y >14 & translation.X > 5 & translation.X < 7)
            {
                m_Animator.ChangeAnimation("realdeath");
            }
            if (translation.Y <= 6.02 & translation.Y > 5 & translation.X > 4 & translation.X < 6)
            {
                m_Animator.ChangeAnimation("realdeath");
            }
            if (translation.Y <= 5.02 & translation.Y > 4 & translation.X > 9 & translation.X < 11)
            {
                m_Animator.ChangeAnimation("realdeath");
            }
            if (translation.Y <= -1.98 & translation.Y > -3 & translation.X > 5 & translation.X < 7)
            {
                m_Animator.ChangeAnimation("realdeath");
            }
            if (translation.Y <= -7.98 & translation.Y > -9 & translation.X > 9 & translation.X < 11)
            {
                m_Animator.ChangeAnimation("realdeath");
            }
            if (translation.Y <= -13.98 & translation.Y > -15 & translation.X > 4 & translation.X < 6)
            {
                m_Animator.ChangeAnimation("realdeath");
            }
            if (translation.Y <= -5.98 & translation.Y > -7 & translation.X > 4 & translation.X < 6)
            {
                m_Animator.ChangeAnimation("realdeath");
            }

        }

    }
}
