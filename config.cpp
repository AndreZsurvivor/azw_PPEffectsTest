class CfgPatches
{
	class azw_PPEffectsTest
	{
		units[]				=	{"azw_EffectTest"};
		weapons[]			=	{};
		requiredVersion		=	0.1;
		requiredAddons[]	=	{"DZ_Data"};
	};
};
class CfgMods
{
	class azw_PPEffectsTest
	{
		name				=	"azw_PPEffectsTest";
		credits				=	"AndreZ";
		author				=	"AndreZ";
		authorID			=	"0";
		type				=	"mod";
		dependencies[]		=	{"World"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[]		=	{"azw_PPEffectsTest/Scripts/3_Game"};
			};
			class worldScriptModule
			{
				value		=	"";
				files[]		=	{"azw_PPEffectsTest/Scripts/4_world"};
			};
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class azw_EffectTest : Inventory_Base
	{
		forceFarBubble="true";
		itemBehaviour=2;
		overrideDrawArea="8.0";
		scope=2;
		physLayer="item_large";
		useEntityHierarchy="true";
		model="DZ\gear\radio\walkietalkie.p3d";
	};
};