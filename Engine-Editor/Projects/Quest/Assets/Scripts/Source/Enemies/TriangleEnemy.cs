using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Quest
{
    public class TriangleEnemy : BaseEnemy
    {
        bool initialized = false;
        public HealthComponent healthComponent;
        private EntityTypeComponent entityTypeComponent;
        private MeleeAttackComponent meleeAttackComponent;
        private EnemyRunComponent enemyRunComponent;
        private EnemyAttackBoxComponent enemyAttackBoxComponent;
        private AnimatorComponent animator;

        private RigidBody2DComponent rigidBody;


        public int Health { get; private set; } = 250;
        public float SpeedMultiplier { get; private set; } = 0.5f;
        public int AttackDamage { get; private set; } = 80;
        public float AttackSpeed { get; private set; } = 1f;
        public int KnockbackForce { get; private set; } = 1;

        private Entity player;
        private Player playerScript;

        bool died = false;

        public override void OnCreate()
        {
            player = Entity.FindEntityByName("Player");
            animator = entity.GetComponent<AnimatorComponent>();
            rigidBody = entity.GetComponent<RigidBody2DComponent>();
            animator.ChangeAnimation("triangleEnemyIdle");
            animator.Play("triangleEnemyIdle");

        }

        public override void OnUpdate(float ts)
        {
            if (died) return;
            if (playerScript == null) playerScript = player.As<Player>();
            if (healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if (enemyRunComponent == null)
            {
                enemyRunComponent = entity.As<EnemyRunComponent>();
            }
            if(enemyAttackBoxComponent == null) enemyAttackBoxComponent = entity.As<EnemyAttackBoxComponent>();
            if(meleeAttackComponent == null) meleeAttackComponent = new MeleeAttackComponent(entity, new List<EntityType> { EntityType.PLAYER }, "PlayerWrapper", enemyAttackBoxComponent, "triangeEnemyAttack");
            if (!initialized) Init();
            meleeAttackComponent.Update(ts);
            if (entityTypeComponent.entityType == EntityType.NONE) entityTypeComponent.entityType = EntityType.ENEMY_TRIANGLE;

            if (healthComponent.IsDead())
            {
                animator.ChangeAnimation("triangleEnemyDeath");
                animator.Play("triangleEnemyDeath");
                died = true;
                return;
            }

            if (Math.Abs(rigidBody.linearVelocity.X) > 0.1f)
            {
                animator.ChangeAnimation("crabWalk");
                animator.Play("crabWalk");
            }
            else
            {
                animator.ChangeAnimation("triangleEnemyIdle");
                animator.Play("triangleEnemyIdle");
            }
        }

        public void Attack()
        {
            meleeAttackComponent.Attack();
        }

        public void TransitionFromAttack()
        {
            animator.Play("crabWalk");
        }

        protected override void Init()
        {
            healthComponent.maxHealth = Health;
            healthComponent.SetHealth(Health);
            meleeAttackComponent.SetDamage(AttackDamage);
            meleeAttackComponent.SetCooldown(AttackSpeed);
            meleeAttackComponent.SetKnockback(KnockbackForce);
            enemyRunComponent.SetMultiplier(SpeedMultiplier);
            initialized = true;
        }

        protected override void Die()
        {
            Entity sep = Entity.FindEntityByName("Separator");
            if (sep.ID != 0)
            {
                Entity.Destroy(sep);
            }
            Entity.Destroy(entity);
        }

    }
};
