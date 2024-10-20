using eg;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Quest{

    class SquareEnemy : BaseEnemy
    {

        bool initialized = false;
        public HealthComponent healthComponent;
        private EntityTypeComponent entityTypeComponent;
        private ShootAttackComponent shootAttackComponent;
        private EnemyRunComponent enemyRunComponent;
        private EnemyAttackBoxComponent enemyAttackBoxComponent;
        private AnimatorComponent animator;
        private RigidBody2DComponent rigidBody;

        public int Health { get; private set; } = 300;
        public float SpeedMultiplier { get; private set; } = 0.5f;
        public int AttackDamage { get; private set; } = 100;
        public float AttackSpeed{ get; private set; } = 2;
        public int KnockbackForce{ get; private set; } = 1;

        private bool died = false;

        private Entity player;

        public override void  OnCreate()
        {
            player = Entity.FindEntityByName("Player");
            animator = entity.GetComponent<AnimatorComponent>();
            rigidBody = entity.GetComponent<RigidBody2DComponent>();
            animator.ChangeAnimation("squareEnemyIdle");
            animator.Play("squareEnemyIdle");
        }

        public override void OnUpdate(float ts)
        {
            if (died) return;
            if (healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if (enemyRunComponent == null)
            {
                enemyRunComponent = entity.As<EnemyRunComponent>(); 
            }
            if(enemyAttackBoxComponent == null) enemyAttackBoxComponent = entity.As<EnemyAttackBoxComponent>();
            if(shootAttackComponent == null) shootAttackComponent = new ShootAttackComponent(entity, new List<EntityType> { EntityType.PLAYER }, "PlayerWrapper", enemyAttackBoxComponent, "squareEnemyAttack");
            if (!initialized) Init();
            if(entityTypeComponent.entityType == EntityType.NONE) entityTypeComponent.entityType = EntityType.ENEMY_SQUARE;

            shootAttackComponent.Update(ts);

            if (healthComponent.IsDead())
            {
                animator.ChangeAnimation("squareEnemyDeath");
                animator.Play("squareEnemyDeath");
                died = true;
                return;
            }
            if (shootAttackComponent.isShooting)
            {
                return;
            }
            if (Math.Abs(rigidBody.linearVelocity.X) > 0.1f)
            {
                animator.ChangeAnimation("squareEnemyWalk");
                animator.Play("squareEnemyWalk");
            }
            else
            {
                animator.ChangeAnimation("squareEnemyIdle");
                animator.Play("squareEnemyIdle");
            }
        }

        public void Atack()
        {
            shootAttackComponent.Attack();
        }

        public void TransitionFromAttack()
        {
            animator.ChangeAnimation("squareEnemyIdle");
            animator.Play("squareEnemyIdle");
            shootAttackComponent.SetIsShooting(false);
        }

        protected override void Init()
        {
            healthComponent.maxHealth = Health;
            healthComponent.SetHealth(Health);
            shootAttackComponent.SetDamage(AttackDamage);
            shootAttackComponent.SetCooldown(AttackSpeed);
            shootAttackComponent.SetKnockback(KnockbackForce);
            enemyRunComponent.SetMultiplier(SpeedMultiplier);
            initialized = true;
        }

        protected override void Die()
        {
            Entity.Destroy(entity);
        }
    }

};
