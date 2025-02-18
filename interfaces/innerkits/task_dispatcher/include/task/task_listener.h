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
#ifndef OHOS_APP_DISPATCHER_TASK_TASK_LISTENER_H
#define OHOS_APP_DISPATCHER_TASK_TASK_LISTENER_H

#include "task_stage.h"

namespace OHOS {
namespace AppExecFwk {

/**
 *  Listen to events of task execution.
 */
class TaskListener {
public:
    virtual ~TaskListener(){};
    /**
     *@brief Called when task stage changed.
     *@param stage The stage
     *@return void
     */
    virtual void OnChanged(const TaskStage &stage) = 0;
};

}  // namespace AppExecFwk
}  // namespace OHOS
#endif
