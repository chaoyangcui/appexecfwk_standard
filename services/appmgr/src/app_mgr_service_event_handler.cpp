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

#include "app_mgr_service_event_handler.h"

#include "app_log_wrapper.h"
#include "app_mgr_service_inner.h"

namespace OHOS {
namespace AppExecFwk {

AMSEventHandler::AMSEventHandler(
    const std::shared_ptr<EventRunner> &runner, const std::weak_ptr<AppMgrServiceInner> &appMgr)
    : EventHandler(runner), appMgr_(appMgr)
{
    APP_LOGI("instance created");
}

AMSEventHandler::~AMSEventHandler()
{
    APP_LOGI("instance destroyed");
}

void AMSEventHandler::ProcessEvent(const InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        APP_LOGE("AppEventHandler::ProcessEvent::parameter error");
        return;
    }

    auto appManager = appMgr_.lock();
    if (!appManager) {
        APP_LOGE("%{public}s, app manager is nullptr", __func__);
        return;
    }
    appManager->HandleTimeOut(event);
    // this->PostTask([&event, &appManager]() { appManager->HandleTimeOut(event); });
}
}  // namespace AppExecFwk
}  // namespace OHOS
