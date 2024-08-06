#include "Inventory.hpp"
#include "Item.hpp"

#ifdef DEMO

static void MoveItemToSlot(int* pItems, int item, int index)
{
	if (index < 0 || index >= C_MAX_INVENTORY_ITEMS)
		return;

	if (pItems[index] == item)
		return;

	// search for the item, if it doesn't exist, return

	int i = 0;
	for (; i < C_MAX_INVENTORY_ITEMS; i++)
	{
		if (pItems[i] == item)
			break;
	}

	if (i == C_MAX_INVENTORY_ITEMS)
		return;

	// swap the slot where our `item` was, and the slot at the index
	int oldItem = pItems[index];
	pItems[index] = pItems[i];
	pItems[i] = oldItem;

#ifndef ORIGINAL_CODE
	if (item > 0)
#endif
	printf("adding item: %s to %d\n", Tile::tiles[item]->getDescriptionId().c_str(), index);
}

static void ShuffleInventoryForDemo(int* pHotbar, int* pItems)
{
	pHotbar[0] = Tile::wood->id;
	pHotbar[1] = Tile::stoneBrick->id;
	pHotbar[2] = Tile::sandStone->id;
	pHotbar[3] = Tile::dirt->id;
	pHotbar[4] = Tile::redBrick->id;
	pHotbar[5] = Tile::rock->id;
	pHotbar[6] = Tile::torch->id;
	pHotbar[7] = Tile::ladder->id;
#ifdef ENH_ENABLE_9TH_SLOT
	pHotbar[8] = Tile::rose->id;
#endif

	MoveItemToSlot(pItems, pHotbar[0], 27);
	MoveItemToSlot(pItems, pHotbar[1], 28);
	MoveItemToSlot(pItems, pHotbar[2], 29);
	MoveItemToSlot(pItems, pHotbar[3], 30);
	MoveItemToSlot(pItems, pHotbar[4], 31);
	MoveItemToSlot(pItems, pHotbar[5], 32);
	MoveItemToSlot(pItems, pHotbar[6], 33);
	MoveItemToSlot(pItems, pHotbar[7], 34);
	MoveItemToSlot(pItems, Tile::flower->id, 35);
	MoveItemToSlot(pItems, Tile::cloth_10->id, 18);
	MoveItemToSlot(pItems, Tile::cloth_20->id, 19);
	MoveItemToSlot(pItems, Tile::cloth_30->id, 20);
	MoveItemToSlot(pItems, Tile::cloth_40->id, 21);
	MoveItemToSlot(pItems, Tile::cloth_50->id, 22);
	MoveItemToSlot(pItems, Tile::cloth_60->id, 23);
	MoveItemToSlot(pItems, Tile::cloth_70->id, 24);
	MoveItemToSlot(pItems, Tile::sand->id, 25);
	MoveItemToSlot(pItems, Tile::glass->id, 26);
	MoveItemToSlot(pItems, Tile::mushroom1->id, 1);
	MoveItemToSlot(pItems, Tile::obsidian->id, 8);

#ifndef ORIGINAL_CODE
	// @NOTE: For Testing
	//pHotbar[1] = Item::camera->m_itemID;
	//pHotbar[2] = Tile::tnt->id;
	//pHotbar[3] = Tile::water->id;
	//pHotbar[4] = Tile::lava->id;
#endif
}

#endif

