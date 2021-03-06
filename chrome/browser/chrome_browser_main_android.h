// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROME_BROWSER_MAIN_ANDROID_H_
#define CHROME_BROWSER_CHROME_BROWSER_MAIN_ANDROID_H_

#include "chrome/browser/chrome_browser_main.h"

class ChromeBrowserMainPartsAndroid : public ChromeBrowserMainParts {
 public:
  explicit ChromeBrowserMainPartsAndroid(
      const content::MainFunctionParams& parameters);
  virtual ~ChromeBrowserMainPartsAndroid();

  // content::BrowserMainParts overrides.
  virtual void PreEarlyInitialization() OVERRIDE;

  // ChromeBrowserMainParts overrides.
  virtual void ShowMissingLocaleMessageBox() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(ChromeBrowserMainPartsAndroid);
  scoped_ptr<MessageLoop> main_message_loop_;
};

#endif  // CHROME_BROWSER_CHROME_BROWSER_MAIN_ANDROID_H_
