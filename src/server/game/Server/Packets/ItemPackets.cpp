/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ItemPackets.h"

namespace WorldPackets::Item
{
void BuyBackItem::Read()
{
    _worldPacket >> VendorGUID;
    _worldPacket >> Slot;
}

void BuyItem::Read()
{
    _worldPacket >> VendorGUID;
    _worldPacket >> ContainerGUID;
    _worldPacket >> Quantity;
    _worldPacket >> Muid;
    _worldPacket >> Slot;
    _worldPacket >> As<int32>(ItemType);
    _worldPacket >> Item;
}

WorldPacket const* BuySucceeded::Write()
{
    _worldPacket << VendorGUID;
    _worldPacket << uint32(Muid);
    _worldPacket << int32(NewQuantity);
    _worldPacket << uint32(QuantityBought);

    return &_worldPacket;
}

WorldPacket const* BuyFailed::Write()
{
    _worldPacket << VendorGUID;
    _worldPacket << uint32(Muid);
    _worldPacket << int32(Reason);

    return &_worldPacket;
}

void GetItemPurchaseData::Read()
{
    _worldPacket >> ItemGUID;
}

ByteBuffer& operator<<(ByteBuffer& data, ItemPurchaseRefundItem& refundItem)
{
    data << int32(refundItem.ItemID);
    data << int32(refundItem.ItemCount);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, ItemPurchaseRefundCurrency& refundCurrency)
{
    data << int32(refundCurrency.CurrencyID);
    data << int32(refundCurrency.CurrencyCount);

    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, ItemPurchaseContents& purchaseContents)
{
    data << uint64(purchaseContents.Money);
    for (uint32 i = 0; i < 5; ++i)
        data << purchaseContents.Items[i];

    for (uint32 i = 0; i < 5; ++i)
        data << purchaseContents.Currencies[i];

    return data;
}

WorldPacket const* SetItemPurchaseData::Write()
{
    _worldPacket << ItemGUID;
    _worldPacket << Contents;
    _worldPacket << uint32(Flags);
    _worldPacket << uint32(PurchaseTime);

    return &_worldPacket;
}

void ItemPurchaseRefund::Read()
{
    _worldPacket >> ItemGUID;
}

WorldPacket const* ItemPurchaseRefundResult::Write()
{
    _worldPacket << ItemGUID;
    _worldPacket << uint8(Result);
    _worldPacket << OptionalInit(Contents);
    _worldPacket.FlushBits();
    if (Contents)
        _worldPacket << *Contents;

    return &_worldPacket;
}

WorldPacket const* ItemExpirePurchaseRefund::Write()
{
    _worldPacket << ItemGUID;

    return &_worldPacket;
}

void RepairItem::Read()
{
    _worldPacket >> NpcGUID;
    _worldPacket >> ItemGUID;
    UseGuildBank = _worldPacket.ReadBit();
}

void SellItem::Read()
{
    _worldPacket >> VendorGUID;
    _worldPacket >> ItemGUID;
    _worldPacket >> Amount;
}

WorldPacket const* ItemTimeUpdate::Write()
{
    _worldPacket << ItemGuid;
    _worldPacket << uint32(DurationLeft);

    return &_worldPacket;
}

WorldPacket const* SetProficiency::Write()
{
    _worldPacket << uint32(ProficiencyMask);
    _worldPacket << uint8(ProficiencyClass);

    return &_worldPacket;
}

WorldPacket const* InventoryChangeFailure::Write()
{
    _worldPacket << int32(BagResult);
    _worldPacket << Item[0];
    _worldPacket << Item[1];
    _worldPacket << uint8(ContainerBSlot); // bag type subclass, used with EQUIP_ERR_EVENT_AUTOEQUIP_BIND_CONFIRM and EQUIP_ERR_WRONG_BAG_TYPE_2

    switch (BagResult)
    {
        case EQUIP_ERR_CANT_EQUIP_LEVEL_I:
        case EQUIP_ERR_PURCHASE_LEVEL_TOO_LOW:
            _worldPacket << int32(Level);
            break;
        case EQUIP_ERR_EVENT_AUTOEQUIP_BIND_CONFIRM:
            _worldPacket << SrcContainer;
            _worldPacket << int32(SrcSlot);
            _worldPacket << DstContainer;
            break;
        case EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_COUNT_EXCEEDED_IS:
        case EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_SOCKETED_EXCEEDED_IS:
        case EQUIP_ERR_ITEM_MAX_LIMIT_CATEGORY_EQUIPPED_EXCEEDED_IS:
            _worldPacket << int32(LimitCategory);
            break;
        default:
            break;
    }

    return &_worldPacket;
}

void SplitItem::Read()
{
    _worldPacket >> Inv;
    _worldPacket >> FromPackSlot;
    _worldPacket >> FromSlot;
    _worldPacket >> ToPackSlot;
    _worldPacket >> ToSlot;
    _worldPacket >> Quantity;
}

void SwapInvItem::Read()
{
    _worldPacket >> Inv;
    _worldPacket >> Slot2;
    _worldPacket >> Slot1;
}

void SwapItem::Read()
{
    _worldPacket >> Inv;
    _worldPacket >> ContainerSlotB;
    _worldPacket >> ContainerSlotA;
    _worldPacket >> SlotB;
    _worldPacket >> SlotA;
}

void AutoEquipItem::Read()
{
    _worldPacket >> Inv;
    _worldPacket >> PackSlot;
    _worldPacket >> Slot;
}

void AutoEquipItemSlot::Read()
{
    _worldPacket >> Inv;
    _worldPacket >> Item;
    _worldPacket >> ItemDstSlot;
}

void AutoStoreBagItem::Read()
{
    _worldPacket >> Inv;
    _worldPacket >> ContainerSlotB;
    _worldPacket >> ContainerSlotA;
    _worldPacket >> SlotA;
}

void DestroyItem::Read()
{
    _worldPacket >> Count;
    _worldPacket >> ContainerId;
    _worldPacket >> SlotNum;
}

WorldPacket const* SellResponse::Write()
{
    _worldPacket << VendorGUID;
    _worldPacket << uint32(ItemGUIDs.size());
    _worldPacket << int32(Reason);
    for (ObjectGuid const& itemGuid : ItemGUIDs)
        _worldPacket << itemGuid;

    return &_worldPacket;
}

WorldPacket const* ItemPushResult::Write()
{
    _worldPacket << PlayerGUID;
    _worldPacket << uint8(Slot);
    _worldPacket << int32(SlotInBag);
    _worldPacket << int32(QuestLogItemID);
    _worldPacket << int32(Quantity);
    _worldPacket << int32(QuantityInInventory);
    _worldPacket << int32(DungeonEncounterID);
    _worldPacket << int32(BattlePetSpeciesID);
    _worldPacket << int32(BattlePetBreedID);
    _worldPacket << uint8(BattlePetBreedQuality);
    _worldPacket << int32(BattlePetLevel);
    _worldPacket << ItemGUID;

    _worldPacket << Bits<1>(Pushed);
    _worldPacket << Bits<1>(Created);
    _worldPacket << Bits<1>(Unused_1017);
    _worldPacket << Bits<3>(DisplayText);
    _worldPacket << Bits<1>(IsBonusRoll);
    _worldPacket << Bits<1>(IsEncounterLoot);
    _worldPacket.FlushBits();

    _worldPacket << Item;

    return &_worldPacket;
}

void ReadItem::Read()
{
    _worldPacket >> PackSlot;
    _worldPacket >> Slot;
}

WorldPacket const* ReadItemResultFailed::Write()
{
    _worldPacket << Item;
    _worldPacket << uint32(Delay);
    _worldPacket.WriteBits(Subcode, 2);

    _worldPacket.FlushBits();

    return &_worldPacket;
}

WorldPacket const* ReadItemResultOK::Write()
{
    _worldPacket << Item;

    return &_worldPacket;
}

void WrapItem::Read()
{
    _worldPacket >> Inv;
}

void CancelTempEnchantment::Read()
{
    _worldPacket >> Slot;
}

WorldPacket const* ItemCooldown::Write()
{
    _worldPacket << ItemGuid;
    _worldPacket << uint32(SpellID);
    _worldPacket << uint32(Cooldown);

    return &_worldPacket;
}

WorldPacket const* EnchantmentLog::Write()
{
    _worldPacket << Owner;
    _worldPacket << Caster;
    _worldPacket << ItemGUID;
    _worldPacket << int32(ItemID);
    _worldPacket << int32(Enchantment);
    _worldPacket << int32(EnchantSlot);

    return &_worldPacket;
}

WorldPacket const* ItemEnchantTimeUpdate::Write()
{
    _worldPacket << ItemGuid;
    _worldPacket << uint32(DurationLeft);
    _worldPacket << uint32(Slot);
    _worldPacket << OwnerGuid;

    return &_worldPacket;
}

void UseCritterItem::Read()
{
    _worldPacket >> ItemGuid;
}

void SocketGems::Read()
{
    _worldPacket >> ItemGuid;
    for (ObjectGuid& gemGuid : GemItem)
        _worldPacket >> gemGuid;
}

WorldPacket const* SocketGemsSuccess::Write()
{
    _worldPacket << Item;

    return &_worldPacket;
}

void RemoveNewItem::Read()
{
    _worldPacket >> ItemGuid;
}

void ReforgeItem::Read()
{
    _worldPacket >> ReforgerGUID;
    _worldPacket >> ContainerId;
    _worldPacket >> SlotNum;
    _worldPacket >> ItemReforgeRecId;
}

WorldPacket const* AddItemPassive::Write()
{
    _worldPacket << int32(SpellID);

    return &_worldPacket;
}

WorldPacket const* RemoveItemPassive::Write()
{
    _worldPacket << int32(SpellID);

    return &_worldPacket;
}

WorldPacket const* SendItemPassives::Write()
{
    _worldPacket << uint32(SpellID.size());
    if (!SpellID.empty())
        _worldPacket.append(SpellID.data(), SpellID.size());

    return &_worldPacket;
}
}
