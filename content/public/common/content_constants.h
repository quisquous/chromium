// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// A handful of resource-like constants related to the Content application.

#ifndef CONTENT_PUBLIC_COMMON_CONTENT_CONSTANTS_H_
#define CONTENT_PUBLIC_COMMON_CONTENT_CONSTANTS_H_

#include <stddef.h>         // For size_t

#include "base/file_path.h"
#include "content/common/content_export.h"

namespace content {

// The name of the directory under BrowserContext::GetPath where the AppCache is
// put.
CONTENT_EXPORT extern const FilePath::CharType kAppCacheDirname[];
// The name of the directory under BrowserContext::GetPath where Pepper plugin
// data is put.
CONTENT_EXPORT extern const FilePath::CharType kPepperDataDirname[];

// The MIME type used for the browser plugin.
CONTENT_EXPORT extern const char kBrowserPluginMimeType[];
// TODO(fsamuel): Remove this once upstreaming of the new browser plugin is
// complete.
CONTENT_EXPORT extern const char kBrowserPluginNewMimeType[];

CONTENT_EXPORT extern const size_t kMaxRendererProcessCount;

// The maximum number of session history entries per tab.
extern const int kMaxSessionHistoryEntries;

// The maximum number of characters of the document's title that we're willing
// to accept in the browser process.
extern const size_t kMaxTitleChars;

// The maximum number of characters in the URL that we're willing to accept
// in the browser process. It is set low enough to avoid damage to the browser
// but high enough that a web site can abuse location.hash for a little storage.
// We have different values for "max accepted" and "max displayed" because
// a data: URI may be legitimately massive, but the full URI would kill all
// known operating systems if you dropped it into a UI control.
CONTENT_EXPORT extern const size_t kMaxURLChars;
CONTENT_EXPORT extern const size_t kMaxURLDisplayChars;

extern const char kStatsFilename[];
extern const int kStatsMaxThreads;
extern const int kStatsMaxCounters;

// Most sequence numbers are used by a renderer when responding to a browser
// request for histogram data.  This reserved number is used when a renderer
// sends an unprovoked update, such as after a page has been loaded.  Using
// this reserved constant avoids any chance of confusion with a response having
// a browser-supplied sequence number.
CONTENT_EXPORT extern const int kHistogramSynchronizerReservedSequenceNumber;

CONTENT_EXPORT extern const char kGpuCompositingFieldTrialName[];
CONTENT_EXPORT extern const char
    kGpuCompositingFieldTrialForceCompositingEnabledName[];
CONTENT_EXPORT extern const char kGpuCompositingFieldTrialThreadEnabledName[];

CONTENT_EXPORT extern const char kStage3DFieldTrialName[];
CONTENT_EXPORT extern const char kStage3DFieldTrialBlacklistedName[];
CONTENT_EXPORT extern const char kStage3DFieldTrialEnabledName[];

}  // namespace content

#endif  // CONTENT_PUBLIC_COMMON_CONTENT_CONSTANTS_H_
