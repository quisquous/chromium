// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef VIEWS_CONTROLS_MENU_MENU_2_H_
#define VIEWS_CONTROLS_MENU_MENU_2_H_

#include "app/menus/menu_model.h"
#include "base/scoped_ptr.h"
#include "views/controls/menu/menu_wrapper.h"

namespace gfx {
class Point;
}

namespace views {

class NativeMenuGtk;
class View;

// A menu. Populated from a model, and relies on a delegate to execute commands.
//
// WARNING: do NOT create and use Menu2 on the stack. Menu2 notifies the model
// of selection AFTER a delay. This means that if use a Menu2 on the stack
// ActivatedAt is never invoked.
class Menu2 {
 public:
  // Creates a new menu populated with the contents of |model|.
  // WARNING: this populates the menu on construction by invoking methods on
  // the model. As such, it is typically not safe to use this as the model
  // from the constructor. EG:
  //   MyClass : menu_(this) {}
  // is likely to have problems.
  explicit Menu2(menus::MenuModel* model);
  virtual ~Menu2() {}

  // How the menu is aligned relative to the point it is shown at.
  // The alignment is reversed by menu if text direction is right to left.
  enum Alignment {
    ALIGN_TOPLEFT,
    ALIGN_TOPRIGHT
  };

  // Runs the menu at the specified point. This method blocks until done.
  // RunContextMenuAt is the same, but the alignment is the default for a
  // context menu.
  void RunMenuAt(const gfx::Point& point, Alignment alignment);
  void RunContextMenuAt(const gfx::Point& point);

  // Cancels the active menu.
  void CancelMenu();

  // Called when the model supplying data to this menu has changed, and the menu
  // must be rebuilt.
  void Rebuild();

  // Called when the states of the menu items in the menu should be refreshed
  // from the model.
  void UpdateStates();

  // For submenus.
  gfx::NativeMenu GetNativeMenu() const;

  // Get the result of the last call to RunMenuAt to determine whether an
  // item was selected, the user navigated to a next or previous menu, or
  // nothing.
  MenuWrapper::MenuAction GetMenuAction() const;

  // Add a listener to receive a callback when the menu opens.
  void AddMenuListener(MenuListener* listener);

  // Remove a menu listener.
  void RemoveMenuListener(MenuListener* listener);

  // Accessors.
  menus::MenuModel* model() const { return model_; }

 private:
  friend class NativeMenuGtk;

  menus::MenuModel* model_;

  // The object that actually implements the menu.
  scoped_ptr<MenuWrapper> wrapper_;

  DISALLOW_COPY_AND_ASSIGN(Menu2);
};

}  // namespace views

#endif  // VIEWS_CONTROLS_MENU_MENU_2_H_
