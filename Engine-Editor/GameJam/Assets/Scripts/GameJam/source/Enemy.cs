using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using eg;

namespace Game
{
    public class Enemy : DefaultBehaviour
    {
        public float Speed;
        public Vector2 Velocity = new Vector2(0);
        public Vector2 Direction = new Vector2(0);
        public int Health;
        public int Damage;
        public float JumpHeight;
        public enum AttackType
        {
            Melee,
            Ranged
        };
        public AttackType attackType;
        protected TransformComponent playerTransform;
        protected TransformComponent Transform;
        protected RigidBody2DComponent RigidBody;
        protected Entity playerEntity;
        Random rnd = new Random();
        void OnCreate()
        {
            Console.WriteLine("Enemy created! - " + entity.ID);
            playerEntity = Entity.FindEntityByName("Player");
            if(playerEntity != null)
            {
                playerTransform = playerEntity.GetComponent<TransformComponent>();
            }
            Transform = GetComponent<TransformComponent>();
        }
        protected void OnUpdate(float ts) {

            if (attackType == AttackType.Ranged)
            {
                if (Math.Abs(playerTransform.translation.X - Transform.translation.X) <= 150 && Math.Abs(playerTransform.translation.X - Transform.translation.X) > 50)
                {
                    Direction.X = playerTransform.translation.X - Transform.translation.X;
                }
                else if (Math.Abs(playerTransform.translation.X - Transform.translation.X) < 50)
                {
                    Direction.X = (playerTransform.translation.X - Transform.translation.X) * -1;
                }
                else if (Math.Abs(playerTransform.translation.X - Transform.translation.X) == 50)
                {
                    Direction.X = 0;
                }
                else
                {
                    Direction.X = rnd.Next(-1, 1);
                };
                
            }
            else if (attackType == AttackType.Melee)
            {
                if (Math.Abs(playerTransform.translation.X - Transform.translation.X) <= 150)
                {
                    Direction.X = playerTransform.translation.X - Transform.translation.X;
                }
                else
                {
                    Direction.X = rnd.Next(-1, 1);
                }
            }
            

            if(Direction.X > 0)
            {
                Velocity.X = Speed;
            }
            else if(Direction.X < 0)
            {
                Velocity.X = -Speed;
            }
            else
            {
                Velocity.X = 0f;
            }
            if(Direction.Y > 0)
            {
                Velocity.Y = Speed;
            }
            else if(Direction.Y < 0)
            {
                Velocity.Y = -Speed;
            }
            else
            {
                Velocity.Y = 0f;
            }
            

            Transform.translation += new Vector3(Velocity.X, Velocity.Y, 0) * ts;

            
        }

        void Jump()
        {
            RigidBody.ApplyLinearImpulse(new Vector2(0, JumpHeight), true);
        }
    }
}