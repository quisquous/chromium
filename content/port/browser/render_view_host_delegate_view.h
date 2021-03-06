// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PORT_BROWSER_RENDER_VIEW_HOST_DELEGATE_VIEW_H_
#define CONTENT_PORT_BROWSER_RENDER_VIEW_HOST_DELEGATE_VIEW_H_

#include <vector>

#include "base/basictypes.h"
#include "content/common/content_export.h"
#include "content/public/common/context_menu_source_type.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebDragOperation.h"

class SkBitmap;
struct WebDropData;
struct WebMenuItem;

namespace gfx {
class ImageSkia;
class Point;
class Rect;
}

namespace content {

struct ContextMenuParams;

// This class provides a way for the RenderViewHost to reach out to its
// delegate's view. It only needs to be implemented by embedders if they don't
// use the default WebContentsView implementations.
class CONTENT_EXPORT RenderViewHostDelegateView {
 public:
  // A context menu should be shown, to be built using the context information
  // provided in the supplied params.
  virtual void ShowContextMenu(const ContextMenuParams& params,
                               ContextMenuSourceType type) {}

  // Shows a popup menu with the specified items.
  // This method should call RenderViewHost::DidSelectPopupMenuItem[s]() or
  // RenderViewHost::DidCancelPopupMenu() based on the user action.
  virtual void ShowPopupMenu(const gfx::Rect& bounds,
                             int item_height,
                             double item_font_size,
                             int selected_item,
                             const std::vector<WebMenuItem>& items,
                             bool right_aligned,
                             bool allow_multiple_selection) = 0;

  // The user started dragging content of the specified type within the
  // RenderView. Contextual information about the dragged content is supplied
  // by WebDropData.
  virtual void StartDragging(const WebDropData& drop_data,
                             WebKit::WebDragOperationsMask allowed_ops,
                             const gfx::ImageSkia& image,
                             const gfx::Point& image_offset) {}

  // The page wants to update the mouse cursor during a drag & drop operation.
  // |operation| describes the current operation (none, move, copy, link.)
  virtual void UpdateDragCursor(WebKit::WebDragOperation operation) {}

  // Notification that view for this delegate got the focus.
  virtual void GotFocus() {}

  // Callback to inform the browser that the page is returning the focus to
  // the browser's chrome. If reverse is true, it means the focus was
  // retrieved by doing a Shift-Tab.
  virtual void TakeFocus(bool reverse) {}

#if defined(OS_ANDROID)
  // Called when an unhandled touch event is sent back from the renderer.
  // TODO(joth): This should be pure virtual.
  virtual void ConfirmTouchEvent(bool handled) {}
#endif

 protected:
  virtual ~RenderViewHostDelegateView() {}
};

}  // namespace content

#endif  // CONTENT_PORT_BROWSER_RENDER_VIEW_HOST_DELEGATE_VIEW_H_
