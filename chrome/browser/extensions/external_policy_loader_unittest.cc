// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>

#include "base/logging.h"
#include "base/message_loop.h"
#include "base/values.h"
#include "base/version.h"
#include "chrome/browser/extensions/external_policy_loader.h"
#include "chrome/browser/extensions/external_provider_impl.h"
#include "chrome/browser/extensions/external_provider_interface.h"
#include "chrome/common/extensions/extension.h"
#include "chrome/common/pref_names.h"
#include "chrome/test/base/testing_pref_service.h"
#include "chrome/test/base/testing_profile.h"
#include "content/public/test/test_browser_thread.h"
#include "testing/gtest/include/gtest/gtest.h"

using content::BrowserThread;

namespace extensions {

class ExternalPolicyProviderTest : public testing::Test {
 public:
  ExternalPolicyProviderTest()
      : loop_(MessageLoop::TYPE_IO),
        ui_thread_(BrowserThread::UI, &loop_) {
  }

  virtual ~ExternalPolicyProviderTest() {}

 private:
  // We need these to satisfy BrowserThread::CurrentlyOn(BrowserThread::UI)
  // checks in ExternalProviderImpl.
  MessageLoop loop_;
  content::TestBrowserThread ui_thread_;
};

class MockExternalPolicyProviderVisitor
    : public ExternalProviderInterface::VisitorInterface {
 public:
  MockExternalPolicyProviderVisitor() {
  }

  // Initialize a provider with |policy_forcelist|, and check that it parses
  // exactly those extensions, that are specified in |policy_validlist|.
  void Visit(ListValue* policy_forcelist,
             ListValue* policy_validlist,
             const std::set<std::string>& ignore_list) {
    profile_.reset(new TestingProfile);
    profile_->GetTestingPrefService()->SetManagedPref(
        prefs::kExtensionInstallForceList,
        policy_forcelist->DeepCopy());
    provider_.reset(new ExternalProviderImpl(
        this,
        new ExternalPolicyLoader(profile_.get()),
        Extension::INVALID,
        Extension::EXTERNAL_POLICY_DOWNLOAD,
        Extension::NO_FLAGS));

    // Extensions will be removed from this list as they visited,
    // so it should be emptied by the end.
    remaining_extensions = policy_validlist;
    provider_->VisitRegisteredExtension();
    EXPECT_EQ(0u, remaining_extensions->GetSize());
  }

  virtual bool OnExternalExtensionFileFound(const std::string& id,
                                            const Version* version,
                                            const FilePath& path,
                                            Extension::Location unused,
                                            int unused2,
                                            bool unused3) {
    ADD_FAILURE() << "There should be no external extensions from files.";
    return false;
  }

  virtual bool OnExternalExtensionUpdateUrlFound(
      const std::string& id, const GURL& update_url,
      Extension::Location location) {
    // Extension has the correct location.
    EXPECT_EQ(Extension::EXTERNAL_POLICY_DOWNLOAD, location);

    // Provider returns the correct location when asked.
    Extension::Location location1;
    scoped_ptr<Version> version1;
    provider_->GetExtensionDetails(id, &location1, &version1);
    EXPECT_EQ(Extension::EXTERNAL_POLICY_DOWNLOAD, location1);
    EXPECT_FALSE(version1.get());

    // Remove the extension from our list.
    StringValue ext_str(id + ";" + update_url.spec());
    EXPECT_NE(false, remaining_extensions->Remove(ext_str, NULL));
    return true;
  }

  virtual void OnExternalProviderReady(
      const ExternalProviderInterface* provider) {
    EXPECT_EQ(provider, provider_.get());
    EXPECT_TRUE(provider->IsReady());
  }

 private:
  ListValue* remaining_extensions;

  scoped_ptr<TestingProfile> profile_;

  scoped_ptr<ExternalProviderImpl> provider_;

  DISALLOW_COPY_AND_ASSIGN(MockExternalPolicyProviderVisitor);
};

TEST_F(ExternalPolicyProviderTest, PolicyIsParsed) {
  ListValue forced_extensions;
  forced_extensions.Append(Value::CreateStringValue(
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa;http://www.example.com/crx?a=5;b=6"));
  forced_extensions.Append(Value::CreateStringValue(
      "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb;"
      "https://clients2.google.com/service/update2/crx"));

  MockExternalPolicyProviderVisitor mv;
  std::set<std::string> empty;
  mv.Visit(&forced_extensions, &forced_extensions, empty);
}

TEST_F(ExternalPolicyProviderTest, InvalidPolicyIsNotParsed) {
  ListValue forced_extensions, valid_extensions;
  StringValue valid(
      "cccccccccccccccccccccccccccccccc;http://www.example.com/crx");
  valid_extensions.Append(valid.DeepCopy());
  forced_extensions.Append(valid.DeepCopy());
  // Add invalid strings:
  forced_extensions.Append(Value::CreateStringValue(""));
  forced_extensions.Append(Value::CreateStringValue(";"));
  forced_extensions.Append(Value::CreateStringValue(";;"));
  forced_extensions.Append(Value::CreateStringValue(
      ";http://www.example.com/crx"));
  forced_extensions.Append(Value::CreateStringValue(
      ";aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
  forced_extensions.Append(Value::CreateStringValue(
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa;"));
  forced_extensions.Append(Value::CreateStringValue(
      "http://www.example.com/crx;aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
  forced_extensions.Append(Value::CreateStringValue(
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa;http#//www.example.com/crx"));
  forced_extensions.Append(Value::CreateStringValue(
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
  forced_extensions.Append(Value::CreateStringValue(
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaahttp#//www.example.com/crx"));

  MockExternalPolicyProviderVisitor mv;
  std::set<std::string> empty;
  mv.Visit(&forced_extensions, &valid_extensions, empty);
}

}  // namespace extensions
