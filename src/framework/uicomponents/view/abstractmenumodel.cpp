/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore BVBA and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "abstractmenumodel.h"

#include "types/translatablestring.h"

#include "log.h"

using namespace muse::uicomponents;
using namespace muse::ui;
using namespace muse::actions;

const int AbstractMenuModel::INVALID_ITEM_INDEX = -1;

AbstractMenuModel::AbstractMenuModel(QObject* parent)
    : QAbstractListModel(parent), muse::Injectable(muse::iocCtxForQmlObject(this))
{
}

QVariant AbstractMenuModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();

    if (!isIndexValid(row)) {
        return QVariant();
    }

    MenuItem* item = m_items.at(row);

    switch (role) {
    case ItemRole: return QVariant::fromValue(item);
    case UserRole: return QVariant();
    }

    return QVariant();
}

bool AbstractMenuModel::isIndexValid(int index) const
{
    return index >= 0 && index < m_items.size();
}

int AbstractMenuModel::rowCount(const QModelIndex&) const
{
    return m_items.count();
}

QHash<int, QByteArray> AbstractMenuModel::roleNames() const
{
    static const QHash<int, QByteArray> roles {
        { ItemRole, "itemRole" }
    };

    return roles;
}

void AbstractMenuModel::handleMenuItem(const QString& itemId)
{
    MenuItem& menuItem = findItem(itemId);

    if (menuItem.query().isValid()) {
        dispatch(menuItem.query());
    } else {
        dispatch(menuItem.action().code, menuItem.args());
    }
}

void AbstractMenuModel::dispatch(const ActionCode& actionCode, const ActionData& args)
{
    dispatcher()->dispatch(actionCode, args);
}

void AbstractMenuModel::dispatch(const muse::actions::ActionQuery& actionQuery)
{
    dispatcher()->dispatch(actionQuery);
}

QVariantMap AbstractMenuModel::get(int index)
{
    QVariantMap result;

    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i(names);
    while (i.hasNext()) {
        i.next();
        QModelIndex idx = this->index(index, 0);
        QVariant data = idx.data(i.key());
        result[i.value()] = data;
    }

    return result;
}

void AbstractMenuModel::load()
{
    uiActionsRegister()->actionStateChanged().onReceive(this, [this](const ActionCodeList& codes) {
        onActionsStateChanges(codes);
    });

    shortcutsRegister()->shortcutsChanged().onNotify(this, [this]() {
        updateShortcutsAll();
    });
}

QVariantList AbstractMenuModel::itemsProperty() const
{
    QVariantList items;

    for (MenuItem* item: m_items) {
        items << QVariant::fromValue(item);
    }

    return items;
}

const MenuItemList& AbstractMenuModel::items() const
{
    return m_items;
}

void AbstractMenuModel::setItems(const MenuItemList& items)
{
    TRACEFUNC;

    beginResetModel();
    m_items = items;
    endResetModel();

    emit itemsChanged();
}

void AbstractMenuModel::clear()
{
    setItems(MenuItemList());
}

int AbstractMenuModel::itemIndex(const QString& itemId) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i]->id() == itemId) {
            return i;
        }
    }

    return INVALID_ITEM_INDEX;
}

MenuItem& AbstractMenuModel::item(int index)
{
    MenuItem& item = *m_items[index];
    if (item.isValid()) {
        return item;
    }

    static MenuItem dummy;
    return dummy;
}

MenuItem& AbstractMenuModel::findItem(const QString& itemId)
{
    return item(m_items, itemId);
}

MenuItem& AbstractMenuModel::findItem(const ActionCode& actionCode)
{
    MenuItemList list = items(m_items, actionCode);
    if (list.empty()) {
        static MenuItem dummy;
        return dummy;
    }

    if (list.size() > 1) {
        LOGD() << "There is more than one item for " << actionCode << ", will return the first one found";
    }

    return *list.front();
}

MenuItemList AbstractMenuModel::findItems(const ActionCode& actionCode)
{
    return items(m_items, actionCode);
}

MenuItem& AbstractMenuModel::findMenu(const QString& menuId)
{
    return menu(m_items, menuId);
}

