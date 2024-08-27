using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    internal class BaseEnemy : DefaultBehaviour
    {
        public HealthComponent healthComponent;

        private EntityTypeComponent entityTypeComponent;

        private RigidBody2DComponent rigidBody;
        private BoxCollider2DComponent collider;
        private TransformComponent transform;

        public int damage = 10;
        public int sightRange = 10;
        public int attackRange = 1;

        public float attackCooldown = 1;
        private float attackTimer = 0;

        #region Momvement
        public float speed = 5f;
        public float acceleration = 0.5f;
        public float decceleration = 0.5f;

        public Vector2 direction = Vector2.Zero;
        #endregion

        private Entity player;
        private HealthComponent playerHealthComponent;
        private Player playerScript;

        public void OnCreate()
        {
            direction = Vector2.Right;


            player = Entity.FindEntityByName("Player");
            collider = GetComponent<BoxCollider2DComponent>();
            transform = GetComponent<TransformComponent>();
            rigidBody = GetComponent<RigidBody2DComponent>();
        }

        public void OnUpdate(float ts)
        {
            if (healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if (playerHealthComponent == null) playerHealthComponent = player.As<HealthComponent>();
            if (playerScript == null) playerScript = player.As<Player>();
            if(entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if(entityTypeComponent.entityType == EntityType.NONE) entityTypeComponent.entityType = EntityType.ENEMY_SQUARE;
            UpdateTimers(ts);
            Move();
            Attack();

            if (healthComponent.IsDead())
            {
                Die();
            }
        }

        public void Attack()
        {
            if (IsPlayerInRange())
            {
                playerHealthComponent.TakeDamage(damage);
                attackTimer = 0;
            }
        }


        public void Move()
        {
            if (IsPlayerInRange())
            {
                return;
            }
            if(IsPlayerInSight())
            {
                TurnTowardPlayer();
                if(!GroundCheckLeft() && !GroundCheckRight())
                {
                    return;
                }
            }
            else if ((GroundCheckMiddle()  && GroundCheckRight() && (WallCheckLeft()|| !GroundCheckLeft()) && direction.X < 0) || (GroundCheckMiddle() && GroundCheckLeft() && (WallCheckRight() || !GroundCheckRight()) && direction.X > 0))
            {
                direction.X *= -1;
            }


            float targetSpeed = direction.X * speed;

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

        private bool IsPlayerInRange()
        {
            Vector2 edgeStartPoint = new Vector2(transform.translation.X, transform.translation.Y);
            Vector2 edgeEndPoint = new Vector2(transform.translation.X + attackRange * direction.X, transform.translation.Y);
            return player.GetComponent<BoxCollider2DComponent>().CollidesWithEdge(edgeStartPoint, edgeEndPoint);
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
        }

        public void UpdateTimers(float ts)
        {
            attackTimer += ts;
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
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X + collider.size.X / 2, transform.translation.Y - collider.size.Y), new Vector2(0.1f, 0.1f)))
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
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X - collider.size.X / 2, transform.translation.Y - collider.size.Y), new Vector2(0.1f, 0.1f)))
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
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X - collider.size.X, transform.translation.Y + collider.size.Y), new Vector2(0.1f, 0.1f)))
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
                if (groundCollider.CollidesWithBox(new Vector2(transform.translation.X + collider.size.X, transform.translation.Y + collider.size.Y), new Vector2(0.1f, 0.1f)))
                {
                    return true;
                }
            }

            return false;
        }
        private void Die()
        {
            Entity.Destroy(entity);
            playerScript.SwitchPlayerType(PlayerType.SQUARE);
        }
    };
}
