// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdlib.h>

#include "chrome/common/url_constants.h"
#include "googleurl/src/url_util.h"

namespace chrome {

const char kAboutScheme[] = "about";
const char kChromeInternalScheme[] = "chrome-internal";
const char kChromeUIScheme[] = "chrome";
const char kDataScheme[] = "data";
const char kExtensionScheme[] = "chrome-extension";
const char kFileScheme[] = "file";
const char kFtpScheme[] = "ftp";
const char kGearsScheme[] = "gears";
const char kHttpScheme[] = "http";
const char kHttpsScheme[] = "https";
const char kJavaScriptScheme[] = "javascript";
const char kMailToScheme[] = "mailto";
const char kMetadataScheme[] = "metadata";
const char kUserScriptScheme[] = "chrome-user-script";
const char kViewSourceScheme[] = "view-source";

#if defined(OS_CHROMEOS)
const char kCrosScheme[] = "cros";
#endif

const char kStandardSchemeSeparator[] = "://";

const char* kSavableSchemes[] = {
  kHttpScheme,
  kHttpsScheme,
  kFileScheme,
  kFtpScheme,
  kExtensionScheme,
  NULL
};

const char kAboutAppCacheInternalsURL[] = "about:appcache-internals";
const char kAboutBlankURL[] = "about:blank";
const char kAboutCacheURL[] = "about:cache";
const char kAboutCrashURL[] = "about:crash";
const char kAboutCreditsURL[] = "about:credits";
const char kAboutHangURL[] = "about:hang";
const char kAboutMemoryURL[] = "about:memory";
const char kAboutNetInternalsURL[] = "about:net-internals";
const char kAboutPluginsURL[] = "about:plugins";
const char kAboutShorthangURL[] = "about:shorthang";
const char kAboutTermsURL[] = "about:terms";

// Use an obfuscated URL to make this nondiscoverable, we only want this
// to be used for testing.
const char kAboutBrowserCrash[] = "about:inducebrowsercrashforrealz";

const char kChromeUIAppLauncherURL[] = "chrome://newtab/#mode=app-launcher";
const char kChromeUIBookmarksURL[] = "chrome://bookmarks/";
const char kChromeUIDevToolsURL[] = "chrome://devtools/";
const char kChromeUIDownloadsURL[] = "chrome://downloads/";
const char kChromeUIExtensionsURL[] = "chrome://extensions/";
const char kChromeUIHistoryURL[] = "chrome://history/";
const char kChromeUIPluginsURL[] = "chrome://plugins/";
const char kChromeUIPrintURL[] = "chrome://print/";
const char kChromeUIFavIconURL[] = "chrome://favicon/";
const char kChromeUIFileBrowseURL[] = "chrome://filebrowse/";
const char kChromeUIMediaplayerURL[] = "chrome://mediaplayer/";
const char kChromeUIOptionsURL[] = "chrome://options/";
const char kChromeUIIPCURL[] = "chrome://about/ipc";
const char kChromeUINetworkURL[] = "chrome://about/network";
const char kChromeUINewTabURL[] = "chrome://newtab";

const char kChromeUIBookmarksHost[] = "bookmarks";
const char kChromeUIDevToolsHost[] = "devtools";
const char kChromeUIDialogHost[] = "dialog";
const char kChromeUIDownloadsHost[] = "downloads";
const char kChromeUIExtensionsHost[] = "extensions";
const char kChromeUIFavIconHost[] = "favicon";
const char kChromeUIHistoryHost[] = "history";
const char kChromeUIPluginsHost[] = "plugins";
const char kChromeUIPrintHost[] = "print";
const char kChromeUIResourcesHost[] = "resources";
const char kChromeUIFileBrowseHost[] = "filebrowse";
const char kChromeUIMediaplayerHost[] = "mediaplayer";
const char kChromeUIOptionsHost[] = "options";
const char kChromeUIInspectorHost[] = "inspector";
const char kChromeUINetInternalsHost[] = "net-internals";
const char kChromeUINewTabHost[] = "newtab";
const char kChromeUIThumbnailPath[] = "thumb";
const char kChromeUIThemePath[] = "theme";

const char kSyncResourcesHost[] = "syncresources";
const char kSyncGaiaLoginPath[] = "gaialogin";
const char kSyncMergeAndSyncPath[] = "mergeandsync";
const char kSyncThrobberPath[] = "throbber.png";
const char kSyncSetupFlowPath[] = "setup";
const char kSyncSetupDonePath[] = "setupdone";

const char kAppCacheViewInternalsURL[] = "chrome://appcache-internals/";

const char kCloudPrintResourcesURL[] = "chrome://cloudprintresources/";
const char kCloudPrintResourcesHost[] = "cloudprintresources";

const char kNetworkViewInternalsURL[] = "chrome://net-internals/";
const char kNetworkViewCacheURL[] = "chrome://view-http-cache/";

void RegisterChromeSchemes() {
  // Don't need "chrome-internal" which was used in old versions of Chrome for
  // the new tab page.
  url_util::AddStandardScheme(kChromeUIScheme);
  url_util::AddStandardScheme(kGearsScheme);
  url_util::AddStandardScheme(kExtensionScheme);
  url_util::AddStandardScheme(kMetadataScheme);
#if defined(OS_CHROMEOS)
  url_util::AddStandardScheme(kCrosScheme);
#endif

  // Prevent future modification of the standard schemes list. This is to
  // prevent accidental creation of data races in the program. AddStandardScheme
  // isn't threadsafe so must be called when GURL isn't used on any other
  // thread. This is really easy to mess up, so we say that all calls to
  // AddStandardScheme in Chrome must be inside this function.
  url_util::LockStandardSchemes();
}

}  // namespace chrome
