// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_BASE_TOUCH_TOUCH_FACTORY_H_
#define UI_BASE_TOUCH_TOUCH_FACTORY_H_

#include <bitset>
#include <map>
#include <vector>

#include "base/hash_tables.h"
#include "base/timer.h"
#include "ui/base/ui_export.h"

template <typename T> struct DefaultSingletonTraits;

typedef unsigned long Cursor;
typedef unsigned long Window;
typedef struct _XDisplay Display;
typedef union _XEvent XEvent;

namespace ui {

// Functions related to determining touch devices.
class UI_EXPORT TouchFactory {
 private:
  TouchFactory();
  ~TouchFactory();

 public:
  // Returns the TouchFactory singleton.
  static TouchFactory* GetInstance();

  // Sets the touch devices from the command line.
  static void SetTouchDeviceListFromCommandLine();

  // Updates the list of devices.
  void UpdateDeviceList(Display* display);

  // Checks whether an XI2 event should be processed or not (i.e. if the event
  // originated from a device we are interested in).
  bool ShouldProcessXI2Event(XEvent* xevent);

  // Setup an X Window for XInput2 events.
  void SetupXI2ForXWindow(::Window xid);

  // Keeps a list of touch devices so that it is possible to determine if a
  // pointer event is a touch-event or a mouse-event. The list is reset each
  // time this is called.
  void SetTouchDeviceList(const std::vector<unsigned int>& devices);

  // Is the device a touch-device?
  bool IsTouchDevice(unsigned int deviceid) const;

  // Is the device a real multi-touch-device? (see doc. for |touch_device_list_|
  // below for more explanation.)
  bool IsMultiTouchDevice(unsigned int deviceid) const;

#if defined(USE_XI2_MT)
  // Tries to find an existing slot ID mapping to tracking ID. If there
  // isn't one already, allocates a new slot ID and sets up the mapping.
  int GetSlotForTrackingID(uint32 tracking_id);

  // Releases the slot ID mapping to tracking ID.
  void ReleaseSlotForTrackingID(uint32 tracking_id);
#endif

  // Is the slot ID currently used?
  bool IsSlotUsed(int slot) const;

  // Marks a slot as being used/unused.
  void SetSlotUsed(int slot, bool used);

  // Grabs the touch devices for the specified window on the specified display.
  // Returns if grab was successful for all touch devices.
  bool GrabTouchDevices(Display* display, ::Window window);

  // Ungrabs the touch devices. Returns if ungrab was successful for all touch
  // devices.
  bool UngrabTouchDevices(Display* display);

  // Updates the root window to show (or hide) the cursor. Also indicate whether
  // the timer should be started to automatically hide the cursor after a
  // certain duration of inactivity (i.e. it is ignored if |show| is false).
  void SetCursorVisible(bool show, bool start_timer);

  // Whether the cursor is currently visible or not.
  bool is_cursor_visible() const {
    return is_cursor_visible_;
  }

  // Whether any touch device is currently present and enabled.
  bool IsTouchDevicePresent();

 private:
  void HideCursorForInactivity() {
    SetCursorVisible(false, false);
  }

  // Requirement for Singleton
  friend struct DefaultSingletonTraits<TouchFactory>;

  // The default cursor is hidden after startup, and when the mouse pointer is
  // idle for a while. Once there is some event from a mouse device, the cursor
  // is immediately displayed.
  bool is_cursor_visible_;

  // Touch events are currently disabled by default. It can be turned on using
  // |kEnableTouchEvents| switch.
  bool touch_events_allowed_;

  // The cursor is hidden if it is idle for a certain amount time. This timer
  // is used to keep track of the idleness.
  base::OneShotTimer<TouchFactory> cursor_timer_;

  // The default cursor.
  Cursor arrow_cursor_;

  // The invisible cursor.
  Cursor invisible_cursor_;

  // NOTE: To keep track of touch devices, we currently maintain a lookup table
  // to quickly decide if a device is a touch device or not. We also maintain a
  // list of the touch devices. Ideally, there will be only one touch device,
  // and instead of having the lookup table and the list, there will be a single
  // identifier for the touch device. This can be completed after enough testing
  // on real touch devices.

  static const int kMaxDeviceNum = 128;

  // A quick lookup table for determining if events from the pointer device
  // should be processed.
  std::bitset<kMaxDeviceNum> pointer_device_lookup_;

  // A quick lookup table for determining if a device is a touch device.
  std::bitset<kMaxDeviceNum> touch_device_lookup_;

  // Indicates whether a touch device is currently available or not.
  bool touch_device_available_;

  // Whether IsTouchDevicePresent() has been called yet.
  bool touch_present_called_;

  // The list of touch devices. For testing/debugging purposes, a single-pointer
  // device (mouse or touch screen without sufficient X/driver support for MT)
  // can sometimes be treated as a touch device. The key in the map represents
  // the device id, and the value represents if the device is multi-touch
  // capable.
  std::map<int, bool> touch_device_list_;

  // Maximum simultaneous touch points.
  static const int kMaxTouchPoints = 32;

#if defined(USE_XI2_MT)
  // Stores the minimum available slot ID which helps get slot ID from
  // tracking ID. When it equals to kMaxTouchPoints, there is no available
  // slot.
  int min_available_slot_;

  // A hash table to map tracking ID to slot.
  typedef base::hash_map<uint32, int> TrackingIdMap;
  TrackingIdMap tracking_id_map_;
#endif

  // A lookup table for slots in use for a touch event.
  std::bitset<kMaxTouchPoints> slots_used_;

  DISALLOW_COPY_AND_ASSIGN(TouchFactory);
};

}  // namespace ui

#endif  // UI_BASE_TOUCH_TOUCH_FACTORY_H_
