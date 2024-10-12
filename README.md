# My-Candy
Store some files that I usually play in Minecraft.

Minecraft ！IVE Minecraft，this' my candy！




# Minecraft NBT 语法和指令

------



```java
1.20.5+ nbt.txt
获得一个无法破坏效率4的金锄
/give @s minecraft:golden_hoe[minecraft:unbreakable={evels:{"unbreakable":1b}},minecraft:enchantments={"efficiency":4}] <数量>


咬人虎[无法破坏]
/give @s minecraft:golden_axe[minecraft:unbreakable={evels:{"unbreakable":1b}},minecraft:enchantments={"sharpness":255}]


抢夺者
/give @s minecraft:golden_axe[minecraft:unbreakable={evels:{"unbreakable":1b}},minecraft:enchantments={"looting":128}]


```

------



```java
工具类.txt
获取一个名称为“极好的剑”、拥有附魔锋利6和耐久2，且无法损坏的金剑。

/give @s minecraft:golden_sword{display:{Name:'["极好的剑"]'},Enchantments:[{id:"minecraft:sharpness",lvl:6s},{id:"minecraft:unbreaking",lvl:2s}],Unbreakable:1b}




获取一个穿在身上时提供20（Armor × 10）点盔甲值的鞘翅。

/give @s minecraft:elytra{AttributeModifiers:[{AttributeName:"generic.armor",Operation:0,Amount:20d,Slot:"chest",Name:"Armor",UUID:[I;1,2,3,4]}]}

/give @s minecraft:leather_chestplate{display:{Name:'["创造者护甲"]'},Enchantments:[{id:"minecraft:protection",lvl:255},{id:"minecraft:fire_protection",lvl:255},{id:"minecraft:unbreaking",lvl:6}],AttributeModifiers:[{AttributeName:"generic.armor",Operation:0,Amount:20d,Slot:"chest",Name:"Armor",UUID:[I;1,2,3,4]}],Unbreakable:1b}


鱼竿
/give @s minecraft:fishing_rod{display:{Name:'["钓鱼佬"]'},Enchantments:[{id:"minecraft:multishot",lvl:255},{id:"minecraft:loyalty",lvl:3},{id:"minecraft:efficiency",lvl:5}],Unbreakable:1b}

minecraft:efficiency

不损之剑
/give @s minecraft:golden_sword{display:{Name:'["不损之剑"]'},Enchantments:[{id:"minecraft:sharpness",lvl:255},{id:"minecraft:unbreaking",lvl:255}],Unbreakable:1b}

/give @s minecraft:diamond_axe{display:{Name:'["创造之斧！"]'},Enchantments:[{id:"minecraft:efficiency",lvl:255},{id:"minecraft:sharpness",lvl:32},{id:"minecraft:unbreaking",lvl:255}],Unbreakable:1b}

/give @s minecraft:diamond_pickaxe{display:{Name:'["创造之镐！"]'},Enchantments:[{id:"minecraft:efficiency",lvl:255},{id:"minecraft:fortune",lvl:25},{id:"minecraft:unbreaking",lvl:255}],Unbreakable:1b}

/give @s minecraft:shield{display:{Name:'["不损之盾"]'},Enchantments:[{id:"minecraft:protection",lvl:32},{id:"minecraft:unbreaking",lvl:32}],Unbreakable:1b}

/give @s minecraft:diamond_hoe{display:{Name:'["创造之锄！"]'},Enchantments:[{id:"minecraft:efficiency",lvl:255},{id:"minecraft:unbreaking",lvl:255}],Unbreakable:1b}

/give @s minecraft:diamond_shovel{display:{Name:'["创造之铲！"]'},Enchantments:[{id:"minecraft:efficiency",lvl:5},{id:"minecraft:unbreaking",lvl:255}],Unbreakable:1b}


咬人虎
/give @s minecraft:golden_axe{display:{Name:'["咬人虎"]'},Enchantments:[{id:"minecraft:sharpness",lvl:255}],Unbreakable:1b}

/give @s minecraft:golden_axe{display:{Name:'["抢夺者"]'},Enchantments:[{id:"minecraft:looting",lvl:128},{id:"minecraft:sharpness",lvl:32},{id:"minecraft:fire_aspect",lvl:1s}],Unbreakable:1b}


/give @s minecraft:totem_of_undying{Enchantments:[{id:"minecraft:infinity",lvl:255},{id:"minecraft:unbreaking",lvl:2s}],Unbreakable:1b}



/give @s minecraft:diamond_sword{display:{Name:'["极好的剑"]'},Enchantments:[{id:"minecraft:sharpness",lvl:255}],Unbreakable:1b}


/give @s minecraft:stick{display:{Name:'["鸡腿255"]'},Enchantments:[{id:"knockback",lvl:255}],Unbreakable:1b}

seed:2786113791405310198




Attack_Speed


/give @p minecraft:netherite_sword{AttributeModifiers:["attack_speed","lvl:6"]}


AttributeModifiers:["attack_speed","lvl:6"]


/give @s minecraft:golden_axe{display:{Name:'["咬人虎"]'},Enchantments:[{id:"minecraft:sharpness",lvl:255}],AttributeModifiers:["attack_speed","lvl:6"],Unbreakable:1b}
```

------



```java
盔甲.txt
/give @s minecraft:diamond_boots{display:{Name:'["创造者护甲"]'},Enchantments:[{id:"minecraft:protection",lvl:255},{id:"minecraft:fire_protection",lvl:255},{id:"minecraft:unbreaking",lvl:6}],Unbreakable:1b}

/give @s minecraft:diamond_helmet{display:{Name:'["创造者护甲"]'},Enchantments:[{id:"minecraft:protection",lvl:255},{id:"minecraft:fire_protection",lvl:255},{id:"minecraft:unbreaking",lvl:6}],Unbreakable:1b}

/give @s minecraft:diamond_leggings{display:{Name:'["创造者护甲"]'},Enchantments:[{id:"minecraft:protection",lvl:255},{id:"minecraft:fire_protection",lvl:255},{id:"minecraft:unbreaking",lvl:6}],Unbreakable:1b}

/give @s minecraft:diamond_chestplate{display:{Name:'["创造者护甲"]'},Enchantments:[{id:"minecraft:protection",lvl:255},{id:"minecraft:fire_protection",lvl:255},{id:"minecraft:unbreaking",lvl:6}],Unbreakable:1b}


鞘翅
/give @s minecraft:elytra{Enchantments:[{id:"minecraft:protection",lvl:255}],AttributeModifiers:[{AttributeName:"generic.armor",Operation:0,Amount:20d,Slot:"chest",Name:"Armor",UUID:[I;1,2,3,4]}],Unbreakable:1b}


/give @s minecraft:diamond_chestplate{display:{Name:'["创造者护甲"]'},Enchantments:[{id:"minecraft:protection",lvl:255}],AttributeModifiers:[{AttributeName:"generic.armor",Operation:0,Amount:20d,Slot:"chest",Name:"Armor",UUID:[I;1,2,3,4]}],Unbreakable:1b}

```
