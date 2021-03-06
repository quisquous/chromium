// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "CCActiveAnimation.h"

#include "CCAnimationTestCommon.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include <wtf/Vector.h>

using namespace WebKitTests;
using namespace cc;

namespace {

PassOwnPtr<CCActiveAnimation> createActiveAnimation(int iterations, double duration)
{
    OwnPtr<CCActiveAnimation> toReturn(CCActiveAnimation::create(adoptPtr(new FakeFloatAnimationCurve(duration)), 0, 1, CCActiveAnimation::Opacity));
    toReturn->setIterations(iterations);
    return toReturn.release();
}

PassOwnPtr<CCActiveAnimation> createActiveAnimation(int iterations)
{
    return createActiveAnimation(iterations, 1);
}

TEST(CCActiveAnimationTest, TrimTimeZeroIterations)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(0));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(-1));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(1));
}

TEST(CCActiveAnimationTest, TrimTimeOneIteration)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(-1));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(1));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(2));
}

TEST(CCActiveAnimationTest, TrimTimeInfiniteIterations)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(-1));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(0.5));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(1));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(1.5));
}

TEST(CCActiveAnimationTest, TrimTimeAlternating)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(-1));
    anim->setAlternatesDirection(true);
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(0.5));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(1));
    EXPECT_EQ(0.75, anim->trimTimeToCurrentIteration(1.25));
}

TEST(CCActiveAnimationTest, TrimTimeStartTime)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setStartTime(4);
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(4));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(4.5));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(5));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(6));
}

TEST(CCActiveAnimationTest, TrimTimeTimeOffset)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setTimeOffset(4);
    anim->setStartTime(4);
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(0.5));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(1));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(1));
}

TEST(CCActiveAnimationTest, TrimTimePauseResume)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(0.5));
    anim->setRunState(CCActiveAnimation::Paused, 0.5);
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(1024));
    anim->setRunState(CCActiveAnimation::Running, 1024);
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(1024));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(1024.5));
}

TEST(CCActiveAnimationTest, TrimTimeSuspendResume)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(0.5));
    anim->suspend(0.5);
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(1024));
    anim->resume(1024);
    EXPECT_EQ(0.5, anim->trimTimeToCurrentIteration(1024));
    EXPECT_EQ(1, anim->trimTimeToCurrentIteration(1024.5));
}

TEST(CCActiveAnimationTest, TrimTimeZeroDuration)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(0, 0));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(-1));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(0));
    EXPECT_EQ(0, anim->trimTimeToCurrentIteration(1));
}

TEST(CCActiveAnimationTest, IsFinishedAtZeroIterations)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(0));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_FALSE(anim->isFinishedAt(-1));
    EXPECT_TRUE(anim->isFinishedAt(0));
    EXPECT_TRUE(anim->isFinishedAt(1));
}

TEST(CCActiveAnimationTest, IsFinishedAtOneIteration)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_FALSE(anim->isFinishedAt(-1));
    EXPECT_FALSE(anim->isFinishedAt(0));
    EXPECT_TRUE(anim->isFinishedAt(1));
    EXPECT_TRUE(anim->isFinishedAt(2));
}

TEST(CCActiveAnimationTest, IsFinishedAtInfiniteIterations)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(-1));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_FALSE(anim->isFinishedAt(0));
    EXPECT_FALSE(anim->isFinishedAt(0.5));
    EXPECT_FALSE(anim->isFinishedAt(1));
    EXPECT_FALSE(anim->isFinishedAt(1.5));
}

TEST(CCActiveAnimationTest, IsFinishedAtNotRunning)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(0));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_TRUE(anim->isFinishedAt(0));
    anim->setRunState(CCActiveAnimation::Paused, 0);
    EXPECT_FALSE(anim->isFinishedAt(0));
    anim->setRunState(CCActiveAnimation::WaitingForNextTick, 0);
    EXPECT_FALSE(anim->isFinishedAt(0));
    anim->setRunState(CCActiveAnimation::WaitingForTargetAvailability, 0);
    EXPECT_FALSE(anim->isFinishedAt(0));
    anim->setRunState(CCActiveAnimation::WaitingForStartTime, 0);
    EXPECT_FALSE(anim->isFinishedAt(0));
    anim->setRunState(CCActiveAnimation::Finished, 0);
    EXPECT_TRUE(anim->isFinishedAt(0));
    anim->setRunState(CCActiveAnimation::Aborted, 0);
    EXPECT_TRUE(anim->isFinishedAt(0));
}

TEST(CCActiveAnimationTest, IsFinished)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::Paused, 0);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::WaitingForNextTick, 0);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::WaitingForTargetAvailability, 0);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::WaitingForStartTime, 0);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::Finished, 0);
    EXPECT_TRUE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::Aborted, 0);
    EXPECT_TRUE(anim->isFinished());
}

TEST(CCActiveAnimationTest, IsFinishedNeedsSynchronizedStartTime)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->setRunState(CCActiveAnimation::Running, 2);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::Paused, 2);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::WaitingForNextTick, 2);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::WaitingForTargetAvailability, 2);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::WaitingForStartTime, 2);
    EXPECT_FALSE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::Finished, 0);
    EXPECT_TRUE(anim->isFinished());
    anim->setRunState(CCActiveAnimation::Aborted, 0);
    EXPECT_TRUE(anim->isFinished());
}

TEST(CCActiveAnimationTest, RunStateChangesIgnoredWhileSuspended)
{
    OwnPtr<CCActiveAnimation> anim(createActiveAnimation(1));
    anim->suspend(0);
    EXPECT_EQ(CCActiveAnimation::Paused, anim->runState());
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_EQ(CCActiveAnimation::Paused, anim->runState());
    anim->resume(0);
    anim->setRunState(CCActiveAnimation::Running, 0);
    EXPECT_EQ(CCActiveAnimation::Running, anim->runState());
}

} // namespace
