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

        public int Health { get; private set; } = 250;
        public float SpeedMultiplier { get; private set; } = 1.5f;
        public int AttackDamage { get; private set; } = 80;
        public float AttackSpeed { get; private set; } = 1f;
        public int KnockbackForce { get; private set; } = 50;

        private Entity player;
        private Player playerScript;

        public override void OnCreate()
        {
            player = Entity.FindEntityByName("Player");
            meleeAttackComponent = new MeleeAttackComponent(entity,new List<EntityType> { EntityType.PLAYER },"PlayerWrapper", enemyAttackBoxComponent);
        }

        public override void OnUpdate(float ts)
        {
            if (playerScript == null) playerScript = player.As<Player>();
            if (healthComponent == null) healthComponent = entity.As<HealthComponent>();
            if(entityTypeComponent == null) entityTypeComponent = entity.As<EntityTypeComponent>();
            if (enemyRunComponent == null) enemyRunComponent = entity.As<EnemyRunComponent>();
            if (!initialized) Init();
            meleeAttackComponent.Update(ts);

            if (healthComponent.IsDead())
            {
                Die();
            }
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
            Entity.Destroy(entity);
        }

    }
};
