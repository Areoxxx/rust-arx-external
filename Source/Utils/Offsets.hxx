#pragma once

#define C_ENGINE(name, type, offset) inline constexpr type name = static_cast<type>(offset)

namespace Offsets
{
	C_ENGINE(Il2CppGetHandle, uint64_t, 0xe7cbf30);
	C_ENGINE(BaseNetworkable, uint64_t, 0xe4910a0);
	C_ENGINE(ListComponent_Projectile__c, uint64_t, 0xE4C31A8);

	namespace BaseNetworkableClass
	{
		C_ENGINE(static_fields, uint64_t, 0xB8);
		C_ENGINE(wrapper_class_ptr, uint64_t, 0x38);
		C_ENGINE(parent_static_fields, uint64_t, 0x10);
		C_ENGINE(entity, uint64_t, 0x18);
		C_ENGINE(List, uint64_t, 0x10);
		C_ENGINE(Count, uint64_t, 0x18);
		C_ENGINE(prefabID, uint64_t, 0x30);
		C_ENGINE(class_name, uint64_t, 0x10);
	}

	namespace Camera
	{
		C_ENGINE(Base, uint64_t, 0xe485658);
		C_ENGINE(StaticOffset, uint64_t, 0xb8);
		C_ENGINE(ObjectOffset, uint64_t, 0x80);
		C_ENGINE(Offset, uint64_t, 0x10);
		C_ENGINE(ViewMatrix, uint64_t, 0x30C);
	}

	namespace ConvarGraphics
	{
		C_ENGINE(Base, uint64_t, 0xe43c980);
		C_ENGINE(fov, uint64_t, 0x2e0);
	}

	namespace TodSky
	{
		C_ENGINE(Class, uint64_t, 0xe436d28);
		C_ENGINE(Instance, uint64_t, 0x88);
		C_ENGINE(Deref1, uint64_t, 0x50);
		C_ENGINE(Deref2, uint64_t, 0x20);
		C_ENGINE(Cycle, uint64_t, 0x40);
		C_ENGINE(Atmosphere, uint64_t, 0x50);
		C_ENGINE(Day, uint64_t, 0x58);
		C_ENGINE(Night, uint64_t, 0x60);
		C_ENGINE(Stars, uint64_t, 0x78);

		namespace TOD_CycleParameters
		{
			C_ENGINE(Hour, uint64_t, 0x10);
		}

		namespace TOD_AtmosphereParameters
		{
			C_ENGINE(RayleighMultiplier, uint64_t, 0x10);
			C_ENGINE(MieMultiplier, uint64_t, 0x14);
			C_ENGINE(Brightness, uint64_t, 0x18);
			C_ENGINE(Contrast, uint64_t, 0x1C);
			C_ENGINE(Directionality, uint64_t, 0x20);
			C_ENGINE(Fogginess, uint64_t, 0x2C);
		}

		namespace TOD_DayParameters
		{
			C_ENGINE(LightColor, uint64_t, 0x10);
			C_ENGINE(LightIntensity, uint64_t, 0x48);
			C_ENGINE(SkyColor, uint64_t, 0x20);
			C_ENGINE(SkyMultiplier, uint64_t, 0x34);
			C_ENGINE(AmbientMultiplier, uint64_t, 0x54);
			C_ENGINE(ReflectionMultiplier, uint64_t, 0x40);
		}

		namespace TOD_NightParameters
		{
			C_ENGINE(LightColor, uint64_t, 0x10);
			C_ENGINE(LightIntensity, uint64_t, 0x50);
			C_ENGINE(SkyColor, uint64_t, 0x20);
			C_ENGINE(SkyMultiplier, uint64_t, 0x54);
			C_ENGINE(AmbientMultiplier, uint64_t, 0x5C);
			C_ENGINE(ReflectionMultiplier, uint64_t, 0x64);
		}

		namespace TOD_StarParameters
		{
			C_ENGINE(Size, uint64_t, 0x10);
			C_ENGINE(Brightness, uint64_t, 0x14);
		}
	}

	namespace GameObject
	{
		C_ENGINE(Object, uint64_t, 0x10);
		C_ENGINE(Transform, uint64_t, 0x30);
		C_ENGINE(PrefabName, uint64_t, 0x60);
	}

