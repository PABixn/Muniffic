using eg;
using Quest.Source;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class Player : DefaultBehaviour
    {

        RigidBody2DComponent rigidBody;
        BoxCollider2DComponent collider;
        TransformComponent transform;

        public HealthComponent healthComponent;

        public BoxCollider2DComponent groundCheck;

        private Vector2 direction = Vector2.Zero;
        public float maxSpeed = 10f;
        public float acceleration = 1f;
        public float decceleration = 1f;

        public bool shouldJump = false;
        public float jumpForce = 10f;
        public float coyoteTime = 0.1f;
        private float coyoteTimer = 0f;
        public float jumpBuffer = 0.1f;
        private float jumpBufferTimer = 0f;

        private List<Entity> enemies = new List<Entity>();
        private List<Entity> enemiesInRange = new List<Entity>();

        private Vector2 attackDirecton = Vector2.Right;
        public Vector2 attackBoxSize = new Vector2(1, 1);
        public Vector2 attackBoxOffset = new Vector2(1, 0);
        public Vector2 attackBoxCenter;

        private float attackCooldown = 0.5f;
        private float attackTimer = 0f;
        private int damage = 10;

        public void OnCreate()
        {

            transform = GetComponent<TransformComponent>();
            rigidBody = GetComponent<RigidBody2DComponent>();
            collider = GetComponent<BoxCollider2DComponent>();
            

            groundCheck = Entity.FindEntityByName("GroundCheck").GetComponent<BoxCollider2DComponent>();

        }

        public void OnUpdate(float ts)
        {
            if(collider == null) return;
            if(healthComponent == null) healthComponent = entity.As<HealthComponent>();

            updateTimers(ts);
            UpdateAttackBox();
            direction = Vector2.Zero;
            if (Input.IsKeyDown(KeyCode.A) && !Input.IsKeyDown(KeyCode.D))
            {
                direction = new Vector2(-1, 0);
                attackDirecton = new Vector2(-1, 0);
            }
            if (Input.IsKeyDown(KeyCode.D) && !Input.IsKeyDown(KeyCode.A))
            {
                direction = new Vector2(1, 0);
                attackDirecton = new Vector2(1, 0);
            }

            //Should be moved to fixed update
            Jump();

            if (rigidBody != null)
            {
                //should be moved to fixed update
                Run(direction);
            }
            Attack();
        }

        private void Run(Vector2 velocity)
        {
            float targetSpeed = velocity.X * maxSpeed;

            float accelRate = (Math.Abs(targetSpeed) > 0.01f) ? acceleration : decceleration;

            float speedDif = targetSpeed - rigidBody.linearVelocity.X;

            float movement = (float)speedDif * accelRate;
            
            rigidBody.ApplyLinearImpulse(Vector2.Right * movement);
        }

        private void Jump()
        {
            if (rigidBody == null) return;

            if(coyoteTimer > 0 && jumpBufferTimer > 0)
            {
                rigidBody.linearVelocity = new Vector2(rigidBody.linearVelocity.X, jumpForce);
                jumpBufferTimer = 0;
                shouldJump = false;
            }
            if(!Input.IsKeyDown(KeyCode.Space) && rigidBody.linearVelocity.Y > 0.0f)
            {
                rigidBody.linearVelocity = new Vector2(rigidBody.linearVelocity.X, rigidBody.linearVelocity.Y * 0.5f);
                coyoteTimer = 0;
            }
        }

        private void updateTimers(float ts)
        {
            if(isGrounded()){
                coyoteTimer = coyoteTime;
            }
            else
            {
                coyoteTimer -= ts;
            }

            if(Input.IsKeyDown(KeyCode.Space))
            {
                jumpBufferTimer = jumpBuffer;
            }
            else
            {
                jumpBufferTimer -= ts;
            }
            attackTimer += ts;
        }

        private bool isGrounded()
        {
            return GroundCheck.IsGrounded(collider);
        }

        private void getEnemiesInRange()
        {
            enemiesInRange.Clear();
            foreach (Entity enemy in Entity.FindEntityByName("Enemies").GetChildren())
            {
                Console.WriteLine("Enemy: " + enemy.name);
                if (enemy.GetComponent<BoxCollider2DComponent>().CollidesWithBox(attackBoxCenter, attackBoxSize))
                {
                    enemiesInRange.Add(enemy);
                }
            }
        }

        private void UpdateAttackBox()
        {
            attackBoxCenter = new Vector2(transform.translation.X + attackBoxOffset.X * attackDirecton.X, transform.translation.Y + attackBoxOffset.Y);
        }

        private void Attack()
        {
            if(attackTimer >= attackCooldown && Input.IsKeyDown(KeyCode.R))
            {
                getEnemiesInRange();
                foreach(Entity enemy in enemiesInRange)
                {
                    enemy.As<BaseEnemy>().healthComponent.TakeDamage(damage);
                }
                attackTimer = 0;
            }
        }
    }
    
}
