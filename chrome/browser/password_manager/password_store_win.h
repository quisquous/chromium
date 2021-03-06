// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_
#define CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/password_manager/password_store_default.h"

class LoginDatabase;
class Profile;
class WebDataService;

namespace webkit {
namespace forms {
struct PasswordForm;
}
}

// Windows PasswordStore implementation that uses the default implementation,
// but also uses IE7 passwords if no others found.
class PasswordStoreWin : public PasswordStoreDefault {
 public:
  // WebDataService is only used for IE7 password fetching.
  PasswordStoreWin(LoginDatabase* login_database,
                   Profile* profile,
                   WebDataService* web_data_service);

  // RefcountedProfileKeyedService:
  virtual void ShutdownOnUIThread() OVERRIDE;

 private:
  class DBHandler;

  virtual ~PasswordStoreWin();

  // Invoked from Shutdown, but run on the DB thread.
  void ShutdownOnDBThread();

  virtual GetLoginsRequest* NewGetLoginsRequest(
      const GetLoginsCallback& callback) OVERRIDE;

  // See PasswordStoreDefault.
  virtual void ForwardLoginsResult(GetLoginsRequest* request) OVERRIDE;

  // Overridden so that we can save the form for later use.
  virtual void GetLoginsImpl(GetLoginsRequest* request,
                             const webkit::forms::PasswordForm& form) OVERRIDE;

  scoped_ptr<DBHandler> db_handler_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreWin);
};

#endif  // CHROME_BROWSER_PASSWORD_MANAGER_PASSWORD_STORE_WIN_H_