MenuItem* AbstractMenuModel::makeMenu(const TranslatableString& title, const MenuItemList& items,
                                      const QString& menuId, bool enabled)
{
    MenuItem* item = new MenuItem(this);
    item->setId(menuId);
    item->setSubitems(items);

    UiAction action;
    action.title = title;
    item->setAction(action);

    UiActionState state;
    state.enabled = enabled;
    item->setState(state);

    return item;
}

MenuItem* AbstractMenuModel::makeMenuItem(const ActionCode& actionCode, const TranslatableString& title)
{
    const UiAction& action = uiActionsRegister()->action(actionCode);
    if (!action.isValid()) {
        LOGW() << "not found action: " << actionCode;
        return nullptr;
    }

    MenuItem* item = new MenuItem(action, this);
    item->setState(uiActionsRegister()->actionState(actionCode));

    if (!title.isEmpty()) {
        item->setTitle(title);
    }

    ActionQuery q(actionCode);
    if (q.isValid()) {
        item->setQuery(q);
    }

    return item;
}

MenuItem* AbstractMenuModel::makeSeparator()
{
    MenuItem* item = new MenuItem(this);

    UiAction action;
    action.title = {};
    item->setAction(action);

    return item;
}

void AbstractMenuModel::onActionsStateChanges(const muse::actions::ActionCodeList& codes)
{
    TRACEFUNC;

    if (codes.empty()) {
        return;
    }

    std::map<actions::ActionCode, ui::UiActionState> states;
    updateState(m_items, codes, states);
}

void AbstractMenuModel::setItem(int index, MenuItem* item)
{
    if (!isIndexValid(index)) {
        return;
    }

    m_items[index] = item;

    QModelIndex modelIndex = this->index(index);
    emit dataChanged(modelIndex, modelIndex);
}

MenuItem& AbstractMenuModel::item(MenuItemList& items, const QString& itemId)
{
    for (MenuItem* menuItem : items) {
        if (!menuItem) {
            continue;
        }
        if (menuItem->id() == itemId) {
            return *menuItem;
        }

        auto subitems = menuItem->subitems();
        if (!subitems.empty()) {
            MenuItem& subitem = item(subitems, itemId);
            if (subitem.id() == itemId) {
                return subitem;
            }
        }
    }

    static MenuItem dummy;
    return dummy;
}

MenuItemList AbstractMenuModel::items(MenuItemList& items, const ActionCode& actionCode)
{
    MenuItemList result;

    for (MenuItem* menuItem : items) {
        if (!menuItem) {
            continue;
        }

        if (menuItem->action().code == actionCode) {
            result.append(menuItem);
        }

        auto subitems = menuItem->subitems();
        if (!subitems.empty()) {
            MenuItemList list = this->items(subitems, actionCode);
            if (!list.empty()) {
                result.append(list);
            }
        }
    }

    return result;
}

MenuItem& AbstractMenuModel::menu(MenuItemList& items, const QString& menuId)
{
    for (MenuItem* item : items) {
        if (!item) {
            continue;
        }

        if (item->id() == menuId) {
            return *item;
        }

        auto subitems = item->subitems();
        MenuItem& menuItem = menu(subitems, menuId);
        if (menuItem.isValid()) {
            return menuItem;
        }
    }

    static MenuItem dummy;
    return dummy;
}

void AbstractMenuModel::updateState(MenuItemList& items, const actions::ActionCodeList& codes,
                                    std::map<actions::ActionCode, ui::UiActionState>& states)
{
    for (MenuItem* menuItem : items) {
        if (!menuItem) {
            continue;
        }

        ActionCode code = menuItem->action().code;
        if (muse::contains(codes, code)) {
            if (!muse::contains(states, code)) {
                states.insert({ code, uiActionsRegister()->actionState(code) });
            }
            menuItem->setState(states.at(code));
        }

        MenuItemList subitems = menuItem->subitems();
        if (!subitems.empty()) {
            updateState(subitems, codes, states);
        }
    }
}

void AbstractMenuModel::updateShortcutsAll()
{
    for (MenuItem* menuItem : m_items) {
        if (!menuItem) {
            continue;
        }

        updateShortcuts(menuItem);
    }
}

void AbstractMenuModel::updateShortcuts(MenuItem* item)
{
    UiAction action = item->action();
    action.shortcuts = shortcutsRegister()->shortcut(action.code).sequences;
    item->setAction(action);

    for (MenuItem* subItem : item->subitems()) {
        if (!subItem) {
            continue;
        }

        updateShortcuts(subItem);
    }
}
