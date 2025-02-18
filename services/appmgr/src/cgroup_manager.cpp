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
#include "cgroup_manager.h"
#include <cinttypes>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <unistd.h>
#include "app_log_wrapper.h"
#include "event_handler.h"
#include "securec.h"

#define CG_CPUSET_DIR "/dev/cpuset"
#define CG_CPUSET_DEFAULT_DIR CG_CPUSET_DIR
#define CG_CPUSET_DEFAULT_TASKS_PATH CG_CPUSET_DEFAULT_DIR "/tasks"
#define CG_CPUSET_BACKGROUND_DIR CG_CPUSET_DIR "/background"
#define CG_CPUSET_BACKGROUND_TASKS_PATH CG_CPUSET_BACKGROUND_DIR "/tasks"

#define CG_CPUCTL_DIR "/dev/cpuctl"
#define CG_CPUCTL_DEFAULT_DIR CG_CPUCTL_DIR
#define CG_CPUCTL_DEFAULT_TASKS_PATH CG_CPUCTL_DEFAULT_DIR "/tasks"
#define CG_CPUCTL_BACKGROUND_DIR CG_CPUCTL_DIR "/background"
#define CG_CPUCTL_BACKGROUND_TASKS_PATH CG_CPUCTL_BACKGROUND_DIR "/tasks"

#define CG_FREEZER_DIR "/dev/freezer"
#define CG_FREEZER_FROZEN_DIR CG_FREEZER_DIR "/frozen"
#define CG_FREEZER_FROZEN_TASKS_PATH CG_FREEZER_FROZEN_DIR "/tasks"
#define CG_FREEZER_THAWED_DIR CG_FREEZER_DIR "/thawed"
#define CG_FREEZER_THAWED_TASKS_PATH CG_FREEZER_THAWED_DIR "/tasks"

#define CG_MEM_DIR "/dev/memcg"
#define CG_MEM_OOMCTL_PATH CG_MEM_DIR "/memory.oom_control"
#define CG_MEM_EVTCTL_PATH CG_MEM_DIR "/cgroup.event_control"
#define CG_MEM_PRESSURE_LEVEL_PATH CG_MEM_DIR "/memory.pressure_level"

namespace OHOS {
namespace AppExecFwk {

namespace {

class ScopeGuard final {
public:
    using Function = std::function<void()>;

public:
    explicit ScopeGuard(Function fn) : fn_(fn), dismissed_(false)
    {}