Inventory::Inventory(Player* pPlayer)
{
	m_pPlayer = pPlayer;
	m_SelectedHotbarSlot = 0;

	for (int i = 0; i < C_MAX_HOTBAR_ITEMS; i++)
		m_hotbar[i] = -1;
	for (int i = 0; i < C_MAX_INVENTORY_ITEMS; i++)
		m_items[i] = -1;

	// @NOTE: This layout of the hotbar and inventory can be seen in the following video,
	// titled "Minecraft - Pocket Edition on Xperia Play".
	// https://www.youtube.com/watch?v=jO-y5wzmK4E

	m_hotbar[0]  = Tile::wood->id;
	m_hotbar[1]  = Tile::cloth_10->id;
	m_hotbar[2]  = Tile::cloth_20->id;
	m_hotbar[3]  = Tile::cloth_30->id;
	m_hotbar[4]  = Tile::cloth_40->id;
	m_hotbar[5]  = Tile::cloth_50->id;
	m_hotbar[6]  = Tile::cloth_60->id;
	m_hotbar[7]  = Tile::ladder->id;

	// slot 8 missing. I assume that's the "..." button

	m_items[0] = Tile::rock->id;
	m_items[1] = Tile::stoneBrick->id;
	m_items[2] = Tile::sandStone->id;
	m_items[3] = Tile::wood->id;
	m_items[4] = Tile::treeTrunk->id;
	m_items[5] = Tile::goldBlock->id;
	m_items[6] = Tile::ironBlock->id;
	m_items[7] = Tile::emeraldBlock->id;
	m_items[8] = Tile::redBrick->id;
	m_items[9] = Tile::leaves->id;
	m_items[10] = Tile::cloth_10->id;
	m_items[11] = Tile::cloth_20->id;
	m_items[12] = Tile::cloth_30->id;
	m_items[13] = Tile::cloth_40->id;
	m_items[14] = Tile::cloth_50->id;
	m_items[15] = Tile::cloth_60->id;
	m_items[16] = Tile::cloth_70->id;
	m_items[17] = Tile::glass->id;
	m_items[18] = Tile::cloth_01->id;
	m_items[19] = Tile::cloth_11->id;
	m_items[20] = Tile::cloth_21->id;
	m_items[21] = Tile::cloth_31->id;
	m_items[22] = Tile::cloth_41->id;
	m_items[23] = Tile::stairs_wood->id;
	m_items[24] = Tile::stairs_stone->id;
	m_items[25] = Tile::stoneSlabHalf->id;
	m_items[26] = Tile::sand->id;
	m_items[27] = Tile::ladder->id;
	m_items[28] = Tile::torch->id;
	m_items[29] = Tile::flower->id;
	m_items[30] = Tile::rose->id;
	m_items[31] = Tile::mushroom1->id;
	m_items[32] = Tile::mushroom2->id;
	m_items[33] = Tile::reeds->id;
	m_items[34] = Tile::obsidian->id;
	m_items[35] = Tile::dirt->id;

#ifdef DEMO
	ShuffleInventoryForDemo(m_hotbar, m_items);
#endif

#ifdef ENH_EXTRA_ITEMS_IN_INV
	// populate the 5th row now with items that might be of interest
	m_items[36] = Tile::tnt->id;
	m_items[37] = Item::camera->m_itemID;
	m_items[38] = Item::door_wood->m_itemID;
	m_items[39] = Tile::gravel->id;
	m_items[40] = Tile::cloth->id;
	m_items[41] = Tile::clay->id;
	m_items[42] = Tile::farmland->id;
	m_items[43] = Tile::lapisOre->id;
	m_items[44] = Item::door_iron->m_itemID;
#endif
}

#ifdef ENH_ENABLE_9TH_SLOT
#define HOTBAR_DIFF 0
#else
#define HOTBAR_DIFF 1
#endif

int Inventory::getSelectionSize()
{
	return C_MAX_HOTBAR_ITEMS;
}

int Inventory::getSelectionSlotItemId(int slot)
{
	if (slot >= 0 && slot < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return m_hotbar[slot];

	if (slot > C_MAX_HOTBAR_ITEMS + C_MAX_INVENTORY_ITEMS - 1 || slot < 0)
		return -1;

	return m_items[slot - C_MAX_HOTBAR_ITEMS];
}

void Inventory::setSelectionSlotItemId(int slotNo, int item)
{
	if (slotNo >= 0 && slotNo < C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		m_hotbar[slotNo] = item;
}

int Inventory::getSelectedItemId()
{
	return getSelectionSlotItemId(m_SelectedHotbarSlot);
}

void Inventory::selectSlot(int slotNo)
{
	if (slotNo < 0 || slotNo >= C_MAX_HOTBAR_ITEMS - HOTBAR_DIFF)
		return;

	m_SelectedHotbarSlot = slotNo;
}