	namespace TransformChain
	{
		C_ENGINE(VisualState, uint64_t, 0x38);
		C_ENGINE(StateData, uint64_t, 0x8);
		C_ENGINE(TransformArray, uint64_t, 0x38);
		C_ENGINE(Position, uint64_t, 0x90);
	}

	namespace BasePlayer
	{
		C_ENGINE(playerModel, uint64_t, 0x2f8);
		C_ENGINE(playerEyes, uint64_t, 0x458);
		C_ENGINE(playerInventory, uint64_t, 0x698);
		C_ENGINE(clActiveItem, uint64_t, 0x4e8);
		C_ENGINE(baseMovement, uint64_t, 0x518);
		C_ENGINE(playerFlags, uint64_t, 0x630);
		C_ENGINE(playerInput, uint64_t, 0x390);
		C_ENGINE(_displayName, uint64_t, 0x2c8);
		C_ENGINE(clothingMoveSpeedReduction, uint64_t, 0x708);
		C_ENGINE(lifestate, uint64_t, 0x270);
		C_ENGINE(currentTeam, uint64_t, 0x4b8);
		C_ENGINE(boneTransforms, uint64_t, 0x50);
		C_ENGINE(steamid, uint64_t, 0x678);
	}

	namespace BaseMovement
	{
		C_ENGINE(TargetMovement, uint64_t, 0x3C);
		C_ENGINE(GravityMultiplier, uint64_t, 0xB0);
		C_ENGINE(GroundAngle, uint64_t, 0xF0);
		C_ENGINE(Grounded, uint64_t, 0x54);
		C_ENGINE(Flying, uint64_t, 0x198);
		C_ENGINE(velocity, uint64_t, 0x204);
	}

	namespace BaseMelee
	{
		C_ENGINE(MaxDistance, uint64_t, 0x340);
	}

	namespace PlayerModel
	{
		C_ENGINE(position, uint64_t, 0x1f8);
		C_ENGINE(SkinnedMultiMesh, uint64_t, 0x2b8);
		C_ENGINE(MaterialList, uint64_t, 0x148);
		C_ENGINE(RendererList, uint64_t, 0x128);
	}

	namespace ModelState
	{
		C_ENGINE(Base, uint64_t, 0x358);
		C_ENGINE(flags, uint64_t, 0x14);
	}

	namespace Renderer
	{
		C_ENGINE(MaterialArray, uint64_t, 0x148);
	}

	namespace BaseCombatEntity
	{
		C_ENGINE(lifestate, uint64_t, 0x270);
		C_ENGINE(_health, uint64_t, 0x27c);
		C_ENGINE(_maxHealth, uint64_t, 0x280);
	}

	namespace BaseEntity
	{
		C_ENGINE(bounds, uint64_t, 0xc8);
		C_ENGINE(model, uint64_t, 0xf0);
		C_ENGINE(SkinnedMultiMesh, uint64_t, 0x298);
		C_ENGINE(skinnedMultiMeshRenderers, uint64_t, 0x50);
	}

	namespace PlayerInventory
	{
		C_ENGINE(containerBelt, uint64_t, 0x60);
		C_ENGINE(containerMain, uint64_t, 0x58);
		C_ENGINE(containerWear, uint64_t, 0x78);
	}

	namespace Item
	{
		C_ENGINE(uid, uint64_t, 0x18);
		C_ENGINE(amount, uint64_t, 0x3c);
		C_ENGINE(heldEntity, uint64_t, 0x80);
		C_ENGINE(heldEntity_2, uint64_t, 0x80);
		C_ENGINE(ItemDefinition, uint64_t, 0xa0);
		C_ENGINE(itemMod, uint64_t, 0x160);
		C_ENGINE(entityRef_entity, uint64_t, 0x10);
	}

	namespace ItemContainer
	{
		C_ENGINE(itemList, uint64_t, 0x60);
	}

	namespace ItemDefinition
	{
		C_ENGINE(itemid, uint64_t, 0x20);
		C_ENGINE(shortname, uint64_t, 0x28);
		C_ENGINE(displayName, uint64_t, 0x40);
		C_ENGINE(category, uint64_t, 0x58);
		C_ENGINE(legacyEnglish, uint64_t, 0x20);
		C_ENGINE(legacyEnglish2, uint64_t, 0x18);
	}