    ~ScopeGuard()
    {
        if (!dismissed_) {
            fn_();
        }
    }

public:
    void Dismiss()
    {
        dismissed_ = true;
    }

private:
    Function fn_;
    bool dismissed_ = false;
};

int WriteValue(int fd, std::string_view v)
{
    if (fd < 0) {
        errno = EINVAL;
        return -1;
    }

    int ret = TEMP_FAILURE_RETRY(write(fd, v.data(), v.size()));
    if (ret != 0) {
        APP_LOGE("%{public}s(%{public}d) err: %{public}s.", __func__, __LINE__, strerror(errno));
    }
    fsync(fd);

    return ret;
}

int WriteValue(int fd, int v, bool newLine = true)
{
    if (fd < 0) {
        errno = EINVAL;
        return -1;
    }

    char str[32] = {0};

    int len = snprintf_s(str, sizeof(str), sizeof(str) - 1, newLine ? "%d\n" : "%d", v);
    if (len < 1) {
        return -1;
    }

    return WriteValue(fd, str);
}

}  // namespace

CgroupManager::CgroupManager() : memoryEventControlFd_(-1)
{
    for (int i = 0; i < SCHED_POLICY_MAX; ++i) {
        cpusetTasksFds_[i] = -1;
    }

    for (int i = 0; i < LOW_MEMORY_LEVEL_MAX; ++i) {
        memoryEventFds_[i] = -1;
        memoryPressureFds_[i] = -1;
    }
}

CgroupManager::~CgroupManager()
{
    for (int i = 0; i < LOW_MEMORY_LEVEL_MAX; ++i) {
        if (memoryEventFds_[i] >= 0) {
            if (eventHandler_) {
                eventHandler_->RemoveFileDescriptorListener(memoryEventFds_[i]);
            }
            close(memoryEventFds_[i]);
        }
        if (memoryPressureFds_[i] >= 0) {
            close(memoryPressureFds_[i]);
        }
    }

    if (memoryEventControlFd_ >= 0) {
        close(memoryEventControlFd_);
    }

    for (int i = 0; i < SCHED_POLICY_MAX; ++i) {
        if (cpusetTasksFds_[i] >= 0) {
            close(cpusetTasksFds_[i]);
        }
    }
}

bool CgroupManager::Init()
{
    APP_LOGE("%{public}s(%{public}d) Init enter.", __func__, __LINE__);
    if (IsInited()) {
        APP_LOGE("%{public}s(%{public}d) already inited.", __func__, __LINE__);
        return false;
    }

    auto eventHandler = std::make_shared<EventHandler>(EventRunner::Create());
    if (!eventHandler) {
        APP_LOGE("%{public}s(%{public}d) failed to get event handler.", __func__, __LINE__);
        return false;
    }

    UniqueFd cpusetTasksFds[SCHED_POLICY_MAX];
    if (!InitCpusetTasksFds(cpusetTasksFds)) {
        return false;
    }

    UniqueFd cpuctlTasksFds[SCHED_POLICY_MAX];
    if (!InitCpuctlTasksFds(cpuctlTasksFds)) {
        return false;
    }

    UniqueFd freezerTasksFds[SCHED_POLICY_FREEZER_MAX];
    if (!InitFreezerTasksFds(freezerTasksFds)) {
        return false;
    }

    UniqueFd memoryEventControlFd;
    if (!InitMemoryEventControlFd(memoryEventControlFd)) {
        return false;
    }

    UniqueFd memoryEventFds[LOW_MEMORY_LEVEL_MAX];
    if (!InitMemoryEventFds(memoryEventFds)) {
        return false;
    }

    UniqueFd memoryPressureFds[LOW_MEMORY_LEVEL_MAX];
    if (!InitMemoryPressureFds(memoryPressureFds)) {
        return false;
    }

    if (!RegisterLowMemoryMonitor(
            memoryEventFds_, memoryPressureFds_, memoryEventControlFd_, LOW_MEMORY_LEVEL_LOW, eventHandler)) {
        return false;
    }

    ScopeGuard lowLevelListenerGuard(
        [&]() { eventHandler->RemoveFileDescriptorListener(memoryEventFds_[LOW_MEMORY_LEVEL_LOW]); });

    if (!RegisterLowMemoryMonitor(
            memoryEventFds_, memoryPressureFds_, memoryEventControlFd_, LOW_MEMORY_LEVEL_MEDIUM, eventHandler)) {
        return false;
    }

    ScopeGuard mediumLevelListenerGuard(
        [&]() { eventHandler->RemoveFileDescriptorListener(memoryEventFds_[LOW_MEMORY_LEVEL_MEDIUM]); });

    if (!RegisterLowMemoryMonitor(
            memoryEventFds_, memoryPressureFds_, memoryEventControlFd_, LOW_MEMORY_LEVEL_CRITICAL, eventHandler)) {
        return false;
    }

    ScopeGuard criticalLevelListenerGuard(
        [&]() { eventHandler->RemoveFileDescriptorListener(memoryEventFds_[LOW_MEMORY_LEVEL_CRITICAL]); });

    eventHandler_ = eventHandler;

    lowLevelListenerGuard.Dismiss();
    mediumLevelListenerGuard.Dismiss();
    criticalLevelListenerGuard.Dismiss();

    return true;
}

bool CgroupManager::IsInited() const
{
    return bool(eventHandler_);
}

bool CgroupManager::SetThreadSchedPolicy(int tid, SchedPolicy schedPolicy)
{
    if (!IsInited()) {
        APP_LOGE("%{public}s(%{public}d) not inited.", __func__, __LINE__);
        return false;
    }

    if (tid < 1) {
        APP_LOGE("%{public}s(%{public}d) invalid tid %{public}d.", __func__, __LINE__, tid);
        return false;
    }

    if (schedPolicy < 0 || schedPolicy >= SchedPolicy::SCHED_POLICY_MAX) {
        APP_LOGE("%{public}s(%{public}d) invalid sched policy %{public}d.", __func__, __LINE__, schedPolicy);
        return false;
    }

    if (schedPolicy == SchedPolicy::SCHED_POLICY_FREEZED) {
        // set frozen of freezer
        if (!SetFreezerSubsystem(tid, SchedPolicyFreezer::SCHED_POLICY_FREEZER_FROZEN)) {
            APP_LOGE("%{public}s(%{public}d) set freezer subsystem failed sched policy %{public}d.",
                __func__,
                __LINE__,
                schedPolicy);
            return false;
        }
    } else {
        // set cpuset
        if (!SetCpusetSubsystem(tid, schedPolicy)) {
            APP_LOGE("%{public}s(%{public}d) set cpuset subsystem failed sched policy %{public}d.",
                __func__,
                __LINE__,
                schedPolicy);
            return false;
        }
        // set cpuctl
        if (!SetCpuctlSubsystem(tid, schedPolicy)) {
            APP_LOGE("%{public}s(%{public}d) set cpuctl subsystem failed sched policy %{public}d.",
                __func__,
                __LINE__,
                schedPolicy);
            return false;
        }

        // set thawed of freezer
        if (!SetFreezerSubsystem(tid, SchedPolicyFreezer::SCHED_POLICY_FREEZER_THAWED)) {
            APP_LOGE("%{public}s(%{public}d) set freezer subsystem failed sched policy %{public}d.",
                __func__,
                __LINE__,
                schedPolicy);
            return false;
        }
    }

    return true;
}

bool CgroupManager::SetProcessSchedPolicy(int pid, SchedPolicy schedPolicy)
{
    if (!IsInited()) {
        APP_LOGE("%{public}s(%{public}d) not inited.", __func__, __LINE__);
        return false;
    }

    if (pid < 1) {
        APP_LOGE("%{public}s(%{public}d) invalid pid %{public}d", __func__, __LINE__, pid);
        return false;
    }

    if (schedPolicy < 0 && schedPolicy >= SCHED_POLICY_MAX) {
        APP_LOGE("%{public}s(%{public}d) invalid sched policy %{public}d", __func__, __LINE__, schedPolicy);
        return false;
    }

    // Set all threads's sched policy inside this process.
    char taskDir[64];
    if (snprintf_s(taskDir, sizeof(taskDir), sizeof(taskDir) - 1, "/proc/%d/task", pid) < 0) {
        return false;
    }

    DIR *dir = opendir(taskDir);
    if (dir == nullptr) {
        APP_LOGE("%{public}s(%{public}d) failed to opendir invalid pid %{public}d taskDir %{public}s , %{public}s",
            __func__,
            __LINE__,
            pid,
            taskDir,
            strerror(errno));
        return false;
    }

    struct dirent *dent;
    while ((dent = readdir(dir))) {
        // Filter out '.' & '..'
        if (dent->d_name[0] != '.') {
            SetThreadSchedPolicy(atoi(dent->d_name), schedPolicy);
        }
    }

    closedir(dir);

    return true;
}

void CgroupManager::OnReadable(int32_t fd)
{
    APP_LOGW("%{public}s(%{public}d) system low memory alert.", __func__, __LINE__);

    if (!LowMemoryAlert) {
        APP_LOGE("%{public}s(%{public}d) 'LowMemoryAlert' not available.", __func__, __LINE__);
        return;
    }

    auto TryToRaiseLowMemoryAlert = [=](LowMemoryLevel level) {
        if (fd == memoryEventFds_[level]) {
            APP_LOGW("%{public}s(%{public}d) checking level %{public}d", __func__, __LINE__, level);
            uint64_t count = 0;
            int ret = TEMP_FAILURE_RETRY(read(fd, &count, sizeof(uint64_t)));
            if (ret <= 0) {
                APP_LOGW("%{public}s(%{public}d) failed to read eventfd %{public}d.", __func__, __LINE__, errno);
                return false;
            }
            if (count < 1) {
                APP_LOGW("%{public}s(%{public}d) invalid eventfd count %{public}" PRIu64 ".", __func__, __LINE__, count);
                return false;
            }
            APP_LOGW(
                "%{public}s(%{public}d) raising low memory alert for level %{public}d...", __func__, __LINE__, level);
            LowMemoryAlert(level);
            return true;
        }
        return false;
    };

    if (TryToRaiseLowMemoryAlert(LOW_MEMORY_LEVEL_LOW)) {
        return;
    }

    if (TryToRaiseLowMemoryAlert(LOW_MEMORY_LEVEL_MEDIUM)) {
        return;
    }

    if (TryToRaiseLowMemoryAlert(LOW_MEMORY_LEVEL_CRITICAL)) {
        return;
    }

    // Should not reach here!
    APP_LOGE("%{public}s(%{public}d) Unknown fd %{public}d.", __func__, __LINE__, fd);
}

bool CgroupManager::RegisterLowMemoryMonitor(const int memoryEventFds[LOW_MEMORY_LEVEL_MAX],
    const int memoryPressureFds[LOW_MEMORY_LEVEL_MAX], const int memoryEventControlFd, const LowMemoryLevel level,
    const std::shared_ptr<EventHandler> &eventHandler)
{
    APP_LOGI("RegisterLowMemoryMonitor(%{public}d) registering low memory monitor %{public}d...", __LINE__, level);

    char buf[64] = {0};
    static const char *levelName[] = {"low", "medium", "critical"};

    if (snprintf_s(buf,
            sizeof(buf),
            sizeof(buf) - 1,
            "%d %d %s",
            memoryEventFds[level],
            memoryPressureFds[level],
            levelName[level]) < 0) {
        return false;
    }

    int ret = TEMP_FAILURE_RETRY(write(memoryEventControlFd, buf, strlen(buf) + 1));
    if (ret < 0) {
        APP_LOGI("RegisterLowMemoryMonitor(%{public}d) failed to write memory control %{public}d...", __LINE__, errno);
        return false;
    }

    eventHandler->AddFileDescriptorListener(memoryEventFds[level], FILE_DESCRIPTOR_INPUT_EVENT, shared_from_this());

    return true;
}

bool CgroupManager::InitCpusetTasksFds(UniqueFd cpusetTasksFds[SCHED_POLICY_CPU_MAX])
{
    cpusetTasksFds[SCHED_POLICY_CPU_DEFAULT] = UniqueFd(open(CG_CPUSET_DEFAULT_TASKS_PATH, O_RDWR));
    cpusetTasksFds[SCHED_POLICY_CPU_BACKGROUND] = UniqueFd(open(CG_CPUSET_BACKGROUND_TASKS_PATH, O_RDWR));
    if (cpusetTasksFds[SCHED_POLICY_CPU_DEFAULT].Get() < 0 || cpusetTasksFds[SCHED_POLICY_CPU_BACKGROUND].Get() < 0) {
        APP_LOGE("%{public}s(%{public}d) cannot open cpuset cgroups %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    cpusetTasksFds_[SCHED_POLICY_CPU_DEFAULT] = cpusetTasksFds[SCHED_POLICY_CPU_DEFAULT].Release();
    cpusetTasksFds_[SCHED_POLICY_CPU_BACKGROUND] = cpusetTasksFds[SCHED_POLICY_CPU_BACKGROUND].Release();
    return true;
}

bool CgroupManager::InitCpuctlTasksFds(UniqueFd cpuctlTasksFds[SCHED_POLICY_CPU_MAX])
{
    cpuctlTasksFds[SCHED_POLICY_CPU_DEFAULT] = UniqueFd(open(CG_CPUCTL_DEFAULT_TASKS_PATH, O_RDWR));
    cpuctlTasksFds[SCHED_POLICY_CPU_BACKGROUND] = UniqueFd(open(CG_CPUCTL_BACKGROUND_TASKS_PATH, O_RDWR));
    if (cpuctlTasksFds[SCHED_POLICY_CPU_DEFAULT].Get() < 0 || cpuctlTasksFds[SCHED_POLICY_CPU_BACKGROUND].Get() < 0) {
        APP_LOGE("%{public}s(%{public}d) cannot open cpuctl cgroups %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    cpuctlTasksFds_[SCHED_POLICY_CPU_DEFAULT] = cpuctlTasksFds[SCHED_POLICY_CPU_DEFAULT].Release();
    cpuctlTasksFds_[SCHED_POLICY_CPU_BACKGROUND] = cpuctlTasksFds[SCHED_POLICY_CPU_BACKGROUND].Release();
    return true;
}

bool CgroupManager::InitFreezerTasksFds(UniqueFd freezerTasksFds[SCHED_POLICY_FREEZER_MAX])
{
    freezerTasksFds[SCHED_POLICY_FREEZER_FROZEN] = UniqueFd(open(CG_FREEZER_FROZEN_TASKS_PATH, O_RDWR));
    freezerTasksFds[SCHED_POLICY_FREEZER_THAWED] = UniqueFd(open(CG_FREEZER_THAWED_TASKS_PATH, O_RDWR));
    if (freezerTasksFds[SCHED_POLICY_FREEZER_FROZEN].Get() < 0 ||
        freezerTasksFds[SCHED_POLICY_FREEZER_THAWED].Get() < 0) {
        APP_LOGE("%{public}s(%{public}d) cannot open freezer cgroups %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    freezerTasksFds_[SCHED_POLICY_FREEZER_FROZEN] = freezerTasksFds[SCHED_POLICY_FREEZER_FROZEN].Release();
    freezerTasksFds_[SCHED_POLICY_FREEZER_THAWED] = freezerTasksFds[SCHED_POLICY_FREEZER_THAWED].Release();
    return true;
}

bool CgroupManager::InitMemoryEventControlFd(UniqueFd &memoryEventControlFd)
{
    memoryEventControlFd = UniqueFd(open(CG_MEM_EVTCTL_PATH, O_WRONLY));
    if (memoryEventControlFd.Get() < 0) {
        APP_LOGE(
            "%{pubid}s(%{publid}d) failed to open memory event control node %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    memoryEventControlFd_ = memoryEventControlFd.Release();
    return true;
}

bool CgroupManager::InitMemoryEventFds(UniqueFd memoryEventFds[LOW_MEMORY_LEVEL_MAX])
{
    memoryEventFds[LOW_MEMORY_LEVEL_LOW] = UniqueFd(eventfd(0, EFD_NONBLOCK));
    memoryEventFds[LOW_MEMORY_LEVEL_MEDIUM] = UniqueFd(eventfd(0, EFD_NONBLOCK));
    memoryEventFds[LOW_MEMORY_LEVEL_CRITICAL] = UniqueFd(eventfd(0, EFD_NONBLOCK));
    if (memoryEventFds[LOW_MEMORY_LEVEL_LOW].Get() < 0 || memoryEventFds[LOW_MEMORY_LEVEL_MEDIUM].Get() < 0 ||
        memoryEventFds[LOW_MEMORY_LEVEL_CRITICAL].Get() < 0) {
        APP_LOGE("%{public}s(${public}d) failed to create memory eventfd %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    memoryEventFds_[LOW_MEMORY_LEVEL_LOW] = memoryEventFds[LOW_MEMORY_LEVEL_LOW].Release();
    memoryEventFds_[LOW_MEMORY_LEVEL_MEDIUM] = memoryEventFds[LOW_MEMORY_LEVEL_MEDIUM].Release();
    memoryEventFds_[LOW_MEMORY_LEVEL_CRITICAL] = memoryEventFds[LOW_MEMORY_LEVEL_CRITICAL].Release();
    return true;
}

bool CgroupManager::InitMemoryPressureFds(UniqueFd memoryPressureFds[LOW_MEMORY_LEVEL_MAX])
{
    memoryPressureFds[LOW_MEMORY_LEVEL_LOW] = UniqueFd(open(CG_MEM_PRESSURE_LEVEL_PATH, O_RDONLY));
    memoryPressureFds[LOW_MEMORY_LEVEL_MEDIUM] = UniqueFd(open(CG_MEM_PRESSURE_LEVEL_PATH, O_RDONLY));
    memoryPressureFds[LOW_MEMORY_LEVEL_CRITICAL] = UniqueFd(open(CG_MEM_PRESSURE_LEVEL_PATH, O_RDONLY));
    if (memoryPressureFds[LOW_MEMORY_LEVEL_LOW].Get() < 0 || memoryPressureFds[LOW_MEMORY_LEVEL_MEDIUM].Get() < 0 ||
        memoryPressureFds[LOW_MEMORY_LEVEL_CRITICAL].Get() < 0) {
        APP_LOGE("%{public}s(${public}d) failed to open memory pressure fd %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    memoryPressureFds_[LOW_MEMORY_LEVEL_LOW] = memoryPressureFds[LOW_MEMORY_LEVEL_LOW].Release();
    memoryPressureFds_[LOW_MEMORY_LEVEL_MEDIUM] = memoryPressureFds[LOW_MEMORY_LEVEL_MEDIUM].Release();
    memoryPressureFds_[LOW_MEMORY_LEVEL_CRITICAL] = memoryPressureFds[LOW_MEMORY_LEVEL_CRITICAL].Release();
    return true;
}

bool CgroupManager::SetCpusetSubsystem(const int tid, const SchedPolicy schedPolicy)
{
    int fd = cpusetTasksFds_[schedPolicy];
    if (fd < 0) {
        APP_LOGE("%{public}s(%{public}d) invalid cpuset fd for policy %{public}d.", __func__, __LINE__, schedPolicy);
        return false;
    }

    int ret = WriteValue(fd, tid);
    if (ret < 0) {
        APP_LOGE("%{public}s(%{public}d) write cpuset tid failed %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    return true;
}

bool CgroupManager::SetCpuctlSubsystem(const int tid, const SchedPolicy schedPolicy)
{
    int fd = cpuctlTasksFds_[schedPolicy];
    if (fd < 0) {
        APP_LOGE("%{public}s(%{public}d) invalid cpuctl fd for policy %{public}d.", __func__, __LINE__, schedPolicy);
        return false;
    }

    int ret = WriteValue(fd, tid);
    if (ret < 0) {
        APP_LOGE("%{public}s(%{public}d) write cpuctl tid failed %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    return true;
}

bool CgroupManager::SetFreezerSubsystem(const int tid, const SchedPolicyFreezer state)
{
    int fd = freezerTasksFds_[state];
    if (fd < 0) {
        APP_LOGE("%{public}s(%{public}d) invalid freezer fd for state %{public}d.", __func__, __LINE__, state);
        return false;
    }

    int ret = WriteValue(fd, tid);
    if (ret < 0) {
        APP_LOGE("%{public}s(%{public}d) write freezer tid failed %{public}d.", __func__, __LINE__, errno);
        return false;
    }

    return true;
}

}  // namespace AppExecFwk
}  // namespace OHOS
