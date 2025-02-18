/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_INSTALL_PARAM_H
#define FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_INSTALL_PARAM_H

#include <string>

#include "parcel.h"

namespace OHOS {
namespace AppExecFwk {

enum class InstallFlag {
    NORMAL = 0,
    // Allow to replace the existing bundle when the new version isn't lower than the old one.
    // If the bundle does not exist, just like normal flag.
    REPLACE_EXISTING = 1,
};

enum class InstallLocation {
    INTERNAL_ONLY = 1,
    PREFER_EXTERNAL = 2,
};

// provides parameters required for installing or uninstalling an application
struct InstallParam : public Parcelable {
    InstallFlag installFlag = InstallFlag::NORMAL;
    InstallLocation installLocation = InstallLocation::INTERNAL_ONLY;
    int userId = 0;
    // is keep user data while uninstall.
    bool isKeepData = false;

    // the parcel object function is not const.
    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    static InstallParam *Unmarshalling(Parcel &parcel);
};

}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_INSTALL_PARAM_H
