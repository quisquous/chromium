// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/public/test/test_content_client_initializer.h"

#include "content/browser/notification_service_impl.h"
#include "content/public/common/content_client.h"
#include "content/public/test/mock_render_process_host.h"
#include "content/test/test_content_browser_client.h"
#include "content/test/test_content_client.h"
#include "content/test/test_render_view_host_factory.h"

namespace content {

TestContentClientInitializer::TestContentClientInitializer() {
  notification_service_.reset(new NotificationServiceImpl());

  DCHECK(!GetContentClient());
  content_client_.reset(new TestContentClient);
  SetContentClient(content_client_.get());

  content_browser_client_.reset(new TestContentBrowserClient());
  content_client_->set_browser_for_testing(content_browser_client_.get());
}

TestContentClientInitializer::~TestContentClientInitializer() {
  test_render_view_host_factory_.reset();
  rph_factory_.reset();
  notification_service_.reset();

  DCHECK_EQ(content_client_.get(), GetContentClient());
  SetContentClient(NULL);
  content_client_.reset();

  content_browser_client_.reset();
}

void TestContentClientInitializer::CreateTestRenderViewHosts() {
#if !defined(OS_IOS)
  rph_factory_.reset(new content::MockRenderProcessHostFactory());
  test_render_view_host_factory_.reset(
      new content::TestRenderViewHostFactory(rph_factory_.get()));
#endif  // OS_IOS
}

}  // namespace content
