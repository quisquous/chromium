// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_TAB_CONTENTS_NAVIGATION_METRICS_RECORDER_H_
#define CHROME_BROWSER_TAB_CONTENTS_NAVIGATION_METRICS_RECORDER_H_

#include "chrome/browser/common/web_contents_user_data.h"
#include "content/public/browser/web_contents_observer.h"

class NavigationMetricsRecorder
    : public content::WebContentsObserver,
      public WebContentsUserData<NavigationMetricsRecorder> {
 public:
  virtual ~NavigationMetricsRecorder();

 private:
  explicit NavigationMetricsRecorder(content::WebContents* web_contents);
  friend class WebContentsUserData<NavigationMetricsRecorder>;

  // content::WebContentsObserver overrides:
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(NavigationMetricsRecorder);
};

#endif  // CHROME_BROWSER_TAB_CONTENTS_NAVIGATION_METRICS_RECORDER_H_
