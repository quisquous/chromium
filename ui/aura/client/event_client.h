// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_AURA_CLIENT_EVENT_CLIENT_H_
#define UI_AURA_CLIENT_EVENT_CLIENT_H_

#include "ui/aura/aura_export.h"
#include "ui/aura/window.h"

namespace aura {

class Event;
class RootWindow;

namespace client {

// An interface implemented by an object that alters event processing.
class AURA_EXPORT EventClient {
 public:
  // Returns true if events can be processed by |window| or any of its children.
  virtual bool CanProcessEventsWithinSubtree(const Window* window) const = 0;

 protected:
  virtual ~EventClient() {}
};

// Sets/Gets the event client on the RootWindow.
AURA_EXPORT void SetEventClient(RootWindow* root_window, EventClient* client);
AURA_EXPORT EventClient* GetEventClient(const RootWindow* root_window);

}  // namespace clients
}  // namespace aura

#endif  // UI_AURA_CLIENT_EVENT_CLIENT_H_