	namespace BaseProjectile
	{
		C_ENGINE(heldentity, uint64_t, 0x30);
		C_ENGINE(ItemDefinition, uint64_t, 0xa0);


		C_ENGINE(RecoilProperties, uint64_t, 0x3A8);



		C_ENGINE(primaryMagazine, uint64_t, 0x380);



		C_ENGINE(reloadTime, uint64_t, 0x378);



		C_ENGINE(aimSway, uint64_t, 0x3A0);
		C_ENGINE(aimSwaySpeed, uint64_t, 0x3A4);
		C_ENGINE(internalBurstFireRateScale, uint64_t, 0x3E8);
		C_ENGINE(automatic, uint64_t, 0x338);
		C_ENGINE(noHeadshots, uint64_t, 0x2BE);
		C_ENGINE(deployDelay, uint64_t, 0x288);
		C_ENGINE(repeatDelay, uint64_t, 0x294);
		C_ENGINE(successFraction, uint64_t, 0x458);
		C_ENGINE(canChangeFireModes, uint64_t, 0x3d8);
		C_ENGINE(isBurstWeapon, uint64_t, 0x3D7);
		C_ENGINE(ammoType, uint64_t, 0x20);
		C_ENGINE(fractionalReload, uint64_t, 0x360);
		C_ENGINE(numShotsFired, uint64_t, 0x3CC);
		C_ENGINE(aimconeCurve, uint64_t, 0x3B0);


		C_ENGINE(aimCone, uint64_t, 0x3B8);



		C_ENGINE(hipAimCone, uint64_t, 0x3BC);



		C_ENGINE(aimconePenaltyPerShot, uint64_t, 0x3C0);
		C_ENGINE(aimConePenaltyMax, uint64_t, 0x3C4);
		C_ENGINE(stancePenaltyScale, uint64_t, 0x3D0);
		C_ENGINE(projectileVelocity, uint64_t, 0x40);
	}

	namespace BowWeapon
	{
		C_ENGINE(attackReady, uint64_t, 0x460);
		C_ENGINE(arrowBack, uint64_t, 0x45C);
		C_ENGINE(swapArrows, uint64_t, 0x460);
		C_ENGINE(wasAiming, uint64_t, 0x470);
	}

	namespace Magazine
	{
		C_ENGINE(capacity, uint64_t, 0x18);
		C_ENGINE(contents, uint64_t, 0x1C);
	}

	namespace RecoilProperties
	{
		C_ENGINE(recoilYawMin, uint64_t, 0x18);
		C_ENGINE(recoilYawMax, uint64_t, 0x1C);
		C_ENGINE(recoilPitchMin, uint64_t, 0x20);
		C_ENGINE(recoilPitchMax, uint64_t, 0x24);
		C_ENGINE(newRecoilOverride, uint64_t, 0x80);
	}

	namespace Projectile
	{
		C_ENGINE(velocity, uint64_t, 0x15c);
		C_ENGINE(spread, uint64_t, 0x38);
		C_ENGINE(drag, uint64_t, 0x34);
		C_ENGINE(gravityModifier, uint64_t, 0x38);
		C_ENGINE(thickness, uint64_t, 0x3c);
	}

	namespace Corpse
	{
		C_ENGINE(parentName, uint64_t, 0x328);
	}

	namespace DroppedItem
	{
		C_ENGINE(item, uint64_t, 0x2a8);
	}

	namespace WorldItem
	{
		C_ENGINE(item, uint64_t, 0x1e0);
	}

	namespace HeldEntity
	{
		C_ENGINE(viewModel, uint64_t, 0x278);
		C_ENGINE(baseviewModel, uint64_t, 0x28);
	}

	namespace PlayerEyes
	{
		C_ENGINE(viewOffset, uint64_t, 0x40);
		C_ENGINE(bodyRotation, uint64_t, 0x50);
		C_ENGINE(headAngles, uint64_t, 0x54);
	}

	namespace PlayerInput
	{
		C_ENGINE(bodyAngles, uint64_t, 0x44);
	}
}
