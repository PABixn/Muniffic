using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class EnemyRunComponent : DefaultBehaviour
    {
        Entity player;
        TransformComponent transform;
        BoxCollider2DComponent collider;
        RigidBody2DComponent rigidBody;
        AnimatorComponent animator;

        public Vector2 direction = Vector2.Right;
        public int sightRange = 10;
        public float speed = 5f;
        public float acceleration = 0.5f;
        public float decceleration = 0.5f;

        private float multiplier = 1.0f;

        private EnemyAttackBoxComponent attackBox;

        public void OnCreate()
        {
            player = Entity.FindEntityByName("Player");
            transform = entity.GetComponent<TransformComponent>();
            collider = entity.GetComponent<BoxCollider2DComponent>();
            rigidBody = entity.GetComponent<RigidBody2DComponent>();
            animator = entity.GetComponent<AnimatorComponent>();
        }

        public void OnUpdate(float ts)
        {
            if (attackBox == null)
            {
                attackBox = entity.As<EnemyAttackBoxComponent>();
            }
            if(attackBox.CollidesWith(player))
            {
                return;
            }

            if (IsPlayerInSight())
            {
                TurnTowardPlayer();
                if (!WallCheckLeft() || !WallCheckRight())
                {
                    return;
                }
            }
            else if (!WallCheckLeft() || !WallCheckRight())
            {
                direction.X *= -1;
                if(attackBox != null)
                    attackBox.attackDirection = direction;
            }

            //float targetSpeed = direction.X * speed * multiplier;

            //float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;

            //float speedDif = targetSpeed - rigidBody.linearVelocity.X;

            //float movement = (float)speedDif * accelRate;

            rigidBody.linearVelocity = new Vector2(direction.X * speed * multiplier, rigidBody.linearVelocity.Y);
            if(entity.As<EntityTypeComponent>().entityType == EntityType.ENEMY_SQUARE)
                animator.Play("squareEnemyWalk");
            else if(entity.As<EntityTypeComponent>().entityType == EntityType.ENEMY_TRIANGLE)
                animator.Play("crabWalk");
        }

        private bool IsPlayerInSight()
        {
            Vector2 edgeStartPoint = new Vector2(transform.translation.X - sightRange, transform.translation.Y);
            Vector2 edgeEndpoint = new Vector2(transform.translation.X + sightRange, transform.translation.Y);
            return player.GetComponent<BoxCollider2DComponent>().CollidesWithEdge(edgeStartPoint, edgeEndpoint);
        }


        public void TurnTowardPlayer()
        {
            if (player.GetComponent<TransformComponent>().translation.X < transform.translation.X)
            {
                direction = Vector2.Left;
            }
            else
            {
                direction = Vector2.Right;
            }
            if (attackBox != null)
                attackBox.attackDirection = direction;
        }

        public bool WallCheckLeft()
        {
            return transform.translation.X >= 9.5f; 
        }

        public bool WallCheckRight()
        {
            return transform.translation.X <= 19.6f;
        }

        public void SetMultiplier(float multiplier)
        {
            this.multiplier = multiplier;
        }
    }
}
