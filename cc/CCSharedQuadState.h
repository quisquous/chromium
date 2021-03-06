// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CCSharedQuadState_h
#define CCSharedQuadState_h

#include "IntRect.h"
#include "base/memory/scoped_ptr.h"
#include <public/WebTransformationMatrix.h>

namespace cc {

struct CCSharedQuadState {
    int id;

    // Transforms from quad's original content space to its target content space.
    WebKit::WebTransformationMatrix quadTransform;
    // This rect lives in the content space for the quad's originating layer.
    IntRect visibleContentRect;
    IntRect clippedRectInTarget;
    float opacity;
    bool opaque;

    static scoped_ptr<CCSharedQuadState> create(const WebKit::WebTransformationMatrix& quadTransform, const IntRect& visibleContentRect, const IntRect& clippedRectInTarget, float opacity, bool opaque);
    CCSharedQuadState(const WebKit::WebTransformationMatrix& quadTransform, const IntRect& visibleContentRect, const IntRect& clippedRectInTarget, float opacity, bool opaque);

    scoped_ptr<CCSharedQuadState> copy() const;
};

}

#endif
