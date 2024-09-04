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
        }

        public void OnUpdate(float ts)
        {
            if(attackBox == null)
            {
                attackBox = entity.As<EnemyAttackBoxComponent>();
            }
            if(attackBox.isEnemyinRange(player))
            {
                return;
            }
            if (IsPlayerInSight())
            {
                TurnTowardPlayer();
                if (!GroundCheckLeft() && !GroundCheckRight())
                {
                    return;
                }
            }
            else if ((GroundCheckMiddle() && GroundCheckRight() && (WallCheckLeft() || !GroundCheckLeft()) && direction.X < 0) || (GroundCheckMiddle() && GroundCheckLeft() && (WallCheckRight() || !GroundCheckRight()) && direction.X > 0))
            {
                direction.X *= -1;
                if(attackBox != null)
                    attackBox.attackDirecton = direction;
            }

            float targetSpeed = direction.X * speed * multiplier;

            float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;

            float speedDif = targetSpeed - rigidBody.linearVelocity.X;

            float movement = (float)speedDif * accelRate;

            rigidBody.ApplyLinearImpulse(Vector2.Right * movement);
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
                attackBox.attackDirecton = direction;
        }

        public bool GroundCheckMiddle()
        {
            List<Entity> entities = Entity.FindEntityByName("Ground").GetChildren();

            foreach (Entity entity in entities)
            {
                BoxCollider2DComponent groundCollider = entity.GetComponent<BoxCollider2DComponent>();
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X, transform.translation.Y - collider.size.Y), new Vector2(0.1f, 0.1f)))
                {
                    return true;
                }
            }

            return false;
        }

        public bool GroundCheckRight()
        {
            List<Entity> entities = Entity.FindEntityByName("Ground").GetChildren();

            foreach (Entity entity in entities)
            {
                BoxCollider2DComponent groundCollider = entity.GetComponent<BoxCollider2DComponent>();
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X + collider.size.X, transform.translation.Y - collider.size.Y), new Vector2(0.1f, 0.1f)))
                {
                    return true;
                }
            }

            return false;
        }

        public bool GroundCheckLeft()
        {
            List<Entity> entities = Entity.FindEntityByName("Ground").GetChildren();

            foreach (Entity entity in entities)
            {
                BoxCollider2DComponent groundCollider = entity.GetComponent<BoxCollider2DComponent>();
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X - collider.size.X, transform.translation.Y - collider.size.Y), new Vector2(0.1f, 0.1f)))
                {
                    return true;
                }
            }

            return false;
        }

        public bool WallCheckLeft()
        {
            List<Entity> entities = Entity.FindEntityByName("Ground").GetChildren();

            foreach (Entity entity in entities)
            {
                BoxCollider2DComponent groundCollider = entity.GetComponent<BoxCollider2DComponent>();
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X - collider.size.X, transform.translation.Y), new Vector2(0.1f, 0.1f)))
                {
                    return true;
                }
            }

            return false;
        }

        public bool WallCheckRight()
        {
            List<Entity> entities = Entity.FindEntityByName("Ground").GetChildren();

            foreach (Entity entity in entities)
            {
                BoxCollider2DComponent groundCollider = entity.GetComponent<BoxCollider2DComponent>();
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X + collider.size.X, transform.translation.Y), new Vector2(0.1f, 0.1f)))
                {
                    return true;
                }
            }

            return false;
        }

        public void SetMultiplier(float multiplier)
        {
            this.multiplier = multiplier;
        }
    }
}
