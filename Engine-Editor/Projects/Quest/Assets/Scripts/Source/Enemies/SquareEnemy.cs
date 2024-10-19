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

        public int Health { get; private set; } = 800;
        public float SpeedMultiplier { get; private set; } = 0.5f;
        public int AttackDamage { get; private set; } = 100;
        public float AttackSpeed{ get; private set; } = 2;
        public int KnockbackForce{ get; private set; } = 20;



        private Entity player;
        private Player playerScript;

        public override void  OnCreate()
        {
            player = Entity.FindEntityByName("Player");
        }

        public override void OnUpdate(float ts)
        {
            if (playerScript == null) playerScript = player.As<Player>();
            if (healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if (enemyRunComponent == null) enemyRunComponent = entity.As<EnemyRunComponent>();
            if(enemyAttackBoxComponent == null) enemyAttackBoxComponent = entity.As<EnemyAttackBoxComponent>();
            if(shootAttackComponent == null) shootAttackComponent = new ShootAttackComponent(entity, new List<EntityType> { EntityType.PLAYER }, "PlayerWrapper", enemyAttackBoxComponent);
            if (!initialized) Init();
            if(entityTypeComponent.entityType == EntityType.NONE) entityTypeComponent.entityType = EntityType.ENEMY_SQUARE;

            shootAttackComponent.Update(ts);

            if (healthComponent.IsDead())
            {
                Die();
            }
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
